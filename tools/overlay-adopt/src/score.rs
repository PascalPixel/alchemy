use crate::park::{placeholder_span, truth_window};
use candidate_show::disasm::disassemble;
use candidate_show::render::{align_streams, ordered_lines, side_by_side};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::compile::compile_overlay_c;
use overlay_disasm::OVERLAY_BASE;
use std::path::{Path, PathBuf};
use tempfile::tempdir;
pub(crate) fn resolve(root: &Path, target: &str) -> Result<(String, i64), String> {
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
    if let Some((overlay, address)) = Path::new(target)
        .file_stem()
        .and_then(|stem| stem.to_str())
        .and_then(|stem| stem.split_once("_c_"))
        .and_then(|(overlay, address)| {
            i64::from_str_radix(address, 16)
                .ok()
                .map(|address| (overlay.to_string(), address))
        })
    {
        return Ok((overlay, address));
    }
    let owner = SourcePaths::load(root)?
        .owner_for_path(Path::new(target))?
        .ok_or_else(|| format!("{target}: not a mapped overlay source"))?;
    Ok((
        owner
            .overlay_id()
            .ok_or_else(|| format!("{target}: not an overlay source"))?,
        i64::from(owner.address()),
    ))
}
fn source_for(root: &Path, overlay: &str, address: i64) -> Result<PathBuf, String> {
    let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
    let candidates = [
        SourcePaths::load(root)?.source_path(owner),
        root.join(format!(
            "games/gs1/recon/en/overlays/{overlay}_c_{address:08x}.c"
        )),
    ];
    candidates
        .into_iter()
        .find(|candidate| candidate.exists())
        .ok_or_else(|| format!("no source for {overlay}:{address:08x}"))
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let (mut align, mut target, mut override_span) = (false, None, None);
    let mut extra = Vec::new();
    let mut args = argv.iter();
    while let Some(argument) = args.next() {
        match argument.as_str() {
            "--flags" => extra.extend(
                args.next()
                    .ok_or("--flags needs a comma-separated value")?
                    .split(',')
                    .map(str::to_string),
            ),
            "--span" => {
                override_span = Some(
                    args.next()
                        .and_then(|value| value.parse().ok())
                        .ok_or("--span wants a decimal byte count")?,
                )
            }
            "--align" => align = true,
            "-h" | "--help" => {
                println!(
                    "usage: overlay score <overlay>:<addressHex> | <source.c> [--align]\n\n\
                     Compare candidate and reference bytes. A mapped owner supplies the span;\n\
                     --span BYTES is an explicit read-only diagnostic override."
                );
                return Ok(0);
            }
            other if target.is_none() => target = Some(other.to_string()),
            other => return Err(format!("unexpected argument {other:?}")),
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
    let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
    let reviewed = crate::reviewed_spans(root)?;
    let span = match override_span {
        Some(span) => Some(span),
        None => placeholder_span(root, &overlay, address)?
            .or_else(|| reviewed.get(&owner).map(|span| *span as i64)),
    };
    let span = match span {
        Some(span) => span,
        None => match crate::audited_kind(root, &overlay, address)? {
            Some(kind) => {
                return Err(format!(
                    "{overlay}:{address:08x} begins in audited {kind}; it is not a mapped owner"
                ))
            }
            None => return Err(format!("{overlay}:{address:08x} has no mapped owner span")),
        },
    };
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
    let left_lines = ordered_lines(&left);
    let right_lines = ordered_lines(&right);
    let (class, wrong) = candidate_show::render::residual_class(&left_lines, &right_lines);
    println!("class={class} wrong_instructions={wrong}");
    if !align && left_lines.len() != right_lines.len() {
        println!("note: instruction counts differ; use --align to resynchronize");
    }
    let rows = if align {
        align_streams(&left_lines, &right_lines)
    } else {
        (0..left_lines.len().max(right_lines.len()))
            .map(|i| (left_lines.get(i).cloned(), right_lines.get(i).cloned()))
            .collect()
    };
    println!("      candidate                      reference");
    print!("{}", side_by_side(&rows));
    Ok(if differing == 0 { 0 } else { 1 })
}
pub fn audit_corpus(root: &Path) -> Result<i32, String> {
    let directory = root.join("games/gs1/recon/en/overlays");
    let mut sources = std::fs::read_dir(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .filter_map(Result::ok)
        .map(|entry| entry.path())
        .filter(|path| path.extension().and_then(|value| value.to_str()) == Some("c"))
        .collect::<Vec<_>>();
    sources.sort();
    if sources.is_empty() {
        return Err("overlay reconstruction corpus is empty".into());
    }
    let paths = SourcePaths::load(root)?;
    let reviewed = crate::reviewed_spans(root)?;
    // registered, nonowner, installed, nonexact, ordinary, nonordinary, exact-unmapped, placeholders, unregistered
    let mut count = [0usize; 9];
    for source in &sources {
        let target = source.to_string_lossy();
        let (overlay, address) = resolve(root, &target)?;
        let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
        let placeholder = placeholder_span(root, &overlay, address)?;
        let span = placeholder.or_else(|| reviewed.get(&owner).map(|span| *span as i64));
        let Some(span) = span else {
            if crate::audited_kind(root, &overlay, address)?.as_deref() != Some("literal_pool") {
                return Err(format!(
                    "{} has no mapped owner and is not audited literal-pool data",
                    source.display()
                ));
            }
            count[1] += 1;
            println!("not-owner\t{overlay}:{address:08x}\taudited-literal-pool");
            continue;
        };
        let registered = paths.registered_name(owner).is_some();
        count[0] += usize::from(registered);
        count[7] += usize::from(placeholder.is_some());
        count[8] += usize::from(!registered);
        let (reference, _) = truth_window(root, &overlay, address, span)?;
        let work = tempdir().map_err(|error| error.to_string())?;
        if compile_overlay_c(source, work.path(), &overlay, None, &[])?.data != reference {
            count[3] += usize::from(registered);
            continue;
        }
        let destination = paths.registered_source_path(owner);
        if destination.as_ref().is_ok_and(|path| path.is_file()) {
            count[2] += 1;
            continue;
        }
        let class = if !registered || destination.is_err() {
            count[6] += 1;
            "unmapped"
        } else {
            let ordinary = crate::ordinary_source(root, source)?;
            count[5 - usize::from(ordinary)] += 1;
            ["nonordinary", "ordinary"][usize::from(ordinary)]
        };
        println!("exact-retained\t{}\t{class}", owner.id());
    }
    println!("overlay-corpus sources={} registered_owners={} placeholder_spans={} literal_pool_nonowners={} installed_exact={} nonexact={} exact_retained_ordinary={} exact_retained_nonordinary={} exact_unmapped={} unregistered_candidates={}", sources.len(), count[0], count[7], count[1], count[2], count[3], count[4], count[5], count[6], count[8]);
    Ok(i32::from(count[4..7].iter().sum::<usize>() != 0))
}
