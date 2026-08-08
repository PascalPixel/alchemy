//! Every primitive observation `selfTest` makes, recorded as a `key -> value`
//! pair so the differential harness can hand the keys to the live TypeScript
//! and byte-compare the values.
//!
//! WHY A TRACE AND NOT JUST A PASS/FAIL. `selfTest` is one function that either
//! throws or prints one line. Comparing that against the TypeScript is a
//! one-case comparison: it proves almost nothing, and it would still "pass" if
//! both sides had drifted in the same direction or if the assertions had been
//! silently weakened. What is actually worth comparing is the several hundred
//! primitive calls the assertions are made of -- every `cflagsForTargetSource`,
//! `usesAgbccCompiler`, `directCompilerCommand` and `sourceToAssemblyPlan` the
//! body performs, with its arguments and its result. Those are recorded here as
//! the body runs, so the harness corpus is *derived from the port* rather than
//! hand-listed next to it and cannot fall out of step with it.
//!
//! PORT NOTE -- the keys are tab-separated and the values use U+001F between
//! list items and U+001E between structure sections, matching the encoding the
//! sibling `alchemy-plan` harness already uses. No key or value component can
//! contain a tab: they are all repository paths, flag spellings and hex stems.

use std::fmt::Write as _;

use alchemy_plan::plan::{
    CompilerFamily, CompilerFlagMutations, SourceToAssemblyPlan, SourceToAssemblyPlanOptions,
};
use alchemy_routing::routing::CompilerTarget;

/// Separates list items inside a value.
pub const UNIT: char = '\u{1f}';
/// Separates structure sections inside a value.
pub const SECTION: char = '\u{1e}';

/// One recorded observation.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Record {
    pub key: String,
    pub value: String,
}

/// The observations made by one run of the ported body, in the order they were
/// made. `Vec` and not a map: order is the evidence that the body ran the way
/// the TypeScript runs, and duplicates are real -- `selfTest` asks the same
/// question more than once in several places, and the harness deduplicates
/// later rather than hiding it here.
#[derive(Debug, Clone, Default)]
pub struct Trace {
    pub records: Vec<Record>,
}

impl Trace {
    pub fn new() -> Self {
        Trace::default()
    }

    pub fn push(&mut self, key: String, value: String) {
        self.records.push(Record { key, value });
    }

    /// Distinct keys, in first-seen order.
    ///
    /// PORT NOTE: first-seen order, not sorted, and no `HashSet` anywhere --
    /// the harness feeds this list to a subprocess and zips the replies back
    /// positionally, so a reordering would silently compare the wrong pairs.
    pub fn distinct_keys(&self) -> Vec<String> {
        let mut seen: Vec<String> = Vec::new();
        for record in &self.records {
            if !seen.iter().any(|key| key == &record.key) {
                seen.push(record.key.clone());
            }
        }
        seen
    }

    /// The value recorded for `key` (the first one; a repeated key always
    /// carries the same value because the primitives are pure).
    pub fn value_of(&self, key: &str) -> Option<&str> {
        self.records
            .iter()
            .find(|record| record.key == key)
            .map(|record| record.value.as_str())
    }
}

/// `join(list, UNIT)`, used for flag lists and argv.
pub fn join_unit<S: AsRef<str>>(items: &[S]) -> String {
    let mut out = String::new();
    for (index, item) in items.iter().enumerate() {
        if index > 0 {
            out.push(UNIT);
        }
        out.push_str(item.as_ref());
    }
    out
}

/// Serialise a `Result` the way the harness compares it: `ok<TAB>payload` or
/// `err<TAB>message`.
///
/// PORT NOTE: the TypeScript side stringifies a thrown `Error` as
/// `"Error: <message>"`, so the harness compares only that the *message* is a
/// substring, never the prose of the wrapper. See `selftest_corpus.rs`.
pub fn outcome(result: &Result<Vec<String>, String>) -> String {
    match result {
        Ok(argv) => format!("ok\t{}", join_unit(argv)),
        Err(message) => format!("err\t{message}"),
    }
}

/// Serialise a plan into the same nine sections the sibling `alchemy-plan`
/// probe emits, so the two encodings stay readable side by side.
pub fn plan_payload(plan: &SourceToAssemblyPlan) -> String {
    let mut steps = String::new();
    for (index, step) in plan.steps.iter().enumerate() {
        if index > 0 {
            steps.push(SECTION);
        }
        let _ = write!(steps, "{}", step.kind.as_str());
        for argument in &step.command {
            steps.push(UNIT);
            steps.push_str(argument);
        }
    }
    [
        plan.target.as_str().to_string(),
        plan.requested_family.as_str().to_string(),
        plan.family.as_str().to_string(),
        plan.routing_source.clone(),
        plan.input.clone(),
        plan.output.clone(),
        plan.compiler_input.clone(),
        join_unit(&plan.flags),
        steps,
    ]
    .join(&SECTION.to_string())
}

/// The key for a `sourceToAssemblyPlan` call. Every optional field is present
/// in the key, spelled `-` when `undefined`, so two different option sets can
/// never collide onto one key.
pub fn plan_key(options: &SourceToAssemblyPlanOptions) -> String {
    let family = options
        .family
        .map(CompilerFamily::as_str)
        .unwrap_or("-")
        .to_string();
    let empty = CompilerFlagMutations::default();
    let flags = options.flags.as_ref().unwrap_or(&empty);
    format!(
        "PLAN\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
        options.target.as_str(),
        options.routing_source,
        options.input,
        options.output,
        family,
        dash_if_empty(&join_unit(&flags.add_flags)),
        dash_if_empty(&join_unit(&flags.remove_flags)),
        options.preprocessed_output.as_deref().unwrap_or("-"),
        options.dumpbase.as_deref().unwrap_or("-"),
    )
}

fn dash_if_empty(value: &str) -> String {
    if value.is_empty() {
        "-".to_string()
    } else {
        value.to_string()
    }
}

/// The key for a `cflagsForTargetSource` call.
pub fn cflags_key(target: CompilerTarget, source: &str) -> String {
    format!("CF\t{}\t{source}", target.as_str())
}

/// The key for a `usesAgbccCompiler` call.
pub fn uses_agbcc_key(target: CompilerTarget, source: &str) -> String {
    format!("UA\t{}\t{source}", target.as_str())
}

/// The key for a `directCompilerCommand` call.
pub fn direct_key(input: &str, output: &str, dumpbase: &str, source: Option<&str>) -> String {
    format!("DCC\t{input}\t{output}\t{dumpbase}\t{}", source.unwrap_or("-"))
}
