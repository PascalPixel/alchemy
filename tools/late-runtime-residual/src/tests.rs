// Tests for the residual port. Every fixture is assembled from the tracked
// catalog rather than asserted against a frozen blob, so a catalog change fails
// the test instead of silently passing it.

use super::*;
use serde_json::json;

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn catalog_path() -> PathBuf {
    repository_root().join("assets/data/late_runtime_catalog.json")
}

fn catalog() -> LateRuntimeCatalog {
    read_late_runtime_catalog(&catalog_path()).expect("the tracked catalog parses")
}

fn scratch(name: &str) -> PathBuf {
    let directory = std::env::temp_dir().join(format!("late-residual-test-{name}"));
    let _ = std::fs::remove_dir_all(&directory);
    std::fs::create_dir_all(&directory).expect("scratch directory");
    directory
}

/// The self-test's synthetic image: `aki_*` components filled 0xff, everything
/// else 0x00, so every component is a `uniform_fill`.
fn synthetic_image() -> Vec<u8> {
    let mut image = vec![0u8; 0x100000];
    for range in &catalog().ranges {
        for component in &range.components {
            let start = (component.address - ROM_BASE) as usize;
            let end = (component.end - ROM_BASE) as usize;
            image[start..end].fill(if component.name.starts_with("aki_") {
                255
            } else {
                0
            });
        }
    }
    image
}

fn export_to(image: &[u8], name: &str) -> PathBuf {
    let directory = scratch(name);
    let rom = directory.join("rom.gba");
    std::fs::write(&rom, image).expect("write rom");
    let out = directory.join("source");
    export_late_runtime_residual(&rom, &out, &catalog_path()).expect("export succeeds");
    out.join("index.json")
}

fn read_json(path: &Path) -> Value {
    serde_json::from_str(&std::fs::read_to_string(path).expect("read")).expect("parse")
}

fn write_json(path: &Path, value: &Value) {
    std::fs::write(path, pretty(value)).expect("write");
}

// ---------------------------------------------------------------------------
// layout and totals
// ---------------------------------------------------------------------------

#[test]
fn the_tracked_catalog_covers_exactly_the_residual_byte_total() {
    let total: i64 = catalog()
        .ranges
        .iter()
        .map(|range| i64::from(range.end - range.address))
        .sum();
    assert_eq!(total, SOURCE_BYTES);
    assert_eq!(catalog().ranges.len(), 3);
}

#[test]
fn a_synthetic_image_round_trips_through_the_source_document() {
    let image = synthetic_image();
    let index = export_to(&image, "roundtrip");
    let built = build_late_runtime_residual(&index, &catalog_path()).expect("build");
    assert_eq!(built.source_bytes, SOURCE_BYTES);
    assert_eq!(built.regions.len(), 3);
    for (address, data) in built.regions.iter() {
        let start = (*address - ROM_BASE) as usize;
        assert_eq!(data.as_slice(), &image[start..start + data.len()]);
    }
}

#[test]
fn region_addresses_keep_catalog_order() {
    let index = export_to(&synthetic_image(), "order");
    let built = build_late_runtime_residual(&index, &catalog_path()).expect("build");
    let seen: Vec<u32> = built.regions.iter().map(|(address, _)| *address).collect();
    let expected: Vec<u32> = catalog().ranges.iter().map(|range| range.address).collect();
    assert_eq!(seen, expected);
}

// ---------------------------------------------------------------------------
// representation
// ---------------------------------------------------------------------------

#[test]
fn representation_picks_uniform_fill_only_for_zero_and_ff() {
    assert_eq!(
        representation(&[0, 0, 0]).unwrap(),
        Values::UniformFill { value: 0 }
    );
    assert_eq!(
        representation(&[255, 255]).unwrap(),
        Values::UniformFill { value: 255 }
    );
    // A uniform run of any other byte stays explicit: the TypeScript gates on
    // `data[0] === 0 || data[0] === 255` before testing uniformity.
    assert_eq!(
        representation(&[7, 7, 7]).unwrap(),
        Values::ByteValues {
            values: vec![7, 7, 7]
        }
    );
    assert_eq!(
        representation(&[0, 0, 1]).unwrap(),
        Values::ByteValues {
            values: vec![0, 0, 1]
        }
    );
    assert_eq!(
        representation(&[]).unwrap_err(),
        "late residual component is empty"
    );
}

#[test]
fn a_non_uniform_image_emits_byte_values() {
    let mut image = synthetic_image();
    let first = catalog().ranges[0].components[0].clone();
    let start = (first.address - ROM_BASE) as usize;
    image[start] = 1;
    image[start + 1] = 2;
    let index = export_to(&image, "bytevalues");
    let document = read_json(&index);
    let component = &document["regions"][0]["components"][0];
    assert_eq!(component["representation"], json!("byte_values"));
    assert_eq!(component["values"][0], json!(1));
    assert_eq!(component["values"].as_array().unwrap().len(), first.size());
    build_late_runtime_residual(&index, &catalog_path()).expect("build");
}

// ---------------------------------------------------------------------------
// rejections -- each one drives a distinct guard in the TypeScript
// ---------------------------------------------------------------------------

#[test]
fn malformed_sources_are_rejected_with_the_typescript_messages() {
    let index = export_to(&synthetic_image(), "reject");
    let original = read_json(&index);
    let mut rejected = 0usize;

    let mut check = |mutate: &dyn Fn(&mut Value), expected: &str| {
        let mut document = original.clone();
        mutate(&mut document);
        write_json(&index, &document);
        let error = build_late_runtime_residual(&index, &catalog_path())
            .expect_err("the mutation must be rejected");
        assert_eq!(error, expected, "for expected {expected}");
        rejected += 1;
    };

    check(
        &|value| value["format"] = json!(2),
        "late residual source identity differs",
    );
    check(
        &|value| value["kind"] = json!("other"),
        "late residual source identity differs",
    );
    check(
        &|value| value["regions"] = json!({}),
        "late residual source identity differs",
    );
    check(
        &|value| {
            value.as_object_mut().unwrap().remove("source_bytes");
        },
        "late residual source fields differ",
    );
    check(
        &|value| {
            value["regions"].as_array_mut().unwrap().pop();
        },
        "late residual region count differs",
    );
    check(
        &|value| value["regions"][0]["name"] = json!("nope"),
        "range_080f38bc layout differs",
    );
    check(
        &|value| value["regions"][0]["address"] = json!("0x00000000"),
        "range_080f38bc layout differs",
    );
    check(
        &|value| {
            value["regions"][0]["components"]
                .as_array_mut()
                .unwrap()
                .pop();
        },
        "range_080f38bc layout differs",
    );
    check(
        &|value| value["regions"][0]["components"][0]["role"] = json!("x"),
        "haikei_stream metadata differs",
    );
    check(
        &|value| value["regions"][0]["components"][0]["type"] = json!("u16"),
        "haikei_stream metadata differs",
    );
    check(
        &|value| value["regions"][0]["components"][0]["value"] = json!(3),
        "haikei_stream fill differs",
    );
    check(
        &|value| value["regions"][0]["components"][0]["values"] = json!([]),
        "haikei_stream fields differ",
    );
    check(
        &|value| value["regions"][0]["components"][0] = json!(Value::Null),
        "haikei_stream is null",
    );
    check(
        &|value| value["source_bytes"] = json!(7295),
        "late residual source-byte total differs",
    );

    assert_eq!(rejected, 14, "every guard above must have fired");
    write_json(&index, &original);
    build_late_runtime_residual(&index, &catalog_path()).expect("the original still builds");
}

#[test]
fn a_byte_values_entry_outside_the_byte_range_is_rejected() {
    let mut image = synthetic_image();
    let start = (catalog().ranges[0].components[0].address - ROM_BASE) as usize;
    image[start] = 1;
    let index = export_to(&image, "byterange");
    let mut document = read_json(&index);
    document["regions"][0]["components"][0]["values"][2] = json!(256);
    write_json(&index, &document);
    assert_eq!(
        build_late_runtime_residual(&index, &catalog_path()).unwrap_err(),
        "haikei_stream value 2 differs"
    );
}

/// `1.0` is the integer 1 in JS and passes `Number.isSafeInteger`, so a source
/// spelled with a trailing `.0` must still build. (It will fail the canonical
/// JSON check first if written that way by hand, so this exercises the
/// primitive directly.)
#[test]
fn a_float_spelled_integer_is_a_safe_integer() {
    assert_eq!(safe_integer(&json!(1.0)), Some(1));
    assert_eq!(safe_integer(&json!(1.5)), None);
    assert_eq!(safe_integer(&json!("1")), None);
    assert_eq!(integer(&json!(255.0), 0, 255, "x").unwrap(), 255);
    assert_eq!(integer(&json!(256), 0, 255, "x").unwrap_err(), "x differs");
}

#[test]
fn a_non_canonical_source_is_rejected() {
    let directory = scratch("noncanonical");
    let index = directory.join("index.json");
    std::fs::write(&index, "{\"format\":1,\"kind\":\"golden-sun-late-runtime-residual\",\"source_bytes\":7294,\"regions\":[]}").expect("write");
    assert_eq!(
        build_late_runtime_residual(&index, &catalog_path()).unwrap_err(),
        "late residual source is not canonical JSON"
    );
}

#[test]
fn a_non_object_source_is_rejected() {
    let directory = scratch("nonobject");
    let index = directory.join("index.json");
    std::fs::write(&index, "[]\n").expect("write");
    assert_eq!(
        build_late_runtime_residual(&index, &catalog_path()).unwrap_err(),
        "late residual source must be an object"
    );
}

// ---------------------------------------------------------------------------
// truncated ROMs -- the clamping `subarray` path
// ---------------------------------------------------------------------------

#[test]
fn a_rom_too_small_for_the_catalog_reports_the_component_by_name() {
    let directory = scratch("short");
    let rom = directory.join("rom.gba");
    std::fs::write(&rom, vec![0u8; 0x1000]).expect("write");
    let error =
        export_late_runtime_residual(&rom, &directory.join("out"), &catalog_path()).unwrap_err();
    assert_eq!(error, "haikei_stream lies outside the ROM");
}

#[test]
fn verify_against_a_different_image_reports_the_region_address() {
    let index = export_to(&synthetic_image(), "verifydiff");
    let directory = scratch("verifydiff-rom");
    let mut image = synthetic_image();
    image[(catalog().ranges[0].address - ROM_BASE) as usize] = 9;
    let rom = directory.join("rom.gba");
    std::fs::write(&rom, &image).expect("write");
    assert_eq!(
        verify_late_runtime_residual(&rom, &index, &catalog_path()).unwrap_err(),
        "0x080f38bc differs from the ROM"
    );
}

// ---------------------------------------------------------------------------
// output shape
// ---------------------------------------------------------------------------

fn walk_numbers(value: &Value, count: &mut usize) {
    match value {
        Value::Number(number) => {
            *count += 1;
            let raw = number.as_f64().expect("finite JSON number");
            assert!(
                raw.fract() == 0.0,
                "non-integral number {raw} reached the output"
            );
        }
        Value::Array(items) => items.iter().for_each(|item| walk_numbers(item, count)),
        Value::Object(map) => map.values().for_each(|item| walk_numbers(item, count)),
        _ => {}
    }
}

#[test]
fn emitted_json_is_integers_only() {
    let mut image = synthetic_image();
    image[(catalog().ranges[0].components[0].address - ROM_BASE) as usize] = 1;
    let index = export_to(&image, "integers");
    let mut count = 0usize;
    walk_numbers(&read_json(&index), &mut count);
    assert!(
        count > 100,
        "the walk must actually have seen numbers, saw {count}"
    );
}

#[test]
fn component_keys_are_emitted_in_source_order() {
    let index = export_to(&synthetic_image(), "keyorder");
    let document = read_json(&index);
    let keys: Vec<&str> = document["regions"][0]["components"][0]
        .as_object()
        .unwrap()
        .keys()
        .map(String::as_str)
        .collect();
    assert_eq!(
        keys,
        [
            "name",
            "address",
            "end",
            "role",
            "type",
            "representation",
            "value"
        ]
    );
    let top: Vec<&str> = document
        .as_object()
        .unwrap()
        .keys()
        .map(String::as_str)
        .collect();
    assert_eq!(top, ["format", "kind", "source_bytes", "regions"]);
}

#[test]
fn typed_kind_maps_the_catalog_kinds() {
    let components = catalog();
    let mut seen: Vec<&str> = Vec::new();
    for range in &components.ranges {
        for component in &range.components {
            let kind = typed_kind(component);
            if !seen.contains(&kind) {
                seen.push(kind);
            }
            let expected = match component.kind {
                ComponentKind::Table(TableKind::S16) => "s16",
                ComponentKind::Table(TableKind::U16) => "u16",
                _ => "u8",
            };
            assert_eq!(kind, expected);
        }
    }
    seen.sort_unstable();
    assert_eq!(seen, ["s16", "u16", "u8"]);
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

#[test]
fn the_self_test_passes_and_cleans_up() {
    let line = self_test(&catalog_path()).expect("self-test passes");
    assert_eq!(line, "self-test=ok source_bytes=7294");
    let temporary = temporary_root().join(format!("alchemy-late-residual-{}", std::process::id()));
    assert!(
        !temporary.exists(),
        "the self-test directory must be removed"
    );
}
