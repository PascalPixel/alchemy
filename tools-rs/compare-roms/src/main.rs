// CLI shell for the ROM comparison report. All logic lives in the library so
// it can be exercised by tests; this file only moves bytes and sets exit codes.
//
// PORT NOTE: on failure the TypeScript entry point lets the error escape into
// Bun, which prints `error: <message>` followed by a JavaScript stack trace and
// exits 1. The first stderr line and the exit code are reproduced exactly; the
// stack-trace lines are not, since they name TypeScript frames that do not
// exist here.
//
// PORT NOTE: the one message text that differs is an unreadable input file.
// Bun raises `ENOENT: no such file or directory, open '<path>'` from
// `Bun.file().arrayBuffer()`; this prints `error: <path>: <std::io::Error>`.
// Exit code and the fact of failure are identical; only the wording differs,
// and no caller parses it.

use std::io::Write;

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match compare_roms::run(&args) {
        Ok(compare_roms::Run::Printed(text)) => println!("{text}"),
        Ok(compare_roms::Run::Wrote { path, contents, line }) => {
            // PORT NOTE: `Bun.write` creates missing parent directories.
            if let Some(parent) = std::path::Path::new(&path).parent() {
                if !parent.as_os_str().is_empty() {
                    if let Err(error) = std::fs::create_dir_all(parent) {
                        fail(&format!("{path}: {error}"));
                    }
                }
            }
            if let Err(error) = std::fs::write(&path, contents) {
                fail(&format!("{path}: {error}"));
            }
            println!("{line}");
        }
        Err(message) => fail(&message),
    }
}

fn fail(message: &str) -> ! {
    let _ = writeln!(std::io::stderr(), "error: {message}");
    std::process::exit(1);
}
