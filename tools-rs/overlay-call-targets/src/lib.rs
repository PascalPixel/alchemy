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
//! See the TypeScript original's header comment for the full history and
//! measured rationale; it is reproduced there, not duplicated here.
//!
//! The TypeScript original is retained: eight other TS tools still import
//! from it (`overlay_published.ts`, `overlay_twins.ts`, `overlay_unindexed.ts`,
//! overlay certification, Full-C progress, overlay gap analysis,
//! `overlay_call_order_check.ts`, and bl-site-symbols). Only `overlay_show.ts`
//! was ported off it so far, onto this crate.

use std::collections::HashSet;
use std::path::PathBuf;

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE as DISASM_OVERLAY_BASE};

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
        .expect("tools-rs has a parent")
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
    let signed = if upper >= 0x0400 { upper - 0x0800 } else { upper };
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
    let path = root().join("assets/code").join(format!("{overlay}_overlay.s"));
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
        return Classified { kind: Kind::Prologue, imported: None };
    }
    if target >= 0 && (target + 1) < image.len() as i64 {
        let opening = read_u16le(image, target);
        if (opening & 0xfe00) == 0xb400 {
            return Classified { kind: Kind::Prologue, imported: None };
        }
        if (target + 3) < image.len() as i64 && (opening & 0xff80) == 0xb080 {
            let second = read_u16le(image, target + 2);
            if (second & 0xfe00) == 0xb400 {
                return Classified { kind: Kind::Prologue, imported: None };
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
            return Classified { kind: Kind::Veneer, imported: Some((word & !1) as i64) };
        }
        // A bare `bx rN` slot is the overlay's own call_via bank.
        if (first & 0xff87) == 0x4700 {
            return Classified { kind: Kind::CallVia, imported: None };
        }
    }
    if reaches_return(image, target) {
        return Classified { kind: Kind::Leaf, imported: None };
    }
    Classified { kind: Kind::Unknown, imported: None }
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
    let text = std::fs::read_to_string(&path)
        .map_err(|_| format!("missing {}; run the overlay inventory first", path.display()))?;
    let value = parse_json(&text)?;
    let functions = value
        .get("functions")
        .and_then(Value::as_array)
        .ok_or("overlays.json: missing functions array")?;
    let mut rows = Vec::with_capacity(functions.len());
    for function in functions {
        rows.push(InventoryRow {
            overlay: function.get("overlay").and_then(Value::as_str).unwrap_or("").to_string(),
            offset: function.get("offset").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            span_bytes: function.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            starts_with_prologue: function.get("starts_with_prologue").map(Value::truthy).unwrap_or(false),
            contained_by_len: function
                .get("contained_by")
                .and_then(Value::as_array)
                .map(<[Value]>::len)
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
            .map(|row| Span { offset: row.offset, span_bytes: row.span_bytes })
            .collect(),
        Some(owner) => rows
            .iter()
            .filter(|row| row.offset == owner)
            .map(|row| Span { offset: row.offset, span_bytes: row.span_bytes })
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
            spans = vec![Span { offset: owner, span_bytes: end - owner }];
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
                    sites.push(CallSite { site: at, target, kind });
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
pub fn resolved_call_names(overlay: &str, owner: i64, owner_end: i64) -> Result<Vec<(i64, String)>, String> {
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
    names.iter().rev().find(|(at, _)| *at == site).map(|(_, name)| name.as_str())
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
        let Some(address) = line_address(line) else { continue };
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
    text.strip_prefix("resource_")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b)))
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
        if !stripped.is_empty() && stripped.len() <= 4 && stripped.bytes().all(|b| b.is_ascii_hexdigit()) {
            bounds.push(i64::from_str_radix(stripped, 16).map_err(|e| e.to_string())?);
            continue;
        }
        return Err(format!(
            "overlay_call_targets: unrecognised argument {argument:?}.\nBounds are two SEPARATE arguments in either spelling (`1c14 1d0c` or `0x1c14 0x1d0c`).\nIf this looks like two bounds in one string, a shell passed them unsplit — zsh does not word-split an unquoted expansion."
        ));
    }
    if bounds.len() > 2 {
        return Err(format!(
            "overlay_call_targets: {} bounds given, at most two are used (owner start and end). Refusing rather than silently ignoring the rest.",
            bounds.len()
        ));
    }
    Ok(bounds)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn displacement_decode() {
        // `f000 fe67` stores 0xcce, so the callee is at 0xcd0 — a real
        // `resource_39f` owner. Measured from the live disassembly.
        assert_eq!(stored_displacement(0xf000, 0xfe67), Some(0xcce));
        assert_eq!(target_offset(0xf000, 0xfe67), Some(0xcd0));
        // `f002 fe75` appears at two sites and must resolve to ONE callee.
        assert_eq!(target_offset(0xf002, 0xfe75), Some(0x2cec));
        assert_eq!(stored_displacement(0x4770, 0x0000), None);
        assert_eq!(stored_displacement(0xf000, 0x4770), None);
        // A negative prefix must sign-extend rather than wrap.
        assert_eq!(stored_displacement(0xf7ff, 0xf800), Some(-0x1000));
    }

    #[test]
    fn annotate_replaces_wrong_callee_only() {
        let listing = " 2001082:\tf002 fedd \tbl\t0x2003e40\n 2001086:\tmovs r0, #1\n";
        let annotated = annotate(listing, &[(0x1082, "Func_0808a010".to_string())]);
        assert!(annotated.contains("bl Func_0808a010"));
        assert!(!annotated.contains("0x2003e40"));
        assert!(annotated.contains("movs r0, #1"));
    }

    #[test]
    fn classify_shapes() {
        let mut image = vec![0u8; 16];
        // Trailing word 0x0808a011 — the Thumb bit must be masked off.
        image[0..8].copy_from_slice(&[0x00, 0x4c, 0x20, 0x47, 0x11, 0xa0, 0x08, 0x08]);
        let veneer = classify(&image, 0, &HashSet::new());
        assert_eq!(veneer.kind, Kind::Veneer);
        assert_eq!(veneer.imported, Some(0x0808a010));
        let mut prologues = HashSet::new();
        prologues.insert(0i64);
        assert_eq!(classify(&image, 0, &prologues).kind, Kind::Prologue);

        // A `push {r4, lr}` opening is a prologue even when the inventory missed it.
        let unlisted = [0x10u8, 0xb5, 0, 0, 0, 0, 0, 0];
        assert_eq!(classify(&unlisted, 0, &HashSet::new()).kind, Kind::Prologue);

        // Stack reservation before push.
        let stack_first = [0x84u8, 0xb0, 0xe0, 0xb5, 0, 0, 0, 0];
        assert_eq!(classify(&stack_first, 0, &HashSet::new()).kind, Kind::Prologue);
    }

    #[test]
    fn bounds_parse_both_spellings() {
        let overlay = "resource_3af".to_string();
        let bare = parse_bounds(&[overlay.clone(), "1c14".into(), "1d0c".into()], None).unwrap();
        assert_eq!(bare, vec![0x1c14, 0x1d0c]);
        let prefixed = parse_bounds(&[overlay.clone(), "0x1c14".into(), "0x1d0c".into()], None).unwrap();
        assert_eq!(prefixed, vec![0x1c14, 0x1d0c]);
        let mixed = parse_bounds(
            &[overlay.clone(), "0X1C14".into(), "1d0c".into(), "--annotate".into(), "--json".into()],
            None,
        )
        .unwrap();
        assert_eq!(mixed, vec![0x1c14, 0x1d0c]);
        assert_eq!(parse_bounds(&[overlay, "--json".into()], None).unwrap(), Vec::<i64>::new());
    }

    #[test]
    fn bounds_reject_bad_arguments() {
        let cases: Vec<Vec<&str>> = vec![
            vec!["resource_3c9", "3660 36d0"],
            vec!["resource_3c9", "14000"],
            vec!["resource_3c9", "1c14", "1d0g"],
            vec!["resource_3c9", "--anotate"],
            vec!["resource_3c9", "1c14", "1d0c", "1e00"],
        ];
        for argv in cases {
            let args: Vec<String> = argv.into_iter().map(String::from).collect();
            assert!(parse_bounds(&args, Some("resource_3c9")).is_err(), "{args:?} should be rejected");
        }
        let named = parse_bounds(
            &["resource_3c9".into(), "0x1c14".into(), "1d0c".into(), "--annotate".into()],
            Some("resource_3c9"),
        )
        .unwrap();
        assert_eq!(named, vec![0x1c14, 0x1d0c]);
    }

    #[test]
    fn partial_annotation_is_detectable() {
        let listing = concat!(
            " 2000388:\tb520      \tpush\t{r5, lr}\n",
            " 20003b4:\tf004 f9dd \tbl\t0x2004772\n",
            " 20003ee:\tf004 f9f5 \tbl\t0x20047dc\n",
        );
        assert_eq!(unannotated_call_sites(listing, &[]).len(), 2);
        let half = unannotated_call_sites(listing, &[(0x3b4, "Func_080770c0".into())]);
        assert_eq!(half, vec![0x3ee]);
        let full = vec![(0x3b4, "Func_080770c0".to_string()), (0x3ee, "Func_0808a038".to_string())];
        assert!(unannotated_call_sites(listing, &full).is_empty());
        assert!(annotate(listing, &full).contains("bl Func_0808a038"));
    }

    #[test]
    fn conditional_branches_are_not_calls() {
        let conditional = " 20012f4:\td90a      \tbls.n\t0x200130c\n";
        assert!(unannotated_call_sites(conditional, &[]).is_empty());
        assert!(unannotated_call_sites("  0x20003fc = 0x02000240\n", &[]).is_empty());
    }

    #[test]
    fn resolves_nothing_both_directions() {
        assert!(resolves_nothing(0, 0));
        assert!(!resolves_nothing(1, 0));
        assert!(!resolves_nothing(0, 2));
    }
}
