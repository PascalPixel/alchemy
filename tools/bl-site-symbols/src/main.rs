use std::process::ExitCode;

fn main() -> ExitCode {
    match bl_site_symbols::run(&std::env::args().skip(1).collect::<Vec<_>>()) {
        Ok(output) => {
            print!("{output}");
            if !output.ends_with('\n') {
                println!();
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}
