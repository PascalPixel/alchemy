//! Native candidate compilation and compatibility primitives.
//!
//! The port is split by concern:
//!
//! * [`jsstring`] and [`jsnum`] hold the JavaScript semantics the rest of the
//!   crate leans on. They exist so that no other module has to remember that
//!   `Math.max` propagates NaN or that `\s` excludes U+0085.
//! * [`json`] is the insertion-ordered value type, the strict parser, and the
//!   `canonicalJson` writer. No `HashMap`: key order is the file format.
//! * [`verify`] compiles and links one candidate against its target bytes.

pub mod jsnum;
pub mod json;
pub mod jsstring;
pub mod verify;

pub use alchemy_routing::routing::CompilerTarget;
pub use verify::{
    verify_candidate, CandidateCompilerConfiguration, CandidateCompilerFamily, Verification,
    ROM_BASE,
};
