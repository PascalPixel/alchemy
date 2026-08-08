//! `bun tools/lib/alchemy_gcc.ts --self-test`, as a binary.
//!
//! Prints the same success line on success and the same message on failure.
//!
//! PORT NOTE: the TypeScript throws, so Bun prints the message on stderr and
//! exits 1. This writes the message on stderr and exits 1. The stack-trace and
//! `error:` decoration Bun adds around it is not reproduced; the harness
//! compares exit code and message, never the wrapper prose.

use std::process::ExitCode;

fn main() -> ExitCode {
    // `--trace` dumps the recorded primitive calls, which is what feeds the
    // differential harness. Without it the binary behaves exactly like the
    // TypeScript entry point.
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
