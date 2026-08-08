//! A Rust port of `tools/lib/permute_m2c.ts`.
//!
//! The tool takes the near-miss rows of the m2c match report, applies five
//! mechanical source rewrites to each draft (plus a leading-argument threading
//! pass, applied on its own and on top of each of the five), and keeps the first
//! variant whose linked `.text` is byte-identical to the ROM. A survivor is
//! written to `exact/` and its hand-written `asm/` source is removed.
//!
//! Module map:
//!
//! * [`jsstring`] and [`jsnum`] -- the ECMAScript primitives the port needs:
//!   whitespace and word classes, UTF-16 ordering, `Number`, `ToInt32`,
//!   `padStart`, and the subtraction comparator.
//! * [`patterns`] -- the six regular expressions, hand-compiled. The `regex`
//!   crate is not used: its character classes are Unicode where JavaScript's
//!   are ASCII, and its `.` matches characters JavaScript's does not.
//! * [`transforms`] -- the pure rewrites and the eleven-variant fan-out.
//! * [`json`] -- an insertion-ordered reader for the report and the
//!   classification file.
//! * [`assembly`] -- the two functions that touch `asm/`.
//! * [`paths`] -- the environment overrides that keep this port out of the
//!   repository during a parity run.
//! * [`pipeline`] -- `main()`.
//!
//! There is deliberately **no cache** and **no seeded RNG** in this tool; the
//! TypeScript has neither, and the eleven variants are a fixed fan-out rather
//! than a sampled one. Nothing here is memoized that the TypeScript does not
//! memoize.

pub mod assembly;
pub mod cli;
pub mod json;
pub mod jsnum;
pub mod jsstring;
pub mod paths;
pub mod patterns;
pub mod pipeline;
pub mod test_support;
pub mod transforms;

pub use cli::{parse_arguments, Options, ParseOutcome, USAGE};
pub use assembly::{replaceable_assembly, retained_assembly_stems};
pub use pipeline::{near_misses, report_rows, run, NearMiss, Outcome, UNKNOWN_TYPES};
pub use transforms::{
    candidates, lift_constants, post_increment_walk, state_block, thread_leading_argument,
    volatile_hardware,
};
