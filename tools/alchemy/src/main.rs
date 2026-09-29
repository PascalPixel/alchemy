use std::process::ExitCode;

mod bootstrap;
mod build;
mod build_rom;
mod build_text;
mod check;
mod compiler;
mod coverage;
mod disasm;
mod format;
mod gate;
mod overlay;
mod parallel;
mod permute;
mod raw;
mod recovery;
mod targets;
mod verify;

const USAGE: &str = "usage: alchemy <command> [args]\n\
  bootstrap             install or validate the persistent compiler toolchain\n\
  extract OWNER         extract reference bytes for psynergy decompile\n\
  inspect OWNER         resolve calls and symbols; --asm shows annotated instructions\n\
  build                 build compilers, maintained source, assembly and ROMs\n\
  verify                the landing gate: every make verify gate in waves, one line each\n\
  coverage              publish README progress and both figures from verified builds\n\
  raw                   inspect or rebuild ROM-derived unresolved assembly\n\
  permute DRAFT         search equivalent C for a draft function against its listing\n\
  check                 run repository contract checks\n\
  format                format native game data and check uppercase filenames";

fn main() -> ExitCode {
    compiler::routing::prefer_installed_binutils();
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    if matches!(
        command,
        "adopt" | "unit" | "overlay" | "land" | "score" | "targets" | "cross-edition"
    ) {
        return result(Err(format!(
            "{command} used the removed owner/translation-unit catalogs; use maintained source and Make/linker rules"
        )));
    }
    match command {
        "bootstrap" => result(bootstrap::run(rest)),
        "build" => build::entry(rest),
        "verify" => verify::entry(rest),
        "coverage" => make_target(command, rest),
        "raw" => result(raw::run(rest)),
        "permute" => result(permute::run(rest)),
        "check" => check::entry(rest),
        "format" => result(format::run(rest)),
        "extract" | "inspect" => recovery_command(command, rest),
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

fn make_target(target: &str, arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        if target == "coverage" {
            println!(
                "usage: alchemy coverage\nPublishes README progress and both figures from each game's verified build."
            );
        } else {
            println!("usage: alchemy {target}\nRuns the repository's make {target} contract.");
        }
        return ExitCode::SUCCESS;
    }
    if !arguments.is_empty() {
        eprintln!("usage: alchemy {target}");
        return ExitCode::from(2);
    }
    match std::process::Command::new("make")
        .current_dir(crate::compiler::routing::root())
        .arg(target)
        .status()
    {
        Ok(status) if status.success() => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => result(Err(format!("cannot run make {target}: {error}"))),
    }
}

fn recovery_command(command: &str, arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        let usage = match command {
            "extract" => "extract OWNER --out out/FILE [--span BYTES]",
            "inspect" => "inspect OWNER [--span BYTES] [--asm]",
            _ => return recovery::cli::entry(&["--help".to_string()]),
        };
        println!(
            "usage: alchemy {usage}\nOWNER is a main-ROM address or resource-qualified address."
        );
        return ExitCode::SUCCESS;
    }
    let mut args = vec![command.to_string()];
    args.extend_from_slice(arguments);
    recovery::cli::entry(&args)
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
