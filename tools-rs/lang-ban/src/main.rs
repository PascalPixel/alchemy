// Our own codebase is ASM, C, Rust, WAV, PCM -- and JSON as a fallback.
//
// This check fails when a file with a banned extension exists under `tools/` or
// `tools-rs/` that is not in the allowlist. The allowlist is the grandfathered
// set captured when the ban landed; it may shrink and must never grow. A port
// lands in `tools-rs/`, its callers move, and only then does the original leave
// both the tree and this file.
//
//   cargo run -p lang-ban            # check
//   cargo run -p lang-ban -- --stat  # how far the port has to go
//
// Deleting ahead of the port is the failure mode this ordering exists to
// prevent: every parallel runner imports tools/lib/jobs.ts, so removing it
// before its callers are Rust breaks the whole tree at once.
//
// Judgement calls, recorded because they are not derivable from the rule:
//
//   * TOML and Markdown are NOT banned. They are config and docs, not code.
//     `Cargo.toml` is non-negotiable -- Rust cannot build without it -- and
//     banning `.md` would ban AGENTS.md, the file that states the rule. The
//     allowlist covers languages that execute; it does not cover the manifest
//     and prose that describe what executes.
//
//   * Third-party clones living outside this repository are exempt by
//     construction, not by exception. decomp-permuter at
//     ~/Developer/pret/decomp-permuter is Python, and stays Python: it is not
//     our codebase. lang-ban walks `tools/` and `tools-rs/` under the
//     repository root and nothing else, so an outside clone is never reached.
//
//   * Scanning nothing is a failure, not a pass. A check that silently
//     succeeds when its roots vanish or its walk breaks is worse than no check
//     at all, so an empty scan exits non-zero.

use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

/// Extensions that may not appear under the scanned roots.
/// Executable languages only -- see the module comment on TOML and Markdown.
const BANNED: [&str; 8] = ["ts", "js", "mjs", "cjs", "py", "sh", "css", "html"];
const SCANNED: [&str; 2] = ["tools", "tools-rs"];

fn repo_root() -> PathBuf {
    // The crate sits at <root>/tools-rs/lang-ban.
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .ancestors()
        .nth(2)
        .expect("crate must live two levels below the repository root")
        .to_path_buf()
}

fn allowlist_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).join("allowlist.txt")
}

fn read_allowlist() -> BTreeSet<String> {
    let path = allowlist_path();
    let text = fs::read_to_string(&path).unwrap_or_default();
    text.lines()
        .map(str::trim)
        .filter(|line| !line.is_empty() && !line.starts_with('#'))
        .map(str::to_string)
        .collect()
}

fn banned_extension(path: &Path) -> bool {
    path.extension()
        .and_then(|e| e.to_str())
        .is_some_and(|e| BANNED.contains(&e))
}

/// What a walk of the scanned roots saw.
struct Scan {
    /// Every file visited, banned or not. Zero here means the check is broken.
    visited: usize,
    /// Relative paths of the files carrying a banned extension.
    found: BTreeSet<String>,
}

fn walk(dir: &Path, root: &Path, scan: &mut Scan) {
    let Ok(entries) = fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        let name = entry.file_name();
        let name = name.to_string_lossy();
        if name == "node_modules" || name == "target" || name.starts_with('.') {
            continue;
        }
        if path.is_dir() {
            walk(&path, root, scan);
        } else {
            scan.visited += 1;
            if banned_extension(&path) {
                if let Ok(relative) = path.strip_prefix(root) {
                    scan.found
                        .insert(relative.to_string_lossy().replace('\\', "/"));
                }
            }
        }
    }
}

fn scan(root: &Path) -> Scan {
    let mut scan = Scan {
        visited: 0,
        found: BTreeSet::new(),
    };
    for dir in SCANNED {
        walk(&root.join(dir), root, &mut scan);
    }
    scan
}

/// How many of `paths` carry each banned extension, for the per-extension report.
fn by_extension(paths: &BTreeSet<String>) -> BTreeMap<&'static str, usize> {
    let mut counts = BTreeMap::new();
    for path in paths {
        if let Some(ext) = Path::new(path).extension().and_then(|e| e.to_str()) {
            if let Some(known) = BANNED.iter().find(|b| **b == ext) {
                *counts.entry(*known).or_insert(0) += 1;
            }
        }
    }
    counts
}

fn render_counts(counts: &BTreeMap<&'static str, usize>) -> String {
    counts
        .iter()
        .map(|(ext, n)| format!(".{ext}={n}"))
        .collect::<Vec<_>>()
        .join(" ")
}

fn main() -> ExitCode {
    let root = repo_root();
    let Scan { visited, found } = scan(&root);
    let allowed = read_allowlist();

    if std::env::args().any(|a| a == "--write-allowlist") {
        let mut body = String::from(
            "# Grandfathered banned-language files under tools/ and tools-rs/.\n\
             # This list may shrink and must never grow. See AGENTS.md, Hard rules.\n",
        );
        for path in &found {
            body.push_str(path);
            body.push('\n');
        }
        fs::write(allowlist_path(), body).expect("allowlist must be writable");
        println!("lang-ban: wrote {} grandfathered paths", found.len());
        return ExitCode::SUCCESS;
    }

    // Scanning nothing is not passing. If the roots are gone or the walk broke,
    // every subsequent comparison is vacuously clean, so refuse to report ok.
    if visited == 0 {
        eprintln!(
            "lang-ban: walked {} and visited zero files -- the check is not running.",
            SCANNED.join(", ")
        );
        return ExitCode::FAILURE;
    }

    let added: Vec<&String> = found.difference(&allowed).collect();
    let removed: Vec<&String> = allowed.difference(&found).collect();
    let counts = by_extension(&found);

    if std::env::args().any(|a| a == "--stat") {
        println!(
            "lang-ban: {} banned-language files remain ({}), {} ported out so far",
            found.len(),
            render_counts(&counts),
            removed.len()
        );
        return ExitCode::SUCCESS;
    }

    if added.is_empty() {
        println!(
            "lang-ban ok: scanned {visited} files under {}, {} grandfathered ({}), {} ported out, 0 violations",
            SCANNED.join(" and "),
            found.len(),
            render_counts(&counts),
            removed.len()
        );
        return ExitCode::SUCCESS;
    }

    eprintln!(
        "lang-ban: {} new banned-language file(s) under {}:",
        added.len(),
        SCANNED.join(", ")
    );
    for path in added {
        eprintln!("  {path}");
    }
    eprintln!("New tooling goes in tools-rs/ as a workspace crate. See AGENTS.md, Hard rules.");
    ExitCode::FAILURE
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::atomic::{AtomicUsize, Ordering};

    /// A throwaway repository root with a `tools/` directory, so the tests never
    /// depend on the real tree.
    struct Fixture {
        root: PathBuf,
    }

    impl Fixture {
        fn new() -> Self {
            static N: AtomicUsize = AtomicUsize::new(0);
            let root = std::env::temp_dir().join(format!(
                "lang-ban-test-{}-{}",
                std::process::id(),
                N.fetch_add(1, Ordering::Relaxed)
            ));
            let _ = fs::remove_dir_all(&root);
            fs::create_dir_all(root.join("tools")).expect("fixture root");
            fs::create_dir_all(root.join("tools-rs")).expect("fixture root");
            Fixture { root }
        }

        fn write(&self, relative: &str, body: &str) {
            let path = self.root.join(relative);
            fs::create_dir_all(path.parent().expect("parent")).expect("mkdir");
            fs::write(path, body).expect("write");
        }
    }

    impl Drop for Fixture {
        fn drop(&mut self) {
            let _ = fs::remove_dir_all(&self.root);
        }
    }

    #[test]
    fn new_banned_extensions_are_caught() {
        let fixture = Fixture::new();
        for ext in BANNED {
            fixture.write(&format!("tools/new_file.{ext}"), "x");
        }
        let found = scan(&fixture.root).found;
        assert_eq!(found.len(), BANNED.len(), "every banned extension is found");
        for ext in BANNED {
            assert!(
                found.contains(&format!("tools/new_file.{ext}")),
                ".{ext} must be banned"
            );
        }
        // With an empty allowlist every one of them is a violation.
        let allowed: BTreeSet<String> = BTreeSet::new();
        assert_eq!(found.difference(&allowed).count(), BANNED.len());
    }

    #[test]
    fn allowed_extensions_pass() {
        let fixture = Fixture::new();
        for ext in ["rs", "c", "h", "s", "json", "toml", "md", "wav", "pcm"] {
            fixture.write(&format!("tools/new_file.{ext}"), "x");
            fixture.write(&format!("tools-rs/new_file.{ext}"), "x");
        }
        assert!(
            scan(&fixture.root).found.is_empty(),
            "code, config, docs and audio are allowed"
        );
    }

    #[test]
    fn grandfathering_still_works() {
        let fixture = Fixture::new();
        fixture.write("tools/old.ts", "x");
        let found = scan(&fixture.root).found;
        let allowed: BTreeSet<String> = ["tools/old.ts".to_string()].into_iter().collect();
        assert!(
            found.difference(&allowed).next().is_none(),
            "an allowlisted file passes"
        );

        fixture.write("tools/fresh.py", "x");
        let found = scan(&fixture.root).found;
        let added: Vec<&String> = found.difference(&allowed).collect();
        assert_eq!(added, vec![&"tools/fresh.py".to_string()]);
    }

    #[test]
    fn both_roots_are_scanned() {
        let fixture = Fixture::new();
        fixture.write("tools/a.ts", "x");
        fixture.write("tools-rs/b.ts", "x");
        fixture.write("elsewhere/c.ts", "x");
        let found = scan(&fixture.root).found;
        assert_eq!(
            found,
            ["tools-rs/b.ts".to_string(), "tools/a.ts".to_string()]
                .into_iter()
                .collect::<BTreeSet<_>>(),
            "only tools/ and tools-rs/ are policed"
        );
    }

    #[test]
    fn scanning_nothing_is_not_passing() {
        let fixture = Fixture::new();
        let scan = scan(&fixture.root);
        assert!(scan.found.is_empty());
        // Zero files visited is the exact condition main() turns into a
        // non-zero exit: an empty tree must never report ok.
        assert_eq!(scan.visited, 0);
    }

    #[test]
    fn per_extension_counts_are_reported() {
        let fixture = Fixture::new();
        fixture.write("tools/a.ts", "x");
        fixture.write("tools/b.ts", "x");
        fixture.write("tools/c.py", "x");
        let counts = by_extension(&scan(&fixture.root).found);
        assert_eq!(counts.get("ts"), Some(&2));
        assert_eq!(counts.get("py"), Some(&1));
        assert_eq!(render_counts(&counts), ".py=1 .ts=2");
    }

    #[test]
    fn the_real_tree_scans_something() {
        // Guards the roots against being renamed out from under the check.
        let scan = scan(&repo_root());
        assert!(scan.visited > 0, "the real roots must contain files");
    }
}
