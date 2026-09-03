//! What an overlay's calls really reach. An overlay `bl` stores the target
//! image offset minus two, not a PC-relative displacement, so the pseudo
//! symbol a unit spells for a site (`Func_02004280`, the PC-relative decode)
//! is particular to that site. The real target is `symbol - site - 2` into
//! the image: a veneer (`ldr r4, [pc]; bx r4; .word main`) names an import
//! from the main image, and a prologue names a function of the overlay
//! itself. Registered names come from the source register.

use crate::decode::{decode_window, Kind, OVERLAY_BASE};
use crate::owners::{overlay_image, parse_owner, span_for};
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use std::path::Path;

/// One call site of a lifted unit, resolved.
#[derive(Debug, Clone, serde::Serialize)]
pub struct Import {
    /// The site's address.
    pub site: u32,
    /// The pseudo symbol the unit spells for the site.
    pub symbol: String,
    /// The real target's address in the overlay.
    pub target: u32,
    /// `veneer`, `prologue`, or `other`.
    pub kind: &'static str,
    /// The main-image function a veneer imports.
    pub main: Option<u32>,
    /// The registered name of the import or overlay function, if any.
    pub name: Option<String>,
}

fn read_u16(image: &[u8], at: usize) -> Option<u16> {
    Some(u16::from_le_bytes([*image.get(at)?, *image.get(at + 1)?]))
}

fn read_u32(image: &[u8], at: usize) -> Option<u32> {
    Some(u32::from_le_bytes([
        *image.get(at)?,
        *image.get(at + 1)?,
        *image.get(at + 2)?,
        *image.get(at + 3)?,
    ]))
}

/// Resolves every call site of an owner's window.
pub fn imports(root: &Path, owner: &str, span: Option<u32>) -> Result<Vec<Import>, String> {
    let (overlay, entry) = parse_owner(owner)?;
    let span = match span {
        Some(span) => span,
        None => span_for(root, &overlay, entry)?,
    };
    let image = overlay_image(root, &overlay)?;
    let sources = SourcePaths::load(root)?;
    if (entry - OVERLAY_BASE) as usize >= image.len() {
        return Err(format!("{owner}: entry past the image end"));
    }
    let ins = decode_window(&image, entry, span);
    let mut found = Vec::new();
    for x in &ins {
        let Kind::Bl { target: symbol } = x.kind else {
            continue;
        };
        let offset = symbol as i64 - x.addr as i64 - 2;
        if offset < 0 || offset as usize + 8 > image.len() {
            continue;
        }
        let at = offset as usize;
        let target = OVERLAY_BASE + offset as u32;
        let first = read_u16(&image, at).unwrap_or(0);
        let second = read_u16(&image, at + 2).unwrap_or(0);
        let (kind, main) = if first == 0x4c00 && second == 0x4720 {
            ("veneer", read_u32(&image, at + 4).map(|w| w & !1))
        } else if (first & 0xfe00) == 0xb400 || (first & 0xff80) == 0xb080 {
            ("prologue", None)
        } else {
            ("other", None)
        };
        let name = match main {
            Some(main) => SourceOwner::parse(&format!("main:{main:08x}"))
                .ok()
                .and_then(|o| sources.registered_name(o).map(str::to_string)),
            None => SourceOwner::parse(&format!("{overlay}:{target:08x}"))
                .ok()
                .and_then(|o| sources.registered_name(o).map(str::to_string)),
        };
        found.push(Import {
            site: x.addr,
            symbol: format!("Func_{symbol:08x}"),
            target,
            kind,
            main,
            name,
        });
    }
    Ok(found)
}
