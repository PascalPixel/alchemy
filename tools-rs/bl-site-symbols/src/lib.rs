//! Native implementation of tools/assets/bl_site_symbols.ts.
//!
//! A Thumb bl in an overlay stores a displacement that is not the same value
//! the assembler expects for the symbol. Decode the two halfwords at the call
//! site and name the symbol at the address the assembler needs.

use std::fs;
use std::path::{Path, PathBuf};

use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};
use serde_json::Value;

pub type Result<T> = std::result::Result<T, String>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct CallSite {
    pub site: i64,
    pub symbol: i64,
}

/// Decode every Thumb bl in [start, start + span).
pub fn bl_site_symbols(image: &[u8], start: i64, span: i64, base: i64) -> Vec<CallSite> {
    let Some(end) = start.checked_add(span) else {
        return Vec::new();
    };
    let mut sites = Vec::new();
    let mut address = start;
    while address.checked_add(3).is_some_and(|last| last < end) {
        let offset = address - base;
        if offset >= 0 {
            let at = offset as usize;
            if at + 3 < image.len() {
                let high = u16::from_le_bytes([image[at], image[at + 1]]);
                let low = u16::from_le_bytes([image[at + 2], image[at + 3]]);
                if (high & 0xf800) == 0xf000 && (low & 0xf800) == 0xf800 {
                    let packed = (((high & 0x07ff) as i64) << 12) | (((low & 0x07ff) as i64) << 1);
                    let displacement = if packed & 0x40_0000 != 0 {
                        packed - 0x80_0000
                    } else {
                        packed
                    };
                    sites.push(CallSite {
                        site: address,
                        symbol: address + 4 + displacement,
                    });
                }
            }
        }
        address += 2;
    }
    sites
}

pub fn symbol_name(address: i64) -> String {
    format!("Func_{address:08x}")
}

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate directory has a parent")
        .parent()
        .expect("tools-rs has a parent")
        .to_path_buf()
}

fn json_file(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

fn number(value: Option<&Value>) -> Option<i64> {
    value
        .and_then(Value::as_i64)
        .or_else(|| value.and_then(Value::as_f64).map(|number| number as i64))
}

fn parse_hex(text: &str) -> Option<i64> {
    let text = text
        .strip_prefix("0x")
        .or_else(|| text.strip_prefix("0X"))
        .unwrap_or(text);
    if text.is_empty() || !text.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return None;
    }
    i64::from_str_radix(text, 16).ok()
}

fn parse_span(text: &str) -> Option<i64> {
    if text.starts_with("0x") || text.starts_with("0X") {
        parse_hex(text)
    } else {
        text.parse().ok()
    }
}

struct SpanSources {
    inventory: Option<Value>,
    regions: Option<Value>,
}

impl SpanSources {
    fn load(root: &Path) -> Result<Self> {
        let inventory_path = root.join("out/decomp/overlays.json");
        let inventory = if inventory_path.exists() {
            Some(json_file(&inventory_path)?)
        } else {
            None
        };
        let regions_path = root.join("semantic/regions.json");
        let regions = if regions_path.exists() {
            Some(json_file(&regions_path)?)
        } else {
            None
        };
        Ok(Self { inventory, regions })
    }

    fn resolve(&self, overlay: &str, offset: i64) -> Result<Option<i64>> {
        if let Some(inventory) = &self.inventory {
            let functions = inventory
                .get("functions")
                .and_then(Value::as_array)
                .ok_or("overlays.json: missing functions array")?;
            let id = format!("{overlay}:{offset:x}");
            let padded_id = format!("{overlay}:{offset:04x}");
            if let Some(span) = functions.iter().find_map(|item| {
                (item.get("id").and_then(Value::as_str) == Some(id.as_str())
                    || item.get("id").and_then(Value::as_str) == Some(padded_id.as_str()))
                .then(|| number(item.get("span_bytes")))
                .flatten()
            }) {
                return Ok(Some(span));
            }
        }

        let Some(regions) = &self.regions else {
            return Ok(None);
        };
        let manual_regions = regions
            .get("manual_regions")
            .and_then(Value::as_array)
            .ok_or("regions.json: missing manual_regions array")?;
        let entry = format!("0x{:08x}", OVERLAY_BASE + offset);
        Ok(manual_regions.iter().find_map(|item| {
            (item.get("overlay").and_then(Value::as_str) == Some(overlay)
                && item.get("entry").and_then(Value::as_str) == Some(entry.as_str()))
            .then(|| number(item.get("span_bytes")))
            .flatten()
        }))
    }
}

/// Resolve a span from the native overlay inventory, with the same manual
/// region fallback used by the TypeScript implementation. Native inventory
/// rows use four hexadecimal digits; accepting the unpadded spelling as well
/// keeps this reader compatible with older generated inventories.
pub fn resolve_span(root: &Path, overlay: &str, offset: i64) -> Result<Option<i64>> {
    SpanSources::load(root)?.resolve(overlay, offset)
}

fn assemble_for(root: &Path, overlay: &str) -> Result<Vec<u8>> {
    let path = root
        .join("assets/code")
        .join(format!("{overlay}_overlay.s"));
    assemble_overlay(&OverlaySource::path(&path), OVERLAY_BASE)
        .map_err(|error| format!("{}: {error}", path.display()))
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct ScanRow {
    id: String,
    span: i64,
    sites: usize,
    named: usize,
}

fn semantic_owner(name: &str) -> Option<(String, i64)> {
    let lower = name.to_ascii_lowercase();
    let stem = lower.strip_suffix(".c")?;
    let rest = stem.strip_prefix("resource_")?;
    let (overlay_hex, address_hex) = rest.split_once("_c_")?;
    if overlay_hex.is_empty()
        || address_hex.len() != 8
        || !overlay_hex.bytes().all(|byte| byte.is_ascii_hexdigit())
        || !address_hex.bytes().all(|byte| byte.is_ascii_hexdigit())
    {
        return None;
    }
    let overlay = format!("resource_{overlay_hex}");
    let address = i64::from_str_radix(address_hex, 16).ok()?;
    Some((overlay, address - OVERLAY_BASE))
}

pub fn scan(root: &Path) -> Result<String> {
    let semantic_dir = root.join("semantic");
    let mut names: Vec<String> = fs::read_dir(&semantic_dir)
        .map_err(|error| format!("{}: {error}", semantic_dir.display()))?
        .filter_map(|entry| entry.ok())
        .filter_map(|entry| entry.file_name().into_string().ok())
        .collect();
    names.sort();

    let span_sources = SpanSources::load(root)?;
    let mut rows = Vec::new();
    let mut image_cache: Vec<(String, Vec<u8>)> = Vec::new();
    for name in names {
        let Some((overlay, offset)) = semantic_owner(&name) else {
            continue;
        };
        let Some(span) = span_sources.resolve(&overlay, offset)? else {
            continue;
        };
        let image = if let Some((_, image)) = image_cache.iter().find(|(key, _)| key == &overlay) {
            image.clone()
        } else {
            let Ok(image) = assemble_for(root, &overlay) else {
                continue;
            };
            image_cache.push((overlay.clone(), image.clone()));
            image
        };
        let sites = bl_site_symbols(&image, OVERLAY_BASE + offset, span, OVERLAY_BASE);
        if sites.is_empty() {
            continue;
        }
        let source_path = semantic_dir.join(&name);
        let source = fs::read_to_string(&source_path)
            .map_err(|error| format!("{}: {error}", source_path.display()))?;
        let named = sites
            .iter()
            .filter(|site| source.contains(&symbol_name(site.symbol)))
            .count();
        if named < sites.len() {
            rows.push(ScanRow {
                id: format!("{overlay}:{offset:x}"),
                span,
                sites: sites.len(),
                named,
            });
        }
    }

    rows.sort_by(|left, right| right.span.cmp(&left.span));
    let bytes: i64 = rows.iter().map(|row| row.span).sum();
    let mut output = format!(
        "owners not naming every call site at its decoded address: {} ({} span bytes)\n",
        rows.len(),
        bytes
    );
    for row in rows.iter().take(25) {
        output.push_str(&format!(
            "  {:<20} span={:>5} sites={} already_named={}\n",
            row.id, row.span, row.sites, row.named
        ));
    }
    output.push_str(
        "a bl-displacement residual is only PART of most of these; the span is an upper bound.\n",
    );
    Ok(output)
}

pub fn self_test() -> Result<String> {
    let base = 0x0200_0000i64;
    let mut image = vec![0u8; 0x40];
    let site = base + 0x10;
    let target = base + 0x30;
    encode_bl(&mut image, site - base, target - (site + 4));
    let found = bl_site_symbols(&image, base, 0x40, base);
    if found
        != [CallSite {
            site,
            symbol: target,
        }]
    {
        return Err(format!("forward fixture differs: {found:?}"));
    }
    if symbol_name(target) != "Func_02000030" {
        return Err("symbol name fixture differs".into());
    }

    let back_site = base + 0x20;
    let back_target = base + 4;
    let mut back_image = vec![0u8; 0x40];
    encode_bl(
        &mut back_image,
        back_site - base,
        back_target - (back_site + 4),
    );
    let back = bl_site_symbols(&back_image, base, 0x40, base);
    if back.len() != 1 || back[0].symbol != back_target {
        return Err(format!("backward fixture differs: {back:?}"));
    }
    Ok("self-test=ok tool=bl_site_symbols".into())
}

fn encode_bl(image: &mut [u8], site: i64, displacement: i64) {
    let high = 0xf000 | (((displacement >> 12) as u16) & 0x07ff);
    let low = 0xf800 | (((displacement >> 1) as u16) & 0x07ff);
    let at = site as usize;
    image[at..at + 2].copy_from_slice(&high.to_le_bytes());
    image[at + 2..at + 4].copy_from_slice(&low.to_le_bytes());
}

pub fn run(args: &[String]) -> Result<String> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    if args.iter().any(|arg| arg == "--scan") {
        return scan(&root());
    }

    let id = args.first().cloned().unwrap_or_default();
    let Some((overlay, offset_text)) = id.split_once(':') else {
        return Err("usage: bl_site_symbols.ts <overlay:offsetHex> [--span BYTES]".into());
    };
    if !overlay
        .strip_prefix("resource_")
        .is_some_and(|rest| !rest.is_empty() && rest.bytes().all(|byte| byte.is_ascii_hexdigit()))
    {
        return Err("usage: bl_site_symbols.ts <overlay:offsetHex> [--span BYTES]".into());
    }
    let offset = parse_hex(offset_text).ok_or_else(|| {
        "usage: bl_site_symbols.ts <overlay:offsetHex> [--span BYTES]".to_string()
    })?;
    let span_argument = args.iter().position(|arg| arg == "--span");
    let span = match span_argument {
        Some(index) => {
            let text = args
                .get(index + 1)
                .ok_or_else(|| "missing value for --span".to_string())?;
            parse_span(text).ok_or_else(|| "span must be a number".to_string())?
        }
        None => resolve_span(&root(), overlay, offset)?
            .ok_or_else(|| format!("no registered span for {id}; pass --span BYTES"))?,
    };
    let image = assemble_for(&root(), overlay)?;
    let sites = bl_site_symbols(&image, OVERLAY_BASE + offset, span, OVERLAY_BASE);
    let mut output = format!("{id} span={span} call_sites={}\n", sites.len());
    for site in sites {
        output.push_str(&format!(
            "  0x{:x} -> {}\n",
            site.site,
            symbol_name(site.symbol)
        ));
    }
    output.push_str("name the i-th call in source order after the i-th site above;\n");
    output.push_str("a repeated callee still needs a DIFFERENT name at each of its sites.\n");
    Ok(output)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::time::{SystemTime, UNIX_EPOCH};

    #[test]
    fn decodes_forward_and_backward_sites() {
        let base = 0x0200_0000i64;
        let mut image = vec![0u8; 0x40];
        encode_bl(&mut image, 0x10, 0x20 - 0x10 - 4);
        encode_bl(&mut image, 0x20, 4 - 0x20 - 4);
        assert_eq!(
            bl_site_symbols(&image, base, 0x40, base),
            vec![
                CallSite {
                    site: base + 0x10,
                    symbol: base + 0x20
                },
                CallSite {
                    site: base + 0x20,
                    symbol: base + 4
                },
            ]
        );
    }

    #[test]
    fn span_bounds_exclude_a_call_at_the_end() {
        let base = 0x0200_0000i64;
        let mut image = vec![0u8; 0x20];
        encode_bl(&mut image, 0x0c, 0);
        assert_eq!(bl_site_symbols(&image, base, 0x0f, base).len(), 0);
        assert_eq!(bl_site_symbols(&image, base, 0x10, base).len(), 1);
    }

    #[test]
    fn inventory_has_priority_over_manual_regions() {
        let stamp = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("clock")
            .as_nanos();
        let root = std::env::temp_dir().join(format!("bl-site-symbols-test-{stamp}"));
        fs::create_dir_all(root.join("out/decomp")).expect("inventory directory");
        fs::create_dir_all(root.join("semantic")).expect("semantic directory");
        fs::write(
            root.join("out/decomp/overlays.json"),
            r#"{"functions":[{"id":"resource_382:3ac","span_bytes":148}]}"#,
        )
        .expect("inventory");
        fs::write(
            root.join("semantic/regions.json"),
            r#"{"manual_regions":[{"overlay":"resource_382","entry":"0x020003ac","span_bytes":12}]}"#,
        )
        .expect("regions");
        assert_eq!(
            resolve_span(&root, "resource_382", 0x3ac).unwrap(),
            Some(148)
        );
        fs::remove_dir_all(root).expect("temporary fixture cleanup");
    }

    #[test]
    fn self_test_output_is_stable() {
        assert_eq!(self_test().unwrap(), "self-test=ok tool=bl_site_symbols");
    }
}
