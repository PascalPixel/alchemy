//! Executable-byte audit derived from canonical ROM images.
pub(crate) mod index;
mod verification;

use super::proof::{full_asset_manifest, full_build_report, last_full_build};
use crate::compiler::canonical_json::canonical_json;
use crate::overlay::assembly::{
    compiler_runtime_spans, executable_spans, main_executable_spans, ExecutableSpan, OVERLAY_BASE,
    ROM_BASE,
};
use crate::overlay::flow::{alignment_spans, overlay_code, veneer_spans, Failure};
use crate::overlay::rom::CanonicalRom;
use crate::targets::{decomp_target, target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use serde::Deserialize;
use serde_json::{json, Value};
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy coverage audit --target TARGET [--output out/...json] [--calibrate --expected LEDGER.json]
       alchemy coverage audit --target TARGET --inventory
       alchemy coverage audit (--target TARGET | --all) --data
The candidate report goes to out/<target>/reports/executable-audit-candidate.json unless
--output names another path; it never replaces the executable inventory. Each run prints the
sha256 of the candidate's canonical overlay intervals. --calibrate lists every range that
differs from the ledger named by --expected: a diagnostic that grants no authority.
--inventory writes out/<target>/reports/executable.json: the complete inventory only when the
overlay intervals hash to the digest recon/<game>/metrics/audit-verification.json records as
independently verified and a byte-identical full ROM build proves the main image, otherwise a
pending inventory. The inventory is pending while the run is in progress and after it fails.";

#[derive(Default)]
struct Options {
    target: Option<String>,
    all: bool,
    output: Option<PathBuf>,
    calibrate: bool,
    inventory: bool,
    expected: Option<PathBuf>,
    help: bool,
    data: bool,
}

#[derive(Deserialize)]
struct ExpectedReport {
    main: ExpectedMain,
    overlays: Vec<ExpectedOverlay>,
}

#[derive(Deserialize)]
struct ExpectedMain {
    executable_bytes: u64,
    intervals: Vec<ExpectedSpan>,
}

#[derive(Deserialize)]
struct ExpectedSpan {
    start: i64,
    end: i64,
    kind: String,
}

#[derive(Deserialize)]
struct ExpectedOverlay {
    id: String,
    executable_bytes: u64,
    intervals: Vec<ExpectedSpan>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut index = 0;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--target" => {
                index += 1;
                options.target = arguments.get(index).cloned();
            }
            "--output" => {
                index += 1;
                options.output = arguments.get(index).map(PathBuf::from);
            }
            "--expected" => {
                index += 1;
                options.expected = arguments.get(index).map(PathBuf::from);
            }
            "--all" => options.all = true,
            "--calibrate" => options.calibrate = true,
            "--inventory" => options.inventory = true,
            "--data" => options.data = true,
            "-h" | "--help" => options.help = true,
            argument => return Err(format!("unrecognized argument {argument:?}\n{USAGE}")),
        }
        index += 1;
    }
    if options.help {
        return Ok(options);
    }
    if options.target.is_some() == options.all {
        return Err(format!("choose exactly one of --target or --all\n{USAGE}"));
    }
    if options.data
        && (options.calibrate
            || options.inventory
            || options.output.is_some()
            || options.expected.is_some())
    {
        return Err(format!(
            "--data writes out/<target>/reports/rom-index.json and accepts no other mode\n{USAGE}"
        ));
    }
    if options.all && !options.data {
        return Err(format!("--all requires --data\n{USAGE}"));
    }
    if options.calibrate && options.inventory {
        return Err(format!("choose --calibrate or --inventory\n{USAGE}"));
    }
    if options.calibrate != options.expected.is_some() {
        return Err(format!(
            "--expected names the ledger --calibrate compares with; --inventory is gated by the verification record, not a ledger\n{USAGE}"
        ));
    }
    if options.inventory && options.output.is_some() {
        return Err(format!(
            "--inventory writes only out/<target>/reports/executable.json\n{USAGE}"
        ));
    }
    Ok(options)
}

fn resolve(root: &Path, path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    }
}

/// The authoritative executable inventory that progress and coverage read.
fn inventory_path(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.output_dir).join("reports/executable.json")
}

/// The diagnostic candidate report, which coverage reads for data resources.
fn candidate_path(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.output_dir)
        .join("reports/executable-audit-candidate.json")
}

fn union_bytes(spans: &[ExecutableSpan]) -> u64 {
    let mut ranges = spans
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = merged.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        merged.push(range);
    }
    merged
        .into_iter()
        .map(|(start, end)| (end - start) as u64)
        .sum()
}

fn normalized_ranges<I: IntoIterator<Item = (i64, i64)>>(ranges: I) -> Vec<(i64, i64)> {
    let mut ranges = ranges
        .into_iter()
        .filter(|(start, end)| end > start)
        .collect::<Vec<_>>();
    ranges.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for range in ranges {
        if let Some(last) = merged.last_mut() {
            if range.0 <= last.1 {
                last.1 = last.1.max(range.1);
                continue;
            }
        }
        merged.push(range);
    }
    merged
}

fn intersection_bytes(left: &[(i64, i64)], right: &[(i64, i64)]) -> u64 {
    let left = normalized_ranges(left.iter().copied());
    let right = normalized_ranges(right.iter().copied());
    let mut total = 0u64;
    let mut a = 0;
    let mut b = 0;
    while a < left.len() && b < right.len() {
        let start = left[a].0.max(right[b].0);
        let end = left[a].1.min(right[b].1);
        if end > start {
            total += (end - start) as u64;
        }
        if left[a].1 < right[b].1 {
            a += 1;
        } else {
            b += 1;
        }
    }
    total
}

fn difference_ranges(left: &[(i64, i64)], right: &[(i64, i64)]) -> Vec<(i64, i64)> {
    let right = normalized_ranges(right.iter().copied());
    let mut out = Vec::new();
    for (start, end) in normalized_ranges(left.iter().copied()) {
        let mut cursor = start;
        for &(cut_start, cut_end) in &right {
            if cut_end <= cursor {
                continue;
            }
            if cut_start >= end {
                break;
            }
            if cut_start > cursor {
                out.push((cursor, cut_start.min(end)));
            }
            cursor = cursor.max(cut_end);
        }
        if cursor < end {
            out.push((cursor, end));
        }
    }
    out
}

fn subtract_spans(spans: Vec<ExecutableSpan>, cuts: &[(i64, i64)]) -> Vec<ExecutableSpan> {
    let cuts = normalized_ranges(cuts.iter().copied());
    let mut out = Vec::new();
    for span in spans {
        for (start, end) in difference_ranges(&[(span.start, span.end)], &cuts) {
            out.push(ExecutableSpan {
                start,
                end,
                kind: span.kind,
            });
        }
    }
    out
}

/// Byte-reproduced asset regions and the manifest path that verified them.
type AssetCuts = (Vec<(i64, i64)>, Option<String>);

/// The byte-reproduced asset regions and the manifest that verified them:
/// the full ROM build's when present, otherwise the asset build's. A
/// diagnostic until [`main_image_proof`] binds the full build's manifest to
/// its proof.
fn asset_cuts(root: &Path, target: DecompTarget) -> Result<AssetCuts, String> {
    let candidates = [
        full_asset_manifest(target),
        format!("{}/assets/manifest.json", target.output_dir),
    ];
    let Some(relative) = candidates.iter().find(|path| root.join(path).exists()) else {
        return Ok((Vec::new(), None));
    };
    let bytes =
        std::fs::read(root.join(relative)).map_err(|error| format!("{relative}: {error}"))?;
    Ok((manifest_cuts(&bytes, relative)?, Some(relative.clone())))
}

/// The byte-reproduced asset regions an asset manifest, read from `label`,
/// lists.
fn manifest_cuts(bytes: &[u8], label: &str) -> Result<Vec<(i64, i64)>, String> {
    let manifest: Value =
        serde_json::from_slice(bytes).map_err(|error| format!("{label}: {error}"))?;
    Ok(manifest["regions"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| {
            let start = row["address"].as_i64()?;
            let size = row["size"].as_i64()?;
            (size > 0).then_some((start, start + size))
        })
        .collect())
}

/// The main image starts after the cartridge header.
const MAIN_START: i64 = ROM_BASE + 0xc0;

/// The main image's asset complement: every byte of a ROM of `rom_bytes`
/// after its header that no byte-reproduced asset region claims.
fn asset_complement(rom_bytes: i64, cuts: &[(i64, i64)]) -> Vec<(i64, i64)> {
    difference_ranges(&[(MAIN_START, ROM_BASE + rom_bytes)], cuts)
}

type OwnerEntries = std::collections::BTreeMap<String, std::collections::BTreeSet<i64>>;

/// Owner entry points by image: every owner the production translation units
/// link (canonical and instance placements) and every owner a retained
/// overlay listing bounds, by C placeholder or owner label. An owner entry is
/// code whatever its listing spells there, so the decoder walks from it.
fn owner_entries(root: &Path, target: DecompTarget) -> Result<OwnerEntries, String> {
    use crate::compiler::source_paths::SourceOwner;
    use crate::compiler::translation_units::TranslationUnits;
    let mut entries = OwnerEntries::new();
    let units = TranslationUnits::declared_game(root, target.compiler)?;
    for unit in &units.units {
        let canonical = unit.owners.iter().map(|owner| owner.address);
        entries
            .entry(unit.image().to_owned())
            .or_default()
            .extend(canonical.map(i64::from));
        for (image, instance) in &unit.instances {
            let placed = instance.owners.values().map(|owner| owner.address);
            entries
                .entry(image.clone())
                .or_default()
                .extend(placed.map(i64::from));
        }
    }
    for owner in crate::overlay::owners::owner_spans(root, target)?.into_keys() {
        if let SourceOwner::Overlay { resource, address } = owner {
            entries
                .entry(format!("resource_{resource:03x}"))
                .or_default()
                .insert(i64::from(address));
        }
    }
    Ok(entries)
}

fn failure_row(failure: &Failure) -> Value {
    json!({
        "entry": failure.entry,
        "run": failure.run,
        "at": failure.at,
        "reason": failure.reason,
    })
}

/// One code overlay's executable bytes: the Thumb code, literal pools and
/// switch tables control flow proves, fixed veneers, the stock interworking
/// bank, and the alignment between them. Retained listings are not evidence:
/// how a listing spells a byte does not make it code or data.
fn audit_overlay(
    target: DecompTarget,
    rom: &CanonicalRom,
    resource: usize,
    owners: &OwnerEntries,
) -> Result<(Value, u64, u64, u64), String> {
    let stream = rom.stream(resource)?;
    let id = format!("resource_{resource:03x}");
    let image = &stream.decoded;
    let entries = owners
        .get(&id)
        .map(|entries| entries.iter().copied().collect::<Vec<_>>())
        .unwrap_or_default();
    let code = overlay_code(image, OVERLAY_BASE, target.overlay_entry_veneers, &entries)
        .map_err(|error| format!("{id}: {error}"))?;
    let mut spans = code.spans.clone();
    spans.extend(veneer_spans(image, OVERLAY_BASE));
    spans.extend(compiler_runtime_spans(image, OVERLAY_BASE));
    let alignment = alignment_spans(image, OVERLAY_BASE, &spans);
    spans.extend(alignment);
    let executable = union_bytes(&spans);
    let decoded = image.len() as u64;
    let encoded = (stream.end - stream.start) as u64;
    let row = json!({
        "id": id,
        "decoded_bytes": decoded,
        "rom_start": ROM_BASE + stream.start as i64,
        "rom_end": ROM_BASE + stream.end as i64,
        "encoded_bytes": encoded,
        "executable_bytes": executable,
        "excluded_bytes": decoded - executable,
        "intervals": spans.into_iter().map(|span| json!({
            "start": span.start, "end": span.end, "kind": span.kind,
        })).collect::<Vec<_>>(),
        "proof": {
            "passes": code.passes,
            "unresolved_computed_jumps": code.unresolved_jumps,
            "conflicts": code.conflicts,
            "failed_runs": code.failed_runs.iter().map(failure_row).collect::<Vec<_>>(),
            "accepted_candidates": code.accepted.iter().map(|(seed, evidence)| json!({
                "seed": seed,
                "evidence": evidence.name(),
            })).collect::<Vec<_>>(),
            "functions": code.functions.iter().map(|(entry, evidence)| json!([entry, evidence.name()])).collect::<Vec<_>>(),
            "word_far_jumps": code.far_jumps.iter().map(|(site, label)| json!([site, label])).collect::<Vec<_>>(),
            "rejected_owner_entries": code.rejected_owners.iter().map(|(entry, reason)| json!({
                "entry": entry,
                "reason": reason,
            })).collect::<Vec<_>>(),
            "rejected_candidates": code.rejected.iter().map(|rejection| json!({
                "seed": rejection.seed,
                "evidence": rejection.evidence.name(),
                "failure": failure_row(&rejection.failure),
            })).collect::<Vec<_>>(),
        },
    });
    Ok((row, decoded, encoded, executable))
}

fn report(root: &Path, target: DecompTarget) -> Result<Value, String> {
    let rom = CanonicalRom::load_target(root, target)?;
    let resources = rom.overlay_resources(target.overlay_entry_veneers);
    let code_resources = resources
        .iter()
        .copied()
        .collect::<std::collections::BTreeSet<_>>();
    let resource_rows = (0..rom.resource_count())
        .map(|resource| {
            let pointer = rom.resource_pointer(resource)?;
            let stream = rom.stream(resource).ok();
            let role = if code_resources.contains(&resource) {
                "code-overlay"
            } else if stream.is_some() {
                "data-resource"
            } else {
                "raw-pointer"
            };
            let mut row = json!({
                "id": format!("resource_{resource:03x}"),
                "rom_start": ROM_BASE + pointer as i64,
                "role": role,
            });
            if let Some(stream) = stream {
                let encoded = stream.encoded()?;
                let end = stream.start + encoded.len();
                if rom.bytes().get(stream.start..end) != Some(encoded.as_slice()) {
                    return Err(format!(
                        "resource_{resource:03x} does not reproduce its physical encoding"
                    ));
                }
                row["rom_start"] = json!(ROM_BASE + stream.start as i64);
                row["rom_end"] = json!(ROM_BASE + end as i64);
                row["encoded_bytes"] = json!(end - stream.start);
            }
            Ok(row)
        })
        .collect::<Result<Vec<_>, String>>()?;
    let owners = owner_entries(root, target)?;
    // Discovery invokes short-lived assembler/objdump processes and spends
    // most of its time outside Rust. Ask the OS directly because some
    // sandboxes under-report through Rust's affinity-aware API.
    let workers = crate::parallel::workers(resources.len());
    eprintln!(
        "auditing {} overlays with {workers} workers",
        resources.len()
    );
    let next = std::sync::atomic::AtomicUsize::new(0);
    let results = std::sync::Mutex::new(Vec::with_capacity(resources.len()));
    std::thread::scope(|scope| {
        for _ in 0..workers {
            let next = &next;
            let results = &results;
            let resources = &resources;
            let rom = &rom;
            let owners = &owners;
            scope.spawn(move || loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                let Some(&resource) = resources.get(index) else {
                    break;
                };
                results
                    .lock()
                    .unwrap()
                    .push((index, audit_overlay(target, rom, resource, owners)));
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);
    let mut overlays = Vec::with_capacity(results.len());
    let mut decoded_total = 0u64;
    let mut encoded_total = 0u64;
    let mut executable_total = 0u64;
    for (_, result) in results {
        let (row, decoded, encoded, executable) = result?;
        overlays.push(row);
        decoded_total += decoded;
        encoded_total += encoded;
        executable_total += executable;
    }
    // This remains a candidate until every discovered code range and asset
    // complement round-trips. Scan the supplied image and subtract verified
    // asset regions; no committed address ledger supplies the answer.
    let main_end = ROM_BASE + rom.bytes().len() as i64;
    let main_start = MAIN_START;
    let main_image = &rom.bytes()[(main_start - ROM_BASE) as usize..(main_end - ROM_BASE) as usize];
    let (cuts, asset_manifest) = asset_cuts(root, target)?;
    let asset_complement = asset_complement(rom.bytes().len() as i64, &cuts)
        .into_iter()
        .map(|(start, end)| ExecutableSpan {
            start,
            end,
            kind: "asset_complement",
        })
        .collect::<Vec<_>>();
    let asset_complement_bytes = union_bytes(&asset_complement);
    let main_spans = subtract_spans(executable_spans(main_image, main_start)?, &cuts);
    let main_executable = union_bytes(&main_spans);
    let call_target_spans = subtract_spans(main_executable_spans(main_image, main_start)?, &cuts);
    let call_target_executable = union_bytes(&call_target_spans);
    Ok(json!({
        "format": "alchemy-executable-audit-v1",
        "target": target.id.as_str(),
        "rom_sha256": crate::compiler::sha256::hex(rom.bytes()),
        "state": "candidate",
        "scope": "code-overlays",
        "audit": "candidate",
        "resources": resource_rows,
        "overlays": overlays,
        "main": {
            "start": main_start,
            "end": main_end,
            "candidate_executable_bytes": main_executable,
            "intervals": main_spans.into_iter().map(|span| json!({
                "start": span.start,
                "end": span.end,
                "kind": span.kind,
            })).collect::<Vec<_>>(),
            "methods": {
                "prologue_sweep": {"candidate_executable_bytes": main_executable},
                "call_targets": {
                    "candidate_executable_bytes": call_target_executable,
                    "intervals": call_target_spans.into_iter().map(|span| json!({
                        "start": span.start,
                        "end": span.end,
                        "kind": span.kind,
                    })).collect::<Vec<_>>(),
                },
                "asset_complement": {
                    "candidate_executable_bytes": asset_complement_bytes,
                    "intervals": asset_complement.into_iter().map(|span| json!({
                        "start": span.start,
                        "end": span.end,
                        "kind": span.kind,
                    })).collect::<Vec<_>>(),
                },
            },
            "asset_cut_bytes": intersection_bytes(&cuts, &[(main_start, main_end)]),
            "asset_manifest": asset_manifest,
        },
        "summary": {
            "overlays": overlays.len(),
            "decoded_bytes": decoded_total,
            "encoded_bytes": encoded_total,
            "executable_bytes": executable_total,
            "main_candidate_executable_bytes": main_executable,
            "excluded_bytes": decoded_total - executable_total,
        },
        "derivation": [
            "resource-directory streams that decode to the target's fixed entry-veneer shape",
            "only streams whose traced encoding reproduces the ROM receive physical byte extents; raw directory pointers are not files",
            "ARMv4T Thumb instructions reached by control flow from loader entry veneers, owner-register entries, framed word-aligned Thumb function pointers and calls from proved code, through branches, GCC far jumps (BLs to halfword boundaries or to word-aligned labels inside the caller's own walk) and GCC switch tables proved by their register chain",
            "owner-register entries on the zero alignment halfword after a return, or inside another function's straight-line code, are dropped and the proof repeated without them",
            "word-aligned framed prologues, the routine after the entry veneer table and word-aligned Thumb pointers proved code loads are candidates only: each is kept when every run of its walk and of every new function it calls ends in control flow, and discarded whole otherwise",
            "a gap between proved functions is kept only when complete candidate walks from its word boundaries explain every byte; a lone bx lr that no pointer word or BL names explains nothing, and a boundary a stored Thumb pointer names is recorded as stored_pointer",
            "PC-relative literal words and switch tables proved code reads, fixed ldr/bx veneers, the stock interworking bank, and zero halfwords aligning executable bytes"
        ]
    }))
}

/// How a candidate audit compares with a ledger, listing every differing
/// range so that a mismatch names its bytes rather than a total. A diagnostic:
/// the committed ledgers are known to be wrong in places and gate nothing.
struct Calibration {
    exact: bool,
    false_bytes: u64,
    missed_bytes: u64,
    text: String,
}

fn intervals(rows: &Value) -> Vec<(i64, i64)> {
    rows.as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| Some((row["start"].as_i64()?, row["end"].as_i64()?)))
        .collect()
}

fn listed(ranges: &[(i64, i64)]) -> String {
    ranges
        .iter()
        .map(|(start, end)| format!("0x{start:08x}-0x{end:08x}({})", end - start))
        .collect::<Vec<_>>()
        .join(",")
}

fn calibrate(document: &Value, expected: &ExpectedReport) -> Result<Calibration, String> {
    let expected_main = expected
        .main
        .intervals
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    let method = |intervals_of: &Value, total: &Value, name: &str| {
        let spans = intervals(intervals_of);
        let bytes = total
            .as_u64()
            .ok_or_else(|| format!("audit report has no main {name} byte total"))?;
        let overlap = intersection_bytes(&spans, &expected_main);
        Ok::<_, String>((
            spans,
            overlap,
            bytes.saturating_sub(overlap),
            expected.main.executable_bytes.saturating_sub(overlap),
        ))
    };
    let main = &document["main"];
    let (prologue, prologue_overlap, prologue_false, prologue_missed) = method(
        &main["intervals"],
        &main["candidate_executable_bytes"],
        "prologue-sweep",
    )?;
    let calls = &main["methods"]["call_targets"];
    let (_, call_overlap, call_false, call_missed) = method(
        &calls["intervals"],
        &calls["candidate_executable_bytes"],
        "call-target",
    )?;
    let complement = &main["methods"]["asset_complement"];
    let (_, complement_overlap, complement_false, complement_missed) = method(
        &complement["intervals"],
        &complement["candidate_executable_bytes"],
        "asset-complement",
    )?;
    let mut prologue_missed_by_kind = std::collections::BTreeMap::<&str, u64>::new();
    for span in &expected.main.intervals {
        let found = intersection_bytes(&prologue, &[(span.start, span.end)]);
        *prologue_missed_by_kind.entry(&span.kind).or_default() +=
            (span.end - span.start) as u64 - found;
    }
    let prologue_missed_by_kind = prologue_missed_by_kind
        .into_iter()
        .map(|(kind, bytes)| format!("{kind}={bytes}"))
        .collect::<Vec<_>>()
        .join(",");
    let actual = document["overlays"]
        .as_array()
        .ok_or("audit report has no overlays")?;
    let mut mismatches = Vec::new();
    let mut overlay_false = 0u64;
    let mut overlay_missed = 0u64;
    for row in &expected.overlays {
        let expected_spans = row
            .intervals
            .iter()
            .map(|span| (span.start, span.end))
            .collect::<Vec<_>>();
        let Some(found) = actual.iter().find(|actual| actual["id"] == row.id.as_str()) else {
            overlay_missed += row.executable_bytes;
            mismatches.push(format!(
                "{} expected {} got missing",
                row.id, row.executable_bytes
            ));
            continue;
        };
        let bytes = found["executable_bytes"].as_u64().unwrap_or(0);
        let found_spans = intervals(&found["intervals"]);
        let common = intersection_bytes(&found_spans, &expected_spans);
        let false_bytes = bytes.saturating_sub(common);
        let missed_bytes = row.executable_bytes.saturating_sub(common);
        overlay_false += false_bytes;
        overlay_missed += missed_bytes;
        if bytes != row.executable_bytes || false_bytes != 0 || missed_bytes != 0 {
            mismatches.push(format!(
                "{} expected {} got {} false {} [{}] missed {} [{}]",
                row.id,
                row.executable_bytes,
                bytes,
                false_bytes,
                listed(&difference_ranges(&found_spans, &expected_spans)),
                missed_bytes,
                listed(&difference_ranges(&expected_spans, &found_spans)),
            ));
        }
    }
    for found in actual {
        let id = found["id"].as_str().unwrap_or("");
        if !expected.overlays.iter().any(|row| row.id == id) {
            let bytes = found["executable_bytes"].as_u64().unwrap_or(0);
            overlay_false += bytes;
            mismatches.push(format!("{id} expected missing got {bytes}"));
        }
    }
    let exact = mismatches.is_empty() && complement_false == 0 && complement_missed == 0;
    let total = expected.main.executable_bytes;
    let mut text = vec![
        format!(
            "calibration={}: main asset-complement overlap {complement_overlap}/{total}, false-positive {complement_false}, missed {complement_missed}; prologue overlap {prologue_overlap}/{total}, false-positive {prologue_false}, missed {prologue_missed} ({prologue_missed_by_kind}); call/pointer overlap {call_overlap}/{total}, false-positive {call_false}, missed {call_missed}",
            if exact { "exact" } else { "inexact" }
        ),
        format!(
            "overlays {}: false-positive {overlay_false}, missed {overlay_missed}, mismatched {}/{}",
            actual.len(),
            mismatches.len(),
            expected.overlays.len()
        ),
    ];
    text.extend(mismatches);
    Ok(Calibration {
        exact,
        false_bytes: overlay_false + complement_false,
        missed_bytes: overlay_missed + complement_missed,
        text: text.join("\n"),
    })
}

/// Inventory kinds, most specific first. A byte several methods classify
/// takes the first kind that covers it, so the published intervals partition
/// each image instead of overlapping.
const KINDS: [(&str, &str); 7] = [
    (
        "veneer",
        "fixed ldr r4, [pc, #0]; bx r4 loader veneer whose target is addressable",
    ),
    (
        "compiler_runtime",
        "complete stock GCC Thumb interworking bank (_call_via_rX)",
    ),
    (
        "jump_table",
        "GCC switch table proved through its compare, scale, load and mov pc register chain",
    ),
    (
        "literal_pool",
        "word a pc-relative load of proven Thumb code reads",
    ),
    (
        "thumb",
        "ARMv4T Thumb instruction proved by control flow from loader veneers, owner-register entries, framed function pointers and calls, or by a candidate entry whose complete walk ends in control flow",
    ),
    (
        "executable_alignment",
        "zero halfword aligning code, a literal pool or a switch table between executable bytes",
    ),
    (
        "asset_complement",
        "canonical ROM after its header minus every byte-reproduced asset region",
    ),
];

/// Partition overlapping classified spans into disjoint intervals, each with
/// the most specific kind covering it.
fn partition(rows: &Value) -> Result<Vec<(i64, i64, &'static str)>, String> {
    let mut events = Vec::new();
    for row in rows.as_array().into_iter().flatten() {
        let (Some(start), Some(end)) = (row["start"].as_i64(), row["end"].as_i64()) else {
            return Err("audit interval has no integer bounds".into());
        };
        let kind = row["kind"].as_str().unwrap_or("");
        let rank = KINDS
            .iter()
            .position(|(name, _)| *name == kind)
            .ok_or_else(|| format!("audit interval has unknown kind {kind:?}"))?;
        if end > start {
            events.push((start, rank, 1i64));
            events.push((end, rank, -1i64));
        }
    }
    events.sort_unstable();
    let mut open = [0i64; KINDS.len()];
    let mut out: Vec<(i64, i64, &'static str)> = Vec::new();
    let mut cursor = None;
    for (at, rank, delta) in events {
        if let Some(from) = cursor.filter(|from| *from < at) {
            if let Some(kind) = open.iter().position(|count| *count > 0) {
                let kind = KINDS[kind].0;
                match out.last_mut() {
                    Some(last) if last.1 == from && last.2 == kind => last.1 = at,
                    _ => out.push((from, at, kind)),
                }
            }
        }
        open[rank] += delta;
        cursor = Some(at);
    }
    Ok(out)
}

fn published(spans: &[(i64, i64, &'static str)]) -> (u64, Vec<Value>) {
    let bytes = spans
        .iter()
        .map(|(start, end, _)| (end - start) as u64)
        .sum();
    let rows = spans
        .iter()
        .map(|(start, end, kind)| json!({"start": start, "end": end, "kind": kind, "evidence_ref": kind}))
        .collect();
    (bytes, rows)
}

/// The complete Full-C Byte Share inventory progress and coverage score
/// against, generated from a candidate audit whose overlay intervals are the
/// independently verified output and whose main complement is proven.
fn inventory(document: &Value, verification: Value) -> Result<Value, String> {
    let (main_bytes, main_rows) = published(&partition(
        &document["main"]["methods"]["asset_complement"]["intervals"],
    )?);
    let mut total = main_bytes;
    let overlays = document["overlays"]
        .as_array()
        .ok_or("audit report has no overlays")?
        .iter()
        .map(|row| {
            let (bytes, rows) = published(&partition(&row["intervals"])?);
            total += bytes;
            Ok(json!({
                "id": row["id"],
                "decoded_bytes": row["decoded_bytes"],
                "rom_start": row["rom_start"],
                "rom_end": row["rom_end"],
                "executable_bytes": bytes,
                "audit": "complete",
                "intervals": rows,
            }))
        })
        .collect::<Result<Vec<_>, String>>()?;
    Ok(json!({
        "format": 1,
        "metric": "full-c-byte-share",
        "target": document["target"],
        "rom_sha256": document["rom_sha256"],
        "state": "audited",
        "audit": "complete",
        "verification": verification,
        "overlay_count": overlays.len(),
        "total_union_bytes": total,
        "evidence": KINDS.iter().map(|(kind, evidence)| (kind.to_string(), json!(evidence))).collect::<serde_json::Map<_, _>>(),
        "main": {
            "id": "main",
            "audit": "complete",
            "executable_bytes": main_bytes,
            "intervals": main_rows,
        },
        "overlays": overlays,
    }))
}

/// The inventory while its audit is incomplete: scoring reports `?`.
fn pending_inventory(
    target: DecompTarget,
    rom_sha256: &Value,
    verification: Value,
    reasons: &[String],
) -> Value {
    json!({
        "format": 1,
        "metric": "full-c-byte-share",
        "target": target.id.as_str(),
        "rom_sha256": rom_sha256,
        "state": "pending",
        "audit": "incomplete",
        "pending": reasons,
        "verification": verification,
        "overlay_count": 0,
        "main": {"id": "main", "audit": "incomplete"},
        "overlays": [],
    })
}

fn write_json(path: &Path, value: &Value) -> Result<(), String> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent).map_err(|error| error.to_string())?;
    }
    std::fs::write(path, canonical_json(value))
        .map_err(|error| format!("{}: {error}", path.display()))
}

fn lexical(path: &Path) -> PathBuf {
    path.components()
        .filter(|component| !matches!(component, std::path::Component::CurDir))
        .collect()
}

/// A candidate report may go anywhere outside the checkout or under `out/`,
/// but never over a tracked file or any target's executable inventory.
fn candidate_destination(root: &Path, path: &Path) -> Result<PathBuf, String> {
    let path = lexical(&resolve(root, path));
    let root = lexical(root);
    let inventory = TARGET_IDS
        .into_iter()
        .any(|id| path == lexical(&inventory_path(&root, target_for(id))));
    if inventory || (path.starts_with(&root) && !path.starts_with(root.join("out"))) {
        return Err(format!(
            "{}: the candidate audit never replaces an executable inventory or a tracked file",
            path.display()
        ));
    }
    Ok(path)
}

/// An `--inventory` run with invalid arguments fails like any other: before
/// the error returns it withdraws the inventory of the target it names, or
/// every existing inventory when it names no valid target.
fn refuse_arguments(root: &Path, arguments: &[String], error: String) -> String {
    if !arguments.iter().any(|argument| argument == "--inventory") {
        return error;
    }
    let named = arguments
        .iter()
        .rposition(|argument| argument == "--target")
        .and_then(|index| arguments.get(index + 1))
        .and_then(|value| decomp_target(Some(value)).ok());
    let targets = match named {
        Some(target) => vec![target],
        None => TARGET_IDS.into_iter().map(target_for).collect(),
    };
    let reason = format!("the last --inventory run failed: {error}");
    for target in targets {
        let path = inventory_path(root, target);
        if !path.exists() {
            continue;
        }
        let pending = pending_inventory(target, &Value::Null, Value::Null, &[reason.clone()]);
        if let Err(failure) = write_json(&path, &pending) {
            return format!("{error}\nthe inventory was not withdrawn: {failure}");
        }
    }
    error
}

pub fn run(root: &Path, arguments: &[String]) -> Result<String, String> {
    let options = parse(arguments).map_err(|error| refuse_arguments(root, arguments, error))?;
    if options.help {
        return Ok(USAGE.into());
    }
    if options.all {
        let canonical = [DecompTargetId::TbsEn, DecompTargetId::TlaEn];
        let mut reports = Vec::with_capacity(TARGET_IDS.len());
        for id in canonical
            .into_iter()
            .chain(TARGET_IDS.into_iter().filter(|id| !canonical.contains(id)))
        {
            reports.push(index::run(root, target_for(id))?);
        }
        return Ok(reports.join("\n"));
    }
    let target = decomp_target(options.target.as_deref())
        .map_err(|error| refuse_arguments(root, arguments, error))?;
    if options.data {
        return index::run(root, target);
    }
    if options.inventory {
        return refresh_inventory(root, target);
    }
    let candidate = match &options.output {
        Some(path) => candidate_destination(root, path)?,
        None => candidate_path(root, target),
    };
    let (document, summary) = audit_candidate(root, target, &candidate)?;
    let Some(expected_path) = options.expected.as_deref().map(|path| resolve(root, path)) else {
        return Ok(summary);
    };
    let expected: ExpectedReport = serde_json::from_slice(
        &std::fs::read(&expected_path)
            .map_err(|error| format!("{}: {error}", expected_path.display()))?,
    )
    .map_err(|error| format!("{}: {error}", expected_path.display()))?;
    let calibration = calibrate(&document, &expected)?;
    let text = format!(
        "{summary}\nexpected={} false_bytes={} missed_bytes={}\n{}",
        expected_path
            .strip_prefix(root)
            .unwrap_or(&expected_path)
            .display(),
        calibration.false_bytes,
        calibration.missed_bytes,
        calibration.text
    );
    if calibration.exact {
        Ok(text)
    } else {
        Err(text)
    }
}

/// `--inventory`, which a canonical full build also runs when it leaves no
/// authoritative inventory: when none exists yet, or its own inputs (the
/// verified overlay record, the reference ROM or the build's asset layout)
/// changed.
pub(crate) fn refresh_inventory(root: &Path, target: DecompTarget) -> Result<String, String> {
    let candidate = candidate_path(root, target);
    record_inventory(root, target, || audit_candidate(root, target, &candidate))
}

/// Audits the target's ROM, writes the candidate report to `destination`
/// and summarizes it with the digest of its overlay intervals.
fn audit_candidate(
    root: &Path,
    target: DecompTarget,
    destination: &Path,
) -> Result<(Value, String), String> {
    let document = report(root, target)?;
    write_json(destination, &document)?;
    let summary = format!(
        "candidate={} target={} overlays={} executable={} decoded={} overlay_sha256={}",
        destination
            .strip_prefix(root)
            .unwrap_or(destination)
            .display(),
        target.id,
        document["summary"]["overlays"],
        document["summary"]["executable_bytes"],
        document["summary"]["decoded_bytes"],
        verification::overlay_digest(&document)?
    );
    Ok((document, summary))
}

/// `--inventory`: writes the inventory [`gated_inventory`] derives from the
/// candidate `audit` produces. The inventory is withdrawn to a pending one
/// before the audit starts and again when any step fails, so an earlier
/// authoritative inventory never outlives a run that did not finish.
fn record_inventory(
    root: &Path,
    target: DecompTarget,
    audit: impl FnOnce() -> Result<(Value, String), String>,
) -> Result<String, String> {
    let path = inventory_path(root, target);
    let withdraw = |reason: String| {
        write_json(
            &path,
            &pending_inventory(target, &Value::Null, Value::Null, &[reason]),
        )
    };
    withdraw("an --inventory run began and has not finished".into())?;
    let result = audit().and_then(|(document, summary)| {
        let (inventory, pending) = gated_inventory(root, target, &document)?;
        write_json(&path, &inventory)?;
        let shown = path.strip_prefix(root).unwrap_or(&path).display();
        Ok(if pending.is_empty() {
            format!(
                "{summary}\ninventory={shown} target={} state=audited executable={}",
                target.id, inventory["total_union_bytes"]
            )
        } else {
            format!(
                "{summary}\ninventory={shown} target={} state=pending: {}",
                target.id,
                pending.join("; ")
            )
        })
    });
    result.or_else(|error| {
        withdraw(format!("the last --inventory run failed: {error}"))
            .map_err(|failure| format!("{error}\nthe inventory was not withdrawn: {failure}"))?;
        Err(error)
    })
}

/// The inventory `--inventory` writes, with the reasons it is pending. The
/// overlays count when their intervals hash to the independently verified
/// digest; the main image when a byte-identical full ROM build proves its
/// asset complement. A game lacking either stays pending: its DONE is `?`.
/// A complete inventory must pass the readers' own validation.
fn gated_inventory(
    root: &Path,
    target: DecompTarget,
    document: &Value,
) -> Result<(Value, Vec<String>), String> {
    let overlays = verification::overlays(root, target, document)?;
    let main = main_image_proof(root, target, document);
    let mut provenance = overlays.provenance;
    provenance["main"] = if main.is_ok() {
        main_proof(target)
    } else {
        json!({"state": "unproven"})
    };
    let pending = overlays
        .pending
        .into_iter()
        .chain(main.err())
        .collect::<Vec<_>>();
    if !pending.is_empty() {
        let inventory = pending_inventory(target, &document["rom_sha256"], provenance, &pending);
        return Ok((inventory, pending));
    }
    let inventory = inventory(document, provenance)?;
    super::pipeline::validated_inventory(root, &inventory, target)?;
    Ok((inventory, pending))
}

/// The main image is the asset complement only when a byte-identical full
/// ROM build accounts for every byte: its verified asset regions, and
/// source for everything else. A partial asset build leaves unidentified
/// data inside the complement. The build must still prove it
/// ([`last_full_build`]) and have rebuilt the ROM this audit read.
fn main_image_proof(root: &Path, target: DecompTarget, document: &Value) -> Result<(), String> {
    let full = full_asset_manifest(target);
    if document["main"]["asset_manifest"].as_str() != Some(full.as_str()) {
        return Err(format!(
            "the main-image complement needs the byte-identical full ROM build's asset manifest {full}"
        ));
    }
    let (build, _) = last_full_build(root, target)
        .map_err(|error| format!("the main-image complement is unproven: {error}"))?;
    if document["rom_sha256"] != build.rom_sha256.as_str() {
        return Err(
            "the main-image complement is unproven: the audited ROM is not the one the full build reproduced"
                .into(),
        );
    }
    Ok(())
}

/// The main-image proof a complete inventory records, which readers require
/// verbatim: the asset complement of the byte-identical full ROM build whose
/// report and asset manifest it names.
fn main_proof(target: DecompTarget) -> Value {
    json!({
        "state": "proven",
        "proof": "byte-identical full ROM build",
        "build_report": full_build_report(target),
        "asset_manifest": full_asset_manifest(target),
    })
}

/// Whether a generated inventory is the automatic count, which every reader
/// requires before it scores one: overlay intervals that hash to the
/// independently verified digest, and main intervals that are the asset
/// complement of the last byte-identical full ROM build of the same ROM. Both
/// are recomputed from the inventory and the build's artifacts
/// ([`last_full_build`]), never taken from what the inventory or the build's
/// report states, so a copied ledger or a hand-made file is never scored.
/// The count depends on the reference ROM and the build's asset layout, not
/// on the source tree: it stays authoritative across source and tool changes
/// until a build changes that layout, starts or fails. `Err` says why the
/// inventory is not authoritative.
pub(crate) fn authenticate(
    root: &Path,
    target: DecompTarget,
    inventory: &Value,
) -> Result<(), String> {
    verification::authenticate(root, target, inventory)?;
    if inventory["verification"]["main"] != main_proof(target) {
        return Err(format!(
            "it records no main-image proof by the byte-identical full ROM build of {}",
            target.id
        ));
    }
    let (build, _) = last_full_build(root, target)
        .map_err(|error| format!("its main-image proof no longer holds: {error}"))?;
    if inventory["rom_sha256"] != build.rom_sha256.as_str() {
        return Err(
            "its main-image proof no longer holds: it audits a ROM other than the one the full build reproduced"
                .into(),
        );
    }
    let manifest = full_asset_manifest(target);
    let complement = asset_complement(
        target.rom_size as i64,
        &manifest_cuts(&build.asset_manifest, &manifest)?,
    );
    let main = partition(&inventory["main"]["intervals"])?;
    if main.iter().any(|(_, _, kind)| *kind != "asset_complement")
        || !main
            .iter()
            .map(|(start, end, _)| (*start, *end))
            .eq(complement)
    {
        return Err(format!(
            "its main intervals are not the asset complement of {manifest}"
        ));
    }
    Ok(())
}

/// Writes an inventory exactly as `--inventory` does, with every input it
/// needs to be authoritative: the verification record of its overlays and a
/// byte-identical full build whose asset regions leave `main` as the
/// complement. `target` must support a full build; a game that does not yet
/// is tested through [`super::proof::fully_buildable`]. Returns the
/// candidate audit and the inventory; readers' tests start from them and
/// break one input at a time. Write every tracked input first: inputs
/// written afterwards make the build's proof stale.
#[cfg(test)]
pub(crate) fn authoritative_fixture(
    root: &Path,
    target: DecompTarget,
    main: &[(i64, i64)],
    overlays: Value,
) -> (Value, Value) {
    let count = overlays.as_array().unwrap().len();
    let write = |path: &str, value: Value| {
        let path = root.join(path);
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        std::fs::write(path, canonical_json(&value)).unwrap();
    };
    let end = ROM_BASE + target.rom_size as i64;
    let regions = difference_ranges(&[(MAIN_START, end)], main)
        .into_iter()
        .map(|(start, end)| json!({"address": start, "size": end - start}))
        .collect::<Vec<_>>();
    let rom_sha256 =
        super::proof::full_build_fixture(root, target, &json!({"format": 1, "regions": regions}));
    let (decoded, executable) =
        overlays
            .as_array()
            .unwrap()
            .iter()
            .fold((0, 0), |(decoded, executable), row| {
                let bytes = partition(&row["intervals"])
                    .unwrap()
                    .iter()
                    .map(|(start, end, _)| end - start)
                    .sum::<i64>();
                (
                    decoded + row["decoded_bytes"].as_i64().unwrap(),
                    executable + bytes,
                )
            });
    let document = json!({
        "target": target.id.as_str(),
        "rom_sha256": rom_sha256,
        "overlays": overlays,
        "summary": {
            "decoded_bytes": decoded,
            "executable_bytes": executable,
            "excluded_bytes": decoded - executable,
        },
        "main": {
            "asset_manifest": full_asset_manifest(target),
            "methods": {"asset_complement": {"intervals": main.iter().map(|(start, end)| {
                json!({"start": start, "end": end, "kind": "asset_complement"})
            }).collect::<Vec<_>>()}},
        },
    });
    let mut record = verification::tests::record(&verification::overlay_digest(&document).unwrap());
    record["verified"]["target"] = json!(target.id.as_str());
    record["verified"]["overlays"] = json!(count);
    record["verified"]["decoded_bytes"] = json!(decoded);
    record["verified"]["executable_bytes"] = json!(executable);
    record["verified"]["excluded_bytes"] = json!(decoded - executable);
    record["residual"] =
        json!({"bytes": 0, "counted": false, "reason": "none in this fixture", "items": []});
    write(&verification::record_path(target), record);
    let (inventory, pending) = gated_inventory(root, target, &document).unwrap();
    assert!(pending.is_empty(), "{pending:?}");
    write_json(&inventory_path(root, target), &inventory).unwrap();
    (document, inventory)
}

#[cfg(test)]
mod tests {
    use super::{
        authoritative_fixture, calibrate, candidate_destination, difference_ranges,
        gated_inventory, intersection_bytes, main_image_proof, parse, partition, pending_inventory,
        record_inventory, union_bytes, ExpectedReport,
    };
    use crate::coverage::proof::fully_buildable;
    use crate::overlay::assembly::ExecutableSpan;
    use serde_json::json;
    use std::path::Path;

    fn arguments(text: &str) -> Vec<String> {
        text.split_whitespace().map(String::from).collect()
    }

    #[test]
    fn all_data_is_the_single_twelve_target_entry_point() {
        let options = parse(&["--all".into(), "--data".into()]).unwrap();
        assert!(options.all && options.data && options.target.is_none());
        assert!(parse(&["--all".into(), "--target".into(), "tbs-en".into()]).is_err());
        assert!(parse(&["--data".into()]).is_err());
    }

    #[test]
    fn only_calibration_reads_a_ledger() {
        assert!(parse(&arguments("--target tbs-en --calibrate")).is_err());
        assert!(parse(&arguments("--target tbs-en --expected a.json")).is_err());
        assert!(parse(&arguments("--target tla-en --inventory --expected a.json")).is_err());
        assert!(parse(&arguments(
            "--target tbs-en --calibrate --inventory --expected a.json"
        ))
        .is_err());
        assert!(parse(&arguments("--target tbs-en --inventory --output b.json")).is_err());
        assert!(parse(&arguments("--target tbs-en --data --expected a.json")).is_err());
        let options = parse(&arguments("--target tla-en --inventory")).unwrap();
        assert!(options.inventory && options.expected.is_none());
        let options = parse(&arguments("--target tbs-en --calibrate --expected a.json")).unwrap();
        assert!(options.calibrate && options.expected.is_some());
    }

    /// Overlays count only while they hash to the verified digest, the main
    /// image only when a byte-identical full build proves it, and the
    /// inventory is complete only when both hold.
    #[test]
    fn the_inventory_needs_verified_overlays_and_a_proven_main_image() {
        use super::verification::{overlay_digest, record_path, tests as record};
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        let mut document = record::document();
        document["rom_sha256"] = json!("00");
        document["main"] = candidate(json!({}))["main"].clone();
        document["main"]["asset_manifest"] = json!("out/tla-en/full/assets/manifest.json");
        let write = |path: &str, text: String| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let digest = overlay_digest(&document).unwrap();

        let (pending, reasons) = gated_inventory(root.path(), target, &document).unwrap();
        assert_eq!(reasons.len(), 2, "{reasons:?}");
        assert!(
            reasons[0].contains("no independent verification"),
            "{reasons:?}"
        );
        assert!(reasons[1].contains("main-image complement"), "{reasons:?}");
        assert_eq!(pending["state"], "pending");
        assert_eq!(pending["verification"]["overlay_sha256"], digest.as_str());
        assert_eq!(pending["verification"]["overlays"], "unverified");

        write(&record_path(target), record::record(&digest).to_string());
        let (pending, reasons) = gated_inventory(root.path(), target, &document).unwrap();
        assert_eq!(reasons.len(), 1, "{reasons:?}");
        assert!(reasons[0].contains("main-image complement"), "{reasons:?}");
        assert_eq!(pending["verification"]["overlays"], "verified");
        assert_eq!(
            pending["verification"]["main"],
            json!({"state": "unproven"})
        );
        assert!(
            crate::coverage::pipeline::validated_inventory(root.path(), &pending, target)
                .unwrap_err()
                .contains("withheld")
        );

        // A byte-identical full build whose asset regions leave exactly the
        // candidate's complement, 0x08000100..0x08000104, of the 16 MiB ROM.
        let rom = crate::coverage::proof::full_build_fixture(
            root.path(),
            target,
            &json!({"regions": [
                {"address": 0x080000c0, "size": 0x40},
                {"address": 0x08000104, "size": 0x0100_0000 - 0x104}
            ]}),
        );
        // It proves nothing about another ROM.
        let (_, reasons) = gated_inventory(root.path(), target, &document).unwrap();
        assert!(
            reasons[0].contains("not the one the full build reproduced"),
            "{reasons:?}"
        );
        document["rom_sha256"] = json!(rom);
        let (complete, reasons) = gated_inventory(root.path(), target, &document).unwrap();
        assert!(reasons.is_empty(), "{reasons:?}");
        assert_eq!(complete["state"], "audited");
        assert_eq!(complete["verification"]["verified_sha256"], digest.as_str());
        assert_eq!(complete["verification"]["runs"], json!(["wf_synthetic"]));
        assert_eq!(complete["verification"]["main"]["state"], "proven");
        assert_eq!(complete["total_union_bytes"], 28);

        // A failed rebuild withdraws the proof, and with it the main image.
        crate::coverage::proof::withdraw_full_build(root.path(), target).unwrap();
        let (pending, reasons) = gated_inventory(root.path(), target, &document).unwrap();
        assert_eq!(pending["state"], "pending");
        assert_eq!(reasons.len(), 1, "{reasons:?}");
        assert!(
            reasons[0].contains("cannot read out/tla-en/full/rebuilt.json"),
            "{reasons:?}"
        );
        crate::coverage::proof::full_build_fixture(
            root.path(),
            target,
            &json!({"regions": [
                {"address": 0x080000c0, "size": 0x40},
                {"address": 0x08000104, "size": 0x0100_0000 - 0x104}
            ]}),
        );

        // Any change to the method's output returns the overlays to pending.
        let mut changed = document.clone();
        changed["overlays"][0]["intervals"][0]["kind"] = json!("literal_pool");
        let (_, reasons) = gated_inventory(root.path(), target, &changed).unwrap();
        assert_eq!(reasons.len(), 1, "{reasons:?}");
        assert!(reasons[0].contains("re-verification"), "{reasons:?}");

        // Another edition's intervals are not the verified target's.
        let mut edition = document.clone();
        edition["target"] = json!("tla-ja");
        let edition_target = crate::targets::target_for(crate::targets::DecompTargetId::TlaJa);
        let (_, reasons) = gated_inventory(root.path(), edition_target, &edition).unwrap();
        assert!(
            reasons[0].contains("no independent verification of tla-ja"),
            "{reasons:?}"
        );

        // A record whose digest matches while its totals disagree is refused.
        let mut totals = record::record(&digest);
        totals["verified"]["executable_bytes"] = json!(25);
        totals["verified"]["excluded_bytes"] = json!(39);
        write(&record_path(target), totals.to_string());
        assert!(gated_inventory(root.path(), target, &document)
            .unwrap_err()
            .contains("recorded totals"));
    }

    #[test]
    fn the_candidate_never_replaces_an_inventory_or_tracked_file() {
        let root = Path::new("/repo");
        for path in [
            "out/tbs-en/reports/executable.json",
            "./out/tla-en/reports/executable.json",
            "/repo/out/tbs-ja/reports/executable.json",
            "games/THE BROKEN SEAL/metrics/executable.json",
        ] {
            assert!(
                candidate_destination(root, Path::new(path)).is_err(),
                "{path}"
            );
        }
        assert_eq!(
            candidate_destination(root, Path::new("out/work/audit.json")).unwrap(),
            Path::new("/repo/out/work/audit.json")
        );
        assert!(candidate_destination(root, Path::new("/tmp/audit.json")).is_ok());
    }

    #[test]
    fn overlapping_kinds_are_counted_once() {
        let spans = [
            ExecutableSpan {
                start: 0,
                end: 8,
                kind: "veneer",
            },
            ExecutableSpan {
                start: 0,
                end: 4,
                kind: "thumb",
            },
            ExecutableSpan {
                start: 10,
                end: 12,
                kind: "thumb",
            },
        ];
        assert_eq!(union_bytes(&spans), 10);
    }

    #[test]
    fn calibration_counts_overlap_without_double_counting() {
        assert_eq!(
            intersection_bytes(&[(0, 8), (4, 12)], &[(2, 6), (10, 14)]),
            6
        );
    }

    fn candidate(overlay: serde_json::Value) -> serde_json::Value {
        json!({
            "target": "tla-en",
            "rom_sha256": "00",
            "main": {
                "candidate_executable_bytes": 4,
                "intervals": [{"start": 0x08000100, "end": 0x08000104, "kind": "thumb"}],
                "methods": {
                    "call_targets": {"candidate_executable_bytes": 0, "intervals": []},
                    "asset_complement": {
                        "candidate_executable_bytes": 4,
                        "intervals": [{"start": 0x08000100, "end": 0x08000104, "kind": "asset_complement"}]
                    }
                }
            },
            "overlays": [overlay]
        })
    }

    fn expected() -> ExpectedReport {
        serde_json::from_value(json!({
            "main": {"executable_bytes": 4, "intervals": [
                {"start": 0x08000100, "end": 0x08000104, "kind": "thumb"}
            ]},
            "overlays": [{"id": "resource_001", "executable_bytes": 12, "intervals": [
                {"start": 0x02000000, "end": 0x0200000c, "kind": "thumb"}
            ]}]
        }))
        .unwrap()
    }

    #[test]
    fn calibration_names_every_differing_range() {
        let exact = candidate(json!({
            "id": "resource_001", "executable_bytes": 12,
            "intervals": [
                {"start": 0x02000000, "end": 0x02000008, "kind": "veneer"},
                {"start": 0x02000004, "end": 0x0200000c, "kind": "thumb"}
            ]
        }));
        assert!(calibrate(&exact, &expected()).unwrap().exact);
        let shifted = candidate(json!({
            "id": "resource_001", "executable_bytes": 12,
            "intervals": [{"start": 0x02000002, "end": 0x0200000e, "kind": "thumb"}]
        }));
        let result = calibrate(&shifted, &expected()).unwrap();
        assert!(!result.exact);
        assert_eq!((result.false_bytes, result.missed_bytes), (2, 2));
        assert!(result.text.contains("[0x0200000c-0x0200000e(2)]"));
        assert!(result.text.contains("[0x02000000-0x02000002(2)]"));
        let mut extra = shifted.clone();
        extra["overlays"] = json!([exact["overlays"][0], {"id": "resource_002", "executable_bytes": 2, "intervals": []}]);
        assert!(!calibrate(&extra, &expected()).unwrap().exact);
    }

    #[test]
    fn partition_keeps_the_most_specific_kind_without_overlap() {
        let rows = json!([
            {"start": 0, "end": 12, "kind": "thumb"},
            {"start": 4, "end": 8, "kind": "literal_pool"},
            {"start": 12, "end": 14, "kind": "executable_alignment"},
            {"start": 12, "end": 20, "kind": "veneer"},
            {"start": 24, "end": 26, "kind": "thumb"}
        ]);
        assert_eq!(
            partition(&rows).unwrap(),
            vec![
                (0, 4, "thumb"),
                (4, 8, "literal_pool"),
                (8, 12, "thumb"),
                (12, 20, "veneer"),
                (24, 26, "thumb")
            ]
        );
        assert!(partition(&json!([{"start": 0, "end": 2, "kind": "guess"}])).is_err());
    }

    fn overlay() -> serde_json::Value {
        json!({
            "id": "resource_001", "decoded_bytes": 16, "executable_bytes": 12,
            "rom_start": 0x08100000, "rom_end": 0x08100010,
            "intervals": [
                {"start": 0x02000000, "end": 0x02000008, "kind": "veneer"},
                {"start": 0x02000004, "end": 0x0200000c, "kind": "thumb"}
            ]
        })
    }

    #[test]
    fn generated_inventory_is_what_scoring_validates() {
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        let (_, complete) = authoritative_fixture(
            root.path(),
            target,
            &[(0x08000100, 0x08000104)],
            json!([overlay()]),
        );
        let (main, overlays) =
            crate::coverage::pipeline::validated_inventory(root.path(), &complete, target).unwrap();
        assert_eq!(crate::coverage::model::bytes(&main), 4);
        assert_eq!(crate::coverage::model::bytes(&overlays["resource_001"]), 12);
        assert_eq!(complete["total_union_bytes"], 16);
        assert_eq!(complete["overlays"][0]["intervals"][0]["kind"], "veneer");
        let pending = pending_inventory(target, &json!("00"), json!({}), &["reason".into()]);
        assert!(
            crate::coverage::pipeline::validated_inventory(root.path(), &pending, target)
                .unwrap_err()
                .contains("withheld")
        );
    }

    /// Every way an --inventory run can fail, including an audit that never
    /// produces a candidate, leaves a pending inventory in place of an
    /// earlier authoritative one before the error returns.
    #[test]
    fn a_failed_inventory_run_withdraws_an_authoritative_inventory() {
        use super::verification::record_path;
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEn);
        let establish = || {
            authoritative_fixture(
                root.path(),
                target,
                &[(0x08000100, 0x08000104)],
                json!([overlay()]),
            )
            .0
        };
        let authoritative = || {
            crate::coverage::pipeline::authoritative_inventory(root.path(), target)
                .unwrap()
                .is_some()
        };
        let withdrawn = |error: &str| {
            let path = root.path().join("out/tla-en/reports/executable.json");
            let inventory: serde_json::Value =
                serde_json::from_slice(&std::fs::read(path).unwrap()).unwrap();
            assert_eq!(inventory["state"], "pending");
            let reason = inventory["pending"][0].as_str().unwrap();
            assert!(reason.contains(error), "{reason}");
            assert!(!authoritative());
        };

        // The audit itself fails: this root's ROM is no ROM of the game.
        establish();
        assert!(authoritative());
        let error = super::run(root.path(), &arguments("--target tla-en --inventory")).unwrap_err();
        withdrawn(&error);

        // Its arguments are invalid, whether the target is named or not.
        for invalid in [
            "--target tla-en --inventory --output x.json",
            "--target tla-en --inventory --calibrate",
            "--target tla-en --inventory --bogus",
            "--target tla-en --inventory --data",
            "--target tla-xx --inventory",
            "--inventory --all",
            "--inventory",
        ] {
            establish();
            assert!(authoritative(), "{invalid}");
            let error = super::run(root.path(), &arguments(invalid)).unwrap_err();
            withdrawn(&error);
        }
        // An invalid run without --inventory leaves the inventory alone.
        establish();
        super::run(root.path(), &arguments("--target tla-en --bogus")).unwrap_err();
        assert!(authoritative());

        // The digest matches but the record's totals do not.
        let document = establish();
        assert!(authoritative());
        let record = root.path().join(record_path(target));
        let text = std::fs::read_to_string(&record).unwrap();
        let mut totals: serde_json::Value = serde_json::from_str(&text).unwrap();
        totals["verified"]["executable_bytes"] = json!(11);
        totals["verified"]["excluded_bytes"] = json!(5);
        std::fs::write(&record, totals.to_string()).unwrap();
        let candidate = || Ok((document.clone(), String::new()));
        let error = record_inventory(root.path(), target, candidate).unwrap_err();
        assert!(error.contains("recorded totals"), "{error}");
        withdrawn(&error);

        // The verification record is malformed.
        establish();
        std::fs::write(&record, "{").unwrap();
        let error = record_inventory(root.path(), target, candidate).unwrap_err();
        withdrawn(&error);

        // A run that completes writes the authoritative inventory again.
        establish();
        let summary = record_inventory(root.path(), target, candidate).unwrap();
        assert!(summary.contains("state=audited executable=16"), "{summary}");
        assert!(authoritative());
    }

    /// The main image needs the full build's proof, recomputed from its
    /// artifacts: the flags of a hand-written report, or any artifact
    /// changed after the build, prove nothing.
    #[test]
    fn main_complement_needs_a_byte_identical_full_rom_build() {
        let root = tempfile::tempdir().unwrap();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        let mut document = candidate(json!({}));
        document["target"] = json!("tbs-en");
        document["main"]["asset_manifest"] = json!("out/tbs-en/assets/manifest.json");
        assert!(main_image_proof(root.path(), target, &document).is_err());
        document["main"]["asset_manifest"] = json!("out/tbs-en/full/assets/manifest.json");
        assert!(main_image_proof(root.path(), target, &document).is_err());
        let rom = crate::coverage::proof::full_build_fixture(
            root.path(),
            target,
            &json!({"regions": [{"address": 0x08000104, "size": 0x7f_fefc}]}),
        );
        document["rom_sha256"] = json!(rom);
        main_image_proof(root.path(), target, &document).unwrap();
        let unproven = |reason: &str| {
            let error = main_image_proof(root.path(), target, &document).unwrap_err();
            assert!(error.contains(reason), "{error}");
        };
        let path = |relative: &str| root.path().join(relative);
        let report = std::fs::read(path("out/tbs-en/full/rebuilt.json")).unwrap();

        // The report's flags, hand-written without the proof the build records.
        std::fs::write(
            path("out/tbs-en/full/rebuilt.json"),
            r#"{"format":1,"target":"tbs-en","verification":"rom","byte_identical":true,"unowned_bytes":0,"rom_fallback_bytes":0}"#,
        )
        .unwrap();
        unproven("records no main-image proof");
        // A proof whose digests were copied, beside ROM fallback bytes.
        let mut forged: serde_json::Value = serde_json::from_slice(&report).unwrap();
        forged["rom_fallback_bytes"] = json!(8);
        std::fs::write(path("out/tbs-en/full/rebuilt.json"), forged.to_string()).unwrap();
        unproven("zero rom_fallback_bytes");
        std::fs::write(path("out/tbs-en/full/rebuilt.json"), &report).unwrap();
        main_image_proof(root.path(), target, &document).unwrap();

        // A rebuilt ROM that differs from the one the build recorded, or
        // that is missing.
        std::fs::write(path("out/tbs-en/full/rebuilt.gba"), b"another ROM").unwrap();
        unproven("is not the ROM the build recorded");
        std::fs::remove_file(path("out/tbs-en/full/rebuilt.gba")).unwrap();
        unproven("cannot read out/tbs-en/full/rebuilt.gba");
        // A proof rewritten to name the rebuilt ROM, which is not the
        // registered reference ROM.
        std::fs::write(path("out/tbs-en/full/rebuilt.gba"), b"another ROM").unwrap();
        let mut forged: serde_json::Value = serde_json::from_slice(&report).unwrap();
        forged["main_image_proof"]["rom_sha256"] =
            json!(crate::compiler::sha256::hex(b"another ROM"));
        std::fs::write(path("out/tbs-en/full/rebuilt.json"), forged.to_string()).unwrap();
        unproven("is not the reference ROM recon/tbs/text.json registers");
        // Nor does a local ROM replaced by that one vouch for it.
        let reference = std::fs::read(path("roms/tbs-en.gba")).unwrap();
        std::fs::write(path("roms/tbs-en.gba"), b"another ROM").unwrap();
        unproven("is not the reference ROM recon/tbs/text.json registers");
        std::fs::write(path("out/tbs-en/full/rebuilt.json"), &report).unwrap();
        std::fs::write(path("out/tbs-en/full/rebuilt.gba"), &reference).unwrap();
        unproven("is not the local reference ROM roms/tbs-en.gba");
        std::fs::write(path("roms/tbs-en.gba"), &reference).unwrap();
        main_image_proof(root.path(), target, &document).unwrap();

        // An asset manifest edited after the build, such as a partial one.
        let manifest = std::fs::read(path("out/tbs-en/full/assets/manifest.json")).unwrap();
        std::fs::write(
            path("out/tbs-en/full/assets/manifest.json"),
            r#"{"regions":[]}"#,
        )
        .unwrap();
        unproven("is not the asset manifest the build recorded");
        std::fs::write(path("out/tbs-en/full/assets/manifest.json"), &manifest).unwrap();

        // An encoder, codec, machine definition, retained listing or asset
        // changed after the build leaves the layout that build proved, so the
        // complement stands; the build no longer proves the tree, so it can
        // credit nothing until it runs again.
        for input in [
            "tools/alchemy/src/build_assets/packer.rs",
            "tools/alchemy/src/build_assets.rs",
            "tools/psynergy/src/assets/lz.rs",
            "recon/tbs/machine.json",
            "recon/tbs/assets.json",
            "recon/tbs/raw/overlays/resource_001_overlay.s",
            "games/THE BROKEN SEAL/SRC/FIELD/COMMON/MAP.JSON",
        ] {
            std::fs::create_dir_all(path(input).parent().unwrap()).unwrap();
            std::fs::write(path(input), "changed").unwrap();
            main_image_proof(root.path(), target, &document).unwrap();
            let stale = crate::coverage::proof::full_build(root.path(), target).map(|_| ());
            assert!(
                stale
                    .unwrap_err()
                    .contains("build inputs changed after the build"),
                "{input}"
            );
            std::fs::remove_file(path(input)).unwrap();
            assert!(crate::coverage::proof::full_build(root.path(), target).is_ok());
        }
    }

    /// An edition without a supported full ROM build proves no main image,
    /// whatever files claim one.
    #[test]
    fn an_unsupported_full_build_proves_no_main_image() {
        let root = tempfile::tempdir().unwrap();
        let edition = crate::targets::target_for(crate::targets::DecompTargetId::TlaJa);
        assert_ne!(edition.build_support, crate::targets::BuildSupport::Full);
        let mut document = candidate(json!({}));
        document["main"]["asset_manifest"] = json!("out/tla-ja/full/assets/manifest.json");
        document["rom_sha256"] = json!(crate::coverage::proof::full_build_fixture(
            root.path(),
            fully_buildable(edition),
            &json!({"regions": [{"address": 0x08000104, "size": 0xff_fefc}]}),
        ));
        main_image_proof(root.path(), fully_buildable(edition), &document).unwrap();
        let error = main_image_proof(root.path(), edition, &document).unwrap_err();
        assert!(error.contains("no supported full ROM build"), "{error}");
    }

    #[test]
    fn asset_complement_splits_at_every_verified_region() {
        assert_eq!(
            difference_ranges(&[(0, 20)], &[(2, 4), (8, 12), (10, 16)]),
            vec![(0, 2), (4, 8), (16, 20)]
        );
    }
}
