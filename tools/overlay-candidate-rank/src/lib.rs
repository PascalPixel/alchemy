//! `tools/overlay/overlay_candidate_rank.ts`, in Rust.
//!
//! Compiles semantic overlay owners without touching their canonical
//! overlays, then ranks the candidates by likely exactification effort. See
//! The original's header carries the full rationale and is deliberately not
//! duplicated here. It was deleted with the rest of the TypeScript layer;
//! recover it with `git show e3867da35:tools/overlay/overlay_candidate_rank.ts`.

use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::thread::available_parallelism;

use exact_reading_list::json::{parse as parse_json, Value};
use exact_reading_list::{reading_list, Pairing};
use overlay_disasm::{assemble_overlay, compile_overlay_candidate, OverlaySource, OVERLAY_BASE};

/// 80% of the cores, never all of them. Kept
/// local rather than a dependency on the `jobs` crate, which ships only a
/// binary target, not a library one.
const SHARE: f64 = 0.8;

fn jobs_for(cores: usize) -> usize {
    ((cores as f64) * SHARE).floor().max(1.0) as usize
}

fn resolve_jobs(requested: Option<i64>, cores: usize) -> usize {
    let cap = jobs_for(cores);
    match requested {
        Some(n) if n >= 1 => (n as usize).min(cap),
        _ => cap,
    }
}

#[derive(Debug, Clone)]
pub struct Measurement {
    pub id: String,
    pub overlay: String,
    pub address: String,
    pub span: i64,
    pub candidate_bytes: Option<i64>,
    pub size_delta: Option<i64>,
    pub differing_halfwords: Option<i64>,
    /// `exact` / `ordering` / `allocation` / `unemittable` / `wrong`, from
    /// `candidate_show::render::residual_class`.
    ///
    /// Ranking by differing halfwords alone puts a blocked scheduler tie and a
    /// two-line source defect in the same tier. This column is what tells them
    /// apart, and only the last is worth opening a source for:
    ///
    /// - `ordering`, both sides hold the same instructions in a different
    ///   sequence, which is settled after reload;
    /// - `allocation`, they differ only in which registers they name, which is
    ///   the allocator's choice;
    /// - `unemittable`, the REFERENCE side holds an instruction stock gcc 2.96
    ///   cannot emit for Thumb from any source at all, so the region was never
    ///   C (bef5cad7c);
    /// - `wrong`, a genuine difference, and the only class a source reading can
    ///   move.
    pub residual_class: Option<String>,
    pub wrong_instructions: Option<i64>,
    pub semantic_source: String,
    pub error: Option<String>,
}

/// Disassemble both sides at their real address and classify the residual.
///
/// The address matters: a Thumb pc-relative load resolves against it, so
/// disassembling at 0 would make every pool target differ and report an
/// ordering-only row as wrong.
fn classify_owner(
    work: &Path,
    candidate: &[u8],
    reference: &[u8],
    address: f64,
) -> Result<(&'static str, i64), String> {
    let ours = work.join("rank-candidate.bin");
    let theirs = work.join("rank-reference.bin");
    fs::write(&ours, candidate).map_err(|error| error.to_string())?;
    fs::write(&theirs, reference).map_err(|error| error.to_string())?;
    let ordered = |rows: candidate_show::disasm::Rows| -> Vec<String> {
        let mut keys: Vec<f64> = rows.keys().collect();
        keys.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
        keys.iter()
            .map(|key| rows.get(*key).unwrap_or("").to_string())
            .collect()
    };
    let left = ordered(candidate_show::disasm::disassemble(
        &ours.to_string_lossy(),
        address,
    )?);
    let right = ordered(candidate_show::disasm::disassemble(
        &theirs.to_string_lossy(),
        address,
    )?);
    let _ = fs::remove_file(&ours);
    let _ = fs::remove_file(&theirs);
    Ok(candidate_show::render::residual_class(&left, &right))
}

/// `countDifferingHalfwords(actual, expected)`.
pub fn count_differing_halfwords(actual: &[u8], expected: &[u8]) -> i64 {
    let mut differing = 0i64;
    let length = actual.len().max(expected.len());
    let mut offset = 0usize;
    while offset < length {
        let a0 = actual.get(offset).copied();
        let a1 = actual.get(offset + 1).copied();
        let e0 = expected.get(offset).copied();
        let e1 = expected.get(offset + 1).copied();
        if a0 != e0 || a1 != e1 {
            differing += 1;
        }
        offset += 2;
    }
    differing
}

/// `effortTier(row)`.
pub fn effort_tier(row: &Measurement) -> i64 {
    if row.error.is_some() || row.size_delta.is_none() || row.differing_halfwords.is_none() {
        return 5;
    }
    let size_delta = row.size_delta.unwrap();
    let differing = row.differing_halfwords.unwrap();
    if size_delta != 0 {
        return 4;
    }
    if differing == 0 {
        return 0;
    }
    if differing <= 8 {
        return 1;
    }
    if differing <= 20 {
        return 2;
    }
    3
}

/// `compareMeasurements(left, right)`.
fn compare_measurements(left: &Measurement, right: &Measurement) -> std::cmp::Ordering {
    let tier = effort_tier(left) - effort_tier(right);
    if tier != 0 {
        return tier.cmp(&0);
    }
    let left_diff = left.differing_halfwords.unwrap_or(i64::MAX);
    let right_diff = right.differing_halfwords.unwrap_or(i64::MAX);
    if left_diff != right_diff {
        return left_diff.cmp(&right_diff);
    }
    (right.span - left.span).cmp(&0)
}

fn json_escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for ch in text.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out
}

fn render_worker_input(work: &Path, rows: &[Pairing]) -> String {
    let mut out = String::from("{\"work\":\"");
    out.push_str(&json_escape(&work.to_string_lossy()));
    out.push_str("\",\"rows\":[");
    for (index, row) in rows.iter().enumerate() {
        if index > 0 {
            out.push(',');
        }
        out.push_str(&format!(
            "{{\"overlay\":\"{}\",\"address\":\"{}\",\"bytes\":{},\"semanticSource\":\"{}\",\"blocked\":{}}}",
            json_escape(&row.overlay),
            json_escape(&row.address),
            row.bytes,
            json_escape(&row.semantic_source),
            row.blocked,
        ));
    }
    out.push_str("]}");
    out
}

fn parse_worker_input(text: &str) -> Result<(PathBuf, Vec<Pairing>), String> {
    let value = parse_json(text)?;
    let work = value
        .get("work")
        .and_then(Value::as_str)
        .ok_or("worker input missing work")?;
    let rows = value
        .get("rows")
        .and_then(Value::as_array)
        .ok_or("worker input missing rows")?;
    let mut pairings = Vec::with_capacity(rows.len());
    for row in rows {
        pairings.push(Pairing {
            overlay: row
                .get("overlay")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            address: row
                .get("address")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            bytes: row.get("bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            semantic_source: row
                .get("semanticSource")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            blocked: false,
        });
    }
    Ok((PathBuf::from(work), pairings))
}

/// `JSON.stringify(measurements)`: compact, used for worker input/output.
fn render_measurements(measurements: &[Measurement]) -> String {
    let mut out = String::from("[");
    for (index, row) in measurements.iter().enumerate() {
        if index > 0 {
            out.push(',');
        }
        out.push_str(&format!(
            "{{\"id\":\"{}\",\"overlay\":\"{}\",\"address\":\"{}\",\"span\":{}",
            json_escape(&row.id),
            json_escape(&row.overlay),
            json_escape(&row.address),
            row.span,
        ));
        if let Some(bytes) = row.candidate_bytes {
            out.push_str(&format!(",\"candidateBytes\":{bytes}"));
        }
        if let Some(delta) = row.size_delta {
            out.push_str(&format!(",\"sizeDelta\":{delta}"));
        }
        if let Some(diff) = row.differing_halfwords {
            out.push_str(&format!(",\"differingHalfwords\":{diff}"));
        }
        if let Some(class) = &row.residual_class {
            out.push_str(&format!(",\"residualClass\":\"{}\"", json_escape(class)));
        }
        if let Some(wrong) = row.wrong_instructions {
            out.push_str(&format!(",\"wrongInstructions\":{wrong}"));
        }
        out.push_str(&format!(
            ",\"semanticSource\":\"{}\"",
            json_escape(&row.semantic_source)
        ));
        if let Some(error) = &row.error {
            out.push_str(&format!(",\"error\":\"{}\"", json_escape(error)));
        }
        out.push('}');
    }
    out.push(']');
    out
}

/// `JSON.stringify({ measured }, null, 2)`: the pretty-printed `report.json`.
fn render_report(measurements: &[Measurement]) -> String {
    if measurements.is_empty() {
        return "{\n  \"measured\": []\n}\n".to_string();
    }
    let mut out = String::from("{\n  \"measured\": [\n");
    for (index, row) in measurements.iter().enumerate() {
        out.push_str("    {\n");
        out.push_str(&format!("      \"id\": \"{}\",\n", json_escape(&row.id)));
        out.push_str(&format!(
            "      \"overlay\": \"{}\",\n",
            json_escape(&row.overlay)
        ));
        out.push_str(&format!(
            "      \"address\": \"{}\",\n",
            json_escape(&row.address)
        ));
        let has_more = row.candidate_bytes.is_some()
            || row.size_delta.is_some()
            || row.differing_halfwords.is_some()
            || row.error.is_some();
        out.push_str(&format!(
            "      \"span\": {}{}\n",
            row.span,
            if has_more { "," } else { "" }
        ));
        if let Some(bytes) = row.candidate_bytes {
            let more = row.size_delta.is_some()
                || row.differing_halfwords.is_some()
                || row.error.is_some();
            out.push_str(&format!(
                "      \"candidateBytes\": {bytes}{}\n",
                if more { "," } else { "" }
            ));
        }
        if let Some(delta) = row.size_delta {
            let more = row.differing_halfwords.is_some() || row.error.is_some();
            out.push_str(&format!(
                "      \"sizeDelta\": {delta}{}\n",
                if more { "," } else { "" }
            ));
        }
        if let Some(diff) = row.differing_halfwords {
            out.push_str(&format!("      \"differingHalfwords\": {diff},\n"));
        }
        // The class and its count belong in the REPORT, not just in the console
        // table. They are the two columns that say whether an owner is worth
        // opening, the worker already serialises them and `parse_measurements`
        // already reads them -- only this renderer dropped them, so every bulk
        // consumer of report.json was blind to the classification and had to
        // re-derive it by scoring all 605 owners again.
        if let Some(class) = &row.residual_class {
            out.push_str(&format!(
                "      \"residualClass\": \"{}\",\n",
                json_escape(class)
            ));
        }
        if let Some(wrong) = row.wrong_instructions {
            out.push_str(&format!("      \"wrongInstructions\": {wrong},\n"));
        }
        out.push_str(&format!(
            "      \"semanticSource\": \"{}\"{}\n",
            json_escape(&row.semantic_source),
            if row.error.is_some() { "," } else { "" }
        ));
        if let Some(error) = &row.error {
            out.push_str(&format!("      \"error\": \"{}\"\n", json_escape(error)));
        }
        out.push_str("    }");
        if index + 1 < measurements.len() {
            out.push(',');
        }
        out.push('\n');
    }
    out.push_str("  ]\n}\n");
    out
}

fn parse_measurements(text: &str) -> Result<Vec<Measurement>, String> {
    let value = parse_json(text)?;
    let items = value.as_array().ok_or("worker output must be an array")?;
    let mut measurements = Vec::with_capacity(items.len());
    for item in items {
        measurements.push(Measurement {
            id: item
                .get("id")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            overlay: item
                .get("overlay")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            address: item
                .get("address")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            span: item.get("span").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            candidate_bytes: item
                .get("candidateBytes")
                .and_then(Value::as_f64)
                .map(|n| n as i64),
            size_delta: item
                .get("sizeDelta")
                .and_then(Value::as_f64)
                .map(|n| n as i64),
            differing_halfwords: item
                .get("differingHalfwords")
                .and_then(Value::as_f64)
                .map(|n| n as i64),
            residual_class: item
                .get("residualClass")
                .and_then(Value::as_str)
                .map(str::to_string),
            wrong_instructions: item
                .get("wrongInstructions")
                .and_then(Value::as_f64)
                .map(|n| n as i64),
            semantic_source: item
                .get("semanticSource")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            error: item
                .get("error")
                .and_then(Value::as_str)
                .map(str::to_string),
        });
    }
    Ok(measurements)
}

/// `measureWorker(inputPath, outputPath)`.
pub fn measure_worker(root: &Path, input_path: &Path, output_path: &Path) -> Result<(), String> {
    let input_text = fs::read_to_string(input_path).map_err(|error| error.to_string())?;
    let (work, rows) = parse_worker_input(&input_text)?;
    let mut images: Vec<(String, Vec<u8>)> = Vec::new();
    let mut measurements = Vec::with_capacity(rows.len());
    for row in &rows {
        let last4 = &row.address[row.address.len().saturating_sub(4)..];
        let id = format!("{}:{}", row.overlay, last4);
        let result = (|| -> Result<Measurement, String> {
            let image = match images.iter().find(|(name, _)| name == &row.overlay) {
                Some((_, data)) => data.clone(),
                None => {
                    let source_path = root
                        .join("assets/code")
                        .join(format!("{}_overlay.s", row.overlay));
                    let data = assemble_overlay(&OverlaySource::path(&source_path), OVERLAY_BASE)?;
                    images.push((row.overlay.clone(), data.clone()));
                    data
                }
            };
            let address = i64::from_str_radix(row.address.trim_start_matches("0x"), 16)
                .map_err(|error| error.to_string())?;
            let start = (address - OVERLAY_BASE).max(0) as usize;
            let end = (start + row.bytes as usize).min(image.len());
            let expected = if start < image.len() {
                &image[start..end]
            } else {
                &[]
            };
            let source = root.join(&row.semantic_source);
            let routing_source =
                root.join("exact")
                    .join(format!("{}_c_{}.c", row.overlay, &row.address[2..]));
            let owner_work = work.join(format!("{}-{}", row.overlay, &row.address[2..]));
            fs::create_dir_all(&owner_work).map_err(|error| error.to_string())?;
            let compiled = compile_overlay_candidate(
                &source,
                &owner_work,
                &row.overlay,
                Some(&routing_source),
                &[],
            )?;
            // Classifying costs two disassembles per owner and is what makes
            // the table actionable, so a failure here degrades the row to an
            // unclassified one rather than losing the measurement.
            let (class, wrong) =
                match classify_owner(&owner_work, &compiled.data, expected, address as f64) {
                    Ok(pair) => (Some(pair.0.to_string()), Some(pair.1)),
                    Err(message) => {
                        eprintln!("{id}: residual not classified: {message}");
                        (None, None)
                    }
                };
            Ok(Measurement {
                id: id.clone(),
                overlay: row.overlay.clone(),
                address: row.address.clone(),
                span: row.bytes,
                candidate_bytes: Some(compiled.data.len() as i64),
                size_delta: Some(compiled.data.len() as i64 - row.bytes),
                differing_halfwords: Some(count_differing_halfwords(&compiled.data, expected)),
                residual_class: class,
                wrong_instructions: wrong,
                semantic_source: row.semantic_source.clone(),
                error: None,
            })
        })();
        measurements.push(match result {
            Ok(measurement) => measurement,
            Err(message) => Measurement {
                id,
                overlay: row.overlay.clone(),
                address: row.address.clone(),
                span: row.bytes,
                candidate_bytes: None,
                size_delta: None,
                differing_halfwords: None,
                residual_class: None,
                wrong_instructions: None,
                semantic_source: row.semantic_source.clone(),
                error: Some(message),
            },
        });
    }
    fs::write(output_path, render_measurements(&measurements)).map_err(|error| error.to_string())
}

pub fn self_test() -> Result<(), String> {
    let base = |size_delta: Option<i64>, differing_halfwords: Option<i64>| Measurement {
        id: "resource_000:0000".to_string(),
        overlay: "resource_000".to_string(),
        address: "0x02000000".to_string(),
        span: 16,
        candidate_bytes: None,
        size_delta,
        differing_halfwords,
        residual_class: None,
        wrong_instructions: None,
        semantic_source: "semantic/example.c".to_string(),
        error: None,
    };
    if count_differing_halfwords(&[0, 1], &[0, 1]) != 0 {
        return Err("equal".to_string());
    }
    if count_differing_halfwords(&[0, 1], &[0, 2]) != 1 {
        return Err("diff".to_string());
    }
    if effort_tier(&base(Some(0), Some(0))) != 0 {
        return Err("exact tier".to_string());
    }
    if effort_tier(&base(Some(0), Some(8))) != 1 {
        return Err("near tier".to_string());
    }
    if effort_tier(&base(Some(0), Some(20))) != 2 {
        return Err("shape tier".to_string());
    }
    if effort_tier(&base(Some(2), Some(1))) != 4 {
        return Err("size tier".to_string());
    }
    println!("self-test=ok tool=overlay-candidate-rank");
    Ok(())
}

fn value_after<'a>(args: &'a [String], flag: &str) -> Option<&'a str> {
    args.iter()
        .position(|arg| arg == flag)
        .and_then(|index| args.get(index + 1))
        .map(String::as_str)
}

const USAGE: &str = "usage: overlay-candidate-rank [--overlay resource_NNN] [--jobs N] [--top N] [--max N] [--work PATH]\n       overlay-candidate-rank --self-test\n       overlay-candidate-rank --worker INPUT OUTPUT";

fn validate_args(args: &[String]) -> Result<bool, String> {
    if args.first().map(String::as_str) == Some("--worker") {
        return if args.len() == 3 {
            Ok(false)
        } else {
            Err("usage: overlay-candidate-rank --worker INPUT OUTPUT".to_string())
        };
    }
    if args.len() == 1 && args[0] == "--self-test" {
        return Ok(false);
    }
    if args.len() == 1 && matches!(args[0].as_str(), "-h" | "--help") {
        println!("{USAGE}");
        return Ok(true);
    }
    let value_flags = ["--overlay", "--jobs", "--top", "--max", "--work"];
    let mut index = 0;
    while index < args.len() {
        let argument = args[index].as_str();
        if !value_flags.contains(&argument) {
            return Err(format!("unknown or misplaced option: {argument}\n{USAGE}"));
        }
        let value = args
            .get(index + 1)
            .ok_or_else(|| format!("{argument} requires a value\n{USAGE}"))?;
        if value.starts_with('-') && argument != "--work" {
            return Err(format!("{argument} requires a value\n{USAGE}"));
        }
        if matches!(argument, "--jobs" | "--top" | "--max") {
            let number = value
                .parse::<i64>()
                .map_err(|_| format!("{argument} must be a positive integer\n{USAGE}"))?;
            if number < 1 {
                return Err(format!("{argument} must be a positive integer\n{USAGE}"));
            }
        }
        index += 2;
    }
    Ok(false)
}

/// `main()`. `root` is `overlay_candidate_rank.ts`'s `ROOT`; `self_exe` is the
/// path this process was invoked as, reused to spawn worker children.
///
/// `subcommand` is the argv this command is reached by inside its host. Under
/// the consolidated `overlay` binary that is `candidate-rank`, and a worker
/// child spawned without it lands on the host's dispatcher, which rejects
/// `--worker` as an unknown command. Empty when the crate is its own binary.
pub fn run(
    root: &Path,
    self_exe: &Path,
    subcommand: &[&str],
    args: &[String],
) -> Result<(), String> {
    if validate_args(args)? {
        return Ok(());
    }
    if args.first().map(String::as_str) == Some("--worker") {
        let input_path = args.get(1).ok_or("--worker requires an input path")?;
        let output_path = args.get(2).ok_or("--worker requires an output path")?;
        return measure_worker(root, Path::new(input_path), Path::new(output_path));
    }
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let only_overlay = value_after(args, "--overlay");
    let requested_jobs = value_after(args, "--jobs").and_then(|text| text.parse::<i64>().ok());
    let cores = available_parallelism().map(|n| n.get()).unwrap_or(1);
    let jobs = resolve_jobs(requested_jobs, cores) as i64;
    let top = value_after(args, "--top")
        .and_then(|text| text.parse::<i64>().ok())
        .unwrap_or(40);
    let limit = value_after(args, "--max")
        .and_then(|text| text.parse::<i64>().ok())
        .unwrap_or(i64::MAX);
    if jobs < 1 || top < 1 || limit < 1 {
        return Err("--jobs, --top and --max must be positive integers".to_string());
    }
    let mut rows: Vec<Pairing> = reading_list(root)?
        .into_iter()
        .filter(|row| !row.blocked)
        .collect();
    if let Some(overlay) = only_overlay {
        rows.retain(|row| row.overlay == overlay);
    }
    if limit < rows.len() as i64 {
        rows.truncate(limit.max(0) as usize);
    }
    let work = value_after(args, "--work")
        .map(PathBuf::from)
        .unwrap_or_else(|| root.join("out/overlay-candidate-rank"));
    fs::create_dir_all(&work).map_err(|error| error.to_string())?;

    let bucket_count = (jobs as usize).min(rows.len().max(1));
    let mut buckets: Vec<Vec<Pairing>> = (0..bucket_count).map(|_| Vec::new()).collect();
    for (index, row) in rows.into_iter().enumerate() {
        buckets[index % bucket_count].push(row);
    }

    let mut children = Vec::with_capacity(buckets.len());
    for (index, bucket) in buckets.iter().enumerate() {
        let input_path = work.join(format!("worker-{index}.input.json"));
        let output_path = work.join(format!("worker-{index}.output.json"));
        let owner_work = work.join(format!("worker-{index}"));
        fs::write(&input_path, render_worker_input(&owner_work, bucket))
            .map_err(|error| error.to_string())?;
        let child = Command::new(self_exe)
            .args(subcommand)
            .arg("--worker")
            .arg(&input_path)
            .arg(&output_path)
            .current_dir(root)
            // Piped, not inherited: the failure path below reads this back, and
            // an inherited handle makes `child.stderr.take()` return None and
            // reports a bare exit status instead of the worker's own message.
            .stderr(std::process::Stdio::piped())
            .spawn()
            .map_err(|error| error.to_string())?;
        children.push((child, output_path));
    }

    let mut measured: Vec<Measurement> = Vec::new();
    for (child, output_path) in children {
        // `wait_with_output`, not `wait`: it drains the stderr pipe while the
        // child runs, so a worker that outtalks the pipe buffer cannot wedge.
        let finished = child.wait_with_output().map_err(|error| error.to_string())?;
        let status = finished.status;
        if !status.success() {
            let stderr = String::from_utf8_lossy(&finished.stderr).trim().to_string();
            return Err(if stderr.is_empty() {
                format!("worker failed: {status}")
            } else {
                stderr
            });
        }
        if output_path.exists() {
            let text = fs::read_to_string(&output_path).map_err(|error| error.to_string())?;
            measured.extend(parse_measurements(&text)?);
        }
    }
    measured.sort_by(compare_measurements);

    fs::write(work.join("report.json"), render_report(&measured))
        .map_err(|error| error.to_string())?;

    // `class` is the column to read first. `ordering` means both sides hold the
    // same instructions and only their order differs, which no source shape
    // reaches; `wrong` means an instruction is genuinely wrong and reading the
    // source will find it. Ranking by diff_hw alone mixes the two.
    println!(
        "tier  owner                 span  bytes  delta  diff_hw  class     wrong  semantic source"
    );
    for row in measured.iter().take(top.max(0) as usize) {
        let delta = match row.size_delta {
            None => "error".to_string(),
            Some(delta) if delta >= 0 => format!("+{delta}"),
            Some(delta) => format!("{delta}"),
        };
        let candidate_bytes = row
            .candidate_bytes
            .map(|n| n.to_string())
            .unwrap_or_else(|| "-".to_string());
        let differing = row
            .differing_halfwords
            .map(|n| n.to_string())
            .unwrap_or_else(|| "-".to_string());
        let class = row.residual_class.clone().unwrap_or_else(|| "-".to_string());
        let wrong = row
            .wrong_instructions
            .map(|n| n.to_string())
            .unwrap_or_else(|| "-".to_string());
        println!(
            "{:>4}  {:<20}  {:>4}  {:>5}  {:>5}  {:>7}  {:<8}  {:>5}  {}",
            effort_tier(row),
            row.id,
            row.span,
            candidate_bytes,
            delta,
            differing,
            class,
            wrong,
            row.semantic_source,
        );
    }
    let counts: Vec<usize> = (0..6)
        .map(|tier| {
            measured
                .iter()
                .filter(|row| effort_tier(row) == tier)
                .count()
        })
        .collect();
    println!(
        "measured={} exact={} near8={} near20={} same_size_far={} size_mismatch={} errors={}",
        measured.len(),
        counts[0],
        counts[1],
        counts[2],
        counts[3],
        counts[4],
        counts[5],
    );
    println!("report={}", work.join("report.json").to_string_lossy());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn measurement(
        span: i64,
        size_delta: Option<i64>,
        differing: Option<i64>,
        error: Option<&str>,
    ) -> Measurement {
        Measurement {
            id: "resource_0:0".to_string(),
            overlay: "resource_0".to_string(),
            address: "0x0".to_string(),
            span,
            candidate_bytes: None,
            size_delta,
            differing_halfwords: differing,
            residual_class: None,
            wrong_instructions: None,
            semantic_source: "semantic".to_string(),
            error: error.map(str::to_string),
        }
    }

    #[test]
    fn differing_halfwords_counts_by_halfword() {
        assert_eq!(count_differing_halfwords(&[1, 2, 3, 4], &[1, 2, 3, 4]), 0);
        assert_eq!(count_differing_halfwords(&[1, 2, 3, 4], &[1, 2, 3, 5]), 1);
        assert_eq!(count_differing_halfwords(&[1, 2], &[1, 2, 3, 4]), 1);
        assert_eq!(count_differing_halfwords(&[], &[1, 2, 3, 4]), 2);
    }

    #[test]
    fn tier_ordering_matches_ts() {
        assert_eq!(effort_tier(&measurement(4, None, None, Some("boom"))), 5);
        assert_eq!(effort_tier(&measurement(4, Some(2), Some(0), None)), 4);
        assert_eq!(effort_tier(&measurement(4, Some(0), Some(0), None)), 0);
        assert_eq!(effort_tier(&measurement(4, Some(0), Some(8), None)), 1);
        assert_eq!(effort_tier(&measurement(4, Some(0), Some(20), None)), 2);
        assert_eq!(effort_tier(&measurement(4, Some(0), Some(21), None)), 3);
    }

    #[test]
    fn compare_orders_by_tier_then_diff_then_span_desc() {
        let low_tier = measurement(4, Some(0), Some(0), None);
        let high_tier = measurement(4, Some(2), Some(0), None);
        assert_eq!(
            compare_measurements(&low_tier, &high_tier),
            std::cmp::Ordering::Less
        );

        let fewer_diff = measurement(4, Some(0), Some(1), None);
        let more_diff = measurement(4, Some(0), Some(9), None);
        assert_eq!(
            compare_measurements(&fewer_diff, &more_diff),
            std::cmp::Ordering::Less
        );

        let bigger_span = measurement(8, Some(0), Some(1), None);
        let smaller_span = measurement(4, Some(0), Some(1), None);
        assert_eq!(
            compare_measurements(&bigger_span, &smaller_span),
            std::cmp::Ordering::Less
        );
    }

    #[test]
    fn render_report_matches_json_stringify_pretty() {
        let rows = vec![measurement(4, Some(0), Some(2), None)];
        let expected = "{\n  \"measured\": [\n    {\n      \"id\": \"resource_0:0\",\n      \"overlay\": \"resource_0\",\n      \"address\": \"0x0\",\n      \"span\": 4,\n      \"sizeDelta\": 0,\n      \"differingHalfwords\": 2,\n      \"semanticSource\": \"semantic\"\n    }\n  ]\n}\n";
        assert_eq!(render_report(&rows), expected);
        assert_eq!(render_report(&[]), "{\n  \"measured\": []\n}\n");
    }

    /// The classification has to survive into the report, because that is the
    /// only place a bulk consumer can read it. It did not, for as long as the
    /// column existed: the worker serialised it and `parse_measurements` read it,
    /// and this renderer silently dropped it.
    #[test]
    fn the_report_carries_the_residual_class_and_its_count() {
        let mut row = measurement(4, Some(0), Some(2), None);
        row.residual_class = Some("allocation".to_string());
        row.wrong_instructions = Some(8);
        let rendered = render_report(&[row]);
        assert!(
            rendered.contains("\"residualClass\": \"allocation\","),
            "report lost the class: {rendered}"
        );
        assert!(
            rendered.contains("\"wrongInstructions\": 8,"),
            "report lost the wrong-instruction count: {rendered}"
        );
        // And a round trip, so the two renderers cannot drift apart again.
        let parsed = parse_measurements(&render_measurements(&[measurement_with_class()]))
            .expect("worker output parses");
        assert_eq!(parsed[0].residual_class.as_deref(), Some("ordering"));
        assert_eq!(parsed[0].wrong_instructions, Some(0));
    }

    fn measurement_with_class() -> Measurement {
        let mut row = measurement(4, Some(0), Some(2), None);
        row.residual_class = Some("ordering".to_string());
        row.wrong_instructions = Some(0);
        row
    }

    #[test]
    fn help_and_unknown_options_are_rejected_before_work() {
        assert!(validate_args(&["--unknown".into()]).is_err());
        assert!(validate_args(&["--jobs".into()]).is_err());
        assert!(validate_args(&["--jobs".into(), "0".into()]).is_err());
        assert!(validate_args(&["--help".into()]).unwrap());
    }
}
