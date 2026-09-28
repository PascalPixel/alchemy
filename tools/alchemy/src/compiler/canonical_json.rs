// Canonical JSON uses two-space indentation and packs short data tuples.
// `preserve_order` is required because key reordering rewrites tracked metrics.

use serde_json::Value;
use std::path::Path;

fn is_primitive(value: &Value) -> bool {
    !matches!(value, Value::Array(_) | Value::Object(_))
}

fn encode_scalar(value: &Value) -> String {
    serde_json::to_string(value).expect("a scalar always serializes")
}

const LINE_WIDTH: usize = 120;

fn inline(value: &Value, budget: usize) -> Option<String> {
    let mut text = String::new();
    let close = match value {
        Value::Array(items) => {
            text.push('[');
            for (index, item) in items.iter().enumerate() {
                if index != 0 {
                    text.push_str(", ");
                }
                text.push_str(&inline(item, budget.checked_sub(text.len() + 1)?)?);
            }
            ']'
        }
        Value::Object(entries) => {
            text.push('{');
            for (index, (key, item)) in entries.iter().enumerate() {
                if index != 0 {
                    text.push_str(", ");
                }
                text.push_str(&serde_json::to_string(key).ok()?);
                text.push_str(": ");
                text.push_str(&inline(item, budget.checked_sub(text.len() + 1)?)?);
            }
            '}'
        }
        _ => {
            let text = encode_scalar(value);
            return (text.len() <= budget).then_some(text);
        }
    };
    text.push(close);
    (text.len() <= budget).then_some(text)
}

fn reflow(value: &Value, indent: &str, available: usize) -> String {
    if is_primitive(value) {
        return encode_scalar(value);
    }

    let inner = format!("{indent}  ");

    match value {
        Value::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            let budget = LINE_WIDTH.saturating_sub(inner.len());
            if let Some(text) = inline(value, available) {
                return text;
            }
            let packed = items.iter().all(|item| {
                is_primitive(item)
                    || item
                        .as_array()
                        .is_some_and(|row| row.iter().all(is_primitive))
            });
            if packed {
                let mut lines = Vec::new();
                let mut line = inner.clone();
                for (index, item) in items.iter().enumerate() {
                    let text = inline(item, budget).unwrap_or_else(|| reflow(item, &inner, budget));
                    let comma = if index + 1 == items.len() { "" } else { "," };
                    if line.len() > inner.len()
                        && line.len() + 1 + text.len() + comma.len() > LINE_WIDTH
                    {
                        lines.push(line);
                        line = inner.clone();
                    }
                    if line.len() > inner.len() {
                        line.push(' ');
                    }
                    line.push_str(&text);
                    line.push_str(comma);
                }
                lines.push(line);
                return format!("[\n{}\n{indent}]", lines.join("\n"));
            }
            let parts: Vec<String> = items
                .iter()
                .map(|item| {
                    format!(
                        "{inner}{}",
                        inline(item, budget).unwrap_or_else(|| reflow(item, &inner, budget))
                    )
                })
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
                    let lead = format!("{inner}{}: ", encode_scalar(&key));
                    let budget = LINE_WIDTH.saturating_sub(lead.len());
                    format!(
                        "{lead}{}",
                        inline(item, budget).unwrap_or_else(|| reflow(item, &inner, budget))
                    )
                })
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!("primitives are handled above"),
    }
}

pub fn canonical_json(value: &Value) -> String {
    reflow(value, "", LINE_WIDTH)
}

pub fn write_canonical(path: &Path, value: &Value) -> Result<(), String> {
    let temporary =
        tempfile::NamedTempFile::new_in(path.parent().ok_or("JSON output has no parent")?)
            .map_err(|error| error.to_string())?;
    std::fs::write(temporary.path(), format!("{}\n", canonical_json(value)))
        .map_err(|error| format!("{}: {error}", path.display()))?;
    temporary.persist(path).map_err(|error| error.to_string())?;
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn data_tuples_pack_without_changing_values_or_key_order() {
        let value = serde_json::json!({"z":vec![serde_json::json!(["c",3,7]);100],"a":[{"id":1,"flags":2},{"id":2,"flags":4}]});
        let text = canonical_json(&value);
        assert_eq!(serde_json::from_str::<Value>(&text).unwrap(), value);
        assert!(text.lines().count() < 25);
        assert!(text.lines().all(|line| line.len() <= LINE_WIDTH));
        assert!(text.find("\"z\"").unwrap() < text.find("\"a\"").unwrap());
        assert_eq!(
            canonical_json(&serde_json::from_str::<Value>(&text).unwrap()),
            text
        );
    }
}
