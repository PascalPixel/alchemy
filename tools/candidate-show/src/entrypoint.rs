use crate::{
    cli::{options_of, ParseOutcome, USAGE},
    diff::self_test,
    render::{render, RenderOutput},
};
use compiler_core::{
    overlay_call_via_base,
    routing::root,
    translation_units::{TranslationUnit, TranslationUnits},
};
use overlay_disasm::{canonical_overlay, OVERLAY_LINK_BIAS};
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
fn run(mut options: crate::cli::Options) -> Result<String, String> {
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
    options.source = unit.source.to_string_lossy().into_owned();
    options.configuration.absolute_symbols = unit.canonical_symbols()?;
    let work = options.work.clone().unwrap_or_else(|| {
        root()
            .join("scratch/candidate-show")
            .join(&id)
            .to_string_lossy()
            .into_owned()
    });
    options.work = Some(work.clone());
    if let Some(overlay) = &unit.overlay {
        options.overlay = Some(overlay.clone());
        options.configuration.call_via_base = Some(overlay_call_via_base(overlay));
        options.configuration.label_word_bias = Some(OVERLAY_LINK_BIAS as u64);
        let reference = overlay_reference(root(), overlay)?;
        let path = Path::new(&work).join(format!(
            "reference-{}.bin",
            compiler_core::sha256::hex(&reference)
        ));
        std::fs::create_dir_all(&work).map_err(|error| format!("{work}: {error}"))?;
        std::fs::write(&path, reference).map_err(|error| format!("{}: {error}", path.display()))?;
        options.rom = Some(path.to_string_lossy().into_owned());
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
        output.push_str(&format!("owner={address_text}\n{}", rendered.stdout));
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
            "translation unit {id} has byte mismatches in {}",
            byte_mismatches.join(",")
        ));
    }
    Ok(output)
}
fn overlay_reference(repository: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    canonical_overlay(repository, overlay)
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
    let declared = unit
        .owners
        .iter()
        .map(|entry| (entry.address, entry.extent))
        .chain(
            unit.local_symbols
                .iter()
                .map(|entry| (entry.address, entry.extent)),
        );
    let mut mismatches = Vec::new();
    for (address, extent) in declared {
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
    fn entrypoint_contracts() {
        let output = |difference| RenderOutput {
            stdout: String::new(),
            candidate_length: 4,
            reference_length: 4,
            differing_halfwords: difference,
            rows: 2,
        };
        assert!(!exact_mismatch(&output(0)));
        assert!(exact_mismatch(&output(1)));
        let repository =
            std::env::temp_dir().join(format!("candidate-show-no-rom-{}", std::process::id()));
        let error = overlay_reference(&repository, "resource_36f").unwrap_err();
        assert!(error.contains("roms/gs1-en.gba"));
        let work = std::env::temp_dir().join("alchemy-overlay-unit-test");
        let _ = std::fs::remove_dir_all(&work);
        let arguments = [
            "--unit",
            "scene-event-runtime",
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
        assert_eq!(output.matches("differing_halfwords=0").count(), 5);
        assert_eq!(output.matches("compile=fresh").count(), 1);
        assert_eq!(output.matches("compile=shared-object").count(), 4);
        let _ = std::fs::remove_dir_all(work);
    }
}
