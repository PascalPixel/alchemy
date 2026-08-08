//! Forbid passing a multi-parameter function to an array method that supplies
//! an index.
//!
//! WHY THIS EXISTS. `externalSymbolAssembly` grew a second parameter when
//! overlays stopped sharing one `call_via` bank. Every `names.map(externalSymbolAssembly)`
//! in the tree kept compiling and started feeding the ARRAY INDEX in as
//! `callViaBase`: `_call_via_r3` resolved to `0x0000000c` instead of
//! `0x080072f0`, the assembler could not reach it with a Thumb `bl`, each
//! affected main-image object grew a long-branch veneer that ran past its
//! claimed span, and `build:claimed` failed to link with fifteen section
//! overlaps while nothing in the C had changed. Seven tools had written the
//! emitter as a bare callback and all seven were wrong.
//!
//! WHY IT IS REBUILT. The scan added to catch those seven scanned NOTHING. It
//! resolved its directory with `dirname(dirname(dirname(...)))` from
//! `tools/lib/alchemy_gcc.ts`, which is the repository root, called the result
//! `toolsDirectory`, read it FLAT, and kept the `.ts` files. There are no `.ts`
//! files in the repository root, so the predicate never ran once. Even pointed
//! at `tools/` the flat read would have missed `tools/lib/` and `tools/check/`,
//! which is where every tool actually lives.
//!
//! It was also hardcoded to two function names. The bug is not about
//! `externalSymbolAssembly`; it is about arity. So this version DERIVES the set
//! of dangerous callees by reading the declarations in the tree and taking
//! every function with two or more parameters. A function that grows a second
//! parameter tomorrow is covered the moment it grows it, which is the only
//! version of this lint that would have caught the original incident on the
//! commit that caused it.

use std::collections::BTreeMap;
use std::fs;

use crate::{blank_literals, is_word_byte, line_of, Finding, Report};

/// Array methods that pass `(element, index, array)` to their callback. A bare
/// reference here is the trap: the extra arguments are silently accepted.
///
/// `reduce` is deliberately absent -- its callback's second argument is the
/// element, not an index, so the failure shape is different and this lint would
/// be guessing.
pub const INDEXING_METHODS: [&str; 10] = [
    "map",
    "forEach",
    "filter",
    "flatMap",
    "some",
    "every",
    "find",
    "findIndex",
    "findLast",
    "findLastIndex",
];

/// Floors. Far below the real tree so an ordinary edit never trips them; a
/// scanner that goes blind always does. The 2026 tree reads ~145 files, indexes
/// ~1900 functions, and examines ~900 callback sites.
const FILE_FLOOR: usize = 40;
const FUNCTION_FLOOR: usize = 200;
const MULTI_PARAMETER_FLOOR: usize = 50;
const SITE_FLOOR: usize = 100;
const BARE_REFERENCE_FLOOR: usize = 1;

/// Parameter count of a declared function, or `None` if the text at `open` is
/// not a parameter list this can read.
fn parameter_count(text: &str, open: usize) -> Option<usize> {
    let end = crate::balanced_end(text, open)?;
    let inside = text[open + 1..end].trim();
    if inside.is_empty() {
        return Some(0);
    }
    let bytes = inside.as_bytes();
    let mut depth = 0usize;
    let mut count = 1usize;
    let mut index = 0;
    let mut quote: Option<u8> = None;
    while index < bytes.len() {
        let byte = bytes[index];
        if let Some(mark) = quote {
            if byte == b'\\' {
                index += 2;
                continue;
            }
            if byte == mark {
                quote = None;
            }
            index += 1;
            continue;
        }
        match byte {
            b'"' | b'\'' | b'`' => quote = Some(byte),
            b'(' | b'[' | b'{' | b'<' => depth += 1,
            b')' | b']' | b'}' | b'>' => depth = depth.saturating_sub(1),
            b',' if depth == 0 => count += 1,
            _ => {}
        }
        index += 1;
    }
    Some(count)
}

/// Name and parameter count of every `function NAME(...)` in comment-blanked
/// text. Arrow constants are covered too, because `const f = (a, b) => ...` is
/// just as passable by reference.
pub fn declared_functions(blanked: &str) -> Vec<(String, usize)> {
    let bytes = blanked.as_bytes();
    let mut found = Vec::new();

    let mut index = 0;
    while let Some(hit) = blanked[index..].find("function") {
        let start = index + hit;
        index = start + 8;
        // A real keyword, not the tail of `myfunction`.
        if start > 0 && is_word_byte(bytes[start - 1]) {
            continue;
        }
        let mut at = index;
        while at < bytes.len() && bytes[at].is_ascii_whitespace() {
            at += 1;
        }
        if bytes.get(at) == Some(&b'*') {
            at += 1;
            while at < bytes.len() && bytes[at].is_ascii_whitespace() {
                at += 1;
            }
        }
        let name_start = at;
        while at < bytes.len() && is_word_byte(bytes[at]) {
            at += 1;
        }
        if at == name_start {
            continue;
        }
        let name = blanked[name_start..at].to_string();
        // A generic signature may sit between the name and the parameters.
        while at < bytes.len() && bytes[at] != b'(' && bytes[at] != b'\n' {
            at += 1;
        }
        if bytes.get(at) != Some(&b'(') {
            continue;
        }
        if let Some(count) = parameter_count(blanked, at) {
            found.push((name, count));
        }
    }

    // `const NAME = (a, b): T => ...` and `const NAME = async (a, b) => ...`.
    for keyword in ["const ", "let ", "var "] {
        let mut index = 0;
        while let Some(hit) = blanked[index..].find(keyword) {
            let start = index + hit;
            index = start + keyword.len();
            if start > 0 && is_word_byte(bytes[start - 1]) {
                continue;
            }
            let mut at = index;
            let name_start = at;
            while at < bytes.len() && is_word_byte(bytes[at]) {
                at += 1;
            }
            if at == name_start {
                continue;
            }
            let name = blanked[name_start..at].to_string();
            while at < bytes.len() && bytes[at] != b'=' && bytes[at] != b'\n' {
                at += 1;
            }
            if bytes.get(at) != Some(&b'=') || bytes.get(at + 1) == Some(&b'=') {
                continue;
            }
            at += 1;
            while at < bytes.len() && bytes[at].is_ascii_whitespace() {
                at += 1;
            }
            if blanked[at..].starts_with("async") {
                at += 5;
                while at < bytes.len() && bytes[at].is_ascii_whitespace() {
                    at += 1;
                }
            }
            if bytes.get(at) != Some(&b'(') {
                continue;
            }
            let Some(close) = crate::balanced_end(blanked, at) else { continue };
            // Only an arrow makes this a function; `const x = (a, b)` is a
            // comma expression and `const x = (y)` is a parenthesised value.
            let tail = blanked[close + 1..].trim_start();
            let arrow = tail.starts_with("=>")
                || (tail.starts_with(':') && tail.contains("=>") && {
                    let head = &tail[..tail.find("=>").unwrap_or(0)];
                    !head.contains('\n') || head.trim_end().ends_with('|')
                });
            if !arrow {
                continue;
            }
            if let Some(count) = parameter_count(blanked, at) {
                found.push((name, count));
            }
        }
    }
    found
}

/// Every local binding introduced by an `import` statement.
///
/// WHY THIS IS NEEDED. The first version of this lint resolved a callee's arity
/// against a single tree-wide name table, and that produced a wall of false
/// alarms: `tools/make/build_assets.ts` declares a ONE-parameter `sourcePath`,
/// but `tools/make/staff_roll.ts` and `tools/make/resource_01c.ts` each declare
/// a TWO-parameter `sourcePath`, so all seventeen `nested.forEach(sourcePath)`
/// sites in `build_assets.ts` were reported against a function they never call.
/// `hexadecimal` collides across ten files the same way. A lint that reports
/// seventeen non-bugs to find one real one is a lint people turn off, so
/// resolution is now: the declaration in THIS file wins (it shadows), otherwise
/// the name must actually be imported here, otherwise the site is unresolved and
/// counted rather than accused.
///
/// `import X from`, `import * as X from` and `import { a, b as c } from` all
/// bind a local name; the `as` alias is the one that matters.
pub fn imported_names(blanked: &str) -> Vec<String> {
    let mut names = Vec::new();
    let mut index = 0;
    while let Some(hit) = blanked[index..].find("import ") {
        let start = index + hit;
        index = start + 7;
        if start > 0 && is_word_byte(blanked.as_bytes()[start - 1]) {
            continue;
        }
        let Some(from) = blanked[index..].find(" from ") else { continue };
        let clause = &blanked[index..index + from];
        for piece in clause.split([',', '{', '}']) {
            let piece = piece.trim();
            if piece.is_empty() {
                continue;
            }
            let binding = match piece.rsplit_once(" as ") {
                Some((_, alias)) => alias.trim(),
                None => piece,
            };
            let binding = binding.trim_start_matches('*').trim();
            if !binding.is_empty() && binding.bytes().all(is_word_byte) && binding != "type" {
                names.push(binding.to_string());
            }
        }
    }
    names.sort();
    names.dedup();
    names
}

/// A `.method(identifier)` or `.method(identifier, ...)` site: the callback is
/// passed BY REFERENCE, so every argument the method supplies reaches it.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BareReference {
    pub method: String,
    pub callee: String,
    pub offset: usize,
}

pub fn bare_reference_sites(blanked: &str) -> (usize, Vec<BareReference>) {
    let bytes = blanked.as_bytes();
    let mut examined = 0usize;
    let mut sites = Vec::new();
    let mut index = 0;
    while index < bytes.len() {
        if bytes[index] != b'.' {
            index += 1;
            continue;
        }
        let name_start = index + 1;
        let mut at = name_start;
        while at < bytes.len() && is_word_byte(bytes[at]) {
            at += 1;
        }
        let method = &blanked[name_start..at];
        if !INDEXING_METHODS.contains(&method) {
            index = at.max(index + 1);
            continue;
        }
        while at < bytes.len() && bytes[at].is_ascii_whitespace() {
            at += 1;
        }
        if bytes.get(at) != Some(&b'(') {
            index = at.max(index + 1);
            continue;
        }
        examined += 1;
        let mut inner = at + 1;
        while inner < bytes.len() && bytes[inner].is_ascii_whitespace() {
            inner += 1;
        }
        let callee_start = inner;
        while inner < bytes.len() && is_word_byte(bytes[inner]) {
            inner += 1;
        }
        let callee = &blanked[callee_start..inner];
        let mut after = inner;
        while after < bytes.len() && bytes[after].is_ascii_whitespace() {
            after += 1;
        }
        // A bare reference ends right there. `f(x)`, `f.bind(...)`, `x => ...`
        // and `(x) => ...` all continue with something else, and all of them
        // control their own arity.
        let bare = !callee.is_empty()
            && matches!(bytes.get(after), Some(b')') | Some(b','))
            && !callee.bytes().next().is_some_and(|byte| byte.is_ascii_digit());
        if bare {
            sites.push(BareReference {
                method: method.to_string(),
                callee: callee.to_string(),
                offset: callee_start,
            });
        }
        index = at + 1;
    }
    (examined, sites)
}

/// Scan the TypeScript tooling for arity leaks.
///
/// `sources` is a list of (label, text) pairs so the tests can drive it without
/// a tree and the binary can drive it with one.
pub fn analyse(sources: &[(String, String)]) -> Report {
    let mut report = Report::new("callback-arity");

    // Two passes: the whole corpus is indexed before anything is judged,
    // because the declaration and the leak are usually in different files --
    // that is what made the original bug survive review.
    let mut blanked_sources: Vec<(String, String)> = Vec::new();
    let mut arities: BTreeMap<String, usize> = BTreeMap::new();
    let mut local: Vec<BTreeMap<String, usize>> = Vec::new();
    for (label, text) in sources {
        let blanked = blank_literals(text);
        let mut here: BTreeMap<String, usize> = BTreeMap::new();
        for (name, count) in declared_functions(&blanked) {
            // Widest declaration wins: an overload or a re-export that takes
            // more arguments is still reachable through the same identifier.
            let entry = arities.entry(name.clone()).or_insert(count);
            *entry = (*entry).max(count);
            let entry = here.entry(name).or_insert(count);
            *entry = (*entry).max(count);
        }
        local.push(here);
        blanked_sources.push((label.clone(), blanked));
    }
    let multi_parameter = arities.values().filter(|count| **count >= 2).count();

    let mut examined = 0usize;
    let mut bare = 0usize;
    let mut unresolved = 0usize;
    let mut findings: Vec<Finding> = Vec::new();
    for (position, (label, blanked)) in blanked_sources.iter().enumerate() {
        let (sites, references) = bare_reference_sites(blanked);
        examined += sites;
        bare += references.len();
        let here = &local[position];
        let imported = imported_names(blanked);
        for reference in references {
            // Shadowing order, not a global name soup. See `imported_names`.
            let resolved = here.get(&reference.callee).copied().or_else(|| {
                imported
                    .contains(&reference.callee)
                    .then(|| arities.get(&reference.callee).copied())
                    .flatten()
            });
            let Some(count) = resolved else {
                if arities.contains_key(&reference.callee) {
                    // A same-named function exists somewhere but this file
                    // neither declares nor imports it; accusing it would be the
                    // `sourcePath` false alarm all over again.
                    unresolved += 1;
                }
                continue;
            };
            if count < 2 {
                continue;
            }
            findings.push(Finding {
                file: label.clone(),
                line: line_of(blanked, reference.offset),
                message: format!(
                    ".{}({}) passes a {}-parameter function by reference, so the array \
                     index arrives as its second argument",
                    reference.method, reference.callee, count
                ),
            });
        }
    }

    report.count_with_floor("files", sources.len(), FILE_FLOOR);
    report.count_with_floor("functions_indexed", arities.len(), FUNCTION_FLOOR);
    report.count_with_floor("multi_parameter_functions", multi_parameter, MULTI_PARAMETER_FLOOR);
    report.count_with_floor("callback_sites_examined", examined, SITE_FLOOR);
    // The detector that finds nothing because it is broken and the detector
    // that finds nothing because the tree is clean look identical from the
    // outside. The tree always contains SOME bare callback reference
    // (`.map(Number)` and friends), so requiring one proves the detector runs.
    report.count_with_floor("bare_references_seen", bare, BARE_REFERENCE_FLOOR);
    // Reported, not hidden: these are the sites where a same-named function
    // exists elsewhere in the tree and this file neither declares nor imports
    // it. If this number ever dwarfs `leaks`, the resolver is the thing to fix.
    report.count("unresolved_same_name_references", unresolved);
    report.count("leaks", findings.len());
    report.findings = findings;
    report
}

/// Read the TypeScript tooling. Note the directories: `tools`, RECURSIVELY.
pub fn read_tree(root: &std::path::Path) -> Result<Vec<(String, String)>, String> {
    let mut sources = Vec::new();
    for directory in ["tools"] {
        let base = root.join(directory);
        let names = crate::files_with_suffix(&base, ".ts")
            .map_err(|error| format!("{directory}: {error}"))?;
        for name in names {
            let text = fs::read_to_string(base.join(&name))
                .map_err(|error| format!("{directory}/{name}: {error}"))?;
            sources.push((format!("{directory}/{name}"), text));
        }
    }
    Ok(sources)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn corpus(text: &str) -> Vec<(String, String)> {
        vec![("t.ts".to_string(), text.to_string())]
    }

    #[test]
    fn the_incident_shape_is_caught() {
        let text = "export function externalSymbolAssembly(name: string, callViaBase = 0) {}\n\
                    const out = names.map(externalSymbolAssembly);\n";
        let report = analyse(&corpus(text));
        assert_eq!(report.findings.len(), 1);
        assert_eq!(report.findings[0].line, 2);
    }

    #[test]
    fn a_wrapping_arrow_is_not_a_leak() {
        let text = "export function externalSymbolAssembly(name: string, callViaBase = 0) {}\n\
                    const out = names.map((name) => externalSymbolAssembly(name));\n";
        assert!(analyse(&corpus(text)).findings.is_empty());
    }

    #[test]
    fn a_single_parameter_callback_is_not_a_leak() {
        let text = "function hex(value: number) { return value; }\n\
                    const out = values.map(hex);\n";
        assert!(analyse(&corpus(text)).findings.is_empty());
    }

    #[test]
    fn the_leak_is_found_across_files() {
        // The original incident: the declaration moved in one file, the callers
        // were in seven others. A per-file lint sees neither half.
        let sources = vec![
            (
                "lib.ts".to_string(),
                "export function externalSymbol(name: string, base: number) {}\n".to_string(),
            ),
            (
                "tool.ts".to_string(),
                "import { externalSymbol } from \"./lib.ts\";\nnames.forEach(externalSymbol);\n"
                    .to_string(),
            ),
        ];
        let report = analyse(&sources);
        assert_eq!(report.findings.len(), 1);
        assert_eq!(report.findings[0].file, "tool.ts");
    }

    #[test]
    fn a_same_name_function_in_an_unrelated_file_is_not_a_leak() {
        // The `sourcePath` false alarm: build_assets.ts declares a one-parameter
        // `sourcePath` and staff_roll.ts declares a two-parameter one. The call
        // site belongs to the local declaration and nothing else.
        let sources = vec![
            ("other.ts".to_string(), "function sourcePath(a: string, b: string) {}\n".to_string()),
            (
                "here.ts".to_string(),
                "function sourcePath(name: string) {}\nnested.forEach(sourcePath);\n".to_string(),
            ),
        ];
        let report = analyse(&sources);
        assert!(report.findings.is_empty(), "{:?}", report.findings);
    }

    #[test]
    fn an_unimported_same_name_reference_is_counted_and_not_accused() {
        let sources = vec![
            ("other.ts".to_string(), "function id(a: string, b: string) {}\n".to_string()),
            ("here.ts".to_string(), "names.map(id);\n".to_string()),
        ];
        let report = analyse(&sources);
        assert!(report.findings.is_empty());
        assert_eq!(report.counts.iter().find(|(name, _)| *name
            == "unresolved_same_name_references").map(|(_, value)| *value), Some(1));
    }

    #[test]
    fn every_indexing_method_is_covered() {
        // A leak through `.filter` is as wrong as one through `.map`, and the
        // original only ever looked at `.map`.
        for method in INDEXING_METHODS {
            let text = format!(
                "function emit(a: string, b: number) {{}}\nconst x = names.{method}(emit);\n"
            );
            assert_eq!(analyse(&corpus(&text)).findings.len(), 1, "{method} must be covered");
        }
    }

    #[test]
    fn a_reference_inside_a_comment_or_a_string_is_not_a_leak() {
        let text = "function emit(a: string, b: number) {}\n\
                    // names.map(emit) is the trap this documents\n\
                    const help = \"names.map(emit)\";\n";
        assert!(analyse(&corpus(text)).findings.is_empty());
    }

    #[test]
    fn arrow_constants_count_as_declarations() {
        let text = "const emit = (a: string, b: number): string => a;\n\
                    const x = names.map(emit);\n";
        assert_eq!(analyse(&corpus(text)).findings.len(), 1);
    }

    #[test]
    fn an_empty_corpus_is_a_failure_and_not_a_pass() {
        // THE DEFECT ITSELF. The TypeScript scanned an empty file list and
        // returned quietly.
        let report = analyse(&[]);
        assert!(report.findings.is_empty(), "nothing to find");
        assert!(!report.ok(), "but scanning nothing is not passing");
        assert!(report.blind.iter().any(|reason| reason.contains("files=0")));
    }
}
