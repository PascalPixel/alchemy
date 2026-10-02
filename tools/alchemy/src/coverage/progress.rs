//! DONE from each game's six verified builds together, measured by
//! [`super::calcrom`]: the English build gives the bytes, and every edition
//! earns those of the credited objects its own build links.
use crate::coverage::calcrom::{measure_game, Game};
use crate::coverage::jsnum::{commas, floor_percent};
use crate::coverage::tree::root;
use std::path::Path;

const USAGE: &str = "usage: alchemy check progress [--target tbs-en|tla-en] [--check|--subject|--write-report|--write-decomp-report|--self-test]\n\
Reports each game's DONE in all six of its editions together, then each edition's share.\n\
--target names a game by its English build, which supplies the code byte weights.\n\
--write-decomp-report builds and compares all twelve editions, then exports protobuf reports.\n\
The combined report exposes twelve versions named by game and flag, with no aggregate versions.\n\
Individual tbs-ja through tla-it reports keep the build IDs for local inspection.\n\
Code and initialized data use English byte weights in all twelve versions.\n\
Data's denominator is the English .data, .rodata and unidentified section inventory.\n\
An edition earns a source section's English weight only when its verified build links that\n\
object and input section in the same image; localized sizes may differ. Missing sections,\n\
unbuilt overlays and scaffolding earn nothing. BSS, packed code overlays and data found only\n\
in another edition are excluded. Data measures section coverage, not whole-asset completion\n\
or the edition's native byte total; the report category names the English weighting.\n\
Treemap units follow placed map objects, split into C, FAKEMATCH, assembly and pending portions.\n\
Existing source files have links; global padding deductions keep explicitly labeled accounting units.\n\
Upload only out/reports/decomp/combined_report/report.pb as artifact combined_report.\n\
decomp.dev splits it into the twelve named versions; choose The Broken Seal 🇯🇵 as the default.\n\
The artifact contains only report.pb, with counts and no private inputs, game bytes or function claims.";

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

/// Each edition's own DONE bytes, after the game's rows.
fn edition_rows(game: &Game) -> Vec<(&'static str, String)> {
    let mut rows = vec![(
        "english_executable_bytes",
        game.english.done.executable.to_string(),
    )];
    for (language, edition) in &game.editions {
        let field = match *language {
            "ja" => "ja_done_bytes",
            "en" => "en_done_bytes",
            "de" => "de_done_bytes",
            "es" => "es_done_bytes",
            "fr" => "fr_done_bytes",
            _ => "it_done_bytes",
        };
        rows.push((field, edition.done.bytes().to_string()));
    }
    rows
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

/// DONE bytes: shared permanent assembly, shared exact C, the game's own
/// permanent assembly (with the compiler library) and the game's own exact C,
/// out of executable bytes. A game's published DONE adds up its six editions:
/// each edition's bytes are the English build's bytes of the credited objects
/// that edition links, and `executable` is six times the English build's
/// executable bytes. The Broken Seal is ☀️ and The Lost Age ⚓️.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct GameDone {
    pub common_asm: i64,
    pub common_c: i64,
    pub game_asm: i64,
    pub game_c: i64,
    pub executable: i64,
    /// Whole 8-byte far-call stubs (veneers), counted within the assembly.
    pub veneers: i64,
}
impl GameDone {
    /// DONE in its parts, in percentage points of the executable bytes: C,
    /// assembly without the stubs, and the 8-byte stubs.
    pub fn parts(&self) -> (f64, f64, f64) {
        (
            floor_percent(self.common_c + self.game_c, self.executable),
            floor_percent(
                self.common_asm + self.game_asm - self.veneers,
                self.executable,
            ),
            floor_percent(self.veneers, self.executable),
        )
    }
    pub fn bytes(&self) -> i64 {
        self.common_asm + self.common_c + self.game_asm + self.game_c
    }
    pub fn percent(&self) -> f64 {
        floor_percent(self.bytes(), self.executable)
    }
}
impl std::ops::AddAssign for GameDone {
    fn add_assign(&mut self, other: GameDone) {
        self.common_asm += other.common_asm;
        self.common_c += other.common_c;
        self.game_asm += other.game_asm;
        self.game_c += other.game_c;
        self.executable += other.executable;
        self.veneers += other.veneers;
    }
}

/// A game's DONE in all six editions together, or `None` while any of its
/// six builds is not byte-identical and current. `target` is the game's
/// English build.
pub fn measured(root: &Path, target: &str) -> Result<Option<GameDone>, String> {
    Ok(status(root, target)?.ok().map(|game| game.combined().done))
}

/// A game's measurement, or why it is pending.
fn status(root: &Path, target: &str) -> Result<Result<Game, String>, String> {
    measure_game(root, crate::targets::decomp_target(Some(target))?)
}

/// The commit prefix: both games' DONE percentages, `pending` for a game
/// without six verified builds.
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

/// One game's DONE in all six editions with its parts and what the English
/// build has not yet in C, or why it is pending.
fn done_line(mark: &str, game: &str, status: &Result<Game, String>) -> String {
    match status {
        Ok(measured) => {
            let all = measured.combined();
            let (d, english) = (all.done, &measured.english);
            let (c, assembly, stubs) = d.parts();
            let mut line = format!(
                "{mark} {game} DONE: {} / {} executable bytes in six editions ({:.2}%) = C {} ({c:.2} points) + assembly {} ({assembly:.2}, compiler library {} of it) + 8-byte stubs {} ({stubs:.2}); FAKEMATCH-steered C {} ({:.2} points, removed last); uncredited padding {} not counted; not yet C in the English build: disassembly {} + overlay listings {}",
                commas(d.bytes()), commas(d.executable), d.percent(),
                commas(d.common_c + d.game_c), commas(d.common_asm + d.game_asm - d.veneers), commas(all.library), commas(d.veneers),
                commas(all.steered), floor_percent(all.steered, d.executable), commas(all.uncredited),
                commas(english.raw), commas(english.listings)
            );
            for (object, bytes) in &english.other {
                line.push_str(&format!(" + unclassified {} ({object})", commas(*bytes)));
            }
            line
        }
        Err(reason) => format!("{mark} {game} DONE: {reason}"),
    }
}

/// One short line per edition: its own share of the English build's
/// executable bytes, so a lagging edition shows.
fn edition_lines(mark: &str, status: &Result<Game, String>) -> Vec<String> {
    let Ok(game) = status else {
        return Vec::new();
    };
    game.editions
        .iter()
        .map(|(language, edition)| {
            let d = edition.done;
            format!(
                "{mark}   {language} {:.2}% ({} / {})",
                d.percent(),
                commas(d.bytes()),
                commas(d.executable)
            )
        })
        .collect()
}

/// One game's data and name coverage in its English build, as pret's calcrom
/// reports beside code.
fn data_line(mark: &str, game: &str, status: &Result<Game, String>) -> Option<String> {
    let m = &status.as_ref().ok()?.english;
    let data = m.data_source + m.data_scaffold;
    let n = m.names;
    let share = |part: i64, whole: i64| {
        if whole == 0 {
            0.0
        } else {
            100.0 * part as f64 / whole as f64
        }
    };
    Some(format!(
        "{mark} {game} data: {} / {} bytes from source ({:.2}%), scaffold {}; names: {} of {} documented ({:.2}%), {} address-only, {} with an address",
        commas(m.data_source), commas(data), share(m.data_source, data), commas(m.data_scaffold),
        commas(n.documented()), commas(n.total), share(n.documented(), n.total),
        commas(n.undocumented), commas(n.partial)
    ))
}

fn display(report: &GameDone) -> String {
    format!(
        "DONE: {} / {} executable bytes in six editions ({:.2}%)\nExact C: {:.2}%",
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
            flag @ ("--check"
            | "--subject"
            | "--write-report"
            | "--write-decomp-report"
            | "--self-test") => {
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
        return Ok("self-test=ok metric=done-executable-byte-share-in-six-editions".into());
    }
    let root = root();
    if action == "--write-decomp-report" {
        return super::decomp::write(&root);
    }
    if action == "--subject" {
        return crate::verify::verified_subject(&root);
    }
    if action.is_empty() {
        let mut lines = Vec::new();
        for (mark, game, target) in [
            ("☀️", "The Broken Seal", "tbs-en"),
            ("⚓️", "The Lost Age", "tla-en"),
        ] {
            let state = status(&root, target)?;
            lines.push(done_line(mark, game, &state));
            lines.extend(edition_lines(mark, &state));
            lines.extend(data_line(mark, game, &state));
        }
        return Ok(lines.join("\n"));
    }
    let state = status(&root, &target)?;
    let report = state.as_ref().map(|game| game.combined().done);
    let rows = match &state {
        Ok(game) => {
            let mut rows = report_rows(&game.combined().done, &target);
            rows.extend(edition_rows(game));
            rows
        }
        Err(reason) => pending_rows(&target, reason),
    };
    match action {
        "--check" => report
            .map(|report| display(&report))
            .map_err(|reason| reason.clone()),
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
    use crate::coverage::calcrom::{Counted, Measurement};

    #[test]
    fn reports_state_done_and_exact_c_shares() {
        let done = GameDone {
            common_c: 20,
            game_c: 10,
            game_asm: 20,
            common_asm: 0,
            executable: 200,
            veneers: 0,
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
        let state = status(root, "tla-en").unwrap();
        assert_eq!(
            done_line("⚓️", "The Lost Age", &state),
            "⚓️ The Lost Age DONE: pending a build of out/tla-en/tla-en.gba"
        );
        assert!(edition_lines("⚓️", &state).is_empty());
    }

    #[test]
    fn the_report_shows_the_six_editions_together_and_each_edition_alone() {
        // English credits 600 of 1,000 bytes; the other five link half of it.
        let edition = |c, steered| Counted {
            done: GameDone {
                game_c: c,
                executable: 1000,
                ..GameDone::default()
            },
            steered,
            ..Counted::default()
        };
        let game = Game {
            edition_credits: std::collections::BTreeMap::new(),
            edition_data: std::collections::BTreeMap::new(),
            english: Measurement {
                raw: 300,
                listings: 100,
                ..Measurement::default()
            },
            editions: ["ja", "en", "de", "es", "fr", "it"]
                .into_iter()
                .map(|language| {
                    if language == "en" {
                        (language, edition(600, 60))
                    } else {
                        (language, edition(300, 30))
                    }
                })
                .collect(),
        };
        let all = game.combined();
        assert_eq!((all.done.bytes(), all.done.executable), (2100, 6000));
        assert_eq!((all.done.percent(), all.steered), (35.0, 210));
        let state = Ok(game);
        let line = done_line("☀️", "The Broken Seal", &state);
        assert!(
            line.starts_with(
                "☀️ The Broken Seal DONE: 2,100 / 6,000 executable bytes in six editions (35.00%) = C 2,100 (35.00 points)"
            ),
            "{line}"
        );
        assert!(
            line.ends_with(
                "not yet C in the English build: disassembly 300 + overlay listings 100"
            ),
            "{line}"
        );
        assert_eq!(
            edition_lines("☀️", &state),
            [
                "☀️   ja 30.00% (300 / 1,000)",
                "☀️   en 60.00% (600 / 1,000)",
                "☀️   de 30.00% (300 / 1,000)",
                "☀️   es 30.00% (300 / 1,000)",
                "☀️   fr 30.00% (300 / 1,000)",
                "☀️   it 30.00% (300 / 1,000)",
            ]
        );
        let rows = table(&edition_rows(state.as_ref().unwrap()));
        assert!(rows.contains("ja_done_bytes\t300\n") && rows.contains("en_done_bytes\t600\n"));
    }
}
