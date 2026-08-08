//! Discover and run the native Rust self-tests.
//!
//! Each native tool owns the `--self-test` switch in its Rust source.  This
//! runner discovers those binaries from their Cargo manifests, runs them with
//! bounded parallelism, and refuses to pass when discovery finds nothing.
//!
//! ```text
//! cargo run --manifest-path tools/self-test/Cargo.toml
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --list
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --jobs 4
//! cargo run --manifest-path tools/self-test/Cargo.toml -- --self-test
//! ```

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};
use std::time::{Instant, SystemTime};

const JOB_SHARE_NUMERATOR: usize = 4;
const JOB_SHARE_DENOMINATOR: usize = 5;

fn jobs_for(cores: usize) -> usize {
    cores
        .saturating_mul(JOB_SHARE_NUMERATOR)
        .checked_div(JOB_SHARE_DENOMINATOR)
        .unwrap_or(0)
        .max(1)
}

fn available_parallelism() -> usize {
    std::thread::available_parallelism()
        .map(|count| count.get())
        .unwrap_or(1)
}

fn parse_jobs(args: &[String], cores: usize) -> Result<usize, String> {
    let cap = jobs_for(cores);
    let mut requested = None;
    let mut index = 0;
    while index < args.len() {
        let argument = &args[index];
        let raw = if argument == "--jobs" {
            index += 1;
            args.get(index)
                .ok_or_else(|| "--jobs requires a positive integer".to_string())?
        } else if let Some(value) = argument.strip_prefix("--jobs=") {
            value
        } else {
            index += 1;
            continue;
        };

        if requested.is_some() {
            return Err("--jobs may only be specified once".to_string());
        }
        let value = raw
            .parse::<usize>()
            .map_err(|_| format!("invalid --jobs value: {raw}"))?;
        if value == 0 {
            return Err("--jobs must be at least 1".to_string());
        }
        requested = Some(value.min(cap));
        index += 1;
    }
    Ok(requested.unwrap_or(cap))
}

#[derive(Debug, Clone)]
struct BinaryTarget {
    name: String,
    path: PathBuf,
}

#[derive(Debug, Default)]
struct PartialBinary {
    name: Option<String>,
    path: Option<PathBuf>,
}

fn append_binary(
    targets: &mut Vec<BinaryTarget>,
    binary: Option<PartialBinary>,
    package_name: Option<&str>,
) {
    let Some(binary) = binary else {
        return;
    };
    let Some(name) = binary.name else {
        return;
    };
    let path = binary.path.unwrap_or_else(|| {
        if package_name == Some(name.as_str()) {
            PathBuf::from("src/main.rs")
        } else {
            PathBuf::from("src/bin").join(format!("{name}.rs"))
        }
    });
    targets.push(BinaryTarget { name, path });
}

fn quoted_value(line: &str, key: &str) -> Option<String> {
    let value = line.strip_prefix(key)?.trim();
    let value = value.strip_prefix('"')?;
    let end = value.find('"')?;
    Some(value[..end].to_string())
}

fn binary_targets(manifest: &str, crate_dir: &Path) -> Vec<BinaryTarget> {
    let mut package_name = None;
    let mut in_package = false;
    let mut in_binary = false;
    let mut current: Option<PartialBinary> = None;
    let mut targets = Vec::new();

    for raw_line in manifest.lines() {
        let line = raw_line.trim();
        if line == "[package]" {
            append_binary(&mut targets, current.take(), package_name.as_deref());
            in_package = true;
            in_binary = false;
            continue;
        }
        if line == "[[bin]]" {
            append_binary(&mut targets, current.take(), package_name.as_deref());
            in_package = false;
            in_binary = true;
            current = Some(PartialBinary::default());
            continue;
        }
        if line.starts_with('[') {
            append_binary(&mut targets, current.take(), package_name.as_deref());
            in_package = false;
            in_binary = false;
            continue;
        }

        if in_package {
            if let Some(value) = quoted_value(line, "name =") {
                package_name = Some(value);
            }
        } else if in_binary {
            let binary = current.as_mut().expect("binary section has a value");
            if binary.name.is_none() {
                binary.name = quoted_value(line, "name =");
            }
            if binary.path.is_none() {
                binary.path = quoted_value(line, "path =").map(PathBuf::from);
            }
        }
    }
    append_binary(&mut targets, current, package_name.as_deref());

    if targets.is_empty() {
        if let Some(name) = package_name {
            let main = crate_dir.join("src/main.rs");
            if main.is_file() {
                targets.push(BinaryTarget {
                    name,
                    path: PathBuf::from("src/main.rs"),
                });
            }
        }
    }

    // Cargo supplies the conventional path when a bin target omits `path`.
    // The manifests in this tree spell it out, but keeping the fallback here
    // makes discovery work for a newly added native crate too.
    targets
}

fn rust_source_files(source_root: &Path) -> std::io::Result<Vec<PathBuf>> {
    fn visit(directory: &Path, files: &mut Vec<PathBuf>) -> std::io::Result<()> {
        let mut entries = fs::read_dir(directory)?.collect::<Result<Vec<_>, _>>()?;
        entries.sort_by_key(|entry| entry.file_name());
        for entry in entries {
            let path = entry.path();
            let kind = entry.file_type()?;
            if kind.is_dir() {
                visit(&path, files)?;
            } else if kind.is_file() && path.extension().is_some_and(|ext| ext == "rs") {
                files.push(path);
            }
        }
        Ok(())
    }

    let mut files = Vec::new();
    visit(source_root, &mut files)?;
    Ok(files)
}

fn contains_self_test_switch(path: &Path) -> bool {
    fs::read_to_string(path)
        .map(|source| source.contains("--self-test"))
        .unwrap_or(false)
}

fn forwards_native_arguments(path: &Path) -> bool {
    let Ok(source) = fs::read_to_string(path) else {
        return false;
    };
    // Some binaries keep their CLI in the crate library.  They are still
    // discoverable when the entry point forwards the process arguments.  A
    // dispatcher is deliberately not included: it consumes a group name
    // before it calls its library and therefore does not expose a top-level
    // self-test switch.
    (source.contains("std::env::args") || source.contains("env::args"))
        && !source.contains("Group::parse")
        && !source.contains("top_level_usage")
}

fn native_root(root: &Path) -> Result<PathBuf, String> {
    let directory = root.join("tools");
    directory
        .join("self-test/Cargo.toml")
        .is_file()
        .then_some(directory)
        .ok_or_else(|| {
            format!(
                "could not find the native tools root under {}",
                root.display()
            )
        })
}

fn newest_input_time(manifest: &Path, source_root: &Path) -> Option<SystemTime> {
    fn visit(path: &Path, newest: &mut SystemTime) -> std::io::Result<()> {
        let metadata = fs::metadata(path)?;
        let modified = metadata.modified()?;
        if modified > *newest {
            *newest = modified;
        }
        if metadata.is_dir() {
            let mut entries = fs::read_dir(path)?.collect::<Result<Vec<_>, _>>()?;
            entries.sort_by_key(|entry| entry.file_name());
            for entry in entries {
                visit(&entry.path(), newest)?;
            }
        }
        Ok(())
    }

    let mut newest = fs::metadata(manifest).ok()?.modified().ok()?;
    visit(source_root, &mut newest).ok()?;
    Some(newest)
}

fn binary_is_current(binary: &Path, manifest: &Path, source_root: &Path) -> bool {
    if fs::read(binary)
        .map(|bytes| {
            bytes
                .windows(b"tools-rs/".len())
                .any(|window| window == b"tools-rs/")
        })
        .unwrap_or(false)
    {
        return false;
    }
    let binary_time = fs::metadata(binary)
        .and_then(|metadata| metadata.modified())
        .ok();
    let input_time = newest_input_time(manifest, source_root);
    match (binary_time, input_time) {
        (Some(binary), Some(input)) => binary >= input,
        _ => false,
    }
}

fn current_release_binary(
    native_root: &Path,
    crate_dir: &Path,
    binary_name: &str,
    manifest: &Path,
    source_root: &Path,
) -> Option<PathBuf> {
    [
        crate_dir.join("target/release").join(binary_name),
        native_root.join("target/release").join(binary_name),
    ]
    .into_iter()
    .find(|binary| binary_is_current(binary, manifest, source_root))
}

#[derive(Debug, Clone)]
struct Invocation {
    name: String,
    program: PathBuf,
    arguments: Vec<String>,
}

fn native_tools(root: &Path) -> Result<Vec<Invocation>, String> {
    let native_root = native_root(root)?;
    let entries = fs::read_dir(&native_root).map_err(|error| {
        format!(
            "could not scan native tools root {}: {error}",
            native_root.display()
        )
    })?;
    let mut by_crate: BTreeMap<String, Vec<(String, PathBuf, Vec<String>)>> = BTreeMap::new();

    for entry in entries {
        let entry = entry.map_err(|error| format!("could not read native tools entry: {error}"))?;
        let crate_dir = entry.path();
        if !entry.file_type().map(|kind| kind.is_dir()).unwrap_or(false) {
            continue;
        }
        let crate_name = entry.file_name().to_string_lossy().into_owned();
        if crate_name == "self-test" || crate_name == "target" {
            continue;
        }

        let manifest_path = crate_dir.join("Cargo.toml");
        let source_root = crate_dir.join("src");
        if !manifest_path.is_file() || !source_root.is_dir() {
            continue;
        }
        let manifest = fs::read_to_string(&manifest_path).map_err(|error| {
            format!(
                "could not read native manifest {}: {error}",
                manifest_path.display()
            )
        })?;
        let source_files = rust_source_files(&source_root).map_err(|error| {
            format!(
                "could not scan native sources {}: {error}",
                source_root.display()
            )
        })?;
        if !source_files
            .iter()
            .any(|path| contains_self_test_switch(path))
        {
            continue;
        }
        let library_has_self_test = contains_self_test_switch(&source_root.join("lib.rs"));
        let targets = binary_targets(&manifest, &crate_dir);
        let selected: Vec<BinaryTarget> = targets
            .into_iter()
            .filter(|target| {
                let source_path = crate_dir.join(&target.path);
                contains_self_test_switch(&source_path)
                    || (library_has_self_test && forwards_native_arguments(&source_path))
            })
            .collect();
        if selected.is_empty() {
            continue;
        }

        for target in selected {
            let binary = current_release_binary(
                &native_root,
                &crate_dir,
                &target.name,
                &manifest_path,
                &source_root,
            );
            let (program, arguments) = match binary {
                Some(program) => (program, vec!["--self-test".to_string()]),
                None => (
                    PathBuf::from("cargo"),
                    vec![
                        "run".to_string(),
                        "--quiet".to_string(),
                        "--release".to_string(),
                        "--manifest-path".to_string(),
                        manifest_path.to_string_lossy().into_owned(),
                        "--bin".to_string(),
                        target.name.clone(),
                        "--".to_string(),
                        "--self-test".to_string(),
                    ],
                ),
            };
            by_crate
                .entry(crate_name.clone())
                .or_default()
                .push((target.name, program, arguments));
        }
    }

    if by_crate.is_empty() {
        return Ok(Vec::new());
    }

    let mut tools = Vec::new();
    for (crate_name, mut binaries) in by_crate {
        binaries.sort_by(|left, right| left.0.as_bytes().cmp(right.0.as_bytes()));
        let multiple = binaries.len() > 1;
        for (binary_name, program, arguments) in binaries {
            let name = if multiple {
                format!("rust/{crate_name}/{binary_name}")
            } else {
                format!("rust/{crate_name}")
            };
            tools.push(Invocation {
                name,
                program,
                arguments,
            });
        }
    }
    tools.sort_by(|left, right| left.name.as_bytes().cmp(right.name.as_bytes()));
    Ok(tools)
}

#[derive(Debug)]
struct Outcome {
    tool: String,
    ok: bool,
    output: String,
    elapsed_ms: u128,
}

fn run_one(root: &Path, tool: &Invocation) -> Outcome {
    let started = Instant::now();
    let spawned = Command::new(&tool.program)
        .args(&tool.arguments)
        .current_dir(root)
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .output();
    let elapsed_ms = started.elapsed().as_millis();
    match spawned {
        Ok(output) => {
            let stdout = String::from_utf8_lossy(&output.stdout);
            let stderr = String::from_utf8_lossy(&output.stderr);
            let mut report = if stderr.is_empty() {
                stdout.into_owned()
            } else {
                stderr.into_owned()
            };
            if !output.status.success() && report.trim().is_empty() {
                report = format!("process exited with {status}", status = output.status);
            }
            Outcome {
                tool: tool.name.clone(),
                ok: output.status.success(),
                output: report.trim().to_string(),
                elapsed_ms,
            }
        }
        Err(error) => Outcome {
            tool: tool.name.clone(),
            ok: false,
            output: format!("could not spawn {}: {error}", tool.program.display()),
            elapsed_ms,
        },
    }
}

fn run_all(root: &Path, tools: &[Invocation], jobs: usize) -> Vec<Outcome> {
    let next = Arc::new(AtomicUsize::new(0));
    let results = Arc::new(Mutex::new(Vec::with_capacity(tools.len())));
    let workers = jobs.min(tools.len());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = Arc::clone(&next);
            let results = Arc::clone(&results);
            scope.spawn(move || loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                if index >= tools.len() {
                    break;
                }
                let outcome = run_one(root, &tools[index]);
                results
                    .lock()
                    .expect("self-test results mutex")
                    .push(outcome);
            });
        }
    });
    Arc::try_unwrap(results)
        .expect("self-test workers still hold results")
        .into_inner()
        .expect("self-test results mutex")
}

fn tail_lines(output: &str, count: usize) -> impl Iterator<Item = &str> {
    let lines: Vec<&str> = output.lines().collect();
    let start = lines.len().saturating_sub(count);
    lines.into_iter().skip(start)
}

fn repo_root() -> PathBuf {
    if let Ok(explicit) = std::env::var("ALCHEMY_ROOT") {
        return PathBuf::from(explicit);
    }
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| PathBuf::from("."))
}

fn self_test() {
    let manifest = r#"
[package]
name = "example-tool"
version = "0.1.0"

[[bin]]
name = "example-tool"
path = "src/main.rs"

[[bin]]
name = "example-tool-bench"
path = "src/bin/bench.rs"
"#;
    let targets = binary_targets(manifest, Path::new("/fixture"));
    assert_eq!(targets.len(), 2, "Cargo binary discovery lost a target");
    assert_eq!(targets[0].name, "example-tool");
    assert_eq!(targets[1].path, PathBuf::from("src/bin/bench.rs"));

    assert_eq!(jobs_for(18), 14);
    assert_eq!(jobs_for(1), 1);
    assert_eq!(parse_jobs(&["--jobs".into(), "4".into()], 18), Ok(4));
    assert_eq!(parse_jobs(&["--jobs=64".into()], 18), Ok(14));
    assert!(parse_jobs(&["--jobs".into()], 18).is_err());
    assert!(parse_jobs(&["--jobs".into(), "0".into()], 18).is_err());

    let wrapper = Path::new("/fixture/src/main.rs");
    assert!(
        !forwards_native_arguments(wrapper),
        "fixture paths must not read the repository"
    );
    assert_eq!(
        tail_lines("1\n2\n3\n4\n5\n6\n7", 6).collect::<Vec<_>>(),
        vec!["2", "3", "4", "5", "6", "7"]
    );
    println!("self_test self-test ok");
}

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|argument| argument == "--self-test") {
        self_test();
        return;
    }

    let root = repo_root();
    let jobs = match parse_jobs(&args, available_parallelism()) {
        Ok(jobs) => jobs,
        Err(error) => {
            eprintln!("self-test: {error}");
            std::process::exit(2);
        }
    };
    let tools = match native_tools(&root) {
        Ok(tools) => tools,
        Err(error) => {
            eprintln!("self-test: {error}");
            std::process::exit(1);
        }
    };

    // A successful empty run is a vacuous pass.  Keep this guard immediately
    // after discovery so both normal execution and `--list` reject a broken
    // root or an accidentally unregistered native crate.
    if tools.is_empty() {
        eprintln!(
            "no native crate exposes a self-test under {} -- refusing to pass without scanning anything",
            native_root(&root)
                .map(|path| path.display().to_string())
                .unwrap_or_else(|_| "the repository".to_string())
        );
        std::process::exit(1);
    }

    if args.iter().any(|argument| argument == "--list") {
        for tool in &tools {
            println!("  {}", tool.name);
        }
        println!("{} native tools expose a self-test", tools.len());
        return;
    }

    let started = Instant::now();
    let mut results = run_all(&root, &tools, jobs);
    results.sort_by(|left, right| left.tool.as_bytes().cmp(right.tool.as_bytes()));
    let failed: Vec<&Outcome> = results.iter().filter(|result| !result.ok).collect();
    for result in &failed {
        eprintln!("FAIL {} ({}ms)", result.tool, result.elapsed_ms);
        for line in tail_lines(&result.output, 6) {
            eprintln!("     {line}");
        }
    }
    let seconds = started.elapsed().as_secs_f64();
    println!(
        "self-tests: {}/{} passed in {seconds:.1}s (jobs={jobs})",
        results.len() - failed.len(),
        results.len()
    );
    if !failed.is_empty() {
        std::process::exit(1);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::time::Duration;

    #[test]
    fn binary_targets_use_explicit_main_bin_and_package_fallback() {
        let manifest = r#"
[package]
name = "package-name"

[[bin]]
name = "helper"
path = "src/bin/helper.rs"

[[bin]]
name = "actual-tool"
path = "src/main.rs"
"#;
        let targets = binary_targets(manifest, Path::new("/missing"));
        assert_eq!(targets[1].name, "actual-tool");
        assert_eq!(targets[1].path, PathBuf::from("src/main.rs"));

        let fallback = binary_targets(
            "[package]\nname = \"fallback\"\n",
            Path::new(env!("CARGO_MANIFEST_DIR")),
        );
        assert_eq!(fallback.len(), 1);
        assert_eq!(fallback[0].name, "fallback");
    }

    #[test]
    fn jobs_are_bounded_and_invalid_values_are_rejected() {
        assert_eq!(jobs_for(18), 14);
        assert_eq!(jobs_for(2), 1);
        assert_eq!(parse_jobs(&[], 18), Ok(14));
        assert_eq!(parse_jobs(&["--jobs".into(), "4".into()], 18), Ok(4));
        assert_eq!(parse_jobs(&["--jobs=64".into()], 18), Ok(14));
        assert!(parse_jobs(&["--jobs".into(), "nope".into()], 18).is_err());
        assert!(parse_jobs(&["--jobs".into(), "0".into()], 18).is_err());
    }

    #[test]
    fn freshness_requires_a_binary_at_least_as_new_as_all_inputs() {
        let root = std::env::temp_dir().join(format!("self-test-freshness-{}", std::process::id()));
        let _ = fs::remove_dir_all(&root);
        fs::create_dir_all(root.join("src")).expect("fixture directory");
        let manifest = root.join("Cargo.toml");
        let source = root.join("src/main.rs");
        let binary = root.join("target/release/tool");
        fs::write(&manifest, "[package]\nname = \"tool\"\n").expect("manifest");
        fs::write(&source, "fn main() {}\n").expect("source");
        assert!(!binary_is_current(&binary, &manifest, &root.join("src")));
        fs::create_dir_all(binary.parent().expect("binary parent")).expect("target directory");
        fs::write(&binary, "binary\n").expect("binary");
        // A just-written binary is normally newer than the inputs.  If the
        // filesystem exposes equal timestamps, equality is intentionally also
        // accepted by binary_is_current.
        assert!(binary_is_current(&binary, &manifest, &root.join("src")));
        fs::write(&binary, b"embedded /repo/tools-rs/tool path\n").expect("retired binary");
        assert!(!binary_is_current(&binary, &manifest, &root.join("src")));
        fs::write(&binary, "binary\n").expect("current binary");
        std::thread::sleep(Duration::from_millis(10));
        fs::write(&source, "fn main() { println!(\"changed\"); }\n").expect("newer source");
        assert!(!binary_is_current(&binary, &manifest, &root.join("src")));
        fs::remove_dir_all(&root).expect("fixture cleanup");
    }

    #[test]
    fn failure_reports_are_limited_to_the_last_six_lines() {
        assert_eq!(
            tail_lines("1\n2\n3\n4\n5\n6\n7", 6).collect::<Vec<_>>(),
            vec!["2", "3", "4", "5", "6", "7"]
        );
        assert_eq!(tail_lines("only", 6).collect::<Vec<_>>(), vec!["only"]);
    }

    #[test]
    fn native_root_is_selected_without_a_tool_manifest_registry() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .expect("repository root");
        assert!(native_root(root).is_ok());
    }
}
