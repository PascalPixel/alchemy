pub mod cli;

use serde_json::{json, Value};
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};

const ROM_BASE: u64 = 0x0800_0000;
const ROM_SIZE: usize = 0x80_0000;

#[derive(Debug)]
struct Region {
    start: u64,
    end: u64,
    kind: String,
    confidence: String,
    evidence: String,
}

#[derive(Debug)]
pub struct Audit {
    pub executable: usize,
    pub exact: usize,
    pub retained: usize,
    pub kinds: BTreeMap<(String, String), (usize, usize)>,
}

pub fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR")).parent().and_then(Path::parent).expect("audit is under tools").to_path_buf()
}

fn document(path: &Path) -> Result<Value, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn integer(value: &Value, label: &str) -> Result<u64, String> {
    if let Some(number) = value.as_u64() {
        return Ok(number);
    }
    if let Some(text) = value.as_str() {
        let parsed = text.strip_prefix("0x").map_or_else(|| text.parse(), |hex| u64::from_str_radix(hex, 16));
        return parsed.map_err(|_| format!("{label} must be an integer"));
    }
    Err(format!("{label} must be an integer"))
}

fn span(value: &Value, label: &str) -> Result<(u64, u64), String> {
    let start = integer(value.get("start").or_else(|| value.get("address")).ok_or_else(|| format!("{label} has no start or address"))?, &format!("{label}.start"))?;
    let end = if let Some(end) = value.get("end") { integer(end, &format!("{label}.end"))? } else { start.checked_add(integer(value.get("size").ok_or_else(|| format!("{label} has no size"))?, &format!("{label}.size"))?).ok_or_else(|| format!("{label} overflows"))? };
    if end <= start || start < ROM_BASE || end > ROM_BASE + ROM_SIZE as u64 {
        return Err(format!("{label} is empty or outside the ROM"));
    }
    Ok((start, end))
}

fn values<'a>(document: &'a Value, path: &[&str]) -> Result<&'a Vec<Value>, String> {
    let mut value = document;
    for key in path {
        value = value.get(*key).ok_or_else(|| format!("missing {}", path.join(".")))?;
    }
    value.as_array().ok_or_else(|| format!("{} is not an array", path.join(".")))
}

fn regions(document: &Value, assembly: bool) -> Result<Vec<Region>, String> {
    values(document, &["regions"])?
        .iter()
        .enumerate()
        .map(|(index, value)| {
            let (start, end) = span(value, &format!("region {index}"))?;
            let text = |key: &str| value.get(key).and_then(Value::as_str).unwrap_or("").to_string();
            let retention = text("retention");
            let region = Region { start, end, kind: text("kind"), confidence: text("confidence"), evidence: text("evidence") };
            if assembly && (region.kind.is_empty() || region.confidence.is_empty() || region.evidence.is_empty() || retention.is_empty()) {
                return Err(format!("assembly region at 0x{start:08x} lacks retained classification evidence"));
            }
            Ok(region)
        })
        .collect()
}

fn mark_executable(mask: &mut [u8], start: u64, end: u64) {
    let start = (start - ROM_BASE) as usize;
    let end = (end - ROM_BASE) as usize;
    mask[start..end].iter_mut().for_each(|byte| *byte |= 1);
}

fn mark_source(mask: &mut [u8], region: &Region, bit: u8, label: &str) -> Result<(), String> {
    let start = (region.start - ROM_BASE) as usize;
    let end = (region.end - ROM_BASE) as usize;
    for (offset, byte) in mask[start..end].iter_mut().enumerate() {
        if *byte & 1 == 0 {
            return Err(format!("{label} at 0x{:08x} lies outside the executable inventory", region.start + offset as u64));
        }
        if *byte & 6 != 0 {
            return Err(format!("source regions overlap at 0x{:08x}", region.start + offset as u64));
        }
        *byte |= bit;
    }
    Ok(())
}

fn stale(output: &Path, source: &Path) -> Result<(), String> {
    let built = std::fs::metadata(output).and_then(|metadata| metadata.modified()).map_err(|error| format!("{}: {error}", output.display()))?;
    let edited = std::fs::metadata(source).and_then(|metadata| metadata.modified()).map_err(|error| format!("{}: {error}", source.display()))?;
    if built < edited {
        Err(format!("{} is stale; run make build-full", output.display()))
    } else {
        Ok(())
    }
}

pub fn audit(root: &Path) -> Result<Audit, String> {
    let inventory_path = root.join("games/gs1/metrics/gs1-en-executable.json");
    let asm_path = root.join("out/gs1-en/full/asm/manifest.json");
    let claimed_path = root.join("out/gs1-en/full/claimed/manifest.json");
    stale(&asm_path, &root.join("games/gs1/asm/classification.json"))?;

    let inventory = document(&inventory_path)?;
    let asm = regions(&document(&asm_path)?, true)?;
    let claimed = regions(&document(&claimed_path)?, false)?;
    if asm.is_empty() || claimed.is_empty() {
        return Err("retained audit read an empty build manifest".into());
    }

    let mut mask = vec![0u8; ROM_SIZE];
    for (index, value) in values(&inventory, &["main", "intervals"])?.iter().enumerate() {
        let (start, end) = span(value, &format!("executable interval {index}"))?;
        mark_executable(&mut mask, start, end);
    }
    for region in &claimed {
        mark_source(&mut mask, region, 2, "exact C")?;
    }
    for region in &asm {
        mark_source(&mut mask, region, 4, "assembly")?;
    }

    let executable = mask.iter().filter(|byte| **byte & 1 != 0).count();
    let exact = mask.iter().filter(|byte| **byte & 2 != 0).count();
    let retained = mask.iter().filter(|byte| **byte & 4 != 0).count();
    let uncovered = mask.iter().filter(|byte| **byte == 1).count();
    if executable == 0 || uncovered != 0 || exact + retained != executable {
        return Err(format!("retained complement differs: executable={executable} exact={exact} retained={retained} uncovered={uncovered}"));
    }

    let mut kinds = BTreeMap::new();
    for region in asm {
        let row = kinds.entry((region.kind, region.confidence)).or_insert((0usize, 0usize));
        row.0 += 1;
        row.1 += (region.end - region.start) as usize;
    }
    Ok(Audit { executable, exact, retained, kinds })
}

impl Audit {
    pub fn json(&self) -> Value {
        let kinds = self.kinds.iter().map(|((kind, confidence), (regions, bytes))| json!({"kind":kind,"confidence":confidence,"regions":regions,"bytes":bytes})).collect::<Vec<_>>();
        json!({
            "format": 1,
            "kind": "core-retained-complement-audit",
            "status": "ok",
            "bytes": {"executable":self.executable,"exact_c":self.exact,"retained":self.retained},
            "retained_by_kind_confidence": kinds,
            "failures": []
        })
    }
}
