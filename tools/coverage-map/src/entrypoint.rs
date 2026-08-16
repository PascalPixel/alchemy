//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

// Thin entry point: everything lives in the library root.
//
// PORT NOTE: a `throw` out of the TypeScript entry point is reported by Bun as
// `error: <message>` on stderr with exit status 1, followed by a stack trace
// this binary has no equivalent for. The `error:` line and the status are what
// the parity harness compares.

pub fn entry(arguments: &[String]) {
    let argv: Vec<String> = arguments.to_vec();
    if argv.as_slice() == ["--self-test"] {
        match crate::selftest::self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => {
                eprintln!("error: {message}");
                std::process::exit(1);
            }
        }
        return;
    }
    match crate::cli::run(&argv) {
        Ok(line) => println!("{line}"),
        Err(message) => {
            eprintln!("error: {message}");
            std::process::exit(1);
        }
    }
}
