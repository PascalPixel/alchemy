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

/// `cachedResult(document, key)`.
///
/// PORT NOTE -- BUG REPRODUCED. This validates only FOUR fields:
/// `cache_key`, `stem`, `compiled` and `exact`. A cache row that claims
/// `compiled: true, exact: false` but omits `first_difference` is ACCEPTED, and
/// `main()` then evaluates `result.first_difference!.toString(16)` on it, which
/// is a TypeError at runtime. The non-null assertion is the legacy implementation's own,
/// and the crash is reachable from any hand-edited or truncated cache file.
/// Reproduced: the reader below accepts the row, and the reporter surfaces the
/// same failure. Pinned in `tests/cache.rs`.
///
/// PORT NOTE -- `typeof result.stem !== "string"` and friends are the ONLY
/// type tests. `expected_size` is read back untyped and could be a string; the
/// legacy implementation would then print it as one. Modelled by reading it through the
/// same permissive path.
pub fn cached_result(document: &Json, key: &str) -> Option<Outcome> {
    // `document === null || typeof document !== "object"` -- note that
    // `typeof null` is `"object"`, which is why `null` is tested separately.
    // An ARRAY passes `typeof === "object"` and would fall through to the
    // field tests, which it then fails. Reproduced by matching on `Object`
    // only after the array has had its chance to fail.
    if !matches!(document, Json::Object(_) | Json::Array(_)) {
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
    let compiled = flag("compiled")?;
    let exact = flag("exact")?;
    let count = |field: &str| match document.get(field) {
        Some(Json::Number(value)) => Some(*value as usize),
        _ => None,
    };
    Some(Outcome {
        stem,
        source: text("source").unwrap_or_default(),
        cache_key: key.to_string(),
        cached: flag("cached").unwrap_or(false),
        compiled,
        exact,
        expected_size: count("expected_size").unwrap_or(0),
        actual_size: count("actual_size"),
        differing_bytes: count("differing_bytes"),
        first_difference: count("first_difference"),
        error: text("error"),
    })
}

/// Read and validate a cache file.
///
/// PORT NOTE -- BUG REPRODUCED. In the legacy implementation this read sits OUTSIDE the
/// `try` block that guards the compile, so a TRUNCATED OR CORRUPT cache JSON
/// does not fall back to recompiling: `JSON.parse` throws and takes the whole
/// run down with an unhandled rejection. That is a real hazard for a cache
/// directory written by a killed process. The `Err` returned here is
/// propagated the same way rather than being swallowed into a recompile.
pub fn read_cache(path: &str, key: &str) -> Result<Option<Outcome>, String> {
    if !Path::new(path).exists() {
        return Ok(None);
    }
    let text = std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
    let document = parse(&text)?;
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
        std::fs::create_dir_all(parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    let temporary = format!("{path}.{}.tmp", std::process::id());
    let body = canonical_json(value)? + "\n";
    std::fs::write(&temporary, body).map_err(|error| format!("{temporary}: {error}"))?;
    std::fs::rename(&temporary, path).map_err(|error| format!("{path}: {error}"))?;
    Ok(())
}
