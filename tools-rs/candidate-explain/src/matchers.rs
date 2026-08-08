//! Hand-written equivalents of the four regular expressions in
//! `candidate_explain.ts`.
//!
//! PORT NOTE -- none of these uses the `regex` crate, and that is a decision,
//! not an omission. Four separate JavaScript/Rust regex divergences bite this
//! file specifically:
//!
//!   * `\s` differs on U+0085 / U+00A0 / U+FEFF / U+2028 / U+2029.
//!   * `\d` and `\w` are ASCII-only in JavaScript and Unicode-aware in Rust.
//!   * JavaScript's `.` excludes the four line terminators, so a CRLF input
//!     makes `(.*)$` fail outright instead of capturing a trailing `\r`.
//!   * JavaScript's `$` without the `m` flag matches only at the very end of
//!     the subject and does NOT tolerate a trailing newline, where Rust's `$`
//!     in multi-line mode would.
//!
//! Each function below states which pattern it reproduces and which flags that
//! pattern carries. The `i` flag is load-bearing on exactly one of the four; a
//! port that adds it to the others, or drops it from that one, is wrong in a
//! way `tests/js_traps.rs` catches.

use crate::jsnum::{is_js_line_terminator, is_js_whitespace};

/// `/^\s+([0-9a-f]+):\t[0-9a-f ]+\t(.*)$/.exec(line)` -- NO flags.
///
/// Returns `(offset_digits, tail)` on a match.
///
/// PORT NOTE -- the hex class is lowercase-only and the pattern carries no `i`
/// flag. objdump emits lowercase, so adding `i` would look harmless; it would
/// also silently start accepting rows this tool is specified to skip. The
/// missing-`i` class of defect is confirmed live in this repository, so both
/// directions are pinned by test.
pub fn objdump_row(line: &str) -> Option<(&str, &str)> {
    let bytes = line.as_bytes();
    let mut index = 0usize;

    // `\s+`
    let start = index;
    while index < bytes.len() {
        let c = line[index..].chars().next()?;
        if is_js_whitespace(c) || is_js_line_terminator(c) {
            index += c.len_utf8();
        } else {
            break;
        }
    }
    if index == start {
        return None;
    }

    // `([0-9a-f]+)`
    let offset_start = index;
    while index < bytes.len() && is_lower_hex(bytes[index]) {
        index += 1;
    }
    if index == offset_start {
        return None;
    }
    let offset_digits = &line[offset_start..index];

    // `:` `\t`
    if !line[index..].starts_with(":\t") {
        return None;
    }
    index += 2;

    // `[0-9a-f ]+`
    let bytes_start = index;
    while index < bytes.len() && (is_lower_hex(bytes[index]) || bytes[index] == b' ') {
        index += 1;
    }
    if index == bytes_start {
        return None;
    }

    // `\t`
    if !line[index..].starts_with('\t') {
        return None;
    }
    index += 1;

    // `(.*)$` -- `.` excludes line terminators and `$` is end-of-subject.
    let tail = &line[index..];
    if tail.chars().any(is_js_line_terminator) {
        return None;
    }
    Some((offset_digits, tail))
}

fn is_lower_hex(byte: u8) -> bool {
    byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte)
}

/// `/Ready list \(t = *\d+\): *(.*)$/.exec(line)` -- NO flags, unanchored.
///
/// PORT NOTE -- `\d` is ASCII-only in JavaScript. `[[:digit:]]` in Rust's
/// `regex` is too, but `char::is_numeric` is not, so the digit scan is spelled
/// `is_ascii_digit` rather than delegated.
pub fn ready_list(line: &str) -> Option<&str> {
    const HEAD: &str = "Ready list (t =";
    // `.exec` scans every start position; reproduce that rather than assuming
    // the first occurrence is the only one.
    let mut from = 0usize;
    while let Some(found) = line[from..].find(HEAD) {
        let at = from + found;
        if let Some(tail) = ready_list_at(line, at + HEAD.len()) {
            return Some(tail);
        }
        from = at + 1;
        if from > line.len() {
            break;
        }
    }
    None
}

fn ready_list_at(line: &str, mut index: usize) -> Option<&str> {
    let bytes = line.as_bytes();
    // ` *`
    while index < bytes.len() && bytes[index] == b' ' {
        index += 1;
    }
    // `\d+`
    let digits_start = index;
    while index < bytes.len() && bytes[index].is_ascii_digit() {
        index += 1;
    }
    if index == digits_start {
        return None;
    }
    // `\):`
    if !line[index..].starts_with("):") {
        return None;
    }
    index += 2;
    // ` *`
    while index < bytes.len() && bytes[index] == b' ' {
        index += 1;
    }
    // `(.*)$`
    let tail = &line[index..];
    if tail.chars().any(is_js_line_terminator) {
        return None;
    }
    Some(tail)
}

/// `new RegExp("--> scheduling insn <<<(\\d+)>>>")` -- NO flags, unanchored.
///
/// `.test(line)` is [`scheduling_insn_present`]; `.exec(line)[1]` is the
/// returned digit run.
pub fn scheduling_insn(line: &str) -> Option<&str> {
    const HEAD: &str = "--> scheduling insn <<<";
    let bytes = line.as_bytes();
    let mut from = 0usize;
    while let Some(found) = line[from..].find(HEAD) {
        let at = from + found;
        let mut index = at + HEAD.len();
        let digits_start = index;
        while index < bytes.len() && bytes[index].is_ascii_digit() {
            index += 1;
        }
        if index > digits_start && line[index..].starts_with(">>>") {
            return Some(&line[digits_start..index]);
        }
        from = at + 1;
        if from > line.len() {
            break;
        }
    }
    None
}

/// `.test(...)` for the same pattern.
pub fn scheduling_insn_present(line: &str) -> bool {
    scheduling_insn(line).is_some()
}

/// `/^(resource_[0-9a-f]+):(?:0x)?([0-9a-f]+)$/i.exec(id)`.
///
/// Returns `(overlay, offset_digits)`.
///
/// PORT NOTE -- this pattern DOES carry the `i` flag, so `RESOURCE_0A:FF` is a
/// match and yields `RESOURCE_0A` verbatim as the overlay name (the capture is
/// the input text, not a lowercased form). Dropping the flag would make every
/// uppercase overlay id fall through to main-image mode and try to compile a
/// nonexistent `.c` file. `$` is end-of-subject: a trailing newline does not
/// match.
pub fn overlay_id(id: &str) -> Option<(&str, &str)> {
    let lower = id.to_ascii_lowercase();
    let bytes = lower.as_bytes();
    if !lower.starts_with("resource_") {
        return None;
    }
    let mut index = "resource_".len();
    let hex_start = index;
    while index < bytes.len() && is_lower_hex(bytes[index]) {
        index += 1;
    }
    if index == hex_start {
        return None;
    }
    let overlay_end = index;
    if !lower[index..].starts_with(':') {
        return None;
    }
    index += 1;
    // `(?:0x)?` -- optional, and `x` is not a hex digit so the alternative
    // never competes with the capture group for the same text.
    if lower[index..].starts_with("0x") {
        index += 2;
    }
    let offset_start = index;
    while index < bytes.len() && is_lower_hex(bytes[index]) {
        index += 1;
    }
    if index == offset_start || index != bytes.len() {
        return None;
    }
    // Captures come from the ORIGINAL subject, not the lowercased copy. The
    // ASCII-only lowercasing above keeps byte offsets aligned between the two.
    Some((&id[..overlay_end], &id[offset_start..index]))
}
