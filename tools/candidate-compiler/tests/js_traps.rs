//! JavaScript numeric and buffer semantics retained by the candidate compiler.

use candidate_compiler::jsnum::{parse_hex, to_js_number_string};
use candidate_compiler::verify::js_subarray;

#[test]
fn parse_hex_accepts_uppercase_because_the_regex_carries_its_i_flag() {
    // `/^[0-9a-f]+$/i`. A port that drops the `i` rejects every uppercase stem
    // and every uppercase `nm` size field. That defect exists elsewhere in this
    // repository; it does not exist here.
    assert_eq!(parse_hex("0800ABCD").unwrap(), 0x0800_abcd);
    assert_eq!(parse_hex("0800abcd").unwrap(), 0x0800_abcd);
    assert_eq!(parse_hex("F").unwrap(), 15);
    // The class is ASCII-only, as JavaScript character classes are.
    assert!(parse_hex("").is_err());
    assert!(parse_hex("0x10").is_err());
    assert!(parse_hex("１２").is_err()); // fullwidth digits
    assert!(parse_hex("١٢").is_err()); // Arabic-Indic digits
    assert!(parse_hex(" 10").is_err()); // anchored, so no leading space
    assert!(parse_hex("10\n").is_err()); // `$` here is not multiline
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
