use std::process::ExitCode;

mod layout;
mod no_asm;
mod publication;

const USAGE: &str =
    "usage: alchemy check <publication|coverage|layout|no-asm|dead|progress> [args]";

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "publication" => publication::entry(rest),
        "layout" => layout::entry(rest),
        "coverage" => {
            crate::coverage::entry(rest);
            ExitCode::SUCCESS
        }
        "no-asm" => no_asm::entry(rest),
        "dead" => crate::dead::check_entry(rest),
        "progress" => {
            crate::coverage::progress::entry(rest);
            ExitCode::SUCCESS
        }
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
