// Canonical Full-C Byte Share metric.
//
// The denominator is an audited union of executable intervals, not a ROM-size,
// file-size, function-count, or discovery-count proxy. The numerator is the
// union of byte-identical C-owned intervals from the normal claimed build and
// verified code-overlay placeholders. Every address space is unioned
// independently.
//
// Native implementation; every deliberate compatibility choice carries a
// PORT NOTE.

mod intervals;
mod js;
mod json;
mod overlay;
mod sha256;

use std::collections::HashSet;
use std::path::{Path, PathBuf};
use std::process::Command;

use intervals::{
    commas, format_subject, interval_bytes, merge_classified, overlap_diagnostics,
    round_half_up_percent, to_fixed_2, union_intervals, Interval,
};
use json::{canonical_json, Value};

const DEFAULT_TARGET: &str = "gs1-en";

/// `dirname(dirname(dirname(...)))` of the TypeScript's own path.
///
/// PORT NOTE: the TypeScript derives ROOT from `import.meta.url`, which is the
/// source file. A release binary lives in `target/`, so the equivalent anchor is
/// the crate manifest directory baked in at compile time: `tools-rs/full-c-progress`
/// -> repository root.
fn root() -> PathBuf {
    let manifest = Path::new(env!("CARGO_MANIFEST_DIR"));
    manifest.parent().and_then(Path::parent).expect("crate lives two levels below ROOT").to_path_buf()
}

// ---------------------------------------------------------------------------
// Targets (from decomp_targets.ts)
// ---------------------------------------------------------------------------

fn output_dir(target: &str) -> &'static str {
    match target {
        "gs2-en" => "out/gs2-en",
        _ => "out",
    }
}

fn parse_decomp_target(value: Option<&str>) -> Result<String, String> {
    match value {
        Some("gs1-en") => Ok("gs1-en".to_string()),
        Some("gs2-en") => Ok("gs2-en".to_string()),
        // `JSON.stringify(undefined)` is `undefined`, so a trailing `--target`
        // produces an unquoted word where a bad string produces a quoted one.
        Some(other) => Err(format!(
            "unsupported decomp target {}; expected gs1-en or gs2-en",
            json::quote(other)
        )),
        None => Err("unsupported decomp target undefined; expected gs1-en or gs2-en".to_string()),
    }
}

// ---------------------------------------------------------------------------
// Model
// ---------------------------------------------------------------------------

#[derive(Debug, Clone)]
pub struct Namespace {
    pub id: String,
    pub decoded_bytes: Option<f64>,
    pub executable_bytes: f64,
    pub excluded_bytes: Option<f64>,
    pub audit: String,
    pub intervals: Vec<Interval>,
    pub evidence: Vec<String>,
}

#[derive(Debug, Clone)]
pub struct Inventory {
    pub target: String,
    pub derivation: String,
    pub audit: String,
    pub total_union_bytes: f64,
    pub main: Namespace,
    pub overlays: Vec<Namespace>,
    pub caveats: Option<Vec<String>>,
}

#[derive(Debug, Clone)]
pub struct OwnedSpan {
    pub namespace: String,
    pub start: f64,
    pub end: f64,
    pub owner: String,
}

#[derive(Debug, Clone)]
pub struct ProgressReport {
    pub target: String,
    pub full_c_bytes: f64,
    pub executable_bytes: f64,
    pub remaining_bytes: f64,
    pub percent: f64,
    pub main_full_c_bytes: f64,
    pub main_executable_bytes: f64,
    pub overlays_full_c_bytes: f64,
    pub overlays_executable_bytes: f64,
}

// ---------------------------------------------------------------------------
// JSON <-> model
// ---------------------------------------------------------------------------

fn read_json(path: &Path) -> Result<Value, String> {
    if !path.exists() {
        return Err(format!("missing {}", path.display()));
    }
    json::parse(&overlay::read_lossy(path)?)
}

fn number_field(value: &Value, key: &str, label: &str) -> Result<f64, String> {
    let raw = value.get(key).and_then(Value::as_f64);
    intervals::integer(raw.unwrap_or(f64::NAN), label)
}

fn optional_number(value: &Value, key: &str) -> Option<f64> {
    match value.get(key) {
        Some(Value::Num(number)) => Some(*number),
        _ => None,
    }
}

fn string_field(value: &Value, key: &str) -> String {
    value.get(key).and_then(Value::as_str).unwrap_or_default().to_string()
}

fn namespace_from_json(value: &Value) -> Result<Namespace, String> {
    let mut parsed = Vec::new();
    if let Some(items) = value.get("intervals").and_then(Value::as_array) {
        for item in items {
            parsed.push(Interval {
                start: item.get("start").and_then(Value::as_f64).unwrap_or(f64::NAN),
                end: item.get("end").and_then(Value::as_f64).unwrap_or(f64::NAN),
                kind: string_field(item, "kind"),
                evidence: string_field(item, "evidence"),
            });
        }
    }
    Ok(Namespace {
        id: string_field(value, "id"),
        decoded_bytes: optional_number(value, "decoded_bytes"),
        executable_bytes: value
            .get("executable_bytes")
            .and_then(Value::as_f64)
            .unwrap_or(f64::NAN),
        excluded_bytes: optional_number(value, "excluded_bytes"),
        audit: string_field(value, "audit"),
        intervals: parsed,
        evidence: value
            .get("evidence")
            .and_then(Value::as_array)
            .map(|items| {
                items.iter().map(|item| item.as_str().unwrap_or_default().to_string()).collect()
            })
            .unwrap_or_default(),
    })
}

fn inventory_from_json(value: &Value) -> Result<Inventory, String> {
    // `format !== 1 || metric !== "full-c-byte-share"` is checked by
    // `validate_inventory`; a missing field simply fails that check, as in JS.
    let format_ok = value.get("format").and_then(Value::as_f64) == Some(1.0);
    let metric_ok = value.get("metric").and_then(Value::as_str) == Some("full-c-byte-share");
    if !format_ok || !metric_ok {
        return Err("unsupported executable inventory format".to_string());
    }
    let main = value
        .get("main")
        .ok_or_else(|| "unsupported executable inventory format".to_string())
        .and_then(namespace_from_json)?;
    let mut overlays = Vec::new();
    if let Some(items) = value.get("overlays").and_then(Value::as_array) {
        for item in items {
            overlays.push(namespace_from_json(item)?);
        }
    }
    Ok(Inventory {
        target: string_field(value, "target"),
        derivation: string_field(value, "derivation"),
        audit: string_field(value, "audit"),
        total_union_bytes: value
            .get("total_union_bytes")
            .and_then(Value::as_f64)
            .unwrap_or(f64::NAN),
        main,
        overlays,
        caveats: value.get("caveats").and_then(Value::as_array).map(|items| {
            items.iter().map(|item| item.as_str().unwrap_or_default().to_string()).collect()
        }),
    })
}

fn interval_json(interval: &Interval) -> Value {
    json::obj(vec![
        ("start", Value::Num(interval.start)),
        ("end", Value::Num(interval.end)),
        ("kind", json::string(&interval.kind)),
        ("evidence", json::string(&interval.evidence)),
    ])
}

fn namespace_json(namespace: &Namespace) -> Value {
    // Key order follows the TypeScript object literals exactly: the main image
    // omits decoded/excluded bytes, overlays place them right after `id`.
    let mut members: Vec<(&str, Value)> = vec![("id", json::string(&namespace.id))];
    if let Some(decoded) = namespace.decoded_bytes {
        members.push(("decoded_bytes", Value::Num(decoded)));
    }
    members.push(("executable_bytes", Value::Num(namespace.executable_bytes)));
    if let Some(excluded) = namespace.excluded_bytes {
        members.push(("excluded_bytes", Value::Num(excluded)));
    }
    members.push(("audit", json::string(&namespace.audit)));
    members.push((
        "intervals",
        Value::Arr(namespace.intervals.iter().map(interval_json).collect()),
    ));
    members.push((
        "evidence",
        Value::Arr(namespace.evidence.iter().map(|text| json::string(text)).collect()),
    ));
    json::obj(members)
}

fn inventory_json(inventory: &Inventory) -> Value {
    let mut members: Vec<(&str, Value)> = vec![
        ("format", json::num(1)),
        ("metric", json::string("full-c-byte-share")),
        ("target", json::string(&inventory.target)),
        ("derivation", json::string(&inventory.derivation)),
        ("audit", json::string(&inventory.audit)),
        ("total_union_bytes", Value::Num(inventory.total_union_bytes)),
        ("main", namespace_json(&inventory.main)),
        (
            "overlays",
            Value::Arr(inventory.overlays.iter().map(namespace_json).collect()),
        ),
    ];
    if let Some(caveats) = &inventory.caveats {
        members.push((
            "caveats",
            Value::Arr(caveats.iter().map(|text| json::string(text)).collect()),
        ));
    }
    json::obj(members)
}

fn report_json(report: &ProgressReport) -> Value {
    json::obj(vec![
        ("format", json::num(1)),
        ("metric", json::string("full-c-byte-share")),
        ("target", json::string(&report.target)),
        ("full_c_bytes", Value::Num(report.full_c_bytes)),
        ("executable_bytes", Value::Num(report.executable_bytes)),
        ("remaining_bytes", Value::Num(report.remaining_bytes)),
        ("percent", Value::Num(report.percent)),
        (
            "main",
            json::obj(vec![
                ("full_c_bytes", Value::Num(report.main_full_c_bytes)),
                ("executable_bytes", Value::Num(report.main_executable_bytes)),
            ]),
        ),
        (
            "overlays",
            json::obj(vec![
                ("full_c_bytes", Value::Num(report.overlays_full_c_bytes)),
                ("executable_bytes", Value::Num(report.overlays_executable_bytes)),
            ]),
        ),
        ("audit", json::string("complete")),
    ])
}

// ---------------------------------------------------------------------------
// Main image inventory
// ---------------------------------------------------------------------------

fn main_kind(kind: &str) -> &'static str {
    if kind == "alignment_padding" {
        return "executable_alignment";
    }
    if kind == "literal_pool" {
        return "literal_pool";
    }
    if kind.contains("veneer") || kind.contains("thunk") {
        return "veneer";
    }
    if kind.contains("arm") || kind == "gba_arm_entry" {
        return "arm";
    }
    if kind == "compiler_output" {
        return "thumb";
    }
    "structural"
}

/// `String(value)` for the values a manifest can hold here.
fn js_string(value: Option<&Value>) -> String {
    match value {
        Some(Value::Str(text)) => text.clone(),
        Some(Value::Num(number)) => json::number(*number),
        Some(Value::Bool(flag)) => flag.to_string(),
        Some(Value::Null) => "null".to_string(),
        Some(_) => "[object Object]".to_string(),
        None => "undefined".to_string(),
    }
}

fn regions_of(manifest: &Value) -> &[Value] {
    manifest.get("regions").and_then(Value::as_array).unwrap_or(&[])
}

fn main_inventory(root: &Path, target: &str) -> Result<Namespace, String> {
    let output = root.join(output_dir(target)).join("full");
    let asm_path = output.join("asm").join("manifest.json");
    let asm_manifest = if asm_path.exists() {
        read_json(&asm_path)?
    } else {
        json::obj(vec![("regions", Value::Arr(Vec::new()))])
    };
    let claimed_manifest = read_json(&output.join("claimed").join("manifest.json"))?;

    let mut intervals: Vec<Interval> = Vec::new();
    for region in regions_of(&asm_manifest) {
        let address = number_field(region, "address", "assembly region address")?;
        let size = number_field(region, "size", "assembly region size")?;
        let kind = region.get("kind");
        intervals.push(Interval {
            start: address,
            end: address + size,
            kind: main_kind(&match kind {
                Some(value) => js_string(Some(value)),
                None => "structural".to_string(),
            })
            .to_string(),
            evidence: format!(
                "{}:{}",
                js_string(region.get("source")),
                match kind {
                    Some(value) => js_string(Some(value)),
                    None => "classified-assembly".to_string(),
                }
            ),
        });
    }
    for region in regions_of(&claimed_manifest) {
        let address = number_field(region, "address", "C region address")?;
        let size = number_field(region, "size", "C region size")?;
        intervals.push(Interval {
            start: address,
            end: address + size,
            kind: "thumb".to_string(),
            evidence: format!("{}:byte-identical-claimed-C", js_string(region.get("source"))),
        });
    }
    let diagnostics = overlap_diagnostics(&intervals)?;
    if !diagnostics.is_empty() {
        return Err(format!("main executable ownership overlaps:\n{}", diagnostics.join("\n")));
    }
    let merged = merge_classified(&intervals)?;
    let directory = output_dir(target);
    Ok(Namespace {
        id: "main".to_string(),
        decoded_bytes: None,
        executable_bytes: interval_bytes(&merged)?,
        excluded_bytes: None,
        audit: "complete".to_string(),
        intervals: merged,
        evidence: vec![
            if asm_path.exists() {
                format!("{directory}/full/asm/manifest.json")
            } else {
                "assembly classification unavailable".to_string()
            },
            format!("{directory}/full/claimed/manifest.json"),
            "byte-identical full build and tracked assembly classification".to_string(),
        ],
    })
}

// ---------------------------------------------------------------------------
// Overlay images (bun sidecar)
// ---------------------------------------------------------------------------

/// Compile every overlay that has an exact-C sibling, in one sidecar call.
///
/// See `overlay_images.ts` for why this cannot be done natively.
fn overlay_images(root: &Path, sources: &[String]) -> Result<Vec<Vec<u8>>, String> {
    if sources.is_empty() {
        return Ok(Vec::new());
    }
    let stamp = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_err(|error| error.to_string())?
        .as_nanos();
    let work = std::env::temp_dir().join(format!("alchemy-full-c-images-{}-{stamp}", std::process::id()));
    std::fs::create_dir_all(&work).map_err(|error| error.to_string())?;
    let list = work.join("sources.txt");
    let output = work.join("images.bin");
    let outcome = (|| -> Result<Vec<Vec<u8>>, String> {
        std::fs::write(&list, sources.join("\n")).map_err(|error| error.to_string())?;
        let script = root.join("tools-rs").join("full-c-progress").join("overlay_images.ts");
        let result = Command::new("bun")
            .arg(&script)
            .arg(&list)
            .arg(&output)
            .current_dir(root)
            .output()
            .map_err(|error| error.to_string())?;
        if !result.status.success() {
            return Err(String::from_utf8_lossy(&result.stderr)
                .trim_matches(js::is_js_space)
                .to_string());
        }
        let blob = std::fs::read(&output).map_err(|error| error.to_string())?;
        let mut images = Vec::with_capacity(sources.len());
        let mut cursor = 0usize;
        for _ in 0..sources.len() {
            if cursor + 8 > blob.len() {
                return Err("overlay image sidecar returned a truncated response".to_string());
            }
            let mut header = [0u8; 8];
            header.copy_from_slice(&blob[cursor..cursor + 8]);
            let length = u64::from_le_bytes(header) as usize;
            cursor += 8;
            if cursor + length > blob.len() {
                return Err("overlay image sidecar returned a truncated response".to_string());
            }
            images.push(blob[cursor..cursor + length].to_vec());
            cursor += length;
        }
        Ok(images)
    })();
    let _ = std::fs::remove_dir_all(&work);
    outcome
}

/// `max(1, floor(cores * 0.8))` — the repository-wide parallelism cap.
fn resolve_jobs() -> usize {
    let cores = std::thread::available_parallelism().map(|count| count.get()).unwrap_or(1);
    ((cores as f64 * 0.8).floor() as usize).max(1)
}

fn overlay_inventories(root: &Path, sources: &[String]) -> Result<Vec<Namespace>, String> {
    let mut compiled: Vec<usize> = Vec::new();
    for (index, source) in sources.iter().enumerate() {
        if !overlay::overlay_c_sources(root, source).is_empty() {
            compiled.push(index);
        }
    }
    let compiled_sources: Vec<String> =
        compiled.iter().map(|index| sources[*index].clone()).collect();
    let images = overlay_images(root, &compiled_sources)?;
    let mut per_source: Vec<Option<Vec<u8>>> = vec![None; sources.len()];
    for (slot, image) in compiled.iter().zip(images) {
        per_source[*slot] = Some(image);
    }

    let next = std::sync::atomic::AtomicUsize::new(0);
    let results: Vec<std::sync::Mutex<Option<Result<Namespace, String>>>> =
        (0..sources.len()).map(|_| std::sync::Mutex::new(None)).collect();
    let jobs = resolve_jobs().min(sources.len().max(1));
    std::thread::scope(|scope| {
        for _ in 0..jobs {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
                if index >= sources.len() {
                    break;
                }
                let outcome = overlay::overlay_inventory(
                    root,
                    &sources[index],
                    per_source[index].as_deref(),
                    &[],
                );
                *results[index].lock().expect("inventory slot") = Some(outcome);
            });
        }
    });

    let mut inventories = Vec::with_capacity(sources.len());
    for slot in results {
        let outcome = slot.into_inner().expect("inventory slot").expect("every index is claimed");
        inventories.push(outcome?);
    }
    Ok(inventories)
}

// ---------------------------------------------------------------------------
// Inventory derivation and validation
// ---------------------------------------------------------------------------

fn derive_inventory(root: &Path, target: &str) -> Result<Inventory, String> {
    let main = main_inventory(root, target)?;
    if target == "gs2-en" {
        let mut incomplete = main.clone();
        incomplete.audit = "incomplete".to_string();
        return Ok(Inventory {
            target: target.to_string(),
            derivation: "full-c-progress-v1".to_string(),
            audit: "incomplete".to_string(),
            total_union_bytes: main.executable_bytes,
            main: incomplete,
            overlays: Vec::new(),
            caveats: Some(vec![
                "GS2-English is a compiler/bootstrap target and does not yet have a complete \
                 executable classification."
                    .to_string(),
            ]),
        });
    }
    let sources = overlay::overlay_sources(root, target)?;
    let overlays = overlay_inventories(root, &sources)?;
    let total = main.executable_bytes
        + overlays.iter().map(|overlay| overlay.executable_bytes).sum::<f64>();
    Ok(Inventory {
        target: target.to_string(),
        derivation: "full-c-progress-v1".to_string(),
        audit: "complete".to_string(),
        total_union_bytes: total,
        main,
        overlays,
        caveats: None,
    })
}

fn inventory_path(root: &Path, target: &str) -> PathBuf {
    root.join("metrics").join(format!("{target}-executable.json"))
}

fn report_path(root: &Path, target: &str) -> PathBuf {
    root.join("metrics").join(format!("{target}-progress.json"))
}

fn validate_inventory(inventory: &Inventory) -> Result<(), String> {
    for namespace in std::iter::once(&inventory.main).chain(inventory.overlays.iter()) {
        let diagnostics = overlap_diagnostics(&namespace.intervals)?;
        if !diagnostics.is_empty() {
            return Err(format!("{} overlaps:\n{}", namespace.id, diagnostics.join("\n")));
        }
        let measured = interval_bytes(&namespace.intervals)?;
        if measured != namespace.executable_bytes {
            return Err(format!(
                "{} executable total is stale: {} != {}",
                namespace.id,
                json::number(namespace.executable_bytes),
                json::number(measured)
            ));
        }
        if let Some(decoded) = namespace.decoded_bytes {
            match namespace.excluded_bytes {
                Some(excluded) if namespace.executable_bytes + excluded == decoded => {}
                _ => {
                    return Err(format!("{} decoded byte classification is incomplete", namespace.id))
                }
            }
        }
    }
    let total = inventory.main.executable_bytes
        + inventory.overlays.iter().map(|overlay| overlay.executable_bytes).sum::<f64>();
    if total != inventory.total_union_bytes {
        return Err("inventory total is stale".to_string());
    }
    Ok(())
}

fn spans_contained(spans: &[OwnedSpan], inventory: &Namespace) -> Result<f64, String> {
    let executable = union_intervals(&inventory.intervals)?;
    let mut ordered: Vec<&OwnedSpan> = spans.iter().collect();
    ordered.sort_by(|left, right| {
        left.start
            .partial_cmp(&right.start)
            .unwrap()
            .then(left.end.partial_cmp(&right.end).unwrap())
    });
    let mut previous: Option<&OwnedSpan> = None;
    for span in &ordered {
        if span.end <= span.start {
            return Err(format!("{}: empty C span", span.owner));
        }
        if let Some(last) = previous {
            if span.start < last.end && !(span.start == last.start && span.end == last.end) {
                return Err(format!(
                    "{}: overlapping C ownership: {} and {}",
                    span.namespace, last.owner, span.owner
                ));
            }
        }
        previous = Some(span);
        if !executable
            .iter()
            .any(|interval| interval.start <= span.start && span.end <= interval.end)
        {
            return Err(format!("{}: C span is outside audited executable intervals", span.owner));
        }
    }
    let owned: Vec<Interval> = ordered
        .iter()
        .map(|span| Interval {
            start: span.start,
            end: span.end,
            kind: "thumb".to_string(),
            evidence: span.owner.clone(),
        })
        .collect();
    Ok(union_intervals(&owned)?.iter().map(|span| span.end - span.start).sum())
}

fn main_c_spans(root: &Path, target: &str) -> Result<Vec<OwnedSpan>, String> {
    let manifest =
        read_json(&root.join(output_dir(target)).join("full").join("claimed").join("manifest.json"))?;
    let verification = manifest.get("verification").and_then(Value::as_str).unwrap_or("");
    let claimed = manifest.get("claimed_bytes").and_then(Value::as_f64);
    if !matches!(verification, "exact" | "source_only" | "rom")
        || !claimed.map(intervals::is_safe_integer).unwrap_or(false)
    {
        return Err("claimed build is not verified".to_string());
    }
    let mut spans = Vec::new();
    for region in regions_of(&manifest) {
        let source = js_string(region.get("source"));
        let path = root.join(&source);
        if !path.exists() || !js::canonical_c_source(&overlay::read_lossy(&path)?) {
            continue;
        }
        let address = number_field(region, "address", "C address")?;
        let size = number_field(region, "size", "C size")?;
        spans.push(OwnedSpan {
            namespace: "main".to_string(),
            start: address,
            end: address + size,
            owner: source,
        });
    }
    Ok(spans)
}

fn calculate_progress(
    inventory: &Inventory,
    main_spans: &[OwnedSpan],
    overlay_spans: &[(String, Vec<OwnedSpan>)],
) -> Result<ProgressReport, String> {
    validate_inventory(inventory)?;
    if inventory.audit != "complete"
        || inventory.main.audit != "complete"
        || inventory.overlays.iter().any(|overlay| overlay.audit != "complete")
    {
        return Err(format!(
            "Full-C Byte Share withheld: {} executable audit is incomplete",
            inventory.target
        ));
    }
    let main_bytes = spans_contained(main_spans, &inventory.main)?;
    let mut overlay_bytes = 0.0;
    let expected: HashSet<&str> =
        inventory.overlays.iter().map(|overlay| overlay.id.as_str()).collect();
    for overlay in &inventory.overlays {
        let spans = overlay_spans
            .iter()
            .find(|(id, _)| *id == overlay.id)
            .map(|(_, spans)| spans.as_slice())
            .unwrap_or(&[]);
        overlay_bytes += spans_contained(spans, overlay)?;
    }
    for (id, _) in overlay_spans {
        if !expected.contains(id.as_str()) {
            return Err(format!("C spans belong to un-inventoried code overlay {id}"));
        }
    }
    let full_c_bytes = main_bytes + overlay_bytes;
    let executable_bytes = inventory.total_union_bytes;
    let remaining_bytes = executable_bytes - full_c_bytes;
    if full_c_bytes < 0.0
        || full_c_bytes > executable_bytes
        || full_c_bytes + remaining_bytes != executable_bytes
    {
        return Err("Full-C arithmetic invariant failed".to_string());
    }
    Ok(ProgressReport {
        target: inventory.target.clone(),
        full_c_bytes,
        executable_bytes,
        remaining_bytes,
        percent: round_half_up_percent(full_c_bytes, executable_bytes)?,
        main_full_c_bytes: main_bytes,
        main_executable_bytes: inventory.main.executable_bytes,
        overlays_full_c_bytes: overlay_bytes,
        overlays_executable_bytes: inventory
            .overlays
            .iter()
            .map(|overlay| overlay.executable_bytes)
            .sum(),
    })
}

fn current_progress(root: &Path, target: &str) -> Result<ProgressReport, String> {
    let inventory = inventory_from_json(&read_json(&inventory_path(root, target))?)?;
    if inventory.target != target {
        return Err("inventory target mismatch".to_string());
    }
    let mut overlay_map: Vec<(String, Vec<OwnedSpan>)> = Vec::new();
    for source in overlay::overlay_sources(root, target)? {
        let file = source.rsplit('/').next().unwrap_or(&source).to_string();
        let id = js::strip_overlay_suffix(&file);
        overlay_map.push((id, overlay::overlay_c_spans(root, &source)?));
    }
    calculate_progress(&inventory, &main_c_spans(root, target)?, &overlay_map)
}

fn stable_inventory_shape(inventory: &Inventory) -> String {
    sha256::hex(canonical_json(&inventory_json(inventory)).as_bytes())
}

fn format_report(report: &ProgressReport) -> Result<String, String> {
    Ok([
        format!(
            "Full-C Byte Share: {} / {} executable bytes ({}%)",
            commas(report.full_c_bytes)?,
            commas(report.executable_bytes)?,
            to_fixed_2(report.percent)
        ),
        format!(
            "Main image: {} / {} executable bytes",
            commas(report.main_full_c_bytes)?,
            commas(report.main_executable_bytes)?
        ),
        format!(
            "Code overlays: {} / {} executable bytes",
            commas(report.overlays_full_c_bytes)?,
            commas(report.overlays_executable_bytes)?
        ),
    ]
    .join("\n"))
}

/// PORT NOTE: the TypeScript compares `canonicalJson` of the two unions. A
/// union is a list of `{start, end}` pairs whose JSON text is injective in
/// those numbers, so comparing the pairs directly is equivalent and avoids
/// serialising twice per namespace.
fn equal_union(left: &Namespace, right: &Namespace) -> Result<bool, String> {
    Ok(union_intervals(&left.intervals)? == union_intervals(&right.intervals)?
        && left.executable_bytes == right.executable_bytes
        && left.decoded_bytes == right.decoded_bytes
        && left.excluded_bytes == right.excluded_bytes)
}

fn check_current_inventory(root: &Path, target: &str, tracked: &Inventory) -> Result<(), String> {
    let derived = derive_inventory(root, target)?;
    validate_inventory(&derived)?;
    if tracked.audit != derived.audit
        || !equal_union(&tracked.main, &derived.main)?
        || tracked.overlays.len() != derived.overlays.len()
    {
        return Err(format!("tracked {target} executable inventory is stale"));
    }
    for index in 0..tracked.overlays.len() {
        if tracked.overlays[index].id != derived.overlays[index].id
            || !equal_union(&tracked.overlays[index], &derived.overlays[index])?
        {
            return Err(format!(
                "tracked {target} code-overlay inventory is stale at {}",
                tracked.overlays[index].id
            ));
        }
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

struct Options {
    target: String,
    check: bool,
    subject: bool,
    json: bool,
    write_inventory: bool,
    write_report: bool,
    self_test: bool,
}

enum Parsed {
    Options(Box<Options>),
    Help,
}

fn options_of(argv: &[String]) -> Result<Parsed, String> {
    let mut options = Options {
        target: DEFAULT_TARGET.to_string(),
        check: false,
        subject: false,
        json: false,
        write_inventory: false,
        write_report: false,
        self_test: false,
    };
    let mut index = 0;
    while index < argv.len() {
        let argument = argv[index].as_str();
        match argument {
            "--target" => {
                index += 1;
                options.target = parse_decomp_target(argv.get(index).map(String::as_str))?;
            }
            "--check" => options.check = true,
            "--subject" => options.subject = true,
            "--json" => options.json = true,
            "--write-inventory" => options.write_inventory = true,
            "--write-report" => options.write_report = true,
            "--self-test" => options.self_test = true,
            "-h" | "--help" => return Ok(Parsed::Help),
            other => return Err(format!("unrecognized argument: {other}")),
        }
        index += 1;
    }
    Ok(Parsed::Options(Box::new(options)))
}

fn relative(root: &Path, path: &Path) -> String {
    // `path.slice(ROOT.length + 1)`.
    let full = path.to_string_lossy().into_owned();
    let prefix = root.to_string_lossy().len() + 1;
    full.get(prefix..).unwrap_or("").to_string()
}

fn run(argv: &[String]) -> Result<(), String> {
    let options = match options_of(argv)? {
        Parsed::Help => {
            println!(
                "usage: full-c-progress [--target gs1-en|gs2-en] \
                 [--check|--subject|--json|--write-inventory|--write-report|--self-test]"
            );
            std::process::exit(0);
        }
        Parsed::Options(options) => options,
    };
    let root = root();
    if options.self_test {
        return self_test();
    }
    if options.write_inventory {
        let inventory = derive_inventory(&root, &options.target)?;
        let path = inventory_path(&root, &options.target);
        std::fs::write(&path, canonical_json(&inventory_json(&inventory)))
            .map_err(|error| error.to_string())?;
        println!(
            "inventory={} audit={} executable_bytes={} sha256={}",
            relative(&root, &path),
            inventory.audit,
            json::number(inventory.total_union_bytes),
            stable_inventory_shape(&inventory)
        );
        return Ok(());
    }
    if options.write_report {
        let report = current_progress(&root, &options.target)?;
        let path = report_path(&root, &options.target);
        std::fs::write(&path, canonical_json(&report_json(&report)))
            .map_err(|error| error.to_string())?;
        println!(
            "report={} {}",
            relative(&root, &path),
            format_subject(report.full_c_bytes, report.executable_bytes)?
        );
        return Ok(());
    }
    let inventory = inventory_from_json(&read_json(&inventory_path(&root, &options.target))?)?;
    validate_inventory(&inventory)?;
    if options.check {
        check_current_inventory(&root, &options.target, &inventory)?;
    }
    let report = current_progress(&root, &options.target)?;
    if options.check {
        let cached = read_json(&report_path(&root, &options.target))?;
        if canonical_json(&cached) != canonical_json(&report_json(&report)) {
            return Err(format!("tracked {} Full-C report is stale", options.target));
        }
    }
    if options.subject {
        println!("{}", format_subject(report.full_c_bytes, report.executable_bytes)?);
    } else if options.json {
        println!("{}", canonical_json(&report_json(&report)));
    } else {
        println!("{}", format_report(&report)?);
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// Self-test
// ---------------------------------------------------------------------------

fn test_item(start: i64, end: i64, kind: &str) -> Interval {
    Interval::new(start, end, kind, "test")
}

fn expect_reject<T>(outcome: Result<T, String>, label: &str) -> Result<(), String> {
    if outcome.is_ok() {
        return Err(format!("self-test failed to reject {label}"));
    }
    Ok(())
}

fn self_test_inventory() -> Inventory {
    Inventory {
        target: "gs1-en".to_string(),
        derivation: "test".to_string(),
        audit: "complete".to_string(),
        total_union_bytes: 18.0,
        main: Namespace {
            id: "main".to_string(),
            decoded_bytes: None,
            executable_bytes: 10.0,
            excluded_bytes: None,
            audit: "complete".to_string(),
            intervals: vec![
                test_item(0, 4, "arm"),
                test_item(4, 8, "thumb"),
                test_item(8, 9, "literal_pool"),
                test_item(9, 10, "executable_alignment"),
            ],
            evidence: vec!["test".to_string()],
        },
        overlays: vec![Namespace {
            id: "overlay-a".to_string(),
            decoded_bytes: Some(12.0),
            executable_bytes: 8.0,
            excluded_bytes: Some(4.0),
            audit: "complete".to_string(),
            intervals: vec![test_item(0x200_0000, 0x200_0008, "thumb")],
            evidence: vec!["test".to_string()],
        }],
        caveats: None,
    }
}

fn self_test() -> Result<(), String> {
    let item = |start, end| test_item(start, end, "thumb");
    if interval_bytes(&[])? != 0.0 {
        return Err("empty interval union failed".to_string());
    }
    if interval_bytes(&[item(0, 2), item(2, 4)])? != 4.0 {
        return Err("adjacent interval union failed".to_string());
    }
    if interval_bytes(&[item(0, 8), item(2, 4)])? != 8.0 {
        return Err("nested interval union failed".to_string());
    }
    if interval_bytes(&[item(0, 4), item(2, 6)])? != 6.0 {
        return Err("partial interval union failed".to_string());
    }
    if overlap_diagnostics(&[item(0, 4), item(2, 6)])?.len() != 1 {
        return Err("overlap diagnostics failed".to_string());
    }
    if !overlap_diagnostics(&[item(0, 4), item(0, 4)])?.is_empty() {
        return Err("identical alias failed".to_string());
    }
    if intervals::kilobytes(1999.0)? != 1.0 || intervals::kilobytes(999.0)? != 0.0 {
        return Err("kilobyte floor failed".to_string());
    }
    if format_subject(123_456.0, 1_234_567.0)? != "[ ☀️ 123 / 1,234 ]" {
        return Err("subject format failed".to_string());
    }
    match intervals::parse_subject("decomp: x [ ☀️ 123 / 1,234 ]")? {
        Some(parsed)
            if parsed.full_c_kilobytes == 123.0 && parsed.executable_kilobytes == 1234.0 => {}
        _ => return Err("subject parse failed".to_string()),
    }
    for invalid in [
        "x [ ☀️ 1234 / 1,234 ]",
        "x [ ☀️ 123/1,234 ]",
        "x [ ☀️ 123 / 1,234 bytes]",
        "x [C 123,456/1,234,567 bytes]",
        "x [ ☀️ 2 / 1 ]",
        "x [123 of 456]",
    ] {
        let parsed = intervals::parse_subject(invalid).and_then(|subject| match subject {
            Some(subject) => Ok(subject),
            None => Err("rejected".to_string()),
        });
        expect_reject(parsed, invalid)?;
    }
    if round_half_up_percent(1.0, 8.0)? != 12.5 {
        return Err("round-half-up failed".to_string());
    }

    // The raw-leaf battery lives beside the code it exercises, in
    // `overlay.rs`'s `#[cfg(test)]` module, which `cargo test` runs. The
    // self-test keeps the arithmetic and reporting checks it can run in a
    // release binary.
    let inventory = self_test_inventory();
    let report = calculate_progress(
        &inventory,
        &[OwnedSpan {
            namespace: "main".to_string(),
            start: 0.0,
            end: 4.0,
            owner: "main-c".to_string(),
        }],
        &[(
            "overlay-a".to_string(),
            vec![OwnedSpan {
                namespace: "overlay-a".to_string(),
                start: 0x200_0000 as f64,
                end: 0x200_0002 as f64,
                owner: "overlay-c".to_string(),
            }],
        )],
    )?;
    if report.full_c_bytes != 6.0 || report.executable_bytes != 18.0 {
        return Err("progress calculation failed".to_string());
    }
    expect_reject(
        calculate_progress(
            &inventory,
            &[
                OwnedSpan {
                    namespace: "main".to_string(),
                    start: 0.0,
                    end: 4.0,
                    owner: "c".to_string(),
                },
                OwnedSpan {
                    namespace: "main".to_string(),
                    start: 2.0,
                    end: 6.0,
                    owner: "asm-overlap".to_string(),
                },
            ],
            &[],
        ),
        "C/assembly ownership overlap",
    )?;
    let mut incomplete = inventory.clone();
    incomplete.audit = "incomplete".to_string();
    expect_reject(calculate_progress(&incomplete, &[], &[]), "incomplete audit")?;
    let mut target_isolation = inventory.clone();
    target_isolation.target = "gs2-en".to_string();
    if target_isolation.target == inventory.target {
        return Err("target isolation failed".to_string());
    }
    println!("self-test=ok metric=full-c-byte-share");
    Ok(())
}

fn main() {
    let argv: Vec<String> = std::env::args().skip(1).collect();
    if let Err(message) = run(&argv) {
        // PORT NOTE: an uncaught `throw` in bun prints a stack trace after the
        // message. Only the message is reproduced here; the trace is noise that
        // no caller parses.
        eprintln!("error: {message}");
        std::process::exit(1);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_self_test_battery_passes() {
        self_test().expect("self-test");
    }

    #[test]
    fn main_kinds_follow_the_documented_precedence() {
        assert_eq!(main_kind("alignment_padding"), "executable_alignment");
        assert_eq!(main_kind("literal_pool"), "literal_pool");
        assert_eq!(main_kind("arm_veneer"), "veneer");
        assert_eq!(main_kind("thunk_table"), "veneer");
        assert_eq!(main_kind("gba_arm_entry"), "arm");
        assert_eq!(main_kind("hand_written_arm"), "arm");
        assert_eq!(main_kind("compiler_output"), "thumb");
        assert_eq!(main_kind("data"), "structural");
        // `includes("veneer")` is tested before `includes("arm")`.
        assert_eq!(main_kind("arm_veneer_pool"), "veneer");
    }

    #[test]
    fn a_bad_target_is_quoted_but_a_missing_one_is_not() {
        assert_eq!(parse_decomp_target(Some("gs1-en")).unwrap(), "gs1-en");
        assert_eq!(
            parse_decomp_target(Some("gs3")).unwrap_err(),
            "unsupported decomp target \"gs3\"; expected gs1-en or gs2-en"
        );
        assert_eq!(
            parse_decomp_target(None).unwrap_err(),
            "unsupported decomp target undefined; expected gs1-en or gs2-en"
        );
    }

    #[test]
    fn unknown_arguments_are_rejected_by_name() {
        assert_eq!(
            options_of(&["--nope".to_string()]).err().unwrap(),
            "unrecognized argument: --nope"
        );
        let parsed = options_of(&["--check".to_string(), "--json".to_string()]).unwrap();
        match parsed {
            Parsed::Options(options) => {
                assert!(options.check && options.json);
                assert_eq!(options.target, "gs1-en");
            }
            Parsed::Help => panic!("expected options"),
        }
    }

    #[test]
    fn a_stale_total_is_a_hard_failure() {
        let mut inventory = self_test_inventory();
        inventory.total_union_bytes = 19.0;
        assert_eq!(validate_inventory(&inventory).unwrap_err(), "inventory total is stale");

        let mut stale = self_test_inventory();
        stale.main.executable_bytes = 11.0;
        stale.total_union_bytes = 19.0;
        assert_eq!(
            validate_inventory(&stale).unwrap_err(),
            "main executable total is stale: 11 != 10"
        );

        let mut unaccounted = self_test_inventory();
        unaccounted.overlays[0].excluded_bytes = Some(3.0);
        assert_eq!(
            validate_inventory(&unaccounted).unwrap_err(),
            "overlay-a decoded byte classification is incomplete"
        );
    }

    #[test]
    fn c_spans_have_to_sit_inside_the_audited_intervals() {
        let inventory = self_test_inventory();
        let outside = spans_contained(
            &[OwnedSpan {
                namespace: "main".to_string(),
                start: 10.0,
                end: 12.0,
                owner: "stray.c".to_string(),
            }],
            &inventory.main,
        );
        assert_eq!(
            outside.unwrap_err(),
            "stray.c: C span is outside audited executable intervals"
        );

        let empty = spans_contained(
            &[OwnedSpan {
                namespace: "main".to_string(),
                start: 4.0,
                end: 4.0,
                owner: "empty.c".to_string(),
            }],
            &inventory.main,
        );
        assert_eq!(empty.unwrap_err(), "empty.c: empty C span");

        // An exact duplicate is an alias, not an overlap.
        let alias = OwnedSpan {
            namespace: "main".to_string(),
            start: 0.0,
            end: 4.0,
            owner: "a.c".to_string(),
        };
        let twin = OwnedSpan { owner: "b.c".to_string(), ..alias.clone() };
        assert_eq!(spans_contained(&[alias, twin], &inventory.main).unwrap(), 4.0);
    }

    #[test]
    fn spans_for_an_unknown_overlay_are_rejected() {
        let inventory = self_test_inventory();
        let outcome = calculate_progress(
            &inventory,
            &[],
            &[(
                "overlay-z".to_string(),
                vec![OwnedSpan {
                    namespace: "overlay-z".to_string(),
                    start: 0x200_0000 as f64,
                    end: 0x200_0002 as f64,
                    owner: "z".to_string(),
                }],
            )],
        );
        assert_eq!(
            outcome.unwrap_err(),
            "C spans belong to un-inventoried code overlay overlay-z"
        );
    }

    #[test]
    fn the_report_renders_with_thousands_separators_and_two_decimals() {
        let report = ProgressReport {
            target: "gs1-en".to_string(),
            full_c_bytes: 317_066.0,
            executable_bytes: 1_345_890.0,
            remaining_bytes: 1_028_824.0,
            percent: 23.56,
            main_full_c_bytes: 109_020.0,
            main_executable_bytes: 548_364.0,
            overlays_full_c_bytes: 208_046.0,
            overlays_executable_bytes: 797_526.0,
        };
        assert_eq!(
            format_report(&report).unwrap(),
            "Full-C Byte Share: 317,066 / 1,345,890 executable bytes (23.56%)\n\
             Main image: 109,020 / 548,364 executable bytes\n\
             Code overlays: 208,046 / 797,526 executable bytes"
        );
    }

    #[test]
    fn inventory_json_keeps_the_typescript_key_order() {
        let text = canonical_json(&inventory_json(&self_test_inventory()));
        let keys: Vec<&str> = text
            .lines()
            .filter_map(|line| line.trim().strip_prefix('"'))
            .filter_map(|line| line.split('"').next())
            .collect();
        assert_eq!(&keys[0..8], &[
            "format",
            "metric",
            "target",
            "derivation",
            "audit",
            "total_union_bytes",
            "main",
            "id",
        ]);
        // Overlays carry decoded/excluded bytes; the main image does not.
        assert!(text.contains("\"decoded_bytes\""));
        assert!(!canonical_json(&namespace_json(&self_test_inventory().main))
            .contains("decoded_bytes"));
    }

    #[test]
    fn the_shape_digest_tracks_the_canonical_json() {
        let inventory = self_test_inventory();
        let expected = sha256::hex(canonical_json(&inventory_json(&inventory)).as_bytes());
        assert_eq!(stable_inventory_shape(&inventory), expected);
        let mut moved = inventory.clone();
        moved.derivation = "test2".to_string();
        assert_ne!(stable_inventory_shape(&moved), expected);
    }

    #[test]
    fn string_coercion_matches_js_for_missing_manifest_fields() {
        assert_eq!(js_string(None), "undefined");
        assert_eq!(js_string(Some(&Value::Num(16.0))), "16");
        assert_eq!(js_string(Some(&json::string("src.s"))), "src.s");
    }
}
