//! Owner lookup against the repository: the retained overlay listings,
//! the source register, the canonical ROM, and the overlay scorer.

use crate::compiler::build_io::read as read_file;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::targets::DecompTarget;
use std::path::{Path, PathBuf};
use std::process::Command;

pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("alchemy is under tools")
        .to_path_buf()
}

#[derive(Debug, Clone)]
pub struct Module {
    pub overlay: String,
    pub entry: u32,
    pub span: u32,
    pub name: String,
    pub registered: bool,
}

impl Module {
    pub fn key(&self) -> String {
        format!("{}:{:08x}", self.overlay, self.entry)
    }
}

/// Every not-yet-C overlay owner its listing labels, in address order.
pub fn modules(root: &Path) -> Result<Vec<Module>, String> {
    let sources = SourcePaths::load(root)?;
    Ok(
        crate::overlay::owners::listed_owners(root, default_target())?
            .into_iter()
            .map(|(owner, (name, extent))| Module {
                registered: sources.mapped_relative_path(owner).is_some(),
                overlay: owner.overlay_id().unwrap_or_default(),
                entry: owner.address(),
                span: extent as u32,
                name,
            })
            .collect(),
    )
}

/// Parses `<overlay>:<hex>` into its parts.
pub fn parse_owner(owner: &str) -> Result<(String, u32), String> {
    let resolved = SourceOwner::parse_argument(owner)?;
    let overlay = resolved
        .overlay_id()
        .ok_or_else(|| format!("{owner}: expected <overlay>:<addressHex>"))?;
    Ok((overlay, resolved.address()))
}

/// Resolve an overlay owner from its listing bounds or its installed C placeholder.
/// A caller-supplied span confirms the extent; it cannot establish a new owner.
pub fn span_for(
    root: &Path,
    overlay: &str,
    entry: u32,
    requested: Option<u32>,
) -> Result<u32, String> {
    span_for_target(root, default_target(), overlay, entry, requested)
}

/// The production default target, `tbs-en`, which every legacy entry point assumes.
pub fn default_target() -> DecompTarget {
    crate::targets::target_for(crate::targets::DEFAULT_TARGET)
}

/// `span_for` against one registered target's reviewed register and retained assembly.
pub fn span_for_target(
    root: &Path,
    target: DecompTarget,
    overlay: &str,
    entry: u32,
    requested: Option<u32>,
) -> Result<u32, String> {
    let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
    let reviewed = crate::overlay::owners::owner_spans(root, target)?;
    let paths = SourcePaths::load_for_game(root, target.compiler.as_str())?;
    let installed = if paths
        .mapped_source_path(owner)
        .is_some_and(|path| path.is_file())
    {
        let path = root.join(target.overlay_assembly(overlay));
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        crate::compiler::overlay::placeholder_extent(&text, entry)
    } else {
        None
    };
    let span = crate::compiler::translation_units::resolve_overlay_span(
        &reviewed,
        owner,
        installed,
        requested.map(|span| span as usize),
    )?;
    u32::try_from(span).map_err(|_| format!("{}: owner extent exceeds address space", owner.id()))
}

#[derive(Debug, Clone)]
pub struct Score {
    pub candidate: u32,
    pub reference: u32,
    pub differing: u32,
}

/// Re-enter the unified executable, falling back to Cargo before installation.
pub fn tool_command(root: &Path, group: &str) -> Command {
    let built = root.join("tools/out/cargo-target/release/alchemy");
    let mut command = if built.is_file() {
        Command::new(built)
    } else {
        let mut command = Command::new("cargo");
        command.args([
            "run",
            "--offline",
            "--quiet",
            "--release",
            "--manifest-path",
            &root.join("tools/alchemy/Cargo.toml").to_string_lossy(),
            "--",
        ]);
        command
    };
    if !group.is_empty() {
        command.arg(group);
    }
    command
}

/// Scores a candidate source against an owner through the overlay scorer.
pub fn score(root: &Path, source: &Path, owner: &str, span: u32) -> Result<Score, String> {
    score_in(root, source, owner, span, None)
}

/// Scores in a named work directory. An empty one holds no compile cache, so
/// the compiler runs again rather than answering from an earlier result.
pub fn score_in(
    root: &Path,
    source: &Path,
    owner: &str,
    span: u32,
    work: Option<&Path>,
) -> Result<Score, String> {
    let owner = SourceOwner::parse_argument(owner)?;
    let extent_flag = if owner.is_main() { "--size" } else { "--span" };
    let mut command = tool_command(root, "");
    command.current_dir(root).arg("score").arg(source).args([
        "--owner",
        &owner.id(),
        extent_flag,
        &span.to_string(),
        "--align",
    ]);
    if let Some(work) = work {
        command.arg("--work").arg(work);
    }
    let output = command.output().map_err(|error| format!("diff: {error}"))?;
    let report = format!(
        "{}{}",
        String::from_utf8_lossy(&output.stdout),
        String::from_utf8_lossy(&output.stderr)
    );
    let line = report
        .lines()
        .find(|line| line.starts_with("candidate="))
        .ok_or_else(|| {
            let tail: Vec<&str> = report
                .lines()
                .filter(|line| line.contains("error") || line.contains("cand"))
                .take(4)
                .collect();
            format!("no score line; {}", tail.join(" | "))
        })?;
    let mut fields = line.split_whitespace().filter_map(|field| {
        field
            .split_once('=')
            .and_then(|(_, value)| value.parse::<u32>().ok())
    });
    let candidate = fields.next().ok_or("score line lacks candidate")?;
    let reference = fields.next().ok_or("score line lacks reference")?;
    let differing = fields
        .next()
        .ok_or("score line lacks differing_halfwords")?;
    Ok(Score {
        candidate,
        reference,
        differing,
    })
}

/// Read one complete owner window, independent of its address space.
pub fn image_window(
    root: &Path,
    owner: &str,
    span: Option<u32>,
) -> Result<(Vec<u8>, u32, u32, u32), String> {
    image_window_for(root, default_target(), owner, span)
}

/// `image_window` against one registered target's ROM, register and assembly.
pub fn image_window_for(
    root: &Path,
    target: DecompTarget,
    owner: &str,
    span: Option<u32>,
) -> Result<(Vec<u8>, u32, u32, u32), String> {
    let owner = SourceOwner::parse_argument(owner)?;
    let entry = owner.address();
    let (image, base, extent) = if let Some(overlay) = owner.overlay_id() {
        let extent = span_for_target(root, target, &overlay, entry, span)?;
        (
            crate::overlay::rom::canonical_overlay_for(root, target, &overlay)?,
            psynergy::decode::OVERLAY_BASE,
            extent,
        )
    } else {
        let extent = match span {
            Some(span) => span,
            None => main_extent_for(root, target, entry)?,
        };
        (
            read_file(root.join(target.rom))?,
            psynergy::decode::MAIN_BASE,
            extent,
        )
    };
    let end = entry
        .checked_sub(base)
        .and_then(|start| start.checked_add(extent));
    if extent == 0 || !end.is_some_and(|end| end as usize <= image.len()) {
        return Err(format!("{}: owner extent is outside the image", owner.id()));
    }
    Ok((image, base, entry, extent))
}

#[cfg(test)]
mod owner_tests {
    use super::*;

    #[test]
    fn listing_labels_and_requested_spans_cannot_create_owners() {
        let root = tempfile::tempdir().unwrap();
        let listings = root.path().join("recon/tbs/raw/overlays");
        std::fs::create_dir_all(&listings).unwrap();
        std::fs::write(
            listings.join("resource_374_overlay.s"),
            "\t.space 0x1000\nScene_Run:\n\t.space 0x200\n",
        )
        .unwrap();
        assert_eq!(
            span_for(root.path(), "resource_374", 0x02001000, None).unwrap(),
            512
        );
        assert_eq!(
            span_for(root.path(), "resource_374", 0x02001000, Some(512)).unwrap(),
            512
        );
        assert!(span_for(root.path(), "resource_374", 0x02001000, Some(32)).is_err());
        let error = image_window(root.path(), "resource_374:02001010", Some(32)).unwrap_err();
        assert!(error.contains("reviewed"), "{error}");
        assert!(crate::recovery::adopt::adopt(
            root.path(),
            &crate::recovery::adopt::Request {
                owner: "resource_374:02001010",
                span: Some(32),
                name: None,
                path: None,
                source: None,
            }
        )
        .is_err());
        assert!(!root.path().join("games/THE BROKEN SEAL/SRC").exists());
    }
}

/// The extent of a main owner: its retained assembly under the target's `asm`
/// directory assembled and measured, exactly as the integration gate measures it.
pub fn main_extent_for(root: &Path, target: DecompTarget, address: u32) -> Result<u32, String> {
    let source = root.join(format!("{}/{address:08x}.s", target.asm_dir));
    if !source.is_file() {
        return Err(format!("main:{address:08x} has no retained assembly"));
    }
    let scratch = root.join("out/recovery/main/extent");
    std::fs::create_dir_all(&scratch).map_err(|error| format!("{}: {error}", scratch.display()))?;
    let object = scratch.join(format!("{address:08x}.o"));
    let binary = scratch.join(format!("{address:08x}.bin"));
    let assembled = Command::new("arm-none-eabi-as")
        .args(["-mcpu=arm7tdmi", "-mthumb-interwork", "-o"])
        .arg(&object)
        .arg(&source)
        .output()
        .map_err(|error| format!("arm-none-eabi-as: {error}"))?;
    if !assembled.status.success() {
        return Err(format!(
            "as failed for main:{address:08x}: {}",
            String::from_utf8_lossy(&assembled.stderr).trim()
        ));
    }
    let copied = Command::new("arm-none-eabi-objcopy")
        .args(["-O", "binary", "-j", ".text"])
        .arg(&object)
        .arg(&binary)
        .output()
        .map_err(|error| format!("arm-none-eabi-objcopy: {error}"))?;
    if !copied.status.success() {
        return Err(format!("objcopy failed for main:{address:08x}"));
    }
    let size = std::fs::metadata(&binary)
        .map_err(|error| format!("{}: {error}", binary.display()))?
        .len();
    Ok(size as u32)
}
