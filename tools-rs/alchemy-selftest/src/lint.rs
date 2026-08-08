//! `callbackArityLint()`, the prologue `selfTest` runs before it touches
//! routing, and the `overlayStemCollisionLint()` it tail-calls.
//!
//! PORT NOTE -- WHY THIS IS NOT `alchemy_lints::callback_arity`. The sibling
//! `alchemy-lints` crate deliberately *rebuilt* this lint: it walks the tree
//! recursively and generalises over every multi-parameter function, because the
//! TypeScript version scans nothing (see below). That is the better lint and it
//! stays where it is. This module is the other job -- reproducing what
//! `selfTest` actually calls, bug included, so the differential harness has
//! something to be at parity with. Two files, two purposes, no duplication of
//! intent.

use std::path::Path;

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

    let leaking = map_by_reference_offenders(&repository_root())?;
    if !leaking.is_empty() {
        return Err(format!(
            "these pass externalSymbolAssembly to .map() by reference, which feeds the \
array index in as callViaBase: {}",
            leaking.join(", ")
        ));
    }

    overlay_stem_collision_lint()
}

/// The directory scan inside `callbackArityLint`.
///
/// PORT NOTE -- BUG REPRODUCED ON PURPOSE. The TypeScript computes the scan
/// directory as `dirname(dirname(dirname(fileURLToPath(import.meta.url))))`.
/// The file is `<root>/tools/lib/alchemy_gcc.ts`, so three `dirname` calls land
/// on `<root>`, not on `<root>/tools` as the comment above the code ("Seven
/// tools wrote the emitter as a bare callback") plainly intends. The repository
/// root holds no `.ts` files at all, so `readdirSync(...).filter(endsWith(".ts"))`
/// is empty and the lint has never inspected a single line. `overlayStemCollisionLint`
/// two functions below uses the *same* three-`dirname` expression and is correct
/// there, because it goes on to `join(root, "tools/lib/alchemy_gcc.ts")` -- which
/// is what proves the target here was meant to be one level down.
///
/// Reproduced rather than fixed: parity is the contract, and the fix belongs in
/// `alchemy_lints::callback_arity`, which already has it. The `read_dir` is a
/// real one, not a stubbed empty vector, so the day someone drops a `.ts` file
/// at the repository root both implementations start behaving identically again.
fn map_by_reference_offenders(directory: &Path) -> Result<Vec<String>, String> {
    let mut names: Vec<String> = Vec::new();
    let entries = std::fs::read_dir(directory)
        .map_err(|error| format!("cannot read {}: {error}", directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("cannot read {}: {error}", directory.display()))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if !name.ends_with(".ts") {
            continue;
        }
        // PORT NOTE: `readFileSync(..., "utf8")` on a non-UTF-8 file yields
        // replacement characters rather than throwing, which `from_utf8_lossy`
        // matches. A read *error* (a directory named `x.ts`, a permission
        // failure) does throw in Bun, so it propagates here too.
        let bytes = std::fs::read(entry.path())
            .map_err(|error| format!("cannot read {}: {error}", entry.path().display()))?;
        let text = String::from_utf8_lossy(&bytes);
        if text.split('\n').any(|line| {
            !line.trim_start().starts_with("//") && has_map_by_reference(line)
        }) {
            names.push(name);
        }
    }
    // PORT NOTE: `.sort()` on filenames -- UTF-16 code-unit order, matched by
    // `js_string_cmp` rather than `str::cmp`.
    names.sort_by(|left, right| crate::jsstring::js_string_cmp(left, right));
    Ok(names)
}

/// `/\.map\(\s*externalSymbol(Assembly)?\s*\)/.test(line)`.
///
/// PORT NOTE: hand-matched, not `regex`. The pattern is small enough that a
/// scanner is clearer than pulling in a crate, and the two details that matter
/// are easy to get wrong in either: JavaScript `\s` is NOT Rust's
/// `char::is_whitespace` (JS includes U+FEFF and excludes U+0085, Rust the
/// reverse), and the `(Assembly)?` group is greedy, so `externalSymbolAssembly`
/// must be tried before the bare `externalSymbol` prefix -- with the *optional*
/// group the bare name also matches, which is exactly what the lint wants,
/// hence no attempt to require one or the other.
fn has_map_by_reference(line: &str) -> bool {
    let characters: Vec<char> = line.chars().collect();
    let dot_map: Vec<char> = ".map(".chars().collect();
    for start in 0..characters.len() {
        if !characters[start..].starts_with(&dot_map) {
            continue;
        }
        let mut index = start + dot_map.len();
        while index < characters.len() && is_js_space(characters[index]) {
            index += 1;
        }
        let name: Vec<char> = "externalSymbol".chars().collect();
        if !characters[index..].starts_with(&name) {
            continue;
        }
        index += name.len();
        let assembly: Vec<char> = "Assembly".chars().collect();
        if characters[index..].starts_with(&assembly) {
            index += assembly.len();
        }
        while index < characters.len() && is_js_space(characters[index]) {
            index += 1;
        }
        if characters.get(index) == Some(&')') {
            return true;
        }
    }
    false
}

/// JavaScript's `\s`.
///
/// PORT NOTE: the differences from Rust's `char::is_whitespace` are real, not
/// theoretical: JS `\s` includes U+FEFF (byte-order mark) and U+00A0, and
/// EXCLUDES U+0085 (NEL), which Rust's `White_Space` property includes. A file
/// with a BOM immediately after `.map(` would be missed by the naive version.
pub fn is_js_space(character: char) -> bool {
    matches!(
        character,
        '\u{9}' | '\u{a}' | '\u{b}' | '\u{c}' | '\u{d}' | '\u{20}'
            | '\u{a0}' | '\u{1680}' | '\u{2000}'..='\u{200a}'
            | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}'
            | '\u{feff}'
    )
}

/// `overlayStemCollisionLint()`, delegated to the sibling crate that already
/// owns it. Only the failure *shape* is asserted here, exactly as `selfTest`
/// does: any collision throws, none passes silently.
pub fn overlay_stem_collision_lint() -> Result<(), String> {
    let root = repository_root();
    let (source, names) = stem_collision::read_tree(&root)?;
    let report = stem_collision::analyse(&source, &names);
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn map_by_reference_matches_both_arities_and_interior_space() {
        assert!(has_map_by_reference("names.map(externalSymbol)"));
        assert!(has_map_by_reference("names.map( externalSymbolAssembly )"));
        assert!(has_map_by_reference("x.map(\texternalSymbol\t)"));
    }

    #[test]
    fn map_by_reference_rejects_a_real_call() {
        assert!(!has_map_by_reference(
            "names.map((name) => externalSymbolAssembly(name, base))"
        ));
        assert!(!has_map_by_reference("names.map(externalSymbolish)"));
    }

    #[test]
    fn js_space_is_not_rust_whitespace() {
        // U+0085 is Rust whitespace but not JavaScript `\s`.
        assert!('\u{85}'.is_whitespace());
        assert!(!is_js_space('\u{85}'));
        // U+FEFF is JavaScript `\s` but not Rust whitespace.
        assert!(!'\u{feff}'.is_whitespace());
        assert!(is_js_space('\u{feff}'));
        assert!(has_map_by_reference(".map(\u{feff}externalSymbol)"));
    }

    #[test]
    fn the_repository_root_scan_finds_nothing_because_it_is_the_wrong_directory() {
        // Pins the reproduced bug. If this ever starts failing it means a `.ts`
        // file appeared at the repository root, and BOTH this port and the
        // TypeScript would then scan it -- which is still parity. What must
        // never happen is this port quietly scanning `tools/` while the
        // TypeScript scans the root.
        let root = repository_root();
        let scanned = std::fs::read_dir(&root)
            .expect("repository root is readable")
            .filter_map(|entry| entry.ok())
            .filter(|entry| entry.file_name().to_string_lossy().ends_with(".ts"))
            .count();
        let tools = root.join("tools/lib");
        let would_have_scanned = std::fs::read_dir(&tools)
            .expect("tools/lib is readable")
            .filter_map(|entry| entry.ok())
            .filter(|entry| entry.file_name().to_string_lossy().ends_with(".ts"))
            .count();
        assert_eq!(scanned, 0, "the lint's actual scan directory has no .ts files");
        assert!(
            would_have_scanned > 0,
            "the directory it was meant to scan is full of them, which is the bug"
        );
    }
}
