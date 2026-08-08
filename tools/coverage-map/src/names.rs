// The four filename grammars the coverage map keys everything off.
//
// WHY: which bytes count as byte-exact C, which count as semantic C, and where
// a region ends are all decided by matching a *filename*. The TypeScript spells
// these as four regular expressions; a hand-written matcher has to reproduce
// them exactly, including the parts JavaScript makes easy to get wrong.
//
// PORT NOTE (JS regex semantics preserved here):
//   * `[0-9a-f]` with the `i` flag is ASCII-only in JavaScript -- it never
//     matches a non-ASCII digit -- so `is_ascii_hexdigit` is the right test.
//   * `^`/`$` without the `m` flag anchor the whole string, and JS `$` does NOT
//     allow a trailing newline (unlike some other engines), so these matchers
//     are strict full-string matches.
//   * `[0-9a-f]+` is greedy with backtracking. `SEMANTIC_OVERLAY_SOURCE` needs
//     that: `c` is itself a hex digit, so the greedy run can swallow the `c` of
//     the following `_c_` separator and must be able to give it back.

fn is_hex(character: char) -> bool {
    character.is_ascii_hexdigit()
}

/// `/^(08[0-9a-f]{6})\.c$/i` -- a main-image source named for its address.
pub fn main_source(name: &str) -> Option<&str> {
    let body = name
        .strip_suffix(".c")
        .or_else(|| name.strip_suffix(".C"))?;
    if body.len() != 8 || !body.is_char_boundary(2) {
        return None;
    }
    if !body[..2].eq_ignore_ascii_case("08") || !body.chars().all(is_hex) {
        return None;
    }
    Some(body)
}

/// `/^([0-9a-f]{8})\.s$/i` -- an audited assembly region file.
pub fn assembly_source(name: &str) -> Option<&str> {
    let body = name
        .strip_suffix(".s")
        .or_else(|| name.strip_suffix(".S"))?;
    if body.len() != 8 || !body.chars().all(is_hex) {
        return None;
    }
    Some(body)
}

/// `/^(resource_[0-9a-f]+)_overlay\.s$/i` -- an overlay's assembly source.
pub fn overlay_assembly(name: &str) -> Option<&str> {
    let lower = name.to_ascii_lowercase();
    let body = lower.strip_suffix("_overlay.s")?;
    let digits = body.strip_prefix("resource_")?;
    if digits.is_empty() || !digits.chars().all(is_hex) {
        return None;
    }
    Some(&name[..body.len()])
}

/// `/^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i` -- a semantic overlay owner.
///
/// Returns `(overlay, entry_hex)` as slices of the original, case preserved,
/// exactly like the regex captures.
pub fn semantic_overlay_source(name: &str) -> Option<(&str, &str)> {
    let lower = name.to_ascii_lowercase();
    let body = lower.strip_suffix(".c")?;
    if body.len() < 8 {
        return None;
    }
    let (head, entry) = body.split_at(body.len() - 8);
    if !entry.chars().all(is_hex) {
        return None;
    }
    let overlay = head.strip_suffix("_c_")?;
    let digits = overlay.strip_prefix("resource_")?;
    if digits.is_empty() || !digits.chars().all(is_hex) {
        return None;
    }
    let overlay_len = overlay.len();
    Some((&name[..overlay_len], &name[name.len() - 10..name.len() - 2]))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn main_source_requires_the_08_prefix_and_exact_width() {
        assert_eq!(main_source("08000abc.c"), Some("08000abc"));
        assert_eq!(main_source("0800ABCD.c"), Some("0800ABCD"));
        assert_eq!(main_source("09000abc.c"), None);
        assert_eq!(main_source("08000ab.c"), None);
        assert_eq!(main_source("08000abcd.c"), None);
        assert_eq!(main_source("08000abc.s"), None);
        // JS `$` does not tolerate a trailing newline.
        assert_eq!(main_source("08000abc.c\n"), None);
        // `[0-9a-f]` is ASCII-only even with the `i` flag.
        assert_eq!(main_source("0800ab\u{ff41}c.c"), None);
    }

    #[test]
    fn assembly_source_matches_eight_hex_digits() {
        assert_eq!(assembly_source("08000000.s"), Some("08000000"));
        assert_eq!(assembly_source("0800000.s"), None);
        assert_eq!(assembly_source("alignment.json"), None);
    }

    #[test]
    fn overlay_assembly_captures_the_resource_id() {
        assert_eq!(
            overlay_assembly("resource_1a2_overlay.s"),
            Some("resource_1a2")
        );
        assert_eq!(overlay_assembly("resource__overlay.s"), None);
        assert_eq!(overlay_assembly("resource_1a2_overlay.c"), None);
    }

    #[test]
    fn semantic_overlay_source_gives_back_the_greedy_c() {
        assert_eq!(
            semantic_overlay_source("resource_1a2_c_08001234.c"),
            Some(("resource_1a2", "08001234")),
        );
        // The resource id ends in `c`, which the greedy hex run would otherwise
        // eat from the `_c_` separator.
        assert_eq!(
            semantic_overlay_source("resource_1c_c_08001234.c"),
            Some(("resource_1c", "08001234")),
        );
        assert_eq!(semantic_overlay_source("resource_1a2_c_0800123.c"), None);
        assert_eq!(semantic_overlay_source("08001234.c"), None);
    }
}
