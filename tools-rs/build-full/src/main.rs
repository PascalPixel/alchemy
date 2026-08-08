use build_full::{build, parse_args, repository_root, ParseOutcome};

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.as_slice() == ["--self-test"] {
        build_full::self_test().unwrap_or_else(|error| fail(&error));
        println!("self-test=ok");
        return;
    }
    let options = match parse_args(&arguments) {
        Ok(ParseOutcome::Help) => {
            println!("usage: build-full [-h] [--target gs1-en|gs2-en] [-o OUTPUT] [--claimed-output CLAIMED_OUTPUT] [--asm-output ASM_OUTPUT] [--asset-manifest ASSET_MANIFEST] [--asset-output ASSET_OUTPUT] [--jobs JOBS] [--source-only] [rom] | --self-test");
            return;
        }
        Ok(ParseOutcome::Run(options)) => options,
        Err(error) => fail(&error),
    };
    let cwd = std::env::current_dir().unwrap_or_else(|error| fail(&format!("cwd: {error}")));
    let summary = build(&repository_root(), &cwd, &options).unwrap_or_else(|error| fail(&error));
    println!("{summary}");
}

fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}
