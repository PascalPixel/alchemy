//! Full-C reporting over the coverage map's shared audited interval model.
use coverage_map::jsnum::{commas, round_half_up};
use coverage_map::model::{bytes, normalize, Span};
use coverage_map::pipeline::{build_coverage_map, progress_tally, BuildOptions, ProgressTally};
use coverage_map::tree::{root, work_tree_at};
use serde_json::{json, Value};
use std::path::Path;

const USAGE: &str = "usage: full-c-progress [--target gs1-en|gs2-en] [--check|--subject|--json|--write-report|--self-test]";

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

fn proven_assembly_bytes(root: &Path, target: &str) -> Result<i64, String> {
    let tree = work_tree_at(root.to_path_buf());
    let map = build_coverage_map(&BuildOptions {
        target: target.into(),
        exact: &tree,
        recon: Some(&tree),
        prefer_verified_assets: true,
    })?;
    map.document
        .pointer("/categories/proven_asm/bytes")
        .and_then(Value::as_i64)
        .ok_or_else(|| "coverage map has no categories.proven_asm.bytes".into())
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

fn subject(report: &ProgressTally, proven_assembly: i64) -> Result<String, String> {
    let (exact, executable) = totals(report);
    let done = exact + proven_assembly;
    if done < 0 || done > executable {
        return Err("DONE numerator exceeds executable denominator".into());
    }
    Ok(format!(
        "☀️ {}% –",
        (done * 100 + executable / 2) / executable
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
    let mut target = "gs1-en".to_string();
    let mut action = "";
    let mut index = 0;
    while index < argv.len() {
        match argv[index].as_str() {
            "--target" => {
                index += 1;
                target = match argv.get(index).map(String::as_str) {
                    Some(target @ ("gs1-en" | "gs2-en")) => target.into(),
                    Some(other) => {
                        return Err(format!(
                            "unsupported decomp target \"{other}\"; expected gs1-en or gs2-en"
                        ))
                    }
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected gs1-en or gs2-en".into(),
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
        prefer_verified_assets: true,
    })?;
    match action {
        "--check" => {
            check_build(&root, &target)?;
            Ok(display(&report))
        }
        "--subject" => subject(&report, proven_assembly_bytes(&root, &target)?),
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
                subject(&report, proven_assembly_bytes(&root, &target)?)?
            ))
        }
        "" => Ok(display(&report)),
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
