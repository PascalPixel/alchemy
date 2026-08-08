//! `tools/overlay/overlay_twins.ts`, in Rust.
//!
//! Find owners that are the SAME routine shared verbatim across overlays.
//! See the TypeScript original's header comment for the full rationale; it is
//! reproduced there, not duplicated here.
//!
//! The TypeScript original is retained until this crate is wired into
//! `tools/overlay/index.ts` and verified byte-identical on stdout, stderr and
//! exit code.

use std::collections::HashSet;
use std::fs;
use std::path::{Path, PathBuf};

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_disasm::{assemble_overlay, overlay_c_spans, OverlaySource, OVERLAY_BASE};

const USAGE: &str = "usage: overlay-twins [resource_NNN] [--unconverted] [--semantic] [--self-test]";

/// `skeletonDigest(image, offset, size)`.
///
/// Digest an owner's instruction skeleton, masking what differs legitimately
/// between two copies of the same routine in different overlays: both halves
/// of every `bl` pair (the displacement encodes the callee's offset in *this*
/// overlay's image, and the veneer table sits at a different offset in each
/// overlay), and any halfword inside the span that is not reached as an
/// instruction (literal pools), approximated by masking words that decode as
/// pool-like.
pub fn skeleton_digest(image: &[u8], offset: i64, size: i64) -> String {
    let mut buffer: Vec<u8> = Vec::new();
    let end = ((offset + size) as usize).min(image.len());
    let offset = offset.max(0) as usize;
    let mut at = offset;
    while at + 1 < end {
        let halfword = (image[at] as u16) | ((image[at + 1] as u16) << 8);
        let is_bl_prefix = (halfword & 0xf800) == 0xf000;
        if is_bl_prefix
            && at + 3 < end
            && (((image[at + 2] as u16) | ((image[at + 3] as u16) << 8)) & 0xf800) == 0xf800
        {
            // Mask both halves of the pair, but record that a call happened here.
            buffer.extend_from_slice(b"CALL");
            at += 2;
            at += 2;
            continue;
        }
        if at & 3 == 0 && at + 3 < end {
            let word = (halfword as u32)
                | (((image[at + 2] as u32) | ((image[at + 3] as u32) << 8)) << 16);
            let region = (word >> 24) & 0xff;
            if region == 0x02 || region == 0x03 || region == 0x08 || region == 0x09 {
                buffer.extend_from_slice(b"PTR");
                at += 2;
                at += 2;
                continue;
            }
        }
        buffer.push((halfword & 0xff) as u8);
        buffer.push((halfword >> 8) as u8);
        at += 2;
    }
    alchemy_bundle::sha256::hex(&buffer)[..16].to_string()
}

fn read_json(path: &Path) -> Result<Value, String> {
    let bytes = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    parse_json(&text).map_err(|error| format!("{}: {error}", path.display()))
}

/// `inventory()`.
fn inventory(root: &Path) -> Result<Value, String> {
    let path = root.join("out").join("decomp").join("overlays.json");
    if !path.exists() {
        return Err(format!("missing {}", path.display()));
    }
    read_json(&path)
}

fn f64_field(row: &Value, key: &str) -> f64 {
    row.get(key).and_then(Value::as_f64).unwrap_or(0.0)
}

fn str_field(row: &Value, key: &str) -> String {
    row.get(key).and_then(Value::as_str).unwrap_or_default().to_string()
}

fn truthy_field(row: &Value, key: &str) -> bool {
    row.get(key).map(Value::truthy).unwrap_or(false)
}

fn contained_by_len(row: &Value) -> usize {
    row.get("contained_by").and_then(Value::as_array).map(<[Value]>::len).unwrap_or(0)
}

/// The filter `twinGroups` applies to every row (discovered or synthesized
/// from `exact/`): `starts_with_prologue`, `returns > 0`, not a structural
/// veneer, not a data walk, not contained by anything.
fn passes_filter(row: &Value) -> bool {
    truthy_field(row, "starts_with_prologue")
        && f64_field(row, "returns") > 0.0
        && !truthy_field(row, "structural_veneer")
        && !truthy_field(row, "data_walk")
        && contained_by_len(row) == 0
}

/// `convertedKeys(includeSemantic)`.
fn converted_keys(root: &Path, include_semantic: bool) -> HashSet<String> {
    let mut keys = HashSet::new();
    let exact_re = |name: &str| -> Option<(String, String)> {
        let prefix = "resource_";
        if !name.starts_with(prefix) || !name.ends_with(".c") {
            return None;
        }
        let mid = "_c_0200";
        let mid_at = name.find(mid)?;
        let overlay = &name[..mid_at];
        let hexpart = &name[mid_at + mid.len()..name.len() - 2];
        if hexpart.len() != 4 || !hexpart.chars().all(|c| c.is_ascii_hexdigit()) {
            return None;
        }
        let overlay_suffix = &overlay[prefix.len()..];
        if overlay_suffix.is_empty() || !overlay_suffix.chars().all(|c| c.is_ascii_hexdigit()) {
            return None;
        }
        Some((overlay.to_string(), hexpart.to_string()))
    };
    let scan = |dir: &Path, keys: &mut HashSet<String>| {
        if let Ok(entries) = fs::read_dir(dir) {
            for entry in entries.filter_map(Result::ok) {
                let name = entry.file_name().to_string_lossy().into_owned();
                if let Some((overlay, hexpart)) = exact_re(&name) {
                    keys.insert(format!("{overlay}:{hexpart}"));
                }
            }
        }
    };
    scan(&root.join("exact"), &mut keys);
    if include_semantic {
        scan(&root.join("semantic"), &mut keys);
    }
    keys
}

struct Row {
    overlay: String,
    offset: i64,
    span_bytes: i64,
}

/// `exactRows()`: exact-C owners recovered from the compiler-produced extents
/// the overlay builder already exposes, since exact-C owners disappear from
/// `out/decomp/overlays.json`.
fn exact_rows(root: &Path) -> Vec<Row> {
    let code = root.join("assets").join("code");
    let mut rows = Vec::new();
    let Ok(entries) = fs::read_dir(&code) else { return rows };
    let mut names: Vec<String> = entries
        .filter_map(Result::ok)
        .map(|entry| entry.file_name().to_string_lossy().into_owned())
        .filter(|name| {
            name.starts_with("resource_")
                && name.ends_with("_overlay.s")
                && name["resource_".len()..name.len() - "_overlay.s".len()]
                    .chars()
                    .all(|c| c.is_ascii_hexdigit())
                && !name["resource_".len()..name.len() - "_overlay.s".len()].is_empty()
        })
        .collect();
    names.sort();
    for name in names {
        let overlay = name.strip_suffix("_overlay.s").unwrap().to_string();
        let source = OverlaySource::path(code.join(&name));
        for span in overlay_c_spans(&source, OVERLAY_BASE) {
            rows.push(Row {
                overlay: overlay.clone(),
                offset: span.start,
                span_bytes: span.end - span.start,
            });
        }
    }
    rows
}

pub struct Twin {
    pub overlay: String,
    pub offset: i64,
    pub bytes: i64,
    pub converted: bool,
}

/// `twinGroups(includeSemantic)`.
pub fn twin_groups(root: &Path, include_semantic: bool) -> Result<Vec<Vec<Twin>>, String> {
    let inventory_value = inventory(root)?;
    let discovered: &[Value] = inventory_value.get("functions").and_then(Value::as_array).unwrap_or(&[]);
    let mut known: HashSet<String> = HashSet::new();
    for row in discovered {
        known.insert(format!("{}:{}", str_field(row, "overlay"), f64_field(row, "offset") as i64));
    }
    let extra_rows: Vec<Row> = exact_rows(root)
        .into_iter()
        .filter(|row| !known.contains(&format!("{}:{}", row.overlay, row.offset)))
        .collect();

    // Unified row view: (overlay, offset, span_bytes, passes-filter).
    struct View {
        overlay: String,
        offset: i64,
        span_bytes: i64,
    }
    let mut rows: Vec<View> = Vec::new();
    for row in discovered {
        if !passes_filter(row) {
            continue;
        }
        rows.push(View {
            overlay: str_field(row, "overlay"),
            offset: f64_field(row, "offset") as i64,
            span_bytes: f64_field(row, "span_bytes") as i64,
        });
    }
    for row in extra_rows {
        // Synthesized exact rows always pass the filter (starts_with_prologue
        // = true, returns = 1, structural_veneer = false, data_walk = false,
        // contained_by = []), same as the TS original.
        rows.push(View {
            overlay: row.overlay,
            offset: row.offset,
            span_bytes: row.span_bytes,
        });
    }

    let converted = converted_keys(root, include_semantic);
    let mut images: Vec<(String, Vec<u8>)> = Vec::new();
    let mut groups: Vec<(String, Vec<Twin>)> = Vec::new();

    for row in rows {
        let image_index = match images.iter().position(|(name, _)| *name == row.overlay) {
            Some(index) => index,
            None => {
                let source_path = root.join("assets").join("code").join(format!("{}_overlay.s", row.overlay));
                if !source_path.exists() {
                    continue;
                }
                // `assembleOverlay` throwing in the TS original is not caught
                // anywhere on this path either, so a bad overlay source aborts
                // the whole run here too, rather than silently dropping rows.
                let data = assemble_overlay(&OverlaySource::path(&source_path), OVERLAY_BASE)?;
                images.push((row.overlay.clone(), data));
                images.len() - 1
            }
        };
        let image = &images[image_index].1;
        if row.offset + row.span_bytes > image.len() as i64 {
            continue;
        }
        // Size is part of the key: two routines of different length are not twins.
        let key = format!("{}:{}", row.span_bytes, skeleton_digest(image, row.offset, row.span_bytes));
        let twin = Twin {
            overlay: row.overlay.clone(),
            offset: row.offset,
            bytes: row.span_bytes,
            converted: converted.contains(&format!("{}:{:04x}", row.overlay, row.offset)),
        };
        match groups.iter_mut().find(|(k, _)| *k == key) {
            Some((_, list)) => list.push(twin),
            None => groups.push((key, vec![twin])),
        }
    }

    let mut result: Vec<Vec<Twin>> = groups.into_iter().map(|(_, list)| list).filter(|g| g.len() > 1).collect();
    result.sort_by(|left, right| {
        let left_gain: i64 = left.iter().filter(|t| !t.converted).map(|t| t.bytes).sum();
        let right_gain: i64 = right.iter().filter(|t| !t.converted).map(|t| t.bytes).sum();
        right_gain.cmp(&left_gain)
    });
    Ok(result)
}

pub fn self_test() -> Result<(), String> {
    // Two images identical except for the BL displacement must digest the same.
    let left = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x10, 0xbd];
    let right = [0x10u8, 0xb5, 0x02, 0xf0, 0x75, 0xfe, 0x10, 0xbd];
    if skeleton_digest(&left, 0, 8) != skeleton_digest(&right, 0, 8) {
        return Err("BL displacement must be masked".to_string());
    }
    // A different instruction must NOT digest the same.
    let other = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x00, 0xbd];
    if skeleton_digest(&left, 0, 8) == skeleton_digest(&other, 0, 8) {
        return Err("a real instruction difference must survive".to_string());
    }
    // A call's position is significant: moving it changes the digest.
    let moved = [0x00u8, 0xf0, 0x02, 0xf8, 0x10, 0xb5, 0x10, 0xbd];
    if skeleton_digest(&left, 0, 8) == skeleton_digest(&moved, 0, 8) {
        return Err("call position must stay significant".to_string());
    }
    // Pointer-shaped pool words differing per overlay must be masked.
    let pool_a = [0x10u8, 0xb5, 0x00, 0x00, 0xf8, 0xae, 0x00, 0x02];
    let pool_b = [0x10u8, 0xb5, 0x00, 0x00, 0xf0, 0xb1, 0x00, 0x02];
    if skeleton_digest(&pool_a, 0, 8) != skeleton_digest(&pool_b, 0, 8) {
        return Err("pointer pool words must be masked".to_string());
    }
    println!("self-test=ok");
    Ok(())
}

/// `reportLeads(groups)`.
fn report_leads(groups: &[Vec<Twin>], out: &mut String) {
    struct Lead<'a> {
        group: &'a Vec<Twin>,
        unlock: i64,
    }
    let mut leads: Vec<Lead> = groups
        .iter()
        .filter(|group| group.iter().all(|twin| !twin.converted))
        .map(|group| Lead {
            group,
            unlock: (group.len() as i64 - 1) * group[0].bytes,
        })
        .collect();
    leads.sort_by(|left, right| right.unlock.cmp(&left.unlock));
    for lead in &leads {
        let first = &lead.group[0];
        out.push_str(&format!(
            "{:>6}B unlocked  {}x{}B  read {}:{} -> transpose {}\n",
            lead.unlock,
            lead.group.len(),
            first.bytes,
            first.overlay,
            format!("{:04x}", first.offset),
            lead.group.len() - 1,
        ));
    }
    let total: i64 = leads.iter().map(|lead| lead.unlock).sum();
    let reading: i64 = leads.iter().map(|lead| lead.group[0].bytes).sum();
    out.push_str(&format!("\nleads={} unlock_bytes={} reading_bytes={}\n", leads.len(), total, reading));
}

/// `main()`.
pub fn run(root: &Path, args: &[String]) -> Result<(), String> {
    if args.iter().any(|a| a == "--help" || a == "-h") {
        println!("{USAGE}");
        return Ok(());
    }
    if args.iter().any(|a| a == "--self-test") {
        return self_test();
    }
    let is_resource = |s: &str| {
        s.starts_with("resource_")
            && !s["resource_".len()..].is_empty()
            && s["resource_".len()..].chars().all(|c| c.is_ascii_hexdigit())
    };
    let known_flags = ["--unconverted", "--semantic", "--leads"];
    if let Some(unknown) = args.iter().find(|a| !is_resource(a) && !known_flags.contains(&a.as_str())) {
        return Err(format!("{USAGE}\nunrecognised argument: {unknown}"));
    }
    if args.iter().filter(|a| is_resource(a)).count() > 1 {
        return Err(format!("{USAGE}\nselect at most one overlay"));
    }
    let only = args.iter().find(|a| is_resource(a)).cloned();
    let unconverted_only = args.iter().any(|a| a == "--unconverted");
    let include_semantic = args.iter().any(|a| a == "--semantic");
    let mut groups = twin_groups(root, include_semantic)?;
    if let Some(only) = &only {
        groups.retain(|g| g.iter().any(|t| &t.overlay == only));
    }
    if args.iter().any(|a| a == "--leads") {
        let mut out = String::new();
        report_leads(&groups, &mut out);
        print!("{out}");
        return Ok(());
    }
    if unconverted_only {
        // A reusable family needs BOTH a source and an unconverted sibling.
        groups.retain(|group| group.iter().any(|t| t.converted) && group.iter().any(|t| !t.converted));
    }
    let mut recoverable: i64 = 0;
    for group in &groups {
        let done: Vec<&Twin> = group.iter().filter(|t| t.converted).collect();
        let todo: Vec<&Twin> = group.iter().filter(|t| !t.converted).collect();
        recoverable += todo.iter().map(|t| t.bytes).sum::<i64>();
        if let Some(first_done) = done.first() {
            println!(
                "{} bytes x{}  source: {}:{:x}",
                group[0].bytes,
                group.len(),
                first_done.overlay,
                first_done.offset
            );
        } else {
            println!("{} bytes x{}  (none converted yet)", group[0].bytes, group.len());
        }
        for twin in &todo {
            println!("    TODO {}:{:04x}", twin.overlay, twin.offset);
        }
    }
    println!(
        "\ngroups={} recoverable_bytes={} mode={}",
        groups.len(),
        recoverable,
        if include_semantic { "semantic" } else { "exact" }
    );
    Ok(())
}

pub fn root_from_exe(manifest_dir: &str) -> PathBuf {
    Path::new(manifest_dir)
        .parent()
        .and_then(Path::parent)
        .expect("crate sits two levels below the repository root")
        .to_path_buf()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn bl_displacement_is_masked() {
        let left = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x10, 0xbd];
        let right = [0x10u8, 0xb5, 0x02, 0xf0, 0x75, 0xfe, 0x10, 0xbd];
        assert_eq!(skeleton_digest(&left, 0, 8), skeleton_digest(&right, 0, 8));
    }

    #[test]
    fn real_instruction_difference_survives() {
        let left = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x10, 0xbd];
        let other = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x00, 0xbd];
        assert_ne!(skeleton_digest(&left, 0, 8), skeleton_digest(&other, 0, 8));
    }

    #[test]
    fn call_position_is_significant() {
        let left = [0x10u8, 0xb5, 0x00, 0xf0, 0x02, 0xf8, 0x10, 0xbd];
        let moved = [0x00u8, 0xf0, 0x02, 0xf8, 0x10, 0xb5, 0x10, 0xbd];
        assert_ne!(skeleton_digest(&left, 0, 8), skeleton_digest(&moved, 0, 8));
    }

    #[test]
    fn pointer_pool_words_are_masked() {
        let pool_a = [0x10u8, 0xb5, 0x00, 0x00, 0xf8, 0xae, 0x00, 0x02];
        let pool_b = [0x10u8, 0xb5, 0x00, 0x00, 0xf0, 0xb1, 0x00, 0x02];
        assert_eq!(skeleton_digest(&pool_a, 0, 8), skeleton_digest(&pool_b, 0, 8));
    }

    #[test]
    fn digest_is_a_16_char_hex_prefix() {
        let data = [0x10u8, 0xb5, 0x10, 0xbd];
        let digest = skeleton_digest(&data, 0, 4);
        assert_eq!(digest.len(), 16);
        assert!(digest.chars().all(|c| c.is_ascii_hexdigit()));
    }

    #[test]
    fn self_test_passes() {
        assert!(self_test().is_ok());
    }
}
