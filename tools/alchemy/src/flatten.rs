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
//! Conflicting data and function declarations stay at block scope inside
//! the functions of the file that made each declaration; their types must
//! not change when the sources join. A wrapper spelling the scene work
//! pointer as a literal takes the symbol; two wrappers with one name and
//! different callees are told apart by the callee address. Whatever remains
//! is reported and stops the run: it is the shared interface the unit forces
//! into the open.

use crate::compiler::build_io::read_json;
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

pub fn entry(args: &[String]) -> ExitCode {
    match run(args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

fn run(args: &[String]) -> Result<(), String> {
    const USAGE: &str = "usage: alchemy unit flatten <game> <resource_NNN> --id <unit-id> --path <src-relative .c> [--owners <hex,hex,...>] [--partial] [--apply]";
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let apply = args.iter().any(|a| a == "--apply");
    let partial = args.iter().any(|a| a == "--partial");
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
            skip = a != "--apply" && a != "--partial" && args.get(i + 1).is_some();
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
        return Err(USAGE.into());
    };
    let root = std::env::current_dir().map_err(|e| e.to_string())?;
    let source_root = root.join("games").join(&game).join("src");
    let inventory: Value =
        read_json(&root.join(format!("out/{game}-en/full/rebuilt.owner-inventory.json")))?;
    let register_path = root.join(format!("games/{game}/source-paths.json"));
    let mut register: Value = read_json(&register_path)?;
    let manifest_path = root.join(format!("games/{game}/recon/translation-units.json"));
    let mut manifest: Value = read_json(&manifest_path)?;

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
    if retained > 0 && !partial {
        return Err(format!("{overlay}: {retained} owners are not exact C; pass --partial to consolidate the exact ones and leave the rest as assembly"));
    }
    if let Some(list) = flag("--owners") {
        let wanted = list
            .split(',')
            .map(str::trim)
            .filter(|value| !value.is_empty())
            .map(|value| u32::from_str_radix(value.trim_start_matches("0x"), 16))
            .collect::<Result<BTreeSet<u32>, _>>()
            .map_err(|error| format!("--owners: {error}"))?;
        let held: BTreeSet<u32> = owners.iter().map(|owner| owner.address).collect();
        let absent: Vec<String> = wanted
            .difference(&held)
            .map(|address| format!("{address:08x}"))
            .collect();
        if !absent.is_empty() {
            return Err(format!(
                "--owners names {} address(es) {overlay} has no exact C for: {}",
                absent.len(),
                absent.join(",")
            ));
        }
        owners.retain(|owner| wanted.contains(&owner.address));
    }
    if owners.is_empty() {
        return Err(format!("{overlay}: no owners"));
    }
    // Registered owners are not the full executable inventory: unregistered
    // code must also be closed before the overlay can be flattened as complete.
    let tree = crate::coverage::tree::work_tree_at(root.clone());
    let coverage =
        crate::coverage::pipeline::build_coverage_map(&crate::coverage::pipeline::BuildOptions {
            target: format!("{game}-en"),
            exact: &tree,
            recon: None,
        })?;
    if partial {
        if let Err(open) = check_overlay_coverage(&coverage.executable_areas, &overlay) {
            println!("  partial: {open}");
        }
    } else {
        check_overlay_coverage(&coverage.executable_areas, &overlay)?;
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
    let split = split_call_aliases(
        &root,
        &overlay,
        &owners,
        &mut parsed,
        &source_root,
        &manifest,
    )?;

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
            if let Some((_, name, _)) = declaration(&it.text) {
                let symbol = resolve(&name);
                if symbol != name {
                    it.text = replace_word(&it.text, &name, &symbol);
                }
            }
        }
    }
    // Reconcile data and function declarations.
    let mut declarations: BTreeMap<(&str, String), BTreeMap<PathBuf, String>> = BTreeMap::new();
    for (file, list) in &parsed {
        for it in list {
            if it.kind != "extern" && it.kind != "prototype" {
                continue;
            }
            if let Some((kind, name, text)) = declaration(&it.text) {
                declarations
                    .entry((kind, name))
                    .or_default()
                    .insert(file.clone(), text);
            }
        }
    }
    // A symbol two files declare with different shapes keeps each file's own
    // `extern` at block scope inside that file's functions, exactly the
    // declaration each owner compiled with. Identical declarations rise to
    // file scope once.
    let mut block_scoped: BTreeSet<String> = BTreeSet::new();
    let mut block_decls: BTreeMap<PathBuf, Vec<String>> = BTreeMap::new();
    let mut unified_decls = BTreeMap::new();
    let mut conflicts: Vec<String> = Vec::new();
    // A file that calls a function without declaring it compiled against the
    // implicit `int f()`; a prototype another file wrote must not reach it.
    let mut calls: BTreeMap<PathBuf, BTreeSet<String>> = BTreeMap::new();
    for (file, list) in &parsed {
        let set = calls.entry(file.clone()).or_default();
        for it in list
            .iter()
            .filter(|it| matches!(it.kind, "function" | "inline" | "other"))
        {
            let text = &it.text;
            let mut from = 0;
            while let Some(at) = text[from..].find("Func_") {
                let start = from + at;
                let end = start
                    + 5
                    + text[start + 5..]
                        .find(|c: char| !c.is_ascii_hexdigit())
                        .unwrap_or(text.len() - start - 5);
                if end - start == 13 && text[end..].trim_start().starts_with('(') {
                    set.insert(text[start..end].to_string());
                }
                from = end.max(start + 5);
            }
        }
    }
    // A function the unit itself defines: a prototype another owner wrote
    // with a different signature stays in that owner's functions only.
    let mut definitions = BTreeMap::new();
    for (_, list) in &parsed {
        for it in list.iter().filter(|it| it.kind == "function") {
            let head = it.text.lines().next().unwrap_or("");
            let head = head.trim_end().trim_end_matches('{').trim_end();
            if let Some((name, ret, params)) = function_decl(&format!("{head};")) {
                // A definition spelt through its alias define also defines
                // the registered name the prototypes use.
                let prefix = format!("#define {name} ");
                for target in list
                    .iter()
                    .filter(|d| d.kind == "define")
                    .filter_map(|d| d.text.trim().strip_prefix(&prefix))
                    .map(|rest| rest.trim().to_string())
                    .filter(|rest| rest.starts_with("Func_"))
                {
                    definitions.insert(target.clone(), prototype(&target, &ret, &params));
                }
                definitions.insert(name.clone(), prototype(&name, &ret, &params));
            }
        }
    }
    for ((kind, name), by_file) in &declarations {
        let mut distinct: BTreeSet<&String> = by_file.values().collect();
        let function = *kind == "prototype";
        if function {
            if let Some(text) = definitions.get(name) {
                distinct.insert(text);
            }
        }
        let implicit_user = function
            && parsed.iter().any(|(file, _)| {
                !by_file.contains_key(file) && calls.get(file).is_some_and(|c| c.contains(name))
            });
        if distinct.len() == 1 && !implicit_user {
            unified_decls.insert((*kind, name.clone()), (*distinct.first().unwrap()).clone());
            continue;
        }
        block_scoped.insert(name.clone());
        for (file, text) in by_file {
            block_decls
                .entry(file.clone())
                .or_default()
                .push(text.clone());
        }
    }
    if !block_scoped.is_empty() {
        println!(
            "  block-scoped prototypes: {}",
            block_scoped.iter().cloned().collect::<Vec<_>>().join(" ")
        );
    }
    // Two files defining one record layout, typedef, or macro name
    // differently each keep their own under a name suffixed with the file's
    // first owner address; that file's every use follows. Identical
    // definitions still rise to file scope once.
    {
        let mut first_text: BTreeMap<(&str, String), (PathBuf, String)> = BTreeMap::new();
        let mut renames: BTreeMap<PathBuf, Vec<(String, String)>> = BTreeMap::new();
        for (file, list) in &parsed {
            let suffix = owners
                .iter()
                .find(|o| &o.source == file)
                .map(|o| format!("{:08x}", o.address))
                .unwrap_or_else(|| "shared".into());
            for it in list {
                if !matches!(it.kind, "define" | "struct" | "typedef") {
                    continue;
                }
                let name = if it.kind == "define" {
                    it.key.split('(').next().unwrap_or(&it.key).to_string()
                } else {
                    it.key.clone()
                };
                let slot = (it.kind, name.clone());
                match first_text.get(&slot) {
                    None => {
                        first_text.insert(slot, (file.clone(), norm(&it.text)));
                    }
                    Some((origin, text)) if origin != file && *text != norm(&it.text) => {
                        let fresh = format!("{name}_{suffix}");
                        let list = renames.entry(file.clone()).or_default();
                        if !list.iter().any(|(from, _)| *from == name) {
                            println!(
                                "  {} {name} -> {fresh} in {}",
                                it.kind,
                                rel(file, &source_root)
                            );
                            list.push((name.clone(), fresh));
                        }
                    }
                    Some(_) => {}
                }
            }
        }
        for (file, list) in parsed.iter_mut() {
            let Some(map) = renames.get(file) else {
                continue;
            };
            for it in list.iter_mut() {
                if it.kind == "include" {
                    continue;
                }
                for (from, to) in map {
                    it.text = replace_word(&it.text, from, to);
                    if it.key == *from || it.key.starts_with(&format!("{from}(")) {
                        it.key = it.key.replacen(from, to, 1);
                    }
                }
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
                    if let Some((kind, name, _)) = declaration(&it.text) {
                        let (entries, order) = if kind == "extern" {
                            (&mut externs, &mut extern_order)
                        } else {
                            (&mut prototypes, &mut prototype_order)
                        };
                        if !block_scoped.contains(&name) && !entries.contains_key(&name) {
                            let mut unified = it.clone();
                            unified.text = unified_decls
                                .get(&(kind, name.clone()))
                                .cloned()
                                .unwrap_or_else(|| it.text.clone());
                            order.push(name.clone());
                            entries.insert(name, unified);
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
                            // The file spelt the literal, so it declared no
                            // symbol; the wrapper now needs one in scope.
                            if block_scoped.contains("Data_03001ebc") {
                                let decls = block_decls.entry(have.from.clone()).or_default();
                                if !decls.iter().any(|d| d.contains("Data_03001ebc")) {
                                    decls.push("extern u8 Data_03001ebc[];".into());
                                }
                            }
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
                "dropped" => {}
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
    if !conflicts.is_empty() {
        let mut message = format!("{overlay}: {} declaration conflicts:", conflicts.len());
        for c in &conflicts {
            message.push_str(&format!("\n  {c}"));
        }
        return Err(message);
    }

    // Owner names: the register's name, else the file's alias for the address.
    // Alias defines the shared headers carry (`#define Name Func_xxxx` in
    // games/<game>/include), so a function defined under such a name still
    // links by its address.
    let mut header_aliases: BTreeMap<String, Vec<String>> = BTreeMap::new();
    if let Ok(entries) = fs::read_dir(root.join("games").join(&game).join("include")) {
        for entry in entries.flatten() {
            let path = entry.path();
            if path.extension().is_some_and(|x| x == "h") {
                if let Ok(text) = fs::read_to_string(&path) {
                    for line in text.lines() {
                        let words: Vec<&str> = line.split_whitespace().collect();
                        if words.len() == 3
                            && words[0] == "#define"
                            && words[2].starts_with("Func_")
                            && !words[1].contains('(')
                        {
                            header_aliases
                                .entry(words[2].to_string())
                                .or_default()
                                .push(words[1].to_string());
                        }
                    }
                }
            }
        }
    }
    let alias_for = |o: &Owner| -> String {
        let want = format!("Func_{:08x}", o.address);
        for (name, it) in &defines {
            if norm(&it.text).ends_with(&format!(" {want}")) {
                return name.clone();
            }
        }
        // Several overlays share addresses, so a header may alias one
        // address under several names: the one this file defines wins.
        if let Some(names) = header_aliases.get(&want) {
            if let Some(name) = names
                .iter()
                .find(|n| functions.iter().any(|f| f.from == o.source && &f.key == *n))
            {
                return name.clone();
            }
        }
        // The registered name is what the file defines when it carries no
        // alias define of its own.
        if let Some(name) = register["owners"][&o.key]["name"].as_str() {
            return name.to_string();
        }
        o.name.clone()
    };
    let aliases: Vec<String> = owners.iter().map(alias_for).collect();
    // The unit is linked by `Func_<address>` symbols. A function defined
    // under its registered name without an alias define gets one, so the
    // symbol exists; a file whose definition matches neither is reported
    // with what it does define.
    let mut alias_defines: Vec<String> = Vec::new();
    for (o, alias) in owners.iter().zip(&aliases) {
        let legacy = format!("Func_{:08x}", o.address);
        let defined = functions
            .iter()
            .filter(|f| f.from == o.source)
            .map(|f| f.key.clone())
            .collect::<Vec<_>>();
        if defined.iter().any(|k| k == &legacy) {
            continue;
        }
        if defined.iter().any(|k| k == alias) {
            if !defines.contains_key(alias) {
                alias_defines.push(format!("#define {alias} {legacy}"));
            }
            continue;
        }
        return Err(format!(
            "{}: no function named {alias} or {legacy} in its source {} (it defines: {})",
            o.key,
            rel(&o.source, &source_root),
            defined.join(", ")
        ));
    }

    // Order: the shared types, then every alias define (an included header
    // may declare through an alias, as the owners' files did), then the
    // other includes, layouts, declarations, wrappers, and the functions.
    let mut out: Vec<String> = vec!["#include \"types.h\"".into()];
    out.push(String::new());
    for name in &define_order {
        out.push(defines[name].text.clone());
    }
    for define in &alias_defines {
        out.push(define.clone());
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
    // A body from a file with block-scoped prototypes opens with them.
    let with_decls = |it: &Item| -> String {
        let Some(decls) = block_decls.get(&it.from) else {
            return it.text.clone();
        };
        let mut lines: Vec<String> = it.text.lines().map(str::to_string).collect();
        // The body opens on its own `{` line or at the end of the head line.
        let open = lines
            .iter()
            .position(|l| l.trim() == "{")
            .or_else(|| lines.iter().position(|l| l.trim_end().ends_with('{')));
        if let Some(open) = open {
            for (offset, decl) in decls.iter().enumerate() {
                lines.insert(open + 1 + offset, format!("    {decl}"));
            }
            if decls.len() > 0
                && lines
                    .get(open + 1 + decls.len())
                    .is_some_and(|l| !l.trim().is_empty())
            {
                lines.insert(open + 1 + decls.len(), String::new());
            }
        }
        lines.join("\n")
    };
    for it in &inlines {
        out.push(with_decls(it));
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
            out.push(with_decls(f));
            out.push(String::new());
        }
    }
    for f in &functions {
        if emitted.insert(f.text.clone()) {
            out.push(with_decls(f));
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
        "absolute_symbols": split
            .iter()
            .map(|(alias, (address, kind))| {
                (
                    alias.clone(),
                    json!({"address": format!("0x{address:08x}"), "kind": kind}),
                )
            })
            .collect::<Map<String, Value>>(),
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

/// `extern [qualifiers] type [*...]name[dims];` without a parameter list.
fn data_shape(text: &str) -> Option<(String, String)> {
    let t = text.trim();
    let body = t.strip_prefix("extern ")?.strip_suffix(';')?.trim();
    if body.contains('(') {
        return None;
    }
    let (body, dims) = match body.find('[') {
        Some(at) if body.ends_with(']') => (&body[..at], body[at..].to_string()),
        _ => (body, String::new()),
    };
    let body = body.trim();
    let name_start = body.rfind(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))? + 1;
    let name = &body[name_start..];
    if name.is_empty() {
        return None;
    }
    let mut head = body[..name_start].trim().to_string();
    let mut stars = 0;
    while head.ends_with('*') {
        head.pop();
        stars += 1;
        head = head.trim_end().to_string();
    }
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
    let mut words = vec!["extern".to_string()];
    words.extend(qualifiers);
    words.extend(ty);
    Some((
        name.to_string(),
        format!("{} {}{name}{dims};", words.join(" "), "*".repeat(stars)),
    ))
}

fn prototype(name: &str, ret: &str, params: &str) -> String {
    let space = if ret.ends_with('*') { "" } else { " " };
    format!("{ret}{space}{name}({params});")
}

fn declaration(text: &str) -> Option<(&'static str, String, String)> {
    if let Some((name, text)) = data_shape(text) {
        Some(("extern", name, text))
    } else {
        let (name, ret, params) = function_decl(text)?;
        let text = prototype(&name, &ret, &params);
        Some(("prototype", name, text))
    }
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
    let mut done = false;
    replace_words(text, from, to, |after| {
        if after.trim_start().starts_with('(') && !(first_only && done) {
            done = true;
            true
        } else {
            false
        }
    })
}

/// Overlay call identity is per owner. A legacy `Func_<addr>` name is derived
/// from a reference call displacement, so one name can reach two runtime slots
/// -- inside one owner, or across the owners now sharing a translation unit.
/// Compiled alone, each owner carried its own declarations; the units this
/// flatten supersedes hold them. Carry every superseded declaration forward,
/// add the bindings only the merge makes visible, rename whatever collides,
/// and return the complete alias -> runtime address map for the new unit.
fn split_call_aliases(
    root: &Path,
    overlay: &str,
    owners: &[Owner],
    parsed: &mut [(PathBuf, Vec<Item>)],
    source_root: &Path,
    manifest: &Value,
) -> Result<BTreeMap<String, (u64, String)>, String> {
    use crate::compiler::overlay::{call_symbols, RESOURCE_BASE};
    let addresses: BTreeSet<u32> = owners.iter().map(|owner| owner.address).collect();
    let file_of: BTreeMap<u32, PathBuf> = owners
        .iter()
        .map(|owner| (owner.address, owner.source.clone()))
        .collect();

    // What each file already compiled against.
    let mut effective: BTreeMap<PathBuf, BTreeMap<String, (u64, String)>> = BTreeMap::new();
    for unit in manifest["units"].as_array().into_iter().flatten() {
        if unit["overlay"] != overlay {
            continue;
        }
        let members: Vec<u32> = unit["owners"]
            .as_array()
            .into_iter()
            .flatten()
            .filter_map(|member| {
                u32::from_str_radix(
                    member["address"]
                        .as_str()
                        .unwrap_or("")
                        .trim_start_matches("0x"),
                    16,
                )
                .ok()
            })
            .collect();
        if !members.iter().any(|address| addresses.contains(address)) {
            continue;
        }
        let Some(symbols) = unit["absolute_symbols"].as_object() else {
            continue;
        };
        for address in members.iter().filter(|address| addresses.contains(address)) {
            let Some(file) = file_of.get(address) else {
                continue;
            };
            let map = effective.entry(file.clone()).or_default();
            for (alias, symbol) in symbols {
                let Some(value) = symbol["address"]
                    .as_str()
                    .and_then(|text| u64::from_str_radix(text.trim_start_matches("0x"), 16).ok())
                else {
                    return Err(format!("{}: {alias} has no address", unit["id"]));
                };
                let kind = symbol["kind"].as_str().unwrap_or("thumb").to_string();
                if map
                    .insert(alias.clone(), (value, kind))
                    .is_some_and(|had| had.0 != value)
                {
                    return Err(format!(
                        "{}: {alias} is declared twice with different addresses",
                        rel(file, source_root)
                    ));
                }
            }
        }
    }

    // What the reference says each owner actually calls.
    let image = crate::overlay::rom::canonical_overlay(root, overlay)?;
    for owner in owners {
        let offset = owner
            .address
            .checked_sub(RESOURCE_BASE)
            .ok_or_else(|| format!("owner {:08x} precedes the overlay base", owner.address))?
            as usize;
        let map = effective.entry(owner.source.clone()).or_default();
        for (name, found) in call_symbols(&image, offset, owner.extent)? {
            if found.len() == 1 {
                let target = *found.iter().next().unwrap();
                if let Some((declared, _)) = map.get(&name) {
                    if *declared != target && !map.values().any(|(address, _)| *address == target) {
                        return Err(format!(
                            "{}: {name} binds {target:08x} but its unit declares {declared:08x}",
                            rel(&owner.source, source_root)
                        ));
                    }
                    continue;
                }
                map.insert(name, (target, "thumb".into()));
                continue;
            }
            // Several slots inside one owner: only that owner's own
            // declarations can say which call site reaches which.
            let variant = format!("{name}_");
            let covered: BTreeSet<u64> = map
                .iter()
                .filter(|(alias, _)| **alias == name || alias.starts_with(&variant))
                .map(|(_, (address, _))| *address)
                .collect();
            let missing: Vec<u64> = found.difference(&covered).copied().collect();
            match missing.as_slice() {
                [] => {}
                [only] if !map.contains_key(&name) => {
                    map.insert(name, (*only, "thumb".into()));
                }
                _ => println!(
                    "  note: {name} reaches {} runtime slots inside {} and {} are declared",
                    found.len(),
                    rel(&owner.source, source_root),
                    covered.len()
                ),
            }
        }
    }

    // Merge. The first file to claim a name keeps it; a file that needs the
    // same name for a different slot gets a fresh alias and is rewritten.
    let mut declared: BTreeMap<String, (u64, String)> = BTreeMap::new();
    let mut taken: BTreeSet<String> = BTreeSet::new();
    let order: Vec<PathBuf> = parsed.iter().map(|(file, _)| file.clone()).collect();
    for file in &order {
        for alias in effective.get(file).into_iter().flat_map(|map| map.keys()) {
            taken.insert(alias.clone());
        }
    }
    let mut splits = 0;
    for file in &order {
        let Some(map) = effective.get(file).cloned() else {
            continue;
        };
        for (name, (address, kind)) in map {
            match declared.get(&name) {
                None => {
                    declared.insert(name.clone(), (address, kind));
                    taken.insert(name);
                }
                Some((had, _)) if *had == address => {}
                Some(_) => {
                    let mut index = 0;
                    let mut alias = format!("{name}_{}", suffix(index));
                    while taken.contains(&alias) {
                        index += 1;
                        alias = format!("{name}_{}", suffix(index));
                    }
                    println!(
                        "  split call {name} -> {alias} in {}",
                        rel(file, source_root)
                    );
                    splits += 1;
                    declared.insert(alias.clone(), (address, kind));
                    taken.insert(alias.clone());
                    for (at, list) in parsed.iter_mut() {
                        if at != file {
                            continue;
                        }
                        for item in list.iter_mut() {
                            if item.kind == "include" {
                                continue;
                            }
                            item.text = replace_word(&item.text, &name, &alias);
                            if item.key == name {
                                item.key = alias.clone();
                            }
                        }
                    }
                }
            }
        }
    }
    if splits > 0 {
        println!("  {splits} call aliases split across owners");
    }
    // Only names that actually needed a decision are worth declaring; a name
    // with one binding everywhere still resolves from the reference.
    let ambiguous: BTreeSet<String> = declared
        .keys()
        .filter(|alias| {
            let base = alias
                .rsplit_once('_')
                .map_or(alias.as_str(), |(head, _)| head);
            declared.keys().any(|other| {
                other.as_str() != alias.as_str()
                    && (other.as_str() == base || other.starts_with(&format!("{base}_")))
            })
        })
        .cloned()
        .collect();
    Ok(declared
        .into_iter()
        .filter(|(alias, _)| ambiguous.contains(alias))
        .collect())
}

/// `a`..`z`, then `aa`, `ab`, ... so a name with many bindings still reads.
fn suffix(index: usize) -> String {
    let mut value = index;
    let mut out = String::new();
    loop {
        out.insert(0, (b'a' + (value % 26) as u8) as char);
        if value < 26 {
            return out;
        }
        value = value / 26 - 1;
    }
}

/// `__attribute__((...))` may sit anywhere in a declarator. Remove every
/// group with its balanced parentheses so the declared name is what remains.
fn strip_attributes(head: &str) -> String {
    let mut out = head.to_string();
    while let Some(at) = out.find("__attribute__") {
        let Some(open) = out[at..].find('(').map(|offset| at + offset) else {
            out.replace_range(at..at + "__attribute__".len(), "");
            continue;
        };
        let mut depth = 0i32;
        let mut end = None;
        for (offset, character) in out[open..].char_indices() {
            match character {
                '(' => depth += 1,
                ')' => {
                    depth -= 1;
                    if depth == 0 {
                        end = Some(open + offset + 1);
                        break;
                    }
                }
                _ => {}
            }
        }
        match end {
            Some(close) => out.replace_range(at..close, ""),
            None => {
                out.replace_range(at..at + "__attribute__".len(), "");
            }
        }
    }
    out
}

fn replace_word(text: &str, word: &str, with: &str) -> String {
    replace_words(text, word, with, |_| true)
}

fn replace_words(
    text: &str,
    word: &str,
    with: &str,
    mut accept: impl FnMut(&str) -> bool,
) -> String {
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
        out.push_str(if whole && accept(after) { with } else { word });
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
/// A comment written above a declaration belongs to it. Merging owners must
/// not strand it at the top of the module, so it travels inside the item it
/// introduces, whatever blank lines separate the two.
fn attach_comments(mut list: Vec<Item>) -> Vec<Item> {
    let mut out: Vec<Item> = Vec::new();
    let mut held: Vec<Item> = Vec::new();
    for item in list.drain(..) {
        if item.kind == "adjacent" {
            continue;
        }
        if item.kind == "comment" {
            held.push(item);
            continue;
        }
        // Only a definition carries its comment. Declarations are reconciled
        // by text and deduplicated across files, so a comment merged into one
        // would make two identical declarations look like a conflict.
        if held.is_empty() || !matches!(item.kind, "function" | "inline" | "struct" | "typedef") {
            out.append(&mut held);
            out.push(item);
            continue;
        }
        let lead = held
            .drain(..)
            .map(|comment| comment.text)
            .collect::<Vec<_>>()
            .join("\n\n");
        let mut item = item;
        item.text = format!("{lead}\n{}", item.text);
        out.push(item);
    }
    out.append(&mut held);
    out
}

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
            out.push(Item {
                kind: "adjacent",
                key: String::new(),
                text: String::new(),
                from: file.to_path_buf(),
            });
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
        // A trailing `/* comment */` after a declaration does not extend it.
        let code_of = |line: &str| -> String {
            let mut code = line.to_string();
            while let Some(open) = code.rfind("/*") {
                match code[open..].find("*/") {
                    Some(close) => code.replace_range(open..open + close + 2, ""),
                    None => break,
                }
            }
            code
        };
        while j < lines.len() {
            let code = code_of(lines[j]);
            for c in code.chars() {
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
            if !seen_brace && code.trim_end().ends_with(';') {
                break;
            }
            j += 1;
        }
        let j = j.min(lines.len() - 1);
        let block = &lines[i..=j];
        let head = block[0].trim();
        i = j + 1;
        // Several declarations on one line are several items.
        if !seen_brace && block.len() == 1 && code_of(head).matches(';').count() > 1 {
            let mut depth = 0i32;
            let mut start = 0usize;
            let text = code_of(head);
            for (index, c) in text.char_indices() {
                match c {
                    '(' | '[' => depth += 1,
                    ')' | ']' => depth -= 1,
                    ';' if depth == 0 => {
                        let piece = text[start..=index].trim();
                        if !piece.is_empty() {
                            out.extend(items(piece, file));
                        }
                        start = index + 1;
                    }
                    _ => {}
                }
            }
            continue;
        }
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
        let inline = head.contains("__inline__") || head.starts_with("static inline");
        let record = ["struct", "typedef struct", "union", "typedef union"]
            .iter()
            .any(|prefix| head.starts_with(prefix))
            && !head.split('{').next().unwrap_or(head).contains('(');
        if inline || (!record && head.contains('(') && !head.ends_with(';')) {
            let bare = strip_attributes(head);
            let name = bare
                .split('(')
                .next()
                .and_then(|h| h.split_whitespace().last())
                .unwrap_or(&bare)
                .trim_start_matches('*')
                .to_string();
            push(
                &mut out,
                if inline { "inline" } else { "function" },
                name,
                block,
            );
        } else if record {
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
        } else {
            push(&mut out, "other", block.join("\n"), block);
        }
    }
    attach_comments(out)
}

fn header_still_used(source_root: &Path, header: &Path) -> Result<bool, String> {
    let name = header.file_name().unwrap_or_default().to_string_lossy();
    let needle = format!("#include \"{name}\"");
    for entry in walkdir::WalkDir::new(source_root).follow_links(true) {
        let entry = entry.map_err(|e| e.to_string())?;
        let path = entry.path();
        if !entry.file_type().is_dir()
            && path != header
            && path
                .extension()
                .is_some_and(|x| x == "c" || x == "h" || x == "inc")
            && fs::read_to_string(path)
                .map_err(|e| format!("{}: {e}", path.display()))?
                .contains(&needle)
        {
            return Ok(true);
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

fn write_json(path: &Path, value: &Value) -> Result<(), String> {
    let text = serde_json::to_string_pretty(value).map_err(|e| e.to_string())?;
    fs::write(path, format!("{text}\n")).map_err(|e| format!("{}: {e}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn flatten_rejects_unregistered_holes_and_missing_coverage() {
        use crate::coverage::model::{area, Category, Tile};
        let tile = |category: Category| {
            let mut tile = Tile {
                bytes: 4,
                group: Some("test".into()),
                ..Tile::default()
            };
            tile.categories[category as usize] = 4;
            tile
        };
        let mut areas = vec![area(
            "overlays",
            "Overlays",
            vec![tile(Category::ProvenC), tile(Category::ProvenAsm)],
        )];
        assert!(check_overlay_coverage(&areas, "resource_test").is_ok());
        assert!(check_overlay_coverage(&areas, "resource_absent").is_err());
        for category in [Category::Unknown, Category::DraftC, Category::DraftAsm] {
            areas[0].tiles.push(tile(category));
            assert!(check_overlay_coverage(&areas, "resource_test")
                .unwrap_err()
                .contains("4 unresolved"));
            areas[0].tiles.pop();
        }
    }

    #[test]
    fn declarations_keep_qualifiers_dimensions_and_prototype_shapes() {
        for (input, kind, expected) in [
            ("extern u8 Data[];", "extern", "extern u8 Data[];"),
            (
                "extern const volatile struct Work **Data[][64];",
                "extern",
                "extern const volatile struct Work **Data[][64];",
            ),
            ("extern u8 const Data;", "extern", "extern const u8 Data;"),
            (
                "extern void *Func(s32 n);",
                "prototype",
                "void *Func(s32 n);",
            ),
            ("int Func();", "prototype", "int Func();"),
            ("int Func(void);", "prototype", "int Func(void);"),
        ] {
            let (actual_kind, _, text) = declaration(input).unwrap();
            assert_eq!((actual_kind, text.as_str()), (kind, expected));
        }
        assert!(declaration("static int Func(void);").is_none());
        assert_eq!(
            prototype("Func_12345678", "int", "int Func"),
            "int Func_12345678(int Func);"
        );
    }

    #[test]
    fn replacements_distinguish_identifiers_calls_and_first_definition() {
        let text = "Call xCall Call_x Call(); Call \n(1); &Call;";
        assert_eq!(
            replace_word(text, "Call", "Next"),
            "Next xCall Call_x Next(); Next \n(1); &Next;"
        );
        assert_eq!(
            rename_call(text, "Call", "Next", false),
            "Call xCall Call_x Next(); Next \n(1); &Call;"
        );
        assert_eq!(
            rename_call(text, "Call", "Next", true),
            "Call xCall Call_x Next(); Call \n(1); &Call;"
        );
    }
}

fn check_overlay_coverage(
    areas: &[crate::coverage::model::Area],
    overlay: &str,
) -> Result<(), String> {
    use crate::coverage::model::Category;
    let group = overlay
        .strip_prefix("resource_")
        .ok_or("invalid overlay ID")?;
    let tiles: Vec<_> = areas
        .iter()
        .filter(|area| area.id == "overlays")
        .flat_map(|area| &area.tiles)
        .filter(|tile| tile.group.as_deref() == Some(group))
        .collect();
    let unresolved: i64 = tiles
        .iter()
        .map(|tile| {
            tile.bytes
                - tile.categories[Category::ProvenC as usize]
                - tile.categories[Category::ProvenAsm as usize]
        })
        .sum();
    if tiles.is_empty() || unresolved != 0 {
        return Err(format!("{overlay}: {unresolved} unresolved executable bytes or missing coverage; registered owners alone cannot prove completion"));
    }
    Ok(())
}
