//! Full-C reporting over the coverage map's shared audited interval model.
#[allow(dead_code)]
#[path = "../../coverage-map/src/model.rs"]
mod model;
use compiler_core::source_paths::{SourceOwner, SourcePaths};
use model::{bytes, intersect, normalize, Span};
use serde_json::Value;
use std::collections::BTreeMap;
use std::path::{Path, PathBuf};
const DEFAULT_TARGET: &str = "gs1-en";
fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("crate lives two levels below the repository")
        .to_path_buf()
}
fn read(path: &Path) -> Result<String, String> {
    std::fs::read(path)
        .map(|bytes| String::from_utf8_lossy(&bytes).into_owned())
        .map_err(|error| format!("cannot read {}: {error}", path.display()))
}
fn json(path: &Path) -> Result<Value, String> {
    serde_json::from_str(&read(path)?).map_err(|error| format!("{}: {error}", path.display()))
}
fn get<'a>(value: &'a Value, key: &str) -> Option<&'a Value> {
    value.as_object()?.get(key)
}
fn text(value: &Value, key: &str) -> String {
    get(value, key)
        .and_then(Value::as_str)
        .unwrap_or_default()
        .to_string()
}
fn integer(value: &Value, key: &str) -> Option<i64> {
    get(value, key).and_then(Value::as_i64).or_else(|| {
        get(value, key)
            .and_then(Value::as_u64)
            .and_then(|n| i64::try_from(n).ok())
    })
}
fn array<'a>(value: &'a Value, key: &str) -> &'a [Value] {
    get(value, key)
        .and_then(Value::as_array)
        .map(Vec::as_slice)
        .unwrap_or(&[])
}
fn number(n: i64) -> Value {
    Value::Number(n.into())
}
fn string(s: impl Into<String>) -> Value {
    Value::String(s.into())
}
fn object(fields: Vec<(&str, Value)>) -> Value {
    Value::Object(fields.into_iter().map(|(k, v)| (k.into(), v)).collect())
}
#[derive(Clone, Debug, PartialEq, Eq)]
struct Region {
    span: Span,
    kind: String,
    evidence: String,
}
#[derive(Clone, Debug, PartialEq, Eq)]
struct Namespace {
    id: String,
    decoded_bytes: Option<i64>,
    executable_bytes: i64,
    excluded_bytes: Option<i64>,
    audit: String,
    regions: Vec<Region>,
    evidence: Vec<String>,
}
#[derive(Clone, Debug, PartialEq, Eq)]
struct Inventory {
    target: String,
    derivation: String,
    audit: String,
    total_union_bytes: i64,
    main: Namespace,
    overlays: Vec<Namespace>,
    caveats: Option<Vec<String>>,
}
#[derive(Clone, Debug, PartialEq)]
struct Report {
    target: String,
    full_c_bytes: i64,
    executable_bytes: i64,
    remaining_bytes: i64,
    percent: f64,
    main_full_c_bytes: i64,
    main_executable_bytes: i64,
    overlays_full_c_bytes: i64,
    overlays_executable_bytes: i64,
}
fn span(value: &Value) -> Option<Span> {
    Some(Span::new(integer(value, "start")?, integer(value, "end")?))
}
fn region(value: &Value) -> Option<Region> {
    let span = span(value)?;
    (span.end > span.start).then(|| Region {
        span,
        kind: text(value, "kind"),
        evidence: text(value, "evidence"),
    })
}
fn namespace(value: &Value) -> Result<Namespace, String> {
    let regions = array(value, "intervals")
        .iter()
        .filter_map(region)
        .collect();
    Ok(Namespace {
        id: text(value, "id"),
        decoded_bytes: get(value, "decoded_bytes").and_then(Value::as_i64),
        executable_bytes: integer(value, "executable_bytes")
            .ok_or_else(|| "executable inventory has a non-integer byte count".to_string())?,
        excluded_bytes: get(value, "excluded_bytes").and_then(Value::as_i64),
        audit: text(value, "audit"),
        regions,
        evidence: array(value, "evidence")
            .iter()
            .filter_map(Value::as_str)
            .map(str::to_string)
            .collect(),
    })
}
fn inventory(value: &Value) -> Result<Inventory, String> {
    if integer(value, "format") != Some(1) || text(value, "metric") != "full-c-byte-share" {
        return Err("unsupported executable inventory format".into());
    }
    Ok(Inventory {
        target: text(value, "target"),
        derivation: text(value, "derivation"),
        audit: text(value, "audit"),
        total_union_bytes: integer(value, "total_union_bytes")
            .ok_or_else(|| "executable inventory has no total_union_bytes".to_string())?,
        main: namespace(get(value, "main").ok_or("executable inventory has no main")?)?,
        overlays: array(value, "overlays")
            .iter()
            .map(namespace)
            .collect::<Result<_, _>>()?,
        caveats: get(value, "caveats")
            .and_then(Value::as_array)
            .map(|items| {
                items
                    .iter()
                    .filter_map(Value::as_str)
                    .map(str::to_string)
                    .collect()
            }),
    })
}
fn regions_json(regions: &[Region]) -> Value {
    Value::Array(
        regions
            .iter()
            .map(|row| {
                object(vec![
                    ("start", number(row.span.start)),
                    ("end", number(row.span.end)),
                    ("kind", string(&row.kind)),
                    ("evidence", string(&row.evidence)),
                ])
            })
            .collect(),
    )
}
fn namespace_json(value: &Namespace) -> Value {
    let mut fields = vec![("id", string(&value.id))];
    if let Some(bytes) = value.decoded_bytes {
        fields.push(("decoded_bytes", number(bytes)));
    }
    fields.push(("executable_bytes", number(value.executable_bytes)));
    if let Some(bytes) = value.excluded_bytes {
        fields.push(("excluded_bytes", number(bytes)));
    }
    fields.extend([
        ("audit", string(&value.audit)),
        ("intervals", regions_json(&value.regions)),
        (
            "evidence",
            Value::Array(value.evidence.iter().map(string).collect()),
        ),
    ]);
    object(fields)
}
fn inventory_json(value: &Inventory) -> Value {
    let mut fields = vec![
        ("format", number(1)),
        ("metric", string("full-c-byte-share")),
        ("target", string(&value.target)),
        ("derivation", string(&value.derivation)),
        ("audit", string(&value.audit)),
        ("total_union_bytes", number(value.total_union_bytes)),
        ("main", namespace_json(&value.main)),
        (
            "overlays",
            Value::Array(value.overlays.iter().map(namespace_json).collect()),
        ),
    ];
    if let Some(caveats) = &value.caveats {
        fields.push((
            "caveats",
            Value::Array(caveats.iter().map(string).collect()),
        ));
    }
    object(fields)
}
fn report_json(value: &Report) -> Value {
    object(vec![
        ("format", number(1)),
        ("metric", string("full-c-byte-share")),
        ("target", string(&value.target)),
        ("full_c_bytes", number(value.full_c_bytes)),
        ("executable_bytes", number(value.executable_bytes)),
        ("remaining_bytes", number(value.remaining_bytes)),
        (
            "percent",
            serde_json::Number::from_f64(value.percent)
                .map(Value::Number)
                .unwrap_or(Value::Null),
        ),
        (
            "main",
            object(vec![
                ("full_c_bytes", number(value.main_full_c_bytes)),
                ("executable_bytes", number(value.main_executable_bytes)),
            ]),
        ),
        (
            "overlays",
            object(vec![
                ("full_c_bytes", number(value.overlays_full_c_bytes)),
                ("executable_bytes", number(value.overlays_executable_bytes)),
            ]),
        ),
        ("audit", string("complete")),
    ])
}
fn executable(namespace: &Namespace) -> Vec<Span> {
    normalize(
        &namespace
            .regions
            .iter()
            .map(|row| row.span)
            .collect::<Vec<_>>(),
    )
}
fn validate_namespace(namespace: &Namespace) -> Result<(), String> {
    let mut regions = namespace.regions.clone();
    regions.sort_by_key(|row| (row.span.start, row.span.end));
    for pair in regions.windows(2) {
        if pair[1].span.start < pair[0].span.end {
            return Err(format!(
                "{} has overlapping executable intervals",
                namespace.id
            ));
        }
    }
    let measured = bytes(&executable(namespace));
    if measured != namespace.executable_bytes {
        return Err(format!(
            "{} executable total is stale: {} != {}",
            namespace.id, namespace.executable_bytes, measured
        ));
    }
    if let Some(decoded) = namespace.decoded_bytes {
        if namespace.excluded_bytes != Some(decoded - namespace.executable_bytes) {
            return Err(format!(
                "{} decoded byte classification is incomplete",
                namespace.id
            ));
        }
    }
    Ok(())
}
fn validate_inventory(value: &Inventory) -> Result<(), String> {
    if value.audit != "complete" || value.main.audit != "complete" {
        return Err(format!(
            "Full-C Byte Share withheld: {} executable audit is incomplete",
            value.target
        ));
    }
    validate_namespace(&value.main)?;
    for overlay in &value.overlays {
        if overlay.audit != "complete" {
            return Err(format!(
                "Full-C Byte Share withheld: {} is incomplete",
                overlay.id
            ));
        }
        validate_namespace(overlay)?;
    }
    let total = value.main.executable_bytes
        + value
            .overlays
            .iter()
            .map(|item| item.executable_bytes)
            .sum::<i64>();
    if total != value.total_union_bytes {
        return Err("executable inventory total is stale".into());
    }
    Ok(())
}
fn canonical(source: &str) -> bool {
    !source.contains(".incbin")
        && !source.contains("M2C_ERROR")
        && !source.contains("__asm__")
        && !source.contains("asm volatile")
        && !source
            .lines()
            .any(|line| line.contains("register") && line.contains("asm") && line.contains('('))
}
fn source_span(value: &Value, root: &Path) -> Option<Span> {
    let start = integer(value, "address")?;
    let size = integer(value, "size")?;
    let source = text(value, "source");
    (size > 0 && canonical(&read(&root.join(source)).ok()?)).then(|| Span::new(start, start + size))
}
fn main_exact(root: &Path, target: &str, namespace: &Namespace) -> Result<Vec<Span>, String> {
    let manifest = json(
        &root
            .join("out")
            .join(target)
            .join("full/claimed/manifest.json"),
    )?;
    let spans: Vec<_> = array(&manifest, "regions")
        .iter()
        .filter_map(|row| source_span(row, root))
        .collect();
    owned(&spans, namespace)
}
fn hex(value: &str) -> Option<i64> {
    i64::from_str_radix(value.trim().trim_start_matches("0x"), 16).ok()
}
fn space(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix(".space")?.trim();
    if value.starts_with('-') {
        return None;
    }
    value.strip_prefix("0x").map_or_else(
        || value.parse().ok(),
        |value| i64::from_str_radix(value, 16).ok(),
    )
}
fn owner_label(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix("AlchemyC_")?.trim_end_matches(':');
    (value.len() == 8 && value.chars().all(|c| c.is_ascii_hexdigit()))
        .then(|| hex(value))
        .flatten()
}
fn local_label(line: &str) -> bool {
    line.trim().starts_with(".L_") && line.trim_end().ends_with(':')
}
fn overlay_id(file: &str) -> Option<String> {
    Some(format!(
        "resource_{}",
        file.strip_prefix("resource_")?.strip_suffix("_overlay.s")?
    ))
}
fn overlay_source_spans(
    root: &Path,
    file: &str,
    namespace: &Namespace,
) -> Result<Vec<Span>, String> {
    let Some(id) = overlay_id(file) else {
        return Ok(Vec::new());
    };
    let source = read(&root.join("games/gs1/assets/code").join(file))?;
    let source_paths = SourcePaths::load(root)?;
    let mut owners: Vec<Vec<Span>> = Vec::new();
    let mut current: Option<Vec<Span>> = None;
    let mut cursor = 0;
    for line in source.lines() {
        if let Some(address) = owner_label(line) {
            if let Some(spans) = current.take() {
                if !spans.is_empty() {
                    owners.push(spans);
                }
            }
            cursor = address;
            current = Some(Vec::new());
            continue;
        }
        if current.is_some() && (line.trim().is_empty() || local_label(line)) {
            continue;
        }
        if let Some(size) = space(line) {
            if let Some(spans) = current.as_mut() {
                spans.push(Span::new(cursor, cursor + size));
                cursor += size;
                continue;
            }
        }
        if !line.trim().is_empty() {
            if let Some(spans) = current.take() {
                if !spans.is_empty() {
                    owners.push(spans);
                }
            }
        }
    }
    if let Some(spans) = current {
        if !spans.is_empty() {
            owners.push(spans);
        }
    }
    let mut exact = Vec::new();
    for spans in owners {
        let Some(entry) = spans.first().map(|span| span.start) else {
            continue;
        };
        let owner = SourceOwner::parse(&format!("{id}:{entry:08x}"))?;
        let path = source_paths.source_path(owner);
        if path.exists() && canonical(&read(&path)?) {
            exact.extend(spans);
        }
    }
    owned(&exact, namespace)
}
fn overlay_exact(root: &Path, value: &Inventory) -> Result<BTreeMap<String, Vec<Span>>, String> {
    let mut files = std::fs::read_dir(root.join("games/gs1/assets/code"))
        .map_err(|error| format!("cannot list overlay assembly: {error}"))?
        .filter_map(|item| {
            item.ok()
                .map(|item| item.file_name().to_string_lossy().into_owned())
        })
        .filter(|name| name.ends_with("_overlay.s"))
        .collect::<Vec<_>>();
    files.sort();
    let by_id: BTreeMap<_, _> = value
        .overlays
        .iter()
        .map(|item| (item.id.as_str(), item))
        .collect();
    let mut result = BTreeMap::new();
    for file in files {
        let Some(id) = overlay_id(&file) else {
            continue;
        };
        let Some(namespace) = by_id.get(id.as_str()) else {
            continue;
        };
        result.insert(id, overlay_source_spans(root, &file, namespace)?);
    }
    Ok(result)
}
fn owned(spans: &[Span], namespace: &Namespace) -> Result<Vec<Span>, String> {
    let executable = executable(namespace);
    let mut ordered = spans.to_vec();
    ordered.sort_by_key(|span| (span.start, span.end));
    for pair in ordered.windows(2) {
        if pair[1].start < pair[0].end && pair[1] != pair[0] {
            return Err(format!("{} has overlapping C ownership", namespace.id));
        }
    }
    for span in &ordered {
        if intersect(&[*span], &executable).is_empty() {
            return Err(format!(
                "{} C ownership is outside audited executable intervals",
                namespace.id
            ));
        }
    }
    Ok(intersect(&ordered, &executable))
}
fn report(root: &Path, target: &str, value: &Inventory) -> Result<Report, String> {
    validate_inventory(value)?;
    let main = main_exact(root, target, &value.main)?;
    let overlays = overlay_exact(root, value)?;
    let main_bytes = bytes(&main);
    let overlay_bytes = value
        .overlays
        .iter()
        .map(|namespace| {
            bytes(
                overlays
                    .get(&namespace.id)
                    .map(Vec::as_slice)
                    .unwrap_or(&[]),
            )
        })
        .sum::<i64>();
    let full = main_bytes + overlay_bytes;
    let executable = value.total_union_bytes;
    let remaining = executable - full;
    if full < 0 || remaining < 0 || full + remaining != executable {
        return Err("Full-C arithmetic invariant failed".into());
    }
    Ok(Report {
        target: target.into(),
        full_c_bytes: full,
        executable_bytes: executable,
        remaining_bytes: remaining,
        percent: (full as f64 * 100.0 / executable as f64 * 100.0 + 0.5).floor() / 100.0,
        main_full_c_bytes: main_bytes,
        main_executable_bytes: value.main.executable_bytes,
        overlays_full_c_bytes: overlay_bytes,
        overlays_executable_bytes: value
            .overlays
            .iter()
            .map(|item| item.executable_bytes)
            .sum(),
    })
}
fn report_path(root: &Path, target: &str) -> PathBuf {
    root.join("games")
        .join(target.split('-').next().unwrap_or("gs1"))
        .join("metrics")
        .join(format!("{target}-progress.json"))
}
fn inventory_path(root: &Path, target: &str) -> PathBuf {
    root.join("games")
        .join(target.split('-').next().unwrap_or("gs1"))
        .join("metrics")
        .join(format!("{target}-executable.json"))
}
fn permanent_bytes(root: &Path, target: &str) -> Result<i64, String> {
    let value = json(
        &root
            .join("games")
            .join(target.split('-').next().unwrap_or("gs1"))
            .join("metrics")
            .join(format!("{target}-coverage-map.json")),
    )?;
    get(
        get(&value, "categories").ok_or("coverage map has no categories")?,
        "retained_asm",
    )
    .and_then(|value| get(value, "bytes"))
    .and_then(Value::as_i64)
    .ok_or_else(|| "coverage map has no categories.retained_asm.bytes".into())
}
fn check_build(root: &Path, target: &str) -> Result<(), String> {
    let value = json(&root.join("out").join(target).join("full/rebuilt.json"))?;
    if get(&value, "byte_identical") != Some(&Value::Bool(true)) {
        return Err("the last full build was not byte-identical".into());
    }
    for field in ["rom_fallback_bytes", "unowned_bytes"] {
        if get(&value, field).and_then(Value::as_i64).unwrap_or(0) > 0 {
            return Err(format!("the last full build has non-zero {field}"));
        }
    }
    Ok(())
}
fn comma(n: i64) -> String {
    let raw = n.to_string();
    let (sign, digits) = raw
        .strip_prefix('-')
        .map_or(("", raw.as_str()), |s| ("-", s));
    let mut out = sign.to_string();
    for (i, ch) in digits.chars().enumerate() {
        if i > 0 && (digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(ch);
    }
    out
}
fn fixed2(value: f64) -> String {
    format!("{value:.2}")
}
fn subject(report: &Report, retained: i64) -> Result<String, String> {
    let done = report.full_c_bytes + retained;
    if done < 0 || done > report.executable_bytes {
        return Err("DONE numerator exceeds executable denominator".into());
    }
    Ok(format!(
        "☀️ {}% –",
        (done * 100 + report.executable_bytes / 2) / report.executable_bytes
    ))
}
fn display(report: &Report) -> String {
    format!(
        "Full-C Byte Share: {} / {} executable bytes ({}%)\nMain image: {} / {} executable bytes\nCode overlays: {} / {} executable bytes",
        comma(report.full_c_bytes),
        comma(report.executable_bytes),
        fixed2(report.percent),
        comma(report.main_full_c_bytes),
        comma(report.main_executable_bytes),
        comma(report.overlays_full_c_bytes),
        comma(report.overlays_executable_bytes)
    )
}
fn derive_inventory(root: &Path, target: &str) -> Result<Inventory, String> {
    // Re-emit the audited inventory without deriving it from source ownership.
    let value = inventory(&json(&inventory_path(root, target))?)?;
    if value.target != target {
        return Err("executable inventory target mismatch".into());
    }
    Ok(value)
}
fn relative(root: &Path, path: &Path) -> String {
    path.strip_prefix(root)
        .unwrap_or(path)
        .display()
        .to_string()
}
#[derive(Default)]
struct Options {
    target: String,
    check: bool,
    subject: bool,
    json: bool,
    write_inventory: bool,
    write_report: bool,
    self_test: bool,
}
fn options(argv: &[String]) -> Result<Option<Options>, String> {
    let mut options = Options {
        target: DEFAULT_TARGET.into(),
        ..Options::default()
    };
    let mut i = 0;
    while i < argv.len() {
        match argv[i].as_str() {
            "--target" => {
                i += 1;
                options.target = match argv.get(i).map(String::as_str) {
                    Some(target @ ("gs1-en" | "gs2-en")) => target.into(),
                    Some(other) => {
                        return Err(format!(
                            "unsupported decomp target \"{other}\"; expected gs1-en or gs2-en"
                        ))
                    }
                    None => {
                        return Err(
                            "unsupported decomp target undefined; expected gs1-en or gs2-en".into(),
                        )
                    }
                };
            }
            "--check" => options.check = true,
            "--subject" => options.subject = true,
            "--json" => options.json = true,
            "--write-inventory" => options.write_inventory = true,
            "--write-report" => options.write_report = true,
            "--self-test" => options.self_test = true,
            "-h" | "--help" => return Ok(None),
            other => return Err(format!("unrecognized argument: {other}")),
        }
        i += 1;
    }
    Ok(Some(options))
}
fn run(argv: &[String]) -> Result<String, String> {
    let Some(options) = options(argv)? else {
        return Ok("usage: full-c-progress [--target gs1-en|gs2-en] [--check|--subject|--json|--write-inventory|--write-report|--self-test]".into());
    };
    if options.self_test {
        if bytes(&normalize(&[Span::new(0, 4), Span::new(2, 8)])) != 8 {
            return Err("interval self-test failed".into());
        }
        return Ok("self-test=ok metric=full-c-byte-share".into());
    }
    let root = root();
    let tracked = inventory(&json(&inventory_path(&root, &options.target))?)?;
    if options.write_inventory {
        let output = serde_json::to_string_pretty(&inventory_json(&derive_inventory(
            &root,
            &options.target,
        )?))
        .map_err(|error| error.to_string())?;
        std::fs::write(
            inventory_path(&root, &options.target),
            format!("{output}\n"),
        )
        .map_err(|error| error.to_string())?;
        return Ok(format!(
            "inventory={} audit={} executable_bytes={}",
            relative(&root, &inventory_path(&root, &options.target)),
            tracked.audit,
            tracked.total_union_bytes
        ));
    }
    let current = report(&root, &options.target, &tracked)?;
    if options.write_report {
        let output = serde_json::to_string_pretty(&report_json(&current))
            .map_err(|error| error.to_string())?;
        std::fs::write(report_path(&root, &options.target), format!("{output}\n"))
            .map_err(|error| error.to_string())?;
        return Ok(format!(
            "report={} {}",
            relative(&root, &report_path(&root, &options.target)),
            subject(&current, permanent_bytes(&root, &options.target)?)?
        ));
    }
    if options.check {
        validate_inventory(&tracked)?;
        let expected = report_json(&current);
        let cached = json(&report_path(&root, &options.target))?;
        if cached != expected {
            return Err(format!("tracked {} Full-C report is stale", options.target));
        }
        check_build(&root, &options.target)?;
    }
    if options.subject {
        Ok(subject(&current, permanent_bytes(&root, &options.target)?)?)
    } else if options.json {
        serde_json::to_string(&report_json(&current)).map_err(|error| error.to_string())
    } else {
        Ok(display(&current))
    }
}
pub fn entry(arguments: &[String]) {
    match run(arguments) {
        Ok(output) => println!("{output}"),
        Err(error) => {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
    }
}
