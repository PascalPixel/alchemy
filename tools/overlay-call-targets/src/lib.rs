//! `tools/lib/overlay_call_targets.ts`, in Rust.
//!
//! Resolves an overlay's `bl` targets correctly. An overlay `bl` does NOT
//! store a pc-relative displacement the way a linked main-image `bl` does. It
//! stores the target's image offset minus two:
//!
//! ```text
//! true_target_offset = stored_displacement + 2
//! ```
//!
//! The original's header carries the full rationale and is deliberately not
//! duplicated here. It was deleted with the rest of the TypeScript layer;
//! recover it with `git show e3867da35:tools/lib/overlay_call_targets.ts`.
//!
//! The TypeScript original is retained: eight other TS tools still import
//! from it (`overlay_published.ts`, `overlay_twins.ts`, `overlay_unindexed.ts`,
//! overlay certification, Full-C progress, overlay gap analysis,
//! `overlay_call_order_check.ts`, and bl-site-symbols). Only `overlay_show.ts`
//! was ported off it so far, onto this crate.

use std::collections::HashSet;
use std::path::PathBuf;

use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE as DISASM_OVERLAY_BASE};
use serde_json::Value;

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
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools has a parent")
        .to_path_buf()
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

/// True when a whole-overlay run resolved nothing and must therefore FAIL.
///
/// A run with explicit bounds is exempt: the caller stated a span, and an
/// empty result there is a real answer about that span.
pub fn resolves_nothing(site_count: usize, bound_count: usize) -> bool {
    site_count == 0 && bound_count == 0
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

/// Resolve every call site in one explicitly bounded owner to its C spelling.
///
/// Kept here so readers such as `overlay_show` can annotate a listing without
/// reimplementing the overlay's non-PC-relative BL rule. Explicit bounds are
/// required by the caller: an unbounded run intentionally has different
/// inventory semantics and is not safe for a tracked owner.
pub fn resolved_call_names(
    overlay: &str,
    owner: i64,
    owner_end: i64,
) -> Result<Vec<(i64, String)>, String> {
    let image = overlay_image(overlay)?;
    let rows = inventory()?;
    let prologues: HashSet<i64> = rows
        .iter()
        .filter(|row| row.overlay == overlay && row.starts_with_prologue)
        .map(|row| row.offset)
        .collect();
    let mut names = Vec::new();
    for site in resolve_overlay(overlay, Some(owner), Some(owner_end))? {
        let detail = classify(&image, site.target, &prologues);
        let name = match detail.imported {
            Some(imported) => format!("Func_{imported:08x}"),
            None => format!("Func_{:08x}", OVERLAY_BASE + site.target),
        };
        names.push((site.site, name));
    }
    Ok(names)
}

fn names_get(names: &[(i64, String)], site: i64) -> Option<&str> {
    names
        .iter()
        .rev()
        .find(|(at, _)| *at == site)
        .map(|(_, name)| name.as_str())
}

/// A colon-anchored `HHHHHHH:` line-start address, as `overlay_show`/objdump
/// listings print it.
fn line_address(line: &str) -> Option<i64> {
    let trimmed = line.trim_start();
    let leading_ws = line.len() - trimmed.len();
    // Only pure whitespace may precede the address, matching `^\s*`.
    if line[..leading_ws].chars().any(|c| !c.is_whitespace()) {
        return None;
    }
    let colon = trimmed.find(':')?;
    let digits = &trimmed[..colon];
    if digits.is_empty() || !digits.bytes().all(|b| b.is_ascii_hexdigit()) {
        return None;
    }
    i64::from_str_radix(digits, 16).ok()
}

/// Rewrite an `overlay_show` listing so each `bl` names its REAL callee.
pub fn annotate(listing: &str, sites: &[(i64, String)]) -> String {
    let mut out: Vec<String> = Vec::new();
    for line in listing.split('\n') {
        let Some(address) = line_address(line) else {
            out.push(line.to_string());
            continue;
        };
        let site = address - OVERLAY_BASE;
        let Some(name) = names_get(sites, site) else {
            out.push(line.to_string());
            continue;
        };
        out.push(replace_bl_target(line, name));
    }
    out.join("\n")
}

/// `line.replace(/\bbl\s+\S+/, replacement)`: replace the FIRST `bl <target>`
/// run, matching only a whole-word `bl` (so `bls`/`blt`/`bl_x` do not match).
fn replace_bl_target(line: &str, name: &str) -> String {
    let bytes = line.as_bytes();
    let mut i = 0usize;
    while i + 1 < bytes.len() {
        if &line[i..i + 2] == "bl" {
            let word_start_ok = i == 0 || !is_word_byte(bytes[i - 1]);
            let after = bytes.get(i + 2).copied();
            let word_end_ok = after.map(|b| !is_word_byte(b)).unwrap_or(true);
            if word_start_ok && word_end_ok {
                // Require at least one whitespace char after `bl`.
                if let Some(b) = after {
                    if (b as char).is_whitespace() {
                        let mut j = i + 2;
                        while j < bytes.len() && (bytes[j] as char).is_whitespace() {
                            j += 1;
                        }
                        let target_start = j;
                        while j < bytes.len() && !(bytes[j] as char).is_whitespace() {
                            j += 1;
                        }
                        if j > target_start {
                            return format!("{}bl {}{}", &line[..i], name, &line[j..]);
                        }
                    }
                }
            }
        }
        i += 1;
    }
    line.to_string()
}

fn is_word_byte(b: u8) -> bool {
    b.is_ascii_alphanumeric() || b == b'_'
}

/// Every `bl` line in the listing whose site the resolver did NOT cover.
///
/// Matches `bl ` with a trailing space so that `bls`, `blt` and friends,
/// which are conditional branches and not calls, are not counted.
pub fn unannotated_call_sites(listing: &str, sites: &[(i64, String)]) -> Vec<i64> {
    let mut missed = Vec::new();
    for line in listing.split('\n') {
        let Some(address) = line_address(line) else {
            continue;
        };
        if !contains_bl_word(line) {
            continue;
        }
        let site = address - OVERLAY_BASE;
        if names_get(sites, site).is_none() {
            missed.push(site);
        }
    }
    missed
}

/// `/\bbl\s/.test(line)`.
fn contains_bl_word(line: &str) -> bool {
    let bytes = line.as_bytes();
    let mut i = 0usize;
    while i + 2 < bytes.len() {
        if &line[i..i + 2] == "bl" {
            let word_start_ok = i == 0 || !is_word_byte(bytes[i - 1]);
            if word_start_ok && (bytes[i + 2] as char).is_whitespace() {
                return true;
            }
        }
        i += 1;
    }
    false
}

const KNOWN_FLAGS: [&str; 3] = ["--self-test", "--json", "--annotate"];

/// `/^resource_[0-9a-f]+$/` — lowercase only, no `i` flag in the original.
fn is_resource_name(text: &str) -> bool {
    text.strip_prefix("resource_").is_some_and(|rest| {
        !rest.is_empty()
            && rest
                .bytes()
                .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
    })
}

/// Pick the owner/end bounds out of the command line.
///
/// `parseBounds` CONSUMES rather than filters: every argument must be
/// accounted for, and an unrecognised or surplus one is an error with the
/// offending text quoted. See the TypeScript original's comment for the two
/// measured failure modes this guards against.
pub fn parse_bounds(args: &[String], overlay: Option<&str>) -> Result<Vec<i64>, String> {
    let mut bounds = Vec::new();
    for argument in args {
        if KNOWN_FLAGS.contains(&argument.as_str()) {
            continue;
        }
        if Some(argument.as_str()) == overlay {
            continue;
        }
        if overlay.is_none() && is_resource_name(argument) {
            continue;
        }
        let stripped = argument
            .strip_prefix("0x")
            .or_else(|| argument.strip_prefix("0X"))
            .unwrap_or(argument.as_str());
        if !stripped.is_empty()
            && stripped.len() <= 4
            && stripped.bytes().all(|b| b.is_ascii_hexdigit())
        {
            bounds.push(i64::from_str_radix(stripped, 16).map_err(|e| e.to_string())?);
            continue;
        }
        return Err(format!(
            "overlay_call_targets: unrecognised argument {argument:?}.\nBounds are two SEPARATE arguments in either spelling (`1c14 1d0c` or `0x1c14 0x1d0c`).\nIf this looks like two bounds in one string, a shell passed them unsplit — zsh does not word-split an unquoted expansion."
        ));
    }
    if bounds.len() > 2 {
        return Err(format!("overlay_call_targets: {} bounds given, at most two are used (owner start and end). Refusing rather than silently ignoring the rest.", bounds.len()));
    }
    Ok(bounds)
}
