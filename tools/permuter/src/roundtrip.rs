//! Parse-and-reprint round trip over `lang-c`.
//!
//! The round trip is the verification gate for the AST layer: a preprocessed
//! source is parsed, re-emitted by `cemit`, and the caller compiles both
//! sides and requires identical assembly. Anything the printer cannot
//! express shows up as a compile failure or an assembly difference.

use lang_c::driver::{parse_preprocessed, Config, Flavor};

use crate::cemit::emit_translation_unit;

/// Parses an already-preprocessed source and returns the re-emitted C, or
/// the parse error.
pub fn roundtrip(src: &str) -> Result<String, String> {
    let mut config = Config::default();
    config.flavor = Flavor::GnuC11;
    config.cpp_command = String::new();
    let parsed = parse_preprocessed(&config, src.to_string())
        .map_err(|error| format!("parse failed: {error}"))?;
    Ok(emit_translation_unit(&parsed.unit))
}

pub fn self_test() -> Result<(), String> {
    let emitted = roundtrip("int g; int f(void) { return g; }")?;
    if !emitted.contains("int g;") {
        return Err("roundtrip self-test: declaration lost".to_string());
    }
    Ok(())
}
