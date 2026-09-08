//! Execute an explicit tool command without choosing a compiler or its flags.
use std::path::Path;
use std::process::Command;

pub fn run<S: AsRef<str>>(command: &[S], cwd: impl AsRef<Path>) -> Result<String, String> {
    let program = command.first().ok_or("run: empty command")?.as_ref();
    let name = Path::new(program)
        .file_name()
        .and_then(|name| name.to_str())
        .unwrap_or(program);
    let output = Command::new(program)
        .args(command[1..].iter().map(AsRef::as_ref))
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{name}: {error}"))?;
    let stdout = String::from_utf8_lossy(&output.stdout).into_owned();
    if output.status.success() {
        return Ok(stdout);
    }
    let stderr = String::from_utf8_lossy(&output.stderr);
    let detail = if stderr.trim().is_empty() {
        stdout.trim()
    } else {
        stderr.trim()
    };
    if detail.is_empty() {
        Err(format!("{name} failed"))
    } else {
        Err(format!("{name} failed: {detail}"))
    }
}

#[cfg(all(test, unix))]
mod tests {
    use super::*;

    #[test]
    fn preserves_arguments_and_stdout_without_shell_interpretation() {
        let cwd = std::env::temp_dir();
        assert_eq!(
            run(
                &["/bin/sh", "-c", "printf '%s' \"$1\"", "test", "a b; $HOME"],
                &cwd
            )
            .unwrap(),
            "a b; $HOME"
        );
        let expected = cwd.canonicalize().unwrap();
        assert_eq!(
            run(&["/bin/pwd", "-P"], &cwd).unwrap().trim(),
            expected.to_str().unwrap()
        );
    }

    #[test]
    fn reports_failures_and_rejects_empty_commands() {
        let cwd = std::env::temp_dir();
        assert!(run::<&str>(&[], &cwd)
            .unwrap_err()
            .contains("empty command"));
        assert!(run(&["/nonexistent-psynergy-tool"], &cwd)
            .unwrap_err()
            .starts_with("nonexistent-psynergy-tool:"));
        for (script, expected) in [
            ("printf out; printf err >&2; exit 1", "sh failed: err"),
            ("printf out; printf ' ' >&2; exit 1", "sh failed: out"),
            ("exit 1", "sh failed"),
            ("kill -TERM $$", "sh failed"),
        ] {
            assert_eq!(run(&["/bin/sh", "-c", script], &cwd).unwrap_err(), expected);
        }
    }
}
