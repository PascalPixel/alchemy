//! Source edits proved one file at a time: each set of edits is compiled and
//! assembled with each edition's compiler, flags and assembler, and kept only
//! when the object (every section's bytes, every relocation and symbol) is
//! identical and the compiler says nothing new in every edition of the game
//! that builds the file. Sets are tried together and bisected
//! when one fails, so a file costs a handful of compiles, not one per edit.

use crate::compiler::bundle::compiler_command;
use crate::compiler::routing::{cflags_for_target_source, compiler_assembly_command, is_arm};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::path::Path;
use std::process::Command;

/// Replace `start..end` with `text`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Edit {
    pub start: usize,
    pub end: usize,
    pub text: String,
}

fn joins(a: u8, b: u8) -> bool {
    let word = |c: u8| c.is_ascii_alphanumeric() || c == b'_';
    let operator = |c: u8| b"+-*/%<>=!&|^~.".contains(&c);
    (word(a) && word(b)) || (operator(a) && operator(b))
}

/// The text with the edits made, overlapping ones after the first dropped.
/// An edit that takes text out also takes the blanks after it, and keeps one
/// space wherever the tokens on either side would otherwise run together.
pub(crate) fn apply(text: &str, edits: &[Edit]) -> String {
    let mut sorted = edits.to_vec();
    sorted.sort_by_key(|e| e.start);
    let bytes = text.as_bytes();
    let mut out = String::with_capacity(text.len());
    let mut at = 0;
    for edit in sorted {
        if edit.start < at {
            continue;
        }
        out.push_str(&text[at..edit.start]);
        let mut end = edit.end;
        if edit.text.is_empty() {
            while end < bytes.len() && matches!(bytes[end], b' ' | b'\t') {
                end += 1;
            }
        }
        let next = edit.text.bytes().next().or_else(|| bytes.get(end).copied());
        if let (Some(&a), Some(b)) = (out.as_bytes().last(), next) {
            if joins(a, b) {
                out.push(' ');
            }
        }
        out.push_str(&edit.text);
        if let (Some(a), Some(&b)) = (edit.text.bytes().last(), bytes.get(end)) {
            if joins(a, b) {
                out.push(' ');
            }
        }
        at = end;
    }
    out.push_str(&text[at..]);
    out
}

/// The candidates outside FAKEMATCH-tagged statements and asm.
pub(crate) fn untagged(text: &str, found: Vec<Edit>) -> Vec<Edit> {
    let tagged = super::tagged::ranges(text);
    found.into_iter().filter(|e| !super::tagged::covered(&tagged, e.start, e.end)).collect()
}

/// What an object file gives the linker: every section's name, size and
/// bytes, its relocations by target name, and its symbols. Debug sections
/// and the local `.L` labels, which only number GCC's jumps, are left out.
pub(crate) fn object_contents(bytes: &[u8]) -> Result<String, String> {
    use object::{Object, ObjectSection, ObjectSymbol, RelocationTarget, SectionKind};
    use std::fmt::Write;
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let section_name = |index| file.section_by_index(index).ok().and_then(|s| s.name().ok().map(str::to_string)).unwrap_or_default();
    let mut out = String::new();
    for section in file.sections() {
        let name = section.name().unwrap_or("");
        if matches!(section.kind(), SectionKind::Metadata | SectionKind::Debug | SectionKind::DebugString)
            || name.starts_with(".stab")
            || name.starts_with(".debug")
        {
            continue;
        }
        let data = section.data().map_err(|e| e.to_string())?;
        let _ = writeln!(out, "section {name} {:?} size {} align {} {data:02x?}", section.kind(), section.size(), section.align());
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
            let _ = writeln!(out, "  reloc {offset:#x} {:?} {} {target}", relocation.flags(), relocation.addend());
        }
    }
    let mut symbols: Vec<String> = file
        .symbols()
        .filter(|s| !s.name().unwrap_or("").starts_with(".L"))
        .map(|s| {
            let section = s.section_index().map(section_name).unwrap_or_default();
            format!("symbol {} {section} {:#x} {} {:?} {:?}", s.name().unwrap_or(""), s.address(), s.size(), s.kind(), s.scope())
        })
        .collect();
    symbols.sort();
    out.push_str(&symbols.join("\n"));
    Ok(out)
}

/// The compiler's messages without their line numbers, sorted, so an edit
/// that moves lines (an else flattened) leaves them the same unless a
/// message is new.
pub(crate) fn unnumbered(messages: &str) -> String {
    let number = regex::Regex::new(r":\d+:").expect("pattern");
    let mut lines: Vec<String> = messages.lines().map(|line| number.replace(line, ":").into_owned()).collect();
    lines.sort();
    lines.join("\n")
}

/// Candidates that overlap an earlier one are dropped.
pub(crate) fn disjoint(mut edits: Vec<Edit>) -> Vec<Edit> {
    edits.sort_by_key(|e| e.start);
    let mut out: Vec<Edit> = Vec::with_capacity(edits.len());
    for edit in edits {
        if out.last().is_none_or(|last| last.end <= edit.start) {
            out.push(edit);
        }
    }
    out
}

struct Build {
    target: DecompTarget,
    imports: Option<tempfile::TempDir>,
    directory: tempfile::TempDir,
}

impl Build {
    /// The object's contents and the compiler's messages for this text in
    /// place of the file. Assembly text is not compared: an edit that only
    /// renumbers GCC's `.L` labels leaves the object identical.
    fn compile(&self, root: &Path, file: &Path, text: &str) -> Result<(String, String), String> {
        let (bytes, messages) = self.assemble(root, file, text)?;
        Ok((object_contents(&bytes)?, unnumbered(&messages)))
    }

    /// The object file itself and the messages as the tools gave them.
    fn assemble(&self, root: &Path, file: &Path, text: &str) -> Result<(Vec<u8>, String), String> {
        self.assemble_among(root, file, text, &[])
    }

    /// As `assemble`, with other files of the tree read as these texts:
    /// each is a path under the root and what stands in its place. The
    /// file and the others are laid out in a copy of the tree's folders,
    /// so an include beside the file finds a replaced neighbour first, and
    /// each folder of the search path that holds a replaced file is
    /// searched in the copy before the tree.
    fn assemble_among(&self, root: &Path, file: &Path, text: &str, others: &[(String, String)]) -> Result<(Vec<u8>, String), String> {
        let name = file.file_name().and_then(|n| n.to_str()).unwrap_or("source.c");
        let assembly = self.directory.path().join("out.s");
        let object = self.directory.path().join("OUT.O");
        let flags = cflags_for_target_source(self.target.compiler, name);
        let mut arguments = Vec::new();
        let source = if others.is_empty() {
            self.directory.path().join(name)
        } else {
            let copy = self.directory.path().join("tree");
            if copy.exists() {
                std::fs::remove_dir_all(&copy).map_err(|e| e.to_string())?;
            }
            let place = |path: &Path, text: &str| -> Result<std::path::PathBuf, String> {
                let to = copy.join(path.strip_prefix(root).unwrap_or(path));
                std::fs::create_dir_all(to.parent().unwrap_or(&copy)).map_err(|e| e.to_string())?;
                std::fs::write(&to, text).map_err(|e| e.to_string())?;
                Ok(to)
            };
            for (path, text) in others {
                place(Path::new(path), text)?;
            }
            for flag in &flags {
                let shadow = flag.strip_prefix("-I").and_then(|folder| Path::new(folder).strip_prefix(root).ok()).map(|folder| copy.join(folder));
                if let Some(shadow) = shadow.filter(|shadow| shadow.is_dir()) {
                    arguments.push(format!("-I{}", shadow.display()));
                }
            }
            place(file, "")?
        };
        std::fs::write(&source, text).map_err(|e| e.to_string())?;
        arguments.push(format!("-I{}", file.parent().unwrap_or(root).display()));
        arguments.extend(flags);
        // What a replaced C file includes beside itself is still found
        // beside it, after everything the search path offers.
        for (path, _) in others.iter().filter(|(path, _)| path.ends_with(".c")) {
            let beside = format!("-I{}", root.join(path).parent().unwrap_or(root).display());
            if !arguments.contains(&beside) {
                arguments.push(beside);
            }
        }
        arguments.push(format!("-D{}=1", self.target.edition_define));
        if let Some(imports) = &self.imports {
            arguments.push(format!("-I{}", imports.path().display()));
        }
        arguments.extend(["-S".into(), "-o".into(), assembly.to_string_lossy().into_owned()]);
        arguments.extend(["-x".into(), "c".into(), source.to_string_lossy().into_owned()]);
        let command = compiler_command(&arguments)?;
        let output = Command::new(&command[0]).args(&command[1..]).current_dir(root).output().map_err(|e| e.to_string())?;
        let mut messages = String::from_utf8_lossy(&output.stderr).into_owned();
        if !output.status.success() {
            return Err(messages);
        }
        let arm = is_arm(self.target.compiler, name);
        let command = compiler_assembly_command(&assembly.to_string_lossy(), &object.to_string_lossy(), arm);
        let output = Command::new(&command[0]).args(&command[1..]).current_dir(root).output().map_err(|e| e.to_string())?;
        messages.push_str(&String::from_utf8_lossy(&output.stderr));
        if !output.status.success() {
            return Err(messages);
        }
        let bytes = std::fs::read(&object).map_err(|e| e.to_string())?;
        Ok((bytes, messages))
    }
}

/// Every edition of these games.
pub(crate) fn editions(games: &[DecompTarget]) -> Vec<DecompTarget> {
    games
        .iter()
        .flat_map(|&game| TARGET_IDS.iter().map(|&id| target_for(id)).filter(move |t| t.compiler == game.compiler))
        .collect()
}

/// The edits that leave the file's assembly and messages unchanged in every
/// game it builds in, applied, and how many there were.
pub(crate) fn proved(root: &Path, file: &Path, text: &str, games: &[DecompTarget], found: Vec<Edit>) -> Result<(String, usize), String> {
    let found = disjoint(found);
    if found.is_empty() {
        return Ok((text.to_string(), 0));
    }
    // Every edition of the game proves every edit: an edition's headers and
    // defines can change what an edit outside any #if means (script.c's
    // casts kept English and broke tbs-ja). A file one edition cannot
    // compile is skipped whole.
    let mut builds = Vec::new();
    for target in editions(games) {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let directory = tempfile::Builder::new().prefix("alchemy-trial-").tempdir().map_err(|e| e.to_string())?;
        let build = Build { target, imports, directory };
        let baseline = build.compile(root, file, text)?;
        builds.push((build, baseline));
    }
    let (kept, _) = crate::keep::bisect(&found, |set: &[Edit]| -> Result<bool, String> {
        let candidate = apply(text, set);
        for (build, baseline) in &builds {
            match build.compile(root, file, &candidate) {
                Ok(result) if &result == baseline => {}
                _ => return Ok(false),
            }
        }
        Ok(true)
    })?;
    Ok((apply(text, &kept), kept.len()))
}

/// One edition's compiler for whole-file comparisons outside `proved`.
pub(crate) struct Compiler {
    build: Build,
}

impl Compiler {
    pub(crate) fn new(root: &Path, target: DecompTarget) -> Result<Compiler, String> {
        let imports = crate::build_text::fresh_c_imports(root, target)?;
        let directory = tempfile::Builder::new().prefix("alchemy-trial-").tempdir().map_err(|e| e.to_string())?;
        Ok(Compiler { build: Build { target, imports, directory } })
    }

    /// The object file of this text in place of the file, or the
    /// compiler's messages when it does not compile.
    pub(crate) fn object(&self, root: &Path, file: &Path, text: &str) -> Result<Vec<u8>, String> {
        self.build.assemble(root, file, text).map(|(object, _)| object)
    }

    /// The object file of this text in place of the file, compiled among
    /// other replaced files of the tree: each a path under the root and
    /// the text read in its place.
    pub(crate) fn object_among(&self, root: &Path, file: &Path, text: &str, others: &[(String, String)]) -> Result<Vec<u8>, String> {
        self.build.assemble_among(root, file, text, others).map(|(object, _)| object)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn cut(start: usize, end: usize) -> Edit {
        Edit { start, end, text: String::new() }
    }

    #[test]
    fn removals_keep_tokens_apart_and_overlaps_go() {
        let text = "a = b -(s16)-c;\nreturn(u8)x;\n";
        assert_eq!(apply(text, &[cut(7, 12), cut(22, 26)]), "a = b - -c;\nreturn x;\n");
        let edits = disjoint(vec![cut(0, 4), cut(2, 6), cut(6, 8)]);
        assert_eq!(edits, vec![cut(0, 4), cut(6, 8)]);
    }

    #[test]
    fn messages_compare_without_line_numbers() {
        let before = "a.c: In function `F':\nA.C:12: warning: unused variable `x'\n";
        let after = "a.c: In function `F':\nA.C:11: warning: unused variable `x'\n";
        assert_eq!(unnumbered(before), unnumbered(after));
        assert_ne!(unnumbered(before), unnumbered("a.c: In function `F':\nA.C:11: warning: unused variable `y'\n"));
    }

    #[test]
    fn replacements_are_spaced_only_when_needed() {
        assert_eq!(apply("x=(y);", &[Edit { start: 2, end: 5, text: "y".into() }]), "x=y;");
        assert_eq!(apply("return(y);", &[Edit { start: 6, end: 9, text: "y".into() }]), "return y;");
    }
}
