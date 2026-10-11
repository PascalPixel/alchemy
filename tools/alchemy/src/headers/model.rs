//! The ☀️ sources as the headers pass sees them: which files make each
//! translation unit, what each file declares and defines, and where a name
//! a unit uses is defined.

use super::scan::{entries, Entry, What};
use crate::compiler::routing::{family_for_source, CompilerFamily, COMMON_INCLUDE_MODULES};
use crate::targets::{target_for, DecompTargetId};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

pub(crate) const SOURCE: &str = "games/tbs/src/";
pub(crate) const INCLUDE: &str = "games/tbs/include/";
pub(crate) const COMMON: &str = "games/common/include/";
pub(crate) const UNIT: &str = "unit/";

#[derive(Default)]
pub(crate) struct Model {
    /// The C files the editions compile, the C files those include, and
    /// every header they reach.
    pub units: Vec<String>,
    pub fragments: Vec<String>,
    pub headers: Vec<String>,
    pub text: BTreeMap<String, String>,
    pub entries: BTreeMap<String, Vec<Entry>>,
    /// The units of the main image; the others are overlays' units.
    pub main: BTreeSet<String>,
    /// The units on the ARM route, which only the whole build compiles.
    pub arm: BTreeSet<String>,
    /// What Nintendo's library sources include: the pass leaves library
    /// source and what it reads as they are.
    pub frozen: BTreeSet<String>,
    /// The far-call banks' entries, by the stub's name.
    pub far: BTreeMap<String, Far>,
    /// Each overlay unit's import veneers: the name its code calls and the
    /// stub the veneer jumps to.
    pub imports: BTreeMap<String, BTreeMap<String, String>>,
    /// Each unit's files after itself: the fragments and headers it reaches.
    pub reach: BTreeMap<String, Vec<String>>,
    /// A run that leaves objects for a later one: only functions get a
    /// header's declaration.
    pub functions_only: bool,
    /// Objects a run leaves declared as they are: their uses need a view
    /// the bytes do not take, and each is reported.
    pub leave: BTreeSet<String>,
}

/// One entry of a far-call bank's list: `FAR_CALL(name, target, type,
/// params)`, which declares the stub, or `FAR_ENTRY(name, target)`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Far {
    pub target: String,
    pub file: String,
    pub start: usize,
    pub end: usize,
    pub declares: bool,
}

/// The entries of a far-call bank's list.
pub(crate) fn far_entries(file: &str, text: &str) -> Vec<(String, Far)> {
    let mut found = Vec::new();
    let mut at = 0;
    while let Some((start, word)) = ["FAR_CALL(", "FAR_ENTRY("].iter().filter_map(|word| text[at..].find(word).map(|i| (at + i, *word))).min() {
        let open = start + word.len() - 1;
        let Some(close) = crate::camelotize::source::closing(text.as_bytes(), open) else {
            break;
        };
        at = close + 1;
        let line = text[..start].rfind('\n').map_or(0, |n| n + 1);
        if text[line..start].trim_start().starts_with('#') {
            continue;
        }
        let parts = crate::camelotize::source::split_commas(text.as_bytes(), open + 1..close);
        if let [name, target, ..] = &parts[..] {
            let far = Far { target: text[target.clone()].trim().to_string(), file: file.to_string(), start, end: close + 1, declares: word == "FAR_CALL(" };
            found.push((text[name.clone()].trim().to_string(), far));
        }
    }
    found
}

/// The veneers of an overlay's import file: each label and the target of
/// the `overlay_veneer` under it.
pub(crate) fn import_veneers(text: &str) -> BTreeMap<String, String> {
    let mut found = BTreeMap::new();
    let mut label: Option<&str> = None;
    for line in text.lines().map(str::trim) {
        if let Some(name) = line.strip_suffix(':').filter(|name| !name.is_empty() && name.chars().all(|c| c.is_ascii_alphanumeric() || c == '_')) {
            label = Some(name);
        } else if let (Some(target), Some(name)) = (line.strip_prefix("overlay_veneer").map(str::trim), label.take()) {
            found.insert(name.to_string(), target.to_string());
        }
    }
    found
}

pub(crate) fn normalize(path: &str) -> String {
    let mut parts: Vec<&str> = Vec::new();
    for part in path.split('/') {
        match part {
            "" | "." => {}
            ".." => {
                parts.pop();
            }
            part => parts.push(part),
        }
    }
    parts.join("/")
}

fn directory(file: &str) -> &str {
    file.rsplit_once('/').map_or("", |(directory, _)| directory)
}

/// The quoted names a file includes, in order.
pub(crate) fn include_names(text: &str) -> Vec<String> {
    crate::camelotize::source::includes(text).into_iter().map(|(name, _)| name).collect()
}

/// The header a unit header's path names: `games/tbs/src/a/b.c` gives
/// `games/tbs/include/unit/a/b.h`.
pub(crate) fn unit_header(file: &str) -> Option<String> {
    let stem = file.strip_prefix(SOURCE)?.strip_suffix(".c")?;
    Some(format!("{INCLUDE}{UNIT}{stem}.h"))
}

/// How an include line spells a header on the game's search path.
pub(crate) fn spelling(header: &str) -> Option<String> {
    if let Some(rest) = header.strip_prefix(INCLUDE) {
        return Some(rest.to_string());
    }
    let rest = header.strip_prefix(COMMON)?;
    let (module, name) = rest.split_once('/')?;
    COMMON_INCLUDE_MODULES.contains(&module).then(|| name.to_string())
}

impl Model {
    /// Where an include line of `from`, compiled in `unit`, finds its file.
    fn resolve(&self, exists: &dyn Fn(&str) -> bool, unit: &str, from: &str, name: &str) -> Option<String> {
        let mut candidates = vec![format!("{}/{name}", directory(from)), format!("{}/{name}", directory(unit)), format!("{INCLUDE}{name}")];
        candidates.extend(COMMON_INCLUDE_MODULES.iter().map(|module| format!("{COMMON}{module}/{name}")));
        candidates.into_iter().map(|c| normalize(&c)).find(|c| exists(c))
    }

    pub(crate) fn read(root: &Path) -> Result<Model, String> {
        let linked = crate::dead::Linked::read(root)?;
        let game = target_for(DecompTargetId::TbsJa);
        let (mut units, mut library, mut arm) = (BTreeSet::new(), BTreeSet::new(), BTreeSet::new());
        for (target, compiled) in &linked.units {
            if target.compiler != game.compiler {
                continue;
            }
            for unit in compiled {
                match family_for_source(game.compiler, unit) {
                    CompilerFamily::Agbcc | CompilerFamily::AgbccFlash => library.insert(unit.clone()),
                    family if unit.starts_with(SOURCE) => {
                        if family.is_arm() {
                            arm.insert(unit.clone());
                        }
                        units.insert(unit.clone())
                    }
                    _ => false,
                };
            }
        }
        let rows = crate::object_list::read(root, game)?.unwrap_or_default();
        let main: BTreeSet<String> =
            rows.iter().filter(|row| row.image == "main").map(|row| format!("{}.c", row.object)).filter(|file| units.contains(file)).collect();
        let mut model = Model { units: units.into_iter().collect(), main, arm, ..Model::default() };
        let exists = |path: &str| root.join(path).is_file();
        for unit in &library {
            let mut seen = vec![unit.clone()];
            let mut at = 0;
            while at < seen.len() {
                let file = seen[at].clone();
                at += 1;
                let text = std::fs::read_to_string(root.join(&file)).map_err(|e| format!("{file}: {e}"))?;
                for name in include_names(&text) {
                    if let Some(found) = model.resolve(&exists, unit, &file, &name) {
                        if !seen.contains(&found) {
                            seen.push(found);
                        }
                    }
                }
            }
            model.frozen.extend(seen);
        }
        let (mut fragments, mut headers) = (BTreeSet::new(), BTreeSet::new());
        for unit in model.units.clone() {
            let mut seen = vec![unit.clone()];
            let mut at = 0;
            while at < seen.len() {
                let file = seen[at].clone();
                at += 1;
                if !model.text.contains_key(&file) {
                    let text = std::fs::read_to_string(root.join(&file)).map_err(|e| format!("{file}: {e}"))?;
                    model.entries.insert(file.clone(), entries(&text));
                    model.text.insert(file.clone(), text);
                }
                for name in include_names(&model.text[&file]) {
                    if let Some(found) = model.resolve(&exists, &unit, &file, &name) {
                        if !seen.contains(&found) {
                            seen.push(found);
                        }
                    }
                }
            }
            for file in &seen[1..] {
                if file.ends_with(".c") {
                    fragments.insert(file.clone());
                } else {
                    headers.insert(file.clone());
                }
            }
            model.reach.insert(unit, seen[1..].to_vec());
        }
        model.fragments = fragments.into_iter().filter(|f| !model.units.contains(f)).collect();
        model.headers = headers.into_iter().collect();
        for header in model.headers.iter().filter(|header| header.starts_with(&format!("{INCLUDE}farcall/"))) {
            model.far.extend(far_entries(header, &model.text[header]));
        }
        // An overlay's import file stands in its image beside its unit.
        for row in rows.iter().filter(|row| row.image != "main" && row.object.ends_with("/import")) {
            let Ok(text) = std::fs::read_to_string(root.join(format!("{}.s", row.object))) else {
                continue;
            };
            let veneers = import_veneers(&text);
            for unit in rows.iter().filter(|other| other.image == row.image).map(|other| format!("{}.c", other.object)) {
                if model.units.contains(&unit) {
                    model.imports.entry(unit).or_default().extend(veneers.clone());
                }
            }
        }
        Ok(model)
    }

    /// A unit's own C text: itself and the C files it includes.
    pub(crate) fn parts<'a>(&'a self, unit: &'a str) -> impl Iterator<Item = &'a str> {
        std::iter::once(unit).chain(self.reach[unit].iter().map(String::as_str).filter(|f| f.ends_with(".c")))
    }

    /// The units that compile a file: itself, or every unit that reaches it.
    pub(crate) fn users<'a>(&'a self, file: &'a str) -> Vec<&'a str> {
        self.units.iter().map(String::as_str).filter(|unit| *unit == file || self.reach[*unit].iter().any(|f| f == file)).collect()
    }

    /// The functions and objects the C files define for other code, by
    /// name: file and entry.
    pub(crate) fn definitions(&self) -> BTreeMap<&str, Vec<(&str, &Entry)>> {
        let mut found: BTreeMap<&str, Vec<(&str, &Entry)>> = BTreeMap::new();
        for file in self.units.iter().chain(&self.fragments) {
            for entry in &self.entries[file] {
                if matches!(entry.what, What::Definition { .. } | What::Object) && !entry.local {
                    found.entry(&entry.name).or_default().push((file, entry));
                }
            }
        }
        found
    }

    /// Whether a C file's code is in the main image.
    pub(crate) fn in_main(&self, file: &str) -> bool {
        self.main.contains(file) || self.main.iter().any(|unit| self.reach[unit].iter().any(|f| f == file))
    }

    /// The C file whose definition of `name` a unit's code means: one in
    /// the unit's own text, else the main image's only one.
    pub(crate) fn home<'a>(&'a self, definitions: &BTreeMap<&str, Vec<(&'a str, &'a Entry)>>, unit: &str, name: &str) -> Option<(&'a str, &'a Entry)> {
        let all = definitions.get(name)?;
        if let Some(own) = self.parts(unit).find_map(|part| all.iter().find(|(file, _)| *file == part)) {
            return Some(*own);
        }
        let mut main = all.iter().filter(|(file, _)| self.in_main(file));
        match (main.next(), main.next()) {
            (Some(one), None) => Some(*one),
            (Some(one), Some(_)) => {
                // Edition arms of one function in one file are one definition.
                let rest: Vec<_> = all.iter().filter(|(file, _)| self.in_main(file)).collect();
                rest.iter().all(|(file, _)| *file == one.0).then_some(*one)
            }
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_banks_list_gives_each_stub_its_target() {
        let text = "#define FAR_CALL(name, target, type, params) type name params;\nFAR_ENTRY(GetFar, Get)\n#ifndef OWN_RunFar\nFAR_CALL(\n\tRunFar, Run, struct P *, (s32 a, void (*hook)(s32)))\n#endif\nFAR_SLOT(Idle)\n";
        let found = far_entries("farcall/a.h", text);
        assert_eq!(found.iter().map(|(name, far)| (name.as_str(), far.target.as_str(), far.declares, &text[far.start..far.end])).collect::<Vec<_>>(), [
            ("GetFar", "Get", false, "FAR_ENTRY(GetFar, Get)"),
            ("RunFar", "Run", true, "FAR_CALL(\n\tRunFar, Run, struct P *, (s32 a, void (*hook)(s32)))"),
        ]);
    }

    #[test]
    fn an_import_file_gives_each_veneer_its_target() {
        let text = "\t.thumb\n\t.global Engine_TaskWait\n\t.thumb_func\nEngine_TaskWait:\n\toverlay_veneer WaitFramesFar\n\t.global __divsi3\n__divsi3:\n\toverlay_veneer IwramSignedDivide\nData:\n\t.word 0\n";
        assert_eq!(import_veneers(text).into_iter().collect::<Vec<_>>(), [("Engine_TaskWait".to_string(), "WaitFramesFar".to_string()), ("__divsi3".to_string(), "IwramSignedDivide".to_string())]);
    }

    #[test]
    fn paths_and_spellings() {
        assert_eq!(normalize("games/tbs/src/field/a/../common/x.c"), "games/tbs/src/field/common/x.c");
        assert_eq!(unit_header("games/tbs/src/field/a/mura.c").as_deref(), Some("games/tbs/include/unit/field/a/mura.h"));
        assert_eq!(unit_header("games/common/src/a.c"), None);
        assert_eq!(spelling("games/tbs/include/unit/field/a/mura.h").as_deref(), Some("unit/field/a/mura.h"));
        assert_eq!(spelling("games/common/include/battle/battle.h").as_deref(), Some("battle.h"));
        assert_eq!(spelling("games/tbs/src/field/a/cave.h"), None);
    }

    fn model() -> Model {
        let mut model = Model::default();
        let mut add = |file: &str, text: &str| {
            model.entries.insert(file.to_string(), entries(text));
            model.text.insert(file.to_string(), text.to_string());
        };
        add("games/tbs/src/a.c", "void Run(void)\n{\n}\ns32 Shared(void)\n{\n}\n");
        add("games/tbs/src/o1.c", "s32 Scene(void)\n{\n}\n#include \"f.c\"\n");
        add("games/tbs/src/o2.c", "u8 Scene(s32 a)\n{\n}\ns32 Shared(s32 own)\n{\n}\n");
        add("games/tbs/src/f.c", "void Part(void)\n{\n}\n");
        model.units = vec!["games/tbs/src/a.c".into(), "games/tbs/src/o1.c".into(), "games/tbs/src/o2.c".into()];
        model.fragments = vec!["games/tbs/src/f.c".into()];
        model.main.insert("games/tbs/src/a.c".into());
        model.reach.insert("games/tbs/src/a.c".into(), vec![]);
        model.reach.insert("games/tbs/src/o1.c".into(), vec!["games/tbs/src/f.c".into()]);
        model.reach.insert("games/tbs/src/o2.c".into(), vec![]);
        model
    }

    #[test]
    fn a_name_means_the_units_own_definition_then_the_main_images() {
        let model = model();
        let definitions = model.definitions();
        let home = |unit: &str, name: &str| model.home(&definitions, unit, name).map(|(file, _)| file);
        assert_eq!(home("games/tbs/src/o1.c", "Scene"), Some("games/tbs/src/o1.c"));
        assert_eq!(home("games/tbs/src/o2.c", "Scene"), Some("games/tbs/src/o2.c"));
        assert_eq!(home("games/tbs/src/a.c", "Scene"), None);
        assert_eq!(home("games/tbs/src/o1.c", "Part"), Some("games/tbs/src/f.c"));
        assert_eq!(home("games/tbs/src/o2.c", "Part"), None);
        assert_eq!(home("games/tbs/src/o1.c", "Shared"), Some("games/tbs/src/a.c"));
        assert_eq!(home("games/tbs/src/o2.c", "Shared"), Some("games/tbs/src/o2.c"));
        assert_eq!(model.users("games/tbs/src/f.c"), vec!["games/tbs/src/o1.c"]);
    }
}
