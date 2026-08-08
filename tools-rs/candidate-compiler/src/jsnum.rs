//! JavaScript number semantics.
//!
//! There is one numeric type in JavaScript and it is `f64`. Every count, size
//! and score historically use that type, and three places in this crate
//! depend on behaviour that `usize`/`i64` do not have: `Number(undefined)` is
//! `NaN` and poisons the score, `Math.max`/`Math.abs` propagate that `NaN`, and
//! the comparator built on top treats a `NaN` result as "not less than", which
//! silently keeps the incumbent. See [`compare_tuple`].

/// `Number.prototype.toString(16)` for a non-negative integral value, then
/// `padStart(8, "0")`.
///
/// PORT NOTE -- `padStart` pads but never truncates, so a value wider than
/// eight hex digits widens the field instead of wrapping. `{:08x}` behaves the
/// same way, which is why it is used, but only after the value is proved to fit
/// `u64`; a wider value would be `Infinity` in JavaScript and is rejected.
pub fn hex8(value: f64) -> String {
    debug_assert!(value >= 0.0 && value.fract() == 0.0);
    format!("{:08x}", value as u64)
}

/// `parseHex` -- `/^[0-9a-f]+$/i` then `Number.parseInt(value, 16)`.
///
/// PORT NOTE -- the guard is written by hand rather than with the `regex`
/// crate on purpose. `[0-9a-f]` with the `i` flag also accepts `A-F`; a port
/// that drops the flag would reject every uppercase stem, which is the exact
/// class of defect (`/^ldr/` missing its `i` beside a `/gi` sibling) already
/// confirmed elsewhere in this repository. `tests/js_traps.rs` pins the
/// uppercase case.
///
/// PORT NOTE -- the guard is ASCII-only, matching JavaScript character classes.
/// Fullwidth digits and Arabic-Indic digits are rejected here as they are there.
pub fn parse_hex(value: &str) -> Result<f64, String> {
    let ok = !value.is_empty()
        && value.bytes().all(|b| {
            b.is_ascii_digit() || (b'a'..=b'f').contains(&b) || (b'A'..=b'F').contains(&b)
        });
    if !ok {
        return Err(format!("invalid hexadecimal value: {value}"));
    }
    // `Number.parseInt(s, 16)` computes the exact mathematical value and then
    // rounds once to the nearest `f64`. Accumulating in `u128` and converting
    // reproduces that exactly for up to 32 hex digits, which covers every stem
    // and every `nm` size field this tool will ever see. Beyond that the
    // accumulate-in-`f64` fallback double-rounds, as would any practical
    // implementation; no input in the corpus reaches it.
    if value.len() <= 32 {
        let mut acc: u128 = 0;
        for b in value.bytes() {
            acc = acc * 16 + u128::from(hex_digit(b));
        }
        return Ok(acc as f64);
    }
    let mut acc = 0.0f64;
    for b in value.bytes() {
        acc = acc * 16.0 + f64::from(hex_digit(b));
    }
    Ok(acc)
}

fn hex_digit(b: u8) -> u8 {
    match b {
        b'0'..=b'9' => b - b'0',
        b'a'..=b'f' => b - b'a' + 10,
        _ => b - b'A' + 10,
    }
}

/// `Math.max(0, x)`.
///
/// PORT NOTE -- `f64::max` returns the non-NaN operand, so `0.0f64.max(f64::NAN)`
/// is `0.0`. `Math.max(0, NaN)` is `NaN`. The difference is the whole reason
/// the poisoned-span defect below reaches the score at all, so this helper
/// exists rather than a call to `f64::max`.
pub fn math_max(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left > right {
        left
    } else {
        right
    }
}

/// `Math.abs(x)`. `f64::abs` already matches, including on NaN; named for
/// symmetry with [`math_max`] so the pair reads as a unit.
pub fn math_abs(value: f64) -> f64 {
    value.abs()
}

/// `compareTuple(left, right)` and the `< 0` test its only caller applies.
///
/// PORT NOTE -- this returns the raw `f64` difference, not an `Ordering`,
/// because the caller's test is `compareTuple(...) < 0` and a `NaN` difference
/// makes that `false`. `partial_cmp` would give `None` and `total_cmp` would
/// order NaN as greater than everything; both would be defensible and both
/// would be wrong. A poisoned score therefore never displaces the incumbent,
/// and the first candidate evaluated wins by default. See
/// [`crate::pipeline::DraftOutcome`] for how a score gets poisoned in the first
/// place.
pub fn compare_tuple(left: [f64; 3], right: [f64; 3]) -> f64 {
    for index in 0..3 {
        // `NaN !== NaN` is true in JavaScript, so a poisoned element always
        // takes this branch and returns a NaN difference. Rust's `!=` on f64
        // agrees exactly.
        if left[index] != right[index] {
            return left[index] - right[index];
        }
    }
    0.0
}

/// `compareTuple(a, b) < 0`, spelled once so the NaN behaviour has a name.
pub fn is_strictly_better(candidate: [f64; 3], incumbent: [f64; 3]) -> bool {
    compare_tuple(candidate, incumbent) < 0.0
}

/// ECMAScript `ToString(Number)` restricted to the values this tool emits.
///
/// PORT NOTE -- `serde_json`/ryu write `1.0` where JavaScript writes `1`, and
/// JavaScript switches to exponential notation below 1e-6 and at or above 1e21
/// where Rust never does. Every number this tool emits is an integer by
/// construction (a parsed hex address, a byte count, a length), so this
/// function refuses a non-integral or non-finite input rather than guessing.
/// `tests/report_json.rs` walks every number in a generated report and fails on
/// anything this rejects, which is the proof that the restriction holds.
pub fn to_js_number_string(value: f64) -> Result<String, String> {
    if !value.is_finite() {
        // `JSON.stringify(NaN)` and `JSON.stringify(Infinity)` both write
        // `null`. Reaching here would mean a poisoned score escaped into the
        // report, which the incumbent rule above prevents; it is surfaced as an
        // error rather than silently written.
        return Err(format!("non-finite number in report: {value}"));
    }
    if value.fract() != 0.0 {
        return Err(format!("non-integral number in report: {value}"));
    }
    if value.abs() >= 9.007_199_254_740_992e15 {
        return Err(format!(
            "number outside the exactly-representable integer range: {value}"
        ));
    }
    let integral = value as i64;
    // `-0` stringifies as `0`.
    Ok(integral.to_string())
}
