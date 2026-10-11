//! A unit's object in each edition, part by part: each function and each
//! data object by its own bytes and relocations, so a change is known by
//! the name it changes and not only by the file.

use crate::camelotize::trial::Compiler;
use crate::targets::{target_for, DecompTarget, DecompTargetId};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

pub(crate) fn editions() -> [DecompTarget; 6] {
    target_for(DecompTargetId::TbsJa).editions()
}

/// The bytes of one symbol's extent, or of what is left of a section, and
/// the relocations inside it by offset and target name.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Part {
    pub bytes: Vec<u8>,
    pub relocations: String,
    /// Whether the part is a function's code.
    pub code: bool,
}

pub(crate) type Parts = BTreeMap<String, Part>;

/// What an object file gives the linker, by name: every sized symbol's
/// extent, the rest of each section under the section's name, and the
/// symbol table under `symbols`. Debug sections are left out.
pub(crate) fn parts(bytes: &[u8]) -> Result<Parts, String> {
    use object::{Object, ObjectSection, ObjectSymbol, RelocationTarget, SectionKind, SymbolKind};
    use std::fmt::Write;
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let section_name = |index| file.section_by_index(index).ok().and_then(|s| s.name().ok().map(str::to_string)).unwrap_or_default();
    let mut out = Parts::new();
    for section in file.sections() {
        let name = section.name().unwrap_or("");
        if matches!(section.kind(), SectionKind::Metadata | SectionKind::Debug | SectionKind::DebugString) || name.starts_with(".stab") || name.starts_with(".debug") {
            continue;
        }
        let data = section.data().map_err(|e| e.to_string())?;
        // (start, end, name, code), by start.
        let mut owners: Vec<(u64, u64, String, bool)> = file
            .symbols()
            .filter(|s| s.section_index() == Some(section.index()) && s.size() > 0 && !s.name().unwrap_or("").starts_with(".L"))
            .filter(|s| matches!(s.kind(), SymbolKind::Text | SymbolKind::Data | SymbolKind::Unknown))
            .map(|s| {
                // A Thumb function's symbol is not always typed as code.
                let code = section.kind() == SectionKind::Text;
                let start = (if code { s.address() & !1 } else { s.address() }).saturating_sub(section.address());
                let length = data.len() as u64;
                (start.min(length), (start + s.size()).min(length), s.name().unwrap_or("").to_string(), code)
            })
            .collect();
        owners.sort();
        owners.dedup_by(|later, earlier| later.0 < earlier.1);
        let mut rest = Part::default();
        let mut at = 0u64;
        for (start, end, symbol, code) in &owners {
            rest.bytes.extend_from_slice(&data[at as usize..(*start).max(at) as usize]);
            let part = out.entry(symbol.clone()).or_default();
            part.bytes.extend_from_slice(&data[*start as usize..*end as usize]);
            part.code = *code;
            at = *end;
        }
        rest.bytes.extend_from_slice(&data[(at as usize).min(data.len())..]);
        for (offset, relocation) in section.relocations() {
            let target = match relocation.target() {
                RelocationTarget::Symbol(index) => match file.symbol_by_index(index) {
                    Ok(symbol) if !symbol.name().unwrap_or("").is_empty() => symbol.name().unwrap_or("").to_string(),
                    Ok(symbol) => symbol.section_index().map(section_name).unwrap_or_default(),
                    Err(e) => return Err(e.to_string()),
                },
                RelocationTarget::Section(index) => section_name(index),
                _ => String::new(),
            };
            let owner = owners.iter().find(|(start, end, _, _)| (*start..*end).contains(&offset));
            let (part, base) = match owner {
                Some((start, _, symbol, _)) => (out.get_mut(symbol).expect("a part of each owner"), *start),
                None => (&mut rest, 0),
            };
            let _ = writeln!(part.relocations, "{:#x} {:?} {} {target}", offset - base, relocation.flags(), relocation.addend());
        }
        let _ = writeln!(rest.relocations, "{:?} size {} align {}", section.kind(), section.size(), section.align());
        out.insert(format!("section {name}"), rest);
    }
    let mut symbols: Vec<String> = file
        .symbols()
        .filter(|s| !s.name().unwrap_or("").starts_with(".L") && !s.name().unwrap_or("").is_empty())
        .map(|s| format!("{} {} {} {:?} {:?}", s.name().unwrap_or(""), s.section_index().map(section_name).unwrap_or_default(), s.size(), s.kind(), s.scope()))
        .collect();
    symbols.sort();
    out.insert("symbols".to_string(), Part { relocations: symbols.join("\n"), ..Part::default() });
    Ok(out)
}

/// A unit's compilers, one per edition, in `editions()` order.
pub(crate) struct Bench {
    compilers: Vec<Compiler>,
}

/// A unit's parts in each edition, or the compiler's messages.
pub(crate) type Built = Vec<Result<Parts, String>>;

impl Bench {
    pub(crate) fn new(root: &Path) -> Result<Bench, String> {
        Ok(Bench { compilers: editions().iter().map(|t| Compiler::new(root, *t)).collect::<Result<_, _>>()? })
    }

    /// The parts of this text in place of the file, in every edition.
    pub(crate) fn build(&self, root: &Path, file: &str, text: &str) -> Built {
        self.compilers.iter().map(|compiler| compiler.object(root, &root.join(file), text).and_then(|bytes| parts(&bytes))).collect()
    }
}

/// The names whose part differs between two builds in any edition; an
/// edition that does not compile on either side gives `compile`.
pub(crate) fn differing(before: &Built, now: &Built) -> BTreeSet<String> {
    let mut found = BTreeSet::new();
    for (old, new) in before.iter().zip(now) {
        let (Ok(old), Ok(new)) = (old, new) else {
            if old.is_ok() != new.is_ok() {
                found.insert("compile".to_string());
            }
            continue;
        };
        for name in old.keys().chain(new.keys()) {
            if old.get(name) != new.get(name) {
                found.insert(name.clone());
            }
        }
    }
    found
}

/// How one name's part differs, in the first edition where it does.
pub(crate) fn difference(before: &Built, now: &Built, name: &str) -> Option<String> {
    for (old, new) in before.iter().zip(now) {
        let (Ok(old), Ok(new)) = (old, new) else {
            continue;
        };
        let (Some(old), Some(new)) = (old.get(name), new.get(name)) else {
            continue;
        };
        if old.bytes.len() != new.bytes.len() {
            let (longer, by) = if new.bytes.len() > old.bytes.len() { ("longer", new.bytes.len() - old.bytes.len()) } else { ("shorter", old.bytes.len() - new.bytes.len()) };
            return Some(format!("is {by} bytes {longer}"));
        }
        let changed = old.bytes.iter().zip(&new.bytes).filter(|(a, b)| a != b).count();
        if changed > 0 {
            return Some(format!("has {changed} of {} bytes different", old.bytes.len()));
        }
        if old.relocations != new.relocations {
            return Some("has other relocations".to_string());
        }
    }
    None
}

/// The first line of a compiler's messages that is not a warning or a
/// line of context.
pub(crate) fn first_error(messages: &str) -> String {
    let line = messages.lines().find(|l| !l.contains("warning:") && !l.contains("In function") && !l.contains("At top level") && !l.contains("In file included") && !l.trim_start().starts_with("from ") && !l.trim().is_empty());
    line.unwrap_or("").replace('\t', " ")
}

#[cfg(test)]
mod tests {
    use super::*;

    fn built(parts: &[(&str, &[u8], &str)]) -> Built {
        vec![Ok(parts.iter().map(|(name, bytes, relocations)| (name.to_string(), Part { bytes: bytes.to_vec(), relocations: relocations.to_string(), code: true })).collect())]
    }

    #[test]
    fn a_difference_is_known_by_the_part_it_changes() {
        let before = built(&[("Run", &[1, 2, 3, 4], "0x0 Draw"), ("Draw", &[5, 6], ""), ("Gone", &[1], "")]);
        let now = built(&[("Run", &[1, 2, 9, 4], "0x0 Draw"), ("Draw", &[5, 6, 7, 8], "")]);
        assert_eq!(differing(&before, &now).into_iter().collect::<Vec<_>>(), ["Draw", "Gone", "Run"]);
        assert_eq!(difference(&before, &now, "Run").as_deref(), Some("has 1 of 4 bytes different"));
        assert_eq!(difference(&before, &now, "Draw").as_deref(), Some("is 2 bytes longer"));
        assert_eq!(differing(&before, &vec![Err("x.c:1: parse error".into())]).into_iter().collect::<Vec<_>>(), ["compile"]);
        assert_eq!(first_error("In file included from a.h:3,\n                 from b.c:1:\nx.c: In function `Run':\nx.c:3: warning: y\nx.c:4: too many arguments to function `Draw'\n"), "x.c:4: too many arguments to function `Draw'");
    }
}
