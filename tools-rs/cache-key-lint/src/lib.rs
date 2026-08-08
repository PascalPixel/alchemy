// Forbid hand-maintained version literals in cache keys.
//
// Ported from tools/check/cache_key_lint.ts, with the provenance note kept
// because it records a real incident rather than a style preference.
//
// WHY THIS EXISTS. One stale `out/cache/overlay-c` entry affected multiple
// checkouts. The key carried `overlay-c-v3`, a string bumped by hand whenever
// the post-compile rewriting changed. It was bumped correctly and the comment
// explaining it was honest; the defect was the MECHANISM, which holds only
// while every future editor remembers. When it was not bumped, a stale entry
// served bytes that no longer reconstruct resource_39c's LZ plan under a key
// the tool accepts, `verify` died in `build_assets`, and made `main` appear
// red. It was not: switching commits does not touch `out/`, so repeated runs
// shared the same poisoned entry.
//
// THE RULE. A cache key must be a function of its inputs — the source bytes it
// derives from, and a digest of the TOOL'S OWN SOURCE for the logic applied to
// them. Never a literal describing the logic. `-vN` is a promise about memory;
// a source digest is a measurement.
//
// PORT NOTE. The scan now covers `.rs` as well as `.ts`, and the key-context
// vocabulary gained the Rust spellings (`Hasher`, `Sha256`, `update`). A lint
// that only understood the language being ported away from would go quietly
// blind exactly as the tooling moved.

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Finding {
    pub file: String,
    pub line: usize,
    pub text: String,
}

/// Lines that construct a digest. A version literal only matters in a key.
fn is_key_context(line: &str) -> bool {
    const MARKERS: [&str; 7] =
        ["update(", "cryptohasher", "cachekey", "cache_key", "digest", "hasher", "sha256"];
    let lowered = line.to_ascii_lowercase();
    MARKERS.iter().any(|marker| lowered.contains(marker))
}

/// A hand-maintained version literal inside a string: `foo-v3:` or a bare `-v12`
/// before a separator. Deliberately narrow — it matches the shape that actually
/// caused the outage and not, say, a filename or a URL.
///
/// Equivalent to `/["'`][a-z0-9-]*-v\d+\s*[:\0]/i` in the TypeScript, hand-rolled
/// so the crate needs no regex dependency.
fn has_version_literal(line: &str) -> bool {
    let bytes = line.as_bytes();
    for (start, byte) in bytes.iter().enumerate() {
        if !matches!(byte, b'"' | b'\'' | b'`') {
            continue;
        }
        let mut at = start + 1;
        // `[a-z0-9-]*` then the `-v` that opens the version.
        let run_start = at;
        while at < bytes.len()
            && (bytes[at].is_ascii_alphanumeric() || bytes[at] == b'-')
            && !bytes[at].is_ascii_uppercase()
        {
            at += 1;
        }
        // Walk back to the last `-v` inside the run and require digits after it.
        let run = &line[run_start..at];
        let Some(marker) = run.to_ascii_lowercase().rfind("-v") else { continue };
        let mut digits = run_start + marker + 2;
        let digit_start = digits;
        while digits < bytes.len() && bytes[digits].is_ascii_digit() {
            digits += 1;
        }
        if digits == digit_start {
            continue;
        }
        let mut after = digits;
        while after < bytes.len() && (bytes[after] as char).is_whitespace() {
            after += 1;
        }
        if after < bytes.len() && (bytes[after] == b':' || bytes[after] == 0) {
            return true;
        }
        // A `\0` written as an escape in source is two characters.
        if line[after..].starts_with("\\0") {
            return true;
        }
    }
    false
}

/// Scan one file's text. Pure, so the tests need no tree access.
pub fn find_violations(file: &str, text: &str) -> Vec<Finding> {
    let mut found = Vec::new();
    for (index, raw) in text.lines().enumerate() {
        let line = raw.trim();
        // A comment may name the old spelling while explaining why it is gone.
        if line.starts_with("//") || line.starts_with('*') || line.starts_with("/*") {
            continue;
        }
        if is_key_context(line) && has_version_literal(line) {
            found.push(Finding { file: file.to_string(), line: index + 1, text: line.to_string() });
        }
    }
    found
}

/// Every `.ts` and `.rs` under `directory`, relative-pathed and sorted.
/// Recursive: tools live in domain folders, and a flat read would lint only the
/// dispatchers while reporting success. `target/` is skipped — it holds build
/// output, not tooling.
pub fn scannable_files(directory: &Path) -> io::Result<Vec<String>> {
    fn walk(at: &Path, prefix: &str, into: &mut Vec<String>) -> io::Result<()> {
        let mut entries: Vec<PathBuf> =
            fs::read_dir(at)?.filter_map(Result::ok).map(|entry| entry.path()).collect();
        entries.sort();
        for path in entries {
            let Some(name) = path.file_name().and_then(|name| name.to_str()) else { continue };
            let relative =
                if prefix.is_empty() { name.to_string() } else { format!("{prefix}/{name}") };
            if path.is_dir() {
                if name == "target" || name == "node_modules" {
                    continue;
                }
                walk(&path, &relative, into)?;
            } else if name.ends_with(".ts") || name.ends_with(".rs") {
                into.push(relative);
            }
        }
        Ok(())
    }
    let mut files = Vec::new();
    if directory.is_dir() {
        walk(directory, "", &mut files)?;
    }
    files.sort();
    Ok(files)
}

#[cfg(test)]
mod tests {
    use super::*;

    // The fixtures are ASSEMBLED rather than written out, so this file does not
    // itself contain the pattern. The alternative was exempting this file from
    // the scan, and an exception in a rule is the same author's-memory defect
    // the rule exists to remove.

    #[test]
    fn a_versioned_key_literal_is_caught() {
        let offending = format!("digest.update(`overlay-c-{}:${{hex(address)}}`);", "v3");
        assert_eq!(find_violations("t.ts", &offending).len(), 1);
    }

    #[test]
    fn a_source_keyed_digest_passes() {
        let fixed = "digest.update(`overlay-c:${selfDigest()}:${hex(address)}\\0`);";
        assert_eq!(find_violations("t.ts", fixed), Vec::new());
    }

    #[test]
    fn a_comment_naming_the_old_spelling_is_not_flagged() {
        // Or the history could never be written down next to the fix.
        let comment = format!("// it was `overlay-c-{}:` before, bumped by hand", "v3");
        assert_eq!(find_violations("t.ts", &comment), Vec::new());
    }

    #[test]
    fn a_version_literal_outside_a_key_context_is_not_flagged() {
        let url = format!("const url = \"https://example.invalid/api-{}:x\";", "v2");
        assert_eq!(find_violations("t.ts", &url), Vec::new());
    }

    #[test]
    fn both_quote_styles_are_caught() {
        // Both directions, so the rule cannot rot into always-passing.
        let double = format!("key.update(\"asm-{}:\" + address);", "v1");
        assert_eq!(find_violations("t.ts", &double).len(), 1);
        let single = format!("key.update('asm-{}:' + address);", "v1");
        assert_eq!(find_violations("t.ts", &single).len(), 1);
    }

    #[test]
    fn the_rust_spellings_are_understood() {
        let rust = format!("hasher.update(format!(\"overlay-c-{}:{{address:x}}\").as_bytes());", "v3");
        assert_eq!(find_violations("t.rs", &rust).len(), 1, "a Rust cache key must be caught too");
        let fixed = "hasher.update(format!(\"overlay-c:{}:{address:x}\", self_digest()).as_bytes());";
        assert_eq!(find_violations("t.rs", fixed), Vec::new());
    }

    #[test]
    fn rust_and_typescript_are_both_scanned_and_target_is_skipped() {
        let dir = std::env::temp_dir().join(format!("alchemy-cache-key-lint-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("sub")).unwrap();
        fs::create_dir_all(dir.join("target")).unwrap();
        for name in ["b.ts", "a.rs", "notes.md"] {
            fs::write(dir.join(name), b"").unwrap();
        }
        fs::write(dir.join("sub").join("c.rs"), b"").unwrap();
        fs::write(dir.join("target").join("build.rs"), b"").unwrap();

        assert_eq!(scannable_files(&dir).unwrap(), vec!["a.rs", "b.ts", "sub/c.rs"]);
        fs::remove_dir_all(&dir).unwrap();
    }
}
