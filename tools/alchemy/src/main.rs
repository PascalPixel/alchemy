use std::process::ExitCode;

mod bootstrap;
mod build;
mod build_rom;
mod build_text;
mod check;
mod compiler;
mod coverage;
mod edition;
mod gate;
mod targets;
mod verify;

const USAGE: &str = "usage: alchemy <command> [args]\n\
  bootstrap             install or validate the persistent compiler toolchain\n\
  build                 build compilers, maintained source, assembly and ROMs\n\
  verify                the gates of make verify, make land and the commit hooks\n\
  check                 run repository contract checks";

fn main() -> ExitCode {
    compiler::routing::prefer_installed_binutils();
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    match command {
        "bootstrap" => result(bootstrap::run(rest)),
        "build" => build::entry(rest),
        "verify" => verify::entry(rest),
        "check" => check::entry(rest),
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
