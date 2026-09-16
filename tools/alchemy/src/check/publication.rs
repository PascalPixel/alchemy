//! Fail-closed publication checks for staged changes, outgoing history and
//! whole tracked trees. Only what pret would commit passes: editable build
//! inputs, source and tooling, never presentation material made from the game.
use std::io::{BufRead, BufReader, Read, Write};
use std::path::Path;
use std::process::{Command, ExitCode, Stdio};
const BLOCKED_EXTENSIONS: &[&str] = &[
    "a", "bin", "bps", "bsdiff", "d", "diff", "dis", "dll", "dmp", "dump", "dylib", "elf", "exe",
    "gba", "gz", "ips", "lst", "log", "map", "o", "patch", "raw", "rom", "sav", "so", "sym", "tar",
    "tgz", "unidiff", "ups", "xdelta", "xdelta3", "zip", "7z",
];
/// Fonts, rasters, audio and video no build consumes.
const PRESENTATION_EXTENSIONS: &[&str] = &[
    "otf", "ttf", "ttc", "woff", "woff2", "eot", "fnt", "bdf", "pcf", "gif", "jpg", "jpeg", "webp",
    "bmp", "ico", "avif", "apng", "tif", "tiff", "mp3", "ogg", "flac", "m4a", "aac", "mp4", "webm",
    "mov",
];
const BACKUP_EXTENSIONS: &[&str] = &["bak", "orig", "rej", "swp"];
/// Code a game without an asset manifest may track under its asset roots.
const MANIFESTLESS_EXTENSIONS: &[&str] = &["c", "h", "inc", "gitkeep"];
const FONT_TABLES: &[&[u8]] = &[
    b"BASE", b"CFF ", b"COLR", b"CPAL", b"DSIG", b"EBDT", b"EBLC", b"FFTM", b"GDEF", b"GPOS",
    b"GSUB", b"LTSH", b"OS/2", b"STAT", b"SVG ", b"VDMX", b"cmap", b"cvt ", b"fpgm", b"gasp",
    b"glyf", b"hdmx", b"head", b"hhea", b"kern", b"loca", b"maxp", b"name", b"post", b"prep",
];
const BASE64_RUN_MAX: usize = 255;
const BASE64_LINE_MIN: usize = 60;
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
    if listed(suffix, BLOCKED_EXTENSIONS) {
        return Some("private or generated file type");
    }
    if listed(suffix, PRESENTATION_EXTENSIONS) {
        return Some("presentation material: pret commits only editable build inputs");
    }
    if listed(suffix, BACKUP_EXTENSIONS) || leaf.ends_with('~') {
        return Some("editor or merge backup");
    }
    if directories
        .iter()
        .any(|directory| directory.eq_ignore_ascii_case("preview"))
        && !suffix.eq_ignore_ascii_case("svg")
    {
        return Some("PREVIEW holds only the generated coverage figure SVG");
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
fn new_text_file_reason(path: &str, existing: bool) -> Option<String> {
    if existing
        || !listed(extension(path), &["md", "txt"])
        || matches!(
            path,
            "README.md" | "CONTRIBUTING.md" | "AGENTS.md" | "CLAUDE.md"
        )
    {
        return None;
    }
    Some(format!("separate document {path}: use CONTRIBUTING.md"))
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
                // External decompiler and compiler distributions are source inputs,
                // and a nested checkout owns its own documents.
                if entry.file_name() != ".git"
                    && path != root.join("m2c")
                    && !path.join(".git").exists()
                    && !path.join("gcc/toplev.c").is_file()
                {
                    pending.push(path);
                }
                continue;
            }
            let relative = path
                .strip_prefix(root)
                .map_err(|e| e.to_string())?
                .to_string_lossy();
            if !listed(extension(&relative), &["md", "txt"])
                || matches!(relative.as_ref(), "README.md" | "CONTRIBUTING.md")
            {
                continue;
            }
            if matches!(relative.as_ref(), "AGENTS.md" | "CLAUDE.md") {
                let pointer = std::fs::read_to_string(&path).unwrap_or_default();
                if kind.is_symlink()
                    || matches!(
                        pointer.trim(),
                        "@CONTRIBUTING.md" | "See [CONTRIBUTING.md](CONTRIBUTING.md)."
                    )
                {
                    continue;
                }
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
    depth: u8,
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
        depth: *data.get(24)?,
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
/// The binary build inputs a game may track; everything else is text.
fn registered_binary(path: &str, data: &[u8]) -> bool {
    let components: Vec<_> = path.split('/').collect();
    let area = match components.as_slice() {
        ["games", _, area, _, ..] => *area,
        _ => return data.is_empty(),
    };
    if data.is_empty() {
        return true;
    }
    if extension(path).eq_ignore_ascii_case("pcm4") {
        return area == "SOUND" && components[3] == "SAMPLE" && data.len() <= 64;
    }
    let indexed = png(data)
        .is_some_and(|image| matches!(image.colour, 0 | 3) && image.depth <= 8 && !image.animated);
    let wave = data.get(8..12).is_some_and(|form| {
        (data.starts_with(b"RIFF") && form == b"WAVE")
            || (data.starts_with(b"FORM") && form == b"AIFF")
    });
    (matches!(area, "SRC" | "TEXT") && indexed)
        || (area == "SRC" && data.starts_with(b"ALCHTOK1"))
        || (area == "SOUND" && (wave || data.starts_with(b"MThd")))
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
fn base64_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || matches!(byte, b'+' | b'/')
}
/// Longest base64-alphabet run in a line that mixes upper case, lower case and
/// digits, as encoded bytes do; bit rows and hex digests do not.
fn base64_run(line: &[u8]) -> usize {
    let mut best = 0;
    let mut run = (0, 0);
    for byte in line.iter().chain(b" ").copied() {
        if base64_byte(byte) || (byte == b'=' && run.0 > 0) {
            let class = match byte {
                b'A'..=b'Z' => 1,
                b'a'..=b'z' => 2,
                b'0'..=b'9' => 4,
                _ => 0,
            };
            run = (run.0 + 1, run.1 | class);
            continue;
        }
        if run.1 == 7 && run.0 > best {
            best = run.0;
        }
        run = (0, 0);
    }
    best
}
fn base64_reason(text: &str) -> Option<&'static str> {
    let mut block = (0, 0);
    for line in text.lines() {
        let length = base64_run(line.as_bytes());
        if length > BASE64_RUN_MAX {
            return Some("base64 payload: pret commits only editable build inputs");
        }
        block = if length >= BASE64_LINE_MIN {
            (block.0 + 1, block.1 + length)
        } else {
            (0, 0)
        };
        if block.0 >= 2 && block.1 > BASE64_RUN_MAX {
            return Some("base64 payload: pret commits only editable build inputs");
        }
    }
    None
}
fn included_bytes_reason(path: &str, text: &str) -> Option<&'static str> {
    if !extension(path).eq_ignore_ascii_case("rs") {
        return None;
    }
    let embedded = text.match_indices("include_bytes!(").any(|(index, word)| {
        let rest = &text[index + word.len()..];
        let mut depth = 1;
        let end = rest
            .char_indices()
            .find(|(_, ch)| {
                depth += match ch {
                    '(' => 1,
                    ')' => -1,
                    _ => 0,
                };
                depth == 0
            })
            .map_or(rest.len(), |(end, _)| end);
        rest[..end].split('"').skip(1).step_by(2).any(|literal| {
            let literal = literal.replace('\\', "/");
            literal
                .split('/')
                .any(|component| matches!(component, "games" | "roms" | "out"))
                || listed(extension(&literal), PRESENTATION_EXTENSIONS)
                || listed(extension(&literal), BLOCKED_EXTENSIONS)
        })
    });
    embedded.then_some("include_bytes! of game, ROM or output bytes")
}
fn publication_data_reason(path: &str, data: &[u8]) -> Option<&'static str> {
    if listed(extension(path), &["asm", "s"]) && incbin(data) {
        return Some("committed incbin payload");
    }
    if let Some(reason) =
        publication_content_reason(data).or_else(|| presentation_magic_reason(data))
    {
        return Some(reason);
    }
    if binary(data) || extension(path).eq_ignore_ascii_case("pcm4") {
        return (!registered_binary(path, data))
            .then_some("unregistered binary: pret commits only editable build inputs");
    }
    let text = std::str::from_utf8(data).unwrap_or("");
    data_uri_reason(text)
        .or_else(|| base64_reason(text))
        .or_else(|| included_bytes_reason(path, text))
}
/// Games whose asset manifest is tracked in the inspected tree.
fn manifest_games<'a>(paths: impl IntoIterator<Item = &'a str>) -> Vec<String> {
    paths
        .into_iter()
        .filter_map(
            |path| match path.split('/').collect::<Vec<_>>().as_slice() {
                ["games", game, "SRC", "SYSTEM", "RESOURCE.JSON"] => Some(game.to_string()),
                _ => None,
            },
        )
        .collect()
}
fn manifestless_reason(path: &str, manifests: &[String]) -> Option<&'static str> {
    let components: Vec<_> = path.split('/').collect();
    let asset = match components.as_slice() {
        ["games", game, "SRC" | "SOUND" | "TEXT", _, ..] => Some(game),
        ["games", game, "asm", "overlays", _, ..] => Some(game),
        _ => None,
    }?;
    let code = listed(extension(path), MANIFESTLESS_EXTENSIONS);
    (!code && !manifests.iter().any(|game| game == asset))
        .then_some("game material without a consuming asset manifest (SRC/SYSTEM/RESOURCE.JSON)")
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
/// Parse `git diff --raw -z`; rename records have an old and a new path.
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
        if metadata[4] != "D" && metadata[1] != "160000" {
            let existing = metadata[4] != "A" && !metadata[4].starts_with('C');
            changes.push((path, existing));
        }
    }
    Ok((!fields.is_empty(), changes))
}
struct Entry {
    scope: String,
    path: String,
    object: String,
    existing: bool,
    manifest_reason: Option<&'static str>,
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
    let mut failures = Vec::new();
    let mut readable = Vec::new();
    for entry in entries {
        if let Some(reason) = new_text_file_reason(&entry.path, entry.existing) {
            failures.push(format!("{} {reason}", entry.scope));
            continue;
        }
        if let Some(reason) = publication_path_reason(&entry.path).or(entry.manifest_reason) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
            continue;
        }
        readable.push(entry);
    }
    let objects = readable.iter().map(|entry| entry.object.clone()).collect();
    blobs(root, objects, |index, data| {
        let entry = &readable[index];
        let reason = publication_data_reason(&entry.path, data).map(str::to_string);
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
/// Tracked `(object, path)` pairs of the index or of a revision, without gitlinks.
fn tracked(root: &Path, revision: Option<&str>) -> Result<Vec<(String, String)>, String> {
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
            .is_some_and(|(_, last): &(String, String)| last == path);
        if fields[0] != "160000" && !repeated {
            entries.push((object.to_string(), path.to_string()));
        }
    }
    Ok(entries)
}
fn manifests_of(root: &Path, revision: Option<&str>) -> Result<Vec<String>, String> {
    let paths = tracked(root, revision)?;
    Ok(manifest_games(paths.iter().map(|(_, path)| path.as_str())))
}
fn tree_entries(root: &Path, revision: Option<&str>) -> Result<Vec<Entry>, String> {
    let paths = tracked(root, revision)?;
    let manifests = manifest_games(paths.iter().map(|(_, path)| path.as_str()));
    let scope = revision.map_or("tree".to_string(), |revision| {
        format!("tree {}", &revision[..12.min(revision.len())])
    });
    Ok(paths
        .into_iter()
        .map(|(object, path)| Entry {
            scope: scope.clone(),
            manifest_reason: manifestless_reason(&path, &manifests),
            object,
            path,
            existing: true,
        })
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
        return Err("publication gate scanned nothing: no staged change to inspect".to_string());
    }
    let manifests = manifests_of(root, None)?;
    let entries = changes
        .into_iter()
        .map(|(path, existing)| Entry {
            scope: "staged".to_string(),
            object: format!(":{path}"),
            manifest_reason: manifestless_reason(&path, &manifests),
            path,
            existing,
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
        for (path, existing) in changes {
            entries.push(Entry {
                scope: commit[..12.min(commit.len())].to_string(),
                object: format!("{commit}:{path}"),
                manifest_reason: manifestless_reason(&path, &manifests),
                path,
                existing,
            });
        }
    }
    // Each pushed tip must also pass as a whole tree, not only as its deltas.
    for tip in tips {
        entries.extend(tree_entries(root, Some(&tip))?);
    }
    scan(root, entries, false)
}
/// Every file-level decision for one blob, as `scan` makes it.
fn publication_reason(path: &str, data: &[u8], manifests: &[String]) -> Option<&'static str> {
    publication_path_reason(path)
        .or_else(|| manifestless_reason(path, manifests))
        .or_else(|| publication_data_reason(path, data))
}
fn png_fixture(depth: u8, colour: u8, animated: bool) -> Vec<u8> {
    let chunk = |kind: &[u8], body: &[u8]| {
        let mut chunk = (body.len() as u32).to_be_bytes().to_vec();
        chunk.extend_from_slice(kind);
        chunk.extend_from_slice(body);
        chunk.extend_from_slice(&[0; 4]);
        chunk
    };
    let mut data = b"\x89PNG\r\n\x1a\n".to_vec();
    data.extend(chunk(
        b"IHDR",
        &[0, 0, 0, 16, 0, 0, 0, 16, depth, colour, 0, 0, 0],
    ));
    if animated {
        data.extend(chunk(b"acTL", &[0, 0, 0, 2, 0, 0, 0, 0]));
    }
    data.extend(chunk(b"IDAT", &[0x78, 0x01]));
    data.extend(chunk(b"IEND", &[]));
    data
}
/// Deterministic encoded-looking text; fixtures are built at run time so the
/// tracked gate source never carries the payloads it rejects.
fn base64_fixture(length: usize, seed: u32) -> String {
    let alphabet = b"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    let mut state = seed;
    (0..length)
        .map(|_| {
            state = state.wrapping_mul(1_103_515_245).wrapping_add(12_345);
            alphabet[(state >> 16) as usize % 64] as char
        })
        .collect()
}
/// `(path, bytes, game has a manifest, expected rejection reason fragment)`.
type Fixture = (&'static str, Vec<u8>, bool, Option<&'static str>);
fn fixtures() -> Vec<Fixture> {
    let tbs = "games/THE BROKEN SEAL";
    let uri = |media: &str| format!("data{}{media};base64,{}", ':', base64_fixture(24, 7));
    let font = [b"OTTO".as_slice(), &[0, 10, 0, 128, 0, 3, 0, 32], b"CFF "].concat();
    let mut fragment = vec![0u8; 0x1000];
    fragment[..4].copy_from_slice(&[0x2e, 0, 0, 0xea]);
    fragment[0xb2] = 0x96;
    let sum = fragment[0xa0..=0xbc]
        .iter()
        .fold(0u8, |sum, byte| sum.wrapping_add(*byte));
    fragment[0xbd] = 0u8.wrapping_sub(sum).wrapping_sub(0x19);
    let lines = (0..5)
        .map(|seed| format!("  {}\n", base64_fixture(76, seed)))
        .collect::<String>();
    let rows = (0..16)
        .map(|_| format!("        \"{}\",\n", "01".repeat(52)))
        .collect::<String>();
    let checksums = (0..4)
        .map(|seed| {
            format!(
                "checksum = \"{:064x}\"\n",
                u128::from(seed as u32) * 0x9e37_79b9_7f4a_7c15
            )
        })
        .collect::<String>();
    let wave = [
        b"RIFF".as_slice(),
        &[36, 0, 0, 0],
        b"WAVEfmt ",
        &[16, 0, 0, 0, 1, 0],
    ]
    .concat();
    let midi = [b"MThd".as_slice(), &[0, 0, 0, 6, 0, 1, 0, 2, 0, 96]].concat();
    let unregistered = Some("unregistered binary");
    let presentation = Some("presentation material");
    let embed = Some("data URI");
    let base64 = Some("base64 payload");
    vec![
        (
            "tools/alchemy/GRAPHICS/Weyard.otf",
            font.clone(),
            true,
            presentation,
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
            presentation,
        ),
        (
            "games/THE BROKEN SEAL/PREVIEW/title.png",
            png_fixture(4, 3, false),
            true,
            Some("PREVIEW"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/SHOT.PNG",
            png_fixture(8, 6, false),
            true,
            Some("truecolour"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/IDLE.INDEXED.PNG",
            png_fixture(4, 3, true),
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
            "games/THE BROKEN SEAL/PREVIEW/TBS-EN-ROM.SVG",
            format!("<svg><image href=\"{}\"/></svg>", uri("image/png")).into_bytes(),
            true,
            embed,
        ),
        (
            "tools/alchemy/src/dashboard/index.html",
            format!("<style>@font-face{{src:url({})}}</style>", uri("font/otf")).into_bytes(),
            true,
            embed,
        ),
        (
            "tools/alchemy/src/coverage/figure.rs",
            format!(
                "format!(\"{}{{}}\", payload)",
                uri("application/octet-stream")
            )
            .into_bytes(),
            true,
            embed,
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/BLOB.JSON",
            format!("{{\"bytes\": \"{}\"}}\n", base64_fixture(300, 3)).into_bytes(),
            true,
            base64,
        ),
        (
            "tools/alchemy/src/dashboard/font.css",
            lines.into_bytes(),
            true,
            base64,
        ),
        (
            "tools/alchemy/src/assets.rs",
            format!(
                "const LOGO: &[u8] = include_bytes{}\"../../../{tbs}/SRC/LOGO.DAT\");\n",
                "!("
            )
            .into_bytes(),
            true,
            Some("include_bytes!"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/HEADER.DAT",
            fragment,
            true,
            Some("ROM header fragment"),
        ),
        (
            "games/THE BROKEN SEAL/recon/en/main/0800ebec.c.bak",
            b"int x;\n".to_vec(),
            true,
            Some("backup"),
        ),
        (
            "games/THE LOST AGE/SOUND/SEQUENCE/X.MID",
            midi.clone(),
            false,
            Some("asset manifest"),
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/FONT/LOCALIZATION_GLYPHS_0020_00FF.1BPP.PNG",
            png_fixture(1, 3, false),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/SRC/GRAPHICS/TILE/UI_MTF_00.INDEXED.PNG",
            png_fixture(4, 3, false),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/TEXT/STAFF_ROLL_MOJI.1BPP.PNG",
            png_fixture(8, 0, false),
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
            midi,
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
            b"ALCHTOK1\x09\xd0\x01\0".to_vec(),
            true,
            None,
        ),
        (
            "games/THE LOST AGE/asm/overlays/.gitkeep",
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
        ("tools/Cargo.lock", checksums.into_bytes(), true, None),
        (
            "games/THE BROKEN SEAL/SRC/SYSTEM/ROM_HEADER.JSON",
            rows.into_bytes(),
            true,
            None,
        ),
        (
            "tools/alchemy/src/dashboard/style.css",
            b"body { font: 400 16px monospace; }\n".to_vec(),
            true,
            None,
        ),
        (
            "games/THE BROKEN SEAL/PREVIEW/TBS-EN-ROM.SVG",
            b"<svg><style>.label{font-family:monospace}</style><rect width=\"8\"/></svg>".to_vec(),
            true,
            None,
        ),
    ]
}
fn check_fixtures() -> Result<(), String> {
    let tbs = vec!["THE BROKEN SEAL".to_string()];
    let both = vec!["THE BROKEN SEAL".to_string(), "THE LOST AGE".to_string()];
    for (path, data, manifested, expected) in fixtures() {
        let manifests = if manifested { &both } else { &tbs };
        let actual = publication_reason(path, &data, manifests);
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
fn self_test() -> Result<(), String> {
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
        "games/THE BROKEN SEAL/asm/080000c0.s~",
    ] {
        if publication_path_reason(path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for path in [
        "src/main.c",
        "games/THE BROKEN SEAL/asm/080000c0.s",
        "games/THE BROKEN SEAL/PREVIEW/TBS-EN-ROM.SVG",
        "games/THE BROKEN SEAL/SOUND/SEQUENCE/THEME.mid",
        "games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE.wav",
        "games/THE BROKEN SEAL/SRC/SYSTEM/RESOURCE.JSON",
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
    let hygiene_holds = publication_data_reason(
        "games/THE BROKEN SEAL/asm/08000000.s",
        b".incbin \"rom.gba\"\n",
    ) == Some("committed incbin payload")
        && conflict_marker_reason("CONTRIBUTING.md", b"a\n<<<<<<< HEAD\nb\n").is_some()
        && conflict_marker_reason("CONTRIBUTING.md", b"a\n>>>>>>> topic\n").is_some()
        && publication_data_reason("CONTRIBUTING.md", b"x\n<<<<<<< HEAD\n").is_none()
        && conflict_marker_reason("CONTRIBUTING.md", b"Title\n=======\n\nbody\n").is_none()
        && conflict_marker_reason("CONTRIBUTING.md", b"see <<<<<<<HEAD in the output\n").is_none()
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
const USAGE: &str = "usage: check publication [--documents | --staged | --pre-push | --tree [REV] | --self-test]\n\nModes:\n  --documents    Check owned documentation, including ignored output.\n  --staged       Check staged files before committing.\n  --pre-push     Check outgoing history and each pushed tree using update lines on stdin.\n  --tree [REV]   Check every file tracked in the index, or in revision REV.\n  --self-test    Run the publication gate's internal checks.\n  -h, --help     Show this help.";
fn fail(message: &str) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}
pub(super) fn entry(arguments: &[String]) -> ExitCode {
    let root = crate::compiler::routing::root();
    match arguments {
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
        [argument] if argument == "--self-test" => self_test().map_or_else(
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
    fn git_records_preserve_renames_and_skip_submodules() {
        let raw = b":100644 100644 a b M\0kept.c\0\
                    :100644 100644 a b R100\0old.txt\0new.txt\0\
                    :000000 160000 a b A\0vendor\0";
        let (_, changes) = raw_changes(raw).unwrap();
        assert_eq!(changes.len(), 2);
        assert_eq!(changes[0], ("kept.c".to_string(), true));
        assert_eq!(changes[1], ("new.txt".to_string(), true));
        assert!(new_text_file_reason("README.md", false).is_none());
        assert!(new_text_file_reason("CONTRIBUTING.md", false).is_none());
        assert!(new_text_file_reason("AGENTS.md", false).is_none());
        assert!(new_text_file_reason("notes.txt", false).is_some());
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
    fn documents_include_ignored_output_and_allow_only_agent_pointers() {
        let root = std::env::temp_dir().join(format!("alchemy-documents-{}", std::process::id()));
        std::fs::create_dir_all(root.join("out")).unwrap();
        std::fs::write(root.join(".gitignore"), "out/\n").unwrap();
        std::fs::write(root.join("CONTRIBUTING.md"), "guide").unwrap();
        std::fs::write(root.join("AGENTS.md"), "@CONTRIBUTING.md\n").unwrap();
        std::fs::create_dir_all(root.join("out/compiler/gcc")).unwrap();
        std::fs::write(root.join("out/compiler/gcc/toplev.c"), "compiler source").unwrap();
        std::fs::write(root.join("out/compiler/gcc/thumb.md"), "(define_insn)").unwrap();
        std::fs::create_dir_all(root.join("worktrees/scene")).unwrap();
        std::fs::write(root.join("worktrees/scene/.git"), "gitdir: ../../.git\n").unwrap();
        std::fs::write(root.join("worktrees/scene/README.md"), "its own").unwrap();
        std::fs::write(root.join("worktrees/scene/score.txt"), "its own").unwrap();
        assert!(check_documents(&root).is_ok());
        for name in ["out/verdict.md", "out/score.txt", "AGENTS.md"] {
            std::fs::write(root.join(name), "another guide").unwrap();
            assert!(check_documents(&root).unwrap_err().contains(name));
            std::fs::remove_file(root.join(name)).unwrap();
        }
        std::fs::remove_dir_all(root).unwrap();
    }
    #[test]
    fn every_publication_rule_rejects_its_fixture_and_accepts_build_inputs() {
        check_fixtures().unwrap();
        self_test().unwrap();
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
        let midi = [b"MThd".as_slice(), &[0, 0, 0, 6, 0, 1, 0, 1, 0, 96]].concat();
        let inputs = commit(
            root,
            &[
                ("games/X/SRC/SYSTEM/RESOURCE.JSON", b"{}\n".to_vec()),
                (
                    "games/X/SRC/GRAPHICS/TILE/A.4BPP.PNG",
                    png_fixture(4, 3, false),
                ),
                ("games/X/SOUND/SEQUENCE/A.MID", midi.clone()),
                ("games/X/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("games/Y/SRC/MAIN.C", b"void main(void) {}\n".to_vec()),
                ("games/X/PREVIEW/X-ROM.SVG", b"<svg/>\n".to_vec()),
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
        let manifestless = commit(root, &[("games/Y/SOUND/SEQUENCE/A.MID", midi)]);
        let error = check_tree(root, Some(&manifestless)).unwrap_err();
        assert!(error.contains("games/Y/SOUND/SEQUENCE/A.MID: game material without"));
        assert!(!error.contains("games/X/SOUND"), "{error}");
    }
}
