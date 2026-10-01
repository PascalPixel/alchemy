use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <compilers|runtime|rom|overlays> [args]";

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
        "compilers" => crate::make_target("compiler-sources", &rest),
        "rom" => crate::result(crate::build_rom::run(&rest)),
        "overlays" => crate::result(crate::build_rom::run_overlays(&rest)),
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
