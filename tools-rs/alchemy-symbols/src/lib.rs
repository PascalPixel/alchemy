//! External symbol resolution: the names a reconstructed translation unit may
//! reference without defining, and the assembly stub that gives each one an
//! address.
//!
//! WHY THIS CRATE EXISTS. `tools/lib/alchemy_gcc.ts` is 4,404 lines and is the
//! single blocker for porting the Bun tooling to Rust: a tool that needs one
//! function out of it has to drag a `bun` sidecar along, and every port that
//! did came back at parity or slower. The file is being taken apart a coherent
//! layer at a time. `alchemy-routing` took the routing tables; this crate takes
//! the symbol layer, which is the natural next slice because it is pure: no
//! filesystem, no subprocess, no hashing, no clock. Its whole input is a symbol
//! name and a `call_via` bank address.
//!
//! Ported here, from `alchemy_gcc.ts`:
//!
//! * `CALL_VIA_BASE`, `ADDRESS_SYMBOL`, `CALL_VIA_SYMBOL`, `CALL_VIA_ALIAS`,
//!   `CALL_VIA_REGISTERS`
//! * `ExternalSymbol`, `externalSymbol`, `externalSymbolAssembly`
//! * `OVERLAY_CALL_VIA_BASE`, `SOURCE_CALL_VIA_BASE`, `overlayCallViaBase`
//!
//! The three regexes are matched by hand rather than with a regex engine; see
//! `symbols` for the PORT NOTEs on the traps that hides (JS `[0-9]` is
//! ASCII-only, the alternation `1[0-3]|[0-9]` cannot produce `r00`, and JS `$`
//! without the `m` flag still permits nothing after it because these patterns
//! are `^...$` on a whole name).
//!
//! DUPLICATION. Both tables and both address constants exist twice while the
//! TypeScript is live. Nothing generates either side, so the parity is measured
//! rather than promised: `drift` parses the declarations back out of
//! `alchemy_gcc.ts` and compares them element-wise and in order, and fails if
//! either mirror is unreadable or if fewer tables were compared than exist.
//! Run it with `cargo run --bin symbols-drift`, or as `cargo test`.
//!
//! RETIREMENT. When `tools/lib/alchemy_gcc.ts` is finally deleted, delete
//! `src/drift.rs`, `src/bin/symbols_drift.rs` and the `[[bin]]` entry in
//! `Cargo.toml`. Keep `symbols.rs` and `call_via_data.rs`: those are the
//! surviving implementation. A drift check with only one mirror left is exactly
//! the check-that-cannot-fail it was written to prevent.
//!
//! Also at that point, delete the duplicate `external_symbol` /
//! `external_symbol_assembly` pair in `tools-rs/verify/src/lib.rs` (see
//! `symbols` PORT NOTE about its missing `callViaBase` parameter) and have
//! `verify` depend on this crate instead.

pub mod call_via_data;
pub mod drift;
pub mod symbols;

pub use symbols::{
    external_symbol, external_symbol_assembly, overlay_call_via_base, ExternalSymbol,
    CALL_VIA_BASE,
};
