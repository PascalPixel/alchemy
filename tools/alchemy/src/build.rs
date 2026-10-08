use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <compilers|runtime|rom|modern> [args]";

pub fn entry(args: &[String]) -> ExitCode {
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "runtime" => crate::result(crate::compiler::runtime::entry(&rest)),
        "compilers" if rest == ["--help"] || rest == ["-h"] => {
            println!(
                "usage: alchemy build compilers\nBuilds pinned compiler sources without installing or admitting executables.\nUse alchemy bootstrap --from BUNDLE to install an approved distribution."
            );
            ExitCode::SUCCESS
        }
        "compilers" => compiler_sources(&rest),
        "rom" => crate::result(crate::build_rom::run(&rest)),
        "modern" => crate::result(crate::modern::run(&rest)),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        other => {
            eprintln!("unknown build stage: {other}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}

fn compiler_sources(arguments: &[String]) -> ExitCode {
    if !arguments.is_empty() {
        eprintln!("usage: alchemy build compilers");
        return ExitCode::from(2);
    }
    match std::process::Command::new("make")
        .current_dir(crate::compiler::routing::root())
        .arg("compiler-sources")
        .status()
    {
        Ok(status) if status.success() => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => crate::result(Err(format!("cannot run make compiler-sources: {error}"))),
    }
}
