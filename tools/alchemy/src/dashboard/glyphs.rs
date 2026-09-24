//! The dashboard's lettering and tab icons. Every string is drawn from the
//! tracked glyph sheet (see `coverage::letters`), cut as CSS mask sprites at
//! one pixel scale; the icons come from the tracked icon banks and the ROM
//! palette. Both images are built into the dashboard cache and served from
//! there; neither is a font file and neither may ever be committed.
use super::cache::{self, Store};
use crate::build_assets::{icon_bank_source, raw_palette_bank, ICON_BANKS, ICON_PALETTE_BANK};
use crate::coverage::letters::{Letters, LINE, PIXEL, SHEET};
use psynergy::assets::image::{indexed_png, IndexedImage};
use std::{collections::BTreeMap, path::Path, sync::Mutex};

/// Each tab's icon: an icon bank the graphics review identifies and a frame.
pub(super) const TAB_ICONS: [(&str, u8, u32); 5] = [
    ("Files", 4, 148),
    ("ROM coverage", 4, 137),
    ("Music", 5, 53),
    ("Maps", 4, 167),
    ("Text", 4, 176),
];
/// The served glyph sheet is drawn at two device pixels per game pixel, so a
/// retina display shows it without resampling.
const SHEET_SCALE: u32 = 2;
const SHEET_COLUMNS: u32 = 16;

#[derive(Clone, Default)]
pub(super) struct Assets {
    pub letters: Option<(String, Letters)>,
    pub icons: Option<String>,
}
static ASSETS: Mutex<Assets> = Mutex::new(Assets {
    letters: None,
    icons: None,
});
pub(super) fn current() -> Assets {
    ASSETS.lock().unwrap_or_else(|e| e.into_inner()).clone()
}

/// Build (or reuse) the cached images; each failure leaves the other usable.
pub(super) fn refresh(root: &Path) -> Result<String, String> {
    let store = Store::at(root);
    let letters = letters_file(root, &store);
    let icons = icon_file(root, &store);
    let mut assets = ASSETS.lock().unwrap_or_else(|e| e.into_inner());
    assets.letters = letters.as_ref().ok().cloned();
    assets.icons = icons.as_ref().ok().cloned();
    match (letters, icons) {
        (Ok(_), Ok(_)) => Ok("lettering and icons cached".into()),
        (Err(error), _) => Err(format!("lettering: {error}")),
        (_, Err(error)) => Err(format!("icons: {error}")),
    }
}
/// A cached file's bytes by its published name, `letters-<stamp>.png` or
/// `icons-<stamp>.png`; only current stamps are served.
pub(super) fn file(root: &Path, name: &str) -> Option<(&'static str, Vec<u8>)> {
    let assets = current();
    let (stem, extension) = name.rsplit_once('.')?;
    let (kind, stamp) = stem.split_once('-')?;
    let current = match (kind, extension) {
        ("letters", "png") => assets.letters.map(|(stamp, _)| stamp),
        ("icons", "png") => assets.icons,
        _ => return None,
    };
    (current.as_deref() == Some(stamp))
        .then(|| Store::at(root).load(kind, stamp, extension))
        .flatten()
        .map(|bytes| ("image/png", bytes))
}

fn read(root: &Path, path: &str) -> Result<Vec<u8>, String> {
    std::fs::read(root.join(path)).map_err(|error| format!("{path}: {error}"))
}
fn letters_file(root: &Path, store: &Store) -> Result<(String, Letters), String> {
    let letters = Letters::load(root)?;
    let mut parts = vec![cache::code_identity().into_bytes(), read(root, SHEET)?];
    parts.extend(letters.rows.iter().map(|rows| {
        rows.iter()
            .flat_map(|row| row.to_le_bytes())
            .collect::<Vec<_>>()
    }));
    let stamp = cache::stamp(&parts.iter().map(Vec::as_slice).collect::<Vec<_>>());
    store.get_or_build("letters", &stamp, "png", || mask_sheet(&letters))?;
    Ok((stamp, letters))
}
/// The sheet as an alpha mask, ink opaque, `SHEET_SCALE` device pixels to
/// the game pixel, sixteen frames to a row.
fn mask_sheet(letters: &Letters) -> Result<Vec<u8>, String> {
    let frames = letters.rows.len() as u32;
    let (width, height) = (
        SHEET_COLUMNS * LINE * SHEET_SCALE,
        frames.div_ceil(SHEET_COLUMNS) * LINE * SHEET_SCALE,
    );
    let mut alpha = vec![0u8; (width * height) as usize];
    for frame in 0..frames {
        let (left, top) = (frame % SHEET_COLUMNS * LINE, frame / SHEET_COLUMNS * LINE);
        for y in 0..LINE * SHEET_SCALE {
            for x in 0..LINE * SHEET_SCALE {
                if letters.ink(frame as usize, x / SHEET_SCALE, y / SHEET_SCALE) {
                    let at = (top * SHEET_SCALE + y) * width + left * SHEET_SCALE + x;
                    alpha[at as usize] = 255;
                }
            }
        }
    }
    let mut out = Vec::new();
    let mut encoder = png::Encoder::new(&mut out, width, height);
    encoder.set_color(png::ColorType::GrayscaleAlpha);
    encoder.set_depth(png::BitDepth::Eight);
    let pixels = alpha.iter().flat_map(|a| [255, *a]).collect::<Vec<_>>();
    encoder
        .write_header()
        .and_then(|mut writer| writer.write_image_data(&pixels))
        .map_err(|e| e.to_string())?;
    Ok(out)
}
/// The lettering rules: a run of glyph sprites per text node, one class per
/// code with its advance and place on the sheet, lengths in game pixels.
pub(super) fn lettering_css() -> String {
    let Some((stamp, letters)) = current().letters else {
        return String::new();
    };
    let columns = SHEET_COLUMNS;
    let mut css = format!(
        ".t i{{display:block;height:{LINE}px;background:currentColor;-webkit-mask:url(/cache/letters-{stamp}.png) 0 0/{}px {}px no-repeat;mask:url(/cache/letters-{stamp}.png) 0 0/{}px {}px no-repeat}}",
        columns * LINE,
        (letters.rows.len() as u32).div_ceil(columns) * LINE,
        columns * LINE,
        (letters.rows.len() as u32).div_ceil(columns) * LINE,
    );
    for frame in 0..letters.rows.len() as u32 {
        let advance = letters.advance[frame as usize];
        if advance == 0 {
            continue;
        }
        let (x, y) = (frame % columns * LINE, frame / columns * LINE);
        css.push_str(&format!(
            ".t .c{:02x}{{width:{advance}px;-webkit-mask-position:-{x}px -{y}px;mask-position:-{x}px -{y}px}}",
            frame + 0x20
        ));
    }
    super::chrome::pixels(&css)
}
/// Every text node of a page's body drawn from the sheet: words as unbroken
/// sprite runs, the text itself kept for readers, search and copying.
/// Script, style, title and option text is left alone, and so is any
/// character the sheet lacks, which falls back to the system face.
pub(super) fn letter(html: &str) -> String {
    let Some((_, letters)) = current().letters else {
        return html.to_string();
    };
    letter_with(&letters, html)
}
pub(super) fn letter_with(letters: &Letters, html: &str) -> String {
    let mut out = String::with_capacity(html.len() * 3);
    let mut rest = html;
    let mut raw: Option<&str> = None;
    let mut body = false;
    while !rest.is_empty() {
        let next = rest.find('<').unwrap_or(rest.len());
        let (text, tail) = rest.split_at(next);
        if body && raw.is_none() && !text.trim().is_empty() {
            out.push_str(&run(letters, &unescape(text)));
        } else {
            out.push_str(text);
        }
        if tail.is_empty() {
            break;
        }
        let end = tail.find('>').map_or(tail.len(), |at| at + 1);
        let tag = &tail[..end];
        out.push_str(tag);
        rest = &tail[end..];
        let name = tag
            .trim_start_matches('<')
            .split(|c: char| c.is_whitespace() || c == '>' || c == '/')
            .find(|part| !part.is_empty())
            .unwrap_or("")
            .to_ascii_lowercase();
        let closing = tag.starts_with("</");
        match raw {
            Some(open) if closing && name == open => raw = None,
            Some(_) => {}
            None if !closing
                && matches!(
                    name.as_str(),
                    "script" | "style" | "title" | "option" | "textarea"
                ) =>
            {
                raw = Some(match name.as_str() {
                    "script" => "script",
                    "style" => "style",
                    "title" => "title",
                    "option" => "option",
                    _ => "textarea",
                })
            }
            None if name == "body" => body = !closing,
            None => {}
        }
    }
    out
}
fn unescape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    let mut rest = text;
    while let Some(at) = rest.find('&') {
        out.push_str(&rest[..at]);
        rest = &rest[at..];
        let Some(end) = rest.find(';').filter(|end| *end <= 10) else {
            out.push('&');
            rest = &rest[1..];
            continue;
        };
        let entity = &rest[1..end];
        let decoded = match entity {
            "amp" => Some('&'),
            "lt" => Some('<'),
            "gt" => Some('>'),
            "quot" => Some('"'),
            "apos" => Some('\''),
            "nbsp" => Some('\u{a0}'),
            _ => entity
                .strip_prefix("#x")
                .map(|hex| u32::from_str_radix(hex, 16))
                .or_else(|| entity.strip_prefix('#').map(str::parse))
                .and_then(Result::ok)
                .and_then(char::from_u32),
        };
        match decoded {
            Some(character) => {
                out.push(character);
                rest = &rest[end + 1..];
            }
            None => {
                out.push('&');
                rest = &rest[1..];
            }
        }
    }
    out.push_str(rest);
    out
}
/// One text node as `<span class="t">`: the hidden text, then each word as an
/// unbreakable run of glyphs with its following space, lines split at
/// newlines, and characters the sheet lacks in the system face.
fn run(letters: &Letters, text: &str) -> String {
    let escaped = crate::coverage::boxtree::esc(text);
    let mut out =
        format!("<span class=\"t\"><span class=\"sr\">{escaped}</span><span aria-hidden=\"true\">");
    let mut word = String::new();
    let mut fallback = String::new();
    let flush_fallback = |word: &mut String, fallback: &mut String| {
        if !fallback.is_empty() {
            word.push_str(&format!(
                "<span class=\"f\">{}</span>",
                crate::coverage::boxtree::esc(fallback)
            ));
            fallback.clear();
        }
    };
    for character in text.chars() {
        if character == '\n' {
            flush_fallback(&mut word, &mut fallback);
            if !word.is_empty() {
                out.push_str(&format!("<b>{word}</b>"));
                word.clear();
            }
            out.push_str("<br>");
            continue;
        }
        let character = if character == '\t' { ' ' } else { character };
        match letters.frame(character) {
            Some(frame) => {
                flush_fallback(&mut word, &mut fallback);
                word.push_str(&format!("<i class=\"c{:02x}\"></i>", frame + 0x20));
            }
            None => fallback.push(character),
        }
        if character == ' ' {
            out.push_str(&format!("<b>{word}</b>"));
            word.clear();
        }
    }
    flush_fallback(&mut word, &mut fallback);
    if !word.is_empty() {
        out.push_str(&format!("<b>{word}</b>"));
    }
    out.push_str("</span></span>");
    out
}
/// A glyph run's width in CSS pixels, for layout that must know it ahead.
pub(super) fn width(text: &str) -> u32 {
    match current().letters {
        Some((_, letters)) => letters.width(text) * PIXEL,
        None => text.chars().count() as u32 * 8 * PIXEL,
    }
}

fn icon_file(root: &Path, store: &Store) -> Result<String, String> {
    let target = crate::targets::decomp_target(Some("tbs-en"))?;
    let rom = read(root, target.rom)?;
    let colors = raw_palette_bank(root, &target, &rom, ICON_PALETTE_BANK)?;
    let mut banks = BTreeMap::new();
    for bank in ICON_BANKS {
        banks.insert(bank, read(root, &icon_bank_source(bank))?);
    }
    let palette = colors
        .iter()
        .flat_map(|color| color.to_le_bytes())
        .collect::<Vec<_>>();
    let mut parts = vec![cache::code_identity().into_bytes(), palette];
    parts.extend(banks.values().cloned());
    let stamp = cache::stamp(&parts.iter().map(Vec::as_slice).collect::<Vec<_>>());
    store.get_or_build("icons", &stamp, "png", || {
        let images = banks
            .iter()
            .map(|(bank, bytes)| Ok((*bank, indexed_png(bytes).map_err(|e| e.to_string())?)))
            .collect::<Result<BTreeMap<_, _>, String>>()?;
        icon_strip(&images, &colors)
    })?;
    Ok(stamp)
}

/// One 16x16 frame per tab, index zero transparent, in `TAB_ICONS` order.
fn icon_strip(banks: &BTreeMap<u8, IndexedImage>, colors: &[u16; 16]) -> Result<Vec<u8>, String> {
    let width = 16 * TAB_ICONS.len();
    let mut rgba = vec![0u8; width * 16 * 4];
    for (slot, (_, bank, frame)) in TAB_ICONS.iter().enumerate() {
        let image = banks.get(bank).ok_or("icon bank absent")?;
        let columns = image.width / 16;
        if *frame >= columns * (image.height / 16) {
            return Err(format!("icon {frame} lies outside bank {bank}"));
        }
        let (left, top) = (frame % columns * 16, frame / columns * 16);
        for y in 0..16 {
            for x in 0..16 {
                let index = image.pixels[((top + y) * image.width + left + x) as usize] as usize;
                if index == 0 {
                    continue;
                }
                let color = colors[index & 15];
                let at = ((y as usize) * width + slot * 16 + x as usize) * 4;
                for (channel, shift) in [0, 5, 10].into_iter().enumerate() {
                    let value = (color >> shift) & 31;
                    rgba[at + channel] = ((value << 3) | (value >> 2)) as u8;
                }
                rgba[at + 3] = 255;
            }
        }
    }
    let mut out = Vec::new();
    let mut encoder = png::Encoder::new(&mut out, width as u32, 16);
    encoder.set_color(png::ColorType::Rgba);
    encoder.set_depth(png::BitDepth::Eight);
    encoder
        .write_header()
        .and_then(|mut writer| writer.write_image_data(&rgba))
        .map_err(|e| e.to_string())?;
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn body_text_becomes_glyph_runs_and_raw_text_does_not() {
        let letters = crate::coverage::letters::fixture();
        let html = "<html><head><title>A&amp;B</title><style>p{x:1}</style></head><body><a class=\"tab\">Up &amp; 神</a><select><option>Keep</option></select><script>let a=1<2</script>\n</body></html>";
        let lettered = letter_with(&letters, html);
        assert!(
            lettered.contains("<title>A&amp;B</title>")
                && lettered.contains("<style>p{x:1}</style>")
        );
        assert!(
            lettered.contains("<option>Keep</option>")
                && lettered.contains("<script>let a=1<2</script>")
        );
        assert!(lettered.contains("<span class=\"sr\">Up &amp; 神</span>"));
        assert!(lettered
            .contains("<b><i class=\"c55\"></i><i class=\"c70\"></i><i class=\"c20\"></i></b>"));
        assert!(lettered.contains(
            "<b><i class=\"c26\"></i><i class=\"c20\"></i></b><b><span class=\"f\">神</span></b>"
        ));
        assert_eq!(unescape("&lt;a&gt; &#233;&#x41;&unknown"), "<a> éA&unknown");
    }
    #[test]
    fn served_names_accept_only_current_stamps() {
        let root = tempfile::tempdir().unwrap();
        assert!(file(root.path(), "letters-0123456789abcdef.png").is_none());
        assert!(file(root.path(), "font-0123456789abcdef.ttf").is_none());
        assert!(file(root.path(), "../letters-x.png").is_none());
        assert!(file(root.path(), "icons.png").is_none());
    }
    #[test]
    fn the_mask_sheet_doubles_every_glyph_pixel() {
        let letters = crate::coverage::letters::fixture();
        let png = mask_sheet(&letters).unwrap();
        let decoder = png::Decoder::new(std::io::Cursor::new(png));
        let reader = decoder.read_info().unwrap();
        assert_eq!((reader.info().width, reader.info().height), (512, 448));
    }
}
