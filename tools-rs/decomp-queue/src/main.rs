// CLI for the exact-C search queue, ported from tools/decomp/decomp_queue.ts.
//
// WHY THIS FILE EXISTS
// There are hundreds of half-matched functions and only so many hours; this
// binary ranks them by how much progress a unit of work is likely to buy. It
// re-reads the previous report so unchanged candidates keep their diagnosis
// instead of being recompiled, which is what makes the queue resumable.
//
// PORT NOTE: the compile-and-link step still runs through
// `tools/lib/match_m2c.ts`, because `verifyCandidate` sits on
// `tools/lib/alchemy_gcc.ts` (the compiler routing layer), which is being
// ported separately. Like tools-rs/decomp-diagnose, this binary shells out to a
// generated Bun bridge for that one step. When an `alchemy-gcc` crate lands,
// the bridge is the only thing that has to go.
//
// PORT NOTE: `diagnoseCandidate` and its bridge live in decomp-diagnose's
// *binary*, not its library, so they are reproduced here rather than imported.
// Everything else comes from `decomp_diagnose` as a path dependency.
//
// PORT NOTE: on failure Bun prints `error: <message>` plus a JS stack trace and
// a version banner. This binary prints `error: <message>` and exits 1; the exit
// code and the message line match, the stack trace does not exist.

use std::cmp::Ordering;
use std::collections::HashMap;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering as AtomicOrdering};
use std::sync::Mutex;

use decomp_diagnose::json::{self, Value};
use decomp_diagnose::{
    assembly_sizes, basename, diagnose_instructions, disassemble, lookup, repo_root,
    CandidateDiagnosis, ROM_BASE,
};
use decomp_queue::{
    arguments_of, best_body, best_score, canonical_json, expected_value, js, queue_item,
    retained_assembly_stems, wyhash, Options, Parsed, M2C_PREAMBLE, MAX_SAFE_INTEGER,
};

/// PORT NOTE: the TypeScript derives ROOT from `import.meta.url`, so pointing
/// it at a different tree is a matter of copying `tools/` there. The Rust
/// mirror's root is baked in at compile time by `repo_root()`, so it gets the
/// same knob the `permute-m2c` port uses: one environment variable that
/// DEFAULTS to the production path. Production sets nothing and both mirrors
/// resolve to the repository; the parity harness sets this and
/// `PERMUTE_M2C_ASM` to the same fixture root.
pub fn queue_root() -> std::path::PathBuf {
    match std::env::var("DECOMP_QUEUE_ROOT") {
        Ok(value) if !value.is_empty() => std::path::PathBuf::from(value),
        _ => repo_root().to_path_buf(),
    }
}

fn main() {
    if let Err(message) = run() {
        eprintln!("error: {message}");
        std::process::exit(1);
    }
}

struct Target {
    stem: String,
    state: Value,
    source: PathBuf,
    hash: String,
}

fn run() -> Result<(), String> {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    // `navigator.hardwareConcurrency` is the logical core count under Bun.
    let cores = std::thread::available_parallelism().map_or(0.0, |value| value.get() as f64);
    let options = match arguments_of(&argv, cores)? {
        Parsed::Help(usage) => {
            println!("{usage}");
            return Ok(());
        }
        Parsed::Options(options) => options,
    };
    let root = queue_root();
    let out = root.join("out").join("decomp");
    std::fs::create_dir_all(&out).map_err(|error| error.to_string())?;
    let report_path = out.join("queue.json");

    let previous = if !options.refresh && report_path.exists() {
        Some(json::parse(&read_utf8(&report_path)?)?)
    } else {
        None
    };
    let mut cached: HashMap<String, Value> = HashMap::new();
    if let Some(Value::Array(items)) = previous.as_ref().and_then(|report| report.get("items")) {
        for item in items {
            let stem = string_field(item, "stem");
            let hash = string_field(item, "source_hash");
            cached.insert(format!("{stem}:{hash}"), item.clone());
        }
    }

    let retained = retained_assembly_stems()?;
    let states = root.join("out").join("permute1").join("state");
    let mut names: Vec<String> = std::fs::read_dir(&states)
        .map_err(|error| format!("ENOENT: {}: {error}", states.display()))?
        .map(|entry| {
            entry
                .map(|item| item.file_name().to_string_lossy().into_owned())
                .map_err(|error| error.to_string())
        })
        .collect::<Result<Vec<String>, String>>()?
        .into_iter()
        .filter(|name| name.ends_with(".json"))
        .collect();
    // PORT NOTE: `Array#sort` with no comparator orders by UTF-16 code unit.
    // These names are ASCII, where that coincides with Rust's byte order.
    names.sort_unstable();

    let mut pending: Vec<Target> = Vec::new();
    for name in names {
        let stem = basename(&name, ".json");
        if retained.contains(&stem) || !root.join("asm").join(format!("{stem}.s")).exists() {
            continue;
        }
        let state = json::parse(&read_utf8(&states.join(&name))?)?;
        let Some(source) = candidate_for(&out, &root, &stem, &state)? else {
            continue;
        };
        let hash = wyhash::base36(wyhash::bun_hash(
            &std::fs::read(&source).map_err(|error| error.to_string())?,
        ));
        pending.push(Target {
            stem,
            state,
            source,
            hash,
        });
    }

    // PORT NOTE: `Array#sort` is stable per spec, and a comparator returning
    // NaN (`Infinity - Infinity`, i.e. two states with no best score) is
    // treated as 0. `sort_by` is stable and the NaN case maps to `Equal`, so
    // ties keep the filename order established above.
    pending.sort_by(|left, right| {
        let difference = best_score(&left.state, f64::INFINITY) - best_score(&right.state, f64::INFINITY);
        sign_ordering(difference)
    });
    let limit = options.limit.min(pending.len() as f64) as usize;
    let selected = &pending[..limit];

    let rom_path = root.join("roms").join("gs1-en.gba");
    let rom = std::fs::read(&rom_path)
        .map_err(|error| format!("ENOENT: {}: {error}", rom_path.display()))?;

    let items = run_workers(&options, selected, &cached, &rom, &rom_path, &out, &root);

    let mut ranked: Vec<Value> = items.into_iter().flatten().collect();
    ranked.sort_by(|left, right| {
        let difference = number_field(right, "expected_value") - number_field(left, "expected_value");
        match sign_ordering(difference) {
            // PORT NOTE: the tiebreak is `localeCompare`, which is ICU
            // collation. Stems are eight lowercase hex digits, where ICU root
            // collation and byte order agree (digits before letters in both).
            Ordering::Equal => string_field(left, "stem").cmp(&string_field(right, "stem")),
            other => other,
        }
    });

    let report = Value::Object(vec![
        ("format".to_string(), Value::Number(1.0)),
        (
            "generated_at".to_string(),
            Value::String(js::iso_timestamp(now_millis())),
        ),
        ("items".to_string(), Value::Array(ranked.clone())),
    ]);
    std::fs::write(&report_path, format!("{}\n", canonical_json(&report)))
        .map_err(|error| error.to_string())?;
    if let Some(targets_out) = &options.targets_out {
        let stems: Vec<String> = ranked.iter().map(|item| string_field(item, "stem")).collect();
        std::fs::write(targets_out, format!("{}\n", stems.join("\n")))
            .map_err(|error| error.to_string())?;
    }

    let plateaus = ranked
        .iter()
        .filter(|item| matches!(item.get("plateau"), Some(Value::Bool(true))))
        .count();
    let register_led = ranked
        .iter()
        .filter(|item| {
            item.get("diagnosis")
                .map(|diagnosis| number_field(diagnosis, "register_fraction") >= 0.5)
                .unwrap_or(false)
        })
        .count();
    println!(
        "queued={} plateaus={plateaus} register_led={register_led} report={}",
        ranked.len(),
        report_path.display()
    );
    for item in ranked.iter().take(20) {
        let diagnosis = item.get("diagnosis").cloned().unwrap_or(Value::Null);
        println!(
            "{}\tvalue={}\tbytes={}\t{}",
            string_field(item, "stem"),
            js::to_fixed_1(number_field(item, "expected_value")),
            js::number_to_string(number_field(&diagnosis, "byte_mismatches")),
            string_field(&diagnosis, "dominant"),
        );
    }
    Ok(())
}

/// The async worker pool: `Math.min(jobs, selected.length)` workers pulling
/// from a shared cursor and writing into a pre-sized, index-keyed array.
///
/// PORT NOTE: the JS pool is cooperative concurrency over one thread; this is
/// real parallelism over OS threads. The index-keyed result array is what makes
/// that safe — the output order is the selection order either way, never the
/// completion order.
fn run_workers(
    options: &Options,
    selected: &[Target],
    cached: &HashMap<String, Value>,
    rom: &[u8],
    rom_path: &Path,
    out: &Path,
    root: &Path,
) -> Vec<Option<Value>> {
    let slots: Vec<Mutex<Option<Value>>> = (0..selected.len()).map(|_| Mutex::new(None)).collect();
    let cursor = AtomicUsize::new(0);
    let workers = options.jobs.min(selected.len() as f64) as usize;
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = cursor.fetch_add(1, AtomicOrdering::SeqCst);
                if index >= selected.len() {
                    return;
                }
                let target = &selected[index];
                if let Some(hit) = cached.get(&format!("{}:{}", target.stem, target.hash)) {
                    *slots[index].lock().expect("no worker panics") = Some(hit.clone());
                    continue;
                }
                let scratch = out.join("work").join(&target.stem);
                match diagnose_candidate(&target.source, rom, rom_path, &scratch, root) {
                    Ok(diagnosis) => {
                        let priority = expected_value(&diagnosis, &target.state);
                        let candidate = relative_to(root, &target.source);
                        let item = queue_item(
                            &target.stem,
                            &candidate,
                            &target.hash,
                            best_score(&target.state, MAX_SAFE_INTEGER),
                            priority,
                            &diagnosis,
                        );
                        *slots[index].lock().expect("no worker panics") = Some(item);
                    }
                    Err(message) => {
                        // A single `writeln!` so parallel skips stay on their
                        // own lines; the JS gets that for free from `console.error`.
                        let mut stderr = std::io::stderr().lock();
                        let _ = writeln!(stderr, "skip {}: {message}", target.stem);
                    }
                }
            });
        }
    });
    slots
        .into_iter()
        .map(|slot| slot.into_inner().expect("no worker panics"))
        .collect()
}

/// `candidateFor(stem, state)` — materialise the C the diagnosis will compile.
fn candidate_for(
    out: &Path,
    root: &Path,
    stem: &str,
    state: &Value,
) -> Result<Option<PathBuf>, String> {
    let directory = out.join("candidates");
    std::fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    let output = directory.join(format!("{stem}.c"));
    if let Some(body) = best_body(state) {
        // PORT NOTE: the guard is `startsWith("typedef")`, so a body that
        // already carries the preamble is written through unchanged.
        let text = if body.starts_with("typedef") {
            body.to_string()
        } else {
            format!("{M2C_PREAMBLE}{body}")
        };
        std::fs::write(&output, text).map_err(|error| error.to_string())?;
        return Ok(Some(output));
    }
    let brute = root
        .join("out")
        .join("brute")
        .join("best")
        .join(format!("{stem}.c"));
    if brute.exists() {
        let bytes = std::fs::read(&brute).map_err(|error| error.to_string())?;
        std::fs::write(&output, bytes).map_err(|error| error.to_string())?;
        return Ok(Some(output));
    }
    Ok(None)
}

/// `path.slice(ROOT.length + 1)`.
///
/// PORT NOTE: `String#slice` counts UTF-16 units and clamps; repository paths
/// here are ASCII, so a byte offset is the same offset.
fn relative_to(root: &Path, path: &Path) -> String {
    let root = root.display().to_string();
    let path = path.display().to_string();
    match path.get(root.len() + 1..) {
        Some(rest) => rest.to_string(),
        None => String::new(),
    }
}

/// A JS comparator's sign, with NaN meaning "equal" as `Array#sort` treats it.
fn sign_ordering(difference: f64) -> Ordering {
    if difference < 0.0 {
        Ordering::Less
    } else if difference > 0.0 {
        Ordering::Greater
    } else {
        Ordering::Equal
    }
}

fn number_field(value: &Value, key: &str) -> f64 {
    match value.get(key) {
        Some(Value::Number(number)) => *number,
        _ => f64::NAN,
    }
}

fn string_field(value: &Value, key: &str) -> String {
    match value.get(key) {
        Some(Value::String(text)) => text.clone(),
        _ => String::new(),
    }
}

/// `readFileSync(path, "utf8")` — lossy, never an encoding error.
fn read_utf8(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("ENOENT: {}: {error}", path.display()))
}

fn now_millis() -> i64 {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map(|elapsed| elapsed.as_millis() as i64)
        .unwrap_or(0)
}

// ---------------------------------------------------------------------------
// decomp_diagnose.ts: diagnoseCandidate, reproduced from that crate's binary
// ---------------------------------------------------------------------------

/// `Buffer#subarray` index clamping, including the negative-index rewrite.
fn js_subarray(length: usize, start: i64, end: i64) -> (usize, usize) {
    let resolve = |index: i64| -> usize {
        if index < 0 {
            (length as i64 + index).max(0) as usize
        } else {
            (index as usize).min(length)
        }
    };
    let begin = resolve(start);
    let finish = resolve(end);
    (begin, finish.max(begin))
}

fn diagnose_candidate(
    source: &Path,
    rom: &[u8],
    rom_path: &Path,
    scratch: &Path,
    root: &Path,
) -> Result<CandidateDiagnosis, String> {
    std::fs::create_dir_all(scratch).map_err(|error| error.to_string())?;
    let stem = basename(&source.display().to_string(), ".c");
    let verification = verify_candidate(source, rom_path, scratch, root)?;
    let address =
        i64::from_str_radix(&stem, 16).map_err(|_| format!("invalid hexadecimal value: {stem}"))?;
    let target_size =
        lookup(&assembly_sizes(root), &stem).unwrap_or(verification.expected_length);
    let (begin, finish) = js_subarray(
        rom.len(),
        address - ROM_BASE,
        address - ROM_BASE + target_size,
    );
    let target = &rom[begin..finish];
    let actual = disassemble(
        &verification.actual,
        address,
        &scratch.join(format!("{stem}.actual.bin")),
    )?;
    let expected = disassemble(target, address, &scratch.join(format!("{stem}.expected.bin")))?;
    let mut bytes = (verification.actual.len() as i64 - target.len() as i64).abs();
    bytes += verification
        .actual
        .iter()
        .zip(target.iter())
        .filter(|(left, right)| left != right)
        .count() as i64;
    Ok(diagnose_instructions(
        &stem,
        &actual,
        &expected,
        verification.actual.len() as i64,
        target.len() as i64,
        bytes,
    ))
}

struct Verification {
    actual: Vec<u8>,
    expected_length: i64,
}

const BRIDGE: &str = r#"// Generated by tools-rs/decomp-queue; safe to delete.
import { verifyCandidate } from "@ROOT@/tools/lib/match_m2c.ts";
import { readFileSync, writeFileSync } from "node:fs";
const [source, romPath, scratch, out] = Bun.argv.slice(2);
try {
  const verification = await verifyCandidate(source, readFileSync(romPath), scratch);
  writeFileSync(out, verification.actual);
  process.stdout.write(String(verification.expected.length));
} catch (error) {
  writeFileSync(out + ".error", String(error?.message));
  process.exit(3);
}
"#;
fn verify_candidate(
    source: &Path,
    rom_path: &Path,
    scratch: &Path,
    root: &Path,
) -> Result<Verification, String> {
    let bridge = scratch.join("verify_bridge.ts");
    std::fs::write(
        &bridge,
        BRIDGE.replace("@ROOT@", &root.display().to_string()),
    )
    .map_err(|error| error.to_string())?;
    let actual_path = scratch.join("verify_bridge.actual.bin");
    let error_path = scratch.join("verify_bridge.actual.bin.error");
    // A stale sidecar from a previous run would masquerade as this run's error.
    let _ = std::fs::remove_file(&error_path);
    let rom_path = std::fs::canonicalize(rom_path).map_err(|error| error.to_string())?;
    let output = Command::new("bun")
        .arg(&bridge)
        .arg(source)
        .arg(&rom_path)
        .arg(scratch)
        .arg(&actual_path)
        .current_dir(root)
        .output()
        .map_err(|error| format!("bun could not be started: {error}"))?;
    if !output.status.success() {
        // PORT NOTE: the TypeScript catches the throw in-process and prints
        // `(error as Error).message`, which is routinely multi-line (a compiler
        // diagnostic). Bun's own `error: ...` banner cannot be parsed back into
        // that message, so the bridge catches and writes it to a sidecar file
        // instead. `String(error?.message)` reproduces the `undefined` a
        // non-Error throw would produce.
        return Err(match std::fs::read(&error_path) {
            Ok(bytes) => String::from_utf8_lossy(&bytes).into_owned(),
            Err(_) => {
                eprint!("{}", String::from_utf8_lossy(&output.stderr));
                "verifyCandidate failed".to_string()
            }
        });
    }
    let expected_length: i64 = String::from_utf8_lossy(&output.stdout)
        .trim()
        .parse()
        .map_err(|_| "bridge produced no expected length".to_string())?;
    let actual = std::fs::read(&actual_path).map_err(|error| error.to_string())?;
    Ok(Verification {
        actual,
        expected_length,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn subarray_indices_clamp_and_accept_negatives_like_javascript() {
        assert_eq!(js_subarray(10, 2, 5), (2, 5));
        assert_eq!(js_subarray(10, 8, 40), (8, 10));
        assert_eq!(js_subarray(10, -3, 40), (7, 10));
        assert_eq!(js_subarray(10, 6, 3), (6, 6));
    }

    #[test]
    fn comparator_signs_follow_javascript_sort() {
        assert_eq!(sign_ordering(-1.0), Ordering::Less);
        assert_eq!(sign_ordering(1.0), Ordering::Greater);
        assert_eq!(sign_ordering(0.0), Ordering::Equal);
        // Infinity - Infinity is NaN, which Array#sort treats as 0.
        assert_eq!(
            sign_ordering(f64::INFINITY - f64::INFINITY),
            Ordering::Equal
        );
    }

    #[test]
    fn ranking_orders_by_value_then_stem() {
        let item = |stem: &str, value: f64| {
            Value::Object(vec![
                ("stem".to_string(), Value::String(stem.to_string())),
                ("expected_value".to_string(), Value::Number(value)),
            ])
        };
        let mut ranked = [
            item("08000002", 10.0),
            item("08000001", 10.0),
            item("08000003", 99.0),
        ];
        ranked.sort_by(|left, right| {
            let difference =
                number_field(right, "expected_value") - number_field(left, "expected_value");
            match sign_ordering(difference) {
                Ordering::Equal => string_field(left, "stem").cmp(&string_field(right, "stem")),
                other => other,
            }
        });
        let stems: Vec<String> = ranked.iter().map(|item| string_field(item, "stem")).collect();
        assert_eq!(stems, ["08000003", "08000001", "08000002"]);
    }

    #[test]
    fn relative_paths_drop_the_root_prefix() {
        let root = Path::new("/repo");
        assert_eq!(
            relative_to(root, Path::new("/repo/out/decomp/candidates/a.c")),
            "out/decomp/candidates/a.c"
        );
    }

}
