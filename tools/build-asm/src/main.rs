use build_asm::{build, parse_args, repository_root, ParseOutcome};

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.as_slice() == ["--self-test"] {
        let sample = vec!["--source-only".to_string(), "--output=out/test".to_string()];
        match parse_args(&sample) {
            Ok(ParseOutcome::Run(options))
                if options.source_only && options.output == "out/test" =>
            {
                println!("self-test=ok");
                return;
            }
            _ => fail("self-test failed"),
        }
    }
    let options = match parse_args(&arguments) {
        Ok(ParseOutcome::Help) => {
            println!(
                "usage: build-asm [-h] [--source-only] [--output OUTPUT] [--source SOURCE] [rom]"
            );
            return;
        }
        Ok(ParseOutcome::Run(options)) => options,
        Err(message) => fail(&message),
    };
    let cwd = std::env::current_dir().unwrap_or_else(|error| fail(&format!("cwd: {error}")));
    let report = build(&repository_root(), &cwd, &options).unwrap_or_else(|message| fail(&message));
    println!("regions={} bytes={}", report.regions, report.bytes);
    println!("{}", report.counts);
}

fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}
