//! Typed parsing of one arm-none-eabi-objdump Thumb (ARMv4T) disassembly line
//! into a mnemonic plus a list of typed operands, so an aligner can compare a
//! real instruction against an RtlInsn structurally (same destination register,
//! same instruction class) instead of by raw text.
//!
//! Rust port of `tools/lib/thumb_disasm.ts`. Behaviour is bit-for-bit identical
//! to the TypeScript for every ASCII input; see the `PORT NOTE`s below for the
//! two places where the underlying language semantics had to be re-created by
//! hand (JavaScript whitespace/`\w` classes, and `Number(...)` coercion).

/// A single parsed operand.
///
/// PORT NOTE: `Imm` carries an `f64`, not an integer. The TypeScript builds it
/// with `Number(...)`, which is a double, and accepts things objdump never
/// emits (`#0x1f`, `#1e3`, `#` -> `0`). Narrowing to `i64` here would silently
/// change behaviour on those inputs, so the double is preserved exactly.
#[derive(Debug, Clone, PartialEq)]
pub enum Operand {
    Reg {
        name: String,
        number: i64,
        writeback: bool,
    },
    Imm {
        value: f64,
    },
    Mem {
        base: String,
        offset_text: String,
    },
    RegList {
        names: Vec<String>,
    },
    Label {
        text: String,
    },
    Other {
        text: String,
    },
}

/// One disassembled instruction: byte offset, mnemonic, operands, raw line.
#[derive(Debug, Clone, PartialEq)]
pub struct Instruction {
    pub offset: i64,
    pub mnemonic: String,
    pub operands: Vec<Operand>,
    pub raw: String,
}

// ---------------------------------------------------------------------------
// JavaScript character classes
// ---------------------------------------------------------------------------

/// PORT NOTE: JavaScript's `\s` (and the set `String.prototype.trim` strips) is
/// wider than Rust's `char::is_whitespace` in one direction (it includes
/// U+FEFF) and narrower in another (Rust's includes U+0085, JS's does not).
/// Re-created exactly so `trim()`, `search(/\s/)` and `split(/\s/)` agree.
fn is_js_space(c: char) -> bool {
    matches!(
        c,
        '\u{9}'
            | '\u{a}'
            | '\u{b}'
            | '\u{c}'
            | '\u{d}'
            | '\u{20}'
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

/// JavaScript `\w` == `[A-Za-z0-9_]` (ASCII only, no Unicode flag involved).
fn is_js_word(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// `String.prototype.trim()`.
fn js_trim(text: &str) -> &str {
    text.trim_matches(is_js_space)
}

// ---------------------------------------------------------------------------
// Hand-rolled regexes (no regex crate)
// ---------------------------------------------------------------------------

/// `/^(r\d{1,2}|sp|lr|pc)$/i`
fn is_reg_name(text: &str) -> bool {
    let lower = text.to_ascii_lowercase();
    if lower == "sp" || lower == "lr" || lower == "pc" {
        return true;
    }
    match lower.strip_prefix('r') {
        Some(digits) => {
            !digits.is_empty()
                && digits.len() <= 2
                && digits.bytes().all(|b| b.is_ascii_digit())
        }
        None => false,
    }
}

/// Numeric value of a register name, or `None` if it is not one.
///
/// Mirrors `/^r(\d{1,2})$/` exactly: `r99` is a "register" numbered 99, and
/// `r003` is not a register at all.
pub fn register_number(name: &str) -> Option<i64> {
    let lower = name.to_ascii_lowercase();
    match lower.as_str() {
        "sp" => return Some(13),
        "lr" => return Some(14),
        "pc" => return Some(15),
        _ => {}
    }
    let digits = lower.strip_prefix('r')?;
    if digits.is_empty() || digits.len() > 2 || !digits.bytes().all(|b| b.is_ascii_digit()) {
        return None;
    }
    digits.parse::<i64>().ok()
}

/// `/^(\w+)!$/` -> the captured word.
fn match_writeback(text: &str) -> Option<&str> {
    let head = text.strip_suffix('!')?;
    if head.is_empty() || !head.chars().all(is_js_word) {
        return None;
    }
    Some(head)
}

/// `/^0x[0-9a-f]+$/i`
fn is_hex_literal(text: &str) -> bool {
    let rest = match text.get(..2) {
        Some(p) if p.eq_ignore_ascii_case("0x") => &text[2..],
        _ => return false,
    };
    !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_hexdigit())
}

/// `/^[0-9a-f]+\s*<.*>$/i`
///
/// `.` in a non-`s` JavaScript regex does not match line terminators, and `$`
/// without the `m` flag anchors at the very end of the string; both are
/// reproduced here.
fn is_symbolic_label(text: &str) -> bool {
    let mut digits = 0usize;
    let mut idx = text.len();
    for (i, c) in text.char_indices() {
        if c.is_ascii_hexdigit() {
            digits += 1;
            continue;
        }
        idx = i;
        break;
    }
    if digits == 0 {
        return false;
    }
    let rest = text[idx..].trim_start_matches(is_js_space);
    let inner = match rest.strip_prefix('<') {
        Some(inner) => inner,
        None => return false,
    };
    // Greedy `.*` followed by `>$`: the final char must be `>`, and everything
    // between must contain no line terminator.
    let inner = match inner.strip_suffix('>') {
        Some(inner) => inner,
        None => return false,
    };
    !inner.contains(['\n', '\r', '\u{2028}', '\u{2029}'])
}

// ---------------------------------------------------------------------------
// JavaScript `Number(string)` coercion
// ---------------------------------------------------------------------------

/// PORT NOTE: `Number("")` is `0`, `Number("0x1f")` is `31`, `Number("1e3")` is
/// `1000`, and `Number("12abc")` is `NaN`. Rust's `f64::from_str` disagrees on
/// every one of those, so the ECMAScript `StringNumericLiteral` grammar is
/// implemented directly. The one documented divergence: radix literals whose
/// digits exceed `u128` fall back to floating accumulation and may differ from
/// V8 in the last ulp. objdump never emits such an operand (that would be a
/// 39-digit hex immediate), and the differential harness stays inside `u128`.
pub fn js_number(text: &str) -> f64 {
    let s = js_trim(text);
    if s.is_empty() {
        return 0.0;
    }
    if let Some(rest) = s.strip_prefix("0x").or_else(|| s.strip_prefix("0X")) {
        return radix_literal(rest, 16);
    }
    if let Some(rest) = s.strip_prefix("0o").or_else(|| s.strip_prefix("0O")) {
        return radix_literal(rest, 8);
    }
    if let Some(rest) = s.strip_prefix("0b").or_else(|| s.strip_prefix("0B")) {
        return radix_literal(rest, 2);
    }
    let (sign, body) = match s.strip_prefix('-') {
        Some(body) => (-1.0f64, body),
        None => (1.0f64, s.strip_prefix('+').unwrap_or(s)),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    match decimal_literal(body) {
        Some(value) => sign * value,
        None => f64::NAN,
    }
}

fn radix_literal(digits: &str, radix: u32) -> f64 {
    if digits.is_empty() || !digits.chars().all(|c| c.is_digit(radix)) {
        return f64::NAN;
    }
    if let Ok(value) = u128::from_str_radix(digits, radix) {
        return value as f64;
    }
    let mut acc = 0.0f64;
    for c in digits.chars() {
        acc = acc * f64::from(radix) + f64::from(c.to_digit(radix).unwrap_or(0));
    }
    acc
}

/// `StrUnsignedDecimalLiteral` minus `Infinity` (handled by the caller).
fn decimal_literal(body: &str) -> Option<f64> {
    let bytes = body.as_bytes();
    let mut i = 0usize;
    let mut int_digits = 0usize;
    while i < bytes.len() && bytes[i].is_ascii_digit() {
        i += 1;
        int_digits += 1;
    }
    let mut frac_digits = 0usize;
    if i < bytes.len() && bytes[i] == b'.' {
        i += 1;
        while i < bytes.len() && bytes[i].is_ascii_digit() {
            i += 1;
            frac_digits += 1;
        }
    }
    if int_digits == 0 && frac_digits == 0 {
        return None;
    }
    if i < bytes.len() && (bytes[i] | 0x20) == b'e' {
        i += 1;
        if i < bytes.len() && (bytes[i] == b'+' || bytes[i] == b'-') {
            i += 1;
        }
        let start = i;
        while i < bytes.len() && bytes[i].is_ascii_digit() {
            i += 1;
        }
        if i == start {
            return None;
        }
    }
    if i != bytes.len() {
        return None;
    }
    // Grammar already validated; normalise the two forms Rust's parser rejects
    // (`.5` and `5.`) and hand it correctly-rounded parsing.
    let mut normalised = String::with_capacity(body.len() + 2);
    if body.starts_with('.') {
        normalised.push('0');
    }
    normalised.push_str(body);
    if int_digits > 0 && frac_digits == 0 && body.contains('.') {
        // e.g. "5." or "5.e3" -> insert a zero after the dot.
        normalised = normalised.replacen('.', ".0", 1);
    }
    normalised.parse::<f64>().ok()
}

// ---------------------------------------------------------------------------
// Parsing
// ---------------------------------------------------------------------------

/// Splits an objdump operand string on top-level commas only, respecting
/// `[...]` memory operands and `{...}` register lists so their internal commas
/// (`[r0, #9]`, `{r5, lr}`) do not get treated as operand separators.
fn split_operands(text: &str) -> Vec<String> {
    let mut parts: Vec<String> = Vec::new();
    let mut depth: i64 = 0;
    let mut current = String::new();
    for c in text.chars() {
        if c == '[' || c == '{' {
            depth += 1;
        }
        if c == ']' || c == '}' {
            depth -= 1;
        }
        if c == ',' && depth == 0 {
            parts.push(js_trim(&current).to_string());
            current.clear();
            continue;
        }
        current.push(c);
    }
    let tail = js_trim(&current);
    if !tail.is_empty() {
        parts.push(tail.to_string());
    }
    parts
}

/// Parses one already-split operand into its typed form.
pub fn parse_operand(text: &str) -> Operand {
    let trimmed = js_trim(text);

    if trimmed.starts_with('{') {
        let stripped: String = trimmed.chars().filter(|c| *c != '{' && *c != '}').collect();
        let names = stripped
            .split(',')
            .map(|piece| js_trim(piece).to_string())
            .filter(|piece| !piece.is_empty())
            .collect();
        return Operand::RegList { names };
    }

    if trimmed.starts_with('[') {
        // PORT NOTE: when there is no `]`, JS `indexOf` yields -1 and
        // `slice(1, -1)` drops the final character instead of throwing. That
        // quirk is load-bearing for malformed lines, so it is preserved.
        let end = match trimmed.find(']') {
            Some(idx) => idx,
            None => trimmed.len().saturating_sub(1),
        };
        let inner = if end > 1 { &trimmed[1..end] } else { "" };
        let mut pieces = inner.split(',').map(js_trim);
        let base = pieces.next().unwrap_or("").to_string();
        let offset_text = pieces.collect::<Vec<_>>().join(",");
        return Operand::Mem { base, offset_text };
    }

    if let Some(word) = match_writeback(trimmed) {
        if is_reg_name(word) {
            return match register_number(word) {
                Some(number) => Operand::Reg {
                    name: word.to_string(),
                    number,
                    writeback: true,
                },
                None => Operand::Other {
                    text: trimmed.to_string(),
                },
            };
        }
    }

    if is_reg_name(trimmed) {
        return match register_number(trimmed) {
            Some(number) => Operand::Reg {
                name: trimmed.to_string(),
                number,
                writeback: false,
            },
            None => Operand::Other {
                text: trimmed.to_string(),
            },
        };
    }

    if let Some(rest) = trimmed.strip_prefix('#') {
        let head = match rest.find(is_js_space) {
            Some(idx) => &rest[..idx],
            None => rest,
        };
        let value = js_number(head);
        return if value.is_finite() {
            Operand::Imm { value }
        } else {
            Operand::Other {
                text: trimmed.to_string(),
            }
        };
    }

    if is_hex_literal(trimmed) || is_symbolic_label(trimmed) {
        return Operand::Label {
            text: trimmed.to_string(),
        };
    }

    Operand::Other {
        text: trimmed.to_string(),
    }
}

/// `objdump_line` is the mnemonic + operands column, with any trailing
/// `@ (0x38)`-style address comment still attached.
pub fn parse_instruction(offset: i64, objdump_line: &str) -> Instruction {
    let without_comment = match objdump_line.find("\t@") {
        Some(idx) => &objdump_line[..idx],
        None => objdump_line,
    };
    let without_comment = js_trim(without_comment);

    let (mnemonic, operand_text) = match without_comment.find(is_js_space) {
        Some(idx) => {
            // Every character JS's `\s` matches is a single UTF-16 unit, so
            // "skip one code unit" and "skip one char" agree here.
            let space_len = without_comment[idx..]
                .chars()
                .next()
                .map_or(1, char::len_utf8);
            (
                &without_comment[..idx],
                js_trim(&without_comment[idx + space_len..]),
            )
        }
        None => (without_comment, ""),
    };

    let operands = if operand_text.is_empty() {
        Vec::new()
    } else {
        split_operands(operand_text)
            .iter()
            .map(|piece| parse_operand(piece))
            .collect()
    };

    Instruction {
        offset,
        mnemonic: mnemonic.to_string(),
        operands,
        raw: objdump_line.to_string(),
    }
}

// ---------------------------------------------------------------------------
// Classification
// ---------------------------------------------------------------------------

/// The register an instruction's own encoding treats as its "primary
/// destination": for a data-processing/load op, its first reg operand. A
/// store's first reg operand is a SOURCE, not a destination, and reglist/branch
/// forms have no single answer -- both return `None` deliberately.
pub fn primary_dest_register(instruction: &Instruction) -> Option<i64> {
    if is_store_mnemonic(&instruction.mnemonic) {
        return None;
    }
    match instruction.operands.first() {
        Some(Operand::Reg { number, .. }) => Some(*number),
        _ => None,
    }
}

fn starts_with_any(text: &str, prefixes: &[&str]) -> bool {
    prefixes.iter().any(|p| text.starts_with(p))
}

/// `/^(str|strb|strh|stm|stmia|push)/` -- a prefix match, not a whole-word one.
pub fn is_store_mnemonic(mnemonic: &str) -> bool {
    starts_with_any(mnemonic, &["str", "strb", "strh", "stm", "stmia", "push"])
}

/// `/^(ldr|ldrb|ldrh|ldrsb|ldrsh|ldm|ldmia|pop)/`
pub fn is_load_mnemonic(mnemonic: &str) -> bool {
    starts_with_any(
        mnemonic,
        &["ldr", "ldrb", "ldrh", "ldrsb", "ldrsh", "ldm", "ldmia", "pop"],
    )
}

/// `/^bl[x]?$/` -- anchored both ends, so only `bl` and `blx`.
pub fn is_call_mnemonic(mnemonic: &str) -> bool {
    mnemonic == "bl" || mnemonic == "blx"
}

/// `/^(b|bx|beq|...|ble)$/` -- anchored both ends.
pub fn is_branch_mnemonic(mnemonic: &str) -> bool {
    matches!(
        mnemonic,
        "b" | "bx"
            | "beq"
            | "bne"
            | "bcs"
            | "bcc"
            | "bmi"
            | "bpl"
            | "bvs"
            | "bvc"
            | "bhi"
            | "bls"
            | "bge"
            | "blt"
            | "bgt"
            | "ble"
    )
}

// ---------------------------------------------------------------------------
// Canonical rendering (used by the differential harness against the TS)
// ---------------------------------------------------------------------------

impl Operand {
    /// A lossless, unambiguous one-line rendering. Immediates are printed as
    /// raw IEEE-754 bits so `-0` and `0` are distinguishable and no
    /// float-to-decimal formatting difference between the two languages can
    /// mask (or fake) a mismatch.
    pub fn canonical(&self) -> String {
        match self {
            Operand::Reg {
                name,
                number,
                writeback,
            } => format!("reg|{name}|{number}|{}", u8::from(*writeback)),
            Operand::Imm { value } => format!("imm|{:016x}", value.to_bits()),
            Operand::Mem { base, offset_text } => format!("mem|{base}|{offset_text}"),
            Operand::RegList { names } => format!("reglist|{}", names.join("\u{1}")),
            Operand::Label { text } => format!("label|{text}"),
            Operand::Other { text } => format!("other|{text}"),
        }
    }
}

impl Instruction {
    /// Canonical rendering of the whole instruction, minus `raw` (which is just
    /// the input echoed back).
    pub fn canonical(&self) -> String {
        let operands: Vec<String> = self.operands.iter().map(Operand::canonical).collect();
        format!(
            "{}\u{2}{}\u{2}{}\u{2}{}",
            self.offset,
            self.mnemonic,
            operands.join("\u{3}"),
            match primary_dest_register(self) {
                Some(n) => n.to_string(),
                None => "null".to_string(),
            }
        )
    }
}

/// Classification flags for one mnemonic, rendered for the differential
/// harness: store/load/call/branch as `0`/`1`.
pub fn classify(mnemonic: &str) -> String {
    format!(
        "{}{}{}{}",
        u8::from(is_store_mnemonic(mnemonic)),
        u8::from(is_load_mnemonic(mnemonic)),
        u8::from(is_call_mnemonic(mnemonic)),
        u8::from(is_branch_mnemonic(mnemonic)),
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    // --- ports of the TypeScript selfTest() ---------------------------------

    #[test]
    fn parses_a_three_operand_add() {
        let add = parse_instruction(0x6, "adds\tr3, r3, r2");
        assert_eq!(add.mnemonic, "adds");
        assert_eq!(add.operands.len(), 3);
        assert_eq!(
            add.operands[0],
            Operand::Reg {
                name: "r3".into(),
                number: 3,
                writeback: false
            }
        );
        assert_eq!(primary_dest_register(&add), Some(3));
    }

    #[test]
    fn a_stores_first_operand_is_a_source() {
        let store = parse_instruction(0x10, "strb\tr3, [r0, #9]");
        assert_eq!(
            store.operands[0],
            Operand::Reg {
                name: "r3".into(),
                number: 3,
                writeback: false
            }
        );
        assert_eq!(
            store.operands[1],
            Operand::Mem {
                base: "r0".into(),
                offset_text: "#9".into()
            }
        );
        assert_eq!(primary_dest_register(&store), None);
    }

    #[test]
    fn strips_the_trailing_address_comment() {
        let load = parse_instruction(0x0, "ldr\tr3, [pc, #12]\t@ (0x38)");
        assert_eq!(load.mnemonic, "ldr");
        assert_eq!(load.operands.len(), 2);
        assert_eq!(primary_dest_register(&load), Some(3));
        assert_eq!(load.raw, "ldr\tr3, [pc, #12]\t@ (0x38)");
    }

    #[test]
    fn parses_a_register_list() {
        let push = parse_instruction(0x0, "push\t{r5, lr}");
        assert_eq!(
            push.operands[0],
            Operand::RegList {
                names: vec!["r5".into(), "lr".into()]
            }
        );
        assert_eq!(primary_dest_register(&push), None);
    }

    #[test]
    fn parses_writeback_registers() {
        let wb = parse_instruction(0x2, "ldmia\tr0!, {r5}");
        assert_eq!(
            wb.operands[0],
            Operand::Reg {
                name: "r0".into(),
                number: 0,
                writeback: true
            }
        );
    }

    #[test]
    fn a_branch_and_link_takes_a_label() {
        let call = parse_instruction(0x2c, "bl\t0x11f4");
        assert!(is_call_mnemonic(&call.mnemonic));
        assert_eq!(
            call.operands[0],
            Operand::Label {
                text: "0x11f4".into()
            }
        );
    }

    #[test]
    fn register_names_resolve() {
        assert_eq!(register_number("sp"), Some(13));
        assert_eq!(register_number("lr"), Some(14));
        assert_eq!(register_number("pc"), Some(15));
        assert_eq!(register_number("r10"), Some(10));
        assert_eq!(register_number("notareg"), None);
    }

    // --- edge cases the TypeScript self-test missed -------------------------

    #[test]
    fn register_names_are_case_insensitive_but_keep_their_spelling() {
        assert_eq!(register_number("SP"), Some(13));
        assert_eq!(register_number("R7"), Some(7));
        // The `name` field preserves the source spelling, uppercase and all.
        assert_eq!(
            parse_operand("R7"),
            Operand::Reg {
                name: "R7".into(),
                number: 7,
                writeback: false
            }
        );
    }

    #[test]
    fn two_digit_register_cap_is_exactly_two() {
        assert_eq!(register_number("r99"), Some(99)); // matches `\d{1,2}`
        assert_eq!(register_number("r100"), None);
        assert_eq!(register_number("r"), None);
        assert_eq!(register_number("r0x1"), None);
    }

    #[test]
    fn symbolic_labels_are_recognised() {
        assert_eq!(
            parse_operand("11f4 <FooBar+0x8>"),
            Operand::Label {
                text: "11f4 <FooBar+0x8>".into()
            }
        );
        assert_eq!(
            parse_operand("0X1F"),
            Operand::Label {
                text: "0X1F".into()
            }
        );
        // Non-hex leading run: not a label.
        assert!(matches!(parse_operand("zz <x>"), Operand::Other { .. }));
        // Missing closing angle: not a label.
        assert!(matches!(parse_operand("11f4 <x"), Operand::Other { .. }));
    }

    #[test]
    fn negative_and_hex_immediates() {
        assert_eq!(parse_operand("#-4"), Operand::Imm { value: -4.0 });
        assert_eq!(parse_operand("#0x1f"), Operand::Imm { value: 31.0 });
        // `Number("")` is 0, so a bare `#` is a *finite* immediate, not `other`.
        assert_eq!(parse_operand("#"), Operand::Imm { value: 0.0 });
        // Only the text up to the first whitespace is coerced.
        assert_eq!(parse_operand("#12 junk"), Operand::Imm { value: 12.0 });
        // Unparseable -> NaN -> `other`.
        assert!(matches!(parse_operand("#abc"), Operand::Other { .. }));
        assert!(matches!(parse_operand("#Infinity"), Operand::Other { .. }));
    }

    #[test]
    fn js_number_coercion_matches_the_engine() {
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("  7  "), 7.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number(".5"), 0.5);
        assert_eq!(js_number("5."), 5.0);
        assert_eq!(js_number("+3"), 3.0);
        assert!(js_number("1e").is_nan());
        assert!(js_number("12abc").is_nan());
        assert!(js_number("-0x10").is_nan()); // sign is not allowed on radix
        assert!(js_number("Infinity").is_infinite());
        assert_eq!(js_number("-0").to_bits(), (-0.0f64).to_bits());
    }

    #[test]
    fn nested_brackets_do_not_split_operands() {
        let insn = parse_instruction(0, "str\tr1, [r2, r3], {r4, r5}");
        assert_eq!(insn.operands.len(), 3);
        assert_eq!(
            insn.operands[1],
            Operand::Mem {
                base: "r2".into(),
                offset_text: "r3".into()
            }
        );
        assert_eq!(
            insn.operands[2],
            Operand::RegList {
                names: vec!["r4".into(), "r5".into()]
            }
        );
    }

    #[test]
    fn memory_operand_with_multiple_offset_pieces_keeps_the_commas() {
        assert_eq!(
            parse_operand("[r0, r1, lsl #2]"),
            Operand::Mem {
                base: "r0".into(),
                offset_text: "r1,lsl #2".into()
            }
        );
    }

    #[test]
    fn unterminated_memory_operand_drops_the_last_character() {
        // PORT NOTE quirk: `indexOf("]") === -1` -> `slice(1, -1)`.
        assert_eq!(
            parse_operand("[r0, #4"),
            Operand::Mem {
                base: "r0".into(),
                offset_text: "#".into()
            }
        );
        assert_eq!(
            parse_operand("["),
            Operand::Mem {
                base: "".into(),
                offset_text: "".into()
            }
        );
    }

    #[test]
    fn empty_reglist_entries_are_dropped() {
        assert_eq!(
            parse_operand("{r0, , r1}"),
            Operand::RegList {
                names: vec!["r0".into(), "r1".into()]
            }
        );
        assert_eq!(
            parse_operand("{}"),
            Operand::RegList { names: vec![] }
        );
    }

    #[test]
    fn writeback_on_a_non_register_word_falls_through() {
        assert!(matches!(parse_operand("foo!"), Operand::Other { .. }));
        assert!(matches!(parse_operand("!"), Operand::Other { .. }));
        assert_eq!(
            parse_operand("sp!"),
            Operand::Reg {
                name: "sp".into(),
                number: 13,
                writeback: true
            }
        );
    }

    #[test]
    fn mnemonic_only_instruction_has_no_operands() {
        let nop = parse_instruction(0, "nop");
        assert_eq!(nop.mnemonic, "nop");
        assert!(nop.operands.is_empty());
        let empty = parse_instruction(0, "");
        assert_eq!(empty.mnemonic, "");
        assert!(empty.operands.is_empty());
    }

    #[test]
    fn trailing_comma_produces_no_empty_operand_but_a_leading_one_does() {
        assert_eq!(split_operands("r0, r1,"), vec!["r0".to_string(), "r1".into()]);
        assert_eq!(split_operands(",r0"), vec!["".to_string(), "r0".into()]);
    }

    #[test]
    fn classification_prefixes_are_not_whole_words() {
        // These are prefix regexes in the TS, so conditional/suffixed forms hit.
        assert!(is_store_mnemonic("strh"));
        assert!(is_store_mnemonic("streq"));
        assert!(is_load_mnemonic("ldrsb"));
        assert!(is_load_mnemonic("ldmia"));
        assert!(!is_store_mnemonic("ldr"));
        // But call/branch are anchored both ends.
        assert!(is_call_mnemonic("bl"));
        assert!(is_call_mnemonic("blx"));
        assert!(!is_call_mnemonic("bleq"));
        assert!(is_branch_mnemonic("b"));
        assert!(is_branch_mnemonic("ble"));
        assert!(!is_branch_mnemonic("bl"));
        assert!(!is_branch_mnemonic("bxx"));
    }

    #[test]
    fn push_and_pop_classification_is_asymmetric() {
        assert!(is_store_mnemonic("push"));
        assert!(!is_load_mnemonic("push"));
        assert!(is_load_mnemonic("pop"));
        assert!(!is_store_mnemonic("pop"));
    }

    #[test]
    fn pc_relative_load_keeps_pc_as_the_base_not_the_dest() {
        let insn = parse_instruction(0x10, "ldr\tr0, [pc, #24]\t@ (0x2c)");
        assert_eq!(primary_dest_register(&insn), Some(0));
        assert_eq!(
            insn.operands[1],
            Operand::Mem {
                base: "pc".into(),
                offset_text: "#24".into()
            }
        );
    }

    #[test]
    fn multiple_spaces_after_the_mnemonic_are_trimmed() {
        let insn = parse_instruction(0, "movs    r0,   #1");
        assert_eq!(insn.mnemonic, "movs");
        assert_eq!(
            insn.operands,
            vec![
                Operand::Reg {
                    name: "r0".into(),
                    number: 0,
                    writeback: false
                },
                Operand::Imm { value: 1.0 }
            ]
        );
    }

    #[test]
    fn canonical_rendering_is_stable() {
        let insn = parse_instruction(6, "adds\tr3, r3, #2");
        assert_eq!(
            insn.canonical(),
            "6\u{2}adds\u{2}reg|r3|3|0\u{3}reg|r3|3|0\u{3}imm|4000000000000000\u{2}3"
        );
    }
}
