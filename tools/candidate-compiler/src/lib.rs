//! Native candidate compilation and compatibility primitives.
//!
//! The port is split by concern:
//!
//! * [`jsstring`] and [`jsnum`] hold the JavaScript semantics the rest of the
//!   crate leans on. They exist so that no other module has to remember that
//!   `Math.max` propagates NaN or that `\s` excludes U+0085.
//! * [`verify`] compiles and links one candidate against its target bytes.

pub mod jsnum;
pub mod jsstring;
pub mod verify;

pub use compiler_core::routing::CompilerTarget;
pub use verify::{compile_to_assembly, verify_candidate, verify_candidate_owned_routed, verify_candidate_routed, CandidateCompilerConfiguration, CandidateCompilerFamily, Verification, ROM_BASE};
