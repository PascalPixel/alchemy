//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

// The dashboard binary: live server by default, self-test on request.

fn usage() {
    println!(
        "Usage: dashboard-server [--self-test]\n\n\
        Options:\n  -h, --help     Show this help\n  --self-test    Run the dashboard self-test"
    );
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let mut arguments = std::env::args().skip(1);
    let Some(argument) = arguments.next() else {
        if let Err(error) = crate::server::run() {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
        return;
    };
    match argument.as_str() {
        "-h" | "--help" => usage(),
        "--self-test" if arguments.next().is_none() => match crate::selftest::self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => {
                // Keep self-test failures concise and machine-readable.
                eprintln!("error: {message}");
                std::process::exit(1);
            }
        },
        other => {
            eprintln!("error: unknown argument {other:?}\n");
            usage();
            std::process::exit(2);
        }
    }
}
