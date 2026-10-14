//! A C draft compiled as the Makefile compiles the file it would replace:
//! the same compiler, options and edition for its route (K1, K2).

use std::path::{Path, PathBuf};
use std::sync::OnceLock;

/// The checkout: the folder above tools/ that holds games/.
pub fn root() -> &'static Path {
    static ROOT: OnceLock<PathBuf> = OnceLock::new();
    ROOT.get_or_init(|| {
        let current = std::env::current_dir().ok();
        if let Some(found) = current.as_deref().and_then(|folder| {
            folder.ancestors().find(|folder| folder.join("tools/alchemy/Cargo.toml").is_file() && folder.join("games").is_dir())
        }) {
            return found.to_path_buf();
        }
        Path::new(env!("CARGO_MANIFEST_DIR")).parent().and_then(Path::parent).expect("checkout").to_path_buf()
    })
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerTarget {
    Tbs,
    Tla,
}

impl CompilerTarget {
    pub fn as_str(self) -> &'static str {
        match self {
            CompilerTarget::Tbs => "tbs",
            CompilerTarget::Tla => "tla",
        }
    }
}

#[derive(Clone, Copy, PartialEq, Eq, Debug)]
pub enum CompilerFamily {
    Game,
    Agbcc,
    AgbccFlash,
    GameArm,
}

/// The Makefile's SOUND_C, FLASH_C and ARM_C.
const SOUND_C: &[&str] = &["games/common/src/sound/sound.c", "games/common/src/sound/tables.c", "games/tbs/src/sound/engine.c"];
const FLASH_C: &[&str] = &[
    "games/common/src/system/save/flash.c",
    "games/common/src/system/save/sst.c",
    "games/common/src/system/save/atmel.c",
    "games/common/src/system/save/program_flash_sector.c",
];
const ARM_C: &[&str] = &[
    "games/tbs/src/window/moji.c",
    "games/tbs/src/field/update.c",
    "games/tbs/src/field/vertices.c",
    "games/tla/src/graphics/tile/metatile.c",
];

fn relative(source: &str) -> &str {
    let root = root().to_string_lossy();
    source.strip_prefix(root.as_ref()).map_or(source, |rest| rest.trim_start_matches('/'))
}

/// The route the Makefile gives a source: by its path, as the file it would
/// replace when it is a draft named after one.
pub fn family_for_source(_target: CompilerTarget, source: &str) -> CompilerFamily {
    let source = relative(source);
    let tla_sound = source.starts_with("games/tla/src/sound/")
        && (source.ends_with("/engine.c") || source.ends_with("/tail.c") || source.contains("/music_track_"));
    if SOUND_C.contains(&source) || tla_sound {
        CompilerFamily::Agbcc
    } else if FLASH_C.contains(&source) {
        CompilerFamily::AgbccFlash
    } else if ARM_C.contains(&source) {
        CompilerFamily::GameArm
    } else {
        CompilerFamily::Game
    }
}

const COMMON_MODULES: [&str; 8] = ["battle", "field", "game", "graphics", "lib", "menu", "sound", "system"];

fn includes(target: CompilerTarget) -> Vec<String> {
    let mut flags = vec![format!("-I{}/games/{}/include", root().display(), target.as_str())];
    flags.extend(COMMON_MODULES.iter().map(|module| format!("-I{}/games/common/include/{module}", root().display())));
    flags
}

fn tool(name: &str) -> String {
    root().join("tools/out/compilers").join(name).to_string_lossy().into_owned()
}

/// The game's C options (the Makefile's CFLAGS_tbs and CFLAGS_tla).
pub fn cflags(target: CompilerTarget) -> Vec<String> {
    let mut flags: Vec<String> = ["-O2", "-mthumb"].map(String::from).to_vec();
    if target == CompilerTarget::Tbs {
        flags.push("-mthumb-interwork".into());
    }
    flags.extend(["-mcpu=arm7tdmi", "-nostdinc", "-fcall-used-r4"].map(String::from));
    if target == CompilerTarget::Tla {
        flags.extend(["-mtune=arm9tdmi", "-mthumb-split-constants", "-mthumb-call-via-lr"].map(String::from));
    }
    flags
}

/// ARM C's options (GCC 2.95.2).
pub fn arm_cflags() -> Vec<String> {
    ["-O2", "-mthumb-interwork", "-fomit-frame-pointer", "-fcall-used-r4"].map(String::from).to_vec()
}

const CPPFLAGS: &[&str] = &[
    "-lang-c", "-nostdinc", "-Acpu(arm)", "-Amachine(arm)", "-D__CHAR_UNSIGNED__", "-D__OPTIMIZE__",
    "-D__ARM_ARCH_4T__", "-D__APCS_32__", "-D__ARMEL__", "-Darm_elf", "-D__ELF__",
];

/// The commands that compile `source` into `object`, through its own route
/// or `family`: the edition's define and its build folder (for the message
/// ids) come from `define` and `build`.
pub fn steps(
    target: CompilerTarget,
    define: &str,
    build: &Path,
    source: &Path,
    object: &Path,
    family: Option<CompilerFamily>,
) -> Vec<Vec<String>> {
    let text = |path: &Path| path.to_string_lossy().into_owned();
    let (source_text, assembly, preprocessed) = (text(source), text(&object.with_extension("s")), text(&object.with_extension("i")));
    let family = family.unwrap_or_else(|| family_for_source(target, &source_text));
    let edition = [format!("-D{define}=1"), format!("-I{}", build.display())];
    let dumpbase = source.file_name().map(|name| name.to_string_lossy().into_owned()).unwrap_or_default();
    let mut steps = Vec::new();
    match family {
        CompilerFamily::Game => {
            let mut compile = vec![tool("xgcc"), format!("-B{}/", tool(""))];
            compile.extend(cflags(target));
            compile.extend(includes(target));
            compile.extend(edition);
            compile.extend(["-S".into(), "-o".into(), assembly.clone(), source_text]);
            steps.push(compile);
        }
        CompilerFamily::Agbcc | CompilerFamily::AgbccFlash => {
            let mut preprocess = vec![tool("cpp0")];
            preprocess.extend(CPPFLAGS.iter().map(|flag| flag.to_string()));
            preprocess.push(format!("-I{}/games/{}/include", root().display(), target.as_str()));
            preprocess.extend(["-D__GNUC__=2", "-D__GNUC_MINOR__=9", "-D__GNUC_PATCHLEVEL__=0", "-D__THUMBEL__", "-Dthumb", "-D__thumb__"].map(String::from));
            preprocess.extend(edition);
            preprocess.extend([source_text, preprocessed.clone()]);
            steps.push(preprocess);
            let level = if family == CompilerFamily::AgbccFlash { "-O" } else { "-O2" };
            steps.push(vec![tool("agbcc/old_agbcc"), preprocessed, "-dumpbase".into(), dumpbase, "-mthumb-interwork".into(), level.into(), "-o".into(), assembly.clone()]);
        }
        CompilerFamily::GameArm => {
            let mut preprocess = vec![tool("cpp0")];
            preprocess.extend(CPPFLAGS.iter().map(|flag| flag.to_string()));
            preprocess.extend(includes(target));
            preprocess.extend(["-D__GNUC__=2", "-D__GNUC_MINOR__=95", "-D__GNUC_PATCHLEVEL__=2", "-Darm", "-D__arm__"].map(String::from));
            preprocess.extend(edition);
            preprocess.extend([source_text, preprocessed.clone()]);
            steps.push(preprocess);
            let mut compile = vec![tool("gcc2952_arm"), preprocessed, "-dumpbase".into(), dumpbase];
            compile.extend(arm_cflags());
            compile.extend(["-o".into(), assembly.clone()]);
            steps.push(compile);
        }
    }
    let mut assemble = vec![tool("as"), "-marm7tdmi".into()];
    if family != CompilerFamily::GameArm {
        assemble.push("-mthumb".into());
    }
    assemble.extend(["-mthumb-interwork".into(), "-o".into(), text(object), assembly]);
    steps.push(assemble);
    steps
}
