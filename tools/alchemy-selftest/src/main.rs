//! Native compiler self-test binary.
//!
//! Prints the stable success line on success and the diagnostic on failure.

use std::process::ExitCode;

fn main() -> ExitCode {
    // `--trace` dumps the recorded primitive calls for the diagnostic harness.
    let trace_requested = std::env::args().skip(1).any(|argument| argument == "--trace");
    match alchemy_selftest::self_test() {
        Ok((summary, trace)) => {
            if trace_requested {
                for record in &trace.records {
                    // PORT NOTE: U+001D between key and value, not a tab.
                    // Both key and value contain tabs internally, so a tab
                    // here would make the split ambiguous.
                    println!("{}\u{1d}{}", record.key, record.value);
                }
            }
            println!("{}", summary.line());
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
