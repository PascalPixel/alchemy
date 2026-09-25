//! The dashboard's lettering and tab icons. Every string is drawn from the
//! tracked glyph sheets (see `coverage::letters`), cut as CSS mask sprites at
//! one pixel scale: labels in the upright menu font, quoted game text in the
//! dialogue fonts. The icons come from the tracked icon banks and the ROM
//! palette; Weyard UI's logo marks from their masks in `coverage::letters`. The images are built into the dashboard cache and served from
//! there; none is a font file and none may ever be committed.
use super::cache::{self, Store};
use crate::build_assets::{icon_bank_source, raw_palette_bank, ICON_BANKS, ICON_PALETTE_BANK};
use crate::coverage::letters::{Letters, LINE, MARKS, PIXEL};
use psynergy::assets::image::{indexed_png, IndexedImage};
use std::{collections::BTreeMap, path::Path, sync::Mutex};

/// Each tab's icon: an icon bank the graphics review identifies and a frame.
pub(super) const TAB_ICONS: [(&str, u8, u32); 5] = [
    ("Files", 4, 254),
    ("ROM coverage", 4, 137),
    ("Music", 5, 53),
    ("Maps", 4, 175),
    ("Text", 4, 176),
];
/// The served glyph sheets are drawn at two device pixels per game pixel,
/// so a retina display shows them without resampling.
pub(super) const SHEET_SCALE: u32 = 2;
pub(super) const SHEET_COLUMNS: u32 = 16;

/// The faces by the class that selects them: `u` the menu font for every
/// label, `d` the Western dialogue font and `j` the Japanese one for quoted
/// game text (a `data-face` attribute on the element that holds it).
pub(super) const FACES: [&str; 3] = ["u", "d", "j"];

#[derive(Clone)]
pub(super) struct Face {
    pub class: &'static str,
    pub stamp: String,
    pub letters: Letters,
}
#[derive(Clone, Default)]
pub(super) struct Assets {
    pub faces: Vec<Face>,
    pub icons: Option<String>,
}
static ASSETS: Mutex<Assets> = Mutex::new(Assets {
    faces: Vec::new(),
    icons: None,
});
pub(super) fn current() -> Assets {
    ASSETS.lock().unwrap_or_else(|e| e.into_inner()).clone()
}
impl Assets {
    pub(super) fn face(&self, class: &str) -> Option<&Face> {
        self.faces.iter().find(|face| face.class == class)
    }
}

/// Build (or reuse) the cached images; each failure leaves the rest usable.
pub(super) fn refresh(root: &Path) -> Result<String, String> {
    let store = Store::at(root);
    let mut faces = Vec::new();
    let mut failures = Vec::new();
    for class in FACES {
        let letters = match class {
            "u" => Letters::menu(root),
            "d" => Letters::dialogue(root),
            _ => Letters::japanese(root),
        };
        match letters.and_then(|letters| face_file(&store, class, letters)) {
            Ok(face) => faces.push(face),
            Err(error) => failures.push(format!("{class}: {error}")),
        }
    }
    let icons = icon_file(root, &store);
    let mut assets = ASSETS.lock().unwrap_or_else(|e| e.into_inner());
    assets.faces = faces;
    assets.icons = icons.as_ref().ok().cloned();
    if let Err(error) = icons {
        failures.push(format!("icons: {error}"));
    }
    if failures.is_empty() {
        Ok("lettering and icons cached".into())
    } else {
        Err(failures.join("; "))
    }
}
/// A cached file's bytes by its published name, `letters<class>-<stamp>.png`,
/// `icons-<stamp>.png` or `marks-<stamp>.png`; only current stamps are served.
pub(super) fn file(root: &Path, name: &str) -> Option<(&'static str, Vec<u8>)> {
    let assets = current();
    let (stem, extension) = name.rsplit_once('.')?;
    let (kind, stamp) = stem.split_once('-')?;
    if extension != "png" {
        return None;
    }
    let known = match kind {
        _ if kind.starts_with("letters") => assets
            .faces
            .iter()
            .any(|face| kind == format!("letters{}", face.class) && face.stamp == stamp),
        "icons" => assets.icons.as_deref() == Some(stamp),
        "marks" => {
            let (current, sheet) = marks();
            return (stamp == current).then(|| ("image/png", sheet));
        }
        _ => false,
    };
    known
        .then(|| Store::at(root).load(kind, stamp, extension))
        .flatten()
        .map(|bytes| ("image/png", bytes))
}

fn read(root: &Path, path: &str) -> Result<Vec<u8>, String> {
    std::fs::read(root.join(path)).map_err(|error| format!("{path}: {error}"))
}
fn face_file(store: &Store, class: &'static str, letters: Letters) -> Result<Face, String> {
    let sheet = mask_sheet(&letters)?;
    // Each face keeps its own cache name, so saving one never retires another.
    let stamp = cache::stamp(&[cache::code_identity().as_bytes(), &sheet]);
    store.get_or_build(&format!("letters{class}"), &stamp, "png", || Ok(sheet))?;
    Ok(Face {
        class,
        stamp,
        letters,
    })
}
/// A face's frames as an alpha mask, ink opaque, `SHEET_SCALE` device pixels
/// to the game pixel, sixteen frames to a row.
fn mask_sheet(letters: &Letters) -> Result<Vec<u8>, String> {
    let (cell_w, cell_h) = letters.cell;
    let frames = letters.rows.len() as u32;
    let (width, height) = (
        SHEET_COLUMNS * cell_w * SHEET_SCALE,
        frames.div_ceil(SHEET_COLUMNS).max(1) * cell_h * SHEET_SCALE,
    );
    let mut alpha = vec![0u8; (width * height) as usize];
    for frame in 0..frames {
        let (left, top) = (
            frame % SHEET_COLUMNS * cell_w,
            frame / SHEET_COLUMNS * cell_h,
        );
        for y in 0..cell_h * SHEET_SCALE {
            for x in 0..cell_w * SHEET_SCALE {
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
/// The lettering rules: per face, a sprite per frame with its advance and
/// place on the sheet, lengths in game pixels.
pub(super) fn lettering_css() -> String {
    let mut css = String::new();
    for face in current().faces {
        let Face {
            class,
            stamp,
            letters,
        } = &face;
        let (cell_w, cell_h) = letters.cell;
        let size = format!(
            "{}px {}px",
            SHEET_COLUMNS * cell_w,
            (letters.rows.len() as u32).div_ceil(SHEET_COLUMNS).max(1) * cell_h
        );
        css.push_str(&format!(
            ".{class} i{{display:block;height:{cell_h}px;-webkit-mask:url(/cache/letters{class}-{stamp}.png) 0 0/{size} no-repeat;mask:url(/cache/letters{class}-{stamp}.png) 0 0/{size} no-repeat}}"
        ));
        for (frame, advance) in letters.advance.iter().enumerate() {
            if *advance == 0 {
                continue;
            }
            let frame = frame as u32;
            let (x, y) = (
                frame % SHEET_COLUMNS * cell_w,
                frame / SHEET_COLUMNS * cell_h,
            );
            css.push_str(&format!(
                ".{class} .g{frame:02x}{{width:{advance}px;-webkit-mask-position:-{x}px -{y}px;mask-position:-{x}px -{y}px}}"
            ));
        }
    }
    super::chrome::pixels(&css)
}
/// Every text node of a page's body drawn from the sheets: words as unbroken
/// sprite runs, the text itself kept for readers, search and copying.
/// Script, style, title and option text is left alone, and so is any
/// character a face lacks, which falls back to the system face.
pub(super) fn letter(html: &str) -> String {
    let assets = current();
    let faces = assets
        .faces
        .iter()
        .map(|face| (face.class, &face.letters))
        .collect::<Vec<_>>();
    if faces.is_empty() {
        return html.to_string();
    }
    letter_with(&faces, html)
}
pub(super) fn letter_with(faces: &[(&str, &Letters)], html: &str) -> String {
    let face = |class: &str| {
        faces
            .iter()
            .find(|(name, _)| *name == class)
            .or(faces.first())
            .copied()
    };
    let mut out = String::with_capacity(html.len() * 3);
    let mut rest = html;
    let mut raw: Option<String> = None;
    let mut body = false;
    // The element that chose a quoted-text face, and the face.
    let mut quoted: Option<(String, &str)> = None;
    while !rest.is_empty() {
        // Raw text runs verbatim to its own closing tag, whatever it holds.
        if let Some(open) = &raw {
            let close = rest.find(&format!("</{open}")).unwrap_or(rest.len());
            out.push_str(&rest[..close]);
            rest = &rest[close..];
            if rest.is_empty() {
                break;
            }
        }
        let next = rest.find('<').unwrap_or(rest.len());
        let (text, tail) = rest.split_at(next);
        let chosen = face(quoted.as_ref().map_or("u", |(_, class)| class));
        match chosen {
            Some((class, letters)) if body && raw.is_none() && !text.trim().is_empty() => {
                out.push_str(&run(class, letters, &unescape(text)));
            }
            _ => out.push_str(text),
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
        match &raw {
            Some(open) if closing && name == *open => raw = None,
            Some(_) => {}
            None if !closing
                && matches!(
                    name.as_str(),
                    "script" | "style" | "title" | "option" | "textarea"
                ) =>
            {
                raw = Some(name.clone())
            }
            None if name == "body" => body = !closing,
            None => {}
        }
        if closing && quoted.as_ref().is_some_and(|(open, _)| *open == name) {
            quoted = None;
        } else if !closing {
            if let Some(at) = tag.find("data-face=\"") {
                let value = &tag[at + 11..];
                let value = &value[..value.find('"').unwrap_or(0)];
                if let Some(class) = FACES.iter().find(|class| **class == value) {
                    quoted = Some((name, class));
                }
            }
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
/// One text node as `<span class="t CLASS">`: the hidden text, then each word
/// as an unbreakable run of glyphs with its following space, lines split at
/// newlines, and characters the face lacks in the system face.
fn run(class: &str, letters: &Letters, text: &str) -> String {
    let escaped = crate::coverage::boxtree::esc(text);
    let mut out = format!(
        "<span class=\"t {class}\"><span class=\"sr\">{escaped}</span><span aria-hidden=\"true\">"
    );
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
                word.push_str(&format!("<i class=\"g{frame:02x}\"></i>"));
            }
            None => fallback.push(character),
        }
        if character == ' ' || character == '\u{3000}' {
            flush_fallback(&mut word, &mut fallback);
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
/// A label's width in CSS pixels in the menu font, for layout that must know
/// it ahead.
pub(super) fn width(text: &str) -> u32 {
    match current().face("u") {
        Some(face) => face.letters.width(text) * PIXEL,
        None => text.chars().count() as u32 * 6 * PIXEL,
    }
}
/// The menu face's sheet and advances, for script-drawn text.
pub(super) fn menu_meta() -> Option<(String, Vec<u32>, (u32, u32))> {
    current().face("u").map(|face| {
        (
            format!("lettersu-{}", face.stamp),
            face.letters.advance.clone(),
            face.letters.cell,
        )
    })
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

/// The logo marks as one strip of `LINE`-square one-colour masks in
/// `MARKS` order, at `SHEET_SCALE`, with its stamp. They need no inputs, so
/// they are drawn from the code alone and served without the cache.
pub(super) fn marks() -> (String, Vec<u8>) {
    static SHEET: std::sync::OnceLock<(String, Vec<u8>)> = std::sync::OnceLock::new();
    SHEET
        .get_or_init(|| {
            let (scale, line) = (SHEET_SCALE as usize, LINE as usize);
            let width = line * MARKS.len() * scale;
            let mut alpha = vec![0u8; width * line * scale];
            for (slot, (_, rows)) in MARKS.iter().enumerate() {
                for (y, row) in rows.iter().enumerate() {
                    for (x, pixel) in row.chars().enumerate() {
                        if pixel != '#' {
                            continue;
                        }
                        for dy in 0..scale {
                            for dx in 0..scale {
                                alpha[(y * scale + dy) * width + (slot * line + x) * scale + dx] =
                                    255;
                            }
                        }
                    }
                }
            }
            let sheet = grey_alpha_png(width as u32, (line * scale) as u32, &alpha)
                .expect("an in-memory PNG");
            (cache::stamp(&[&sheet]), sheet)
        })
        .clone()
}
/// A mark's slot in the strip, for its mask position.
pub(super) fn mark_slot(name: &str) -> u32 {
    MARKS.iter().position(|(key, _)| *key == name).unwrap_or(0) as u32
}
fn grey_alpha_png(width: u32, height: u32, alpha: &[u8]) -> Result<Vec<u8>, String> {
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

/// One 16x16 frame per tab, in `TAB_ICONS` order, as a one-colour mask at
/// `SHEET_SCALE` device pixels per game pixel: each icon keeps its lighter
/// half of colours as ink and drops its dark outline, which the page redraws
/// as the labels' one-pixel shadow. The page tints the ink.
fn icon_strip(banks: &BTreeMap<u8, IndexedImage>, colors: &[u16; 16]) -> Result<Vec<u8>, String> {
    let luminance = |index: usize| {
        let color = colors[index & 15];
        let channel = |shift: u16| u32::from((color >> shift) & 31);
        2 * channel(0) + 5 * channel(5) + channel(10)
    };
    let scale = SHEET_SCALE as usize;
    let width = 16 * TAB_ICONS.len() * scale;
    let mut alpha = vec![0u8; width * 16 * scale];
    for (slot, (_, bank, frame)) in TAB_ICONS.iter().enumerate() {
        let image = banks.get(bank).ok_or("icon bank absent")?;
        let columns = image.width / 16;
        if *frame >= columns * (image.height / 16) {
            return Err(format!("icon {frame} lies outside bank {bank}"));
        }
        let (left, top) = (frame % columns * 16, frame / columns * 16);
        let index =
            |x: u32, y: u32| image.pixels[((top + y) * image.width + left + x) as usize] as usize;
        let opaque = (0..16)
            .flat_map(|y| (0..16).map(move |x| (x, y)))
            .filter(|(x, y)| index(*x, *y) != 0)
            .map(|(x, y)| luminance(index(x, y)))
            .collect::<Vec<_>>();
        let mean = opaque.iter().sum::<u32>() / opaque.len().max(1) as u32;
        for y in 0..16u32 {
            for x in 0..16u32 {
                let pixel = index(x, y);
                if pixel == 0 || luminance(pixel) < mean {
                    continue;
                }
                for dy in 0..scale {
                    for dx in 0..scale {
                        let at = (y as usize * scale + dy) * width
                            + (slot * 16 + x as usize) * scale
                            + dx;
                        alpha[at] = 255;
                    }
                }
            }
        }
    }
    grey_alpha_png(width as u32, (16 * scale) as u32, &alpha)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn body_text_becomes_glyph_runs_in_its_face_and_raw_text_does_not() {
        let letters = crate::coverage::letters::fixture();
        let faces = [("u", &letters), ("d", &letters)];
        let html = "<html><head><title>A&amp;B</title><style>p{x:1}</style></head><body><a class=\"tab\">Up &amp; 神</a><select><option>Keep</option></select><script>let a=1<2</script><td data-face=\"d\">Hi</td><td>Hi</td>\n</body></html>";
        let lettered = letter_with(&faces, html);
        assert!(
            lettered.contains("<title>A&amp;B</title>")
                && lettered.contains("<style>p{x:1}</style>")
        );
        assert!(
            lettered.contains("<option>Keep</option>")
                && lettered.contains("<script>let a=1<2</script>")
        );
        assert!(lettered.contains("<span class=\"t u\"><span class=\"sr\">Up &amp; 神</span>"));
        assert!(lettered
            .contains("<b><i class=\"g35\"></i><i class=\"g50\"></i><i class=\"g00\"></i></b>"));
        assert!(lettered.contains(
            "<b><i class=\"g06\"></i><i class=\"g00\"></i></b><b><span class=\"f\">神</span></b>"
        ));
        assert!(lettered.contains("<td data-face=\"d\"><span class=\"t d\">"));
        assert!(lettered.contains("</td><td><span class=\"t u\">"));
        assert_eq!(unescape("&lt;a&gt; &#233;&#x41;&unknown"), "<a> éA&unknown");
    }
    #[test]
    fn served_names_accept_only_current_stamps() {
        let root = tempfile::tempdir().unwrap();
        assert!(file(root.path(), "lettersu-0123456789abcdef.png").is_none());
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
