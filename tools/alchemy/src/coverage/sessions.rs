//! Which model wrote each commit: a Co-Authored-By trailer that names a
//! version, else an unversioned trailer or agent author, else untagged.
//! Pascal's recorded rules settle the unversioned ones. Nothing outside the
//! repository's history is read.
use super::history::{day_number, UNTAGGED};
use std::collections::BTreeMap;
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
pub(crate) const LOG: &str = "%H|%ct|%ad|%an|%(trailers:key=Co-Authored-By,valueonly,separator=;)";
#[derive(Debug, Default)]
pub(crate) struct Commit {
    pub sha: String,
    pub time: i64,
    pub date: String,
    pub author: String,
    pub trailers: String,
}
pub(crate) fn commits(log: &str) -> Vec<Commit> {
    log.lines()
        .filter_map(|line| {
            let mut fields = line.splitn(5, '|');
            Some(Commit {
                sha: fields.next()?.into(),
                time: fields.next()?.parse().ok()?,
                date: fields.next()?.into(),
                author: fields.next()?.into(),
                trailers: fields.next().unwrap_or("").into(),
            })
        })
        .collect()
}
/// A commit's models before inference (versioned trailers, else an
/// unversioned trailer or agent author, else untagged) and after.
pub(crate) fn label(commit: &Commit) -> (Vec<String>, Vec<String>) {
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
        return (versioned.clone(), versioned);
    }
    let hint = named
        .first()
        .map(|(n, _)| n.clone())
        .or_else(|| author_model(&commit.author).map(String::from));
    let before = vec![hint.unwrap_or_else(|| UNTAGGED.into())];
    // Pascal (2026-09-24): commits no trailer or author places were made
    // with Sol 5.6.
    let after = if before == [UNTAGGED] {
        vec![UNPLACED.to_string()]
    } else {
        before.clone()
    };
    (before, after)
}
/// The model of a commit nothing else identifies.
pub(crate) const UNPLACED: &str = "Sol 5.6";
/// Commits per model per day, and how many commits moved from one label to
/// another, for every commit `git log --format=LOG` printed.
pub(crate) fn tally(
    log: &str,
) -> (
    BTreeMap<String, BTreeMap<String, u64>>,
    BTreeMap<(String, String), u64>,
) {
    let mut days = BTreeMap::<String, BTreeMap<String, u64>>::new();
    let mut moved = BTreeMap::new();
    for commit in commits(log) {
        let (before, after) = label(&commit);
        if before != after {
            *moved
                .entry((before.join("+"), after.join("+")))
                .or_default() += 1;
        }
        let day = days.entry(commit.date.clone()).or_default();
        for model in after {
            *day.entry(model).or_default() += 1;
        }
    }
    resolve_unversioned_claude(&mut days);
    (days, moved)
}
/// Pascal (2026-09-24): an unversioned Claude commit was Fable 5 or Sonnet 5.
/// A day's unversioned commits go to whichever of the two did more that day,
/// else to the one active on the nearest day.
fn resolve_unversioned_claude(days: &mut BTreeMap<String, BTreeMap<String, u64>>) {
    const CANDIDATES: [&str; 2] = ["Fable 5", "Sonnet 5"];
    let active: Vec<(i64, &str, u64)> = days
        .iter()
        .flat_map(|(date, models)| {
            CANDIDATES.iter().filter_map(move |c| {
                models
                    .get(*c)
                    .filter(|n| **n > 0)
                    .map(|n| (day_number(date).unwrap_or(0), *c, *n))
            })
        })
        .collect();
    for models in days.values_mut() {
        if let Some(count) = models.remove("Codex") {
            *models.entry(UNPLACED.to_string()).or_default() += count;
        }
    }
    let dates: Vec<String> = days.keys().cloned().collect();
    for date in dates {
        let Some(count) = days.get_mut(&date).and_then(|m| m.remove("Claude")) else {
            continue;
        };
        let today = day_number(&date).unwrap_or(0);
        let pick = active
            .iter()
            .min_by_key(|(d, c, n)| ((d - today).abs(), std::cmp::Reverse(*n), *c))
            .map_or("Sonnet 5", |(_, c, _)| *c);
        *days
            .get_mut(&date)
            .unwrap()
            .entry(pick.to_string())
            .or_default() += count;
    }
}
/// The `git log` of every branch, from `since` (a date) when given.
pub(crate) fn git_log(root: &Path, since: Option<&str>) -> Result<String, String> {
    let mut command = std::process::Command::new("git");
    command.args([
        "log",
        "--all",
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
        let log = "a|1000|1970-01-01|Pascal Pixel|Claude Opus 5.5 <n>\n\
                   b|1000|1970-01-01|Claude|Claude <n>\n\
                   c|1000|1970-01-01|Pascal Pixel|\n\
                   d|1000|1970-01-01|Codex|\n\
                   g|1000|1970-01-01|Cursor Agent|Pascal Pixel <p>\n";
        let (days, moved) = tally(log);
        let day = &days["1970-01-01"];
        for (model, count) in [("Opus 5.5", 1), ("Sonnet 5", 1), (UNPLACED, 2), ("Grok", 1)] {
            assert_eq!(day.get(model), Some(&count), "{model}");
        }
        assert_eq!(day.get(UNTAGGED), None);
        assert_eq!(moved[&(UNTAGGED.into(), UNPLACED.into())], 1);
        assert_eq!(family("Astra 6"), Some(Family::Codex));
    }
    #[test]
    fn unversioned_claude_goes_to_the_nearest_fable_5_or_sonnet_5() {
        let mut days = BTreeMap::from([
            (
                "2026-07-20".to_string(),
                BTreeMap::from([("Fable 5".to_string(), 3)]),
            ),
            (
                "2026-07-21".to_string(),
                BTreeMap::from([("Claude".to_string(), 2)]),
            ),
            (
                "2026-08-10".to_string(),
                BTreeMap::from([("Sonnet 5".to_string(), 1), ("Claude".to_string(), 4)]),
            ),
        ]);
        resolve_unversioned_claude(&mut days);
        assert_eq!(days["2026-07-21"].get("Fable 5"), Some(&2));
        assert_eq!(days["2026-08-10"].get("Sonnet 5"), Some(&5));
        assert!(days.values().all(|m| !m.contains_key("Claude")));
    }
}
