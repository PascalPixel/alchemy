use std::process::ExitCode;

const USAGE: &str = "usage: core-retained-audit --check [--json]";

pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments.iter().any(|argument| matches!(argument.as_str(), "-h" | "--help")) {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if !arguments.iter().any(|argument| argument == "--check")
        || arguments.iter().any(|argument| !matches!(argument.as_str(), "--check" | "--json"))
    {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    }
    match crate::audit(&crate::repository_root()) {
        Ok(audit) => {
            if arguments.iter().any(|argument| argument == "--json") {
                println!("{}", serde_json::to_string_pretty(&audit.json()).unwrap());
            } else {
                println!(
                    "status=ok executable={} exact_c={} retained={}",
                    audit.executable, audit.exact, audit.retained
                );
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
