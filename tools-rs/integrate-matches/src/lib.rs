//! `tools/lib/integrate_matches.ts`, ported.
//!
//! The installation gate. Given a directory of m2c drafts named `src_<addr>.c`,
//! it compiles and links each one at its real ROM address, links the
//! hand-written `asm/<addr>.s` the same way, and accepts the draft only when
//! the two byte streams are identical. With `--apply` it then moves the draft
//! into `exact/`, deletes the assembly, retires the address's scratch files and
//! closes its dossier.
//!
//! The port is split by concern:
//!
//! * [`jsregex`] holds the six regular expressions, written by hand, with the
//!   missing `i` flags reproduced rather than repaired.
//! * [`jsint`] holds THE ONE numeric parse. `parseInt(s, 16)` accepts a `0x`
//!   prefix and trailing junk; `from_str_radix` rejects both, and routing every
//!   call site through one helper is the fix for a defect that has already cost
//!   this project 1,506 bytes across two call sites in one crate.
//! * [`extent`] is `mismatch` and `linkedFunctionExtent`.
//! * [`cleanup`] is `cleanupInstalledScratch`, the only function here that
//!   deletes.
//! * [`link`] is `run`, `commandError` and `linkedBytes`.
//! * [`cli`] is `parseArguments`; [`pipeline`] is `main`; [`selftest`] is
//!   `--self-test`.
//!
//! NOTHING IS CACHED. `integrate_matches.ts` has no cache of any kind: it
//! spawns the toolchain on every candidate, every run. There is therefore no
//! cache key in this crate to get wrong, and none was invented for the port.
//! `tools-rs/cache-key-lint` is still run against it, and reports nothing.

pub mod cleanup;
pub mod cli;
pub mod extent;
pub mod jsint;
pub mod jsregex;
pub mod link;
pub mod pipeline;
pub mod selftest;

pub use cleanup::{cleanup_installed_scratch, Cleanup};
pub use cli::{parse_arguments, Options, ParseOutcome, USAGE};
/// `linkedFunctionExtent`, exported for every other port that imports it from
/// the TypeScript: `candidate_show.ts`, `mode_sweep.ts` and
/// `compiler_corpus_regression.ts` all do. In particular
/// `tools-rs/candidate-show/src/extent.rs` carries an inlined copy with a note
/// to delete it once this crate exists; it exists, and the copy can go.
pub use extent::{linked_function_extent, mismatch, Mismatch};
pub use link::{linked_bytes, Kind};
pub use pipeline::{run_pipeline, Report, ROOT_OVERRIDE};
