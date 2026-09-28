//! Main's commit subject, derived from the current verified production builds.

use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: check commit-progress [--write] COMMIT_MESSAGE\n\
On main, derive both DONE percentages from verified receipts; --write updates the subject.\n\
Other branches keep their subjects and require no build receipts.";

fn valid(message: &str, expected: &str) -> bool {
    crate::verify::valid_subject(message, expected)
}

fn rewrite(message: &str, expected: &str) -> Result<String, String> {
    let (subject, body) = message
        .split_once('\n')
        .map_or((message, ""), |(subject, body)| (subject, body));
    let carriage_return = subject.ends_with('\r');
    let subject = subject.trim_end_matches('\r');
    let prefix =
        regex::Regex::new(r"^☀️\s+(?:\d+(?:\.\d+)?|\?)%\s+⚓️\s+(?:\d+(?:\.\d+)?|\?)%\s+[–-]\s*")
            .map_err(|error| error.to_string())?;
    let title = prefix.replace(subject, "");
    let title = title.trim();
    if title.is_empty() || title.starts_with('#') {
        return Err("commit subject needs a title".into());
    }
    let mut rewritten = format!("{expected} {title}");
    if message.contains('\n') {
        if carriage_return {
            rewritten.push('\r');
        }
        rewritten.push('\n');
        rewritten.push_str(body);
    }
    Ok(rewritten)
}

fn self_test() -> Result<(), String> {
    let expected = "☀️ 52.34% ⚓️ 7.89% –";
    let bad = [
        "missing",
        "☀️ 52% – one game",
        "☀️ 51% ⚓️ ?% – stale",
        "☀️ 52% ⚓️ 7% – unmeasured",
        "☀️ 52% ⚓️ ?% - wrong dash",
        "☀️ 52.34% ⚓️ 7.89% –",
    ];
    if !valid("☀️ 52.34% ⚓️ 7.89% – Name the owner", expected)
        || bad.into_iter().any(|message| valid(message, expected))
        || rewrite("☀️ 51% ⚓️ ?% – Name the owner\n\nBody\n", expected)?
            != "☀️ 52.34% ⚓️ 7.89% – Name the owner\n\nBody\n"
    {
        return Err("subject prefix self-test failed".into());
    }
    println!("self-test=ok prefix=done-percent");
    Ok(())
}

fn run(root: &Path, arguments: &[String]) -> Result<(), String> {
    match arguments {
        [flag] if flag == "--self-test" => self_test(),
        [message] | [_, message] => {
            let write = arguments.len() == 2 && arguments[0] == "--write";
            if arguments.len() == 2 && !write {
                return Err(USAGE.into());
            }
            if !crate::verify::is_main(root)? {
                return Ok(());
            }
            let path = Path::new(message);
            let message = std::fs::read_to_string(path).map_err(|error| error.to_string())?;
            let expected = crate::verify::verified_subject(root)?;
            if write {
                let rewritten = rewrite(&message, &expected)?;
                std::fs::write(path, rewritten).map_err(|error| error.to_string())?;
                return Ok(());
            }
            if valid(&message, &expected) {
                Ok(())
            } else {
                Err(format!("commit subject must start with {expected}"))
            }
        }
        _ => Err(USAGE.into()),
    }
}

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    if arguments == ["--help"] || arguments == ["-h"] {
        println!("{USAGE}");
        return ExitCode::SUCCESS;
    }
    if let Err(error) = run(&crate::compiler::routing::root(), arguments) {
        eprintln!("error: {error}");
        return ExitCode::FAILURE;
    }
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn main_replaces_stale_or_missing_prefix_and_preserves_body() {
        let expected = "☀️ 73.65% ⚓️ 2.14% –";
        let body = "\n\nPreserve all drafts.\n\nCo-Authored-By: Sol 6 <agent@example.com>\n";
        for subject in ["Preserve drafts", "☀️ 52% ⚓️ ?% – Preserve drafts"] {
            let rewritten = rewrite(&format!("{subject}{body}"), expected).unwrap();
            assert_eq!(rewritten, format!("{expected} Preserve drafts{body}"));
            assert!(valid(&rewritten, expected));
            assert_eq!(rewrite(&rewritten, expected).unwrap(), rewritten);
        }
        assert!(!valid("☀️ 73% ⚓️ 2% – Preserve drafts", expected));
    }

    #[test]
    fn rewrite_keeps_line_endings_and_rejects_empty_titles() {
        let expected = "☀️ 73.65% ⚓️ 2.14% –";
        assert_eq!(
            rewrite("Preserve drafts\r\n\r\nBody\r\n", expected).unwrap(),
            format!("{expected} Preserve drafts\r\n\r\nBody\r\n")
        );
        assert!(rewrite("", expected).is_err());
        assert!(rewrite("☀️ 52% ⚓️ ?% –", expected).is_err());
    }

    #[test]
    fn branch_commit_needs_no_receipts_or_progress_subject() {
        let directory = tempfile::tempdir().unwrap();
        let status = std::process::Command::new("git")
            .args(["init", "--quiet", "--initial-branch=draft"])
            .arg(directory.path())
            .status()
            .unwrap();
        assert!(status.success());
        let path = directory.path().join("message");
        std::fs::write(&path, "Preserve a near miss\n").unwrap();
        run(
            directory.path(),
            &["--write".into(), path.display().to_string()],
        )
        .unwrap();
        assert_eq!(
            std::fs::read_to_string(path).unwrap(),
            "Preserve a near miss\n"
        );
    }
}
