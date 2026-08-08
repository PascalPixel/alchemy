use canonical_json::is_canonical_json_text;
use import_asset::indexed_png;
use serde_json::Value;
use std::fs;
use std::path::{Path, PathBuf};

pub const ADDRESS: usize = 0x080c2a0a;
pub const END: usize = 0x080c5b30;
pub const SIZE: usize = END - ADDRESS;
const ROM_BASE: usize = 0x08000000;
const KIHON_END: usize = 0x080c3734;
const KOMA_ADDRESS: usize = KIHON_END;
const KOMA_END: usize = 0x080c3f34;
const HAICHI_ADDRESS: usize = KOMA_END;
const HAICHI_END: usize = 0x080c5938;
const HOSEI_ADDRESS: usize = HAICHI_END;
const HOSEI_END: usize = 0x080c5a30;
const GAUGE_ADDRESS: usize = HOSEI_END;

pub type Result<T> = std::result::Result<T, Error>;
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);
impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for Error {}
fn err<T>(s: impl Into<String>) -> Result<T> {
    Err(Error(s.into()))
}
fn object<'a>(v: &'a Value, name: &str) -> Result<&'a serde_json::Map<String, Value>> {
    v.as_object()
        .ok_or_else(|| Error(format!("{name} must be an object")))
}
fn integer(v: Option<&Value>, name: &str, min: i64, max: i64) -> Result<i64> {
    let n = v
        .and_then(Value::as_i64)
        .ok_or_else(|| Error(format!("invalid {name}")))?;
    if n < min || n > max {
        return err(format!("invalid {name}"));
    }
    Ok(n)
}
fn u32v(v: Option<&Value>, name: &str) -> Result<u32> {
    let n = v
        .and_then(Value::as_u64)
        .ok_or_else(|| Error(format!("invalid {name}")))?;
    u32::try_from(n).map_err(|_| Error(format!("invalid {name}")))
}
fn arr<'a>(v: &'a Value, count: usize, name: &str) -> Result<&'a [Value]> {
    let a = v
        .as_array()
        .ok_or_else(|| Error(format!("{name} requires {count} entries")))?;
    if a.len() != count {
        return err(format!("{name} requires {count} entries"));
    }
    Ok(a)
}
fn keys(v: &Value, expected: &[&str], name: &str) -> Result<()> {
    let o = object(v, name)?;
    let mut a: Vec<_> = o.keys().map(String::as_str).collect();
    let mut b = expected.to_vec();
    a.sort_unstable();
    b.sort_unstable();
    if a != b {
        return err(format!("{name} has unknown fields"));
    }
    Ok(())
}
fn value(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    let v: Value =
        serde_json::from_str(&text).map_err(|e| Error(format!("{}: {e}", path.display())))?;
    if v.get("format") != Some(&Value::from(1)) {
        return err(format!("{}: unsupported source format", path.display()));
    }
    if !is_canonical_json_text(&text, &v) {
        return err(format!("{}: source is not canonical JSON", path.display()));
    }
    Ok(v)
}
fn word(out: &mut Vec<u8>, v: Option<&Value>, name: &str) -> Result<()> {
    out.extend_from_slice(&u32v(v, name)?.to_le_bytes());
    Ok(())
}
fn signed_word(out: &mut Vec<u8>, v: Option<&Value>, name: &str) -> Result<()> {
    let n = integer(v, name, -0x80000000, 0x7fffffff)?;
    out.extend_from_slice(&(n as i32).to_le_bytes());
    Ok(())
}
fn byte(v: Option<&Value>, name: &str) -> Result<u8> {
    Ok(integer(v, name, 0, 255)? as u8)
}
fn signed_byte(v: Option<&Value>, name: &str) -> Result<u8> {
    Ok((integer(v, name, -128, 127)? as i8) as u8)
}
fn half(out: &mut Vec<u8>, v: Option<&Value>, name: &str) -> Result<()> {
    out.extend_from_slice(&(integer(v, name, 0, 65535)? as u16).to_le_bytes());
    Ok(())
}
fn extent(v: &Value, address: usize, size: usize, name: &str) -> Result<()> {
    let o = object(v, name)?;
    if o.get("address").and_then(Value::as_str) != Some(&format!("0x{address:08x}"))
        || o.get("size").and_then(Value::as_u64) != Some(size as u64)
    {
        return err(format!("{name} extent differs"));
    }
    Ok(())
}

fn sparse(v: &Value, count: usize, name: &str, allowed: &[u8]) -> Result<Vec<u8>> {
    keys(v, &["count", "default", "entries"], name)?;
    let o = object(v, name)?;
    if o.get("count").and_then(Value::as_u64) != Some(count as u64)
        || o.get("default").and_then(Value::as_u64) != Some(0)
    {
        return err(format!("{name} shape differs"));
    }
    let mut out = vec![0; count];
    let mut previous = None;
    for (i, e) in arr(
        o.get("entries").unwrap(),
        o.get("entries")
            .and_then(Value::as_array)
            .map_or(0, Vec::len),
        name,
    )?
    .iter()
    .enumerate()
    {
        keys(e, &["id", "value"], &format!("{name} entry {i}"))?;
        let id = integer(object(e, "entry")?.get("id"), "ID", 0, (count - 1) as i64)? as usize;
        if previous.is_some_and(|p| id <= p) {
            return err(format!("{name} IDs must be unique and increasing"));
        }
        let val = byte(object(e, "entry")?.get("value"), "value")?;
        if val == 0 || !allowed.contains(&val) {
            return err(format!("{name} value is not a known mode"));
        }
        out[id] = val;
        previous = Some(id);
    }
    Ok(out)
}
fn flags(v: &Value) -> Result<Vec<u8>> {
    keys(v, &["count", "entries"], "display flags")?;
    let o = object(v, "display flags")?;
    if o.get("count").and_then(Value::as_u64) != Some(519) {
        return err("display flag shape differs");
    }
    let mut out = vec![0; 519 * 4];
    let mut previous = None;
    for (i, e) in o
        .get("entries")
        .unwrap()
        .as_array()
        .ok_or_else(|| Error("display flag shape differs".into()))?
        .iter()
        .enumerate()
    {
        keys(
            e,
            &["id", "animation_id", "descriptor_flags"],
            &format!("display flag {i}"),
        )?;
        let eo = object(e, "display flag")?;
        let id = integer(eo.get("id"), "display flag ID", 0, 518)? as usize;
        if previous.is_some_and(|p| id <= p) {
            return err("display flag IDs must be unique and increasing");
        }
        let a = integer(eo.get("animation_id"), "animation ID", 0, 0xfff)? as u32;
        let f = integer(eo.get("descriptor_flags"), "descriptor flags", 0, 0xfffff)? as u32;
        if ![0, 1, 2, 4, 8, 0x40, 0x80, 0x81].contains(&f) {
            return err("display descriptor flags are not recognized");
        }
        out[id * 4..id * 4 + 4].copy_from_slice(&(a + (f << 12)).to_le_bytes());
        previous = Some(id);
    }
    Ok(out)
}
fn pairs(v: &Value, count: usize, name: &str, term: bool) -> Result<Vec<u8>> {
    let a = arr(v, count, name)?;
    let mut out = vec![0; (count + term as usize) * 4];
    let mut ids = Vec::new();
    let mut slots = Vec::new();
    for (i, e) in a.iter().enumerate() {
        keys(e, &["id", "slot"], &format!("{name} {i}"))?;
        let o = object(e, name)?;
        let id = integer(o.get("id"), "ID", 0, 65535)? as u16;
        let slot = integer(o.get("slot"), "slot", 0, 65535)? as u16;
        if id == 65535 && slot == 65535 {
            return err(format!("{name} contains an early terminator"));
        }
        if ids.contains(&id) || slots.contains(&slot) {
            return err(format!("{name} IDs and slots must be unique"));
        }
        ids.push(id);
        slots.push(slot);
        out[i * 4..i * 4 + 2].copy_from_slice(&id.to_le_bytes());
        out[i * 4 + 2..i * 4 + 4].copy_from_slice(&slot.to_le_bytes());
    }
    if term {
        out[count * 4..count * 4 + 4].copy_from_slice(&u32::MAX.to_le_bytes());
    }
    Ok(out)
}

fn build_kihon(path: &Path) -> Result<Vec<u8>> {
    let s = value(path)?;
    keys(
        &s,
        &[
            "format",
            "address",
            "size",
            "alignment",
            "party_capacity",
            "party_order_offsets",
            "actor_pose_rows",
            "party_positions",
            "transform_q16",
            "formula_control",
            "rate_tables",
            "probability_curves",
            "action_modes",
            "action_alignment",
            "action_display",
            "selection_layout",
            "particle_offsets",
            "particle_dimensions",
            "particle_sources",
            "particle_adjustments",
            "short_id_map",
            "long_id_map",
        ],
        "basic battle tables",
    )?;
    extent(&s, ADDRESS, KIHON_END - ADDRESS, "basic battle tables")?;
    let o = object(&s, "basic")?;
    let mut out = vec![0, 0];
    word(&mut out, o.get("party_capacity"), "party capacity")?;
    for (i, x) in arr(
        o.get("party_order_offsets").unwrap(),
        12,
        "party order offsets",
    )?
    .iter()
    .enumerate()
    {
        out.push(signed_byte(Some(x), &format!("party order offset {i}"))?);
    }
    for (i, row) in arr(o.get("actor_pose_rows").unwrap(), 5, "actor pose rows")?
        .iter()
        .enumerate()
    {
        let ro = object(row, "actor pose row")?;
        let actor = [0, 1, 2, 3, 5][i];
        if ro.get("actor").and_then(Value::as_i64) != Some(actor) {
            return err("actor pose row actor differs");
        }
        for (x, p) in arr(ro.get("pose_ids").unwrap(), 7, "pose IDs")?
            .iter()
            .enumerate()
        {
            half(&mut out, Some(p), &format!("pose {i}:{x}"))?;
        }
    }
    for (i, p) in arr(o.get("party_positions").unwrap(), 13, "party positions")?
        .iter()
        .enumerate()
    {
        let po = object(p, "party position")?;
        out.push(signed_byte(po.get("x"), "party x")?);
        out.push(signed_byte(po.get("y"), "party y")?);
    }
    for (i, x) in arr(o.get("transform_q16").unwrap(), 15, "transform")?
        .iter()
        .enumerate()
    {
        signed_word(&mut out, Some(x), &format!("transform {i}"))?;
    }
    for (i, x) in arr(o.get("formula_control").unwrap(), 8, "formula control")?
        .iter()
        .enumerate()
    {
        out.push(byte(Some(x), &format!("formula control {i}"))?);
    }
    for (row, r) in arr(o.get("rate_tables").unwrap(), 8, "rate tables")?
        .iter()
        .enumerate()
    {
        for (col, x) in arr(r, 6, "rate table")?.iter().enumerate() {
            word(&mut out, Some(x), &format!("rate {row}:{col}"))?;
        }
    }
    for (r, row) in arr(
        o.get("probability_curves").unwrap(),
        3,
        "probability curves",
    )?
    .iter()
    .enumerate()
    {
        for (c, x) in arr(row, 8, "probability curve")?.iter().enumerate() {
            out.push(byte(Some(x), &format!("probability {r}:{c}"))?);
        }
    }
    out.extend(sparse(
        o.get("action_modes").unwrap(),
        518,
        "action modes",
        &[2, 3, 5, 6, 8, 9],
    )?);
    out.extend([0, 0]);
    out.extend(flags(o.get("action_display").unwrap())?);
    for (n, x) in arr(
        object(o.get("selection_layout").unwrap(), "selection")?
            .get("defaults")
            .unwrap(),
        2,
        "selection defaults",
    )?
    .iter()
    .enumerate()
    {
        word(&mut out, Some(x), &format!("selection default {n}"))?;
    }
    let sel = object(o.get("selection_layout").unwrap(), "selection")?;
    for (n, x) in arr(sel.get("order").unwrap(), 16, "selection order")?
        .iter()
        .enumerate()
    {
        out.push(byte(Some(x), &format!("selection order {n}"))?);
    }
    for (row, r) in arr(sel.get("matrix_q16").unwrap(), 4, "selection matrix")?
        .iter()
        .enumerate()
    {
        for (col, x) in arr(r, 3, "selection matrix row")?.iter().enumerate() {
            signed_word(&mut out, Some(x), &format!("selection {row}:{col}"))?;
        }
    }
    for (n, x) in arr(o.get("particle_offsets").unwrap(), 7, "particle offsets")?
        .iter()
        .enumerate()
    {
        word(&mut out, Some(x), &format!("particle offset {n}"))?;
    }
    for (n, x) in arr(
        o.get("particle_dimensions").unwrap(),
        7,
        "particle dimensions",
    )?
    .iter()
    .enumerate()
    {
        out.push(byte(Some(x), &format!("particle dimension {n}"))?);
    }
    out.push(0);
    for (n, x) in arr(o.get("particle_sources").unwrap(), 4, "particle sources")?
        .iter()
        .enumerate()
    {
        word(&mut out, Some(x), &format!("particle source {n}"))?;
    }
    for (n, x) in arr(
        o.get("particle_adjustments").unwrap(),
        4,
        "particle adjustments",
    )?
    .iter()
    .enumerate()
    {
        word(&mut out, Some(x), &format!("particle adjustment {n}"))?;
    }
    out.extend(pairs(
        o.get("short_id_map").unwrap(),
        8,
        "short ID map",
        true,
    )?);
    let long = o.get("long_id_map").unwrap();
    for (i, e) in arr(long, 49, "long ID map")?.iter().enumerate() {
        if object(e, "long")?.get("slot").and_then(Value::as_i64) != Some(i as i64) {
            return err("long ID map slots must be sequential");
        }
    }
    out.extend(pairs(long, 49, "long ID map", true)?);
    if out.len() != KIHON_END - ADDRESS {
        return err("basic battle table size differs");
    }
    Ok(out)
}

fn atlas(path: &Path, frames: usize, tw: usize, th: usize, cols: usize) -> Result<Vec<u8>> {
    let data = fs::read(path).map_err(|e| Error(e.to_string()))?;
    let im = indexed_png(&data).map_err(|e| Error(e.0))?;
    let w = tw * 8;
    let h = th * 8;
    if im.width as usize != cols * w || im.height as usize != ((frames + cols - 1) / cols) * h {
        return err(format!("{}: atlas dimensions differ", path.display()));
    }
    let palette: (Vec<[u8; 3]>) = (0..16)
        .map(|i| {
            let v = (i * 8) as u8;
            [v, v, v]
        })
        .collect();
    if im.palette != palette || im.pixels.iter().any(|x| *x >= 16) {
        return err(format!("{}: symbolic palette differs", path.display()));
    }
    let mut out = Vec::with_capacity(frames * tw * th * 32);
    for f in 0..frames {
        let left = (f % cols) * w;
        let top = (f / cols) * h;
        for ty in 0..th {
            for tx in 0..tw {
                for y in 0..8 {
                    for x in (0..8).step_by(2) {
                        let p = (top + ty * 8 + y) * im.width as usize + left + tx * 8 + x;
                        out.push(im.pixels[p] as u8 | (im.pixels[p + 1] as u8) << 4);
                    }
                }
            }
        }
    }
    Ok(out)
}
fn haichi(path: &Path) -> Result<Vec<u8>> {
    let s = value(path)?;
    extent(
        &s,
        HAICHI_ADDRESS,
        HAICHI_END - HAICHI_ADDRESS,
        "formation display table",
    )?;
    let o = object(&s, "formation")?;
    let mut out = Vec::new();
    for (g, group) in arr(o.get("groups").unwrap(), 72, "formation groups")?
        .iter()
        .enumerate()
    {
        let a = group
            .as_array()
            .ok_or_else(|| Error("formation group invalid".into()))?;
        if a.len() > 6 || ((g < 71 && a.is_empty()) || (g == 71 && !a.is_empty())) {
            return err("formation groups require 71 presets and one final terminator");
        }
        for e in a {
            let eo = object(e, "formation entry")?;
            let mut r = vec![0; 20];
            r[0] = integer(eo.get("actor"), "actor", 0, 5)? as u8;
            r[1] = byte(eo.get("level"), "level")?;
            for (i, x) in arr(eo.get("djinn_counts").unwrap(), 4, "Djinn counts")?
                .iter()
                .enumerate()
            {
                r[i + 2] = byte(Some(x), "Djinn count")?;
            }
            for (i, x) in arr(eo.get("equipment").unwrap(), 4, "equipment")?
                .iter()
                .enumerate()
            {
                r[6 + i * 2..8 + i * 2].copy_from_slice(
                    &(integer(Some(x), "equipment", 0, 65535)? as u16).to_le_bytes(),
                );
            }
            for (i, x) in arr(eo.get("abilities").unwrap(), 2, "abilities")?
                .iter()
                .enumerate()
            {
                r[14 + i * 2..16 + i * 2].copy_from_slice(
                    &(integer(Some(x), "ability", 0, 65535)? as u16).to_le_bytes(),
                );
            }
            out.extend(r);
        }
        let mut t = vec![0; 20];
        t[0] = 255;
        out.extend(t);
    }
    if out.len() != HAICHI_END - HAICHI_ADDRESS {
        return err("formation display table size differs");
    }
    Ok(out)
}
fn hosei(path: &Path) -> Result<Vec<u8>> {
    let s = value(path)?;
    extent(
        &s,
        HOSEI_ADDRESS,
        HOSEI_END - HOSEI_ADDRESS,
        "battle correction tables",
    )?;
    let o = object(&s, "correction")?;
    let mut out = Vec::new();
    word(&mut out, o.get("object_descriptor"), "object descriptor")?;
    for e in arr(o.get("weapon_map").unwrap(), 51, "weapon map")? {
        let eo = object(e, "weapon map")?;
        let id = integer(eo.get("item_id"), "item ID", 0, 511)? as u16;
        let c = integer(eo.get("weapon_class"), "weapon class", 0, 6)? as u16;
        out.extend_from_slice(&(id | (c << 9)).to_le_bytes());
    }
    out.extend_from_slice(&u16::MAX.to_le_bytes());
    let curves = object(o.get("object_curves").unwrap(), "object curves")?;
    for n in [
        "field_52_q16",
        "field_48_q16",
        "field_40_q16",
        "effect_percent",
    ] {
        for x in arr(curves.get(n).unwrap(), 8, n)? {
            word(&mut out, Some(x), n)?;
        }
    }
    for x in arr(o.get("control").unwrap(), 3, "control")? {
        word(&mut out, Some(x), "control")?;
    }
    if out.len() != HOSEI_END - HOSEI_ADDRESS {
        return err("battle correction table size differs");
    }
    Ok(out)
}

pub fn build_sentou_hyouji(index_path: &Path) -> Result<Vec<u8>> {
    let i = value(index_path)?;
    keys(
        &i,
        &["format", "kind", "address", "size", "end", "sources"],
        "battle display index",
    )?;
    let o = object(&i, "index")?;
    if o.get("kind").and_then(Value::as_str) != Some("golden-sun-sentou-hyouji") {
        return err("battle display index extent differs");
    }
    let src = object(o.get("sources").unwrap(), "battle display sources")?;
    let dir = index_path.parent().unwrap_or(Path::new("."));
    let prefix = index_path
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or("")
        .strip_suffix("index.json")
        .unwrap_or("");
    let p = |s: &str| dir.join(format!("{prefix}{s}"));
    let mut out = build_kihon(&p(src.get("kihon").and_then(Value::as_str).unwrap()))?;
    out.extend(atlas(
        &p(object(src.get("koma").unwrap(), "koma")?
            .get("source")
            .and_then(Value::as_str)
            .unwrap()),
        16,
        2,
        2,
        8,
    )?);
    out.extend(haichi(&p(src
        .get("haichi")
        .and_then(Value::as_str)
        .unwrap()))?);
    out.extend(hosei(&p(src
        .get("hosei")
        .and_then(Value::as_str)
        .unwrap()))?);
    out.extend(atlas(
        &p(object(src.get("gauge").unwrap(), "gauge")?
            .get("source")
            .and_then(Value::as_str)
            .unwrap()),
        8,
        1,
        1,
        8,
    )?);
    if out.len() != SIZE {
        return err("battle display output size differs");
    }
    Ok(out)
}
pub fn verify_sentou_hyouji(rom: &[u8], index: &Path) -> Result<()> {
    let start = ADDRESS - ROM_BASE;
    let expected = rom
        .get(start..start + SIZE)
        .ok_or_else(|| Error("ROM is too small for battle display data".into()))?;
    let built = build_sentou_hyouji(index)?;
    if built != expected {
        return err("battle display differs from ROM");
    }
    Ok(())
}
pub fn self_test() -> Result<()> {
    let index = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("../../assets/graphics/sentou_hyouji_index.json");
    if index.exists() {
        build_sentou_hyouji(&index)?;
    }
    Ok(())
}
