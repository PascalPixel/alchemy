use crate::c_items::{blank_line_before, items, line_marker, names, normalized, Item, Kind, Space};
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

#[derive(Clone, Default)]
pub(crate) struct Known {
    pub(crate) defined: HashMap<(Space, String), String>,
    pub(crate) declared: HashMap<(Space, String), String>,
    pub(crate) texts: HashSet<String>,
}

impl Known {
    fn add(&mut self, item: &Item, origin: &str) {
        for name in names(item) {
            let key = (name.space, name.name);
            if name.defines {
                self.defined.entry(key.clone()).or_insert_with(|| origin.to_string());
            }
            self.declared.entry(key).or_insert(name.signature);
        }
        self.texts.insert(normalized(item));
    }

    pub(crate) fn of_headers(expanded: &str, main: &str) -> Known {
        let mut known = Known::default();
        let mut file = String::from("<built-in>");
        for item in items(expanded) {
            if let Some(marked) = line_marker(&item) {
                file = marked;
            } else if file != main {
                known.add(&item, &file);
            }
        }
        known
    }
}

pub(crate) fn with_includes(module: &str, draft: &str) -> (String, Vec<String>) {
    let module_items = items(module);
    let mut have: HashSet<String> = module_items
        .iter()
        .filter_map(|i| match &i.kind {
            Kind::Include(header) => Some(header.clone()),
            _ => None,
        })
        .collect();
    let added: Vec<String> = items(draft)
        .iter()
        .filter_map(|i| match &i.kind {
            Kind::Include(header) if have.insert(header.clone()) => Some(i.text.trim().to_string()),
            _ => None,
        })
        .collect();
    if added.is_empty() {
        return (module.to_string(), added);
    }
    let code = module_items
        .iter()
        .position(|i| matches!(i.kind, Kind::Declaration | Kind::Function { .. }))
        .unwrap_or(module_items.len());
    let lines = added.join("\n") + "\n";
    let (at, text) = match module_items[..code].iter().rev().find(|i| matches!(i.kind, Kind::Include(_))) {
        Some(last) => match module[last.end..].find('\n') {
            Some(newline) => (last.end + newline + 1, lines),
            None => (module.len(), format!("\n{lines}")),
        },
        None => {
            let first = module_items[..code].iter().find(|i| i.kind != Kind::Comment).or(module_items.get(code));
            match first {
                Some(item) => (item.start, lines + "\n"),
                None => (module.len(), lines),
            }
        }
    };
    (format!("{}{text}{}", &module[..at], &module[at..]), added)
}

pub(crate) struct Merged {
    pub text: String,
    pub defined: Vec<String>,
    pub present: usize,
    pub differing: Vec<String>,
}

fn head_position(module_items: &[Item]) -> Option<usize> {
    let first = module_items.iter().position(|i| matches!(i.kind, Kind::Function { .. }))?;
    let comments = module_items[..first].iter().rev().take_while(|i| i.kind == Kind::Comment).count();
    Some(module_items[first - comments].start)
}

enum Fate {
    Keep,
    Present,
    Refuse(String),
}

pub(crate) fn merge(module: &str, draft: &str, headers: &Known, head: bool, module_name: &str) -> Result<Merged, String> {
    let module_items = items(module);
    let at = if head { head_position(&module_items).unwrap_or(module.len()) } else { module.len() };
    let (mut before, mut after) = (Known::default(), Known::default());
    for item in &module_items {
        let side = if item.start < at { &mut before } else { &mut after };
        side.add(item, module_name);
    }

    let draft_items = items(draft);
    let preamble = draft_items.iter().take_while(|i| matches!(i.kind, Kind::Comment | Kind::Include(_))).count();
    let (mut kept, mut pending) = (Vec::new(), Vec::new());
    let (mut refusals, mut differing, mut defined, mut present) = (Vec::new(), Vec::new(), Vec::new(), 0);
    for item in &draft_items[preamble..] {
        match item.kind {
            Kind::Comment => {
                pending.push(item);
                continue;
            }
            Kind::Include(_) => continue,
            _ => {}
        }
        let comments = std::mem::take(&mut pending);
        match fate(item, headers, &before, &after, &mut differing) {
            Fate::Keep => {
                defined.extend(names(item).into_iter().filter(|n| n.defines).map(|n| n.describe()));
                kept.extend(comments);
                kept.push(item);
            }
            Fate::Present => present += 1,
            Fate::Refuse(why) => refusals.push(why),
        }
    }
    kept.extend(pending);
    differing.sort();
    differing.dedup();
    if !refusals.is_empty() {
        return Err(format!("the draft cannot merge into {module_name}:\n  {}", refusals.join("\n  ")));
    }

    let mut block = String::new();
    for (n, item) in kept.iter().enumerate() {
        if n > 0 {
            block.push_str(if blank_line_before(draft, item.start) { "\n\n" } else { "\n" });
        }
        block.push_str(item.text);
    }
    let text = if block.is_empty() {
        module.to_string()
    } else if at < module.len() {
        format!("{}{block}\n\n{}", &module[..at], &module[at..])
    } else {
        format!("{}\n\n{block}\n", module.trim_end())
    };
    Ok(Merged { text, defined, present, differing })
}

fn fate(item: &Item, headers: &Known, before: &Known, after: &Known, differing: &mut Vec<String>) -> Fate {
    let item_names = names(item);
    let text = normalized(item);
    if item_names.iter().any(|n| n.defines) {
        if headers.texts.contains(&text) || before.texts.contains(&text) {
            return Fate::Present;
        }
        let mut clashes = Vec::new();
        for name in item_names.iter().filter(|n| n.defines) {
            let key = (name.space, name.name.clone());
            if let Some(origin) = headers.defined.get(&key).or(before.defined.get(&key)) {
                clashes.push(format!("{} is already defined in {origin}", name.describe()));
            } else if after.defined.contains_key(&key) {
                clashes.push(format!(
                    "{} is defined below the module's first function, where --head would use it first",
                    name.describe()
                ));
            }
        }
        if !clashes.is_empty() {
            return Fate::Refuse(clashes.join("; "));
        }
    }
    let mut all_known = !item_names.is_empty();
    for name in &item_names {
        let key = (name.space, name.name.clone());
        match headers.declared.get(&key).or(before.declared.get(&key)) {
            Some(signature) if *signature != name.signature => differing.push(name.describe()),
            Some(_) => {}
            None => all_known = false,
        }
    }
    if all_known && !item_names.iter().any(|n| n.defines) {
        Fate::Present
    } else {
        Fate::Keep
    }
}

fn read(path: &Path) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display()))
}

pub(crate) fn header_names(root: &Path, target: DecompTarget, module: &Path, text: &str) -> Result<Known, String> {
    let directory = tempfile::Builder::new()
        .prefix("alchemy-adopt-into-")
        .tempdir()
        .map_err(|e| e.to_string())?;
    let source = directory.path().join("module.c");
    std::fs::write(&source, text).map_err(|e| format!("{}: {e}", source.display()))?;
    let imports = crate::build_text::fresh_c_imports(root, target)?;
    let routing = module.strip_prefix(root).unwrap_or(module).to_string_lossy().into_owned();
    let mut command = crate::compiler::preprocess::command(target, &routing, false)?;
    if let Some(imports) = &imports {
        command.insert(1, format!("-I{}", imports.path().display()));
    }
    if let Some(parent) = module.parent() {
        command.insert(1, format!("-I{}", parent.display()));
    }
    let main = source.to_string_lossy().into_owned();
    command.extend(["-dD".to_string(), main.clone()]);
    let expanded = crate::process::run(&command, root).map_err(|e| format!("{routing}: {e}"))?;
    Ok(Known::of_headers(&expanded, &main))
}

fn module_path(root: &Path, module: &Path) -> Result<PathBuf, String> {
    let path = if module.is_file() { module.to_path_buf() } else { root.join(module) };
    let path = std::fs::canonicalize(&path).map_err(|e| format!("{}: {e}", module.display()))?;
    let canonical_root = std::fs::canonicalize(root).map_err(|e| e.to_string())?;
    match path.strip_prefix(&canonical_root) {
        Ok(relative) if relative.starts_with("games") => Ok(root.join(relative)),
        _ => Err(format!("{}: the module must be source under games/", module.display())),
    }
}

pub(crate) fn run(root: &Path, game: &str, address: &str, module: &Path, head: bool) -> Result<(), String> {
    let draft_name = format!("recon/{game}/en/main/{address}.c");
    let draft = read(&root.join(&draft_name))?;
    let module = module_path(root, module)?;
    let module_name = module.strip_prefix(root).unwrap_or(&module).display().to_string();
    let (with, includes) = with_includes(&read(&module)?, &draft);
    let headers = header_names(root, decomp_target(Some(&format!("{game}-en")))?, &module, &with)?;
    let merged = merge(&with, &draft, &headers, head, &module_name).map_err(|e| format!("{draft_name}: {e}"))?;
    std::fs::write(&module, &merged.text).map_err(|e| format!("{}: {e}", module.display()))?;
    let tracked = crate::process::run(&["git", "ls-files", "--error-unmatch", &draft_name], root).is_ok();
    if tracked {
        crate::process::run(&["git", "rm", "-q", "-f", &draft_name], root)?;
    } else {
        std::fs::remove_file(root.join(&draft_name)).map_err(|e| format!("{draft_name}: {e}"))?;
    }
    println!("merged {draft_name} into {module_name}");
    for include in &includes {
        println!("  added {include}");
    }
    if !merged.defined.is_empty() {
        println!("  defines {}", merged.defined.join(", "));
    }
    if merged.present > 0 {
        println!("  left {} of the draft's declarations to the module, which has them", merged.present);
    }
    if !merged.differing.is_empty() {
        println!(
            "  the module or its headers declare {} otherwise than the draft did; check they agree",
            merged.differing.join(", ")
        );
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    const MODULE: &str = "/* Shop module. */\n#include \"TYPES.H\"\n#include \"SHOP.H\"\n\nstruct Slot {\n    u8 id;\n};\n\nvoid Shop_Close(void);\n\nvoid Shop_Open(void)\n{\n    Shop_Close();\n}\n";

    const DRAFT: &str = "/* Draft: Shop_Count, 2026-10-05.\n * Score 0: exact. */\n#include \"TYPES.H\"\n#include \"ITEM.H\"\n\nstruct Slot {\n    u8 id;\n};\nvoid Shop_Close(void);\ns32 Item_Count(u32 id);\n\n/* Counts the slot's items. */\ns32 Shop_Count(struct Slot *slot)\n{\n    return Item_Count(slot->id);\n}\n";

    #[test]
    fn adds_missing_includes_after_the_top_include_block() {
        let (text, added) = with_includes(MODULE, DRAFT);
        assert_eq!(added, ["#include \"ITEM.H\""]);
        assert!(text.starts_with("/* Shop module. */\n#include \"TYPES.H\"\n#include \"SHOP.H\"\n#include \"ITEM.H\"\n\nstruct Slot"));
        let (bare, _) = with_includes("void f(void) {}\n", DRAFT);
        assert!(bare.starts_with("#include \"TYPES.H\"\n#include \"ITEM.H\"\n\nvoid f"));
        assert_eq!(with_includes(MODULE, "#include \"SHOP.H\"\n").1.len(), 0);
    }

    #[test]
    fn appends_what_the_module_lacks_and_drops_the_audit_header() {
        let (with, _) = with_includes(MODULE, DRAFT);
        let merged = merge(&with, DRAFT, &Known::default(), false, "SHOP.C").unwrap();
        assert_eq!(merged.present, 2);
        assert_eq!(merged.defined, ["Shop_Count"]);
        assert!(merged.text.ends_with("    Shop_Close();\n}\n\ns32 Item_Count(u32 id);\n\n/* Counts the slot's items. */\ns32 Shop_Count(struct Slot *slot)\n{\n    return Item_Count(slot->id);\n}\n"));
        assert!(!merged.text.contains("Draft:"));
    }

    #[test]
    fn head_puts_the_draft_before_the_first_function() {
        let module = "#include \"TYPES.H\"\n\nvoid Shop_Close(void);\n\n/* Opens. */\nvoid Shop_Open(void)\n{\n}\n";
        let merged = merge(module, DRAFT, &Known::default(), true, "SHOP.C").unwrap();
        assert_eq!(
            merged.text,
            "#include \"TYPES.H\"\n\nvoid Shop_Close(void);\n\nstruct Slot {\n    u8 id;\n};\ns32 Item_Count(u32 id);\n\n/* Counts the slot's items. */\ns32 Shop_Count(struct Slot *slot)\n{\n    return Item_Count(slot->id);\n}\n\n/* Opens. */\nvoid Shop_Open(void)\n{\n}\n"
        );
        let later = "#include \"TYPES.H\"\nvoid Shop_Open(void)\n{\n}\nstruct Slot {\n    u8 id;\n};\n";
        let error = merge(later, DRAFT, &Known::default(), true, "SHOP.C").err().unwrap();
        assert!(error.contains("struct Slot is defined below the module's first function"), "{error}");
    }

    #[test]
    fn refuses_a_name_defined_twice() {
        let module = "#include \"TYPES.H\"\nstruct Slot { u16 id; };\ns32 Shop_Count(struct Slot *slot)\n{\n    return 0;\n}\n";
        let error = merge(module, DRAFT, &Known::default(), false, "SHOP.C").err().unwrap();
        assert!(error.contains("struct Slot is already defined in SHOP.C"), "{error}");
        assert!(error.contains("Shop_Count is already defined in SHOP.C"), "{error}");
        let headers = Known::of_headers(
            "# 1 \"/tmp/module.c\"\n# 1 \"games/COMMON/INCLUDE/GAME/ITEM.H\" 1\ntypedef unsigned short u16;\ns32 Item_Count(u32 count);\nstruct Slot { u32 id; };\n# 2 \"/tmp/module.c\" 2\n",
            "/tmp/module.c",
        );
        let error = merge(MODULE.replace("struct Slot {\n    u8 id;\n};\n", "").as_str(), DRAFT, &headers, false, "SHOP.C").err().unwrap();
        assert_eq!(error, "the draft cannot merge into SHOP.C:\n  struct Slot is already defined in games/COMMON/INCLUDE/GAME/ITEM.H");
    }

    #[test]
    fn leaves_identical_header_definitions_and_notes_differing_declarations() {
        let headers = Known::of_headers(
            "# 1 \"games/X/TYPES.H\" 1\ntypedef unsigned short u16;\nvoid Item_Count(void);\n# 3 \"/tmp/module.c\" 2\nvoid Local(void);\n",
            "/tmp/module.c",
        );
        let draft = "#include \"TYPES.H\"\ntypedef unsigned short u16;\ns32 Item_Count(u32 id);\nvoid Local(u32);\nstruct Rec *Find(u32 key)\n{\n    return 0;\n}\n";
        let module = "#include \"TYPES.H\"\nstruct Event *Find(u32);\n";
        let merged = merge(module, draft, &headers, false, "M.C").unwrap();
        assert_eq!(merged.text, "#include \"TYPES.H\"\nstruct Event *Find(u32);\n\nvoid Local(u32);\nstruct Rec *Find(u32 key)\n{\n    return 0;\n}\n");
        assert_eq!(merged.differing, ["Find", "Item_Count"]);
        assert_eq!(merged.present, 2);
    }
}
