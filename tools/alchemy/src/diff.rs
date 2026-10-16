use crate::compile::{family_for_source, root, CompilerFamily};
use crate::process::run as command;
use crate::targets::{decomp_target, DecompTarget};
use crate::elf::Elf;
use crate::ld_map::{Map, Placed, ROM};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy diff DRAFT.c [ADDRESS[,ADDRESS...]] [--game tbs|tla] [--edition EDITION] [--arm] [--rtl] [--heat] [--show]
       alchemy diff OVERLAY_DRAFT.c [--overlay resource_XXX] [--symbol NAME] [--edition EDITION] [--rtl] [--show]
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
an adopted function absent from that object uses the linked overlay's bounds
and code decoded from our reference ROM, as alchemy show does.
The overlay defaults from the path (resource_XXX). When no separate listing
object exists, a main-image function is resolved by ADDRESS or --symbol
in the selected edition's map and ELF. Its reference ROM extent is read only
for comparison; literal pools and jump tables are omitted. --arm explicitly
selects ARM for scaffolds without instruction-set markers. Mixed-mode
scaffolds are unsupported. This normalized comparison is analysis only;
the full twelve-edition ROM comparison remains the matching authority.
--arm compiles an ARM draft of either game as the build compiles game ARM C
(GAME_ARM_SOURCES, AGENTS.md K1): stock GCC 2.95.2 at the common ARM
setting, assembled as ARM. Credit comes only from source the build links.
Output stays in obj/drafts/arm.
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

pub(crate) const GAME_ARM_NOTICE: &str = "game ARM route: stock GCC 2.95.2 at the common ARM setting.";

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    target: DecompTarget,
    show: bool,
    arm: bool,
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
    let mut arm = false;
    let mut rtl = false;
    let mut heat = false;
    let mut overlay = None;
    let mut symbol = None;
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--show" => show = true,
            "--arm" => arm = true,
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
    if let Some(symbol) = symbol.as_ref().filter(|_| addresses.is_empty() && overlay.is_none()) {
        addresses.push(symbol.clone());
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
    let game = game.or(edition_game).unwrap_or(if text.contains("/tla/") {
        "tla"
    } else {
        "tbs"
    });
    let target = target.unwrap_or(decomp_target(Some(&format!("{game}-en")))?);
    if arm && overlay.is_some() {
        return Err("--arm compiles main-image game ARM C; overlays are Thumb".into());
    }
    Ok(Options { draft, addresses, game, target, show, arm, rtl, heat, overlay, symbol })
}

fn draft_object(output: &Path, stem: &str) -> PathBuf {
    output.join("obj/drafts").join(format!("{stem}.o"))
}

fn arm_object(output: &Path, stem: &str) -> PathBuf {
    output.join("obj/drafts/arm").join(format!("{stem}.o"))
}

pub(crate) fn draft_steps(target: DecompTarget, draft: &Path, object: &Path) -> Result<Vec<Vec<String>>, String> {
    let steps = c_steps(target, draft, object, None);
    Ok(with_draft_assembly(target, object, steps))
}

/// A draft of either game compiled as the build compiles a GAME_ARM_SOURCES file.
pub(crate) fn arm_steps(target: DecompTarget, draft: &Path, object: &Path) -> Result<Vec<Vec<String>>, String> {
    if family_for_source(target.compiler, &draft.to_string_lossy()) != CompilerFamily::Game {
        return Err("--arm compiles game C only; library compiler routes are unchanged".into());
    }
    let steps = c_steps(target, draft, object, Some(CompilerFamily::GameArm));
    Ok(with_draft_assembly(target, object, steps))
}

/// Asks the compiler step for its per-pass dumps: the 2.96 driver is the
/// first step and takes the scheduler's verbose dump too; on the ARM route
/// the first step is cpp0, so the ARM cc1 (the second) takes -da alone,
/// as it knows no -fsched-verbose.
fn rtl_flags(steps: &mut [Vec<String>], arm: bool) {
    if arm {
        steps[1].push("-da".into());
    } else {
        steps[0].extend(["-da".into(), "-fsched-verbose=5".into()]);
    }
}

/// Replaces the build's assembly step with the comparison's own assembler.
fn with_draft_assembly(target: DecompTarget, object: &Path, mut steps: Vec<Vec<String>>) -> Vec<Vec<String>> {
    steps.pop();
    let mut assemble = vec!["arm-none-eabi-as".to_string(), "-mcpu=arm7tdmi".into()];
    if target.compiler == crate::compile::CompilerTarget::Tbs {
        assemble.push("-mthumb-interwork".into());
    }
    assemble.extend([
        "-o".into(), object.to_string_lossy().into_owned(),
        object.with_extension("s").to_string_lossy().into_owned(),
    ]);
    steps.push(assemble);
    steps
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
    normalize_with_state(objdump, relocated, &mut false)
}

fn normalize_with_state(objdump: &str, relocated: &BTreeSet<(u32, String)>, lr_setup: &mut bool) -> Vec<String> {
    // GCC emits the standalone Thumb BL suffix as .short, so the assembler
    // marks it as data. Only recognize it immediately after the LR setup.
    objdump
        .lines()
        .filter_map(|line| {
            let (address, rest) = line.trim_start().split_once(":\t")?;
            let at = u32::from_str_radix(address, 16).ok()?;
            let (raw, rest) = match rest.split_once('\t') {
                Some((raw, text)) if !raw.trim().is_empty() && raw.split_whitespace().all(|word|
                    matches!(word.len(), 4 | 8) && word.chars().all(|c| c.is_ascii_hexdigit())) => (Some(raw.trim()), text),
                _ => (None, rest),
            };
            let text = rest.split(['@', ';']).next().unwrap_or("").trim();
            let mut words = text.split_whitespace();
            let mnemonic = words.next()?;
            let operands: Vec<&str> = words.collect();
            let call_suffix = *lr_setup && mnemonic == ".short" && operands == ["0xf800"];
            *lr_setup = mnemonic == "mov" && operands.first() == Some(&"lr,");
            if call_suffix {
                return Some("bl".into());
            }
            if mnemonic == "bl" || mnemonic == "blx" {
                return Some(mnemonic.to_string());
            }
            let mut out = if is_branch(mnemonic) && !operands.is_empty() {
                // In relocatable objects objdump can borrow a neighbouring
                // relocation's symbol and misprint an ordinary local branch.
                // Its encoded Thumb displacement remains unambiguous.
                let encoded = raw.filter(|raw| raw.len() == 4)
                    .and_then(|raw| u16::from_str_radix(raw, 16).ok())
                    .and_then(|half| match half {
                        h if h & 0xf800 == 0xe000 => Some(((h << 5) as i16 >> 4) as i32 + 4),
                        h if h & 0xf000 == 0xd000 && h & 0x0f00 < 0x0e00 => Some((h as u8 as i8 as i32) * 2 + 4),
                        _ => None,
                    });
                // A Thumb branch just before a relocated literal word: objdump
                // reads the word's relocation as the branch's own and prints
                // the offset itself against that symbol (`3d0 <Msg+0x3d0>`).
                let symbol = operands.get(1).map(|w| w.trim_start_matches('<').split('+').next().unwrap_or(""));
                let misread = symbol.is_some_and(|s| relocated.contains(&(at + 2, s.to_string())));
                match (encoded, u32::from_str_radix(operands[0].trim_start_matches("0x"), 16)) {
                    (Some(delta), _) => format!("{mnemonic} {delta:+}"),
                    (None, Ok(target)) if misread => format!("{mnemonic} {:+}", i64::from(target)),
                    (None, Ok(target)) => format!("{mnemonic} {:+}", i64::from(target) - i64::from(at)),
                    (None, Err(_)) => text.to_string(),
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

/// The scorer needs only the LCS length and the script's initial equal rows.
/// Keep the full table and its tie-breaking in `compare` for printable diffs.
pub(crate) fn compare_counts(listing: &[String], draft: &[String]) -> (usize, usize) {
    let prefix = listing.iter().zip(draft).take_while(|(a, b)| a == b).count();
    let (listing, draft) = (&listing[prefix..], &draft[prefix..]);
    let suffix = listing.iter().rev().zip(draft.iter().rev()).take_while(|(a, b)| a == b).count();
    let (listing, draft) = (&listing[..listing.len() - suffix], &draft[..draft.len() - suffix]);
    if listing.is_empty() || draft.is_empty() {
        return (prefix + suffix, prefix);
    }

    // Equal ends can belong to an LCS even when rows repeat. Only the middle
    // needs dynamic programming, with the shorter slice as the row width.
    let (rows, columns) = if listing.len() < draft.len() { (draft, listing) } else { (listing, draft) };
    let mut previous = vec![0usize; columns.len() + 1];
    let mut current = vec![0usize; columns.len() + 1];
    for row in rows {
        for (j, column) in columns.iter().enumerate() {
            current[j + 1] = if row == column {
                previous[j] + 1
            } else {
                previous[j + 1].max(current[j])
            };
        }
        std::mem::swap(&mut previous, &mut current);
    }
    (prefix + suffix + previous[columns.len()], prefix)
}

pub(crate) fn disassemble(object: &Path, symbol: Option<&str>) -> Result<Vec<String>, String> {
    Ok(normalize(&disassemble_text(object, symbol)?))
}

/// Keep the encoded instructions available to callers that need another
/// view of the same disassembly. Zero suppression is unchanged.
pub(crate) fn disassemble_text(object: &Path, symbol: Option<&str>) -> Result<String, String> {
    let object = object.to_string_lossy();
    let only = symbol.map(|s| format!("--disassemble={s}"));
    let mut arguments = vec!["arm-none-eabi-objdump", "-d", "-r"];
    if let Some(only) = only.as_deref() {
        arguments.push(only);
    }
    arguments.push(&object);
    command(&arguments, root())
}

/// Each compiled instruction, normalized as `normalize` does, with the
/// source line (file, line) objdump -l prints before it from the line table.
pub(crate) fn normalize_located(objdump: &str) -> Vec<(String, Option<(String, u32)>)> {
    let mut at: Option<(String, u32)> = None;
    let mut out = Vec::new();
    let relocated = relocations(objdump);
    let mut lr_setup = false;
    for line in objdump.lines() {
        if let Some(location) = source_location(line) {
            at = Some(location);
            continue;
        }
        out.extend(normalize_with_state(line, &relocated, &mut lr_setup).into_iter().map(|row| (row, at.clone())));
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
        command(&with_line_table(step), root())?;
    }
    let object = object.to_string_lossy();
    let only = symbol.map(|s| format!("--disassemble={s}"));
    let mut arguments = vec!["arm-none-eabi-objdump", "-d", "-l", "-r"];
    if let Some(only) = only.as_deref() {
        arguments.push(only);
    }
    arguments.push(&object);
    let in_draft = |file: &str| Path::new(file).file_name().is_some_and(|name| name == file_name);
    let mut rows = normalize_located(&command(&arguments, root())?);
    if rows.iter().all(|(_, at)| at.is_none()) {
        // objdump -l rejects gcc_arm's table as mangled: its unit length is
        // one byte short of its last end_sequence. The DWARF dumper still
        // reads every row, so place the instructions by address instead.
        let mut plain = vec!["arm-none-eabi-objdump", "-d", "-r"];
        if let Some(only) = only.as_deref() {
            plain.push(only);
        }
        plain.push(&object);
        let table = decoded_lines(&command(&["arm-none-eabi-objdump", "--wide", "--dwarf=decodedline", &object], root())?);
        rows = located_by_address(&command(&plain, root())?, &table);
    }
    Ok(rows
        .into_iter()
        .map(|(row, at)| (row, at.filter(|(file, _)| in_draft(file)).map(|(_, line)| line)))
        .collect())
}

/// The rows of objdump's decoded line table: (address, file, line).
fn decoded_lines(dump: &str) -> Vec<(u32, String, u32)> {
    let mut rows = Vec::new();
    for line in dump.lines() {
        // File name (which may hold spaces), line, address, then optional
        // view and stmt columns.
        let words: Vec<&str> = line.split_whitespace().collect();
        let found = (1..words.len()).rev().find_map(|i| {
            let address = words[i];
            let address = if address == "0" { Some(0) } else { u32::from_str_radix(address.strip_prefix("0x")?, 16).ok() }?;
            let number: u32 = words[i - 1].parse().ok()?;
            let file = words[..i - 1].join(" ");
            (!file.is_empty() && !file.ends_with(':')).then_some((address, file, number))
        });
        rows.extend(found);
    }
    rows
}

/// Each instruction of a plain disassembly, normalized, with the line of
/// the last table row at or before its address.
fn located_by_address(objdump: &str, table: &[(u32, String, u32)]) -> Vec<(String, Option<(String, u32)>)> {
    let mut out = Vec::new();
    for line in objdump.lines() {
        let Some(at) = line.trim_start().split_once(":\t").and_then(|(address, _)| u32::from_str_radix(address, 16).ok()) else {
            continue;
        };
        let place = table.iter().filter(|(address, _, _)| *address <= at).max_by_key(|(address, _, _)| *address).map(|(_, file, number)| (file.clone(), *number));
        out.extend(normalize(line).into_iter().map(|row| (row, place.clone())));
    }
    out
}

/// A compile step with -g added: the driver's (-S) or a direct cc1's
/// (-dumpbase, the library and game ARM routes); other steps unchanged.
fn with_line_table(step: &[String]) -> Vec<String> {
    let mut step = step.to_vec();
    if step.iter().any(|argument| argument == "-S" || argument == "-dumpbase") {
        step.push("-g".into());
    }
    step
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
    scaffold_extent_mode(map, elf, wanted, false)
}

fn scaffold_extent_mode(map: &Map, elf: &Elf, wanted: &str, arm: bool) -> Result<(u32, u32, u32, String), String> {
    // STT_ARM_TFUNC (15) is the old assembler's Thumb function alias type.
    let functions: Vec<(u32, String)> = elf.symbols.iter()
        .filter(|symbol| matches!(symbol.kind, 2 | 15) && !symbol.name.starts_with('$'))
        .map(|symbol| (symbol.value & !1, symbol.name.clone()))
        .collect();
    let (section, start, name) = crate::show::resolve(map, Some(elf), &functions, wanted)?;
    let scaffold = section.object().starts_with("recon/") && section.object().ends_with("/rom.o")
        && section.name.starts_with(".rom.");
    let source = section.object().starts_with("games/")
        && (section.name == ".text" || section.name.starts_with(".text."));
    if !(scaffold || source) || !section.loaded || section.load < ROM {
        return Err(format!("{name}: no main-image code extent; build the selected edition first"));
    }
    let statics: Vec<u32> = functions.iter().map(|(address, _)| *address).collect();
    let size = elf.symbols.iter().find(|symbol| symbol.name == name && matches!(symbol.kind, 2 | 15)).map(|symbol| symbol.size);
    let end = crate::show::function_end(section, start, &statics, size);
    if arm {
        let thumb = elf.symbols.iter().any(|symbol| (symbol.value & !1) >= start && (symbol.value & !1) < end
            && (symbol.name.starts_with("$t") || symbol.kind == 15 || (symbol.kind == 2 && symbol.value & 1 != 0)));
        if thumb || start & 3 != 0 || end & 3 != 0 || end <= start || end > section.end() {
            return Err("ARM scaffold comparison needs word-aligned bounds without Thumb or mixed-mode symbols".into());
        }
    } else {
        require_thumb_scaffold(elf, section, start, end)?;
    }
    let offset = section.load - ROM + (start - section.address);
    Ok((start, end, start.wrapping_sub(offset), name))
}

fn require_thumb_scaffold(elf: &Elf, section: &Placed, start: u32, end: u32) -> Result<(), String> {
    let thumb = elf.symbols.iter().any(|symbol| (symbol.value & !1) == start
        && (symbol.kind == 15 || (symbol.kind == 2 && symbol.value & 1 != 0)));
    let arm = elf.symbols.iter().any(|symbol| symbol.value >= start && symbol.value < end
        && (symbol.name.starts_with("$a") || (symbol.kind == 2 && symbol.value & 1 == 0)));
    if !thumb || arm || start & 1 != 0 || end <= start || end > section.end() {
        return Err("main-image comparison supports a bounded Thumb function only; ARM, mixed or unknown ISA is unsupported".into());
    }
    Ok(())
}

fn thumb_reference(image: &[u8], base: u32, start: u32, end: u32) -> Result<Vec<String>, String> {
    if start < base || end <= start || (end - base) as usize > image.len() {
        return Err("scaffold function runs past the reference image".into());
    }
    let lines = crate::show::thumb_lines(image, base, start, end, &crate::show::Names::new());
    if lines.iter().any(|line| line.text.starts_with(".2byte")) {
        return Err("scaffold contains an unsupported Thumb instruction; no comparison produced".into());
    }
    // Mask only the data in this temporary disassembly view. Otherwise
    // objdump can read a pool's final halfword as a wide Thumb instruction
    // and consume the first real instruction after it. Code is unchanged;
    // pool values remain outside this instruction-only comparison.
    let mut view = image[(start - base) as usize..(end - base) as usize].to_vec();
    for line in lines.iter().filter(|line| line.wide) {
        let at = (line.address - start) as usize;
        view[at..at + line.bytes.len()].fill(0);
    }
    let file = tempfile::NamedTempFile::new().map_err(|error| format!("Thumb analysis file: {error}"))?;
    std::fs::write(file.path(), view).map_err(|error| format!("Thumb analysis file: {error}"))?;
    let instructions: BTreeSet<u32> = lines.iter().filter(|line| !line.wide).map(|line| line.address).collect();
    // Binary objdump interprets a standalone BL suffix plus its successor as
    // Thumb-2 even with -marmv4t. Split at the call using our ARM7 decoder so
    // disassembly resumes at the next real instruction, without changing bytes.
    let calls: Vec<u32> = lines.iter().filter(|line| line.text.starts_with("bl lr"))
        .map(|line| line.address).collect();
    let mut cursor = start;
    let mut code = String::new();
    for boundary in calls.into_iter().chain(std::iter::once(end)) {
        if cursor < boundary {
            let output = command(&[
                "arm-none-eabi-objdump", "-D", "-b", "binary", "-marm", "-Mforce-thumb",
                "--no-show-raw-insn", "--disassemble-zeroes",
                &format!("--adjust-vma={start:#x}"), &format!("--start-address={cursor:#x}"),
                &format!("--stop-address={boundary:#x}"), &file.path().to_string_lossy(),
            ], root())?;
            for line in output.lines().filter(|line| {
                line.trim_start().split_once(":\t")
                    .and_then(|(address, _)| u32::from_str_radix(address, 16).ok())
                    .is_some_and(|address| instructions.contains(&address))
            }) {
                code.push_str(line);
                code.push('\n');
            }
        }
        if boundary < end {
            code.push_str(&format!("{boundary:x}:\tbl lr\n"));
            cursor = boundary + 2;
        }
    }
    Ok(normalize(&code))
}

fn arm_reference(file: &Path, image: &[u8], base: u32, start: u32, end: u32) -> Result<Vec<String>, String> {
    if start < base || end <= start || start & 3 != 0 || end & 3 != 0 || (end - base) as usize > image.len() {
        return Err("ARM scaffold function has invalid reference bounds".into());
    }
    let bytes = &image[(start - base) as usize..(end - base) as usize];
    let (_, pool, table) = arm_tokens(bytes, start);
    let output = command(&[
        "arm-none-eabi-objdump", "-D", "-b", "binary", "-marm",
        "--no-show-raw-insn", "--disassemble-zeroes",
        &format!("--adjust-vma={base:#x}"), &format!("--start-address={start:#x}"),
        &format!("--stop-address={end:#x}"), &file.to_string_lossy(),
    ], root())?;
    let code: String = output.lines().filter(|line| {
        line.trim_start().split_once(":\t")
            .and_then(|(address, _)| u32::from_str_radix(address, 16).ok())
            .filter(|&address| address >= start && address < end)
            .is_some_and(|address| {
                let index = ((address - start) / 4) as usize;
                !pool.contains(&index) && !table.contains(&index)
            })
    }).map(|line| format!("{line}\n")).collect();
    if code.contains(".word") || code.contains("undefined") {
        return Err("scaffold contains an unsupported ARM instruction; no comparison produced".into());
    }
    Ok(normalize(&code))
}

pub(crate) fn scaffold_reference(target: DecompTarget, wanted: &str, arm: bool) -> Result<(Vec<String>, String), String> {
    let output = root().join(target.output_dir);
    let read = |name: &str| std::fs::read(output.join(name))
        .map_err(|error| format!("{}: {error}; build {} first", output.join(name).display(), target.id));
    let map = Map::parse(&String::from_utf8_lossy(&read(&format!("{}.map", target.id))?));
    let elf = Elf::parse(&read(&format!("{}.elf", target.id))?)?;
    let (start, end, base, name) = if arm {
        scaffold_extent_mode(&map, &elf, wanted, true)?
    } else {
        scaffold_extent(&map, &elf, wanted)?
    };
    let reference_path = root().join(target.rom);
    let reference = std::fs::read(&reference_path)
        .map_err(|error| format!("{}: {error}", reference_path.display()))?;
    target.verify_reference(&reference)?;
    let tokens = if arm {
        arm_reference(&reference_path, &reference, base, start, end)?
    } else {
        thumb_reference(&reference, base, start, end)?
    };
    let mode = if arm { "ARM" } else { "Thumb" };
    println!("{mode} main-image analysis: {} {name}, {start:#010x}..{end:#010x}; full ROM comparison remains required.", target.id);
    Ok((tokens, name))
}

fn resource_name(path: &str) -> Option<String> {
    let at = path.find("resource_")?;
    let id = &path[at + 9..at + 12];
    id.chars().all(|c| c.is_ascii_hexdigit()).then(|| format!("resource_{id}"))
}

fn overlay_extent(map: &Map, elf: &Elf, wanted: &str) -> Result<(u32, u32, u32, String), String> {
    let functions: Vec<(u32, String)> = elf.symbols.iter()
        .filter(|symbol| matches!(symbol.kind, 2 | 15) && !matches!(symbol.section, 0 | 0xfff1)
            && !symbol.name.starts_with('$'))
        .map(|symbol| (symbol.value & !1, symbol.name.clone()))
        .collect();
    let (section, start, name) = crate::show::resolve(map, Some(elf), &functions, wanted)?;
    if !(section.name == ".text" || section.name.starts_with(".text.")) {
        return Err(format!("{name}: no overlay code extent"));
    }
    let symbol = elf.symbols.iter().find(|symbol| symbol.name == name
        && matches!(symbol.kind, 2 | 15) && !matches!(symbol.section, 0 | 0xfff1))
        .ok_or_else(|| format!("{name}: no defined overlay function"))?;
    let statics = functions.iter().map(|(address, _)| *address).collect::<Vec<_>>();
    let end = crate::show::function_end(section, start, &statics, Some(symbol.size));
    require_thumb_scaffold(elf, section, start, end)?;
    let base = elf.sections.iter().filter(|section| !section.bytes.is_empty())
        .map(|section| section.address).min().ok_or("overlay ELF has no placed bytes")?;
    Ok((start, end, base, name))
}

pub(crate) fn overlay_reference(target: DecompTarget, overlay: &str, wanted: &str) -> Result<(Vec<String>, String), String> {
    let id = overlay.strip_prefix("resource_").filter(|id| id.len() == 3 && id.bytes().all(|c| c.is_ascii_hexdigit()))
        .ok_or("overlay must be resource_XXX")?;
    let output = root().join(target.output_dir);
    let read = |name: &str| std::fs::read(output.join(name)).map_err(|error| format!("{name}: {error}; build {} first", target.id));
    let map = Map::parse(&String::from_utf8_lossy(&read(&format!("overlays/resource_{id}.map"))?));
    let elf = Elf::parse(&read(&format!("overlays/resource_{id}.elf"))?)?;
    let (start, end, _, name) = overlay_extent(&map, &elf, wanted)?;
    let native = crate::show::overlay_function_reference(target, overlay, &name)?;
    if native.start != start || native.end != end || native.name != name {
        return Err(format!("{name}: overlay analysis bounds disagree"));
    }
    let tokens = thumb_reference(&native.bytes, start, start, end)?;
    println!("Thumb overlay ROM analysis: {} {overlay} {name}, {start:#010x}..{end:#010x}; full ROM comparison remains required.", target.id);
    Ok((tokens, name))
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
        .filter(|n| !n.starts_with('$') && !n.starts_with(".L") && n != ".gcc2_compiled.")
        .collect())
}

/// PERM macros are the permuter's notation, not C: diff compares only a
/// draft the build could compile.
fn refuse_perm_macros(draft: &Path) -> Result<(), String> {
    let text = std::fs::read_to_string(draft).map_err(|e| format!("{}: {e}", draft.display()))?;
    if crate::permute::perm::has_macros(&text) {
        return Err(format!(
            "{}: the draft uses PERM macros, which only alchemy permute expands; diff a best-N.c it wrote, or remove the macros",
            draft.display()
        ));
    }
    Ok(())
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse(arguments)?;
    if options.arm {
        println!("{GAME_ARM_NOTICE}");
    }
    let root = root();
    let target = options.target;
    let stem = options
        .draft
        .file_stem()
        .map(|s| s.to_string_lossy().into_owned())
        .unwrap_or_default();
    let object = if options.arm {
        arm_object(&root.join(target.output_dir), &stem)
    } else {
        draft_object(&root.join(target.output_dir), &stem)
    };
    std::fs::create_dir_all(object.parent().unwrap()).map_err(|e| e.to_string())?;
    let draft = std::fs::canonicalize(&options.draft)
        .map_err(|e| format!("{}: {e}", options.draft.display()))?;
    refuse_perm_macros(&draft)?;
    let mut steps = if options.arm {
        arm_steps(target, &draft, &object)?
    } else {
        draft_steps(target, &draft, &object)?
    };
    if options.rtl {
        rtl_flags(&mut steps, options.arm);
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
    command(&assemble, root)?;
    let mut listing = Vec::new();
    let mut draft_symbol = None;
    if let Some(overlay) = &options.overlay {
        let reference = root.join(target.output_dir).join(format!("overlays/{overlay}_overlay.o"));
        let symbol = match &options.symbol {
            Some(s) => s.clone(),
            None => {
                let mut ours = if reference.exists() { text_symbols(&reference)? } else { Vec::new() };
                let linked = root.join(target.output_dir).join(format!("overlays/{overlay}.elf"));
                if linked.exists() {
                    ours.extend(text_symbols(&linked)?);
                    ours.sort();
                    ours.dedup();
                }
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
        let raw = if reference.exists() { disassemble(&reference, Some(&symbol))? } else { Vec::new() };
        if raw.is_empty() {
            listing.extend(overlay_reference(target, overlay, &symbol)?.0);
        } else {
            listing.extend(raw);
        }
        if text_symbols(&object)?.contains(&symbol) {
            draft_symbol = Some(symbol);
        }
    }
    for address in &options.addresses {
        let reference = root
            .join(target.output_dir)
            .join(format!("obj/recon/{}/raw/{address}.o", options.game));
        if !reference.exists() {
            let wanted = options.symbol.as_deref().unwrap_or(address);
            let (tokens, name) = scaffold_reference(target, wanted, options.arm)?;
            listing.extend(tokens);
            if options.addresses.len() == 1 && text_symbols(&object)?.contains(&name) {
                draft_symbol = Some(name);
            }
        } else {
            listing.extend(disassemble(&reference, None)?);
        }
    }
    if listing.is_empty() {
        return Err("the selected reference function contains no instructions".into());
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
        let heat_object = if options.arm {
            arm_object(&root.join(target.output_dir), &format!("{stem}-heat"))
        } else {
            draft_object(&root.join(target.output_dir), &format!("{stem}-heat"))
        };
        let steps = if options.arm {
            arm_steps(target, &draft, &heat_object)?
        } else {
            draft_steps(target, &draft, &heat_object)?
        };
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

    fn assert_compare_counts(listing: &[String], draft: &[String]) {
        let (common, script) = compare(listing, draft);
        let prefix = script.iter().take_while(|line| line.starts_with("  ")).count();
        assert_eq!(compare_counts(listing, draft), (common, prefix), "{listing:?} / {draft:?}");
    }

    #[test]
    fn compare_counts_matches_empty_equal_ends_and_duplicate_ties() {
        let rows = |text: &str| text.split_whitespace().map(String::from).collect::<Vec<_>>();
        for (listing, draft) in [
            ("", ""), ("", "a b"), ("a b", ""), ("a b", "a b"),
            ("a b", "a b c"), ("a b c", "a b"),
            ("a b c d", "a x c d"), ("a b c", "x b c"),
            ("a b a b", "b a b a"), ("a a b a", "a b a a"),
            ("a a a", "a a"), ("a b a c a", "a a b c a"),
            ("a b", "a x x x x b"), ("a b c", "x y z"),
        ] {
            assert_compare_counts(&rows(listing), &rows(draft));
        }
    }

    #[test]
    fn compare_counts_matches_full_diff_exhaustively() {
        let mut cases = vec![Vec::<String>::new()];
        let mut level = cases.clone();
        for _ in 0..4 {
            let mut next = Vec::new();
            for rows in &level {
                for token in ["a", "b", "c"] {
                    let mut extended = rows.clone();
                    extended.push(token.to_string());
                    next.push(extended);
                }
            }
            cases.extend(next.iter().cloned());
            level = next;
        }
        // All 14,641 pairs, including unequal lengths and both orientations.
        for listing in &cases {
            for draft in &cases {
                assert_compare_counts(listing, draft);
            }
        }
    }

    #[test]
    fn compare_counts_matches_large_near_identical_function_slices() {
        // Synthetic instruction rows, including duplicates after row 2,048.
        let listing: Vec<_> = (0..2105).map(|i| {
            let i = i % 2048;
            format!("ldr r{}, [r{}, #{}]", i % 8, (i / 8) % 8, (i / 64) * 4)
        }).collect();
        let mut replacement = listing.clone();
        replacement[2055] = "bl".into();
        let mut swap = listing.clone();
        swap.swap(2055, 2056);
        let mut insertion = listing.clone();
        insertion.insert(2055, "bl".into());
        let mut deletion = listing.clone();
        deletion.remove(2055);
        let mut two_changes = swap.clone();
        two_changes[0] = "push {lr}".into();
        for draft in [listing.clone(), replacement, swap, insertion, deletion, two_changes] {
            assert_compare_counts(&listing, &draft);
            assert_compare_counts(&draft, &listing);
        }
    }

    #[test]
    fn encoded_thumb_branches_ignore_misleading_objdump_symbols() {
        assert_eq!(normalize("    361e:\te34a     \tb.n\tb80 <Wrong+0x6c>\n    3620:\td1fc     \tbne.n\t0 <Wrong>\n    3622:\te7fc     \tb.n\t0 <Wrong>\n"),
            ["b.n +1688", "bne.n -4", "b.n -4"]);
        assert_eq!(normalize("    12:\tf000 f801 \tbl\t18 <Callee>\n    16:\t2001     \tmovs\tr0, #1\n    18:\t12345678 \t.word\t0x12345678\n"),
            ["bl", "movs r0, #1"]);
    }

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
        let symbol = |name: &str, value, kind| crate::elf::Symbol { name: name.into(), value, size: 0, kind, bind: 1, section: 1 };
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
        assert!(scaffold_extent_mode(&map, &elf, "First", true).is_ok());
        elf.symbols[0].kind = 0;
        assert!(scaffold_extent(&map, &elf, "First").is_err());
        assert!(scaffold_extent_mode(&map, &elf, "First", true).is_ok());
        elf.symbols.push(symbol("$t", ROM + 0x108, 0));
        assert!(scaffold_extent_mode(&map, &elf, "First", true).unwrap_err().contains("mixed"));
        elf.symbols.pop();
        elf.symbols[0].kind = 2;
        elf.symbols[0].size = 0x0e;
        assert!(scaffold_extent_mode(&map, &elf, "First", true).is_err());
        let mut ordinary = section;
        ordinary.path = "games/tbs/src/test.o".into();
        assert!(scaffold_extent(&Map { placed: vec![ordinary] }, &elf, "First").unwrap_err().contains("no main-image code"));
    }

    #[test]
    fn adopted_code_bounds_use_linked_symbols_and_refuse_data() {
        let section = Placed {
            name: ".text".into(), path: "out/tla-en/obj/games/tla/src/battle/summon/definition.o".into(),
            address: ROM + 0x100, load: ROM + 0x100, size: 0x60, loaded: true,
            symbols: vec![(ROM + 0x100, "First".into()), (ROM + 0x128, "Next".into())],
        };
        let symbol = |name: &str, value, size| crate::elf::Symbol {
            name: name.into(), value, size, kind: 2, bind: 1, section: 1,
        };
        let elf = Elf { sections: vec![], symbols: vec![
            symbol("First", ROM + 0x101, 0x24), symbol("Next", ROM + 0x129, 0x20),
        ] };
        let map = Map { placed: vec![section.clone()] };
        assert_eq!(scaffold_extent(&map, &elf, "First").unwrap(), (ROM + 0x100, ROM + 0x124, ROM, "First".into()));
        assert_eq!(scaffold_extent(&map, &elf, "08000100").unwrap().1, ROM + 0x124);
        let mut data = section.clone();
        data.name = ".rodata".into();
        assert!(scaffold_extent(&Map { placed: vec![data] }, &elf, "First").is_err());
        let mut unloaded = section;
        unloaded.loaded = false;
        assert!(scaffold_extent(&Map { placed: vec![unloaded] }, &elf, "First").is_err());
    }

    #[test]
    fn adopted_code_uses_its_linked_extent_and_still_refuses_data_and_unloaded_sections() {
        let section = Placed {
            name: ".text".into(), path: "out/tbs-en/obj/games/tbs/src/system/link/link.o".into(),
            address: ROM + 0x100, load: ROM + 0x100, size: 0x30, loaded: true,
            symbols: vec![(ROM + 0x100, "First".into()), (ROM + 0x118, "Next".into())],
        };
        let elf = Elf { sections: vec![], symbols: vec![
            crate::elf::Symbol { name: "First".into(), value: ROM + 0x101, size: 0x10, kind: 2, bind: 1, section: 1 },
            crate::elf::Symbol { name: "Next".into(), value: ROM + 0x119, size: 0x18, kind: 2, bind: 1, section: 1 },
        ] };
        let mut map = Map { placed: vec![section] };
        assert_eq!(scaffold_extent(&map, &elf, "First").unwrap(), (ROM + 0x100, ROM + 0x110, ROM, "First".into()));
        map.placed[0].name = ".data".into();
        assert!(scaffold_extent(&map, &elf, "First").is_err());
        map.placed[0].name = ".text.First".into();
        assert!(scaffold_extent(&map, &elf, "First").is_ok());
        map.placed[0].loaded = false;
        assert!(scaffold_extent(&map, &elf, "First").is_err());
        map.placed[0].loaded = true;
        map.placed[0].load = 0x03000000;
        assert!(scaffold_extent(&map, &elf, "First").is_err());
    }

    #[test]
    fn overlay_bounds_exclude_imported_functions_data_and_mixed_code() {
        let base = 0x02008000;
        let mut map = Map { placed: vec![Placed {
            name: ".text".into(), path: "out/tbs-en/obj/games/tbs/src/field/scene.o".into(),
            address: base, load: base, size: 0x30, loaded: true,
            symbols: vec![(base, "First".into()), (base + 0x18, "Next".into())],
        }] };
        let mut elf = Elf {
            sections: vec![crate::elf::Section { address: base, bytes: vec![0; 0x30] }],
            symbols: vec![
                crate::elf::Symbol { name: "First".into(), value: base + 1, size: 0x10, kind: 2, bind: 1, section: 1 },
                crate::elf::Symbol { name: "Next".into(), value: base + 0x19, size: 0x18, kind: 2, bind: 1, section: 1 },
                crate::elf::Symbol { name: "Import".into(), value: base + 9, size: 0, kind: 2, bind: 1, section: 0xfff1 },
            ],
        };
        assert_eq!(overlay_extent(&map, &elf, "First").unwrap(), (base, base + 0x10, base, "First".into()));
        assert!(overlay_extent(&map, &elf, "Import").is_err());
        map.placed[0].name = ".rodata".into();
        assert!(overlay_extent(&map, &elf, "First").is_err());
        map.placed[0].name = ".text.First".into();
        elf.symbols[0].section = 0;
        assert!(overlay_extent(&map, &elf, "First").is_err());
        elf.symbols[0].section = 1;
        elf.symbols.push(crate::elf::Symbol { name: "$a".into(), value: base + 4, size: 0, kind: 0, bind: 0, section: 1 });
        assert!(overlay_extent(&map, &elf, "First").is_err());
    }

    #[test]
    fn scaffold_reference_omits_pools_tables_and_adjacent_functions() {
        
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("reference.s");
        let object = work.path().join("reference.o");
        let binary = work.path().join("reference.bin");
        std::fs::write(&source, ".syntax unified\n.thumb\n.text\n.global First\n.thumb_func\nFirst:\nldr r0, .Lpool\nb .Lend\nmovs r0, #1\nbx lr\n.align 2\n.Lpool:\n.word 0x12345678\n.Lend:\nmovs r0, #2\nbx lr\n.size First, .-First\n.thumb_func\nNext:\nmovs r0, #99\nbx lr\n").unwrap();
        command(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-o", &object.to_string_lossy(), &source.to_string_lossy()], root()).unwrap();
        command(&["arm-none-eabi-objcopy", "-O", "binary", "--only-section=.text", &object.to_string_lossy(), &binary.to_string_lossy()], root()).unwrap();
        let image = std::fs::read(&binary).unwrap();
        let actual = thumb_reference(&image, ROM, ROM, ROM + 16).unwrap();
        assert_eq!(actual, disassemble(&object, Some("First")).unwrap());
        assert_eq!(actual.len(), 6);
        assert!(!actual.iter().any(|line| line.contains("#99")));
        assert!(thumb_reference(&image, ROM, ROM, ROM + 100).is_err());

        // A pool word whose last halfword begins a Thumb-2 encoding must
        // not swallow the first instruction after the pool.
        let mut wide_pool = image.clone();
        wide_pool[10..12].copy_from_slice(&0xf000u16.to_le_bytes());
        assert_eq!(thumb_reference(&wide_pool, ROM, ROM, ROM + 16).unwrap(), actual);

        let mut image: Vec<u8> = [0x4a05u16, 0x00bb, 0x589b, 0x469f].into_iter().flat_map(u16::to_le_bytes).collect();
        image.extend((ROM + 0x10).to_le_bytes());
        image.extend((ROM + 0x14).to_le_bytes());
        image.extend([0x2001u16, 0x4770, 0x2002, 0x4770].into_iter().flat_map(u16::to_le_bytes));
        image.extend((ROM + 8).to_le_bytes());
        let table = thumb_reference(&image, ROM, ROM, ROM + 28).unwrap();
        assert_eq!(table.len(), 8);
        assert_eq!(&table[4..], ["movs r0, #1", "bx lr", "movs r0, #2", "bx lr"]);
    }

    #[test]
    fn thumb_reference_keeps_a_standalone_call_via_lr_and_its_successor() {
        
        let work = tempfile::tempdir().unwrap();
        let binary = work.path().join("reference.bin");
        let image: Vec<u8> = [0x469eu16, 0xf800, 0xb003, 0xbd60]
            .into_iter().flat_map(u16::to_le_bytes).collect();
        std::fs::write(&binary, &image).unwrap();
        assert_eq!(thumb_reference(&image, ROM, ROM, ROM + 8).unwrap(),
            ["mov lr, r3", "bl", "add sp, #12", "pop {r5, r6, pc}"]);
        assert_eq!(normalize("0:\tmov lr, r3\n2:\t.short 0xf800\n4:\tadd sp, #12\n6:\tpop {r5, r6, pc}\n"),
            ["mov lr, r3", "bl", "add sp, #12", "pop {r5, r6, pc}"]);
        assert!(normalize("0:\t.short 0xf800\n").is_empty());
        let located = normalize_located("test.c:1\n0:\tmov lr, r3\ntest.c:2\n2:\t.short 0xf800\n4:\tadd sp, #12\n");
        assert_eq!(located.iter().map(|(row, _)| row.as_str()).collect::<Vec<_>>(),
            ["mov lr, r3", "bl", "add sp, #12"]);
        assert_eq!(located[1].1, Some(("test.c".into(), 2)));
    }

    #[test]
    fn sliced_thumb_reference_handles_pools_and_call_suffixes_together() {
        
        let body: Vec<u8> = [0x4802u16, 0xe005, 0x469e, 0xf800, 0xb003, 0xbd60,
            0x1234, 0xf000, 0x2007, 0x4770].into_iter().flat_map(u16::to_le_bytes).collect();
        let expected = thumb_reference(&body, ROM, ROM, ROM + 20).unwrap();
        let mut image = vec![0; 8];
        image.extend(body);
        assert_eq!(thumb_reference(&image, ROM, ROM + 8, ROM + 28).unwrap(), expected);
        assert_eq!(expected.len(), 8);
        assert!(expected.iter().any(|instruction| instruction == "bl"));
        assert!(expected.iter().any(|instruction| instruction == "movs r0, #7"));
    }

    #[test]
    fn main_scaffold_can_be_named_explicitly() {
        let args = ["candidate.c", "--edition", "tbs-ja", "--symbol", "SaveMenu_SelectSlot"].map(String::from);
        let parsed = parse(&args).unwrap();
        assert_eq!(parsed.addresses, ["SaveMenu_SelectSlot"]);
        assert_eq!(parsed.target.id.as_str(), "tbs-ja");
    }

    #[test]
    fn arm_scaffold_reference_omits_pools_and_adjacent_functions() {
        
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("reference.s");
        let object = work.path().join("reference.o");
        let binary = work.path().join("reference.bin");
        std::fs::write(&source, ".syntax unified\n.arm\n.text\n.global First\n.type First,%function\nFirst:\nldr r0, .Lpool\nb .Lend\nmov r0, #1\nbx lr\n.Lpool:\n.word 0x12345678\n.Lend:\nmov r0, #2\nbx lr\n.size First, .-First\n.type Next,%function\nNext:\nmov r0, #99\nbx lr\n").unwrap();
        command(&["arm-none-eabi-as", "-mcpu=arm7tdmi", "-o", &object.to_string_lossy(), &source.to_string_lossy()], root()).unwrap();
        command(&["arm-none-eabi-objcopy", "-O", "binary", "--only-section=.text", &object.to_string_lossy(), &binary.to_string_lossy()], root()).unwrap();
        let image = std::fs::read(&binary).unwrap();
        let actual = arm_reference(&binary, &image, ROM, ROM, ROM + 28).unwrap();
        assert_eq!(actual, disassemble(&object, Some("First")).unwrap());
        assert_eq!(actual.len(), 6);
        assert!(!actual.iter().any(|line| line.contains("#99")));
        assert!(arm_reference(&binary, &image, ROM, ROM, ROM + 100).is_err());
        assert!(arm_reference(&binary, &image, ROM, ROM + 2, ROM + 28).is_err());
        assert!(arm_reference(&binary, &image, ROM, ROM + 28, ROM).is_err());

        let image: Vec<u8> = [0xe79f_f100u32, 0xe1a0_0000, ROM + 16, ROM + 20, 0xe3a0_0001, 0xe12f_ff1e]
            .into_iter().flat_map(u32::to_le_bytes).collect();
        std::fs::write(&binary, &image).unwrap();
        assert_eq!(arm_reference(&binary, &image, ROM, ROM, ROM + 24).unwrap(),
            ["ldr pc, [pc, r0, lsl #2]", "mov r0, #1", "bx lr"]);
    }

    #[test]
    fn rtl_dumps_go_to_the_compiler_step_of_each_route() {
        let plan = || vec![vec!["first".to_string()], vec!["second".to_string()], vec!["as".to_string()]];
        let mut thumb = plan();
        rtl_flags(&mut thumb, false);
        assert_eq!(thumb[0], ["first", "-da", "-fsched-verbose=5"]);
        assert_eq!(thumb[1], ["second"]);
        let mut arm = plan();
        rtl_flags(&mut arm, true);
        assert_eq!(arm[0], ["first"]);
        assert_eq!(arm[1], ["second", "-da"]);
    }

    #[test]
    fn arm_is_the_game_arm_route_through_stock_gcc2952() {
        let args = |items: &[&str]| items.iter().map(|item| item.to_string()).collect::<Vec<_>>();
        assert!(!parse(&args(&["recon/tbs/en/main/0800a0f8.c"])).unwrap().arm);
        assert!(parse(&args(&["recon/tbs/en/main/080158e8.c", "--arm"])).unwrap().arm);
        assert_eq!(parse(&args(&["080388fc.c", "--arm", "--game", "tla"])).unwrap().game, "tla");
        assert!(parse(&args(&["08023e70.c", "--rtl"])).unwrap().rtl);
        for flag in ["-O1", "-fomit-frame-pointer", "-marm", "--cflags", "--camelot-arm"] {
            assert!(parse(&args(&["0800a0f8.c", "--arm", flag])).is_err(), "{flag}");
        }
        let output = Path::new("out/tbs-ja");
        assert_eq!(draft_object(output, "0800a0f8"), output.join("obj/drafts/0800a0f8.o"));
        assert_eq!(arm_object(output, "0800a0f8"), output.join("obj/drafts/arm/0800a0f8.o"));
        let target = decomp_target(Some("tbs-en")).unwrap();
        let source = root().join("recon/tbs/en/main/08000000.c");
        let object = root().join(target.output_dir).join("obj/drafts/arm/test.o");
        let steps = arm_steps(target, &source, &object).unwrap();
        assert_eq!(steps.len(), 3);
        assert!(steps[1][0].ends_with("gcc2952_arm"));
        for flag in crate::compile::arm_cflags() {
            assert!(steps[1].contains(&flag), "{flag}");
        }
        assert!(!steps[2].iter().any(|flag| flag == "-mthumb"));
        let library = root().join("games/tbs/src/sound/engine.c");
        assert!(arm_steps(target, &library, &object).is_err());
        let tla = decomp_target(Some("tla-en")).unwrap();
        let tla_source = root().join("recon/tla/en/main/08000000.c");
        let tla_steps = arm_steps(tla, &tla_source, &object).unwrap();
        assert!(tla_steps[1][0].ends_with("gcc2952_arm"));
        assert!(!tla_steps[2].iter().any(|flag| flag == "-mthumb"));
    }

    #[test]
    fn draft_steps_compile_as_the_build_does() {
        for edition in ["tbs-ja", "tbs-en", "tla-ja", "tla-it"] {
            let target = decomp_target(Some(edition)).unwrap();
            let source = root().join("recon").join(&edition[..3]).join("en/main/08000000.c");
            let object = root().join(target.output_dir).join("obj/drafts/test.o");
            let production = c_steps(target, &source, &object, None);
            let draft = draft_steps(target, &source, &object).unwrap();
            assert_eq!(draft[..draft.len() - 1], production[..production.len() - 1]);
            assert!(production[0].iter().any(|flag| flag == "-mthumb"));
        }
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
        
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("probe.c");
        std::fs::write(&source, "int g;\nint probe(int a, int b)\n{\n    int c = a * 3;\n    if (b)\n        c += g;\n    return c + b;\n}\n").unwrap();
        let target = decomp_target(Some("tbs-en")).unwrap();
        let object = draft_object(&work.path().join("tbs-en"), "probe");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        let steps = draft_steps(target, &source, &object).unwrap();
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
    fn line_tables_reach_the_driver_and_direct_cc1_compile_steps() {
        let words = |text: &str| text.split(' ').map(String::from).collect::<Vec<_>>();
        let cpp = words("cpp0 -lang-c probe.c probe.i");
        let cc1 = words("gcc2952_arm probe.i -dumpbase probe.c -O2 -o probe.s");
        let driver = words("xgcc -O2 -S -o probe.s probe.c");
        let assemble = words("arm-none-eabi-as -o probe.o probe.s");
        assert_eq!(with_line_table(&cpp), cpp);
        assert_eq!(with_line_table(&assemble), assemble);
        assert_eq!(with_line_table(&cc1).last().map(String::as_str), Some("-g"));
        assert_eq!(with_line_table(&driver).last().map(String::as_str), Some("-g"));
    }

    #[test]
    fn located_rows_follow_objdump_line_markers() {
        let objdump = "00000000 <f>:\nf():\n/work/probe.c:3\n   0:\tpush\t{lr}\n/work/probe.c:4 (discriminator 1)\n   2:\tmovs\tr0, #1\n/work/event.h:12\n   4:\tbl\t0 <Use>\n";
        let rows = normalize_located(objdump);
        assert_eq!(rows, [
            ("push {lr}".to_string(), Some(("/work/probe.c".to_string(), 3))),
            ("movs r0, #1".to_string(), Some(("/work/probe.c".to_string(), 4))),
            ("bl".to_string(), Some(("/work/event.h".to_string(), 12))),
        ]);
        // A row from a header goes to the next row in the draft.
        let found = heat(&["push {lr}".to_string()], &[("push {lr}".into(), Some(3)), ("bl".into(), None), ("movs r0, #1".into(), Some(9))]);
        assert_eq!((found.lines.into_iter().collect::<Vec<_>>(), found.first), (vec![(9, 2)], Some(9)));
    }

    #[test]
    fn located_rows_follow_the_decoded_table_when_objdump_l_cannot() {
        let dump = "CU: /w/0800a494.c:\nFile name    Line number    Starting address    View    Stmt\n/w/0800a494.c    84    0    x\n/w/0800a494.c    105    0x4    x\n/w/games/tbs/x.h    12    0x8    1    x\n/w/0800a494.c    104    0xc    x\n";
        let table = decoded_lines(dump);
        assert_eq!(table, [
            (0, "/w/0800a494.c".to_string(), 84),
            (4, "/w/0800a494.c".to_string(), 105),
            (8, "/w/games/tbs/x.h".to_string(), 12),
            (12, "/w/0800a494.c".to_string(), 104),
        ]);
        let objdump = "00000000 <f>:\n   0:\tpush\t{r4, lr}\n   4:\tmov\tr4, r0\n   8:\tldr\tr0, [r4]\n   c:\tadd\tr0, r0, #1\n  10:\tpop\t{r4, pc}\n";
        let rows = located_by_address(objdump, &table);
        let lines: Vec<Option<u32>> = rows.iter().map(|(_, at)| at.as_ref().map(|(_, line)| *line)).collect();
        assert_eq!(lines, [Some(84), Some(105), Some(12), Some(104), Some(104)]);
    }

    #[test]
    fn overlay_comes_from_the_draft_path() {
        assert_eq!(resource_name("recon/tbs/en/overlays/resource_383_c_020048c8.c").as_deref(), Some("resource_383"));
        assert_eq!(resource_name("recon/tbs/en/overlays/resource_3cd/02008768.c").as_deref(), Some("resource_3cd"));
        assert_eq!(resource_name("recon/tbs/en/main/08011bf4.c"), None);
    }

    #[test]
    fn text_symbols_do_not_offer_the_compiler_marker_as_a_function() {
        
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("marker.s");
        let object = work.path().join("marker.o");
        std::fs::write(&source, ".text\n.local .gcc2_compiled.\n.gcc2_compiled.:\n.global RealFunction\n.type RealFunction, %function\n.thumb_func\nRealFunction:\n bx lr\n.size RealFunction, .-RealFunction\n").unwrap();
        command(&["arm-none-eabi-as", "-mthumb", "-o", &object.to_string_lossy(), &source.to_string_lossy()], root()).unwrap();
        assert_eq!(text_symbols(&object).unwrap(), vec!["RealFunction"]);
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

/// A draft's compile commands, as the Makefile compiles the file it would
/// replace in this edition, or through `family`.
fn c_steps(target: DecompTarget, draft: &Path, object: &Path, family: Option<CompilerFamily>) -> Vec<Vec<String>> {
    crate::compile::steps(target.compiler, target.edition_define, &root().join(target.output_dir), draft, object, family)
}

fn arm_word(bytes: &[u8], i: usize) -> u32 {
    u32::from_le_bytes([bytes[i * 4], bytes[i * 4 + 1], bytes[i * 4 + 2], bytes[i * 4 + 3]])
}

/// Jump-table words of ARM code at `base`: after `ldr pc, [pc, rN, lsl #2]` or
/// `add pc, pc, rN, lsl #2` (any condition), the words from pc + 8 on whose
/// value is an address inside the function. They move with the function, so
/// they compare as relocations, not instructions.
fn arm_tables(bytes: &[u8], base: u32) -> BTreeSet<usize> {
    let count = bytes.len() / 4;
    let inside = |w: u32| w >= base && w < base + bytes.len() as u32;
    let mut table = BTreeSet::new();
    for i in 0..count {
        let w = arm_word(bytes, i);
        if w & 0x0fff_fff0 != 0x079f_f100 && w & 0x0fff_fff0 != 0x008f_f100 {
            continue;
        }
        let mut j = i + 2;
        while j < count && inside(arm_word(bytes, j)) {
            table.insert(j);
            j += 1;
        }
    }
    table
}

/// ARM code compares word by word: pool words a pc-relative load reads are
/// dropped, jump-table words become `table`, and calls lose their targets;
/// branches inside stay relative.
fn arm_tokens(bytes: &[u8], base: u32) -> (Vec<String>, BTreeSet<usize>, BTreeSet<usize>) {
    let count = bytes.len() / 4;
    let mut pool = BTreeSet::new();
    for i in 0..count {
        let w = arm_word(bytes, i);
        if w & 0x0f3f_0000 == 0x051f_0000 {
            let offset = (w & 0xfff) as i64;
            let target = i as i64 * 4 + 8 + if w & (1 << 23) != 0 { offset } else { -offset };
            if target >= 0 && target % 4 == 0 && (target / 4) < count as i64 {
                pool.insert((target / 4) as usize);
            }
        }
    }
    let table = arm_tables(bytes, base);
    let tokens = (0..count)
        .filter(|i| !pool.contains(i))
        .map(|i| {
            let w = arm_word(bytes, i);
            if table.contains(&i) {
                "table".to_string()
            } else if w & 0x0f00_0000 == 0x0b00_0000 {
                format!("bl{:x}", w >> 28)
            } else {
                format!("{w:08x}")
            }
        })
        .collect();
    (tokens, pool, table)
}
