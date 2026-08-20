pub fn number(value: f64) -> String {
    if value == 0.0 {
        "0".into()
    } else if value.fract() == 0.0 {
        format!("{value:.0}")
    } else {
        format!("{value}")
    }
}
pub fn js_number_string(value: f64) -> String {
    number(value)
}
pub fn commas(value: i64) -> String {
    let text = value.to_string();
    let (sign, digits) = text
        .strip_prefix('-')
        .map_or(("", text.as_str()), |v| ("-", v));
    let mut out = String::with_capacity(text.len() + text.len() / 3);
    out.push_str(sign);
    for (i, c) in digits.chars().enumerate() {
        if i > 0 && (digits.len() - i) % 3 == 0 {
            out.push(',');
        }
        out.push(c);
    }
    out
}
pub fn round_half_up(n: i64, d: i64) -> f64 {
    if d == 0 {
        0.0
    } else {
        ((n as f64 * 10_000.0 / d as f64) + 0.5).floor() / 100.0
    }
}
pub fn hex8(value: i64) -> String {
    format!("{value:08x}")
}
