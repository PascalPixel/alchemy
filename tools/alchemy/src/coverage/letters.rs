//! The lettering of the dashboard and the README figures, cut from the game's
//! own glyph sheets as tracked in the source tree and drawn at one pixel
//! scale. Three faces: the upright menu font (resource 0x13) for every label,
//! the Western dialogue font for quoted game text, and the Japanese editions'
//! dialogue font for Japanese game text. Nothing here reads a ROM or builds
//! a font file.
use psynergy::assets::image::indexed_png;
use serde_json::Value;
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
pub(crate) const MENU: &str = "MENU_GLYPHS.JSON";
pub(crate) const DIALOGUE: &str = "LOCALIZATION_GLYPHS.JSON";
pub(crate) const JAPANESE: &str = "JAPANESE_GLYPHS.JSON";
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

fn read_json(root: &Path, path: &str) -> Result<Value, String> {
    let bytes = std::fs::read(root.join(path)).map_err(|e| format!("{path}: {e}"))?;
    serde_json::from_slice(&bytes).map_err(|e| format!("{path}: {e}"))
}
fn number(value: &Value) -> Option<u32> {
    value.as_u64().map(|n| n as u32)
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
        let manifest = read_json(root, &path)?;
        let glyphs = &manifest["glyphs"];
        let (Some(width), Some(height), Some(columns)) = (
            number(&glyphs["frame_width"]),
            number(&glyphs["frame_height"]),
            number(&glyphs["columns"]),
        ) else {
            return Err(format!("{path} lacks its frame geometry"));
        };
        let source = manifest["components"][0]["source"]
            .as_str()
            .ok_or_else(|| format!("{path} lacks its sheet"))?;
        let rows = frames(root, source, (width, height), columns, 256, |index| {
            index == 1
        })?;
        let widths = &glyphs["advances"];
        let table_path = widths["source"]
            .as_str()
            .ok_or("menu widths lack a source")?;
        let table = read_json(root, table_path)?;
        let values = table
            .pointer(widths["pointer"].as_str().unwrap_or(""))
            .and_then(Value::as_array)
            .ok_or("menu width table lacks values")?;
        let first = number(&widths["first_code"]).unwrap_or(0) as usize;
        let mut advance = vec![0; 256];
        for (offset, value) in values.iter().enumerate() {
            if let Some(slot) = advance.get_mut(first + offset) {
                *slot = number(value).unwrap_or(0);
            }
        }
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
            cell: (width, height),
            top: (LINE - height) / 2,
            rows,
            advance,
            map,
        })
    }
    /// The Western dialogue font, 16x16 frames with a two-byte advance.
    pub(crate) fn dialogue(root: &Path) -> Result<Self, String> {
        let path = sheet(GAMES[0].1, DIALOGUE);
        let table = read_json(root, &path)?;
        let segment = &table["segments"][0];
        let (face, _) = Self::segment(root, segment)?;
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
            let table = read_json(root, &path)?;
            let layouts = read_json(root, &format!("recon/{game}/text.json"))?;
            let characters = layouts
                .as_array()
                .and_then(|all| all.iter().find(|layout| layout["japanese"] == true))
                .and_then(|layout| layout["characters"].as_str())
                .ok_or_else(|| format!("{game} has no Japanese character map"))?
                .chars()
                .collect::<Vec<_>>();
            let mut index = 0;
            for segment in table["segments"].as_array().into_iter().flatten() {
                let (part, count) = Self::segment(root, segment)?;
                for frame in 0..count {
                    let character = characters.get(index + frame).copied();
                    let Some(character) = character.filter(|c| *c != '\u{fffd}') else {
                        continue;
                    };
                    let blank = part.rows[frame].iter().all(|row| *row == 0);
                    if part.advance[frame] == 0 || (blank && character != ' ') {
                        continue;
                    }
                    if face.map.contains_key(&character) {
                        continue;
                    }
                    face.map.insert(character, face.rows.len());
                    face.rows.push(part.rows[frame].clone());
                    face.advance.push(part.advance[frame]);
                }
                index += count;
            }
        }
        if face.rows.is_empty() {
            return Err("no Japanese glyph sheet is tracked".into());
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
    /// One typed-table segment of 16x16 frames: its rows and advances (the
    /// record's own, or the segment's fixed one).
    fn segment(root: &Path, segment: &Value) -> Result<(Self, usize), String> {
        let image = &segment["image"];
        let source = image["source"]
            .as_str()
            .ok_or("glyph table lacks its image")?;
        let (Some(width), Some(height), Some(columns), Some(count)) = (
            number(&image["frame_width"]),
            number(&image["frame_height"]),
            number(&image["columns"]),
            number(&image["frames"]),
        ) else {
            return Err(format!("{source}: glyph table lacks its frame geometry"));
        };
        if (width, height) != (LINE, LINE) {
            return Err(format!(
                "{source}: frames are {width}x{height}, not {LINE}x{LINE}"
            ));
        }
        let fixed = number(&segment["advance"]);
        let advance = segment["records"]
            .as_array()
            .ok_or("glyph table lacks records")?
            .iter()
            .map(|record| number(&record["advance"]).or(fixed).unwrap_or(0))
            .collect::<Vec<_>>();
        let rows = frames(
            root,
            source,
            (width, height),
            columns,
            count as usize,
            |index| index != 0,
        )?;
        Ok((
            Self {
                cell: (width, height),
                top: 0,
                rows,
                advance,
                map: BTreeMap::new(),
            },
            count as usize,
        ))
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
