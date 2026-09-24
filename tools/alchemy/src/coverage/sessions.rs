//! Which model wrote each commit. A Co-Authored-By trailer that names a
//! version wins; otherwise the agent session that was producing events in
//! the repository just before the commit, read from the local Codex, Claude
//! Code and Grok logs, names it. A commit with neither keeps what its author
//! or unversioned trailer says, else counts as untagged.
use super::history::{day_number, UNTAGGED};
use std::collections::{BTreeMap, HashMap};
use std::path::{Path, PathBuf};

/// A session is active for a commit when it produced an event this many
/// seconds before it, or shortly after (the tool result of the commit).
const BEFORE: i64 = 300;
const AFTER: i64 = 60;
/// Directory names the repository has been checked out under: its own, its
/// worktree folder, and the name it had in July before it was renamed.
const CHECKOUTS: [&str; 3] = ["alchemy", "alchemy-worktrees", "goldensun-pure"];

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
fn title(word: &str) -> String {
    let mut chars = word.chars();
    chars
        .next()
        .map(|c| c.to_uppercase().chain(chars).collect())
        .unwrap_or_default()
}
/// A logged model id as the chart names it: `claude-opus-5-5` is "Opus
/// 5.5", `gpt-5.6-sol` is "Sol 5.6", `grok-4.6` is "Grok 4.6". Ids that
/// name no working model (reviewers, aliases, placeholders) give `None`.
pub(crate) fn readable(id: &str) -> Option<String> {
    if let Some(rest) = id.strip_prefix("claude-") {
        let mut parts = rest.split('-');
        let name = title(parts.next()?);
        let version = parts
            .take_while(|part| part.len() <= 2 && part.bytes().all(|b| b.is_ascii_digit()))
            .collect::<Vec<_>>();
        return (!version.is_empty()).then(|| format!("{name} {}", version.join(".")));
    }
    if let Some(rest) = id.strip_prefix("gpt-") {
        let mut parts = rest.split('-');
        let version = parts.next()?;
        let words = parts.map(title).collect::<Vec<_>>();
        // Named like Claude's models (Pascal, 2026-09-24): "Sol 5.6".
        return Some(if words.is_empty() {
            format!("GPT-{version}")
        } else {
            format!("{} {version}", words.join(" "))
        });
    }
    let version = id.strip_prefix("grok-")?;
    Some(format!("Grok {version}"))
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
        "Cursor Agent" => Some("Cursor"),
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

/// Every event the agents logged in the repository, as (time, model, cwd)
/// with both names interned.
#[derive(Default)]
pub(crate) struct Activity {
    names: Vec<String>,
    index: HashMap<String, u32>,
    events: Vec<(i64, u32, u32)>,
}
impl Activity {
    fn intern(&mut self, name: &str) -> u32 {
        if let Some(id) = self.index.get(name) {
            return *id;
        }
        let id = self.names.len() as u32;
        self.names.push(name.into());
        self.index.insert(name.into(), id);
        id
    }
    pub(crate) fn push(&mut self, time: i64, model: &str, cwd: &str) {
        if !in_repository(cwd) {
            return;
        }
        let event = (time, self.intern(model), self.intern(cwd));
        if self.events.last() != Some(&event) {
            self.events.push(event);
        }
    }
    fn sort(&mut self) {
        self.events.sort_unstable();
        self.events.dedup();
    }
    /// The model of the event nearest `time` among the sessions active for
    /// it, within `family` when given, preferring sessions working in
    /// `path`.
    pub(crate) fn model_at(
        &self,
        time: i64,
        family_of: Option<Family>,
        path: Option<&str>,
    ) -> Option<&str> {
        let from = self.events.partition_point(|e| e.0 < time - BEFORE);
        let to = self.events.partition_point(|e| e.0 <= time + AFTER);
        let active = self.events[from..to]
            .iter()
            .filter(|e| family_of.is_none() || family(&self.names[e.1 as usize]) == family_of)
            .collect::<Vec<_>>();
        let here = |e: &(i64, u32, u32)| {
            path.is_some_and(|path| {
                let cwd = &self.names[e.2 as usize];
                cwd == path || cwd.starts_with(&format!("{path}/"))
            })
        };
        let preferred = if active.iter().any(|e| here(e)) {
            active.into_iter().filter(|e| here(e)).collect()
        } else {
            active
        };
        preferred
            .into_iter()
            // Nearest first; at equal distance, the event before the commit.
            .min_by_key(|e| ((e.0 - time).abs(), e.0 > time))
            .map(|e| self.names[e.1 as usize].as_str())
    }
}
fn in_repository(cwd: &str) -> bool {
    cwd.split('/').any(|part| CHECKOUTS.contains(&part))
}

/// Seconds since 1970 of an ISO-8601 UTC time such as
/// `2026-07-20T10:48:24.528Z`.
pub(crate) fn iso_seconds(text: &str) -> Option<i64> {
    let day = day_number(text.get(..10)?)?;
    let field = |at: usize| text.get(at..at + 2)?.parse::<i64>().ok();
    Some(day * 86_400 + field(11)? * 3600 + field(14)? * 60 + field(17)?)
}
/// The string value following the first unescaped `key` in a JSON line;
/// quotes inside JSON strings are escaped, so a match is a real key.
fn value_after<'a>(line: &'a str, key: &str) -> Option<&'a str> {
    let start = line.find(key)? + key.len();
    let end = line[start..].find('"')?;
    Some(&line[start..start + end])
}
fn value_last<'a>(line: &'a str, key: &str) -> Option<&'a str> {
    let start = line.rfind(key)? + key.len();
    let end = line[start..].find('"')?;
    Some(&line[start..start + end])
}
fn jsonl_files(dir: &Path, since: i64, out: &mut Vec<PathBuf>) {
    let Ok(entries) = std::fs::read_dir(dir) else {
        return;
    };
    for entry in entries.flatten() {
        let path = entry.path();
        if path.is_dir() {
            jsonl_files(&path, since, out);
        } else if path.extension().is_some_and(|e| e == "jsonl") {
            let modified = entry
                .metadata()
                .and_then(|m| m.modified())
                .ok()
                .and_then(|t| t.duration_since(std::time::UNIX_EPOCH).ok())
                .map_or(i64::MAX, |d| d.as_secs() as i64);
            if modified >= since {
                out.push(path);
            }
        }
    }
}

/// Claude Code: every assistant message carries its model and timestamp.
pub(crate) fn read_claude(text: &str, activity: &mut Activity) {
    for line in text.lines() {
        let (Some(id), Some(time)) = (
            value_after(line, "\"message\":{\"model\":\""),
            value_last(line, "\"timestamp\":\"").and_then(iso_seconds),
        ) else {
            continue;
        };
        if let (Some(model), Some(cwd)) = (readable(id), value_last(line, "\"cwd\":\"")) {
            activity.push(time, &model, cwd);
        }
    }
}
/// Codex: each turn names its model and working directory; every later
/// event in the file carries a timestamp. A review thread's own model
/// writes nothing, so its events count for the thread it reviews, whose
/// model `threads` gives even when that thread's own log is gone.
pub(crate) fn read_codex(text: &str, threads: &HashMap<String, String>, activity: &mut Activity) {
    let mut parent: Option<String> = None;
    let mut turn: Option<(String, String)> = None;
    for line in text.lines() {
        if line.contains("\"type\":\"session_meta\"") {
            parent = value_after(line, "\"parent_thread_id\":\"")
                .and_then(|id| threads.get(id))
                .and_then(|model| readable(model));
            continue;
        }
        if line.contains("\"type\":\"turn_context\"") {
            let context = serde_json::from_str::<serde_json::Value>(line).ok();
            let payload = context.as_ref().map(|c| &c["payload"]);
            turn = payload.and_then(|p| {
                let model = p["model"]
                    .as_str()
                    .and_then(readable)
                    .or_else(|| parent.clone());
                Some((model?, p["cwd"].as_str()?.to_string()))
            });
            continue;
        }
        let (Some((model, cwd)), Some(time)) = (
            &turn,
            line.strip_prefix("{\"timestamp\":\"").and_then(iso_seconds),
        ) else {
            continue;
        };
        activity.push(time, model, cwd);
    }
}
/// Every Codex thread's model by id, from the desktop app's thread index.
fn codex_threads(home: &Path) -> HashMap<String, String> {
    let output = std::process::Command::new("sqlite3")
        .arg("-readonly")
        .arg(home.join(".codex/state_5.sqlite"))
        .arg("select id || '|' || model from threads where model is not null")
        .output();
    let text = output
        .map(|o| String::from_utf8_lossy(&o.stdout).into_owned())
        .unwrap_or_default();
    text.lines()
        .filter_map(|line| line.split_once('|'))
        .map(|(id, model)| (id.to_string(), model.to_string()))
        .collect()
}
/// Grok: a session's summary names its directory and model; each update
/// line carries a Unix timestamp and, on prompts, the model it used.
pub(crate) fn read_grok(summary: &str, updates: &str, activity: &mut Activity) {
    let Ok(summary) = serde_json::from_str::<serde_json::Value>(summary) else {
        return;
    };
    let (Some(cwd), Some(mut model)) = (
        summary["info"]["cwd"].as_str(),
        summary["current_model_id"].as_str().and_then(readable),
    ) else {
        return;
    };
    for line in updates.lines() {
        if let Some(named) = value_after(line, "\"modelId\":\"").and_then(readable) {
            model = named;
        }
        let time = line
            .strip_prefix("{\"timestamp\":")
            .and_then(|rest| rest.split(',').next())
            .and_then(|n| n.parse().ok());
        if let Some(time) = time {
            activity.push(time, &model, cwd);
        }
    }
}
/// Everything the local agent logs under `home` say about the repository,
/// from files modified at or after `since`.
pub(crate) fn activity(home: &Path, since: i64) -> Activity {
    let mut activity = Activity::default();
    let read = |path: &Path| {
        std::fs::read(path)
            .ok()
            .map(|b| String::from_utf8_lossy(&b).into_owned())
    };
    let mut files = Vec::new();
    let threads = codex_threads(home);
    jsonl_files(&home.join(".codex/sessions"), since, &mut files);
    for path in files.drain(..) {
        if let Some(text) = read(&path).filter(|t| t.contains("\"cwd\":\"")) {
            if CHECKOUTS
                .iter()
                .any(|name| text.contains(&format!("/{name}")))
            {
                read_codex(&text, &threads, &mut activity);
            }
        }
    }
    for dir in std::fs::read_dir(home.join(".claude/projects"))
        .into_iter()
        .flatten()
        .flatten()
    {
        let name = dir.file_name().to_string_lossy().into_owned();
        if CHECKOUTS.iter().any(|checkout| name.contains(checkout)) {
            jsonl_files(&dir.path(), since, &mut files);
        }
    }
    for path in files.drain(..) {
        if let Some(text) = read(&path) {
            read_claude(&text, &mut activity);
        }
    }
    for scope in std::fs::read_dir(home.join(".grok/sessions"))
        .into_iter()
        .flatten()
        .flatten()
    {
        for session in std::fs::read_dir(scope.path())
            .into_iter()
            .flatten()
            .flatten()
        {
            let (summary, updates) = (
                read(&session.path().join("summary.json")),
                read(&session.path().join("updates.jsonl")),
            );
            if let (Some(summary), Some(updates)) = (summary, updates) {
                read_grok(&summary, &updates, &mut activity);
            }
        }
    }
    activity.sort();
    activity
}

/// Which checkout made each commit, from the reflogs of the main checkout
/// and every live worktree.
pub(crate) fn checkouts(root: &Path) -> HashMap<String, String> {
    let mut out = HashMap::new();
    let common = std::process::Command::new("git")
        .args(["rev-parse", "--path-format=absolute", "--git-common-dir"])
        .current_dir(root)
        .output()
        .ok()
        .map(|o| PathBuf::from(String::from_utf8_lossy(&o.stdout).trim()));
    let Some(common) = common else {
        return out;
    };
    let mut logs = Vec::new();
    if let Some(main) = common.parent() {
        logs.push((common.join("logs/HEAD"), main.to_path_buf()));
    }
    for entry in std::fs::read_dir(common.join("worktrees"))
        .into_iter()
        .flatten()
        .flatten()
    {
        let gitdir = std::fs::read_to_string(entry.path().join("gitdir")).unwrap_or_default();
        if let Some(path) = Path::new(gitdir.trim()).parent() {
            logs.push((entry.path().join("logs/HEAD"), path.to_path_buf()));
        }
    }
    for (log, path) in logs {
        let text = std::fs::read_to_string(log).unwrap_or_default();
        for line in text.lines() {
            let Some((head, message)) = line.split_once('\t') else {
                continue;
            };
            if message.starts_with("commit") {
                if let Some(sha) = head.split(' ').nth(1) {
                    out.insert(sha.to_string(), path.display().to_string());
                }
            }
        }
    }
    out
}

/// A commit's models before inference (versioned trailers, else an
/// unversioned trailer or agent author, else untagged) and after.
pub(crate) fn label(
    commit: &Commit,
    activity: &Activity,
    checkouts: &HashMap<String, String>,
) -> (Vec<String>, Vec<String>) {
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
    let before = vec![hint.clone().unwrap_or_else(|| UNTAGGED.into())];
    // Cursor left no logs, so another agent's session says nothing of it.
    if hint.as_deref() == Some("Cursor") {
        return (before.clone(), before);
    }
    let family_of = hint.as_deref().and_then(family);
    let path = checkouts.get(&commit.sha).map(String::as_str);
    let after = activity
        .model_at(commit.time, family_of, path)
        .map_or_else(|| before.clone(), |model| vec![model.to_string()]);
    // Pascal (2026-09-24): commits no trailer, author or session places were
    // made with Sol 5.6.
    let after = if after == [UNTAGGED] {
        vec![UNPLACED.to_string()]
    } else {
        after
    };
    (before, after)
}
/// The model of a commit nothing else identifies.
pub(crate) const UNPLACED: &str = "Sol 5.6";
/// Commits per model per day, and how many commits moved from one label to
/// another, for every commit `git log --format=LOG` printed.
pub(crate) fn tally(
    log: &str,
    activity: &Activity,
    checkouts: &HashMap<String, String>,
) -> (
    BTreeMap<String, BTreeMap<String, u64>>,
    BTreeMap<(String, String), u64>,
) {
    let mut days = BTreeMap::<String, BTreeMap<String, u64>>::new();
    let mut moved = BTreeMap::new();
    for commit in commits(log) {
        let (before, after) = label(&commit, activity, checkouts);
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
pub(crate) fn home() -> PathBuf {
    std::env::var_os("HOME")
        .map(PathBuf::from)
        .unwrap_or_default()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn logged_ids_read_as_versioned_names() {
        for (id, name) in [
            ("claude-opus-5-5", Some("Opus 5.5")),
            ("claude-opus-5", Some("Opus 5")),
            ("claude-haiku-4-5-20251001", Some("Haiku 4.5")),
            ("claude-fable-5-1", Some("Fable 5.1")),
            ("gpt-5.6-sol", Some("Sol 5.6")),
            ("gpt-6-astra", Some("Astra 6")),
            ("grok-4.6", Some("Grok 4.6")),
            ("codex-auto-review", None),
            ("<synthetic>", None),
            ("opus", None),
        ] {
            assert_eq!(readable(id).as_deref(), name, "{id}");
        }
        assert_eq!(family("Astra 6"), Some(Family::Codex));
        assert_eq!(iso_seconds("1970-01-02T01:02:03.500Z"), Some(90_123));
    }
    #[test]
    fn commits_take_their_trailer_else_the_nearest_active_session() {
        let mut activity = Activity::default();
        read_claude(
            "{\"message\":{\"model\":\"claude-opus-5\",\"content\":\"\\\"message\\\":{\\\"model\\\":\\\"x\\\"\"},\"cwd\":\"/r/alchemy\",\"timestamp\":\"1970-01-01T00:16:40.000Z\"}\n\
             {\"message\":{\"model\":\"claude-fable-5-1\"},\"cwd\":\"/r/alchemy-worktrees/b\",\"timestamp\":\"1970-01-01T00:16:00.000Z\"}\n\
             {\"message\":{\"model\":\"claude-sonnet-5\"},\"cwd\":\"/r/alchemy-demo\",\"timestamp\":\"1970-01-01T00:16:40.000Z\"}\n",
            &mut activity,
        );
        // A reviewer's events count for the Sol thread it reviews.
        let threads = HashMap::from([("t".to_string(), "gpt-5.6-sol".to_string())]);
        read_codex(
            "{\"timestamp\":\"1970-01-01T00:00:00.000Z\",\"type\":\"session_meta\",\"payload\":{\"parent_thread_id\":\"t\"}}\n\
             {\"timestamp\":\"1970-01-01T00:00:00.000Z\",\"type\":\"turn_context\",\"payload\":{\"model\":\"codex-auto-review\",\"cwd\":\"/r/alchemy\"}}\n\
             {\"timestamp\":\"1970-01-01T00:16:30.000Z\",\"type\":\"event_msg\"}\n",
            &threads,
            &mut activity,
        );
        read_grok(
            "{\"info\":{\"cwd\":\"/r/alchemy\"},\"current_model_id\":\"grok-4.6\"}",
            "{\"timestamp\":5000,\"method\":\"x\"}\n",
            &mut activity,
        );
        activity.sort();
        let checkouts = HashMap::from([("b".to_string(), "/r/alchemy-worktrees/b".to_string())]);
        let log = "a|1000|1970-01-01|Pascal Pixel|Claude Opus 5.5 <n>\n\
                   b|1000|1970-01-01|Claude|Claude <n>\n\
                   c|1000|1970-01-01|Pascal Pixel|\n\
                   d|1000|1970-01-01|Codex|\n\
                   e|5010|1970-01-01|Pascal Pixel|\n\
                   f|9000|1970-01-01|Pascal Pixel|\n\
                   g|1000|1970-01-01|Cursor Agent|Pascal Pixel <p>\n";
        let (days, moved) = tally(log, &activity, &checkouts);
        let day = &days["1970-01-01"];
        for (model, count) in [
            ("Opus 5.5", 1),
            ("Fable 5.1", 1),
            ("Opus 5", 1),
            ("Sol 5.6", 2),
            ("Grok 4.6", 1),
            ("Cursor", 1),
        ] {
            assert_eq!(day.get(model), Some(&count), "{model}");
        }
        assert_eq!(moved[&("Claude".into(), "Fable 5.1".into())], 1);
        assert_eq!(moved[&(UNTAGGED.into(), "Opus 5".into())], 1);
        assert_eq!(day.get(UNTAGGED), None);
        assert_eq!(moved.values().sum::<u64>(), 5);
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
