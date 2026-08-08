//! CLI for the asset exporters: pull a byte range out of a ROM (or a loose
//! file) and write it as a PNG, printing a one-line JSON report.
//!
//! WHY THIS FILE EXISTS: the encoders in `lib.rs` are useless without a way to
//! aim them at a ROM offset, and the report line is what the surrounding
//! tooling reads back to confirm what was extracted. The refusal to write over
//! the input is the load-bearing part -- a mistyped `-o` would otherwise
//! destroy the ROM the whole project is measured against.
//!
//! Port of the `main` half of `tools/lib/export_asset.ts`.

use std::path::{Path, PathBuf};
use std::process::ExitCode;

use export_asset::{
    byte_png, cli_number, palette_png, palette_rgba_image, rgba_image, self_test, tile_png,
    ExportError, ROM_BASE,
};
use import_asset::{sorted_json, Report, Rgb};

const USAGE: &str = "usage: export_asset.ts [--self-test] {tiles,tiles-file,palette-file,bytes-file,rgba-file,palette-rgba-file,palette-rgba} ...";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(lines) => {
            for line in lines {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            // PORT NOTE: an uncaught `throw` in bun prints a stack trace to
            // stderr and exits 1. The parity harness compares the first line
            // and the exit code, not the trace, which is host-specific.
            eprintln!("error: {}", error.0);
            ExitCode::FAILURE
        }
    }
}

/// `args.findIndex((arg) => names.includes(arg))` followed by the next
/// element, which means the flag's value may itself look like a flag and the
/// error always names the *last* alias.
fn option(args: &[String], names: &[&str]) -> Result<String, ExportError> {
    match args.iter().position(|arg| names.contains(&arg.as_str())) {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(ExportError(format!(
            "{} is required",
            names.last().copied().unwrap_or_default()
        ))),
    }
}

fn has(args: &[String], name: &str) -> bool {
    args.iter().any(|arg| arg == name)
}

/// PORT NOTE: an unreadable input fails on both sides with exit 1, but the
/// wording is the host's -- node says `ENOENT: no such file or directory,
/// open '...'` and std::io says `No such file or directory (os error 2)`.
/// Nothing parses this text, so the divergence is accepted rather than faked.
fn read(path: &str) -> Result<Vec<u8>, ExportError> {
    std::fs::read(path).map_err(|error| ExportError(format!("{path}: {error}")))
}

/// Two paths are the same file when their resolved real paths match; when
/// either does not exist yet -- the normal case for the output -- fall back to
/// lexical resolution against the working directory.
fn same(left: &str, right: &str) -> bool {
    match (std::fs::canonicalize(left), std::fs::canonicalize(right)) {
        (Ok(left), Ok(right)) => left == right,
        _ => absolute(left) == absolute(right),
    }
}

fn absolute(path: &str) -> PathBuf {
    let path = Path::new(path);
    if path.is_absolute() {
        return normalize(path);
    }
    let mut base = std::env::current_dir().unwrap_or_default();
    base.push(path);
    normalize(&base)
}

/// `node:path.resolve` collapses `.` and `..` textually, without touching the
/// filesystem, so a `..` is removed even when the parent is a symlink.
fn normalize(path: &Path) -> PathBuf {
    let mut out = PathBuf::new();
    for part in path.components() {
        match part {
            std::path::Component::CurDir => {}
            std::path::Component::ParentDir => {
                out.pop();
            }
            other => out.push(other),
        }
    }
    out
}

fn palette_option(args: &[String]) -> Result<Option<Vec<Rgb>>, ExportError> {
    if !has(args, "--palette") {
        return Ok(None);
    }
    let path = option(args, &["--palette"])?;
    let decoded =
        import_asset::indexed_png(&read(&path)?).map_err(|error| ExportError(error.0))?;
    Ok(Some(decoded.palette))
}

fn width_option(args: &[String]) -> Result<f64, ExportError> {
    // PORT NOTE: the default is the *string* "16" fed through the same
    // `Number(...)` validation, not the number 16; the observable behaviour is
    // identical but keeping the shape makes the divergence check trivial.
    if has(args, "--width") {
        cli_number(&option(args, &["--width"])?)
    } else {
        cli_number("16")
    }
}

fn run(args: &[String]) -> Result<Vec<String>, ExportError> {
    if has(args, "-h") || has(args, "--help") {
        return Ok(vec![USAGE.to_string()]);
    }
    let mut lines = Vec::new();
    let mut args: Vec<String> = args.to_vec();
    if has(&args, "--self-test") {
        lines.push(self_test()?);
        args.retain(|arg| arg != "--self-test");
        if args.is_empty() {
            return Ok(lines);
        }
    }

    let command = args.first().cloned().unwrap_or_default();
    let input = args.get(1).cloned().unwrap_or_default();
    // PORT NOTE: `!command` in JS is true for the empty string as well as for
    // a missing argument, so an explicit `""` argument is rejected here too.
    if command.is_empty() || input.is_empty() {
        return Err(ExportError("an asset command is required".to_string()));
    }
    let output = option(&args, &["-o", "--output"])?;

    let (image, report): (Vec<u8>, Report) = if command == "tiles" || command == "palette-rgba" {
        let rom = read(&input)?;
        let address = cli_number(&option(&args, &["--address"])?)?;
        let size = cli_number(&option(&args, &["--size"])?)?;
        let start = address - f64::from(ROM_BASE);
        let end = start + size;
        if start < 0.0 || end > rom.len() as f64 || start >= end {
            return Err(ExportError(format!(
                "{} range is outside the ROM or empty",
                if command == "tiles" { "tile" } else { "palette" }
            )));
        }
        let slice = &rom[start as usize..end as usize];
        if same(&output, &input) {
            return Err(ExportError("refusing to overwrite the input ROM".to_string()));
        }
        if command == "tiles" {
            let palette = palette_option(&args)?;
            let bpp = cli_number(&option(&args, &["--bpp"])?)?;
            let columns = cli_number(&option(&args, &["--columns"])?)?;
            tile_png(slice, bpp, columns, palette.as_deref())?
        } else {
            palette_rgba_image(slice, width_option(&args)?)?
        }
    } else {
        if same(&output, &input) {
            return Err(ExportError("refusing to overwrite the input".to_string()));
        }
        let raw = read(&input)?;
        match command.as_str() {
            "tiles-file" => {
                let palette = palette_option(&args)?;
                let bpp = cli_number(&option(&args, &["--bpp"])?)?;
                let columns = cli_number(&option(&args, &["--columns"])?)?;
                tile_png(&raw, bpp, columns, palette.as_deref())?
            }
            "palette-file" => palette_png(&raw)?,
            "bytes-file" => byte_png(&raw, cli_number(&option(&args, &["--width"])?)?)?,
            "rgba-file" => rgba_image(&raw, cli_number(&option(&args, &["--width"])?)?)?,
            "palette-rgba-file" => palette_rgba_image(&raw, width_option(&args)?)?,
            _ => return Err(ExportError("an asset command is required".to_string())),
        }
    };

    if let Some(parent) = Path::new(&output).parent() {
        if !parent.as_os_str().is_empty() {
            std::fs::create_dir_all(parent)
                .map_err(|error| ExportError(format!("{}: {error}", parent.display())))?;
        }
    }
    std::fs::write(&output, &image).map_err(|error| ExportError(format!("{output}: {error}")))?;
    lines.push(sorted_json(&report));
    Ok(lines)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn owned(args: &[&str]) -> Vec<String> {
        args.iter().map(|arg| (*arg).to_string()).collect()
    }

    fn scratch(name: &str) -> PathBuf {
        let mut path = std::env::temp_dir();
        path.push(format!("export-asset-test-{name}"));
        let _ = std::fs::remove_dir_all(&path);
        std::fs::create_dir_all(&path).expect("scratch");
        path
    }

    #[test]
    fn help_wins_over_every_other_argument() {
        assert_eq!(run(&owned(&["tiles", "rom.gba", "--help"])).expect("help"), vec![USAGE]);
        assert_eq!(run(&owned(&["-h"])).expect("help"), vec![USAGE]);
    }

    #[test]
    fn a_bare_self_test_runs_and_stops_but_a_trailing_command_still_runs() {
        assert_eq!(run(&owned(&["--self-test"])).expect("self-test"), vec!["self-test=ok"]);
        let dir = scratch("selftest-then-command");
        let input = dir.join("bytes.bin");
        std::fs::write(&input, vec![7u8; 64]).expect("input");
        let output = dir.join("nested/out.png");
        let lines = run(&owned(&[
            "--self-test",
            "bytes-file",
            input.to_str().expect("utf8"),
            "-o",
            output.to_str().expect("utf8"),
            "--width",
            "8",
        ]))
        .expect("run");
        assert_eq!(lines[0], "self-test=ok");
        assert_eq!(lines[1], "{\"bytes\": 64, \"height\": 8, \"width\": 8}");
        // The nested output directory has to be created, not assumed.
        assert!(output.exists());
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn writing_over_the_input_is_refused_before_anything_is_encoded() {
        let dir = scratch("overwrite");
        let input = dir.join("bytes.bin");
        std::fs::write(&input, vec![7u8; 64]).expect("input");
        let path = input.to_str().expect("utf8");
        assert_eq!(
            run(&owned(&["bytes-file", path, "-o", path, "--width", "8"])),
            Err(ExportError("refusing to overwrite the input".to_string()))
        );
        // A different spelling of the same file is still the same file.
        let indirect = format!("{}/./bytes.bin", dir.to_str().expect("utf8"));
        assert_eq!(
            run(&owned(&["bytes-file", path, "-o", &indirect, "--width", "8"])),
            Err(ExportError("refusing to overwrite the input".to_string()))
        );
        assert_eq!(std::fs::read(&input).expect("input intact"), vec![7u8; 64]);
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn a_rom_range_is_checked_against_the_rom_length_not_just_the_address() {
        let dir = scratch("rom-range");
        let input = dir.join("rom.gba");
        std::fs::write(&input, vec![0u8; 256]).expect("rom");
        let path = input.to_str().expect("utf8");
        let output = dir.join("out.png");
        let output = output.to_str().expect("utf8");
        let args = |address: &str, size: &str| {
            owned(&[
                "tiles", path, "-o", output, "--address", address, "--size", size, "--bpp", "4",
                "--columns", "1",
            ])
        };
        // Below ROM_BASE, past the end, and empty all report the same way.
        for (address, size) in [("0x07ffffff", "32"), ("0x08000000", "512"), ("0x08000000", "0")] {
            assert_eq!(
                run(&args(address, size)),
                Err(ExportError("tile range is outside the ROM or empty".to_string())),
                "{address} {size}"
            );
        }
        assert!(run(&args("0x08000000", "32")).is_ok());
        // The message names the palette, not the tiles, for the other command.
        assert_eq!(
            run(&owned(&[
                "palette-rgba", path, "-o", output, "--address", "0x09000000", "--size", "32"
            ])),
            Err(ExportError("palette range is outside the ROM or empty".to_string()))
        );
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn a_missing_flag_names_the_last_alias() {
        let dir = scratch("missing-flag");
        let input = dir.join("bytes.bin");
        std::fs::write(&input, vec![7u8; 64]).expect("input");
        let path = input.to_str().expect("utf8");
        assert_eq!(
            run(&owned(&["bytes-file", path])),
            Err(ExportError("--output is required".to_string()))
        );
        // A flag in the final position has no value and counts as missing.
        assert_eq!(
            run(&owned(&["bytes-file", path, "-o"])),
            Err(ExportError("--output is required".to_string()))
        );
        assert_eq!(
            run(&owned(&["bytes-file", path, "-o", "/dev/null"])),
            Err(ExportError("--width is required".to_string()))
        );
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn an_unknown_or_absent_command_is_rejected() {
        for args in [vec![], vec!["tiles"], vec!["", "input"]] {
            assert_eq!(
                run(&owned(&args)),
                Err(ExportError("an asset command is required".to_string())),
                "{args:?}"
            );
        }
        // An unknown command still reads its input first, exactly as the
        // TypeScript does, so the fixture has to exist for the rejection to be
        // the thing under test rather than an ENOENT.
        let dir = scratch("unknown-command");
        let input = dir.join("bytes.bin");
        std::fs::write(&input, vec![7u8; 64]).expect("input");
        assert_eq!(
            run(&owned(&["wobble", input.to_str().expect("utf8"), "-o", "out.png"])),
            Err(ExportError("an asset command is required".to_string()))
        );
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }

    #[test]
    fn palette_rgba_defaults_to_sixteen_wide() {
        let dir = scratch("palette-width");
        let input = dir.join("palette.pal");
        std::fs::write(&input, vec![0u8; 64]).expect("palette");
        let path = input.to_str().expect("utf8");
        let output = dir.join("out.png");
        let output = output.to_str().expect("utf8");
        let lines = run(&owned(&["palette-rgba-file", path, "-o", output])).expect("default");
        assert_eq!(
            lines[0],
            "{\"height\": 2, \"palette_entries\": 32, \"pixels\": 32, \"width\": 16}"
        );
        let lines = run(&owned(&["palette-rgba-file", path, "-o", output, "--width", "8"]))
            .expect("explicit");
        assert_eq!(
            lines[0],
            "{\"height\": 4, \"palette_entries\": 32, \"pixels\": 32, \"width\": 8}"
        );
        std::fs::remove_dir_all(&dir).expect("cleanup");
    }
}
