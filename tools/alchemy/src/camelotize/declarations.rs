use crate::compiler::routing::COMMON_INCLUDE_MODULES;
use crate::targets::DecompTarget;
use regex::Regex;
use std::collections::BTreeMap;
use std::path::Path;
use std::sync::OnceLock;

fn declaration() -> &'static Regex {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    PATTERN.get_or_init(|| {
        Regex::new(
            r"^(?:extern )?(?:const )?(?:void|s8|u8|s16|u16|s32|u32|char|int|struct \w+|union \w+|[A-Z]\w*)(?:\s+\**\s*|\*+\s*)([A-Z][A-Za-z0-9]*_\w+)\s*\(([^;{}]*)\);[ \t]*$",
        )
        .expect("declaration pattern")
    })
}

pub(crate) fn index(root: &Path, target: DecompTarget) -> Result<BTreeMap<String, String>, String> {
    let mut headers = Vec::new();
    let mut directories = vec![root.join("games").join(target.game_dir()).join("include")];
    directories.extend(COMMON_INCLUDE_MODULES.iter().map(|m| root.join("games/common/include").join(m)));
    for directory in directories {
        for entry in walkdir::WalkDir::new(&directory).into_iter().filter_map(Result::ok) {
            if entry.path().extension().is_some_and(|e| e == "h") {
                headers.push(entry.path().to_path_buf());
            }
        }
    }
    headers.sort();
    let mut found = BTreeMap::new();
    for header in headers {
        let text = std::fs::read_to_string(&header).map_err(|e| format!("{}: {e}", header.display()))?;
        let name = header.file_name().and_then(|n| n.to_str()).unwrap_or_default().to_string();
        add(&mut found, &text, &name);
    }
    Ok(found)
}

fn add(found: &mut BTreeMap<String, String>, text: &str, header: &str) {
    for line in text.lines() {
        if let Some(c) = declaration().captures(line) {
            if !c[2].trim().is_empty() {
                found.entry(c[1].to_string()).or_insert_with(|| header.to_string());
            }
        }
    }
}

pub(crate) fn rewrite(text: &str, index: &BTreeMap<String, String>) -> (String, usize) {
    let mut needed: Vec<String> = Vec::new();
    let mut kept = Vec::new();
    let mut removed = 0;
    for line in text.split_inclusive('\n') {
        let bare = line.trim_end_matches(['\n', '\r']);
        if let Some(c) = declaration().captures(bare) {
            if let Some(header) = index.get(&c[1]) {
                if !needed.contains(header) {
                    needed.push(header.clone());
                }
                removed += 1;
                continue;
            }
        }
        kept.push(line);
    }
    if removed == 0 {
        return (text.to_string(), 0);
    }
    let mut out = kept.concat();
    while out.contains("\n\n\n") {
        out = out.replace("\n\n\n", "\n\n");
    }
    for header in needed {
        let included = super::source::includes(&out)
            .iter()
            .any(|(name, _)| name.rsplit('/').next() == Some(header.as_str()));
        if !included {
            out = super::source::insert_include(&out, &header);
        }
    }
    (out, removed)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn indexed() -> BTreeMap<String, String> {
        let mut found = BTreeMap::new();
        add(&mut found, "void Object_SetMode(void *object, s32 mode);\ns32 Engine_Wait();\n", "field_event.h");
        found
    }

    #[test]
    fn a_declaration_a_header_makes_gives_way_to_the_header() {
        let text = "#include \"types.h\"\n\nvoid Object_SetMode(void *, s32);\n\nvoid F(void)\n{\n    Object_SetMode(0, 1);\n}\n";
        let (out, removed) = rewrite(text, &indexed());
        assert_eq!(removed, 1);
        assert!(out.starts_with("#include \"types.h\"\n#include \"field_event.h\"\n"));
        assert!(!out.contains("void Object_SetMode(void *, s32);"));
    }

    #[test]
    fn unprototyped_header_declarations_and_unknown_functions_stay() {
        let text = "#include \"field_event.h\"\ns32 Engine_Wait();\nvoid Local_Thing(s32 x);\n";
        assert_eq!(rewrite(text, &indexed()), (text.to_string(), 0));
    }

    #[test]
    fn a_macro_call_is_not_a_declaration() {
        let mut found = BTreeMap::new();
        add(&mut found, "LAYOUT_SIZE_GUARD(Work_Size, struct Work, 0x0c);\nu8 *Work_Find(s32 id);\nvoid\t\tWork_Free(u8 *work);\n", "work.h");
        assert_eq!(found.keys().collect::<Vec<_>>(), ["Work_Find", "Work_Free"]);
        let text = "LAYOUT_OFFSET_GUARD(Work_Child, struct Work, child, 0x14);\nstruct Work*Work_Find(s32);\n";
        let (out, removed) = rewrite(text, &found);
        assert_eq!(removed, 1);
        assert!(out.contains("LAYOUT_OFFSET_GUARD(Work_Child, struct Work, child, 0x14);"));
        assert!(!out.contains("Work_Find"));
    }

    #[test]
    fn an_included_header_is_not_added_twice() {
        let text = "#include \"field_event.h\"\nvoid Object_SetMode(void *, s32);\n";
        let (out, removed) = rewrite(text, &indexed());
        assert_eq!(removed, 1);
        assert_eq!(out.matches("field_event.h").count(), 1);
    }
}
