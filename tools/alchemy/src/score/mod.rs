pub mod allocator;
pub mod cli;
pub mod disasm;
pub mod patch;
pub mod render;
pub mod structure;
pub mod triage;

use crate::score::{
    cli::{options_of, ParseOutcome, USAGE},
    render::{render, RenderOutput},
};
use psynergy::compare::differing_offsets;

fn self_test() -> Result<String, String> {
    if differing_offsets(&[0, 1, 2], &[0, 1, 2], 2).is_empty()
        && differing_offsets(&[0, 1, 2], &[0, 1], 2) == [2].into()
    {
        Ok("candidate show self-test passed".into())
    } else {
        Err("candidate show self-test failed".into())
    }
}
use crate::compiler::{
    routing::root,
    symbols::overlay_call_via_base,
    translation_units::{TranslationUnit, TranslationUnits},
};
use crate::overlay::rom::canonical_overlay;
use std::path::Path;
use std::process::Command;
pub fn entry(arguments: &[String]) {
    if arguments.iter().any(|arg| arg == "--self-test") {
        match self_test() {
            Ok(line) => println!("{line}"),
            Err(error) => fail(&error),
        }
        return;
    }
    match options_of(root(), arguments) {
        Ok(ParseOutcome::Help) => println!("{USAGE}"),
        Ok(ParseOutcome::Options(options)) => match run(*options) {
            Ok(output) => print!("{output}"),
            Err(error) => fail(&error),
        },
        Err(error) => fail(&error),
    }
}
fn run(mut options: crate::score::cli::Options) -> Result<String, String> {
    let Some(id) = options.unit.clone() else {
        return render(root(), &options).map(|output| output.stdout);
    };
    let manifest = TranslationUnits::load(root())?;
    let unit = manifest
        .unit(&id)
        .ok_or_else(|| format!("unknown translation unit {id}"))?
        .clone();
    if unit.target()? != options.target {
        return Err(format!(
            "translation unit {id} belongs to {}, not {}",
            unit.game,
            options.target.as_str()
        ));
    }
    if let Some(overlay) = unit.overlay.clone() {
        if options.owner.is_none() && unit.exact() {
            return score_overlay_unit(&unit, &overlay);
        }
    }
    options.source = unit.source.to_string_lossy().into_owned();
    options.configuration.absolute_symbols = unit.canonical_symbols()?;
    let default_work = format!("scratch/score/{id}");
    let work = options.work.clone().unwrap_or(default_work);
    let work = root().join(work).to_string_lossy().into_owned();
    options.work = Some(work.clone());
    if let Some(overlay) = &unit.overlay {
        options.overlay = Some(overlay.clone());
        options.configuration.call_via_base = Some(overlay_call_via_base(overlay));
        let reference = canonical_overlay(root(), overlay)?;
        let path = Path::new(&work).join(format!(
            "reference-{}.bin",
            crate::compiler::sha256::hex(&reference)
        ));
        std::fs::create_dir_all(&work).map_err(|error| format!("{work}: {error}"))?;
        std::fs::write(&path, reference).map_err(|error| format!("{}: {error}", path.display()))?;
        options.rom = Some(path.to_string_lossy().into_owned());
    }
    let selected_owner = options.owner;
    if let Some(address) = selected_owner {
        if !unit.owners.iter().any(|owner| owner.address == address) {
            return Err(format!("{id} does not declare 0x{address:08x}"));
        }
    }
    let mut output = String::new();
    let mut layout_mismatches = Vec::new();
    let exact_unit = unit.exact();
    let mut byte_mismatches = Vec::new();
    for (index, owner) in unit.owners.iter().enumerate() {
        let address = owner.address;
        let address_text = format!("0x{address:08x}");
        options.owner = Some(address);
        options.size = Some(owner.extent);
        options.configuration.overlay_extent = unit.overlay.as_ref().map(|_| owner.extent);
        if index == 1 {
            options.precompiled_object = Some(
                Path::new(&work)
                    .join(format!("{:08x}.o", unit.owners[0].address))
                    .to_string_lossy()
                    .into_owned(),
            );
        }
        let rendered = render(root(), &options)?;
        if exact_unit && exact_mismatch(&rendered) {
            byte_mismatches.push(address_text.clone());
        }
        if index == 0 {
            layout_mismatches = validate_layout(
                &unit,
                &Path::new(&work).join(format!("{:08x}.o", address)),
                address,
            )?;
            if !layout_mismatches.is_empty() && exact_unit {
                return Err(format!(
                    "translation unit {id} has {} symbol offset mismatches",
                    layout_mismatches.len()
                ));
            }
        }
        if selected_owner.is_none() || selected_owner == Some(address) {
            output.push_str(&format!(
                "scope=translation-unit\nowner={address_text}\n{}",
                rendered.stdout
            ));
        }
    }
    if !layout_mismatches.is_empty() {
        output.push_str(&format!(
            "layout_mismatches={} owners={}\n",
            layout_mismatches.len(),
            layout_mismatches.join(",")
        ));
    }
    if !byte_mismatches.is_empty() {
        return Err(format!(
            "{output}translation unit {id} has byte mismatches in {}",
            byte_mismatches.join(",")
        ));
    }
    Ok(output)
}
/// An overlay unit is proved the way the full build places it: the unit
/// compiles once, every function is linked at its owner's address, and each
/// member's bytes are compared with the canonical overlay image.
fn score_overlay_unit(unit: &TranslationUnit, overlay: &str) -> Result<String, String> {
    let compiled = crate::overlay::compile::compile_declared_overlay_unit(unit, "en", None)?;
    let reference = canonical_overlay(root(), overlay)?;
    let base = 0x0200_0000i64;
    let mut output = String::new();
    let mut mismatches = Vec::new();
    for owner in &unit.owners {
        let address = i64::from(owner.address);
        let offset = usize::try_from(address - compiled.address)
            .map_err(|_| format!("{}: owner precedes the compiled unit", unit.id))?;
        let candidate = compiled
            .data
            .get(offset..offset + owner.extent)
            .ok_or_else(|| format!("{}: compiled unit lacks 0x{address:08x}", unit.id))?;
        let start = usize::try_from(address - base)
            .map_err(|_| format!("{}: owner precedes the overlay image", unit.id))?;
        let expected = reference
            .get(start..start + owner.extent)
            .ok_or_else(|| format!("{}: overlay image lacks 0x{address:08x}", unit.id))?;
        let differing = candidate
            .chunks(2)
            .zip(expected.chunks(2))
            .filter(|(a, b)| a != b)
            .count();
        output.push_str(&format!(
            "scope=translation-unit\nowner=0x{address:08x}\ncandidate={} reference={} differing_halfwords={differing}\n",
            candidate.len(),
            expected.len()
        ));
        if differing != 0 {
            if let Some(first) = candidate.iter().zip(expected).position(|(a, b)| a != b) {
                output.push_str(&format!("first_difference=+0x{first:x}\n"));
            }
            mismatches.push(format!("0x{address:08x}"));
        }
    }
    if !mismatches.is_empty() && unit.exact() {
        return Err(format!(
            "{output}translation unit {} has byte mismatches in {}",
            unit.id,
            mismatches.join(",")
        ));
    }
    Ok(output)
}

fn exact_mismatch(output: &RenderOutput) -> bool {
    output.differing_halfwords != 0
        || output.candidate_length != output.reference_length
        || (output.candidate_length == 0
            && !output
                .stdout
                .contains("\nclass=exact wrong_instructions=0\n"))
}
fn validate_layout(
    unit: &TranslationUnit,
    object: &Path,
    base: u32,
) -> Result<Vec<String>, String> {
    let output = Command::new("arm-none-eabi-nm")
        .args(["-S", "--defined-only"])
        .arg(object)
        .output()
        .map_err(|error| format!("arm-none-eabi-nm failed: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "cannot inspect translation unit {}",
            object.display()
        ));
    }
    let rows = String::from_utf8_lossy(&output.stdout);
    let mut mismatches = Vec::new();
    for (address, _, extent) in unit.symbols() {
        let owner = unit.source_owner(address)?;
        let symbol = owner.legacy_name();
        let offset = address
            .checked_sub(base)
            .ok_or_else(|| format!("{} precedes its translation unit", owner.id()))?;
        let found = rows.lines().any(|row| {
            let fields: Vec<_> = row.split_whitespace().collect();
            fields
                .first()
                .and_then(|field| u32::from_str_radix(field, 16).ok())
                == Some(offset)
                && fields
                    .get(1)
                    .and_then(|field| usize::from_str_radix(field, 16).ok())
                    == Some(extent)
                && fields.last() == Some(&symbol.as_str())
        });
        if !found {
            mismatches.push(owner.id());
        }
    }
    Ok(mismatches)
}
fn fail(message: &str) -> ! {
    eprintln!("{message}");
    std::process::exit(1)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn retained_overlay_unit_scores_without_an_owner_override() {
        // Any overlay unit that is retained right now: naming one would break
        // the test every time that owner is adopted.
        let manifest = crate::compiler::translation_units::TranslationUnits::load(root()).unwrap();
        let unit = manifest
            .units
            .iter()
            .find(|unit| {
                unit.overlay.is_some()
                    && unit.owners.iter().any(|owner| {
                        owner.state
                            == crate::compiler::translation_units::OwnerState::RetainedAssembly
                    })
            })
            .expect("a retained overlay unit exists");
        let owner = unit
            .owners
            .iter()
            .find(|owner| {
                owner.state == crate::compiler::translation_units::OwnerState::RetainedAssembly
            })
            .unwrap();
        let work = tempfile::tempdir().unwrap();
        let mut options = crate::score::cli::Options::gs1(String::new());
        options.unit = Some(unit.id.clone());
        options.work = Some(work.path().to_string_lossy().into_owned());
        options.first = true;
        let output = run(options).unwrap();
        assert!(output.contains("scope=translation-unit"));
        assert!(output.contains(&format!("owner=0x{:08x}", owner.address)));
        assert!(output.contains("differing_halfwords="));
    }
    #[test]
    fn entrypoint_contracts() {
        let output = |difference| RenderOutput {
            stdout: String::new(),
            candidate_length: 4,
            reference_length: 4,
            differing_halfwords: difference,
            allocator: None,
            residual: crate::score::triage::classify(&[], &[], 4, 4, difference),
        };
        assert!(!exact_mismatch(&output(0)));
        assert!(exact_mismatch(&output(1)));
        let repository = std::env::temp_dir().join(format!("diff-no-rom-{}", std::process::id()));
        let error = canonical_overlay(&repository, "resource_36f").unwrap_err();
        assert!(error.contains("roms/gs1-en.gba"));
        let work = root().join("out/diff-unit-test");
        let _ = std::fs::remove_dir_all(&work);
        std::fs::create_dir_all(&work).unwrap();
        let patch = work.join("unit-relative-include.patch");
        std::fs::write(&patch, "diff --git a/motion_particle.c b/motion_particle.c\n--- a/motion_particle.c\n+++ b/motion_particle.c\n@@ -1 +1 @@\n-#include \"types.h\"\n+#include \"../../../../include/types.h\"\n").unwrap();
        let arguments = [
            "--unit",
            "actor-motion-particle-scene",
            "--owner",
            "02000074",
            "--patch",
            patch.to_str().unwrap(),
            "--first",
            "--work",
            work.to_str().unwrap(),
        ]
        .into_iter()
        .map(str::to_string)
        .collect::<Vec<_>>();
        let ParseOutcome::Options(options) = options_of(root(), &arguments).unwrap() else {
            panic!("expected options")
        };
        let output = run(*options).unwrap();
        let staged = root()
            .join(&work)
            .join("try/games/gs1/src/overlays/scene/actor/motion_particle.c");
        assert!(std::fs::read_to_string(staged)
            .unwrap()
            .contains("../../../../include/types.h"));
        assert_eq!(output.matches("scope=translation-unit").count(), 1);
        assert_eq!(output.matches("owner=0x02000074").count(), 1);
        assert_eq!(output.matches("differing_halfwords=0").count(), 1);
        assert_eq!(output.matches("compile=fresh").count(), 0);
        assert_eq!(output.matches("compile=shared-object").count(), 1);
        let _ = std::fs::remove_dir_all(work);
    }
}
