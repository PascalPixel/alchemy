//! Compiler routing; `routing_data` is the sole table source.
//!
//! A whole source file routes to exactly one compiler family and flag set.
//! Compiler decisions name the ordinary build inputs, never functions or
//! calculated owner inventories.
use crate::compiler::routing_data::*;
use std::path::{Component, Path, PathBuf};
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
/// Both games use the licensed agscc source and the same executable bundle,
/// with the same stock options.
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
/// This is a per-game build decision: TBS interworks and TLA does not.
/// TLA's inherited library files keep the Agbcc family's interworking flags.
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
    for flag in ["-mcpu=arm7tdmi", "-nostdinc", "-fcall-used-r4"] {
        flags.push(flag.to_string());
    }
    flags.push(include_flag(target));
    flags
}
pub fn cflags() -> Vec<String> {
    base_cflags(CompilerTarget::Tbs)
}
pub fn agbcc_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O2"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}
/// The flash library's agbcc flags: the library family's set at -O.
pub fn agbcc_flash_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O"]
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
/// Repository, absolute repository, and game-SRC-relative paths all name
/// the same ordinary source file. Synthetic address paths have no family
/// declaration and receive the game default.
fn natural_source(target: CompilerTarget, source: &str) -> Option<PathBuf> {
    let path = Path::new(source);
    let path = if path.is_absolute() {
        path.strip_prefix(root()).ok()?
    } else {
        path
    };
    if path
        .components()
        .any(|part| matches!(part, Component::ParentDir))
    {
        return None;
    }
    let path = path
        .components()
        .filter(|part| !matches!(part, Component::CurDir))
        .collect::<PathBuf>();
    let game = Path::new("games").join(target.directory()).join("SRC");
    if path.starts_with("games") {
        (path.starts_with(&game) || path.starts_with("games/COMMON/SRC")).then_some(path)
    } else {
        Some(game.join(path.strip_prefix("SRC").unwrap_or(&path)))
    }
}
fn has(table: &[&str], source: &Path) -> bool {
    table.iter().any(|entry| source == Path::new(entry))
}
pub fn family_for_source(target: CompilerTarget, source: &str) -> CompilerFamily {
    let Some(source) = natural_source(target, source) else {
        return CompilerFamily::Game;
    };
    if has(AGBCC_SOURCES, &source)
        || (source
            .extension()
            .and_then(|ext| ext.to_str())
            .is_some_and(|ext| ext.eq_ignore_ascii_case("c"))
            && AGBCC_DIRECTORIES
                .iter()
                .any(|directory| source.starts_with(directory)))
    {
        return CompilerFamily::Agbcc;
    }
    if has(AGBCC_FLASH_SOURCES, &source) {
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
        let derived: Vec<&String> = tla.iter().filter(|flag| !flag.starts_with("-I")).collect();
        assert_eq!(shared, derived);
        for flags in [&tbs, &tla] {
            assert!(!flags
                .iter()
                .any(|flag| flag == "-mthumb-inline-register-call"));
        }
    }
    /// The games' interworking decisions differ; inherited library files
    /// retain their library flag set.
    #[test]
    fn only_tbs_game_code_interworks() {
        let tbs = cflags_for_target_source(CompilerTarget::Tbs, "MENU/INPUT_CANCEL_SOUND_TICK.C");
        let tla = cflags_for_target_source(CompilerTarget::Tla, "GAME/FLAGS/GET_BYTE.C");
        assert!(tbs.iter().any(|flag| flag == "-mthumb-interwork"));
        assert!(!tla.iter().any(|flag| flag == "-mthumb-interwork"));
        assert_eq!(
            bundle_for(CompilerTarget::Tbs),
            bundle_for(CompilerTarget::Tla)
        );
        for flags in [&tbs, &tla] {
            // No game-specific compiler option exists: both games use stock GCC.
            assert!(!flags.iter().any(|flag| flag.starts_with("-mgs")));
            assert!(flags.iter().any(|flag| flag == "-fcall-used-r4"));
            assert!(flags.iter().any(|flag| flag == "-mthumb"));
        }
    }
    #[test]
    fn game_code_always_compiles_with_the_canonical_flags() {
        for source in [
            "SOUND/COMMAND.C",
            "SOUND/PLAY_PLAY_CUE_RETURN_ONE.C",
            "SYSTEM/SAVE/STATE.C",
            "SYSTEM/SAVE/WRITE_PAIR.C",
            "SYSTEM/SAVE/SUMMARY.C",
            "MENU/INPUT_CANCEL_SOUND_TICK.C",
            "FIELD/ARUTAMIRA_DOU/ROOM_VIS.C",
        ] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Tbs, source),
                cflags(),
                "unexpected compiler family for {source}"
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
    }
    #[test]
    fn family_tables_name_existing_whole_files_without_duplicate_routes() {
        let mut files = std::collections::BTreeSet::new();
        for table in [AGBCC_SOURCES, AGBCC_FLASH_SOURCES] {
            for entry in table {
                assert!(
                    entry.starts_with("games/") && entry.contains("/SRC/") && entry.ends_with(".C")
                );
                assert!(
                    root().join(entry).is_file(),
                    "missing routed source {entry}"
                );
                assert!(files.insert(*entry), "duplicate family for {entry}");
            }
        }
        for directory in AGBCC_DIRECTORIES {
            assert!(root().join(directory).is_dir());
            assert!(!files
                .iter()
                .any(|file| Path::new(file).starts_with(directory)));
        }
    }
    #[test]
    fn natural_paths_route_the_same_whole_file_and_respect_game_boundaries() {
        // The flash library is shared from COMMON, so both games route it.
        let source = "games/COMMON/SRC/SYSTEM/SAVE/IDENTIFY_FLASH.C";
        for target in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            for path in [
                source.to_string(),
                format!("./{source}"),
                root().join(source).to_string_lossy().into_owned(),
            ] {
                assert_eq!(family_for_source(target, &path), CompilerFamily::AgbccFlash);
            }
        }
        assert_eq!(
            family_for_source(
                CompilerTarget::Tbs,
                "games/THE LOST AGE/SRC/SYSTEM/SAVE/FLASH_VERIFY_CALLBACK.C"
            ),
            CompilerFamily::Game
        );
        for target in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            assert_eq!(
                family_for_source(target, "games/COMMON/SRC/SOUND/MUSIC_PLAYER.C"),
                CompilerFamily::Agbcc
            );
            for source in [
                "games/COMMON/SRC/SOUND_EFFECT/SCHEDULE.C",
                "games/COMMON/SRC/SOUND/../GAME/FLAGS.C",
                "games/COMMON/SRC/SOUND/DRIVER/UPDATE.S",
                "08006878.c",
                "resource_3a8_c_0200142c.c",
            ] {
                assert_eq!(
                    family_for_source(target, source),
                    CompilerFamily::Game,
                    "unexpected route for {source}"
                );
            }
        }
    }
    #[test]
    fn agbcc_families_keep_their_flag_sets() {
        assert_eq!(
            cflags_for_target_source(
                CompilerTarget::Tbs,
                "games/COMMON/SRC/SYSTEM/SAVE/IDENTIFY_FLASH.C"
            ),
            agbcc_flash_cflags()
        );
        for source in [
            "games/COMMON/SRC/SOUND/CGB_UPDATE_CHANNELS.C",
            "games/COMMON/SRC/SYSTEM/SAVE/FLASH_ERASE_VERIFY.C",
        ] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Tbs, source),
                agbcc_cflags()
            );
        }
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Tla, "SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C"),
            agbcc_cflags()
        );
    }
}
