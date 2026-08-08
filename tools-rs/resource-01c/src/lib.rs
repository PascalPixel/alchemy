//! Native codec for the audited resource_01c kana glyph bank.
//!
//! The TypeScript module remains as a library dependency of the legacy asset
//! builder. This crate owns the equivalent standalone export, verify, and
//! self-test commands so direct use does not require Bun.

use std::fs;
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};

use canonical_json::is_canonical_json_text;
use export_asset::tile_png;
use extract_resource::{decode_general_trace, encode_general, GeneralToken};
use import_asset::{gba_graphics, Rgb};
use serde_json::{json, Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x0080_0000;
pub const RESOURCE_TABLE: u32 = 0x0832_0000;
pub const RESOURCE_ID: u32 = 0x01c;
pub const RESOURCE_ADDRESS: u32 = 0x0833_ac08;
pub const RESOURCE_SIZE: usize = 0x02b4;
pub const RESOURCE_END: u32 = RESOURCE_ADDRESS + RESOURCE_SIZE as u32;
pub const PALETTE_BYTES: usize = 0x20;
pub const GLYPHS: usize = 36;
pub const ATLAS_COLUMNS: usize = 9;
const TILE_BYTES: usize = 0x20;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

fn err(message: impl Into<String>) -> Error {
    Error(message.into())
}

#[derive(Debug, Clone)]
struct Plan {
    tokens: Vec<GeneralToken>,
    source: String,
}

fn read(path: &Path) -> Result<Vec<u8>, Error> {
    fs::read(path).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn write(path: &Path, data: &[u8]) -> Result<(), Error> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent)
            .map_err(|error| err(format!("{}: {error}", parent.display())))?;
    }
    fs::write(path, data).map_err(|error| err(format!("{}: {error}", path.display())))
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, Error> {
    value
        .as_object()
        .ok_or_else(|| err(format!("{label} must be an object")))
}

fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<(), Error> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    actual.sort_unstable();
    let mut expected = expected.to_vec();
    expected.sort_unstable();
    if actual != expected {
        return Err(err(format!("{label} has unexpected fields")));
    }
    Ok(())
}

fn string<'a>(value: &'a Value, name: &str) -> Result<&'a str, Error> {
    value
        .get(name)
        .and_then(Value::as_str)
        .ok_or_else(|| err(format!("invalid {name}")))
}

fn number(value: &Value) -> Option<u64> {
    value.as_u64().or_else(|| {
        value.as_str().and_then(|text| {
            text.strip_prefix("0x")
                .or_else(|| text.strip_prefix("0X"))
                .map_or_else(
                    || text.parse().ok(),
                    |hex| u64::from_str_radix(hex, 16).ok(),
                )
        })
    })
}

fn number_field(value: &Value, name: &str) -> Result<u64, Error> {
    value
        .get(name)
        .and_then(number)
        .ok_or_else(|| err(format!("invalid {name}")))
}

fn json_document(path: &Path, label: &str) -> Result<Value, Error> {
    let text =
        fs::read_to_string(path).map_err(|error| err(format!("{}: {error}", path.display())))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|error| err(format!("{label}: {error}")))?;
    if !is_canonical_json_text(&text, &value) {
        return Err(err(format!("{label} is not canonical JSON")));
    }
    Ok(value)
}

fn parse_tokens(value: &Value) -> Result<Vec<GeneralToken>, Error> {
    let values = value
        .as_array()
        .ok_or_else(|| err("kana tokens must be an array"))?;
    let mut tokens = Vec::with_capacity(values.len());
    for (index, value) in values.iter().enumerate() {
        let token = value
            .as_array()
            .ok_or_else(|| err(format!("kana token {index} must be an array")))?;
        match token.first().and_then(Value::as_str) {
            Some("l") if token.len() == 2 => {
                let count = token[1].as_u64();
                if count.is_none_or(|count| count == 0) {
                    return Err(err(format!(
                        "kana token {index} has an invalid literal shape"
                    )));
                }
                tokens.push(GeneralToken::Literal(count.unwrap() as u32));
            }
            Some("c") if token.len() == 3 => {
                let length = token[1].as_u64();
                let distance = token[2].as_u64();
                if length.is_none_or(|value| !(2..=137).contains(&value))
                    || distance.is_none_or(|value| !(1..=0x1020).contains(&value))
                {
                    return Err(err(format!("kana token {index} has an invalid copy shape")));
                }
                tokens.push(GeneralToken::Copy {
                    length: length.unwrap() as u32,
                    distance: distance.unwrap() as u32,
                });
            }
            _ => {
                return Err(err(format!(
                    "kana token {index} has an unsupported operation"
                )))
            }
        }
    }
    Ok(tokens)
}

fn token_value(token: GeneralToken) -> Value {
    match token {
        GeneralToken::Literal(count) => json!(["l", count]),
        GeneralToken::Copy { length, distance } => json!(["c", length, distance]),
    }
}

fn hex(value: u32, width: usize) -> String {
    format!("0x{value:0width$x}")
}

fn read_plan(path: &Path) -> Result<Plan, Error> {
    let value = json_document(path, "kana plan")?;
    let plan = object(&value, "kana plan")?;
    exact_keys(
        plan,
        &[
            "format",
            "kind",
            "codec",
            "resource_id",
            "address",
            "size",
            "end",
            "decoded_size",
            "palette_entries",
            "glyph_width",
            "glyph_height",
            "glyphs",
            "atlas_columns",
            "source",
            "tokens",
        ],
        "kana plan",
    )?;
    if number_field(&value, "format")? != 1
        || string(&value, "kind")? != "golden-sun-kana-glyph-bank"
        || string(&value, "codec")? != "golden-sun-general-lz"
        || string(&value, "resource_id")? != "01c"
        || string(&value, "address")? != hex(RESOURCE_ADDRESS, 8)
        || string(&value, "size")? != hex(RESOURCE_SIZE as u32, 1)
        || string(&value, "end")? != hex(RESOURCE_END, 8)
        || number_field(&value, "decoded_size")? != (PALETTE_BYTES + GLYPHS * TILE_BYTES) as u64
        || number_field(&value, "palette_entries")? != 16
        || number_field(&value, "glyph_width")? != 8
        || number_field(&value, "glyph_height")? != 8
        || number_field(&value, "glyphs")? != GLYPHS as u64
        || number_field(&value, "atlas_columns")? != ATLAS_COLUMNS as u64
        || string(&value, "source")? != "kana.4bpp.png"
    {
        return Err(err("kana plan differs from the audited layout"));
    }
    Ok(Plan {
        tokens: parse_tokens(plan.get("tokens").unwrap())?,
        source: string(&value, "source")?.to_string(),
    })
}

fn bgr555(data: &[u8]) -> Result<Vec<Rgb>, Error> {
    if data.len() != PALETTE_BYTES {
        return Err(err("kana palette has the wrong size"));
    }
    data.chunks_exact(2)
        .map(|chunk| {
            let value = u16::from_le_bytes([chunk[0], chunk[1]]);
            if value & 0x8000 != 0 {
                return Err(err("kana palette uses bit 15"));
            }
            Ok([
                ((value & 31) << 3) as u8,
                (((value >> 5) & 31) << 3) as u8,
                (((value >> 10) & 31) << 3) as u8,
            ])
        })
        .collect()
}

fn physical_path(path: &Path) -> PathBuf {
    let absolute = if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir().unwrap_or_default().join(path)
    };
    let mut existing = absolute.clone();
    let mut suffix = Vec::new();
    while !existing.exists() {
        let Some(name) = existing.file_name() else {
            return absolute;
        };
        suffix.push(name.to_owned());
        let Some(parent) = existing.parent() else {
            return absolute;
        };
        existing = parent.to_path_buf();
    }
    let mut result = fs::canonicalize(existing).unwrap_or_else(|_| absolute.clone());
    for name in suffix.iter().rev() {
        result.push(name);
    }
    result
}

fn path_contains(directory: &Path, path: &Path) -> bool {
    let directory = physical_path(directory);
    let path = physical_path(path);
    let mut base = directory.components();
    let mut child = path.components();
    loop {
        match (base.next(), child.next()) {
            (None, _) => return true,
            (Some(left), Some(right)) if left == right => {}
            _ => return false,
        }
    }
}

fn validate_export_destination(rom: &Path, directory: &Path) -> Result<(), Error> {
    if physical_path(rom) == physical_path(directory) || path_contains(directory, rom) {
        return Err(err("kana export directory must not contain its input ROM"));
    }
    if !directory.exists() {
        return Ok(());
    }
    let metadata = fs::symlink_metadata(directory).map_err(|error| err(error.to_string()))?;
    if !metadata.file_type().is_dir() {
        return Err(err("kana export destination must be a directory"));
    }
    let mut entries = fs::read_dir(directory).map_err(|error| err(error.to_string()))?;
    if entries.next().is_none() {
        return Ok(());
    }
    let marker = directory.join("stream.json");
    if !marker.is_file() {
        return Err(err(
            "refusing to replace a directory that is not a kana package",
        ));
    }
    read_plan(&marker)
        .map(|_| ())
        .map_err(|_| err("refusing to replace a directory that is not a canonical kana package"))
}

fn source_path(plan_path: &Path, name: &str) -> Result<PathBuf, Error> {
    if name != "kana.4bpp.png" {
        return Err(err("kana source name differs"));
    }
    let basename = plan_path
        .file_name()
        .and_then(|name| name.to_str())
        .ok_or_else(|| err("kana source name differs"))?;
    let prefix = if basename.bytes().all(|byte| {
        byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_' || byte == b'.'
    }) {
        basename
            .rfind('_')
            .map_or_else(String::new, |index| basename[..=index].to_string())
    } else {
        String::new()
    };
    let root = fs::canonicalize(
        plan_path
            .parent()
            .ok_or_else(|| err("kana plan has no parent"))?,
    )
    .map_err(|error| err(error.to_string()))?;
    let path = fs::canonicalize(root.join(format!("{prefix}{name}")))
        .map_err(|error| err(error.to_string()))?;
    let relative = path
        .strip_prefix(&root)
        .map_err(|_| err("kana source escaped its directory"))?;
    if relative != Path::new(&format!("{prefix}{name}")) {
        return Err(err("kana source escaped its directory"));
    }
    Ok(path)
}

fn read_kana_image(path: &Path) -> Result<(Vec<u8>, Vec<u8>), Error> {
    let encoded = read(path)?;
    let (tiles, palette, report) = gba_graphics(&encoded, 4.0).map_err(|error| err(error.0))?;
    let colors = bgr555(&palette)?;
    let (reencoded, _) =
        tile_png(&tiles, 4.0, ATLAS_COLUMNS as f64, Some(&colors)).map_err(|error| err(error.0))?;
    if reencoded != encoded {
        return Err(err("kana atlas is not a canonical source PNG"));
    }
    if report.get("width") != Some((ATLAS_COLUMNS * 8) as f64)
        || report.get("height") != Some((GLYPHS / ATLAS_COLUMNS * 8) as f64)
        || report.get("tiles") != Some(GLYPHS as f64)
        || report.get("palette_entries") != Some(16.0)
        || palette.len() != PALETTE_BYTES
        || tiles.len() != GLYPHS * TILE_BYTES
    {
        return Err(err("kana atlas differs from the audited dimensions"));
    }
    Ok((tiles, palette))
}

pub fn build_resource_01c(plan_path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>), Error> {
    let plan = read_plan(plan_path)?;
    let image_path = source_path(plan_path, &plan.source)?;
    let (tiles, palette) = read_kana_image(&image_path)?;
    let mut decoded = palette;
    decoded.extend_from_slice(&tiles);
    let encoded = encode_general(&decoded, &plan.tokens).map_err(|error| err(error.0))?;
    if encoded.len() != RESOURCE_SIZE {
        return Err(err("kana resource has the wrong encoded size"));
    }
    Ok((encoded, vec![physical_path(plan_path), image_path]))
}

fn resource_pointer(rom: &[u8], id: u32) -> Result<u32, Error> {
    let start = RESOURCE_TABLE
        .checked_sub(ROM_BASE)
        .and_then(|offset| (offset as usize).checked_add(id as usize * 4))
        .ok_or_else(|| err("resource table is outside the ROM"))?;
    let bytes = rom
        .get(start..start + 4)
        .ok_or_else(|| err("resource table is outside the ROM"))?;
    Ok(u32::from_le_bytes(bytes.try_into().unwrap()))
}

fn validate_directory_bounds(rom: &[u8]) -> Result<(), Error> {
    if resource_pointer(rom, RESOURCE_ID)? != RESOURCE_ADDRESS
        || resource_pointer(rom, RESOURCE_ID + 1)? != RESOURCE_END
    {
        return Err(err(
            "kana resource differs from the audited directory bounds",
        ));
    }
    Ok(())
}

fn unique_temp(parent: &Path) -> Result<PathBuf, Error> {
    let stamp = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map(|duration| duration.as_nanos())
        .unwrap_or_default();
    for attempt in 0..100u32 {
        let path = parent.join(format!(
            ".kana-export-{}-{}-{attempt}",
            std::process::id(),
            stamp
        ));
        match fs::create_dir(&path) {
            Ok(()) => return Ok(path),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {}
            Err(error) => return Err(err(error.to_string())),
        }
    }
    Err(err("could not create kana export transaction"))
}

fn replace_directory<T>(
    directory: &Path,
    write_staged: impl FnOnce(&Path) -> Result<T, Error>,
) -> Result<T, Error> {
    let destination = directory.to_path_buf();
    let parent = destination
        .parent()
        .ok_or_else(|| err("kana export requires a dedicated directory"))?;
    if destination == parent {
        return Err(err("kana export requires a dedicated directory"));
    }
    fs::create_dir_all(parent).map_err(|error| err(error.to_string()))?;
    let transaction = unique_temp(parent)?;
    let staged = transaction.join("new");
    let previous = transaction.join("previous");
    let result = write_staged(&staged);
    if result.is_err() {
        let _ = fs::remove_dir_all(&transaction);
        return result;
    }
    let result = result.unwrap();
    if destination.exists() {
        fs::rename(&destination, &previous).map_err(|error| err(error.to_string()))?;
    }
    if let Err(error) = fs::rename(&staged, &destination) {
        if previous.exists() {
            let _ = fs::rename(&previous, &destination);
        }
        let _ = fs::remove_dir_all(&transaction);
        return Err(err(error.to_string()));
    }
    fs::remove_dir_all(&transaction).map_err(|error| err(error.to_string()))?;
    Ok(result)
}

fn write_resource(rom: &[u8], directory: &Path) -> Result<Plan, Error> {
    if rom.len() != ROM_SIZE {
        return Err(err("kana exporter requires the 8 MiB canonical ROM"));
    }
    validate_directory_bounds(rom)?;
    let start = (RESOURCE_ADDRESS - ROM_BASE) as usize;
    let encoded = rom
        .get(start..start + RESOURCE_SIZE)
        .ok_or_else(|| err("kana resource is outside the ROM"))?;
    let (decoded, used, tokens) =
        decode_general_trace(encoded, 0, encoded.len(), 0x10000).map_err(|error| err(error.0))?;
    if used != encoded.len() || decoded.len() != PALETTE_BYTES + GLYPHS * TILE_BYTES {
        return Err(err("kana resource has an unexpected compressed extent"));
    }
    let replay = encode_general(&decoded, &tokens).map_err(|error| err(error.0))?;
    if replay != encoded {
        return Err(err("kana compression replay differs"));
    }
    fs::create_dir_all(directory).map_err(|error| err(error.to_string()))?;
    let image_path = directory.join("kana.4bpp.png");
    let plan_path = directory.join("stream.json");
    let colors = bgr555(&decoded[..PALETTE_BYTES])?;
    let (image, _) = tile_png(
        &decoded[PALETTE_BYTES..],
        4.0,
        ATLAS_COLUMNS as f64,
        Some(&colors),
    )
    .map_err(|error| err(error.0))?;
    write(&image_path, &image)?;
    let plan_value = json!({
        "format": 1,
        "kind": "golden-sun-kana-glyph-bank",
        "codec": "golden-sun-general-lz",
        "resource_id": "01c",
        "address": hex(RESOURCE_ADDRESS, 8),
        "size": hex(RESOURCE_SIZE as u32, 1),
        "end": hex(RESOURCE_END, 8),
        "decoded_size": decoded.len(),
        "palette_entries": 16,
        "glyph_width": 8,
        "glyph_height": 8,
        "glyphs": GLYPHS,
        "atlas_columns": ATLAS_COLUMNS,
        "source": "kana.4bpp.png",
        "tokens": tokens.iter().copied().map(token_value).collect::<Vec<_>>(),
    });
    let plan_text = serde_json::to_string(&plan_value).map_err(|error| err(error.to_string()))?;
    write(&plan_path, format!("{plan_text}\n").as_bytes())?;
    let (rebuilt, _) = build_resource_01c(&plan_path)?;
    if rebuilt != encoded {
        return Err(err("exported kana resource differs"));
    }
    Ok(Plan {
        tokens,
        source: "kana.4bpp.png".into(),
    })
}

pub fn export_resource_01c(rom_path: &Path, directory: &Path) -> Result<usize, Error> {
    validate_export_destination(rom_path, directory)?;
    let rom = read(rom_path)?;
    replace_directory(directory, |staged| {
        write_resource(&rom, staged).map(|plan| plan.tokens.len())
    })
}

pub fn verify_resource_01c(rom_path: &Path, plan_path: &Path) -> Result<String, Error> {
    let rom = read(rom_path)?;
    if rom.len() != ROM_SIZE {
        return Err(err("kana verifier requires the 8 MiB canonical ROM"));
    }
    validate_directory_bounds(&rom)?;
    let (built, _) = build_resource_01c(plan_path)?;
    let start = (RESOURCE_ADDRESS - ROM_BASE) as usize;
    let original = &rom[start..start + RESOURCE_SIZE];
    if built != original {
        return Err(err("kana resource differs from ROM"));
    }
    Ok(format!(
        "identical=true glyphs={GLYPHS} source_bytes={}",
        built.len()
    ))
}

pub fn self_test() -> Result<(), Error> {
    let mut palette = vec![0u8; PALETTE_BYTES];
    for index in 0..16u16 {
        palette[index as usize * 2..index as usize * 2 + 2]
            .copy_from_slice(&(index | index << 5 | index << 10).to_le_bytes());
    }
    let colors = bgr555(&palette)?;
    if colors.len() != 16 || colors[15] != [120, 120, 120] {
        return Err(err("kana palette self-test failed"));
    }
    if parse_tokens(&json!([["l", 1, "ignored"]])).is_ok() {
        return Err(err("kana token strictness self-test failed"));
    }
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .unwrap();
    let plan = root.join("assets/graphics/resource_01c_stream.json");
    let rom = root.join("baserom.gba");
    if plan.is_file() && rom.is_file() {
        verify_resource_01c(&rom, &plan)?;
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tracked_resource_matches_the_released_rom() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .unwrap();
        let plan = root.join("assets/graphics/resource_01c_stream.json");
        let rom = root.join("baserom.gba");
        let result = verify_resource_01c(&rom, &plan).expect("resource 01c parity");
        assert_eq!(result, "identical=true glyphs=36 source_bytes=692");
    }

    #[test]
    fn token_plan_is_strict_and_round_trips() {
        let tokens = parse_tokens(&json!([["l", 2], ["c", 2, 1]])).unwrap();
        assert_eq!(
            tokens,
            vec![
                GeneralToken::Literal(2),
                GeneralToken::Copy {
                    length: 2,
                    distance: 1
                }
            ]
        );
        assert!(parse_tokens(&json!([["c", 1, 1]])).is_err());
        assert!(parse_tokens(&json!([["l", 1, "extra"]])).is_err());
    }

    #[test]
    fn palette_rejects_high_bit() {
        assert!(bgr555(&[0, 0x80].repeat(16)).is_err());
    }
}
