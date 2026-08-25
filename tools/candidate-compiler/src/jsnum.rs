//! Numeric compatibility at the candidate compiler's report boundary.

/// JS hexadecimal plus `padStart(8, "0")`; padding never truncates wide values.
pub fn hex8(value: u64) -> String {
    format!("{value:08x}")
}

/// ASCII, case-insensitive hexadecimal used by addresses and binutils fields.
pub fn parse_hex(value: &str) -> Result<u64, String> {
    if value.is_empty() || !value.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!("invalid hexadecimal value: {value}"));
    }
    u64::from_str_radix(value, 16).map_err(|error| error.to_string())
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
        return Err(format!(
            "number outside the exactly-representable integer range: {value}"
        ));
    }
    let integral = value as i64;
    // `-0` stringifies as `0`.
    Ok(integral.to_string())
}
