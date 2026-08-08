//! String ordering primitives copied out of JavaScript, not out of Rust.
//!
//! PROVENANCE: this file is a verbatim copy of `tools-rs/mode-cohort/src/collate.rs`,
//! taken deliberately rather than reimplemented. That module's table was
//! MEASURED against 27,225 `localeCompare` triples run in Bun, and writing a
//! third independent implementation of a multi-level collator is how the two
//! existing ones would drift apart. `tools-rs/mode-sweep/src/lib.rs` carries a
//! single-pass interleaved `aAbB` variant which is correct only over the
//! lowercase mode-id alphabet it was calibrated for; do not reach for that one
//! from here, and see `case_is_tertiary_not_primary` below for the pair that
//! separates them.
//!
//! `overlay_mode_cohort.ts` uses two different string orders and they are not
//! the same order. Getting either wrong silently reshuffles the report.

use std::cmp::Ordering;

/// The full printable-ASCII order of the default `en-US` collator, MEASURED in
/// Bun rather than assumed:
///
/// ```text
/// const chars = []; for (let i = 32; i < 127; i++) chars.push(String.fromCharCode(i));
/// chars.sort((a, b) => a.localeCompare(b)).join("")
/// ```
///
/// PORT NOTE -- THIS IS NOT `str::cmp`. ICU root collation puts whitespace and
/// punctuation first, then symbols, then digits, then letters with the
/// lowercase form of each letter ahead of its uppercase form. In raw ASCII
/// `'+'` (0x2B) precedes `'-'` (0x2D) and `'='` (0x3D) follows the digits;
/// under the collator both are the other way round, and every uppercase letter
/// precedes every lowercase one. `mode_cohort.ts` breaks four of its five sort
/// ties with `localeCompare`, so `str::cmp` here reorders real output.
pub const COLLATED_ASCII: &str =
    " _-,;:!?.'\"()[]{}@*/\\&#%`^+<=>|~$0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ";

/// The same table with the case pairs collapsed: this is the PRIMARY strength
/// order.
const COLLATED_PRIMARY: &str =
    " _-,;:!?.'\"()[]{}@*/\\&#%`^+<=>|~$0123456789abcdefghijklmnopqrstuvwxyz";

fn rank(character: char) -> u32 {
    let folded = character.to_ascii_lowercase();
    match COLLATED_PRIMARY
        .chars()
        .position(|candidate| candidate == folded)
    {
        Some(index) => index as u32,
        // Deliberately unreachable for the stems, mode ids and repository-
        // relative source paths this tool sees; `collation_covers_every_*` pins
        // that. Falling back to the scalar value keeps the comparator total so
        // an unexpected character cannot panic mid-sort.
        None => 1000 + character as u32,
    }
}

/// The TERTIARY strength: lowercase sorts before uppercase, and it is only
/// consulted once the whole primary key has compared equal.
fn case_rank(character: char) -> u32 {
    u32::from(character.is_ascii_uppercase())
}

/// `String#localeCompare` under the default `en-US` collator.
///
/// PORT NOTE -- THIS IS GENUINELY MULTI-LEVEL AND A SINGLE-PASS PER-CHARACTER
/// COMPARE IS WRONG. Two measurements pin the shape:
///
///  * `"a-c".localeCompare("ab") === -1`. If ICU shifted punctuation to a
///    variable weight it would compare `"ac"` against `"ab"` and answer `+1`.
///    So punctuation is NOT ignorable, and it carries a primary weight below
///    every letter and digit.
///  * `"A".localeCompare("a-b") === -1`, while `"a".localeCompare("A") === -1`
///    too. A single interleaved `aAbB` table gives `+1` for the first of those.
///    Case is a TERTIARY difference: it is only consulted after the entire
///    primary key has compared equal, so `A` loses to `a-b` on primary length
///    before its case is ever looked at.
///
/// The correct form is therefore two passes: the case-folded primary key first,
/// then case as a tiebreak. `case_is_tertiary_not_primary` pins the pair that
/// distinguishes the two implementations.
pub fn collate(left: &str, right: &str) -> Ordering {
    compare_by(left, right, rank).then_with(|| compare_by(left, right, case_rank))
}

fn compare_by(left: &str, right: &str, weight: fn(char) -> u32) -> Ordering {
    let mut left_ranks = left.chars().map(weight);
    let mut right_ranks = right.chars().map(weight);
    loop {
        match (left_ranks.next(), right_ranks.next()) {
            (None, None) => return Ordering::Equal,
            (None, Some(_)) => return Ordering::Less,
            (Some(_), None) => return Ordering::Greater,
            (Some(a), Some(b)) if a != b => return a.cmp(&b),
            _ => {}
        }
    }
}

/// JavaScript's *default* `Array#sort` comparator: ascending UTF-16 code unit
/// order.
///
/// PORT NOTE -- THIS IS NOT `Vec<String>::sort` AND NOT `localeCompare`.
/// `mode_cohort.ts` writes both `stems.sort()` (default) and
/// `sort((l, r) => l.stem.localeCompare(r.stem))` within ten lines of each
/// other, and they disagree the moment a stem contains punctuation or an
/// uppercase letter. The default comparator is also not `str::cmp`: Rust
/// compares UTF-8 bytes, so a character in U+E000..U+FFFF sorts *after* an
/// astral character in UTF-8 but *before* it in UTF-16.
pub fn cmp_utf16(left: &str, right: &str) -> Ordering {
    let mut left_units = left.encode_utf16();
    let mut right_units = right.encode_utf16();
    loop {
        match (left_units.next(), right_units.next()) {
            (None, None) => return Ordering::Equal,
            (None, Some(_)) => return Ordering::Less,
            (Some(_), None) => return Ordering::Greater,
            (Some(a), Some(b)) if a != b => return a.cmp(&b),
            _ => {}
        }
    }
}

/// `items.sort()` with no comparator.
pub fn sort_default(items: &mut [String]) {
    items.sort_by(|left, right| cmp_utf16(left, right));
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn collated_ascii_is_the_whole_printable_range() {
        let mut seen: Vec<char> = COLLATED_ASCII.chars().collect();
        assert_eq!(seen.len(), 95, "measured table must cover U+0020..U+007E");
        seen.sort_unstable();
        seen.dedup();
        assert_eq!(seen.len(), 95, "measured table must not repeat a character");
        assert_eq!(seen[0], ' ');
        assert_eq!(seen[94], '~');
    }

    #[test]
    fn collation_is_not_ascii_order() {
        // The three orderings that a `str::cmp` "simplification" would flip.
        assert_eq!(collate("a-b", "a+b"), Ordering::Less);
        assert_eq!("a-b".cmp("a+b"), Ordering::Greater);
        assert_eq!(collate("a=b", "a0b"), Ordering::Less);
        assert_eq!("a=b".cmp("a0b"), Ordering::Greater);
        assert_eq!(collate("a", "A"), Ordering::Less);
        assert_eq!("a".cmp("A"), Ordering::Greater);
    }

    #[test]
    fn punctuation_is_not_variable() {
        // Measured in Bun: "a-c".localeCompare("ab") === -1. If ICU shifted
        // punctuation to a variable weight this would be +1 and the whole
        // per-character form would be wrong.
        assert_eq!(collate("a-c", "ab"), Ordering::Less);
        assert_eq!(collate("a+c", "ab"), Ordering::Less);
        assert_eq!(collate("a=c", "ab"), Ordering::Less);
        assert_eq!(collate("no-gcse", "nogcse"), Ordering::Less);
    }

    #[test]
    fn shorter_prefix_sorts_first() {
        assert_eq!(collate("a", "ab"), Ordering::Less);
        assert_eq!(collate("ab", "a"), Ordering::Greater);
        assert_eq!(collate("ab", "ab"), Ordering::Equal);
    }

    #[test]
    fn agrees_with_mode_sweep_on_the_mode_id_alphabet() {
        // Two independently calibrated tables must not drift apart. Every
        // ordered pair over the alphabet `mode-sweep` calibrated for.
        let alphabet: Vec<char> = mode_sweep::COLLATED_ALPHABET.chars().collect();
        for &left in &alphabet {
            for &right in &alphabet {
                let a = format!("x{left}y");
                let b = format!("x{right}y");
                assert_eq!(
                    collate(&a, &b),
                    mode_sweep::collate(&a, &b),
                    "collators disagree on {a:?} vs {b:?}"
                );
            }
        }
    }

    #[test]
    fn collation_covers_every_mode_id_and_stem_character() {
        for config in mode_sweep::single_configs() {
            for id in &config.ids {
                for character in id.chars() {
                    assert!(
                        COLLATED_ASCII.contains(character),
                        "mode id {id:?} uses {character:?}, outside the measured table"
                    );
                }
            }
        }
        // Stems are eight lowercase hex digits from `basename(source, ".c")`.
        for character in "0123456789abcdef".chars() {
            assert!(COLLATED_ASCII.contains(character));
        }
    }

    #[test]
    fn default_sort_is_code_unit_not_locale() {
        let mut items = vec!["a-b".to_string(), "a+b".to_string(), "A".to_string(), "a".to_string()];
        sort_default(&mut items);
        // JS: ["a+b","a-b","A","a"].sort() === ["A","a","a+b","a-b"]
        assert_eq!(items, vec!["A", "a", "a+b", "a-b"]);
        let mut located = vec!["a-b".to_string(), "a+b".to_string(), "A".to_string(), "a".to_string()];
        located.sort_by(|l, r| collate(l, r));
        // Measured in Bun: sort((l, r) => l.localeCompare(r)).
        assert_eq!(located, vec!["a", "A", "a-b", "a+b"]);
    }

    #[test]
    fn case_is_tertiary_not_primary() {
        // The pair that a one-pass interleaved `aAbB` table gets backwards.
        assert_eq!(collate("A", "a-b"), Ordering::Less);
        assert_eq!(collate("a", "A"), Ordering::Less);
        assert_eq!(collate("ab", "aB"), Ordering::Less);
        assert_eq!(collate("aB", "ab"), Ordering::Greater);
        // Primary still wins over case.
        assert_eq!(collate("aB", "ac"), Ordering::Less);
    }

    #[test]
    fn utf16_order_differs_from_utf8_byte_order() {
        // U+E000 (private use) vs U+10000 (astral). UTF-16: 0xE000 < 0xD800
        // is false, the surrogate lead 0xD800 is *below* 0xE000, so the astral
        // character sorts first. UTF-8 byte order says the opposite.
        let bmp = "\u{e000}".to_string();
        let astral = "\u{10000}".to_string();
        assert_eq!(cmp_utf16(&astral, &bmp), Ordering::Less);
        assert_eq!(astral.cmp(&bmp), Ordering::Greater);
    }
}
