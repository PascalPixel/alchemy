//! Compiler routing: which flags each translation unit is built with.
//!
//! This crate is the Rust home of the routing half of `tools/lib/alchemy_gcc.ts`
//! -- the path constants, the ~137 routing allowlist tables, the source-key
//! helpers, and `cflags_for_source` / `cflags_for_target_source` /
//! `uses_agbcc_compiler`. It was promoted out of `tools-rs/verify/src/`, where
//! it had been inlined; `verify` now depends on it by path and re-exports it, so
//! the paths its callers use are unchanged.
//!
//! The tables are duplicated between this crate and `alchemy_gcc.ts` while both
//! exist. That duplication is enforced, not trusted: see `drift`, which parses
//! the tables back out of the TypeScript and compares them element-wise. Run it
//! with `cargo run --release --bin routing-drift`, or as `cargo test`.
//!
//! When `alchemy_gcc.ts` is deleted, delete `drift` and its binary with it --
//! there is nothing left to compare against, and a comparison with no other side
//! is the defect it exists to police.

// `bundle_data` (the approved-digest tables) moved to `tools-rs/alchemy-bundle/`
// on 2026-08-07. It never belonged here: the tables are bundle-admission data,
// not routing data, and they carried an explicit "hand-synced with NO gate"
// note because this crate's flat string-sequence drift check cannot read a
// three-level nested object literal with bare-identifier keys. The bundle crate
// has an object-literal parser and now gates them. Import from
// `alchemy_bundle::bundle_data`; do not reintroduce a copy here.

pub mod drift;
pub mod routing;
pub mod routing_data;
