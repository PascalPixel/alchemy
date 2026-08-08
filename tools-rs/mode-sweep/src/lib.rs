//! Native mode-sweep planning, scoring, and reporting.
//!
//! mode_sweep explores historically plausible compiler configurations for one C
//! candidate. It is a diagnostic tool, not a promotion tool: it searches the
//! routed compiler first, then one change at a time, compatible pairs only when
//! asked, and triples only when an exact-sized pair is already within five
//! halfwords.
//!
//! WHAT IS PORTED HERE: the mode table, the compatibility algebra, all four
//! planning phases, the difference classifier, argument parsing, the cache-entry
//! acceptance predicate, the content-addressed hashing and the self-test.
//! Everything in that list is pure and testable, and it is what
//! `mode_cohort.ts`, `overlay_mode_cohort.ts` and `search-compiler-modes`
//! actually import.
//!
//! The compiler boundary is the native `candidate-compiler` crate. This crate
//! deliberately owns no second compiler implementation: routing and command
//! construction remain in the Rust layers below it.

use std::path::{Path, PathBuf};

use search_compiler_modes::{canonical_json, sha256_hex, Json};

// ---------------------------------------------------------------------------
// Repository root
// ---------------------------------------------------------------------------

/// The repository root for the native mode-sweep crate.
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("tools-rs/mode-sweep is two levels below the repository root")
        .to_path_buf()
}

/// `FORMAT` in the TypeScript: the report/cache schema version. A bump
/// invalidates every cached score because it is hashed into the cache key.
pub const FORMAT: u32 = 4;

// ---------------------------------------------------------------------------
// Fork modes
// ---------------------------------------------------------------------------

/// The fork modes, in source order.
///
/// The comments are the incident record: each one names the witness address
/// that forced the flag into the fork. They are load-bearing documentation, not
/// decoration -- a mode with no witness is a mode nobody can justify keeping.
///
/// Order is load-bearing twice over: `MODES` appends these in order, and every
/// pair/triple enumeration walks `MODES` by index before sorting, so a
/// reordering changes which configurations survive a `--max-pairs` cut.
pub const FORK_MODES: &[&str] = &[
    "-mgrouped-dma-store",
    "-mpreserve-single-bit-test",
    "-mentry-low-register-order",
    "-mthumb-and-sets-cc",
    "-mcall-arg0-move-first",
    "-mearly-frame-allocation",
    "-mhigh-register-move-first",
    "-mthumb-entry-literal-first",
    "-mthumb-early-literal-pool",
    "-mthumb-immediate-latency",
    "-mthumb-load-latency-one",
    "-fno-thumb-contiguous-immediate",
    "-fthumb-split-group-base",
    "-fthumb-hoist-parameter-save",
    "-fthumb-minipool-tail-first",
    "-fthumb-entry-saves-descending",
    "-fthumb-group-control-last",
    "-fthumb-group-value1-before-base",
    "-fthumb-move-before-alu",
    "-fthumb-orr-dead-input-reuse",
    "-fthumb-call-arg1-before-arg0",
    "-fthumb-call-arg0-reg-source",
    "-fthumb-sink-constant-past-memory",
    "-fthumb-sink-store-past-store",
    "-fthumb-earliest-frame-allocation",
    "-fthumb-copy-before-add-immediate",
    "-fthumb-sink-add-immediate",
    "-fthumb-hoist-add-immediate",
    "-fthumb-no-constant-reuse",
    "-fthumb-pool-load-base-first",
    "-fthumb-move-before-unary-alu",
    // Same transposition without the "only undo a scheduler inversion" gate,
    // restricted to a pair of plain literals. Witness resource_3ae:02dc.
    "-fthumb-call-literal-arg1-first",
    "-fthumb-call-arg0-before-store",
    // Mirror of -fthumb-next-arg-between-split: the last plain immediate call
    // argument goes ahead of a preceding split constant's shift. Witness
    // resource_3b1:366c.
    "-fthumb-arg-before-final-shift",
    // Inverse of -fthumb-call-pool-arg1-first, gated on the third argument still
    // being written after r0. Witness resource_371:1888.
    "-fthumb-call-arg0-before-pool",
    // Register-move twin of the above. Witness resource_3a7:0b8c.
    "-fthumb-call-argreg-before-pool",
    // Age-ordered transposition of two adjacent in-place constant shifts.
    // Witnesses resource_3bc:4494 and resource_3a4:02cc.
    "-fthumb-swap-adjacent-shifts",
    // A pool load that completes a call's argument list, sunk to the call.
    // Witness resource_3c6:0158.
    "-fthumb-sink-pool-load-to-use",
    // Unwired fork mode found by sweeping toplev.c directly; witness resource_39e:26d8.
    "-fthumb-sink-constant-past-call",
    // Two-pool-word twin of -fthumb-call-arg0-before-pool. Witness
    // resource_371:1a98.
    "-fthumb-call-arg0-before-pool-pair",
    // The orr destination tie, resolved toward the older input. Witness
    // resource_3b3:1fd4.
    "-fthumb-orr-into-older-input",
    // Two in-place constant shifts transposed across one unrelated insn.
    // Witness resource_3c8:2f30.
    // the thirteen :0104 integrators -- the load below the store
    "-fthumb-sink-load-past-store",
    "-fthumb-high-move-before-store",
    "-fthumb-pool-load-before-load",
    "-fthumb-shift-before-store-in-split",
    // resource_39b:0f48 -- the immediate between two pool loads
    "-fthumb-call-arg0-between-pool-pair",
    // resource_3ca:004c -- the 0x05000000 halfword write
    "-fthumb-store-value-before-base",
    "-fthumb-swap-shifts-across-insn",
    // The plain argument ahead of a mid-sheet split-constant shift.
    // Witness resource_3b1:5ca4.
    "-fthumb-arg-before-shift-in-sheet",
    // The literal r1-before-r0 pair, only in a sheet opening after a call.
    // Witnesses resource_3b9:06bc, resource_376:0258, resource_376:0190.
    "-fthumb-call-literal-arg1-first-after-call",
    "-fthumb-call-literal-arg1-first-chained",
    "-fthumb-small-shift-before-immediates",
    "-fthumb-blockmove-dest-before-source",
    // Both stacked arguments in registers before either store.
    // Witnesses resource_382:0fb4/:1010/:113c, resource_3b9:1c6c.
    "-fthumb-stack-args-before-stores",
    "-fthumb-postcall-byte-increment-r2",
    "-fthumb-entry-frame-cluster",
    "-fthumb-literal-before-index-shift",
    "-fthumb-low-constant-before-high-move",
    "-fthumb-high-move-before-stack-store",
    // Source-routed 0807a664 backend fingerprint; explored explicitly so the
    // evidence gate cannot hide a live route from future sweeps.
    "-fthumb-0807a664-exact",
    // The fork has carried this one since the grouped-descriptor work but neither
    // the explorer nor the router ever named it, so it was invisible to every
    // sweep. It is what closes 080b5ad4.
    "-fthumb-group-value2-in-place",
    // Grouped-DMA control-word class, fork commit cee872a (2026-08-05):
    // rematerialise a shared pool-class control word per transfer, and the
    // sched2 pool-load-late ready-list tie-break. Witnesses 080f377c and
    // 08091174.
    "-fthumb-group-control-rematerialize",
    "-fthumb-sched-pool-load-late",
    // Thumb leaf link-register and if-conversion classes, 2026-08-06. The first
    // stops the spurious push {lr} on empty-frame Thumb leaves whose far-jump
    // answer is latched before branch lengths exist; the second models a
    // compiler without ifcvt.c, which gcc 2.95 did not have. Witness 080fa1ac.
    "-fthumb-leaf-no-lr",
    "-fthumb-no-if-convert",
    // Routed in alchemy_gcc.ts but never registered here, so no sweep could
    // reach them. A routed mode absent from this list is a mode no sweep can
    // rediscover on a fresh owner.
    "-fthumb-arg0-after-split",
    "-fthumb-call-arg0-pool-load",
    "-fthumb-collapse-dead-scratch",
    "-fthumb-group-zero-any-register",
    "-fthumb-return-value-before-stack-adjust",
    "-fthumb-sink-block-constant",
    "-fthumb-sink-dependent-load",
    "-fthumb-sink-group-pool-loads",
    "-fthumb-sink-past-pool-load",
    "-fthumb-sink-stack-adjust",
    "-mlow-reg-order=2013",
    "-mlow-reg-order=01231230",
    "-mlow-reg-order=30120123",
    // 2026-08-07: -mcallee-reg-order= permutes the r4-r7 run of REG_ALLOC_ORDER,
    // the gap between -mlow-reg-order= (r0-r3) and -mhigh-reg-order= (r8-r11).
    // Three owners reached instruction-for-instruction agreement with two
    // call-saved pseudos swapped, which no source form could move. Only the
    // adjacent transpositions are registered; the full 24 permutations are
    // reachable by hand when a floor is tagged [register] and nothing else.
    "-mcallee-reg-order=1023",
    "-mcallee-reg-order=0213",
    "-mcallee-reg-order=0132",
    // 2026-08-07: the mirror of -mearly-frame-allocation. It lowers a Thumb
    // stack decrement's scheduling priority so incoming-argument copies issue
    // ahead of `sub sp, #N'. Witness 080b0744.
    "-fthumb-late-frame-allocation",
    // 2026-08-07: the r0-value twin of -fthumb-group-control-last. Witness
    // 0801a4fc.
    "-fthumb-group-pooled-control-last",
    // 2026-08-07: high-register variant of -fthumb-move-before-alu. Witness
    // 0808b868.
    "-fthumb-high-move-before-alu",
    // 2026-08-07: the immediate-operand widening of -fthumb-move-before-alu,
    // which also accepts a load as the insn issued early. Witness 0801fd34.
    "-fthumb-move-before-immediate-alu",
    // 2026-08-07: anchor hoisted loop invariants at the head of the preheader
    // block rather than immediately before the loop note, so they lead the
    // preheader's own insns instead of following them. Witness 080b5d3c.
    "-floop-invariant-block-head",
    // 2026-08-07: the ARM back end's CANONICALIZE_COMPARISON rewrites `x > C'
    // into `x >= C+1' whenever C+1 passes const_ok_for_arm -- an ARM-mode
    // immediate predicate that says nothing about Thumb, where the rewrite
    // trades a pool word for a two-insn constant build and flips the emitted
    // condition code. This suppresses it in Thumb. Witness 0808ddec's tail.
    "-fthumb-no-canonicalize-comparison",
    // A small HImode constant reaches its register with `movs' instead of the
    // literal-pool load *thumb_movhi_insn's "mn" alternative forces. Witness
    // resource_377:03f8 (2026-08-07).
    "-fthumb-hi-immediate",
    "-fthumb-call-pool-arg1-first",
];

/// The stock GCC pass switches that the
/// historical-family factorial phase crosses against each alternative compiler.
pub const STOCK_SWITCHES: &[&str] = &[
    "-fno-defer-pop",
    "-fno-thread-jumps",
    "-fno-omit-frame-pointer",
    "-fno-schedule-insns2",
    "-fno-gcse",
    "-fno-cse-follow-jumps",
    "-fno-cse-skip-blocks",
    "-fno-expensive-optimizations",
    "-fno-peephole",
    "-fno-strength-reduce",
    "-fno-regmove",
    "-fno-rerun-cse-after-loop",
    "-fno-rerun-loop-opt",
    "-fno-caller-saves",
    "-fno-force-mem",
    "-fno-peephole2",
    "-fno-inline-functions",
    "-fno-delete-null-pointer-checks",
    "-fno-sched-depend-count",
    "-fno-optimize-sibling-calls",
    "-fno-canonicalize-comparison",
];

// ---------------------------------------------------------------------------
// Modes
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Family {
    Compiler,
    Optimization,
    Abi,
    Scheduler,
    Cse,
    RegisterAllocation,
    Backend,
}

/// Candidate compiler family names. Kept as strings because the
/// value is written verbatim into `config.compiler_family` in every report.
pub const ROUTED: &str = "routed";

#[derive(Debug, Clone)]
pub struct Mode {
    pub id: String,
    pub family: Family,
    pub add_flags: Vec<String>,
    pub remove_flags: Vec<String>,
    pub compiler_family: Option<&'static str>,
    /// `None` is TypeScript's `undefined`: no restriction at all. An empty
    /// vector would mean "supported nowhere", which is a different thing.
    pub supported_compiler_families: Option<Vec<&'static str>>,
    pub exclusive: bool,
    pub evidence: &'static str,
}

struct Spec {
    id: &'static str,
    family: Family,
    add: &'static [&'static str],
    remove: &'static [&'static str],
    compiler_family: Option<&'static str>,
    supported: Option<&'static [&'static str]>,
    exclusive: bool,
    evidence: &'static str,
}

const HISTORICAL: &str = "historical";
const PROVEN: &str = "proven-routing";

const fn spec(
    id: &'static str,
    family: Family,
    add: &'static [&'static str],
    evidence: &'static str,
) -> Spec {
    Spec {
        id,
        family,
        add,
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: false,
        evidence,
    }
}

/// The declared prefix of `MODES`, before the generated `FORK_MODES` and
/// old-agbcc tails are appended.
const DECLARED: &[Spec] = &[
    Spec {
        id: "compiler-gcc296",
        family: Family::Compiler,
        add: &[],
        remove: &[],
        compiler_family: Some("gcc296"),
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "compiler-old-agbcc",
        family: Family::Compiler,
        add: &[],
        remove: &[],
        compiler_family: Some("old-agbcc"),
        supported: None,
        exclusive: true,
        evidence: PROVEN,
    },
    Spec {
        id: "compiler-pret-early-thumb",
        family: Family::Compiler,
        add: &[],
        remove: &[],
        compiler_family: Some("pret-early-thumb"),
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "compiler-gcc2951",
        family: Family::Compiler,
        add: &[],
        remove: &[],
        compiler_family: Some("gcc2951"),
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "opt-o1",
        family: Family::Optimization,
        add: &["-O1"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: PROVEN,
    },
    Spec {
        id: "opt-o2",
        family: Family::Optimization,
        add: &["-O2"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "opt-o3",
        family: Family::Optimization,
        add: &["-O3"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "opt-os",
        family: Family::Optimization,
        add: &["-Os"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    Spec {
        id: "abi-standard-r4",
        family: Family::Abi,
        add: &[],
        remove: &["-fcall-used-r4"],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: PROVEN,
    },
    Spec {
        id: "abi-fixed-r3",
        family: Family::Abi,
        add: &["-ffixed-r3"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: PROVEN,
    },
    Spec {
        id: "abi-fixed-lr",
        family: Family::Abi,
        add: &["-ffixed-r14"],
        remove: &[],
        compiler_family: None,
        supported: None,
        exclusive: true,
        evidence: HISTORICAL,
    },
    spec(
        "call-defer-pop-off",
        Family::Backend,
        &["-fno-defer-pop"],
        HISTORICAL,
    ),
    spec(
        "cfg-thread-jumps-off",
        Family::Cse,
        &["-fno-thread-jumps"],
        HISTORICAL,
    ),
    spec(
        "frame-pointer-kept",
        Family::Backend,
        &["-fno-omit-frame-pointer"],
        HISTORICAL,
    ),
    spec(
        "sched-prereload-off",
        Family::Scheduler,
        &["-fno-schedule-insns"],
        PROVEN,
    ),
    spec(
        "sched-postreload-off",
        Family::Scheduler,
        &["-fno-schedule-insns2"],
        PROVEN,
    ),
    spec(
        "sched-depend-count-off",
        Family::Scheduler,
        &["-fno-sched-depend-count"],
        PROVEN,
    ),
    spec("cse-gcse-off", Family::Cse, &["-fno-gcse"], PROVEN),
    spec(
        "cse-follow-off",
        Family::Cse,
        &["-fno-cse-follow-jumps"],
        PROVEN,
    ),
    spec(
        "cse-skip-blocks-off",
        Family::Cse,
        &["-fno-cse-skip-blocks"],
        HISTORICAL,
    ),
    spec(
        "cse-rerun-loop-off",
        Family::Cse,
        &["-fno-rerun-cse-after-loop"],
        PROVEN,
    ),
    spec(
        "alias-strict-off",
        Family::Cse,
        &["-fno-strict-aliasing"],
        PROVEN,
    ),
    spec(
        "cse-two-insn-immediate-off",
        Family::Cse,
        &["-fno-cse-two-insn-immediate"],
        PROVEN,
    ),
    spec(
        "cse-shift-immediate-off",
        Family::Cse,
        &["-fno-cse-shift-immediate"],
        PROVEN,
    ),
    spec(
        "cse-pool-immediate-off",
        Family::Cse,
        &["-fno-cse-pool-immediate"],
        PROVEN,
    ),
    spec(
        "gcse-insert-load-off",
        Family::Cse,
        &["-fno-gcse-insert-load"],
        PROVEN,
    ),
    spec(
        "sched-low-dest-first",
        Family::Scheduler,
        &["-fsched-low-dest-first"],
        PROVEN,
    ),
    spec(
        "sched-high-dest-first",
        Family::Scheduler,
        &["-fsched-high-dest-first"],
        PROVEN,
    ),
    spec(
        "sched-alias-off",
        Family::Scheduler,
        &["-fno-sched-alias"],
        PROVEN,
    ),
    spec(
        "sched-store-first",
        Family::Scheduler,
        &["-fsched-store-first"],
        PROVEN,
    ),
    spec(
        "loop-rerun-off",
        Family::Cse,
        &["-fno-rerun-loop-opt"],
        HISTORICAL,
    ),
    spec(
        "cse-expensive-off",
        Family::Cse,
        &["-fno-expensive-optimizations"],
        PROVEN,
    ),
    spec(
        "reg-peephole-off",
        Family::RegisterAllocation,
        &["-fno-peephole"],
        HISTORICAL,
    ),
    spec(
        "reg-strength-reduce-off",
        Family::RegisterAllocation,
        &["-fno-strength-reduce"],
        PROVEN,
    ),
    spec(
        "reg-regmove-off",
        Family::RegisterAllocation,
        &["-fno-regmove"],
        PROVEN,
    ),
    spec(
        "reg-caller-saves-off",
        Family::RegisterAllocation,
        &["-fno-caller-saves"],
        HISTORICAL,
    ),
    spec(
        "reg-force-mem-off",
        Family::RegisterAllocation,
        &["-fno-force-mem"],
        HISTORICAL,
    ),
    Spec {
        id: "reg-peephole2-off",
        family: Family::RegisterAllocation,
        add: &["-fno-peephole2"],
        remove: &[],
        compiler_family: None,
        supported: Some(&[ROUTED, "gcc296"]),
        exclusive: false,
        evidence: HISTORICAL,
    },
    spec(
        "inline-functions-off",
        Family::Backend,
        &["-fno-inline-functions"],
        HISTORICAL,
    ),
    Spec {
        id: "delete-null-checks-off",
        family: Family::Backend,
        add: &["-fno-delete-null-pointer-checks"],
        remove: &[],
        compiler_family: None,
        supported: Some(&[ROUTED, "gcc296"]),
        exclusive: false,
        evidence: HISTORICAL,
    },
    Spec {
        id: "sibling-calls-off",
        family: Family::Backend,
        add: &["-fno-optimize-sibling-calls"],
        remove: &[],
        compiler_family: None,
        supported: Some(&[ROUTED, "gcc296"]),
        exclusive: false,
        evidence: PROVEN,
    },
    spec(
        "comparison-canonicalization-off",
        Family::Backend,
        &["-fno-canonicalize-comparison"],
        PROVEN,
    ),
];

/// The old-agbcc backend flags appended after `FORK_MODES`.
const AGBCC_FLAGS: &[&str] = &[
    "-mliteral-before-shift",
    "-mcommutative-copy-constant",
    "-mprologue-next-high-reg",
    "-mcompare-only-and-tst",
];

/// The complete mode table, in exact source order.
pub fn modes() -> Vec<Mode> {
    let mut list: Vec<Mode> = DECLARED
        .iter()
        .map(|item| Mode {
            id: item.id.to_string(),
            family: item.family,
            add_flags: item.add.iter().map(|f| f.to_string()).collect(),
            remove_flags: item.remove.iter().map(|f| f.to_string()).collect(),
            compiler_family: item.compiler_family,
            supported_compiler_families: item.supported.map(|s| s.to_vec()),
            exclusive: item.exclusive,
            evidence: item.evidence,
        })
        .collect();
    for flag in FORK_MODES {
        list.push(Mode {
            // `flag.slice(2)` in the TypeScript: drop the leading "-f" / "-m".
            // PORT NOTE: every FORK_MODES entry is ASCII, so a byte slice and a
            // UTF-16 slice agree. `fork_mode_ids_are_ascii` pins that.
            id: flag[2..].to_string(),
            family: Family::Backend,
            add_flags: vec![flag.to_string()],
            remove_flags: Vec::new(),
            compiler_family: None,
            supported_compiler_families: None,
            exclusive: false,
            evidence: PROVEN,
        });
    }
    for flag in AGBCC_FLAGS {
        list.push(Mode {
            id: format!("agbcc-{}", &flag[2..]),
            family: Family::Backend,
            add_flags: vec![flag.to_string()],
            remove_flags: Vec::new(),
            compiler_family: Some("old-agbcc"),
            supported_compiler_families: None,
            exclusive: false,
            evidence: PROVEN,
        });
    }
    list
}

// ---------------------------------------------------------------------------
// Configurations
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Config {
    pub ids: Vec<String>,
    pub flags: Vec<String>,
    pub remove_flags: Vec<String>,
    pub compiler_family: String,
}

impl Config {
    /// `config.ids.join("+")`, the sort key used by every planning phase.
    pub fn key(&self) -> String {
        self.ids.join("+")
    }

    /// The JSON form, in TypeScript property-declaration order. That order is
    /// load-bearing: it is hashed into the cache key through `canonicalJson`.
    pub fn to_json(&self) -> Json {
        Json::Object(vec![
            (
                "ids".into(),
                Json::Array(self.ids.iter().map(|s| Json::String(s.clone())).collect()),
            ),
            (
                "flags".into(),
                Json::Array(self.flags.iter().map(|s| Json::String(s.clone())).collect()),
            ),
            (
                "remove_flags".into(),
                Json::Array(
                    self.remove_flags
                        .iter()
                        .map(|s| Json::String(s.clone()))
                        .collect(),
                ),
            ),
            (
                "compiler_family".into(),
                Json::String(self.compiler_family.clone()),
            ),
        ])
    }
}

/// JavaScript's default `Array#sort` comparator on strings: ascending UTF-16
/// code unit order. Mode ids are ASCII, so byte order agrees.
fn sort_code_unit(items: &mut [String]) {
    items.sort();
}

/// `localeCompare` under the `en-US` default collator, restricted to the
/// alphabet mode ids actually use.
///
/// PORT NOTE -- THIS IS NOT `str::cmp`. mode_sweep sorts configurations with
/// `a.ids.join("+").localeCompare(b.ids.join("+"))`, and ICU root collation
/// orders punctuation before symbols before digits before letters. In ASCII,
/// `'+'` (0x2B) sorts before `'-'` (0x2D); under the collator the order is the
/// other way round, and `'='` (0x3D) sorts before the digits instead of after
/// them. Sorting these keys with `str::cmp` produces a different plan, and
/// `--max-pairs` then cuts a different set of configurations.
///
/// Measured against Bun's `localeCompare`, not assumed:
///   `"-+=0123456789abcdefghijklmnopqrstuvwxyz"` is the sorted character set,
///   and `["a-b","a+b","a=b","a0b","ab"]` is the sorted string set, so within
///   this alphabet the collator degenerates to a per-character rank compare
///   with shorter-prefix-first. `collation_matches_measured_bun_order` pins
///   both observations, and `mode_ids_use_only_the_collated_alphabet` fails if
///   a future mode id introduces a character this rank table does not cover.
/// PORT NOTE -- CASE IS A TERTIARY WEIGHT, NOT A POSITION IN ONE ALPHABET.
/// An earlier form of this function ranked characters in a single pass. That is
/// wrong the moment a key mixes case, because ICU only consults case once the
/// entire case-folded key has compared equal. Measured in Bun:
///   `"A".localeCompare("a-b") === -1`, which a single interleaved `aAbB` table
///   answers `+1`, since `A` loses on the primary key's length before its case
///   is ever looked at.
///   `"a-c".localeCompare("ab") === -1`, so punctuation is NOT variable-weighted
///   and does carry a primary weight below every digit and letter.
/// The correct form is two passes: case-folded primary, then case as a tiebreak.
/// This is not currently reachable here, because every mode id is lowercase and
/// `mode_ids_use_only_the_collated_alphabet` enforces that, but the crate was
/// one uppercase id away from silently reshuffling every report.
pub fn collate(left: &str, right: &str) -> std::cmp::Ordering {
    fn rank(byte: u8) -> u32 {
        match byte.to_ascii_lowercase() {
            b'-' => 0,
            b'+' => 1,
            b'=' => 2,
            folded @ b'0'..=b'9' => 10 + (folded - b'0') as u32,
            folded @ b'a'..=b'z' => 100 + (folded - b'a') as u32,
            // Deliberately unreachable for real mode ids; see the alphabet
            // test. Falling back to the raw byte keeps the comparator total so
            // an unexpected character cannot panic mid-sort.
            other => 1000 + other as u32,
        }
    }
    /// Lowercase before uppercase, consulted only on a primary tie.
    fn case_rank(byte: u8) -> u32 {
        u32::from(byte.is_ascii_uppercase())
    }
    compare_by(left, right, rank).then_with(|| compare_by(left, right, case_rank))
}

fn compare_by(left: &str, right: &str, weight: fn(u8) -> u32) -> std::cmp::Ordering {
    let mut left_ranks = left.bytes().map(weight);
    let mut right_ranks = right.bytes().map(weight);
    loop {
        match (left_ranks.next(), right_ranks.next()) {
            (None, None) => return std::cmp::Ordering::Equal,
            (None, Some(_)) => return std::cmp::Ordering::Less,
            (Some(_), None) => return std::cmp::Ordering::Greater,
            (Some(a), Some(b)) if a != b => return a.cmp(&b),
            _ => {}
        }
    }
}

/// The alphabet `collate` is calibrated for.
pub const COLLATED_ALPHABET: &str = "-+=0123456789abcdefghijklmnopqrstuvwxyz";

/// Port of `compatible`.
pub fn compatible(selection: &[&Mode]) -> bool {
    let mut exclusive: Vec<Family> = Vec::new();
    let mut flags: Vec<&str> = Vec::new();
    for mode in selection {
        if mode.exclusive && exclusive.contains(&mode.family) {
            return false;
        }
        if mode.exclusive {
            exclusive.push(mode.family);
        }
        for flag in mode.add_flags.iter().chain(mode.remove_flags.iter()) {
            if flags.contains(&flag.as_str()) {
                return false;
            }
            flags.push(flag);
        }
    }
    let mut selected: Vec<&str> = Vec::new();
    for mode in selection {
        if let Some(family) = mode.compiler_family {
            if !selected.contains(&family) {
                selected.push(family);
            }
        }
    }
    if selected.len() > 1 {
        return false;
    }
    let compiler = *selected.first().unwrap_or(&ROUTED);
    if selection.iter().any(|mode| {
        mode.supported_compiler_families
            .as_ref()
            .is_some_and(|supported| !supported.contains(&compiler))
    }) {
        return false;
    }
    if compiler == "old-agbcc"
        && selection.iter().any(|mode| {
            mode.family == Family::Scheduler
                || mode.family == Family::Backend && mode.compiler_family != Some("old-agbcc")
        })
    {
        return false;
    }
    true
}

/// Port of `configOf`.
pub fn config_of(selection: &[&Mode]) -> Config {
    let mut ids: Vec<String> = selection.iter().map(|mode| mode.id.clone()).collect();
    sort_code_unit(&mut ids);
    Config {
        ids,
        flags: selection
            .iter()
            .flat_map(|mode| mode.add_flags.clone())
            .collect(),
        remove_flags: selection
            .iter()
            .flat_map(|mode| mode.remove_flags.clone())
            .collect(),
        compiler_family: selection
            .iter()
            .find_map(|mode| mode.compiler_family)
            .unwrap_or(ROUTED)
            .to_string(),
    }
}

/// Port of `singleConfigs`: the routed default first, then one mode at a time.
pub fn single_configs() -> Vec<Config> {
    let table = modes();
    let mut configs = vec![Config {
        ids: Vec::new(),
        flags: Vec::new(),
        remove_flags: Vec::new(),
        compiler_family: ROUTED.to_string(),
    }];
    configs.extend(table.iter().map(|mode| config_of(&[mode])));
    configs
}

/// Port of `pairConfigs`. `limit` is TypeScript's default `Infinity` when
/// `None`; `Array#slice` clamps, so an over-large limit is not an error.
pub fn pair_configs(limit: Option<usize>) -> Vec<Config> {
    let table = modes();
    let mut configs: Vec<Config> = Vec::new();
    for left in 0..table.len() {
        for right in (left + 1)..table.len() {
            let selection = [&table[left], &table[right]];
            if compatible(&selection) {
                configs.push(config_of(&selection));
            }
        }
    }
    configs.sort_by(|a, b| collate(&a.key(), &b.key()));
    truncate(configs, limit)
}

fn truncate(mut configs: Vec<Config>, limit: Option<usize>) -> Vec<Config> {
    if let Some(limit) = limit {
        configs.truncate(limit);
    }
    configs
}

/// A single-phase score as far as `rankedPairConfigs` reads it.
#[derive(Debug, Clone)]
pub struct Rank {
    pub exact: bool,
    /// `Infinity` when the configuration produced no evidence at all.
    pub floor: f64,
}

/// Port of `rankedPairConfigs`.
///
/// PORT NOTE ON NaN: the TypeScript comparator is
/// `quality(left) - quality(right) || localeCompare(...)`. `quality` sums
/// `Infinity` for every id with no evidence, so `Infinity - Infinity` yields
/// `NaN` whenever both sides are unevidenced -- and `NaN` is falsy, so the
/// comparator silently falls through to the `localeCompare` tiebreak instead of
/// reporting "equal". `f64::partial_cmp` would give `None` there and
/// `total_cmp` would give an ordering, so both are wrong. This reproduces the
/// falsy-difference rule exactly.
pub fn ranked_pair_configs(single_ranks: &[(String, Rank)], limit: usize) -> Vec<Config> {
    // `new Map(...)`: a repeated key overwrites in place, keeping the position
    // of the first insertion. Only the value is observable here.
    let mut ranks: Vec<(String, Rank)> = Vec::new();
    for (id, rank) in single_ranks {
        match ranks.iter_mut().find(|(name, _)| name == id) {
            Some(slot) => slot.1 = rank.clone(),
            None => ranks.push((id.clone(), rank.clone())),
        }
    }
    let quality = |config: &Config| -> f64 {
        config.ids.iter().fold(0.0f64, |sum, id| {
            match ranks.iter().find(|(name, _)| name == id) {
                Some((_, rank)) => sum + if rank.exact { -1_000_000.0 } else { 0.0 } + rank.floor,
                None => sum + 1_000_000.0,
            }
        })
    };
    let mut configs = pair_configs(None);
    configs.sort_by(|left, right| {
        let difference = quality(left) - quality(right);
        // `||` in JavaScript: 0, -0 and NaN are all falsy and fall through.
        if difference.is_nan() || difference == 0.0 {
            collate(&left.key(), &right.key())
        } else if difference < 0.0 {
            std::cmp::Ordering::Less
        } else {
            std::cmp::Ordering::Greater
        }
    });
    configs.truncate(limit);
    configs
}

/// Port of `tripleConfigs`. A triple needs evidence: at least two of its
/// constituents must have occurred in a strong pair or single result. That is
/// what stops a cubic blind permutation.
pub fn triple_configs(seed_ids: &[String], limit: Option<usize>) -> Vec<Config> {
    let table = modes();
    let seed: Vec<&str> = seed_ids.iter().map(|s| s.as_str()).collect();
    let mut configs: Vec<Config> = Vec::new();
    for a in 0..table.len() {
        for b in (a + 1)..table.len() {
            for c in (b + 1)..table.len() {
                let selection = [&table[a], &table[b], &table[c]];
                let seeded = selection
                    .iter()
                    .filter(|mode| seed.contains(&mode.id.as_str()))
                    .count();
                if seeded < 2 || !compatible(&selection) {
                    continue;
                }
                configs.push(config_of(&selection));
            }
        }
    }
    configs.sort_by(|a, b| collate(&a.key(), &b.key()));
    truncate(configs, limit)
}

/// Port of `historicalFamilyFactorialConfigs`: each alternative historical
/// compiler crossed with every compatible pair of stock pass switches. These
/// are deterministic and do not depend on a candidate-specific ranking.
pub fn historical_family_factorial_configs() -> Vec<Config> {
    let table = modes();
    let compiler_modes: Vec<&Mode> = table
        .iter()
        .filter(|mode| {
            mode.family == Family::Compiler
                && mode.compiler_family.is_some()
                && mode.compiler_family != Some("gcc296")
        })
        .collect();
    let stock_modes: Vec<&Mode> = table
        .iter()
        .filter(|mode| {
            mode.add_flags.len() == 1 && STOCK_SWITCHES.contains(&mode.add_flags[0].as_str())
        })
        .collect();
    let mut configs: Vec<Config> = Vec::new();
    for compiler in &compiler_modes {
        for left in 0..stock_modes.len() {
            for right in (left + 1)..stock_modes.len() {
                let selection = [*compiler, stock_modes[left], stock_modes[right]];
                if compatible(&selection) {
                    configs.push(config_of(&selection));
                }
            }
        }
    }
    configs.sort_by(|a, b| collate(&a.key(), &b.key()));
    configs
}

/// Port of `combinations`, retained for callers of the old helper.
pub fn combinations(pairs: bool) -> Vec<Vec<String>> {
    let mut out: Vec<Vec<String>> = single_configs().into_iter().map(|c| c.flags).collect();
    if pairs {
        out.extend(pair_configs(None).into_iter().map(|c| c.flags));
    }
    out
}

// ---------------------------------------------------------------------------
// Hashing and content addressing
// ---------------------------------------------------------------------------

/// Port of `hash`: SHA-256 over each part followed by a NUL separator. The
/// separator is what stops `hash("ab","c")` colliding with `hash("a","bc")`.
pub fn hash(parts: &[&[u8]]) -> String {
    let mut buffer: Vec<u8> = Vec::new();
    for part in parts {
        buffer.extend_from_slice(part);
        buffer.push(0);
    }
    sha256_hex(&buffer)
}

/// Native mode-sweep output directory, delegating to the existing
/// implementation in `search-compiler-modes` rather than keeping a second copy.
pub fn mode_sweep_output_directory(root: &Path, source: &str, contents: &[u8]) -> String {
    search_compiler_modes::mode_sweep_output_directory(root, source, contents)
}

/// Native source modules whose bytes go into the compiler signature.
pub const SIGNATURE_SOURCES: [&str; 3] = [
    "tools-rs/mode-sweep/src/lib.rs",
    "tools-rs/mode-sweep/src/main.rs",
    "tools-rs/candidate-compiler/src/verify.rs",
];

/// Native compiler signature inputs.
pub fn signature_source_paths(root: &Path) -> Vec<PathBuf> {
    SIGNATURE_SOURCES
        .iter()
        .map(|name| root.join(name))
        .collect()
}

/// Port of `compilerSignature` over an explicit path list, so the bug-for-bug
/// and corrected variants share one implementation.
pub fn compiler_signature_from(
    bundle_signature: &str,
    paths: &[PathBuf],
) -> std::io::Result<String> {
    let mut parts: Vec<Vec<u8>> = vec![bundle_signature.as_bytes().to_vec()];
    for path in paths {
        parts.push(std::fs::read(path)?);
    }
    let borrowed: Vec<&[u8]> = parts.iter().map(|p| p.as_slice()).collect();
    Ok(hash(&borrowed))
}

pub fn compiler_signature(root: &Path) -> std::io::Result<String> {
    compiler_signature_from(
        &alchemy_bundle::bundle::compiler_bundle_signature(),
        &signature_source_paths(root),
    )
}

// ---------------------------------------------------------------------------
// Routed-coverage contract
// ---------------------------------------------------------------------------

/// Port of `evidencedRoutingFlags` from tools/lib/alchemy_gcc.ts, built on the
/// routing tables already ported into `alchemy-routing`.
///
/// This is the executable coverage contract behind the self-test: adding a
/// routed mode without adding a matching explorer mode must fail. A routed mode
/// absent from `FORK_MODES` is a mode no sweep can rediscover.
pub fn evidenced_routing_flags() -> Vec<String> {
    use alchemy_routing::routing::{
        agbcc_cflags, cflags, cflags_for_target_source, gs2_cflags, CompilerTarget,
    };
    let mut baseline: Vec<String> = Vec::new();
    for flag in cflags()
        .into_iter()
        .chain(gs2_cflags())
        .chain(agbcc_cflags())
    {
        if !baseline.contains(&flag) {
            baseline.push(flag);
        }
    }
    let mut found: Vec<String> = Vec::new();
    let mut address: u32 = 0x0800_0000;
    while address < 0x0810_0000 {
        let source = format!("/tmp/{address:08x}.c");
        for flag in cflags_for_target_source(CompilerTarget::Gs1, &source) {
            if !baseline.contains(&flag) && !found.contains(&flag) {
                found.push(flag);
            }
        }
        address += 4;
    }
    found
}

// ---------------------------------------------------------------------------
// Classification
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub struct Evidence {
    pub differing_halfwords: u64,
    pub size_delta: i64,
    pub exact: bool,
    pub exact_size: bool,
    pub instruction_order_proxy: bool,
    pub register_allocation_proxy: u64,
    pub literal_placement_proxy: bool,
    pub control_flow_proxy: bool,
}

impl Evidence {
    pub fn to_json(&self) -> Json {
        Json::Object(vec![
            (
                "differing_halfwords".into(),
                Json::Number(self.differing_halfwords as f64),
            ),
            ("size_delta".into(), Json::Number(self.size_delta as f64)),
            ("exact".into(), Json::Bool(self.exact)),
            ("exact_size".into(), Json::Bool(self.exact_size)),
            (
                "instruction_order_proxy".into(),
                Json::Bool(self.instruction_order_proxy),
            ),
            (
                "register_allocation_proxy".into(),
                Json::Number(self.register_allocation_proxy as f64),
            ),
            (
                "literal_placement_proxy".into(),
                Json::Bool(self.literal_placement_proxy),
            ),
            (
                "control_flow_proxy".into(),
                Json::Bool(self.control_flow_proxy),
            ),
        ])
    }
}

/// Port of `mnemonic`: `instruction.split(/\s+/)[0] ?? ""`.
///
/// PORT NOTE: JavaScript's `String#split` on a regex that matches at position
/// zero yields a leading empty string, so a leading-space instruction has an
/// empty mnemonic. `str::split_whitespace` would skip it. That difference is
/// reproduced, and `mnemonic_of_leading_space_is_empty` pins it.
pub fn mnemonic(instruction: &str) -> String {
    let bytes = instruction.as_bytes();
    let mut end = 0;
    while end < bytes.len() && !is_js_regex_space(bytes[end]) {
        end += 1;
    }
    instruction[..end].to_string()
}

/// The characters JavaScript's `\s` matches, restricted to the single-byte
/// range the disassembly actually contains. The multi-byte members of `\s`
/// (U+00A0, U+FEFF, U+2028/9, U+3000) cannot occur in objdump output.
fn is_js_regex_space(byte: u8) -> bool {
    matches!(byte, b' ' | b'\t' | b'\n' | b'\r' | 0x0b | 0x0c)
}

fn is_word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// Port of `normalizedRegisters`:
/// `replace(/\b(?:r(?:1[0-5]|[0-9])|sp|lr|pc)\b/gi, "REG")`.
///
/// The alternation order is load-bearing: `r1[0-5]` is tried before `r[0-9]`,
/// so `r10` normalizes whole rather than leaving a stray `0`. JavaScript `\b`
/// is ASCII-only, which is what `is_word_byte` implements.
pub fn normalized_registers(instruction: &str) -> String {
    let bytes = instruction.as_bytes();
    let mut out = String::with_capacity(instruction.len());
    let mut index = 0;
    while index < bytes.len() {
        let boundary_before = index == 0 || !is_word_byte(bytes[index - 1]);
        let matched = if boundary_before {
            register_match(bytes, index)
        } else {
            None
        };
        match matched {
            Some(length)
                if index + length == bytes.len() || !is_word_byte(bytes[index + length]) =>
            {
                out.push_str("REG");
                index += length;
            }
            _ => {
                out.push(bytes[index] as char);
                index += 1;
            }
        }
    }
    out
}

/// Length of the register token starting at `index`, honouring the `/i` flag
/// and the regex's alternation order.
fn register_match(bytes: &[u8], index: usize) -> Option<usize> {
    let lower = |offset: usize| bytes.get(index + offset).map(|b| b.to_ascii_lowercase());
    if lower(0) == Some(b'r') {
        if let (Some(b'1'), Some(digit)) = (lower(1), lower(2)) {
            if (b'0'..=b'5').contains(&digit) {
                return Some(3);
            }
        }
        if let Some(digit) = lower(1) {
            if digit.is_ascii_digit() {
                return Some(2);
            }
        }
        return None;
    }
    match (lower(0), lower(1)) {
        (Some(b's'), Some(b'p')) | (Some(b'l'), Some(b'r')) | (Some(b'p'), Some(b'c')) => Some(2),
        _ => None,
    }
}

/// `/^(?:b|bl|bx)/` -- which, because the alternation is ordered and
/// unanchored at the tail, is simply "starts with `b`".
fn is_branch(instruction: &str) -> bool {
    instruction.starts_with('b')
}

/// `/^ldr/.test(item) && /\bpc\b/.test(item)`.
///
/// PORT NOTE: neither regex carries the `i` flag, unlike `normalizedRegisters`'
/// `/gi`. The comparison is case-SENSITIVE, so `LDR R0, [PC, #16]` is not a
/// literal load as far as this proxy is concerned. Reproduced, not corrected --
/// objdump emits lowercase, so the distinction never fires in practice, but
/// "helpfully" making it case-insensitive would change the proxy's answer on
/// any future uppercase source.
fn is_literal_load(instruction: &str) -> bool {
    if !instruction.starts_with("ldr") {
        return false;
    }
    let bytes = instruction.as_bytes();
    (0..bytes.len().saturating_sub(1)).any(|index| {
        bytes[index] == b'p'
            && bytes[index + 1] == b'c'
            && (index == 0 || !is_word_byte(bytes[index - 1]))
            && (index + 2 == bytes.len() || !is_word_byte(bytes[index + 2]))
    })
}

fn joined<'a>(items: impl Iterator<Item = &'a str>) -> String {
    items.collect::<Vec<_>>().join("\n")
}

/// Port of `classify`.
pub fn classify(
    actual: &[u8],
    reference: &[u8],
    actual_asm: &[String],
    reference_asm: &[String],
) -> Evidence {
    // `Math.ceil(len / 2)` on each side: a trailing odd byte still counts as a
    // whole halfword of difference.
    let halfwords = |length: usize| length.div_ceil(2);
    let mut differing = halfwords(actual.len()).abs_diff(halfwords(reference.len())) as u64;
    // `& ~1`: compare only whole halfwords of the shared prefix.
    let shared = actual.len().min(reference.len()) & !1usize;
    let mut offset = 0;
    while offset < shared {
        let left = u16::from_le_bytes([actual[offset], actual[offset + 1]]);
        let right = u16::from_le_bytes([reference[offset], reference[offset + 1]]);
        if left != right {
            differing += 1;
        }
        offset += 2;
    }

    let actual_mnemonics: Vec<String> = actual_asm.iter().map(|i| mnemonic(i)).collect();
    let reference_mnemonics: Vec<String> = reference_asm.iter().map(|i| mnemonic(i)).collect();
    let histogram = |items: &[String]| {
        // `[...items].sort()`: a copy, sorted by UTF-16 code unit.
        let mut sorted = items.to_vec();
        sort_code_unit(&mut sorted);
        sorted.join("\n")
    };

    let mut register_proxy = 0u64;
    for index in 0..actual_asm.len().min(reference_asm.len()) {
        if actual_asm[index] != reference_asm[index]
            && normalized_registers(&actual_asm[index])
                == normalized_registers(&reference_asm[index])
        {
            register_proxy += 1;
        }
    }

    Evidence {
        differing_halfwords: differing,
        size_delta: actual.len() as i64 - reference.len() as i64,
        exact: actual == reference,
        exact_size: actual.len() == reference.len(),
        instruction_order_proxy: histogram(&actual_mnemonics) == histogram(&reference_mnemonics)
            && actual_mnemonics.join("\n") != reference_mnemonics.join("\n"),
        register_allocation_proxy: register_proxy,
        literal_placement_proxy: joined(
            actual_asm
                .iter()
                .map(String::as_str)
                .filter(|i| is_literal_load(i)),
        ) != joined(
            reference_asm
                .iter()
                .map(String::as_str)
                .filter(|i| is_literal_load(i)),
        ),
        control_flow_proxy: joined(
            actual_asm
                .iter()
                .map(String::as_str)
                .filter(|i| is_branch(i)),
        ) != joined(
            reference_asm
                .iter()
                .map(String::as_str)
                .filter(|i| is_branch(i)),
        ),
    }
}

/// Port of `disassembly`'s line filter:
/// `/^\s+[0-9a-f]+:\t[0-9a-f ]+\t(.*)$/`.
///
/// PORT NOTE ON CRLF: JavaScript's `.` excludes the four line terminators and
/// `$` without `/m` only matches at end of input, so a line ending in `\r`
/// (which survives a `split("\n")`) fails the match entirely and is dropped.
/// That is reproduced here, not corrected -- objdump output on this toolchain
/// is LF, so the branch is unreachable in practice, but a Windows-built objdump
/// would silently disassemble to nothing on both sides identically.
pub fn disassembly_line(line: &str) -> Option<String> {
    let bytes = line.as_bytes();
    let mut index = 0;
    while index < bytes.len() && is_js_regex_space(bytes[index]) {
        index += 1;
    }
    if index == 0 {
        return None; // `\s+` needs at least one space.
    }
    let start = index;
    while index < bytes.len()
        && bytes[index].is_ascii_hexdigit()
        && !bytes[index].is_ascii_uppercase()
    {
        index += 1;
    }
    if index == start || bytes.get(index) != Some(&b':') {
        return None;
    }
    index += 1;
    if bytes.get(index) != Some(&b'\t') {
        return None;
    }
    index += 1;
    let start = index;
    while index < bytes.len()
        && (bytes[index] == b' '
            || (bytes[index].is_ascii_hexdigit() && !bytes[index].is_ascii_uppercase()))
    {
        index += 1;
    }
    if index == start || bytes.get(index) != Some(&b'\t') {
        return None;
    }
    index += 1;
    let tail = &line[index..];
    // `.` excludes CR/LF/LS/PS, and `$` demands end of input, so any of them in
    // the tail makes the whole match fail.
    if tail
        .chars()
        .any(|c| matches!(c, '\n' | '\r' | '\u{2028}' | '\u{2029}'))
    {
        return None;
    }
    Some(tail.trim().to_string())
}

// ---------------------------------------------------------------------------
// Options
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub struct Options {
    pub source: String,
    /// `Option` because `--rom` with no following argument leaves the field
    /// `undefined` in JavaScript rather than erroring.
    pub rom: Option<String>,
    pub pairs: bool,
    pub triples: bool,
    pub family_factorial: bool,
    /// `f64`, not `usize`: `Number(argv[++index])` yields `NaN` for a missing
    /// or non-numeric value and `1.5` for `"1.5"`, and the integer check below
    /// is what rejects them. Parsing straight to `usize` would reject different
    /// inputs than the TypeScript does.
    pub jobs: f64,
    pub top: f64,
    pub max_pairs: f64,
    pub max_triples: f64,
}

pub const USAGE: &str =
    "usage: mode-sweep <candidate.c> [--pairs] [--triples] [--family-factorial] [--jobs N] [--top N] [--max-pairs N] [--max-triples N] [--rom FILE]";

/// `-h`/`--help` was requested: the TypeScript prints usage and exits zero.
#[derive(Debug)]
pub enum OptionsOutcome {
    Parsed(Box<Options>),
    Help,
}

/// Port of `Number(x)` for the numeric flags, restricted to what a command line
/// can carry. Whitespace-only and empty strings coerce to zero, which the
/// integer check then rejects for being below one.
fn js_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(|c: char| {
        matches!(c, '\u{9}'..='\u{d}' | ' ' | '\u{a0}' | '\u{feff}' | '\u{1680}' | '\u{2000}'..='\u{200a}' | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}')
    });
    if trimmed.is_empty() {
        return 0.0;
    }
    if let Some(hex) = trimmed
        .strip_prefix("0x")
        .or_else(|| trimmed.strip_prefix("0X"))
    {
        return u64::from_str_radix(hex, 16)
            .map(|v| v as f64)
            .unwrap_or(f64::NAN);
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

/// `Number.isInteger(value) && value >= 1`.
///
/// Written as an explicit predicate rather than `!(value < 1.0)` because NaN
/// makes the negated form a different function.
fn is_positive_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value >= 1.0
}

/// Port of `optionsOf`. `hardware_concurrency` is the caller's
/// `navigator.hardwareConcurrency`; pass `None` for JavaScript's falsy default
/// of 8.
pub fn options_of(
    argv: &[String],
    root: &Path,
    hardware_concurrency: Option<u32>,
) -> Result<OptionsOutcome, String> {
    let concurrency = match hardware_concurrency {
        // `(navigator.hardwareConcurrency || 8)`: zero is falsy in JavaScript.
        Some(0) | None => 8i64,
        Some(value) => value as i64,
    };
    let mut options = Options {
        source: String::new(),
        rom: Some(root.join("roms/gs1-en.gba").to_string_lossy().into_owned()),
        pairs: false,
        triples: false,
        family_factorial: false,
        jobs: 1i64.max(10i64.min(concurrency - 2)) as f64,
        top: 16.0,
        max_pairs: 256.0,
        max_triples: 64.0,
    };
    let mut rest: Vec<String> = Vec::new();
    let mut index = 0usize;
    // `argv[++index]` past the end is `undefined`, not an error: `--rom` leaves
    // `rom` undefined and the numeric flags coerce to NaN and are rejected
    // below by the integer check.
    let next = |index: &mut usize| -> Option<String> {
        *index += 1;
        argv.get(*index).cloned()
    };
    while index < argv.len() {
        let argument = argv[index].as_str();
        match argument {
            "--rom" => options.rom = next(&mut index),
            "--pairs" => options.pairs = true,
            "--triples" => {
                options.triples = true;
                options.pairs = true;
            }
            "--family-factorial" => options.family_factorial = true,
            "--jobs" => options.jobs = next(&mut index).map_or(f64::NAN, |v| js_number(&v)),
            "--top" => options.top = next(&mut index).map_or(f64::NAN, |v| js_number(&v)),
            "--max-pairs" => {
                options.max_pairs = next(&mut index).map_or(f64::NAN, |v| js_number(&v))
            }
            "--max-triples" => {
                options.max_triples = next(&mut index).map_or(f64::NAN, |v| js_number(&v))
            }
            "-h" | "--help" => return Ok(OptionsOutcome::Help),
            other => rest.push(other.to_string()),
        }
        index += 1;
    }
    if rest.len() != 1 {
        return Err("usage: mode-sweep <candidate.c> [--pairs] [--triples]".to_string());
    }
    for (name, value) in [
        ("jobs", options.jobs),
        ("top", options.top),
        ("max-pairs", options.max_pairs),
        ("max-triples", options.max_triples),
    ] {
        if !is_positive_integer(value) {
            return Err(format!("{name} must be a positive integer"));
        }
    }
    options.source = rest[0].clone();
    Ok(OptionsOutcome::Parsed(Box::new(options)))
}

// ---------------------------------------------------------------------------
// Cache acceptance
// ---------------------------------------------------------------------------

/// Port of `acceptedCache`: a cached score is usable only when its key matches
/// and its shape is intact. `compiled` must be a boolean specifically -- a
/// truthy string would be accepted by a looser check and then mis-tallied.
pub fn accepted_cache(document: &Json, cache_key: &str) -> bool {
    let Json::Object(_) = document else {
        return false;
    };
    let key_matches = document.get("cache_key").and_then(Json::as_str) == Some(cache_key);
    let compiled_is_boolean = matches!(document.get("compiled"), Some(Json::Bool(_)));
    let config = document.get("config");
    let config_shaped = config.is_some_and(|config| {
        matches!(config.get("ids"), Some(Json::Array(_)))
            && matches!(config.get("flags"), Some(Json::Array(_)))
    });
    key_matches && compiled_is_boolean && config_shaped
}

// ---------------------------------------------------------------------------
// Self-test
// ---------------------------------------------------------------------------

/// Port of `selfTest`, plus the port's own invariants. Returns the number of
/// checks executed so the caller can enforce a floor: a self-test that silently
/// stops checking things is worse than no self-test.
pub fn self_test() -> Result<usize, String> {
    let mut checks = 0usize;
    let mut check = |ok: bool, message: &str| -> Result<(), String> {
        checks += 1;
        if ok {
            Ok(())
        } else {
            Err(message.to_string())
        }
    };

    let singles = single_configs();
    check(
        !singles.is_empty() && singles[0].flags.is_empty() && singles[0].compiler_family == ROUTED,
        "default configuration must be first",
    )?;
    let mut keys: Vec<String> = singles.iter().map(Config::key).collect();
    keys.sort();
    let unique = {
        keys.dedup();
        keys.len()
    };
    check(
        unique == singles.len(),
        "single configuration planning is not unique",
    )?;

    let mut explored: Vec<String> = Vec::new();
    for mode in modes() {
        for flag in mode.add_flags {
            if !explored.contains(&flag) {
                explored.push(flag);
            }
        }
    }
    let missing: Vec<String> = evidenced_routing_flags()
        .into_iter()
        .filter(|flag| !explored.contains(flag))
        .collect();
    check(
        missing.is_empty(),
        &format!(
            "routed compiler modes missing from explorer: {}",
            missing.join(", ")
        ),
    )?;

    let pairs = pair_configs(None);
    check(
        !pairs.iter().any(|config| {
            config.ids.iter().any(|id| id == "opt-o1") && config.ids.iter().any(|id| id == "opt-o2")
        }),
        "incompatible optimization levels were paired",
    )?;
    let json = |configs: &[Config]| {
        canonical_json(&Json::Array(configs.iter().map(Config::to_json).collect()))
    };
    check(
        json(&pair_configs(Some(17))) == json(&pair_configs(Some(17))),
        "pair planning is nondeterministic",
    )?;

    let seeds = vec![
        "sched-postreload-off".to_string(),
        "cse-gcse-off".to_string(),
    ];
    let triples = triple_configs(&seeds, Some(8));
    check(
        !triples.is_empty()
            && !triples
                .iter()
                .any(|config| config.ids.iter().filter(|id| seeds.contains(id)).count() < 2),
        "triple planning escaped its evidence seed",
    )?;

    let factorial = historical_family_factorial_configs();
    check(
        !factorial.is_empty()
            && !factorial.iter().any(|config| {
                config.ids.len() != 3
                    || config.compiler_family == ROUTED
                    || config.compiler_family == "gcc296"
            })
            && factorial.iter().any(|config| {
                config.compiler_family == "gcc2951"
                    && config.ids.iter().any(|id| id == "cse-gcse-off")
                    && config.ids.iter().any(|id| id == "reg-regmove-off")
            }),
        "historical family factorial planning differs",
    )?;

    let reference = [0x01u8, 0x20, 0x02, 0x21];
    let exact = classify(
        &reference,
        &reference,
        &[s("movs r0, #1")],
        &[s("movs r0, #1")],
    );
    check(
        exact.exact && exact.differing_halfwords == 0,
        "exact classification differs",
    )?;
    let registers = classify(
        &[0x01, 0x20],
        &[0x01, 0x21],
        &[s("movs r0, #1")],
        &[s("movs r1, #1")],
    );
    check(
        registers.register_allocation_proxy == 1 && registers.differing_halfwords == 1,
        "register classification differs",
    )?;
    let control_flow = classify(
        &[0x00, 0xe0],
        &[0x01, 0xe0],
        &[s("b.n 0x4")],
        &[s("b.n 0x6")],
    );
    check(
        control_flow.control_flow_proxy,
        "control-flow classification differs",
    )?;

    let flags_json = canonical_json(&Json::Array(vec![Json::String("-O1".into())]));
    let key1 = hash(&[b"source", b"reference", flags_json.as_bytes()]);
    let key2 = hash(&[b"source", b"reference", flags_json.as_bytes()]);
    let changed = hash(&[b"changed", b"reference", flags_json.as_bytes()]);
    check(
        key1 == key2 && key1 != changed,
        "cache keys are not deterministic/content-sensitive",
    )?;

    let cached = Json::Object(vec![
        (
            "config".into(),
            Config {
                ids: vec!["opt-o1".into()],
                flags: vec!["-O1".into()],
                remove_flags: Vec::new(),
                compiler_family: ROUTED.into(),
            }
            .to_json(),
        ),
        ("cache_key".into(), Json::String(key1.clone())),
        ("cached".into(), Json::Bool(false)),
        ("compiled".into(), Json::Bool(false)),
    ]);
    let malformed = Json::Object(vec![("cache_key".into(), Json::String(key1.clone()))]);
    check(
        accepted_cache(&cached, &key1)
            && !accepted_cache(&cached, &format!("{key1}stale"))
            && !accepted_cache(&malformed, &key1),
        "cache acceptance does not reject stale/malformed entries",
    )?;

    // Port-specific invariants. The collator is the one place where a plausible
    // Rust idiom (`str::cmp`) silently produces a different plan, so it is
    // checked here as well as in the unit tests.
    check(
        collate("a-b", "a+b") == std::cmp::Ordering::Less
            && "a-b".cmp("a+b") == std::cmp::Ordering::Greater,
        "collation no longer differs from byte order, which means it is wrong",
    )?;
    check(
        modes()
            .iter()
            .all(|mode| mode.id.chars().all(|c| COLLATED_ALPHABET.contains(c))),
        "a mode id uses a character the collation rank table was not calibrated for",
    )?;

    Ok(checks)
}

fn s(text: &str) -> String {
    text.to_string()
}

/// The number of checks `self_test` must execute. A future edit that drops one
/// fails instead of quietly passing a smaller suite.
pub const SELF_TEST_CHECK_FLOOR: usize = 14;

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes_and_meets_its_check_floor() {
        let checks = self_test().expect("self-test");
        assert_eq!(
            checks, SELF_TEST_CHECK_FLOOR,
            "self-test check count changed; update the floor deliberately"
        );
    }

    #[test]
    fn mode_table_has_the_expected_shape() {
        // Counts measured against Bun:
        //   FORK_MODES.length === 89, STOCK_SWITCHES.length === 21,
        //   MODES.length === 135.
        assert_eq!(FORK_MODES.len(), 89);
        assert_eq!(STOCK_SWITCHES.len(), 21);
        assert_eq!(modes().len(), 135);
        assert_eq!(
            modes().len(),
            DECLARED.len() + FORK_MODES.len() + AGBCC_FLAGS.len()
        );
    }

    #[test]
    fn fork_modes_are_unique_and_prefixed() {
        let mut seen: Vec<&str> = Vec::new();
        for flag in FORK_MODES {
            assert!(flag.starts_with("-f") || flag.starts_with("-m"), "{flag}");
            assert!(!seen.contains(flag), "duplicate fork mode {flag}");
            seen.push(flag);
        }
    }

    #[test]
    fn fork_mode_ids_are_ascii() {
        // `flag.slice(2)` is a UTF-16 slice in JavaScript and a byte slice
        // here; they agree only while every flag is ASCII.
        for flag in FORK_MODES.iter().chain(AGBCC_FLAGS.iter()) {
            assert!(flag.is_ascii(), "{flag}");
        }
    }

    #[test]
    fn collation_matches_measured_bun_order() {
        // Measured: [...set].sort((a,b)=>a.localeCompare(b)).join("")
        let mut characters: Vec<String> =
            COLLATED_ALPHABET.chars().map(|c| c.to_string()).collect();
        characters.reverse();
        characters.sort_by(|a, b| collate(a, b));
        assert_eq!(characters.concat(), COLLATED_ALPHABET);

        // Measured: ["a-b","ab","a+b","a=b","a0b"].sort(localeCompare)
        let mut strings = vec![s("a-b"), s("ab"), s("a+b"), s("a=b"), s("a0b")];
        strings.sort_by(|a, b| collate(a, b));
        assert_eq!(
            strings,
            vec![s("a-b"), s("a+b"), s("a=b"), s("a0b"), s("ab")]
        );

        // Measured: ["ab","a"].sort(localeCompare) === ["a","ab"]
        assert_eq!(collate("a", "ab"), std::cmp::Ordering::Less);
    }

    #[test]
    fn case_is_tertiary_not_primary() {
        // The pair a single-pass interleaved `aAbB` table gets backwards, and
        // the reason this comparator makes two passes. Measured in Bun.
        assert_eq!(collate("A", "a-b"), std::cmp::Ordering::Less);
        assert_eq!(collate("a", "A"), std::cmp::Ordering::Less);
        assert_eq!(collate("ab", "aB"), std::cmp::Ordering::Less);
        assert_eq!(collate("aB", "ab"), std::cmp::Ordering::Greater);
        // Primary still outranks case: a `c` beats a case difference on `b`.
        assert_eq!(collate("aB", "ac"), std::cmp::Ordering::Less);
        // Punctuation is not variable-weighted; it sorts below every letter.
        assert_eq!(collate("a-c", "ab"), std::cmp::Ordering::Less);
    }

    #[test]
    fn mode_ids_use_only_the_collated_alphabet() {
        for mode in modes() {
            for character in mode.id.chars() {
                assert!(
                    COLLATED_ALPHABET.contains(character),
                    "mode id {} uses {character:?}, outside the calibrated collation alphabet",
                    mode.id
                );
            }
        }
    }

    #[test]
    fn signature_paths_are_native_and_complete() {
        let fake_root = Path::new("/repo");
        let paths = signature_source_paths(fake_root);
        assert_eq!(paths.len(), 3);
        assert!(paths
            .iter()
            .all(|path| path.extension().and_then(|e| e.to_str()) == Some("rs")));
        for path in signature_source_paths(&root()) {
            assert!(path.exists(), "{} is missing", path.display());
        }
    }

    #[test]
    fn classify_counts_halfwords_and_odd_tails() {
        let evidence = classify(&[1, 2, 3], &[1, 2], &[], &[]);
        // ceil(3/2) - ceil(2/2) = 1 length halfword, and the shared 2-byte
        // prefix matches, so exactly one differing halfword.
        assert_eq!(evidence.differing_halfwords, 1);
        assert_eq!(evidence.size_delta, 1);
        assert!(!evidence.exact_size);
    }

    #[test]
    fn normalized_registers_prefers_the_two_digit_alternative() {
        assert_eq!(normalized_registers("mov r10, r1"), "mov REG, REG");
        assert_eq!(normalized_registers("mov R10, SP"), "mov REG, REG");
        // r16 is not a register: r1 matches, 6 is left, but \b then fails
        // because 6 is a word character -- so nothing is replaced.
        assert_eq!(normalized_registers("mov r16, r1"), "mov r16, REG");
        // A word character before the token blocks \b.
        assert_eq!(normalized_registers("xr1"), "xr1");
        assert_eq!(normalized_registers("push {lr}"), "push {REG}");
    }

    #[test]
    fn mnemonic_of_leading_space_is_empty() {
        assert_eq!(mnemonic(" movs r0, #1"), "");
        assert_eq!(mnemonic("movs r0, #1"), "movs");
        assert_eq!(mnemonic(""), "");
    }

    #[test]
    fn literal_loads_need_a_whole_pc_word() {
        assert!(is_literal_load("ldr r0, [pc, #16]"));
        assert!(!is_literal_load("ldr r0, [r1, #16]"));
        assert!(!is_literal_load("ldrpc"));
        assert!(!is_literal_load("str r0, [pc, #16]"));
        // No `i` flag on either regex in the TypeScript.
        assert!(!is_literal_load("LDR R0, [PC, #16]"));
        assert!(!is_literal_load("ldr r0, [PC, #16]"));
    }

    #[test]
    fn disassembly_line_parses_objdump_and_rejects_crlf() {
        assert_eq!(
            disassembly_line("   4:\t2001      \tmovs\tr0, #1").as_deref(),
            Some("movs\tr0, #1")
        );
        // No leading whitespace: `\s+` fails.
        assert_eq!(disassembly_line("4:\t2001      \tmovs\tr0, #1"), None);
        // CR survives split("\n"); `.` excludes it and `$` demands end of
        // input, so the whole line is dropped.
        assert_eq!(disassembly_line("   4:\t2001      \tmovs\tr0, #1\r"), None);
    }

    #[test]
    fn options_reject_non_integers_the_way_number_does() {
        let root = Path::new("/repo");
        let parse = |args: &[&str]| {
            options_of(
                &args.iter().map(|a| a.to_string()).collect::<Vec<_>>(),
                root,
                Some(18),
            )
        };
        let OptionsOutcome::Parsed(options) = parse(&["a.c"]).unwrap() else {
            panic!("expected options");
        };
        assert_eq!(options.source, "a.c");
        assert_eq!(options.jobs, 10.0); // min(10, 18 - 2)
        assert_eq!(options.top, 16.0);
        assert!(!options.pairs && !options.triples);
        assert_eq!(options.rom.as_deref(), Some("/repo/roms/gs1-en.gba"));

        // `--triples` implies `--pairs`.
        let OptionsOutcome::Parsed(options) = parse(&["a.c", "--triples"]).unwrap() else {
            panic!("expected options");
        };
        assert!(options.pairs && options.triples);

        // parseInt would accept "1.5"; Number does not, and Number.isInteger
        // rejects it.
        assert_eq!(
            parse(&["a.c", "--jobs", "1.5"]).unwrap_err(),
            "jobs must be a positive integer"
        );
        assert_eq!(
            parse(&["a.c", "--jobs", "x"]).unwrap_err(),
            "jobs must be a positive integer"
        );
        // A trailing `--jobs` consumes `undefined` -> NaN.
        assert_eq!(
            parse(&["a.c", "--jobs"]).unwrap_err(),
            "jobs must be a positive integer"
        );
        assert_eq!(
            parse(&["a.c", "--top", "0"]).unwrap_err(),
            "top must be a positive integer"
        );
        assert!(parse(&[]).is_err());
        assert!(parse(&["a.c", "b.c"]).is_err());
        assert!(matches!(parse(&["--help"]).unwrap(), OptionsOutcome::Help));
        // A trailing `--rom` leaves the field undefined rather than erroring.
        let OptionsOutcome::Parsed(options) = parse(&["a.c", "--rom"]).unwrap() else {
            panic!("expected options");
        };
        assert_eq!(options.rom, None);
    }

    #[test]
    fn low_concurrency_still_yields_one_job() {
        let root = Path::new("/repo");
        for (reported, expected) in [
            (Some(1u32), 1.0),
            (Some(2), 1.0),
            (Some(4), 2.0),
            (None, 6.0),
            (Some(0), 6.0),
        ] {
            let OptionsOutcome::Parsed(options) = options_of(&[s("a.c")], root, reported).unwrap()
            else {
                panic!("expected options");
            };
            assert_eq!(options.jobs, expected, "hardwareConcurrency = {reported:?}");
        }
    }

    #[test]
    fn compatibility_rejects_the_documented_conflicts() {
        let table = modes();
        let by_id = |id: &str| {
            table
                .iter()
                .find(|m| m.id == id)
                .unwrap_or_else(|| panic!("{id}"))
        };
        // Two exclusive modes in one family.
        assert!(!compatible(&[by_id("opt-o1"), by_id("opt-o2")]));
        // old-agbcc excludes scheduler modes.
        assert!(!compatible(&[
            by_id("compiler-old-agbcc"),
            by_id("sched-postreload-off")
        ]));
        // old-agbcc excludes backend modes that are not its own.
        assert!(!compatible(&[
            by_id("compiler-old-agbcc"),
            by_id("thumb-leaf-no-lr")
        ]));
        // ... but accepts its own.
        assert!(compatible(&[
            by_id("compiler-old-agbcc"),
            by_id("agbcc-literal-before-shift")
        ]));
        // supportedCompilerFamilies gates on the selected compiler.
        assert!(!compatible(&[
            by_id("compiler-gcc2951"),
            by_id("reg-peephole2-off")
        ]));
        assert!(compatible(&[
            by_id("compiler-gcc296"),
            by_id("reg-peephole2-off")
        ]));
    }

    #[test]
    fn ranked_pairs_fall_through_nan_to_the_collation_tiebreak() {
        // Every id unevidenced: quality is Infinity on both sides, the
        // difference is NaN, and JavaScript's `||` treats NaN as falsy, so the
        // order is purely the collation tiebreak. A `partial_cmp` port would
        // leave the input order instead.
        let ranks: Vec<(String, Rank)> = modes()
            .iter()
            .map(|mode| {
                (
                    mode.id.clone(),
                    Rank {
                        exact: false,
                        floor: f64::INFINITY,
                    },
                )
            })
            .collect();
        let ranked = ranked_pair_configs(&ranks, 32);
        let plain = pair_configs(Some(32));
        assert_eq!(
            ranked, plain,
            "NaN quality must fall through to collation order"
        );
        assert!(!ranked.is_empty());
    }

    #[test]
    fn ranked_pairs_put_exact_singles_first() {
        let table = modes();
        let ranks: Vec<(String, Rank)> = table
            .iter()
            .map(|mode| {
                (
                    mode.id.clone(),
                    Rank {
                        exact: mode.id == "cse-gcse-off",
                        floor: 100.0,
                    },
                )
            })
            .collect();
        let ranked = ranked_pair_configs(&ranks, 8);
        assert!(
            ranked
                .iter()
                .take(4)
                .all(|c| c.ids.iter().any(|id| id == "cse-gcse-off")),
            "an exact single must dominate the pair ranking"
        );
    }

    #[test]
    fn planning_is_non_degenerate_and_deterministic() {
        let pairs = pair_configs(None);
        assert!(pairs.len() > 1000, "pair plan collapsed to {}", pairs.len());
        let mut keys: Vec<String> = pairs.iter().map(Config::key).collect();
        let before = keys.len();
        keys.sort();
        keys.dedup();
        assert_eq!(keys.len(), before, "pair plan contains duplicates");
        assert_eq!(pairs, pair_configs(None));

        let factorial = historical_family_factorial_configs();
        assert!(
            factorial.len() > 100,
            "factorial plan collapsed to {}",
            factorial.len()
        );
    }

    #[test]
    fn evidenced_routing_flags_are_covered_and_non_empty() {
        let flags = evidenced_routing_flags();
        assert!(
            flags.len() >= 10,
            "the routed-flag walk found only {} flags; a broken walk would make the coverage contract vacuous",
            flags.len()
        );
        let explored: Vec<String> = modes().into_iter().flat_map(|m| m.add_flags).collect();
        for flag in &flags {
            assert!(
                explored.contains(flag),
                "routed flag {flag} is unreachable by any sweep"
            );
        }
    }

    #[test]
    fn hash_separates_its_parts() {
        assert_ne!(hash(&[b"ab", b"c"]), hash(&[b"a", b"bc"]));
        assert_eq!(hash(&[b"a"]), hash(&[b"a"]));
    }
}
