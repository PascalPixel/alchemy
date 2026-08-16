//! CLI for `reverse-gcc296`.
//!
//!   reverse-gcc296 semantic/080bbb0c.c            # descend, report, write nothing
//!   reverse-gcc296 semantic/080bbb0c.c --apply    # write the improvement back
//!   reverse-gcc296 semantic/080bbb0c.c --rounds 5
//!   reverse-gcc296 --self-test

use std::path::PathBuf;
use std::process::ExitCode;

use reverse_gcc296::{default_jobs, repository_root, rewrite, score, Search};

const USAGE: &str = "\
Usage: reverse-gcc296 <source.c> [--apply] [--rounds N] [--jobs N]
       reverse-gcc296 --self-test

Systematic single-edit descent against the byte oracle. The objective is a
size-independent structural distance; the emitted size is reported but never
constrained.";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.is_empty() || args[0] == "-h" || args[0] == "--help" {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if args[0] == "--self-test" {
        return self_test();
    }

    let source_path = PathBuf::from(&args[0]);
    let mut apply = false;
    let mut rounds = 40usize;
    let mut jobs = default_jobs();
    let mut index = 1;
    while index < args.len() {
        match args[index].as_str() {
            "--apply" => apply = true,
            "--rounds" => {
                index += 1;
                rounds = args.get(index).and_then(|v| v.parse().ok()).unwrap_or(rounds);
            }
            "--jobs" => {
                index += 1;
                jobs = args.get(index).and_then(|v| v.parse().ok()).unwrap_or(jobs);
            }
            other => {
                eprintln!("unknown argument: {other}\n\n{USAGE}");
                return ExitCode::FAILURE;
            }
        }
        index += 1;
    }

    let root = repository_root();
    let absolute = if source_path.is_absolute() {
        source_path.clone()
    } else {
        root.join(&source_path)
    };
    let Some(stem) = absolute.file_stem().and_then(|s| s.to_str()).map(str::to_string) else {
        eprintln!("cannot determine owner stem from {}", absolute.display());
        return ExitCode::FAILURE;
    };
    let seed = match std::fs::read_to_string(&absolute) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("{}: {error}", absolute.display());
            return ExitCode::FAILURE;
        }
    };
    let Some(objdump) = score::find_objdump() else {
        eprintln!("no arm-none-eabi-objdump found");
        return ExitCode::FAILURE;
    };
    let candidate_show = root.join("tools/candidate-show/target/release/candidate-show");
    if !candidate_show.exists() {
        eprintln!("build candidate-show first: {}", candidate_show.display());
        return ExitCode::FAILURE;
    }

    let work = std::env::temp_dir().join(format!("reverse-gcc296-{stem}"));
    let search = Search {
        root: root.clone(),
        candidate_show,
        objdump,
        work,
        stem,
        jobs,
    };

    println!("jobs={} rounds<={}", search.jobs, rounds);
    let (best, history) = search.descend(&seed, rounds, |line| println!("{line}"));

    if history.is_empty() {
        println!("no improvement found");
        return ExitCode::SUCCESS;
    }
    println!("\naccepted {} edits:", history.len());
    for step in &history {
        println!("  {:<16} distance={:<6} size={}", step.label, step.distance, step.size);
    }
    if apply {
        if let Err(error) = std::fs::write(&absolute, &best) {
            eprintln!("{}: {error}", absolute.display());
            return ExitCode::FAILURE;
        }
        println!("written to {}", absolute.display());
    } else {
        println!("(dry run; pass --apply to write)");
    }
    ExitCode::SUCCESS
}

/// Checks that need no compiler, so the tool can be validated anywhere.
fn self_test() -> ExitCode {
    let source = "    if (a == 1) {\n        b = 2;\n    } else {\n        b = 3;\n    }";
    let variants = rewrite::arm_order_variants(source);
    if variants.len() != 1 {
        eprintln!("self-test: expected 1 arm variant, got {}", variants.len());
        return ExitCode::FAILURE;
    }
    if !variants[0].source.contains("if (!(a == 1)) {") {
        eprintln!("self-test: condition was not negated");
        return ExitCode::FAILURE;
    }
    // `a = 1; b = 2; c = a;` has two legal swaps: `b = 2` is independent of both
    // neighbours. The illegal one is exchanging `a = 1` with `c = a`, which are
    // not adjacent.
    let statements = "    a = 1;\n    b = 2;\n    c = a;";
    let swaps = rewrite::statement_order_variants(statements);
    if swaps.len() != 2 {
        eprintln!("self-test: expected 2 legal statement swaps, got {}", swaps.len());
        return ExitCode::FAILURE;
    }
    // A genuine hazard must still be refused.
    let hazard = rewrite::statement_order_variants("    a = 1;\n    b = a;");
    if !hazard.is_empty() {
        eprintln!("self-test: read-after-write was not refused");
        return ExitCode::FAILURE;
    }
    if score::canonicalise("movs\tr1, #1") != score::canonicalise("movs\tr7, #1") {
        eprintln!("self-test: registers not blinded");
        return ExitCode::FAILURE;
    }
    println!("self-test ok");
    ExitCode::SUCCESS
}
