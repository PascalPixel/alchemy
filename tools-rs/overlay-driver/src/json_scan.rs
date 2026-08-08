// Targeted JSON field extraction for the two ownership tables.
//
// WHY NOT A JSON LIBRARY
// `out/decomp/overlays.json` is ~84 MB and only two of its ~20 per-record fields
// are ever read. Building a value tree for it costs hundreds of megabytes for
// nothing, and pulling in serde_json would also drag the `preserve_order` key
// hazard into a crate that never re-serialises anything. This scanner walks the
// bytes once, captures the fields it wants at record depth, and skips the rest --
// including the nested arrays (`instruction_offsets`) and nested objects
// (`contained_by`) that a naive brace count would trip over.

/// One record's two interesting fields. `None` means the key was absent or held
/// the wrong JSON type, which the TypeScript would surface as `undefined` and
/// therefore never match.
#[derive(Debug, Default, PartialEq)]
pub struct Record {
    pub overlay: Option<String>,
    pub text: Option<String>,
    pub number: Option<i64>,
}

/// Scans `array_key`'s array of objects, capturing `"overlay"`, plus `text_key`
/// (a string field) and `number_key` (a numeric field) when present.
pub fn scan_records(
    bytes: &[u8],
    array_key: &str,
    text_key: &str,
    number_key: &str,
) -> Vec<Record> {
    let mut records = Vec::new();
    let Some(mut i) = find_array(bytes, array_key) else {
        return records;
    };
    loop {
        i = skip_ws(bytes, i);
        match bytes.get(i) {
            Some(b'{') => {
                let (record, next) = scan_object(bytes, i, text_key, number_key);
                records.push(record);
                i = next;
            }
            Some(b',') => i += 1,
            _ => break,
        }
    }
    records
}

fn scan_object(bytes: &[u8], start: usize, text_key: &str, number_key: &str) -> (Record, usize) {
    let mut record = Record::default();
    let mut i = start + 1;
    loop {
        i = skip_ws(bytes, i);
        match bytes.get(i) {
            Some(b'"') => {}
            Some(b',') => {
                i += 1;
                continue;
            }
            Some(b'}') => return (record, i + 1),
            _ => return (record, i),
        }
        let (key, next) = read_string(bytes, i);
        i = skip_ws(bytes, next);
        if bytes.get(i) != Some(&b':') {
            return (record, i);
        }
        i = skip_ws(bytes, i + 1);
        let wants_text = key == "overlay" || key == text_key;
        let wants_number = key == number_key;
        match bytes.get(i) {
            Some(b'"') => {
                let (value, next) = read_string(bytes, i);
                if wants_text {
                    if key == "overlay" {
                        record.overlay = Some(value);
                    } else {
                        record.text = Some(value);
                    }
                }
                i = next;
            }
            Some(b'{') | Some(b'[') => i = skip_container(bytes, i),
            _ => {
                let (value, next) = read_scalar(bytes, i);
                if wants_number {
                    record.number = value.parse::<f64>().ok().map(|n| n as i64);
                }
                i = next;
            }
        }
    }
}

/// Index of the first element inside the array named `array_key`.
///
/// The key name is not unique in these files: `out/decomp/overlays.json` carries
/// a `totals.functions` COUNT (`"functions": 11673`) hundreds of bytes before the
/// `functions` ARRAY. Stopping at the first textual match therefore finds a number
/// and reports zero rows, which silently turned the whole "inventory row" owner
/// class off. Keep scanning until the key is actually followed by `: [`.
fn find_array(bytes: &[u8], array_key: &str) -> Option<usize> {
    let needle = format!("\"{array_key}\"");
    let needle = needle.as_bytes();
    let mut from = 0usize;
    while let Some(hit) = find(&bytes[from..], needle) {
        let at = from + hit;
        let mut i = skip_ws(bytes, at + needle.len());
        if bytes.get(i) == Some(&b':') {
            i = skip_ws(bytes, i + 1);
            if bytes.get(i) == Some(&b'[') {
                return Some(i + 1);
            }
        }
        from = at + needle.len();
    }
    None
}

fn find(haystack: &[u8], needle: &[u8]) -> Option<usize> {
    haystack.windows(needle.len()).position(|w| w == needle)
}

fn skip_ws(bytes: &[u8], mut i: usize) -> usize {
    while matches!(bytes.get(i), Some(b' ') | Some(b'\t') | Some(b'\n') | Some(b'\r')) {
        i += 1;
    }
    i
}

fn read_string(bytes: &[u8], start: usize) -> (String, usize) {
    let mut out = Vec::new();
    let mut i = start + 1;
    while i < bytes.len() {
        match bytes[i] {
            b'"' => break,
            b'\\' => {
                i += 1;
                match bytes.get(i) {
                    Some(b'n') => out.push(b'\n'),
                    Some(b't') => out.push(b'\t'),
                    Some(b'r') => out.push(b'\r'),
                    Some(b'u') => {
                        let hex = std::str::from_utf8(bytes.get(i + 1..i + 5).unwrap_or(b"0000"))
                            .unwrap_or("0000");
                        let code = u32::from_str_radix(hex, 16).unwrap_or(0);
                        let mut buffer = [0u8; 4];
                        out.extend_from_slice(
                            char::from_u32(code)
                                .unwrap_or('\u{fffd}')
                                .encode_utf8(&mut buffer)
                                .as_bytes(),
                        );
                        i += 4;
                    }
                    Some(other) => out.push(*other),
                    None => break,
                }
                i += 1;
            }
            other => {
                out.push(other);
                i += 1;
            }
        }
    }
    // PORT NOTE: `readFileSync(path, "utf8")` is lossy, so invalid UTF-8 becomes
    // U+FFFD in the TypeScript too. `from_utf8_lossy` reproduces that exactly.
    (String::from_utf8_lossy(&out).into_owned(), i + 1)
}

fn read_scalar(bytes: &[u8], start: usize) -> (String, usize) {
    let mut i = start;
    while i < bytes.len() && !matches!(bytes[i], b',' | b'}' | b']') {
        i += 1;
    }
    let text = String::from_utf8_lossy(&bytes[start..i]).trim().to_string();
    (text, i)
}

fn skip_container(bytes: &[u8], start: usize) -> usize {
    let mut depth = 0i32;
    let mut i = start;
    while i < bytes.len() {
        match bytes[i] {
            b'{' | b'[' => {
                depth += 1;
                i += 1;
            }
            b'}' | b']' => {
                depth -= 1;
                i += 1;
                if depth == 0 {
                    return i;
                }
            }
            b'"' => i = read_string(bytes, i).1,
            _ => i += 1,
        }
    }
    i
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reads_manual_regions_shape() {
        let document = br#"{
  "format": 1,
  "manual_regions": [
    { "overlay": "resource_36f", "entry": "0x020001c0", "span_bytes": 120 },
    { "overlay": "resource_380", "entry": "0x02003f24", "evidence": "text" }
  ]
}"#;
        let records = scan_records(document, "manual_regions", "entry", "offset");
        assert_eq!(records.len(), 2);
        assert_eq!(records[0].overlay.as_deref(), Some("resource_36f"));
        assert_eq!(records[0].text.as_deref(), Some("0x020001c0"));
        assert_eq!(records[1].text.as_deref(), Some("0x02003f24"));
    }

    #[test]
    fn nested_arrays_and_objects_do_not_derail_the_record_walk() {
        let document = br#"{"functions":[
  {"id":"a","overlay":"resource_36f","instruction_offsets":[0,2,4],"offset":448,
   "contained_by":[{"overlay":"decoy","offset":9999}],"data_walk":false},
  {"overlay":"resource_37a","offset":16}
]}"#;
        let records = scan_records(document, "functions", "entry", "offset");
        assert_eq!(records.len(), 2);
        assert_eq!(records[0].overlay.as_deref(), Some("resource_36f"));
        assert_eq!(records[0].number, Some(448));
        assert_eq!(records[1].overlay.as_deref(), Some("resource_37a"));
        assert_eq!(records[1].number, Some(16));
    }

    #[test]
    fn braces_inside_strings_are_not_structure() {
        let document = br#"{"functions":[{"overlay":"a}b","offset":1,"note":"] } ["}]}"#;
        let records = scan_records(document, "functions", "entry", "offset");
        assert_eq!(records.len(), 1);
        assert_eq!(records[0].overlay.as_deref(), Some("a}b"));
        assert_eq!(records[0].number, Some(1));
    }

    #[test]
    fn a_same_named_count_before_the_array_does_not_shadow_it() {
        // This is the real out/decomp/overlays.json shape: totals.functions is a
        // COUNT, and the array of the same name comes later.
        let document = br#"{"totals":{"overlays":96,"functions":11673},
"functions":[{"overlay":"resource_fffc","offset":16}]}"#;
        let records = scan_records(document, "functions", "entry", "offset");
        assert_eq!(records.len(), 1);
        assert_eq!(records[0].overlay.as_deref(), Some("resource_fffc"));
        assert_eq!(records[0].number, Some(16));
    }

    #[test]
    fn a_missing_array_yields_no_records() {
        assert!(scan_records(br#"{"format":1}"#, "functions", "entry", "offset").is_empty());
        assert!(scan_records(br#"{"functions":[]}"#, "functions", "entry", "offset").is_empty());
    }
}
