use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <runtime|rom|modern> [args]";

pub fn entry(args: &[String]) -> ExitCode {
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "runtime" => crate::result(crate::compiler::runtime::entry(&rest)),
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
