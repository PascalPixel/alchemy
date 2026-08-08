use std::collections::HashSet;

use overlay_call_targets::{overlay_image, target_offset, BASE_SHIFT, OVERLAY_BASE};
use overlay_gaps::{is_return_shape, mask_banks, owner_spans, rule_tail};
use overlay_published::overlay_names;

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Finding {
    pub offset: i64,
    pub region: String,
    pub note: String,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Certification {
    pub overlay: String,
    pub owners: usize,
    pub image_bytes: usize,
    pub tail_start: i64,
    pub tail_bytes: i64,
    pub tail_veneers: i64,
    pub tail_call_via: i64,
    pub tail_returns: usize,
    pub tail_pointer_halves: usize,
    pub tail_dispatched: usize,
    pub bl_sites: usize,
    pub bl_to_bank: usize,
    pub bl_to_code: usize,
    pub tail_pointers: usize,
    pub multi_return_owners: Vec<i64>,
    pub findings: Vec<Finding>,
}

fn u16le(image: &[u8], at: i64) -> u16 {
    if at < 0 || at + 1 >= image.len() as i64 {
        return 0;
    }
    let at = at as usize;
    image[at] as u16 | ((image[at + 1] as u16) << 8)
}

fn u32le(image: &[u8], at: i64) -> u32 {
    if at < 0 || at + 3 >= image.len() as i64 {
        return 0;
    }
    let at = at as usize;
    (image[at] as u32)
        | ((image[at + 1] as u32) << 8)
        | ((image[at + 2] as u32) << 16)
        | ((image[at + 3] as u32) << 24)
}

/// Offset designated by an in-image function pointer, or `None` elsewhere.
pub fn pointer_offset(word: u32, image_length: usize) -> Option<i64> {
    let offset = (word & !1) as i64 - OVERLAY_BASE - BASE_SHIFT;
    (offset >= 0 && offset < image_length as i64).then_some(offset)
}

fn empty(overlay: &str, image_bytes: usize) -> Certification {
    Certification {
        overlay: overlay.to_string(),
        owners: 0,
        image_bytes,
        tail_start: 0,
        tail_bytes: 0,
        tail_veneers: 0,
        tail_call_via: 0,
        tail_returns: 0,
        tail_pointer_halves: 0,
        tail_dispatched: 0,
        bl_sites: 0,
        bl_to_bank: 0,
        bl_to_code: 0,
        tail_pointers: 0,
        multi_return_owners: vec![],
        findings: vec![Finding {
            offset: 0,
            region: "OWNER".into(),
            note: "no recorded owners -- nothing to certify".into(),
        }],
    }
}

pub fn certify(overlay: &str) -> Result<Certification, String> {
    let image = overlay_image(overlay)?;
    let spans_result = owner_spans(overlay)?;
    certify_image(overlay, &image, &spans_result.spans)
}

/// Certify a supplied image and owner list. Keeping this pure makes the
/// self-test independent of the current ROM and owner inventory.
pub fn certify_image(
    overlay: &str,
    image: &[u8],
    owner_list: &[(i64, i64)],
) -> Result<Certification, String> {
    let mut spans = owner_list.to_vec();
    spans.sort_by_key(|(start, _)| *start);
    spans.retain(|(start, end)| end > start);
    if spans.is_empty() {
        return Ok(empty(overlay, image.len()));
    }

    let mut findings = Vec::new();
    for (start, end) in &spans {
        if *end > image.len() as i64 || *start < 0 {
            findings.push(Finding {
                offset: *start,
                region: "OWNER".into(),
                note: format!(
                    "recorded span ends at 0x{end:x}, past the {}-byte image",
                    image.len()
                ),
            });
        }
    }
    for (_, end) in &mut spans {
        *end = (*end).min(image.len() as i64);
    }

    let tail_start = spans.last().map(|(_, end)| *end).unwrap_or(0);
    let tail = rule_tail(image, tail_start);
    let (covered, _, _) = mask_banks(image, tail_start, image.len() as i64);
    let in_bank = |offset: i64| {
        offset >= tail_start
            && offset < image.len() as i64
            && covered
                .get((offset - tail_start) as usize)
                .copied()
                .unwrap_or(false)
    };
    let starts: HashSet<i64> = spans.iter().map(|(start, _)| *start).collect();

    let mut bl_sites = 0usize;
    let mut bl_to_bank = 0usize;
    let mut bl_to_code = 0usize;
    let mut dispatched = HashSet::new();
    let mut stray_calls = Vec::new();
    for (start, end) in &spans {
        let mut at = *start;
        while at + 4 <= *end {
            let high = u16le(image, at);
            let low = u16le(image, at + 2);
            if let Some(target) = target_offset(high, low) {
                if target >= 0 && target < image.len() as i64 {
                    bl_sites += 1;
                    if in_bank(target) {
                        bl_to_bank += 1;
                    } else if target < tail_start {
                        bl_to_code += 1;
                    } else {
                        dispatched.insert(target);
                        stray_calls.push((at, target));
                    }
                }
            }
            at += 2;
        }
    }

    let mut tail_pointer_halves = 0usize;
    let mut tail_dispatched = 0usize;
    let mut ruled_dispatch = HashSet::new();
    for &at in &tail.returns {
        if at % 4 == 0 {
            let value = u32le(image, at);
            if value & 1 == 1 {
                if let Some(target) = pointer_offset(value, image.len()) {
                    if starts.contains(&target) {
                        tail_pointer_halves += 1;
                        continue;
                    }
                }
            }
        }
        if (u16le(image, at) & 0xff87) == 0x4700 && dispatched.contains(&at) {
            tail_dispatched += 1;
            ruled_dispatch.insert(at);
            continue;
        }
        findings.push(Finding {
            offset: at,
            region: "TAIL".into(),
            note: format!(
                "return shape 0x{:x} outside the banks, not a published-pointer half, not dispatched",
                u16le(image, at)
            ),
        });
    }
    for (at, target) in stray_calls {
        if !ruled_dispatch.contains(&target) {
            findings.push(Finding {
                offset: at,
                region: "BL".into(),
                note: format!("resolves to 0x{target:x}, inside the tail and outside every bank"),
            });
        }
    }

    let mut multi_return_owners = Vec::new();
    for (start, end) in &spans {
        let mut returns = 0usize;
        let mut at = *start;
        while at + 1 < *end {
            if is_return_shape(u16le(image, at)) {
                returns += 1;
            }
            at += 2;
        }
        if returns == 0 {
            findings.push(Finding {
                offset: *start,
                region: "OWNER".into(),
                note: "recorded span contains NO return shape".into(),
            });
        } else if returns > 1 {
            multi_return_owners.push(*start);
        }
    }

    let mut tail_pointers = 0usize;
    let mut at = 0i64;
    while at + 4 <= image.len() as i64 {
        let value = u32le(image, at);
        if let Some(target) = pointer_offset(value, image.len()) {
            if target >= tail_start && !in_bank(target) {
                tail_pointers += 1;
                if value & 1 == 1 {
                    findings.push(Finding {
                        offset: at,
                        region: "POINTER".into(),
                        note: format!(
                            "word 0x{value:x} carries the Thumb bit into tail data at 0x{target:x}"
                        ),
                    });
                }
            }
        }
        at += 4;
    }

    Ok(Certification {
        overlay: overlay.to_string(),
        owners: spans.len(),
        image_bytes: image.len(),
        tail_start,
        tail_bytes: tail.bytes,
        tail_veneers: tail.veneers,
        tail_call_via: tail.call_via,
        tail_returns: tail.returns.len(),
        tail_pointer_halves,
        tail_dispatched,
        bl_sites,
        bl_to_bank,
        bl_to_code,
        tail_pointers,
        multi_return_owners,
        findings,
    })
}

fn synthetic(owners: &[u16], tail: &[u16]) -> Vec<u8> {
    owners
        .iter()
        .chain(tail)
        .flat_map(|word| word.to_le_bytes())
        .collect()
}

pub fn self_test() -> Result<(), String> {
    if pointer_offset(0x0200c0b5, 0x10000) != Some(0x40b4)
        || pointer_offset(0x0200c0b4, 0x10000) != Some(0x40b4)
        || pointer_offset(0x03001ebc, 0x10000).is_some()
        || pointer_offset(0x02000004, 0x10000).is_some()
    {
        return Err("overlay certify self-test: pointer offsets".into());
    }
    let image = synthetic(&[0xb500, 0xbd00], &[0, 0]);
    let clean = certify_image("synthetic", &image, &[(0, 4)])?;
    if !clean.findings.is_empty() {
        return Err("overlay certify self-test: clean image".into());
    }
    let leaf = certify_image(
        "synthetic",
        &synthetic(&[0xb500, 0xbd00], &[0x2000, 0x4770]),
        &[(0, 4)],
    )?;
    if !leaf.findings.iter().any(|finding| finding.region == "TAIL") {
        return Err("overlay certify self-test: tail leaf".into());
    }
    let veneer = certify_image(
        "synthetic",
        &synthetic(&[0xb500, 0xbd00], &[0x4800, 0x4700, 0x0100, 0x0300]),
        &[(0, 4)],
    )?;
    if veneer.tail_veneers != 1 || !veneer.findings.is_empty() {
        return Err("overlay certify self-test: veneer mask".into());
    }
    let getter = certify_image(
        "synthetic",
        &synthetic(&[0xb500, 0xbd00], &[0x4800, 0x4770, 0x0100, 0x0300]),
        &[(0, 4)],
    )?;
    if getter.tail_veneers != 0 || getter.findings.is_empty() {
        return Err("overlay certify self-test: getter stub".into());
    }
    let mut wide = vec![0u8; 0x3d50];
    wide[0x3d40..0x3d42].copy_from_slice(&0xb500u16.to_le_bytes());
    wide[0x3d42..0x3d44].copy_from_slice(&0xbd00u16.to_le_bytes());
    wide[0x3d48..0x3d4c].copy_from_slice(&0x0200bd41u32.to_le_bytes());
    let pointer_half = certify_image("synthetic", &wide, &[(0x3d40, 0x3d44)])?;
    if pointer_half.tail_pointer_halves != 1 || !pointer_half.findings.is_empty() {
        return Err("overlay certify self-test: pointer half".into());
    }
    let swallowed = certify_image(
        "synthetic",
        &synthetic(&[0xb500, 0xbd00, 0x2000, 0x4770], &[0]),
        &[(0, 8)],
    )?;
    if swallowed.multi_return_owners != vec![0] {
        return Err("overlay certify self-test: swallowed leaf".into());
    }
    let overrun = certify_image("synthetic", &image, &[(0, 999)])?;
    if !overrun
        .findings
        .iter()
        .any(|finding| finding.region == "OWNER" && finding.note.contains("past"))
    {
        return Err("overlay certify self-test: span overrun".into());
    }
    println!("sweep E self-test passed (8 checks, tail ruling, owner returns, bl, pointers)");
    Ok(())
}

fn json_escape(text: &str) -> String {
    format!("\"{}\"", text.replace('\\', "\\\\").replace('"', "\\\""))
}

fn ints(values: &[i64]) -> String {
    format!(
        "[{}]",
        values
            .iter()
            .map(ToString::to_string)
            .collect::<Vec<_>>()
            .join(",")
    )
}

fn certification_json(result: &Certification) -> String {
    let findings = result
        .findings
        .iter()
        .map(|finding| {
            format!(
                "{{\"offset\":{},\"region\":{},\"note\":{}}}",
                finding.offset,
                json_escape(&finding.region),
                json_escape(&finding.note)
            )
        })
        .collect::<Vec<_>>()
        .join(",");
    format!(
        "{{\"overlay\":{},\"owners\":{},\"imageBytes\":{},\"tailStart\":{},\"tailBytes\":{},\"tailVeneers\":{},\"tailCallVia\":{},\"tailReturns\":{},\"tailPointerHalves\":{},\"tailDispatched\":{},\"blSites\":{},\"blToBank\":{},\"blToCode\":{},\"tailPointers\":{},\"multiReturnOwners\":{},\"findings\":[{}]}}",
        json_escape(&result.overlay), result.owners, result.image_bytes, result.tail_start,
        result.tail_bytes, result.tail_veneers, result.tail_call_via, result.tail_returns,
        result.tail_pointer_halves, result.tail_dispatched, result.bl_sites, result.bl_to_bank,
        result.bl_to_code, result.tail_pointers, ints(&result.multi_return_owners), findings
    )
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let requested = args
        .iter()
        .filter(|arg| !arg.starts_with("--"))
        .cloned()
        .collect::<Vec<_>>();
    let known = overlay_names()?;
    for name in &requested {
        if !known.contains(name) {
            println!("NOTHING SWEPT — no overlay named {name}. This is a FAILURE, not a pass.");
            return Err("unknown overlay".into());
        }
    }
    let overlays = if requested.is_empty() {
        known
    } else {
        requested
    };
    if overlays.is_empty() {
        return Err("NOTHING SWEPT — this is a FAILURE, not a pass.".into());
    }
    let results = overlays
        .iter()
        .map(|overlay| certify(overlay))
        .collect::<Result<Vec<_>, _>>()?;
    if args.iter().any(|arg| arg == "--json") {
        println!(
            "[{}]",
            results
                .iter()
                .map(certification_json)
                .collect::<Vec<_>>()
                .join(",")
        );
        return Ok(());
    }
    let findings: usize = results.iter().map(|result| result.findings.len()).sum();
    for result in &results {
        if result.findings.is_empty() && result.multi_return_owners.is_empty() && results.len() > 1
        {
            continue;
        }
        println!(
            "{}  owners={} tail={}B from 0x{:x} ({} veneers, {} call_via) returns={} ruled={} pointer-halves + {} dispatched bl={} ({} bank, {} code)",
            result.overlay, result.owners, result.tail_bytes, result.tail_start,
            result.tail_veneers, result.tail_call_via, result.tail_returns,
            result.tail_pointer_halves, result.tail_dispatched, result.bl_sites,
            result.bl_to_bank, result.bl_to_code
        );
        for owner in &result.multi_return_owners {
            println!(
                "  OWNER  0x{:x}  more than one return shape — read for a swallowed leaf",
                OVERLAY_BASE + owner
            );
        }
        for finding in &result.findings {
            println!(
                "  {:<7}0x{:x}  {}",
                finding.region,
                OVERLAY_BASE + finding.offset,
                finding.note
            );
        }
    }
    println!("\noverlays={} sweep_e_findings={findings}", results.len());
    if findings > 0 {
        return Err(format!("sweep E found {findings} findings"));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn native_self_test_passes() {
        self_test().unwrap();
    }

    #[test]
    fn prologue_helpers_are_used_by_the_native_boundary() {
        assert!(overlay_published::is_prologue_shape(0xb500));
        assert!(overlay_published::saves_link_register(0xb560));
    }
}
