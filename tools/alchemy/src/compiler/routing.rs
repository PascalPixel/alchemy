//! Compiler routing; `routing_data` is the sole table source.
//!
//! A source routes to exactly one compiler family, and every member of a
//! family compiles with that family's one flag set. There is no per-file
//! flag: a function that is not exact under its family's flags is not exact,
//! and stays retained assembly until an ordinary C spelling reproduces it.
use crate::compiler::routing_data::*;
use crate::compiler::source_paths::SourceOwner;
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
    root().join("tools/out/compilers")
}
/// Native `arm-none-eabi` binutils built by bootstrap; ignored like the compilers.
pub fn binutils_prefix() -> PathBuf {
    root().join("tools/out/binutils")
}
/// Put the installed binutils first on `PATH` for every child process, so the
/// build never depends on system packages or an emulated host toolchain.
pub fn prefer_installed_binutils() {
    let bin = binutils_prefix().join("bin");
    if !bin.is_dir() {
        return;
    }
    let current = std::env::var_os("PATH").unwrap_or_default();
    if let Some(path) = path_with_first(&bin, &current) {
        std::env::set_var("PATH", path);
    }
}
fn path_with_first(first: &Path, current: &std::ffi::OsStr) -> Option<std::ffi::OsString> {
    let rest = std::env::split_paths(current).filter(|entry| entry != first);
    std::env::join_paths(std::iter::once(first.to_path_buf()).chain(rest)).ok()
}
/// Both games use the licensed agscc source and the same executable bundle.
/// TLA selects its reconstructed lowering with the explicit -mgs2 option.
pub fn bundle_for(_target: CompilerTarget) -> PathBuf {
    bundle()
}
pub fn agbcc_driver() -> PathBuf {
    bundle().join("agbcc").join("old_agbcc")
}
/// Modern syntax support with the historical integer/soft-float object ABI.
// GAS marks softfpa objects as VFP; explicit FPA with soft-float retains
// the historical integer/soft-float ABI used by the compiler objects.
pub fn assembly_command(source: &str, object: &str) -> Vec<String> {
    [
        "arm-none-eabi-as",
        "-mcpu=arm7tdmi",
        "-mthumb-interwork",
        "-meabi=gnu",
        "-mfpu=fpa",
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
    Tbs,
    Tla,
}
impl CompilerTarget {
    pub fn directory(self) -> &'static str {
        game_directory(self.as_str())
    }
    /// This game's reconstruction scaffolding, `recon/tbs` or `recon/tla`.
    pub fn recon(self) -> &'static str {
        match self {
            CompilerTarget::Tbs => "recon/tbs",
            CompilerTarget::Tla => "recon/tla",
        }
    }
    pub fn as_str(self) -> &'static str {
        match self {
            CompilerTarget::Tbs => "tbs",
            CompilerTarget::Tla => "tla",
        }
    }
}
/// Build IDs remain stable when their physical workspaces are renamed.
pub fn game_directory(game: &str) -> &str {
    match game {
        "tbs" => "THE BROKEN SEAL",
        "tla" => "THE LOST AGE",
        _ => game,
    }
}
/// `games/<GAME>` holds only the Camelot-shaped tree; drafts, retained
/// listings, registries and metrics live under `recon/<build id>`.
pub fn recon_directory(game: &str) -> String {
    format!("recon/{game}")
}
/// The compiler family a source belongs to. Membership is provenance (which
/// compiler and library built the bytes), never a per-function tuning.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerFamily {
    /// Game code: the staged GCC 2.96 with the canonical flags.
    Game,
    /// Library code built with agbcc.
    Agbcc,
    /// The flash library, built with agbcc at -O.
    AgbccFlash,
}
pub(crate) fn include_flag(target: CompilerTarget) -> String {
    format!(
        "-I{}",
        root()
            .join("games")
            .join(target.directory())
            .join("INCLUDE")
            .display()
    )
}
/// Whether the game's own code was built to interwork with ARM callers.
///
/// This is a per-game build fact read off the shipped images, not a tuning.
/// TBS interworks: 1174 of its 1195 located tbs-en functions return through
/// `pop {rN}; bx rN`. TLA does not: 1539 of 1581 tla-en functions return with
/// `pop {..., pc}`, which arm.c `thumb_exit` reaches only when TARGET_INTERWORK
/// is clear, and the battle owner's epilogue at 08120454+0x2054 matches that
/// output byte for byte while TBS's owner matches the interworking output.
/// TLA's remaining interworking returns sit in objects inherited from the TBS
/// build, which keep the Agbcc family and its own flag set.
fn interworks(target: CompilerTarget) -> bool {
    match target {
        CompilerTarget::Tbs => true,
        CompilerTarget::Tla => false,
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
    if target == CompilerTarget::Tla {
        flags.push("-mgs2".to_string());
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
    base_cflags(CompilerTarget::Tbs)
}
pub fn agbcc_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O2", "-fno-builtin", "-ffreestanding"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}
/// The flash library's agbcc flags: the library family's set at -O.
pub fn agbcc_flash_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O", "-fno-builtin", "-ffreestanding"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}
/// The compiler runtime the images linked from the toolchain library, built
/// from the licensed agscc container (`compiler::runtime`): the canonical
/// flags without interworking, with the stock r4 callee-saved ABI and without
/// the game's include tree, uniformly for every member.
pub fn runtime_library_cflags() -> Vec<String> {
    cflags()
        .into_iter()
        .filter(|f| f != "-mthumb-interwork" && f != "-fcall-used-r4" && !f.starts_with("-I"))
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
/// The owner a routing source names. Routing sources are the synthetic
/// owner routes (`SourceOwner::routing_path_for_game`), whose stem is the
/// owner's legacy stem; any other path names no owner and routes as game code.
fn routed_owner(source: &str) -> Option<String> {
    SourceOwner::from_legacy_stem(source_stem_ref(source)).map(SourceOwner::id)
}
fn has(table: &'static [&'static str], owner: Option<&str>) -> bool {
    owner.is_some_and(|owner| table.contains(&owner))
}
pub fn family_for_source(target: CompilerTarget, source: &str) -> CompilerFamily {
    let owner = routed_owner(source);
    let owner = owner.as_deref();
    let agbcc: &[&str] = match target {
        CompilerTarget::Tbs => AGBCC_SOURCES,
        CompilerTarget::Tla => TLA_AGBCC_SOURCES,
    };
    if has(agbcc, owner) {
        return CompilerFamily::Agbcc;
    }
    if target == CompilerTarget::Tbs && has(AGBCC_FLASH_SOURCES, owner) {
        return CompilerFamily::AgbccFlash;
    }
    CompilerFamily::Game
}
pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    matches!(
        family_for_source(target, source),
        CompilerFamily::Agbcc | CompilerFamily::AgbccFlash
    )
}
pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    match (family_for_source(target, source), target) {
        (CompilerFamily::Agbcc, _) => agbcc_cflags(),
        (CompilerFamily::AgbccFlash, _) => agbcc_flash_cflags(),
        (CompilerFamily::Game, CompilerTarget::Tbs) => cflags(),
        (CompilerFamily::Game, CompilerTarget::Tla) => base_cflags(CompilerTarget::Tla),
    }
}
#[cfg(test)]
mod target_tests {
    use super::*;
    #[test]
    fn installed_binutils_lead_path_once() {
        let first = Path::new("/repo/tools/out/binutils/bin");
        let current =
            std::env::join_paths(["/usr/local/bin", "/repo/tools/out/binutils/bin", "/usr/bin"])
                .unwrap();
        let path = path_with_first(first, &current).unwrap();
        let entries: Vec<PathBuf> = std::env::split_paths(&path).collect();
        assert_eq!(
            entries,
            [first, Path::new("/usr/local/bin"), Path::new("/usr/bin")]
        );
    }
    #[test]
    fn assembly_uses_historical_soft_float_abi() {
        let command = assembly_command("input.s", "output.o");
        for flag in ["-meabi=gnu", "-mfpu=fpa", "-mfloat-abi=soft"] {
            assert!(command.iter().any(|arg| arg == flag));
        }
        assert_eq!(&command[command.len() - 3..], ["-o", "output.o", "input.s"]);
    }
    #[test]
    fn each_game_uses_its_own_include_tree() {
        let tbs = cflags_for_target(CompilerTarget::Tbs);
        let tla = cflags_for_target(CompilerTarget::Tla);
        assert!(tbs
            .iter()
            .any(|flag| flag.ends_with("/games/THE BROKEN SEAL/INCLUDE")));
        assert!(tla
            .iter()
            .any(|flag| flag.ends_with("/games/THE LOST AGE/INCLUDE")));
        assert!(!tla
            .iter()
            .any(|flag| flag.ends_with("/games/THE BROKEN SEAL/INCLUDE")));
        let shared: Vec<&String> = tbs
            .iter()
            .filter(|flag| *flag != "-mthumb-interwork" && !flag.starts_with("-I"))
            .collect();
        let derived: Vec<&String> = tla
            .iter()
            .filter(|flag| *flag != "-mgs2" && !flag.starts_with("-I"))
            .collect();
        assert_eq!(shared, derived);
        for flags in [&tbs, &tla] {
            assert!(!flags
                .iter()
                .any(|flag| flag == "-mthumb-inline-register-call"));
        }
    }
    /// The images disagree about interworking, so the two Game routes do too.
    /// TLA keeps the -fcall-used-r4 ABI: only 29 of 1414 measured tla-en
    /// functions save r4, and all of those are inherited Agbcc-family objects.
    #[test]
    fn only_tbs_game_code_interworks() {
        let tbs = cflags_for_target_source(CompilerTarget::Tbs, "080bbb0c.c");
        let tla = cflags_for_target_source(CompilerTarget::Tla, "08120454.c");
        assert!(tbs.iter().any(|flag| flag == "-mthumb-interwork"));
        assert!(!tla.iter().any(|flag| flag == "-mthumb-interwork"));
        assert!(tla.iter().any(|flag| flag == "-mgs2"));
        assert!(!tbs.iter().any(|flag| flag == "-mgs2"));
        assert_eq!(
            bundle_for(CompilerTarget::Tbs),
            bundle_for(CompilerTarget::Tla)
        );
        assert!(!cflags_for_target_source(CompilerTarget::Tla, "081c2168.c")
            .iter()
            .any(|flag| flag == "-mgs2"));
        for flags in [&tbs, &tla] {
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
            "games/THE BROKEN SEAL/src/resource_3ab_c_020007f4.c",
            "games/THE BROKEN SEAL/src/resource_381_c_02002e0c.c",
        ] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Tbs, owner),
                cflags(),
                "per-file override for {owner}"
            );
        }
    }
    #[test]
    fn runtime_library_flags_are_the_stock_canonical_set() {
        let flags = runtime_library_cflags();
        assert!(!flags.iter().any(|flag| flag == "-mthumb-interwork"));
        assert!(!flags.iter().any(|flag| flag == "-fcall-used-r4"));
        assert!(!flags.iter().any(|flag| flag.starts_with("-I")));
        for flag in ["-O2", "-mthumb", "-mcpu=arm7tdmi"] {
            assert!(flags.iter().any(|candidate| candidate == flag));
        }
        // The former overlay soft-float owners now route as ordinary game
        // code: no tracked C claims them; the container builds them.
        assert_eq!(
            family_for_source(
                CompilerTarget::Tbs,
                "games/THE BROKEN SEAL/src/resource_3a7_c_0200142c.c"
            ),
            CompilerFamily::Game
        );
    }
    /// Tables are keyed by canonical owner IDs. Membership is provenance, so an
    /// owner stays listed when its C is retired to assembly.
    #[test]
    fn family_tables_name_canonical_owners() {
        for table in [AGBCC_SOURCES, TLA_AGBCC_SOURCES] {
            for entry in table {
                let owner = SourceOwner::parse(entry).expect("canonical owner id");
                assert_eq!(owner.id(), *entry);
            }
        }
    }
    /// An owner route claims only its own owner: a main address does not
    /// claim the overlay function linked at the same number, nor the other game.
    #[test]
    fn family_follows_the_owner_route() {
        assert_eq!(
            family_for_source(
                CompilerTarget::Tla,
                &SourceOwner::Main(0x081c_2168)
                    .routing_path_for_game("tla")
                    .to_string_lossy()
            ),
            CompilerFamily::Agbcc
        );
        assert_eq!(
            family_for_source(CompilerTarget::Tbs, "081c2168.c"),
            CompilerFamily::Game
        );
        assert_eq!(
            family_for_source(CompilerTarget::Tla, "080fb670.c"),
            CompilerFamily::Game
        );
        assert_eq!(
            family_for_source(CompilerTarget::Tbs, "resource_3a8_c_0200142c.c"),
            CompilerFamily::Game
        );
        assert_eq!(
            family_for_source(CompilerTarget::Tbs, "0200142c.c"),
            CompilerFamily::Game
        );
        assert_eq!(
            family_for_source(CompilerTarget::Tla, "SOUND/FADE_MUSIC_PLAYER.C"),
            CompilerFamily::Game
        );
    }
    #[test]
    fn agbcc_family_has_one_flag_set() {
        for owner in ["080fb670.c", "08006878.c", "080fa514.c"] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Tbs, owner),
                agbcc_cflags()
            );
        }
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Tla, "081c2168.c"),
            agbcc_cflags()
        );
    }
}
