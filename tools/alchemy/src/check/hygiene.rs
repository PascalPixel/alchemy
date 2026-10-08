//! Repository hygiene over tracked files: no scratch, build output or stray
//! binaries, no empty files, text in UTF-8, and in games/ and tools/ no
//! CRLF, trailing whitespace or missing final newline.

use std::path::Path;
use std::process::{Command, ExitCode};

const USAGE: &str = "usage: alchemy check hygiene\n\
alchemy check hygiene --fix [--skip PREFIX]...\n\
Fails on tracked .DS_Store, scratch and build output (*.o, *.elf, *.map,\n\
*.log, *.i, backups, out/), binaries other than PNG, WAV, MIDI and PCM4\n\
assets, empty files and text that is not UTF-8; and, in games/ and tools/,\n\
on CRLF line endings, trailing whitespace (TSV fields excepted) and a\n\
missing final newline.";

const BINARY: [&str; 4] = ["png", "wav", "mid", "pcm4"];
const SCRATCH: [&str; 14] = [
    ".o", ".elf", ".map", ".log", ".i", ".gba", ".bin", ".lz", ".orig", ".rej", ".swp", ".tmp",
    ".bak", "~",
];

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    if arguments.first().is_some_and(|flag| flag == "--fix") {
        let skip = arguments[1..].iter().filter(|a| *a != "--skip").cloned().collect::<Vec<_>>();
        return match fix(crate::compiler::routing::root(), &skip) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }
    if !arguments.is_empty() {
        println!("{USAGE}");
        return if arguments == ["-h"] || arguments == ["--help"] {
            ExitCode::SUCCESS
        } else {
            ExitCode::from(2)
        };
    }
    let root = crate::compiler::routing::root();
    match scan(root) {
        Ok(problems) if problems.is_empty() => ExitCode::SUCCESS,
        Ok(problems) => {
            for problem in &problems {
                eprintln!("{problem}");
            }
            eprintln!("error: {} tracked file(s) fail repository hygiene", problems.len());
            ExitCode::FAILURE
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn scan(root: &Path) -> Result<Vec<String>, String> {
    let output = Command::new("git")
        .args(["ls-files", "-z", "-s"])
        .current_dir(root)
        .output()
        .map_err(|error| format!("git ls-files: {error}"))?;
    let mut problems = Vec::new();
    for record in output.stdout.split(|byte| *byte == 0).filter(|r| !r.is_empty()) {
        let record = String::from_utf8_lossy(record);
        let Some((meta, path)) = record.split_once('\t') else { continue };
        if meta.starts_with("160000") {
            continue;
        }
        // A file deleted in the working tree is not this check's business.
        let Ok(bytes) = std::fs::read(root.join(path)) else { continue };
        problems.extend(reasons(path, &bytes).into_iter().map(|reason| format!("{path}: {reason}")));
    }
    Ok(problems)
}

pub(crate) fn reasons(path: &str, bytes: &[u8]) -> Vec<&'static str> {
    let name = path.rsplit('/').next().unwrap_or(path);
    let extension = name.rsplit_once('.').map_or("", |(_, e)| e);
    if name == ".DS_Store" {
        return vec!["Finder metadata"];
    }
    if path.starts_with("out/") || path.starts_with("tools/out/") || SCRATCH.iter().any(|s| name.ends_with(s)) {
        return vec!["scratch or build output"];
    }
    if bytes.is_empty() {
        return vec!["empty file"];
    }
    if BINARY.contains(&extension) {
        return Vec::new();
    }
    if bytes.contains(&0) {
        return vec!["stray binary"];
    }
    let Ok(text) = std::str::from_utf8(bytes) else {
        return vec!["not UTF-8"];
    };
    let mut found = Vec::new();
    if !(path.starts_with("games/") || path.starts_with("tools/")) {
        return found;
    }
    if text.contains("\r\n") {
        found.push("CRLF line endings");
    }
    if !extension.eq_ignore_ascii_case("tsv")
        && text.lines().any(|line| line.ends_with(' ') || line.ends_with('\t'))
    {
        found.push("trailing whitespace");
    }
    if !text.ends_with('\n') {
        found.push("no final newline");
    }
    found
}

/// Fixes line endings, trailing whitespace and final newlines in place,
/// leaving files under the skipped prefixes alone.
fn fix(root: &Path, skip: &[String]) -> Result<(), String> {
    for problem in scan(root)? {
        let (path, reason) = problem.rsplit_once(": ").unwrap_or((&problem, ""));
        if !matches!(reason, "CRLF line endings" | "trailing whitespace" | "no final newline")
            || skip.iter().any(|prefix| path.starts_with(prefix.as_str()))
        {
            continue;
        }
        let file = root.join(path);
        let text = std::fs::read_to_string(&file).map_err(|e| format!("{path}: {e}"))?;
        std::fs::write(&file, fixed(&text)).map_err(|e| format!("{path}: {e}"))?;
    }
    Ok(())
}

/// Rewrites a text file's line endings, trailing whitespace and final
/// newline; used to fix what the check finds.
pub(crate) fn fixed(text: &str) -> String {
    let mut out: String = text
        .replace("\r\n", "\n")
        .lines()
        .map(|line| line.trim_end_matches([' ', '\t']))
        .collect::<Vec<_>>()
        .join("\n");
    out.push('\n');
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn each_kind_of_problem_is_named() {
        assert_eq!(reasons("a/.DS_Store", b"x"), ["Finder metadata"]);
        assert_eq!(reasons("games/x.o", b"x"), ["scratch or build output"]);
        assert_eq!(reasons("games/x.c", b""), ["empty file"]);
        assert!(reasons("games/x.png", b"\0\x89").is_empty());
        assert_eq!(reasons("games/x.dat", b"a\0b"), ["stray binary"]);
        assert_eq!(reasons("games/x.c", b"\xff\xfe"), ["not UTF-8"]);
        assert_eq!(
            reasons("games/x.c", b"int a; \r\nint b;"),
            ["CRLF line endings", "trailing whitespace", "no final newline"]
        );
        assert!(reasons("games/x.tsv", b"a\t\n").is_empty());
        assert!(reasons("recon/x.s", b"a \n").is_empty());
    }

    #[test]
    fn fixing_leaves_clean_text() {
        let text = fixed("int a; \r\nint b;\t");
        assert_eq!(text, "int a;\nint b;\n");
        assert!(reasons("games/x.c", text.as_bytes()).is_empty());
    }
}
