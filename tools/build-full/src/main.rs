//! Thin shim. The CLI lives in `cli::run` so this command can be linked into a
//! shared entry point; this binary stays only until a consolidated `build`
//! executable exists.
fn main() -> std::process::ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    match build_full::cli::run(&arguments) {
        0 => std::process::ExitCode::SUCCESS,
        _ => std::process::ExitCode::FAILURE,
    }
}
