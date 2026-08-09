//! `interface Result`, `cachedResult` and `atomicJson`.

use crate::jsvalue::{canonical_json, number, object, parse, string, Json};
use std::path::Path;

/// `interface Result`.
///
/// PORT NOTE -- the optional fields are OMITTED, not written as `null`. The
/// legacy implementation builds the object with a spread,
/// `...(difference.first === null ? {} : { first_difference: difference.first })`,
/// so the key is absent when there is no difference. An `Option` serialised as
/// `null` would change the report bytes and every cache key that reads it back.
#[derive(Debug, Clone, PartialEq)]
pub struct Outcome {
    pub stem: String,
    pub source: String,
    pub cache_key: String,
    pub cached: bool,
    pub compiled: bool,
    pub exact: bool,
    pub expected_size: usize,
    pub actual_size: Option<usize>,
    pub differing_bytes: Option<usize>,
    pub first_difference: Option<usize>,
    pub error: Option<String>,
}

impl Outcome {
    /// Emit in the exact key order the object literals use. Key order is the
    /// file format.
    pub fn to_json(&self) -> Json {
        let mut entries: Vec<(&str, Json)> = vec![
            ("stem", string(&self.stem)),
            ("source", string(&self.source)),
            ("cache_key", string(&self.cache_key)),
            ("cached", Json::Bool(self.cached)),
            ("compiled", Json::Bool(self.compiled)),
            ("exact", Json::Bool(self.exact)),
            ("expected_size", number(self.expected_size)),
        ];
        if let Some(value) = self.actual_size {
            entries.push(("actual_size", number(value)));
        }
        if let Some(value) = self.differing_bytes {
            entries.push(("differing_bytes", number(value)));
        }
        if let Some(value) = self.first_difference {
            entries.push(("first_difference", number(value)));
        }
        if let Some(value) = &self.error {
            entries.push(("error", string(value)));
        }
        object(entries)
    }
}

/// `cachedResult(document, key)`, with branch-specific cache schemas.
pub fn cached_result(document: &Json, key: &str) -> Option<Outcome> {
    // `document === null || typeof document !== "object"` -- note that
    // `typeof null` is `"object"`, which is why `null` is tested separately.
    // An ARRAY passes `typeof === "object"` and would fall through to the
    // field tests, which it then fails. Reproduced by matching on `Object`
    // only after the array has had its chance to fail.
    if !matches!(document, Json::Object(_)) {
        return None;
    }
    let text = |field: &str| match document.get(field) {
        Some(Json::String(value)) => Some(value.clone()),
        _ => None,
    };
    let flag = |field: &str| match document.get(field) {
        Some(Json::Bool(value)) => Some(*value),
        _ => None,
    };
    if text("cache_key").as_deref() != Some(key) {
        return None;
    }
    let stem = text("stem")?;
    let source = text("source")?;
    let cached = flag("cached")?;
    let compiled = flag("compiled")?;
    let exact = flag("exact")?;
    let count = |field: &str| nonnegative_integer(document.get(field));
    let expected_size = count("expected_size")?;
    if !compiled {
        if exact
            || text("error").is_none()
            || document.get("actual_size").is_some()
            || document.get("differing_bytes").is_some()
            || document.get("first_difference").is_some()
        {
            return None;
        }
    } else {
        if text("error").is_some() {
            return None;
        }
        let actual_size = count("actual_size")?;
        let differing_bytes = count("differing_bytes")?;
        if exact {
            if actual_size != expected_size
                || differing_bytes != 0
                || document.get("first_difference").is_some()
            {
                return None;
            }
        } else {
            let first_difference = count("first_difference")?;
            if differing_bytes == 0 || first_difference >= actual_size.max(expected_size) {
                return None;
            }
        }
    }
    Some(Outcome {
        stem,
        source,
        cache_key: key.to_string(),
        cached,
        compiled,
        exact,
        expected_size,
        actual_size: count("actual_size"),
        differing_bytes: count("differing_bytes"),
        first_difference: count("first_difference"),
        error: text("error"),
    })
}

fn nonnegative_integer(value: Option<&Json>) -> Option<usize> {
    let Json::Number(number) = value? else {
        return None;
    };
    (number.is_finite() && *number >= 0.0 && number.fract() == 0.0).then_some(*number as usize)
}

/// Read and validate a cache file. Missing, corrupt, truncated, and semantically
/// invalid entries are cache misses; filesystem failures remain errors.
pub fn read_cache(path: &str, key: &str) -> Result<Option<Outcome>, String> {
    let bytes = match std::fs::read(path) {
        Ok(bytes) => bytes,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => return Err(format!("{path}: {error}")),
    };
    let text = match String::from_utf8(bytes) {
        Ok(text) => text,
        Err(_) => return Ok(None),
    };
    let document = match parse(&text) {
        Ok(document) => document,
        Err(_) => return Ok(None),
    };
    Ok(cached_result(&document, key))
}

/// `atomicJson(path, value)`.
///
/// PORT NOTE -- the temporary name is `${path}.${process.pid}.tmp`, which is
/// NONDETERMINISTIC by construction. It never survives the `rename`, so it
/// does not reach any artifact; the differential comparison harness does not have to normalise
/// it, and it is named here only so nobody adds it to a manifest later.
pub fn atomic_json(path: &str, value: &Json) -> Result<(), String> {
    let target = Path::new(path);
    if let Some(parent) = target.parent() {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    let temporary = format!("{path}.{}.tmp", std::process::id());
    let body = canonical_json(value)? + "\n";
    std::fs::write(&temporary, body).map_err(|error| format!("{temporary}: {error}"))?;
    std::fs::rename(&temporary, path).map_err(|error| format!("{path}: {error}"))?;
    Ok(())
}
