//! Compiler routing; `routing_data` is the sole table source.
//!
//! A source routes to exactly one compiler family, and every member of a
//! family compiles with that family's one flag set. There is no per-file
//! flag: a function that is not exact under its family's flags is not exact,
//! and stays retained assembly until an ordinary C spelling reproduces it.
use crate::compiler::routing_data::*;
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
    root().join("out/compilers/dist")
}
pub fn driver() -> PathBuf {
    bundle().join("xgcc")
}
pub fn agbcc_driver() -> PathBuf {
    bundle().join("agbcc").join("old_agbcc")
}
/// Modern syntax support with the historical integer/soft-float object ABI.
pub fn assembly_command(source: &str, object: &str) -> Vec<String> {
    [
        "arm-none-eabi-as",
        "-mcpu=arm7tdmi",
        "-mthumb-interwork",
        "-meabi=gnu",
        "-mfpu=softfpa",
        "-mfloat-abi=soft",
        "-o",
        object,
        source,
    ]
    .iter()
    .map(|s| (*s).to_string())
    .collect()
}
/// Unmodified GCC/agbcc output uses era GAS alignment semantics.
pub fn compiler_assembly_command(source: &str, object: &str) -> Vec<String> {
    let mut command = vec![bundle().join("as").to_string_lossy().into_owned()];
    command.extend(
        [
            "-marm7tdmi",
            "-mthumb",
            "-mthumb-interwork",
            "-o",
            object,
            source,
        ]
        .iter()
        .map(|s| (*s).to_string()),
    );
    command
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
/// Whether the game's own code was built to interwork with ARM callers.
///
/// This is a per-game build fact read off the shipped images, not a tuning.
/// GS1 interworks: 1174 of its 1195 located gs1-en functions return through
/// `pop {rN}; bx rN`. GS2 does not: 1539 of 1581 gs2-en functions return with
/// `pop {..., pc}`, which arm.c `thumb_exit` reaches only when TARGET_INTERWORK
/// is clear, and the battle owner's epilogue at 08120454+0x2054 matches that
/// output byte for byte while GS1's owner matches the interworking output.
/// GS2's remaining interworking returns sit in objects inherited from the GS1
/// build, which keep the Agbcc family and its own flag set.
fn interworks(target: CompilerTarget) -> bool {
    match target {
        CompilerTarget::Gs1 => true,
        CompilerTarget::Gs2 => false,
    }
}
fn base_cflags(target: CompilerTarget) -> Vec<String> {
    let mut flags: Vec<String> = ["-O2", "-mthumb"]
        .iter()
        .map(|s| (*s).to_string())
        .collect();
    if interworks(target) {
        flags.push("-mthumb-interwork".to_string());
    }
    for flag in [
        "-mcpu=arm7tdmi",
        "-fno-builtin",
        "-nostdinc",
        "-ffreestanding",
        "-fcall-used-r4",
    ] {
        flags.push(flag.to_string());
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
    fn assembly_uses_historical_soft_float_abi() {
        let command = assembly_command("input.s", "output.o");
        for flag in ["-meabi=gnu", "-mfpu=softfpa", "-mfloat-abi=soft"] {
            assert!(command.iter().any(|arg| arg == flag));
        }
        assert_eq!(&command[command.len() - 3..], ["-o", "output.o", "input.s"]);
    }
    #[test]
    fn each_game_uses_its_own_include_tree() {
        let gs1 = cflags_for_target(CompilerTarget::Gs1);
        let gs2 = cflags_for_target(CompilerTarget::Gs2);
        assert!(gs1.iter().any(|flag| flag.ends_with("/games/gs1/include")));
        assert!(gs2.iter().any(|flag| flag.ends_with("/games/gs2/include")));
        assert!(!gs2.iter().any(|flag| flag.ends_with("/games/gs1/include")));
        let shared: Vec<&String> = gs1
            .iter()
            .filter(|flag| *flag != "-mthumb-interwork" && !flag.starts_with("-I"))
            .collect();
        let derived: Vec<&String> = gs2.iter().filter(|flag| !flag.starts_with("-I")).collect();
        assert_eq!(shared, derived);
        for flags in [&gs1, &gs2] {
            assert!(!flags
                .iter()
                .any(|flag| flag == "-mthumb-inline-register-call"));
        }
    }
    /// The images disagree about interworking, so the two Game routes do too.
    /// GS2 keeps the -fcall-used-r4 ABI: only 29 of 1414 measured gs2-en
    /// functions save r4, and all of those are inherited Agbcc-family objects.
    #[test]
    fn only_gs1_game_code_interworks() {
        let gs1 = cflags_for_target_source(CompilerTarget::Gs1, "080bbb0c.c");
        let gs2 = cflags_for_target_source(CompilerTarget::Gs2, "08120454.c");
        assert!(gs1.iter().any(|flag| flag == "-mthumb-interwork"));
        assert!(!gs2.iter().any(|flag| flag == "-mthumb-interwork"));
        for flags in [&gs1, &gs2] {
            assert!(flags.iter().any(|flag| flag == "-fcall-used-r4"));
            assert!(flags.iter().any(|flag| flag == "-mthumb"));
        }
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
                cflags_for_target_source(CompilerTarget::Gs1, owner),
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
            let flags = cflags_for_target_source(CompilerTarget::Gs1, owner);
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
