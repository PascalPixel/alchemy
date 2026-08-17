//! `main()`.

use std::path::Path;
use std::process::Command;

use alchemy_routing::routing::CompilerTarget;
use candidate_compiler::jsnum::to_js_number_string;
use candidate_compiler::verify::{
    js_subarray, verify_candidate, CandidateCompilerFamily, ROM_BASE,
};

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
    let rom_path = options.rom.as_deref().ok_or_else(|| {
        "The \"path\" argument must be of type string. Received undefined".to_string()
    })?;
    let work = options.work.as_deref().ok_or_else(|| {
        "The \"path\" argument must be of type string. Received undefined".to_string()
    })?;
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

    // PORT NOTE -- `basename(source, ".c")`, NOT `candidate_compiler`'s `sourceStem`,
    // which strips `extname(path)` whatever it is. For `x.cpp` the two
    // disagree and this file uses the literal `.c` form.
    let stem = basename_without(&options.source, ".c").to_string();
    // PORT NOTE -- `Number.parseInt(stem, 16)` with NO validating guard, unlike
    // `candidate_compiler`'s `parseHex`. It tolerates a `0x` prefix and stops at the
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

    // PORT NOTE -- the `exitCode === 0` test is load-bearing: a failed `nm`
    // falls back to the entry symbol's own size rather than erroring. This also
    // carried a gcc2951 exclusion, dropped with that family.
    let extent = if symbols.status.success() {
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
        if !offsets
            .iter()
            .any(|existing| same_value_zero(*existing, key))
        {
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
    if options.align {
        // Ordered instruction streams, then aligned as sequences.
        let ordered = |map: &crate::disasm::Rows| -> Vec<String> {
            let mut keys: Vec<f64> = map.keys().collect();
            keys.sort_by(js_numeric_comparator);
            keys.iter()
                .map(|key| map.get(*key).unwrap_or("").to_string())
                .collect()
        };
        let left_lines = ordered(&left);
        let right_lines = ordered(&right);
        stdout.push_str("      candidate                      reference\n");
        for (candidate, reference) in align_streams(&left_lines, &right_lines) {
            let mark = match (&candidate, &reference) {
                (Some(a), Some(b)) if a == b => " ",
                (Some(_), Some(_)) => "!",
                (Some(_), None) => "+",
                (None, Some(_)) => "-",
                (None, None) => " ",
            };
            let a = slice_utf16(&pad_end(candidate.as_deref().unwrap_or(""), 30), 30);
            let b = reference.as_deref().unwrap_or("");
            stdout.push_str(&format!("  {mark} {a} {b}\n"));
        }
        return Ok(RenderOutput {
            stdout,
            candidate_length: actual.len(),
            reference_length: expected.len(),
            differing_halfwords: differing.len(),
            rows: offsets.len(),
        });
    }
    if actual.len() != expected.len() {
        stdout.push_str(
            "  note: the two sides are different lengths, so the offset view below is\n             \x20       phase-shifted and every later row will read as a difference.\n             \x20       Re-run with --align to see the insertion or deletion itself.\n",
        );
    }
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


/// Canonicalise one instruction so that alignment is not defeated by things
/// that shift with position.
///
/// Branch and pc-relative targets move whenever anything before them changes
/// size, so two otherwise identical instructions would fail to pair. Blinding
/// them lets the alignment find the real insertion or deletion.
pub fn alignment_key(instruction: &str) -> String {
    let text = match instruction.find('@') {
        Some(at) => &instruction[..at],
        None => instruction,
    };
    let mut out = String::with_capacity(text.len());
    let bytes: Vec<char> = text.chars().collect();
    let mut index = 0;
    while index < bytes.len() {
        let c = bytes[index];
        if c == '0' && index + 1 < bytes.len() && bytes[index + 1] == 'x' {
            out.push_str("0xN");
            index += 2;
            while index < bytes.len() && bytes[index].is_ascii_hexdigit() {
                index += 1;
            }
            continue;
        }
        if c.is_ascii_digit() {
            out.push('N');
            while index < bytes.len() && bytes[index].is_ascii_digit() {
                index += 1;
            }
            continue;
        }
        out.push(if c == '\t' { ' ' } else { c });
        index += 1;
    }
    out.split_whitespace().collect::<Vec<_>>().join(" ")
}

/// Blank a pc-relative offset, keeping the resolved target the disassembler
/// prints beside it.
///
/// The offset encodes an instruction's POSITION, so without this a pool load
/// that merely moved reads as two different instructions and every transposed
/// literal load is misreported as a wrong instruction.
pub fn without_pc_offset(instruction: &str) -> String {
    let mut out = String::with_capacity(instruction.len());
    let mut rest = instruction;
    while let Some(start) = rest.find("[pc, #") {
        out.push_str(&rest[..start]);
        out.push_str("[pc]");
        rest = match rest[start..].find(']') {
            Some(end) => &rest[start + end + 1..],
            None => "",
        };
    }
    out.push_str(rest);
    out
}

/// Is the residual worth reading the source for?
///
/// Returns `("exact" | "ordering" | "allocation" | "wrong",
/// wrong_instruction_count)`.
///
/// Two owners can show the same differing-halfword count and be completely
/// different problems. When both sides hold the SAME instructions and only the
/// order differs, no source shape reaches it: emitted order is settled after
/// reload by `rank_for_schedule`'s tie-break chain, and the C no longer
/// controls it. When an instruction is genuinely wrong, the source is wrong
/// and reading it will find the defect.
///
/// `allocation` is the same idea one step further. Two sides can hold different
/// instruction TEXT and still differ only in which register the allocator picked:
/// `resource_3c7` and `080a524c` build a value in r3 where the reference uses r2
/// and are otherwise identical, and `08004144` moves a pair through r1 against
/// the reference's r5. Which register holds a value is decided after the source,
/// like order is, so those belong with `ordering` and not with a readable defect.
/// Without this they count as `wrong`, which sends a contributor at 26 owners --
/// 12 in the main image at 1,954 bytes, 14 in the overlays at 2,392 -- looking
/// for a source bug that is not there.
///
/// Both classes are EVIDENCE, not proof. Equal multisets, register-blind or not,
/// say a source reading is unlikely to reach the residual; they cannot say no
/// change anywhere reaches it. Treat them as "open these last".
///
/// Measured 2026-08-17 over the 282 size-exact parked overlay rows: 129 rows
/// (18,414 bytes) were ordering-only and 152 rows (43,900 bytes) carried a
/// wrong instruction. Ranking by this instead of by differing halfwords is
/// what separates a two-line fix from eighteen copies of a blocked tie.
pub fn residual_class(left: &[String], right: &[String]) -> (&'static str, i64) {
    let mut pool: std::collections::BTreeMap<String, i64> = std::collections::BTreeMap::new();
    for line in left {
        *pool.entry(without_pc_offset(line)).or_default() += 1;
    }
    for line in right {
        *pool.entry(without_pc_offset(line)).or_default() -= 1;
    }
    let wrong: i64 = pool.values().map(|count| count.abs()).sum();
    if wrong > 0 {
        // Same instructions up to which registers they name? Then the residual
        // is the allocator's, and no source spelling names it.
        let mut blind: std::collections::BTreeMap<String, i64> =
            std::collections::BTreeMap::new();
        for line in left {
            *blind.entry(without_register(&without_pc_offset(line))).or_default() += 1;
        }
        for line in right {
            *blind.entry(without_register(&without_pc_offset(line))).or_default() -= 1;
        }
        if blind.values().all(|count| *count == 0) {
            return ("allocation", wrong);
        }
        return ("wrong", wrong);
    }
    if left == right {
        ("exact", 0)
    } else {
        ("ordering", 0)
    }
}

/// The instruction with every register name replaced by `R`.
///
/// Keeps mnemonics, immediates and addressing shape, so a real difference still
/// shows: `strb` against `strh` survives and so does `#31` against `#30`. `pc` is
/// deliberately left alone, because `[pc]` is an addressing mode
/// `without_pc_offset` has already collapsed and blanking it would hide a
/// pool-versus-register difference.
pub fn without_register(instruction: &str) -> String {
    let chars: Vec<char> = instruction.chars().collect();
    let mut out = String::with_capacity(instruction.len());
    let mut index = 0usize;
    while index < chars.len() {
        let starts_word = index == 0 || !is_word(chars[index - 1]);
        if starts_word {
            let width = register_width(&chars[index..]);
            if width > 0 && !chars.get(index + width).is_some_and(|c| is_word(*c)) {
                out.push('R');
                index += width;
                continue;
            }
        }
        out.push(chars[index]);
        index += 1;
    }
    out
}

fn is_word(c: char) -> bool {
    c.is_alphanumeric() || c == '_'
}

/// How many characters of a register name start here, or 0.
fn register_width(rest: &[char]) -> usize {
    let word: String = rest
        .iter()
        .take_while(|c| c.is_alphanumeric())
        .collect::<String>()
        .to_ascii_lowercase();
    if let Some(digits) = word.strip_prefix('r') {
        if !digits.is_empty() && digits.chars().all(|c| c.is_ascii_digit()) {
            if digits.parse::<u32>().is_ok_and(|number| number <= 15) {
                return word.len();
            }
        }
    }
    if matches!(word.as_str(), "sp" | "lr" | "fp" | "ip" | "sl") {
        return word.len();
    }
    0
}

/// Longest-common-subsequence alignment of two instruction streams.
///
/// Returns one row per output line as `(candidate, reference)`, where `None`
/// on either side is an instruction the other stream does not have.
pub fn align_streams(left: &[String], right: &[String]) -> Vec<(Option<String>, Option<String>)> {
    let a: Vec<String> = left.iter().map(|line| alignment_key(line)).collect();
    let b: Vec<String> = right.iter().map(|line| alignment_key(line)).collect();
    let (rows, columns) = (a.len(), b.len());
    // Standard LCS table; these streams are function-sized, so the quadratic
    // table is small and the clarity is worth more than the cleverness.
    let mut table = vec![vec![0usize; columns + 1]; rows + 1];
    for i in (0..rows).rev() {
        for j in (0..columns).rev() {
            table[i][j] = if a[i] == b[j] {
                table[i + 1][j + 1] + 1
            } else {
                table[i + 1][j].max(table[i][j + 1])
            };
        }
    }
    let mut out = Vec::new();
    let (mut i, mut j) = (0usize, 0usize);
    while i < rows && j < columns {
        if a[i] == b[j] {
            out.push((Some(left[i].clone()), Some(right[j].clone())));
            i += 1;
            j += 1;
        } else if table[i + 1][j] >= table[i][j + 1] {
            out.push((Some(left[i].clone()), None));
            i += 1;
        } else {
            out.push((None, Some(right[j].clone())));
            j += 1;
        }
    }
    while i < rows {
        out.push((Some(left[i].clone()), None));
        i += 1;
    }
    while j < columns {
        out.push((None, Some(right[j].clone())));
        j += 1;
    }
    out
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
mod residual_tests {
    use super::*;

    fn lines(text: &[&str]) -> Vec<String> {
        text.iter().map(|line| line.to_string()).collect()
    }

    #[test]
    fn identical_streams_are_exact() {
        let a = lines(&["movs r0, #1", "bx lr"]);
        assert_eq!(residual_class(&a, &a), ("exact", 0));
    }

    #[test]
    fn a_transposition_is_ordering_not_a_wrong_instruction() {
        let left = lines(&["movs r0, #1", "lsls r1, r1, #8"]);
        let right = lines(&["lsls r1, r1, #8", "movs r0, #1"]);
        assert_eq!(residual_class(&left, &right), ("ordering", 0));
    }

    #[test]
    fn a_moved_pool_load_is_ordering_despite_its_changed_offset() {
        // The offset encodes position; without blanking it this reads as two
        // wrong instructions and an entire blocked family looks reachable.
        let left = lines(&["ldr r2, [pc, #24] @ (0x200077c)", "bx lr"]);
        let right = lines(&["bx lr", "ldr r2, [pc, #20] @ (0x200077c)"]);
        assert_eq!(residual_class(&left, &right), ("ordering", 0));
    }

    #[test]
    fn a_different_target_is_still_wrong() {
        // Only the offset is blanked, never the resolved address, or a genuine
        // pool-word difference would be hidden.
        let left = lines(&["ldr r2, [pc, #24] @ (0x200077c)"]);
        let right = lines(&["ldr r2, [pc, #24] @ (0x2000780)"]);
        assert_eq!(residual_class(&left, &right), ("wrong", 2));
    }

    #[test]
    fn a_wrong_register_counts_once_per_side() {
        // The COUNT is what this pins, and it is unchanged: a differing register
        // is one instruction on each side, so two. The CLASS became `allocation`
        // when that class was added -- a residual that is a consistent rename and
        // nothing else is the allocator's choice, not a source defect. Five owners
        // were opened to check that before changing this expectation
        // (080a524c, 08004144, 08078144, 080b0958, 0800fec8) and no source shape
        // moved the register on any of them.
        let left = lines(&["movs r1, #33", "bx lr"]);
        let right = lines(&["movs r3, #33", "bx lr"]);
        assert_eq!(residual_class(&left, &right), ("allocation", 2));
    }
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
    fn a_pure_register_rename_is_allocation_not_a_wrong_instruction() {
        // 080a524c's real residual: the flag is built in r3 where the reference
        // uses r2, and nothing else differs.
        let left = vec![
            "movs\tr3, #1".to_string(),
            "mov\tr8, r3".to_string(),
            "cmp\tr3, #0".to_string(),
        ];
        let right = vec![
            "movs\tr2, #1".to_string(),
            "mov\tr8, r2".to_string(),
            "cmp\tr2, #0".to_string(),
        ];
        assert_eq!(residual_class(&left, &right).0, "allocation");
    }

    #[test]
    fn allocation_does_not_swallow_a_real_difference() {
        // A store width and an immediate are the source's to decide, so neither
        // may be blanked away with the register names.
        let width_left = vec!["strb\tr3, [r2, #0]".to_string()];
        let width_right = vec!["strh\tr3, [r2, #0]".to_string()];
        assert_eq!(residual_class(&width_left, &width_right).0, "wrong");
        let imm_left = vec!["movs\tr2, #31".to_string()];
        let imm_right = vec!["movs\tr2, #30".to_string()];
        assert_eq!(residual_class(&imm_left, &imm_right).0, "wrong");
    }

    #[test]
    fn ordering_and_exact_still_outrank_allocation() {
        let a = vec!["movs\tr0, #1".to_string(), "movs\tr1, #2".to_string()];
        let mut swapped = a.clone();
        swapped.reverse();
        assert_eq!(residual_class(&a, &swapped).0, "ordering");
        assert_eq!(residual_class(&a, &a).0, "exact");
    }

    #[test]
    fn register_blanking_leaves_symbols_and_pc_alone() {
        // `r16` is not a register and a symbol beginning with `r` is not one.
        assert_eq!(without_register("bl\tr16_helper"), "bl\tr16_helper");
        assert_eq!(without_register("ldr\tr3, [pc]"), "ldr\tR, [pc]");
        assert_eq!(without_register("mov\tsl, r6"), "mov\tR, R");
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
