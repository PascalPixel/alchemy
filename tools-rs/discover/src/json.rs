// A four-variant, insertion-ordered JSON value plus the canonical writer.
//
// WHY this is here instead of a path dependency on tools-rs/canonical-json:
// that crate declares `version.workspace` / `edition.workspace`, so it only
// builds from inside tools-rs/Cargo.toml's workspace. This crate carries a bare
// `[workspace]` line so it can build concurrently with its siblings, which
// means it cannot inherit those keys and cannot path-depend on a crate that
// does. PORT NOTE: the reflow rules below are a transcription of
// tools/lib/canonical_json.ts (objects one key per line, arrays of primitives
// inline), restricted to the value shapes `report()` actually emits, and with
// zero third-party dependencies.

/// Object key order is significant: the report is compared byte-for-byte
/// against the TypeScript writer's output, so this is a Vec of pairs and never
/// a map that could reorder.
pub enum Json {
    Null,
    Bool(bool),
    Int(i64),
    Str(String),
    Arr(Vec<Json>),
    Obj(Vec<(String, Json)>),
}

impl Json {
    fn is_primitive(&self) -> bool {
        !matches!(self, Json::Arr(_) | Json::Obj(_))
    }
}

/// JSON string escaping as `JSON.stringify` performs it for a string.
///
/// PORT NOTE: every string this crate emits is ASCII (mode names, seed source
/// labels built from lowercase hex, fixed report keys), so the lone-surrogate
/// and non-BMP corners of `JSON.stringify` cannot be reached. Non-ASCII input
/// is still passed through verbatim, which matches `JSON.stringify` for every
/// scalar above U+001F other than `"` and `\`.
fn escape(text: &str, out: &mut String) {
    out.push('"');
    for character in text.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
}

fn scalar(value: &Json, out: &mut String) {
    match value {
        Json::Null => out.push_str("null"),
        Json::Bool(true) => out.push_str("true"),
        Json::Bool(false) => out.push_str("false"),
        Json::Int(number) => out.push_str(&number.to_string()),
        Json::Str(text) => escape(text, out),
        _ => unreachable!("scalar called on a container"),
    }
}

fn reflow(value: &Json, indent: &str, out: &mut String) {
    if value.is_primitive() {
        scalar(value, out);
        return;
    }
    let inner = format!("{indent}  ");
    match value {
        Json::Arr(items) => {
            if items.is_empty() {
                out.push_str("[]");
                return;
            }
            if items.iter().all(Json::is_primitive) {
                out.push('[');
                for (index, item) in items.iter().enumerate() {
                    if index > 0 {
                        out.push_str(", ");
                    }
                    scalar(item, out);
                }
                out.push(']');
                return;
            }
            out.push_str("[\n");
            for (index, item) in items.iter().enumerate() {
                if index > 0 {
                    out.push_str(",\n");
                }
                out.push_str(&inner);
                reflow(item, &inner, out);
            }
            out.push('\n');
            out.push_str(indent);
            out.push(']');
        }
        Json::Obj(entries) => {
            if entries.is_empty() {
                out.push_str("{}");
                return;
            }
            out.push_str("{\n");
            for (index, (key, item)) in entries.iter().enumerate() {
                if index > 0 {
                    out.push_str(",\n");
                }
                out.push_str(&inner);
                escape(key, out);
                out.push_str(": ");
                reflow(item, &inner, out);
            }
            out.push('\n');
            out.push_str(indent);
            out.push('}');
        }
        _ => unreachable!("primitives are handled above"),
    }
}

pub fn canonical_json(value: &Json) -> String {
    let mut out = String::new();
    reflow(value, "", &mut out);
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn obj(pairs: Vec<(&str, Json)>) -> Json {
        Json::Obj(pairs.into_iter().map(|(k, v)| (k.to_string(), v)).collect())
    }

    #[test]
    fn primitive_arrays_stay_on_one_line() {
        let value = obj(vec![
            ("a", Json::Arr(vec![Json::Int(1), Json::Int(2), Json::Int(3)])),
            ("b", Json::Arr(vec![])),
            ("c", Json::Null),
        ]);
        assert_eq!(
            canonical_json(&value),
            "{\n  \"a\": [1, 2, 3],\n  \"b\": [],\n  \"c\": null\n}"
        );
    }

    #[test]
    fn nested_objects_indent_by_two() {
        let value = obj(vec![(
            "rows",
            Json::Arr(vec![obj(vec![
                ("mode", Json::Str("thumb".into())),
                ("targets", Json::Arr(vec![Json::Int(-2)])),
            ])]),
        )]);
        assert_eq!(
            canonical_json(&value),
            "{\n  \"rows\": [\n    {\n      \"mode\": \"thumb\",\n      \"targets\": [-2]\n    }\n  ]\n}"
        );
    }

    #[test]
    fn keys_keep_insertion_order_rather_than_sorting() {
        let value = obj(vec![("z", Json::Int(1)), ("a", Json::Int(2))]);
        assert_eq!(canonical_json(&value), "{\n  \"z\": 1,\n  \"a\": 2\n}");
    }

    #[test]
    fn strings_escape_the_json_stringify_way() {
        let mut out = String::new();
        escape("a\"b\\c\nd\u{1}", &mut out);
        assert_eq!(out, "\"a\\\"b\\\\c\\nd\\u0001\"");
    }
}
