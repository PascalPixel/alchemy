//! Each test here pins one JavaScript semantic that the naive Rust spelling
//! gets wrong. A later "simplification" to the obvious form must FAIL these.

use compiler_corpus_regression::cli::{parse_arguments, ParseOutcome};
use compiler_corpus_regression::config::{compiler_configuration_of, flags_of};
use compiler_corpus_regression::corpus::{deterministic_sample, hash, hexadecimal_stem, Member};
use compiler_corpus_regression::diff::byte_difference;
use compiler_corpus_regression::extent::linked_function_extent;
use compiler_corpus_regression::jsparse::{
    default_sort_cmp, is_js_integer, is_js_safe_integer, js_abs, js_greater_than, parse_int,
    utf16_slice_to,
};
use compiler_corpus_regression::jsvalue::{canonical_json, object, parse, string, strings, Json};
use compiler_corpus_regression::pipeline::{compiler_config_json, relative_source, rom_slice};
use compiler_corpus_regression::result::{cached_result, Outcome};

// ---------------------------------------------------------------- parseInt

#[test]
fn parse_int_accepts_the_prefix_and_trailing_junk_that_from_str_radix_rejects() {
    // The 1,506-byte defect, both halves of it.
    assert_eq!(parse_int("0x1f", 16), Some(31.0));
    assert_eq!(parse_int("1fzz", 16), Some(31.0));
    assert_eq!(parse_int("8x", 10), Some(8.0));
    assert_eq!(parse_int("1.5", 10), Some(1.0));
    // `i64::from_str_radix` would reject every one of the above.
    assert!(i64::from_str_radix("0x1f", 16).is_err());
    assert!(i64::from_str_radix("1fzz", 16).is_err());
}

#[test]
fn parse_int_radix_zero_is_auto_detect_and_radix_ten_is_not() {
    assert_eq!(parse_int("0x10000", 0), Some(65536.0));
    assert_eq!(parse_int("10000", 0), Some(10000.0));
    // Radix 10 stops at the `x`, so `0x10` is ZERO, not sixteen.
    assert_eq!(parse_int("0x10", 10), Some(0.0));
}

#[test]
fn parse_int_returns_none_for_nan_and_handles_signs_and_js_whitespace() {
    assert_eq!(parse_int("", 10), None);
    assert_eq!(parse_int("zz", 16), None);
    assert_eq!(parse_int("-20", 16), Some(-32.0));
    assert_eq!(parse_int("+7", 10), Some(7.0));
    // JavaScript's `\s` includes U+00A0 and U+FEFF and EXCLUDES U+0085.
    assert_eq!(parse_int("\u{00a0}12", 10), Some(12.0));
    assert_eq!(parse_int("\u{feff}12", 10), Some(12.0));
    assert_eq!(parse_int("\u{0085}12", 10), None);
}

#[test]
fn number_is_integer_is_false_for_nan_where_a_naive_port_would_say_true() {
    assert!(!is_js_integer(None));
    assert!(is_js_integer(Some(0.0)));
    assert!(!is_js_integer(Some(1.5)));
    assert!(!is_js_safe_integer(Some(f64::NAN)));
    assert!(!is_js_safe_integer(Some(9007199254740993.0)));
}

// ------------------------------------------------------------------ NaN

#[test]
fn math_abs_and_greater_than_model_nan_the_way_javascript_does() {
    assert!(js_abs(f64::NAN).is_nan());
    // `NaN > 0` is FALSE and `NaN <= 0` is ALSO false. clippy would like to
    // rewrite `!(x > y)` as `x <= y`; it is wrong, and that is why this is a
    // named helper.
    assert!(!js_greater_than(f64::NAN, 0.0));
    assert!(!js_greater_than(0.0, f64::NAN));
    // Both directions of the comparison are false for NaN. Routed through a
    // named helper because clippy rejects a literal `NAN <= 0.0`, and the
    // point of the assertion is exactly that neither side holds.
    fn less_or_equal(left: f64, right: f64) -> bool {
        left <= right
    }
    assert!(!less_or_equal(f64::NAN, 0.0));
    assert!(!less_or_equal(0.0, f64::NAN));
    // `f64::max` does NOT propagate NaN; `Math.max` does. Pinned so nobody
    // reaches for `max` in the extent fold.
    assert_eq!(f64::NAN.max(1.0), 1.0);
}

// ------------------------------------------------------------ byteDifference

#[test]
fn byte_difference_reports_zero_for_identical_odd_length_buffers() {
    // The `candidate_show.ts` phantom: `shared & ~1` would drop the last byte
    // of a 3-byte buffer below the extent and report a difference at offset 2.
    let buffer = [1u8, 2, 3];
    assert_eq!(byte_difference(&buffer, &buffer).count, 0);
    assert_eq!(byte_difference(&buffer, &buffer).first, None);
    let five = [9u8, 8, 7, 6, 5];
    assert_eq!(byte_difference(&five, &five).count, 0);
}

#[test]
fn byte_difference_counts_the_tail_past_the_shorter_buffer() {
    let difference = byte_difference(&[1, 2, 3], &[1, 4]);
    assert_eq!(difference.count, 2);
    assert_eq!(difference.first, Some(1));
    // Reading past a typed array yields `undefined`, and
    // `undefined === undefined` is TRUE, so without the two length guards the
    // tail would be counted EQUAL. It is counted differing.
    let difference = byte_difference(&[1, 2, 3, 4], &[1, 2]);
    assert_eq!(difference.count, 2);
    assert_eq!(difference.first, Some(2));
    // Empty against empty is zero, not a panic.
    assert_eq!(byte_difference(&[], &[]).count, 0);
}

// ------------------------------------------------------- linkedFunctionExtent

const NM: &str = "08000100 00000010 T Func_08000100\n08000110 00000008 t Func_08000110\n";

#[test]
fn linked_function_extent_sums_to_the_last_symbol_end() {
    let extent = linked_function_extent(NM, "Func_08000100", 0x0800_0100 as f64, 0x20).unwrap();
    assert_eq!(extent, 0x18);
}

#[test]
fn linked_function_extent_silently_drops_uppercase_symbols() {
    // `/^Func_[0-9a-f]{8}$/` has NO `i` flag. This is faithful, and the drop
    // is the reason the target is then missing and the call errors.
    let upper = "08000100 00000010 T Func_08000ABC\n";
    let error = linked_function_extent(upper, "Func_08000ABC", 0x0800_0100 as f64, 0x20);
    assert_eq!(error, Err("compiled function symbols differ".to_string()));
}

#[test]
fn linked_function_extent_handles_crlf_and_js_whitespace_and_hex_fields() {
    let crlf = "08000100 00000010 T Func_08000100\r\n";
    assert_eq!(
        linked_function_extent(crlf, "Func_08000100", 0x0800_0100 as f64, 0x20),
        Ok(0x10)
    );
    // A non-breaking space is `\s` in JavaScript but not a plain ASCII split.
    let nbsp = "08000100\u{00a0}00000010\u{00a0}T\u{00a0}Func_08000100";
    assert_eq!(
        linked_function_extent(nbsp, "Func_08000100", 0x0800_0100 as f64, 0x20),
        Ok(0x10)
    );
}

#[test]
fn linked_function_extent_rejects_a_zero_size_and_an_overlong_extent() {
    let zero = "08000100 00000000 T Func_08000100\n";
    assert!(linked_function_extent(zero, "Func_08000100", 0x0800_0100 as f64, 0x20).is_err());
    // A byteLength that cannot contain the extent. 0x10 would EXCLUDE the
    // second symbol by the address filter rather than overflow, so the
    // overflow case needs a single oversized symbol.
    let big = "08000100 00000040 T Func_08000100\n";
    assert!(linked_function_extent(big, "Func_08000100", 0x0800_0100 as f64, 0x20).is_err());
}

// ------------------------------------------------------------ hexadecimalStem

#[test]
fn hexadecimal_stem_lowercases_before_matching_and_rejects_non_hex() {
    assert_eq!(hexadecimal_stem("exact/08ABCDEF.c").unwrap(), "08abcdef");
    assert_eq!(hexadecimal_stem("08000000").unwrap(), "08000000");
    assert!(hexadecimal_stem("exact/zzzzzzzz.c").is_err());
    assert!(hexadecimal_stem("exact/0800000.c").is_err());
    // `$` without `m` matches only at the very end of input, so an embedded
    // newline must NOT be accepted.
    assert!(hexadecimal_stem("08000000\nx").is_err());
}

// ---------------------------------------------------------------- sampling

fn member(stem: &str, address: u32) -> Member {
    Member { stem: stem.into(), source: stem.into(), address: address as f64, size: 4.0 }
}

#[test]
fn deterministic_sample_is_input_order_independent_and_matches_the_naive_comparator() {
    let members: Vec<Member> = (0..64u32)
        .map(|index| member(&format!("080000{index:02x}"), 0x0800_0000 + index))
        .collect();
    let forward = deterministic_sample(&members, 16, "seed");
    let mut reversed = members.clone();
    reversed.reverse();
    let backward = deterministic_sample(&reversed, 16, "seed");
    let stems = |list: &[Member]| list.iter().map(|m| m.stem.clone()).collect::<Vec<_>>();
    assert_eq!(stems(&forward), stems(&backward));
    assert_eq!(forward.len(), 16);

    // The decorate-sort-undecorate must produce EXACTLY the sequence the
    // TypeScript's naive comparator does, hash recomputed inside and all.
    let mut naive = members.clone();
    naive.sort_by(|left, right| {
        let a = hash(&[b"seed", left.stem.as_bytes()]);
        let b = hash(&[b"seed", right.stem.as_bytes()]);
        a.cmp(&b).then_with(|| {
            if left.address < right.address {
                std::cmp::Ordering::Less
            } else if left.address > right.address {
                std::cmp::Ordering::Greater
            } else {
                std::cmp::Ordering::Equal
            }
        })
    });
    naive.truncate(16);
    let mut sorted = naive;
    sorted.sort_by(|left, right| left.address.total_cmp(&right.address));
    assert_eq!(stems(&sorted), stems(&forward));
}

#[test]
fn deterministic_sample_of_zero_selects_everything_and_slice_clamps() {
    let members: Vec<Member> = (0..3u32)
        .map(|index| member(&format!("0800000{index}"), 0x0800_0000 + index))
        .collect();
    assert_eq!(deterministic_sample(&members, 0, "s").len(), 3);
    // `slice(0, 99)` CLAMPS where `[..99]` panics.
    assert_eq!(deterministic_sample(&members, 99, "s").len(), 3);
}

// ------------------------------------------------------------------- hash

#[test]
fn hash_separates_its_parts_so_concatenation_cannot_collide() {
    assert_ne!(hash(&[b"ab", b"c"]), hash(&[b"a", b"bc"]));
    assert_ne!(hash(&[b"a"]), hash(&[b"a", b""]));
}

// ------------------------------------------------------------------ config

#[test]
fn flags_of_dedupes_in_insertion_order_and_rejects_non_arrays() {
    let flags = flags_of(&strings(&[
        "-O2".into(),
        "-fno-gcse".into(),
        "-O2".into(),
    ]))
    .unwrap();
    // A `HashSet` would lose the order; gcc is later-flag-wins so order is
    // machine code.
    assert_eq!(flags, vec!["-O2".to_string(), "-fno-gcse".to_string()]);
    assert!(flags_of(&string("-O2")).is_err());
    assert!(flags_of(&Json::Array(vec![Json::Number(2.0)])).is_err());
}

#[test]
fn compiler_configuration_rejects_the_four_invalid_shapes() {
    for text in [
        r#"{"family":"unknown"}"#,
        r#"{"addFlags":"-O2"}"#,
        r#"{"addFlags":["-O2"],"removeFlags":["-O2"]}"#,
        r#"{"family":"gcc296","compiler_family":"old-agbcc"}"#,
    ] {
        assert!(
            compiler_configuration_of(&parse(text).unwrap()).is_err(),
            "accepted {text}"
        );
    }
}

// ---------------------------------------------------------------- CLI parse

fn args(list: &[&str]) -> Vec<String> {
    list.iter().map(|item| item.to_string()).collect()
}

fn options(list: &[&str]) -> compiler_corpus_regression::cli::Options {
    match parse_arguments(&args(list)).unwrap() {
        ParseOutcome::Options(options) => *options,
        ParseOutcome::Help => panic!("expected options"),
    }
}

#[test]
fn cli_rejects_a_run_that_changes_nothing() {
    assert!(parse_arguments(&args(&[])).is_err());
    assert!(parse_arguments(&args(&["--flags", ",,"])).is_err());
}

#[test]
fn cli_numeric_guards_use_javascript_semantics() {
    // `Number.isInteger(NaN)` is false, so this is the "must be >= 0" message
    // and not a Rust parse error.
    assert_eq!(
        parse_arguments(&args(&["--flags", "-O2", "--sample", "x"])).unwrap_err(),
        "--sample must be >= 0"
    );
    assert_eq!(
        parse_arguments(&args(&["--flags", "-O2", "--jobs", "0"])).unwrap_err(),
        "--jobs must be between 1 and 32"
    );
    // `--radius` is radix 0, so `0x10000` is 65,536.
    assert_eq!(options(&["--flags", "-O2", "--radius", "0x10000"]).radius, 65536.0);
    assert_eq!(options(&["--flags", "-O2", "--radius", "10000"]).radius, 10000.0);
    // `--sample`/`--jobs` are radix 10, so `0x8` is 0 and fails the jobs guard.
    assert!(parse_arguments(&args(&["--flags", "-O2", "--jobs", "0x8"])).is_err());
}

#[test]
fn cli_flags_dedupe_across_occurrences_keeping_the_first_position() {
    let parsed = options(&["--flags", "-O2,-fno-gcse", "--flags", "-O2,-fomit-frame-pointer"]);
    assert_eq!(
        parsed.flags,
        vec![
            "-O2".to_string(),
            "-fno-gcse".to_string(),
            "-fomit-frame-pointer".to_string()
        ]
    );
    // `filter(Boolean)` drops the empties.
    assert_eq!(options(&["--flags", ",,-O2,"]).flags, vec!["-O2".to_string()]);
}

#[test]
fn cli_sources_are_insertion_ordered_and_deduped() {
    let parsed = options(&["--flags", "-O2", "--sources", "08000010,08000000,08000010"]);
    assert_eq!(
        parsed.sources,
        vec!["08000010".to_string(), "08000000".to_string()]
    );
}

#[test]
fn cli_missing_value_and_unknown_argument_hard_fail() {
    assert!(parse_arguments(&args(&["--flags"])).is_err());
    assert_eq!(
        parse_arguments(&args(&["--nope"])).unwrap_err(),
        "unknown argument: --nope"
    );
}

// ------------------------------------------------------------------ slices

#[test]
fn rom_slice_clamps_where_rust_indexing_would_panic() {
    let rom = vec![0u8; 16];
    assert_eq!(rom_slice(&rom, 0x0800_0000 as f64, 4.0).len(), 4);
    // Past the end: `Buffer#subarray` clamps, `rom[a..b]` panics.
    assert_eq!(rom_slice(&rom, 0x0800_0010 as f64, 8.0).len(), 0);
    assert_eq!(rom_slice(&rom, 0x0800_000c as f64, 99.0).len(), 4);
    // Before the base, and NaN.
    assert_eq!(rom_slice(&rom, 0.0, 4.0).len(), 0);
    assert_eq!(rom_slice(&rom, f64::NAN, 4.0).len(), 0);
}

#[test]
fn error_truncation_counts_utf16_units() {
    let astral = "\u{1f600}".repeat(400); // 400 chars, 800 UTF-16 units
    let cut = utf16_slice_to(&astral, 600);
    assert_eq!(cut.encode_utf16().count(), 600);
    // A byte slice would have cut at a different character entirely.
    assert_eq!(cut.chars().count(), 300);
    assert_eq!(utf16_slice_to("short", 600), "short");
}

#[test]
fn relative_source_strips_root_length_plus_one_without_checking_the_prefix() {
    let root = std::path::Path::new("/a/b");
    assert_eq!(relative_source("/a/b/exact/x.c", root), "exact/x.c");
    // Faithful to the unchecked strip: a foreign path is mangled, not refused.
    assert_eq!(relative_source("/zzzzzzzzz/x.c", root), "zzzzz/x.c");
}

// ------------------------------------------------------------------- cache

fn example() -> Json {
    parse(
        r#"{"stem":"08000000","source":"exact/08000000.c","cache_key":"key",
            "cached":false,"compiled":true,"exact":true,"expected_size":4}"#,
    )
    .unwrap()
}

#[test]
fn cached_result_validates_only_four_fields() {
    assert!(cached_result(&example(), "key").is_some());
    assert!(cached_result(&example(), "stale").is_none());
    assert!(cached_result(&object(vec![("cache_key", string("key"))]), "key").is_none());
    assert!(cached_result(&Json::Null, "key").is_none());
}

#[test]
fn cached_result_accepts_a_row_that_makes_the_reporter_crash() {
    // BUG PINNED: `compiled: true, exact: false` with NO `first_difference` is
    // ACCEPTED here and is exactly what makes `result.first_difference!` a
    // TypeError in the TypeScript reporter.
    let row = parse(
        r#"{"stem":"08000000","cache_key":"key","compiled":true,"exact":false}"#,
    )
    .unwrap();
    let accepted = cached_result(&row, "key").expect("row must be accepted, defect and all");
    assert!(accepted.first_difference.is_none());
}

// -------------------------------------------------------------- report JSON

#[test]
fn every_number_the_report_can_emit_round_trips_through_the_js_writer() {
    // The float-formatting trap: ryu does not match JavaScript. Every number
    // this tool writes is a non-negative integer below 2^53, so the writer's
    // integer path is the only one reachable. This walks the whole set.
    let outcome = Outcome {
        stem: "08000000".into(),
        source: "exact/08000000.c".into(),
        cache_key: "k".into(),
        cached: false,
        compiled: true,
        exact: false,
        expected_size: 20,
        actual_size: Some(24),
        differing_bytes: Some(3),
        first_difference: Some(0),
        error: None,
    };
    let text = canonical_json(&outcome.to_json()).unwrap();
    // `canonicalJson` is the repository's pretty writer: two-space indent and
    // a space after each colon. The numbers are what matter here.
    assert!(text.contains(r#""expected_size": 20"#), "{text}");
    assert!(text.contains(r#""first_difference": 0"#), "{text}");
    // ryu would write `20.0`; ECMAScript writes `20`. Walk every number.
    for number in ["20", "24", "3", "0"] {
        assert!(!text.contains(&format!("{number}.0")), "float spelling: {text}");
    }
    // Key order is the file format.
    let json = outcome.to_json();
    let keys: Vec<&str> = match &json {
        Json::Object(entries) => entries.iter().map(|(key, _)| key.as_str()).collect(),
        _ => panic!("object"),
    };
    assert_eq!(
        keys,
        vec![
            "stem",
            "source",
            "cache_key",
            "cached",
            "compiled",
            "exact",
            "expected_size",
            "actual_size",
            "differing_bytes",
            "first_difference"
        ]
    );
}

#[test]
fn absent_optional_fields_are_omitted_and_never_written_as_null() {
    let outcome = Outcome {
        stem: "08000000".into(),
        source: "exact/08000000.c".into(),
        cache_key: "k".into(),
        cached: false,
        compiled: true,
        exact: true,
        expected_size: 20,
        actual_size: Some(20),
        differing_bytes: Some(0),
        first_difference: None,
        error: None,
    };
    let text = canonical_json(&outcome.to_json()).unwrap();
    assert!(!text.contains("first_difference"), "{text}");
    assert!(!text.contains("null"), "{text}");
}

#[test]
fn compiler_config_json_key_order_is_the_cache_key() {
    let config = compiler_configuration_of(
        &parse(r#"{"family":"gcc296","addFlags":["-O2"],"removeFlags":[]}"#).unwrap(),
    )
    .unwrap();
    assert_eq!(
        canonical_json(&compiler_config_json(&config)).unwrap(),
        "{\n  \"family\": \"gcc296\",\n  \"addFlags\": [\"-O2\"],\n  \"removeFlags\": []\n}"
    );
}

// --------------------------------------------------------------- collation

#[test]
fn source_sort_is_utf16_code_unit_order_not_str_cmp_and_not_locale_compare() {
    let mut list = vec!["b".to_string(), "A".to_string(), "a".to_string()];
    list.sort_by(|left, right| default_sort_cmp(left, right));
    // Default `sort()` is code-unit order, so uppercase sorts FIRST.
    // `localeCompare` would put "a" before "A".
    assert_eq!(list, vec!["A".to_string(), "a".to_string(), "b".to_string()]);
}
