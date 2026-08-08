// Hard gate: C and header sources may never use assembly escape hatches.
//
// Ported from tools/check/no_asm_c.ts. The scanner blanks comments and string
// and character literals before matching, so a token named inside prose or
// inside a string is not a finding, while `asm`, `__asm`, `__asm__`, and
// `__asm_` in real code all are. Newlines survive blanking so reported line
// numbers point at the source, not at the blanked copy.

use std::fs;
use std::io;
use std::path::{Path, PathBuf};

pub const SOURCE_ROOTS: [&str; 5] = ["assets", "games", "include", "semantic", "exact"];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Finding {
    pub file: String,
    pub line: usize,
    pub token: String,
}

#[derive(Clone, Copy, PartialEq, Eq)]
enum State {
    Code,
    Line,
    Block,
    Str,
    Char,
}

/// Replace comments and literals with spaces, preserving newlines and byte
/// positions so an offset in the result is an offset in the input.
pub fn code_only(text: &str) -> String {
    let bytes = text.as_bytes();
    let mut output = String::with_capacity(text.len());
    let mut state = State::Code;
    let mut index = 0;
    while index < bytes.len() {
        let char = bytes[index];
        let next = bytes.get(index + 1).copied();
        match state {
            State::Code => match (char, next) {
                (b'/', Some(b'/')) => {
                    output.push_str("  ");
                    index += 2;
                    state = State::Line;
                    continue;
                }
                (b'/', Some(b'*')) => {
                    output.push_str("  ");
                    index += 2;
                    state = State::Block;
                    continue;
                }
                (b'"', _) => {
                    output.push(' ');
                    state = State::Str;
                }
                (b'\'', _) => {
                    output.push(' ');
                    state = State::Char;
                }
                _ => output.push(char as char),
            },
            State::Line => {
                if char == b'\n' {
                    output.push('\n');
                    state = State::Code;
                } else {
                    output.push(' ');
                }
            }
            State::Block => {
                if char == b'*' && next == Some(b'/') {
                    output.push_str("  ");
                    index += 2;
                    state = State::Code;
                    continue;
                }
                output.push(if char == b'\n' { '\n' } else { ' ' });
            }
            State::Str | State::Char => {
                // A backslash escapes the next byte, including a closing quote
                // and including a line continuation.
                if char == b'\\' && next.is_some() {
                    output.push_str(if next == Some(b'\n') { " \n" } else { "  " });
                    index += 2;
                    continue;
                }
                let closes = (state == State::Str && char == b'"') || (state == State::Char && char == b'\'');
                if closes {
                    output.push(' ');
                    state = State::Code;
                } else {
                    output.push(if char == b'\n' { '\n' } else { ' ' });
                }
            }
        }
        index += 1;
    }
    output
}

fn is_word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// The TypeScript matched `\b(?:asm|__asm_*)\b`. In Rust that is: a word
/// starting at a boundary that is either exactly `asm` or `__asm` followed by
/// any run of underscores, and nothing else.
fn forbidden_token(word: &str) -> bool {
    if word == "asm" {
        return true;
    }
    // `\b` before `__asm` sits between a non-word char and `_`, so a longer
    // identifier ending in `__asm` never matches — the boundary fails.
    match word.strip_prefix("__asm") {
        Some(tail) => tail.bytes().all(|byte| byte == b'_'),
        None => false,
    }
}

pub fn find_forbidden(file: &str, text: &str) -> Vec<Finding> {
    let code = code_only(text);
    let bytes = code.as_bytes();
    let mut findings = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        if !is_word_byte(bytes[index]) {
            index += 1;
            continue;
        }
        let start = index;
        while index < bytes.len() && is_word_byte(bytes[index]) {
            index += 1;
        }
        let word = &code[start..index];
        if forbidden_token(word) {
            findings.push(Finding {
                file: file.to_string(),
                line: code[..start].bytes().filter(|byte| *byte == b'\n').count() + 1,
                token: word.to_string(),
            });
        }
    }
    findings
}

/// Every `.c` and `.h` under `directory`, sorted, recursing depth-first. A
/// missing root yields nothing rather than failing: the caller checks that the
/// overall file count is nonzero.
pub fn source_files(directory: &Path) -> io::Result<Vec<PathBuf>> {
    if !directory.is_dir() {
        return Ok(Vec::new());
    }
    let mut names: Vec<PathBuf> =
        fs::read_dir(directory)?.filter_map(Result::ok).map(|entry| entry.path()).collect();
    names.sort();
    let mut files = Vec::new();
    for path in names {
        if path.is_dir() {
            files.extend(source_files(&path)?);
        } else if matches!(path.extension().and_then(|e| e.to_str()), Some("c") | Some("h")) {
            files.push(path);
        }
    }
    Ok(files)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn every_escape_spelling_is_caught() {
        for source in ["asm(\"\")", "__asm(\"\")", "__asm__(\"r5\")", "__asm_(\"\")"] {
            assert_eq!(find_forbidden("bad.c", source).len(), 1, "missed {source}");
        }
    }

    #[test]
    fn comments_strings_and_longer_names_are_not_findings() {
        let clean = "/* asm(\"\") */\nconst char *s = \"__asm__(x)\";\nint assembly = 1;\n";
        assert_eq!(find_forbidden("clean.c", clean), Vec::new());
        assert_eq!(find_forbidden("clean.c", "// asm(\"\")\n"), Vec::new());
        assert_eq!(find_forbidden("clean.c", "int my_asm = 1;\n"), Vec::new());
        assert_eq!(find_forbidden("clean.c", "int asmx = 1;\n"), Vec::new());
    }

    #[test]
    fn line_numbers_point_at_the_source() {
        let text = "int a;\n/* filler\n   filler */\nasm(\"\");\n";
        let findings = find_forbidden("x.c", text);
        assert_eq!(findings.len(), 1);
        assert_eq!(findings[0].line, 4);
        assert_eq!(findings[0].token, "asm");
    }

    #[test]
    fn an_escaped_quote_does_not_end_a_literal() {
        // If the `\"` were treated as a close, the `asm` after it would be read
        // as code and wrongly reported.
        assert_eq!(find_forbidden("x.c", "const char *s = \"a\\\" asm(\\\"\\\") b\";\n"), Vec::new());
    }

    #[test]
    fn a_line_continuation_inside_a_literal_is_survived() {
        let text = "const char *s = \"a\\\nasm\";\nint b;\n";
        assert_eq!(find_forbidden("x.c", text), Vec::new());
    }

    #[test]
    fn source_files_are_sorted_and_filtered() {
        let dir = std::env::temp_dir().join(format!("alchemy-no-asm-c-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("sub")).unwrap();
        for name in ["b.c", "a.h", "notes.md"] {
            fs::write(dir.join(name), b"").unwrap();
        }
        fs::write(dir.join("sub").join("c.c"), b"").unwrap();

        let files = source_files(&dir).unwrap();
        let names: Vec<String> =
            files.iter().map(|p| p.strip_prefix(&dir).unwrap().to_string_lossy().into()).collect();
        assert_eq!(names, vec!["a.h", "b.c", "sub/c.c"]);
        fs::remove_dir_all(&dir).unwrap();
    }
}
