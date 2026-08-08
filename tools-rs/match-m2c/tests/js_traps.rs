//! Every JavaScript-semantics trap this port dodges, pinned.
//!
//! The point of each test is that the naive Rust spelling PASSES the functional
//! path and FAILS here. If a later change "simplifies" one of these helpers back
//! to `char::is_whitespace`, `f64::max`, `str::lines`, `&data[a..b]`, or a plain
//! string comparison, exactly one of these tests goes red.

use match_m2c::json::{canonical_json, parse, Json};
use match_m2c::jsnum::{
    compare_tuple, is_strictly_better, math_max, parse_hex, to_js_number_string,
};
use match_m2c::jsstring::{is_js_space, js_split_lines, js_split_whitespace_runs, js_trim, utf16_cmp};
use match_m2c::verify::js_subarray;

#[test]
fn js_whitespace_differs_from_rust_in_both_directions() {
    // Rust says U+0085 is whitespace. JavaScript's `\s` does not contain it.
    assert!('\u{85}'.is_whitespace());
    assert!(!is_js_space('\u{85}'));
    // JavaScript's `\s` contains U+FEFF. Rust's `White_Space` does not.
    assert!(!'\u{feff}'.is_whitespace());
    assert!(is_js_space('\u{feff}'));
    // The exotic spaces JavaScript does share.
    for c in ['\u{a0}', '\u{1680}', '\u{2000}', '\u{200a}', '\u{202f}', '\u{205f}', '\u{3000}'] {
        assert!(is_js_space(c), "{c:?}");
    }
    // U+200B ZERO WIDTH SPACE is in neither set, despite the name.
    assert!(!is_js_space('\u{200b}'));

    // The consequence: `"a\u{85}b"` is ONE field in JavaScript, two under a
    // naive port.
    assert_eq!(js_split_whitespace_runs("a\u{85}b"), vec!["a\u{85}b"]);
    assert_eq!(js_trim("\u{feff} x \u{feff}"), "x");
    assert_eq!(js_trim("\u{85}x"), "\u{85}x");
}

#[test]
fn split_keeps_the_empty_fields_that_split_whitespace_drops() {
    // `"  a  ".split(/\s+/)` is `["", "a", ""]`, not `["a"]`.
    assert_eq!(js_split_whitespace_runs("  a  "), vec!["", "a", ""]);
    // `"".split(/\s+/)` is `[""]`, not `[]`.
    assert_eq!(js_split_whitespace_runs(""), vec![""]);
    // `str::split_whitespace` gives none of that.
    assert_eq!("  a  ".split_whitespace().count(), 1);
}

#[test]
fn line_split_keeps_the_trailing_empty_field() {
    // `"a\n".split(/\r?\n/)` is `["a", ""]`; `str::lines` yields just `["a"]`.
    assert_eq!(js_split_lines("a\n"), vec!["a", ""]);
    assert_eq!("a\n".lines().count(), 1);
    assert_eq!(js_split_lines("a\r\nb"), vec!["a", "b"]);
    // A bare `\r` is NOT a separator for `/\r?\n/`.
    assert_eq!(js_split_lines("a\rb"), vec!["a\rb"]);
    assert_eq!(js_split_lines(""), vec![""]);
}

#[test]
fn sort_orders_by_utf16_code_unit_not_by_byte() {
    // U+FF00 (BMP, one UTF-16 unit) against U+10000 (a surrogate pair whose
    // lead unit is U+D800). By byte, U+10000 sorts after; by UTF-16 code unit
    // it sorts BEFORE, because 0xD800 < 0xFF00.
    let bmp = "\u{ff00}";
    let astral = "\u{10000}";
    assert!(bmp.as_bytes() < astral.as_bytes());
    assert_eq!(utf16_cmp(astral, bmp), std::cmp::Ordering::Less);
    // Plain ASCII agrees, which is why the difference never shows up in the
    // corpus and why this test has to construct the case.
    assert_eq!(utf16_cmp("0800a.c", "0800b.c"), std::cmp::Ordering::Less);
}

#[test]
fn parse_hex_accepts_uppercase_because_the_regex_carries_its_i_flag() {
    // `/^[0-9a-f]+$/i`. A port that drops the `i` rejects every uppercase stem
    // and every uppercase `nm` size field. That defect exists elsewhere in this
    // repository; it does not exist here.
    assert_eq!(parse_hex("0800ABCD").unwrap(), 0x0800_abcd as f64);
    assert_eq!(parse_hex("0800abcd").unwrap(), 0x0800_abcd as f64);
    assert_eq!(parse_hex("F").unwrap(), 15.0);
    // The class is ASCII-only, as JavaScript character classes are.
    assert!(parse_hex("").is_err());
    assert!(parse_hex("0x10").is_err());
    assert!(parse_hex("１２").is_err()); // fullwidth digits
    assert!(parse_hex("١٢").is_err()); // Arabic-Indic digits
    assert!(parse_hex(" 10").is_err()); // anchored, so no leading space
    assert!(parse_hex("10\n").is_err()); // `$` here is not multiline
}

#[test]
fn math_max_propagates_nan_where_f64_max_swallows_it() {
    assert!(math_max(0.0, f64::NAN).is_nan());
    // The naive spelling, for contrast. This is the whole poisoned-span defect.
    assert_eq!(0.0f64.max(f64::NAN), 0.0);
}

#[test]
fn a_poisoned_score_never_displaces_the_incumbent() {
    let incumbent = [10.0, -4.0, 2.0];
    let poisoned = [f64::NAN, -100.0, 0.0];
    // `compareTuple` returns NaN, and `NaN < 0` is false.
    assert!(compare_tuple(poisoned, incumbent).is_nan());
    assert!(!is_strictly_better(poisoned, incumbent));
    // ...and it is not "worse" either: the incumbent does not beat it back.
    assert!(!is_strictly_better(incumbent, poisoned));
    // `total_cmp` would call the poisoned score GREATER and `partial_cmp` would
    // give `None`; both would be a different program.
    assert_eq!(f64::NAN.total_cmp(&0.0), std::cmp::Ordering::Greater);
    assert_eq!(f64::NAN.partial_cmp(&0.0), None);
    // A genuine improvement still wins.
    assert!(is_strictly_better([9.0, 0.0, 0.0], incumbent));
    assert!(!is_strictly_better(incumbent, incumbent));
}

#[test]
fn overlay_address_clamps_to_rom_start() {
    let rom: Vec<u8> = (0u8..32).collect();
    // `0x02000000 - 0x08000000` is hugely negative. `&rom[offset..]` would not
    // even compile as an index; `subarray` clamps to 0 and returns bytes from
    // the START of the ROM. Wrong, faithfully.
    let offset = f64::from(0x0200_0000u32) - f64::from(0x0800_0000u32);
    let sliced = js_subarray(&rom, offset, offset + 4.0);
    assert_eq!(sliced, Vec::<u8>::new());
    // A moderate negative index is measured from the end, not rejected.
    assert_eq!(js_subarray(&rom, -3.0, 32.0), vec![29, 30, 31]);
    // An end past the buffer clamps instead of panicking.
    assert_eq!(js_subarray(&rom, 30.0, 9_999.0), vec![30, 31]);
    // NaN becomes 0.
    assert_eq!(js_subarray(&rom, f64::NAN, 2.0), vec![0, 1]);
    // An inverted range is empty, not a panic.
    assert_eq!(js_subarray(&rom, 5.0, 2.0), Vec::<u8>::new());
}

#[test]
fn to_number_follows_the_string_numeric_literal_grammar() {
    let n = |text: &str| Json::to_js_number(Some(&Json::String(text.to_string())));
    assert_eq!(n(""), 0.0); // `Number("")` is 0, not NaN
    assert_eq!(n("  \t 12 "), 12.0); // trimmed
    assert_eq!(n("0x10"), 16.0); // hex prefix accepted
    assert_eq!(n("0b101"), 5.0);
    assert_eq!(n("0o17"), 15.0);
    assert!(n("12abc").is_nan()); // no prefix parse, unlike `parseInt`
    assert_eq!(n("Infinity"), f64::INFINITY);
    // `f64::from_str` accepts all three of these. `Number` accepts none.
    assert!(n("inf").is_nan());
    assert!(n("nan").is_nan());
    assert!(n("1_0").is_nan());
    assert!("inf".parse::<f64>().is_ok());
    // `undefined` is NaN; `null` is 0. This is how a missing `size` poisons a
    // span.
    assert!(Json::to_js_number(None).is_nan());
    assert_eq!(Json::to_js_number(Some(&Json::Null)), 0.0);
    assert_eq!(Json::to_js_number(Some(&Json::Bool(true))), 1.0);
    assert_eq!(Json::to_js_number(Some(&Json::Array(vec![]))), 0.0);
    assert!(Json::to_js_number(Some(&Json::Object(vec![]))).is_nan());
}

#[test]
fn numbers_are_written_the_way_javascript_writes_them() {
    // ryu writes `1.0`; JavaScript writes `1`.
    assert_eq!(to_js_number_string(1.0).unwrap(), "1");
    assert_eq!(to_js_number_string(-0.0).unwrap(), "0");
    assert_eq!(to_js_number_string(134_217_728.0).unwrap(), "134217728");
    // Anything this function cannot write the JavaScript way is refused rather
    // than guessed at.
    assert!(to_js_number_string(1.5).is_err());
    assert!(to_js_number_string(f64::NAN).is_err());
    assert!(to_js_number_string(1e-7).is_err());
}

#[test]
fn object_key_order_is_insertion_order() {
    // The one reason there is no `HashMap` anywhere in this crate.
    let value = Json::Object(vec![
        ("entry".into(), Json::Number(0x0800_0000 as f64)),
        ("matched".into(), Json::Bool(false)),
        ("size".into(), Json::Number(4.0)),
    ]);
    let text = canonical_json(&value).unwrap();
    let entry = text.find("entry").unwrap();
    let matched = text.find("matched").unwrap();
    let size = text.find("\"size\"").unwrap();
    assert!(entry < matched && matched < size);
    // A round trip preserves it, duplicate keys included (last value wins, first
    // position kept, as `JSON.parse` does).
    let reparsed = parse(r#"{"b":1,"a":2,"b":3}"#).unwrap();
    let Json::Object(entries) = &reparsed else {
        panic!("expected an object");
    };
    assert_eq!(entries.len(), 2);
    assert_eq!(entries[0].0, "b");
    assert_eq!(entries[1].0, "a");
    assert_eq!(Json::to_js_number(reparsed.get("b")), 3.0);
}
