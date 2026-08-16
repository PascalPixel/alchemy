//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

// CLI entry point for the exact-reading-list report.
//
//   exact-reading-list                  # every overlay, ranked
//   exact-reading-list resource_373     # one overlay
//   exact-reading-list --json
//   exact-reading-list --blocked        # only the rows held back
//   exact-reading-list --self-test
//
// PORT NOTE: the TS throws on a stale executable map, so Bun prints a stack
// trace to stderr and exits 1. This prints just the message and exits 1. The
// exit code and stdout are identical; stderr text is not, and cannot be without
// faking a JS stack.

use std::io::Write;
use std::process::ExitCode;

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let args: Vec<String> = arguments.to_vec();
    let options = match crate::parse_args(&args) {
        Ok(options) => options,
        Err(message) => return fail(&message),
    };

    if options.help {
        println!("{}", crate::USAGE);
        return ExitCode::SUCCESS;
    }

    if options.self_test {
        return match crate::self_test() {
            Ok(()) => {
                println!("self-test=ok");
                ExitCode::SUCCESS
            }
            Err(message) => fail(&message),
        };
    }

    let root = match crate::repository_root() {
        Ok(root) => root,
        Err(message) => return fail(&message),
    };
    let all = match crate::reading_list(&root) {
        Ok(list) => list,
        Err(message) => return fail(&message),
    };

    if options.json {
        let mut list: Vec<crate::Pairing> = all
            .into_iter()
            .filter(|item| item.blocked == options.blocked)
            .collect();
        if let Some(name) = options.only.as_deref() {
            list.retain(|item| item.overlay == name);
        }
        println!("{}", crate::render_json(&list));
        return ExitCode::SUCCESS;
    }

    print!(
        "{}",
        crate::render_report(&all, options.only.as_deref(), options.blocked)
    );
    ExitCode::SUCCESS
}

fn fail(message: &str) -> ExitCode {
    let _ = writeln!(std::io::stderr(), "{message}");
    ExitCode::FAILURE
}
