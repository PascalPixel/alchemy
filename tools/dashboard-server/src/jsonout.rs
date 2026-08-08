// Compact `JSON.stringify` with JavaScript key order and JavaScript number
// formatting.
//
// WHY a hand-rolled value rather than a serde derive: the dashboard's
// `/snapshot` body and every SSE `data:` frame are consumed byte for byte by
// the browser client. `serde_json` sorts object keys unless the
// `preserve_order` feature is on, and its number writer is ryu, which is not
// ECMAScript `ToString(Number)`. Both would change bytes silently.
//
// PORT NOTE: `JSON.stringify` OMITS a key whose value is `undefined`. That is
// modelled here by `Json::Undefined`, which the writer skips inside an object
// (a bare `Undefined` at the top level would be `undefined`, which this tool
// never emits, so it writes `null` there and a test pins it).

use coverage_map::jsnum::js_number_string;

/// An insertion-ordered JSON value. `Vec<(String, Json)>`, never a `HashMap`:
/// JS object iteration order is insertion order for string keys and it is
/// load-bearing for the compared bytes.
#[derive(Debug, Clone, PartialEq)]
pub enum Json {
    /// A key carrying this is dropped, exactly as `JSON.stringify` drops
    /// `undefined`-valued properties.
    Undefined,
    Null,
    Bool(bool),
    Num(f64),
    Str(String),
    Arr(Vec<Json>),
    Obj(Vec<(String, Json)>),
}

impl Json {
    pub fn obj(members: Vec<(&str, Json)>) -> Json {
        Json::Obj(
            members
                .into_iter()
                .map(|(key, value)| (key.to_string(), value))
                .collect(),
        )
    }

    pub fn str(value: &str) -> Json {
        Json::Str(value.to_string())
    }

    /// `JSON.stringify(value)` with no spacer.
    pub fn stringify(&self) -> String {
        let mut out = String::new();
        write_value(self, &mut out);
        out
    }
}

/// `JSON.stringify` of a string: escape `"`, `\` and the C0 controls only.
///
/// PORT NOTE: V8 also escapes lone surrogates as `\udXXX`. A Rust `String`
/// cannot hold a lone surrogate, so that branch is unreachable here rather
/// than omitted.
pub fn quote(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for character in text.chars() {
        match character {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            other if (other as u32) < 0x20 => {
                out.push_str(&format!("\\u{:04x}", other as u32));
            }
            other => out.push(other),
        }
    }
    out.push('"');
    out
}

/// `JSON.stringify` of a number.
///
/// PORT NOTE: NaN and the infinities serialise as `null`, NOT as `NaN` /
/// `Infinity`. A reader's `typeof x === "number"` gate then sees `null` and
/// silently fails, which is the documented hazard; reproduced faithfully.
fn number(value: f64) -> String {
    if value.is_finite() {
        js_number_string(value)
    } else {
        "null".to_string()
    }
}

fn write_value(value: &Json, out: &mut String) {
    match value {
        // Only reachable at the top level; inside an object or array the
        // caller has already handled it.
        Json::Undefined | Json::Null => out.push_str("null"),
        Json::Bool(flag) => out.push_str(if *flag { "true" } else { "false" }),
        Json::Num(number_value) => out.push_str(&number(*number_value)),
        Json::Str(text) => out.push_str(&quote(text)),
        Json::Arr(items) => {
            out.push('[');
            for (index, item) in items.iter().enumerate() {
                if index > 0 {
                    out.push(',');
                }
                // PORT NOTE: inside an ARRAY, `undefined` becomes `null`, it
                // is not dropped. Different rule from objects, same value.
                match item {
                    Json::Undefined => out.push_str("null"),
                    other => write_value(other, out),
                }
            }
            out.push(']');
        }
        Json::Obj(members) => {
            out.push('{');
            let mut first = true;
            for (key, member) in members {
                if matches!(member, Json::Undefined) {
                    continue;
                }
                if !first {
                    out.push(',');
                }
                first = false;
                out.push_str(&quote(key));
                out.push(':');
                write_value(member, out);
            }
            out.push('}');
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn undefined_members_are_dropped_but_key_order_survives() {
        let value = Json::obj(vec![
            ("page", Json::str("a")),
            ("generatedAt", Json::Undefined),
            ("scanning", Json::Bool(false)),
        ]);
        assert_eq!(value.stringify(), r#"{"page":"a","scanning":false}"#);
    }

    #[test]
    fn insertion_order_is_not_sorted_order() {
        // A naive port on a BTreeMap or on serde_json without
        // `preserve_order` emits these sorted and fails here.
        let value = Json::obj(vec![("zeta", Json::Num(1.0)), ("alpha", Json::Num(2.0))]);
        assert_eq!(value.stringify(), r#"{"zeta":1,"alpha":2}"#);
    }

    #[test]
    fn undefined_inside_an_array_is_null_not_dropped() {
        let value = Json::Arr(vec![Json::Num(1.0), Json::Undefined, Json::Num(2.0)]);
        assert_eq!(value.stringify(), "[1,null,2]");
    }

    #[test]
    fn integral_doubles_lose_their_rust_point_zero() {
        // `format!("{}", 1.0f64)` is "1.0"; JS writes "1".
        assert_eq!(Json::Num(1.0).stringify(), "1");
        assert_eq!(Json::Num(1_345_890.0).stringify(), "1345890");
        assert_eq!(Json::Num(-0.0).stringify(), "0");
    }

    #[test]
    fn the_page_version_float_survives_round_trip() {
        // A real observed `mtimeMs`. Any lossy formatter changes these bytes
        // and the `?v=` cache buster in the document shell drifts with it.
        assert_eq!(
            Json::Num(1_785_756_380_214.988_8).stringify(),
            "1785756380214.9888"
        );
    }

    #[test]
    fn exponent_boundaries_match_javascript() {
        assert_eq!(Json::Num(1e21).stringify(), "1e+21");
        assert_eq!(Json::Num(1e20).stringify(), "100000000000000000000");
        assert_eq!(Json::Num(1e-6).stringify(), "0.000001");
        assert_eq!(Json::Num(1e-7).stringify(), "1e-7");
    }

    #[test]
    fn nan_and_infinity_serialise_as_null() {
        assert_eq!(Json::Num(f64::NAN).stringify(), "null");
        assert_eq!(Json::Num(f64::INFINITY).stringify(), "null");
        assert_eq!(Json::Num(f64::NEG_INFINITY).stringify(), "null");
    }

    #[test]
    fn control_characters_and_quotes_escape_but_astral_text_does_not() {
        assert_eq!(
            quote("a\u{22}b\u{5c}c\nd\u{1}"),
            "\"a\\\"b\\\\c\\nd\\u0001\""
        );
        assert_eq!(
            quote("Reading the stones… ☀️"),
            "\"Reading the stones… ☀️\""
        );
    }
}
