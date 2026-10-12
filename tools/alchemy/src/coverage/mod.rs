pub(crate) mod arm;
pub(crate) mod boxtree;
pub(crate) mod calcrom;
pub(crate) mod data;
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

use crate::coverage::calcrom::Game;
use crate::coverage::progress::GameDone;
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

pub fn floor_percent(n: i64, d: i64) -> f64 {
    if d == 0 {
        0.0
    } else {
        (n as i128 * 10_000 / d as i128) as f64 / 100.0
    }
}

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

fn status(games: &[Result<Game, String>; 2]) -> String {
    let (sun, anchor) = done(games);
    status_line(sun, anchor)
}
fn done(games: &[Result<Game, String>; 2]) -> (Option<GameDone>, Option<GameDone>) {
    let done = |game: &Result<Game, String>| game.as_ref().ok().map(|g| g.combined().done);
    (done(&games[0]), done(&games[1]))
}
fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| {
            format!("code {:.2}% data {:.2}%", d.percent(), d.data_percent())
        })
    };
    format!("☀️ {} · ⚓️ {}", show(sun), show(anchor))
}
const MEASUREMENT_HEADER: &str =
    "game\tcommon_asm\tcommon_c\tgame_asm\tgame_c\texecutable\tveneers\tdata\tdata_total";
/// A measurement from before DONE data (Pascal, 2026-10-08) carries code alone.
const CODE_ONLY_HEADER: &str = "game\tcommon_asm\tcommon_c\tgame_asm\tgame_c\texecutable\tveneers";
fn measurement_text(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let mut text = format!("{MEASUREMENT_HEADER}\n");
    for (game, done) in [("tbs", sun), ("tla", anchor)] {
        if let Some(d) = done {
            text.push_str(&format!(
                "{game}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\n",
                d.common_asm,
                d.common_c,
                d.game_asm,
                d.game_c,
                d.executable,
                d.veneers,
                d.data,
                d.data_total
            ));
        }
    }
    text
}
fn parse_measurement(text: &str) -> Result<[Option<GameDone>; 2], String> {
    let mut lines = text.lines();
    let width = match lines.next() {
        Some(MEASUREMENT_HEADER) => 8,
        Some(CODE_ONLY_HEADER) => 6,
        _ => return Err("measurement has an unexpected header".into()),
    };
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
        if values.len() != width {
            return Err(format!("measurement row has the wrong width: {line}"));
        }
        let (data, data_total) = (
            values.get(6).copied().unwrap_or_default(),
            values.get(7).copied().unwrap_or_default(),
        );
        if data < 0 || data > data_total {
            return Err(format!("measurement row has impossible data: {line}"));
        }
        if games[at].is_some() {
            return Err(format!("measurement names {} twice", fields[0]));
        }
        games[at] = Some(GameDone {
            common_asm: values[0],
            common_c: values[1],
            game_asm: values[2],
            game_c: values[3],
            executable: values[4],
            veneers: values[5],
            data,
            data_total,
        });
    }
    Ok(games)
}
fn render_figures(
    root: &Path,
    history: &history::History,
) -> Result<(raster::Canvas, raster::Canvas), String> {
    let letters = letters::Letters::face();
    let chart = figure::chart(&letters, &history::as_drawn(history));
    Ok((chart, figure::map(&letters, root)))
}
fn publish_figures(root: &Path, measurement: &Path, directory: &Path) -> Result<String, String> {
    let [sun, anchor] = parse_measurement(&read(measurement)?)?;
    let path = directory.join("history.tsv");
    let mut history = history::load_file(&path)?;
    // What main measured at its landings since the last publication.
    if let Ok(carried) = history::load(root) {
        history::take_over(&mut history, &carried);
    }
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
    let root = crate::compiler::routing::root();
    match argv {
        [flag] if flag == "-h" || flag == "--help" => Ok(USAGE.into()),
        [flag] if flag == "--self-test" => Ok("self-test=ok coverage".into()),
        [flag, measurement, directory] if flag == "--publish" => {
            publish_figures(root, Path::new(measurement), Path::new(directory))
        }
        [] => Ok(status(&measure_both(root)?)),
        [flag] if flag == "--report" => report(root),
        _ => Err(format!(
            "unrecognized arguments: {}\n{USAGE}",
            argv.join(" ")
        )),
    }
}

pub(crate) fn report(root: &Path) -> Result<String, String> {
    let games = measure_both(root)?;
    let (sun, anchor) = done(&games);
    let decomp = decomp::write(root, [("tbs", &games[0]), ("tla", &games[1])])?;
    let path = root.join(publish::MEASUREMENT);
    std::fs::create_dir_all(path.parent().expect("measurement directory"))
        .map_err(|e| format!("{}: {e}", publish::MEASUREMENT))?;
    write(&path, &measurement_text(sun, anchor))?;
    Ok(format!(
        "measured {} {decomp} measurement={}",
        status_line(sun, anchor),
        publish::MEASUREMENT
    ))
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
            data: 992,
            data_total: 1000,
            ..GameDone::default()
        };
        assert_eq!(
            status_line(Some(sun), None),
            "☀️ code 59.00% data 99.20% · ⚓️ pending"
        );
        let anchor = GameDone {
            game_c: 10,
            game_asm: 50,
            veneers: 30,
            executable: 1000,
            ..GameDone::default()
        };
        assert_eq!(
            status_line(Some(sun), Some(anchor)),
            "☀️ code 59.00% data 99.20% · ⚓️ code 6.00% data 0.00%"
        );
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
            data: 7,
            data_total: 9,
        };
        let text = measurement_text(Some(sun), None);
        assert_eq!(parse_measurement(&text).unwrap(), [Some(sun), None]);
        assert!(parse_measurement("game\n").is_err());
        assert!(parse_measurement(&format!("{text}tbs\t1\t2\t3\t4\t100\t1\t0\t0\n")).is_err());
        assert!(parse_measurement(&format!("{text}tla\t1\t2\n")).is_err());
        assert!(parse_measurement(&text.replace("\t7\t9", "\t9\t7")).is_err());
        // A code-only measurement from before DONE data still reads.
        let old = "game\tcommon_asm\tcommon_c\tgame_asm\tgame_c\texecutable\tveneers\n\
                   tla\t1\t2\t3\t4\t100\t1\n";
        let [None, Some(anchor)] = parse_measurement(old).unwrap() else {
            panic!("a code-only measurement reads")
        };
        assert_eq!((anchor.bytes(), anchor.data_total), (10, 0));
    }
}
