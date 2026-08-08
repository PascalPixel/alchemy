//! Documentation policy for the native command groups.
//!
//! A tool is a top-level group in the native dispatcher.  The dispatcher is
//! the registry; this crate only owns the Markdown parser and the rule that
//! every registered group has a real section in AGENTS.md.

/// The one source of truth for the user-facing native tools.
pub fn entry_points() -> Vec<String> {
    dispatch::groups()
        .iter()
        .map(|group| group.name().to_string())
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
