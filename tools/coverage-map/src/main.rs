// Thin entry point: everything lives in the library root.
//
// PORT NOTE: a `throw` out of the TypeScript entry point is reported by Bun as
// `error: <message>` on stderr with exit status 1, followed by a stack trace
// this binary has no equivalent for. The `error:` line and the status are what
// the parity harness compares.

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    match coverage_map::cli::run(&argv) {
        Ok(line) => println!("{line}"),
        Err(message) => {
            eprintln!("error: {message}");
            std::process::exit(1);
        }
    }
}
