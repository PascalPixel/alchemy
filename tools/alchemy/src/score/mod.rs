pub mod allocator;
pub mod cli;
pub mod disasm;
pub mod patch;
pub mod render;
pub mod structure;
pub mod triage;
pub mod variants;

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
fn run(options: crate::score::cli::Options) -> Result<String, String> {
    let Some(id) = options.unit.clone() else {
        let output = render(root(), &options)?;
        let twins = options.owner.map_or(String::new(), |address| {
            let owner = crate::compiler::source_paths::SourceOwner::Main(address);
            crate::score::cli::siblings_line(root(), owner, output.reference_length)
        });
        return Ok(format!("{}{twins}", output.stdout));
    };
    let manifest = TranslationUnits::load_game(root(), options.target)?;
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
    if options.instance.is_some() || options.all_instances {
        return score_instances(&unit, &options);
    }
    score_unit(&unit, options)
}
fn score_unit(
    unit: &TranslationUnit,
    mut options: crate::score::cli::Options,
) -> Result<String, String> {
    let id = &unit.id;
    if let Some(address) = options.owner {
        if !unit.owners.iter().any(|owner| owner.address == address) {
            return Err(format!("{id} does not declare 0x{address:08x}"));
        }
    }
    if let Some(overlay) = unit.overlay.clone() {
        if unit.exact() && options.patch.is_none() && !options.asm && !options.allocator_order {
            let work = options.work.as_ref().map(|work| root().join(work));
            return score_overlay_unit(&unit, &overlay, work.as_deref(), options.owner);
        }
    }
    options.source = unit.source.to_string_lossy().into_owned();
    options.configuration.absolute_symbols = unit.canonical_symbols()?;
    let default_work = format!("out/score/{id}");
    let work = options.work.clone().unwrap_or(default_work);
    let work = root().join(work).to_string_lossy().into_owned();
    options.work = Some(work.clone());
    if let Some(overlay) = &unit.overlay {
        options.overlay = Some(overlay.clone());
        options.configuration.call_via_base = Some(overlay_call_via_base(overlay));
        let reference = crate::overlay::rom::canonical_overlay_for(
            root(),
            crate::overlay::owners::production_target(options.target),
            overlay,
        )?;
        let path = Path::new(&work).join(format!(
            "reference-{}.bin",
            crate::compiler::sha256::hex(&reference)
        ));
        std::fs::create_dir_all(&work).map_err(|error| format!("{work}: {error}"))?;
        std::fs::write(&path, reference).map_err(|error| format!("{}: {error}", path.display()))?;
        options.rom = Some(path.to_string_lossy().into_owned());
    }
    let selected_owner = options.owner;
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
fn score_overlay_unit(
    unit: &TranslationUnit,
    overlay: &str,
    work: Option<&Path>,
    first: Option<u32>,
) -> Result<String, String> {
    let (output, mismatches) = score_overlay_image(unit, overlay, None, work, false, first)?;
    if !mismatches.is_empty() && unit.exact() {
        return Err(format!(
            "{output}translation unit {} has byte mismatches in {}",
            unit.id,
            mismatches.join(",")
        ));
    }
    Ok(output)
}
/// `--instance IMAGE` or `--all-instances`: the unit linked into each image
/// as production links it, every owner compared over its complete extent in
/// its own image, including an instance's trailing alignment halfword.
fn score_instances(
    unit: &TranslationUnit,
    options: &crate::score::cli::Options,
) -> Result<String, String> {
    if unit.overlay.is_none() || !unit.exact() {
        return Err(format!(
            "{}: instances belong to wholly exact overlay units",
            unit.id
        ));
    }
    let images = match &options.instance {
        Some(image) if unit.instance(image).is_some() => vec![image.as_str()],
        Some(image) => {
            let instances = unit.instances.keys().cloned().collect::<Vec<_>>();
            return Err(format!(
                "{}: no instance links it into {image} (instances: {})",
                unit.id,
                instances.join(", ")
            ));
        }
        None => unit.images().collect(),
    };
    let work = options.work.as_ref().map(|work| root().join(work));
    report_images(unit, &images, |image| {
        if image == "main" {
            let placed = unit.main_placement()?.ok_or("missing main instance")?;
            let mut options = options.clone();
            options.instance = None;
            options.all_instances = false;
            options.owner = None;
            options.work = work
                .as_ref()
                .map(|work| work.join("main").to_string_lossy().into_owned());
            return score_unit(&placed, options)
                .map(|output| (format!("image=main\n{output}"), Vec::new()));
        }
        let work = work.as_ref().map(|work| work.join(image));
        score_overlay_image(unit, image, None, work.as_deref(), true, None)
    })
}
/// Every owner of every image, then one summary line; an error when any
/// owner differs.
fn report_images(
    unit: &TranslationUnit,
    images: &[&str],
    score: impl Fn(&str) -> Result<(String, Vec<String>), String>,
) -> Result<String, String> {
    let (mut output, mut mismatches, mut owners) = (String::new(), Vec::new(), 0);
    for image in images {
        let (scored, differing) = score(image)?;
        output.push_str(&scored);
        owners += unit.owners_in(image).count();
        mismatches.extend(
            differing
                .into_iter()
                .map(|owner| format!("{image}:{owner}")),
        );
    }
    output.push_str(&format!(
        "images={} owners={owners} differing_owners={}\n",
        images.len(),
        mismatches.len()
    ));
    if !mismatches.is_empty() {
        return Err(format!(
            "{output}translation unit {} has byte mismatches in {}",
            unit.id,
            mismatches.join(",")
        ));
    }
    Ok(output)
}
/// One image's owners of `unit`, compiled from `candidate` or the unit
/// source: the score lines and the owners whose bytes differ.
pub(crate) fn score_overlay_image(
    unit: &TranslationUnit,
    image: &str,
    candidate: Option<&Path>,
    work: Option<&Path>,
    label: bool,
    first: Option<u32>,
) -> Result<(String, Vec<String>), String> {
    let compiled = crate::overlay::compile::compile_unit_in_image(unit, image, candidate, work)?;
    // Each game's units compare with the overlay its own ROM loads.
    let reference = crate::overlay::rom::canonical_overlay_for(
        root(),
        crate::overlay::owners::production_target(unit.target()?),
        image,
    )?;
    compare_image_owners(unit, image, &compiled, &reference, label, first)
}
/// Each owner `image` links, compiled against its complete extent in that
/// image's resource-form reference; `first` puts one owner's lines first,
/// where a caller reading one score line finds them.
fn compare_image_owners(
    unit: &TranslationUnit,
    image: &str,
    compiled: &crate::overlay::compile::Compiled,
    reference: &[u8],
    label: bool,
    first: Option<u32>,
) -> Result<(String, Vec<String>), String> {
    let base = 0x0200_0000i64;
    let mut output = String::new();
    let mut mismatches = Vec::new();
    let mut owners = unit.owners_in(image).collect::<Vec<_>>();
    owners.sort_by_key(|owner| Some(owner.address) != first);
    // A unit's data section links only into its canonical overlay.
    let data = unit
        .data
        .filter(|_| unit.overlay.as_deref() == Some(image))
        .map(|data| (data.address, data.extent, "data"));
    let members = owners
        .into_iter()
        .map(|owner| (owner.address, owner.extent, "owner"))
        .chain(data);
    for (address, extent, scope) in members {
        let address = i64::from(address);
        let offset = usize::try_from(address - compiled.address)
            .map_err(|_| format!("{}: {scope} precedes the compiled unit", unit.id))?;
        let candidate = compiled
            .data
            .get(offset..offset + extent)
            .ok_or_else(|| format!("{}: compiled unit lacks 0x{address:08x}", unit.id))?;
        let start = usize::try_from(address - base)
            .map_err(|_| format!("{}: {scope} precedes the overlay image", unit.id))?;
        let expected = reference
            .get(start..start + extent)
            .ok_or_else(|| format!("{}: overlay image lacks 0x{address:08x}", unit.id))?;
        let differing = candidate
            .chunks(2)
            .zip(expected.chunks(2))
            .filter(|(a, b)| a != b)
            .count();
        let image_line = match label {
            true => format!("image={image}\n"),
            false => String::new(),
        };
        output.push_str(&format!(
            "scope=translation-unit\n{image_line}{scope}=0x{address:08x}\ncandidate={} reference={} differing_halfwords={differing}\n",
            candidate.len(),
            expected.len()
        ));
        if differing != 0 {
            if let Some(first) = candidate.iter().zip(expected).position(|(a, b)| a != b) {
                output.push_str(&format!("first_difference=+0x{first:x}\n"));
            }
            mismatches.push(format!("0x{address:08x}"));
        }
        output.push_str(&format!(
            "halfword_edits={}\n",
            psynergy::compare::edit_distance(candidate, expected, 2)
        ));
    }
    Ok((output, mismatches))
}

pub(crate) fn exact_mismatch(output: &RenderOutput) -> bool {
    output.differing_halfwords != 0
        || output.candidate_length != output.reference_length
        || (output.candidate_length == 0
            && !output
                .stdout
                .contains("\nclass=exact wrong_instructions=0\n"))
}
pub(crate) fn validate_layout(
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
        let owner = unit.source_owner(unit.image(), address)?;
        let symbol = owner.legacy_name();
        let offset = address
            .checked_sub(base)
            .ok_or_else(|| format!("{} precedes its translation unit", owner.id()))?;
        let found = crate::compiler::symbols::function_symbol_fields(&rows, &symbol).is_some_and(
            |fields| {
                u32::from_str_radix(fields[0], 16).ok() == Some(offset)
                    && usize::from_str_radix(fields[1], 16).ok() == Some(extent)
            },
        );
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
    fn lost_age_selected_overlay_member_still_uses_the_complete_unit() {
        if !root().join("roms/tla-en.gba").is_file() {
            return;
        }
        let mut options = crate::score::cli::Options::tbs(String::new());
        options.target = crate::compiler::routing::CompilerTarget::Tla;
        options.unit = Some("venus-lighthouse-approach-scene".into());
        options.owner = Some(0x02000038);
        options.work = Some("out/tla-en/score-unit-route-test".into());
        let output = run(options).unwrap();
        assert_eq!(output.matches("differing_halfwords=0").count(), 7);
        assert!(output.starts_with("scope=translation-unit\nowner=0x02000038\n"));
    }

    #[test]
    fn score_unit_all_instances_reports_every_owner_including_alignment_halfword() {
        use crate::compiler::translation_units::fixture::Repository;
        use crate::overlay::compile::Compiled;
        let repository = Repository::new();
        let unit = repository.load().unwrap().units.remove(0);
        // Resource-form images whose owners hold a pattern of their offsets;
        // FieldScene_FindActorRegion's last halfword in resource_39b is fill.
        let reference = |image: &str| {
            let mut bytes = vec![0u8; 0x1000];
            for owner in unit.owners_in(image) {
                let start = (owner.address - 0x0200_0000) as usize;
                for (index, byte) in bytes[start..start + owner.extent].iter_mut().enumerate() {
                    *byte = (start + index) as u8 | 1;
                }
            }
            if image == "resource_39b" {
                bytes[0x0630 + 294..0x0630 + 296].fill(0);
            }
            bytes
        };
        // The unit as compiled for one image: one span from its first owner.
        let compiled = |image: &str, fill: [u8; 2]| {
            let owners = unit.owners_in(image).collect::<Vec<_>>();
            let first = owners[0].address as usize - 0x0200_0000;
            let last = owners[owners.len() - 1];
            let end = last.address as usize - 0x0200_0000 + last.extent;
            let mut data = reference(image)[first..end].to_vec();
            if image == "resource_39b" {
                data[294..296].copy_from_slice(&fill);
            }
            Compiled {
                address: i64::from(owners[0].address),
                data,
            }
        };
        let images = unit.images().collect::<Vec<_>>();
        let report = |fill: [u8; 2]| {
            report_images(&unit, &images, |image| {
                compare_image_owners(
                    &unit,
                    image,
                    &compiled(image, fill),
                    &reference(image),
                    true,
                    None,
                )
            })
        };
        let output = report([0, 0]).unwrap();
        assert_eq!(output.matches("scope=translation-unit\nimage=").count(), 6);
        for (image, owner, extent) in [
            ("resource_3bf", "0x0200034c", 296),
            ("resource_3bf", "0x020008c0", 284),
            ("resource_389", "0x0200034c", 296),
            ("resource_389", "0x020008c0", 284),
            ("resource_39b", "0x02000630", 296),
            ("resource_39b", "0x02000ba4", 284),
        ] {
            let line = format!(
                "image={image}\nowner={owner}\ncandidate={extent} reference={extent} differing_halfwords=0\n"
            );
            assert!(output.contains(&line), "{line}{output}");
        }
        assert!(
            output.ends_with("images=3 owners=6 differing_owners=0\n"),
            "{output}"
        );
        // Modern NOP fill in the alignment halfword is a real difference.
        let error = report([0xc0, 0x46]).unwrap_err();
        assert!(
            error.contains("image=resource_39b\nowner=0x02000630\ncandidate=296 reference=296 differing_halfwords=1\nfirst_difference=+0x126\n"),
            "{error}"
        );
        assert!(
            error.contains("images=3 owners=6 differing_owners=1\n"),
            "{error}"
        );
        assert!(
            error.ends_with(
                "translation unit staged-actor has byte mismatches in resource_39b:0x02000630"
            ),
            "{error}"
        );
        // A standalone score reads the first score line: the selected owner's.
        let redraw = 0x0200_0ba4;
        let (lines, _) = compare_image_owners(
            &unit,
            "resource_39b",
            &compiled("resource_39b", [0, 0]),
            &reference("resource_39b"),
            true,
            Some(redraw),
        )
        .unwrap();
        assert!(
            lines.starts_with("scope=translation-unit\nimage=resource_39b\nowner=0x02000ba4\n"),
            "{lines}"
        );
        assert_eq!(lines.matches("owner=0x02000630").count(), 1);
    }
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
                        owner.state == crate::compiler::translation_units::OwnerState::NotYetC
                    })
            })
            .expect("a retained overlay unit exists");
        let owner = unit
            .owners
            .iter()
            .find(|owner| owner.state == crate::compiler::translation_units::OwnerState::NotYetC)
            .unwrap();
        let work = tempfile::tempdir().unwrap();
        let mut options = crate::score::cli::Options::tbs(String::new());
        options.unit = Some(unit.id.clone());
        options.work = Some(work.path().to_string_lossy().into_owned());
        options.first = true;
        let output = run(options).unwrap();
        assert!(output.contains("scope=translation-unit"));
        assert!(output.contains(&format!("owner=0x{:08x}", owner.address)));
        assert!(output.contains("differing_halfwords="));
    }
    #[test]
    #[ignore = "reads the local ROMs and compiles real owners; make test-integration"]
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
        let error =
            crate::overlay::rom::canonical_overlay(&repository, "resource_36f").unwrap_err();
        assert!(error.contains("roms/tbs-en.gba"));
        let work = root().join("out/diff-unit-test");
        let _ = std::fs::remove_dir_all(&work);
        std::fs::create_dir_all(&work).unwrap();
        let patch = work.join("unit-relative-include.patch");
        std::fs::write(&patch, "diff --git a/MOTION_PARTICLE.C b/MOTION_PARTICLE.C\n--- a/MOTION_PARTICLE.C\n+++ b/MOTION_PARTICLE.C\n@@ -1 +1 @@\n-#include \"TYPES.H\"\n+#include \"../../../INCLUDE/TYPES.H\"\n").unwrap();
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
            .join("try/games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/MOTION_PARTICLE.C");
        assert!(std::fs::read_to_string(staged)
            .unwrap()
            .contains("../../../INCLUDE/TYPES.H"));
        assert_eq!(output.matches("scope=translation-unit").count(), 1);
        assert_eq!(output.matches("owner=0x02000074").count(), 1);
        assert_eq!(output.matches("differing_halfwords=0").count(), 1);
        assert_eq!(output.matches("compile=fresh").count(), 0);
        assert_eq!(output.matches("compile=shared-object").count(), 1);
        let _ = std::fs::remove_dir_all(work);
    }
}
