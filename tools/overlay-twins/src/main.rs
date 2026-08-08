//! `bun tools/overlay/overlay_twins.ts` in Rust.

use std::env;
use std::path::PathBuf;
use std::process::ExitCode;

fn root() -> PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(std::path::Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf()
}

fn main() -> ExitCode {
    let args: Vec<String> = env::args().skip(1).collect();
    match overlay_twins::run(&root(), &args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
