//! `dev`: one executable for the development-loop tools.
//!
//! Fifth consolidation. See docs/ENTRY-POINTS.md for the principle and the
//! recipes; see tools/overlay for the original template.
//!
//! CAUTION, learned the hard way: `pub fn run` is not a reliable signal that a
//! crate exposes a CLI. `build_rom::run(root, command)` is a SUBPROCESS SPAWNER
//! that executes `command[0]`. Wiring it up here as a command adapter made
//! `make build-rom` panic instead of failing cleanly. Read what the function
//! does before linking it, never just its name.

use std::process::ExitCode;

const USAGE: &str = "usage: dev <command> [args]\n       dev --list";
const COMMANDS: &[(&str, &str)] = &[
    ("permute", "bounded C permutation search over a candidate"),
    ("residuals", "audit remaining residual bytes"),
    ("bl-symbols", "report bl call-site symbols"),
    ("dashboard-server", "dashboard server"),
];

fn text(result: Result<String, String>) -> ExitCode {
    match result {
        Ok(output) => {
            if !output.is_empty() {
                print!("{output}");
                if !output.ends_with('\n') {
                    println!();
                }
            }
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        list();
        return ExitCode::from(2);
    };
    if matches!(command, "-h" | "--help" | "--list") {
        println!("{USAGE}\n\ncommands:");
        list();
        return ExitCode::SUCCESS;
    }
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "dashboard-server" => { dashboard_server::cli::entry(&rest); ExitCode::SUCCESS }
        "residuals" => text(audit_residuals::run(&rest)),
        "bl-symbols" => text(bl_site_symbols::run(&rest).map_err(|e| e.to_string())),
        "permute" => match alchemy_permuter::run(rest) {
            Ok(()) => ExitCode::SUCCESS,
            Err(message) => {
                eprintln!("error: {message}");
                ExitCode::FAILURE
            }
        },
        other => {
            eprintln!("unknown dev command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<12} {summary}");
    }
}
