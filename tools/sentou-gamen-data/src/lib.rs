//! Native build/verify support for `tools/make/sentou_gamen_data.ts`.
//!
//! This crate intentionally has no exporter: the tracked battle-screen JSON
//! and PNGs are authored assets, while the build and ROM verification paths are
//! the parts needed by the native asset toolchain.

use canonical_json::is_canonical_json_text;
use import_asset::indexed_png;
use serde_json::Value;
use std::fs;
use std::path::{Path, PathBuf};

pub const ROM_BASE: usize = 0x0800_0000;
pub const ADDRESS: usize = 0x080a_ea4c;
pub const END: usize = 0x080b_0000;
pub const SIZE: usize = END - ADDRESS;
const DISPLAY_GLYPH_ADDRESS: usize = 0x080a_f20c;
const MASK_TILE_ADDRESS: usize = 0x080a_f26c;
const ALIGNMENT_ADDRESS: usize = 0x080a_f314;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
pub type Result<T> = std::result::Result<T, Error>;
fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

#[derive(Clone, Copy)]
enum Role {
    FrameAtlas,
    Flat,
}

#[derive(Clone, Copy)]
struct GraphicSpec {
    address: usize,
    size: usize,
    source: &'static str,
    role: Role,
    tiles_wide: usize,
    tiles_high: usize,
    frames: usize,
    columns: usize,
}

const GRAPHICS: [GraphicSpec; 5] = [
    GraphicSpec {
        address: ADDRESS,
        size: 0x600,
        source: "koma.4bpp.png",
        role: Role::FrameAtlas,
        tiles_wide: 2,
        tiles_high: 2,
        frames: 12,
        columns: 3,
    },
    GraphicSpec {
        address: 0x080a_f04c,
        size: 0x40,
        source: "obi.4bpp.png",
        role: Role::Flat,
        tiles_wide: 2,
        tiles_high: 1,
        frames: 0,
        columns: 0,
    },
    GraphicSpec {
        address: 0x080a_f08c,
        size: 0x100,
        source: "men.4bpp.png",
        role: Role::Flat,
        tiles_wide: 4,
        tiles_high: 2,
        frames: 0,
        columns: 0,
    },
    GraphicSpec {
        address: 0x080a_f18c,
        size: 0x80,
        source: "hyouji.4bpp.png",
        role: Role::Flat,
        tiles_wide: 2,
        tiles_high: 2,
        frames: 0,
        columns: 0,
    },
    GraphicSpec {
        address: MASK_TILE_ADDRESS,
        size: 0x20,
        source: "masuku.4bpp.png",
        role: Role::Flat,
        tiles_wide: 1,
        tiles_high: 1,
        frames: 0,
        columns: 0,
    },
];

fn object<'a>(value: &'a Value, name: &str) -> Result<&'a serde_json::Map<String, Value>> {
    value
        .as_object()
        .ok_or_else(|| Error(format!("{name} must be an object")))
}

fn keys(value: &Value, expected: &[&str], name: &str) -> Result<()> {
    let actual = object(value, name)?;
    let mut a: Vec<&str> = actual.keys().map(String::as_str).collect();
    let mut e = expected.to_vec();
    a.sort_unstable();
    e.sort_unstable();
    if a != e {
        return err(format!("{name} has unexpected fields"));
    }
    Ok(())
}

fn array<'a>(value: &'a Value, count: usize, name: &str) -> Result<&'a [Value]> {
    let values = value
        .as_array()
        .ok_or_else(|| Error(format!("{name} requires {count} entries")))?;
    if values.len() != count {
        return err(format!("{name} requires {count} entries"));
    }
    Ok(values)
}

fn integer(value: Option<&Value>, min: i64, max: i64, name: &str) -> Result<i64> {
    let n = value
        .and_then(Value::as_i64)
        .ok_or_else(|| Error(format!("{name} is outside its range")))?;
    if !(min..=max).contains(&n) {
        return err(format!("{name} is outside its range"));
    }
    Ok(n)
}

fn unsigned(value: Option<&Value>, max: u64, name: &str) -> Result<u64> {
    let n = value
        .and_then(Value::as_u64)
        .ok_or_else(|| Error(format!("{name} is outside its range")))?;
    if n > max {
        return err(format!("{name} is outside its range"));
    }
    Ok(n)
}

fn address(value: Option<&Value>, expected: usize, name: &str) -> Result<()> {
    if value.and_then(Value::as_str) != Some(&format!("0x{expected:08x}")) {
        return err(format!("{name} differs"));
    }
    Ok(())
}

fn source_json(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    let value: Value =
        serde_json::from_str(&text).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    if !is_canonical_json_text(&text, &value) {
        return err("battle-screen source is not canonical JSON");
    }
    keys(
        &value,
        &[
            "format",
            "kind",
            "address",
            "end",
            "size",
            "graphics",
            "display_glyphs",
            "mask_rows",
            "auxiliary_display_glyphs",
            "offsets",
            "orders",
            "lookup_ids",
            "mode_selectors",
            "object_resource_ids",
            "alignment",
        ],
        "battle-screen source",
    )?;
    if value.get("format") != Some(&Value::from(1))
        || value.get("kind").and_then(Value::as_str) != Some("golden-sun-sentou-gamen-data")
        || value.get("size").and_then(Value::as_u64) != Some(SIZE as u64)
    {
        return err("battle-screen extent differs");
    }
    address(value.get("address"), ADDRESS, "battle-screen address")?;
    address(value.get("end"), END, "battle-screen end")?;
    Ok(value)
}

fn layout(spec: GraphicSpec) -> (usize, usize, Vec<(usize, usize)>) {
    let (width, height, frame_count) = match spec.role {
        Role::FrameAtlas => {
            let fw = spec.tiles_wide * 8;
            let fh = spec.tiles_high * 8;
            (
                spec.columns * fw,
                spec.frames.div_ceil(spec.columns) * fh,
                spec.frames,
            )
        }
        Role::Flat => (spec.tiles_wide * 8, spec.tiles_high * 8, 1),
    };
    let mut tiles = Vec::new();
    for frame in 0..frame_count {
        let (left, top) = match spec.role {
            Role::FrameAtlas => (
                frame % spec.columns * spec.tiles_wide * 8,
                frame / spec.columns * spec.tiles_high * 8,
            ),
            Role::Flat => (0, 0),
        };
        for y in 0..spec.tiles_high {
            for x in 0..spec.tiles_wide {
                tiles.push((left + x * 8, top + y * 8));
            }
        }
    }
    (width, height, tiles)
}

fn child(index_path: &Path, name: &str) -> Result<PathBuf> {
    if !GRAPHICS.iter().any(|s| s.source == name) {
        return err("battle-screen graphic name differs");
    }
    let root = index_path
        .parent()
        .ok_or_else(|| Error("battle-screen index has no parent".into()))?;
    let prefix = index_path
        .file_name()
        .and_then(|n| n.to_str())
        .unwrap_or("")
        .strip_suffix("index.json")
        .unwrap_or("");
    let candidate = root.join(format!("{prefix}{name}"));
    let path = candidate
        .canonicalize()
        .map_err(|e| Error(format!("{}: {e}", candidate.display())))?;
    let root = root
        .canonicalize()
        .map_err(|e| Error(format!("{}: {e}", root.display())))?;
    if !path.starts_with(&root) {
        return err("battle-screen graphic escaped its directory");
    }
    Ok(path)
}

fn graphic(path: &Path, spec: GraphicSpec) -> Result<Vec<u8>> {
    let image =
        indexed_png(&fs::read(path).map_err(|e| Error(format!("{}: {e}", path.display())))?)
            .map_err(|e| Error(e.0))?;
    let (width, height, tiles) = layout(spec);
    if image.width as usize != width || image.height as usize != height {
        return err(format!("{}: graphic dimensions differ", path.display()));
    }
    let palette: Vec<[u8; 3]> = (0..16).map(|i| [i * 8, i * 8, i * 8]).collect();
    if image.palette != palette {
        return err(format!("{}: symbolic palette differs", path.display()));
    }
    if tiles.len() * 32 != spec.size {
        return err("battle-screen graphic geometry differs from its extent");
    }
    let mut output = Vec::with_capacity(spec.size);
    for (left, top) in tiles {
        for y in 0..8 {
            for x in (0..8).step_by(2) {
                let a = image.pixels[(top + y) * width + left + x];
                let b = image.pixels[(top + y) * width + left + x + 1];
                if a >= 16 || b >= 16 {
                    return err(format!("{}: pixel exceeds 4bpp", path.display()));
                }
                output.push(a as u8 | (b as u8) << 4);
            }
        }
    }
    Ok(output)
}

fn words(value: &Value, rows: usize, columns: usize, max: u64, name: &str) -> Result<Vec<u8>> {
    let mut out = Vec::with_capacity(rows * columns * 4);
    for (r, row) in array(value, rows, name)?.iter().enumerate() {
        for (c, item) in array(row, columns, &format!("{name} row {r}"))?
            .iter()
            .enumerate()
        {
            let n = unsigned(Some(item), max, &format!("{name} {r}:{c}"))? as u32;
            out.extend_from_slice(&n.to_le_bytes());
        }
    }
    Ok(out)
}

fn glyph_cells(value: &Value, rows: usize, columns: usize, name: &str) -> Result<Vec<u8>> {
    let mut out = vec![0u8; rows * columns * 4];
    for (r, row) in array(value, rows, name)?.iter().enumerate() {
        for (c, cell) in array(row, columns, &format!("{name} row {r}"))?
            .iter()
            .enumerate()
        {
            let glyphs = cell
                .as_array()
                .ok_or_else(|| Error(format!("{name} {r}:{c} requires one to three glyphs")))?;
            if !(1..=3).contains(&glyphs.len()) {
                return err(format!("{name} {r}:{c} requires one to three glyphs"));
            }
            for (i, glyph) in glyphs.iter().enumerate() {
                out[r * columns * 4 + c * 4 + i] =
                    integer(Some(glyph), 1, 255, &format!("{name} {r}:{c}:{i}"))? as u8;
            }
        }
    }
    Ok(out)
}

fn signed_bytes(value: &Value, count: usize, name: &str) -> Result<Vec<u8>> {
    Ok(array(value, count, name)?
        .iter()
        .enumerate()
        .map(|(i, v)| integer(Some(v), -128, 127, &format!("{name} {i}")).map(|n| n as i8 as u8))
        .collect::<Result<Vec<_>>>()?)
}

fn orders(value: &Value) -> Result<Vec<u8>> {
    let lengths = [10, 10, 19, 19];
    let mut out = Vec::new();
    for (i, row) in array(value, 4, "orders")?.iter().enumerate() {
        let values: Vec<u8> = array(row, lengths[i], "order")?
            .iter()
            .enumerate()
            .map(|(j, v)| unsigned(Some(v), 0xfe, &format!("order {i}:{j}")).map(|n| n as u8))
            .collect::<Result<_>>()?;
        let mut sorted = values.clone();
        sorted.sort_unstable();
        let mut expected: Vec<u8> = (0..10).collect();
        if i >= 2 {
            expected.extend(0x81..0x8a);
        }
        if sorted != expected {
            return err(format!("order {i} is not the required permutation"));
        }
        out.extend(values);
        out.push(0xff);
    }
    Ok(out)
}

pub fn build_sentou_gamen_data(index_path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>)> {
    let source = source_json(index_path)?;
    let entries = array(
        source.get("graphics").unwrap(),
        GRAPHICS.len(),
        "battle-screen graphic catalog",
    )?;
    let mut images = Vec::new();
    let mut nested = vec![index_path.to_path_buf()];
    for (i, entry) in entries.iter().enumerate() {
        let spec = GRAPHICS[i];
        let role_keys = match spec.role {
            Role::FrameAtlas => &[
                "address",
                "size",
                "source",
                "role",
                "frames",
                "frame_tiles_wide",
                "frame_tiles_high",
                "columns",
            ][..],
            Role::Flat => &[
                "address",
                "size",
                "source",
                "role",
                "tiles_wide",
                "tiles_high",
            ][..],
        };
        keys(entry, role_keys, &format!("battle-screen graphic {i}"))?;
        let o = object(entry, "battle-screen graphic")?;
        address(
            o.get("address"),
            spec.address,
            &format!("battle-screen graphic {i} address"),
        )?;
        if o.get("size").and_then(Value::as_u64) != Some(spec.size as u64)
            || o.get("source").and_then(Value::as_str) != Some(spec.source)
        {
            return err(format!("battle-screen graphic {i} layout differs"));
        }
        if matches!(spec.role, Role::FrameAtlas)
            && (o.get("frames").and_then(Value::as_u64) != Some(12)
                || o.get("frame_tiles_wide").and_then(Value::as_u64) != Some(2)
                || o.get("frame_tiles_high").and_then(Value::as_u64) != Some(2)
                || o.get("columns").and_then(Value::as_u64) != Some(3))
        {
            return err(format!("battle-screen graphic {i} frame layout differs"));
        }
        let path = child(index_path, spec.source)?;
        nested.push(path.clone());
        images.push(graphic(&path, spec)?);
    }
    keys(
        source.get("offsets").unwrap(),
        &["x", "y"],
        "battle-screen offsets",
    )?;
    keys(
        source.get("alignment").unwrap(),
        &["address", "end", "fill"],
        "battle-screen alignment",
    )?;
    let alignment = object(source.get("alignment").unwrap(), "alignment")?;
    address(
        alignment.get("address"),
        ALIGNMENT_ADDRESS,
        "battle-screen alignment address",
    )?;
    address(alignment.get("end"), END, "battle-screen alignment end")?;
    if alignment.get("fill").and_then(Value::as_i64) != Some(0) {
        return err("battle-screen alignment differs");
    }
    let mut out = Vec::new();
    out.extend(&images[0]);
    out.extend(&images[1]);
    out.extend(&images[2]);
    out.extend(&images[3]);
    out.extend(glyph_cells(
        source.get("display_glyphs").unwrap(),
        3,
        4,
        "display glyphs",
    )?);
    out.extend(words(
        source.get("mask_rows").unwrap(),
        6,
        2,
        u32::MAX as u64,
        "mask rows",
    )?);
    out.extend(&images[4]);
    let auxiliary = Value::Array(vec![source
        .get("auxiliary_display_glyphs")
        .unwrap()
        .clone()]);
    out.extend(glyph_cells(&auxiliary, 1, 2, "auxiliary display glyphs")?);
    let offsets = object(source.get("offsets").unwrap(), "offsets")?;
    out.extend(signed_bytes(offsets.get("x").unwrap(), 9, "x offsets")?);
    out.extend(signed_bytes(offsets.get("y").unwrap(), 9, "y offsets")?);
    out.extend(orders(source.get("orders").unwrap())?);
    out.extend(words(
        source.get("lookup_ids").unwrap(),
        2,
        3,
        0xffff,
        "lookup IDs",
    )?);
    let selectors = array(source.get("mode_selectors").unwrap(), 5, "mode selectors")?;
    for (i, v) in selectors.iter().enumerate() {
        let n = unsigned(Some(v), 7, &format!("mode selectors {i}"))?;
        if n != 6 && n != 7 {
            return err("mode selector is outside its known modes");
        }
        out.push(n as u8);
    }
    out.extend([0, 0, 0]);
    let object_ids = Value::Array(vec![source.get("object_resource_ids").unwrap().clone()]);
    out.extend(words(&object_ids, 1, 4, 0xffff, "object resource IDs")?);
    out.resize(SIZE, 0);
    if out.len() != SIZE {
        return err("battle-screen package size differs");
    }
    Ok((out, nested))
}

pub fn verify_sentou_gamen_data(rom: &[u8], index_path: &Path) -> Result<()> {
    if rom.len() != 0x800000 {
        return err("battle-screen verifier requires the canonical 8 MiB ROM");
    }
    let start = ADDRESS - ROM_BASE;
    let expected = &rom[start..start + SIZE];
    let (actual, _) = build_sentou_gamen_data(index_path)?;
    if actual != expected {
        return err("battle-screen package differs from ROM");
    }
    Ok(())
}

pub fn self_test() -> Result<()> {
    let dimensions = [(48, 64), (16, 8), (32, 16), (16, 16), (8, 8)];
    let mut cursor = ADDRESS;
    for (i, spec) in GRAPHICS.iter().enumerate() {
        let (width, height, tiles) = layout(*spec);
        if (width, height) != dimensions[i] || tiles.len() * 32 != spec.size {
            return err("battle-screen graphic partition differs");
        }
        if spec.address < MASK_TILE_ADDRESS {
            if spec.address != cursor {
                return err("battle-screen display graphics are not contiguous");
            }
            cursor += spec.size;
        }
    }
    if cursor != DISPLAY_GLYPH_ADDRESS || SIZE != 5556 {
        return err("battle-screen display graphic extent differs");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn layout_and_constants_are_stable() {
        self_test().unwrap();
    }
}
