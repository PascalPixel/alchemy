use crate::build_rom::c_steps;
use crate::compiler::routing::{family_for_source, root, CompilerFamily};
use crate::process::run as command;
use crate::targets::{decomp_target, DecompTarget};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy diff DRAFT.c [ADDRESS[,ADDRESS...]] [--game tbs|tla] [--edition EDITION] [--arm] [--show]
       alchemy diff OVERLAY_DRAFT.c [--overlay resource_XXX] [--symbol NAME] [--edition EDITION] [--arm] [--show]
Compiles DRAFT.c for --edition (default: the game's English edition) with
the build's own compiler and options, then compares its instructions with
listing objects out/EDITION/obj/recon/<game>/raw/ADDRESS.o (build the
edition first). ADDRESS defaults to the draft's file name; give several,
comma-separated, when the draft holds a nested function and its parent.
The game defaults from the draft's path. Prints the instruction counts and
how many differ; --show prints the comparison, - for the listing and + for
the draft. "differ 0" means the code matches instruction for instruction.
An overlay draft (a path under overlays/) compares with the function of
the same name, or --symbol, in out/EDITION/overlays/OVERLAY_overlay.o;
the overlay defaults from the path (resource_XXX). Main-image scaffolds
without a separate listing object are not supported.
--arm is an explicit analysis-only study with the pinned game compiler:
replace -mthumb with stock -marm, retaining every other game option. This
is not an admitted build route and earns no credit, even at differ 0.
Study output stays in obj/drafts/arm-study; no build routing is changed."#;

const ARM_STUDY_NOTICE: &str = "ARM study only: pinned game compiler, -mthumb replaced by -marm; all other game options unchanged.\nNot an admitted build route; this comparison earns no credit.";

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    target: DecompTarget,
    show: bool,
    arm_study: bool,
    overlay: Option<String>,
    symbol: Option<String>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut draft = None;
    let mut addresses = Vec::new();
    let mut game = None;
    let mut target = None;
    let mut show = false;
    let mut arm_study = false;
    let mut overlay = None;
    let mut symbol = None;
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--show" => show = true,
            "--arm" => arm_study = true,
            "--overlay" => {
                i += 1;
                overlay = Some(arguments.get(i).ok_or_else(|| USAGE.to_string())?.clone());
            }
            "--symbol" => {
                i += 1;
                symbol = Some(arguments.get(i).ok_or_else(|| USAGE.to_string())?.clone());
            }
            "--edition" => {
                i += 1;
                target = Some(decomp_target(Some(arguments.get(i).ok_or_else(|| USAGE.to_string())?))?);
            }
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => Some("tbs"),
                    Some("tla") => Some("tla"),
                    _ => return Err(USAGE.into()),
                }
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value if value.starts_with('-') => return Err(format!("unknown diff option: {value}\n{USAGE}")),
            value if draft.is_none() => draft = Some(PathBuf::from(value)),
            value if addresses.is_empty() => {
                addresses = value.split(',').map(str::to_string).collect()
            }
            _ => return Err(USAGE.into()),
        }
        i += 1;
    }
    let draft = draft.ok_or_else(|| USAGE.to_string())?;
    let path_text = draft.to_string_lossy().into_owned();
    if overlay.is_none() && path_text.contains("/overlays/") {
        overlay = resource_name(&path_text);
    }
    if addresses.is_empty() && overlay.is_none() {
        let stem = draft
            .file_stem()
            .map(|s| s.to_string_lossy().into_owned())
            .unwrap_or_default();
        if stem.len() != 8 || !stem.chars().all(|c| c.is_ascii_hexdigit()) {
            return Err(format!("{}: name the listing address\n{USAGE}", draft.display()));
        }
        addresses.push(stem);
    }
    let text = draft.to_string_lossy();
    let edition_game = target.map(|target| if target.id.as_str().starts_with("tla-") { "tla" } else { "tbs" });
    if matches!((game, edition_game), (Some(game), Some(edition_game)) if game != edition_game) {
        return Err("--game and --edition name different games".into());
    }
    let game = game.or(edition_game).unwrap_or(if text.contains("/tla/") || text.contains("THE LOST AGE") {
        "tla"
    } else {
        "tbs"
    });
    let target = target.unwrap_or(decomp_target(Some(&format!("{game}-en")))?);
    Ok(Options { draft, addresses, game, target, show, arm_study, overlay, symbol })
}

fn draft_object(output: &Path, stem: &str, arm_study: bool) -> PathBuf {
    let directory = if arm_study { "obj/drafts/arm-study" } else { "obj/drafts" };
    output.join(directory).join(format!("{stem}.o"))
}

fn draft_steps(target: DecompTarget, draft: &Path, object: &Path, arm_study: bool) -> Result<Vec<Vec<String>>, String> {
    if arm_study && family_for_source(target.compiler, &draft.to_string_lossy()) != CompilerFamily::Game {
        return Err("--arm studies the pinned game compiler only; library compiler routes are unchanged".into());
    }
    let mut steps = c_steps(target, draft, object)?;
    steps.pop();
    if arm_study {
        // This command-local experiment never changes the production plan or routing.
        if steps.len() != 1 || steps[0].iter().filter(|flag| *flag == "-mthumb").count() != 1 {
            return Err("--arm expected one canonical game compiler command with -mthumb".into());
        }
        for flag in &mut steps[0] {
            if flag == "-mthumb" {
                *flag = "-marm".into();
            }
        }
    }
    let mut assemble = vec!["arm-none-eabi-as".to_string(), "-mcpu=arm7tdmi".into()];
    if target.compiler == crate::compiler::routing::CompilerTarget::Tbs {
        assemble.push("-mthumb-interwork".into());
    }
    assemble.extend([
        "-o".into(), object.to_string_lossy().into_owned(),
        object.with_extension("s").to_string_lossy().into_owned(),
    ]);
    steps.push(assemble);
    Ok(steps)
}

fn study_assembly(step: &[String], root: &Path) -> Result<String, String> {
    // The assembler may succeed after diagnosing invalid ARM register overlap.
    // Keep those diagnostics visible in this study; compiler commands still use
    // the normal address-stable runner above.
    let output = std::process::Command::new(&step[0])
        .args(&step[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("ARM study assembler: {error}"))?;
    let diagnostics = String::from_utf8_lossy(&output.stderr).into_owned();
    if !output.status.success() {
        return Err(format!("ARM study assembler failed: {}", diagnostics.trim()));
    }
    Ok(diagnostics)
}

pub(crate) fn normalize(objdump: &str) -> Vec<String> {
    objdump
        .lines()
        .filter_map(|line| {
            let (address, rest) = line.trim_start().split_once(":\t")?;
            let at = u32::from_str_radix(address, 16).ok()?;
            let text = rest.split(['@', ';']).next().unwrap_or("").trim();
            let mut words = text.split_whitespace();
            let mnemonic = words.next()?;
            let operands: Vec<&str> = words.collect();
            if mnemonic == "bl" || mnemonic == "blx" {
                return Some(mnemonic.to_string());
            }
            let mut out = if is_branch(mnemonic) && !operands.is_empty() {
                match u32::from_str_radix(operands[0], 16) {
                    Ok(target) => format!("{mnemonic} {:+}", i64::from(target) - i64::from(at)),
                    Err(_) => text.to_string(),
                }
            } else {
                operands.iter().fold(mnemonic.to_string(), |mut s, w| {
                    s.push(' ');
                    s.push_str(w);
                    s
                })
            };
            if let Some(cut) = out.find(" <") {
                out.truncate(cut);
                if !is_branch(mnemonic) {
                    out.push_str(" <t>");
                }
            }
            if mnemonic.starts_with('.') || mnemonic == "nop" || out == "movs r0, r0" {
                return None;
            }
            Some(out.split_whitespace().collect::<Vec<_>>().join(" "))
        })
        .collect()
}

fn is_branch(mnemonic: &str) -> bool {
    let base = mnemonic.trim_end_matches(".n").trim_end_matches(".w");
    base == "b"
        || (base.len() == 3
            && base.starts_with('b')
            && [
                "eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc", "hi", "ls", "ge", "lt", "gt", "le",
            ]
            .contains(&&base[1..]))
}

pub(crate) fn compare(listing: &[String], draft: &[String]) -> (usize, Vec<String>) {
    let (n, m) = (listing.len(), draft.len());
    let mut table = vec![vec![0u32; m + 1]; n + 1];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            table[i][j] = if listing[i] == draft[j] {
                table[i + 1][j + 1] + 1
            } else {
                table[i + 1][j].max(table[i][j + 1])
            };
        }
    }
    let (mut i, mut j, mut script) = (0, 0, Vec::new());
    while i < n || j < m {
        if i < n && j < m && listing[i] == draft[j] {
            script.push(format!("  {}", listing[i]));
            i += 1;
            j += 1;
        } else if j < m && (i == n || table[i][j + 1] >= table[i + 1][j]) {
            script.push(format!("+ {}", draft[j]));
            j += 1;
        } else {
            script.push(format!("- {}", listing[i]));
            i += 1;
        }
    }
    (table[0][0] as usize, script)
}

fn disassemble(object: &Path, symbol: Option<&str>) -> Result<Vec<String>, String> {
    let object = object.to_string_lossy();
    let only = symbol.map(|s| format!("--disassemble={s}"));
    let mut arguments = vec!["arm-none-eabi-objdump", "-d", "-r", "--no-show-raw-insn"];
    if let Some(only) = only.as_deref() {
        arguments.push(only);
    }
    arguments.push(&object);
    Ok(normalize(&command(&arguments, root())?))
}

fn resource_name(path: &str) -> Option<String> {
    let at = path.find("resource_")?;
    let id = &path[at + 9..at + 12];
    id.chars().all(|c| c.is_ascii_hexdigit()).then(|| format!("resource_{id}"))
}

fn text_symbols(object: &Path) -> Result<Vec<String>, String> {
    let listing = command(&["arm-none-eabi-nm", "--defined-only", &object.to_string_lossy()], root())?;
    Ok(listing
        .lines()
        .filter_map(|l| {
            let mut words = l.split_whitespace();
            let (_, kind, name) = (words.next()?, words.next()?, words.next()?);
            matches!(kind, "T" | "t").then(|| name.to_string())
        })
        .filter(|n| !n.starts_with('$') && !n.starts_with(".L"))
        .collect())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse(arguments)?;
    if options.arm_study {
        println!("{ARM_STUDY_NOTICE}");
    }
    let root = root();
    let target = options.target;
    let stem = options
        .draft
        .file_stem()
        .map(|s| s.to_string_lossy().into_owned())
        .unwrap_or_default();
    let object = draft_object(&root.join(target.output_dir), &stem, options.arm_study);
    std::fs::create_dir_all(object.parent().unwrap()).map_err(|e| e.to_string())?;
    let draft = std::fs::canonicalize(&options.draft)
        .map_err(|e| format!("{}: {e}", options.draft.display()))?;
    let mut steps = draft_steps(target, &draft, &object, options.arm_study)?;
    let assemble = steps.pop().expect("draft plan ends with assembly");
    for step in steps {
        command(&step, root)?;
    }
    if options.arm_study {
        eprint!("{}", study_assembly(&assemble, root)?);
    } else {
        command(&assemble, root)?;
    }
    let mut listing = Vec::new();
    let mut draft_symbol = None;
    if let Some(overlay) = &options.overlay {
        let reference = root.join(target.output_dir).join(format!("overlays/{overlay}_overlay.o"));
        if !reference.exists() {
            return Err(format!("{}: no overlay listing object; build {} first", reference.display(), target.id));
        }
        let symbol = match &options.symbol {
            Some(s) => s.clone(),
            None => {
                let ours = text_symbols(&reference)?;
                let shared: Vec<String> = text_symbols(&object)?.into_iter().filter(|s| ours.contains(s)).collect();
                let stem = options.draft.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
                let address = stem.rsplit('_').next().unwrap_or("").to_ascii_lowercase();
                let by_address: Vec<String> = if address.len() == 8 { ours.iter().filter(|s| s.to_ascii_lowercase().ends_with(&address)).cloned().collect() } else { Vec::new() };
                match (shared.as_slice(), by_address.as_slice()) {
                    ([one], _) | ([], [one]) => one.clone(),
                    _ => return Err(format!("name the listing function with --symbol (the draft and {overlay} share {} names)", shared.len())),
                }
            }
        };
        listing.extend(disassemble(&reference, Some(&symbol))?);
        if text_symbols(&object)?.contains(&symbol) {
            draft_symbol = Some(symbol);
        }
    }
    for address in &options.addresses {
        let reference = root
            .join(target.output_dir)
            .join(format!("obj/recon/{}/raw/{address}.o", options.game));
        if !reference.exists() {
            return Err(format!(
                "{}: no listing object for {}; build that edition first (main-image scaffolds without a separate listing are unsupported)",
                reference.display(),
                target.id
            ));
        }
        listing.extend(disassemble(&reference, None)?);
    }
    let compiled = disassemble(&object, draft_symbol.as_deref())?;
    let (common, script) = compare(&listing, &compiled);
    println!(
        "listing {} instructions, draft {}, common {common}, differ {}",
        listing.len(),
        compiled.len(),
        listing.len() + compiled.len() - 2 * common
    );
    if options.show {
        for line in script {
            println!("{line}");
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn arm_study_is_explicit_and_does_not_accept_other_codegen_options() {
        let args = |items: &[&str]| items.iter().map(|item| item.to_string()).collect::<Vec<_>>();
        let default = parse(&args(&["recon/tbs/en/main/0800a0f8.c"])).unwrap();
        assert!(!default.arm_study);
        let study = parse(&args(&["0800a0f8.c", "--arm", "--edition", "tbs-ja"])).unwrap();
        assert!(study.arm_study);
        assert_eq!(study.target.id.as_str(), "tbs-ja");
        for flag in ["-O1", "-fomit-frame-pointer", "-marm", "--cflags"] {
            assert!(parse(&args(&["0800a0f8.c", "--arm", flag])).is_err(), "{flag}");
        }
        let output = Path::new("out/tbs-ja");
        assert_eq!(draft_object(output, "0800a0f8", false), output.join("obj/drafts/0800a0f8.o"));
        assert_eq!(draft_object(output, "0800a0f8", true), output.join("obj/drafts/arm-study/0800a0f8.o"));
        assert!(ARM_STUDY_NOTICE.contains("Not an admitted build route"));
    }

    #[test]
    fn arm_study_changes_only_the_game_instruction_set() {
        for edition in ["tbs-ja", "tbs-en", "tla-ja", "tla-it"] {
            let target = decomp_target(Some(edition)).unwrap();
            let source = root().join("recon").join(&edition[..3]).join("en/main/08000000.c");
            let object = root().join(target.output_dir).join("obj/drafts/test.o");
            let production = c_steps(target, &source, &object).unwrap();
            let default = draft_steps(target, &source, &object, false).unwrap();
            assert_eq!(default[..default.len() - 1], production[..production.len() - 1]);
            let study = draft_steps(target, &source, &object, true).unwrap();
            let mut expected = default.clone();
            for flag in &mut expected[0] {
                if flag == "-mthumb" {
                    *flag = "-marm".into();
                }
            }
            assert_eq!(study, expected);
            assert_eq!(c_steps(target, &source, &object).unwrap(), production);
        }
        let target = decomp_target(Some("tbs-en")).unwrap();
        let library = root().join("games/THE BROKEN SEAL/SRC/SOUND/MUSIC_TRACK_OPERATE_WORK_BYTE.C");
        assert!(draft_steps(target, &library, Path::new("out/tbs-en/obj/drafts/test.o"), true)
            .unwrap_err().contains("library compiler routes are unchanged"));
    }

    #[test]
    fn study_compiles_arm_while_default_and_production_stay_thumb() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("study.c");
        std::fs::write(&source, "\
#if !defined(TBS_EDITION_JA) && !defined(TLA_EDITION_IT)
#error wrong edition
#endif
#ifdef __thumb__
int study(void) { return 23; }
#else
int study(void) { return 17; }
#endif
").unwrap();
        for edition in ["tbs-ja", "tla-it"] {
            let target = decomp_target(Some(edition)).unwrap();
            for arm in [true, false] {
                let object = draft_object(&work.path().join(edition), "study", arm);
                std::fs::create_dir_all(object.parent().unwrap()).unwrap();
                for step in draft_steps(target, &source, &object, arm).unwrap() {
                    command(&step, root()).unwrap();
                }
                let elf = crate::elf::Elf::parse(&std::fs::read(&object).unwrap()).unwrap();
                let mode = if arm { "$a" } else { "$t" };
                assert!(elf.symbols.iter().any(|symbol| symbol.name == mode), "{edition}: {mode}");
                let instructions = disassemble(&object, None).unwrap();
                assert!(instructions.iter().any(|line| line.ends_with(if arm { "#17" } else { "#23" })), "{instructions:?}");
                let production = c_steps(target, &source, &object).unwrap();
                assert!(production[0].iter().any(|flag| flag == "-mthumb"));
                assert!(!production[0].iter().any(|flag| flag == "-marm"));
            }
        }
    }

    #[test]
    fn arm_study_keeps_successful_assembler_warnings_and_rejects_errors() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let assembly = work.path().join("overlap.s");
        let object = work.path().join("overlap.o");
        let step = vec![
            "arm-none-eabi-as".into(), "-mcpu=arm7tdmi".into(),
            "-o".into(), object.to_string_lossy().into_owned(),
            assembly.to_string_lossy().into_owned(),
        ];
        std::fs::write(&assembly, ".arm\nsmull r0, r2, r0, r3\n").unwrap();
        let diagnostics = study_assembly(&step, root()).unwrap();
        assert!(object.exists());
        assert!(diagnostics.contains("rdhi, rdlo and rm must all be different"), "{diagnostics}");
        std::fs::write(&assembly, ".arm\nnot_an_instruction\n").unwrap();
        assert!(study_assembly(&step, root()).unwrap_err().contains("ARM study assembler failed"));
    }

    #[test]
    fn editions_select_the_existing_build_target() {
        let args = |items: &[&str]| items.iter().map(|item| item.to_string()).collect::<Vec<_>>();
        let default = parse(&args(&["recon/tla/en/main/08123456.c"])).unwrap();
        assert_eq!(default.target.id.as_str(), "tla-en");
        let japanese = parse(&args(&[
            "recon/tbs/ja/overlays/resource_3b7_c_02000ac8.c", "--edition", "tbs-ja",
        ])).unwrap();
        assert_eq!(japanese.target.id.as_str(), "tbs-ja");
        assert_eq!(japanese.target.edition_define, "TBS_EDITION_JA");
        assert_eq!(japanese.target.output_dir, "out/tbs-ja");
        assert_eq!(japanese.overlay.as_deref(), Some("resource_3b7"));
        assert!(parse(&args(&["08123456.c", "--game", "tla", "--edition", "tbs-ja"])).is_err());
        assert!(parse(&args(&["08123456.c", "--edition", "tbs-xx"])).is_err());
    }

    #[test]
    fn overlay_comes_from_the_draft_path() {
        assert_eq!(resource_name("recon/tbs/en/overlays/resource_383_c_020048c8.c").as_deref(), Some("resource_383"));
        assert_eq!(resource_name("recon/tbs/en/overlays/resource_3cd/02008768.c").as_deref(), Some("resource_3cd"));
        assert_eq!(resource_name("recon/tbs/en/main/08011bf4.c"), None);
    }

    #[test]
    fn branches_compare_by_distance_and_calls_by_shape() {
        let listing = "   0:\tpush\t{r5, lr}\n   2:\tbl\t40 <Foo>\n   6:\tbgt.n\te <x+0xe>\n   8:\tldr\tr3, [pc, #4]\t@ (10 <x+0x10>)\n   a:\tnop\t\t\t@ (mov r8, r8)\n";
        let draft = "   0:\tpush\t{r5, lr}\n   2:\tbl\t80 <Bar>\n   6:\tbgt.n\t12 <x+0x12>\n   8:\tldr\tr3, [pc, #4]\t@ (10 <x+0x10>)\n";
        let (a, b) = (normalize(listing), normalize(draft));
        assert_eq!(a, ["push {r5, lr}", "bl", "bgt.n +8", "ldr r3, [pc, #4]"]);
        let (common, script) = compare(&a, &b);
        assert_eq!(common, 3);
        assert!(script.contains(&"- bgt.n +8".to_string()));
        assert!(script.contains(&"+ bgt.n +12".to_string()));
    }
}
