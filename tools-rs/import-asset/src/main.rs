//! CLI entrypoint, ported from `main()` in `tools/lib/import_asset.ts`.
//!
//! WHY THIS FILE EXISTS
//! --------------------
//! The asset builders under `tools/make/` shell out to this command to turn a
//! tracked PNG or MIDI into the exact bytes the ROM expects, so its stdout, its
//! error text, and its exit code are all part of the build contract.
//!
//! PORT NOTE: stdout is byte-identical to the TypeScript. Failures differ only
//! on stderr: Bun prints an uncaught-exception stack trace, this binary prints
//! `error: <message>`. Both exit non-zero, and the message itself is
//! character-for-character the same — including Node's `ENOENT: no such file or
//! directory, open '<path>'` wording, which is reproduced by hand below.

use std::path::Path;
use std::process::ExitCode;

use import_asset::{
    canonical_midi_json, gba_graphics, indexed_png, midi_events, self_test, sorted_json,
    AssetError, Report,
};

/// `Number(text)`.
///
/// PORT NOTE: `Number()` implements the ECMAScript StringNumericLiteral
/// grammar, not `f64::from_str`. It trims whitespace, maps the empty string to
/// `0`, accepts `0x`/`0o`/`0b` and a bare `Infinity`, rejects Rust-only spellings
/// like `inf` and `nan`, and yields `NaN` for anything else — and `NaN` is a
/// *valid* bpp as far as `gba_graphics` is concerned, so this must not error.
fn js_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(|character: char| {
        character.is_whitespace() || character == '\u{feff}'
    });
    if trimmed.is_empty() {
        return 0.0;
    }
    let (sign, body) = match trimmed.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, trimmed.strip_prefix('+').unwrap_or(trimmed)),
    };
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    if sign == 1.0 && trimmed.len() > 2 {
        let radix = match &trimmed[..2] {
            "0x" | "0X" => Some(16),
            "0o" | "0O" => Some(8),
            "0b" | "0B" => Some(2),
            _ => None,
        };
        if let Some(radix) = radix {
            return match u128::from_str_radix(&trimmed[2..], radix) {
                Ok(value) => value as f64,
                Err(_) => f64::NAN,
            };
        }
    }
    // Reject the spellings Rust accepts but JavaScript does not.
    if body.is_empty()
        || !body
            .chars()
            .all(|character| character.is_ascii_digit() || ".eE+-".contains(character))
    {
        return f64::NAN;
    }
    body.parse::<f64>().map(|value| sign * value).unwrap_or(f64::NAN)
}

fn option(args: &[String], name: &str) -> Result<String, AssetError> {
    match args.iter().position(|arg| arg == name) {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(AssetError(format!("{name} is required"))),
    }
}

/// PORT NOTE: matches Node's `readFileSync` error text so build logs read the
/// same before and after the port.
fn read_file(path: &str) -> Result<Vec<u8>, AssetError> {
    std::fs::read(path).map_err(|error| {
        if error.kind() == std::io::ErrorKind::NotFound {
            AssetError(format!("ENOENT: no such file or directory, open '{path}'"))
        } else {
            AssetError(format!("{error}, open '{path}'"))
        }
    })
}

fn write_file(path: &str, bytes: &[u8]) -> Result<(), AssetError> {
    // `dirname("out.bin")` is `"."`, which `mkdirSync(..., {recursive:true})`
    // accepts as a no-op; `create_dir_all(".")` does too.
    let parent = Path::new(path).parent().unwrap_or(Path::new("."));
    let parent = if parent.as_os_str().is_empty() { Path::new(".") } else { parent };
    std::fs::create_dir_all(parent)
        .map_err(|error| AssetError(format!("{error}, mkdir '{}'", parent.display())))?;
    std::fs::write(path, bytes)
        .map_err(|error| AssetError(format!("{error}, open '{path}'")))
}

fn run(mut args: Vec<String>) -> Result<(), AssetError> {
    if args.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("usage: import_asset.ts [--self-test] {{png,indexed,midi}} ...");
        return Ok(());
    }
    if args.iter().any(|arg| arg == "--self-test") {
        println!("{}", self_test()?);
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(());
        }
    }
    let command = args.first().cloned().unwrap_or_default();
    let input = args.get(1).cloned().unwrap_or_default();
    // `if (!input)` catches both a missing argument and an empty string.
    if input.is_empty() {
        return Err(AssetError("an asset command is required".to_string()));
    }
    match command.as_str() {
        "png" => {
            // Argument evaluation order is load-bearing: the TypeScript reads
            // the file and decodes it *before* `--tiles`/`--palette` are looked
            // up, so a corrupt PNG reports the decode failure even when those
            // flags are missing.
            let data = read_file(&input)?;
            let bpp = js_number(&option(&args, "--bpp")?);
            let (tiles, palette, report) = gba_graphics(&data, bpp)?;
            let tile_path = option(&args, "--tiles")?;
            let palette_path = option(&args, "--palette")?;
            write_file(&tile_path, &tiles)?;
            write_file(&palette_path, &palette)?;
            println!("{}", sorted_json(&report));
        }
        "indexed" => {
            let flag = if args.iter().any(|arg| arg == "-o") { "-o" } else { "--output" };
            let output = option(&args, flag)?;
            let image = indexed_png(&read_file(&input)?)?;
            let bytes: Vec<u8> = image.pixels.iter().map(|pixel| *pixel as u8).collect();
            write_file(&output, &bytes)?;
            let mut report = Report::default();
            report.set("width", f64::from(image.width));
            report.set("height", f64::from(image.height));
            report.set("pixels", image.pixels.len() as f64);
            report.set("palette_entries", image.palette.len() as f64);
            println!("{}", sorted_json(&report));
        }
        "midi" => {
            let flag = if args.iter().any(|arg| arg == "-o") { "-o" } else { "--output" };
            let output = option(&args, flag)?;
            let report = midi_events(&read_file(&input)?)?;
            write_file(&output, format!("{}\n", canonical_midi_json(&report)).as_bytes())?;
            println!("tracks={} events={}", report.tracks, report.events.len());
        }
        _ => return Err(AssetError("an asset command is required".to_string())),
    }
    Ok(())
}

fn main() -> ExitCode {
    match run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn js_number_follows_the_ecmascript_grammar() {
        assert_eq!(js_number("4"), 4.0);
        assert_eq!(js_number(" 8 "), 8.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("0x10"), 16.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(js_number("-Infinity"), f64::NEG_INFINITY);
        assert!(js_number("12abc").is_nan());
        assert!(js_number("inf").is_nan());
        assert!(js_number("nan").is_nan());
        assert!(js_number("-0x10").is_nan());
        assert_eq!(js_number("1.5"), 1.5);
    }

    #[test]
    fn option_requires_a_following_value() {
        let args: Vec<String> = ["png", "in.png", "--bpp"].iter().map(|s| s.to_string()).collect();
        assert_eq!(option(&args, "--bpp").unwrap_err().0, "--bpp is required");
        assert_eq!(option(&args, "--tiles").unwrap_err().0, "--tiles is required");
    }

    #[test]
    fn missing_input_is_rejected_before_any_io() {
        assert_eq!(
            run(vec!["png".to_string()]).unwrap_err().0,
            "an asset command is required"
        );
        assert_eq!(
            run(vec!["png".to_string(), String::new()]).unwrap_err().0,
            "an asset command is required"
        );
        assert_eq!(
            run(vec!["bogus".to_string(), "x".to_string()]).unwrap_err().0,
            "an asset command is required"
        );
    }

    #[test]
    fn a_missing_file_uses_nodes_enoent_wording() {
        assert_eq!(
            read_file("does/not/exist.png").unwrap_err().0,
            "ENOENT: no such file or directory, open 'does/not/exist.png'"
        );
    }

    #[test]
    fn png_decodes_before_output_flags_are_checked() {
        // A nonexistent input surfaces ENOENT even though --bpp is also absent,
        // because the read happens first.
        let args: Vec<String> =
            ["png", "missing.png"].iter().map(|s| s.to_string()).collect();
        assert!(run(args).unwrap_err().0.starts_with("ENOENT"));
    }
}
