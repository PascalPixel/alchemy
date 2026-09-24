//! The one lettering of the dashboard and the README figures: the game's
//! Western glyph sheet as tracked in the source tree, drawn at one pixel
//! scale. Nothing here reads a ROM or builds a font file.
use psynergy::assets::image::indexed_png;
use serde_json::Value;
use std::path::Path;

/// CSS pixels per game pixel, the one scale of the dashboard and the figures:
/// a game pixel is one CSS pixel, so a retina display draws it as two device
/// pixels. Every length either draws is a whole number of game pixels.
pub(crate) const PIXEL: u32 = 1;
/// Device pixels per game pixel in the README figures, drawn for retina
/// displays and shown at half their width.
pub(crate) const FIGURE_SCALE: u32 = 2;
/// A glyph frame and a text line, in game pixels.
pub(crate) const LINE: u32 = 16;
/// The tracked sheet's record table; its image is named inside it.
pub(crate) const SHEET: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS/FONT/LOCALIZATION_GLYPHS.JSON";
const FIRST: u32 = 0x20;

/// Glyphs for codes 0x20..=0xFF, each sixteen rows of sixteen columns with
/// the most significant bit leftmost, and each code's advance.
#[derive(Clone, PartialEq, Debug)]
pub(crate) struct Letters {
    pub rows: Vec<[u16; 16]>,
    pub advance: Vec<u32>,
}

impl Letters {
    pub(crate) fn load(root: &Path) -> Result<Self, String> {
        let text = std::fs::read(root.join(SHEET)).map_err(|e| format!("{SHEET}: {e}"))?;
        let table: Value = serde_json::from_slice(&text).map_err(|e| format!("{SHEET}: {e}"))?;
        let segment = &table["segments"][0];
        let image = &segment["image"];
        let source = image["source"]
            .as_str()
            .ok_or("glyph sheet lacks its image")?;
        let number = |value: &Value| value.as_u64().map(|n| n as u32);
        let (Some(width), Some(height), Some(columns)) = (
            number(&image["frame_width"]),
            number(&image["frame_height"]),
            number(&image["columns"]),
        ) else {
            return Err("glyph sheet lacks its frame geometry".into());
        };
        if (width, height) != (LINE, LINE) {
            return Err(format!(
                "glyph frames are {width}x{height}, not {LINE}x{LINE}"
            ));
        }
        let advance = segment["records"]
            .as_array()
            .ok_or("glyph sheet lacks records")?
            .iter()
            .map(|record| number(&record["advance"]).ok_or("record lacks an advance"))
            .collect::<Result<Vec<_>, _>>()?;
        let bytes = std::fs::read(root.join(source)).map_err(|e| format!("{source}: {e}"))?;
        let png = indexed_png(&bytes).map_err(|e| format!("{source}: {e}"))?;
        let rows = (0..advance.len() as u32)
            .map(|frame| {
                let (left, top) = (frame % columns * LINE, frame / columns * LINE);
                let mut rows = [0u16; 16];
                for (y, row) in rows.iter_mut().enumerate() {
                    for x in 0..LINE {
                        let at = (top + y as u32) * png.width + left + x;
                        if png.pixels.get(at as usize).is_some_and(|index| *index != 0) {
                            *row |= 0x8000 >> x;
                        }
                    }
                }
                rows
            })
            .collect();
        Ok(Self { rows, advance })
    }
    /// A character's frame, when the sheet draws it: Latin-1 codes, except
    /// 0x7F..=0x9F, which hold the game's own button and ornament symbols.
    pub(crate) fn frame(&self, character: char) -> Option<usize> {
        let code = character as u32;
        if !(FIRST..=0xff).contains(&code) || (0x7f..0xa0).contains(&code) {
            return None;
        }
        let frame = (code - FIRST) as usize;
        (self.advance.get(frame).copied().unwrap_or(0) > 0).then_some(frame)
    }
    /// A string's width in game pixels; a character the sheet lacks takes a
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
        x < LINE && y < LINE && self.rows[frame][y as usize] & (0x8000 >> x) != 0
    }
}

#[cfg(test)]
pub(crate) fn fixture() -> Letters {
    // A three-pixel square for every code, advancing four.
    let mut rows = [0u16; 16];
    for row in &mut rows[8..11] {
        *row = 0xe000;
    }
    Letters {
        rows: vec![rows; 224],
        advance: vec![4; 224],
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn the_tracked_sheet_draws_latin_one_but_not_the_game_symbols() {
        let root = crate::coverage::tree::root();
        let letters = Letters::load(&root).unwrap();
        assert_eq!(letters.advance.len(), 224);
        let h = letters.frame('H').unwrap();
        assert!((0..LINE).any(|y| letters.ink(h, 0, y) || letters.ink(h, 1, y)));
        assert!(letters.frame('é').is_some());
        assert_eq!(letters.frame('\u{8c}'), None);
        assert_eq!(letters.frame('神'), None);
        assert_eq!(letters.width("—"), letters.width("?"));
    }
}
