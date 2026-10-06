mod barriers;
mod declarations;
mod fields;
mod registers;
mod source;
mod stabs;

use crate::compiler::routing::{root, COMMON_INCLUDE_MODULES};
use crate::targets::{target_for, DecompTarget, DecompTargetId};
use std::collections::{HashMap, HashSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy camelotize REWRITE FILE.C...
Rewrites maintained C the way Camelot wrote it, in place, and prints how
many places changed in each file. REWRITE is one of:
  actors        byte offsets on field actors as struct FieldActor members
  events        byte offsets on the event work as struct EventWork members
  registers     hardware register addresses by their IO_REG.H names
  declarations  a file's own declarations of functions a header declares,
                replaced by an include of that header
  barriers      FAKEMATCH-tagged empty asm barriers, with their tag
Struct layouts and types come from the build's own compiler for each game
the file builds in. Then run alchemy keep: it keeps only the files whose
edits leave all twelve editions byte-identical."#;

fn games(file: &Path) -> Vec<DecompTarget> {
    let text = file.to_string_lossy();
    let mut found = Vec::new();
    if !text.contains("THE LOST AGE") {
        found.push(target_for(DecompTargetId::TbsJa));
    }
    if !text.contains("THE BROKEN SEAL") {
        found.push(target_for(DecompTargetId::TlaJa));
    }
    found
}

fn search(root: &Path, target: DecompTarget) -> Vec<PathBuf> {
    let mut paths = vec![root.join("games").join(target.game_dir()).join("INCLUDE")];
    paths.extend(
        COMMON_INCLUDE_MODULES
            .iter()
            .map(|module| root.join("games/COMMON/INCLUDE").join(module)),
    );
    paths
}

fn header_words(file: &Path, search: &[PathBuf]) -> (HashSet<String>, String) {
    let mut words = HashSet::new();
    let mut texts = String::new();
    for header in source::reachable(file, search) {
        if header == file {
            continue;
        }
        let Ok(text) = std::fs::read_to_string(&header) else {
            continue;
        };
        for word in text.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')) {
            if !word.is_empty() {
                words.insert(word.to_string());
            }
        }
        texts.push_str(&text);
        texts.push('\n');
    }
    (words, texts)
}

fn unit(root: &Path, file: &Path, text: &str, spec: &fields::Spec) -> Result<Option<fields::Unit>, String> {
    let names: Vec<&str> = spec.getters.iter().chain(spec.globals).copied().collect();
    let probes: Vec<stabs::Probe> = names
        .iter()
        .enumerate()
        .map(|(i, name)| stabs::Probe {
            name: format!("alchemy_probe_{i}"),
            expression: if spec.getters.contains(name) {
                format!("{name}(0)")
            } else {
                (*name).to_string()
            },
        })
        .collect();
    let mut layouts = Vec::new();
    let mut agreed: Option<HashMap<String, stabs::Kind>> = None;
    let mut declared = HashSet::new();
    let mut header_text = String::new();
    for target in games(file) {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let compiled = stabs::compile(target, imports.as_ref().map(|d| d.path()), file, &probes)?;
        let Some(members) = compiled.members(spec.tag) else {
            return Ok(None);
        };
        layouts.push(members);
        let types: HashMap<String, stabs::Kind> = names
            .iter()
            .enumerate()
            .filter_map(|(i, name)| Some((name.to_string(), compiled.typedef(&format!("alchemy_probe_{i}"))?)))
            .collect();
        agreed = Some(match agreed {
            None => types,
            Some(before) => before.into_iter().filter(|(k, v)| types.get(k) == Some(v)).collect(),
        });
        let (words, texts) = header_words(file, &search(root, target));
        declared.extend(words);
        header_text.push_str(&texts);
    }
    let macros = fields::macros(&format!("{header_text}\n{text}"));
    Ok(Some(fields::Unit { layouts, types: agreed.unwrap_or_default(), declared, macros }))
}

fn registers_in(root: &Path, file: &Path, text: &str) -> Result<(String, usize), String> {
    let mut all = Vec::new();
    for header in registers::HEADERS {
        let path = root.join(header);
        let body = std::fs::read_to_string(&path).map_err(|e| format!("{header}: {e}"))?;
        all.extend(registers::defines(&body, &path));
    }
    let table = registers::Table::new(all);
    let (mut out, count, headers) = registers::rewrite(text, &table);
    if count == 0 {
        return Ok((out, 0));
    }
    let target = games(file)[0];
    let paths = search(root, target);
    let reached = source::reachable(file, &paths);
    for header in headers {
        let header = header.canonicalize().unwrap_or(header);
        if reached.iter().any(|h| h.canonicalize().ok().as_deref() == Some(header.as_path())) {
            continue;
        }
        let name = source::spelling(file, &header, &paths)
            .ok_or_else(|| format!("{}: cannot spell an include of {}", file.display(), header.display()))?;
        out = source::insert_include(&out, &name);
    }
    Ok((out, count))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let Some((rewrite, files)) = arguments.split_first() else {
        return Err(USAGE.into());
    };
    if rewrite == "-h" || rewrite == "--help" || files.is_empty() {
        return Err(USAGE.into());
    }
    let root = root();
    let mut total = 0;
    for name in files {
        let file = std::path::absolute(name).map_err(|e| format!("{name}: {e}"))?;
        let text = std::fs::read_to_string(&file).map_err(|e| format!("{name}: {e}"))?;
        let (out, count) = match rewrite.as_str() {
            "actors" | "events" => {
                let spec = if rewrite == "actors" { &fields::ACTORS } else { &fields::EVENTS };
                match unit(root, &file, &text, spec) {
                    Ok(Some(unit)) => fields::rewrite(&text, spec, &unit),
                    Ok(None) => (text.clone(), 0),
                    Err(error) => {
                        eprintln!("{name}: skipped: {}", error.lines().next().unwrap_or(""));
                        continue;
                    }
                }
            }
            "registers" => registers_in(root, &file, &text)?,
            "declarations" => {
                let target = games(&file)[0];
                declarations::rewrite(&text, &declarations::index(root, target)?)
            }
            "barriers" => barriers::rewrite(&text),
            other => return Err(format!("unknown rewrite {other}\n{USAGE}")),
        };
        if count > 0 && out != text {
            std::fs::write(&file, &out).map_err(|e| format!("{name}: {e}"))?;
            println!("{count}\t{name}");
            total += count;
        }
    }
    println!("{total} changes; run alchemy keep to keep only byte-identical files");
    Ok(())
}
