//! Pixel layouts over decoded indexed images: frame atlases, GBA tiles in
//! frame order, metatiles and the pictures a tilemap draws.
//! Pixel codecs themselves (4/8bpp packing, zero-skip, MTF4, delta7, tilemap
//! delta) are in `assets`; this module arranges what they are given.
use crate::assets::image::{GbaBpp, IndexedImage};

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

/// Frames of `frame_width` by `frame_height` pixels, `columns` to a row,
/// numbered left to right and top to bottom.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) struct Atlas {
    pub frame_width: usize,
    pub frame_height: usize,
    pub columns: usize,
}

impl Atlas {
    /// How many frame slots an image of `width` by `height` holds.
    pub(crate) fn slots(&self, width: usize, height: usize) -> Result<usize, String> {
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
}

/// Pack pixels `width` by `height` into GBA 8x8 tiles. Without an atlas the
/// tiles run left to right, top to bottom; with one, each of its first
/// `frames` frames (whole tiles each) contributes its own tiles in that
/// order before the next frame's. A four-bit tile keeps each pixel's low
/// four bits, as pret's gbagfx does: an 8-bit PNG may carry each tile's
/// palette row in the high bits, so a bank whose tiles use several rows is
/// drawn in its real colours.
pub(crate) fn tiles(
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
                GbaBpp::Bpp4 => data.extend(
                    values
                        .chunks_exact(2)
                        .map(|pair| (pair[0] & 0xf) | (pair[1] & 0xf) << 4),
                ),
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
    pub(crate) fn frame_rows(&self) -> usize {
        self.high + self.hidden.len().div_ceil(self.wide)
    }

    /// Pack the tiles a drawing `width` by `height` pixels holds: one
    /// picture, or with `frames` as many frames as it stacks. A cell that
    /// shows a tile twice must agree, and a cell showing no run tile, like
    /// a picture's hidden tile, is blank.
    pub(crate) fn tiles(
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
