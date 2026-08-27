use std::process::ExitCode;

const USAGE: &str = "usage: check <publication|commit-progress|owners|retained|coverage|integrate|no-asm|progress|routes|doctor> [args]";

const ROM_GAMES: [&str; 2] = ["gs1", "gs2"];
const ROM_EDITIONS: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];

fn doctor() -> ExitCode {
    use compiler_core::bundle::{validate_agbcc_bundle, validate_bundle};
    use compiler_core::routing::CompilerTarget;
    let mut ok = true;
    for target in [CompilerTarget::Gs1, CompilerTarget::Gs2] {
        match validate_bundle(target) {
            Ok(()) => println!("ok: alchemy-gcc {} bundle", target.as_str()),
            Err(message) => {
                println!("fail: {message}");
                ok = false;
            }
        }
    }
    match validate_agbcc_bundle() {
        Ok(()) => println!("ok: alchemy-gcc agbcc bundle"),
        Err(message) => {
            println!("fail: {message}");
            ok = false;
        }
    }
    for game in ROM_GAMES {
        for edition in ROM_EDITIONS {
            let path = std::path::Path::new("roms").join(format!("{game}-{edition}.gba"));
            match std::fs::metadata(&path) {
                Ok(meta) if meta.len() > 0 => println!("ok: {}", path.display()),
                Ok(_) => {
                    println!("fail: {} is empty", path.display());
                    ok = false;
                }
                Err(_) => {
                    println!("fail: {} is missing", path.display());
                    ok = false;
                }
            }
        }
    }
    if ok {
        println!("doctor ok: roms, bundle, and toolchain all pass");
        ExitCode::SUCCESS
    } else {
        println!("doctor failed: see fail lines above");
        ExitCode::FAILURE
    }
}

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "publication" | "check-publication" => check_publication::cli::entry(rest),
        "commit-progress" | "check-commit-progress" => check_commit_progress::cli::entry(rest),
        "owners" | "check-unmatchable" => check_unmatchable::cli::entry(rest),
        "retained" | "core-retained-audit" => core_retained_audit::cli::entry(rest),
        "coverage" | "coverage-map" => {
            coverage_map::entrypoint::entry(rest);
            ExitCode::SUCCESS
        }
        "integrate" | "integrate-matches" => integrate_matches::entry(rest),
        "no-asm" | "no-asm-c" => no_asm_c::cli::entry(rest),
        "progress" => {
            full_c_progress::entry(rest);
            ExitCode::SUCCESS
        }
        "routes" | "route-dump" => route_dump::entrypoint::entry(rest),
        "doctor" => doctor(),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown check command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}
