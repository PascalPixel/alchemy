//! `tools/lib/match_m2c.ts`, ported.
//!
//! Compile every m2c draft against the ROM bytes it claims to reproduce, try
//! the eight `M2C_UNK` substitutions when the draft has an unknown type, keep
//! the best-scoring attempt, and write a canonical report.
//!
//! The port is split by concern:
//!
//! * [`jsstring`] and [`jsnum`] hold the JavaScript semantics the rest of the
//!   crate leans on. They exist so that no other module has to remember that
//!   `Math.max` propagates NaN or that `\s` excludes U+0085.
//! * [`json`] is the insertion-ordered value type, the strict parser, and the
//!   `canonicalJson` writer. No `HashMap`: key order is the file format.
//! * [`verify`] is `verifyCandidate`, the half that spawns the toolchain.
//! * [`cli`] is `parseArguments`.
//! * [`pipeline`] is `main()`.

pub mod cli;
pub mod json;
pub mod jsnum;
pub mod jsstring;
pub mod pipeline;
pub mod verify;

pub use cli::{parse_arguments, Options, ParseOutcome, USAGE};
pub use pipeline::{run_pipeline, DraftOutcome, REJECT, UNKNOWN_TYPE_STRATEGIES};
pub use verify::{
    verify_candidate, CandidateCompilerConfiguration, CandidateCompilerFamily, Verification,
    M2C_PREAMBLE, ROM_BASE,
};
