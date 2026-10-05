//! DONE (AGENTS.md C2, C3): pret's calcrom over the linker maps of each
//! game's six verified builds, published as README's progress line and the
//! decomp.dev report from one measurement.
pub(crate) mod boxtree;
pub(crate) mod calcrom;
pub(crate) mod decomp;
pub(crate) mod figure;
pub(crate) mod figures;
pub(crate) mod history;
pub(crate) mod jsnum;
pub(crate) mod letters;
pub(crate) mod model;
pub(crate) mod palette;
pub(crate) mod progress;
pub(crate) mod publish;
pub(crate) mod raster;
pub(crate) mod sessions;
pub(crate) mod tree;

use crate::coverage::calcrom::Game;
use crate::coverage::progress::GameDone;
use std::path::Path;

const USAGE: &str = "usage: alchemy check coverage [--write|--check]\n\
Measures both games once, each from its six verified builds (make compare-editions); a game\n\
without all six stays pending. --write publishes README's progress line and the decomp.dev\n\
report from that one measurement, so it also serves as the check; --check fails when README's\n\
progress line is stale.";

pub fn commas(value: i64) -> String {
    let text = value.to_string();
    let (sign, digits) = text
        .strip_prefix('-')
        .map_or(("", text.as_str()), |v| ("-", v));
    let mut out = String::from(sign);
    for (i, c) in digits.chars().enumerate() {
        if i > 0 && (digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(c);
    }
    out
}

/// A share as a percentage to two decimals, rounded down, so 100% is shown
/// only when every byte is done.
pub fn floor_percent(n: i64, d: i64) -> f64 {
    if d == 0 {
        0.0
    } else {
        (n as i128 * 10_000 / d as i128) as f64 / 100.0
    }
}

/// Both games, each in all six editions together, measured at once:
/// ☀️ from `tbs-en` and ⚓️ from `tla-en`, or why each is pending.
pub(crate) fn measure_both(root: &Path) -> Result<[Result<Game, String>; 2], String> {
    let [sun, anchor] = std::thread::scope(|scope| {
        ["tbs-en", "tla-en"]
            .map(|id| {
                scope.spawn(move || {
                    calcrom::measure_game(root, crate::targets::decomp_target(Some(id))?)
                })
            })
            .map(|thread| thread.join().expect("measurement thread"))
    });
    Ok([sun?, anchor?])
}

/// The README status line under "## Progress": ☀️ The Broken Seal and
/// ⚓️ The Lost Age, each DONE in all six of its editions together and
/// pending until six byte-identical builds measure it.
fn status(games: &[Result<Game, String>; 2]) -> String {
    let done = |game: &Result<Game, String>| game.as_ref().ok().map(|g| g.combined().done);
    status_line(done(&games[0]), done(&games[1]))
}

fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    format!("**☀️ {} · ⚓️ {}**", show(sun), show(anchor))
}

fn update_readme(text: &str, status: &str) -> Result<String, String> {
    if text.matches("**☀️ ").count() != 1 {
        return Err("README must contain exactly one \"**☀️ \" progress line".into());
    }
    let start = text.find("**☀️ ").unwrap();
    let end = text[start..]
        .find('\n')
        .map_or(text.len(), |end| start + end);
    Ok(format!("{}{status}{}", &text[..start], &text[end..]))
}

fn run(argv: &[String]) -> Result<String, String> {
    let action = match argv {
        [] => "",
        [flag] if flag == "--write" || flag == "--check" => flag.as_str(),
        [flag] if flag == "-h" || flag == "--help" => return Ok(USAGE.into()),
        _ => {
            return Err(format!(
                "unrecognized arguments: {}\n{USAGE}",
                argv.join(" ")
            ))
        }
    };
    run_at(crate::compiler::routing::root(), action)
}

fn run_at(root: &Path, action: &str) -> Result<String, String> {
    let games = measure_both(root)?;
    let status = status(&games);
    let path = root.join("README.md");
    let readme = std::fs::read_to_string(&path).map_err(|e| format!("README.md: {e}"))?;
    let updated = update_readme(&readme, &status)?;
    match action {
        "--check" if updated != readme => {
            Err("README progress is stale; run: make coverage".into())
        }
        "--check" => Ok(format!("coverage=current {status}")),
        "--write" => {
            std::fs::write(&path, updated).map_err(|e| format!("README.md: {e}"))?;
            let report = decomp::write(root, [("tbs", &games[0]), ("tla", &games[1])])?;
            Ok(format!("published {status} {report}"))
        }
        _ => Ok(status),
    }
}

/// `make land`: both games measured once, README's progress line and the
/// decomp.dev report written from that one count.
pub(crate) fn publish_count(root: &Path) -> Result<String, String> {
    run_at(root, "--write")
}

pub fn entry(arguments: &[String]) {
    match run(arguments) {
        Ok(line) => println!("{line}"),
        Err(error) => {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
    }
}
