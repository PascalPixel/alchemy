use std::io::Write;

fn main() {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
    match audit_residuals::run(&arguments) {
        Ok(output) => {
            if !output.is_empty() {
                println!("{output}");
            }
        }
        Err(message) => {
            let _ = writeln!(std::io::stderr(), "error: {message}");
            std::process::exit(1);
        }
    }
}
