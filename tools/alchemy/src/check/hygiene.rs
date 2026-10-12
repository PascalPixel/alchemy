//! Repository hygiene over tracked files: no scratch, build output or stray
//! binaries, no empty files, text in UTF-8, and in games/ and tools/ no
//! CRLF, trailing whitespace or missing final newline.

use std::path::Path;
use std::process::{Command, ExitCode};

const USAGE: &str = "usage: alchemy check hygiene\n\
alchemy check hygiene --fix [--skip PREFIX]...\n\
Fails on tracked .DS_Store, scratch and build output (*.o, *.elf, *.map,\n\
*.log, *.i, backups, out/), binaries other than PNG, WAV, MIDI and PCM4\n\
assets, empty files and text that is not UTF-8; in games/ and recon/, on a\n\
name in capitals other than a preprocessed assembly file (NAME.S) and on a\n\
.S file whose name is not in capitals; and, in games/ and tools/, on CRLF\n\
line endings, trailing whitespace (TSV fields excepted) and a missing final\n\
newline.";

const BINARY: [&str; 4] = ["png", "wav", "mid", "pcm4"];
const SCRATCH: [&str; 14] = [
    ".o", ".elf", ".map", ".log", ".i", ".gba", ".bin", ".lz", ".orig", ".rej", ".swp", ".tmp",
    ".bak", "~",
];

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    if arguments.first().is_some_and(|flag| flag == "--fix") {
        let skip = arguments[1..].iter().filter(|a| *a != "--skip").cloned().collect::<Vec<_>>();
        return match fix(crate::compiler::routing::root(), &skip) {
            Ok(()) => ExitCode::SUCCESS,
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }
    if !arguments.is_empty() {
        println!("{USAGE}");
        return if arguments == ["-h"] || arguments == ["--help"] {
            ExitCode::SUCCESS
        } else {
            ExitCode::from(2)
        };
    }
    let root = crate::compiler::routing::root();
    match scan(root) {
        Ok(problems) if problems.is_empty() => ExitCode::SUCCESS,
        Ok(problems) => {
            for problem in &problems {
                eprintln!("{problem}");
            }
            eprintln!("error: {} tracked file(s) fail repository hygiene", problems.len());
            ExitCode::FAILURE
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

fn scan(root: &Path) -> Result<Vec<String>, String> {
    let output = Command::new("git")
        .args(["ls-files", "-z", "-s"])
        .current_dir(root)
        .output()
        .map_err(|error| format!("git ls-files: {error}"))?;
    let mut problems = Vec::new();
    for record in output.stdout.split(|byte| *byte == 0).filter(|r| !r.is_empty()) {
        let record = String::from_utf8_lossy(record);
        let Some((meta, path)) = record.split_once('\t') else { continue };
        if meta.starts_with("160000") {
            continue;
        }
        // A file deleted in the working tree is not this check's business.
        let Ok(bytes) = std::fs::read(root.join(path)) else { continue };
        problems.extend(reasons(path, &bytes).into_iter().map(|reason| format!("{path}: {reason}")));
    }
    Ok(problems)
}

pub(crate) fn reasons(path: &str, bytes: &[u8]) -> Vec<&'static str> {
    let mut found: Vec<&'static str> = name_case_reason(path).into_iter().collect();
    found.extend(content_reasons(path, bytes));
    found
}

pub(crate) const NAME_CASE_REASON: &str = "file and folder names are lower case; only a preprocessed \
assembly file is in capitals, name and extension (NAME.S)";

/// The case rule for game sources and their scaffolding (S3): every folder
/// and file under games/ and recon/ is named in lower case, except an
/// assembly file the C preprocessor reads, whose capital `.S` extension opts
/// it in, as GCC's driver reads it, and whose whole name is in capitals.
pub(crate) fn name_case_reason(path: &str) -> Option<&'static str> {
    let components: Vec<&str> = path.split('/').collect();
    let [top, folders @ .., leaf] = components.as_slice() else {
        return None;
    };
    if !matches!(*top, "games" | "recon") {
        return None;
    }
    let lower = |name: &str| !name.bytes().any(|byte| byte.is_ascii_uppercase());
    let preprocessed = leaf.rsplit_once('.').filter(|(_, extension)| *extension == "S");
    let leaf_fits = if let Some((stem, _)) = preprocessed {
        !leaf.bytes().any(|byte| byte.is_ascii_lowercase())
            && stem.bytes().any(|byte| byte.is_ascii_uppercase())
    } else {
        lower(leaf)
    };
    (!(leaf_fits && folders.iter().all(|folder| lower(folder)))).then_some(NAME_CASE_REASON)
}

/// The length rule for a source file a tool names (S3): lower-case
/// letters, digits and underscores, eight at most before the extension. A
/// scene's file takes its place's full name and may be longer: a file
/// directly in its game's scene folder (games/<game>/src/scene), its entry
/// and import tables in that folder's `entry` and `import`, and its unit
/// header (games/<game>/include/unit/scene). `alchemy unit` holds the files
/// it names to this; the check reads the case of tracked files and not
/// their length, which pictures, sounds and older sources exceed.
pub(crate) fn name_length_fits(path: &str) -> bool {
    let (folder, leaf) = path.rsplit_once('/').unwrap_or(("", path));
    let stem = leaf.rsplit_once('.').map_or(leaf, |(stem, _)| stem);
    let plain = !stem.is_empty()
        && stem.bytes().all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_');
    let scene = matches!(
        folder.split('/').collect::<Vec<_>>().as_slice(),
        ["games", _, "src", "scene"] | ["games", _, "src", "scene", "entry" | "import"] | ["games", _, "include", "unit", "scene"]
    );
    plain && (stem.len() <= 8 || scene)
}

fn content_reasons(path: &str, bytes: &[u8]) -> Vec<&'static str> {
    let name = path.rsplit('/').next().unwrap_or(path);
    let extension = name.rsplit_once('.').map_or("", |(_, e)| e);
    if name == ".DS_Store" {
        return vec!["Finder metadata"];
    }
    if path.starts_with("out/") || path.starts_with("tools/out/") || SCRATCH.iter().any(|s| name.ends_with(s)) {
        return vec!["scratch or build output"];
    }
    if bytes.is_empty() {
        return vec!["empty file"];
    }
    if BINARY.contains(&extension) {
        return Vec::new();
    }
    if bytes.contains(&0) {
        return vec!["stray binary"];
    }
    let Ok(text) = std::str::from_utf8(bytes) else {
        return vec!["not UTF-8"];
    };
    let mut found = Vec::new();
    if !(path.starts_with("games/") || path.starts_with("tools/")) {
        return found;
    }
    if text.contains("\r\n") {
        found.push("CRLF line endings");
    }
    if !extension.eq_ignore_ascii_case("tsv")
        && text.lines().any(|line| line.ends_with(' ') || line.ends_with('\t'))
    {
        found.push("trailing whitespace");
    }
    if !text.ends_with('\n') {
        found.push("no final newline");
    }
    found
}

/// Fixes line endings, trailing whitespace and final newlines in place,
/// leaving files under the skipped prefixes alone.
fn fix(root: &Path, skip: &[String]) -> Result<(), String> {
    for problem in scan(root)? {
        let (path, reason) = problem.rsplit_once(": ").unwrap_or((&problem, ""));
        if !matches!(reason, "CRLF line endings" | "trailing whitespace" | "no final newline")
            || skip.iter().any(|prefix| path.starts_with(prefix.as_str()))
        {
            continue;
        }
        let file = root.join(path);
        let text = std::fs::read_to_string(&file).map_err(|e| format!("{path}: {e}"))?;
        std::fs::write(&file, fixed(&text)).map_err(|e| format!("{path}: {e}"))?;
    }
    Ok(())
}

/// Rewrites a text file's line endings, trailing whitespace and final
/// newline; used to fix what the check finds.
pub(crate) fn fixed(text: &str) -> String {
    let mut out: String = text
        .replace("\r\n", "\n")
        .lines()
        .map(|line| line.trim_end_matches([' ', '\t']))
        .collect::<Vec<_>>()
        .join("\n");
    out.push('\n');
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn each_kind_of_problem_is_named() {
        assert_eq!(reasons("a/.DS_Store", b"x"), ["Finder metadata"]);
        assert_eq!(reasons("games/x.o", b"x"), ["scratch or build output"]);
        assert_eq!(reasons("games/x.c", b""), ["empty file"]);
        assert!(reasons("games/x.png", b"\0\x89").is_empty());
        assert_eq!(reasons("games/x.dat", b"a\0b"), ["stray binary"]);
        assert_eq!(reasons("games/X.c", b"x\n"), [NAME_CASE_REASON]);
        assert_eq!(reasons("games/x.c", b"\xff\xfe"), ["not UTF-8"]);
        assert_eq!(
            reasons("games/x.c", b"int a; \r\nint b;"),
            ["CRLF line endings", "trailing whitespace", "no final newline"]
        );
        assert!(reasons("games/x.tsv", b"a\t\n").is_empty());
        assert!(reasons("recon/x.s", b"a \n").is_empty());
    }

    #[test]
    fn names_are_lower_case_but_preprocessed_assembly() {
        for path in [
            "games/tbs/src/battle/tgtexec.c",
            "games/tbs/include/chara.h",
            "games/tbs/main.ld",
            "games/tbs/objects.tsv",
            "games/common/src/system/overlays.s",
            "games/tbs/src/system/ROM_HEAD.S",
            "recon/tla/raw/08000630.s",
            "recon/tla/ja/main.ld",
            "tools/alchemy/src/Main.rs",
            "README.md",
        ] {
            assert_eq!(name_case_reason(path), None, "{path}");
        }
        for path in [
            "games/tbs/src/battle/TGTEXEC.C",
            "games/tbs/src/battle/TGTEXEC.c",
            "games/tbs/SRC/battle/tgtexec.c",
            "games/THE BROKEN SEAL/main.ld",
            "games/tbs/src/system/rom_head.S",
            "games/tbs/src/system/Rom_Head.S",
            "games/tbs/src/system/ROM_HEAD.s",
            "games/tbs/src/system/OVERLAY/ROM_HEAD.S",
            "recon/tla/raw/08000630.S",
            "recon/tla/de/MAIN.LD",
        ] {
            assert_eq!(name_case_reason(path), Some(NAME_CASE_REASON), "{path}");
        }
    }

    #[test]
    fn a_scenes_file_takes_its_places_full_name_and_no_other_file_a_long_one() {
        // A scene's file directly in its game's scene folder, its tables
        // and its headers.
        for path in [
            "games/tbs/src/scene/korashiamu_iriguchi.c",
            "games/tbs/src/scene/korashiamu_iriguchi.h",
            "games/tbs/src/scene/runpa_jo_fortress.h",
            "games/tbs/src/scene/korima_hashi_falling_effect.c",
            "games/tbs/src/scene/entry/korashiamu_iriguchi.s",
            "games/tbs/src/scene/import/korashiamu_iriguchi.s",
            "games/tbs/include/unit/scene/korashiamu_iriguchi.h",
            "games/tla/src/scene/korashiamu_iriguchi.c",
        ] {
            assert!(name_length_fits(path), "{path}");
            assert!(reasons(path, b"x\n").is_empty(), "{path}");
        }
        // Eight letters fit anywhere, as they did.
        for path in ["games/tbs/src/menu/title.c", "games/tbs/src/system/rom_head.s", "games/tbs/src/scene/common/effect/psynergy.c", "a/window2.c", "window2.c"] {
            assert!(name_length_fits(path), "{path}");
        }
        // A long name is refused everywhere else: another folder, a folder
        // under the scenes', a folder that only looks like it, and outside
        // a game's sources.
        for path in [
            "games/tbs/src/menu/title/titlescreen.c",
            "games/tbs/src/field/haidia_mura/haidia_mura.c",
            "games/tbs/src/scene/common/effect/psynergy_fx.c",
            "games/tbs/src/scene/entry/more/haidia_mura.s",
            "games/tbs/src/scenes/haidia_mura.c",
            "games/tbs/src/menu/scene/haidia_mura.c",
            "games/tbs/include/unit/menu/haidia_mura.h",
            "games/tbs/include/scene/haidia_mura.h",
            "recon/tbs/src/scene/haidia_mura.c",
            "tools/src/scene/haidia_mura.c",
            "src/scene/haidia_mura.c",
            "a/windowing2.c",
        ] {
            assert!(!name_length_fits(path), "{path}");
        }
        // Length is no licence for capitals, there or anywhere.
        for path in ["games/tbs/src/scene/Haidia_Mura.c", "games/tbs/src/scene/HAIDIA.c", "a/Window.c", "games/tbs/src/scene/.c"] {
            assert!(!name_length_fits(path), "{path}");
        }
        assert_eq!(reasons("games/tbs/src/scene/Haidia_Mura.c", b"x\n"), [NAME_CASE_REASON]);
    }

    #[test]
    fn fixing_leaves_clean_text() {
        let text = fixed("int a; \r\nint b;\t");
        assert_eq!(text, "int a;\nint b;\n");
        assert!(reasons("games/x.c", text.as_bytes()).is_empty());
    }
}
