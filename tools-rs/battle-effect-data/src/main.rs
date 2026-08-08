// CLI entry point, matching `main(args)` in tools/make/battle_effect_data.ts.

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if let Err(message) = battle_effect_data::run(args) {
        // PORT NOTE: Bun prints a source excerpt and a stack trace around this
        // line. The `error: <message>` line itself is identical, and that is
        // what the parity run compares; the excerpt is noise no caller reads.
        eprintln!("error: {message}");
        std::process::exit(1);
    }
}
