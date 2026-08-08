//! `regionSize(stem)`.

use std::path::Path;

use match_m2c::json::{parse, Json};

/// `regionSize(stem)` -- the first `regions[]` row whose `basename(source,
/// ".s")` equals `stem`.
///
/// The assembly builder already fixed each region's extent; the entry symbol's own
/// size is only the head of it when a trailing function shares the region.
///
/// PORT NOTE -- the two manifest paths are probed with `.find(existsSync)`, so
/// `out/full/asm/manifest.json` WINS over `out/asm/manifest.json` when both
/// exist. Order is behaviour.
///
/// PORT NOTE -- `document.regions ?? []` tolerates a missing key, and the row's
/// `size` is read with no validation at all. A row that omits `size` returns
/// `undefined`, which the caller's `??` then replaces with the linked length,
/// so it is indistinguishable from "no such region" -- reproduced as `None`. A
/// row whose `size` is a string would in JavaScript flow into a string
/// concatenation downstream; nothing in the corpus does that and modelling it
/// would require a variant-typed size, so [`Json::to_js_number`] is applied and
/// a non-numeric size becomes `NaN`, which the `subarray` clamp then treats as
/// 0. That is the one place this port is knowingly narrower than the source.
pub fn region_size(root: &Path, stem: &str) -> Option<f64> {
    let candidates = [
        root.join("out/full/asm/manifest.json"),
        root.join("out/asm/manifest.json"),
    ];
    let path = candidates.iter().find(|p| p.exists())?;
    let text = std::fs::read_to_string(path).ok()?;
    let document = parse(&text).ok()?;
    let regions = match document.get("regions") {
        Some(Json::Array(items)) => items,
        // `?? []` catches `null` and `undefined`; a non-array `regions` would
        // make `for…of` throw in JavaScript. Treated as empty; no manifest in
        // the corpus is shaped that way.
        _ => return None,
    };
    for region in regions {
        let source = match region.get("source") {
            Some(Json::String(s)) => s.as_str(),
            _ => continue,
        };
        if basename_without(source, ".s") == stem {
            return Some(Json::to_js_number(region.get("size")));
        }
    }
    None
}

/// `basename(path, ".s")`.
///
/// PORT NOTE -- Node strips the extension only when it is a proper, non-equal
/// suffix: `basename(".s", ".s")` is `".s"`, not `""`.
pub fn basename_without<'a>(path: &'a str, extension: &str) -> &'a str {
    let base = basename(path);
    if !extension.is_empty() && base.len() > extension.len() && base.ends_with(extension) {
        &base[..base.len() - extension.len()]
    } else {
        base
    }
}

/// `basename(path)` -- POSIX, trailing separators stripped.
pub fn basename(path: &str) -> &str {
    let trimmed = path.trim_end_matches('/');
    if trimmed.is_empty() {
        return if path.is_empty() { "" } else { "/" };
    }
    match trimmed.rfind('/') {
        Some(at) => &trimmed[at + 1..],
        None => trimmed,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn basename_strips_only_a_proper_suffix() {
        assert_eq!(basename_without("asm/080000c0.s", ".s"), "080000c0");
        assert_eq!(basename_without(".s", ".s"), ".s");
        assert_eq!(basename_without("a/b/", ".s"), "b");
        assert_eq!(basename_without("plain", ".s"), "plain");
    }
}
