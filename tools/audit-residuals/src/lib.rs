// Source-only residual accounting for the English Golden Sun image.
//
// Ported from tools/metrics/audit_residuals.ts. This crate deliberately owns
// the complete implementation and has no runtime dependency on Bun or on a
// generated report: the four manifests and the full report are its inputs.

use canonical_json::canonical_json;
use serde::Serialize;
use serde_json::{Map, Value};
use std::fs;
use std::path::Path;

pub const ROM_BASE: u64 = 0x0800_0000;
pub const ROM_SIZE: u64 = 0x0080_0000;
pub const AUDIO_ADDRESS: u64 = 0x080f_b792;
pub const RESOURCE_ADDRESS: u64 = 0x0832_0000;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct Interval {
    pub address: u64,
    pub end: u64,
    pub size: u64,
}

#[derive(Clone, Debug, PartialEq, Serialize)]
pub struct BandReport {
    pub name: &'static str,
    pub address: String,
    pub end: String,
    pub unowned_bytes: u64,
    pub gaps: usize,
}

#[derive(Clone, Debug, PartialEq, Serialize)]
pub struct MissingResource {
    pub id: String,
    pub name: String,
    pub address: String,
    pub end: String,
    pub size: u64,
}

#[derive(Clone, Debug, PartialEq, Serialize)]
pub struct ResidualAudit {
    pub format: u64,
    pub kind: &'static str,
    pub verification: &'static str,
    pub rom_base: String,
    pub rom_size: u64,
    pub source_regions: usize,
    pub source_bytes: u64,
    pub unowned_bytes: u64,
    pub gaps: usize,
    pub project_completion_status: &'static str,
    pub project_completion_percent: Option<f64>,
    pub byte_reconstruction_bytes: u64,
    pub byte_reconstruction_remaining_bytes: u64,
    pub byte_reconstruction_percent: f64,
    pub total_decompilation_bytes: u64,
    pub total_decompilation_remaining_bytes: u64,
    pub total_decompilation_percent: f64,
    pub total_decompilation_semantics: &'static str,
    pub bands: Vec<BandReport>,
    pub missing_resources: Vec<MissingResource>,
    pub missing_resource_bytes: u64,
}

#[derive(Clone, Debug)]
pub struct Inputs {
    pub full: Value,
    pub fallback: Value,
    pub claimed: Value,
    pub assembly: Value,
    pub assets: Value,
    pub directory: Value,
}

#[derive(Clone, Debug)]
struct Options {
    full_report: String,
    fallback_manifest: Option<String>,
    claimed_manifest: String,
    asm_manifest: String,
    asset_manifest: String,
    resource_directory: String,
    json: bool,
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>, String> {
    match value {
        Value::Object(value) => Ok(value),
        _ => Err(format!("{label} must be an object")),
    }
}

fn field<'a>(value: &'a Map<String, Value>, name: &str) -> Option<&'a Value> {
    value.get(name)
}

fn strict_number(value: Option<&Value>, expected: f64) -> bool {
    matches!(value, Some(Value::Number(number)) if number.as_f64() == Some(expected))
}

fn strict_boolean(value: Option<&Value>, expected: bool) -> bool {
    matches!(value, Some(Value::Bool(actual)) if *actual == expected)
}

fn strict_string(value: Option<&Value>, expected: &str) -> bool {
    matches!(value, Some(Value::String(actual)) if actual == expected)
}

fn js_number(text: &str) -> f64 {
    let text =
        text.trim_matches(|character: char| character.is_whitespace() || character == '\u{feff}');
    if text.is_empty() {
        return 0.0;
    }
    let (sign, unsigned) = match text.as_bytes().first() {
        Some(b'+') => (1.0, &text[1..]),
        Some(b'-') => (-1.0, &text[1..]),
        _ => (1.0, text),
    };
    if unsigned.eq_ignore_ascii_case("infinity") {
        return sign * f64::INFINITY;
    }
    for (prefix, radix) in [("0x", 16), ("0o", 8), ("0b", 2)] {
        if let Some(digits) = text
            .strip_prefix(prefix)
            .or_else(|| text.strip_prefix(&prefix.to_uppercase()))
        {
            if digits.is_empty() {
                return f64::NAN;
            }
            let mut result = 0.0;
            for digit in digits.chars() {
                let Some(digit) = digit.to_digit(radix) else {
                    return f64::NAN;
                };
                result = result * f64::from(radix) + f64::from(digit);
            }
            return result;
        }
    }
    text.parse::<f64>().unwrap_or(f64::NAN)
}

fn integer(value: Option<&Value>, label: &str, minimum: u64, maximum: u64) -> Result<u64, String> {
    let parsed = match value {
        Some(Value::String(value)) => js_number(value),
        Some(Value::Number(value)) => value.as_f64().unwrap_or(f64::NAN),
        _ => f64::NAN,
    };
    if !parsed.is_finite()
        || parsed.fract() != 0.0
        || parsed < minimum as f64
        || parsed > maximum as f64
        || parsed.abs() > 9_007_199_254_740_991.0
    {
        return Err(format!("{label} must be an integer"));
    }
    Ok(parsed as u64)
}

fn hexadecimal(value: u64) -> String {
    format!("0x{value:08x}")
}

fn resource_name(index: usize, count: u64) -> String {
    let width = 3usize.max(format!("{:x}", count - 1).len());
    format!("resource_{index:0width$x}")
}

fn regions(value: &Value, label: &str, base: u64, size: u64) -> Result<Vec<Interval>, String> {
    let manifest = object(value, label)?;
    if !strict_number(field(manifest, "format"), 1.0)
        || !matches!(field(manifest, "regions"), Some(Value::Array(_)))
    {
        return Err(format!("{label} has an unsupported format"));
    }
    if manifest.contains_key("rom_base")
        && integer(
            field(manifest, "rom_base"),
            &format!("{label} ROM base"),
            0,
            0xffff_ffff,
        )? != base
    {
        return Err(format!("{label} ROM base differs"));
    }
    if manifest.contains_key("rom_size")
        && integer(
            field(manifest, "rom_size"),
            &format!("{label} ROM size"),
            0,
            0xffff_ffff,
        )? != size
    {
        return Err(format!("{label} ROM size differs"));
    }
    if manifest.contains_key("verification")
        && !strict_string(field(manifest, "verification"), "source_only")
    {
        return Err(format!("{label} is not source-only"));
    }
    let Value::Array(raw_regions) = &manifest["regions"] else {
        unreachable!();
    };
    raw_regions
        .iter()
        .enumerate()
        .map(|(index, raw)| {
            let region = object(raw, &format!("{label} region {index}"))?;
            let address = integer(
                field(region, "address"),
                &format!("{label} region {index} address"),
                0,
                0xffff_ffff,
            )?;
            let bytes = integer(
                field(region, "size"),
                &format!("{label} region {index} size"),
                1,
                0xffff_ffff,
            )?;
            let end = address
                .checked_add(bytes)
                .ok_or_else(|| format!("{label} region {index} lies outside the ROM"))?;
            if address < base || end > base + size {
                return Err(format!("{label} region {index} lies outside the ROM"));
            }
            if region.contains_key("end")
                && integer(
                    field(region, "end"),
                    &format!("{label} region {index} end"),
                    0,
                    0xffff_ffff,
                )? != end
            {
                return Err(format!("{label} region {index} end differs"));
            }
            Ok(Interval {
                address,
                end,
                size: bytes,
            })
        })
        .collect()
}

fn mark(mask: &mut [u8], source: &[Interval], label: &str, base: u64) -> Result<(), String> {
    for (index, region) in source.iter().enumerate() {
        let start = (region.address - base) as usize;
        let end = (region.end - base) as usize;
        if mask[start..end].iter().any(|value| *value != 0) {
            return Err(format!("{label} region {index} overlaps another source"));
        }
        mask[start..end].fill(1);
    }
    Ok(())
}

pub fn unowned_regions(mask: &[u8], base: u64) -> Result<Vec<Interval>, String> {
    let mut result = Vec::new();
    let mut start: Option<usize> = None;
    for index in 0..=mask.len() {
        let value = if index == mask.len() { 1 } else { mask[index] };
        if value != 0 && value != 1 {
            return Err(format!("coverage byte {index} differs"));
        }
        if value == 0 && start.is_none() {
            start = Some(index);
        } else if value == 1 {
            if let Some(start) = start.take() {
                result.push(Interval {
                    address: base + start as u64,
                    end: base + index as u64,
                    size: (index - start) as u64,
                });
            }
        }
    }
    Ok(result)
}

fn compare_fallback(
    value: &Value,
    expected: &[Interval],
    base: u64,
    size: u64,
) -> Result<(), String> {
    let actual = regions(value, "fallback manifest", base, size)?;
    if actual.len() != expected.len()
        || actual.iter().zip(expected).any(|(actual, expected)| {
            actual.address != expected.address || actual.size != expected.size
        })
    {
        return Err("fallback manifest differs from source ownership".to_string());
    }
    Ok(())
}

fn band(name: &'static str, address: u64, end: u64, gaps: &[Interval]) -> BandReport {
    let mut bytes = 0;
    let mut count = 0;
    for gap in gaps {
        let overlap = end.min(gap.end).saturating_sub(address.max(gap.address));
        if overlap == 0 {
            continue;
        }
        bytes += overlap;
        count += 1;
    }
    BandReport {
        name,
        address: hexadecimal(address),
        end: hexadecimal(end),
        unowned_bytes: bytes,
        gaps: count,
    }
}

fn resource_pointers(value: &Value, base: u64, size: u64) -> Result<Vec<(usize, u64)>, String> {
    let directory = object(value, "resource directory")?;
    if !strict_number(field(directory, "format"), 1.0)
        || !strict_string(field(directory, "kind"), "golden-sun-resource-directory")
    {
        return Err("resource directory has an unsupported format".to_string());
    }
    let count = integer(
        field(directory, "slot_count"),
        "resource directory slot count",
        2,
        0x10000,
    )?;
    let Some(Value::Array(slots)) = field(directory, "slots") else {
        return Err("resource directory slot count differs".to_string());
    };
    if slots.len() as u64 != count {
        return Err("resource directory slot count differs".to_string());
    }
    let mut pointers = Vec::new();
    let mut seen = std::collections::HashSet::new();
    for (index, slot) in slots.iter().enumerate() {
        let Some(slot) = slot.as_str() else {
            return Err(format!("resource directory slot {index} differs"));
        };
        let Some(raw) = slot.strip_prefix("resource:") else {
            continue;
        };
        if raw.len() != 10
            || !raw.starts_with("0x")
            || !raw[2..]
                .bytes()
                .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
        {
            return Err(format!("resource directory slot {index} is not canonical"));
        }
        let address = integer(
            Some(&Value::String(raw.to_string())),
            &format!("resource directory slot {index} address"),
            base,
            base + size - 1,
        )?;
        if !seen.insert(address) {
            return Err(format!(
                "resource directory pointer {} is duplicated",
                hexadecimal(address)
            ));
        }
        pointers.push((index, address));
    }
    Ok(pointers)
}

fn missing_resources(
    mask: &[u8],
    value: &Value,
    base: u64,
) -> Result<Vec<MissingResource>, String> {
    let directory = object(value, "resource directory")?;
    let count = integer(
        field(directory, "slot_count"),
        "resource directory slot count",
        2,
        0x10000,
    )?;
    let pointers = resource_pointers(value, base, mask.len() as u64)?;
    let mut result = Vec::new();
    for position in 0..pointers.len() {
        let (index, address) = pointers[position];
        let next = pointers[position + 1..]
            .iter()
            .find(|(_, candidate)| *candidate > address)
            .map(|(_, candidate)| *candidate);
        let end = next.unwrap_or(base + mask.len() as u64);
        if end <= address {
            continue;
        }
        let source = &mask[(address - base) as usize..(end - base) as usize];
        if source.is_empty() || source.iter().any(|value| *value != 0) {
            continue;
        }
        let name = resource_name(index, count);
        let width = name.len() - "resource_".len();
        result.push(MissingResource {
            id: format!("0x{index:0width$x}"),
            name,
            address: hexadecimal(address),
            end: hexadecimal(end),
            size: end - address,
        });
    }
    Ok(result)
}

fn percent(value: u64) -> f64 {
    format!("{:.2}", value as f64 * 100.0 / ROM_SIZE as f64)
        .parse()
        .expect("formatted percentage is numeric")
}

fn number_eq(value: Option<&Value>, expected: f64) -> bool {
    matches!(value, Some(Value::Number(value)) if value.as_f64() == Some(expected))
}

pub fn audit_residuals(inputs: &Inputs) -> Result<ResidualAudit, String> {
    let full = object(&inputs.full, "full report")?;
    if !strict_number(field(full, "format"), 1.0)
        || !strict_string(field(full, "verification"), "source_only")
        || !strict_boolean(field(full, "byte_identical"), false)
        || !matches!(field(full, "output"), Some(Value::Null))
    {
        return Err("full report is not a source-only build".to_string());
    }
    let base = integer(
        field(full, "rom_base"),
        "full report ROM base",
        0,
        0xffff_ffff,
    )?;
    let size = integer(
        field(full, "rom_size"),
        "full report ROM size",
        1,
        0xffff_ffff,
    )?;
    if base != ROM_BASE || size != ROM_SIZE {
        return Err("full report target differs".to_string());
    }
    let claimed = regions(&inputs.claimed, "claimed manifest", base, size)?;
    let assembly = regions(&inputs.assembly, "assembly manifest", base, size)?;
    let assets = regions(&inputs.assets, "asset manifest", base, size)?;
    let mut mask = vec![0u8; size as usize];
    mark(&mut mask, &claimed, "claimed manifest", base)?;
    mark(&mut mask, &assembly, "assembly manifest", base)?;
    mark(&mut mask, &assets, "asset manifest", base)?;
    let gaps = unowned_regions(&mask, base)?;
    compare_fallback(&inputs.fallback, &gaps, base, size)?;
    let source_bytes = mask.iter().map(|value| u64::from(*value)).sum::<u64>();
    let unowned_bytes = gaps.iter().map(|region| region.size).sum::<u64>();
    let code_bytes = claimed.iter().map(|region| region.size).sum::<u64>();
    let asm_bytes = assembly.iter().map(|region| region.size).sum::<u64>();
    let asset_bytes = assets.iter().map(|region| region.size).sum::<u64>();
    let source_regions = claimed.len() + assembly.len() + assets.len();
    let expected = [
        ("code_regions", claimed.len() as u64),
        ("code_bytes", code_bytes),
        ("asm_regions", assembly.len() as u64),
        ("asm_bytes", asm_bytes),
        ("asset_regions", assets.len() as u64),
        ("asset_bytes", asset_bytes),
        ("source_regions", source_regions as u64),
        ("source_bytes", source_bytes),
        ("unowned_bytes", unowned_bytes),
        ("unowned_regions", gaps.len() as u64),
        ("rom_fallback_bytes", unowned_bytes),
        ("fallback_regions", gaps.len() as u64),
    ];
    for (key, value) in expected {
        if integer(
            field(full, key),
            &format!("full report {key}"),
            0,
            0xffff_ffff,
        )? != value
        {
            return Err(format!("full report {key} differs"));
        }
    }
    if source_bytes + unowned_bytes != size {
        return Err("source ownership does not span the ROM".to_string());
    }
    let asm_debt_bytes = integer(
        field(full, "asm_c_debt_bytes"),
        "full report asm_c_debt_bytes",
        0,
        0xffff_ffff,
    )?;
    let retained_asm_bytes = integer(
        field(full, "asm_retained_structural_bytes"),
        "full report asm_retained_structural_bytes",
        0,
        0xffff_ffff,
    )?;
    if asm_debt_bytes + retained_asm_bytes != asm_bytes {
        return Err("assembly accounting differs".to_string());
    }
    let byte_reconstruction_bytes = code_bytes + asset_bytes + retained_asm_bytes;
    let byte_reconstruction_remaining_bytes = asm_debt_bytes + unowned_bytes;
    let byte_reconstruction_percent = percent(byte_reconstruction_bytes);
    if byte_reconstruction_bytes + byte_reconstruction_remaining_bytes != size
        || integer(
            field(full, "byte_reconstruction_bytes"),
            "full report byte_reconstruction_bytes",
            0,
            0xffff_ffff,
        )? != byte_reconstruction_bytes
        || integer(
            field(full, "byte_reconstruction_remaining_bytes"),
            "full report byte_reconstruction_remaining_bytes",
            0,
            0xffff_ffff,
        )? != byte_reconstruction_remaining_bytes
        || !number_eq(
            field(full, "byte_reconstruction_percent"),
            byte_reconstruction_percent,
        )
    {
        return Err("byte reconstruction accounting differs".to_string());
    }
    if integer(
        field(full, "total_decompilation_bytes"),
        "full report total_decompilation_bytes",
        0,
        0xffff_ffff,
    )? != byte_reconstruction_bytes
        || integer(
            field(full, "total_decompilation_remaining_bytes"),
            "full report total_decompilation_remaining_bytes",
            0,
            0xffff_ffff,
        )? != byte_reconstruction_remaining_bytes
        || !number_eq(
            field(full, "total_decompilation_percent"),
            byte_reconstruction_percent,
        )
        || !strict_string(
            field(full, "total_decompilation_semantics"),
            "deprecated_alias_for_byte_reconstruction",
        )
    {
        return Err("deprecated byte reconstruction aliases differ".to_string());
    }
    let project_completion = object(
        field(full, "project_completion").ok_or_else(|| {
            "project completion must remain unscored while its audit is pending".to_string()
        })?,
        "full report project_completion",
    )?;
    if !strict_string(field(project_completion, "status"), "audit_pending")
        || !matches!(field(project_completion, "percent"), Some(Value::Null))
        || !strict_string(
            field(project_completion, "scoring"),
            "withheld_until_all_dimensions_are_audited",
        )
    {
        return Err(
            "project completion must remain unscored while its audit is pending".to_string(),
        );
    }
    let resources = missing_resources(&mask, &inputs.directory, base)?;
    let bands = vec![
        band("main", base, AUDIO_ADDRESS, &gaps),
        band("audio", AUDIO_ADDRESS, RESOURCE_ADDRESS, &gaps),
        band("resources", RESOURCE_ADDRESS, base + size, &gaps),
    ];
    if bands.iter().map(|item| item.unowned_bytes).sum::<u64>() != unowned_bytes {
        return Err("residual bands do not span the ROM".to_string());
    }
    Ok(ResidualAudit {
        format: 1,
        kind: "alchemy-source-residual-audit",
        verification: "source_only",
        rom_base: hexadecimal(base),
        rom_size: size,
        source_regions,
        source_bytes,
        unowned_bytes,
        gaps: gaps.len(),
        project_completion_status: "audit_pending",
        project_completion_percent: None,
        byte_reconstruction_bytes,
        byte_reconstruction_remaining_bytes,
        byte_reconstruction_percent,
        total_decompilation_bytes: byte_reconstruction_bytes,
        total_decompilation_remaining_bytes: byte_reconstruction_remaining_bytes,
        total_decompilation_percent: byte_reconstruction_percent,
        total_decompilation_semantics: "deprecated_alias_for_byte_reconstruction",
        bands,
        missing_resource_bytes: resources.iter().map(|resource| resource.size).sum(),
        missing_resources: resources,
    })
}

fn report_value(report: &ResidualAudit) -> Value {
    let mut value = serde_json::to_value(report).expect("residual report serializes");
    let Value::Object(object) = &mut value else {
        unreachable!();
    };
    object.insert(
        "byte_reconstruction_percent".to_string(),
        decimal_number(report.byte_reconstruction_percent),
    );
    object.insert(
        "total_decompilation_percent".to_string(),
        decimal_number(report.total_decompilation_percent),
    );
    value
}

fn decimal_number(value: f64) -> Value {
    let text = format!("{value:.2}");
    let text = text.trim_end_matches('0').trim_end_matches('.');
    serde_json::from_str(text).expect("formatted percentage is valid JSON")
}

fn test_inputs() -> Inputs {
    let gap_list = vec![
        serde_json::json!({"address": ROM_BASE + 2, "size": 2}),
        serde_json::json!({"address": AUDIO_ADDRESS + 1, "size": 3}),
        serde_json::json!({"address": 0x0832_0020u64, "size": 16}),
    ];
    let claimed_regions = vec![
        serde_json::json!({"address": ROM_BASE, "size": 2}),
        serde_json::json!({"address": ROM_BASE + 4, "size": AUDIO_ADDRESS + 1 - (ROM_BASE + 4)}),
    ];
    let assembly_regions = vec![serde_json::json!({
        "address": AUDIO_ADDRESS + 4,
        "size": 0x0832_0020u64 - (AUDIO_ADDRESS + 4)
    })];
    let asset_regions = vec![serde_json::json!({
        "address": 0x0832_0030u64,
        "size": ROM_BASE + ROM_SIZE - 0x0832_0030u64
    })];
    let source_regions = claimed_regions.len() + assembly_regions.len() + asset_regions.len();
    let unowned_bytes = 21u64;
    let code_bytes = 2 + AUDIO_ADDRESS + 1 - (ROM_BASE + 4);
    let asm_bytes = 0x0832_0020u64 - (AUDIO_ADDRESS + 4);
    let asset_bytes = ROM_BASE + ROM_SIZE - 0x0832_0030u64;
    let byte_reconstruction_bytes = code_bytes + asset_bytes;
    let byte_reconstruction_remaining_bytes = asm_bytes + unowned_bytes;
    let byte_reconstruction_percent = percent(byte_reconstruction_bytes);
    Inputs {
        full: serde_json::json!({
            "format": 1,
            "rom_base": ROM_BASE,
            "rom_size": ROM_SIZE,
            "code_regions": claimed_regions.len(),
            "code_bytes": code_bytes,
            "asm_regions": assembly_regions.len(),
            "asm_bytes": asm_bytes,
            "asm_c_debt_bytes": asm_bytes,
            "asm_retained_structural_bytes": 0,
            "asset_regions": asset_regions.len(),
            "asset_bytes": asset_bytes,
            "source_regions": source_regions,
            "source_bytes": ROM_SIZE - unowned_bytes,
            "unowned_bytes": unowned_bytes,
            "unowned_regions": gap_list.len(),
            "project_completion": {
                "status": "audit_pending",
                "percent": null,
                "scoring": "withheld_until_all_dimensions_are_audited"
            },
            "byte_reconstruction_bytes": byte_reconstruction_bytes,
            "byte_reconstruction_remaining_bytes": byte_reconstruction_remaining_bytes,
            "byte_reconstruction_percent": byte_reconstruction_percent,
            "total_decompilation_bytes": byte_reconstruction_bytes,
            "total_decompilation_remaining_bytes": byte_reconstruction_remaining_bytes,
            "total_decompilation_percent": byte_reconstruction_percent,
            "total_decompilation_semantics": "deprecated_alias_for_byte_reconstruction",
            "rom_fallback_bytes": unowned_bytes,
            "fallback_regions": gap_list.len(),
            "verification": "source_only",
            "byte_identical": false,
            "output": null
        }),
        fallback: serde_json::json!({"format": 1, "rom_base": ROM_BASE, "rom_size": ROM_SIZE, "regions": gap_list}),
        claimed: serde_json::json!({"format": 1, "rom_base": ROM_BASE, "rom_size": ROM_SIZE, "verification": "source_only", "regions": claimed_regions}),
        assembly: serde_json::json!({"format": 1, "rom_base": ROM_BASE, "verification": "source_only", "regions": assembly_regions}),
        assets: serde_json::json!({"format": 1, "rom_base": ROM_BASE, "rom_size": ROM_SIZE, "regions": asset_regions}),
        directory: serde_json::json!({
            "format": 1,
            "kind": "golden-sun-resource-directory",
            "address": "0x08320000",
            "slot_count": 4,
            "slots": ["rom-base", "directory-self", "resource:0x08320020", "resource:0x08320030"]
        }),
    }
}

fn must_reject(inputs: &Inputs, expected: &str) -> Result<(), String> {
    match audit_residuals(inputs) {
        Err(error) if error.contains(expected) => Ok(()),
        Err(error) => Err(format!("unexpected rejection: {error}")),
        Ok(_) => Err(format!("audit accepted invalid input for {expected}")),
    }
}

pub fn self_test() -> Result<(), String> {
    let inputs = test_inputs();
    let report = audit_residuals(&inputs)?;
    if report.unowned_bytes != 21
        || report.gaps != 3
        || report.source_bytes != ROM_SIZE - 21
        || report.project_completion_status != "audit_pending"
        || report.project_completion_percent.is_some()
        || report.byte_reconstruction_bytes + report.byte_reconstruction_remaining_bytes != ROM_SIZE
        || report.bands[0].unowned_bytes != 2
        || report.bands[0].gaps != 1
        || report.bands[1].unowned_bytes != 3
        || report.bands[1].gaps != 1
        || report.bands[2].unowned_bytes != 16
        || report.bands[2].gaps != 1
        || report.missing_resources.len() != 1
        || report.missing_resources[0].name != "resource_002"
        || report.missing_resources[0].size != 16
        || report.missing_resource_bytes != 16
    {
        return Err("residual audit self-test differs".to_string());
    }
    let mut overlap = inputs.clone();
    overlap
        .assembly
        .as_object_mut()
        .unwrap()
        .get_mut("regions")
        .unwrap()
        .as_array_mut()
        .unwrap()
        .push(serde_json::json!({"address": ROM_BASE, "size": 1}));
    must_reject(&overlap, "overlaps")?;
    let mut fallback = inputs.clone();
    fallback.fallback["regions"][0]["size"] = serde_json::json!(1);
    must_reject(&fallback, "fallback manifest differs")?;
    let mut verified = inputs.clone();
    verified.full["verification"] = serde_json::json!("rom");
    must_reject(&verified, "not a source-only build")?;
    let mut duplicate = inputs;
    duplicate.directory["slots"] = serde_json::json!([
        "rom-base",
        "directory-self",
        "resource:0x08320020",
        "resource:0x08320020"
    ]);
    must_reject(&duplicate, "duplicated")
}

fn usage() -> &'static str {
    "usage: audit_residuals.ts [--full-report FILE] [--fallback-manifest FILE] [--claimed-manifest FILE] [--asm-manifest FILE] [--asset-manifest FILE] [--resource-directory FILE] [--json] | --self-test"
}

fn parse_args(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options {
        full_report: "out/full/rebuilt.json".to_string(),
        fallback_manifest: None,
        claimed_manifest: "out/full/claimed/manifest.json".to_string(),
        asm_manifest: "out/full/asm/manifest.json".to_string(),
        asset_manifest: "out/full/assets/manifest.json".to_string(),
        resource_directory: "assets/data/resource_directory.json".to_string(),
        json: false,
    };
    let mut index = 0;
    while index < arguments.len() {
        let argument = &arguments[index];
        if argument == "--json" {
            options.json = true;
            index += 1;
            continue;
        }
        let (name, inline) = argument
            .split_once('=')
            .map_or((argument.as_str(), None), |(name, value)| {
                (name, Some(value))
            });
        let value = if let Some(value) = inline {
            index += 1;
            value.to_string()
        } else {
            let value = arguments
                .get(index + 1)
                .cloned()
                .ok_or_else(|| format!("{name} requires a value"))?;
            index += 2;
            value
        };
        if value.is_empty() {
            return Err(format!("{name} requires a value"));
        }
        match name {
            "--full-report" => options.full_report = value,
            "--fallback-manifest" => options.fallback_manifest = Some(value),
            "--claimed-manifest" => options.claimed_manifest = value,
            "--asm-manifest" => options.asm_manifest = value,
            "--asset-manifest" => options.asset_manifest = value,
            "--resource-directory" => options.resource_directory = value,
            _ => return Err(format!("unrecognized argument: {argument}")),
        }
    }
    Ok(options)
}

fn read_json(path: &str, label: &str) -> Result<Value, String> {
    if !Path::new(path).exists() {
        return Err(format!("{label} does not exist: {path}"));
    }
    let text =
        fs::read_to_string(path).map_err(|_| format!("{label} is not valid JSON: {path}"))?;
    serde_json::from_str(&text).map_err(|_| format!("{label} is not valid JSON: {path}"))
}

fn js_string(value: Option<&Value>) -> String {
    match value {
        None | Some(Value::Null) => String::new(),
        Some(Value::String(value)) => value.clone(),
        Some(Value::Bool(value)) => value.to_string(),
        Some(Value::Number(value)) => value.to_string(),
        Some(_) => "[object Object]".to_string(),
    }
}

fn print_report(report: &ResidualAudit) -> String {
    let mut lines = vec![format!(
        "source_only=True source_regions={} source_bytes={} unowned_bytes={} gaps={} source_owned={}",
        report.source_regions,
        report.source_bytes,
        report.unowned_bytes,
        report.gaps,
        if report.unowned_bytes == 0 { "yes" } else { "no" }
    )];
    for item in &report.bands {
        lines.push(format!(
            "band={} address={} end={} unowned_bytes={} gaps={}",
            item.name, item.address, item.end, item.unowned_bytes, item.gaps
        ));
    }
    lines.push(format!(
        "missing_resources={} bytes={}",
        report.missing_resources.len(),
        report.missing_resource_bytes
    ));
    for resource in &report.missing_resources {
        lines.push(format!(
            "{} address={} end={} bytes={}",
            resource.name, resource.address, resource.end, resource.size
        ));
    }
    lines.join("\n")
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    if arguments
        .iter()
        .any(|argument| argument == "-h" || argument == "--help")
    {
        return Ok(usage().to_string());
    }
    if arguments.iter().any(|argument| argument == "--self-test") {
        if arguments.len() != 1 {
            return Err("--self-test does not accept other arguments".to_string());
        }
        self_test()?;
        return Ok("self-test=ok".to_string());
    }
    let options = parse_args(arguments)?;
    let full = read_json(&options.full_report, "full report")?;
    let fallback_path = options
        .fallback_manifest
        .clone()
        .unwrap_or_else(|| js_string(full.get("fallback_manifest")));
    if fallback_path.is_empty() {
        return Err("full report has no fallback manifest".to_string());
    }
    let report = audit_residuals(&Inputs {
        fallback: read_json(&fallback_path, "fallback manifest")?,
        claimed: read_json(&options.claimed_manifest, "claimed manifest")?,
        assembly: read_json(&options.asm_manifest, "assembly manifest")?,
        assets: read_json(&options.asset_manifest, "asset manifest")?,
        directory: read_json(&options.resource_directory, "resource directory")?,
        full,
    })?;
    if options.json {
        Ok(canonical_json(&report_value(&report)))
    } else {
        Ok(print_report(&report))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes() {
        self_test().unwrap();
    }

    #[test]
    fn cli_self_test_matches_typescript_output() {
        assert_eq!(run(&["--self-test".to_string()]).unwrap(), "self-test=ok");
    }

    #[test]
    fn cli_rejects_extra_self_test_arguments() {
        assert_eq!(
            run(&["--self-test".to_string(), "--json".to_string()]).unwrap_err(),
            "--self-test does not accept other arguments"
        );
    }
}
