//! The two regular expressions and the byte scorer.
//!
//! Both patterns carry the `i` flag in the TypeScript, and both are hand-rolled
//! here rather than handed to the `regex` crate. The reason is not performance:
//! it is that a hand-rolled matcher makes the case rule and the anchor rule
//! visible at the point of use, and a missing `i` flag is a defect this
//! repository has already shipped once (see `overlay-disasm`'s
//! `bias_in_image_label_words`, where the ABSENCE of `i` is load-bearing).

use crate::jsops::{js_length, js_parse_int, js_slice_from, js_slice_to};

/// `byteDifference(actual, expected)`.
///
/// PORT NOTE: the extent is `Math.max` of the two lengths, so every byte past
/// the shorter buffer counts as a difference. That makes a short compile score
/// worse than a wrong one of the right length, which is intended.
pub struct ByteDifference {
    pub count: usize,
    pub first: Option<usize>,
}

pub fn byte_difference(actual: &[u8], expected: &[u8]) -> ByteDifference {
    let mut count = 0usize;
    let mut first: Option<usize> = None;
    let extent = actual.len().max(expected.len());
    for offset in 0..extent {
        if offset < actual.len() && offset < expected.len() && actual[offset] == expected[offset] {
            continue;
        }
        count += 1;
        if first.is_none() {
            first = Some(offset);
        }
    }
    ByteDifference { count, first }
}

pub struct CandidateSpec {
    pub id: String,
    pub source: String,
}

/// `candidateSpec(value)`.
///
/// PORT NOTE -- `marker < 1`, NOT `marker < 0`. A spec that opens with `=` has
/// `indexOf("=") === 0` and is rejected, because an empty id is not a valid
/// owner. Porting the guard as "was an `=` found" would accept `=x.c`.
pub fn candidate_spec(value: &str) -> Result<CandidateSpec, String> {
    let message = format!("candidate must be OVERLAY:OFFSET=PATH, got: {value}");
    let marker = match index_of(value, '=') {
        Some(marker) if marker >= 1 => marker,
        _ => return Err(message),
    };
    let id = js_slice_to(value, marker).to_lowercase();
    let source = js_slice_from(value, marker + 1);
    if !is_owner_id(&id) || js_length(&source) == 0 {
        return Err(message);
    }
    Ok(CandidateSpec { id, source })
}

/// `value.indexOf("=")` in UTF-16 code units.
fn index_of(text: &str, needle: char) -> Option<usize> {
    let mut index = 0usize;
    for character in text.chars() {
        if character == needle {
            return Some(index);
        }
        index += character.len_utf16();
    }
    None
}

/// `/^[a-z0-9_]+:[0-9a-f]{4,}$/i`.
///
/// PORT NOTE -- FOUR TRAPS, ALL LIVE:
///
///  * The `i` flag is PRESENT, so `RESOURCE_394:07E0` is accepted. The caller
///    lowercases before testing, which makes the flag look redundant; it is not,
///    because `String#toLowerCase` is locale-independent while the `i` flag is
///    the one that would matter if that call were ever removed.
///  * `[a-z0-9_]` and `[0-9a-f]` are ASCII RANGES, not `\w` and not `is_digit`.
///    JavaScript's `\w` is ASCII-only too, but Rust's `char::is_alphanumeric`
///    is not, so `resource_୩୪` would pass a Unicode-aware port.
///  * `^` and `$` are STRING anchors here: the pattern has no `m` flag, so a
///    trailing newline is NOT allowed. Rust's `regex` crate `$` also refuses a
///    trailing newline, but JavaScript's `$` under `m` would allow it and the
///    two are one flag apart.
///  * `{4,}` is a MINIMUM, not an exact count, so a full eight-digit address
///    passes as readily as a four-digit offset.
fn is_owner_id(id: &str) -> bool {
    let colon = match id.find(':') {
        Some(index) => index,
        None => return false,
    };
    let (head, tail) = (&id[..colon], &id[colon + 1..]);
    if head.is_empty()
        || !head
            .chars()
            .all(|c| c.is_ascii_lowercase() || c.is_ascii_uppercase() || c.is_ascii_digit() || c == '_')
    {
        return false;
    }
    // `[0-9a-f]` under `i` is exactly `[0-9a-fA-F]`; `is_ascii_hexdigit` is the
    // same set and nothing wider.
    tail.len() >= 4 && tail.chars().all(|c| c.is_ascii_hexdigit())
}

/// `/(?:^|_)c_(020[0-9a-f]{5})\.c$/i` applied to `basename(source)`, returning
/// the captured address.
///
/// PORT NOTE: the pattern is anchored at the end, so the match is unique and
/// the matcher can work backwards without backtracking. The leading
/// `(?:^|_)` means `abc_020007e0.c` does NOT match (no `c_` separator) while
/// both `resource_394_c_020007e0.c` and `c_020007e0.c` do.
pub fn address_in_basename(basename: &str) -> Option<f64> {
    let units: Vec<char> = basename.chars().collect();
    // `.c$`, case-insensitive.
    let length = units.len();
    if length < 2 + 8 + 2 {
        return None;
    }
    if units[length - 2] != '.' || !units[length - 1].eq_ignore_ascii_case(&'c') {
        return None;
    }
    let digits_start = length - 2 - 8;
    let digits: String = units[digits_start..length - 2].iter().collect();
    // Hex-digit test FIRST: the `020` prefix test slices by byte and a
    // multi-byte character in that window would panic on a `char` boundary.
    if !digits.chars().all(|c| c.is_ascii_hexdigit()) || !digits[..3].eq_ignore_ascii_case("020") {
        return None;
    }
    // `c_` immediately before, and either the string start or `_` before that.
    if digits_start < 2 {
        return None;
    }
    if !units[digits_start - 2].eq_ignore_ascii_case(&'c') || units[digits_start - 1] != '_' {
        return None;
    }
    if digits_start - 2 != 0 && units[digits_start - 3] != '_' {
        return None;
    }
    Some(js_parse_int(&digits, 16))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn byte_difference_counts_the_tail_of_the_longer_buffer() {
        let difference = byte_difference(&[1, 2, 3], &[1, 4]);
        assert_eq!(difference.count, 2);
        assert_eq!(difference.first, Some(1));
        let equal = byte_difference(&[1, 2], &[1, 2]);
        assert_eq!(equal.count, 0);
        assert_eq!(equal.first, None);
        // A correct prefix that runs long is still wrong.
        let long = byte_difference(&[1, 2, 3, 4], &[1, 2]);
        assert_eq!(long.count, 2);
        assert_eq!(long.first, Some(2));
    }

    #[test]
    fn candidate_spec_rejects_a_leading_equals() {
        // `indexOf("=") === 0`, which `marker < 1` rejects and `marker < 0`
        // would accept.
        assert!(candidate_spec("=x.c").is_err());
        assert!(candidate_spec("resource_394:07e0=").is_err());
        assert!(candidate_spec("resource_394=x.c").is_err());
        let parsed = candidate_spec("resource_394:07e0=semantic/o/x.c").expect("valid");
        assert_eq!(parsed.id, "resource_394:07e0");
        assert_eq!(parsed.source, "semantic/o/x.c");
    }

    #[test]
    fn candidate_spec_is_case_insensitive_and_lowercases() {
        let parsed = candidate_spec("RESOURCE_394:07E0=X.c").expect("valid");
        assert_eq!(parsed.id, "resource_394:07e0");
        // Only the id is lowercased; the path keeps its case because macOS is
        // case-insensitive but the routing tables are not.
        assert_eq!(parsed.source, "X.c");
    }

    #[test]
    fn owner_id_rejects_non_ascii_and_short_offsets() {
        assert!(!is_owner_id("resource_394:07e"));
        assert!(is_owner_id("resource_394:07e0"));
        assert!(!is_owner_id("resource_39\u{b3}:07e0"));
        assert!(!is_owner_id("resource_394:07g0"));
        assert!(!is_owner_id(":07e0"));
        // No `m` flag: a trailing newline must not be accepted.
        assert!(!is_owner_id("resource_394:07e0\n"));
    }

    #[test]
    fn address_pattern_requires_the_c_separator_and_the_020_bank() {
        assert_eq!(address_in_basename("resource_394_c_020007e0.c"), Some(0x020007e0 as f64));
        assert_eq!(address_in_basename("c_020007e0.c"), Some(0x020007e0 as f64));
        assert_eq!(address_in_basename("RESOURCE_394_C_020007E0.C"), Some(0x020007e0 as f64));
        // No `c_` separator.
        assert_eq!(address_in_basename("resource_394_020007e0.c"), None);
        // Wrong bank.
        assert_eq!(address_in_basename("resource_394_c_030007e0.c"), None);
        // `$` is a string anchor with no `m` flag.
        assert_eq!(address_in_basename("resource_394_c_020007e0.c\n"), None);
        // `(?:^|_)`: a letter before `c_` does not satisfy the alternation.
        assert_eq!(address_in_basename("xc_020007e0.c"), None);
    }
}
