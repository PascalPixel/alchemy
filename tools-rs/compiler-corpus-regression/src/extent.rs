//! `linkedFunctionExtent` from `tools/lib/integrate_matches.ts:150`.
//!
//! DUPLICATION, DELIBERATE AND TEMPORARY. `tools-rs/integrate-matches` is
//! under concurrent construction by another agent and is off limits for this
//! task, so the one function this tool imports from it is ported here. When
//! that crate lands, this module should be deleted and replaced by a path
//! dependency; it is thirty lines and the tests below move with it.

use crate::jsparse::{is_js_safe_integer, parse_int};

struct Entry {
    address: f64,
    size: f64,
    name: String,
}

/// `linkedFunctionExtent(output, target, address, byteLength)`.
///
/// PORT NOTE -- `output.split(/\r?\n/)`. The `\r?` is present in the source, so
/// CRLF output from a toolchain is handled. It is spelled out below rather
/// than using `str::lines`, which also strips a trailing `\r` but differs on a
/// final newline: `"a\n".split(/\r?\n/)` is `["a", ""]` (two elements, the
/// empty one dropped by `filter(Boolean)`) whereas `"a\n".lines()` yields one.
/// The `filter(Boolean)` makes them agree here, but only by accident, and the
/// accident is not worth relying on.
///
/// PORT NOTE -- `/^[Tt]$/` names BOTH cases explicitly, so the missing `i`
/// flag is not a defect there. `/^Func_[0-9a-f]{8}$/` has NO `i` flag and is
/// lowercase-only, so an uppercase symbol `Func_08ABCDEF` is NOT matched and
/// the entry is silently dropped. That is faithful: the missing-`i` defect
/// class is confirmed four times over in this repository, so each pattern was
/// checked individually rather than as a group, and this one is intentional in
/// effect because every stem the corpus produces is already lowercased.
/// `tests/extent.rs` pins the uppercase drop so a later "obvious fix" fails.
///
/// PORT NOTE -- `$` in JavaScript WITHOUT the `m` flag matches only at the very
/// end of the input, and does NOT match before a trailing `\n` or `\r` the way
/// Perl's does. The field has already been split on whitespace so it cannot
/// contain either, but the anchors are implemented as full-string equality
/// below rather than as a suffix test, which is the same thing done safely.
///
/// PORT NOTE -- `line.trim().split(/\s+/)`. JavaScript's `\s` includes
/// U+00A0, U+FEFF, U+2028, U+2029 and U+3000, and EXCLUDES U+0085; Rust's
/// `char::is_whitespace` is very nearly the reverse. `nm` output is ASCII, but
/// the split routes through `candidate_compiler::jsstring` so that the port has ONE
/// definition of `\s` and a stray NBSP in a mangled symbol splits the same way
/// on both sides.
pub fn linked_function_extent(
    output: &str,
    target: &str,
    address: f64,
    byte_length: usize,
) -> Result<usize, String> {
    let mut functions: Vec<Entry> = Vec::new();
    for line in split_crlf_lines(output) {
        if line.is_empty() {
            continue; // `.filter(Boolean)`
        }
        let fields = candidate_compiler::jsstring::js_split_whitespace_runs(candidate_compiler::jsstring::js_trim(
            line,
        ));
        if fields.len() < 4 {
            continue;
        }
        // `fields.at(-2)` and `fields.at(-1)`. `at(-n)` past the start returns
        // `undefined`; the length guard above makes that unreachable, and the
        // indexing below cannot underflow because of it.
        let kind = fields[fields.len() - 2];
        let name = fields[fields.len() - 1];
        if !(kind == "T" || kind == "t") {
            continue;
        }
        if !is_func_symbol(name) {
            continue;
        }
        // EVERY numeric parse in this port routes through `parse_int`.
        // `Number.parseInt(fields[0], 16)` accepts a `0x` prefix and stops at
        // trailing junk; `i64::from_str_radix` rejects both, and the
        // difference has already cost this repository 1,506 bytes at two call
        // sites.
        let entry = Entry {
            address: parse_int(fields[0], 16).unwrap_or(f64::NAN),
            size: parse_int(fields[1], 16).unwrap_or(f64::NAN),
            name: name.to_string(),
        };
        // `entry.address >= address && entry.address < address + byteLength`.
        // A NaN address fails BOTH comparisons and is filtered out here, which
        // is why the `isSafeInteger` guard below can never see a NaN address
        // but CAN see a NaN size.
        if entry.address >= address && entry.address < address + byte_length as f64 {
            functions.push(entry);
        }
    }

    let has_target = functions
        .iter()
        .any(|entry| entry.name == target && entry.address == address);
    let has_bad = functions.iter().any(|entry| {
        !is_js_safe_integer(Some(entry.address))
            || !is_js_safe_integer(Some(entry.size))
            || entry.size <= 0.0
    });
    if !has_target || has_bad {
        return Err("compiled function symbols differ".to_string());
    }

    // `Math.max(...functions.map(...))`.
    //
    // PORT NOTE -- `Math.max()` with NO arguments is `-Infinity`, and
    // `Math.max` PROPAGATES NaN where `f64::max` returns the non-NaN operand.
    // Neither can be reached here: an empty `functions` makes `has_target`
    // false and returns above, and every surviving size and address is a safe
    // integer. The fold is written with an explicit `-Infinity` seed and a
    // NaN-propagating step anyway, because "unreachable" is a property of the
    // guards above and those guards are the thing a future edit removes.
    let mut end = f64::NEG_INFINITY;
    for entry in &functions {
        let candidate = entry.address + entry.size;
        end = if end.is_nan() || candidate.is_nan() {
            f64::NAN
        } else if candidate > end {
            candidate
        } else {
            end
        };
    }

    // `if (end <= address || end - address > byteLength) throw`.
    //
    // PORT NOTE -- `Infinity - Infinity` is NaN, and `NaN > byteLength` is
    // false, so a poisoned `end` would slip through the SECOND test while the
    // FIRST (`NaN <= address` is also false) lets it past too. Written with
    // explicit NaN handling so the poisoned case is an error rather than a
    // silent `NaN as usize` (which saturates to 0 in Rust and would return a
    // zero-length extent).
    let span = end - address;
    if end.is_nan() || span.is_nan() {
        return Err("compiled function extent differs".to_string());
    }
    if end <= address || span > byte_length as f64 {
        return Err("compiled function extent differs".to_string());
    }
    Ok(span as usize)
}

/// `/^Func_[0-9a-f]{8}$/` -- no `i` flag, checked individually.
fn is_func_symbol(name: &str) -> bool {
    let Some(rest) = name.strip_prefix("Func_") else {
        return false;
    };
    rest.len() == 8
        && rest
            .bytes()
            .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// `text.split(/\r?\n/)`.
fn split_crlf_lines(text: &str) -> Vec<&str> {
    let mut out = Vec::new();
    let bytes = text.as_bytes();
    let mut start = 0usize;
    let mut index = 0usize;
    while index < bytes.len() {
        if bytes[index] == b'\n' {
            let mut end = index;
            if end > start && bytes[end - 1] == b'\r' {
                end -= 1;
            }
            out.push(&text[start..end]);
            start = index + 1;
        }
        index += 1;
    }
    out.push(&text[start..]);
    out
}
