//! The symbol map: one name per reconstructed symbol, in one place.
//!
//! Names used to live only as `#define` lines inside individual sources, so a
//! symbol could be called one thing in one owner and something else in the
//! owner next to it, and renaming meant finding every site by hand. `symbols.sym`
//! is the single map; this validates it and emits the header sources include.
//!
//! SCOPE IS NOT OPTIONAL. Every overlay loads at `0x02000000`, so an address on
//! its own is ambiguous -- `Func_02000030` is a different function in each of
//! the nine overlays that define one. Entries are therefore `<scope> <symbol>
//! <name>`, where scope is `main` or the overlay stem, and uniqueness is
//! enforced per scope rather than globally.

pub mod cli;

use std::collections::BTreeMap;
use std::path::Path;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Entry {
    pub scope: String,
    pub symbol: String,
    pub name: String,
}

/// Parse `symbols.sym`. Blank lines and `#` comments are ignored.
pub fn parse(text: &str) -> Result<Vec<Entry>, String> {
    let mut entries = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let line = line.trim();
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        let fields: Vec<&str> = line.split_whitespace().collect();
        if fields.len() != 3 {
            return Err(format!(
                "symbols.sym:{}: expected `<scope> <symbol> <name>`, got {line:?}",
                index + 1
            ));
        }
        let (scope, symbol, name) = (fields[0], fields[1], fields[2]);
        let known = symbol.starts_with("Func_") || symbol.starts_with("Data_") || symbol.starts_with("Value_");
        if !known {
            return Err(format!(
                "symbols.sym:{}: {symbol} is not a Func_/Data_/Value_ symbol",
                index + 1
            ));
        }
        if name.chars().next().is_some_and(|c| c.is_ascii_digit()) {
            return Err(format!("symbols.sym:{}: {name} is not a C identifier", index + 1));
        }
        entries.push(Entry {
            scope: scope.to_string(),
            symbol: symbol.to_string(),
            name: name.to_string(),
        });
    }
    Ok(entries)
}

/// One name per symbol and one symbol per name, WITHIN a scope.
pub fn violations(entries: &[Entry]) -> Vec<String> {
    let mut by_symbol: BTreeMap<(&str, &str), Vec<&str>> = BTreeMap::new();
    let mut by_name: BTreeMap<(&str, &str), Vec<&str>> = BTreeMap::new();
    for entry in entries {
        by_symbol
            .entry((&entry.scope, &entry.symbol))
            .or_default()
            .push(&entry.name);
        by_name
            .entry((&entry.scope, &entry.name))
            .or_default()
            .push(&entry.symbol);
    }
    let mut problems = Vec::new();
    for ((scope, symbol), names) in &by_symbol {
        if names.len() > 1 {
            problems.push(format!("{scope} {symbol} has {} names: {}", names.len(), names.join(", ")));
        }
    }
    for ((scope, name), symbols) in &by_name {
        if symbols.len() > 1 {
            problems.push(format!("{scope} {name} names {} symbols: {}", symbols.len(), symbols.join(", ")));
        }
    }
    problems
}

/// The header sources include, so a rename in the map reaches every use.
pub fn header(entries: &[Entry]) -> String {
    let mut out = String::from(
        "/* Generated from symbols.sym by `check symbols --emit`. Do not edit. */\n\
         #ifndef ALCHEMY_SYMBOLS_H\n#define ALCHEMY_SYMBOLS_H\n",
    );
    let mut scope = "";
    for entry in entries {
        if entry.scope != scope {
            scope = &entry.scope;
            out.push_str(&format!("\n/* {scope} */\n"));
        }
        out.push_str(&format!("#define {} {}\n", entry.name, entry.symbol));
    }
    out.push_str("\n#endif\n");
    out
}

pub fn map_path(root: &Path) -> std::path::PathBuf {
    root.join("symbols.sym")
}

pub fn self_test() -> Result<(), String> {
    let good = "# comment\nmain Func_08000000 Boot\nresource_372 Func_02000030 Start\n";
    let entries = parse(good)?;
    if entries.len() != 2 {
        return Err("parse dropped an entry".into());
    }
    if !violations(&entries).is_empty() {
        return Err("a well-formed map must pass".into());
    }
    // The same address in two overlays is NOT a conflict.
    let scoped = parse("resource_372 Func_02000030 A\nresource_373 Func_02000030 B\n")?;
    if !violations(&scoped).is_empty() {
        return Err("scope must disambiguate the shared overlay address space".into());
    }
    // Two names for one symbol in one scope IS a conflict.
    let clash = parse("resource_372 Func_02000030 A\nresource_372 Func_02000030 B\n")?;
    if violations(&clash).is_empty() {
        return Err("two names for one symbol must be refused".into());
    }
    if parse("main Func_08000000\n").is_ok() {
        return Err("a malformed row must be refused".into());
    }
    if !header(&entries).contains("#define Boot Func_08000000") {
        return Err("the header must define each name".into());
    }
    Ok(())
}
