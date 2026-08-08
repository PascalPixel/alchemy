//! `tools/overlay/overlay_entry.ts`, in Rust.
//!
//! Recovers the true entry of an overlay function that discovery seeded
//! partway into its body, by walking back to the previous function's return.
//! See the TypeScript original's header comment for the full rationale; it is
//! reproduced there, not duplicated here.
//!
//! The TypeScript original is retained until this crate is wired into
//! `tools/overlay/index.ts` and verified byte-identical on stdout, stderr and
//! exit code.

use std::path::PathBuf;

use exact_reading_list::json::{parse as parse_json, Value};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};

/// `bx rN`. The reconstruction's epilogues end `pop {rN}` / `bx rN`, and leaf
/// functions end `bx lr`; both terminate on this encoding.
const BX_MASK: u16 = 0xff87;
const BX: u16 = 0x4700;
const PROLOGUE_MASK: u16 = 0xff00;
const PROLOGUE: u16 = 0xb500;
/// `mov r8, r8` (the canonical Thumb nop) and zero padding sit between a
/// return and the next entry; so do the previous function's pool words, which
/// is why the walk forward stops at the first halfword that is neither.
fn is_padding(half: u16) -> bool {
    half == 0x0000 || half == 0x46c0
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Recovered {
    pub entry: i64,
    pub moved_by: i64,
    pub starts_with_prologue: bool,
}

fn read_u16le(data: &[u8], at: i64) -> u16 {
    if at < 0 {
        return 0;
    }
    let at = at as usize;
    if at + 2 > data.len() {
        return 0;
    }
    u16::from_le_bytes([data[at], data[at + 1]])
}

/// Walk backward from `offset` to the nearest return and treat the halfword
/// just after it as the true entry, skipping padding in between. `None` when
/// no return is found within `window`.
pub fn recover_entry(data: &[u8], offset: i64, window: i64) -> Option<Recovered> {
    // The seed itself can land outside the body, in the previous function's
    // pool word. ARMv4T has no cbz/cbnz, so that encoding range (0xb100-
    // 0xbb7f minus the 0xb500 family) can never be code. In that case, scan
    // forward for the entry instead of backward.
    let seed = if offset + 2 <= data.len() as i64 {
        read_u16le(data, offset)
    } else {
        0
    };
    let impossible_on_armv4t = (seed & 0xf500) == 0xb100 && (seed & 0xff00) != 0xb500;
    if impossible_on_armv4t {
        let mut scan = (offset + 3) & !3;
        let end = std::cmp::min(data.len() as i64, offset + window);
        while scan < end {
            if (read_u16le(data, scan) & PROLOGUE_MASK) == PROLOGUE {
                return Some(Recovered {
                    entry: scan,
                    moved_by: offset - scan,
                    starts_with_prologue: true,
                });
            }
            scan += 4;
        }
        return None;
    }
    let floor = std::cmp::max(0, offset - window);
    let mut at = offset - 2;
    while at >= floor {
        let half = read_u16le(data, at);
        if (half & BX_MASK) != BX {
            at -= 2;
            continue;
        }
        let mut entry = at + 2;
        while entry + 2 <= data.len() as i64 && is_padding(read_u16le(data, entry)) {
            entry += 2;
        }
        // The halfword right after a return is not always the next entry: the
        // previous function's pool word can sit in between. If a word-aligned
        // prologue is found before `offset`, take that; otherwise take the
        // halfword right after the padding as-is -- leaf functions have no
        // prologue, so that is correct.
        if (read_u16le(data, entry) & PROLOGUE_MASK) != PROLOGUE {
            let mut scan = (entry + 3) & !3;
            while scan < offset {
                if (read_u16le(data, scan) & PROLOGUE_MASK) == PROLOGUE {
                    entry = scan;
                    break;
                }
                scan += 4;
            }
        }
        if entry > offset {
            return None;
        }
        return Some(Recovered {
            entry,
            moved_by: offset - entry,
            starts_with_prologue: (read_u16le(data, entry) & PROLOGUE_MASK) == PROLOGUE,
        });
    }
    None
}

pub fn self_test() -> Result<String, String> {
    // Previous function's return / padding / entry / a decodable body / return.
    let halves: [u16; 10] = [
        0x2000, 0x4708, 0x0000, 0xb500, 0x2001, 0x2002, 0x2003, 0x2004, 0x2005, 0x4770,
    ];
    let mut data = Vec::with_capacity(halves.len() * 2);
    for half in halves {
        data.extend_from_slice(&half.to_le_bytes());
    }
    // Called mid-body (0x0a); must cross the padding and return 0x06.
    let found = recover_entry(&data, 10, 512).ok_or("overlay entry self-test: wrong entry")?;
    if found.entry != 6 {
        return Err("overlay entry self-test: wrong entry".to_string());
    }
    if found.moved_by != 4 || !found.starts_with_prologue {
        return Err("overlay entry self-test: wrong metadata".to_string());
    }
    // No return upstream means no claimed entry.
    if recover_entry(&[0u8; 8], 6, 512).is_some() {
        return Err("overlay entry self-test: must not invent an entry".to_string());
    }
    Ok("overlay entry self-test passed".to_string())
}

fn root() -> PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn load_overlay(overlay: &str) -> Result<Vec<u8>, String> {
    let path = root().join("assets/code").join(format!("{overlay}_overlay.s"));
    assemble_overlay(&OverlaySource::path(&path), OVERLAY_BASE)
}

/// The stem of an `exact/*_c_HHHHHHHH.c` filename, or `None` if it does not
/// match `/_c_([0-9a-f]{8})\.c$/`.
fn exact_stem(name: &str) -> Option<i64> {
    let suffix = name.strip_suffix(".c")?;
    let (before, digits) = suffix.rsplit_once("_c_")?;
    let _ = before;
    if digits.len() != 8 || !digits.bytes().all(|b| b.is_ascii_hexdigit() && !b.is_ascii_uppercase()) {
        return None;
    }
    i64::from_str_radix(digits, 16).ok()
}

pub fn run(argv: &[String]) -> Result<Vec<String>, String> {
    if argv.iter().any(|a| a == "--self-test") {
        return Ok(vec![self_test()?]);
    }
    let all = argv.iter().any(|a| a == "--all");
    let positional: Vec<&String> = argv.iter().filter(|a| !a.starts_with("--")).collect();

    if !all {
        let overlay = positional.first().copied();
        let offset_text = positional.get(1).copied();
        let (overlay, offset_text) = match (overlay, offset_text) {
            (Some(o), Some(t)) => (o, t),
            _ => return Err("usage: overlay_entry.ts <overlay> <offsetHex> | --all".to_string()),
        };
        let offset = i64::from_str_radix(offset_text.trim_start_matches("0x"), 16)
            .map_err(|_| format!("invalid offset: {offset_text}"))?;
        let data = load_overlay(overlay)?;
        return Ok(vec![match recover_entry(&data, offset, 512) {
            None => format!(
                "{overlay}:{offset_text} no return found within the window; entry unchanged"
            ),
            Some(found) => format!(
                "{overlay}:{offset_text} -> {:04x} (back {} bytes, prologue={})",
                found.entry, found.moved_by, found.starts_with_prologue
            ),
        }]);
    }

    let mut out = Vec::new();
    let inventory_text = std::fs::read_to_string(root().join("out/decomp/overlays.json"))
        .map_err(|error| error.to_string())?;
    let inventory = parse_json(&inventory_text)?;
    let functions = inventory
        .get("functions")
        .and_then(Value::as_array)
        .ok_or("overlays.json: missing functions array")?;

    let mut converted = std::collections::HashSet::new();
    for entry in std::fs::read_dir(root().join("exact")).map_err(|error| error.to_string())? {
        let entry = entry.map_err(|error| error.to_string())?;
        if let Some(name) = entry.file_name().to_str() {
            if let Some(stem) = exact_stem(name) {
                converted.insert(stem);
            }
        }
    }

    let mut cache: std::collections::HashMap<String, Vec<u8>> = std::collections::HashMap::new();
    let mut examined = 0i64;
    let mut moved = 0i64;
    for function in functions {
        let structural_veneer = function.get("structural_veneer").map(Value::truthy).unwrap_or(false);
        let data_walk = function.get("data_walk").map(Value::truthy).unwrap_or(false);
        let code_bytes = function.get("code_bytes").and_then(Value::as_f64).unwrap_or(0.0);
        if structural_veneer || data_walk || code_bytes < 8.0 {
            continue;
        }
        let entry_addr = function.get("entry").and_then(Value::as_f64).unwrap_or(f64::NAN) as i64;
        let contained_by_len = function
            .get("contained_by")
            .and_then(Value::as_array)
            .map(<[Value]>::len)
            .unwrap_or(0);
        if converted.contains(&entry_addr) || contained_by_len > 0 {
            continue;
        }
        let overlay = function.get("overlay").and_then(Value::as_str).unwrap_or("");
        let offset = function.get("offset").and_then(Value::as_f64).unwrap_or(f64::NAN) as i64;
        let id = function.get("id").and_then(Value::as_str).unwrap_or("");
        if !cache.contains_key(overlay) {
            cache.insert(overlay.to_string(), load_overlay(overlay)?);
        }
        let data = &cache[overlay];
        if (read_u16le(data, offset) & PROLOGUE_MASK) == PROLOGUE {
            continue;
        }
        examined += 1;
        let found = match recover_entry(data, offset, 512) {
            Some(found) if found.moved_by != 0 => found,
            _ => continue,
        };
        moved += 1;
        out.push(format!(
            "  {id} -> {overlay}:{:04x} back={} prologue={}",
            found.entry, found.moved_by, found.starts_with_prologue
        ));
    }
    out.push(format!("examined={examined} recovered={moved}"));
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_self_test_passes() {
        assert_eq!(self_test().unwrap(), "overlay entry self-test passed");
        assert_eq!(run(&["--self-test".to_string()]).unwrap().len(), 1);
    }

    #[test]
    fn exact_stem_matches_only_lowercase_hex() {
        assert_eq!(exact_stem("src_c_0809a44c.c"), Some(0x0809a44c));
        assert_eq!(exact_stem("src_c_0809A44C.c"), None);
        assert_eq!(exact_stem("src_0809a44c.c"), None);
        assert_eq!(exact_stem("src_c_0809a44c.h"), None);
    }
}
