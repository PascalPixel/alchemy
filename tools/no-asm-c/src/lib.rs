pub mod cli;

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

pub const SOURCE_ROOTS: [&str; 1] = ["games"];

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
            findings.push(Finding { file: file.to_string(), line: code[..start].bytes().filter(|byte| *byte == b'\n').count() + 1, token: word.to_string() });
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
    let mut names: Vec<PathBuf> = fs::read_dir(directory)?.filter_map(Result::ok).map(|entry| entry.path()).collect();
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

/// Exercise the lexical gate and its source-file boundary with a real fixture.
pub fn self_test() -> Result<String, String> {
    let forbidden = find_forbidden("fixture.c", "void f(void) { __asm__(\"nop\"); }\n");
    if forbidden.len() != 1 || forbidden[0].token != "__asm__" {
        return Err("no-asm-c self-test missed inline assembly".into());
    }
    if !find_forbidden("fixture.c", "const char *text = \"asm(\\\"\\\")\";\n").is_empty() {
        return Err("no-asm-c self-test flagged assembly in a string".into());
    }
    let directory = std::env::temp_dir().join(format!("alchemy-no-asm-c-self-test-{}", std::process::id()));
    let _ = fs::remove_dir_all(&directory);
    let result = (|| {
        fs::create_dir_all(directory.join("nested")).map_err(|error| error.to_string())?;
        fs::write(directory.join("nested/a.c"), "int a;\n").map_err(|error| error.to_string())?;
        fs::write(directory.join("b.h"), "int b;\n").map_err(|error| error.to_string())?;
        fs::write(directory.join("notes.md"), "not C\n").map_err(|error| error.to_string())?;
        let files = source_files(&directory).map_err(|error| error.to_string())?;
        if files.len() != 2 || files.iter().any(|path| path.extension().and_then(|x| x.to_str()) == Some("md")) {
            return Err("no-asm-c self-test source boundary changed".into());
        }
        Ok("self-test=ok files=2".into())
    })();
    let _ = fs::remove_dir_all(&directory);
    result
}
