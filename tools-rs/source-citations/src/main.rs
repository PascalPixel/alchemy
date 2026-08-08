//! Port of the `main()` in `tools/check/source_citations.ts`.
//!
//!   source-citations
//!   source-citations --self-test

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use source_citations::{check, self_test};

/// The TypeScript resolves ROOT from import.meta.url; the binary resolves it
/// from its own manifest, which sits two levels below the repository root.
fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below the repository root")
        .to_path_buf()
}

fn main() -> ExitCode {
    let root = root();
    if std::env::args().any(|argument| argument == "--self-test") {
        return match self_test(&root) {
            Ok(()) => {
                println!("source_citations self-test ok");
                ExitCode::SUCCESS
            }
            Err(problem) => {
                eprintln!("error: {problem}");
                ExitCode::FAILURE
            }
        };
    }
    match check(&root) {
        Ok(summary) => {
            println!("{summary}");
            ExitCode::SUCCESS
        }
        Err(problems) => {
            for problem in &problems {
                eprintln!("error: {problem}");
            }
            ExitCode::FAILURE
        }
    }
}
