//! JavaScript's sole numeric type is `f64`. Undefined values poison scores with
//! NaN; max/abs propagate it, and comparisons then retain the incumbent.

/// JS hexadecimal plus `padStart(8, "0")`; padding never truncates wide values.
pub fn hex8(value: f64) -> String {
    debug_assert!(value >= 0.0 && value.fract() == 0.0);
    format!("{:08x}", value as u64)
}

/// ASCII, case-insensitive JS `/^[0-9a-f]+$/`, then base-16 `parseInt`.
pub fn parse_hex(value: &str) -> Result<f64, String> {
    let ok = !value.is_empty() && value.bytes().all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b) || (b'A'..=b'F').contains(&b));
    if !ok {
        return Err(format!("invalid hexadecimal value: {value}"));
    }
    // u128 preserves parseInt's single f64 rounding through 32 hex digits; the
    // fallback is outside corpus input sizes.
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

/// JS `Math.max`, which propagates NaN unlike `f64::max`.
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

/// JS `Math.abs`; `f64::abs` already agrees on NaN.
pub fn math_abs(value: f64) -> f64 {
    value.abs()
}

/// Raw JS tuple difference: NaN makes `< 0` false and retains the incumbent.
pub fn compare_tuple(left: [f64; 3], right: [f64; 3]) -> f64 {
    for index in 0..3 {
        // Rust f64 `!=` matches JS NaN inequality.
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

/// JS number formatting restricted to exact finite integers emitted by reports;
/// rejecting other values avoids Rust/JS decimal and exponent differences.
pub fn to_js_number_string(value: f64) -> Result<String, String> {
    if !value.is_finite() {
        // Do not silently turn escaped poisoned scores into JSON null.
        return Err(format!("non-finite number in report: {value}"));
    }
    if value.fract() != 0.0 {
        return Err(format!("non-integral number in report: {value}"));
    }
    if value.abs() >= 9.007_199_254_740_992e15 {
        return Err(format!("number outside the exactly-representable integer range: {value}"));
    }
    let integral = value as i64;
    // `-0` stringifies as `0`.
    Ok(integral.to_string())
}
