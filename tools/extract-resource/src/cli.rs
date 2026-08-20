//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

//! CLI entrypoint, ported from `main()` in `tools/lib/extract_resource.ts`.
//!
//! PORT NOTE: stdout is byte-identical to the TypeScript. Usage failures differ
//! only on stderr: Node prints an uncaught-exception stack trace, this binary
//! prints `error: <message>`. Both exit non-zero, and the message text itself
//! is character-for-character the same.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use crate::{decode, self_test, ResourceKind, ROM_BASE, TABLE};

const VALUE_OPTIONS: [&str; 7] = [
    "--id",
    "--address",
    "--format",
    "--input-end",
    "--max-output",
    "-o",
    "--output",
];

const USAGE: &str = "usage: extract-resource [ROM] [--id ID | --address ADDRESS] [--format {auto,general,palette}] [-o OUTPUT | --verify-only]";

/// `Number(text)` from JavaScript, returning NaN for anything JS rejects.
///
/// PORT NOTE: this matters — the TypeScript accepts `--address 0x8320000`
/// because `Number` understands `0x`/`0o`/`0b` prefixes and exponent notation.
/// `f64::from_str` alone would reject the hex form and accept `inf`/`nan`,
/// which JS does not, so both directions are handled explicitly.
fn js_number(text: &str) -> f64 {
    let text = text.trim_matches(|c: char| c.is_whitespace() || c == '\u{feff}');
    if text.is_empty() {
        return 0.0;
    }
    let radix_prefix = if text.len() > 2 {
        Some(&text[..2])
    } else {
        None
    };
    if let Some(prefix) = radix_prefix {
        let radix = match prefix {
            "0x" | "0X" => Some(16u32),
            "0o" | "0O" => Some(8),
            "0b" | "0B" => Some(2),
            _ => None,
        };
        if let Some(radix) = radix {
            let mut value = 0f64;
            for character in text[2..].chars() {
                match character.to_digit(radix) {
                    Some(digit) => value = value * f64::from(radix) + f64::from(digit),
                    None => return f64::NAN,
                }
            }
            return value;
        }
    }
    match text {
        "Infinity" | "+Infinity" => return f64::INFINITY,
        "-Infinity" => return f64::NEG_INFINITY,
        _ => {}
    }
    // Rust accepts `inf`, `infinity`, `nan` (any case); JavaScript does not.
    let lower = text.to_ascii_lowercase();
    let bare = lower.trim_start_matches(['+', '-']);
    if bare.starts_with("inf") || bare.starts_with("nan") {
        return f64::NAN;
    }
    text.parse::<f64>().unwrap_or(f64::NAN)
}

fn integer(text: Option<&str>, label: &str) -> Result<i64, String> {
    let text = match text {
        Some(text) => text,
        None => return Err(format!("{label} is required")),
    };
    let result = js_number(text);
    if !result.is_finite() || result.fract() != 0.0 {
        return Err(format!("invalid integer: {text}"));
    }
    Ok(result as i64)
}

fn option<'a>(args: &'a [String], name: &str) -> Option<&'a str> {
    let index = args.iter().position(|arg| arg == name)?;
    args.get(index + 1).map(String::as_str)
}

/// `option(...) ?? undefined` folded with JS truthiness: an empty string is
/// falsy in the TypeScript, so it is treated as absent here too.
fn truthy(value: Option<&str>) -> Option<&str> {
    value.filter(|text| !text.is_empty())
}

fn same(a: &str, b: &str) -> bool {
    match (std::fs::canonicalize(a), std::fs::canonicalize(b)) {
        (Ok(left), Ok(right)) => left == right,
        _ => {
            let resolve =
                |path: &str| std::path::absolute(path).unwrap_or_else(|_| PathBuf::from(path));
            resolve(a) == resolve(b)
        }
    }
}

fn run(mut args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        self_test()?;
        println!("self-test=ok");
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    let rom_path = args
        .iter()
        .find(|arg| {
            !arg.starts_with('-')
                && !args.iter().enumerate().any(|(index, previous)| {
                    args.get(index + 1) == Some(*arg) && VALUE_OPTIONS.contains(&previous.as_str())
                })
        })
        .cloned()
        .ok_or("ROM is required unless only --self-test is used")?;

    let id_text = option(&args, "--id");
    let address_text = option(&args, "--address");
    if id_text.is_none() && address_text.is_none() {
        return Err("one of --id or --address is required".into());
    }
    if id_text.is_some() && address_text.is_some() {
        return Err("--id and --address are mutually exclusive".into());
    }
    let verify = args.iter().any(|arg| arg == "--verify-only");
    let output_path = truthy(option(&args, "-o")).or_else(|| truthy(option(&args, "--output")));
    if verify && output_path.is_some() {
        return Err("--verify-only and --output are mutually exclusive".into());
    }
    if !verify && output_path.is_none() {
        return Err("--output is required unless --verify-only is used".into());
    }
    let maximum = match option(&args, "--max-output") {
        None => 0x400_0000i64,
        text => integer(text, "--max-output")?,
    };
    if maximum < 0 {
        return Err("--max-output cannot be negative".into());
    }
    let data = std::fs::read(&rom_path).map_err(|error| error.to_string())?;
    let rom_end = i64::from(ROM_BASE) + data.len() as i64;
    let address: i64 = if let Some(id_text) = id_text {
        let id = integer(Some(id_text), "--id")?;
        if id < 0 {
            return Err("--id cannot be negative".into());
        }
        let entry = i64::from(TABLE) - i64::from(ROM_BASE) + id.saturating_mul(4);
        if entry < 0 || entry + 4 > data.len() as i64 {
            return Err("resource ID lies outside the ROM pointer table".into());
        }
        let entry = entry as usize;
        i64::from(u32::from_le_bytes([
            data[entry],
            data[entry + 1],
            data[entry + 2],
            data[entry + 3],
        ]))
    } else {
        integer(address_text, "--address")?
    };
    let input_end = match option(&args, "--input-end") {
        None => rom_end,
        text => integer(text, "--input-end")?,
    };
    if !(i64::from(ROM_BASE) <= address && address < input_end && input_end <= rom_end) {
        return Err("source/input bound is outside ROM or empty".into());
    }
    let format = match option(&args, "--format").unwrap_or("auto") {
        "auto" => None,
        "general" => Some(ResourceKind::General),
        "palette" => Some(ResourceKind::Palette),
        _ => return Err("invalid resource format".into()),
    };
    let (kind, output, cursor) = decode(
        &data,
        (address - i64::from(ROM_BASE)) as usize,
        (input_end - i64::from(ROM_BASE)) as usize,
        maximum as u64,
        format,
    )
    .map_err(|error| error.0)?;
    let end_address = i64::from(ROM_BASE) + cursor as i64;
    if !verify {
        if let Some(output_path) = output_path {
            if same(output_path, &rom_path) {
                return Err("refusing to overwrite the input ROM".into());
            }
            let parent = Path::new(output_path).parent().unwrap_or(Path::new(""));
            let parent = if parent.as_os_str().is_empty() {
                Path::new(".")
            } else {
                parent
            };
            std::fs::create_dir_all(parent).map_err(|error| error.to_string())?;
            std::fs::write(output_path, &output).map_err(|error| error.to_string())?;
        }
    }
    println!(
        "format={kind} source={address:08x} end={end:08x} consumed=0x{consumed:x} decoded=0x{decoded:x}",
        end = end_address,
        consumed = end_address - address,
        decoded = output.len()
    );
    Ok(())
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    match run(arguments.to_vec()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
