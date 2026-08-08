// The TypeScript this crate ports validates UNTRUSTED JSON, so it is written
// against JavaScript's coercion rules rather than against a schema. Two of
// those rules change observable behaviour and cannot be skipped:
//
//   * `RegExp#test(x)` coerces `x` to a string first, so a catalog whose
//     `id` field is missing is tested as the literal text `"undefined"` --
//     which MATCHES `/^[a-z0-9_]+$/` and passes.
//   * There is one number type. `128.0` in a JSON file is `128`, prints as
//     `128`, and satisfies `Number.isSafeInteger`. `serde_json` keeps the
//     `128.0` spelling and would print it back, so integral floats are
//     normalised on the way in.
//
// Everything here exists to keep those two rules, not to be idiomatic Rust.

use serde_json::{Map, Value};

/// Look up a property. `None` is JavaScript's `undefined`; an explicit JSON
/// `null` stays `Some(Value::Null)`, because the original distinguishes them
/// (`entry.segments !== undefined` is TRUE for `null`).
pub fn get<'a>(value: &'a Value, key: &str) -> Option<&'a Value> {
    value.as_object().and_then(|map| map.get(key))
}

pub fn as_str(value: Option<&Value>) -> Option<&str> {
    match value {
        Some(Value::String(text)) => Some(text.as_str()),
        _ => None,
    }
}

pub fn as_number(value: Option<&Value>) -> Option<f64> {
    match value {
        Some(Value::Number(number)) => number.as_f64(),
        _ => None,
    }
}

pub fn as_array(value: Option<&Value>) -> Option<&Vec<Value>> {
    match value {
        Some(Value::Array(items)) => Some(items),
        _ => None,
    }
}

/// `typeof value === "object" && value !== null && !Array.isArray(value)`.
pub fn is_plain_object(value: Option<&Value>) -> bool {
    matches!(value, Some(Value::Object(_)))
}

/// ECMAScript `ToString` for the shapes a parsed JSON document can hold, plus
/// `undefined` for a missing property. Only ever used as the argument of a
/// `RegExp#test`, where the coercion is otherwise invisible.
pub fn to_js_string(value: Option<&Value>) -> String {
    match value {
        None => "undefined".to_string(),
        Some(Value::Null) => "null".to_string(),
        Some(Value::Bool(flag)) => flag.to_string(),
        Some(Value::String(text)) => text.clone(),
        Some(Value::Number(number)) => number_to_js_string(number.as_f64().unwrap_or(f64::NAN)),
        // `Array.prototype.toString` is `join(",")`, and `join` maps
        // null/undefined elements to the empty string.
        Some(Value::Array(items)) => items
            .iter()
            .map(|item| match item {
                Value::Null => String::new(),
                other => to_js_string(Some(other)),
            })
            .collect::<Vec<String>>()
            .join(","),
        Some(Value::Object(_)) => "[object Object]".to_string(),
    }
}

/// ECMAScript `Number::toString` (ES2024 6.1.6.1.20) for the integral range
/// this tool actually produces, with the exponential branches spelled out so a
/// future caller cannot silently get Rust's `{}` formatting instead.
///
/// PORT NOTE: only the finite branches matter here -- every number this crate
/// emits has already passed `Number.isSafeInteger` -- but the shared
/// `canonical-json` crate serialises through `serde_json`, which writes `1.0`
/// where JavaScript writes `1`. Integral floats are therefore folded to
/// integers by [`normalize_numbers`] on the way in, and this function is the
/// reference the tests check that folding against.
pub fn number_to_js_string(value: f64) -> String {
    if value.is_nan() {
        return "NaN".to_string();
    }
    if value == 0.0 {
        return "0".to_string();
    }
    if value < 0.0 {
        return format!("-{}", number_to_js_string(-value));
    }
    if value.is_infinite() {
        return "Infinity".to_string();
    }
    // `{:e}` gives the shortest round-tripping digits in `d.dddde<exp>` form,
    // which is exactly the (s, k, n) triple the specification is written
    // against: `s` is the digit string, `k` its length, `n` the decimal
    // exponent plus one.
    let scientific = format!("{value:e}");
    let (mantissa, exponent) = scientific.split_once('e').expect("`{:e}` always emits an exponent");
    let digits: String = mantissa.chars().filter(|c| *c != '.').collect();
    let digits = digits.trim_end_matches('0');
    let digits = if digits.is_empty() { "0" } else { digits };
    let k = digits.len() as i32;
    let n = exponent.parse::<i32>().expect("`{:e}` always emits an integer exponent") + 1;

    if k <= n && n <= 21 {
        return format!("{}{}", digits, "0".repeat((n - k) as usize));
    }
    if 0 < n && n <= 21 {
        return format!("{}.{}", &digits[..n as usize], &digits[n as usize..]);
    }
    if -6 < n && n <= 0 {
        return format!("0.{}{}", "0".repeat((-n) as usize), digits);
    }
    let sign = if n > 0 { "+" } else { "-" };
    let magnitude = (n - 1).abs();
    if k == 1 {
        return format!("{digits}e{sign}{magnitude}");
    }
    format!("{}.{}e{}{}", &digits[..1], &digits[1..], sign, magnitude)
}

/// Fold integral floats to integers so the shared canonical-JSON writer prints
/// them the way JavaScript does. See the PORT NOTE on [`number_to_js_string`].
pub fn normalize_numbers(value: Value) -> Value {
    match value {
        Value::Number(number) => match number.as_f64() {
            Some(raw) if number.is_f64() && raw.fract() == 0.0 && raw.abs() <= 9.007_199_254_740_992e15 => {
                Value::from(raw as i64)
            }
            _ => Value::Number(number),
        },
        Value::Array(items) => Value::Array(items.into_iter().map(normalize_numbers).collect()),
        Value::Object(entries) => {
            let mut map = Map::new();
            for (key, item) in entries {
                map.insert(key, normalize_numbers(item));
            }
            Value::Object(map)
        }
        other => other,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn regexp_test_sees_the_string_undefined() {
        let object = serde_json::json!({ "present": "ok" });
        assert_eq!(to_js_string(get(&object, "missing")), "undefined");
        assert_eq!(to_js_string(get(&object, "present")), "ok");
        assert_eq!(to_js_string(Some(&Value::Null)), "null");
        assert_eq!(to_js_string(Some(&serde_json::json!([1, 2]))), "1,2");
        assert_eq!(to_js_string(Some(&serde_json::json!({}))), "[object Object]");
    }

    #[test]
    fn number_to_string_matches_ecmascript_on_both_branches() {
        // Fixed-notation branch, including the `1` that Rust would write `1.0`.
        assert_eq!(number_to_js_string(1.0), "1");
        assert_eq!(number_to_js_string(6184.0), "6184");
        assert_eq!(number_to_js_string(0.0), "0");
        assert_eq!(number_to_js_string(-2.5), "-2.5");
        assert_eq!(number_to_js_string(0.000001), "0.000001");
        assert_eq!(number_to_js_string(1e21), "1e+21");
        // Exponential branch, which Rust's `{}` never emits.
        assert_eq!(number_to_js_string(1e-7), "1e-7");
        assert_eq!(number_to_js_string(1.5e-7), "1.5e-7");
        assert_eq!(number_to_js_string(1.2e22), "1.2e+22");
    }

    #[test]
    fn integral_floats_fold_to_integers() {
        let parsed: Value = serde_json::from_str("{\"a\": 128.0, \"b\": [1.0, 2.5]}").expect("valid JSON");
        let folded = normalize_numbers(parsed);
        assert_eq!(serde_json::to_string(&folded).expect("serialises"), "{\"a\":128,\"b\":[1,2.5]}");
    }
}
