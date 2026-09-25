//! `alchemy overlay adopt`: replace one reviewed overlay owner's retained
//! assembly with an `AlchemyC_` placeholder filled by exact C.
//!
//! Every path the adoption reads or writes comes from one target: the
//! game's reviewed register, source-path register, retained assembly, and
//! the ROM the rebuilt overlay must equal.

use crate::compiler::no_asm::{expanded_forbidden, find_forbidden};
use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::overlay::assembly::OVERLAY_BASE;
use crate::overlay::compile::assemble_overlay;
use crate::overlay::source::OverlaySource;
use crate::overlay::{internal_aliases, listing_offsets, placeholder_lines, region_lines, rom};
use crate::targets::{decomp_target, DecompTarget};
use std::fs;
use std::path::{Path, PathBuf};

#[derive(Debug, Clone)]
pub struct Options {
    pub span: Option<i64>,
    pub id: String,
    pub source: String,
    pub target: DecompTarget,
    pub apply: bool,
    pub where_: bool,
}

const USAGE: &str = "usage: alchemy overlay adopt <overlay:offsetHex> --source FILE [--span BYTES] [--target tbs-en|tla-en] [--apply] [--where]";

fn options_of(argv: &[String]) -> Result<Option<Options>, String> {
    let (mut span, mut id, mut source, mut target) = (None, String::new(), String::new(), None);
    let (mut apply, mut where_) = (false, false);
    let mut args = argv.iter();
    while let Some(argument) = args.next() {
        match argument.as_str() {
            "--source" | "-s" => source = args.next().cloned().unwrap_or_default(),
            "--span" => {
                span = Some(
                    args.next()
                        .and_then(|value| value.parse::<i64>().ok())
                        .filter(|value| *value > 0)
                        .ok_or("--span must be a positive byte count")?,
                )
            }
            "--target" => target = Some(args.next().ok_or("--target requires a target id")?),
            "--apply" => apply = true,
            "--where" => where_ = true,
            "-h" | "--help" => return Ok(None),
            _ if id.is_empty() => id = argument.clone(),
            _ => return Err(format!("unrecognized argument: {argument}")),
        }
    }
    if id.is_empty() || source.is_empty() {
        return Err("both an overlay function id and --source are required".to_string());
    }
    let target = decomp_target(target.map(String::as_str))?;
    let production = crate::overlay::owners::production_target(target.compiler);
    if target.id != production.id {
        return Err(format!(
            "{} overlays are retained for {}; adopt against that target, not {}",
            target.game_dir(),
            production.id,
            target.id
        ));
    }
    Ok(Some(Options {
        span,
        id,
        source,
        target,
        apply,
        where_,
    }))
}

pub(crate) fn audited_span(
    root: &Path,
    target: DecompTarget,
    owner: SourceOwner,
    span_bytes: i64,
) -> Result<(), String> {
    crate::compiler::translation_units::resolve_overlay_span(
        &crate::overlay::owners::reviewed_spans(root, target)?,
        owner,
        None,
        Some(usize::try_from(span_bytes).map_err(|_| "invalid overlay span")?),
    )?;
    Ok(())
}

struct OverlayLock {
    path: PathBuf,
}
impl OverlayLock {
    fn acquire(assembly: &Path, target: DecompTarget) -> Result<Self, String> {
        let path = assembly.with_extension("s.adopt-lock");
        for attempt in 0..600 {
            match fs::OpenOptions::new()
                .write(true)
                .create_new(true)
                .open(&path)
            {
                Ok(_) => return Ok(OverlayLock { path }),
                Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {
                    if attempt == 0 {
                        eprintln!(
                            "waiting for another adoption to finish with {}",
                            assembly.display()
                        );
                    }
                    std::thread::sleep(std::time::Duration::from_millis(100));
                }
                Err(error) => return Err(format!("{}: {error}", path.display())),
            }
        }
        Err(format!(
            "{} is still locked after 60s. If no other adoption is running, a \
             previous one was killed mid-splice: check `git status {}/` \
             before deleting the lock file.",
            path.display(),
            target.overlay_dir()
        ))
    }
}
impl Drop for OverlayLock {
    fn drop(&mut self) {
        let _ = fs::remove_file(&self.path);
    }
}

fn revert(
    installed: &Path,
    assembly: &Path,
    preexisting: &Option<Vec<u8>>,
    original_text: &str,
) -> Result<(), String> {
    match preexisting {
        Some(data) => fs::write(installed, data).map_err(|error| error.to_string())?,
        None => {
            let _ = fs::remove_file(installed);
        }
    }
    fs::write(assembly, original_text).map_err(|error| error.to_string())
}

fn differing_runs(actual: &[u8], expected: &[u8]) -> String {
    let mut runs = Vec::new();
    let mut start = None;
    let mut previous = 0;
    for offset in
        (0..actual.len().min(expected.len())).filter(|offset| actual[*offset] != expected[*offset])
    {
        if start.is_some() && offset != previous + 1 {
            runs.push((start.take().unwrap(), previous));
        }
        start.get_or_insert(offset);
        previous = offset;
    }
    if let Some(start) = start {
        runs.push((start, previous));
    }
    if runs.is_empty() {
        return "(none; length change only)".to_string();
    }
    runs.into_iter()
        .map(|(start, end)| format!("0x{:x}+{}", OVERLAY_BASE + start as i64, end - start + 1))
        .collect::<Vec<_>>()
        .join(" ")
}

/// Thirty fresh compiles must agree before installation. The Broken Seal
/// check runs through `alchemy score`; that route scores overlay owners
/// against the Broken Seal ROM only, so other games compile the installed
/// source in-process against their own overlay image, uncached, under the
/// same compiler pin.
fn repeatable(
    root: &Path,
    target: DecompTarget,
    installed: &Path,
    id: &str,
    owner: SourceOwner,
    span: i64,
    baseline: &[u8],
) -> Result<String, String> {
    let width = u32::try_from(span).map_err(|_| format!("{id}: span exceeds u32"))?;
    if target.compiler == CompilerTarget::Tbs {
        return crate::recovery::repeatable(root, installed, id, width);
    }
    const RUNS: usize = 30;
    let overlay = owner.overlay_id().ok_or("expected an overlay owner")?;
    // A member of a translation unit repeats the unit compile the production
    // listing links; a standalone owner compiles its own source.
    let unit =
        crate::compiler::translation_units::TranslationUnits::load_game(root, target.compiler)?
            .unit_for_game_owner(target.compiler.as_str(), owner)
            .cloned();
    let offset = crate::overlay::overlay_offset(owner);
    let expected = baseline
        .get(offset..offset + span as usize)
        .ok_or_else(|| format!("{id}: owner lies outside the overlay image"))?;
    let cc1 = crate::compiler::routing::bundle_for(target.compiler).join("cc1");
    let pin = || -> Result<String, String> {
        let bytes = fs::read(&cc1).map_err(|error| format!("{}: {error}", cc1.display()))?;
        Ok(crate::compiler::sha256::hex(&bytes))
    };
    let before = pin()?;
    let workers = std::thread::available_parallelism().map_or(1, |n| n.get().min(RUNS));
    let next = std::sync::atomic::AtomicUsize::new(0);
    let exact = std::thread::scope(|scope| {
        (0..workers)
            .map(|_| {
                scope.spawn(|| {
                    let mut exact = 0;
                    while next.fetch_add(1, std::sync::atomic::Ordering::Relaxed) < RUNS {
                        let work = tempfile::tempdir().map_err(|error| error.to_string())?;
                        let compiled = match &unit {
                            Some(unit) => {
                                crate::overlay::compile::compile_overlay_unit_fresh(unit)?
                                    .into_iter()
                                    .find(|member| member.address == i64::from(owner.address()))
                                    .ok_or_else(|| {
                                        format!("{id}: unit {} did not place it", unit.id)
                                    })?
                            }
                            None => crate::overlay::compile::compile_overlay_c_fresh(
                                target,
                                installed,
                                work.path(),
                                &overlay,
                                span as usize,
                            )?,
                        };
                        exact += usize::from(compiled.data == expected);
                    }
                    Ok::<usize, String>(exact)
                })
            })
            .collect::<Vec<_>>()
            .into_iter()
            .map(|handle| {
                handle
                    .join()
                    .map_err(|_| "repeat worker panicked".to_string())?
            })
            .sum::<Result<usize, String>>()
    })?;
    let after = pin()?;
    if before != after {
        return Err(format!(
            "{id}: the compiler changed during the repeat check ({} then {}); nothing adopted -- rerun once the bundle is still",
            &before[..12],
            &after[..12]
        ));
    }
    if exact != RUNS {
        return Err(format!(
            "{id}: exact in {exact} of {RUNS} compiles; nothing adopted. A split is a toolchain defect to diagnose, not bad luck to retry"
        ));
    }
    Ok(format!(
        "repeatability runs={RUNS} exact={exact} compiler={}",
        &before[..12]
    ))
}

/// An owner whose registered source other owners share adopts only that
/// source, byte for byte. When the source is a module linked into several
/// images, the owner must also be declared where its unit links it, as a
/// canonical owner or an instance member, so it never compiles on its own.
fn shared_source_guard(
    root: &Path,
    source_paths: &SourcePaths,
    owner: SourceOwner,
    installed: &Path,
    candidate: &[u8],
) -> Result<(), String> {
    let shared = source_paths
        .owners_for_path(installed)
        .into_iter()
        .any(|registered| registered != owner);
    if !shared {
        return Ok(());
    }
    if fs::read(installed).ok().as_deref() != Some(candidate) {
        return Err(format!(
            "{} is shared by other exact owners; refusing to overwrite it with differing source",
            installed.display()
        ));
    }
    if !source_paths.links_module_path(installed) {
        return Ok(());
    }
    let relative = installed.strip_prefix(root).unwrap_or(installed);
    let declared = crate::compiler::translation_units::TranslationUnits::declared(root)?
        .units
        .iter()
        .any(|unit| unit.source == relative && unit.declares(&owner.image(), owner.address()));
    if declared {
        return Ok(());
    }
    Err(format!(
        "{} is linked into several images; declare {} in the instances of its translation unit before adopting it",
        installed.display(),
        owner.id()
    ))
}

pub fn run(root: &Path, args: &[String]) -> Result<i32, String> {
    let Some(options) = options_of(args)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    let target = options.target;
    let source_text = fs::read_to_string(&options.source)
        .map_err(|error| format!("{}: {error}", options.source))?;
    let mut forbidden = find_forbidden(&options.source, &source_text)
        .into_iter()
        .map(|finding| format!("{}:{}", finding.token, finding.line))
        .collect::<Vec<_>>()
        .join(",");
    let (overlay, address) = options
        .id
        .split_once(':')
        .ok_or_else(|| format!("unparseable overlay id: {}", options.id))?;
    let entry = i64::from_str_radix(address.trim_start_matches("0x"), 16)
        .map_err(|_| format!("unparseable overlay id: {}", options.id))?;
    let offset = if entry >= OVERLAY_BASE {
        entry - OVERLAY_BASE
    } else {
        entry
    };
    let entry = OVERLAY_BASE + offset;
    let span = options
        .span
        .ok_or("--span BYTES is required for overlay adoption")?;
    let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
    audited_span(root, target, owner, span)?;
    // Every overlay adoption, direct or through `alchemy adopt`, passes here:
    // an equivalent twin left behind or copied refuses before anything moves.
    // The sibling census reads The Broken Seal's images and register.
    if target.compiler == CompilerTarget::Tbs {
        let extent = usize::try_from(span).map_err(|_| format!("{}: invalid span", options.id))?;
        for line in crate::siblings::guard(root, &[(owner, extent)])? {
            println!("{line}");
        }
    }
    let source_paths = SourcePaths::load_for_game(root, target.compiler.as_str())?;
    let installed = source_paths.registered_source_path(owner)?;
    let stem = owner.address_stem();
    let assembly = root.join(target.overlay_assembly(overlay));
    let _lock = OverlayLock::acquire(&assembly, target)?;
    // New TU declarations precede their C placeholders during adoption.
    // Compare the completed overlay with the ROM, not that transitional tree.
    let baseline = rom::canonical_overlay_for(root, target, overlay)?;
    let original_text = fs::read_to_string(&assembly).map_err(|error| error.to_string())?;
    let lines: Vec<String> = original_text
        .split('\n')
        .map(|line| line.to_string())
        .collect();
    let offsets = listing_offsets(&assembly)?;
    let (first, last) = region_lines(&offsets, offset, span)?;
    let marker = format!("AlchemyC_{stem}:");
    if lines.iter().any(|line| line == &marker) {
        return Err(format!("{} is already adopted as C", options.id));
    }
    let aliases = internal_aliases(&lines, first, last, offset, span)?;
    let mut replaced_lines: Vec<String> = Vec::with_capacity(lines.len());
    replaced_lines.extend(lines[..(first - 1) as usize].iter().cloned());
    replaced_lines.extend(placeholder_lines(&stem, span, &aliases));
    replaced_lines.extend(lines[last as usize..].iter().cloned());
    let replaced = replaced_lines.join("\n");
    let preexisting = if installed.exists() {
        Some(fs::read(&installed).map_err(|error| error.to_string())?)
    } else {
        None
    };
    let candidate = fs::read(&options.source).map_err(|error| error.to_string())?;
    shared_source_guard(root, &source_paths, owner, &installed, &candidate)?;
    let source_is_installed = fs::canonicalize(&options.source)
        .ok()
        .zip(fs::canonicalize(&installed).ok())
        .is_some_and(|(source, destination)| source == destination);
    let rebuild: Result<Vec<u8>, String> = (|| {
        if let Some(parent) = installed.parent() {
            fs::create_dir_all(parent).map_err(|error| error.to_string())?;
        }
        if !source_is_installed {
            fs::copy(&options.source, &installed).map_err(|error| error.to_string())?;
        }
        fs::write(&assembly, &replaced).map_err(|error| error.to_string())?;
        assemble_overlay(&OverlaySource::path(&assembly), OVERLAY_BASE)
    })();
    let rebuilt = match rebuild {
        Ok(data) => data,
        Err(error) => {
            revert(&installed, &assembly, &preexisting, &original_text)?;
            return Err(error);
        }
    };
    if rebuilt.len() != baseline.len() || rebuilt != baseline {
        let differing = psynergy::compare::differing_offsets(&rebuilt, &baseline, 1).len();
        revert(&installed, &assembly, &preexisting, &original_text)?;
        println!(
            "adopt=rejected {} differing_bytes={} size={}/{}",
            options.id,
            differing,
            rebuilt.len(),
            baseline.len()
        );
        if options.where_ {
            println!("differing_at {}", differing_runs(&rebuilt, &baseline));
        }
        return Ok(1);
    }
    if forbidden.is_empty() {
        forbidden = match expanded_forbidden(root, &installed) {
            Ok(forbidden) => forbidden,
            Err(error) => {
                revert(&installed, &assembly, &preexisting, &original_text)?;
                return Err(error);
            }
        };
    }
    if !forbidden.is_empty() {
        revert(&installed, &assembly, &preexisting, &original_text)?;
        println!(
            "adopt=evidence-only {} exact_bytes={} forbidden={} source_retained=true",
            options.id, span, forbidden
        );
        return Ok(1);
    }
    // One exact assembly is a sample; installation needs thirty fresh
    // compiles to agree. The check runs here because every overlay
    // adoption, direct or through `alchemy adopt`, passes through this apply.
    if options.apply {
        match repeatable(
            root,
            target,
            &installed,
            &options.id,
            owner,
            span,
            &baseline,
        ) {
            Ok(line) => println!("{line}"),
            Err(error) => {
                revert(&installed, &assembly, &preexisting, &original_text)?;
                return Err(error);
            }
        }
    }
    if !options.apply {
        revert(&installed, &assembly, &preexisting, &original_text)?;
        let source_base = crate::compiler::plan::basename(&options.source);
        println!(
            "adopt=ready {} span={} aliases={} lines={}-{} source={} (pass --apply to install)",
            options.id,
            span,
            aliases.len(),
            first,
            last,
            source_base
        );
        return Ok(0);
    }
    println!(
        "adopt=applied {} span={} aliases={} c={}",
        options.id,
        span,
        aliases.len(),
        source_paths.repository_relative_path(owner).display()
    );
    Ok(0)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::compiler::translation_units::fixture::{Repository, FIND};
    use crate::targets::DecompTargetId;
    use serde_json::json;

    fn arguments(values: &[&str]) -> Vec<String> {
        values.iter().map(|value| value.to_string()).collect()
    }

    #[test]
    fn target_defaults_to_the_broken_seal_and_routes_the_lost_age() {
        let default = options_of(&arguments(&["resource_36f:1c0", "--source", "a.c"]))
            .unwrap()
            .unwrap();
        assert_eq!(default.target.id, DecompTargetId::TbsEn);
        let lost_age = options_of(&arguments(&[
            "resource_64e:80",
            "--source",
            "a.c",
            "--target",
            "tla-en",
        ]))
        .unwrap()
        .unwrap();
        assert_eq!(lost_age.target.id, DecompTargetId::TlaEn);
        assert_eq!(lost_age.target.game_dir(), "games/THE LOST AGE");
    }

    #[test]
    fn only_the_retained_edition_can_adopt() {
        let error = options_of(&arguments(&[
            "resource_64e:80",
            "--source",
            "a.c",
            "--target",
            "tla-ja",
        ]))
        .unwrap_err();
        assert!(error.contains("tla-en"), "{error}");
    }

    #[test]
    fn adopting_a_linked_module_owner_requires_identical_source_and_instance_record() {
        let repository = Repository::new();
        let root = repository.0.path();
        let owner = |id: &str| SourceOwner::parse(id).unwrap();
        let guard = |id: &str, candidate: &[u8]| {
            let names = SourcePaths::load(root).unwrap();
            let owner = owner(id);
            let installed = names.registered_source_path(owner).unwrap();
            shared_source_guard(root, &names, owner, &installed, candidate)
        };
        // A declared instance member adopts only the shared source itself.
        guard("resource_39b:02000630", b"\n").unwrap();
        let error = guard("resource_39b:02000630", b"void f(void) {}\n").unwrap_err();
        assert!(error.contains("is shared by other exact owners"), "{error}");
        // An owner of the module that no unit declares in its image is refused.
        let record = json!({"name": FIND, "source": "FIELD/COMMON/OBJECT/STAGED_ACTOR.C"});
        repository.record("resource_39c:02000630", record);
        let error = guard("resource_39c:02000630", b"\n").unwrap_err();
        assert!(
            error.contains("STAGED_ACTOR.C is linked into several images; declare resource_39c:02000630 in the instances of its translation unit before adopting it"),
            "{error}"
        );
        // Related overlays loaded at one address keep their standalone route.
        repository.write("games/THE BROKEN SEAL/SRC/FIELD/KORIMA/SHARED.C", "\n");
        for id in ["resource_392:02000100", "resource_393:02000100"] {
            repository.record(id, json!("FIELD/KORIMA/SHARED.C"));
        }
        guard("resource_393:02000100", b"\n").unwrap();
        // An owner nothing else shares needs neither.
        repository.write("games/THE BROKEN SEAL/SRC/FIELD/KORIMA/ALONE.C", "\n");
        repository.record("resource_394:02000100", json!("FIELD/KORIMA/ALONE.C"));
        guard("resource_394:02000100", b"void f(void) {}\n").unwrap();
    }
}
