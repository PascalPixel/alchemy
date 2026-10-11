//! alchemy check headers: a ☀️ C file declares nothing another file
//! defines, and no name is declared twice. What is still there is counted
//! by kind against a tracked baseline, and a count may only fall.

use super::model::{Model, COMMON, INCLUDE, UNIT};
use super::scan::{bodies, What};
use std::collections::BTreeMap;
use std::path::Path;

pub(crate) const BASELINE: &str = "tools/alchemy/headers.tsv";

const USAGE: &str = "usage: alchemy check headers [--update | --list]\n\
Counts, in ☀️'s game C files and the C files they include: prototypes of\n\
functions (at file scope and inside functions), extern declarations of\n\
objects (one inside a function under a FAKEMATCH tag is the device S2\n\
allows and is not counted), and macros that cast a name to another type\n\
under the name itself. Counts, in the headers those files read, each\n\
declaration of a name some other header already declares (a line under\n\
#if GAME_TLA is ⚓️'s and not counted; unit headers declare what their own\n\
file defines, so the same name in two of them is two functions). Fails\n\
when a count rises above tools/alchemy/headers.tsv. --update writes the\n\
current counts there; --list prints every finding.";

pub(crate) const KINDS: [&str; 4] = ["prototype", "extern", "cast-macro", "second-declaration"];

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
    for entry in super::scan::entries(text) {
        match entry.what {
            What::Prototype if !entry.local => push("prototype", entry.start, &entry.name),
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

pub(crate) fn findings(root: &Path) -> Result<Vec<Finding>, String> {
    let model = Model::read(root)?;
    let mut found = Vec::new();
    for file in model.units.iter().chain(&model.fragments) {
        found.extend(in_source(file, &model.text[file]));
    }
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
        let source = "#include \"unit/a.h\"\ns32 Other(s32 a);\nextern u8 gTable[];\nstatic void Mine(void);\n#define Other ((void (*)(s32))Other)\n#define gTable ((u16(*)) & gTable)\n#define LIMIT 4\n\nvoid Run(void)\n{\n\tvoid Deep();\n\textern u8 gWork[];\n\t/* FAKEMATCH: the address folds only through this view. */\n\textern u16 gFolded[];\n\tDeep();\n}\n";
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
    fn a_count_may_only_fall() {
        let baseline = parse("kind\tcount\nextern\t3\nprototype\t2\n").unwrap();
        let counts: Counts = [("extern".to_string(), 2), ("prototype".to_string(), 3), ("cast-macro".to_string(), 1)].into();
        assert_eq!(ratchet(&baseline, &counts), ["cast-macro: 1, above the baseline's 0", "prototype: 3, above the baseline's 2"]);
        assert_eq!(parse(&render(&counts)).unwrap(), counts);
    }
}
