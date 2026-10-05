//! Publish locally verified measurements, never the private build inputs.
//! `make land` prepares them; the pre-push hook uploads them by commit to
//! the `decomp-reports` release, where CI draws the progress figures and
//! hands decomp.dev its report.
use super::decomp::REPORT;
use sha2::{Digest, Sha256};
use std::path::Path;
use std::process::Command;

/// Both games' DONE parts, as CI records them in the progress history.
pub(crate) const MEASUREMENT: &str = "out/reports/decomp/measurement.tsv";

const REPO: &str = "PascalPixel/alchemy";
const RELEASE: &str = "decomp-reports";
const RECEIPT: &str = "out/reports/decomp/publication.tsv";
const STALE: &str =
    "progress measurement is stale or missing: run make land before committing main";
/// What a push uploads: the decomp.dev report and the measurement, each
/// named by the commit it measures.
const ASSETS: [(&str, &str); 2] = [(REPORT, "pb"), (MEASUREMENT, "tsv")];

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

fn receipt(tree: &str, files: &[Vec<u8>]) -> String {
    let digests = files.iter().map(|bytes| digest(bytes)).collect::<Vec<_>>();
    format!("{tree}\t{}\n", digests.join("\t"))
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

fn read_assets(root: &Path) -> Result<Vec<Vec<u8>>, String> {
    ASSETS
        .iter()
        .map(|(path, _)| std::fs::read(root.join(path)).map_err(|_| STALE.to_string()))
        .collect()
}

/// Called only after make land's complete build, comparison and publication
/// gates; the coverage report gate wrote both files from their one measurement.
pub(crate) fn prepare(root: &Path) -> Result<(), String> {
    let tree = command(root, "git", &["write-tree"])?;
    let files = read_assets(root)?;
    std::fs::write(root.join(RECEIPT), receipt(&tree, &files))
        .map_err(|error| error.to_string())?;
    println!("progress measurement prepared for source tree {tree}");
    Ok(())
}

fn prepared(root: &Path, tree: &str) -> Result<Vec<Vec<u8>>, String> {
    let files = read_assets(root)?;
    let saved = std::fs::read_to_string(root.join(RECEIPT)).map_err(|_| STALE.to_string())?;
    if !valid_id(tree) || saved != receipt(tree, &files) {
        return Err(STALE.into());
    }
    Ok(files)
}

fn needs_upload(remote_digest: &str, bytes: &[u8]) -> Result<bool, String> {
    match remote_digest {
        "" => Ok(true),
        value if value == digest(bytes) => Ok(false),
        _ => Err(
            "existing measurement for this commit has a different digest; refusing to replace it"
                .into(),
        ),
    }
}

fn remote_digest(root: &Path, name: &str) -> Result<String, String> {
    let query = format!(".assets[] | select(.name == \"{name}\") | .digest");
    command(
        root,
        "gh",
        &[
            "api",
            &format!("repos/{REPO}/releases/tags/{RELEASE}"),
            "--jq",
            &query,
        ],
    )
}

pub(crate) fn upload(root: &Path, commit: &str, remote: &str) -> Result<(), String> {
    if !canonical_remote(remote) {
        return Ok(());
    }
    if !valid_id(commit) {
        return Err("invalid source commit for report publication".into());
    }
    let tree = command(root, "git", &["rev-parse", &format!("{commit}^{{tree}}")])?;
    let files = prepared(root, &tree)?;
    let directory = root.join("out/reports/decomp/upload");
    std::fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    for ((_, extension), bytes) in ASSETS.iter().zip(&files) {
        let name = format!("{commit}.{extension}");
        if !needs_upload(&remote_digest(root, &name)?, bytes)? {
            continue;
        }
        let path = directory.join(&name);
        std::fs::write(&path, bytes).map_err(|error| error.to_string())?;
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
        if needs_upload(&remote_digest(root, &name)?, bytes)? {
            return Err(format!("uploaded {name} is absent from the release"));
        }
    }
    println!("progress measurement uploaded for {commit}; CI publishes it after the push");
    Ok(())
}
