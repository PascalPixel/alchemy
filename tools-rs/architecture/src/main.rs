// CLI entrypoint for the tools/check/architecture.ts port.
//
//   architecture
//   architecture --self-test
//
// Exit codes mirror the TypeScript: 0 on success, 1 when any invariant fails
// (or when the delegated documented.ts run fails).
//
// PORT NOTE -- the TypeScript delegates to documented.ts with
// `Bun.spawnSync(["bun", tools/check/documented.ts])`, captures both streams and
// prints only the trimmed stderr on failure. Here that subprocess is replaced by
// an in-process call into the `documented` crate, which is the same single
// definition the original comment was protecting: `documented::violations` is
// the port of documented.ts, so calling it directly creates no second copy,
// while spawning bun would have kept this binary permanently dependent on the
// TypeScript it replaces. The failure text is reproduced byte for byte,
// including the blank line before the summary and the JS `.trim()` applied to
// the whole block.
//
// PORT NOTE -- directory listings use `std::fs::read_dir` in raw OS order, the
// same order Node's `readdirSync` returns (neither sorts). Listing order is
// observable: it decides the order of the reported problems.
//
// PORT NOTE (deliberate) -- a gate that scanned nothing must fail. If the
// tools tree yields no modules, or the repository yields no markdown, this
// exits 1 with an explicit error instead of printing a green line. The
// TypeScript would pass. On the real tree neither path is reachable.

use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use architecture::{
    broken_doc_commands, broken_doc_paths, broken_imports, cross_tool_imports, entry_points,
    modules, retired_tools, unreachable, Module, LIBRARY,
};

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

/// Node's `readFileSync(path, "utf8")`: invalid bytes become U+FFFD.
fn read_text(path: &Path) -> String {
    let bytes = std::fs::read(path)
        .unwrap_or_else(|error| panic!("cannot read {}: {error}", path.display()));
    String::from_utf8_lossy(&bytes).into_owned()
}

fn list_dir(path: &Path) -> Vec<String> {
    let mut names = Vec::new();
    let entries = std::fs::read_dir(path)
        .unwrap_or_else(|error| panic!("cannot list {}: {error}", path.display()));
    for entry in entries {
        let entry = entry.expect("directory entry");
        names.push(entry.file_name().to_string_lossy().into_owned());
    }
    names
}

fn list_dirs_only(path: &Path) -> Vec<String> {
    let mut names = Vec::new();
    let entries = std::fs::read_dir(path)
        .unwrap_or_else(|error| panic!("cannot list {}: {error}", path.display()));
    for entry in entries {
        let entry = entry.expect("directory entry");
        if entry.file_type().map(|kind| kind.is_dir()).unwrap_or(false) {
            names.push(entry.file_name().to_string_lossy().into_owned());
        }
    }
    names
}

fn markdown_in(directory: &Path, prefix: &str) -> Vec<(String, String)> {
    list_dir(directory)
        .into_iter()
        .filter(|name| name.ends_with(".md"))
        .map(|name| {
            let text = read_text(&directory.join(&name));
            (format!("{prefix}{name}"), text)
        })
        .collect()
}

fn seed_names(root: &Path, tools: &Path) -> BTreeSet<String> {
    let mut parts: Vec<String> = vec![read_text(&root.join("package.json"))];
    for name in list_dir(&root.join(".hooks")) {
        parts.push(read_text(&root.join(".hooks").join(&name)));
    }
    for name in list_dir(root) {
        if name.ends_with(".md") {
            parts.push(read_text(&root.join(&name)));
        }
    }
    let text = parts.join("\n");
    let mut seeds: BTreeSet<String> = BTreeSet::new();
    for name in architecture::seed_targets(&text) {
        seeds.insert(name);
    }
    for entry in list_dirs_only(tools) {
        if entry == LIBRARY {
            continue;
        }
        seeds.insert("index".to_string());
        for file in list_dir(&tools.join(&entry)) {
            if file.ends_with(".ts") {
                seeds.insert(file[..file.len() - 3].to_string());
            }
        }
    }
    seeds
}

/// The floors below are guards, not facts about today's tree: they must stay far
/// enough under the real counts that ordinary growth or pruning never trips
/// them, and far enough above zero that a scanner returning nothing does.
const MODULE_FLOOR: usize = 10;
const DOC_FLOOR: usize = 3;
const RETIRED_FLOOR: usize = 1;
const SEED_FLOOR: usize = 5;

/// PORT NOTE (deliberate) -- a gate that scanned nothing must fail; the
/// TypeScript would print its green line over an empty corpus. Every guard lives
/// in one pure function so each branch can be fired from a fixture count in the
/// tests below rather than by mutating the real tree. `retired`/`seeds` are
/// `None` on the first call, before those two corpora have been read.
fn scanned_nothing(
    modules: usize,
    docs: usize,
    retired: Option<usize>,
    seeds: Option<usize>,
) -> Option<String> {
    if modules == 0 {
        return Some("architecture scanned no modules under tools/ -- nothing was checked".into());
    }
    if docs == 0 {
        return Some("architecture found no markdown to check -- nothing was compared".into());
    }
    if modules < MODULE_FLOOR {
        return Some(format!(
            "architecture found only {modules} modules under tools/ (floor {MODULE_FLOOR}) -- the scanner is broken, not the tree"
        ));
    }
    if docs < DOC_FLOOR {
        return Some(format!(
            "architecture found only {docs} markdown files (floor {DOC_FLOOR}) -- the scanner is broken, not the tree"
        ));
    }
    if let Some(retired) = retired {
        if retired < RETIRED_FLOOR {
            return Some(format!(
                "architecture read only {retired} retirement rows from PROVENANCE.md (floor {RETIRED_FLOOR}) -- the row scanner is broken"
            ));
        }
    }
    if let Some(seeds) = seeds {
        if seeds < SEED_FLOOR {
            return Some(format!(
                "architecture collected only {seeds} reachability seeds (floor {SEED_FLOOR}) -- the seed scanner is broken"
            ));
        }
    }
    None
}

/// `bun tools/check/documented.ts` run in-process. Returns the stderr the
/// subprocess would have produced, or None when it would have exited 0.
fn documented_report(root: &Path, tools_dir: &Path) -> Option<String> {
    let mut names: Vec<String> = list_dir(tools_dir);
    names.sort();
    let tools = documented::entry_points(
        &names,
        &|name: &str| !name.contains('.'),
        &|name: &str| tools_dir.join(name).join("index.ts").exists(),
    );
    let markdown = read_text(&root.join("AGENTS.md"));
    let problems = documented::violations(&tools, &documented::documented(&markdown), root);
    if problems.is_empty() {
        return None;
    }
    let mut text = String::new();
    for problem in &problems {
        text.push_str(&format!("error: {problem}\n"));
    }
    text.push_str(&format!(
        "\n{} tool(s) undocumented. AGENTS.md is the list; there is no other list.\n",
        problems.len()
    ));
    Some(text)
}

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().collect();
    if argv.iter().any(|argument| argument == "--self-test") {
        // PORT NOTE (deliberate) -- the TypeScript flag runs a straight line of
        // `if (...) throw`; deleting one of those lines still prints the same
        // green line. `architecture::self_test` registers every assertion
        // against REQUIRED_CHECKS and refuses to report success if the set is
        // incomplete, so a deleted assertion fails loudly. Success prints the
        // original's exact line and exit code.
        if let Err(why) = architecture::self_test() {
            eprintln!("error: {why}");
            return ExitCode::FAILURE;
        }
        println!("architecture self-test ok");
        return ExitCode::SUCCESS;
    }
    let root = root();
    let tools = root.join("tools");

    let found: Vec<Module> = modules(
        &|path| read_text(&tools.join(path)),
        &|path| list_dir(&tools.join(path)),
    );
    let mut docs = markdown_in(&root, "");
    docs.extend(markdown_in(&root.join("docs"), "docs/"));

    if let Some(why) = scanned_nothing(found.len(), docs.len(), None, None) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }

    let exists = |path: &str| root.join(path).exists();
    let retired = retired_tools(&read_text(&root.join("PROVENANCE.md")));
    let seeds = seed_names(&root, &tools);
    if let Some(why) = scanned_nothing(found.len(), docs.len(), Some(retired.len()), Some(seeds.len())) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }

    let mut problems: Vec<String> = Vec::new();
    problems.extend(cross_tool_imports(&found));
    problems.extend(unreachable(&found, &seeds));
    problems.extend(broken_imports(&found));
    problems.extend(broken_doc_paths(&docs, &exists, &retired));
    problems.extend(broken_doc_commands(&docs, &exists));

    // The delegated documented.ts run. The TypeScript spawns it unconditionally,
    // even when `problems` is already non-empty and its result can no longer
    // change the outcome; that is reproduced (it is only wasted work) so the
    // ordering of any side effect stays identical.
    let documented_stderr = documented_report(&root, &tools);

    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        return ExitCode::FAILURE;
    }
    if let Some(text) = documented_stderr {
        eprintln!("{}", text.trim_matches(architecture::js_is_space));
        return ExitCode::FAILURE;
    }

    let names = list_dir(&tools);
    let tool_names = entry_points(
        &names,
        &|name| !name.contains('.'),
        &|name| tools.join(name).join("index.ts").exists(),
    );
    let lib_modules = found.iter().filter(|m| m.tool == LIBRARY).count();
    println!(
        "architecture ok: {} tools, {} lib modules, no cross-tool imports, nothing unreachable",
        tool_names.len(),
        lib_modules
    );
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Every guard branch, fired from fixture counts. A branch that stopped
    /// firing -- the way a gate quietly goes green -- fails here.
    #[test]
    fn every_scanned_nothing_branch_fires() {
        let fired: Vec<String> = vec![
            scanned_nothing(0, 9, None, None),
            scanned_nothing(9, 0, None, None),
            scanned_nothing(1, 9, None, None),
            scanned_nothing(99, 1, None, None),
            scanned_nothing(99, 9, Some(0), None),
            scanned_nothing(99, 9, Some(9), Some(0)),
        ]
        .into_iter()
        .map(|why| why.expect("guard must fire"))
        .collect();
        let distinct: BTreeSet<&String> = fired.iter().collect();
        assert_eq!(distinct.len(), 6, "each guard must have its own message: {fired:?}");
        assert!(scanned_nothing(99, 9, Some(9), Some(9)).is_none(), "a healthy corpus must pass");
    }

    #[test]
    fn floors_sit_under_the_real_tree() {
        let root = root();
        let tools = root.join("tools");
        let found = architecture::modules(
            &|path| read_text(&tools.join(path)),
            &|path| list_dir(&tools.join(path)),
        );
        let mut docs = markdown_in(&root, "");
        docs.extend(markdown_in(&root.join("docs"), "docs/"));
        let retired = retired_tools(&read_text(&root.join("PROVENANCE.md")));
        let seeds = seed_names(&root, &tools);
        assert_eq!(
            scanned_nothing(found.len(), docs.len(), Some(retired.len()), Some(seeds.len())),
            None,
            "floors must not trip on the real tree: {} modules, {} docs, {} retired, {} seeds",
            found.len(), docs.len(), retired.len(), seeds.len()
        );
        // And well clear of them, so ordinary pruning does not turn a guard into
        // a false alarm.
        assert!(found.len() >= MODULE_FLOOR * 2, "{} modules", found.len());
        assert!(docs.len() >= DOC_FLOOR * 2, "{} docs", docs.len());
    }
}
