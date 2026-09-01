//! Compiler routing; `routing_data` is the sole table source.
use crate::routing_data::*;
use crate::source_paths::lower_hex;
use std::path::{Path, PathBuf};
use std::sync::OnceLock;
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
pub(crate) fn include_flag(target: CompilerTarget) -> String {
    format!(
        "-I{}",
        root()
            .join("games")
            .join(target.as_str())
            .join("include")
            .display()
    )
}
fn base_cflags(target: CompilerTarget) -> Vec<String> {
    let mut flags: Vec<String> = [
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
    .collect();
    if target == CompilerTarget::Gs2 {
        flags.push("-mthumb-inline-register-call".into());
    }
    flags.push(include_flag(target));
    flags
}
pub fn cflags() -> Vec<String> {
    base_cflags(CompilerTarget::Gs1)
}
pub fn agbcc_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}
pub fn cflags_for_target(target: CompilerTarget) -> Vec<String> {
    base_cflags(target)
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
        assert!(!gs1
            .iter()
            .any(|flag| flag == "-mthumb-inline-register-call"));
        assert!(gs2
            .iter()
            .any(|flag| flag == "-mthumb-inline-register-call"));
    }
    #[test]
    fn grouped_runtime_candidates_use_canonical_flags() {
        for owner in [
            "080040e8.c",
            "0800412c.c",
            "08004144.c",
            "08004198.c",
            "080041d8.c",
            "08004278.c",
            "080042c8.c",
            "0800430c.c",
            "08004358.c",
            "0800439c.c",
            "080043e0.c",
            "080060e8.c",
        ] {
            assert_eq!(
                cflags_for_source(owner),
                cflags(),
                "unexpected override for {owner}"
            );
        }
    }
    #[test]
    fn stop_music_track_keeps_r4_callee_saved() {
        let flags = cflags_for_source("080f9ef8.c");
        assert!(!flags.iter().any(|flag| flag == "-fcall-used-r4"));
    }
    #[test]
    fn battle_effect_gcse_route_is_owner_scoped() {
        for owner in ["080994d0.c", "0809abb4.c"] {
            assert!(
                cflags_for_source(owner)
                    .iter()
                    .any(|flag| flag == "-fno-gcse"),
                "missing evidenced GCSE route for {owner}"
            );
        }
        for owner in ["08098cd8.c", "080999f0.c", "0809ae64.c", "0809b698.c"] {
            assert!(
                !cflags_for_source(owner)
                    .iter()
                    .any(|flag| flag == "-fno-gcse"),
                "battle-effect GCSE route leaked to {owner}"
            );
        }
    }
    #[test]
    fn map_tile_block_expensive_route_is_owner_scoped() {
        assert!(cflags_for_source("080114a0.c")
            .iter()
            .any(|flag| flag == "-fno-expensive-optimizations"));
        assert!(
            !cflags_for_source("080113e4.c")
                .iter()
                .any(|flag| flag == "-fno-expensive-optimizations"),
            "map-tile route leaked to its canonical-flags sibling"
        );
    }
}
/// `basename(source, extname(source))` for POSIX paths.
fn source_stem_ref(source: &str) -> &str {
    let base = source.rsplit('/').next().unwrap_or(source);
    // node:path extname ignores a leading dot.
    match base.rfind('.') {
        Some(index) if index > 0 => &base[..index],
        _ => base,
    }
}
pub fn source_stem(source: &str) -> String {
    source_stem_ref(source).to_string()
}
fn is_hex8(value: &str) -> bool {
    value.len() == 8 && lower_hex(value)
}
/// `overlayStem`: an overlay row routes by its bare address, so a candidate
/// verified as `<addr>.c` and the installed `<overlay>_c_<addr>.c` agree.
fn overlay_stem(source: &str) -> &str {
    let stem = source_stem_ref(source);
    if is_hex8(&stem) {
        return stem;
    }
    if let Some(index) = stem.rfind("_c_") {
        let tail = &stem[index + 3..];
        if is_hex8(tail) {
            return tail;
        }
    }
    stem
}
fn has(table: &'static [&'static str], value: &str) -> bool {
    table.contains(&value)
}
/// Overlay flags follow an owner across adopt/park path changes. Path-keyed
/// matching would silently drop sanctioned flags after either move.
fn has_owner(table: &'static [&'static str], source: &str) -> bool {
    let stem = source_stem_ref(source);
    table.iter().any(|entry| source_stem_ref(entry) == stem)
}
/// Append order is load-bearing because later driver options win.
pub fn cflags_for_source(source: &str) -> Vec<String> {
    let stem = overlay_stem(source);
    // Overrides must be evidenced stock GCC 2.96 options, never source disguises.
    let mut out: Vec<String> =
        if has(NO_INTERWORK_SOURCES, stem) || has_owner(NO_INTERWORK_OVERLAY_SOURCES, source) {
            cflags()
                .into_iter()
                .filter(|f| f != "-mthumb-interwork")
                .collect()
        } else {
            cflags()
        };
    // These soft-float leaves require the stock ABI with r4 callee-saved.
    if has_owner(CALLEE_SAVED_R4_OVERLAY_SOURCES, source) || has(CALLEE_SAVED_R4_SOURCES, stem) {
        out.retain(|f| f != "-fcall-used-r4");
    }
    for (matched, flag) in [
        (has(FIXED_R3_SOURCES, stem), "-ffixed-r3"),
        (has(OPTIMIZE_O1_SOURCES, stem), "-O1"),
        (has(OPTIMIZE_OS_SOURCES, stem), "-Os"),
        (has_owner(OPTIMIZE_O3_OVERLAY_SOURCES, source), "-O3"),
        (has(UNSCHEDULED_SOURCES, stem), "-fno-schedule-insns"),
        (has(UNSCHEDULED_SOURCES, stem), "-fno-schedule-insns2"),
        (
            has_owner(UNSCHEDULED_OVERLAY_SOURCES, source),
            "-fno-schedule-insns2",
        ),
        (has(NO_CSE_FOLLOW_SOURCES, stem), "-fno-cse-follow-jumps"),
        (
            has(NO_RERUN_CSE_AFTER_LOOP_SOURCES, stem),
            "-fno-rerun-cse-after-loop",
        ),
        (has(NO_GCSE_SOURCES, stem), "-fno-gcse"),
        (has_owner(NO_GCSE_OVERLAY_SOURCES, source), "-fno-gcse"),
        (
            has(NO_EXPENSIVE_SOURCES, stem),
            "-fno-expensive-optimizations",
        ),
        (
            has(NO_STRENGTH_REDUCE_SOURCES, stem),
            "-fno-strength-reduce",
        ),
        (has(NO_REGMOVE_SOURCES, stem), "-fno-regmove"),
        (has_owner(OPTIMIZE_O1_OVERLAY_SOURCES, source), "-O1"),
        (
            has(NO_OPTIMIZE_SIBLING_CALLS_SOURCES, stem),
            "-fno-optimize-sibling-calls",
        ),
        (has(SCHED2_OFF_THUMB_SOURCES, stem), "-fno-schedule-insns2"),
        (
            has_owner(NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES, source),
            "-fno-rerun-cse-after-loop",
        ),
        (
            has_owner(NO_THREAD_JUMPS_OVERLAY_SOURCES, source),
            "-fno-thread-jumps",
        ),
        (
            has_owner(NO_EXPENSIVE_OVERLAY_SOURCES, source),
            "-fno-expensive-optimizations",
        ),
        (
            has_owner(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES, source),
            "-fno-cse-follow-jumps",
        ),
        (
            has_owner(NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES, source),
            "-fno-cse-skip-blocks",
        ),
        (
            has_owner(NO_CSE_SKIP_BLOCKS_OVERLAY_SOURCES, source),
            "-fno-cse-skip-blocks",
        ),
        (
            has(NO_STRICT_ALIASING_SOURCES, stem),
            "-fno-strict-aliasing",
        ),
        (
            has_owner(NO_STRICT_ALIASING_OVERLAY_SOURCES, source),
            "-fno-strict-aliasing",
        ),
        (has_owner(FIXED_R7_OVERLAY_SOURCES, source), "-ffixed-r7"),
    ] {
        if matched {
            out.push(flag.to_string());
        }
    }
    out
}
pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    let stem = source_stem_ref(source);
    match target {
        CompilerTarget::Gs1 => has(AGBCC_SOURCES, stem),
        CompilerTarget::Gs2 => has(GS2_AGBCC_SOURCES, stem),
    }
}
pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    let stem = source_stem_ref(source);
    if uses_agbcc_compiler(target, source) {
        let mut out = agbcc_cflags();
        let prologue = match target {
            CompilerTarget::Gs1 => AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES,
            CompilerTarget::Gs2 => GS2_AGBCC_PROLOGUE_NEXT_HIGH_REG_SOURCES,
        };
        let narrow_r1 = match target {
            CompilerTarget::Gs1 => AGBCC_TRACK_NARROW_VALUE_R1_SOURCES,
            CompilerTarget::Gs2 => GS2_AGBCC_TRACK_NARROW_VALUE_R1_SOURCES,
        };
        for (table, flag) in [
            (AGBCC_OPTIMIZE_O1_SOURCES, "-O1"),
            (AGBCC_NO_EXPENSIVE_SOURCES, "-fno-expensive-optimizations"),
            (AGBCC_NO_GCSE_SOURCES, "-fno-gcse"),
            (AGBCC_NO_REGMOVE_SOURCES, "-fno-regmove"),
            (AGBCC_LITERAL_BEFORE_SHIFT_SOURCES, "-mliteral-before-shift"),
            (
                AGBCC_COMMUTATIVE_COPY_CONSTANT_SOURCES,
                "-mcommutative-copy-constant",
            ),
            (prologue, "-mprologue-next-high-reg"),
            (narrow_r1, "-mtrack-narrow-value-r1"),
            (AGBCC_COMPARE_ONLY_AND_TST_SOURCES, "-mcompare-only-and-tst"),
        ] {
            if has(table, stem) {
                out.push(flag.to_string());
            }
        }
        return out;
    }
    match target {
        CompilerTarget::Gs1 => cflags_for_source(source),
        CompilerTarget::Gs2 => base_cflags(CompilerTarget::Gs2),
    }
}
