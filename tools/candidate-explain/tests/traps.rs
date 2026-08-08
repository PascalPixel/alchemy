//! One test per trap this port had to dodge. Each of these FAILS if a later
//! "simplification" rewrites the guarded code back into its naive Rust form.

use candidate_explain::cli::{parse_arguments, Options, ParseOutcome};
use candidate_explain::explain::{differing_halfwords, format_pair, Decision};
use candidate_explain::jsnum::{
    pad_end, pad_start, parse_int_16, slice_utf16, to_string_16, trim_end_js, utf16_len,
};
use candidate_explain::matchers::{objdump_row, overlay_id, ready_list, scheduling_insn};
use candidate_explain::ordered::{OrderedMap, OrderedSet};
use candidate_explain::{decision_for_insn, self_test};

// ---------------------------------------------------------------------------
// parseInt vs i64::from_str_radix
// ---------------------------------------------------------------------------

/// `parseInt(text, 16)` accepts a `0x` prefix; `i64::from_str_radix(text, 16)`
/// rejects it outright. Both call sites (`--adjust-vma` offsets and the overlay
/// id's hex tail) route through this one helper for exactly that reason.
#[test]
fn parse_int_16_accepts_the_0x_prefix() {
    assert_eq!(parse_int_16("0x1f4"), 500.0);
    assert_eq!(parse_int_16("0X1F4"), 500.0);
    assert_eq!(parse_int_16("1f4"), 500.0);
    assert!(i64::from_str_radix("0x1f4", 16).is_err(), "the naive form");
}

/// `parseInt` is a PREFIX parse: it consumes the leading valid digits and
/// ignores the rest, where `from_str_radix` errors on the whole string.
#[test]
fn parse_int_16_stops_at_the_first_invalid_digit() {
    assert_eq!(parse_int_16("8x"), 8.0);
    assert_eq!(parse_int_16("1.5"), 1.0);
    assert_eq!(parse_int_16("ffzz"), 255.0);
    assert_eq!(parse_int_16("-10"), -16.0);
}

/// No digits at all is `NaN`, not `0` and not an error.
#[test]
fn parse_int_16_yields_nan_for_no_digits() {
    assert!(parse_int_16("").is_nan());
    assert!(parse_int_16("0x").is_nan());
    assert!(parse_int_16("zz").is_nan());
}

/// `parseInt` skips leading `StrWhiteSpace`, which includes U+00A0 and U+FEFF
/// and does NOT include U+0085.
#[test]
fn parse_int_16_skips_js_whitespace_only() {
    assert_eq!(parse_int_16("\u{00a0}1f"), 31.0);
    assert_eq!(parse_int_16("\u{feff}1f"), 31.0);
    assert_eq!(parse_int_16("\u{3000}1f"), 31.0);
    assert!(
        parse_int_16("\u{0085}1f").is_nan(),
        "U+0085 is Rust White_Space but NOT JS StrWhiteSpace"
    );
}

// ---------------------------------------------------------------------------
// NaN poisoning
// ---------------------------------------------------------------------------

/// `--span` with no following argument is `Number(undefined)` == NaN, which is
/// NOT `undefined`, so the `span === undefined` guard downstream does not fire.
/// `Some(NaN)` must therefore stay a distinct state from `None`.
#[test]
fn dangling_span_flag_produces_some_nan_not_none() {
    let argv: Vec<String> = ["resource_0001:0x40", "--source", "/tmp/x.c", "--span"]
        .iter()
        .map(|s| (*s).to_string())
        .collect();
    let ParseOutcome::Parsed(options) = parse_arguments(&argv, std::path::Path::new("/tmp")) else {
        panic!("expected a parsed overlay invocation");
    };
    let Options::Overlay { span, .. } = *options else {
        panic!("expected overlay mode");
    };
    let span = span.expect("NaN is Some(NaN), never None");
    assert!(span.is_nan());
}

/// `--span abc` is likewise NaN, and `--span ""` is `Number("")` == 0. The two
/// must not collapse together.
#[test]
fn unparsable_span_is_nan_but_empty_span_is_zero() {
    let case = |value: &str| -> f64 {
        let argv: Vec<String> = ["resource_0001:0x40", "--source", "/tmp/x.c", "--span", value]
            .iter()
            .map(|s| (*s).to_string())
            .collect();
        let ParseOutcome::Parsed(options) = parse_arguments(&argv, std::path::Path::new("/tmp"))
        else {
            panic!("expected a parsed overlay invocation");
        };
        let Options::Overlay { span, .. } = *options else {
            panic!("expected overlay mode");
        };
        span.expect("always Some")
    };
    assert!(case("abc").is_nan());
    assert_eq!(case(""), 0.0);
    assert_eq!(case("64"), 64.0);
}

/// `pair.winner === pickedUid` is an ordinary `===`, false whenever either side
/// is NaN. `f64 == f64` agrees. `total_cmp` would call NaN equal to NaN and
/// `partial_cmp` would need an arbitrary `None` fallback; both diverge here.
#[test]
fn nan_winner_never_matches_the_picked_uid() {
    use rtl_schedule::{PairComparison, Tier};
    let pair = PairComparison {
        winner: f64::NAN,
        loser: 19.0,
        tier: Tier::Priority,
        detail: "d".to_string(),
    };
    let text = format_pair(f64::NAN, &pair);
    assert!(
        text.starts_with("?? model expects"),
        "NaN === NaN is false, so the divergence branch must be taken: {text}"
    );
    assert_eq!(f64::NAN.total_cmp(&f64::NAN), std::cmp::Ordering::Equal);
}

/// A NaN uid must find nothing. The lookup is a substring test built from
/// `String(uid)`, so it searches for the literal `<<<NaN>>>` rather than
/// matching the regex's `\d+` against the first decision in the dump.
#[test]
fn nan_uid_finds_no_decision() {
    let dump = "--> scheduling insn <<<15>>>\n--> scheduling insn <<<19>>>\n";
    assert!(decision_for_insn(dump, f64::NAN).is_none());
    assert!(decision_for_insn(dump, 15.0).is_some());
}

// ---------------------------------------------------------------------------
// Clamp versus panic
// ---------------------------------------------------------------------------

/// The tail loop of `differingHalfwords` walks past the end of the shorter
/// buffer, where JavaScript reads `undefined` and Rust indexing would panic.
#[test]
fn differing_halfwords_walks_past_the_shorter_buffer_without_panicking() {
    let found = differing_halfwords(&[1, 2, 3, 4, 5, 6], &[1, 2]);
    let offsets: Vec<i64> = found.iter().copied().collect();
    assert_eq!(offsets, vec![2, 4]);
    // The empty/empty case must not underflow `shared & !1`.
    assert_eq!(differing_halfwords(&[], &[]).size(), 0);
    // An ODD shared length: `shared & !1` rounds DOWN, so offset 2 is emitted
    // by the tail even though byte 2 was never compared.
    let found = differing_halfwords(&[1, 2, 3], &[1, 2, 3, 4, 5]);
    let offsets: Vec<i64> = found.iter().copied().collect();
    assert_eq!(offsets, vec![2, 4]);
}

/// `padEnd(30)` runs BEFORE `slice(0, 30)`, and `slice` clamps rather than
/// panicking on a short string.
#[test]
fn pad_then_slice_clamps_in_that_order() {
    assert_eq!(slice_utf16(&pad_end("abc", 30), 30).len(), 30);
    assert_eq!(
        slice_utf16(&pad_end(&"x".repeat(40), 30), 30),
        "x".repeat(30)
    );
    assert_eq!(slice_utf16("ab", 30), "ab");
}

// ---------------------------------------------------------------------------
// UTF-16 units
// ---------------------------------------------------------------------------

/// `String.length`, `padEnd` and `slice` all count UTF-16 code units, not
/// bytes and not `char`s. An astral character is TWO units.
#[test]
fn padding_and_slicing_count_utf16_units() {
    assert_eq!(utf16_len("\u{1f600}"), 2);
    assert_eq!("\u{1f600}".chars().count(), 1, "the naive char count");
    assert_eq!("\u{1f600}".len(), 4, "the naive byte count");
    // One astral char plus 28 pad units reaches 30 units.
    assert_eq!(utf16_len(&pad_end("\u{1f600}", 30)), 30);
    // Slicing at a boundary that splits a surrogate pair yields a lone
    // surrogate in JS; this port emits U+FFFD rather than invalid UTF-8.
    assert_eq!(slice_utf16("\u{1f600}", 1), "\u{fffd}");
}

/// `padStart(4, "0")` on a string already at or beyond the width is a no-op.
#[test]
fn pad_start_never_truncates() {
    assert_eq!(pad_start("1f", 4, '0'), "001f");
    assert_eq!(pad_start("1f4a2", 4, '0'), "1f4a2");
    assert_eq!(pad_start("15", 4, ' '), "  15");
}

// ---------------------------------------------------------------------------
// Regex flags, character classes and line terminators
// ---------------------------------------------------------------------------

/// The objdump row pattern carries NO `i` flag, so uppercase hex must not
/// match. clippy has previously suggested a case-insensitive comparison here,
/// which would silently create the missing-`i`-flag defect.
#[test]
fn objdump_row_is_case_sensitive() {
    assert_eq!(
        objdump_row("      1f4:\t4770      \tbx\tlr"),
        Some(("1f4", "bx\tlr"))
    );
    assert_eq!(objdump_row("      1F4:\t4770      \tbx\tlr"), None);
}

/// The overlay id pattern is the ONLY one with the `i` flag, and its captures
/// must come from the ORIGINAL subject, not from a lowercased copy.
#[test]
fn overlay_id_is_case_insensitive_but_preserves_the_subject() {
    assert_eq!(
        overlay_id("resource_00AB:0xFF"),
        Some(("resource_00AB", "FF"))
    );
    assert_eq!(overlay_id("RESOURCE_00ab:ff"), Some(("RESOURCE_00ab", "ff")));
    assert_eq!(overlay_id("work/matches/m2c/08002dd8.c"), None);
}

/// JavaScript's `.` excludes the four line terminators and `$` without the `m`
/// flag anchors at the very end, so a CRLF objdump row does NOT match. Using
/// `lines()` (which strips `\r`) or the `s` flag would silently repair it.
#[test]
fn crlf_objdump_rows_do_not_match() {
    assert_eq!(objdump_row("      1f4:\t4770      \tbx\tlr\r"), None);
    assert_eq!(objdump_row("      1f4:\t4770      \tbx\tlr\u{2028}"), None);
    assert_eq!(objdump_row("      1f4:\t4770      \tbx\tlr\u{0085}"), Some(("1f4", "bx\tlr\u{0085}")));
}

/// `Ready list (t = N):` is matched with an UNANCHORED search, so the leading
/// indentation gcc emits must not defeat it, and `\d` is ASCII-only.
#[test]
fn ready_list_matches_unanchored_and_ascii_only() {
    assert_eq!(ready_list("  ;;   Ready list (t =  3):  15  19"), Some("15  19"));
    assert_eq!(ready_list("Ready list (t = 0):"), Some(""));
    assert_eq!(
        ready_list("Ready list (t = \u{0663}):"),
        None,
        "JS \\d is ASCII-only; Arabic-Indic digits must not match"
    );
}

/// `--> scheduling insn <<<N>>>` likewise.
#[test]
fn scheduling_insn_extracts_ascii_digits() {
    assert_eq!(
        scheduling_insn(";;\t--> scheduling insn <<<19>>>"),
        Some("19")
    );
    assert_eq!(scheduling_insn("--> scheduling insn <<<>>>"), None);
}

// ---------------------------------------------------------------------------
// Set / Map ordering
// ---------------------------------------------------------------------------

/// JS `Set` iteration is INSERTION order and load-bearing for the printed
/// diff. A `HashSet` would randomise it and a `BTreeSet` would sort it.
#[test]
fn ordered_set_keeps_insertion_order_and_dedupes() {
    let mut set = OrderedSet::new();
    for value in [8, 2, 8, 4, 2] {
        set.add(value);
    }
    let values: Vec<i64> = set.iter().copied().collect();
    assert_eq!(values, vec![8, 2, 4], "insertion order, silently deduped");
    assert_eq!(set.size(), 3);
    assert!(set.has(4) && !set.has(6));
}

/// `Map#set` on an existing key keeps the key's ORIGINAL position and replaces
/// the value.
#[test]
fn ordered_map_set_keeps_the_original_position() {
    let mut map: OrderedMap<usize> = OrderedMap::new();
    map.set(8, 1);
    map.set(2, 2);
    map.set(8, 3);
    assert_eq!(map.get(8).copied(), Some(3));
    assert_eq!(map.len(), 2);
}

/// The ready-uid list is deliberately a `Vec`, so duplicate uids in one ready
/// list produce duplicate comparison rows rather than being deduped away.
#[test]
fn ready_uids_are_not_deduped() {
    let dump = concat!(
        ";; Ready list (t = 0):  15  19  15\n",
        ";;\t--> scheduling insn <<<21>>>\n",
    );
    let Decision { ready_uids, .. } = decision_for_insn(dump, 21.0).expect("a decision");
    assert_eq!(ready_uids, vec![15.0, 19.0, 15.0]);
}

// ---------------------------------------------------------------------------
// Float formatting
// ---------------------------------------------------------------------------

/// Every number this tool prints is emitted through `to_string_16` (hex) or
/// `js_number_text` (ECMAScript ToString), never through Rust's `{}`. This
/// test walks every numeric shape that can reach the output and proves the
/// two agree with JavaScript, so ryu / `{}` never leaks in.
#[test]
fn every_emitted_number_matches_javascript() {
    use rtl_align::js_number_text;

    // Hex offsets: `offset.toString(16)`. Negative offsets are possible when
    // `--adjust-vma` and the base disagree, and JS renders them with a `-`
    // sign, NOT as two's complement.
    for (value, expected) in [
        (0.0, "0"),
        (4.0, "4"),
        (255.0, "ff"),
        (-16.0, "-10"),
        (134229464.0, "8002dd8"),
    ] {
        assert_eq!(to_string_16(value), expected, "toString(16) of {value}");
    }

    // uids and lengths: `String(n)`. Integral values must never gain a `.0`
    // and must never fall into exponential form below 1e21.
    for (value, expected) in [
        (0.0, "0"),
        (15.0, "15"),
        (1e20, "100000000000000000000"),
        (-0.0, "0"),
        (f64::NAN, "NaN"),
        (f64::INFINITY, "Infinity"),
    ] {
        assert_eq!(js_number_text(value), expected, "String({value})");
    }
    assert_eq!(format!("{}", 15.0_f64), "15", "Rust agrees here");
    assert_ne!(format!("{:?}", 15.0_f64), "15", "but Debug does not");

    // Buffer lengths reach the header as `usize`, which is integral by
    // construction and therefore cannot produce a fractional rendering.
    assert_eq!(format!("{}", 40_usize), "40");

    // KNOWN, AND PROVEN UNREACHABLE: `rtl_align::js_number_text` does not
    // implement ECMAScript's 1e21 exponential threshold, so it renders 1e21 in
    // full rather than as "1e+21". Nothing this tool prints can reach that
    // magnitude: every number on the output path is either a `usize` buffer
    // length, a byte offset bounded by the candidate size, or an RTL uid, and
    // gcc-2.96 allocates uids sequentially from a single `int` counter. The
    // divergence is pinned here so that widening any of those to an arbitrary
    // `f64` makes this assertion the thing that fails first.
    assert_eq!(js_number_text(1e21), "1000000000000000000000");
    assert!(
        [0.0_f64, 40.0, 65535.0, 1e6].iter().all(|value| {
            js_number_text(*value) == format!("{}", *value as i64)
        }),
        "the reachable range is integral and below 1e21"
    );
}

// ---------------------------------------------------------------------------
// Miscellaneous
// ---------------------------------------------------------------------------

/// `trimEnd()` strips JS whitespace AND line terminators, so U+00A0 goes but
/// U+0085 stays.
#[test]
fn trim_end_uses_js_whitespace() {
    assert_eq!(trim_end_js("bx lr\u{00a0}\t \n"), "bx lr");
    assert_eq!(trim_end_js("bx lr\u{0085}"), "bx lr\u{0085}");
}

/// The `--self-test` path is the tool's own contract line.
#[test]
fn self_test_reports_ok() {
    assert_eq!(
        self_test().expect("the fixture must align"),
        "self-test=ok tool=candidate-explain"
    );
}

/// `-h` short-circuits before any other argument is examined, and an
/// unrecognised second positional is an error rather than a silent overwrite.
#[test]
fn argument_parsing_edges() {
    let argv = |items: &[&str]| -> Vec<String> { items.iter().map(|s| (*s).to_string()).collect() };
    let cwd = std::path::Path::new("/tmp");
    assert!(matches!(
        parse_arguments(&argv(&["a.c", "-h"]), cwd),
        ParseOutcome::Help
    ));
    assert!(matches!(
        parse_arguments(&argv(&["a.c", "b.c"]), cwd),
        ParseOutcome::Error(_)
    ));
    assert!(matches!(
        parse_arguments(&argv(&[]), cwd),
        ParseOutcome::Error(_)
    ));
    // A dangling `--work` consumes the missing value as `""`, which then falls
    // back exactly as an absent flag does.
    let ParseOutcome::Parsed(options) = parse_arguments(&argv(&["a.c", "--work"]), cwd) else {
        panic!("expected main mode");
    };
    let Options::Main { work, .. } = *options else {
        panic!("expected main mode");
    };
    assert!(work.ends_with("work/candidate-explain-main"));
}
