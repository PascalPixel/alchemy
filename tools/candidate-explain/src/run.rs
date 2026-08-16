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
    // Score against the OWNER's extent, never the candidate's. See
    // `decomp_diagnose::owner_reference`.
    let expected = decomp_diagnose::owner_reference(repository, &rom, &stem, &verification.expected);
    let actual_path = work.join(format!("{stem}.bin"));
    report(&verification.actual, &expected, work, &actual_path, &stem)
}
