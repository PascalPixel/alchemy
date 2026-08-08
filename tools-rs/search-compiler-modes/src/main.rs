// Batch front-end for the authoritative compiler configuration explorer.
//
// Ported from tools/search/search_compiler_modes.ts. Candidate compilation,
// family selection, flag replacement, cache identity, compatibility rules and
// scoring all live in tools/lib/mode_sweep.ts; this binary only selects queue
// members, invokes that engine once per member, and aggregates the exact
// results into out/decomp/compiler-modes/report.json.
//
// WHY it stays a shell-out: mode_sweep.ts is the authority on what a
// "configuration" is, and duplicating its ~800 lines of compiler knowledge in
// Rust would create a second source of truth for the one thing this
// repository cannot afford to fork. The port covers the driver only.

use std::fs;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

use search_compiler_modes::{
    canonical_json, default_options, mode_sweep_output_directory, options_of, parse_json,
    resolve_path, summarize, Json, Options, ParseOutcome, MISSING_PATH, USAGE,
};
use semantic_queue::semantic_queue;

// The TypeScript derives ROOT from import.meta.url, three directories up from
// tools/search/. CARGO_MANIFEST_DIR is tools-rs/search-compiler-modes at
// compile time, so two up lands on the same repository root.
fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate lives under tools-rs")
        .parent()
        .expect("tools-rs lives at the repository root")
        .to_path_buf()
}

/// PORT NOTE: `navigator.hardwareConcurrency || 1` in Bun is the logical core
/// count; `available_parallelism` is the same number here. The `|| 1` fallback
/// covers the case where the platform will not say.
fn default_jobs() -> f64 {
    let cores = std::thread::available_parallelism()
        .map(std::num::NonZeroUsize::get)
        .unwrap_or(1);
    f64::from(u32::try_from(cores.min(8)).unwrap_or(1))
}

/// `readFileSync(path, "utf8")`.
///
/// PORT NOTE: Node's utf8 decode is lossy — invalid sequences become U+FFFD
/// rather than raising — so this is `String::from_utf8_lossy`, not
/// `fs::read_to_string`, which would error.
fn read_text(path: &Path) -> Result<String, String> {
    let bytes = fs::read(path).map_err(|error| {
        format!(
            "ENOENT: no such file or directory, open '{}' ({error})",
            path.display()
        )
    })?;
    Ok(String::from_utf8_lossy(&bytes).into_owned())
}

struct Item {
    stem: String,
    source: String,
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let root = repo_root();

    // PORT NOTE: cargo test is the real suite, but --self-test survives so the
    // repository's gate can invoke the Rust binary where it invoked
    // `bun tools/search/search_compiler_modes.ts --self-test`.
    if arguments.iter().any(|argument| argument == "--self-test") {
        return self_test(&root);
    }

    match run(&root, &arguments) {
        Ok(line) => {
            println!("{line}");
            ExitCode::SUCCESS
        }
        Err(Failure::Help) => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        Err(Failure::Error(message)) => {
            // PORT NOTE: an uncaught throw in Bun prints a source-framed stack
            // trace. Reproducing that would be noise, so the port prints
            // `error: <message>` — the same first line Bun emits — and exits
            // 1, the same status.
            let mut stderr = std::io::stderr();
            let _ = writeln!(stderr, "error: {message}");
            ExitCode::FAILURE
        }
    }
}

enum Failure {
    Help,
    Error(String),
}

fn self_test(root: &Path) -> ExitCode {
    let defaults = default_options(root, default_jobs());
    let parse = |arguments: &[&str]| -> Options {
        let owned: Vec<String> = arguments.iter().map(|a| (*a).to_string()).collect();
        match options_of(&owned, &defaults).expect("self-test arguments parse") {
            ParseOutcome::Parsed(options) => *options,
            ParseOutcome::Help => panic!("self-test arguments are not --help"),
        }
    };
    let parsed = parse(&["--limit", "7", "--pairs", "--max-pairs", "11"]);
    if parsed.limit != 7.0 || !parsed.pairs || parsed.triples || parsed.max_pairs != 11.0 {
        eprintln!("error: compiler mode batch option parsing differs");
        return ExitCode::FAILURE;
    }
    let triple = parse(&["--triples"]);
    if !triple.pairs || !triple.triples {
        eprintln!("error: triples must imply pairs");
        return ExitCode::FAILURE;
    }
    println!("compiler mode batch self-test passed");
    ExitCode::SUCCESS
}

fn run(root: &Path, arguments: &[String]) -> Result<String, Failure> {
    let defaults = default_options(root, default_jobs());
    let options = match options_of(arguments, &defaults).map_err(Failure::Error)? {
        ParseOutcome::Help => return Err(Failure::Help),
        ParseOutcome::Parsed(options) => *options,
    };
    for path in [&options.queue, &options.rom] {
        if path == MISSING_PATH {
            return Err(Failure::Error(
                "path option given without a value".to_string(),
            ));
        }
    }

    let mut items: Vec<Item> = if options.queue.is_empty() {
        semantic_queue(root)
            .into_iter()
            .map(|entry| Item {
                stem: entry.stem,
                source: resolve_path(root, &entry.draft),
            })
            .collect()
    } else {
        let queue_text = read_text(Path::new(&options.queue)).map_err(Failure::Error)?;
        let queue = parse_json(&queue_text).map_err(Failure::Error)?;
        let entries = queue
            .get("items")
            .and_then(Json::as_array)
            .ok_or_else(|| Failure::Error("queue.items is not iterable".to_string()))?;
        entries
            .iter()
            .filter_map(|entry| {
                let candidate = entry.get("candidate")?.as_str()?;
                let source = resolve_path(root, candidate);
                Path::new(&source).exists().then(|| Item {
                    stem: entry
                        .get("stem")
                        .and_then(Json::as_str)
                        .unwrap_or_default()
                        .to_string(),
                    source,
                })
            })
            .collect()
    };
    items.truncate((options.limit as usize).min(items.len()));
    if items.is_empty() {
        return Err(Failure::Error(
            "no queued candidates have source files".to_string(),
        ));
    }

    let reports = sweep(root, &options, &items)?;
    let summary = summarize(&reports, &options);

    let out = root.join("out/decomp/compiler-modes");
    fs::create_dir_all(&out)
        .map_err(|error| Failure::Error(format!("cannot create {}: {error}", out.display())))?;
    let report_path = out.join("report.json");
    fs::write(&report_path, canonical_json(&summary) + "\n")
        .map_err(|error| Failure::Error(format!("cannot write report: {error}")))?;

    Ok(format!(
        "candidates={} exact_configurations={} report={}",
        reports.len(),
        summary
            .get("matches")
            .and_then(Json::as_array)
            .map_or(0, <[Json]>::len),
        report_path.display()
    ))
}

/// Run mode_sweep.ts once per item across `jobs` workers.
///
/// PORT NOTE: `Promise.all` rejects with whichever worker happened to fail
/// first, which is a race when more than one job is in flight. The port is
/// deterministic instead: it reports the failure belonging to the
/// lowest-numbered item. With `--jobs 1` the two are identical.
fn sweep(root: &Path, options: &Options, items: &[Item]) -> Result<Vec<Json>, Failure> {
    let slots: Mutex<Vec<Option<Json>>> = Mutex::new((0..items.len()).map(|_| None).collect());
    let failures: Mutex<Vec<(usize, String)>> = Mutex::new(Vec::new());
    let cursor = AtomicUsize::new(0);
    let workers = (options.jobs as usize).min(items.len()).max(1);

    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, Ordering::SeqCst);
                if index >= items.len() {
                    return;
                }
                if !failures.lock().expect("failures lock").is_empty() {
                    return;
                }
                match sweep_one(root, options, &items[index]) {
                    Ok(report) => slots.lock().expect("slots lock")[index] = Some(report),
                    Err(message) => failures
                        .lock()
                        .expect("failures lock")
                        .push((index, message)),
                }
            });
        }
    });

    let mut failures = failures.into_inner().expect("failures lock");
    failures.sort_by_key(|(index, _)| *index);
    if let Some((_, message)) = failures.first() {
        return Err(Failure::Error(message.clone()));
    }
    Ok(slots
        .into_inner()
        .expect("slots lock")
        .into_iter()
        .map(|slot| slot.expect("every item produced a report"))
        .collect())
}

fn sweep_one(root: &Path, options: &Options, item: &Item) -> Result<Json, String> {
    let mut command = Command::new("bun");
    command
        .arg(root.join("tools/lib/mode_sweep.ts"))
        .arg(&item.source)
        .arg("--rom")
        .arg(&options.rom)
        .arg("--jobs")
        .arg("1")
        .arg("--top")
        .arg("1")
        .arg("--max-pairs")
        .arg(number_argument(options.max_pairs))
        .arg("--max-triples")
        .arg(number_argument(options.max_triples));
    if options.pairs {
        command.arg("--pairs");
    }
    if options.triples {
        command.arg("--triples");
    }
    let output = command
        .current_dir(root)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .output()
        .map_err(|error| format!("{}: cannot spawn bun: {error}", item.stem))?;

    if !output.status.success() {
        // PORT NOTE: `String(number)` for the child's message, and the same
        // `(stderr || stdout).trim().slice(0, 600)` truncation. `slice` counts
        // UTF-16 code units; the child's diagnostics are ASCII, and the port
        // truncates on characters, which agrees for that input.
        let stderr = String::from_utf8_lossy(&output.stderr).into_owned();
        let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
        let chosen = if stderr.is_empty() { stdout } else { stderr };
        let trimmed: String = chosen.trim().chars().take(600).collect();
        return Err(format!("{}: {trimmed}", item.stem));
    }

    let contents = fs::read(&item.source)
        .map_err(|error| format!("{}: cannot re-read source: {error}", item.stem))?;
    let directory = mode_sweep_output_directory(root, &item.source, &contents);
    let report_path = Path::new(&directory).join("report.json");
    let text = read_text(&report_path)?;
    parse_json(&text).map_err(|error| format!("{}: {error}", item.stem))
}

/// `String(value)` for the child's numeric arguments.
fn number_argument(value: f64) -> String {
    search_compiler_modes::js_number_text(value)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn repo_root_holds_the_native_queue_inputs() {
        assert!(repo_root()
            .join("tools-rs/semantic-queue/Cargo.toml")
            .exists());
        assert!(repo_root().join("tools/lib/mode_sweep.ts").exists());
    }

    #[test]
    fn default_jobs_is_capped_at_eight() {
        let jobs = default_jobs();
        assert!((1.0..=8.0).contains(&jobs), "unexpected job default {jobs}");
    }

    #[test]
    fn child_number_arguments_print_as_integers() {
        assert_eq!(number_argument(256.0), "256");
        assert_eq!(number_argument(1.0), "1");
    }

    #[test]
    fn missing_queue_reports_the_node_style_error() {
        let error = read_text(Path::new("/nonexistent/queue.json")).expect_err("must fail");
        assert!(
            error.starts_with("ENOENT: no such file or directory"),
            "{error}"
        );
    }
}
