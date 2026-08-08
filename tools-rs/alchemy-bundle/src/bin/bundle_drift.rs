//! Fail the build when `tools/lib/alchemy_gcc.ts` and this crate disagree.
//!
//! WHY. The digest tables and the signature path list live in both languages
//! with no generator on either side. This binary is the thing that makes that
//! survivable: it exits non-zero on any element-wise difference, on a missing
//! declaration, and on comparing fewer subjects than exist.
//!
//! RETIREMENT: delete with `src/drift.rs` when the TypeScript goes.

fn main() {
    match alchemy_bundle::drift::check() {
        Ok(report) => {
            println!(
                "bundle parity: {} subjects in sync ({} distinct elements compared)",
                report.compared(),
                report.distinct_elements
            );
            for name in &report.agreed {
                println!("  ok  {name}");
            }
        }
        Err(message) => {
            eprintln!("{message}");
            std::process::exit(1);
        }
    }
}
