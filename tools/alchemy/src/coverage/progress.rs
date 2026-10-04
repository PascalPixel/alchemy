//! `make progress`: DONE from each game's six verified builds together,
//! measured by [`super::calcrom`]: the English build gives the bytes, and
//! every edition earns those of the credited objects its own build links.
use super::calcrom::Game;
use super::{commas, floor_percent, measure_both};

const USAGE: &str = "usage: alchemy check progress\n\
Reports each game's DONE in all six of its editions together, in its parts, then each edition's share.";

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

fn run(argv: &[String]) -> Result<String, String> {
    if !argv.is_empty() {
        return match argv {
            [flag] if flag == "-h" || flag == "--help" => Ok(USAGE.into()),
            _ => Err(format!(
                "unrecognized arguments: {}\n{USAGE}",
                argv.join(" ")
            )),
        };
    }
    let games = measure_both(crate::compiler::routing::root())?;
    let mut lines = Vec::new();
    for ((mark, game), state) in [("☀️", "The Broken Seal"), ("⚓️", "The Lost Age")]
        .iter()
        .zip(&games)
    {
        lines.push(done_line(mark, game, state));
        lines.extend(edition_lines(mark, state));
    }
    Ok(lines.join("\n"))
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
