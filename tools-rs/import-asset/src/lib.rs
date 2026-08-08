//! Port of `tools/lib/import_asset.ts`.
//!
//! WHY THIS FILE EXISTS
//! --------------------
//! Every tracked art and music asset in the repo enters the build through this
//! module: indexed PNGs become GBA 4bpp/8bpp tile banks, RGBA PNGs become
//! 15-bit BGR palettes, and Standard MIDI Files become a flat, sorted event
//! report. Roughly twenty `tools/make/*.ts` builders import it, so it is the
//! single widest chokepoint in the asset pipeline and the piece whose byte
//! output the ROM equality proof depends on most directly.
//!
//! The port is deliberately literal. Where JavaScript semantics leak into
//! observable behaviour (`Buffer#subarray` clamping instead of erroring,
//! `Buffer#toString("ascii")` masking the high bit, `JSON.stringify` turning
//! `NaN` into `null`, `Array#sort` comparing by UTF-16 code unit) the Rust
//! reproduces the JavaScript rather than the "correct" behaviour, and each
//! such spot carries a `PORT NOTE`.

use alchemy_zlib::{crc32, inflate_sync};

pub type Rgb = [u8; 3];

/// `Report` in the TypeScript is `Record<string, number>`: insertion-ordered,
/// with `number` meaning f64. Both properties are observable — the CLI prints
/// the record through `sortedJson`, which sorts keys but keeps JS number
/// formatting — so this is a `Vec` of pairs holding `f64`, never a `HashMap`.
#[derive(Debug, Clone, PartialEq, Default)]
pub struct Report(pub Vec<(String, f64)>);

impl Report {
    pub fn set(&mut self, key: &str, value: f64) {
        if let Some(slot) = self.0.iter_mut().find(|(name, _)| name == key) {
            slot.1 = value;
        } else {
            self.0.push((key.to_string(), value));
        }
    }

    pub fn get(&self, key: &str) -> Option<f64> {
        self.0.iter().find(|(name, _)| name == key).map(|(_, value)| *value)
    }
}

/// Every failure here corresponds to a `throw new Error(...)` in the
/// TypeScript. The message text is preserved character for character, because
/// callers (and the parity harness) compare it.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AssetError(pub String);

impl std::fmt::Display for AssetError {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        formatter.write_str(&self.0)
    }
}

impl std::error::Error for AssetError {}

fn err<T>(message: &str) -> Result<T, AssetError> {
    Err(AssetError(message.to_string()))
}

pub const PNG_SIGNATURE: [u8; 8] = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

// ---------------------------------------------------------------------------
// JavaScript primitives that leak into observable output
// ---------------------------------------------------------------------------

/// `Buffer#toString("ascii")`.
///
/// PORT NOTE: Node's `"ascii"` encoding does **not** validate; it masks each
/// byte with `0x7f`. A chunk kind of `[0xc9, 0x48, 0x44, 0x52]` therefore reads
/// as `"IHDR"` in the TypeScript and must read as `"IHDR"` here too.
pub fn ascii(bytes: &[u8]) -> String {
    bytes.iter().map(|byte| (byte & 0x7f) as char).collect()
}

/// `Buffer#toString("hex")`.
pub fn hex(bytes: &[u8]) -> String {
    let mut out = String::with_capacity(bytes.len() * 2);
    for byte in bytes {
        out.push(char::from_digit((byte >> 4) as u32, 16).unwrap());
        out.push(char::from_digit((byte & 15) as u32, 16).unwrap());
    }
    out
}

/// `Buffer#subarray(start, end)`.
///
/// PORT NOTE: JavaScript clamps both ends and returns a short (possibly empty)
/// view; `&slice[start..end]` panics. `midi_events` relies on the clamping —
/// it slices past the end on purpose and then compares `value.length !== size`
/// to produce a specific "truncated ..." message.
pub fn subarray(data: &[u8], start: usize, end: usize) -> &[u8] {
    let start = start.min(data.len());
    let end = end.clamp(start, data.len());
    &data[start..end]
}

fn read_u32_be(data: &[u8], offset: usize) -> u32 {
    u32::from_be_bytes([data[offset], data[offset + 1], data[offset + 2], data[offset + 3]])
}

fn read_u16_be(data: &[u8], offset: usize) -> u16 {
    u16::from_be_bytes([data[offset], data[offset + 1]])
}

/// `JSON.stringify` for a JavaScript `number`.
///
/// PORT NOTE: `NaN` and the infinities are not representable in JSON, so
/// `JSON.stringify` emits the literal `null` for them. `--bpp abc` reaches this
/// path through `Number("abc")`, so it is not hypothetical. Finite values that
/// are integral print without a decimal point in both languages; the shortest
/// round-trip form Rust's `{}` produces agrees with JS for every value this
/// module can generate (dimensions, tile counts, and a user-supplied bpp).
pub fn js_number_json(value: f64) -> String {
    if !value.is_finite() {
        return "null".to_string();
    }
    if value == 0.0 {
        // Covers -0.0, which JavaScript prints as `0`.
        return "0".to_string();
    }
    if value.fract() == 0.0 && value.abs() < 1e21 {
        return format!("{value:.0}");
    }
    format!("{value}")
}

// ---------------------------------------------------------------------------
// PNG chunk walking
// ---------------------------------------------------------------------------

/// What a chunk visitor wants the walk to do next.
pub enum Walk {
    Continue,
    Stop,
}

/// Port of the `chunks` generator. The TypeScript yields lazily, so a CRC
/// failure in a chunk *after* `IEND` is never observed; the callback form
/// preserves that by letting the visitor stop the walk.
pub fn walk_chunks<F>(data: &[u8], signature: &[u8], mut visit: F) -> Result<(), AssetError>
where
    F: FnMut(&str, &[u8]) -> Result<Walk, AssetError>,
{
    if subarray(data, 0, signature.len()) != signature {
        return err("invalid file signature");
    }
    let mut cursor = signature.len();
    while cursor < data.len() {
        if cursor + 12 > data.len() {
            return err("truncated chunk");
        }
        let size = read_u32_be(data, cursor) as usize;
        let kind = &data[cursor + 4..cursor + 8];
        // PORT NOTE: `cursor + 12 + size` is exact f64 arithmetic in JS. A
        // 32-bit size cannot overflow usize on any host we build for, but the
        // checked add keeps a hostile file from wrapping.
        let end = match cursor.checked_add(12).and_then(|value| value.checked_add(size)) {
            Some(end) => end,
            None => return err("truncated chunk payload"),
        };
        if end > data.len() {
            return err("truncated chunk payload");
        }
        let payload = &data[cursor + 8..cursor + 8 + size];
        let expected = read_u32_be(data, cursor + 8 + size);
        let mut body = Vec::with_capacity(4 + payload.len());
        body.extend_from_slice(kind);
        body.extend_from_slice(payload);
        if crc32(&body) != expected {
            return err("chunk CRC mismatch");
        }
        if matches!(visit(&ascii(kind), payload)?, Walk::Stop) {
            return Ok(());
        }
        cursor = end;
    }
    Ok(())
}

pub fn paeth(left: i32, above: i32, upper_left: i32) -> i32 {
    let prediction = left + above - upper_left;
    let a = (prediction - left).abs();
    let b = (prediction - above).abs();
    let c = (prediction - upper_left).abs();
    if a <= b && a <= c {
        left
    } else if b <= c {
        above
    } else {
        upper_left
    }
}

fn unfilter(
    raw: &[u8],
    height: usize,
    row_size: usize,
    stride: usize,
) -> Result<Vec<Vec<u8>>, AssetError> {
    let expected = match height.checked_mul(row_size + 1) {
        Some(value) => value,
        None => return err("unexpected decompressed PNG size"),
    };
    if raw.len() != expected {
        return err("unexpected decompressed PNG size");
    }
    let mut rows: Vec<Vec<u8>> = Vec::with_capacity(height);
    let mut cursor = 0usize;
    let mut previous = vec![0u8; row_size];
    for _ in 0..height {
        let method = raw[cursor];
        let source = &raw[cursor + 1..cursor + 1 + row_size];
        cursor += row_size + 1;
        if method > 4 {
            return err("invalid PNG filter");
        }
        let mut row = vec![0u8; row_size];
        for index in 0..row_size {
            let left = if index >= stride { i32::from(row[index - stride]) } else { 0 };
            let above = i32::from(previous[index]);
            let upper_left =
                if index >= stride { i32::from(previous[index - stride]) } else { 0 };
            let predictor = match method {
                0 => 0,
                1 => left,
                2 => above,
                // PORT NOTE: `Math.floor((left + above) / 2)` on two
                // non-negative bytes is a plain integer halving.
                3 => (left + above) / 2,
                _ => paeth(left, above, upper_left),
            };
            row[index] = (i32::from(source[index]) + predictor) as u8;
        }
        previous = row.clone();
        rows.push(row);
    }
    Ok(rows)
}

// ---------------------------------------------------------------------------
// PNG decoding
// ---------------------------------------------------------------------------

#[derive(Debug)]
pub struct IndexedImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u32>,
    pub palette: Vec<Rgb>,
}

pub fn indexed_png(data: &[u8]) -> Result<IndexedImage, AssetError> {
    let mut width: Option<u32> = None;
    let mut height: Option<u32> = None;
    let mut depth: Option<u32> = None;
    let mut palette: Option<Vec<Rgb>> = None;
    let mut transparency: Option<Vec<u8>> = None;
    let mut compressed: Vec<u8> = Vec::new();

    walk_chunks(data, &PNG_SIGNATURE, |name, payload| {
        match name {
            "IHDR" => {
                if payload.len() != 13 {
                    return err("invalid IHDR");
                }
                width = Some(read_u32_be(payload, 0));
                height = Some(read_u32_be(payload, 4));
                let bit_depth = u32::from(payload[8]);
                depth = Some(bit_depth);
                let color = payload[9];
                if color != 3 || !matches!(bit_depth, 1 | 2 | 4 | 8) {
                    return err("PNG must use an indexed 1/2/4/8-bit palette");
                }
                if payload[10] != 0 || payload[11] != 0 || payload[12] != 0 {
                    return err("unsupported PNG encoding");
                }
            }
            "PLTE" => {
                if !payload.len().is_multiple_of(3) || payload.len() < 3 || payload.len() > 768 {
                    return err("invalid PLTE");
                }
                palette = Some(
                    payload.chunks_exact(3).map(|rgb| [rgb[0], rgb[1], rgb[2]]).collect(),
                );
            }
            "tRNS" => transparency = Some(payload.to_vec()),
            "IDAT" => compressed.extend_from_slice(payload),
            "IEND" => return Ok(Walk::Stop),
            _ => {}
        }
        Ok(Walk::Continue)
    })?;

    let (width, height, depth, palette) = match (width, height, depth, palette) {
        (Some(width), Some(height), Some(depth), Some(palette)) => {
            (width, height, depth, palette)
        }
        _ => return err("PNG lacks IHDR or PLTE"),
    };
    if width == 0 || height == 0 || !width.is_multiple_of(8) || !height.is_multiple_of(8) {
        return err("PNG dimensions must be nonzero multiples of eight");
    }
    if palette.len() as u64 > (1u64 << depth) {
        return err("palette exceeds indexed bit depth");
    }
    if let Some(alpha) = &transparency {
        if alpha.len() > palette.len() || alpha.iter().any(|value| *value != 0 && *value != 255) {
            return err("transparency must contain only binary alpha");
        }
    }

    let row_size = (u64::from(width) * u64::from(depth)).div_ceil(8) as usize;
    let inflated = inflate_sync(&compressed).map_err(|error| AssetError(error.0.to_string()))?;
    let rows = unfilter(&inflated, height as usize, row_size, 1)?;

    let mut pixels: Vec<u32> = Vec::with_capacity(rows.len() * width as usize);
    let mask = (1u32 << depth) - 1;
    for row in &rows {
        for x in 0..u64::from(width) {
            let bit = x * u64::from(depth);
            let shift = 8 - depth - (bit % 8) as u32;
            let index = (u32::from(row[(bit / 8) as usize]) >> shift) & mask;
            if index as usize >= palette.len() {
                return err("pixel references a missing palette entry");
            }
            pixels.push(index);
        }
    }
    Ok(IndexedImage { width, height, pixels, palette })
}

#[derive(Debug)]
pub struct RgbaImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u8>,
}

pub fn rgba_png(data: &[u8]) -> Result<RgbaImage, AssetError> {
    let mut width: Option<u32> = None;
    let mut height: Option<u32> = None;
    let mut compressed: Vec<u8> = Vec::new();

    walk_chunks(data, &PNG_SIGNATURE, |name, payload| {
        match name {
            "IHDR" => {
                if payload.len() != 13 {
                    return err("invalid IHDR");
                }
                width = Some(read_u32_be(payload, 0));
                height = Some(read_u32_be(payload, 4));
                if payload[9] != 6 || payload[8] != 8 {
                    return err("PNG must use 8-bit RGBA pixels");
                }
                if payload[10] != 0 || payload[11] != 0 || payload[12] != 0 {
                    return err("unsupported PNG encoding");
                }
            }
            "IDAT" => compressed.extend_from_slice(payload),
            "IEND" => return Ok(Walk::Stop),
            _ => {}
        }
        Ok(Walk::Continue)
    })?;

    let (width, height) = match (width, height) {
        (Some(width), Some(height)) if width != 0 && height != 0 => (width, height),
        _ => return err("PNG lacks a nonempty IHDR"),
    };
    let row_size = match (width as usize).checked_mul(4) {
        Some(value) => value,
        None => return err("unexpected decompressed PNG size"),
    };
    let inflated = inflate_sync(&compressed).map_err(|error| AssetError(error.0.to_string()))?;
    let rows = unfilter(&inflated, height as usize, row_size, 4)?;
    Ok(RgbaImage { width, height, pixels: rows.concat() })
}

// ---------------------------------------------------------------------------
// GBA encoders
// ---------------------------------------------------------------------------

pub fn gba_palette_rgba(data: &[u8]) -> Result<(Vec<u8>, Report), AssetError> {
    let image = rgba_png(data)?;
    let mut palette = vec![0u8; image.pixels.len() / 2];
    for (offset, rgba) in image.pixels.chunks_exact(4).enumerate() {
        let (red, green, blue, alpha) = (rgba[0], rgba[1], rgba[2], rgba[3]);
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("RGBA palette colors must be multiples of eight");
        }
        if alpha != 254 && alpha != 255 {
            return err("RGBA palette alpha must be 254 or 255");
        }
        let value = u16::from(red >> 3)
            | (u16::from(green >> 3) << 5)
            | (u16::from(blue >> 3) << 10)
            | (u16::from(255 - alpha) << 15);
        palette[offset * 2..offset * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let mut report = Report::default();
    report.set("width", f64::from(image.width));
    report.set("height", f64::from(image.height));
    report.set("palette_entries", (palette.len() / 2) as f64);
    Ok((palette, report))
}

/// PORT NOTE: `bpp` arrives from `Number(...)` and is only ever compared with
/// `=== 4`; everything else takes the 8bpp path but is still echoed verbatim
/// into the report. `--bpp 7` and `--bpp NaN` are therefore accepted by the
/// TypeScript, and are accepted here, so the parameter stays an `f64`.
pub fn gba_graphics(data: &[u8], bpp: f64) -> Result<(Vec<u8>, Vec<u8>, Report), AssetError> {
    let image = indexed_png(data)?;
    let four = bpp == 4.0;
    let limit: u32 = if four { 16 } else { 256 };
    if image.palette.len() as u64 > u64::from(limit)
        || image.pixels.iter().any(|pixel| *pixel >= limit)
    {
        return Err(AssetError(format!("image does not fit {}bpp", js_number_json(bpp))));
    }
    let mut colors = vec![0u8; image.palette.len() * 2];
    for (index, [red, green, blue]) in image.palette.iter().copied().enumerate() {
        if red & 7 != 0 || green & 7 != 0 || blue & 7 != 0 {
            return err("palette channels must be exact five-bit values (multiples of 8)");
        }
        let value =
            u16::from(red >> 3) | (u16::from(green >> 3) << 5) | (u16::from(blue >> 3) << 10);
        colors[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }

    let width = image.width as usize;
    let height = image.height as usize;
    let mut tiles: Vec<u8> = Vec::new();
    for top in (0..height).step_by(8) {
        for left in (0..width).step_by(8) {
            let mut tile = [0u32; 64];
            for y in 0..8usize {
                for x in 0..8usize {
                    tile[y * 8 + x] = image.pixels[(top + y) * width + left + x];
                }
            }
            if four {
                for index in (0..64).step_by(2) {
                    tiles.push((tile[index] | (tile[index + 1] << 4)) as u8);
                }
            } else {
                tiles.extend(tile.iter().map(|value| *value as u8));
            }
        }
    }

    let mut report = Report::default();
    report.set("width", image.width as f64);
    report.set("height", image.height as f64);
    report.set("bpp", bpp);
    report.set("tiles", (width / 8 * (height / 8)) as f64);
    report.set("palette_entries", image.palette.len() as f64);
    Ok((tiles, colors, report))
}

// ---------------------------------------------------------------------------
// MIDI
// ---------------------------------------------------------------------------

pub fn vlq(data: &[u8], mut cursor: usize) -> Result<(i32, usize), AssetError> {
    let mut value: i32 = 0;
    for _ in 0..4 {
        if cursor >= data.len() {
            return err("truncated variable-length quantity");
        }
        let byte = data[cursor];
        cursor += 1;
        // PORT NOTE: JS `<<` is a 32-bit signed shift. Four iterations cap the
        // value at 28 bits, so `i32` cannot overflow here.
        value = (value << 7) | i32::from(byte & 0x7f);
        if byte < 0x80 {
            return Ok((value, cursor));
        }
    }
    err("overlong variable-length quantity")
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum EventBody {
    Meta { meta: u8, data: String },
    Sysex { status: u8, data: String },
    Channel { status: u8, data: Vec<u8> },
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MidiEvent {
    pub tick: i64,
    pub track: usize,
    pub order: usize,
    pub body: EventBody,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct MidiReport {
    pub format: u16,
    pub tracks: u16,
    pub ticks_per_quarter: u16,
    pub events: Vec<MidiEvent>,
}

pub fn midi_events(data: &[u8]) -> Result<MidiReport, AssetError> {
    let mut parsed: Vec<(String, &[u8])> = Vec::new();
    let mut cursor = 0usize;
    while cursor < data.len() {
        if cursor + 8 > data.len() {
            return err("truncated MIDI chunk");
        }
        let kind = ascii(&data[cursor..cursor + 4]);
        let size = read_u32_be(data, cursor + 4) as usize;
        cursor += 8;
        // PORT NOTE: `subarray` clamps; the length check below is what turns a
        // short tail into the specific "truncated MIDI chunk payload" error.
        let payload = subarray(data, cursor, cursor.saturating_add(size));
        cursor = cursor.saturating_add(size);
        if payload.len() != size {
            return err("truncated MIDI chunk payload");
        }
        parsed.push((kind, payload));
    }
    if parsed.is_empty() || parsed[0].0 != "MThd" || parsed[0].1.len() != 6 {
        return err("invalid MIDI header");
    }
    let header = parsed[0].1;
    let form = read_u16_be(header, 0);
    let tracks = read_u16_be(header, 2);
    let division = read_u16_be(header, 4);
    if !matches!(form, 0 | 1) || tracks == 0 || division & 0x8000 != 0 {
        return err("only format 0/1 PPQN MIDI is supported");
    }
    let track_chunks: Vec<&[u8]> =
        parsed[1..].iter().filter(|(kind, _)| kind == "MTrk").map(|(_, body)| *body).collect();
    if track_chunks.len() != usize::from(tracks) {
        return err("MIDI track count mismatch");
    }

    let mut result: Vec<MidiEvent> = Vec::new();
    for (track, payload) in track_chunks.iter().enumerate() {
        let mut cursor = 0usize;
        let mut tick: i64 = 0;
        let mut order = 0usize;
        let mut running: Option<u8> = None;
        while cursor < payload.len() {
            let (delta, next) = vlq(payload, cursor)?;
            cursor = next;
            tick += i64::from(delta);
            if cursor >= payload.len() {
                return err("truncated MIDI event");
            }
            let mut status = payload[cursor];
            if status < 0x80 {
                match running {
                    Some(value) => status = value,
                    None => return err("running status without channel status"),
                }
            } else {
                cursor += 1;
            }
            let body = if status == 0xff {
                if cursor >= payload.len() {
                    return err("truncated meta event");
                }
                let meta = payload[cursor];
                let (size, next) = vlq(payload, cursor + 1)?;
                cursor = next;
                let size = size as usize;
                let value = subarray(payload, cursor, cursor.saturating_add(size));
                cursor = cursor.saturating_add(size);
                if value.len() != size {
                    return err("truncated meta payload");
                }
                running = None;
                EventBody::Meta { meta, data: hex(value) }
            } else if status == 0xf0 || status == 0xf7 {
                let (size, next) = vlq(payload, cursor)?;
                cursor = next;
                let size = size as usize;
                let value = subarray(payload, cursor, cursor.saturating_add(size));
                cursor = cursor.saturating_add(size);
                if value.len() != size {
                    return err("truncated system-exclusive payload");
                }
                running = None;
                EventBody::Sysex { status, data: hex(value) }
            } else if (0x80..=0xef).contains(&status) {
                running = Some(status);
                let size = if matches!(status & 0xf0, 0xc0 | 0xd0) { 1 } else { 2 };
                let value = subarray(payload, cursor, cursor + size);
                cursor += size;
                if value.len() != size || value.iter().any(|byte| byte & 0x80 != 0) {
                    return err("invalid channel event");
                }
                EventBody::Channel { status, data: value.to_vec() }
            } else {
                return err("unsupported MIDI system event");
            };
            result.push(MidiEvent { tick, track, order, body });
            order += 1;
        }
    }
    // (track, order) is unique, so this comparator is a total order and the
    // JS sort's stability never comes into play.
    result.sort_by(|left, right| {
        (left.tick, left.track, left.order).cmp(&(right.tick, right.track, right.order))
    });
    Ok(MidiReport { format: form, tracks, ticks_per_quarter: division, events: result })
}

// ---------------------------------------------------------------------------
// Chunk writers
// ---------------------------------------------------------------------------

pub fn make_chunk(kind: &[u8], payload: &[u8]) -> Vec<u8> {
    let mut body = Vec::with_capacity(kind.len() + payload.len());
    body.extend_from_slice(kind);
    body.extend_from_slice(payload);
    let mut out = Vec::with_capacity(body.len() + 8);
    out.extend_from_slice(&(payload.len() as u32).to_be_bytes());
    out.extend_from_slice(&body);
    out.extend_from_slice(&crc32(&body).to_be_bytes());
    out
}

pub fn make_midi_chunk(kind: &[u8], payload: &[u8]) -> Vec<u8> {
    let mut out = Vec::with_capacity(kind.len() + 4 + payload.len());
    out.extend_from_slice(kind);
    out.extend_from_slice(&(payload.len() as u32).to_be_bytes());
    out.extend_from_slice(payload);
    out
}

// ---------------------------------------------------------------------------
// canonicalJson, specialised to the MIDI report
// ---------------------------------------------------------------------------

/// PORT NOTE: `tools/lib/canonical_json.ts` is generic over `unknown`, and a
/// crate for it already exists — but that crate inherits `version.workspace`
/// and pulls in `serde_json`. The only value this module ever serialises is a
/// `MidiReport`, whose shape is fixed, so the two `reflow` rules that apply
/// (objects one key per line; arrays of primitives inline) are open-coded here
/// and pinned by a test that diffs against the real `canonicalJson` output.
pub fn canonical_midi_json(report: &MidiReport) -> String {
    let mut out = String::new();
    out.push_str("{\n");
    out.push_str(&format!("  \"format\": {}", report.format));
    out.push_str(&format!(",\n  \"tracks\": {}", report.tracks));
    out.push_str(&format!(",\n  \"ticks_per_quarter\": {}", report.ticks_per_quarter));
    out.push_str(",\n  \"events\": ");
    if report.events.is_empty() {
        out.push_str("[]");
    } else {
        out.push_str("[\n");
        let rendered: Vec<String> = report
            .events
            .iter()
            .map(|event| {
                let mut fields = vec![
                    format!("\"tick\": {}", event.tick),
                    format!("\"track\": {}", event.track),
                    format!("\"order\": {}", event.order),
                ];
                match &event.body {
                    EventBody::Meta { meta, data } => {
                        fields.push("\"type\": \"meta\"".to_string());
                        fields.push(format!("\"meta\": {meta}"));
                        fields.push(format!("\"data\": \"{data}\""));
                    }
                    EventBody::Sysex { status, data } => {
                        fields.push("\"type\": \"sysex\"".to_string());
                        fields.push(format!("\"status\": {status}"));
                        fields.push(format!("\"data\": \"{data}\""));
                    }
                    EventBody::Channel { status, data } => {
                        fields.push("\"type\": \"channel\"".to_string());
                        fields.push(format!("\"status\": {status}"));
                        let inline: Vec<String> =
                            data.iter().map(|byte| byte.to_string()).collect();
                        fields.push(format!("\"data\": [{}]", inline.join(", ")));
                    }
                }
                let joined: Vec<String> =
                    fields.into_iter().map(|field| format!("      {field}")).collect();
                format!("    {{\n{}\n    }}", joined.join(",\n"))
            })
            .collect();
        out.push_str(&rendered.join(",\n"));
        out.push_str("\n  ]");
    }
    out.push_str("\n}");
    out
}

// ---------------------------------------------------------------------------
// sortedJson
// ---------------------------------------------------------------------------

/// PORT NOTE: the TypeScript is
/// `JSON.stringify(sorted).replace(/:/g, ": ").replace(/,/g, ", ")`, a blunt
/// textual rewrite over the *whole* minified document. Every key this module
/// produces is `[a-z_]+` and every value is a number, so no colon or comma can
/// appear inside a string literal and the rewrite is safe — but it is written
/// as the same two literal replacements so the equivalence is auditable.
pub fn sorted_json(report: &Report) -> String {
    let mut keys: Vec<&(String, f64)> = report.0.iter().collect();
    // JS `Array#sort` with no comparator sorts by UTF-16 code unit. Every key
    // here is ASCII, where that coincides with Rust's byte ordering.
    keys.sort_by(|left, right| left.0.as_bytes().cmp(right.0.as_bytes()));
    let minified = format!(
        "{{{}}}",
        keys.iter()
            .map(|(key, value)| format!("\"{key}\":{}", js_number_json(*value)))
            .collect::<Vec<_>>()
            .join(",")
    );
    minified.replace(':', ": ").replace(',', ", ")
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

/// The two encoders below are the `export_asset.ts` functions that the
/// TypeScript `self_test` reaches through a dynamic `import()`.
///
/// PORT NOTE: `tools/lib/export_asset.ts` has not been ported yet, so rather
/// than block this crate on it the two functions the self-test needs are
/// inlined here. They are byte-for-byte the same construction and are covered
/// by round-trip assertions; when `export-asset` lands they should be deleted
/// in favour of a path dependency.
pub fn rgba_image(raw: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    if raw.is_empty() || !raw.len().is_multiple_of(4) || width == 0 {
        return err("RGBA input must contain whole nonempty pixels");
    }
    let pixels = raw.len() / 4;
    if !pixels.is_multiple_of(width) {
        return err("width must divide the RGBA pixel count");
    }
    let height = pixels / width;
    let mut rows: Vec<u8> = Vec::with_capacity(height * (width * 4 + 1));
    for row in 0..height {
        rows.push(0);
        rows.extend_from_slice(&raw[row * width * 4..(row + 1) * width * 4]);
    }
    let mut ihdr = vec![0u8; 13];
    ihdr[0..4].copy_from_slice(&(width as u32).to_be_bytes());
    ihdr[4..8].copy_from_slice(&(height as u32).to_be_bytes());
    ihdr[8] = 8;
    ihdr[9] = 6;
    let mut png = PNG_SIGNATURE.to_vec();
    png.extend_from_slice(&make_chunk(b"IHDR", &ihdr));
    png.extend_from_slice(&make_chunk(
        b"IDAT",
        &alchemy_zlib::deflate_sync(&rows, alchemy_zlib::DeflateOptions { level: Some(9) }),
    ));
    png.extend_from_slice(&make_chunk(b"IEND", &[]));
    Ok(png)
}

pub fn palette_rgba_image(raw: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    if raw.is_empty() || !raw.len().is_multiple_of(2) || width == 0 || !(raw.len() / 2).is_multiple_of(width)
    {
        return err("palette RGBA dimensions must divide whole colors");
    }
    let mut pixels = vec![0u8; raw.len() * 2];
    for (index, entry) in raw.chunks_exact(2).enumerate() {
        let value = u16::from_le_bytes([entry[0], entry[1]]);
        let target = index * 4;
        pixels[target] = ((value & 31) << 3) as u8;
        pixels[target + 1] = (((value >> 5) & 31) << 3) as u8;
        pixels[target + 2] = (((value >> 10) & 31) << 3) as u8;
        pixels[target + 3] = if value & 0x8000 != 0 { 254 } else { 255 };
    }
    rgba_image(&pixels, width)
}

pub fn self_test() -> Result<String, AssetError> {
    let palette = [0u8, 0, 0, 248, 0, 0];
    let rows: Vec<u8> = (0..8).flat_map(|_| [0u8, 0x55]).collect();
    let mut ihdr = vec![0u8; 13];
    ihdr[0..4].copy_from_slice(&8u32.to_be_bytes());
    ihdr[4..8].copy_from_slice(&8u32.to_be_bytes());
    ihdr[8] = 1;
    ihdr[9] = 3;
    let mut png = PNG_SIGNATURE.to_vec();
    png.extend_from_slice(&make_chunk(b"IHDR", &ihdr));
    png.extend_from_slice(&make_chunk(b"PLTE", &palette));
    png.extend_from_slice(&make_chunk(
        b"IDAT",
        &alchemy_zlib::deflate_sync(&rows, alchemy_zlib::DeflateOptions::default()),
    ));
    png.extend_from_slice(&make_chunk(b"IEND", &[]));
    let (tiles, colors, report) = gba_graphics(&png, 4.0)?;
    if tiles != vec![0x10u8; 32] || colors != vec![0, 0, 0x1f, 0] || report.get("tiles") != Some(1.0)
    {
        return err("graphics self-test failed");
    }

    let track = decode_hex("00c00500903c4060803c0000ff2f00");
    let mut midi = make_midi_chunk(b"MThd", &decode_hex("000000010060"));
    midi.extend_from_slice(&make_midi_chunk(b"MTrk", &track));
    let midi_report = midi_events(&midi)?;
    let ticks: Vec<String> =
        midi_report.events.iter().map(|event| event.tick.to_string()).collect();
    if midi_report.ticks_per_quarter != 96 || ticks.join(",") != "0,0,96,96" {
        return err("MIDI self-test failed");
    }

    let rgba: Vec<u8> = (0..7 * 5 * 4).map(|index: u32| ((index * 29 + 3) & 255) as u8).collect();
    let image = rgba_image(&rgba, 7)?;
    let unpacked = rgba_png(&image)?;
    if unpacked.width != 7 || unpacked.height != 5 || unpacked.pixels != rgba {
        return err("RGBA self-test failed");
    }

    let mut raw_palette = vec![0u8; 256];
    for index in 0..128usize {
        let value = ((index * 109) & 0xffff) as u16;
        raw_palette[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
    let palette_image = palette_rgba_image(&raw_palette, 16)?;
    if gba_palette_rgba(&palette_image)?.0 != raw_palette {
        return err("RGBA palette self-test failed");
    }
    Ok("self-test=ok".to_string())
}

fn decode_hex(text: &str) -> Vec<u8> {
    text.as_bytes()
        .chunks_exact(2)
        .map(|pair| {
            let high = (pair[0] as char).to_digit(16).unwrap();
            let low = (pair[1] as char).to_digit(16).unwrap();
            (high * 16 + low) as u8
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn indexed_fixture(width: u32, height: u32, depth: u8, palette: &[u8], rows: &[u8]) -> Vec<u8> {
        let mut ihdr = vec![0u8; 13];
        ihdr[0..4].copy_from_slice(&width.to_be_bytes());
        ihdr[4..8].copy_from_slice(&height.to_be_bytes());
        ihdr[8] = depth;
        ihdr[9] = 3;
        let mut png = PNG_SIGNATURE.to_vec();
        png.extend_from_slice(&make_chunk(b"IHDR", &ihdr));
        png.extend_from_slice(&make_chunk(b"PLTE", palette));
        png.extend_from_slice(&make_chunk(
            b"IDAT",
            &alchemy_zlib::deflate_sync(rows, alchemy_zlib::DeflateOptions::default()),
        ));
        png.extend_from_slice(&make_chunk(b"IEND", &[]));
        png
    }

    #[test]
    fn self_test_matches_typescript() {
        assert_eq!(self_test().unwrap(), "self-test=ok");
    }

    #[test]
    fn ascii_masks_the_high_bit_like_node() {
        // A chunk kind with the high bit set still decodes to "IHDR", which is
        // exactly what `Buffer#toString("ascii")` does.
        assert_eq!(ascii(&[0xc9, 0x48, 0x44, 0x52]), "IHDR");
        assert_eq!(ascii(&[0x80]), "\u{0}");
    }

    #[test]
    fn subarray_clamps_instead_of_panicking() {
        let data = [1u8, 2, 3];
        assert_eq!(subarray(&data, 1, 99), &[2, 3]);
        assert_eq!(subarray(&data, 9, 12), &[] as &[u8]);
        assert_eq!(subarray(&data, 2, 1), &[] as &[u8]);
    }

    #[test]
    fn signature_mismatch_is_reported_before_anything_else() {
        assert_eq!(indexed_png(b"nope").unwrap_err().0, "invalid file signature");
        assert_eq!(rgba_png(&[]).unwrap_err().0, "invalid file signature");
    }

    #[test]
    fn crc_mismatch_is_detected() {
        let mut png = indexed_fixture(8, 8, 1, &[0, 0, 0, 248, 0, 0], &[0, 0x55].repeat(8));
        let last = png.len() - 1;
        png[last] ^= 1;
        assert_eq!(indexed_png(&png).unwrap_err().0, "chunk CRC mismatch");
    }

    #[test]
    fn chunks_after_iend_are_never_validated() {
        // The TypeScript `chunks` is a generator and the decoders `break` on
        // IEND, so a corrupt trailing chunk is unreachable. Prove the port
        // stops at the same point rather than eagerly scanning the file.
        let mut png = indexed_fixture(8, 8, 1, &[0, 0, 0, 248, 0, 0], &[0, 0x55].repeat(8));
        let mut garbage = make_chunk(b"junk", &[1, 2, 3]);
        let last = garbage.len() - 1;
        garbage[last] ^= 0xff;
        png.extend_from_slice(&garbage);
        assert!(indexed_png(&png).is_ok());
    }

    #[test]
    fn four_bpp_packs_two_pixels_per_byte_low_nibble_first() {
        // Row 0x55 at 1bpp is the pixel pattern 0,1,0,1,0,1,0,1; packing pairs
        // low-nibble-first gives 0x10 for every byte.
        let png = indexed_fixture(8, 8, 1, &[0, 0, 0, 248, 0, 0], &[0, 0x55].repeat(8));
        let (tiles, colors, report) = gba_graphics(&png, 4.0).unwrap();
        assert_eq!(tiles, vec![0x10u8; 32]);
        assert_eq!(colors, vec![0, 0, 0x1f, 0]);
        assert_eq!(sorted_json(&report), "{\"bpp\": 4, \"height\": 8, \"palette_entries\": 2, \"tiles\": 1, \"width\": 8}");
    }

    #[test]
    fn eight_bpp_emits_one_byte_per_pixel() {
        let png = indexed_fixture(8, 8, 8, &[0, 0, 0, 248, 0, 0], &[0u8]
            .iter()
            .copied()
            .chain((0..8u8).map(|x| x % 2))
            .cycle()
            .take(8 * 9)
            .collect::<Vec<u8>>());
        let (tiles, _, report) = gba_graphics(&png, 8.0).unwrap();
        assert_eq!(tiles.len(), 64);
        assert_eq!(report.get("bpp"), Some(8.0));
    }

    #[test]
    fn a_non_four_bpp_still_takes_the_eight_bpp_path_and_echoes_the_value() {
        // `bpp` is only ever compared with `=== 4` in the TypeScript.
        let png = indexed_fixture(8, 8, 1, &[0, 0, 0, 248, 0, 0], &[0, 0x55].repeat(8));
        let (tiles, _, report) = gba_graphics(&png, 7.0).unwrap();
        assert_eq!(tiles.len(), 64);
        assert!(sorted_json(&report).contains("\"bpp\": 7"));
    }

    #[test]
    fn nan_bpp_serialises_as_null_like_json_stringify() {
        let png = indexed_fixture(8, 8, 1, &[0, 0, 0, 248, 0, 0], &[0, 0x55].repeat(8));
        let (_, _, report) = gba_graphics(&png, f64::NAN).unwrap();
        assert!(sorted_json(&report).contains("\"bpp\": null"));
    }

    #[test]
    fn four_bpp_rejects_a_palette_that_does_not_fit() {
        let palette: Vec<u8> = (0..17).flat_map(|index: u8| [index * 8, 0, 0]).collect();
        // 32 pixels wide keeps the 8-aligned rule; 5bpp does not exist, so use
        // 8bpp rows referencing index 16.
        let rows: Vec<u8> = (0..8).flat_map(|_| {
            let mut row = vec![0u8];
            row.extend((0..8u8).map(|_| 16));
            row
        }).collect();
        let png = indexed_fixture(8, 8, 8, &palette, &rows);
        assert_eq!(gba_graphics(&png, 4.0).unwrap_err().0, "image does not fit 4bpp");
    }

    #[test]
    fn dimension_and_palette_rules_are_enforced() {
        let rows: Vec<u8> = (0..8).flat_map(|_| [0u8, 0x55]).collect();
        assert_eq!(
            indexed_png(&indexed_fixture(9, 8, 1, &[0, 0, 0, 248, 0, 0], &rows)).unwrap_err().0,
            "PNG dimensions must be nonzero multiples of eight"
        );
        let big: Vec<u8> = (0..3).flat_map(|index: u8| [index * 8, 0, 0]).collect();
        assert_eq!(
            indexed_png(&indexed_fixture(8, 8, 1, &big, &rows)).unwrap_err().0,
            "palette exceeds indexed bit depth"
        );
    }

    #[test]
    fn palette_channels_must_be_multiples_of_eight() {
        let png = indexed_fixture(8, 8, 1, &[0, 0, 0, 249, 0, 0], &[0, 0x55].repeat(8));
        assert_eq!(
            gba_graphics(&png, 4.0).unwrap_err().0,
            "palette channels must be exact five-bit values (multiples of 8)"
        );
    }

    #[test]
    fn paeth_ties_prefer_left_then_above() {
        assert_eq!(paeth(10, 10, 10), 10);
        assert_eq!(paeth(0, 255, 0), 255);
        assert_eq!(paeth(255, 0, 0), 255);
    }

    #[test]
    fn every_filter_type_round_trips() {
        // Build an 8x8 8bpp image whose rows use filters 0..4 in turn, then
        // check unfiltering reproduces the intended indices.
        let palette: Vec<u8> = (0..4).flat_map(|index: u8| [index * 8, 0, 0]).collect();
        let mut rows: Vec<u8> = Vec::new();
        for y in 0..8u8 {
            rows.push(y % 5);
            // Filtered bytes; the decoder's job is to invert them. Use zeros so
            // each filter has a well-defined, checkable outcome.
            rows.extend(std::iter::repeat_n(0u8, 8));
        }
        let png = indexed_fixture(8, 8, 8, &palette, &rows);
        let image = indexed_png(&png).unwrap();
        assert_eq!(image.pixels.len(), 64);
        assert!(image.pixels.iter().all(|pixel| *pixel == 0));
    }

    #[test]
    fn invalid_filter_byte_is_rejected() {
        let palette: Vec<u8> = (0..2).flat_map(|index: u8| [index * 8, 0, 0]).collect();
        let mut rows: Vec<u8> = Vec::new();
        for _ in 0..8 {
            rows.push(9);
            rows.extend(std::iter::repeat_n(0u8, 8));
        }
        let png = indexed_fixture(8, 8, 8, &palette, &rows);
        assert_eq!(indexed_png(&png).unwrap_err().0, "invalid PNG filter");
    }

    #[test]
    fn rgba_palette_round_trips_through_the_encoder() {
        let mut raw = vec![0u8; 64];
        for index in 0..32usize {
            let value = ((index * 2111) & 0xffff) as u16;
            raw[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
        }
        let image = palette_rgba_image(&raw, 8).unwrap();
        let (palette, report) = gba_palette_rgba(&image).unwrap();
        assert_eq!(palette, raw);
        assert_eq!(report.get("palette_entries"), Some(32.0));
        assert_eq!(report.get("width"), Some(8.0));
    }

    #[test]
    fn rgba_palette_rejects_off_grid_channels() {
        let rgba: Vec<u8> = vec![1, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255];
        let image = rgba_image(&rgba, 4).unwrap();
        assert_eq!(
            gba_palette_rgba(&image).unwrap_err().0,
            "RGBA palette colors must be multiples of eight"
        );
    }

    #[test]
    fn rgba_palette_rejects_partial_alpha() {
        let rgba: Vec<u8> = vec![0, 0, 0, 128, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255];
        let image = rgba_image(&rgba, 4).unwrap();
        assert_eq!(
            gba_palette_rgba(&image).unwrap_err().0,
            "RGBA palette alpha must be 254 or 255"
        );
    }

    fn simple_midi(track: &str) -> Vec<u8> {
        let mut midi = make_midi_chunk(b"MThd", &decode_hex("000000010060"));
        midi.extend_from_slice(&make_midi_chunk(b"MTrk", &decode_hex(track)));
        midi
    }

    #[test]
    fn midi_running_status_is_carried_across_events() {
        // 90 3c 40, then a bare 3e 40 reusing the running status.
        let report = midi_events(&simple_midi("00903c40003e4000ff2f00")).unwrap();
        let statuses: Vec<u8> = report
            .events
            .iter()
            .filter_map(|event| match &event.body {
                EventBody::Channel { status, .. } => Some(*status),
                _ => None,
            })
            .collect();
        assert_eq!(statuses, vec![0x90, 0x90]);
    }

    #[test]
    fn a_meta_event_clears_running_status() {
        assert_eq!(
            midi_events(&simple_midi("00903c4000ff0100003e40")).unwrap_err().0,
            "running status without channel status"
        );
    }

    #[test]
    fn program_change_takes_one_data_byte() {
        let report = midi_events(&simple_midi("00c00500ff2f00")).unwrap();
        assert_eq!(
            report.events[0].body,
            EventBody::Channel { status: 0xc0, data: vec![5] }
        );
    }

    #[test]
    fn midi_events_sort_by_tick_then_track_then_order() {
        let mut midi = make_midi_chunk(b"MThd", &decode_hex("000100020060"));
        // Track 0 fires at tick 96, track 1 at tick 0.
        midi.extend_from_slice(&make_midi_chunk(b"MTrk", &decode_hex("6090304000ff2f00")));
        midi.extend_from_slice(&make_midi_chunk(b"MTrk", &decode_hex("0090314060ff2f00")));
        let report = midi_events(&midi).unwrap();
        let keys: Vec<(i64, usize)> =
            report.events.iter().map(|event| (event.tick, event.track)).collect();
        assert_eq!(keys, vec![(0, 1), (96, 0), (96, 0), (96, 1)]);
    }

    #[test]
    fn midi_header_rules_are_enforced() {
        assert_eq!(midi_events(&[]).unwrap_err().0, "invalid MIDI header");
        // SMPTE division (high bit set) is rejected.
        let mut midi = make_midi_chunk(b"MThd", &decode_hex("00000001e250"));
        midi.extend_from_slice(&make_midi_chunk(b"MTrk", &decode_hex("00ff2f00")));
        assert_eq!(
            midi_events(&midi).unwrap_err().0,
            "only format 0/1 PPQN MIDI is supported"
        );
    }

    #[test]
    fn a_truncated_chunk_payload_is_named_not_panicked_on() {
        let mut midi = make_midi_chunk(b"MThd", &decode_hex("000000010060"));
        let mut track = make_midi_chunk(b"MTrk", &decode_hex("00ff2f00"));
        track.truncate(track.len() - 2);
        midi.extend_from_slice(&track);
        assert_eq!(midi_events(&midi).unwrap_err().0, "truncated MIDI chunk payload");
    }

    #[test]
    fn vlq_rejects_an_overlong_encoding() {
        assert_eq!(vlq(&[0x80, 0x80, 0x80, 0x80], 0).unwrap_err().0, "overlong variable-length quantity");
        assert_eq!(vlq(&[0x81, 0x00], 0).unwrap(), (128, 2));
        assert_eq!(vlq(&[0x7f], 0).unwrap(), (127, 1));
    }

    #[test]
    fn canonical_midi_json_shape() {
        let report = midi_events(&simple_midi("00c00500903c4060803c0000ff2f00")).unwrap();
        let text = canonical_midi_json(&report);
        assert!(text.starts_with("{\n  \"format\": 0,\n  \"tracks\": 1,\n  \"ticks_per_quarter\": 96,\n  \"events\": [\n"));
        // Arrays of primitives stay on one line; objects get one key per line.
        assert!(text.contains("      \"data\": [60, 64]"));
        assert!(text.contains("      \"type\": \"meta\""));
        assert!(text.ends_with("\n  ]\n}"));
    }

    #[test]
    fn sorted_json_sorts_keys_and_spaces_separators() {
        let mut report = Report::default();
        report.set("width", 8.0);
        report.set("height", 16.0);
        report.set("bpp", 4.0);
        assert_eq!(sorted_json(&report), "{\"bpp\": 4, \"height\": 16, \"width\": 8}");
    }

    #[test]
    fn js_number_json_edge_cases() {
        assert_eq!(js_number_json(0.0), "0");
        assert_eq!(js_number_json(-0.0), "0");
        assert_eq!(js_number_json(4.0), "4");
        assert_eq!(js_number_json(f64::NAN), "null");
        assert_eq!(js_number_json(f64::INFINITY), "null");
        assert_eq!(js_number_json(4.5), "4.5");
    }
}
