//! Owner lookup against the repository: the retained module register,
//! the source register, the canonical ROM, and the overlay scorer.

use compiler_core::source_paths::{SourceOwner, SourcePaths};
use serde::Deserialize;
use std::path::{Path, PathBuf};
use std::process::Command;

pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("decompile is under tools")
        .to_path_buf()
}

#[derive(Debug, Clone)]
pub struct Module {
    pub overlay: String,
    pub entry: u32,
    pub span: u32,
    pub kind: String,
    pub registered: bool,
}

impl Module {
    pub fn key(&self) -> String {
        format!("{}:{:08x}", self.overlay, self.entry)
    }
}

#[derive(Deserialize)]
struct Assembly {
    regions: Vec<Region>,
}

#[derive(Deserialize)]
struct Region {
    overlay: String,
    start: String,
    end: String,
    kind: String,
}

fn parse_hex(text: &str) -> Result<u32, String> {
    u32::from_str_radix(text.trim_start_matches("0x"), 16).map_err(|_| format!("{text}: not hex"))
}

/// Every retained overlay module, in register order.
pub fn modules(root: &Path) -> Result<Vec<Module>, String> {
    let path = root.join("games/gs1/semantic/overlay-assembly.json");
    let assembly: Assembly = compiler_core::build_io::read_json(&path)?;
    let sources = SourcePaths::load(root)?;
    let mut modules = Vec::new();
    for region in assembly.regions {
        let entry = parse_hex(&region.start)?;
        let end = parse_hex(&region.end)?;
        let owner = SourceOwner::parse(&format!("{}:{entry:08x}", region.overlay))?;
        modules.push(Module {
            registered: sources.mapped_relative_path(owner).is_some(),
            overlay: region.overlay,
            entry,
            span: end.saturating_sub(entry),
            kind: region.kind,
        });
    }
    Ok(modules)
}

/// Parses `<overlay>:<hex>` into its parts.
pub fn parse_owner(owner: &str) -> Result<(String, u32), String> {
    let resolved = SourceOwner::parse_argument(owner)?;
    let overlay = resolved
        .overlay_id()
        .ok_or_else(|| format!("{owner}: expected <overlay>:<addressHex>"))?;
    Ok((overlay, resolved.address()))
}

/// Resolve an overlay owner from reviewed bounds or its installed C placeholder.
/// A caller-supplied span confirms the extent; it cannot establish a new owner.
pub fn span_for(
    root: &Path,
    overlay: &str,
    entry: u32,
    requested: Option<u32>,
) -> Result<u32, String> {
    let owner = SourceOwner::parse(&format!("{overlay}:{entry:08x}"))?;
    let reviewed = compiler_core::translation_units::reviewed_overlay_spans(root)?;
    let paths = SourcePaths::load(root)?;
    let installed = if paths
        .mapped_source_path(owner)
        .is_some_and(|path| path.is_file())
    {
        let path = root.join(format!("games/gs1/assets/code/{overlay}_overlay.s"));
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        compiler_core::overlay::placeholder_extent(&text, entry)
    } else {
        None
    };
    let span = compiler_core::translation_units::resolve_overlay_span(
        &reviewed,
        owner,
        installed,
        requested.map(|span| span as usize),
    )?;
    u32::try_from(span).map_err(|_| format!("{}: owner extent exceeds address space", owner.id()))
}

pub fn overlay_image(root: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    disassemble::rom::canonical_overlay(root, overlay)
}

#[derive(Debug, Clone)]
pub struct Score {
    pub candidate: u32,
    pub reference: u32,
    pub differing: u32,
    pub report: String,
}

/// Re-enter the unified executable, falling back to Cargo before installation.
pub fn tool_command(root: &Path, group: &str) -> Command {
    let built = root.join("out/cargo-target/release/alchemy");
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
    let owner = SourceOwner::parse_argument(owner)?;
    let extent_flag = if owner.is_main() { "--size" } else { "--span" };
    let output = tool_command(root, "")
        .current_dir(root)
        .arg("diff")
        .arg(source)
        .args([
            "--owner",
            &owner.id(),
            extent_flag,
            &span.to_string(),
            "--align",
        ])
        .output()
        .map_err(|error| format!("diff: {error}"))?;
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
        report,
    })
}

/// The canonical main image, read once per call: the ROM as loaded at
/// `decode::MAIN_BASE`.
pub fn main_image(root: &Path) -> Result<Vec<u8>, String> {
    let path = root.join("roms/gs1-en.gba");
    std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
}

/// Resolve main owners with the same spelling rules as the public command.
pub fn parse_main_owner(owner: &str) -> Option<u32> {
    let owner = SourceOwner::parse_argument(owner).ok()?;
    owner.is_main().then(|| owner.address())
}

/// Read one complete owner window, independent of its address space.
pub fn image_window(
    root: &Path,
    owner: &str,
    span: Option<u32>,
) -> Result<(Vec<u8>, u32, u32, u32), String> {
    let owner = SourceOwner::parse_argument(owner)?;
    let entry = owner.address();
    let (image, base, extent) = if let Some(overlay) = owner.overlay_id() {
        let extent = span_for(root, &overlay, entry, span)?;
        (
            overlay_image(root, &overlay)?,
            crate::decode::OVERLAY_BASE,
            extent,
        )
    } else {
        let extent = match span {
            Some(span) => span,
            None => main_extent(root, entry)?,
        };
        (main_image(root)?, crate::decode::MAIN_BASE, extent)
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
    fn retained_regions_and_requested_spans_cannot_create_owners() {
        let root = tempfile::tempdir().unwrap();
        let semantic = root.path().join("games/gs1/semantic");
        std::fs::create_dir_all(&semantic).unwrap();
        std::fs::write(semantic.join("regions.json"), r#"{"manual_regions":[{"overlay":"resource_374","entry":"0x02001000","span_bytes":512}]}"#).unwrap();
        std::fs::write(semantic.join("overlay-assembly.json"), r#"{"regions":[{"overlay":"resource_374","start":"0x02001010","end":"0x02001030","kind":"structured_scene_module"}]}"#).unwrap();
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
        assert!(crate::adopt::adopt(
            root.path(),
            &crate::adopt::Request {
                owner: "resource_374:02001010",
                span: Some(32),
                name: None,
                path: None,
                source: None,
            }
        )
        .is_err());
        assert!(!root.path().join("games/gs1/src").exists());
    }

    #[test]
    fn main_addresses_share_the_register_parser() {
        for spelling in ["080bbb0c", "0x080bbb0c", "main:080bbb0c"] {
            assert_eq!(parse_main_owner(spelling), Some(0x080bbb0c));
        }
        for spelling in ["resource_3ba:02002910", "main:garbage", ""] {
            assert_eq!(parse_main_owner(spelling), None);
        }
    }
}

/// The extent of a main owner: its retained assembly under `games/gs1/asm`
/// assembled and measured, exactly as the integration gate measures it.
pub fn main_extent(root: &Path, address: u32) -> Result<u32, String> {
    let source = root.join(format!("games/gs1/asm/{address:08x}.s"));
    if !source.is_file() {
        return Err(format!("main:{address:08x} has no retained assembly"));
    }
    let scratch = root.join("out/decompile/main/extent");
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
