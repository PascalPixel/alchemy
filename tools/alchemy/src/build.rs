use std::process::ExitCode;

const USAGE: &str = "usage: alchemy build <compilers|runtime|native|rom> [args]";

pub fn entry(args: &[String]) -> ExitCode {
    let Some(command) = args.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest: Vec<String> = args[1..].to_vec();
    if matches!(command, "claimed" | "assets" | "allocator") {
        return crate::result(Err(format!(
            "{command} used the removed generated catalogs; use build native with maintained Make/linker rules"
        )));
    }
    if matches!(command, "full" | "asm") {
        return crate::result(Err(format!(
            "{command} composed the ROM from removed receipts; build rom links maintained source"
        )));
    }
    match command {
        "native" => crate::result(crate::compiler::native::run(&rest)),
        "runtime" => crate::result(crate::compiler::runtime::entry(&rest)),
        "compilers" if rest == ["--help"] || rest == ["-h"] => {
            println!(
                "usage: alchemy build compilers\nBuilds pinned compiler sources without installing or admitting executables.\nUse alchemy bootstrap --from BUNDLE to install an approved distribution."
            );
            ExitCode::SUCCESS
        }
        "compilers" => crate::make_target("compiler-sources", &rest),
        "rom" => crate::result(crate::build_rom::run(&rest)),
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
