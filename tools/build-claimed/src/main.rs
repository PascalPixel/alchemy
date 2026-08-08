use build_claimed::{build, parse_args, root, self_test, ParsedArgs};

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    if arguments.iter().any(|argument| argument == "--self-test") {
        match self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => fail(&message),
        }
        return;
    }

    let options = match parse_args(&arguments) {
        Ok(ParsedArgs::Help) => {
            println!("{}", build_claimed::usage_text());
            return;
        }
        Ok(ParsedArgs::Run(options)) => options,
        Err(message) => fail(&message),
    };
    let cwd = std::env::current_dir()
        .unwrap_or_else(|error| fail(&format!("cwd: {error}")));
    let repository = root();
    let summary = build(&options, &repository, &cwd.to_string_lossy())
        .unwrap_or_else(|message| fail(&message));

    println!("{}", summary.summary_line());
    for failure in &summary.failures {
        println!("{failure}");
    }
    if !summary.failures.is_empty() {
        std::process::exit(1);
    }
}

fn fail(message: &str) -> ! {
    eprintln!("error: {message}");
    std::process::exit(1);
}
