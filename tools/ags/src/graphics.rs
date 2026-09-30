//! Pixel layouts over decoded indexed images: sections, frame atlases, GBA
//! tiles in frame order, object banks placed by their tilemaps, 1bpp glyph
//! rows, and banks of zero-skip frames whose directory the linker fills.
//! Pixel codecs themselves (4/8bpp packing, zero-skip, MTF4, delta7, tilemap
//! delta) are Psynergy's; this module arranges what they are given.
use super::asm::Data;
use psynergy::assets::compression::encode_zero_skip;
use psynergy::assets::image::{GbaBpp, IndexedImage};
use std::collections::BTreeMap;

fn tile_bytes(bpp: GbaBpp) -> usize {
    match bpp {
        GbaBpp::Bpp4 => 32,
        GbaBpp::Bpp8 => 64,
    }
}

/// An indexed image's palette indices, row by row, one byte each.
pub fn indices(image: &IndexedImage) -> Vec<u8> {
    image.pixels.iter().map(|pixel| *pixel as u8).collect()
}

/// A rectangle of pixels.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Rect {
    pub x: usize,
    pub y: usize,
    pub width: usize,
    pub height: usize,
}

/// The pixels of `rect`, row by row, from pixels `width` wide and `height` high.
pub fn section(pixels: &[u8], width: usize, height: usize, rect: Rect) -> Result<Vec<u8>, String> {
    if rect.width == 0
        || rect.height == 0
        || rect.x.checked_add(rect.width).is_none_or(|end| end > width)
        || rect
            .y
            .checked_add(rect.height)
            .is_none_or(|end| end > height)
        || pixels.len() != width * height
    {
        return Err("section lies outside its sheet".into());
    }
    let mut output = Vec::with_capacity(rect.width * rect.height);
    for row in rect.y..rect.y + rect.height {
        let start = row * width + rect.x;
        output.extend_from_slice(&pixels[start..start + rect.width]);
    }
    Ok(output)
}

/// Frames of `frame_width` by `frame_height` pixels, `columns` to a row,
/// numbered left to right and top to bottom.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Atlas {
    pub frame_width: usize,
    pub frame_height: usize,
    pub columns: usize,
}

impl Atlas {
    /// How many frame slots an image of `width` by `height` holds.
    pub fn slots(&self, width: usize, height: usize) -> Result<usize, String> {
        if self.frame_width == 0
            || self.frame_height == 0
            || self.columns == 0
            || self.columns.checked_mul(self.frame_width) != Some(width)
            || height % self.frame_height != 0
        {
            return Err("atlas dimensions differ".into());
        }
        Ok(self.columns * (height / self.frame_height))
    }
    /// The pixels of each frame in `selected`, row by row, from pixels of
    /// `depth` bytes each.
    pub fn frames(
        &self,
        pixels: &[u8],
        width: usize,
        height: usize,
        depth: usize,
        selected: &[usize],
    ) -> Result<Vec<Vec<u8>>, String> {
        let slots = self.slots(width, height)?;
        if pixels.len() != width * height * depth {
            return Err("atlas pixels differ from its dimensions".into());
        }
        selected
            .iter()
            .map(|&frame| {
                if frame >= slots {
                    return Err(format!("frame {frame} lies outside its atlas"));
                }
                let left = frame % self.columns * self.frame_width;
                let top = frame / self.columns * self.frame_height;
                let mut buffer = Vec::with_capacity(self.frame_width * self.frame_height * depth);
                for y in 0..self.frame_height {
                    let start = ((top + y) * width + left) * depth;
                    buffer.extend_from_slice(&pixels[start..start + self.frame_width * depth]);
                }
                Ok(buffer)
            })
            .collect()
    }
}

/// Pack pixels `width` by `height` into GBA 8x8 tiles. Without an atlas the
/// tiles run left to right, top to bottom; with one, each of its first
/// `frames` frames (whole tiles each) contributes its own tiles in that
/// order before the next frame's.
pub fn tiles(
    pixels: &[u8],
    width: usize,
    height: usize,
    bpp: GbaBpp,
    atlas: Option<(Atlas, usize)>,
) -> Result<Vec<u8>, String> {
    if width % 8 != 0 || height % 8 != 0 || pixels.len() != width * height {
        return Err("tiles need whole 8x8 cells".into());
    }
    let mut corners = Vec::new();
    match atlas {
        Some((atlas, frames)) => {
            let (w, h) = (atlas.frame_width, atlas.frame_height);
            if w % 8 != 0
                || h % 8 != 0
                || frames == 0
                || frames.div_ceil(atlas.columns.max(1)).checked_mul(h) != Some(height)
                || frames > atlas.slots(width, height)?
            {
                return Err("tile frame atlas dimensions differ".into());
            }
            for frame in 0..frames {
                for y in (0..h).step_by(8) {
                    for x in (0..w).step_by(8) {
                        corners
                            .push((frame % atlas.columns * w + x, frame / atlas.columns * h + y));
                    }
                }
            }
        }
        None => {
            for y in (0..height).step_by(8) {
                for x in (0..width).step_by(8) {
                    corners.push((x, y));
                }
            }
        }
    }
    let mut data = Vec::with_capacity(corners.len() * tile_bytes(bpp));
    for (x, y) in corners {
        for row in 0..8 {
            let values = &pixels[(y + row) * width + x..][..8];
            match bpp {
                GbaBpp::Bpp4 => {
                    if values.iter().any(|pixel| *pixel > 15) {
                        return Err("four-bit tile pixel exceeds its palette".into());
                    }
                    data.extend(values.chunks_exact(2).map(|pair| pair[0] | pair[1] << 4));
                }
                GbaBpp::Bpp8 => data.extend_from_slice(values),
            }
        }
    }
    Ok(data)
}

/// Pack pixels into tiles metatile by metatile, as pret's gbagfx does with
/// -mwidth and -mheight: the sheet is cut row by row into metatiles of
/// `meta_width` by `meta_height` tiles, and each metatile's tiles run left
/// to right, top to bottom, before the next metatile's. A one-by-one
/// metatile is the plain row-major order.
pub fn metatiles(
    pixels: &[u8],
    width: usize,
    height: usize,
    bpp: GbaBpp,
    meta_width: usize,
    meta_height: usize,
) -> Result<Vec<u8>, String> {
    if meta_width == 0 || meta_height == 0 {
        return Err("metatile dimensions must be positive".into());
    }
    let (cell_width, cell_height) = (meta_width * 8, meta_height * 8);
    if width % cell_width != 0 || height % cell_height != 0 || pixels.len() != width * height {
        return Err("the image is not whole metatiles".into());
    }
    let mut data = Vec::with_capacity(pixels.len());
    for my in (0..height).step_by(cell_height) {
        for mx in (0..width).step_by(cell_width) {
            let mut cell = Vec::with_capacity(cell_width * cell_height);
            for row in 0..cell_height {
                cell.extend_from_slice(&pixels[(my + row) * width + mx..][..cell_width]);
            }
            data.extend(tiles(&cell, cell_width, cell_height, bpp, None)?);
        }
    }
    Ok(data)
}

/// `count` tiles from tile `first` of packed tiles.
pub fn tile_range(
    tiles: &[u8],
    bpp: GbaBpp,
    first: usize,
    count: usize,
) -> Result<Vec<u8>, String> {
    let size = tile_bytes(bpp);
    let start = first.checked_mul(size).ok_or("tile offset overflows")?;
    let end = count
        .checked_mul(size)
        .and_then(|length| length.checked_add(start))
        .ok_or("tile count overflows")?;
    if count == 0 {
        return Err("tile range must be nonempty".into());
    }
    Ok(tiles
        .get(start..end)
        .ok_or("tile range lies outside its sheet")?
        .to_vec())
}

/// `data` up to its stored `size`: a canvas drawn larger than the stored
/// data must be blank past it.
pub fn stored_extent(mut data: Vec<u8>, size: usize) -> Result<Vec<u8>, String> {
    if size > data.len() || data[size..].iter().any(|byte| *byte != 0) {
        return Err("canvas carries data beyond its stored extent".into());
    }
    data.truncate(size);
    Ok(data)
}

/// One sheet of a 4bpp object bank: an indexed image whose 8x8 cells its
/// tilemap places. Each tilemap entry gives the cell's tile slot, its
/// palette bank and its flips.
pub struct ObjectSheet<'a> {
    pub pixels: &'a [u8],
    pub width: usize,
    pub height: usize,
    /// Little-endian 16-bit tilemap entries, one per cell.
    pub tilemap: &'a [u8],
    /// The slot the bank's first tile has in the tilemap's numbering.
    pub base_tile: usize,
    /// Cells whose tiles lie outside this bank belong to another window and
    /// are skipped instead of refused.
    pub mixed_windows: bool,
}

fn flip_tile(pixels: &[u8], hflip: bool, vflip: bool) -> Vec<u8> {
    let mut output = Vec::with_capacity(64);
    for y in 0..8 {
        for x in 0..8 {
            let yy = if vflip { 7 - y } else { y };
            let xx = if hflip { 7 - x } else { x };
            output.push(pixels[yy * 8 + xx]);
        }
    }
    output
}

/// A 4bpp tile bank whose slots the sheets' cells fill, unflipped, over the
/// packed `fallback` tiles. Every cell's pixels must lie in its palette bank,
/// and a slot placed twice must hold the same tile. With
/// `require_blank_fallback` a placed slot must be blank in the fallback.
pub fn object_bank(
    fallback: Vec<u8>,
    sheets: &[ObjectSheet],
    require_blank_fallback: bool,
) -> Result<Vec<u8>, String> {
    if fallback.len() % 32 != 0 {
        return Err("object bank fallback holds partial tiles".into());
    }
    let tile_count = fallback.len() / 32;
    let mut output = fallback;
    let mut claimed: BTreeMap<usize, Vec<u8>> = BTreeMap::new();
    for sheet in sheets {
        let (width, height) = (sheet.width, sheet.height);
        if width % 8 != 0
            || height % 8 != 0
            || sheet.pixels.len() != width * height
            || sheet.tilemap.len() != width / 8 * (height / 8) * 2
        {
            return Err("object sheet dimensions differ from its tilemap".into());
        }
        for (cell, entry) in sheet.tilemap.chunks_exact(2).enumerate() {
            let entry = u16::from_le_bytes([entry[0], entry[1]]);
            let tile = usize::from(entry & 0x03ff);
            let palette = usize::from(entry >> 12);
            let (hflip, vflip) = (entry & 0x0400 != 0, entry & 0x0800 != 0);
            let Some(slot) = tile
                .checked_sub(sheet.base_tile)
                .filter(|slot| *slot < tile_count)
            else {
                if sheet.mixed_windows {
                    continue;
                }
                return Err(format!("cell {cell} places a tile outside this bank"));
            };
            let left = cell % (width / 8) * 8;
            let top = cell / (width / 8) * 8;
            let mut displayed = Vec::with_capacity(64);
            for y in 0..8 {
                displayed.extend_from_slice(&sheet.pixels[(top + y) * width + left..][..8]);
            }
            if displayed
                .iter()
                .any(|pixel| usize::from(*pixel) / 16 != palette)
            {
                return Err(format!("cell {cell} differs from its palette bank"));
            }
            let canonical: Vec<u8> = flip_tile(&displayed, hflip, vflip)
                .iter()
                .map(|pixel| pixel & 15)
                .collect();
            let packed: Vec<u8> = canonical
                .chunks_exact(2)
                .map(|pair| pair[0] | pair[1] << 4)
                .collect();
            if claimed
                .get(&slot)
                .is_some_and(|previous| *previous != packed)
            {
                return Err(format!("tile slot {slot} is placed inconsistently"));
            }
            claimed.insert(slot, packed);
        }
    }
    for (slot, packed) in claimed {
        let tile = &mut output[slot * 32..slot * 32 + 32];
        if require_blank_fallback && tile.iter().any(|byte| *byte != 0) {
            return Err(format!("fallback still holds tile {slot}"));
        }
        tile.copy_from_slice(&packed);
    }
    Ok(output)
}

/// The rows of a 1bpp glyph frame `width` pixels wide as integers, pixel x
/// at bit `width - 1 - x`. Rows from `rows` down must be blank.
pub fn glyph_rows(frame: &[u8], width: usize, rows: usize) -> Result<Vec<u32>, String> {
    if !(1..=32).contains(&width) || frame.len() % width != 0 || rows > frame.len() / width {
        return Err("glyph rows exceed their frame".into());
    }
    if frame.iter().any(|pixel| *pixel > 1) {
        return Err("glyph frame is not 1bpp".into());
    }
    if frame[rows * width..].iter().any(|pixel| *pixel != 0) {
        return Err("glyph rows below the stored ones are not blank".into());
    }
    Ok((0..rows)
        .map(|y| {
            (0..width).fold(0u32, |row, x| {
                row | u32::from(frame[y * width + x]) << (width - 1 - x)
            })
        })
        .collect())
}

/// Colours as little-endian BGR555 words; every channel must be a multiple
/// of eight.
pub fn bgr555(colors: &[[u8; 3]]) -> Result<Vec<u8>, String> {
    let mut output = Vec::with_capacity(colors.len() * 2);
    for [red, green, blue] in colors {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return Err("palette channels must be exact five-bit values".into());
        }
        let word = u16::from(red >> 3) | u16::from(green >> 3) << 5 | u16::from(blue >> 3) << 10;
        output.extend(word.to_le_bytes());
    }
    Ok(output)
}

/// A sprite bank: `frames` zero-skip coded one after another, then a
/// word-aligned, null-terminated directory of their addresses. `label`
/// names the bank's start; the directory's words are that label plus each
/// frame's offset, which the linker resolves.
pub fn zero_skip_bank(label: &str, frames: &[Vec<u8>]) -> Result<Data, String> {
    if frames.is_empty() {
        return Err("sprite bank needs frames".into());
    }
    let mut data = Data::default();
    data.align_to(4, 0)?;
    data.label(label, true);
    let mut offsets = Vec::with_capacity(frames.len());
    for frame in frames {
        offsets.push(data.bytes.len());
        data.bytes
            .extend(encode_zero_skip(frame).map_err(|error| error.to_string())?);
    }
    data.align_to(4, 0)?;
    for offset in offsets {
        data.pointer(label, offset as i64);
    }
    data.bytes.extend([0; 4]);
    Ok(data)
}

/// A run of BG tiles drawn as the tilemap that shows them: tiles `first`
/// to `first + count`, in the box of map cells that show any of them.
/// A picture is that box once; its tiles the map never shows are blank.
/// Animation frames are that box stacked, each frame the run again, the
/// tiles its map never shows laid row by row in the rows beneath the box.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct MapDrawing {
    /// Cell column and row of the box's top left, and its size in cells.
    pub left: usize,
    pub top: usize,
    pub wide: usize,
    pub high: usize,
    /// Each box cell, row by row: the run tile it shows and its flips.
    pub cells: Vec<Option<(usize, bool, bool)>>,
    /// The run tiles no cell shows, in order.
    pub hidden: Vec<usize>,
    /// The run's length in tiles.
    pub count: usize,
}

impl MapDrawing {
    /// Lay out tiles `first..first + count` as `entries`, 16-bit tilemap
    /// entries `map_wide` cells a row, show them.
    pub fn new(
        entries: &[u16],
        map_wide: usize,
        first: usize,
        count: usize,
    ) -> Result<Self, String> {
        if map_wide == 0 || count == 0 || entries.len() % map_wide != 0 {
            return Err("a tilemap drawing needs whole map rows and tiles".into());
        }
        let shows = |entry: u16| {
            usize::from(entry & 0x3ff)
                .checked_sub(first)
                .filter(|tile| *tile < count)
        };
        let showing: Vec<usize> = (0..entries.len())
            .filter(|cell| shows(entries[*cell]).is_some())
            .collect();
        let Some(&last) = showing.last() else {
            return Err(format!(
                "the tilemap shows none of tiles {first}..{}",
                first + count
            ));
        };
        let top = showing[0] / map_wide;
        let left = showing
            .iter()
            .map(|cell| cell % map_wide)
            .min()
            .unwrap_or(0);
        let right = showing
            .iter()
            .map(|cell| cell % map_wide)
            .max()
            .unwrap_or(0);
        let (wide, high) = (right + 1 - left, last / map_wide + 1 - top);
        let mut cells = Vec::with_capacity(wide * high);
        let mut shown = vec![false; count];
        for row in top..top + high {
            for column in left..left + wide {
                let entry = entries[row * map_wide + column];
                cells.push(shows(entry).map(|tile| {
                    shown[tile] = true;
                    (tile, entry & 0x400 != 0, entry & 0x800 != 0)
                }));
            }
        }
        let hidden = (0..count).filter(|tile| !shown[*tile]).collect();
        Ok(Self {
            left,
            top,
            wide,
            high,
            cells,
            hidden,
            count,
        })
    }

    /// Cell rows a frame takes: the box, then its hidden tiles' rows.
    pub fn frame_rows(&self) -> usize {
        self.high + self.hidden.len().div_ceil(self.wide)
    }

    /// Pack the tiles a drawing `width` by `height` pixels holds: one
    /// picture, or with `frames` as many frames as it stacks. A cell that
    /// shows a tile twice must agree, and a cell showing no run tile, like
    /// a picture's hidden tile, is blank.
    pub fn tiles(
        &self,
        pixels: &[u8],
        width: usize,
        height: usize,
        bpp: GbaBpp,
        frames: bool,
    ) -> Result<Vec<u8>, String> {
        let rows = if frames { self.frame_rows() } else { self.high };
        if width != self.wide * 8
            || pixels.len() != width * height
            || height == 0
            || height % (rows * 8) != 0
            || (!frames && height != rows * 8)
        {
            return Err(format!(
                "a tilemap {} is {}x{} pixels{}",
                if frames { "frame" } else { "picture" },
                self.wide * 8,
                rows * 8,
                if frames { ", stacked" } else { "" }
            ));
        }
        let count = self.count;
        let cell = |frame: usize, index: usize| -> Vec<u8> {
            let (x, y) = (
                index % self.wide * 8,
                (frame * rows + index / self.wide) * 8,
            );
            (0..64)
                .map(|i| pixels[(y + i / 8) * width + x + i % 8])
                .collect()
        };
        let mut output = Vec::new();
        for frame in 0..height / (rows * 8) {
            let mut run: Vec<Option<Vec<u8>>> = vec![None; count];
            for (index, shown) in self.cells.iter().enumerate() {
                let drawn = cell(frame, index);
                let Some((tile, hflip, vflip)) = *shown else {
                    if drawn.iter().any(|pixel| *pixel != 0) {
                        return Err(format!(
                            "cell {index} of the box shows no tile of the run but is inked"
                        ));
                    }
                    continue;
                };
                let canonical = flip_tile(&drawn, hflip, vflip);
                if run[tile]
                    .as_ref()
                    .is_some_and(|previous| *previous != canonical)
                {
                    return Err(format!("tile {tile} is drawn two ways"));
                }
                run[tile] = Some(canonical);
            }
            for (slot, tile) in self.hidden.iter().enumerate() {
                if frames {
                    run[*tile] = Some(cell(frame, self.wide * self.high + slot));
                }
            }
            if frames {
                for slot in self.hidden.len()..(rows - self.high) * self.wide {
                    if cell(frame, self.wide * self.high + slot)
                        .iter()
                        .any(|pixel| *pixel != 0)
                    {
                        return Err("a frame is inked past its hidden tiles".into());
                    }
                }
            }
            for tile in run {
                let tile = tile.unwrap_or_else(|| vec![0; 64]);
                output.extend(tiles(&tile, 8, 8, bpp, None)?);
            }
        }
        Ok(output)
    }

    /// Draw packed tiles, whole runs of this drawing, as its picture
    /// (one run) or its stacked frames: pixels and their height.
    pub fn draw(
        &self,
        packed: &[u8],
        bpp: GbaBpp,
        frames: bool,
    ) -> Result<(Vec<u8>, usize), String> {
        let size = tile_bytes(bpp);
        let count = self.count;
        if packed.is_empty()
            || packed.len() % (size * count) != 0
            || (!frames && packed.len() != size * count)
        {
            return Err(format!(
                "a tilemap drawing takes whole runs of {count} tiles"
            ));
        }
        let rows = if frames { self.frame_rows() } else { self.high };
        let runs = packed.len() / (size * count);
        let width = self.wide * 8;
        let height = runs * rows * 8;
        let mut pixels = vec![0u8; width * height];
        let unpack = |tile: &[u8]| -> Vec<u8> {
            match bpp {
                GbaBpp::Bpp4 => tile
                    .iter()
                    .flat_map(|byte| [byte & 15, byte >> 4])
                    .collect(),
                GbaBpp::Bpp8 => tile.to_vec(),
            }
        };
        for frame in 0..runs {
            let run = &packed[frame * size * count..][..size * count];
            let mut put = |index: usize, tile: Vec<u8>| {
                let (x, y) = (
                    index % self.wide * 8,
                    (frame * rows + index / self.wide) * 8,
                );
                for i in 0..64 {
                    pixels[(y + i / 8) * width + x + i % 8] = tile[i];
                }
            };
            for (index, shown) in self.cells.iter().enumerate() {
                if let Some((tile, hflip, vflip)) = *shown {
                    put(
                        index,
                        flip_tile(&unpack(&run[tile * size..][..size]), hflip, vflip),
                    );
                }
            }
            for (slot, tile) in self.hidden.iter().enumerate() {
                let pixels = unpack(&run[tile * size..][..size]);
                if !frames && pixels.iter().any(|pixel| *pixel != 0) {
                    return Err(format!(
                        "tile {tile} is inked but the tilemap never shows it"
                    ));
                }
                if frames {
                    put(self.wide * self.high + slot, pixels);
                }
            }
        }
        Ok((pixels, height))
    }
}

#[cfg(test)]
mod map_drawing_tests {
    use super::*;

    /// A 3x2 map: row 0 shows tile 0, tile 1 and tile 2 flipped
    /// horizontally; row 1 shows tile 1 again and two cells of tile 9.
    const MAP: [u16; 6] = [0, 1, 2 | 0x400, 1, 9, 9];

    fn tile(value: u8) -> Vec<u8> {
        // A tile with one inked pixel at its left edge, so flips show.
        (0..64).map(|i| if i == 8 { value } else { 0 }).collect()
    }

    #[test]
    fn a_picture_holds_the_tiles_its_map_shows_and_blanks_the_rest() {
        let drawing = MapDrawing::new(&MAP, 3, 0, 4).unwrap();
        assert_eq!(
            (drawing.left, drawing.top, drawing.wide, drawing.high),
            (0, 0, 3, 2)
        );
        assert_eq!(drawing.hidden, [3]);
        let packed: Vec<u8> = [tile(5), tile(6), tile(7), vec![0; 64]].concat();
        let (pixels, height) = drawing.draw(&packed, GbaBpp::Bpp8, false).unwrap();
        assert_eq!(height, 16);
        // Tile 2 is drawn flipped: its ink at the right edge.
        assert_eq!(pixels[8 * 3 + 16 + 7], 7);
        assert_eq!(
            drawing.tiles(&pixels, 24, 16, GbaBpp::Bpp8, false).unwrap(),
            packed
        );
        // A hidden tile that is inked has no place in a picture.
        let inked: Vec<u8> = [tile(5), tile(6), tile(7), tile(8)].concat();
        assert!(drawing.draw(&inked, GbaBpp::Bpp8, false).is_err());
        // A tile drawn two ways is refused.
        let mut wrong = pixels.clone();
        wrong[24 * 8 + 8 + 8] = 3;
        assert!(drawing.tiles(&wrong, 24, 16, GbaBpp::Bpp8, false).is_err());
    }

    #[test]
    fn frames_stack_the_run_with_its_hidden_tiles_beneath() {
        let drawing = MapDrawing::new(&MAP, 3, 1, 3).unwrap();
        assert_eq!(
            (drawing.left, drawing.top, drawing.wide, drawing.high),
            (0, 0, 3, 2)
        );
        assert_eq!(drawing.hidden, [2]);
        assert_eq!(drawing.frame_rows(), 3);
        let packed: Vec<u8> = [tile(1), tile(2), tile(3), tile(4), tile(5), tile(6)]
            .concat()
            .iter()
            .map(|pixel| pixel & 15)
            .collect::<Vec<u8>>()
            .chunks(2)
            .map(|pair| pair[0] | pair[1] << 4)
            .collect();
        let (pixels, height) = drawing.draw(&packed, GbaBpp::Bpp4, true).unwrap();
        assert_eq!(height, 48);
        assert_eq!(
            drawing.tiles(&pixels, 24, 48, GbaBpp::Bpp4, true).unwrap(),
            packed
        );
        // Cells that show a tile outside the run must stay blank.
        let mut stray = pixels.clone();
        stray[24 * 8 + 8] = 1;
        assert!(drawing.tiles(&stray, 24, 48, GbaBpp::Bpp4, true).is_err());
        assert!(MapDrawing::new(&MAP, 3, 20, 4).is_err());
    }
}

#[cfg(test)]
mod metatile_tests {
    use super::*;

    #[test]
    fn metatiles_group_each_frame_tiles_before_the_next() {
        // A 32x8 sheet, four tiles numbered by their pixel value, cut into
        // 2x1 metatiles: tiles 0,1 then 2,3, the same as row-major here; a
        // 16x16 sheet cut into 1x2 metatiles takes tile 0, tile 2, tile 1, tile 3.
        let mut pixels = vec![0u8; 16 * 16];
        for y in 0..16 {
            for x in 0..16 {
                pixels[y * 16 + x] = (y / 8 * 2 + x / 8) as u8;
            }
        }
        let packed = metatiles(&pixels, 16, 16, GbaBpp::Bpp8, 1, 2).unwrap();
        let order: Vec<u8> = packed.chunks(64).map(|tile| tile[0]).collect();
        assert_eq!(order, [0, 2, 1, 3]);
        let plain = metatiles(&pixels, 16, 16, GbaBpp::Bpp8, 1, 1).unwrap();
        assert_eq!(plain, tiles(&pixels, 16, 16, GbaBpp::Bpp8, None).unwrap());
        assert!(metatiles(&pixels, 16, 16, GbaBpp::Bpp8, 3, 1).is_err());
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn sections_keep_rows_and_refuse_to_leave_the_sheet() {
        let pixels: Vec<u8> = (0..12).collect();
        let rect = |x, y, width, height| Rect {
            x,
            y,
            width,
            height,
        };
        assert_eq!(
            section(&pixels, 4, 3, rect(1, 1, 2, 2)).unwrap(),
            [5, 6, 9, 10]
        );
        for bad in [rect(3, 0, 2, 1), rect(0, 2, 1, 2), rect(0, 0, 0, 1)] {
            assert!(section(&pixels, 4, 3, bad).is_err());
        }
    }

    #[test]
    fn atlases_select_frames_in_any_order() {
        let mut pixels = vec![0u8; 16 * 8];
        for y in 0..8 {
            pixels[y * 16] = 1;
            pixels[y * 16 + 8] = 2;
        }
        let atlas = Atlas {
            frame_width: 8,
            frame_height: 8,
            columns: 2,
        };
        let frames = atlas.frames(&pixels, 16, 8, 1, &[1, 0]).unwrap();
        assert_eq!(frames[0][0], 2);
        assert_eq!(frames[1][0], 1);
        assert!(atlas.frames(&pixels, 16, 8, 1, &[2]).is_err());
        let wrong = Atlas {
            columns: 3,
            ..atlas
        };
        assert!(wrong.frames(&pixels, 16, 8, 1, &[0]).is_err());
    }

    #[test]
    fn tile_frames_keep_each_frames_tile_order() {
        // Two 16x16 frames side by side: each frame's four tiles in turn.
        let mut pixels = vec![0u8; 32 * 16];
        for (index, (x, y)) in [
            (0, 0),
            (8, 0),
            (0, 8),
            (8, 8),
            (16, 0),
            (24, 0),
            (16, 8),
            (24, 8),
        ]
        .into_iter()
        .enumerate()
        {
            pixels[y * 32 + x] = index as u8;
        }
        let atlas = Atlas {
            frame_width: 16,
            frame_height: 16,
            columns: 2,
        };
        let built = tiles(&pixels, 32, 16, GbaBpp::Bpp8, Some((atlas, 2))).unwrap();
        let firsts: Vec<u8> = built.chunks(64).map(|tile| tile[0]).collect();
        assert_eq!(firsts, [0, 1, 2, 3, 4, 5, 6, 7]);
        let plain = tiles(&pixels, 32, 16, GbaBpp::Bpp8, None).unwrap();
        let firsts: Vec<u8> = plain.chunks(64).map(|tile| tile[0]).collect();
        assert_eq!(firsts, [0, 1, 4, 5, 2, 3, 6, 7]);
        assert!(tiles(&pixels, 32, 16, GbaBpp::Bpp8, Some((atlas, 3))).is_err());
        assert!(tiles(&pixels, 32, 16, GbaBpp::Bpp4, None).is_ok());
        pixels[0] = 16;
        assert!(tiles(&pixels, 32, 16, GbaBpp::Bpp4, None).is_err());
    }

    #[test]
    fn tile_ranges_and_canvases_keep_only_whole_stored_data() {
        let sheet = [vec![0x13; 32], vec![0x57; 32]].concat();
        assert_eq!(
            tile_range(&sheet, GbaBpp::Bpp4, 1, 1).unwrap(),
            vec![0x57; 32]
        );
        for (first, count) in [(2, 1), (1, 0), (usize::MAX, 1), (1, usize::MAX)] {
            assert!(tile_range(&sheet, GbaBpp::Bpp4, first, count).is_err());
        }
        assert_eq!(stored_extent(vec![1, 2, 0, 0], 2).unwrap(), [1, 2]);
        assert!(stored_extent(vec![1, 2, 0, 3], 2).is_err());
        assert!(stored_extent(vec![1], 2).is_err());
    }

    #[test]
    fn object_banks_unflip_cells_into_their_slots() {
        // One 16x8 sheet: cell 0 fills slot 1 flipped horizontally, cell 1
        // slot 0 as drawn, both in palette bank 1 (indices 16..32).
        let mut pixels = vec![16u8; 16 * 8];
        pixels[0] = 17;
        pixels[8] = 18;
        let tilemap = [0x01, 0x14, 0x00, 0x10];
        let sheet = ObjectSheet {
            pixels: &pixels,
            width: 16,
            height: 8,
            tilemap: &tilemap,
            base_tile: 0,
            mixed_windows: false,
        };
        let bank = object_bank(vec![0; 64], &[sheet], true).unwrap();
        // Slot 1 is cell 0 mirrored: its index 1 moves to the row's end.
        assert_eq!(bank[32 + 3], 0x10);
        assert_eq!(bank[0], 0x02);
        let sheet = ObjectSheet {
            pixels: &pixels,
            width: 16,
            height: 8,
            tilemap: &tilemap,
            base_tile: 0,
            mixed_windows: false,
        };
        assert!(object_bank(vec![1; 64], &[sheet], true).is_err());
        let outside = [0x05, 0x10, 0x00, 0x10];
        let sheet = ObjectSheet {
            pixels: &pixels,
            width: 16,
            height: 8,
            tilemap: &outside,
            base_tile: 0,
            mixed_windows: false,
        };
        assert!(object_bank(vec![0; 64], &[sheet], true).is_err());
    }

    #[test]
    fn glyph_rows_pack_left_pixels_high_and_keep_the_tail_blank() {
        let mut frame = vec![0u8; 16 * 16];
        frame[0] = 1;
        frame[16 + 15] = 1;
        assert_eq!(glyph_rows(&frame, 16, 2).unwrap(), [0x8000, 1]);
        assert!(glyph_rows(&frame, 16, 1).is_err());
        frame[3] = 2;
        assert!(glyph_rows(&frame, 16, 2).is_err());
    }

    #[test]
    fn sprite_banks_link_their_directory_to_their_own_label() {
        let bank = zero_skip_bank("Bank", &[vec![1, 2], vec![0, 3]]).unwrap();
        assert_eq!(
            bank.bytes_at(0x1000).unwrap(),
            [1, 2, 0, 0xe0, 3, 0, 0, 0, 0, 0x10, 0, 0, 3, 0x10, 0, 0, 0, 0, 0, 0]
        );
        assert!(bank.source().unwrap().contains("\t.4byte Bank + 3\n"));
        assert!(zero_skip_bank("Bank", &[]).is_err());
        assert!(zero_skip_bank("Bank", &[vec![0xe0]]).is_err());
        assert_eq!(bgr555(&[[8, 16, 248]]).unwrap(), [0x41, 0x7c]);
        assert!(bgr555(&[[1, 0, 0]]).is_err());
    }
}
