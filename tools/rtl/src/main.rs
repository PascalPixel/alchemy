//! `rtl`: one executable for the RTL inspection tools.
//!
//! Fourth consolidation. See `tools/overlay` for why.

use std::process::ExitCode;

const USAGE: &str = "usage: rtl <command> [args]\n       rtl --list";
const COMMANDS: &[(&str, &str)] = &[
    ("insn", "inspect RTL instructions"),
    ("sexpr", "inspect RTL s-expressions"),
];

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
        "insn" => rtl_insn::cli::entry(&rest),
        "sexpr" => rtl_sexpr::cli::entry(&rest),
        other => {
            eprintln!("unknown rtl command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<8} {summary}");
    }
}
