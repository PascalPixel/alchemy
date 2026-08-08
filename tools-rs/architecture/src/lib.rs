// Port of tools/check/architecture.ts -- enforce the shape of tools/.
//
// Three invariants, unchanged from the TypeScript original:
//   1. Every tool is documented in AGENTS.md   (delegated to documented.ts)
//   2. A tool never imports from another tool  (shared code belongs in lib/)
//   3. Every module is reachable from a tool, a script, a hook, or a document
//
// PORT NOTE -- dependencies. architecture.ts imports exactly two local symbols
// and this crate takes both from the ports of the files they live in, rather
// than restating them:
//   * `entryPoints` from tools/check/documented.ts   -> `documented::entry_points`
//   * `retiredTools` from tools/check/source_citations.ts
//                                              -> `source_citations::retired_tools`
// Both were inlined here at first, and both had already drifted: the inlined
// `retired_tools` used JS `\s` while the source-citations port uses ASCII
// whitespace, and the inlined `entry_points` sorted by UTF-16 code unit while
// documented's sorts by byte. Two hand-synced copies of a gate's rule is exactly
// the drift class these gates exist to catch, and architecture.ts's own comment
// says so ("Counted through documented.ts's definition, not a second one here.
// The two disagreed -- 9 against 10"). See the two PORT NOTEs on the re-exports
// below for what each surviving divergence can and cannot change.
//   No other local module is used by architecture.ts.
//
// PORT NOTE -- regexes are hand-rolled (no regex crate). Each one below names
// the JS pattern it replaces and matches JS semantics exactly, including
// non-overlapping `matchAll` advancement (a consumed trailing `|` is NOT
// available to the next match; `retired_tools` tests pin this).
//
// PORT NOTE -- `\s` follows JS, not Rust, wherever this crate scans: JS `\s`
// includes U+00A0, U+1680, U+2000-U+200A, U+2028, U+2029, U+202F, U+205F,
// U+3000, U+FEFF, and excludes nothing Rust's `char::is_whitespace` includes
// except U+FEFF (which Rust does not treat as whitespace). `js_is_space` pins
// this; it is used for the stderr trim, which is `String.prototype.trim()`.
//
// PORT NOTE -- files are read with `String::from_utf8_lossy`, matching Node's
// `readFileSync(path, "utf8")`, which replaces invalid bytes with U+FFFD
// rather than failing.
//
// PORT NOTE (deliberate behaviour change, gate hardening) -- the TypeScript
// gate passes when it scans nothing: an empty tools/ tree or a repository with
// no markdown would print "architecture ok: 0 tools, 0 lib modules". The Rust
// binary treats "scanned nothing" as a hard failure (see `main.rs`). On the
// real tree both agree, so observable output is unchanged.

use std::collections::{BTreeSet, HashMap, HashSet};

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Module {
    pub tool: String,
    pub name: String,
    pub source: String,
}

pub const LIBRARY: &str = "lib";

// ---------------------------------------------------------------------------
// JS primitives
// ---------------------------------------------------------------------------

/// JS `\s` in a regular expression.
pub fn js_is_space(c: char) -> bool {
    matches!(
        c,
        ' ' | '\t'
            | '\n'
            | '\u{b}'
            | '\u{c}'
            | '\r'
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

// ---------------------------------------------------------------------------
// tiny hand-rolled scanning helpers
// ---------------------------------------------------------------------------

fn is_ident(c: u8) -> bool {
    c.is_ascii_lowercase() || c.is_ascii_digit() || c == b'_'
}

/// `[a-z0-9_]+` starting at `at`; returns the end index, or None if empty.
fn ident_end(bytes: &[u8], at: usize) -> Option<usize> {
    let mut end = at;
    while end < bytes.len() && is_ident(bytes[end]) {
        end += 1;
    }
    if end == at {
        None
    } else {
        Some(end)
    }
}

fn starts_with_at(bytes: &[u8], at: usize, needle: &[u8]) -> bool {
    bytes.len() >= at + needle.len() && &bytes[at..at + needle.len()] == needle
}

// ---------------------------------------------------------------------------
// the two symbols architecture.ts imports, taken from the ports of the files
// they live in rather than restated here
// ---------------------------------------------------------------------------

/// `entryPoints` from tools/check/documented.ts.
///
/// PORT NOTE (accepted divergence, count-only): the TypeScript ends with a bare
/// `.sort()`, which orders by UTF-16 code unit; `documented::entry_points` sorts
/// by UTF-8 byte. The two disagree only for astral characters in a tool name,
/// which cannot occur here -- and architecture.ts uses this for `tools.length`
/// alone, so order is not observable through this crate at all. Owning one
/// definition is worth more than a private copy that sorts differently.
pub use documented::entry_points;

/// `retiredTools` from tools/check/source_citations.ts.
///
/// PORT NOTE (accepted divergence, and the reason it is worth naming): the
/// TypeScript pattern is `/\|\s*`(tools\/[a-z0-9_\/]+\.ts)`\s*\|/g` with JS
/// `\s`; the source-citations port narrows `\s` to ASCII whitespace, a
/// divergence documented in that crate. A retirement row separated from its
/// pipe by U+00A0 or U+FEFF would therefore be retired by bun and not by this
/// binary. PROVENANCE.md today contains only U+0020 and U+000A whitespace
/// (verified: its only non-ASCII characters are U+2014, U+201C, U+201D), so the
/// two agree byte for byte on the real tree; `retired_tools_agrees_on_the_real_
/// provenance` pins that, and `retired_tools_uses_ascii_whitespace_only` pins
/// the divergence so it cannot change unnoticed. Reported, not silently fixed:
/// the fix belongs in source-citations, where the one definition lives.
///
/// The three holes in the retirement mechanism (a row with no sha silences a
/// citation with no recoverability check; a sha below the 7-hex floor does the
/// same; the pattern is not scoped to the retirement table, so any markdown
/// table row anywhere in PROVENANCE.md grants retirement) are reproduced, not
/// fixed, in source-citations. Taking the dependency keeps them reproduced here
/// identically instead of forking a second set of holes.
pub use source_citations::retired_tools;

// ---------------------------------------------------------------------------
// architecture.ts proper
// ---------------------------------------------------------------------------

pub fn modules(
    read: &dyn Fn(&str) -> String,
    list: &dyn Fn(&str) -> Vec<String>,
) -> Vec<Module> {
    let mut found: Vec<Module> = Vec::new();
    for entry in list("") {
        if entry.ends_with(".ts") {
            found.push(Module {
                tool: String::new(),
                name: entry[..entry.len() - 3].to_string(),
                source: read(&entry),
            });
            continue;
        }
        if entry.contains('.') || entry == "scratch" {
            continue;
        }
        let files = list(&entry);
        if !files.iter().any(|f| f == "index.ts") && entry != LIBRARY {
            continue;
        }
        for file in &files {
            if file.ends_with(".ts") {
                found.push(Module {
                    tool: entry.clone(),
                    name: file[..file.len() - 3].to_string(),
                    source: read(&format!("{entry}/{file}")),
                });
            }
        }
    }
    found
}

/// JS: `/from "\.\.\/([a-z0-9_]+)\/([a-z0-9_]+)\.ts"/g`
fn cross_tool_matches(source: &str) -> Vec<(String, String)> {
    let bytes = source.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        if !starts_with_at(bytes, at, b"from \"../") {
            at += 1;
            continue;
        }
        let mut scan = at + b"from \"../".len();
        let Some(tool_end) = ident_end(bytes, scan) else {
            at += 1;
            continue;
        };
        let tool = source[scan..tool_end].to_string();
        scan = tool_end;
        if scan >= bytes.len() || bytes[scan] != b'/' {
            at += 1;
            continue;
        }
        scan += 1;
        let Some(name_end) = ident_end(bytes, scan) else {
            at += 1;
            continue;
        };
        let name = source[scan..name_end].to_string();
        scan = name_end;
        if !starts_with_at(bytes, scan, b".ts\"") {
            at += 1;
            continue;
        }
        out.push((tool, name));
        at = scan + 4;
    }
    out
}

pub fn cross_tool_imports(found: &[Module]) -> Vec<String> {
    let mut problems = Vec::new();
    for module in found {
        if module.tool == "check" && module.name == "architecture" {
            continue;
        }
        for (tool, name) in cross_tool_matches(&module.source) {
            if tool == LIBRARY || tool == module.tool {
                continue;
            }
            problems.push(format!(
                "tools/{}/{}.ts imports {}/{} -- a tool may import lib/, never another tool; move the shared part to lib/",
                module.tool, module.name, tool, name
            ));
        }
    }
    problems
}

/// JS: `/(?:from |import\()"(\.{1,2})\/(?:([a-z0-9_]+)\/)?([a-z0-9_]+)\.ts"/g`
/// Returns (dots, folder, name).
fn relative_import_matches(source: &str) -> Vec<(String, Option<String>, String)> {
    let bytes = source.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        let head = if starts_with_at(bytes, at, b"from \"") {
            b"from \"".len()
        } else if starts_with_at(bytes, at, b"import(\"") {
            b"import(\"".len()
        } else {
            at += 1;
            continue;
        };
        let mut scan = at + head;
        // `\.{1,2}` is greedy but backtracks; the only continuation is `/`,
        // so two dots followed by `/` wins, otherwise try one dot.
        let dots = if starts_with_at(bytes, scan, b"../") {
            scan += 2;
            ".."
        } else if starts_with_at(bytes, scan, b"./") {
            scan += 1;
            "."
        } else {
            at += 1;
            continue;
        };
        scan += 1; // the `/`
        // Greedy optional `([a-z0-9_]+)/`, with backtracking to "absent".
        if let Some(end) = ident_end(bytes, scan) {
            if end < bytes.len() && bytes[end] == b'/' {
                if let Some(name_end) = ident_end(bytes, end + 1) {
                    if starts_with_at(bytes, name_end, b".ts\"") {
                        out.push((
                            dots.to_string(),
                            Some(source[scan..end].to_string()),
                            source[end + 1..name_end].to_string(),
                        ));
                        at = name_end + 4;
                        continue;
                    }
                }
            }
        }
        let Some(name_end) = ident_end(bytes, scan) else {
            at += 1;
            continue;
        };
        if !starts_with_at(bytes, name_end, b".ts\"") {
            at += 1;
            continue;
        }
        out.push((dots.to_string(), None, source[scan..name_end].to_string()));
        at = name_end + 4;
    }
    out
}

pub fn broken_imports(found: &[Module]) -> Vec<String> {
    let present: HashSet<String> = found
        .iter()
        .map(|m| format!("{}/{}", m.tool, m.name))
        .collect();
    let mut problems = Vec::new();
    for module in found {
        if module.tool == "check" && module.name == "architecture" {
            continue;
        }
        for (dots, folder, name) in relative_import_matches(&module.source) {
            let target = match folder {
                Some(folder) => format!("{folder}/{name}"),
                None => {
                    if dots == "." {
                        format!("{}/{}", module.tool, name)
                    } else {
                        format!("/{name}")
                    }
                }
            };
            if !present.contains(&target) {
                // JS: `${target || name}` -- an empty target falls back to the
                // bare name. `target` is never empty here (it always holds at
                // least a `/`), but the fallback is kept for fidelity.
                let shown = if target.is_empty() { &name } else { &target };
                problems.push(format!(
                    "tools/{}/{}.ts imports {} which does not exist",
                    module.tool, module.name, shown
                ));
            }
        }
    }
    problems
}

/// JS: `/from "\.{1,2}\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts"/g`
fn reach_import_names(source: &str) -> Vec<String> {
    let bytes = source.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        if !starts_with_at(bytes, at, b"from \"") {
            at += 1;
            continue;
        }
        let mut scan = at + b"from \"".len();
        if starts_with_at(bytes, scan, b"../") {
            scan += 3;
        } else if starts_with_at(bytes, scan, b"./") {
            scan += 2;
        } else {
            at += 1;
            continue;
        }
        if let Some(end) = ident_end(bytes, scan) {
            if end < bytes.len() && bytes[end] == b'/' {
                if let Some(name_end) = ident_end(bytes, end + 1) {
                    if starts_with_at(bytes, name_end, b".ts\"") {
                        out.push(source[end + 1..name_end].to_string());
                        at = name_end + 4;
                        continue;
                    }
                }
            }
            if starts_with_at(bytes, end, b".ts\"") {
                out.push(source[scan..end].to_string());
                at = end + 4;
                continue;
            }
        }
        at += 1;
    }
    out
}

/// JS: `/tools\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts/g` (unanchored: `xtools/a.ts`
/// matches too, exactly as in JS -- there is no `\b` in the pattern).
fn tools_path_names(source: &str) -> Vec<String> {
    let bytes = source.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        if !starts_with_at(bytes, at, b"tools/") {
            at += 1;
            continue;
        }
        let scan = at + b"tools/".len();
        if let Some(end) = ident_end(bytes, scan) {
            if end < bytes.len() && bytes[end] == b'/' {
                if let Some(name_end) = ident_end(bytes, end + 1) {
                    if starts_with_at(bytes, name_end, b".ts") {
                        out.push(source[end + 1..name_end].to_string());
                        at = name_end + 3;
                        continue;
                    }
                }
            }
            if starts_with_at(bytes, end, b".ts") {
                out.push(source[scan..end].to_string());
                at = end + 3;
                continue;
            }
        }
        at += 1;
    }
    out
}

/// seedNames()'s scan of package.json, .hooks/*, and *.md for `tools/<name>.ts`
/// -- the same `/tools\/(?:[a-z0-9_]+\/)?([a-z0-9_]+)\.ts/g` used for reach.
pub fn seed_targets(text: &str) -> Vec<String> {
    tools_path_names(text)
}

pub fn unreachable(found: &[Module], seeds: &BTreeSet<String>) -> Vec<String> {
    // JS builds `new Map(found.map(...))`: a later module with a duplicate name
    // overwrites an earlier one. HashMap::insert has the same last-wins rule.
    let mut by_name: HashMap<&str, &Module> = HashMap::new();
    for module in found {
        by_name.insert(module.name.as_str(), module);
    }
    let mut seen: HashSet<String> = HashSet::new();
    // JS seeds the stack with `[...seeds]` (insertion order) and pops from the
    // end. Traversal order cannot change the result -- `seen` is a set and the
    // report is built from `found` -- so a sorted seed set is equivalent.
    let mut stack: Vec<String> = seeds.iter().cloned().collect();
    while let Some(name) = stack.pop() {
        if seen.contains(&name) {
            continue;
        }
        seen.insert(name.clone());
        let Some(module) = by_name.get(name.as_str()) else {
            continue;
        };
        for target in reach_import_names(&module.source) {
            if !seen.contains(&target) {
                stack.push(target);
            }
        }
        for target in tools_path_names(&module.source) {
            if !seen.contains(&target) {
                stack.push(target);
            }
        }
    }
    found
        .iter()
        .filter(|m| !seen.contains(&m.name))
        .map(|m| {
            format!(
                "tools/{}/{}.ts is reachable from nothing -- wire it up or delete it",
                m.tool, m.name
            )
        })
        .collect()
}

/// JS: `/bun tools\/([a-z0-9_]+)\/index\.ts ([a-z0-9_]+)/g`
fn doc_command_matches(text: &str) -> Vec<(String, String)> {
    let bytes = text.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        if !starts_with_at(bytes, at, b"bun tools/") {
            at += 1;
            continue;
        }
        let mut scan = at + b"bun tools/".len();
        let Some(tool_end) = ident_end(bytes, scan) else {
            at += 1;
            continue;
        };
        let tool = text[scan..tool_end].to_string();
        scan = tool_end;
        if !starts_with_at(bytes, scan, b"/index.ts ") {
            at += 1;
            continue;
        }
        scan += b"/index.ts ".len();
        let Some(sub_end) = ident_end(bytes, scan) else {
            at += 1;
            continue;
        };
        out.push((tool, text[scan..sub_end].to_string()));
        at = sub_end;
    }
    out
}

pub fn broken_doc_commands(
    docs: &[(String, String)],
    exists: &dyn Fn(&str) -> bool,
) -> Vec<String> {
    let mut problems = Vec::new();
    for (name, text) in docs {
        for (tool, sub) in doc_command_matches(text) {
            if !exists(&format!("tools/{tool}/{sub}.ts")) {
                problems.push(format!(
                    "{name}: \"bun tools/{tool}/index.ts {sub}\" names a subcommand that does not exist"
                ));
            }
        }
    }
    problems
}

/// JS: `/tools\/[a-z0-9_\/]+\.ts/g`
fn doc_paths(line: &str) -> Vec<String> {
    let bytes = line.as_bytes();
    let mut out = Vec::new();
    let mut at = 0usize;
    while at < bytes.len() {
        if !starts_with_at(bytes, at, b"tools/") {
            at += 1;
            continue;
        }
        let scan = at + b"tools/".len();
        let mut end = scan;
        while end < bytes.len() && (is_ident(bytes[end]) || bytes[end] == b'/') {
            end += 1;
        }
        // The class excludes `.`, so backtracking can never expose a `.ts`
        // that maximal munch missed.
        if end == scan || !starts_with_at(bytes, end, b".ts") {
            at += 1;
            continue;
        }
        out.push(line[at..end + 3].to_string());
        at = end + 3;
    }
    out
}

pub fn broken_doc_paths(
    docs: &[(String, String)],
    exists: &dyn Fn(&str) -> bool,
    retired: &BTreeSet<String>,
) -> Vec<String> {
    let mut problems = Vec::new();
    for (name, text) in docs {
        for line in text.split('\n') {
            let paths = doc_paths(line);
            let mapping = paths.len() > 1 && paths.iter().any(|p| exists(p));
            if mapping {
                continue;
            }
            for path in paths {
                if !exists(&path) && !retired.contains(&path) {
                    problems.push(format!("{name} names {path}, which does not exist"));
                }
            }
        }
    }
    problems
}

// ---------------------------------------------------------------------------
// self-test
// ---------------------------------------------------------------------------

// The TypeScript self-test is a straight line of `if (...) throw`. Deleting one
// of its assertions shrinks the guarantee silently: the flag still prints
// "architecture self-test ok". Here every assertion registers its name against
// REQUIRED_CHECKS, and the run fails if the set is incomplete -- so removing an
// assertion is a loud failure rather than a quieter pass.

/// Every assertion `self_test` must have run before it may report success.
pub const REQUIRED_CHECKS: &[&str] = &[
    "cross_tool_import_caught",
    "lib_and_same_tool_import_allowed",
    "orphan_reported",
    "transitive_reach_counts",
    "modules_lists_files_and_tool_folders",
    "folder_without_index_is_not_a_tool",
    "dead_doc_path_caught",
    "mapping_row_exempt",
    "missing_subcommand_caught",
    "retired_path_silences_a_dead_doc_path",
    "broken_import_caught",
    "dynamic_import_scanned",
];

/// Runs every assertion in REQUIRED_CHECKS and reports the first failure, or a
/// list of the checks that never ran.
pub fn self_test() -> Result<(), String> {
    let mut ran: BTreeSet<&'static str> = BTreeSet::new();
    let mut check = |name: &'static str, ok: bool, why: &str| -> Result<(), String> {
        ran.insert(name);
        if ok {
            Ok(())
        } else {
            Err(format!("{name}: {why}"))
        }
    };

    let fake = vec![
        Module { tool: "overlay".into(), name: "a".into(), source: "from \"../lib/shared.ts\"".into() },
        Module { tool: "overlay".into(), name: "b".into(), source: "from \"./a.ts\"".into() },
        Module { tool: "search".into(), name: "c".into(), source: "from \"../overlay/a.ts\"".into() },
        Module { tool: LIBRARY.into(), name: "shared".into(), source: String::new() },
    ];

    let cross = cross_tool_imports(&fake);
    check(
        "cross_tool_import_caught",
        cross.len() == 1 && cross[0].contains("search"),
        "a cross-tool import must be caught",
    )?;
    let without_search: Vec<Module> = fake.iter().filter(|m| m.tool != "search").cloned().collect();
    check(
        "lib_and_same_tool_import_allowed",
        cross_tool_imports(&without_search).is_empty(),
        "lib/ and same-tool imports must be allowed",
    )?;

    let seeds: BTreeSet<String> = ["b".to_string()].into_iter().collect();
    let orphans = unreachable(&fake, &seeds);
    check(
        "orphan_reported",
        orphans.iter().any(|line| line.contains("/c.ts")),
        "an orphan must be reported",
    )?;
    check(
        "transitive_reach_counts",
        !orphans.iter().any(|line| line.contains("/shared.ts")),
        "transitive reach must count",
    )?;

    let listed = modules(
        &|_| "from \"../lib/x.ts\"".to_string(),
        &|path| {
            if path.is_empty() {
                vec!["overlay".to_string(), "verify.ts".to_string()]
            } else {
                vec!["index.ts".to_string()]
            }
        },
    );
    check(
        "modules_lists_files_and_tool_folders",
        listed.len() == 2,
        "modules() must find the folder's index and the loose file",
    )?;
    let with_stray = modules(
        &|_| String::new(),
        &|path| match path {
            "" => vec!["overlay".to_string(), "gcc296".to_string()],
            "gcc296" => vec!["cc1.ts".to_string()],
            _ => vec!["index.ts".to_string()],
        },
    );
    check(
        "folder_without_index_is_not_a_tool",
        !with_stray.iter().any(|m| m.tool == "gcc296"),
        "a folder without index.ts must not count as a tool",
    )?;

    // Deliberately unresolvable fixture names, for the reason the TypeScript
    // gives: a bulk path-rewriter once retargeted real-looking fixture paths and
    // flipped the assertion by rewriting the predicate rather than the data.
    let present = "tools/kept_fixture/present_fixture.ts";
    let doc_problems = broken_doc_paths(
        &[("X.md".to_string(), format!("see {present}\nand tools/absent_fixture.ts"))],
        &|path| path == present,
        &BTreeSet::new(),
    );
    check(
        "dead_doc_path_caught",
        doc_problems.len() == 1 && doc_problems[0].contains("tools/absent_fixture.ts"),
        "a document naming a missing tool path must be caught",
    )?;
    check(
        "mapping_row_exempt",
        broken_doc_paths(
            &[("X.md".to_string(), format!("| `tools/absent_fixture.ts` | `{present}` |"))],
            &|path| path == present,
            &BTreeSet::new(),
        )
        .is_empty(),
        "an old->new mapping row must not be reported",
    )?;
    check(
        "retired_path_silences_a_dead_doc_path",
        broken_doc_paths(
            &[("X.md".to_string(), "tools/absent_fixture.ts".to_string())],
            &|_| false,
            &["tools/absent_fixture.ts".to_string()].into_iter().collect(),
        )
        .is_empty(),
        "a recorded retirement must silence a dead doc path",
    )?;

    check(
        "missing_subcommand_caught",
        broken_doc_commands(
            &[("Y.md".to_string(), "run bun tools/absent_fixture/index.ts nope_fixture".to_string())],
            &|_| false,
        )
        .len()
            == 1,
        "a document promising a missing subcommand must be caught",
    )?;

    let importers = vec![
        Module { tool: "overlay".into(), name: "index".into(), source: "from \"./helper.ts\"".into() },
    ];
    check(
        "broken_import_caught",
        broken_imports(&importers).len() == 1,
        "an import of a missing sibling must be caught",
    )?;
    let dynamic = vec![
        Module { tool: "overlay".into(), name: "index".into(), source: "await import(\"./gone.ts\")".into() },
    ];
    check(
        "dynamic_import_scanned",
        broken_imports(&dynamic).len() == 1,
        "a dynamic import must be scanned too",
    )?;

    let missing: Vec<&str> = REQUIRED_CHECKS.iter().copied().filter(|name| !ran.contains(name)).collect();
    if !missing.is_empty() {
        return Err(format!(
            "self-test is incomplete: {} of {} checks never ran ({})",
            missing.len(),
            REQUIRED_CHECKS.len(),
            missing.join(", ")
        ));
    }
    let unregistered: Vec<&str> = ran
        .iter()
        .copied()
        .filter(|name| !REQUIRED_CHECKS.contains(name))
        .collect();
    if !unregistered.is_empty() {
        return Err(format!(
            "self-test ran unregistered checks: {}",
            unregistered.join(", ")
        ));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes_and_runs_every_registered_check() {
        self_test().expect("self-test");
        assert_eq!(REQUIRED_CHECKS.len(), 12, "a check was added or deleted without updating the floor");
    }

    fn m(tool: &str, name: &str, source: &str) -> Module {
        Module {
            tool: tool.to_string(),
            name: name.to_string(),
            source: source.to_string(),
        }
    }

    fn docs(pairs: &[(&str, &str)]) -> Vec<(String, String)> {
        pairs
            .iter()
            .map(|(a, b)| (a.to_string(), b.to_string()))
            .collect()
    }

    // ---- ported from the TypeScript --self-test ---------------------------

    fn fake() -> Vec<Module> {
        vec![
            m("overlay", "a", "from \"../lib/shared.ts\""),
            m("overlay", "b", "from \"./a.ts\""),
            m("search", "c", "from \"../overlay/a.ts\""),
            m(LIBRARY, "shared", ""),
        ]
    }

    #[test]
    fn cross_tool_import_is_caught() {
        let cross = cross_tool_imports(&fake());
        assert_eq!(cross.len(), 1);
        assert!(cross[0].contains("search"));
        assert_eq!(
            cross[0],
            "tools/search/c.ts imports overlay/a -- a tool may import lib/, never another tool; move the shared part to lib/"
        );
    }

    #[test]
    fn lib_and_same_tool_imports_are_allowed() {
        let kept: Vec<Module> = fake().into_iter().filter(|m| m.tool != "search").collect();
        assert!(cross_tool_imports(&kept).is_empty());
    }

    #[test]
    fn orphans_are_reported_and_reach_is_transitive() {
        let seeds: BTreeSet<String> = ["b".to_string()].into_iter().collect();
        let orphans = unreachable(&fake(), &seeds);
        assert!(orphans.iter().any(|l| l.contains("/c.ts")));
        assert!(!orphans.iter().any(|l| l.contains("/shared.ts")));
    }

    #[test]
    fn modules_lists_toplevel_files_and_tool_folders() {
        let listed = modules(
            &|_| "from \"../lib/x.ts\"".to_string(),
            &|path| {
                if path.is_empty() {
                    vec!["overlay".to_string(), "verify.ts".to_string()]
                } else {
                    vec!["index.ts".to_string()]
                }
            },
        );
        assert_eq!(listed.len(), 2);
        assert_eq!(listed[0].tool, "overlay");
        assert_eq!(listed[0].name, "index");
        assert_eq!(listed[1].tool, "");
        assert_eq!(listed[1].name, "verify");
    }

    #[test]
    fn folder_without_index_is_not_a_tool() {
        let with_stray = modules(
            &|_| String::new(),
            &|path| match path {
                "" => vec!["overlay".to_string(), "gcc296".to_string()],
                "gcc296" => vec!["cc1.ts".to_string()],
                _ => vec!["index.ts".to_string()],
            },
        );
        assert!(!with_stray.iter().any(|m| m.tool == "gcc296"));
        assert_eq!(with_stray.len(), 1);
    }

    #[test]
    fn document_naming_a_missing_tool_path_is_caught() {
        let present = "tools/kept_fixture/present_fixture.ts";
        let problems = broken_doc_paths(
            &docs(&[("X.md", &format!("see {present}\nand tools/absent_fixture.ts"))]),
            &|path| path == present,
            &BTreeSet::new(),
        );
        assert_eq!(problems.len(), 1);
        assert!(problems[0].contains("tools/absent_fixture.ts"));
        assert_eq!(
            problems[0],
            "X.md names tools/absent_fixture.ts, which does not exist"
        );
    }

    #[test]
    fn mapping_row_is_exempt() {
        let present = "tools/kept_fixture/present_fixture.ts";
        let problems = broken_doc_paths(
            &docs(&[(
                "X.md",
                &format!("| `tools/absent_fixture.ts` | `{present}` |"),
            )]),
            &|path| path == present,
            &BTreeSet::new(),
        );
        assert!(problems.is_empty());
    }

    #[test]
    fn document_promising_a_missing_subcommand_is_caught() {
        let problems = broken_doc_commands(
            &docs(&[("Y.md", "run bun tools/absent_fixture/index.ts nope_fixture")]),
            &|_| false,
        );
        assert_eq!(problems.len(), 1);
        assert_eq!(
            problems[0],
            "Y.md: \"bun tools/absent_fixture/index.ts nope_fixture\" names a subcommand that does not exist"
        );
    }

    // ---- edge cases the TypeScript self-test missed ------------------------

    #[test]
    fn retired_path_silences_a_dead_doc_path() {
        let retired: BTreeSet<String> = ["tools/absent_fixture.ts".to_string()].into_iter().collect();
        assert!(broken_doc_paths(
            &docs(&[("X.md", "tools/absent_fixture.ts")]),
            &|_| false,
            &retired
        )
        .is_empty());
    }

    #[test]
    fn retired_tools_parses_a_table_row() {
        let retired = retired_tools("| `tools/gone_fixture.ts` | `abc1234` | recover |");
        assert!(retired.contains("tools/gone_fixture.ts"));
        assert_eq!(retired.len(), 1);
    }

    #[test]
    fn retired_tools_consumes_the_trailing_pipe() {
        // Non-overlapping matchAll: the `|` between the two cells is eaten by
        // the first match, so the second cell has no opening `|` and is NOT
        // retired. A regex crate with overlapping semantics would find two.
        let retired = retired_tools("| `tools/a.ts` | `tools/b.ts` | `tools/c.ts` |");
        assert!(retired.contains("tools/a.ts"));
        assert!(!retired.contains("tools/b.ts"));
        assert!(retired.contains("tools/c.ts"));
        assert_eq!(retired.len(), 2);
    }

    #[test]
    fn retired_tools_uses_ascii_whitespace_only() {
        // PORT NOTE, pinned so it cannot change unnoticed. The TypeScript's `\s`
        // includes U+00A0 and U+FEFF, so bun retires this row; the
        // source-citations port narrows `\s` to ASCII and does not. This crate
        // takes that definition rather than keeping a second one, and asserts
        // the difference here instead of hiding it.
        let retired = retired_tools("|\u{feff}`tools/nbsp_fixture.ts`\u{a0}|");
        assert!(retired.is_empty(), "the ASCII-only divergence is the documented behaviour");
        // ASCII whitespace on both sides still parses, which is what the real
        // table uses.
        assert!(retired_tools("|  `tools/nbsp_fixture.ts`\t |")
            .contains("tools/nbsp_fixture.ts"));
    }

    #[test]
    fn retired_tools_agrees_on_the_real_provenance() {
        // The divergence above is unobservable on the tree this gate guards:
        // PROVENANCE.md's only non-ASCII characters are U+2014/U+201C/U+201D and
        // its only whitespace is U+0020/U+000A, so no row can hinge on JS `\s`.
        // A floor, not just an equality: the table must still be found at all.
        let root = std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap()
            .join("PROVENANCE.md");
        let text = String::from_utf8_lossy(&std::fs::read(&root).expect("PROVENANCE.md")).into_owned();
        for character in text.chars() {
            if character.is_whitespace() || js_is_space(character) {
                assert!(
                    matches!(character, ' ' | '\n'),
                    "PROVENANCE.md gained {character:?} whitespace -- the ASCII-only \\s divergence is now observable"
                );
            }
        }
        let retired = retired_tools(&text);
        assert!(
            retired.len() >= 6,
            "the retirement table parsed {} rows; it had 6 -- the pattern stopped matching",
            retired.len()
        );
    }

    #[test]
    fn retired_tools_rejects_uppercase_and_missing_backticks() {
        assert!(retired_tools("| `tools/Gone.ts` |").is_empty());
        assert!(retired_tools("| tools/gone.ts |").is_empty());
        assert!(retired_tools("| `tools/gone.js` |").is_empty());
    }

    #[test]
    fn broken_imports_resolves_all_three_shapes() {
        let found = vec![
            m("overlay", "index", "from \"./helper.ts\"\nfrom \"../lib/z.ts\"\nfrom \"../self_test.ts\"\nimport(\"../lib/missing.ts\")"),
            m("overlay", "helper", ""),
            m(LIBRARY, "z", ""),
            m("", "self_test", ""),
        ];
        let problems = broken_imports(&found);
        assert_eq!(
            problems,
            vec!["tools/overlay/index.ts imports lib/missing which does not exist"]
        );
    }

    #[test]
    fn broken_imports_skips_this_checker_itself() {
        let found = vec![m("check", "architecture", "from \"../nowhere/x.ts\"")];
        assert!(broken_imports(&found).is_empty());
        assert!(cross_tool_imports(&found).is_empty());
    }

    #[test]
    fn dynamic_import_is_scanned() {
        let found = vec![m("overlay", "index", "await import(\"./gone.ts\")")];
        assert_eq!(
            broken_imports(&found),
            vec!["tools/overlay/index.ts imports overlay/gone which does not exist"]
        );
    }

    #[test]
    fn nested_doc_path_does_not_match() {
        // `[a-z0-9_]+` cannot span a slash, so `tools/a/b/c.ts` yields no
        // reachability name at all -- pinned because it is load-bearing for
        // seedNames.
        assert!(tools_path_names("tools/a/b/c.ts").is_empty());
        assert_eq!(tools_path_names("tools/a/b.ts"), vec!["b".to_string()]);
        assert_eq!(tools_path_names("tools/b.ts"), vec!["b".to_string()]);
        // No `\b`: an unanchored match fires mid-word, exactly as in JS.
        assert_eq!(tools_path_names("xtools/b.ts"), vec!["b".to_string()]);
    }

    #[test]
    fn doc_paths_accepts_nested_but_not_dotted() {
        assert_eq!(
            doc_paths("see tools/a/b/c.ts here"),
            vec!["tools/a/b/c.ts".to_string()]
        );
        assert!(doc_paths("tools/a.b.ts").is_empty());
        assert_eq!(
            doc_paths("tools/a.ts and tools/b/c.ts"),
            vec!["tools/a.ts".to_string(), "tools/b/c.ts".to_string()]
        );
    }

    #[test]
    fn mapping_row_needs_a_live_path() {
        // Two dead paths on one line are two defects, not a mapping row.
        let problems = broken_doc_paths(
            &docs(&[("X.md", "| `tools/a.ts` | `tools/b.ts` |")]),
            &|_| false,
            &BTreeSet::new(),
        );
        assert_eq!(problems.len(), 2);
    }

    #[test]
    fn entry_points_matches_documented_ts() {
        let names: Vec<String> = ["overlay", "lib", "scratch", "gcc296", "self_test.ts", "README.md"]
            .iter()
            .map(|s| s.to_string())
            .collect();
        let tools = entry_points(
            &names,
            &|name| !name.contains('.'),
            &|name| name != "gcc296",
        );
        assert_eq!(tools, vec!["overlay".to_string(), "self_test".to_string()]);
    }

    #[test]
    fn entry_points_sorts_by_bytes_not_utf16() {
        // PORT NOTE, pinned. The TypeScript's bare `.sort()` orders by UTF-16
        // code unit, where U+FFFD (0xFFFD) sorts BELOW an astral character
        // (0xD800 lead surrogate); `documented::entry_points` sorts by UTF-8
        // byte, where it sorts above. architecture.ts consumes only
        // `tools.length`, so this cannot reach any output of this crate -- but
        // the divergence is asserted rather than assumed away.
        let names: Vec<String> = vec!["\u{10000}.ts".to_string(), "\u{fffd}.ts".to_string()];
        let tools = entry_points(&names, &|_| false, &|_| true);
        assert_eq!(tools, vec!["\u{fffd}".to_string(), "\u{10000}".to_string()]);
        let mut utf16 = vec!["\u{10000}".to_string(), "\u{fffd}".to_string()];
        utf16.sort_by(|a, b| a.encode_utf16().cmp(b.encode_utf16()));
        assert_ne!(tools, utf16, "the two orders really do disagree here");
    }

    #[test]
    fn modules_skips_dotted_entries_and_scratch() {
        let listed = modules(
            &|_| String::new(),
            &|path| {
                if path.is_empty() {
                    vec![
                        "scratch".to_string(),
                        "node_modules".to_string(),
                        "a.out".to_string(),
                        LIBRARY.to_string(),
                    ]
                } else if path == LIBRARY {
                    vec!["z.ts".to_string()]
                } else {
                    vec!["nope.ts".to_string()]
                }
            },
        );
        // lib is exempt from the index.ts rule; scratch and dotted names are
        // skipped; node_modules has no index.ts so it is not a tool.
        assert_eq!(listed.len(), 1);
        assert_eq!(listed[0].tool, LIBRARY);
        assert_eq!(listed[0].name, "z");
    }

    #[test]
    fn unreachable_follows_tools_paths_in_prose() {
        let found = vec![
            m("check", "index", "// see tools/lib/helper.ts for the rest"),
            m(LIBRARY, "helper", ""),
            m(LIBRARY, "orphan", ""),
        ];
        let seeds: BTreeSet<String> = ["index".to_string()].into_iter().collect();
        assert_eq!(
            unreachable(&found, &seeds),
            vec!["tools/lib/orphan.ts is reachable from nothing -- wire it up or delete it".to_string()]
        );
    }

    #[test]
    fn unreachable_survives_an_import_cycle() {
        let found = vec![
            m(LIBRARY, "a", "from \"./b.ts\""),
            m(LIBRARY, "b", "from \"./a.ts\""),
        ];
        let seeds: BTreeSet<String> = ["a".to_string()].into_iter().collect();
        assert!(unreachable(&found, &seeds).is_empty());
    }

    #[test]
    fn js_space_matches_the_spec_set() {
        for c in [' ', '\t', '\n', '\u{b}', '\u{c}', '\r', '\u{a0}', '\u{1680}', '\u{2000}', '\u{200a}', '\u{2028}', '\u{2029}', '\u{202f}', '\u{205f}', '\u{3000}', '\u{feff}'] {
            assert!(js_is_space(c), "{c:?} must be JS whitespace");
        }
        for c in ['a', '|', '\u{200b}', '\u{180e}'] {
            assert!(!js_is_space(c), "{c:?} must not be JS whitespace");
        }
        // Rust and JS disagree here, which is exactly why js_is_space exists.
        assert!(!'\u{feff}'.is_whitespace());
    }

    #[test]
    fn invalid_utf8_is_replaced_not_rejected() {
        // Node's readFileSync(..., "utf8") substitutes U+FFFD; main.rs uses
        // from_utf8_lossy for the same reason.
        let text = String::from_utf8_lossy(b"from \"../lib/x.ts\"\xff");
        assert!(text.ends_with('\u{fffd}'));
        assert_eq!(reach_import_names(&text), vec!["x".to_string()]);
    }

    #[test]
    fn doc_commands_require_the_exact_index_form() {
        assert!(doc_command_matches("bun tools/check/architecture.ts").is_empty());
        assert!(doc_command_matches("bun tools/check/index.ts").is_empty());
        assert_eq!(
            doc_command_matches("bun tools/check/index.ts documented --self-test"),
            vec![("check".to_string(), "documented".to_string())]
        );
    }
}
