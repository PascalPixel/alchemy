//! Fail the build when `tools/lib/alchemy_gcc.ts` and this crate disagree.
//!
//! WHY. The plan layer's literal sequences, family union and record shapes live
//! in both languages with no generator on either side. This binary is what makes
//! that survivable: it exits non-zero on any element-wise difference, on a
//! declaration that can no longer be found, on comparing fewer subjects than
//! exist, and on an extractor that starts matching nothing.
//!
//! RETIREMENT: delete with `src/drift.rs` when the TypeScript goes.

fn main() {
    match alchemy_plan::drift::check() {
        Ok(report) => {
            println!(
                "plan parity: {} subjects in sync ({} elements compared, {} distinct)",
                report.compared(),
                report.elements,
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
