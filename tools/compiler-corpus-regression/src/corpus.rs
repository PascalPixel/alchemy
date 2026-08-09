//! `hexadecimalStem`, `corpus()` and `deterministicSample`.

use crate::cli::Options;
use crate::jsparse::{js_abs, js_greater_than, locale_compare_hex, parse_int};
use crate::jsvalue::{parse, Json};
use std::cmp::Ordering;
use std::path::{Path, PathBuf};

/// `interface Member`.
#[derive(Debug, Clone, PartialEq)]
pub struct Member {
    pub stem: String,
    pub source: String,
    pub address: f64,
    pub size: f64,
}

/// SHA-256 over a length-prefixed sequence of arbitrary byte parts.
pub fn hash(parts: &[&[u8]]) -> String {
    let mut message: Vec<u8> = Vec::new();
    for part in parts {
        message.extend_from_slice(&(part.len() as u64).to_le_bytes());
        message.extend_from_slice(part);
    }
    alchemy_bundle::sha256::hex(&message)
}

/// `hexadecimalStem(value)`.
///
/// PORT NOTE -- `basename(value).replace(/\.[^.]+$/, "").toLowerCase()`, then
/// `/^[0-9a-f]{8}$/.test(stem)`.
///
/// The test regex has NO `i` flag, but `toLowerCase()` runs FIRST, so an
/// uppercase stem is accepted in practice. That ordering is the whole
/// behaviour and it is easy to invert by accident: checking before lowering
/// rejects `08ABCDEF.c`, which the legacy implementation accepts. Pinned in
/// `tests/corpus.rs`.
///
/// PORT NOTE -- `$` without the `m` flag matches ONLY at the very end of the
/// input in JavaScript. It does NOT match before a trailing newline the way
/// Perl's does, so `"0800000a\n"` is REJECTED. The implementation is a
/// full-string equality test rather than a suffix scan, which reproduces that;
/// a `regex` crate port with `$` would have accepted the trailing newline,
/// because there `$` matches before a final `\n` unless `(?-m)` is forced.
///
/// PORT NOTE -- `replace(/\.[^.]+$/, "")` strips only the LAST extension and
/// only when it contains no dot. `"08000000.tar.gz"` becomes `"08000000.tar"`,
/// which then fails the stem test. `Path::file_stem` would strip the same one,
/// but `Path::with_extension("")` behaves differently on leading-dot names, so
/// the replacement is done by hand.
///
/// PORT NOTE -- `toLowerCase()` is full Unicode, not ASCII: `'İ'` (U+0130)
/// lowercases to TWO code points. `str::to_lowercase` agrees with JavaScript
/// here; `to_ascii_lowercase` would not, and would then fail the stem test on
/// input the legacy implementation also fails, so the divergence is invisible in the
/// artifact. `to_lowercase` is used anyway, because "invisible today" is how
/// these get shipped.
pub fn hexadecimal_stem(value: &str) -> Result<String, String> {
    let base = basename(value);
    let stem = strip_last_extension(base).to_lowercase();
    let valid = stem.len() == 8
        && stem
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b));
    if !valid {
        return Err(format!("invalid source/address: {value}"));
    }
    Ok(stem)
}

/// `basename(value)` -- POSIX `path.basename`, the last non-empty segment.
fn basename(value: &str) -> &str {
    let trimmed = value.trim_end_matches('/');
    if trimmed.is_empty() {
        // `basename("/")` is `"/"`, `basename("")` is `""`.
        return if value.is_empty() { "" } else { "/" };
    }
    match trimmed.rfind('/') {
        Some(index) => &trimmed[index + 1..],
        None => trimmed,
    }
}

/// `replace(/\.[^.]+$/, "")`.
fn strip_last_extension(name: &str) -> &str {
    match name.rfind('.') {
        // `[^.]+` requires at least one non-dot character after the dot, and
        // the `$` anchor requires it to run to the end.
        Some(index) if index + 1 < name.len() => &name[..index],
        _ => name,
    }
}

/// `deterministicSample(members, count, seed)`.
///
/// PORT NOTE -- the comparator is
/// `hash(seed, left.stem).localeCompare(hash(seed, right.stem)) || left.stem.localeCompare(right.stem)`.
/// The `||` means a ZERO from the first comparison falls through to the second;
/// it also means a NaN would, but `localeCompare` cannot return one.
///
/// PORT NOTE -- `localeCompare` is ICU collation, a THIRD order distinct from
/// `str::cmp` and from JavaScript's default `sort()`. Both operands here are
/// drawn from `[0-9a-f]`, where ICU root collation and byte order provably
/// agree; `locale_compare_hex` asserts that domain and `tests/collation.rs`
/// proves the agreement exhaustively over the alphabet.
///
/// PORT NOTE -- the legacy implementation recomputes `hash(seed, stem)` INSIDE the
/// comparator, so it performs O(n log n) sha256 calls: roughly 32,000 of them
/// for the 1,456-member corpus. This port decorates once and sorts the pairs.
/// That is NOT a benchmark trick and it is not the memoisation the brief
/// forbids: the comparator is a pure function of the two stems and the sort is
/// stable on both sides, so the OUTPUT SEQUENCE IS IDENTITICAL, and
/// `tests/sample.rs` proves it by running the naive recompute-per-comparison
/// form against this one over the whole corpus manifest. The forbidden
/// memoisation is of `compiler_bundle_signature`, which is left uncached here
/// and measured separately.
///
/// PORT NOTE -- `Array.prototype.sort` has been REQUIRED to be stable since
/// ES2019. `slice::sort_by` is stable; `sort_unstable_by` is not, and would
/// diverge wherever two members share a digest and a stem, which cannot happen
/// here but is one refactor away from mattering.
pub fn deterministic_sample(members: &[Member], count: usize, seed: &str) -> Vec<Member> {
    let mut decorated: Vec<(String, Member)> = members
        .iter()
        .map(|member| {
            (
                hash(&[seed.as_bytes(), member.stem.as_bytes()]),
                member.clone(),
            )
        })
        .collect();
    decorated.sort_by(|(left_key, left), (right_key, right)| {
        match locale_compare_hex(left_key, right_key) {
            Ordering::Equal => locale_compare_hex(&left.stem, &right.stem),
            other => other,
        }
    });
    let mut ordered: Vec<Member> = decorated.into_iter().map(|(_, member)| member).collect();
    // `count === 0 ? ordered : ordered.slice(0, count)`.
    //
    // PORT NOTE -- `Array.prototype.slice(0, n)` CLAMPS when `n` exceeds the
    // length; `ordered[..n]` PANICS. `--sample 100000` against a 1,456-member
    // corpus is an ordinary thing to type, so the clamp is explicit.
    if count != 0 {
        ordered.truncate(count);
    }
    // `.sort((a, b) => a.address - b.address)`.
    //
    // PORT NOTE -- a comparator returning NaN is spec-treated as 0, i.e. "keep
    // the current order". `a.address - b.address` is NaN only if an address is
    // NaN, which `hexadecimalStem` has already excluded. `total_cmp` would
    // order NaN rather than treating it as equal, and `partial_cmp` would
    // panic on the unwrap; the explicit form below models the spec.
    ordered.sort_by(|a, b| {
        let delta = a.address - b.address;
        if delta.is_nan() || delta == 0.0 {
            Ordering::Equal
        } else if delta < 0.0 {
            Ordering::Less
        } else {
            Ordering::Greater
        }
    });
    ordered
}

/// `corpus(options)`.
///
/// The manifest's canonical exact-C owners use the flat `exact/*.c` layout.
/// Keep the prefix and extension checks here so legacy, non-C, and unrelated
/// manifest entries never enter stem or filesystem handling.
///
/// PORT NOTE -- `found` is a `Map` keyed by stem, so a manifest with two
/// regions sharing a stem keeps the LAST one, and the map preserves INSERTION
/// order for the first appearance. A `HashMap` would change which one survives
/// only by accident but would change the pre-sort order always. Insertion-
/// ordered `Vec` here.
pub fn corpus(options: &Options) -> Result<Vec<Member>, String> {
    corpus_at_root(options, alchemy_routing::routing::root())
}

fn corpus_at_root(options: &Options, root: &Path) -> Result<Vec<Member>, String> {
    let text = std::fs::read_to_string(&options.manifest)
        .map_err(|error| format!("{}: {error}", options.manifest))?;
    let document = parse(&text)?;
    let regions = match document.get("regions") {
        Some(Json::Array(items)) => items.clone(),
        // `document.regions ?? []`
        _ => Vec::new(),
    };
    let mut found: Vec<(String, Member)> = Vec::new();
    for region in &regions {
        let Some(Json::String(source_field)) = region.get("source") else {
            // The legacy implementation types `region.source` as `string` and would throw
            // a TypeError on `undefined.startsWith`. Preserved as an error
            // rather than a skip.
            return Err("region.source is not a string".to_string());
        };
        if !source_field.starts_with("exact/") || !source_field.ends_with(".c") {
            continue;
        }
        let stem = hexadecimal_stem(source_field)?;
        let address = parse_int(&stem, 16).unwrap_or(f64::NAN);
        let Some(source) = resolve_exact_source(root, source_field)? else {
            continue;
        };
        if let Some(prefix) = &options.family {
            if !stem.starts_with(prefix) {
                continue;
            }
        }
        if let Some(near) = options.near {
            // `Math.abs(address - near) > radius` -- NaN fails the comparison
            // and the member is KEPT, which `js_greater_than` models and
            // `!(x <= y)` would not.
            if js_greater_than(js_abs(address - near), options.radius) {
                continue;
            }
        }
        if !options.sources.is_empty() && !options.sources.iter().any(|s| s == &stem) {
            continue;
        }
        let size = match region.get("size") {
            Some(Json::Number(value)) => *value,
            // PORT NOTE -- a missing `size` is `undefined` in the legacy implementation
            // and flows into `member.size` untouched. `Number(undefined)` is
            // NaN and NaN poisoning is a confirmed live defect class here, but
            // `member.size` is never arithmetic'd in this file -- it is only
            // read back as `region.size`, and the byte extent comes from the
            // ROM slice instead. Modelled as NaN so it stays visible.
            _ => f64::NAN,
        };
        let member = Member {
            stem: stem.clone(),
            source,
            address,
            size,
        };
        match found.iter_mut().find(|(key, _)| key == &stem) {
            Some(slot) => slot.1 = member, // `Map#set` overwrites, keeps position
            None => found.push((stem, member)),
        }
    }

    let mut members: Vec<Member> = found.into_iter().map(|(_, member)| member).collect();
    members.sort_by(|a, b| {
        let delta = a.address - b.address;
        if delta.is_nan() || delta == 0.0 {
            Ordering::Equal
        } else if delta < 0.0 {
            Ordering::Less
        } else {
            Ordering::Greater
        }
    });
    Ok(members)
}

/// Resolve a manifest source without allowing a manifest entry to choose files
/// outside the repository's canonical exact-C tree. Manifest paths are data,
/// not filesystem paths supplied by the caller, so they are always rooted at
/// the repository root and must remain relative to `exact/`.
fn resolve_exact_source(root: &Path, source_field: &str) -> Result<Option<String>, String> {
    let relative = Path::new(source_field);
    if relative.is_absolute() {
        return Err(format!("exact source must be relative: {source_field}"));
    }

    let components: Vec<_> = relative.components().collect();
    let flat_exact = components.len() == 2
        && matches!(
            components.first(),
            Some(std::path::Component::Normal(part))
                if *part == std::ffi::OsStr::new("exact")
        )
        && matches!(components.get(1), Some(std::path::Component::Normal(_)));
    if !flat_exact {
        return Err(format!(
            "exact source must be a flat exact/<owner>.c path: {source_field}"
        ));
    }

    let canonical_root = std::fs::canonicalize(root).map_err(|error| {
        format!(
            "cannot canonicalize repository root {}: {error}",
            root.display()
        )
    })?;
    let exact_path = canonical_root.join("exact");
    let exact_root = std::fs::canonicalize(&exact_path).map_err(|error| {
        format!(
            "cannot canonicalize exact-C root {}: {error}",
            exact_path.display()
        )
    })?;
    let source_path = canonical_root.join(relative);

    // Inspect links themselves before canonicalization. This catches both a
    // source symlink and a symlinked directory named `exact`, including one
    // which points back into the repository and would otherwise look harmless.
    if let Some(component) = first_symlink_component(&canonical_root, relative)? {
        return Err(format!(
            "exact source uses symlink component {}: {source_field}",
            component.display()
        ));
    }

    let canonical_source = match std::fs::canonicalize(&source_path) {
        Ok(path) => path,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
        Err(error) => {
            return Err(format!(
                "cannot resolve exact source {source_field}: {error}"
            ))
        }
    };
    if !canonical_source.starts_with(&exact_root) {
        return Err(format!(
            "exact source escapes {}: {source_field}",
            exact_root.display()
        ));
    }
    if !canonical_source.is_file() {
        return Ok(None);
    }
    Ok(Some(canonical_source.to_string_lossy().into_owned()))
}

/// Return the first symlink on `base/relative`, if any. `symlink_metadata`
/// deliberately inspects links themselves instead of following them.
fn first_symlink_component(base: &Path, relative: &Path) -> Result<Option<PathBuf>, String> {
    let mut current = base.to_path_buf();
    for component in relative.components() {
        let std::path::Component::Normal(part) = component else {
            continue;
        };
        current.push(part);
        let metadata = match std::fs::symlink_metadata(&current) {
            Ok(metadata) => metadata,
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => return Ok(None),
            Err(error) => {
                return Err(format!(
                    "cannot inspect exact source component {}: {error}",
                    current.display()
                ));
            }
        };
        if metadata.file_type().is_symlink() {
            return Ok(Some(current));
        }
    }
    Ok(None)
}

/// `path.join(a, b)` for the two-argument case this file uses.
pub fn join(base: &Path, tail: &str) -> String {
    let mut path = PathBuf::from(base);
    path.push(tail);
    // `path.join` NORMALISES: `join(root, "src/../exact/x.c")` collapses to
    // `root/exact/x.c`. `PathBuf::push` does NOT, so the `..` is resolved by
    // hand.
    normalise(&path)
}

fn normalise(path: &Path) -> String {
    let mut parts: Vec<&std::ffi::OsStr> = Vec::new();
    let mut absolute = false;
    for component in path.components() {
        match component {
            std::path::Component::RootDir => {
                absolute = true;
                parts.clear();
            }
            std::path::Component::CurDir => {}
            std::path::Component::ParentDir => {
                // `path.join` drops a `..` that would climb past the root, and
                // KEEPS a leading `..` on a relative path.
                match parts.last() {
                    Some(last) if *last != std::ffi::OsStr::new("..") => {
                        parts.pop();
                    }
                    Some(_) => parts.push(std::ffi::OsStr::new("..")),
                    None => {
                        if !absolute {
                            parts.push(std::ffi::OsStr::new(".."));
                        }
                    }
                }
            }
            std::path::Component::Normal(part) => parts.push(part),
            std::path::Component::Prefix(_) => {}
        }
    }
    let joined = parts
        .iter()
        .map(|part| part.to_string_lossy().into_owned())
        .collect::<Vec<_>>()
        .join("/");
    if absolute {
        format!("/{joined}")
    } else if joined.is_empty() {
        ".".to_string()
    } else {
        joined
    }
}

#[cfg(all(test, any(unix, windows)))]
mod tests {
    use super::resolve_exact_source;
    use std::path::Path;

    #[cfg(unix)]
    fn symlink_file(target: &Path, link: &Path) {
        std::os::unix::fs::symlink(target, link).unwrap();
    }

    #[cfg(windows)]
    fn symlink_file(target: &Path, link: &Path) {
        std::os::windows::fs::symlink_file(target, link).unwrap();
    }

    #[test]
    fn resolve_exact_source_rejects_a_symlink_outside_exact() {
        let root = std::env::temp_dir().join(format!(
            "alchemy-corpus-containment-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        ));
        let exact = root.join("exact");
        let outside = root.join("outside");
        std::fs::create_dir_all(&exact).unwrap();
        std::fs::create_dir_all(&outside).unwrap();
        let target = outside.join("08091780.c");
        std::fs::write(&target, "int outside;\n").unwrap();
        symlink_file(&target, &exact.join("08091780.c"));

        let result = resolve_exact_source(&root, "exact/08091780.c");
        assert!(result.unwrap_err().contains("symlink"));
        std::fs::remove_dir_all(root).unwrap();
    }

    #[test]
    fn resolve_exact_source_uses_the_supplied_root_not_the_process_directory() {
        let root = std::env::temp_dir().join(format!(
            "alchemy-corpus-root-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        ));
        let exact = root.join("exact");
        std::fs::create_dir_all(&exact).unwrap();
        let source = exact.join("08091780.c");
        std::fs::write(&source, "int rooted;\n").unwrap();

        let resolved = resolve_exact_source(&root, "exact/08091780.c")
            .unwrap()
            .expect("source exists below the supplied root");
        assert_eq!(Path::new(&resolved), source.canonicalize().unwrap());
        std::fs::remove_dir_all(root).unwrap();
    }
}
