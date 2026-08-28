// Canonical JSON keeps object keys one per line and primitive arrays inline.
// `preserve_order` is required because key reordering rewrites tracked metrics.

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
