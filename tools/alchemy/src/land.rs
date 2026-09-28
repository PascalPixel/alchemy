//! `alchemy land BRANCH...`: one landing from a landing worktree.
//!
//! Fetch, reset to origin/main, merge each branch, prove the result (test,
//! compare-all, coverage, verify) and squash it into one commit whose
//! subject carries the progress prefix. Conflicts resolve by kind: figures
//! coverage writes again keep main's side, recon drafts keep the branch's,
//! and the JSON registers merge structurally, keeping both sides' additions.
//! Anything else stops the landing with the list. Pushing `main` needs
//! Pascal's authority, so it happens only with `--push`.

use serde_json::{Map, Value};
use std::fs;
use std::path::Path;
use std::process::Command;
use std::time::Instant;

/// The trailer every lane commit carries unless told otherwise.
pub const TRAILER: &str = "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>";

const USAGE: &str =
    "usage: alchemy land BRANCH... [--message TEXT] [--body TEXT] [--trailer TEXT] [--push]\n\
From a clean linked landing worktree: fetch, reset to origin/main, merge each BRANCH (a local\n\
branch, else origin/BRANCH). Conflicts resolve by kind: README.md, PROGRESS*.png and\n\
recon/<game>/metrics/history.json take main's side (coverage writes them again), drafts under\n\
recon/<game>/en take the branch's side, and recon/<game>/source-paths.json,\n\
translation-units.json and semantic/*.json merge structurally, keeping both sides' additions;\n\
any other conflict stops the landing. Retired unit fields (game, compiler_route, owner state) are\n\
stripped and conflict markers under games/ recon/ tools/ refused. Then make test, compare-all,\n\
coverage (again while a figure is pending) and verify, and one squashed commit:\n\
\"<progress prefix> TEXT\" (default: Land BRANCH, ...), the body, then the trailer (default:\n\
Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>). --push then runs git push origin\n\
HEAD:main; without it nothing leaves the worktree. Logs: out/land/<step>.log.";

#[derive(Debug, Default, PartialEq)]
struct Options {
    branches: Vec<String>,
    message: Option<String>,
    body: Option<String>,
    trailer: Option<String>,
    push: bool,
}

pub fn entry(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(arguments)?;
    land(crate::compiler::routing::root(), &options)
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut index = 0;
    while index < arguments.len() {
        let arg = arguments[index].as_str();
        let mut value = || {
            index += 1;
            arguments
                .get(index)
                .cloned()
                .ok_or_else(|| format!("{arg} requires a value\n{USAGE}"))
        };
        match arg {
            "-m" | "--message" => options.message = Some(value()?),
            "--body" => options.body = Some(value()?),
            "--trailer" => options.trailer = Some(value()?),
            "--push" => options.push = true,
            flag if flag.starts_with('-') => return Err(format!("unknown option {flag}\n{USAGE}")),
            branch => options.branches.push(branch.to_string()),
        }
        index += 1;
    }
    if options.branches.is_empty() {
        return Err(USAGE.into());
    }
    Ok(options)
}

fn land(root: &Path, options: &Options) -> Result<(), String> {
    let common = git(
        root,
        &["rev-parse", "--path-format=absolute", "--git-common-dir"],
    )?;
    let own = git(root, &["rev-parse", "--path-format=absolute", "--git-dir"])?;
    if common == own {
        return Err("land from a linked landing worktree, not the main checkout".into());
    }
    let dirty = git(root, &["status", "--porcelain", "--untracked-files=no"])?;
    if !dirty.is_empty() {
        return Err(format!("the landing worktree is not clean:\n{dirty}"));
    }
    git(root, &["fetch", "--quiet", "origin"])?;
    let dropped = git(root, &["log", "--oneline", "origin/main..HEAD"])?;
    if !dropped.is_empty() {
        println!("resetting past local commits (still in the reflog):\n{dropped}");
    }
    // Other worktrees share origin/main; squash onto the commit merged here.
    let base = git(root, &["rev-parse", "origin/main"])?;
    git(root, &["reset", "--quiet", "--hard", &base])?;
    merge_branches(root, &options.branches)?;
    let stripped = strip_retired_fields(root)?;
    if stripped > 0 {
        println!("stripped retired unit fields: {stripped}");
    }
    let markers = conflict_markers(root)?;
    if !markers.is_empty() {
        return Err(format!(
            "conflict markers remain; the merged tree is left for inspection:\n{markers}"
        ));
    }
    let logs = root.join("out/land");
    fs::create_dir_all(&logs).map_err(|error| error.to_string())?;
    for target in ["test", "compare-all", "coverage"] {
        make_step(root, &logs, target)?;
    }
    if progress_prefix(root)?.contains("?%") {
        make_step(root, &logs, "coverage")?;
    }
    git(root, &["add", "--update"])?;
    make_step(root, &logs, "verify")?;
    let prefix = progress_prefix(root)?;
    git(root, &["reset", "--quiet", "--soft", &base])?;
    let message = commit_message(&prefix, options);
    let outcome = step(
        root,
        &logs,
        "commit",
        Command::new("git").args(["commit", "--quiet", "--message", &message]),
    )?;
    if !outcome {
        return Err("the commit hooks refused the landing; the squashed change is staged".into());
    }
    println!("landed {}", git(root, &["log", "--oneline", "-1"])?);
    if options.push {
        if !step(
            root,
            &logs,
            "push",
            Command::new("git").args(["push", "origin", "HEAD:main"]),
        )? {
            return Err("push refused".into());
        }
    } else {
        println!("not pushed; with Pascal's authority: git push origin HEAD:main (or --push)");
    }
    Ok(())
}

/// The progress prefix `make progress-subject` prints, from the merged tree.
fn progress_prefix(root: &Path) -> Result<String, String> {
    let text = output(Command::new("make").current_dir(root).args([
        "-s",
        "--no-print-directory",
        "progress-subject",
    ]))?;
    Ok(text.lines().last().unwrap_or("").trim_end().to_string())
}

/// Runs one step with its output in `out/land/<name>.log`, printing one
/// line, and the log's tail when it fails.
fn step(root: &Path, logs: &Path, name: &str, command: &mut Command) -> Result<bool, String> {
    let path = logs.join(format!("{name}.log"));
    let file = fs::File::create(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let errors = file.try_clone().map_err(|error| error.to_string())?;
    let started = Instant::now();
    let status = command
        .current_dir(root)
        .stdin(std::process::Stdio::null())
        .stdout(file)
        .stderr(errors)
        .status()
        .map_err(|error| format!("{name}: {error}"))?;
    let seconds = started.elapsed().as_secs_f64();
    if status.success() {
        println!("ok   {name:<12} {seconds:.1}s");
        return Ok(true);
    }
    let log = fs::read_to_string(&path).unwrap_or_default();
    let lines = log.lines().collect::<Vec<_>>();
    println!(
        "FAIL {name:<12} {seconds:.1}s  ({})\n{}",
        path.display(),
        lines[lines.len().saturating_sub(20)..].join("\n")
    );
    Ok(false)
}

fn make_step(root: &Path, logs: &Path, target: &str) -> Result<(), String> {
    if step(
        root,
        logs,
        target,
        Command::new("make").args(["--no-print-directory", target]),
    )? {
        Ok(())
    } else {
        Err(format!(
            "make {target} failed; the merged tree is left for inspection"
        ))
    }
}

/// How a landing resolves one conflicted path.
#[derive(Debug, PartialEq)]
enum Resolution {
    /// Figures coverage writes again: main's side.
    Main,
    /// Recon drafts: the branch's side, its newer attempt.
    Branch,
    /// JSON registers: a structural three-way merge.
    Structural,
    /// Anything else stops the landing.
    Stop,
}

fn resolution(path: &str) -> Resolution {
    if matches!(path, "README.md" | "PROGRESS.png" | "PROGRESS_CHART.png") {
        return Resolution::Main;
    }
    let Some((_, rest)) = path
        .strip_prefix("recon/")
        .and_then(|rest| rest.split_once('/'))
    else {
        return Resolution::Stop;
    };
    match rest {
        "metrics/history.json" => Resolution::Main,
        "source-paths.json" | "translation-units.json" => Resolution::Structural,
        _ if rest.starts_with("semantic/") && rest.ends_with(".json") => Resolution::Structural,
        _ if rest.starts_with("en/") && !rest.ends_with(".json") => Resolution::Branch,
        _ => Resolution::Stop,
    }
}

/// Merges each branch onto HEAD, resolving conflicts by `resolution`; an
/// unresolved one aborts that merge and stops with the list.
fn merge_branches(root: &Path, branches: &[String]) -> Result<(), String> {
    for branch in branches {
        let local = format!("refs/heads/{branch}");
        let reference = if git(root, &["rev-parse", "--verify", "--quiet", &local]).is_ok() {
            branch.clone()
        } else {
            format!("origin/{branch}")
        };
        git(root, &["rev-parse", "--verify", "--quiet", &reference])
            .map_err(|_| format!("{branch}: no local branch or origin/{branch}"))?;
        let merged = Command::new("git")
            .current_dir(root)
            .args([
                "merge",
                "--no-ff",
                "--no-edit",
                "--no-verify",
                "--quiet",
                "-m",
            ])
            .arg(format!("Merge {branch}"))
            .arg(&reference)
            .output()
            .map_err(|error| error.to_string())?;
        if merged.status.success() {
            println!("merged {branch}");
            continue;
        }
        let conflicts = git(root, &["diff", "--name-only", "--diff-filter=U"])?;
        if conflicts.is_empty() {
            let _ = git(root, &["merge", "--abort"]);
            return Err(format!(
                "{branch}: merge failed: {}",
                String::from_utf8_lossy(&merged.stderr).trim()
            ));
        }
        let mut unresolved = Vec::new();
        let mut resolved = Vec::new();
        for path in conflicts.lines() {
            let outcome = match resolution(path) {
                Resolution::Main => take_side(root, path, 2).map(|()| "main"),
                Resolution::Branch => take_side(root, path, 3).map(|()| "branch"),
                Resolution::Structural => merge_json(root, path).map(|()| "merged"),
                Resolution::Stop => Err(String::new()),
            };
            match outcome {
                Ok(how) => resolved.push(format!("{path} ({how})")),
                Err(error) if error.is_empty() => unresolved.push(path.to_string()),
                Err(error) => unresolved.push(format!("{path}: {error}")),
            }
        }
        if !unresolved.is_empty() {
            let _ = git(root, &["merge", "--abort"]);
            return Err(format!(
                "{branch}: conflicts a landing does not resolve; merge it by hand:\n{}",
                unresolved.join("\n")
            ));
        }
        git(root, &["commit", "--quiet", "--no-verify", "--no-edit"])?;
        println!("merged {branch}: {}", resolved.join(", "));
    }
    Ok(())
}

/// Resolves a conflicted path to one stage (2 main, 3 branch), deleting it
/// when that side deleted it.
fn take_side(root: &Path, path: &str, stage: u8) -> Result<(), String> {
    if git(root, &["cat-file", "-e", &format!(":{stage}:{path}")]).is_ok() {
        let side = if stage == 2 { "--ours" } else { "--theirs" };
        git(root, &["checkout", side, "--", path])?;
        git(root, &["add", "--", path])?;
    } else {
        git(root, &["rm", "--quiet", "--force", "--", path])?;
    }
    Ok(())
}

/// Three-way merges one conflicted JSON register from its index stages.
fn merge_json(root: &Path, path: &str) -> Result<(), String> {
    let stage = |number: u8| -> Result<Option<Value>, String> {
        match git(root, &["show", &format!(":{number}:{path}")]) {
            Ok(text) => serde_json::from_str(&text)
                .map(Some)
                .map_err(|error| format!("stage {number}: {error}")),
            Err(_) => Ok(None),
        }
    };
    let (base, ours, theirs) = (stage(1)?, stage(2)?, stage(3)?);
    let mut conflicts = Vec::new();
    let merged = merge_values(
        base.as_ref(),
        ours.as_ref(),
        theirs.as_ref(),
        "",
        &mut conflicts,
    );
    if !conflicts.is_empty() {
        conflicts.truncate(8);
        return Err(format!("both sides changed {}", conflicts.join(", ")));
    }
    match merged {
        Some(value) => {
            let text = serde_json::to_string_pretty(&value).map_err(|error| error.to_string())?;
            fs::write(root.join(path), format!("{text}\n")).map_err(|error| error.to_string())?;
            git(root, &["add", "--", path])?;
        }
        None => {
            git(root, &["rm", "--quiet", "--force", "--", path])?;
        }
    }
    Ok(())
}

/// The key a register row merges by: its `id`, else its overlay and start
/// (or entry) address.
fn row_key(value: &Value) -> Option<String> {
    if let Some(id) = value["id"].as_str() {
        return Some(format!("id {id}"));
    }
    let overlay = value["overlay"].as_str()?;
    let start = value["start"]
        .as_str()
        .or_else(|| value["entry"].as_str())?;
    Some(format!("{overlay} {start}"))
}

/// Each row of an array by its key, when every row has a distinct one.
fn keyed(rows: &[Value]) -> Option<Vec<(String, &Value)>> {
    let keyed = rows
        .iter()
        .map(|row| row_key(row).map(|key| (key, row)))
        .collect::<Option<Vec<_>>>()?;
    let distinct = keyed
        .iter()
        .map(|(key, _)| key)
        .collect::<std::collections::BTreeSet<_>>();
    (distinct.len() == keyed.len()).then_some(keyed)
}

/// A three-way merge: a side that did not change a value takes the other's
/// change; objects merge by key and keyed arrays by row, both sides'
/// additions kept (main's order first). A value both sides changed
/// differently is recorded in `conflicts` and keeps main's side. `None` is
/// an absent value.
fn merge_values(
    base: Option<&Value>,
    ours: Option<&Value>,
    theirs: Option<&Value>,
    at: &str,
    conflicts: &mut Vec<String>,
) -> Option<Value> {
    if ours == theirs {
        return ours.cloned();
    }
    if base == ours {
        return theirs.cloned();
    }
    if base == theirs {
        return ours.cloned();
    }
    match (ours, theirs) {
        (Some(Value::Object(left)), Some(Value::Object(right))) => {
            let origin = base.and_then(Value::as_object);
            let mut merged = Map::new();
            for key in left
                .keys()
                .chain(right.keys().filter(|key| !left.contains_key(*key)))
            {
                let value = merge_values(
                    origin.and_then(|origin| origin.get(key)),
                    left.get(key),
                    right.get(key),
                    &format!("{at}.{key}"),
                    conflicts,
                );
                if let Some(value) = value {
                    merged.insert(key.clone(), value);
                }
            }
            Some(Value::Object(merged))
        }
        (Some(Value::Array(left)), Some(Value::Array(right))) => {
            let origin = base
                .and_then(Value::as_array)
                .map(Vec::as_slice)
                .unwrap_or(&[]);
            let (Some(left_rows), Some(right_rows), Some(origin_rows)) =
                (keyed(left), keyed(right), keyed(origin))
            else {
                conflicts.push(at.to_string());
                return ours.cloned();
            };
            let mut merged = Vec::new();
            for (key, row) in &left_rows {
                let value = merge_values(
                    find_row(&origin_rows, key),
                    Some(row),
                    find_row(&right_rows, key),
                    &format!("{at}[{key}]"),
                    conflicts,
                );
                merged.extend(value);
            }
            for (key, row) in right_rows
                .iter()
                .filter(|(key, _)| find_row(&left_rows, key).is_none())
            {
                let value = merge_values(
                    find_row(&origin_rows, key),
                    None,
                    Some(row),
                    &format!("{at}[{key}]"),
                    conflicts,
                );
                merged.extend(value);
            }
            Some(Value::Array(merged))
        }
        _ => {
            conflicts.push(at.to_string());
            ours.cloned()
        }
    }
}

fn find_row<'a>(rows: &[(String, &'a Value)], key: &str) -> Option<&'a Value> {
    rows.iter()
        .find(|(other, _)| other == key)
        .map(|(_, row)| *row)
}

/// Removes the unit fields older lane tooling wrote and placement now
/// decides (`game`, `compiler_route`, an owner's `state`) from every
/// game's translation-unit register; returns how many it removed.
fn strip_retired_fields(root: &Path) -> Result<usize, String> {
    let mut removed = 0;
    for game in ["tbs", "tla"] {
        let path = root.join(format!("recon/{game}/translation-units.json"));
        if !path.is_file() {
            continue;
        }
        let text =
            fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        let mut document: Value =
            serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))?;
        let count = strip_units(&mut document);
        if count > 0 {
            let rendered = serde_json::to_string_pretty(&document).map_err(|e| e.to_string())?;
            fs::write(&path, format!("{rendered}\n")).map_err(|error| error.to_string())?;
            removed += count;
        }
    }
    Ok(removed)
}

fn strip_units(document: &mut Value) -> usize {
    let mut removed = 0;
    for unit in document["units"].as_array_mut().into_iter().flatten() {
        let Some(unit) = unit.as_object_mut() else {
            continue;
        };
        for field in ["game", "compiler_route"] {
            removed += usize::from(unit.shift_remove(field).is_some());
        }
        for owner in unit
            .get_mut("owners")
            .and_then(Value::as_array_mut)
            .into_iter()
            .flatten()
        {
            if let Some(owner) = owner.as_object_mut() {
                removed += usize::from(owner.shift_remove("state").is_some());
            }
        }
    }
    removed
}

/// Whole-line conflict markers under games/, recon/ and tools/.
fn conflict_markers(root: &Path) -> Result<String, String> {
    let found = Command::new("git")
        .current_dir(root)
        .args([
            "grep",
            "-n",
            "-I",
            "-E",
            "^(<<<<<<< |>>>>>>> |=======$)",
            "--",
            "games",
            "recon",
            "tools",
        ])
        .output()
        .map_err(|error| error.to_string())?;
    // git grep exits 1 when nothing matches.
    match found.status.code() {
        Some(0) => Ok(String::from_utf8_lossy(&found.stdout)
            .trim_end()
            .to_string()),
        Some(1) => Ok(String::new()),
        _ => Err(String::from_utf8_lossy(&found.stderr).trim().to_string()),
    }
}

fn commit_message(prefix: &str, options: &Options) -> String {
    let text = options
        .message
        .clone()
        .unwrap_or_else(|| format!("Land {}", options.branches.join(", ")));
    let mut message = format!("{} {}", prefix.trim_end(), text.trim());
    if let Some(body) = &options.body {
        message.push_str("\n\n");
        message.push_str(body.trim());
    }
    message.push_str("\n\n");
    message.push_str(options.trailer.as_deref().unwrap_or(TRAILER));
    message.push('\n');
    message
}

fn git(root: &Path, arguments: &[&str]) -> Result<String, String> {
    output(Command::new("git").current_dir(root).args(arguments))
}

fn output(command: &mut Command) -> Result<String, String> {
    let output = command.output().map_err(|error| error.to_string())?;
    if output.status.success() {
        Ok(String::from_utf8_lossy(&output.stdout)
            .trim_end()
            .to_string())
    } else {
        Err(String::from_utf8_lossy(&output.stderr).trim().to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn parses_branches_message_trailer_and_push() {
        let options = parse(&args(&[
            "wf/a",
            "wf/b",
            "-m",
            "Land two lanes",
            "--trailer",
            "X: y",
            "--push",
        ]))
        .unwrap();
        assert_eq!(options.branches, ["wf/a", "wf/b"]);
        assert_eq!(options.message.as_deref(), Some("Land two lanes"));
        assert_eq!(options.trailer.as_deref(), Some("X: y"));
        assert!(options.push);
        assert!(parse(&args(&["-m", "no branch"])).is_err());
        assert!(parse(&args(&["wf/a", "--force"])).is_err());
        assert!(!parse(&args(&["wf/a"])).unwrap().push);
    }

    #[test]
    fn message_is_prefix_subject_body_and_trailer() {
        let options = Options {
            branches: vec!["wf/a".into(), "wf/b".into()],
            body: Some("Three lanes.".into()),
            ..Options::default()
        };
        assert_eq!(
            commit_message("☀️ 70% ⚓️ 2% –\n", &options),
            format!("☀️ 70% ⚓️ 2% – Land wf/a, wf/b\n\nThree lanes.\n\n{TRAILER}\n")
        );
        let options = Options {
            message: Some("Land overlay lanes".into()),
            trailer: Some("A: b".into()),
            ..Options::default()
        };
        assert_eq!(
            commit_message("P –", &options),
            "P – Land overlay lanes\n\nA: b\n"
        );
    }

    #[test]
    fn conflicts_resolve_by_kind() {
        for path in [
            "README.md",
            "PROGRESS.png",
            "PROGRESS_CHART.png",
            "recon/tbs/metrics/history.json",
            "recon/tla/metrics/history.json",
        ] {
            assert_eq!(resolution(path), Resolution::Main, "{path}");
        }
        for path in [
            "recon/tbs/en/overlays/resource_3ca_c_02000194.c",
            "recon/tbs/en/080bbb0c.c",
        ] {
            assert_eq!(resolution(path), Resolution::Branch, "{path}");
        }
        for path in [
            "recon/tbs/source-paths.json",
            "recon/tbs/translation-units.json",
            "recon/tla/translation-units.json",
            "recon/tbs/semantic/overlay-assembly.json",
            "recon/tbs/semantic/regions.json",
        ] {
            assert_eq!(resolution(path), Resolution::Structural, "{path}");
        }
        for path in [
            "AGENTS.md",
            "recon/tbs/metrics/audit-verification.json",
            "recon/tbs/en/strings.json",
            "games/README.md",
            "tools/alchemy/src/land.rs",
            "recon/tbs/raw/overlays/resource_3ca_overlay.s",
        ] {
            assert_eq!(resolution(path), Resolution::Stop, "{path}");
        }
    }

    fn merged(base: Value, ours: Value, theirs: Value) -> (Option<Value>, Vec<String>) {
        let mut conflicts = Vec::new();
        let value = merge_values(Some(&base), Some(&ours), Some(&theirs), "", &mut conflicts);
        (value, conflicts)
    }

    #[test]
    fn registers_keep_both_sides_additions() {
        let base = json!({"format": 3, "owners": {"a": "A.C", "b": {"name": "B"}}});
        let ours = json!({"format": 3, "owners": {"a": "A.C", "b": {"name": "B"}, "c": {"name": "C", "source": "C.C"}}});
        let theirs = json!({"format": 3, "owners": {"b": {"name": "B", "source": "B.C"}, "d": "D.C", "a": "A.C"}});
        let (value, conflicts) = merged(base, ours, theirs);
        assert!(conflicts.is_empty());
        assert_eq!(
            serde_json::to_string(&value.unwrap()).unwrap(),
            r#"{"format":3,"owners":{"a":"A.C","b":{"name":"B","source":"B.C"},"c":{"name":"C","source":"C.C"},"d":"D.C"}}"#
        );
    }

    #[test]
    fn keyed_rows_merge_by_id_or_overlay_and_start() {
        let unit = |id: &str, source: &str| json!({"id": id, "source": source});
        let base = json!({"units": [unit("a", "A"), unit("b", "B"), unit("gone", "G")]});
        let ours = json!({"units": [unit("a", "A2"), unit("b", "B"), unit("gone", "G"), unit("ours", "O")]});
        let theirs = json!({"units": [unit("a", "A"), unit("b", "B"), unit("theirs", "T")]});
        let (value, conflicts) = merged(base, ours, theirs);
        assert!(conflicts.is_empty());
        let ids = value.unwrap()["units"]
            .as_array()
            .unwrap()
            .iter()
            .map(|unit| {
                format!(
                    "{}={}",
                    unit["id"].as_str().unwrap(),
                    unit["source"].as_str().unwrap()
                )
            })
            .collect::<Vec<_>>();
        assert_eq!(ids, ["a=A2", "b=B", "ours=O", "theirs=T"]);
        // An adoption on the branch drops an evidence row; main adds another.
        let row = |start: &str| json!({"overlay": "resource_3ca", "start": start, "kind": "not_yet_decompiled"});
        let base = json!({"regions": [row("0x02000100"), row("0x02000194")]});
        let ours = json!({"regions": [row("0x02000100"), row("0x02000194"), row("0x02000400")]});
        let theirs = json!({"regions": [row("0x02000100")]});
        let (value, conflicts) = merged(base, ours, theirs);
        assert!(conflicts.is_empty());
        assert_eq!(
            value.unwrap()["regions"],
            json!([row("0x02000100"), row("0x02000400")])
        );
    }

    #[test]
    fn values_both_sides_changed_are_conflicts() {
        let (_, conflicts) = merged(
            json!({"owners": {"a": "A.C"}}),
            json!({"owners": {"a": "B.C"}}),
            json!({"owners": {"a": "C.C"}}),
        );
        assert_eq!(conflicts, [".owners.a"]);
        let (_, conflicts) = merged(
            json!({"units": [1, 2]}),
            json!({"units": [1, 2, 3]}),
            json!({"units": [0, 1, 2]}),
        );
        assert_eq!(conflicts, [".units"]);
        // A row one side changed and the other deleted.
        let (_, conflicts) = merged(
            json!([{"id": "a", "x": 1}]),
            json!([{"id": "a", "x": 2}]),
            json!([]),
        );
        assert_eq!(conflicts, ["[id a]"]);
    }

    #[test]
    fn retired_unit_fields_are_stripped() {
        let mut document = json!({"units": [
            {"id": "a", "game": "tbs", "source": "S", "compiler_route": "canonical-gcc296", "owners": [{"address": "0x02000398", "extent": 420, "state": "exact-c"}]},
            {"id": "b", "source": "T", "owners": [{"address": "0x08000000"}]}
        ]});
        assert_eq!(strip_units(&mut document), 3);
        assert_eq!(
            serde_json::to_string(&document).unwrap(),
            r#"{"units":[{"id":"a","source":"S","owners":[{"address":"0x02000398","extent":420}]},{"id":"b","source":"T","owners":[{"address":"0x08000000"}]}]}"#
        );
        assert_eq!(strip_units(&mut document), 0);
    }

    /// A scratch repository whose main three branches change.
    fn repository() -> tempfile::TempDir {
        let directory = tempfile::tempdir().unwrap();
        let work = directory.path().join("work");
        let run = |arguments: &[&str]| {
            let status = Command::new("git")
                .current_dir(&work)
                .args(arguments)
                .output()
                .unwrap();
            assert!(
                status.status.success(),
                "{arguments:?}: {}",
                String::from_utf8_lossy(&status.stderr)
            );
        };
        let write = |path: &str, text: &str| {
            let path = work.join(path);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        let register = |owners: Value| {
            serde_json::to_string_pretty(&json!({"format": 3, "owners": owners})).unwrap() + "\n"
        };
        fs::create_dir_all(&work).unwrap();
        run(&["init", "--quiet", "-b", "main"]);
        for (key, value) in [
            ("user.name", "t"),
            ("user.email", "t@t"),
            ("commit.gpgsign", "false"),
        ] {
            run(&["config", key, value]);
        }
        write("README.md", "progress 1\n");
        write("tools/a.rs", "one\n");
        write(
            "recon/tbs/source-paths.json",
            &register(json!({"main:08000000": "A.C"})),
        );
        write(
            "recon/tbs/en/overlays/resource_3ca_c_02000194.c",
            "/* draft 1 */\n",
        );
        run(&["add", "-A"]);
        run(&["commit", "--quiet", "-m", "base"]);
        let branches: [(&str, &str, &str, Value, &str); 3] = [
            (
                "wf/a",
                "progress 2\n",
                "one\ntwo\n",
                json!({"main:08000000": "A.C", "main:08000010": "B.C"}),
                "/* draft 2 */\n",
            ),
            (
                "wf/b",
                "progress 3\n",
                "one\n",
                json!({"main:08000000": "A.C", "main:08000020": "C.C"}),
                "/* draft 3 */\n",
            ),
            (
                "wf/c",
                "progress 1\n",
                "three\n",
                json!({"main:08000000": "A.C"}),
                "/* draft 1 */\n",
            ),
        ];
        for (branch, readme, tool, owners, draft) in branches {
            run(&["checkout", "--quiet", "-b", branch, "main"]);
            write("README.md", readme);
            write("tools/a.rs", tool);
            write("recon/tbs/source-paths.json", &register(owners));
            write("recon/tbs/en/overlays/resource_3ca_c_02000194.c", draft);
            run(&["commit", "--quiet", "-am", branch]);
        }
        run(&["checkout", "--quiet", "main"]);
        directory
    }

    #[test]
    fn landings_resolve_figures_drafts_and_registers_and_stop_on_the_rest() {
        let directory = repository();
        let work = directory.path().join("work");
        let read = |path: &str| fs::read_to_string(work.join(path)).unwrap();
        // wf/a lands; wf/b then conflicts in the figures (main's side), the
        // draft (the branch's side) and the register (both additions kept).
        merge_branches(&work, &["wf/a".into(), "wf/b".into()]).unwrap();
        assert_eq!(read("README.md"), "progress 2\n");
        assert_eq!(read("tools/a.rs"), "one\ntwo\n");
        assert_eq!(
            read("recon/tbs/en/overlays/resource_3ca_c_02000194.c"),
            "/* draft 3 */\n"
        );
        let register: Value = serde_json::from_str(&read("recon/tbs/source-paths.json")).unwrap();
        assert_eq!(
            register["owners"],
            json!({"main:08000000": "A.C", "main:08000010": "B.C", "main:08000020": "C.C"})
        );
        assert!(read("recon/tbs/source-paths.json").ends_with("}\n"));
        assert_eq!(conflict_markers(&work).unwrap(), "");
        // wf/c conflicts in tools/a.rs: the merge is aborted and listed.
        let error = merge_branches(&work, &["wf/c".into()]).unwrap_err();
        assert!(
            error.contains("wf/c: conflicts a landing does not resolve")
                && error.ends_with("tools/a.rs"),
            "{error}"
        );
        assert_eq!(git(&work, &["status", "--porcelain"]).unwrap(), "");
        assert!(merge_branches(&work, &["wf/missing".into()])
            .unwrap_err()
            .contains("no local branch"));
        fs::write(
            work.join("tools/a.rs"),
            "<<<<<<< HEAD\none\n=======\nthree\n>>>>>>> wf/c\n",
        )
        .unwrap();
        assert_eq!(conflict_markers(&work).unwrap().lines().count(), 3);
    }
}
