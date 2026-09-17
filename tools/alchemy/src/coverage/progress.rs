//! Full-C reporting over the coverage map's shared audited interval model.
use crate::coverage::jsnum::{commas, round_half_up};
use crate::coverage::model::{bytes, normalize, Category, Span, Tile};
use crate::coverage::pipeline::{
    build_coverage_map, progress_tally, BuildOptions, CoverageMap, ProgressTally,
};
use crate::coverage::tree::{root, work_tree_at};
use serde_json::{json, Value};
use std::path::Path;

const USAGE: &str = "usage: full-c-progress [--target tbs-en|tla-en] [--check|--subject|--json|--write-report|--self-test]";

fn totals(report: &ProgressTally) -> (i64, i64) {
    (
        report.main_exact + report.overlay_exact,
        report.main_executable + report.overlay_executable,
    )
}

fn report_json(report: &ProgressTally, target: &str) -> Value {
    let (exact, executable) = totals(report);
    json!({
        "format": 1,
        "metric": "full-c-byte-share",
        "target": target,
        "full_c_bytes": exact,
        "executable_bytes": executable,
        "remaining_bytes": executable - exact,
        "percent": round_half_up(exact, executable),
        "main": {"full_c_bytes": report.main_exact, "executable_bytes": report.main_executable},
        "overlays": {"full_c_bytes": report.overlay_exact, "executable_bytes": report.overlay_executable},
        "audit": "complete"
    })
}

fn check_build(root: &Path, target: &str) -> Result<(), String> {
    let path = root.join("out").join(target).join("full/rebuilt.json");
    let file = std::fs::File::open(&path)
        .map_err(|error| format!("cannot read {}: {error}", path.display()))?;
    let value: Value =
        serde_json::from_reader(file).map_err(|error| format!("{}: {error}", path.display()))?;
    if value["byte_identical"].as_bool() != Some(true) {
        return Err("the last full build was not byte-identical".into());
    }
    for field in ["rom_fallback_bytes", "unowned_bytes"] {
        if value[field].as_i64().unwrap_or(0) > 0 {
            return Err(format!("the last full build has non-zero {field}"));
        }
    }
    Ok(())
}

/// Where a game's credited bytes live: source shared by both games under
/// `games/COMMON/`, or the game's own.
const COMMON_SOURCE: &str = "games/COMMON/";

/// One game's DONE: shared permanent assembly, shared exact C, the game's own
/// permanent assembly and the game's own exact C, over its audited executable
/// bytes. The Broken Seal is ☀️ and The Lost Age ⚓️.
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
        round_half_up(self.bytes(), self.executable)
    }
    pub fn whole(&self) -> i64 {
        crate::coverage::jsnum::done_percent_whole(self.bytes(), 0, self.executable)
    }
}

/// Split a coverage map's credited bytes by where their source lives. The
/// parts must add up to the map's own exact-C and retained-assembly totals.
pub fn game_done(map: &CoverageMap) -> Result<GameDone, String> {
    fn walk(tile: &Tile, done: &mut GameDone) {
        if !tile.children.is_empty() {
            for child in &tile.children {
                walk(child, done);
            }
            return;
        }
        let common = tile
            .source
            .as_deref()
            .is_some_and(|source| source.starts_with(COMMON_SOURCE));
        let c = tile.categories[Category::ProvenC as usize];
        let asm = tile.categories[Category::ProvenAsm as usize];
        if common {
            done.common_c += c;
            done.common_asm += asm;
        } else {
            done.game_c += c;
            done.game_asm += asm;
        }
    }
    let mut done = GameDone::default();
    for tile in map.executable_areas.iter().flat_map(|area| &area.tiles) {
        walk(tile, &mut done);
    }
    let total = |path: &str| map.document.pointer(path).and_then(Value::as_i64);
    done.executable = total("/executable_bytes").ok_or("coverage map has no executable_bytes")?;
    if Some(done.common_c + done.game_c) != total("/categories/proven_c/bytes")
        || Some(done.common_asm + done.game_asm) != total("/categories/proven_asm/bytes")
    {
        return Err("DONE parts do not add up to the coverage map's totals".into());
    }
    if done.bytes() > done.executable {
        return Err("DONE numerator exceeds executable denominator".into());
    }
    Ok(done)
}

/// A game's DONE, or `None` while its executable audit is incomplete and its
/// denominator therefore unknown.
pub fn measured(root: &Path, target: &str) -> Result<Option<GameDone>, String> {
    let tree = work_tree_at(root.to_path_buf());
    match progress_tally(&BuildOptions {
        target: target.into(),
        exact: &tree,
        recon: None,
    }) {
        Err(error) if error.contains("withheld") => return Ok(None),
        Err(error) => return Err(error),
        Ok(_) => {}
    }
    let map = build_coverage_map(&BuildOptions {
        target: target.into(),
        exact: &tree,
        recon: Some(&tree),
    })?;
    game_done(&map).map(Some)
}

/// The commit prefix: both games' whole DONE percentages, `?` for a game whose
/// executable audit is incomplete.
fn subject(root: &Path) -> Result<String, String> {
    let whole = |done: Option<GameDone>| done.map_or("?".to_string(), |d| d.whole().to_string());
    Ok(format!(
        "☀️ {}% ⚓️ {}% –",
        whole(measured(root, "tbs-en")?),
        whole(measured(root, "tla-en")?)
    ))
}

/// Each game's DONE with its four parts, or why it is not measured yet.
fn done_lines(root: &Path) -> Result<String, String> {
    let line = |mark: &str, game: &str, done: Option<GameDone>| {
        match done {
        Some(d) => format!(
            "{mark} {game} DONE: {} / {} executable bytes ({:.2}%) = common assembly {} + common C {} + game assembly {} + game C {}",
            commas(d.bytes()), commas(d.executable), d.percent(),
            commas(d.common_asm), commas(d.common_c), commas(d.game_asm), commas(d.game_c)
        ),
        None => format!("{mark} {game} DONE: pending its executable audit"),
    }
    };
    Ok(format!(
        "{}\n{}",
        line("☀️", "The Broken Seal", measured(root, "tbs-en")?),
        line("⚓️", "The Lost Age", measured(root, "tla-en")?)
    ))
}

fn display(report: &ProgressTally) -> String {
    let (exact, executable) = totals(report);
    let percent = round_half_up(exact, executable);
    format!(
        "Full-C Byte Share: {} / {} executable bytes ({percent:.2}%)\nMain image: {} / {} executable bytes\nCode overlays: {} / {} executable bytes",
        commas(exact), commas(executable), commas(report.main_exact),
        commas(report.main_executable), commas(report.overlay_exact), commas(report.overlay_executable)
    )
}

fn command<'a>(argv: &'a [String]) -> Result<Option<(String, &'a str)>, String> {
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
            flag @ ("--check" | "--subject" | "--json" | "--write-report" | "--self-test") => {
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
        if bytes(&normalize(&[Span::new(0, 4), Span::new(2, 8)])) != 8 {
            return Err("interval self-test failed".into());
        }
        return Ok("self-test=ok metric=full-c-byte-share".into());
    }
    let root = root();
    let tree = work_tree_at(root.clone());
    let report = progress_tally(&BuildOptions {
        target: target.clone(),
        exact: &tree,
        recon: None,
    })?;
    match action {
        "--check" => {
            check_build(&root, &target)?;
            Ok(display(&report))
        }
        "--subject" => subject(&root),
        "--json" => {
            serde_json::to_string(&report_json(&report, &target)).map_err(|error| error.to_string())
        }
        "--write-report" => {
            let path = root.join("out").join(&target).join("reports/progress.json");
            let output = serde_json::to_string_pretty(&report_json(&report, &target))
                .map_err(|error| error.to_string())?;
            std::fs::create_dir_all(path.parent().unwrap()).map_err(|error| error.to_string())?;
            std::fs::write(&path, format!("{output}\n")).map_err(|error| error.to_string())?;
            Ok(format!(
                "report={} {}",
                path.strip_prefix(&root).unwrap_or(&path).display(),
                subject(&root)?
            ))
        }
        "" => Ok(format!("{}\n{}", display(&report), done_lines(&root)?)),
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
