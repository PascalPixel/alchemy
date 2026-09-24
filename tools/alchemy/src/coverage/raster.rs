//! A small pixel canvas for the README figures: whole game pixels, flat
//! colours, glyphs from the tracked sheet, written as an indexed PNG at
//! `FIGURE_SCALE` device pixels per game pixel, dated by its tIME chunk
//! (the only standard chunk the publication check lets a date ride in).
use super::letters::Letters;

pub(crate) type Rgb = [u8; 3];

pub(crate) fn rgb(hex: &str) -> Rgb {
    let digits = hex.trim_start_matches('#');
    let digits = if digits.len() == 3 {
        digits.chars().flat_map(|c| [c, c]).collect::<String>()
    } else {
        digits.to_string()
    };
    let channel = |at: usize| u8::from_str_radix(&digits[at..at + 2], 16).unwrap_or(0);
    [channel(0), channel(2), channel(4)]
}

pub(crate) struct Canvas {
    pub width: i32,
    pub height: i32,
    pixels: Vec<Rgb>,
}

impl Canvas {
    pub(crate) fn new(width: i32, height: i32, background: &str) -> Self {
        Self {
            width,
            height,
            pixels: vec![rgb(background); (width * height) as usize],
        }
    }
    #[cfg(test)]
    pub(crate) fn get(&self, x: i32, y: i32) -> Option<Rgb> {
        (x >= 0 && y >= 0 && x < self.width && y < self.height)
            .then(|| self.pixels[(y * self.width + x) as usize])
    }
    pub(crate) fn fill(&mut self, x: i32, y: i32, width: i32, height: i32, color: &str) {
        let color = rgb(color);
        for row in y.max(0)..(y + height).min(self.height) {
            for column in x.max(0)..(x + width).min(self.width) {
                self.pixels[(row * self.width + column) as usize] = color;
            }
        }
    }
    /// A one-pixel frame, light along the top and left, dark along the
    /// bottom and right.
    pub(crate) fn bevel(
        &mut self,
        x: i32,
        y: i32,
        width: i32,
        height: i32,
        light: &str,
        dark: &str,
    ) {
        self.fill(x, y, width, 1, light);
        self.fill(x, y, 1, height, light);
        self.fill(x, y + height - 1, width, 1, dark);
        self.fill(x + width - 1, y, 1, height, dark);
    }
    /// A line `thickness` pixels wide from one point to another.
    pub(crate) fn line(&mut self, from: (i32, i32), to: (i32, i32), thickness: i32, color: &str) {
        let (mut x, mut y) = from;
        let (dx, dy) = ((to.0 - x).abs(), -(to.1 - y).abs());
        let (sx, sy) = ((to.0 - x).signum(), (to.1 - y).signum());
        let mut error = dx + dy;
        loop {
            self.fill(x, y, thickness, thickness, color);
            if (x, y) == to {
                break;
            }
            let twice = 2 * error;
            if twice >= dy {
                error += dy;
                x += sx;
            }
            if twice <= dx {
                error += dx;
                y += sy;
            }
        }
    }
    /// Text in a line box whose top is `y`, each glyph's ink in `color` over
    /// a shadow one pixel down and right; returns the advance.
    pub(crate) fn text(
        &mut self,
        letters: &Letters,
        x: i32,
        y: i32,
        text: &str,
        color: &str,
        shadow: Option<&str>,
    ) -> i32 {
        for (ink, dx) in shadow
            .map(|shadow| (shadow, 1))
            .into_iter()
            .chain([(color, 0)])
        {
            let mut at = x;
            for character in text.chars() {
                let frame = letters.stand_in(character);
                let top = y + letters.top as i32 + dx;
                for row in 0..letters.cell.1 {
                    for column in 0..letters.cell.0 {
                        if letters.ink(frame, column, row) {
                            self.fill(at + column as i32 + dx, top + row as i32, 1, 1, ink);
                        }
                    }
                }
                at += letters.advance[frame] as i32;
            }
        }
        letters.width(text) as i32
    }
    /// The canvas as an indexed PNG, each pixel `scale` device pixels wide,
    /// stamped with `date` (`YYYY-MM-DD`) at midnight.
    pub(crate) fn png(&self, scale: u32, date: &str) -> Result<Vec<u8>, String> {
        let mut palette: Vec<Rgb> = Vec::new();
        let mut index = std::collections::HashMap::new();
        let mut indices = Vec::with_capacity(self.pixels.len());
        for pixel in &self.pixels {
            let slot = *index.entry(*pixel).or_insert_with(|| {
                palette.push(*pixel);
                palette.len() - 1
            });
            if slot > 255 {
                return Err("figure uses more than 256 colours".into());
            }
            indices.push(slot as u8);
        }
        let (width, height) = (self.width as u32 * scale, self.height as u32 * scale);
        let mut data = Vec::with_capacity((width * height) as usize);
        for row in 0..self.height as usize {
            let line = &indices[row * self.width as usize..(row + 1) * self.width as usize];
            let wide = line
                .iter()
                .flat_map(|index| std::iter::repeat(*index).take(scale as usize))
                .collect::<Vec<_>>();
            for _ in 0..scale {
                data.extend_from_slice(&wide);
            }
        }
        let mut out = Vec::new();
        let mut encoder = png::Encoder::new(&mut out, width, height);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(palette.concat());
        encoder.set_compression(png::Compression::Best);
        encoder
            .write_header()
            .and_then(|mut writer| writer.write_image_data(&data))
            .map_err(|e| e.to_string())?;
        let mut parts = date.splitn(3, '-').map(|part| part.parse::<u16>().ok());
        let (Some(Some(year)), Some(Some(month)), Some(Some(day))) =
            (parts.next(), parts.next(), parts.next())
        else {
            return Err(format!("{date:?} is not a date"));
        };
        let mut time = year.to_be_bytes().to_vec();
        time.extend([month as u8, day as u8, 0, 0, 0]);
        // The chunk goes just before IEND, the last twelve bytes.
        let end = out.len() - 12;
        out.splice(end..end, chunk(b"tIME", &time));
        Ok(out)
    }
}
fn chunk(kind: &[u8; 4], body: &[u8]) -> Vec<u8> {
    let mut out = (body.len() as u32).to_be_bytes().to_vec();
    out.extend(kind);
    out.extend(body);
    let crc = !out[4..].iter().fold(!0u32, |crc, byte| {
        (0..8).fold(crc ^ u32::from(*byte), |crc, _| {
            (crc >> 1) ^ (0xedb8_8320 & (crc & 1).wrapping_neg())
        })
    });
    out.extend(crc.to_be_bytes());
    out
}

/// The date of a PNG's tIME chunk, `YYYY-MM-DD`.
pub(crate) fn png_date(bytes: &[u8]) -> Option<String> {
    let mut rest = bytes.get(8..)?;
    while rest.len() >= 12 {
        let length = u32::from_be_bytes(rest[..4].try_into().ok()?) as usize;
        let (kind, body) = (&rest[4..8], rest.get(8..8 + length)?);
        if kind == b"tIME" && length == 7 {
            let year = u16::from_be_bytes([body[0], body[1]]);
            return Some(format!("{year:04}-{:02}-{:02}", body[2], body[3]));
        }
        rest = rest.get(12 + length..)?;
    }
    None
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn a_canvas_scales_whole_pixels_and_keeps_its_text_chunks() {
        let letters = crate::coverage::letters::fixture();
        let mut canvas = Canvas::new(8, 16, "#000");
        canvas.fill(0, 0, 1, 1, "#ff0000");
        assert_eq!(canvas.text(&letters, 0, 0, "A", "#fff", Some("#123456")), 4);
        assert_eq!(canvas.get(0, 8), Some([255, 255, 255]));
        assert_eq!(canvas.get(3, 11), Some(rgb("#123456")));
        canvas.line((0, 15), (7, 15), 1, "#00ff00");
        assert_eq!(canvas.get(7, 15), Some([0, 255, 0]));
        let png = canvas.png(2, "2026-09-24").unwrap();
        assert_eq!(png_date(&png).as_deref(), Some("2026-09-24"));
        let mut decoder = png::Decoder::new(std::io::Cursor::new(&png));
        decoder.set_transformations(png::Transformations::EXPAND);
        let mut reader = decoder.read_info().unwrap();
        assert_eq!((reader.info().width, reader.info().height), (16, 32));
        let mut data = vec![0; reader.output_buffer_size()];
        reader.next_frame(&mut data).unwrap();
        // The red game pixel covers two by two device pixels, and no more.
        for at in [0, 3, 48, 51] {
            assert_eq!(&data[at..at + 3], [255, 0, 0]);
        }
        assert_ne!(&data[6..9], [255, 0, 0]);
        assert_eq!(canvas.png(2, "2026-09-24").unwrap(), png);
    }
}
