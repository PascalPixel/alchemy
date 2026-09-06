//! Overlay `bl` words encode target image offset minus two, unlike linked
//! main-image PC-relative branches: `target_offset = stored + 2`.
use std::collections::HashSet;
use std::path::PathBuf;
/// Overlays are linked here, so an in-image address is `pool_word - BASE_SHIFT`.
pub const BASE_SHIFT: i64 = 0x8000;
pub const OVERLAY_BASE: i64 = 0x0200_0000;
/// How far a leaf may run before its `bx lr`, in bytes.
const RETURN_WINDOW: i64 = 128;
fn root() -> PathBuf {
    disassemble::paths::root()
}
/// Decode a Thumb BL pair into the displacement the instruction stores.
/// Returns `None` when the halfwords are not a BL prefix/suffix pair.
pub fn stored_displacement(high: u16, low: u16) -> Option<i64> {
    compiler_core::overlay::displacement(&[high.to_le_bytes(), low.to_le_bytes()].concat())
        .map(i64::from)
}
/// The rule: the stored displacement is the target offset minus two.
pub fn target_offset(high: u16, low: u16) -> Option<i64> {
    stored_displacement(high, low).map(|displacement| displacement + 2)
}
/// Resolve calls from the canonical resource even when production C is broken.
pub fn overlay_image(overlay: &str) -> Result<Vec<u8>, String> {
    disassemble::canonical_overlay(&root(), overlay)
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Kind {
    Veneer,
    Prologue,
    CallVia,
    Leaf,
    Unknown,
}
impl Kind {
    pub fn as_str(self) -> &'static str {
        match self {
            Kind::Veneer => "veneer",
            Kind::Prologue => "prologue",
            Kind::CallVia => "call_via",
            Kind::Leaf => "leaf",
            Kind::Unknown => "unknown",
        }
    }
}
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Classified {
    pub kind: Kind,
    pub imported: Option<i64>,
}
fn read_u16le(image: &[u8], at: i64) -> u16 {
    if at < 0 || (at + 1) as usize >= image.len() {
        return 0;
    }
    let at = at as usize;
    (image[at] as u16) | ((image[at + 1] as u16) << 8)
}
/// Does a `bx lr` appear within the window? A function must return.
fn reaches_return(image: &[u8], offset: i64) -> bool {
    let end = std::cmp::min(image.len() as i64 - 1, offset + RETURN_WINDOW);
    let mut at = offset;
    while at < end {
        if read_u16le(image, at) == 0x4770 {
            return true;
        }
        at += 2;
    }
    false
}
/// Classify a resolved target. A veneer entry is the 8-byte
/// `ldr r4,[pc,#0] / bx r4 / .word T` shape, so the import's real identity is
/// the main-image address in its trailing word.
pub fn classify(image: &[u8], target: i64, prologues: &HashSet<i64>) -> Classified {
    if prologues.contains(&target) {
        return Classified {
            kind: Kind::Prologue,
            imported: None,
        };
    }
    if target >= 0 && (target + 1) < image.len() as i64 {
        let opening = read_u16le(image, target);
        if (opening & 0xfe00) == 0xb400 {
            return Classified {
                kind: Kind::Prologue,
                imported: None,
            };
        }
        if (target + 3) < image.len() as i64 && (opening & 0xff80) == 0xb080 {
            let second = read_u16le(image, target + 2);
            if (second & 0xfe00) == 0xb400 {
                return Classified {
                    kind: Kind::Prologue,
                    imported: None,
                };
            }
        }
    }
    if target >= 0 && (target + 8) <= image.len() as i64 {
        let first = read_u16le(image, target);
        let second = read_u16le(image, target + 2);
        // ldr r4,[pc,#0] == 0x4c00, bx r4 == 0x4720
        if first == 0x4c00 && second == 0x4720 {
            let at = target as usize;
            let word = (image[at + 4] as u32)
                | ((image[at + 5] as u32) << 8)
                | ((image[at + 6] as u32) << 16)
                | ((image[at + 7] as u32) << 24);
            // The stored word carries the Thumb bit; the import's address is even.
            return Classified {
                kind: Kind::Veneer,
                imported: Some((word & !1) as i64),
            };
        }
        // A bare `bx rN` slot is the overlay's own call_via bank.
        if (first & 0xff87) == 0x4700 {
            return Classified {
                kind: Kind::CallVia,
                imported: None,
            };
        }
    }
    if reaches_return(image, target) {
        return Classified {
            kind: Kind::Leaf,
            imported: None,
        };
    }
    Classified {
        kind: Kind::Unknown,
        imported: None,
    }
}
