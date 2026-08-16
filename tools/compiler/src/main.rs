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
    ("rtl-insn", "inspect RTL instructions"),
    ("rtl-sexpr", "inspect RTL s-expressions"),
    ("rtl-schedule", "inspect RTL scheduling"),
    ("rtl-align", "inspect RTL alignment"),
    ("permute", "bounded C permutation search over a candidate"),
    ("residuals", "audit remaining residual bytes"),
    ("bl-symbols", "report bl call-site symbols"),
    ("dashboard-server", "serve the coverage dashboard"),
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
