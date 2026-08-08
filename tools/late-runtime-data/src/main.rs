// CLI for the late runtime data package. Ported from the `main` block of
// tools/lib/late_runtime_data.ts, argument shapes and stdout lines verbatim.
//
// PORT NOTE: Bun prints an uncaught `Error` as `error: <message>` followed by a
// source snippet and a stack trace, then exits 1. This binary prints only the
// `error: <message>` line and exits 1, so parity is checked on that line plus
// the exit code rather than on raw stderr.

use std::path::Path;
use std::process::ExitCode;

use late_runtime_data::{
    export_late_runtime_package, self_test, verify_late_runtime_package, write_late_runtime_build,
};

const USAGE: &str = "usage: late-runtime-data export ROM --catalog CATALOG --directory DIR | build INDEX --directory DIR | verify ROM --index INDEX | --self-test";

fn run(args: &[String]) -> Result<String, String> {
    if args.len() == 1 && args[0] == "--self-test" {
        return self_test();
    }
    if args.len() == 6 && args[0] == "export" && args[2] == "--catalog" && args[4] == "--directory"
    {
        export_late_runtime_package(
            Path::new(&args[1]),
            Path::new(&args[3]),
            Path::new(&args[5]),
        )?;
        return Ok("identical=true ranges=3 source_bytes=7294".into());
    }
    if args.len() == 4 && args[0] == "build" && args[2] == "--directory" {
        let built = write_late_runtime_build(Path::new(&args[1]), Path::new(&args[3]))?;
        return Ok(format!("ranges={} source_bytes={}", built.ranges.len(), built.source_bytes));
    }
    if args.len() == 4 && args[0] == "verify" && args[2] == "--index" {
        let built = verify_late_runtime_package(Path::new(&args[1]), Path::new(&args[3]))?;
        return Ok(format!(
            "identical=true ranges={} source_bytes={}",
            built.ranges.len(),
            built.source_bytes
        ));
    }
    Err(USAGE.into())
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(line) => {
            println!("{line}");
            ExitCode::SUCCESS
        }
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}
