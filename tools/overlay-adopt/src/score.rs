use crate::park::{placeholder_span, truth_window};
use candidate_show::disasm::disassemble;
use candidate_show::render::align_streams;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::compile::compile_overlay_c;
use overlay_disasm::OVERLAY_BASE;
use std::path::{Path, PathBuf};
use tempfile::tempdir;
fn resolve(root: &Path, target: &str) -> Result<(String, i64), String> {
    if let Some((overlay, address)) = target.split_once(':') {
        let address = i64::from_str_radix(address.trim_start_matches("0x"), 16)
            .map_err(|_| format!("{target}: address must be hexadecimal"))?;
        let address = if address < OVERLAY_BASE {
            address + OVERLAY_BASE
        } else {
            address
        };
        return Ok((overlay.to_string(), address));
    }
    if let Some(owner) = SourcePaths::load(root)?.owner_for_path(Path::new(target))? {
        let overlay = owner
            .overlay_id()
            .ok_or_else(|| format!("{target}: not an overlay source"))?;
        return Ok((overlay, owner.address() as i64));
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
    let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
    let exact = SourcePaths::load(root)?.source_path(owner);
    if exact.exists() {
        return Ok(exact);
    }
    let recon = root.join(format!(
        "games/gs1/recon/en/overlays/{overlay}_c_{address:08x}.c"
    ));
    if recon.exists() {
        return Ok(recon);
    }
    Err(format!(
        "no tracked or exact source for {overlay}:{address:08x}"
    ))
}
fn inventory_span(root: &Path, overlay: &str, address: i64) -> Option<i64> {
    let text = std::fs::read_to_string(root.join("out/decomp/overlays.json")).ok()?;
    let value: serde_json::Value = serde_json::from_str(&text).ok()?;
    for function in value.get("functions")?.as_array()? {
        if function.get("overlay")?.as_str()? != overlay {
            continue;
        }
        let entry = match function.get("entry")? {
            serde_json::Value::String(text) => {
                i64::from_str_radix(text.trim_start_matches("0x"), 16).ok()?
            }
            other => other.as_i64()?,
        };
        if entry == address {
            return function.get("span_bytes")?.as_i64();
        }
    }
    None
}
fn reviewed_span(root: &Path, overlay: &str, address: i64) -> Option<i64> {
    let text = std::fs::read_to_string(root.join("games/gs1/semantic/regions.json")).ok()?;
    let value: serde_json::Value = serde_json::from_str(&text).ok()?;
    for region in value.get("manual_regions")?.as_array()? {
        if region.get("overlay")?.as_str()? != overlay {
            continue;
        }
        let entry = match region.get("entry")? {
            serde_json::Value::String(text) => {
                i64::from_str_radix(text.trim_start_matches("0x"), 16).ok()?
            }
            other => other.as_i64()?,
        };
        if entry == address {
            return region.get("span_bytes")?.as_i64();
        }
    }
    None
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let mut align = false;
    let mut target = None;
    let mut extra: Vec<String> = Vec::new();
    let mut expecting_flags = false;
    let mut expecting_span = false;
    let mut override_span: Option<i64> = None;
    for argument in argv {
        if expecting_flags {
            extra.extend(argument.split(',').map(str::to_string));
            expecting_flags = false;
            continue;
        }
        if expecting_span {
            override_span = Some(
                argument
                    .parse::<i64>()
                    .map_err(|_| format!("--span wants a decimal byte count, got {argument:?}"))?,
            );
            expecting_span = false;
            continue;
        }
        match argument.as_str() {
            "--flags" => expecting_flags = true,
            "--span" => expecting_span = true,
            "--align" => align = true,
            "-h" | "--help" => {
                println!(
                    "usage: overlay score <overlay>:<addressHex> | <source.c> [--align]\n\n\
                     Prints the candidate and the ROM reference side by side, in the same\n\
                     shape as `candidate-show`. The span is derived.\n\n\
                     --span BYTES overrides that derivation. It is a DIAGNOSTIC, for\n\
                     the rows whose audited extent is wrong or absent -- a jump table\n\
                     leaves a hole and `adopt` refuses before any byte is compared, so\n\
                     without this there is no way to ask whether such a row is already\n\
                     exact. It does not adopt anything and does not change any gate."
                );
                return Ok(0);
            }
            other => target = Some(other.to_string()),
        }
    }
    let target = target.ok_or("a <overlay>:<addressHex> or source path is required")?;
    let (overlay, address) = resolve(root, &target)?;
    let explicit = Path::new(&target);
    let source = if explicit.is_file() {
        explicit
            .canonicalize()
            .map_err(|error| format!("{}: {error}", explicit.display()))?
    } else {
        source_for(root, &overlay, address)?
    };
    let span = match override_span {
        Some(span) => Some(span),
        None => placeholder_span(root, &overlay, address)?
            .or_else(|| inventory_span(root, &overlay, address))
            .or_else(|| reviewed_span(root, &overlay, address))
            .or(crate::audited_code_span(root, &overlay, address)?),
    }
    .ok_or_else(|| {
        format!("{overlay}:{address:08x} has neither a placeholder nor an audited span")
    })?;
    let (reference, oracle) = truth_window(root, &overlay, address, span)?;
    let work = tempdir().map_err(|error| error.to_string())?;
    let compiled = compile_overlay_c(&source, work.path(), &overlay, None, &extra)?;
    let differing = reference
        .chunks(2)
        .zip(compiled.data.chunks(2))
        .filter(|(left, right)| left != right)
        .count()
        + reference.len().abs_diff(compiled.data.len()).div_ceil(2);
    println!("candidate={} reference={} differing_halfwords={differing} source={} reference_from={oracle}", compiled.data.len(), reference.len(), source.strip_prefix(root).unwrap_or(&source).display(),);
    let bin = tempdir().map_err(|error| error.to_string())?;
    let ours = bin.path().join("candidate.bin");
    let theirs = bin.path().join("reference.bin");
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
    Ok(if differing == 0 { 0 } else { 1 })
}
