//! `tools/overlay/overlay_unindexed.ts`, in Rust.
//!
//! Finds overlay functions that exist in the ROM but appear in no index: for
//! every `bl`/`blx` call site in an overlay's disassembled image, resolve the
//! target with the overlay call rule (`target = stored + 2`), and if the
//! target's first halfword looks like a function prologue and that address
//! isn't in the known inventory and isn't already converted (has a
//! `semantic/` or `exact/` C file), report it as "found" -- with
//! `interior: true` if it falls strictly inside a known row's span.
//!
//! The TypeScript original is retained until this crate is wired into
//! `tools/overlay/index.ts` and verified byte-identical on stdout.

use std::collections::HashSet;
use std::fs;
use std::path::{Path, PathBuf};

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_call_targets::{overlay_image, target_offset};

/// `isPrologue(halfword)`.
pub fn is_prologue(halfword: u16) -> bool {
    (halfword & 0xfe00) == 0xb400
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Found {
    pub overlay: String,
    pub offset: i64,
    pub calls: i64,
    pub interior: bool,
}

struct Row {
    overlay: String,
    offset: i64,
    span_bytes: i64,
}

fn read_u16le(image: &[u8], at: usize) -> u16 {
    (image[at] as u16) | ((image[at + 1] as u16) << 8)
}

/// `scanImage(image, known, spans, converted, overlay)`.
///
/// `calls` iterates in first-seen order, matching a JS `Map`; the final
/// sort by descending call count uses a stable sort, matching `Array#sort`.
pub fn scan_image(
    image: &[u8],
    known: &HashSet<i64>,
    spans: &[(i64, i64)],
    converted: &HashSet<i64>,
    overlay: &str,
) -> Vec<Found> {
    let mut order: Vec<i64> = Vec::new();
    let mut calls: std::collections::HashMap<i64, i64> = std::collections::HashMap::new();
    let mut at: usize = 0;
    while at + 3 < image.len() {
        let high = read_u16le(image, at);
        let low = read_u16le(image, at + 2);
        if let Some(target) = target_offset(high, low) {
            if target >= 0 && (target as usize) + 1 < image.len() {
                if is_prologue(read_u16le(image, target as usize)) {
                    let entry = calls.entry(target).or_insert(0);
                    if *entry == 0 {
                        order.push(target);
                    }
                    *entry += 1;
                }
            }
        }
        at += 2;
    }
    let mut found: Vec<Found> = Vec::new();
    for offset in order {
        if known.contains(&offset) || converted.contains(&offset) {
            continue;
        }
        let count = calls[&offset];
        let interior = spans.iter().any(|(low, high)| offset > *low && offset < *high);
        found.push(Found { overlay: overlay.to_string(), offset, calls: count, interior });
    }
    found.sort_by(|left, right| right.calls.cmp(&left.calls));
    found
}

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn inventory(root: &Path) -> Result<Vec<Row>, String> {
    let path = root.join("out/decomp/overlays.json");
    let text = fs::read_to_string(&path)
        .map_err(|_| format!("missing {}; run the overlay inventory first", path.display()))?;
    let value = parse_json(&text)?;
    let functions = value
        .get("functions")
        .and_then(Value::as_array)
        .ok_or("overlays.json: missing functions array")?;
    let mut rows = Vec::with_capacity(functions.len());
    for function in functions {
        rows.push(Row {
            overlay: function.get("overlay").and_then(Value::as_str).unwrap_or("").to_string(),
            offset: function.get("offset").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            span_bytes: function.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
        });
    }
    Ok(rows)
}

fn dir_names(path: &Path) -> Result<HashSet<String>, String> {
    let mut names = HashSet::new();
    let entries = fs::read_dir(path).map_err(|error| format!("{}: {error}", path.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| error.to_string())?;
        if let Some(name) = entry.file_name().to_str() {
            names.insert(name.to_string());
        }
    }
    Ok(names)
}

/// `scanAll(only)`.
///
/// `byOverlay` iterates in first-seen insertion order, matching a JS `Map`.
pub fn scan_all(root: &Path, only: Option<&str>) -> Result<Vec<Found>, String> {
    let rows = inventory(root)?;
    let semantic = dir_names(&root.join("semantic"))?;
    let exact = dir_names(&root.join("exact"))?;

    let mut overlay_order: Vec<String> = Vec::new();
    let mut by_overlay: std::collections::HashMap<String, Vec<&Row>> = std::collections::HashMap::new();
    for row in &rows {
        if !by_overlay.contains_key(&row.overlay) {
            overlay_order.push(row.overlay.clone());
        }
        by_overlay.entry(row.overlay.clone()).or_default().push(row);
    }

    let mut found: Vec<Found> = Vec::new();
    for overlay in &overlay_order {
        if let Some(only) = only {
            if overlay != only {
                continue;
            }
        }
        let image = match overlay_image(overlay) {
            Ok(image) => image,
            Err(_) => continue,
        };
        let overlay_rows = &by_overlay[overlay];
        let mut converted: HashSet<i64> = HashSet::new();
        let mut offset = 0usize;
        while offset < image.len() {
            let address = format!("{:08x}", 0x02000000u64 + offset as u64);
            let name = format!("{overlay}_c_{address}.c");
            if semantic.contains(&name) || exact.contains(&name) {
                converted.insert(offset as i64);
            }
            offset += 2;
        }
        let known: HashSet<i64> = overlay_rows.iter().map(|row| row.offset).collect();
        let spans: Vec<(i64, i64)> =
            overlay_rows.iter().map(|row| (row.offset, row.offset + row.span_bytes)).collect();
        found.extend(scan_image(&image, &known, &spans, &converted, overlay));
    }
    Ok(found)
}

fn json_escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len());
    for ch in text.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out
}

/// `JSON.stringify(missing, null, 2)`.
pub fn render_json(missing: &[Found]) -> String {
    if missing.is_empty() {
        return "[]".to_string();
    }
    let mut out = String::from("[\n");
    for (index, entry) in missing.iter().enumerate() {
        out.push_str("  {\n");
        out.push_str(&format!("    \"overlay\": \"{}\",\n", json_escape(&entry.overlay)));
        out.push_str(&format!("    \"offset\": {},\n", entry.offset));
        out.push_str(&format!("    \"calls\": {},\n", entry.calls));
        out.push_str(&format!("    \"interior\": {}\n", entry.interior));
        out.push_str("  }");
        if index + 1 < missing.len() {
            out.push(',');
        }
        out.push('\n');
    }
    out.push(']');
    out
}

/// `/^resource_[0-9a-f]+$/`.
fn is_resource_name(text: &str) -> bool {
    text.strip_prefix("resource_")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|b| b.is_ascii_hexdigit()))
}

pub fn self_test() -> Result<(), String> {
    if !is_prologue(0xb5e0) {
        return Err("push {r5,r6,r7,lr} is a prologue".to_string());
    }
    if !is_prologue(0xb500) {
        return Err("push {lr} is a prologue".to_string());
    }
    if !is_prologue(0xb4f0) {
        return Err("a bare push is a prologue".to_string());
    }
    if is_prologue(0x4770) {
        return Err("bx lr is not a prologue".to_string());
    }
    if is_prologue(0xb084) {
        return Err("sub sp,#16 is not a prologue".to_string());
    }

    let mut image = vec![0u8; 0x40];
    image[0] = 0x00;
    image[1] = 0xf0;
    image[2] = 0x07;
    image[3] = 0xf8;
    if target_offset(0xf000, 0xf807) != Some(0x10) {
        return Err("the +2 rule must give 0x10".to_string());
    }
    image[0x10] = 0xe0;
    image[0x11] = 0xb5;

    let bare = scan_image(&image, &HashSet::new(), &[], &HashSet::new(), "");
    if bare.len() != 1 || bare[0].offset != 0x10 {
        return Err("an unindexed callee must be found".to_string());
    }
    if bare[0].calls != 1 {
        return Err("the call must be counted".to_string());
    }
    if bare[0].interior {
        return Err("with no spans nothing is interior".to_string());
    }

    let mut known = HashSet::new();
    known.insert(0x10i64);
    if !scan_image(&image, &known, &[], &HashSet::new(), "").is_empty() {
        return Err("a known row must be dropped".to_string());
    }
    let mut converted = HashSet::new();
    converted.insert(0x10i64);
    if !scan_image(&image, &HashSet::new(), &[], &converted, "").is_empty() {
        return Err("an already-converted address must be dropped".to_string());
    }

    let inside = scan_image(&image, &HashSet::new(), &[(0x08, 0x20)], &HashSet::new(), "");
    if !inside[0].interior {
        return Err("a target inside a span is interior".to_string());
    }
    let at_start = scan_image(&image, &HashSet::new(), &[(0x10, 0x20)], &HashSet::new(), "");
    if at_start[0].interior {
        return Err("a target AT a span start is that row, not interior".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let root = root();
    let only = args.iter().find(|argument| is_resource_name(argument)).map(String::as_str);
    let found = scan_all(&root, only)?;
    let missing: Vec<&Found> = found.iter().filter(|entry| !entry.interior).collect();
    let interior: Vec<&Found> = found.iter().filter(|entry| entry.interior).collect();

    if args.iter().any(|arg| arg == "--json") {
        let owned: Vec<Found> = missing.iter().map(|entry| (*entry).clone()).collect();
        println!("{}", render_json(&owned));
        return Ok(());
    }
    if let Some(_) = only {
        for entry in &missing {
            println!("  0x{:08x}  called {}x", 0x02000000i64 + entry.offset, entry.calls);
        }
    } else {
        let mut order: Vec<String> = Vec::new();
        let mut by_overlay: std::collections::HashMap<String, i64> = std::collections::HashMap::new();
        for entry in &missing {
            if !by_overlay.contains_key(&entry.overlay) {
                order.push(entry.overlay.clone());
            }
            *by_overlay.entry(entry.overlay.clone()).or_insert(0) += 1;
        }
        let mut pairs: Vec<(String, i64)> =
            order.into_iter().map(|overlay| (overlay.clone(), by_overlay[&overlay])).collect();
        pairs.sort_by(|left, right| right.1.cmp(&left.1));
        for (overlay, count) in pairs {
            println!("{overlay}  {count}");
        }
    }
    println!("\nunindexed_called_functions={} interior_already_covered={}", missing.len(), interior.len());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn sample_image() -> Vec<u8> {
        let mut image = vec![0u8; 0x40];
        image[0] = 0x00;
        image[1] = 0xf0;
        image[2] = 0x07;
        image[3] = 0xf8;
        image[0x10] = 0xe0;
        image[0x11] = 0xb5;
        image
    }

    #[test]
    fn prologue_shapes() {
        assert!(is_prologue(0xb5e0));
        assert!(is_prologue(0xb500));
        assert!(is_prologue(0xb4f0));
        assert!(!is_prologue(0x4770));
        assert!(!is_prologue(0xb084));
    }

    #[test]
    fn unindexed_callee_is_found_once() {
        let image = sample_image();
        let found = scan_image(&image, &HashSet::new(), &[], &HashSet::new(), "resource_x");
        assert_eq!(found.len(), 1);
        assert_eq!(found[0].offset, 0x10);
        assert_eq!(found[0].calls, 1);
        assert!(!found[0].interior);
        assert_eq!(found[0].overlay, "resource_x");
    }

    #[test]
    fn known_and_converted_rows_are_dropped() {
        let image = sample_image();
        let mut known = HashSet::new();
        known.insert(0x10i64);
        assert!(scan_image(&image, &known, &[], &HashSet::new(), "").is_empty());

        let mut converted = HashSet::new();
        converted.insert(0x10i64);
        assert!(scan_image(&image, &HashSet::new(), &[], &converted, "").is_empty());
    }

    #[test]
    fn interior_only_when_strictly_inside_a_span() {
        let image = sample_image();
        let inside = scan_image(&image, &HashSet::new(), &[(0x08, 0x20)], &HashSet::new(), "");
        assert!(inside[0].interior);
        let at_start = scan_image(&image, &HashSet::new(), &[(0x10, 0x20)], &HashSet::new(), "");
        assert!(!at_start[0].interior);
    }

    #[test]
    fn render_json_matches_stringify_pretty() {
        let entries = vec![Found { overlay: "resource_0".to_string(), offset: 16, calls: 2, interior: false }];
        let expected = "[\n  {\n    \"overlay\": \"resource_0\",\n    \"offset\": 16,\n    \"calls\": 2,\n    \"interior\": false\n  }\n]";
        assert_eq!(render_json(&entries), expected);
        assert_eq!(render_json(&[]), "[]");
    }

    #[test]
    fn self_test_passes() {
        assert!(self_test().is_ok());
    }
}
