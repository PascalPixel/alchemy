//! The two functions that touch `asm/`: the assemble-and-measure check and the
//! retained-stem census.

use std::collections::BTreeSet;
use std::path::Path;
use std::process::Command;

use crate::jsnum::{js_number, to_int32};
use crate::jsstring::{js_trim, split_crlf_lines};
use crate::patterns;
use crate::paths;

/// `replaceableAssembly(stem, linkedSize, scratch)`.
///
/// Assembles `asm/<stem>.s`, extracts `.text`, and reports whether it is
/// exactly `linked_size` bytes. A missing source is vacuously replaceable, and
/// any toolchain failure is not.
pub fn replaceable_assembly(stem: &str, linked_size: usize, scratch: &Path) -> bool {
    let source = paths::asm_dir().join(format!("{stem}.s"));
    if !source.exists() {
        return true;
    }
    let object = scratch.join(format!("{stem}.asmcheck.o"));
    let binary = scratch.join(format!("{stem}.asmcheck.bin"));

    let assembled = Command::new("arm-none-eabi-as")
        .args(["-mcpu=arm7tdmi", "-mthumb-interwork", "-o"])
        .arg(&object)
        .arg(&source)
        .output();
    // PORT NOTE: `Bun.spawnSync` on a missing binary yields a non-zero
    // `exitCode` rather than throwing, so the TypeScript reports "not
    // replaceable" when the assembler is absent. A spawn error maps to the same
    // answer here rather than to a panic.
    match assembled {
        Ok(result) if result.status.success() => {}
        _ => return false,
    }

    let copied = Command::new("arm-none-eabi-objcopy")
        .args(["-O", "binary", "-j", ".text"])
        .arg(&object)
        .arg(&binary)
        .output();
    match copied {
        Ok(result) if result.status.success() => {}
        _ => return false,
    }

    match std::fs::read(&binary) {
        Ok(bytes) => bytes.len() == linked_size,
        Err(_) => false,
    }
}

/// `retainedAssemblyStems()`.
///
/// The explicitly classified modules, plus every long-call veneer recognised
/// from its four-row canonical shape.
///
/// PORT NOTE: the return type is a `BTreeSet`, not a `HashSet`. Membership is
/// the only operation the caller performs, so JavaScript's insertion order is
/// not observable here, but a `HashSet` would still make the debug output and
/// any future iteration non-deterministic between runs. `BTreeSet` is ordered
/// and free of that hazard.
pub fn retained_assembly_stems() -> Result<BTreeSet<String>, String> {
    let asm = paths::asm_dir();
    let mut result = BTreeSet::new();

    let classification_path = asm.join("classification.json");
    let text = std::fs::read_to_string(&classification_path)
        .map_err(|error| format!("cannot read {}: {error}", classification_path.display()))?;
    let document = crate::json::parse(&text)
        .map_err(|error| format!("cannot parse {}: {error}", classification_path.display()))?;
    if let Some(groups) = document.get("groups").and_then(|value| value.as_array()) {
        for group in groups {
            if let Some(files) = group.get("files").and_then(|value| value.as_array()) {
                for stem in files {
                    if let Some(name) = stem.as_str() {
                        result.insert(name.to_lowercase());
                    }
                }
            }
        }
    }

    let mut names: Vec<String> = Vec::new();
    let entries = std::fs::read_dir(&asm)
        .map_err(|error| format!("cannot read {}: {error}", asm.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("cannot read {}: {error}", asm.display()))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if patterns::is_address_asm_name(&name) {
            names.push(name);
        }
    }
    // `readdirSync` order is not defined; sorting makes this port's traversal
    // reproducible without changing the resulting set.
    names.sort();

    for name in names {
        let stem = &name[..name.len() - 2];
        let body = std::fs::read_to_string(asm.join(&name))
            .map_err(|error| format!("cannot read {name}: {error}"))?;
        let rows: Vec<String> = split_crlf_lines(&body)
            .into_iter()
            .map(|row| js_trim(patterns::strip_comment(row)).to_string())
            .filter(|row| !row.is_empty())
            .collect();
        let needle = format!("Func_{stem}:");
        let Some(entry) = rows.iter().position(|row| *row == needle) else {
            continue;
        };
        // PORT NOTE: `entry + 3 >= rows.length` skips the row it is about to
        // read, so a file whose literal is the very last row is skipped. The
        // off-by-one is reproduced.
        if entry + 3 >= rows.len() {
            continue;
        }
        let load = patterns::is_veneer_load(&rows[entry + 1]);
        let branch = patterns::is_veneer_branch(&rows[entry + 2]);
        let Some(literal) = patterns::asm_literal_row(&rows[entry + 3]) else {
            continue;
        };
        // PORT NOTE (trap: `parseInt(text, 16)` versus `from_str_radix`). The
        // TypeScript uses `Number(literal)`, which *requires* the `0x` prefix
        // the pattern captured; `i64::from_str_radix(literal, 16)` would reject
        // that same prefix outright. Both go through `js_number`, the crate's
        // single numeric parse. The `& 1` is a JavaScript bitwise operator, so
        // it is `ToInt32`, not `as i32`.
        if load && branch && (to_int32(js_number(literal)) & 1) != 0 {
            result.insert(stem.to_string());
        }
    }
    Ok(result)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::jsnum::{js_number, to_int32};

    #[test]
    fn the_thumb_bit_test_survives_a_thirty_two_bit_wrap() {
        // A literal above 2^31 wraps rather than saturating.
        assert_eq!(to_int32(js_number("0xFFFFFFFF")) & 1, 1);
        assert_eq!(to_int32(js_number("0x80000000")) & 1, 0);
        assert_eq!(to_int32(js_number("0x08001235")) & 1, 1);
        assert_eq!(to_int32(js_number("0x08001234")) & 1, 0);
    }

    #[test]
    fn a_missing_source_is_vacuously_replaceable() {
        let scratch = crate::test_support::fixture_dir("missing-source");
        assert!(replaceable_assembly(
            "ffffffff_no_such_stem",
            0,
            &scratch
        ));
    }
}
