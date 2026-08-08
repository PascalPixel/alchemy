//! `tools/overlay/overlay_call_order_check.ts`, in Rust.
//!
//! Compare semantic overlay sources with the resolved order of their Thumb
//! BLs. This is deliberately stronger than a multiset check: the source walk
//! below uses postorder evaluation, matching the dependency order the
//! compiler must preserve for nested calls, while keeping sibling argument
//! calls in source order as a conservative review signal. See the TypeScript
//! original's header comment for the full rationale; it is reproduced there,
//! not duplicated here.
//!
//! Ported byte-for-byte behavioural: the same postorder call extraction, the
//! same machine-call classification, and the same self-test.

use std::collections::{HashMap, HashSet};
use std::fs;
use std::path::{Path, PathBuf};

use discover::{Discovery, Mode};
use exact_reading_list::json::{parse as parse_json, Value};
use overlay_call_targets::{
    classify, overlay_image, resolve_overlay, target_offset, Kind, OVERLAY_BASE,
};

const CALL_VIA: &str = "(call_via)";
const USAGE: &str = "usage: overlay_call_order_check.ts <resource_NNN> [startHex [endHex]] [--json] | --all [--json] | --self-test";

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn source_directory() -> PathBuf {
    root().join("semantic")
}

fn regions_path() -> PathBuf {
    root().join("semantic").join("regions.json")
}

fn inventory_path() -> PathBuf {
    root().join("out").join("decomp").join("overlays.json")
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct SemanticOwner {
    pub overlay: String,
    pub offset: i64,
    pub span_bytes: i64,
    /// Path to the owning `.c` file (as a display string, matching the
    /// TypeScript original which keeps `owner.source` as a path).
    pub source: String,
    pub symbol: String,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct MachineCall {
    pub site: i64,
    pub target: i64,
    pub kind: Kind,
    pub name: String,
    /// A call-through target loaded from a nearby literal, when provable.
    pub resolved_call_via_target: Option<i64>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct OrderMismatch {
    pub index: i64,
    pub source: Option<String>,
    pub machine: Option<String>,
    pub site: Option<i64>,
    pub detail: Option<String>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct OrderResult {
    pub owner: SemanticOwner,
    pub source_calls: Vec<String>,
    pub machine_calls: Vec<MachineCall>,
    pub mismatches: Vec<OrderMismatch>,
    pub unresolved_call_via: i64,
}

struct InventoryRow {
    overlay: String,
    offset: i64,
    span_bytes: i64,
    starts_with_prologue: bool,
}

struct InventoryFile {
    functions: Vec<InventoryRow>,
}

fn hex8(value: i64) -> String {
    format!("{:08x}", value as u32)
}

fn read_manual_regions() -> Result<HashMap<String, i64>, String> {
    let path = regions_path();
    if !path.exists() {
        return Err(format!("missing {}", path.display()));
    }
    let text = fs::read_to_string(&path).map_err(|e| e.to_string())?;
    let parsed = parse_json(&text)?;
    let mut result = HashMap::new();
    let rows = parsed.get("manual_regions").and_then(Value::as_array).unwrap_or(&[]);
    for row in rows {
        let overlay = row.get("overlay").and_then(Value::as_str).unwrap_or("").to_string();
        let entry = row.get("entry").and_then(Value::as_str).unwrap_or("");
        let span_bytes = row.get("span_bytes").and_then(Value::as_f64);
        let entry_value = i64::from_str_radix(entry.trim_start_matches("0x").trim_start_matches("0X"), 16)
            .map_err(|_| format!("invalid semantic owner boundary: {row:?}"))?;
        let offset = entry_value - OVERLAY_BASE;
        let span_bytes = match span_bytes {
            Some(value) if value.fract() == 0.0 && value > 0.0 => value as i64,
            _ => return Err(format!("invalid semantic owner boundary: overlay={overlay} entry={entry}")),
        };
        if offset < 0 {
            return Err(format!("invalid semantic owner boundary: overlay={overlay} entry={entry}"));
        }
        let key = format!("{overlay}:{offset:x}");
        if result.contains_key(&key) {
            return Err(format!("duplicate semantic owner boundary: {key}"));
        }
        result.insert(key, span_bytes);
    }
    Ok(result)
}

fn read_inventory() -> Result<InventoryFile, String> {
    let path = inventory_path();
    if !path.exists() {
        return Err(format!("missing {}; run the overlay inventory first", path.display()));
    }
    let text = fs::read_to_string(&path).map_err(|e| e.to_string())?;
    let parsed = parse_json(&text)?;
    let functions = parsed.get("functions").and_then(Value::as_array).ok_or("overlays.json: missing functions array")?;
    let mut rows = Vec::with_capacity(functions.len());
    for function in functions {
        rows.push(InventoryRow {
            overlay: function.get("overlay").and_then(Value::as_str).unwrap_or("").to_string(),
            offset: function.get("offset").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            span_bytes: function.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            starts_with_prologue: function.get("starts_with_prologue").map(Value::truthy).unwrap_or(false),
        });
    }
    Ok(InventoryFile { functions: rows })
}

#[derive(Default, Clone)]
pub struct OwnerSelection {
    pub overlay: Option<String>,
    pub start: Option<i64>,
}

fn is_word_char(c: char) -> bool {
    c.is_ascii_alphanumeric() || c == '_'
}

/// `/^(resource_[0-9a-f]+)_c_([0-9a-f]{8})\.c$/i`, returning the two captures
/// with their original casing.
fn match_owner_filename(name: &str) -> Option<(String, String)> {
    let chars: Vec<char> = name.chars().collect();
    let prefix = "resource_";
    if chars.len() < prefix.len() {
        return None;
    }
    if !chars[..prefix.len()].iter().collect::<String>().eq_ignore_ascii_case(prefix) {
        return None;
    }
    let mut i = prefix.len();
    let hex_start = i;
    while i < chars.len() && chars[i].is_ascii_hexdigit() {
        i += 1;
    }
    if i == hex_start {
        return None;
    }
    let group1_end = i;
    let marker = "_c_";
    if i + marker.len() > chars.len() {
        return None;
    }
    if !chars[i..i + marker.len()].iter().collect::<String>().eq_ignore_ascii_case(marker) {
        return None;
    }
    i += marker.len();
    let hex2_start = i;
    if i + 8 > chars.len() {
        return None;
    }
    for k in 0..8 {
        if !chars[i + k].is_ascii_hexdigit() {
            return None;
        }
    }
    i += 8;
    let hex2_end = i;
    if i + 2 != chars.len() {
        return None;
    }
    if chars[i] != '.' || (chars[i + 1] != 'c' && chars[i + 1] != 'C') {
        return None;
    }
    let group1: String = chars[0..group1_end].iter().collect();
    let group2: String = chars[hex2_start..hex2_end].iter().collect();
    Some((group1, group2))
}

fn source_owner_files(selection: &OwnerSelection) -> Result<Vec<PathBuf>, String> {
    let dir = source_directory();
    let entries = fs::read_dir(&dir).map_err(|e| e.to_string())?;
    let mut names: Vec<String> = Vec::new();
    for entry in entries {
        let entry = entry.map_err(|e| e.to_string())?;
        let Some(name) = entry.file_name().to_str().map(str::to_string) else { continue };
        let Some((group1, group2)) = match_owner_filename(&name) else { continue };
        if let Some(overlay) = &selection.overlay {
            if group1.to_lowercase() != *overlay {
                continue;
            }
        }
        if let Some(start) = selection.start {
            let absolute = i64::from_str_radix(&group2, 16).map_err(|e| e.to_string())?;
            if absolute - OVERLAY_BASE != start {
                continue;
            }
        }
        names.push(name);
    }
    names.sort();
    Ok(names.into_iter().map(|name| dir.join(name)).collect())
}

/// Case-sensitive `\bsymbol\s*\(` presence test, matching the TypeScript
/// original's un-flagged (case-sensitive) `RegExp(...).test(source)` guard.
fn contains_symbol_call(source: &str, symbol: &str) -> bool {
    let chars: Vec<char> = source.chars().collect();
    let sym: Vec<char> = symbol.chars().collect();
    if sym.is_empty() || sym.len() > chars.len() {
        return false;
    }
    for i in 0..=(chars.len() - sym.len()) {
        if i > 0 && is_word_char(chars[i - 1]) {
            continue;
        }
        if chars[i..i + sym.len()] != sym[..] {
            continue;
        }
        let mut j = i + sym.len();
        while j < chars.len() && chars[j].is_whitespace() {
            j += 1;
        }
        if j < chars.len() && chars[j] == '(' {
            return true;
        }
    }
    false
}

fn owner_from_source(path: &Path, manual: &HashMap<String, i64>, inventory: &InventoryFile) -> Result<SemanticOwner, String> {
    let name = path.file_name().and_then(|n| n.to_str()).unwrap_or("");
    let Some((group1, group2)) = match_owner_filename(name) else {
        return Err(format!("not a semantic overlay owner source: {}", path.display()));
    };
    let overlay = group1.to_lowercase();
    let absolute = i64::from_str_radix(&group2, 16).map_err(|e| e.to_string())?;
    let offset = absolute - OVERLAY_BASE;
    if offset < 0 {
        return Err(format!("owner address is below overlay base: {}", path.display()));
    }
    let key = format!("{overlay}:{offset:x}");
    let manual_span = manual.get(&key).copied();
    let inventory_row = inventory.functions.iter().find(|row| row.overlay == overlay && row.offset == offset);
    let span_bytes = manual_span.or_else(|| inventory_row.map(|row| row.span_bytes));
    let Some(span_bytes) = span_bytes else {
        return Err(format!("no admitted boundary for {overlay}:0x{offset:x} ({})", path.display()));
    };
    let source = fs::read_to_string(path).map_err(|e| e.to_string())?;
    let symbol = format!("Func_{}", group2.to_lowercase());
    if !contains_symbol_call(&source, &symbol) {
        return Err(format!("{} does not contain its owner symbol {symbol}", path.display()));
    }
    Ok(SemanticOwner { overlay, offset, span_bytes, source: path.display().to_string(), symbol })
}

pub fn load_owners(selection: &OwnerSelection) -> Result<Vec<SemanticOwner>, String> {
    let manual = read_manual_regions()?;
    let inventory = read_inventory()?;
    source_owner_files(selection)?.iter().map(|path| owner_from_source(path, &manual, &inventory)).collect()
}

// ---------------------------------------------------------------------------
// Source-level postorder call extraction.
// ---------------------------------------------------------------------------

fn replace_block_comments(chars: &[char]) -> Vec<char> {
    let mut out = Vec::with_capacity(chars.len());
    let mut i = 0;
    while i < chars.len() {
        if chars[i] == '/' && i + 1 < chars.len() && chars[i + 1] == '*' {
            let mut j = i + 2;
            while j + 1 < chars.len() && !(chars[j] == '*' && chars[j + 1] == '/') {
                j += 1;
            }
            if j + 1 < chars.len() {
                out.push(' ');
                i = j + 2;
            } else {
                out.extend_from_slice(&chars[i..]);
                i = chars.len();
            }
        } else {
            out.push(chars[i]);
            i += 1;
        }
    }
    out
}

fn replace_line_comments(chars: &[char]) -> Vec<char> {
    let mut out = Vec::with_capacity(chars.len());
    let mut i = 0;
    while i < chars.len() {
        if chars[i] == '/' && i + 1 < chars.len() && chars[i + 1] == '/' {
            while i < chars.len() && chars[i] != '\n' {
                i += 1;
            }
            out.push(' ');
        } else {
            out.push(chars[i]);
            i += 1;
        }
    }
    out
}

fn replace_quoted(chars: &[char], quote: char) -> Vec<char> {
    let mut out = Vec::with_capacity(chars.len());
    let mut i = 0;
    while i < chars.len() {
        if chars[i] == quote {
            let mut j = i + 1;
            let mut closed = false;
            while j < chars.len() {
                if chars[j] == '\\' && j + 1 < chars.len() {
                    j += 2;
                    continue;
                }
                if chars[j] == quote {
                    closed = true;
                    j += 1;
                    break;
                }
                j += 1;
            }
            if closed {
                out.push(quote);
                out.push(quote);
                i = j;
            } else {
                out.extend_from_slice(&chars[i..]);
                i = chars.len();
            }
        } else {
            out.push(chars[i]);
            i += 1;
        }
    }
    out
}

fn strip_comments_and_strings(source: &str) -> Vec<char> {
    let chars: Vec<char> = source.chars().collect();
    let chars = replace_block_comments(&chars);
    let chars = replace_line_comments(&chars);
    let chars = replace_quoted(&chars, '"');
    replace_quoted(&chars, '\'')
}

fn matching_delimiter(text: &[char], open: usize, left: char, right: char) -> Result<usize, String> {
    let mut depth = 0i64;
    for index in open..text.len() {
        if text[index] == left {
            depth += 1;
        } else if text[index] == right {
            depth -= 1;
            if depth == 0 {
                return Ok(index);
            }
        }
    }
    Err(format!("unbalanced {left}{right} pair near byte {open}"))
}

fn skip_ws_expect_paren(text: &[char], from: usize) -> Option<usize> {
    let mut j = from;
    while j < text.len() && text[j].is_whitespace() {
        j += 1;
    }
    if j < text.len() && text[j] == '(' {
        Some(j)
    } else {
        None
    }
}

/// `\bsymbol\s*\(`, case-insensitive, searching from `from`. Returns the
/// index of the `(`.
fn find_symbol_paren(clean: &[char], symbol: &[char], from: usize) -> Option<usize> {
    if symbol.is_empty() || symbol.len() > clean.len() {
        return None;
    }
    for i in from..=(clean.len() - symbol.len()) {
        if i > 0 && is_word_char(clean[i - 1]) {
            continue;
        }
        let mut matches = true;
        for k in 0..symbol.len() {
            if !clean[i + k].eq_ignore_ascii_case(&symbol[k]) {
                matches = false;
                break;
            }
        }
        if !matches {
            continue;
        }
        if let Some(open) = skip_ws_expect_paren(clean, i + symbol.len()) {
            return Some(open);
        }
        // A non-`(` continuation is not a global-regex retry point here: the
        // TypeScript's `pattern.lastIndex` is advanced automatically by the
        // failed match attempt only when the whole pattern fails, which for a
        // fixed-length prefix like this one means trying the very next
        // index — exactly the outer loop's next iteration.
    }
    None
}

fn owner_body(source: &str, symbol: &str) -> Result<Vec<char>, String> {
    let clean = strip_comments_and_strings(source);
    let symbol_chars: Vec<char> = symbol.chars().collect();
    let mut cursor = 0usize;
    loop {
        let Some(open) = find_symbol_paren(&clean, &symbol_chars, cursor) else {
            return Err(format!("{symbol} has no function definition in its source"));
        };
        let close = matching_delimiter(&clean, open, '(', ')')?;
        let mut after = close + 1;
        while after < clean.len() && clean[after].is_whitespace() {
            after += 1;
        }
        if after >= clean.len() || clean[after] != '{' {
            // Prototype/declaration: JS `pattern.lastIndex` continues from the
            // end of the matched "symbol\s*(" text, i.e. right after `open`.
            cursor = open + 1;
            continue;
        }
        let body_end = matching_delimiter(&clean, after, '{', '}')?;
        return Ok(clean[after + 1..body_end].to_vec());
    }
}

fn is_hex8(chars: &[char]) -> bool {
    chars.len() == 8 && chars.iter().all(|c| c.is_ascii_hexdigit())
}

/// `\b(Func_[0-9a-f]{8}|callback)\s*\(`, case-insensitive, from `from`.
/// Returns `(open_paren_index, canonical_lowercase_token)`.
fn find_call_start(body: &[char], from: usize) -> Option<(usize, String)> {
    let callback: Vec<char> = "callback".chars().collect();
    let func_prefix: Vec<char> = "func_".chars().collect();
    for i in from..body.len() {
        if i > 0 && is_word_char(body[i - 1]) {
            continue;
        }
        if i + func_prefix.len() <= body.len()
            && body[i..i + func_prefix.len()].iter().zip(&func_prefix).all(|(a, b)| a.eq_ignore_ascii_case(b))
            && i + func_prefix.len() + 8 <= body.len()
            && is_hex8(&body[i + func_prefix.len()..i + func_prefix.len() + 8])
        {
            let after = i + func_prefix.len() + 8;
            if let Some(open) = skip_ws_expect_paren(body, after) {
                let hex: String = body[i + func_prefix.len()..after].iter().collect::<String>().to_lowercase();
                return Some((open, format!("func_{hex}")));
            }
        }
        if i + callback.len() <= body.len()
            && body[i..i + callback.len()].iter().zip(&callback).all(|(a, b)| a.eq_ignore_ascii_case(b))
        {
            let after = i + callback.len();
            if let Some(open) = skip_ws_expect_paren(body, after) {
                return Some((open, "callback".to_string()));
            }
        }
    }
    None
}

fn calls_in_body(body: &[char]) -> Result<Vec<String>, String> {
    let mut calls = Vec::new();
    let mut cursor = 0usize;
    loop {
        let Some((open, token)) = find_call_start(body, cursor) else { break };
        let close = matching_delimiter(body, open, '(', ')')?;
        let nested = calls_in_body(&body[open + 1..close])?;
        calls.extend(nested);
        calls.push(if token == "callback" { CALL_VIA.to_string() } else { format!("Func_{}", &token[5..]) });
        cursor = close + 1;
    }
    Ok(calls)
}

/// Keep the public function small and make its recursion impossible to
/// confuse with owner-definition discovery.
pub fn source_calls(source: &str, symbol: &str) -> Result<Vec<String>, String> {
    calls_in_body(&owner_body(source, symbol)?)
}

// ---------------------------------------------------------------------------
// Machine-level call extraction.
// ---------------------------------------------------------------------------

fn get_byte(image: &[u8], offset: i64) -> i64 {
    if offset < 0 {
        return 0;
    }
    let o = offset as usize;
    if o < image.len() {
        image[o] as i64
    } else {
        0
    }
}

fn read_u16(image: &[u8], offset: i64) -> i64 {
    get_byte(image, offset) | (get_byte(image, offset + 1) << 8)
}

fn read_u32(image: &[u8], offset: i64) -> i64 {
    get_byte(image, offset)
        | (get_byte(image, offset + 1) << 8)
        | (get_byte(image, offset + 2) << 16)
        | (get_byte(image, offset + 3) << 24)
}

/// Recover a call-through target only for the strong, local pattern used by
/// the historical overlay output: a literal LDR into the slot's BX register
/// in the preceding 40 bytes. A global pointer or a computed target stays
/// unresolved; guessing in that case would turn an audit into a
/// source-specific oracle.
pub fn call_via_literal_target(image: &[u8], slot_offset: i64, site_offset: i64) -> Option<i64> {
    if slot_offset < 0 || slot_offset + 2 > image.len() as i64 {
        return None;
    }
    let slot = read_u16(image, slot_offset);
    if (slot & 0xff87) != 0x4700 {
        return None;
    }
    let register = (slot >> 3) & 15;
    let mut at = site_offset - 2;
    let floor = std::cmp::max(0, site_offset - 40);
    while at >= floor {
        let half = read_u16(image, at);
        if (half & 0xf800) != 0x4800 {
            at -= 2;
            continue;
        }
        let loaded_register = (half >> 8) & 7;
        if loaded_register != register {
            at -= 2;
            continue;
        }
        let literal = (((OVERLAY_BASE + at + 4) & !3) + ((half & 0xff) << 2)) - OVERLAY_BASE;
        if literal < 0 || literal + 4 > image.len() as i64 {
            return None;
        }
        let value = read_u32(image, literal);
        // IWRAM call-through functions are the only established external
        // targets in this family. A Thumb bit, if present, is metadata
        // rather than part of the name written in the C source.
        if (value & 0xff000000) != 0x03000000 {
            return None;
        }
        return Some(value & !1);
    }
    None
}

fn inventory_prologues(inventory: &InventoryFile, overlay: &str) -> HashSet<i64> {
    inventory
        .functions
        .iter()
        .filter(|row| row.overlay == overlay && row.starts_with_prologue)
        .map(|row| row.offset)
        .collect()
}

fn machine_call_from_target(
    image: &[u8],
    owner: &SemanticOwner,
    prologues: &HashSet<i64>,
    site_offset: i64,
    target: i64,
) -> Option<MachineCall> {
    // BLs into the owner's own span are compiler-generated internal
    // transfers, not calls represented by a separate source function.
    if target > owner.offset && target < owner.offset + owner.span_bytes {
        return None;
    }
    let detail = classify(image, target, prologues);
    if detail.kind == Kind::Veneer {
        if let Some(imported) = detail.imported {
            return Some(MachineCall {
                site: site_offset,
                target,
                kind: detail.kind,
                name: format!("Func_{}", hex8(imported)),
                resolved_call_via_target: None,
            });
        }
    }
    if detail.kind == Kind::CallVia {
        // A bare `bx lr` is a genuine local leaf, not an indirect-call slot.
        // The shared resolver intentionally reports this shape as call_via;
        // mirror overlay_multiset_check's explicit exception so established
        // no-op leaf owners remain comparable by their local address.
        if read_u16(image, target) == 0x4770 {
            return Some(MachineCall {
                site: site_offset,
                target,
                kind: Kind::Leaf,
                name: format!("Func_{}", hex8(OVERLAY_BASE + target)),
                resolved_call_via_target: None,
            });
        }
        let resolved = call_via_literal_target(image, target, site_offset);
        return Some(MachineCall {
            site: site_offset,
            target,
            kind: detail.kind,
            name: CALL_VIA.to_string(),
            resolved_call_via_target: resolved,
        });
    }
    Some(MachineCall {
        site: site_offset,
        target,
        kind: detail.kind,
        name: if detail.kind == Kind::Unknown {
            format!("UNKNOWN_{}", hex8(target))
        } else {
            format!("Func_{}", hex8(OVERLAY_BASE + target))
        },
        resolved_call_via_target: None,
    })
}

/// True when the reachable prefix contains ARMv4T's inline indirect-call
/// idiom.
pub fn contains_inline_call_through(image: &[u8], owner_offset: i64, owner_span_bytes: i64, reachable: &HashSet<i64>) -> bool {
    reachable.iter().any(|&address| {
        let offset = address - OVERLAY_BASE;
        offset >= owner_offset
            && offset + 4 <= owner_offset + owner_span_bytes
            && read_u16(image, offset) == 0x46fc
            && (read_u16(image, offset + 2) & 0xff87) == 0x4700
    })
}

fn machine_calls(owner: &SemanticOwner, image: &[u8], prologues: &HashSet<i64>) -> Result<Vec<MachineCall>, String> {
    let start = OVERLAY_BASE + owner.offset;
    let end = std::cmp::min(start + owner.span_bytes, OVERLAY_BASE + image.len() as i64);
    let mut discovery = Discovery::new(image, OVERLAY_BASE);
    discovery.add_seed(start, Mode::Thumb, "ordered-call-audit");
    discovery.walk_function(start);
    let function_info = discovery.function(start);
    let reachable: HashSet<i64> = function_info.map(|info| info.instructions.iter().copied().collect()).unwrap_or_default();
    if reachable.is_empty() {
        return Err(format!("{}:0x{:x} has no reachable instructions", owner.overlay, owner.offset));
    }

    // Discovery deliberately stops at an unresolved `mov pc, rN`, because it
    // cannot prove the table arms without overlay-specific link semantics. It
    // also treats an inline `mov ip,pc / bx rN` IWRAM call as a return even
    // though execution resumes at the next halfword. The resolver has the
    // complementary evidence: the overlay's stored BL rule (`target - 2`)
    // and the complete explicit owner bound. When the walk records a
    // jump-table dispatch, use that bounded resolver listing so the order
    // audit covers the arms after the table instead of returning only the
    // pre-dispatch prefix. Unknown flat-disassembly hits remain visible as
    // UNKNOWN_*; a pool masquerading as BL is therefore a review signal, not
    // silently discarded.
    let has_jump_table = function_info
        .map(|info| info.unresolved.iter().copied().collect::<Vec<_>>())
        .unwrap_or_default()
        .into_iter()
        .any(|address| {
            let offset = address - OVERLAY_BASE;
            offset >= owner.offset
                && offset + 2 <= owner.offset + owner.span_bytes
                && (read_u16(image, offset) & 0xff87) == 0x4687
        });
    let has_inline_call_through = contains_inline_call_through(image, owner.offset, owner.span_bytes, &reachable);
    if has_jump_table || has_inline_call_through {
        let mut calls = Vec::new();
        for site in resolve_overlay(&owner.overlay, Some(owner.offset), Some(owner.offset + owner.span_bytes))? {
            if let Some(call) = machine_call_from_target(image, owner, prologues, site.site, site.target) {
                calls.push(call);
            }
        }
        return Ok(calls);
    }

    let mut calls = Vec::new();
    let mut address = start;
    while address + 3 < end {
        if reachable.contains(&address) {
            let site_offset = address - OVERLAY_BASE;
            let high = read_u16(image, site_offset) as u16;
            let low = read_u16(image, site_offset + 2) as u16;
            if let Some(target) = target_offset(high, low) {
                if target >= 0 && target < image.len() as i64 {
                    if let Some(call) = machine_call_from_target(image, owner, prologues, site_offset, target) {
                        calls.push(call);
                    }
                }
            }
        }
        address += 2;
    }
    Ok(calls)
}

fn is_iram_name(name: &str) -> bool {
    let prefix = "func_03";
    let lower = name.to_ascii_lowercase();
    if !lower.starts_with(prefix) {
        return false;
    }
    let rest = &lower[prefix.len()..];
    rest.len() == 6 && rest.chars().all(|c| c.is_ascii_hexdigit())
}

pub fn compare_order(owner: SemanticOwner, source_sequence: Vec<String>, machine_sequence: Vec<MachineCall>) -> OrderResult {
    let mut mismatches = Vec::new();
    let mut unresolved_call_via = 0i64;
    let count = std::cmp::max(source_sequence.len(), machine_sequence.len());
    for index in 0..count {
        let source = source_sequence.get(index).cloned();
        let machine = machine_sequence.get(index).cloned();
        let machine = match machine {
            None => {
                mismatches.push(OrderMismatch {
                    index: index as i64,
                    source,
                    machine: None,
                    site: None,
                    detail: Some("source call has no resolved BL site".to_string()),
                });
                continue;
            }
            Some(machine) => machine,
        };
        let Some(source) = source else {
            mismatches.push(OrderMismatch {
                index: index as i64,
                source: None,
                machine: Some(machine.name.clone()),
                site: Some(machine.site),
                detail: Some("resolved BL site has no source call".to_string()),
            });
            continue;
        };
        if machine.kind == Kind::CallVia {
            if source == CALL_VIA {
                if machine.resolved_call_via_target.is_none() {
                    unresolved_call_via += 1;
                }
            } else if let Some(resolved) = machine.resolved_call_via_target {
                let actual = format!("Func_{}", hex8(resolved));
                if actual != source {
                    mismatches.push(OrderMismatch {
                        index: index as i64,
                        source: Some(source),
                        machine: Some(actual),
                        site: Some(machine.site),
                        detail: Some("resolved call_via target differs".to_string()),
                    });
                }
            } else if is_iram_name(&source) {
                unresolved_call_via += 1;
            } else {
                mismatches.push(OrderMismatch {
                    index: index as i64,
                    source: Some(source),
                    machine: Some(CALL_VIA.to_string()),
                    site: Some(machine.site),
                    detail: Some("unresolved call_via is not an IWRAM source call".to_string()),
                });
            }
            continue;
        }
        if machine.name != source {
            mismatches.push(OrderMismatch {
                index: index as i64,
                source: Some(source),
                machine: Some(machine.name.clone()),
                site: Some(machine.site),
                detail: Some(format!("machine kind={}", machine.kind.as_str())),
            });
        }
    }
    OrderResult { owner, source_calls: source_sequence, machine_calls: machine_sequence, mismatches, unresolved_call_via }
}

fn format_owner(owner: &SemanticOwner) -> String {
    format!("{}:0x{:0>4x}", owner.overlay, owner.offset)
}

fn audit_owners(owners: &[SemanticOwner], json: bool) -> Result<i32, String> {
    let inventory = read_inventory()?;
    let mut images: HashMap<String, Vec<u8>> = HashMap::new();
    let mut prologue_sets: HashMap<String, HashSet<i64>> = HashMap::new();
    let mut results: Vec<OrderResult> = Vec::new();
    for owner in owners {
        if !images.contains_key(&owner.overlay) {
            let image = overlay_image(&owner.overlay)?;
            prologue_sets.insert(owner.overlay.clone(), inventory_prologues(&inventory, &owner.overlay));
            images.insert(owner.overlay.clone(), image);
        }
        let image = &images[&owner.overlay];
        let source = fs::read_to_string(&owner.source).map_err(|e| e.to_string())?;
        let source_sequence = source_calls(&source, &owner.symbol)?;
        let machine_sequence = machine_calls(owner, image, &prologue_sets[&owner.overlay])?;
        results.push(compare_order(owner.clone(), source_sequence, machine_sequence));
    }
    if json {
        println!("{}", results_to_json(&results));
    } else {
        let mut failures = 0;
        let mut unresolved = 0i64;
        for result in &results {
            unresolved += result.unresolved_call_via;
            if result.mismatches.is_empty() {
                continue;
            }
            failures += 1;
            println!(
                "FAIL {} source={} machine={}",
                format_owner(&result.owner),
                result.source_calls.len(),
                result.machine_calls.len()
            );
            for mismatch in result.mismatches.iter().take(8) {
                let source = mismatch.source.clone().unwrap_or_else(|| "<none>".to_string());
                let machine = mismatch.machine.clone().unwrap_or_else(|| "<none>".to_string());
                let site = mismatch.site.map(|s| format!(" site=0x{:x}", OVERLAY_BASE + s)).unwrap_or_default();
                println!(
                    "  [{}] source={source} machine={machine}{site} ({})",
                    mismatch.index,
                    mismatch.detail.clone().unwrap_or_else(|| "mismatch".to_string())
                );
            }
            if result.mismatches.len() > 8 {
                println!("  ... {} more mismatch(es)", result.mismatches.len() - 8);
            }
        }
        println!("owners={} passed={} failed={} unresolved_call_via={}", results.len(), results.len() - failures, failures, unresolved);
    }
    Ok(if results.iter().any(|r| !r.mismatches.is_empty()) { 1 } else { 0 })
}

// ---------------------------------------------------------------------------
// JSON output (`JSON.stringify(results, null, 2)`).
// ---------------------------------------------------------------------------

fn indent_of(level: usize) -> String {
    "  ".repeat(level)
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

fn string_array_json(values: &[String], level: usize) -> String {
    if values.is_empty() {
        return "[]".to_string();
    }
    let indent = indent_of(level);
    let item_indent = indent_of(level + 1);
    let body: Vec<String> = values.iter().map(|v| format!("{item_indent}{}", json_escape(v))).collect();
    format!("[\n{}\n{indent}]", body.join(",\n"))
}

fn owner_json(owner: &SemanticOwner, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    format!(
        "{indent}{{\n{field_indent}\"overlay\": {},\n{field_indent}\"offset\": {},\n{field_indent}\"spanBytes\": {},\n{field_indent}\"source\": {},\n{field_indent}\"symbol\": {}\n{indent}}}",
        json_escape(&owner.overlay),
        owner.offset,
        owner.span_bytes,
        json_escape(&owner.source),
        json_escape(&owner.symbol),
    )
}

fn machine_call_json(call: &MachineCall, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    let mut fields = vec![
        format!("{field_indent}\"site\": {}", call.site),
        format!("{field_indent}\"target\": {}", call.target),
        format!("{field_indent}\"kind\": {}", json_escape(call.kind.as_str())),
        format!("{field_indent}\"name\": {}", json_escape(&call.name)),
    ];
    if let Some(resolved) = call.resolved_call_via_target {
        fields.push(format!("{field_indent}\"resolvedCallViaTarget\": {resolved}"));
    }
    format!("{indent}{{\n{}\n{indent}}}", fields.join(",\n"))
}

fn machine_calls_json(calls: &[MachineCall], level: usize) -> String {
    if calls.is_empty() {
        return "[]".to_string();
    }
    let indent = indent_of(level);
    let item_indent = level + 1;
    let body: Vec<String> = calls.iter().map(|c| machine_call_json(c, item_indent)).collect();
    format!("[\n{}\n{indent}]", body.join(",\n"))
}

fn mismatch_json(mismatch: &OrderMismatch, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    let mut fields = vec![format!("{field_indent}\"index\": {}", mismatch.index)];
    if let Some(source) = &mismatch.source {
        fields.push(format!("{field_indent}\"source\": {}", json_escape(source)));
    }
    if let Some(machine) = &mismatch.machine {
        fields.push(format!("{field_indent}\"machine\": {}", json_escape(machine)));
    }
    if let Some(site) = mismatch.site {
        fields.push(format!("{field_indent}\"site\": {site}"));
    }
    if let Some(detail) = &mismatch.detail {
        fields.push(format!("{field_indent}\"detail\": {}", json_escape(detail)));
    }
    format!("{indent}{{\n{}\n{indent}}}", fields.join(",\n"))
}

fn mismatches_json(mismatches: &[OrderMismatch], level: usize) -> String {
    if mismatches.is_empty() {
        return "[]".to_string();
    }
    let indent = indent_of(level);
    let item_indent = level + 1;
    let body: Vec<String> = mismatches.iter().map(|m| mismatch_json(m, item_indent)).collect();
    format!("[\n{}\n{indent}]", body.join(",\n"))
}

pub fn result_json(result: &OrderResult, level: usize) -> String {
    let indent = indent_of(level);
    let field_indent = indent_of(level + 1);
    format!(
        "{indent}{{\n{field_indent}\"owner\": {},\n{field_indent}\"sourceCalls\": {},\n{field_indent}\"machineCalls\": {},\n{field_indent}\"mismatches\": {},\n{field_indent}\"unresolvedCallVia\": {}\n{indent}}}",
        owner_json(&result.owner, level + 1).trim_start(),
        string_array_json(&result.source_calls, level + 1),
        machine_calls_json(&result.machine_calls, level + 1),
        mismatches_json(&result.mismatches, level + 1),
        result.unresolved_call_via,
    )
}

pub fn results_to_json(results: &[OrderResult]) -> String {
    if results.is_empty() {
        return "[]".to_string();
    }
    let body: Vec<String> = results.iter().map(|r| result_json(r, 1)).collect();
    format!("[\n{}\n]", body.join(",\n"))
}

// ---------------------------------------------------------------------------
// CLI argument parsing.
// ---------------------------------------------------------------------------

pub struct ParsedArgs {
    pub overlay: Option<String>,
    pub start: Option<i64>,
    pub end: Option<i64>,
    pub all: bool,
    pub json: bool,
}

fn is_resource_token(s: &str) -> bool {
    let lower = s.to_ascii_lowercase();
    match lower.strip_prefix("resource_") {
        Some(rest) => !rest.is_empty() && rest.chars().all(|c| c.is_ascii_hexdigit()),
        None => false,
    }
}

fn parse_hex_bound(s: &str) -> Option<i64> {
    let lower = s.to_ascii_lowercase();
    let rest = lower.strip_prefix("0x").unwrap_or(&lower);
    if rest.is_empty() || rest.len() > 4 || !rest.chars().all(|c| c.is_ascii_hexdigit()) {
        return None;
    }
    i64::from_str_radix(rest, 16).ok()
}

pub fn parse_args(args: &[String]) -> Result<ParsedArgs, String> {
    let mut overlay: Option<String> = None;
    let mut bounds: Vec<i64> = Vec::new();
    let mut all = false;
    let mut json = false;
    for argument in args {
        if argument == "--all" {
            all = true;
            continue;
        }
        if argument == "--json" {
            json = true;
            continue;
        }
        if is_resource_token(argument) {
            if overlay.is_some() {
                return Err("only one resource overlay may be selected".to_string());
            }
            overlay = Some(argument.to_ascii_lowercase());
            continue;
        }
        if let Some(value) = parse_hex_bound(argument) {
            bounds.push(value);
            continue;
        }
        return Err(format!("unrecognised argument \"{argument}\""));
    }
    if bounds.len() > 2 {
        return Err("at most two owner bounds are accepted".to_string());
    }
    if overlay.is_none() && !all {
        return Err("select an overlay or pass --all".to_string());
    }
    if overlay.is_none() && !bounds.is_empty() {
        return Err("owner bounds require an overlay".to_string());
    }
    if all && overlay.is_some() {
        return Err("--all and an overlay are mutually exclusive".to_string());
    }
    Ok(ParsedArgs { overlay, start: bounds.first().copied(), end: bounds.get(1).copied(), all, json })
}

// ---------------------------------------------------------------------------
// Self-test.
// ---------------------------------------------------------------------------

pub fn self_test() -> Result<(), String> {
    let source = "void Func_02000000(void) { Func_08000002(Func_08000001(1), Func_02000004()); }";
    let expected = vec!["Func_08000001".to_string(), "Func_02000004".to_string(), "Func_08000002".to_string()];
    let actual = source_calls(source, "Func_02000000")?;
    if actual != expected {
        return Err("postorder source-call self-test failed".to_string());
    }
    let callback_source = "void Func_02000000(void (*callback)(void)) { if (callback) callback(); Func_08000002(); }";
    let callback_calls = source_calls(callback_source, "Func_02000000")?;
    if callback_calls != vec![CALL_VIA.to_string(), "Func_08000002".to_string()] {
        return Err("source callback must remain visible as call_via".to_string());
    }

    let mut image = vec![0u8; 0x40];
    // A call-through slot at offset 0x20: bx r3, with a literal target loaded
    // by `ldr r3,[pc,#0]` at the call site immediately before it.
    image[0x20] = 0x18;
    image[0x21] = 0x47;
    image[0x00] = 0x01;
    image[0x01] = 0x48; // ldr r0,[pc,#4] (not slot r3)
    image[0x02] = 0x03;
    image[0x03] = 0x48; // ldr r0,[pc,#12] (not used)
    // For a simple fixture, callViaLiteralTarget should refuse unrelated loads.
    if call_via_literal_target(&image, 0x20, 0x08).is_some() {
        return Err("call_via register self-test failed".to_string());
    }
    image[0x08] = 0x01;
    image[0x09] = 0x4b; // ldr r3,[pc,#4], pool at 0x10
    image[0x10] = 0xd9;
    image[0x11] = 0x01;
    image[0x12] = 0x00;
    image[0x13] = 0x03;
    if call_via_literal_target(&image, 0x20, 0x0a) != Some(0x030001d8) {
        return Err("call_via literal-target self-test failed".to_string());
    }

    let owner = SemanticOwner {
        overlay: "resource_test".to_string(),
        offset: 0,
        span_bytes: 2,
        source: "self-test".to_string(),
        symbol: "Func_02000000".to_string(),
    };
    let via = MachineCall { site: 0, target: 0x20, kind: Kind::CallVia, name: CALL_VIA.to_string(), resolved_call_via_target: None };
    let accepted = compare_order(owner.clone(), vec!["Func_030001d8".to_string()], vec![via.clone()]);
    if !accepted.mismatches.is_empty() || accepted.unresolved_call_via != 1 {
        return Err("unresolved call_via self-test failed".to_string());
    }
    let explicit = compare_order(owner.clone(), vec![CALL_VIA.to_string()], vec![via.clone()]);
    if !explicit.mismatches.is_empty() || explicit.unresolved_call_via != 1 {
        return Err("explicit callback call_via self-test failed".to_string());
    }
    let rejected = compare_order(owner, vec!["Func_08000000".to_string()], vec![via]);
    if rejected.mismatches.len() != 1 {
        return Err("call_via source mismatch self-test failed".to_string());
    }

    let inline: Vec<u8> = vec![0xfc, 0x46, 0x18, 0x47, 0x00, 0x20];
    let reachable_hit: HashSet<i64> = [OVERLAY_BASE].into_iter().collect();
    if !contains_inline_call_through(&inline, 0, inline.len() as i64, &reachable_hit) {
        return Err("inline call-through continuation self-test failed".to_string());
    }
    let reachable_miss: HashSet<i64> = [OVERLAY_BASE + 4].into_iter().collect();
    if contains_inline_call_through(&inline, 0, inline.len() as i64, &reachable_miss) {
        return Err("unreachable inline call-through self-test failed".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

// ---------------------------------------------------------------------------
// Entry point.
// ---------------------------------------------------------------------------

/// `main()`. Returns the process exit code: 0 for a clean pass, 1 for
/// findings (mismatches) or `--help`'s early return. Errors surface as `Err`
/// for the caller to print to stderr, matching the TypeScript original's
/// `catch` block.
pub fn run(args: &[String]) -> Result<i32, String> {
    if args.iter().any(|a| a == "--help" || a == "-h") {
        println!("{USAGE}");
        return Ok(0);
    }
    if args.iter().any(|a| a == "--self-test") {
        self_test()?;
        return Ok(0);
    }
    let parsed = parse_args(args)?;
    let selection = OwnerSelection { overlay: parsed.overlay.clone(), start: parsed.start };
    let mut owners = load_owners(&selection)?;
    if let Some(overlay) = &parsed.overlay {
        owners.retain(|owner| &owner.overlay == overlay);
    }
    if let Some(start) = parsed.start {
        owners.retain(|owner| owner.offset == start);
    }
    if let Some(end) = parsed.end {
        owners.retain(|owner| owner.offset + owner.span_bytes == end);
    }
    if owners.is_empty() {
        return Err("no semantic owners match the requested selection".to_string());
    }
    if let (Some(_start), Some(end)) = (parsed.start, parsed.end) {
        for owner in &owners {
            if owner.offset + owner.span_bytes != end {
                return Err(format!(
                    "{} has measured end 0x{:x}, not 0x{end:x}",
                    format_owner(owner),
                    owner.offset + owner.span_bytes
                ));
            }
        }
    }
    audit_owners(&owners, parsed.json)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn full_self_test() {
        self_test().expect("self-test must pass");
    }
}
