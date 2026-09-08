use crate::compiler::{
    source_paths::{SourceOwner, SourcePaths},
    symbols::overlay_call_via_base,
    translation_units::{resolve_overlay_span, TranslationUnits},
};
use crate::overlay::compile::compile_overlay_c;
use crate::overlay::{
    park::{placeholder_span, truth_window},
    retained_source,
};
use crate::score::{
    cli::{options_of, ParseOutcome, USAGE},
    render::render,
};
use serde_json::Value;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use tempfile::tempdir;
fn nonowner_relationship(
    kind: &str,
    entry: SourceOwner,
    reviewed: &BTreeMap<SourceOwner, usize>,
) -> Option<(&'static str, Option<SourceOwner>)> {
    match kind {
        "literal_pool" => return Some(("literal-pool", None)),
        "executable_alignment" => {}
        _ => return None,
    }
    let overlay = entry.overlay_id()?;
    let address = i64::from(entry.address());
    reviewed.iter().find_map(|(owner, span)| {
        let start = i64::from(owner.address());
        match (owner.overlay_id().as_deref() == Some(&overlay), start) {
            (true, start) if start < address && address < start + *span as i64 => {
                Some(("alignment-inside", Some(*owner)))
            }
            (true, start) if start == address + 2 => Some(("alignment-before", Some(*owner))),
            _ => None,
        }
    })
}
fn retained_fragment_span(
    retention: Option<&str>,
    span: Option<u64>,
    complete: usize,
) -> Option<usize> {
    retention?.starts_with("keep_").then_some(())?;
    let span = usize::try_from(span?).ok().filter(|span| *span > 0)?;
    (span < complete).then_some(span)
}
pub(crate) fn resolve(root: &Path, target: &str) -> Result<SourceOwner, String> {
    if target.contains(':') {
        let owner = SourceOwner::parse_argument(target)?;
        owner
            .overlay_id()
            .ok_or_else(|| format!("{target}: not an overlay owner"))?;
        return Ok(owner);
    }
    if let Some(owner) = Path::new(target)
        .file_stem()
        .and_then(|stem| stem.to_str())
        .and_then(SourceOwner::from_legacy_stem)
    {
        owner
            .overlay_id()
            .ok_or_else(|| format!("{target}: not an overlay owner"))?;
        return Ok(owner);
    }
    let owner = SourcePaths::load(root)?
        .owner_for_path(Path::new(target))?
        .ok_or_else(|| format!("{target}: not a mapped overlay source"))?;
    owner
        .overlay_id()
        .ok_or_else(|| format!("{target}: not an overlay source"))?;
    Ok(owner)
}
fn source_for(root: &Path, paths: &SourcePaths, owner: SourceOwner) -> Result<PathBuf, String> {
    let candidates = [paths.source_path(owner), retained_source(root, owner)];
    candidates
        .into_iter()
        .find(|candidate| candidate.exists())
        .ok_or_else(|| format!("no source for {}", owner.id()))
}
pub fn run(root: &Path, argv: &[String]) -> Result<i32, String> {
    let ParseOutcome::Options(options) = options_of(root, argv)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    if options.unit.is_some() {
        return Err("score complete translation units with alchemy score --unit ID".into());
    }
    if options.target != crate::compiler::routing::CompilerTarget::Gs1
        || argv.iter().any(|arg| arg == "--rom")
    {
        return Err("overlay scoring currently requires the canonical GS1 reference".into());
    }
    let rendered = render_options(root, options)?;
    println!("reference_from=rom representation=loader-runtime container_roundtrip=required");
    print!("{}", rendered.stdout);
    Ok(i32::from(
        rendered.differing_halfwords != 0 || rendered.candidate_length != rendered.reference_length,
    ))
}

pub(crate) fn render_options(
    root: &Path,
    mut options: Box<crate::score::cli::Options>,
) -> Result<crate::score::render::RenderOutput, String> {
    let target = options.source.clone();
    let resolved = if let Some(address) = options.owner {
        let overlay = options
            .overlay
            .as_deref()
            .ok_or("expected a resource-qualified overlay owner")?;
        SourceOwner::parse_argument(&format!("{overlay}:{address:08x}"))?
    } else {
        resolve(root, &target)?
    };
    let overlay = resolved.overlay_id().expect("resolved overlay owner");
    let address = i64::from(resolved.address());
    let paths = SourcePaths::load(root)?;
    let installed = if paths
        .mapped_source_path(resolved)
        .is_some_and(|path| path.is_file())
    {
        placeholder_span(root, resolved)?.and_then(|span| usize::try_from(span).ok())
    } else {
        None
    };
    let span = resolve_overlay_span(
        &crate::overlay::reviewed_spans(root)?,
        resolved,
        installed,
        options.size,
    )?;
    let explicit = Path::new(&target);
    let source = if explicit.is_file() {
        explicit
            .canonicalize()
            .map_err(|error| format!("{}: {error}", explicit.display()))?
    } else {
        source_for(root, &paths, resolved)?
    };
    let work = tempdir().map_err(|error| error.to_string())?;
    let reference = work.path().join("reference.bin");
    let image = crate::overlay::rom::canonical_overlay(root, &overlay)?;
    std::fs::write(&reference, image).map_err(|error| error.to_string())?;
    let units = TranslationUnits::load(root)?;
    options.source = source.to_string_lossy().into_owned();
    options.configuration.call_via_base = Some(
        paths
            .registered_call_via(resolved)
            .map(u64::from)
            .unwrap_or_else(|| overlay_call_via_base(&overlay)),
    );
    options.configuration.overlay_extent = Some(span);
    if let Some(unit) = units.unit_for_game_owner("gs1", resolved) {
        options.configuration.absolute_symbols = unit.canonical_symbols()?;
    }
    options.rom = Some(reference.to_string_lossy().into_owned());
    options.owner = Some(address as u32);
    options.overlay = Some(overlay);
    options.size = Some(span);
    render(root, &options)
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
    let reviewed = crate::overlay::reviewed_spans(root)?;
    let dossiers: Value = serde_json::from_slice(
        &std::fs::read(root.join("games/gs1/recon/en/dossiers.json"))
            .map_err(|error| error.to_string())?,
    )
    .map_err(|error| error.to_string())?;
    // registered, nonowner, installed, nonexact, ordinary, nonordinary, exact-unmapped, placeholders, unregistered, parked
    let mut count = [0usize; 10];
    for source in &sources {
        let target = resolve(root, &source.to_string_lossy())?;
        let overlay = target.overlay_id().expect("resolved overlay owner");
        let address = i64::from(target.address());
        let placeholder = placeholder_span(root, target)?;
        let span = placeholder.or_else(|| reviewed.get(&target).map(|span| *span as i64));
        let Some(span) = span else {
            let kind = crate::overlay::audited_kind(root, &overlay, address)?;
            let relationship = kind
                .as_deref()
                .and_then(|kind| nonowner_relationship(kind, target, &reviewed));
            let Some((relationship, related)) = relationship else {
                return Err(format!(
                    "{} has no mapped owner; audited kind is {} without a reviewed-owner relationship",
                    source.display(), kind.as_deref().unwrap_or("unknown")
                ));
            };
            count[1] += 1;
            println!(
                "not-owner\t{overlay}:{address:08x}\taudited-{relationship}\t{}",
                related
                    .map(|owner| owner.id())
                    .unwrap_or_else(|| "none".into())
            );
            continue;
        };
        let registered = paths.registered_name(target).is_some();
        count[0] += usize::from(registered);
        count[7] += usize::from(placeholder.is_some());
        count[8] += usize::from(!registered);
        let destination = paths.registered_source_path(target);
        if placeholder.is_some() && destination.as_ref().is_ok_and(|path| path.is_file()) {
            count[2] += 1;
            println!("installed-owner\t{}", target.id());
            continue;
        }
        if placeholder.is_none() {
            let dossier = &dossiers["records"][target.id()];
            let candidate_span = dossier["span_bytes"]
                .as_u64()
                .or_else(|| dossier["owner_bytes"].as_u64());
            if let Some(fragment) =
                retained_fragment_span(dossier["retention"].as_str(), candidate_span, span as usize)
            {
                count[9] += 1;
                println!(
                    "unverified-retained-fragment\t{}\tfragment_span={}\tcomplete_span={span}",
                    target.id(),
                    fragment
                );
                continue;
            }
        }
        let (reference, _) = truth_window(root, target, span)?;
        let work = tempdir().map_err(|error| error.to_string())?;
        if compile_overlay_c(source, work.path(), &overlay, span as usize, None, &[])?.data
            != reference
        {
            count[3] += usize::from(registered);
            continue;
        }
        let class = if !registered || destination.is_err() {
            count[6] += 1;
            "unmapped"
        } else {
            let ordinary = crate::compiler::no_asm::ordinary_source(root, source)?;
            count[5 - usize::from(ordinary)] += 1;
            ["nonordinary", "ordinary"][usize::from(ordinary)]
        };
        println!("exact-retained\t{}\t{class}", target.id());
    }
    println!("overlay-corpus sources={} registered_owners={} placeholder_spans={} audited_nonowners={} installed_owners={} nonexact={} exact_retained_ordinary={} exact_retained_nonordinary={} exact_unmapped={} unregistered_candidates={} unverified_retained_fragments={}", sources.len(), count[0], count[7], count[1], count[2], count[3], count[4], count[5], count[6], count[8], count[9]);
    Ok(i32::from(count[4..7].iter().sum::<usize>() != 0))
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn explicit_score_span_cannot_override_a_pool_head_or_owner_extent() {
        let root = crate::compiler::routing::root();
        for (target, span, message) in [
            (
                "resource_3c5:0200186c",
                "596",
                "no reviewed complete owner boundary",
            ),
            (
                "resource_3c5:02001b10",
                "2394",
                "differs from complete installed extent 2396",
            ),
        ] {
            let args = [target, "--span", span].map(str::to_owned);
            assert!(run(root, &args).unwrap_err().contains(message));
        }
    }
    fn owner(value: &str) -> SourceOwner {
        SourceOwner::parse_argument(value).unwrap()
    }
    #[test]
    fn executable_alignment_requires_same_overlay_owner_relationship() {
        let reviewed = BTreeMap::from([(owner("resource_371:02000100"), 0x20)]);
        let relation = |kind, entry| nonowner_relationship(kind, owner(entry), &reviewed);
        for (entry, expected) in [
            ("resource_371:02000110", "alignment-inside"),
            ("resource_371:020000fe", "alignment-before"),
        ] {
            assert_eq!(relation("executable_alignment", entry).unwrap().0, expected);
        }
        assert!([
            ("executable_alignment", "resource_372:02000110"),
            ("executable_alignment", "resource_371:020000fc"),
            ("thumb", "resource_371:02000110"),
            ("unknown", "resource_371:02000110"),
        ]
        .into_iter()
        .all(|(kind, entry)| relation(kind, entry).is_none()));
    }
    #[test]
    fn only_shorter_retained_spans_are_fragments() {
        for (retention, span, expected) in [
            (Some("keep_structured_asm"), Some(12), Some(12)),
            (Some("keep_structured_asm"), Some(16), None),
            (Some("keep_structured_asm"), Some(20), None),
            (None, Some(12), None),
            (Some("measured-draft"), Some(12), None),
            (Some("keep_structured_asm"), None, None),
        ] {
            assert_eq!(retained_fragment_span(retention, span, 16), expected);
        }
    }
}
