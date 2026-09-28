//! Build receipts: verified ranges and the inputs that produced them, and
//! the full ROM build's proof of the main image's asset complement.
//! These are generated under out/, never a second editable classification register.
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

/// Hash maintained build inputs, including headers, placement, bindings,
/// flags, retained listings and compiler/assembler executables, and the asset
/// build that decides the main image's complement: asset sources and
/// manifests, the asset build and its codecs, and the reference machine
/// definition. A changed input invalidates both the receipt and the full
/// build's main-image proof. Presentation files and generated plans grant
/// nothing. Reading this identity does not run a compiler or build a tree.
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
        format!("{}/semantic", game.recon_dir()),
        "games/COMMON/SRC".into(),
        "games/COMMON/INCLUDE".into(),
    ] {
        collect(root, &root.join(folder), &mut files)?;
    }
    // Retained listings: the main image's assembly stage and the overlay and
    // battle packages the asset build assembles.
    let mut listings = Vec::new();
    collect(root, &root.join(game.asm_dir), &mut listings)?;
    files.extend(
        listings
            .into_iter()
            .filter(|path| path.ends_with(".s") || path.ends_with(".inc")),
    );
    for suffix in [
        "source-paths.json",
        "source-bindings.json",
        "translation-units.json",
        "compiler-runtime.json",
        "semantic/overlay-assembly.json",
        "machine.json",
        "assets.json",
        "private-inputs.json",
        "text.json",
    ] {
        let path = format!("{}/{suffix}", game.recon_dir());
        if root.join(&path).is_file() {
            files.push(path);
        }
    }
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

/// The sha256 of each file, hashed across threads. A build asks for its
/// identity several times to prove nothing changed while it ran, so a digest
/// is reused while the file keeps its size and modification time.
fn file_digests(root: &Path, files: &[String]) -> Result<Vec<String>, String> {
    use std::collections::HashMap;
    use std::sync::{Mutex, OnceLock};
    type Seen = HashMap<std::path::PathBuf, (u64, std::time::SystemTime, String)>;
    static SEEN: OnceLock<Mutex<Seen>> = OnceLock::new();
    let seen = SEEN.get_or_init(Default::default);
    let digest = |path: &String| -> Result<String, String> {
        let full = root.join(path);
        let metadata = std::fs::metadata(&full).map_err(|e| format!("{path}: {e}"))?;
        let stamp = (
            metadata.len(),
            metadata.modified().map_err(|e| format!("{path}: {e}"))?,
        );
        if let Some((len, modified, digest)) = seen.lock().unwrap().get(&full) {
            if (*len, *modified) == stamp {
                return Ok(digest.clone());
            }
        }
        let bytes = std::fs::read(&full).map_err(|e| format!("{path}: {e}"))?;
        let digest = sha256::hex(&bytes);
        seen.lock()
            .unwrap()
            .insert(full, (stamp.0, stamp.1, digest.clone()));
        Ok(digest)
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
    let path = root.join(format!("out/{target}/reports/verified-code.json"));
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    let receipt = Receipt {
        format: 1,
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
    if receipt.format != 1
        || receipt.target != target
        || receipt.inputs_sha256 != identity(root, target)?
    {
        return Err(format!("{target}: verified code is stale; verify the changed source before publishing progress"));
    }
    let rom = crate::targets::decomp_target(Some(target))?;
    let hash = sha256::hex(&std::fs::read(root.join(rom.rom)).map_err(|e| e.to_string())?);
    if hash != receipt.rom_sha256 {
        return Err(format!("{target}: reference ROM changed"));
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

/// The sha256 of the reference ROM a full build of `target` must reproduce,
/// as the game's tracked text layout records it: the ROM its private sources
/// are restored from. A local ROM file never vouches for itself.
fn reference_sha256(root: &Path, target: DecompTarget) -> Result<String, String> {
    crate::text_catalog::reference_sha256(root, target.id.as_str())
}

/// The proof a full build of `target` records once it has rebuilt `rom`
/// byte for byte at the canonical locations. `inputs` is the [`identity`]
/// the build started from; a tree that changed while it ran proves nothing,
/// and neither does a ROM other than the registered reference.
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
            "the rebuilt ROM is not the reference ROM {}/text.json registers; its main-image proof is withheld",
            target.recon_dir()
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
    if report["byte_identical"] != true
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
            "{rebuilt} is not the reference ROM {}/text.json registers",
            target.recon_dir()
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
    let registry = format!("{}/text.json", target.recon_dir());
    let mut layouts: Vec<serde_json::Value> = std::fs::read(root.join(&registry))
        .map(|bytes| serde_json::from_slice(&bytes).unwrap())
        .unwrap_or_default();
    layouts.retain(|layout| layout["target"] != target.id.as_str());
    layouts.push(serde_json::json!({"target":target.id.as_str(),"rom_sha256":sha256::hex(&rom)}));
    write(
        &registry,
        serde_json::Value::from(layouts).to_string().as_bytes(),
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

    /// The asset build decides the main image's complement, so its sources,
    /// codecs, machine definitions and the listings it assembles are inputs
    /// as much as the compiler's.
    #[test]
    fn changed_asset_build_inputs_invalidate_build_identity() {
        let root = tempfile::tempdir().unwrap();
        for input in [
            "tools/alchemy/src/build_assets.rs",
            "tools/alchemy/src/build_assets/packer.rs",
            "tools/psynergy/src/assets/lz.rs",
            "recon/tbs/machine.json",
            "recon/tbs/assets.json",
            "recon/tbs/private-inputs.json",
            "recon/tbs/source-bindings.json",
            "recon/tbs/raw/0800a000.s",
            "recon/tbs/raw/overlays/resource_001_overlay.s",
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
            error.contains("not the reference ROM recon/tbs/text.json registers"),
            "{error}"
        );
        let reference = b"tbs-en reference ROM";
        let error =
            full_build_proof(root.path(), target, "an earlier tree", reference).unwrap_err();
        assert!(error.contains("changed during the build"), "{error}");
        std::fs::write(root.path().join("recon/tbs/text.json"), "[]").unwrap();
        let inputs = identity(root.path(), "tbs-en").unwrap();
        let error = full_build_proof(root.path(), target, &inputs, reference).unwrap_err();
        assert!(error.contains("registers no reference ROM"), "{error}");
        let error = full_build(root.path(), target).map(|_| ()).unwrap_err();
        assert!(error.contains("registers no reference ROM"), "{error}");
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
