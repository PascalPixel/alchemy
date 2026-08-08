//! `tools/lib/candidate_show.ts`, ported.
//!
//! Compile one main-image candidate and print its disassembly beside the
//! reference's, aligned by offset, marking every halfword that differs.
//!
//! The port is split by concern:
//!
//! * [`jsparse`] holds the JavaScript number/string semantics this file needs
//!   that Rust does not share: `Number.parseInt(s, 16)`'s prefix tolerance,
//!   `padEnd`/`padStart`/`slice` measured in UTF-16 code units, and the
//!   trailing-`\s` trim.
//! * [`manifest`] is `regionSize`.
//! * [`extent`] is `linkedFunctionExtent`, inlined from
//!   `tools/lib/integrate_matches.ts` (see the module note).
//! * [`disasm`] is `disassemble`, the `objdump` spawn and its line grammar.
//! * [`diff`] is `differingOffsets` and `selfTest`.
//! * [`cli`] is `optionsOf`.
//! * [`render`] is `main()`.
//!
//! Nothing here imports `alchemy_gcc.ts`'s Rust slices by name: everything the
//! TypeScript reaches from there arrives through `verifyCandidate`.

pub mod cli;
pub mod diff;
pub mod disasm;
pub mod extent;
pub mod jsparse;
pub mod manifest;
pub mod render;

pub use cli::{options_of, Options, ParseOutcome, USAGE};
pub use diff::{differing_offsets, self_test};
pub use disasm::disassemble;
pub use extent::linked_function_extent;
pub use manifest::region_size;
pub use render::{render, RenderOutput};
