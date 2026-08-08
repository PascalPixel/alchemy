// Which semantic reconstruction explains each overlay row you have not yet
// matched byte-exactly.
//
// WHY THIS FILE EXISTS: this is the Rust port of `tools/overlay/exact_reading_list.ts`.
// Semantic reconstruction already explains *what the code does* for most overlay
// owners, so for those rows the expensive part — reading assembly until the
// behaviour is clear — is already done and sitting in `semantic/`. The tool
// prints the pairing: every strict-queue owner that has a semantic source but no
// exact source, with the path to read and the row's size, ranked by
// bytes-with-a-reference descending, because that is the axis on which reading
// someone else's reconstruction saves the most time.
//
// Rows whose start address is not inside an audited executable interval are held
// back: they compile, they adopt, and they reproduce the ROM byte-identically,
// but `full_c_progress --write-report` cannot then write its report, so the
// conversion has to be backed out whole. See `starts_in_audited_span`.
//
// The TypeScript original is retained; this crate must stay byte-identical to it
// on stdout, stderr and exit code. Every deliberate divergence is a PORT NOTE.

pub mod json;

use json::Value;
use std::collections::HashSet;
use std::fs;
use std::path::{Path, PathBuf};

/// An audited executable interval, half-open: `start <= address < end`.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Interval {
    pub start: i64,
    pub end: i64,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Pairing {
    pub overlay: String,
    pub address: String,
    pub bytes: i64,
    pub semantic_source: String,
    /// True when the report would reject this row; see `starts_in_audited_span`.
    pub blocked: bool,
}

/// The overlay image base. Row offsets are relative to it.
const OVERLAY_BASE: i64 = 0x0200_0000;

/// Strict-queue rows only: the same filter semantic reconstruction uses.
///
/// PORT NOTE: the TypeScript delegates to `isConvertibleRow` in
/// `tools/lib/semantic_regions_sync.ts`. That file is not ported yet, so the
/// four-line rule is inlined here. It is the ONE definition on the TS side and
/// this must not be allowed to drift from it: `returns > 0`, not a structural
/// veneer, not a data walk, not contained by anything.
///
/// PORT NOTE: the TS reads untyped `JSON.parse` output, so a row missing
/// `returns` evaluates `undefined <= 0` — which is `false` in JS, i.e. the row
/// PASSES. `unwrap_or(false)` below reproduces that rather than the more
/// obvious "missing means reject". Same for `NaN`.
pub fn is_convertible_row(row: &Value) -> bool {
    let returns_non_positive = row
        .get("returns")
        .and_then(Value::as_f64)
        .map(|value| value <= 0.0)
        .unwrap_or(false);
    if returns_non_positive {
        return false;
    }
    let flagged = |key: &str| row.get(key).map(Value::truthy).unwrap_or(false);
    if flagged("structural_veneer") || flagged("data_walk") {
        return false;
    }
    let contained = row
        .get("contained_by")
        .and_then(Value::as_array)
        .map(<[Value]>::len)
        .unwrap_or(0);
    contained == 0
}

/// Alias kept because the TS exports both names and the second is the one the
/// self-test speaks in.
pub fn is_strict_row(row: &Value) -> bool {
    is_convertible_row(row)
}

/// Whether a row's *start* address lands inside one of its overlay's audited
/// executable intervals.
///
/// The intervals in `metrics/gs1-en-executable.json` do not tile their overlay:
/// small unclassified gaps sit between them, and a row that begins in one of
/// those gaps cannot be attributed by the report writer. Only the start matters
/// — a row that begins inside an interval and runs past its end is fine — which
/// is why this is not a containment test.
pub fn starts_in_audited_span(address: i64, intervals: &[Interval]) -> bool {
    intervals
        .iter()
        .any(|interval| interval.start <= address && address < interval.end)
}

fn read_json(path: &Path) -> Result<Value, String> {
    // PORT NOTE: `readFileSync(path, "utf8")` is lossy on invalid UTF-8, so
    // `from_utf8_lossy` is the faithful equivalent, not `String::from_utf8`.
    let bytes = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let text = String::from_utf8_lossy(&bytes);
    json::parse(&text).map_err(|error| format!("{}: {error}", path.display()))
}

/// `overlay id -> intervals`, in file order. Never a `HashMap`: the TS builds a
/// `Map` and this keeps the same insertion order in case a caller iterates it.
fn audited_intervals(root: &Path) -> Result<Vec<(String, Vec<Interval>)>, String> {
    let executable = read_json(&root.join("metrics").join("gs1-en-executable.json"))?;
    let overlays = executable
        .get("overlays")
        .and_then(Value::as_array)
        .unwrap_or(&[]);
    let mut out: Vec<(String, Vec<Interval>)> = Vec::with_capacity(overlays.len());
    for overlay in overlays {
        let id = overlay
            .get("id")
            .and_then(Value::as_str)
            .unwrap_or_default()
            .to_string();
        let intervals = overlay
            .get("intervals")
            .and_then(Value::as_array)
            .unwrap_or(&[])
            .iter()
            .map(|interval| Interval {
                start: interval.get("start").and_then(Value::as_f64).unwrap_or(0.0) as i64,
                end: interval.get("end").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            })
            .collect();
        out.push((id, intervals));
    }
    Ok(out)
}

fn directory_names(path: &Path) -> HashSet<String> {
    // Only ever used for membership, so order does not matter here — the one
    // place in this port where a hash set is safe.
    match fs::read_dir(path) {
        Ok(entries) => entries
            .filter_map(Result::ok)
            .map(|entry| entry.file_name().to_string_lossy().into_owned())
            .collect(),
        Err(_) => HashSet::new(),
    }
}

pub fn reading_list(root: &Path) -> Result<Vec<Pairing>, String> {
    let inventory = read_json(&root.join("out").join("decomp").join("overlays.json"))?;
    let functions = inventory
        .get("functions")
        .and_then(Value::as_array)
        .unwrap_or(&[]);
    let semantic = if root.join("semantic").exists() {
        directory_names(&root.join("semantic"))
    } else {
        HashSet::new()
    };
    // PORT NOTE: the TS calls `readdirSync(join(ROOT, "exact"))` with no
    // `existsSync` guard, so a missing `exact/` is a thrown error there. Here it
    // is an explicit error for the same reason: a silently-empty `exact/` would
    // make every already-converted row reappear as work.
    let exact_dir = root.join("exact");
    if !exact_dir.is_dir() {
        return Err(format!(
            "ENOENT: no such file or directory, scandir '{}'",
            exact_dir.display()
        ));
    }
    let exact = directory_names(&exact_dir);

    let audited = audited_intervals(root)?;
    let mut pairings: Vec<Pairing> = Vec::new();
    for row in functions {
        if !is_strict_row(row) {
            continue;
        }
        let overlay = row
            .get("overlay")
            .and_then(Value::as_str)
            .unwrap_or_default()
            .to_string();
        let offset = row.get("offset").and_then(Value::as_f64).unwrap_or(0.0) as i64;
        let start = OVERLAY_BASE + offset;
        let address = format!("{start:08x}");
        let base = format!("{overlay}_c_{address}.c");
        // Already byte-exact: no remaining conversion work here.
        if exact.contains(&base) {
            continue;
        }
        if !semantic.contains(&base) {
            continue;
        }
        let Some((_, intervals)) = audited.iter().find(|(id, _)| *id == overlay) else {
            return Err(format!(
                "{overlay} is in the inventory but not in metrics/gs1-en-executable.json; \
                 the executable map is stale and every row's blocked flag would be a guess"
            ));
        };
        pairings.push(Pairing {
            overlay,
            address: format!("0x{address}"),
            bytes: row.get("span_bytes").and_then(Value::as_f64).unwrap_or(0.0) as i64,
            semantic_source: format!("semantic/{base}"),
            blocked: !starts_in_audited_span(start, intervals),
        });
    }
    Ok(pairings)
}

/// JS `Number.prototype.toLocaleString()` under the en-US default: groups of
/// three, comma separated. Every value passed here is a non-negative byte count.
pub fn locale_string(value: i64) -> String {
    let negative = value < 0;
    let digits = value.unsigned_abs().to_string();
    let mut out = String::new();
    for (index, digit) in digits.chars().enumerate() {
        if index > 0 && (digits.len() - index).is_multiple_of(3) {
            out.push(',');
        }
        out.push(digit);
    }
    if negative {
        format!("-{out}")
    } else {
        out
    }
}

/// JS `String(n).padStart(width)`, which pads with U+0020 on the left and never
/// truncates.
fn pad_start(text: &str, width: usize) -> String {
    let len = text.chars().count();
    if len >= width {
        return text.to_string();
    }
    format!("{}{text}", " ".repeat(width - len))
}

fn json_escape(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    for character in text.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            control if (control as u32) < 0x20 => {
                out.push_str(&format!("\\u{:04x}", control as u32));
            }
            other => out.push(other),
        }
    }
    out
}

/// `JSON.stringify(list, null, 2)` for the fixed `Pairing` shape.
///
/// PORT NOTE: hand-written rather than derived, because the key order
/// (`overlay`, `address`, `bytes`, `semanticSource`, `blocked`) is the field
/// declaration order in the TS interface and any serializer that sorts keys
/// would silently rewrite the output.
pub fn render_json(list: &[Pairing]) -> String {
    if list.is_empty() {
        return "[]".to_string();
    }
    let mut out = String::from("[\n");
    for (index, item) in list.iter().enumerate() {
        out.push_str("  {\n");
        out.push_str(&format!(
            "    \"overlay\": \"{}\",\n",
            json_escape(&item.overlay)
        ));
        out.push_str(&format!(
            "    \"address\": \"{}\",\n",
            json_escape(&item.address)
        ));
        out.push_str(&format!("    \"bytes\": {},\n", item.bytes));
        out.push_str(&format!(
            "    \"semanticSource\": \"{}\",\n",
            json_escape(&item.semantic_source)
        ));
        out.push_str(&format!("    \"blocked\": {}\n", item.blocked));
        out.push_str("  }");
        if index + 1 < list.len() {
            out.push(',');
        }
        out.push('\n');
    }
    out.push(']');
    out
}

/// JS `/^resource_[0-9a-f]+$/` — anchored, ASCII, at least one hex digit.
fn is_overlay_name(argument: &str) -> bool {
    let Some(rest) = argument.strip_prefix("resource_") else {
        return false;
    };
    !rest.is_empty()
        && rest
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
}

/// The report body, as the TS `main` would print it. Returned rather than
/// printed so tests can assemble a list and assert on the exact text.
pub fn render_report(all: &[Pairing], only: Option<&str>, want_blocked: bool) -> String {
    let mut list: Vec<&Pairing> = all
        .iter()
        .filter(|item| item.blocked == want_blocked)
        .collect();
    if let Some(name) = only {
        list.retain(|item| item.overlay == name);
    }

    // Insertion-ordered grouping: the TS builds a `Map` keyed by overlay and the
    // rank sort below is stable, so first-seen order breaks ties. A `HashMap`
    // would scramble equal-byte overlays.
    let mut by_overlay: Vec<(String, Vec<&Pairing>)> = Vec::new();
    for item in &list {
        match by_overlay.iter_mut().find(|(name, _)| *name == item.overlay) {
            Some((_, items)) => items.push(item),
            None => by_overlay.push((item.overlay.clone(), vec![item])),
        }
    }
    let total_of = |items: &Vec<&Pairing>| items.iter().map(|item| item.bytes).sum::<i64>();
    // `Array.prototype.sort` is stable in JS; `sort_by` is stable in Rust.
    by_overlay.sort_by_key(|entry| std::cmp::Reverse(total_of(&entry.1)));

    let mut out = String::new();
    for (overlay, items) in &by_overlay {
        let bytes = total_of(items);
        out.push_str(&format!(
            "{overlay}  {} bytes across {} owners\n",
            locale_string(bytes),
            items.len()
        ));
        if only.is_some() {
            let mut sorted = items.clone();
            sorted.sort_by_key(|item| item.bytes);
            for item in sorted {
                out.push_str(&format!(
                    "    {}  {}  {}\n",
                    item.address,
                    pad_start(&item.bytes.to_string(), 6),
                    item.semantic_source
                ));
            }
        }
    }
    let total: i64 = list.iter().map(|item| item.bytes).sum();
    out.push_str(&format!(
        "\noverlays={} owners={} bytes_with_a_reference={}\n",
        by_overlay.len(),
        list.len(),
        locale_string(total)
    ));
    let held: Vec<&Pairing> = all.iter().filter(|item| item.blocked).collect();
    if !want_blocked && !held.is_empty() {
        let held_bytes: i64 = held.iter().map(|item| item.bytes).sum();
        out.push_str(&format!(
            "held back (start address not in an audited span, --blocked to list): owners={} bytes={}\n",
            held.len(),
            locale_string(held_bytes)
        ));
    }
    out
}

pub struct Options {
    pub self_test: bool,
    pub json: bool,
    pub blocked: bool,
    pub only: Option<String>,
}

pub fn parse_args(args: &[String]) -> Options {
    Options {
        self_test: args.iter().any(|argument| argument == "--self-test"),
        json: args.iter().any(|argument| argument == "--json"),
        blocked: args.iter().any(|argument| argument == "--blocked"),
        only: args
            .iter()
            .find(|argument| is_overlay_name(argument))
            .cloned(),
    }
}

/// The repository root, found by walking up from the binary's working directory
/// until a marker exists. The TS derives it from `import.meta.url`
/// (`tools/overlay/x.ts` -> three `dirname`s up).
pub fn repository_root() -> Result<PathBuf, String> {
    let mut directory = std::env::current_dir().map_err(|error| error.to_string())?;
    loop {
        if directory.join("tools").is_dir() && directory.join("metrics").is_dir() {
            return Ok(directory);
        }
        if !directory.pop() {
            return Err("could not find the repository root from the working directory".to_string());
        }
    }
}

pub fn self_test() -> Result<(), String> {
    let base = |extra: &str| -> Value {
        let text = format!(
            r#"{{"overlay":"resource_000","offset":0,"span_bytes":16,"starts_with_prologue":true,"returns":1,"structural_veneer":false,"data_walk":false,"contained_by":[]{extra}}}"#
        );
        json::parse(&text).expect("fixture parses")
    };
    if !is_strict_row(&base("")) {
        return Err("a plain owner must be strict".to_string());
    }
    if is_strict_row(&base(r#","data_walk":true"#)) {
        return Err("data walks are not owners".to_string());
    }
    if is_strict_row(&base(r#","returns":0"#)) {
        return Err("a non-returning row is not an owner".to_string());
    }
    if is_strict_row(&base(r#","contained_by":["x"]"#)) {
        return Err("a contained row is a fragment, not an owner".to_string());
    }
    // A LEAF MUST BE A QUEUE ROW. The TS assertion used to demand the opposite,
    // which was the conversion queue's prologue-keyed door written down as a
    // test. A leaf returns; that is what makes it a function.
    if !is_strict_row(&base(r#","starts_with_prologue":false"#)) {
        return Err("a returning leaf must be a queue row".to_string());
    }

    // The two intervals below are adjacent-but-not-touching, which is exactly
    // the shape that produced the five backed-out conversions: a two-byte gap at
    // 0x02000110..0x02000112.
    let intervals = [
        Interval {
            start: 0x0200_0100,
            end: 0x0200_0110,
        },
        Interval {
            start: 0x0200_0112,
            end: 0x0200_0200,
        },
    ];
    let checks: [(bool, &str); 7] = [
        (
            !starts_in_audited_span(0x0200_0100, &intervals),
            "an interval's first byte is inside it",
        ),
        (
            !starts_in_audited_span(0x0200_010e, &intervals),
            "a byte before an interval's end is inside it",
        ),
        (
            starts_in_audited_span(0x0200_0110, &intervals),
            "an interval's end byte is past it, not inside it",
        ),
        (
            starts_in_audited_span(0x0200_0111, &intervals),
            "a row starting in the gap is blocked",
        ),
        (
            !starts_in_audited_span(0x0200_0112, &intervals),
            "the byte after the gap is inside the next interval",
        ),
        (
            starts_in_audited_span(0x0200_0200, &intervals),
            "a row starting past the last interval is blocked",
        ),
        (
            starts_in_audited_span(0x0200_0100, &[]),
            "an overlay with no audited intervals blocks everything",
        ),
    ];
    for (failed, message) in checks {
        if failed {
            return Err(message.to_string());
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn row(json_text: &str) -> Value {
        json::parse(json_text).expect("fixture parses")
    }

    fn pairing(overlay: &str, address: &str, bytes: i64, blocked: bool) -> Pairing {
        Pairing {
            overlay: overlay.to_string(),
            address: address.to_string(),
            bytes,
            semantic_source: format!("semantic/{overlay}_c_{}.c", &address[2..]),
            blocked,
        }
    }

    #[test]
    fn the_ported_self_test_passes() {
        self_test().expect("self-test");
    }

    #[test]
    fn a_row_missing_returns_passes_like_undefined_le_zero() {
        // JS: `undefined <= 0` is false, so the row is NOT rejected. This is the
        // one place the faithful port looks wrong on purpose.
        assert!(is_convertible_row(&row(r#"{"structural_veneer":false}"#)));
        assert!(!is_convertible_row(&row(r#"{"returns":-3}"#)));
    }

    #[test]
    fn structural_veneer_rejects() {
        assert!(!is_convertible_row(&row(
            r#"{"returns":1,"structural_veneer":true}"#
        )));
    }

    #[test]
    fn locale_grouping_matches_js() {
        assert_eq!(locale_string(0), "0");
        assert_eq!(locale_string(999), "999");
        assert_eq!(locale_string(1_000), "1,000");
        assert_eq!(locale_string(256_272), "256,272");
        assert_eq!(locale_string(1_234_567), "1,234,567");
    }

    #[test]
    fn pad_start_never_truncates() {
        assert_eq!(pad_start("16", 6), "    16");
        assert_eq!(pad_start("1234567", 6), "1234567");
    }

    #[test]
    fn overlay_argument_pattern_is_anchored_and_hex_only() {
        assert!(is_overlay_name("resource_373"));
        assert!(is_overlay_name("resource_3ce"));
        assert!(!is_overlay_name("resource_3CE"));
        assert!(!is_overlay_name("resource_3g0"));
        assert!(!is_overlay_name("xresource_373"));
        assert!(!is_overlay_name("resource_373x"));
        assert!(!is_overlay_name("resource_"));
    }

    #[test]
    fn report_ranks_overlays_by_bytes_and_lists_only_when_filtered() {
        let all = vec![
            pairing("resource_001", "0x02000010", 10, false),
            pairing("resource_002", "0x02000020", 100, false),
            pairing("resource_002", "0x02000030", 5, false),
            pairing("resource_003", "0x02000040", 7, true),
        ];
        let report = render_report(&all, None, false);
        assert_eq!(
            report,
            "resource_002  105 bytes across 2 owners\n\
             resource_001  10 bytes across 1 owners\n\
             \noverlays=2 owners=3 bytes_with_a_reference=115\n\
             held back (start address not in an audited span, --blocked to list): owners=1 bytes=7\n"
        );

        let focused = render_report(&all, Some("resource_002"), false);
        assert_eq!(
            focused,
            "resource_002  105 bytes across 2 owners\n\
             \u{20}   0x02000030       5  semantic/resource_002_c_02000030.c\n\
             \u{20}   0x02000020     100  semantic/resource_002_c_02000020.c\n\
             \noverlays=1 owners=2 bytes_with_a_reference=105\n\
             held back (start address not in an audited span, --blocked to list): owners=1 bytes=7\n"
        );
    }

    #[test]
    fn blocked_mode_hides_the_held_back_footer() {
        let all = vec![
            pairing("resource_001", "0x02000010", 10, false),
            pairing("resource_003", "0x02000040", 7, true),
        ];
        let report = render_report(&all, None, true);
        assert_eq!(
            report,
            "resource_003  7 bytes across 1 owners\n\
             \noverlays=1 owners=1 bytes_with_a_reference=7\n"
        );
    }

    #[test]
    fn empty_list_still_prints_a_totals_line() {
        assert_eq!(
            render_report(&[], None, false),
            "\noverlays=0 owners=0 bytes_with_a_reference=0\n"
        );
    }

    #[test]
    fn json_output_keeps_declaration_order() {
        let list = vec![pairing("resource_001", "0x02000010", 10, false)];
        assert_eq!(
            render_json(&list),
            "[\n  {\n    \"overlay\": \"resource_001\",\n    \"address\": \"0x02000010\",\n    \"bytes\": 10,\n    \"semanticSource\": \"semantic/resource_001_c_02000010.c\",\n    \"blocked\": false\n  }\n]"
        );
        assert_eq!(render_json(&[]), "[]");
    }

    #[test]
    fn flags_are_read_positionally_like_the_ts() {
        let args: Vec<String> = ["--json", "resource_3ce", "--blocked"]
            .iter()
            .map(|s| s.to_string())
            .collect();
        let options = parse_args(&args);
        assert!(options.json);
        assert!(options.blocked);
        assert!(!options.self_test);
        assert_eq!(options.only.as_deref(), Some("resource_3ce"));
    }
}
