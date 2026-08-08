//! `tools/lib/overlay_disasm.ts`, in Rust.
//!
//! WHY THIS CRATE EXISTS. This file sits at the centre of the overlay work:
//! twenty TypeScript tools import `assembleOverlay` from it, and every one of
//! them inherited the old compiler-module import. That import is why seven
//! earlier attempts to port this module ran out of context before writing a
//! line. The compiler surface has since been taken apart into native crates:
//!
//! * `alchemy-symbols` supplies `CALL_VIA_BASE`, `external_symbol`,
//!   `external_symbol_assembly` and `overlay_call_via_base`.
//! * `alchemy-plan` supplies `source_to_assembly_plan` and `CompilerCommandStep`.
//! * `alchemy-bundle` supplies `compiler_bundle_signature`, `CompilerTarget`
//!   and the hand-written FIPS 180-4 `sha256`.
//! * `alchemy-routing` supplies nothing this module names directly; it is
//!   reached transitively through `alchemy-plan` and `alchemy-symbols`.
//! * `alchemy-lints` supplies nothing this module names; the three lints are
//!   applied by the compiler lint entrypoint, not by `overlay_disasm.ts`.
//!
//! Two further crates cover the non-compiler imports: `discover` for
//! `Discovery` and `cache-entry` for `write_cache_entry_atomically`.
//!
//! `ROOT` is computed here from `CARGO_MANIFEST_DIR` (see [`paths::root`]).
//!
//! THE CACHE KEY IS LOAD-BEARING. `tools/cache-key-lint` cites this module
//! by name as the reference implementation: a key derives from its inputs plus
//! a digest of the tool's own source, never from a hand-maintained `-vN`
//! literal. See [`compile::self_digest`].
//!
//! ORDER IS BEHAVIOUR. There is no `HashMap` and no `HashSet` in this crate.
//! The TypeScript's `Map`/`Set` iteration is insertion order, and three places
//! here observe it: the external-symbol stub list (which must not dedupe), the
//! `votes` tally in `call_via_bank_base` (first-past-the-post on ties), and the
//! `externals` table in `build_overlay_source`.

pub mod compile;
pub mod disasm;
pub mod paths;
pub mod regex;
pub mod selftest;

pub use compile::{
    assemble_overlay, bias_in_image_label_words, compile_overlay_c, compile_overlay_candidate,
    overlay_c_addresses, overlay_c_spans, overlay_c_sources, self_digest, BiasResult, Compiled,
    Span, OVERLAY_LINK_BIAS,
};
pub use disasm::{build_overlay_source, call_via_bank_base, OVERLAY_BASE, ROM_BASE};
pub use paths::OverlaySource;
