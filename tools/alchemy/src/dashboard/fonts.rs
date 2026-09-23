//! The game's two faces, gathered from every registered ROM for the dashboard
//! cache. The upright face is the Western menu font (8x8 tiles of resource
//! 0x13) with the Japanese 12-row font for kana, kanji and full-width Latin;
//! the italic face is the Western 15-row dialogue font with the same Japanese
//! glyphs, which the games never slant. Nothing here is ever committed.
use crate::overlay::rom::CanonicalRom;
use crate::targets::decomp_target;
use encoding_rs::WINDOWS_1252;
use serde_json::Value;
use std::{collections::BTreeMap, path::Path};

/// One glyph: up to sixteen columns per row, most significant bit leftmost,
/// with `baseline` rows above the baseline.
#[derive(Clone, PartialEq, Debug)]
pub(super) struct Glyph {
    pub advance: i32,
    pub baseline: i32,
    pub rows: Vec<u16>,
}
pub(super) type Face = BTreeMap<char, Glyph>;

const WESTERN: [&str; 5] = ["en", "de", "es", "fr", "it"];
const GAMES: [(&str, &str); 2] = [("tbs", "THE BROKEN SEAL"), ("tla", "THE LOST AGE")];
/// Resource 0x13: 256 4bpp 8x8 tiles indexed by character code, ink in
/// colour 1 and its shadow one pixel down and right in colour 3.
const MENU_FONT_RESOURCE: usize = 0x13;
const TILE_BYTES: usize = 32;
/// The menu text path's advance for each code from 0x20.
const MENU_WIDTHS: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS/TEXT/TABLES.JSON";
const MENU_WIDTHS_KEY: &str = "0x080370d4";
/// Western dialogue records: a two-byte advance and fifteen rows.
const DIALOGUE_RECORD: usize = 32;
/// Japanese records: single-byte codes carry a two-byte advance and twelve
/// rows; extended codes from 0x100 are twelve rows twelve pixels apart.
const JAPANESE_RECORD: usize = 26;
const EXTENDED_RECORD: usize = 24;
const EXTENDED_ADVANCE: i32 = 12;

/// Both faces, from whichever registered ROMs are present; English first,
/// so an edition only adds codes the earlier ones lack.
pub(super) fn faces(root: &Path) -> Result<(Face, Face), String> {
    let widths = menu_widths(root)?;
    let (mut upright, mut italic) = (Face::new(), Face::new());
    for (game, folder) in GAMES {
        let english =
            load(root, &format!("{game}-en"))?.ok_or_else(|| format!("{game}-en ROM is absent"))?;
        let table = format!("games/{folder}/SRC/GRAPHICS/FONT/LOCALIZATION_GLYPHS.JSON");
        let document = json(root, &table)?;
        let start = hex(&document["address"])? - 0x0800_0000;
        for edition in WESTERN {
            let Some(rom) = load(root, &format!("{game}-{edition}"))? else {
                continue;
            };
            let at = if edition == "en" {
                Some(start)
            } else {
                locate(
                    rom.bytes(),
                    &english.bytes()[start..start + 224 * DIALOGUE_RECORD],
                )
            };
            if let Some(at) = at {
                merge(
                    &mut italic,
                    dialogue(&rom.bytes()[at..at + 224 * DIALOGUE_RECORD]),
                );
            }
            let tiles = rom.resource_pointer(MENU_FONT_RESOURCE)?;
            let tiles = rom
                .bytes()
                .get(tiles..tiles + 256 * TILE_BYTES)
                .ok_or("menu font lies outside the ROM")?;
            merge(&mut upright, menu(tiles, &widths));
        }
    }
    let japanese = japanese(root)?;
    merge(&mut upright, japanese.clone());
    merge(&mut italic, japanese);
    Ok((upright, italic))
}

fn load(root: &Path, id: &str) -> Result<Option<CanonicalRom>, String> {
    let target = decomp_target(Some(id))?;
    if !root.join(target.rom).exists() {
        return Ok(None);
    }
    CanonicalRom::load_target(root, target).map(Some)
}
fn json(root: &Path, path: &str) -> Result<Value, String> {
    let bytes = std::fs::read(root.join(path)).map_err(|e| format!("{path}: {e}"))?;
    serde_json::from_slice(&bytes).map_err(|e| format!("{path}: {e}"))
}
fn hex(value: &Value) -> Result<usize, String> {
    value
        .as_str()
        .and_then(|text| usize::from_str_radix(text.trim_start_matches("0x"), 16).ok())
        .ok_or_else(|| format!("{value} is not an address"))
}
fn merge(face: &mut Face, glyphs: Face) {
    for (character, glyph) in glyphs {
        face.entry(character).or_insert(glyph);
    }
}
fn menu_widths(root: &Path) -> Result<Vec<i32>, String> {
    json(root, MENU_WIDTHS)?["tables"][MENU_WIDTHS_KEY]["segments"][0]["values"]
        .as_array()
        .ok_or("menu width table lacks values")?
        .iter()
        .map(|value| {
            value
                .as_u64()
                .map(|n| n as i32)
                .ok_or("width is not a number".into())
        })
        .collect()
}
/// A Western code's character; 0x7F–0x9F hold the game's own button and
/// ornament symbols, not Windows-1252 punctuation, so they are left out.
fn western(code: usize) -> Option<char> {
    if (0x7f..0xa0).contains(&code) {
        return None;
    }
    WINDOWS_1252.decode(&[code as u8]).0.chars().next()
}
fn blank(rows: &[u16]) -> bool {
    rows.iter().all(|row| *row == 0)
}
/// A Western dialogue table relocates between editions; it is found where the
/// most four-record runs of the English table recur, and only by that vote.
fn locate(rom: &[u8], english: &[u8]) -> Option<usize> {
    let mut votes = BTreeMap::<usize, usize>::new();
    for code in (0x21..0x7d).step_by(4) {
        let first = (code - 0x20) * DIALOGUE_RECORD;
        let probe = &english[first..first + 4 * DIALOGUE_RECORD];
        for at in (0..rom.len() - probe.len()).step_by(4) {
            if rom[at..at + probe.len()] == *probe && at >= first {
                *votes.entry(at - first).or_default() += 1;
            }
        }
    }
    votes
        .into_iter()
        .max_by_key(|(_, count)| *count)
        .filter(|(_, count)| *count >= 6)
        .map(|(at, _)| at)
}
fn word(bytes: &[u8], at: usize) -> u16 {
    u16::from_le_bytes([bytes[at], bytes[at + 1]])
}
/// The baseline sits under the capital H's last inked row.
fn baseline(rows: &[&[u16]]) -> i32 {
    rows.iter()
        .filter_map(|rows| rows.iter().rposition(|row| *row != 0))
        .next()
        .map_or(0, |row| row as i32 + 1)
}
fn menu(tiles: &[u8], widths: &[i32]) -> Face {
    let rows = |code: usize| -> Vec<u16> {
        tiles[code * TILE_BYTES..(code + 1) * TILE_BYTES]
            .chunks_exact(4)
            .map(|row| {
                (0..8).fold(0u16, |bits, x| {
                    let nibble = row[x / 2] >> (4 * (x % 2)) & 15;
                    bits | (u16::from(nibble == 1) << (15 - x))
                })
            })
            .collect()
    };
    let base = baseline(&[&rows(usize::from(b'H'))]);
    let mut face = Face::new();
    for (offset, advance) in widths.iter().enumerate() {
        let code = 0x20 + offset;
        let Some(character) = western(code).filter(|_| code != 0x5c && code < 0x100) else {
            continue;
        };
        let rows = rows(code);
        if *advance > 1 && (!blank(&rows) || character.is_whitespace() || character == '\u{a0}') {
            face.insert(
                character,
                Glyph {
                    advance: *advance,
                    baseline: base,
                    rows,
                },
            );
        }
    }
    face
}
fn dialogue(table: &[u8]) -> Face {
    let rows = |code: usize| -> Vec<u16> {
        let at = (code - 0x20) * DIALOGUE_RECORD + 2;
        (0..15).map(|row| word(table, at + row * 2)).collect()
    };
    let base = baseline(&[&rows(usize::from(b'H'))]);
    let mut face = Face::new();
    for code in 0x20..0x100 {
        let Some(character) = western(code) else {
            continue;
        };
        let advance = i32::from(word(table, (code - 0x20) * DIALOGUE_RECORD));
        let rows = rows(code);
        if advance > 0 && (!blank(&rows) || character.is_whitespace() || character == '\u{a0}') {
            face.insert(
                character,
                Glyph {
                    advance,
                    baseline: base,
                    rows,
                },
            );
        }
    }
    face
}
/// Japanese glyphs by the registered character map of each Japanese archive.
/// Its single-byte Latin is full width, so it takes the full-width forms.
fn japanese(root: &Path) -> Result<Face, String> {
    let mut face = Face::new();
    for (game, _) in GAMES {
        let Some(rom) = load(root, &format!("{game}-ja"))? else {
            continue;
        };
        let layouts = json(root, &format!("recon/{game}/text.json"))?;
        let layout = layouts
            .as_array()
            .and_then(|all| all.iter().find(|layout| layout["japanese"] == true))
            .ok_or_else(|| format!("{game} has no Japanese text layout"))?;
        let characters = layout["characters"]
            .as_str()
            .ok_or("Japanese layout lacks characters")?
            .chars()
            .collect::<Vec<_>>();
        let evidence = &layout["character_evidence"];
        let single = hex(&evidence["single_byte_font"])? - 0x0800_0000;
        let extended = hex(&evidence["extended_font"])? - 0x0800_0000;
        let bytes = rom.bytes();
        let record = |code: usize| -> (i32, Vec<u16>) {
            if code < 0x100 {
                let at = single + (code - 0x20) * JAPANESE_RECORD;
                (
                    i32::from(word(bytes, at)),
                    (0..12).map(|row| word(bytes, at + 2 + row * 2)).collect(),
                )
            } else {
                let at = extended + (code - 0x100) * EXTENDED_RECORD;
                (
                    EXTENDED_ADVANCE,
                    (0..12).map(|row| word(bytes, at + row * 2)).collect(),
                )
            }
        };
        let base = baseline(&[&record(usize::from(b'H')).1]);
        for (index, character) in characters.iter().enumerate() {
            let code = index + 0x20;
            let character = match *character {
                '\u{fffd}' => continue,
                ' ' => '\u{3000}',
                '¥' => '\u{ffe5}',
                ascii @ '!'..='~' => char::from_u32(ascii as u32 + 0xfee0).unwrap(),
                other => other,
            };
            let (advance, rows) = record(code);
            if advance > 0 && (!blank(&rows) || character == '\u{3000}') {
                face.entry(character).or_insert(Glyph {
                    advance,
                    baseline: base,
                    rows,
                });
            }
        }
    }
    Ok(face)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn western_codes_follow_the_text_catalogs_except_game_symbols() {
        assert_eq!(western(0x41), Some('A'));
        assert_eq!(western(0xe9), Some('é'));
        assert_eq!(western(0x8c), None);
        assert_eq!(western(0x9f), None);
    }
    #[test]
    fn a_relocated_table_is_found_by_its_recurring_records() {
        let english = (0..224 * DIALOGUE_RECORD as u32)
            .map(|i| (i.wrapping_mul(2_654_435_761) >> 13) as u8)
            .collect::<Vec<_>>();
        let mut rom = vec![0u8; 0x400];
        rom.extend(&english);
        rom.extend([0; 0x40]);
        // One redrawn glyph does not move the vote.
        rom[0x400 + 0x30 * DIALOGUE_RECORD + 4] ^= 0xff;
        assert_eq!(locate(&rom, &english), Some(0x400));
        assert_eq!(locate(&vec![0u8; english.len() * 2], &english), None);
    }
    #[test]
    fn menu_tiles_ink_only_colour_one() {
        let mut tiles = vec![0x44u8; 256 * TILE_BYTES];
        for y in 0..7 {
            tiles[0x48 * TILE_BYTES + y * 4] = 0x31;
        }
        let mut widths = vec![1; 0xe0];
        widths[0] = 4;
        widths[0x28] = 6;
        let face = menu(&tiles, &widths);
        assert_eq!(face.keys().copied().collect::<String>(), " H");
        let h = &face[&'H'];
        assert_eq!((h.advance, h.baseline), (6, 7));
        assert!(h.rows[..7].iter().all(|row| *row == 0x8000));
    }
}
