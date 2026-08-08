//! Probe every documented TypeScript tool command to make sure it starts.
//!
//! A command may exit non-zero for a legitimate finding, and a command that
//! ignores --help may keep running. Neither is a startup failure. The gate
//! therefore looks for the same unmistakable crash markers as the original
//! Bun implementation and kills a still-running probe after 20 seconds.

use std::fs;
use std::io::Read;
use std::path::{Path, PathBuf};
use std::process::{Child, Command, ExitCode, Stdio};
use std::sync::{
    atomic::{AtomicUsize, Ordering},
    Arc, Mutex,
};
use std::thread;
use std::time::{Duration, Instant};

const PROBE_TIMEOUT: Duration = Duration::from_secs(20);
const CRASH_MARKERS: &[&str] = &[
    "error: Cannot find module",
    "error: Cannot find package",
    "ENOENT: no such file or directory, open '--",
    "is not a function",
    "undefined is not an object",
    "SyntaxError",
    "TypeError:",
    "ReferenceError:",
];

const UNPROBED: &[(&str, &str)] = &[
    ("make/build_rom", "rebuilds the whole ROM"),
    ("make/build_full", "rebuilds every overlay"),
    ("make/build_assets", "re-encodes 2,431 asset regions"),
    ("make/build_asm", "regenerates the disassembly tree"),
    ("make/build_claimed", "rebuilds the claimed image"),
    ("make/build_semantic", "rewrites the semantic tree"),
    ("metrics/dashboard_server", "starts a server and waits"),
    ("overlay/overlay_adopt", "installs a source into an overlay"),
];

#[derive(Debug, PartialEq, Eq)]
struct Outcome {
    command: String,
    ok: bool,
    detail: String,
}

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate is under tools-rs")
        .parent()
        .expect("tools-rs is under the repository root")
        .to_path_buf()
}

fn crashed(output: &str) -> Option<&'static str> {
    CRASH_MARKERS
        .iter()
        .copied()
        .find(|marker| output.contains(marker))
}

fn commands_from_files<I>(tool: &str, files: I) -> Vec<String>
where
    I: IntoIterator<Item = String>,
{
    files
        .into_iter()
        .filter(|file| file.ends_with(".ts") && file != "index.ts")
        .map(|file| format!("{tool}/{}", file.trim_end_matches(".ts")))
        .filter(|command| !is_unprobed(command))
        .collect()
}

fn commands_under(tools: &[String], tools_dir: &Path) -> Result<Vec<String>, String> {
    let mut commands = Vec::new();
    for tool in tools {
        let entries = fs::read_dir(tools_dir.join(tool))
            .map_err(|error| format!("could not read tools/{tool}: {error}"))?;
        let files = entries
            .map(|entry| entry.map(|entry| entry.file_name().to_string_lossy().into_owned()))
            .collect::<Result<Vec<_>, _>>()
            .map_err(|error| format!("could not list tools/{tool}: {error}"))?;
        commands.extend(commands_from_files(tool, files));
    }
    commands.sort();
    Ok(commands)
}

fn is_unprobed(command: &str) -> bool {
    UNPROBED.iter().any(|(name, _)| *name == command)
}

fn tool_directories(tools_dir: &Path) -> Result<Vec<String>, String> {
    let entries = fs::read_dir(tools_dir)
        .map_err(|error| format!("could not read {}: {error}", tools_dir.display()))?;
    let mut tools = Vec::new();
    for entry in entries {
        let entry = entry.map_err(|error| format!("could not list tools: {error}"))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if !entry
            .file_type()
            .map_err(|error| format!("could not inspect tools/{name}: {error}"))?
            .is_dir()
            || name == "lib"
            || name == "scratch"
        {
            continue;
        }
        if tools_dir.join(&name).join("index.ts").exists() {
            tools.push(name);
        }
    }
    Ok(tools)
}

fn read_pipe<R>(mut pipe: R) -> thread::JoinHandle<Vec<u8>>
where
    R: Read + Send + 'static,
{
    thread::spawn(move || {
        let mut bytes = Vec::new();
        let _ = pipe.read_to_end(&mut bytes);
        bytes
    })
}

fn finish_probe(mut child: Child) -> Result<(String, String), String> {
    let stdout = child
        .stdout
        .take()
        .ok_or_else(|| "probe stdout was not captured".to_string())?;
    let stderr = child
        .stderr
        .take()
        .ok_or_else(|| "probe stderr was not captured".to_string())?;
    let stdout_thread = read_pipe(stdout);
    let stderr_thread = read_pipe(stderr);

    let deadline = Instant::now() + PROBE_TIMEOUT;
    loop {
        match child.try_wait() {
            Ok(Some(_)) => break,
            Ok(None) if Instant::now() >= deadline => {
                let _ = child.kill();
                let _ = child.wait();
                break;
            }
            Ok(None) => thread::sleep(Duration::from_millis(10)),
            Err(error) => return Err(format!("could not inspect probe: {error}")),
        }
    }

    let stdout = stdout_thread
        .join()
        .map_err(|_| "stdout reader panicked".to_string())?;
    let stderr = stderr_thread
        .join()
        .map_err(|_| "stderr reader panicked".to_string())?;
    Ok((
        String::from_utf8_lossy(&stdout).into_owned(),
        String::from_utf8_lossy(&stderr).into_owned(),
    ))
}

fn probe(root: &Path, command: &str) -> Outcome {
    let (tool, subcommand) = command
        .split_once('/')
        .expect("commands contain a tool and a subcommand");
    let script = root
        .join("tools")
        .join(tool)
        .join(format!("{subcommand}.ts"));
    let spawned = Command::new("bun")
        .arg(&script)
        .arg("--help")
        .current_dir(root)
        .env("ALCHEMY_PROBE", "1")
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn();
    let result = spawned
        .map_err(|error| format!("could not start bun: {error}"))
        .and_then(finish_probe)
        .map(|(stdout, stderr)| format!("{stdout}\n{stderr}"));
    match result {
        Ok(output) => match crashed(&output) {
            Some(marker) => Outcome {
                command: command.to_string(),
                ok: false,
                detail: marker.to_string(),
            },
            None => Outcome {
                command: command.to_string(),
                ok: true,
                detail: String::new(),
            },
        },
        Err(error) => Outcome {
            command: command.to_string(),
            ok: false,
            detail: error,
        },
    }
}

fn available_parallelism() -> usize {
    thread::available_parallelism()
        .map(|parallelism| parallelism.get())
        .unwrap_or(1)
}

fn jobs() -> usize {
    (((available_parallelism() as f64) * 0.8).floor() as usize).max(1)
}

fn run_all(commands: &[String], worker_count: usize, root: &Path) -> Vec<Outcome> {
    if commands.is_empty() {
        return Vec::new();
    }
    let commands = Arc::new(commands.to_vec());
    let next = Arc::new(AtomicUsize::new(0));
    let results = Arc::new(Mutex::new(Vec::with_capacity(commands.len())));
    let workers = worker_count.min(commands.len());
    thread::scope(|scope| {
        for _ in 0..workers {
            let commands = Arc::clone(&commands);
            let next = Arc::clone(&next);
            let results = Arc::clone(&results);
            scope.spawn(move || loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                let Some(command) = commands.get(index) else {
                    break;
                };
                results
                    .lock()
                    .expect("probe results mutex poisoned")
                    .push(probe(root, command));
            });
        }
    });
    Arc::try_unwrap(results)
        .expect("probe workers still exist")
        .into_inner()
        .expect("probe results mutex poisoned")
}

fn self_test() -> Result<(), String> {
    if crashed("usage: overlay <subcommand>").is_some() {
        return Err("a usage message must pass".into());
    }
    if crashed("error: Cannot find module './gone.ts'").is_none() {
        return Err("a missing module must fail".into());
    }
    if crashed("TypeError: x is not a function").is_none() {
        return Err("a TypeError must fail".into());
    }
    let mut commands = commands_from_files(
        "overlay",
        ["index.ts", "overlay_twins.ts", "notes.md"]
            .into_iter()
            .map(String::from),
    );
    commands.extend(commands_from_files(
        "make",
        ["build_rom.ts", "cache_entry.ts"]
            .into_iter()
            .map(String::from),
    ));
    commands.sort();
    if commands != ["make/cache_entry", "overlay/overlay_twins"] {
        return Err(format!("commands_under gave {}", commands.join(",")));
    }
    if !is_unprobed("make/build_rom") {
        return Err("destructive commands must stay unprobed".into());
    }
    println!("commands self-test ok");
    Ok(())
}

fn run() -> Result<(), String> {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }

    let root = root();
    let tools_dir = root.join("tools");
    let tools = tool_directories(&tools_dir)?;
    let commands = commands_under(&tools, &tools_dir)?;
    let started = Instant::now();
    let results = run_all(&commands, jobs(), &root);
    let mut failed = results
        .iter()
        .filter(|result| !result.ok)
        .collect::<Vec<_>>();
    failed.sort_by(|left, right| left.command.cmp(&right.command));
    for result in &failed {
        eprintln!("FAIL {}: {}", result.command, result.detail);
    }
    println!(
        "commands: {}/{} start cleanly in {:.1}s ({} unprobed by name)",
        results.len() - failed.len(),
        results.len(),
        started.elapsed().as_secs_f64(),
        UNPROBED.len()
    );
    if failed.is_empty() {
        Ok(())
    } else {
        Err(format!("{} command probes failed", failed.len()))
    }
}

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            if !error.is_empty() {
                eprintln!("error: {error}");
            }
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn crash_markers_match_startup_failures_only() {
        assert_eq!(crashed("usage: overlay <subcommand>"), None);
        assert_eq!(
            crashed("TypeError: x is not a function"),
            Some("is not a function")
        );
    }

    #[test]
    fn self_test_specification_passes() {
        self_test().expect("commands self-test");
    }

    #[test]
    fn jobs_keep_one_slot_and_leave_headroom() {
        assert_eq!(((18.0_f64 * 0.8).floor() as usize).max(1), 14);
        assert_eq!(((1.0_f64 * 0.8).floor() as usize).max(1), 1);
    }
}
