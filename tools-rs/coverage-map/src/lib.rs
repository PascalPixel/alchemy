// ROM coverage map: a treemap of the audited GS1-EN image, drawn as a dark
// product-dashboard card for the README.
//
// The map answers one question visually: of every byte in the English ROM,
// which bytes are already expressed as byte-exact C, which are expressed as
// reviewed semantic C, and which are still assembly or non-code asset data.
//
// Exact and semantic ownership are derived from tracked evidence without a ROM
// read or compiler. The dark-gray retained-assembly layer additionally consumes
// the latest verified full-build assembly manifest; when that manifest is
// absent, only explicit tracked non-code ranges are dark gray and the rest
// stays gray.
//
//   * executable classification  metrics/<target>-executable.json
//   * exact main ownership       exact/<address>.c against audited region bounds
//   * exact overlay ownership    assets/code/*_overlay.s AlchemyC placeholders
//   * semantic ownership         semantic/ sources and their tracked spans
//   * ROM layout                 assets/manifest.json addresses and sizes
//
// The derived exact totals are reconciled against the tracked Full-C report
// before anything is written: a mismatch is an error, never a rounded picture.
//
// PORT NOTE (module wiring): `tools/metrics/coverage_map.ts` imports
// `canonicalCSource`, `roundHalfUpPercent` and `unionIntervals` from
// the Full-C progress implementation in `tools-rs/full-c-progress`. That crate
// publishes no `[lib]` target, so a
// Cargo `path` dependency is impossible without editing another crate's
// manifest. The available form of "depend on it, do not reimplement it" is to
// include its sources directly with `#[path]`, which is what the three
// declarations below do. `intervals.rs` contains `use crate::json;`, so the
// json module has to be named `json` at this crate root.
//
// PORT NOTE (closure): the import list above is the entire dependency surface.
// The map never calls `assembleOverlay`, so it never reaches the alchemy-gcc
// compile plan and needs no `bun` sidecar; this binary is fully native.

// PORT NOTE (crate shape): the modules live in a library root rather than in
// `main.rs` because the tool is only half ported. The renderers that call
// `tracked_document`, `asset_bucket` and friends are not written yet, and in a
// binary crate those faithful ports would read as dead code. A library root
// makes them reachable API instead of unreachable code, which is the honest
// structure and also makes the crate linkable by later ports. Nothing is
// suppressed to achieve it.


#[path = "../../full-c-progress/src/json.rs"]
pub mod json;


#[path = "../../full-c-progress/src/js.rs"]
pub mod js;


#[path = "../../full-c-progress/src/intervals.rs"]
pub mod intervals;

pub mod assets;
pub mod base64;
pub mod cli;
pub mod jsnum;
pub mod jstext;
pub mod sha1;
pub mod svg;
pub mod treemap;
pub mod boxtree;
pub mod verified;
pub mod model;
pub mod names;
pub mod ordered;
pub mod ownership;
pub mod pipeline;
pub mod selftest;
pub mod spans;
pub mod tiles;
pub mod testsupport;
pub mod tree;
