//! DONE accounting from verified build receipts and fixed executable inventories.
use crate::coverage::jsnum::{commas, round_half_up};
use crate::coverage::model::{bytes, intersect, normalize, subtract, Span};
use crate::coverage::pipeline::{validated_inventory, CoverageMap};
use crate::coverage::tree::root;
use serde_json::{json, Value};
use std::path::Path;

const USAGE: &str = "usage: alchemy check progress [--target tbs-en|tla-en] [--check|--subject|--json|--write-report|--self-test]";

fn report_json(report: &GameDone, target: &str) -> Value {
    let exact = report.common_c + report.game_c;
    let executable = report.executable;
    json!({
        "format": 2,
        "metric": "done-executable-byte-share",
        "target": target,
        "exact_c_bytes": exact,
        "permanent_assembly_bytes": report.common_asm + report.game_asm,
        "done_bytes": report.bytes(),
        "executable_bytes": executable,
        "remaining_bytes": executable - report.bytes(),
        "done_percent": report.percent(),
        "exact_c_percent": round_half_up(exact, executable),
        "parts": report,
        "state": "verified",
        "audit": "complete"
    })
}

fn pending_json(target: &str) -> Value {
    json!({
        "format": 2,
        "metric": "done-executable-byte-share",
        "target": target,
        "state": "pending",
        "audit": "incomplete",
        "done_bytes": Value::Null,
        "executable_bytes": Value::Null,
        "done_percent": Value::Null
    })
}

/// Where a game's credited bytes live: source shared by both games under
/// `games/COMMON/`, or the game's own.
const COMMON_SOURCE: &str = "games/COMMON/";

/// One game's DONE: shared permanent assembly, shared exact C, the game's own
/// permanent assembly and the game's own exact C, over its audited executable
/// bytes. The Broken Seal is ☀️ and The Lost Age ⚓️.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
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

/// Read the independently calculated score carried by the presentation map.
pub fn game_done(map: &CoverageMap) -> Result<GameDone, String> {
    serde_json::from_value(
        map.document
            .get("done")
            .cloned()
            .ok_or("coverage has no score")?,
    )
    .map_err(|e| e.to_string())
}

/// Count unions independently in each image. C takes precedence over
/// assembly; COMMON attribution never adds another copy of the same bytes.
fn tally(
    credits: &[super::proof::Credit],
    images: &std::collections::BTreeMap<String, Vec<Span>>,
) -> Result<GameDone, String> {
    for credit in credits {
        let exec = images
            .get(&credit.image)
            .ok_or_else(|| format!("unknown credit image {}", credit.image))?;
        if credit.end <= credit.start
            || credit.source.is_empty()
            || !matches!(credit.kind.as_str(), "c" | "assembly")
        {
            return Err("invalid build credit".into());
        }
        // A linked function may carry embedded data excluded by the audited
        // inventory; only its intersection with executable ranges counts.
        if intersect(&[Span::new(credit.start, credit.end)], exec).is_empty() {
            return Err(format!(
                "{} credit lies outside executable ranges",
                credit.image
            ));
        }
    }
    let mut done = GameDone::default();
    for (image, exec) in images {
        let ranges = |kind: &str, common: Option<bool>| {
            normalize(
                &credits
                    .iter()
                    .filter(|c| {
                        &c.image == image
                            && c.kind == kind
                            && common.is_none_or(|v| c.source.starts_with(COMMON_SOURCE) == v)
                    })
                    .map(|c| Span::new(c.start, c.end))
                    .collect::<Vec<_>>(),
            )
        };
        let c = intersect(&ranges("c", None), exec);
        let asm = subtract(&intersect(&ranges("assembly", None), exec), &c);
        let common_c = intersect(&ranges("c", Some(true)), &c);
        let common_asm = intersect(&ranges("assembly", Some(true)), &asm);
        if !intersect(&common_c, &ranges("c", Some(false))).is_empty()
            || !intersect(&common_asm, &ranges("assembly", Some(false))).is_empty()
        {
            return Err(format!(
                "{image}: COMMON and game sources claim the same bytes"
            ));
        }
        done.common_c += bytes(&common_c);
        done.game_c += bytes(&c) - bytes(&common_c);
        done.common_asm += bytes(&common_asm);
        done.game_asm += bytes(&asm) - bytes(&common_asm);
        done.executable += bytes(exec);
    }
    Ok(done)
}

/// A game's DONE, or `None` while it is unmeasured: its executable audit is
/// incomplete or not the independently verified automatic count, so its
/// denominator is unknown, or no byte-identical full build of the current
/// tree has left a receipt, so nothing is credited.
pub fn measured(root: &Path, target: &str) -> Result<Option<GameDone>, String> {
    Ok(status(root, target)?.ok())
}

/// A game's DONE, or why it is unmeasured.
fn status(root: &Path, target: &str) -> Result<Result<GameDone, String>, String> {
    let game = crate::targets::decomp_target(Some(target))?;
    let path = root.join(format!("out/{target}/reports/executable.json"));
    let text = match std::fs::read(&path) {
        Ok(text) => text,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            return Ok(Err(AUDIT_PENDING.into()))
        }
        Err(error) => return Err(format!("{}: {error}", path.display())),
    };
    let inventory: Value = serde_json::from_slice(&text).map_err(|e| e.to_string())?;
    let (main, mut images) = match validated_inventory(root, &inventory, game) {
        // A build that started or failed leaves no proof of the main image.
        Err(error) if error.contains("main-image proof no longer holds") => {
            return Ok(Err(BUILD_PENDING.into()))
        }
        Err(error) if error.contains("withheld") => return Ok(Err(AUDIT_PENDING.into())),
        Err(error) => return Err(error),
        Ok(images) => images,
    };
    images.insert("main".into(), main);
    // The denominator holds across source changes; credit needs a receipt
    // from a byte-identical build of exactly this tree.
    let Ok(receipt) = super::proof::read(root, target) else {
        return Ok(Err(BUILD_PENDING.into()));
    };
    if inventory["rom_sha256"].as_str() != Some(&receipt.rom_sha256) {
        return Err(format!(
            "{target}: executable inventory and verified source name different ROMs"
        ));
    }
    tally(&receipt.credits, &images).map(Ok)
}
const AUDIT_PENDING: &str = "pending its executable audit";
const BUILD_PENDING: &str = "pending a byte-identical build of the current tree";

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
    let line = |mark: &str, game: &str, done: Result<GameDone, String>| {
        match done {
        Ok(d) => format!(
            "{mark} {game} DONE: {} / {} executable bytes ({:.2}%) = common assembly {} + common C {} + game assembly {} + game C {}",
            commas(d.bytes()), commas(d.executable), d.percent(),
            commas(d.common_asm), commas(d.common_c), commas(d.game_asm), commas(d.game_c)
        ),
        Err(reason) => format!("{mark} {game} DONE: {reason}"),
    }
    };
    Ok(format!(
        "{}\n{}",
        line("☀️", "The Broken Seal", status(root, "tbs-en")?),
        line("⚓️", "The Lost Age", status(root, "tla-en")?)
    ))
}

fn display(report: &GameDone) -> String {
    format!(
        "DONE: {} / {} executable bytes ({:.2}%)\nExact C: {:.2}%",
        commas(report.bytes()),
        commas(report.executable),
        report.percent(),
        round_half_up(report.common_c + report.game_c, report.executable)
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
        return Ok("self-test=ok metric=done-executable-byte-share".into());
    }
    let root = root();
    if action == "--subject" {
        return subject(&root);
    }
    if action.is_empty() {
        return done_lines(&root);
    }
    let report = measured(&root, &target)?;
    if report.is_none() {
        if matches!(action, "--json" | "--write-report") {
            let output = serde_json::to_string_pretty(&pending_json(&target))
                .map_err(|error| error.to_string())?;
            if action == "--json" {
                return Ok(output);
            }
            let path = root.join("out").join(&target).join("reports/progress.json");
            std::fs::create_dir_all(path.parent().unwrap()).map_err(|error| error.to_string())?;
            std::fs::write(&path, format!("{output}\n")).map_err(|error| error.to_string())?;
            return Ok(format!(
                "report={} state=pending",
                path.strip_prefix(&root).unwrap_or(&path).display()
            ));
        }
    }
    let report = report.ok_or("executable audit is incomplete")?;
    match action {
        // A measured report already rests on the proof of a byte-identical
        // full build ([`super::proof::full_build`]).
        "--check" => Ok(display(&report)),
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
                "report={} target={} DONE={:.2}%",
                path.strip_prefix(&root).unwrap_or(&path).display(),
                target,
                report.percent()
            ))
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
    use super::super::proof::Credit;
    use super::*;
    #[test]
    fn credit_is_unique_per_image_and_c_supersedes_assembly() {
        let images = std::collections::BTreeMap::from([
            ("main".into(), vec![Span::new(0, 100)]),
            ("resource_a".into(), vec![Span::new(0, 100)]),
        ]);
        let credit = |image: &str, start, end, kind: &str, source: &str| Credit {
            image: image.into(),
            start,
            end,
            kind: kind.into(),
            source: source.into(),
        };
        let common = credit("main", 0, 20, "c", "games/COMMON/SRC/SOUND.C");
        let rows = vec![
            common.clone(),
            common,
            credit("main", 0, 40, "assembly", "recon/tla/raw/sound.s"),
            credit("resource_a", 0, 10, "c", "games/THE LOST AGE/SRC/FIELD.C"),
        ];
        let done = tally(&rows, &images).unwrap();
        assert_eq!(
            done,
            GameDone {
                common_c: 20,
                game_c: 10,
                game_asm: 20,
                common_asm: 0,
                executable: 200
            }
        );
        assert_eq!(done.percent(), 25.0);
        let report = report_json(&done, "tla-en");
        assert_eq!(report["done_percent"], 25.0);
        assert_eq!(report["exact_c_percent"], 15.0);
    }
    /// The Lost Age reports a real ⚓️ exactly while its byte-identical full
    /// build proves its inventory and its receipt is current; a withdrawn
    /// build, as any failed or mismatching one leaves, returns it to `?`.
    #[test]
    fn the_lost_age_is_measured_only_while_its_full_build_proves_it() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        crate::coverage::audit::authoritative_fixture(
            root,
            target,
            &[(0x0800_0100, 0x0800_0104)],
            json!([{"id": "resource_test", "decoded_bytes": 8, "intervals": [
                {"start": 0x0200_0000, "end": 0x0200_0004, "kind": "thumb"}
            ]}]),
        );
        // A proven inventory without the build's receipt is pending, never
        // a score.
        assert_eq!(status(root, "tla-en").unwrap(), Err(BUILD_PENDING.into()));
        let rom = std::fs::read(root.join(target.rom)).unwrap();
        let inputs = super::super::proof::identity(root, "tla-en").unwrap();
        let credit = |image: &str, start, end, kind: &str, source: &str| Credit {
            image: image.into(),
            start,
            end,
            kind: kind.into(),
            source: source.into(),
        };
        super::super::proof::write(
            root,
            "tla-en",
            &rom,
            &inputs,
            vec![
                credit(
                    "main",
                    0x0800_0100,
                    0x0800_0104,
                    "c",
                    "games/THE LOST AGE/SRC/A.C",
                ),
                credit(
                    "resource_test",
                    0x0200_0000,
                    0x0200_0002,
                    "assembly",
                    "recon/tla/raw/overlays/resource_test_overlay.s",
                ),
            ],
        )
        .unwrap();
        let done = measured(root, "tla-en").unwrap().unwrap();
        assert_eq!(
            done,
            GameDone {
                game_c: 4,
                game_asm: 2,
                executable: 8,
                ..GameDone::default()
            }
        );
        assert_eq!(subject(root).unwrap(), "☀️ ?% ⚓️ 75% –");
        // A changed source keeps the inventory but makes the receipt stale.
        let source = root.join("games/THE LOST AGE/SRC/A.C");
        std::fs::create_dir_all(source.parent().unwrap()).unwrap();
        std::fs::write(&source, "void A(void) {}\n").unwrap();
        assert!(
            crate::coverage::pipeline::authoritative_inventory(root, target)
                .unwrap()
                .is_some()
        );
        assert_eq!(status(root, "tla-en").unwrap(), Err(BUILD_PENDING.into()));
        assert_eq!(subject(root).unwrap(), "☀️ ?% ⚓️ ?% –");
        std::fs::remove_file(&source).unwrap();
        assert_eq!(subject(root).unwrap(), "☀️ ?% ⚓️ 75% –");
        crate::coverage::proof::withdraw_full_build(root, target).unwrap();
        assert_eq!(measured(root, "tla-en").unwrap(), None);
        assert_eq!(subject(root).unwrap(), "☀️ ?% ⚓️ ?% –");
    }
    #[test]
    fn conflicting_source_attribution_is_an_error() {
        let images = std::collections::BTreeMap::from([("main".into(), vec![Span::new(0, 100)])]);
        let mut credit = Credit {
            image: "main".into(),
            start: 0,
            end: 8,
            source: "games/COMMON/SRC/A.C".into(),
            kind: "c".into(),
        };
        let mut rows = vec![credit.clone()];
        credit.source = "games/THE LOST AGE/SRC/A.C".into();
        rows.push(credit);
        assert!(tally(&rows, &images).unwrap_err().contains("same bytes"));
    }
}
