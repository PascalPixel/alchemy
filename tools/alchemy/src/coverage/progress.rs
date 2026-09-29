//! DONE from each game's verified build, measured by [`super::calcrom`].
use crate::coverage::calcrom::{measure, Measurement};
use crate::coverage::jsnum::{commas, floor_percent};
use crate::coverage::tree::root;
use std::path::Path;

const USAGE: &str = "usage: alchemy check progress [--target tbs-en|tla-en] [--check|--subject|--write-report|--self-test]";

/// The progress report as TSV rows of field and value.
fn report_rows(report: &GameDone, target: &str) -> Vec<(&'static str, String)> {
    let exact = report.common_c + report.game_c;
    let executable = report.executable;
    vec![
        ("target", target.to_string()),
        ("state", "verified".into()),
        ("exact_c_bytes", exact.to_string()),
        (
            "permanent_assembly_bytes",
            (report.common_asm + report.game_asm).to_string(),
        ),
        ("common_asm", report.common_asm.to_string()),
        ("common_c", report.common_c.to_string()),
        ("game_asm", report.game_asm.to_string()),
        ("game_c", report.game_c.to_string()),
        ("done_bytes", report.bytes().to_string()),
        ("executable_bytes", executable.to_string()),
        ("remaining_bytes", (executable - report.bytes()).to_string()),
        ("done_percent", report.percent().to_string()),
        (
            "exact_c_percent",
            floor_percent(exact, executable).to_string(),
        ),
    ]
}

fn pending_rows(target: &str, reason: &str) -> Vec<(&'static str, String)> {
    vec![
        ("target", target.to_string()),
        ("state", "pending".into()),
        ("reason", reason.replace(['\t', '\n'], " ")),
    ]
}

fn table(rows: &[(&'static str, String)]) -> String {
    rows.iter()
        .map(|(field, value)| format!("{field}\t{value}\n"))
        .collect()
}

/// One game's DONE: shared permanent assembly, shared exact C, the game's own
/// permanent assembly (with the compiler library) and the game's own exact C,
/// over every executable byte its verified build links. The Broken Seal is
/// ☀️ and The Lost Age ⚓️.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct GameDone {
    pub common_asm: i64,
    pub common_c: i64,
    pub game_asm: i64,
    pub game_c: i64,
    pub executable: i64,
}
impl GameDone {
    pub fn bytes(&self) -> i64 {
        self.common_asm + self.common_c + self.game_asm + self.game_c
    }
    pub fn percent(&self) -> f64 {
        floor_percent(self.bytes(), self.executable)
    }
}

/// A game's DONE, or `None` while its build is not byte-identical.
pub fn measured(root: &Path, target: &str) -> Result<Option<GameDone>, String> {
    Ok(status(root, target)?
        .ok()
        .map(|measurement| measurement.done))
}

/// A game's measurement, or why it is pending.
fn status(root: &Path, target: &str) -> Result<Result<Measurement, String>, String> {
    measure(root, crate::targets::decomp_target(Some(target))?)
}

/// The commit prefix: both games' DONE percentages, `pending` for a game
/// without a verified build.
pub(crate) fn subject(root: &Path) -> Result<String, String> {
    let percent = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    Ok(format!(
        "☀️ {} ⚓️ {} –",
        percent(measured(root, "tbs-en")?),
        percent(measured(root, "tla-en")?)
    ))
}

/// One game's DONE with its parts and what is not yet C, or why it is pending.
fn done_line(mark: &str, game: &str, status: Result<Measurement, String>) -> String {
    match status {
        Ok(m) => {
            let d = m.done;
            let mut line = format!(
                "{mark} {game} DONE: {} / {} executable bytes ({:.2}%) = common assembly {} + common C {} + game assembly {} (compiler library {}) + game C {}; not yet C: disassembly {} + overlay listings {}",
                commas(d.bytes()), commas(d.executable), d.percent(),
                commas(d.common_asm), commas(d.common_c), commas(d.game_asm), commas(m.library), commas(d.game_c),
                commas(m.raw), commas(m.listings)
            );
            for (object, bytes) in &m.other {
                line.push_str(&format!(" + unclassified {} ({object})", commas(*bytes)));
            }
            line
        }
        Err(reason) => format!("{mark} {game} DONE: {reason}"),
    }
}

fn display(report: &GameDone) -> String {
    format!(
        "DONE: {} / {} executable bytes ({:.2}%)\nExact C: {:.2}%",
        commas(report.bytes()),
        commas(report.executable),
        report.percent(),
        floor_percent(report.common_c + report.game_c, report.executable)
    )
}

fn command(argv: &[String]) -> Result<Option<(String, &str)>, String> {
    let mut target = "tbs-en".to_string();
    let mut action = "";
    let mut index = 0;
    while index < argv.len() {
        match argv[index].as_str() {
            "--target" => {
                index += 1;
                target = match argv.get(index).map(String::as_str) {
                    Some(target @ ("tbs-en" | "tla-en")) => target.into(),
                    Some(other) => {
                        return Err(format!(
                            "unsupported decomp target \"{other}\"; expected tbs-en or tla-en"
                        ))
                    }
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected tbs-en or tla-en".into(),
                        )
                    }
                };
            }
            "-h" | "--help" => return Ok(None),
            flag @ ("--check" | "--subject" | "--write-report" | "--self-test") => {
                if !action.is_empty() {
                    return Err("choose only one progress action".into());
                }
                action = flag;
            }
            other => return Err(format!("unrecognized argument: {other}")),
        }
        index += 1;
    }
    Ok(Some((target, action)))
}

fn run(argv: &[String]) -> Result<String, String> {
    let Some((target, action)) = command(argv)? else {
        return Ok(USAGE.into());
    };
    if action == "--self-test" {
        return Ok("self-test=ok metric=done-executable-byte-share".into());
    }
    let root = root();
    if action == "--subject" {
        return crate::verify::verified_subject(&root);
    }
    if action.is_empty() {
        return Ok(format!(
            "{}\n{}",
            done_line("☀️", "The Broken Seal", status(&root, "tbs-en")?),
            done_line("⚓️", "The Lost Age", status(&root, "tla-en")?)
        ));
    }
    let report = match status(&root, &target)? {
        Ok(measurement) => Ok(measurement.done),
        Err(reason) => Err(reason),
    };
    let rows = match &report {
        Ok(report) => report_rows(report, &target),
        Err(reason) => pending_rows(&target, reason),
    };
    match action {
        "--check" => report.map(|report| display(&report)),
        "--write-report" => {
            let path = root.join("out").join(&target).join("reports/progress.tsv");
            std::fs::create_dir_all(path.parent().unwrap()).map_err(|error| error.to_string())?;
            std::fs::write(&path, table(&rows)).map_err(|error| error.to_string())?;
            let shown = path.strip_prefix(&root).unwrap_or(&path).display();
            Ok(match report {
                Ok(report) => format!(
                    "report={shown} target={target} DONE={:.2}%",
                    report.percent()
                ),
                Err(_) => format!("report={shown} state=pending"),
            })
        }
        _ => unreachable!(),
    }
}

pub fn entry(arguments: &[String]) {
    match run(arguments) {
        Ok(output) => println!("{output}"),
        Err(error) => {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reports_state_done_and_exact_c_shares() {
        let done = GameDone {
            common_c: 20,
            game_c: 10,
            game_asm: 20,
            common_asm: 0,
            executable: 200,
        };
        assert_eq!(done.percent(), 25.0);
        let report = table(&report_rows(&done, "tla-en"));
        assert!(report.contains("done_percent\t25\n"), "{report}");
        assert!(report.contains("exact_c_percent\t15\n"), "{report}");
        assert!(table(&pending_rows("tla-en", "pending")).contains("state\tpending\n"));
    }

    #[test]
    fn a_game_without_a_verified_build_is_pending_in_the_subject() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        assert!(subject(root).is_err());
        std::fs::write(
            root.join("rom.sha1"),
            "0000000000000000000000000000000000000000  out/tbs-en/tbs-en.gba\n\
             0000000000000000000000000000000000000000  out/tla-en/tla-en.gba\n",
        )
        .unwrap();
        assert_eq!(subject(root).unwrap(), "☀️ pending ⚓️ pending –");
        let line = done_line("⚓️", "The Lost Age", status(root, "tla-en").unwrap());
        assert_eq!(
            line,
            "⚓️ The Lost Age DONE: pending a build of out/tla-en/tla-en.gba"
        );
    }
}
