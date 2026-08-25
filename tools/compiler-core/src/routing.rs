//! Compiler routing; `routing_data` is the sole table source.

use std::collections::{HashMap, HashSet};
use std::path::{Component, Path, PathBuf};
use std::sync::OnceLock;

use crate::routing_data::*;

/// Repository root: `<crate>/../..`.
pub fn root() -> &'static Path {
    static ROOT: OnceLock<PathBuf> = OnceLock::new();
    ROOT.get_or_init(|| Path::new(env!("CARGO_MANIFEST_DIR")).parent().expect("crate dir has a parent").parent().expect("tools has a parent").to_path_buf())
}

pub fn bundle() -> PathBuf {
    root().join("alchemy-gcc").join("dist")
}
pub fn driver() -> PathBuf {
    bundle().join("xgcc")
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
        // GS2 is a build target, not a compiler family; it also uses GCC 2.96.
        CompilerTarget::Gs2 => bundle(),
    }
}

pub fn driver_for_target(target: CompilerTarget) -> PathBuf {
    match target {
        CompilerTarget::Gs1 => driver(),
        CompilerTarget::Gs2 => driver(),
    }
}

fn include_flag(target: CompilerTarget) -> String {
    format!("-I{}", root().join("games").join(target.as_str()).join("include").display())
}

fn base_cflags(target: CompilerTarget) -> Vec<String> {
    ["-O2", "-mthumb", "-mthumb-interwork", "-mcpu=arm7tdmi", "-fno-builtin", "-nostdinc", "-ffreestanding", "-fcall-used-r4"].iter().map(|s| (*s).to_string()).chain(std::iter::once(include_flag(target))).collect()
}

pub fn cflags() -> Vec<String> {
    base_cflags(CompilerTarget::Gs1)
}

pub fn gs2_cflags() -> Vec<String> {
    // Keep a target-specific surface for future measured GS2 flag deltas.
    base_cflags(CompilerTarget::Gs2)
}

pub fn agbcc_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding"].iter().map(|s| (*s).to_string()).collect()
}

pub fn cflags_for_target(target: CompilerTarget) -> Vec<String> {
    match target {
        CompilerTarget::Gs1 => cflags(),
        CompilerTarget::Gs2 => gs2_cflags(),
    }
}

#[cfg(test)]
mod target_tests {
    use super::*;

    #[test]
    fn each_game_uses_its_own_include_tree() {
        let gs1 = cflags_for_target(CompilerTarget::Gs1);
        let gs2 = cflags_for_target(CompilerTarget::Gs2);
        assert!(gs1.iter().any(|flag| flag.ends_with("/games/gs1/include")));
        assert!(gs2.iter().any(|flag| flag.ends_with("/games/gs2/include")));
        assert!(!gs2.iter().any(|flag| flag.ends_with("/games/gs1/include")));
    }

    #[test]
    fn grouped_runtime_candidates_use_canonical_flags() {
        for owner in ["080040e8.c", "0800412c.c", "08004144.c", "08004198.c", "080041d8.c", "08004278.c", "080042c8.c", "0800430c.c", "08004358.c", "0800439c.c", "080043e0.c", "080060e8.c"] {
            assert_eq!(cflags_for_source(owner), cflags(), "unexpected override for {owner}");
        }
    }
}

/// `basename(source, extname(source))` for POSIX paths.
pub fn source_stem(source: &str) -> String {
    let base = source.rsplit('/').next().unwrap_or(source);
    // node:path extname ignores a leading dot.
    match base.rfind('.') {
        Some(index) if index > 0 => base[..index].to_string(),
        _ => base.to_string(),
    }
}

fn is_hex8(value: &str) -> bool {
    value.len() == 8 && value.bytes().all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase())
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
    let resolved = if Path::new(source).is_absolute() { normalize(Path::new(source)) } else { normalize(&root().join(source)) };
    let base = normalize(root());
    let resolved_parts: Vec<_> = resolved.components().collect();
    let base_parts: Vec<_> = base.components().collect();
    let shared = resolved_parts.iter().zip(base_parts.iter()).take_while(|(a, b)| a == b).count();
    let mut parts: Vec<String> = vec!["..".to_string(); base_parts.len() - shared];
    for component in &resolved_parts[shared..] {
        parts.push(component.as_os_str().to_string_lossy().into_owned());
    }
    parts.join("/")
}

fn set(table: &'static [&'static str]) -> &'static HashSet<&'static str> {
    // Static table addresses are stable cache keys.
    static CACHE: OnceLock<std::sync::Mutex<HashMap<usize, &'static HashSet<&'static str>>>> = OnceLock::new();
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

/// Cache of each table's file stems.
fn stem_set(table: &'static [&'static str]) -> &'static HashSet<String> {
    static CACHE: OnceLock<std::sync::Mutex<HashMap<usize, &'static HashSet<String>>>> = OnceLock::new();
    let cache = CACHE.get_or_init(|| std::sync::Mutex::new(HashMap::new()));
    let key = table.as_ptr() as usize;
    let mut guard = cache.lock().expect("routing cache is not poisoned");
    guard.entry(key).or_insert_with(|| {
        let built: HashSet<String> = table.iter().map(|entry| source_stem(entry)).collect();
        Box::leak(Box::new(built))
    })
}

/// Overlay flags follow an owner across adopt/park path changes. Path-keyed
/// matching would silently drop sanctioned flags after either move.
fn has_owner(table: &'static [&'static str], source: &str) -> bool {
    stem_set(table).contains(&source_stem(source))
}

/// Append order is load-bearing because later driver options win.
pub fn cflags_for_source(source: &str) -> Vec<String> {
    let stem = overlay_stem(source);
    let stem = stem.as_str();

    // Overrides must be evidenced stock GCC 2.96 options, never source disguises.
    let mut out: Vec<String> = if has(NO_INTERWORK_SOURCES, stem) || has_owner(NO_INTERWORK_OVERLAY_SOURCES, source) { cflags().into_iter().filter(|f| f != "-mthumb-interwork").collect() } else { cflags() };

    // These soft-float leaves require the stock ABI with r4 callee-saved.
    if has_owner(CALLEE_SAVED_R4_OVERLAY_SOURCES, source) || has(CALLEE_SAVED_R4_SOURCES, stem) {
        out.retain(|f| f != "-fcall-used-r4");
    }

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
    if has_owner(OPTIMIZE_O3_OVERLAY_SOURCES, source) {
        push!(&["-O3"]);
    }
    if has(UNSCHEDULED_SOURCES, stem) {
        push!(&["-fno-schedule-insns", "-fno-schedule-insns2"]);
    }
    if has_owner(UNSCHEDULED_OVERLAY_SOURCES, source) {
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
    if has_owner(NO_GCSE_OVERLAY_SOURCES, source) {
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
    if has_owner(OPTIMIZE_O1_OVERLAY_SOURCES, source) {
        push!(&["-O1"]);
    }
    if has(NO_OPTIMIZE_SIBLING_CALLS_SOURCES, stem) {
        push!(&["-fno-optimize-sibling-calls"]);
    }
    if has(SCHED2_OFF_THUMB_SOURCES, stem) {
        push!(&["-fno-schedule-insns2"]);
    }
    if has_owner(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES, source) {
        push!(&["-fno-rerun-cse-after-loop"]);
    }
    if has_owner(NO_THREAD_JUMPS_OVERLAY_SOURCES, source) {
        push!(&["-fno-thread-jumps"]);
    }
    if has_owner(NO_EXPENSIVE_OVERLAY_SOURCES, source) {
        push!(&["-fno-expensive-optimizations"]);
    }
    if has_owner(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES, source) {
        push!(&["-fno-cse-follow-jumps", "-fno-cse-skip-blocks"]);
    }
    if has_owner(NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES, source) {
        push!(&["-fno-cse-skip-blocks"]);
    }
    if has(NO_STRICT_ALIASING_SOURCES, stem) {
        push!(&["-fno-strict-aliasing"]);
    }
    if has_owner(NO_STRICT_ALIASING_OVERLAY_SOURCES, source) {
        push!(&["-fno-strict-aliasing"]);
    }
    if has_owner(FIXED_R7_OVERLAY_SOURCES, source) {
        push!(&["-ffixed-r7"]);
    }

    out
}

pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    let stem = source_stem(source);
    match target {
        CompilerTarget::Gs1 => has(AGBCC_SOURCES, &stem),
        CompilerTarget::Gs2 => has(GS2_AGBCC_SOURCES, &stem),
    }
}

pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    let stem = source_stem(source);
    if uses_agbcc_compiler(target, source) {
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
        if match target {
            CompilerTarget::Gs1 => has(AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES, &stem),
            CompilerTarget::Gs2 => has(GS2_AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES, &stem),
        } {
            out.push("-mprologue-next-high-reg".to_string());
        }
        if match target {
            CompilerTarget::Gs1 => has(AGBCC_TRACK_NARROW_VALUE_R1_SOURCES, &stem),
            CompilerTarget::Gs2 => has(GS2_AGBCC_TRACK_NARROW_VALUE_R1_SOURCES, &stem),
        } {
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
