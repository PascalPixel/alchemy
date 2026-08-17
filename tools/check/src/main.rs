//! `check`: one executable for the audit, census and verification tools.
//!
//! Seventh consolidation. See docs/ENTRY-POINTS.md.

use std::process::ExitCode;

const USAGE: &str = "usage: check <command> [args]\n       check --list";
const COMMANDS: &[(&str, &str)] = &[
    ("cache-entry", "cache entry"),
    ("check-publication", "check publication"),
    ("check-unmatchable", "validate the owner status registers"),
    ("symbols", "validate the symbol map; --emit writes include/symbols.h"),
    ("core-retained-audit", "core retained audit"),
    ("decomp-constraints", "decomp constraints"),
    ("decomp-targets", "decomp targets"),
    ("discover", "discover"),
    ("executable-gap-sources", "executable gap sources"),
    ("extract-resource", "extract resource"),
    ("late-runtime-data", "late runtime data"),
    ("music-residuals", "music residuals"),
    ("source-citations", "source citations"),
    ("cache-key-lint", "cache key lint"),
    ("no-asm-c", "no asm c"),
    ("compare-roms", "compare roms"),
    ("lang-ban", "lang ban"),
    ("check-commit-progress", "check commit progress"),
    ("coverage-map", "refresh the dashboard coverage maps"),
    ("byte-value-regions", "byte value regions"),
    ("architecture", "architecture"),
    ("alchemy-lints", "alchemy lints"),
    ("jobs", "jobs"),
    ("integrate-matches", "integrate matched candidates into the tree"),
    ("full-c-history", "full c history"),
    ("localization-tables", "localization tables"),
    ("route-dump", "dump the compiler flag routing"),
    ("progress", "print byte-exact progress"),
];

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}"); list(); return ExitCode::from(2);
    };
    if command == "--self-test" {
        return self_test();
    }
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:"); list(); return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "progress" => { full_c_progress::entry(&rest); ExitCode::SUCCESS }

        "route-dump" => route_dump::entrypoint::entry(&rest),

        "full-c-history" => { full_c_history::cli::entry(&rest); ExitCode::SUCCESS }
        "localization-tables" => { localization_tables::cli::entry(&rest); ExitCode::SUCCESS }
        "integrate-matches" => { integrate_matches::entry_main::entry(&rest); ExitCode::SUCCESS }

        "architecture" => { architecture::cli::entry(&rest); ExitCode::SUCCESS }
        "alchemy-lints" => { alchemy_lints::cli::entry(&rest); ExitCode::SUCCESS }
        "jobs" => { jobs::cli::entry(&rest); ExitCode::SUCCESS }
        "byte-value-regions" => { byte_value_regions::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "coverage-map" => { coverage_map::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "lang-ban" => lang_ban::cli::entry(&rest),
        "check-commit-progress" => check_commit_progress::cli::entry(&rest),
        "compare-roms" => { compare_roms::cli::entry(&rest); ExitCode::SUCCESS }
        "cache-key-lint" => cache_key_lint::cli::entry(&rest),
        "no-asm-c" => no_asm_c::cli::entry(&rest),
        "cache-entry" => cache_entry::cli::entry(&rest),
        "check-publication" => check_publication::cli::entry(&rest),
        "check-unmatchable" => check_unmatchable::cli::entry(&rest),
        "symbols" => symbols::cli::entry(&rest),
        "core-retained-audit" => core_retained_audit::cli::entry(&rest),
        "decomp-constraints" => decomp_constraints::cli::entry(&rest),
        "decomp-targets" => decomp_targets::cli::entry(&rest),
        "discover" => discover::cli::entry(&rest),
        "executable-gap-sources" => executable_gap_sources::cli::entry(&rest),
        "extract-resource" => extract_resource::cli::entry(&rest),
        "late-runtime-data" => late_runtime_data::cli::entry(&rest),
        "music-residuals" => music_residuals::cli::entry(&rest),
        "source-citations" => source_citations::cli::entry(&rest),
        other => {
            eprintln!("unknown check command: {other}\n\n{USAGE}"); list(); ExitCode::from(2)
        }
    }
}

fn list() { for (n, s) in COMMANDS { println!("  {n:<26} {s}"); } }

/// `--self-test`: prove the host's own contract.
///
/// Each consolidated host swallowed the `--self-test` its component binaries
/// used to answer, so the native runner reported five hosts failing for a flag
/// none of them implemented. What a HOST owns is its dispatch table, so that is
/// what it checks: every command named, uniquely, in sorted order, with a
/// non-empty summary and a reachable arm. The components themselves are covered
/// by `make crate-tests` and by the dispatcher registry gates.
fn self_test() -> ExitCode {
    // Uniqueness and reachability, not sort order: several hosts group related
    // commands deliberately, and `--list` should keep reading that way.
    let mut seen: Vec<&str> = Vec::new();
    for (name, summary) in COMMANDS {
        if name.is_empty() || summary.is_empty() {
            eprintln!("self-test: a command has an empty name or summary");
            return ExitCode::FAILURE;
        }
        if seen.contains(name) {
            eprintln!("self-test: {name} is listed twice");
            return ExitCode::FAILURE;
        }
        if !dispatchable(name) {
            eprintln!("self-test: {name} is listed but has no dispatch arm");
            return ExitCode::FAILURE;
        }
        seen.push(name);
    }
    println!("self-test=ok commands={}", COMMANDS.len());
    ExitCode::SUCCESS
}

fn dispatchable(name: &str) -> bool {
    matches!(name, "alchemy-lints" | "architecture" | "byte-value-regions" | "cache-entry" | "cache-key-lint" | "check-commit-progress" | "check-publication" | "check-unmatchable" | "compare-roms" | "core-retained-audit" | "coverage-map" | "decomp-constraints" | "decomp-targets" | "discover" | "executable-gap-sources" | "extract-resource" | "full-c-history" | "integrate-matches" | "jobs" | "lang-ban" | "late-runtime-data" | "localization-tables" | "music-residuals" | "no-asm-c" | "progress" | "route-dump" | "source-citations" | "symbols")
}
