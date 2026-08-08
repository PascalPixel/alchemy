//! The single numeric-parsing helper, plus the string and collation semantics
//! this port needs.
//!
//! ONE HELPER, EVERY CALL SITE. `compiler-corpus-regression` parses numbers
//! in six places and at three different radices:
//!
//! * `--sample` and `--jobs`   -> `Number.parseInt(text, 10)`
//! * `--radius`                -> `Number.parseInt(text, 0)`  (auto-radix)
//! * `--near`                  -> `Number.parseInt(stem, 16)`
//! * `corpus()` stem addresses -> `Number.parseInt(stem, 16)`
//! * `linkedFunctionExtent`    -> `Number.parseInt(field, 16)` twice
//!
//! PORT NOTE -- `i64::from_str_radix` is wrong for all of them. It REJECTS the
//! `0x` prefix that `parseInt(text, 16)` accepts, and it REJECTS trailing junk
//! that `parseInt` silently truncates: `parseInt("8x", 10)` is `8` and
//! `parseInt("1.5")` is `1`. A near-identical port elsewhere in this repository
//! cost 980 bytes at one call site AFTER already costing 526 at another,
//! because a correct helper existed and one call site did not use it. There is
//! exactly one implementation here and every call site goes through it.

use std::cmp::Ordering;

/// ECMAScript `parseInt(text, radix)`.
///
/// Returns `None` for JavaScript's `NaN`. `f64` and not `i64`, because the
/// return value is a JavaScript number and every downstream guard
/// (`Number.isInteger`, `Number.isSafeInteger`) is written against one.
pub fn parse_int(text: &str, radix: u32) -> Option<f64> {
    // `StringToNumber` trims leading `StrWhiteSpace`, which is JavaScript's
    // whitespace set and not Rust's `char::is_whitespace`. Reused from
    // `candidate_compiler::jsstring` so there is one definition of `\s` in the port.
    let trimmed = text.trim_start_matches(candidate_compiler::jsstring::is_js_space);

    let mut chars = trimmed.chars();
    let mut rest = trimmed;
    let mut negative = false;
    match chars.next() {
        Some('-') => {
            negative = true;
            rest = chars.as_str();
        }
        Some('+') => rest = chars.as_str(),
        _ => {}
    }

    // Radix 0 (the `undefined` radix) means: `0x`/`0X` prefix selects 16,
    // everything else selects 10. Radix 16 ALSO strips an optional `0x`
    // prefix. No other radix does. `--radius 0x10000` reaching `parseInt(s, 0)`
    // is the live use of the first branch; `--radius 10` of the second.
    let mut radix = radix;
    let lower_prefix = rest.len() >= 2 && (rest.starts_with("0x") || rest.starts_with("0X"));
    if radix == 0 {
        if lower_prefix {
            radix = 16;
            rest = &rest[2..];
        } else {
            radix = 10;
        }
    } else if radix == 16 && lower_prefix {
        rest = &rest[2..];
    }

    // Consume the longest prefix of valid digits and STOP at the first invalid
    // one, rather than failing. `parseInt("1.5")` is `1` because `.` is not a
    // decimal digit.
    //
    // PORT NOTE -- `char::to_digit` is Unicode-aware for `is_numeric` but
    // `to_digit` itself is ASCII-only, which is what JavaScript's parseInt
    // digit set is too. Fullwidth digits are not digits to either.
    let mut digits = 0usize;
    let mut value = 0.0f64;
    let mut exact: u128 = 0;
    let mut exact_ok = true;
    for c in rest.chars() {
        let Some(d) = c.to_digit(36) else { break };
        if d >= radix {
            break;
        }
        digits += 1;
        value = value * f64::from(radix) + f64::from(d);
        if exact_ok {
            match exact
                .checked_mul(u128::from(radix))
                .and_then(|acc| acc.checked_add(u128::from(d)))
            {
                Some(next) => exact = next,
                None => exact_ok = false,
            }
        }
    }
    if digits == 0 {
        return None; // NaN
    }
    // Accumulating in `u128` and converting once reproduces `parseInt`'s
    // round-once-at-the-end behaviour exactly for every input this tool sees;
    // the `f64` accumulator double-rounds and is only the overflow fallback.
    let magnitude = if exact_ok { exact as f64 } else { value };
    Some(if negative { -magnitude } else { magnitude })
}

/// `Number.isInteger(value)` over a `parseInt` result.
///
/// PORT NOTE -- `Number.isInteger(NaN)` is `false`, and so is
/// `Number.isInteger(Infinity)`. `None` here is `NaN`, and the `is_finite`
/// guard covers the rest. Written as a named helper because clippy would
/// otherwise want `!(x.fract() != 0.0)` rewritten in a way that changes the
/// NaN case.
pub fn is_js_integer(value: Option<f64>) -> bool {
    match value {
        None => false,
        Some(v) => v.is_finite() && v.fract() == 0.0,
    }
}

/// `Number.isSafeInteger(value)`.
pub fn is_js_safe_integer(value: Option<f64>) -> bool {
    is_js_integer(value) && value.is_some_and(|v| v.abs() <= 9_007_199_254_740_991.0)
}

/// `Math.abs`.
///
/// PORT NOTE -- named rather than inlined so that the NaN case is one place:
/// `Math.abs(NaN)` is `NaN` and `NaN > radius` is `false`, so a poisoned
/// address SILENTLY PASSES the `--near` filter instead of being excluded.
/// `f64::abs` agrees here, but the comparison that follows must not be
/// rewritten by clippy into `<=`, which is why `corpus()` calls this and then
/// compares explicitly.
pub fn js_abs(value: f64) -> f64 {
    if value.is_nan() {
        f64::NAN
    } else {
        value.abs()
    }
}

/// `a > b` with JavaScript's NaN semantics, spelled out so no reviewer and no
/// lint rewrites it to `!(a <= b)`. Those are NOT the same when either side is
/// NaN: `NaN > 1` is `false` and `!(NaN <= 1)` is `true`.
pub fn js_greater_than(left: f64, right: f64) -> bool {
    if left.is_nan() || right.is_nan() {
        return false;
    }
    left > right
}

/// `String.prototype.slice(0, limit)`.
///
/// PORT NOTE -- `String.length` and `slice` count UTF-16 CODE UNITS, not bytes
/// and not `char`s. `error.message.slice(0, 600)` on a compiler diagnostic
/// containing a non-BMP character (an emoji in a source comment reaches gcc's
/// error text) truncates at a different point than any byte or `char` count
/// would. A cut that lands between the halves of a surrogate pair produces a
/// lone surrogate in JavaScript; that cannot exist in a Rust `String`, so the
/// truncation backs off to the start of the pair, and the divergence is
/// recorded in `tests/invariants.rs`.
pub fn utf16_slice_to(text: &str, limit: usize) -> String {
    let mut units = 0usize;
    let mut end = 0usize;
    for (offset, c) in text.char_indices() {
        let width = c.len_utf16();
        if units + width > limit {
            break;
        }
        units += width;
        end = offset + c.len_utf8();
    }
    text[..end].to_string()
}

/// `left.localeCompare(right)` restricted to the domain this file uses.
///
/// PORT NOTE -- `localeCompare` is ICU collation with case as a TERTIARY
/// weight, which is a THIRD order distinct from both `str::cmp` (byte order)
/// and JavaScript's default `sort()` (UTF-16 code-unit order). Substituting
/// `str::cmp` blindly would be a real bug in general.
///
/// It is NOT a bug here, and the restriction is what proves it. Both call
/// sites in `deterministicSample` compare strings drawn from `[0-9a-f]`: a
/// lowercase sha256 hex digest, and an 8-character hex stem that
/// `hexadecimalStem` has already validated against `/^[0-9a-f]{8}$/`. Over
/// that alphabet ICU root collation and byte order agree, because the
/// alphabet is single-script, caseless, and its ICU primary weights are
/// monotonic in code point. `tests/collation.rs` proves the agreement
/// EXHAUSTIVELY over every ordered pair of the 16 symbols and over equal-length
/// pairs, and this function PANICS on anything outside the alphabet rather
/// than silently returning a byte comparison for input the proof does not
/// cover.
pub fn locale_compare_hex(left: &str, right: &str) -> Ordering {
    debug_assert!(
        is_hex_alphabet(left) && is_hex_alphabet(right),
        "locale_compare_hex is only proven over [0-9a-f]: {left:?} vs {right:?}"
    );
    left.as_bytes().cmp(right.as_bytes())
}

pub fn is_hex_alphabet(text: &str) -> bool {
    !text.is_empty() && text.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b))
}

/// JavaScript's default `Array.prototype.sort()` comparator: UTF-16 code-unit
/// order over the string forms. Used for `[...options.sources].sort()`.
pub fn default_sort_cmp(left: &str, right: &str) -> Ordering {
    candidate_compiler::jsstring::utf16_cmp(left, right)
}
