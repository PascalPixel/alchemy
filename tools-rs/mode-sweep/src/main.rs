//! CLI for the ported half of `tools/lib/mode_sweep.ts`.
//!
//! `--self-test` and the planning subcommands are complete. The scoring run is
//! NOT: it needs `verifyCandidate` from `tools/lib/match_m2c.ts`, which needs
//! `sourceToAssemblyPlan` / `externalSymbol` / `externalSymbolAssembly` from
//! `tools/lib/alchemy_gcc.ts`. That file is out of bounds for this port and has
//! no CLI entry point to shell out to, so `--run` reports the blocker instead of
//! pretending to sweep. `tools/lib/mode_sweep.ts` remains the only thing that
//! can actually compile a sweep today.

use std::process::ExitCode;

use mode_sweep::{
    self_test, single_configs, pair_configs, historical_family_factorial_configs, triple_configs,
    compiler_signature, root, Config, OptionsOutcome, options_of, SELF_TEST_CHECK_FLOOR, USAGE,
};
use search_compiler_modes::{canonical_json, Json};

fn main() -> ExitCode {
    let argv: Vec<String> = std::env::args().skip(1).collect();

    if argv.iter().any(|a| a == "--self-test") {
        return match self_test() {
            Ok(checks) if checks >= SELF_TEST_CHECK_FLOOR => {
                println!("mode sweep self-test passed ({checks} checks)");
                ExitCode::SUCCESS
            }
            Ok(checks) => {
                eprintln!(
                    "mode sweep self-test executed only {checks} checks, below the floor of {SELF_TEST_CHECK_FLOOR}"
                );
                ExitCode::FAILURE
            }
            Err(message) => {
                eprintln!("mode sweep self-test failed: {message}");
                ExitCode::FAILURE
            }
        };
    }

    // Planning-only inspection modes. They exist because the plan is the half
    // of mode_sweep that is portable, and being able to dump it is what makes
    // the port checkable against the TypeScript.
    if let Some(plan) = argv.iter().find_map(|a| a.strip_prefix("--plan=")) {
        let configs: Vec<Config> = match plan {
            "singles" => single_configs(),
            "pairs" => pair_configs(None),
            "family-factorial" => historical_family_factorial_configs(),
            other if other.starts_with("triples:") => {
                let seeds: Vec<String> = other["triples:".len()..]
                    .split(',')
                    .map(str::to_string)
                    .collect();
                triple_configs(&seeds, None)
            }
            other => {
                eprintln!("unknown plan: {other}");
                return ExitCode::FAILURE;
            }
        };
        println!(
            "{}",
            canonical_json(&Json::Array(configs.iter().map(Config::to_json).collect()))
        );
        return ExitCode::SUCCESS;
    }

    if argv.iter().any(|a| a == "--compiler-signature") {
        return match compiler_signature(&root()) {
            Ok(signature) => {
                println!("{signature}");
                ExitCode::SUCCESS
            }
            Err(error) => {
                // Expected today: mode_sweep.ts:466 looks for five modules in
                // tools/ that live in tools/lib/. Reproduced deliberately.
                eprintln!("compiler signature failed: {error}");
                ExitCode::FAILURE
            }
        };
    }

    match options_of(&argv, &root(), std::thread::available_parallelism().ok().map(|n| n.get() as u32)) {
        Ok(OptionsOutcome::Help) => {
            println!("{USAGE}");
            println!("  --self-test              run the invariant suite");
            println!("  --plan=singles|pairs|family-factorial|triples:a,b");
            println!("  --compiler-signature     hash the compiler bundle plus sibling sources");
            ExitCode::SUCCESS
        }
        Ok(OptionsOutcome::Parsed(options)) => {
            eprintln!(
                "mode-sweep (Rust) parses and plans but cannot score {}.",
                options.source
            );
            eprintln!(
                "The scoring loop needs verifyCandidate -> alchemy_gcc.ts, which is not ported."
            );
            eprintln!("Run the sweep with: bun tools/lib/mode_sweep.ts {}", options.source);
            ExitCode::FAILURE
        }
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
