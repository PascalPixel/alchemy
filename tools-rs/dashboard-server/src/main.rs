// `if (import.meta.main) { --self-test | main() }`.

fn main() {
    // Not a route of the oracle: a filter used only by the parity harness, so
    // the normaliser it depends on is the SAME code the unit tests pin.
    if std::env::args().any(|argument| argument == "--normalise") {
        let mut input = String::new();
        if std::io::Read::read_to_string(&mut std::io::stdin(), &mut input).is_err() {
            // Binary bodies (the font) are compared by digest, never piped here.
            std::process::exit(2);
        }
        let root = std::env::args()
            .skip_while(|argument| argument != "--root")
            .nth(1)
            .unwrap_or_default();
        print!("{}", dashboard_server::parity::normalise(&input, &root));
        return;
    }
    let self_test = std::env::args().any(|argument| argument == "--self-test");
    if self_test {
        match dashboard_server::selftest::self_test() {
            Ok(line) => println!("{line}"),
            Err(message) => {
                // PORT NOTE: Bun prints `error: <message>` for a thrown Error
                // and exits 1. Native failures (ENOENT, SyntaxError) print NO
                // `error: ` prefix. Everything reachable here is a thrown
                // Error in the oracle, so the prefix is correct.
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
