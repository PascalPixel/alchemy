//! O2: one name per place. Fails when a built image (main or overlay, any of
//! the twelve editions) defines two or more global, non-absolute symbols at
//! one address, unless tools/alchemy/aliases.tsv keeps that group with its
//! reason. Reads the ELFs a build left in out/; it never builds.

use crate::elf::{Elf, Symbol};
use crate::targets::{target_for, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};

pub(crate) const ALLOWLIST: &str = "tools/alchemy/aliases.tsv";

const USAGE: &str = "usage: alchemy check aliases\n\
Fails when a built ELF of any edition (out/<edition>/<edition>.elf and\n\
out/<edition>/overlays/*.elf) has two or more global, non-absolute symbols\n\
at one address (O2). Thumb function values are compared without their mode\n\
bit. Local symbols, .gcc2_compiled. and AlchemyUncredited* markers are\n\
ignored. Groups kept on purpose are rows of tools/alchemy/aliases.tsv:\n\
game, the names (comma-separated), reason; a row no build has any more fails\n\
too, so the list only shrinks. Build all twelve editions first.";

const SHN_UNDEF: u16 = 0;
const SHN_ABS: u16 = 0xfff1;
const STB_LOCAL: u8 = 0;
const STT_FUNC: u8 = 2;
const STT_ARM_TFUNC: u8 = 13;

pub fn entry(args: &[String]) -> std::process::ExitCode {
    match args {
        [] => {}
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return std::process::ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return std::process::ExitCode::from(2);
        }
    }
    match run(crate::compiler::routing::root()) {
        Ok(summary) => {
            println!("{summary}");
            std::process::ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}

/// A row of the allowlist: the game and the exact set of names it keeps.
type Kept = BTreeSet<(String, BTreeSet<String>)>;

fn run(root: &Path) -> Result<String, String> {
    let text = fs::read_to_string(root.join(ALLOWLIST)).map_err(|e| format!("{ALLOWLIST}: {e}"))?;
    let kept = parse(&text)?;
    let mut used = Kept::new();
    let mut failures = Vec::new();
    let mut images = 0;
    for id in TARGET_IDS {
        let target = target_for(id);
        let game = id.as_str().split_once('-').map_or(id.as_str(), |(game, _)| game).to_string();
        for path in images_of(root, target.output_dir, id.as_str())? {
            let data = fs::read(&path).map_err(|e| format!("{}: {e}", path.display()))?;
            let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", path.display()))?;
            images += 1;
            for (address, names) in groups(&elf.symbols) {
                let key = (game.clone(), names);
                if kept.contains(&key) {
                    used.insert(key);
                    continue;
                }
                let shown = path.strip_prefix(root).unwrap_or(&path).display();
                failures.push(format!("{shown}: {address:08x} {}", join(&key.1)));
            }
        }
    }
    let stale: Vec<String> = kept
        .difference(&used)
        .map(|(game, names)| format!("{ALLOWLIST}: {game} {} matches no build; remove the row", join(names)))
        .collect();
    if failures.is_empty() && stale.is_empty() {
        return Ok(format!("aliases: OK, {images} images, {} kept groups", used.len()));
    }
    let mut report = failures;
    if !report.is_empty() {
        report.push("one name per place (O2): merge the names, or keep the group in the allowlist with its reason".into());
    }
    report.extend(stale);
    Err(report.join("\n"))
}

fn join(names: &BTreeSet<String>) -> String {
    names.iter().cloned().collect::<Vec<_>>().join(",")
}

/// The edition's main image and its overlays, or an error that says to build.
fn images_of(root: &Path, output_dir: &str, id: &str) -> Result<Vec<PathBuf>, String> {
    let dir = root.join(output_dir);
    let main = dir.join(format!("{id}.elf"));
    if !main.is_file() {
        return Err(format!(
            "{output_dir}/{id}.elf is missing: build all twelve editions first (make compare-editions)"
        ));
    }
    let mut paths = vec![main];
    if let Ok(entries) = fs::read_dir(dir.join("overlays")) {
        let mut overlays: Vec<PathBuf> = entries
            .filter_map(Result::ok)
            .map(|entry| entry.path())
            .filter(|path| path.extension().is_some_and(|e| e == "elf"))
            .collect();
        overlays.sort();
        paths.extend(overlays);
    }
    Ok(paths)
}

/// Whether a symbol names a place in this image: global or weak, defined in
/// a section, and not a compiler or build marker.
fn counts(symbol: &Symbol) -> bool {
    symbol.bind != STB_LOCAL
        && symbol.section != SHN_UNDEF
        && symbol.section != SHN_ABS
        && !symbol.name.is_empty()
        && !symbol.name.starts_with('$')
        && symbol.name != ".gcc2_compiled."
        && !symbol.name.starts_with("AlchemyUncredited")
}

/// The place a symbol names: a Thumb function's value without its mode bit.
fn place(symbol: &Symbol) -> u32 {
    if matches!(symbol.kind, STT_FUNC | STT_ARM_TFUNC) {
        symbol.value & !1
    } else {
        symbol.value
    }
}

/// Every address two or more distinct counted names share, in address order.
fn groups(symbols: &[Symbol]) -> Vec<(u32, BTreeSet<String>)> {
    let mut by_place: BTreeMap<u32, BTreeSet<String>> = BTreeMap::new();
    for symbol in symbols.iter().filter(|s| counts(s)) {
        by_place.entry(place(symbol)).or_default().insert(symbol.name.clone());
    }
    by_place.into_iter().filter(|(_, names)| names.len() > 1).collect()
}

fn parse(text: &str) -> Result<Kept, String> {
    let mut kept = Kept::new();
    for (number, line) in text.lines().enumerate() {
        if number == 0 || line.trim().is_empty() || line.starts_with('#') {
            continue;
        }
        let fields: Vec<&str> = line.split('\t').collect();
        let [game, names, reason] = fields[..] else {
            return Err(format!("{ALLOWLIST}:{}: expected game, names, reason", number + 1));
        };
        if !matches!(game, "tbs" | "tla") || reason.trim().is_empty() {
            return Err(format!("{ALLOWLIST}:{}: a row needs game tbs or tla and a reason", number + 1));
        }
        let names: BTreeSet<String> = names.split(',').map(|n| n.trim().to_string()).filter(|n| !n.is_empty()).collect();
        if names.len() < 2 {
            return Err(format!("{ALLOWLIST}:{}: a group names two or more symbols", number + 1));
        }
        if !kept.insert((game.to_string(), names)) {
            return Err(format!("{ALLOWLIST}:{}: duplicate row", number + 1));
        }
    }
    Ok(kept)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn symbol(name: &str, value: u32, kind: u8, bind: u8, section: u16) -> Symbol {
        Symbol { name: name.into(), value, size: 0, kind, bind, section }
    }

    #[test]
    fn groups_global_names_at_one_place_and_ignores_the_rest() {
        let symbols = vec![
            // A data table label against its bank's first Thumb veneer.
            symbol("FarCall_Table", 0x0800_9000, 15, 1, 5),
            symbol("Veneer", 0x0800_9001, STT_FUNC, 1, 5),
            // Locals, absolutes, undefined, markers and the compiler's label.
            symbol("local", 0x0800_9000, 0, STB_LOCAL, 5),
            symbol("Fixed", 0x0800_9000, 0, 1, SHN_ABS),
            symbol("Missing", 0x0800_9000, 0, 1, SHN_UNDEF),
            symbol("AlchemyUncredited_1", 0x0800_9000, 0, 1, 5),
            symbol(".gcc2_compiled.", 0x0800_9000, 0, 1, 5),
            symbol("$t", 0x0800_9000, 0, 1, 5),
            // Byte data at neighbouring odd and even addresses stays apart.
            symbol("ByteA", 0x0800_a000, 1, 1, 5),
            symbol("ByteB", 0x0800_a001, 1, 1, 5),
            // A weak alias counts.
            symbol("Strong", 0x0800_b000, 1, 1, 5),
            symbol("Weak", 0x0800_b000, 1, 2, 5),
        ];
        let found = groups(&symbols);
        let names: Vec<(u32, String)> = found.iter().map(|(a, n)| (*a, join(n))).collect();
        assert_eq!(
            names,
            vec![(0x0800_9000, "FarCall_Table,Veneer".into()), (0x0800_b000, "Strong,Weak".into())]
        );
    }

    #[test]
    fn allowlist_rows_need_a_game_two_names_and_a_reason() {
        let kept = parse("game\tsymbols\treason\ntbs\tB,A\tkept\n").unwrap();
        let names: BTreeSet<String> = ["A", "B"].map(String::from).into();
        assert!(kept.contains(&("tbs".to_string(), names)));
        assert!(parse("h\nxyz\tA,B\tkept\n").is_err());
        assert!(parse("h\ntbs\tA\tkept\n").is_err());
        assert!(parse("h\ntbs\tA,B\t\n").is_err());
        assert!(parse("h\ntbs\tA,B\n").is_err());
        assert!(parse("h\ntbs\tA,B\tx\ntbs\tB,A\ty\n").is_err());
    }

    #[test]
    fn missing_builds_ask_for_a_build() {
        let dir = std::env::temp_dir().join(format!("alchemy-aliases-{}", std::process::id()));
        let error = images_of(&dir, "out/tbs-ja", "tbs-ja").unwrap_err();
        assert!(error.contains("build all twelve editions first"), "{error}");
    }
}
