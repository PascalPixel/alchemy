//! `disassemble(binary, base)`.
//!
//! 逆アセンブルを「番地 -> 表記」の対にほどく。並べる際の照合鍵は番地ではなく
//! 先頭からの変位にする。候補と参照は同じ番地に置かれるとは限らないため。

use std::process::Command;

use crate::jsparse::{is_js_space, js_parse_int_radix, trim_js_trailing_space};

/// A `Map<number, string>`: insertion-ordered, last write wins on an existing
/// key while keeping that key's original position.
///
/// PORT NOTE -- NOT a `HashMap`. JavaScript `Map` iteration order is insertion
/// order, and this crate's key-union step iterates it.
#[derive(Debug, Default, Clone)]
pub struct Rows {
    entries: Vec<(f64, String)>,
}

impl Rows {
    pub fn set(&mut self, key: f64, value: String) {
        // `Map` keys compare with SameValueZero, which is `==` for the finite
        // values this can produce.
        if let Some(slot) = self.entries.iter_mut().find(|(k, _)| *k == key) {
            slot.1 = value;
            return;
        }
        self.entries.push((key, value));
    }

    pub fn get(&self, key: f64) -> Option<&str> {
        self.entries
            .iter()
            .find(|(k, _)| *k == key)
            .map(|(_, v)| v.as_str())
    }

    pub fn keys(&self) -> impl Iterator<Item = f64> + '_ {
        self.entries.iter().map(|(k, _)| *k)
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

/// Parse one `objdump -D` row against
/// `/^\s+([0-9a-f]+):\t([0-9a-f ]+)\t(.*)$/`.
///
/// PORT NOTE -- written by hand rather than with the `regex` crate because
/// three JavaScript-specific rules decide whether a line matches:
///
/// * `\s` is JavaScript's set (U+00A0, U+FEFF, U+2028/9, U+3000 in; U+0085
///   out), not `White_Space` and not `[ \t\r\n\f]`.
/// * `[0-9a-f]` carries no `i` flag, so an uppercase-hex objdump would produce
///   an EMPTY table. Reproduced exactly; GNU objdump emits lowercase.
/// * `.` excludes U+000A, U+000D, U+2028 and U+2029, and `$` without the `m`
///   flag anchors at end of input. Together that means a CRLF-terminated line
///   DOES NOT MATCH, so CRLF `objdump` output parses to an empty table. This is
///   the documented CRLF trap and it is reproduced, not papered over: see
///   `crlf_row_does_not_match`.
pub fn parse_row(line: &str) -> Option<(&str, &str, &str)> {
    let mut rest = line;
    // `\s+`
    let mut leading = 0usize;
    for c in rest.chars() {
        if is_js_space(c) {
            leading += c.len_utf8();
        } else {
            break;
        }
    }
    if leading == 0 {
        return None;
    }
    rest = &rest[leading..];
    // `([0-9a-f]+)`
    let address_len = rest
        .bytes()
        .take_while(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b))
        .count();
    if address_len == 0 {
        return None;
    }
    let address = &rest[..address_len];
    rest = &rest[address_len..];
    // `:\t`
    rest = rest.strip_prefix(':')?.strip_prefix('\t')?;
    // `([0-9a-f ]+)` -- greedy, and the class cannot contain the `\t` that must
    // follow, so there is nothing to backtrack.
    let bytes_len = rest
        .bytes()
        .take_while(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(b) || *b == b' ')
        .count();
    if bytes_len == 0 {
        return None;
    }
    let encoded = &rest[..bytes_len];
    rest = &rest[bytes_len..];
    rest = rest.strip_prefix('\t')?;
    // `(.*)$`: `.` excludes the four line terminators, and `$` demands the end
    // of input, so any line terminator in the tail fails the whole match.
    if rest.contains(['\n', '\r', '\u{2028}', '\u{2029}']) {
        return None;
    }
    Some((address, encoded, rest))
}

/// Build the offset table from `objdump` text.
pub fn rows_from_output(output: &str, base: f64) -> Rows {
    let mut rows = Rows::default();
    // `.split("\n")`, NOT `/\r?\n/`: a `\r` survives into the tail and, per
    // `parse_row`, kills the match.
    for line in output.split('\n') {
        if let Some((address, _encoded, text)) = parse_row(line) {
            rows.set(
                js_parse_int_radix(address, 16) - base,
                trim_js_trailing_space(text).to_string(),
            );
        }
    }
    rows
}

/// `disassemble(binary, base)` -- spawns `arm-none-eabi-objdump`.
pub fn disassemble(binary: &str, base: f64) -> Result<Rows, String> {
    let adjust = format!("--adjust-vma=0x{}", hex_lower(base));
    let dumped = Command::new("arm-none-eabi-objdump")
        .args([
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &adjust,
            binary,
        ])
        .output()
        .map_err(|error| format!("objdump failed: {error}"))?;
    if !dumped.status.success() {
        let stderr = String::from_utf8_lossy(&dumped.stderr);
        return Err(format!("objdump failed: {}", stderr.trim()));
    }
    let text = String::from_utf8_lossy(&dumped.stdout).into_owned();
    Ok(rows_from_output(&text, base))
}

/// `base.toString(16)`.
fn hex_lower(value: f64) -> String {
    debug_assert!(value >= 0.0 && value.fract() == 0.0);
    format!("{:x}", value as u64)
}

#[cfg(test)]
mod tests {
    use super::*;

    const ROW: &str = "       0:\t b500      \tpush\t{lr}   ";

    #[test]
    fn a_normal_row_parses_and_the_tail_is_right_trimmed() {
        let rows = rows_from_output(ROW, 0.0);
        assert_eq!(rows.get(0.0), Some("push\t{lr}"));
    }

    #[test]
    fn base_is_subtracted_so_the_key_is_an_offset() {
        let text = "    8000004:\tb500      \tpush\t{lr}";
        let rows = rows_from_output(text, 134_217_728.0);
        assert_eq!(rows.len(), 1);
        assert_eq!(rows.keys().next(), Some(4.0));
    }

    #[test]
    fn crlf_row_does_not_match() {
        // Reproduced JavaScript behaviour: `.` excludes `\r` and `$` anchors at
        // end of input, so a CRLF stream parses to an EMPTY table.
        let rows = rows_from_output("       0:\tb500      \tpush\t{lr}\r\n", 0.0);
        assert!(rows.is_empty());
    }

    #[test]
    fn uppercase_hex_address_does_not_match_no_i_flag() {
        let rows = rows_from_output("       A:\tb500      \tpush\t{lr}", 0.0);
        assert!(rows.is_empty());
    }

    #[test]
    fn header_and_blank_lines_are_skipped() {
        let text = "\ncandidate.bin:     file format binary\n\nDisassembly of section .data:\n\n00000000 <.data>:\n       0:\tb500      \tpush\t{lr}\n";
        let rows = rows_from_output(text, 0.0);
        assert_eq!(rows.len(), 1);
    }

    #[test]
    fn a_row_with_no_leading_whitespace_does_not_match() {
        assert!(parse_row("0:\tb500      \tpush\t{lr}").is_none());
    }

    #[test]
    fn map_is_insertion_ordered_and_last_write_wins() {
        let mut rows = Rows::default();
        rows.set(4.0, "second".into());
        rows.set(0.0, "first".into());
        rows.set(4.0, "replaced".into());
        assert_eq!(rows.keys().collect::<Vec<_>>(), vec![4.0, 0.0]);
        assert_eq!(rows.get(4.0), Some("replaced"));
    }
}
