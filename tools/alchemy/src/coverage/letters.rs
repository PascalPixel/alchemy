//! The lettering of the dashboard and the README figures, cut from the game's
//! own editable PNG sheets. Advances come from the verified local ROM's
//! font records; no generated glyph catalog or font file is tracked.
use psynergy::assets::image::indexed_png;
use std::collections::BTreeMap;
use std::path::Path;
use unicode_normalization::UnicodeNormalization;

/// CSS pixels per game pixel, the one scale of the dashboard and the figures:
/// a game pixel is one CSS pixel, so a retina display draws it as two device
/// pixels. Every length either draws is a whole number of game pixels.
pub(crate) const PIXEL: u32 = 1;
/// Device pixels per game pixel in the README figures, drawn for retina
/// displays and shown at half their width.
pub(crate) const FIGURE_SCALE: u32 = 2;
/// A text line, in game pixels: the dialogue font's cell, and the box the
/// eight-pixel menu font sits centred in.
pub(crate) const LINE: u32 = 16;

const FONT: &str = "SRC/GRAPHICS/FONT";
const GAMES: [(&str, &str); 2] = [("tbs", "THE BROKEN SEAL"), ("tla", "THE LOST AGE")];
/// The sheets each face is cut from, relative to a game folder.
pub(crate) const MENU: &str = "MENU_GLYPHS_0000_00FF.4BPP.PNG";
pub(crate) const DIALOGUE: &str = "LOCALIZATION_GLYPHS_0020_00FF.1BPP.PNG";
pub(crate) const JAPANESE: &str = "JAPANESE_GLYPHS_0020_00FF.1BPP.PNG";
/// Columns a voicing mark moves right to sit at a kana's upper right.
const VOICING_SHIFT: u32 = 5;

pub(crate) fn sheet(game: &str, name: &str) -> String {
    format!("games/{game}/{FONT}/{name}")
}

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
/// The indexed pixels of a sheet's frames, `cell` wide and tall, `columns`
/// to a row.
fn frames(
    root: &Path,
    source: &str,
    cell: (u32, u32),
    columns: u32,
    count: usize,
    ink: impl Fn(u32) -> bool,
) -> Result<Vec<Vec<u16>>, String> {
    let bytes = std::fs::read(root.join(source)).map_err(|e| format!("{source}: {e}"))?;
    let png = indexed_png(&bytes).map_err(|e| format!("{source}: {e}"))?;
    if columns == 0
        || cell.0 == 0
        || cell.0 > 16
        || cell.1 == 0
        || png.width != columns * cell.0
        || png.height < (count as u32).div_ceil(columns) * cell.1
    {
        return Err(format!(
            "{source}: glyph sheet does not fit its frame geometry"
        ));
    }
    Ok((0..count as u32)
        .map(|frame| {
            let (left, top) = (frame % columns * cell.0, frame / columns * cell.1);
            (0..cell.1)
                .map(|y| {
                    (0..cell.0).fold(0u16, |row, x| {
                        let at = (top + y) * png.width + left + x;
                        match png.pixels.get(at as usize) {
                            Some(index) if ink(*index) => row | 0x8000 >> x,
                            _ => row,
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
        let path = sheet(GAMES[0].1, MENU);
        let rows = frames(root, &path, (8, 8), 16, 256, |index| index == 1)?;
        let mut advance = vec![0; 256];
        advance[0x20..].copy_from_slice(&advances(
            &reference(root, "tbs-en")?,
            0x0803_70d4,
            224,
            1,
            false,
        )?);
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
    /// The Western dialogue font, 16x16 frames with a two-byte advance.
    pub(crate) fn dialogue(root: &Path) -> Result<Self, String> {
        let path = sheet(GAMES[0].1, DIALOGUE);
        let rows = frames(root, &path, (LINE, LINE), 16, 224, |index| index != 0)?;
        // Western records are a little-endian advance and fifteen 16-bit rows.
        let advance = advances(&reference(root, "tbs-en")?, 0x0803_2224, 224, 32, true)?;
        let face = Self {
            cell: (LINE, LINE),
            top: 0,
            rows,
            advance,
            map: BTreeMap::new(),
        };
        let mut map = BTreeMap::new();
        for frame in 0..face.rows.len() {
            if let Some(character) = western(frame + 0x20) {
                if face.advance[frame] > 0 {
                    map.insert(character, frame);
                }
            }
        }
        Ok(Self { map, ..face })
    }
    /// The Japanese editions' dialogue font: the single-byte block and each
    /// game's extended kanji, by the characters its text catalog decodes.
    pub(crate) fn japanese(root: &Path) -> Result<Self, String> {
        let mut face = Self {
            cell: (LINE, LINE),
            top: 0,
            rows: Vec::new(),
            advance: Vec::new(),
            map: BTreeMap::new(),
        };
        for (game, folder) in GAMES {
            let path = sheet(folder, JAPANESE);
            if !root.join(&path).exists() {
                continue;
            }
            let target = format!("{game}-ja");
            let characters = crate::text_catalog::ARCHIVES
                .iter()
                .find(|spec| spec.target == target)
                .and_then(|spec| spec.characters)
                .ok_or_else(|| format!("{game} has no Japanese character map"))?
                .chars()
                .collect::<Vec<_>>();
            let (address, extended, count) = if game == "tbs" {
                (0x0803_2470, "JAPANESE_GLYPHS_0100_0171.1BPP.PNG", 114)
            } else {
                (0x0805_a8cc, "JAPANESE_GLYPHS_0100_0197.1BPP.PNG", 152)
            };
            // Japanese base records contain an advance and twelve 16-bit rows.
            let base_advance = advances(&reference(root, &target)?, address, 224, 26, true)?;
            let mut index = 0;
            for (source, count, advance) in [
                (path, 224, base_advance),
                (sheet(folder, extended), count, vec![12; count]),
            ] {
                let rows = frames(root, &source, (LINE, LINE), 16, count, |index| index != 0)?;
                for frame in 0..count {
                    let character = characters.get(index + frame).copied();
                    let Some(character) = character.filter(|c| *c != '\u{fffd}') else {
                        continue;
                    };
                    let blank = rows[frame].iter().all(|row| *row == 0);
                    if advance[frame] == 0 || (blank && character != ' ') {
                        continue;
                    }
                    if face.map.contains_key(&character) {
                        continue;
                    }
                    face.map.insert(character, face.rows.len());
                    face.rows.push(rows[frame].clone());
                    face.advance.push(advance[frame]);
                }
                index += count;
            }
        }
        if face.rows.is_empty() {
            return Err("no Japanese glyph PNG sheet is available".into());
        }
        // The fonts hold no voiced kana: the game prints the base kana and a
        // voicing mark (codes 0xDE, 0xDF) at its upper right. Each precomposed
        // kana the catalog decodes is drawn the same way, as one frame.
        for composed in '\u{3040}'..='\u{30ff}' {
            if face.map.contains_key(&composed) {
                continue;
            }
            let parts = std::iter::once(composed).nfd().collect::<Vec<_>>();
            let [base, mark] = parts[..] else {
                continue;
            };
            let (Some(base), Some(mark)) = (face.frame(base), face.frame(mark)) else {
                continue;
            };
            let rows = face.rows[base]
                .iter()
                .zip(&face.rows[mark])
                .map(|(glyph, voicing)| glyph | voicing >> VOICING_SHIFT)
                .collect();
            face.map.insert(composed, face.rows.len());
            face.rows.push(rows);
            face.advance.push(face.advance[base]);
        }
        Ok(face)
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
    fn the_tracked_sheets_draw_their_faces() {
        let root = crate::coverage::tree::root();
        let menu = Letters::menu(&root).unwrap();
        assert_eq!((menu.cell, menu.top, menu.rows.len()), ((8, 8), 4, 256));
        let h = menu.frame('H').unwrap();
        assert_eq!(h, 0x48);
        assert!((0..8).any(|y| menu.ink(h, 0, y) || menu.ink(h, 1, y)));
        assert!(menu.frame('é').is_some() && menu.frame('\\').is_none());
        assert_eq!(menu.frame('\u{8c}'), None);
        assert_eq!(menu.width("—"), menu.width("?"));
        let dialogue = Letters::dialogue(&root).unwrap();
        assert_eq!((dialogue.cell, dialogue.rows.len()), ((16, 16), 224));
        assert!(dialogue.frame('é').is_some() && dialogue.frame('神').is_none());
        let japanese = Letters::japanese(&root).unwrap();
        for character in ['あ', 'ア', '神', '殿', 'A', 'だ', 'パ'] {
            assert!(japanese.frame(character).is_some(), "{character}");
        }
        // Kanji advance twelve pixels; The Lost Age adds its own.
        assert_eq!(japanese.advance[japanese.frame('神').unwrap()], 12);
        assert!(japanese.frame('黄').is_some());
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
