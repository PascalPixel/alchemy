// Python `json.dumps` with default settings, for exactly the value shapes
// c_structure.py emits.
//
// WHY this is here instead of a path dependency on tools-rs/canonical-json:
// that crate declares `version.workspace` / `edition.workspace`, so it only
// builds from inside tools-rs/Cargo.toml's workspace. This crate carries a bare
// `[workspace]` line so it can build concurrently with its siblings, which
// means it cannot inherit those keys and cannot path-depend on a crate that
// does. The same reasoning produced tools-rs/discover/src/json.rs; this file is
// its sibling, retargeted from the TypeScript canonical writer to CPython's
// `json.dumps`.
//
// PORT NOTE: `json.dumps(obj)` uses separators `", "` and `": "` (not the
// compact form), no trailing newline of its own -- `print()` adds the "\n" --
// and `ensure_ascii=True`, which escapes every scalar above U+007E as one or
// two `\uXXXX` units. All three are reproduced below.

/// Object key order is significant: `declarations` is a Python `dict` built by
/// insertion, and the output is compared byte-for-byte against the Python
/// writer, so this is a Vec of pairs and never a map that could reorder.
pub enum Json {
    Bool(bool),
    Int(i64),
    Str(String),
    Null,
    Arr(Vec<Json>),
    Obj(Vec<(String, Json)>),
}

/// String escaping as `json.dumps` performs it with `ensure_ascii=True`.
///
/// PORT NOTE: CPython's C encoder emits `\uXXXX` for every code point above
/// U+007E, splitting non-BMP scalars into a UTF-16 surrogate pair. Rust `char`
/// values are always scalar values, so the lone-surrogate case CPython can hit
/// via `str` is unreachable here; every other case is reproduced exactly.
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
            c if (' '..='~').contains(&c) => out.push(c),
            c => {
                let point = c as u32;
                if point < 0x10000 {
                    out.push_str(&format!("\\u{point:04x}"));
                } else {
                    let rest = point - 0x10000;
                    out.push_str(&format!("\\u{:04x}", 0xd800 + (rest >> 10)));
                    out.push_str(&format!("\\u{:04x}", 0xdc00 + (rest & 0x3ff)));
                }
            }
        }
    }
    out.push('"');
}

fn write(value: &Json, out: &mut String) {
    match value {
        Json::Bool(true) => out.push_str("true"),
        Json::Bool(false) => out.push_str("false"),
        Json::Null => out.push_str("null"),
        Json::Int(number) => out.push_str(&number.to_string()),
        Json::Str(text) => escape(text, out),
        Json::Arr(items) => {
            out.push('[');
            for (index, item) in items.iter().enumerate() {
                if index > 0 {
                    out.push_str(", ");
                }
                write(item, out);
            }
            out.push(']');
        }
        Json::Obj(entries) => {
            out.push('{');
            for (index, (key, item)) in entries.iter().enumerate() {
                if index > 0 {
                    out.push_str(", ");
                }
                escape(key, out);
                out.push_str(": ");
                write(item, out);
            }
            out.push('}');
        }
    }
}

pub fn dumps(value: &Json) -> String {
    let mut out = String::new();
    write(value, &mut out);
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Every check below is counted, and the count is asserted, so that a
    /// future edit which drops a case fails instead of silently passing.
    #[test]
    fn matches_cpython_json_dumps() {
        let mut checks = 0;
        let mut expect = |value: Json, want: &str| {
            assert_eq!(dumps(&value), want);
            checks += 1;
        };

        expect(Json::Null, "null");
        expect(Json::Bool(true), "true");
        expect(Json::Bool(false), "false");
        expect(Json::Int(0), "0");
        expect(Json::Int(-12), "-12");
        expect(Json::Arr(vec![]), "[]");
        expect(Json::Obj(vec![]), "{}");
        // Separators are ", " and ": ", not the compact forms.
        expect(
            Json::Arr(vec![Json::Int(1), Json::Int(2)]),
            "[1, 2]",
        );
        expect(
            Json::Obj(vec![
                ("a".into(), Json::Int(1)),
                ("b".into(), Json::Arr(vec![Json::Str("x".into())])),
            ]),
            "{\"a\": 1, \"b\": [\"x\"]}",
        );
        // Insertion order survives; it is not sorted.
        expect(
            Json::Obj(vec![("b".into(), Json::Int(1)), ("a".into(), Json::Int(2))]),
            "{\"b\": 1, \"a\": 2}",
        );
        expect(Json::Str("a\"b\\c".into()), "\"a\\\"b\\\\c\"");
        expect(Json::Str("\n\r\t\u{8}\u{c}".into()), "\"\\n\\r\\t\\b\\f\"");
        expect(Json::Str("\u{1}".into()), "\"\\u0001\"");
        // ensure_ascii=True: above U+007E becomes \uXXXX ...
        expect(Json::Str("\u{7f}\u{e9}".into()), "\"\\u007f\\u00e9\"");
        // ... and non-BMP becomes a surrogate pair.
        expect(Json::Str("\u{1f600}".into()), "\"\\ud83d\\ude00\"");

        assert_eq!(checks, 15, "a json check was dropped");
    }
}
