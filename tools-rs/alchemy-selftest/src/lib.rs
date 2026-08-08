//! `selfTest()` from `tools/lib/alchemy_gcc.ts`, ported to Rust.
//!
//! The entry point is [`selftest::self_test`]. It returns the summary counters
//! the TypeScript prints, plus the [`trace::Trace`] of every primitive call it
//! made along the way, which is what the differential harness compares against
//! the live TypeScript.

pub mod jsstring;
pub mod lint;
pub mod selftest;
pub mod trace;

pub use selftest::{self_test, Summary};
