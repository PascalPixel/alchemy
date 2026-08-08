//! List semantic sources that an exact source at the same address has superseded.
//!
//! Port of `tools/semantic/semantic_superseded.ts`.
//!
//! PORT NOTE: the TypeScript imports `canonicalCSource` from
//! `tools/lib/full_c_progress.ts`. Only that one function is needed, so it is
//! inlined here as [`canonical_c_source`] with its four rejection patterns
//! hand-rolled (no regex crate):
//!   * `/\bregister\b[^;\n]*\basm\s*\(/`
//!   * `/\b__asm__\b|\basm\s+volatile\b/`
//!   * `/\.incbin\b/`
//!   * `/\bM2C_ERROR\b/`
//!
//! Nothing else from that module is used or ported.

/// Each pair is (semantic directory, the exact directory that supersedes it).
/// A semantic source is superseded when the exact directory holds the same file
/// name -- both source trees name a file after the address it owns, so equal
/// names mean equal addresses. Disk mode additionally requires canonical C.
/// Main-image and overlay sources share one flat directory each since the
/// exact/semantic tree consolidation, so this is a single pair now.
pub const SOURCE_PAIRS: &[(&str, &str)] = &[("semantic", "exact")];

/// Names of semantic sources superseded by an exact source, sorted.
///
/// `listing` yields the file names in a source directory; `present` reports
/// whether a superseding exact source exists at the given repo-relative path.
pub fn superseded_sources(
    listing: &mut dyn FnMut(&str) -> Vec<String>,
    present: &mut dyn FnMut(&str) -> bool,
) -> Vec<String> {
    let mut found: Vec<String> = Vec::new();
    for (semantic, exact) in SOURCE_PAIRS {
        for name in listing(semantic) {
            if !name.ends_with(".c") {
                continue;
            }
            if present(&format!("{exact}/{name}")) {
                found.push(format!("{semantic}/{name}"));
            }
        }
    }
    // PORT NOTE: JS `Array.prototype.sort` compares UTF-16 code units; Rust
    // sorts byte-wise. The two orders differ only for non-BMP or high-plane
    // characters, which source file names never contain.
    found.sort();
    found
}

fn is_word_byte(b: u8) -> bool {
    b.is_ascii_alphanumeric() || b == b'_'
}

/// `\b` at byte offset `at` in `text` (word boundary between `at-1` and `at`).
fn boundary_at(text: &[u8], at: usize) -> bool {
    let before = at > 0 && is_word_byte(text[at - 1]);
    let after = at < text.len() && is_word_byte(text[at]);
    before != after
}

/// True when `needle` starts at `at`, preceded by a word boundary.
fn word_start(text: &[u8], at: usize, needle: &[u8]) -> bool {
    text[at..].starts_with(needle) && boundary_at(text, at)
}

/// True when `needle` sits at `at` bounded by word boundaries on both sides.
fn whole_word(text: &[u8], at: usize, needle: &[u8]) -> bool {
    word_start(text, at, needle) && boundary_at(text, at + needle.len())
}

/// `\s` exactly as JavaScript defines it: WhiteSpace + LineTerminator, i.e.
/// TAB LF VT FF CR SP, U+00A0, U+FEFF, the Zs category, and U+2028/U+2029.
///
/// PORT NOTE: this deliberately is NOT Rust's `char::is_whitespace`
/// (Unicode `White_Space`), which includes U+0085 that JS excludes and omits
/// U+FEFF that JS includes.
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
            | '\u{feff}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
    )
}

/// Length in bytes of the `\s` run starting at `at`, or 0 when none.
///
/// Scanning is byte-oriented, so a UTF-8 continuation byte at `at` (mid
/// character) is not whitespace and yields 0 -- which is correct, since a JS
/// regex could never begin a `\s` match there either.
fn space_run(text: &[u8], at: usize) -> usize {
    let mut cursor = at;
    while cursor < text.len() {
        match decode_char(text, cursor) {
            Some((c, width)) if is_js_space(c) => cursor += width,
            _ => break,
        }
    }
    cursor - at
}

/// Decode the UTF-8 character starting at `at`, or `None` if the bytes there
/// are not a well-formed character start. Inputs come from
/// `String::from_utf8_lossy`, so malformed sequences never survive to here.
fn decode_char(text: &[u8], at: usize) -> Option<(char, usize)> {
    let width = match text[at] {
        0x00..=0x7f => 1,
        0xc0..=0xdf => 2,
        0xe0..=0xef => 3,
        0xf0..=0xf7 => 4,
        _ => return None,
    };
    let end = at.checked_add(width)?;
    if end > text.len() {
        return None;
    }
    let c = std::str::from_utf8(&text[at..end]).ok()?.chars().next()?;
    Some((c, width))
}

/// `/\bregister\b[^;\n]*\basm\s*\(/`
fn register_asm_call(text: &[u8]) -> bool {
    for start in 0..text.len() {
        if !whole_word(text, start, b"register") {
            continue;
        }
        let mut at = start + b"register".len();
        while at < text.len() {
            if word_start(text, at, b"asm") {
                let after = at + 3 + space_run(text, at + 3);
                if after < text.len() && text[after] == b'(' {
                    return true;
                }
            }
            // `[^;\n]*` may not step over a `;` or a newline.
            if text[at] == b';' || text[at] == b'\n' {
                break;
            }
            at += 1;
        }
    }
    false
}

/// `/\b__asm__\b|\basm\s+volatile\b/`
fn asm_keyword(text: &[u8]) -> bool {
    for at in 0..text.len() {
        if whole_word(text, at, b"__asm__") {
            return true;
        }
        if word_start(text, at, b"asm") {
            // `\s+` needs at least one space character.
            let spaces = space_run(text, at + 3);
            let after = at + 3 + spaces;
            if spaces > 0 && whole_word(text, after, b"volatile") {
                return true;
            }
        }
    }
    false
}

fn contains_word(text: &[u8], needle: &[u8]) -> bool {
    (0..text.len()).any(|at| whole_word(text, at, needle))
}

/// `/\.incbin\b/` -- a literal dot then `incbin` then a word boundary.
fn incbin(text: &[u8]) -> bool {
    (0..text.len()).any(|at| {
        text[at] == b'.'
            && text[at + 1..].starts_with(b"incbin")
            && boundary_at(text, at + 1 + b"incbin".len())
    })
}

/// Inlined from `tools/lib/full_c_progress.ts`.
pub fn canonical_c_source(source: &str) -> bool {
    let text = source.as_bytes();
    !(register_asm_call(text)
        || asm_keyword(text)
        || incbin(text)
        || contains_word(text, b"M2C_ERROR"))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn ts_listing(directory: &str) -> Vec<String> {
        if directory == "semantic" {
            [
                "resource_3aa_c_02000230.c",
                "resource_3aa_c_02000400.c",
                "notes.md",
                "08006d50.c",
            ]
            .iter()
            .map(|s| s.to_string())
            .collect()
        } else {
            Vec::new()
        }
    }

    /// The TypeScript `--self-test`, verbatim.
    #[test]
    fn self_test_matches_typescript() {
        let found = superseded_sources(
            &mut ts_listing,
            &mut |path: &str| {
                path == "exact/resource_3aa_c_02000230.c" || path == "exact/08006d50.c"
            },
        );
        assert_eq!(
            found,
            vec!["semantic/08006d50.c", "semantic/resource_3aa_c_02000230.c"]
        );
    }

    /// Second half of the TypeScript self-test: no exact counterpart, no report.
    #[test]
    fn nothing_superseded_when_no_exact_counterpart() {
        assert!(superseded_sources(&mut ts_listing, &mut |_| false).is_empty());
    }

    #[test]
    fn non_c_files_are_never_probed_or_reported() {
        let mut probed: Vec<String> = Vec::new();
        let found = superseded_sources(
            &mut |_| {
                ["notes.md", "a.c", "b.h", "c.s", "d.c.bak", ".c"]
                    .iter()
                    .map(|s| s.to_string())
                    .collect()
            },
            &mut |path| {
                probed.push(path.to_string());
                true
            },
        );
        // `.c` ends with ".c" in JS too, so it is probed and reported.
        assert_eq!(probed, vec!["exact/a.c", "exact/.c"]);
        assert_eq!(found, vec!["semantic/.c", "semantic/a.c"]);
    }

    #[test]
    fn output_is_sorted() {
        let found = superseded_sources(
            &mut |_| {
                ["z.c", "a.c", "m.c"]
                    .iter()
                    .map(|s| s.to_string())
                    .collect()
            },
            &mut |_| true,
        );
        assert_eq!(found, vec!["semantic/a.c", "semantic/m.c", "semantic/z.c"]);
    }

    #[test]
    fn canonical_accepts_plain_c() {
        assert!(canonical_c_source(
            "int main(void) {\n  register int x = 1;\n  return x;\n}\n"
        ));
        assert!(canonical_c_source(""));
        // Substrings that only look like the keywords.
        assert!(canonical_c_source("int disasm(void);\nint registers;\n"));
        assert!(canonical_c_source("char *s = \"incbinary\";\n"));
        assert!(canonical_c_source("int M2C_ERRORS;\n"));
        assert!(canonical_c_source("int xM2C_ERROR;\n"));
    }

    #[test]
    fn canonical_rejects_register_asm() {
        assert!(!canonical_c_source("register int r asm(\"r0\");"));
        assert!(!canonical_c_source("register int r asm (\"r0\");"));
        // The `[^;\n]*` gap may not cross a `;` or a newline.
        assert!(canonical_c_source("register int r; int q = asm(\"x\");"));
        assert!(canonical_c_source("register int r\nint q = asm(\"x\");"));
        // ...but a later `register` on its own line can still match.
        assert!(!canonical_c_source(
            "register int a;\nregister int b asm(\"r1\");"
        ));
        // `asm` needs a leading word boundary and a following `(`.
        assert!(canonical_c_source("register int rasm(\"r0\");"));
        assert!(canonical_c_source("register int r asm \"r0\";"));
    }

    #[test]
    fn canonical_rejects_asm_keywords() {
        assert!(!canonical_c_source("__asm__(\"nop\");"));
        assert!(!canonical_c_source("  asm volatile (\"nop\");"));
        assert!(!canonical_c_source("asm\tvolatile(\"nop\");"));
        assert!(!canonical_c_source("asm\nvolatile(\"nop\");"));
        // `\s+` requires at least one space, and both ends need boundaries.
        assert!(canonical_c_source("asmvolatile(\"nop\");"));
        assert!(canonical_c_source("asm volatiles;"));
        assert!(canonical_c_source("x__asm__();"));
        assert!(canonical_c_source("__asm__x();"));
    }

    #[test]
    fn canonical_rejects_incbin_and_m2c_error() {
        assert!(!canonical_c_source("  .incbin \"baserom.gba\"\n"));
        assert!(!canonical_c_source("x.incbin;"));
        assert!(canonical_c_source(".incbinary"));
        assert!(!canonical_c_source("  M2C_ERROR(unknown instruction);\n"));
        assert!(!canonical_c_source("M2C_ERROR"));
    }

    /// `\s` in a JS regex is not Rust's `White_Space`: it includes U+FEFF and
    /// excludes U+0085. Both `asm\s+volatile` and `asm\s*\(` depend on it.
    #[test]
    fn js_whitespace_set_not_unicode_white_space() {
        for space in ["\u{a0}", "\u{feff}", "\u{2028}", "\u{3000}", "\u{200a}"] {
            assert!(
                !canonical_c_source(&format!("asm{space}volatile(0);")),
                "expected {space:?} to satisfy JS \\s"
            );
            assert!(!canonical_c_source(&format!(
                "register int r asm{space}(\"r0\");"
            )));
        }
        // U+0085 is Unicode White_Space but NOT JavaScript `\s`.
        assert!(canonical_c_source("asm\u{85}volatile(0);"));
        // `\s*` means zero spaces is fine for the `asm(` form...
        assert!(!canonical_c_source("register int r asm(\"r0\");"));
        // ...but `\s+` means zero spaces is not, for the volatile form.
        assert!(canonical_c_source("asm\u{85}volatile;"));
    }

    /// A CRLF file must behave exactly like an LF one: the `[^;\n]*` gap in
    /// the register pattern stops at `\n`, and `\r` is an ordinary gap byte.
    #[test]
    fn crlf_input_matches_lf_input() {
        assert!(!canonical_c_source("register int r asm(\"r0\");\r\n"));
        assert!(canonical_c_source("register int r;\r\nint q = asm(\"x\");\r\n"));
        assert!(!canonical_c_source("a\r\n__asm__(\"nop\");\r\n"));
    }

    #[test]
    fn canonical_handles_multibyte_utf8() {
        // Byte scanning must not trip over non-ASCII comment text.
        assert!(canonical_c_source("/* naïve — ok */\nint x;\n"));
        assert!(!canonical_c_source("/* naïve */\n__asm__(\"nop\");\n"));
    }
}
