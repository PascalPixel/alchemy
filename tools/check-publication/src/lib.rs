//! Publication gate --- port of `tools/check/check_publication.ts`.
//!
//! Nothing private or generated may leave this machine: not as a staged blob,
//! not as a blob in outgoing history, not as a commit message. Every message
//! this module produces matches the TypeScript original character for
//! character.
//!
//! PORT NOTE: every regular expression is hand-rolled (no regex crate). The
//! hand-rolled matchers reproduce the JavaScript semantics that are actually
//! load-bearing here, including `\b` being ASCII-only and `^` under the `m`
//! flag matching after `\r` as well as `\n`. The one JavaScript behaviour not
//! reproduced is `^` matching after U+2028/U+2029, which no commit message or
//! source file in this repository contains.
//!
//! PORT NOTE: git is invoked with `-C <root>` instead of inheriting the process
//! working directory, so every scan can be pointed at a temporary tree in
//! tests. At the repository root the two are identical.

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
    "toolchain",
    "toolchains",
    "work",
];

const REPORT_EXTENSIONS: &[&str] = &["csv", "json", "jsonl", "log", "tsv", "txt"];

/// The words `PRIVATE_REPORT` looks for, delimited by `.`, `_`, `-`, or an end.
const PRIVATE_REPORT_WORDS: &[&str] = &["analysis", "comparison", "diff", "dump", "report"];

const MARKER_EXTENSIONS: &[&str] = &[
    "md", "ts", "js", "json", "sh", "c", "h", "s", "asm", "tsv", "txt",
];

/// `/^0+$/` --- a git object id of all zeroes means "no such ref".
fn zero_oid(value: &str) -> bool {
    !value.is_empty() && value.bytes().all(|byte| byte == b'0')
}

fn is_separator(byte: u8) -> bool {
    matches!(byte, b'.' | b'_' | b'-')
}

/// `/(?:^|[._-])(?:analysis|comparison|diff|dump|report)(?:[._-]|$)/i`
fn private_report(leaf: &str) -> bool {
    let lower = leaf.to_ascii_lowercase();
    let bytes = lower.as_bytes();
    for word in PRIVATE_REPORT_WORDS {
        let word = word.as_bytes();
        if bytes.len() < word.len() {
            continue;
        }
        for start in 0..=(bytes.len() - word.len()) {
            if &bytes[start..start + word.len()] != word {
                continue;
            }
            let before_ok = start == 0 || is_separator(bytes[start - 1]);
            let end = start + word.len();
            let after_ok = end == bytes.len() || is_separator(bytes[end]);
            if before_ok && after_ok {
                return true;
            }
        }
    }
    false
}

fn extension(path: &str) -> String {
    let leaf = match path.rfind('/') {
        Some(index) => &path[index + 1..],
        None => path,
    };
    match leaf.rfind('.') {
        Some(dot) => leaf[dot + 1..].to_ascii_lowercase(),
        None => String::new(),
    }
}

fn canonical_binary_source(path: &str) -> bool {
    let normalized = path.replace('\\', "/").to_lowercase();
    if !normalized.starts_with("assets/maps/") {
        return false;
    }
    let leaf = match normalized.rfind('/') {
        Some(index) => normalized[index + 1..].to_string(),
        None => normalized.clone(),
    };
    leaf == "metatiles.bin" || leaf == "metatile_attributes.bin"
}

/// Why a repository path may not be published, or `None` if it may.
pub fn publication_path_reason(path: &str) -> Option<&'static str> {
    let normalized = path.replace('\\', "/");
    let components: Vec<&str> = normalized.split('/').filter(|item| !item.is_empty()).collect();
    let leaf = components
        .last()
        .map(|item| item.to_lowercase())
        .unwrap_or_default();
    let directories: Vec<String> = components[..components.len().saturating_sub(1)]
        .iter()
        .map(|item| item.to_lowercase())
        .collect();
    let suffix = extension(&normalized);
    if normalized.starts_with('/') || components.contains(&"..") {
        return Some("invalid repository path");
    }
    if directories
        .iter()
        .any(|item| BLOCKED_DIRECTORIES.contains(&item.as_str()) || item.starts_with(".cmatch"))
    {
        return Some("private or generated directory");
    }
    if leaf == "baserom" || leaf.starts_with("baserom.") {
        return Some("private ROM name");
    }
    if leaf.contains(".gba.") || leaf.contains(".rom.") {
        return Some("private ROM name");
    }
    if BLOCKED_EXTENSIONS.contains(&suffix.as_str()) && !canonical_binary_source(&normalized) {
        return Some("private or generated file type");
    }
    if REPORT_EXTENSIONS.contains(&suffix.as_str()) && private_report(&leaf) {
        return Some("private analysis report");
    }
    None
}

fn gba_image(data: &[u8]) -> bool {
    if data.len() < 0xc0 || !data.len().is_multiple_of(0x8000) || data.len() > 0x0400_0000 {
        return false;
    }
    if data[0xb2] != 0x96 || data[0xb3] != 0 || data[0xb4] != 0 {
        return false;
    }
    for byte in &data[0xb5..=0xbb] {
        if *byte != 0 {
            return false;
        }
    }
    let mut sum: u8 = 0;
    for byte in &data[0xa0..=0xbc] {
        sum = sum.wrapping_add(*byte);
    }
    data[0xbd] == (0u8.wrapping_sub(sum).wrapping_sub(0x19))
}

/// Why a blob's *content* may not be published, or `None` if it may.
pub fn publication_content_reason(data: &[u8]) -> Option<&'static str> {
    if gba_image(data) {
        return Some("GBA ROM image");
    }
    if data.len() >= 4 && data[..4] == [0x7f, 0x45, 0x4c, 0x46] {
        return Some("ELF build product");
    }
    if data.len() >= 8 && &data[..8] == b"!<arch>\n" {
        return Some("archive or object library");
    }
    if data.len() >= 2 && data[0] == 0x4d && data[1] == 0x5a {
        return Some("native executable");
    }
    if data.len() >= 4 {
        let magic = u32::from_be_bytes([data[0], data[1], data[2], data[3]]);
        const MAGICS: [u32; 9] = [
            0xfeed_face,
            0xcefa_edfe,
            0xfeed_facf,
            0xcffa_edfe,
            0xcafe_babe,
            0xbeba_feca,
            0xcafe_babf,
            0xbfba_feca,
            0x0061_736d,
        ];
        if MAGICS.contains(&magic) {
            return Some("native executable");
        }
    }
    None
}

/// JavaScript `\s` for the character classes this port needs.
fn is_js_space(ch: char) -> bool {
    ch.is_whitespace() || ch == '\u{feff}'
}

/// Positions at which JavaScript's `^` matches under the `m` flag.
fn line_starts(text: &str) -> Vec<usize> {
    let bytes = text.as_bytes();
    let mut starts = vec![0usize];
    for (index, byte) in bytes.iter().enumerate() {
        if *byte == b'\n' || *byte == b'\r' {
            starts.push(index + 1);
        }
    }
    starts
}

/// `/^(?:<{7}|>{7}) /` anchored at the start of `line`.
fn marker_line(line: &str) -> bool {
    let bytes = line.as_bytes();
    if bytes.len() < 8 || bytes[7] != b' ' {
        return false;
    }
    bytes[..7].iter().all(|byte| *byte == b'<') || bytes[..7].iter().all(|byte| *byte == b'>')
}

/// `/^\s*\.incbin\b/im`
fn incbin(text: &str) -> bool {
    for start in line_starts(text) {
        let rest = &text[start..];
        let trimmed = rest.trim_start_matches(is_js_space);
        if trimmed.len() < 7 {
            continue;
        }
        if !trimmed.as_bytes()[..7].eq_ignore_ascii_case(b".incbin") {
            continue;
        }
        match trimmed[7..].chars().next() {
            None => return true,
            Some(next) if !(next.is_ascii_alphanumeric() || next == '_') => return true,
            Some(_) => {}
        }
    }
    false
}

/// Why a file carries an unresolved merge conflict, or `None`.
///
/// Only the opening and closing markers are matched. A bare `=======` is a
/// valid Markdown heading underline, so it is deliberately not matched.
pub fn conflict_marker_reason(path: &str, data: &[u8]) -> Option<String> {
    if !MARKER_EXTENSIONS.contains(&extension(path).as_str()) {
        return None;
    }
    let text = String::from_utf8_lossy(data);
    let found = line_starts(&text)
        .into_iter()
        .any(|start| marker_line(&text[start..]));
    if !found {
        return None;
    }
    // The reported line number comes from a plain `\n` split, exactly as the
    // TypeScript does; a `\r`-only match therefore reports line 0 in both.
    let line = text
        .split('\n')
        .position(marker_line)
        .map(|index| index as isize + 1)
        .unwrap_or(0);
    Some(format!(
        "unresolved conflict marker at line {line}; resolve the merge before committing"
    ))
}

/// Why a staged or committed entry may not be published, or `None`.
pub fn publication_entry_reason(path: &str, data: &[u8]) -> Option<String> {
    if let Some(reason) = publication_path_reason(path) {
        return Some(reason.to_string());
    }
    let suffix = extension(path);
    if (suffix == "asm" || suffix == "s") && incbin(&String::from_utf8_lossy(data)) {
        return Some("committed incbin payload".to_string());
    }
    publication_content_reason(data).map(|reason| reason.to_string())
}

/// `/(?:\b[0-9a-fA-F]{2}\b[ \t]+){7}\b[0-9a-fA-F]{2}\b/`
///
/// A run of eight space-separated hex byte pairs: someone pasted bytes.
fn byte_dump(message: &str) -> bool {
    let bytes = message.as_bytes();
    let word = |byte: u8| byte.is_ascii_alphanumeric() || byte == b'_';
    let mut runs: Vec<(usize, usize)> = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        if word(bytes[index]) {
            let start = index;
            while index < bytes.len() && word(bytes[index]) {
                index += 1;
            }
            runs.push((start, index));
        } else {
            index += 1;
        }
    }
    let pair = |run: &(usize, usize)| {
        run.1 - run.0 == 2 && bytes[run.0..run.1].iter().all(u8::is_ascii_hexdigit)
    };
    let mut streak = 0usize;
    for position in 0..runs.len() {
        if !pair(&runs[position]) {
            streak = 0;
            continue;
        }
        let joined = position > 0
            && pair(&runs[position - 1])
            && streak > 0
            && bytes[runs[position - 1].1..runs[position].0]
                .iter()
                .all(|byte| *byte == b' ' || *byte == b'\t')
            && runs[position].0 > runs[position - 1].1;
        streak = if joined { streak + 1 } else { 1 };
        if streak >= 8 {
            return true;
        }
    }
    false
}

/// Reject a commit MESSAGE that carries ROM bytes.
///
/// Everything else in this file scans file blobs. Commit messages were never
/// scanned at all, so the gate passing said nothing about them -- and messages
/// are published to the remote exactly like file contents are.
///
/// This deliberately checks one unambiguous thing: a run of space-separated hex
/// byte pairs, which is a pasted byte dump and never ordinary prose. It does
/// NOT try to detect quoted disassembly.
pub fn commit_message_reason(message: &str) -> Option<&'static str> {
    if byte_dump(message) {
        Some("commit message contains a raw byte dump")
    } else {
        None
    }
}

// --- git plumbing ----------------------------------------------------------

fn git(root: &Path, args: &[&str], input: Option<&str>, label: &str) -> Result<Vec<u8>, String> {
    use std::io::Write;
    use std::process::Stdio;
    let mut command = Command::new("git");
    command
        .arg("-C")
        .arg(root)
        .args(args)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .stdin(if input.is_some() {
            Stdio::piped()
        } else {
            Stdio::null()
        });
    let mut child = command
        .spawn()
        .map_err(|error| format!("{label} failed: {error}"))?;
    if let Some(text) = input {
        child
            .stdin
            .as_mut()
            .ok_or_else(|| format!("{label} failed"))?
            .write_all(text.as_bytes())
            .map_err(|error| format!("{label} failed: {error}"))?;
    }
    let result = child
        .wait_with_output()
        .map_err(|error| format!("{label} failed: {error}"))?;
    if !result.status.success() {
        let stderr = String::from_utf8_lossy(&result.stderr).trim().to_string();
        return Err(if stderr.is_empty() {
            format!("{label} failed")
        } else {
            stderr
        });
    }
    Ok(result.stdout)
}

fn nul_list(value: &[u8]) -> Vec<String> {
    String::from_utf8_lossy(value)
        .split('\0')
        .filter(|item| !item.is_empty())
        .map(str::to_string)
        .collect()
}

fn lines(value: &[u8]) -> Vec<String> {
    String::from_utf8_lossy(value)
        .split('\n')
        .map(|item| item.trim().to_string())
        .filter(|item| !item.is_empty())
        .collect()
}

/// One thing to scan. `data` is fetched lazily, exactly as in the TypeScript.
pub struct Entry<'a> {
    pub scope: String,
    pub path: String,
    data: Box<dyn Fn() -> Result<Vec<u8>, String> + 'a>,
}

impl Entry<'_> {
    pub fn data(&self) -> Result<Vec<u8>, String> {
        (self.data)()
    }
}

/// The shared reject pass. Failure order follows entry order.
pub fn reject(entries: &[Entry]) -> Result<(), String> {
    let mut failures: Vec<String> = Vec::new();
    for entry in entries {
        if let Some(reason) = publication_path_reason(&entry.path) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
            continue;
        }
        if let Some(reason) = publication_entry_reason(&entry.path, &entry.data()?) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
        }
    }
    if failures.is_empty() {
        Ok(())
    } else {
        Err(format!("publication gate rejected:\n{}", failures.join("\n")))
    }
}

fn staged_paths(root: &Path) -> Result<Vec<String>, String> {
    let paths = nul_list(&git(
        root,
        &[
            "diff",
            "--cached",
            "--name-only",
            "--diff-filter=ACMRT",
            "-z",
        ],
        None,
        "staged path scan",
    )?);
    // Exclude submodules which are tracked as commit objects, not blobs.
    let mut kept = Vec::new();
    for path in paths {
        let stage = String::from_utf8_lossy(&git(
            root,
            &["ls-files", "--stage", &path],
            None,
            "ls-files",
        )?)
        .to_string();
        if !stage.is_empty() && !stage.starts_with("160000") {
            kept.push(path);
        }
    }
    Ok(kept)
}

/// Every path with a staged change, deletions included. Used only by the
/// scanned-nothing guard, so that a deletion-only commit is not mistaken for an
/// empty index.
fn staged_anything(root: &Path) -> Result<Vec<String>, String> {
    Ok(nul_list(&git(
        root,
        &["diff", "--cached", "--name-only", "-z"],
        None,
        "staged path scan",
    )?))
}

fn changed_paths(root: &Path, commit: &str) -> Result<Vec<String>, String> {
    let paths = nul_list(&git(
        root,
        &[
            "diff-tree",
            "--root",
            "--no-commit-id",
            "--name-only",
            "--diff-filter=ACMRT",
            "-r",
            "-z",
            commit,
        ],
        None,
        &format!("commit path scan {commit}"),
    )?);
    let mut kept = Vec::new();
    for path in paths {
        let ls_tree =
            String::from_utf8_lossy(&git(root, &["ls-tree", commit, &path], None, "ls-tree")?)
                .to_string();
        if !ls_tree.is_empty() && !ls_tree.contains(" commit ") {
            kept.push(path);
        }
    }
    Ok(kept)
}

/// Gate the index.
///
/// PORT NOTE: the TypeScript passes trivially when nothing is staged at all --
/// a gate that can pass without looking at anything is the defect this gate
/// exists to police. This port fails instead.
pub fn check_staged(root: &Path) -> Result<(), String> {
    if staged_anything(root)?.is_empty() {
        return Err("publication gate scanned nothing: no staged change to inspect".to_string());
    }
    let entries: Vec<Entry> = staged_paths(root)?
        .into_iter()
        .map(|path| {
            let owned = path.clone();
            Entry {
                scope: "staged".to_string(),
                path,
                data: Box::new(move || {
                    git(
                        root,
                        &["show", &format!(":{owned}")],
                        None,
                        &format!("staged blob {owned}"),
                    )
                }),
            }
        })
        .collect();
    reject(&entries)?;
    // Conflict markers are current-tree hygiene, so scan staged content only.
    // The publication scan over immutable outgoing history remains separate.
    let mut failures: Vec<String> = Vec::new();
    for entry in &entries {
        if let Some(reason) = conflict_marker_reason(&entry.path, &entry.data()?) {
            failures.push(format!("{} {}: {reason}", entry.scope, entry.path));
        }
    }
    if failures.is_empty() {
        Ok(())
    } else {
        Err(format!("publication gate rejected:\n{}", failures.join("\n")))
    }
}

fn revisions(root: &Path, local: &str, remote: &str) -> Result<Vec<String>, String> {
    let negated = format!("^{remote}");
    let mut args = vec!["rev-list", local];
    if !zero_oid(remote) {
        args.push(&negated);
    }
    Ok(lines(&git(
        root,
        &args,
        None,
        &format!("outgoing revision scan {local}"),
    )?))
}

fn commit_message(root: &Path, commit: &str) -> Result<String, String> {
    Ok(String::from_utf8_lossy(&git(
        root,
        &["log", "-1", "--format=%B", commit],
        None,
        &format!("commit message {commit}"),
    )?)
    .to_string())
}

/// Gate outgoing history. `updates` is the pre-push hook's stdin.
///
/// PORT NOTE: the TypeScript passes trivially on empty stdin. This port fails
/// instead, for the same reason `check_staged` does. An update list that names
/// only deletions still scans nothing and still passes, which is correct: there
/// is genuinely no outgoing content in that push.
///
/// Returns the message failures it printed alongside the error, so a caller can
/// reproduce the TypeScript's stderr ordering.
pub fn check_push(root: &Path, updates: &str) -> Result<(), PushError> {
    let updates = lines(updates.as_bytes());
    if updates.is_empty() {
        return Err(PushError {
            message_failures: Vec::new(),
            error: "publication gate scanned nothing: no ref update on stdin".to_string(),
        });
    }
    let fail = |error: String| PushError {
        message_failures: Vec::new(),
        error,
    };
    let mut commits: Vec<String> = Vec::new();
    for update in updates {
        let fields: Vec<&str> = update.split_whitespace().collect();
        if fields.len() != 4 {
            return Err(fail("invalid pre-push update".to_string()));
        }
        let (local, remote) = (fields[1], fields[3]);
        if zero_oid(local) {
            continue;
        }
        for commit in revisions(root, local, remote).map_err(fail)? {
            if !commits.contains(&commit) {
                commits.push(commit);
            }
        }
    }
    let mut message_failures: Vec<String> = Vec::new();
    for commit in &commits {
        let message = commit_message(root, commit).map_err(fail)?;
        if let Some(reason) = commit_message_reason(&message) {
            message_failures.push(format!("{}: {reason}", &commit[..12.min(commit.len())]));
        }
    }
    if !message_failures.is_empty() {
        let count = message_failures.len();
        return Err(PushError {
            message_failures,
            error: format!("refusing to publish {count} commit message(s)"),
        });
    }
    let mut entries: Vec<Entry> = Vec::new();
    for commit in &commits {
        for path in changed_paths(root, commit).map_err(fail)? {
            let scope = commit[..12.min(commit.len())].to_string();
            let owned_commit = commit.clone();
            let owned_path = path.clone();
            entries.push(Entry {
                scope,
                path,
                data: Box::new(move || {
                    git(
                        root,
                        &["show", &format!("{owned_commit}:{owned_path}")],
                        None,
                        &format!("commit blob {owned_commit}:{owned_path}"),
                    )
                }),
            });
        }
    }
    reject(&entries).map_err(fail)
}

/// A push rejection: the per-commit lines to print, then the error itself.
#[derive(Debug)]
pub struct PushError {
    pub message_failures: Vec<String>,
    pub error: String,
}

/// Paths the gate must refuse. Shared by the binary's self-test and the tests.
pub const REJECTED_PATHS: &[&str] = &[
    "gs1-en.gba",
        "gs1-ja.gba",
        "gs1-de.gba",
        "gs1-es.gba",
        "gs1-fr.gba",
        "gs1-it.gba",
        "gs2-en.gba",
        "gs2-ja.gba",
        "gs2-de.gba",
        "gs2-es.gba",
        "gs2-fr.gba",
        "gs2-it.gba",
        "roms/private/gs1-en.gba",
        "out/diff.json",
        "work/rom.raw",
        "alchemy-gcc/bin/compiler",
        "analysis/regions.json",
        "reports/comparison.json",
        "dump.bin",
        "private-diff.json",
        "game.elf",
        "gs1-en.gba.lz",
        "regional.patch",
        "engine.bsdiff",
        ".cmatch-fresh/result.s",
        "comparisons/shared-runs.json",
        "compiler-output/function.s",
];

/// Paths the gate must let through.
pub const ACCEPTED_PATHS: &[&str] = &[
    "src/main.c",
        "asm/080000c0.s",
        "assets/graphics/title.png",
        "assets/audio/theme.mid",
        "assets/audio/wave.wav",
        "assets/data/layout.json",
        "tools/compare-roms/src/main.rs",
        "tools/build-full/src/main.rs",
        "assets/data/resource_2_build_stamp.txt",
        "assets/maps/town/metatiles.bin",
        "assets/maps/town/metatile_attributes.bin",
    "rom.sha1",
];

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    use std::path::PathBuf;

    #[test]
    fn private_paths_are_rejected() {
        assert!(!REJECTED_PATHS.is_empty(), "scanning nothing is not passing");
        for path in REJECTED_PATHS {
            assert!(
                publication_path_reason(path).is_some(),
                "private path accepted: {path}"
            );
        }
    }

    #[test]
    fn source_paths_are_accepted() {
        assert!(!ACCEPTED_PATHS.is_empty(), "scanning nothing is not passing");
        for path in ACCEPTED_PATHS {
            assert_eq!(
                publication_path_reason(path),
                None,
                "source path rejected: {path}"
            );
        }
    }

    fn fake_rom() -> Vec<u8> {
        let mut rom = vec![0u8; 0x8000];
        rom[0xb2] = 0x96;
        let mut sum: u8 = 0;
        for byte in &rom[0xa0..=0xbc] {
            sum = sum.wrapping_add(*byte);
        }
        rom[0xbd] = 0u8.wrapping_sub(sum).wrapping_sub(0x19);
        rom
    }

    #[test]
    fn disguised_binaries_are_rejected() {
        assert_eq!(
            publication_content_reason(&fake_rom()),
            Some("GBA ROM image")
        );
        assert_eq!(
            publication_content_reason(&[0x7f, 0x45, 0x4c, 0x46]),
            Some("ELF build product")
        );
        assert_eq!(
            publication_content_reason(b"!<arch>\n"),
            Some("archive or object library")
        );
        assert_eq!(publication_content_reason(b"canonical source"), None);
        assert_eq!(
            publication_entry_reason("asm/08000000.s", b".incbin \"rom.gba\"\n").as_deref(),
            Some("committed incbin payload")
        );
    }

    #[test]
    fn conflict_markers_are_rejected() {
        assert!(conflict_marker_reason("HANDOVER.md", b"a\n<<<<<<< HEAD\nb\n").is_some());
        assert!(conflict_marker_reason("HANDOVER.md", b"a\n>>>>>>> topic\n").is_some());
        // The shared entry reason must not flag markers because it also scans
        // immutable outgoing history.
        assert_eq!(
            publication_entry_reason("HANDOVER.md", b"x\n<<<<<<< HEAD\n"),
            None
        );
        // A bare ======= is a Markdown heading underline, not a conflict.
        assert_eq!(
            conflict_marker_reason("HANDOVER.md", b"Title\n=======\n\nbody\n"),
            None
        );
        // <<<<<<< without the trailing space is ordinary prose or a diff sample.
        assert_eq!(
            conflict_marker_reason("HANDOVER.md", b"see <<<<<<<HEAD in the output\n"),
            None
        );
        assert_eq!(
            conflict_marker_reason("assets/readme/x.png", b"<<<<<<< HEAD\n"),
            None
        );
    }

    #[test]
    fn byte_dumps_in_messages_are_rejected() {
        assert!(commit_message_reason("fixed the header\n\n00 11 22 33 44 55 66 77\n").is_some());
        for accepted in [
            "Close 12 owners the sweep left open\n",
            "reverts 3d36cfb0aa11bb22cc33dd44ee55ff6677889900\n",
            "resource_39b:e6c span 0x02000e6c..0x02000e78 is not audited\n",
            "the prologue pushes r7 where the reference does not\n",
            "the low halfword ff 00 stayed wrong\n",
        ] {
            assert_eq!(
                commit_message_reason(accepted),
                None,
                "a legitimate commit message was rejected: {}",
                accepted.trim()
            );
        }
    }

    // --- edge cases the TypeScript self-test missed ------------------------

    #[test]
    fn conflict_marker_reports_the_first_line() {
        assert_eq!(
            conflict_marker_reason("a.md", b"one\ntwo\n<<<<<<< HEAD\nx\n>>>>>>> t\n").as_deref(),
            Some("unresolved conflict marker at line 3; resolve the merge before committing")
        );
    }

    #[test]
    fn eight_angle_brackets_are_not_a_marker() {
        // `{7}` is exact and cannot backtrack, so an eighth bracket kills it.
        assert_eq!(conflict_marker_reason("a.md", b"<<<<<<<< HEAD\n"), None);
        assert!(conflict_marker_reason("a.md", b"<<<<<<< \n").is_some());
    }

    #[test]
    fn byte_dump_needs_eight_pairs_in_a_row() {
        assert_eq!(commit_message_reason("00 11 22 33 44 55 66"), None);
        assert!(commit_message_reason("00 11 22 33 44 55 66 77").is_some());
        assert!(commit_message_reason("zz 00 11 22 33 44 55 66 77 zz").is_some());
        // A non-space separator breaks the run.
        assert_eq!(commit_message_reason("00 11 22 33,44 55 66 77"), None);
        // Tabs are allowed by the pattern.
        assert!(commit_message_reason("00\t11\t22\t33\t44\t55\t66\t77").is_some());
        // A three-character word is not a byte pair.
        assert_eq!(commit_message_reason("00 11 22 33 444 55 66 77 88"), None);
        // Nine pairs still trip it.
        assert!(commit_message_reason("00 11 22 33 44 55 66 77 88").is_some());
    }

    #[test]
    fn private_report_needs_a_delimited_word() {
        assert_eq!(
            publication_path_reason("notes/rundiffs.json"),
            None,
            "an embedded word must not trip the report rule"
        );
        assert_eq!(
            publication_path_reason("notes/run-diff.json"),
            Some("private analysis report")
        );
        assert_eq!(
            publication_path_reason("notes/DIFF.JSON"),
            Some("private analysis report")
        );
        // The report rule only applies to report extensions.
        assert_eq!(publication_path_reason("notes/diff.md"), None);
    }

    #[test]
    fn escapes_and_absolutes_are_invalid_paths() {
        assert_eq!(
            publication_path_reason("/etc/passwd"),
            Some("invalid repository path")
        );
        assert_eq!(
            publication_path_reason("src/../out/x.txt"),
            Some("invalid repository path")
        );
        // Backslashes normalise to separators before the directory check.
        assert_eq!(
            publication_path_reason("out\\thing.txt"),
            Some("private or generated directory")
        );
    }

    #[test]
    fn canonical_map_binaries_survive_only_under_assets_maps() {
        assert_eq!(publication_path_reason("assets/maps/t/metatiles.bin"), None);
        assert_eq!(
            publication_path_reason("data/metatiles.bin"),
            Some("private or generated file type")
        );
    }

    #[test]
    fn baserom_names_are_private() {
        assert_eq!(
            publication_path_reason("baserom"),
            Some("private ROM name")
        );
        assert_eq!(
            publication_path_reason("src/baserom.agb"),
            Some("private ROM name")
        );
        assert_eq!(
            publication_path_reason("src/a.rom.txt"),
            Some("private ROM name")
        );
    }

    #[test]
    fn gba_header_must_match_exactly() {
        let mut rom = fake_rom();
        assert!(publication_content_reason(&rom).is_some());
        rom[0xbd] ^= 1;
        assert_eq!(publication_content_reason(&rom), None, "bad complement");
        let short = fake_rom()[..0x4000].to_vec();
        assert_eq!(publication_content_reason(&short), None, "bad size");
    }

    #[test]
    fn wasm_and_mach_o_magics_are_executables() {
        assert_eq!(
            publication_content_reason(&[0x00, 0x61, 0x73, 0x6d]),
            Some("native executable")
        );
        assert_eq!(
            publication_content_reason(&[0xcf, 0xfa, 0xed, 0xfe]),
            Some("native executable")
        );
        assert_eq!(publication_content_reason(b"MZ"), Some("native executable"));
    }

    #[test]
    fn incbin_matches_only_at_a_line_start() {
        assert!(
            publication_entry_reason("a.s", b"\n   \t.INCBIN \"x\"\n").is_some(),
            "leading whitespace and case must still match"
        );
        assert_eq!(
            publication_entry_reason("a.s", b"call .incbinned\n"),
            None,
            "a word boundary is required after .incbin"
        );
        assert_eq!(
            publication_entry_reason("a.c", b".incbin \"x\"\n"),
            None,
            "only asm sources are scanned for incbin"
        );
    }

    // --- end-to-end against a temporary tree -------------------------------

    struct Temp(PathBuf);

    impl Drop for Temp {
        fn drop(&mut self) {
            let _ = fs::remove_dir_all(&self.0);
        }
    }

    fn temp_repo(name: &str) -> Temp {
        let root = std::env::temp_dir().join(format!("check-publication-{name}-{}", std::process::id()));
        let _ = fs::remove_dir_all(&root);
        fs::create_dir_all(&root).unwrap();
        for args in [
            vec!["init", "-q", "-b", "main"],
            vec!["config", "user.email", "t@example.com"],
            vec!["config", "user.name", "Test"],
            vec!["config", "commit.gpgsign", "false"],
        ] {
            git(&root, &args, None, "setup").unwrap();
        }
        Temp(root)
    }

    fn write(root: &Path, path: &str, body: &[u8]) {
        let full = root.join(path);
        fs::create_dir_all(full.parent().unwrap()).unwrap();
        fs::write(full, body).unwrap();
    }

    #[test]
    fn staged_scan_gates_a_temp_tree() {
        let repo = temp_repo("staged");
        let root = repo.0.as_path();

        // An empty index is a scan of nothing, and that is not a pass.
        let empty = check_staged(root).unwrap_err();
        assert_eq!(
            empty,
            "publication gate scanned nothing: no staged change to inspect"
        );

        write(root, "src/main.c", b"int main(void) { return 0; }\n");
        git(root, &["add", "src/main.c"], None, "add").unwrap();
        check_staged(root).unwrap();

        write(root, "out/report.json", b"{}\n");
        git(root, &["add", "-f", "out/report.json"], None, "add").unwrap();
        assert_eq!(
            check_staged(root).unwrap_err(),
            "publication gate rejected:\nstaged out/report.json: private or generated directory"
        );
    }

    #[test]
    fn staged_scan_catches_content_and_markers() {
        let repo = temp_repo("content");
        let root = repo.0.as_path();

        write(root, "notes.md", b"a\n<<<<<<< HEAD\nb\n");
        git(root, &["add", "notes.md"], None, "add").unwrap();
        assert_eq!(
            check_staged(root).unwrap_err(),
            "publication gate rejected:\nstaged notes.md: unresolved conflict marker at line 2; resolve the merge before committing"
        );

        write(root, "notes.md", b"clean\n");
        write(root, "tool.sha1", &[0x7f, 0x45, 0x4c, 0x46, 0, 0]);
        git(root, &["add", "notes.md", "tool.sha1"], None, "add").unwrap();
        assert_eq!(
            check_staged(root).unwrap_err(),
            "publication gate rejected:\nstaged tool.sha1: ELF build product"
        );
    }

    #[test]
    fn push_scan_gates_outgoing_history() {
        let repo = temp_repo("push");
        let root = repo.0.as_path();

        assert_eq!(
            check_push(root, "").unwrap_err().error,
            "publication gate scanned nothing: no ref update on stdin"
        );

        write(root, "src/main.c", b"int main(void) { return 0; }\n");
        git(root, &["add", "src/main.c"], None, "add").unwrap();
        git(root, &["commit", "-q", "-m", "seed"], None, "commit").unwrap();
        let head = String::from_utf8_lossy(&git(root, &["rev-parse", "HEAD"], None, "rev").unwrap())
            .trim()
            .to_string();
        let zero = "0".repeat(40);
        let update = format!("refs/heads/main {head} refs/heads/main {zero}\n");
        check_push(root, &update).unwrap();

        // A commit that publishes a private blob.
        write(root, "dump.bin", b"payload");
        git(root, &["add", "-f", "dump.bin"], None, "add").unwrap();
        git(root, &["commit", "-q", "-m", "leak"], None, "commit").unwrap();
        let head = String::from_utf8_lossy(&git(root, &["rev-parse", "HEAD"], None, "rev").unwrap())
            .trim()
            .to_string();
        let update = format!("refs/heads/main {head} refs/heads/main {zero}\n");
        let failed = check_push(root, &update).unwrap_err();
        assert!(
            failed
                .error
                .contains("dump.bin: private or generated file type"),
            "{}",
            failed.error
        );

        // A malformed update line is rejected before any git call.
        assert_eq!(
            check_push(root, "three fields only").unwrap_err().error,
            "invalid pre-push update"
        );
    }

    #[test]
    fn push_scan_gates_commit_messages() {
        let repo = temp_repo("message");
        let root = repo.0.as_path();
        write(root, "src/main.c", b"int main(void) { return 0; }\n");
        git(root, &["add", "src/main.c"], None, "add").unwrap();
        git(
            root,
            &["commit", "-q", "-m", "header\n\n00 11 22 33 44 55 66 77\n"],
            None,
            "commit",
        )
        .unwrap();
        let head = String::from_utf8_lossy(&git(root, &["rev-parse", "HEAD"], None, "rev").unwrap())
            .trim()
            .to_string();
        let zero = "0".repeat(40);
        let failed = check_push(
            root,
            &format!("refs/heads/main {head} refs/heads/main {zero}\n"),
        )
        .unwrap_err();
        assert_eq!(failed.error, "refusing to publish 1 commit message(s)");
        assert_eq!(failed.message_failures.len(), 1);
        assert!(
            failed.message_failures[0]
                .ends_with(": commit message contains a raw byte dump"),
            "{}",
            failed.message_failures[0]
        );
    }
}
