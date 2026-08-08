//! PORT NOTE: ported from `tools/lib/alchemy_gcc.ts` -- the compiler-routing
//! surface that `verify.ts` reaches transitively through `sourceToAssemblyPlan`.
//! Ported here: `ROOT`, `BUNDLE`/`DRIVER`/`GS2_*`/`AGBCC_*` path constants,
//! `CFLAGS`/`GS2_CFLAGS`/`AGBCC_CFLAGS`, `bundleForTarget`, `driverForTarget`,
//! `cflagsForTarget`, `sourceStem`, `overlayStem`, `sourceKey`,
//! `cflagsForSource`, `cflagsForTargetSource`, `usesAgbccCompiler`, and every
//! routing `Set`/`Map` those read. The tables live in `routing_data.rs`.
//!
//! This module used to live inside the `verify` crate and told the reader to
//! delete it once `alchemy_gcc.ts` was ported. That is done: this crate *is*
//! the port, `verify` depends on it by path, and the remaining duplication
//! against the still-live TypeScript is checked by `crate::drift` rather than
//! promised by a comment.

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
            .expect("tools-rs has a parent")
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

    let abi_base: Vec<String> =
        if has(DEFAULT_ABI_SOURCES, stem) || has(DEFAULT_ABI_OVERLAY_SOURCES, key) {
            cflags()
                .into_iter()
                .filter(|f| f != "-fcall-used-r4")
                .collect()
        } else {
            cflags()
        };
    let mut out: Vec<String> =
        if has(NO_INTERWORK_SOURCES, stem) || has(NO_INTERWORK_OVERLAY_SOURCES, key) {
            abi_base
                .into_iter()
                .filter(|f| f != "-mthumb-interwork")
                .collect()
        } else {
            abi_base
        };

    macro_rules! push {
        ($flags:expr) => {
            out.extend($flags.iter().map(|f| (*f).to_string()))
        };
    }

    if has(FIXED_R3_SOURCES, stem) {
        push!(&["-ffixed-r3"]);
    }
    if has(FIXED_LR_SOURCES, stem) {
        push!(&["-ffixed-r14"]);
    }
    if has(OPTIMIZE_O1_SOURCES, stem) {
        push!(&["-O1"]);
    }
    if has(THUMB_0807A664_SOURCES, stem) {
        push!(&["-fno-gcse", "-fno-force-mem", "-fthumb-0807a664-exact"]);
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
    if has(NO_CSE_TWO_INSN_IMMEDIATE_SOURCES, stem) {
        push!(&["-fno-cse-two-insn-immediate"]);
    }
    if has(NO_RERUN_CSE_AFTER_LOOP_SOURCES, stem) {
        push!(&["-fno-rerun-cse-after-loop"]);
    }
    if has(NO_RERUN_LOOP_OPT_SOURCES, stem) {
        push!(&["-fno-rerun-loop-opt"]);
    }
    if has(NO_GCSE_SOURCES, stem) {
        push!(&["-fno-gcse"]);
    }
    if has(NO_EXPENSIVE_SOURCES, stem) {
        push!(&["-fno-expensive-optimizations"]);
    }
    if has(NO_STRENGTH_REDUCE_SOURCES, stem) {
        push!(&["-fno-strength-reduce"]);
    }
    if has(SCHED_HIGH_DEST_FIRST_SOURCES, stem) {
        push!(&["-fsched-high-dest-first"]);
    }
    if has(SCHED_LOW_DEST_FIRST_SOURCES, stem) {
        push!(&["-fsched-low-dest-first"]);
    }
    if has(NO_CONTIGUOUS_IMMEDIATE_SOURCES, stem) {
        push!(&["-fno-thumb-contiguous-immediate"]);
    }
    if has(NO_SCHED_DEPEND_COUNT_SOURCES, stem) {
        push!(&["-fno-sched-depend-count"]);
    }
    if has(LATE_FRAME_ALLOCATION_SOURCES, stem) {
        push!(&["-fthumb-late-frame-allocation"]);
    }
    if has(SPLIT_GROUP_BASE_SOURCES, stem) {
        push!(&["-fthumb-split-group-base"]);
    }
    if has(HOIST_PARAMETER_SAVE_SOURCES, stem) {
        push!(&["-fthumb-hoist-parameter-save"]);
    }
    if has(MINIPOOL_TAIL_FIRST_SOURCES, stem) {
        push!(&["-fthumb-minipool-tail-first"]);
    }
    if has(ENTRY_SAVES_DESCENDING_SOURCES, stem) {
        push!(&["-fthumb-entry-saves-descending"]);
    }
    if has(GROUP_CONTROL_LAST_SOURCES, stem) {
        push!(&["-fthumb-group-control-last"]);
    }
    if has(GROUP_POOLED_CONTROL_LAST_SOURCES, stem) {
        push!(&["-fthumb-group-pooled-control-last"]);
    }
    if has(HIGH_MOVE_BEFORE_ALU_SOURCES, stem) {
        push!(&["-fthumb-high-move-before-alu"]);
    }
    if has(MOVE_BEFORE_IMMEDIATE_ALU_SOURCES, stem) {
        push!(&["-fno-schedule-insns2", "-fthumb-move-before-immediate-alu"]);
    }
    if has(LOOP_INVARIANT_BLOCK_HEAD_SOURCES, stem) {
        push!(&["-floop-invariant-block-head"]);
    }
    if has(GROUP_ZERO_ANY_REGISTER_SOURCES, stem) {
        push!(&["-fthumb-group-zero-any-register"]);
    }
    if has(ARG0_AFTER_SPLIT_SOURCES, stem) {
        push!(&["-fthumb-arg0-after-split"]);
    }
    if has(CALL_ARG0_POOL_LOAD_SOURCES, stem) {
        push!(&["-fthumb-call-arg0-pool-load"]);
    }
    if has(RETURN_VALUE_BEFORE_STACK_ADJUST_SOURCES, stem) {
        push!(&["-fthumb-return-value-before-stack-adjust"]);
    }
    if has(SINK_GROUP_POOL_LOADS_SOURCES, stem) {
        push!(&["-fthumb-sink-group-pool-loads"]);
    }
    if has(SINK_STACK_ADJUST_SOURCES, stem) {
        push!(&["-fthumb-sink-stack-adjust"]);
    }
    if has(SINK_DEPENDENT_LOAD_SOURCES, stem) {
        push!(&["-fthumb-sink-dependent-load"]);
    }
    if has(COLLAPSE_DEAD_SCRATCH_SOURCES, stem) {
        push!(&["-fthumb-collapse-dead-scratch"]);
    }
    if has(SINK_BLOCK_CONSTANT_SOURCES, stem) {
        push!(&["-fthumb-sink-block-constant"]);
    }
    if has(SINK_PAST_POOL_LOAD_SOURCES, stem) {
        push!(&["-fthumb-sink-past-pool-load"]);
    }
    if has(GROUP_VALUE1_BEFORE_BASE_SOURCES, stem) {
        push!(&["-fthumb-group-value1-before-base"]);
    }
    if has(GROUP_CONTROL_LAST_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-group-control-last"]);
    }
    if has(MOVE_BEFORE_ALU_SOURCES, stem) {
        push!(&["-fthumb-move-before-alu"]);
    }
    if has(NO_REGMOVE_SOURCES, stem) {
        push!(&["-fno-regmove"]);
    }
    if has(ENTRY_LITERAL_FIRST_SOURCES, stem) {
        push!(&["-fno-schedule-insns2", "-mthumb-entry-literal-first"]);
    }
    if has(HIGH_REGISTER_MOVE_FIRST_SOURCES, stem) {
        push!(&["-mhigh-register-move-first"]);
    }
    if has(ORR_DEAD_INPUT_REUSE_SOURCES, stem) {
        push!(&["-fthumb-orr-dead-input-reuse"]);
    }
    if has(ORR_DEAD_INPUT_REUSE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-orr-dead-input-reuse"]);
    }
    if has(CALL_LITERAL_ARG1_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-literal-arg1-first"]);
    }
    if has(CALL_ARG1_BEFORE_ARG0_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-arg1-before-arg0"]);
    }
    if has(CALL_ARG0_REG_SOURCE_SOURCES, stem) {
        push!(&[
            "-fthumb-call-arg1-before-arg0",
            "-fthumb-call-arg0-reg-source"
        ]);
    }
    if has(SINK_CONSTANT_PAST_CALL_SOURCES, stem) {
        push!(&["-fthumb-sink-constant-past-call"]);
    }
    if has(MOVE_BEFORE_UNARY_ALU_SOURCES, stem) {
        push!(&["-fthumb-move-before-unary-alu"]);
    }
    // -O1 is appended after the baseline -O2 and wins as the later option.
    if has(OPTIMIZE_O1_OVERLAY_SOURCES, key) {
        push!(&["-O1"]);
    }
    if has(FIXED_LR_OVERLAY_SOURCES, key) {
        push!(&["-ffixed-r14"]);
    }
    if has(ENTRY_FRAME_CLUSTER_SOURCES, stem) {
        push!(&["-fthumb-entry-frame-cluster"]);
    }
    if has(LITERAL_BEFORE_INDEX_SHIFT_SOURCES, stem) {
        push!(&["-fthumb-literal-before-index-shift"]);
    }
    if has(LOW_CONSTANT_BEFORE_HIGH_MOVE_SOURCES, stem) {
        push!(&["-fthumb-low-constant-before-high-move"]);
    }
    if has(HIGH_MOVE_BEFORE_STACK_STORE_SOURCES, stem) {
        push!(&["-fthumb-high-move-before-stack-store"]);
    }
    if has(EARLY_FRAME_ALLOCATION_SOURCES, stem) {
        push!(&["-mearly-frame-allocation"]);
    }
    if has(NO_OPTIMIZE_SIBLING_CALLS_SOURCES, stem) {
        push!(&["-fno-optimize-sibling-calls"]);
    }
    if has(GROUPED_DMA_STORE_SOURCES, stem) {
        push!(&["-mgrouped-dma-store"]);
    }
    if has(NO_CONSTANT_REUSE_SOURCES, stem) {
        push!(&["-fthumb-no-constant-reuse"]);
    }
    if has(HOIST_ADD_IMMEDIATE_SOURCES, stem) {
        push!(&[
            "-mgrouped-dma-store",
            "-fthumb-group-control-rematerialize",
            "-mlow-reg-order=2013",
            "-fthumb-sink-block-constant",
            "-fthumb-hoist-add-immediate",
        ]);
    }
    if has(SINK_ADD_IMMEDIATE_SOURCES, stem) {
        push!(&[
            "-mgrouped-dma-store",
            "-fthumb-sink-group-pool-loads",
            "-mthumb-load-latency-one",
            "-mearly-frame-allocation",
            "-fthumb-move-before-immediate-alu",
            "-fthumb-sink-block-constant",
            "-fthumb-sink-constant-past-memory",
            "-fthumb-earliest-frame-allocation",
            "-fthumb-copy-before-add-immediate",
            "-fno-schedule-insns2",
            "-fthumb-sink-add-immediate",
        ]);
    }
    if has(SINK_STORE_PAST_STORE_SOURCES, stem) {
        push!(&[
            "-mgrouped-dma-store",
            "-fthumb-sink-group-pool-loads",
            "-mthumb-load-latency-one",
            "-mearly-frame-allocation",
            "-fthumb-move-before-immediate-alu",
            "-fthumb-sink-block-constant",
            "-fthumb-sink-constant-past-memory",
            "-fthumb-sink-store-past-store",
            "-fthumb-pool-load-base-first",
        ]);
    }
    if has(GROUP_VALUE2_IN_PLACE_SOURCES, stem) {
        push!(&["-fthumb-group-value2-in-place"]);
    }
    if has(THUMB_IMMEDIATE_LATENCY_SOURCES, stem) {
        push!(&["-mthumb-immediate-latency"]);
    }
    if has(CALL_ARG0_MOVE_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-mcall-arg0-move-first"]);
    }
    if has(CALL_ARG0_BEFORE_STORE_SOURCES, stem) {
        push!(&[
            "-fno-sched-alias",
            "-fsched-store-first",
            "-fthumb-call-arg0-before-store"
        ]);
    }
    if has(POSTCALL_BYTE_INCREMENT_R2_SOURCES, stem) {
        push!(&["-fthumb-postcall-byte-increment-r2"]);
    }
    if has(GROUP_CONTROL_REMATERIALIZE_SOURCES, stem) {
        push!(&["-fthumb-group-control-rematerialize"]);
    }
    if has(SCHED_POOL_LOAD_LATE_SOURCES, stem) {
        push!(&["-fthumb-sched-pool-load-late"]);
    }
    if has(SCHED2_OFF_THUMB_SOURCES, stem) {
        push!(&["-fno-schedule-insns2"]);
    }
    if has(THUMB_LEAF_NO_LR_SOURCES, stem) {
        push!(&["-fthumb-leaf-no-lr"]);
    }
    if has(THUMB_NO_IF_CONVERT_SOURCES, stem) {
        push!(&["-fthumb-no-if-convert"]);
    }
    if let Some(order) = lookup(THUMB_LOW_REG_ORDER_SOURCES, stem) {
        out.push(format!("-mlow-reg-order={order}"));
    }
    if let Some(order) = lookup(THUMB_CALLEE_REG_ORDER_SOURCES, stem) {
        out.push(format!("-mcallee-reg-order={order}"));
    }
    if has(THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES, key) {
        push!(&["-mthumb-immediate-latency"]);
    }
    if has(NO_CANONICALIZE_COMPARISON_OVERLAY_SOURCES, key) {
        push!(&["-fno-canonicalize-comparison"]);
    }
    if has(NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES, key) {
        push!(&["-fno-sched-depend-count"]);
    }
    if has(THUMB_LOAD_LATENCY_ONE_OVERLAY_SOURCES, key) {
        push!(&["-mthumb-load-latency-one"]);
    }
    // PORT NOTE: the TypeScript has a `sourceKey(source) === "exact/
    // resource_379_c_02000074.c" ? [] : []` arm that contributes nothing in
    // either branch. It is dropped rather than reproduced.
    if has(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES, key) {
        push!(&["-fno-rerun-cse-after-loop"]);
    }
    if has(SCHED_POOL_LOAD_LATE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-sched-pool-load-late"]);
    }
    if has(SCHED_IMMEDIATE_BEFORE_POOL_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-sched-immediate-before-pool"]);
    }
    if has(THUMB_HI_IMMEDIATE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-hi-immediate"]);
    }
    if has(CALL_POOL_ARG1_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-pool-arg1-first"]);
    }
    if has(ARG_BEFORE_FINAL_SHIFT_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-arg-before-final-shift"]);
    }
    if has(CALL_ARG0_BEFORE_POOL_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-arg0-before-pool"]);
    }
    if has(CALL_ARGREG_BEFORE_POOL_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-argreg-before-pool"]);
    }
    if has(SWAP_ADJACENT_SHIFTS_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-swap-adjacent-shifts"]);
    }
    if has(STACK_ARGS_BEFORE_STORES_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-stack-args-before-stores"]);
    }
    if has(LITERAL_ARG1_FIRST_AFTER_CALL_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-literal-arg1-first-after-call"]);
    }
    if has(SMALL_SHIFT_BEFORE_IMMEDIATES_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-small-shift-before-immediates"]);
    }
    if has(BLOCKMOVE_DEST_BEFORE_SOURCE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-blockmove-dest-before-source"]);
    }
    if has(LITERAL_ARG1_FIRST_CHAINED_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-literal-arg1-first-chained"]);
    }
    if has(HIGH_MOVE_BEFORE_STORE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-high-move-before-store"]);
    }
    if has(POOL_LOAD_BEFORE_LOAD_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-pool-load-before-load"]);
    }
    if has(SHIFT_BEFORE_STORE_IN_SPLIT_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-shift-before-store-in-split"]);
    }
    if has(ARG_BEFORE_SHIFT_IN_SHEET_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-arg-before-shift-in-sheet"]);
    }
    if has(SINK_LOAD_PAST_STORE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-sink-load-past-store"]);
    }
    if has(CALL_ARG0_BETWEEN_POOL_PAIR_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-arg0-between-pool-pair"]);
    }
    if has(STORE_VALUE_BEFORE_BASE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-store-value-before-base"]);
    }
    if has(SWAP_SHIFTS_ACROSS_INSN_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-swap-shifts-across-insn"]);
    }
    if has(ORR_INTO_OLDER_INPUT_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-orr-into-older-input"]);
    }
    if has(CALL_ARG0_BEFORE_POOL_PAIR_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-call-arg0-before-pool-pair"]);
    }
    if has(SINK_POOL_LOAD_TO_USE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-sink-pool-load-to-use"]);
    }
    if has(SINK_CONSTANT_PAST_CALL_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-sink-constant-past-call"]);
    }
    if has(NO_THREAD_JUMPS_OVERLAY_SOURCES, key) {
        push!(&["-fno-thread-jumps"]);
    }
    if has(NO_GCSE_OVERLAY_SOURCES, key) {
        push!(&["-fno-gcse"]);
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
    if has(NO_CSE_TWO_INSN_IMMEDIATE_OVERLAY_SOURCES, key) {
        push!(&["-fno-cse-two-insn-immediate"]);
    }
    if has(NO_CSE_SHIFT_IMMEDIATE_OVERLAY_SOURCES, key) {
        push!(&["-fno-cse-shift-immediate"]);
    }
    if has(NO_CSE_POOL_IMMEDIATE_OVERLAY_SOURCES, key) {
        push!(&["-fno-cse-pool-immediate"]);
    }
    if has(NO_CONSTANT_REUSE_OVERLAY_SOURCES, key) {
        push!(&["-fthumb-no-constant-reuse"]);
    }
    if has(SCHED_LOW_DEST_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-fsched-low-dest-first"]);
    }
    if has(SCHED_HIGH_DEST_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-fsched-high-dest-first"]);
    }
    if has(SCHED_CALL_DEST_DESCENDING_OVERLAY_SOURCES, key) {
        push!(&["-fsched-call-dest-descending"]);
    }
    if has(FIXED_R7_OVERLAY_SOURCES, key) {
        push!(&["-ffixed-r7"]);
    }
    if has(NO_SCHED_ALIAS_OVERLAY_SOURCES, key) {
        push!(&["-fno-sched-alias"]);
    }
    if has(NO_GCSE_INSERT_LOAD_OVERLAY_SOURCES, key) {
        push!(&["-fno-gcse-insert-load"]);
    }
    if has(SCHED_STORE_FIRST_OVERLAY_SOURCES, key) {
        push!(&["-fsched-store-first"]);
    }
    if has(GROUPED_DMA_STORE_OVERLAY_SOURCES, key) {
        push!(&["-mgrouped-dma-store"]);
    }
    if has(EARLY_LITERAL_POOL_OVERLAY_SOURCES, stem) || has(EARLY_LITERAL_POOL_OVERLAY_PATHS, key) {
        push!(&["-mthumb-early-literal-pool"]);
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
        if has(AGBCC_LITERAL_BEFORE_SHIFT_SOURCES, &stem) {
            out.push("-mliteral-before-shift".to_string());
        }
        if has(AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES, &stem) {
            out.push("-mcommutative-copy-constant".to_string());
        }
        if has(AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES, &stem) {
            out.push("-mprologue-next-high-reg".to_string());
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
}
