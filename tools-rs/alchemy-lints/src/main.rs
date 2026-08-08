//! Run the three toolchain-free lints over the tree.
//!
//!   alchemy-lints                    all three
//!   alchemy-lints callback-arity     one of them
//!
//! Every lint prints the counters it accumulated BEFORE its verdict, because
//! the counters are the evidence that the verdict means anything. A run that
//! reports `leaks=0` next to `files=0` is a failure, and it says so.

use std::process::ExitCode;

use alchemy_lints::{callback_arity, flag_capability, repository_root, stem_collision, Report};

fn print(report: &Report) {
    println!("== {}", report.lint);
    for finding in &report.findings {
        println!("  {}:{}  {}", finding.file, finding.line, finding.message);
    }
    let counts: Vec<String> =
        report.counts.iter().map(|(name, value)| format!("{name}={value}")).collect();
    println!("  {}", counts.join(" "));
    for reason in &report.blind {
        println!("  BLIND: {reason}");
    }
    println!("  {}", if report.ok() { "ok" } else { "FAILED" });
}

fn main() -> ExitCode {
    let root = repository_root();
    let wanted: Vec<String> = std::env::args().skip(1).collect();
    let selected = |name: &str| wanted.is_empty() || wanted.iter().any(|one| one == name);

    let mut reports = Vec::new();
    if selected("callback-arity") {
        match callback_arity::read_tree(&root) {
            Ok(sources) => reports.push(callback_arity::analyse(&sources)),
            Err(message) => {
                eprintln!("error: {message}");
                return ExitCode::FAILURE;
            }
        }
    }
    if selected("overlay-stem-collision") {
        match stem_collision::read_tree(&root) {
            Ok((source, names)) => reports.push(stem_collision::analyse(&source, &names)),
            Err(message) => {
                eprintln!("error: {message}");
                return ExitCode::FAILURE;
            }
        }
    }
    if selected("flag-capability") {
        match flag_capability::read_tree(&root) {
            Ok(source) => reports.push(flag_capability::analyse(&source)),
            Err(message) => {
                eprintln!("error: {message}");
                return ExitCode::FAILURE;
            }
        }
    }

    if reports.is_empty() {
        eprintln!("error: no lint selected; known lints are callback-arity, \
                   overlay-stem-collision, flag-capability");
        return ExitCode::FAILURE;
    }
    for report in &reports {
        print(report);
    }
    if reports.iter().all(Report::ok) {
        ExitCode::SUCCESS
    } else {
        ExitCode::FAILURE
    }
}
