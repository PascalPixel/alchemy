//! Overlay `bl` words encode target image offset minus two, unlike linked
//! main-image PC-relative branches: `target_offset = stored + 2`.
use disassemble::{assemble_overlay, OverlaySource, OVERLAY_BASE as DISASM_OVERLAY_BASE};
use serde_json::Value;
use std::collections::HashSet;
use std::path::PathBuf;
/// Overlays are linked here, so an in-image address is `pool_word - BASE_SHIFT`.
pub const BASE_SHIFT: i64 = 0x8000;
pub const OVERLAY_BASE: i64 = 0x0200_0000;
/// How far a leaf may run before its `bx lr`, in bytes.
///
/// Deliberately a LOCAL copy of the TypeScript original's window: see that
/// file's comment on why sharing the constant via an import would create a
/// load-bearing module cycle.
const RETURN_WINDOW: i64 = 128;
fn root() -> PathBuf {
    disassemble::paths::root()
}
/// Decode a Thumb BL pair into the displacement the instruction stores.
/// Returns `None` when the halfwords are not a BL prefix/suffix pair.
pub fn stored_displacement(high: u16, low: u16) -> Option<i64> {
    if (high & 0xf800) != 0xf000 {
        return None;
    }
    if (low & 0xf800) != 0xf800 {
        return None;
    }
    let upper = (high & 0x07ff) as i64;
    let lower = (low & 0x07ff) as i64;
    // The prefix carries a signed 11-bit field; sign-extend before combining.
    let signed = if upper >= 0x0400 {
        upper - 0x0800
    } else {
        upper
    };
    Some((signed << 12) | (lower << 1))
}
/// The rule: the stored displacement is the target offset minus two.
pub fn target_offset(high: u16, low: u16) -> Option<i64> {
    stored_displacement(high, low).map(|displacement| displacement + 2)
}
/// Overlay image bytes, as the reconstruction assembles them. This is the
/// same path `overlay_show` reads, so the offsets here line up with its
/// listing.
pub fn overlay_image(overlay: &str) -> Result<Vec<u8>, String> {
    let path = root()
        .join("games/gs1/assets/code")
        .join(format!("{overlay}_overlay.s"));
    if !path.exists() {
        return Err(format!("no reconstruction assembly for {overlay}"));
    }
    assemble_overlay(&OverlaySource::path(&path), DISASM_OVERLAY_BASE)
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
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct CallSite {
    /// Offset of the BL prefix halfword within the overlay image.
    pub site: i64,
    /// Resolved target offset within the overlay image.
    pub target: i64,
    /// What the target lands on, once classified.
    pub kind: Kind,
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
struct InventoryRow {
    overlay: String,
    offset: i64,
    span_bytes: i64,
    starts_with_prologue: bool,
    contained_by_len: usize,
}
fn inventory() -> Result<Vec<InventoryRow>, String> {
    let path = root().join("out/decomp/overlays.json");
    let text = std::fs::read_to_string(&path).map_err(|_| {
        format!(
            "missing {}; run the overlay inventory first",
            path.display()
        )
    })?;
    let value: Value = serde_json::from_str(&text).map_err(|error| error.to_string())?;
    let functions = value
        .get("functions")
        .and_then(Value::as_array)
        .ok_or("overlays.json: missing functions array")?;
    let mut rows = Vec::with_capacity(functions.len());
    for function in functions {
        rows.push(InventoryRow {
            overlay: function
                .get("overlay")
                .and_then(Value::as_str)
                .unwrap_or("")
                .to_string(),
            offset: function.get("offset").and_then(Value::as_i64).unwrap_or(0),
            span_bytes: function
                .get("span_bytes")
                .and_then(Value::as_i64)
                .unwrap_or(0),
            starts_with_prologue: function
                .get("starts_with_prologue")
                .and_then(Value::as_bool)
                .unwrap_or(false),
            contained_by_len: function
                .get("contained_by")
                .and_then(Value::as_array)
                .map(Vec::len)
                .unwrap_or(0),
        });
    }
    Ok(rows)
}
/// Resolve every `bl` in an overlay (or one bounded owner) to its target.
///
/// `owner_end` only has an effect when `owner` is `Some`; an explicit end
/// always wins over the inventory span, matching the TypeScript rule that the
/// caller stating a boundary outranks a possibly-wrong or absent one.
pub fn resolve_overlay(
    overlay: &str,
    owner: Option<i64>,
    owner_end: Option<i64>,
) -> Result<Vec<CallSite>, String> {
    let image = overlay_image(overlay)?;
    let rows = inventory()?;
    let rows: Vec<&InventoryRow> = rows.iter().filter(|row| row.overlay == overlay).collect();
    let prologues: HashSet<i64> = rows
        .iter()
        .filter(|row| row.starts_with_prologue)
        .map(|row| row.offset)
        .collect();
    struct Span {
        offset: i64,
        span_bytes: i64,
    }
    let mut spans: Vec<Span> = match owner {
        None => rows
            .iter()
            .filter(|row| row.contained_by_len == 0)
            .map(|row| Span {
                offset: row.offset,
                span_bytes: row.span_bytes,
            })
            .collect(),
        Some(owner) => rows
            .iter()
            .filter(|row| row.offset == owner)
            .map(|row| Span {
                offset: row.offset,
                span_bytes: row.span_bytes,
            })
            .collect(),
    };
    if let Some(owner) = owner {
        if spans.is_empty() {
            if owner >= image.len() as i64 {
                return Err(format!("owner 0x{owner:x} is past the image end"));
            }
            let mut offsets: Vec<i64> = rows.iter().map(|row| row.offset).collect();
            offsets.sort_unstable();
            offsets.dedup();
            let next = offsets.into_iter().find(|offset| *offset > owner);
            let end = owner_end.or(next).unwrap_or(image.len() as i64);
            if owner_end.is_none() {
                eprintln!(
                    "note: 0x{owner:x} is not an unconverted inventory row (already tracked?).\n      Walking 0x{owner:x}..0x{end:x}, bounded by the next unconverted row — this MAY INCLUDE neighbouring tracked functions.\n      Pass an explicit end offset as a third argument to bound it exactly."
                );
            }
            spans = vec![Span {
                offset: owner,
                span_bytes: end - owner,
            }];
        }
    }
    let mut sites = Vec::new();
    for span in &spans {
        let claimed = match (owner, owner_end) {
            (Some(_), Some(owner_end)) => owner_end,
            _ => span.offset + span.span_bytes,
        };
        let end = std::cmp::min(claimed, image.len() as i64 - 3);
        let mut at = span.offset;
        while at < end {
            let high = read_u16le(&image, at);
            let low = read_u16le(&image, at + 2);
            if let Some(target) = target_offset(high, low) {
                if target >= 0 && target < image.len() as i64 {
                    let kind = classify(&image, target, &prologues).kind;
                    sites.push(CallSite {
                        site: at,
                        target,
                        kind,
                    });
                }
            }
            at += 2;
        }
    }
    Ok(sites)
}
