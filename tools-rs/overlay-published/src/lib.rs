//! `tools/lib/overlay_published.ts`, in Rust.
//!
//! Find overlay functions that are installed by DATA rather than called by
//! code. See the TypeScript original's header comment for the full history
//! and measured rationale (the 2026-08-01 `resource_3af` regression that
//! motivated the `calledLeaf`/`publishedLeaf` tri-class); it is reproduced
//! there in full, not duplicated here. This port preserves every invariant
//! from that comment and from `selfTest()` bit for bit.
//!
//! The remaining overlay gap analysis still consumes the corresponding legacy
//! helpers until that command is migrated.

use std::collections::HashSet;
use std::fs;
use std::path::PathBuf;

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_call_targets::{
    classify, overlay_image, target_offset, Kind, BASE_SHIFT, OVERLAY_BASE,
};
use overlay_disasm::{overlay_c_spans, OverlaySource};

fn root() -> PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

/// A `push {regs}` (0xb4xx) or `push {regs, lr}` (0xb5xx) opening.
pub fn is_prologue_shape(halfword: u16) -> bool {
    (halfword & 0xfe00) == 0xb400
}

/// A prologue that can be returned through has to save `lr`.
pub fn saves_link_register(halfword: u16) -> bool {
    (halfword & 0xff00) == 0xb500
}

/// Resolve a pool/table word to the image offset it publishes, or `None`.
///
/// The Thumb bit must be set -- a published *function* pointer is odd -- and
/// the overlay's link base subtracted, exactly as `overlay_call_targets` does
/// for jump-table entries.
pub fn published_offset(word: u32, image_length: i64) -> Option<i64> {
    if (word & 1) == 0 {
        return None;
    }
    let offset = (word & !1u32) as i64 - (OVERLAY_BASE + BASE_SHIFT);
    if offset < 0 || offset + 2 > image_length {
        return None;
    }
    Some(offset)
}

#[derive(Debug, Clone)]
pub struct ShapedRow {
    pub offset: i64,
    pub halfword: u16,
    pub owner: Option<i64>,
    pub delta: Option<i64>,
    pub verdict: String,
}

#[derive(Debug, Clone)]
pub struct OverlayResidue {
    pub overlay: String,
    /// bl-reached prologues with no owner.
    pub called: Vec<i64>,
    /// bl-reached targets with no owner that are NOT prologue-shaped: LEAVES.
    ///
    /// THE SAME DEFECT AS `published_leaf`, IN SWEEP A, FIXED IN SWEEP B (found
    /// 2026-08-01). Kept separate from `called` so the prologue-confirmed
    /// count stays a regression invariant. BOTH must be empty to close an
    /// overlay.
    pub called_leaf: Vec<i64>,
    /// bl-reached targets with no owner that do not reach a return: NOT CODE.
    /// Reported rather than dropped; they do not count towards residue.
    pub called_data: Vec<i64>,
    /// published prologues with no owner.
    pub published: Vec<i64>,
    /// Published pointers with no owner whose target is NOT prologue-shaped:
    /// LEAF functions. Kept separate from `published` so the
    /// prologue-confirmed count stays a stable regression invariant. BOTH
    /// must be empty to close an overlay.
    pub published_leaf: Vec<i64>,
    /// Published pointers with no owner that are NOT code at all. Reported
    /// rather than dropped; they do not count towards residue.
    pub published_data: Vec<i64>,
    /// prologue-shaped halfwords with no owner, each classified.
    pub shaped: Vec<ShapedRow>,
}

/// How far a real leaf is allowed to run before it returns.
///
/// 301 of the 306 rows the relaxed gate admitted reach a `bx lr` inside 128
/// bytes; the five that never do are data. The window is deliberately
/// generous -- the longest body in the cohort is the 54-byte record
/// integrator -- so that widening it further would not change the verdict on
/// anything measured.
pub const RETURN_WINDOW: i64 = 128;

/// `bx lr`, the only return a leaf has.
const BX_LR: u16 = 0x4770;

/// True when `offset` reaches a `bx lr` within `window` bytes.
///
/// This is the whole discriminator between a leaf and a table of odd words.
/// It scans halfwords, so it does not care where instruction boundaries fall
/// -- data that happens to contain 0x4770 will pass, and that is the safe
/// direction to be wrong in: a false leaf is read by a human and ruled, a
/// false table is silently lost.
pub fn reaches_return(image: &[u8], offset: i64, window: i64) -> bool {
    let end = std::cmp::min(image.len() as i64 - 1, offset + window);
    let mut at = offset;
    while at < end {
        let a = at as usize;
        if a + 1 >= image.len() {
            break;
        }
        let halfword = (image[a] as u16) | ((image[a + 1] as u16) << 8);
        if halfword == BX_LR {
            return true;
        }
        at += 2;
    }
    false
}

fn reaches_return_default(image: &[u8], offset: i64) -> bool {
    reaches_return(image, offset, RETURN_WINDOW)
}

/// Parse `^{overlay}_c_0*([0-9a-f]+)\.c$` against `name`, returning the
/// captured hex digits (with the `0*` prefix already stripped by the regex
/// engine's greedy match, matching group 1 exactly).
fn parse_owner_filename(overlay: &str, name: &str) -> Option<i64> {
    let prefix = format!("{overlay}_c_");
    let rest = name.strip_prefix(prefix.as_str())?;
    let rest = rest.strip_suffix(".c")?;
    let digits = rest.trim_start_matches('0');
    // If the whole thing was zeros, keep one digit -- parse_owner_filename is
    // only ever asked about filenames that matched `0*([0-9a-f]+)`, i.e. at
    // least one hex digit is captured even when it is `0`.
    let digits = if digits.is_empty() { "0" } else { digits };
    if digits.is_empty() || !digits.bytes().all(|b| b.is_ascii_hexdigit() && !(b as char).is_ascii_uppercase()) {
        return None;
    }
    if rest.is_empty() || !rest.bytes().all(|b| b.is_ascii_hexdigit() && !(b as char).is_ascii_uppercase()) {
        return None;
    }
    i64::from_str_radix(digits, 16).ok()
}

/// Owners are whatever the tree already claims: a C file, or a manual region.
pub fn owner_set(overlay: &str) -> HashSet<i64> {
    let mut owners = HashSet::new();
    for directory in ["exact", "semantic"] {
        let path = root().join(directory);
        let Ok(entries) = fs::read_dir(&path) else { continue };
        for entry in entries.flatten() {
            let name = entry.file_name();
            let Some(name) = name.to_str() else { continue };
            if let Some(offset) = parse_owner_filename(overlay, name) {
                owners.insert(offset - OVERLAY_BASE);
            }
        }
    }
    for region in manual_regions(overlay) {
        owners.insert(region.start);
    }
    owners
}

#[derive(Debug, Clone, Copy)]
pub struct Region {
    pub start: i64,
    pub end: i64,
}

/// Recorded semantic bodies, as `[start, end)` offsets.
///
/// These are the only spans the tree states outright. Exact-C rows under
/// `exact/` carry no span here -- their extent is whatever the compiler
/// produces -- so a candidate sitting behind an exact row cannot be ruled
/// inside a body by this data alone, and is reported as needing the owner's
/// check rather than silently dropped.
pub fn manual_regions(overlay: &str) -> Vec<Region> {
    let path = root().join("semantic").join("regions.json");
    let Ok(text) = fs::read_to_string(&path) else { return Vec::new() };
    let Ok(document) = parse_json(&text) else { return Vec::new() };
    let mut spans = Vec::new();
    if let Some(regions) = document.get("manual_regions").and_then(Value::as_array) {
        for region in regions {
            let region_overlay = region.get("overlay").and_then(Value::as_str);
            if region_overlay != Some(overlay) {
                continue;
            }
            let Some(entry) = region.get("entry").and_then(Value::as_str) else { continue };
            let entry = entry.strip_prefix("0x").or_else(|| entry.strip_prefix("0X")).unwrap_or(entry);
            let Ok(entry_value) = i64::from_str_radix(entry, 16) else { continue };
            let start = entry_value - OVERLAY_BASE;
            let span_bytes = region.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64;
            spans.push(Region { start, end: start + span_bytes });
        }
    }
    spans.sort_by_key(|span| span.start);
    spans
}

/// Real, compiled spans for this overlay's exact-C rows.
///
/// These are not recorded in the tree -- an exact row's extent is whatever
/// the compiler produces -- so this tool used to have no way to tell an
/// address inside such a row from an address after it, and reported UNRULED.
/// That ambiguity is what hid the data-installed callbacks in the first
/// place, so closing it is worth the compile. `overlay_c_spans` omits any row
/// that fails to build rather than guessing, and the omission simply leaves
/// the old UNRULED behaviour in place for that row, which is the safe
/// direction.
pub fn exact_spans(overlay: &str) -> Vec<Region> {
    let source_path = root().join("assets").join("code").join(format!("{overlay}_overlay.s"));
    if !source_path.exists() {
        return Vec::new();
    }
    let source = OverlaySource::path(&source_path);
    overlay_c_spans(&source, OVERLAY_BASE)
        .into_iter()
        .map(|span| Region { start: span.start, end: span.end })
        .collect()
}

/// Exact-C row start offsets, used only to name the rows a span is missing
/// for.
pub fn exact_starts(overlay: &str) -> HashSet<i64> {
    let mut starts = HashSet::new();
    let path = root().join("exact");
    let Ok(entries) = fs::read_dir(&path) else { return starts };
    for entry in entries.flatten() {
        let name = entry.file_name();
        let Some(name) = name.to_str() else { continue };
        if let Some(offset) = parse_owner_filename(overlay, name) {
            starts.insert(offset - OVERLAY_BASE);
        }
    }
    starts
}

fn read_u16le(image: &[u8], at: i64) -> Option<u16> {
    if at < 0 {
        return None;
    }
    let a = at as usize;
    if a + 1 >= image.len() {
        return None;
    }
    Some((image[a] as u16) | ((image[a + 1] as u16) << 8))
}

pub fn sweep(overlay: &str) -> Result<OverlayResidue, String> {
    let image = overlay_image(overlay)?;
    let owners = owner_set(overlay);
    let mut sorted: Vec<i64> = owners.iter().copied().collect();
    sorted.sort_unstable();
    let bodies = manual_regions(overlay);
    let exact = exact_starts(overlay);
    let compiled = exact_spans(overlay);
    let measured_exact_start: HashSet<i64> = compiled.iter().map(|span| span.start).collect();

    // Inside a body the tree measured, or inside an exact-C row this tool has
    // now compiled. Both are real spans; the second used to be invisible.
    let inside_recorded_body = |offset: i64| -> bool {
        bodies.iter().any(|body| offset > body.start && offset < body.end)
            || compiled.iter().any(|span| offset > span.start && offset < span.end)
    };
    // Behind an exact-C row whose span is STILL unknown -- that is, one
    // `overlay_c_spans` could not compile. A row it did compile is now
    // decided by `inside_recorded_body` above, either way, so it is no longer
    // UNRULED.
    let behind_exact_row = |offset: i64| -> bool {
        let mut nearest: Option<i64> = None;
        for &start in &sorted {
            if start <= offset {
                nearest = Some(start);
            } else {
                break;
            }
        }
        match nearest {
            Some(start) => exact.contains(&start) && !measured_exact_start.contains(&start),
            None => false,
        }
    };

    let mut called: Vec<i64> = Vec::new();
    let mut called_leaf: Vec<i64> = Vec::new();
    let mut called_data: Vec<i64> = Vec::new();
    let mut seen_call: HashSet<i64> = HashSet::new();
    let mut at = 0i64;
    while at + 4 <= image.len() as i64 {
        let a = at as usize;
        let high = (image[a] as u16) | ((image[a + 1] as u16) << 8);
        let low = (image[a + 2] as u16) | ((image[a + 3] as u16) << 8);
        if let Some(target) = target_offset(high, low) {
            if target >= 0 && target < image.len() as i64 {
                let kind = classify(&image, target, &HashSet::new()).kind;
                let is_veneer_or_call_via = kind == Kind::Veneer || kind == Kind::CallVia;
                if !is_veneer_or_call_via && !owners.contains(&target) && !seen_call.contains(&target)
                    && !inside_recorded_body(target)
                {
                    seen_call.insert(target);
                    if kind == Kind::Prologue {
                        called.push(target);
                    } else if reaches_return_default(&image, target) {
                        called_leaf.push(target);
                    } else {
                        called_data.push(target);
                    }
                }
            }
        }
        at += 2;
    }

    let mut published: Vec<i64> = Vec::new();
    let mut published_leaf: Vec<i64> = Vec::new();
    let mut published_data: Vec<i64> = Vec::new();
    let mut seen_publish: HashSet<i64> = HashSet::new();
    let mut at = 0i64;
    while at + 4 <= image.len() as i64 {
        let a = at as usize;
        let word = (image[a] as u32)
            | ((image[a + 1] as u32) << 8)
            | ((image[a + 2] as u32) << 16)
            | ((image[a + 3] as u32) << 24);
        if let Some(offset) = published_offset(word, image.len() as i64) {
            if !owners.contains(&offset) && !seen_publish.contains(&offset) && !inside_recorded_body(offset) {
                seen_publish.insert(offset);
                let opening = read_u16le(&image, offset).unwrap_or(0);
                if is_prologue_shape(opening) {
                    published.push(offset);
                } else if reaches_return_default(&image, offset) {
                    published_leaf.push(offset);
                } else {
                    published_data.push(offset);
                }
            }
        }
        at += 4;
    }

    let mut shaped: Vec<ShapedRow> = Vec::new();
    let mut at = 0i64;
    while at + 2 <= image.len() as i64 {
        let halfword = read_u16le(&image, at).unwrap();
        if is_prologue_shape(halfword) && !owners.contains(&at) && !inside_recorded_body(at) {
            let mut owner: Option<i64> = None;
            for &candidate in &sorted {
                if candidate <= at {
                    owner = Some(candidate);
                } else {
                    break;
                }
            }
            let delta = owner.map(|owner| at - owner);
            // A spill push sits within the prologue of the owner it belongs
            // to. Twelve bytes covers the widest spill seen so far (fp/sl
            // then r9/r8, two pairs).
            let verdict = if delta.map(|delta| delta <= 12).unwrap_or(false) {
                "spill inside an owner's prologue".to_string()
            } else if !saves_link_register(halfword) {
                "pool word wearing a push (no lr, cannot be returned through)".to_string()
            } else if behind_exact_row(at) {
                "UNRULED — saves lr, but sits behind an exact-C row whose span this tool cannot read".to_string()
            } else {
                "UNEXPLAINED — saves lr, outside every recorded body".to_string()
            };
            shaped.push(ShapedRow { offset: at, halfword, owner, delta, verdict });
        }
        at += 2;
    }

    Ok(OverlayResidue {
        overlay: overlay.to_string(),
        called,
        called_leaf,
        called_data,
        published,
        published_leaf,
        published_data,
        shaped,
    })
}

pub fn overlay_names() -> Result<Vec<String>, String> {
    let path = root().join("assets").join("code");
    let entries = fs::read_dir(&path).map_err(|e| e.to_string())?;
    let mut names: Vec<String> = Vec::new();
    for entry in entries.flatten() {
        let name = entry.file_name();
        let Some(name) = name.to_str() else { continue };
        if let Some(stripped) = name.strip_suffix("_overlay.s") {
            names.push(stripped.to_string());
        }
    }
    names.sort();
    Ok(names)
}

pub fn self_test() -> Result<(), String> {
    if !is_prologue_shape(0xb500) {
        return Err("push {lr} is a prologue shape".into());
    }
    if !is_prologue_shape(0xb5e0) {
        return Err("push {r5,r6,r7,lr} is a prologue shape".into());
    }
    if !is_prologue_shape(0xb460) {
        return Err("a bare push is a prologue shape".into());
    }
    if is_prologue_shape(0xb082) {
        return Err("sub sp,#n must not read as a push".into());
    }
    if is_prologue_shape(0xbc60) {
        return Err("pop must not read as a push".into());
    }
    // The lr discriminator, taken from the real trap: resource_3af's pool word
    // 0x0200b4bd decodes as `push {r0, r2, r3, r4, r5, r7}` at 0x02001654 and
    // was mistaken for the next function's prologue, under-measuring
    // 0x02001444 by 48 bytes. No lr, so nothing can return through it.
    if saves_link_register(0xb4bd) {
        return Err("a push without lr must not claim to save it".into());
    }
    if !saves_link_register(0xb560) {
        return Err("push {r5, r6, lr} saves lr".into());
    }
    // Published-pointer resolution: an overlay function at image 0x3c88 is
    // published as 0x0200bc89 -- offset + OVERLAY_BASE + 0x8000, Thumb bit set.
    if published_offset(0x0200bc89, 0x5b2c) != Some(0x3c88) {
        return Err("published resolution is wrong".into());
    }
    // An even word is data, not a function pointer.
    if published_offset(0x0200bc88, 0x5b2c).is_some() {
        return Err("an even word is not a function pointer".into());
    }
    // Out of range must be rejected rather than clamped.
    if published_offset(0x0200bc89, 0x100).is_some() {
        return Err("out-of-range must be rejected".into());
    }
    if published_offset(0x00000001, 0x5b2c).is_some() {
        return Err("a word below the link base must be rejected".into());
    }
    // Exact-C spans, the limitation this tool shipped with. resource_380's
    // exact row at 0x02000390 compiles to 868 bytes and therefore ends
    // exactly at 0x020006f4, where a published callback begins. Before the
    // spans were readable the tool could not tell those two apart and
    // reported UNRULED, so this asserts BOTH halves of the fix: the span is
    // produced at all, and it decides 0x020006f4 to be outside rather than
    // inside.
    let spans = exact_spans("resource_380");
    let row = spans.iter().find(|span| span.start == 0x390);
    let row = match row {
        Some(row) => row,
        None => return Err("resource_380's exact row at 0x02000390 must compile to a span".into()),
    };
    if row.end != 0x6f4 {
        return Err(format!("exact row 0x02000390 must end at 0x020006f4, got 0x{:x}", row.end));
    }
    if spans.iter().any(|span| 0x6f4 > span.start && 0x6f4 < span.end) {
        return Err("0x020006f4 is a row start, not an interior address".into());
    }
    // A row that fails to compile must be omitted, never guessed at, so a
    // start with no span has to stay absent rather than appear with a zero
    // length.
    if spans.iter().any(|span| span.end <= span.start) {
        return Err("an empty span is a guess, not a measurement".into());
    }
    // The verdict string is UPPERCASE. An earlier draft of this check compared
    // against "unruled" and therefore matched nothing and passed vacuously --
    // which is the whole failure mode this tool exists to catch, reproduced
    // inside its own self-test.
    let residue = sweep("resource_380")?;
    let unruled: Vec<&ShapedRow> = residue.shaped.iter().filter(|row| row.verdict.starts_with("UNRULED")).collect();
    if !unruled.is_empty() {
        return Err(format!("resource_380 must have no UNRULED rows left, got {}", unruled.len()));
    }
    // The bounded-window discriminator, asserted on SYNTHETIC bytes rather
    // than on today's residue -- a check pinned to the rows currently in the
    // tree rots the moment they are drafted.
    let mut getter = vec![0u8; 64];
    getter[0] = 0x00;
    getter[1] = 0x48; // ldr r0, [pc, #0]
    getter[2] = 0x70;
    getter[3] = 0x47; // bx lr
    if !reaches_return_default(&getter, 0) {
        return Err("a getter returns at +2 and must read as a leaf".into());
    }
    let table = vec![0u8; 64]; // no 0x4770 anywhere
    if reaches_return_default(&table, 0) {
        return Err("a table with no return must read as data".into());
    }
    // The window has to BE a window. A return past it does not rescue the
    // row, which is the difference between this check and 'is there a bx lr
    // later'.
    let mut far = vec![0u8; 64];
    far[40] = 0x70;
    far[41] = 0x47;
    if reaches_return(&far, 0, 16) {
        return Err("a return past the window must not count".into());
    }
    if !reaches_return(&far, 0, 64) {
        return Err("a return inside the window must count".into());
    }
    // And it must not run off the end of the image looking for one.
    let mut short = vec![0u8; 4];
    short[2] = 0x70;
    short[3] = 0x47;
    if !reaches_return(&short, 0, 1024) {
        return Err("a return at the last halfword must count".into());
    }
    if reaches_return(&vec![0u8; 4], 0, 1024) {
        return Err("running off the end must not invent a return".into());
    }

    // SWEEP A'S TRI-CLASS, asserted the way sweep B's is: the prologue-confirmed
    // count is the regression invariant, and the leaf class must be ADDED
    // rather than the old one disturbed. Measured on the tree at the moment of
    // the edit: `A called` 224 and `B published` 464 both UNCHANGED, residue
    // 1597 -> 1607, and the difference is exactly the ten `A leaf` rows.
    // Asserted here on synthetic bytes so it does not rot as those ten get
    // drafted.
    //
    // The three shapes a bl target can wear, at 0x20 in a synthetic image:
    //   a `push {lr}` prologue  -> called
    //   `ldr r0,=X / bx lr`     -> calledLeaf  (a leaf: no push, returns)
    //   a table with no return  -> calledData  (not code)
    let kind_of = |opening: &[u16]| -> &'static str {
        let mut image = vec![0u8; 0x60];
        for (index, halfword) in opening.iter().enumerate() {
            image[0x20 + index * 2] = (*halfword & 0xff) as u8;
            image[0x20 + index * 2 + 1] = (*halfword >> 8) as u8;
        }
        if classify(&image, 0x20, &HashSet::new()).kind == Kind::Prologue {
            "called"
        } else if reaches_return_default(&image, 0x20) {
            "calledLeaf"
        } else {
            "calledData"
        }
    };
    if kind_of(&[0xb500, 0x4770]) != "called" {
        return Err("sweep A: a push prologue must stay in `called` -- that count is the invariant".into());
    }
    if kind_of(&[0x4800, 0x4770]) != "calledLeaf" {
        return Err("sweep A: `ldr r0,=X / bx lr` is a LEAF and must not be dropped".into());
    }
    if kind_of(&[0x0000, 0x0000]) != "calledData" {
        return Err("sweep A: bytes that never return are data, and must be reported as such".into());
    }
    // And the direction that matters most: the leaf must not be silently
    // absent. Before 2026-08-01 this shape produced NOTHING at all -- not a
    // leaf, not data, not a complaint. A tri-class that can still return
    // nothing is the same defect wearing three names.
    for shape in [[0xb500u16, 0x4770], [0x4800, 0x4770], [0x0000, 0x0000]] {
        let result = kind_of(&shape);
        if !["called", "calledLeaf", "calledData"].contains(&result) {
            return Err("sweep A: every resolved bl target must land in exactly one class".into());
        }
    }

    println!("self-test=ok");
    Ok(())
}

fn hex(offset: i64) -> String {
    format!("0x{:x}", OVERLAY_BASE + offset)
}

fn json_escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for c in text.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            _ => out.push(c),
        }
    }
    out.push('"');
    out
}

/// Matches `JSON.stringify(value, null, 2)`'s indentation exactly. `level` is
/// the indent depth of the value ITSELF (0 for the top-level array).
fn indent_of(level: usize) -> String {
    "  ".repeat(level)
}

fn shaped_row_json(row: &ShapedRow, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    let owner = match row.owner {
        Some(owner) => owner.to_string(),
        None => "null".to_string(),
    };
    let delta = match row.delta {
        Some(delta) => delta.to_string(),
        None => "null".to_string(),
    };
    format!(
        "{indent}{{\n{field_indent}\"offset\": {},\n{field_indent}\"halfword\": {},\n{field_indent}\"owner\": {owner},\n{field_indent}\"delta\": {delta},\n{field_indent}\"verdict\": {}\n{indent}}}",
        row.offset,
        row.halfword,
        json_escape(&row.verdict),
    )
}

fn int_array_json(values: &[i64], level: usize) -> String {
    if values.is_empty() {
        return "[]".to_string();
    }
    let indent = indent_of(level);
    let item_indent = indent_of(level + 1);
    let body: Vec<String> = values.iter().map(|v| format!("{item_indent}{v}")).collect();
    format!("[\n{}\n{indent}]", body.join(",\n"))
}

/// `level` is the indent depth of the object itself (1 when it sits inside
/// the top-level results array).
pub fn residue_to_json(result: &OverlayResidue, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    let shaped = if result.shaped.is_empty() {
        "[]".to_string()
    } else {
        let item_indent = level + 2;
        let body: Vec<String> =
            result.shaped.iter().map(|row| shaped_row_json(row, item_indent)).collect();
        format!("[\n{}\n{field_indent}]", body.join(",\n"))
    };
    format!(
        "{indent}{{\n{field_indent}\"overlay\": {},\n{field_indent}\"called\": {},\n{field_indent}\"calledLeaf\": {},\n{field_indent}\"calledData\": {},\n{field_indent}\"published\": {},\n{field_indent}\"publishedLeaf\": {},\n{field_indent}\"publishedData\": {},\n{field_indent}\"shaped\": {}\n{indent}}}",
        json_escape(&result.overlay),
        int_array_json(&result.called, level + 1),
        int_array_json(&result.called_leaf, level + 1),
        int_array_json(&result.called_data, level + 1),
        int_array_json(&result.published, level + 1),
        int_array_json(&result.published_leaf, level + 1),
        int_array_json(&result.published_data, level + 1),
        shaped,
    )
}

pub fn results_to_json(results: &[OverlayResidue]) -> String {
    if results.is_empty() {
        return "[]".to_string();
    }
    let body: Vec<String> = results.iter().map(|r| residue_to_json(r, 1)).collect();
    format!("[\n{}\n]", body.join(",\n"))
}

/// `main()`.
pub fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    // A NAME IS REJECTED BY EXISTENCE, NOT BY SHAPE (fixed 2026-08-01). See
    // the TypeScript original's comment: a well-formed but nonexistent name
    // used to silently sweep every overlay instead of failing.
    let known: HashSet<String> = overlay_names()?.into_iter().collect();
    let requested: Vec<&String> = args.iter().filter(|a| !a.starts_with("--")).collect();
    for name in &requested {
        if !known.contains(name.as_str()) {
            println!("NOTHING SWEPT — no overlay named {name}. This is a FAILURE, not a pass.");
            std::process::exit(1);
        }
    }
    let overlays: Vec<String> = if !requested.is_empty() {
        requested.into_iter().cloned().collect()
    } else {
        overlay_names()?
    };
    let mut results = Vec::with_capacity(overlays.len());
    for overlay in &overlays {
        results.push(sweep(overlay)?);
    }
    if args.iter().any(|a| a == "--json") {
        println!("{}", results_to_json(&results));
        return Ok(());
    }
    let mut residue = 0i64;
    for result in &results {
        let unexplained: Vec<&ShapedRow> =
            result.shaped.iter().filter(|row| row.verdict.starts_with("UNEXPLAINED")).collect();
        let unruled: Vec<&ShapedRow> = result.shaped.iter().filter(|row| row.verdict.starts_with("UNRULED")).collect();
        if result.called.is_empty()
            && result.called_leaf.is_empty()
            && result.called_data.is_empty()
            && result.published.is_empty()
            && result.published_leaf.is_empty()
            && result.published_data.is_empty()
            && unexplained.is_empty()
            && unruled.is_empty()
        {
            continue;
        }
        residue += result.called.len() as i64
            + result.called_leaf.len() as i64
            + result.published.len() as i64
            + result.published_leaf.len() as i64
            + unexplained.len() as i64;
        println!("{}", result.overlay);
        for offset in &result.called {
            println!("  A called    {}", hex(*offset));
        }
        for offset in &result.called_leaf {
            println!("  A leaf      {}  bl-reached, no push prologue", hex(*offset));
        }
        for offset in &result.called_data {
            println!(
                "  A data      {}  bl-reached, no push prologue, NO RETURN within {RETURN_WINDOW} bytes -- RULED DATA, not residue",
                hex(*offset)
            );
        }
        for offset in &result.published {
            println!("  B published {}", hex(*offset));
        }
        for offset in &result.published_leaf {
            println!("  B leaf      {}  published, no push prologue", hex(*offset));
        }
        for offset in &result.published_data {
            println!(
                "  B data      {}  published, no push prologue, NO RETURN within {RETURN_WINDOW} bytes -- RULED DATA, not residue",
                hex(*offset)
            );
        }
        for row in unexplained.iter().chain(unruled.iter()) {
            let label = if row.verdict.starts_with("UNRULED") { "unruled " } else { "shaped  " };
            let owner = match row.owner {
                Some(owner) => hex(owner),
                None => "0x?".to_string(),
            };
            println!(
                "  C {label}  {}  {:x}  nearest owner {owner} +{}",
                hex(row.offset),
                row.halfword,
                row.delta.map(|d| d.to_string()).unwrap_or_else(|| "null".to_string()),
            );
        }
    }
    println!();
    println!("overlays={} residue={residue}", results.len());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn prologue_shape() {
        assert!(is_prologue_shape(0xb500));
        assert!(is_prologue_shape(0xb5e0));
        assert!(is_prologue_shape(0xb460));
        assert!(!is_prologue_shape(0xb082));
        assert!(!is_prologue_shape(0xbc60));
    }

    #[test]
    fn link_register_discriminator() {
        // resource_3af's pool word 0x0200b4bd decodes as
        // `push {r0, r2, r3, r4, r5, r7}` and cannot be returned through.
        assert!(!saves_link_register(0xb4bd));
        assert!(saves_link_register(0xb560));
    }

    #[test]
    fn published_resolution() {
        assert_eq!(published_offset(0x0200bc89, 0x5b2c), Some(0x3c88));
        assert_eq!(published_offset(0x0200bc88, 0x5b2c), None);
        assert_eq!(published_offset(0x0200bc89, 0x100), None);
        assert_eq!(published_offset(0x00000001, 0x5b2c), None);
    }

    #[test]
    fn exact_spans_resource_380() {
        let spans = exact_spans("resource_380");
        let row = spans.iter().find(|span| span.start == 0x390).expect("resource_380 row at 0x390");
        assert_eq!(row.end, 0x6f4);
        assert!(!spans.iter().any(|span| 0x6f4 > span.start && 0x6f4 < span.end));
        assert!(!spans.iter().any(|span| span.end <= span.start));
    }

    #[test]
    fn resource_380_has_no_unruled_rows() {
        let residue = sweep("resource_380").expect("resource_380 sweeps");
        let unruled: Vec<&ShapedRow> = residue.shaped.iter().filter(|row| row.verdict.starts_with("UNRULED")).collect();
        assert!(unruled.is_empty(), "expected no UNRULED rows, got {}", unruled.len());
    }

    #[test]
    fn return_window_discriminator() {
        let mut getter = vec![0u8; 64];
        getter[0] = 0x00;
        getter[1] = 0x48;
        getter[2] = 0x70;
        getter[3] = 0x47;
        assert!(reaches_return_default(&getter, 0));

        let table = vec![0u8; 64];
        assert!(!reaches_return_default(&table, 0));

        let mut far = vec![0u8; 64];
        far[40] = 0x70;
        far[41] = 0x47;
        assert!(!reaches_return(&far, 0, 16));
        assert!(reaches_return(&far, 0, 64));

        let mut short = vec![0u8; 4];
        short[2] = 0x70;
        short[3] = 0x47;
        assert!(reaches_return(&short, 0, 1024));
        assert!(!reaches_return(&vec![0u8; 4], 0, 1024));
    }

    #[test]
    fn sweep_a_tri_class() {
        let kind_of = |opening: &[u16]| -> &'static str {
            let mut image = vec![0u8; 0x60];
            for (index, halfword) in opening.iter().enumerate() {
                image[0x20 + index * 2] = (*halfword & 0xff) as u8;
                image[0x20 + index * 2 + 1] = (*halfword >> 8) as u8;
            }
            if classify(&image, 0x20, &HashSet::new()).kind == Kind::Prologue {
                "called"
            } else if reaches_return_default(&image, 0x20) {
                "calledLeaf"
            } else {
                "calledData"
            }
        };
        assert_eq!(kind_of(&[0xb500, 0x4770]), "called");
        assert_eq!(kind_of(&[0x4800, 0x4770]), "calledLeaf");
        assert_eq!(kind_of(&[0x0000, 0x0000]), "calledData");
        for shape in [[0xb500u16, 0x4770], [0x4800, 0x4770], [0x0000, 0x0000]] {
            assert!(["called", "calledLeaf", "calledData"].contains(&kind_of(&shape)));
        }
    }

    #[test]
    fn full_self_test() {
        self_test().expect("self_test() should pass");
    }
}
