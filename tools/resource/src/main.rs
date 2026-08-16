//! `resource`: one executable for the resource decoders.
//!
//! Second consolidation after `overlay`. See that crate for why: `tools/` shipped
//! 115 executables that nothing could enumerate, so agents repeatedly rebuilt
//! work that already existed.
//!
//! Each former binary is now a library linked here. The `run` signatures differ
//! in argument shape and error type, so every adapter below is lifted from that
//! crate's own `main.rs` rather than assumed.

use std::process::ExitCode;

const USAGE: &str = "usage: resource <command> [args]\n       resource --list";

const COMMANDS: &[(&str, &str)] = &[
    ("3ce", "decode resource 3ce"),
    ("5", "decode resource 5"),
    ("d1-d3", "decode resources d1 through d3"),
    ("title", "decode title-screen resources"),
    ("sentou", "decode battle (sentou) resources"),
];

fn report<E: std::fmt::Display>(result: Result<(), E>) -> ExitCode {
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
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
        "3ce" => report(resource_3ce::run(rest)),
        "5" => report(resource_5::run(rest)),
        "d1-d3" => report(resource_d1_d3::run(rest)),
        "title" => report(title_resources::run(rest)),
        "sentou" => report(sentou_resources::run(&rest)),
        other => {
            eprintln!("unknown resource command: {other}\n\n{USAGE}");
            list();
            ExitCode::from(2)
        }
    }
}

fn list() {
    for (name, summary) in COMMANDS {
        println!("  {name:<10} {summary}");
    }
}
