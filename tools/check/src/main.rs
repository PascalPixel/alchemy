//! `check`: one executable for the audit, census and verification tools.
//!
//! Seventh consolidation. See docs/ENTRY-POINTS.md.

use std::process::ExitCode;

const USAGE: &str = "usage: check <command> [args]\n       check --list";
const COMMANDS: &[(&str, &str)] = &[
    ("cache-entry", "cache entry"),
    ("check-publication", "check publication"),
    ("check-sanctum", "check sanctum"),
    ("core-retained-audit", "core retained audit"),
    ("decomp-constraints", "decomp constraints"),
    ("decomp-targets", "decomp targets"),
    ("discover", "discover"),
    ("executable-gap-sources", "executable gap sources"),
    ("extract-resource", "extract resource"),
    ("late-runtime-data", "late runtime data"),
    ("music-residuals", "music residuals"),
    ("semantic-owner-scope", "semantic owner scope"),
    ("semantic-superseded", "semantic superseded"),
    ("source-citations", "source citations"),
    ("cache-key-lint", "cache key lint"),
    ("no-asm-c", "no asm c"),
    ("compare-roms", "compare roms"),
    ("semantic-queue", "semantic queue"),
    ("lang-ban", "lang ban"),
    ("check-commit-progress", "check commit progress"),
    ("coverage-map", "refresh the dashboard coverage maps"),
    ("byte-value-regions", "byte value regions"),
    ("architecture", "architecture"),
    ("alchemy-lints", "alchemy lints"),
    ("jobs", "jobs"),
    ("integrate-matches", "integrate matched candidates into the tree"),
];

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}"); list(); return ExitCode::from(2);
    };
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:"); list(); return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "integrate-matches" => { integrate_matches::entry_main::entry(&rest); ExitCode::SUCCESS }

        "architecture" => { architecture::cli::entry(&rest); ExitCode::SUCCESS }
        "alchemy-lints" => { alchemy_lints::cli::entry(&rest); ExitCode::SUCCESS }
        "jobs" => { jobs::cli::entry(&rest); ExitCode::SUCCESS }
        "byte-value-regions" => { byte_value_regions::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "coverage-map" => { coverage_map::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "lang-ban" => lang_ban::cli::entry(&rest),
        "check-commit-progress" => check_commit_progress::cli::entry(&rest),
        "compare-roms" => { compare_roms::cli::entry(&rest); ExitCode::SUCCESS }
        "semantic-queue" => { semantic_queue::cli::entry(&rest); ExitCode::SUCCESS }
        "cache-key-lint" => cache_key_lint::cli::entry(&rest),
        "no-asm-c" => no_asm_c::cli::entry(&rest),
        "cache-entry" => cache_entry::cli::entry(&rest),
        "check-publication" => check_publication::cli::entry(&rest),
        "check-sanctum" => check_sanctum::cli::entry(&rest),
        "core-retained-audit" => core_retained_audit::cli::entry(&rest),
        "decomp-constraints" => decomp_constraints::cli::entry(&rest),
        "decomp-targets" => decomp_targets::cli::entry(&rest),
        "discover" => discover::cli::entry(&rest),
        "executable-gap-sources" => executable_gap_sources::cli::entry(&rest),
        "extract-resource" => extract_resource::cli::entry(&rest),
        "late-runtime-data" => late_runtime_data::cli::entry(&rest),
        "music-residuals" => music_residuals::cli::entry(&rest),
        "semantic-owner-scope" => semantic_owner_scope::cli::entry(&rest),
        "semantic-superseded" => semantic_superseded::cli::entry(&rest),
        "source-citations" => source_citations::cli::entry(&rest),
        other => {
            eprintln!("unknown check command: {other}\n\n{USAGE}"); list(); ExitCode::from(2)
        }
    }
}

fn list() { for (n, s) in COMMANDS { println!("  {n:<26} {s}"); } }
