//! alchemy: the matching aids and the progress count. The ROMs build from
//! the Makefile alone; nothing here takes part in a build.

use std::process::ExitCode;

mod compile;
mod decode;
mod diff;
mod elf;
mod ld_map;
mod permute;
mod process;
mod progress;
mod show;
mod targets;
mod thumb;

const USAGE: &str = r#"usage: alchemy <command> [args]
Run any command with --help for its options.

  diff      a C draft against the listing it would replace
  permute   search rewrites and PERM macro choices of a draft toward differ 0
  progress  DONE code and data in each edition, from the linker maps"#;

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "diff" => result(diff::run(rest)),
        "permute" => result(permute::run(rest)),
        "progress" => result(progress::run(rest)),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown alchemy command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}

fn result(value: Result<(), String>) -> ExitCode {
    match value {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
