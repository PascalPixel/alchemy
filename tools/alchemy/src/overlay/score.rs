use crate::compiler::{
    routing::CompilerTarget,
    source_paths::{SourceOwner, SourcePaths},
    symbols::overlay_call_via_base,
    translation_units::{resolve_overlay_span, TranslationUnits},
};
use crate::score::{
    cli::{options_of, ParseOutcome, USAGE},
    render::render,
};
use std::path::{Path, PathBuf};
pub(crate) fn resolve(root: &Path, target: &str) -> Result<SourceOwner, String> {
    resolve_for(root, target, CompilerTarget::Tbs)
}
fn resolve_for(root: &Path, target: &str, game: CompilerTarget) -> Result<SourceOwner, String> {
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
    let owner = SourcePaths::load_for_game(root, game.as_str())?
        .owner_for_path(Path::new(target))?
        .ok_or_else(|| format!("{target}: not a mapped overlay source"))?;
    owner
        .overlay_id()
        .ok_or_else(|| format!("{target}: not an overlay source"))?;
    Ok(owner)
}
fn source_for(
    root: &Path,
    paths: &SourcePaths,
    owner: SourceOwner,
    game: CompilerTarget,
) -> Result<PathBuf, String> {
    let candidates = [
        paths.source_path(owner),
        root.join(format!(
            "{}/en/overlays/{}.c",
            game.recon(),
            owner.legacy_stem()
        )),
    ];
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
    if argv.iter().any(|arg| arg == "--rom") {
        return Err(
            "overlay scoring requires the selected game's canonical reference; omit --rom".into(),
        );
    }
    if let Some((output, exact)) = score_instance_owner(root, &options)? {
        println!("reference_from=rom representation=loader-runtime container_roundtrip=required");
        print!("{output}");
        return Ok(i32::from(!exact));
    }
    let owner = match (options.owner, options.overlay.as_deref()) {
        (Some(address), Some(overlay)) => SourceOwner::parse(&format!("{overlay}:{address:08x}")),
        _ => resolve_for(root, &options.source, options.target),
    };
    let game = options.target;
    let rendered = render_options(root, options)?;
    println!("reference_from=rom representation=loader-runtime container_roundtrip=required");
    print!("{}", rendered.stdout);
    if let (CompilerTarget::Tbs, Ok(owner)) = (game, owner) {
        print!(
            "{}",
            crate::score::cli::siblings_line(root, owner, rendered.reference_length)
        );
    }
    Ok(i32::from(
        rendered.differing_halfwords != 0 || rendered.candidate_length != rendered.reference_length,
    ))
}

/// An instance owner has no standalone object: its candidate compiles as the
/// whole unit source, links into the owner's image as production links it,
/// and every owner of that instance is compared over its complete extent.
fn score_instance_owner(
    root: &Path,
    options: &crate::score::cli::Options,
) -> Result<Option<(String, bool)>, String> {
    let (Some(address), Some(overlay)) = (options.owner, options.overlay.as_deref()) else {
        return Ok(None);
    };
    let owner = SourceOwner::parse(&format!("{overlay}:{address:08x}"))?;
    let units = TranslationUnits::load_game(root, options.target)?;
    let Some(unit) = units.unit_for_game_owner(options.target.as_str(), owner) else {
        return Ok(None);
    };
    let Some(member) = unit.instance_owner(overlay, address) else {
        return Ok(None);
    };
    let route = format!("alchemy score --unit {} --instance {overlay}", unit.id);
    if options.asm
        || options.allocator_order
        || options.patch.is_some()
        || options.configuration.owner_symbol.is_some()
        || options.configuration.reference_symbols
    {
        return Err(format!(
            "{} is an instance owner of unit {}: its score compiles the whole unit, so --asm, --allocator-order, --patch, --symbol and --reference-symbols do not apply; see {route}",
            owner.id(),
            unit.id
        ));
    }
    if let Some(size) = options.size.filter(|size| *size != member.extent) {
        return Err(format!(
            "{}: requested span {size} differs from complete instance extent {}",
            owner.id(),
            member.extent
        ));
    }
    let explicit = Path::new(&options.source);
    let candidate = match explicit.is_file() {
        true => explicit
            .canonicalize()
            .map_err(|error| format!("{}: {error}", explicit.display()))?,
        false => root.join(&unit.source),
    };
    let work = options.work.as_ref().map(|work| root.join(work));
    let (scored, differing) = crate::score::score_overlay_image(
        unit,
        overlay,
        Some(&candidate),
        work.as_deref(),
        true,
        Some(address),
    )?;
    let output = format!(
        "unit={} instance={overlay} owner={}\n{scored}differing_owners={}\n",
        unit.id,
        owner.id(),
        differing.len()
    );
    Ok(Some((output, differing.is_empty())))
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
        resolve_for(root, &target, options.target)?
    };
    let overlay = resolved.overlay_id().expect("resolved overlay owner");
    let address = i64::from(resolved.address());
    let game = crate::overlay::owners::production_target(options.target);
    let paths = SourcePaths::load_for_game(root, options.target.as_str())?;
    let installed = if paths
        .mapped_source_path(resolved)
        .is_some_and(|path| path.is_file())
    {
        let listing = root.join(game.overlay_assembly(&overlay));
        let text = std::fs::read_to_string(&listing)
            .map_err(|error| format!("{}: {error}", listing.display()))?;
        crate::overlay::park::placeholder_block(&text.lines().collect::<Vec<_>>(), address)
            .and_then(|row| usize::try_from(row.span).ok())
    } else {
        None
    };
    let span = resolve_overlay_span(
        &crate::overlay::owners::owner_spans(root, game)?,
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
        source_for(root, &paths, resolved, options.target)?
    };
    let work = root.join(
        options
            .work
            .as_deref()
            .ok_or("overlay score needs a work directory")?,
    );
    std::fs::create_dir_all(&work).map_err(|error| error.to_string())?;
    let reference = work.join("reference-overlay.bin");
    let image = crate::overlay::rom::canonical_overlay_for(root, game, &overlay)?;
    std::fs::write(&reference, image).map_err(|error| error.to_string())?;
    let units = TranslationUnits::load_game(root, options.target)?;
    options.source = source.to_string_lossy().into_owned();
    options.configuration.call_via_base = Some(
        paths
            .registered_call_via(resolved)
            .map(u64::from)
            .unwrap_or_else(|| overlay_call_via_base(&overlay)),
    );
    options.configuration.overlay_extent = Some(span);
    if let Some(unit) = units.unit_for_game_owner(options.target.as_str(), resolved) {
        if unit.instance_owner(&overlay, resolved.address()).is_some() {
            return Err(format!(
                "{} is an instance owner of unit {}; score it through its unit with alchemy score --unit {} --instance {overlay}",
                resolved.id(),
                unit.id,
                unit.id
            ));
        }
        options.configuration.absolute_symbols = unit.canonical_symbols()?;
    }
    options.rom = Some(reference.to_string_lossy().into_owned());
    options.owner = Some(address as u32);
    options.overlay = Some(overlay);
    options.size = Some(span);
    let mut rendered = render(root, &options)?;
    if options.target == CompilerTarget::Tla {
        let next = if crate::score::exact_mismatch(&rendered) {
            format!(
                "next=alchemy score {:?} --target tla --owner {} --size {span} --work {:?} --first",
                options.source,
                resolved.id(),
                work
            )
        } else {
            format!("next=alchemy overlay adopt {} --source {:?} --span {span} --target tla-en (complete-owner and production verification still required)", resolved.id(), options.source)
        };
        rendered.stdout = rendered
            .stdout
            .lines()
            .map(|line| {
                if line.starts_with("next=") {
                    next.as_str()
                } else {
                    line
                }
            })
            .collect::<Vec<_>>()
            .join("\n")
            + "\n";
    }
    Ok(rendered)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn lost_age_scores_its_registered_source_and_reviewed_extent() {
        let root = crate::compiler::routing::root();
        if !root.join("roms/tla-en.gba").is_file() {
            return;
        }
        let args = [
            "resource_64d:02000038",
            "--target",
            "tla",
            "--work",
            "out/tla-en/score-route-test",
        ]
        .map(str::to_owned);
        let ParseOutcome::Options(options) = options_of(root, &args).unwrap() else {
            panic!("expected options");
        };
        let result = render_options(root, options).unwrap();
        assert_eq!(
            (
                result.candidate_length,
                result.reference_length,
                result.differing_halfwords
            ),
            (8, 8, 0)
        );
        assert!(result.stdout.contains("--target tla-en"));
        let args = ["resource_64d:02000038", "--target", "tla", "--size", "6"].map(str::to_owned);
        assert!(run(root, &args)
            .unwrap_err()
            .contains("complete installed extent 8"));
    }

    #[test]
    fn lost_age_rejects_unreviewed_extents_and_reference_overrides() {
        let root = crate::compiler::routing::root();
        let args = ["resource_64d:02000000", "--target", "tla", "--size", "56"].map(str::to_owned);
        assert!(run(root, &args)
            .unwrap_err()
            .contains("no reviewed complete owner boundary"));
        let args = [
            "resource_64d:02000038",
            "--target",
            "tla",
            "--rom",
            "roms/tbs-en.gba",
        ]
        .map(str::to_owned);
        assert!(run(root, &args)
            .unwrap_err()
            .contains("selected game's canonical reference"));
    }

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
}
