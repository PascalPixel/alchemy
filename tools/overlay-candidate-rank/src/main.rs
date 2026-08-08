//! `bun tools/overlay/overlay_candidate_rank.ts` in Rust.

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
    let self_exe = env::current_exe().unwrap_or_else(|_| PathBuf::from("overlay-candidate-rank"));
    match overlay_candidate_rank::run(&root(), &self_exe, &args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("{message}");
            ExitCode::FAILURE
        }
    }
}
