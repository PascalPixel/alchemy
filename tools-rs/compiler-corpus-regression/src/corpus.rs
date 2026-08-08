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

/// `hash(...parts)`: sha256 over each part followed by a NUL separator.
///
/// PORT NOTE -- the trailing `"\0"` after EVERY part, including the last, is
/// what makes the digest unambiguous, and dropping it would change every cache
/// key in `out/`. `alchemy_bundle::sha256::hex` is the portable implementation;
/// see the benchmark note in `README`-less `src/bin/` about how much slower it
/// is than Bun's native `CryptoHasher`.
pub fn hash(parts: &[&[u8]]) -> String {
    let mut message: Vec<u8> = Vec::new();
    for part in parts {
        message.extend_from_slice(part);
        message.push(0);
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
/// rejects `08ABCDEF.c`, which the TypeScript accepts. Pinned in
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
/// input the TypeScript also fails, so the divergence is invisible in the
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
/// PORT NOTE -- the TypeScript recomputes `hash(seed, stem)` INSIDE the
/// comparator, so it performs O(n log n) sha256 calls: roughly 32,000 of them
/// for the 1,456-member corpus. This port decorates once and sorts the pairs.
/// That is NOT a benchmark trick and it is not the memoisation the brief
/// forbids: the comparator is a pure function of the two stems and the sort is
/// stable on both sides, so the OUTPUT SEQUENCE IS IDENTITICAL, and
/// `tests/sample.rs` proves it by running the naive recompute-per-comparison
/// form against this one over the whole corpus manifest. The forbidden
/// memoisation is of `compilerBundleSignature`, which is left uncached here
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
/// PORT NOTE -- BUG REPRODUCED, AND IT IS THE REASON THIS TOOL HAS NEVER RUN.
/// The filter is `region.source.startsWith("src/")`. The manifest at
/// `out/full/claimed/manifest.json` has used the `exact/` prefix since long
/// before this tool was written, and there is NO `src/` directory in the
/// repository at all. The filter therefore matches ZERO regions on the real
/// manifest and `main()` dies at "no exact-C sources matched the selection
/// filters" every single time. This is NOT the `b3ab4841b` path break; `git
/// log -S` puts the `src/` literal in the tool's FIRST commit (`c7b7f3736`).
/// It is reproduced here rather than fixed, per the brief, and reported.
///
/// PORT NOTE -- `isAbsolute(region.source)` on the next line is DEAD CODE for
/// the same reason: a path that starts with `src/` is never absolute, so the
/// true branch is unreachable. Reproduced.
///
/// PORT NOTE -- `found` is a `Map` keyed by stem, so a manifest with two
/// regions sharing a stem keeps the LAST one, and the map preserves INSERTION
/// order for the first appearance. A `HashMap` would change which one survives
/// only by accident but would change the pre-sort order always. Insertion-
/// ordered `Vec` here.
pub fn corpus(options: &Options) -> Result<Vec<Member>, String> {
    let text = std::fs::read_to_string(&options.manifest)
        .map_err(|error| format!("{}: {error}", options.manifest))?;
    let document = parse(&text)?;
    let regions = match document.get("regions") {
        Some(Json::Array(items)) => items.clone(),
        // `document.regions ?? []`
        _ => Vec::new(),
    };

    let root = alchemy_routing::routing::root();
    let mut found: Vec<(String, Member)> = Vec::new();
    for region in &regions {
        let Some(Json::String(source_field)) = region.get("source") else {
            // The TypeScript types `region.source` as `string` and would throw
            // a TypeError on `undefined.startsWith`. Preserved as an error
            // rather than a skip.
            return Err("region.source is not a string".to_string());
        };
        if !source_field.starts_with("src/") || !source_field.ends_with(".c") {
            continue;
        }
        let stem = hexadecimal_stem(source_field)?;
        let address = parse_int(&stem, 16).unwrap_or(f64::NAN);
        let source = if Path::new(source_field).is_absolute() {
            source_field.clone()
        } else {
            join(root, source_field)
        };
        if !Path::new(&source).exists() {
            continue;
        }
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
            // PORT NOTE -- a missing `size` is `undefined` in the TypeScript
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

/// `path.join(a, b)` for the two-argument case this file uses.
pub fn join(base: &Path, tail: &str) -> String {
    let mut path = PathBuf::from(base);
    path.push(tail);
    // `path.join` NORMALISES: `join(root, "src/../exact/x.c")` collapses to
    // `root/exact/x.c`. `PathBuf::push` does NOT, so the `..` is resolved by
    // hand. This matters: it is the only way any input reaches the compile
    // path at all, given the `src/` filter above.
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
