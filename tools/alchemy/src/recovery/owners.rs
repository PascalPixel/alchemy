//! Owner lookup for the recovery aids: the verified ROM holds an owner's
//! bytes, and the images `alchemy build rom` links name and bound it. No
//! register, placeholder or recorded extent is read.

use crate::compiler::build_io::read as read_file;
use crate::targets::DecompTarget;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
use std::process::Command;

pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("alchemy is under tools")
        .to_path_buf()
}

/// An owner named on the command line: a main-image address, or an address
/// in resource coordinates inside one overlay.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub enum Owner {
    Main(u32),
    Overlay { resource: u16, address: u32 },
}
impl Owner {
    /// `ADDR`, `main:ADDR` or `resource_XXX:ADDR`, hexadecimal; an overlay
    /// address below the resource base is an offset into the overlay.
    pub fn parse_argument(input: &str) -> Result<Self, String> {
        let (space, address) = input.split_once(':').unwrap_or(("main", input));
        let mut address = u32::from_str_radix(address.trim_start_matches("0x"), 16)
            .map_err(|_| format!("{input}: address must be hexadecimal"))?;
        if space == "main" {
            return Ok(Self::Main(address));
        }
        let resource = space
            .strip_prefix("resource_")
            .and_then(|id| u16::from_str_radix(id, 16).ok())
            .ok_or_else(|| format!("{input}: expected main:ADDR or resource_XXX:ADDR"))?;
        if address < 0x0200_0000 {
            address += 0x0200_0000;
        }
        Ok(Self::Overlay { resource, address })
    }
    pub fn id(self) -> String {
        match self {
            Self::Main(address) => format!("main:{address:08x}"),
            Self::Overlay { resource, address } => {
                format!("resource_{resource:03x}:{address:08x}")
            }
        }
    }
    pub fn address(self) -> u32 {
        match self {
            Self::Main(address) | Self::Overlay { address, .. } => address,
        }
    }
    pub fn overlay_id(self) -> Option<String> {
        match self {
            Self::Main(_) => None,
            Self::Overlay { resource, .. } => Some(format!("resource_{resource:03x}")),
        }
    }
}

/// The production default target, `tbs-en`.
pub fn default_target() -> DecompTarget {
    crate::targets::target_for(crate::targets::DEFAULT_TARGET)
}

/// The main image `alchemy build rom` links for `target`.
pub fn main_elf(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.output_dir)
        .join(format!("{}.elf", target.id))
}

/// One overlay as `alchemy build rom` links it, at its load address.
pub fn overlay_elf(root: &Path, target: DecompTarget, overlay: &str) -> PathBuf {
    root.join(target.output_dir)
        .join("overlays")
        .join(format!("{overlay}.elf"))
}

/// Every code label a linked image defines, by address: the names its C
/// definitions and assembly labels give the bytes. An image not built yet
/// names nothing.
pub fn linked_names(root: &Path, elf: &Path) -> Result<BTreeMap<u32, String>, String> {
    if !elf.is_file() {
        return Ok(BTreeMap::new());
    }
    let table = psynergy::process::run(
        &["arm-none-eabi-nm", "--defined-only", &elf.to_string_lossy()],
        root,
    )?;
    Ok(code_labels(&table))
}

/// The text symbols of an `nm` table, the first name at each address.
fn code_labels(table: &str) -> BTreeMap<u32, String> {
    let mut names = BTreeMap::new();
    for line in table.lines() {
        let mut fields = line.split_whitespace();
        let (Some(value), Some(kind), Some(name)) = (fields.next(), fields.next(), fields.next())
        else {
            continue;
        };
        if !matches!(kind, "T" | "t") || name.starts_with(['.', '$']) {
            continue;
        }
        if let Ok(value) = u32::from_str_radix(value, 16) {
            names.entry(value & !1).or_insert_with(|| name.to_owned());
        }
    }
    names
}

/// An overlay owner's extent: from its label in the linked overlay to the
/// next label, or to the end of the image.
fn overlay_extent(
    root: &Path,
    target: DecompTarget,
    overlay: &str,
    entry: u32,
    image: usize,
) -> Result<u32, String> {
    let elf = overlay_elf(root, target, overlay);
    if !elf.is_file() {
        return Err(format!(
            "{overlay} is not linked yet; run alchemy build rom --target {} or pass --span",
            target.id
        ));
    }
    let loaded =
        entry - crate::compiler::overlay::RESOURCE_BASE + crate::compiler::overlay::RUNTIME_BASE;
    let end = crate::compiler::overlay::RUNTIME_BASE + image as u32;
    let labels = linked_names(root, &elf)?;
    if !labels.contains_key(&loaded) {
        return Err(format!(
            "{overlay}:{entry:08x} is not a label of the linked overlay; pass --span"
        ));
    }
    let next = labels
        .range(loaded + 1..end)
        .next()
        .map_or(end, |(address, _)| *address);
    Ok(next - loaded)
}

/// Read one complete owner window, independent of its address space.
pub fn image_window(
    root: &Path,
    owner: &str,
    span: Option<u32>,
) -> Result<(Vec<u8>, u32, u32, u32), String> {
    image_window_for(root, default_target(), owner, span)
}

/// `image_window` against one target's ROM and linked images. An explicit
/// `span` bounds the window; otherwise a main owner is its retained
/// listing's size and an overlay owner reaches its linked label's successor.
pub fn image_window_for(
    root: &Path,
    target: DecompTarget,
    owner: &str,
    span: Option<u32>,
) -> Result<(Vec<u8>, u32, u32, u32), String> {
    let owner = Owner::parse_argument(owner)?;
    let entry = owner.address();
    let (image, base, extent) = if let Some(overlay) = owner.overlay_id() {
        let image = crate::overlay::rom::canonical_overlay_for(root, target, &overlay)?;
        let extent = match span {
            Some(span) => span,
            None => overlay_extent(root, target, &overlay, entry, image.len())?,
        };
        (image, psynergy::decode::OVERLAY_BASE, extent)
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn owners_parse_main_and_overlay_forms() {
        assert_eq!(
            Owner::parse_argument("08001234").unwrap(),
            Owner::Main(0x0800_1234)
        );
        assert_eq!(
            Owner::parse_argument("main:0x08001234").unwrap(),
            Owner::Main(0x0800_1234)
        );
        let overlay = Owner::parse_argument("resource_3bf:0200034c").unwrap();
        assert_eq!(overlay, Owner::parse_argument("resource_3bf:34c").unwrap());
        assert_eq!(overlay.id(), "resource_3bf:0200034c");
        assert_eq!(overlay.overlay_id().as_deref(), Some("resource_3bf"));
        for invalid in ["xyz", "resource_3bf:zz", "overlay:02000000", "resource_:0"] {
            assert!(Owner::parse_argument(invalid).is_err(), "{invalid}");
        }
    }

    #[test]
    fn code_labels_keep_text_names_and_skip_markers_and_absolutes() {
        let table = "02008000 A gOverlayArea\n02008030 t .gcc2_compiled.\n02008030 T FixedPoint_Distance\n0200806d T Thumb_Entry\n020080c4 a *ABS*0x20080c4\n020080c4 t Local_Helper\n";
        let labels = code_labels(table);
        assert_eq!(
            labels.into_iter().collect::<Vec<_>>(),
            [
                (0x0200_8030, "FixedPoint_Distance".to_owned()),
                (0x0200_806c, "Thumb_Entry".to_owned()),
                (0x0200_80c4, "Local_Helper".to_owned()),
            ]
        );
    }

    #[test]
    fn an_unlinked_overlay_owner_needs_an_explicit_span() {
        let root = tempfile::tempdir().unwrap();
        let target = default_target();
        let error =
            overlay_extent(root.path(), target, "resource_374", 0x0200_1000, 0x100).unwrap_err();
        assert!(error.contains("--span"), "{error}");
        assert!(linked_names(
            root.path(),
            &overlay_elf(root.path(), target, "resource_374")
        )
        .unwrap()
        .is_empty());
    }
}
