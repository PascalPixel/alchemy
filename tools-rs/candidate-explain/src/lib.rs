//! Rust port of `tools/lib/candidate_explain.ts`.
//!
//! Compile a candidate with gcc-2.96's own `-fsched-verbose=9` scheduler trace
//! enabled, correlate the final RTL insn order with the actual disassembled
//! instructions by CONTENT (instruction class + destination register, via
//! `rtl-align`), and print the compiler's own ready-list trace beside each
//! differing row.
//!
//! Every module this file exposes has a one-to-one counterpart in the
//! TypeScript. Nothing is simplified, merged or repaired along the way: the
//! defects listed in `PORT NOTE` comments are reproduced deliberately so the
//! differential harness in `src/bin/candidate_explain_parity.rs` can hold both
//! implementations to the same output.

pub mod cli;
pub mod explain;
pub mod jsnum;
pub mod matchers;
pub mod ordered;
pub mod run;
pub mod selftest;

pub use cli::{parse_arguments, Options, ParseOutcome, USAGE};
pub use explain::{
    decision_for_insn, describe_insn, differing_halfwords, disassemble_instructions,
    find_dump_file, format_pair, report, trace_for_insn, Decision, EXPLAIN_FLAGS,
};
pub use run::{run_main_image, run_overlay};
pub use selftest::self_test;
