//! Compiler routing: which flags each translation unit is built with.
//!
//! This crate is the native home of the compiler routing tables -- the path
//! constants, the ~137 routing allowlist tables, the source-key
//! helpers, and `cflags_for_source` / `cflags_for_target_source` /
//! `uses_agbcc_compiler`. It was promoted out of `tools/verify/src/`, where
//! it had been inlined; `verify` now depends on it by path and re-exports it, so
//! the paths its callers use are unchanged.
//!
//! The tables are the native source of truth. Their order is load-bearing:
//! flags are appended in the order established by the compiler evidence, and
//! later options win when the compiler sees a conflict.

// `bundle_data` (the approved-digest tables) moved to `tools/alchemy-bundle/`
// on 2026-08-07. It never belonged here: the tables are bundle-admission data,
// not routing data, and they carried an explicit "hand-synced with NO gate"
// note because this crate's flat string-sequence drift check cannot read a
// three-level nested object literal with bare-identifier keys. The bundle crate
// has an object-literal parser and now gates them. Import from
// `alchemy_bundle::bundle_data`; do not reintroduce a copy here.

pub mod routing;
pub mod routing_data;
