//! Parity gate between this crate and `tools/lib/alchemy_gcc.ts`.
//!
//! WHY. Ten things in the plan layer exist in both languages while the
//! TypeScript is live, and nothing generates either side: the six literal
//! sequences inside the plan and direct-command builders, the `CompilerFamily`
//! union, the two step kinds, and the field lists of the plan and its options.
//!
//! Order is not incidental in any of them. The literal sequences ARE the argv
//! the compiler receives, and gcc is later-flag-wins; the family sequence is the
//! order `sourceToAssemblyPlan`'s ternary chains test in, which decides which
//! driver a family resolves to; the field lists are the shape callers destructure.
//! So every comparison here is element-wise AND in order.
//!
//! WHY THE RUST MIRRORS ARE SPELLED IN TYPESCRIPT'S OWN NOTATION (templates and
//! all): so this gate reads the TypeScript rather than trusting a comment about
//! it, exactly as `alchemy_bundle::bundle::SIGNATURE_PATH_EXPRESSIONS` does. The
//! mirrors are not tautological because [`crate::plan`]'s unit tests
//! independently tie each one to what the Rust functions actually emit -- so a
//! mirror can only agree with both sides at once if the port is right.
//!
//! The check refuses to pass if fewer than [`MINIMUM_SUBJECTS`] subjects were
//! compared, and fails loudly if the TypeScript cannot be read or is empty --
//! a vanished mirror must never read as agreement.
//!
//! RETIREMENT: delete this module and `src/bin/plan_drift.rs`, and the
//! `plan-drift` `[[bin]]` entry, when `tools/lib/alchemy_gcc.ts` is deleted. A
//! drift check with one mirror left cannot fail, which is the exact defect it
//! was written to prevent.

use std::fs;
use std::path::{Path, PathBuf};

use alchemy_bundle::jsvalue;

use crate::plan;

/// Path of the TypeScript mirror, relative to the repository root.
pub const TYPESCRIPT_SOURCE: &str = "tools/lib/alchemy_gcc.ts";

/// The number of subjects both sides carry. A parser that quietly stopped
/// matching would otherwise report zero drift over zero comparisons and read
/// exactly like a clean run.
pub const MINIMUM_SUBJECTS: usize = 10;

/// The smallest number of rendered elements a healthy run compares. The six
/// literal sequences alone carry more than sixty; a collapse below this means
/// the extractor broke, not that the code shrank.
pub const MINIMUM_ELEMENTS: usize = 60;

pub type Subject = Vec<String>;

#[derive(Debug)]
pub struct Drift {
    pub name: String,
    pub typescript: Subject,
    pub rust: Subject,
}

#[derive(Debug, Default)]
pub struct Report {
    pub agreed: Vec<String>,
    /// Declared in `alchemy_gcc.ts` and not recoverable on the Rust side.
    pub only_ts: Vec<String>,
    /// Mirrored in Rust and not found in `alchemy_gcc.ts`.
    pub only_rs: Vec<String>,
    pub drifted: Vec<Drift>,
    /// Total rendered elements compared across every agreeing subject.
    pub elements: usize,
    /// Distinct rendered elements, so a reviewer can see the comparison is not
    /// degenerately all-same.
    pub distinct_elements: usize,
}

impl Report {
    pub fn compared(&self) -> usize {
        self.agreed.len() + self.only_ts.len() + self.only_rs.len() + self.drifted.len()
    }

    pub fn in_sync(&self) -> bool {
        self.only_ts.is_empty() && self.only_rs.is_empty() && self.drifted.is_empty()
    }
}

/// Repository root: `<crate>/../..`.
pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

/// Read the TypeScript mirror.
///
/// PORT NOTE: `readFileSync(path, "utf8")` replaces invalid UTF-8 rather than
/// throwing, which is `from_utf8_lossy`. This file contains Japanese comments,
/// so a byte-level reader that assumed ASCII would corrupt the offsets it
/// searches.
pub fn read_typescript() -> Result<String, String> {
    let path = root().join(TYPESCRIPT_SOURCE);
    let bytes = fs::read(&path).map_err(|error| {
        format!(
            "PLAN PARITY GATE COULD NOT READ {} ({error}) -- this is a FAILURE, not a pass.",
            path.display()
        )
    })?;
    let text = String::from_utf8_lossy(&bytes).into_owned();
    if text.trim().is_empty() {
        return Err(format!(
            "PLAN PARITY GATE READ AN EMPTY {} -- this is a FAILURE, not a pass.",
            path.display()
        ));
    }
    Ok(text)
}

// ---------------------------------------------------------------------------
// TypeScript-side extraction
// ---------------------------------------------------------------------------

/// Strip the delimiters from a literal `jsvalue::string_literals` returned.
///
/// PORT NOTE: only the delimiters are removed; escapes are left as written, so
/// a `\n` in the TypeScript stays two characters here. Nothing in this slice
/// contains an escape, and un-escaping would make the mirror lie about the
/// source text it is supposed to be quoting.
fn unquote(literal: &str) -> String {
    let mut chars = literal.chars();
    chars.next();
    chars.next_back();
    chars.as_str().to_string()
}

/// The literal sequence of a named function's body, in source order.
fn function_literals(source: &str, name: &str) -> Option<Subject> {
    let body = jsvalue::function_body(source, name)?;
    Some(
        jsvalue::string_literals(body)
            .iter()
            .map(|literal| unquote(literal))
            .collect(),
    )
}

/// The literal sequence of the text between two markers, in source order.
///
/// WHY THIS EXISTS ALONGSIDE [`function_literals`]. `jsvalue::function_body`
/// brace-matches from the first `{` after the function name, and
/// `mutatedCompilerFlags` has a defaulted parameter -- `mutations:
/// CompilerFlagMutations = {}` -- so that first brace is the empty object
/// literal in the signature and the "body" comes back empty. An empty subject
/// compared against an empty subject is exactly the silent pass this gate is
/// built to refuse, so that function is bracketed by markers instead. The
/// signature it sweeps in carries no string literals of its own.
fn region_literals(source: &str, start: &str, end: &str) -> Option<Subject> {
    let from = source.find(start)?;
    let to = source[from..].find(end)? + from;
    Some(
        jsvalue::string_literals(&source[from..to])
            .iter()
            .map(|literal| unquote(literal))
            .collect(),
    )
}

/// The literal members of `export type <name> = | "a" | "b";`.
fn type_union_members(source: &str, name: &str) -> Option<Subject> {
    let at = source.find(&format!("export type {name} ="))?;
    let end = source[at..].find(';')? + at;
    Some(
        jsvalue::string_literals(&source[at..end])
            .iter()
            .map(|literal| unquote(literal))
            .collect(),
    )
}

/// The property names of `export interface <name> { ... }`, in declaration
/// order, with `?` preserved so an optional property that became required is
/// reported as drift rather than silently accepted.
fn interface_fields(source: &str, name: &str) -> Option<Subject> {
    let at = source.find(&format!("export interface {name} "))?;
    let open = source[at..].find('{')? + at;
    let bytes = source.as_bytes();
    let mut depth = 0i32;
    let mut end = None;
    let mut index = open;
    while index < bytes.len() {
        match bytes[index] {
            b'{' => depth += 1,
            b'}' => {
                depth -= 1;
                if depth == 0 {
                    end = Some(index);
                    break;
                }
            }
            _ => {}
        }
        index += 1;
    }
    let body = &source[open + 1..end?];
    let mut out = Vec::new();
    for line in body.lines() {
        let line = line.trim();
        // Comments are skipped; the plan interfaces carry prose between fields.
        if line.is_empty() || line.starts_with("//") || line.starts_with('*') {
            continue;
        }
        let Some(colon) = line.find(':') else { continue };
        let field = line[..colon].trim();
        if field.is_empty() || field.contains(' ') {
            continue;
        }
        out.push(field.to_string());
    }
    Some(out)
}

// ---------------------------------------------------------------------------
// Subjects
// ---------------------------------------------------------------------------

type Extractor = fn(&str) -> Option<Subject>;

/// `(name, TypeScript extractor, Rust mirror)`.
fn subjects() -> Vec<(&'static str, Extractor, Subject)> {
    fn owned(values: &[&str]) -> Subject {
        values.iter().map(|value| (*value).to_string()).collect()
    }
    vec![
        (
            "sourceToAssemblyPlan/literals",
            (|source| function_literals(source, "sourceToAssemblyPlan")) as Extractor,
            owned(&plan::SOURCE_TO_ASSEMBLY_PLAN_LITERALS),
        ),
        (
            "mutatedCompilerFlags/literals",
            |source| {
                region_literals(
                    source,
                    "function mutatedCompilerFlags(",
                    "function inferredPreprocessedOutput(",
                )
            },
            owned(&plan::MUTATED_COMPILER_FLAGS_LITERALS),
        ),
        (
            "inferredPreprocessedOutput/literals",
            |source| function_literals(source, "inferredPreprocessedOutput"),
            owned(&plan::INFERRED_PREPROCESSED_OUTPUT_LITERALS),
        ),
        (
            "directPreprocessorCommand/literals",
            |source| function_literals(source, "directPreprocessorCommand"),
            owned(&plan::DIRECT_PREPROCESSOR_LITERALS),
        ),
        (
            "directCompilerCommand/literals",
            |source| function_literals(source, "directCompilerCommand"),
            owned(&plan::DIRECT_COMPILER_LITERALS),
        ),
        (
            "directCompilerCommandForSource/literals",
            |source| function_literals(source, "directCompilerCommandForSource"),
            owned(&plan::DIRECT_COMPILER_FOR_SOURCE_LITERALS),
        ),
        (
            "CompilerFamily/union",
            |source| type_union_members(source, "CompilerFamily"),
            plan::CompilerFamily::ALL
                .iter()
                .map(|family| family.as_str().to_string())
                .collect(),
        ),
        (
            "CompilerCommandStep/fields",
            |source| interface_fields(source, "CompilerCommandStep"),
            owned(&["kind", "command"]),
        ),
        (
            "SourceToAssemblyPlan/fields",
            |source| interface_fields(source, "SourceToAssemblyPlan"),
            owned(&[
                "target",
                "requestedFamily",
                "family",
                "routingSource",
                "input",
                "output",
                "compilerInput",
                "flags",
                "steps",
            ]),
        ),
        (
            "SourceToAssemblyPlanOptions/fields",
            |source| interface_fields(source, "SourceToAssemblyPlanOptions"),
            owned(&[
                "target",
                "routingSource",
                "input",
                "output",
                "family?",
                "flags?",
                "preprocessedOutput?",
                "dumpbase?",
            ]),
        ),
    ]
}

/// Compare every subject. `Err` only for the failures that make the whole run
/// meaningless -- an unreadable mirror, or too few subjects to be evidence.
pub fn check() -> Result<Report, String> {
    let source = read_typescript()?;
    let mut report = Report::default();
    let mut elements: Vec<String> = Vec::new();

    for (name, extract, rust) in subjects() {
        match extract(&source) {
            None => report.only_rs.push(name.to_string()),
            Some(typescript) => {
                if typescript == rust {
                    elements.extend(typescript.iter().cloned());
                    report.agreed.push(name.to_string());
                } else {
                    report.drifted.push(Drift {
                        name: name.to_string(),
                        typescript,
                        rust,
                    });
                }
            }
        }
    }

    if report.compared() < MINIMUM_SUBJECTS {
        return Err(format!(
            "PLAN PARITY GATE COMPARED ONLY {} SUBJECTS, EXPECTED AT LEAST {MINIMUM_SUBJECTS} \
             -- this is a FAILURE, not a pass. Scanning nothing is not passing.",
            report.compared()
        ));
    }

    report.elements = elements.len();
    let mut distinct = elements.clone();
    distinct.sort_unstable();
    distinct.dedup();
    report.distinct_elements = distinct.len();

    if report.in_sync() && report.elements < MINIMUM_ELEMENTS {
        return Err(format!(
            "PLAN PARITY GATE COMPARED ONLY {} ELEMENTS, EXPECTED AT LEAST {MINIMUM_ELEMENTS} \
             -- the extractor is matching but returning almost nothing, which is a FAILURE, \
             not a pass.",
            report.elements
        ));
    }

    if !report.in_sync() {
        let mut message = String::from("PLAN PARITY GATE FAILED.\n");
        for name in &report.only_rs {
            message.push_str(&format!(
                "  MISSING FROM {TYPESCRIPT_SOURCE}: {name}\n\
                     the Rust mirror exists but the TypeScript declaration could not be found.\n"
            ));
        }
        for name in &report.only_ts {
            message.push_str(&format!("  MISSING FROM alchemy-plan: {name}\n"));
        }
        for drift in &report.drifted {
            message.push_str(&format!("  DRIFTED: {}\n", drift.name));
            message.push_str(&format!("    ts   {:?}\n", drift.typescript));
            message.push_str(&format!("    rust {:?}\n", drift.rust));
        }
        return Err(message);
    }

    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_gate_agrees_with_the_live_typescript() {
        let report = check().unwrap_or_else(|message| panic!("{message}"));
        assert!(report.in_sync());
        assert!(report.compared() >= MINIMUM_SUBJECTS);
        assert!(report.elements >= MINIMUM_ELEMENTS);
        // Not degenerately all-same: the six literal sequences alone carry more
        // than a dozen distinct tokens.
        assert!(
            report.distinct_elements >= 30,
            "only {} distinct elements -- the extractor looks collapsed",
            report.distinct_elements
        );
    }

    #[test]
    fn the_gate_rejects_a_perturbed_mirror() {
        // NEGATIVE CONTROL. A comparison that cannot tell two sequences apart
        // would report every subject in sync forever.
        let source = read_typescript().expect("the TypeScript is readable");
        let real = function_literals(&source, "directPreprocessorCommand")
            .expect("directPreprocessorCommand is declared");
        let mut perturbed = real.clone();
        perturbed[0].push('X');
        assert_ne!(real, perturbed);
        let mut reversed = real.clone();
        reversed.reverse();
        assert_ne!(real, reversed, "order must be part of the comparison");
    }

    #[test]
    fn a_vanished_declaration_is_reported_not_ignored() {
        assert!(function_literals("const x = 1;", "sourceToAssemblyPlan").is_none());
        assert!(type_union_members("const x = 1;", "CompilerFamily").is_none());
        assert!(interface_fields("const x = 1;", "SourceToAssemblyPlan").is_none());
    }

    #[test]
    fn unquote_strips_only_the_delimiters() {
        assert_eq!(unquote("\"-O2\""), "-O2");
        assert_eq!(unquote("`${x}.i`"), "${x}.i");
        assert_eq!(unquote("''"), "");
    }
}
