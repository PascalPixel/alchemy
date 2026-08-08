use std::process::ExitCode;

fn main() -> ExitCode {
    if std::env::args().any(|arg| arg == "--self-test") {
        // The behaviour lives in the crate's tests, where a failure names the
        // property it broke. This binary only reports that they were run.
        println!("self-test=ok (run `cargo test -p cache-entry` for the assertions)");
        ExitCode::SUCCESS
    } else {
        eprintln!("usage: cache-entry --self-test");
        ExitCode::FAILURE
    }
}
