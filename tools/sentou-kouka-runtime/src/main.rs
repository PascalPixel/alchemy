use std::process::ExitCode;

fn main() -> ExitCode {
    match sentou_kouka_runtime::run(std::env::args().skip(1).collect()) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
