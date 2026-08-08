// CLI for the early-runtime data package. Ported from `main()` in
// tools/make/early_runtime_data.ts -- argument shapes, stdout lines and the
// usage text are verbatim, including the `.ts` filename in the usage string,
// which callers grep for.
//
// PORT NOTE: Bun prints an uncaught `Error` as `error: <message>` followed by a
// source snippet and a stack trace, then exits 1. This binary prints only the
// `error: <message>` line and exits 1, so parity is checked on that line plus
// the exit code rather than on raw stderr.

use std::io::Write;
use std::process::ExitCode;

use early_runtime_data::{
    build_early_runtime_data, default_catalog_path, export_early_runtime_data, nodepath, self_test,
    system_error, verify_early_runtime_data, write_build, Error, EARLY_RUNTIME_SOURCE_BYTES,
};

const USAGE: &str = "usage: early_runtime_data.ts export ROM --directory DIR | build INDEX --directory DIR | verify ROM INDEX | --self-test";

/// Bun prints these two throw shapes without its `error: ` prefix.
fn is_bare_throw(message: &str) -> bool {
    message.starts_with("SyntaxError: ")
        || [
            "ENOENT",
            "EACCES",
            "ENOTDIR",
            "EISDIR",
            "ELOOP",
            "ENOTEMPTY",
            "EIO",
        ]
        .iter()
        .any(|code| message.starts_with(&format!("{code}: ")))
}

fn read_rom(path: &str) -> Result<Vec<u8>, Error> {
    std::fs::read(path).map_err(|error| system_error("open", path, &error))
}

fn run(args: &[String]) -> Result<String, Error> {
    let catalog = default_catalog_path();
    if let [command, source, address] = args {
        if command == "build-region-stdout" {
            let digits = address.strip_prefix("0x").unwrap_or(address);
            let address = i64::from_str_radix(digits, 16)
                .map_err(|_| Error(format!("invalid address: {address}")))?;
            let built = build_early_runtime_data(source, &catalog)?;
            let region = built
                .regions
                .iter()
                .find(|(start, _)| *start == address)
                .ok_or_else(|| {
                    Error("early-runtime asset address is not a produced region".into())
                })?;
            eprintln!(
                "{{\"source_bytes\":{},\"region_address\":\"0x{address:08x}\"}}",
                built.source_bytes
            );
            std::io::stdout()
                .write_all(&region.1)
                .map_err(|error| Error(error.to_string()))?;
            return Ok(String::new());
        }
    }
    if args.len() == 1 && args[0] == "--self-test" {
        return self_test();
    }
    if args.len() == 4 && args[0] == "export" && args[2] == "--directory" {
        let rom_path = nodepath::resolve(&[&args[1]]);
        let rom = read_rom(&rom_path)?;
        export_early_runtime_data(&rom, &args[3], Some(&rom_path), &catalog)?;
        return Ok(format!("source_bytes={EARLY_RUNTIME_SOURCE_BYTES}"));
    }
    if args.len() == 3 && args[0] == "verify" {
        let rom = read_rom(&args[1])?;
        verify_early_runtime_data(&rom, &args[2], &catalog)?;
        return Ok(format!(
            "identical=true source_bytes={EARLY_RUNTIME_SOURCE_BYTES}"
        ));
    }
    if args.len() == 4 && args[0] == "build" && args[2] == "--directory" {
        write_build(&args[1], &args[3], &catalog)?;
        // `writeBuild` prints the constant, not the built total: they are equal
        // whenever the build succeeds, and the original prints the constant.
        return Ok(format!("source_bytes={EARLY_RUNTIME_SOURCE_BYTES}"));
    }
    Err(Error(USAGE.to_string()))
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(line) => {
            if !line.is_empty() {
                println!("{line}");
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            // PORT NOTE: Bun prefixes an uncaught `Error` with `error: `, but a
            // Node `SystemError` (ENOENT and friends, thrown by `readFileSync`)
            // and a `SyntaxError` from `JSON.parse` are not `Error` instances it
            // constructed, so Bun prints those two WITHOUT the prefix. Match that
            // shape exactly, or the diagnostic a caller greps for differs.
            if is_bare_throw(&error.0) {
                eprintln!("{error}");
            } else {
                eprintln!("error: {error}");
            }
            ExitCode::FAILURE
        }
    }
}
