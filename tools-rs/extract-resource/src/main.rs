//! CLI entrypoint, ported from `main()` in `tools/lib/extract_resource.ts`.
//!
//! PORT NOTE: stdout is byte-identical to the TypeScript. Usage failures differ
//! only on stderr: Node prints an uncaught-exception stack trace, this binary
//! prints `error: <message>`. Both exit non-zero, and the message text itself
//! is character-for-character the same.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use extract_resource::{decode, self_test, ResourceKind, ROM_BASE, TABLE};

const VALUE_OPTIONS: [&str; 7] =
    ["--id", "--address", "--format", "--input-end", "--max-output", "-o", "--output"];

const USAGE: &str = "usage: extract_resource.ts [ROM] [--id ID | --address ADDRESS] [--format {auto,general,palette}] [-o OUTPUT | --verify-only]";

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
    let radix_prefix = if text.len() > 2 { Some(&text[..2]) } else { None };
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
            let resolve = |path: &str| {
                std::path::absolute(path).unwrap_or_else(|_| PathBuf::from(path))
            };
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
                    args.get(index + 1) == Some(*arg)
                        && VALUE_OPTIONS.contains(&previous.as_str())
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
            let parent = if parent.as_os_str().is_empty() { Path::new(".") } else { parent };
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

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_number_matches_javascript() {
        assert_eq!(js_number("0x8320000"), 137_494_528.0);
        assert_eq!(js_number("0X10"), 16.0);
        assert_eq!(js_number("0b1010"), 10.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number("  42  "), 42.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   "), 0.0);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number("-5"), -5.0);
        assert_eq!(js_number("12.0"), 12.0);
        assert!(js_number("12.5").fract() != 0.0);
        assert!(js_number("0x").is_nan());
        assert!(js_number("0xzz").is_nan());
        assert!(js_number("inf").is_nan());
        assert!(js_number("nan").is_nan());
        assert!(js_number("NaN").is_nan());
        assert!(js_number("12abc").is_nan());
        assert!(js_number("-0x10").is_nan());
        assert_eq!(js_number("Infinity"), f64::INFINITY);
    }

    #[test]
    fn integer_rejects_non_integers_with_the_javascript_message() {
        assert_eq!(integer(Some("0x20"), "--address"), Ok(32));
        assert_eq!(
            integer(Some("1.5"), "--address"),
            Err("invalid integer: 1.5".into())
        );
        assert_eq!(
            integer(Some("Infinity"), "--address"),
            Err("invalid integer: Infinity".into())
        );
        assert_eq!(integer(None, "--address"), Err("--address is required".into()));
    }

    #[test]
    fn option_lookup_matches_index_plus_one() {
        let args: Vec<String> =
            ["rom.gba", "--id", "7", "--format"].iter().map(|s| s.to_string()).collect();
        assert_eq!(option(&args, "--id"), Some("7"));
        assert_eq!(option(&args, "--format"), None);
        assert_eq!(option(&args, "--address"), None);
    }

    #[test]
    fn usage_errors_match_the_typescript() {
        let args = |list: &[&str]| list.iter().map(|s| s.to_string()).collect::<Vec<_>>();
        assert_eq!(
            run(args(&["rom.gba", "--verify-only"])),
            Err("one of --id or --address is required".into())
        );
        assert_eq!(
            run(args(&["rom.gba", "--id", "1", "--address", "2", "--verify-only"])),
            Err("--id and --address are mutually exclusive".into())
        );
        assert_eq!(
            run(args(&["rom.gba", "--id", "1", "--verify-only", "-o", "out.bin"])),
            Err("--verify-only and --output are mutually exclusive".into())
        );
        assert_eq!(
            run(args(&["rom.gba", "--id", "1"])),
            Err("--output is required unless --verify-only is used".into())
        );
        assert_eq!(
            run(args(&["--id", "1", "--verify-only"])),
            Err("ROM is required unless only --self-test is used".into())
        );
        assert_eq!(
            run(args(&["rom.gba", "--id", "1", "--verify-only", "--max-output", "-1"])),
            Err("--max-output cannot be negative".into())
        );
    }

    #[test]
    fn rom_path_is_not_confused_with_an_option_value() {
        // "7" follows --id, so it must not be picked as the ROM path; the
        // missing ROM is reported instead.
        let args: Vec<String> = ["--id", "7", "--verify-only"].iter().map(|s| s.to_string()).collect();
        assert_eq!(run(args), Err("ROM is required unless only --self-test is used".into()));
    }
}
