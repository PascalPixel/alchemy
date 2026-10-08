use ags::assets::image::{indexed_bitmap_png, PNG_SIGNATURE};
use ags::assets::midi::{midi_events, EventBody, MidiEvent};
use ags::assets::wav::wav_pcm8;
use std::collections::{BTreeMap, BTreeSet};
use std::io::{BufRead, BufReader, Read, Write};
use std::path::Path;
use std::process::{Command, ExitCode, Stdio};
const BLOCKED_EXTENSIONS: &[&str] = &[
    "a", "bin", "bps", "bsdiff", "d", "diff", "dis", "dll", "dmp", "dump", "dylib", "elf", "exe",
    "gba", "gz", "ips", "lst", "log", "map", "o", "patch", "raw", "rom", "sav", "so", "sym", "tar",
    "tgz", "unidiff", "ups", "xdelta", "xdelta3", "zip", "7z",
];
pub(crate) const PRESENTATION_EXTENSIONS: &[&str] = &[
    "otf", "ttf", "ttc", "woff", "woff2", "eot", "fnt", "bdf", "pcf", "gif", "jpg", "jpeg", "webp",
    "bmp", "ico", "avif", "apng", "tif", "tiff", "mp3", "ogg", "flac", "m4a", "aac", "mp4", "webm",
    "mov", "svg",
];
const NATIVE_INPUT_EXTENSIONS: &[&str] = &[
    "c", "h", "inc", "s", "ld", "mk", "gitkeep", "png", "wav", "mid", "pcm4", "po", "tsv",
];
const METADATA_DIRECTORIES: &[&str] = &["metrics", "preview", "recon", "semantic"];
const DATA_TABLE_EXTENSIONS: &[&str] = &["tsv"];
const APPROVED_GITLINKS: &[&str] = &["agbcc", "agscc"];
const FONT_TABLES: &[&[u8]] = &[
    b"BASE", b"CFF ", b"COLR", b"CPAL", b"DSIG", b"EBDT", b"EBLC", b"FFTM", b"GDEF", b"GPOS",
    b"GSUB", b"LTSH", b"OS/2", b"STAT", b"SVG ", b"VDMX", b"cmap", b"cvt ", b"fpgm", b"gasp",
    b"glyf", b"hdmx", b"head", b"hhea", b"kern", b"loca", b"maxp", b"name", b"post", b"prep",
];
const PNG_ANCILLARY: &[(&[u8], usize)] = &[
    (b"tRNS", 256),
    (b"sRGB", 1),
    (b"gAMA", 4),
    (b"cHRM", 32),
    (b"pHYs", 9),
    (b"sBIT", 4),
    (b"bKGD", 6),
    (b"tIME", 7),
];
const PNG_SCANLINES_MAX: usize = 1 << 26;
const ENCODED_RUN_MIN: usize = 16;
const ENCODED_CHARACTERS_MAX: usize = 128;
const DIGEST_RUNS_MAX: usize = 2_048;
const NUMERIC_RUN_MIN: usize = 16;
const NUMERIC_ELEMENTS_MAX: usize = 2_048;
const JSON_REASON: &str = "JSON is banned: write a TSV table, plain text or a source form";
const INTEGER_SUFFIXES: &[&str] = &[
    "usize", "isize", "u128", "i128", "u64", "i64", "u32", "i32", "u16", "i16", "u8", "i8", "ull",
    "llu", "ul", "lu", "ll", "u", "l",
];
const LOGO: std::ops::Range<usize> = 0x04..0xa0;
const LOGO_REASON: &str = "Nintendo logo from a GBA cartridge header: L1 keeps it only in tools/alchemy/src/gbafix.rs, the build's header fix-up";
/// The one file L1 lets hold the logo, as pret keeps it only in tools/gbafix.
const LOGO_SOURCE: &str = "tools/alchemy/src/gbafix.rs";
/// Data directives a numeric run reads through, as an assembler table
/// spells its bytes line by line.
const DATA_DIRECTIVES: &[&str] = &[
    ".byte", ".2byte", ".hword", ".short", ".4byte", ".word", ".long", ".int", ".8byte", ".quad",
];
const UNREGISTERED: &str = "unregistered binary: pret commits only editable build inputs";
const GREY_SHEET: &str =
    "PNG is a grey sheet: an indexed asset carries its real palette, not a grey ramp";
const BLOCKED_DIRECTORIES: &[&str] = &[
    ".cache",
    "alchemy-gcc",
    "agbcc",
    "agscc",
    "analysis",
    "build",
    "builds",
    "cmatch",
    "comparisons",
    "compiler-output",
    "compilers",
    "diffs",
    "disassembly",
    "dist",
    "dump",
    "dumps",
    "m2c",
    "objdump",
    "out",
    "private",
    "report",
    "reports",
    "rom",
    "roms",
    "scratch",
    "toolchain",
    "toolchains",
    "work",
];
const REPORT_EXTENSIONS: &[&str] = &["csv", "log", "tsv", "txt"];
const REPORT_WORDS: &[&str] = &["analysis", "comparison", "diff", "dump", "report"];
const GENERATED_LEDGER_NAMES: &[&str] = &[
    "assets.json",
    "private-inputs.json",
    "source-paths.json",
    "source-bindings.json",
    "translation-units.json",
    "machine.json",
    "manifest.json",
    "index.json",
];
const GENERATED_REASON: &str =
    "calculated bookkeeping belongs in ignored out/: keep source decisions in code and build rules";
fn json_text(text: &str) -> bool {
    let text = text.trim();
    (text.starts_with('{') && text.ends_with('}')) || (text.starts_with('[') && text.ends_with(']'))
}
fn json_path(path: &str) -> bool {
    listed(extension(path), &["json", "jsonl", "json5", "geojson"])
}
fn listed(value: &str, choices: &[&str]) -> bool {
    choices
        .iter()
        .any(|choice| value.eq_ignore_ascii_case(choice))
}
fn extension(path: &str) -> &str {
    path.rsplit(['/', '\\'])
        .next()
        .and_then(|leaf| leaf.rsplit_once('.').map(|(_, suffix)| suffix))
        .unwrap_or("")
}
fn publication_path_reason(path: &str) -> Option<&'static str> {
    let normalized = path.replace('\\', "/");
    let components: Vec<_> = normalized
        .split('/')
        .filter(|component| !component.is_empty())
        .collect();
    let leaf = components.last().copied().unwrap_or("");
    let directories = &components[..components.len().saturating_sub(1)];
    if normalized.starts_with('/') || components.contains(&"..") {
        return Some("invalid repository path");
    }
    if json_path(&normalized) {
        return Some(JSON_REASON);
    }
    if directories.iter().any(|directory| {
        listed(directory, BLOCKED_DIRECTORIES)
            || directory.to_ascii_lowercase().starts_with(".cmatch")
    }) {
        return Some("private or generated directory");
    }
    let lower_leaf = leaf.to_ascii_lowercase();
    if extension(leaf).eq_ignore_ascii_case("tokens") {
        return Some("stored compression token table: recover the encoder");
    }
    if components
        .first()
        .is_some_and(|top| top.eq_ignore_ascii_case("recon"))
        && listed(&lower_leaf, GENERATED_LEDGER_NAMES)
    {
        return Some(GENERATED_REASON);
    }
    if listed(
        &lower_leaf,
        &["char_common.png", "tile_bank.png", "data.bin"],
    ) {
        return Some("unidentified asset dump: reconstruct an identified editable input");
    }
    if lower_leaf == "baserom"
        || lower_leaf.starts_with("baserom.")
        || lower_leaf.contains(".gba.")
        || lower_leaf.contains(".rom.")
    {
        return Some("private ROM name");
    }
    let suffix = extension(&normalized);
    if listed(suffix, BLOCKED_EXTENSIONS) {
        return Some("private or generated file type");
    }
    if listed(suffix, PRESENTATION_EXTENSIONS) {
        return Some("presentation material: pret commits only editable build inputs");
    }
    if directories
        .iter()
        .any(|directory| directory.eq_ignore_ascii_case("preview"))
    {
        return Some("PREVIEW material belongs under ignored out/");
    }
    let report_name = leaf
        .split(['.', '_', '-'])
        .any(|word| listed(word, REPORT_WORDS));
    if listed(suffix, REPORT_EXTENSIONS) && report_name {
        return Some("private analysis report");
    }
    None
}
fn gba_header(data: &[u8]) -> bool {
    if data.len() < 0xc0 {
        return false;
    }
    if data[0xb2..=0xb4] != [0x96, 0, 0] || data[0xb5..=0xbb].iter().any(|byte| *byte != 0) {
        return false;
    }
    let sum = data[0xa0..=0xbc]
        .iter()
        .fold(0u8, |sum, byte| sum.wrapping_add(*byte));
    data[0xbd] == 0u8.wrapping_sub(sum).wrapping_sub(0x19)
}
fn gba_image(data: &[u8]) -> bool {
    data.len().is_multiple_of(0x8000) && data.len() <= 0x0400_0000 && gba_header(data)
}
/// The logo as the user's own ROM carries it. L1 keeps it in exactly one
/// committed file, LOGO_SOURCE, the build's header fix-up; the header
/// sources reserve it as `.space 156`.
fn nintendo_logo(root: &Path) -> Option<Vec<u8>> {
    let mut header = [0; 0xc0];
    std::fs::File::open(root.join("roms/tbs-en.gba"))
        .ok()?
        .read_exact(&mut header)
        .ok()?;
    gba_header(&header).then(|| header[LOGO].to_vec())
}
fn contains(data: &[u8], needle: &[u8]) -> bool {
    !needle.is_empty()
        && data
            .windows(needle.len())
            .any(|window| window[0] == needle[0] && window == needle)
}
fn publication_content_reason(data: &[u8]) -> Option<&'static str> {
    if gba_image(data) {
        return Some("GBA ROM image");
    }
    if data.get(3) == Some(&0xea) && gba_header(data) {
        return Some("GBA ROM header fragment");
    }
    if data.starts_with(&[0x7f, b'E', b'L', b'F']) {
        return Some("ELF build product");
    }
    if data.starts_with(b"!<arch>\n") {
        return Some("archive or object library");
    }
    if data.starts_with(b"MZ") {
        return Some("native executable");
    }
    let magic = data
        .get(..4)
        .map(|bytes| u32::from_be_bytes(bytes.try_into().unwrap()));
    if matches!(
        magic,
        Some(
            0xfeed_face
                | 0xcefa_edfe
                | 0xfeed_facf
                | 0xcffa_edfe
                | 0xcafe_babe
                | 0xbeba_feca
                | 0xcafe_babf
                | 0xbfba_feca
                | 0x0061_736d
        )
    ) {
        return Some("native executable");
    }
    None
}
fn font_range(text: &str) -> bool {
    let text = text
        .chars()
        .filter(|ch| !ch.is_whitespace())
        .collect::<String>();
    let tokens = regex::Regex::new(r"0[xX][0-9a-fA-F]+|[0-9]+|[-+*()]")
        .expect("font layout arithmetic")
        .find_iter(&text)
        .map(|token| token.as_str())
        .collect::<Vec<_>>();
    if tokens.concat() != text || tokens.is_empty() {
        return false;
    }
    fn atom(tokens: &[&str], at: &mut usize) -> Option<usize> {
        let token = *tokens.get(*at)?;
        *at += 1;
        if token == "(" {
            let value = sum(tokens, at)?;
            if tokens.get(*at) != Some(&")") {
                return None;
            }
            *at += 1;
            Some(value)
        } else if let Some(digits) = token
            .strip_prefix("0x")
            .or_else(|| token.strip_prefix("0X"))
        {
            usize::from_str_radix(digits, 16).ok()
        } else {
            token.parse().ok()
        }
    }
    fn product(tokens: &[&str], at: &mut usize) -> Option<usize> {
        let mut value = atom(tokens, at)?;
        while tokens.get(*at) == Some(&"*") {
            *at += 1;
            value = value.checked_mul(atom(tokens, at)?)?;
        }
        Some(value)
    }
    fn sum(tokens: &[&str], at: &mut usize) -> Option<usize> {
        let mut value = product(tokens, at)?;
        while let Some(operator @ ("+" | "-")) = tokens.get(*at).copied() {
            *at += 1;
            let next = product(tokens, at)?;
            value = if operator == "+" {
                value.checked_add(next)?
            } else {
                value.checked_sub(next)?
            };
        }
        Some(value)
    }
    let mut at = 0;
    sum(&tokens, &mut at).is_some() && at == tokens.len()
}
fn incbin(path: &str, data: &[u8]) -> bool {
    let base_rom = regex::Regex::new(
        r#"^\s*\.incbin\s+"baserom\.gba"\s*,\s*0x[0-9a-f]+\s*,\s*0x[0-9a-f]+\s*$"#,
    )
    .expect("base ROM range pattern");
    let overlay = regex::Regex::new(r#"^\s*\.incbin\s+"overlays/resource_[0-9a-f]+\.lz"\s*$"#)
        .expect("built overlay pattern");
    let built_sound = regex::Regex::new(
        r#"^\s*\.incbin\s+"(?:common/)?sound(?:/[a-z0-9_]+)+(?:\.[a-z0-9]+)?\.bin"\s*$"#,
    )
    .expect("built sound pattern");
    let built_graphics = regex::Regex::new(
        r#"^\s*\.incbin\s+"((?:common/)?([a-z][a-z0-9_]*)(?:/[a-z0-9_]+)+\.[a-z0-9.]+)"\s*$"#,
    )
    .expect("built graphics pattern");
    let built_font = regex::Regex::new(
        r#"^\s*\.incbin\s+"((?:common/)?graphics(?:/[a-z0-9_]+)+\.font)"\s*,\s*([^,]+?)(?:\s*,\s*([^,]+?))?\s*$"#,
    ).expect("built font range pattern");
    let is_built_graphics = |line: &str| {
        built_graphics
            .captures(line)
            .is_some_and(|capture| !matches!(&capture[2], "sound" | "games" | "text" | "overlays" | "obj" | "symbols") && ags::resource::is_recipe(&capture[1]))
            || built_font.captures(line).is_some_and(|capture| {
                ags::resource::is_recipe(&capture[1])
                    && font_range(&capture[2])
                    && capture.get(3).is_none_or(|size| font_range(size.as_str()))
            })
    };
    let scaffolding = path.starts_with("recon/");
    let parts = path.split('/').collect::<Vec<_>>();
    let sound_source = matches!(
        parts.as_slice(),
        ["games", game, "sound", .., _] if *game != "common"
    );
    let asset_source = matches!(parts.as_slice(), ["games", _, "src", .., _]);
    // A game's overlay streams, which the build links from the overlays it
    // builds, live in one source beside the overlays' link scripts.
    let overlay_source = matches!(
        parts.as_slice(),
        ["games", game, "src", "system", "overlay", "overlays.s"] if *game != "common"
    );
    let text = String::from_utf8_lossy(data);
    text.split(['\n', '\r'])
        .filter(|line| !(scaffolding && base_rom.is_match(line)))
        .filter(|line| !((scaffolding || overlay_source) && overlay.is_match(line)))
        .filter(|line| !(sound_source && built_sound.is_match(line)))
        .filter(|line| !(asset_source && is_built_graphics(line)))
        .any(|line| {
            let trimmed =
                line.trim_start_matches(|ch: char| ch.is_whitespace() || ch == '\u{feff}');
            let bytes = trimmed.as_bytes();
            bytes
                .get(..7)
                .is_some_and(|word| word.eq_ignore_ascii_case(b".incbin"))
                && bytes
                    .get(7)
                    .is_none_or(|next| !(next.is_ascii_alphanumeric() || *next == b'_'))
        })
}
fn binary(data: &[u8]) -> bool {
    data.contains(&0) || std::str::from_utf8(data).is_err()
}
struct Png {
    colour: u8,
    animated: bool,
}
fn png(data: &[u8]) -> Option<Png> {
    if !data.starts_with(b"\x89PNG\r\n\x1a\n") || data.get(12..16) != Some(b"IHDR") {
        return None;
    }
    let mut animated = false;
    let mut offset = 8;
    while let Some(header) = data.get(offset..offset + 8) {
        let length = u32::from_be_bytes(header[..4].try_into().unwrap()) as usize;
        animated |= &header[4..] == b"acTL";
        if &header[4..] == b"IEND" {
            break;
        }
        offset = offset.saturating_add(length).saturating_add(12);
    }
    Some(Png {
        colour: *data.get(25)?,
        animated,
    })
}
fn presentation_magic_reason(data: &[u8]) -> Option<&'static str> {
    let head = |length: usize| data.get(..length).unwrap_or(&[]);
    let sfnt = matches!(head(4), b"OTTO" | b"true" | b"typ1" | b"\0\x01\0\0")
        && data
            .get(12..16)
            .is_some_and(|tag| FONT_TABLES.contains(&tag));
    let packed = matches!(head(4), b"ttcf" | b"wOFF" | b"wOF2") && binary(data);
    if sfnt || packed {
        return Some("font: pret commits only editable build inputs");
    }
    if matches!(head(6), b"GIF87a" | b"GIF89a") {
        return Some("GIF image: pret commits only editable build inputs");
    }
    if head(3) == [0xff, 0xd8, 0xff] {
        return Some("JPEG image: pret commits only editable build inputs");
    }
    if head(4) == b"RIFF" && data.get(8..12) == Some(b"WEBP") {
        return Some("WebP image: pret commits only editable build inputs");
    }
    let bitmap_size = data
        .get(2..6)
        .map(|size| u32::from_le_bytes(size.try_into().unwrap()) as usize);
    if head(2) == b"BM" && bitmap_size == Some(data.len()) {
        return Some("BMP image: pret commits only editable build inputs");
    }
    match png(data) {
        Some(image) if image.animated => {
            Some("animated PNG: pret commits only editable build inputs")
        }
        Some(image) if matches!(image.colour, 2 | 4 | 6) => {
            Some("truecolour or alpha PNG: pret commits indexed build inputs")
        }
        _ => None,
    }
}
fn png_chunks(data: &[u8]) -> Option<Vec<(&[u8], &[u8])>> {
    let mut rest = data.strip_prefix(PNG_SIGNATURE.as_slice())?;
    let mut chunks = Vec::new();
    while !rest.is_empty() {
        let length = u32::from_be_bytes(rest.get(..4)?.try_into().ok()?) as usize;
        let body = rest.get(8..length.checked_add(8)?)?;
        chunks.push((&rest[4..8], body));
        rest = rest.get(length + 12..)?;
    }
    Some(chunks)
}
fn inflate(stream: &[u8], size: usize) -> Option<Vec<u8>> {
    let mut decoder = fdeflate::Decompressor::new();
    let mut output = vec![0; size + 1];
    let (mut consumed, mut produced) = (0, 0);
    while !decoder.is_done() {
        let (input, written) = decoder
            .read(&stream[consumed..], &mut output, produced, true)
            .ok()?;
        if input == 0 && written == 0 {
            return None;
        }
        consumed += input;
        produced += written;
    }
    output.truncate(size);
    (produced == size).then_some(output)
}
fn inflate_exact(stream: &[u8], size: usize) -> Option<Vec<u8>> {
    let output = inflate(stream, size)?;
    let (_, shorter) = stream.split_last()?;
    inflate(shorter, size).is_none().then_some(output)
}
fn indexed_png_bytes(data: &[u8]) -> Option<Vec<u8>> {
    exact_indexed_stream(data)?;
    let image = indexed_bitmap_png(data).ok()?;
    let depth = data[24];
    Some(
        image
            .pixels
            .chunks(8 / usize::from(depth))
            .map(|group| group.iter().fold(0u32, |byte, pixel| byte << depth | pixel) as u8)
            .collect(),
    )
}
fn grey_sheet(data: &[u8]) -> bool {
    png_chunks(data)
        .and_then(|chunks| chunks.into_iter().find(|(kind, _)| *kind == b"PLTE"))
        .is_some_and(|(_, palette)| {
            palette.len() > 6
                && palette.chunks(3).all(|colour| {
                    colour.len() == 3 && colour[0] == colour[1] && colour[1] == colour[2]
                })
        })
}
fn exact_indexed_stream(data: &[u8]) -> Option<()> {
    let chunks = png_chunks(data)?;
    let (_, header) = chunks
        .first()
        .filter(|(kind, body)| *kind == b"IHDR" && body.len() == 13)?;
    chunks
        .last()
        .filter(|(kind, body)| *kind == b"IEND" && body.is_empty())?;
    let number = |at: usize| u32::from_be_bytes(header[at..at + 4].try_into().unwrap()) as usize;
    let (width, height, depth) = (number(0), number(4), header[8]);
    if width == 0 || height == 0 || !matches!(depth, 1 | 2 | 4 | 8) || header[9..] != [3, 0, 0, 0] {
        return None;
    }
    let kinds: Vec<&[u8]> = chunks.iter().map(|(kind, _)| *kind).collect();
    let first = kinds.iter().position(|kind| *kind == b"IDAT")?;
    let last = kinds.iter().rposition(|kind| *kind == b"IDAT")?;
    let palette = kinds.iter().position(|kind| *kind == b"PLTE")?;
    let ordered = chunks
        .iter()
        .enumerate()
        .all(|(index, (kind, body))| match *kind {
            b"IHDR" => index == 0,
            b"PLTE" => index == palette && palette < first,
            b"IDAT" => (first..=last).contains(&index),
            b"IEND" => index + 1 == chunks.len(),
            _ => PNG_ANCILLARY
                .iter()
                .any(|(name, limit)| kind == name && body.len() <= *limit),
        })
        && kinds[first..=last].iter().all(|kind| *kind == b"IDAT");
    if !ordered {
        return None;
    }
    let row = (width * usize::from(depth)).div_ceil(8) + 1;
    let size = row
        .checked_mul(height)
        .filter(|size| *size <= PNG_SCANLINES_MAX)?;
    let stream: Vec<u8> = chunks[first..=last]
        .iter()
        .flat_map(|(_, body)| body.iter().copied())
        .collect();
    let scanlines = inflate_exact(&stream, size)?;
    (!scanlines.chunks(row).any(|line| line[0] > 4)).then_some(())
}
fn midi_reason(data: &[u8]) -> Option<&'static str> {
    const MALFORMED: &str =
        "MIDI is not an exact sequence build input: MThd, MTrk, sized or text metas only";
    let mut offset = 0;
    while offset < data.len() {
        let expected: &[u8] = if offset == 0 { b"MThd" } else { b"MTrk" };
        let size = data
            .get(offset + 4..offset + 8)
            .map(|size| u32::from_be_bytes(size.try_into().unwrap()) as usize);
        match size {
            Some(size) if data[offset..offset + 4] == *expected => offset += 8 + size,
            _ => return Some(MALFORMED),
        }
    }
    let Ok(report) = midi_events(data) else {
        return Some(MALFORMED);
    };
    if offset != data.len() {
        return Some(MALFORMED);
    }
    let mut text = String::new();
    let mut last: Vec<Option<&MidiEvent>> = vec![None; usize::from(report.tracks)];
    for event in &report.events {
        match &event.body {
            EventBody::Sysex { .. } => return Some(MALFORMED),
            EventBody::Meta { meta, data } => match (meta, data.len()) {
                (0x00, 0 | 2)
                | (0x20 | 0x21, 1)
                | (0x2f, 0)
                | (0x51, 3)
                | (0x54, 5)
                | (0x58, 4)
                | (0x59, 2) => {}
                (0x01..=0x0f | 0x7f, _) => match std::str::from_utf8(data) {
                    Ok(value) if value.trim_start().starts_with(['{', '[']) => {
                        return Some("MIDI text is JSON; the format is banned")
                    }
                    Ok(value) => {
                        text.push_str(value);
                        text.push('\n');
                    }
                    Err(_) => return Some(MALFORMED),
                },
                _ => return Some(MALFORMED),
            },
            EventBody::Channel { .. } => {}
        }
        let final_event = &mut last[event.track];
        if final_event.is_none_or(|known| known.order < event.order) {
            *final_event = Some(event);
        }
    }
    let closed = last.iter().all(|event| {
        event.is_some_and(
            |event| matches!(&event.body, EventBody::Meta { meta: 0x2f, data } if data.is_empty()),
        )
    });
    if !closed {
        return Some(MALFORMED);
    }
    placement_reason(&text)
        .or_else(|| data_uri_reason(&text))
        .or_else(|| encoded_reason(&text, false))
}
fn placement_reason(text: &str) -> Option<&'static str> {
    let address = text.match_indices("0x").any(|(index, _)| {
        text[index + 2..]
            .bytes()
            .take_while(u8::is_ascii_hexdigit)
            .count()
            >= 4
    });
    let placement = ["base=", "externals="]
        .iter()
        .any(|field| text.contains(field));
    (address || placement)
        .then_some("MIDI records a ROM address or placement; the build supplies it")
}
fn palette_built(path: &str) -> bool {
    own_palette_built(path) || part_palette_built(path)
}

fn part_palette_built(path: &str) -> bool {
    let file = Path::new(path);
    let (Some(directory), Some(stem)) = (file.parent(), file.file_stem().and_then(|s| s.to_str()))
    else {
        return false;
    };
    let Ok(entries) = std::fs::read_dir(directory) else {
        return false;
    };
    entries.flatten().any(|entry| {
        let list = entry.path();
        if !list
            .extension()
            .is_some_and(|ext| ext.eq_ignore_ascii_case("tsv"))
        {
            return false;
        }
        let Ok(text) = std::fs::read_to_string(&list) else {
            return false;
        };
        let lines: Vec<Vec<&str>> = text
            .lines()
            .map(|line| line.split('\t').map(str::trim).collect::<Vec<_>>())
            .filter(|fields| !fields[0].is_empty())
            .collect();
        let parts: Vec<&str> = lines.iter().map(|fields| fields[0]).collect();
        match parts.split_first() {
            // The list's first part is the palette itself: the list writes it.
            Some((first, rest)) if lines[0].get(1) == Some(&"gbapal") => {
                (first.eq_ignore_ascii_case(stem) || rest.iter().any(|part| part.eq_ignore_ascii_case(stem)))
                    && list
                        .file_stem()
                        .and_then(|list| list.to_str())
                        .is_some_and(|list| incbinned(path, &format!("{list}.parts")))
            }
            Some((first, rest)) if rest.iter().any(|part| part.eq_ignore_ascii_case(stem)) => {
                let first = first.to_ascii_lowercase();
                own_palette_built(&directory.join(format!("{first}.png")).to_string_lossy())
            }
            _ => false,
        }
    })
}

fn own_palette_built(path: &str) -> bool {
    Path::new(path)
        .file_stem()
        .and_then(|stem| stem.to_str())
        .is_some_and(|stem| incbinned(path, &format!("{stem}.gbapal")))
}

/// Whether an assembly source beside `path` incbins the build product `name` from that folder.
fn incbinned(path: &str, name: &str) -> bool {
    let Some(directory) = Path::new(path).parent() else {
        return false;
    };
    let Some(source_root) = path.find("/src/").map(|at| &path[..at + 5]) else {
        return false;
    };
    let relative = directory
        .to_string_lossy()
        .strip_prefix(source_root)
        .map(|rest| format!("{rest}/{name}"))
        .unwrap_or_default();
    let Ok(entries) = std::fs::read_dir(directory) else {
        return false;
    };
    entries.flatten().any(|entry| {
        entry.path().extension().is_some_and(|ext| ext == "s" || ext == "S")
            && std::fs::read_to_string(entry.path())
                .is_ok_and(|text| text.contains(&format!("\"{relative}")))
    })
}
fn binary_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    if data.is_empty() {
        return None;
    }
    if matches!(path, "PROGRESS.png" | "PROGRESS_CHART.png") {
        if exact_indexed_stream(data).is_none() {
            return Some("README figure is not an exact indexed PNG");
        }
        if grey_sheet(data) {
            return Some(GREY_SHEET);
        }
        return logo
            .is_some_and(|logo| indexed_png_bytes(data).is_none_or(|pixels| contains(&pixels, logo)))
            .then_some(LOGO_REASON);
    }
    let components: Vec<_> = path.split('/').collect();
    let (area, rest) = match components.as_slice() {
        ["games", _, area, rest @ ..] if !rest.is_empty() => (*area, rest.join("/")),
        _ => return Some(UNREGISTERED),
    };
    match (area, extension(path).to_ascii_lowercase().as_str()) {
        ("sound", "pcm4") => {
            (!rest.starts_with("sample/") || data.len() > 64).then_some(UNREGISTERED)
        }
        ("src" | "text", "png") => match indexed_png_bytes(data) {
            None => {
                Some("PNG is not an exact indexed build input: standard chunks, one exact stream")
            }
            Some(_) if grey_sheet(data) && !palette_built(path) => Some(GREY_SHEET),
            Some(pixels) => logo
                .is_some_and(|logo| contains(&pixels, logo))
                .then_some(LOGO_REASON),
        },
        ("sound", "wav") => wav_pcm8(data)
            .is_err()
            .then_some("WAV is not a canonical mono 8-bit PCM build input"),
        ("sound", "mid") => midi_reason(data),
        _ => Some(UNREGISTERED),
    }
}
fn data_uri_reason(text: &str) -> Option<&'static str> {
    let lower = text.to_ascii_lowercase();
    let embedded = lower.match_indices("data:").any(|(index, _)| {
        let rest = &lower[index + 5..];
        let header = &rest[..rest.find(',').unwrap_or(0).min(160)];
        let media = header.split(';').next().unwrap_or("");
        let typed = [
            "font/",
            "image/",
            "audio/",
            "video/",
            "application/font",
            "application/x-font",
            "application/vnd.ms-fontobject",
            "application/octet-stream",
        ]
        .iter()
        .any(|kind| media.starts_with(kind));
        typed && header.split(';').any(|parameter| parameter == "base64")
    });
    embedded.then_some("embedded data URI: pret commits only editable build inputs")
}
enum Run {
    Plain,
    Digest,
    Encoded,
}
fn classify(run: &[u8]) -> Run {
    if run.len() < ENCODED_RUN_MIN {
        return Run::Plain;
    }
    let digits = run
        .strip_prefix(b"0x")
        .or_else(|| run.strip_prefix(b"0X"))
        .unwrap_or(run);
    if digits.iter().all(u8::is_ascii_hexdigit) {
        return match digits.len() {
            _ if digits.iter().all(u8::is_ascii_digit) => Run::Plain,
            16 | 32 | 40 | 64 => Run::Digest,
            _ => Run::Encoded,
        };
    }
    let base32 = run
        .iter()
        .all(|byte| byte.is_ascii_uppercase() || (b'2'..=b'7').contains(byte))
        && run.iter().any(u8::is_ascii_digit);
    let mixed = run.iter().any(u8::is_ascii_uppercase) && run.iter().any(u8::is_ascii_lowercase);
    if base32 || (mixed && !wordlike(run)) {
        Run::Encoded
    } else {
        Run::Plain
    }
}
fn wordlike(run: &[u8]) -> bool {
    let (mut letters, mut worded, mut index) = (0, 0, 0);
    while index < run.len() {
        if run[index].is_ascii_digit() {
            let prefixed = run[index] == b'0'
                && matches!(run.get(index + 1), Some(b'x' | b'X'))
                && run.get(index + 2).is_some_and(u8::is_ascii_hexdigit);
            index += 1 + usize::from(prefixed);
            while run.get(index).is_some_and(u8::is_ascii_hexdigit) {
                index += 1;
            }
            continue;
        }
        let capital = usize::from(run[index].is_ascii_uppercase());
        let lower = run[index + capital..]
            .iter()
            .take_while(|byte| byte.is_ascii_lowercase())
            .count();
        let length = if lower >= 2 { capital + lower } else { 1 };
        letters += length;
        worded += if lower >= 2 { length } else { 0 };
        index += length;
    }
    letters < 8 || worded * 5 >= letters * 3
}
fn numeric(token: &[u8]) -> bool {
    let token = match token {
        [b'-' | b'+', rest @ ..] => rest,
        _ => token,
    };
    if token.len() == 2 && token.iter().all(u8::is_ascii_hexdigit) {
        return true;
    }
    let body = without_suffix(token);
    let digits = |digits: &[u8], limit: usize, valid: fn(&u8) -> bool| {
        !digits.is_empty() && digits.len() <= limit && digits.iter().all(valid)
    };
    match body {
        [b'0', b'x' | b'X', rest @ ..] => digits(rest, 16, u8::is_ascii_hexdigit),
        [b'0', b'b' | b'B', rest @ ..] => digits(rest, 64, |bit| matches!(bit, b'0' | b'1')),
        [b'x' | b'X', rest @ ..] => digits(rest, 2, u8::is_ascii_hexdigit),
        _ => digits(body, 20, u8::is_ascii_digit),
    }
}
fn without_suffix(token: &[u8]) -> &[u8] {
    INTEGER_SUFFIXES
        .iter()
        .find_map(|suffix| {
            let split = token
                .len()
                .checked_sub(suffix.len())
                .filter(|split| *split > 0)?;
            token[split..]
                .eq_ignore_ascii_case(suffix.as_bytes())
                .then(|| &token[..split])
        })
        .unwrap_or(token)
}
fn numeric_separator(byte: &u8) -> bool {
    byte.is_ascii_whitespace() || b",;:()[]{}\\\"'".contains(byte)
}
/// The bytes one numeric token spells: a decimal value as one byte (-128
/// to 255), a two-digit token as hex when `pairs` reads a dump, and a hex
/// or binary value as little-endian bytes as wide as its digits.
fn token_bytes(token: &[u8], pairs: bool) -> Option<Vec<u8>> {
    let (negative, token) = match token {
        [b'-', rest @ ..] => (true, rest),
        [b'+', rest @ ..] => (false, rest),
        _ => (false, token),
    };
    let body = without_suffix(token);
    let (digits, radix, width) = match body {
        [b'0', b'x' | b'X', rest @ ..] => (rest, 16, rest.len().div_ceil(2)),
        [b'x' | b'X', rest @ ..] if rest.len() <= 2 => (rest, 16, 1),
        [b'0', b'b' | b'B', rest @ ..] => (rest, 2, rest.len().div_ceil(8)),
        _ if pairs && body.len() == 2 => (body, 16, 1),
        _ => (body, 10, 1),
    };
    let width = width.max(1).next_power_of_two();
    if digits.is_empty() || width > 8 || (negative && radix != 10) {
        return None;
    }
    let value = u64::from_str_radix(std::str::from_utf8(digits).ok()?, radix).ok()?;
    match (negative, radix) {
        (true, _) => (1..=128)
            .contains(&value)
            .then(|| vec![(256 - value) as u8]),
        (false, 10) => u8::try_from(value).ok().map(|byte| vec![byte]),
        (false, _) => Some(value.to_le_bytes()[..width].to_vec()),
    }
}
/// Whether a text spells `needle` as a run of numbers: an array of bytes,
/// halfwords or words, assembler data lines, escapes or a hex dump.
fn spells(text: &[u8], needle: &[u8]) -> bool {
    [false, true].into_iter().any(|pairs| {
        let mut run = Vec::new();
        for token in text
            .split(numeric_separator)
            .filter(|token| !token.is_empty())
        {
            if let Some(bytes) = token_bytes(token, pairs) {
                run.extend(bytes);
            } else if !DATA_DIRECTIVES
                .iter()
                .any(|directive| token.eq_ignore_ascii_case(directive.as_bytes()))
            {
                if contains(&run, needle) {
                    return true;
                }
                run.clear();
            }
        }
        contains(&run, needle)
    })
}
fn numeric_elements(text: &[u8]) -> usize {
    let separator = numeric_separator;
    let (mut total, mut run) = (0, 0);
    for token in text.split(separator).filter(|token| !token.is_empty()) {
        if numeric(token) {
            run += 1;
            continue;
        }
        total += if run >= NUMERIC_RUN_MIN { run } else { 0 };
        run = 0;
    }
    total + if run >= NUMERIC_RUN_MIN { run } else { 0 }
}
fn encoded_reason(text: &str, arrays: bool) -> Option<&'static str> {
    let (mut characters, mut digests) = (0, 0);
    for run in text.as_bytes().split(|byte| !byte.is_ascii_alphanumeric()) {
        match classify(run) {
            Run::Encoded => characters += run.len(),
            Run::Digest => digests += 1,
            Run::Plain => {}
        }
    }
    if characters > ENCODED_CHARACTERS_MAX {
        return Some(
            "encoded payload (base64, base32 or hex): pret commits only editable build inputs",
        );
    }
    if digests > DIGEST_RUNS_MAX {
        return Some(
            "encoded payload in digest-sized hex runs: pret commits only editable build inputs",
        );
    }
    (arrays && numeric_elements(text.as_bytes()) > NUMERIC_ELEMENTS_MAX).then_some(
        "numeric array outside the game data tables: pret commits only editable build inputs",
    )
}

fn calculated_bookkeeping_reason(text: &str) -> Option<&'static str> {
    let header = text.lines().next().unwrap_or("");
    header
        .split(['\t', ','])
        .map(|field| field.trim().trim_matches('"'))
        .any(|field| {
            listed(
                field,
                &[
                    "decoded_sha256",
                    "encoded_sha256",
                    "payload_sha256",
                    "plan_sha256",
                    "owner_inventory",
                    "executable_bytes",
                    "rom_fallback_bytes",
                ],
            )
        })
        .then_some(GENERATED_REASON)
}

fn blocked_include(literal: &str, bytes: bool) -> bool {
    let literal = literal.replace('\\', "/");
    literal.split('/').any(|component| {
        listed(component, &["out", "roms"]) || (bytes && listed(component, &["games", "recon"]))
    }) || listed(extension(&literal), PRESENTATION_EXTENSIONS)
        || listed(extension(&literal), BLOCKED_EXTENSIONS)
}
fn included_bytes_reason(path: &str, text: &str) -> Option<&'static str> {
    if !extension(path).eq_ignore_ascii_case("rs") {
        return None;
    }
    let embedded = text.match_indices("include_").any(|(index, word)| {
        if text[..index]
            .bytes()
            .next_back()
            .is_some_and(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
        {
            return false;
        }
        let rest = &text[index + word.len()..];
        let (bytes, rest) = match (rest.strip_prefix("bytes"), rest.strip_prefix("str")) {
            (Some(rest), _) => (true, rest),
            (_, Some(rest)) => (false, rest),
            _ => return false,
        };
        let Some(rest) = rest.trim_start().strip_prefix('!') else {
            return false;
        };
        let rest = rest.trim_start();
        let (open, close) = match rest.chars().next() {
            Some('(') => ('(', ')'),
            Some('[') => ('[', ']'),
            Some('{') => ('{', '}'),
            _ => return false,
        };
        let mut depth = 0;
        let end = rest
            .char_indices()
            .find(|(_, ch)| {
                depth += i32::from(*ch == open) - i32::from(*ch == close);
                depth == 0
            })
            .map_or(rest.len(), |(end, _)| end);
        let literals: Vec<_> = rest[..end].split('"').skip(1).step_by(2).collect();
        blocked_include(&literals.concat(), bytes)
            || literals
                .into_iter()
                .any(|literal| blocked_include(literal, bytes))
    });
    embedded.then_some("include_bytes! or include_str! of game, ROM or output bytes")
}
fn attributes_reason(path: &str, text: &str) -> Option<&'static str> {
    let leaf = path.rsplit('/').next().unwrap_or(path);
    let filtered = leaf.eq_ignore_ascii_case(".gitattributes")
        && text
            .lines()
            .filter(|line| !line.trim_start().starts_with('#'))
            .flat_map(str::split_whitespace)
            .any(|attribute| attribute.starts_with("filter="));
    filtered.then_some(
        "Git filter attribute such as filter=lfs: content would bypass the publication gate",
    )
}
fn publication_data_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    if json_path(path) {
        return Some(JSON_REASON);
    }
    let logo = logo.filter(|_| path != LOGO_SOURCE);
    if logo.is_some_and(|logo| contains(data, logo) || (!binary(data) && spells(data, logo))) {
        return Some(LOGO_REASON);
    }
    if listed(extension(path), &["asm", "inc", "s"]) && incbin(path, data) {
        return Some("committed incbin payload");
    }
    if data.starts_with(b"version https://git-lfs") || data.starts_with(b"version https://hawser") {
        return Some("Git LFS pointer: content stored outside Git bypasses the publication gate");
    }
    if let Some(reason) =
        publication_content_reason(data).or_else(|| presentation_magic_reason(data))
    {
        return Some(reason);
    }
    if binary(data) || extension(path).eq_ignore_ascii_case("pcm4") {
        return binary_reason(path, data, logo);
    }
    let text = std::str::from_utf8(data).unwrap_or("");
    if json_text(text) {
        return Some(JSON_REASON);
    }
    if let Some(reason) = patch_reason(path, data) {
        return Some(reason);
    }
    let table = (asset_game(path).is_some() && listed(extension(path), DATA_TABLE_EXTENSIONS))
        || (path.starts_with("recon/") && listed(extension(path), &["s", "inc"]));
    data_uri_reason(text)
        .or_else(|| encoded_reason(text, !table))
        .or_else(|| calculated_bookkeeping_reason(text))
        .or_else(|| included_bytes_reason(path, text))
        .or_else(|| attributes_reason(path, text))
        .or_else(|| address_equate_reason(path, text))
        .or_else(|| edition_equate_reason(path, text))
        .or_else(|| linker_assignment_reason(path, text))
        .or_else(|| c_alias_reason(path, text))
        .or_else(|| raw_address_reason(path, text))
        .or_else(|| raw_encoding_reason(path, text))
        .or_else(|| uncredited_code_reason(path, text))
}
const C_ALIAS_REASON: &str =
    "GCC alias attribute in maintained C (O2): define one name where its bytes are";
fn c_alias_reason(path: &str, text: &str) -> Option<&'static str> {
    if !path.starts_with("games/") || !listed(extension(path), &["c", "h"]) {
        return None;
    }
    let text = text.replace("\\\r\n", "").replace("\\\n", "");
    if !text.contains("__attribute") {
        return None;
    }
    static TOKEN: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let token = TOKEN
        .get_or_init(|| regex::Regex::new(r"[A-Za-z_][A-Za-z0-9_]*|\S").expect("C token pattern"));
    let code = unquoted_source(&text);
    let tokens = token
        .find_iter(&code)
        .map(|found| found.as_str())
        .collect::<Vec<_>>();
    let matching = |open: usize| {
        let mut depth = 0;
        for (at, token) in tokens.iter().enumerate().skip(open) {
            match *token {
                "(" => depth += 1,
                ")" if depth == 1 => return Some(at),
                ")" => depth -= 1,
                _ => {}
            }
        }
        None
    };
    for index in 0..tokens.len() {
        if !matches!(tokens[index], "__attribute" | "__attribute__")
            || tokens.get(index + 1) != Some(&"(")
            || tokens.get(index + 2) != Some(&"(")
        {
            continue;
        }
        let end = matching(index + 2)?;
        let mut at = index + 3;
        while at < end {
            if matches!(tokens[at], "alias" | "__alias__") && tokens.get(at + 1) == Some(&"(") {
                return Some(C_ALIAS_REASON);
            }
            at = if tokens[at] == "(" {
                matching(at)? + 1
            } else {
                at + 1
            };
        }
    }
    None
}
fn unquoted_source(text: &str) -> String {
    static NON_CODE: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    NON_CODE
        .get_or_init(|| {
            regex::Regex::new(r#"(?s)/\*.*?\*/|//[^\r\n]*|@[^\r\n]*|"(?:\\.|[^"\\])*""#)
                .expect("source comments and strings pattern")
        })
        .replace_all(text, " ")
        .into_owned()
}
const LINKER_ASSIGNMENT_REASON: &str = "linker alias or stored number (O2): define the name where its bytes are; layout and sizes come from sections and symbol differences";
fn linker_section_expression(text: &str) -> bool {
    static TOKEN: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let token = TOKEN.get_or_init(|| {
        regex::Regex::new(r"[A-Za-z_.$][\w.$]*|0[xX][0-9a-fA-F]+|[0-9]+|[()+-]")
            .expect("linker arithmetic token pattern")
    });
    let mut tokens = Vec::new();
    let mut end = 0;
    for found in token.find_iter(text) {
        if !text[end..found.start()].trim().is_empty() {
            return false;
        }
        tokens.push(found.as_str());
        end = found.end();
    }
    if !text[end..].trim().is_empty() {
        return false;
    }
    fn take(tokens: &[&str], index: &mut usize, wanted: &str) -> bool {
        if tokens.get(*index) != Some(&wanted) {
            return false;
        }
        *index += 1;
        true
    }
    fn atom(tokens: &[&str], index: &mut usize, section: &mut bool) -> bool {
        let Some(&word) = tokens.get(*index) else {
            return false;
        };
        *index += 1;
        match word {
            "+" | "-" => atom(tokens, index, section),
            "(" | "ABSOLUTE" => {
                (word == "(" || take(tokens, index, "("))
                    && expression(tokens, index, section)
                    && take(tokens, index, ")")
            }
            "LOADADDR" | "ADDR" => {
                if !take(tokens, index, "(") {
                    return false;
                }
                let Some(&name) = tokens.get(*index) else {
                    return false;
                };
                if !name
                    .starts_with(|c: char| c.is_ascii_alphabetic() || matches!(c, '_' | '.' | '$'))
                {
                    return false;
                }
                *index += 1;
                *section = true;
                take(tokens, index, ")")
            }
            ")" => false,
            _ => tokens.get(*index) != Some(&"("),
        }
    }
    fn expression(tokens: &[&str], index: &mut usize, section: &mut bool) -> bool {
        if !atom(tokens, index, section) {
            return false;
        }
        while matches!(tokens.get(*index), Some(&"+" | &"-")) {
            *index += 1;
            if !atom(tokens, index, section) {
                return false;
            }
        }
        true
    }
    let (mut index, mut section) = (0, false);
    expression(&tokens, &mut index, &mut section) && index == tokens.len() && section
}
fn linker_assignment_reason(path: &str, text: &str) -> Option<&'static str> {
    let parts = path.split('/').collect::<Vec<_>>();
    if !matches!(parts.as_slice(), ["games", _, leaf] if extension(leaf).eq_ignore_ascii_case("ld"))
    {
        return None;
    }
    static ASSIGNMENT: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    static DIFFERENCE: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let assignment = ASSIGNMENT.get_or_init(|| {
        regex::Regex::new(r"([A-Za-z_.$][\w.$]*)\s*=\s*([^;{}]+);")
            .expect("linker assignment pattern")
    });
    let difference = DIFFERENCE.get_or_init(|| {
        regex::Regex::new(
            r"^\s*(?:[A-Za-z_.$][\w.$]*\s*-\s*[A-Za-z_.$][\w.$]*|ABSOLUTE\s*\(\s*[A-Za-z_.$][\w.$]*\s*-\s*[A-Za-z_.$][\w.$]*\s*\))\s*$",
        )
        .expect("linker symbol difference pattern")
    });
    let code = unquoted_source(text);
    assignment
        .captures_iter(&code)
        .any(|capture| {
            &capture[1] != "."
                && !difference.is_match(&capture[2])
                && !linker_section_expression(&capture[2])
        })
        .then_some(LINKER_ASSIGNMENT_REASON)
}
const RAW_ADDRESS_REASON: &str = "numeric call target or ROM/RAM literal-pool address in raw disassembly (O2): reference a label where its bytes are";
fn numeric_word(text: &str) -> Option<u32> {
    crate::assembly::addresses::integer(text)
}
fn memory_address(word: u32) -> bool {
    (0x0200_0000..0x0204_0000).contains(&word)
        || (0x0300_0000..0x0300_8000).contains(&word)
        || (0x0800_0000..0x0e00_0000).contains(&word)
}
fn register_name(word: &str) -> Option<&str> {
    match word {
        "ip" => Some("r12"),
        "sp" => Some("r13"),
        "lr" => Some("r14"),
        "pc" => Some("r15"),
        _ => word
            .strip_prefix('r')
            .and_then(|digits| digits.parse::<u8>().ok())
            .filter(|&number| number < 16)
            .map(|_| word),
    }
}
fn loaded_pointer(lines: &[&str], register: &str) -> bool {
    let Some(register) = register_name(register) else {
        return false;
    };
    let mut aliases = BTreeSet::from([register.to_string()]);
    for line in lines {
        let line = line.split_once(':').map_or(*line, |(_, rest)| rest).trim();
        let Some((word, operands)) = line.split_once(char::is_whitespace) else {
            continue;
        };
        let word = word.to_ascii_lowercase();
        if matches!(
            word.as_str(),
            ".section"
                | ".text"
                | ".data"
                | ".bss"
                | ".thumb_func"
                | ".4byte"
                | ".word"
                | ".long"
                | ".int"
        ) {
            break;
        }
        let registers = operands
            .split(|c: char| !c.is_ascii_alphanumeric())
            .filter_map(register_name)
            .collect::<Vec<_>>();
        if word.starts_with("ldr") || word.starts_with("str") {
            let base = operands.split_once('[').and_then(|(_, rest)| {
                rest.split(|c: char| !c.is_ascii_alphanumeric())
                    .find_map(register_name)
            });
            if base.is_some_and(|base| aliases.contains(base)) {
                return true;
            }
        }
        if (word.starts_with("ldm") || word.starts_with("stm"))
            && registers
                .first()
                .is_some_and(|base| aliases.contains(*base))
        {
            return true;
        }
        if matches!(word.as_str(), "bx" | "blx")
            && registers
                .first()
                .is_some_and(|target| aliases.contains(*target))
        {
            return true;
        }
        if word == "bl"
            && operands
                .trim()
                .strip_prefix("_call_via_")
                .and_then(register_name)
                .is_some_and(|target| aliases.contains(target))
        {
            return true;
        }
        if word.starts_with("mov") && registers.len() == 2 {
            if registers[0] == "r15" && aliases.contains(registers[1]) {
                return true;
            }
            let copied = aliases.contains(registers[1]);
            aliases.remove(registers[0]);
            if copied {
                aliases.insert(registers[0].to_string());
            }
        } else if word == "bl" || word == "blx" || (word.starts_with("bl") && word.len() > 3) {
            for register in ["r0", "r1", "r2", "r3", "r4", "r12", "r14"] {
                aliases.remove(register);
            }
        } else if word == "pop" {
            for register in &registers {
                aliases.remove(*register);
            }
            if registers.contains(&"r15") {
                break;
            }
        } else if matches!(word.as_str(), "b" | "bal" | "bx") {
            break;
        } else if assembly_instruction(&word)
            && !word.starts_with('b')
            && !word.starts_with("str")
            && !word.starts_with("stm")
            && !matches!(word.as_str(), "cmp" | "cmn" | "tst" | "teq" | "push")
        {
            if let Some(register) = registers.first() {
                aliases.remove(*register);
            }
        }
        if aliases.is_empty() {
            break;
        }
    }
    false
}
fn loaded_pointer_words(words: &[Option<u32>], start: usize, register: u32) -> bool {
    let mut states = BTreeMap::from([(start, 1_u16 << register)]);
    let mut queue = std::collections::VecDeque::from([start]);
    while let Some(index) = queue.pop_front() {
        let Some(Some(word)) = words.get(index) else {
            continue;
        };
        let before = states[&index];
        let mut aliases = before;
        let conditional = word >> 28 != 14;
        let mut next = vec![index + 1];
        let base = (word >> 16) & 15;
        let destination = (word >> 12) & 15;
        let memory = word & 0x0c00_0000 == 0x0400_0000;
        let multiple = word & 0x0e00_0000 == 0x0800_0000;
        if (memory || multiple) && aliases & (1 << base) != 0 {
            return true;
        }
        if word & 0x0fff_fff0 == 0x012f_ff10 {
            if aliases & (1 << (word & 15)) != 0 {
                return true;
            }
            next.clear();
        } else if word & 0x0fff_0ff0 == 0x01a0_0000 {
            let copied = aliases & (1 << (word & 15)) != 0;
            if destination == 15 {
                if copied {
                    return true;
                }
                next.clear();
            }
            aliases &= !(1 << destination);
            if copied {
                aliases |= 1 << destination;
            }
        } else if memory {
            if word & (1 << 20) != 0 {
                aliases &= !(1 << destination);
            }
        } else if multiple {
            if word & (1 << 20) != 0 {
                aliases &= !(*word as u16);
            }
        } else if word & 0x0e00_0000 == 0x0a00_0000 {
            if word & (1 << 24) == 0 {
                let displacement = ((*word as i32) << 8) >> 8;
                let target = index as i64 + 2 + i64::from(displacement);
                next.clear();
                if 0 <= target && target < words.len() as i64 {
                    next.push(target as usize);
                }
                if conditional {
                    next.push(index + 1);
                }
            } else {
                aliases &= !0x501f;
            }
        } else if word & 0x0c00_0000 == 0 {
            let opcode = (word >> 21) & 15;
            if !matches!(opcode, 8..=11) {
                if destination == 15 {
                    next.clear();
                } else if matches!(opcode, 2 | 4) && word & (1 << 25) != 0 {
                    let copied = aliases & (1 << base) != 0;
                    aliases &= !(1 << destination);
                    if copied {
                        aliases |= 1 << destination;
                    }
                } else {
                    aliases &= !(1 << destination);
                }
            }
        } else {
            continue;
        }
        if conditional {
            aliases |= before;
            if next.is_empty() {
                next.push(index + 1);
            }
        }
        if aliases == 0 {
            continue;
        }
        for next in next {
            if next >= words.len() {
                continue;
            }
            let old = states.entry(next).or_default();
            let combined = *old | aliases;
            if *old != combined {
                *old = combined;
                queue.push_back(next);
            }
        }
    }
    false
}
fn raw_address_reason(path: &str, text: &str) -> Option<&'static str> {
    let parts = path.split('/').collect::<Vec<_>>();
    let listing = matches!(parts.as_slice(), ["recon", _, "raw", .., leaf] if extension(leaf).eq_ignore_ascii_case("s"));
    let overlay_data = matches!(parts.as_slice(), ["games", _, "src", "system", "overlay", leaf] if extension(leaf).eq_ignore_ascii_case("s"));
    if !listing && !overlay_data {
        return None;
    }
    static CALL: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    static LOAD: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let call = CALL.get_or_init(|| {
        regex::Regex::new(r"(?im)^\s*bl(?:x|eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(?:\.[nw])?\s+#?(?:0x[0-9a-f]+|[0-9]+)\b")
            .expect("numeric assembly call pattern")
    });
    let load = LOAD.get_or_init(|| {
        regex::Regex::new(r"(?im)^\s*ldr(?:eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al)?(?:\.[nw])?\s+(r(?:1[0-5]|[0-9])|ip|lr|sp),\s*(=?[A-Za-z_.$][\w.$]*|=0x[0-9a-f]+|=[0-9]+)\s*$")
            .expect("assembly literal pool load pattern")
    });
    let code = unquoted_source(text).to_ascii_lowercase();
    if call.is_match(&code) || !crate::assembly::addresses::numeric_calls(&code).is_empty() {
        return Some(RAW_ADDRESS_REASON);
    }
    if !crate::assembly::addresses::sites(
        &[&code],
        &crate::assembly::addresses::Types::default(),
    )
    .is_empty()
    {
        return Some(RAW_ADDRESS_REASON);
    }
    let mut pools = BTreeSet::new();
    let lines = code.lines().collect::<Vec<_>>();
    for (index, line) in lines.iter().enumerate() {
        let Some(capture) = load.captures(line) else {
            continue;
        };
        if !loaded_pointer(&lines[index + 1..], &capture[1]) {
            continue;
        }
        let operand = capture.get(2).expect("literal pool operand").as_str();
        if operand
            .strip_prefix('=')
            .and_then(numeric_word)
            .is_some_and(memory_address)
        {
            return Some(RAW_ADDRESS_REASON);
        }
        pools.insert(operand.trim_start_matches('='));
    }
    let mut pool = false;
    let mut encoded = Vec::new();
    let inspect_encoded = |words: &[Option<u32>]| {
        words.iter().enumerate().any(|(index, word)| {
            let Some(word) = word else { return false };
            if word & 0x0f7f_0000 != 0x051f_0000 || word & 3 != 0 {
                return false;
            }
            if !loaded_pointer_words(words, index + 1, (word >> 12) & 15) {
                return false;
            }
            let displacement = (word & 0xfff) as isize / 4;
            let target = index as isize
                + 2
                + if word & (1 << 23) != 0 {
                    displacement
                } else {
                    -displacement
                };
            target >= 0
                && words
                    .get(target as usize)
                    .copied()
                    .flatten()
                    .is_some_and(memory_address)
        })
    };
    for line in code.lines() {
        let mut line = line.trim();
        if let Some((label, rest)) = line.split_once(':') {
            pool = pools.contains(label.trim());
            line = rest.trim();
        }
        if line.is_empty() || line.starts_with(".align") {
            continue;
        }
        let Some((directive, operands)) = line.split_once(char::is_whitespace) else {
            continue;
        };
        if matches!(
            directive,
            ".section" | ".text" | ".data" | ".bss" | ".pushsection" | ".popsection" | ".previous"
        ) {
            if inspect_encoded(&encoded) {
                return Some(RAW_ADDRESS_REASON);
            }
            encoded.clear();
            pool = false;
            continue;
        }
        if matches!(directive, ".4byte" | ".word" | ".long" | ".int") {
            for operand in operands.split(',') {
                let word = numeric_word(operand);
                if pool && word.is_some_and(memory_address) {
                    return Some(RAW_ADDRESS_REASON);
                }
                encoded.push(word);
            }
        } else if !directive.starts_with('.')
            || matches!(directive, ".2byte" | ".hword" | ".byte" | ".short")
        {
            if inspect_encoded(&encoded) {
                return Some(RAW_ADDRESS_REASON);
            }
            encoded.clear();
            pool = false;
        }
    }
    inspect_encoded(&encoded).then_some(RAW_ADDRESS_REASON)
}
const EDITION_EQUATE_REASON: &str = "equate in an edition scaffold: it brings in base-ROM bytes and labels where they are, never a number the linked code reads";
fn edition_equate_reason(path: &str, text: &str) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let ["recon", game, lang, leaf] = components.as_slice() else {
        return None;
    };
    let edition = lang.len() == 2 && lang.bytes().all(|b| b.is_ascii_lowercase());
    if !matches!(*game, "tbs" | "tla") || !edition || !extension(leaf).eq_ignore_ascii_case("s") {
        return None;
    }
    static EQUATE: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let equate = EQUATE.get_or_init(|| {
        regex::Regex::new(r"(?im)^[ \t]*(?:\.(?:set|equ|equiv|eqv)\b|[A-Za-z_.$][\w.$]*[ \t]*=)")
            .expect("edition equate pattern")
    });
    equate.is_match(text).then_some(EDITION_EQUATE_REASON)
}
const ADDRESS_EQUATE_REASON: &str = "assembler equate of a full address: define the name as a label where its bytes are and reference it";
fn address_equate_reason(path: &str, text: &str) -> Option<&'static str> {
    let game = path.starts_with("games/") || path.starts_with("recon/");
    if !game || !listed(extension(path), &["s", "inc", "asm"]) {
        return None;
    }
    static EQUATE: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let equate = EQUATE.get_or_init(|| {
        regex::Regex::new(
            r"(?im)^[ \t]*\.(?:set|equ|equiv|eqv)[ \t]+[^,\s]+[ \t]*,[ \t]*0x[0-9a-f]{8}(?:[^0-9a-z_]|$)",
        )
        .expect("address equate pattern")
    });
    equate.is_match(text).then_some(ADDRESS_EQUATE_REASON)
}
const RAW_ENCODING_REASON: &str = "credited assembly spelled as an encoding (.inst, or data that control runs into as code): write the instruction, or keep the module as disassembly in recon/<game>/raw";
#[derive(Clone, Copy, PartialEq, Eq)]
enum Statement {
    Label(bool),
    Instruction {
        ends_flow: bool,
        uncredited: bool,
    },
    Encoding,
    Data(bool),
    Section,
    Directive,
    Macro,
}
fn assembly_instruction(mnemonic: &str) -> bool {
    static MNEMONIC: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    MNEMONIC
        .get_or_init(|| {
            regex::Regex::new(
                r"(?i)^(?:adc|add|adr|and|asr|bic|blx|bl|bx|b|cdp|cmn|cmp|eor|ldc|ldm|ldr|lsl|lsr|mcr|mla|mov|mrc|mrs|msr|mul|mvn|neg|nop|orr|pop|push|ror|rsb|rsc|sbc|smlal|smull|stc|stm|str|sub|svc|swi|swp|teq|tst|umlal|umull)(?:eq|ne|cs|hs|cc|lo|mi|pl|vs|vc|hi|ls|ge|lt|gt|le|al|s|b|h|sb|sh|t|bt|ia|ib|da|db|fd|ed|fa|ea)*(?:\.n|\.w)?$",
            )
            .expect("instruction mnemonic pattern")
        })
        .is_match(mnemonic)
}
fn assembly_ends_flow(mnemonic: &str, operands: &str) -> bool {
    let mnemonic = mnemonic.to_ascii_lowercase();
    let mnemonic = mnemonic
        .strip_suffix(".n")
        .or_else(|| mnemonic.strip_suffix(".w"))
        .unwrap_or(&mnemonic);
    let operands: String = operands
        .chars()
        .filter(|c| !c.is_whitespace())
        .collect::<String>()
        .to_ascii_lowercase();
    let writes_pc = operands.starts_with("pc,") || operands.starts_with("r15,");
    let lists_pc = operands
        .split(|c: char| !c.is_ascii_alphanumeric())
        .any(|register| register == "pc" || register == "r15");
    match mnemonic {
        "b" | "bal" | "bx" => true,
        "pop" => lists_pc,
        "ldm" | "ldmia" | "ldmib" | "ldmda" | "ldmdb" | "ldmfd" | "ldmed" | "ldmfa" | "ldmea" => {
            lists_pc && operands.contains('{')
        }
        "mov" | "movs" | "add" | "adds" | "sub" | "subs" | "ldr" => writes_pc,
        _ => false,
    }
}
fn assembly_statements(text: &str) -> Vec<Statement> {
    let mut statements = Vec::new();
    let (mut block, mut function_next, mut uncredited) = (false, false, false);
    let mut functions = BTreeSet::new();
    for line in text.lines() {
        let mut code = String::new();
        let mut chars = line.chars().peekable();
        let mut quoted = false;
        while let Some(c) = chars.next() {
            if block {
                if c == '*' && chars.peek() == Some(&'/') {
                    chars.next();
                    block = false;
                }
                continue;
            }
            if quoted {
                quoted = c != '"';
                continue;
            }
            match c {
                '"' => quoted = true,
                '@' => break,
                '/' if chars.peek() == Some(&'/') => break,
                '/' if chars.peek() == Some(&'*') => {
                    chars.next();
                    block = true;
                }
                _ => code.push(c),
            }
        }
        if code.trim_start().starts_with('#') {
            continue;
        }
        for part in code.split(';') {
            let mut part = part.trim();
            while let Some((name, rest)) = part.split_once(':').filter(|(name, _)| {
                !name.is_empty()
                    && name
                        .chars()
                        .all(|c| c.is_ascii_alphanumeric() || matches!(c, '_' | '.' | '$'))
            }) {
                if name.starts_with("AlchemyUncreditedEnd_") {
                    uncredited = false;
                } else if name.starts_with("AlchemyUncredited_") {
                    uncredited = true;
                }
                statements.push(Statement::Label(
                    std::mem::take(&mut function_next) || functions.contains(name),
                ));
                part = rest.trim_start();
            }
            if part.is_empty() {
                continue;
            }
            let (word, operands) = part
                .split_once(char::is_whitespace)
                .map_or((part, ""), |(word, rest)| (word, rest.trim()));
            let directive = word.to_ascii_lowercase();
            let statement = match directive.as_str() {
                ".inst" | ".inst.n" | ".inst.w" => Statement::Encoding,
                ".byte" | ".2byte" | ".hword" | ".short" | ".4byte" | ".word" | ".long"
                | ".int" | ".fill" | ".space" | ".skip" | ".zero" => Statement::Data(uncredited),
                ".text" | ".data" | ".bss" | ".section" | ".pushsection" | ".popsection"
                | ".previous" => Statement::Section,
                ".thumb_func" => {
                    function_next = true;
                    Statement::Directive
                }
                ".type" => {
                    if let Some((name, kind)) = operands.split_once(',') {
                        if kind.trim().trim_start_matches(['%', '@', '#']) == "function" {
                            functions.insert(name.trim().to_owned());
                        }
                    }
                    Statement::Directive
                }
                _ if directive.starts_with('.') => Statement::Directive,
                _ if assembly_instruction(word) => Statement::Instruction {
                    ends_flow: assembly_ends_flow(word, operands),
                    uncredited: uncredited && !word.eq_ignore_ascii_case("nop"),
                },
                _ => Statement::Macro,
            };
            statements.push(statement);
        }
    }
    statements
}
const UNCREDITED_CODE_REASON: &str = "an AlchemyUncredited span holds an instruction other than nop (C2): only padding, data and alignment are left out of the count";
fn uncredited_code_reason(path: &str, text: &str) -> Option<&'static str> {
    if !path.starts_with("games/") || !listed(extension(path), &["s", "inc", "asm"]) {
        return None;
    }
    assembly_statements(text)
        .iter()
        .any(|statement| {
            matches!(
                statement,
                Statement::Instruction {
                    uncredited: true,
                    ..
                }
            )
        })
        .then_some(UNCREDITED_CODE_REASON)
}
fn raw_encoding_reason(path: &str, text: &str) -> Option<&'static str> {
    if !path.starts_with("games/") || !listed(extension(path), &["s", "inc", "asm"]) {
        return None;
    }
    let statements = assembly_statements(text);
    for (index, statement) in statements.iter().enumerate() {
        match statement {
            Statement::Encoding => return Some(RAW_ENCODING_REASON),
            Statement::Data(false) => {}
            _ => continue,
        }
        let mut entry = false;
        let before = statements[..index]
            .iter()
            .rev()
            .find(|previous| match previous {
                Statement::Label(function) => {
                    entry |= *function;
                    false
                }
                Statement::Directive => false,
                _ => true,
            });
        if let Some(Statement::Instruction {
            ends_flow: false, ..
        }) = before {
            return Some(RAW_ENCODING_REASON);
        }
        let after = statements[index..]
            .iter()
            .find(|next| !matches!(next, Statement::Data(_) | Statement::Directive));
        if entry && matches!(after, Some(Statement::Instruction { .. })) {
            return Some(RAW_ENCODING_REASON);
        }
    }
    None
}
fn asset_game(path: &str) -> Option<&str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, game, area, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("games") || rest.is_empty() {
        return None;
    }
    let asset = if area.eq_ignore_ascii_case("asm") {
        rest.len() > 1 && rest[0].eq_ignore_ascii_case("overlays")
    } else {
        !listed(area, METADATA_DIRECTORIES)
    };
    asset.then_some(*game)
}
fn shared_root_reason(path: &str) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, root, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("games") || !root.eq_ignore_ascii_case("common") {
        return None;
    }
    let source = matches!(rest, ["src", _, .., leaf]
        if listed(extension(leaf), &["c", "s", "png", "tsv"]));
    let interface = matches!(rest, ["include", _, .., leaf] if extension(leaf) == "h");
    let sequence = matches!(rest, ["sound", "sequence", leaf]
        if listed(extension(leaf), &["mid", "s"]));
    let sample = matches!(rest, ["sound", "sample", leaf]
        if listed(extension(leaf), &["wav", "pcm4"]));
    (!(source || interface || sequence || sample)).then_some(
        "games/common holds only shared src/<module>/ sources and inputs, include/<module>/*.h \
         and sound/ sequences and samples",
    )
}
const RECON_REASON: &str = "recon/<game> holds only raw disassembly and its linker scripts, the top-level assembly scaffolding, an edition's assembly scaffold and main.ld and C drafts under an edition";
fn recon_path_reason(path: &str) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("recon") {
        return None;
    }
    let game = |name: &str| matches!(name, "tbs" | "tla");
    let edition = |name: &str| name.len() == 2 && name.bytes().all(|b| b.is_ascii_lowercase());
    let named = |leaf: &str, extensions: &[&str]| {
        leaf.rsplit_once('.')
            .is_some_and(|(stem, suffix)| !stem.is_empty() && extensions.contains(&suffix))
    };
    let scaffolding = *top == "recon"
        && match rest {
            [g, .., ".gitkeep"] => game(g),
            [g, leaf] => game(g) && named(leaf, &["s"]),
            [g, "raw", leaf] => game(g) && named(leaf, &["s", "S"]),
            [g, "raw", "overlays", leaf] => game(g) && named(leaf, &["s", "ld"]),
            [g, e, "main.ld"] => game(g) && edition(e),
            [g, e, leaf] if named(leaf, &["s"]) => game(g) && edition(e),
            [g, e, .., leaf] => game(g) && edition(e) && named(leaf, &["c", "h"]),
            _ => false,
        };
    (!scaffolding).then_some(RECON_REASON)
}
fn native_path_reason(path: &str) -> Option<&'static str> {
    if let Some(reason) = shared_root_reason(path).or_else(|| recon_path_reason(path)) {
        return Some(reason);
    }
    let components: Vec<_> = path.split('/').collect();
    let [top, _, area, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("games") {
        return None;
    }
    let native = *top == "games"
        && ((matches!(*area, "src" | "include" | "sound" | "text")
            && !rest.is_empty()
            && listed(extension(path), NATIVE_INPUT_EXTENSIONS))
            || (rest.is_empty() && listed(extension(path), &["ld", "mk"])));
    (!native).then_some("game material must be an editable native game input")
}
fn byte_dump(message: &str) -> bool {
    let bytes = message.as_bytes();
    let word = |byte: u8| byte.is_ascii_alphanumeric() || byte == b'_';
    let mut index = 0;
    let mut previous = None;
    let mut streak = 0;
    while index < bytes.len() {
        while index < bytes.len() && !word(bytes[index]) {
            index += 1;
        }
        if index == bytes.len() {
            break;
        }
        let start = index;
        while index < bytes.len() && word(bytes[index]) {
            index += 1;
        }
        let pair = index - start == 2 && bytes[start..index].iter().all(u8::is_ascii_hexdigit);
        if !pair {
            streak = 0;
            previous = None;
            continue;
        }
        let joined = previous.is_some_and(|end| {
            bytes[end..start]
                .iter()
                .all(|byte| matches!(byte, b' ' | b'\t'))
        });
        streak = if joined { streak + 1 } else { 1 };
        previous = Some(index);
        if streak == 8 {
            return true;
        }
    }
    false
}
fn commit_message_reason(message: &str) -> Option<&'static str> {
    byte_dump(message).then_some("commit message contains a raw byte dump")
}
fn git(root: &Path, args: &[&str], label: &str) -> Result<Vec<u8>, String> {
    let result = Command::new("git")
        .arg("-C")
        .arg(root)
        .args(args)
        .output()
        .map_err(|error| format!("{label} failed: {error}"))?;
    if result.status.success() {
        return Ok(result.stdout);
    }
    let stderr = String::from_utf8_lossy(&result.stderr).trim().to_string();
    Err(if stderr.is_empty() {
        format!("{label} failed")
    } else {
        stderr
    })
}
fn nul_list(value: &[u8]) -> Vec<String> {
    value
        .split(|byte| *byte == 0)
        .filter(|field| !field.is_empty())
        .map(|field| String::from_utf8_lossy(field).into_owned())
        .collect()
}
fn raw_changes(value: &[u8]) -> Result<(bool, Vec<(String, bool)>), String> {
    let fields = nul_list(value);
    let mut changes = Vec::new();
    let mut index = 0;
    while index < fields.len() {
        let metadata: Vec<_> = fields[index].split_whitespace().collect();
        if metadata.len() != 5 || !metadata[0].starts_with(':') {
            return Err("invalid raw git diff".to_string());
        }
        index += 1;
        let paired = metadata[4].starts_with('R') || metadata[4].starts_with('C');
        if index + usize::from(paired) >= fields.len() {
            return Err("invalid raw git diff".to_string());
        }
        let path = fields[index + usize::from(paired)].clone();
        index += 1 + usize::from(paired);
        if metadata[4] != "D" {
            changes.push((path, metadata[1] == "160000"));
        }
    }
    Ok((!fields.is_empty(), changes))
}
struct Entry {
    scope: String,
    path: String,
    object: String,
    listing_reason: Option<&'static str>,
}
fn inspected(scope: &str, path: String, object: String, gitlink: bool) -> Option<Entry> {
    let listing_reason = if gitlink {
        if APPROVED_GITLINKS.contains(&path.as_str()) {
            return None;
        }
        Some("unapproved gitlink: only the agbcc and agscc submodules are approved")
    } else {
        native_path_reason(&path)
    };
    Some(Entry {
        scope: scope.to_string(),
        path,
        object,
        listing_reason,
    })
}
fn blobs(
    root: &Path,
    objects: Vec<String>,
    mut visit: impl FnMut(usize, &[u8]),
) -> Result<(), String> {
    let mut child = Command::new("git")
        .arg("-C")
        .arg(root)
        .args(["cat-file", "--batch"])
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .spawn()
        .map_err(|error| format!("blob scan failed: {error}"))?;
    let mut input = child.stdin.take().expect("piped stdin");
    let requests: String = objects.iter().map(|object| format!("{object}\n")).collect();
    let writer = std::thread::spawn(move || input.write_all(requests.as_bytes()));
    let mut output = BufReader::new(child.stdout.take().expect("piped stdout"));
    let mut data = Vec::new();
    for (index, object) in objects.iter().enumerate() {
        let mut header = String::new();
        output
            .read_line(&mut header)
            .map_err(|error| format!("blob scan failed: {error}"))?;
        let fields: Vec<_> = header.split_whitespace().collect();
        let size = match fields.as_slice() {
            [_, "blob", size] => size.parse::<usize>().ok(),
            _ => None,
        }
        .ok_or_else(|| format!("blob {object}: {}", header.trim()))?;
        data.resize(size + 1, 0);
        output
            .read_exact(&mut data)
            .map_err(|error| format!("blob {object} failed: {error}"))?;
        visit(index, &data[..size]);
    }
    drop(output);
    writer
        .join()
        .map_err(|_| "blob scan writer panicked".to_string())?
        .map_err(|error| format!("blob scan failed: {error}"))?;
    child
        .wait()
        .map_err(|error| format!("blob scan failed: {error}"))?;
    Ok(())
}
fn scan(root: &Path, entries: Vec<Entry>) -> Result<(), String> {
    let logo = nintendo_logo(root);
    let mut failures = Vec::new();
    let mut readable = Vec::new();
    for entry in entries {
        match publication_path_reason(&entry.path).or(entry.listing_reason) {
            Some(reason) => failures.push(format!("{} {}: {reason}", entry.scope, entry.path)),
            None => readable.push(entry),
        }
    }
    let objects = readable.iter().map(|entry| entry.object.clone()).collect();
    blobs(root, objects, |index, data| {
        let entry = &readable[index];
        if let Some(reason) = publication_data_reason(&entry.path, data, logo.as_deref()) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
        }
    })?;
    if failures.is_empty() {
        Ok(())
    } else {
        Err(format!(
            "publication gate rejected:\n{}",
            failures.join("\n")
        ))
    }
}
fn tracked(root: &Path, revision: Option<&str>) -> Result<Vec<(bool, String, String)>, String> {
    let output = match revision {
        None => git(root, &["ls-files", "--stage", "-z"], "index scan")?,
        Some(revision) => git(
            root,
            &["ls-tree", "-r", "-z", "--full-tree", revision],
            &format!("tree scan {revision}"),
        )?,
    };
    let mut entries = Vec::new();
    for record in nul_list(&output) {
        let (metadata, path) = record
            .split_once('\t')
            .ok_or_else(|| "invalid git tree listing".to_string())?;
        let fields: Vec<_> = metadata.split_whitespace().collect();
        if fields.len() != 3 {
            return Err("invalid git tree listing".to_string());
        }
        let object = if revision.is_some() {
            fields[2]
        } else {
            fields[1]
        };
        let repeated = entries
            .last()
            .is_some_and(|(_, _, last): &(bool, String, String)| last == path);
        if !repeated {
            entries.push((fields[0] == "160000", object.to_string(), path.to_string()));
        }
    }
    Ok(entries)
}
fn tree_entries(root: &Path, revision: Option<&str>) -> Result<Vec<Entry>, String> {
    let records = tracked(root, revision)?;
    let scope = revision.map_or("tree".to_string(), |revision| {
        format!("tree {}", &revision[..12.min(revision.len())])
    });
    Ok(records
        .into_iter()
        .filter_map(|(gitlink, object, path)| inspected(&scope, path, object, gitlink))
        .collect())
}
fn check_tree(root: &Path, revision: Option<&str>) -> Result<(), String> {
    let entries = tree_entries(root, revision)?;
    if entries.is_empty() {
        return Err("publication gate scanned nothing: the tree is empty".to_string());
    }
    scan(root, entries)
}
fn check_staged(root: &Path) -> Result<(), String> {
    let output = git(
        root,
        &[
            "diff",
            "--cached",
            "--raw",
            "--no-abbrev",
            "--find-renames",
            "-z",
        ],
        "staged path scan",
    )?;
    let (anything, changes) = raw_changes(&output)?;
    if !anything {
        if git(
            root,
            &["rev-parse", "--verify", "MERGE_HEAD"],
            "pending merge",
        )
        .is_ok()
        {
            return check_tree(root, None);
        }
        return Err("publication gate scanned nothing: no staged change to inspect".to_string());
    }
    let entries = changes
        .into_iter()
        .filter_map(|(path, gitlink)| {
            let object = format!(":{path}");
            inspected("staged", path, object, gitlink)
        })
        .collect();
    scan(root, entries)
}
fn revisions(root: &Path, local: &str, remote: &str) -> Result<Vec<String>, String> {
    let mut args = vec!["rev-list", local, "--not", "--remotes"];
    if !remote.bytes().all(|byte| byte == b'0') {
        args.push(remote);
    }
    git(root, &args, &format!("outgoing revision scan {local}")).map(|output| {
        String::from_utf8_lossy(&output)
            .lines()
            .map(str::trim)
            .filter(|line| !line.is_empty())
            .map(str::to_string)
            .collect()
    })
}
fn check_push(root: &Path, updates: &str) -> Result<(), String> {
    let updates: Vec<_> = updates
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .collect();
    if updates.is_empty() {
        return Ok(());
    }
    let mut commits = Vec::new();
    let mut tips = Vec::new();
    for update in updates {
        let fields: Vec<_> = update.split_whitespace().collect();
        if fields.len() != 4 {
            return Err("invalid pre-push update".to_string());
        }
        if fields[1].bytes().all(|byte| byte == b'0') {
            continue;
        }
        if !tips.iter().any(|tip| tip == fields[1]) {
            tips.push(fields[1].to_string());
        }
        for commit in revisions(root, fields[1], fields[3])? {
            if !commits.contains(&commit) {
                commits.push(commit);
            }
        }
    }
    let mut message_failures = Vec::new();
    for commit in &commits {
        let message = git(
            root,
            &["show", "-s", "--format=%B", commit],
            &format!("commit message {commit}"),
        )?;
        if let Some(reason) = commit_message_reason(&String::from_utf8_lossy(&message)) {
            message_failures.push(format!("{}: {reason}", &commit[..12.min(commit.len())]));
        }
    }
    if !message_failures.is_empty() {
        let count = message_failures.len();
        return Err(format!(
            "{}\nrefusing to publish {count} commit message(s)",
            message_failures.join("\n")
        ));
    }
    let mut entries = Vec::new();
    for commit in commits {
        let output = git(
            root,
            &[
                "diff-tree",
                "--root",
                "--no-commit-id",
                "--raw",
                "--no-abbrev",
                "--find-renames",
                "-r",
                "-z",
                &commit,
            ],
            &format!("commit path scan {commit}"),
        )?;
        let (_, changes) = raw_changes(&output)?;
        let scope = &commit[..12.min(commit.len())];
        entries.extend(changes.into_iter().filter_map(|(path, gitlink)| {
            inspected(scope, path.clone(), format!("{commit}:{path}"), gitlink)
        }));
    }
    for tip in tips {
        entries.extend(tree_entries(root, Some(&tip))?);
    }
    scan(root, entries)
}
const USAGE: &str = "usage: check publication [--staged | --pre-push | --tree [REV]]\n\nModes:\n  --staged       Check staged files before committing.\n  --pre-push     Check commits absent from remotes and each pushed tree using update lines on stdin.\n  --tree [REV]   Check every file tracked in the index, or in revision REV.\n  -h, --help     Show this help.";
fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}
pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    match arguments {
        [argument] if argument == "-h" || argument == "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        [argument] if argument == "--staged" => {
            check_staged(root).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        [argument] if argument == "--tree" => {
            check_tree(root, None).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        [argument, revision] if argument == "--tree" => check_tree(root, Some(revision))
            .map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS),
        [argument] if argument == "--pre-push" => {
            let mut updates = String::new();
            if let Err(error) = std::io::stdin().read_to_string(&mut updates) {
                return fail(&format!("pre-push stdin failed: {error}"));
            }
            check_push(root, &updates).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        _ => fail(USAGE),
    }
}
#[cfg(test)]
mod tests {
    use super::*;

    fn publication_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
        publication_path_reason(path)
            .or_else(|| native_path_reason(path))
            .or_else(|| publication_data_reason(path, data, logo))
    }
    fn fixture_bytes(length: usize, seed: u32) -> Vec<u8> {
        let mut state = seed;
        (0..length)
            .map(|_| {
                state = state.wrapping_mul(1_103_515_245).wrapping_add(12_345);
                (state >> 16) as u8
            })
            .collect()
    }
    fn encoded_fixture(alphabet: &[u8], length: usize, seed: u32) -> String {
        fixture_bytes(length, seed)
            .into_iter()
            .map(|byte| alphabet[usize::from(byte) % alphabet.len()] as char)
            .collect()
    }
    fn crc32(bytes: &[u8]) -> u32 {
        !bytes.iter().fold(!0u32, |crc, byte| {
            (0..8).fold(crc ^ u32::from(*byte), |crc, _| {
                (crc >> 1) ^ (0xedb8_8320 & 0u32.wrapping_sub(crc & 1))
            })
        })
    }
    fn png_chunk(kind: &[u8], body: &[u8]) -> Vec<u8> {
        let mut chunk = (body.len() as u32).to_be_bytes().to_vec();
        chunk.extend_from_slice(kind);
        chunk.extend_from_slice(body);
        let crc = crc32(&chunk[4..]);
        chunk.extend_from_slice(&crc.to_be_bytes());
        chunk
    }
    fn scanlines(size: usize, depth: usize) -> Vec<u8> {
        let row = (size * depth).div_ceil(8) + 1;
        vec![0; row * size]
    }
    fn png_fixture(
        size: u32,
        depth: u8,
        colour: u8,
        stream: &[u8],
        extra: &[(&[u8], &[u8])],
    ) -> Vec<u8> {
        let header = [
            size.to_be_bytes().as_slice(),
            &size.to_be_bytes(),
            &[depth, colour, 0, 0, 0],
        ]
        .concat();
        let palette: Vec<u8> = (0..1usize << depth)
            .flat_map(|index| [index as u8, (index * 2) as u8, 255 - index as u8])
            .collect();
        let mut data = PNG_SIGNATURE.to_vec();
        data.extend(png_chunk(b"IHDR", &header));
        data.extend(png_chunk(b"PLTE", &palette));
        for (kind, body) in extra {
            data.extend(png_chunk(kind, body));
        }
        data.extend(png_chunk(b"IDAT", stream));
        data.extend(png_chunk(b"IEND", &[]));
        data
    }
    fn indexed_fixture(depth: u8) -> Vec<u8> {
        let stream = fdeflate::compress_to_vec(&scanlines(16, usize::from(depth)));
        png_fixture(16, depth, 3, &stream, &[])
    }
    fn grey_fixture(depth: u8) -> Vec<u8> {
        let stream = fdeflate::compress_to_vec(&scanlines(16, usize::from(depth)));
        let header = [
            16u32.to_be_bytes().as_slice(),
            &16u32.to_be_bytes(),
            &[depth, 3, 0, 0, 0],
        ]
        .concat();
        let palette: Vec<u8> = (0..1usize << depth)
            .flat_map(|index| [((index * 255) >> depth) as u8; 3])
            .collect();
        [
            PNG_SIGNATURE.as_slice(),
            &png_chunk(b"IHDR", &header),
            &png_chunk(b"PLTE", &palette),
            &png_chunk(b"IDAT", &stream),
            &png_chunk(b"IEND", &[]),
        ]
        .concat()
    }
    fn midi_fixture(events: &[u8], closed: bool) -> Vec<u8> {
        let mut track = [&[0, 0x90, 60, 100, 96, 0x80, 60, 0], events].concat();
        if closed {
            track.extend([0, 0xff, 0x2f, 0]);
        }
        [
            b"MThd".as_slice(),
            &[0, 0, 0, 6, 0, 0, 0, 1, 0, 96],
            b"MTrk",
            &(track.len() as u32).to_be_bytes(),
            &track,
        ]
        .concat()
    }
    fn logo_fixture() -> Vec<u8> {
        fixture_bytes(LOGO.len(), 0x0bad_1060)
    }
    type Fixture = (&'static str, Vec<u8>, Option<&'static str>);
    fn binary_fixtures() -> Vec<Fixture> {
        let logo = logo_fixture();
        let font = [b"OTTO".as_slice(), &[0, 10, 0, 128, 0, 3, 0, 32], b"CFF "].concat();
        let mut fragment = vec![0u8; 0x1000];
        fragment[..4].copy_from_slice(&[0x2e, 0, 0, 0xea]);
        fragment[0xb2] = 0x96;
        let sum = fragment[0xa0..=0xbc]
            .iter()
            .fold(0u8, |sum, byte| sum.wrapping_add(*byte));
        fragment[0xbd] = 0u8.wrapping_sub(sum).wrapping_sub(0x19);
        let blank = scanlines(16, 4);
        let stream = fdeflate::compress_to_vec(&blank);
        let mut trailing = indexed_fixture(4);
        trailing.extend_from_slice(&[0; 4]);
        let surplus = fdeflate::compress_to_vec(&[blank.as_slice(), &[0; 64]].concat());
        let second = [
            stream.clone(),
            fdeflate::compress_to_vec(&fixture_bytes(512, 9)),
        ]
        .concat();
        let mut row = logo.clone();
        row.resize(160, 0);
        let mut hidden = scanlines(160, 8);
        hidden[0] = 1;
        for index in 0..row.len() {
            let left = index.checked_sub(1).map_or(0, |left| row[left]);
            hidden[1 + index] = row[index].wrapping_sub(left);
        }
        let hidden = fdeflate::compress_to_vec(&hidden);
        let wave = ags::assets::wav::pcm8_wav(&[0; 64], 8000).unwrap();
        let listed_wave = [
            b"RIFF".as_slice(),
            &(wave.len() as u32 + 4).to_le_bytes(),
            &wave[8..36],
            b"LIST\x04\0\0\0INFO",
            &wave[36..],
        ]
        .concat();
        let samples: Vec<u8> = logo.iter().map(|byte| byte.wrapping_sub(128)).collect();
        let midi = midi_fixture(&[], true);
        let meta = |kind: u8, payload: &[u8]| {
            ags::assets::midi::append_conductor_meta(&midi, kind, payload).unwrap()
        };
        let base64: Vec<u8> = (b'A'..=b'Z')
            .chain(b'a'..=b'z')
            .chain(b'0'..=b'9')
            .chain(*b"+/")
            .collect();
        let directive = [b"alchemy-mid2agb\0".as_slice(), b"{\"format\":1}"].concat();
        let table = [b"ALCHTOK1".as_slice(), &[9, 0xd0, 0x01, 0, 5, 8, 3, 7]].concat();
        let garbled = [b"ALCHTOK1".as_slice(), &fixture_bytes(64, 5)].concat();
        let unregistered = Some("unregistered binary");
        let malformed_png = Some("exact indexed build input");
        let wav = Some("canonical mono 8-bit PCM");
        let sequence = Some("exact sequence build input");
        let nintendo = Some("Nintendo logo");
        vec![
            (
                "tools/alchemy/GRAPHICS/Weyard.otf",
                font.clone(),
                Some("presentation material"),
            ),
            ("notes.dat", font, Some("font")),
            (
                "x.PNG",
                b"GIF89a\x10\0\x10\0\x80\0\0".to_vec(),
                Some("GIF image"),
            ),
            (
                "games/tbs/preview/djinn_101_idle.gif",
                b"GIF89a".to_vec(),
                Some("presentation material"),
            ),
            (
                "games/tbs/preview/title.png",
                indexed_fixture(4),
                Some("PREVIEW"),
            ),
            (
                "games/tbs/src/graphics/tile/shot.png",
                png_fixture(16, 8, 6, &stream, &[]),
                Some("truecolour"),
            ),
            (
                "games/tbs/src/graphics/tile/idle.indexed.png",
                png_fixture(16, 4, 3, &stream, &[(b"acTL", &[0, 0, 0, 2, 0, 0, 0, 0])]),
                Some("animated PNG"),
            ),
            (
                "games/tbs/src/table.dat",
                b"\x01\x02\0\x03".to_vec(),
                Some("editable native game input"),
            ),
            (
                "games/tbs/sound/sample/wave_00.pcm4",
                vec![0x5a; 65],
                unregistered,
            ),
            ("tools/alchemy/tests/tone.wav", wave.clone(), unregistered),
            (
                "games/tbs/sound/x.png",
                indexed_fixture(4),
                unregistered,
            ),
            (
                "tools/alchemy/tests/header.dat",
                fragment,
                Some("ROM header fragment"),
            ),
            (
                "games/tla/sound/sequence/x.mid",
                midi.clone(),
                None,
            ),
            (
                "games/tbs/src/graphics/tile/trailing.indexed.png",
                trailing,
                malformed_png,
            ),
            (
                "games/tbs/src/graphics/tile/text.indexed.png",
                png_fixture(16, 4, 3, &stream, &[(b"tEXt", b"Comment\0payload")]),
                malformed_png,
            ),
            (
                "games/tbs/src/graphics/tile/surplus.indexed.png",
                png_fixture(16, 4, 3, &surplus, &[]),
                malformed_png,
            ),
            (
                "games/tbs/src/graphics/tile/second.indexed.png",
                png_fixture(16, 4, 3, &second, &[]),
                malformed_png,
            ),
            (
                "games/tbs/src/graphics/tile/logo.indexed.png",
                png_fixture(160, 8, 3, &hidden, &[]),
                nintendo,
            ),
            (
                "games/tbs/sound/sample/long.pcm8.wav",
                [wave.as_slice(), &[0]].concat(),
                wav,
            ),
            (
                "games/tbs/sound/sample/list.pcm8.wav",
                listed_wave,
                wav,
            ),
            (
                "games/tbs/sound/sample/logo.pcm8.wav",
                ags::assets::wav::pcm8_wav(&samples, 8000).unwrap(),
                nintendo,
            ),
            (
                "games/tbs/sound/sequence/chunk.mid",
                [midi.as_slice(), b"XXXX\0\0\0\x01\0"].concat(),
                sequence,
            ),
            (
                "games/tbs/sound/sequence/sysex.mid",
                midi_fixture(&[0, 0xf0, 3, 1, 2, 0xf7], true),
                sequence,
            ),
            (
                "games/tbs/sound/sequence/binary.mid",
                meta(0x7f, &[0xff, 0xfe, 0x80]),
                sequence,
            ),
            (
                "games/tbs/sound/sequence/open.mid",
                midi_fixture(&[], false),
                sequence,
            ),
            (
                "games/tbs/sound/sequence/tempo.mid",
                meta(0x51, &fixture_bytes(64, 3)),
                sequence,
            ),
            (
                "games/tbs/sound/sequence/timed.mid",
                meta(0x51, &[7, 0xa1, 0x20]),
                None,
            ),
            (
                "games/tbs/sound/sequence/payload.mid",
                meta(0x01, encoded_fixture(&base64, 600, 2).as_bytes()),
                Some("encoded payload"),
            ),
            (
                "games/tbs/src/graphics/common/spare.tokens",
                table.clone(),
                Some("stored compression token table"),
            ),
            (
                "games/tbs/src/graphics/common/compression.tokens",
                garbled,
                Some("stored compression token table"),
            ),
            (
                "games/tbs/src/graphics/font/localization_glyphs_0020_00ff.1bpp.png",
                indexed_fixture(1),
                None,
            ),
            (
                "games/tbs/src/graphics/tile/ui_mtf_00.indexed.png",
                indexed_fixture(4),
                None,
            ),
            (
                "games/tbs/src/graphics/tile/ui_tile.8bpp.png",
                png_fixture(
                    16,
                    8,
                    3,
                    &fdeflate::compress_to_vec(&scanlines(16, 8)),
                    &[(b"tRNS", &[0])],
                ),
                None,
            ),
            (
                "games/tbs/text/staff_roll_moji.1bpp.png",
                indexed_fixture(1),
                None,
            ),
            (
                "games/tbs/text/ink_and_paper.1bpp.png",
                grey_fixture(1),
                None,
            ),
            (
                "games/tbs/src/graphics/tile/ui_tile.4bpp.png",
                grey_fixture(4),
                Some("grey sheet"),
            ),
            (
                "games/tla/sound/sample/wave_00.pcm8.wav",
                wave,
                None,
            ),
            (
                "games/tbs/sound/sequence/theme.mid",
                meta(0x7f, &directive),
                None,
            ),
            (
                "games/tbs/sound/sequence/skeleton.mid",
                meta(0x01, b"smsh-sequence 1\nstream track_1\n"),
                None,
            ),
            (
                "games/tbs/sound/sequence/json.mid",
                meta(0x01, br#"{"format":1,"layout":[]}"#),
                Some("JSON"),
            ),
            (
                "games/tbs/sound/sequence/marker.mid",
                meta(0x06, br#"["fine"]"#),
                Some("JSON"),
            ),
            (
                "games/tbs/sound/sequence/placed.mid",
                meta(0x01, b"smsh-sequence 1\nheader s base=0x08000000\n"),
                Some("ROM address or placement"),
            ),
            (
                "games/tbs/sound/sequence/tone_bank.mid",
                meta(0x01, b"smsh-sequence 1\nheader s externals=tone_bank\n"),
                Some("ROM address or placement"),
            ),
            (
                "games/tbs/sound/sequence/literal.mid",
                meta(0x06, b"goto 0x0815fb78"),
                Some("ROM address or placement"),
            ),
            (
                "games/tbs/sound/sample/wave_00.pcm4",
                vec![0x5a; 16],
                None,
            ),
            (
                "games/tbs/src/graphics/common/compression.tokens",
                table,
                Some("stored compression token table"),
            ),
        ]
    }
    fn text_fixtures() -> Vec<Fixture> {
        let tbs = "games/tbs";
        let letters = || (b'A'..=b'Z').chain(b'a'..=b'z').chain(b'0'..=b'9');
        let base64: Vec<u8> = letters().chain(*b"+/").collect();
        let base64url: Vec<u8> = letters().chain(*b"-_").collect();
        let base32: Vec<u8> = (b'A'..=b'Z').chain(b'2'..=b'7').collect();
        let hex: Vec<u8> = (b'0'..=b'9').chain(b'a'..=b'f').collect();
        let text = |value: String| value.into_bytes();
        let uri = |media: &str| {
            format!(
                "data{}{media};base64,{}",
                ':',
                encoded_fixture(&base64, 24, 7)
            )
        };
        let lines = |width: usize, count: u32| {
            (0..count)
                .map(|seed| format!("  {}\n", encoded_fixture(&base64, width, seed)))
                .collect::<String>()
        };
        let quoted = (0..12)
            .map(|seed| format!("    \"{}\",\n", encoded_fixture(&base64url, 32, seed)))
            .collect::<String>();
        let digests = |count: u64| {
            (0..count)
                .map(|seed| format!("{:016x}\n", seed.wrapping_mul(0x9e37_79b9_7f4a_7c15)))
                .collect::<String>()
        };
        let checksums = (0..4u64)
            .map(|seed| {
                format!(
                    "checksum = \"{:064x}\"\n",
                    u128::from(seed) * 0x9e37_79b9_7f4a_7c15
                )
            })
            .collect::<String>();
        let rows = (0..16)
            .map(|_| format!("\"{}\"", "01".repeat(52)))
            .collect::<Vec<_>>()
            .join(",\n        ");
        let rows = format!("{{\"logo\": [\n        {rows}\n]}}\n");
        let bytes = fixture_bytes(4096, 6);
        let joined = |format: fn(&u8) -> String, separator: &str| {
            bytes.iter().map(format).collect::<Vec<_>>().join(separator)
        };
        let array = joined(|byte| format!("0x{byte:02x}"), ", ");
        let decimal = joined(|byte| byte.to_string(), ", ");
        let escapes = joined(|byte| format!("\\x{byte:02x}"), "");
        let dump = joined(|byte| format!("{byte:02x}"), " ");
        let identifiers = (0..400)
            .map(|index| {
                let address = 0x0200_d650 + index * 4;
                let body = format!("RunOverlayObjectCommand{index}(); Func_{address:08x}();");
                format!("void SelectActor{index}SceneVariant(void) {{ {body} }}\n")
            })
            .collect::<String>();
        let pointer = format!(
            "version https://git-lfs.github.com/spec/v1\noid sha256:{}\nsize 8388608\n",
            encoded_fixture(&hex, 64, 3)
        );
        let bang = "!";
        let logo_include = format!(
            "include_bytes{bang} (\"../../../games/{}/LOGO.DAT\")",
            &tbs[6..]
        );
        let header_include =
            format!("include_str {bang} [concat{bang}(\"../ro\", \"ms/header.json\")]");
        let source_include = format!("include_str{bang}(\"../../../../{tbs}/asm/08002d5c.s\")");
        let svg_uri = text(format!("<svg><image href=\"{}\"/></svg>", uri("image/png")));
        let css_uri = text(format!(
            "<style>@font-face{{src:url({})}}</style>",
            uri("font/otf")
        ));
        let rust_uri = text(format!(
            "format!(\"{}{{}}\", x)",
            uri("application/octet-stream")
        ));
        let json_base64 = text(format!(
            "{{\"bytes\": \"{}\"}}\n",
            encoded_fixture(&base64, 300, 3)
        ));
        let base32_key = text(format!(
            "const KEY: &str = \"{}\";\n",
            encoded_fixture(&base32, 400, 4)
        ));
        let hex_blob = text(format!(
            "{{\"blob\": \"{}\"}}\n",
            encoded_fixture(&hex, 400, 5)
        ));
        let quoted = text(format!("const DATA: &[&str] = &[\n{quoted}];\n"));
        let array = text(format!("const DATA: [u8; 4096] = [{array}];\n"));
        let escapes = text(format!("const DATA: &[u8] = b\"{escapes}\";\n"));
        let dump = text(format!("// {dump}\n"));
        let c_table = text(format!("const u8 table[] = {{{decimal}}};\n"));
        let json_table = text(format!("{{\"values\": [{decimal}]}}\n"));
        let logo_include = text(format!("const LOGO: &[u8] = {logo_include};\n"));
        let header_include = text(format!("const HEADER: &str = {header_include};\n"));
        let source_include = text(format!("let source = {source_include};\n"));
        let attributes = b"*.PNG filter=lfs diff=lfs merge=lfs -text\n".to_vec();
        let svg = b"<svg><style>.label{font-family:monospace}</style><rect/></svg>".to_vec();
        let empty = || b"{}\n".to_vec();
        let encoded = Some("encoded payload");
        let arrays = Some("numeric array");
        let native = Some("editable native game input");
        let include = Some("include_bytes! or include_str!");
        let uri = Some("data URI");
        vec![
            ("README.md", svg_uri, uri),
            ("tools/alchemy/src/dashboard/index.html", css_uri, uri),
            ("tools/alchemy/src/coverage/figure.rs", rust_uri, uri),
            (
                "games/tbs/src/system/blob.json",
                json_base64,
                Some(JSON_REASON),
            ),
            (
                "tools/alchemy/src/dashboard/font.css",
                text(lines(76, 5)),
                encoded,
            ),
            (
                "tools/alchemy/src/dashboard/glyphs.css",
                text(lines(40, 12)),
                encoded,
            ),
            ("tools/alchemy/src/assets.rs", quoted, encoded),
            ("tools/alchemy/src/key.rs", base32_key, encoded),
            ("tools/alchemy/src/blob.json", hex_blob, Some(JSON_REASON)),
            (
                "tools/alchemy/src/words.rs",
                text(digests(17_000)),
                Some("digest-sized"),
            ),
            ("tools/alchemy/src/rom_table.rs", array, arrays),
            ("tools/alchemy/src/rom_bytes.rs", escapes, arrays),
            ("tools/alchemy/src/rom_dump.rs", dump, arrays),
            ("games/tbs/src/battle/table.c", c_table, arrays),
            (
                "games/tbs/src/battle/data/table.json",
                json_table,
                Some(JSON_REASON),
            ),
            ("games/tla/data/table.tsv", empty(), native),
            ("games/tla/SRC/battle/table.tsv", empty(), native),
            ("Games/tbs/src/table.tsv", empty(), native),
            ("tools/alchemy/src/logo.rs", logo_include, include),
            ("tools/alchemy/src/header.rs", header_include, include),
            (
                "tools/alchemy/src/recovery/fixture.rs",
                source_include,
                None,
            ),
            (
                "tools/alchemy/assets/figure.png",
                text(pointer),
                Some("Git LFS pointer"),
            ),
            (".gitattributes", attributes, Some("filter attribute")),
            (".gitattributes", b"*.TOKENS binary\n".to_vec(), None),
            ("recon/tla/raw/overlays/.gitkeep", Vec::new(), None),
            (
                "games/tla/src/main/x.c",
                b"void f(void) {}\n".to_vec(),
                None,
            ),
            (
                "games/common/src/sound/x.c",
                b"void f(void) {}\n".to_vec(),
                None,
            ),
            (
                "games/common/src/x.c",
                b"void f(void) {}\n".to_vec(),
                Some("games/common holds only"),
            ),
            (
                "games/common/src/graphics/font/text.tsv",
                b"first\t20\n".to_vec(),
                None,
            ),
            (
                "games/common/src/sound/x.h",
                b"void f(void);\n".to_vec(),
                Some("games/common holds only"),
            ),
            (
                "games/common/include/sound/x.h",
                b"void f(void);\n".to_vec(),
                None,
            ),
            (
                "games/common/sound/sequence/x.s",
                b"\t.section .rodata\n".to_vec(),
                None,
            ),
            (
                "games/common/sound/sample/x.s",
                b"\t.section .rodata\n".to_vec(),
                Some("games/common holds only"),
            ),
            (
                "games/common/sound/sequence/deep/x.s",
                b"\t.section .rodata\n".to_vec(),
                Some("games/common holds only"),
            ),
            (
                "games/common/src/sound/table.dat",
                empty(),
                Some("games/common holds only"),
            ),
            (
                "games/common/recon/translation-units.tsv",
                empty(),
                Some("games/common holds only"),
            ),
            (
                "games/tbs/src/field/scene/scene.c",
                text(identifiers),
                None,
            ),
            ("tools/Cargo.lock", text(checksums), None),
            ("tools/alchemy/src/hashes.rs", text(digests(2_048)), None),
            (
                "games/tbs/src/system/rom_header.json",
                text(rows),
                Some(JSON_REASON),
            ),
            (
                "tools/alchemy/src/dashboard/style.css",
                b"body { font: 16px mono; }\n".to_vec(),
                None,
            ),
            ("PROGRESS.svg", svg, Some("presentation material")),
        ]
    }
    fn json_ban_fixtures() -> Vec<Fixture> {
        [
            "games/tbs/src/game/table.json",
            "tools/alchemy/data/settings.json",
            "out/report.jsonl",
            "README.JSON5",
        ]
        .into_iter()
        .map(|path| (path, b"x".to_vec(), Some("JSON is banned")))
        .collect()
    }
    fn check_fixtures() -> Result<(), String> {
        let logo = logo_fixture();
        let fixtures = binary_fixtures()
            .into_iter()
            .chain(text_fixtures())
            .chain(json_ban_fixtures());
        for (path, data, expected) in fixtures {
            let actual = publication_reason(path, &data, Some(&logo));
            let holds = match expected {
                Some(fragment) => actual.is_some_and(|reason| reason.contains(fragment)),
                None => actual.is_none(),
            };
            if !holds {
                return Err(format!(
                    "publication fixture {path}: expected {expected:?}, got {actual:?}"
                ));
            }
        }
        Ok(())
    }

    #[test]
    fn publication_fixtures_follow_the_current_policy() {
        check_fixtures().unwrap();
    }

    #[test]
    fn the_logo_spelled_in_numbers_is_rejected_outside_the_fix_up_tool() {
        let logo = logo_fixture();
        let joined = |bytes: &[u8], format: &dyn Fn(&u8) -> String, separator: &str| {
            bytes.iter().map(format).collect::<Vec<_>>().join(separator)
        };
        let hex = |byte: &u8| format!("0x{byte:02x}");
        let array = format!("const LOGO: [u8; 156] = [{}];\n", joined(&logo, &hex, ", "));
        let rows = logo
            .chunks(12)
            .map(|row| format!("\t.byte {}\n", joined(row, &hex, ", ")))
            .collect::<String>();
        let words = logo
            .chunks(4)
            .map(|word| {
                let word = u32::from_le_bytes(word.try_into().unwrap());
                format!("\t.4byte 0x{word:08x}\n")
            })
            .collect::<String>();
        let spelled = [
            ("tools/alchemy/src/header.rs", array.clone()),
            ("tools/gbafix/src/gbafix.rs", array.clone()),
            (
                "tools/alchemy/src/escapes.rs",
                format!(
                    "const LOGO: &[u8] = b\"{}\";\n",
                    joined(&logo, &|byte| format!("\\x{byte:02x}"), "")
                ),
            ),
            (
                "tools/alchemy/src/dump.rs",
                format!("// {}\n", joined(&logo, &|byte| format!("{byte:02x}"), " ")),
            ),
            ("games/tbs/src/system/rom_head.s", rows),
            ("games/tla/src/system/rom_head.s", words),
            (
                "games/tbs/src/system/logo.c",
                format!(
                    "const s8 logo[] = {{{}}};\n",
                    joined(&logo, &|byte| (*byte as i8).to_string(), ", ")
                ),
            ),
        ];
        for (path, text) in spelled {
            let reason = publication_reason(path, text.as_bytes(), Some(&logo));
            assert_eq!(reason, Some(LOGO_REASON), "{path}");
        }
        let exempt = publication_reason(LOGO_SOURCE, array.as_bytes(), Some(&logo));
        assert_eq!(exempt, None, "the fix-up tool keeps the logo (L1)");
        let header = "tools/alchemy/src/header.rs";
        assert_eq!(publication_reason(header, array.as_bytes(), None), None);
        let part = joined(&logo[1..], &hex, ", ");
        let part = format!("const PART: [u8; 155] = [{part}];\n");
        let partial = publication_reason(header, part.as_bytes(), Some(&logo));
        assert_eq!(partial, None, "155 of the logo's bytes are not the logo");
    }

    #[test]
    fn only_the_fix_up_tool_may_carry_the_real_logo() {
        let source = include_str!("../gbafix.rs").as_bytes();
        let logo = crate::gbafix::NINTENDO_LOGO;
        assert!(spells(source, &logo));
        assert_eq!(publication_reason(LOGO_SOURCE, source, Some(&logo)), None);
        assert_eq!(
            publication_reason("tools/alchemy/src/header.rs", source, Some(&logo)),
            Some(LOGO_REASON)
        );
    }

    #[test]
    fn maintained_c_names_are_real_definitions_not_gcc_alias_attributes() {
        for path in [
            "games/tbs/src/battle/draw_party_panels_with_empty_list.c",
            "games/tbs/src/battle/select_weighted_index.c",
            "games/tbs/src/graphics/text/draw_draw_localized_resource.c",
            "games/tla/src/system/a.c",
            "games/common/include/system/a.h",
            "games/common/src/system/a.c",
        ] {
            for source in [
                "extern void Public(void) __attribute__((alias(\"Target\")));",
                "extern int Public __attribute (( __alias__ (\"Target\")));",
                "extern void Public(void) __attribute__ /* a */ ( /* b */ ( /* c */ __alias__ /* d */ (\"Target\") /* e */ ) /* f */ );",
                "extern void Public(void) __attribute__((unused, alias(\"Target\"), aligned(4)));",
                "extern void Public(void) __attri\\\nbute__((a\\\nlias(\"Target\")));",
                "#define PUBLIC __attribute__((alias(\"Target\")))\nextern void Public(void) PUBLIC;",
                "#define PUBLIC \\\n __attribute__((__alias__(\"Target\")))\nextern void Public(void) PUBLIC;",
            ] {
                assert_eq!(
                    publication_reason(path, source.as_bytes(), None),
                    Some(C_ALIAS_REASON),
                    "{path}: {source}"
                );
            }
        }
    }

    #[test]
    fn alias_text_and_unrelated_attributes_do_not_claim_another_definition() {
        for source in [
            "/* __attribute__((alias(\"Target\"))) */\nint Ordinary;",
            "// __attribute__((__alias__(\"Target\")))\nint Ordinary;",
            "const char *note = \"__attribute__((alias(\\\"Target\\\")))\";",
            "const char *note = \"escaped \\\" quote; __attribute__((__alias__(\\\"Target\\\")))\";",
            "int alias(void); void f(void) { alias(); }",
            "int __alias__(void); void f(void) { __alias__(); }",
            "int value __attribute__((section(\"alias\"), aligned(4)));",
            "void f(void) __attribute__((format(printf, 1, 2)));",
            "int value __attribute__((aligned(sizeof(alias()))));",
            "int value __attribute__((aligned(sizeof(obj.__alias__))));",
            "#define NOTE \"__attribute__((alias(\\\"Target\\\")))\"\nint Ordinary;",
            "#define NOTE /* __attribute__((alias(\"Target\"))) */ 1\nint Ordinary;",
        ] {
            assert_eq!(
                publication_reason("games/common/include/system/a.h", source.as_bytes(), None),
                None,
                "{source}"
            );
        }
        let source = b"extern void Public(void) __attribute__((alias(\"Target\")));";
        for path in [
            "tools/tests/A.c",
            "recon/tbs/en/main/a.c",
            "games/tla/src/system/a.s",
        ] {
            assert_eq!(publication_reason(path, source, None), None, "{path}");
        }
    }

    #[test]
    fn only_base_rom_ranges_and_built_overlays_may_incbin_and_only_in_scaffolding() {
        let range = b".incbin \"baserom.gba\", 0x00037464, 0x0003c3a4\n";
        assert!(!super::incbin("recon/tbs/unidentified.s", range));
        assert!(super::incbin("games/tbs/src/data.s", range));
        assert!(super::incbin(
            "recon/tbs/unidentified.s",
            b".incbin \"TEXT.BIN\"\n"
        ));
        assert!(super::incbin(
            "recon/tbs/unidentified.s",
            b".incbin \"baserom.gba\"\n"
        ));
        let overlay = b".incbin \"overlays/resource_36f.lz\"\n";
        assert!(!super::incbin("recon/tbs/overlays.s", overlay));
        assert!(super::incbin("games/tbs/src/data.s", overlay));
        assert!(super::incbin(
            "recon/tbs/overlays.s",
            b".incbin \"overlays/resource_36f.bin\"\n"
        ));
    }

    #[test]
    fn a_games_overlay_source_may_incbin_only_its_built_overlay_streams() {
        let source = "games/tbs/src/system/overlay/overlays.s";
        let overlay = b"Resource_Overlay36F:\n\t.incbin \"overlays/resource_36f.lz\"\n";
        assert!(!super::incbin(source, overlay));
        assert!(!super::incbin("games/tla/src/system/overlay/overlays.s", overlay));
        for path in [
            "games/common/src/system/overlay/overlays.s",
            "games/tbs/src/system/overlay/ovl_36f.s",
            "games/tbs/src/system/overlays.s",
            "games/tbs/src/overlay/overlays.s",
            "games/tbs/sound/overlays.s",
        ] {
            assert!(super::incbin(path, overlay), "{path}");
        }
        for line in [
            b".incbin \"baserom.gba\", 0x007795e6, 0x00000002\n".as_slice(),
            b".incbin \"overlays/resource_36f.bin\"\n",
            b".incbin \"overlays/resource_36f.lz\", 0, 16\n",
            b".incbin \"overlays/../../roms/tbs-en.gba\"\n",
            b".incbin \"OVERLAYS/RESOURCE_36F.lz\"\n",
        ] {
            assert!(super::incbin(source, line), "{}", String::from_utf8_lossy(line));
        }
    }

    #[test]
    fn sound_data_sources_may_incbin_only_the_sound_files_the_build_makes() {
        let sample = b"Sound_Wave00:\n\t.incbin \"sound/sample/wave_00.pcm8.bin\"\n";
        let wave = b"\t.incbin \"sound/sample/cgb_wave_0.bin\"\n";
        let shared = b"\t.incbin \"common/sound/sample/wave_20.pcm8.bin\"\n";
        for path in [
            "games/tbs/sound/samples.s",
            "games/tla/sound/cgb_waves.s",
        ] {
            assert!(!super::incbin(path, sample), "{path}");
            assert!(!super::incbin(path, wave), "{path}");
            assert!(!super::incbin(path, shared), "{path}");
        }
        assert!(super::incbin(
            "games/tbs/sound/samples.s",
            b"\t.incbin \"common/sample/wave_20.pcm8.bin\"\n"
        ));
        for (path, line) in [
            ("games/tbs/src/sound/data.s", sample.as_slice()),
            ("recon/tbs/unidentified.s", sample),
            ("games/common/sound/samples.s", sample),
            (
                "games/tbs/sound/samples.s",
                b".incbin \"baserom.gba\", 0x000fd048, 0x000002ac\n",
            ),
            (
                "games/tbs/sound/samples.s",
                b".incbin \"sound/sample/wave_00.pcm8.wav\"\n",
            ),
            (
                "games/tbs/sound/samples.s",
                b".incbin \"sound/../../../roms/tbs-en.bin\"\n",
            ),
            (
                "games/tbs/sound/samples.s",
                b".incbin \"sound/sample/wave_00.pcm8.bin\", 0, 16\n",
            ),
            (
                "games/tbs/sound/samples.s",
                b".incbin \"overlays/resource_36f.lz\"\n",
            ),
        ] {
            assert!(super::incbin(path, line), "{path}");
        }
        assert_eq!(
            super::publication_data_reason("games/tbs/sound/samples.s", sample, None),
            None
        );
    }

    #[test]
    fn asset_sources_may_incbin_only_the_graphics_files_the_build_makes() {
        let sheet = b"BattleFx_Star:\n\t.incbin \"graphics/fx/star.gbapal\"\n\t.incbin \"graphics/fx/star.bitmap.lz\"\n";
        let tiles =
            b"\t.incbin \"graphics/fx/star.4bpp.mtf\"\n\t.incbin \"graphics/fx/star.8bpp\"\n\t.incbin \"graphics/fx/star.4bpp32x16.lz\"\n\t.incbin \"map/m/metatiles.delta1.lz\"\n\t.incbin \"map/m/animation.script.lz\"\n\t.incbin \"map/m/path.table.lz\"\n\t.incbin \"graphics/fx/font.glyphs\"\n\t.incbin \"graphics/ui/icons/icons.icons4\"\n\t.incbin \"map/world/blocks.blocks\"\n";
        for path in [
            "games/tbs/src/graphics/fx/star.s",
            "games/tla/src/battle/effect/star.s",
        ] {
            assert!(!super::incbin(path, sheet), "{path}");
            assert!(!super::incbin(path, tiles), "{path}");
        }
        let star = "games/tbs/src/graphics/fx/star.s";
        for (path, line) in [
            ("recon/tbs/unidentified.s", sheet.as_slice()),
            ("games/tbs/sound/star.s", sheet),
            (star, b".incbin \"graphics/fx/star.png\"\n"),
            (star, b".incbin \"graphics/fx/star.raw\"\n"),
            (star, b".incbin \"graphics/fx/star.4bpp.zip\"\n"),
            (star, b".incbin \"map/m/cells.delta3.lz\"\n"),
            (star, b".incbin \"graphics/ui/icons.icons5\"\n"),
            (star, b".incbin \"map/world/blocks.block\"\n"),
            (star, b".incbin \"text/m/cells.bin\"\n"),
            (star, b".incbin \"map/m/end.bin\"\n"),
            (star, b".incbin \"graphics/fx/STAR.4bpp\"\n"),
            (star, b".incbin \"graphics/../../roms/tbs-en.4bpp\"\n"),
            (star, b".incbin \"graphics/fx/star.4bpp\", 0, 16\n"),
            (star, b".incbin \"baserom.gba\", 0x003cd090, 0x00000488\n"),
        ] {
            assert!(super::incbin(path, line), "{path}");
        }
        assert_eq!(super::publication_data_reason(star, sheet, None), None);
    }

    #[test]
    fn any_module_may_incbin_the_tables_the_build_makes() {
        let items = "games/tbs/src/battle/item/items.s";
        for line in [
            ".incbin \"battle/item/items.table\"\n",
            ".incbin \"game/class/classes.table.lz\"\n",
            ".incbin \"common/menu/icons/arrow.4bpp\"\n",
        ] {
            assert!(!super::incbin(items, line.as_bytes()), "{line}");
            assert!(super::incbin("games/tbs/sound/items.s", line.as_bytes()), "{line}");
        }
        for line in [
            ".incbin \"battle/x.bin\"\n",
            ".incbin \"battle/item/items.tsv\"\n",
            ".incbin \"BATTLE/item/items.table\"\n",
            ".incbin \"sound/sample/x.table\"\n",
            ".incbin \"battle/../../roms/tbs-en.table\"\n",
        ] {
            assert!(super::incbin(items, line.as_bytes()), "{line}");
        }
    }

    #[test]
    fn generated_fonts_allow_only_constant_character_layout_ranges() {
        let path = "games/common/src/graphics/font/text.s";
        for line in [
            ".incbin \"graphics/font/text_ja.font\", 0, (0x100 - 0x20) * (2 + 12 * 2)\n",
            ".incbin \"graphics/font/text_ja.font\", (0x100 - 0x20) * (2 + 12 * 2)\n",
        ] {
            assert!(!super::incbin(path, line.as_bytes()), "{line}");
            assert!(
                super::incbin("recon/tbs/ja/rom.s", line.as_bytes()),
                "{line}"
            );
        }
        for line in [
            ".incbin \"baserom.gba\", 0, (0x100 - 0x20) * 26\n",
            ".incbin \"graphics/font/text_ja.raw\", 0, 26\n",
            ".incbin \"graphics/font/text_ja.font.lz\", 0, 26\n",
            ".incbin \"graphics/font/text_ja.font\", ExpectedOffset, 26\n",
            ".incbin \"graphics/font/text_ja.font\", 0, LOADADDR(.font)\n",
            ".incbin \"graphics/font/text_ja.font\", 0, 2 +\n",
            ".incbin \"graphics/font/text_ja.font\", 0, (2 * 26\n",
            ".incbin \"graphics/font/text_ja.font\", -1, 26\n",
        ] {
            assert!(super::incbin(path, line.as_bytes()), "{line}");
        }
    }

    #[test]
    fn raw_bin_inputs_are_refused_by_path_in_game_and_shared_sources() {
        for path in [
            "games/tbs/src/x.bin",
            "games/tla/src/x.bin",
            "games/common/src/m/x.bin",
            "games/tbs/src/x.bin",
        ] {
            assert_eq!(
                publication_path_reason(path),
                Some("private or generated file type"),
                "{path}"
            );
            assert!(native_path_reason(path).is_some(), "{path}");
        }
    }

    #[test]
    fn linker_names_come_from_bytes_sections_or_symbol_differences() {
        for path in [
            "games/tbs/main.ld",
            "games/tla/constants.ld",
        ] {
            for source in [
                "Alias = Defined;\n",
                "Place = 0x08000100;\n",
                "Count = 8;\n",
                "SECTIONS { .text : { Alias = Defined; } }\n",
                "Length = End - Start; Alias = Defined;\n",
                "PROVIDE(Alias = Defined);\n",
                "Alias = Defined + ADDR(.text) * 0;\n",
                "Alias = ADDR(.text) ? Defined : Other;\n",
                "Alias = Defined /* ADDR(.text) */;\n",
            ] {
                assert_eq!(
                    linker_assignment_reason(path, source),
                    Some(LINKER_ASSIGNMENT_REASON),
                    "{path}: {source}"
                );
                assert_eq!(
                    publication_data_reason(path, source.as_bytes(), None),
                    Some(LINKER_ASSIGNMENT_REASON)
                );
            }
            for source in [
                "MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\n",
                "Length = End - Start;\n",
                "Length = ABSOLUTE(End - Start);\n",
                "Length = . - Start;\n",
                "Start = ADDR(.text);\n",
                "Start = LOADADDR(.text) + 1;\n",
                "Entry = LOADADDR(.runtime) + (Routine - ADDR(.runtime));\n",
                ". = ALIGN(4);\n",
                "/* Place = 0x08000100; */\n",
            ] {
                assert!(
                    linker_assignment_reason(path, source).is_none(),
                    "{path}: {source}"
                );
            }
        }
        assert!(linker_assignment_reason("tools/linker.ld", "Alias = Defined;").is_none());
    }

    #[test]
    fn raw_calls_and_literal_pools_use_labels_without_rejecting_instruction_encodings() {
        let path = "recon/tla/raw/routine.s";
        for source in [
            "bl 0x08000101\n",
            "blx 50331648\n",
            "blne 0x03000100\n",
            "local: bl (0x08000000 + 0x101)\n",
            "blxeq #(0x03000000 | 1)\n",
            "ldr r0, =0x02000000\nldr r1, [r0]\n",
            "LDR R0, .L_POOL\nLDR R1, [R0]\nBX LR\n.L_POOL:\n.WORD 0x03000100\n",
            "ldr r0, .L_pool\nldr r1, [r0]\nbx lr\n.L_pool:\n.4byte 0x03000100\n",
            "ldr r0, .L_pool\nbx r0\n.L_pool:\n.word 0x08000101\n",
            "ldr r0, .L_pool\nmov r5, r0\nmovs r0, #0\nstr r1, [r5]\nbx lr\n.L_pool:\n.4byte 0x02000100\n",
            ".4byte 0xe59f0004\n.4byte 0xe5901000\n.4byte 0xe12fff1e\n.4byte 0x02004778\n",
            ".4byte 0x03000100\n.4byte 0xe51f000c\n.4byte 0xe5901000\n.4byte 0xe12fff1e\n",
            ".4byte 0xe59f0004\n.4byte 0xe1a05000\n.4byte 0xe5951000\n.4byte 0x02004778\n",
            ".word 0xe59f0010, 0xe3510000, 0x1a000000, 0xe3a00000, 0xe5901000, 0xe12fff1e, 0x02000100\n",
            ".word 0xe59f0004, 0x03a00000, 0xe5901000, 0x02000100\n",
        ] {
            assert_eq!(
                raw_address_reason(path, source),
                Some(RAW_ADDRESS_REASON),
                "{source}"
            );
            assert_eq!(
                publication_data_reason(path, source.as_bytes(), None),
                Some(RAW_ADDRESS_REASON)
            );
        }
        for source in [
            "bl Routine\n",
            "bl Routine + 4\n",
            "ble .L_again\n",
            "ldr r0, .L_pool\nbx lr\n.L_pool:\n.4byte Buffer + 4\n",
            "ldr r0, .L_pool\nbx lr\n.L_pool:\n.4byte 0x04000100\n",
            "ldr r0, .L_pool\nbx lr\n.L_pool:\n.4byte 0x03010000\n",
            ".4byte 0xe59f0000\n.4byte 0xe12fff1e\n.4byte Buffer\n",
            ".4byte 0x03a00001\n.4byte 0x0a000044\n",
            ".4byte 0xe59f0000\n.4byte 0xe12fff1e\n.4byte 0x00004778\n",
            "@ bl 0x08000100\n/* .4byte 0x02000000 */\n",
            ".ascii \"bl 0x08000100\"\n",
            "ldr r0, .L_pool\nmovs r0, #0\nldr r1, [r0]\n.L_pool:\n.4byte 0x02000100\n",
            "ldr r0, .L_pool\nbl Motion_CamBounds\nbx lr\n.L_pool:\n.4byte 0x08040000\n",
            "ldr r0, .L_pool\nbl CallRoutine\nldr r1, [r0]\n.L_pool:\n.4byte 0x02000100\n",
            "ldr r0, .L_pool\nldr r1, [r0]\nbx lr\n.L_pool:\n.4byte 4\n.section .rodata\n.4byte 0x0200001a\n",
            ".4byte 0xe59f0004\n.4byte 0xe1a01000\n.4byte 0xeb000001\n.4byte 0x08040000\n",
            ".4byte 0xe59f0004\n.4byte 0xe3a00000\n.4byte 0xe5901000\n.4byte 0x02000100\n",
            ".word 0xe59f0010, 0xea000001, 0xe3a00000, 0xe5901000, 0xe12fff1e, 0xe12fff1e, 0x02000100\n",
        ] {
            assert!(raw_address_reason(path, source).is_none(), "{source}");
        }
        assert!(
            raw_address_reason("games/tla/src/routine.s", "bl 0x08000100\n").is_none()
        );
        assert_eq!(
            raw_address_reason(
                "games/tbs/src/system/overlay/ovl_3a7.s",
                "ldr r0, =0x02000000\nldr r1, [r0]\n"
            ),
            Some(RAW_ADDRESS_REASON)
        );
    }

    #[test]
    fn message_archives_are_built_assembly_included_by_a_tracked_source() {
        let root = crate::compiler::routing::root();
        for game in ["tbs", "tla"] {
            let path = format!("games/{game}/text/messages.s");
            let data = std::fs::read(root.join(&path)).unwrap();
            assert_eq!(publication_reason(&path, &data, None), None, "{path}");
            assert!(String::from_utf8_lossy(&data).contains("\t.include \"text/messages.inc\"\n"));
        }
        assert!(super::incbin(
            "games/tbs/text/messages.s",
            b".incbin \"text/messages.bin\"\n"
        ));
        let named = b"msgctxt \"MsgHpRecover\"\nmsgid \"02077\"\nmsgstr \"HP!\"\n";
        assert_eq!(
            publication_reason("games/tbs/text/en.po", named, None),
            None
        );
    }

    #[test]
    fn editable_sources_need_no_calculated_asset_catalog() {
        for (path, data) in [
            (
                "games/tbs/src/field/runpa/events.tsv",
                b"actor\tx\ty\tscript\nGuard\t4\t8\tOpenGate\n".to_vec(),
            ),
            (
                "games/tla/main.ld",
                b"MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\nSoundMixer = LOADADDR(.iwram_mixer) + 1;\n"
                    .to_vec(),
            ),
            (
                "games/tla/build.mk",
                b"GAME_OBJECTS := battle/start.o field/event.o\n".to_vec(),
            ),
            (
                "games/tla/include/link.h",
                b"extern int Data_081c0000;\n#define BufferAddress 0x02000000\n".to_vec(),
            ),
            (
                "games/tla/src/system/machine.s",
                b".syntax unified\n.thumb\n.global CallRoutine\nCallRoutine:\n bx r0\n"
                    .to_vec(),
            ),
            (
                "games/tbs/text/en.po",
                b"msgid \"000001\"\nmsgstr \"Open the gate.\"\n".to_vec(),
            ),
        ] {
            assert_eq!(publication_reason(path, &data, None), None, "{path}");
        }
    }

    #[test]
    fn calculated_ledgers_and_saved_answers_fail_after_renaming_or_splitting() {
        for data in [
            r#"{"decoded_sha256":"00"}"#,
            r#"[{"source":"field/event.c","address":"0x08000100","size":32}]"#,
            r#"{"codec":"golden-sun-kind2-lz","frames":[{"tokens":[2],"lookahead":"00"}]}"#,
        ] {
            for path in [
                "recon/tbs/part.json",
                "recon/tla/part.inc",
                "tools/renamed.dat",
            ] {
                assert_eq!(
                    publication_data_reason(path, data.as_bytes(), None),
                    Some(JSON_REASON),
                    "{path}"
                );
            }
        }
        assert_eq!(
            publication_data_reason("recon/tbs/part.tsv", b"source\tdecoded_sha256\n", None),
            Some(GENERATED_REASON)
        );
        for path in [
            "recon/tbs/private-inputs.json",
            "games/tbs/src/graphics/token.tokens",
            "games/tbs/src/graphics/char_common.png",
            "games/tla/src/data.bin",
        ] {
            assert!(publication_path_reason(path).is_some(), "{path}");
        }
    }

    #[test]
    fn maintained_assembly_is_source_while_serialized_listings_are_not() {
        let address = 0x0800_0100;
        let listing = format!(
            ".syntax unified\n.thumb\n.global Func_{address:08x}\nFunc_{address:08x}:\n bx lr\n"
        );
        let equated = listing.replace(
            ".thumb\n",
            &format!(".thumb\n.set sub_{address:08x}, 0x{address:08x}\n"),
        );
        for path in ["recon/tbs/raw/routine.s", "games/x/src/routine.s"] {
            assert_eq!(publication_reason(path, listing.as_bytes(), None), None);
            assert_eq!(
                publication_reason(path, equated.as_bytes(), None),
                Some(ADDRESS_EQUATE_REASON)
            );
        }
        let objdump = format!("{address:08x}: 4770 bx lr\n");
        assert_eq!(
            publication_data_reason("recon/tbs/raw/routine.s", objdump.as_bytes(), None),
            None
        );
        let serialized = format!("{{\"listing\": {equated:?}, \"instructions\": {objdump:?}}}");
        assert_eq!(
            publication_data_reason("recon/tbs/renamed.inc", serialized.as_bytes(), None),
            Some(JSON_REASON)
        );
        let words = (0..NUMERIC_ELEMENTS_MAX + 1)
            .map(|value| format!(".word 0x{value:08x}, 0x08000000, 0x08000100, 0x00000000\n"))
            .collect::<String>();
        assert_eq!(
            publication_reason("recon/tbs/raw/table.s", words.as_bytes(), None),
            None
        );
        let c = format!(
            "extern int Data_{address:08x};\nint Func_{address:08x}(void)\n{{\n return (int)&Data_{address:08x};\n}}\n"
        );
        assert_eq!(publication_data_reason("game.c", c.as_bytes(), None), None);
        let linker = format!("Data_{address:08x} = 0x{address:08x};\n");
        assert_eq!(
            publication_data_reason("link.ld", linker.as_bytes(), None),
            None
        );
        let library = format!(
            ".syntax unified\n.thumb\n.global MultiplyWords\nMultiplyWords:\n cmp r0, #0\n beq .L_{address:08x}\n bx lr\n.L_{address:08x}:\n bx lr\n"
        );
        assert_eq!(
            publication_data_reason("library.s", library.as_bytes(), None),
            None
        );
        let global_alias = format!(
            ".syntax unified\n.thumb\n.global Func_{address:08x}\nFunc_{address:08x}:\n bx lr\n"
        );
        assert_eq!(
            publication_data_reason("library.s", global_alias.as_bytes(), None),
            None
        );
    }

    #[test]
    fn generated_bookkeeping_is_rejected_in_staged_tree_and_outgoing_history() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "publication fixture").unwrap();
        let base = commit(
            root,
            &[("games/x/src/start.c", b"void Start(void) {}\n".to_vec())],
        );
        let path = root.join("recon/tbs/raw/renamed.s");
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(&path, "source\tpayload_sha256\n").unwrap();
        git(
            root,
            &["add", "recon/tbs/raw/renamed.s"],
            "publication fixture",
        )
        .unwrap();
        assert!(check_staged(root).unwrap_err().contains(GENERATED_REASON));
        let tip = commit(root, &[]);
        assert!(check_tree(root, Some(&tip))
            .unwrap_err()
            .contains(GENERATED_REASON));
        let update = format!("refs/heads/cleanup {tip} refs/heads/cleanup {base}\n");
        assert!(check_push(root, &update)
            .unwrap_err()
            .contains(GENERATED_REASON));
    }

    #[test]
    fn history_only_merges_scan_the_whole_tree() {
        for (file, permitted) in [("README.md", true), ("NOTES.json", false)] {
            let directory = tempfile::tempdir().unwrap();
            let root = directory.path();
            let run = |args: &[&str]| git(root, args, "merge fixture").unwrap();
            run(&["init", "-b", "main"]);
            run(&["config", "user.name", "Fixture"]);
            run(&["config", "user.email", "fixture@example.invalid"]);
            std::fs::write(root.join(file), "Project introduction.\n").unwrap();
            run(&["add", "--", file]);
            run(&["commit", "-m", "Initial tree"]);
            assert!(check_staged(root).unwrap_err().contains("no staged change"));
            run(&["checkout", "-b", "side"]);
            run(&["commit", "--allow-empty", "-m", "Separate history"]);
            run(&["checkout", "main"]);
            run(&["merge", "--no-ff", "--no-commit", "side"]);
            assert_eq!(check_staged(root).is_ok(), permitted);
        }
    }

    #[test]
    fn git_records_name_new_paths_and_flag_gitlinks() {
        let raw = b":100644 100644 a b M\0kept.c\0\
                    :100644 100644 a b R100\0README.md\0GUIDE.md\0\
                    :000000 160000 a b A\0vendor\0\
                    :160000 000000 a b D\0retired\0";
        let (_, changes) = raw_changes(raw).unwrap();
        assert_eq!(
            changes,
            [
                ("kept.c".to_string(), false),
                ("GUIDE.md".to_string(), false),
                ("vendor".to_string(), true)
            ]
        );
        assert!(inspected("staged", "agbcc".into(), ":agbcc".into(), true).is_none());
        let foreign = inspected("staged", "vendor".into(), ":vendor".into(), true).unwrap();
        assert!(foreign
            .listing_reason
            .unwrap()
            .contains("unapproved gitlink"));
    }
    #[test]
    fn push_without_ref_updates_is_a_noop_but_malformed_updates_fail() {
        let root = tempfile::tempdir().unwrap();
        assert!(check_push(root.path(), "").is_ok());
        assert!(check_push(root.path(), "\n  \n").is_ok());
        assert_eq!(
            check_push(root.path(), "invalid").unwrap_err(),
            "invalid pre-push update"
        );
    }
    #[test]
    fn credited_assembly_is_written_as_instructions() {
        let tbs = "games/tbs/src/sound/hook.s";
        let refused = [
            ".syntax unified\n\t.thumb\nHook:\n\t.inst.n 0x4718\n",
            "\t.arm\n\tmov r0, r0\n\t.inst 0xe1a0a00a\n\tbx lr\n",
            "\t.thumb\n\tmovs r0, #1\n\t.2byte 0x4770\n",
            "\t.arm\n\tldr r0, [pc, #4]\n\t.4byte 0xe12fff1e\n",
            "\t.thumb\n\tbl Other\n.L_pool:\n\t.4byte 0x03000658\n",
            "\t.arm\n\tbxeq lr\n\t.word 0xe1a00000\n",
            "\t.thumb\n\t.global Entry\n\t.thumb_func\nEntry:\n\t.2byte 0xb500\n\tbl Other\n",
            "\t.type Arm, %function\nArm:\n\t.4byte 0xe92d4000\n\tbl Other\n",
            "\tmovs r0, r1 ; .hword 0x46c0\n",
            "\tadds r0, #1\n\t.fill 1, 2, 0x4770\n",
        ];
        for text in refused {
            assert_eq!(
                raw_encoding_reason(tbs, text),
                Some(RAW_ENCODING_REASON),
                "{text:?}"
            );
            assert!(raw_encoding_reason("games/tla/src/x.inc", text).is_some());
        }
        let accepted = [
            "\t.thumb\n\tpop {r4, pc}\n\t.align 2, 0\n.L_pool:\n\t.4byte Table\n\t.4byte 0x04000200\n",
            "\t.thumb\n\tbx lr\n\t.4byte 0x03007ff0\nNext:\n\tpush {lr}\n",
            "\t.arm\n\tb .L_end\n\t.4byte 0x03001c90\n\tldr r2, [pc, #-12]\n",
            "\t.arm\n\tadd pc, pc, r4\n\t.2byte 0xfee0, 0x0020\n\tsub r2, r2, #2\n",
            "\t.arm\n\tldr pc, [pc, r0, lsl #2]\n\t.4byte .L_a, .L_b\n",
            "\t.arm\n\tldmfd sp!, {r4, pc}\n\t.4byte 1\n",
            "\t.thumb\n\tmov pc, r0\n\t.2byte 0\n",
            "\t.thumb\n\tbx pc\n\t.2byte 0x0200\n\t.arm\n\tpush {r4, lr}\n",
            "\t.arm\n\tmul r9, r7, ip\nAlchemyUncredited_08000f98:\n\t.4byte 0xfedcba98\nAlchemyUncreditedEnd_08000f98:\n\tmlane r0, fp, r9, r0\n",
            "\toverlay_veneer Target\n\t.4byte 0\n",
            "\t.thumb\n\tmovs r0, #1\n\t.section .rodata\n\t.4byte 0x12345678\n",
            "\t.thumb\n\tmovs r0, #1 @ .inst.n 0x4770\n\tbx lr /* .2byte 0 */\n",
            "Table:\n\t.2byte 1, 2, 3\n\t.4byte Other\n",
            ".macro veneer target\n\tldr r4, [pc, #0]\n\tbx r4\n\t.4byte \\target\n.endm\n",
        ];
        for text in accepted {
            assert!(raw_encoding_reason(tbs, text).is_none(), "{text:?}");
        }
        let padding = [
            "\t.arm\n\tmul r9, r7, ip\nAlchemyUncredited_08000f98:\n\t.4byte 0xfedcba98\nAlchemyUncreditedEnd_08000f98:\n\tmlane r0, fp, r9, r0\n",
            "\tbx lr\nAlchemyUncredited_080f9a96:\n\t.2byte 0\nAlchemyUncreditedEnd_080f9a96:\nNext:\n",
            "\tldr pc, [pc, ip, lsl #2]\nAlchemyUncredited_080f0100:\n\tnop\nAlchemyUncreditedEnd_080f0100:\n",
            "\tadd r1, pc, #4\n\tbx r1\nAlchemyUncredited_080f967e:\n\t.arm\nAlchemyUncreditedEnd_080f967e:\n",
        ];
        for text in padding {
            assert!(uncredited_code_reason(tbs, text).is_none(), "{text:?}");
        }
        let hidden = "\tbx lr\nAlchemyUncredited_08001000:\n\tmovs r0, #1\n\tbx lr\nAlchemyUncreditedEnd_08001000:\n";
        assert_eq!(uncredited_code_reason(tbs, hidden), Some(UNCREDITED_CODE_REASON));
        let spelled = "\t.thumb\n\tmovs r0, #1\n\t.inst.n 0x4770\n";
        assert!(raw_encoding_reason("recon/tbs/raw/080006fc.s", spelled).is_none());
        assert!(raw_encoding_reason("games/tbs/src/x.c", spelled).is_none());
        assert_eq!(
            publication_data_reason(tbs, spelled.as_bytes(), None),
            Some(RAW_ENCODING_REASON)
        );
    }
    #[test]
    fn edition_scaffolds_hold_no_equates() {
        let scaffold = "\t.section .rom.000158dc, \"ax\"\n\t.global Tile_BuildMetatiles\nFunc_080158e8:\n\t.incbin \"baserom.gba\", 0x000158dc, 0x000001f4\n";
        for path in [
            "recon/tbs/ja/rom.s",
            "recon/tla/de/rom.s",
            "recon/tbs/de/sym_iwram.s",
        ] {
            assert!(edition_equate_reason(path, scaffold).is_none(), "{path}");
            for line in [
                "\t.set Object_UpdateAllCodeSize, 0x4e8\n",
                ".equ Size, End - Start\n",
                "\t.equiv\tLength,0x214\n",
                "  .eqv Count, 3\n",
                "Tile_BuildMetatilesCodeSize = 0x1f4\n",
                "\tSize=End-Start\n",
            ] {
                let text = format!("{scaffold}\t.global X\n{line}");
                assert_eq!(
                    edition_equate_reason(path, &text),
                    Some(EDITION_EQUATE_REASON),
                    "{path}: {line:?}"
                );
                assert_eq!(
                    publication_data_reason(path, text.as_bytes(), None),
                    Some(EDITION_EQUATE_REASON)
                );
            }
        }
        let accepted = "@ .set Size, 0x4e8 once stood here\n\tldr r0, =Label\n";
        assert!(edition_equate_reason("recon/tbs/ja/rom.s", accepted).is_none());
        let measured = "\t.set Tile_BuildMetatilesCodeSize, . - Tile_BuildMetatiles\n";
        for path in [
            "recon/tbs/raw/080158e8.s",
            "recon/tbs/unidentified.s",
            "games/tbs/src/x.s",
            "recon/tbs/ja/main.ld",
        ] {
            assert!(edition_equate_reason(path, measured).is_none(), "{path}");
        }
    }
    #[test]
    fn full_address_equates_are_refused_in_game_and_reconstruction_assembly() {
        let tbs = "games/tbs/src/battle/effect/tables.s";
        let listing = "recon/tbs/raw/overlays/resource_3a7_overlay.s";
        let refused = [
            "\t.set\t.L_case_2, 0x080f0144\n",
            ".set .L_080f9b10__080f9ac2, 0x080f9ac2\n",
            "  .equ gBuffer, 0x03001B10 @ restated\n",
            "\t.equiv\tRecordEnd,0x080006fc",
            "\t.eqv Data_02000100, 0x02000100\n",
        ];
        for line in refused {
            assert_eq!(
                address_equate_reason(tbs, &format!(".syntax unified\n{line}")),
                Some(ADDRESS_EQUATE_REASON),
                "{line:?}"
            );
            assert!(address_equate_reason(listing, line).is_some(), "{line:?}");
            assert!(address_equate_reason("games/tla/src/x.inc", line).is_some());
        }
        let accepted = [
            "\t.set FIELD_OVERLAY_LOAD_BIAS, 0x8000\n",
            "\t.set FOREVER, 0xffff\n",
            "\t.set Size, . - Start\n",
            "\t.set .Ldivisor, .Ldivisor + 1\n",
            "\t.4byte 0x080f0144\n",
            "@ .set .L_case_2, 0x080f0144 was a restated address\n",
            "\t.set Wide, 0x080f01440\n",
        ];
        for line in accepted {
            assert!(address_equate_reason(tbs, line).is_none(), "{line:?}");
        }
        let restated = "\t.set .L_case_2, 0x080f0144\n";
        assert!(address_equate_reason("tools/alchemy/src/x.rs", restated).is_none());
        assert!(address_equate_reason("docs/notes.s", restated).is_none());
        assert!(address_equate_reason("games/tbs/src/x.c", restated).is_none());
        assert_eq!(
            publication_data_reason(tbs, restated.as_bytes(), None),
            Some(ADDRESS_EQUATE_REASON)
        );
    }
    #[test]
    fn later_parts_share_the_first_parts_built_palette() {
        let temp = tempfile::tempdir().unwrap();
        let directory = temp.path().join("games/x/src/graphics");
        std::fs::create_dir_all(&directory).unwrap();
        let second = directory.join("w_2.png").to_string_lossy().into_owned();
        std::fs::write(directory.join("w.tsv"), "W_1\t4bpp\nW_2\t4bpp16x32\n").unwrap();
        assert!(!palette_built(&second));
        std::fs::write(directory.join("w.s"), "\t.incbin \"graphics/w_1.gbapal\"\n").unwrap();
        assert!(palette_built(&second));
        assert!(palette_built(&directory.join("w_1.png").to_string_lossy()));
    }

    #[test]
    fn a_part_list_that_starts_with_the_palette_writes_it() {
        let temp = tempfile::tempdir().unwrap();
        let directory = temp.path().join("games/x/src/graphics");
        std::fs::create_dir_all(&directory).unwrap();
        let sheet = directory.join("t.png").to_string_lossy().into_owned();
        let other = directory.join("u.png").to_string_lossy().into_owned();
        std::fs::write(directory.join("t.tsv"), "T\tgbapal\nT\t4bpp8x16\n").unwrap();
        assert!(!palette_built(&sheet));
        std::fs::write(directory.join("t.s"), "\t.incbin \"graphics/t.4bpp.lz\"\n").unwrap();
        assert!(!palette_built(&sheet));
        std::fs::write(directory.join("t.s"), "\t.incbin \"graphics/t.parts.lz\"\n").unwrap();
        assert!(palette_built(&sheet));
        assert!(!palette_built(&other));
        std::fs::write(directory.join("t.tsv"), "T\t4bpp\nT\tgbapal\n").unwrap();
        assert!(!palette_built(&sheet));
    }

    #[test]
    fn a_grey_palette_passes_only_when_the_build_writes_it() {
        let temp = tempfile::tempdir().unwrap();
        let directory = temp.path().join("games/x/src/graphics");
        std::fs::create_dir_all(&directory).unwrap();
        let png = directory.join("logo.png");
        let path = png.to_string_lossy().into_owned();
        assert!(!palette_built(&path));
        std::fs::write(
            directory.join("other.s"),
            "\t.incbin \"graphics/other.gbapal\"\n",
        )
        .unwrap();
        assert!(!palette_built(&path));
        std::fs::write(
            directory.join("logo.s"),
            "\t.incbin \"graphics/logo.gbapal\"\n",
        )
        .unwrap();
        assert!(palette_built(&path));
    }
    #[test]
    fn encoded_measures_whole_texts_and_spares_identifiers_and_digests() {
        let base64: Vec<u8> = (b'A'..=b'Z')
            .chain(b'a'..=b'z')
            .chain(b'0'..=b'9')
            .chain(*b"+/")
            .collect();
        let short = (0..10)
            .map(|seed| format!("{}\n", encoded_fixture(&base64, 48, seed)))
            .collect::<String>();
        assert!(encoded_reason(&short, true).is_some());
        assert!(encoded_reason(&short[..49], true).is_none());
        for name in [
            "SelectActor25SceneVariant",
            "RunOverlayObjectCommand14",
            "SetWorkspaceHalfword382To1018",
            "0xfffffffffffffff3",
            "0x0FFFFFFFFFFFFFFFu",
        ] {
            assert!(
                matches!(classify(name.as_bytes()), Run::Plain | Run::Digest),
                "{name}"
            );
        }
        let hex: Vec<u8> = (b'0'..=b'9').chain(b'a'..=b'f').collect();
        let base32: Vec<u8> = (b'A'..=b'Z').chain(b'2'..=b'7').collect();
        for (alphabet, length) in [(&hex, 20), (&hex, 48), (&base32, 16), (&base32, 32)] {
            let run = encoded_fixture(alphabet, length, 11);
            assert!(matches!(classify(run.as_bytes()), Run::Encoded), "{run}");
        }
        let digest = encoded_fixture(&hex, 64, 12);
        assert!(matches!(classify(digest.as_bytes()), Run::Digest));
        assert!(matches!(classify("1".repeat(64).as_bytes()), Run::Plain));
        for token in ["0x1f", "-12", "255u8", "0xffULL", "x7f", "de", "0b1010"] {
            assert!(numeric(token.as_bytes()), "{token}");
        }
        for token in ["u8", "i32", "0x", "r7", "1.5", "face"] {
            assert!(!numeric(token.as_bytes()), "{token}");
        }
        let fifteen = (0..15)
            .map(|index| index.to_string())
            .collect::<Vec<_>>()
            .join(", ");
        assert_eq!(numeric_elements(fifteen.as_bytes()), 0);
        assert_eq!(numeric_elements(format!("[{fifteen}, 15]").as_bytes()), 16);
    }
    #[test]
    fn logo_hidden_in_filtered_pixels_is_found_after_decoding() {
        let logo = logo_fixture();
        let (_, png, _) = binary_fixtures()
            .into_iter()
            .find(|(path, ..)| path.ends_with("/logo.indexed.png"))
            .unwrap();
        assert!(!contains(&png, &logo));
        assert!(contains(&indexed_png_bytes(&png).unwrap(), &logo));
        let path = "games/tbs/src/graphics/tile/logo.indexed.png";
        assert_eq!(publication_data_reason(path, &png, None), None);
        assert_eq!(
            publication_data_reason(path, &png, Some(&logo)),
            Some(LOGO_REASON)
        );
    }
    #[test]
    fn exact_inflation_refuses_surplus_and_trailing_streams() {
        let data = fixture_bytes(300, 1);
        let stream = fdeflate::compress_to_vec(&data);
        assert_eq!(inflate_exact(&stream, 300).unwrap(), data);
        assert!(inflate_exact(&stream, 299).is_none());
        assert!(inflate_exact(&stream, 301).is_none());
        for trailing in [1, 4, 8, 64] {
            let padded = [stream.as_slice(), &vec![0; trailing]].concat();
            assert!(inflate(&padded, 300).is_some());
            assert!(inflate_exact(&padded, 300).is_none(), "{trailing}");
        }
        assert!(inflate_exact(&stream[..stream.len() - 1], 300).is_none());
    }
    fn commit(root: &Path, files: &[(&str, Vec<u8>)]) -> String {
        for (path, data) in files {
            let file = root.join(path);
            std::fs::create_dir_all(file.parent().unwrap()).unwrap();
            std::fs::write(file, data).unwrap();
        }
        let run = |args: &[&str]| git(root, args, "fixture git").unwrap();
        run(&["add", "--all"]);
        run(&[
            "-c",
            "user.name=fixture",
            "-c",
            "user.email=fixture@example.invalid",
            "commit",
            "--quiet",
            "--message",
            "fixture",
        ]);
        String::from_utf8(run(&["rev-parse", "HEAD"]))
            .unwrap()
            .trim()
            .to_string()
    }

    #[test]
    fn advertised_remote_tip_is_excluded_even_without_a_remote_tracking_ref() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let published = commit(root, &[("README.md", b"Project\n".to_vec())]);
        let tip = commit(
            root,
            &[("games/x/src/start.c", b"void Start(void) {}\n".to_vec())],
        );
        assert_eq!(revisions(root, &tip, &published).unwrap(), [tip]);
    }

    #[test]
    fn push_refuses_forbidden_outgoing_versions_even_when_the_tip_deleted_them() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let published = commit(root, &[("README.md", b"Project\n".to_vec())]);
        git(
            root,
            &["update-ref", "refs/remotes/origin/main", &published],
            "fixture remote",
        )
        .unwrap();
        commit(root, &[("private.DUMP", b"fixture\n".to_vec())]);
        git(root, &["rm", "private.DUMP"], "fixture git").unwrap();
        let tip = commit(root, &[]);
        assert!(check_tree(root, Some(&tip)).is_ok());
        let update = format!(
            "refs/heads/topic {tip} refs/heads/topic {}\n",
            "0".repeat(40)
        );
        assert!(check_push(root, &update)
            .unwrap_err()
            .contains("private.DUMP"));
    }

    #[test]
    fn push_still_scans_the_whole_tip_when_no_commit_is_outgoing() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let published = commit(root, &[("private.DUMP", b"fixture\n".to_vec())]);
        git(
            root,
            &["update-ref", "refs/remotes/origin/main", &published],
            "fixture remote",
        )
        .unwrap();
        let zero = "0".repeat(40);
        assert!(revisions(root, &published, &zero).unwrap().is_empty());
        let update = format!("refs/heads/topic {published} refs/heads/topic {zero}\n");
        assert!(check_push(root, &update)
            .unwrap_err()
            .contains("private.DUMP"));
    }

    #[test]
    fn push_still_refuses_outgoing_commit_messages_with_byte_dumps() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let published = commit(root, &[("README.md", b"Project\n".to_vec())]);
        git(
            root,
            &["update-ref", "refs/remotes/origin/main", &published],
            "fixture remote",
        )
        .unwrap();
        let bytes = (0..32)
            .map(|value| format!("{value:02x} "))
            .collect::<String>();
        git(
            root,
            &[
                "-c",
                "user.name=fixture",
                "-c",
                "user.email=fixture@example.invalid",
                "commit",
                "--quiet",
                "--allow-empty",
                "--message",
                &format!("Fixture\n\n{bytes}"),
            ],
            "fixture message",
        )
        .unwrap();
        let tip =
            String::from_utf8(git(root, &["rev-parse", "HEAD"], "fixture tip").unwrap()).unwrap();
        let update = format!(
            "refs/heads/topic {} refs/heads/topic {}\n",
            tip.trim(),
            "0".repeat(40)
        );
        assert!(check_push(root, &update)
            .unwrap_err()
            .contains("raw byte dump"));
    }
    #[test]
    fn tree_mode_passes_pret_style_inputs_and_reports_only_presentation_material() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let midi = midi_fixture(&[], true);
        let inputs = commit(
            root,
            &[
                ("games/x/src/graphics/tile/a.4bpp.png", indexed_fixture(4)),
                ("games/x/sound/sequence/a.mid", midi.clone()),
                ("games/x/src/main.c", b"void main(void) {}\n".to_vec()),
                ("games/y/src/main.c", b"void main(void) {}\n".to_vec()),
                ("tools/Cargo.lock", b"checksum = \"00ff\"\n".to_vec()),
            ],
        );
        assert!(check_tree(root, None).is_ok());
        assert!(check_tree(root, Some(&inputs)).is_ok());
        let gif = commit(
            root,
            &[(
                "games/x/preview/idle.gif",
                b"GIF89a\x01\0\x01\0\0\0\0".to_vec(),
            )],
        );
        let error = check_tree(root, Some(&gif)).unwrap_err();
        assert_eq!(error.lines().count(), 2, "{error}");
        assert!(error.contains("games/x/preview/idle.gif: presentation material"));
        assert!(check_tree(root, None).unwrap_err().contains("idle.gif"));
        let zero = "0".repeat(40);
        let update = format!("refs/heads/main {gif} refs/heads/main {inputs}\n");
        let error = check_push(root, &update).unwrap_err();
        assert!(
            error.contains("idle.gif") && !error.contains("a.4bpp.png"),
            "{error}"
        );
        let update = format!("refs/heads/main {inputs} refs/heads/main {zero}\n");
        assert!(check_push(root, &update).is_ok());
        let native_and_misplaced = commit(
            root,
            &[
                ("games/y/sound/sequence/a.mid", midi),
                ("games/y/data/table.json", b"{}\n".to_vec()),
            ],
        );
        let error = check_tree(root, Some(&native_and_misplaced)).unwrap_err();
        assert!(!error.contains("games/y/sound/sequence/a.mid:"));
        assert!(error.contains("games/y/data/table.json: JSON is banned"));
        assert!(!error.contains("games/x/sound"), "{error}");
    }
}

/// The most edits one stream may list: Camelot's post-pack edits are single
/// literals (W3), never a way to carry a stream's bytes.
const PATCH_LITERALS: usize = 8;

/// A post-pack patch (W3) must parse, list few literals, prove each one with
/// a broad set of stand-ins, and mark every other row as a copy of a listed
/// literal.
fn patch_reason(path: &str, data: &[u8]) -> Option<&'static str> {
    if !path.ends_with("_patch.tsv") {
        return None;
    }
    let Ok(patch) = ags::patch::Patch::parse(path, data) else {
        return Some("patch file does not parse as position, pack, ship, proof");
    };
    if patch.literals() > PATCH_LITERALS {
        return Some("patch file lists more edited literals than a post-pack edit");
    }
    let literals: Vec<usize> = patch
        .edits
        .iter()
        .filter(|edit| !edit.proof.starts_with("copy of"))
        .map(|edit| edit.position)
        .collect();
    for edit in &patch.edits {
        if let Some(origin) = edit.proof.strip_prefix("copy of ") {
            if !origin
                .trim()
                .parse::<usize>()
                .is_ok_and(|origin| literals.contains(&origin))
            {
                return Some("patch copy row names no listed literal");
            }
            continue;
        }
        let broad = edit
            .proof
            .strip_prefix("literal: ")
            .and_then(|rest| rest.split_once(" of 255 stand-ins reproduce the stream"))
            .and_then(|(count, _)| count.parse::<usize>().ok())
            .is_some_and(|count| count >= 128);
        if !broad {
            return Some("patch literal lacks its proof: most stand-ins must reproduce the stream");
        }
    }
    None
}

#[cfg(test)]
mod patch_tests {
    use super::patch_reason;

    #[test]
    fn a_proven_patch_passes_and_a_byte_carrier_does_not() {
        let good = "position\tpack\tship\tproof\n\
            10\t0x02\t0x01\tliteral: 254 of 255 stand-ins reproduce the stream; this is the lowest\n\
            12\t0x02\t0x01\tcopy of 10\n";
        assert_eq!(patch_reason("map/x/grid_patch.tsv", good.as_bytes()), None);
        let narrow = "position\tpack\tship\tproof\n\
            10\t0x02\t0x01\tliteral: 3 of 255 stand-ins reproduce the stream\n";
        assert!(patch_reason("map/x/grid_patch.tsv", narrow.as_bytes()).is_some());
        let orphan = "position\tpack\tship\tproof\n\
            10\t0x02\t0x01\tliteral: 254 of 255 stand-ins reproduce the stream\n\
            12\t0x02\t0x01\tcopy of 11\n";
        assert!(patch_reason("map/x/grid_patch.tsv", orphan.as_bytes()).is_some());
        let mut many = String::from("position\tpack\tship\tproof\n");
        for position in 0..9 {
            many.push_str(&format!(
                "{position}\t0x02\t0x01\tliteral: 254 of 255 stand-ins reproduce the stream\n"
            ));
        }
        assert!(patch_reason("map/x/grid_patch.tsv", many.as_bytes()).is_some());
    }
}
