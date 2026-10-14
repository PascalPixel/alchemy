use super::letters::Letters;
use super::palette::{corner_for, cut, DARK, LIGHT, LIGHT_OPACITY, SHADOW};

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
    pixels: Vec<Option<Rgb>>,
}
#[derive(Clone, Copy, PartialEq, Eq)]
pub(crate) enum Relief {
    Raised,
    Sunken,
}
pub(crate) fn blend(top: Rgb, over: Rgb, percent: u32) -> Rgb {
    [0, 1, 2].map(|c| {
        ((u32::from(top[c]) * percent + u32::from(over[c]) * (100 - percent) + 50) / 100) as u8
    })
}

impl Canvas {
    pub(crate) fn new(width: i32, height: i32, background: &str) -> Self {
        Self {
            width,
            height,
            pixels: vec![Some(rgb(background)); (width * height) as usize],
        }
    }
    #[cfg(test)]
    pub(crate) fn get(&self, x: i32, y: i32) -> Option<Rgb> {
        (x >= 0 && y >= 0 && x < self.width && y < self.height)
            .then(|| self.pixels[(y * self.width + x) as usize])
            .flatten()
    }
    fn at(&mut self, x: i32, y: i32) -> Option<&mut Option<Rgb>> {
        (x >= 0 && y >= 0 && x < self.width && y < self.height)
            .then(|| &mut self.pixels[(y * self.width + x) as usize])
    }
    fn corner_pixels(x: i32, y: i32, width: i32, height: i32) -> Vec<(i32, i32)> {
        let corner = corner_for(width, height);
        let mut out = Vec::new();
        for down in 0..corner {
            for across in 0..corner {
                if cut(across, down, corner) {
                    let (right, bottom) = (x + width - 1 - across, y + height - 1 - down);
                    out.extend([
                        (x + across, y + down),
                        (right, y + down),
                        (x + across, bottom),
                        (right, bottom),
                    ]);
                }
            }
        }
        out
    }
    pub(crate) fn rounded(&mut self, bounds: (i32, i32, i32, i32), draw: impl FnOnce(&mut Self)) {
        let (x, y, width, height) = bounds;
        let kept = Self::corner_pixels(x, y, width, height)
            .into_iter()
            .filter_map(|(px, py)| Some(((px, py), *self.at(px, py)?)))
            .collect::<Vec<_>>();
        draw(self);
        for ((px, py), pixel) in kept {
            if let Some(slot) = self.at(px, py) {
                *slot = pixel;
            }
        }
    }
    pub(crate) fn clear_corners(&mut self, x: i32, y: i32, width: i32, height: i32) {
        for (px, py) in Self::corner_pixels(x, y, width, height) {
            if let Some(slot) = self.at(px, py) {
                *slot = None;
            }
        }
    }
    fn bevel_pixel(&mut self, x: i32, y: i32, color: &str) {
        let ink = rgb(color);
        if let Some(slot) = self.at(x, y) {
            *slot = Some(match ([LIGHT, DARK, SHADOW].contains(&color), *slot) {
                (true, Some(beneath)) => blend(ink, beneath, LIGHT_OPACITY),
                _ => ink,
            });
        }
    }
    pub(crate) fn fill(&mut self, x: i32, y: i32, width: i32, height: i32, color: &str) {
        let color = Some(rgb(color));
        for row in y.max(0)..(y + height).min(self.height) {
            for column in x.max(0)..(x + width).min(self.width) {
                self.pixels[(row * self.width + column) as usize] = color;
            }
        }
    }
    pub(crate) fn rounded_fill(&mut self, x: i32, y: i32, width: i32, height: i32, color: &str) {
        self.rounded((x, y, width, height), |canvas| {
            canvas.fill(x, y, width, height, color)
        });
    }
    pub(crate) fn bevel(&mut self, x: i32, y: i32, width: i32, height: i32, relief: Relief) {
        let (first, second) = match relief {
            Relief::Raised => (LIGHT, DARK),
            Relief::Sunken => (DARK, LIGHT),
        };
        let corner = corner_for(width, height);
        let (right, bottom) = (x + width - 1, y + height - 1);
        let edge = |across: i32, down: i32| {
            let near = |offset: i32, span: i32| offset.min(span - 1 - offset);
            let (a, d) = (near(across, width), near(down, height));
            !cut(a, d, corner) && (cut(a - 1, d, corner) || cut(a, d - 1, corner))
        };
        for down in 0..height {
            for across in 0..width {
                if !edge(across, down) {
                    continue;
                }
                let (px, py) = (x + across, y + down);
                let top_left = across < width - 1 - across && down < height - 1 - down;
                let on_edge = px == x || py == y || px == right || py == bottom;
                let second_side = if on_edge {
                    px == right || py == bottom
                } else {
                    !top_left
                };
                self.bevel_pixel(px, py, if second_side { second } else { first });
            }
        }
    }
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
                            self.bevel_pixel(at + column as i32 + dx, top + row as i32, ink);
                        }
                    }
                }
                at += letters.advance[frame] as i32;
            }
        }
        letters.width(text) as i32
    }
    pub(crate) fn mark(&mut self, x: i32, y: i32, name: &str, color: &str, shadow: &str) -> i32 {
        for (ink, dx) in [(shadow, 1), (color, 0)] {
            for (row, line) in super::letters::mark(name).iter().enumerate() {
                for (column, pixel) in line.chars().enumerate() {
                    if pixel == '#' {
                        self.bevel_pixel(x + column as i32 + dx, y + row as i32 + dx, ink);
                    }
                }
            }
        }
        super::letters::mark_width(name) as i32
    }
    #[cfg(test)]
    pub(crate) fn rgba(&self, scale: u32) -> Vec<u8> {
        let scale = scale as usize;
        let mut out = Vec::with_capacity(self.pixels.len() * scale * scale * 4);
        for row in self.pixels.chunks(self.width as usize) {
            let wide = row
                .iter()
                .map(|pixel| match pixel {
                    Some([r, g, b]) => [*r, *g, *b, 255],
                    None => [0; 4],
                })
                .flat_map(|pixel| std::iter::repeat_n(pixel, scale).flatten())
                .collect::<Vec<_>>();
            for _ in 0..scale {
                out.extend_from_slice(&wide);
            }
        }
        out
    }
    pub(crate) fn png(&self, scale: u32, date: &str) -> Result<Vec<u8>, String> {
        let mut palette: Vec<Rgb> = Vec::new();
        let mut index = std::collections::HashMap::new();
        let clear = self.pixels.contains(&None);
        if clear {
            palette.push([0; 3]);
            index.insert(None, 0);
        }
        let mut indices = Vec::with_capacity(self.pixels.len());
        for pixel in &self.pixels {
            let slot = *index.entry(*pixel).or_insert_with(|| {
                palette.push(pixel.unwrap_or_default());
                palette.len() - 1
            });
            if slot > 255 {
                return Err("figure uses more than 256 colours".into());
            }
            indices.push(slot as u8);
        }
        let depth: usize = match palette.len() {
            0..=2 => 1,
            3..=4 => 2,
            5..=16 => 4,
            _ => 8,
        };
        let (width, height) = (self.width as u32 * scale, self.height as u32 * scale);
        let stride = (width as usize * depth).div_ceil(8);
        let mut raw = Vec::with_capacity((stride + 1) * height as usize);
        let mut above: Option<Vec<u8>> = None;
        for line in indices.chunks(self.width as usize) {
            let mut packed = vec![0u8; stride];
            let wide = line
                .iter()
                .flat_map(|index| std::iter::repeat_n(*index, scale as usize));
            for (x, index) in wide.enumerate() {
                let bit = x * depth;
                packed[bit / 8] |= index << (8 - depth - bit % 8);
            }
            for _ in 0..scale {
                match &above {
                    Some(previous) => {
                        raw.push(2);
                        raw.extend(packed.iter().zip(previous).map(|(a, b)| a.wrapping_sub(*b)));
                    }
                    None => {
                        raw.push(0);
                        raw.extend(&packed);
                    }
                }
                above = Some(packed.clone());
            }
        }
        let idat = fdeflate::compress_to_vec(&raw);
        let mut parts = date.splitn(3, '-').map(|part| part.parse::<u16>().ok());
        let (Some(Some(year)), Some(Some(month)), Some(Some(day))) =
            (parts.next(), parts.next(), parts.next())
        else {
            return Err(format!("{date:?} is not a date"));
        };
        let mut time = year.to_be_bytes().to_vec();
        time.extend([month as u8, day as u8, 0, 0, 0]);
        let mut header = width.to_be_bytes().to_vec();
        header.extend(height.to_be_bytes());
        header.extend([depth as u8, 3, 0, 0, 0]);
        let mut out = b"\x89PNG\r\n\x1a\n".to_vec();
        out.extend(chunk(b"IHDR", &header));
        out.extend(chunk(b"PLTE", &palette.concat()));
        if clear {
            out.extend(chunk(b"tRNS", &[0]));
        }
        out.extend(chunk(b"IDAT", &idat));
        out.extend(chunk(b"tIME", &time));
        out.extend(chunk(b"IEND", &[]));
        Ok(out)
    }
}
#[cfg(test)]
pub(crate) fn decode(bytes: &[u8]) -> Option<(u32, u32, Vec<u8>)> {
    let mut decoder = png::Decoder::new(std::io::Cursor::new(bytes));
    decoder.set_transformations(png::Transformations::EXPAND);
    let mut reader = decoder.read_info().ok()?;
    let mut data = vec![0; reader.output_buffer_size()];
    let info = reader.next_frame(&mut data).ok()?;
    if info.bit_depth != png::BitDepth::Eight {
        return None;
    }
    data.truncate(info.buffer_size());
    let data = match info.color_type {
        png::ColorType::Rgba => data,
        png::ColorType::Rgb => data
            .chunks(3)
            .flat_map(|pixel| [pixel[0], pixel[1], pixel[2], 255])
            .collect(),
        _ => return None,
    };
    Some((info.width, info.height, data))
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

#[cfg(test)]
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
        let letters = crate::progress::letters::fixture();
        let mut canvas = Canvas::new(8, 16, "#000");
        canvas.fill(0, 0, 1, 1, "#ff0000");
        assert_eq!(canvas.text(&letters, 0, 0, "A", "#fff", Some("#123456")), 4);
        assert_eq!(canvas.get(0, 8), Some([255, 255, 255]));
        assert_eq!(canvas.get(3, 11), Some(rgb("#123456")));
        canvas.line((0, 15), (7, 15), 1, "#00ff00");
        assert_eq!(canvas.get(7, 15), Some([0, 255, 0]));
        let png = canvas.png(2, "2026-09-24").unwrap();
        assert_eq!(png_date(&png).as_deref(), Some("2026-09-24"));
        let (width, height, data) = decode(&png).unwrap();
        assert_eq!((width, height), (16, 32));
        assert_eq!(data, canvas.rgba(2));
        for at in [0, 4, 64, 68] {
            assert_eq!(&data[at..at + 4], [255, 0, 0, 255]);
        }
        assert_ne!(&data[8..11], [255, 0, 0]);
        assert_eq!(canvas.png(2, "2026-09-24").unwrap(), png);
    }
    #[test]
    fn boxes_step_their_corners_and_lay_translucent_light_bevels() {
        use crate::progress::palette::{FACE, LIGHT_OPACITY, WELL};
        let mut canvas = Canvas::new(12, 10, FACE);
        canvas.rounded_fill(2, 2, 8, 6, WELL);
        canvas.bevel(2, 2, 8, 6, Relief::Raised);
        let (face, light, dark) = (rgb(FACE), rgb(LIGHT), rgb(DARK));
        for (x, y) in [(2, 2), (9, 2), (2, 7), (9, 7)] {
            assert_eq!(canvas.get(x, y), Some(face), "{x},{y}");
        }
        let (o, rest) = (LIGHT_OPACITY, 100 - LIGHT_OPACITY);
        let over_well = blend(light, rgb(WELL), o);
        assert_eq!(
            over_well,
            [
                (201 * o + 0x17 * rest + 50) / 100,
                (225 * o + 0x60 * rest + 50) / 100,
                (220 * o + 0x6f * rest + 50) / 100,
            ]
            .map(|c: u32| c as u8)
        );
        for (x, y) in [(3, 2), (8, 2), (2, 3), (2, 6)] {
            assert_eq!(canvas.get(x, y), Some(over_well), "{x},{y}");
        }
        for (x, y) in [(9, 3), (9, 6), (3, 7), (8, 7)] {
            assert_eq!(canvas.get(x, y), Some(blend(dark, rgb(WELL), o)), "{x},{y}");
        }
        assert_eq!(canvas.get(3, 3), Some(rgb(WELL)));
        canvas.bevel(2, 2, 8, 6, Relief::Sunken);
        assert_eq!(
            canvas.get(9, 3),
            Some(blend(light, blend(dark, rgb(WELL), o), o))
        );
        assert_eq!(canvas.get(3, 2), Some(blend(dark, over_well, o)));
        canvas.clear_corners(0, 0, 12, 10);
        assert_eq!(canvas.get(0, 0), None);
        assert_eq!(canvas.get(1, 0), Some(face));
        let png = canvas.png(2, "2026-09-24").unwrap();
        let (_, _, data) = decode(&png).unwrap();
        assert_eq!(&data[..8], [0, 0, 0, 0, 0, 0, 0, 0]);
        assert_eq!(data, canvas.rgba(2));
    }
    #[test]
    fn every_bit_depth_decodes_to_the_rendered_pixels() {
        for colours in [2, 3, 7, 40] {
            let mut canvas = Canvas::new(5, 3, "#000");
            for n in 0..colours {
                canvas.fill(
                    n % 5,
                    (n / 5) % 3,
                    1,
                    1,
                    &format!("#{:02x}{:02x}10", n * 6, 255 - n),
                );
            }
            let png = canvas.png(2, "2026-09-24").unwrap();
            assert_eq!(decode(&png), Some((10, 6, canvas.rgba(2))), "{colours}");
        }
    }
}
