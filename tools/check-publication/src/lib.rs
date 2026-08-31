//! Fail-closed publication checks for staged changes and outgoing history.
pub mod cli;
use std::path::Path;
use std::process::Command;
const BLOCKED_EXTENSIONS: &[&str] = &[
    "a", "bin", "bps", "bsdiff", "d", "diff", "dis", "dll", "dmp", "dump", "dylib", "elf", "exe",
    "gba", "gz", "ips", "lst", "log", "map", "o", "patch", "raw", "rom", "sav", "so", "sym", "tar",
    "tgz", "ups", "xdelta", "xdelta3", "zip", "7z",
];
const BLOCKED_DIRECTORIES: &[&str] = &[
    ".cache",
    "alchemy-gcc",
    "analysis",
    "build",
    "builds",
    "cmatch",
    "comparisons",
    "compiler-output",
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
fn canonical_binary_source(path: &str) -> bool {
    let normalized = path.replace('\\', "/").to_ascii_lowercase();
    normalized.starts_with("games/gs1/assets/maps/")
        && matches!(
            normalized.rsplit('/').next(),
            Some("metatiles.bin" | "metatile_attributes.bin")
        )
}
pub fn publication_path_reason(path: &str) -> Option<&'static str> {
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
    if listed(suffix, BLOCKED_EXTENSIONS) && !canonical_binary_source(&normalized) {
        return Some("private or generated file type");
    }
    let report_name = leaf
        .split(['.', '_', '-'])
        .any(|word| listed(word, REPORT_WORDS));
    if listed(suffix, REPORT_EXTENSIONS) && report_name {
        return Some("private analysis report");
    }
    None
}
fn gba_image(data: &[u8]) -> bool {
    if data.len() < 0xc0 || !data.len().is_multiple_of(0x8000) || data.len() > 0x0400_0000 {
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
pub fn publication_content_reason(data: &[u8]) -> Option<&'static str> {
    if gba_image(data) {
        return Some("GBA ROM image");
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
pub fn conflict_marker_reason(path: &str, data: &[u8]) -> Option<String> {
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
    let suffix = extension(path);
    if (!suffix.eq_ignore_ascii_case("txt") && !suffix.eq_ignore_ascii_case("md")) || existing {
        return None;
    }
    if suffix.eq_ignore_ascii_case("md") {
        if matches!(path, "README.md" | "CONTRIBUTING.md") {
            return None;
        }
        return Some(format!(
            "new markdown file {path}: this repository keeps exactly two \
             (README.md and CONTRIBUTING.md). Put project procedure in \
             CONTRIBUTING.md, which is the single contributor guide"
        ));
    }
    Some(format!(
        "new text file {path}: notes and scratch output do not belong in the tree. \
         Put durable content in CONTRIBUTING.md and transient output in the \
         scratch directory"
    ))
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
fn publication_data_reason(path: &str, data: &[u8]) -> Option<&'static str> {
    if listed(extension(path), &["asm", "s"]) && incbin(data) {
        Some("committed incbin payload")
    } else {
        publication_content_reason(data)
    }
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
pub fn commit_message_reason(message: &str) -> Option<&'static str> {
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
fn existed_in_head(root: &Path, path: &str) -> bool {
    git(root, &["ls-tree", "HEAD", "--", path], "tracked path scan")
        .is_ok_and(|output| !output.is_empty())
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
}
fn scan(root: &Path, entries: Vec<Entry>, conflicts: bool) -> Result<(), String> {
    let mut failures = Vec::new();
    for entry in entries {
        let existing = entry.existing
            || (listed(extension(&entry.path), &["txt", "md"])
                && existed_in_head(root, &entry.path));
        if let Some(reason) = new_text_file_reason(&entry.path, existing) {
            failures.push(format!("{} {reason}", entry.scope));
            continue;
        }
        if let Some(reason) = publication_path_reason(&entry.path) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
            continue;
        }
        let data = git(
            root,
            &["show", &entry.object],
            &format!("blob {}", entry.object),
        )?;
        if let Some(reason) = publication_data_reason(&entry.path, &data) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
            continue;
        }
        if conflicts {
            if let Some(reason) = conflict_marker_reason(&entry.path, &data) {
                failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
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
pub fn check_staged(root: &Path) -> Result<(), String> {
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
    let entries = changes
        .into_iter()
        .map(|(path, existing)| Entry {
            scope: "staged".to_string(),
            object: format!(":{path}"),
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
pub fn check_push(root: &Path, updates: &str) -> Result<(), String> {
    let updates: Vec<_> = updates
        .lines()
        .map(str::trim)
        .filter(|line| !line.is_empty())
        .collect();
    if updates.is_empty() {
        return Err("publication gate scanned nothing: no ref update on stdin".to_string());
    }
    let mut commits = Vec::new();
    for update in updates {
        let fields: Vec<_> = update.split_whitespace().collect();
        if fields.len() != 4 {
            return Err("invalid pre-push update".to_string());
        }
        if fields[1].bytes().all(|byte| byte == b'0') {
            continue;
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
        for (path, existing) in changes {
            entries.push(Entry {
                scope: commit[..12.min(commit.len())].to_string(),
                object: format!("{commit}:{path}"),
                path,
                existing,
            });
        }
    }
    scan(root, entries, false)
}
pub fn self_test() -> Result<(), String> {
    for directory in BLOCKED_DIRECTORIES {
        let path = format!("{directory}/fixture.c");
        if publication_path_reason(&path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for suffix in BLOCKED_EXTENSIONS {
        let path = format!("fixture.{suffix}");
        if publication_path_reason(&path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for path in [
        "baserom",
        "private-diff.json",
        "gs1-en.gba.lz",
        ".cmatch-fresh/result.s",
    ] {
        if publication_path_reason(path).is_none() {
            return Err(format!("private path accepted: {path}"));
        }
    }
    for path in [
        "src/main.c",
        "games/gs1/asm/080000c0.s",
        "games/gs1/assets/graphics/title.png",
        "games/gs1/sound/theme.mid",
        "games/gs1/sound/wave.wav",
        "games/gs1/assets/data/layout.json",
        "tools/compare-roms/src/main.rs",
        "tools/build-full/src/main.rs",
        "games/gs1/assets/data/resource_2_build_stamp.txt",
        "games/gs1/assets/maps/town/metatiles.bin",
        "games/gs1/assets/maps/town/metatile_attributes.bin",
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
    let hygiene_holds =
        publication_data_reason("games/gs1/asm/08000000.s", b".incbin \"rom.gba\"\n")
            == Some("committed incbin payload")
            && conflict_marker_reason("CONTRIBUTING.md", b"a\n<<<<<<< HEAD\nb\n").is_some()
            && conflict_marker_reason("CONTRIBUTING.md", b"a\n>>>>>>> topic\n").is_some()
            && publication_data_reason("CONTRIBUTING.md", b"x\n<<<<<<< HEAD\n").is_none()
            && conflict_marker_reason("CONTRIBUTING.md", b"Title\n=======\n\nbody\n").is_none()
            && conflict_marker_reason("CONTRIBUTING.md", b"see <<<<<<<HEAD in the output\n")
                .is_none()
            && conflict_marker_reason("games/gs1/assets/readme/x.png", b"<<<<<<< HEAD\n").is_none();
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
        assert!(new_text_file_reason("AGENTS.md", false).is_some());
        assert!(new_text_file_reason("notes.txt", false).is_some());
    }
}
