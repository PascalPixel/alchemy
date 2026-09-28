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
/// A share as a percentage to two decimals, rounded down, so 100% is shown
/// only when every byte is done.
pub fn floor_percent(n: i64, d: i64) -> f64 {
    if d == 0 {
        0.0
    } else {
        (n as i128 * 10_000 / d as i128) as f64 / 100.0
    }
}
#[cfg(test)]
mod done_tests {
    use super::*;
    #[test]
    fn shares_round_down_to_hundredths() {
        assert_eq!(floor_percent(500, 1000), 50.0);
        assert_eq!(floor_percent(632_006, 1_354_302), 46.66);
        assert_eq!(floor_percent(249_266, 1_622_410), 15.36);
        assert_eq!(floor_percent(9_999, 10_000), 99.99);
        assert_eq!(floor_percent(2, 0), 0.0);
        assert_eq!(commas(-1_622_410), "-1,622,410");
    }
}
