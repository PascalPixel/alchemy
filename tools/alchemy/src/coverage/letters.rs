//! The lettering of the README figures, read from the verified local ROM:
//! the menu font's tiles and its advances. No glyph sheet, generated glyph
//! catalog or font file is tracked.
use std::collections::BTreeMap;
use std::path::Path;

/// Device pixels per game pixel in the README figures, drawn for retina
/// displays and shown at half their width.
pub(crate) const FIGURE_SCALE: u32 = 2;
/// A text line, in game pixels: the dialogue font's cell, and the box the
/// eight-pixel menu font sits centred in.
pub(crate) const LINE: u32 = 16;

/// The upright menu font's 256 4bpp tiles in The Broken Seal's English ROM,
/// by character code.
const MENU_TILES: u32 = 0x0832_0fb0;

/// One face: frames of `cell` game pixels (rows most significant bit
/// leftmost), each frame's advance, and the characters it draws.
#[derive(Clone, PartialEq, Debug)]
pub(crate) struct Letters {
    pub cell: (u32, u32),
    /// Rows from the top of a `LINE` box to the top of a cell.
    pub top: u32,
    pub rows: Vec<Vec<u16>>,
    pub advance: Vec<u32>,
    pub map: BTreeMap<char, usize>,
}

fn reference(root: &Path, target: &str) -> Result<Vec<u8>, String> {
    let spec = crate::text_catalog::ARCHIVES
        .iter()
        .find(|spec| spec.target == target)
        .ok_or_else(|| format!("unknown font edition {target}"))?;
    let bytes =
        std::fs::read(root.join(spec.rom)).map_err(|error| format!("{}: {error}", spec.rom))?;
    crate::text_catalog::verify_reference(root, target, &bytes)?;
    Ok(bytes)
}

fn advances(
    rom: &[u8],
    address: u32,
    count: usize,
    stride: usize,
    wide: bool,
) -> Result<Vec<u32>, String> {
    let start = address
        .checked_sub(0x0800_0000)
        .ok_or("font advances precede the ROM")? as usize;
    (0..count)
        .map(|index| {
            let offset = start
                .checked_add(index.checked_mul(stride).ok_or("font record overflow")?)
                .ok_or("font record overflow")?;
            let low = *rom.get(offset).ok_or("font advance exceeds the ROM")?;
            if wide {
                let high = *rom.get(offset + 1).ok_or("font advance exceeds the ROM")?;
                Ok(u16::from_le_bytes([low, high]) as u32)
            } else {
                Ok(low as u32)
            }
        })
        .collect()
}
/// `count` 8x8 4bpp tiles at `address` as rows of ink bits, most
/// significant bit leftmost; each byte holds two pixels, low nibble first.
fn tiles(
    rom: &[u8],
    address: u32,
    count: usize,
    ink: impl Fn(u8) -> bool,
) -> Result<Vec<Vec<u16>>, String> {
    let start = address
        .checked_sub(0x0800_0000)
        .ok_or("font tiles precede the ROM")? as usize;
    let bytes = start
        .checked_add(count * 32)
        .and_then(|end| rom.get(start..end))
        .ok_or("font tiles exceed the ROM")?;
    Ok(bytes
        .chunks(32)
        .map(|tile| {
            tile.chunks(4)
                .map(|row| {
                    (0..8).fold(0u16, |bits, x| {
                        if ink(row[x / 2] >> (x % 2 * 4) & 15) {
                            bits | 0x8000 >> x
                        } else {
                            bits
                        }
                    })
                })
                .collect()
        })
        .collect())
}
/// A Western code's character; 0x7F–0x9F hold the game's own button and
/// ornament symbols, not Latin-1, so they are left out.
fn western(code: usize) -> Option<char> {
    (!(0x7f..0xa0).contains(&code))
        .then(|| char::from_u32(code as u32))
        .flatten()
}

impl Letters {
    /// The upright menu font: 256 8x8 tiles by character code, ink in
    /// colour 1, advances from the menu text path's width table.
    pub(crate) fn menu(root: &Path) -> Result<Self, String> {
        let rom = reference(root, "tbs-en")?;
        let rows = tiles(&rom, MENU_TILES, 256, |index| index == 1)?;
        let mut advance = vec![0; 256];
        advance[0x20..].copy_from_slice(&advances(&rom, 0x0803_70d4, 224, 1, false)?);
        let mut map = BTreeMap::new();
        for (code, rows) in rows.iter().enumerate() {
            let blank = rows.iter().all(|row| *row == 0);
            let Some(character) = western(code).filter(|_| code != 0x5c) else {
                continue;
            };
            if advance[code] > 1 && (!blank || character == ' ') {
                map.insert(character, code);
            }
        }
        // The yen sign takes the backslash's code, as on Japanese keyboards.
        if !map.contains_key(&'¥') && advance[0x5c] > 1 {
            map.insert('¥', 0x5c);
        }
        Ok(Self {
            cell: (8, 8),
            top: 4,
            rows,
            advance,
            map,
        })
    }
    /// A character's frame, when the face draws it.
    pub(crate) fn frame(&self, character: char) -> Option<usize> {
        self.map.get(&character).copied()
    }
    /// A string's width in game pixels; a character the face lacks takes a
    /// question mark's place.
    pub(crate) fn width(&self, text: &str) -> u32 {
        text.chars().map(|c| self.advance[self.stand_in(c)]).sum()
    }
    pub(crate) fn stand_in(&self, character: char) -> usize {
        self.frame(character)
            .or_else(|| self.frame('?'))
            .unwrap_or(0)
    }
    pub(crate) fn ink(&self, frame: usize, x: u32, y: u32) -> bool {
        x < self.cell.0 && y < self.cell.1 && self.rows[frame][y as usize] & (0x8000 >> x) != 0
    }
}

/// Weyard UI's three logo marks: Alchemy's mountain, The Broken Seal's sun
/// and The Lost Age's anchor. No tracked item, Psynergy or status icon draws
/// them cleanly, so they are drawn here as one-colour masks on a `LINE` box,
/// `#` ink, and take the labels' one-pixel shadow wherever they are shown.
pub(crate) const MARKS: [(&str, [&str; LINE as usize]); 3] = [
    (
        "alchemy",
        [
            "................",
            "................",
            "......#.........",
            "......#.........",
            ".....###........",
            ".....###........",
            "....#.#.#.......",
            "....#####.......",
            "...#######.#....",
            "...##########...",
            "..############..",
            "..#############.",
            ".##############.",
            ".##############.",
            "###############.",
            "................",
        ],
    ),
    (
        "tbs",
        [
            "................",
            "................",
            "......#.........",
            ".#....#....#....",
            "..#.......#.....",
            ".....###........",
            "....#####.......",
            "...#######......",
            "##.#######.##...",
            "...#######......",
            "....#####.......",
            ".....###........",
            "..#.......#.....",
            ".#....#....#....",
            "......#.........",
            "................",
        ],
    ),
    (
        "tla",
        [
            "................",
            ".....###........",
            "....#...#.......",
            ".....###........",
            "......#.........",
            "..#########.....",
            "......#.........",
            "......#.........",
            "......#.........",
            "......#.........",
            "#.....#.....#...",
            "##....#....##...",
            ".##...#...##....",
            "..###.#.###.....",
            "....#####.......",
            "................",
        ],
    ),
];
/// A mark's rows, by name.
pub(crate) fn mark(name: &str) -> &'static [&'static str; LINE as usize] {
    &MARKS
        .iter()
        .find(|(key, _)| *key == name)
        .expect("a Weyard UI mark")
        .1
}
/// A mark's width: its rightmost ink column and one more.
pub(crate) fn mark_width(name: &str) -> u32 {
    mark(name)
        .iter()
        .filter_map(|row| row.rfind('#'))
        .max()
        .map_or(0, |column| column as u32 + 1)
}

#[cfg(test)]
pub(crate) fn fixture() -> Letters {
    // A three-pixel square for every Latin-1 code, advancing four.
    let mut rows = vec![0u16; 16];
    for row in &mut rows[8..11] {
        *row = 0xe000;
    }
    Letters {
        cell: (16, 16),
        top: 0,
        rows: vec![rows; 224],
        advance: vec![4; 224],
        map: (0x20..0x100)
            .filter_map(|code| Some((western(code)?, code - 0x20)))
            .collect(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn font_advance_records_keep_stride_and_little_endian_widths() {
        let data = [6, 1, 99, 99, 12, 2];
        assert_eq!(
            advances(&data, 0x0800_0000, 2, 4, true).unwrap(),
            [262, 524]
        );
        assert_eq!(advances(&data, 0x0800_0000, 2, 4, false).unwrap(), [6, 12]);
        assert!(advances(&data, 0x0800_0000, 3, 4, true).is_err());
        assert!(advances(&data, 0x0700_0000, 1, 4, false).is_err());
    }
    #[test]
    fn font_tiles_read_low_nibble_first_and_stay_inside_the_rom() {
        let mut rom = vec![0u8; 32];
        rom[0] = 0x01;
        rom[4] = 0x10;
        rom[31] = 0x11;
        let rows = tiles(&rom, 0x0800_0000, 1, |index| index == 1).unwrap();
        assert_eq!(rows[0][0], 0x8000);
        assert_eq!(rows[0][1], 0x4000);
        assert_eq!(rows[0][7], 0x0300);
        assert!(tiles(&rom, 0x0800_0000, 2, |_| true).is_err());
        assert!(tiles(&rom, 0x0700_0000, 1, |_| true).is_err());
    }
    #[test]
    fn the_rom_font_draws_its_face() {
        let root = crate::coverage::tree::root();
        let menu = Letters::menu(&root).unwrap();
        assert_eq!((menu.cell, menu.top, menu.rows.len()), ((8, 8), 4, 256));
        let h = menu.frame('H').unwrap();
        assert_eq!(h, 0x48);
        assert!((0..8).any(|y| menu.ink(h, 0, y) || menu.ink(h, 1, y)));
        assert!(menu.frame('é').is_some() && menu.frame('\\').is_none());
        assert_eq!(menu.frame('\u{8c}'), None);
        assert_eq!(menu.width("—"), menu.width("?"));
    }
    #[test]
    fn the_marks_are_one_colour_masks_on_a_line_box() {
        for (name, rows) in MARKS {
            assert!(
                rows.iter()
                    .all(|row| row.len() == LINE as usize
                        && row.chars().all(|c| c == '.' || c == '#')),
                "{name}"
            );
            // Each keeps its last column clear for the shadow.
            assert!(mark_width(name) < LINE, "{name}");
        }
        assert_eq!(
            (mark_width("alchemy"), mark_width("tbs"), mark_width("tla")),
            (15, 13, 13)
        );
    }
}
