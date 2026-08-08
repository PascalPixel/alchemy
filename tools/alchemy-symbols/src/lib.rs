//! External symbol resolution: the names a reconstructed translation unit may
//! reference without defining, and the assembly stub that gives each one an
//! address.
//!
//! WHY THIS CRATE EXISTS. The former compiler module was the single blocker for
//! porting the Bun tooling to Rust: a tool that needed one function out of it
//! had to drag a `bun` sidecar along, and every port that did came back at
//! parity or slower. The module was taken apart a coherent layer at a time.
//! `alchemy-routing` took the routing tables; this crate takes the symbol layer,
//! which is the natural next slice because it is pure: no
//! filesystem, no subprocess, no hashing, no clock. Its whole input is a symbol
//! name and a `call_via` bank address.
//!
//! The native surface contains:
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
pub mod call_via_data;
pub mod symbols;

pub use symbols::{
    external_symbol, external_symbol_assembly, overlay_call_via_base, ExternalSymbol, CALL_VIA_BASE,
};
