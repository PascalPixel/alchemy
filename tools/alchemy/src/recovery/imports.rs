//! Resolve Golden Sun overlay calls. An overlay `bl` stores the target
//! image offset minus two, not a PC-relative displacement, so the pseudo
//! symbol a unit spells for a site (`Func_02004280`, the PC-relative decode)
//! is particular to that site. The real target is `symbol - site - 2` into
//! the image: a veneer (`ldr r4, [pc]; bx r4; .word main`) names an import
//! from the main image, and a prologue names a function of the overlay
//! itself. Names come from the labels of the images `alchemy build rom`
//! links, when they are built.

use super::owners::{image_window_for, linked_names, main_elf, overlay_elf, Owner};
use crate::targets::DecompTarget;
use psynergy::decode::{decode_window_at, Kind, MAIN_BASE, OVERLAY_BASE};
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
    /// The label the linked image defines at the target, if it is built.
    pub name: Option<String>,
}

fn read_u16(image: &[u8], at: usize) -> Option<u16> {
    Some(u16::from_le_bytes([*image.get(at)?, *image.get(at + 1)?]))
}

/// Resolves every call site of an owner's window against one target's ROM
/// and linked images.
pub fn imports_for(
    root: &Path,
    target: DecompTarget,
    owner: &str,
    span: Option<u32>,
) -> Result<Vec<Import>, String> {
    let resolved = Owner::parse_argument(owner)?;
    let (image, base, entry, span) = image_window_for(root, target, owner, span)?;
    let main_names = linked_names(root, &main_elf(root, target))?;
    let overlay_names = match resolved.overlay_id() {
        Some(overlay) => linked_names(root, &overlay_elf(root, target, &overlay))?,
        None => Default::default(),
    };
    let ins = decode_window_at(&image, base, entry, span);
    let mut found = Vec::new();
    for x in &ins {
        let Kind::Bl { target: symbol } = x.kind else {
            continue;
        };
        if base == MAIN_BASE {
            found.push(Import {
                site: x.addr,
                symbol: format!("Func_{symbol:08x}"),
                target: symbol,
                kind: "direct",
                main: Some(symbol),
                name: main_names.get(&symbol).cloned(),
            });
            continue;
        }
        let offset = symbol as i64 - x.addr as i64 - 2;
        if offset < 0 || offset as usize + 8 > image.len() {
            continue;
        }
        let at = offset as usize;
        let target = OVERLAY_BASE + offset as u32;
        let first = read_u16(&image, at).unwrap_or(0);
        let (kind, main) = if let Some(word) = psynergy::thumb::veneer_target(&image[at..]) {
            ("veneer", Some(word & !1))
        } else if (first & 0xfe00) == 0xb400 || (first & 0xff80) == 0xb080 {
            ("prologue", None)
        } else {
            ("other", None)
        };
        let name = match main {
            Some(main) => main_names.get(&main).cloned(),
            None => overlay_names
                .get(&(crate::compiler::overlay::RUNTIME_BASE + offset as u32))
                .cloned(),
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
