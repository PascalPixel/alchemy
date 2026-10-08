use crate::compiler::routing_data::*;
use std::path::{Component, Path, PathBuf};
use std::sync::OnceLock;
pub fn root() -> &'static Path {
    static ROOT: OnceLock<PathBuf> = OnceLock::new();
    ROOT.get_or_init(|| {
        let current = std::env::current_dir().ok();
        if let Some(found) = current.as_deref().and_then(|dir| {
            dir.ancestors()
                .find(|d| d.join("tools/alchemy/Cargo.toml").is_file() && d.join("games").is_dir())
        }) {
            return found.to_path_buf();
        }
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
pub fn binutils_prefix() -> PathBuf {
    root().join("tools/out/binutils")
}
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
pub fn bundle_for(_target: CompilerTarget) -> PathBuf {
    bundle()
}
pub fn agbcc_driver() -> PathBuf {
    bundle().join("agbcc").join("old_agbcc")
}
pub fn agbcc_arm_driver() -> PathBuf {
    bundle().join("agbcc").join("agbcc_arm")
}
pub fn driver_for_family(family: CompilerFamily) -> Option<PathBuf> {
    match family {
        CompilerFamily::Game => None,
        CompilerFamily::Agbcc | CompilerFamily::AgbccFlash => Some(agbcc_driver()),
        CompilerFamily::AgbccArm | CompilerFamily::GameArm => Some(agbcc_arm_driver()),
    }
}
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
pub fn compiler_assembly_command(source: &str, object: &str, arm: bool) -> Vec<String> {
    let mut command = vec![bundle().join("as").to_string_lossy().into_owned()];
    command.push("-marm7tdmi".to_string());
    if !arm {
        command.push("-mthumb".to_string());
    }
    command.extend(
        ["-mthumb-interwork", "-o", object, source]
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
pub fn game_directory(game: &str) -> &str {
    match game {
        "tbs" => "THE BROKEN SEAL",
        "tla" => "THE LOST AGE",
        _ => game,
    }
}
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerFamily {
    Game,
    Agbcc,
    AgbccFlash,
    AgbccArm,
    /// Game ARM C in either game (GAME_ARM_SOURCES): stock agbcc_arm at the
    /// library ARM setting, with the game's include paths (AGENTS.md K1).
    GameArm,
}
impl CompilerFamily {
    pub fn is_arm(self) -> bool {
        matches!(self, CompilerFamily::AgbccArm | CompilerFamily::GameArm)
    }
    pub fn uses_agbcc_driver(self) -> bool {
        self != CompilerFamily::Game
    }
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
pub(crate) const COMMON_INCLUDE_MODULES: &[&str] = &[
    "BATTLE", "FIELD", "GAME", "GRAPHICS", "LIB", "MENU", "SOUND", "SYSTEM",
];
fn common_include_flags() -> impl Iterator<Item = String> {
    COMMON_INCLUDE_MODULES.iter().map(|module| {
        format!(
            "-I{}",
            root().join("games/COMMON/INCLUDE").join(module).display()
        )
    })
}
/// Include paths an agbcc-driver family searches after the game's own tree:
/// game ARM C reads the same shared modules as the rest of its game's C.
pub(crate) fn family_include_flags(family: CompilerFamily) -> Vec<String> {
    if family == CompilerFamily::GameArm {
        common_include_flags().collect()
    } else {
        Vec::new()
    }
}
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
    if target == CompilerTarget::Tla {
        flags.push("-mtune=arm9tdmi".to_string());
        flags.push("-mthumb-split-constants".to_string());
        flags.push("-mthumb-call-via-lr".to_string());
    }
    flags.push(include_flag(target));
    flags.extend(common_include_flags());
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
pub fn agbcc_arm_cflags() -> Vec<String> {
    [
        "-O2",
        "-mthumb-interwork",
        "-fomit-frame-pointer",
        "-fcall-used-r4",
    ]
    .iter()
    .map(|s| (*s).to_string())
    .collect()
}
pub fn agbcc_flash_cflags() -> Vec<String> {
    ["-mthumb-interwork", "-O"]
        .iter()
        .map(|s| (*s).to_string())
        .collect()
}
pub fn runtime_library_cflags() -> Vec<String> {
    cflags()
        .into_iter()
        .filter(|f| f != "-mthumb-interwork" && f != "-fcall-used-r4" && !f.starts_with("-I"))
        .collect()
}
#[cfg(test)]
pub fn cflags_for_target(target: CompilerTarget) -> Vec<String> {
    base_cflags(target)
}
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
    if has(AGBCC_SOURCES, &source) {
        return CompilerFamily::Agbcc;
    }
    if has(AGBCC_FLASH_SOURCES, &source) {
        return CompilerFamily::AgbccFlash;
    }
    if has(AGBCC_ARM_SOURCES, &source) {
        return CompilerFamily::AgbccArm;
    }
    if has(GAME_ARM_SOURCES, &source) {
        return CompilerFamily::GameArm;
    }
    CompilerFamily::Game
}
pub fn uses_agbcc_compiler(target: CompilerTarget, source: &str) -> bool {
    family_for_source(target, source).uses_agbcc_driver()
}
pub fn is_arm(target: CompilerTarget, source: &str) -> bool {
    family_for_source(target, source).is_arm()
}
pub fn cflags_for_target_source(target: CompilerTarget, source: &str) -> Vec<String> {
    cflags_for_family(target, family_for_source(target, source))
}
pub fn cflags_for_family(target: CompilerTarget, family: CompilerFamily) -> Vec<String> {
    match (family, target) {
        (CompilerFamily::Agbcc, _) => agbcc_cflags(),
        (CompilerFamily::AgbccFlash, _) => agbcc_flash_cflags(),
        (CompilerFamily::AgbccArm | CompilerFamily::GameArm, _) => agbcc_arm_cflags(),
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
            .filter(|flag| {
                *flag != "-mthumb-split-constants"
                    && *flag != "-mthumb-call-via-lr"
                    && *flag != "-mtune=arm9tdmi"
                    && !flag.starts_with("-I")
            })
            .collect();
        assert_eq!(shared, derived);
        for flags in [&tbs, &tla] {
            assert!(!flags
                .iter()
                .any(|flag| flag == "-mthumb-inline-register-call"));
        }
    }
    #[test]
    fn game_code_searches_every_shared_module_after_its_own_tree() {
        let mut modules: Vec<String> = std::fs::read_dir(root().join("games/COMMON/INCLUDE"))
            .unwrap()
            .map(|entry| entry.unwrap())
            .filter(|entry| entry.file_type().unwrap().is_dir())
            .map(|entry| entry.file_name().to_string_lossy().into_owned())
            .collect();
        modules.sort();
        assert_eq!(modules, COMMON_INCLUDE_MODULES);
        for target in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            let flags = cflags_for_target(target);
            let includes: Vec<&String> = flags.iter().filter(|f| f.starts_with("-I")).collect();
            assert_eq!(includes.len(), 1 + modules.len());
            assert!(includes[0].ends_with(&format!("/games/{}/INCLUDE", target.directory())));
            for (flag, module) in includes[1..].iter().zip(&modules) {
                assert!(flag.ends_with(&format!("/games/COMMON/INCLUDE/{module}")));
            }
        }
        for game in ["THE BROKEN SEAL", "THE LOST AGE"] {
            for entry in std::fs::read_dir(root().join("games").join(game).join("INCLUDE")).unwrap()
            {
                let path = entry.unwrap().path();
                let text = std::fs::read_to_string(&path).unwrap_or_default();
                let lines: Vec<&str> = text.lines().filter(|l| !l.trim().is_empty()).collect();
                assert!(
                    !(lines.len() == 1 && lines[0].contains("COMMON/INCLUDE")),
                    "{} only forwards to a shared header",
                    path.display()
                );
            }
        }
    }
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
            assert!(!flags.iter().any(|flag| flag.starts_with("-mgs")));
            assert!(flags.iter().any(|flag| flag == "-fcall-used-r4"));
            assert!(flags.iter().any(|flag| flag == "-mthumb"));
        }
    }
    #[test]
    fn only_library_files_leave_the_game_compiler() {
        for source in AGBCC_SOURCES
            .iter()
            .chain(AGBCC_FLASH_SOURCES)
            .chain(AGBCC_ARM_SOURCES)
        {
            assert!(
                source.contains("/SRC/SOUND/") || source.contains("/SRC/SYSTEM/SAVE/"),
                "{source} is not a library file"
            );
        }
        for target in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            assert_eq!(
                cflags_for_target_source(target, "GAME/FLAGS/GET_BYTE.C"),
                cflags_for_target_source(target, "BATTLE/EFFECT/ANY_FILE.C")
            );
        }
    }
    #[test]
    fn only_tla_game_code_splits_constants() {
        let split = |flags: Vec<String>| flags.iter().any(|flag| flag == "-mthumb-split-constants");
        assert!(split(cflags_for_target_source(
            CompilerTarget::Tla,
            "GAME/FLAGS/GET_BYTE.C"
        )));
        assert!(!split(cflags_for_target_source(
            CompilerTarget::Tbs,
            "MENU/INPUT_CANCEL_SOUND_TICK.C"
        )));
        assert!(!split(cflags()));
        assert!(!split(cflags_for_target_source(
            CompilerTarget::Tla,
            "SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C"
        )));
    }
    #[test]
    fn call_via_lr_calls_through_a_register_inline() {
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("c.c");
        std::fs::write(&source, "void f(void (*g)(void)) { g(); }\n").unwrap();
        let compile = |via: bool| {
            let output = work.path().join(if via { "via.s" } else { "stock.s" });
            let mut arguments: Vec<String> = ["-O2", "-mthumb", "-S", "-o"]
                .iter()
                .map(|s| (*s).to_string())
                .collect();
            arguments.push(output.to_string_lossy().into_owned());
            if via {
                arguments.push("-mthumb-call-via-lr".into());
            }
            arguments.push(source.to_string_lossy().into_owned());
            let argv = crate::compiler::bundle::compiler_command_for_target(
                CompilerTarget::Tla,
                &arguments,
            )
            .unwrap();
            let status = std::process::Command::new(&argv[0])
                .args(&argv[1..])
                .status()
                .unwrap();
            assert!(status.success());
            std::fs::read_to_string(output).unwrap()
        };
        let via = compile(true);
        assert!(via.contains("mov\tlr, r0") && via.contains(".2byte\t0xf800"));
        assert!(compile(false).contains("_call_via_r0"));
        let flags = |t, s| cflags_for_target_source(t, s);
        let has = |f: Vec<String>| f.iter().any(|x| x == "-mthumb-call-via-lr");
        assert!(has(flags(CompilerTarget::Tla, "GAME/FLAGS/GET_BYTE.C")));
        assert!(!has(flags(
            CompilerTarget::Tbs,
            "MENU/INPUT_CANCEL_SOUND_TICK.C"
        )));
    }
    #[test]
    fn split_constants_build_an_odd_constant_with_an_add() {
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("k.c");
        std::fs::write(&source, "int f(void) { return 301; }\n").unwrap();
        let compile = |split: bool| {
            let output = work.path().join(if split { "split.s" } else { "stock.s" });
            let mut arguments: Vec<String> = ["-O2", "-mthumb", "-S", "-o"]
                .iter()
                .map(|s| (*s).to_string())
                .collect();
            arguments.push(output.to_string_lossy().into_owned());
            if split {
                arguments.push("-mthumb-split-constants".into());
            }
            arguments.push(source.to_string_lossy().into_owned());
            let argv = crate::compiler::bundle::compiler_command_for_target(
                CompilerTarget::Tla,
                &arguments,
            )
            .unwrap();
            let status = std::process::Command::new(&argv[0])
                .args(&argv[1..])
                .status()
                .unwrap();
            assert!(status.success());
            std::fs::read_to_string(output).unwrap()
        };
        let split = compile(true);
        assert!(split.contains("mov\tr0, #46") && split.contains("add\tr0, r0, #255"));
        assert!(!split.contains("ldr"));
        let stock = compile(false);
        assert!(stock.contains("ldr\tr0, .L") && stock.contains(".word\t301"));
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
        let library = root().join("games/COMMON/SRC/SOUND");
        for entry in std::fs::read_dir(&library).unwrap() {
            let name = entry.unwrap().file_name().to_string_lossy().into_owned();
            if name.ends_with(".C") {
                let source = format!("games/COMMON/SRC/SOUND/{name}");
                assert!(
                    AGBCC_SOURCES.contains(&source.as_str()),
                    "unrouted library module {source}"
                );
            }
        }
    }
    #[test]
    fn natural_paths_route_the_same_whole_file_and_respect_game_boundaries() {
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
        for target in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            assert_eq!(
                family_for_source(
                    target,
                    "games/COMMON/SRC/SYSTEM/SAVE/FLASH_VERIFY_CALLBACK.C"
                ),
                CompilerFamily::AgbccFlash
            );
            for source in [
                "games/THE BROKEN SEAL/SRC/SYSTEM/SAVE/FLASH_VERIFY_CALLBACK.C",
                "games/THE LOST AGE/SRC/SYSTEM/SAVE/FLASH_VERIFY_CALLBACK.C",
            ] {
                assert_eq!(family_for_source(target, source), CompilerFamily::Game);
            }
            assert_eq!(
                family_for_source(target, "games/COMMON/SRC/SOUND/ENGINE.C"),
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
    fn arm_routines_use_agbcc_arm_and_assemble_as_arm() {
        assert_eq!(
            agbcc_arm_cflags(),
            [
                "-O2",
                "-mthumb-interwork",
                "-fomit-frame-pointer",
                "-fcall-used-r4"
            ]
        );
        assert!(agbcc_arm_driver().ends_with("agbcc/agbcc_arm"));
        let arm = compiler_assembly_command("a.s", "a.o", true);
        let thumb = compiler_assembly_command("a.s", "a.o", false);
        assert!(!arm.contains(&"-mthumb".to_string()));
        assert!(thumb.contains(&"-mthumb".to_string()));
        for source in AGBCC_ARM_SOURCES {
            assert_eq!(
                family_for_source(CompilerTarget::Tbs, source),
                CompilerFamily::AgbccArm
            );
        }
    }
    #[test]
    fn game_arm_compiles_game_arm_files_of_either_game_with_stock_agbcc_arm() {
        assert_eq!(
            driver_for_family(CompilerFamily::GameArm),
            Some(agbcc_arm_driver())
        );
        let games = [CompilerTarget::Tbs, CompilerTarget::Tla];
        for target in games {
            assert_eq!(cflags_for_family(target, CompilerFamily::GameArm), agbcc_arm_cflags());
        }
        assert!(CompilerFamily::GameArm.is_arm());
        let mut files = std::collections::BTreeSet::new();
        for entry in GAME_ARM_SOURCES {
            let owners: &[CompilerTarget] = if entry.starts_with("games/COMMON/SRC/") {
                &games
            } else if entry.starts_with("games/THE BROKEN SEAL/SRC/") {
                &[CompilerTarget::Tbs]
            } else if entry.starts_with("games/THE LOST AGE/SRC/") {
                &[CompilerTarget::Tla]
            } else {
                panic!("{entry} is not game C")
            };
            assert!(entry.ends_with(".C"), "{entry} is not game C");
            assert!(!entry.contains("/SRC/SOUND/") && !entry.contains("/SRC/SYSTEM/SAVE/"));
            assert!(root().join(entry).is_file(), "missing routed source {entry}");
            assert!(files.insert(*entry), "duplicate route for {entry}");
            for table in [AGBCC_SOURCES, AGBCC_FLASH_SOURCES, AGBCC_ARM_SOURCES] {
                assert!(!table.contains(entry), "{entry} has two routes");
            }
            for target in games {
                let routed = owners.contains(&target);
                let family = if routed { CompilerFamily::GameArm } else { CompilerFamily::Game };
                assert_eq!(family_for_source(target, entry), family, "{entry} in {target:?}");
                assert_eq!(is_arm(target, entry), routed);
                if routed {
                    assert_eq!(cflags_for_target_source(target, entry), agbcc_arm_cflags());
                }
            }
        }
        for target in games {
            assert_eq!(family_for_source(target, "GRAPHICS/ANY_FILE.C"), CompilerFamily::Game);
        }
    }
    #[test]
    fn agbcc_families_keep_their_flag_sets() {
        for source in [
            "games/COMMON/SRC/SYSTEM/SAVE/IDENTIFY_FLASH.C",
            "games/COMMON/SRC/SYSTEM/SAVE/FLASH_ERASE_VERIFY.C",
            "games/COMMON/SRC/SYSTEM/SAVE/FLASH_READ_ROUTINES.C",
        ] {
            assert_eq!(
                cflags_for_target_source(CompilerTarget::Tbs, source),
                agbcc_flash_cflags()
            );
        }
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Tbs, "games/COMMON/SRC/SOUND/ENGINE.C"),
            agbcc_cflags()
        );
        assert_eq!(
            cflags_for_target_source(CompilerTarget::Tla, "SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C"),
            agbcc_cflags()
        );
    }
}
