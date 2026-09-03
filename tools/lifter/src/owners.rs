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
        .expect("lifter is under tools")
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
    let text = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let assembly: Assembly =
        serde_json::from_slice(&text).map_err(|error| format!("{}: {error}", path.display()))?;
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
    let (overlay, address) = owner
        .split_once(':')
        .ok_or_else(|| format!("{owner}: expected <overlay>:<addressHex>"))?;
    let mut entry = parse_hex(address)?;
    if entry < crate::decode::OVERLAY_BASE {
        entry += crate::decode::OVERLAY_BASE;
    }
    Ok((overlay.to_string(), entry))
}

/// The registered span of an owner, from the retained module register.
pub fn span_for(root: &Path, overlay: &str, entry: u32) -> Result<u32, String> {
    modules(root)?
        .into_iter()
        .find(|m| m.overlay == overlay && m.entry == entry)
        .map(|m| m.span)
        .ok_or_else(|| format!("{overlay}:{entry:08x} is not a retained module; pass --span"))
}

pub fn overlay_image(root: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    overlay_disasm::rom::canonical_overlay(root, overlay)
}

#[derive(Debug, Clone)]
pub struct Score {
    pub candidate: u32,
    pub reference: u32,
    pub differing: u32,
    pub report: String,
    /// The longer span the candidate is exact at, when its pool outgrows the
    /// registered span into an unregistered gap.
    pub extended: Option<u32>,
}

/// A sibling host tool: the built binary when present, else `cargo run`.
pub fn tool_command(root: &Path, tool: &str) -> Command {
    let built = root.join(format!("out/cargo-target/release/{tool}"));
    if built.is_file() {
        return Command::new(built);
    }
    let mut command = Command::new("cargo");
    command.args([
        "run",
        "--offline",
        "--quiet",
        "--release",
        "--manifest-path",
        &root
            .join(format!("tools/{tool}/Cargo.toml"))
            .to_string_lossy(),
        "--",
    ]);
    command
}

/// Scores a candidate source against an owner through the overlay scorer.
pub fn score(root: &Path, source: &Path, owner: &str, span: u32) -> Result<Score, String> {
    let output = tool_command(root, "overlay")
        .current_dir(root)
        .arg("score")
        .arg(source)
        .args(["--owner", owner, "--span", &span.to_string(), "--align"])
        .output()
        .map_err(|error| format!("overlay score: {error}"))?;
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
        extended: None,
    })
}

/// A module whose literal pool sits past its registered end is exact at its
/// own size. When the candidate outgrows the span and the bytes beyond the
/// span are an unregistered gap, this scores again over the candidate's size
/// and reports that span through `extended`.
pub fn score_extending(
    root: &Path,
    source: &Path,
    owner: &str,
    span: u32,
) -> Result<Score, String> {
    let result = score(root, source, owner, span)?;
    if result.differing == 0 || result.candidate <= result.reference {
        return Ok(result);
    }
    let (overlay, entry) = parse_owner(owner)?;
    let end = entry + result.candidate;
    // An unregistered region wholly inside the extension is absorbed; a
    // registered one, or one straddling the end, keeps the span as it is.
    let taken = modules(root)?.into_iter().any(|m| {
        m.overlay == overlay
            && m.entry != entry
            && m.entry < end
            && m.entry + m.span > entry + span
            && (m.registered || m.entry + m.span > end)
    });
    if taken {
        return Ok(result);
    }
    match score(root, source, owner, result.candidate) {
        Ok(mut extended) if extended.differing == 0 => {
            extended.extended = Some(result.candidate);
            Ok(extended)
        }
        _ => Ok(result),
    }
}

/// The compiler's first complaint in a failed score report.
pub fn first_error(report: &str) -> Option<String> {
    report
        .lines()
        .find(|line| line.contains(".c:") && !line.contains("warning"))
        .map(|line| {
            line.split_once(".c:")
                .map(|(_, rest)| rest.to_string())
                .unwrap_or_else(|| line.to_string())
        })
}
