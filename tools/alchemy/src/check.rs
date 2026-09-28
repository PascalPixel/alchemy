#[cfg(test)]
use std::process::Command;
use std::process::ExitCode;

mod commit_progress;
mod no_asm;
mod publication;
pub(crate) use publication::PRESENTATION_EXTENSIONS;

const USAGE: &str =
    "usage: alchemy check <publication|commit-progress|coverage|no-asm|progress|routes> [args]";

/// Run a check body and report its error the way every check does.
fn report(result: Result<(), String>) -> ExitCode {
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
fn fixture_repository(files: &[(&str, &str, bool)]) -> tempfile::TempDir {
    let directory = tempfile::tempdir().unwrap();
    let git = |args: &[&str]| {
        let status = Command::new("git")
            .arg("-C")
            .arg(directory.path())
            .args(args)
            .status()
            .unwrap();
        assert!(status.success(), "git {args:?}");
    };
    git(&["init", "--quiet"]);
    for (path, text, tracked) in files {
        let path_on_disk = directory.path().join(path);
        std::fs::create_dir_all(path_on_disk.parent().unwrap()).unwrap();
        std::fs::write(path_on_disk, text).unwrap();
        if *tracked {
            git(&["add", "--", path]);
        }
    }
    directory
}

fn routes(arguments: &[String]) -> ExitCode {
    if arguments == ["--standard"] {
        for flag in crate::compiler::routing::cflags_for_target(
            crate::compiler::routing::CompilerTarget::Tbs,
        ) {
            println!("{flag}");
        }
        ExitCode::SUCCESS
    } else {
        eprintln!("usage: alchemy check routes --standard");
        ExitCode::from(2)
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let rest = &arguments[1..];
    if matches!(
        command,
        "source-tracking" | "owners" | "tla-owners" | "integrate" | "siblings" | "source-build"
    ) {
        return report(Err(format!(
            "{command} used the removed catalogs or receipts; make compare builds and verifies maintained source"
        )));
    }
    match command {
        "source-tracking" if rest.is_empty() => {
            match crate::build_assets::check_source_tracking() {
                Ok(()) => ExitCode::SUCCESS,
                Err(error) => {
                    eprintln!("error: {error}");
                    ExitCode::FAILURE
                }
            }
        }
        "publication" => publication::entry(rest),
        "commit-progress" => commit_progress::entry(rest),
        "coverage" => {
            crate::coverage::entry(rest);
            ExitCode::SUCCESS
        }
        "no-asm" => no_asm::entry(rest),
        "progress" => {
            crate::coverage::progress::entry(rest);
            ExitCode::SUCCESS
        }
        "routes" => routes(rest),
        "-h" | "--help" => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        _ => {
            eprintln!("unknown check command: {command}\n{USAGE}");
            ExitCode::from(2)
        }
    }
}
