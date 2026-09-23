//! Fail-closed publication checks for staged changes, outgoing history and
//! whole tracked trees. Only what pret would commit passes: editable build
//! inputs, source and tooling, never presentation material made from the game.
use psynergy::assets::image::{indexed_png, PNG_SIGNATURE};
use psynergy::assets::midi::{midi_events, EventBody, MidiEvent};
use psynergy::assets::wav::wav_pcm8;
use std::collections::BTreeSet;
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
/// Code a game without an asset manifest may track under its asset roots.
const MANIFESTLESS_EXTENSIONS: &[&str] = &["c", "h", "inc", "gitkeep"];
/// Tooling metadata areas the former layout kept under `games/<game>/`; every
/// other directory there is an asset root. Reconstruction metadata now lives
/// under `recon/<id>/`, outside the game tree; these names still classify
/// outgoing history written before that move.
const METADATA_DIRECTORIES: &[&str] = &["metrics", "preview", "recon", "semantic"];
/// Structured tables the asset build reads, where long numeric arrays are data.
const DATA_TABLE_EXTENSIONS: &[&str] = &["json", "tsv"];
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
/// Digest-sized hex runs one text may hold; private-inputs.json carries about 3,400.
const DIGEST_RUNS_MAX: usize = 16_384;
/// Consecutive integer literals that form an array rather than an expression.
const NUMERIC_RUN_MIN: usize = 16;
/// Array elements a text outside the game data tables may hold; the tree peaks
/// near 720 in the executable-gap package.
const NUMERIC_ELEMENTS_MAX: usize = 2_048;
/// Byte values one flat JSON array may hold before only a named typed table
/// explains it; the tracked tree peaks at 518 in `action_modes`.
const JSON_BYTE_ARRAY_MIN: usize = 256;
/// Element types of a typed table segment, as `build_assets::typed_table` reads them.
const TYPED_ELEMENTS: &[&str] = &[
    "u8",
    "s8",
    "le-u16",
    "le-s16",
    "le-u32",
    "le-s32",
    "ascii-fixed",
    "ascii-pool",
    "pool-pointer",
    "thumb-pointer",
    "record",
];
/// Words that mark bytes nobody has explained yet, in a name or a label.
const UNEXPLAINED_WORDS: &[&str] = &[
    "blob",
    "dump",
    "opaque",
    "pending",
    "raw",
    "residual",
    "unclassified",
    "unidentified",
    "unknown",
    "unreferenced",
    "unresolved",
];
/// Words that name a container rather than what it holds.
const CONTAINER_WORDS: &[&str] = &[
    "array", "block", "buffer", "byte", "bytes", "chunk", "data", "region", "segment", "span",
    "storage", "table", "values",
];
/// Kinds and representations that store copied or decoded bytes, not a source form.
const BYTE_COPY_LABELS: &[&str] = &[
    "byte-values",
    "decoded-byte-streams",
    "integer-region-package",
    "integer-regions",
];
/// Label words for bytes that are a stream or a fill rather than a table.
const STREAM_LABEL_WORDS: &[&str] = &["fill", "padding", "stream", "streams"];
/// Labels of a JSON object that say what kind of bytes it holds.
const BYTE_LABEL_FIELDS: &[&str] = &["kind", "representation", "role", "source_kind"];
/// State words for bytes whose content still awaits verification. A state of
/// `typed_values_pending` names typed values whose meaning is still open, and
/// is not a dump.
const UNVERIFIED_STATE_WORDS: &[&str] = &["required", "unverified"];
const JSON_BYTE_DUMP_REASON: &str = "byte dump in JSON: copied bytes, a stream, fill or unexplained byte run, or 256 or more byte values outside the named values of a typed table; decode them into a source form or register a private input";
const JSON_UNPARSED_REASON: &str = "tracked JSON does not parse, so its numbers cannot be measured";
const INTEGER_SUFFIXES: &[&str] = &[
    "usize", "isize", "u128", "i128", "u64", "i64", "u32", "i32", "u16", "i16", "u8", "i8", "ull",
    "llu", "ul", "lu", "ll", "u", "l",
];
/// Where the cartridge logo sits in a GBA header.
const LOGO: std::ops::Range<usize> = 0x04..0xa0;
const LOGO_REASON: &str = "Nintendo logo from a GBA cartridge header: ROM material";
const UNREGISTERED: &str = "unregistered binary: pret commits only editable build inputs";
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
const REPORT_EXTENSIONS: &[&str] = &["csv", "json", "jsonl", "log", "tsv", "txt"];
const REPORT_WORDS: &[&str] = &["analysis", "comparison", "diff", "dump", "report"];
const MARKER_EXTENSIONS: &[&str] = &[
    "md", "ts", "js", "json", "sh", "c", "h", "s", "asm", "tsv", "txt",
];
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
    if directories.iter().any(|directory| {
        listed(directory, BLOCKED_DIRECTORIES)
            || directory.to_ascii_lowercase().starts_with(".cmatch")
    }) {
        return Some("private or generated directory");
    }
    let lower_leaf = leaf.to_ascii_lowercase();
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
    if listed(suffix, PRESENTATION_EXTENSIONS) && normalized != "PROGRESS.svg" {
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
            "PREVIEW material belongs under ignored out/; the README figure is root PROGRESS.svg",
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
/// These are raw .text section bytes produced by score/allocator.rs, not notes.
fn allocator_section(path: &Path) -> bool {
    path.parent()
        .and_then(Path::file_name)
        .is_some_and(|name| name == "allocator-order")
        && path
            .file_name()
            .is_some_and(|name| name == "normal.text" || name == "diagnostic.text")
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
                || (relative.starts_with("out/") && allocator_section(&path))
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
fn incbin(data: &[u8]) -> bool {
    let text = String::from_utf8_lossy(data);
    text.split(['\n', '\r']).any(|line| {
        let trimmed = line.trim_start_matches(|ch: char| ch.is_whitespace() || ch == '\u{feff}');
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
    if scanlines.chunks(row).any(|line| line[0] > 4) {
        return None;
    }
    let image = indexed_png(data).ok()?;
    Some(
        image
            .pixels
            .chunks(8 / usize::from(depth))
            .map(|group| group.iter().fold(0u32, |byte, pixel| byte << depth | pixel) as u8)
            .collect(),
    )
}
/// A standard MIDI file exactly as the sequence build reads it: MThd then only
/// MTrk chunks covering the file, every track closed by end-of-track, text
/// meta events as text and the rest at their specified sizes, and no
/// system-exclusive payloads.
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
    data_uri_reason(&text).or_else(|| encoded_reason(&text, false))
}
/// The binary build inputs a game may track, each parsed exactly as the asset
/// build reads it; everything else is text.
fn binary_reason(path: &str, data: &[u8], logo: Option<&[u8]>) -> Option<&'static str> {
    if data.is_empty() {
        return None;
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
/// Lower-case alphanumeric words of a name or label.
fn label_words(label: &str) -> impl Iterator<Item = String> + '_ {
    label
        .split(|ch: char| !ch.is_ascii_alphanumeric())
        .filter(|word| !word.is_empty())
        .map(str::to_ascii_lowercase)
}
/// A name that says what its values mean: no word marks the bytes as
/// unexplained, and one word of three or more letters is not a container,
/// an address or a number, as `inventory_counter_slots` and not `residual_001`.
fn semantic_name(name: &str) -> bool {
    let words: Vec<_> = label_words(name).collect();
    !words.iter().any(|word| listed(word, UNEXPLAINED_WORDS))
        && words.iter().any(|word| {
            word.len() >= 3
                && word.bytes().all(|byte| byte.is_ascii_alphabetic())
                && !listed(word, CONTAINER_WORDS)
        })
}
type JsonObject = serde_json::Map<String, serde_json::Value>;
fn labels(object: &JsonObject) -> impl Iterator<Item = &str> {
    BYTE_LABEL_FIELDS
        .iter()
        .filter_map(|field| object.get(*field).and_then(serde_json::Value::as_str))
}
/// An object whose kind or representation stores copied or decoded bytes
/// instead of a source form, as `decoded-byte-streams` or `byte_values`.
fn byte_copy_label(object: &JsonObject) -> bool {
    labels(object).any(|label| {
        let label = label_words(label).collect::<Vec<_>>().join("-");
        listed(&label, BYTE_COPY_LABELS)
    })
}
/// An object whose labels say its bytes are a stream, fill or unexplained,
/// as `general_lz_stream` or `unresolved_fill`, or whose source state says
/// their content still awaits verification, as `fill_verification_required`.
fn unfinished_bytes_label(object: &JsonObject) -> bool {
    let unverified = object
        .get("source_state")
        .and_then(serde_json::Value::as_str)
        .is_some_and(|state| label_words(state).any(|word| listed(&word, UNVERIFIED_STATE_WORDS)));
    unverified
        || labels(object).any(|label| {
            label_words(label)
                .any(|word| listed(&word, UNEXPLAINED_WORDS) || listed(&word, STREAM_LABEL_WORDS))
        })
}
/// A key that is a content digest, as in a map of streams keyed by hash.
fn digest_key(key: &str) -> bool {
    key.len() >= 16 && key.bytes().all(|byte| byte.is_ascii_hexdigit())
}
/// A whole number from 0 to 255, however it is written.
fn byte_value(value: &serde_json::Value) -> bool {
    value
        .as_f64()
        .is_some_and(|number| number.fract() == 0.0 && (0.0..=255.0).contains(&number))
}
/// Whether an array under `key` is the `values` of a typed table segment
/// whose element type and semantic name give it structure, reached through
/// no digest key.
fn typed_table_values(key: Option<&str>, objects: &[&JsonObject], digest_path: bool) -> bool {
    let Some(segment) = objects.last() else {
        return false;
    };
    let names: Vec<_> = ["name", "id"]
        .iter()
        .filter_map(|field| segment.get(*field).and_then(serde_json::Value::as_str))
        .collect();
    key == Some("values")
        && !digest_path
        && segment
            .get("element")
            .and_then(serde_json::Value::as_str)
            .is_some_and(|element| TYPED_ELEMENTS.contains(&element))
        && !names.is_empty()
        && names
            .iter()
            .all(|name| !label_words(name).any(|word| listed(&word, UNEXPLAINED_WORDS)))
        && names.iter().any(|name| semantic_name(name))
}
/// Whether a JSON value copies bytes instead of explaining them: an object
/// labelled as copied bytes; a flat array of `JSON_BYTE_ARRAY_MIN` or more
/// byte values that is not a typed table's named values; or a byte run of
/// `NUMERIC_RUN_MIN` or more inside an object labelled as a stream, fill or
/// unexplained bytes.
fn copied_bytes<'a>(
    value: &'a serde_json::Value,
    key: Option<&str>,
    objects: &mut Vec<&'a JsonObject>,
    digest_path: bool,
) -> bool {
    match value {
        serde_json::Value::Array(items) => {
            let bytes = items.iter().all(byte_value);
            let untyped = items.len() >= JSON_BYTE_ARRAY_MIN
                && !typed_table_values(key, objects, digest_path);
            let unfinished = items.len() >= NUMERIC_RUN_MIN
                && objects.iter().any(|object| unfinished_bytes_label(object));
            (bytes && (untyped || unfinished))
                || items
                    .iter()
                    .any(|item| copied_bytes(item, None, objects, digest_path))
        }
        serde_json::Value::Object(object) => {
            if byte_copy_label(object) {
                return true;
            }
            objects.push(object);
            let copied = object.iter().any(|(name, item)| {
                copied_bytes(item, Some(name), objects, digest_path || digest_key(name))
            });
            objects.pop();
            copied
        }
        _ => false,
    }
}
/// Saved compression decisions remain answers when nested, split into short
/// arrays, or addressed through a separate binary table.
// Pascal temporarily admitted the already committed compression debt while
// recovery continues. This checkpoint freezes it; it is not a growing allowlist.
const LEGACY_COMPRESSION_BASELINE: &str = "d08ee3a28fc92afe15c6215d0997a05659395f1c";
const LEGACY_COMPRESSION_TABLE: &str =
    "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS";
const COMPRESSION_ANSWER_REASON: &str =
    "stored compression decisions or padding: recover the encoder and packer";

fn computed_controls(tokens: &serde_json::Value) -> bool {
    matches!(tokens["predictor"].as_str(), Some("lzss" | "greedy-lz-v1"))
        && tokens["exceptions"].as_array().is_some_and(Vec::is_empty)
}

fn compression_answers(value: &serde_json::Value, inherited_lz: bool) -> bool {
    use serde_json::Value;
    match value {
        Value::Object(object) => {
            if object
                .get("token_table")
                .is_some_and(|table| table["format"] == "alchemy-lz-controls-v1")
            {
                return true;
            }
            let lz = object
                .get("codec")
                .or_else(|| object.get("recipe_codec"))
                .and_then(Value::as_str)
                .map_or(inherited_lz, |codec| codec.contains("-lz"));
            if lz {
                if object.get("lookahead").is_some_and(|padding| padding != "") {
                    return true;
                }
                if let Some(tokens) = object.get("tokens") {
                    // New compressors normally omit controls altogether.
                    if !computed_controls(tokens) {
                        return true;
                    }
                }
            }
            object.values().any(|child| compression_answers(child, lz))
        }
        Value::Array(rows) => rows
            .iter()
            .any(|child| compression_answers(child, inherited_lz)),
        _ => false,
    }
}
/// Byte dumps stored as JSON numbers, which the text measures cannot see
/// inside game data tables: decoded streams, residual regions, hash-keyed
/// stream maps and any other long flat byte array without a typed table.
fn json_byte_dump_reason(path: &str, text: &str) -> Option<&'static str> {
    if !extension(path).eq_ignore_ascii_case("json") {
        return None;
    }
    let Ok(document) = serde_json::from_str::<serde_json::Value>(text) else {
        return Some(JSON_UNPARSED_REASON);
    };
    if compression_answers(&document, false) {
        return Some(COMPRESSION_ANSWER_REASON);
    }
    copied_bytes(&document, None, &mut Vec::new(), false).then_some(JSON_BYTE_DUMP_REASON)
}

/// Remove only frozen legacy fields from the document used for further checks.
/// Real build inputs stay unchanged; unrelated payloads still fail publication.
fn legacy_compression_projection(
    before: &serde_json::Value,
    after: &mut serde_json::Value,
    old_codec: &str,
    new_codec: &str,
) -> bool {
    use serde_json::Value;
    match after {
        Value::Object(object) => {
            let new_codec = object
                .get("codec")
                .or_else(|| object.get("recipe_codec"))
                .and_then(Value::as_str)
                .unwrap_or(new_codec)
                .to_string();
            let old_codec = before
                .get("codec")
                .or_else(|| before.get("recipe_codec"))
                .and_then(Value::as_str)
                .unwrap_or(old_codec);
            for key in ["token_table", "tokens", "lookahead"] {
                let Some(value) = object.get(key) else {
                    continue;
                };
                let debt = match key {
                    "token_table" => value["format"] == "alchemy-lz-controls-v1",
                    "tokens" => new_codec.contains("-lz") && !computed_controls(value),
                    "lookahead" => new_codec.contains("-lz") && value != "",
                    _ => false,
                };
                if debt {
                    if before.get(key) != Some(value)
                        || (key != "token_table" && old_codec != new_codec)
                    {
                        return false;
                    }
                    object.remove(key);
                }
            }
            object.iter_mut().all(|(key, value)| {
                legacy_compression_projection(&before[key], value, old_codec, &new_codec)
            })
        }
        Value::Array(rows) => rows.iter_mut().enumerate().all(|(index, value)| {
            legacy_compression_projection(&before[index], value, old_codec, new_codec)
        }),
        _ => true,
    }
}

fn publication_data_reason_with_legacy(
    root: &Path,
    path: &str,
    data: &[u8],
    logo: Option<&[u8]>,
) -> Option<&'static str> {
    let reason = publication_data_reason(path, data, logo)?;
    if reason != COMPRESSION_ANSWER_REASON && path != LEGACY_COMPRESSION_TABLE {
        return Some(reason);
    }
    let object = format!("{LEGACY_COMPRESSION_BASELINE}:{path}");
    let Ok(before) = git(root, &["show", &object], "frozen compression debt") else {
        return Some(reason);
    };
    if path == LEGACY_COMPRESSION_TABLE {
        return (data != before).then_some(reason);
    }
    let (Ok(before), Ok(mut after)) = (
        serde_json::from_slice::<serde_json::Value>(&before),
        serde_json::from_slice::<serde_json::Value>(data),
    ) else {
        return Some(reason);
    };
    if !legacy_compression_projection(&before, &mut after, "", "") {
        return Some(reason);
    }
    let cleaned = serde_json::to_vec(&after).expect("JSON value serializes");
    publication_data_reason(path, &cleaned, logo)
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
    if logo.is_some_and(|logo| contains(data, logo)) {
        return Some(LOGO_REASON);
    }
    if listed(extension(path), &["asm", "inc", "s"]) && incbin(data) {
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
    let table = asset_game(path).is_some() && listed(extension(path), DATA_TABLE_EXTENSIONS);
    license_reason(text)
        .or_else(|| diff_reason(text))
        .or_else(|| data_uri_reason(text))
        .or_else(|| encoded_reason(text, !table))
        .or_else(|| json_byte_dump_reason(path, text))
        .or_else(|| included_bytes_reason(path, text))
        .or_else(|| attributes_reason(path, text))
        .or_else(|| runtime_definition_reason(path, text))
}
/// Games whose asset manifest is tracked in the inspected tree, named by
/// their `games/` directory: `recon/<id>/assets.json`, or the former
/// `games/<game>/recon/assets.json` that outgoing history may still hold.
fn manifest_games<'a>(paths: impl IntoIterator<Item = &'a str>) -> Vec<String> {
    paths
        .into_iter()
        .filter_map(
            |path| match path.split('/').collect::<Vec<_>>().as_slice() {
                ["recon", id, "assets.json"] => {
                    Some(crate::compiler::routing::game_directory(id).to_string())
                }
                ["games", game, "recon", "assets.json"] => Some(game.to_string()),
                _ => None,
            },
        )
        .collect()
}
/// The game whose asset roots hold `path`: every directory under
/// `games/<game>/` except tooling metadata, with `asm/overlays` holding
/// overlay streams, all matched without regard to case. Retained listings
/// and the data packages the asset build reads beside them stay an asset
/// root of their game under `recon/<id>/raw`.
fn asset_game(path: &str) -> Option<&str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, game, area, rest @ ..] = components.as_slice() else {
        return None;
    };
    if top.eq_ignore_ascii_case("recon") {
        return (area.eq_ignore_ascii_case("raw") && !rest.is_empty())
            .then(|| crate::compiler::routing::game_directory(*game));
    }
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
/// The shared root holds only nested C source and interface headers that every
/// game compiles byte-exact from the same text; assets and metadata stay in a
/// game.
fn shared_root_reason(path: &str) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let [top, root, rest @ ..] = components.as_slice() else {
        return None;
    };
    if !top.eq_ignore_ascii_case("games") || !root.eq_ignore_ascii_case("COMMON") {
        return None;
    }
    let source = matches!(rest, ["SRC", _, .., leaf] if extension(leaf) == "C");
    let interface = matches!(rest, ["INCLUDE", _, .., leaf] if extension(leaf) == "H");
    (!(source || interface))
        .then_some("games/COMMON holds only shared SRC/<module>/*.C and INCLUDE/<module>/*.H")
}
fn manifestless_reason(path: &str, manifests: &[String]) -> Option<&'static str> {
    if let Some(reason) = shared_root_reason(path) {
        return Some(reason);
    }
    let game = asset_game(path)?;
    // A manifest covers only the exact spelling of the tree it was read from.
    let manifested = (path.starts_with("games/") || path.starts_with("recon/"))
        && manifests.iter().any(|known| known == game);
    let code = listed(extension(path), MANIFESTLESS_EXTENSIONS);
    (!code && !manifested)
        .then_some("game material without a consuming asset manifest (recon/<game>/assets.json)")
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
}
/// One inspected path. Approved compiler submodules carry no blob to read;
/// any other gitlink fails without being read.
fn inspected(
    scope: &str,
    path: String,
    object: String,
    gitlink: bool,
    manifests: &[String],
) -> Option<Entry> {
    let listing_reason = if gitlink {
        if APPROVED_GITLINKS.contains(&path.as_str()) {
            return None;
        }
        Some("unapproved gitlink: only the agbcc and agscc submodules are approved")
    } else {
        manifestless_reason(&path, manifests)
    };
    Some(Entry {
        scope: scope.to_string(),
        path,
        object,
        listing_reason,
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
        let reason = publication_data_reason_with_legacy(root, &entry.path, data, logo.as_deref())
            .map(str::to_string);
        let reason = reason.or_else(|| {
            conflicts
                .then(|| conflict_marker_reason(&entry.path, data))
                .flatten()
        });
        if let Some(reason) = reason {
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
fn manifests_of(root: &Path, revision: Option<&str>) -> Result<Vec<String>, String> {
    let records = tracked(root, revision)?;
    Ok(manifest_games(
        records.iter().map(|(_, _, path)| path.as_str()),
    ))
}
fn tree_entries(root: &Path, revision: Option<&str>) -> Result<Vec<Entry>, String> {
    let records = tracked(root, revision)?;
    let manifests = manifest_games(records.iter().map(|(_, _, path)| path.as_str()));
    let scope = revision.map_or("tree".to_string(), |revision| {
        format!("tree {}", &revision[..12.min(revision.len())])
    });
    Ok(records
        .into_iter()
        .filter_map(|(gitlink, object, path)| inspected(&scope, path, object, gitlink, &manifests))
        .collect())
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
    let manifests = manifests_of(root, None)?;
    let entries = changes
        .into_iter()
        .filter_map(|(path, gitlink)| {
            let object = format!(":{path}");
            inspected("staged", path, object, gitlink, &manifests)
        })
        .collect();
    scan(root, entries, true)
}
fn revisions(root: &Path, local: &str, remote: &str) -> Result<Vec<String>, String> {
    let excluded = format!("^{remote}");
    let mut args = vec!["rev-list", local];
    if !remote.bytes().all(|byte| byte == b'0') {
        args.push(&excluded);
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
    let source = if listed(extension(path), &["c", "h"]) {
        c
    } else {
        assembly
    };
    source
        .is_match(text)
        .then_some("compiler runtime routine: build it from its licensed container")
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
/// `out/history-audit.json` with every finding the history rewrite removes.
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
            messages.push(serde_json::json!({"commit": commit, "reason": reason}));
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
        let manifests = manifests_of(root, Some(commit))?;
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
            if let Some(entry) =
                inspected(commit, path, object, metadata[1] == "160000", &manifests)
            {
                entries.push(entry);
            }
        }
    }
    let logo = nintendo_logo(root);
    let mut files = Vec::new();
    let mut readable = Vec::new();
    for entry in entries {
        match publication_path_reason(&entry.path).or(entry.listing_reason) {
            Some(reason) => files.push(serde_json::json!({
                "commit": entry.scope, "path": entry.path, "blob": entry.object, "reason": reason
            })),
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
            files.push(serde_json::json!({
                "commit": entry.scope, "path": entry.path, "blob": entry.object, "reason": reason
            }));
        }
    })?;
    let report = serde_json::json!({
        "format": 1,
        "commits": commits.len(),
        "file_versions": seen.len(),
        "files": files,
        "messages": messages,
    });
    let path = root.join("out/history-audit.json");
    std::fs::create_dir_all(root.join("out")).map_err(|error| error.to_string())?;
    std::fs::write(
        &path,
        serde_json::to_string_pretty(&report).map_err(|e| e.to_string())? + "\n",
    )
    .map_err(|error| error.to_string())?;
    println!(
        "history-audit commits={} file_versions={} files={} messages={} report=out/history-audit.json",
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
        let manifests = manifests_of(root, Some(&commit))?;
        let scope = &commit[..12.min(commit.len())];
        entries.extend(changes.into_iter().filter_map(|(path, gitlink)| {
            let object = format!("{commit}:{path}");
            inspected(scope, path, object, gitlink, &manifests)
        }));
    }
    // Each pushed tip must also pass as a whole tree, not only as its deltas.
    for tip in tips {
        entries.extend(tree_entries(root, Some(&tip))?);
    }
    scan(root, entries, false)
}
/// Every file-level decision for one blob, as `scan` makes it.
fn publication_reason(
    path: &str,
    data: &[u8],
    manifests: &[String],
    logo: Option<&[u8]>,
) -> Option<&'static str> {
    publication_path_reason(path)
        .or_else(|| manifestless_reason(path, manifests))
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
    let palette: Vec<u8> = (0..3usize << depth)
        .map(|index| (index / 3) as u8)
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
/// `(path, bytes, game has a manifest, expected rejection reason fragment)`.
type Fixture = (&'static str, Vec<u8>, bool, Option<&'static str>);
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
            true,
            Some("presentation material"),
        ),
        ("notes.dat", font, true, Some("font")),
        (
            "x.PNG",
            b"GIF89a\x10\0\x10\0\x80\0\0".to_vec(),
            true,
            Some("GIF image"),
        ),
        (
            "games/THE BROKEN SEAL/PREVIEW/DJINN_101_IDLE.GIF",
            b"GIF89a".to_vec(),
            true,
            Some("presentation material"),
        ),
        (
            "games/THE BROKEN SEAL/PREVIEW/title.png",
            indexed_fixture(4),
            true,
            Some("PREVIEW"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SHOT.PNG",
            png_fixture(16, 8, 6, &stream, &[]),
            true,
            Some("truecolour"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/IDLE.INDEXED.PNG",
            png_fixture(16, 4, 3, &stream, &[(b"acTL", &[0, 0, 0, 2, 0, 0, 0, 0])]),
            true,
            Some("animated PNG"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/TABLE.DAT",
            b"\x01\x02\0\x03".to_vec(),
            true,
            unregistered,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE_00.PCM4",
            vec![0x5a; 65],
            true,
            unregistered,
        ),
        (
            "tools/alchemy/tests/tone.wav",
            wave.clone(),
            true,
            unregistered,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/X.PNG",
            indexed_fixture(4),
            true,
            unregistered,
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/HEADER.DAT",
            fragment,
            true,
            Some("ROM header fragment"),
        ),
        (
            "games/THE LOST AGE/SOUND/SEQUENCE/X.MID",
            midi.clone(),
            false,
            Some("asset manifest"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/TRAILING.INDEXED.PNG",
            trailing,
            true,
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/TEXT.INDEXED.PNG",
            png_fixture(16, 4, 3, &stream, &[(b"tEXt", b"Comment\0payload")]),
            true,
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SURPLUS.INDEXED.PNG",
            png_fixture(16, 4, 3, &surplus, &[]),
            true,
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SECOND.INDEXED.PNG",
            png_fixture(16, 4, 3, &second, &[]),
            true,
            malformed_png,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/LOGO.INDEXED.PNG",
            png_fixture(160, 8, 3, &hidden, &[]),
            true,
            nintendo,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LONG.PCM8.WAV",
            [wave.as_slice(), &[0]].concat(),
            true,
            wav,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LIST.PCM8.WAV",
            listed_wave,
            true,
            wav,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/LOGO.PCM8.WAV",
            psynergy::assets::wav::pcm8_wav(&samples, 8000).unwrap(),
            true,
            nintendo,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/CHUNK.MID",
            [midi.as_slice(), b"XXXX\0\0\0\x01\0"].concat(),
            true,
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/SYSEX.MID",
            midi_fixture(&[0, 0xf0, 3, 1, 2, 0xf7], true),
            true,
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/BINARY.MID",
            meta(0x7f, &[0xff, 0xfe, 0x80]),
            true,
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/OPEN.MID",
            midi_fixture(&[], false),
            true,
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/TEMPO.MID",
            meta(0x51, &fixture_bytes(64, 3)),
            true,
            sequence,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/TIMED.MID",
            meta(0x51, &[7, 0xa1, 0x20]),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/PAYLOAD.MID",
            meta(0x01, encoded_fixture(&base64, 600, 2).as_bytes()),
            true,
            Some("encoded payload"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/SPARE.TOKENS",
            table.clone(),
            true,
            unregistered,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS",
            garbled,
            true,
            unregistered,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/FONT/LOCALIZATION_GLYPHS_0020_00FF.1BPP.PNG",
            indexed_fixture(1),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/UI_MTF_00.INDEXED.PNG",
            indexed_fixture(4),
            true,
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
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/TEXT/STAFF_ROLL_MOJI.1BPP.PNG",
            indexed_fixture(1),
            true,
            None,
        ),
        (
            "games/THE LOST AGE/SOUND/SAMPLE/WAVE_00.PCM8.WAV",
            wave,
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/THEME.MID",
            meta(0x7f, &directive),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE_00.PCM4",
            vec![0x5a; 16],
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/COMPRESSION.TOKENS",
            table,
            true,
            unregistered,
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
    let manifest = Some("asset manifest");
    let include = Some("include_bytes! or include_str!");
    let uri = Some("data URI");
    vec![
        (
            "PROGRESS.svg",
            svg_uri,
            true,
            uri,
        ),
        ("tools/alchemy/src/dashboard/index.html", css_uri, true, uri),
        ("tools/alchemy/src/coverage/figure.rs", rust_uri, true, uri),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/BLOB.JSON",
            json_base64,
            true,
            encoded,
        ),
        (
            "tools/alchemy/src/dashboard/font.css",
            text(lines(76, 5)),
            true,
            encoded,
        ),
        (
            "tools/alchemy/src/dashboard/glyphs.css",
            text(lines(40, 12)),
            true,
            encoded,
        ),
        ("tools/alchemy/src/assets.rs", quoted, true, encoded),
        ("tools/alchemy/src/key.rs", base32_key, true, encoded),
        ("tools/alchemy/src/blob.json", hex_blob, true, encoded),
        (
            "tools/alchemy/src/words.rs",
            text(digests(17_000)),
            true,
            Some("digest-sized"),
        ),
        ("tools/alchemy/src/rom_table.rs", array, true, arrays),
        ("tools/alchemy/src/rom_bytes.rs", escapes, true, arrays),
        ("tools/alchemy/src/rom_dump.rs", dump, true, arrays),
        (
            "games/THE BROKEN SEAL/SRC/BATTLE/TABLE.C",
            c_table,
            true,
            arrays,
        ),
        (
            "games/THE BROKEN SEAL/SRC/BATTLE/DATA/TABLE.JSON",
            json_table,
            true,
            Some("byte dump in JSON"),
        ),
        (
            "games/THE LOST AGE/DATA/TABLE.JSON",
            empty(),
            false,
            manifest,
        ),
        (
            "games/THE LOST AGE/src/battle/table.json",
            empty(),
            false,
            manifest,
        ),
        (
            "Games/THE BROKEN SEAL/SRC/TABLE.JSON",
            empty(),
            true,
            manifest,
        ),
        ("tools/alchemy/src/logo.rs", logo_include, true, include),
        ("tools/alchemy/src/header.rs", header_include, true, include),
        (
            "tools/alchemy/src/recovery/fixture.rs",
            source_include,
            true,
            None,
        ),
        (
            "tools/alchemy/assets/figure.png",
            text(pointer),
            true,
            Some("Git LFS pointer"),
        ),
        (".gitattributes", attributes, true, Some("filter attribute")),
        (".gitattributes", b"*.TOKENS binary\n".to_vec(), true, None),
        (
            "recon/tbs/en/main/0800ebec.c.bak",
            b"int x;\n".to_vec(),
            true,
            Some("backup"),
        ),
        (
            "recon/tla/raw/overlays/.gitkeep",
            Vec::new(),
            false,
            None,
        ),
        (
            "games/THE LOST AGE/SRC/MAIN/X.C",
            b"void f(void) {}\n".to_vec(),
            false,
            None,
        ),
        (
            "games/COMMON/SRC/SOUND/X.C",
            b"void f(void) {}\n".to_vec(),
            false,
            None,
        ),
        (
            "games/COMMON/SRC/X.C",
            b"void f(void) {}\n".to_vec(),
            true,
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/SRC/SOUND/X.H",
            b"void f(void);\n".to_vec(),
            true,
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/INCLUDE/SOUND/X.H",
            b"void f(void);\n".to_vec(),
            false,
            None,
        ),
        (
            "games/COMMON/SRC/SOUND/TABLE.JSON",
            empty(),
            true,
            Some("games/COMMON holds only"),
        ),
        (
            "games/COMMON/recon/translation-units.json",
            empty(),
            true,
            Some("games/COMMON holds only"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/FIELD/SCENE/SCENE.C",
            text(identifiers),
            true,
            None,
        ),
        ("tools/Cargo.lock", text(checksums), true, None),
        (
            "tools/alchemy/src/hashes.rs",
            text(digests(4_096)),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/ROM_HEADER.JSON",
            text(rows),
            true,
            None,
        ),
        (
            "tools/alchemy/src/dashboard/style.css",
            b"body { font: 16px mono; }\n".to_vec(),
            true,
            None,
        ),
        (
            "PROGRESS.svg",
            svg,
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/INCLUDE/ADD_PARTS_BODY.INC",
            text(license_header),
            true,
            license,
        ),
        (
            "recon/tbs/raw/080072e4.s",
            text(wrapped_license),
            true,
            license,
        ),
        ("tools/alchemy/src/runtime.rs", text(lesser_license), true, license),
        (
            "games/THE BROKEN SEAL/SRC/LIB/SOFT_FLOAT.C",
            text(copyright),
            true,
            license,
        ),
        (
            "recon/tbs/notes.json",
            text(unified),
            true,
            patch,
        ),
        ("AGENTS.md", text(headerless), true, patch),
        ("tools/alchemy/src/compiler.rs", text(context), true, patch),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/BUILD.INC",
            text(binary_patch),
            true,
            patch,
        ),
        (
            "AGENTS.md",
            text(format!(
                "Code whose license this repository cannot carry stays in the {} submodules; {}{} marks a hunk.\n",
                "licensed", "@", "@"
            )),
            true,
            None,
        ),
        (
            "tools/alchemy/src/score/fixture.rs",
            text(format!(
                "write(&patch, \"{a}{a} -1 +1 {a}{a}\\n-a\\n+b\\n\");\n{a}{a} -1 +1 {a}{a}\nnot a body\n",
                a = '@'
            )),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/FIELD/FOUNDATION.C",
            text(format!(
                "/* {} of the {} */\nvoid Found(void) {{}}\n",
                "the free software",
                FOUNDATION.replace('|', "")
            )),
            true,
            None,
        ),
    ]
}
/// Typed tables that explain their bytes, and byte dumps in the shapes the
/// tree once carried: decoded stream maps keyed by hash, residual byte-value
/// regions, streams, fills, bytes awaiting verification and bare arrays.
/// Every array is built here, so this source carries none of them.
fn json_fixtures() -> Vec<Fixture> {
    use serde_json::{json, Value};
    const DATABASE: &str = "games/THE BROKEN SEAL/SRC/GAME/GAMEPLAY_DATABASES.JSON";
    const CATALOG: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/TABLES.JSON";
    const STAFF: &str = "games/THE BROKEN SEAL/TEXT/STAFF_ROLL_INDEX.JSON";
    const STREAMS: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/DATA.JSON";
    const RESIDUAL: &str = "games/THE BROKEN SEAL/SRC/SYSTEM/FINAL_BYTE_REGIONS_INDEX.JSON";
    const RUNTIME: &str = "games/THE BROKEN SEAL/SRC/SYSTEM/EARLY_RUNTIME_INDEX.JSON";
    let bytes = |length: usize| json!(fixture_bytes(length, 9));
    let document = |value: Value| format!("{value:#}\n").into_bytes();
    // A typed segment labelled `field: label` with `extra` fields merged in.
    let segment = |field: &str, label: &str, element: &str, length: usize, extra: Value| {
        let mut segment = json!({"element": element, "stride": 1, "values": bytes(length)});
        segment[field] = json!(label);
        for (key, value) in extra.as_object().into_iter().flatten() {
            segment[key] = value.clone();
        }
        segment
    };
    let table = |segments: Vec<Value>| {
        document(json!({"format": 1, "kind": "typed-table", "segments": segments}))
    };
    let named =
        |name: &str, length: usize| table(vec![segment("name", name, "u8", length, json!({}))]);
    let catalog = document(
        json!({"format": 1, "kind": "typed-table-catalog", "tables": {
        "0x080c2a0a": {"format": 1, "kind": "typed-table", "segments": [
            segment("name", "action_modes", "u8", 518, json!({"min": 0, "max": 9})),
            segment("name", "action_display", "u8", 300, json!({}))]}}}),
    );
    let digest = |seed: u64| format!("{:064x}", u128::from(seed + 1) * 0x9e37_79b9_7f4a_7c15);
    let streams: serde_json::Map<_, _> = (0..3).map(|seed| (digest(seed), bytes(768))).collect();
    let decoded = json!({"format": 1, "kind": "decoded-byte-streams", "streams": streams});
    let hashed: serde_json::Map<_, _> = [(
        digest(7),
        segment("name", "title_tiles", "u8", 300, json!({})),
    )]
    .into_iter()
    .collect();
    let residual = json!({"format": 1, "regions": [{"name": "residual_001",
        "representation": "byte_values", "values": bytes(1916)}]});
    // Typed rows whose meaning is still open, as grid_lookup_a is today.
    let rows: Vec<Vec<u8>> = fixture_bytes(256, 5)
        .chunks(16)
        .map(<[u8]>::to_vec)
        .collect();
    let pending = json!({"id": "grid_lookup_a", "element": "u8", "stride": 16,
        "source_state": "typed_values_pending", "values": rows});
    let required = segment(
        "id",
        "lookup_storage",
        "u8",
        4036,
        json!({"source_state": "private_content_and_fill_verification_required"}),
    );
    let stream = segment(
        "name",
        "haikei_stream",
        "u8",
        239,
        json!({"role": "compressed_background", "source_kind": "general_lz_stream"}),
    );
    let fill = segment(
        "name",
        "aki_080f53ce",
        "u8",
        76,
        json!({"source_kind": "unresolved_fill"}),
    );
    let envelope = segment(
        "name",
        "henka_hyou",
        "u8",
        60,
        json!({"role": "envelope_table"}),
    );
    let package = json!({"format": 1, "regions": [{"kind": "integer-region-package",
        "address": "0x08001b70", "index": "FINAL_BYTE_REGIONS_INDEX.JSON"}]});
    let floats: Vec<f64> = fixture_bytes(256, 9).into_iter().map(f64::from).collect();
    // Whole numbers written as floats are still bytes.
    let floats = json!({"segments": [{"name": "title_tiles", "values": floats}]});
    let dump = Some("byte dump in JSON");
    vec![
        (DATABASE, named("inventory_counter_slots", 512), true, None),
        (CATALOG, catalog, true, None),
        (
            STAFF,
            table(vec![segment("id", "lines", "pool-pointer", 339, json!({}))]),
            true,
            None,
        ),
        (
            "tools/alchemy/src/slots.json",
            named("inventory_counter_slots", 512),
            true,
            None,
        ),
        (DATABASE, named("residual_001", 255), true, None),
        (DATABASE, named("residual_001", 256), true, dump),
        (DATABASE, named("unreferenced_storage", 512), true, dump),
        (DATABASE, named("region_0807b490", 512), true, dump),
        (DATABASE, named("pending_slots", 512), true, dump),
        (
            DATABASE,
            table(vec![segment(
                "name",
                "inventory_counter_slots",
                "bytes",
                512,
                json!({}),
            )]),
            true,
            dump,
        ),
        (
            DATABASE,
            document(
                json!({"name": "inventory_counter_slots", "element": "u8", "bytes": bytes(512)}),
            ),
            true,
            dump,
        ),
        (DATABASE, document(floats), true, dump),
        (STREAMS, document(decoded), true, dump),
        (
            STREAMS,
            document(json!({"format": 1, "streams": hashed})),
            true,
            dump,
        ),
        (RESIDUAL, document(residual), true, dump),
        (RUNTIME, table(vec![pending]), true, None),
        (RUNTIME, table(vec![required]), true, dump),
        (RUNTIME, table(vec![stream]), true, dump),
        (RUNTIME, table(vec![fill]), true, dump),
        (RUNTIME, table(vec![envelope]), true, None),
        ("recon/tbs/assets.json", document(package), true, dump),
        (
            "tools/alchemy/src/rom.json",
            document(bytes(256)),
            true,
            dump,
        ),
        (
            DATABASE,
            b"{\"values\": [1, 2,]}\n".to_vec(),
            true,
            Some("does not parse"),
        ),
    ]
}
fn check_fixtures() -> Result<(), String> {
    let tbs = vec!["THE BROKEN SEAL".to_string()];
    let both = vec!["THE BROKEN SEAL".to_string(), "THE LOST AGE".to_string()];
    let logo = logo_fixture();
    let fixtures = binary_fixtures()
        .into_iter()
        .chain(text_fixtures())
        .chain(json_fixtures());
    for (path, data, manifested, expected) in fixtures {
        let manifests = if manifested { &both } else { &tbs };
        let actual = publication_reason(path, &data, manifests, Some(&logo));
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
        "recon/tbs/raw/080000c0.s~",
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
    for path in [
        "src/main.c",
        "README.md",
        "AGENTS.md",
        "recon/tbs/raw/080000c0.s",
        "PROGRESS.svg",
        "games/THE BROKEN SEAL/SOUND/SEQUENCE/THEME.mid",
        "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE.wav",
        "recon/tbs/assets.json",
        "tools/compare-roms/src/main.rs",
        "tools/alchemy/src/build_full.rs",
        "games/THE BROKEN SEAL/SRC/SYSTEM/BUILD_STAMP.JSON",
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
const USAGE: &str = "usage: check publication [--documents | --staged | --pre-push | --tree [REV] | --history [REV] | --self-test]\n\nModes:\n  --documents    Check owned documentation, including ignored output.\n  --history [REV] Check every commit message and file version in history, or reachable from REV; writes out/history-audit.json.\n  --staged       Check staged files before committing.\n  --pre-push     Check outgoing history and each pushed tree using update lines on stdin.\n  --tree [REV]   Check every file tracked in the index, or in revision REV.\n  --self-test    Run the publication gate's internal checks.\n  -h, --help     Show this help.";
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
    use super::*;
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
        assert!(inspected("staged", "agbcc".into(), ":agbcc".into(), true, &[]).is_none());
        let foreign = inspected("staged", "vendor".into(), ":vendor".into(), true, &[]).unwrap();
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
        std::fs::write(root.join("out/allocator-order/normal.text"), [0u8, 1]).unwrap();
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
    fn json_byte_dumps_need_the_named_values_of_a_typed_table() {
        for name in [
            "inventory_counter_slots",
            "action_modes",
            "lines",
            "hyou_a_001",
        ] {
            assert!(semantic_name(name), "{name}");
        }
        for name in [
            "residual_001",
            "unreferenced_storage",
            "region_0807b490",
            "table_values",
            "u8",
            "pending_slots",
        ] {
            assert!(!semantic_name(name), "{name}");
        }
        assert!(digest_key(&"9e".repeat(32)) && !digest_key("0x080c2a0a"));
        // Every JSON fixture is decided by this rule alone.
        for (path, data, _, expected) in json_fixtures() {
            let actual = json_byte_dump_reason(path, std::str::from_utf8(&data).unwrap());
            assert_eq!(actual.is_some(), expected.is_some(), "{path}: {actual:?}");
        }
        assert!(json_byte_dump_reason("games/X/SRC/TABLE.TSV", "not json").is_none());
    }
    #[test]
    fn every_publication_rule_rejects_its_fixture_and_accepts_build_inputs() {
        check_fixtures().unwrap();
        self_test(crate::compiler::routing::root()).unwrap();
    }
    #[test]
    fn reconstruction_scaffolding_keeps_its_game_outside_the_game_tree() {
        assert_eq!(
            manifest_games([
                "recon/tbs/assets.json",
                "recon/tbs/raw/assets.json",
                "games/THE LOST AGE/recon/assets.json",
            ]),
            ["THE BROKEN SEAL", "THE LOST AGE"]
        );
        assert_eq!(
            asset_game("recon/tla/raw/overlays/resource_64a_overlay.s"),
            Some("THE LOST AGE")
        );
        assert_eq!(asset_game("recon/tla/semantic/regions.json"), None);
        assert_eq!(asset_game("recon/tla/translation-units.json"), None);
        let manifests = manifest_games(["recon/tla/assets.json"]);
        assert!(
            manifestless_reason("recon/tla/raw/executable_gaps/index.json", &manifests).is_none()
        );
        assert!(
            manifestless_reason("recon/tbs/raw/executable_gaps/index.json", &manifests).is_some()
        );
        assert!(manifestless_reason("recon/tbs/translation-units.json", &[]).is_none());
    }
    #[test]
    fn frozen_compression_debt_allows_removal_but_not_changed_answers() {
        use serde_json::json;
        let before =
            json!({"codec":"golden-sun-kind2-lz","frames":[{"tokens":[2,[2,4]],"lookahead":"00"}]});
        for mut after in [
            before.clone(),
            json!({"codec":"golden-sun-kind2-lz","frames":[{}]}),
            json!({"codec":"golden-sun-kind2-lz","frames":[{"tokens":{"predictor":"lzss","exceptions":[]}}]}),
        ] {
            assert!(legacy_compression_projection(&before, &mut after, "", ""));
            assert!(!compression_answers(&after, false));
        }
        for mut after in [
            json!({"codec":"golden-sun-kind2-lz","frames":[{"tokens":[3,[2,4]]}]}),
            json!({"codec":"golden-sun-kind2-lz","frames":[{"lookahead":"01"}]}),
            json!({"codec":"golden-sun-general-lz","frames":[{"tokens":[2,[2,4]]}]}),
            json!({"codec":"golden-sun-kind2-lz","frames":[{}, {"tokens":[2,[2,4]]}]}),
        ] {
            assert!(!legacy_compression_projection(&before, &mut after, "", ""));
        }
    }
    #[test]
    fn compression_answers_are_rejected_independently_of_names_and_size() {
        use serde_json::json;
        let codec = "golden-sun-palette-lz";
        for plan in [
            json!({"codec":codec,"tokens":[["g",[["l"],["e"]]]]}),
            json!({"codec":codec,"tokens":{"predictor":"greedy-lz-v1","exceptions":[[1,["c",2,1]]]}}),
            json!({"codec":codec,"tokens":{"offset":8,"size":2,"count":1}}),
            json!({"codec":codec,"lookahead":"00"}),
            json!({"recipe_codec":"golden-sun-arena-lz","recipes":{"bank":[{"tokens":[["l",1]]}]}}),
            json!({"token_table":{"format":"alchemy-lz-controls-v1","source":"renamed.bin"}}),
        ] {
            let nested = json!({"arbitrary":{"items":[plan]}}).to_string();
            for path in [
                "games/X/SRC/WORLD.JSON",
                "games/X/recon/small.json",
                "tools/renamed.json",
            ] {
                assert!(json_byte_dump_reason(path, &nested)
                    .unwrap()
                    .contains("stored compression decisions"));
            }
        }
        for plan in [
            json!({"codec":codec,"decoded_size":1024,"encoded_size":100}),
            json!({"codec":codec,"tokens":{"predictor":"lzss","exceptions":[]}}),
            json!({"codec":codec,"tokens":{"predictor":"greedy-lz-v1","exceptions":[]}}),
            json!({"codec":codec,"lookahead":""}),
            json!({"codec":"language-parser","tokens":["identifier","semicolon"]}),
        ] {
            assert!(json_byte_dump_reason("games/X/SRC/INPUT.JSON", &plan.to_string()).is_none());
        }
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
        let (_, png, _, _) = binary_fixtures()
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
    fn tree_mode_passes_pret_style_inputs_and_reports_only_presentation_material() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"], "fixture git").unwrap();
        let midi = midi_fixture(&[], true);
        let inputs = commit(
            root,
            &[
                ("games/X/recon/assets.json", b"{}\n".to_vec()),
                ("games/X/SRC/GRAPHICS/TILE/A.4BPP.PNG", indexed_fixture(4)),
                ("games/X/SOUND/SEQUENCE/A.MID", midi.clone()),
                ("games/X/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("games/Y/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("PROGRESS.svg", b"<svg/>\n".to_vec()),
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
        let manifestless = commit(
            root,
            &[
                ("games/Y/SOUND/SEQUENCE/A.MID", midi),
                ("games/Y/Data/TABLE.JSON", b"{}\n".to_vec()),
            ],
        );
        let error = check_tree(root, Some(&manifestless)).unwrap_err();
        assert!(error.contains("games/Y/SOUND/SEQUENCE/A.MID: game material without"));
        assert!(error.contains("games/Y/Data/TABLE.JSON: game material without"));
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
        std::fs::write(root.join("gcc.json"), &unified).unwrap();
        run(&["add", "games/X/SRC/LIB/RUNTIME.C", "gcc.json"]);
        let error = check_staged(root).unwrap_err();
        assert!(
            error.contains("staged games/X/SRC/LIB/RUNTIME.C: license marker"),
            "{error}"
        );
        assert!(error.contains("staged gcc.json: patch or diff"), "{error}");
        let error = check_tree(root, None).unwrap_err();
        assert!(error.contains("tree gcc.json: patch or diff"), "{error}");
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
            "gcc.json: patch or diff",
            "runtime.rs: license marker",
            "compiler.rs: patch or diff",
        ] {
            assert!(error.contains(path), "{path}: {error}");
        }
        let update = format!("refs/heads/main {linked} refs/heads/main {base}\n");
        let error = check_push(root, &update).unwrap_err();
        assert!(error.contains("vendor/gcc: unapproved gitlink"), "{error}");
        assert!(error.contains("runtime.rs: license marker"), "{error}");
        assert!(error.contains("gcc.json: patch or diff"), "{error}");
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
