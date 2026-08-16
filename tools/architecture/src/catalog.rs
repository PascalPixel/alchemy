//! "Does a tool for this already exist?"
//!
//! WHY THIS EXISTS. In one session I rebuilt existing work twice. `reverse-gcc296`
//! reimplemented three `alchemy-permuter` passes, `shape-sweep`'s
//! `InlineSingleUseTemp`, and `statement-order-sweep-main` outright (that last
//! crate is now deleted, its job absorbed). Then, while
//! auditing for exactly that failure, I wrote a duplicate of this very crate's
//! registration check.
//!
//! Neither duplication was caused by the names. Both were caused by not being
//! able to ASK. `tools/` holds 131 crates; grepping for a name only finds a tool
//! if you already guessed its name, and the names are drawn from four different
//! conventions including romaji and raw ROM identifiers. Searching what a tool
//! DOES is a different question from searching what it is called, and only the
//! first one is answerable by someone who does not already know the answer.
//!
//! So: `architecture --search WORD...` matches against crate names AND their doc
//! headers, and reports which crates cannot be searched at all because they carry
//! no description. That count is the real debt, not the crate count.

use std::path::Path;

/// One crate, as the catalog sees it.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Tool {
    pub name: String,
    /// First `//!` line of lib.rs or main.rs, if any.
    pub summary: Option<String>,
}

impl Tool {
    /// Text a search matches against.
    pub fn haystack(&self) -> String {
        let mut text = self.name.replace('-', " ");
        if let Some(summary) = &self.summary {
            text.push(' ');
            text.push_str(summary);
        }
        text.to_lowercase()
    }
}

/// Pull the first doc-comment line out of a Rust source file.
pub fn first_doc_line(source: &str) -> Option<String> {
    for line in source.lines().take(40) {
        let trimmed = line.trim_start();
        if let Some(rest) = trimmed.strip_prefix("//!") {
            let text = rest.trim();
            if !text.is_empty() {
                return Some(text.to_string());
            }
        }
    }
    None
}

/// Read every crate under `tools/`.
pub fn read(tools_directory: &Path) -> Result<Vec<Tool>, String> {
    let mut tools = Vec::new();
    let entries = std::fs::read_dir(tools_directory)
        .map_err(|error| format!("{}: {error}", tools_directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("{}: {error}", tools_directory.display()))?;
        if !entry.file_type().map(|kind| kind.is_dir()).unwrap_or(false) {
            continue;
        }
        let Some(name) = entry.file_name().to_str().map(str::to_string) else { continue };
        if !entry.path().join("Cargo.toml").exists() {
            continue;
        }
        let summary = ["src/lib.rs", "src/main.rs"]
            .iter()
            .filter_map(|relative| std::fs::read_to_string(entry.path().join(relative)).ok())
            .find_map(|source| first_doc_line(&source));
        tools.push(Tool { name, summary });
    }
    tools.sort_by(|a, b| a.name.cmp(&b.name));
    Ok(tools)
}

/// Crates whose haystack contains every search term.
///
/// Every term must match, because the useful question is narrow ("permute
/// statement order") and an any-term match on 131 crates returns noise.
pub fn search<'a>(tools: &'a [Tool], terms: &[String]) -> Vec<&'a Tool> {
    if terms.is_empty() {
        return Vec::new();
    }
    let lowered: Vec<String> = terms.iter().map(|term| term.to_lowercase()).collect();
    tools
        .iter()
        .filter(|tool| {
            let haystack = tool.haystack();
            lowered.iter().all(|term| haystack.contains(term.as_str()))
        })
        .collect()
}

/// Crates with no description, which are invisible to `search`.
pub fn undescribed(tools: &[Tool]) -> Vec<&Tool> {
    tools.iter().filter(|tool| tool.summary.is_none()).collect()
}

/// Render search results for the terminal.
pub fn render(tools: &[Tool], terms: &[String]) -> String {
    let hits = search(tools, terms);
    let mut text = String::new();
    if hits.is_empty() {
        text.push_str(&format!(
            "no tool matches {:?}.\n\
             Before writing a new one, try broader terms: this searches names and\n\
             one-line descriptions, and {} of {} crates carry no description yet.\n",
            terms.join(" "),
            undescribed(tools).len(),
            tools.len()
        ));
        return text;
    }
    text.push_str(&format!("{} of {} crates match {:?}:\n", hits.len(), tools.len(), terms.join(" ")));
    for tool in hits {
        match &tool.summary {
            Some(summary) => text.push_str(&format!("  {:<28} {}\n", tool.name, summary)),
            None => text.push_str(&format!("  {:<28} (no description)\n", tool.name)),
        }
    }
    text
}

#[cfg(test)]
mod tests {
    use super::*;

    fn tool(name: &str, summary: Option<&str>) -> Tool {
        Tool { name: name.to_string(), summary: summary.map(str::to_string) }
    }

    fn corpus() -> Vec<Tool> {
        vec![
            tool("shape-sweep", Some("Native bounded source-shape search.")),
            tool("alchemy-permuter", Some("Alchemy's native, bounded C permutation search.")),
            tool("statement-order-sweep-main", None),
            tool("sentou-hyouji", Some("Battle display resource decoding.")),
        ]
    }

    #[test]
    fn extracts_the_first_doc_line() {
        assert_eq!(
            first_doc_line("//! Native bounded source-shape search.\n//! more\n"),
            Some("Native bounded source-shape search.".to_string())
        );
    }

    #[test]
    fn skips_blank_doc_lines() {
        assert_eq!(first_doc_line("//!\n//!\n//! Real text.\n"), Some("Real text.".to_string()));
    }

    #[test]
    fn a_file_with_no_doc_header_has_no_summary() {
        assert_eq!(first_doc_line("use std::fs;\nfn main() {}\n"), None);
    }

    #[test]
    fn finds_a_tool_by_what_it_does_not_its_name() {
        // The exact case that would have stopped me writing a duplicate.
        let hits = search(&corpus(), &["search".into()]);
        let names: Vec<&str> = hits.iter().map(|t| t.name.as_str()).collect();
        assert!(names.contains(&"shape-sweep"));
        assert!(names.contains(&"alchemy-permuter"));
    }

    #[test]
    fn finds_a_romaji_named_tool_by_its_english_description() {
        // Nobody searching "battle" guesses `sentou-hyouji`.
        let hits = search(&corpus(), &["battle".into()]);
        assert_eq!(hits.len(), 1);
        assert_eq!(hits[0].name, "sentou-hyouji");
    }

    #[test]
    fn every_term_must_match() {
        assert!(search(&corpus(), &["bounded".into(), "permutation".into()]).len() == 1);
        assert!(search(&corpus(), &["bounded".into(), "nonsense".into()]).is_empty());
    }

    #[test]
    fn hyphens_in_names_are_searchable_as_words() {
        assert_eq!(search(&corpus(), &["shape".into()]).len(), 1);
    }

    #[test]
    fn undescribed_crates_are_reported_as_the_real_debt() {
        let missing = undescribed(&corpus());
        assert_eq!(missing.len(), 1);
        assert_eq!(missing[0].name, "statement-order-sweep-main");
    }

    #[test]
    fn a_miss_tells_you_the_search_surface_is_incomplete() {
        let text = render(&corpus(), &["nonexistent".into()]);
        assert!(text.contains("no tool matches"));
        assert!(text.contains("carry no description"), "must admit its own blind spot");
    }

    #[test]
    fn empty_terms_match_nothing_rather_than_everything() {
        assert!(search(&corpus(), &[]).is_empty());
    }
}
