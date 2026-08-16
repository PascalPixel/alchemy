//! `main()`'s two branches.

use std::path::Path;

use alchemy_routing::routing::{root, CompilerTarget};
use candidate_compiler::verify::js_subarray;
use candidate_compiler::{verify_candidate, CandidateCompilerConfiguration, CandidateCompilerFamily};
use overlay_disasm::{assemble_overlay, compile_overlay_candidate, OverlaySource, OVERLAY_BASE};

use crate::cli::basename_without_c;
use crate::explain::{report, EXPLAIN_FLAGS};
use crate::jsnum::to_string_16;

/// The dump flags this tool compiles with, and deliberately nothing else.
///
/// There is no environment override here. `EXPLAIN_FLAGS` is codegen-neutral by
/// construction -- `-dS`, `-dR` and `-fsched-verbose=9` only ask for dumps -- so
/// what this tool compiles is what the routed build compiles, and a residual it
/// reports is a residual the ROM build has. An `ALCHEMY_CANDIDATE_EXPLAIN_EXTRA_FLAGS`
/// variable used to append arbitrary unvalidated tokens here. Nothing read it,
/// and it reached `mutated_compiler_flags` unfiltered, so a stray `-O1` in a
/// shell silently beat the routed `-O2` under gcc's later-flag-wins rule and
/// this tool would then explain a diff against a compile the ROM build never
/// performs. If a flag ever needs to vary, add a CLI argument: an explicit
/// argument appears in the command the reader can see, an exported variable
/// does not.
fn explain_flags() -> Vec<String> {
    EXPLAIN_FLAGS
        .iter()
        .map(|flag| (*flag).to_string())
        .collect()
}

/// The `options.mode === "overlay"` branch.
///
/// Takes the parsed `Options` value whole rather than seven positional
/// parameters. The TypeScript passes a single `options` object here too, so
/// this is the shape of the original, not a clippy workaround: nothing is
/// suppressed with `#[allow]`.
pub fn run_overlay(options: &crate::cli::Options) -> Result<String, String> {
    let crate::cli::Options::Overlay {
        id,
        overlay,
        offset,
        source,
        routing_source,
        work,
        span,
    } = options
    else {
        return Err("run_overlay called with a main-image invocation".to_string());
    };
    let (offset, span, source, routing_source, work) = (
        *offset,
        *span,
        source.as_path(),
        routing_source.as_path(),
        work.as_path(),
    );
    let inventory_path = root().join("out/decomp/overlays.json");
    let mut span = span;
    if span.is_none() && inventory_path.exists() {
        let text = String::from_utf8_lossy(
            &std::fs::read(&inventory_path).map_err(|error| error.to_string())?,
        )
        .into_owned();
        span = inventory_span(&text, id);
    }
    // PORT NOTE -- `span === undefined` is the ONLY rejected state. A `NaN`
    // span (from `--span` with no value, or `--span abc`) sails through this
    // guard and poisons every arithmetic downstream. `Option::is_none` is the
    // faithful translation; `span.map_or(true, f64::is_nan)` would be a fix.
    let Some(span) = span else {
        return Err(format!("no inventory row for {id}; pass --span BYTES"));
    };
    if !source.exists() {
        return Err(format!("candidate does not exist: {}", source.display()));
    }

    let overlay_path = root()
        .join("assets/code")
        .join(format!("{overlay}_overlay.s"));
    let image = assemble_overlay(&OverlaySource::path(&overlay_path), OVERLAY_BASE)?;
    // `image.subarray(offset, offset + span)` -- clamping, never panicking, and
    // empty whenever `span` is NaN.
    let expected = js_subarray(&image, offset, offset + span);
    let compiled = compile_overlay_candidate(
        source,
        work,
        overlay,
        Some(routing_source),
        &explain_flags(),
    )?;
    if (compiled.address as f64) != OVERLAY_BASE as f64 + offset {
        return Err(format!(
            "candidate entry 0x{} does not match {id}",
            to_string_16(compiled.address as f64)
        ));
    }
    let actual_path = work.join("candidate.bin");
    std::fs::write(&actual_path, &compiled.data).map_err(|error| error.to_string())?;
    report(
        &compiled.data,
        &expected,
        work,
        &actual_path,
        &basename_without_c(&routing_source.to_string_lossy()),
    )
}

/// `inventory.functions.find((item) => item.id === options.id)?.span_bytes`.
///
/// PORT NOTE -- a hand parse rather than `serde_json`, which this crate does
/// not depend on. `find` returns the FIRST matching row, so a duplicate `id`
/// keeps the first `span_bytes`; scanning is therefore linear and stops at the
/// first hit. A missing `span_bytes` on the matched row yields `undefined`,
/// which is `None` here, not `Some(NaN)`.
fn inventory_span(text: &str, id: &str) -> Option<f64> {
    let needle = format!("\"id\":\"{id}\"");
    let compact: String = text.chars().filter(|c| !c.is_whitespace()).collect();
    let at = compact.find(&needle)?;
    // The matched object ends at the next `}`.
    let end = compact[at..]
        .find('}')
        .map(|offset| at + offset)
        .unwrap_or(compact.len());
    let key = compact[at..end].find("\"span_bytes\":")? + at + "\"span_bytes\":".len();
    let digits: String = compact[key..end]
        .chars()
        .take_while(|c| c.is_ascii_digit() || *c == '-' || *c == '.' || *c == 'e')
        .collect();
    if digits.is_empty() {
        return None;
    }
    Some(thumb_disasm::js_number(&digits))
}

/// Slice the reference to the OWNER's extent, never to the candidate's.
///
/// `verify_candidate` derives its span from the linked binary whenever `nm`
/// reports no size, which is the normal case for these translation units. Its
/// `expected` is therefore exactly as long as whatever the candidate happened
/// to compile to, and comparing against that hides a size deficit completely:
/// a candidate four bytes short is measured against a reference truncated to
/// the same four-bytes-short length, so the missing tail can never appear in
/// `differing_halfwords`. On `080bbb0c` that reported `reference=6328` against
/// a true owner extent of 6,332, disagreeing with `decomp-diagnose` on the same
/// owner and making the deficit invisible in the tool whose whole job is to
/// explain the residual.
///
/// `decomp-diagnose` already resolves this through the assembly manifest. That
/// lookup is reused here rather than reimplemented, so the two tools cannot
/// disagree about one owner's reference length. When the manifest does not name
/// the stem there is no authoritative extent to use, and the caller's slice
/// stands.
fn authoritative_reference(repository: &Path, rom: &[u8], stem: &str, fallback: &[u8]) -> Vec<u8> {
    let Ok(address) = i64::from_str_radix(stem, 16) else {
        return fallback.to_vec();
    };
    let Some(size) = decomp_diagnose::lookup(&decomp_diagnose::assembly_sizes(repository), stem)
    else {
        return fallback.to_vec();
    };
    let offset = address as f64 - candidate_compiler::ROM_BASE;
    if offset < 0.0 || size <= 0 {
        return fallback.to_vec();
    }
    let begin = (offset as usize).min(rom.len());
    let end = begin.saturating_add(size as usize).min(rom.len());
    rom[begin..end].to_vec()
}

/// The main-image branch.
pub fn run_main_image(source: &Path, work: &Path) -> Result<String, String> {
    let repository = root();
    let rom_path = repository.join("roms/gs1-en.gba");
    // `readFileSync` throws on a missing ROM; scanning nothing is not passing.
    let rom = std::fs::read(&rom_path)
        .map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let verification = verify_candidate(
        &source.to_string_lossy(),
        &rom,
        &work.to_string_lossy(),
        &explain_flags(),
        candidate_compiler::ROM_BASE,
        CompilerTarget::Gs1,
        &CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            add_flags: Vec::new(),
            remove_flags: Vec::new(),
        },
    )?;
    let stem = basename_without_c(&source.to_string_lossy());
    let expected = authoritative_reference(&repository, &rom, &stem, &verification.expected);
    let actual_path = work.join(format!("{stem}.bin"));
    report(&verification.actual, &expected, work, &actual_path, &stem)
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A root with no assembly manifest has no authoritative extent to offer,
    /// so the caller's slice has to survive intact. Silently returning an empty
    /// reference here would read as "every byte matches".
    #[test]
    fn an_unknown_stem_keeps_the_callers_reference() {
        let rom = vec![0xaa; 64];
        let fallback = [1u8, 2, 3, 4];
        let missing = Path::new("/nonexistent-candidate-explain-root");
        assert_eq!(
            authoritative_reference(missing, &rom, "080bbb0c", &fallback),
            fallback.to_vec()
        );
    }

    /// A stem that is not hexadecimal has no address, so there is nothing to
    /// slice and the fallback stands rather than a panic or an empty vector.
    #[test]
    fn a_non_hexadecimal_stem_keeps_the_callers_reference() {
        let rom = vec![0xaa; 64];
        let fallback = [7u8, 8];
        let root = alchemy_routing::routing::root();
        assert_eq!(
            authoritative_reference(root, &rom, "not-an-address", &fallback),
            fallback.to_vec()
        );
    }

    /// An address below the ROM base cannot index the image. The fallback is
    /// the only honest answer; a negative offset must never wrap into a slice.
    #[test]
    fn an_address_below_the_rom_base_keeps_the_callers_reference() {
        let rom = vec![0xaa; 64];
        let fallback = [9u8];
        let root = alchemy_routing::routing::root();
        assert_eq!(
            authoritative_reference(root, &rom, "00000010", &fallback),
            fallback.to_vec()
        );
    }
}
