//! Differential-parity driver for the Rust `verify` port.
//!
//! Emits one JSON object per source, with exactly the key order and value
//! formatting that `harness/ts_verify.ts` emits, so `diff` is the comparison.
//!
//! usage: parity <repoRoot> <romPath> <outDir> <listFile>

use std::fs;
use std::path::Path;

use verify::routing::CompilerTarget;
use verify::sha256::sha256_hex;
use verify::verify_details;

/// Mirrors `category()` in the TypeScript driver. Categories that only the
/// Rust side can produce (the added hard failures) get their own names, so a
/// divergence is visible rather than folded into "other".
fn category(message: &str) -> String {
    if message.contains("unsupported external symbol") {
        return "unsupported-external-symbol".into();
    }
    if message.contains("missing linked symbol") {
        return "missing-linked-symbol".into();
    }
    if let Some(name) = tool_failure(message) {
        return format!("tool-failed:{name}");
    }
    for (needle, name) in [
        ("8-digit lowercase hex", "bad-address-filename"),
        ("has zero size", "zero-size-symbol"),
        ("runs past the end of the", "rom-slice-out-of-range"),
        ("is below the ROM base", "rom-slice-out-of-range"),
        ("refusing to compare", "incomparable"),
        ("bytes but", "short-binary"),
    ] {
        if message.contains(needle) {
            return name.into();
        }
    }
    if message.contains("No such file or directory") {
        return "enoent".into();
    }
    "other".into()
}

/// `^([A-Za-z0-9_.-]+) failed` without pulling in a regex engine.
fn tool_failure(message: &str) -> Option<&str> {
    let (head, _) = message.split_once(" failed")?;
    if head.is_empty()
        || !head
            .bytes()
            .all(|b| b.is_ascii_alphanumeric() || b == b'_' || b == b'.' || b == b'-')
    {
        return None;
    }
    Some(head)
}

fn json_string(value: &str) -> String {
    let mut out = String::with_capacity(value.len() + 2);
    out.push('"');
    for character in value.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            control if (control as u32) < 0x20 => {
                out.push_str(&format!("\\u{:04x}", control as u32));
            }
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    let [_root, rom_path, out_dir, list_file] = arguments.as_slice() else {
        eprintln!("usage: parity <repoRoot> <romPath> <outDir> <listFile>");
        std::process::exit(2);
    };
    // Exit 1, not a panic: the TypeScript driver dies with exit 1 on an
    // unreadable ROM, and failure agreement is about the exit code.
    let Ok(rom) = fs::read(rom_path) else {
        eprintln!("cannot read {rom_path}");
        std::process::exit(1);
    };
    let Ok(list) = fs::read_to_string(list_file) else {
        eprintln!("cannot read {list_file}");
        std::process::exit(1);
    };
    for source in list.lines().filter(|line| !line.is_empty()) {
        let name = Path::new(source)
            .file_name()
            .map(|n| n.to_string_lossy().into_owned())
            .unwrap_or_else(|| source.to_string());
        match verify_details(source, &rom, out_dir, CompilerTarget::Gs1) {
            Ok((actual, expected, size)) => println!(
                "{{\"source\":{},\"status\":\"ok\",\"size\":{},\"matched\":{},\"actual\":{},\"expected\":{},\"actualLength\":{},\"expectedLength\":{}}}",
                json_string(&name),
                size,
                actual == expected,
                json_string(&sha256_hex(&actual)),
                json_string(&sha256_hex(&expected)),
                actual.len(),
                expected.len(),
            ),
            Err(message) => println!(
                "{{\"source\":{},\"status\":\"error\",\"category\":{},\"message\":{}}}",
                json_string(&name),
                json_string(&category(&message)),
                json_string(&message),
            ),
        }
    }
}
