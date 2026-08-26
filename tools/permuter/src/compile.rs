use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

use candidate_compiler::{verify_candidate_owned_routed, CandidateCompilerConfiguration, ROM_BASE};
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::{assemble_overlay, compile_overlay_c, OverlaySource, OVERLAY_BASE};
use serde_json::Value;
use tempfile::tempdir;

#[derive(Clone, Debug)]
enum Kind {
    Main { rom: Vec<u8> },
    Overlay { name: String, address: i64 },
}

#[derive(Clone, Debug)]
pub struct Target {
    source: PathBuf,
    basename: String,
    owner: SourceOwner,
    expected: Vec<u8>,
    baseline: Score,
    kind: Kind,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Score {
    pub exact: bool,
    pub differing_halfwords: usize,
    pub expected_size: usize,
    pub actual_size: usize,
    pub first_difference: Option<usize>,
}

fn basename(path: &Path) -> Result<String, String> {
    path.file_name()
        .and_then(|name| name.to_str())
        .map(str::to_string)
        .ok_or_else(|| format!("{} has no UTF-8 filename", path.display()))
}

fn source_owner(path: &Path) -> Result<SourceOwner, String> {
    if let Some(owner) = SourcePaths::load(&root())?.owner_for_path(path)? {
        return Ok(owner);
    }
    let stem = path
        .file_stem()
        .and_then(|value| value.to_str())
        .unwrap_or("");
    SourceOwner::from_legacy_stem(stem)
        .ok_or_else(|| format!("{} is not registered to a source owner", path.display()))
}

fn local_flags(path: &Path) -> Vec<String> {
    path.parent()
        .map(|parent| vec![format!("-I{}", parent.display())])
        .unwrap_or_default()
}

fn read_json(path: &Path) -> Result<Value, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

fn hexadecimal(value: &Value) -> Option<u64> {
    value.as_u64().or_else(|| {
        value
            .as_str()
            .and_then(|text| u64::from_str_radix(text.trim_start_matches("0x"), 16).ok())
    })
}

fn overlay_span(name: &str, address: u32) -> Result<usize, String> {
    let path = root().join("games/gs1/semantic/regions.json");
    let document = read_json(&path)?;
    document["manual_regions"]
        .as_array()
        .into_iter()
        .flatten()
        .find(|region| {
            region["overlay"].as_str() == Some(name)
                && hexadecimal(&region["entry"]) == Some(u64::from(address))
        })
        .and_then(|region| region["span_bytes"].as_u64())
        .and_then(|span| usize::try_from(span).ok())
        .ok_or_else(|| {
            format!(
                "{name} 0x{address:08x} has no reviewed owner span in {}",
                path.display()
            )
        })
}

fn run(command: &[String], directory: &Path) -> Result<(), String> {
    let output = Command::new(&command[0])
        .args(&command[1..])
        .current_dir(root())
        .output()
        .map_err(|error| format!("cannot run {}: {error}", command[0]))?;
    if output.status.success() {
        return Ok(());
    }
    let detail = if output.stderr.is_empty() {
        &output.stdout
    } else {
        &output.stderr
    };
    Err(format!(
        "{} failed in {}: {}",
        command[0],
        directory.display(),
        String::from_utf8_lossy(detail).trim()
    ))
}

fn main_span(stem: &str, directory: &Path) -> Result<usize, String> {
    let assembly = root().join("games/gs1/asm").join(format!("{stem}.s"));
    if assembly.is_file() {
        let object = directory.join("reference.o");
        let binary = directory.join("reference.bin");
        run(
            &[
                "arm-none-eabi-as".into(),
                "-mcpu=arm7tdmi".into(),
                "-mthumb-interwork".into(),
                "-o".into(),
                object.to_string_lossy().into_owned(),
                assembly.to_string_lossy().into_owned(),
            ],
            directory,
        )?;
        run(
            &[
                "arm-none-eabi-objcopy".into(),
                "-O".into(),
                "binary".into(),
                "-j".into(),
                ".text".into(),
                object.to_string_lossy().into_owned(),
                binary.to_string_lossy().into_owned(),
            ],
            directory,
        )?;
        return fs::metadata(&binary)
            .map(|metadata| metadata.len() as usize)
            .map_err(|error| format!("{}: {error}", binary.display()));
    }
    let address = u64::from_str_radix(stem, 16).map_err(|error| error.to_string())?;
    for relative in [
        "out/gs1-en/full/claimed/manifest.json",
        "out/gs1-en/claimed/manifest.json",
    ] {
        let path = root().join(relative);
        let Ok(document) = read_json(&path) else {
            continue;
        };
        let size = document["regions"]
            .as_array()
            .into_iter()
            .flatten()
            .find_map(|region| {
                (hexadecimal(&region["address"]) == Some(address))
                    .then(|| region["size"].as_u64())
                    .flatten()
            });
        if let Some(size) = size {
            return usize::try_from(size).map_err(|error| error.to_string());
        }
    }
    Err(format!(
        "no assembly or claimed manifest records main owner {stem}"
    ))
}

fn window(bytes: &[u8], start: i64, length: usize) -> Result<Vec<u8>, String> {
    let start = usize::try_from(start).map_err(|_| "reference window starts before its image")?;
    bytes
        .get(start..start.saturating_add(length))
        .map(<[u8]>::to_vec)
        .ok_or_else(|| "reference owner span extends beyond its image".to_string())
}

fn score(actual: &[u8], expected: &[u8]) -> Score {
    let overlap = actual.len().min(expected.len());
    let differing_halfwords = actual[..overlap]
        .chunks(2)
        .zip(expected[..overlap].chunks(2))
        .filter(|(left, right)| left != right)
        .count()
        + actual.len().abs_diff(expected.len()).div_ceil(2);
    let first_difference = actual
        .iter()
        .zip(expected)
        .position(|(left, right)| left != right)
        .or_else(|| (actual.len() != expected.len()).then_some(overlap));
    Score {
        exact: actual == expected,
        differing_halfwords,
        expected_size: expected.len(),
        actual_size: actual.len(),
        first_difference,
    }
}

impl Target {
    pub fn prepare(path: &Path, source: &str) -> Result<Self, String> {
        let source_path = if path.is_absolute() {
            path.to_path_buf()
        } else {
            root().join(path)
        };
        let basename = basename(&source_path)?;
        let owner = source_owner(&source_path)?;
        let work = tempdir().map_err(|error| error.to_string())?;
        let candidate = work.path().join(&basename);
        fs::write(&candidate, source)
            .map_err(|error| format!("{}: {error}", candidate.display()))?;
        let (expected, actual, kind) = if let Some(name) = owner.overlay_id() {
            let compiled = compile_overlay_c(
                &candidate,
                work.path(),
                &name,
                Some(&owner.routing_path()),
                &local_flags(&source_path),
            )?;
            let reference = assemble_overlay(
                &OverlaySource::path(
                    root()
                        .join("games/gs1/assets/code")
                        .join(format!("{name}_overlay.s")),
                ),
                OVERLAY_BASE,
            )?;
            let span = overlay_span(&name, owner.address())?;
            let expected = window(&reference, compiled.address - OVERLAY_BASE, span)?;
            (
                expected,
                compiled.data,
                Kind::Overlay {
                    name,
                    address: compiled.address,
                },
            )
        } else {
            let rom_path = root().join("roms/gs1-en.gba");
            let rom =
                fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
            let stem = owner.address_stem();
            let verification = verify_candidate_owned_routed(
                &candidate.to_string_lossy(),
                &owner.routing_path().to_string_lossy(),
                &stem,
                &rom,
                &work.path().to_string_lossy(),
                &local_flags(&source_path),
                ROM_BASE,
                CompilerTarget::Gs1,
                &CandidateCompilerConfiguration::default(),
            )?;
            let expected = window(
                &rom,
                i64::from(owner.address()) - ROM_BASE as i64,
                main_span(&stem, work.path())?,
            )?;
            (expected, verification.actual, Kind::Main { rom })
        };
        let baseline = score(&actual, &expected);
        Ok(Self {
            source: source_path,
            basename,
            owner,
            expected,
            baseline,
            kind,
        })
    }

    pub fn baseline(&self) -> Score {
        self.baseline.clone()
    }

    pub fn compile(&self, source: &str) -> Result<Score, String> {
        let work = tempdir().map_err(|error| error.to_string())?;
        let candidate = work.path().join(&self.basename);
        fs::write(&candidate, source)
            .map_err(|error| format!("{}: {error}", candidate.display()))?;
        let actual = match &self.kind {
            Kind::Overlay { name, address } => {
                let compiled = compile_overlay_c(
                    &candidate,
                    work.path(),
                    name,
                    Some(&self.owner.routing_path()),
                    &local_flags(&self.source),
                )?;
                if compiled.address != *address {
                    return Err(format!(
                        "candidate linked at 0x{:08x}, expected 0x{address:08x}",
                        compiled.address
                    ));
                }
                compiled.data
            }
            Kind::Main { rom } => {
                verify_candidate_owned_routed(
                    &candidate.to_string_lossy(),
                    &self.owner.routing_path().to_string_lossy(),
                    &self.owner.address_stem(),
                    rom,
                    &work.path().to_string_lossy(),
                    &local_flags(&self.source),
                    ROM_BASE,
                    CompilerTarget::Gs1,
                    &CandidateCompilerConfiguration::default(),
                )?
                .actual
            }
        };
        Ok(score(&actual, &self.expected))
    }
}

pub fn self_test() -> Result<(), String> {
    let changed = score(&[1, 0, 2, 0], &[1, 0, 3, 0]);
    let short = score(&[1, 0], &[1, 0, 3, 0]);
    if changed.exact
        || changed.differing_halfwords != 1
        || changed.first_difference != Some(2)
        || short.differing_halfwords != 1
        || short.actual_size != 2
    {
        return Err("linked-byte scorer drifted".into());
    }
    Ok(())
}
