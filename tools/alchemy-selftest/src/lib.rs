//! Native compiler self-test over the routing, plan, bundle, symbol, and lint
//! crates.
//!
//! The entry point is [`selftest::self_test`]. It returns the summary counters
//! the command prints, plus the [`trace::Trace`] of every primitive call made
//! along the way for diagnostic harnesses.

pub mod jsstring;
pub mod lint;
pub mod selftest;
pub mod trace;

pub use selftest::{self_test, Summary};
