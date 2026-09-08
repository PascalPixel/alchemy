use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <compilers|asm|claimed|full|rom> [args]";

pub fn entry(args: &[String]) -> ExitCode {
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "compilers" if rest == ["--help"] || rest == ["-h"] => {
            println!(
                "usage: alchemy build compilers\nBuilds pinned compiler sources without staging."
            );
            ExitCode::SUCCESS
        }
        "compilers" => crate::make_target("compilers", &rest),
        "asm" => {
            build_asm::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "claimed" => {
            build_claimed::cli::entry(&rest);
            ExitCode::SUCCESS
        }
        "full" | "rom" => crate::result(crate::build_full::run(&rest)),
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
