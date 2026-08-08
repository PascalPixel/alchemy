// The dashboard binary: live server by default, self-test on request.

fn main() {
    let self_test = std::env::args().any(|argument| argument == "--self-test");
    if self_test {
        match dashboard_server::selftest::self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => {
                // Keep self-test failures concise and machine-readable.
                eprintln!("error: {message}");
                std::process::exit(1);
            }
        }
        return;
    }
    if let Err(error) = dashboard_server::server::run() {
        eprintln!("error: {error}");
        std::process::exit(1);
    }
}
