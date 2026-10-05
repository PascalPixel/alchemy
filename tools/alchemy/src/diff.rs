use crate::build_rom::c_steps;
use crate::compiler::routing::root;
use crate::process::run as command;
use crate::targets::decomp_target;
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy diff DRAFT.c [ADDRESS[,ADDRESS...]] [--game tbs|tla] [--show]
       alchemy diff OVERLAY_DRAFT.c [--overlay resource_XXX] [--symbol NAME] [--show]
Compiles DRAFT.c for the game's English edition with the build's own
compiler and options, then compares its instructions with the English raw
listing objects out/<game>-en/obj/recon/<game>/raw/ADDRESS.o (build the
edition first). ADDRESS defaults to the draft's file name; give several,
comma-separated, when the draft holds a nested function and its parent.
The game defaults from the draft's path. Prints the instruction counts and
how many differ; --show prints the comparison, - for the listing and + for
the draft. "differ 0" means the code matches instruction for instruction.
An overlay draft (a path under overlays/) compares with the function of
the same name, or --symbol, in out/<game>-en/overlays/OVERLAY_overlay.o;
the overlay defaults from the path (resource_XXX)."#;

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    show: bool,
    overlay: Option<String>,
    symbol: Option<String>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut draft = None;
    let mut addresses = Vec::new();
    let mut game = None;
    let mut show = false;
    let mut overlay = None;
    let mut symbol = None;
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--show" => show = true,
            "--overlay" => {
                i += 1;
                overlay = Some(arguments.get(i).ok_or_else(|| USAGE.to_string())?.clone());
            }
            "--symbol" => {
                i += 1;
                symbol = Some(arguments.get(i).ok_or_else(|| USAGE.to_string())?.clone());
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
    let game = game.unwrap_or(if text.contains("/tla/") || text.contains("THE LOST AGE") {
        "tla"
    } else {
        "tbs"
    });
    Ok(Options { draft, addresses, game, show, overlay, symbol })
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
    let root = root();
    let target = decomp_target(Some(&format!("{}-en", options.game)))?;
    let stem = options
        .draft
        .file_stem()
        .map(|s| s.to_string_lossy().into_owned())
        .unwrap_or_default();
    let object = root
        .join(target.output_dir)
        .join("obj/drafts")
        .join(format!("{stem}.o"));
    std::fs::create_dir_all(object.parent().unwrap()).map_err(|e| e.to_string())?;
    let draft = std::fs::canonicalize(&options.draft)
        .map_err(|e| format!("{}: {e}", options.draft.display()))?;
    let mut steps = c_steps(target, &draft, &object)?;
    steps.pop();
    for step in steps {
        command(&step, root)?;
    }
    let assembly = object.with_extension("s").to_string_lossy().into_owned();
    let object_text = object.to_string_lossy().into_owned();
    let mut assemble = vec!["arm-none-eabi-as", "-mcpu=arm7tdmi"];
    if options.game == "tbs" {
        assemble.push("-mthumb-interwork");
    }
    assemble.extend(["-o", &object_text, &assembly]);
    command(&assemble, root)?;
    let mut listing = Vec::new();
    let mut draft_symbol = None;
    if let Some(overlay) = &options.overlay {
        let reference = root.join(target.output_dir).join(format!("overlays/{overlay}_overlay.o"));
        if !reference.exists() {
            return Err(format!("{}: no overlay listing object; build {}-en first", reference.display(), options.game));
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
                "{}: no listing object; build {}-en first",
                reference.display(),
                options.game
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
