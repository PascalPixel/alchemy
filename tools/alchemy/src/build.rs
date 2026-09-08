use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <compilers|asm|claimed|full|rom|assets|allocator> [args]";

pub fn entry(args: &[String]) -> ExitCode {
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    match command {
        "assets" => crate::build_assets::entry(&rest),
        "allocator" => crate::allocator::entry(&rest),
        "compilers" if rest == ["--help"] || rest == ["-h"] => {
            println!(
                "usage: alchemy build compilers\nBuilds pinned compiler sources without staging."
            );
            ExitCode::SUCCESS
        }
        "compilers" => crate::make_target("compilers", &rest),
        "asm" => crate::result(crate::build_asm::entry(&rest)),
        "claimed" => crate::result(crate::build_claimed::entry(&rest)),
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
