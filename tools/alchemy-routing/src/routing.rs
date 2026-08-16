//! Native compiler-routing surface used by `verify` through
//! `source_to_assembly_plan`.
//! Ported here: `ROOT`, `BUNDLE`/`DRIVER`/`GS2_*`/`AGBCC_*` path constants,
//! `CFLAGS`/`GS2_CFLAGS`/`AGBCC_CFLAGS`, `bundleForTarget`, `driverForTarget`,
//! `cflagsForTarget`, `sourceStem`, `overlayStem`, `sourceKey`,
//! `cflagsForSource`, `cflagsForTargetSource`, `usesAgbccCompiler`, and every
//! routing `Set`/`Map` those read. The tables live in `routing_data.rs`.
//!
//! This module used to live inside the `verify` crate. It now owns the routing
//! implementation directly; the tables in `routing_data` are the sole source
//! of routing decisions.

use std::collections::{HashMap, HashSet};
use std::path::{Component, Path, PathBuf};
use std::sync::OnceLock;

use crate::routing_data::*;

/// Repository root: `<crate>/../..`.
pub fn root() -> &'static Path {
    static ROOT: OnceLock<PathBuf> = OnceLock::new();
    ROOT.get_or_init(|| {
        Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .expect("crate dir has a parent")
            .parent()
            .expect("tools has a parent")
            .to_path_buf()
    })
}

pub fn bundle() -> PathBuf {
    root().join("alchemy-gcc").join("dist")
}
pub fn driver() -> PathBuf {
    bundle().join("xgcc")
}
pub fn gs2_bundle() -> PathBuf {
    bundle().join("gs2")
}
pub fn gs2_driver() -> PathBuf {
    gs2_bundle().join("xgcc")
}
pub fn agbcc_driver() -> PathBuf {
    bundle().join("agbcc").join("old_agbcc")
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerTarget {
    Gs1,
    Gs2,
}

impl CompilerTarget {
    pub fn as_str(self) -> &'static str {
        match self {
            CompilerTarget::Gs1 => "gs1",
            CompilerTarget::Gs2 => "gs2",
        }
    }
}

pub fn bundle_for_target(target: CompilerTarget) -> PathBuf {
    match target {
        CompilerTarget::Gs1 => bundle(),
        CompilerTarget::Gs2 => gs2_bundle(),
    }
}

pub fn driver_for_target(target: CompilerTarget) -> PathBuf {
    match target {
        CompilerTarget::Gs1 => driver(),
        CompilerTarget::Gs2 => gs2_driver(),
    }
}

fn include_flag() -> String {
    format!("-I{}", root().join("include").display())
}

pub fn cflags() -> Vec<String> {
    [
        "-O2",
        "-mthumb",
        "-mthumb-interwork",
        "-mcpu=arm7tdmi",
        "-fno-builtin",
        "-nostdinc",
        "-ffreestanding",
        "-fcall-used-r4",
    ]
    .iter()
    .map(|s| (*s).to_string())
    .chain(std::iter::once(include_flag()))
    .collect()
}

pub fn gs2_cflags() -> Vec<String> {
    let mut flags = cflags();
    flags.push("-ffixed-r7".to_string());
    flags
}

pub fn agbcc_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}

pub fn cflags_for_target(target: CompilerTarget) -> Vec<String> {
    match target {
        CompilerTarget::Gs1 => cflags(),
        CompilerTarget::Gs2 => gs2_cflags(),
    }
}

// ---------------------------------------------------------------------------
// Key derivation (node:path `basename`/`extname`/`relative` semantics).
// ---------------------------------------------------------------------------

/// `basename(source, extname(source))` for POSIX paths.
pub fn source_stem(source: &str) -> String {
    let base = source.rsplit('/').next().unwrap_or(source);
    // node's extname(): the last '.' that is not the first character of the
    // basename, and not a trailing '.' at position 0.
    match base.rfind('.') {
        Some(index) if index > 0 => base[..index].to_string(),
        _ => base.to_string(),
    }
}

fn is_hex8(value: &str) -> bool {
    value.len() == 8
        && value
            .bytes()
            .all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
}

/// `overlayStem`: an overlay row routes by its bare address, so a candidate
/// verified as `<addr>.c` and the installed `<overlay>_c_<addr>.c` agree.
pub fn overlay_stem(source: &str) -> String {
    let stem = source_stem(source);
    if is_hex8(&stem) {
        return stem;
    }
    if let Some(index) = stem.rfind("_c_") {
        let tail = &stem[index + 3..];
        if is_hex8(tail) {
            return tail.to_string();
        }
    }
    stem
}

fn normalize(path: &Path) -> PathBuf {
    let mut out = PathBuf::new();
    for component in path.components() {
        match component {
            Component::CurDir => {}
            Component::ParentDir => {
                if !out.pop() {
                    out.push("..");
                }
            }
            other => out.push(other.as_os_str()),
        }
    }
    out
}

/// `sourceKey`: `relative(ROOT, resolve(ROOT, source))` with `/` separators.
pub fn source_key(source: &str) -> String {
    let resolved = if Path::new(source).is_absolute() {
        normalize(Path::new(source))
    } else {
        normalize(&root().join(source))
    };
    let base = normalize(root());
    let resolved_parts: Vec<_> = resolved.components().collect();
    let base_parts: Vec<_> = base.components().collect();
    let shared = resolved_parts
        .iter()
        .zip(base_parts.iter())
        .take_while(|(a, b)| a == b)
        .count();
    let mut parts: Vec<String> = vec!["..".to_string(); base_parts.len() - shared];
    for component in &resolved_parts[shared..] {
        parts.push(component.as_os_str().to_string_lossy().into_owned());
    }
    parts.join("/")
}

// ---------------------------------------------------------------------------
// Routing tables.
// ---------------------------------------------------------------------------

fn set(table: &'static [&'static str]) -> &'static HashSet<&'static str> {
    // One cache keyed by the table's address; the tables are `static`, so the
    // pointer identifies the set.
    static CACHE: OnceLock<std::sync::Mutex<HashMap<usize, &'static HashSet<&'static str>>>> =
        OnceLock::new();
    let cache = CACHE.get_or_init(|| std::sync::Mutex::new(HashMap::new()));
    let key = table.as_ptr() as usize;
    let mut guard = cache.lock().expect("routing cache is not poisoned");
    guard.entry(key).or_insert_with(|| {
        let built: HashSet<&'static str> = table.iter().copied().collect();
        Box::leak(Box::new(built))
    })
}

fn has(table: &'static [&'static str], value: &str) -> bool {
    set(table).contains(value)
}

fn lookup(table: &'static [(&'static str, &'static str)], value: &str) -> Option<&'static str> {
    table
        .iter()
        .find(|(key, _)| *key == value)
        .map(|(_, entry)| *entry)
}

/// `cflagsForSource`. The order of the appended flags is load-bearing: later
/// options win in the driver, and the routed command line for already-verified
/// regions must not be rewritten.
pub fn cflags_for_source(source: &str) -> Vec<String> {
    let stem = overlay_stem(source);
    let stem = stem.as_str();
    let key = source_key(source);
    let key = key.as_str();

    // pret shape. One base flag set, a small number of per-file overrides, and
    // every override a STOCK gcc 2.96 option.
    //
    // pokeemerald's Makefile is the model: one CFLAGS, about eight per-file
    // lines, each either a compiler selection or a stock flag. agbcc is a fixed
    // compiler and nobody adds an option to it per function. The 120 options
    // this fork invented are gone from routing; see
    // tools/route-dump/data/invented-flags.txt for the list and AGENTS.md for
    // the rule that forbids adding another.
    //
    // Owners that were reproducing only because an invented option was routed
    // to them no longer reproduce. That is the point: the difference is back in
    // the source, where it can be found and fixed, instead of hidden behind a
    // switch.
    let mut out: Vec<String> =
        if has(NO_INTERWORK_SOURCES, stem) || has(NO_INTERWORK_OVERLAY_SOURCES, key) {
            cflags()
                .into_iter()
                .filter(|f| f != "-mthumb-interwork")
                .collect()
        } else {
            cflags()
        };

    macro_rules! push {
        ($flags:expr) => {
            out.extend($flags.iter().map(|f| (*f).to_string()))
        };
    }

    if has(FIXED_R3_SOURCES, stem) {
        push!(&["-ffixed-r3"]);
    }
    if has(OPTIMIZE_O1_SOURCES, stem) {
        push!(&["-O1"]);
    }
    if has(OPTIMIZE_OS_SOURCES, stem) {
        push!(&["-Os"]);
    }
    if has(OPTIMIZE_O3_OVERLAY_SOURCES, key) {
        push!(&["-O3"]);
    }
    if has(UNSCHEDULED_SOURCES, stem) {
        push!(&["-fno-schedule-insns", "-fno-schedule-insns2"]);
    }
    if has(UNSCHEDULED_OVERLAY_SOURCES, key) {
        push!(&["-fno-schedule-insns2"]);
    }
    if has(NO_CSE_FOLLOW_SOURCES, stem) {
        push!(&["-fno-cse-follow-jumps"]);
    }
    if has(NO_RERUN_CSE_AFTER_LOOP_SOURCES, stem) {
        push!(&["-fno-rerun-cse-after-loop"]);
    }
    if has(NO_GCSE_SOURCES, stem) {
        push!(&["-fno-gcse"]);
    }
    if has(NO_GCSE_OVERLAY_SOURCES, key) {
        push!(&["-fno-gcse"]);
    }
    if has(NO_EXPENSIVE_SOURCES, stem) {
        push!(&["-fno-expensive-optimizations"]);
    }
    if has(NO_STRENGTH_REDUCE_SOURCES, stem) {
        push!(&["-fno-strength-reduce"]);
    }
    if has(NO_REGMOVE_SOURCES, stem) {
        push!(&["-fno-regmove"]);
    }
    if has(OPTIMIZE_O1_OVERLAY_SOURCES, key) {
        push!(&["-O1"]);
    }
    if has(NO_OPTIMIZE_SIBLING_CALLS_SOURCES, stem) {
        push!(&["-fno-optimize-sibling-calls"]);
    }
    if has(SCHED2_OFF_THUMB_SOURCES, stem) {
        push!(&["-fno-schedule-insns2"]);
    }
    if has(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES, key) {
        push!(&["-fno-rerun-cse-after-loop"]);
    }
    if has(NO_THREAD_JUMPS_OVERLAY_SOURCES, key) {
        push!(&["-fno-thread-jumps"]);
    }
    if has(NO_EXPENSIVE_OVERLAY_SOURCES, key) {
        push!(&["-fno-expensive-optimizations"]);
    }
    if has(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES, key) {
        push!(&["-fno-cse-follow-jumps", "-fno-cse-skip-blocks"]);
    }
    if has(NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES, key) {
        push!(&["-fno-cse-skip-blocks"]);
    }
    if has(NO_STRICT_ALIASING_OVERLAY_SOURCES, key) {
        push!(&["-fno-strict-aliasing"]);
    }
    if has(FIXED_R7_OVERLAY_SOURCES, key) {
        push!(&["-ffixed-r7"]);
    }

    out
}

/// `usesAgbccCompiler`.
pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    target == CompilerTarget::Gs1 && has(AGBCC_SOURCES, &source_stem(source))
}

/// `cflagsForTargetSource`.
pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    let stem = source_stem(source);
    if target == CompilerTarget::Gs1 && has(AGBCC_SOURCES, &stem) {
        let mut out = agbcc_cflags();
        if has(AGBCC_OPTIMIZE_O1_SOURCES, &stem) {
            out.push("-O1".to_string());
        }
        if has(AGBCC_NO_EXPENSIVE_SOURCES, &stem) {
            out.push("-fno-expensive-optimizations".to_string());
        }
        if has(AGBCC_NO_GCSE_SOURCES, &stem) {
            out.push("-fno-gcse".to_string());
        }
        if has(AGBCC_NO_REGMOVE_SOURCES, &stem) {
            out.push("-fno-regmove".to_string());
        }
        if has(AGBCC_LITERAL_BEFORE_SHIFT_SOURCES, &stem) {
            out.push("-mliteral-before-shift".to_string());
        }
        if has(AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES, &stem) {
            out.push("-mcommutative-copy-constant".to_string());
        }
        if has(AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES, &stem) {
            out.push("-mprologue-next-high-reg".to_string());
        }
        if has(AGBCC_TRACK_NARROW_VALUE_R1_SOURCES, &stem) {
            out.push("-mtrack-narrow-value-r1".to_string());
        }
        if has(AGBCC_COMPARE_ONLY_AND_TST_SOURCES, &stem) {
            out.push("-mcompare-only-and-tst".to_string());
        }
        return out;
    }
    match target {
        CompilerTarget::Gs1 => cflags_for_source(source),
        CompilerTarget::Gs2 => gs2_cflags(),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn stems_and_keys() {
        assert_eq!(source_stem("/a/b/080fb6ec.c"), "080fb6ec");
        assert_eq!(source_stem("/a/b/noext"), "noext");
        assert_eq!(source_stem("/a/b/.hidden"), ".hidden");
        assert_eq!(overlay_stem("/a/b/080fb6ec.c"), "080fb6ec");
        assert_eq!(
            overlay_stem("/x/exact/resource_3a7_c_02001554.c"),
            "02001554"
        );
        assert_eq!(overlay_stem("/x/plain_name.c"), "plain_name");
        assert_eq!(source_key("exact/a.c"), "exact/a.c");
        assert_eq!(
            source_key(root().join("exact/a.c").to_str().unwrap()),
            "exact/a.c"
        );
    }

    #[test]
    fn baseline_flags_match_cflags() {
        // An address not in any routing set gets the plain baseline.
        assert_eq!(cflags_for_source("/tmp/0deadbee.c"), cflags());
    }

    #[test]
    fn interwork_is_stripped_for_the_routed_family() {
        let flags = cflags_for_source("/tmp/0200142c.c");
        assert!(!flags.iter().any(|f| f == "-mthumb-interwork"));
        assert!(flags.iter().any(|f| f == "-mthumb"));
    }

    #[test]
    fn routed_extra_flags_append_after_the_baseline() {
        let flags = cflags_for_source("/tmp/080049e8.c");
        let o2 = flags.iter().position(|f| f == "-O2").expect("-O2 baseline");
        let o1 = flags.iter().position(|f| f == "-O1").expect("routed -O1");
        assert!(o1 > o2, "the routed -O1 must come after the baseline -O2");
        assert_eq!(flags[..o2 + 1], cflags()[..o2 + 1]);
    }

    #[test]
    fn agbcc_sources_route_to_the_agbcc_flag_set() {
        assert!(uses_agbcc_compiler(CompilerTarget::Gs1, "/tmp/080fa514.c"));
        assert!(!uses_agbcc_compiler(CompilerTarget::Gs2, "/tmp/080fa514.c"));
        let flags = cflags_for_target_source(CompilerTarget::Gs1, "/tmp/080fa514.c");
        assert!(flags.starts_with(&agbcc_cflags()));
        assert!(flags.iter().any(|f| f == "-mcommutative-copy-constant"));
    }

    #[test]
    fn gs2_ignores_gs1_routing() {
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Gs2, "/tmp/080049e8.c"),
            gs2_cflags()
        );
    }

    fn assert_flag(source: &str, flag: &str) {
        let flags = cflags_for_source(source);
        assert!(
            flags.iter().any(|candidate| candidate == flag),
            "{source} should route {flag}, got {flags:?}"
        );
    }

    fn assert_no_flag(source: &str, flag: &str) {
        let flags = cflags_for_source(source);
        assert!(
            flags.iter().all(|candidate| candidate != flag),
            "{source} should not route {flag}, got {flags:?}"
        );
    }

    #[test]
    fn resource_3bb_polled_load_route_is_owner_specific() {
        assert_flag(
            "exact/resource_3bb_c_02000970.c",
            "-fno-hoist-volatile-address",
        );
        assert_no_flag(
            "exact/resource_3bb_c_02000950.c",
            "-fno-hoist-volatile-address",
        );
    }

    #[test]
    fn resource_373_scheduler_flags_cover_semantic_and_exact_paths() {
        let paired_routes = [
            (
                "-fthumb-pool-long-call-arg0",
                "semantic/resource_373_c_02002f14.c",
                "exact/resource_373_c_02002f14.c",
            ),
            (
                "-fthumb-order-zero-arg1-before-nonzero-arg0",
                "semantic/resource_373_c_02002f14.c",
                "exact/resource_373_c_02002f14.c",
            ),
            (
                "-fthumb-store-before-pool-load",
                "semantic/resource_373_c_02002a54.c",
                "exact/resource_373_c_02002a54.c",
            ),
            (
                "-fthumb-order-8-0-20-args",
                "semantic/resource_373_c_02002cb0.c",
                "exact/resource_373_c_02002cb0.c",
            ),
            (
                "-fthumb-zero-r1-before-r0-load-after-8-2",
                "semantic/resource_373_c_02002cb0.c",
                "exact/resource_373_c_02002cb0.c",
            ),
        ];

        for (flag, semantic, exact) in paired_routes {
            assert_flag(semantic, flag);
            assert_flag(exact, flag);
        }

        // These neighboring owners are deliberately not part of the 373
        // routes. Keep the assertions close to the positive paths so a broad
        // stem-based match cannot silently admit them later.
        assert_no_flag(
            "semantic/resource_373_c_02002f15.c",
            "-fthumb-pool-long-call-arg0",
        );
        assert_no_flag(
            "semantic/resource_373_c_02002a55.c",
            "-fthumb-store-before-pool-load",
        );
        assert_no_flag(
            "semantic/resource_373_c_02002cb1.c",
            "-fthumb-order-8-0-20-args",
        );
    }

    #[test]
    fn main_image_080bbb0c_pool_r1_lsl4_route_is_path_specific() {
        assert_flag("semantic/080bbb0c.c", "-fthumb-pool-r1-lsl4");
        assert_flag("exact/080bbb0c.c", "-fthumb-pool-r1-lsl4");
        // The callee and the nearest neighboring owners never inherit the
        // repair; the route names exactly one main-image function.
        assert_no_flag("exact/080bbabc.c", "-fthumb-pool-r1-lsl4");
        assert_no_flag("semantic/080bb938.c", "-fthumb-pool-r1-lsl4");
        assert_no_flag(
            "semantic/resource_373_c_02003fb0.c",
            "-fthumb-pool-r1-lsl4",
        );
    }

    #[test]
    fn resource_39a_byte_arg_sheet_route_is_path_specific() {
        assert_flag(
            "semantic/resource_39a_c_02001b1c.c",
            "-fthumb-call-arg12-before-arg0",
        );
        assert_flag(
            "exact/resource_39a_c_02001b1c.c",
            "-fthumb-call-arg12-before-arg0",
        );
        assert_no_flag(
            "semantic/resource_39a_c_02001b20.c",
            "-fthumb-call-arg12-before-arg0",
        );
    }

    #[test]
    fn resource_373_stack_and_register_routes_preserve_path_specificity() {
        assert_flag(
            "semantic/resource_373_c_020008c0.c",
            "-fthumb-stack-slot-before-table",
        );
        assert_no_flag(
            "exact/resource_373_c_020008c0.c",
            "-fthumb-stack-slot-before-table",
        );
        assert_no_flag(
            "semantic/resource_373_c_020008c1.c",
            "-fthumb-stack-slot-before-table",
        );

        for source in [
            "semantic/resource_373_c_020008c0.c",
            "semantic/resource_373_c_0200564c.c",
        ] {
            assert_flag(source, "-mcallee-reg-order=0132");
        }
        assert_no_flag("exact/resource_373_c_020008c0.c", "-mcallee-reg-order=0132");
        assert_no_flag("exact/resource_373_c_0200564c.c", "-mcallee-reg-order=0132");

        assert_flag(
            "semantic/resource_373_c_0200564c.c",
            "-mhigh-reg-order=1203",
        );
        assert_no_flag("exact/resource_373_c_0200564c.c", "-mhigh-reg-order=1203");

        assert_flag(
            "semantic/resource_373_c_0200564c.c",
            "-fno-sched-depend-count",
        );
        assert_no_flag("exact/resource_373_c_0200564c.c", "-fno-sched-depend-count");
        assert_no_flag(
            "semantic/resource_373_c_0200565c.c",
            "-fno-sched-depend-count",
        );
    }

    #[test]
    fn resource_36f_portrait_dma_route_is_owner_specific() {
        for source in [
            "semantic/resource_36f_c_020001c0.c",
            "exact/resource_36f_c_020001c0.c",
        ] {
            for flag in [
                "-mgrouped-dma-store",
                "-fno-thumb-contiguous-immediate",
                "-fno-cse-pool-immediate",
                "-fthumb-sink-group-pool-loads",
                "-fthumb-pool-load-base-first",
            ] {
                assert_flag(source, flag);
            }
        }

        for flag in [
            "-mgrouped-dma-store",
            "-fno-thumb-contiguous-immediate",
            "-fno-cse-pool-immediate",
            "-fthumb-sink-group-pool-loads",
            "-fthumb-pool-load-base-first",
        ] {
            assert_no_flag("semantic/resource_36f_c_020001c1.c", flag);
        }
    }

    #[test]
    fn resource_3b0_root_routes_are_owner_specific() {
        for source in [
            "semantic/resource_3b0_c_020000c0.c",
            "exact/resource_3b0_c_020000c0.c",
        ] {
            assert_flag(source, "-fthumb-hi-immediate");
            assert_no_flag(source, "-fno-cse-two-insn-immediate");
        }

        for source in [
            "semantic/resource_3b0_c_02000240.c",
            "exact/resource_3b0_c_02000240.c",
        ] {
            assert_flag(source, "-fthumb-hi-immediate");
            assert_flag(source, "-fno-cse-two-insn-immediate");
        }

        for flag in [
            "-fthumb-hi-immediate",
            "-fno-cse-two-insn-immediate",
        ] {
            assert_no_flag("semantic/resource_3b0_c_02000241.c", flag);
        }
    }

    #[test]
    fn resource_392_query_copy_route_is_owner_specific() {
        for source in [
            "semantic/resource_392_c_020009f8.c",
            "exact/resource_392_c_020009f8.c",
        ] {
            assert_flag(source, "-fthumb-blockmove-dest-before-source");
        }
        assert_no_flag(
            "semantic/resource_392_c_020009f9.c",
            "-fthumb-blockmove-dest-before-source",
        );
    }

    #[test]
    fn resource_392_palette_twins_share_narrow_routes() {
        for source in [
            "semantic/resource_392_c_02000bcc.c",
            "exact/resource_392_c_02000bcc.c",
            "semantic/resource_393_c_02000d9c.c",
            "exact/resource_393_c_02000d9c.c",
            "semantic/resource_394_c_02000f74.c",
            "exact/resource_394_c_02000f74.c",
        ] {
            for flag in [
                "-fthumb-group-control-last",
                "-fthumb-group-control-rematerialize",
                "-mthumb-load-latency-one",
                "-mgrouped-dma-store",
            ] {
                assert_flag(source, flag);
            }
        }

        assert_no_flag(
            "exact/resource_37a_c_02000d9c.c",
            "-fthumb-group-control-rematerialize",
        );
    }

    #[test]
    fn resource_3c2_dialogue_routes_are_path_specific() {
        for source in [
            "semantic/resource_3c2_c_0200006c.c",
            "exact/resource_3c2_c_0200006c.c",
            "semantic/resource_3c7_c_020000c8.c",
            "exact/resource_3c7_c_020000c8.c",
        ] {
            assert_flag(source, "-fno-gcse");
        }
        assert_no_flag("semantic/resource_3c2_c_0200006d.c", "-fno-gcse");
    }

    #[test]
    fn resource_3ce_value_entry_route_is_path_specific() {
        for source in [
            "semantic/resource_3ce_c_02000cf4.c",
            "exact/resource_3ce_c_02000cf4.c",
            "semantic/resource_3cd_c_020004b0.c",
            "exact/resource_3cd_c_020004b0.c",
        ] {
            for flag in [
                "-mgrouped-dma-store",
                "-fno-flow2-cleanup-cfg",
                "-fno-cse-two-insn-immediate",
                "-fno-reload-cse-regs",
                "-fthumb-group-value1-before-base",
                "-fthumb-group-value1-in-place",
                "-fthumb-sink-group-pool-loads",
                "-fthumb-group-control-last",
                "-fthumb-hoist-add-immediate",
                "-fthumb-3ce-value-entry-cluster",
            ] {
                assert_flag(source, flag);
            }
        }
        assert_no_flag(
            "semantic/resource_3ce_c_02000cf5.c",
            "-fthumb-group-value1-before-base",
        );

        for source in [
            "semantic/resource_3ce_c_02000b10.c",
            "exact/resource_3ce_c_02000b10.c",
        ] {
            assert_flag(source, "-fthumb-no-constant-reuse");
            assert_flag(source, "-fthumb-sink-past-pool-load");
        }
        assert_no_flag(
            "semantic/resource_3ce_c_02000b11.c",
            "-fthumb-no-constant-reuse",
        );
    }

    #[test]
    fn resource_38f_call_cluster_route_is_path_specific() {
        for source in [
            "semantic/resource_38f_c_020003c8.c",
            "exact/resource_38f_c_020003c8.c",
        ] {
            assert_flag(source, "-fsched-38f-call-cluster");
        }
        assert_no_flag(
            "semantic/resource_38f_c_020003c9.c",
            "-fsched-38f-call-cluster",
        );
    }

    #[test]
    fn resource_392_scene_pair_route_is_path_specific() {
        for source in [
            "semantic/resource_392_c_02000a2c.c",
            "exact/resource_392_c_02000a2c.c",
        ] {
            assert_flag(
                source,
                "-fthumb-order-zero-arg1-before-nonzero-arg0",
            );
        }
        assert_no_flag(
            "semantic/resource_392_c_02000a2d.c",
            "-fthumb-order-zero-arg1-before-nonzero-arg0",
        );
    }

    #[test]
    fn resource_3af_phase_route_is_path_specific() {
        for source in [
            "semantic/resource_3af_c_020000c4.c",
            "exact/resource_3af_c_020000c4.c",
        ] {
            assert_flag(source, "-fsched-low-dest-first");
        }
        assert_no_flag(
            "semantic/resource_3af_c_020000c5.c",
            "-fsched-low-dest-first",
        );
    }

    #[test]
    fn resource_391_entry_scene_route_is_path_specific() {
        for source in [
            "semantic/resource_391_c_02000c68.c",
            "exact/resource_391_c_02000c68.c",
        ] {
            assert_flag(source, "-fthumb-stack-args-before-stores");
            assert_flag(source, "-fthumb-scene-call-sheets");
        }
        assert_no_flag(
            "semantic/resource_391_c_02000c69.c",
            "-fthumb-scene-call-sheets",
        );
    }
}
