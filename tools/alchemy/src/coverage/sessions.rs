//! Which model wrote each commit: a Co-Authored-By trailer that names a
//! version, else an unversioned trailer or agent author, else untagged.
//! Nothing outside the repository's history is read; days before
//! `history::FROZEN_THROUGH` keep their approved counts instead.
use super::history::UNTAGGED;
use std::path::Path;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Family {
    Claude,
    Codex,
    Grok,
}
/// The family of a readable model name.
pub(crate) fn family(name: &str) -> Option<Family> {
    let word = name.split([' ', '-']).next().unwrap_or("");
    match word {
        "Opus" | "Fable" | "Sonnet" | "Haiku" | "Claude" => Some(Family::Claude),
        "GPT" | "Codex" | "Sol" | "Luna" | "Terra" | "Astra" => Some(Family::Codex),
        "Grok" => Some(Family::Grok),
        _ => None,
    }
}
/// The model a Co-Authored-By trailer names, and whether it names a
/// version: "Claude Opus 4.8 (1M context)" is Opus 4.8; "Claude" and
/// "OpenAI Codex" name only their family.
fn trailer_model(trailer: &str) -> Option<(String, bool)> {
    let name = trailer.split(['<', '(']).next().unwrap_or("").trim();
    let name = name.strip_prefix("Claude ").unwrap_or(name);
    if name.contains("Codex") || name.contains("OpenAI") {
        return Some(("Codex".into(), false));
    }
    if name == "Claude" {
        return Some(("Claude".into(), false));
    }
    let versioned = name.bytes().any(|b| b.is_ascii_digit());
    (versioned && family(name).is_some()).then(|| (name.to_string(), true))
}
fn author_model(author: &str) -> Option<&'static str> {
    match author.trim() {
        "Codex" => Some("Codex"),
        // Cursor's agent ran Grok (Pascal, 2026-09-24).
        "Cursor Agent" => Some("Grok"),
        "Claude" => Some("Claude"),
        _ => None,
    }
}

/// One commit as `git log --format=LOG` prints it.
pub(crate) const LOG: &str = "%ad|%an|%(trailers:key=Co-Authored-By,valueonly,separator=;)";
#[derive(Debug, Default)]
pub(crate) struct Commit {
    pub date: String,
    pub author: String,
    pub trailers: String,
}
pub(crate) fn commits(log: &str) -> Vec<Commit> {
    log.lines()
        .filter_map(|line| {
            let mut fields = line.splitn(3, '|');
            Some(Commit {
                date: fields.next()?.into(),
                author: fields.next()?.into(),
                trailers: fields.next().unwrap_or("").into(),
            })
        })
        .collect()
}
/// A commit's models: its versioned trailers, else an unversioned trailer
/// or agent author, else untagged.
pub(crate) fn label(commit: &Commit) -> Vec<String> {
    let named = commit
        .trailers
        .split(';')
        .filter_map(trailer_model)
        .collect::<Vec<_>>();
    let mut versioned = named
        .iter()
        .filter(|(_, v)| *v)
        .map(|(n, _)| n.clone())
        .collect::<Vec<_>>();
    versioned.sort();
    versioned.dedup();
    if !versioned.is_empty() {
        return versioned;
    }
    let hint = named
        .first()
        .map(|(n, _)| n.clone())
        .or_else(|| author_model(&commit.author).map(String::from));
    vec![hint.unwrap_or_else(|| UNTAGGED.into())]
}
/// The `git log` of the checked-out branch, main in CI, from `since` (a date) when given.
pub(crate) fn git_log(root: &Path, since: Option<&str>) -> Result<String, String> {
    let mut command = std::process::Command::new("git");
    command.args([
        "log",
        "HEAD",
        &format!("--format={LOG}"),
        "--date=format:%Y-%m-%d",
    ]);
    if let Some(since) = since {
        command.arg(format!("--since={since} 00:00"));
    }
    let output = command
        .current_dir(root)
        .output()
        .map_err(|e| format!("git log: {e}"))?;
    Ok(String::from_utf8_lossy(&output.stdout).into_owned())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn commits_take_their_trailer_else_their_author() {
        let log = "1970-01-01|Pascal Pixel|Claude Opus 5.5 <n>\n\
                   1970-01-01|Claude|Claude <n>\n\
                   1970-01-01|Pascal Pixel|\n\
                   1970-01-01|Codex|\n\
                   1970-01-01|Cursor Agent|Pascal Pixel <p>\n";
        let labels = commits(log).iter().map(label).collect::<Vec<_>>();
        assert_eq!(
            labels,
            [
                vec!["Opus 5.5".to_string()],
                vec!["Claude".into()],
                vec![UNTAGGED.into()],
                vec!["Codex".into()],
                vec!["Grok".into()],
            ]
        );
        assert_eq!(family("Astra 6"), Some(Family::Codex));
    }
}
