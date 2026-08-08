//! A tool path cited in a reconstructed source must resolve, or be a recorded
//! retirement.
//!
//! Reconstructed sources cite the tool that derived a fact. That is provenance,
//! not decoration, so a citation naming a file that no longer exists breaks the
//! trail. A restructure broke 802 of them across 14 tool paths and every gate
//! stayed green: broken_doc_paths checks documents, and nothing checked source.
//!
//! Repointing a citation at a different surviving tool is forbidden and this
//! cannot detect it -- it would assert a derivation that never happened. When a
//! cited tool is deleted, record it in PROVENANCE.md instead.
//!
//! Port of `tools/check/source_citations.ts`.

use std::collections::BTreeSet;
use std::path::Path;
use std::process::Command;

// PORT NOTE: the TypeScript citation pattern is /tools\/[a-z0-9_/]+\.ts/g and
// this port reproduces it exactly. An earlier draft widened it to also accept
// `tools/<crate>/<path>.rs`; that was removed. This is the gate that decides
// whether a cited tool may be deleted, so it must report what the TypeScript
// reports and nothing else -- a widened pattern would make the Rust and bun
// gates disagree about which tools are cited, which is precisely the class of
// drift this file exists to catch.
const TS_PREFIX: &[u8] = b"tools/";

fn is_ts_body(byte: u8) -> bool {
    byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_' || byte == b'/'
}

// PORT NOTE: JavaScript's \s covers a handful of exotic Unicode spaces on top of
// the ASCII set. This port matches ASCII whitespace only. A Markdown table cell
// separated from its pipe by U+2028 is not a thing that occurs, and treating the
// input as bytes keeps the scanner allocation-free.
fn is_space(byte: u8) -> bool {
    matches!(byte, b' ' | b'\t' | b'\n' | b'\r' | 0x0b | 0x0c)
}

/// One `tools/...\.ts` citation starting at `at`, if any. Returns the end
/// offset of the match.
fn citation_at(bytes: &[u8], at: usize) -> Option<usize> {
    if !bytes[at..].starts_with(TS_PREFIX) {
        return None;
    }
    let start = at + TS_PREFIX.len();
    let mut end = start;
    while end < bytes.len() && is_ts_body(bytes[end]) {
        end += 1;
    }
    // The body class excludes '.', so a shorter body can never be followed by
    // the extension's dot; the greedy run is the only candidate and the regex
    // engine's backtracking is a no-op here.
    if end == start || !bytes[end..].starts_with(b".ts") {
        return None;
    }
    Some(end + 3)
}

/// Every tool path cited in `text`, in order, with repeats.
pub fn citations(text: &str) -> Vec<&str> {
    let bytes = text.as_bytes();
    let mut found = Vec::new();
    let mut at = 0;
    // A leftmost scan, like the unanchored JavaScript regex, but skipping
    // directly to the next 't' instead of testing every offset. The prefix is
    // a literal, so an offset that is not a 't' can never begin a match; the
    // corpus is ~14 MB and the naive walk is most of the runtime.
    while at < bytes.len() {
        let Some(offset) = bytes[at..].iter().position(|&byte| byte == b't') else {
            break;
        };
        at += offset;
        match citation_at(bytes, at) {
            Some(end) => {
                found.push(&text[at..end]);
                at = end;
            }
            None => at += 1,
        }
    }
    found
}

fn skip_spaces(bytes: &[u8], mut at: usize) -> usize {
    while at < bytes.len() && is_space(bytes[at]) {
        at += 1;
    }
    at
}

/// Matches `|` whitespace `` `<tool path>` `` whitespace `|` at `at`, returning
/// the cited path and the offset just past the trailing pipe.
fn row_head(bytes: &[u8], at: usize) -> Option<(usize, usize, usize)> {
    if bytes[at] != b'|' {
        return None;
    }
    let open = skip_spaces(bytes, at + 1);
    if bytes.get(open) != Some(&b'`') {
        return None;
    }
    let path_start = open + 1;
    let path_end = citation_at(bytes, path_start)?;
    if bytes.get(path_end) != Some(&b'`') {
        return None;
    }
    let pipe = skip_spaces(bytes, path_end + 1);
    if bytes.get(pipe) != Some(&b'|') {
        return None;
    }
    Some((path_start, path_end, pipe + 1))
}

/// Tool paths recorded as retired in PROVENANCE.md's retirement table.
pub fn retired_tools(provenance: &str) -> BTreeSet<String> {
    let bytes = provenance.as_bytes();
    let mut retired = BTreeSet::new();
    let mut at = 0;
    while at < bytes.len() {
        match row_head(bytes, at) {
            Some((path_start, path_end, next)) => {
                retired.insert(provenance[path_start..path_end].to_string());
                at = next;
            }
            None => at += 1,
        }
    }
    retired
}

/// Retirement rows that also name a commit: `(tool path, sha)`.
pub fn retirement_rows(provenance: &str) -> Vec<(String, String)> {
    let bytes = provenance.as_bytes();
    let mut rows = Vec::new();
    let mut at = 0;
    while at < bytes.len() {
        let Some((path_start, path_end, after_pipe)) = row_head(bytes, at) else {
            at += 1;
            continue;
        };
        let open = skip_spaces(bytes, after_pipe);
        if bytes.get(open) != Some(&b'`') {
            at += 1;
            continue;
        }
        let sha_start = open + 1;
        let mut sha_end = sha_start;
        while sha_end < bytes.len() && matches!(bytes[sha_end], b'0'..=b'9' | b'a'..=b'f') {
            sha_end += 1;
        }
        // /[0-9a-f]{7,40}/ followed by a backtick: an over-long run cannot
        // backtrack into one, since every byte it would stop on is another hex
        // digit rather than the closing backtick.
        let length = sha_end - sha_start;
        if !(7..=40).contains(&length) || bytes.get(sha_end) != Some(&b'`') {
            at += 1;
            continue;
        }
        rows.push((
            provenance[path_start..path_end].to_string(),
            provenance[sha_start..sha_end].to_string(),
        ));
        at = sha_end + 1;
    }
    rows
}

/// Citations that name neither a live file nor a recorded retirement, most
/// widely cited first.
pub fn broken_citations(
    sources: &[(String, String)],
    exists: &dyn Fn(&str) -> bool,
    retired: &BTreeSet<String>,
) -> Vec<String> {
    let mut counts: Vec<(String, usize)> = Vec::new();
    for (_, text) in sources {
        for path in citations(text) {
            if exists(path) || retired.contains(path) {
                continue;
            }
            match counts.iter_mut().find(|(seen, _)| seen == path) {
                Some(entry) => entry.1 += 1,
                None => counts.push((path.to_string(), 1)),
            }
        }
    }
    // Stable, like Array.prototype.sort: ties keep first-citation order.
    counts.sort_by_key(|(_, count)| std::cmp::Reverse(*count));
    counts
        .into_iter()
        .map(|(path, count)| {
            format!(
                "{path} is cited by {count} source(s) but does not exist; \
                 repoint it to the tool's current path, or record its retirement in PROVENANCE.md"
            )
        })
        .collect()
}

/// The retirement table silences a broken citation, so each row must actually
/// recover. Without this it is an unbounded permission slip: add a row and any
/// citation passes, whether or not the commit it names still holds the file.
pub fn unrecoverable_retirements(provenance: &str, root: &Path) -> Vec<String> {
    let mut problems = Vec::new();
    for (path, sha) in retirement_rows(provenance) {
        let found = Command::new("git")
            .args(["cat-file", "-e", &format!("{sha}:{path}")])
            .current_dir(root)
            .output();
        let recoverable = matches!(found, Ok(output) if output.status.success());
        if !recoverable {
            problems.push(format!(
                "PROVENANCE.md claims {path} is recoverable at {sha}, but it is not there"
            ));
        }
    }
    problems
}

/// The directories a reconstructed source may live in.
pub const CORPUS: &[&str] = &["exact", "semantic", "asm", "include", "assets/code"];

// Recursive, and asm/ and include/ too. The first version read only the top
// level of exact/ and semantic/ -- which is where the citations happen to be
// today, and this gate exists precisely because "where nobody thought to check"
// is the failure mode.
fn walk(directory: &Path, prefix: &str, sources: &mut Vec<(String, String)>) {
    let Ok(entries) = std::fs::read_dir(directory) else {
        return; // The TypeScript returns early when the directory is absent.
    };
    // PORT NOTE: entries are sorted by name. readdirSync leaves them in
    // filesystem order; sorting only affects which of two equally cited broken
    // paths is reported first, and makes that tie-break reproducible.
    let mut names: Vec<_> = entries
        .flatten()
        .map(|entry| (entry.file_name(), entry.path()))
        .collect();
    names.sort_by(|left, right| left.0.cmp(&right.0));
    for (name, path) in names {
        let name = name.to_string_lossy().into_owned();
        let relative = format!("{prefix}/{name}");
        if path.is_dir() {
            walk(&path, &relative, sources);
        } else if matches!(
            name.rsplit_once('.').map(|(_, extension)| extension),
            Some("c" | "h" | "s" | "inc")
        ) {
            let Ok(bytes) = std::fs::read(&path) else {
                continue;
            };
            // readFileSync(..., "utf8") replaces invalid bytes, like from_utf8_lossy.
            sources.push((relative, String::from_utf8_lossy(&bytes).into_owned()));
        }
    }
}

/// Every reconstructed source under `root`, as `(relative path, text)`.
pub fn collect_sources(root: &Path) -> Vec<(String, String)> {
    let mut sources = Vec::new();
    for directory in CORPUS {
        walk(&root.join(directory), directory, &mut sources);
    }
    sources
}

/// The whole gate: `Ok(summary line)` or `Err(problem lines)`.
pub fn check(root: &Path) -> Result<String, Vec<String>> {
    let sources = collect_sources(root);
    let provenance = match std::fs::read_to_string(root.join("PROVENANCE.md")) {
        Ok(text) => text,
        Err(error) => return Err(vec![format!("cannot read PROVENANCE.md: {error}")]),
    };

    // PORT NOTE (deliberate divergence, and the reason it exists): the
    // TypeScript prints "source citations ok: 0 sources" and exits 0 when the
    // corpus is empty. That is indistinguishable from a clean tree, so a gate
    // that has stopped seeing the tree -- wrong root, renamed directory,
    // drifted extension list -- reads as passing. Scanning nothing is not
    // passing.
    if sources.is_empty() {
        return Err(vec![format!(
            "no sources scanned under {}; scanning nothing is not passing",
            CORPUS.join(", ")
        )]);
    }
    // The same failure one layer in. If the citation pattern stops matching,
    // every source is scanned, no citation is found, no citation is broken, and
    // the run is again indistinguishable from a clean tree. A corpus this size
    // always cites something, so a floor of one turns a silently dead pattern
    // into a failure instead of a green gate.
    let cited: usize = sources.iter().map(|(_, text)| citations(text).len()).sum();
    if cited == 0 {
        return Err(vec![format!(
            "{} sources scanned but not one citation matched; \
             the citation pattern is no longer finding anything",
            sources.len()
        )]);
    }

    let retired = retired_tools(&provenance);
    let mut problems = broken_citations(&sources, &|path| root.join(path).exists(), &retired);
    problems.extend(unrecoverable_retirements(&provenance, root));
    if !problems.is_empty() {
        return Err(problems);
    }
    Ok(format!(
        "source citations ok: {} sources, {} recorded retirement(s)",
        sources.len(),
        retired.len()
    ))
}

/// Every assertion `self_test` is expected to execute. A self-test that stops
/// checking something is the same defect as a gate that stops scanning: it
/// still prints "ok". `self_test` records each check it reaches and refuses to
/// pass unless the set is complete, so deleting an assertion fails loudly
/// instead of quietly shrinking the guarantee.
pub const SELF_TEST_CHECKS: &[&str] = &[
    "retirement table parsed",
    "one broken citation reported",
    "broken path named",
    "citations counted",
    "resolvable citations pass",
    "unreachable retirement commit rejected",
];

/// The TypeScript `selfTest()`, kept runnable from the binary's `--self-test`.
///
/// PORT NOTE: the TypeScript version throws on the first failed assertion and
/// has no way to notice an assertion that was deleted. This one counts.
pub fn self_test(root: &Path) -> Result<(), String> {
    let mut ran: Vec<&str> = Vec::new();

    let retired = retired_tools("| `tools/gone_fixture.ts` | `abc1234` | recover |");
    ran.push("retirement table parsed");
    if !retired.contains("tools/gone_fixture.ts") {
        return Err("retirement table not parsed".into());
    }
    let sources: Vec<(String, String)> = [
        ("a.c", "// derived by tools/live_fixture.ts"),
        ("b.c", "// derived by tools/gone_fixture.ts"),
        ("c.c", "// derived by tools/absent_fixture.ts"),
        ("d.c", "// derived by tools/absent_fixture.ts"),
    ]
    .iter()
    .map(|(name, text)| ((*name).to_string(), (*text).to_string()))
    .collect();
    let problems = broken_citations(&sources, &|path| path == "tools/live_fixture.ts", &retired);
    ran.push("one broken citation reported");
    if problems.len() != 1 {
        return Err(format!("expected one problem, got {}", problems.len()));
    }
    ran.push("broken path named");
    if !problems[0].contains("tools/absent_fixture.ts") {
        return Err("wrong path reported".into());
    }
    ran.push("citations counted");
    if !problems[0].contains("cited by 2") {
        return Err("citations must be counted".into());
    }
    ran.push("resolvable citations pass");
    if !broken_citations(&sources, &|_| true, &BTreeSet::new()).is_empty() {
        return Err("resolvable citations must pass".into());
    }
    ran.push("unreachable retirement commit rejected");
    if unrecoverable_retirements("| `tools/x.ts` | `0000000` | recover |", root).len() != 1 {
        return Err("a retirement row naming an unreachable commit must fail".into());
    }

    // Scanning nothing is not passing, and neither is asserting nothing.
    let missing: Vec<&&str> = SELF_TEST_CHECKS.iter().filter(|c| !ran.contains(c)).collect();
    if !missing.is_empty() {
        return Err(format!(
            "self-test executed {} of {} checks; missing {missing:?}",
            ran.len(),
            SELF_TEST_CHECKS.len()
        ));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn repo_root() -> std::path::PathBuf {
        // <crate>/tools/source-citations -> repository root.
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .ancestors()
            .nth(2)
            .expect("crate lives two levels below the repository root")
            .to_path_buf()
    }

    fn sources(texts: &[&str]) -> Vec<(String, String)> {
        texts
            .iter()
            .enumerate()
            .map(|(index, text)| (format!("s{index}.c"), (*text).to_string()))
            .collect()
    }

    #[test]
    fn ported_self_test_passes() {
        self_test(&repo_root()).expect("ported self-test");
    }

    #[test]
    fn citation_matches_nested_and_hyphenless_typescript_paths() {
        assert_eq!(
            citations("see tools/lib/overlay_published.ts, ok"),
            vec!["tools/lib/overlay_published.ts"]
        );
        // The character class excludes '.', so a dotted stem does not match.
        assert!(citations("tools/a.b.ts").is_empty());
        // Uppercase and hyphens are outside the TypeScript class.
        assert!(citations("tools/Foo.ts").is_empty());
        assert!(citations("tools/a-b.ts").is_empty());
    }

    #[test]
    fn citation_is_unanchored_and_greedy_like_the_original() {
        // The pattern has no word boundary, so an embedded path still matches.
        assert_eq!(citations("mytools/a.ts"), vec!["tools/a.ts"]);
        // A longer extension is truncated at .ts, as the JavaScript regex does.
        assert_eq!(citations("tools/a.tsx"), vec!["tools/a.ts"]);
        // Greedy: one match spans the inner "tools/" too.
        assert_eq!(citations("tools/x/tools/y.ts"), vec!["tools/x/tools/y.ts"]);
    }

    #[test]
    fn citation_pattern_is_not_widened_to_rust_paths() {
        // The de-widening PORT NOTE, asserted. `tools/a/b.rs` contains no
        // `tools/` substring, so nothing matches; `tools/a/b.ts` likewise,
        // because the hyphen is outside the body class.
        assert!(citations("tools/a/b.rs").is_empty());
        assert!(citations("tools/a/b.ts").is_empty());
    }

    #[test]
    fn the_real_corpus_is_neither_empty_nor_citation_free() {
        // The two ways this gate could pass without looking. Both are checked
        // by the binary; both are pinned here so a corpus move fails the tests
        // as well as the run.
        let root = repo_root();
        let mut scanned = 0usize;
        let mut cited = 0usize;
        let mut walk = vec![
            root.join("exact"),
            root.join("semantic"),
            root.join("asm"),
            root.join("include"),
            root.join("assets/code"),
        ];
        while let Some(directory) = walk.pop() {
            let Ok(entries) = std::fs::read_dir(&directory) else {
                continue;
            };
            for entry in entries.flatten() {
                let path = entry.path();
                if path.is_dir() {
                    walk.push(path);
                } else if matches!(
                    path.extension().and_then(|e| e.to_str()),
                    Some("c" | "h" | "s" | "inc")
                ) {
                    let text = String::from_utf8_lossy(&std::fs::read(&path).unwrap()).into_owned();
                    scanned += 1;
                    cited += citations(&text).len();
                }
            }
        }
        assert!(scanned > 0, "no sources were scanned, so the check proved nothing");
        assert!(cited > 0, "no citations matched, so a dead pattern would look clean");
    }

    #[test]
    fn crlf_and_bom_inputs_still_yield_citations() {
        // JavaScript's `.` excludes CR/LF, so a CRLF file can silently parse to
        // nothing in a line-oriented scanner. This one is byte-oriented; pin it.
        assert_eq!(
            citations("\u{feff}// a\r\n// derived by tools/x/y.ts\r\n"),
            vec!["tools/x/y.ts"]
        );
        assert_eq!(
            retired_tools("\r\n| `tools/x.ts` | `abc1234` |\r\n").len(),
            1
        );
    }

    #[test]
    fn self_test_reports_every_check_it_ran() {
        // A self-test that stops asserting must not keep printing "ok".
        assert_eq!(SELF_TEST_CHECKS.len(), 6);
        self_test(&repo_root()).expect("all checks execute");
    }

    #[test]
    fn broken_citations_counts_repeats_within_one_source() {
        // The TypeScript self-test only ever counted across files.
        let sources = sources(&["tools/a.ts tools/a.ts tools/a.ts"]);
        let problems = broken_citations(&sources, &|_| false, &BTreeSet::new());
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("cited by 3 source(s)"));
    }

    #[test]
    fn broken_citations_orders_by_descending_count() {
        let sources = sources(&["tools/rare.ts", "tools/common.ts", "tools/common.ts"]);
        let problems = broken_citations(&sources, &|_| false, &BTreeSet::new());
        assert_eq!(problems.len(), 2);
        assert!(problems[0].starts_with("tools/common.ts"));
        assert!(problems[1].starts_with("tools/rare.ts"));
    }

    #[test]
    fn retirement_only_silences_the_path_it_names() {
        let retired = retired_tools("| `tools/gone.ts` | `abc1234` | recover |");
        let sources = sources(&["tools/gone.ts tools/other.ts"]);
        let problems = broken_citations(&sources, &|_| false, &retired);
        assert_eq!(problems.len(), 1);
        assert!(problems[0].starts_with("tools/other.ts"));
    }

    #[test]
    fn retirement_rows_reject_malformed_shas() {
        // Six hex digits is under the {7,40} floor.
        assert!(retirement_rows("| `tools/x.ts` | `abc123` |").is_empty());
        // Forty-one is over the ceiling, and cannot backtrack to a shorter one.
        let long = "a".repeat(41);
        assert!(retirement_rows(&format!("| `tools/x.ts` | `{long}` |")).is_empty());
        // Non-hex characters end the run before the closing backtick.
        assert!(retirement_rows("| `tools/x.ts` | `abc123z` |").is_empty());
        assert_eq!(
            retirement_rows("| `tools/x.ts` | `abc1234` | recover |"),
            vec![("tools/x.ts".to_string(), "abc1234".to_string())]
        );
    }

    #[test]
    fn retirement_table_needs_backticks_and_pipes() {
        assert!(retired_tools("tools/x.ts").is_empty());
        assert!(retired_tools("| tools/x.ts |").is_empty());
        assert!(retired_tools("| `tools/x.ts`").is_empty());
        // Prose mentioning a path in backticks mid-sentence is not a row.
        assert!(retired_tools("see `tools/x.ts` for details").is_empty());
    }

    fn fixture_root(name: &str, files: &[(&str, &[u8])]) -> std::path::PathBuf {
        // Namespaced scratchpad only: never the repository, never /tmp root.
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("fixtures")
            .join(name);
        let _ = std::fs::remove_dir_all(&root);
        for (relative, bytes) in files {
            let path = root.join(relative);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(&path, bytes).unwrap();
        }
        std::fs::create_dir_all(&root).unwrap();
        root
    }

    #[test]
    fn an_empty_corpus_is_a_failure_not_a_pass() {
        let root = fixture_root("empty", &[("PROVENANCE.md", b"")]);
        let problems = check(&root).expect_err("scanning nothing must not pass");
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("scanning nothing is not passing"), "{problems:?}");
    }

    #[test]
    fn a_corpus_with_no_citations_at_all_is_a_failure() {
        // Stands in for the citation pattern silently ceasing to match: sources
        // are present and scanned, and every one of them is citation-free.
        let root = fixture_root(
            "uncited",
            &[
                ("PROVENANCE.md", b"" as &[u8]),
                ("exact/a.c", b"int a;\n"),
                ("semantic/b.h", b"int b;\n"),
            ],
        );
        let problems = check(&root).expect_err("no citations must not pass");
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("2 sources scanned but not one citation"), "{problems:?}");
    }

    #[test]
    fn a_citation_free_corpus_would_otherwise_have_looked_clean() {
        // The negative control for the guard above: without the floor, this
        // exact tree produces zero problems, which is why the floor exists.
        let root = fixture_root("uncited_control", &[("exact/a.c", b"int a;\n")]);
        let sources = collect_sources(&root);
        assert_eq!(sources.len(), 1);
        assert!(broken_citations(&sources, &|_| false, &BTreeSet::new()).is_empty());
    }

    #[test]
    fn a_corpus_with_citations_passes_and_counts_them() {
        let root = fixture_root(
            "cited",
            &[
                ("PROVENANCE.md", b"| `tools/gone.ts` | `abc1234` |" as &[u8]),
                ("exact/a.c", b"// derived by tools/check/source_citations.ts\n"),
                // Nested, and a non-source extension that must be ignored.
                ("asm/deep/b.inc", b".byte 0 // tools/check/source_citations.ts\n"),
                ("exact/notes.md", b"tools/absent.ts\n"),
            ],
        );
        let sources = collect_sources(&root);
        assert_eq!(sources.len(), 2, "only .c/.h/.s/.inc are sources");
        // The fixture root has no tools/ tree, so existence is judged against
        // the real repository to keep this about the walk, not the filesystem.
        let real = repo_root();
        let retired = retired_tools("| `tools/gone.ts` | `abc1234` |");
        assert!(broken_citations(&sources, &|p| real.join(p).exists(), &retired).is_empty());
    }

    #[test]
    fn a_missing_provenance_file_is_a_failure() {
        let root = fixture_root("no_provenance", &[("exact/a.c", b"tools/x.ts")]);
        let problems = check(&root).expect_err("a missing PROVENANCE.md must fail");
        assert!(problems[0].starts_with("cannot read PROVENANCE.md"), "{problems:?}");
    }

    #[test]
    fn the_real_provenance_table_parses_and_recovers() {
        let root = repo_root();
        let provenance = std::fs::read_to_string(root.join("PROVENANCE.md")).unwrap();
        let retired = retired_tools(&provenance);
        let rows = retirement_rows(&provenance);
        assert!(!retired.is_empty(), "PROVENANCE.md records no retirements");
        assert_eq!(retired.len(), rows.len(), "every retirement row must name a commit");
        assert!(unrecoverable_retirements(&provenance, &root).is_empty());
    }
}
