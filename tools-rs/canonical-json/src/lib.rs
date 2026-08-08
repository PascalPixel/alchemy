// Compact canonical form: objects one key per line, arrays of primitives
// inline on one line. This replaces pretty-printing with two-space indent as
// the writer default: the old form spent one line per array element, which put
// two million lines of numeric bulk into the tracked tree. Checkers accept the
// compact form, the fully minified form, and (during migration) the legacy
// two-space form.
//
// Ported from tools/lib/canonical_json.ts. Key order is significant, so the
// serde_json dependency is built with `preserve_order`; a port that sorted keys
// would rewrite every tracked metrics file on first run.

use serde_json::Value;

fn is_primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

fn encode_scalar(value: &Value) -> String {
    serde_json::to_string(value).expect("a scalar always serializes")
}

fn reflow(value: &Value, indent: &str) -> String {
    if is_primitive(value) {
        return encode_scalar(value);
    }

    let inner = format!("{indent}  ");

    match value {
        Value::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(is_primitive) {
                let parts: Vec<String> = items.iter().map(encode_scalar).collect();
                return format!("[{}]", parts.join(", "));
            }
            let parts: Vec<String> = items
                .iter()
                .map(|item| format!("{inner}{}", reflow(item, &inner)))
                .collect();
            format!("[\n{}\n{indent}]", parts.join(",\n"))
        }
        Value::Object(entries) => {
            if entries.is_empty() {
                return "{}".to_string();
            }
            let parts: Vec<String> = entries
                .iter()
                .map(|(key, item)| {
                    let key = Value::String(key.clone());
                    format!("{inner}{}: {}", encode_scalar(&key), reflow(item, &inner))
                })
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!("primitives are handled above"),
    }
}

pub fn canonical_json(value: &Value) -> String {
    reflow(value, "")
}

/// Checkers accept three spellings while the migration runs: the canonical
/// compact form, the fully minified form, and the legacy two-space form.
pub fn is_canonical_json_text(text: &str, value: &Value) -> bool {
    let compact = format!("{}\n", canonical_json(value));
    if text == compact {
        return true;
    }
    if serde_json::to_string(value).is_ok_and(|s| format!("{s}\n") == text) {
        return true;
    }
    serde_json::to_string_pretty(value).is_ok_and(|s| format!("{s}\n") == text)
}

#[cfg(test)]
mod tests {
    use super::*;
    use serde_json::json;

    #[test]
    fn primitives_and_empties() {
        assert_eq!(canonical_json(&json!(null)), "null");
        assert_eq!(canonical_json(&json!(1)), "1");
        assert_eq!(canonical_json(&json!("a")), "\"a\"");
        assert_eq!(canonical_json(&json!(true)), "true");
        assert_eq!(canonical_json(&json!([])), "[]");
        assert_eq!(canonical_json(&json!({})), "{}");
    }

    #[test]
    fn primitive_arrays_stay_on_one_line() {
        assert_eq!(canonical_json(&json!([1, 2, 3])), "[1, 2, 3]");
        assert_eq!(canonical_json(&json!(["a", "b"])), "[\"a\", \"b\"]");
    }

    #[test]
    fn objects_take_one_key_per_line() {
        assert_eq!(canonical_json(&json!({"a": 1})), "{\n  \"a\": 1\n}");
    }

    #[test]
    fn nested_arrays_indent() {
        assert_eq!(
            canonical_json(&json!({"a": [[1], [2]]})),
            "{\n  \"a\": [\n    [1],\n    [2]\n  ]\n}"
        );
    }

    #[test]
    fn key_order_is_preserved_not_sorted() {
        let value: Value = serde_json::from_str(r#"{"b":1,"a":2}"#).unwrap();
        assert_eq!(canonical_json(&value), "{\n  \"b\": 1,\n  \"a\": 2\n}");
    }

    #[test]
    fn all_three_migration_spellings_are_accepted() {
        let value = json!({"a": [1, 2]});
        assert!(is_canonical_json_text(&format!("{}\n", canonical_json(&value)), &value));
        assert!(is_canonical_json_text(
            &format!("{}\n", serde_json::to_string(&value).unwrap()),
            &value
        ));
        assert!(is_canonical_json_text(
            &format!("{}\n", serde_json::to_string_pretty(&value).unwrap()),
            &value
        ));
        assert!(!is_canonical_json_text("{}\n", &value));
    }
}
