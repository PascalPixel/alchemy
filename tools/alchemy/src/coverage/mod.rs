pub(crate) mod boxtree;
pub(crate) mod calcrom;
pub(crate) mod figure;
pub(crate) mod history;
pub(crate) mod jsnum;
pub(crate) mod letters;
pub(crate) mod model;
pub(crate) mod palette;
pub(crate) mod progress;
pub(crate) mod raster;
pub(crate) mod sessions;
pub(crate) mod tree;

use crate::coverage::progress::{measured, GameDone};
use crate::coverage::tree::root;
use std::path::Path;
const USAGE: &str =
    "usage: alchemy check coverage [--write [--publication]|--check|--models|--self-test]\n\
Publishes README's progress line, today's progress history row and both figures from each game's\n\
verified build (make compare); a game without one stays pending. --publication preserves approved\n\
model attribution; --check fails when any published value is stale.";
fn read(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|b| String::from_utf8_lossy(&b).into_owned())
        .map_err(|e| format!("cannot read {}: {e}", path.display()))
}
fn write(path: &Path, text: &str) -> Result<(), String> {
    std::fs::write(path, text).map_err(|e| format!("cannot write {}: {e}", path.display()))
}
#[derive(Default)]
struct Options {
    write: bool,
    check: bool,
    self_test: bool,
    models: bool,
    publication: bool,
    help: bool,
}
fn parse(argv: &[String]) -> Result<Options, String> {
    let mut o = Options::default();
    for argument in argv {
        match argument.as_str() {
            "--write" => o.write = true,
            "--check" => o.check = true,
            "--self-test" => o.self_test = true,
            "--models" => o.models = true,
            "--publication" => o.publication = true,
            "-h" | "--help" => {
                o.help = true;
                break;
            }
            other => return Err(format!("unrecognized argument: {other}")),
        }
    }
    Ok(o)
}
/// The README status line under "## Progress": ☀️ The Broken Seal and
/// ⚓️ The Lost Age, each pending until a byte-identical build measures it.
/// ⚓️ is always shown in its parts: C, assembly and 8-byte stubs.
fn status_line(sun: Option<GameDone>, anchor: Option<GameDone>) -> String {
    let show = |done: Option<GameDone>| {
        done.map_or("pending".to_string(), |d| format!("{:.2}%", d.percent()))
    };
    let parts = anchor.map_or(String::new(), |d| {
        let (c, assembly, stubs) = d.parts();
        format!(" (C {c:.2} + assembly {assembly:.2} + stubs {stubs:.2})")
    });
    format!("**☀️ {} · ⚓️ {}{parts}**", show(sun), show(anchor))
}
fn update_readme(text: &str, status: &str) -> String {
    let mut out = text.to_string();
    if let Some(start) = out.find("**☀️ ") {
        if let Some(end) = out[start..].find('\n') {
            out.replace_range(start..start + end, status);
        }
    }
    out
}
#[cfg(test)]
mod tests {
    use super::{status_line, update_readme};
    use crate::coverage::progress::GameDone;
    #[test]
    #[ignore = "slow: writes and compresses both figures twice"]
    fn figures_are_redrawn_with_each_count_and_match_the_readme() {
        use super::{check_figures, figure, figure_date_current, history, write_figures};
        let root = tempfile::tempdir().unwrap();
        let root = root.path();
        std::fs::create_dir_all(history::path(root).parent().unwrap()).unwrap();
        std::fs::write(
            history::path(root),
            history::text(&history::History {
                began: "2026-07-16".into(),
                days: vec![history::Day {
                    tbs: Some(history::Measure::published(1.0)),
                    ..history::Day::new("2026-07-16")
                }],
                ..history::History::default()
            }),
        )
        .unwrap();
        assert!(std::process::Command::new("git")
            .args(["init", "--quiet"])
            .current_dir(root)
            .status()
            .unwrap()
            .success());
        let done = |bytes| GameDone {
            game_c: bytes,
            executable: 1000,
            ..GameDone::default()
        };
        write_figures(root, Some(done(600)), None, false).unwrap();
        let chart = std::fs::read(root.join(figure::CHART)).unwrap();
        let map = std::fs::read(root.join(figure::MAP)).unwrap();
        check_figures(root).unwrap();
        // A later count the same day redraws the chart with it.
        write_figures(root, Some(done(610)), None, false).unwrap();
        assert_ne!(std::fs::read(root.join(figure::CHART)).unwrap(), chart);
        let _ = map;
        let recorded = history::load(root).unwrap();
        let today = history::today();
        let row = recorded.days.last().unwrap().clone();
        let tbs = |row: &history::Day| row.tbs.as_ref().and_then(history::Measure::percent);
        assert_eq!((row.date.as_str(), tbs(&row)), (today.as_str(), Some(61.0)));
        assert_eq!(tbs(recorded.figures.as_ref().unwrap()), Some(61.0));
        check_figures(root).unwrap();
        // A README stating another number fails.
        std::fs::write(root.join("README.md"), "**☀️ 60.00% · ⚓️ pending**\n").unwrap();
        assert!(check_figures(root).is_err());
        std::fs::write(root.join("README.md"), "**☀️ 61.00% · ⚓️ pending**\n").unwrap();
        check_figures(root).unwrap();
        // A tampered chart fails; yesterday's figures pass only until today has a row.
        std::fs::write(root.join(figure::CHART), &map).unwrap();
        assert!(check_figures(root).is_err());
        let yesterday = history::previous(&today).unwrap();
        assert!(figure_date_current(&today, &today, true));
        assert!(figure_date_current(&yesterday, &today, false));
        assert!(!figure_date_current(&yesterday, &today, true));
        assert!(!figure_date_current(
            &history::previous(&yesterday).unwrap(),
            &today,
            false
        ));
    }

    #[test]
    fn readme_status_shows_each_verified_game_or_pending() {
        let sun = GameDone {
            game_c: 250,
            game_asm: 340,
            executable: 1000,
            ..GameDone::default()
        };
        let status = status_line(Some(sun), None);
        assert_eq!(status, "**☀️ 59.00% · ⚓️ pending**");
        let anchor = GameDone {
            game_c: 10,
            game_asm: 50,
            veneers: 30,
            executable: 1000,
            ..GameDone::default()
        };
        assert_eq!(
            status_line(Some(sun), Some(anchor)),
            "**☀️ 59.00% · ⚓️ 6.00% (C 1.00 + assembly 2.00 + stubs 3.00)**"
        );
        let updated = update_readme(
            "# Alchemy\n\n## Progress\n\n**☀️ 52% · ⚓️ 1%**\n\nDetails\n",
            &status,
        );
        assert_eq!(
            updated,
            "# Alchemy\n\n## Progress\n\n**☀️ 59.00% · ⚓️ pending**\n\nDetails\n"
        );
    }
}
/// Both README figures as the history's recorded figure date draws them.
fn render_figures(
    root: &Path,
    history: &history::History,
) -> Result<(raster::Canvas, raster::Canvas), String> {
    let letters = letters::Letters::face();
    let chart = figure::chart(&letters, &history::as_drawn(history));
    Ok((chart, figure::map(&letters, root)))
}
/// Record today's verified counts and redraw both figures, so the chart
/// always shows the numbers the README states.
fn write_figures(
    root: &Path,
    sun: Option<GameDone>,
    anchor: Option<GameDone>,
    publication: bool,
) -> Result<(), String> {
    let today = history::today();
    let mut history = history::load(root)?;
    history::record(&mut history, &today, sun, anchor);
    let models = if publication {
        history::publication_models(root, &history, &today)?
    } else {
        history::models_on(root, &today)?
    };
    history::record_models(&mut history, &today, &models);
    history::mark_drawn(&mut history, &today);
    write(&history::path(root), &history::text(&history))?;
    let (chart, map) = render_figures(root, &history)?;
    let scale = letters::FIGURE_SCALE;
    let (chart, map) = (chart.png(scale, &today)?, map.png(scale, &today)?);
    std::fs::write(root.join(figure::CHART), chart)
        .map_err(|e| format!("{}: {e}", figure::CHART))?;
    std::fs::write(root.join(figure::MAP), map).map_err(|e| format!("{}: {e}", figure::MAP))
}
/// The committed figures are current when they carry the history's figure
/// date, that date is today (or yesterday while today has no row), they show
/// the latest recorded row and the README's progress line, the chart
/// is exactly what that day's rows draw, and the map is exactly what the
/// tracked files draw unless they changed since it was drawn that day.
fn check_figures(root: &Path) -> Result<(), String> {
    let stale = |why: &str| {
        Err(format!(
            "README figures are stale ({why}); run: make coverage"
        ))
    };
    let history = history::load(root)?;
    let figures = history.figures.clone().unwrap_or_default();
    let date = figures.date.clone();
    let today = history::today();
    let has_today = history.days.iter().any(|row| row.date == today);
    if !figure_date_current(&date, &today, has_today) {
        return stale(&format!("drawn on {date:?}"));
    }
    let latest = history.days.last();
    let percent = |row: &history::Day, game| row.game(game).and_then(history::Measure::percent);
    for game in ["tbs", "tla"] {
        let shown = percent(&figures, game);
        if latest.map(|row| percent(row, game)) != Some(shown) {
            return stale(&format!("{game} is not the latest recorded row"));
        }
    }
    let readme = std::fs::read_to_string(root.join("README.md")).unwrap_or_default();
    for (icon, game) in [("☀️", "tbs"), ("⚓️", "tla")] {
        if history.pending(game) {
            if readme.contains("**☀️ ") && !readme.contains(&format!("{icon} pending")) {
                return stale(&format!("{game} has no verified current measurement"));
            }
            continue;
        }
        if let Some(shown) = percent(&figures, game) {
            // The README floors to hundredths, as the chart labels do.
            let shown = (shown * 100.0 + 1e-9).floor() / 100.0;
            if readme.contains("**☀️ ") && !readme.contains(&format!("{icon} {shown:.2}%")) {
                return stale(&format!("{game} {shown:.2}% is not the README's progress"));
            }
        }
    }
    let chart =
        std::fs::read(root.join(figure::CHART)).map_err(|e| format!("{}: {e}", figure::CHART))?;
    let map = std::fs::read(root.join(figure::MAP)).map_err(|e| format!("{}: {e}", figure::MAP))?;
    for (name, png) in [(figure::CHART, &chart), (figure::MAP, &map)] {
        if raster::png_date(png).as_deref() != Some(date.as_str()) {
            return stale(&format!("{name} does not carry {date}"));
        }
    }
    // Compared by decoded pixels, so a check never has to deflate again.
    let (expected_chart, expected_map) = render_figures(root, &history)?;
    let scale = letters::FIGURE_SCALE;
    let drawn = |canvas: &raster::Canvas| {
        Some((
            canvas.width as u32 * scale,
            canvas.height as u32 * scale,
            canvas.rgba(scale),
        ))
    };
    if raster::decode(&chart) != drawn(&expected_chart) {
        return stale(&format!("{} differs from its rows", figure::CHART));
    }
    if raster::decode(&map) != drawn(&expected_map) {
        return stale(&format!("{} differs from the tracked files", figure::MAP));
    }
    Ok(())
}
fn figure_date_current(date: &str, today: &str, has_today: bool) -> bool {
    date == today || (!has_today && history::previous(today).as_deref() == Some(date))
}
fn run(argv: &[String]) -> Result<String, String> {
    let o = parse(argv)?;
    if o.help {
        return Ok(USAGE.into());
    }
    if o.self_test {
        return Ok("self-test=ok coverage".into());
    }
    if o.publication && (!o.write || o.check || o.models) {
        return Err("--publication requires --write; it cannot relabel models".into());
    }
    let root = root();
    if o.models {
        // Relabel every day's commits by model from their trailers and authors.
        let mut history = history::load(&root)?;
        let moved = history::relabel_models(&root, &mut history)?;
        write(&history::path(&root), &history::text(&history))?;
        return Ok(moved
            .iter()
            .map(|((from, to), n)| format!("{n}\t{from} -> {to}"))
            .collect::<Vec<_>>()
            .join("\n"));
    }
    let (sun, anchor) = (measured(&root, "tbs-en")?, measured(&root, "tla-en")?);
    let status = status_line(sun, anchor);
    let readme = read(&root.join("README.md"))?;
    let updated = update_readme(&readme, &status);
    if o.check {
        if updated != readme {
            return Err("README progress is stale; run: make coverage".into());
        }
        check_figures(&root)?;
        return Ok(format!("coverage=current {status}"));
    }
    if o.write {
        write(&root.join("README.md"), &updated)?;
        write_figures(&root, sun, anchor, o.publication)?;
        return Ok(format!(
            "published {status} figures={},{}",
            figure::CHART,
            figure::MAP
        ));
    }
    Ok(status)
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
