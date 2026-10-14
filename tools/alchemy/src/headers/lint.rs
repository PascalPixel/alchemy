//! alchemy check headers: a ☀️ C file declares nothing another file
//! defines, no name is declared twice, and a function defined for other
//! code is declared in its file's header or left out there for a stated
//! cause. What is still there is counted by kind against a tracked
//! baseline, and a count may only fall.

use super::model::{Model, COMMON, INCLUDE, UNIT};
use super::scan::{bodies, What};
use std::collections::BTreeMap;
use std::path::Path;

pub(crate) const BASELINE: &str = "tools/alchemy/headers.tsv";

const USAGE: &str = "usage: alchemy check headers [--update | --list]\n\
Counts, in ☀️'s game C files and the C files they include: prototypes of\n\
functions (at file scope and inside functions; a `static` prototype of a\n\
function the same file defines `static` is that file's own forward\n\
declaration and is not counted), extern declarations of\n\
objects (one inside a function under a FAKEMATCH tag is the device S2\n\
allows and is not counted), and macros that cast a name to another type\n\
under the name itself. Counts, in the headers those files read, each\n\
declaration of a name some other header already declares (a line under\n\
#if GAME_TLA is ⚓️'s and not counted; unit headers declare what their own\n\
file defines, so the same name in two of them is two functions). Counts\n\
each function a C file defines for other code that no header declares,\n\
unless its file's header holds, in the prototype's place, the comment\n\
that the function is called before its definition with no prototype in\n\
scope, or that it is called from another named C file with no prototype\n\
in scope (one line: `NAME is called from FILE.c with no prototype in\n\
scope`, then what that call shows); and each such comment that is not\n\
true of its file: the file does not define the function, the header\n\
declares it too, another file names it or nothing calls it above its\n\
definition (the first form), or the file it names does not name it (the\n\
second) (alchemy headers undeclared measures which functions the bytes\n\
need left out). Fails when a count\n\
rises above tools/alchemy/headers.tsv. --update writes the current\n\
counts there; --list prints every finding.";

pub(crate) const KINDS: [&str; 5] = ["prototype", "extern", "cast-macro", "second-declaration", "undeclared"];

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) struct Finding {
    pub kind: &'static str,
    pub file: String,
    pub line: usize,
    pub name: String,
}

fn line(text: &str, offset: usize) -> usize {
    text[..offset].matches('\n').count() + 1
}

/// Whether the declaration starting at `start` stands under a FAKEMATCH
/// tag: in a comment on the line above or on its own line.
fn tagged(text: &str, start: usize, end: usize) -> bool {
    let from = text[..start].rfind('\n').map_or(0, |n| n + 1);
    let above = text[..from].trim_end();
    let comment = above.rfind("/*").filter(|_| above.ends_with("*/")).or_else(|| above.rfind('\n').map(|n| n + 1).filter(|&l| above[l..].trim_start().starts_with("//")));
    let to = text[end..].find('\n').map_or(text.len(), |n| end + n);
    text[from..to].contains("FAKEMATCH") || comment.is_some_and(|open| above[open..].contains("FAKEMATCH"))
}

/// What one C file still declares for itself.
pub(crate) fn in_source(file: &str, text: &str) -> Vec<Finding> {
    let mut found = Vec::new();
    let mut push = |kind, at: usize, name: &str| found.push(Finding { kind, file: file.to_string(), line: line(text, at), name: name.to_string() });
    let all = super::scan::entries(text);
    // A file's forward declaration of its own static function declares
    // nothing another file defines.
    let own = |name: &str| all.iter().any(|entry| entry.name == name && entry.local && matches!(entry.what, What::Definition { .. }));
    for entry in &all {
        match entry.what {
            What::Prototype if !(entry.local && own(&entry.name)) => push("prototype", entry.start, &entry.name),
            What::Extern => push("extern", entry.start, &entry.name),
            What::ShapeMacro { .. } | What::ViewMacro => push("cast-macro", entry.start, &entry.name),
            _ => {}
        }
    }
    for body in bodies(text, &|_| false) {
        for local in &body.locals {
            if local.kind == super::scan::Scope::Prototype {
                push("prototype", local.start, &local.name);
            } else if !tagged(text, local.start, local.end) {
                push("extern", local.start, &local.name);
            }
        }
    }
    found.sort();
    found
}

/// Each declaration of a name that another header already declares.
/// `headers` gives each header's path and text, in reading order.
pub(crate) fn second_declarations(headers: &[(&str, &str)]) -> Vec<Finding> {
    let unit = format!("{INCLUDE}{UNIT}");
    let mut first: BTreeMap<String, (String, bool)> = BTreeMap::new();
    let mut found = Vec::new();
    for (file, text) in headers {
        let own = file.starts_with(&unit);
        let mut here = std::collections::BTreeSet::new();
        for entry in super::scan::entries(text) {
            if !matches!(entry.what, What::Prototype | What::Extern) || entry.local {
                continue;
            }
            // ⚓️'s line of a shared header is not ☀️'s declaration.
            if file.starts_with(COMMON) && entry.conditions.iter().any(|c| c.contains("GAME_TLA") && !c.replace(' ', "").starts_with("!")) {
                continue;
            }
            // One header's arms of a conditional declare one name once.
            if !here.insert(entry.name.clone()) {
                continue;
            }
            match first.get(&entry.name) {
                Some((_, true)) if own => {}
                Some(_) => found.push(Finding { kind: "second-declaration", file: file.to_string(), line: line(text, entry.start), name: entry.name.clone() }),
                None => {
                    first.insert(entry.name.clone(), (file.to_string(), own));
                }
            }
        }
    }
    found
}

/// Where a C file of a unit stands in the unit's text: the unit's own
/// offset, or the unit's line that includes the file and the offset in it.
pub(crate) fn place(model: &Model, unit: &str, file: &str, offset: usize) -> Option<(usize, usize)> {
    if file == unit {
        return Some((offset, 0));
    }
    let folder = unit.rsplit_once('/').map_or("", |(folder, _)| folder);
    let mut at = 0;
    for row in model.text[unit].split_inclusive('\n') {
        let name = row.trim_start().strip_prefix("#include").and_then(|rest| rest.trim_start().strip_prefix('"')).and_then(|rest| rest.split('"').next());
        if name.is_some_and(|name| super::model::normalize(&format!("{folder}/{name}")) == file) {
            return Some((at, offset + 1));
        }
        at += row.len();
    }
    None
}

/// Whether some unit that compiles the file calls the function above its
/// definition.
fn called_above(model: &Model, home: &str, name: &str) -> bool {
    let Some(defined) = model.entries[home].iter().filter(|entry| entry.name == name && matches!(entry.what, What::Definition { .. })).map(|entry| entry.start).min() else {
        return false;
    };
    model.users(home).iter().any(|unit| {
        let Some(definition) = place(model, unit, home, defined) else {
            return false;
        };
        model.parts(unit).any(|part| {
            let calls = super::undeclared::sites(&model.text[part], &|word| word == name);
            calls.iter().any(|call| place(model, unit, part, call.at).is_some_and(|call| call < definition))
        })
    })
}

/// The other file that names a function of `home`: a unit that does not
/// compile the file and means its definition, or a far-call bank's list.
fn named_elsewhere(model: &Model, home: &str, name: &str) -> Option<String> {
    let definitions = model.definitions();
    let users = model.users(home);
    let unit = model.units.iter().find(|unit| {
        !users.contains(&unit.as_str())
            && model.home(&definitions, unit, name).is_some_and(|(file, _)| file == home)
            && model.parts(unit).any(|part| super::plan::identifiers(&model.text[part]).contains(name))
    });
    unit.cloned().or_else(|| model.far.values().find(|far| far.target == name && model.in_main(home)).map(|far| far.file.clone()))
}

/// Whether the unit `file` (a path or its last parts) names a function of
/// `home` and means its definition, from outside the units that compile
/// `home`.
fn named_by(model: &Model, home: &str, name: &str, file: &str) -> bool {
    let definitions = model.definitions();
    let users = model.users(home);
    let suffix = format!("/{file}");
    model.units.iter().any(|unit| {
        (unit == file || unit.ends_with(&suffix))
            && !users.contains(&unit.as_str())
            && model.home(&definitions, unit, name).is_some_and(|(file, _)| file == home)
            && model.parts(unit).any(|part| super::plan::identifiers(&model.text[part]).contains(name))
    })
}

/// What the unit headers leave undeclared without cause. A function a C
/// file defines for other code has its prototype in the file's header,
/// or a comment there in its place: the function is called before its
/// definition with no prototype in scope, or it is called from another
/// named C file with no prototype in scope. Counted: such a function with
/// neither, and a comment that is not true of its file (the file does not
/// define the function, its header declares it too; for the first form,
/// another file names it or nothing calls it above its definition; for
/// the second, the file it names does not name the function). `headers`
/// gives each unit header's text.
pub(crate) fn undeclared(model: &Model, headers: &BTreeMap<String, String>) -> Vec<Finding> {
    let unit = format!("{INCLUDE}{UNIT}");
    // What the other headers declare: a function one of them declares is
    // declared, if not where it should be.
    let elsewhere: std::collections::BTreeSet<&str> = model
        .headers
        .iter()
        .filter(|header| !header.starts_with(&unit))
        .flat_map(|header| model.entries[header].iter())
        .filter(|entry| entry.what == What::Prototype)
        .map(|entry| entry.name.as_str())
        .collect();
    let mut found = Vec::new();
    for home in model.units.iter().chain(&model.fragments) {
        let Some(header) = super::model::unit_header(home) else {
            continue;
        };
        let text = headers.get(&header).map_or("", String::as_str);
        let declared: std::collections::BTreeSet<String> = super::scan::entries(text).into_iter().filter(|entry| entry.what == What::Prototype).map(|entry| entry.name).collect();
        let out = super::undeclared::left_out(text);
        let source = &model.text[home];
        let defined: Vec<&super::scan::Entry> = model.entries[home].iter().filter(|entry| matches!(entry.what, What::Definition { .. }) && !entry.local).collect();
        // A prototype in the file itself is counted as that.
        let own: std::collections::BTreeSet<&str> = model.entries[home].iter().filter(|entry| entry.what == What::Prototype).map(|entry| entry.name.as_str()).collect();
        let mut told = std::collections::BTreeSet::new();
        for entry in &defined {
            let known = declared.contains(&entry.name) || out.iter().any(|(name, _, _)| *name == entry.name) || elsewhere.contains(entry.name.as_str()) || own.contains(entry.name.as_str());
            if !known && told.insert(&entry.name) {
                found.push(Finding { kind: "undeclared", file: home.clone(), line: line(source, entry.start), name: format!("{}: its header has no prototype of it and no comment in its place", entry.name) });
            }
        }
        for note in super::undeclared::notes(text) {
            let (name, start) = (&note.name, &note.start);
            let why = if !defined.iter().any(|entry| entry.name == *name) {
                Some("its file does not define it for other code".to_string())
            } else if declared.contains(*name) {
                Some("its header declares it too".to_string())
            } else if let Some(file) = note.from {
                (!named_by(model, home, name, file)).then(|| format!("{file} does not name it"))
            } else if let Some(other) = named_elsewhere(model, home, name) {
                Some(format!("{other} names it too"))
            } else if !called_above(model, home, name) {
                Some("nothing calls it above its definition".to_string())
            } else {
                None
            };
            if let Some(why) = why {
                found.push(Finding { kind: "undeclared", file: header.clone(), line: line(text, *start), name: format!("{name}: {why}") });
            }
        }
    }
    found
}

pub(crate) fn findings(root: &Path) -> Result<Vec<Finding>, String> {
    let model = Model::read(root)?;
    let mut found = Vec::new();
    for file in model.units.iter().chain(&model.fragments) {
        found.extend(in_source(file, &model.text[file]));
    }
    let own: BTreeMap<String, String> = model
        .units
        .iter()
        .chain(&model.fragments)
        .filter_map(|file| super::model::unit_header(file))
        .filter_map(|header| std::fs::read_to_string(root.join(&header)).ok().map(|text| (header, text)))
        .collect();
    found.extend(undeclared(&model, &own));
    // Unit headers first: a name's own header is its first declaration.
    let unit = format!("{INCLUDE}{UNIT}");
    let mut headers: Vec<(&str, &str)> = model.headers.iter().filter(|h| h.ends_with(".h")).map(|h| (h.as_str(), model.text[h].as_str())).collect();
    headers.sort_by_key(|(file, _)| (!file.starts_with(&unit), *file));
    found.extend(second_declarations(&headers));
    Ok(found)
}

pub(crate) type Counts = BTreeMap<String, usize>;

pub(crate) fn count(found: &[Finding]) -> Counts {
    let mut counts: Counts = KINDS.iter().map(|kind| (kind.to_string(), 0)).collect();
    for finding in found {
        *counts.entry(finding.kind.to_string()).or_default() += 1;
    }
    counts
}

fn render(counts: &Counts) -> String {
    let mut text = String::from("kind\tcount\n");
    for (kind, value) in counts {
        text.push_str(&format!("{kind}\t{value}\n"));
    }
    text
}

fn parse(text: &str) -> Result<Counts, String> {
    text.lines()
        .skip(1)
        .map(|row| {
            let (kind, value) = row.split_once('\t').ok_or_else(|| format!("{BASELINE}: {row:?}"))?;
            Ok((kind.to_string(), value.parse().map_err(|_| format!("{BASELINE}: {row:?}"))?))
        })
        .collect()
}

/// The kinds whose count is above the baseline's, each as a message.
pub(crate) fn ratchet(baseline: &Counts, counts: &Counts) -> Vec<String> {
    counts
        .iter()
        .filter_map(|(kind, value)| {
            let allowed = baseline.get(kind).copied().unwrap_or(0);
            (*value > allowed).then(|| format!("{kind}: {value}, above the baseline's {allowed}"))
        })
        .collect()
}

fn run(args: &[String]) -> Result<(), String> {
    let root = crate::compiler::routing::root();
    let found = findings(root)?;
    let counts = count(&found);
    match args {
        [] => {}
        [flag] if flag == "--update" => {
            std::fs::write(root.join(BASELINE), render(&counts)).map_err(|e| format!("{BASELINE}: {e}"))?;
            print!("{}", render(&counts));
            return Ok(());
        }
        [flag] if flag == "--list" => {
            for finding in &found {
                println!("{}:{}: {} {}", finding.file, finding.line, finding.kind, finding.name);
            }
            print!("{}", render(&counts));
            return Ok(());
        }
        _ => return Err(USAGE.into()),
    }
    let baseline = parse(&std::fs::read_to_string(root.join(BASELINE)).map_err(|e| format!("{BASELINE}: {e}"))?)?;
    let failures = ratchet(&baseline, &counts);
    if !failures.is_empty() {
        return Err(format!("{}\ndeclare the name once, in the header of the file that defines it (alchemy check headers --list shows each place)", failures.join("\n")));
    }
    let lower: Vec<String> = counts.iter().filter(|(kind, value)| baseline.get(*kind).is_some_and(|b| *value < b)).map(|(kind, value)| format!("{kind} fell to {value}")).collect();
    if !lower.is_empty() {
        eprintln!("{}; run alchemy check headers --update to lock it in", lower.join(", "));
    }
    Ok(())
}

pub fn entry(args: &[String]) -> std::process::ExitCode {
    match run(args) {
        Ok(()) => std::process::ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("error: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_c_file_is_refused_what_a_header_should_say() {
        let source = "#include \"unit/a.h\"\ns32 Other(s32 a);\nextern u8 gTable[];\nstatic void Mine(void);\n#define Other ((void (*)(s32))Other)\n#define gTable ((u16(*)) & gTable)\n#define LIMIT 4\n\nvoid Run(void)\n{\n\tvoid Deep();\n\textern u8 gWork[];\n\t/* FAKEMATCH: the address folds only through this view. */\n\textern u16 gFolded[];\n\tDeep();\n}\n\nstatic void Mine(void)\n{\n}\n";
        let found: Vec<(&str, usize, String)> = in_source("games/tbs/src/a.c", source).into_iter().map(|f| (f.kind, f.line, f.name)).collect();
        assert_eq!(
            found,
            vec![
                ("cast-macro", 5, "Other".to_string()),
                ("cast-macro", 6, "gTable".to_string()),
                ("extern", 3, "gTable".to_string()),
                ("extern", 12, "gWork".to_string()),
                ("prototype", 2, "Other".to_string()),
                ("prototype", 11, "Deep".to_string()),
            ]
        );
    }

    #[test]
    fn a_files_forward_declaration_of_its_own_static_function_is_not_counted() {
        let source = "static void Mine(s32 a);\nstatic s32 Theirs(void);\nstatic void Shown(void);\nvoid Open(void);\n\nvoid Run(void)\n{\n\tMine(Theirs());\n}\n\nstatic void Mine(s32 a)\n{\n}\n\nvoid Shown(void)\n{\n}\n\nvoid Open(void)\n{\n}\n";
        let found: Vec<(&str, usize, String)> = in_source("games/tbs/src/a.c", source).into_iter().map(|f| (f.kind, f.line, f.name)).collect();
        // Theirs is defined nowhere in the file, Shown is not static
        // where the file defines it, and Open's prototype is not static.
        assert_eq!(found, vec![("prototype", 2, "Theirs".to_string()), ("prototype", 3, "Shown".to_string()), ("prototype", 4, "Open".to_string())]);
    }

    #[test]
    fn a_name_has_one_header() {
        let headers = [
            ("games/tbs/include/unit/a.h", "void Run(void);\nvoid Scene(void);\n"),
            ("games/tbs/include/unit/b.h", "void Scene(void);\n"),
            ("games/common/include/field/field.h", "#if GAME_TLA\nvoid Run(s32 a);\n#endif\n#if !GAME_TLA\nvoid Scene(void);\n#endif\n#if EDITION_INTERNATIONAL\nextern u8 gTable[];\n#else\nextern u16 gTable[];\n#endif\n"),
            ("games/tbs/include/old.h", "void Run(void);\nextern u8 gTable[];\nstatic inline void Help(void) { }\n"),
        ];
        let found: Vec<(String, usize, String)> = second_declarations(&headers).into_iter().map(|f| (f.file, f.line, f.name)).collect();
        assert_eq!(
            found,
            vec![
                ("games/common/include/field/field.h".to_string(), 5, "Scene".to_string()),
                ("games/tbs/include/old.h".to_string(), 1, "Run".to_string()),
                ("games/tbs/include/old.h".to_string(), 2, "gTable".to_string()),
            ]
        );
    }

    #[test]
    fn a_function_is_declared_or_left_out_with_cause() {
        let unit = "games/tbs/src/main/a.c";
        let part = "games/tbs/src/main/f.c";
        let other = "games/tbs/src/o/b.c";
        let header = "games/tbs/include/unit/main/a.h";
        let said = |name: &str| format!("/* {name} is called before its definition with no prototype in scope. */\n");
        let source = "void Run(void)\n{\n\tLate(1);\n\tShared(2);\n\tPart();\n}\nvoid Late(s32 a)\n{\n}\nvoid Never(s32 a)\n{\n}\nvoid User(void)\n{\n\tNever(1);\n}\nvoid Shared(s32 a)\n{\n}\nvoid Bare(void)\n{\n}\nvoid Both(void)\n{\n}\nstatic void Hidden(void)\n{\n}\nvoid Elsewhere(void)\n{\n}\n#include \"f.c\"\n";
        let own = format!("void Run(void);\n{}{}void User(void);\n{}{}void Both(void);\n{}", said("Late"), said("Never"), said("Shared"), said("Both"), said("Gone"));
        let files = [
            (unit, source),
            (part, "void Part(void)\n{\n}\n"),
            (other, "void Scene(void)\n{\n\tShared(1);\n}\n"),
            ("games/tbs/include/old.h", "void Elsewhere(void);\n"),
        ];
        let model = Model::of(&files, &[(unit, &[part, "games/tbs/include/old.h"]), (other, &[])]);
        let headers: BTreeMap<String, String> = [
            (header.to_string(), own),
            ("games/tbs/include/unit/main/f.h".to_string(), said("Part")),
            ("games/tbs/include/unit/o/b.h".to_string(), "void Scene(void);\n".to_string()),
        ]
        .into();
        let all = undeclared(&model, &headers);
        let found: Vec<(&str, usize, &str)> = all.iter().map(|f| (f.file.as_str(), f.line, f.name.as_str())).collect();
        // Late is called above its definition, and Part above the line that
        // includes its file: their comments are true. Elsewhere has a
        // prototype in another header, which is counted as that.
        assert_eq!(
            found,
            vec![
                (unit, 20, "Bare: its header has no prototype of it and no comment in its place"),
                (header, 3, "Never: nothing calls it above its definition"),
                (header, 5, "Shared: games/tbs/src/o/b.c names it too"),
                (header, 6, "Both: its header declares it too"),
                (header, 8, "Gone: its file does not define it for other code"),
            ]
        );
        assert!(all.iter().all(|f| f.kind == "undeclared"));
    }

    #[test]
    fn a_function_another_file_calls_undeclared_is_left_out_by_that_file() {
        let unit = "games/tbs/src/main/a.c";
        let other = "games/tbs/src/main/b.c";
        let header = "games/tbs/include/unit/main/a.h";
        let from = |name: &str, file: &str| format!("/* {name} is called from {file} with no prototype in scope: in one file the definition would come first. */\n");
        let source = "void Shared(s32 a)\n{\n}\nvoid Lone(void)\n{\n}\nvoid Wrong(void)\n{\n}\n";
        let own = format!("{}{}{}", from("Shared", "b.c"), from("Lone", "b.c"), from("Wrong", "c.c"));
        let files = [(unit, source), (other, "void Scene(void)\n{\n\tShared(1);\n\tWrong();\n}\n")];
        let none: &[&str] = &[];
        let model = Model::of(&files, &[(unit, none), (other, none)]);
        let headers: BTreeMap<String, String> =
            [(header.to_string(), own), ("games/tbs/include/unit/main/b.h".to_string(), "void Scene(void);\n".to_string())].into();
        let found: Vec<(usize, String)> = undeclared(&model, &headers).into_iter().map(|f| (f.line, f.name)).collect();
        // b.c calls Shared undeclared: that comment is true, where the
        // first form's would count b.c naming it.
        assert_eq!(found, [(2, "Lone: b.c does not name it".to_string()), (3, "Wrong: c.c does not name it".to_string())]);
    }

    #[test]
    fn a_count_may_only_fall() {
        let baseline = parse("kind\tcount\nextern\t3\nprototype\t2\n").unwrap();
        let counts: Counts = [("extern".to_string(), 2), ("prototype".to_string(), 3), ("cast-macro".to_string(), 1)].into();
        assert_eq!(ratchet(&baseline, &counts), ["cast-macro: 1, above the baseline's 0", "prototype: 3, above the baseline's 2"]);
        assert_eq!(parse(&render(&counts)).unwrap(), counts);
    }
}
