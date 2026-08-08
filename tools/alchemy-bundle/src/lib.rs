//! Native compiler-bundle layer.
//!
//! WHY THIS CRATE EXISTS. The former compiler module defeated repeated attempts
//! to port it whole. It was taken apart one coherent layer at a time:
//! `alchemy-routing` (the routing tables) and
//! `alchemy-symbols` (symbol extraction) landed first, this crate is the third.
//! It owns everything between "which compiler bundle is on disk" and "what
//! command line invokes it": the approved-digest tables, host admission,
//! bundle validation, `compilerBundleSignature`, and the compiler-command
//! builders. It stops cleanly before `alchemy-plan` (TypeScript line 3594).
//!
//! This crate owns the former bundle section and its approved digests.
//!
//! TWO STANDING HAZARDS, both encoded rather than commented:
//!
//! 1. ORDER IS BEHAVIOUR. gcc is later-flag-wins, digest arrays are searched
//!    with `includes`, and executables are stat'd in declaration order. There
//!    is deliberately no `HashMap` or `HashSet` anywhere in this crate; every
//!    table is an ordered slice. A hash container here would compile, pass a
//!    casual read, and silently produce a different compiler invocation.
//!
//! 2. `compilerBundleSignature` omits `GCC3_DRIVER`, a cache-key behavior
//!    preserved from the admitted compiler bundle. Its omission is pinned by a
//!    unit test because changing it invalidates every repository cache.

pub mod bundle;
pub mod bundle_data;
pub mod sha256;
