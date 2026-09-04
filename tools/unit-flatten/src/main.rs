//! Flatten an overlay's exact owners into one translation unit.
//!
//!   unit-flatten <game> <resource_NNN> --id <unit-id> --path <src-relative .c> [--apply]
//!
//! Reads the owner inventory for the overlay, takes every exact owner's
//! production source in address order, and writes one flat C file: includes
//! once, local include composites expanded in place, declarations unified
//! (identical lines deduplicated, one prototype per symbol), inline wrappers
//! and record layouts once, then the functions in address order. With
//! `--apply` it also registers every owner to the flat file in
//! source-paths.json, removes the superseded per-owner files and any header
//! only they included, and declares the unit in translation-units.json.
//! Without `--apply` it writes out/unit-flatten/<unit-id>.c and prints the
//! manifest entry.
//!
//! Declarations that differ between owners are reconciled mechanically where
//! the reconciliation cannot change a byte: a data symbol becomes one
//! `extern u8 Name[];` and a file that typed it otherwise reads it through a
//! cast of that shape; a function keeps the non-void return type and drops
//! to an unprototyped `()` when parameter lists disagree; a wrapper spelling
//! the scene work pointer as a literal takes the symbol; two wrappers with
//! one name and different callees are told apart by the callee address.
//! Whatever remains is reported and stops the run: it is the shared
//! interface the unit forces into the open.

use serde_json::{json, Map, Value};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};

struct Owner {
    key: String,
    address: u32,
    extent: usize,
    name: String,
    source: PathBuf,
}

#[derive(Clone)]
struct Item {
    kind: &'static str,
    key: String,
    text: String,
    from: PathBuf,
}

fn main() -> ExitCode {
    match run() {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run() -> Result<(), String> {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let apply = args.iter().any(|a| a == "--apply");
    let flag = |name: &str| {
        args.iter()
            .position(|a| a == name)
            .and_then(|i| args.get(i + 1).cloned())
    };
    let mut positional = Vec::new();
    let mut skip = false;
    for (i, a) in args.iter().enumerate() {
        if skip {
            skip = false;
            continue;
        }
        if a.starts_with("--") {
            skip = a != "--apply" && args.get(i + 1).is_some();
            continue;
        }
        positional.push(a.clone());
    }
    let (Some(game), Some(overlay), Some(unit_id), Some(unit_path)) = (
        positional.first().cloned(),
        positional.get(1).cloned(),
        flag("--id"),
        flag("--path"),
    ) else {
        return Err("usage: unit-flatten <game> <resource_NNN> --id <unit-id> --path <src-relative .c> [--apply]".into());
    };
    let root = std::env::current_dir().map_err(|e| e.to_string())?;
    let source_root = root.join("games").join(&game).join("src");
    let inventory =
        read_json(&root.join(format!("out/{game}-en/full/rebuilt.owner-inventory.json")))?;
    let register_path = root.join(format!("games/{game}/source-paths.json"));
    let mut register = read_json(&register_path)?;
    let manifest_path = root.join(format!("games/{game}/recon/translation-units.json"));
    let mut manifest = read_json(&manifest_path)?;

    let mut owners = Vec::new();
    let mut retained = 0;
    for entry in inventory["owners"]
        .as_array()
        .ok_or("inventory lacks owners")?
    {
        if entry["container"]["kind"] != "overlay-image"
            || entry["container"]["overlay"] != overlay.as_str()
        {
            continue;
        }
        if entry["production"]["state"] != "exact-c" {
            retained += 1;
            continue;
        }
        let address = u32::from_str_radix(
            entry["address"]
                .as_str()
                .unwrap_or("")
                .trim_start_matches("0x"),
            16,
        )
        .map_err(|_| format!("bad address in {}", entry["id"]))?;
        owners.push(Owner {
            key: entry["id"].as_str().unwrap_or("").to_string(),
            address,
            extent: entry["production"]["extent_bytes"].as_u64().unwrap_or(0) as usize,
            name: entry["name"].as_str().unwrap_or("").to_string(),
            source: root.join(entry["registration"]["source_path"].as_str().unwrap_or("")),
        });
    }
    if retained > 0 {
        return Err(format!("{overlay}: {retained} owners are not exact C; this tool flattens wholly exact overlays only"));
    }
    if owners.is_empty() {
        return Err(format!("{overlay}: no owners"));
    }
    owners.sort_by_key(|o| o.address);

    let mut files: Vec<PathBuf> = Vec::new();
    for o in &owners {
        if !files.contains(&o.source) {
            files.push(o.source.clone());
        }
    }
    let mut inlined: BTreeSet<PathBuf> = BTreeSet::new();
    let mut parsed: Vec<(PathBuf, Vec<Item>)> = Vec::new();
    for file in &files {
        let text = expand_local_includes(file, &mut inlined)?;
        parsed.push((file.clone(), items(&text, file)));
    }

    // Object-like aliases, `#define Name Func_xxxx`: two files declaring one
    // symbol under different names are reconciled by the symbol.
    let mut aliases: BTreeMap<String, String> = BTreeMap::new();
    let mut macros: BTreeSet<String> = BTreeSet::new();
    for (_, list) in &parsed {
        for it in list.iter().filter(|it| it.kind == "define") {
            let words: Vec<&str> = it.text.split_whitespace().collect();
            if words.len() == 3
                && (words[2].starts_with("Func_") || words[2].starts_with("Data_"))
                && !words[1].contains('(')
            {
                aliases.insert(words[1].to_string(), words[2].to_string());
                continue;
            }
            // A forwarding macro, `#define Name(args...) Func_xxxx(args)`,
            // names the callee too; any other function-like macro just
            // shadows a prototype of the same name.
            if let Some(open) = words.get(1).and_then(|w| w.find('(')) {
                let name = words[1][..open].to_string();
                let body = words[2..].join(" ");
                if let Some(symbol) = body.strip_suffix("(args)").filter(|s| {
                    s.starts_with("Func_") && s.chars().skip(5).all(|c| c.is_ascii_hexdigit())
                }) {
                    aliases.insert(name.clone(), symbol.to_string());
                }
                macros.insert(name);
            }
        }
    }
    let resolve = |name: &str| {
        aliases
            .get(name)
            .cloned()
            .unwrap_or_else(|| name.to_string())
    };
    for (_, list) in parsed.iter_mut() {
        for it in list.iter_mut() {
            if it.kind != "extern" && it.kind != "prototype" {
                continue;
            }
            if let Some((name, _, _)) = function_decl(&it.text) {
                if macros.contains(&name) && !aliases.contains_key(&name) {
                    // A prototype of a macro name: meaningless once the
                    // macro is defined first.
                    it.kind = "dropped";
                    continue;
                }
            }
            if let Some((name, _)) = data_shape(&it.text) {
                let symbol = resolve(&name);
                if symbol != name {
                    it.text = replace_word(&it.text, &name, &symbol);
                }
            } else if let Some((name, _, _)) = function_decl(&it.text) {
                let symbol = resolve(&name);
                if symbol != name {
                    it.text = replace_word(&it.text, &name, &symbol);
                }
            }
        }
    }
    // Reconcile data and function declarations.
    let mut data_decls: BTreeMap<String, BTreeMap<PathBuf, Shape>> = BTreeMap::new();
    let mut func_decls: BTreeMap<String, BTreeMap<PathBuf, (String, String)>> = BTreeMap::new();
    for (file, list) in &parsed {
        for it in list {
            if it.kind != "extern" && it.kind != "prototype" {
                continue;
            }
            if let Some((name, shape)) = data_shape(&it.text) {
                data_decls
                    .entry(name)
                    .or_default()
                    .insert(file.clone(), shape);
            } else if let Some((name, ret, params)) = function_decl(&it.text) {
                func_decls
                    .entry(name)
                    .or_default()
                    .insert(file.clone(), (ret, params));
            }
        }
    }
    let mut rewrites: BTreeMap<PathBuf, BTreeMap<String, String>> = BTreeMap::new();
    let mut unified_data: BTreeMap<String, String> = BTreeMap::new();
    for (name, by_file) in &data_decls {
        let distinct: BTreeSet<String> = by_file.values().map(|s| s.text()).collect();
        if distinct.len() == 1 {
            unified_data.insert(
                name.clone(),
                by_file.values().next().unwrap().declaration(name),
            );
            continue;
        }
        unified_data.insert(name.clone(), format!("extern u8 {name}[];"));
        for (file, shape) in by_file {
            if shape.kind == "array" && shape.ty == "u8" && shape.qualifiers.is_empty() {
                continue;
            }
            rewrites
                .entry(file.clone())
                .or_default()
                .insert(name.clone(), shape.access(name));
        }
    }
    let mut unified_funcs: BTreeMap<String, String> = BTreeMap::new();
    let mut conflicts: Vec<String> = Vec::new();
    for (name, by_file) in &func_decls {
        let rets: BTreeSet<&str> = by_file.values().map(|(r, _)| r.as_str()).collect();
        let params: BTreeSet<&str> = by_file.values().map(|(_, p)| p.as_str()).collect();
        let non_void: Vec<&str> = rets.iter().copied().filter(|r| *r != "void").collect();
        let ret = match non_void.len() {
            0 => "void",
            1 => non_void[0],
            _ => {
                conflicts.push(format!(
                    "function {name}: return types {}",
                    rets.iter().copied().collect::<Vec<_>>().join(" | ")
                ));
                non_void[0]
            }
        };
        let list = if params.len() == 1 {
            params.iter().next().unwrap().to_string()
        } else {
            String::new()
        };
        let space = if ret.ends_with('*') { "" } else { " " };
        unified_funcs.insert(name.clone(), format!("{ret}{space}{name}({list});"));
    }
    for (file, list) in parsed.iter_mut() {
        let Some(map) = rewrites.get(file) else {
            continue;
        };
        for it in list.iter_mut() {
            if it.kind == "extern" || it.kind == "prototype" || it.kind == "include" {
                continue;
            }
            for (name, replacement) in map {
                it.text = replace_word(&it.text, name, replacement);
            }
        }
    }

    // Unify.
    let mut includes: Vec<String> = Vec::new();
    let mut defines: BTreeMap<String, Item> = BTreeMap::new();
    let mut define_order: Vec<String> = Vec::new();
    let mut externs: BTreeMap<String, Item> = BTreeMap::new();
    let mut extern_order: Vec<String> = Vec::new();
    let mut prototypes: BTreeMap<String, Item> = BTreeMap::new();
    let mut prototype_order: Vec<String> = Vec::new();
    let mut structs: Vec<Item> = Vec::new();
    let mut typedefs: Vec<Item> = Vec::new();
    let mut inlines: Vec<Item> = Vec::new();
    let mut comments: Vec<Item> = Vec::new();
    let mut functions: Vec<Item> = Vec::new();
    let mut others: Vec<Item> = Vec::new();
    let mut renamed: Vec<String> = Vec::new();
    let mut renames: Vec<(PathBuf, String, String)> = Vec::new();
    for (file, list) in &parsed {
        for it in list {
            match it.kind {
                "include" => {
                    if !includes.contains(&it.key) {
                        includes.push(it.key.clone());
                    }
                }
                "define" => {
                    if let Some(have) = defines.get(&it.key) {
                        if norm(&have.text) != norm(&it.text) {
                            conflicts.push(format!(
                                "define {}: {} vs {}",
                                it.key,
                                rel(&have.from, &source_root),
                                rel(file, &source_root)
                            ));
                        }
                    } else {
                        define_order.push(it.key.clone());
                        defines.insert(it.key.clone(), it.clone());
                    }
                }
                "extern" | "prototype" => {
                    if let Some((name, _)) = data_shape(&it.text) {
                        if !externs.contains_key(&name) {
                            let mut unified = it.clone();
                            unified.text = unified_data
                                .get(&name)
                                .cloned()
                                .unwrap_or_else(|| it.text.clone());
                            extern_order.push(name.clone());
                            externs.insert(name, unified);
                        }
                    } else if let Some((name, _, _)) = function_decl(&it.text) {
                        if !prototypes.contains_key(&name) {
                            let mut unified = it.clone();
                            unified.text = unified_funcs
                                .get(&name)
                                .cloned()
                                .unwrap_or_else(|| it.text.clone());
                            prototype_order.push(name.clone());
                            prototypes.insert(name, unified);
                        }
                    } else if let Some(have) = externs.get(&it.key) {
                        if norm(&have.text) != norm(&it.text) {
                            conflicts.push(format!(
                                "declaration {}: \"{}\" ({}) vs \"{}\" ({})",
                                it.key,
                                norm(&have.text),
                                rel(&have.from, &source_root),
                                norm(&it.text),
                                rel(file, &source_root)
                            ));
                        }
                    } else {
                        extern_order.push(it.key.clone());
                        externs.insert(it.key.clone(), it.clone());
                    }
                }
                "struct" => {
                    if let Some(have) = structs.iter().find(|s| s.key == it.key) {
                        if norm(&have.text) != norm(&it.text) {
                            conflicts.push(format!(
                                "struct {}: {} vs {}",
                                it.key,
                                rel(&have.from, &source_root),
                                rel(file, &source_root)
                            ));
                        }
                    } else {
                        structs.push(it.clone());
                    }
                }
                "typedef" => {
                    if !typedefs.iter().any(|t| t.key == it.key) {
                        typedefs.push(it.clone());
                    }
                }
                "inline" => {
                    let Some(index) = inlines.iter().position(|w| w.key == it.key) else {
                        inlines.push(it.clone());
                        continue;
                    };
                    if norm(&inlines[index].text) == norm(&it.text) {
                        continue;
                    }
                    let symbolic = |t: &str| {
                        t.replace("0x03001ebc", "Data_03001ebc")
                            .replace("0x3001ebc", "Data_03001ebc")
                    };
                    if norm(&symbolic(&inlines[index].text)) == norm(&symbolic(&it.text)) {
                        let have = &mut inlines[index];
                        if have.text != symbolic(&have.text) {
                            renamed.push(format!(
                                "{}: literal 0x03001ebc spelt Data_03001ebc ({})",
                                it.key,
                                rel(&have.from, &source_root)
                            ));
                            have.text = symbolic(&have.text);
                        }
                        continue;
                    }
                    let callee_of = |t: &str| callee_address(t);
                    let suffix = match (callee_of(&it.text), callee_of(&inlines[index].text)) {
                        (Some(a), Some(b)) if a != b => a,
                        (Some(a), None) => a,
                        _ => owners
                            .iter()
                            .find(|o| &o.source == file)
                            .map(|o| format!("{:08x}", o.address))
                            .unwrap_or_else(|| "2".into()),
                    };
                    let fresh = format!("{}_{suffix}", it.key);
                    if inlines.iter().any(|w| w.key == fresh) {
                        conflicts.push(format!(
                            "inline {}: {} vs {} (and {fresh} taken)",
                            it.key,
                            rel(&inlines[index].from, &source_root),
                            rel(file, &source_root)
                        ));
                        continue;
                    }
                    let mut wrapper = it.clone();
                    wrapper.text = rename_call(&it.text, &it.key, &fresh, true);
                    wrapper.key = fresh.clone();
                    inlines.push(wrapper);
                    renamed.push(format!(
                        "{} -> {fresh} in {}",
                        it.key,
                        rel(file, &source_root)
                    ));
                    renames.push((file.clone(), it.key.clone(), fresh));
                }
                "comment" => {
                    if !comments.iter().any(|c| norm(&c.text) == norm(&it.text)) {
                        comments.push(it.clone());
                    }
                }
                "function" => functions.push(it.clone()),
                _ => others.push(it.clone()),
            }
        }
    }
    for (file, from, to) in &renames {
        for f in functions.iter_mut() {
            if &f.from == file {
                f.text = rename_call(&f.text, from, to, false);
            }
        }
    }
    for line in &renamed {
        println!("  {line}");
    }
    for (file, map) in &rewrites {
        let list: Vec<String> = map.iter().map(|(n, r)| format!("{n} -> {r}")).collect();
        println!("  {}: {}", rel(file, &source_root), list.join(", "));
    }
    if !conflicts.is_empty() {
        let mut message = format!("{overlay}: {} declaration conflicts:", conflicts.len());
        for c in &conflicts {
            message.push_str(&format!("\n  {c}"));
        }
        return Err(message);
    }

    // Owner names: the register's name, else the file's alias for the address.
    let alias_for = |o: &Owner| -> String {
        let want = format!("Func_{:08x}", o.address);
        for (name, it) in &defines {
            if norm(&it.text).ends_with(&format!(" {want}")) {
                return name.clone();
            }
        }
        o.name.clone()
    };
    let aliases: Vec<String> = owners.iter().map(alias_for).collect();
    for (o, alias) in owners.iter().zip(&aliases) {
        let legacy = format!("Func_{:08x}", o.address);
        if !functions.iter().any(|f| &f.key == alias || f.key == legacy) {
            return Err(format!(
                "{}: no function named {alias} in its source {}",
                o.key,
                rel(&o.source, &source_root)
            ));
        }
    }

    // Order: the shared types, then every alias define (an included header
    // may declare through an alias, as the owners' files did), then the
    // other includes, layouts, declarations, wrappers, and the functions.
    let mut out: Vec<String> = vec!["#include \"types.h\"".into()];
    out.push(String::new());
    for name in &define_order {
        out.push(defines[name].text.clone());
    }
    if !define_order.is_empty() {
        out.push(String::new());
    }
    for include in includes.iter().filter(|i| !i.contains("\"types.h\"")) {
        out.push(include.clone());
    }
    if includes.iter().any(|i| !i.contains("\"types.h\"")) {
        out.push(String::new());
    }
    for it in &typedefs {
        out.push(it.text.clone());
    }
    for it in &structs {
        out.push(it.text.clone());
        out.push(String::new());
    }
    for name in &extern_order {
        out.push(externs[name].text.clone());
    }
    if !extern_order.is_empty() {
        out.push(String::new());
    }
    for name in &prototype_order {
        out.push(prototypes[name].text.clone());
    }
    if !prototype_order.is_empty() {
        out.push(String::new());
    }
    for it in &comments {
        out.push(it.text.clone());
        out.push(String::new());
    }
    for it in &inlines {
        out.push(it.text.clone());
        out.push(String::new());
    }
    for it in &others {
        out.push(it.text.clone());
        out.push(String::new());
    }
    let mut emitted: BTreeSet<String> = BTreeSet::new();
    for (o, alias) in owners.iter().zip(&aliases) {
        let legacy = format!("Func_{:08x}", o.address);
        if let Some(f) = functions
            .iter()
            .find(|f| (&f.key == alias || f.key == legacy) && !emitted.contains(&f.text))
        {
            emitted.insert(f.text.clone());
            out.push(f.text.clone());
            out.push(String::new());
        }
    }
    for f in &functions {
        if emitted.insert(f.text.clone()) {
            out.push(f.text.clone());
            out.push(String::new());
        }
    }
    while out.last().is_some_and(|l| l.is_empty()) {
        out.pop();
    }
    let flat = format!("{}\n", out.join("\n"));
    let target = if apply {
        source_root.join(&unit_path)
    } else {
        root.join(format!("out/unit-flatten/{unit_id}.c"))
    };
    if let Some(parent) = target.parent() {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    fs::write(&target, &flat).map_err(|e| format!("{}: {e}", target.display()))?;
    println!(
        "{overlay}: {} owners, {} files -> {} ({} lines)",
        owners.len(),
        files.len(),
        rel(&target, &root),
        flat.lines().count()
    );
    println!(
        "  declarations: {} extern, {} prototypes, {} defines, {} records, {} wrappers, {} inlined headers",
        extern_order.len(),
        prototype_order.len(),
        define_order.len(),
        structs.len(),
        inlines.len(),
        inlined.len()
    );

    let entry = json!({
        "id": unit_id,
        "game": game,
        "source": format!("games/{game}/src/{unit_path}"),
        "compiler_route": "canonical-gcc296",
        "overlay": overlay,
        "absolute_symbols": {},
        "local_symbols": [],
        "owners": owners.iter().map(|o| json!({"address": format!("0x{:08x}", o.address), "extent": o.extent, "state": "exact-c"})).collect::<Vec<_>>(),
    });
    if !apply {
        println!(
            "manifest entry:\n{}",
            serde_json::to_string_pretty(&entry).map_err(|e| e.to_string())?
        );
        return Ok(());
    }
    // Register every owner to the flat file: overlay grouped sources carry the
    // shared path on every member.
    let owners_map = register["owners"]
        .as_object_mut()
        .ok_or("source-paths.json: owners must be an object")?;
    for (o, alias) in owners.iter().zip(&aliases) {
        let mut record = Map::new();
        record.insert("name".into(), Value::String(alias.clone()));
        record.insert("source".into(), Value::String(unit_path.clone()));
        owners_map.insert(o.key.clone(), Value::Object(record));
    }
    write_json(&register_path, &register)?;
    let mut removed = 0;
    let still_registered: BTreeSet<String> = register["owners"]
        .as_object()
        .unwrap()
        .values()
        .filter_map(|v| {
            v.as_str()
                .map(str::to_string)
                .or_else(|| v["source"].as_str().map(str::to_string))
        })
        .collect();
    for file in &files {
        let relative = rel(file, &source_root);
        if relative == unit_path || still_registered.contains(&relative) {
            continue;
        }
        git_rm(&root, file)?;
        removed += 1;
    }
    for header in &inlined {
        if header_still_used(&source_root, header)? {
            continue;
        }
        git_rm(&root, header)?;
        removed += 1;
    }
    let addresses: BTreeSet<u32> = owners.iter().map(|o| o.address).collect();
    let units = manifest["units"]
        .as_array_mut()
        .ok_or("translation-units.json: units must be an array")?;
    let before = units.len();
    units.retain(|u| {
        !(u["overlay"] == overlay.as_str()
            && u["owners"].as_array().is_some_and(|members| {
                members.iter().all(|m| {
                    u32::from_str_radix(
                        m["address"].as_str().unwrap_or("").trim_start_matches("0x"),
                        16,
                    )
                    .is_ok_and(|a| addresses.contains(&a))
                })
            }))
    });
    units.push(entry);
    let after = units.len();
    write_json(&manifest_path, &manifest)?;
    println!(
        "registered {} owners to {unit_path}; removed {removed} files; units {before} -> {after}",
        owners.len()
    );
    Ok(())
}

#[derive(Clone)]
struct Shape {
    kind: &'static str,
    ty: String,
    qualifiers: String,
}

impl Shape {
    fn text(&self) -> String {
        format!("{} {} {}", self.qualifiers, self.ty, self.kind)
            .trim()
            .to_string()
    }
    fn declaration(&self, name: &str) -> String {
        let q = if self.qualifiers.is_empty() {
            String::new()
        } else {
            format!("{} ", self.qualifiers)
        };
        match self.kind {
            "array" => format!("extern {q}{} {name}[];", self.ty),
            "pointer" => format!("extern {q}{} *{name};", self.ty),
            _ => format!("extern {q}{} {name};", self.ty),
        }
    }
    fn access(&self, name: &str) -> String {
        let q = if self.qualifiers.is_empty() {
            String::new()
        } else {
            format!("{} ", self.qualifiers)
        };
        match self.kind {
            "array" => format!("(({q}{} *){name})", self.ty),
            "pointer" => format!("(*({q}{} **){name})", self.ty),
            _ => format!("(*({q}{} *){name})", self.ty),
        }
    }
}

/// `extern [qualifiers] type [*]name[[]];` without a parameter list.
fn data_shape(text: &str) -> Option<(String, Shape)> {
    let t = text.trim();
    let body = t.strip_prefix("extern ")?.strip_suffix(';')?.trim();
    if body.contains('(') {
        return None;
    }
    let (body, kind) = match body.rfind('[') {
        Some(at) if body.ends_with(']') => (&body[..at], "array"),
        _ => (body, "scalar"),
    };
    let body = body.trim();
    let name_start = body.rfind(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))? + 1;
    let name = &body[name_start..];
    if name.is_empty() {
        return None;
    }
    let mut head = body[..name_start].trim().to_string();
    let kind = if head.ends_with('*') {
        head.pop();
        if kind == "array" {
            "array"
        } else {
            "pointer"
        }
    } else {
        kind
    };
    let mut qualifiers = Vec::new();
    let mut ty = Vec::new();
    for word in head.split_whitespace() {
        if word == "volatile" || word == "const" {
            qualifiers.push(word.to_string());
        } else {
            ty.push(word.to_string());
        }
    }
    if ty.is_empty() {
        return None;
    }
    Some((
        name.to_string(),
        Shape {
            kind,
            ty: ty.join(" "),
            qualifiers: qualifiers.join(" "),
        },
    ))
}

/// `[extern] ret name(params);` → (name, ret, params).
fn function_decl(text: &str) -> Option<(String, String, String)> {
    let t = text.trim().strip_suffix(';')?;
    let t = t.strip_prefix("extern ").unwrap_or(t);
    let open = t.find('(')?;
    let close = t.rfind(')')?;
    let head = t[..open].trim();
    let name_start = head.rfind(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))? + 1;
    let name = &head[name_start..];
    let ret = head[..name_start].trim();
    if name.is_empty() || ret.is_empty() || ret.starts_with("static") {
        return None;
    }
    Some((
        name.to_string(),
        ret.to_string(),
        t[open + 1..close].trim().to_string(),
    ))
}

fn callee_address(text: &str) -> Option<String> {
    let at = text.find("Func_")?;
    let hex: String = text[at + 5..].chars().take(8).collect();
    (hex.len() == 8
        && hex.chars().all(|c| c.is_ascii_hexdigit())
        && text[at + 13..].trim_start().starts_with('('))
    .then_some(hex)
}

/// Rename a wrapper's definition head (first occurrence before `(`) or, for
/// a body, every call of it.
fn rename_call(text: &str, from: &str, to: &str, first_only: bool) -> String {
    let mut out = String::new();
    let mut rest = text;
    let mut done = false;
    while let Some(at) = rest.find(from) {
        let before = &rest[..at];
        let after = &rest[at + from.len()..];
        let word = !before
            .chars()
            .last()
            .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_')
            && !after
                .chars()
                .next()
                .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
        let call = after.trim_start().starts_with('(');
        out.push_str(before);
        if word && call && !(first_only && done) {
            out.push_str(to);
            done = true;
        } else {
            out.push_str(from);
        }
        rest = after;
    }
    out.push_str(rest);
    out
}

fn replace_word(text: &str, word: &str, with: &str) -> String {
    let mut out = String::new();
    let mut rest = text;
    while let Some(at) = rest.find(word) {
        let before = &rest[..at];
        let after = &rest[at + word.len()..];
        let whole = !before
            .chars()
            .last()
            .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_')
            && !after
                .chars()
                .next()
                .is_some_and(|c| c.is_ascii_alphanumeric() || c == '_');
        out.push_str(before);
        out.push_str(if whole { with } else { word });
        rest = after;
    }
    out.push_str(rest);
    out
}

fn norm(s: &str) -> String {
    s.split_whitespace().collect::<Vec<_>>().join(" ")
}

fn rel(path: &Path, base: &Path) -> String {
    path.strip_prefix(base)
        .unwrap_or(path)
        .to_string_lossy()
        .into_owned()
}

/// The file's text with every `#include "name"` that names a file beside it
/// replaced by that file's text, recursively; headers from the include path
/// stay as directives.
fn expand_local_includes(path: &Path, inlined: &mut BTreeSet<PathBuf>) -> Result<String, String> {
    let text = fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
    let directory = path.parent().unwrap_or(Path::new("."));
    // An inlined header's include guard is meaningless in the flat unit.
    let mut lines: Vec<&str> = text.lines().collect();
    let guarded = lines.len() >= 3
        && lines[0].trim().starts_with("#ifndef ")
        && lines[1].trim().starts_with("#define ")
        && lines
            .iter()
            .rev()
            .find(|l| !l.trim().is_empty())
            .is_some_and(|l| l.trim() == "#endif");
    if guarded {
        while lines.last().is_some_and(|l| l.trim().is_empty()) {
            lines.pop();
        }
        lines.pop();
        lines.drain(0..2);
    }
    let mut out = String::new();
    for line in lines {
        let trimmed = line.trim();
        if let Some(name) = trimmed
            .strip_prefix("#include \"")
            .and_then(|r| r.strip_suffix('"'))
        {
            let local = directory.join(name);
            if local.is_file() {
                inlined.insert(local.clone());
                out.push_str(&expand_local_includes(&local, inlined)?);
                if !out.ends_with('\n') {
                    out.push('\n');
                }
                continue;
            }
        }
        out.push_str(line);
        out.push('\n');
    }
    Ok(out)
}

/// Top-level items of a source: preprocessor lines, comments, declarations
/// ending in `;`, and brace blocks (records, wrappers, functions).
fn items(text: &str, file: &Path) -> Vec<Item> {
    let lines: Vec<&str> = text.lines().collect();
    let mut out = Vec::new();
    let mut i = 0;
    let push = |out: &mut Vec<Item>, kind: &'static str, key: String, block: &[&str]| {
        out.push(Item {
            kind,
            key,
            text: block.join("\n"),
            from: file.to_path_buf(),
        });
    };
    while i < lines.len() {
        let line = lines[i];
        let t = line.trim();
        if t.is_empty() {
            i += 1;
            continue;
        }
        if t.starts_with("#include") {
            push(&mut out, "include", t.to_string(), &lines[i..=i]);
            i += 1;
            continue;
        }
        if t.starts_with("#define") {
            let key = t.split_whitespace().nth(1).unwrap_or("").to_string();
            push(&mut out, "define", key, &lines[i..=i]);
            i += 1;
            continue;
        }
        if t.starts_with("/*") {
            let mut j = i;
            while !lines[j].contains("*/") && j + 1 < lines.len() {
                j += 1;
            }
            push(&mut out, "comment", lines[i..=j].join("\n"), &lines[i..=j]);
            i = j + 1;
            continue;
        }
        if t.starts_with("//") {
            push(&mut out, "comment", t.to_string(), &lines[i..=i]);
            i += 1;
            continue;
        }
        let mut j = i;
        let mut depth = 0i32;
        let mut seen_brace = false;
        while j < lines.len() {
            for c in lines[j].chars() {
                if c == '{' {
                    depth += 1;
                    seen_brace = true;
                } else if c == '}' {
                    depth -= 1;
                }
            }
            if seen_brace && depth == 0 {
                break;
            }
            if !seen_brace && lines[j].trim_end().ends_with(';') {
                break;
            }
            j += 1;
        }
        let j = j.min(lines.len() - 1);
        let block = &lines[i..=j];
        let head = block[0].trim();
        i = j + 1;
        if !seen_brace {
            if head.starts_with("extern ") {
                let key = data_shape(head)
                    .map(|(n, _)| n)
                    .or_else(|| function_decl(head).map(|(n, _, _)| n))
                    .unwrap_or_else(|| head.to_string());
                push(&mut out, "extern", key, block);
            } else if let Some((name, _, _)) = function_decl(head) {
                push(&mut out, "prototype", name, block);
            } else if head.starts_with("typedef") {
                push(&mut out, "typedef", head.to_string(), block);
            } else {
                push(&mut out, "other", block.join("\n"), block);
            }
            continue;
        }
        if head.contains("__inline__") || head.starts_with("static inline") {
            let name = head
                .split('(')
                .next()
                .and_then(|h| h.split_whitespace().last())
                .unwrap_or(head)
                .trim_start_matches('*')
                .to_string();
            push(&mut out, "inline", name, block);
        } else if head.starts_with("struct")
            || head.starts_with("typedef struct")
            || head.starts_with("union")
            || head.starts_with("typedef union")
        {
            let tail = block[block.len() - 1].trim();
            let name = head
                .split_whitespace()
                .nth(if head.starts_with("typedef") { 2 } else { 1 })
                .filter(|w| *w != "{")
                .map(|w| w.trim_end_matches('{').to_string())
                .or_else(|| {
                    tail.strip_prefix('}')
                        .map(|r| r.trim().trim_end_matches(';').trim().to_string())
                })
                .unwrap_or_else(|| head.to_string());
            push(&mut out, "struct", name, block);
        } else if head.contains('(') && !head.ends_with(';') {
            let name = head
                .split('(')
                .next()
                .and_then(|h| h.split_whitespace().last())
                .unwrap_or(head)
                .trim_start_matches('*')
                .to_string();
            push(&mut out, "function", name, block);
        } else {
            push(&mut out, "other", block.join("\n"), block);
        }
    }
    out
}

fn header_still_used(source_root: &Path, header: &Path) -> Result<bool, String> {
    let name = header
        .file_name()
        .unwrap_or_default()
        .to_string_lossy()
        .into_owned();
    let needle = format!("#include \"{name}\"");
    let mut stack = vec![source_root.to_path_buf()];
    while let Some(dir) = stack.pop() {
        for entry in fs::read_dir(&dir).map_err(|e| format!("{}: {e}", dir.display()))? {
            let path = entry.map_err(|e| e.to_string())?.path();
            if path.is_dir() {
                stack.push(path);
            } else if path != header
                && path
                    .extension()
                    .is_some_and(|x| x == "c" || x == "h" || x == "inc")
            {
                if fs::read_to_string(&path)
                    .map_err(|e| format!("{}: {e}", path.display()))?
                    .contains(&needle)
                {
                    return Ok(true);
                }
            }
        }
    }
    Ok(false)
}

fn git_rm(root: &Path, path: &Path) -> Result<(), String> {
    let status = Command::new("git")
        .current_dir(root)
        .args(["rm", "-q", "-f"])
        .arg(path)
        .status()
        .map_err(|e| format!("git rm: {e}"))?;
    if !status.success() {
        fs::remove_file(path).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    Ok(())
}

fn read_json(path: &Path) -> Result<Value, String> {
    let text = fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))?;
    serde_json::from_str(&text).map_err(|e| format!("{}: {e}", path.display()))
}

fn write_json(path: &Path, value: &Value) -> Result<(), String> {
    let text = serde_json::to_string_pretty(value).map_err(|e| e.to_string())?;
    fs::write(path, format!("{text}\n")).map_err(|e| format!("{}: {e}", path.display()))
}
