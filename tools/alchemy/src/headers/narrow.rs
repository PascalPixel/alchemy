//! `alchemy headers narrow`: a ☀️ file keeps the unit headers it names
//! something from, and its include of every other unit header goes.
//!
//! A cut (`alchemy unit split`) gives the new file's unit header to every
//! file that read the old one, so that no file sees less than before. Most
//! of those files name nothing of the new header. This pass takes those
//! includes out again. A header gives its readers every macro, type,
//! function and datum it defines or declares; a file that names none of
//! them compiles the same without it. A file names what its own text names
//! and what the files it includes name, other unit headers apart: a C
//! fragment it includes at its place, a header whose macro or inline
//! function calls into a unit. The build and the modern build's
//! implicit-declaration warnings judge the result.

use crate::c_items;
use crate::headers::plan::identifiers;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

const INCLUDE: &str = "games/tbs/include/";
const UNIT: &str = "unit/";
const TREES: [&str; 3] = ["games/tbs/src", "games/tbs/include", "games/common/include"];

/// The names a header gives the files that read it.
fn gives(text: &str) -> BTreeSet<String> {
    // Its include guard is no name a reader uses.
    let guard = |name: &str| name.starts_with("ALCHEMY_") && name.ends_with("_H");
    c_items::flat(text).iter().flat_map(|piece| c_items::names(&piece.item)).map(|name| name.name).filter(|name| !guard(name)).collect()
}

/// The unit header an include line names: `#include "unit/a/b.h"`.
fn unit_include(line: &str) -> Option<&str> {
    let name = line.trim().strip_prefix('#')?.trim_start().strip_prefix("include")?.trim().strip_prefix('"')?.strip_suffix('"')?;
    (name.starts_with(UNIT) && name.ends_with(".h")).then_some(name)
}

/// A file's text without the unit-header includes it names nothing from,
/// and how many went. `own` is the file's own unit header, which stays;
/// `given` tells what a unit header gives, or None for one that is not
/// there to read (its include stays).
fn narrowed(text: &str, named: &BTreeSet<String>, own: Option<&str>, given: &dyn Fn(&str) -> Option<BTreeSet<String>>) -> (String, usize) {
    let mut out = String::with_capacity(text.len());
    let mut gone = 0;
    for line in text.split_inclusive('\n') {
        let needless = unit_include(line).is_some_and(|name| {
            // A header that gives nothing holds only the note of a function left
            // out of it; it is read for that note and stays.
            Some(name) != own && given(name).is_some_and(|names| !names.is_empty() && !names.iter().any(|name| named.contains(name)))
        });
        if needless {
            gone += 1;
        } else {
            out.push_str(line);
        }
    }
    (out, gone)
}

fn sources(root: &Path, folder: &str, found: &mut Vec<String>) {
    let Ok(entries) = fs::read_dir(root.join(folder)) else {
        return;
    };
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        let path = format!("{folder}/{name}");
        if entry.file_type().is_ok_and(|kind| kind.is_dir()) {
            sources(root, &path, found);
        } else if name.ends_with(".c") || name.ends_with(".h") {
            found.push(path);
        }
    }
}

/// The words in the bodies of a header's macros and inline functions: the
/// code its reader compiles.
fn body_words(text: &str) -> BTreeSet<String> {
    let words = |text: &str| -> Vec<String> {
        text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).filter(|word| word.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_')).map(str::to_owned).collect()
    };
    c_items::flat(text)
        .iter()
        .flat_map(|piece| match &piece.item.kind {
            c_items::Kind::Directive if piece.item.text.trim_start().trim_start_matches('#').trim_start().starts_with("define") => words(piece.item.text),
            c_items::Kind::Function { body } => words(&piece.item.text[*body..]),
            _ => Vec::new(),
        })
        .collect()
}

/// A header's text with an include of each unit header its macro and inline
/// bodies call into and it does not read yet, after its last include, and
/// how many were added. `owner` names the one unit header that gives a name.
fn self_sufficient(text: &str, own: &str, owner: &BTreeMap<String, String>) -> (String, usize) {
    let reads: BTreeSet<String> = crate::headers::model::include_names(text).into_iter().collect();
    let wanted: BTreeSet<&String> = body_words(text).iter().filter_map(|word| owner.get(word)).filter(|unit| unit.as_str() != own && !reads.contains(*unit)).collect();
    if wanted.is_empty() {
        return (text.to_owned(), 0);
    }
    let lines: Vec<&str> = text.split_inclusive('\n').collect();
    let is_include = |line: &&str| line.trim_start().strip_prefix('#').is_some_and(|rest| rest.trim_start().starts_with("include"));
    let at = lines.iter().rposition(is_include).or_else(|| lines.iter().position(|line| line.trim_start().starts_with("#define"))).map_or(0, |at| at + 1);
    let mut out: String = lines[..at].concat();
    for unit in &wanted {
        out.push_str(&format!("#include \"{unit}\"\n"));
    }
    out.push_str(&lines[at..].concat());
    (out, wanted.len())
}

/// What an included file passes on to its reader: its identifiers and those
/// of every file it includes in turn that is no unit header, found by the
/// include's last name. Of a
/// header only the bodies of its macros and inline functions count: that
/// is the code its reader compiles (a far-call list names every function
/// of a bank and calls none). A header's names that a unit header it
/// includes itself gives are its own affair: it reads that unit header,
/// whoever includes it.
fn names_of(file: &str, texts: &BTreeMap<String, String>, by_name: &BTreeMap<String, Vec<String>>, given: &dyn Fn(&str) -> Option<BTreeSet<String>>, open: &mut BTreeSet<String>, named: &mut BTreeSet<String>) {
    let Some(text) = texts.get(file) else {
        return;
    };
    if !open.insert(file.to_owned()) {
        return;
    }
    let includes = crate::headers::model::include_names(text);
    let mut own: BTreeSet<String> = if file.ends_with(".h") {
        body_words(text)
    } else {
        identifiers(text).into_iter().map(str::to_owned).collect()
    };
    if file.ends_with(".h") {
        for unit in includes.iter().filter(|include| include.starts_with(UNIT)) {
            for name in given(unit).into_iter().flatten() {
                own.remove(&name);
            }
        }
    }
    named.extend(own);
    for include in includes {
        if include.starts_with(UNIT) {
            continue;
        }
        let last = include.rsplit('/').next().unwrap_or(&include);
        for path in by_name.get(last).into_iter().flatten() {
            names_of(path, texts, by_name, given, open, named);
        }
    }
}

pub(crate) fn run(root: &Path, dry_run: bool) -> Result<(), String> {
    let mut files = Vec::new();
    for tree in TREES {
        sources(root, tree, &mut files);
    }
    files.sort();
    let mut texts = BTreeMap::new();
    let mut by_name: BTreeMap<String, Vec<String>> = BTreeMap::new();
    for file in &files {
        texts.insert(file.clone(), fs::read_to_string(root.join(file)).map_err(|error| format!("{file}: {error}"))?);
        // A plain include never means a unit header of the same last name.
        if !file.starts_with(&format!("{INCLUDE}{UNIT}")) {
            by_name.entry(file.rsplit('/').next().unwrap_or(file).to_owned()).or_default().push(file.clone());
        }
    }
    // What each C file's unit header gives; another header under unit/ (the
    // overlays' import list) gives its names through macros and is not judged.
    let mut known: BTreeMap<String, Option<BTreeSet<String>>> = BTreeMap::new();
    for file in files.iter().filter(|file| file.starts_with(INCLUDE) && file[INCLUDE.len()..].starts_with(UNIT)) {
        let name = &file[INCLUDE.len()..];
        let source = format!("games/tbs/src/{}.c", name.trim_start_matches(UNIT).trim_end_matches(".h"));
        known.insert(name.to_owned(), root.join(source).is_file().then(|| gives(&texts[file])));
    }
    // A ☀️ header whose macro or inline bodies call into a unit reads that
    // unit's header itself, so its readers need not.
    let mut givers: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for (unit, names) in known.iter().filter_map(|(unit, names)| names.as_ref().map(|names| (unit, names))) {
        for name in names {
            givers.entry(name.clone()).or_default().insert(unit.clone());
        }
    }
    let owner: BTreeMap<String, String> = givers.into_iter().filter(|(_, units)| units.len() == 1).map(|(name, units)| (name, units.into_iter().next().expect("one"))).collect();
    let (mut added, mut widened) = (0, 0);
    for file in files.iter().filter(|file| file.starts_with(INCLUDE) && !file[INCLUDE.len()..].starts_with(UNIT)) {
        let (now, count) = self_sufficient(&texts[file], &file[INCLUDE.len()..], &owner);
        if count > 0 {
            added += count;
            widened += 1;
            if !dry_run {
                fs::write(root.join(file), &now).map_err(|error| format!("{file}: {error}"))?;
            }
            texts.insert(file.clone(), now);
        }
    }
    println!("{added} includes of unit headers {} {widened} headers whose macros or inline functions call into those units", if dry_run { "would go into" } else { "added to" });
    let (mut total, mut changed) = (0, 0);
    for file in &files {
        let text = &texts[file];
        let own = file
            .strip_prefix("games/tbs/src/")
            .and_then(|stem| stem.strip_suffix(".c"))
            .map(|stem| format!("{UNIT}{stem}.h"))
            .or_else(|| file.strip_prefix(INCLUDE).map(str::to_owned));
        // The file's own text names what it names; what it includes adds
        // what that passes on.
        let mut named: BTreeSet<String> = identifiers(text).into_iter().map(str::to_owned).collect();
        let mut open = BTreeSet::from([file.clone()]);
        for include in crate::headers::model::include_names(text).into_iter().filter(|include| !include.starts_with(UNIT)) {
            let last = include.rsplit('/').next().unwrap_or(&include).to_owned();
            for path in by_name.get(&last).into_iter().flatten() {
                names_of(path, &texts, &by_name, &|name| known.get(name).cloned().flatten(), &mut open, &mut named);
            }
        }
        let (now, gone) = narrowed(text, &named, own.as_deref(), &|name| known.get(name).cloned().flatten());
        if gone > 0 {
            total += gone;
            changed += 1;
            if !dry_run {
                fs::write(root.join(file), now).map_err(|error| format!("{file}: {error}"))?;
            }
        }
    }
    println!(
        "{total} includes of unit headers {} {changed} files of {}: each named nothing its header gives",
        if dry_run { "would go from" } else { "taken out of" },
        files.len()
    );
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_file_keeps_the_unit_headers_it_names_something_from() {
        let given = |name: &str| match name {
            "unit/a/used.h" => Some(gives("#define LIMIT 4\nvoid Used(s32 x);\n")),
            "unit/a/own.h" => Some(gives("void Mine(void);\n")),
            "unit/a/idle.h" => Some(gives("struct Idle\n{\n\ts32 x;\n};\nvoid Idle_Run(struct Idle *idle);\n")),
            _ => None,
        };
        let text = "#include \"unit/a/own.h\"\n#include \"unit/a/used.h\"\n#include \"unit/a/idle.h\"\n#include \"unit/a/gone.h\"\n#include \"types.h\"\n\nvoid Mine(void)\n{\n\tUsed(LIMIT); // Idle_Run is only a word in a comment\n}\n";
        let named = |text: &str| identifiers(text).into_iter().map(str::to_owned).collect::<BTreeSet<_>>();
        let (now, gone) = narrowed(text, &named(text), Some("unit/a/own.h"), &given);
        // The own header and the one it calls into stay; so does a header
        // that is not there to read, and what is no unit header.
        assert_eq!(gone, 1);
        assert_eq!(now, text.replace("#include \"unit/a/idle.h\"\n", ""));
        // A type is a name too.
        let typed = "#include \"unit/a/idle.h\"\nstruct Idle gIdle;\n";
        let (_, gone) = narrowed(typed, &named(typed), None, &given);
        assert_eq!(gone, 0);
        // A header that gives nothing is kept: it holds a note, not names.
        assert_eq!(narrowed("#include \"unit/a/note.h\"\n", &BTreeSet::new(), None, &|_| Some(BTreeSet::new())).1, 0);
        // Without a name of its own header, a file still keeps that header.
        assert_eq!(narrowed("#include \"unit/a/own.h\"\n", &BTreeSet::new(), Some("unit/a/own.h"), &given).1, 0);
        // A name the file reaches through a fragment or a macro header it
        // includes keeps the unit header that gives it.
        let texts = BTreeMap::from([
            ("games/g/src/a/unit.c".to_owned(), "#include \"unit/a/idle.h\"\n#include \"../b/part.c\"\n".to_owned()),
            ("games/g/src/b/part.c".to_owned(), "void Part(void)\n{\n\tIdle_Run(0);\n}\n".to_owned()),
        ]);
        let by_name = BTreeMap::from([("part.c".to_owned(), vec!["games/g/src/b/part.c".to_owned()])]);
        let mut reached = BTreeSet::new();
        names_of("games/g/src/b/part.c", &texts, &by_name, &given, &mut BTreeSet::new(), &mut reached);
        assert_eq!(narrowed(&texts["games/g/src/a/unit.c"], &reached, None, &given).1, 0);
    }

    #[test]
    fn a_header_reads_the_unit_headers_its_macros_call_into() {
        let owner = BTreeMap::from([("Actor_Move".to_owned(), "unit/a/actor.h".to_owned()), ("Wait".to_owned(), "unit/a/wait.h".to_owned())]);
        let text = "#ifndef EVENT_H\n#define EVENT_H\n#include \"types.h\"\n#include \"unit/a/wait.h\"\n\n#define MOVE(a) Actor_Move(a)\n#define WAIT(n) Wait(n)\nvoid Actor_Stop(s32 a); // a prototype calls nothing\n#endif\n";
        let (now, added) = self_sufficient(text, "event.h", &owner);
        assert_eq!(added, 1);
        assert_eq!(now, text.replace("#include \"unit/a/wait.h\"\n", "#include \"unit/a/wait.h\"\n#include \"unit/a/actor.h\"\n"));
        // What it reads already, and its own names, add nothing.
        assert_eq!(self_sufficient(&now, "event.h", &owner).1, 0);
        assert_eq!(self_sufficient("#define MOVE(a) Actor_Move(a)\n", "unit/a/actor.h", &owner).1, 0);
    }
}
