//! Publish locally verified measurements, never the private build inputs.
use super::decomp::REPORT;
use sha2::{Digest, Sha256};
use std::path::Path;
use std::process::Command;

const REPO: &str = "PascalPixel/alchemy";
const RELEASE: &str = "decomp-reports";
const RECEIPT: &str = "out/reports/decomp/publication.tsv";
const STALE: &str = "decomp.dev report is stale or missing: run make land before committing main";

fn command(root: &Path, program: &str, arguments: &[&str]) -> Result<String, String> {
    let output = Command::new(program)
        .args(arguments)
        .current_dir(root)
        .output()
        .map_err(|error| format!("cannot run {program}: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "{program} failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(String::from_utf8_lossy(&output.stdout).trim().to_string())
}

fn digest(bytes: &[u8]) -> String {
    format!("sha256:{:x}", Sha256::digest(bytes))
}

fn receipt(tree: &str, bytes: &[u8]) -> String {
    format!("{tree}\t{}\n", digest(bytes))
}

fn valid_id(id: &str) -> bool {
    id.len() == 40 && id.bytes().all(|b| b.is_ascii_hexdigit())
}

fn canonical_remote(remote: &str) -> bool {
    [
        "https://github.com/PascalPixel/alchemy",
        "https://github.com/PascalPixel/alchemy.git",
        "git@github.com:PascalPixel/alchemy.git",
        "ssh://git@github.com/PascalPixel/alchemy.git",
    ]
    .iter()
    .any(|url| remote.eq_ignore_ascii_case(url))
}

/// Called only after make land's complete build, comparison and publication
/// gates; the coverage gate wrote the report from their one measurement.
pub(crate) fn prepare(root: &Path) -> Result<(), String> {
    let tree = command(root, "git", &["write-tree"])?;
    let bytes = std::fs::read(root.join(REPORT)).map_err(|_| STALE.to_string())?;
    std::fs::write(root.join(RECEIPT), receipt(&tree, &bytes))
        .map_err(|error| error.to_string())?;
    println!("decomp.dev report prepared for source tree {tree}");
    Ok(())
}

fn prepared(root: &Path, tree: &str) -> Result<Vec<u8>, String> {
    let bytes = std::fs::read(root.join(REPORT)).map_err(|_| STALE.to_string())?;
    let saved = std::fs::read_to_string(root.join(RECEIPT)).map_err(|_| STALE.to_string())?;
    if !valid_id(tree) || saved != receipt(tree, &bytes) {
        return Err(STALE.into());
    }
    Ok(bytes)
}

fn needs_upload(remote_digest: &str, bytes: &[u8]) -> Result<bool, String> {
    match remote_digest {
        "" => Ok(true),
        value if value == digest(bytes) => Ok(false),
        _ => Err(
            "existing report for this commit has a different digest; refusing to replace it".into(),
        ),
    }
}

pub(crate) fn upload(root: &Path, commit: &str, remote: &str) -> Result<(), String> {
    if !canonical_remote(remote) {
        return Ok(());
    }
    if !valid_id(commit) {
        return Err("invalid source commit for report publication".into());
    }
    let tree = command(root, "git", &["rev-parse", &format!("{commit}^{{tree}}")])?;
    let bytes = prepared(root, &tree)?;
    let name = format!("{commit}.pb");
    let query = format!(".assets[] | select(.name == \"{name}\") | .digest");
    let remote_digest = command(
        root,
        "gh",
        &[
            "api",
            &format!("repos/{REPO}/releases/tags/{RELEASE}"),
            "--jq",
            &query,
        ],
    )?;
    if needs_upload(&remote_digest, &bytes)? {
        let directory = root.join("out/reports/decomp/upload");
        std::fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
        let path = directory.join(name);
        std::fs::write(&path, &bytes).map_err(|error| error.to_string())?;
        command(
            root,
            "gh",
            &[
                "release",
                "upload",
                RELEASE,
                path.to_str().ok_or("invalid report path")?,
                "--repo",
                REPO,
            ],
        )?;
        let uploaded = command(
            root,
            "gh",
            &[
                "api",
                &format!("repos/{REPO}/releases/tags/{RELEASE}"),
                "--jq",
                &query,
            ],
        )?;
        if needs_upload(&uploaded, &bytes)? {
            return Err("uploaded report is absent from the release".into());
        }
    }
    println!("decomp.dev report uploaded for {commit}; GitHub will publish it after the push");
    Ok(())
}
