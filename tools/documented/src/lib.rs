//! Documentation policy for the native command layer.
//!
//! The dispatcher is the registry. Every group has operational policy in
//! AGENTS.md and every public command appears in docs/TOOLS.md.

pub mod cli;

use std::collections::{BTreeMap, BTreeSet};

/// The one source of truth for the user-facing native tools.
pub fn entry_points() -> Vec<String> {
    dispatch::groups()
        .iter()
        .map(|group| group.name().to_string())
        .collect()
}

/// Every public command in dispatcher order.
pub fn command_names() -> Vec<String> {
    dispatch::all_entries()
        .map(|(_, entry)| entry.name.to_string())
        .collect()
}

/// Parsed sections from AGENTS.md, retaining first-seen order while allowing a
/// later duplicate heading to replace its word count.
#[derive(Debug, Default, Clone)]
pub struct Docs {
    entries: Vec<(String, usize)>,
}

impl Docs {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn set(&mut self, name: &str, words: usize) {
        for entry in &mut self.entries {
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

fn is_heading_line(line: &str) -> bool {
    let bytes = line.as_bytes();
    let hashes = bytes.iter().take_while(|byte| **byte == b'#').count();
    (2..=3).contains(&hashes)
        && line[hashes..]
            .chars()
            .next()
            .is_some_and(char::is_whitespace)
}

/// Read a lowercase native group name from a level-two or level-three heading.
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
    while at < bytes.len()
        && (bytes[at].is_ascii_lowercase() || bytes[at].is_ascii_digit() || bytes[at] == b'_')
    {
        at += 1;
    }
    if at == start {
        return None;
    }
    match bytes.get(at) {
        None | Some(b'`') => Some(&line[start..at]),
        Some(byte) if !is_word_byte(*byte) => Some(&line[start..at]),
        Some(_) => None,
    }
}

/// Parse native tool sections and count their prose until the next section.
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

/// Empty input is a broken scan, not a repository with no documentation.
pub fn scanned_nothing(tool_count: usize, document_count: usize) -> Option<&'static str> {
    if tool_count == 0 {
        Some("documented found no native tool groups -- the registry is empty")
    } else if document_count == 0 {
        Some("documented found no Markdown sections -- the documentation scan is empty")
    } else {
        None
    }
}

/// Check the dispatcher registry against AGENTS.md.
pub fn violations(tools: &[String], docs: &Docs) -> Vec<String> {
    let mut problems = Vec::new();
    for tool in tools {
        match docs.get(tool) {
            None => problems.push(format!(
                "{tool}: no section in AGENTS.md -- document it or remove the native tool"
            )),
            Some(words) if words < MINIMUM_WORDS => problems.push(format!(
                "{tool}: only {words} words; a heading is a listing, not documentation"
            )),
            Some(_) => {}
        }
    }
    problems
}

fn fence_marker(line: &str) -> Option<u8> {
    let bytes = line.trim_start().as_bytes();
    let marker = *bytes.first()?;
    if marker != b'`' && marker != b'~' {
        return None;
    }
    (bytes.iter().take_while(|byte| **byte == marker).count() >= 3).then_some(marker)
}

fn table_cells(line: &str) -> Option<Vec<&str>> {
    let line = line.trim();
    let body = line.strip_prefix('|')?;
    let body = body.strip_suffix('|').unwrap_or(body);
    let cells = body.split('|').map(str::trim).collect::<Vec<_>>();
    (!cells.is_empty() && cells.iter().any(|cell| !cell.is_empty())).then_some(cells)
}

fn is_command_table_header(cells: &[&str]) -> bool {
    cells.len() >= 2
        && (cells[0].eq_ignore_ascii_case("command") || cells[0].eq_ignore_ascii_case("commands"))
}

fn is_command_identifier(token: &str) -> bool {
    let mut bytes = token.bytes();
    matches!(bytes.next(), Some(b'a'..=b'z'))
        && bytes.all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
}

fn backticked_identifiers(cell: &str) -> Vec<String> {
    let mut entries = Vec::new();
    let mut remaining = cell;
    while let Some(open) = remaining.find('`') {
        remaining = &remaining[open + 1..];
        let Some(close) = remaining.find('`') else {
            break;
        };
        let token = &remaining[..close];
        if is_command_identifier(token) {
            entries.push(token.to_string());
        }
        remaining = &remaining[close + 1..];
    }
    entries
}

/// Backticked command identifiers in `Command`/`Commands` table rows.
///
/// The catalog deliberately ignores prose, paths, descriptions, and fenced
/// examples. Those are not claims that a command is part of the public
/// dispatcher registry.
pub fn cataloged(markdown: &str) -> Vec<String> {
    let mut entries = Vec::new();
    let mut fence = None;
    let mut in_command_table = false;
    for line in markdown.lines() {
        if let Some(marker) = fence_marker(line) {
            if fence == Some(marker) {
                fence = None;
            } else if fence.is_none() {
                fence = Some(marker);
            }
            in_command_table = false;
            continue;
        }
        if fence.is_some() {
            continue;
        }
        let Some(cells) = table_cells(line) else {
            in_command_table = false;
            continue;
        };
        if is_command_table_header(&cells) {
            in_command_table = true;
            continue;
        }
        if in_command_table {
            entries.extend(backticked_identifiers(cells[0]));
        }
    }
    entries
}

pub fn catalog_violations(commands: &[String], catalog: &[String]) -> Vec<String> {
    let public: BTreeSet<&str> = commands.iter().map(String::as_str).collect();
    let mut counts = BTreeMap::new();
    for entry in catalog {
        *counts.entry(entry.as_str()).or_insert(0usize) += 1;
    }

    let mut problems = commands
        .iter()
        .filter(|command| !counts.contains_key(command.as_str()))
        .map(|command| {
            format!(
                "{command}: absent from docs/TOOLS.md -- document it or remove the dispatch entry"
            )
        })
        .collect::<Vec<_>>();
    for (entry, count) in counts {
        if !public.contains(entry) {
            problems.push(format!(
                "{entry}: stale or unknown command in docs/TOOLS.md -- remove it or register it in the dispatcher"
            ));
        } else if count > 1 {
            problems.push(format!(
                "{entry}: appears {count} times in docs/TOOLS.md -- catalog each public command exactly once"
            ));
        }
    }
    problems
}

/// Exercise the registry/document parser and its failure conditions through
/// the binary's public `--self-test` path.
pub fn self_test() -> Result<String, String> {
    let tools = entry_points();
    let commands = command_names();
    if tools.is_empty() || commands.is_empty() {
        return Err("documented self-test found an empty registry".into());
    }
    let markdown: String = tools
        .iter()
        .map(|tool| format!("## {tool}\n\n{}\n", "native ".repeat(MINIMUM_WORDS)))
        .collect();
    let docs = documented(&markdown);
    if !violations(&tools, &docs).is_empty() {
        return Err("documented self-test rejected complete group documentation".into());
    }
    let catalog_text = commands
        .iter()
        .map(|command| format!("| `{command}` | self-test |"))
        .collect::<Vec<_>>()
        .join("\n");
    let catalog_text = format!("| Command | Purpose |\n|---|---|\n{catalog_text}");
    let catalog = cataloged(&catalog_text);
    if !catalog_violations(&commands, &catalog).is_empty() {
        return Err("documented self-test rejected a complete command catalog".into());
    }
    let mut missing = catalog.clone();
    missing.remove(0);
    if catalog_violations(&commands, &missing).len() != 1 {
        return Err("documented self-test accepted a missing public command".into());
    }
    let mut stale = catalog.clone();
    stale.push("stale_command".into());
    if !catalog_violations(&commands, &stale)
        .iter()
        .any(|problem| problem.starts_with("stale_command: stale or unknown"))
    {
        return Err("documented self-test accepted a stale public command".into());
    }
    let mut duplicate = catalog.clone();
    duplicate.push(commands[0].clone());
    if !catalog_violations(&commands, &duplicate)
        .iter()
        .any(|problem| problem.starts_with(&format!("{}: appears 2 times", commands[0])))
    {
        return Err("documented self-test accepted a duplicate public command".into());
    }
    let short = documented("## overlay\n\nshort\n");
    if violations(&["overlay".into()], &short).is_empty() {
        return Err("documented self-test accepted a short section".into());
    }
    if scanned_nothing(0, docs.len()).is_none() || scanned_nothing(tools.len(), 0).is_none() {
        return Err("documented self-test accepted an empty scan".into());
    }
    Ok(format!(
        "self-test=ok groups={} commands={}",
        tools.len(),
        commands.len()
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn names(list: &[&str]) -> Vec<String> {
        list.iter().map(|name| name.to_string()).collect()
    }

    #[test]
    fn registry_is_native_and_has_the_documented_groups() {
        assert_eq!(
            entry_points(),
            names(&[
                "assets", "check", "compiler", "decomp", "make", "metrics", "overlay", "search",
                "semantic",
            ])
        );
    }

    #[test]
    fn command_catalog_is_dispatch_authoritative() {
        let commands = command_names();
        assert!(commands.len() > 90);
        assert!(commands.contains(&"overlay_adopt".to_string()));
        assert!(commands.contains(&"full_c_progress".to_string()));
        assert!(!commands.contains(&"candidate_explain".to_string()));
    }

    #[test]
    fn catalog_parser_accepts_only_identifier_tokens() {
        let found = cataloged(
            "See `overlay_adopt`, `tools/documented`, and `shape_sweep`.\n\n| Command | Purpose |\n|---|---|\n| `overlay_adopt`, `shape_sweep` | public |\n| `not-public` | stale |\n| `tools/path` | path |",
        );
        assert!(found.iter().any(|entry| entry == "overlay_adopt"));
        assert!(found.iter().any(|entry| entry == "shape_sweep"));
        assert!(!found.iter().any(|entry| entry == "not-public"));
        assert!(!found.iter().any(|entry| entry == "tools/path"));
    }

    #[test]
    fn missing_catalog_commands_fail() {
        let commands = names(&["overlay_adopt", "shape_sweep"]);
        let catalog = cataloged("| Command | Purpose |\n|---|---|\n| `overlay_adopt` | public |");
        assert_eq!(
            catalog_violations(&commands, &catalog),
            vec![
                "shape_sweep: absent from docs/TOOLS.md -- document it or remove the dispatch entry"
                    .to_string()
            ]
        );
    }

    #[test]
    fn stale_catalog_commands_fail() {
        let commands = names(&["overlay_adopt"]);
        let catalog = cataloged(
            "| Command | Purpose |\n|---|---|\n| `overlay_adopt` | public |\n| `old_command` | removed |",
        );
        let problems = catalog_violations(&commands, &catalog);
        assert!(problems.iter().any(|problem| {
            problem == "old_command: stale or unknown command in docs/TOOLS.md -- remove it or register it in the dispatcher"
        }));
    }

    #[test]
    fn duplicate_catalog_commands_fail() {
        let commands = names(&["overlay_adopt"]);
        let catalog = cataloged(
            "| Command | Purpose |\n|---|---|\n| `overlay_adopt` | first |\n| `overlay_adopt` | second |",
        );
        assert_eq!(catalog.len(), 2);
        assert_eq!(
            catalog_violations(&commands, &catalog),
            vec![
                "overlay_adopt: appears 2 times in docs/TOOLS.md -- catalog each public command exactly once"
                    .to_string()
            ]
        );
    }

    #[test]
    fn fenced_examples_and_prose_paths_are_not_catalog_entries() {
        let found = cataloged(
            "```markdown\n| Command | Purpose |\n|---|---|\n| `fenced_command` | example |\n```\nSee `tools/documented` and `example_command` in prose.\n\n| Command | Purpose |\n|---|---|\n| `overlay_adopt` | public |",
        );
        assert_eq!(found, names(&["overlay_adopt"]));
    }

    #[test]
    fn current_live_catalog_matches_the_dispatch_registry() {
        let commands = command_names();
        let catalog = cataloged(include_str!("../../../docs/TOOLS.md"));
        assert_eq!(catalog.len(), commands.len());
        assert!(catalog_violations(&commands, &catalog).is_empty());
    }

    #[test]
    fn a_real_paragraph_counts() {
        let docs = documented(&format!(
            "## overlay\n\n{}\n\n## verify\n\ntoo short\n",
            "word ".repeat(30)
        ));
        assert_eq!(docs.get("overlay"), Some(30));
        assert_eq!(docs.get("verify"), Some(2));
    }

    #[test]
    fn missing_and_short_sections_fail() {
        let docs = documented(&format!(
            "## overlay\n\n{}\n\n## verify\n\ntoo short\n",
            "word ".repeat(30)
        ));
        let problems = violations(&names(&["overlay", "verify", "ghost"]), &docs);
        assert_eq!(
            problems,
            vec![
                "verify: only 2 words; a heading is a listing, not documentation".to_string(),
                "ghost: no section in AGENTS.md -- document it or remove the native tool"
                    .to_string(),
            ]
        );
    }

    #[test]
    fn non_tool_headings_do_not_become_tools() {
        let docs = documented("## Ledgers\n\none two three\n\n## overlay\n\nwords\n");
        assert_eq!(docs.get("Ledgers"), None);
        assert_eq!(docs.get("overlay"), Some(1));
    }

    #[test]
    fn heading_edge_cases_match_the_native_rule() {
        assert_eq!(
            documented("## `overlay`\n\nwords here\n").get("overlay"),
            Some(2)
        );
        assert_eq!(
            documented("## overlay — the code overlays\n\nwords\n").get("overlay"),
            Some(1)
        );
        assert!(documented("## overlayX\n\nwords words words\n").is_empty());
        assert!(documented("#### deeper\n\nwords\n").is_empty());
        assert!(documented("# overlay\n\nwords\n").is_empty());
        assert!(documented("##overlay\n\nwords\n").is_empty());
    }

    #[test]
    fn duplicate_sections_keep_first_position_and_last_count() {
        let docs = documented("## a1\n\none\n\n## b2\n\nx\n\n## a1\n\none two three\n");
        let order: Vec<&str> = docs.iter().map(|(name, _)| name).collect();
        assert_eq!(order, vec!["a1", "b2"]);
        assert_eq!(docs.get("a1"), Some(3));
    }

    #[test]
    fn empty_scans_are_failures() {
        assert!(scanned_nothing(0, 1).is_some());
        assert!(scanned_nothing(1, 0).is_some());
        assert!(scanned_nothing(1, 1).is_none());
    }

    #[test]
    fn exactly_the_minimum_passes() {
        let docs = documented(&format!("## overlay\n\n{}", "word ".repeat(MINIMUM_WORDS)));
        assert!(violations(&names(&["overlay"]), &docs).is_empty());
    }
}
