//! The dashboard's game font and tab icons. The two faces come from every
//! registered ROM (see `fonts`); the icons from the tracked icon banks and the
//! ROM palette. All are built into the dashboard cache and served from there;
//! none may ever be committed.
use super::cache::{self, Store};
use super::fonts::{self, Face};
use crate::build_assets::{icon_bank_source, raw_palette_bank, ICON_BANKS, ICON_PALETTE_BANK};
use psynergy::assets::image::{indexed_png, IndexedImage};
use std::{collections::BTreeMap, path::Path, sync::Mutex};

/// Each tab's icon: an icon bank the graphics review identifies and a frame.
pub(super) const TAB_ICONS: [(&str, u8, u32); 5] = [
    ("Files", 4, 148),
    ("ROM coverage", 4, 137),
    ("Music", 5, 53),
    ("Maps", 4, 167),
    ("Text", 4, 176),
];
/// Font units per glyph pixel: a sixteen-pixel em draws each glyph pixel as
/// two CSS pixels at 32px, the dashboard's one text size.
const UNIT: i32 = 64;
const EM: u16 = 1024;
/// Every face shares one baseline: thirteen pixels above, three below.
const ASCENT: i32 = 13;
const DESCENT: i32 = 3;
pub(super) const FAMILY: &str = "Alchemy";

#[derive(Clone, Default)]
pub(super) struct Assets {
    pub font: Option<String>,
    pub italic: Option<String>,
    pub icons: Option<String>,
}
static ASSETS: Mutex<Assets> = Mutex::new(Assets {
    font: None,
    italic: None,
    icons: None,
});
pub(super) fn current() -> Assets {
    ASSETS.lock().unwrap_or_else(|e| e.into_inner()).clone()
}

/// Build (or reuse) the cached files; each failure leaves the others usable.
pub(super) fn refresh(root: &Path) -> Result<String, String> {
    let store = Store::at(root);
    let faces = font_files(root, &store);
    let icons = icon_file(root, &store);
    let mut assets = ASSETS.lock().unwrap_or_else(|e| e.into_inner());
    let (font, italic) = faces.as_ref().ok().cloned().unzip();
    (assets.font, assets.italic) = (font, italic);
    assets.icons = icons.as_ref().ok().cloned();
    match (faces, icons) {
        (Ok(_), Ok(_)) => Ok("fonts and icons cached".into()),
        (Err(error), _) => Err(format!("font: {error}")),
        (_, Err(error)) => Err(format!("icons: {error}")),
    }
}
/// A cached file's bytes by its published name, `font-<stamp>.ttf`,
/// `italic-<stamp>.ttf` or `icons-<stamp>.png`; only current stamps are served.
pub(super) fn file(root: &Path, name: &str) -> Option<(&'static str, Vec<u8>)> {
    let assets = current();
    let (stem, extension) = name.rsplit_once('.')?;
    let (kind, stamp) = stem.split_once('-')?;
    let (current, mime) = match (kind, extension) {
        ("font", "ttf") => (assets.font, "font/ttf"),
        ("italic", "ttf") => (assets.italic, "font/ttf"),
        ("icons", "png") => (assets.icons, "image/png"),
        _ => return None,
    };
    (current.as_deref() == Some(stamp))
        .then(|| Store::at(root).load(kind, stamp, extension))
        .flatten()
        .map(|bytes| (mime, bytes))
}

fn read(root: &Path, path: &str) -> Result<Vec<u8>, String> {
    std::fs::read(root.join(path)).map_err(|error| format!("{path}: {error}"))
}
/// Both faces, stamped by the glyphs they draw rather than by whole ROMs.
fn font_files(root: &Path, store: &Store) -> Result<(String, String), String> {
    let (upright, italic) = fonts::faces(root)?;
    let mut stamps = Vec::new();
    for (kind, face, slanted) in [("font", &upright, false), ("italic", &italic, true)] {
        let stamp = cache::stamp(&[cache::code_identity().as_bytes(), &serialise(face)]);
        store.get_or_build(kind, &stamp, "ttf", || truetype(face, slanted))?;
        stamps.push(stamp);
    }
    Ok((stamps[0].clone(), stamps[1].clone()))
}
fn serialise(face: &Face) -> Vec<u8> {
    let mut out = Vec::new();
    for (character, glyph) in face {
        out.extend((*character as u32).to_le_bytes());
        out.extend([
            glyph.advance as u8,
            glyph.baseline as u8,
            glyph.rows.len() as u8,
        ]);
        out.extend(glyph.rows.iter().flat_map(|row| row.to_le_bytes()));
    }
    out
}
fn icon_file(root: &Path, store: &Store) -> Result<String, String> {
    let target = crate::targets::decomp_target(Some("tbs-en"))?;
    let rom = read(root, target.rom)?;
    let colors = raw_palette_bank(root, &target, &rom, ICON_PALETTE_BANK)?;
    let mut banks = BTreeMap::new();
    for bank in ICON_BANKS {
        banks.insert(bank, read(root, &icon_bank_source(bank))?);
    }
    let palette = colors
        .iter()
        .flat_map(|color| color.to_le_bytes())
        .collect::<Vec<_>>();
    let mut parts = vec![cache::code_identity().into_bytes(), palette];
    parts.extend(banks.values().cloned());
    let stamp = cache::stamp(&parts.iter().map(Vec::as_slice).collect::<Vec<_>>());
    store.get_or_build("icons", &stamp, "png", || {
        let images = banks
            .iter()
            .map(|(bank, bytes)| Ok((*bank, indexed_png(bytes).map_err(|e| e.to_string())?)))
            .collect::<Result<BTreeMap<_, _>, String>>()?;
        icon_strip(&images, &colors)
    })?;
    Ok(stamp)
}

/// One 16x16 frame per tab, index zero transparent, in `TAB_ICONS` order.
fn icon_strip(banks: &BTreeMap<u8, IndexedImage>, colors: &[u16; 16]) -> Result<Vec<u8>, String> {
    let width = 16 * TAB_ICONS.len();
    let mut rgba = vec![0u8; width * 16 * 4];
    for (slot, (_, bank, frame)) in TAB_ICONS.iter().enumerate() {
        let image = banks.get(bank).ok_or("icon bank absent")?;
        let columns = image.width / 16;
        if *frame >= columns * (image.height / 16) {
            return Err(format!("icon {frame} lies outside bank {bank}"));
        }
        let (left, top) = (frame % columns * 16, frame / columns * 16);
        for y in 0..16 {
            for x in 0..16 {
                let index = image.pixels[((top + y) * image.width + left + x) as usize] as usize;
                if index == 0 {
                    continue;
                }
                let color = colors[index & 15];
                let at = ((y as usize) * width + slot * 16 + x as usize) * 4;
                for (channel, shift) in [0, 5, 10].into_iter().enumerate() {
                    let value = (color >> shift) & 31;
                    rgba[at + channel] = ((value << 3) | (value >> 2)) as u8;
                }
                rgba[at + 3] = 255;
            }
        }
    }
    let mut out = Vec::new();
    let mut encoder = png::Encoder::new(&mut out, width as u32, 16);
    encoder.set_color(png::ColorType::Rgba);
    encoder.set_depth(png::BitDepth::Eight);
    encoder
        .write_header()
        .and_then(|mut writer| writer.write_image_data(&rgba))
        .map_err(|e| e.to_string())?;
    Ok(out)
}

fn u16be(out: &mut Vec<u8>, value: i32) {
    out.extend_from_slice(&(value as u16).to_be_bytes());
}
fn u32be(out: &mut Vec<u8>, value: u32) {
    out.extend_from_slice(&value.to_be_bytes());
}
/// Horizontal runs of ink, one rectangle each; `baseline` rows sit above y = 0.
fn outline(rows: &[u16], baseline: i32) -> Vec<[(i32, i32); 4]> {
    let mut boxes = Vec::new();
    for (y, row) in rows.iter().enumerate() {
        let mut x = 0;
        while x < 16 {
            if row >> (15 - x) & 1 == 0 {
                x += 1;
                continue;
            }
            let start = x;
            while x < 16 && row >> (15 - x) & 1 == 1 {
                x += 1;
            }
            let top = (baseline - y as i32) * UNIT;
            let bottom = top - UNIT;
            let (left, right) = (start * UNIT, x * UNIT);
            // Clockwise, as TrueType outlines expect for filled contours.
            boxes.push([(left, bottom), (left, top), (right, top), (right, bottom)]);
        }
    }
    boxes
}
fn glyph_data(boxes: &[[(i32, i32); 4]]) -> Vec<u8> {
    if boxes.is_empty() {
        return Vec::new();
    }
    let points = boxes.iter().flatten().collect::<Vec<_>>();
    let mut out = Vec::new();
    u16be(&mut out, boxes.len() as i32);
    u16be(&mut out, points.iter().map(|p| p.0).min().unwrap());
    u16be(&mut out, points.iter().map(|p| p.1).min().unwrap());
    u16be(&mut out, points.iter().map(|p| p.0).max().unwrap());
    u16be(&mut out, points.iter().map(|p| p.1).max().unwrap());
    for index in 0..boxes.len() {
        u16be(&mut out, (index * 4 + 3) as i32);
    }
    u16be(&mut out, 0);
    out.extend(std::iter::repeat_n(1u8, points.len()));
    for axis in 0..2 {
        let mut previous = 0;
        for point in &points {
            let value = if axis == 0 { point.0 } else { point.1 };
            u16be(&mut out, value - previous);
            previous = value;
        }
    }
    while out.len() % 4 != 0 {
        out.push(0);
    }
    out
}
fn character_map(codes: &[u32]) -> Vec<u8> {
    let mut segments: Vec<(u32, u32, u32)> = Vec::new();
    for (index, code) in codes.iter().enumerate() {
        let glyph = index as u32 + 1;
        match segments.last_mut() {
            Some(last) if last.1 + 1 == *code => last.1 = *code,
            _ => segments.push((*code, *code, glyph)),
        }
    }
    segments.push((0xffff, 0xffff, 0));
    let count = segments.len() as i32;
    let search = 1 << (31 - (count as u32).leading_zeros());
    let mut table = Vec::new();
    for value in [4, 16 + 8 * count, 0, count * 2, search * 2] {
        u16be(&mut table, value);
    }
    u16be(&mut table, search.trailing_zeros() as i32);
    u16be(&mut table, count * 2 - search * 2);
    for segment in &segments {
        u16be(&mut table, segment.1 as i32);
    }
    u16be(&mut table, 0);
    for segment in &segments {
        u16be(&mut table, segment.0 as i32);
    }
    for segment in &segments {
        let delta = if segment.0 == 0xffff {
            1
        } else {
            segment.2.wrapping_sub(segment.0) & 0xffff
        };
        u16be(&mut table, delta as i32);
    }
    for _ in &segments {
        u16be(&mut table, 0);
    }
    let mut out = Vec::new();
    for value in [0, 1, 3, 1] {
        u16be(&mut out, value);
    }
    u32be(&mut out, 12);
    out.extend(table);
    out
}
fn names(slanted: bool) -> Vec<u8> {
    let style = if slanted { "Italic" } else { "Regular" };
    let (full, postscript) = (format!("{FAMILY} {style}"), format!("{FAMILY}-{style}"));
    let strings = [
        (1, FAMILY),
        (2, style),
        (3, "Alchemy dashboard cache"),
        (4, full.as_str()),
        (5, "Version 1.0"),
        (6, postscript.as_str()),
    ];
    let mut records = Vec::new();
    let mut storage = Vec::new();
    for (id, text) in strings {
        let data = text
            .encode_utf16()
            .flat_map(u16::to_be_bytes)
            .collect::<Vec<_>>();
        for value in [3, 1, 0x409, id, data.len() as i32, storage.len() as i32] {
            u16be(&mut records, value);
        }
        storage.extend(data);
    }
    let mut out = Vec::new();
    for value in [0, strings.len() as i32, 6 + 12 * strings.len() as i32] {
        u16be(&mut out, value);
    }
    out.extend(records);
    out.extend(storage);
    out
}
/// A minimal TrueType font whose outlines are the glyph pixels themselves.
fn truetype(glyphs: &Face, slanted: bool) -> Result<Vec<u8>, String> {
    let capital = glyphs.get(&'H').ok_or("face has no H")?;
    let (ascent, descent) = (ASCENT * UNIT, -DESCENT * UNIT);
    let characters = glyphs.keys().copied().collect::<Vec<_>>();
    let codes = characters.iter().map(|c| *c as u32).collect::<Vec<_>>();
    let outlines = characters
        .iter()
        .map(|character| outline(&glyphs[character].rows, glyphs[character].baseline))
        .collect::<Vec<_>>();
    let mut glyf = Vec::new();
    let mut loca = vec![0u32];
    let mut metrics = Vec::new();
    let (mut points, mut contours) = (0, 0);
    for (index, boxes) in std::iter::once(Vec::new()).chain(outlines).enumerate() {
        glyf.extend(glyph_data(&boxes));
        loca.push(glyf.len() as u32);
        points = points.max(boxes.len() * 4);
        contours = contours.max(boxes.len());
        let advance = match index {
            0 => 4 * UNIT,
            _ => glyphs[&characters[index - 1]].advance * UNIT,
        };
        let bearing = boxes.iter().flatten().map(|p| p.0).min().unwrap_or(0);
        u16be(&mut metrics, advance);
        u16be(&mut metrics, bearing);
    }
    let count = codes.len() as i32 + 1;
    let long = glyf.len() > 0x1fffe;
    let mut locations = Vec::new();
    for offset in &loca {
        if long {
            u32be(&mut locations, *offset);
        } else {
            u16be(&mut locations, (*offset / 2) as i32);
        }
    }
    let widest = glyphs.values().map(|g| g.advance).max().unwrap_or(8) * UNIT;
    let mut head = Vec::new();
    u32be(&mut head, 0x0001_0000);
    u32be(&mut head, 0x0001_0000);
    u32be(&mut head, 0);
    u32be(&mut head, 0x5f0f_3cf5);
    u16be(&mut head, 0b1001);
    u16be(&mut head, i32::from(EM));
    head.extend([0; 16]);
    for value in [
        0,
        descent,
        16 * UNIT,
        ascent,
        if slanted { 2 } else { 0 },
        8,
        2,
        i32::from(long),
        0,
    ] {
        u16be(&mut head, value);
    }
    let mut hhea = Vec::new();
    u32be(&mut hhea, 0x0001_0000);
    for value in [
        ascent,
        descent,
        0,
        widest,
        0,
        0,
        16 * UNIT,
        1,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    ] {
        u16be(&mut hhea, value);
    }
    u16be(&mut hhea, count);
    let mut maxp = Vec::new();
    u32be(&mut maxp, 0x0001_0000);
    for value in [
        count,
        points as i32,
        contours as i32,
        0,
        0,
        2,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    ] {
        u16be(&mut maxp, value);
    }
    let mut os2 = Vec::new();
    let average = widest / 2;
    for value in [4, average, 400, 5, 0] {
        u16be(&mut os2, value);
    }
    for value in [
        5 * UNIT,
        5 * UNIT,
        0,
        UNIT,
        5 * UNIT,
        5 * UNIT,
        0,
        3 * UNIT,
        UNIT,
        2 * UNIT,
        0,
    ] {
        u16be(&mut os2, value);
    }
    os2.extend([0; 10]);
    for value in [0b11u32, 0, 0, 0] {
        u32be(&mut os2, value);
    }
    os2.extend(b"ALCH");
    let last = *codes.last().ok_or("glyph table is empty")? as i32;
    for value in [
        if slanted { 1 } else { 0x40 },
        codes[0] as i32,
        last,
        ascent,
        descent,
        0,
        ascent,
        -descent,
    ] {
        u16be(&mut os2, value);
    }
    u32be(&mut os2, 1);
    u32be(&mut os2, 0);
    let capital_height =
        (capital.baseline - capital.rows.iter().position(|r| *r != 0).unwrap() as i32) * UNIT;
    for value in [capital_height * 2 / 3, capital_height, 0, 32, 1] {
        u16be(&mut os2, value);
    }
    let mut post = Vec::new();
    u32be(&mut post, 0x0003_0000);
    u32be(&mut post, 0);
    for value in [-UNIT, UNIT] {
        u16be(&mut post, value);
    }
    post.extend([0; 20]);
    let mut tables: Vec<(&[u8; 4], Vec<u8>)> = vec![
        (b"OS/2", os2),
        (b"cmap", character_map(&codes)),
        (b"glyf", glyf),
        (b"head", head),
        (b"hhea", hhea),
        (b"hmtx", metrics),
        (b"loca", locations),
        (b"maxp", maxp),
        (b"name", names(slanted)),
        (b"post", post),
    ];
    tables.sort_by_key(|(tag, _)| **tag);
    let count = tables.len() as i32;
    let search = 1 << (31 - (count as u32).leading_zeros());
    let mut out = Vec::new();
    u32be(&mut out, 0x0001_0000);
    for value in [
        count,
        search * 16,
        search.trailing_zeros() as i32,
        count * 16 - search * 16,
    ] {
        u16be(&mut out, value);
    }
    let mut offset = 12 + 16 * tables.len() as u32;
    let mut body = Vec::new();
    for (tag, mut data) in tables {
        let length = data.len() as u32;
        while data.len() % 4 != 0 {
            data.push(0);
        }
        let sum = data.chunks_exact(4).fold(0u32, |sum, word| {
            sum.wrapping_add(u32::from_be_bytes([word[0], word[1], word[2], word[3]]))
        });
        out.extend_from_slice(tag);
        u32be(&mut out, sum);
        u32be(&mut out, offset);
        u32be(&mut out, length);
        offset += data.len() as u32;
        body.extend(data);
    }
    out.extend(body);
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;
    /// A face with a space, an H (two stems of seven rows joined by a bar)
    /// and one twelve-row kanji on its own baseline.
    fn face() -> Face {
        let mut h = vec![0x8800u16; 7];
        h[3] = 0xf800;
        h.push(0);
        let glyph = |advance, baseline, rows| fonts::Glyph {
            advance,
            baseline,
            rows,
        };
        Face::from([
            (' ', glyph(4, 7, vec![0; 8])),
            ('H', glyph(6, 7, h)),
            ('神', glyph(12, 11, vec![0xffe0; 12])),
        ])
    }
    #[test]
    fn faces_become_well_formed_truetype_fonts() {
        for slanted in [false, true] {
            let font = truetype(&face(), slanted).unwrap();
            assert_eq!(&font[..4], &[0, 1, 0, 0]);
            let tables = u16::from_be_bytes([font[4], font[5]]) as usize;
            let mut tags = Vec::new();
            for index in 0..tables {
                let record = &font[12 + index * 16..28 + index * 16];
                let offset = u32::from_be_bytes(record[8..12].try_into().unwrap()) as usize;
                let length = u32::from_be_bytes(record[12..16].try_into().unwrap()) as usize;
                assert!(offset % 4 == 0 && offset + length <= font.len());
                tags.push(String::from_utf8(record[..4].to_vec()).unwrap());
            }
            assert_eq!(
                tags,
                ["OS/2", "cmap", "glyf", "head", "hhea", "hmtx", "loca", "maxp", "name", "post"]
            );
        }
        // Six stem rows of two runs and the joined bar: thirteen runs, all above
        // the baseline; the kanji keeps one row below it.
        let face = face();
        let boxes = outline(&face[&'H'].rows, 7);
        assert_eq!(boxes.len(), 13);
        assert!(boxes.iter().flatten().all(|point| point.1 >= 0));
        let kanji = outline(&face[&'神'].rows, 11);
        assert_eq!(kanji.iter().flatten().map(|p| p.1).min(), Some(-UNIT));
    }
    #[test]
    fn cmap_segments_map_consecutive_codes_to_consecutive_glyphs() {
        let map = character_map(&[0x20, 0x21, 0x41]);
        let subtable = &map[12..];
        assert_eq!(u16::from_be_bytes([subtable[6], subtable[7]]), 6);
        let ends = &subtable[14..20];
        assert_eq!(ends, [0x00, 0x21, 0x00, 0x41, 0xff, 0xff]);
    }
    #[test]
    fn served_names_accept_only_current_stamps() {
        let root = tempfile::tempdir().unwrap();
        assert!(file(root.path(), "font-0123456789abcdef.ttf").is_none());
        assert!(file(root.path(), "../font-x.ttf").is_none());
        assert!(file(root.path(), "icons.png").is_none());
    }
}
