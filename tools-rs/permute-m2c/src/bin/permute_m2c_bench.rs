//! An honest benchmark.
//!
//! Two measurements, reported separately and never compared to each other:
//!
//! * whole-process: `bun tools/lib/permute_m2c.ts` against the release binary,
//!   both with the production defaults. This includes interpreter startup on one
//!   side and nothing comparable on the other, which is exactly why it is
//!   reported on its own.
//! * in-process: `candidates()` over the real corpus on each side, timed inside
//!   the process so startup is excluded from both.
//!
//! The compile path is measured separately again, because it is dominated by
//! `arm-none-eabi-*` subprocesses that both sides pay identically.

use std::path::PathBuf;
use std::process::Command;
use std::time::Instant;

fn root() -> PathBuf {
    permute_m2c::paths::root().to_path_buf()
}

fn corpus() -> Vec<String> {
    let directory = root().join("work/matches/m2c");
    let mut files: Vec<PathBuf> = std::fs::read_dir(&directory)
        .expect("corpus")
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| path.extension().is_some_and(|value| value == "c"))
        .collect();
    files.sort();
    assert!(!files.is_empty(), "the corpus is empty");
    files
        .iter()
        .map(|path| std::fs::read_to_string(path).expect("draft"))
        .collect()
}

/// Run a command five times and keep the fastest.
///
/// A single shot is dominated by page-cache warmth and scheduler noise; the
/// first process measured looked twice as slow purely for being first.
fn best_of_five(build: impl Fn() -> Command) -> (std::time::Duration, std::process::Output) {
    let mut best = std::time::Duration::MAX;
    let mut last = None;
    for _ in 0..5 {
        let started = Instant::now();
        let output = build().output().expect("subprocess");
        best = best.min(started.elapsed());
        last = Some(output);
    }
    (best, last.expect("one run"))
}

fn whole_process() {
    let (typescript_elapsed, typescript) = best_of_five(|| {
        let mut command = Command::new("bun");
        command.arg("tools/lib/permute_m2c.ts").current_dir(root());
        command
    });
    assert!(typescript.status.success(), "the typescript run failed");

    let binary = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("target/release/permute-m2c");
    if !binary.exists() {
        println!("whole-process: release binary missing, run `cargo build --release` first");
        return;
    }
    let (rust_elapsed, rust) = best_of_five(|| {
        let mut command = Command::new(&binary);
        command.current_dir(root());
        command
    });
    assert!(rust.status.success(), "the rust run failed");
    assert_eq!(
        String::from_utf8_lossy(&typescript.stdout),
        String::from_utf8_lossy(&rust.stdout),
        "whole-process stdout differs"
    );
    println!(
        "whole-process: typescript {:.3}s, rust {:.3}s ({:.2}x)",
        typescript_elapsed.as_secs_f64(),
        rust_elapsed.as_secs_f64(),
        typescript_elapsed.as_secs_f64() / rust_elapsed.as_secs_f64()
    );
}

fn in_process(drafts: &[String]) {
    let started = Instant::now();
    let mut total = 0usize;
    for draft in drafts {
        for body in permute_m2c::candidates(draft) {
            total += body.len();
        }
    }
    let rust_elapsed = started.elapsed();
    println!(
        "in-process (rust): {} drafts, {} candidate bytes, {:.3}s",
        drafts.len(),
        total,
        rust_elapsed.as_secs_f64()
    );

    let script = r#"
const root = process.env.BENCH_ROOT;
const { candidates } = await import(root + "/tools/lib/permute_m2c.ts");
const { readdirSync, readFileSync } = await import("node:fs");
const directory = root + "/work/matches/m2c";
const drafts = readdirSync(directory)
  .filter((name) => name.endsWith(".c"))
  .sort()
  .map((name) => readFileSync(directory + "/" + name, "utf8"));
const started = Bun.nanoseconds();
let total = 0;
for (const draft of drafts) for (const body of candidates(draft)) total += body.length;
console.log(JSON.stringify({ drafts: drafts.length, total, seconds: (Bun.nanoseconds() - started) / 1e9 }));
"#;
    let output = Command::new("bun")
        .arg("-e")
        .arg(script)
        .env("BENCH_ROOT", root())
        .current_dir(root())
        .output()
        .expect("bun");
    assert!(output.status.success(), "the typescript in-process run failed");
    let text = String::from_utf8_lossy(&output.stdout);
    let document = permute_m2c::json::parse(text.trim()).expect("bench json");
    let seconds = document
        .get("seconds")
        .map(permute_m2c::json::Value::to_number)
        .unwrap_or(f64::NAN);
    println!(
        "in-process (typescript): {} drafts, {} candidate bytes, {:.3}s",
        document
            .get("drafts")
            .map(permute_m2c::json::Value::to_number)
            .unwrap_or(f64::NAN),
        document
            .get("total")
            .map(permute_m2c::json::Value::to_number)
            .unwrap_or(f64::NAN),
        seconds
    );
    println!(
        "in-process ratio (typescript / rust): {:.2}x",
        seconds / rust_elapsed.as_secs_f64()
    );
    // PORT NOTE: the candidate byte totals differ by construction only if the
    // ports diverge; the parity harness is what proves they do not. This line is
    // a smoke check, not the proof.
}

/// Where the in-process time goes on the Rust side, transform by transform.
fn transform_split(drafts: &[String]) {
    let mut rows: Vec<(&str, f64)> = Vec::new();
    macro_rules! time {
        ($label:literal, $call:expr) => {{
            let started = Instant::now();
            let mut sink = 0usize;
            for draft in drafts {
                sink += $call(draft).len();
            }
            std::hint::black_box(sink);
            rows.push(($label, started.elapsed().as_secs_f64()));
        }};
    }
    time!("lift_constants", permute_m2c::lift_constants);
    time!("volatile_hardware", permute_m2c::volatile_hardware);
    time!("post_increment_walk", permute_m2c::post_increment_walk);
    time!("thread_leading_argument", permute_m2c::thread_leading_argument);
    let started = Instant::now();
    let mut sink = 0usize;
    for draft in drafts {
        sink += permute_m2c::state_block(draft, false).len();
    }
    std::hint::black_box(sink);
    rows.push(("state_block", started.elapsed().as_secs_f64()));
    for (label, seconds) in rows {
        println!("  in-process split (rust): {label} {seconds:.3}s");
    }
}

fn main() {
    let drafts = corpus();
    in_process(&drafts);
    transform_split(&drafts);
    whole_process();
}
