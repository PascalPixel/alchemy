use super::{figure, history, letters, measure_both};
use crate::compiler::routing::root;
use std::time::Instant;

const USAGE: &str = r#"usage: alchemy figure [--no-record] [--times]
After a build of all twelve editions (make compare-editions), measures DONE
for both games, records it as today's row of history.tsv beside the figures
with the commits' models, and redraws PROGRESS_CHART.png and PROGRESS.png.
--no-record redraws both from the history as it stands; --times prints how
long each step took."#;

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        return Err(USAGE.into());
    }
    if let Some(unknown) = arguments
        .iter()
        .find(|a| !matches!(a.as_str(), "--no-record" | "--times"))
    {
        return Err(format!("unknown option {unknown}\n{USAGE}"));
    }
    let record = !arguments.iter().any(|a| a == "--no-record");
    let times = arguments.iter().any(|a| a == "--times");
    let root = root();
    let mut steps: Vec<(&str, f64)> = Vec::new();
    let mut clock = Instant::now();
    let mut lap = |name: &'static str, steps: &mut Vec<(&str, f64)>| {
        steps.push((name, clock.elapsed().as_secs_f64()));
        clock = Instant::now();
    };

    let mut table = history::load(root)?;
    let today = history::today();
    if record {
        let [sun, anchor] = measure_both(root)?;
        let (sun, anchor) = (
            sun.ok().map(|game| game.combined().done),
            anchor.ok().map(|game| game.combined().done),
        );
        if sun.is_none() && anchor.is_none() {
            return Err("no verified build to measure; run make compare-editions first".into());
        }
        lap("measure", &mut steps);
        let hour = history::this_hour();
        history::record(&mut table, &today, sun, anchor);
        history::record_hour(&mut table, &hour, sun, anchor);
        history::derive_models(root, &mut table)?;
        lap("history", &mut steps);
    }
    history::mark_drawn(&mut table, &today);
    std::fs::write(history::path(root), history::text(&table))
        .map_err(|e| format!("{}: {e}", history::path(root).display()))?;

    let face = letters::Letters::face();
    let chart = figure::chart(&face, &history::as_drawn(&table));
    lap("chart", &mut steps);
    let map = figure::map(&face, root);
    lap("treemap", &mut steps);
    let scale = letters::FIGURE_SCALE;
    let (chart, map) = (chart.png(scale, &today)?, map.png(scale, &today)?);
    lap("encode", &mut steps);
    for (name, png) in [(figure::CHART, &chart), (figure::MAP, &map)] {
        std::fs::write(root.join(name), png).map_err(|e| format!("{name}: {e}"))?;
    }
    println!("wrote {} and {}", figure::CHART, figure::MAP);
    if times {
        let total: f64 = steps.iter().map(|(_, s)| s).sum();
        for (name, seconds) in &steps {
            println!("{name:>8} {seconds:7.3}s");
        }
        println!("{:>8} {total:7.3}s", "total");
    }
    Ok(())
}
