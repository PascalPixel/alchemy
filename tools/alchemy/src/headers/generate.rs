//! A C file's header: the prototype of each function and the `extern` of
//! each object it defines for other code, in the file's order, with the
//! types they name.

use super::model::{spelling, Model, INCLUDE, UNIT};
use super::scan::{Entry, What};
use crate::c_items::{declared, flat, Kind, Space};
use std::collections::{BTreeMap, BTreeSet};

/// The macros a condition may test in a header that any file includes.
const EDITION_MACROS: &[&str] = &["EDITION_INTERNATIONAL", "GAME_TLA", "MODERN", "defined"];

/// Where each typedef name, enum tag and struct or union is defined.
#[derive(Default)]
pub(crate) struct Types {
    pub typedefs: BTreeMap<String, String>,
    pub enums: BTreeMap<String, String>,
    /// By `struct Tag` or `union Tag`.
    pub tags: BTreeMap<String, String>,
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
                } else if name.defines && name.space == Space::Tag {
                    self.tags.entry(name.signature).or_insert_with(|| header.to_string());
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

/// What a declaration needs before it: the tags it names.
#[derive(Default)]
pub(crate) struct Needs {
    pub tags: Vec<String>,
    pub includes: Vec<String>,
}

/// What one signature needs before it: the tags it only points to,
/// declared, and the headers of its typedef names, its enums and the
/// structs it holds whole. An error is a name no header on the search
/// path defines.
pub(crate) fn needs(signature: &str, name: &str, types: &Types, own: &Types) -> Result<Needs, String> {
    let mut out = Needs::default();
    let found: Vec<&str> = signature.split(' ').filter(|t| !t.is_empty()).collect();
    let mut named = false;
    let reached = |header: Option<&String>, what: &str| -> Result<String, String> {
        match header {
            Some(header) => spelling(header).ok_or_else(|| format!("names {what}, defined in {header}, off the search path")),
            None => Err(format!("names {what}, which no header defines")),
        }
    };
    // A type the file defines for itself is the one it means.
    let find = |own: &BTreeMap<String, String>, all: &'_ BTreeMap<String, String>, key: &str| -> Option<String> { own.get(key).or_else(|| all.get(key)).cloned() };
    for (i, word) in found.iter().enumerate() {
        if !word.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_') {
            continue;
        }
        if *word == name && !named {
            named = true;
            continue;
        }
        match i.checked_sub(1).map(|before| found[before]) {
            Some(keyword @ ("struct" | "union")) => {
                let tag = format!("{keyword} {word}");
                // A pointer to the struct needs its tag; the struct held
                // whole needs its definition.
                if found.get(i + 1) == Some(&"*") {
                    out.tags.push(format!("{tag};"));
                } else {
                    out.includes.push(reached(find(&own.tags, &types.tags, &tag).as_ref(), &tag)?);
                }
            }
            Some("enum") => out.includes.push(reached(find(&own.enums, &types.enums, word).as_ref(), &format!("enum {word}"))?),
            _ if KEYWORDS.contains(word) => {}
            _ => out.includes.push(reached(find(&own.typedefs, &types.typedefs, word).as_ref(), word)?),
        }
    }
    Ok(out)
}

/// An object definition's text as its declaration: the initializer and
/// comments out, `extern` before it.
fn external(item: &str) -> Option<String> {
    let mask = crate::camelotize::source::mask(item);
    let code = String::from_utf8_lossy(&mask).into_owned();
    if code.contains("__attribute__") || code.contains("asm") {
        return None;
    }
    let (mut depth, mut end) = (0usize, code.len());
    for (i, byte) in code.bytes().enumerate() {
        match byte {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => depth = depth.saturating_sub(1),
            b'=' | b';' if depth == 0 => {
                end = i;
                break;
            }
            _ => {}
        }
    }
    let text: Vec<&str> = code[..end].split_whitespace().filter(|word| *word != "extern").collect();
    Some(format!("extern {};", text.join(" ")))
}

/// The type definitions a unit header already holds, in its order: a type
/// that the file's functions name for other code lives in the header, and
/// a rewrite of the header keeps it.
pub(crate) fn held(existing: &str) -> Vec<&str> {
    flat(existing)
        .into_iter()
        .filter(|piece| piece.item.kind == Kind::Declaration && (piece.item.text.trim_start().starts_with("typedef") || piece.item.text.contains('{')))
        .filter(|piece| declared(&piece.item).iter().all(|d| d.typedef))
        .map(|piece| piece.item.text)
        .collect()
}

/// The structs and unions a text holds whole: `struct Tag` or `union Tag`
/// with no star and no body after it.
fn whole_tags(text: &str) -> Vec<String> {
    let found: Vec<&str> = text.split_whitespace().collect();
    let mut out = Vec::new();
    for (i, word) in found.iter().enumerate() {
        if !matches!(*word, "struct" | "union") {
            continue;
        }
        let Some(tag) = found.get(i + 1).filter(|tag| tag.chars().all(|c| c.is_ascii_alphanumeric() || c == '_')) else {
            continue;
        };
        if found.get(i + 2).is_some_and(|next| !next.starts_with('*') && !next.starts_with('{')) {
            out.push(format!("{word} {tag}"));
        }
    }
    out
}

/// One wording of a definition: its declaration, the conditions it stands
/// under and its signature.
type Wording<'a> = (String, &'a [String], &'a str);

/// The header's text for the entries of one C file and the names it
/// declares, or none when the file defines nothing for other code.
/// `problems` takes what it could not say; such a name is left out.
/// `existing` is the header as it stands: the types it holds stay.
pub(crate) fn header(path: &str, text: &str, existing: Option<&str>, entries: &[Entry], types: &Types, problems: &mut Vec<String>) -> Option<(String, BTreeSet<String>)> {
    let held: Vec<&str> = existing.map(held).unwrap_or_default();
    // Each name once, at its first definition, with every wording it has.
    let mut order: Vec<&str> = Vec::new();
    let mut wordings: BTreeMap<&str, Vec<Wording>> = BTreeMap::new();
    for entry in entries {
        if entry.local {
            continue;
        }
        let declaration = match &entry.what {
            What::Definition { prototype } => prototype.clone(),
            What::Object if entry.declarators == 1 => match external(&text[entry.start..entry.end]) {
                Some(declaration) => declaration,
                None => continue,
            },
            _ => continue,
        };
        let list = wordings.entry(&entry.name).or_default();
        if list.is_empty() {
            order.push(&entry.name);
        }
        if !list.iter().any(|(text, _, _)| *text == declaration) {
            list.push((declaration, &entry.conditions, &entry.signature));
        }
    }
    let (mut tags, mut includes, mut names) = (BTreeSet::new(), BTreeSet::new(), BTreeSet::new());
    let mut lines: Vec<(String, String)> = Vec::new();
    let mut own_types = Types::default();
    own_types.add(path, text);
    own_types.typedefs.values_mut().chain(own_types.enums.values_mut()).chain(own_types.tags.values_mut()).for_each(|file| *file = "its own file".to_string());
    // A type the header holds is in reach of every line of it, and what
    // the type itself names comes with the header.
    let mut held_types = Types::default();
    for text in &held {
        held_types.add(path, text);
        includes.extend(words(text).into_iter().filter(|word| !held_types.typedefs.contains_key(*word)).filter_map(|word| types.typedefs.get(word)).filter_map(|file| spelling(file)));
        includes.extend(whole_tags(text).iter().filter(|tag| !held_types.tags.contains_key(*tag)).filter_map(|tag| types.tags.get(tag)).filter_map(|file| spelling(file)));
    }
    own_types.typedefs.extend(held_types.typedefs);
    own_types.enums.extend(held_types.enums);
    own_types.tags.extend(held_types.tags);
    'names: for name in order {
        let list = &wordings[name];
        let mut own = (Vec::new(), Vec::new(), Vec::new());
        for (declaration, conditions, signature) in list {
            let condition = if list.len() == 1 { String::new() } else { conditions.join(" && ") };
            if !editions_only(&condition) {
                problems.push(format!("{path}: {name} has several wordings under `{condition}`, which a header cannot test"));
                continue 'names;
            }
            match needs(signature, name, types, &own_types) {
                Ok(needs) => {
                    own.0.extend(needs.tags);
                    own.1.extend(needs.includes);
                }
                Err(problem) => {
                    problems.push(format!("{path}: {name} {problem}"));
                    continue 'names;
                }
            }
            own.2.push((condition, declaration.clone()));
        }
        tags.extend(own.0);
        includes.extend(own.1);
        lines.extend(own.2);
        names.insert(name.to_string());
    }
    if lines.is_empty() && held.is_empty() {
        return None;
    }
    if let Some(own) = spelling(path) {
        includes.remove(&own);
    }
    Some((render(path, tags, includes, &held, &lines), names))
}

/// A header's text: its guard, what its declarations need, and each
/// declaration under its condition.
pub(crate) fn render(path: &str, tags: BTreeSet<String>, mut includes: BTreeSet<String>, held: &[&str], lines: &[(String, String)]) -> String {
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
    for text in held {
        out.push('\n');
        out.push_str(text.trim_end());
        out.push('\n');
    }
    out.push('\n');
    let mut open = String::new();
    for (condition, declaration) in lines {
        if *condition != open {
            if !open.is_empty() {
                out.push_str("#endif\n");
            }
            if !condition.is_empty() {
                out.push_str(&format!("#if {condition}\n"));
            }
            open = condition.clone();
        }
        out.push_str(declaration);
        out.push('\n');
    }
    if !open.is_empty() {
        out.push_str("#endif\n");
    }
    out.push_str("\n#endif\n");
    out
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::headers::scan::entries;

    fn types() -> Types {
        let mut types = Types::default();
        types.add("games/tbs/include/types.h", "typedef signed int s32;\ntypedef unsigned char u8;\ntypedef signed short s16;\n");
        types.add("games/common/include/field/objdisp.h", "typedef struct Q Handle;\nenum Facing { NORTH };\nstruct Cell { s32 x; };\n");
        types.add("games/tbs/src/field/a/local.h", "typedef struct { s32 x; } Private;\nstruct Row { s32 x; };\n");
        types
    }

    #[test]
    fn a_header_holds_each_callable_function_once_with_its_types() {
        let source = "static void Hidden(void)\n{\n}\nconst struct Actor *Get(s32 id, Handle h)\n{\n}\n#if EDITION_INTERNATIONAL\nvoid Draw(union Cell *cell, enum Facing f)\n{\n}\n#else\nvoid Draw(union Cell *cell)\n{\n}\n#endif\n#if defined(TBS_EDITION_DE)\nvoid Only(void)\n{\n}\n#endif\nvoid Apart(Private *p)\n{\n}\n";
        let mut problems = Vec::new();
        let (text, names) = header("games/tbs/include/unit/field/a/mura.h", source, None, &entries(source), &types(), &mut problems).unwrap();
        assert_eq!(problems, ["games/tbs/include/unit/field/a/mura.h: Apart names Private, defined in games/tbs/src/field/a/local.h, off the search path"]);
        assert_eq!(names.iter().map(String::as_str).collect::<Vec<_>>(), ["Draw", "Get", "Only"]);
        assert_eq!(
            text,
            "#ifndef ALCHEMY_UNIT_FIELD_A_MURA_H\n#define ALCHEMY_UNIT_FIELD_A_MURA_H\n\n#include \"types.h\"\n#include \"edition.h\"\n#include \"objdisp.h\"\n\nstruct Actor;\nunion Cell;\n\nconst struct Actor *Get(s32 id, Handle h);\n#if EDITION_INTERNATIONAL\nvoid Draw(union Cell *cell, enum Facing f);\n#endif\n#if !(EDITION_INTERNATIONAL)\nvoid Draw(union Cell *cell);\n#endif\nvoid Only(void);\n\n#endif\n"
        );
        assert!(header("x.h", "", None, &entries("static void Hidden(void)\n{\n}\n"), &types(), &mut problems).is_none());
    }

    #[test]
    fn a_header_declares_each_object_as_its_definition_types_it() {
        let source = "const s16 gSteps[] = { 1, 2 }; // 歩幅\nstruct Cell gCells[4];\nstruct Actor *gActor = 0;\nstatic u8 gMine;\nstruct Row gRows[2];\nu8 gA, gB;\n";
        let mut problems = Vec::new();
        let (text, names) = header("games/tbs/include/unit/a.h", source, None, &entries(source), &types(), &mut problems).unwrap();
        assert_eq!(names.iter().map(String::as_str).collect::<Vec<_>>(), ["gActor", "gCells", "gSteps"]);
        assert_eq!(problems, ["games/tbs/include/unit/a.h: gRows names struct Row, defined in games/tbs/src/field/a/local.h, off the search path"]);
        assert_eq!(
            text,
            "#ifndef ALCHEMY_UNIT_A_H\n#define ALCHEMY_UNIT_A_H\n\n#include \"types.h\"\n#include \"objdisp.h\"\n\nstruct Actor;\n\nextern const s16 gSteps[];\nextern struct Cell gCells[4];\nextern struct Actor *gActor;\n\n#endif\n"
        );
    }

    #[test]
    fn a_header_keeps_the_types_it_holds_and_declares_what_names_them() {
        assert_eq!(whole_tags("struct Cell cell;\n\tstruct Actor *actor;\n\tunion Pos\n\t{\n\t} p;"), ["struct Cell"]);
        let existing = "#ifndef ALCHEMY_UNIT_A_H\n#define ALCHEMY_UNIT_A_H\n\n#include \"types.h\"\n\nstruct Actor;\n\ntypedef struct Record\n{\n\ts32 x;\n\tHandle h;\n} Record;\n\nvoid Old(void);\n\n#endif\n";
        let source = "s32 Copy(Record *record)\n{\n}\n";
        let mut problems = Vec::new();
        assert_eq!(held(existing), ["typedef struct Record\n{\n\ts32 x;\n\tHandle h;\n} Record;"]);
        let (text, names) = header("games/tbs/include/unit/a.h", source, Some(existing), &entries(source), &types(), &mut problems).unwrap();
        assert_eq!(problems, Vec::<String>::new());
        assert_eq!(names.iter().map(String::as_str).collect::<Vec<_>>(), ["Copy"]);
        assert_eq!(
            text,
            "#ifndef ALCHEMY_UNIT_A_H\n#define ALCHEMY_UNIT_A_H\n\n#include \"types.h\"\n#include \"objdisp.h\"\n\ntypedef struct Record\n{\n\ts32 x;\n\tHandle h;\n} Record;\n\ns32 Copy(Record *record);\n\n#endif\n"
        );
    }

    #[test]
    fn wordings_under_a_files_own_macro_are_a_problem() {
        let source = "#if LOCAL_SWITCH\nvoid Draw(void)\n{\n}\n#else\nvoid Draw(s32 a)\n{\n}\n#endif\n";
        let mut problems = Vec::new();
        assert!(header("games/tbs/include/unit/a.h", source, None, &entries(source), &types(), &mut problems).is_none());
        assert_eq!(problems.len(), 1);
        let unknown = "void Draw(Unknown *p)\n{\n}\n";
        assert!(header("games/tbs/include/unit/a.h", unknown, None, &entries(unknown), &types(), &mut problems).is_none());
        assert_eq!(problems[1], "games/tbs/include/unit/a.h: Draw names Unknown, which no header defines");
    }
}
