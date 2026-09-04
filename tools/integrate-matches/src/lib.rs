//! Main-image exact-C adoption gate.
//!
//! A candidate is adoptable only when its linked bytes equal the bytes produced
//! by the corresponding hand-written assembly at the same ROM address.
//! `--apply` performs the move and cleanup only after that proof.
pub mod retire;
use candidate_compiler::verify::run as run_command;
use candidate_compiler::{verify_candidate_owned_routed, CandidateCompilerConfiguration, ROM_BASE};
use compiler_core::plan::direct_preprocessor_command;
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use compiler_core::translation_units::TranslationUnits;
use no_asm_c::{find_forbidden, source_files};
use std::fs;
use std::path::{Path, PathBuf};
pub const USAGE: &str = "usage: integrate-matches [-h] [--apply|--check] directory";
pub const ROOT_OVERRIDE: &str = "ALCHEMY_INTEGRATE_ROOT";
#[derive(Default)]
struct PipelineReport {
    lines: Vec<String>,
    candidates: usize,
    already_installed: usize,
    accepted: usize,
    evidence_only: usize,
    byte_rejected: usize,
    unscored: usize,
    apply: bool,
}
impl PipelineReport {
    fn clean_for_check(&self) -> bool {
        !self.apply
            && self.candidates
                == self.already_installed
                    + self.accepted
                    + self.evidence_only
                    + self.byte_rejected
                    + self.unscored
            && self.accepted == 0
            && self.evidence_only == 0
            && self.candidates > 0
            && self.unscored == 0
    }
}
fn valid_address(stem: &str) -> bool {
    stem.len() == 8
        && stem.starts_with("08")
        && stem
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
}
fn candidate_stem(path: &Path) -> Option<&str> {
    let stem = path.file_stem()?.to_str()?;
    let stem = stem.strip_prefix("src_").unwrap_or(stem);
    (path.extension()?.to_str()? == "c" && valid_address(stem)).then_some(stem)
}
fn adoptable(forbidden: &str, address_mapped: bool) -> bool {
    forbidden.is_empty() && address_mapped
}
fn first_difference(left: &[u8], right: &[u8]) -> Option<usize> {
    left.iter()
        .zip(right)
        .position(|(left, right)| left != right)
        .or_else(|| (left.len() != right.len()).then_some(left.len().min(right.len())))
}
fn assembly_extent(stem: &str, source: &Path, scratch: &Path) -> Result<usize, String> {
    fs::create_dir_all(scratch).map_err(|error| format!("{}: {error}", scratch.display()))?;
    let prefix = scratch.join(format!("{stem}.reference"));
    let prefix = prefix.to_string_lossy().into_owned();
    let listing = format!("{prefix}.s");
    let object = format!("{prefix}.o");
    let binary = format!("{prefix}.bin");
    fs::copy(source, &listing).map_err(|error| format!("{}: {error}", source.display()))?;
    run_command(
        &[
            "arm-none-eabi-as",
            "-mcpu=arm7tdmi",
            "-mthumb-interwork",
            "-o",
            &object,
            &listing,
        ],
        root(),
    )?;
    run_command(
        &[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &object,
            &binary,
        ],
        root(),
    )?;
    fs::metadata(&binary)
        .map(|metadata| metadata.len() as usize)
        .map_err(|error| format!("{binary}: {error}"))
}
fn root_directory() -> PathBuf {
    std::env::var_os(ROOT_OVERRIDE)
        .filter(|value| !value.is_empty())
        .map(PathBuf::from)
        .unwrap_or_else(|| root().to_path_buf())
}
fn close_dossier(path: &Path, state: &str) -> Result<bool, String> {
    if !path.exists() {
        return Ok(false);
    }
    let source =
        fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let state_start = source
        .starts_with("State:")
        .then_some(0)
        .or_else(|| source.find("\nState:").map(|offset| offset + 1));
    let updated = if let Some(start) = state_start {
        let end = source[start..]
            .find(['\r', '\n'])
            .map_or(source.len(), |end| start + end);
        format!("{}{}{}", &source[..start], state, &source[end..])
    } else if source.starts_with('#') {
        source.find('\n').map_or_else(
            || source.clone(),
            |end| format!("{}\n{}\n{}", &source[..end + 1], state, &source[end + 1..]),
        )
    } else {
        source
    };
    fs::write(path, updated).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(true)
}
fn cleanup(stem: &str, work_root: &Path, date: &str) -> Result<(usize, bool), String> {
    let mut removed = 0;
    if work_root.exists() {
        let prefix = format!("{stem}.");
        for entry in
            fs::read_dir(work_root).map_err(|error| format!("{}: {error}", work_root.display()))?
        {
            let entry = entry.map_err(|error| format!("{}: {error}", work_root.display()))?;
            if entry
                .file_type()
                .map_err(|error| error.to_string())?
                .is_file()
                && entry.file_name().to_string_lossy().starts_with(&prefix)
            {
                fs::remove_file(entry.path())
                    .map_err(|error| format!("{}: {error}", entry.path().display()))?;
                removed += 1;
            }
        }
    }
    let hand = work_root.join("hand").join(stem);
    if hand.exists() {
        fs::remove_dir_all(&hand).map_err(|error| format!("{}: {error}", hand.display()))?;
        removed += 1;
    }
    let dossier = work_root.join("walls").join(format!("{stem}.md"));
    let closed = close_dossier(
        &dossier,
        &format!("State: CLOSED — {date}. Installed by `tools/integrate-matches`."),
    )?;
    Ok((removed, closed))
}
fn today_utc() -> String {
    let days = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |duration| duration.as_secs() / 86_400) as i64;
    let z = days + 719_468;
    let era = (if z >= 0 { z } else { z - 146_096 }) / 146_097;
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let y = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let month = (5 * doy + 2) / 153;
    let day = doy - (153 * month + 2) / 5 + 1;
    let month = if month < 10 { month + 3 } else { month - 9 };
    let year = if month <= 2 { y + 1 } else { y };
    format!("{year:04}-{month:02}-{day:02}")
}
fn run_pipeline(directory: &str, apply: bool) -> Result<PipelineReport, String> {
    let repository = root_directory();
    let source_paths = SourcePaths::load(&repository)?;
    let units = TranslationUnits::load(&repository)?;
    let directory = PathBuf::from(directory);
    let directory = if directory.is_absolute() {
        directory
    } else {
        repository.join(directory)
    };
    if !directory.is_dir() {
        return Err(format!("{}: not a directory", directory.display()));
    }
    // Pure per-candidate compile scratch: consumed only within this same
    // loop iteration to compute a score, so it lives under system temp
    // rather than the tracked repo tree (this used to leave thousands of
    // files under scratch/integrate-gate that no cleanup pass ever reached).
    let gate = std::env::temp_dir().join("alchemy-integrate-gate");
    fs::create_dir_all(&gate).map_err(|error| format!("{}: {error}", gate.display()))?;
    let candidates = source_files(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?
        .into_iter()
        .filter_map(|path| {
            let stem = candidate_stem(&path)?.to_string();
            Some((stem, path))
        })
        .collect::<Vec<_>>();
    let candidate_count = candidates.len();
    let rom_path = repository.join("roms/gs1-en.gba");
    let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    let mut accepted = Vec::new();
    let mut evidence = Vec::new();
    let mut byte_rejected = Vec::new();
    let mut unscored = Vec::new();
    let mut already_installed = 0;
    for (stem, candidate) in candidates {
        let owner = SourceOwner::parse(&format!("main:{stem}"))?;
        let asm = repository.join("games/gs1/asm").join(format!("{stem}.s"));
        if source_paths.source_path(owner).exists() {
            if asm.exists() {
                unscored.push((stem, "installed C still has retained assembly".into()));
            } else {
                already_installed += 1;
            }
            continue;
        }
        let source_bytes = fs::read(&candidate).map_err(|error| error.to_string())?;
        let source = String::from_utf8_lossy(&source_bytes);
        let mut forbidden = find_forbidden(&candidate.to_string_lossy(), &source)
            .into_iter()
            .map(|finding| format!("{} at line {}", finding.token, finding.line))
            .collect::<Vec<_>>()
            .join(", ");
        let mapped = source_paths.registered_source_path(owner);
        if !asm.exists() {
            unscored.push((stem, "no retained assembly owner".into()));
            continue;
        }
        let result = (|| {
            let extent = assembly_extent(&stem, &asm, &gate)?;
            // Flags route by the owner's address-stem routing path, exactly as
            // the production standalone compile does; a descriptive mapped
            // path must not change the owner's recorded flag family.
            let route = repository.join(owner.routing_path());
            // A candidate inside a declared reconstruction-composition unit
            // links with that unit's manifest symbols, exactly as the unit
            // compile does.
            let mut configuration = CandidateCompilerConfiguration::default();
            if let Some(unit) = units.units.iter().find(|unit| {
                unit.overlay.is_none()
                    && unit
                        .owners
                        .iter()
                        .any(|member| member.address == owner.address())
            }) {
                configuration.absolute_symbols = unit.canonical_symbols()?;
            }
            let verified = verify_candidate_owned_routed(
                &candidate.to_string_lossy(),
                &route.to_string_lossy(),
                &stem,
                &rom,
                &gate.to_string_lossy(),
                &[],
                ROM_BASE,
                CompilerTarget::Gs1,
                &configuration,
            )?;
            let difference = first_difference(&verified.expected, &verified.actual)
                .or_else(|| (verified.actual.len() != extent).then_some(verified.actual.len()));
            if let Some(offset) = difference {
                byte_rejected.push((
                    stem.clone(),
                    format!(
                        "bytes differ at +0x{offset:x} (asm={}B c={}B)",
                        extent,
                        verified.actual.len()
                    ),
                ));
            } else {
                let preprocessed = gate.join(format!("{stem}.ordinary.i"));
                run_command(
                    &direct_preprocessor_command(
                        &candidate.to_string_lossy(),
                        &preprocessed.to_string_lossy(),
                    )?,
                    root(),
                )?;
                let expanded = fs::read_to_string(&preprocessed)
                    .map_err(|error| format!("{}: {error}", preprocessed.display()))?;
                if forbidden.is_empty() {
                    forbidden = find_forbidden(&preprocessed.to_string_lossy(), &expanded)
                        .into_iter()
                        .map(|finding| format!("{} after preprocessing", finding.token))
                        .collect::<Vec<_>>()
                        .join(", ");
                }
                if adoptable(&forbidden, mapped.is_ok()) {
                    accepted.push((stem.clone(), extent, candidate.clone()));
                    return Ok(());
                }
                let reason = if forbidden.is_empty() {
                    mapped.unwrap_err()
                } else {
                    format!("nonordinary C: {forbidden}")
                };
                evidence.push((stem.clone(), extent, reason));
            }
            Ok::<(), String>(())
        })();
        if let Err(error) = result {
            unscored.push((stem, error));
        }
    }
    let mut lines = accepted
        .iter()
        .map(|(stem, size, _)| format!("accept {stem} ({size}B)"))
        .collect::<Vec<_>>();
    lines.extend(
        evidence
            .iter()
            .map(|(stem, size, reason)| format!("evidence-only {stem} ({size}B exact): {reason}")),
    );
    if apply {
        let date = today_utc();
        for (stem, _, candidate) in &accepted {
            let owner = SourceOwner::parse(&format!("main:{stem}"))?;
            let exact = source_paths.registered_source_path(owner)?;
            if let Some(parent) = exact.parent() {
                fs::create_dir_all(parent)
                    .map_err(|error| format!("{}: {error}", parent.display()))?;
            }
            fs::copy(candidate, &exact).map_err(|error| format!("{}: {error}", exact.display()))?;
            let asm = repository.join("games/gs1/asm").join(format!("{stem}.s"));
            if asm.exists() {
                fs::remove_file(&asm).map_err(|error| format!("{}: {error}", asm.display()))?;
            }
            let (scratch_removed, scratch_closed) =
                cleanup(stem, &repository.join("scratch"), &date)?;
            let (draft_removed, draft_closed) = cleanup(stem, &repository.join("draft"), &date)?;
            let removed = scratch_removed + draft_removed;
            let closed = scratch_closed || draft_closed;
            if removed > 0 || closed {
                lines.push(format!(
                    "clean {stem} scratch={} wall={}",
                    removed,
                    if closed { "closed" } else { "absent" }
                ));
            }
        }
    }
    lines.extend(
        byte_rejected
            .iter()
            .map(|(stem, reason)| format!("reject {stem}: {reason}")),
    );
    lines.extend(
        unscored
            .iter()
            .map(|(stem, reason)| format!("unscored {stem}: {reason}")),
    );
    lines.push(format!(
        "candidates={} already_installed={} accepted={} evidence_only={} byte_rejected={} unscored={}{}",
        candidate_count,
        already_installed,
        accepted.len(),
        evidence.len(),
        byte_rejected.len(),
        unscored.len(),
        if apply { " (applied)" } else { " (dry run)" }
    ));
    Ok(PipelineReport {
        lines,
        candidates: candidate_count,
        already_installed,
        accepted: accepted.len(),
        evidence_only: evidence.len(),
        byte_rejected: byte_rejected.len(),
        unscored: unscored.len(),
        apply,
    })
}
fn parse_arguments(arguments: &[String]) -> Result<Option<(String, bool, bool)>, String> {
    if arguments
        .iter()
        .any(|arg| matches!(arg.as_str(), "-h" | "--help"))
    {
        return Ok(None);
    }
    let apply = arguments.iter().any(|arg| arg == "--apply");
    let check = arguments.iter().any(|arg| arg == "--check");
    if apply && check {
        return Err("--apply and --check cannot be combined".into());
    }
    if let Some(arg) = arguments
        .iter()
        .find(|arg| arg.starts_with('-') && !matches!(arg.as_str(), "--apply" | "--check"))
    {
        return Err(format!("unrecognized argument: {arg}"));
    }
    let directories = arguments
        .iter()
        .filter(|arg| !matches!(arg.as_str(), "--apply" | "--check"))
        .collect::<Vec<_>>();
    match directories.as_slice() {
        [directory] => Ok(Some(((*directory).clone(), apply, check))),
        [] => Err("the following arguments are required: directory".into()),
        [_, extra, ..] => Err(format!("unrecognized argument: {extra}")),
    }
}
pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let result = match parse_arguments(arguments) {
        Ok(None) => {
            println!("{USAGE}");
            return std::process::ExitCode::SUCCESS;
        }
        Ok(Some((directory, apply, check))) => run_pipeline(&directory, apply).and_then(|report| {
            let clean = report.clean_for_check();
            for line in report.lines {
                println!("{line}");
            }
            if check && !clean {
                Err("candidate corpus contains exact retained, evidence-only, or unscored C".into())
            } else {
                Ok(())
            }
        }),
        Err(error) => Err(error),
    };
    if let Err(error) = result {
        eprintln!("{error}");
        std::process::ExitCode::FAILURE
    } else {
        std::process::ExitCode::SUCCESS
    }
}
#[cfg(test)]
mod tests {
    use super::{adoptable, candidate_stem, PipelineReport};
    use std::path::Path;
    #[test]
    fn discovers_scratch_and_recursive_reconstruction_names() {
        assert_eq!(
            candidate_stem(Path::new("src_08001234.c")),
            Some("08001234")
        );
        assert_eq!(
            candidate_stem(Path::new("nested/0800abcd.c")),
            Some("0800abcd")
        );
        assert_eq!(candidate_stem(Path::new("resource_370_c_02000030.c")), None);
        assert!(adoptable("", true));
        assert!(!adoptable("ABI attribute naked", true));
        assert!(!adoptable("", false));
    }
    #[test]
    fn corpus_check_requires_complete_accounting() {
        let clean = PipelineReport {
            candidates: 1,
            already_installed: 1,
            ..Default::default()
        };
        assert!(clean.clean_for_check());
        assert!(!PipelineReport {
            unscored: 1,
            ..clean
        }
        .clean_for_check());
    }
}
