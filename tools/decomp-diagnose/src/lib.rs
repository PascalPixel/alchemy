// Exact-C candidate diagnostics, ported from tools/decomp/decomp_diagnose.ts.
//
// WHY THIS FILE EXISTS
// A byte diff between a candidate's compiled output and the ROM says "wrong"
// but never says "wrong how". This module classifies each instruction-level
// mismatch into one of seven kinds so the queue can tell a register-allocation
// near-miss (worth a flag sweep) apart from a semantic miss (worth a rewrite).
// Candidate and reference bytes stay in ignored scratch space; only the
// aggregate mismatch classes are durable.
//
// The port is byte-for-byte faithful to the TypeScript, including its JS
// string/regex semantics; every deliberate divergence carries a PORT NOTE.

use std::path::{Path, PathBuf};
use std::process::Command;

pub mod json;
pub mod jsstr;

use jsstr::{js_trim, replace_all_ws_runs, strip_comment_tail, substitute_addresses, substitute_registers};

pub const ROM_BASE: i64 = 0x0800_0000;

/// Repository root, mirroring the TypeScript `ROOT` computed from `import.meta.url`.
pub fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum MismatchKind {
    RegisterOnly,
    InstructionReorder,
    LiteralOrAddress,
    ControlFlow,
    Semantic,
    MissingInstruction,
    ExtraInstruction,
}

/// Insertion order is load-bearing: it drives both the `counts` object key
/// order in the JSON report and the first-wins tie-break for `dominant`.
pub const KINDS: [MismatchKind; 7] = [
    MismatchKind::RegisterOnly,
    MismatchKind::InstructionReorder,
    MismatchKind::LiteralOrAddress,
    MismatchKind::ControlFlow,
    MismatchKind::Semantic,
    MismatchKind::MissingInstruction,
    MismatchKind::ExtraInstruction,
];

impl MismatchKind {
    pub fn name(self) -> &'static str {
        match self {
            MismatchKind::RegisterOnly => "register_only",
            MismatchKind::InstructionReorder => "instruction_reorder",
            MismatchKind::LiteralOrAddress => "literal_or_address",
            MismatchKind::ControlFlow => "control_flow",
            MismatchKind::Semantic => "semantic",
            MismatchKind::MissingInstruction => "missing_instruction",
            MismatchKind::ExtraInstruction => "extra_instruction",
        }
    }

    fn index(self) -> usize {
        KINDS.iter().position(|kind| *kind == self).unwrap()
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct DecodedInstruction {
    pub address: i64,
    pub size: usize,
    pub mnemonic: String,
    pub operands: String,
}

#[derive(Clone, Debug, PartialEq)]
pub struct CandidateDiagnosis {
    pub stem: String,
    pub byte_mismatches: i64,
    pub actual_size: i64,
    pub expected_size: i64,
    pub instruction_mismatches: i64,
    pub counts: [i64; 7],
    /// `"exact"` when there are no mismatches, otherwise a `MismatchKind` name.
    pub dominant: String,
    pub register_fraction: f64,
    pub semantic_fraction: f64,
}

impl CandidateDiagnosis {
    pub fn count(&self, kind: MismatchKind) -> i64 {
        self.counts[kind.index()]
    }

    /// Reproduces `JSON.stringify(diagnosis, null, 2)` exactly: key order follows
    /// the object literal, and numbers use the shortest round-tripping form
    /// (Rust's `{}` for f64 agrees with JS `Number#toString` over [0, 1]).
    pub fn to_json(&self) -> String {
        let mut out = String::new();
        out.push_str("{\n");
        out.push_str(&format!("  \"stem\": {},\n", json::quote(&self.stem)));
        out.push_str(&format!("  \"byte_mismatches\": {},\n", self.byte_mismatches));
        out.push_str(&format!("  \"actual_size\": {},\n", self.actual_size));
        out.push_str(&format!("  \"expected_size\": {},\n", self.expected_size));
        out.push_str(&format!(
            "  \"instruction_mismatches\": {},\n",
            self.instruction_mismatches
        ));
        out.push_str("  \"counts\": {\n");
        for (index, kind) in KINDS.iter().enumerate() {
            let comma = if index + 1 == KINDS.len() { "" } else { "," };
            out.push_str(&format!(
                "    \"{}\": {}{}\n",
                kind.name(),
                self.counts[index],
                comma
            ));
        }
        out.push_str("  },\n");
        out.push_str(&format!("  \"dominant\": {},\n", json::quote(&self.dominant)));
        out.push_str(&format!(
            "  \"register_fraction\": {},\n",
            json::number(self.register_fraction)
        ));
        out.push_str(&format!(
            "  \"semantic_fraction\": {}\n",
            json::number(self.semantic_fraction)
        ));
        out.push('}');
        out
    }
}

const S_SUFFIX_BASES: &[&str] = &[
    "mov", "mvn", "add", "adc", "sub", "sbc", "rsb", "and", "orr", "eor", "bic", "lsl", "lsr",
    "asr", "ror", "neg", "mul",
];

/// `value.toLowerCase().replace(/\.(?:n|w)$/, "")` plus the flag-setting `s`
/// suffix strip for the data-processing mnemonics.
pub fn base_mnemonic(value: &str) -> String {
    let lowered = value.to_lowercase();
    let bare = if lowered.ends_with(".n") || lowered.ends_with(".w") {
        &lowered[..lowered.len() - 2]
    } else {
        lowered.as_str()
    };
    if let Some(stem) = bare.strip_suffix('s') {
        if S_SUFFIX_BASES.contains(&stem) {
            return stem.to_string();
        }
    }
    bare.to_string()
}

pub fn normalized_operands(value: &str, registers: bool) -> String {
    let mut text = strip_comment_tail(value);
    text = replace_all_ws_runs(&text);
    text = substitute_addresses(&text);
    let mut text = js_trim(&text);
    if !registers {
        text = substitute_registers(&text);
    }
    text
}

pub fn signature(instruction: &DecodedInstruction, registers: bool) -> String {
    format!(
        "{} {}",
        base_mnemonic(&instruction.mnemonic),
        normalized_operands(&instruction.operands, registers)
    )
}

const CONTROL_CONDITIONS: &[&str] = &[
    "l", "x", "eq", "ne", "cs", "cc", "hs", "lo", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt",
    "gt", "le",
];

/// The source regex is fully anchored, so membership is equivalent to matching.
pub fn is_control(instruction: &DecodedInstruction) -> bool {
    let mnemonic = base_mnemonic(&instruction.mnemonic);
    if matches!(mnemonic.as_str(), "cbz" | "cbnz" | "pop" | "b") {
        return true;
    }
    match mnemonic.strip_prefix('b') {
        Some(rest) => CONTROL_CONDITIONS.contains(&rest),
        None => false,
    }
}

pub fn is_literal_or_address(instruction: &DecodedInstruction) -> bool {
    let mnemonic = base_mnemonic(&instruction.mnemonic);
    if mnemonic == "ldr" && jsstr::has_word(&instruction.operands, "pc") {
        return true;
    }
    jsstr::contains_ignore_ascii_case(&instruction.operands, "<address>")
        || jsstr::has_rom_address_literal(&instruction.operands)
}

type Pair<'a> = (Option<&'a DecodedInstruction>, Option<&'a DecodedInstruction>);

/// Minimum-edit alignment keeps an insertion from making every later
/// instruction appear to be a semantic mismatch.
///
/// PORT NOTE: the TypeScript cost matrix is a `Uint16Array`, so costs above
/// 65535 wrap. `wrapping_add` and `u16` preserve that (pathological) behaviour
/// rather than quietly widening it.
fn align<'a>(actual: &'a [DecodedInstruction], expected: &'a [DecodedInstruction]) -> Vec<Pair<'a>> {
    let rows = actual.len() + 1;
    let columns = expected.len() + 1;
    let mut cost = vec![vec![0u16; columns]; rows];
    let mut mv = vec![vec![0u8; columns]; rows];
    for row in 1..rows {
        cost[row][0] = row as u16;
        mv[row][0] = 1;
    }
    for column in 1..columns {
        cost[0][column] = column as u16;
        mv[0][column] = 2;
    }
    let actual_loose: Vec<String> = actual.iter().map(|item| signature(item, false)).collect();
    let expected_loose: Vec<String> = expected.iter().map(|item| signature(item, false)).collect();
    for row in 1..rows {
        for column in 1..columns {
            let substitution = u16::from(actual_loose[row - 1] != expected_loose[column - 1]);
            let choices = [
                cost[row - 1][column - 1].wrapping_add(substitution),
                cost[row - 1][column].wrapping_add(1),
                cost[row][column - 1].wrapping_add(1),
            ];
            let mut choice = 0usize;
            if choices[1] < choices[choice] {
                choice = 1;
            }
            if choices[2] < choices[choice] {
                choice = 2;
            }
            cost[row][column] = choices[choice];
            mv[row][column] = choice as u8;
        }
    }
    let mut result: Vec<Pair<'a>> = Vec::new();
    let mut row = actual.len();
    let mut column = expected.len();
    while row > 0 || column > 0 {
        let choice = mv[row][column];
        if row > 0 && column > 0 && choice == 0 {
            row -= 1;
            column -= 1;
            result.push((Some(&actual[row]), Some(&expected[column])));
        } else if row > 0 && (column == 0 || choice == 1) {
            row -= 1;
            result.push((Some(&actual[row]), None));
        } else {
            column -= 1;
            result.push((None, Some(&expected[column])));
        }
    }
    result.reverse();
    result
}

pub fn diagnose_instructions(
    stem: &str,
    actual: &[DecodedInstruction],
    expected: &[DecodedInstruction],
    actual_bytes: i64,
    expected_bytes: i64,
    byte_mismatches: i64,
) -> CandidateDiagnosis {
    let mut counts = [0i64; 7];
    let pairs = align(actual, expected);
    let actual_signatures: Vec<String> = actual.iter().map(|item| signature(item, false)).collect();
    let expected_signatures: Vec<String> =
        expected.iter().map(|item| signature(item, false)).collect();
    let mut mismatches = 0i64;
    for (index, (left, right)) in pairs.iter().enumerate() {
        let (left, right) = match (left, right) {
            (None, _) => {
                counts[MismatchKind::MissingInstruction.index()] += 1;
                mismatches += 1;
                continue;
            }
            (Some(left), None) => {
                let _ = left;
                counts[MismatchKind::ExtraInstruction.index()] += 1;
                mismatches += 1;
                continue;
            }
            (Some(left), Some(right)) => (*left, *right),
        };
        if signature(left, true) == signature(right, true) {
            continue;
        }
        mismatches += 1;
        let loose_left = signature(left, false);
        let loose_right = signature(right, false);
        if loose_left == loose_right {
            counts[MismatchKind::RegisterOnly.index()] += 1;
            continue;
        }
        let a = actual_signatures.iter().position(|item| *item == loose_right);
        let e = expected_signatures.iter().position(|item| *item == loose_left);
        let index = index as i64;
        let near = |found: Option<usize>| {
            found.is_some_and(|found| (found as i64 - index).abs() <= 3)
        };
        if near(a) || near(e) {
            counts[MismatchKind::InstructionReorder.index()] += 1;
        } else if is_literal_or_address(left) || is_literal_or_address(right) {
            counts[MismatchKind::LiteralOrAddress.index()] += 1;
        } else if is_control(left) || is_control(right) {
            counts[MismatchKind::ControlFlow.index()] += 1;
        } else {
            counts[MismatchKind::Semantic.index()] += 1;
        }
    }
    let dominant = if mismatches == 0 {
        "exact".to_string()
    } else {
        let mut best = 0usize;
        for index in 0..KINDS.len() {
            if counts[index] > counts[best] {
                best = index;
            }
        }
        KINDS[best].name().to_string()
    };
    let semantic = counts[MismatchKind::Semantic.index()]
        + counts[MismatchKind::ControlFlow.index()]
        + counts[MismatchKind::LiteralOrAddress.index()];
    let register = counts[MismatchKind::RegisterOnly.index()]
        + counts[MismatchKind::InstructionReorder.index()];
    CandidateDiagnosis {
        stem: stem.to_string(),
        byte_mismatches,
        actual_size: actual_bytes,
        expected_size: expected_bytes,
        instruction_mismatches: mismatches,
        counts,
        dominant,
        register_fraction: if mismatches == 0 {
            0.0
        } else {
            register as f64 / mismatches as f64
        },
        semantic_fraction: if mismatches == 0 {
            0.0
        } else {
            semantic as f64 / mismatches as f64
        },
    }
}

/// Parses one `arm-none-eabi-objdump -D` listing line.
///
/// Mirrors `/^\s*([0-9a-f]+):\s+([0-9a-f ]+?)\s+\t(\S+)\s*(.*)$/i` — an
/// unanchored-at-the-right, lazily-expanding encoded-bytes group followed by a
/// literal tab.
pub fn parse_listing_row(row: &str) -> Option<DecodedInstruction> {
    let chars: Vec<char> = row.chars().collect();
    let mut cursor = 0usize;
    while cursor < chars.len() && jsstr::is_js_space(chars[cursor]) {
        cursor += 1;
    }
    let address_start = cursor;
    while cursor < chars.len() && chars[cursor].is_ascii_hexdigit() {
        cursor += 1;
    }
    if cursor == address_start || chars.get(cursor) != Some(&':') {
        return None;
    }
    let address_text: String = chars[address_start..cursor].iter().collect();
    let after_colon = cursor + 1;
    let mut gap_end = after_colon;
    while gap_end < chars.len() && jsstr::is_js_space(chars[gap_end]) {
        gap_end += 1;
    }
    // `\s+` is greedy; backtrack to shorter gaps the way the engine does.
    for gap in (after_colon + 1..=gap_end).rev() {
        if let Some(instruction) = parse_encoded(&chars, gap, &address_text) {
            return Some(instruction);
        }
    }
    None
}

/// Lazy `([0-9a-f ]+?)` followed by greedy `\s+` and a literal tab.
fn parse_encoded(chars: &[char], start: usize, address_text: &str) -> Option<DecodedInstruction> {
    let mut end = start;
    while end < chars.len() && (chars[end].is_ascii_hexdigit() || chars[end] == ' ') {
        end += 1;
        let mut run = end;
        while run < chars.len() && jsstr::is_js_space(chars[run]) {
            run += 1;
        }
        // The tab must sit after at least one whitespace character, so it can
        // only be at offsets `end + 1 ..= run - 1` within the run.
        for tab in (end + 1..run).rev() {
            if chars[tab] != '\t' {
                continue;
            }
            if let Some(instruction) =
                parse_operands(chars, tab + 1, address_text, &chars[start..end])
            {
                return Some(instruction);
            }
        }
    }
    None
}

/// `(\S+)\s*(.*)$`.
fn parse_operands(
    chars: &[char],
    start: usize,
    address_text: &str,
    encoded: &[char],
) -> Option<DecodedInstruction> {
    let mut cursor = start;
    while cursor < chars.len() && !jsstr::is_js_space(chars[cursor]) {
        cursor += 1;
    }
    if cursor == start {
        return None;
    }
    let mnemonic: String = chars[start..cursor].iter().collect();
    while cursor < chars.len() && jsstr::is_js_space(chars[cursor]) {
        cursor += 1;
    }
    let operands: String = chars[cursor..].iter().collect();
    // `.*$` cannot cross a line terminator.
    if operands.chars().any(jsstr::is_line_terminator) {
        return None;
    }
    let encoded: String = encoded.iter().filter(|item| **item != ' ').collect();
    let address = i64::from_str_radix(&address_text.to_lowercase(), 16).ok()?;
    Some(DecodedInstruction {
        address,
        size: encoded.len() / 2,
        mnemonic,
        operands: js_trim(&operands),
    })
}

/// Runs objdump over `bytes` written to `path`, returning the decoded listing.
pub fn disassemble(bytes: &[u8], address: i64, path: &Path) -> Result<Vec<DecodedInstruction>, String> {
    std::fs::write(path, bytes).map_err(|error| error.to_string())?;
    let output = Command::new("arm-none-eabi-objdump")
        .args([
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &format!("--adjust-vma=0x{:x}", address),
        ])
        .arg(path)
        .current_dir(repo_root())
        .output()
        .map_err(|error| error.to_string())?;
    if !output.status.success() {
        return Err("objdump failed".to_string());
    }
    // PORT NOTE: `new Response(stdout).text()` is lossy UTF-8, matching
    // `String::from_utf8_lossy` rather than a strict decode.
    let text = String::from_utf8_lossy(&output.stdout);
    Ok(split_lines(&text)
        .into_iter()
        .filter_map(parse_listing_row)
        .collect())
}

/// `text.split(/\r?\n/)`.
pub fn split_lines(text: &str) -> Vec<&str> {
    let mut rows = Vec::new();
    let mut start = 0usize;
    let raw = text.as_bytes();
    let mut index = 0usize;
    while index < raw.len() {
        if raw[index] == b'\n' {
            let end = if index > start && raw[index - 1] == b'\r' {
                index - 1
            } else {
                index
            };
            rows.push(&text[start..end]);
            start = index + 1;
        }
        index += 1;
    }
    rows.push(&text[start..]);
    rows
}

/// Assembly manifest lookup: stem -> region size, first manifest that exists wins.
pub fn assembly_sizes(root: &Path) -> Vec<(String, i64)> {
    let candidates = [
        root.join("out/full/asm/manifest.json"),
        root.join("out/asm/manifest.json"),
    ];
    let Some(path) = candidates.into_iter().find(|path| path.exists()) else {
        return Vec::new();
    };
    let Ok(text) = std::fs::read(&path) else {
        return Vec::new();
    };
    let text = String::from_utf8_lossy(&text);
    let Ok(document) = json::parse(&text) else {
        return Vec::new();
    };
    let mut sizes = Vec::new();
    if let Some(regions) = document.get("regions").and_then(json::Value::as_array) {
        for region in regions {
            let (Some(source), Some(size)) = (
                region.get("source").and_then(json::Value::as_str),
                region.get("size").and_then(json::Value::as_number),
            ) else {
                continue;
            };
            sizes.push((basename(source, ".s"), size as i64));
        }
    }
    sizes
}

/// Node's `basename(path, extension)`.
pub fn basename(path: &str, extension: &str) -> String {
    let tail = path.rsplit('/').next().unwrap_or(path);
    if !extension.is_empty() && tail.len() > extension.len() && tail.ends_with(extension) {
        return tail[..tail.len() - extension.len()].to_string();
    }
    tail.to_string()
}

/// `Map#get` over an insertion-ordered map where later writes win.
pub fn lookup(sizes: &[(String, i64)], stem: &str) -> Option<i64> {
    sizes
        .iter()
        .rev()
        .find(|(key, _)| key == stem)
        .map(|(_, value)| *value)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn insn(mnemonic: &str, operands: &str, address: i64) -> DecodedInstruction {
        DecodedInstruction {
            address,
            size: 2,
            mnemonic: mnemonic.to_string(),
            operands: operands.to_string(),
        }
    }

    fn reference() -> Vec<DecodedInstruction> {
        vec![
            insn("ldr", "r3, [r0]", 0),
            insn("str", "r2, [r1]", 2),
            insn("bx", "lr", 4),
        ]
    }

    #[test]
    fn register_only_diagnosis_matches_the_typescript_self_test() {
        let expected = reference();
        let registers = vec![
            insn("ldr", "r2, [r0]", 0),
            insn("str", "r3, [r1]", 2),
            insn("bx", "lr", 4),
        ];
        let first = diagnose_instructions("test", &registers, &expected, 0, 0, 0);
        assert_eq!(first.count(MismatchKind::RegisterOnly), 2);
        assert_eq!(first.dominant, "register_only");
    }

    #[test]
    fn alignment_charges_an_insertion_once() {
        let expected = reference();
        let mut inserted = vec![insn("mov", "r3, r2", 0)];
        for item in &expected {
            let mut shifted = item.clone();
            shifted.address += 2;
            inserted.push(shifted);
        }
        let second = diagnose_instructions("test", &inserted, &expected, 0, 0, 0);
        assert_eq!(second.count(MismatchKind::ExtraInstruction), 1);
    }

    #[test]
    fn identical_listings_are_exact() {
        let diagnosis = diagnose_instructions("test", &reference(), &reference(), 8, 8, 0);
        assert_eq!(diagnosis.dominant, "exact");
        assert_eq!(diagnosis.instruction_mismatches, 0);
        assert_eq!(diagnosis.register_fraction, 0.0);
    }

    #[test]
    fn base_mnemonic_strips_width_and_flag_suffixes() {
        assert_eq!(base_mnemonic("ADDS"), "add");
        assert_eq!(base_mnemonic("bls.n"), "bls");
        assert_eq!(base_mnemonic("push"), "push");
        // `ldrs` is not a flag-setting data-processing mnemonic.
        assert_eq!(base_mnemonic("ldrs"), "ldrs");
        assert_eq!(base_mnemonic("movs.w"), "mov");
    }

    #[test]
    fn operands_normalize_comments_registers_and_addresses() {
        assert_eq!(normalized_operands("r0, [pc, #20]\t@ (0x80037f8)", true), "r0, [pc, #20]");
        assert_eq!(
            normalized_operands("r0, [pc, #20]\t@ (0x80037f8)", false),
            "<reg>, [<reg>, #20]"
        );
        assert_eq!(normalized_operands("0x8003e58 <thing>", true), "<address>");
        // r13 is not a recognized register token and survives verbatim.
        assert_eq!(normalized_operands("r13, r10", false), "r13, <reg>");
    }

    #[test]
    fn control_and_literal_classifiers_follow_the_source_alternations() {
        assert!(is_control(&insn("b", "0x80037dc", 0)));
        assert!(is_control(&insn("blo", "0x80037dc", 0)));
        assert!(is_control(&insn("ble", "0x80037dc", 0)));
        assert!(is_control(&insn("pop", "{pc}", 0)));
        assert!(!is_control(&insn("bic", "r0, r1", 0)));
        assert!(is_literal_or_address(&insn("ldr", "r0, [pc, #20]", 0)));
        assert!(!is_literal_or_address(&insn("adds", "r0, r1", 0)));
        assert!(is_literal_or_address(&insn("adds", "r0, 0x08003e58", 0)));
        // Only 0x08 followed by six hex digits counts.
        assert!(!is_literal_or_address(&insn("adds", "r0, 0x0800", 0)));
    }

    #[test]
    fn listing_rows_decode_two_and_four_byte_encodings() {
        let two = parse_listing_row(" 80037d4:\tb500      \tpush\t{lr}").unwrap();
        assert_eq!(two.address, 0x80037d4);
        assert_eq!(two.size, 2);
        assert_eq!(two.mnemonic, "push");
        assert_eq!(two.operands, "{lr}");
        let four = parse_listing_row(" 80037e0:\tf7ff fffe \tbl\t0x80037e4").unwrap();
        assert_eq!(four.size, 4);
        assert_eq!(four.mnemonic, "bl");
        assert_eq!(four.operands, "0x80037e4");
        let with_comment =
            parse_listing_row(" 80037e2:\t4805      \tldr\tr0, [pc, #20]\t@ (0x80037f8)").unwrap();
        assert_eq!(with_comment.operands, "r0, [pc, #20]\t@ (0x80037f8)");
    }

    #[test]
    fn listing_headers_and_labels_are_rejected() {
        assert!(parse_listing_row("").is_none());
        assert!(parse_listing_row("080037d4 <.data>:").is_none());
        assert!(parse_listing_row("candidate.bin:     file format binary").is_none());
        assert!(parse_listing_row("Disassembly of section .data:").is_none());
    }

    #[test]
    fn json_matches_the_stringify_layout() {
        let diagnosis = diagnose_instructions(
            "080037d4",
            &[insn("ldr", "r2, [r0]", 0)],
            &[insn("ldr", "r3, [r0]", 0)],
            2,
            2,
            1,
        );
        let text = diagnosis.to_json();
        assert!(text.starts_with("{\n  \"stem\": \"080037d4\",\n"));
        assert!(text.contains("  \"counts\": {\n    \"register_only\": 1,\n"));
        assert!(text.contains("\"register_fraction\": 1,\n"));
        assert!(text.ends_with("\"semantic_fraction\": 0\n}"));
    }

    #[test]
    fn basename_and_lookup_follow_node_and_map_semantics() {
        assert_eq!(basename("asm/080000c0.s", ".s"), "080000c0");
        assert_eq!(basename("asm/x.s", ".c"), "x.s");
        assert_eq!(basename(".s", ".s"), ".s");
        let sizes = vec![("a".to_string(), 1), ("a".to_string(), 2)];
        assert_eq!(lookup(&sizes, "a"), Some(2));
        assert_eq!(lookup(&sizes, "b"), None);
    }

    /// Float formatting is a known porting trap: ryu does not match JS, and
    /// `toFixed` rounds by decimal-string carry rather than half-to-even. This
    /// tool emits exactly two floats, `register_fraction` and
    /// `semantic_fraction`, and both are `k / mismatches` for integer `k` and
    /// `mismatches`. Rather than argue the trap is inapplicable, this walks
    /// EVERY value those fields can take for up to 40 mismatches against a
    /// table generated by `String(k / n)` under the live Bun, and requires an
    /// exact string match. `src/js_number_reference.txt` is that table.
    #[test]
    fn every_emitted_float_matches_javascript_number_to_string() {
        let reference: Vec<&str> = include_str!("js_number_reference.txt")
            .split(' ')
            .collect();
        let mut index = 0usize;
        for denominator in 1..=40i64 {
            for numerator in 0..=denominator {
                let value = numerator as f64 / denominator as f64;
                assert_eq!(
                    json::number(value),
                    reference[index],
                    "{numerator}/{denominator} formats differently than JavaScript"
                );
                index += 1;
            }
        }
        assert_eq!(index, reference.len());
        // The two integral endpoints are the ones a naive `{}`/ryu port gets
        // wrong ("0.0"/"1.0"); pin them by name too.
        assert_eq!(json::number(0.0), "0");
        assert_eq!(json::number(1.0), "1");
    }

    /// The manifest branch of `expectedSize` is exercised by only one of the
    /// twenty-four live corpus candidates, so the parity harness barely covers
    /// it. This drives it directly off a fixture, including the "first
    /// manifest that exists wins" precedence and the `basename(source, ".s")`
    /// key derivation.
    #[test]
    fn assembly_sizes_reads_the_first_manifest_that_exists() {
        let fixture = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join(format!("fixtures-{}-manifest", std::process::id()));
        let full = fixture.join("out/full/asm");
        let plain = fixture.join("out/asm");
        std::fs::create_dir_all(&full).unwrap();
        std::fs::create_dir_all(&plain).unwrap();
        std::fs::write(
            plain.join("manifest.json"),
            r#"{"regions":[{"source":"asm/deadbeef.s","size":999}]}"#,
        )
        .unwrap();
        // No `out/full/asm/manifest.json` yet, so the fallback is used.
        assert_eq!(lookup(&assembly_sizes(&fixture), "deadbeef"), Some(999));
        std::fs::write(
            full.join("manifest.json"),
            r#"{"regions":[{"source":"asm/08002df0.s","size":16},{"source":"nested/dir/0800abcd.s","size":4}]}"#,
        )
        .unwrap();
        let sizes = assembly_sizes(&fixture);
        assert_eq!(lookup(&sizes, "08002df0"), Some(16));
        assert_eq!(lookup(&sizes, "0800abcd"), Some(4));
        // The fallback manifest is now shadowed, not merged.
        assert_eq!(lookup(&sizes, "deadbeef"), None);
        // A missing root yields no sizes rather than a panic.
        assert!(assembly_sizes(Path::new("/nonexistent-parity-root")).is_empty());
    }

    #[test]
    fn split_lines_handles_crlf_and_a_trailing_segment() {
        assert_eq!(split_lines("a\r\nb\nc"), vec!["a", "b", "c"]);
        assert_eq!(split_lines(""), vec![""]);
        assert_eq!(split_lines("a\n"), vec!["a", ""]);
    }
}
