use crate::build_rom::c_steps;
use crate::compiler::routing::{family_for_source, root, CompilerFamily};
use crate::process::run as command;
use crate::targets::{decomp_target, DecompTarget};
use crate::elf::Elf;
use crate::ld_map::{Map, Placed, ROM};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy diff DRAFT.c [ADDRESS[,ADDRESS...]] [--game tbs|tla] [--edition EDITION] [--arm] [--rtl] [--heat] [--show]
       alchemy diff OVERLAY_DRAFT.c [--overlay resource_XXX] [--symbol NAME] [--edition EDITION] [--arm] [--rtl] [--show]
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
the overlay defaults from the path (resource_XXX). When no separate listing
object exists, a main-image Thumb scaffold is resolved by ADDRESS or --symbol
in the selected edition's map and ELF. Its reference ROM extent is read only
for comparison; literal pools and jump tables are omitted. ARM or mixed-mode
scaffolds are unsupported. This normalized comparison is analysis only;
the full twelve-edition ROM comparison remains the matching authority.
--arm is an explicit analysis-only study with the pinned game compiler:
replace -mthumb with stock -marm, retaining every other game option. This
is not an admitted build route and earns no credit, even at differ 0.
Study output stays in obj/drafts/arm-study; no build routing is changed.
--rtl keeps the compiler's dump after every pass (GCC's -da) in
out/EDITION/obj/drafts/rtl/, named DRAFT.c.PASS, to find the pass where the
draft's allocation or scheduling first leaves the listing's. The lreg and greg
dumps give each pseudo's allocation priority; with -fsched-verbose=5 the sched
and sched2 dumps say in words why each instruction was scheduled where it is.
--heat compiles the draft once more with -g (analysis only) and prints its
function's lines, each with its count of differing instructions in diff's
own alignment (a listing row counts on the draft line it falls before), and
marks the line where the comparison first diverges with >. Allocation and
scheduling cascade from early differences, so start there."#;

const ARM_STUDY_NOTICE: &str = "ARM study only: pinned game compiler, -mthumb replaced by -marm; all other game options unchanged.\nNot an admitted build route; this comparison earns no credit.";

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    target: DecompTarget,
    show: bool,
    arm_study: bool,
    rtl: bool,
    heat: bool,
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
    let mut rtl = false;
    let mut heat = false;
    let mut overlay = None;
    let mut symbol = None;
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--show" => show = true,
            "--arm" => arm_study = true,
            "--rtl" => rtl = true,
            "--heat" => heat = true,
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
    if addresses.is_empty() && overlay.is_none() && symbol.is_some() {
        addresses.push(symbol.as_ref().unwrap().clone());
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
    if heat && arm_study {
        return Err("--heat maps the game route's code; it does not combine with --arm".into());
    }
    Ok(Options { draft, addresses, game, target, show, arm_study, rtl, heat, overlay, symbol })
}

fn draft_object(output: &Path, stem: &str, arm_study: bool) -> PathBuf {
    let directory = if arm_study { "obj/drafts/arm-study" } else { "obj/drafts" };
    output.join(directory).join(format!("{stem}.o"))
}

pub(crate) fn draft_steps(target: DecompTarget, draft: &Path, object: &Path, arm_study: bool) -> Result<Vec<Vec<String>>, String> {
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
    normalize_with(objdump, &relocations(objdump))
}

/// Each relocation objdump -r prints, as (address, symbol).
fn relocations(objdump: &str) -> BTreeSet<(u32, String)> {
    objdump
        .lines()
        .filter_map(|line| {
            let (address, rest) = line.trim_start().split_once(": R_")?;
            let at = u32::from_str_radix(address, 16).ok()?;
            Some((at, rest.split_whitespace().nth(1)?.to_string()))
        })
        .collect()
}

fn normalize_with(objdump: &str, relocated: &BTreeSet<(u32, String)>) -> Vec<String> {
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
                // A Thumb branch just before a relocated literal word: objdump
                // reads the word's relocation as the branch's own and prints
                // the offset itself against that symbol (`3d0 <Msg+0x3d0>`).
                let symbol = operands.get(1).map(|w| w.trim_start_matches('<').split('+').next().unwrap_or(""));
                let misread = symbol.is_some_and(|s| relocated.contains(&(at + 2, s.to_string())));
                match u32::from_str_radix(operands[0].trim_start_matches("0x"), 16) {
                    Ok(target) if misread => format!("{mnemonic} {:+}", i64::from(target)),
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

pub(crate) fn disassemble(object: &Path, symbol: Option<&str>) -> Result<Vec<String>, String> {
    let object = object.to_string_lossy();
    let only = symbol.map(|s| format!("--disassemble={s}"));
    let mut arguments = vec!["arm-none-eabi-objdump", "-d", "-r", "--no-show-raw-insn"];
    if let Some(only) = only.as_deref() {
        arguments.push(only);
    }
    arguments.push(&object);
    Ok(normalize(&command(&arguments, root())?))
}

/// Each compiled instruction, normalized as `normalize` does, with the
/// source line (file, line) objdump -l prints before it from the line table.
pub(crate) fn normalize_located(objdump: &str) -> Vec<(String, Option<(String, u32)>)> {
    let mut at: Option<(String, u32)> = None;
    let mut out = Vec::new();
    let relocated = relocations(objdump);
    for line in objdump.lines() {
        if let Some(location) = source_location(line) {
            at = Some(location);
            continue;
        }
        out.extend(normalize_with(line, &relocated).into_iter().map(|row| (row, at.clone())));
    }
    out
}

fn source_location(line: &str) -> Option<(String, u32)> {
    if line.starts_with([' ', '\t']) {
        return None;
    }
    let line = line.split(" (discriminator").next()?.trim_end();
    let (file, number) = line.rsplit_once(':')?;
    let number = number.parse().ok()?;
    (!file.is_empty()).then(|| (file.to_string(), number))
}

/// Run a draft plan (ending in assembly) once more with -g on its compile
/// step, for analysis only, and disassemble with agscc's DWARF 2 line
/// table: each instruction and its line in the file named `file_name`, or
/// None for code from another file (a header's inline helper).
pub(crate) fn located(steps: &[Vec<String>], object: &Path, symbol: Option<&str>, file_name: &str) -> Result<Vec<(String, Option<u32>)>, String> {
    for step in steps {
        let mut step = step.clone();
        if step.iter().any(|argument| argument == "-S") {
            step.push("-g".into());
        }
        command(&step, root())?;
    }
    let object = object.to_string_lossy();
    let only = symbol.map(|s| format!("--disassemble={s}"));
    let mut arguments = vec!["arm-none-eabi-objdump", "-d", "-l", "-r", "--no-show-raw-insn"];
    if let Some(only) = only.as_deref() {
        arguments.push(only);
    }
    arguments.push(&object);
    let in_draft = |file: &str| Path::new(file).file_name().is_some_and(|name| name == file_name);
    Ok(normalize_located(&command(&arguments, root())?)
        .into_iter()
        .map(|(row, at)| (row, at.filter(|(file, _)| in_draft(file)).map(|(_, line)| line)))
        .collect())
}

/// Where a draft's code differs from its listing, by source line.
#[derive(Debug, Default, PartialEq, Eq)]
pub(crate) struct Heat {
    /// Differing rows (inserted or deleted, as differ counts them) by line.
    pub lines: BTreeMap<u32, usize>,
    /// The line where the comparison first diverges.
    pub first: Option<u32>,
}

/// Charge each differing row of diff's own alignment to a draft line: a
/// draft row to its own line, a listing row to the draft row it falls
/// before (the last one at the end). A row with no line in the draft (a
/// header's inline code) goes to the nearest following row that has one.
pub(crate) fn heat(listing: &[String], draft: &[(String, Option<u32>)]) -> Heat {
    let rows: Vec<String> = draft.iter().map(|(row, _)| row.clone()).collect();
    let line_at = |j: usize| -> Option<u32> {
        let j = j.min(draft.len());
        draft[j..].iter().find_map(|(_, line)| *line).or_else(|| draft[..j].iter().rev().find_map(|(_, line)| *line))
    };
    let (_, script) = compare(listing, &rows);
    let mut result = Heat::default();
    let mut j = 0;
    for entry in &script {
        let tag = entry.as_bytes()[0];
        if tag == b' ' {
            j += 1;
            continue;
        }
        let line = line_at(j);
        if tag == b'+' {
            j += 1;
        }
        if let Some(line) = line {
            *result.lines.entry(line).or_default() += 1;
            result.first.get_or_insert(line);
        }
    }
    result
}

/// The draft's function, each line with its count of differing rows and
/// the first divergence marked `>`.
fn print_heat(text: &str, heat: &Heat, located: &[(String, Option<u32>)]) {
    let lines: Vec<&str> = text.lines().collect();
    let mapped: Vec<u32> = located.iter().filter_map(|(_, line)| *line).collect();
    let (Some(&low), Some(&high)) = (mapped.iter().min(), mapped.iter().max()) else {
        println!("heat: no instruction carries a line of the draft");
        return;
    };
    // Back to the definition's header: the line before the opening brace.
    let mut start = low as usize;
    while start > 1 && !lines.get(start - 2).is_some_and(|line| line.trim().is_empty() || line.trim_end().ends_with(';') || line.trim_end().ends_with("*/")) {
        start -= 1;
    }
    for number in start..=(high as usize).min(lines.len()) {
        let count = heat.lines.get(&(number as u32)).copied().unwrap_or(0);
        let mark = if heat.first == Some(number as u32) { '>' } else { ' ' };
        let shown = if count > 0 { count.to_string() } else { String::new() };
        println!("{shown:>4} {mark}{number:>5}  {}", lines[number - 1]);
    }
}

fn scaffold_extent(map: &Map, elf: &Elf, wanted: &str) -> Result<(u32, u32, u32, String), String> {
    // STT_ARM_TFUNC (15) is the old assembler's Thumb function alias type.
    let functions: Vec<(u32, String)> = elf.symbols.iter()
        .filter(|symbol| matches!(symbol.kind, 2 | 15) && !symbol.name.starts_with('$'))
        .map(|symbol| (symbol.value & !1, symbol.name.clone()))
        .collect();
    let (section, start, name) = crate::show::resolve(map, Some(elf), &functions, wanted)?;
    if !section.object().starts_with("recon/") || !section.object().ends_with("/rom.o")
        || !section.name.starts_with(".rom.") || !section.loaded || section.load < ROM {
        return Err(format!("{name}: no main-image scaffold extent; build the selected edition first"));
    }
    let statics: Vec<u32> = functions.iter().map(|(address, _)| *address).collect();
    let size = elf.symbols.iter().find(|symbol| symbol.name == name && matches!(symbol.kind, 2 | 15)).map(|symbol| symbol.size);
    let end = crate::show::function_end(section, start, &statics, size);
    require_thumb_scaffold(elf, section, start, end)?;
    let offset = section.load - ROM + (start - section.address);
    Ok((start, end, start.wrapping_sub(offset), name))
}

fn require_thumb_scaffold(elf: &Elf, section: &Placed, start: u32, end: u32) -> Result<(), String> {
    let thumb = elf.symbols.iter().any(|symbol| (symbol.value & !1) == start
        && (symbol.kind == 15 || (symbol.kind == 2 && symbol.value & 1 != 0)));
    let arm = elf.symbols.iter().any(|symbol| symbol.value >= start && symbol.value < end
        && (symbol.name.starts_with("$a") || (symbol.kind == 2 && symbol.value & 1 == 0)));
    if !thumb || arm || start & 1 != 0 || end <= start || end > section.end() {
        return Err("main-image scaffold comparison supports a bounded Thumb function only; ARM, mixed or unknown ISA is unsupported".into());
    }
    Ok(())
}

fn thumb_reference(file: &Path, image: &[u8], base: u32, start: u32, end: u32) -> Result<Vec<String>, String> {
    if start < base || (end - base) as usize > image.len() {
        return Err("scaffold function runs past the reference image".into());
    }
    let lines = crate::show::thumb_lines(image, base, start, end, &crate::show::Names::new());
    if lines.iter().any(|line| line.text.starts_with(".2byte")) {
        return Err("scaffold contains an unsupported Thumb instruction; no comparison produced".into());
    }
    let instructions: BTreeSet<u32> = lines.iter().filter(|line| !line.wide).map(|line| line.address).collect();
    let output = command(&[
        "arm-none-eabi-objdump", "-D", "-b", "binary", "-marm", "-Mforce-thumb",
        "--no-show-raw-insn", "--disassemble-zeroes",
        &format!("--adjust-vma={base:#x}"), &format!("--start-address={start:#x}"),
        &format!("--stop-address={end:#x}"), &file.to_string_lossy(),
    ], root())?;
    let code: String = output.lines().filter(|line| {
        line.trim_start().split_once(":\t")
            .and_then(|(address, _)| u32::from_str_radix(address, 16).ok())
            .is_some_and(|address| instructions.contains(&address))
    }).map(|line| format!("{line}\n")).collect();
    Ok(normalize(&code))
}

pub(crate) fn scaffold_reference(target: DecompTarget, wanted: &str) -> Result<(Vec<String>, String), String> {
    let output = root().join(target.output_dir);
    let read = |name: &str| std::fs::read(output.join(name))
        .map_err(|error| format!("{}: {error}; build {} first", output.join(name).display(), target.id));
    let map = Map::parse(&String::from_utf8_lossy(&read(&format!("{}.map", target.id))?));
    let elf = Elf::parse(&read(&format!("{}.elf", target.id))?)?;
    let (start, end, base, name) = scaffold_extent(&map, &elf, wanted)?;
    let reference = read("baserom.gba")?;
    target.verify_reference(&reference)?;
    let tokens = thumb_reference(&output.join("baserom.gba"), &reference, base, start, end)?;
    println!("Thumb scaffold analysis: {} {name}, {start:#010x}..{end:#010x}; full ROM comparison remains required.", target.id);
    Ok((tokens, name))
}

fn resource_name(path: &str) -> Option<String> {
    let at = path.find("resource_")?;
    let id = &path[at + 9..at + 12];
    id.chars().all(|c| c.is_ascii_hexdigit()).then(|| format!("resource_{id}"))
}

pub(crate) fn text_symbols(object: &Path) -> Result<Vec<String>, String> {
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

/// PERM macros are the permuter's notation, not C: diff compares only a
/// draft the build could compile.
fn refuse_perm_macros(draft: &Path) -> Result<(), String> {
    let text = std::fs::read_to_string(draft).map_err(|e| format!("{}: {e}", draft.display()))?;
    if crate::permute::perm::has_macros(&text) {
        return Err(format!(
            "{}: the draft uses PERM macros (PERM_GENERAL, PERM_ONEOF, PERM_LINESWAP, PERM_RANDOMIZE or PERM_IGNORE), which only alchemy permute expands; diff a best-N.c it wrote, or remove the macros",
            draft.display()
        ));
    }
    Ok(())
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
    refuse_perm_macros(&draft)?;
    let mut steps = draft_steps(target, &draft, &object, options.arm_study)?;
    if options.rtl {
        steps[0].extend(["-da".into(), "-fsched-verbose=5".into()]);
    }
    let assemble = steps.pop().expect("draft plan ends with assembly");
    for step in steps {
        command(&step, root)?;
    }
    if options.rtl {
        // The 2.96 driver names each dump after the input and writes it in
        // the working directory; gather them beside the draft object.
        let dumps = object.parent().unwrap().join("rtl");
        std::fs::create_dir_all(&dumps).map_err(|e| e.to_string())?;
        let name = draft.file_name().unwrap_or_default().to_string_lossy().into_owned();
        for entry in std::fs::read_dir(root).map_err(|e| e.to_string())?.flatten() {
            let file = entry.file_name().to_string_lossy().into_owned();
            if file.starts_with(&format!("{name}.")) {
                std::fs::rename(entry.path(), dumps.join(&file)).map_err(|e| format!("{file}: {e}"))?;
            }
        }
        println!("pass dumps: {}/{name}.*", dumps.display());
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
            if options.arm_study {
                return Err("--arm needs a separate listing object; ARM main-image scaffold comparison is unsupported".into());
            }
            let wanted = options.symbol.as_deref().unwrap_or(address);
            let (tokens, name) = scaffold_reference(target, wanted)?;
            listing.extend(tokens);
            if options.addresses.len() == 1 && text_symbols(&object)?.contains(&name) {
                draft_symbol = Some(name);
            }
        } else {
            listing.extend(disassemble(&reference, None)?);
        }
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
    if options.heat {
        let heat_object = draft_object(&root.join(target.output_dir), &format!("{stem}-heat"), false);
        let steps = draft_steps(target, &draft, &heat_object, false)?;
        let name = draft.file_name().unwrap_or_default().to_string_lossy().into_owned();
        let located = located(&steps, &heat_object, draft_symbol.as_deref(), &name)?;
        if located.iter().map(|(row, _)| row).ne(compiled.iter()) {
            println!("heat: -g changed the code; the lines below are approximate");
        }
        let text = std::fs::read_to_string(&draft).map_err(|e| format!("{}: {e}", draft.display()))?;
        print_heat(&text, &heat(&listing, &located), &located);
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_branch_before_a_relocated_literal_keeps_its_offset() {
        let objdump = "    1b94:\tbl\t0 <Owner>\n    1b96:\tb.n\t3d0 <MsgDefUp+0x3d0>\n\t...\n\t\t\t1b98: R_ARM_ABS32\tMsgDefUp\n    1f66:\tb.n\t1b96 <F+0x1b96>\n";
        assert_eq!(normalize(objdump), vec!["bl", "b.n +976", "b.n -976"]);
        let rows: Vec<String> = normalize_located(objdump).into_iter().map(|(row, _)| row).collect();
        assert_eq!(rows, vec!["bl", "b.n +976", "b.n -976"]);
    }

    #[test]
    fn scaffold_bounds_use_symbols_and_refuse_arm_or_unknown_modes() {
        let section = Placed {
            name: ".rom.00000100".into(), path: "out/tbs-ja/obj/recon/tbs/ja/rom.o".into(),
            address: ROM + 0x100, load: ROM + 0x100, size: 0x30, loaded: true,
            symbols: vec![(ROM + 0x100, "First".into()), (ROM + 0x118, "Next".into())],
        };
        let map = Map { placed: vec![section.clone()] };
        let symbol = |name: &str, value, kind| crate::elf::Symbol { name: name.into(), value, size: 0, kind, section: 1 };
        let mut elf = Elf { sections: vec![], symbols: vec![symbol("First", ROM + 0x101, 2), symbol("Next", ROM + 0x119, 2)] };
        assert_eq!(scaffold_extent(&map, &elf, "First").unwrap(), (ROM + 0x100, ROM + 0x118, ROM, "First".into()));
        assert_eq!(scaffold_extent(&map, &elf, "08000100").unwrap().1, ROM + 0x118);
        elf.symbols[0].size = 0x10;
        assert_eq!(scaffold_extent(&map, &elf, "First").unwrap().1, ROM + 0x110);
        elf.symbols[0].kind = 15;
        elf.symbols[0].value &= !1;
        assert!(scaffold_extent(&map, &elf, "First").is_ok());
        elf.symbols.push(symbol("$a", ROM + 0x108, 0));
        assert!(scaffold_extent(&map, &elf, "First").unwrap_err().contains("mixed"));
        elf.symbols.pop();
        elf.symbols[0].kind = 2;
        assert!(scaffold_extent(&map, &elf, "First").is_err());
        elf.symbols[0].kind = 0;
        assert!(scaffold_extent(&map, &elf, "First").is_err());
        let mut ordinary = section;
        ordinary.path = "games/THE BROKEN SEAL/SRC/TEST.o".into();
        assert!(scaffold_extent(&Map { placed: vec![ordinary] }, &elf, "First").unwrap_err().contains("no main-image scaffold"));
    }

    #[test]
    fn scaffold_reference_omits_pools_tables_and_adjacent_functions() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("reference.s");
        let object = work.path().join("reference.o");
        let binary = work.path().join("reference.bin");
        std::fs::write(&source, ".syntax unified\n.thumb\n.text\n.global First\n.thumb_func\nFirst:\nldr r0, .Lpool\nb .Lend\nmovs r0, #1\nbx lr\n.align 2\n.Lpool:\n.word 0x12345678\n.Lend:\nmovs r0, #2\nbx lr\n.size First, .-First\n.thumb_func\nNext:\nmovs r0, #99\nbx lr\n").unwrap();
        command(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-o", &object.to_string_lossy(), &source.to_string_lossy()], root()).unwrap();
        command(&["arm-none-eabi-objcopy", "-O", "binary", "--only-section=.text", &object.to_string_lossy(), &binary.to_string_lossy()], root()).unwrap();
        let image = std::fs::read(&binary).unwrap();
        let actual = thumb_reference(&binary, &image, ROM, ROM, ROM + 16).unwrap();
        assert_eq!(actual, disassemble(&object, Some("First")).unwrap());
        assert_eq!(actual.len(), 6);
        assert!(!actual.iter().any(|line| line.contains("#99")));
        assert!(thumb_reference(&binary, &image, ROM, ROM, ROM + 100).is_err());

        let mut image: Vec<u8> = [0x4a05u16, 0x00bb, 0x589b, 0x469f].into_iter().flat_map(u16::to_le_bytes).collect();
        image.extend((ROM + 0x10).to_le_bytes());
        image.extend((ROM + 0x14).to_le_bytes());
        image.extend([0x2001u16, 0x4770, 0x2002, 0x4770].into_iter().flat_map(u16::to_le_bytes));
        image.extend((ROM + 8).to_le_bytes());
        std::fs::write(&binary, &image).unwrap();
        let table = thumb_reference(&binary, &image, ROM, ROM, ROM + 28).unwrap();
        assert_eq!(table.len(), 8);
        assert_eq!(&table[4..], ["movs r0, #1", "bx lr", "movs r0, #2", "bx lr"]);
    }

    #[test]
    fn main_scaffold_can_be_named_explicitly() {
        let args = ["candidate.c", "--edition", "tbs-ja", "--symbol", "SaveMenu_SelectSlot"].map(String::from);
        let parsed = parse(&args).unwrap();
        assert_eq!(parsed.addresses, ["SaveMenu_SelectSlot"]);
        assert_eq!(parsed.target.id.as_str(), "tbs-ja");
    }

    #[test]
    fn arm_study_is_explicit_and_does_not_accept_other_codegen_options() {
        let args = |items: &[&str]| items.iter().map(|item| item.to_string()).collect::<Vec<_>>();
        let default = parse(&args(&["recon/tbs/en/main/0800a0f8.c"])).unwrap();
        assert!(!default.arm_study);
        let study = parse(&args(&["0800a0f8.c", "--arm", "--edition", "tbs-ja"])).unwrap();
        assert!(study.arm_study);
        assert!(!study.rtl);
        assert!(parse(&args(&["08023e70.c", "--rtl"])).unwrap().rtl);
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
    fn drafts_with_perm_macros_are_refused() {
        let work = tempfile::tempdir().unwrap();
        let draft = work.path().join("08000000.c");
        std::fs::write(&draft, "int f(void) { return PERM_ONEOF(1, 2); }\n").unwrap();
        let error = run(&[draft.to_string_lossy().into_owned()]).unwrap_err();
        assert!(error.contains("PERM macros") && error.contains("alchemy permute"), "{error}");
        std::fs::write(&draft, "/* PERM_ONEOF(1, 2) was tried */\nint f(void) { return 1; }\n").unwrap();
        assert!(refuse_perm_macros(&draft).is_ok());
    }

    #[test]
    fn heat_maps_instructions_to_draft_lines_through_the_line_table() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("probe.c");
        std::fs::write(&source, "int g;\nint probe(int a, int b)\n{\n    int c = a * 3;\n    if (b)\n        c += g;\n    return c + b;\n}\n").unwrap();
        let target = decomp_target(Some("tbs-en")).unwrap();
        let object = draft_object(&work.path().join("tbs-en"), "probe", false);
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        let steps = draft_steps(target, &source, &object, false).unwrap();
        for step in &steps {
            command(step, root()).unwrap();
        }
        let plain = disassemble(&object, None).unwrap();
        let located = located(&steps, &object, None, "probe.c").unwrap();
        // -g is analysis only: the code is the same instruction for instruction.
        assert_eq!(located.iter().map(|(row, _)| row.clone()).collect::<Vec<_>>(), plain);
        let line = |text: &str| located.iter().find(|(row, _)| row == text).unwrap_or_else(|| panic!("{text}: {located:?}")).1;
        assert_eq!(line("cmp r1, #0"), Some(5));
        assert_eq!(line("ldr r3, [r3, #0]"), Some(6));
        assert_eq!(line("adds r0, r0, r1"), Some(7));
        assert!(located.iter().all(|(_, line)| line.is_some_and(|line| (3..=8).contains(&line))), "{located:?}");
        // A listing that loads g differently differs on line 6 alone.
        let listing: Vec<String> = plain.iter().map(|row| if row == "ldr r3, [r3, #0]" { "ldrh r3, [r3, #0]".into() } else { row.clone() }).collect();
        let found = heat(&listing, &located);
        assert_eq!((found.lines.into_iter().collect::<Vec<_>>(), found.first), (vec![(6, 2)], Some(6)));
        // A listing row missing from the draft counts on the line it falls before.
        let mut longer = plain.clone();
        let at = plain.iter().position(|row| row == "adds r0, r0, r1").unwrap();
        longer.insert(at, "movs r2, #0".into());
        let found = heat(&longer, &located);
        assert_eq!((found.lines.into_iter().collect::<Vec<_>>(), found.first), (vec![(7, 1)], Some(7)));
        assert_eq!(heat(&plain, &located), Heat::default());
    }

    #[test]
    fn located_rows_follow_objdump_line_markers() {
        let objdump = "00000000 <f>:\nf():\n/work/probe.c:3\n   0:\tpush\t{lr}\n/work/probe.c:4 (discriminator 1)\n   2:\tmovs\tr0, #1\n/work/EVENT.H:12\n   4:\tbl\t0 <Use>\n";
        let rows = normalize_located(objdump);
        assert_eq!(rows, [
            ("push {lr}".to_string(), Some(("/work/probe.c".to_string(), 3))),
            ("movs r0, #1".to_string(), Some(("/work/probe.c".to_string(), 4))),
            ("bl".to_string(), Some(("/work/EVENT.H".to_string(), 12))),
        ]);
        // A row from a header goes to the next row in the draft.
        let found = heat(&["push {lr}".to_string()], &[("push {lr}".into(), Some(3)), ("bl".into(), None), ("movs r0, #1".into(), Some(9))]);
        assert_eq!((found.lines.into_iter().collect::<Vec<_>>(), found.first), (vec![(9, 2)], Some(9)));
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
