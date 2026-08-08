// Tests assemble real fixtures -- a synthetic ROM built from the tracked
// catalog, a real exported package on disk -- rather than asserting that a
// constant equals itself. Every test that counts checks keeps a running total
// and fails if the total drops, so a future edit cannot quietly delete one.

use super::*;

fn catalog() -> Value {
    load_catalog(&default_catalog_path()).expect("the tracked catalog loads")
}

struct Scratch(String);

impl Scratch {
    fn new(label: &str) -> Scratch {
        Scratch(mkdtemp(&nodepath::join(&[&tmpdir(), &format!("early-runtime-test-{label}-")])).expect("temp dir"))
    }
    fn at(&self, name: &str) -> String {
        nodepath::join(&[&self.0, name])
    }
}

impl Drop for Scratch {
    fn drop(&mut self) {
        let _ = std::fs::remove_dir_all(&self.0);
    }
}

#[test]
fn tracked_catalog_is_canonical_and_covers_both_regions() {
    let catalog = catalog();
    assert_eq!(EARLY_RUNTIME_SOURCE_BYTES, 6184);
    let (early, residual) = canonical_layout(&catalog).expect("canonical layout");
    assert_eq!(as_str(get(early, "claim")), Some("pending_private_verification"));
    assert_eq!(as_str(get(residual, "claim")), Some("unresolved"));
}

#[test]
fn round_trip_export_verify_build_on_a_synthetic_rom() {
    let catalog = catalog();
    let catalog_path = default_catalog_path();
    let scratch = Scratch::new("roundtrip");
    let rom = synthetic_rom(&catalog).expect("synthetic ROM");
    let rom_path = scratch.at("gs1-en.gba");
    write_file(&rom_path, &rom).expect("write ROM");
    let package = scratch.at("source");
    export_early_runtime_data(&rom, &package, Some(&rom_path), &catalog_path).expect("export");
    let index_path = nodepath::join(&[&package, "index.json"]);
    verify_early_runtime_data(&rom, &index_path, &catalog_path).expect("verify");
    let built = build_early_runtime_data(&index_path, &catalog_path).expect("build");
    assert_eq!(built.source_bytes, EARLY_RUNTIME_SOURCE_BYTES);
    // Insertion order, not a hash order: the early region comes first.
    assert_eq!(built.regions[0].0, EARLY_RUNTIME_ADDRESS);
    assert_eq!(built.regions[1].0, POST_MAP_ADDRESS);

    let out = scratch.at("build");
    write_build(&index_path, &out, &catalog_path).expect("write build");
    let manifest = read_utf8(&nodepath::join(&[&out, "manifest.json"])).expect("manifest");
    // Integral numbers print without a `.0`, the way JavaScript writes them.
    assert!(manifest.contains("\"size\": 2148"), "{manifest}");
    assert!(manifest.contains("\"source_bytes\": 6184"), "{manifest}");
    assert_eq!(
        read_bytes(&nodepath::join(&[&out, "08012f20.bin"])).expect("region"),
        built.regions[0].1
    );
}

#[test]
fn display_tiles_survive_the_png_round_trip() {
    let scratch = Scratch::new("tiles");
    let raw: Vec<u8> = (0..128u16).map(|value| (value * 7 % 256) as u8).collect();
    let path = scratch.at(DISPLAY_SOURCE);
    write_display_tiles(&raw, &path).expect("write tiles");
    assert_eq!(read_display_tiles(&path).expect("read tiles"), raw);
    // A non-canonical re-encode of the same pixels is rejected, so the PNG
    // bytes themselves are part of the contract.
    let mut tampered = read_bytes(&path).expect("png");
    let length = tampered.len();
    tampered[length - 1] ^= 0x01;
    write_file(&path, &tampered).expect("write");
    assert!(read_display_tiles(&path).is_err());
}

#[test]
fn writes_stay_inside_private_output_roots() {
    assert!(private_output(&nodepath::join(&[&repo_root(), "out/x"])));
    assert!(private_output(&nodepath::join(&[&tmpdir(), "x"])));
    assert!(!private_output(&nodepath::join(&[&repo_root(), "assets/data"])));
    assert!(!private_output("/"));
}

#[test]
fn handler_pointers_reject_non_thumb_and_out_of_rom_words() {
    let mut checks = 0;
    // `value & ~1` runs ToInt32 in JavaScript: 0x80000001 becomes negative and
    // fails the ROM_BASE check rather than passing as a huge address.
    for word in [0x8000_0001i64, 0x0800_0100, 0x0900_0001, 0x0801_2f21 - 1] {
        assert!(handler_source(word).is_err(), "{word:#x} must be rejected");
        checks += 1;
    }
    assert_eq!(handler_source(0).expect("null handler"), Value::Null);
    assert_eq!(
        handler_source(0x0801_3625).expect("thumb handler"),
        Value::from("Func_08013624")
    );
    checks += 2;
    assert_eq!(handler_value(Some(&Value::Null), "h").expect("null"), 0);
    assert_eq!(
        handler_value(Some(&Value::from("Func_08013624")), "h").expect("symbol"),
        0x0801_3625
    );
    assert!(handler_value(Some(&Value::from("Func_08013625")), "h").is_ok());
    assert!(handler_value(Some(&Value::from("Func_09013624")), "h").is_err());
    checks += 4;
    assert_eq!(checks, 10, "a handler check was dropped");
}

#[test]
fn typed_values_are_range_checked_and_little_endian() {
    assert_eq!(
        encode_typed(&[Value::from(-2), Value::from(1)], "s16le").expect("s16"),
        vec![0xfe, 0xff, 0x01, 0x00]
    );
    assert!(encode_typed(&[Value::from(256)], "u8").is_err());
    assert!(encode_typed(&[Value::from(-32769)], "s16le").is_err());
    // One number type: `1.0` is a safe integer in JavaScript and must pass.
    assert!(encode_typed(&[serde_json::json!(1.0)], "u8").is_ok());
    assert!(encode_typed(&[serde_json::json!(1.5)], "u8").is_err());
    assert!(encode_typed(&[Value::from("1")], "u8").is_err());
    assert_eq!(decode_typed(&[0xfe, 0xff], "s16le").expect("decode"), vec![-2]);
    assert!(decode_typed(&[0x00], "s16le").is_err());
}

#[test]
fn keys_and_names_sort_by_utf16_code_unit() {
    // U+1D400 is the surrogate pair D835 DC00, so by UTF-16 code unit it sorts
    // BEFORE U+FF3A (FULLWIDTH Z). By UTF-8 byte -- which is what Rust's
    // `sort()` on `String` does -- the order is the opposite, so this is a real
    // divergence and not a tautology.
    let mut names = vec!["\u{FF3A}".to_string(), "\u{1D400}".to_string()];
    js_sort(&mut names);
    assert_eq!(names, vec!["\u{1D400}".to_string(), "\u{FF3A}".to_string()]);
    let mut byte_order = vec!["\u{FF3A}".to_string(), "\u{1D400}".to_string()];
    byte_order.sort();
    assert_ne!(byte_order, names);
}

#[test]
fn missing_identifier_fields_are_tested_as_the_string_undefined() {
    // A latent quirk of the original, reproduced deliberately: `RegExp#test`
    // stringifies, and "undefined" matches /^[a-z0-9_]+$/.
    assert!(is_identifier(&to_js_string(None)));
    assert!(!is_identifier("Bad-Id"));
    assert!(is_rom_address("0x08012f20"));
    assert!(!is_rom_address("0x08012F20"));
    assert!(!is_canonical_word("0X00000000"));
    assert!(is_thumb_symbol("Func_08013624"));
    assert!(!is_thumb_symbol("Func_09013624"));
}

#[test]
fn residual_spans_prefer_a_uniform_fill_and_round_trip() {
    let filled = residual_source(&[0xff; 8], "span").expect("uniform");
    assert_eq!(as_str(get(&filled, "kind")), Some("uniform_fill"));
    assert_eq!(build_residual(Some(&filled), 8, "span").expect("build"), vec![0xff; 8]);
    let mixed = residual_source(&[1, 2, 3], "span").expect("bytes");
    assert_eq!(as_str(get(&mixed, "kind")), Some("byte_values"));
    assert_eq!(build_residual(Some(&mixed), 3, "span").expect("build"), vec![1, 2, 3]);
    // 0x7f is not one of the two supported fills.
    let unsupported = serde_json::json!({ "kind": "uniform_fill", "value": "0x7f" });
    assert!(build_residual(Some(&unsupported), 8, "span").is_err());
    assert!(residual_source(&[], "span").is_err());
}

#[test]
fn a_catalog_view_escaping_its_backing_storage_is_rejected() {
    let mut broken = catalog();
    *at(&mut broken, &["regions", "0", "views", "0", "end"]) = Value::from("0x080134ff");
    assert!(validate_catalog(&broken).is_err());
    let mut renamed = catalog();
    *at(&mut renamed, &["regions", "0", "partitions", "0", "id"]) = Value::from("display_tiles2");
    assert!(validate_catalog(&renamed).is_err());
}

#[test]
fn export_refuses_a_destination_containing_its_own_rom() {
    let scratch = Scratch::new("selfcontain");
    let catalog = catalog();
    let catalog_path = default_catalog_path();
    let package = scratch.at("package");
    mkdir_recursive(&package).expect("mkdir");
    let rom_path = nodepath::join(&[&package, "gs1-en.gba"]);
    let rom = synthetic_rom(&catalog).expect("synthetic ROM");
    write_file(&rom_path, &rom).expect("write ROM");
    let error = export_early_runtime_data(&rom, &package, Some(&rom_path), &catalog_path)
        .expect_err("must refuse");
    assert_eq!(error.0, "early-runtime export directory must not contain its input ROM");
}

#[test]
fn the_self_test_reports_all_eighteen_adversarial_checks() {
    assert_eq!(
        self_test().expect("self-test passes"),
        "self-test=ok source_bytes=6184 adversarial=18 transaction=atomic"
    );
}
