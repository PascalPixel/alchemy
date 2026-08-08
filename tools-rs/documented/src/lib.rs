// Every tool must be documented in AGENTS.md, or it must not exist.
//
// This repository reached 184 tools because adding one cost nothing and nobody
// could see the total. The rule that stops it recurring is not a budget, it is
// this gate: a tool with no section in AGENTS.md fails the build, and the only
// two ways to pass are to write the section or delete the tool.
//
// A "tool" is an entry point: tools/<name>/index.ts, or a top-level tools/*.ts.
// Files under tools/lib/ and the modules a tool dispatches to are implementation
// and are deliberately not listed -- pokeemerald documents 11 binaries, not the
// 82 source files behind them.
//
// Port of tools/check/documented.ts.
//
// PORT NOTE (scope): the scan still covers `tools/` only, and deliberately does
// not enumerate `tools-rs/*` crates. Those crates are ports of the *modules* a
// tool dispatches to (no_asm_c, cache_key_lint, source_citations, jobs, ...),
// not of entry points; the TypeScript gate never required those modules to have
// a section either, and widening the scan would change the rule from "every
// entry point is documented" to "every source file is documented", which is the
// listing habit the comment above rejects. It would also fail on the real tree
// today: no `tools-rs` crate has a heading in AGENTS.md, and crate names such as
// `alignment-tail` cannot even be expressed as a heading, since the heading
// pattern only accepts [a-z0-9_]. When a Rust *entry point* replaces a
// `tools/<name>` entry point rather than a module behind one, that name is
// already required here via the tools/ listing, and the gap to close then is the
// directory scan, not the documentation rule. Reported rather than loosened.

use std::collections::HashSet;
use std::path::Path;

/// The single definition of "a tool", shared rather than restated. It was stated
/// twice once: architecture.ts learned to skip folders without an index.ts and
/// this did not, so an untracked tools/gcc296/ build directory failed the gate in
/// one worktree and passed in another.
pub fn entry_points(
    names: &[String],
    is_directory: &dyn Fn(&str) -> bool,
    has_index: &dyn Fn(&str) -> bool,
) -> Vec<String> {
    let mut tools: Vec<String> = Vec::new();
    for name in names {
        if is_directory(name) {
            // A folder is a tool only if it has an index.ts. Anything else under
            // tools/ is untracked output and is not ours to document.
            // `lib` is shared implementation; `scratch` is deliberately exempt.
            // The best find of the restructure session came from a throwaway
            // script sweeping toplev.c against the board -- it closed
            // resource_39e:26d8 and showed 31 of the fork's 74 -fthumb-* modes
            // were absent from the sweep list. A gate that fails that script is a
            // gate against the work that pays. Scratch is untracked and never
            // runs in a gate; graduate anything that survives into a real tool
            // with a section on the page.
            if name != "lib" && name != "scratch" && has_index(name) {
                tools.push(name.clone());
            }
        } else if let Some(stem) = name.strip_suffix(".ts") {
            tools.push(stem.to_string());
        }
    }
    tools.sort();
    tools
}

/// PORT NOTE: JavaScript's `Map` is insertion-ordered and `violations` iterates
/// it, so the port cannot use a `HashMap` without reordering the reported
/// problems. This is that `Map`: insertion-ordered, last write wins the value and
/// the first insertion keeps the position.
#[derive(Debug, Default, Clone)]
pub struct Docs {
    entries: Vec<(String, usize)>,
}

impl Docs {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn set(&mut self, name: &str, words: usize) {
        for entry in self.entries.iter_mut() {
            if entry.0 == name {
                entry.1 = words;
                return;
            }
        }
        self.entries.push((name.to_string(), words));
    }

    pub fn get(&self, name: &str) -> Option<usize> {
        self.entries
            .iter()
            .find(|entry| entry.0 == name)
            .map(|entry| entry.1)
    }

    pub fn iter(&self) -> impl Iterator<Item = (&str, usize)> {
        self.entries.iter().map(|entry| (entry.0.as_str(), entry.1))
    }

    pub fn len(&self) -> usize {
        self.entries.len()
    }

    pub fn is_empty(&self) -> bool {
        self.entries.is_empty()
    }
}

fn is_word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// `/^#{2,3}\s/`: two or three hashes, then whitespace. Four hashes never match,
/// because the quantifier is capped and the pattern is anchored.
fn is_heading_line(line: &str) -> bool {
    let bytes = line.as_bytes();
    let hashes = bytes.iter().take_while(|byte| **byte == b'#').count();
    (2..=3).contains(&hashes)
        && line[hashes..]
            .chars()
            .next()
            .is_some_and(|character| character.is_whitespace())
}

/// `/^#{2,3}\s+`?([a-z0-9_]+)`?\b/`, hand-rolled.
///
/// The trailing `\b` is the subtle part. After a greedy identifier the next byte
/// is never in `[a-z0-9_]`, but it can still be a word byte (an uppercase letter
/// or a digit-free `A-Z`), and then the boundary fails and no shorter identifier
/// can rescue it. A closing backtick always leaves a boundary available, either
/// before it or after it, so it always matches.
fn heading_name(line: &str) -> Option<&str> {
    let bytes = line.as_bytes();
    let hashes = bytes.iter().take_while(|byte| **byte == b'#').count();
    if !(2..=3).contains(&hashes) {
        return None;
    }
    let mut at = hashes;
    let spaces = line[at..]
        .chars()
        .take_while(|character| character.is_whitespace())
        .map(char::len_utf8)
        .sum::<usize>();
    if spaces == 0 {
        return None;
    }
    at += spaces;
    if bytes.get(at) == Some(&b'`') {
        at += 1;
    }
    let start = at;
    while at < bytes.len() && (bytes[at].is_ascii_lowercase() || bytes[at].is_ascii_digit() || bytes[at] == b'_')
    {
        at += 1;
    }
    if at == start {
        return None;
    }
    match bytes.get(at) {
        None => Some(&line[start..at]),
        Some(b'`') => Some(&line[start..at]),
        Some(byte) if !is_word_byte(*byte) => Some(&line[start..at]),
        Some(_) => None,
    }
}

/// A heading names the tool, so `## overlay` and `## overlay -- ...` both count.
/// Requiring a paragraph as well is the point: a bare heading is a listing, not
/// documentation, and the rule is that a reader learns what the tool is for.
pub fn documented(markdown: &str) -> Docs {
    let mut found = Docs::new();
    let lines: Vec<&str> = markdown.split('\n').collect();
    for at in 0..lines.len() {
        let Some(name) = heading_name(lines[at]) else {
            continue;
        };
        let mut words = 0usize;
        for line in lines.iter().skip(at + 1) {
            if is_heading_line(line) {
                break;
            }
            words += line.split_whitespace().count();
        }
        found.set(name, words);
    }
    found
}

pub const MINIMUM_WORDS: usize = 20;

/// PORT NOTE: the TypeScript closed over a module-level `ROOT`; the port takes it
/// as an argument so the check is testable without touching the real tree. Same
/// filesystem probe, same messages.
pub fn violations(tools: &[String], docs: &Docs, root: &Path) -> Vec<String> {
    let mut problems: Vec<String> = Vec::new();
    for tool in tools {
        match docs.get(tool) {
            None => problems.push(format!(
                "{tool}: no section in AGENTS.md -- document it or delete the tool"
            )),
            Some(words) if words < MINIMUM_WORDS => problems.push(format!(
                "{tool}: only {words} words; a heading is a listing, not documentation"
            )),
            Some(_) => {}
        }
    }
    let known: HashSet<&str> = tools.iter().map(String::as_str).collect();
    for (name, _) in docs.iter() {
        if name.chars().count() > 2
            && !known.contains(name)
            && root.join("tools").join(format!("{name}.ts")).exists()
        {
            problems.push(format!("{name}: documented but is not an entry point"));
        }
    }
    problems
}

#[cfg(test)]
mod tests {
    use super::*;

    fn names(list: &[&str]) -> Vec<String> {
        list.iter().map(|name| name.to_string()).collect()
    }

    // The TypeScript self-test, ported whole.
    #[test]
    fn entry_points_skips_lib_and_indexless_folders() {
        let given = names(&["overlay", "lib", "gcc296", "verify.ts", "notes.md"]);
        let tools = entry_points(
            &given,
            &|name: &str| !name.contains('.'),
            &|name: &str| name == "overlay",
        );
        assert_eq!(tools, names(&["overlay", "verify"]));
    }

    #[test]
    fn a_real_paragraph_counts() {
        let docs = documented(&format!(
            "## overlay\n\n{}\n\n## verify\n\ntoo short\n",
            "word ".repeat(30)
        ));
        assert_eq!(docs.get("overlay"), Some(30));
        assert!(docs.get("overlay").unwrap() >= MINIMUM_WORDS);
        assert_eq!(docs.get("verify"), Some(2));
    }

    #[test]
    fn undocumented_and_stub_fail_while_documented_passes() {
        let docs = documented(&format!(
            "## overlay\n\n{}\n\n## verify\n\ntoo short\n",
            "word ".repeat(30)
        ));
        let problems = violations(
            &names(&["overlay", "verify", "ghost"]),
            &docs,
            Path::new("/nonexistent-root"),
        );
        assert!(problems.iter().any(|p| p.starts_with("ghost:")));
        assert!(problems.iter().any(|p| p.starts_with("verify:")));
        assert!(!problems.iter().any(|p| p.starts_with("overlay:")));
        assert_eq!(
            problems,
            vec![
                "verify: only 2 words; a heading is a listing, not documentation".to_string(),
                "ghost: no section in AGENTS.md -- document it or delete the tool".to_string(),
            ]
        );
    }

    #[test]
    fn a_heading_with_a_dash_suffix_still_matches() {
        let docs = documented(&format!(
            "## overlay --- the code overlays\n\n{}",
            "word ".repeat(30)
        ));
        assert_eq!(docs.get("overlay"), Some(30));
    }

    // Edge cases the TypeScript self-test never covered.

    #[test]
    fn backticked_headings_match_and_strip_the_ticks() {
        let docs = documented("## `overlay`\n\nsome words here\n");
        assert_eq!(docs.get("overlay"), Some(3));
        let open_only = documented("## `overlay\n\nsome words here\n");
        assert_eq!(open_only.get("overlay"), Some(3));
    }

    #[test]
    fn a_word_character_directly_after_the_name_defeats_the_boundary() {
        // `[a-z0-9_]+` is greedy, so `overlayX` leaves `X` -- still a word
        // character, so `\b` fails and no shorter prefix can rescue it.
        assert!(documented("## overlayX\n\nwords words words\n").is_empty());
    }

    #[test]
    fn four_hashes_are_not_a_heading_and_do_not_stop_the_count() {
        let docs = documented("## overlay\n\none two\n\n#### deeper\n\nthree four\n");
        assert_eq!(docs.get("deeper"), None);
        assert_eq!(docs.get("overlay"), Some(6)); // "#### deeper" counts as prose
        assert_eq!(docs.len(), 1);
    }

    #[test]
    fn a_single_hash_is_not_a_heading() {
        let docs = documented("# overlay\n\none two three\n");
        assert!(docs.is_empty());
    }

    #[test]
    fn hashes_without_whitespace_are_not_a_heading() {
        assert!(documented("##overlay\n\none two three\n").is_empty());
    }

    #[test]
    fn uppercase_and_empty_names_do_not_match() {
        assert!(documented("## Ledgers\n\none two three\n").is_empty());
        assert!(documented("## -- nothing\n\none two three\n").is_empty());
        assert!(documented("## ``\n\none two three\n").is_empty());
    }

    #[test]
    fn the_next_heading_stops_the_word_count() {
        let docs = documented("## a1\n\none two\n\n## b2\n\nthree four five\n");
        assert_eq!(docs.get("a1"), Some(2));
        assert_eq!(docs.get("b2"), Some(3));
    }

    #[test]
    fn a_duplicate_heading_keeps_its_position_and_takes_the_last_count() {
        let docs = documented("## a1\n\none two\n\n## b2\n\nx\n\n## a1\n\none two three\n");
        let order: Vec<&str> = docs.iter().map(|(name, _)| name).collect();
        assert_eq!(order, vec!["a1", "b2"]);
        assert_eq!(docs.get("a1"), Some(3));
    }

    #[test]
    fn tabs_count_as_heading_whitespace() {
        let docs = documented("##\toverlay\n\none two three\n");
        assert_eq!(docs.get("overlay"), Some(3));
    }

    #[test]
    fn exactly_the_minimum_passes_and_one_short_fails() {
        let at = documented(&format!("## overlay\n\n{}", "word ".repeat(MINIMUM_WORDS)));
        assert!(violations(&names(&["overlay"]), &at, Path::new("/nope")).is_empty());
        let under = documented(&format!("## overlay\n\n{}", "word ".repeat(MINIMUM_WORDS - 1)));
        let problems = violations(&names(&["overlay"]), &under, Path::new("/nope"));
        assert_eq!(
            problems,
            vec!["overlay: only 19 words; a heading is a listing, not documentation".to_string()]
        );
    }

    #[test]
    fn a_documented_non_entry_point_is_reported_only_when_the_file_exists() {
        let root = std::env::temp_dir().join(format!(
            "documented-port-{}-{}",
            std::process::id(),
            "orphan"
        ));
        let tools = root.join("tools");
        std::fs::create_dir_all(&tools).unwrap();
        std::fs::write(tools.join("orphan.ts"), "// stale\n").unwrap();

        let docs = documented(&format!(
            "## orphan\n\n{}\n\n## ab\n\n{}\n",
            "word ".repeat(30),
            "word ".repeat(30)
        ));
        let problems = violations(&[], &docs, &root);
        assert_eq!(
            problems,
            vec!["orphan: documented but is not an entry point".to_string()]
        );

        // Short names are exempt, even if tools/ab.ts existed.
        std::fs::write(tools.join("ab.ts"), "// short\n").unwrap();
        assert_eq!(violations(&[], &docs, &root).len(), 1);

        // ...and a name that IS an entry point is not reported.
        assert!(violations(&names(&["orphan"]), &docs, &root).is_empty());

        std::fs::remove_dir_all(&root).unwrap();
    }

    #[test]
    fn top_level_ts_files_become_tools_and_other_files_do_not() {
        let given = names(&["self_test.ts", "README.md", "notes", ".gitignore"]);
        let tools = entry_points(&given, &|name: &str| !name.contains('.'), &|_| true);
        assert_eq!(tools, names(&["notes", "self_test"]));
    }
}
