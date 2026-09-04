//! Weyard font pipeline: extract the text font from all twelve Golden Sun
//! ROMs, write one PNG sheet each, and merge them into a single weyard.otf.
//!
//!   weyard-font
//!
//! Output is ROM-derived and therefore generated, not tracked: it lands in
//! ignored out/fonts/ (or $WEYARD_OUT). Re-run to reproduce every sheet and
//! the font.
//!
//! Two glyph formats, both validated against ROM bytes:
//!   latin  32-byte entries, 16x15px: [advance][0][15 rows x 2 bytes LE, MSB-first]
//!          codes 0x20..0xFF, Latin-1 identity encoding
//!   kana   26-byte entries, 16x13px: [13 rows x 2 bytes LE, MSB-first], fixed width
//!          codes 0x20.., ASCII below 0x80; JIS X 0201 katakana at 0xA1..0xDF;
//!          hiragana mirroring that layout at -0x20 (0x86..0x9F) and +0x20 (0xE0..0xFD)

use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::ExitCode;

const ROM: usize = 0x0800_0000;
const LATIN_ROWS: usize = 15;
const KANA_ROWS: usize = 13;
const LATIN_COUNT: usize = 224; // 0x20..0xFF
const KANA_COUNT: usize = 224; // 0x20..0xFF
const KANJI_COUNT: usize = 114;
const KANJI_ROWS: usize = 12;
const KANJI: &str = "神殿名前中武器長剣発動呪使水火風地毒兄宝石町行炎船海氷道具島男女力土大上玉山気目入口岩天空防母北戦闘回復全体単攻撃守備速敵魔法特別変化状態術者精霊仲間王子村人少年旅立雷竜光闇混乱眠毒石化封印死亡";
const CELL: usize = 16;
const UNITS: u16 = 1024;
const PX: i32 = 64;
const BASELINE: i32 = 11;

#[derive(Clone, Copy, PartialEq, Eq)]
enum Kind {
    Latin,
    Kana,
}

#[derive(Clone)]
struct Glyph {
    advance: usize,
    rows: Vec<u16>,
}

struct Font {
    rom: &'static str,
    kind: Kind,
    base: usize,
    glyphs: Vec<Glyph>,
}

const SOURCES: [(&str, Kind, usize); 12] = [
    ("gs1-ja", Kind::Kana, 0x0803_2470),
    ("gs1-en", Kind::Latin, 0x0803_2224),
    ("gs1-de", Kind::Latin, 0x0803_0fd4),
    ("gs1-es", Kind::Latin, 0x0803_14b4),
    ("gs1-fr", Kind::Latin, 0x0803_12cc),
    ("gs1-it", Kind::Latin, 0x0803_236c),
    ("gs2-ja", Kind::Kana, 0x0805_a8cc),
    ("gs2-en", Kind::Latin, 0x0805_a4e0),
    ("gs2-de", Kind::Latin, 0x0805_a7d4),
    ("gs2-es", Kind::Latin, 0x0805_a760),
    ("gs2-fr", Kind::Latin, 0x0805_a7e8),
    ("gs2-it", Kind::Latin, 0x0805_a6e4),
];

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn extract(rom: &[u8], kind: Kind, base: usize) -> Vec<Glyph> {
    let at = base - ROM;
    let (stride, row_count, count, skip) = match kind {
        Kind::Latin => (32, LATIN_ROWS, LATIN_COUNT, 2),
        Kind::Kana => (26, KANA_ROWS, KANA_COUNT, 0),
    };
    let mut out = Vec::new();
    for g in 0..count {
        let start = at + g * stride;
        let rows: Vec<u16> = (0..row_count)
            .map(|r| {
                let o = start + skip + 2 * r;
                u16::from(rom[o]) | (u16::from(rom[o + 1]) << 8)
            })
            .collect();
        out.push(Glyph {
            advance: if kind == Kind::Latin {
                usize::from(rom[start])
            } else {
                CELL
            },
            rows,
        });
    }
    if kind == Kind::Kana {
        // The kanji block follows the kana block with its own geometry:
        // 114 entries, 24-byte stride, 12 rows. Pad to 13 so sheets stay uniform.
        let kanji_base = at + KANA_COUNT * 26;
        for g in 0..KANJI_COUNT {
            let start = kanji_base + g * 24;
            let mut rows: Vec<u16> = (0..KANJI_ROWS)
                .map(|r| {
                    let o = start + 2 * r;
                    u16::from(rom[o]) | (u16::from(rom[o + 1]) << 8)
                })
                .collect();
            rows.push(0);
            out.push(Glyph {
                advance: CELL,
                rows,
            });
        }
    }
    out
}

// ---------------------------------------------------------------- PNG output
fn crc32(buf: &[u8]) -> u32 {
    let mut table = [0u32; 256];
    for (i, slot) in table.iter_mut().enumerate() {
        let mut c = i as u32;
        for _ in 0..8 {
            c = if c & 1 != 0 {
                0xedb8_8320 ^ (c >> 1)
            } else {
                c >> 1
            };
        }
        *slot = c;
    }
    let mut c = 0xffff_ffffu32;
    for &b in buf {
        c = table[((c ^ u32::from(b)) & 255) as usize] ^ (c >> 8);
    }
    c ^ 0xffff_ffff
}

/// A zlib stream of stored deflate blocks: no compression, no dependency.
fn zlib_stored(raw: &[u8]) -> Vec<u8> {
    let mut out = vec![0x78, 0x01];
    let mut chunks = raw.chunks(65535).peekable();
    if raw.is_empty() {
        out.extend_from_slice(&[1, 0, 0, 0xff, 0xff]);
    }
    while let Some(chunk) = chunks.next() {
        let last = chunks.peek().is_none();
        out.push(u8::from(last));
        let len = chunk.len() as u16;
        out.extend_from_slice(&len.to_le_bytes());
        out.extend_from_slice(&(!len).to_le_bytes());
        out.extend_from_slice(chunk);
    }
    let (mut a, mut b) = (1u32, 0u32);
    for &byte in raw {
        a = (a + u32::from(byte)) % 65521;
        b = (b + a) % 65521;
    }
    out.extend_from_slice(&((b << 16) | a).to_be_bytes());
    out
}

fn write_png(path: &Path, w: usize, h: usize, px: &[u8]) -> Result<(), String> {
    let mut raw = vec![0u8; (w + 1) * h];
    for y in 0..h {
        raw[y * (w + 1) + 1..(y + 1) * (w + 1)].copy_from_slice(&px[y * w..(y + 1) * w]);
    }
    let z = zlib_stored(&raw);
    let chunk = |tag: &[u8; 4], data: &[u8]| -> Vec<u8> {
        let mut body = tag.to_vec();
        body.extend_from_slice(data);
        let mut out = (data.len() as u32).to_be_bytes().to_vec();
        out.extend_from_slice(&body);
        out.extend_from_slice(&crc32(&body).to_be_bytes());
        out
    };
    let mut ihdr = Vec::new();
    ihdr.extend_from_slice(&(w as u32).to_be_bytes());
    ihdr.extend_from_slice(&(h as u32).to_be_bytes());
    ihdr.extend_from_slice(&[8, 0, 0, 0, 0]);
    let mut file = vec![0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];
    file.extend(chunk(b"IHDR", &ihdr));
    file.extend(chunk(b"IDAT", &z));
    file.extend(chunk(b"IEND", &[]));
    fs::write(path, file).map_err(|e| format!("{}: {e}", path.display()))
}

fn sheet(font: &Font, path: &Path, scale: usize) -> Result<(usize, usize), String> {
    let row_count = if font.kind == Kind::Latin {
        LATIN_ROWS
    } else {
        KANA_ROWS
    };
    let cols = 16;
    let rows = font.glyphs.len().div_ceil(cols);
    let cw = CELL + 1;
    let chh = row_count + 1;
    let w = cols * cw * scale;
    let h = rows * chh * scale;
    let mut px = vec![0x1cu8; w * h];
    for (index, glyph) in font.glyphs.iter().enumerate() {
        let gx = (index % cols) * cw;
        let gy = (index / cols) * chh;
        for (r, v) in glyph.rows.iter().enumerate() {
            for x in 0..CELL {
                if (v >> (15 - x)) & 1 != 0 {
                    for sy in 0..scale {
                        for sx in 0..scale {
                            let yy = (gy + r) * scale + sy;
                            let xx = (gx + x) * scale + sx;
                            if yy < h && xx < w {
                                px[yy * w + xx] = 0xff;
                            }
                        }
                    }
                }
            }
        }
    }
    write_png(path, w, h, &px)?;
    Ok((w, h))
}

// ------------------------------------------------------------ JA -> Unicode
fn kana_map() -> BTreeMap<u32, u32> {
    let mut m = BTreeMap::new();
    for c in 0x20..0x80u32 {
        m.insert(c, c);
    }
    m.insert(0x5c, 0x00a5); // JIS X 0201 keeps yen at the backslash slot
    let katakana = "。「」、・ヲァィゥェォャュョッーアイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワン゛゜";
    for (i, ch) in katakana.chars().enumerate() {
        m.insert(0xa1 + i as u32, ch as u32);
    }
    // hiragana mirrors the katakana layout: 0x86..0x9F is katakana-0x20,
    // 0xE0..0xFD is katakana+0x20. Convert katakana -> hiragana where one exists.
    let to_hira = |u: u32| {
        if (0x30a1..=0x30f3).contains(&u) {
            u - 0x60
        } else {
            u
        }
    };
    for c in 0x86..=0x9fu32 {
        if let Some(&k) = m.get(&(c + 0x20)) {
            m.insert(c, to_hira(k));
        }
    }
    for c in 0xe0..=0xfdu32 {
        if let Some(&k) = m.get(&(c - 0x20)) {
            m.insert(c, to_hira(k));
        }
    }
    m
}

// ------------------------------------------------------------- font assembly
fn contours(rows: &[u16]) -> Vec<Vec<(i32, i32)>> {
    let mut shapes = Vec::new();
    for (r, v) in rows.iter().enumerate() {
        let mut x = 0usize;
        while x < CELL {
            if (v >> (15 - x)) & 1 != 0 {
                let start = x;
                while x < CELL && (v >> (15 - x)) & 1 != 0 {
                    x += 1;
                }
                let top = (BASELINE - r as i32) * PX;
                let bottom = top - PX;
                shapes.push(vec![
                    (start as i32 * PX, bottom),
                    (x as i32 * PX, bottom),
                    (x as i32 * PX, top),
                    (start as i32 * PX, top),
                ]);
            } else {
                x += 1;
            }
        }
    }
    shapes
}

fn be16(out: &mut Vec<u8>, v: i32) {
    out.extend_from_slice(&(v as i16).to_be_bytes());
}
fn beu16(out: &mut Vec<u8>, v: u32) {
    out.extend_from_slice(&(v as u16).to_be_bytes());
}
fn beu32(out: &mut Vec<u8>, v: u32) {
    out.extend_from_slice(&v.to_be_bytes());
}

fn glyf_entry(rows: &[u16]) -> Vec<u8> {
    let shapes = contours(rows);
    if shapes.is_empty() {
        return Vec::new();
    }
    let pts: Vec<(i32, i32)> = shapes.iter().flatten().copied().collect();
    let mut out = Vec::new();
    be16(&mut out, shapes.len() as i32);
    be16(&mut out, pts.iter().map(|p| p.0).min().unwrap());
    be16(&mut out, pts.iter().map(|p| p.1).min().unwrap());
    be16(&mut out, pts.iter().map(|p| p.0).max().unwrap());
    be16(&mut out, pts.iter().map(|p| p.1).max().unwrap());
    let mut acc: i32 = -1;
    for s in &shapes {
        acc += s.len() as i32;
        beu16(&mut out, acc as u32);
    }
    beu16(&mut out, 0); // zero instructions
    out.extend(std::iter::repeat(0x01u8).take(pts.len()));
    let (mut px, mut py) = (0, 0);
    for p in &pts {
        be16(&mut out, p.0 - px);
        px = p.0;
    }
    for p in &pts {
        be16(&mut out, p.1 - py);
        py = p.1;
    }
    while out.len() % 4 != 0 {
        out.push(0);
    }
    out
}

fn cmap_table(code_to_index: &BTreeMap<u32, u32>) -> Vec<u8> {
    let codes: Vec<u32> = code_to_index.keys().copied().collect();
    let mut segs: Vec<(u32, u32)> = Vec::new();
    let (mut s, mut p): (Option<u32>, u32) = (None, 0);
    for &c in &codes {
        match s {
            None => {
                s = Some(c);
                p = c;
            }
            Some(_) if c == p + 1 => p = c,
            Some(start) => {
                segs.push((start, p));
                s = Some(c);
                p = c;
            }
        }
    }
    if let Some(start) = s {
        segs.push((start, p));
    }
    segs.push((0xffff, 0xffff));
    let n = segs.len() as u32;
    let (mut search, mut sel) = (1u32, 0u32);
    while search * 2 <= n {
        search *= 2;
        sel += 1;
    }
    let mut sub = Vec::new();
    beu16(&mut sub, 4);
    beu16(&mut sub, 16 + 8 * n);
    beu16(&mut sub, 0);
    beu16(&mut sub, n * 2);
    beu16(&mut sub, search * 2);
    beu16(&mut sub, sel);
    beu16(&mut sub, n * 2 - search * 2);
    for sg in &segs {
        beu16(&mut sub, sg.1);
    }
    beu16(&mut sub, 0);
    for sg in &segs {
        beu16(&mut sub, sg.0);
    }
    for sg in &segs {
        let d = if sg.0 == 0xffff {
            1
        } else {
            (code_to_index[&sg.0].wrapping_sub(sg.0)) & 0xffff
        };
        beu16(&mut sub, d);
    }
    for _ in &segs {
        beu16(&mut sub, 0);
    }
    let mut out = Vec::new();
    beu16(&mut out, 0);
    beu16(&mut out, 1);
    beu16(&mut out, 3);
    beu16(&mut out, 1);
    beu32(&mut out, 12);
    out.extend(sub);
    out
}

fn name_table(strings: &[(u32, &str)]) -> Vec<u8> {
    let mut recs = Vec::new();
    let mut store = Vec::new();
    let mut offset = 0u32;
    for (id, text) in strings {
        let data: Vec<u8> = text.encode_utf16().flat_map(|u| u.to_be_bytes()).collect();
        beu16(&mut recs, 3);
        beu16(&mut recs, 1);
        beu16(&mut recs, 0x409);
        beu16(&mut recs, *id);
        beu16(&mut recs, data.len() as u32);
        beu16(&mut recs, offset);
        offset += data.len() as u32;
        store.extend(data);
    }
    let mut out = Vec::new();
    beu16(&mut out, 0);
    beu16(&mut out, strings.len() as u32);
    beu16(&mut out, 6 + 12 * strings.len() as u32);
    out.extend(recs);
    out.extend(store);
    out
}

fn build_font(glyphs: &BTreeMap<u32, Glyph>, path: &Path) -> Result<(usize, usize), String> {
    let codes: Vec<u32> = glyphs.keys().copied().collect();
    let order: Vec<Option<u32>> = std::iter::once(None)
        .chain(codes.iter().map(|c| Some(*c)))
        .collect();
    let mut code_to_index = BTreeMap::new();
    for (i, c) in order.iter().enumerate() {
        if let Some(c) = c {
            code_to_index.insert(*c, i as u32);
        }
    }
    let mut glyf = Vec::new();
    let mut loca = vec![0u32];
    let mut advances = Vec::new();
    for c in &order {
        let entry = match c {
            None => Vec::new(),
            Some(c) => glyf_entry(&glyphs[c].rows),
        };
        glyf.extend_from_slice(&entry);
        loca.push(glyf.len() as u32);
        advances.push((c.map(|c| glyphs[&c].advance).unwrap_or(8) as i32 * PX) as u32);
    }
    let long_loca = glyf.len() > 0x1fffe;
    let mut loca_buf = Vec::new();
    for v in &loca {
        if long_loca {
            beu32(&mut loca_buf, *v);
        } else {
            beu16(&mut loca_buf, v / 2);
        }
    }
    let num = order.len() as u32;
    let descent = -(LATIN_ROWS as i32 - BASELINE) * PX;

    let mut head = vec![0u8; 54];
    head[0..4].copy_from_slice(&0x0001_0000u32.to_be_bytes());
    head[4..8].copy_from_slice(&0x0001_0000u32.to_be_bytes());
    head[12..16].copy_from_slice(&0x5f0f_3cf5u32.to_be_bytes());
    head[16..18].copy_from_slice(&3u16.to_be_bytes());
    head[18..20].copy_from_slice(&UNITS.to_be_bytes());
    head[38..40].copy_from_slice(&(descent as i16).to_be_bytes());
    head[40..42].copy_from_slice(&((CELL as i32 * PX) as i16).to_be_bytes());
    head[42..44].copy_from_slice(&((BASELINE * PX) as i16).to_be_bytes());
    head[46..48].copy_from_slice(&8u16.to_be_bytes());
    head[48..50].copy_from_slice(&2i16.to_be_bytes());
    head[50..52].copy_from_slice(&(i16::from(long_loca)).to_be_bytes());

    let mut hhea = vec![0u8; 36];
    hhea[0..4].copy_from_slice(&0x0001_0000u32.to_be_bytes());
    hhea[4..6].copy_from_slice(&((BASELINE * PX) as i16).to_be_bytes());
    hhea[6..8].copy_from_slice(&(descent as i16).to_be_bytes());
    hhea[10..12].copy_from_slice(&(*advances.iter().max().unwrap_or(&0) as u16).to_be_bytes());
    hhea[30..32].copy_from_slice(&1i16.to_be_bytes());
    hhea[34..36].copy_from_slice(&(num as u16).to_be_bytes());

    let mut hmtx = Vec::new();
    for a in &advances {
        beu16(&mut hmtx, *a);
        beu16(&mut hmtx, 0);
    }

    let mut maxp = vec![0u8; 32];
    maxp[0..4].copy_from_slice(&0x0001_0000u32.to_be_bytes());
    maxp[4..6].copy_from_slice(&(num as u16).to_be_bytes());
    maxp[6..8].copy_from_slice(&((CELL * LATIN_ROWS * 4) as u16).to_be_bytes());
    maxp[8..10].copy_from_slice(&((CELL * LATIN_ROWS) as u16).to_be_bytes());
    maxp[14..16].copy_from_slice(&2u16.to_be_bytes());

    let mut os2 = vec![0u8; 96];
    os2[0..2].copy_from_slice(&4u16.to_be_bytes());
    os2[2..4].copy_from_slice(&((PX * 8) as i16).to_be_bytes());
    os2[4..6].copy_from_slice(&400u16.to_be_bytes());
    os2[6..8].copy_from_slice(&5u16.to_be_bytes());
    os2[68..70].copy_from_slice(&((PX * 8) as i16).to_be_bytes());
    os2[58..62].copy_from_slice(b"WYRD");
    os2[64..66].copy_from_slice(&(codes[0] as u16).to_be_bytes());
    let last = *codes.last().unwrap();
    os2[66..68].copy_from_slice(&(if last > 0xffff { 0xffff } else { last as u16 }).to_be_bytes());
    os2[68..70].copy_from_slice(&((BASELINE * PX) as i16).to_be_bytes());
    os2[70..72].copy_from_slice(&(descent as i16).to_be_bytes());
    os2[74..76].copy_from_slice(&((BASELINE * PX) as u16).to_be_bytes());
    os2[76..78].copy_from_slice(&(((LATIN_ROWS as i32 - BASELINE) * PX) as u16).to_be_bytes());
    os2[88..90].copy_from_slice(&((PX * 8) as i16).to_be_bytes());
    os2[90..92].copy_from_slice(&((PX * 5) as i16).to_be_bytes());

    let mut post = vec![0u8; 32];
    post[0..4].copy_from_slice(&0x0003_0000u32.to_be_bytes());
    post[12..14].copy_from_slice(&1u16.to_be_bytes());

    let name = name_table(&[
        (0, "Reconstructed from Golden Sun ROM glyph data."),
        (1, "Weyard"),
        (2, "Regular"),
        (3, "Weyard 1.0"),
        (4, "Weyard"),
        (5, "Version 1.0"),
        (6, "Weyard-Regular"),
    ]);

    let mut tables: Vec<(&[u8; 4], Vec<u8>)> = vec![
        (b"OS/2", os2),
        (b"cmap", cmap_table(&code_to_index)),
        (b"glyf", glyf),
        (b"head", head),
        (b"hhea", hhea),
        (b"hmtx", hmtx),
        (b"loca", loca_buf),
        (b"maxp", maxp),
        (b"name", name),
        (b"post", post),
    ];
    tables.sort_by(|a, b| a.0.cmp(b.0));
    let count = tables.len() as u32;
    let (mut search, mut sel) = (1u32, 0u32);
    while search * 2 <= count {
        search *= 2;
        sel += 1;
    }
    let mut header = Vec::new();
    beu32(&mut header, 0x0001_0000);
    beu16(&mut header, count);
    beu16(&mut header, search * 16);
    beu16(&mut header, sel);
    beu16(&mut header, count * 16 - search * 16);
    let mut offset = 12 + 16 * count;
    let mut dir = Vec::new();
    let mut body = Vec::new();
    for (tag, data) in &tables {
        let mut padded = data.clone();
        while padded.len() % 4 != 0 {
            padded.push(0);
        }
        let mut sum = 0u32;
        for word in padded.chunks(4) {
            sum = sum.wrapping_add(u32::from_be_bytes([word[0], word[1], word[2], word[3]]));
        }
        dir.extend_from_slice(*tag);
        beu32(&mut dir, sum);
        beu32(&mut dir, offset);
        beu32(&mut dir, data.len() as u32);
        offset += padded.len() as u32;
        body.extend(padded);
    }
    let mut out = header;
    out.extend(dir);
    out.extend(body);
    let bytes = out.len();
    fs::write(path, out).map_err(|e| format!("{}: {e}", path.display()))?;
    Ok((num as usize, bytes))
}

// --------------------------------------------------------------------- main
fn run() -> Result<(), String> {
    let out_dir = std::env::var("WEYARD_OUT").unwrap_or_else(|_| "out/fonts".into());
    fs::create_dir_all(&out_dir).map_err(|e| format!("{out_dir}: {e}"))?;
    let mut fonts: Vec<Font> = Vec::new();
    println!("rom      kind   base        glyphs  sheet");
    for (rom_name, kind, base) in SOURCES {
        let rom = fs::read(format!("roms/{rom_name}.gba"))
            .map_err(|e| format!("roms/{rom_name}.gba: {e}"))?;
        let glyphs = extract(&rom, kind, base);
        let font = Font {
            rom: rom_name,
            kind,
            base,
            glyphs,
        };
        let path = Path::new(&out_dir).join(format!("{rom_name}.png"));
        let (w, h) = sheet(&font, &path, 3)?;
        println!(
            "{:<8} {:<6} 0x{:x}  {:>4}   {} ({w}x{h})",
            font.rom,
            if kind == Kind::Latin { "latin" } else { "kana" },
            font.base,
            font.glyphs.len(),
            path.display()
        );
        fonts.push(font);
    }

    // Merge. Latin editions vote per Latin-1 code; JA supplies everything else.
    let mut merged: BTreeMap<u32, Glyph> = BTreeMap::new();
    let mut conflicts = 0;
    for code in 0x20..=0xffu32 {
        let mut seen: BTreeMap<Vec<u16>, (usize, Glyph)> = BTreeMap::new();
        for f in fonts.iter().filter(|f| f.kind == Kind::Latin) {
            let Some(g) = f.glyphs.get(code as usize - 0x20) else {
                continue;
            };
            if !g.rows.iter().any(|v| *v != 0) {
                continue;
            }
            seen.entry(g.rows.clone()).or_insert((0, g.clone())).0 += 1;
        }
        if seen.is_empty() {
            continue;
        }
        if seen.len() > 1 {
            conflicts += 1;
        }
        let best = seen.values().max_by_key(|(n, _)| *n).unwrap();
        merged.insert(code, best.1.clone());
    }
    let map = kana_map();
    let ja = fonts
        .iter()
        .find(|f| f.kind == Kind::Kana)
        .ok_or("no kana font")?;
    let mut ja_added = 0;
    let mut kanji = 0;
    let kanji_chars: Vec<char> = KANJI.chars().collect();
    for (index, glyph) in ja.glyphs.iter().enumerate() {
        if !glyph.rows.iter().any(|v| *v != 0) {
            continue;
        }
        let unicode = if index < KANA_COUNT {
            map.get(&(0x20 + index as u32)).copied()
        } else {
            let u = kanji_chars.get(index - KANA_COUNT).map(|c| *c as u32);
            if u.is_some() {
                kanji += 1;
            }
            u
        };
        let Some(unicode) = unicode else { continue };
        if let std::collections::btree_map::Entry::Vacant(slot) = merged.entry(unicode) {
            slot.insert(glyph.clone());
            ja_added += 1;
        }
    }
    let path = Path::new(&out_dir).join("weyard.otf");
    let (glyph_count, bytes) = build_font(&merged, &path)?;
    println!(
        "\nmerged: {} characters (Latin-1 from 10 ROMs, {conflicts} codes where editions differ; {ja_added} Japanese from JA, of which {kanji} kanji)",
        merged.len()
    );
    println!(
        "font:   {}  {glyph_count} glyphs, {bytes} bytes",
        path.display()
    );
    Ok(())
}
