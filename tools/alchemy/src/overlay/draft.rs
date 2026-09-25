//! `alchemy overlay draft OWNER`: park an open trial as a committed draft.
//!
//! A near miss is never thrown away. The trial source is scored once more
//! and written to the owner's recon draft with its residual in a
//! `NONMATCHING:` header (the figures `alchemy targets` reads), the trial is
//! undone so the listing and registers return to their state before it, and
//! the draft is committed with the progress prefix.

use crate::compiler::source_paths::SourceOwner;
use crate::overlay::names;
use crate::overlay::trial::{overlay_owner, units_path, write_json, Context, Trial};
use crate::score::variants::Scored;
use std::fs;
use std::path::Path;
use std::process::Command;

const USAGE: &str = "usage: alchemy overlay draft OWNER [--note TEXT] [--wall TEXT] [--no-commit] [--trailer TEXT]\n\
Scores OWNER's open trial, writes it to recon/<game>/en/overlays/<owner>.c under a NONMATCHING\n\
header (bytes, candidate size, differing halfwords, halfword edits, date, where it is meant to\n\
live, then `Remaining: TEXT` and `WALL: TEXT`), undoes the trial and commits the draft alone with\n\
the progress prefix and the trailer (default: Co-Authored-By: Claude Opus 5.5\n\
<noreply@anthropic.com>). An exact trial is adopted instead: alchemy overlay adopt OWNER --apply.";

#[derive(Debug, PartialEq)]
struct Options {
    owner: String,
    note: Option<String>,
    wall: Option<String>,
    commit: bool,
    trailer: String,
}

fn parse(arguments: &[String]) -> Result<Option<Options>, String> {
    let mut options = Options {
        owner: String::new(),
        note: None,
        wall: None,
        commit: true,
        trailer: crate::land::TRAILER.into(),
    };
    let mut rest = arguments.iter();
    while let Some(argument) = rest.next() {
        let mut value = || {
            rest.next()
                .cloned()
                .ok_or_else(|| format!("{argument} requires a value"))
        };
        match argument.as_str() {
            "--note" => options.note = Some(value()?),
            "--wall" => options.wall = Some(value()?),
            "--trailer" => options.trailer = value()?,
            "--no-commit" => options.commit = false,
            "-h" | "--help" => return Ok(None),
            flag if flag.starts_with('-') => return Err(format!("unknown option {flag}\n{USAGE}")),
            _ if options.owner.is_empty() => options.owner = argument.clone(),
            other => return Err(format!("unexpected argument {other}\n{USAGE}")),
        }
    }
    if options.owner.is_empty() {
        return Err(USAGE.into());
    }
    Ok(Some(options))
}

/// Today's date, `YYYY-MM-DD`, in UTC.
fn today() -> String {
    let seconds = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |elapsed| elapsed.as_secs());
    civil_date((seconds / 86_400) as i64)
}

/// The proleptic Gregorian date `days` after 1970-01-01.
fn civil_date(days: i64) -> String {
    let z = days + 719_468;
    let era = z.div_euclid(146_097);
    let day_of_era = z.rem_euclid(146_097);
    let year_of_era =
        (day_of_era - day_of_era / 1460 + day_of_era / 36_524 - day_of_era / 146_096) / 365;
    let day_of_year = day_of_era - (365 * year_of_era + year_of_era / 4 - year_of_era / 100);
    let shifted = (5 * day_of_year + 2) / 153;
    let day = day_of_year - (153 * shifted + 2) / 5 + 1;
    let month = if shifted < 10 {
        shifted + 3
    } else {
        shifted - 9
    };
    let year = year_of_era + era * 400 + i64::from(month <= 2);
    format!("{year:04}-{month:02}-{day:02}")
}

/// Words wrapped into a C comment no wider than 78 columns.
fn comment(paragraphs: &[String]) -> String {
    let mut lines = Vec::new();
    for paragraph in paragraphs {
        let mut line = String::new();
        for word in paragraph.split_whitespace() {
            if !line.is_empty() && line.len() + 1 + word.len() > 74 {
                lines.push(std::mem::take(&mut line));
            }
            if !line.is_empty() {
                line.push(' ');
            }
            line.push_str(word);
        }
        lines.push(line);
    }
    let mut text = String::new();
    for (index, line) in lines.iter().enumerate() {
        text.push_str(if index == 0 { "/* " } else { " * " });
        text.push_str(line);
        text.push_str(if index + 1 == lines.len() {
            " */\n"
        } else {
            "\n"
        });
    }
    text
}

/// The draft header: the residual as `alchemy targets` reads it, where the
/// C is meant to live, what remains and any wall.
fn header(
    trial: &Trial,
    scored: &Scored,
    meant_for: &str,
    date: &str,
    note: Option<&str>,
    wall: Option<&str>,
) -> String {
    let number = |value: Option<usize>| value.map_or("?".into(), |value| value.to_string());
    let residual = match &scored.error {
        Some(error) => format!("does not compile ({error})"),
        None => format!(
            "candidate {}, {} differing halfwords, {} halfword edits",
            number(scored.candidate),
            number(scored.differing),
            number(scored.edits)
        ),
    };
    let mut paragraph = format!(
        "NONMATCHING: {} bytes, {residual} ({date}). {}, meant for {meant_for} as a single-overlay unit binding its names at their runtime addresses (an import veneer's listing offset plus 0x8000).",
        trial.span, trial.name
    );
    if let Some(note) = note.filter(|note| !note.trim().is_empty()) {
        paragraph.push_str(&format!(" Remaining: {}", note.trim()));
    }
    let mut paragraphs = vec![paragraph];
    if let Some(wall) = wall.filter(|wall| !wall.trim().is_empty()) {
        paragraphs.push(format!("WALL: {}", wall.trim()));
    }
    comment(&paragraphs)
}

/// The source without a leading `NONMATCHING` header an earlier draft left.
fn without_header(source: &str) -> &str {
    let trimmed = source.trim_start();
    if trimmed.starts_with("/* NONMATCHING") {
        if let Some(end) = trimmed.find("*/") {
            return trimmed[end + 2..].trim_start_matches('\n');
        }
    }
    source
}

fn scored(root: &Path, source: &str, owner: SourceOwner) -> Result<Scored, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let output = Command::new(executable)
        .current_dir(root)
        .args(["score", source, "--owner", &owner.id()])
        .output()
        .map_err(|error| error.to_string())?;
    Ok(Scored::parse(
        &String::from_utf8_lossy(&output.stdout),
        &String::from_utf8_lossy(&output.stderr),
        output.status.success(),
    ))
}

/// The progress prefix a commit subject must start with.
pub fn progress_prefix(root: &Path) -> Result<String, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let output = Command::new(executable)
        .current_dir(root)
        .args(["check", "progress", "--subject"])
        .output()
        .map_err(|error| error.to_string())?;
    let text = String::from_utf8_lossy(&output.stdout);
    let prefix = text.lines().last().unwrap_or("").trim_end().to_string();
    if !output.status.success() || prefix.is_empty() {
        return Err(format!(
            "no progress prefix: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(prefix)
}

pub fn run(root: &Path, arguments: &[String]) -> Result<i32, String> {
    let Some(options) = parse(arguments)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    let owner = overlay_owner(&options.owner)?;
    let trial = Trial::require(root, owner)?;
    let target = trial.target()?;
    let scored = scored(root, &trial.source, owner)?;
    if scored.error.is_none() && scored.differing == Some(0) && scored.candidate == scored.reference
    {
        return Err(format!(
            "{} is exact; adopt it: alchemy overlay adopt {} --apply",
            trial.owner, trial.owner
        ));
    }
    let source = fs::read_to_string(root.join(&trial.source))
        .map_err(|error| format!("{}: {error}", trial.source))?;
    let meant_for = trial
        .source
        .strip_prefix(&format!("{}/SRC/", target.game_dir()))
        .unwrap_or(&trial.source)
        .to_string();
    let text = header(
        &trial,
        &scored,
        &meant_for,
        &today(),
        options.note.as_deref(),
        options.wall.as_deref(),
    ) + without_header(&source);
    let draft = root
        .join(target.recon_dir())
        .join("en/overlays")
        .join(format!("{}.c", owner.legacy_stem()));
    fs::create_dir_all(draft.parent().unwrap()).map_err(|error| error.to_string())?;
    let previous = fs::read(&draft).ok();
    fs::write(&draft, &text).map_err(|error| format!("{}: {error}", draft.display()))?;
    if let Err(error) = trial.undo(root) {
        match previous {
            Some(bytes) => fs::write(&draft, bytes).map_err(|e| e.to_string())?,
            None => fs::remove_file(&draft).map_err(|e| e.to_string())?,
        }
        return Err(error);
    }
    let relative = crate::compiler::build_io::relative(root, &draft);
    let rebound = rebind_retained(root, &trial, &relative, &text)?;
    println!(
        "draft={relative} differing={} trial=undone{}",
        scored
            .differing
            .map_or("?".into(), |value| value.to_string()),
        if rebound {
            " retained_unit=rebound"
        } else {
            ""
        }
    );
    if !options.commit {
        return Ok(0);
    }
    let mut paths = vec![relative.clone()];
    if rebound {
        paths.push(crate::compiler::build_io::relative(
            root,
            units_path(root, target),
        ));
    }
    let subject = format!(
        "{} Draft {} {} ({} bytes, {} differing halfwords)",
        progress_prefix(root)?,
        trial.owner,
        trial.name,
        trial.span,
        scored
            .differing
            .map_or("?".into(), |value| value.to_string())
    );
    let message = format!("{subject}\n\n{}\n", options.trailer);
    let add = Command::new("git")
        .current_dir(root)
        .args(["add", "--"])
        .args(&paths)
        .status()
        .map_err(|error| error.to_string())?;
    let commit = Command::new("git")
        .current_dir(root)
        .args(["commit", "--quiet", "--message", &message, "--"])
        .args(&paths)
        .output()
        .map_err(|error| error.to_string())?;
    if !add.success() || !commit.status.success() {
        let output = format!(
            "{}{}",
            String::from_utf8_lossy(&commit.stdout),
            String::from_utf8_lossy(&commit.stderr)
        );
        let lines = output.lines().collect::<Vec<_>>();
        return Err(format!(
            "git refused the draft commit (its hooks want no other unstaged change in the tree); {relative} is written and staged:\n{}",
            lines[lines.len().saturating_sub(6)..].join("\n")
        ));
    }
    println!("committed {subject}");
    Ok(0)
}

/// A unit the trial displaced that compiles the draft (a retained unit)
/// keeps its bindings and gains any the new draft needs.
fn rebind_retained(root: &Path, trial: &Trial, draft: &str, text: &str) -> Result<bool, String> {
    if !trial
        .units
        .iter()
        .any(|(_, unit)| unit["source"].as_str() == Some(draft))
    {
        return Ok(false);
    }
    let target = trial.target()?;
    let context = Context::load(root, target, trial.owner()?)?;
    let path = units_path(root, target);
    let mut document: serde_json::Value = serde_json::from_str(
        &fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?,
    )
    .map_err(|error| error.to_string())?;
    let mut changed = false;
    for unit in document["units"].as_array_mut().into_iter().flatten() {
        if unit["source"].as_str() != Some(draft) {
            continue;
        }
        let kept = names::declared_symbols(&serde_json::json!({"units": [unit.clone()]}))
            .into_iter()
            .map(|symbol| (symbol.name, symbol.bound))
            .collect();
        let symbols = names::symbols_json(&context.bind([text], &kept, &trial.name));
        if unit["absolute_symbols"] != symbols {
            unit["absolute_symbols"] = symbols;
            changed = true;
        }
    }
    if changed {
        write_json(&path, &document)?;
    }
    Ok(changed)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn parses_note_wall_trailer_and_commit() {
        let options = parse(&args(&[
            "resource_3ca:02000194",
            "--note",
            "r8 allocation",
            "--wall",
            "loop",
            "--no-commit",
        ]))
        .unwrap()
        .unwrap();
        assert_eq!(options.note.as_deref(), Some("r8 allocation"));
        assert_eq!(options.wall.as_deref(), Some("loop"));
        assert!(!options.commit);
        assert_eq!(options.trailer, crate::land::TRAILER);
        assert!(parse(&args(&["resource_3ca:02000194", "--note"])).is_err());
        assert!(parse(&args(&[])).is_err());
    }

    #[test]
    fn dates_are_civil() {
        assert_eq!(civil_date(0), "1970-01-01");
        assert_eq!(civil_date(11_016), "2000-02-29");
        assert_eq!(civil_date(20_721), "2026-09-25");
    }

    fn trial() -> Trial {
        Trial {
            owner: "resource_3ca:02000194".into(),
            target: "tbs-en".into(),
            span: 554,
            name: "BabiFune_Func02000194".into(),
            source: "games/THE BROKEN SEAL/SRC/FIELD/BABI_FUNE/F_00194.C".into(),
            unit: "scene-3ca-02000194".into(),
            record: None,
            units: Vec::new(),
            placeholder: Vec::new(),
            removed: Vec::new(),
        }
    }

    #[test]
    fn header_records_what_targets_reads_and_wraps() {
        let scored = Scored {
            candidate: Some(552),
            reference: Some(554),
            differing: Some(12),
            edits: Some(20),
            error: None,
        };
        let text = header(
            &trial(),
            &scored,
            "FIELD/BABI_FUNE/F_00194.C",
            "2026-09-25",
            Some("the loop keeps r8"),
            Some("r8/sl allocation"),
        );
        assert!(text.starts_with("/* NONMATCHING: 554 bytes, candidate 552, 12 differing halfwords, 20 halfword\n * edits (2026-09-25). BabiFune_Func02000194, meant for\n"), "{text}");
        assert!(
            text.contains("Remaining: the loop keeps r8\n * WALL: r8/sl allocation */\n"),
            "{text}"
        );
        assert!(text.lines().all(|line| line.len() <= 78), "{text}");
        let draft = format!("{text}#include \"TYPES.H\"\n");
        assert_eq!(without_header(&draft), "#include \"TYPES.H\"\n");
        assert_eq!(
            without_header("#include \"TYPES.H\"\n"),
            "#include \"TYPES.H\"\n"
        );
        let failed = Scored {
            error: Some("F_00194.C:3: parse error".into()),
            ..Scored::default()
        };
        assert!(header(&trial(), &failed, "X.C", "2026-09-25", None, None)
            .contains("554 bytes, does not compile (F_00194.C:3: parse error)"));
    }
}
