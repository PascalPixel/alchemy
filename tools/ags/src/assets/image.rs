use super::{err, AssetError};
use std::io::Cursor;
pub(crate) type Rgb = [u8; 3];
pub const PNG_SIGNATURE: [u8; 8] = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

fn png_error(error: impl std::fmt::Display) -> AssetError {
    AssetError(error.to_string())
}

fn decode(data: &[u8]) -> Result<(png::OutputInfo, Vec<u8>, png::Info<'static>), AssetError> {
    if !data.starts_with(&PNG_SIGNATURE) {
        return err("invalid file signature");
    }
    let mut decoder = png::Decoder::new(Cursor::new(data));
    decoder.set_transformations(png::Transformations::IDENTITY);
    let mut reader = decoder.read_info().map_err(png_error)?;
    let info = reader.info().clone();
    let mut bytes = vec![0; reader.output_buffer_size()];
    let output = reader.next_frame(&mut bytes).map_err(png_error)?;
    bytes.truncate(output.buffer_size());
    Ok((output, bytes, info))
}

#[derive(Debug)]
pub struct IndexedImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u32>,
    pub palette: Vec<Rgb>,
    pub has_transparency: bool,
}

pub fn indexed_png(data: &[u8]) -> Result<IndexedImage, AssetError> {
    let image = indexed_bitmap_png(data)?;
    if !image.width.is_multiple_of(8) || !image.height.is_multiple_of(8) {
        return err("PNG dimensions must be nonzero multiples of eight");
    }
    Ok(image)
}

pub fn indexed_bitmap_png(data: &[u8]) -> Result<IndexedImage, AssetError> {
    let (output, bytes, info) = decode(data)?;
    if output.color_type != png::ColorType::Indexed
        || !matches!(
            output.bit_depth,
            png::BitDepth::One | png::BitDepth::Two | png::BitDepth::Four | png::BitDepth::Eight
        )
    {
        return err("PNG must use an indexed 1/2/4/8-bit palette");
    }
    let (width, height) = (output.width, output.height);
    if width == 0 || height == 0 {
        return err("PNG lacks a nonempty IHDR");
    }
    let depth = output.bit_depth as u32;
    let raw_palette = info
        .palette
        .as_deref()
        .ok_or_else(|| AssetError("PNG lacks IHDR or PLTE".into()))?;
    if raw_palette.len() < 3 || raw_palette.len() > 768 || !raw_palette.len().is_multiple_of(3) {
        return err("invalid PLTE");
    }
    let palette: Vec<Rgb> = raw_palette
        .as_chunks::<3>().0.iter()
        .map(|rgb| [rgb[0], rgb[1], rgb[2]])
        .collect();
    if palette.len() as u32 > 1 << depth {
        return err("palette exceeds indexed bit depth");
    }
    let has_transparency = info
        .trns
        .as_deref()
        .is_some_and(|alpha| alpha.iter().any(|value| *value != 255));
    if let Some(alpha) = info.trns.as_deref() {
        if alpha.len() > palette.len() || alpha.iter().any(|value| *value != 0 && *value != 255) {
            return err("transparency must contain only binary alpha");
        }
    }
    let mut pixels = Vec::with_capacity(width as usize * height as usize);
    let mask = (1u32 << depth) - 1;
    let row_size = (width as usize * depth as usize).div_ceil(8);
    for row in bytes.chunks_exact(output.line_size).take(height as usize) {
        for x in 0..width as usize {
            let bit = x * depth as usize;
            let index = (u32::from(row[bit / 8]) >> (8 - depth - (bit % 8) as u32)) & mask;
            if index as usize >= palette.len() {
                return err("pixel references a missing palette entry");
            }
            pixels.push(index);
        }
    }
    if bytes.len() != row_size * height as usize {
        return err("unexpected decompressed PNG size");
    }
    Ok(IndexedImage {
        width,
        height,
        pixels,
        palette,
        has_transparency,
    })
}

pub(crate) fn encode_png(
    width: usize,
    height: usize,
    color: png::ColorType,
    depth: png::BitDepth,
    palette: Option<&[u8]>,
    transparency: Option<&[u8]>,
    pixels: &[u8],
) -> Result<Vec<u8>, AssetError> {
    let mut output = Vec::new();
    let mut encoder = png::Encoder::new(&mut output, width as u32, height as u32);
    encoder.set_color(color);
    encoder.set_depth(depth);
    if let Some(palette) = palette {
        encoder.set_palette(palette);
    }
    if let Some(alpha) = transparency {
        encoder.set_trns(alpha);
    }
    let mut writer = encoder.write_header().map_err(png_error)?;
    writer.write_image_data(pixels).map_err(png_error)?;
    writer.finish().map_err(png_error)?;
    Ok(output)
}

/// Palette indices `width` to a row, packed `depth` bits a pixel with the
/// leftmost pixel highest and every row in whole bytes, as a PNG holds them.
pub(crate) fn packed_rows(pixels: &[u8], width: usize, depth: png::BitDepth) -> Vec<u8> {
    let depth = depth as usize;
    let row_bytes = (width * depth).div_ceil(8);
    let mut packed = vec![0u8; row_bytes * (pixels.len() / width)];
    for (y, row) in pixels.chunks(width).enumerate() {
        for (x, &index) in row.iter().enumerate() {
            let bit = x * depth;
            packed[y * row_bytes + bit / 8] |= index << (8 - depth - bit % 8);
        }
    }
    packed
}

/// The PLTE and tRNS entries of an indexed PNG as written: two drawings
/// with the same entries show the same colours and transparency.
pub fn png_palette(data: &[u8]) -> Result<(Vec<u8>, Vec<u8>), AssetError> {
    let (output, _, info) = decode(data)?;
    if output.color_type != png::ColorType::Indexed {
        return err("only an indexed PNG has a palette");
    }
    let palette = info
        .palette
        .as_deref()
        .ok_or_else(|| AssetError("PNG lacks PLTE".into()))?;
    Ok((palette.to_vec(), info.trns.as_deref().unwrap_or_default().to_vec()))
}

/// A PNG of the palette indices `pixels`, `width` to a row, written with the
/// palette, transparency and bit depth of the indexed PNG `like`: a sheet
/// drawn again keeps its colours exactly.
pub fn png_like(like: &[u8], pixels: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    let (output, _, info) = decode(like)?;
    if output.color_type != png::ColorType::Indexed {
        return err("only an indexed PNG lends its palette");
    }
    let palette = info
        .palette
        .as_deref()
        .ok_or_else(|| AssetError("PNG lacks PLTE".into()))?;
    if width == 0 || pixels.is_empty() || !pixels.len().is_multiple_of(width) {
        return err("a drawing needs whole nonempty rows");
    }
    let entries = (palette.len() / 3).min(1 << output.bit_depth as usize);
    if pixels.iter().any(|index| *index as usize >= entries) {
        return err("a pixel references a missing palette entry");
    }
    encode_png(
        width,
        pixels.len() / width,
        png::ColorType::Indexed,
        output.bit_depth,
        Some(palette),
        info.trns.as_deref(),
        &packed_rows(pixels, width, output.bit_depth),
    )
}

/// The palette indices of a tile stream drawn `tiles_wide` tiles to a row,
/// in stream order: the sheet a plain `.4bpp` or `.8bpp` recipe reads.
pub fn tile_sheet_pixels(tiles: &[u8], bpp: GbaBpp, tiles_wide: usize) -> Result<Vec<u8>, AssetError> {
    let (tiles_high, width, height) = image_shape(tiles, bpp, tiles_wide)?;
    let mut pixels = vec![0u8; width * height];
    for tile in 0..tiles_high * tiles_wide {
        let (left, top) = (tile % tiles_wide * 8, tile / tiles_wide * 8);
        let bytes = &tiles[tile * bpp.tile_bytes()..][..bpp.tile_bytes()];
        for i in 0..64 {
            pixels[(top + i / 8) * width + left + i % 8] = match bpp {
                GbaBpp::Bpp4 => bytes[i / 2] >> (i % 2 * 4) & 0x0f,
                GbaBpp::Bpp8 => bytes[i],
            };
        }
    }
    Ok(pixels)
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum GbaBpp {
    Bpp4,
    Bpp8,
}

impl GbaBpp {
    pub fn tile_bytes(self) -> usize {
        match self {
            Self::Bpp4 => 32,
            Self::Bpp8 => 64,
        }
    }

    fn palette_entries(self) -> usize {
        match self {
            Self::Bpp4 => 16,
            Self::Bpp8 => 256,
        }
    }

    fn name(self) -> &'static str {
        match self {
            Self::Bpp4 => "4bpp",
            Self::Bpp8 => "8bpp",
        }
    }
}

fn bgr555_palette(image: &IndexedImage) -> Result<Vec<u8>, AssetError> {
    let mut palette = Vec::with_capacity(image.palette.len() * 2);
    for [red, green, blue] in &image.palette {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("palette channels must be exact five-bit values (multiples of 8)");
        }
        let value =
            u16::from(*red >> 3) | u16::from(*green >> 3) << 5 | u16::from(*blue >> 3) << 10;
        palette.extend_from_slice(&value.to_le_bytes());
    }
    Ok(palette)
}

pub fn bgr555_palette_from_png(data: &[u8]) -> Result<Vec<u8>, AssetError> {
    bgr555_palette_of(&indexed_bitmap_png(data)?)
}

pub fn bgr555_palette_of(image: &IndexedImage) -> Result<Vec<u8>, AssetError> {
    if image.has_transparency {
        return err("GBA BGR555 palettes cannot represent transparent PNG entries");
    }
    bgr555_palette(image)
}

fn palette_rgb(palette: &[u8], bpp: GbaBpp) -> Result<Vec<u8>, AssetError> {
    if palette.is_empty() || !palette.len().is_multiple_of(2) {
        return err("BGR555 palette must contain a nonempty whole number of words");
    }
    if palette.len() / 2 > bpp.palette_entries() {
        return err(format!("palette exceeds {} entry limit", bpp.name()));
    }
    let mut rgb = Vec::with_capacity(palette.len() / 2 * 3);
    for word in palette.as_chunks::<2>().0 {
        let value = u16::from_le_bytes([word[0], word[1]]);
        if value & 0x8000 != 0 {
            return err("BGR555 palette uses unsupported transparent bit 15");
        }
        rgb.extend_from_slice(&[
            ((value & 0x1f) << 3) as u8,
            (((value >> 5) & 0x1f) << 3) as u8,
            (((value >> 10) & 0x1f) << 3) as u8,
        ]);
    }
    Ok(rgb)
}

fn image_shape(
    tiles: &[u8],
    bpp: GbaBpp,
    tiles_wide: usize,
) -> Result<(usize, usize, usize), AssetError> {
    if tiles.is_empty() || !tiles.len().is_multiple_of(bpp.tile_bytes()) {
        return err("tile input must contain a nonempty whole number of GBA tiles");
    }
    if tiles_wide == 0 {
        return err("--tiles-wide must be greater than zero");
    }
    let count = tiles.len() / bpp.tile_bytes();
    if !count.is_multiple_of(tiles_wide) {
        return err("tile count must divide evenly into --tiles-wide rows");
    }
    let tiles_high = count / tiles_wide;
    let width = tiles_wide
        .checked_mul(8)
        .ok_or_else(|| AssetError("image width overflows platform bounds".into()))?;
    let height = tiles_high
        .checked_mul(8)
        .ok_or_else(|| AssetError("image height overflows platform bounds".into()))?;
    if width > u32::MAX as usize || height > u32::MAX as usize {
        return err("image dimensions exceed PNG bounds");
    }
    Ok((tiles_high, width, height))
}

pub fn png_from_bitmap(pixels: &[u8], palette: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    let rgb = palette_rgb(palette, GbaBpp::Bpp8)?;
    if width == 0 || pixels.is_empty() || !pixels.len().is_multiple_of(width) {
        return err("a bitmap needs whole nonempty rows");
    }
    if pixels.iter().any(|index| *index as usize >= rgb.len() / 3) {
        return err("bitmap references an index outside the BGR555 palette");
    }
    encode_png(
        width,
        pixels.len() / width,
        png::ColorType::Indexed,
        png::BitDepth::Eight,
        Some(&rgb),
        None,
        pixels,
    )
}

pub const OBJ_SHAPES: [(usize, usize); 12] = [
    (1, 1),
    (2, 2),
    (4, 4),
    (8, 8),
    (2, 1),
    (4, 1),
    (4, 2),
    (8, 4),
    (1, 2),
    (1, 4),
    (2, 4),
    (4, 8),
];

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct TileLayout {
    pub meta: (usize, usize),
    pub tiles_wide: usize,
}

pub fn tiles_from_metatiles(tiles: &[u8], bpp: GbaBpp, layout: TileLayout) -> Vec<u8> {
    let size = bpp.tile_bytes();
    let (mw, mh) = layout.meta;
    let count = tiles.len() / size;
    let metas_wide = (layout.tiles_wide / mw).max(1);
    (0..count)
        .map(|index| {
            let (x, y) = (index % layout.tiles_wide, index / layout.tiles_wide);
            let meta = y / mh * metas_wide + x / mw;
            meta * mw * mh + y % mh * mw + x % mw
        })
        .flat_map(|source| tiles[source * size..][..size].iter().copied())
        .collect()
}

pub fn tile_sheet_layout(tiles: &[u8], bpp: GbaBpp) -> TileLayout {
    tile_sheet_layout_of(tiles, bpp, &OBJ_SHAPES)
}

pub fn tile_sheet_score(tiles: &[u8], bpp: GbaBpp, layout: TileLayout) -> f64 {
    let (same, total) = tile_sheet_seams(tiles, bpp, layout);
    (same as f64 + 1.0) / (total as f64 + 2.0)
}

pub fn sprite_runs_score(tiles: &[u8], bpp: GbaBpp, runs: &[SpriteRun]) -> f64 {
    let size = bpp.tile_bytes();
    let (same, total) = runs.iter().fold((0, 0), |(same, total), run| {
        let part = &tiles[run.first * size..(run.first + run.count) * size];
        let (s, t) = tile_sheet_seams(part, bpp, run.layout);
        (same + s, total + t)
    });
    (same as f64 + 1.0) / (total as f64 + 2.0)
}

fn tile_sheet_seams(tiles: &[u8], bpp: GbaBpp, layout: TileLayout) -> (usize, usize) {
    let count = tiles.len() / bpp.tile_bytes();
    let pixel = |tiles: &[u8], tile: usize, x: usize, y: usize| match bpp {
        GbaBpp::Bpp4 => (tiles[tile * 32 + y * 4 + x / 2] >> (x % 2 * 4)) & 0x0f,
        GbaBpp::Bpp8 => tiles[tile * 64 + y * 8 + x],
    };
    let sheet = tiles_from_metatiles(tiles, bpp, layout);
    let wide = layout.tiles_wide.max(1);
    let (mut same, mut total) = (0usize, 0usize);
    let mut seam = |a: u8, b: u8| {
        if a != 0 || b != 0 {
            total += 1;
            same += usize::from(a != 0 && b != 0);
        }
    };
    for tile in 0..count {
        for edge in 0..8 {
            if tile % wide + 1 < wide && tile + 1 < count {
                seam(
                    pixel(&sheet, tile, 7, edge),
                    pixel(&sheet, tile + 1, 0, edge),
                );
            }
            if tile + wide < count {
                seam(
                    pixel(&sheet, tile, edge, 7),
                    pixel(&sheet, tile + wide, edge, 0),
                );
            }
        }
    }
    (same, total)
}

fn tile_sheet_layout_of(tiles: &[u8], bpp: GbaBpp, shapes: &[(usize, usize)]) -> TileLayout {
    let count = tiles.len() / bpp.tile_bytes();
    let score = |layout: TileLayout| tile_sheet_score(tiles, bpp, layout);
    let first = shapes.first().copied().unwrap_or((1, 1));
    let plain = TileLayout {
        meta: first,
        tiles_wide: if first == (1, 1) {
            count.clamp(1, 32)
        } else {
            first.0
        },
    };
    let mut best = (plain, -1.0, -1.0);
    for &(mw, mh) in shapes {
        if !count.is_multiple_of(mw * mh) {
            continue;
        }
        let metas = count / (mw * mh);
        for metas_wide in (1..=metas).filter(|wide| metas.is_multiple_of(*wide) && wide * mw <= 64) {
            if metas_wide == 1 && (mw, mh) != (1, 1) && shapes.len() > 1
                || metas_wide * mw < 2 && count > 1
            {
                continue;
            }
            let layout = TileLayout {
                meta: (mw, mh),
                tiles_wide: metas_wide * mw,
            };
            let value = score(layout);
            let margin = if (mw, mh) == (1, 1) || metas_wide == 1 {
                0.0
            } else {
                0.05
            };
            if value - margin > best.1 {
                best = (layout, value - margin, value);
            }
        }
    }
    best.0
}

const PICTURE_PRICE: i64 = 24;

const PICTURE_SPRITES: usize = 8;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SpriteRun {
    pub first: usize,
    pub count: usize,
    pub layout: TileLayout,
}

/// The best split ending here: (score, sprite shape, picture count).
type Step = (i64, (usize, usize), usize);
/// A run of pictures: (first, count, sprite shape, sprites).
type Group = (usize, usize, (usize, usize), Vec<usize>);

pub fn sprite_runs(tiles: &[u8], bpp: GbaBpp) -> Vec<SpriteRun> {
    let count = tiles.len() / bpp.tile_bytes();
    let pixel = |tile: usize, x: usize, y: usize| match bpp {
        GbaBpp::Bpp4 => (tiles[tile * 32 + y * 4 + x / 2] >> (x % 2 * 4)) & 0x0f,
        GbaBpp::Bpp8 => tiles[tile * 64 + y * 8 + x],
    };
    let picture = |first: usize, (mw, mh): (usize, usize), sprites: usize| {
        let wide = mw * sprites;
        let at = |x: usize, y: usize| first + x / mw * mw * mh + y * mw + x % mw;
        let mut value = -PICTURE_PRICE;
        let mut seam = |a: u8, b: u8| {
            if a != 0 || b != 0 {
                value += if a != 0 && b != 0 { 1 } else { -1 };
            }
        };
        for y in 0..mh {
            for x in 0..wide {
                for edge in 0..8 {
                    if x + 1 < wide {
                        seam(pixel(at(x, y), 7, edge), pixel(at(x + 1, y), 0, edge));
                    }
                    if y + 1 < mh {
                        seam(pixel(at(x, y), edge, 7), pixel(at(x, y + 1), edge, 0));
                    }
                }
            }
        }
        value
    };
    let mut best: Vec<Option<Step>> = vec![None; count + 1];
    best[0] = Some((0, (1, 1), 0));
    for end in 1..=count {
        for &(mw, mh) in &OBJ_SHAPES {
            let size = mw * mh;
            for sprites in 1..=PICTURE_SPRITES {
                let Some(start) = end.checked_sub(size * sprites) else {
                    break;
                };
                let Some((before, ..)) = best[start] else {
                    continue;
                };
                let value = before + picture(start, (mw, mh), sprites);
                if best[end].is_none_or(|(known, ..)| value > known) {
                    best[end] = Some((value, (mw, mh), sprites));
                }
            }
        }
    }
    let mut pictures = Vec::new();
    let mut end = count;
    while end > 0 {
        let (_, meta, sprites) = best[end].expect("every tile count is whole 8x8 sprites");
        end -= meta.0 * meta.1 * sprites;
        pictures.push((end, meta, sprites));
    }
    pictures.reverse();
    let mut groups: Vec<Group> = Vec::new();
    for (first, meta, sprites) in pictures {
        let tiles_in = meta.0 * meta.1 * sprites;
        match groups.last_mut() {
            Some(group) if group.2 == meta => {
                group.1 += tiles_in;
                group.3.push(meta.0 * sprites);
            }
            _ => groups.push((first, tiles_in, meta, vec![meta.0 * sprites])),
        }
    }
    let size = bpp.tile_bytes();
    groups
        .into_iter()
        .map(|(first, count, meta, widths)| {
            let row = count / meta.1;
            let tiles_wide = if widths
                .iter()
                .all(|wide| *wide == widths[0] && *wide > meta.0)
            {
                widths[0]
            } else if row <= 64 {
                row
            } else {
                tile_sheet_layout_of(&tiles[first * size..(first + count) * size], bpp, &[meta])
                    .tiles_wide
            };
            let meta = if tiles_wide == meta.0 { (1, 1) } else { meta };
            SpriteRun {
                first,
                count,
                layout: TileLayout { meta, tiles_wide },
            }
        })
        .collect()
}
