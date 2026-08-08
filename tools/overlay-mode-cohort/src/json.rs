//! Ordered-JSON helpers shared by the score, cache and report layers.
//!
//! `Json` itself comes from `search-compiler-modes`, which models an object as
//! `Vec<(String, Json)>`. That is not a stylistic choice: JavaScript object key
//! order is insertion order, `canonicalJson` emits keys in that order, and the
//! result is HASHED INTO THE CACHE KEY. A `HashMap` here would produce a
//! different key on every run of the same input.

use search_compiler_modes::Json;

/// `object[key] = value` with JavaScript's assignment semantics: an existing
/// key keeps its ORIGINAL POSITION and only its value changes; a new key is
/// appended.
///
/// PORT NOTE: this is what `{ ...cached, cached: true }` does. `cached` is
/// already present in every cached row, so the spread does not move it to the
/// end. Appending instead would reorder the on-disk report relative to the
/// TypeScript for every cache hit.
pub fn set(object: &mut Json, key: &str, value: Json) {
    if let Json::Object(entries) = object {
        for entry in entries.iter_mut() {
            if entry.0 == key {
                entry.1 = value;
                return;
            }
        }
        entries.push((key.to_string(), value));
    }
}

pub fn get<'a>(object: &'a Json, key: &str) -> Option<&'a Json> {
    object.get(key)
}

pub fn as_str(value: Option<&Json>) -> Option<&str> {
    match value {
        Some(Json::String(text)) => Some(text.as_str()),
        _ => None,
    }
}

pub fn as_bool(value: Option<&Json>) -> Option<bool> {
    match value {
        Some(Json::Bool(flag)) => Some(*flag),
        _ => None,
    }
}

/// `typeof value === "number"`.
///
/// PORT NOTE: JavaScript has one number type, so a value written as `2` and a
/// value written as `2.0` are indistinguishable and both pass
/// `typeof x === "number"`. NaN and Infinity also pass, even though
/// `JSON.stringify` writes them as `null` and they therefore cannot survive a
/// round trip through the cache file. The `null` they come back as is NOT a
/// number, so `acceptedScore` rejects the row and it is recompiled: that is the
/// existing behaviour and it is reproduced rather than repaired.
pub fn as_number(value: Option<&Json>) -> Option<f64> {
    match value {
        Some(Json::Number(number)) => Some(*number),
        _ => None,
    }
}

/// `value ?? fallback` over a possibly-absent number.
///
/// PORT NOTE: `??` tests for `null` and `undefined` only. A missing key and an
/// explicit `null` both take the fallback here, which matches, because
/// `JSON.parse` turns the written `null` back into `null`.
pub fn number_or(value: Option<&Json>, fallback: f64) -> f64 {
    match value {
        Some(Json::Number(number)) => *number,
        _ => fallback,
    }
}

pub fn array_of_strings(items: &[String]) -> Json {
    Json::Array(items.iter().map(|item| Json::String(item.clone())).collect())
}

/// `value.length` on a JSON array, 0 for anything else.
pub fn array_len(value: Option<&Json>) -> usize {
    match value {
        Some(Json::Array(items)) => items.len(),
        _ => 0,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn spread_overwrite_keeps_the_original_key_position() {
        let mut row = Json::Object(vec![
            ("id".into(), Json::String("x".into())),
            ("cached".into(), Json::Bool(false)),
            ("compiled".into(), Json::Bool(true)),
        ]);
        set(&mut row, "cached", Json::Bool(true));
        let keys: Vec<&str> = match &row {
            Json::Object(entries) => entries.iter().map(|(k, _)| k.as_str()).collect(),
            _ => unreachable!(),
        };
        assert_eq!(keys, vec!["id", "cached", "compiled"]);
        assert_eq!(as_bool(get(&row, "cached")), Some(true));
    }

    #[test]
    fn a_new_key_is_appended() {
        let mut row = Json::Object(vec![("id".into(), Json::String("x".into()))]);
        set(&mut row, "cached", Json::Bool(true));
        let keys: Vec<&str> = match &row {
            Json::Object(entries) => entries.iter().map(|(k, _)| k.as_str()).collect(),
            _ => unreachable!(),
        };
        assert_eq!(keys, vec!["id", "cached"]);
    }

    #[test]
    fn null_is_not_a_number_so_a_nan_row_is_rejected() {
        // `JSON.stringify(NaN)` is `null`; the parsed value fails the
        // `typeof === "number"` gate on the way back in.
        assert_eq!(as_number(Some(&Json::Null)), None);
        assert_eq!(number_or(Some(&Json::Null), f64::INFINITY), f64::INFINITY);
        assert_eq!(number_or(None, f64::INFINITY), f64::INFINITY);
    }
}
