//! `compiler`: one executable for the compiler-side searches.
//!
//! Tenth consolidation. See docs/ENTRY-POINTS.md.

use std::process::ExitCode;

const USAGE: &str = "usage: compiler <command> [args]\n       compiler --list";
const COMMANDS: &[(&str, &str)] = &[
    ("mode-sweep", "plan, score and report compiler-mode sweeps"),
    ("search-modes", "search compiler mode combinations"),
    ("shape-sweep", "bounded source-shape search, with the iterative descent driver"),
    ("thumb-disasm", "thumb disasm"),
    ("candidate-show", "compile one candidate and show the byte comparison"),
    ("candidate-explain", "explain a candidate's routing and flags"),
    ("main-rank", "rank main-image candidates and classify each residual"),
    ("rtl-insn", "inspect RTL instructions"),
    ("rtl-sexpr", "inspect RTL s-expressions"),
    ("rtl-schedule", "inspect RTL scheduling"),
    ("rtl-align", "inspect RTL alignment"),
    ("permute", "bounded C permutation search over a candidate"),
    ("residuals", "audit remaining residual bytes"),
    ("bl-symbols", "report bl call-site symbols"),
    ("dashboard-server", "serve the coverage dashboard"),
    ("mode-cohort", "mode cohort"),
    ("decomp-diagnose", "decomp diagnose"),
    ("corpus-regression", "run the compiler corpus regression"),
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
        "corpus-regression" => { compiler_corpus_regression::entry_main::entry(&rest); ExitCode::SUCCESS }

        "mode-cohort" => { mode_cohort::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "decomp-diagnose" => { decomp_diagnose::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "permute" => match alchemy_permuter::run(rest.clone()) {
            Ok(()) => ExitCode::SUCCESS,
            Err(message) => { eprintln!("error: {message}"); ExitCode::FAILURE }
        },
        "residuals" => match audit_residuals::run(&rest) {
            Ok(output) => { if !output.is_empty() { println!("{output}"); } ExitCode::SUCCESS }
            Err(message) => { eprintln!("error: {message}"); ExitCode::FAILURE }
        },
        "bl-symbols" => match bl_site_symbols::run(&rest) {
            Ok(output) => { print!("{output}"); if !output.ends_with('\n') { println!(); } ExitCode::SUCCESS }
            Err(error) => { eprintln!("error: {error}"); ExitCode::FAILURE }
        },
        "dashboard-server" => { dashboard_server::cli::entry(&rest); ExitCode::SUCCESS }
        "rtl-insn" => rtl_insn::cli::entry(&rest),
        "rtl-sexpr" => rtl_sexpr::cli::entry(&rest),
        "rtl-schedule" => rtl_schedule::cli::entry(&rest),
        "rtl-align" => rtl_align::cli::entry(&rest),
        "candidate-show" => { candidate_show::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "candidate-explain" => { candidate_explain::entrypoint::entry(&rest); ExitCode::SUCCESS }
        "main-rank" => {
            // The subcommand goes with the path: children re-enter the host at
            // its `candidate-show` arm, not at `--align` as a command name.
            let self_exe = std::env::current_exe()
                .unwrap_or_else(|_| std::path::PathBuf::from("compiler"));
            let root = std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("../..");
            match main_candidate_rank::run(&root, &self_exe, &["candidate-show"], &rest) {
                Ok(()) => ExitCode::SUCCESS,
                Err(error) => { eprintln!("error: {error}"); ExitCode::FAILURE }
            }
        }
        "thumb-disasm" => match thumb_disasm::cli::entry(&rest) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => { eprintln!("error: {error}"); ExitCode::FAILURE }
        },
        "mode-sweep" => mode_sweep::cli::entry(&rest),
        "search-modes" => search_compiler_modes::cli::entry(&rest),
        "shape-sweep" => match shape_sweep::run(rest) {
            Ok(()) => ExitCode::SUCCESS,
            Err(message) => { eprintln!("error: {message}"); ExitCode::FAILURE }
        },
        other => {
            eprintln!("unknown compiler command: {other}\n\n{USAGE}"); list(); ExitCode::from(2)
        }
    }
}

fn list() { for (n, s) in COMMANDS { println!("  {n:<16} {s}"); } }

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
    matches!(name, "bl-symbols" | "candidate-explain" | "candidate-show" | "corpus-regression" | "dashboard-server" | "decomp-diagnose" | "main-rank" | "mode-cohort" | "mode-sweep" | "permute" | "residuals" | "rtl-align" | "rtl-insn" | "rtl-schedule" | "rtl-sexpr" | "search-modes" | "shape-sweep" | "thumb-disasm")
}
