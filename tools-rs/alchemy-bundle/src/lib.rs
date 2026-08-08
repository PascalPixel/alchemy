//! The compiler-bundle layer of `tools/lib/alchemy_gcc.ts`, in Rust.
//!
//! WHY THIS CRATE EXISTS. `tools/lib/alchemy_gcc.ts` is 4,404 lines and has
//! defeated repeated attempts to port it whole. It is being taken apart one
//! coherent layer at a time: `alchemy-routing` (the routing tables) and
//! `alchemy-symbols` (symbol extraction) landed first, this crate is the third.
//! It owns everything between "which compiler bundle is on disk" and "what
//! command line invokes it": the approved-digest tables, host admission,
//! bundle validation, `compilerBundleSignature`, and the compiler-command
//! builders. It stops cleanly before `alchemy-plan` (TypeScript line 3594).
//!
//! Ported from `tools/lib/alchemy_gcc.ts` lines 5-57 and 2987-3592.
//!
//! TWO STANDING HAZARDS, both encoded rather than commented:
//!
//! 1. ORDER IS BEHAVIOUR. gcc is later-flag-wins, digest arrays are searched
//!    with `includes`, and executables are stat'd in declaration order. There
//!    is deliberately no `HashMap` or `HashSet` anywhere in this crate; every
//!    table is an ordered slice. A hash container here would compile, pass a
//!    casual read, and silently produce a different compiler invocation.
//!
//! 2. `compilerBundleSignature` OMITS `GCC3_DRIVER`. That is a real bug,
//!    reproduced exactly as written, because the signature is cache-key
//!    material in six call sites and changing its input set busts every cache
//!    in the repository at once. See the PORT NOTE on
//!    [`bundle::signature_paths`]; the omission is pinned by a unit test and
//!    gated by [`drift`] so a one-sided "fix" fails loudly.
//!
//! RETIREMENT. When `tools/lib/alchemy_gcc.ts` is finally deleted, delete
//! [`drift`], [`jsvalue`], `src/bin/bundle_drift.rs`, `src/bin/bundle_corpus.rs`
//! and both `[[bin]]` entries in `Cargo.toml`. Those exist only to hold two
//! live mirrors together; with one mirror left a drift check cannot fail, which
//! is precisely the failure mode it was written to prevent. [`bundle`],
//! [`bundle_data`] and [`sha256`] stay.

pub mod bundle;
pub mod bundle_data;
pub mod drift;
pub mod jsvalue;
pub mod sha256;
