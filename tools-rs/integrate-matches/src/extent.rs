//! `mismatch` and `linkedFunctionExtent`, the two exported pure functions.
//!
//! `linkedFunctionExtent` is imported by `tools/lib/candidate_show.ts`,
//! `tools/lib/mode_sweep.ts` and `tools/compiler/compiler_corpus_regression.ts`
//! as well as by this file's own `linkedBytes`. It is re-exported from the
//! crate root as [`crate::linked_function_extent`] so the Rust `candidate-show`
//! port can delete the eighteen inlined lines in its `src/extent.rs`.

use match_m2c::jsstring::{js_split_lines, js_split_whitespace_runs, js_trim};

use crate::jsint::{is_safe_integer, math_max_all, parse_int_hex};
use crate::jsregex::{is_function_symbol, is_text_type};

/// The return of `mismatch`.
///
/// PORT NOTE -- `left` and `right` are `{ left?: number }`, and the
/// length-difference branch reads `left[shared]` where `shared` is the SHORTER
/// length. One of the two indexes is therefore out of range and evaluates to
/// `undefined`, not to a byte and not to a panic. `Option<u8>` is that
/// `undefined`. The self-test asserts `short.left === undefined`, so this is
/// pinned by the tool's own contract, not only by this port.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Mismatch {
    pub offset: usize,
    pub left: Option<u8>,
    pub right: Option<u8>,
}

/// `mismatch(left, right)`.
///
/// PORT NOTE -- checked against the `shared & ~1` off-by-one confirmed in
/// `candidate_show.ts`, which reports a phantom difference on identical
/// odd-length buffers. That defect is NOT present here: the loop bound is a
/// plain `Math.min` and every index below it is compared. `tests/js_traps.rs`
/// pins identical odd-length buffers as equal so a later "unification" with the
/// `candidate_show` helper cannot import the bug.
pub fn mismatch(left: &[u8], right: &[u8]) -> Option<Mismatch> {
    let shared = left.len().min(right.len());
    for offset in 0..shared {
        if left[offset] != right[offset] {
            return Some(Mismatch {
                offset,
                left: Some(left[offset]),
                right: Some(right[offset]),
            });
        }
    }
    if left.len() == right.len() {
        return None;
    }
    Some(Mismatch {
        offset: shared,
        // `.get`, not indexing: exactly one of these is out of range and the
        // TypeScript reads `undefined` there rather than throwing.
        left: left.get(shared).copied(),
        right: right.get(shared).copied(),
    })
}

/// One accepted row of `arm-none-eabi-nm -S --defined-only` output.
struct Entry {
    address: f64,
    size: f64,
    name: String,
}

/// `linkedFunctionExtent(output, target, address, byteLength)`.
///
/// Measures how far the linked `.text` region belonging to the target function
/// and any sibling functions the compiler emitted beside it reaches, so that
/// trailing literal pools or alignment padding are not compared as code.
///
/// PORT NOTE -- the filter order is load-bearing and is preserved exactly.
/// Rows are first narrowed by `fields.length >= 4`, then by the type column,
/// then by the name pattern, and only THEN parsed and range-filtered. A row
/// such as `080770c1 A Func_080770c0` has three fields and is dropped before
/// its `A` type is ever inspected; the tool's own self-test pins that case.
///
/// PORT NOTE -- the range filter runs BEFORE the `isSafeInteger` validation, so
/// a row whose address field parses to `NaN` is silently DISCARDED (every
/// comparison against NaN is false) rather than raising "compiled function
/// symbols differ". Only a NaN or non-positive SIZE reaches the throw, and only
/// when its address happened to parse into range. Reordering these two steps to
/// look tidier would turn a dropped row into an error.
///
/// PORT NOTE -- `Math.max` over the ends, so a NaN size that survived the range
/// filter poisons `end`; `end <= address` is then false and `end - address >
/// byteLength` is false, so a poisoned extent would fall through to
/// `return end - address` as NaN. It cannot happen, because the `isSafeInteger`
/// guard above rejects a NaN size first. [`math_max_all`] still propagates NaN
/// rather than using `f64::max`, which would drop it, so the two guards do not
/// have to be reasoned about together.
pub fn linked_function_extent(
    output: &str,
    target: &str,
    address: f64,
    byte_length: usize,
) -> Result<usize, String> {
    let mut functions: Vec<Entry> = Vec::new();
    for line in js_split_lines(output) {
        // `.filter(Boolean)` drops the empty string and nothing else; a
        // whitespace-only line survives it and is handled by the field count.
        if line.is_empty() {
            continue;
        }
        let fields = js_split_whitespace_runs(js_trim(line));
        if fields.len() < 4 {
            continue;
        }
        let type_field = fields[fields.len() - 2];
        let name_field = fields[fields.len() - 1];
        if !is_text_type(type_field) || !is_function_symbol(name_field) {
            continue;
        }
        let entry = Entry {
            address: parse_int_hex(fields[0]),
            size: parse_int_hex(fields[1]),
            name: name_field.to_string(),
        };
        // `entry.address >= address && entry.address < address + byteLength`.
        // NaN fails both, which is the silent-drop noted above.
        if entry.address >= address && entry.address < address + byte_length as f64 {
            functions.push(entry);
        }
    }

    let has_target = functions
        .iter()
        .any(|entry| entry.name == target && entry.address == address);
    let has_invalid = functions.iter().any(|entry| {
        !is_safe_integer(entry.address) || !is_safe_integer(entry.size) || entry.size <= 0.0
    });
    if !has_target || has_invalid {
        return Err("compiled function symbols differ".to_string());
    }

    let ends: Vec<f64> = functions.iter().map(|entry| entry.address + entry.size).collect();
    let end = math_max_all(&ends);
    if end <= address || end - address > byte_length as f64 {
        return Err("compiled function extent differs".to_string());
    }
    Ok((end - address) as usize)
}
