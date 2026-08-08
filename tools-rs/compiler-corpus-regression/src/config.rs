//! `flagsOf`, `compilerFamilyOf` and `compilerConfigurationOf`.

use crate::jsvalue::Json;
use alchemy_plan::plan::CompilerFamily;
use match_m2c::verify::CandidateCompilerConfiguration;

/// `flagsOf(value)`.
///
/// PORT NOTE -- `[...new Set(value)]` dedupes in INSERTION order. A `HashSet`
/// would reorder the flags, and gcc is later-flag-wins, so reordering
/// `["-O2", "-O1"]` changes the machine code this tool exists to compare. The
/// dedupe is an insertion-ordered `Vec` walk for that reason.
///
/// PORT NOTE -- `Set` uses SameValueZero, not `PartialEq`. That distinction
/// bites for NaN and for +0/-0; here the element type is `String`, where
/// SameValueZero and `==` coincide, so the `Vec` walk is exact.
pub fn flags_of(value: &Json) -> Result<Vec<String>, String> {
    let Json::Array(items) = value else {
        return Err("compiler flags must be an array of non-empty strings".to_string());
    };
    let mut out: Vec<String> = Vec::new();
    for item in items {
        match item {
            Json::String(text) if !text.is_empty() => {
                if !out.iter().any(|seen| seen == text) {
                    out.push(text.clone());
                }
            }
            _ => return Err("compiler flags must be an array of non-empty strings".to_string()),
        }
    }
    Ok(out)
}

/// `compilerFamilyOf(value)`.
pub fn compiler_family_of(value: &Json) -> Result<CompilerFamily, String> {
    let text = match value {
        Json::String(text) => text.as_str(),
        _ => "",
    };
    // PORT NOTE -- the TypeScript compares against six exact lowercase
    // literals with `===`. No case folding, no trimming. `"GCC296"` is
    // rejected there and is rejected here.
    match text {
        "routed" => Ok(CompilerFamily::Routed),
        "gcc296" => Ok(CompilerFamily::Gcc296),
        "old-agbcc" => Ok(CompilerFamily::OldAgbcc),
        "pret-early-thumb" => Ok(CompilerFamily::PretEarlyThumb),
        "gcc2951" => Ok(CompilerFamily::Gcc2951),
        "gcc3" => Ok(CompilerFamily::Gcc3),
        _ => Err(
            "compiler config family must be routed, gcc296, old-agbcc, pret-early-thumb, gcc2951, or gcc3"
                .to_string(),
        ),
    }
}

pub fn family_name(family: CompilerFamily) -> &'static str {
    match family {
        CompilerFamily::Routed => "routed",
        CompilerFamily::Gcc296 => "gcc296",
        CompilerFamily::OldAgbcc => "old-agbcc",
        CompilerFamily::PretEarlyThumb => "pret-early-thumb",
        CompilerFamily::Gcc2951 => "gcc2951",
        CompilerFamily::Gcc3 => "gcc3",
    }
}

const ALLOWED: [&str; 7] = [
    "ids",
    "family",
    "addFlags",
    "removeFlags",
    "compiler_family",
    "flags",
    "remove_flags",
];

/// `compilerConfigurationOf(value)`.
pub fn compiler_configuration_of(value: &Json) -> Result<CandidateCompilerConfiguration, String> {
    let Json::Object(entries) = value else {
        return Err("compiler config must be a JSON object".to_string());
    };
    let get = |key: &str| -> Option<&Json> {
        entries.iter().find(|(k, _)| k == key).map(|(_, v)| v)
    };

    // PORT NOTE -- BUG REPRODUCED. When `config` is present the function
    // RECURSES INTO IT AND RETURNS, so every sibling key is silently ignored:
    // the unknown-field check below never runs, and
    // `{"config":{...},"nonsense":1}` is accepted. That is `:105` in the
    // TypeScript and it is reproduced, not fixed. `undefined` and an explicit
    // `null` differ: `document.config !== undefined` is TRUE for `null`, so
    // `{"config": null}` recurses into `null` and fails with "compiler config
    // must be a JSON object". Pinned in `tests/config.rs`.
    if let Some(nested) = get("config") {
        return compiler_configuration_of(nested);
    }

    // PORT NOTE -- `Object.keys(document)` is insertion order and the error
    // message JOINS the unexpected keys in that order. Sorting them, or using
    // a `HashMap`, changes the message text and breaks parity.
    let unexpected: Vec<&str> = entries
        .iter()
        .map(|(k, _)| k.as_str())
        .filter(|key| !ALLOWED.contains(key))
        .collect();
    if !unexpected.is_empty() {
        let plural = if unexpected.len() == 1 { "" } else { "s" };
        return Err(format!(
            "unknown compiler config field{plural}: {}",
            unexpected.join(", ")
        ));
    }

    let family_value = get("family");
    let compiler_family_value = get("compiler_family");
    // PORT NOTE -- the conflict test is `document.family !== document.compiler_family`,
    // a STRICT EQUALITY on JavaScript values. Two structurally equal objects
    // are NOT `===`, so `{"family":{},"compiler_family":{}}` is reported as
    // conflicting even though it looks identical. `Json`'s derived `PartialEq`
    // is structural, so the comparison is written against a reference-ish
    // rule: primitives compare by value (matching `===`), and any non-primitive
    // pair is treated as unequal. Both would be rejected a line later by
    // `compilerFamilyOf` anyway, but the ERROR TEXT differs, and the error text
    // is part of the artifact.
    if let (Some(a), Some(b)) = (family_value, compiler_family_value) {
        if !strict_equals(a, b) {
            return Err("compiler config contains conflicting family fields".to_string());
        }
    }
    if get("addFlags").is_some() && get("flags").is_some() {
        return Err("compiler config contains conflicting added-flag fields".to_string());
    }
    if get("removeFlags").is_some() && get("remove_flags").is_some() {
        return Err("compiler config contains conflicting removed-flag fields".to_string());
    }

    // `document.family ?? document.compiler_family` -- nullish coalescing, so
    // an explicit `null` family FALLS THROUGH to `compiler_family`, while
    // `false` or `0` would not. Reproduced.
    let family_pick = nullish_or(family_value, compiler_family_value);
    let family = match family_pick {
        None => CompilerFamily::Routed,
        Some(v) => compiler_family_of(v)?,
    };
    let add_value = nullish_or(get("addFlags"), get("flags"));
    let remove_value = nullish_or(get("removeFlags"), get("remove_flags"));
    let add_flags = match add_value {
        None => Vec::new(),
        Some(v) => flags_of(v)?,
    };
    let remove_flags = match remove_value {
        None => Vec::new(),
        Some(v) => flags_of(v)?,
    };
    let overlap: Vec<&str> = add_flags
        .iter()
        .filter(|flag| remove_flags.iter().any(|r| r == *flag))
        .map(String::as_str)
        .collect();
    if !overlap.is_empty() {
        return Err(format!(
            "compiler config both adds and removes: {}",
            overlap.join(", ")
        ));
    }
    Ok(CandidateCompilerConfiguration {
        family: Some(family),
        add_flags,
        remove_flags,
    })
}

/// `a ?? b` where absence is `undefined`.
///
/// PORT NOTE -- `??` falls through on `null` AND on `undefined`, but NOT on
/// `false`, `0` or `""`. Only the `null` case distinguishes it from `||`, and
/// only the `false`/`0` case distinguishes it from a plain `is_none`. Both are
/// reachable from a hand-written config file.
fn nullish_or<'a>(left: Option<&'a Json>, right: Option<&'a Json>) -> Option<&'a Json> {
    match left {
        None | Some(Json::Null) => match right {
            None | Some(Json::Null) => None,
            some => some,
        },
        some => some,
    }
}

/// JavaScript `===` restricted to what a parsed JSON document can hold.
fn strict_equals(left: &Json, right: &Json) -> bool {
    match (left, right) {
        (Json::Null, Json::Null) => true,
        (Json::Bool(a), Json::Bool(b)) => a == b,
        // `NaN === NaN` is false, but JSON cannot express NaN, so every
        // `Json::Number` here is finite and `==` is exact.
        (Json::Number(a), Json::Number(b)) => a == b,
        (Json::String(a), Json::String(b)) => a == b,
        // Distinct object/array literals are never `===`.
        _ => false,
    }
}
