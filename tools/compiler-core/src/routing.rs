//! Compiler routing; `routing_data` is the sole table source.
//!
//! A source routes to exactly one compiler family, and every member of a
//! family compiles with that family's one flag set. There is no per-file
//! flag: a function that is not exact under its family's flags is not exact,
//! and stays retained assembly until an ordinary C spelling reproduces it.
use crate::routing_data::*;
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
/// The compiler family a source belongs to. Membership is provenance (which
/// compiler and library built the bytes), never a per-function tuning.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerFamily {
    /// Game code: the staged GCC 2.96 with the canonical flags.
    Game,
    /// The soft-float support leaves shipped prebuilt with the toolchain:
    /// stock ABI (r4 callee-saved) and no interworking.
    SoftFloatLibrary,
    /// Library code built with agbcc.
    Agbcc,
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
/// The soft-float library family: the canonical flags without interworking
/// and with the stock r4 callee-saved ABI, uniformly for every member.
pub fn soft_float_library_cflags() -> Vec<String> {
    cflags()
        .into_iter()
        .filter(|f| f != "-mthumb-interwork" && f != "-fcall-used-r4")
        .collect()
}
pub fn cflags_for_target(target: CompilerTarget) -> Vec<String> {
    base_cflags(target)
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
fn has(table: &'static [&'static str], value: &str) -> bool {
    table.contains(&value)
}
/// Family membership follows an owner across adopt/park path changes.
fn has_owner(table: &'static [&'static str], source: &str) -> bool {
    let stem = source_stem_ref(source);
    table.iter().any(|entry| source_stem_ref(entry) == stem)
}
pub fn family_for_source(target: CompilerTarget, source: &str) -> CompilerFamily {
    let stem = source_stem_ref(source);
    let agbcc = match target {
        CompilerTarget::Gs1 => has(AGBCC_SOURCES, stem),
        CompilerTarget::Gs2 => has(GS2_AGBCC_SOURCES, stem),
    };
    if agbcc {
        return CompilerFamily::Agbcc;
    }
    if target == CompilerTarget::Gs1 && has_owner(SOFT_FLOAT_LIBRARY_OVERLAY_SOURCES, source) {
        return CompilerFamily::SoftFloatLibrary;
    }
    CompilerFamily::Game
}
pub fn cflags_for_source(source: &str) -> Vec<String> {
    match family_for_source(CompilerTarget::Gs1, source) {
        CompilerFamily::SoftFloatLibrary => soft_float_library_cflags(),
        CompilerFamily::Agbcc => agbcc_cflags(),
        CompilerFamily::Game => cflags(),
    }
}
pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    family_for_source(target, source) == CompilerFamily::Agbcc
}
pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    match (family_for_source(target, source), target) {
        (CompilerFamily::Agbcc, _) => agbcc_cflags(),
        (CompilerFamily::SoftFloatLibrary, _) => soft_float_library_cflags(),
        (CompilerFamily::Game, CompilerTarget::Gs1) => cflags(),
        (CompilerFamily::Game, CompilerTarget::Gs2) => base_cflags(CompilerTarget::Gs2),
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
        assert!(!gs1
            .iter()
            .any(|flag| flag == "-mthumb-inline-register-call"));
        assert!(gs2
            .iter()
            .any(|flag| flag == "-mthumb-inline-register-call"));
    }
    #[test]
    fn game_code_always_compiles_with_the_canonical_flags() {
        for owner in [
            "080040e8.c",
            "080f9ef8.c",
            "080994d0.c",
            "080114a0.c",
            "0800307c.c",
            "games/gs1/src/resource_3ab_c_020007f4.c",
            "games/gs1/src/resource_381_c_02002e0c.c",
        ] {
            assert_eq!(
                cflags_for_source(owner),
                cflags(),
                "per-file override for {owner}"
            );
        }
    }
    #[test]
    fn soft_float_library_family_is_uniform() {
        for owner in [
            "games/gs1/src/resource_3a7_c_0200142c.c",
            "games/gs1/src/resource_3a7_c_02001544.c",
            "games/gs1/src/resource_3bf_c_02005ae0.c",
            "games/gs1/src/resource_3a7_c_0200145c.c",
        ] {
            let flags = cflags_for_source(owner);
            assert!(!flags.iter().any(|flag| flag == "-mthumb-interwork"));
            assert!(!flags.iter().any(|flag| flag == "-fcall-used-r4"));
            assert!(flags.iter().any(|flag| flag == "-O2"));
        }
    }
    #[test]
    fn agbcc_family_has_one_flag_set() {
        for owner in ["080fb670.c", "08006878.c", "080fa514.c"] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Gs1, owner),
                agbcc_cflags()
            );
        }
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Gs2, "081c2168.c"),
            agbcc_cflags()
        );
    }
}
