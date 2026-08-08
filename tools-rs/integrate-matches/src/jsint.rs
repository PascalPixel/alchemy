//! THE ONE NUMERIC PARSE.
//!
//! `integrate_matches.ts` calls `Number.parseInt(value, 16)` in three places:
//! on the stem in `linkedBytes`, and on `fields[0]` and `fields[1]` in
//! `linkedFunctionExtent`. Two of those three read arbitrary
//! `arm-none-eabi-nm` output, so the parse cannot assume clean input.
//!
//! WHY THIS FILE EXISTS AT ALL. `i64::from_str_radix` REJECTS a `0x` prefix and
//! REJECTS trailing junk; `parseInt` accepts both. In a sibling crate that
//! difference cost 980 bytes at one call site AFTER already costing 526 at
//! another, because a correct helper existed and one call site did not use it.
//! There is therefore exactly one parse function in this crate and every call
//! site routes through it. `match-m2c`'s `parse_hex` is deliberately NOT used
//! here: it is `/^[0-9a-f]+$/i`-guarded and errors on anything else, which is
//! the RIGHT contract for a stem and the WRONG one for an `nm` field, where the
//! TypeScript wants a silent `NaN` that the later filters swallow.

/// `Number.parseInt(value, 16)`, exactly.
///
/// Returns `f64` because that is the type JavaScript produces, `NaN` included.
/// Callers must test with [`is_safe_integer`] or with a comparison, never by
/// unwrapping.
///
/// PORT NOTE -- the four behaviours that separate this from
/// `i64::from_str_radix`:
///   * leading ECMAScript whitespace is skipped (`StringToNumber`'s
///     `StrWhiteSpace`), including U+00A0 and U+FEFF but NOT U+0085;
///   * an optional `+`/`-` sign is consumed;
///   * a `0x`/`0X` prefix is consumed and IGNORED when the radix is 16;
///   * the longest leading run of hex digits is used and the REST IS DISCARDED,
///     so `parseInt("8x", 16)` is 8 and `parseInt("1.5", 16)` is 1. Only an
///     empty run yields NaN.
///
/// Hex digits here are case-INSENSITIVE, unlike the `[0-9a-f]` character
/// classes in [`crate::jsregex`], which are not. Both are correct; they are
/// different mechanisms in the same file and mixing them up is the trap.
pub fn parse_int_hex(value: &str) -> f64 {
    let mut chars = value.chars().peekable();
    while let Some(c) = chars.peek() {
        if match_m2c::jsstring::is_js_space(*c) {
            chars.next();
        } else {
            break;
        }
    }
    let mut negative = false;
    match chars.peek() {
        Some('-') => {
            negative = true;
            chars.next();
        }
        Some('+') => {
            chars.next();
        }
        _ => {}
    }
    // The `0x` prefix. `parseInt` peeks two characters and only strips them
    // when both are present; a bare trailing `0` is a digit, not a prefix.
    let remainder: String = chars.collect();
    let digits_source = {
        let bytes = remainder.as_bytes();
        if bytes.len() >= 2 && bytes[0] == b'0' && (bytes[1] == b'x' || bytes[1] == b'X') {
            &remainder[2..]
        } else {
            &remainder[..]
        }
    };
    let mut magnitude = 0.0f64;
    let mut seen = 0usize;
    // Accumulate exactly for the widths that occur, then fall back. 2^53 is
    // reached at 14 hex digits; beyond that `parseInt` computes the exact
    // mathematical value and rounds ONCE, which `u128` reproduces to 32 digits.
    let mut exact: u128 = 0;
    for b in digits_source.bytes() {
        let Some(digit) = hex_digit(b) else { break };
        if seen < 32 {
            exact = exact * 16 + u128::from(digit);
        } else {
            if seen == 32 {
                magnitude = exact as f64;
            }
            magnitude = magnitude * 16.0 + f64::from(digit);
        }
        seen += 1;
    }
    if seen == 0 {
        return f64::NAN;
    }
    let unsigned = if seen <= 32 { exact as f64 } else { magnitude };
    if negative {
        -unsigned
    } else {
        unsigned
    }
}

fn hex_digit(b: u8) -> Option<u8> {
    match b {
        b'0'..=b'9' => Some(b - b'0'),
        b'a'..=b'f' => Some(b - b'a' + 10),
        b'A'..=b'F' => Some(b - b'A' + 10),
        _ => None,
    }
}

/// `Number.isSafeInteger(value)`.
///
/// PORT NOTE -- NaN and both infinities are false, which is the only reason
/// this crate can carry a `NaN` out of [`parse_int_hex`] safely.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

/// `Math.max(...values)`.
///
/// PORT NOTE -- `f64::max` returns the NON-NaN operand, so a fold over
/// `f64::max` would silently DROP a poisoned element. `Math.max` PROPAGATES
/// NaN. The empty spread is `-Infinity`, which is also reproduced; the one
/// caller proves the list is non-empty first, but the helper must not quietly
/// disagree with the language if that proof is ever weakened.
pub fn math_max_all(values: &[f64]) -> f64 {
    let mut best = f64::NEG_INFINITY;
    for value in values {
        if value.is_nan() || best.is_nan() {
            return f64::NAN;
        }
        if *value > best {
            best = *value;
        }
    }
    best
}

/// `value.toString(16).padStart(8, "0")`.
///
/// PORT NOTE -- `padStart` pads and never truncates, so a nine-digit value
/// stays nine digits. `{:08x}` agrees. The caller has already proved the value
/// is a `08xxxxxx` address, so neither branch is reachable there, but the
/// helper is the general one.
pub fn hexadecimal(value: f64) -> String {
    format!("{:08x}", value as u64)
}
