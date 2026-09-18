//! Build receipts: verified ranges and the inputs that produced them.
//! These are generated under out/, never a second editable classification register.
use crate::compiler::sha256;
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

/// Hash maintained build inputs, including headers, placement, flags and
/// compiler/assembler executables. Assets and presentation files do not grant
/// code credit. Reading this identity does not run a compiler or build a tree.
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
        format!("{}/raw", game.game_dir()),
        format!("{}/semantic", game.game_dir()),
        "games/COMMON/SRC".into(),
        "games/COMMON/INCLUDE".into(),
    ] {
        collect(root, &root.join(folder), &mut files)?;
    }
    for suffix in [
        "source-paths.json",
        "project.json",
        "PROJECT.JSON",
        "recon/translation-units.json",
        "recon/compiler-runtime.json",
        "raw/classification.json",
        "semantic/overlay-assembly.json",
    ] {
        let path = format!("{}/{suffix}", game.game_dir());
        if root.join(&path).is_file() {
            files.push(path);
        }
    }
    collect(root, &root.join("tools/alchemy/src/compiler"), &mut files)?;
    for directory in [
        "tools/alchemy/src/overlay",
        "tools/alchemy/src/score",
        "tools/psynergy/src",
    ] {
        collect(root, &root.join(directory), &mut files)?;
    }
    collect(root, &root.join("tools/compilers"), &mut files)?;
    collect(root, &root.join("tools/binutils/bin"), &mut files)?;
    for path in [
        "tools/alchemy/src/build_claimed.rs",
        "tools/alchemy/src/build_asm.rs",
        "tools/alchemy/src/build_full.rs",
        "tools/alchemy/src/targets.rs",
        "tools/alchemy/src/check/tla_owners.rs",
        "tools/alchemy/src/overlay/compile.rs",
        "tools/alchemy/src/coverage/proof.rs",
        "tools/alchemy/src/coverage/pipeline.rs",
        "tools/alchemy/src/candidate.rs",
    ] {
        if root.join(path).is_file() {
            files.push(path.into());
        }
    }
    files.sort();
    files.dedup();
    let mut input = Vec::new();
    for path in files {
        let bytes = std::fs::read(root.join(&path)).map_err(|e| format!("{path}: {e}"))?;
        input.extend_from_slice(path.as_bytes());
        input.push(0);
        input.extend_from_slice(sha256::hex(&bytes).as_bytes());
        input.push(0);
    }
    Ok(sha256::hex(&input))
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
    fn changed_overlay_plan_invalidates_build_identity() {
        let root = tempfile::tempdir().unwrap();
        let raw = root.path().join("games/THE LOST AGE/raw/overlays");
        std::fs::create_dir_all(&raw).unwrap();
        let plan = raw.join("resource_001.plan.json");
        std::fs::write(&plan, "{}").unwrap();
        let before = identity(root.path(), "tla-en").unwrap();
        std::fs::write(&plan, "{\"start\":32}").unwrap();
        assert_ne!(before, identity(root.path(), "tla-en").unwrap());
    }
}
