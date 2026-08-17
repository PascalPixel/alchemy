//! Score one overlay row the same way a main-image owner is scored.
//!
//! WHY THIS EXISTS. A main-image owner is scored with `candidate-show`, which
//! prints the two instruction streams side by side. An overlay row had no
//! equivalent: you rehearsed an adoption and got a single differing-byte count,
//! or you knew the span and reached for a scratch tool. Two different
//! instruments for the same question is how incompatible numbers get pooled
//! into one table, and how a contributor learns to rank by a scalar instead of
//! reading the diff.
//!
//! This prints the same shape as `candidate-show`, takes the same `--align`,
//! and needs no span argument: the span comes from the row's placeholder when
//! it is adopted, from the overlay inventory when it is parked, and from the
//! audited interval only as a last resort. The reference comes from the ROM.

use std::path::{Path, PathBuf};

use candidate_show::disasm::disassemble;
use candidate_show::render::align_streams;
use overlay_disasm::compile::compile_overlay_c;
use overlay_disasm::OVERLAY_BASE;

use crate::park::{placeholder_span, truth_window};

fn temp_dir(tag: &str) -> PathBuf {
    let path = std::env::temp_dir().join(format!("alchemy-score-{tag}-{}", std::process::id()));
    let _ = std::fs::create_dir_all(&path);
    path
}

/// `<overlay>:<hex>` or a `semantic/`/`exact/` source path.
fn resolve(root: &Path, target: &str) -> Result<(String, i64), String> {
    if let Some((overlay, address)) = target.split_once(':') {
        let address = i64::from_str_radix(address.trim_start_matches("0x"), 16)
            .map_err(|_| format!("{target}: address must be hexadecimal"))?;
        // Accept both the full address and the overlay-relative offset.
        let address = if address < OVERLAY_BASE { address + OVERLAY_BASE } else { address };
        return Ok((overlay.to_string(), address));
    }
    let name = Path::new(target)
        .file_stem()
        .map(|stem| stem.to_string_lossy().to_string())
        .ok_or_else(|| format!("{target}: not a source path"))?;
    let (overlay, address) = name
        .split_once("_c_")
        .ok_or_else(|| format!("{target}: not an overlay source name"))?;
    let address = i64::from_str_radix(address, 16)
        .map_err(|_| format!("{target}: address must be hexadecimal"))?;
    let _ = root;
    Ok((overlay.to_string(), address))
}

fn source_for(root: &Path, overlay: &str, address: i64) -> Result<PathBuf, String> {
    for directory in ["semantic", "exact"] {
        let path = root.join(format!("{directory}/{overlay}_c_{address:08x}.c"));
        if path.exists() {
            return Ok(path);
        }
    }
    Err(format!(
        "no semantic/ or exact/ source for {overlay}:{address:08x}"
    ))
}


/// The row's own span from the overlay inventory.
///
/// Preferred over the audited interval, which runs to the end of a whole
/// executable run and would report one function's span as many.
fn inventory_span(root: &Path, overlay: &str, address: i64) -> Option<i64> {
    let text = std::fs::read_to_string(root.join("out/decomp/overlays.json")).ok()?;
    let value: serde_json::Value = serde_json::from_str(&text).ok()?;
    for function in value.get("functions")?.as_array()? {
        if function.get("overlay")?.as_str()? != overlay {
            continue;
        }
        let entry = match function.get("entry")? {
            serde_json::Value::String(text) => i64::from_str_radix(text.trim_start_matches("0x"), 16).ok()?,
            other => other.as_i64()?,
        };
        if entry == address {
            return function.get("span_bytes")?.as_i64();
        }
    }
    None
}

pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let mut align = false;
    let mut target = None;
    let mut extra: Vec<String> = Vec::new();
    let mut expecting_flags = false;
    for argument in argv {
        if expecting_flags {
            // Comma-separated, so one shell word carries a whole set.
            extra.extend(argument.split(',').map(str::to_string));
            expecting_flags = false;
            continue;
        }
        match argument.as_str() {
            "--flags" => expecting_flags = true,
            "--align" => align = true,
            "-h" | "--help" => {
                println!(
                    "usage: overlay score <overlay>:<addressHex> | <source.c> [--align]\n\n\
                     Prints the candidate and the ROM reference side by side, in the same\n\
                     shape as `candidate-show`. The span is derived; do not pass one."
                );
                return Ok(0);
            }
            other => target = Some(other.to_string()),
        }
    }
    let target = target.ok_or("a <overlay>:<addressHex> or source path is required")?;
    let (overlay, address) = resolve(root, &target)?;
    // An explicit path is scored AS GIVEN. Deriving the source from the
    // filename instead would silently score the tree's copy, so every variant
    // in a `/tmp` scratch directory would report the same number and a whole
    // experiment would read as "no source shape moves it".
    // Absolutised: the compile runs from a work directory, so a relative path
    // reaches the compiler as a missing file rather than as this source.
    let explicit = Path::new(&target);
    let source = if explicit.is_file() {
        explicit
            .canonicalize()
            .map_err(|error| format!("{}: {error}", explicit.display()))?
    } else {
        source_for(root, &overlay, address)?
    };

    let span = placeholder_span(root, &overlay, address)?
        .or_else(|| inventory_span(root, &overlay, address))
        .or(crate::audited_code_span(root, &overlay, address)?)
        .ok_or_else(|| {
            format!("{overlay}:{address:08x} has neither a placeholder nor an audited span")
        })?;
    let (reference, oracle) = truth_window(root, &overlay, address, span)?;

    let work = temp_dir("work");
    let compiled = compile_overlay_c(&source, &work, &overlay, None, &extra)?;

    let differing = reference
        .chunks(2)
        .zip(compiled.data.chunks(2))
        .filter(|(left, right)| left != right)
        .count()
        + reference.len().abs_diff(compiled.data.len()).div_ceil(2);

    println!(
        "candidate={} reference={} differing_halfwords={differing} source={} reference_from={oracle}",
        compiled.data.len(),
        reference.len(),
        source.strip_prefix(root).unwrap_or(&source).display(),
    );

    let bin = temp_dir("bin");
    let ours = bin.join("candidate.bin");
    let theirs = bin.join("reference.bin");
    std::fs::write(&ours, &compiled.data).map_err(|error| error.to_string())?;
    std::fs::write(&theirs, &reference).map_err(|error| error.to_string())?;
    let base = address as f64;
    let left = disassemble(&ours.to_string_lossy(), base)?;
    let right = disassemble(&theirs.to_string_lossy(), base)?;

    let ordered = |rows: &candidate_show::disasm::Rows| -> Vec<String> {
        let mut keys: Vec<f64> = rows.keys().collect();
        keys.sort_by(|a, b| a.partial_cmp(b).unwrap_or(std::cmp::Ordering::Equal));
        keys.iter()
            .map(|key| rows.get(*key).unwrap_or("").to_string())
            .collect()
    };
    let left_lines = ordered(&left);
    let right_lines = ordered(&right);

    // WHICH RESIDUALS ARE WORTH YOUR TIME.
    //
    // Two rows can show the same differing-halfword count and be completely
    // different problems. If both sides hold the SAME instructions and only
    // their order differs, no source shape reaches it -- the order is decided
    // after reload, by `rank_for_schedule`'s tie-break chain, and the C no
    // longer controls it. If some instruction is genuinely wrong, the source
    // is wrong and reading it will find the defect.
    //
    // Measured 2026-08-17 over the 282 size-exact parked overlay rows: 98 rows
    // (14,858 bytes) are ordering-only and 183 rows (47,456 bytes) carry a
    // wrong instruction. Ranking those 183 by this count, rather than by
    // differing halfwords, is what puts a readable defect at the top.
    //
    // Shared with `overlay candidate-rank`, so the bulk table and the single
    // owner never disagree about which residuals are worth reading.
    let (class, wrong) = candidate_show::render::residual_class(&left_lines, &right_lines);
    println!("class={class} wrong_instructions={wrong}");

    if align {
        println!("      candidate                      reference");
        for (candidate, reference) in align_streams(&left_lines, &right_lines) {
            let mark = match (&candidate, &reference) {
                (Some(a), Some(b)) if a == b => " ",
                (Some(_), Some(_)) => "!",
                (Some(_), None) => "+",
                (None, Some(_)) => "-",
                (None, None) => " ",
            };
            let a = candidate.unwrap_or_default();
            println!("  {mark} {a:<30} {}", reference.unwrap_or_default());
        }
    } else {
        if left_lines.len() != right_lines.len() {
            println!(
                "  note: the two sides have different instruction counts, so the rows below\n\
                 \x20       are phase-shifted. Re-run with --align."
            );
        }
        println!("      candidate                      reference");
        for index in 0..left_lines.len().max(right_lines.len()) {
            let a = left_lines.get(index).cloned().unwrap_or_default();
            let b = right_lines.get(index).cloned().unwrap_or_default();
            let mark = if a == b { " " } else { "!" };
            println!("  {mark} {a:<30} {b}");
        }
    }
    let _ = std::fs::remove_dir_all(&work);
    let _ = std::fs::remove_dir_all(&bin);
    Ok(if differing == 0 { 0 } else { 1 })
}
