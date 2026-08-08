//! Gate binary: fails when `alchemy-symbols` and `tools/lib/alchemy_gcc.ts`
//! disagree about the `call_via` tables or the symbol regexes.
//!
//! RETIREMENT: delete this file, `src/drift.rs` and the `[[bin]]` entry in
//! `Cargo.toml` when `tools/lib/alchemy_gcc.ts` is deleted.

fn main() {
    match alchemy_symbols::drift::check() {
        Ok(report) => {
            println!(
                "alchemy-symbols: {} subjects compared, all in sync with {}",
                report.compared(),
                alchemy_symbols::drift::TYPESCRIPT_SOURCE
            );
        }
        Err(message) => {
            eprintln!("{message}");
            std::process::exit(1);
        }
    }
}
