//! `main()`.

use std::path::Path;
use std::process::Command;

use alchemy_routing::routing::CompilerTarget;
use match_m2c::jsnum::to_js_number_string;
use match_m2c::verify::{js_subarray, verify_candidate, CandidateCompilerFamily, ROM_BASE};

use crate::cli::Options;
use crate::diff::differing_offsets;
use crate::disasm::disassemble;
use crate::extent::linked_function_extent;
use crate::jsparse::{js_parse_int_radix, pad_end, pad_start_zero, slice_utf16};
use crate::manifest::{basename_without, region_size};

/// The whole document `main()` prints, plus the intermediates the parity
/// harness wants to look at.
#[derive(Debug)]
pub struct RenderOutput {
    pub stdout: String,
    pub candidate_length: usize,
    pub reference_length: usize,
    pub differing_halfwords: usize,
    /// True when the side-by-side table has at least one row, i.e. the
    /// disassembly branch actually ran.
    pub rows: usize,
}

/// `main()` minus the argument parsing.
pub fn render(root: &Path, options: &Options) -> Result<RenderOutput, String> {
    let rom_path = options
        .rom
        .as_deref()
        .ok_or_else(|| "The \"path\" argument must be of type string. Received undefined".to_string())?;
    let work = options
        .work
        .as_deref()
        .ok_or_else(|| "The \"path\" argument must be of type string. Received undefined".to_string())?;
    let rom = std::fs::read(rom_path).map_err(|error| format!("{rom_path}: {error}"))?;
    std::fs::create_dir_all(work).map_err(|error| format!("{work}: {error}"))?;

    let verification = verify_candidate(
        &options.source,
        &rom,
        work,
        &options.flags,
        ROM_BASE,
        CompilerTarget::Gs1,
        &options.configuration,
    )?;

    // PORT NOTE -- `basename(source, ".c")`, NOT `match_m2c`'s `sourceStem`,
    // which strips `extname(path)` whatever it is. For `x.cpp` the two
    // disagree and this file uses the literal `.c` form.
    let stem = basename_without(&options.source, ".c").to_string();
    // PORT NOTE -- `Number.parseInt(stem, 16)` with NO validating guard, unlike
    // `match_m2c`'s `parseHex`. It tolerates a `0x` prefix and stops at the
    // first non-hex character. Routed through the one helper.
    let address = js_parse_int_radix(&stem, 16);

    let linked_path = Path::new(work).join(format!("{stem}.bin"));
    let linked = std::fs::read(&linked_path)
        .map_err(|error| format!("{}: {error}", linked_path.display()))?;
    let elf = Path::new(work).join(format!("{stem}.elf"));
    let symbols = Command::new("arm-none-eabi-nm")
        .args(["-S", "--defined-only"])
        .arg(&elf)
        .output()
        .map_err(|error| format!("arm-none-eabi-nm failed: {error}"))?;

    // PORT NOTE -- the gcc2951 exclusion and the `exitCode === 0` test are both
    // load-bearing: a failed `nm` falls back to the entry symbol's own size
    // rather than erroring.
    let extent = if symbols.status.success()
        && options.configuration.family != Some(CandidateCompilerFamily::Gcc2951)
    {
        let text = String::from_utf8_lossy(&symbols.stdout);
        linked_function_extent(&text, &format!("Func_{stem}"), address, linked.len() as f64)?
    } else {
        verification.actual.len() as f64
    };

    // Promoted sources may own a complete multi-function span after their
    // reference `.s` row has been removed. In that case the linked extent is
    // the authoritative candidate boundary; the verification object only knows
    // the entry symbol's head size.
    let expected_size = region_size(root, &stem).unwrap_or(linked.len() as f64);

    // PORT NOTE -- `subarray` CLAMPS. `address - ROM_BASE` is hugely negative
    // for an overlay address, which JavaScript resolves from the END of the
    // ROM, not from 0 (`js_subarray` implements the real `ToIntegerOrInfinity`
    // rule). Rust `&rom[a..b]` would panic. Reproduced, not fixed.
    let actual = js_subarray(&linked, 0.0, extent);
    let begin = address - ROM_BASE;
    let expected = js_subarray(&rom, begin, begin + expected_size);

    let candidate_path = Path::new(work).join("candidate.bin");
    let reference_path = Path::new(work).join("reference.bin");
    std::fs::write(&candidate_path, &actual)
        .map_err(|error| format!("{}: {error}", candidate_path.display()))?;
    std::fs::write(&reference_path, &expected)
        .map_err(|error| format!("{}: {error}", reference_path.display()))?;

    let left = disassemble(&candidate_path.to_string_lossy(), 0.0)?;
    let right = disassemble(&reference_path.to_string_lossy(), 0.0)?;
    let differing = differing_offsets(&actual, &expected);

    // `[...new Set([...left.keys(), ...right.keys()])].sort((a, b) => a - b)`:
    // dedupe keeping FIRST occurrence, then a numeric sort.
    let mut offsets: Vec<f64> = Vec::new();
    for key in left.keys().chain(right.keys()) {
        if !offsets.iter().any(|existing| same_value_zero(*existing, key)) {
            offsets.push(key);
        }
    }
    offsets.sort_by(js_numeric_comparator);

    let mut stdout = String::new();
    // PORT NOTE -- all three counts are integral JavaScript numbers, so
    // `toFixed`/ryu are not involved; every emitted number still goes through
    // `to_js_number_string`, and `tests/formatting.rs` walks the whole document
    // to prove no `1.0`-shaped token can appear.
    stdout.push_str(&format!(
        "candidate={} reference={} differing_halfwords={}\n",
        to_js_number_string(actual.len() as f64)?,
        to_js_number_string(expected.len() as f64)?,
        to_js_number_string(differing.len() as f64)?,
    ));
    stdout.push_str("      offset  candidate                      reference\n");
    for offset in &offsets {
        let mark = if offset_is_differing(&differing, *offset) {
            "!"
        } else {
            " "
        };
        let a = slice_utf16(&pad_end(left.get(*offset).unwrap_or(""), 30), 30);
        let b = right.get(*offset).unwrap_or("");
        stdout.push_str(&format!(
            "  {mark} {}  {a} {b}\n",
            pad_start_zero(&hex_lower(*offset), 4)
        ));
    }

    Ok(RenderOutput {
        stdout,
        candidate_length: actual.len(),
        reference_length: expected.len(),
        differing_halfwords: differing.len(),
        rows: offsets.len(),
    })
}

/// `differing.has(offset)` where the set holds byte offsets and the map key is
/// a JavaScript number.
///
/// PORT NOTE -- a non-integral or negative key can never match, exactly as
/// `Set<number>.has` would fail to find it.
fn offset_is_differing(differing: &std::collections::BTreeSet<usize>, offset: f64) -> bool {
    if offset < 0.0 || offset.fract() != 0.0 || offset > usize::MAX as f64 {
        return false;
    }
    differing.contains(&(offset as usize))
}

/// `Set` membership, which is SameValueZero, not `==`.
///
/// PORT NOTE -- clippy proposes `Vec::contains` here, which is `PartialEq` and
/// therefore treats NaN as never equal, so `new Set([NaN, NaN]).size === 1`
/// would become two entries. `+0` and `-0` are also the same key in a `Set`
/// while being distinct under `total_cmp`. Extracted as a named helper rather
/// than silenced.
fn same_value_zero(left: f64, right: f64) -> bool {
    if left.is_nan() && right.is_nan() {
        return true;
    }
    left == right
}

/// `(a, b) => a - b`.
///
/// PORT NOTE -- when the subtraction is NaN the specification treats the
/// comparator result as 0, i.e. `Equal`. `partial_cmp` would return `None` and
/// `total_cmp` would order NaN, and both are different programs. Offsets here
/// are always finite, but the helper is named and tested so that stays true by
/// construction rather than by assumption.
fn js_numeric_comparator(a: &f64, b: &f64) -> std::cmp::Ordering {
    let difference = a - b;
    if difference.is_nan() || difference == 0.0 {
        std::cmp::Ordering::Equal
    } else if difference < 0.0 {
        std::cmp::Ordering::Less
    } else {
        std::cmp::Ordering::Greater
    }
}

/// `offset.toString(16)`.
fn hex_lower(value: f64) -> String {
    if value < 0.0 || value.fract() != 0.0 {
        // Unreachable for objdump offsets; formatted the JavaScript way rather
        // than panicking.
        return to_js_number_string(value).unwrap_or_else(|_| "NaN".to_string());
    }
    format!("{:x}", value as u64)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn numeric_comparator_treats_nan_as_equal_not_as_an_ordering() {
        let mut values = vec![4.0, 2.0, 6.0, 0.0];
        values.sort_by(js_numeric_comparator);
        assert_eq!(values, vec![0.0, 2.0, 4.0, 6.0]);
        assert_eq!(
            js_numeric_comparator(&f64::INFINITY, &f64::INFINITY),
            std::cmp::Ordering::Equal
        );
        assert_eq!(
            js_numeric_comparator(&f64::NAN, &1.0),
            std::cmp::Ordering::Equal
        );
    }

    #[test]
    fn hex_offsets_are_lowercase_and_zero_padded_to_four() {
        assert_eq!(pad_start_zero(&hex_lower(0.0), 4), "0000");
        assert_eq!(pad_start_zero(&hex_lower(26.0), 4), "001a");
        // Never truncates: a five-digit offset widens the column.
        assert_eq!(pad_start_zero(&hex_lower(0x1_0000 as f64), 4), "10000");
    }

    #[test]
    fn the_candidate_column_is_padded_then_clipped_to_thirty() {
        let short = slice_utf16(&pad_end("push\t{lr}", 30), 30);
        assert_eq!(short.len(), 30);
        let long = "x".repeat(40);
        let clipped = slice_utf16(&pad_end(&long, 30), 30);
        assert_eq!(clipped, "x".repeat(30));
    }
}
