use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

use candidate_compiler::{verify_candidate_owned_routed, CandidateCompilerConfiguration, ROM_BASE};
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use overlay_disasm::{assemble_overlay, compile_overlay_candidate, OverlaySource, OVERLAY_BASE};

static TEMP_COUNTER: AtomicU64 = AtomicU64::new(0);

#[derive(Clone, Debug)]
enum Kind {
    Overlay { name: String, address: i64 },
    Core { rom: Vec<u8> },
}

#[derive(Clone, Debug)]
pub struct PreparedTarget {
    original: PathBuf,
    basename: String,
    owner: SourceOwner,
    expected: Vec<u8>,
    baseline: Score,
    baseline_assembly: Option<String>,
    kind: Kind,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Score {
    pub exact: bool,
    pub differing_halfwords: usize,
    pub expected_size: usize,
    pub actual_size: usize,
    pub first_difference: Option<usize>,
    pub actual: Vec<u8>,
}

struct TempDir(PathBuf);

impl TempDir {
    fn new(label: &str) -> Result<Self, String> {
        let time = SystemTime::now().duration_since(UNIX_EPOCH).map_err(|error| error.to_string())?.as_nanos();
        let count = TEMP_COUNTER.fetch_add(1, Ordering::Relaxed);
        let path = std::env::temp_dir().join(format!("permuter-{label}-{}-{time}-{count}", std::process::id()));
        fs::create_dir(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Ok(Self(path))
    }

    fn path(&self) -> &Path {
        &self.0
    }
}

impl Drop for TempDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

fn basename(path: &Path) -> Result<String, String> {
    path.file_name()
        .and_then(|name| name.to_str())
        .map(str::to_string)
        .ok_or_else(|| format!("{} has no UTF-8 filename", path.display()))
}

fn stem(path: &Path) -> Result<String, String> {
    let name = basename(path)?;
    Ok(name.strip_suffix(".c").unwrap_or(&name).to_string())
}

fn local_include_flags(path: &Path) -> Vec<String> {
    path.parent().map(|parent| vec![format!("-I{}", parent.display())]).unwrap_or_default()
}

fn source_owner(path: &Path) -> Result<SourceOwner, String> {
    let paths = SourcePaths::load(&root())?;
    if let Some(owner) = paths.owner_for_path(path)? {
        return Ok(owner);
    }
    let legacy = stem(path)?;
    SourceOwner::from_legacy_stem(&legacy)
        .ok_or_else(|| format!("{} is not registered to a source owner", path.display()))
}

fn overlay_name(stem: &str) -> Option<String> {
    let (prefix, address) = stem.split_once("_c_")?;
    if !prefix.starts_with("resource_")
        || prefix[9..].is_empty()
        || !prefix[9..].bytes().all(|byte| byte.is_ascii_hexdigit())
        || address.len() != 8
        || !address.bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        return None;
    }
    Some(prefix.to_string())
}

fn slice_clamped(bytes: &[u8], start: i64, length: usize) -> Vec<u8> {
    let start = start.max(0) as usize;
    if start >= bytes.len() {
        return Vec::new();
    }
    bytes[start..start.saturating_add(length).min(bytes.len())].to_vec()
}

fn quoted_field(line: &str, key: &str) -> Option<String> {
    let (_, value) = line.trim().split_once(':')?;
    line.trim()
        .starts_with(&format!("\"{key}\""))
        .then(|| value.trim().trim_end_matches(',').trim_matches('"').to_string())
}

fn integer_field(line: &str, key: &str) -> Option<usize> {
    let (_, value) = line.trim().split_once(':')?;
    line.trim().starts_with(&format!("\"{key}\"")).then(|| value.trim().trim_end_matches(',').parse::<usize>().ok())?
}

fn registered_overlay_span(name: &str, address: i64) -> Result<usize, String> {
    let path = root().join("games/gs1/semantic").join("regions.json");
    let text = fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut overlay = None;
    let mut entry = None;
    let mut span = None;
    for line in text.lines() {
        if line.trim() == "{" {
            overlay = None;
            entry = None;
            span = None;
        }
        overlay = quoted_field(line, "overlay").or(overlay);
        entry = quoted_field(line, "entry")
            .and_then(|value| i64::from_str_radix(value.trim_start_matches("0x"), 16).ok())
            .or(entry);
        span = integer_field(line, "span_bytes").or(span);
        if line.trim().starts_with('}') && overlay.as_deref() == Some(name) && entry == Some(address) {
            return span.ok_or_else(|| format!("{name} 0x{address:08x} has no span_bytes in {}", path.display()));
        }
    }
    Err(format!("{name} 0x{address:08x} has no registered owner span in {}", path.display()))
}

fn checked(program: &str, arguments: &[String], work: &Path) -> Result<(), String> {
    let output = Command::new(program)
        .args(arguments)
        .current_dir(root())
        .output()
        .map_err(|error| format!("cannot run {program}: {error}"))?;
    if output.status.success() {
        Ok(())
    } else {
        let detail = if output.stderr.is_empty() {
            String::from_utf8_lossy(&output.stdout)
        } else {
            String::from_utf8_lossy(&output.stderr)
        };
        Err(format!("{program} failed in {}: {}", work.display(), detail.trim()))
    }
}

fn core_reference_span(stem: &str, work: &Path) -> Result<usize, String> {
    let source = root().join("games/gs1/asm").join(format!("{stem}.s"));
    if !source.is_file() {
        let address = u32::from_str_radix(stem, 16).map_err(|error| error.to_string())?;
        for relative in ["out/gs1-en/full/claimed/manifest.json", "out/gs1-en/claimed/manifest.json"] {
            let manifest = root().join(relative);
            let Ok(text) = fs::read_to_string(&manifest) else {
                continue;
            };
            let Ok(document) = serde_json::from_str::<serde_json::Value>(&text) else {
                continue;
            };
            let size = document["regions"].as_array().and_then(|regions| {
                regions.iter().find_map(|region| {
                    (region["address"].as_u64() == Some(u64::from(address))).then(|| region["size"].as_u64()).flatten()
                })
            });
            if let Some(size) = size {
                return usize::try_from(size).map_err(|error| error.to_string());
            }
        }
        return Err(format!("{} is missing and no claimed manifest records owner {stem}", source.display()));
    }
    let object = work.join(format!("{stem}.reference.o"));
    let binary = work.join(format!("{stem}.reference.bin"));
    checked(
        "arm-none-eabi-as",
        &[
            "-mcpu=arm7tdmi".into(),
            "-mthumb-interwork".into(),
            "-o".into(),
            object.to_string_lossy().into_owned(),
            source.to_string_lossy().into_owned(),
        ],
        work,
    )?;
    checked(
        "arm-none-eabi-objcopy",
        &[
            "-O".into(),
            "binary".into(),
            "-j".into(),
            ".text".into(),
            object.to_string_lossy().into_owned(),
            binary.to_string_lossy().into_owned(),
        ],
        work,
    )?;
    let size = fs::metadata(&binary).map_err(|error| format!("{}: {error}", binary.display()))?.len() as usize;
    if size == 0 {
        return Err(format!("{} assembled to an empty .text", source.display()));
    }
    Ok(size)
}

fn differing_halfwords(actual: &[u8], expected: &[u8]) -> usize {
    let overlap = actual.len().min(expected.len());
    let mut differing = actual.len().abs_diff(expected.len()).div_ceil(2);
    let mut at = 0usize;
    while at + 1 < overlap {
        if actual[at..at + 2] != expected[at..at + 2] {
            differing += 1;
        }
        at += 2;
    }
    if overlap % 2 == 1 && actual[overlap - 1] != expected[overlap - 1] {
        differing += 1;
    }
    differing
}

fn score(actual: Vec<u8>, expected: &[u8]) -> Score {
    let first_difference = actual
        .iter()
        .zip(expected)
        .position(|(left, right)| left != right)
        .or_else(|| (actual.len() != expected.len()).then_some(actual.len().min(expected.len())));
    Score {
        exact: actual == expected,
        differing_halfwords: differing_halfwords(&actual, expected),
        expected_size: expected.len(),
        actual_size: actual.len(),
        first_difference,
        actual,
    }
}

impl PreparedTarget {
    pub fn prepare(original: PathBuf, base_source: &str) -> Result<Self, String> {
        let original = if original.is_absolute() { original } else { root().join(original) };
        let basename = basename(&original)?;
        let owner = source_owner(&original)?;
        let owner_stem = owner.address_stem();
        let work = TempDir::new("prepare")?;
        let base = work.path().join(&basename);
        let local_flags = local_include_flags(&original);
        fs::write(&base, base_source).map_err(|error| format!("{}: {error}", base.display()))?;
        if let Some(name) = owner.overlay_id() {
            let routing = owner.routing_path();
            let compiled = compile_overlay_candidate(&base, work.path(), &name, Some(&routing), &local_flags)?;
            let reference_path = root().join("games/gs1/assets").join("code").join(format!("{name}_overlay.s"));
            let reference = assemble_overlay(&OverlaySource::path(reference_path), OVERLAY_BASE)?;
            let offset = compiled.address - OVERLAY_BASE;
            let span = registered_overlay_span(&name, compiled.address)?;
            let expected = slice_clamped(&reference, offset, span);
            if expected.len() != span {
                return Err(format!("{} registered span extends beyond {name} reference image", original.display()));
            }
            let baseline = score(compiled.data, &expected);
            Ok(Self {
                original,
                basename,
                owner,
                expected,
                baseline,
                baseline_assembly: None,
                kind: Kind::Overlay { name, address: compiled.address },
            })
        } else {
            let rom_path = root().join("roms").join("gs1-en.gba");
            let rom = fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
            let routing = owner.routing_path();
            let verification = verify_candidate_owned_routed(
                &base.to_string_lossy(),
                &routing.to_string_lossy(),
                &owner_stem,
                &rom,
                &work.path().to_string_lossy(),
                &local_flags,
                ROM_BASE,
                CompilerTarget::Gs1,
                &CandidateCompilerConfiguration::default(),
            )?;
            let span = core_reference_span(&owner_stem, work.path())?;
            let address = owner.address() as i64;
            let expected = slice_clamped(&rom, address - ROM_BASE as i64, span);
            if expected.len() != span {
                return Err(format!("{owner_stem} reference span extends beyond the ROM"));
            }
            let baseline = score(verification.actual, &expected);
            let assembly_path = work.path().join(format!("{owner_stem}.s"));
            let baseline_assembly =
                fs::read_to_string(&assembly_path).map_err(|error| format!("{}: {error}", assembly_path.display()))?;
            Ok(Self {
                original,
                basename,
                owner,
                expected,
                baseline,
                baseline_assembly: Some(baseline_assembly),
                kind: Kind::Core { rom },
            })
        }
    }

    pub fn baseline(&self) -> &Score {
        &self.baseline
    }

    pub fn expected(&self) -> &[u8] {
        &self.expected
    }

    pub fn baseline_assembly(&self) -> Option<&str> {
        self.baseline_assembly.as_deref()
    }

    pub fn owner_stem(&self) -> String {
        self.owner.address_stem()
    }

    pub fn identity(&self) -> String {
        let path = self.original.to_string_lossy();
        let mut bytes = self.expected.clone();
        bytes.push(0);
        bytes.extend_from_slice(path.as_bytes());
        compiler_core::sha256::hex(&bytes)
    }

    pub fn compile(&self, source: &str) -> Result<(Score, Option<String>), String> {
        let work = TempDir::new("candidate")?;
        let path = work.path().join(&self.basename);
        let local_flags = local_include_flags(&self.original);
        fs::write(&path, source).map_err(|error| format!("{}: {error}", path.display()))?;
        let (actual, assembly) = match &self.kind {
            Kind::Overlay { name, address } => {
                let routing = self.owner.routing_path();
                let compiled = compile_overlay_candidate(&path, work.path(), name, Some(&routing), &local_flags)?;
                if compiled.address != *address {
                    return Err(format!("candidate linked at 0x{:08x}, expected 0x{address:08x}", compiled.address));
                }
                (compiled.data, None)
            }
            Kind::Core { rom } => {
                let owner_stem = self.owner.address_stem();
                let routing = self.owner.routing_path();
                let verification = verify_candidate_owned_routed(
                    &path.to_string_lossy(),
                    &routing.to_string_lossy(),
                    &owner_stem,
                    rom,
                    &work.path().to_string_lossy(),
                    &local_flags,
                    ROM_BASE,
                    CompilerTarget::Gs1,
                    &CandidateCompilerConfiguration::default(),
                )?;
                let assembly_path = work.path().join(format!("{owner_stem}.s"));
                let assembly = fs::read_to_string(&assembly_path)
                    .map_err(|error| format!("{}: {error}", assembly_path.display()))?;
                (verification.actual, Some(assembly))
            }
        };
        Ok((score(actual, &self.expected), assembly))
    }
}

pub fn self_test() -> Result<(), String> {
    let measured = score(vec![1, 0, 2, 0], &[1, 0, 3, 0]);
    if measured.exact || measured.differing_halfwords != 1 || measured.first_difference != Some(2) {
        return Err("byte scorer failed its one-halfword control".into());
    }
    let short = score(vec![1, 0], &[1, 0, 3, 0]);
    if short.differing_halfwords != 1 || short.expected_size != 4 || short.actual_size != 2 {
        return Err("byte scorer did not penalize truncation".into());
    }
    if overlay_name("resource_373_c_02005b48") != Some("resource_373".into()) || overlay_name("08012345").is_some() {
        return Err("candidate target classification drifted".into());
    }
    if registered_overlay_span("resource_373", 0x0200_5b48)? != 216 {
        return Err("registered overlay owner span drifted".into());
    }
    if local_include_flags(Path::new("recon/main/owner.c")) != ["-Irecon/main"] {
        return Err("candidate-local include routing drifted".into());
    }
    let work = TempDir::new("span-self-test")?;
    if core_reference_span("08029094", work.path())? != 334 {
        return Err("core reference assembly span drifted".into());
    }
    Ok(())
}
