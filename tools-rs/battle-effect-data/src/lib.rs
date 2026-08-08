// Rust port of tools/make/battle_effect_data.ts.
//
// Why this file exists: 0x0809c410..0x080a1000 in the main image is one
// contiguous blob that is not code and not a single kind of data either. It is
// sprite tiles, weighted outcome records, eight rule tables, twenty-three
// smaller tail tables, two different LZ-compressed graphic streams, twenty-nine
// bytecode object scripts, a generated sine table, a gradient table, a symbolic
// pointer triple and a sparse lookup, packed nose-to-tail with no padding. The
// branch tracks that blob as one reviewable JSON document plus six PNGs, and
// this tool is the only thing that converts between the two directions. So it
// has to be exact in both: `export` reproduces the tracked document and PNGs
// byte-for-byte, `build` reproduces the ROM span byte-for-byte, and the export
// path proves the round trip before it returns.
//
// The port keeps every layout table, the field order of the emitted document,
// and every error message verbatim, because the first two are compared against
// the tracked files by the rest of the toolchain and the third is compared
// against the TypeScript in the parity run.

pub mod json;
pub mod jsnum;
pub mod tilepng;

use std::path::{Path, PathBuf};

use extract_resource::{decode_palette_trace, encode_palette, PaletteGroup, PaletteOperation};
use import_asset::gba_graphics;
use json::{canonical_json, minified, Value};
use jsnum::to_number;
use tilepng::tile_png;

pub const ROM_BASE: u32 = 0x0800_0000;
pub const BATTLE_DATA_ADDRESS: u32 = 0x0809_c410;
pub const BATTLE_DATA_END: u32 = 0x080a_1000;

const DIRECT_GRAPHICS_END: u32 = 0x0809_c610;
const WEIGHTED_RECORDS_END: u32 = 0x0809_d170;
const RULE_TABLES_END: u32 = 0x0809_e4ce;
const HALFWORD_GRAPHIC_END: u32 = 0x0809_e680;
const SINE_TABLE_ADDRESS: u32 = 0x0809_ed84;
const SINE_TABLE_END: u32 = 0x0809_ef84;
const GRADIENT_ADDRESS: u32 = 0x0809_f840;
const GRADIENT_END: u32 = 0x0809_fbc0;
const PALETTE_GRAPHICS_ADDRESS: u32 = 0x0809_ff58;
const PALETTE_GRAPHICS_END: u32 = 0x080a_0107;
const SPARSE_TABLE_ADDRESS: u32 = 0x080a_0138;
const SPARSE_TABLE_END: u32 = 0x080a_0288;

/// Every failure is a `throw new Error(...)` in the TypeScript, and the message
/// is what the parity run compares, so errors are plain strings.
pub type Res<T> = Result<T, String>;

fn err<T>(message: &str) -> Res<T> {
    Err(message.to_string())
}

type TableRow = (u32, u32, &'static [&'static str], &'static [&'static str]);

const RULE_LAYOUT: &[TableRow] = &[
    (0x0809_d170, 0x0809_d7a8, &["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], &["Func_0808ace0"]),
    (0x0809_d7a8, 0x0809_d8b0, &["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], &["Func_0808adf0"]),
    (0x0809_d8b0, 0x0809_d9f0, &["value:u16", "phase:u16"], &[]),
    (0x0809_d9f0, 0x0809_ddd8, &["selector_a:s16", "selector_b:s16", "condition:s16", "result:s16"], &["Func_0808b090"]),
    (0x0809_ddd8, 0x0809_e1d8, &["selector_a:s16", "selector_b:s16", "result:u32"], &["Func_0808b158"]),
    (0x0809_e1d8, 0x0809_e270, &["selector_a:s16", "selector_b:s16", "value_a:u16", "value_b:u16"], &["Func_0808b1d8"]),
    (0x0809_e270, 0x0809_e488, &["value:u32"], &["Func_0808b25c"]),
    (0x0809_e488, 0x0809_e4ce, &["value:u16"], &["Func_0808b320"]),
];

const TAIL_TABLE_LAYOUT: &[TableRow] = &[
    (0x0809_e680, 0x0809_e686, &["phase:u8"], &["Func_0808d9a4"]),
    (0x0809_e6b8, 0x0809_e6c0, &["phase:u8"], &["Func_0808e0b0"]),
    (0x0809_e8ac, 0x0809_e8ce, &["intensity:u8"], &["Func_0808f52c"]),
    (0x0809_e8ce, 0x0809_e8ee, &["curve:u8"], &["Func_0808f52c"]),
    (0x0809_e8ee, 0x0809_e92e, &["order:u8"], &["Func_08090658"]),
    (0x0809_e92e, 0x0809_e96e, &["color:u16"], &["Func_08090a5c"]),
    (0x0809_e96e, 0x0809_e9ae, &["color:u16"], &["Func_08090a5c"]),
    (0x0809_e9ae, 0x0809_e9ee, &["color:u16"], &["Func_08090a5c"]),
    (0x0809_e9f0, 0x0809_ebfc, &["id:u16", "value:s16"], &["Func_08091560"]),
    (0x0809_ed80, 0x0809_ed84, &["phase:u16"], &["Func_08092980"]),
    (0x0809_ef84, 0x0809_f024, &["x:s16", "y:s16", "tile:u16", "attribute_a:u16", "attribute_b:u16"], &["Func_08094820"]),
    (0x0809_f024, 0x0809_f0b0, &["y_offset:s16", "tile_offset:u16"], &["Func_08094e7c"]),
    (0x0809_f0f8, 0x0809_f118, &["phase:u16"], &["Func_08097b54"]),
    (0x0809_f118, 0x0809_f11c, &["order:u8"], &["Func_08097c3c"]),
    (0x0809_f128, 0x0809_f12c, &["order:u8"], &[]),
    (0x0809_f160, 0x0809_f168, &["slot:s8"], &["Func_0809ad70"]),
    (0x0809_f168, 0x0809_f188, &["curve:u8"], &["Func_0809bcf8"]),
    (0x0809_f188, 0x0809_f1a8, &["offset:s16"], &["Func_0809bcf8"]),
    (
        0x0809_f1a8,
        0x0809_f810,
        &["resource_id:s16", "group:s8", "variant:s8", "effect_id:u16", "reserved:u16"],
        &["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"],
    ),
    (
        0x0809_f810,
        0x0809_f840,
        &["word_0:u32", "script:ptr", "word_2:u32", "word_3:u32", "word_4:u32", "word_5:u32"],
        &["FunctionHead_0808b674"],
    ),
    (0x0809_fbc0, 0x0809_fbd0, &["packed_config:u32"], &[]),
    (0x0809_fc28, 0x0809_fc2c, &["value:u8"], &["Func_08093304"]),
    (0x080a_0108, 0x080a_0128, &["palette_code:u32"], &["Func_08097384"]),
];

const OPCODE_LAYOUT: &[(u32, &str)] = &[
    (1, "Func_0800d654"), (0, "Func_0800d674"), (3, "Func_0800d9f0"), (3, "Func_0800da18"),
    (3, "Func_0800da40"), (0, "Func_0800da78"), (0, "Func_0800daa0"), (2, "Func_0800dac0"),
    (3, "Func_0800dd70"), (3, "Func_0800df04"), (0, "Func_0800dcdc"), (0, "Func_0800daf0"),
    (2, "Func_0800d710"), (1, "Func_0800d760"), (1, "Func_0800d780"), (1, "Func_0800d7b4"),
    (0, "Func_0800d7e8"), (1, "Func_0800d7f8"), (1, "Func_0800d820"), (1, "Func_0800d850"),
    (1, "Func_0800d880"), (2, "Func_0800e9a0"), (2, "Func_0800e9dc"), (2, "Func_0800ea18"),
    (0, "FunctionHead_0800ebec"), (0, "Func_0800f7f4"), (0, "Func_0800f2f8"), (0, "Func_0800d8e8"),
    (1, "Func_0800d8c4"), (1, "Func_0800d8f4"), (1, "Func_0800d900"), (0, "Func_0800ca2c"),
    (0, "Func_0800ca44"), (0, "Func_0800ca58"), (1, "Func_0800d6a4"), (0, "Func_0800f7dc"),
    (0, "Func_0800d7e8"), (0, "Func_0800d7e8"), (0, "Func_0800d7e8"), (0, "Func_0800d7e8"),
];

const SCRIPT_LAYOUT: &[(u32, u32, &[u32])] = &[
    (0x0809_e6c0, 0x0809_e75c, &[0x0809_e6c0]),
    (0x0809_e75c, 0x0809_e87c, &[0x0809_e75c]),
    (0x0809_e87c, 0x0809_e8a0, &[0x0809_e87c]),
    (0x0809_e8a0, 0x0809_e8ac, &[0x0809_e8a0]),
    (0x0809_ebfc, 0x0809_ed80, &[0x0809_ebfc, 0x0809_ec7c, 0x0809_ecfc]),
    (0x0809_f0b0, 0x0809_f0b4, &[0x0809_f0b0]),
    (0x0809_f0b4, 0x0809_f0bc, &[0x0809_f0b4]),
    (0x0809_f0bc, 0x0809_f0f8, &[0x0809_f0bc, 0x0809_f0d4]),
    (0x0809_f11c, 0x0809_f128, &[0x0809_f11c]),
    (0x0809_f12c, 0x0809_f13c, &[0x0809_f12c]),
    (0x0809_f13c, 0x0809_f148, &[0x0809_f13c]),
    (0x0809_f148, 0x0809_f154, &[0x0809_f148]),
    (0x0809_f154, 0x0809_f160, &[0x0809_f154]),
    (0x0809_f80c, 0x0809_f810, &[0x0809_f80c]),
    (0x0809_fbcc, 0x0809_fbec, &[0x0809_fbcc]),
    (0x0809_fbec, 0x0809_fc04, &[0x0809_fbec]),
    (0x0809_fc04, 0x0809_fc1c, &[0x0809_fc04]),
    (0x0809_fc1c, 0x0809_fc28, &[0x0809_fc1c]),
    (0x0809_fc2c, 0x0809_fd38, &[0x0809_fc2c]),
    (0x0809_fd38, 0x0809_fd44, &[0x0809_fd38]),
    (0x0809_fd44, 0x0809_fe00, &[0x0809_fd44]),
    (0x0809_fe00, 0x0809_fe04, &[0x0809_fe00]),
    (0x0809_fe04, 0x0809_fe10, &[0x0809_fe04]),
    (0x0809_fe10, 0x0809_fecc, &[0x0809_fe10]),
    (0x0809_fecc, 0x0809_ff18, &[0x0809_fecc]),
    (0x0809_ff18, 0x0809_ff2c, &[0x0809_ff18]),
    (0x0809_ff2c, 0x0809_ff40, &[0x0809_ff2c]),
    (0x0809_ff40, 0x0809_ff58, &[0x0809_ff40]),
    (0x080a_0128, 0x080a_012c, &[0x080a_0128]),
];

/// PORT NOTE: a `Map<number, string>`, iterated only by `has`/`get`, so the
/// insertion order is not observable. It is still a slice and not a `HashMap`,
/// because nothing here needs hashing and the ordering trap is not worth
/// re-opening later.
const CALLBACK_SYMBOLS: &[(u32, &str)] = &[
    (0x0808_f305, "Func_0808f304"),
    (0x0809_2a75, "Func_08092a74"),
    (0x0809_3965, "Func_08093964"),
    (0x0809_397d, "Func_0809397c"),
    (0x0809_3a15, "Func_08093a14"),
    (0x0809_8c09, "Func_08098c08"),
];

// ---------------------------------------------------------------------------
// small helpers
// ---------------------------------------------------------------------------

fn hex(value: u32) -> String {
    format!("0x{value:08x}")
}

/// `exactKeys`: the sorted key list must match exactly.
///
/// PORT NOTE: JS sorts strings by UTF-16 code unit and Rust by byte. Every key
/// here is ASCII, where the two orders coincide.
fn exact_keys(value: &Value, keys: &[&str], label: &str) -> Res<()> {
    let mut actual = value.own_keys();
    actual.sort();
    let mut expected: Vec<String> = keys.iter().map(|key| (*key).to_string()).collect();
    expected.sort();
    if actual.join(",") != expected.join(",") {
        return Err(format!("{label} has unknown fields"));
    }
    Ok(())
}

/// `range`: a copy of `[start, end)` in ROM address space.
///
/// PORT NOTE: the TypeScript computes `start - ROM_BASE` in JS numbers, so an
/// address below the base goes negative and trips `first < 0`. The arithmetic
/// is done in `i64` here for the same reason.
fn range(source: &[u8], start: u32, end: u32) -> Res<Vec<u8>> {
    let first = i64::from(start) - i64::from(ROM_BASE);
    let last = i64::from(end) - i64::from(ROM_BASE);
    if first < 0 || last < first || last > source.len() as i64 {
        return err("ROM range differs");
    }
    Ok(source[first as usize..last as usize].to_vec())
}

fn read_u16(data: &[u8], offset: usize) -> u32 {
    u32::from(u16::from_le_bytes([data[offset], data[offset + 1]]))
}

fn read_u32(data: &[u8], offset: usize) -> u32 {
    u32::from_le_bytes([
        data[offset],
        data[offset + 1],
        data[offset + 2],
        data[offset + 3],
    ])
}

fn write_u16(output: &mut [u8], offset: usize, value: u16) {
    output[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}

fn write_u32(output: &mut [u8], offset: usize, value: u32) {
    output[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

/// `Number.isInteger`. Note that this is NOT `isSafeInteger`: the TypeScript
/// uses the unbounded form here, so 2**60 passes the integrality test and is
/// then caught (or not) by the explicit range.
fn is_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0
}

/// `integer(value, minimum, maximum, label)`.
///
/// PORT NOTE: written as `< minimum || > maximum` rather than the negated form
/// so a NaN falls through both comparisons exactly as it does in JS — although
/// `Number.isInteger` has already rejected NaN by then.
fn integer(value: Option<&Value>, minimum: f64, maximum: f64, label: &str) -> Res<f64> {
    let number = match value {
        Some(Value::Num(number)) => *number,
        _ => f64::NAN,
    };
    let integral = matches!(value, Some(Value::Num(_))) && is_integer(number);
    if !integral || number < minimum || number > maximum {
        return Err(format!("{label} is out of range"));
    }
    Ok(number)
}

fn array(value: Option<&Value>) -> Option<&Vec<Value>> {
    match value {
        Some(Value::Arr(items)) => Some(items),
        _ => None,
    }
}

// ---------------------------------------------------------------------------
// source validation
// ---------------------------------------------------------------------------

const SOURCE_KEYS: &[&str] = &[
    "format", "kind", "address", "end", "direct_graphics", "weighted_records", "typed_tables",
    "halfword_graphic", "tail_tables", "sentinel_lookup", "object_scripts", "sine_table",
    "gradients", "palette_graphics", "symbolic_pointers", "sparse_table", "zero_fills",
];

fn text_is(value: Option<&Value>, expected: &str) -> bool {
    matches!(value, Some(Value::Str(text)) if text == expected)
}

fn number_is(value: Option<&Value>, expected: f64) -> bool {
    matches!(value, Some(Value::Num(number)) if *number == expected)
}

/// `parse`: structural validation, returning the same value it was handed.
///
/// PORT NOTE: where the TypeScript would fall off a non-object into a JS
/// `TypeError` (for example `source.weighted_records.records.forEach` when
/// `records` is a string), this port raises a descriptive error instead. Bun's
/// `TypeError` text is not reproducible and no caller reads it; the explicit
/// `throw new Error(...)` messages, which are the ones the parity run compares,
/// are reproduced verbatim.
pub fn parse(value: &Value) -> Res<()> {
    if !matches!(value, Value::Obj(_) | Value::Arr(_)) {
        return err("battle-effect source must be an object");
    }
    exact_keys(value, SOURCE_KEYS, "battle-effect source")?;
    if !number_is(value.get("format"), 2.0)
        || !text_is(value.get("kind"), "golden-sun-battle-effect-data")
        || !text_is(value.get("address"), &hex(BATTLE_DATA_ADDRESS))
        || !text_is(value.get("end"), &hex(BATTLE_DATA_END))
    {
        return err("battle-effect source layout differs");
    }
    let direct = array(value.get("direct_graphics"));
    if direct.is_none_or(|items| items.len() != 2) {
        return err("direct-graphics collection differs");
    }
    for item in direct.unwrap() {
        exact_keys(item, &["address", "end", "source", "bpp", "columns", "role"], "direct graphic")?;
    }
    let weighted = value.get("weighted_records").ok_or("weighted-record source has unknown fields")?;
    exact_keys(
        weighted,
        &["address", "end", "stride", "fields", "consumers", "records"],
        "weighted-record source",
    )?;
    for record in array(weighted.get("records")).ok_or("weighted-record layout differs")? {
        exact_keys(record, &["base_result", "rank_origin", "results", "weights"], "weighted record")?;
    }
    let typed = array(value.get("typed_tables"));
    if typed.is_none_or(|items| items.len() != RULE_LAYOUT.len()) {
        return err("typed-table collection differs");
    }
    for table in typed.unwrap() {
        exact_keys(
            table,
            &["address", "end", "stride", "fields", "consumers", "records"],
            "typed table",
        )?;
    }
    let halfword = value.get("halfword_graphic").ok_or("halfword graphic has unknown fields")?;
    exact_keys(
        halfword,
        &["address", "end", "decoded_bytes", "source", "bpp", "columns", "codec", "tokens"],
        "halfword graphic",
    )?;
    let tail = array(value.get("tail_tables"));
    if tail.is_none_or(|items| items.len() != TAIL_TABLE_LAYOUT.len()) {
        return err("tail-table collection differs");
    }
    for table in tail.unwrap() {
        exact_keys(
            table,
            &["address", "end", "stride", "fields", "consumers", "records"],
            "tail table",
        )?;
    }
    let sentinel = value.get("sentinel_lookup").ok_or("sentinel lookup has unknown fields")?;
    exact_keys(
        sentinel,
        &["address", "end", "fields", "consumers", "records", "sentinel"],
        "sentinel lookup",
    )?;
    let scripts = array(value.get("object_scripts"));
    if scripts.is_none_or(|items| items.len() != SCRIPT_LAYOUT.len()) {
        return err("object-script collection differs");
    }
    for script in scripts.unwrap() {
        exact_keys(script, &["address", "end", "entry_points", "commands"], "object script")?;
        for entry in array(script.get("entry_points")).ok_or("object-script layout differs")? {
            exact_keys(entry, &["name", "address"], "script entry point")?;
        }
        for command in array(script.get("commands")).ok_or("object-script layout differs")? {
            if command.get("skip_word").is_some() {
                exact_keys(command, &["skip_word"], "script skip word")?;
            } else {
                exact_keys(command, &["opcode", "handler", "arguments"], "script command")?;
            }
        }
    }
    exact_keys(
        value.get("sine_table").ok_or("sine table has unknown fields")?,
        &["address", "end", "count", "type", "formula"],
        "sine table",
    )?;
    let gradients = value.get("gradients").ok_or("gradient source has unknown fields")?;
    exact_keys(gradients, &["address", "end", "stride", "consumers", "records"], "gradient source")?;
    for record in array(gradients.get("records")).ok_or("gradient layout differs")? {
        exact_keys(record, &["first_color", "second_color", "runs"], "gradient record")?;
        for run in array(record.get("runs")).ok_or("gradient run count differs")? {
            exact_keys(
                run,
                &["length", "color", "start_low", "end_low", "start_high", "end_high"],
                "gradient run",
            )?;
        }
    }
    let palettes = array(value.get("palette_graphics"));
    if palettes.is_none_or(|items| items.len() != 3) {
        return err("palette-graphic collection differs");
    }
    for graphic in palettes.unwrap() {
        exact_keys(
            graphic,
            &["address", "end", "decoded_bytes", "source", "bpp", "columns", "codec", "tokens"],
            "palette graphic",
        )?;
    }
    exact_keys(
        value.get("symbolic_pointers").ok_or("symbolic pointers has unknown fields")?,
        &["address", "end", "pointers"],
        "symbolic pointers",
    )?;
    let sparse = value.get("sparse_table").ok_or("sparse table has unknown fields")?;
    exact_keys(
        sparse,
        &["address", "end", "count", "fields", "consumers", "records", "sentinel"],
        "sparse table",
    )?;
    for record in array(sparse.get("records")).ok_or("sparse-table layout differs")? {
        exact_keys(record, &["index", "key", "value"], "sparse record")?;
    }
    let fills = array(value.get("zero_fills"));
    if fills.is_none_or(|items| items.len() != 3) {
        return err("zero-fill collection differs");
    }
    for fill in fills.unwrap() {
        exact_keys(fill, &["address", "end"], "zero fill")?;
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// graphics
// ---------------------------------------------------------------------------

/// `graphicPath`: the tracked tree is flat, so the package segments collapse
/// into the file name. The guard is the anchored `[a-z0-9_.-]+\.png` regex,
/// which is what keeps a `../` out of the joined path.
fn graphic_path(root: &Path, source: &str) -> Res<PathBuf> {
    const PREFIX: &str = "graphics/battle/effects/";
    let accepted = match source.strip_prefix(PREFIX) {
        Some(rest) => {
            rest.len() > 4
                && rest.ends_with(".png")
                && rest
                    .chars()
                    .all(|ch| ch.is_ascii_lowercase() || ch.is_ascii_digit() || "_.-".contains(ch))
        }
        None => false,
    };
    if !accepted {
        return err("battle-effect graphic path differs");
    }
    Ok(root.join(format!(
        "graphics/battle_effects_{}",
        &source[PREFIX.len()..]
    )))
}

fn read_file(path: &Path) -> Res<Vec<u8>> {
    std::fs::read(path).map_err(|error| format!("ENOENT: {}: {error}", path.display()))
}

fn build_direct_graphics(source: &Value, root: &Path) -> Res<Vec<u8>> {
    let expected = [
        (BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 0x100),
        (BATTLE_DATA_ADDRESS + 0x100, DIRECT_GRAPHICS_END),
    ];
    let items = array(source.get("direct_graphics")).ok_or("direct-graphics collection differs")?;
    let mut output: Vec<u8> = Vec::new();
    for (index, item) in items.iter().enumerate() {
        let (start, end) = expected[index];
        if !text_is(item.get("address"), &hex(start))
            || !text_is(item.get("end"), &hex(end))
            || !number_is(item.get("bpp"), 4.0)
            || !number_is(item.get("columns"), 4.0)
            || !text_is(item.get("role"), "obj_tiles")
        {
            return err("direct-graphic layout differs");
        }
        let name = item.get("source").and_then(Value::as_str).unwrap_or("");
        let path = graphic_path(root, name)?;
        let (tiles, _, report) = gba_graphics(&read_file(&path)?, 4.0).map_err(|error| error.0)?;
        if report.get("width") != Some(32.0)
            || report.get("height") != Some(16.0)
            || tiles.len() as u32 != end - start
        {
            return err("direct-graphic dimensions differ");
        }
        output.extend_from_slice(&tiles);
    }
    Ok(output)
}

fn graphic_tiles(graphic: &Value, root: &Path) -> Res<Vec<u8>> {
    if !number_is(graphic.get("bpp"), 4.0) {
        return err("compressed graphic depth differs");
    }
    let name = graphic.get("source").and_then(Value::as_str).unwrap_or("");
    let path = graphic_path(root, name)?;
    let (tiles, _, report) = gba_graphics(&read_file(&path)?, 4.0).map_err(|error| error.0)?;
    let columns = graphic.get("columns").and_then(Value::as_num).unwrap_or(f64::NAN);
    let decoded = graphic.get("decoded_bytes").and_then(Value::as_num).unwrap_or(f64::NAN);
    if report.get("width") != Some(columns * 8.0) || tiles.len() as f64 != decoded {
        return err("compressed graphic dimensions differ");
    }
    Ok(tiles)
}

// ---------------------------------------------------------------------------
// weighted records
// ---------------------------------------------------------------------------

fn build_weighted_records(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("weighted_records").ok_or("weighted-record layout differs")?;
    let records = array(table.get("records"));
    let fields = Value::Arr(
        ["base_result:u16", "rank_origin:u16", "results:u16[8]", "weights:u8[8]"]
            .iter()
            .map(|text| Value::Str((*text).to_string()))
            .collect(),
    );
    let consumers = Value::Arr(
        ["Func_0808ae74", "Func_0808b05c"]
            .iter()
            .map(|text| Value::Str((*text).to_string()))
            .collect(),
    );
    if !text_is(table.get("address"), &hex(DIRECT_GRAPHICS_END))
        || !text_is(table.get("end"), &hex(WEIGHTED_RECORDS_END))
        || !number_is(table.get("stride"), 28.0)
        || table.get("fields").map(minified) != Some(minified(&fields))
        || table.get("consumers").map(minified) != Some(minified(&consumers))
        || records.is_none_or(|items| items.len() != 104)
    {
        return err("weighted-record layout differs");
    }
    let records = records.unwrap();
    let mut output = vec![0u8; records.len() * 28];
    for (index, record) in records.iter().enumerate() {
        let offset = index * 28;
        write_u16(
            &mut output,
            offset,
            integer(record.get("base_result"), 0.0, 65535.0, "base result")? as u16,
        );
        write_u16(
            &mut output,
            offset + 2,
            integer(record.get("rank_origin"), 0.0, 65535.0, "rank origin")? as u16,
        );
        let results = array(record.get("results"));
        let weights = array(record.get("weights"));
        if results.is_none_or(|items| items.len() != 8) || weights.is_none_or(|items| items.len() != 8)
        {
            return err("weighted-record array extent differs");
        }
        for (slot, value) in results.unwrap().iter().enumerate() {
            let value = integer(Some(value), 0.0, 65535.0, "weighted result")?;
            write_u16(&mut output, offset + 4 + slot * 2, value as u16);
        }
        for (slot, value) in weights.unwrap().iter().enumerate() {
            let value = integer(Some(value), 0.0, 255.0, "result weight")?;
            output[offset + 20 + slot] = value as u8;
        }
    }
    Ok(output)
}

// ---------------------------------------------------------------------------
// typed tables
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum FieldKind {
    S8,
    U8,
    S16,
    U16,
    S32,
    U32,
    Ptr,
}

/// `fieldKind`: the anchored `^[a-z][a-z0-9_]*:(kind)$` regex.
fn field_kind(field: &str) -> Res<FieldKind> {
    let (name, kind) = match field.split_once(':') {
        Some(parts) => parts,
        None => return err("typed-table field differs"),
    };
    let mut characters = name.chars();
    let named = match characters.next() {
        Some(first) if first.is_ascii_lowercase() => characters
            .all(|ch| ch.is_ascii_lowercase() || ch.is_ascii_digit() || ch == '_'),
        _ => false,
    };
    if !named {
        return err("typed-table field differs");
    }
    match kind {
        "s8" => Ok(FieldKind::S8),
        "u8" => Ok(FieldKind::U8),
        "s16" => Ok(FieldKind::S16),
        "u16" => Ok(FieldKind::U16),
        "s32" => Ok(FieldKind::S32),
        "u32" => Ok(FieldKind::U32),
        "ptr" => Ok(FieldKind::Ptr),
        _ => err("typed-table field differs"),
    }
}

fn field_size(kind: FieldKind) -> usize {
    match kind {
        FieldKind::S8 | FieldKind::U8 => 1,
        FieldKind::S16 | FieldKind::U16 => 2,
        FieldKind::S32 | FieldKind::U32 | FieldKind::Ptr => 4,
    }
}

fn write_field(output: &mut [u8], offset: usize, kind: FieldKind, value: Option<&Value>) -> Res<()> {
    match kind {
        FieldKind::S8 => {
            let number = integer(value, -128.0, 127.0, "signed byte table value")?;
            output[offset] = number as i64 as u8;
        }
        FieldKind::U8 => {
            output[offset] = integer(value, 0.0, 255.0, "byte table value")? as u8;
        }
        FieldKind::S16 => {
            let number = integer(value, -32768.0, 32767.0, "signed table value")?;
            write_u16(output, offset, number as i64 as u16);
        }
        FieldKind::U16 => {
            write_u16(output, offset, integer(value, 0.0, 65535.0, "table value")? as u16);
        }
        FieldKind::S32 => {
            let number = integer(value, -2147483648.0, 2147483647.0, "signed word table value")?;
            write_u32(output, offset, number as i64 as u32);
        }
        FieldKind::U32 => {
            let number = integer(value, 0.0, 4294967295.0, "word table value")?;
            write_u32(output, offset, number as u32);
        }
        FieldKind::Ptr => match value {
            Some(Value::Null) => write_u32(output, offset, 0),
            Some(Value::Str(name)) => match script_symbol(name) {
                Some(address) => write_u32(output, offset, address),
                None => return err("symbolic table pointer differs"),
            },
            _ => return err("symbolic table pointer differs"),
        },
    }
    Ok(())
}

fn read_field(input: &[u8], offset: usize, kind: FieldKind) -> Res<Value> {
    Ok(match kind {
        FieldKind::S8 => Value::Num(f64::from(input[offset] as i8)),
        FieldKind::U8 => Value::Num(f64::from(input[offset])),
        FieldKind::S16 => Value::Num(f64::from(read_u16(input, offset) as u16 as i16)),
        FieldKind::U16 => Value::Num(f64::from(read_u16(input, offset))),
        FieldKind::S32 => Value::Num(f64::from(read_u32(input, offset) as i32)),
        FieldKind::U32 => Value::Num(f64::from(read_u32(input, offset))),
        FieldKind::Ptr => {
            let address = read_u32(input, offset);
            if address == 0 {
                return Ok(Value::Null);
            }
            let symbol = script_name(address);
            if script_symbol(&symbol).is_none() {
                return err("exported table pointer lacks an object-script symbol");
            }
            Value::Str(symbol)
        }
    })
}

fn string_list(items: &[&str]) -> Value {
    Value::Arr(items.iter().map(|text| Value::Str((*text).to_string())).collect())
}

/// The body shared by `buildTypedTables` and `buildTailTables`. The TypeScript
/// keeps them as two near-identical functions whose only differences are the
/// layout table and the error label; both are parameters here.
fn build_table_collection(
    tables: &[Value],
    layout: &[TableRow],
    label: &str,
) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let mut built = Vec::with_capacity(tables.len());
    for (table_index, table) in tables.iter().enumerate() {
        let (start, end, fields, consumers) = layout[table_index];
        if !text_is(table.get("address"), &hex(start))
            || !text_is(table.get("end"), &hex(end))
            || table.get("fields").map(minified) != Some(minified(&string_list(fields)))
            || table.get("consumers").map(minified) != Some(minified(&string_list(consumers)))
        {
            return Err(format!("{label} layout differs"));
        }
        let declared = array(table.get("fields")).ok_or(format!("{label} layout differs"))?;
        let mut kinds = Vec::with_capacity(declared.len());
        for entry in declared {
            kinds.push(field_kind(entry.as_str().unwrap_or(""))?);
        }
        let stride: usize = kinds.iter().copied().map(field_size).sum();
        let records = array(table.get("records"));
        if !number_is(table.get("stride"), stride as f64)
            || records.is_none_or(|items| items.len() * stride != (end - start) as usize)
        {
            return Err(format!("{label} extent differs"));
        }
        let mut output = vec![0u8; (end - start) as usize];
        for (record_index, record) in records.unwrap().iter().enumerate() {
            let values = array(Some(record));
            if values.is_none_or(|items| items.len() != kinds.len()) {
                return Err(format!("{label} record extent differs"));
            }
            let mut offset = record_index * stride;
            for (field_index, value) in values.unwrap().iter().enumerate() {
                let kind = kinds[field_index];
                write_field(&mut output, offset, kind, Some(value))?;
                offset += field_size(kind);
            }
        }
        built.push((start, end, output));
    }
    Ok(built)
}

fn build_typed_tables(source: &Value) -> Res<Vec<u8>> {
    let tables = array(source.get("typed_tables")).ok_or("typed-table collection differs")?;
    let built = build_table_collection(tables, RULE_LAYOUT, "typed-table")?;
    Ok(built.into_iter().flat_map(|(_, _, bytes)| bytes).collect())
}

fn build_tail_tables(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let tables = array(source.get("tail_tables")).ok_or("tail-table collection differs")?;
    build_table_collection(tables, TAIL_TABLE_LAYOUT, "tail-table")
}

// ---------------------------------------------------------------------------
// halfword LZ codec
// ---------------------------------------------------------------------------

/// `["l", n]` / `["c", length, distance]` / `["e"]` from the TypeScript.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum HalfwordToken {
    Literal(u32),
    Copy { length: u32, distance: u32 },
    End,
}

fn append_halfword_copy(output: &mut Vec<u32>, distance: u32, length: u32) -> Res<()> {
    if distance < 1 || distance as usize > output.len() || !(2..=33).contains(&length) {
        return err("halfword copy differs");
    }
    for _ in 0..length {
        output.push(output[output.len() - distance as usize]);
    }
    Ok(())
}

pub fn decode_halfword_trace(encoded: &[u8]) -> Res<(Vec<u8>, Vec<HalfwordToken>)> {
    let mut output: Vec<u32> = Vec::new();
    let mut tokens: Vec<HalfwordToken> = Vec::new();
    let mut cursor = 0usize;
    let mut ended = false;
    while !ended {
        if cursor + 2 > encoded.len() {
            return err("halfword flags are truncated");
        }
        let flags = read_u16(encoded, cursor);
        cursor += 2;
        for bit in (0..16u32).rev() {
            if cursor + 2 > encoded.len() {
                return err("halfword token is truncated");
            }
            let token = read_u16(encoded, cursor);
            cursor += 2;
            if flags & (1 << bit) == 0 {
                output.push(token);
                match tokens.last_mut() {
                    Some(HalfwordToken::Literal(count)) => *count += 1,
                    _ => tokens.push(HalfwordToken::Literal(1)),
                }
            } else if token == 0 {
                tokens.push(HalfwordToken::End);
                ended = true;
                break;
            } else {
                let distance = token >> 5;
                let length = (token & 31) + 2;
                tokens.push(HalfwordToken::Copy { length, distance });
                append_halfword_copy(&mut output, distance, length)?;
            }
        }
    }
    if cursor != encoded.len() {
        return err("halfword stream has trailing bytes");
    }
    let mut decoded = vec![0u8; output.len() * 2];
    for (index, value) in output.iter().enumerate() {
        write_u16(&mut decoded, index * 2, *value as u16);
    }
    Ok((decoded, tokens))
}

/// The expansion of a token plan into per-halfword operations, exactly as the
/// TypeScript builds it: a literal run of `n` becomes `n` separate literals.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum HalfwordOperation {
    Literal,
    Copy { length: u32, distance: u32 },
    End,
}

pub fn encode_halfword(decoded: &[u8], tokens: &[HalfwordToken]) -> Res<Vec<u8>> {
    if !decoded.len().is_multiple_of(2) {
        return err("halfword pixels have an odd size");
    }
    let mut operations: Vec<HalfwordOperation> = Vec::new();
    for token in tokens {
        match *token {
            HalfwordToken::Literal(count) => {
                let count = integer(
                    Some(&Value::Num(f64::from(count))),
                    1.0,
                    decoded.len() as f64 / 2.0,
                    "halfword literal count",
                )? as u32;
                for _ in 0..count {
                    operations.push(HalfwordOperation::Literal);
                }
            }
            HalfwordToken::Copy { length, distance } => {
                let length =
                    integer(Some(&Value::Num(f64::from(length))), 2.0, 33.0, "halfword copy length")?
                        as u32;
                let distance = integer(
                    Some(&Value::Num(f64::from(distance))),
                    1.0,
                    2047.0,
                    "halfword copy distance",
                )? as u32;
                operations.push(HalfwordOperation::Copy { length, distance });
            }
            HalfwordToken::End => operations.push(HalfwordOperation::End),
        }
    }
    let mut encoded: Vec<u8> = Vec::new();
    let mut replay: Vec<u32> = Vec::new();
    let mut operation = 0usize;
    let mut cursor = 0usize;
    let mut ended = false;
    while operation < operations.len() {
        let mut flags: u32 = 0;
        let mut words: Vec<u32> = Vec::new();
        let mut index = 0u32;
        while index < 16 && operation < operations.len() {
            let token = operations[operation];
            // PORT NOTE: unreachable in practice, because the terminator check
            // below fires first for any plan with trailing operations. Kept so
            // the two implementations have the same shape.
            if ended {
                return err("halfword plan has data after terminator");
            }
            match token {
                HalfwordOperation::Literal => {
                    if cursor + 2 > decoded.len() {
                        return err("halfword literal crossed decoded pixels");
                    }
                    let value = read_u16(decoded, cursor);
                    cursor += 2;
                    replay.push(value);
                    words.push(value);
                }
                HalfwordOperation::Copy { length, distance } => {
                    flags |= 1 << (15 - index);
                    if distance as usize > replay.len() {
                        return err("halfword copy crossed replay prefix");
                    }
                    words.push((distance << 5) | (length - 2));
                    append_halfword_copy(&mut replay, distance, length)?;
                    cursor += length as usize * 2;
                }
                HalfwordOperation::End => {
                    flags |= 1 << (15 - index);
                    words.push(0);
                    ended = true;
                    if operation + 1 != operations.len() {
                        return err("halfword terminator is not final");
                    }
                }
            }
            index += 1;
            operation += 1;
        }
        let mut group = vec![0u8; 2 + words.len() * 2];
        write_u16(&mut group, 0, flags as u16);
        for (slot, value) in words.iter().enumerate() {
            write_u16(&mut group, 2 + slot * 2, *value as u16);
        }
        encoded.extend_from_slice(&group);
    }
    let mut replay_bytes = vec![0u8; replay.len() * 2];
    for (index, value) in replay.iter().enumerate() {
        write_u16(&mut replay_bytes, index * 2, *value as u16);
    }
    if !ended || cursor != decoded.len() || replay_bytes != decoded {
        return err("halfword plan does not reconstruct decoded pixels");
    }
    Ok(encoded)
}

fn halfword_token_value(token: &HalfwordToken) -> Value {
    match *token {
        HalfwordToken::Literal(count) => Value::Arr(vec![
            Value::Str("l".to_string()),
            Value::Num(f64::from(count)),
        ]),
        HalfwordToken::Copy { length, distance } => Value::Arr(vec![
            Value::Str("c".to_string()),
            Value::Num(f64::from(length)),
            Value::Num(f64::from(distance)),
        ]),
        HalfwordToken::End => Value::Arr(vec![Value::Str("e".to_string())]),
    }
}

/// PORT NOTE: the TypeScript reads `token[1]`/`token[2]` through `integer(...)`,
/// which demands `Number.isInteger`, so a fractional or non-numeric field is
/// rejected there too; the range bounds are re-checked inside `encode_halfword`
/// so the error labels stay in the same order as the TypeScript's.
fn halfword_token_from(value: &Value) -> Res<HalfwordToken> {
    let items = match value {
        Value::Arr(items) if !items.is_empty() => items,
        _ => return err("halfword plan token differs"),
    };
    match items[0].as_str() {
        Some("l") => {
            if items.len() != 2 {
                return err("halfword literal token differs");
            }
            // The bound is re-applied against the real pixel count later; this
            // only proves the field is an integer at all.
            let count = integer(items.get(1), 1.0, f64::INFINITY, "halfword literal count")?;
            Ok(HalfwordToken::Literal(count as u32))
        }
        Some("c") => {
            if items.len() != 3 {
                return err("halfword copy token differs");
            }
            let length = integer(items.get(1), 2.0, 33.0, "halfword copy length")?;
            let distance = integer(items.get(2), 1.0, 2047.0, "halfword copy distance")?;
            Ok(HalfwordToken::Copy {
                length: length as u32,
                distance: distance as u32,
            })
        }
        Some("e") if items.len() == 1 => Ok(HalfwordToken::End),
        _ => err("halfword plan token differs"),
    }
}

fn build_halfword_graphic(source: &Value, root: &Path) -> Res<Vec<u8>> {
    let graphic = source.get("halfword_graphic").ok_or("halfword graphic layout differs")?;
    if !text_is(graphic.get("address"), &hex(RULE_TABLES_END))
        || !text_is(graphic.get("end"), &hex(HALFWORD_GRAPHIC_END))
        || !number_is(graphic.get("decoded_bytes"), 1536.0)
        || !number_is(graphic.get("columns"), 12.0)
        || !text_is(graphic.get("codec"), "halfword-lz")
    {
        return err("halfword graphic layout differs");
    }
    let tiles = graphic_tiles(graphic, root)?;
    let mut tokens = Vec::new();
    for token in array(graphic.get("tokens")).ok_or("halfword plan token differs")? {
        tokens.push(halfword_token_from(token)?);
    }
    let encoded = encode_halfword(&tiles, &tokens)?;
    if encoded.len() as u32 != HALFWORD_GRAPHIC_END - RULE_TABLES_END {
        return err("halfword graphic encoded size differs");
    }
    Ok(encoded)
}

// ---------------------------------------------------------------------------
// palette LZ graphics
// ---------------------------------------------------------------------------

fn palette_operation_value(operation: &PaletteOperation) -> Value {
    match *operation {
        PaletteOperation::Literal => Value::Arr(vec![Value::Str("l".to_string())]),
        PaletteOperation::End => Value::Arr(vec![Value::Str("e".to_string())]),
        PaletteOperation::Copy { length, distance } => Value::Arr(vec![
            Value::Str("c".to_string()),
            Value::Num(f64::from(length)),
            Value::Num(f64::from(distance)),
        ]),
    }
}

fn palette_group_value(group: &PaletteGroup) -> Value {
    match group {
        PaletteGroup::Zeros => Value::Arr(vec![Value::Str("z".to_string())]),
        PaletteGroup::Group(operations) => Value::Arr(vec![
            Value::Str("g".to_string()),
            Value::Arr(operations.iter().map(palette_operation_value).collect()),
        ]),
    }
}

/// PORT NOTE: `encode_palette` in the TypeScript coerces `token[1]`/`token[2]`
/// with `Number(...)`, which accepts strings ("0x10" is 16) and fractions. The
/// Rust `extract_resource::encode_palette` takes `u32` fields, so this
/// converter applies `Number(...)` (hence `to_number`, keeping the hex-string
/// behaviour) and then refuses a non-integral result rather than letting it
/// silently truncate through `|`. No tracked document contains such a token;
/// this is the one deliberate narrowing in the port and it fails loudly.
fn palette_number(value: Option<&Value>) -> f64 {
    match value {
        Some(Value::Num(number)) => *number,
        Some(Value::Str(text)) => to_number(text),
        Some(Value::Bool(flag)) => f64::from(u8::from(*flag)),
        Some(Value::Null) => 0.0,
        _ => f64::NAN,
    }
}

fn palette_group_from(value: &Value) -> Res<PaletteGroup> {
    let items = match value {
        Value::Arr(items) => items,
        _ => return err("invalid palette token group"),
    };
    if items.first().and_then(Value::as_str) == Some("z") {
        return Ok(PaletteGroup::Zeros);
    }
    let inner = match (items.len(), items.first().and_then(Value::as_str)) {
        (2, Some("g")) => array(items.get(1)).ok_or("invalid palette token group")?,
        _ => return err("invalid palette token group"),
    };
    if inner.is_empty() {
        return err("invalid palette token group");
    }
    let mut operations = Vec::with_capacity(inner.len());
    for token in inner {
        let parts = match token {
            Value::Arr(parts) if !parts.is_empty() => parts,
            _ => return err("invalid palette copy token"),
        };
        match parts[0].as_str() {
            Some("l") => operations.push(PaletteOperation::Literal),
            Some("e") => operations.push(PaletteOperation::End),
            _ => {
                if parts.len() != 3 || parts[0].as_str() != Some("c") {
                    return err("invalid palette copy token");
                }
                let length = palette_number(parts.get(1));
                let distance = palette_number(parts.get(2));
                if !is_integer(length) || !is_integer(distance) || length < 0.0 || distance < 0.0 {
                    return err("palette copy length is invalid");
                }
                operations.push(PaletteOperation::Copy {
                    length: length as u32,
                    distance: distance as u32,
                });
            }
        }
    }
    Ok(PaletteGroup::Group(operations))
}

fn build_palette_graphics(source: &Value, root: &Path) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let expected: [(u32, u32, f64); 3] = [
        (0x0809_ff58, 0x080a_001e, 768.0),
        (0x080a_001e, 0x080a_00b8, 768.0),
        (0x080a_00b8, 0x080a_0107, 512.0),
    ];
    let graphics = array(source.get("palette_graphics")).ok_or("palette-graphic collection differs")?;
    let mut built = Vec::with_capacity(graphics.len());
    for (index, graphic) in graphics.iter().enumerate() {
        let (start, end, decoded_bytes) = expected[index];
        if !text_is(graphic.get("address"), &hex(start))
            || !text_is(graphic.get("end"), &hex(end))
            || !number_is(graphic.get("decoded_bytes"), decoded_bytes)
            || !number_is(graphic.get("columns"), 2.0)
            || !text_is(graphic.get("codec"), "palette-lz")
        {
            return err("palette graphic layout differs");
        }
        let tiles = graphic_tiles(graphic, root)?;
        let mut groups = Vec::new();
        for token in array(graphic.get("tokens")).ok_or("invalid palette token group")? {
            groups.push(palette_group_from(token)?);
        }
        let encoded = encode_palette(&tiles, &groups).map_err(|error| error.0)?;
        if encoded.len() as u32 != end - start {
            return err("palette graphic encoded size differs");
        }
        built.push((start, end, encoded));
    }
    Ok(built)
}

// ---------------------------------------------------------------------------
// the remaining regions
// ---------------------------------------------------------------------------

fn build_sentinel_lookup(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("sentinel_lookup").ok_or("sentinel lookup layout differs")?;
    let records = array(table.get("records"));
    if !text_is(table.get("address"), &hex(0x0809_e686))
        || !text_is(table.get("end"), &hex(0x0809_e6b8))
        || table.get("fields").map(minified) != Some(minified(&string_list(&["key:s16", "value:s16"])))
        || table.get("consumers").map(minified) != Some(minified(&string_list(&["Func_0808ddb8"])))
        || records.is_none_or(|items| items.len() != 12)
        || !number_is(table.get("sentinel"), -1.0)
    {
        return err("sentinel lookup layout differs");
    }
    let mut output = vec![0u8; 50];
    for (index, record) in records.unwrap().iter().enumerate() {
        let values = array(Some(record));
        if values.is_none_or(|items| items.len() != 2) {
            return err("sentinel lookup record differs");
        }
        let values = values.unwrap();
        let key = integer(values.first(), -32768.0, 32767.0, "lookup key")?;
        let value = integer(values.get(1), -32768.0, 32767.0, "lookup value")?;
        write_u16(&mut output, index * 4, key as i64 as u16);
        write_u16(&mut output, index * 4 + 2, value as i64 as u16);
    }
    write_u16(&mut output, 48, 0xffff);
    Ok(output)
}

fn script_name(address: u32) -> String {
    format!("ObjectScript_{address:08x}")
}

/// `scriptSymbols().get(name)`: the object-script entry points plus the six
/// callback symbols. Rebuilt per lookup in the TypeScript; a linear scan over
/// the two static tables is the same thing without the allocation.
fn script_symbol(name: &str) -> Option<u32> {
    for (_, _, entries) in SCRIPT_LAYOUT {
        for address in *entries {
            if script_name(*address) == name {
                return Some(*address);
            }
        }
    }
    CALLBACK_SYMBOLS
        .iter()
        .find(|(_, symbol)| *symbol == name)
        .map(|(address, _)| *address)
}

fn script_word(value: Option<&Value>) -> Res<u32> {
    if let Some(Value::Str(name)) = value {
        return match script_symbol(name) {
            Some(address) => Ok(address),
            None => Err(format!("unknown script symbol: {name}")),
        };
    }
    let number = integer(value, -2147483648.0, 4294967295.0, "script word")?;
    // `>>> 0` is ToUint32; the range check above keeps this inside one wrap.
    Ok(number as i64 as u32)
}

/// `opcodeNumber`: the anchored `^op_([0-9]+)$` regex, then `Number(digits)`.
fn opcode_number(name: &str) -> Res<usize> {
    let digits = match name.strip_prefix("op_") {
        Some(digits) if !digits.is_empty() && digits.bytes().all(|byte| byte.is_ascii_digit()) => {
            digits
        }
        _ => return err("object-script opcode differs"),
    };
    let value = to_number(digits);
    let value = integer(
        Some(&Value::Num(value)),
        0.0,
        (OPCODE_LAYOUT.len() - 1) as f64,
        "object-script opcode",
    )?;
    Ok(value as usize)
}

fn build_object_scripts(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let scripts = array(source.get("object_scripts")).ok_or("object-script collection differs")?;
    let mut built = Vec::with_capacity(scripts.len());
    for (index, script) in scripts.iter().enumerate() {
        let (start, end, entries) = SCRIPT_LAYOUT[index];
        let entry_points = array(script.get("entry_points")).ok_or("object-script layout differs")?;
        if !text_is(script.get("address"), &hex(start))
            || !text_is(script.get("end"), &hex(end))
            || entry_points.len() != entries.len()
        {
            return err("object-script layout differs");
        }
        let mut chunks: Vec<u8> = Vec::new();
        // PORT NOTE: a `Set<number>` of addresses, insertion-ordered and only
        // ever queried with `has`, so a sorted `Vec` would also do; this stays a
        // `Vec` because JS `Set` membership is by SameValueZero over one number
        // type and every address here is already a `u32`.
        let mut boundaries: Vec<u32> = Vec::new();
        let mut size = 0u32;
        for command in array(script.get("commands")).ok_or("object-script layout differs")? {
            boundaries.push(start + size);
            if let Some(word) = command.get("skip_word") {
                let word = script_word(Some(word))?;
                if word <= 63 {
                    return err("object-script skip word is dispatchable");
                }
                chunks.extend_from_slice(&word.to_le_bytes());
                size += 4;
                continue;
            }
            let opcode = opcode_number(command.get("opcode").and_then(Value::as_str).unwrap_or(""))?;
            let (arity, handler) = OPCODE_LAYOUT[opcode];
            let arguments = array(command.get("arguments"));
            if !text_is(command.get("handler"), handler)
                || arguments.is_none_or(|items| items.len() as u32 != arity)
            {
                return err("object-script command layout differs");
            }
            let mut output = vec![0u8; (1 + arity as usize) * 4];
            write_u32(&mut output, 0, opcode as u32);
            for (slot, argument) in arguments.unwrap().iter().enumerate() {
                write_u32(&mut output, 4 + slot * 4, script_word(Some(argument))?);
            }
            size += output.len() as u32;
            chunks.extend_from_slice(&output);
        }
        if start + size != end {
            return err("object-script extent differs");
        }
        for (entry_index, entry) in entry_points.iter().enumerate() {
            let expected = entries[entry_index];
            if !text_is(entry.get("name"), &script_name(expected))
                || !text_is(entry.get("address"), &hex(expected))
                || !boundaries.contains(&expected)
            {
                return err("object-script entry point differs");
            }
        }
        built.push((start, end, chunks));
    }
    Ok(built)
}

fn build_sine_table(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("sine_table").ok_or("sine-table formula differs")?;
    if !text_is(table.get("address"), &hex(SINE_TABLE_ADDRESS))
        || !text_is(table.get("end"), &hex(SINE_TABLE_END))
        || !number_is(table.get("count"), 256.0)
        || !text_is(table.get("type"), "s16")
        || !text_is(table.get("formula"), "trunc(sin(index*pi/128)*0x1000)")
    {
        return err("sine-table formula differs");
    }
    let mut output = vec![0u8; 512];
    for index in 0..256u32 {
        let value = (f64::from(index) * std::f64::consts::PI / 128.0).sin() * 4096.0;
        write_u16(&mut output, index as usize * 2, value.trunc() as i64 as u16);
    }
    Ok(output)
}

fn build_gradients(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("gradients").ok_or("gradient layout differs")?;
    let records = array(table.get("records"));
    if !text_is(table.get("address"), &hex(GRADIENT_ADDRESS))
        || !text_is(table.get("end"), &hex(GRADIENT_END))
        || !number_is(table.get("stride"), 28.0)
        || table.get("consumers").map(minified) != Some(minified(&string_list(&["Func_0808f52c"])))
        || records.is_none_or(|items| items.len() != 32)
    {
        return err("gradient layout differs");
    }
    let mut output = vec![0u8; (GRADIENT_END - GRADIENT_ADDRESS) as usize];
    for (record_index, record) in records.unwrap().iter().enumerate() {
        let runs = array(record.get("runs")).ok_or("gradient run count differs")?;
        if runs.len() != 3 {
            return err("gradient run count differs");
        }
        let offset = record_index * 28;
        let first = integer(record.get("first_color"), 0.0, 65535.0, "gradient first color")?;
        let second = integer(record.get("second_color"), 0.0, 65535.0, "gradient second color")?;
        write_u16(&mut output, offset, first as u16);
        write_u16(&mut output, offset + 2, second as u16);
        for (run_index, run) in runs.iter().enumerate() {
            let run_offset = offset + 4 + run_index * 8;
            let length = integer(run.get("length"), 0.0, 65535.0, "gradient run length")?;
            let color = integer(run.get("color"), 0.0, 65535.0, "gradient color")?;
            write_u16(&mut output, run_offset, length as u16);
            write_u16(&mut output, run_offset + 2, color as u16);
            output[run_offset + 4] = integer(run.get("start_low"), 0.0, 255.0, "gradient low start")? as u8;
            output[run_offset + 5] = integer(run.get("end_low"), 0.0, 255.0, "gradient low end")? as u8;
            output[run_offset + 6] = integer(run.get("start_high"), 0.0, 255.0, "gradient high start")? as u8;
            output[run_offset + 7] = integer(run.get("end_high"), 0.0, 255.0, "gradient high end")? as u8;
        }
    }
    Ok(output)
}

fn build_symbolic_pointers(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("symbolic_pointers").ok_or("symbolic pointer table differs")?;
    let expected = Value::Arr(
        [0x0809_f13cu32, 0x0809_f148, 0x0809_f154]
            .iter()
            .map(|address| Value::Str(script_name(*address)))
            .collect(),
    );
    if !text_is(table.get("address"), &hex(0x080a_012c))
        || !text_is(table.get("end"), &hex(0x080a_0138))
        || table.get("pointers").map(minified) != Some(minified(&expected))
    {
        return err("symbolic pointer table differs");
    }
    let mut output = vec![0u8; 12];
    for (index, name) in array(table.get("pointers")).unwrap().iter().enumerate() {
        let address = name.as_str().and_then(script_symbol).unwrap_or(0);
        write_u32(&mut output, index * 4, address);
    }
    Ok(output)
}

fn build_sparse_table(source: &Value) -> Res<Vec<u8>> {
    let table = source.get("sparse_table").ok_or("sparse-table layout differs")?;
    let sentinel = Value::Arr(vec![Value::Num(-1.0), Value::Num(0.0)]);
    if !text_is(table.get("address"), &hex(SPARSE_TABLE_ADDRESS))
        || !text_is(table.get("end"), &hex(SPARSE_TABLE_END))
        || !number_is(table.get("count"), 41.0)
        || table.get("fields").map(minified) != Some(minified(&string_list(&["key:s32", "value:s32"])))
        || table.get("consumers").map(minified) != Some(minified(&string_list(&["Func_0809bcf8"])))
        || table.get("sentinel").map(minified) != Some(minified(&sentinel))
    {
        return err("sparse-table layout differs");
    }
    let mut output = vec![0u8; (SPARSE_TABLE_END - SPARSE_TABLE_ADDRESS) as usize];
    let mut indices: Vec<u32> = Vec::new();
    for record in array(table.get("records")).ok_or("sparse-table layout differs")? {
        let index = integer(record.get("index"), 0.0, 40.0, "sparse-table index")? as u32;
        if indices.contains(&index) {
            return err("sparse-table index is duplicated");
        }
        indices.push(index);
        let key = integer(record.get("key"), -2147483648.0, 2147483647.0, "sparse-table key")?;
        let value = integer(record.get("value"), -2147483648.0, 2147483647.0, "sparse-table value")?;
        write_u32(&mut output, index as usize * 8, key as i64 as u32);
        write_u32(&mut output, index as usize * 8 + 4, value as i64 as u32);
    }
    write_u32(&mut output, 41 * 8, 0xffff_ffff);
    Ok(output)
}

fn build_zero_fills(source: &Value) -> Res<Vec<(u32, u32, Vec<u8>)>> {
    let expected = [
        (0x0809_e9eeu32, 0x0809_e9f0u32),
        (0x080a_0107, 0x080a_0108),
        (0x080a_0288, BATTLE_DATA_END),
    ];
    let fills = array(source.get("zero_fills")).ok_or("zero-fill collection differs")?;
    let mut built = Vec::with_capacity(fills.len());
    for (index, fill) in fills.iter().enumerate() {
        let (start, end) = expected[index];
        if !text_is(fill.get("address"), &hex(start)) || !text_is(fill.get("end"), &hex(end)) {
            return err("zero-fill layout differs");
        }
        built.push((start, end, vec![0u8; (end - start) as usize]));
    }
    Ok(built)
}

/// Paint the segments into the tail span, refusing an inconsistent overlap and
/// any byte no segment claimed.
fn assemble_tail(segments: &[(u32, u32, Vec<u8>)]) -> Res<Vec<u8>> {
    let span = (BATTLE_DATA_END - RULE_TABLES_END) as usize;
    let mut output = vec![0u8; span];
    let mut written = vec![0u8; span];
    for (start, end, bytes) in segments {
        if *start < RULE_TABLES_END
            || *end > BATTLE_DATA_END
            || start >= end
            || bytes.len() as u32 != end - start
        {
            return err("battle-effect tail segment differs");
        }
        let offset = (start - RULE_TABLES_END) as usize;
        for (index, byte) in bytes.iter().enumerate() {
            if written[offset + index] != 0 && output[offset + index] != *byte {
                return Err(format!(
                    "battle-effect overlap differs at {}",
                    hex(start + index as u32)
                ));
            }
            output[offset + index] = *byte;
            written[offset + index] = 1;
        }
    }
    if let Some(hole) = written.iter().position(|flag| *flag == 0) {
        return Err(format!(
            "battle-effect tail has a hole at {}",
            hex(RULE_TABLES_END + hole as u32)
        ));
    }
    Ok(output)
}

// ---------------------------------------------------------------------------
// build
// ---------------------------------------------------------------------------

pub fn build_battle_effect_data(value: &Value, root: &Path) -> Res<Vec<u8>> {
    parse(value)?;
    let mut prefix = build_direct_graphics(value, root)?;
    prefix.extend_from_slice(&build_weighted_records(value)?);
    prefix.extend_from_slice(&build_typed_tables(value)?);
    if prefix.len() as u32 != RULE_TABLES_END - BATTLE_DATA_ADDRESS {
        return err("battle-effect table prefix extent differs");
    }
    let mut segments: Vec<(u32, u32, Vec<u8>)> = vec![(
        RULE_TABLES_END,
        HALFWORD_GRAPHIC_END,
        build_halfword_graphic(value, root)?,
    )];
    segments.extend(build_tail_tables(value)?);
    segments.push((0x0809_e686, 0x0809_e6b8, build_sentinel_lookup(value)?));
    segments.extend(build_object_scripts(value)?);
    segments.push((SINE_TABLE_ADDRESS, SINE_TABLE_END, build_sine_table(value)?));
    segments.push((GRADIENT_ADDRESS, GRADIENT_END, build_gradients(value)?));
    segments.extend(build_palette_graphics(value, root)?);
    segments.push((0x080a_012c, 0x080a_0138, build_symbolic_pointers(value)?));
    segments.push((
        SPARSE_TABLE_ADDRESS,
        SPARSE_TABLE_END,
        build_sparse_table(value)?,
    ));
    segments.extend(build_zero_fills(value)?);
    let mut output = prefix;
    output.extend_from_slice(&assemble_tail(&segments)?);
    if output.len() as u32 != BATTLE_DATA_END - BATTLE_DATA_ADDRESS {
        return err("battle-effect source extent differs");
    }
    Ok(output)
}

// ---------------------------------------------------------------------------
// export
// ---------------------------------------------------------------------------

fn export_table_collection(rom: &[u8], layout: &[TableRow]) -> Res<Value> {
    let mut tables = Vec::with_capacity(layout.len());
    for (start, end, fields, consumers) in layout {
        let raw = range(rom, *start, *end)?;
        let mut kinds = Vec::with_capacity(fields.len());
        for entry in *fields {
            kinds.push(field_kind(entry)?);
        }
        let stride: usize = kinds.iter().copied().map(field_size).sum();
        if raw.len() % stride != 0 {
            return err("exported typed-table extent differs");
        }
        let mut records = Vec::new();
        let mut record_offset = 0usize;
        while record_offset < raw.len() {
            let mut offset = record_offset;
            let mut record = Vec::with_capacity(kinds.len());
            for kind in &kinds {
                record.push(read_field(&raw, offset, *kind)?);
                offset += field_size(*kind);
            }
            records.push(Value::Arr(record));
            record_offset += stride;
        }
        tables.push(Value::obj(vec![
            ("address", Value::Str(hex(*start))),
            ("end", Value::Str(hex(*end))),
            ("stride", Value::Num(stride as f64)),
            ("fields", string_list(fields)),
            ("consumers", string_list(consumers)),
            ("records", Value::Arr(records)),
        ]));
    }
    Ok(Value::Arr(tables))
}

/// PORT NOTE (real divergence in the TypeScript, reproduced deliberately):
/// `exportHalfwordGraphic` and `exportPaletteGraphics` write their PNGs to
/// `<root>/graphics/battle/effects/<name>.png`, but `graphicPath` — which is
/// what `build` reads back, including the round-trip check at the end of
/// `export` — resolves the same logical name to the FLAT
/// `<root>/graphics/battle_effects_<name>.png`. So export writes into a nested
/// directory nothing ever reads, and validates itself against the flat files
/// that were already tracked. Reproduced exactly: both paths are written and
/// read as the TypeScript does.
fn export_halfword_graphic(rom: &[u8], root: &Path) -> Res<Value> {
    let encoded = range(rom, RULE_TABLES_END, HALFWORD_GRAPHIC_END)?;
    let (decoded, tokens) = decode_halfword_trace(&encoded)?;
    if decoded.len() != 0x600 {
        return err("halfword graphic decoded size differs");
    }
    let name = "battle_glyphs.4bpp.png";
    let image = tile_png(&decoded, 4.0, 12, None).map_err(|error| error.0)?.0;
    write_file(&root.join(format!("graphics/battle/effects/{name}")), &image)?;
    Ok(Value::obj(vec![
        ("address", Value::Str(hex(RULE_TABLES_END))),
        ("end", Value::Str(hex(HALFWORD_GRAPHIC_END))),
        ("decoded_bytes", Value::Num(decoded.len() as f64)),
        ("source", Value::Str(format!("graphics/battle/effects/{name}"))),
        ("bpp", Value::Num(4.0)),
        ("columns", Value::Num(12.0)),
        ("codec", Value::Str("halfword-lz".to_string())),
        ("tokens", Value::Arr(tokens.iter().map(halfword_token_value).collect())),
    ]))
}

fn export_sentinel_lookup(rom: &[u8]) -> Res<Value> {
    let raw = range(rom, 0x0809_e686, 0x0809_e6b8)?;
    let records: Vec<Value> = (0..12)
        .map(|index| {
            Value::Arr(vec![
                Value::Num(f64::from(read_u16(&raw, index * 4) as u16 as i16)),
                Value::Num(f64::from(read_u16(&raw, index * 4 + 2) as u16 as i16)),
            ])
        })
        .collect();
    Ok(Value::obj(vec![
        ("address", Value::Str(hex(0x0809_e686))),
        ("end", Value::Str(hex(0x0809_e6b8))),
        ("fields", string_list(&["key:s16", "value:s16"])),
        ("consumers", string_list(&["Func_0808ddb8"])),
        ("records", Value::Arr(records)),
        ("sentinel", Value::Num(f64::from(read_u16(&raw, 48) as u16 as i16))),
    ]))
}

/// `sourceWord`: a word above 2^31 is emitted as its signed reading.
fn source_word(value: u32) -> f64 {
    if value >= 0x8000_0000 {
        f64::from(value) - 4294967296.0
    } else {
        f64::from(value)
    }
}

fn export_object_scripts(rom: &[u8]) -> Res<Value> {
    let mut scripts = Vec::with_capacity(SCRIPT_LAYOUT.len());
    for (start, end, entries) in SCRIPT_LAYOUT {
        let raw = range(rom, *start, *end)?;
        let mut commands = Vec::new();
        let mut cursor = 0usize;
        while cursor < raw.len() {
            let opcode = read_u32(&raw, cursor);
            cursor += 4;
            if opcode > 63 {
                commands.push(Value::obj(vec![("skip_word", Value::Num(source_word(opcode)))]));
                continue;
            }
            if opcode as usize >= OPCODE_LAYOUT.len() {
                return err("object-script opcode is outside dispatch table");
            }
            let (arity, handler) = OPCODE_LAYOUT[opcode as usize];
            if cursor + arity as usize * 4 > raw.len() {
                return err("object-script command is truncated");
            }
            let mut arguments = Vec::with_capacity(arity as usize);
            for _ in 0..arity {
                let word = read_u32(&raw, cursor);
                cursor += 4;
                let callback = CALLBACK_SYMBOLS.iter().find(|(address, _)| *address == word);
                arguments.push(match callback {
                    Some((_, name)) if opcode == 34 => Value::Str((*name).to_string()),
                    _ => Value::Num(source_word(word)),
                });
            }
            commands.push(Value::obj(vec![
                ("opcode", Value::Str(format!("op_{opcode}"))),
                ("handler", Value::Str(handler.to_string())),
                ("arguments", Value::Arr(arguments)),
            ]));
        }
        let entry_points: Vec<Value> = entries
            .iter()
            .map(|address| {
                Value::obj(vec![
                    ("name", Value::Str(script_name(*address))),
                    ("address", Value::Str(hex(*address))),
                ])
            })
            .collect();
        scripts.push(Value::obj(vec![
            ("address", Value::Str(hex(*start))),
            ("end", Value::Str(hex(*end))),
            ("entry_points", Value::Arr(entry_points)),
            ("commands", Value::Arr(commands)),
        ]));
    }
    Ok(Value::Arr(scripts))
}

fn export_gradients(rom: &[u8]) -> Res<Value> {
    let raw = range(rom, GRADIENT_ADDRESS, GRADIENT_END)?;
    let mut records = Vec::new();
    let mut offset = 0usize;
    while offset < raw.len() {
        let mut runs = Vec::with_capacity(3);
        for run in 0..3usize {
            let cursor = offset + 4 + run * 8;
            runs.push(Value::obj(vec![
                ("length", Value::Num(f64::from(read_u16(&raw, cursor)))),
                ("color", Value::Num(f64::from(read_u16(&raw, cursor + 2)))),
                ("start_low", Value::Num(f64::from(raw[cursor + 4]))),
                ("end_low", Value::Num(f64::from(raw[cursor + 5]))),
                ("start_high", Value::Num(f64::from(raw[cursor + 6]))),
                ("end_high", Value::Num(f64::from(raw[cursor + 7]))),
            ]));
        }
        records.push(Value::obj(vec![
            ("first_color", Value::Num(f64::from(read_u16(&raw, offset)))),
            ("second_color", Value::Num(f64::from(read_u16(&raw, offset + 2)))),
            ("runs", Value::Arr(runs)),
        ]));
        offset += 28;
    }
    Ok(Value::obj(vec![
        ("address", Value::Str(hex(GRADIENT_ADDRESS))),
        ("end", Value::Str(hex(GRADIENT_END))),
        ("stride", Value::Num(28.0)),
        ("consumers", string_list(&["Func_0808f52c"])),
        ("records", Value::Arr(records)),
    ]))
}

fn export_palette_graphics(rom: &[u8], root: &Path) -> Res<Value> {
    let specs: [(u32, u32, u64, &str); 3] = [
        (PALETTE_GRAPHICS_ADDRESS, 0x080a_001e, 0x300, "particle_slash.4bpp.png"),
        (0x080a_001e, 0x080a_00b8, 0x300, "particle_spark.4bpp.png"),
        (0x080a_00b8, PALETTE_GRAPHICS_END, 0x200, "particle_beam.4bpp.png"),
    ];
    let mut graphics = Vec::with_capacity(specs.len());
    for (start, end, decoded_bytes, name) in specs {
        let encoded = range(rom, start, end)?;
        let (decoded, cursor, tokens) =
            decode_palette_trace(&encoded, 0, encoded.len(), decoded_bytes).map_err(|e| e.0)?;
        if cursor != encoded.len() || decoded.len() as u64 != decoded_bytes {
            return err("palette graphic stream extent differs");
        }
        let image = tile_png(&decoded, 4.0, 2, None).map_err(|error| error.0)?.0;
        write_file(&root.join(format!("graphics/battle/effects/{name}")), &image)?;
        graphics.push(Value::obj(vec![
            ("address", Value::Str(hex(start))),
            ("end", Value::Str(hex(end))),
            ("decoded_bytes", Value::Num(decoded_bytes as f64)),
            ("source", Value::Str(format!("graphics/battle/effects/{name}"))),
            ("bpp", Value::Num(4.0)),
            ("columns", Value::Num(2.0)),
            ("codec", Value::Str("palette-lz".to_string())),
            ("tokens", Value::Arr(tokens.iter().map(palette_group_value).collect())),
        ]));
    }
    Ok(Value::Arr(graphics))
}

/// The declared type says the terminator is exactly (-1, 0). Read it and prove
/// it rather than widening the type: a table that ever terminates differently is
/// a decoding error worth failing on, not a looser type.
fn sparse_sentinel(raw: &[u8]) -> Res<Value> {
    let key = read_u32(raw, 41 * 8) as i32;
    let value = read_u32(raw, 41 * 8 + 4) as i32;
    if key != -1 || value != 0 {
        return Err(format!(
            "sparse table sentinel is ({key}, {value}), expected (-1, 0)"
        ));
    }
    Ok(Value::Arr(vec![Value::Num(-1.0), Value::Num(0.0)]))
}

fn export_sparse_table(rom: &[u8]) -> Res<Value> {
    let raw = range(rom, SPARSE_TABLE_ADDRESS, SPARSE_TABLE_END)?;
    let mut records = Vec::new();
    for index in 0..41usize {
        let key = read_u32(&raw, index * 8) as i32;
        let value = read_u32(&raw, index * 8 + 4) as i32;
        if key != 0 || value != 0 {
            records.push(Value::obj(vec![
                ("index", Value::Num(index as f64)),
                ("key", Value::Num(f64::from(key))),
                ("value", Value::Num(f64::from(value))),
            ]));
        }
    }
    Ok(Value::obj(vec![
        ("address", Value::Str(hex(SPARSE_TABLE_ADDRESS))),
        ("end", Value::Str(hex(SPARSE_TABLE_END))),
        ("count", Value::Num(41.0)),
        ("fields", string_list(&["key:s32", "value:s32"])),
        ("consumers", string_list(&["Func_0809bcf8"])),
        ("records", Value::Arr(records)),
        ("sentinel", sparse_sentinel(&raw)?),
    ]))
}

fn write_file(path: &Path, bytes: &[u8]) -> Res<()> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    std::fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn export_battle_effect_data(rom: &[u8], root: &Path) -> Res<Value> {
    let graphics_root = root.join("graphics/battle/effects");
    std::fs::create_dir_all(&graphics_root)
        .map_err(|error| format!("{}: {error}", graphics_root.display()))?;
    let mut direct = Vec::new();
    for (name, start, end) in [
        ("obj_tiles_0809c410.4bpp.png", BATTLE_DATA_ADDRESS, BATTLE_DATA_ADDRESS + 0x100),
        ("obj_tiles_0809c510.4bpp.png", BATTLE_DATA_ADDRESS + 0x100, DIRECT_GRAPHICS_END),
    ] {
        let image = tile_png(&range(rom, start, end)?, 4.0, 4, None)
            .map_err(|error| error.0)?
            .0;
        write_file(&graphics_root.join(name), &image)?;
        direct.push(Value::obj(vec![
            ("address", Value::Str(hex(start))),
            ("end", Value::Str(hex(end))),
            ("source", Value::Str(format!("graphics/battle/effects/{name}"))),
            ("bpp", Value::Num(4.0)),
            ("columns", Value::Num(4.0)),
            ("role", Value::Str("obj_tiles".to_string())),
        ]));
    }
    let raw = range(rom, DIRECT_GRAPHICS_END, WEIGHTED_RECORDS_END)?;
    let mut records = Vec::new();
    let mut offset = 0usize;
    while offset < raw.len() {
        records.push(Value::obj(vec![
            ("base_result", Value::Num(f64::from(read_u16(&raw, offset)))),
            ("rank_origin", Value::Num(f64::from(read_u16(&raw, offset + 2)))),
            (
                "results",
                Value::Arr(
                    (0..8)
                        .map(|index| Value::Num(f64::from(read_u16(&raw, offset + 4 + index * 2))))
                        .collect(),
                ),
            ),
            (
                "weights",
                Value::Arr(
                    raw[offset + 20..offset + 28]
                        .iter()
                        .map(|byte| Value::Num(f64::from(*byte)))
                        .collect(),
                ),
            ),
        ]));
        offset += 28;
    }
    let source = Value::obj(vec![
        ("format", Value::Num(2.0)),
        ("kind", Value::Str("golden-sun-battle-effect-data".to_string())),
        ("address", Value::Str(hex(BATTLE_DATA_ADDRESS))),
        ("end", Value::Str(hex(BATTLE_DATA_END))),
        ("direct_graphics", Value::Arr(direct)),
        (
            "weighted_records",
            Value::obj(vec![
                ("address", Value::Str(hex(DIRECT_GRAPHICS_END))),
                ("end", Value::Str(hex(WEIGHTED_RECORDS_END))),
                ("stride", Value::Num(28.0)),
                (
                    "fields",
                    string_list(&[
                        "base_result:u16",
                        "rank_origin:u16",
                        "results:u16[8]",
                        "weights:u8[8]",
                    ]),
                ),
                ("consumers", string_list(&["Func_0808ae74", "Func_0808b05c"])),
                ("records", Value::Arr(records)),
            ]),
        ),
        ("typed_tables", export_table_collection(rom, RULE_LAYOUT)?),
        ("halfword_graphic", export_halfword_graphic(rom, root)?),
        ("tail_tables", export_table_collection(rom, TAIL_TABLE_LAYOUT)?),
        ("sentinel_lookup", export_sentinel_lookup(rom)?),
        ("object_scripts", export_object_scripts(rom)?),
        (
            "sine_table",
            Value::obj(vec![
                ("address", Value::Str(hex(SINE_TABLE_ADDRESS))),
                ("end", Value::Str(hex(SINE_TABLE_END))),
                ("count", Value::Num(256.0)),
                ("type", Value::Str("s16".to_string())),
                ("formula", Value::Str("trunc(sin(index*pi/128)*0x1000)".to_string())),
            ]),
        ),
        ("gradients", export_gradients(rom)?),
        ("palette_graphics", export_palette_graphics(rom, root)?),
        (
            "symbolic_pointers",
            Value::obj(vec![
                ("address", Value::Str(hex(0x080a_012c))),
                ("end", Value::Str(hex(0x080a_0138))),
                (
                    "pointers",
                    Value::Arr(
                        [0x0809_f13cu32, 0x0809_f148, 0x0809_f154]
                            .iter()
                            .map(|address| Value::Str(script_name(*address)))
                            .collect(),
                    ),
                ),
            ]),
        ),
        ("sparse_table", export_sparse_table(rom)?),
        (
            "zero_fills",
            Value::Arr(vec![
                Value::obj(vec![
                    ("address", Value::Str(hex(0x0809_e9ee))),
                    ("end", Value::Str(hex(0x0809_e9f0))),
                ]),
                Value::obj(vec![
                    ("address", Value::Str(hex(0x080a_0107))),
                    ("end", Value::Str(hex(0x080a_0108))),
                ]),
                Value::obj(vec![
                    ("address", Value::Str(hex(0x080a_0288))),
                    ("end", Value::Str(hex(BATTLE_DATA_END))),
                ]),
            ]),
        ),
    ]);
    if build_battle_effect_data(&source, root)? != range(rom, BATTLE_DATA_ADDRESS, BATTLE_DATA_END)?
    {
        return err("exported battle-effect source does not round-trip");
    }
    Ok(source)
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

fn option(args: &[String], name: &str) -> Option<String> {
    let index = args.iter().position(|arg| arg == name)?;
    args.get(index + 1).cloned()
}

pub fn self_test() -> Res<()> {
    if field_kind("selector:s16")? != FieldKind::S16
        || field_kind("value:u16")? != FieldKind::U16
        || field_kind("mask:u32")? != FieldKind::U32
        || field_kind("slot:s8")? != FieldKind::S8
        || field_size(FieldKind::S8) != 1
        || field_size(FieldKind::S16) != 2
        || field_size(FieldKind::U32) != 4
    {
        return err("battle-effect field self-test failed");
    }
    let halfwords = [0x34u8, 0x12, 0x34, 0x12, 0x34, 0x12, 0x78, 0x56];
    let plan = [
        HalfwordToken::Literal(1),
        HalfwordToken::Copy { length: 2, distance: 1 },
        HalfwordToken::Literal(1),
        HalfwordToken::End,
    ];
    let encoded = encode_halfword(&halfwords, &plan)?;
    let (decoded, replay) = decode_halfword_trace(&encoded)?;
    if decoded != halfwords || replay != plan {
        return err("halfword codec self-test failed");
    }
    if graphic_path(Path::new("assets"), "../private.png").is_ok() {
        return err("battle-effect path escape was accepted");
    }
    println!("self-test=ok");
    Ok(())
}

pub fn run(args: Vec<String>) -> Res<()> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let command = args.first().cloned().unwrap_or_default();
    let root = option(&args, "--root");
    if command == "export" || command == "export-prefix" {
        let rom_path = args.get(1).cloned().unwrap_or_default();
        let output = option(&args, "--output").unwrap_or_default();
        let root = root.unwrap_or_default();
        if rom_path.is_empty() || root.is_empty() || output.is_empty() {
            return err("usage: battle_effect_data.ts export ROM --root ASSETS --output SOURCE");
        }
        let rom = read_file(Path::new(&rom_path))?;
        let source = export_battle_effect_data(&rom, Path::new(&root))?;
        write_file(Path::new(&output), format!("{}\n", canonical_json(&source)).as_bytes())?;
        println!(
            "identical=true bytes={} records=104 tables={}",
            BATTLE_DATA_END - BATTLE_DATA_ADDRESS,
            RULE_LAYOUT.len() + TAIL_TABLE_LAYOUT.len()
        );
        return Ok(());
    }
    if command == "verify" || command == "verify-prefix" {
        let rom_path = args.get(1).cloned().unwrap_or_default();
        let input = args.get(2).cloned().unwrap_or_default();
        let root = root.unwrap_or_default();
        if rom_path.is_empty() || input.is_empty() || root.is_empty() {
            return err("usage: battle_effect_data.ts verify ROM SOURCE --root ASSETS");
        }
        // PORT NOTE: `readFileSync(path, "utf8")` is lossy, hence from_utf8_lossy.
        let text = String::from_utf8_lossy(&read_file(Path::new(&input))?).into_owned();
        let document = json::parse(&text)?;
        let built = build_battle_effect_data(&document, Path::new(&root))?;
        let rom = read_file(Path::new(&rom_path))?;
        if built != range(&rom, BATTLE_DATA_ADDRESS, BATTLE_DATA_END)? {
            return err("battle-effect source differs from ROM");
        }
        println!("identical=true bytes={} records=104", built.len());
        return Ok(());
    }
    err("usage: battle_effect_data.ts {export ROM --root ASSETS --output SOURCE|verify ROM SOURCE --root ASSETS}")
}

#[cfg(test)]
mod tests;
