//! A C file's header: the prototype of each function it defines for other
//! code, in the file's order, with the types those prototypes name.

use super::model::{spelling, Model, INCLUDE, UNIT};
use super::scan::{Entry, What};
use crate::c_items::{declared, flat, Kind};
use std::collections::{BTreeMap, BTreeSet};

/// The macros a condition may test in a header that any file includes.
const EDITION_MACROS: &[&str] = &["EDITION_INTERNATIONAL", "GAME_TLA", "MODERN", "defined"];

/// Where the headers define each typedef name and each enum tag.
#[derive(Default)]
pub(crate) struct Types {
    pub typedefs: BTreeMap<String, String>,
    pub enums: BTreeMap<String, String>,
}

impl Types {
    pub(crate) fn read(model: &Model) -> Types {
        let mut types = Types::default();
        for header in &model.headers {
            if header.starts_with(&format!("{INCLUDE}{UNIT}")) {
                continue;
            }
            types.add(header, &model.text[header]);
        }
        // A type a C file defines for itself is known, and out of reach.
        for file in model.units.iter().chain(&model.fragments) {
            types.add(file, &model.text[file]);
        }
        types
    }

    pub(crate) fn add(&mut self, header: &str, text: &str) {
        for piece in flat(text) {
            if piece.item.kind != Kind::Declaration {
                continue;
            }
            for d in declared(&piece.item) {
                if d.typedef {
                    self.typedefs.entry(d.name).or_insert_with(|| header.to_string());
                }
            }
            for name in crate::c_items::names(&piece.item) {
                if name.defines && name.signature.starts_with("enum ") {
                    self.enums.entry(name.name).or_insert_with(|| header.to_string());
                }
            }
        }
    }
}

fn words(text: &str) -> Vec<&str> {
    text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).filter(|w| !w.is_empty()).collect()
}

fn guard(header: &str) -> String {
    let stem = header.strip_prefix(INCLUDE).unwrap_or(header).trim_end_matches(".h");
    let name: String = stem.chars().map(|c| if c.is_ascii_alphanumeric() { c.to_ascii_uppercase() } else { '_' }).collect();
    format!("ALCHEMY_{name}_H")
}

fn editions_only(condition: &str) -> bool {
    words(condition).iter().all(|w| {
        EDITION_MACROS.contains(w) || w.starts_with("TBS_EDITION_") || w.starts_with("TLA_EDITION_") || w.chars().all(|c| c.is_ascii_digit())
    })
}

const KEYWORDS: &[&str] = &[
    "void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "const", "volatile", "struct", "union", "enum", "register", "inline", "static",
    "extern",
];

/// What one function's signature needs before it: the tags it names,
/// declared, and the headers of its typedef names and enums. An error is
/// a name no header on the search path defines.
fn needs(signature: &str, name: &str, types: &Types) -> Result<(Vec<String>, Vec<String>), String> {
    let (mut tags, mut includes) = (Vec::new(), Vec::new());
    let found = words(signature);
    let mut named = false;
    for (i, word) in found.iter().enumerate() {
        if *word == name && !named {
            named = true;
            continue;
        }
        match i.checked_sub(1).map(|before| found[before]) {
            Some("struct") => tags.push(format!("struct {word};")),
            Some("union") => tags.push(format!("union {word};")),
            Some("enum") => match types.enums.get(*word).and_then(|h| spelling(h)) {
                Some(include) => includes.push(include),
                None => return Err(format!("names enum {word}, which no header on the search path defines")),
            },
            _ if KEYWORDS.contains(word) || word.starts_with(|c: char| c.is_ascii_digit()) => {}
            _ => match types.typedefs.get(*word) {
                Some(header) => match spelling(header) {
                    Some(include) => includes.push(include),
                    None => return Err(format!("names {word}, defined in {header}, off the search path")),
                },
                None => return Err(format!("names {word}, which no header defines as a type")),
            },
        }
    }
    Ok((tags, includes))
}

/// One wording of a definition: its prototype, the conditions it stands
/// under and its signature.
type Wording<'a> = (&'a str, &'a [String], &'a str);

/// The header's text for the entries of one C file and the names it
/// declares, or none when the file defines nothing for other code.
/// `problems` takes what it could not say; such a function is left out.
pub(crate) fn header(path: &str, entries: &[Entry], types: &Types, problems: &mut Vec<String>) -> Option<(String, BTreeSet<String>)> {
    // Each name once, at its first definition, with every wording it has.
    let mut order: Vec<&str> = Vec::new();
    let mut wordings: BTreeMap<&str, Vec<Wording>> = BTreeMap::new();
    for entry in entries {
        let What::Definition { prototype } = &entry.what else {
            continue;
        };
        if entry.local {
            continue;
        }
        let list = wordings.entry(&entry.name).or_default();
        if list.is_empty() {
            order.push(&entry.name);
        }
        if !list.iter().any(|(text, _, _)| text == prototype) {
            list.push((prototype, &entry.conditions, &entry.signature));
        }
    }
    let (mut tags, mut includes, mut names) = (BTreeSet::new(), BTreeSet::new(), BTreeSet::new());
    let mut lines: Vec<(String, String)> = Vec::new();
    'names: for name in order {
        let list = &wordings[name];
        let mut own = (Vec::new(), Vec::new(), Vec::new());
        for (prototype, conditions, signature) in list {
            let condition = if list.len() == 1 { String::new() } else { conditions.join(" && ") };
            if !editions_only(&condition) {
                problems.push(format!("{path}: {name} has several wordings under `{condition}`, which a header cannot test"));
                continue 'names;
            }
            match needs(signature, name, types) {
                Ok((tags, includes)) => {
                    own.0.extend(tags);
                    own.1.extend(includes);
                }
                Err(problem) => {
                    problems.push(format!("{path}: {name} {problem}"));
                    continue 'names;
                }
            }
            own.2.push((condition, prototype.to_string()));
        }
        tags.extend(own.0);
        includes.extend(own.1);
        lines.extend(own.2);
        names.insert(name.to_string());
    }
    if lines.is_empty() {
        return None;
    }
    includes.remove("types.h");
    if lines.iter().any(|(condition, _)| !condition.is_empty()) {
        includes.insert("edition.h".to_string());
    }
    let guard = guard(path);
    let mut out = format!("#ifndef {guard}\n#define {guard}\n\n#include \"types.h\"\n");
    for include in &includes {
        out.push_str(&format!("#include \"{include}\"\n"));
    }
    if !tags.is_empty() {
        out.push('\n');
        for tag in &tags {
            out.push_str(&format!("{tag}\n"));
        }
    }
    out.push('\n');
    let mut open = String::new();
    for (condition, prototype) in &lines {
        if *condition != open {
            if !open.is_empty() {
                out.push_str("#endif\n");
            }
            if !condition.is_empty() {
                out.push_str(&format!("#if {condition}\n"));
            }
            open = condition.clone();
        }
        out.push_str(prototype);
        out.push('\n');
    }
    if !open.is_empty() {
        out.push_str("#endif\n");
    }
    out.push_str("\n#endif\n");
    Some((out, names))
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::headers::scan::entries;

    #[test]
    fn a_header_holds_each_callable_function_once_with_its_types() {
        let mut types = Types::default();
        types.add("games/tbs/include/types.h", "typedef signed int s32;\n");
        types.add("games/common/include/field/objdisp.h", "typedef struct Q Handle;\nenum Facing { NORTH };\n");
        types.add("games/tbs/src/field/a/local.h", "typedef struct { s32 x; } Private;\n");
        let source = "static void Hidden(void)\n{\n}\nconst struct Actor *Get(s32 id, Handle h)\n{\n}\n#if EDITION_INTERNATIONAL\nvoid Draw(union Cell *cell, enum Facing f)\n{\n}\n#else\nvoid Draw(union Cell *cell)\n{\n}\n#endif\n#if defined(TBS_EDITION_DE)\nvoid Only(void)\n{\n}\n#endif\nvoid Apart(Private *p)\n{\n}\n";
        let mut problems = Vec::new();
        let (text, names) = header("games/tbs/include/unit/field/a/mura.h", &entries(source), &types, &mut problems).unwrap();
        assert_eq!(problems, ["games/tbs/include/unit/field/a/mura.h: Apart names Private, defined in games/tbs/src/field/a/local.h, off the search path"]);
        assert_eq!(names.iter().map(String::as_str).collect::<Vec<_>>(), ["Draw", "Get", "Only"]);
        assert_eq!(
            text,
            "#ifndef ALCHEMY_UNIT_FIELD_A_MURA_H\n#define ALCHEMY_UNIT_FIELD_A_MURA_H\n\n#include \"types.h\"\n#include \"edition.h\"\n#include \"objdisp.h\"\n\nstruct Actor;\nunion Cell;\n\nconst struct Actor *Get(s32 id, Handle h);\n#if EDITION_INTERNATIONAL\nvoid Draw(union Cell *cell, enum Facing f);\n#endif\n#if !(EDITION_INTERNATIONAL)\nvoid Draw(union Cell *cell);\n#endif\nvoid Only(void);\n\n#endif\n"
        );
        assert!(header("x.h", &entries("static void Hidden(void)\n{\n}\n"), &types, &mut problems).is_none());
    }

    #[test]
    fn wordings_under_a_files_own_macro_are_a_problem() {
        let source = "#if LOCAL_SWITCH\nvoid Draw(void)\n{\n}\n#else\nvoid Draw(s32 a)\n{\n}\n#endif\n";
        let mut problems = Vec::new();
        let mut types = Types::default();
        types.add("games/tbs/include/types.h", "typedef signed int s32;\n");
        assert!(header("games/tbs/include/unit/a.h", &entries(source), &types, &mut problems).is_none());
        assert_eq!(problems.len(), 1);
        let unknown = "void Draw(Unknown *p)\n{\n}\n";
        assert!(header("games/tbs/include/unit/a.h", &entries(unknown), &types, &mut problems).is_none());
        assert_eq!(problems[1], "games/tbs/include/unit/a.h: Draw names Unknown, which no header defines as a type");
    }
}
