//! Fail-closed publication checks for staged changes, outgoing history and
//! whole tracked trees. Only what pret would commit passes: editable build
//! inputs, source and tooling, never presentation material made from the game.
use psynergy::assets::image::{indexed_bitmap_png, PNG_SIGNATURE};
use psynergy::assets::midi::{midi_events, EventBody, MidiEvent};
use psynergy::assets::wav::wav_pcm8;
use std::collections::{BTreeMap, BTreeSet};
use std::io::{BufRead, BufReader, Read, Write};
use std::path::Path;
use std::process::{Command, ExitCode, Stdio};
const BLOCKED_EXTENSIONS: &[&str] = &[
    "a", "bin", "bps", "bsdiff", "d", "diff", "dis", "dll", "dmp", "dump", "dylib", "elf", "exe",
    "gba", "gz", "ips", "lst", "log", "map", "o", "patch", "raw", "rom", "sav", "so", "sym", "tar",
    "tgz", "unidiff", "ups", "xdelta", "xdelta3", "zip", "7z",
];
/// Fonts, rasters, audio and video no build consumes.
pub(crate) const PRESENTATION_EXTENSIONS: &[&str] = &[
    "otf", "ttf", "ttc", "woff", "woff2", "eot", "fnt", "bdf", "pcf", "gif", "jpg", "jpeg", "webp",
    "bmp", "ico", "avif", "apng", "tif", "tiff", "mp3", "ogg", "flac", "m4a", "aac", "mp4", "webm",
    "mov", "svg",
];
const BACKUP_EXTENSIONS: &[&str] = &["bak", "orig", "rej", "swp"];
/// The only two owned prose documents, including ignored output.
const DOCUMENT_EXTENSIONS: &[&str] = &[
    "adoc", "asciidoc", "markdown", "md", "mdown", "mkdn", "mdx", "rdoc", "rest", "rst", "text",
    "txt",
];
const OWNED_DOCUMENTS: &[&str] = &["README.md", "AGENTS.md"];
/// Native editable inputs; formats are validated independently of file names.
const NATIVE_INPUT_EXTENSIONS: &[&str] = &[
    "c", "h", "inc", "s", "ld", "mk", "gitkeep", "png", "wav", "mid", "pcm4", "po", "tsv",
];
/// Tooling metadata areas the former layout kept under `games/<game>/`; every
/// other directory there is an asset root. Reconstruction metadata now lives
/// under `recon/<id>/`, outside the game tree; these names still classify
/// outgoing history written before that move.
const METADATA_DIRECTORIES: &[&str] = &["metrics", "preview", "recon", "semantic"];
/// Structured tables the asset build reads, where long numeric arrays are data.
const DATA_TABLE_EXTENSIONS: &[&str] = &["tsv"];
/// The licensed compiler submodules whose commits `make compiler-source-check` pins.
const APPROVED_GITLINKS: &[&str] = &["agbcc", "agscc"];
const FONT_TABLES: &[&[u8]] = &[
    b"BASE", b"CFF ", b"COLR", b"CPAL", b"DSIG", b"EBDT", b"EBLC", b"FFTM", b"GDEF", b"GPOS",
    b"GSUB", b"LTSH", b"OS/2", b"STAT", b"SVG ", b"VDMX", b"cmap", b"cvt ", b"fpgm", b"gasp",
    b"glyf", b"hdmx", b"head", b"hhea", b"kern", b"loca", b"maxp", b"name", b"post", b"prep",
];
/// Standard ancillary PNG chunks and their largest specified bodies. Text,
/// profile, EXIF and private chunks carry arbitrary bytes and are refused.
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
/// Alphanumeric runs this long are measured as possible encodings.
const ENCODED_RUN_MIN: usize = 16;
/// Encoded characters one text may hold; the tracked tree peaks near 32.
const ENCODED_CHARACTERS_MAX: usize = 128;
/// Unexplained digest-sized hex runs one text may hold.
const DIGEST_RUNS_MAX: usize = 2_048;
/// Consecutive integer literals that form an array rather than an expression.
const NUMERIC_RUN_MIN: usize = 16;
/// Array elements a text outside the game data tables may hold.
const NUMERIC_ELEMENTS_MAX: usize = 2_048;
/// JSON is banned from the repository, in every file and every form.
const JSON_REASON: &str = "JSON is banned: write a TSV table, plain text or a source form";
const INTEGER_SUFFIXES: &[&str] = &[
    "usize", "isize", "u128", "i128", "u64", "i64", "u32", "i32", "u16", "i16", "u8", "i8", "ull",
    "llu", "ul", "lu", "ll", "u", "l",
];
/// Where the cartridge logo sits in a GBA header.
const LOGO: std::ops::Range<usize> = 0x04..0xa0;
const LOGO_REASON: &str = "Nintendo logo from a GBA cartridge header: ROM material";
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
const MARKER_EXTENSIONS: &[&str] = &["md", "ts", "js", "sh", "c", "h", "s", "asm", "tsv", "txt"];
/// JSON by its content, whatever the file is called: a text that is one
/// bracketed document.
fn json_text(text: &str) -> bool {
    let text = text.trim();
    (text.starts_with('{') && text.ends_with('}')) || (text.starts_with('[') && text.ends_with(']'))
}
/// A JSON file by its name, in any case: `.json`, `.jsonl` or `.json5`.
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
    if directories
        .iter()
        .any(|directory| listed(directory, TOOLCHAIN_DIRECTORIES))
    {
        return Some("compiler or runtime-library source: keep it in its licensed repository");
    }
    if listed(suffix, BLOCKED_EXTENSIONS) {
        return Some("private or generated file type");
    }
    if listed(suffix, PRESENTATION_EXTENSIONS) {
        return Some("presentation material: pret commits only editable build inputs");
    }
    if listed(suffix, BACKUP_EXTENSIONS) || leaf.ends_with('~') {
        return Some("editor or merge backup");
    }
    // A rename or copy lands here too: every document path is judged as new.
    if listed(suffix, DOCUMENT_EXTENSIONS) && !OWNED_DOCUMENTS.contains(&normalized.as_str()) {
        return Some(
            "separate document: working knowledge belongs in AGENTS.md; README.md is public",
        );
    }
    if directories
        .iter()
        .any(|directory| directory.eq_ignore_ascii_case("preview"))
    {
        return Some(
            "PREVIEW material belongs under ignored out/; the README figures are root PROGRESS_CHART.png and PROGRESS.png",
        );
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
/// The cartridge logo, read at run time from the local verified ROM; the gate
/// source never carries it. Without the ROM the logo scan is skipped.
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
    // A cartridge header behind its entry branch is a ROM fragment at any size.
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
fn marker_line(line: &str) -> bool {
    let bytes = line.as_bytes();
    bytes.get(7) == Some(&b' ')
        && (bytes[..7].iter().all(|byte| *byte == b'<')
            || bytes[..7].iter().all(|byte| *byte == b'>'))
}
fn conflict_marker_reason(path: &str, data: &[u8]) -> Option<String> {
    if !listed(extension(path), MARKER_EXTENSIONS) {
        return None;
    }
    let text = String::from_utf8_lossy(data);
    if !text.split(['\n', '\r']).any(marker_line) {
        return None;
    }
    let line = text
        .split('\n')
        .position(marker_line)
        .map(|index| index + 1)
        .unwrap_or(0);
    Some(format!(
        "unresolved conflict marker at line {line}; resolve the merge before committing"
    ))
}
/// Preserve licensed dependencies only in their established source locations.
/// A nested Git checkout elsewhere must not hide project notes from the gate.
fn upstream_documents(root: &Path, path: &Path) -> bool {
    if APPROVED_GITLINKS.iter().any(|name| path == root.join(name)) {
        return true;
    }
    if !path.starts_with(root.join("tools/out/compiler-build")) {
        return false;
    }
    path.join("gcc/toplev.c").is_file()
        || ["binutils-2.10", "binutils-2.33.1"].iter().any(|name| {
            path == root.join("tools/out/compiler-build/sources").join(name)
                && path.join("configure").is_file()
                && path.join("gas").is_dir()
                && path.join("bfd").is_dir()
        })
}
fn check_documents(root: &Path) -> Result<(), String> {
    let mut pending = vec![root.to_path_buf()];
    let mut rejected = Vec::new();
    while let Some(directory) = pending.pop() {
        for entry in std::fs::read_dir(&directory).map_err(|e| e.to_string())? {
            let entry = entry.map_err(|e| e.to_string())?;
            let path = entry.path();
            let kind = entry.file_type().map_err(|e| e.to_string())?;
            if kind.is_dir() {
                if entry.file_name() != ".git" && !upstream_documents(root, &path) {
                    pending.push(path);
                }
                continue;
            }
            let relative = path
                .strip_prefix(root)
                .map_err(|e| e.to_string())?
                .to_string_lossy();
            if !listed(extension(&relative), DOCUMENT_EXTENSIONS)
                || (matches!(relative.as_ref(), "README.md" | "AGENTS.md") && kind.is_file())
            {
                continue;
            }
            rejected.push(relative.into_owned());
        }
    }
    rejected.sort();
    if rejected.is_empty() {
        Ok(())
    } else {
        Err(format!(
            "separate documentation is forbidden, including ignored files:\n{}",
            rejected.join("\n")
        ))
    }
}
/// An `.incbin` directive, except four forms that commit no bytes. In
/// scaffolding: pret's base-ROM range `.incbin "baserom.gba", OFFSET, SIZE`,
/// which reads the builder's own ROM as pokeemerald's early data files did,
/// and a code overlay the build links from its listing and compresses,
/// `.incbin "overlays/resource_XXX.lz"`. In a game's sound data sources: a
/// file the build makes from the like-named editable input beside them,
/// `.incbin "SOUND/SAMPLE/WAVE_00.PCM8.bin"` from `WAVE_00.PCM8.WAV`, as
/// pret's data files read the `.bin` files wav2agb makes. In a game's asset
/// sources under `SRC`: a file the build makes from the like-named indexed
/// PNG or TSV table, named by its recipe,
/// `.incbin "GRAPHICS/FX/STAR.bitmap.lz"` from `SRC/GRAPHICS/FX/STAR.PNG` or
/// `.incbin "MAP/M/METATILES.delta1.lz"` from `SRC/MAP/M/METATILES.TSV`, as pret's
/// data files read the `.4bpp.lz` files gbagfx makes.
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
        r#"^\s*\.incbin\s+"(?:COMMON/)?SOUND(?:/[A-Z0-9_]+)+(?:\.[A-Z0-9]+)?\.bin"\s*$"#,
    )
    .expect("built sound pattern");
    // A graphics or map file the build makes: an uppercase path whose name
    // is a recipe the ags encoder builds, so the encoder alone decides which
    // forms and codecs exist.
    let built_graphics = regex::Regex::new(
        r#"^\s*\.incbin\s+"((?:COMMON/)?(?:GRAPHICS|MAP)(?:/[A-Z0-9_]+)+\.[a-z0-9.]+)"\s*$"#,
    )
    .expect("built graphics pattern");
    // A font's source-owned character split labels its glyph and kanji groups
    // inside one editable PNG recipe. Only constant layout arithmetic may
    // slice an uncompressed generated font; ROMs and raw assets remain refused.
    let built_font = regex::Regex::new(
        r#"^\s*\.incbin\s+"((?:COMMON/)?GRAPHICS(?:/[A-Z0-9_]+)+\.font)"\s*,\s*([^,]+?)(?:\s*,\s*([^,]+?))?\s*$"#,
    ).expect("built font range pattern");
    let is_built_graphics = |line: &str| {
        built_graphics
            .captures(line)
            .is_some_and(|capture| ags::resource::is_recipe(&capture[1]))
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
        ["games", game, "SOUND", .., _] if *game != "COMMON"
    );
    let asset_source = matches!(parts.as_slice(), ["games", _, "SRC", .., _]);
    let text = String::from_utf8_lossy(data);
    text.split(['\n', '\r'])
        .filter(|line| !(scaffolding && (base_rom.is_match(line) || overlay.is_match(line))))
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
/// A loose PNG sniff for presentation formats, whatever the file is named.
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
/// Signatures of presentation formats, whatever the file is named.
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
/// Every chunk from the PNG signature to the last byte, or `None` when a chunk
/// is truncated.
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
/// Inflate a zlib stream to exactly `size` bytes, checksum included.
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
/// Inflate one zlib stream that must end the input. The decoder may read past
/// a stream's end, so the stream proves it is exact by failing once its last
/// checksum byte is removed.
fn inflate_exact(stream: &[u8], size: usize) -> Option<Vec<u8>> {
    let output = inflate(stream, size)?;
    let (_, shorter) = stream.split_last()?;
    inflate(shorter, size).is_none().then_some(output)
}
/// The packed pixel bytes of a PNG that is exactly an indexed build input:
/// IHDR, PLTE, contiguous IDAT and a final IEND with only small standard
/// ancillary chunks, one zlib stream holding exactly its non-interlaced
/// scanlines, and pixels the asset build reads.
fn indexed_png_bytes(data: &[u8]) -> Option<Vec<u8>> {
    exact_indexed_stream(data)?;
    // A tile sheet or a linear bitmap: bitmaps may be any size.
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
/// An indexed PNG of any size as its packed palette indices, row by row.
fn packed_indices(data: &[u8]) -> Option<Vec<u8>> {
    let mut reader = png::Decoder::new(std::io::Cursor::new(data))
        .read_info()
        .ok()?;
    let mut pixels = vec![0; reader.output_buffer_size()];
    let frame = reader.next_frame(&mut pixels).ok()?;
    pixels.truncate(frame.buffer_size());
    Some(pixels)
}
/// A palette of more than ink and paper whose every colour is grey: a dump
/// drawn without the asset's real palette.
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
/// The chunk and stream half of `indexed_png_bytes`, for an indexed image of
/// any size: the README figures are not tile-aligned build inputs.
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
/// A standard MIDI file exactly as the sequence build reads it: MThd then only
/// MTrk chunks covering the file, every track closed by end-of-track, text
/// meta events as text and the rest at their specified sizes, and no
/// system-exclusive payloads. JSON is banned: a text event that opens an
/// object or array is refused.
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
/// A sequence's text names its own labels and events; where the song lands
/// and what its tone bank's address is belong to the build. A hexadecimal
/// literal of four or more digits, or a placement field, is refused.
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
/// Whether the build writes this PNG's palette into the ROM: a source beside
/// it includes its `.gbapal` recipe. Such a palette is proven by the
/// byte-for-byte build, so it may be grey (Pascal, 2026-09-29), as pret keeps
/// grey images whose palettes the game stores.
fn palette_built(path: &str) -> bool {
    own_palette_built(path) || part_palette_built(path)
}

/// Whether this PNG is a later part of a part list whose first part's palette
/// the build writes: the parts share that one palette (Pascal, 2026-09-30).
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
        let parts: Vec<&str> = text
            .lines()
            .filter_map(|line| line.split('\t').next())
            .filter(|part| !part.is_empty())
            .collect();
        match parts.split_first() {
            Some((first, rest)) if rest.contains(&stem) => {
                own_palette_built(&directory.join(format!("{first}.PNG")).to_string_lossy())
            }
            _ => false,
        }
    })
}

fn own_palette_built(path: &str) -> bool {
    let file = Path::new(path);
    let (Some(directory), Some(stem)) = (file.parent(), file.file_stem().and_then(|s| s.to_str()))
    else {
        return false;
    };
    let Some(source_root) = path.find("/SRC/").map(|at| &path[..at + 5]) else {
        return false;
    };
    let relative = directory
        .to_string_lossy()
        .strip_prefix(source_root)
        .map(|rest| format!("{rest}/{stem}.gbapal"))
        .unwrap_or_default();
    let Ok(entries) = std::fs::read_dir(directory) else {
        return false;
    };
    entries.flatten().any(|entry| {
        entry.path().extension().is_some_and(|ext| ext == "S")
            && std::fs::read_to_string(entry.path())
                .is_ok_and(|text| text.contains(&format!("\"{relative}")))
    })
}
/// The binary build inputs a game may track, each parsed exactly as the asset
/// build reads it; everything else is text.
fn binary_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    if data.is_empty() {
        return None;
    }
    // The README figures are generated, but held to the indexed build-input
    // standard: palette pixels and standard chunks, no text payloads. Only
    // their size is exempt from the tile grid; like any other PNG they carry a
    // real palette and never draw the cartridge logo.
    if matches!(path, "PROGRESS.png" | "PROGRESS_CHART.png") {
        if exact_indexed_stream(data).is_none() {
            return Some("README figure is not an exact indexed PNG");
        }
        if grey_sheet(data) {
            return Some(GREY_SHEET);
        }
        return logo
            .is_some_and(|logo| packed_indices(data).is_none_or(|pixels| contains(&pixels, logo)))
            .then_some(LOGO_REASON);
    }
    let components: Vec<_> = path.split('/').collect();
    let (area, rest) = match components.as_slice() {
        ["games", _, area, rest @ ..] if !rest.is_empty() => (*area, rest.join("/")),
        _ => return Some(UNREGISTERED),
    };
    match (area, extension(path).to_ascii_lowercase().as_str()) {
        ("SOUND", "pcm4") => {
            (!rest.starts_with("SAMPLE/") || data.len() > 64).then_some(UNREGISTERED)
        }
        ("SRC" | "TEXT", "png") => match indexed_png_bytes(data) {
            None => {
                Some("PNG is not an exact indexed build input: standard chunks, one exact stream")
            }
            Some(_) if grey_sheet(data) && !palette_built(path) => Some(GREY_SHEET),
            Some(pixels) => logo
                .is_some_and(|logo| contains(&pixels, logo))
                .then_some(LOGO_REASON),
        },
        ("SOUND", "wav") => wav_pcm8(data)
            .is_err()
            .then_some("WAV is not a canonical mono 8-bit PCM build input"),
        ("SOUND", "mid") => midi_reason(data),
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
/// Classify one alphanumeric run. Decimal runs are numbers; hashes and 64-bit
/// words are digests; other hex, upper-case base32 and mixed-case runs that do
/// not read as words are encodings. Base64 and base64url split into runs at
/// `+`, `/`, `-` and `_`.
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
/// Whether a mixed-case run reads as identifier words: once numbers are
/// removed, three fifths of its letters sit in words of three or more letters,
/// as in `SelectActor25SceneVariant` and not in random base64.
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
/// One integer literal: decimal, `0x` hex, `0b` binary, a `\x` escape or a
/// two-digit hex byte, with an optional Rust or C integer suffix.
fn numeric(token: &[u8]) -> bool {
    let token = match token {
        [b'-' | b'+', rest @ ..] => rest,
        _ => token,
    };
    if token.len() == 2 && token.iter().all(u8::is_ascii_hexdigit) {
        return true;
    }
    let body = INTEGER_SUFFIXES
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
        .unwrap_or(token);
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
/// Elements of numeric array literals: runs of integer tokens separated only
/// by whitespace, commas, brackets, quotes and escapes.
fn numeric_elements(text: &[u8]) -> usize {
    let separator = |byte: &u8| byte.is_ascii_whitespace() || b",;:()[]{}\\\"'".contains(byte);
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
/// Encoded content measured over a whole text, whatever its lines or quoting.
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

/// A digest or measured inventory remains generated bookkeeping after a rename
/// or a split into smaller documents. Reference checksums and dependency pins
/// are deliberate records; they do not carry these measured game fields.
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
/// `include_bytes!` of game, ROM or output bytes, and `include_str!` of ROM or
/// output text, in any delimiter, spacing, case or `concat!` split.
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
/// Filter attributes such as `filter=lfs` store content outside the scanned blob.
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
/// Word sequences that mark text under a license this repository cannot carry.
/// Each is stored split by `|` and joined at run time, so the gate's own
/// source never holds a phrase it rejects.
const LICENSE_PHRASES: &[&str] = &[
    "SP|DX Lic|ense Ident|ifier",
    "G|NU Gen|eral Pub|lic Lic|ense",
    "G|NU Les|ser Gen|eral Pub|lic Lic|ense",
    "G|NU Lib|rary Gen|eral Pub|lic Lic|ense",
];
const COPYRIGHT: &str = "Copy|right";
const FOUNDATION: &str = "Fr|ee Soft|ware Found|ation";
/// Words a copyright line may hold between `Copyright` and the holder: years,
/// `(C)` and separators.
const COPYRIGHT_WINDOW: usize = 32;
const LICENSE_REASON: &str = "license marker (SPDX identifier, GNU GPL or LGPL text, or an FSF copyright): this repository carries no license; compiler and runtime code lives in its licensed submodule";
const DIFF_REASON: &str = "patch or diff content: compiler changes live in their licensed submodules, and a diff is never a tracked input";
fn unsplit(value: &str) -> Vec<String> {
    value
        .replace('|', "")
        .split_whitespace()
        .map(str::to_ascii_lowercase)
        .collect()
}
fn holds_sequence(words: &[&str], phrase: &[String]) -> bool {
    !phrase.is_empty()
        && words
            .windows(phrase.len())
            .any(|window| window.iter().zip(phrase).all(|(word, part)| *word == part))
}
/// License identifiers, GNU license texts and Free Software Foundation
/// copyright lines, matched as word sequences so comment leaders, line breaks
/// and case do not hide them.
fn license_reason(text: &str) -> Option<&'static str> {
    let lower = text.to_ascii_lowercase();
    if !["license", "licence", "foundation"]
        .iter()
        .any(|word| lower.contains(word))
    {
        return None;
    }
    let words: Vec<&str> = lower
        .split(|ch: char| !ch.is_ascii_alphanumeric())
        .filter(|word| !word.is_empty())
        .collect();
    if LICENSE_PHRASES
        .iter()
        .any(|phrase| holds_sequence(&words, &unsplit(phrase)))
    {
        return Some(LICENSE_REASON);
    }
    let copyright = unsplit(COPYRIGHT).concat();
    let foundation = unsplit(FOUNDATION);
    let attributed = words.iter().enumerate().any(|(index, word)| {
        *word == copyright && {
            let end = (index + 1 + COPYRIGHT_WINDOW + foundation.len()).min(words.len());
            holds_sequence(&words[index + 1..end], &foundation)
        }
    });
    attributed.then_some(LICENSE_REASON)
}
/// `N` or `N,M`.
fn line_range(value: &str) -> bool {
    let digits = |part: &str| !part.is_empty() && part.bytes().all(|byte| byte.is_ascii_digit());
    match value.split_once(',') {
        Some((start, count)) => digits(start) && digits(count),
        None => digits(value),
    }
}
/// A unified hunk header, `@@ -A,B +C,D @@`, or a combined one with more `@`
/// and more old ranges.
fn unified_hunk(line: &str) -> bool {
    let fence = line.bytes().take_while(|byte| *byte == b'@').count();
    if fence < 2 {
        return false;
    }
    let Some((ranges, _)) = line[fence..].split_once(&format!(" {}", &line[..fence])) else {
        return false;
    };
    // A leading space, one old range per parent, then the new range.
    let Some(("", ranges)) = ranges.split_once(' ') else {
        return false;
    };
    let ranges: Vec<_> = ranges.split(' ').collect();
    let Some((new, old)) = ranges.split_last() else {
        return false;
    };
    old.len() + 1 == fence
        && old
            .iter()
            .all(|range| range.strip_prefix('-').is_some_and(line_range))
        && new.strip_prefix('+').is_some_and(line_range)
}
/// A context hunk's old-range line, `*** A,B ****`.
fn context_hunk(line: &str) -> bool {
    line.strip_prefix("*** ")
        .and_then(|rest| rest.strip_suffix(" ****"))
        .is_some_and(line_range)
}
/// Unified, combined, context and Git binary diff hunks, judged by structure
/// whatever the file is called.
fn diff_reason(text: &str) -> Option<&'static str> {
    let lines: Vec<&str> = text
        .split('\n')
        .map(|line| line.strip_suffix('\r').unwrap_or(line))
        .collect();
    let hunk = lines.windows(2).any(|pair| {
        let (line, next) = (pair[0], pair[1]);
        (unified_hunk(line) && next.starts_with([' ', '+', '-', '\\']))
            || (line == "*".repeat(15) && context_hunk(next))
            || (line == "GIT binary patch"
                && (next.starts_with("literal ") || next.starts_with("delta ")))
    });
    hunk.then_some(DIFF_REASON)
}
fn publication_data_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    if json_path(path) {
        return Some(JSON_REASON);
    }
    if logo.is_some_and(|logo| contains(data, logo)) {
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
    let table = (asset_game(path).is_some() && listed(extension(path), DATA_TABLE_EXTENSIONS))
        || (path.starts_with("recon/") && listed(extension(path), &["s", "inc"]));
    license_reason(text)
        .or_else(|| diff_reason(text))
        .or_else(|| data_uri_reason(text))
        .or_else(|| encoded_reason(text, !table))
        .or_else(|| calculated_bookkeeping_reason(text))
        .or_else(|| included_bytes_reason(path, text))
        .or_else(|| attributes_reason(path, text))
        .or_else(|| runtime_definition_reason(path, text))
        .or_else(|| address_equate_reason(path, text))
        .or_else(|| edition_equate_reason(path, text))
        .or_else(|| linker_assignment_reason(path, text))
        .or_else(|| raw_address_reason(path, text))
        .or_else(|| raw_encoding_reason(path, text))
}
/// Comments and quoted strings carry no linker or assembler statements.
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
/// Section addresses combined with sums and differences. The entire
/// expression must be arithmetic, so an unrelated ADDR cannot admit an alias.
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
fn raw_source_game(path: &str) -> Option<&str> {
    match path.split('/').collect::<Vec<_>>().as_slice() {
        ["recon", game @ ("tbs" | "tla"), "raw", .., leaf]
            if extension(leaf).eq_ignore_ascii_case("s") =>
        {
            Some(game)
        }
        _ => None,
    }
}
fn game_source(path: &str, game: &str) -> bool {
    let directory = if game == "tbs" {
        "THE BROKEN SEAL"
    } else {
        "THE LOST AGE"
    };
    path.starts_with("games/COMMON/") || path.starts_with(&format!("games/{directory}/"))
}
/// Type evidence is read from the tree being inspected, including push deltas.
/// A declaration identifies pointer use; it never provides a numeric address.
fn raw_pointer_types(
    sources: &[(String, String)],
    game: &str,
) -> psynergy::assembly::addresses::Types {
    use crate::permute::lex::lex;
    use crate::permute::parse::{parameters, Parser};
    let mut text = sources
        .iter()
        .filter(|(path, _)| game_source(path, game) && listed(extension(path), &["c", "h"]))
        .map(|(path, text)| (path, text))
        .collect::<Vec<_>>();
    text.sort_by_key(|(path, _)| (!path.ends_with("/TYPES.H"), !path.ends_with(".H"), *path));
    let mut names = BTreeSet::new();
    let mut units = Vec::new();
    for _ in 0..2 {
        units.clear();
        for (_, text) in &text {
            let Ok(tokens) = lex(text) else {
                continue;
            };
            let mut parser = Parser::new(tokens, &mut names);
            parser.scan_unit();
            units.push(parser.unit);
        }
    }
    let mut pointer_types = BTreeSet::new();
    loop {
        let before = pointer_types.len();
        for unit in &units {
            for (name, (specs, decl)) in &unit.typedefs {
                if decl.before.iter().any(|token| token == "*")
                    || specs.iter().any(|spec| pointer_types.contains(spec))
                {
                    pointer_types.insert(name.clone());
                }
            }
        }
        if pointer_types.len() == before {
            break;
        }
    }
    let mut types = psynergy::assembly::addresses::Types::default();
    for unit in units {
        for (name, (specs, decl)) in unit.globals {
            if specs.iter().any(|spec| spec == "static") {
                continue;
            }
            if decl.after.first().is_some_and(|token| token == "(") {
                let text = decl.after.join(" ");
                let Ok(tokens) = lex(&text) else {
                    continue;
                };
                let Some(end) = crate::permute::parse::matching(&tokens, 0) else {
                    continue;
                };
                let Ok(parameters) = parameters(&tokens[1..end], &mut names.clone()) else {
                    continue;
                };
                // Aggregates and wide scalar arguments have a different ABI
                // register layout; their raw bodies provide the use evidence.
                if parameters.iter().any(|decl| {
                    decl.specs
                        .iter()
                        .filter(|spec| spec.as_str() == "long")
                        .count()
                        > 1
                        || decl
                            .specs
                            .iter()
                            .any(|spec| matches!(spec.as_str(), "double" | "s64" | "u64"))
                        || (decl
                            .specs
                            .iter()
                            .any(|spec| matches!(spec.as_str(), "struct" | "union"))
                            && !decl
                                .items
                                .iter()
                                .any(|item| item.before.iter().any(|token| token == "*")))
                }) {
                    continue;
                }
                let mut mask = 0;
                for (argument, decl) in parameters.into_iter().take(4).enumerate() {
                    if decl.specs.iter().any(|spec| pointer_types.contains(spec))
                        || decl.items.iter().any(|item| {
                            item.before.iter().any(|token| token == "*")
                                || item.after.first().is_some_and(|token| token == "[")
                        })
                    {
                        mask |= 1 << argument;
                    }
                }
                *types
                    .arguments
                    .entry(name.to_ascii_lowercase())
                    .or_default() |= mask;
            } else if decl.after.iter().any(|token| token == "[")
                && (decl.before.iter().any(|token| token == "*")
                    || specs.iter().any(|spec| pointer_types.contains(spec)))
            {
                types.tables.insert(name.to_ascii_lowercase());
            } else if decl.before.iter().any(|token| token == "*")
                || specs.iter().any(|spec| pointer_types.contains(spec))
            {
                types.objects.insert(name.to_ascii_lowercase());
            }
        }
    }
    types
}
fn raw_context_findings(
    sources: &[(String, String)],
) -> BTreeMap<String, Vec<psynergy::assembly::addresses::Site>> {
    let mut findings = BTreeMap::<String, Vec<_>>::new();
    for game in ["tbs", "tla"] {
        let types = raw_pointer_types(sources, game);
        let main = sources
            .iter()
            .enumerate()
            .filter(|(_, (path, _))| {
                raw_source_game(path) == Some(game) && !path.contains("/overlays/")
            })
            .map(|(index, _)| index)
            .collect::<Vec<_>>();
        let overlays = sources
            .iter()
            .enumerate()
            .filter(|(_, (path, _))| {
                raw_source_game(path) == Some(game) && path.contains("/overlays/")
            })
            .map(|(index, _)| index)
            .collect::<Vec<_>>();
        // Overlay labels are local to their own linked image. Equal numeric
        // placeholder names in other resources never supply call evidence.
        for indices in std::iter::once(main).chain(overlays.into_iter().map(|index| vec![index])) {
            let code = indices
                .iter()
                .map(|&index| unquoted_source(&sources[index].1).to_ascii_lowercase())
                .collect::<Vec<_>>();
            let source = code.iter().map(String::as_str).collect::<Vec<_>>();
            for site in psynergy::assembly::addresses::sites(&source, &types) {
                let path = sources[indices[site.source]].0.clone();
                findings.entry(path).or_default().push(site);
            }
        }
    }
    findings
}
fn numeric_word(text: &str) -> Option<u32> {
    psynergy::assembly::addresses::integer(text)
}
fn memory_address(word: u32) -> bool {
    // GBA address-space regions, including the Game Pak ROM's bus mirrors.
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
/// A loaded register is an address only when this local instruction stream
/// dereferences or calls it before overwriting it. Pointer-only arguments and
/// standalone tables need semantic review; coordinates can look like addresses.
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
/// The same local pointer-use proof for ARM instructions that recon still
/// spells as words. Unknown encodings end the proof instead of guessing.
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
    if !matches!(parts.as_slice(), ["recon", _, "raw", .., leaf] if extension(leaf).eq_ignore_ascii_case("s"))
    {
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
    if call.is_match(&code) || !psynergy::assembly::addresses::numeric_calls(&code).is_empty() {
        return Some(RAW_ADDRESS_REASON);
    }
    if !psynergy::assembly::addresses::sites(
        &[&code],
        &psynergy::assembly::addresses::Types::default(),
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
            // An ARM word load from pc: its target is pc (+8), plus or
            // minus the 12-bit byte offset. Only its literal is an address.
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
/// Any equate or symbol assignment in an edition's scaffold,
/// `recon/<game>/<lang>/*.s`: `.set`, `.equ`, `.equiv`, `.eqv` or `name =`.
/// A length or address its code reads would come from the reference ROM;
/// the scaffold places labels at bytes and the linker works out the rest.
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
/// An assembler equate that gives a name a whole 32-bit address, such as
/// `.set .L_x, 0x080f0144`, in game or reconstruction source. Names are
/// labels at their bytes; restating an address beside them is a stored
/// answer, whichever equate directive spells it.
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
/// One statement of an assembly source, as the raw-encoding gate reads it.
#[derive(Clone, Copy, PartialEq, Eq)]
enum Statement {
    /// A label; `true` for a function entry (`.thumb_func` or `.type ..., %function`).
    Label(bool),
    Instruction {
        ends_flow: bool,
    },
    /// `.inst`, which always spells an instruction by its encoding.
    Encoding,
    /// A data directive; `true` inside an AlchemyUncredited_* span.
    Data(bool),
    /// A directive that switches section: control never runs across it.
    Section,
    Directive,
    /// A macro invocation, whose expansion this gate does not read.
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
/// Whether an unconditional instruction leaves straight-line flow: a branch,
/// a return, or any write to pc.
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
/// The statements of an assembly source, comments, strings and preprocessor
/// lines removed, in order.
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
                },
                _ => Statement::Macro,
            };
            statements.push(statement);
        }
    }
    statements
}
/// Credited assembly under `games/` is written as instructions, never as
/// copied encodings (AGENTS.md, oracle leakage invariant 3). Refused: any
/// `.inst`, and credited data that control reaches as code, because it follows
/// an instruction that continues or opens a function before its first
/// instruction. Literal pools and tables after a branch or return are data,
/// as are the words of an AlchemyUncredited_* span, which claims no credit.
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
        if let Some(Statement::Instruction { ends_flow: false }) = before {
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
/// The game whose asset roots hold `path`: every directory under
/// `games/<game>/` except tooling metadata, with `asm/overlays` holding
/// overlay streams, all matched without regard to case. Local reconstruction
/// outputs under `recon/` are never native asset inputs.
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
/// The shared root holds only what every game builds byte-exact from the
/// same text: nested C source, interface headers, asset sources with the
/// PNG and TSV inputs they are built from, and sequences, as MIDI or
/// assembly, under SOUND/SEQUENCE and samples, WAV or PCM4, under
/// SOUND/SAMPLE, as in each game.
fn shared_root_reason(path: &str) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, root, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("games") || !root.eq_ignore_ascii_case("COMMON") {
        return None;
    }
    let source = matches!(rest, ["SRC", _, .., leaf]
        if listed(extension(leaf), &["C", "S", "PNG", "TSV"]));
    let interface = matches!(rest, ["INCLUDE", _, .., leaf] if extension(leaf) == "H");
    let sequence = matches!(rest, ["SOUND", "SEQUENCE", leaf]
        if listed(extension(leaf), &["MID", "S"]));
    let sample = matches!(rest, ["SOUND", "SAMPLE", leaf]
        if listed(extension(leaf), &["WAV", "PCM4"]));
    (!(source || interface || sequence || sample)).then_some(
        "games/COMMON holds only shared SRC/<module>/ sources and inputs, INCLUDE/<module>/*.H \
         and SOUND/ sequences and samples",
    )
}
const RECON_REASON: &str = "recon/<game> holds only raw disassembly and its linker scripts, the top-level assembly scaffolding, an edition's assembly scaffold and MAIN.LD, C drafts under an edition and metrics/history.tsv";
/// `recon/` fails closed as `games/` does: pret's scaffolding forms and the
/// published progress history, nothing else. No JSON or TSV ledger, however
/// it is named, may come back beside the scaffolding.
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
            ["tbs", "metrics", "history.tsv"] => true,
            // An edition's own scaffold and the linker script that places it.
            [g, e, "MAIN.LD"] => game(g) && edition(e),
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
        && ((matches!(*area, "SRC" | "INCLUDE" | "SOUND" | "TEXT")
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
/// Parse `git diff --raw -z` into `(path, gitlink)` records. Renames and copies
/// name their new path, which every path rule judges as new.
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
    revision: Option<String>,
}
/// One inspected path. Approved compiler submodules carry no blob to read;
/// any other gitlink fails without being read.
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
        revision: None,
    })
}
/// Stream blobs through one `git cat-file --batch` process in request order.
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
fn scan(root: &Path, entries: Vec<Entry>, conflicts: bool) -> Result<(), String> {
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
        let reason =
            publication_data_reason(&entry.path, data, logo.as_deref()).map(str::to_string);
        let reason = reason.or_else(|| {
            conflicts
                .then(|| conflict_marker_reason(&entry.path, data))
                .flatten()
        });
        if let Some(reason) = reason {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
        }
    })?;
    let contexts = readable
        .iter()
        .filter(|entry| {
            raw_source_game(&entry.path).is_some()
                || (entry.path.starts_with("games/") && listed(extension(&entry.path), &["c", "h"]))
        })
        .map(|entry| entry.revision.clone())
        .collect::<BTreeSet<_>>();
    for revision in contexts {
        let scope = revision
            .as_deref()
            .map_or("tree", |revision| &revision[..12.min(revision.len())]);
        for (path, sites) in raw_tree_findings(root, revision.as_deref())? {
            for site in sites {
                failures.push(format!(
                    "{scope} {path}:{}: {RAW_ADDRESS_REASON} (0x{:08x})",
                    site.line, site.value
                ));
            }
        }
    }
    if failures.is_empty() {
        Ok(())
    } else {
        Err(format!(
            "publication gate rejected:\n{}",
            failures.join("\n")
        ))
    }
}
/// Tracked `(gitlink, object, path)` records of the index or of a revision.
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
        .filter_map(|(gitlink, object, path)| {
            let mut entry = inspected(&scope, path, object, gitlink)?;
            entry.revision = revision.map(str::to_string);
            Some(entry)
        })
        .collect())
}
fn raw_tree_findings(
    root: &Path,
    revision: Option<&str>,
) -> Result<BTreeMap<String, Vec<psynergy::assembly::addresses::Site>>, String> {
    let sources = tracked(root, revision)?
        .into_iter()
        .filter(|(gitlink, _, path)| {
            !gitlink
                && (raw_source_game(path).is_some()
                    || path.starts_with("games/") && listed(extension(path), &["c", "h"]))
        })
        .map(|(_, object, path)| (object, path))
        .collect::<Vec<_>>();
    let mut text = Vec::new();
    blobs(
        root,
        sources.iter().map(|(object, _)| object.clone()).collect(),
        |index, data| {
            if let Ok(source) = std::str::from_utf8(data) {
                text.push((sources[index].1.clone(), source.to_string()));
            }
        },
    )?;
    Ok(raw_context_findings(&text))
}
fn check_tree(root: &Path, revision: Option<&str>) -> Result<(), String> {
    let entries = tree_entries(root, revision)?;
    if entries.is_empty() {
        return Err("publication gate scanned nothing: the tree is empty".to_string());
    }
    scan(root, entries, false)
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
        // A normal merge may join histories whose file changes already landed.
        // There is still a publishable tree to inspect; scan it in full.
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
    scan(root, entries, true)
}
fn revisions(root: &Path, local: &str, remote: &str) -> Result<Vec<String>, String> {
    // A new branch still shares already-published history. The advertised
    // destination also excludes commits when local remote-tracking refs lag.
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
/// libgcc and soft-float routines a game links from its compiler runtime.
/// Their definitions are licensed runtime code this repository never holds;
/// calls, bindings and comments that name them are fine.
const RUNTIME_ROUTINES: &[&str] = &[
    "__divsi3",
    "__modsi3",
    "__udivsi3",
    "__umodsi3",
    "__divdi3",
    "__moddi3",
    "__udivdi3",
    "__umoddi3",
    "__muldi3",
    "__ashldi3",
    "__ashrdi3",
    "__lshrdi3",
    "__negdi2",
    "__cmpdi2",
    "__ucmpdi2",
    "__addsf3",
    "__subsf3",
    "__mulsf3",
    "__divsf3",
    "__negsf2",
    "__adddf3",
    "__subdf3",
    "__muldf3",
    "__divdf3",
    "__negdf2",
    "__eqsf2",
    "__nesf2",
    "__gtsf2",
    "__gesf2",
    "__ltsf2",
    "__lesf2",
    "__eqdf2",
    "__nedf2",
    "__gtdf2",
    "__gedf2",
    "__ltdf2",
    "__ledf2",
    "__cmpsf2",
    "__cmpdf2",
    "__fixsfsi",
    "__fixdfsi",
    "__fixunssfsi",
    "__fixunsdfsi",
    "__floatsisf",
    "__floatsidf",
    "__extendsfdf2",
    "__truncdfsf2",
    "__pack_f",
    "__unpack_f",
    "__pack_d",
    "__unpack_d",
    "_call_via_r0",
    "_call_via_r1",
    "_call_via_r2",
    "_call_via_r3",
    "_call_via_r4",
    "_call_via_r5",
    "_call_via_r6",
    "_call_via_r7",
    "_call_via_fp",
    "_call_via_ip",
    "_call_via_sl",
    "_call_via_sp",
    "_call_via_lr",
];
/// Directory names that hold compiler, assembler or runtime-library source.
const TOOLCHAIN_DIRECTORIES: &[&str] = &[
    "gcc",
    "libgcc",
    "binutils",
    "newlib",
    "libiberty",
    "bfd",
    "opcodes",
];
fn runtime_definitions() -> &'static [regex::Regex; 2] {
    static PATTERNS: std::sync::OnceLock<[regex::Regex; 2]> = std::sync::OnceLock::new();
    PATTERNS.get_or_init(|| {
        let names = RUNTIME_ROUTINES
            .iter()
            .map(|name| regex::escape(name))
            .collect::<Vec<_>>()
            .join("|");
        [
            // A C function header at the start of a line, not a prototype.
            regex::Regex::new(&format!(r"(?m)^(?:[A-Za-z_][\w \t*]*[ \t*])?_*(?:{names})[ \t]*\([^;]*$"))
                .expect("runtime C pattern"),
            // An assembly symbol, label or lib1funcs entry macro.
            regex::Regex::new(&format!(
                r"(?m)^[ \t]*(?:\.globl[ \t]+_*(?:{names})\b|\.global[ \t]+_*(?:{names})\b|_*(?:{names}):|(?:ARM_)?FUNC_START[ \t(]+_*(?:{names})\b)"
            ))
            .expect("runtime assembly pattern"),
        ]
    })
}
/// A definition of a compiler-runtime routine in C or assembly source.
fn runtime_definition_reason(path: &str, text: &str) -> Option<&'static str> {
    if !listed(extension(path), &["c", "h", "s", "asm", "inc"]) {
        return None;
    }
    let [c, assembly] = runtime_definitions();
    let defined = if listed(extension(path), &["c", "h"]) {
        c.is_match(text)
    } else {
        assembly.is_match(text)
            && !(only_veneers(text)
                && (!text.contains(".macro overlay_veneer")
                    || path.ends_with("SYSTEM/OVERLAY.INC")))
    };
    defined.then_some("compiler runtime routine: build it from its licensed container")
}
/// Whether every compiler-runtime name an assembly source defines labels
/// only a veneer: a stub that jumps to another, non-runtime routine and holds
/// no code of its own, as each overlay's import stub for the game's own
/// divider does. The stub macro itself may only be defined in OVERLAY.INC.
fn only_veneers(text: &str) -> bool {
    let [_, assembly] = runtime_definitions();
    let lines: Vec<&str> = text.lines().map(str::trim).collect();
    let runtime = |name: &str| {
        RUNTIME_ROUTINES
            .iter()
            .any(|routine| name.trim_start_matches('_') == routine.trim_start_matches('_'))
    };
    let veneer = |name: &str| {
        lines.iter().enumerate().any(|(index, line)| {
            line.strip_suffix(':') == Some(name)
                && lines[index + 1..]
                    .iter()
                    .find(|next| {
                        !next.is_empty()
                            && !next.starts_with(".thumb_func")
                            && !next.starts_with(".global")
                            && !next.starts_with(".globl")
                            && !next.ends_with(':')
                    })
                    .and_then(|next| next.strip_prefix("overlay_veneer "))
                    .map(str::trim)
                    .is_some_and(|target| !target.is_empty() && !runtime(target))
        })
    };
    assembly.find_iter(text).all(|found| {
        let definition = found.as_str().trim();
        let name = definition
            .strip_prefix(".globl")
            .or_else(|| definition.strip_prefix(".global"))
            .unwrap_or(definition)
            .trim()
            .trim_end_matches(':');
        !name.contains("FUNC_START") && veneer(name)
    })
}
/// A commit message may describe work, never carry what may not be tracked.
fn history_message_reason(message: &str) -> Option<&'static str> {
    commit_message_reason(message)
        .or_else(|| license_reason(message))
        .or_else(|| diff_reason(message))
        .or_else(|| runtime_definition_reason("message.c", message))
        .or_else(|| runtime_definition_reason("message.s", message))
}
/// Scan every commit reachable from any ref, or from `revision`: each message, and each file
/// version any commit introduced, against today's publication rules. Writes
/// `out/history-audit.tsv` with every finding.
fn check_history(root: &Path, revision: Option<&str>) -> Result<(), String> {
    let listing = git(
        root,
        &["rev-list", revision.unwrap_or("--all")],
        "history revision scan",
    )?;
    let commits: Vec<String> = String::from_utf8_lossy(&listing)
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .map(str::to_string)
        .collect();
    let mut messages = Vec::new();
    let mut entries = Vec::new();
    let mut seen = BTreeSet::new();
    for commit in &commits {
        let message = git(
            root,
            &["show", "-s", "--format=%B", commit],
            "commit message",
        )?;
        if let Some(reason) = history_message_reason(&String::from_utf8_lossy(&message)) {
            messages.push(format!("message\t{commit}\t\t\t{reason}"));
        }
        let output = git(
            root,
            &[
                "diff-tree",
                "--root",
                "--no-commit-id",
                "--raw",
                "--no-abbrev",
                "-r",
                "-z",
                commit,
            ],
            "history path scan",
        )?;
        let fields = nul_list(&output);
        let mut index = 0;
        while index + 1 < fields.len() {
            let metadata: Vec<_> = fields[index].split_whitespace().collect();
            let path = fields[index + 1].clone();
            index += 2;
            if metadata.len() != 5 || metadata[4] == "D" {
                continue;
            }
            let object = metadata[3].to_string();
            if !seen.insert((path.clone(), object.clone())) {
                continue;
            }
            if let Some(entry) = inspected(commit, path, object, metadata[1] == "160000") {
                entries.push(entry);
            }
        }
    }
    let logo = nintendo_logo(root);
    let mut files = Vec::new();
    let mut readable = Vec::new();
    for entry in entries {
        match publication_path_reason(&entry.path).or(entry.listing_reason) {
            Some(reason) => files.push(format!(
                "file\t{}\t{}\t{}\t{reason}",
                entry.scope, entry.path, entry.object
            )),
            None => readable.push(entry),
        }
    }
    let objects = readable.iter().map(|entry| entry.object.clone()).collect();
    blobs(root, objects, |index, data| {
        let entry = &readable[index];
        let text = std::str::from_utf8(data).unwrap_or("");
        let reason = publication_data_reason(&entry.path, data, logo.as_deref())
            .or_else(|| runtime_definition_reason(&entry.path, text));
        if let Some(reason) = reason {
            files.push(format!(
                "file\t{}\t{}\t{}\t{reason}",
                entry.scope, entry.path, entry.object
            ));
        }
    })?;
    let mut report = format!(
        "# commits\t{}\n# file_versions\t{}\nkind\tcommit\tpath\tblob\treason\n",
        commits.len(),
        seen.len()
    );
    for line in files.iter().chain(&messages) {
        report += line;
        report.push('\n');
    }
    let path = root.join("out/history-audit.tsv");
    std::fs::create_dir_all(root.join("out")).map_err(|error| error.to_string())?;
    std::fs::write(&path, report).map_err(|error| error.to_string())?;
    println!(
        "history-audit commits={} file_versions={} files={} messages={} report=out/history-audit.tsv",
        commits.len(),
        seen.len(),
        files.len(),
        messages.len()
    );
    if files.is_empty() && messages.is_empty() {
        Ok(())
    } else {
        Err("history holds material that may not be published".to_string())
    }
}
fn check_push(root: &Path, updates: &str) -> Result<(), String> {
    let updates: Vec<_> = updates
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .collect();
    if updates.is_empty() {
        // Git supplies no updates when the remote is already current. There
        // is no outgoing payload to inspect; staged changes are not pushed.
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
            let object = format!("{commit}:{path}");
            let mut entry = inspected(scope, path, object, gitlink)?;
            entry.revision = Some(commit.clone());
            Some(entry)
        }));
    }
    // Each pushed tip must also pass as a whole tree, not only as its deltas.
    for tip in tips {
        entries.extend(tree_entries(root, Some(&tip))?);
    }
    scan(root, entries, false)
}
/// Every file-level decision for one blob, as `scan` makes it.
fn publication_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    publication_path_reason(path)
        .or_else(|| native_path_reason(path))
        .or_else(|| publication_data_reason(path, data, logo))
}
/// Deterministic pseudo-random bytes. Fixtures are built at run time so the
/// tracked gate source never carries the payloads it rejects.
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
/// Unfiltered blank scanlines of a square image.
fn scanlines(size: usize, depth: usize) -> Vec<u8> {
    let row = (size * depth).div_ceil(8) + 1;
    vec![0; row * size]
}
/// A square PNG: IHDR, a full palette, `extra` chunks, one IDAT and IEND.
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
/// An indexed fixture drawn with a grey ramp in place of a real palette.
fn grey_fixture(depth: u8) -> Vec<u8> {
    let stream = fdeflate::compress_to_vec(&scanlines(16, usize::from(depth)));
    let header = [
        16u32.to_be_bytes().as_slice(),
        &16u32.to_be_bytes(),
        &[depth, 3, 0, 0, 0],
    ]
    .concat();
    let palette: Vec<u8> = (0..1usize << depth)
        .flat_map(|index| [(index * 255 >> depth) as u8; 3])
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
/// A one-track MIDI file of a note and, when `closed`, its end-of-track.
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
/// A deterministic stand-in for the cartridge logo; the real bytes stay in the ROM.
fn logo_fixture() -> Vec<u8> {
    fixture_bytes(LOGO.len(), 0x0bad_1060)
}
/// Toolchain-code fixtures, built at run time from split phrases and
/// substituted hunk markers so this source holds none of what it rejects:
/// `(license header, wrapped license text, lesser license text, FSF copyright,
/// unified diff, headerless hunk, context diff, binary patch)`.
fn toolchain_fixtures() -> [String; 8] {
    let phrase = |split: &str| split.replace('|', "");
    let general = phrase(LICENSE_PHRASES[1]);
    let words: Vec<&str> = general.split(' ').collect();
    let (at, minus, plus, star) = ('@', '-', '+', '*');
    [
        format!(
            "/*\n * {}: GPL-2.0-or-later WITH GCC-exception-2.0\n */\nint body;\n",
            phrase(LICENSE_PHRASES[0]).replace(' ', "-")
        ),
        format!(
            "@ under the terms of the {} {} {}\n@ {} as published by the author\n",
            words[0], words[1], words[2], words[3]
        ),
        format!("; see the {}, version 2.1\n", phrase(LICENSE_PHRASES[2])),
        format!(
            "/* {} (C) 1995, 1996, 1998, 1999, 2000 {}, Inc. */\n",
            phrase(COPYRIGHT),
            phrase(FOUNDATION)
        ),
        format!(
            "{m}{m}{m} a/gcc/config/arm/arm.c\n{p}{p}{p} b/gcc/config/arm/arm.c\n{a}{a} -8806,4 +8806,5 {a}{a} arm_expand_prologue\n context\n{p}  added (rtx);\n",
            m = minus,
            p = plus,
            a = at
        ),
        format!("{a}{a} -1 +1 {a}{a}\n{m}old\n{p}new\n", a = at, m = minus, p = plus),
        format!(
            "{}\n{s}{s}{s} 12,14 {s}{s}{s}{s}\n  kept\n{m}{m}{m} 12,15 {m}{m}{m}{m}\n",
            star.to_string().repeat(15),
            s = star,
            m = minus
        ),
        format!("GIT binary {}\nliteral 12\nzcmZ\n", "patch"),
    ]
}
/// `(path, bytes, expected rejection reason fragment)`.
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
    // The first row holds the logo under the Sub filter: each byte less its left.
    let mut row = logo.clone();
    row.resize(160, 0);
    let mut hidden = scanlines(160, 8);
    hidden[0] = 1;
    for index in 0..row.len() {
        let left = index.checked_sub(1).map_or(0, |left| row[left]);
        hidden[1 + index] = row[index].wrapping_sub(left);
    }
    let hidden = fdeflate::compress_to_vec(&hidden);
    let wave = psynergy::assets::wav::pcm8_wav(&[0; 64], 8000).unwrap();
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
        psynergy::assets::midi::append_conductor_meta(&midi, kind, payload).unwrap()
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
            "games/THE BROKEN SEAL/PREVIEW/DJINN_101_IDLE.GIF",
            b"GIF89a".to_vec(),
            Some("presentation material"),
        ),
        (
            "games/THE BROKEN SEAL/PREVIEW/title.png",
            indexed_fixture(4),
            Some("PREVIEW"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SHOT.PNG",
            png_fixture(16, 8, 6, &stream, &[]),
            Some("truecolour"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/IDLE.INDEXED.PNG",
            png_fixture(16, 4, 3, &stream, &[(b"acTL", &[0, 0, 0, 2, 0, 0, 0, 0])]),
            Some("animated PNG"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/TABLE.DAT",
            b"\x01\x02\0\x03".to_vec(),
            Some("editable native game input"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE_00.PCM4",
            vec![0x5a; 65],
            unregistered,
        ),
        ("tools/alchemy/tests/tone.wav", wave.clone(), unregistered),
        (
            "games/THE BROKEN SEAL/SOUND/X.PNG",
            indexed_fixture(4),
            unregistered,
        ),
        (
            "tools/alchemy/tests/header.dat",
            fragment,
            Some("ROM header fragment"),
        ),
        (
            "games/THE LOST AGE/SOUND/SEQUENCE/X.MID",
            midi.clone(),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/TRAILING.INDEXED.PNG",
            trailing,
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/TEXT.INDEXED.PNG",
            png_fixture(16, 4, 3, &stream, &[(b"tEXt", b"Comment\0payload")]),
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SURPLUS.INDEXED.PNG",
            png_fixture(16, 4, 3, &surplus, &[]),
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SECOND.INDEXED.PNG",
            png_fixture(16, 4, 3, &second, &[]),
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/LOGO.INDEXED.PNG",
            png_fixture(160, 8, 3, &hidden, &[]),
            nintendo,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LONG.PCM8.WAV",
            [wave.as_slice(), &[0]].concat(),
            wav,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LIST.PCM8.WAV",
            listed_wave,
            wav,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LOGO.PCM8.WAV",
            psynergy::assets::wav::pcm8_wav(&samples, 8000).unwrap(),
            nintendo,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/CHUNK.MID",
            [midi.as_slice(), b"XXXX\0\0\0\x01\0"].concat(),
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/SYSEX.MID",
            midi_fixture(&[0, 0xf0, 3, 1, 2, 0xf7], true),
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/BINARY.MID",
            meta(0x7f, &[0xff, 0xfe, 0x80]),
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/OPEN.MID",
            midi_fixture(&[], false),
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/TEMPO.MID",
            meta(0x51, &fixture_bytes(64, 3)),
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/TIMED.MID",
            meta(0x51, &[7, 0xa1, 0x20]),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/PAYLOAD.MID",
            meta(0x01, encoded_fixture(&base64, 600, 2).as_bytes()),
            Some("encoded payload"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/SPARE.TOKENS",
            table.clone(),
            Some("stored compression token table"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS",
            garbled,
            Some("stored compression token table"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/FONT/LOCALIZATION_GLYPHS_0020_00FF.1BPP.PNG",
            indexed_fixture(1),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/UI_MTF_00.INDEXED.PNG",
            indexed_fixture(4),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/UI_TILE.8BPP.PNG",
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
            "games/THE BROKEN SEAL/TEXT/STAFF_ROLL_MOJI.1BPP.PNG",
            indexed_fixture(1),
            None,
        ),
        (
            "games/THE BROKEN SEAL/TEXT/INK_AND_PAPER.1BPP.PNG",
            grey_fixture(1),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/UI_TILE.4BPP.PNG",
            grey_fixture(4),
            Some("grey sheet"),
        ),
        (
            "games/THE LOST AGE/SOUND/SAMPLE/WAVE_00.PCM8.WAV",
            wave,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/THEME.MID",
            meta(0x7f, &directive),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/SKELETON.MID",
            meta(0x01, b"smsh-sequence 1\nstream track_1\n"),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/JSON.MID",
            meta(0x01, br#"{"format":1,"layout":[]}"#),
            Some("JSON"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/MARKER.MID",
            meta(0x06, br#"["fine"]"#),
            Some("JSON"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/PLACED.MID",
            meta(0x01, b"smsh-sequence 1\nheader s base=0x08000000\n"),
            Some("ROM address or placement"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/TONE_BANK.MID",
            meta(0x01, b"smsh-sequence 1\nheader s externals=tone_bank\n"),
            Some("ROM address or placement"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/LITERAL.MID",
            meta(0x06, b"goto 0x0815fb78"),
            Some("ROM address or placement"),
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE_00.PCM4",
            vec![0x5a; 16],
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS",
            table,
            Some("stored compression token table"),
        ),
    ]
}
fn text_fixtures() -> Vec<Fixture> {
    let tbs = "games/THE BROKEN SEAL";
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
        "include_bytes{bang} (\"../../../GAMES/{}/LOGO.DAT\")",
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
    let [license_header, wrapped_license, lesser_license, copyright, unified, headerless, context, binary_patch] =
        toolchain_fixtures();
    let license = Some("license marker");
    let patch = Some("patch or diff");
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
            "games/THE BROKEN SEAL/SRC/SYSTEM/BLOB.JSON",
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
        (
            "games/THE BROKEN SEAL/SRC/BATTLE/TABLE.C",
            c_table,
            arrays,
        ),
        (
            "games/THE BROKEN SEAL/SRC/BATTLE/DATA/TABLE.JSON",
            json_table,
            Some(JSON_REASON),
        ),
        (
            "games/THE LOST AGE/DATA/TABLE.TSV",
            empty(),
            native,
        ),
        (
            "games/THE LOST AGE/src/battle/table.tsv",
            empty(),
            native,
        ),
        (
            "Games/THE BROKEN SEAL/SRC/TABLE.TSV",
            empty(),
            native,
        ),
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
        (
            "recon/tbs/en/main/0800ebec.c.bak",
            b"int x;\n".to_vec(),
            Some("backup"),
        ),
        (
            "recon/tla/raw/overlays/.gitkeep",
            Vec::new(),
            None,
        ),
        (
            "games/THE LOST AGE/SRC/MAIN/X.C",
            b"void f(void) {}\n".to_vec(),
            None,
        ),
        (
            "games/COMMON/SRC/SOUND/X.C",
            b"void f(void) {}\n".to_vec(),
            None,
        ),
        (
            "games/COMMON/SRC/X.C",
            b"void f(void) {}\n".to_vec(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/SRC/GRAPHICS/FONT/TEXT.TSV",
            b"first\t20\n".to_vec(),
            None,
        ),
        (
            "games/COMMON/SRC/SOUND/X.H",
            b"void f(void);\n".to_vec(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/INCLUDE/SOUND/X.H",
            b"void f(void);\n".to_vec(),
            None,
        ),
        (
            "games/COMMON/SOUND/SEQUENCE/X.S",
            b"\t.section .rodata\n".to_vec(),
            None,
        ),
        (
            "games/COMMON/SOUND/SAMPLE/X.S",
            b"\t.section .rodata\n".to_vec(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/SOUND/SEQUENCE/DEEP/X.S",
            b"\t.section .rodata\n".to_vec(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/SRC/SOUND/TABLE.dat",
            empty(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/recon/translation-units.tsv",
            empty(),
            Some("games/COMMON holds only"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/FIELD/SCENE/SCENE.C",
            text(identifiers),
            None,
        ),
        ("tools/Cargo.lock", text(checksums), None),
        (
            "tools/alchemy/src/hashes.rs",
            text(digests(2_048)),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/ROM_HEADER.JSON",
            text(rows),
            Some(JSON_REASON),
        ),
        (
            "tools/alchemy/src/dashboard/style.css",
            b"body { font: 16px mono; }\n".to_vec(),
            None,
        ),
        (
            "PROGRESS.svg",
            svg,
            Some("presentation material"),
        ),
        (
            "games/THE BROKEN SEAL/INCLUDE/ADD_PARTS_BODY.INC",
            text(license_header),
            license,
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/LICENSE.S",
            text(wrapped_license),
            license,
        ),
        ("tools/alchemy/src/runtime.rs", text(lesser_license), license),
        (
            "games/THE BROKEN SEAL/SRC/LIB/SOFT_FLOAT.C",
            text(copyright),
            license,
        ),
        (
            "tools/alchemy/notes.rs",
            text(unified),
            patch,
        ),
        ("AGENTS.md", text(headerless), patch),
        ("tools/alchemy/src/compiler.rs", text(context), patch),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/BUILD.INC",
            text(binary_patch),
            patch,
        ),
        (
            "AGENTS.md",
            text(format!(
                "Code whose license this repository cannot carry stays in the {} submodules; {}{} marks a hunk.\n",
                "licensed", "@", "@"
            )),
            None,
        ),
        (
            "tools/alchemy/src/score/fixture.rs",
            text(format!(
                "write(&patch, \"{a}{a} -1 +1 {a}{a}\\n-a\\n+b\\n\");\n{a}{a} -1 +1 {a}{a}\nnot a body\n",
                a = '@'
            )),
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/FIELD/FOUNDATION.C",
            text(format!(
                "/* {} of the {} */\nvoid Found(void) {{}}\n",
                "the free software",
                FOUNDATION.replace('|', "")
            )),
            None,
        ),
    ]
}
/// JSON anywhere is refused, whatever it holds.
fn json_ban_fixtures() -> Vec<Fixture> {
    [
        "games/THE BROKEN SEAL/SRC/GAME/TABLE.JSON",
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
fn self_test(root: &Path) -> Result<(), String> {
    for directory in BLOCKED_DIRECTORIES {
        let path = format!("{directory}/fixture.c");
        if publication_path_reason(&path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for suffix in BLOCKED_EXTENSIONS
        .iter()
        .chain(PRESENTATION_EXTENSIONS)
        .chain(BACKUP_EXTENSIONS)
        .chain(DOCUMENT_EXTENSIONS)
    {
        let path = format!("fixture.{suffix}");
        if publication_path_reason(&path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for path in [
        "baserom",
        "agscc-tla.unidiff",
        "compiler.UNIDIFF",
        "private-diff.json",
        "tbs-en.gba.lz",
        ".cmatch-fresh/result.s",
        "games/THE BROKEN SEAL/PREVIEW/title.png",
        "games/THE BROKEN SEAL/SRC/X.BIN",
        "games/THE LOST AGE/SRC/X.BIN",
        "games/COMMON/SRC/M/X.BIN",
        "games/THE BROKEN SEAL/SRC/SYSTEM/BUILD_STAMP.JSON",
        "recon/tbs/raw/080000c0.s~",
        "recon/tbs/assets.json",
        "docs/README.md",
        "CONTRIBUTING.md",
        "CLAUDE.md",
        "TODO.md",
        ".agents/RECOVERY.md",
        ".agents/notes.md",
        ".agents/DEEP/TOPIC.md",
        "GUIDE.markdown",
        "tools/alchemy/NOTES.RST",
        "recon/tbs/plan.adoc",
    ] {
        if publication_path_reason(path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    // Every removed recon ledger stays out under its own name, and so does
    // any other file beside pret's scaffolding.
    for game in ["tbs", "tla"] {
        for ledger in [
            "compiler-runtime.json",
            "text.json",
            "locations.tsv",
            "metrics/history.json",
            "metrics/executable.json",
            "semantic/overlay-assembly.json",
            "semantic/regions.json",
            "graphics-review.json",
            "raw/regions.json",
            "raw/overlays/resource_380.tsv",
            "en/main/owners.json",
            "RAW/08000000.s",
        ] {
            let path = format!("recon/{game}/{ledger}");
            if publication_path_reason(&path)
                .or_else(|| native_path_reason(&path))
                .is_none()
            {
                return Err(format!("recon ledger accepted: {path}"));
            }
        }
    }
    for path in [
        "recon/tbs/overlays.s",
        "recon/tla/sym_ewram.s",
        "recon/tbs/raw/08000000.s",
        "recon/tbs/raw/080022ec.S",
        "recon/tla/raw/overlays/resource_64e_overlay.s",
        "recon/tla/raw/overlays/resource_64e.ld",
        "recon/tbs/en/main/08006878.c",
        "recon/tbs/en/main/draft.h",
        "recon/tbs/en/overlays/resource_372/scene.c",
        "recon/tla/de/main/08001234.c",
        "recon/tbs/ja/main/.gitkeep",
        "recon/tbs/ja/MAIN.LD",
        "recon/tla/de/rom.s",
        "recon/tbs/metrics/history.tsv",
    ] {
        if let Some(reason) = publication_path_reason(path).or_else(|| native_path_reason(path)) {
            return Err(format!("recon scaffolding rejected: {path}: {reason}"));
        }
    }
    for path in [
        "src/main.c",
        "README.md",
        "AGENTS.md",
        "PROGRESS.png",
        "PROGRESS_CHART.png",
        "games/THE BROKEN SEAL/SOUND/SEQUENCE/THEME.mid",
        "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE.wav",
        "tools/compare-roms/src/main.rs",
        "tools/alchemy/src/build_rom.rs",
        "rom.sha1",
    ] {
        if let Some(reason) = publication_path_reason(path) {
            return Err(format!("source path rejected: {path}: {reason}"));
        }
    }
    let mut rom = vec![0u8; 0x8000];
    rom[0xb2] = 0x96;
    let sum = rom[0xa0..=0xbc]
        .iter()
        .fold(0u8, |sum, byte| sum.wrapping_add(*byte));
    rom[0xbd] = 0u8.wrapping_sub(sum).wrapping_sub(0x19);
    let signatures_hold = publication_content_reason(&rom) == Some("GBA ROM image")
        && publication_content_reason(&[0x7f, b'E', b'L', b'F']) == Some("ELF build product")
        && publication_content_reason(b"!<arch>\n") == Some("archive or object library")
        && publication_content_reason(b"canonical source").is_none();
    if !signatures_hold {
        return Err("content-signature self-test failed".to_string());
    }
    check_fixtures()?;
    // The real logo, when the local ROM supplies it, is found at any offset.
    if let Some(logo) = nintendo_logo(root) {
        let hidden = [b"version 1\n".as_slice(), &logo, b"\n"].concat();
        if logo.len() != LOGO.len()
            || publication_data_reason("tools/alchemy/src/notes.rs", &hidden, Some(&logo))
                != Some(LOGO_REASON)
        {
            return Err("the cartridge logo from the local ROM was accepted".to_string());
        }
    }
    let hygiene_holds =
        publication_data_reason("recon/tbs/raw/08000000.s", b".incbin \"rom.gba\"\n", None)
            == Some("committed incbin payload")
            && publication_data_reason(
                "games/THE BROKEN SEAL/SRC/FIELD/IMPORT.INC",
                b"  .incbin \"x\"\n",
                None,
            ) == Some("committed incbin payload")
            && conflict_marker_reason("AGENTS.md", b"a\n<<<<<<< HEAD\nb\n").is_some()
            && conflict_marker_reason("AGENTS.md", b"a\n>>>>>>> topic\n").is_some()
            && publication_data_reason("AGENTS.md", b"x\n<<<<<<< HEAD\n", None).is_none()
            && conflict_marker_reason("AGENTS.md", b"Title\n=======\n\nbody\n").is_none()
            && conflict_marker_reason("AGENTS.md", b"see <<<<<<<HEAD in the output\n").is_none()
            && conflict_marker_reason("games/THE BROKEN SEAL/PREVIEW/x.png", b"<<<<<<< HEAD\n")
                .is_none();
    if !hygiene_holds {
        return Err("source-hygiene self-test failed".to_string());
    }
    if commit_message_reason("fixed the header\n\n00 11 22 33 44 55 66 77\n").is_none() {
        return Err("a byte dump in a commit message was accepted".to_string());
    }
    for message in [
        "Close 12 owners the sweep left open\n",
        "reverts 3d36cfb0aa11bb22cc33dd44ee55ff6677889900\n",
        "resource_39b:e6c span 0x02000e6c..0x02000e78 is not audited\n",
        "the prologue pushes r7 where the reference does not\n",
        "the low halfword ff 00 stayed wrong\n",
    ] {
        if commit_message_reason(message).is_some() {
            return Err(format!(
                "a legitimate commit message was rejected: {}",
                message.trim()
            ));
        }
    }
    Ok(())
}
const USAGE: &str = "usage: check publication [--documents | --staged | --pre-push | --tree [REV] | --history [REV] | --self-test]\n\nModes:\n  --documents    Check owned documentation, including ignored output.\n  --history [REV] Check every commit message and file version in history, or reachable from REV; writes out/history-audit.tsv.\n  --staged       Check staged files before committing.\n  --pre-push     Check commits absent from remotes and each pushed tree using update lines on stdin.\n  --tree [REV]   Check every file tracked in the index, or in revision REV.\n  --self-test    Run the publication gate's internal checks.\n  -h, --help     Show this help.";
fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}
pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    match arguments {
        [argument] if argument == "--history" => {
            check_history(root, None).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
        [argument, revision] if argument == "--history" => check_history(root, Some(revision))
            .map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS),
        [argument] if argument == "--documents" => {
            check_documents(root).map_or_else(|error| fail(&error), |_| ExitCode::SUCCESS)
        }
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
        [argument] if argument == "--self-test" => self_test(root).map_or_else(
            |error| fail(&error),
            |_| {
                println!("self-test=ok");
                ExitCode::SUCCESS
            },
        ),
        _ => fail(USAGE),
    }
}
#[cfg(test)]
mod tests {
    #[test]
    fn only_base_rom_ranges_and_built_overlays_may_incbin_and_only_in_scaffolding() {
        let range = b".incbin \"baserom.gba\", 0x00037464, 0x0003c3a4\n";
        assert!(!super::incbin("recon/tbs/unidentified.s", range));
        assert!(super::incbin("games/THE BROKEN SEAL/SRC/DATA.S", range));
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
        assert!(super::incbin("games/THE BROKEN SEAL/SRC/DATA.S", overlay));
        assert!(super::incbin(
            "recon/tbs/overlays.s",
            b".incbin \"overlays/resource_36f.bin\"\n"
        ));
    }

    #[test]
    fn sound_data_sources_may_incbin_only_the_sound_files_the_build_makes() {
        let sample = b"Sound_Wave00:\n\t.incbin \"SOUND/SAMPLE/WAVE_00.PCM8.bin\"\n";
        let wave = b"\t.incbin \"SOUND/SAMPLE/CGB_WAVE_0.bin\"\n";
        let shared = b"\t.incbin \"COMMON/SOUND/SAMPLE/WAVE_20.PCM8.bin\"\n";
        for path in [
            "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
            "games/THE LOST AGE/SOUND/CGB_WAVES.S",
        ] {
            assert!(!super::incbin(path, sample), "{path}");
            assert!(!super::incbin(path, wave), "{path}");
            assert!(!super::incbin(path, shared), "{path}");
        }
        assert!(super::incbin(
            "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
            b"\t.incbin \"COMMON/SAMPLE/WAVE_20.PCM8.bin\"\n"
        ));
        // Only a game's sound sources, and only built files under SOUND.
        for (path, line) in [
            ("games/THE BROKEN SEAL/SRC/SOUND/DATA.S", sample.as_slice()),
            ("recon/tbs/unidentified.s", sample),
            ("games/COMMON/SOUND/SAMPLES.S", sample),
            (
                "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
                b".incbin \"baserom.gba\", 0x000fd048, 0x000002ac\n",
            ),
            (
                "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
                b".incbin \"SOUND/SAMPLE/WAVE_00.PCM8.WAV\"\n",
            ),
            (
                "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
                b".incbin \"SOUND/../../../roms/tbs-en.bin\"\n",
            ),
            (
                "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
                b".incbin \"SOUND/SAMPLE/WAVE_00.PCM8.bin\", 0, 16\n",
            ),
            (
                "games/THE BROKEN SEAL/SOUND/SAMPLES.S",
                b".incbin \"overlays/resource_36f.lz\"\n",
            ),
        ] {
            assert!(super::incbin(path, line), "{path}");
        }
        assert_eq!(
            super::publication_data_reason("games/THE BROKEN SEAL/SOUND/SAMPLES.S", sample, None),
            None
        );
    }

    #[test]
    fn asset_sources_may_incbin_only_the_graphics_files_the_build_makes() {
        let sheet = b"BattleFx_Star:\n\t.incbin \"GRAPHICS/FX/STAR.gbapal\"\n\t.incbin \"GRAPHICS/FX/STAR.bitmap.lz\"\n";
        let tiles =
            b"\t.incbin \"GRAPHICS/FX/STAR.4bpp.mtf\"\n\t.incbin \"GRAPHICS/FX/STAR.8bpp\"\n\t.incbin \"GRAPHICS/FX/STAR.4bpp32x16.lz\"\n\t.incbin \"MAP/M/METATILES.delta1.lz\"\n\t.incbin \"MAP/M/ANIMATION.script.lz\"\n\t.incbin \"MAP/M/PATH.table.lz\"\n\t.incbin \"GRAPHICS/FX/FONT.glyphs\"\n\t.incbin \"GRAPHICS/UI/ICONS/ICONS.icons4\"\n\t.incbin \"MAP/WORLD/BLOCKS.blocks\"\n";
        for path in [
            "games/THE BROKEN SEAL/SRC/GRAPHICS/FX/STAR.S",
            "games/THE LOST AGE/SRC/BATTLE/EFFECT/STAR.S",
        ] {
            assert!(!super::incbin(path, sheet), "{path}");
            assert!(!super::incbin(path, tiles), "{path}");
        }
        // Only a game's sources under SRC, and only recipes under GRAPHICS.
        let star = "games/THE BROKEN SEAL/SRC/GRAPHICS/FX/STAR.S";
        for (path, line) in [
            ("recon/tbs/unidentified.s", sheet.as_slice()),
            ("games/THE BROKEN SEAL/SOUND/STAR.S", sheet),
            (star, b".incbin \"GRAPHICS/FX/STAR.PNG\"\n"),
            (star, b".incbin \"GRAPHICS/FX/STAR.raw\"\n"),
            (star, b".incbin \"GRAPHICS/FX/STAR.4bpp.zip\"\n"),
            (star, b".incbin \"MAP/M/CELLS.delta3.lz\"\n"),
            (star, b".incbin \"GRAPHICS/UI/ICONS.icons5\"\n"),
            (star, b".incbin \"MAP/WORLD/BLOCKS.block\"\n"),
            (star, b".incbin \"TEXT/M/CELLS.bin\"\n"),
            (star, b".incbin \"MAP/M/END.bin\"\n"),
            (star, b".incbin \"GRAPHICS/FX/star.4bpp\"\n"),
            (star, b".incbin \"GRAPHICS/../../roms/tbs-en.4bpp\"\n"),
            (star, b".incbin \"GRAPHICS/FX/STAR.4bpp\", 0, 16\n"),
            (star, b".incbin \"baserom.gba\", 0x003cd090, 0x00000488\n"),
        ] {
            assert!(super::incbin(path, line), "{path}");
        }
        assert_eq!(super::publication_data_reason(star, sheet, None), None);
    }

    #[test]
    fn generated_fonts_allow_only_constant_character_layout_ranges() {
        let path = "games/COMMON/SRC/GRAPHICS/FONT/TEXT.S";
        for line in [
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", 0, (0x100 - 0x20) * (2 + 12 * 2)\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", (0x100 - 0x20) * (2 + 12 * 2)\n",
        ] {
            assert!(!super::incbin(path, line.as_bytes()), "{line}");
            assert!(
                super::incbin("recon/tbs/ja/rom.s", line.as_bytes()),
                "{line}"
            );
        }
        for line in [
            ".incbin \"baserom.gba\", 0, (0x100 - 0x20) * 26\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.raw\", 0, 26\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font.lz\", 0, 26\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", ExpectedOffset, 26\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", 0, LOADADDR(.font)\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", 0, 2 +\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", 0, (2 * 26\n",
            ".incbin \"GRAPHICS/FONT/TEXT_JA.font\", -1, 26\n",
        ] {
            assert!(super::incbin(path, line.as_bytes()), "{line}");
        }
    }

    use super::*;

    #[test]
    fn publication_self_test_fixtures_follow_the_current_policy() {
        let root = tempfile::tempdir().unwrap();
        self_test(root.path()).unwrap();
    }

    #[test]
    fn raw_bin_inputs_are_refused_by_path_in_game_and_shared_sources() {
        for path in [
            "games/THE BROKEN SEAL/SRC/X.BIN",
            "games/THE LOST AGE/SRC/X.BIN",
            "games/COMMON/SRC/M/X.BIN",
            "games/THE BROKEN SEAL/SRC/X.bin",
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
            "games/THE BROKEN SEAL/MAIN.LD",
            "games/THE LOST AGE/CONSTANTS.LD",
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
        let path = "recon/tla/raw/ROUTINE.s";
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
            raw_address_reason("games/THE LOST AGE/SRC/ROUTINE.S", "bl 0x08000100\n").is_none()
        );
    }

    #[test]
    fn raw_address_context_uses_owned_pointer_declarations_and_typed_tables() {
        let sources = [
            ("games/COMMON/INCLUDE/TYPES.H", "typedef int s32; typedef void (*Callback)(void);"),
            ("games/THE LOST AGE/INCLUDE/API.H", "void Reader(const void *source, s32 coordinate); void Callback_Set(Callback callback); extern Callback Handlers[2]; extern void *Work; void Motion_CamBounds(s32 x);"),
            ("games/THE BROKEN SEAL/INCLUDE/API.H", "void Motion_CamBounds(void *x);"),
            ("recon/tla/raw/caller.s", "ldr r0, pool\nbl Reader\nbx lr\npool:\n.word 0x02000100\n"),
            ("recon/tla/raw/callback.s", "ldr r0, pool\nbl Callback_Set\nbx lr\npool:\n.word 0x08000101\n"),
            ("recon/tla/raw/standalone.s", "Handlers:\n.word 0x08000201, NamedHandler\n"),
            ("recon/tla/raw/object.s", "Work:\n.word 0x02000200\n.word 0x08040000\n"),
            ("recon/tla/raw/scalar.s", "ldr r0, pool\nbl Motion_CamBounds\nbx lr\npool:\n.word 0x08040000\n"),
        ].into_iter().map(|(path, text)| (path.to_string(), text.to_string())).collect::<Vec<_>>();
        let findings = raw_context_findings(&sources);
        assert_eq!(
            findings.keys().map(String::as_str).collect::<Vec<_>>(),
            [
                "recon/tla/raw/callback.s",
                "recon/tla/raw/caller.s",
                "recon/tla/raw/object.s",
                "recon/tla/raw/standalone.s"
            ]
        );
        assert_eq!(findings["recon/tla/raw/object.s"].len(), 1);
    }

    #[test]
    fn equal_placeholder_labels_in_other_overlays_do_not_supply_argument_types() {
        let sources = [
            ("recon/tla/raw/overlays/resource_1_overlay.s", ".thumb_func\nFunc_02000000:\nldr r0, pool\nbl Func_02000010\nbx lr\npool:\n.word 0x08040000\n.thumb_func\nFunc_02000010:\nbx lr\n"),
            ("recon/tla/raw/overlays/resource_2_overlay.s", ".thumb_func\nFunc_02000010:\nldr r1, [r0]\nbx lr\n"),
        ].into_iter().map(|(path, text)| (path.to_string(), text.to_string())).collect::<Vec<_>>();
        assert!(raw_context_findings(&sources).is_empty());
    }

    #[test]
    #[ignore = "read-only working-tree audit, run when changing raw address analysis"]
    fn current_raw_address_uses_are_labelled() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap();
        let sources = walkdir::WalkDir::new(root.join("games"))
            .into_iter()
            .chain(walkdir::WalkDir::new(root.join("recon")))
            .filter_map(Result::ok)
            .filter(|entry| entry.file_type().is_file())
            .filter_map(|entry| {
                let path = entry.path().strip_prefix(root).ok()?.to_str()?.to_string();
                (listed(extension(&path), &["c", "h"]) && path.starts_with("games/")
                    || raw_source_game(&path).is_some())
                .then(|| {
                    std::fs::read_to_string(entry.path())
                        .ok()
                        .map(|text| (path, text))
                })
                .flatten()
            })
            .collect::<Vec<_>>();
        let findings = raw_context_findings(&sources);
        for (path, sites) in &findings {
            for site in sites {
                println!("{path}:{}: 0x{:08x}", site.line, site.value);
            }
        }
        assert!(
            findings.is_empty(),
            "{} raw source files still contain address uses",
            findings.len()
        );
    }

    #[test]
    fn message_archives_are_built_assembly_included_by_a_tracked_source() {
        // Each game's TEXT/MESSAGES.S includes the assembly build rom writes
        // from the edition's catalog, whose address words are relocations;
        // the tracked source carries no bytes, and an .incbin of a built
        // archive, which could hold no relocations, stays refused.
        let root = crate::compiler::routing::root();
        for game in ["THE BROKEN SEAL", "THE LOST AGE"] {
            let path = format!("games/{game}/TEXT/MESSAGES.S");
            let data = std::fs::read(root.join(&path)).unwrap();
            assert_eq!(publication_reason(&path, &data, None), None, "{path}");
            assert!(String::from_utf8_lossy(&data).contains("\t.include \"text/messages.inc\"\n"));
        }
        assert!(super::incbin(
            "games/THE BROKEN SEAL/TEXT/MESSAGES.S",
            b".incbin \"text/messages.bin\"\n"
        ));
        let named = b"msgctxt \"MsgHpRecover\"\nmsgid \"02077\"\nmsgstr \"HP!\"\n";
        assert_eq!(
            publication_reason("games/THE BROKEN SEAL/TEXT/EN.PO", named, None),
            None
        );
    }

    #[test]
    fn editable_sources_need_no_calculated_asset_catalog() {
        for (path, data) in [
            (
                "games/THE BROKEN SEAL/SRC/FIELD/RUNPA/EVENTS.TSV",
                b"actor\tx\ty\tscript\nGuard\t4\t8\tOpenGate\n".to_vec(),
            ),
            (
                "games/THE LOST AGE/MAIN.LD",
                b"MEMORY { ROM (rx) : ORIGIN = 0x08000000, LENGTH = 32M }\nSoundMixer = LOADADDR(.iwram_mixer) + 1;\n"
                    .to_vec(),
            ),
            (
                "games/THE LOST AGE/BUILD.MK",
                b"GAME_OBJECTS := BATTLE/START.o FIELD/EVENT.o\n".to_vec(),
            ),
            (
                "games/THE LOST AGE/INCLUDE/LINK.H",
                b"extern int Data_081c0000;\n#define BufferAddress 0x02000000\n".to_vec(),
            ),
            (
                "games/THE LOST AGE/SRC/SYSTEM/MACHINE.S",
                b".syntax unified\n.thumb\n.global CallRoutine\nCallRoutine:\n bx r0\n"
                    .to_vec(),
            ),
            (
                "games/THE BROKEN SEAL/TEXT/EN.PO",
                b"msgid \"000001\"\nmsgstr \"Open the gate.\"\n".to_vec(),
            ),
        ] {
            assert_eq!(publication_reason(path, &data, None), None, "{path}");
        }
    }

    #[test]
    fn calculated_ledgers_and_saved_answers_fail_after_renaming_or_splitting() {
        // Ledgers and saved answers written as JSON are refused as JSON,
        // under any name.
        for data in [
            r#"{"decoded_sha256":"00"}"#,
            r#"[{"source":"FIELD/EVENT.C","address":"0x08000100","size":32}]"#,
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
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TOKEN.TOKENS",
            "games/THE BROKEN SEAL/SRC/GRAPHICS/CHAR_COMMON.PNG",
            "games/THE LOST AGE/SRC/DATA.BIN",
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
        for path in ["recon/tbs/raw/routine.s", "games/X/SRC/ROUTINE.S"] {
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
            publication_data_reason("LINK.LD", linker.as_bytes(), None),
            None
        );
        let library = format!(
            ".syntax unified\n.thumb\n.global MultiplyWords\nMultiplyWords:\n cmp r0, #0\n beq .L_{address:08x}\n bx lr\n.L_{address:08x}:\n bx lr\n"
        );
        assert_eq!(
            publication_data_reason("LIBRARY.S", library.as_bytes(), None),
            None
        );
        let global_alias = format!(
            ".syntax unified\n.thumb\n.global Func_{address:08x}\nFunc_{address:08x}:\n bx lr\n"
        );
        assert_eq!(
            publication_data_reason("LIBRARY.S", global_alias.as_bytes(), None),
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
            &[("games/X/SRC/START.C", b"void Start(void) {}\n".to_vec())],
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
        for (file, permitted) in [("README.md", true), ("UNOWNED.md", false)] {
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
        assert!(publication_path_reason("GUIDE.md").is_some());
        for owned in OWNED_DOCUMENTS {
            assert!(publication_path_reason(owned).is_none());
        }
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
        let tbs = "games/THE BROKEN SEAL/SRC/SOUND/HOOK.S";
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
            assert!(raw_encoding_reason("games/THE LOST AGE/SRC/X.INC", text).is_some());
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
        let spelled = "\t.thumb\n\tmovs r0, #1\n\t.inst.n 0x4770\n";
        assert!(raw_encoding_reason("recon/tbs/raw/080006fc.S", spelled).is_none());
        assert!(raw_encoding_reason("games/THE BROKEN SEAL/SRC/X.C", spelled).is_none());
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
        // Comments and pc-relative loads are no equates; the English
        // disassembly and game source keep their own rules.
        let accepted = "@ .set Size, 0x4e8 once stood here\n\tldr r0, =Label\n";
        assert!(edition_equate_reason("recon/tbs/ja/rom.s", accepted).is_none());
        let measured = "\t.set Tile_BuildMetatilesCodeSize, . - Tile_BuildMetatiles\n";
        for path in [
            "recon/tbs/raw/080158e8.S",
            "recon/tbs/unidentified.s",
            "games/THE BROKEN SEAL/SRC/X.S",
            "recon/tbs/ja/MAIN.LD",
        ] {
            assert!(edition_equate_reason(path, measured).is_none(), "{path}");
        }
    }
    #[test]
    fn full_address_equates_are_refused_in_game_and_reconstruction_assembly() {
        let tbs = "games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/TABLES.S";
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
            assert!(address_equate_reason("games/THE LOST AGE/SRC/X.INC", line).is_some());
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
        assert!(address_equate_reason("games/THE BROKEN SEAL/SRC/X.C", restated).is_none());
        assert_eq!(
            publication_data_reason(tbs, restated.as_bytes(), None),
            Some(ADDRESS_EQUATE_REASON)
        );
    }
    #[test]
    fn runtime_routines_and_toolchain_sources_are_refused_where_they_are_defined() {
        // Fixture names are assembled so this source never spells a definition.
        let routine = format!("__{}si3", "div");
        let defined_c = format!("SItype\n{routine} (SItype a, SItype b)\n{{\n");
        let prototype = format!("SItype {routine} (SItype, SItype);\n");
        let called = format!("int f(int a)\n{{\n    return {routine}(a, 3);\n}}\n");
        let commented = format!("/* bl {routine} at 0x080022ec */\n");
        assert!(runtime_definition_reason("SRC/LIB/DIVIDE.C", &defined_c).is_some());
        assert!(runtime_definition_reason("SRC/LIB/DIVIDE.C", &prototype).is_none());
        assert!(runtime_definition_reason("SRC/LIB/DIVIDE.C", &called).is_none());
        assert!(runtime_definition_reason("SRC/LIB/DIVIDE.C", &commented).is_none());
        let label = format!("\t.global {routine}\n{routine}:\n\tpush {{lr}}\n");
        let branch = format!("\tbl {routine}\n");
        assert!(runtime_definition_reason("asm/lib.s", &label).is_some());
        assert!(runtime_definition_reason("asm/lib.s", &branch).is_none());
        // An overlay's import stub may carry the name the compiler calls,
        // when it only jumps to the game's own routine.
        let veneer = format!(
            "\t.global {routine}\n\t.thumb_func\n{routine}:\n\toverlay_veneer IwramSignedDivide\n"
        );
        assert!(runtime_definition_reason("SRC/FIELD/IMPORT.S", &veneer).is_none());
        let other = format!("__{}si3", "mod");
        let onward = format!("\t.global {routine}\n{routine}:\n\toverlay_veneer {other}\n");
        assert!(runtime_definition_reason("SRC/FIELD/IMPORT.S", &onward).is_some());
        let beside = format!("{veneer}\t.global {other}\n{other}:\n\tpush {{lr}}\n");
        assert!(runtime_definition_reason("SRC/FIELD/IMPORT.S", &beside).is_some());
        let redefined = format!(".macro overlay_veneer target\n\tpush {{lr}}\n.endm\n{veneer}");
        assert!(runtime_definition_reason("SRC/FIELD/IMPORT.S", &redefined).is_some());
        assert!(runtime_definition_reason("notes.json", &defined_c).is_none());
        assert!(history_message_reason(&format!("Add the divider\n\n{defined_c}")).is_some());
        assert!(history_message_reason(&format!("Bind {routine} calls in the link")).is_none());
        assert!(publication_path_reason("vendor/gcc/toplev.c").is_some());
        assert!(publication_path_reason("tools/reverse-gcc296/src/main.rs").is_none());
    }
    #[test]
    fn documents_have_two_owners_even_in_ignored_or_nested_checkouts() {
        let temp = tempfile::tempdir().unwrap();
        let root = temp.path();
        for dir in [
            "out",
            ".agents",
            "worktrees/scene",
            "tools/out/compiler-build/experiment/gcc",
            "tools/out/compiler-build/sources/binutils-2.10/gas",
            "tools/out/compiler-build/sources/binutils-2.10/bfd",
            "out/allocator-order",
            "agscc",
        ] {
            std::fs::create_dir_all(root.join(dir)).unwrap();
        }
        std::fs::write(root.join(".gitignore"), "out/\n").unwrap();
        std::fs::write(root.join("README.md"), "public introduction").unwrap();
        std::fs::write(root.join("AGENTS.md"), "all working guidance").unwrap();
        std::fs::write(root.join("agscc/README.md"), "upstream").unwrap();
        std::fs::write(
            root.join("tools/out/compiler-build/experiment/gcc/toplev.c"),
            "upstream",
        )
        .unwrap();
        std::fs::write(
            root.join("tools/out/compiler-build/experiment/gcc/thumb.md"),
            "(define_insn)",
        )
        .unwrap();
        std::fs::write(
            root.join("tools/out/compiler-build/sources/binutils-2.10/configure"),
            "upstream",
        )
        .unwrap();
        std::fs::write(
            root.join("tools/out/compiler-build/sources/binutils-2.10/README.md"),
            "upstream",
        )
        .unwrap();
        std::fs::write(root.join("worktrees/scene/.git"), "gitdir: ../../.git\n").unwrap();
        assert!(check_documents(root).is_ok());
        for name in [
            "out/verdict.md",
            "out/score.TXT",
            "out/plan.rst",
            "out/notes.text",
            "out/notes.mdown",
            "out/notes.rest",
            "out/notes.adoc",
            "out/allocator-order/normal.text",
            "tools/out/compiler-build/notes.md",
            "TODO.md",
            "CONTRIBUTING.md",
            ".agents/RECOVERY.md",
            "worktrees/scene/README.md",
            "worktrees/scene/score.txt",
        ] {
            std::fs::write(root.join(name), "another guide").unwrap();
            assert!(check_documents(root).unwrap_err().contains(name), "{name}");
            std::fs::remove_file(root.join(name)).unwrap();
        }
        // An extra document symlink must not bypass the same path policy.
        std::os::unix::fs::symlink("../AGENTS.md", root.join("out/alias.md")).unwrap();
        assert!(check_documents(root).unwrap_err().contains("out/alias.md"));
        std::fs::remove_file(root.join("out/alias.md")).unwrap();
        std::fs::write(root.join("CLAUDE.md"), "a competing guide").unwrap();
        assert!(check_documents(root).unwrap_err().contains("CLAUDE.md"));
        std::fs::remove_file(root.join("CLAUDE.md")).unwrap();
        std::os::unix::fs::symlink("AGENTS.md", root.join("CLAUDE.md")).unwrap();
        assert!(check_documents(root).unwrap_err().contains("CLAUDE.md"));
    }
    #[test]
    fn later_parts_share_the_first_parts_built_palette() {
        let temp = tempfile::tempdir().unwrap();
        let directory = temp.path().join("games/X/SRC/GRAPHICS");
        std::fs::create_dir_all(&directory).unwrap();
        let second = directory.join("W_2.PNG").to_string_lossy().into_owned();
        std::fs::write(directory.join("W.TSV"), "W_1\t4bpp\nW_2\t4bpp16x32\n").unwrap();
        assert!(!palette_built(&second));
        std::fs::write(directory.join("W.S"), "\t.incbin \"GRAPHICS/W_1.gbapal\"\n").unwrap();
        assert!(palette_built(&second));
        assert!(palette_built(&directory.join("W_1.PNG").to_string_lossy()));
    }

    #[test]
    fn a_grey_palette_passes_only_when_the_build_writes_it() {
        let temp = tempfile::tempdir().unwrap();
        let directory = temp.path().join("games/X/SRC/GRAPHICS");
        std::fs::create_dir_all(&directory).unwrap();
        let png = directory.join("LOGO.PNG");
        let path = png.to_string_lossy().into_owned();
        assert!(!palette_built(&path));
        std::fs::write(
            directory.join("OTHER.S"),
            "\t.incbin \"GRAPHICS/OTHER.gbapal\"\n",
        )
        .unwrap();
        assert!(!palette_built(&path));
        std::fs::write(
            directory.join("LOGO.S"),
            "\t.incbin \"GRAPHICS/LOGO.gbapal\"\n",
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
        // Each line alone is short; the file as a whole is a payload.
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
            .find(|(path, ..)| path.ends_with("/LOGO.INDEXED.PNG"))
            .unwrap();
        assert!(!contains(&png, &logo));
        assert!(contains(&indexed_png_bytes(&png).unwrap(), &logo));
        let path = "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/LOGO.INDEXED.PNG";
        assert_eq!(publication_data_reason(path, &png, None), None);
        assert_eq!(
            publication_data_reason(path, &png, Some(&logo)),
            Some(LOGO_REASON)
        );
        // The README figures get no pass from the logo or grey-sheet checks.
        for figure in ["PROGRESS.png", "PROGRESS_CHART.png"] {
            assert_eq!(publication_data_reason(figure, &png, None), None);
            assert_eq!(
                publication_data_reason(figure, &png, Some(&logo)),
                Some(LOGO_REASON)
            );
            assert_eq!(
                publication_data_reason(figure, &grey_fixture(4), None),
                Some(GREY_SHEET)
            );
        }
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
    fn raw_pointer_context_is_bound_to_the_index_and_each_outgoing_tree() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let header = "games/THE LOST AGE/INCLUDE/API.H";
        let raw = "recon/tla/raw/caller.s";
        let scalar = b"void Consumer(int value);\n".to_vec();
        let pointer = b"void Consumer(void *value);\n".to_vec();
        let published = commit(
            root,
            &[
                (header, scalar.clone()),
                (
                    raw,
                    b"ldr r0, pool\nbl Consumer\nbx lr\npool:\n.word 0x08040000\n".to_vec(),
                ),
            ],
        );
        assert!(check_tree(root, Some(&published)).is_ok());

        // Unstaged source must not type a staged raw operand. Conversely, a
        // staged declaration can expose an unchanged raw pointer argument.
        std::fs::write(root.join(header), &pointer).unwrap();
        assert!(check_tree(root, None).is_ok());
        git(root, &["add", header], "fixture git").unwrap();
        std::fs::write(root.join(header), &scalar).unwrap();
        assert!(check_tree(root, None).unwrap_err().contains(raw));
        assert!(check_staged(root).unwrap_err().contains(raw));
        assert!(check_tree(root, Some(&published)).is_ok());

        std::fs::write(root.join(header), &pointer).unwrap();
        let bad = commit(root, &[]);
        let tip = commit(root, &[(header, scalar)]);
        assert!(check_tree(root, Some(&tip)).is_ok());
        let update = format!("refs/heads/topic {tip} refs/heads/topic {published}\n");
        let rejected = check_push(root, &update).unwrap_err();
        assert!(rejected.contains(&bad[..12]), "{rejected}");
        assert!(rejected.contains(raw), "{rejected}");
    }

    #[test]
    fn new_branch_push_excludes_published_history_but_full_history_still_audits_it() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        commit(root, &[("private.DUMP", b"fixture\n".to_vec())]);
        git(root, &["rm", "private.DUMP"], "fixture git").unwrap();
        let published = commit(root, &[("README.md", b"Project\n".to_vec())]);
        git(
            root,
            &["update-ref", "refs/remotes/origin/main", &published],
            "fixture remote",
        )
        .unwrap();
        let tip = commit(
            root,
            &[("games/X/SRC/START.C", b"void Start(void) {}\n".to_vec())],
        );
        let zero = "0".repeat(40);
        assert_eq!(revisions(root, &tip, &zero).unwrap(), [tip.clone()]);
        let update = format!("refs/heads/topic {tip} refs/heads/topic {zero}\n");
        assert!(check_push(root, &update).is_ok());
        assert!(check_history(root, Some(&tip)).is_err());
        let report = std::fs::read_to_string(root.join("out/history-audit.tsv")).unwrap();
        assert!(report.contains("private.DUMP"), "{report}");
    }

    #[test]
    fn advertised_remote_tip_is_excluded_even_without_a_remote_tracking_ref() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let published = commit(root, &[("README.md", b"Project\n".to_vec())]);
        let tip = commit(
            root,
            &[("games/X/SRC/START.C", b"void Start(void) {}\n".to_vec())],
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
                ("games/X/SRC/GRAPHICS/TILE/A.4BPP.PNG", indexed_fixture(4)),
                ("games/X/SOUND/SEQUENCE/A.MID", midi.clone()),
                ("games/X/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("games/Y/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("PROGRESS.png", indexed_fixture(4)),
                ("tools/Cargo.lock", b"checksum = \"00ff\"\n".to_vec()),
            ],
        );
        assert!(check_tree(root, None).is_ok());
        assert!(check_tree(root, Some(&inputs)).is_ok());
        let gif = commit(
            root,
            &[(
                "games/X/PREVIEW/IDLE.gif",
                b"GIF89a\x01\0\x01\0\0\0\0".to_vec(),
            )],
        );
        let error = check_tree(root, Some(&gif)).unwrap_err();
        assert_eq!(error.lines().count(), 2, "{error}");
        assert!(error.contains("games/X/PREVIEW/IDLE.gif: presentation material"));
        assert!(check_tree(root, None).unwrap_err().contains("IDLE.gif"));
        let zero = "0".repeat(40);
        let update = format!("refs/heads/main {gif} refs/heads/main {inputs}\n");
        let error = check_push(root, &update).unwrap_err();
        assert!(
            error.contains("IDLE.gif") && !error.contains("A.4BPP.PNG"),
            "{error}"
        );
        let update = format!("refs/heads/main {inputs} refs/heads/main {zero}\n");
        assert!(check_push(root, &update).is_ok());
        let native_and_misplaced = commit(
            root,
            &[
                ("games/Y/SOUND/SEQUENCE/A.MID", midi),
                ("games/Y/Data/TABLE.JSON", b"{}\n".to_vec()),
            ],
        );
        let error = check_tree(root, Some(&native_and_misplaced)).unwrap_err();
        assert!(!error.contains("games/Y/SOUND/SEQUENCE/A.MID:"));
        assert!(error.contains("games/Y/Data/TABLE.JSON: JSON is banned"));
        assert!(!error.contains("games/X/SOUND"), "{error}");
    }
    #[test]
    fn toolchain_license_markers_patches_and_foreign_gitlinks_fail_in_every_mode() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let run = |args: &[&str]| git(root, args, "fixture git").unwrap();
        run(&["init", "--quiet"]);
        let base = commit(root, &[("tools/src/main.rs", b"fn main() {}\n".to_vec())]);
        let [license_header, _, _, copyright, unified, _, context, _] = toolchain_fixtures();
        std::fs::create_dir_all(root.join("games/X/SRC/LIB")).unwrap();
        std::fs::write(root.join("games/X/SRC/LIB/RUNTIME.C"), &license_header).unwrap();
        std::fs::write(root.join("tools/src/gcc.rs"), &unified).unwrap();
        run(&["add", "games/X/SRC/LIB/RUNTIME.C", "tools/src/gcc.rs"]);
        let error = check_staged(root).unwrap_err();
        assert!(
            error.contains("staged games/X/SRC/LIB/RUNTIME.C: license marker"),
            "{error}"
        );
        assert!(
            error.contains("staged tools/src/gcc.rs: patch or diff"),
            "{error}"
        );
        let error = check_tree(root, None).unwrap_err();
        assert!(
            error.contains("tree tools/src/gcc.rs: patch or diff"),
            "{error}"
        );
        let pushed = commit(
            root,
            &[
                ("tools/src/runtime.rs", copyright.into_bytes()),
                ("tools/src/compiler.rs", context.into_bytes()),
            ],
        );
        run(&[
            "update-index",
            "--add",
            "--cacheinfo",
            &format!("160000,{base},vendor/gcc"),
        ]);
        run(&[
            "-c",
            "user.name=fixture",
            "-c",
            "user.email=fixture@example.invalid",
            "commit",
            "--quiet",
            "--message",
            "gitlink",
        ]);
        let linked = String::from_utf8(run(&["rev-parse", "HEAD"]))
            .unwrap()
            .trim()
            .to_string();
        let error = check_tree(root, Some(&pushed)).unwrap_err();
        for path in [
            "RUNTIME.C: license marker",
            "tools/src/gcc.rs: patch or diff",
            "runtime.rs: license marker",
            "compiler.rs: patch or diff",
        ] {
            assert!(error.contains(path), "{path}: {error}");
        }
        let update = format!("refs/heads/main {linked} refs/heads/main {base}\n");
        let error = check_push(root, &update).unwrap_err();
        assert!(error.contains("vendor/gcc: unapproved gitlink"), "{error}");
        assert!(error.contains("runtime.rs: license marker"), "{error}");
        assert!(error.contains("tools/src/gcc.rs: patch or diff"), "{error}");
    }
    #[test]
    fn license_and_diff_structure_ignore_prose_and_escaped_strings() {
        let [license_header, wrapped, lesser, copyright, unified, headerless, context, binary] =
            toolchain_fixtures();
        for text in [&license_header, &wrapped, &lesser, &copyright] {
            assert_eq!(license_reason(text), Some(LICENSE_REASON), "{text}");
            assert_eq!(
                license_reason(&text.to_ascii_uppercase()),
                Some(LICENSE_REASON)
            );
        }
        for text in [&unified, &headerless, &context, &binary] {
            assert_eq!(diff_reason(text), Some(DIFF_REASON), "{text}");
            assert_eq!(diff_reason(&text.replace('\n', "\r\n")), Some(DIFF_REASON));
        }
        let at = "@@";
        assert!(unified_hunk(&format!("{at} -1,2 +3 {at} context")));
        assert!(unified_hunk(&format!("{at}@ -1,2 -1,2 +1,3 {at}@")));
        for line in [
            format!("{at} -1,2 +3"),
            format!("{at} 1,2 +3 {at}"),
            format!("{at} -1,x +3 {at}"),
            format!("{at}@ -1 +1 {at}@"),
            format!("x{at} -1 +1 {at}"),
        ] {
            assert!(!unified_hunk(&line), "{line}");
        }
        assert!(diff_reason(&format!("{at} -1 +1 {at}\nplain\n")).is_none());
        assert!(license_reason("license: see the licensed agscc submodule\n").is_none());
        // The words must appear in order and, for a copyright, near it.
        let far = format!(
            "{} (C) 1999\n{}\n{}\n",
            COPYRIGHT.replace('|', ""),
            "word ".repeat(COPYRIGHT_WINDOW + 1),
            FOUNDATION.replace('|', "")
        );
        assert!(license_reason(&far).is_none());
    }
    #[test]
    fn renamed_documents_lfs_and_foreign_gitlinks_fail_in_every_mode() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let run = |args: &[&str]| git(root, args, "fixture git").unwrap();
        run(&["init", "--quiet"]);
        let head = commit(
            root,
            &[
                ("README.md", b"# fixture\n".to_vec()),
                ("tools/src/main.rs", b"fn main() {}\n".to_vec()),
            ],
        );
        run(&["mv", "README.md", "GUIDE.md"]);
        let error = check_staged(root).unwrap_err();
        assert!(
            error.contains("staged GUIDE.md: separate document"),
            "{error}"
        );
        assert!(check_tree(root, None).unwrap_err().contains("GUIDE.md"));
        run(&["mv", "GUIDE.md", "README.md"]);
        run(&[
            "update-index",
            "--add",
            "--cacheinfo",
            &format!("160000,{head},vendor/tool"),
        ]);
        run(&[
            "update-index",
            "--add",
            "--cacheinfo",
            &format!("160000,{head},agbcc"),
        ]);
        let error = check_staged(root).unwrap_err();
        assert!(
            error.contains("staged vendor/tool: unapproved gitlink"),
            "{error}"
        );
        assert!(!error.contains("staged agbcc"), "{error}");
        let error = check_tree(root, None).unwrap_err();
        assert!(
            error.contains("tree vendor/tool: unapproved gitlink"),
            "{error}"
        );
        assert_eq!(error.lines().count(), 2, "{error}");
        run(&["update-index", "--force-remove", "vendor/tool"]);
        assert!(check_tree(root, None).is_ok());
        let pointer = format!(
            "version https://git-lfs.github.com/spec/v1\noid sha256:{}\nsize 8388608\n",
            "0".repeat(64)
        );
        let pushed = commit(
            root,
            &[
                (
                    ".gitattributes",
                    b"*.gba filter=lfs diff=lfs merge=lfs -text\n".to_vec(),
                ),
                ("tools/assets/figure.png", pointer.into_bytes()),
            ],
        );
        let error = check_tree(root, None).unwrap_err();
        assert!(
            error.contains(".gitattributes: Git filter attribute"),
            "{error}"
        );
        assert!(
            error.contains("tools/assets/figure.png: Git LFS pointer"),
            "{error}"
        );
        let update = format!("refs/heads/main {pushed} refs/heads/main {head}\n");
        let error = check_push(root, &update).unwrap_err();
        assert!(error.contains("figure.png: Git LFS pointer"), "{error}");
    }
}
