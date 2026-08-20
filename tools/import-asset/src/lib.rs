use std::io::Cursor;
pub type Rgb = [u8; 3];
#[derive(Debug, Clone, PartialEq, Default)]
pub struct Report(pub Vec<(String, f64)>);
impl Report {
    pub fn set(&mut self, key: &str, value: f64) {
        match self.0.iter_mut().find(|(name, _)| name == key) {
            Some(slot) => slot.1 = value,
            None => self.0.push((key.to_string(), value)),
        }
    }
    pub fn get(&self, key: &str) -> Option<f64> {
        self.0
            .iter()
            .find(|(name, _)| name == key)
            .map(|(_, value)| *value)
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AssetError(pub String);

impl std::fmt::Display for AssetError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for AssetError {}

fn err<T>(message: impl Into<String>) -> Result<T, AssetError> {
    Err(AssetError(message.into()))
}

pub const PNG_SIGNATURE: [u8; 8] = [0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a];

pub fn ascii(bytes: &[u8]) -> String {
    bytes.iter().map(|byte| (byte & 0x7f) as char).collect()
}

pub fn hex(bytes: &[u8]) -> String {
    const DIGITS: &[u8; 16] = b"0123456789abcdef";
    bytes
        .iter()
        .flat_map(|byte| [DIGITS[(byte >> 4) as usize], DIGITS[(byte & 15) as usize]])
        .map(char::from)
        .collect()
}

pub fn subarray(data: &[u8], start: usize, end: usize) -> &[u8] {
    let start = start.min(data.len());
    &data[start..end.clamp(start, data.len())]
}

pub fn js_number_json(value: f64) -> String {
    if !value.is_finite() {
        return "null".into();
    }
    if value == 0.0 {
        return "0".into();
    }
    if value.fract() == 0.0 && value.abs() < 1e21 {
        return format!("{value:.0}");
    }
    value.to_string()
}

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
}

pub fn indexed_png(data: &[u8]) -> Result<IndexedImage, AssetError> {
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
    if width == 0 || height == 0 || !width.is_multiple_of(8) || !height.is_multiple_of(8) {
        return err("PNG dimensions must be nonzero multiples of eight");
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
        .chunks_exact(3)
        .map(|rgb| [rgb[0], rgb[1], rgb[2]])
        .collect();
    if palette.len() as u32 > 1 << depth {
        return err("palette exceeds indexed bit depth");
    }
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
    })
}

#[derive(Debug)]
pub struct RgbaImage {
    pub width: u32,
    pub height: u32,
    pub pixels: Vec<u8>,
}

pub fn rgba_png(data: &[u8]) -> Result<RgbaImage, AssetError> {
    let (output, bytes, _) = decode(data)?;
    if output.color_type != png::ColorType::Rgba || output.bit_depth != png::BitDepth::Eight {
        return err("PNG must use 8-bit RGBA pixels");
    }
    if output.width == 0 || output.height == 0 {
        return err("PNG lacks a nonempty IHDR");
    }
    if output.line_size != output.width as usize * 4 || bytes.len() != output.buffer_size() {
        return err("unexpected decompressed PNG size");
    }
    Ok(RgbaImage {
        width: output.width,
        height: output.height,
        pixels: bytes,
    })
}

pub fn gba_palette_rgba(data: &[u8]) -> Result<(Vec<u8>, Report), AssetError> {
    let image = rgba_png(data)?;
    let mut palette = Vec::with_capacity(image.pixels.len() / 2);
    for rgba in image.pixels.chunks_exact(4) {
        let (r, g, b, a) = (rgba[0], rgba[1], rgba[2], rgba[3]);
        if r & 7 != 0 || g & 7 != 0 || b & 7 != 0 {
            return err("RGBA palette colors must be multiples of eight");
        }
        if a != 254 && a != 255 {
            return err("RGBA palette alpha must be 254 or 255");
        }
        palette.extend_from_slice(
            &(u16::from(r >> 3)
                | u16::from(g >> 3) << 5
                | u16::from(b >> 3) << 10
                | u16::from(255 - a) << 15)
                .to_le_bytes(),
        );
    }
    let mut report = Report::default();
    report.set("width", image.width.into());
    report.set("height", image.height.into());
    report.set("palette_entries", (palette.len() / 2) as f64);
    Ok((palette, report))
}

pub fn gba_graphics(data: &[u8], bpp: f64) -> Result<(Vec<u8>, Vec<u8>, Report), AssetError> {
    let image = indexed_png(data)?;
    let four = bpp == 4.0;
    let limit = if four { 16u32 } else { 256 };
    if image.palette.len() > limit as usize || image.pixels.iter().any(|pixel| *pixel >= limit) {
        return Err(AssetError(format!(
            "image does not fit {}bpp",
            js_number_json(bpp)
        )));
    }
    let mut palette = Vec::with_capacity(image.palette.len() * 2);
    for [r, g, b] in image.palette {
        if r & 7 != 0 || g & 7 != 0 || b & 7 != 0 {
            return err("palette channels must be exact five-bit values (multiples of 8)");
        }
        palette.extend_from_slice(
            &(u16::from(r >> 3) | u16::from(g >> 3) << 5 | u16::from(b >> 3) << 10).to_le_bytes(),
        );
    }
    let width = image.width as usize;
    let mut tiles = Vec::with_capacity(width * image.height as usize / if four { 2 } else { 1 });
    for top in (0..image.height as usize).step_by(8) {
        for left in (0..width).step_by(8) {
            for y in 0..8 {
                for x in (0..8).step_by(if four { 2 } else { 1 }) {
                    let at = (top + y) * width + left + x;
                    if four {
                        tiles.push((image.pixels[at] | image.pixels[at + 1] << 4) as u8);
                    } else {
                        tiles.push(image.pixels[at] as u8);
                    }
                }
            }
        }
    }
    let mut report = Report::default();
    report.set("width", image.width.into());
    report.set("height", image.height.into());
    report.set("bpp", bpp);
    report.set("tiles", (width / 8 * image.height as usize / 8) as f64);
    report.set("palette_entries", palette.len() as f64 / 2.0);
    Ok((tiles, palette, report))
}

fn be_u16(data: &[u8], at: usize) -> Option<u16> {
    data.get(at..at + 2)
        .map(|x| u16::from_be_bytes([x[0], x[1]]))
}
fn be_u32(data: &[u8], at: usize) -> Option<u32> {
    data.get(at..at + 4)
        .map(|x| u32::from_be_bytes([x[0], x[1], x[2], x[3]]))
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

fn vlq(data: &[u8], mut at: usize) -> Result<(i32, usize), AssetError> {
    let mut value = 0;
    for _ in 0..4 {
        let byte = *data
            .get(at)
            .ok_or_else(|| AssetError("truncated variable-length quantity".into()))?;
        at += 1;
        value = value << 7 | i32::from(byte & 0x7f);
        if byte < 0x80 {
            return Ok((value, at));
        }
    }
    err("overlong variable-length quantity")
}

pub fn midi_events(data: &[u8]) -> Result<MidiReport, AssetError> {
    let mut chunks = Vec::new();
    let mut at = 0;
    while at < data.len() {
        if at + 8 > data.len() {
            return err("truncated MIDI chunk");
        }
        let kind = ascii(&data[at..at + 4]);
        let size = be_u32(data, at + 4).unwrap() as usize;
        at += 8;
        let body = subarray(data, at, at.saturating_add(size));
        at = at.saturating_add(size);
        if body.len() != size {
            return err("truncated MIDI chunk payload");
        }
        chunks.push((kind, body));
    }
    if chunks
        .first()
        .map(|(kind, body)| (kind.as_str(), body.len()))
        != Some(("MThd", 6))
    {
        return err("invalid MIDI header");
    }
    let header = chunks[0].1;
    let format = be_u16(header, 0).unwrap();
    let tracks = be_u16(header, 2).unwrap();
    let division = be_u16(header, 4).unwrap();
    if !matches!(format, 0 | 1) || tracks == 0 || division & 0x8000 != 0 {
        return err("only format 0/1 PPQN MIDI is supported");
    }
    let tracks_data: Vec<&[u8]> = chunks
        .iter()
        .skip(1)
        .filter(|(kind, _)| kind == "MTrk")
        .map(|(_, body)| *body)
        .collect();
    if tracks_data.len() != tracks as usize {
        return err("MIDI track count mismatch");
    }
    let mut events = Vec::new();
    for (track, bytes) in tracks_data.into_iter().enumerate() {
        let (mut at, mut tick, mut order, mut running) = (0, 0i64, 0, None);
        while at < bytes.len() {
            let (delta, next) = vlq(bytes, at)?;
            at = next;
            tick += i64::from(delta);
            let mut status = *bytes
                .get(at)
                .ok_or_else(|| AssetError("truncated MIDI event".into()))?;
            if status < 0x80 {
                status = running
                    .ok_or_else(|| AssetError("running status without channel status".into()))?;
            } else {
                at += 1;
            }
            let body = match status {
                0xff => {
                    let meta = *bytes
                        .get(at)
                        .ok_or_else(|| AssetError("truncated meta event".into()))?;
                    let (n, next) = vlq(bytes, at + 1)?;
                    at = next;
                    let value = subarray(bytes, at, at.saturating_add(n as usize));
                    at += n as usize;
                    if value.len() != n as usize {
                        return err("truncated meta payload");
                    }
                    running = None;
                    EventBody::Meta {
                        meta,
                        data: hex(value),
                    }
                }
                0xf0 | 0xf7 => {
                    let (n, next) = vlq(bytes, at)?;
                    at = next;
                    let value = subarray(bytes, at, at.saturating_add(n as usize));
                    at += n as usize;
                    if value.len() != n as usize {
                        return err("truncated system-exclusive payload");
                    }
                    running = None;
                    EventBody::Sysex {
                        status,
                        data: hex(value),
                    }
                }
                0x80..=0xef => {
                    running = Some(status);
                    let n = if matches!(status & 0xf0, 0xc0 | 0xd0) {
                        1
                    } else {
                        2
                    };
                    let value = subarray(bytes, at, at + n);
                    at += n;
                    if value.len() != n || value.iter().any(|byte| byte & 0x80 != 0) {
                        return err("invalid channel event");
                    }
                    EventBody::Channel {
                        status,
                        data: value.to_vec(),
                    }
                }
                _ => return err("unsupported MIDI system event"),
            };
            events.push(MidiEvent {
                tick,
                track,
                order,
                body,
            });
            order += 1;
        }
    }
    events.sort_by_key(|event| (event.tick, event.track, event.order));
    Ok(MidiReport {
        format,
        tracks,
        ticks_per_quarter: division,
        events,
    })
}

pub fn canonical_midi_json(report: &MidiReport) -> String {
    let mut out = format!(
        "{{\n  \"format\": {},\n  \"tracks\": {},\n  \"ticks_per_quarter\": {},\n  \"events\": ",
        report.format, report.tracks, report.ticks_per_quarter
    );
    if report.events.is_empty() {
        out.push_str("[]");
    } else {
        let events: Vec<String> = report
            .events
            .iter()
            .map(|event| {
                let mut fields = vec![
                    format!("\"tick\": {}", event.tick),
                    format!("\"track\": {}", event.track),
                    format!("\"order\": {}", event.order),
                ];
                match &event.body {
                    EventBody::Meta { meta, data } => fields.extend([
                        "\"type\": \"meta\"".into(),
                        format!("\"meta\": {meta}"),
                        format!("\"data\": \"{data}\""),
                    ]),
                    EventBody::Sysex { status, data } => fields.extend([
                        "\"type\": \"sysex\"".into(),
                        format!("\"status\": {status}"),
                        format!("\"data\": \"{data}\""),
                    ]),
                    EventBody::Channel { status, data } => fields.extend([
                        "\"type\": \"channel\"".into(),
                        format!("\"status\": {status}"),
                        format!(
                            "\"data\": [{}]",
                            data.iter()
                                .map(u8::to_string)
                                .collect::<Vec<_>>()
                                .join(", ")
                        ),
                    ]),
                }
                format!(
                    "    {{\n{}\n    }}",
                    fields
                        .into_iter()
                        .map(|field| format!("      {field}"))
                        .collect::<Vec<_>>()
                        .join(",\n")
                )
            })
            .collect();
        out.push_str("[\n");
        out.push_str(&events.join(",\n"));
        out.push_str("\n  ]");
    }
    out.push_str("\n}");
    out
}

pub fn sorted_json(report: &Report) -> String {
    let mut values: Vec<_> = report.0.iter().collect();
    values.sort_by(|a, b| a.0.cmp(&b.0));
    format!(
        "{{{}}}",
        values
            .iter()
            .map(|(key, value)| format!("\"{key}\": {}", js_number_json(*value)))
            .collect::<Vec<_>>()
            .join(", ")
    )
}

fn encode_png(
    width: usize,
    height: usize,
    color: png::ColorType,
    depth: png::BitDepth,
    palette: Option<&[u8]>,
    pixels: &[u8],
) -> Result<Vec<u8>, AssetError> {
    let mut output = Vec::new();
    let mut encoder = png::Encoder::new(&mut output, width as u32, height as u32);
    encoder.set_color(color);
    encoder.set_depth(depth);
    if let Some(palette) = palette {
        encoder.set_palette(palette);
    }
    let mut writer = encoder.write_header().map_err(png_error)?;
    writer.write_image_data(pixels).map_err(png_error)?;
    writer.finish().map_err(png_error)?;
    Ok(output)
}

fn rgba_image(raw: &[u8], width: usize) -> Result<Vec<u8>, AssetError> {
    if raw.is_empty()
        || !raw.len().is_multiple_of(4)
        || width == 0
        || !(raw.len() / 4).is_multiple_of(width)
    {
        return err("RGBA input must contain whole nonempty pixels");
    }
    encode_png(
        width,
        raw.len() / width / 4,
        png::ColorType::Rgba,
        png::BitDepth::Eight,
        None,
        raw,
    )
}

pub fn self_test() -> Result<String, AssetError> {
    let mut indexed = Vec::new();
    let mut encoder = png::Encoder::new(&mut indexed, 8, 8);
    encoder.set_color(png::ColorType::Indexed);
    encoder.set_depth(png::BitDepth::One);
    encoder.set_palette(vec![0, 0, 0, 248, 0, 0]);
    encoder
        .write_header()
        .map_err(png_error)?
        .write_image_data(&[0x55; 8])
        .map_err(png_error)?;
    let (tiles, palette, _) = gba_graphics(&indexed, 4.0)?;
    if tiles != [0x10; 32] || palette != [0, 0, 0x1f, 0] {
        return err("GBA graphics self-test failed");
    }
    let mut rgba = vec![0; 8 * 8 * 4];
    rgba[4] = 248;
    let image = rgba_image(&rgba, 8)?;
    if rgba_png(&image)?.pixels != rgba {
        return err("RGBA self-test failed");
    }
    let midi = b"MThd\0\0\0\x06\0\0\0\x01\0\x60MTrk\0\0\0\x04\0\xff\x2f\0";
    if midi_events(midi)?.tracks != 1 {
        return err("MIDI self-test failed");
    }
    Ok("self-test=ok".into())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn indexed(depth: png::BitDepth) -> Vec<u8> {
        let mut out = Vec::new();
        let mut encoder = png::Encoder::new(&mut out, 8, 8);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(depth);
        encoder.set_palette(vec![0, 0, 0, 248, 0, 0]);
        let data = vec![0; (8 * depth as usize).div_ceil(8) * 8];
        encoder
            .write_header()
            .unwrap()
            .write_image_data(&data)
            .unwrap();
        out
    }

    #[test]
    fn png_crate_preserves_all_indexed_depths() {
        for depth in [
            png::BitDepth::One,
            png::BitDepth::Two,
            png::BitDepth::Four,
            png::BitDepth::Eight,
        ] {
            assert_eq!(indexed_png(&indexed(depth)).unwrap().pixels.len(), 64);
        }
    }

    #[test]
    fn self_test_is_focused_and_stable() {
        assert_eq!(self_test().unwrap(), "self-test=ok");
    }
}
