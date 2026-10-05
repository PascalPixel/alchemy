pub(crate) mod boxtree;
pub(crate) mod calcrom;
pub(crate) mod decomp;
pub(crate) mod figure;
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

use crate::coverage::progress::GameDone;
use crate::coverage::tree::root;
use std::path::Path;
const USAGE: &str =
    "usage: alchemy check coverage [--report|--publish MEASUREMENT DIRECTORY|--self-test]\n\
Measures both games, each from its six verified builds together (make compare-editions); a game\n\
without all six stays pending. --report writes that measurement and the decomp.dev report under\n\
out/reports/decomp for make land and the push that follows. --publish is CI's: it records a\n\
pushed measurement in DIRECTORY's history.tsv and draws both progress figures there.";
fn read(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|b| String::from_utf8_lossy(&b).into_owned())
        .map_err(|e| format!("cannot read {}: {e}", path.display()))
}
fn write(path: &Path, text: &str) -> Result<(), String> {
    std::fs::write(path, text).map_err(|e| format!("cannot write {}: {e}", path.display()))
}
/// ☀️ The Broken Seal and ⚓️ The Lost Age, each DONE in all six of its
/// editions together and pending until six byte-identical builds measure it.
fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    format!("☀️ {} · ⚓️ {}", show(sun), show(anchor))
}
const MEASUREMENT_HEADER: &str =
    "game\tcommon_asm\tcommon_c\tgame_asm\tgame_c\texecutable\tveneers";
/// The measurement a push carries to CI: each verified game's DONE parts.
fn measurement_text(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let mut text = format!("{MEASUREMENT_HEADER}\n");
    for (game, done) in [("tbs", sun), ("tla", anchor)] {
        if let Some(d) = done {
            text.push_str(&format!(
                "{game}\t{}\t{}\t{}\t{}\t{}\t{}\n",
                d.common_asm, d.common_c, d.game_asm, d.game_c, d.executable, d.veneers
            ));
        }
    }
    text
}
fn parse_measurement(text: &str) -> Result<[Option<GameDone>; 2], String> {
    let mut lines = text.lines();
    if lines.next() != Some(MEASUREMENT_HEADER) {
        return Err("measurement has an unexpected header".into());
    }
    let mut games = [None, None];
    for line in lines {
        let fields = line.split('\t').collect::<Vec<_>>();
        let at = match fields.first() {
            Some(&"tbs") => 0,
            Some(&"tla") => 1,
            _ => return Err(format!("measurement row names no game: {line}")),
        };
        let values = fields[1..]
            .iter()
            .map(|field| field.parse::<i64>())
            .collect::<Result<Vec<_>, _>>()
            .map_err(|_| format!("measurement row is not numeric: {line}"))?;
        let [common_asm, common_c, game_asm, game_c, executable, veneers] = values[..] else {
            return Err(format!("measurement row has the wrong width: {line}"));
        };
        if games[at].is_some() {
            return Err(format!("measurement names {} twice", fields[0]));
        }
        games[at] = Some(GameDone {
            common_asm,
            common_c,
            game_asm,
            game_c,
            executable,
            veneers,
        });
    }
    Ok(games)
}
#[cfg(test)]
mod tests {
    use super::{measurement_text, parse_measurement, status_line};
    use crate::coverage::progress::GameDone;

    #[test]
    fn status_shows_each_verified_game_or_pending() {
        let sun = GameDone {
            game_c: 250,
            game_asm: 340,
            executable: 1000,
            ..GameDone::default()
        };
        assert_eq!(status_line(Some(sun), None), "☀️ 59.00% · ⚓️ pending");
        let anchor = GameDone {
            game_c: 10,
            game_asm: 50,
            veneers: 30,
            executable: 1000,
            ..GameDone::default()
        };
        assert_eq!(status_line(Some(sun), Some(anchor)), "☀️ 59.00% · ⚓️ 6.00%");
    }

    #[test]
    fn a_measurement_survives_its_trip_to_ci() {
        let sun = GameDone {
            common_asm: 1,
            common_c: 2,
            game_asm: 3,
            game_c: 4,
            executable: 100,
            veneers: 1,
        };
        let text = measurement_text(Some(sun), None);
        assert_eq!(parse_measurement(&text).unwrap(), [Some(sun), None]);
        assert!(parse_measurement("game\n").is_err());
        assert!(parse_measurement(&format!("{text}tbs\t1\t2\t3\t4\t100\t1\n")).is_err());
        assert!(parse_measurement(&format!("{text}tla\t1\t2\n")).is_err());
    }
}
/// Both figures as the history's recorded figure date draws them; the map
/// draws the files `root` tracks.
fn render_figures(
    root: &Path,
    history: &history::History,
) -> Result<(raster::Canvas, raster::Canvas), String> {
    let letters = letters::Letters::face();
    let chart = figure::chart(&letters, &history::as_drawn(history));
    Ok((chart, figure::map(&letters, root)))
}
/// CI: record a pushed measurement in `directory`'s history, count every
/// day after the frozen model table from main's commits, and draw both
/// figures beside it.
fn publish_figures(root: &Path, measurement: &Path, directory: &Path) -> Result<String, String> {
    let [sun, anchor] = parse_measurement(&read(measurement)?)?;
    let path = directory.join("history.tsv");
    let mut history = history::load_file(&path)?;
    let hour = history::this_hour();
    let today = hour[..10].to_string();
    history::record(&mut history, &today, sun, anchor);
    history::record_hour(&mut history, &hour, sun, anchor);
    history::derive_models(root, &mut history)?;
    history::mark_drawn(&mut history, &today);
    write(&path, &history::text(&history))?;
    let (chart, map) = render_figures(root, &history)?;
    let scale = letters::FIGURE_SCALE;
    for (name, canvas) in [(figure::CHART, chart), (figure::MAP, map)] {
        std::fs::write(directory.join(name), canvas.png(scale, &today)?)
            .map_err(|e| format!("{name}: {e}"))?;
    }
    Ok(format!(
        "published {} figures={},{}",
        status_line(sun, anchor),
        figure::CHART,
        figure::MAP
    ))
}
fn run(argv: &[String]) -> Result<String, String> {
    let root = root();
    match argv {
        [flag] if flag == "-h" || flag == "--help" => Ok(USAGE.into()),
        [flag] if flag == "--self-test" => Ok("self-test=ok coverage".into()),
        [flag, measurement, directory] if flag == "--publish" => {
            publish_figures(&root, Path::new(measurement), Path::new(directory))
        }
        [] | [_] => {
            let report = match argv {
                [] => false,
                [flag] if flag == "--report" => true,
                _ => {
                    return Err(format!(
                        "unrecognized arguments: {}\n{USAGE}",
                        argv.join(" ")
                    ))
                }
            };
            let games = [
                progress::status(&root, "tbs-en")?,
                progress::status(&root, "tla-en")?,
            ];
            let done = |game: &Result<calcrom::Game, String>| {
                game.as_ref().ok().map(|game| game.combined().done)
            };
            let (sun, anchor) = (done(&games[0]), done(&games[1]));
            let status = status_line(sun, anchor);
            if !report {
                return Ok(status);
            }
            let decomp = decomp::write(&root, [("tbs", &games[0]), ("tla", &games[1])])?;
            let path = root.join(publish::MEASUREMENT);
            std::fs::create_dir_all(path.parent().expect("measurement directory"))
                .map_err(|e| format!("{}: {e}", publish::MEASUREMENT))?;
            write(&path, &measurement_text(sun, anchor))?;
            Ok(format!(
                "measured {status} {decomp} measurement={}",
                publish::MEASUREMENT
            ))
        }
        _ => Err(format!(
            "unrecognized arguments: {}\n{USAGE}",
            argv.join(" ")
        )),
    }
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
