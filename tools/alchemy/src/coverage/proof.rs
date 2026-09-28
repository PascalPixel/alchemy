//! Generated receipts bind source, approved ROMs and the artifacts verified.
use crate::compiler::sha256;
use crate::targets::{BuildSupport, DecompTarget};
use serde::{Deserialize, Serialize};
use std::path::Path;

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Credit {
    pub image: String,
    pub start: i64,
    pub end: i64,
    pub source: String,
    pub kind: String,
}

#[derive(Serialize, Deserialize)]
pub struct Receipt {
    pub format: u8,
    pub target: String,
    pub inputs_sha256: String,
    pub rom_sha256: String,
    pub credits: Vec<Credit>,
}

/// Hash editable build inputs and the tools that consume them. Generated
/// inventories and private extraction output cannot become build authorities.
pub fn identity(root: &Path, target: &str) -> Result<String, String> {
    fn collect(root: &Path, path: &Path, files: &mut Vec<String>) -> Result<(), String> {
        if path.is_dir() {
            for entry in std::fs::read_dir(path).map_err(|e| e.to_string())? {
                collect(root, &entry.map_err(|e| e.to_string())?.path(), files)?;
            }
        } else if path.is_file() {
            files.push(
                path.strip_prefix(root)
                    .map_err(|e| e.to_string())?
                    .to_string_lossy()
                    .into_owned(),
            );
        }
        Ok(())
    }
    let game = crate::targets::decomp_target(Some(target))?;
    let mut files = Vec::new();
    for folder in [
        format!("{}/SRC", game.game_dir()),
        format!("{}/INCLUDE", game.game_dir()),
        format!("{}/GRAPHICS", game.game_dir()),
        format!("{}/SOUND", game.game_dir()),
        format!("{}/TEXT", game.game_dir()),
        format!("{}/BUILD.MK", game.game_dir()),
        format!("{}/MAIN.LD", game.game_dir()),
        "games/COMMON/SRC".into(),
        "games/COMMON/INCLUDE".into(),
    ] {
        collect(root, &root.join(folder), &mut files)?;
    }
    // Maintained fallback assembly is a build input; generated inventories are not.
    let mut assembly = Vec::new();
    collect(root, &root.join(game.asm_dir), &mut assembly)?;
    files.extend(assembly.into_iter().filter(|path| {
        Path::new(path)
            .extension()
            .and_then(|suffix| suffix.to_str())
            .is_some_and(|suffix| {
                ["s", "inc"]
                    .iter()
                    .any(|expected| suffix.eq_ignore_ascii_case(expected))
            })
    }));
    collect(root, &root.join("tools/alchemy/src/compiler"), &mut files)?;
    for directory in [
        "tools/alchemy/src/overlay",
        "tools/alchemy/src/score",
        "tools/alchemy/src/coverage",
        "tools/alchemy/src/build_assets",
        // Psynergy's asset codecs live under src/assets.
        "tools/psynergy/src",
    ] {
        collect(root, &root.join(directory), &mut files)?;
    }
    collect(root, &root.join("tools/out/compilers"), &mut files)?;
    collect(root, &root.join("tools/out/binutils/bin"), &mut files)?;
    for path in [
        "Makefile",
        ".gitmodules",
        "tools/Cargo.toml",
        "tools/Cargo.lock",
        "tools/alchemy/Cargo.toml",
        "tools/alchemy/build.rs",
        "tools/alchemy/src/build_claimed.rs",
        "tools/alchemy/src/build_asm.rs",
        "tools/alchemy/src/build_assets.rs",
        "tools/alchemy/src/text_catalog.rs",
        "tools/alchemy/src/generated_files.rs",
        "tools/alchemy/src/build_full.rs",
        "tools/alchemy/src/targets.rs",
        "tools/alchemy/src/check/tla_owners.rs",
        "tools/alchemy/src/overlay/compile.rs",
        "tools/alchemy/src/candidate.rs",
    ] {
        if root.join(path).is_file() {
            files.push(path.into());
        }
    }
    files.sort();
    files.dedup();
    let digests = file_digests(root, &files)?;
    let mut input = Vec::new();
    for (path, digest) in files.iter().zip(digests) {
        input.extend_from_slice(path.as_bytes());
        input.push(0);
        input.extend_from_slice(digest.as_bytes());
        input.push(0);
    }
    Ok(sha256::hex(&input))
}

/// Rehash every input for each proof check; timestamps cannot prove its contents.
fn file_digests(root: &Path, files: &[String]) -> Result<Vec<String>, String> {
    let digest = |path: &String| -> Result<String, String> {
        let bytes = std::fs::read(root.join(path)).map_err(|e| format!("{path}: {e}"))?;
        Ok(sha256::hex(&bytes))
    };
    let workers = std::thread::available_parallelism()
        .map_or(1, usize::from)
        .min(16);
    let chunk = files.len().div_ceil(workers).max(1);
    std::thread::scope(|scope| {
        let handles = files
            .chunks(chunk)
            .map(|part| scope.spawn(move || part.iter().map(digest).collect::<Result<Vec<_>, _>>()))
            .collect::<Vec<_>>();
        let mut digests = Vec::with_capacity(files.len());
        for handle in handles {
            digests.extend(
                handle
                    .join()
                    .map_err(|_| "identity hashing panicked".to_string())??,
            );
        }
        Ok(digests)
    })
}

pub fn write(
    root: &Path,
    target: &str,
    rom: &[u8],
    inputs: &str,
    credits: Vec<Credit>,
) -> Result<(), String> {
    if identity(root, target)? != inputs {
        return Err("source inputs changed during verification; progress receipt withheld".into());
    }
    let game = crate::targets::decomp_target(Some(target))?;
    if sha256::hex(rom) != reference_sha256(root, game)? {
        return Err("progress receipt requires the approved reference ROM".into());
    }
    let source_build = source_report(root, game)?;
    if source_build {
        if !credits.is_empty() {
            return Err(
                "a source build with private unwritten bytes grants no progress credit".into(),
            );
        }
        verify_source_build(root, game)?;
    }
    let path = root.join(format!("out/{target}/reports/verified-code.json"));
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    let receipt = Receipt {
        format: if source_build { 2 } else { 1 },
        target: target.into(),
        inputs_sha256: inputs.into(),
        rom_sha256: sha256::hex(rom),
        credits,
    };
    crate::compiler::canonical_json::write_canonical(
        &path,
        &serde_json::to_value(receipt).map_err(|e| e.to_string())?,
    )
}

pub fn read(root: &Path, target: &str) -> Result<Receipt, String> {
    let path = root.join(format!("out/{target}/reports/verified-code.json"));
    let receipt: Receipt = serde_json::from_slice(&std::fs::read(&path).map_err(|e| {
        format!(
            "{}: {e}; verify this game's source to produce a progress receipt",
            path.display()
        )
    })?)
    .map_err(|e| e.to_string())?;
    if !matches!(receipt.format, 1 | 2)
        || receipt.target != target
        || receipt.inputs_sha256 != identity(root, target)?
    {
        return Err(format!("{target}: verified code is stale; verify the changed source before publishing progress"));
    }
    let rom = crate::targets::decomp_target(Some(target))?;
    let hash = sha256::hex(&std::fs::read(root.join(rom.rom)).map_err(|e| e.to_string())?);
    if hash != receipt.rom_sha256 || hash != reference_sha256(root, rom)? {
        return Err(format!("{target}: reference ROM changed"));
    }
    if receipt.format == 2 || source_report(root, rom)? {
        if !receipt.credits.is_empty() {
            return Err(
                "a source build with private unwritten bytes grants no progress credit".into(),
            );
        }
        verify_source_build(root, rom)?;
    }
    Ok(receipt)
}

/// The main-image proof a byte-identical full ROM build records in its
/// report. Readers never take it at its word: [`full_build`] recomputes each
/// digest from the artifacts and the tree it names.
#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct FullBuild {
    pub format: u8,
    pub target: String,
    /// The [`identity`] of the tree the build ran on.
    pub inputs_sha256: String,
    /// The reference ROM, which the rebuilt ROM equals.
    pub rom_sha256: String,
    /// The asset manifest whose regions the build reproduced.
    pub asset_manifest_sha256: String,
}

/// The full ROM build's report, which carries its [`FullBuild`] proof.
pub(crate) fn full_build_report(target: DecompTarget) -> String {
    format!("{}/full/rebuilt.json", target.output_dir)
}

/// The ROM the full build rebuilt.
pub(crate) fn full_build_rom(target: DecompTarget) -> String {
    format!("{}/full/rebuilt.gba", target.output_dir)
}

/// The asset manifest of the full build, the only one that can prove the
/// main image's asset complement.
pub(crate) fn full_asset_manifest(target: DecompTarget) -> String {
    format!("{}/full/assets/manifest.json", target.output_dir)
}

fn full_build_supported(target: DecompTarget) -> Result<(), String> {
    if target.build_support != BuildSupport::Full {
        return Err(format!(
            "{} has no supported full ROM build, so its main image stays pending until one exists",
            target.id
        ));
    }
    Ok(())
}

/// Approved identities are independent of the local ROM and generated output.
fn reference_sha256(root: &Path, target: DecompTarget) -> Result<String, String> {
    #[cfg(test)]
    if let Some(oracle) = test_oracle(root, target) {
        return Ok(oracle.rom_sha256);
    }
    crate::text_catalog::reference_sha256(root, target.id.as_str())
}

#[derive(Debug, PartialEq, Serialize, Deserialize)]
struct SourceBuild {
    format: u8,
    target: String,
    inputs_sha256: String,
    rom_sha256: String,
    native_json_sha256: String,
    native_elf_sha256: String,
    native_binary_sha256: String,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    fallback_artifacts_sha256: Option<String>,
}

fn source_report(root: &Path, target: DecompTarget) -> Result<bool, String> {
    let path = root.join(full_build_report(target));
    match std::fs::read(&path) {
        Ok(bytes) => {
            let report: serde_json::Value = serde_json::from_slice(&bytes)
                .map_err(|error| format!("{}: {error}", path.display()))?;
            Ok(report["format"].as_u64() == Some(2))
        }
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => Ok(false),
        Err(error) => Err(format!("{}: {error}", path.display())),
    }
}

fn native_artifact(root: &Path, target: DecompTarget, name: &str) -> std::path::PathBuf {
    root.join(target.output_dir).join("native").join(name)
}

fn artifact_bytes(path: &Path) -> Result<Vec<u8>, String> {
    std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn canonical_artifact(root: &Path, declared: &Path, expected: &Path) -> Result<(), String> {
    let declared = if declared.is_absolute() {
        declared.to_path_buf()
    } else {
        root.join(declared)
    };
    let declared = std::fs::canonicalize(&declared)
        .map_err(|error| format!("{}: {error}", declared.display()))?;
    let expected = std::fs::canonicalize(expected)
        .map_err(|error| format!("{}: {error}", expected.display()))?;
    if declared != expected {
        return Err("native metadata redirects a canonical build artifact".into());
    }
    Ok(())
}

fn linked_binary(root: &Path, target: DecompTarget, elf: &Path) -> Result<Vec<u8>, String> {
    #[cfg(test)]
    if let Some(binary) = test_oracle(root, target).and_then(|oracle| oracle.native_binary) {
        return Ok(binary);
    }
    let _ = target;
    let temporary = tempfile::tempdir().map_err(|error| error.to_string())?;
    let output = temporary.path().join("native.bin");
    psynergy::process::run(
        &[
            "arm-none-eabi-objcopy".into(),
            "-O".into(),
            "binary".into(),
            elf.to_string_lossy().into_owned(),
            output.to_string_lossy().into_owned(),
        ],
        root,
    )?;
    artifact_bytes(&output)
}

fn native_sections(
    root: &Path,
    target: DecompTarget,
    elf: &Path,
) -> Result<Vec<crate::compiler::native::Section>, String> {
    #[cfg(test)]
    if let Some(sections) = test_oracle(root, target).and_then(|oracle| oracle.native_sections) {
        return Ok(sections);
    }
    let _ = target;
    crate::compiler::native::inspect_loaded_sections(root, elf)
}

fn fallback_artifacts(
    root: &Path,
    target: DecompTarget,
    rom: &[u8],
) -> Result<(Option<String>, Vec<(usize, usize)>), String> {
    let directory = root.join(target.output_dir).join("full/asm");
    let manifest = directory.join("manifest.json");
    let bytes = match std::fs::read(&manifest) {
        Ok(bytes) => bytes,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            return Ok((None, Vec::new()))
        }
        Err(error) => return Err(format!("{}: {error}", manifest.display())),
    };
    let document: serde_json::Value = serde_json::from_slice(&bytes)
        .map_err(|error| format!("{}: {error}", manifest.display()))?;
    if document["format"].as_u64() != Some(1)
        || document["rom_base"].as_u64() != Some(0x0800_0000)
        || document["verification"] != "rom"
        || document["source_only"] == true
    {
        return Err("assembly fallback manifest has no ROM verification".into());
    }
    let regions = document["regions"]
        .as_array()
        .filter(|regions| !regions.is_empty())
        .ok_or("assembly fallback manifest has no compiled regions")?;
    let canonical_root = std::fs::canonicalize(root).map_err(|error| error.to_string())?;
    let maintained = crate::build_asm::maintained_assembly(root, target)?
        .into_iter()
        .map(|module| {
            let path = std::fs::canonicalize(&module.source).map_err(|error| error.to_string())?;
            Ok((path, (module.load_address, module.run_address)))
        })
        .collect::<Result<std::collections::BTreeMap<_, _>, String>>()?;
    let mut artifacts = vec![(
        format!("{}/full/asm/manifest.json", target.output_dir),
        sha256::hex(&bytes),
    )];
    let mut ranges = Vec::new();
    let mut seen = std::collections::BTreeSet::new();
    for region in regions {
        if region["confidence"] != "verified" || region["source_only"] == true {
            return Err("assembly fallback region is unverified".into());
        }
        let address = region["address"]
            .as_u64()
            .ok_or("assembly fallback has no load address")?;
        let run_address = region["run_address"]
            .as_u64()
            .ok_or("assembly fallback has no run address")?;
        let size = usize::try_from(
            region["size"]
                .as_u64()
                .ok_or("assembly fallback has no compiled size")?,
        )
        .map_err(|error| error.to_string())?;
        let source = region["source"]
            .as_str()
            .ok_or("assembly fallback has no maintained source")?;
        let source = Path::new(source);
        if source.is_absolute()
            || source
                .components()
                .any(|part| !matches!(part, std::path::Component::Normal(_)))
        {
            return Err("assembly fallback source is not a repository source path".into());
        }
        let source_path = std::fs::canonicalize(root.join(source))
            .map_err(|error| format!("{}: {error}", source.display()))?;
        if !source_path.is_file() || !source_path.starts_with(&canonical_root) {
            return Err("assembly fallback source escapes the repository".into());
        }
        let relative = source_path
            .strip_prefix(&canonical_root)
            .map_err(|error| error.to_string())?;
        let raw = Path::new(target.asm_dir);
        let is_raw = relative.starts_with(raw)
            && !relative.starts_with(raw.join("overlays"))
            && !relative.starts_with(raw.join("battle"))
            && relative.extension().and_then(|suffix| suffix.to_str()) == Some("s");
        let placement =
            if is_raw && region["kind"] == "raw_assembly" && region["origin"] == "unresolved" {
                let name = relative
                    .file_stem()
                    .and_then(|name| name.to_str())
                    .ok_or("assembly fallback source has no name")?;
                let address = u64::from_str_radix(name, 16)
                    .map_err(|_| "raw assembly source has no maintained address")?;
                Some((address, address))
            } else if relative.starts_with(target.source_dir)
                && region["kind"] == "maintained_assembly"
                && region["origin"] == "source"
            {
                maintained.get(&source_path).copied()
            } else {
                None
            };
        if placement != Some((address, run_address)) {
            return Err("assembly fallback placement does not match its maintained source".into());
        }
        if (is_raw && region["retention"] != "not_yet_c")
            || !matches!(region["retention"].as_str(), Some("not_yet_c" | "keep_asm"))
        {
            return Err("assembly fallback has no maintained classification".into());
        }
        let output = Path::new(
            region["output"]
                .as_str()
                .ok_or("assembly fallback has no compiled output")?,
        );
        let expected = directory.join(format!("{address:08x}.bin"));
        canonical_artifact(root, output, &expected)?;
        let output = std::fs::canonicalize(&expected).map_err(|error| error.to_string())?;
        let canonical_directory =
            std::fs::canonicalize(&directory).map_err(|error| error.to_string())?;
        if output.parent() != Some(canonical_directory.as_path())
            || !output.starts_with(canonical_root.join(target.output_dir))
        {
            return Err("assembly fallback output escapes its canonical directory".into());
        }
        if !seen.insert(expected.clone()) {
            return Err("assembly fallback lists one compiled output more than once".into());
        }
        let data = artifact_bytes(&expected)?;
        let start = usize::try_from(
            address
                .checked_sub(0x0800_0000)
                .ok_or("assembly fallback has no ROM address")?,
        )
        .map_err(|error| error.to_string())?;
        let end = start
            .checked_add(size)
            .ok_or("assembly fallback address overflow")?;
        if size == 0 || size != data.len() || rom.get(start..end) != Some(data.as_slice()) {
            return Err(
                "compiled assembly fallback differs in size or bytes from the approved ROM".into(),
            );
        }
        if ranges
            .iter()
            .any(|&(left, right)| start < right && left < end)
        {
            return Err("compiled assembly fallback regions overlap".into());
        }
        let relative = format!("{}/full/asm/{address:08x}.bin", target.output_dir);
        artifacts.push((relative, sha256::hex(&data)));
        ranges.push((start, end));
    }
    artifacts.sort();
    Ok((
        Some(sha256::hex(
            &serde_json::to_vec(&artifacts).map_err(|error| error.to_string())?,
        )),
        ranges,
    ))
}

fn union_bytes(mut ranges: Vec<(usize, usize)>) -> Result<usize, String> {
    ranges.sort_unstable();
    let mut total = 0usize;
    let mut end = 0usize;
    for (start, right) in ranges {
        if right > end {
            total = total
                .checked_add(right - start.max(end))
                .ok_or("source byte count overflow")?;
            end = right;
        }
    }
    Ok(total)
}

fn source_artifacts(
    root: &Path,
    target: DecompTarget,
    rom: &[u8],
) -> Result<(SourceBuild, usize), String> {
    if rom.len() as u64 != target.rom_size || sha256::hex(rom) != reference_sha256(root, target)? {
        return Err(
            "source build requires the approved reference ROM and its complete size".into(),
        );
    }
    if artifact_bytes(&root.join(target.rom))? != rom {
        return Err("source build differs from the approved local reference ROM".into());
    }
    let json = native_artifact(root, target, "native.json");
    let elf = native_artifact(root, target, "native.elf");
    let binary = native_artifact(root, target, "native.bin");
    let metadata = artifact_bytes(&json)?;
    let native: crate::compiler::native::Build = serde_json::from_slice(&metadata)
        .map_err(|error| format!("{}: {error}", json.display()))?;
    crate::compiler::native::maintained_sources(root, target, &native.sources)?;
    canonical_artifact(
        root,
        &native.script,
        &root.join(format!("{}/MAIN.LD", target.game_dir())),
    )?;
    canonical_artifact(root, &native.elf, &elf)?;
    canonical_artifact(root, &native.binary, &binary)?;
    let elf_bytes = artifact_bytes(&elf)?;
    let binary_bytes = artifact_bytes(&binary)?;
    let sections = native_sections(root, target, &elf)?;
    if sections != native.sections {
        return Err("native metadata disagrees with the linked ELF sections".into());
    }
    if sections.is_empty() || binary_bytes != linked_binary(root, target, &elf)? {
        return Err("native binary is not the output of its linked ELF".into());
    }
    let first = sections
        .iter()
        .map(|section| section.load_address)
        .min()
        .unwrap();
    let mut ranges = Vec::new();
    let mut binary_end = 0usize;
    for section in &sections {
        let start = usize::try_from(
            section
                .load_address
                .checked_sub(0x0800_0000)
                .ok_or("linked section has no ROM load address")?,
        )
        .map_err(|error| error.to_string())?;
        let size = usize::try_from(section.size).map_err(|error| error.to_string())?;
        let end = start
            .checked_add(size)
            .ok_or("linked section address overflow")?;
        if size == 0
            || end > rom.len()
            || ranges
                .iter()
                .any(|&(left, right)| start < right && left < end)
        {
            return Err("linked source sections overlap or lie outside the ROM".into());
        }
        let offset =
            usize::try_from(section.load_address - first).map_err(|error| error.to_string())?;
        let end_offset = offset
            .checked_add(size)
            .ok_or("linked binary offset overflow")?;
        let bytes = binary_bytes
            .get(offset..end_offset)
            .ok_or("linked binary is shorter than its sections")?;
        if rom[start..end] != *bytes {
            return Err(format!(
                "{} linked bytes differ from the approved ROM",
                section.name
            ));
        }
        ranges.push((start, end));
        binary_end = binary_end.max(end_offset);
    }
    if binary_end != binary_bytes.len() {
        return Err("native binary contains bytes outside its loaded sections".into());
    }
    let (fallback_artifacts_sha256, fallback_ranges) = fallback_artifacts(root, target, rom)?;
    ranges.extend(fallback_ranges);
    let source_bytes = union_bytes(ranges)?;
    Ok((
        SourceBuild {
            format: 2,
            target: target.id.as_str().into(),
            inputs_sha256: identity(root, target.id.as_str())?,
            rom_sha256: sha256::hex(rom),
            native_json_sha256: sha256::hex(&metadata),
            native_elf_sha256: sha256::hex(&elf_bytes),
            native_binary_sha256: sha256::hex(&binary_bytes),
            fallback_artifacts_sha256,
        },
        source_bytes,
    ))
}

/// Private unwritten bytes remain explicit and grant no source or asset credit.
pub(crate) fn record_source_build(
    root: &Path,
    target: DecompTarget,
    inputs: &str,
    rom: &[u8],
    source_bytes: usize,
) -> Result<serde_json::Value, String> {
    full_build_supported(target)?;
    let (proof, measured) = source_artifacts(root, target, rom)?;
    if proof.inputs_sha256 != inputs {
        return Err("source inputs changed during the build; its proof is withheld".into());
    }
    if measured != source_bytes {
        return Err("source byte count disagrees with the independently inspected ELF".into());
    }
    Ok(serde_json::json!({
        "format": 2, "target": target.id.as_str(), "verification": "rom", "byte_identical": true,
        "source_bytes": measured, "private_unwritten_bytes": rom.len() - measured,
        "credits": [], "source_build_proof": proof,
    }))
}

pub(crate) fn verify_source_build(root: &Path, target: DecompTarget) -> Result<(), String> {
    full_build_supported(target)?;
    let path = root.join(full_build_report(target));
    let report: serde_json::Value = serde_json::from_slice(&artifact_bytes(&path)?)
        .map_err(|error| format!("{}: {error}", path.display()))?;
    if report["format"].as_u64() != Some(2)
        || report["target"] != target.id.as_str()
        || report["verification"] != "rom"
        || report["byte_identical"] != true
        || report["credits"]
            .as_array()
            .is_none_or(|credits| !credits.is_empty())
        || report.get("main_image_proof").is_some()
    {
        return Err("not an uncredited, byte-identical source build".into());
    }
    let recorded: SourceBuild = serde_json::from_value(report["source_build_proof"].clone())
        .map_err(|error| format!("source build records no artifact proof: {error}"))?;
    let rom = artifact_bytes(&root.join(full_build_rom(target)))?;
    let (actual, source_bytes) = source_artifacts(root, target, &rom)?;
    if recorded != actual {
        return Err("source build inputs or artifacts changed after verification".into());
    }
    if report["source_bytes"].as_u64() != Some(source_bytes as u64)
        || report["private_unwritten_bytes"].as_u64() != Some((rom.len() - source_bytes) as u64)
    {
        return Err(
            "source and private unwritten byte counts disagree with the linked build".into(),
        );
    }
    if let Some(recorded_sections) = report.get("sections") {
        let native: crate::compiler::native::Build = serde_json::from_slice(&artifact_bytes(
            &native_artifact(root, target, "native.json"),
        )?)
        .map_err(|error| error.to_string())?;
        if *recorded_sections
            != serde_json::to_value(&native.sections).map_err(|error| error.to_string())?
        {
            return Err("report sections differ from the independently inspected ELF".into());
        }
    }
    Ok(())
}

/// The proof a full build of `target` records once it has rebuilt `rom`
/// byte for byte at the canonical locations. `inputs` is the [`identity`]
/// the build started from; a tree that changed while it ran proves nothing,
/// and neither does a ROM other than the registered reference.
#[cfg(test)]
pub(crate) fn full_build_proof(
    root: &Path,
    target: DecompTarget,
    inputs: &str,
    rom: &[u8],
) -> Result<FullBuild, String> {
    full_build_supported(target)?;
    if identity(root, target.id.as_str())? != inputs {
        return Err(
            "source inputs changed during the build; its main-image proof is withheld".into(),
        );
    }
    if sha256::hex(rom) != reference_sha256(root, target)? {
        return Err(format!(
            "the rebuilt ROM is not the approved {} reference ROM; its main-image proof is withheld",
            target.id
        ));
    }
    let manifest = full_asset_manifest(target);
    let manifest = std::fs::read(root.join(&manifest)).map_err(|e| format!("{manifest}: {e}"))?;
    Ok(FullBuild {
        format: 1,
        target: target.id.as_str().into(),
        inputs_sha256: inputs.into(),
        rom_sha256: sha256::hex(rom),
        asset_manifest_sha256: sha256::hex(&manifest),
    })
}

/// Removes the target's full-build proof, its report and rebuilt ROM, so a
/// build that has started, or failed, leaves no earlier proof behind. The
/// asset stage keeps its manifest as its cache: without a report whose
/// proof records its digest, that manifest proves nothing.
pub(crate) fn withdraw_full_build(root: &Path, target: DecompTarget) -> Result<(), String> {
    for path in [full_build_report(target), full_build_rom(target)] {
        match std::fs::remove_file(root.join(&path)) {
            Ok(()) => {}
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
            Err(error) => return Err(format!("{path}: {error}")),
        }
    }
    Ok(())
}

/// A full build that still proves the main image: the reference ROM it
/// reproduced and the asset manifest bytes its proof covers.
pub(crate) struct VerifiedBuild {
    pub rom_sha256: String,
    pub asset_manifest: Vec<u8>,
}

/// The target's last full ROM build, recomputed from its artifacts, whose
/// recorded inputs are still the tree's: the proof a receipt, and so DONE,
/// needs. `Err` says why the build proves nothing about the tree now.
#[cfg(test)]
pub(crate) fn full_build(root: &Path, target: DecompTarget) -> Result<VerifiedBuild, String> {
    let (build, inputs) = last_full_build(root, target)?;
    if identity(root, target.id.as_str())? != inputs {
        return Err("its build inputs changed after the build".into());
    }
    Ok(build)
}

/// The target's last byte-identical full ROM build, recomputed from its
/// artifacts whatever the tree has become since: a supported full build
/// whose report is byte-identical with nothing unowned, whose rebuilt ROM
/// hashes to the registered reference ROM and equals the local one, and
/// whose asset manifest hashes to the digest its proof records. The
/// executable inventory depends only on the reference ROM and this layout,
/// so it stays authoritative across source and tool changes; a receipt
/// needs [`full_build`]. A build that started, or failed, left none.
pub(crate) fn last_full_build(
    root: &Path,
    target: DecompTarget,
) -> Result<(VerifiedBuild, String), String> {
    full_build_supported(target)?;
    let read =
        |path: &str| std::fs::read(root.join(path)).map_err(|e| format!("cannot read {path}: {e}"));
    let path = full_build_report(target);
    let report: serde_json::Value =
        serde_json::from_slice(&read(&path)?).map_err(|e| format!("{path}: {e}"))?;
    if report["format"].as_u64() != Some(1)
        || report["byte_identical"] != true
        || report["verification"] != "rom"
        || report["target"] != target.id.as_str()
    {
        return Err(format!(
            "{path} is not a byte-identical build of the {} ROM",
            target.id
        ));
    }
    for field in ["unowned_bytes", "rom_fallback_bytes"] {
        if report[field].as_u64() != Some(0) {
            return Err(format!("{path} does not record zero {field}"));
        }
    }
    let proof: FullBuild = serde_json::from_value(report["main_image_proof"].clone())
        .map_err(|_| format!("{path} records no main-image proof"))?;
    if proof.format != 1 || proof.target != target.id.as_str() {
        return Err(format!(
            "{path} records no main-image proof of {}",
            target.id
        ));
    }
    let manifest = full_asset_manifest(target);
    let asset_manifest = read(&manifest)?;
    if sha256::hex(&asset_manifest) != proof.asset_manifest_sha256 {
        return Err(format!(
            "{manifest} is not the asset manifest the build recorded"
        ));
    }
    let rebuilt = full_build_rom(target);
    if sha256::hex(&read(&rebuilt)?) != proof.rom_sha256 {
        return Err(format!("{rebuilt} is not the ROM the build recorded"));
    }
    if proof.rom_sha256 != reference_sha256(root, target)? {
        return Err(format!(
            "{rebuilt} is not the approved {} reference ROM",
            target.id
        ));
    }
    if sha256::hex(&read(target.rom)?) != proof.rom_sha256 {
        return Err(format!(
            "{rebuilt} is not the local reference ROM {}",
            target.rom
        ));
    }
    Ok((
        VerifiedBuild {
            rom_sha256: proof.rom_sha256,
            asset_manifest,
        },
        proof.inputs_sha256,
    ))
}

/// `target` as it will be once its full ROM build is supported, so tests can
/// exercise the readers that a game without one keeps pending.
#[cfg(test)]
pub(crate) fn fully_buildable(target: DecompTarget) -> DecompTarget {
    DecompTarget {
        build_support: BuildSupport::Full,
        ..target
    }
}

#[cfg(test)]
#[derive(Clone)]
struct TestOracle {
    rom_sha256: String,
    native_sections: Option<Vec<crate::compiler::native::Section>>,
    native_binary: Option<Vec<u8>>,
}

#[cfg(test)]
fn test_oracles(
) -> &'static std::sync::Mutex<std::collections::HashMap<(std::path::PathBuf, String), TestOracle>>
{
    static ORACLES: std::sync::OnceLock<
        std::sync::Mutex<std::collections::HashMap<(std::path::PathBuf, String), TestOracle>>,
    > = std::sync::OnceLock::new();
    ORACLES.get_or_init(Default::default)
}

#[cfg(test)]
fn test_oracle(root: &Path, target: DecompTarget) -> Option<TestOracle> {
    test_oracles()
        .lock()
        .unwrap()
        .get(&(root.to_path_buf(), target.id.as_str().into()))
        .cloned()
}

#[cfg(test)]
fn register_test_oracle(root: &Path, target: DecompTarget, oracle: TestOracle) {
    test_oracles()
        .lock()
        .unwrap()
        .insert((root.to_path_buf(), target.id.as_str().into()), oracle);
}

/// Writes what a byte-identical full build of `target` leaves under `root`:
/// a reference ROM registered as the game's reference, the equal rebuilt ROM,
/// `manifest` as its asset manifest and a report whose proof records them
/// and the tree as it is now. Returns the ROM's sha256. Inputs written
/// afterwards make the proof stale.
#[cfg(test)]
pub(crate) fn full_build_fixture(
    root: &Path,
    target: DecompTarget,
    manifest: &serde_json::Value,
) -> String {
    let write = |path: &str, bytes: &[u8]| {
        let path = root.join(path);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(path, bytes).unwrap();
    };
    let rom = format!("{} reference ROM", target.id).into_bytes();
    register_test_oracle(
        root,
        target,
        TestOracle {
            rom_sha256: sha256::hex(&rom),
            native_sections: None,
            native_binary: None,
        },
    );
    write(target.rom, &rom);
    write(&full_build_rom(target), &rom);
    write(
        &full_asset_manifest(target),
        manifest.to_string().as_bytes(),
    );
    let inputs = identity(root, target.id.as_str()).unwrap();
    let proof = full_build_proof(root, target, &inputs, &rom).unwrap();
    let report = serde_json::json!({
        "format": 1, "target": target.id.as_str(), "verification": "rom",
        "byte_identical": true, "unowned_bytes": 0, "rom_fallback_bytes": 0,
        "main_image_proof": proof,
    });
    write(&full_build_report(target), report.to_string().as_bytes());
    sha256::hex(&rom)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn write_fixture(root: &Path, path: &Path, bytes: &[u8]) {
        let path = root.join(path);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(path, bytes).unwrap();
    }

    fn source_fixture(root: &Path) -> DecompTarget {
        let target = DecompTarget {
            rom_size: 4,
            ..crate::targets::decomp_target(Some("tbs-en")).unwrap()
        };
        let rom = [0, 0, 1, 2];
        let binary = [1, 2];
        let sections = vec![crate::compiler::native::Section {
            name: ".text".into(),
            size: 2,
            address: 0x0800_0002,
            load_address: 0x0800_0002,
        }];
        register_test_oracle(
            root,
            target,
            TestOracle {
                rom_sha256: sha256::hex(&rom),
                native_sections: Some(sections.clone()),
                native_binary: Some(binary.to_vec()),
            },
        );
        write_fixture(root, Path::new(target.rom), &rom);
        write_fixture(root, Path::new(&full_build_rom(target)), &rom);
        write_fixture(
            root,
            Path::new(&format!("{}/SRC/BOOT.C", target.game_dir())),
            b"void Boot(void) {}\n",
        );
        write_fixture(
            root,
            Path::new(&format!("{}/MAIN.LD", target.game_dir())),
            b"SECTIONS { .text 0x08000002 : { *(.text) } }\n",
        );
        write_fixture(
            root,
            &native_artifact(root, target, "native.elf"),
            b"explicit test ELF oracle",
        );
        write_fixture(root, &native_artifact(root, target, "native.bin"), &binary);
        let native = crate::compiler::native::Build {
            sources: vec![root.join(format!("{}/SRC/BOOT.C", target.game_dir()))],
            script: root.join(format!("{}/MAIN.LD", target.game_dir())),
            objects: vec![native_artifact(root, target, "obj/BOOT.o")],
            elf: native_artifact(root, target, "native.elf"),
            binary: native_artifact(root, target, "native.bin"),
            symbols: native_artifact(root, target, "native.nm"),
            map: native_artifact(root, target, "native.map"),
            log: native_artifact(root, target, "build.log"),
            sections,
        };
        write_fixture(
            root,
            &native_artifact(root, target, "native.json"),
            &serde_json::to_vec(&native).unwrap(),
        );
        let inputs = identity(root, target.id.as_str()).unwrap();
        let report = record_source_build(root, target, &inputs, &rom, 2).unwrap();
        write_fixture(
            root,
            Path::new(&full_build_report(target)),
            &serde_json::to_vec(&report).unwrap(),
        );
        target
    }

    fn fallback_fixture(root: &Path, target: DecompTarget) -> serde_json::Value {
        write_fixture(
            root,
            Path::new("recon/tbs/raw/08000000.s"),
            b".thumb\n.byte 0, 0, 1\n",
        );
        let output = root.join(target.output_dir).join("full/asm/08000000.bin");
        write_fixture(root, &output, &[0, 0, 1]);
        let manifest = serde_json::json!({
            "format": 1, "rom_base": 0x0800_0000, "verification": "rom", "regions": [{
                "address": 0x0800_0000, "run_address": 0x0800_0000, "size": 3,
                "source": "recon/tbs/raw/08000000.s", "output": output,
                "kind": "raw_assembly", "origin": "unresolved", "retention": "not_yet_c", "confidence": "verified"
            }],
        });
        write_fixture(
            root,
            &root.join(target.output_dir).join("full/asm/manifest.json"),
            &serde_json::to_vec(&manifest).unwrap(),
        );
        let inputs = identity(root, target.id.as_str()).unwrap();
        let report = record_source_build(root, target, &inputs, &[0, 0, 1, 2], 4).unwrap();
        write_fixture(
            root,
            Path::new(&full_build_report(target)),
            &serde_json::to_vec(&report).unwrap(),
        );
        manifest
    }

    #[test]
    fn verified_assembly_fallback_counts_only_its_union_with_native_code() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        fallback_fixture(root.path(), target);
        verify_source_build(root.path(), target).unwrap();
        let report: serde_json::Value = serde_json::from_slice(
            &artifact_bytes(&root.path().join(full_build_report(target))).unwrap(),
        )
        .unwrap();
        assert_eq!(report["source_bytes"], 4);
        assert_eq!(report["private_unwritten_bytes"], 0);
        assert_eq!(report["credits"], serde_json::json!([]));
        assert!(report["source_build_proof"]["fallback_artifacts_sha256"].is_string());
        let inputs = identity(root.path(), target.id.as_str()).unwrap();
        assert!(record_source_build(root.path(), target, &inputs, &[0, 0, 1, 2], 5).is_err());
        for path in [
            root.path()
                .join(target.output_dir)
                .join("full/asm/08000000.bin"),
            root.path()
                .join(target.output_dir)
                .join("full/asm/manifest.json"),
            root.path().join("recon/tbs/raw/08000000.s"),
        ] {
            let before = artifact_bytes(&path).unwrap();
            let mut changed = before.clone();
            changed.push(1);
            std::fs::write(&path, changed).unwrap();
            assert!(
                verify_source_build(root.path(), target).is_err(),
                "{}",
                path.display()
            );
            std::fs::write(&path, before).unwrap();
            verify_source_build(root.path(), target).unwrap();
        }
        std::fs::remove_file(
            root.path()
                .join(target.output_dir)
                .join("full/asm/manifest.json"),
        )
        .unwrap();
        assert!(verify_source_build(root.path(), target).is_err());
    }

    #[test]
    fn fallback_manifest_cannot_claim_unverified_or_unmaintained_material() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        let manifest = fallback_fixture(root.path(), target);
        let path = root
            .path()
            .join(target.output_dir)
            .join("full/asm/manifest.json");
        write_fixture(root.path(), Path::new("tools/OTHER.S"), b".thumb\nbx lr\n");
        for (field, value) in [
            ("confidence", serde_json::json!("unchecked")),
            ("confidence", serde_json::json!("unverified")),
            ("source_only", serde_json::json!(true)),
            ("size", serde_json::json!(2)),
            ("address", serde_json::json!(0x0800_0001)),
            ("run_address", serde_json::json!(0x0200_0000)),
            ("source", serde_json::json!("tools/OTHER.S")),
            ("source", serde_json::json!("../outside.s")),
            ("output", serde_json::json!("roms/tbs-en.gba")),
            ("kind", serde_json::json!("compiler_runtime")),
            ("retention", serde_json::json!("keep_asm")),
        ] {
            let mut changed = manifest.clone();
            changed["regions"][0][field] = value;
            std::fs::write(&path, serde_json::to_vec(&changed).unwrap()).unwrap();
            assert!(verify_source_build(root.path(), target).is_err(), "{field}");
        }
        let mut changed = manifest.clone();
        changed["verification"] = serde_json::json!("source_only");
        std::fs::write(&path, serde_json::to_vec(&changed).unwrap()).unwrap();
        assert!(verify_source_build(root.path(), target).is_err());
    }

    #[test]
    fn source_receipt_cannot_survive_removal_of_its_build_report() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        let receipt = Receipt {
            format: 2,
            target: target.id.as_str().into(),
            inputs_sha256: identity(root.path(), target.id.as_str()).unwrap(),
            rom_sha256: sha256::hex(&[0, 0, 1, 2]),
            credits: Vec::new(),
        };
        write_fixture(
            root.path(),
            Path::new(&format!("{}/reports/verified-code.json", target.output_dir)),
            &serde_json::to_vec(&receipt).unwrap(),
        );
        std::fs::remove_file(root.path().join(full_build_report(target))).unwrap();
        let error = read(root.path(), target.id.as_str()).err().unwrap();
        assert!(error.contains("full/rebuilt.json"), "{error}");
    }

    #[test]
    fn private_source_build_is_exact_but_never_an_asset_complement_or_credit() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        verify_source_build(root.path(), target).unwrap();
        let report: serde_json::Value = serde_json::from_slice(
            &artifact_bytes(&root.path().join(full_build_report(target))).unwrap(),
        )
        .unwrap();
        assert_eq!(report["source_bytes"], 2);
        assert_eq!(report["private_unwritten_bytes"], 2);
        assert_eq!(report["credits"], serde_json::json!([]));
        assert!(last_full_build(root.path(), target).is_err());
        let inputs = identity(root.path(), target.id.as_str()).unwrap();
        assert!(record_source_build(root.path(), target, &inputs, &[0, 0, 1, 2], 4).is_err());
        assert!(record_source_build(root.path(), target, &inputs, &[0, 0, 1, 3], 2).is_err());
        assert!(
            record_source_build(root.path(), target, "stale inputs", &[0, 0, 1, 2], 2).is_err()
        );
    }

    #[test]
    fn source_proof_detects_changed_inputs_rom_and_each_native_artifact() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        let paths = [
            root.path()
                .join(format!("{}/SRC/BOOT.C", target.game_dir())),
            root.path().join(format!("{}/MAIN.LD", target.game_dir())),
            native_artifact(root.path(), target, "native.json"),
            native_artifact(root.path(), target, "native.elf"),
            native_artifact(root.path(), target, "native.bin"),
            root.path().join(full_build_rom(target)),
            root.path().join(target.rom),
        ];
        for path in paths {
            let before = artifact_bytes(&path).unwrap();
            let mut changed = before.clone();
            changed.push(1);
            std::fs::write(&path, changed).unwrap();
            assert!(
                verify_source_build(root.path(), target).is_err(),
                "{}",
                path.display()
            );
            std::fs::write(&path, before).unwrap();
            verify_source_build(root.path(), target).unwrap();
        }
    }

    #[test]
    fn matching_artifacts_cannot_claim_generated_or_unbound_inputs() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        let metadata = native_artifact(root.path(), target, "native.json");
        let original = artifact_bytes(&metadata).unwrap();
        for path in ["out/ROM.S", "games/THE LOST AGE/SRC/BOOT.C"] {
            write_fixture(root.path(), Path::new(path), b"unbound source");
            let mut native: crate::compiler::native::Build =
                serde_json::from_slice(&original).unwrap();
            native.sources = vec![root.path().join(path)];
            std::fs::write(&metadata, serde_json::to_vec(&native).unwrap()).unwrap();
            let inputs = identity(root.path(), target.id.as_str()).unwrap();
            assert!(
                record_source_build(root.path(), target, &inputs, &[0, 0, 1, 2], 2)
                    .unwrap_err()
                    .contains("maintained")
            );
        }
        let script = root.path().join("out/other.LD");
        write_fixture(root.path(), &script, b"SECTIONS {}");
        let mut native: crate::compiler::native::Build = serde_json::from_slice(&original).unwrap();
        native.script = script;
        std::fs::write(&metadata, serde_json::to_vec(&native).unwrap()).unwrap();
        let inputs = identity(root.path(), target.id.as_str()).unwrap();
        assert!(
            record_source_build(root.path(), target, &inputs, &[0, 0, 1, 2], 2)
                .unwrap_err()
                .contains("redirects")
        );
    }

    #[test]
    fn source_report_cannot_invent_counts_credit_or_loaded_sections() {
        let root = tempfile::tempdir().unwrap();
        let target = source_fixture(root.path());
        let path = root.path().join(full_build_report(target));
        let before: serde_json::Value =
            serde_json::from_slice(&artifact_bytes(&path).unwrap()).unwrap();
        for (field, value) in [
            ("source_bytes", serde_json::json!(4)),
            ("private_unwritten_bytes", serde_json::json!(0)),
            (
                "credits",
                serde_json::json!([{"kind":"assembly", "start":0, "end":2}]),
            ),
            ("credits", serde_json::Value::Null),
            ("main_image_proof", serde_json::json!({"format":1})),
            ("sections", serde_json::json!([])),
            ("format", serde_json::json!(1)),
        ] {
            let mut changed = before.clone();
            changed[field] = value;
            std::fs::write(&path, serde_json::to_vec(&changed).unwrap()).unwrap();
            assert!(verify_source_build(root.path(), target).is_err(), "{field}");
        }
        std::fs::write(&path, serde_json::to_vec(&before).unwrap()).unwrap();
        let metadata = native_artifact(root.path(), target, "native.json");
        let mut native: crate::compiler::native::Build =
            serde_json::from_slice(&artifact_bytes(&metadata).unwrap()).unwrap();
        native.sections[0].load_address -= 1;
        std::fs::write(&metadata, serde_json::to_vec(&native).unwrap()).unwrap();
        assert!(verify_source_build(root.path(), target)
            .unwrap_err()
            .contains("linked ELF sections"));
    }

    #[test]
    fn local_rom_and_calculated_catalog_cannot_approve_themselves() {
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::decomp_target(Some("tbs-en")).unwrap();
        let rom = b"self-approved local ROM";
        write_fixture(root.path(), Path::new(target.rom), rom);
        let catalog =
            serde_json::json!([{"target": target.id.as_str(), "rom_sha256": sha256::hex(rom)}]);
        write_fixture(
            root.path(),
            Path::new("recon/tbs/text.json"),
            &serde_json::to_vec(&catalog).unwrap(),
        );
        let inputs = identity(root.path(), target.id.as_str()).unwrap();
        assert!(full_build_proof(root.path(), target, &inputs, rom)
            .unwrap_err()
            .contains("approved"));
        assert!(
            write(root.path(), target.id.as_str(), rom, &inputs, Vec::new())
                .unwrap_err()
                .contains("approved")
        );
    }

    #[test]
    fn maintained_fallback_changes_identity_but_old_calculated_catalogs_do_not() {
        let root = tempfile::tempdir().unwrap();
        write_fixture(
            root.path(),
            Path::new("recon/tbs/raw/START.S"),
            b".thumb\nbx lr\n",
        );
        let before = identity(root.path(), "tbs-en").unwrap();
        write_fixture(
            root.path(),
            Path::new("recon/tbs/raw/START.S"),
            b".thumb\nbx r0\n",
        );
        let after = identity(root.path(), "tbs-en").unwrap();
        assert_ne!(before, after);
        for path in [
            "recon/tbs/text.json",
            "recon/tbs/assets.json",
            "recon/tbs/machine.json",
            "recon/tbs/source-bindings.json",
            "recon/tbs/raw/index.json",
        ] {
            write_fixture(root.path(), Path::new(path), b"{\"calculated\":1}");
            assert_eq!(after, identity(root.path(), "tbs-en").unwrap(), "{path}");
        }
    }

    #[test]
    fn legacy_asset_complement_proof_keeps_its_zero_fallback_requirement() {
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::decomp_target(Some("tbs-en")).unwrap();
        full_build_fixture(root.path(), target, &serde_json::json!({"regions": []}));
        let path = root.path().join(full_build_report(target));
        let before: serde_json::Value =
            serde_json::from_slice(&artifact_bytes(&path).unwrap()).unwrap();
        for field in ["unowned_bytes", "rom_fallback_bytes"] {
            let mut changed = before.clone();
            changed[field] = serde_json::json!(1);
            std::fs::write(&path, serde_json::to_vec(&changed).unwrap()).unwrap();
            assert!(last_full_build(root.path(), target)
                .err()
                .unwrap()
                .contains(field));
        }
    }
    #[test]
    fn changing_a_shared_header_invalidates_build_identity() {
        let root = tempfile::tempdir().unwrap();
        let include = root.path().join("games/COMMON/INCLUDE");
        std::fs::create_dir_all(&include).unwrap();
        let header = include.join("SOUND.H");
        std::fs::write(&header, "typedef int Sample;\n").unwrap();
        let before = identity(root.path(), "tla-en").unwrap();
        std::fs::write(&header, "typedef short Sample;\n").unwrap();
        assert_ne!(before, identity(root.path(), "tla-en").unwrap());
    }

    #[test]
    fn equal_size_and_restored_timestamp_cannot_hide_a_source_change() {
        let root = tempfile::tempdir().unwrap();
        let path = Path::new("games/THE BROKEN SEAL/SRC/BOOT.C");
        write_fixture(root.path(), path, b"void Boot(void) { A(); }\n");
        let full = root.path().join(path);
        let modified = std::fs::metadata(&full).unwrap().modified().unwrap();
        let before = identity(root.path(), "tbs-en").unwrap();
        std::fs::write(&full, b"void Boot(void) { B(); }\n").unwrap();
        std::fs::File::open(&full)
            .unwrap()
            .set_times(std::fs::FileTimes::new().set_modified(modified))
            .unwrap();
        assert_ne!(before, identity(root.path(), "tbs-en").unwrap());
    }

    #[test]
    fn generated_raw_does_not_invalidate_c_identity() {
        let root = tempfile::tempdir().unwrap();
        let raw = root.path().join("recon/tla/raw/overlays");
        std::fs::create_dir_all(&raw).unwrap();
        let plan = raw.join("resource_001.plan.json");
        std::fs::write(&plan, "{}").unwrap();
        let before = identity(root.path(), "tla-en").unwrap();
        std::fs::write(&plan, "{\"start\":32}").unwrap();
        assert_eq!(before, identity(root.path(), "tla-en").unwrap());
    }

    #[test]
    fn generated_executable_inventory_does_not_invalidate_build_identity() {
        let root = tempfile::tempdir().unwrap();
        let metrics = root.path().join("out/tbs-en/reports/executable.json");
        std::fs::create_dir_all(metrics.parent().unwrap()).unwrap();
        std::fs::write(&metrics, "{\"total_union_bytes\":8}").unwrap();
        let before = identity(root.path(), "tbs-en").unwrap();
        std::fs::write(&metrics, "{\"total_union_bytes\":16}").unwrap();
        assert_eq!(before, identity(root.path(), "tbs-en").unwrap());
    }

    #[test]
    fn changed_editable_build_inputs_invalidate_build_identity() {
        let root = tempfile::tempdir().unwrap();
        for input in [
            "tools/alchemy/src/build_assets.rs",
            "tools/alchemy/src/build_assets/packer.rs",
            "tools/psynergy/src/assets/lz.rs",
            "Makefile",
            "games/THE BROKEN SEAL/BUILD.MK",
            "games/THE BROKEN SEAL/MAIN.LD",
            "games/THE BROKEN SEAL/SRC/BOOT.C",
            "games/THE BROKEN SEAL/INCLUDE/BOOT.H",
            "games/THE BROKEN SEAL/SRC/GRAPHICS/MAP.PNG",
            "games/THE BROKEN SEAL/SOUND/SEQUENCE/A.MID",
            "games/THE BROKEN SEAL/TEXT/EN.PO",
        ] {
            let path = root.path().join(input);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(&path, "before").unwrap();
            let before = identity(root.path(), "tbs-en").unwrap();
            std::fs::write(&path, "after").unwrap();
            assert_ne!(before, identity(root.path(), "tbs-en").unwrap(), "{input}");
        }
    }

    /// A full build proves only the ROM the game registers as its reference,
    /// and only for the tree it started from.
    #[test]
    fn a_full_build_proves_only_the_registered_reference_rom() {
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        let rom = full_build_fixture(root.path(), target, &serde_json::json!({"regions": []}));
        let verified = full_build(root.path(), target).map(|build| build.rom_sha256);
        assert_eq!(verified, Ok(rom));
        let inputs = identity(root.path(), "tbs-en").unwrap();
        let error = full_build_proof(root.path(), target, &inputs, b"another ROM").unwrap_err();
        assert!(
            error.contains("not the approved tbs-en reference ROM"),
            "{error}"
        );
        let reference = b"tbs-en reference ROM";
        let error =
            full_build_proof(root.path(), target, "an earlier tree", reference).unwrap_err();
        assert!(error.contains("changed during the build"), "{error}");
        std::fs::write(root.path().join(target.rom), "another ROM").unwrap();
        let error = full_build(root.path(), target).map(|_| ()).unwrap_err();
        assert!(error.contains("not the local reference ROM"), "{error}");
    }

    #[test]
    fn changed_coverage_calculator_invalidates_build_identity() {
        let root = tempfile::tempdir().unwrap();
        let calculator = root.path().join("tools/alchemy/src/coverage/progress.rs");
        std::fs::create_dir_all(calculator.parent().unwrap()).unwrap();
        std::fs::write(&calculator, "fn tally() -> u64 { 8 }").unwrap();
        let before = identity(root.path(), "tbs-en").unwrap();
        std::fs::write(&calculator, "fn tally() -> u64 { 16 }").unwrap();
        assert_ne!(before, identity(root.path(), "tbs-en").unwrap());
    }
}
