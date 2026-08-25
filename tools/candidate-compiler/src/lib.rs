//! Native candidate compilation and compatibility primitives.

pub mod jsnum;
pub mod verify;

pub use compiler_core::routing::CompilerTarget;
pub use verify::{
    compile_to_assembly, verify_candidate_owned_routed, CandidateCompilerConfiguration,
    CandidateCompilerFamily, Verification, ROM_BASE,
};
