//! Verification gates for the AST randomization engine.
//!
//! Three gates, invoked as hidden flags of `compiler permute`:
//!   --ast-gate-determinism <file>  same seed -> same mutation stream
//!   --ast-gate-validity <file>     compile rate of 500 single mutations
//!   --ast-gate-reach <file>        perm_temp_for_expr can re-derive the
//!                                  `power = range * 4` reuse in the
//!                                  hand-undone DK_PP_DMG variant

use std::collections::hash_map::DefaultHasher;
use std::hash::{Hash, Hasher};
use std::path::PathBuf;
use std::process::Command;
use std::sync::atomic::{AtomicUsize, Ordering};

use crate::astpass::{AstPass, AstRandomizer};

fn read(path: &str) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("read {path}: {e}"))
}

fn arg_value(args: &[String], name: &str) -> Option<String> {
    args.iter()
        .position(|a| a == name)
        .and_then(|i| args.get(i + 1))
        .cloned()
}

fn hash_str(s: &str) -> u64 {
    let mut h = DefaultHasher::new();
    s.hash(&mut h);
    h.finish()
}

/// Debug helper: print N mutations for one seed.
pub fn run_ast_randomize(path: &str, args: &[String]) -> Result<(), String> {
    let source = read(path)?;
    let seed: u64 = arg_value(args, "--seed")
        .map(|s| s.parse().map_err(|_| "bad --seed".to_string()))
        .transpose()?
        .unwrap_or(0);
    let count: usize = arg_value(args, "--count")
        .map(|s| s.parse().map_err(|_| "bad --count".to_string()))
        .transpose()?
        .unwrap_or(1);
    let only = arg_value(args, "--pass")
        .map(|name| AstPass::from_name(&name).ok_or(format!("unknown pass {name}")))
        .transpose()?;
    let mut r = AstRandomizer::new(&source, seed, only)?;
    for _ in 0..count {
        let out = r.randomize()?;
        print!("{out}");
        println!("/* ---- mutation boundary ---- */");
    }
    Ok(())
}

/// Gate (a): two independent runs over seeds 0..10, 20 stacked mutations
/// each, must produce identical hash streams.
pub fn gate_determinism(path: &str) -> Result<(), String> {
    let source = read(path)?;
    let stream = |source: &str| -> Result<Vec<u64>, String> {
        let mut hashes = Vec::new();
        for seed in 0..10u64 {
            let mut r = AstRandomizer::new(source, seed, None)?;
            for _ in 0..20 {
                let out = r.randomize()?;
                hashes.push(hash_str(&out));
            }
        }
        Ok(hashes)
    };
    let a = stream(&source)?;
    let b = stream(&source)?;
    if a != b {
        return Err("determinism gate FAILED: streams differ".to_string());
    }
    let mut h = DefaultHasher::new();
    a.hash(&mut h);
    println!(
        "determinism=ok mutations={} stream_hash={:016x}",
        a.len(),
        h.finish()
    );
    Ok(())
}

/// Gate (b): seeds 0..N (default 500), one mutation each, compiled with
/// the project's gcc 2.96 at the standard flags. Reports the compile rate.
pub fn gate_validity(path: &str, args: &[String]) -> Result<(), String> {
    let source = read(path)?;
    let count: usize = arg_value(args, "--count")
        .map(|s| s.parse().map_err(|_| "bad --count".to_string()))
        .transpose()?
        .unwrap_or(500);
    let gcc = arg_value(args, "--gcc").unwrap_or_else(|| "alchemy-gcc/dist/xgcc".to_string());
    // gcc -B needs the trailing slash to treat the value as a directory.
    let bdir = PathBuf::from(&gcc)
        .parent()
        .map(|p| format!("{}/", p.display()))
        .unwrap_or_else(|| "./".to_string());
    let workdir = PathBuf::from("scratch/astgate");
    std::fs::create_dir_all(&workdir).map_err(|e| format!("mkdir scratch/astgate: {e}"))?;

    let ok = AtomicUsize::new(0);
    let mutate_fail = AtomicUsize::new(0);
    let compile_fail = AtomicUsize::new(0);
    let next = AtomicUsize::new(0);
    let threads = 8usize;
    std::thread::scope(|scope| {
        for _ in 0..threads {
            scope.spawn(|| loop {
                let seed = next.fetch_add(1, Ordering::SeqCst);
                if seed >= count {
                    break;
                }
                let out = AstRandomizer::new(&source, seed as u64, None)
                    .and_then(|mut r| r.randomize());
                let out = match out {
                    Ok(o) => o,
                    Err(_) => {
                        mutate_fail.fetch_add(1, Ordering::SeqCst);
                        continue;
                    }
                };
                let file = workdir.join(format!("v{seed}.c"));
                if std::fs::write(&file, &out).is_err() {
                    mutate_fail.fetch_add(1, Ordering::SeqCst);
                    continue;
                }
                let status = Command::new(&gcc)
                    .arg("-B")
                    .arg(&bdir)
                    .args([
                        "-O2",
                        "-mthumb",
                        "-mthumb-interwork",
                        "-mcpu=arm7tdmi",
                        "-fno-builtin",
                        "-nostdinc",
                        "-ffreestanding",
                        "-fcall-used-r4",
                        "-S",
                    ])
                    .arg(&file)
                    .args(["-o", "/dev/null"])
                    .output();
                match status {
                    Ok(o) if o.status.success() => {
                        ok.fetch_add(1, Ordering::SeqCst);
                        let _ = std::fs::remove_file(&file);
                    }
                    _ => {
                        compile_fail.fetch_add(1, Ordering::SeqCst);
                    }
                }
            });
        }
    });
    let ok = ok.load(Ordering::SeqCst);
    let mfail = mutate_fail.load(Ordering::SeqCst);
    let cfail = compile_fail.load(Ordering::SeqCst);
    println!(
        "validity: total={count} compiled_ok={ok} compile_fail={cfail} mutate_fail={mfail} rate={:.1}%",
        100.0 * ok as f64 / count as f64
    );
    Ok(())
}

/// Gate (c): reachability of the existing-variable-reuse machinery. On the
/// hand-undone variant, perm_temp_for_expr must at least once emit an
/// assignment of `range * 4` to the existing variable `power`, with the
/// DK_PP_DMG occurrence rewritten to read `power`.
pub fn gate_reach(path: &str, args: &[String]) -> Result<(), String> {
    let source = read(path)?;
    let attempts: usize = arg_value(args, "--attempts")
        .map(|s| s.parse().map_err(|_| "bad --attempts".to_string()))
        .transpose()?
        .unwrap_or(20000);
    let hits = AtomicUsize::new(0);
    let assigns = AtomicUsize::new(0);
    let fails = AtomicUsize::new(0);
    let next = AtomicUsize::new(0);
    let mut sample: std::sync::Mutex<Option<String>> = std::sync::Mutex::new(None);
    let threads = 8usize;
    std::thread::scope(|scope| {
        for _ in 0..threads {
            scope.spawn(|| loop {
                let seed = next.fetch_add(1, Ordering::SeqCst);
                if seed >= attempts {
                    break;
                }
                let out = AstRandomizer::new(&source, seed as u64, Some(AstPass::TempForExpr))
                    .and_then(|mut r| r.randomize());
                let out = match out {
                    Ok(o) => o,
                    Err(_) => {
                        fails.fetch_add(1, Ordering::SeqCst);
                        continue;
                    }
                };
                let assigned = out.contains("power = (range * 4)");
                if assigned {
                    assigns.fetch_add(1, Ordering::SeqCst);
                }
                if assigned && out.contains("(power + 72)") {
                    hits.fetch_add(1, Ordering::SeqCst);
                    let mut guard = sample.lock().unwrap();
                    if guard.is_none() {
                        *guard = Some(format!("seed={seed}"));
                    }
                }
            });
        }
    });
    let hits = hits.load(Ordering::SeqCst);
    let assigns = assigns.load(Ordering::SeqCst);
    println!(
        "reach: attempts={attempts} power_assigns={assigns} full_hits={hits} mutate_fail={} first={}",
        fails.load(Ordering::SeqCst),
        sample.get_mut().unwrap().clone().unwrap_or_else(|| "-".to_string())
    );
    if hits == 0 {
        return Err("reachability gate FAILED: no hit".to_string());
    }
    Ok(())
}

pub fn self_test() -> Result<(), String> {
    let src = "int g; int f(int a) { int x; x = a * 4 + 72; return x + g; }";
    let mut r = AstRandomizer::new(src, 1, None)?;
    let out = r.randomize()?;
    if out.is_empty() {
        return Err("astgate self-test: empty mutation".to_string());
    }
    Ok(())
}
