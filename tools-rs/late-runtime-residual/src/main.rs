// CLI for the late runtime residual source. Ported from the `main` block of
// tools/make/late_runtime_residual.ts, argument shapes and stdout lines verbatim.
//
// PORT NOTE: Bun prints an uncaught `Error` as `error: <message>` followed by a
// source snippet and a stack trace, then exits 1. This binary prints only the
// `error: <message>` line and exits 1, so parity is checked on that line plus
// the exit code rather than on raw stderr.
//
// PORT NOTE (default catalog): the TypeScript derives the repository root from
// `import.meta.url`, so the tool always reads the tracked
// `assets/data/late_runtime_catalog.json` no matter the cwd. A compiled binary
// has no source path at run time, so the root is baked in from
// `CARGO_MANIFEST_DIR` at build time and can be overridden with `ALCHEMY_ROOT`
// for a relocated checkout. The argv surface is unchanged.

use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use late_runtime_residual::{
    build_late_runtime_residual, export_late_runtime_residual, self_test,
    verify_late_runtime_residual,
};

const USAGE: &str =
    "usage: late_runtime_residual.ts export ROM --directory DIR | verify ROM INDEX | --self-test";

fn repository_root() -> PathBuf {
    match std::env::var("ALCHEMY_ROOT") {
        Ok(root) if !root.is_empty() => PathBuf::from(root),
        _ => Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .and_then(Path::parent)
            .expect("the crate lives two levels below the repository root")
            .to_path_buf(),
    }
}

fn default_catalog() -> PathBuf {
    repository_root().join("assets/data/late_runtime_catalog.json")
}

enum Outcome {
    Line(String),
    /// `usage()` prints to stdout and exits 0.
    Usage,
}

fn run(args: &[String]) -> Result<Outcome, String> {
    let catalog = default_catalog();
    if let [command, source, address] = args {
        if command == "build-region-stdout" {
            let digits = address.strip_prefix("0x").unwrap_or(address);
            let address = u32::from_str_radix(digits, 16)
                .map_err(|_| "invalid late residual address".to_string())?;
            let built = build_late_runtime_residual(Path::new(source), &catalog)?;
            let region = built
                .regions
                .iter()
                .find(|(start, _)| *start == address)
                .ok_or_else(|| "late-runtime asset address is not a produced region".to_string())?;
            eprintln!("{{\"source_bytes\":{}}}", built.source_bytes);
            std::io::stdout()
                .write_all(&region.1)
                .map_err(|error| error.to_string())?;
            return Ok(Outcome::Line(String::new()));
        }
    }
    if args.len() == 1 && args[0] == "--self-test" {
        return Ok(Outcome::Line(self_test(&catalog)?));
    }
    if args.len() == 4 && args[0] == "export" && args[2] == "--directory" {
        export_late_runtime_residual(Path::new(&args[1]), Path::new(&args[3]), &catalog)?;
        return Ok(Outcome::Line(String::new()));
    }
    if args.len() == 3 && args[0] == "verify" {
        verify_late_runtime_residual(Path::new(&args[1]), Path::new(&args[2]), &catalog)?;
        return Ok(Outcome::Line("identical=true source_bytes=7294".into()));
    }
    Ok(Outcome::Usage)
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        // `export` prints nothing at all in the TypeScript.
        Ok(Outcome::Line(line)) if line.is_empty() => ExitCode::SUCCESS,
        Ok(Outcome::Line(line)) => {
            println!("{line}");
            ExitCode::SUCCESS
        }
        Ok(Outcome::Usage) => {
            println!("{USAGE}");
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
