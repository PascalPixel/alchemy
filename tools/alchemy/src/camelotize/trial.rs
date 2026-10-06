//! Source edits proved one file at a time: each set of edits is compiled with
//! the English build's compiler and flags and kept only when the file's
//! assembly (code, data and every relocation it names) is identical and the
//! compiler says nothing new. An edit inside an `#if` block is compiled for
//! every edition of the game instead. Sets are tried together and bisected
//! when one fails, so a file costs a handful of compiles, not one per edit.

use crate::compiler::bundle::compiler_command_for_target;
use crate::compiler::routing::cflags_for_target_source;
use crate::targets::{target_for, DecompTarget, DecompTargetId, TARGET_IDS};
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

/// The byte ranges inside `#if` ... `#endif` blocks, where one build
/// compiles only one branch.
pub(crate) fn conditional(text: &str) -> Vec<std::ops::Range<usize>> {
    let mut ranges = Vec::new();
    let mut depth = 0usize;
    let mut start = 0;
    let mut at = 0;
    for line in text.split_inclusive('\n') {
        let directive = line.trim_start().strip_prefix('#').map(str::trim_start).unwrap_or("");
        let word: String = directive.chars().take_while(char::is_ascii_alphabetic).collect();
        match word.as_str() {
            "if" | "ifdef" | "ifndef" => {
                if depth == 0 {
                    start = at;
                }
                depth += 1;
            }
            "endif" if depth > 0 => {
                depth -= 1;
                if depth == 0 {
                    ranges.push(start..at + line.len());
                }
            }
            _ => {}
        }
        at += line.len();
    }
    if depth > 0 {
        ranges.push(start..text.len());
    }
    ranges
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

fn english(target: DecompTarget) -> DecompTarget {
    match target.id {
        DecompTargetId::TbsJa => target_for(DecompTargetId::TbsEn),
        DecompTargetId::TlaJa => target_for(DecompTargetId::TlaEn),
        _ => target,
    }
}

struct Build {
    target: DecompTarget,
    imports: Option<tempfile::TempDir>,
    directory: tempfile::TempDir,
}

impl Build {
    /// The assembly and the compiler's messages for this text in place of the file.
    fn compile(&self, root: &Path, file: &Path, text: &str) -> Result<(String, String), String> {
        let name = file.file_name().and_then(|n| n.to_str()).unwrap_or("SOURCE.C");
        let source = self.directory.path().join(name);
        let assembly = self.directory.path().join("OUT.S");
        std::fs::write(&source, text).map_err(|e| e.to_string())?;
        let mut arguments = vec![format!("-I{}", file.parent().unwrap_or(root).display())];
        arguments.extend(cflags_for_target_source(self.target.compiler, name));
        arguments.push(format!("-D{}=1", self.target.edition_define));
        if let Some(imports) = &self.imports {
            arguments.push(format!("-I{}", imports.path().display()));
        }
        arguments.extend(["-S".into(), "-o".into(), assembly.to_string_lossy().into_owned()]);
        arguments.extend(["-x".into(), "c".into(), source.to_string_lossy().into_owned()]);
        let command = compiler_command_for_target(self.target.compiler, &arguments).map_err(|e| e.to_string())?;
        let output = Command::new(&command[0]).args(&command[1..]).current_dir(root).output().map_err(|e| e.to_string())?;
        let messages = String::from_utf8_lossy(&output.stderr).into_owned();
        if !output.status.success() {
            return Err(messages);
        }
        let text = std::fs::read_to_string(&assembly).map_err(|e| e.to_string())?;
        Ok((text, messages))
    }
}

/// The edits that leave the file's assembly and messages unchanged in every
/// game it builds in, applied, and how many there were.
pub(crate) fn proved(root: &Path, file: &Path, text: &str, games: &[DecompTarget], found: Vec<Edit>) -> Result<(String, usize), String> {
    let found = disjoint(found);
    if found.is_empty() {
        return Ok((text.to_string(), 0));
    }
    // An edit inside an #if block is proved by every edition of the game,
    // since one build compiles only one branch; otherwise English suffices.
    let blocks = conditional(text);
    let branched = found.iter().any(|e| blocks.iter().any(|b| b.start < e.end && e.start < b.end));
    let targets: Vec<DecompTarget> = games
        .iter()
        .flat_map(|&game| {
            if branched {
                TARGET_IDS.iter().map(|&id| target_for(id)).filter(|t| t.compiler == game.compiler).collect()
            } else {
                vec![english(game)]
            }
        })
        .collect();
    let mut builds = Vec::new();
    for target in targets {
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
    fn conditional_blocks_are_found_whole() {
        let text = "a;\n#if X\nb;\n#  ifdef Y\nc;\n#endif\n#else\nd;\n#endif\ne;\n";
        let ranges = conditional(text);
        assert_eq!(ranges.len(), 1);
        assert_eq!(&text[ranges[0].clone()], "#if X\nb;\n#  ifdef Y\nc;\n#endif\n#else\nd;\n#endif\n");
    }

    #[test]
    fn replacements_are_spaced_only_when_needed() {
        assert_eq!(apply("x=(y);", &[Edit { start: 2, end: 5, text: "y".into() }]), "x=y;");
        assert_eq!(apply("return(y);", &[Edit { start: 6, end: 9, text: "y".into() }]), "return y;");
    }
}
