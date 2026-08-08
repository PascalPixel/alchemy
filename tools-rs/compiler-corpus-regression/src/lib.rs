//! Port of `tools/compiler/compiler_corpus_regression.ts`.
//!
//! The tool recompiles a deterministic slice of the exact-C corpus under a
//! proposed compiler configuration and reports every byte that changes against
//! the ROM. It is the instrument for the pending gcc-2.96 codegen-identity
//! check, so fidelity to the TypeScript outranks speed everywhere in this
//! crate. Where the two would disagree, the TypeScript wins and the divergence
//! is written down as a `PORT NOTE`.
//!
//! TWO DEFECTS ARE CORRECTED HERE, both documented at their site and both
//! confirmed by running the TypeScript:
//!
//! 1. `corpus()` filters `region.source.startsWith("src/")` while every region
//!    in `out/full/claimed/manifest.json` is under `exact/`, and there is no
//!    `src/` directory in the repository at all. `git log -L 240,245` puts that
//!    literal in the tool's FIRST commit `c7b7f3736`, so it is NOT a
//!    `b3ab4841b` breakage. See `corpus::corpus`.
//! 2. `main()` digests `join(ROOT, "tools", name)` for four files that
//!    `b3ab4841b` moved into `tools/lib/` and `tools/compiler/`. That is the
//!    confirmed `b3ab4841b` breakage and the corrected paths are used here.
//!
//! Because of (1) the TypeScript has never once processed a corpus member, and
//! because of (2) it could not have reached the compile path even if it had.

pub mod cli;
pub mod config;
pub mod corpus;
pub mod diff;
pub mod extent;
pub mod jsparse;
pub mod jsvalue;
pub mod pipeline;
pub mod result;
pub mod selftest;

/// `const FORMAT = 3`.
///
/// PORT NOTE -- this is a hand-maintained literal and it is the one part of the
/// cache key that is NOT derived from an input. It is tolerable only because
/// `compilerSignature` also digests the tool's own source, so a change to the
/// logic invalidates every key whether or not anyone remembers to bump this.
/// Do not add a second `-vN` literal anywhere; see `pipeline::compiler_signature`.
pub const FORMAT: f64 = 3.0;

/// `ROM_BASE` from `tools/lib/match_m2c.ts`.
pub const ROM_BASE: f64 = 0x0800_0000 as f64;
