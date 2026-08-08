//! The insertion-ordered JSON value, re-exported.
//!
//! This is `candidate_compiler::json` and NOT a second copy. That module already holds
//! the strict parser, the `canonicalJson` writer and the ECMAScript
//! `ToString(Number)` this port needs, all over a `Vec<(String, Json)>` whose
//! ordering guarantee is the file format. A duplicate here is exactly the
//! hand-sync problem the port effort exists to end.
//!
//! NO `HashMap`, NO `serde_json`. `serde_json` sorts object keys unless
//! `preserve_order` is enabled, and its float writer does not match
//! JavaScript's.

pub use candidate_compiler::json::{canonical_json, parse, Json};

/// Build an object literal, preserving the order the keys are written in.
pub fn object(entries: Vec<(&str, Json)>) -> Json {
    Json::Object(
        entries
            .into_iter()
            .map(|(key, value)| (key.to_string(), value))
            .collect(),
    )
}

/// `Json::Number` from a `usize` length or count.
pub fn number(value: usize) -> Json {
    Json::Number(value as f64)
}

pub fn string(value: &str) -> Json {
    Json::String(value.to_string())
}

pub fn strings(values: &[String]) -> Json {
    Json::Array(values.iter().map(|v| Json::String(v.clone())).collect())
}
