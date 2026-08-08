//! Native compiler lint checks that `selfTest` runs before it touches routing.
//!
//! The repository-wide `alchemy-lints` crate owns the full checks. This module
//! keeps the small pre-routing assertions that belong to compiler self-test and
//! delegates the native stem-collision check to that crate.

use alchemy_lints::{repository_root, stem_collision};
use alchemy_symbols::symbols::{external_symbol_assembly, CALL_VIA_BASE};

/// `callbackArityLint()`.
pub fn callback_arity_lint() -> Result<(), String> {
    let direct = external_symbol_assembly("_call_via_r3", CALL_VIA_BASE)?;
    if !direct.contains("0x080072f0") {
        return Err(format!(
            "call_via default base self-test failed: {}",
            direct.trim()
        ));
    }
    if !external_symbol_assembly("_call_via_r3", 0x0200_61b4)?.contains("0x020061c0") {
        return Err("call_via explicit base self-test failed".to_string());
    }

    overlay_stem_collision_lint()
}

/// `overlayStemCollisionLint()`, delegated to the sibling crate that already
/// owns it. Only the failure *shape* is asserted here, exactly as `selfTest`
/// does: any collision throws, none passes silently.
pub fn overlay_stem_collision_lint() -> Result<(), String> {
    let root = repository_root();
    let names = stem_collision::read_tree(&root)?;
    let report = stem_collision::analyse(&names);
    if report.ok() {
        return Ok(());
    }
    Err(format!(
        "these stem-keyed routing entries match more than one overlay source, so they \
apply to overlays they were never meant for -- move each to the path-keyed \
set naming the overlay it was added for: {}",
        report
            .findings
            .iter()
            .map(|finding| finding.message.clone())
            .collect::<Vec<String>>()
            .join("; ")
    ))
}
