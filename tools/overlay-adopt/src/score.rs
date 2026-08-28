use crate::park::{placeholder_span, truth_window};
use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};
use candidate_show::{cli::Options, render::render};
use compiler_core::{
    overlay_call_via_base,
    routing::CompilerTarget,
    source_paths::{SourceOwner, SourcePaths},
    translation_units::TranslationUnits,
};
use overlay_disasm::compile::compile_overlay_c;
use overlay_disasm::{OVERLAY_BASE, OVERLAY_LINK_BIAS};
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
    let work = tempdir().map_err(|error| error.to_string())?;
    let reference = work.path().join("reference.bin");
    let (window, oracle) = truth_window(root, &overlay, address, span)?;
    let start = (address - OVERLAY_BASE) as usize;
    let mut image = vec![0; start + window.len()];
    image[start..].copy_from_slice(&window);
    std::fs::write(&reference, image).map_err(|error| error.to_string())?;
    let paths = SourcePaths::load(root)?;
    let units = TranslationUnits::load(root)?;
    let mut configuration = CandidateCompilerConfiguration {
        family: Some(CandidateCompilerFamily::Routed),
        call_via_base: Some(
            paths
                .registered_call_via(owner)
                .map(u64::from)
                .unwrap_or_else(|| overlay_call_via_base(&overlay)),
        ),
        label_word_bias: Some(OVERLAY_LINK_BIAS as u64),
        ..Default::default()
    };
    if let Some(unit) = units.unit_for_game_owner("gs1", owner) {
        configuration.absolute_symbols = unit.canonical_symbols()?;
    }
    let rendered = render(
        root,
        &Options {
            source: source.to_string_lossy().into_owned(),
            rom: Some(reference.to_string_lossy().into_owned()),
            work: Some(work.path().to_string_lossy().into_owned()),
            flags: extra,
            configuration,
            target: CompilerTarget::Gs1,
            owner: Some(address as u32),
            overlay: Some(overlay),
            unit: None,
            precompiled_object: None,
            size: Some(span as usize),
            align,
            first: false,
            allocator_order: false,
            asm: false,
            patch: None,
        },
    )?;
    println!("reference_from={oracle}");
    print!("{}", rendered.stdout);
    Ok(i32::from(
        rendered.differing_halfwords != 0 || rendered.candidate_length != rendered.reference_length,
    ))
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
            let ordinary = no_asm_c::ordinary_source(root, source)?;
            count[5 - usize::from(ordinary)] += 1;
            ["nonordinary", "ordinary"][usize::from(ordinary)]
        };
        println!("exact-retained\t{}\t{class}", owner.id());
    }
    println!("overlay-corpus sources={} registered_owners={} placeholder_spans={} literal_pool_nonowners={} installed_exact={} nonexact={} exact_retained_ordinary={} exact_retained_nonordinary={} exact_unmapped={} unregistered_candidates={}", sources.len(), count[0], count[7], count[1], count[2], count[3], count[4], count[5], count[6], count[8]);
    Ok(i32::from(count[4..7].iter().sum::<usize>() != 0))
}
