//! A command that takes an argument slice and never reads it.
//!
//! WHY THIS EXISTS. The failure is silent and reads as success. `decomp-targets`
//! took `arguments: &[String]`, ignored it, and ran its self-test, so
//! `check decomp-targets --json` printed the self-test and exited 0 exactly as
//! though the flag had been honoured. The same defect had already appeared in
//! three asset adapters, which read the HOST's `std::env::args()` instead of the
//! slice they were handed and so produced truncated output for every subcommand
//! but the first.
//!
//! Twice is a class, not an accident, and neither instance was caught by
//! anything: the compiler's unused-variable warning was one of eight printing on
//! every build, and the tree still passed `verify`.
//!
//! WHAT COUNTS AS READING IT. Any mention of the parameter by name inside the
//! body. A command that genuinely takes no arguments should either name the
//! parameter `_arguments`, which says so, or reject what it cannot honour --
//! which is better, because the caller learns their flag did nothing.

use std::collections::BTreeSet;
use std::path::Path;

use crate::Report;

/// A dispatch entry point: `fn entry(arguments: &[String])` and its variants.
///
/// Matched by hand rather than by parsing Rust: the shape is rigid, and a lint
/// that needs a syntax tree to find a two-line function is a lint nobody will
/// keep working.
fn entry_points(text: &str) -> Vec<(usize, String, String)> {
    let mut found = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let trimmed = line.trim_start();
        let Some(rest) = trimmed
            .strip_prefix("pub fn ")
            .or_else(|| trimmed.strip_prefix("fn "))
        else {
            continue;
        };
        let Some((name, tail)) = rest.split_once('(') else {
            continue;
        };
        if !matches!(name, "entry" | "run" | "main") {
            continue;
        }
        // The first parameter must be a slice or Vec of String, which is what
        // every dispatch host hands a command.
        let parameter = tail.split(',').next().unwrap_or("");
        let Some((binding, kind)) = parameter.split_once(':') else {
            continue;
        };
        let kind = kind.trim();
        if !(kind.contains("[String]") || kind.contains("Vec<String>")) {
            continue;
        }
        // `mut args` binds `args`; the qualifier is not part of the name.
        let binding = binding.trim().strip_prefix("mut ").unwrap_or(binding.trim()).trim();
        if binding.is_empty() || binding.starts_with('_') || binding.starts_with('&') {
            continue;
        }
        found.push((index + 1, name.to_string(), binding.to_string()));
    }
    found
}

/// The body of the function starting at `line`, by brace balance.
fn body_after(text: &str, line: usize) -> String {
    let lines: Vec<&str> = text.lines().collect();
    let mut depth = 0i32;
    let mut started = false;
    let mut body = String::new();
    for row in lines.iter().skip(line.saturating_sub(1)) {
        for character in row.chars() {
            if character == '{' {
                depth += 1;
                started = true;
            } else if character == '}' {
                depth -= 1;
            }
        }
        body.push_str(row);
        body.push('\n');
        if started && depth <= 0 {
            break;
        }
    }
    body
}

fn mentions(body: &str, binding: &str) -> bool {
    let bytes = body.as_bytes();
    let needle = binding.as_bytes();
    let word = |byte: u8| byte == b'_' || byte.is_ascii_alphanumeric();
    let mut index = 0usize;
    // Skip the signature line, so the declaration itself is not a mention.
    let start = body.find('{').map(|at| at + 1).unwrap_or(0);
    while let Some(at) = body[index.max(start)..].find(binding) {
        let absolute = index.max(start) + at;
        let before_ok = absolute == 0 || !word(bytes[absolute - 1]);
        let after = absolute + needle.len();
        let after_ok = after >= bytes.len() || !word(bytes[after]);
        if before_ok && after_ok {
            return true;
        }
        index = absolute + 1;
        if index >= body.len() {
            break;
        }
    }
    false
}

pub fn analyse(root: &Path) -> Result<Report, String> {
    let mut report = Report::new("ignored-arguments");
    let mut sources: Vec<std::path::PathBuf> = Vec::new();
    let tools = root.join("tools");
    let mut stack = vec![tools];
    while let Some(directory) = stack.pop() {
        let Ok(entries) = std::fs::read_dir(&directory) else {
            continue;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() {
                if path.file_name().is_some_and(|name| name == "target") {
                    continue;
                }
                stack.push(path);
            } else if path.extension().is_some_and(|ext| ext == "rs") {
                sources.push(path);
            }
        }
    }
    sources.sort();

    let mut checked = 0usize;
    let mut offenders: BTreeSet<(String, usize, String)> = BTreeSet::new();
    for path in &sources {
        let Ok(text) = std::fs::read_to_string(path) else {
            continue;
        };
        for (line, name, binding) in entry_points(&text) {
            checked += 1;
            let body = body_after(&text, line);
            if !mentions(&body, &binding) {
                let relative = path.strip_prefix(root).unwrap_or(path);
                offenders.insert((
                    relative.display().to_string(),
                    line,
                    format!(
                        "{name} takes `{binding}` and never reads it -- reject what it \
                         cannot honour, or name it `_{binding}`"
                    ),
                ));
            }
        }
    }

    // A floor, so a scanner that stops matching cannot pass by finding nothing.
    report.count_with_floor("entry_points_checked", checked, 40);
    for (file, line, message) in offenders {
        report.findings.push(crate::Finding { file, line, message });
    }
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_command_that_reads_its_arguments_is_fine() {
        let text = "pub fn entry(arguments: &[String]) -> ExitCode {\n    parse(arguments);\n}\n";
        let points = entry_points(text);
        assert_eq!(points.len(), 1);
        assert!(mentions(&body_after(text, points[0].0), &points[0].2));
    }

    #[test]
    fn a_command_that_ignores_them_is_caught() {
        let text = "pub fn entry(arguments: &[String]) -> ExitCode {\n    self_test()\n}\n";
        let points = entry_points(text);
        assert_eq!(points.len(), 1);
        assert!(!mentions(&body_after(text, points[0].0), &points[0].2));
    }

    #[test]
    fn an_underscored_binding_says_it_means_to_ignore_them() {
        let text = "pub fn entry(_arguments: &[String]) -> ExitCode {\n    self_test()\n}\n";
        assert!(entry_points(text).is_empty());
    }

    #[test]
    fn a_substring_is_not_a_mention() {
        // `arguments_used` must not count as a use of `arguments`.
        let text =
            "pub fn entry(arguments: &[String]) -> ExitCode {\n    let arguments_used = 0;\n}\n";
        let points = entry_points(text);
        assert!(!mentions(&body_after(text, points[0].0), &points[0].2));
    }

    #[test]
    fn a_mut_binding_is_read_by_its_name_not_its_qualifier() {
        let text = "pub fn run(mut args: Vec<String>) -> ExitCode {\n    args.pop();\n}\n";
        let points = entry_points(text);
        assert_eq!(points.len(), 1);
        assert_eq!(points[0].2, "args");
        assert!(mentions(&body_after(text, points[0].0), &points[0].2));
    }

    #[test]
    fn a_function_taking_something_else_is_not_an_entry_point() {
        let text = "pub fn entry(root: &Path) -> ExitCode {\n    go()\n}\n";
        assert!(entry_points(text).is_empty());
    }
}
