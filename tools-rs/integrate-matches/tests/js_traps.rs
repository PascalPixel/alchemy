//! Every JavaScript-semantics trap this port had to dodge, pinned.
//!
//! These are not "does the function work" tests. Each one fails if the port is
//! later "simplified" back to the obvious Rust spelling: `from_str_radix` for
//! `parseInt`, `f64::max` for `Math.max`, `strip_suffix` for `basename`,
//! `str::cmp` for the default `Array#sort`, a case-insensitive regex where the
//! TypeScript has no `i` flag, and so on.

use integrate_matches::cleanup::{civil_date, cleanup_installed_scratch};
use integrate_matches::extent::{linked_function_extent, mismatch};
use integrate_matches::jsint::{hexadecimal, is_safe_integer, math_max_all, parse_int_hex};
use integrate_matches::jsregex::{
    carries_helper, has_state_line, insert_state_after_heading, is_candidate_name,
    is_function_symbol, is_line_terminator, is_source_address, is_text_type, replace_state_line,
};

// -------------------------------------------------------------- parseInt(s, 16)

#[test]
fn parse_int_hex_accepts_the_prefix_from_str_radix_rejects() {
    // `Number.parseInt("0x40", 16)` is 64. `i64::from_str_radix("0x40", 16)`
    // is an error. That exact substitution has already cost this project 980
    // bytes at one call site after costing 526 at another, which is why every
    // numeric parse in this crate goes through this one function.
    assert_eq!(parse_int_hex("0x40"), 64.0);
    assert_eq!(parse_int_hex("0X40"), 64.0);
    assert!(i64::from_str_radix("0x40", 16).is_err());
}

#[test]
fn parse_int_hex_stops_at_the_first_bad_character() {
    // `parseInt` consumes the longest valid prefix and DISCARDS the rest; it
    // does not fail. A `.parse()` port would reject all four of these.
    assert_eq!(parse_int_hex("08021360 <Func>"), 134_353_760.0);
    assert_eq!(parse_int_hex("40zz"), 64.0);
    assert_eq!(parse_int_hex("1.5"), 1.0);
    assert_eq!(parse_int_hex("  \t0x1f"), 31.0);
    assert_eq!(parse_int_hex("-10"), -16.0);
}

#[test]
fn parse_int_hex_is_nan_only_when_no_digit_starts_the_string() {
    assert!(parse_int_hex("").is_nan());
    assert!(parse_int_hex("zz").is_nan());
    assert!(parse_int_hex("0x").is_nan());
    // Case-INSENSITIVE digits, unlike the `[0-9a-f]` character classes below,
    // which really are case-sensitive in the source.
    assert_eq!(parse_int_hex("ABCDEF"), 11_259_375.0);
}

#[test]
fn is_safe_integer_matches_number_is_safe_integer() {
    assert!(is_safe_integer(0.0));
    assert!(is_safe_integer(9_007_199_254_740_991.0));
    assert!(!is_safe_integer(9_007_199_254_740_992.0));
    assert!(!is_safe_integer(1.5));
    assert!(!is_safe_integer(f64::NAN));
    assert!(!is_safe_integer(f64::INFINITY));
}

// ------------------------------------------------------------ Math.max and NaN

#[test]
fn math_max_propagates_nan_where_f64_max_swallows_it() {
    // THE trap. `Math.max(1, NaN)` is NaN; `1.0f64.max(f64::NAN)` is 1.0.
    // `Math.max` is what decides the compiled extent, so a `f64::max` port
    // would silently invent a plausible length out of an unparsed symbol row.
    assert!(math_max_all(&[1.0, f64::NAN, 3.0]).is_nan());
    assert_eq!(1.0f64.max(f64::NAN), 1.0);
    assert_eq!(math_max_all(&[1.0, 3.0, 2.0]), 3.0);
    // `Math.max()` with no arguments is -Infinity, not an error and not 0.
    assert_eq!(math_max_all(&[]), f64::NEG_INFINITY);
}

#[test]
fn infinity_minus_infinity_is_nan_not_zero() {
    // `Math.max(...[]) - address` when `address` is also -Infinity. The
    // TypeScript reaches this only through a corrupt `nm` dump, but the value
    // it produces is NaN, and NaN is falsy, which is how a missing field turns
    // into a silently disabled check elsewhere in this codebase.
    assert!((f64::NEG_INFINITY - f64::NEG_INFINITY).is_nan());
}

// --------------------------------------------------------- the missing i flags

#[test]
fn the_address_guard_is_case_sensitive() {
    // `/^08[0-9a-f]{6}$/` -- NO `i`. Uppercase hex is rejected by the
    // TypeScript, so it must be rejected here. A `(?i)` port would accept
    // addresses the real tool refuses, and this repository has confirmed the
    // missing-`i` defect class FOUR separate times.
    assert!(is_source_address("08021360"));
    assert!(is_source_address("080abcde"));
    assert!(!is_source_address("080ABCDE"));
    assert!(!is_source_address("0802136"));
    assert!(!is_source_address("080213600"));
    assert!(!is_source_address("09021360"));
}

#[test]
fn the_function_symbol_guard_is_case_sensitive() {
    // `/^Func_[0-9a-f]{8}$/` -- NO `i`. This is the exact pattern the missing
    // flag was most recently confirmed on.
    assert!(is_function_symbol("Func_08021360"));
    assert!(!is_function_symbol("Func_08021ABC"));
    assert!(!is_function_symbol("func_08021360"));
    assert!(!is_function_symbol("FUNC_08021360"));
    assert!(!is_function_symbol("Func_0802136"));
}

#[test]
fn the_symbol_type_column_accepts_exactly_t_and_lowercase_t() {
    // `/^[Tt]$/` -- the case pair is written out, so this one IS both cases,
    // and only these two.
    assert!(is_text_type("T"));
    assert!(is_text_type("t"));
    assert!(!is_text_type("A"));
    assert!(!is_text_type("D"));
    assert!(!is_text_type("TT"));
}

#[test]
fn the_candidate_filter_dot_excludes_line_terminators() {
    // `/^src_.*\.c$/` -- `.` in ECMAScript matches anything EXCEPT the four
    // line terminators, and without `m` the anchors are string ends. Rust's
    // `.` in a naive port, or a `starts_with`/`ends_with` pair, would accept a
    // filename containing a newline.
    assert!(is_candidate_name("src_08021360.c"));
    assert!(is_candidate_name("src_.c"));
    assert!(!is_candidate_name("src_08021360.s"));
    assert!(!is_candidate_name("gate"));
    assert!(!is_candidate_name("src_a\nb.c"));
    assert!(!is_candidate_name("src_a\u{2028}b.c"));
    assert!(is_candidate_name("src_a\u{0085}b.c"), "U+0085 is NOT a JS line terminator");
}

#[test]
fn js_line_terminators_are_exactly_four_and_u0085_is_not_one() {
    for c in ['\n', '\r', '\u{2028}', '\u{2029}'] {
        assert!(is_line_terminator(c), "{c:?} is a JS line terminator");
    }
    // Rust's `char::is_control` and Unicode `White_Space` both include U+0085;
    // ECMAScript's LineTerminator production does not.
    assert!(!is_line_terminator('\u{0085}'));
    assert!(!is_line_terminator('\u{000b}'));
}

// ---------------------------------------------------------- multiline anchors

#[test]
fn the_helper_guard_anchors_only_static_and_inline() {
    // `/inline_fn|^(static|inline)\b/m`. The alternation is ASYMMETRIC:
    // `inline_fn` matches anywhere, the two keywords only at a line start.
    assert!(carries_helper("int x;\nstatic int helper(void) { return 1; }\n"));
    assert!(carries_helper("void f(void) { inline_fn(); }\n"));
    assert!(carries_helper("inline void f(void);\n"));
    // Not at a line start, so the keyword alternative does not fire.
    assert!(!carries_helper("void f(void) { int static_thing = 1; }\n"));
    assert!(!carries_helper("  static int x;\n"));
    // `\b` in ECMAScript is ASCII-only, so `static_` has no boundary after
    // `static` and the keyword alternative does not fire on it either.
    assert!(!carries_helper("static_cast\n"));
    assert!(carries_helper("static int x;\n"));
}

#[test]
fn multiline_caret_fires_after_a_lone_carriage_return() {
    // ECMAScript's `m` treats every LineTerminator as a line start, including
    // a bare `\r`. Rust's `regex` crate with `(?m)` only recognises `\n`, so a
    // classic-Mac-line-ending source would slip past a naive port.
    assert!(carries_helper("int x;\rstatic int helper(void);\r"));
    assert!(carries_helper("int x;\u{2028}static int helper(void);"));
}

// --------------------------------------------------------- the dossier rewrite

#[test]
fn the_state_line_rewrite_replaces_only_the_first_match() {
    // `String#replace` with a non-global regex replaces ONE occurrence. A
    // `replace_all` port would rewrite a historical State line further down a
    // dossier that the real tool leaves alone.
    let source = "# 08021360\n\nState: OPEN\n\nnotes\n\nState: OPEN\n";
    let rewritten = replace_state_line(source, "State: CLOSED");
    assert_eq!(rewritten, "# 08021360\n\nState: CLOSED\n\nnotes\n\nState: OPEN\n");
}

#[test]
fn the_state_line_rewrite_survives_crlf() {
    // `/^State:.*$/m` -- `.` excludes `\r`, and `$` in multiline mode matches
    // BEFORE the `\r`, so the carriage return must survive the replacement.
    let source = "# 08021360\r\nState: OPEN\r\nnotes\r\n";
    assert!(has_state_line(source));
    let rewritten = replace_state_line(source, "State: CLOSED");
    assert_eq!(rewritten, "# 08021360\r\nState: CLOSED\r\nnotes\r\n");
}

#[test]
fn the_heading_insert_has_no_multiline_flag() {
    // `/^(#[^\n]*\n)/` -- NO `m`, so it matches only at the START of the
    // string. A dossier whose first line is not a heading gets nothing
    // inserted, even if a heading appears later.
    let with = insert_state_after_heading("# 08021360\nbody\n", "State: CLOSED");
    assert_eq!(with, "# 08021360\n\nState: CLOSED\nbody\n");
    let without = insert_state_after_heading("preamble\n# 08021360\nbody\n", "State: CLOSED");
    assert_eq!(without, "preamble\n# 08021360\nbody\n");
}

// ------------------------------------------------------------ byte comparison

#[test]
fn identical_odd_length_buffers_report_no_difference() {
    // `candidate_show.ts` carries a live off-by-one here: it masks the shared
    // length with `& ~1`, which for an odd length falls BELOW the real bound
    // and reports a PHANTOM difference on identical buffers. This function
    // does not have that shape, and this test exists so nobody adds it.
    assert!(mismatch(&[1, 2, 3], &[1, 2, 3]).is_none());
    assert!(mismatch(&[1], &[1]).is_none());
    assert!(mismatch(&[], &[]).is_none());
    assert!(mismatch(&[1, 2, 3, 4, 5, 6, 7], &[1, 2, 3, 4, 5, 6, 7]).is_none());
}

#[test]
fn a_length_difference_reports_undefined_on_the_short_side() {
    // `left[shared]` past the end is `undefined` in JavaScript, and the
    // TypeScript reports it as such. Rust's `left[shared]` PANICS, so the port
    // must model the absence rather than index.
    let short = mismatch(&[1], &[1, 2]).expect("lengths differ");
    assert_eq!(short.offset, 1);
    assert_eq!(short.left, None);
    assert_eq!(short.right, Some(2));
    let long = mismatch(&[1, 2], &[1]).expect("lengths differ");
    assert_eq!(long.offset, 1);
    assert_eq!(long.left, Some(2));
    assert_eq!(long.right, None);
}

#[test]
fn a_content_difference_wins_over_a_length_difference() {
    let found = mismatch(&[1, 9], &[1, 2, 3]).expect("bytes differ");
    assert_eq!(found.offset, 1);
    assert_eq!(found.left, Some(9));
    assert_eq!(found.right, Some(2));
}

// ------------------------------------------------------------- the nm parsing

#[test]
fn an_absolute_symbol_row_cannot_widen_the_extent() {
    // The `A` row has only THREE fields, so `fields.length >= 4` drops it
    // before its type column is ever read. This is the tool's own self-test
    // fixture, and it pins the filter ORDER, not just the outcome.
    let symbols = "08021360 00000030 T Func_08021360\n080770c1 A Func_080770c0";
    assert_eq!(linked_function_extent(symbols, "Func_08021360", 0x0802_1360 as f64, 48), Ok(48));
}

#[test]
fn extent_parsing_tolerates_crlf_and_blank_rows() {
    let symbols = "\r\n08021360 00000030 T Func_08021360\r\n\r\n";
    assert_eq!(linked_function_extent(symbols, "Func_08021360", 0x0802_1360 as f64, 48), Ok(48));
}

#[test]
fn extent_parsing_rejects_uppercase_symbol_names() {
    // Same missing `i` as above, reached through the real entry point.
    let symbols = "08021360 00000030 T Func_08021ABC";
    assert!(linked_function_extent(symbols, "Func_08021ABC", 0x0802_1360 as f64, 48).is_err());
}

// ------------------------------------------------------------------ formatting

#[test]
fn hexadecimal_pads_but_never_truncates() {
    // `padStart(8, "0")` pads a short string and leaves a long one ALONE. A
    // `{:08x}` port agrees; a fixed-width slice would not.
    assert_eq!(hexadecimal(0x0802_1360 as f64), "08021360");
    assert_eq!(hexadecimal(0.0), "00000000");
    assert_eq!(hexadecimal(0x1_0000_0000u64 as f64), "100000000");
}

// -------------------------------------------------------------- sort ordering

#[test]
fn removed_entries_sort_in_utf16_code_unit_order() {
    // `Array#sort()` with no comparator is UTF-16 code-unit order, which is
    // neither `str::cmp` (code POINT order) nor any locale collation. The
    // orders disagree for astral characters, where UTF-16 surrogates sort
    // BELOW U+E000..U+FFFF.
    let mut names = vec!["\u{10000}".to_string(), "\u{ffff}".to_string()];
    names.sort_by(|left, right| match_m2c::jsstring::utf16_cmp(left, right));
    assert_eq!(names, vec!["\u{10000}".to_string(), "\u{ffff}".to_string()]);
    let mut plain = vec!["\u{10000}".to_string(), "\u{ffff}".to_string()];
    plain.sort();
    assert_eq!(plain, vec!["\u{ffff}".to_string(), "\u{10000}".to_string()]);
}

// ------------------------------------------------------------------- the date

#[test]
fn civil_date_matches_to_iso_string_slice() {
    // `new Date().toISOString().slice(0, 10)` in UTC. Checked against known
    // days since the epoch, including a leap day and a century boundary.
    assert_eq!(civil_date(0), "1970-01-01");
    assert_eq!(civil_date(11_017), "2000-03-01");
    assert_eq!(civil_date(11_016), "2000-02-29");
    assert_eq!(civil_date(20_656), "2026-07-22");
    assert_eq!(civil_date(-1), "1969-12-31");
}

// ------------------------------------------------------------- the cleanup set

#[test]
fn cleanup_removes_the_prefixed_scratch_and_leaves_everything_else() {
    let base = std::env::temp_dir().join(format!("integrate-matches-trap-{}", std::process::id()));
    let _ = std::fs::remove_dir_all(&base);
    std::fs::create_dir_all(base.join("walls")).expect("walls");
    std::fs::create_dir_all(base.join("hand").join("08021360")).expect("hand");
    std::fs::write(base.join("08021360.c"), "candidate\n").expect("candidate");
    std::fs::write(base.join("08021360.txt"), "notes\n").expect("notes");
    // A DIFFERENT address that merely starts with the same characters. The
    // guard is a `<stem>.` prefix, not a bare `startsWith(stem)`, so this file
    // must survive.
    std::fs::write(base.join("080213600.c"), "other\n").expect("other");
    std::fs::write(base.join("unrelated.c"), "keep\n").expect("unrelated");
    std::fs::write(base.join("walls").join("08021360.md"), "# 08021360\n\nState: OPEN\n")
        .expect("dossier");

    let cleaned = cleanup_installed_scratch("08021360", &base, "2026-07-22").expect("cleanup");
    assert_eq!(cleaned.removed.join(","), "08021360.c,08021360.txt,hand/08021360/");
    assert!(cleaned.dossier_closed);
    assert!(base.join("080213600.c").exists(), "a longer address must not be swept up");
    assert!(base.join("unrelated.c").exists());
    let dossier = std::fs::read_to_string(base.join("walls").join("08021360.md")).expect("read");
    assert!(dossier.contains("State: CLOSED — 2026-07-22"));
    let _ = std::fs::remove_dir_all(&base);
}

#[test]
fn cleanup_refuses_an_address_that_fails_the_guard() {
    let base = std::env::temp_dir().join(format!("integrate-matches-guard-{}", std::process::id()));
    let _ = std::fs::remove_dir_all(&base);
    std::fs::create_dir_all(&base).expect("base");
    // Uppercase hex, again: the guard has no `i` flag, so this must be
    // refused rather than deleting anything.
    assert!(cleanup_installed_scratch("080ABCDE", &base, "2026-07-22").is_err());
    assert!(cleanup_installed_scratch("../etc", &base, "2026-07-22").is_err());
    let _ = std::fs::remove_dir_all(&base);
}

// ---------------------------------------------------------- the tool self-test

#[test]
fn the_ported_self_test_passes() {
    assert_eq!(integrate_matches::selftest::self_test().as_deref(), Ok("self-test=ok"));
}
