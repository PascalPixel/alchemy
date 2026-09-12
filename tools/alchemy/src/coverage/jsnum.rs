pub fn number(value: f64) -> String {
    if value == 0.0 {
        "0".into()
    } else if value.fract() == 0.0 {
        format!("{value:.0}")
    } else {
        format!("{value}")
    }
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
/// DONE counts exact C plus retained assembly over executable bytes.
/// Every reader of the figure - the coverage summary, the README, the commit
/// gate and the dashboard - takes it from here.
pub fn done_bytes(proven_c: i64, proven_asm: i64) -> i64 {
    proven_c + proven_asm
}
/// DONE as a percentage to two decimals, half up.
pub fn done_percent(proven_c: i64, proven_asm: i64, executable: i64) -> f64 {
    round_half_up(done_bytes(proven_c, proven_asm), executable)
}
/// DONE as a whole percentage, half up: the commit-subject prefix.
pub fn done_percent_whole(proven_c: i64, proven_asm: i64, executable: i64) -> i64 {
    if executable == 0 {
        0
    } else {
        (done_bytes(proven_c, proven_asm) * 100 + executable / 2) / executable
    }
}
#[cfg(test)]
mod done_tests {
    use super::*;
    #[test]
    fn done_is_exact_c_plus_retained_assembly() {
        assert_eq!(done_bytes(400, 100), 500);
        assert_eq!(done_percent(400, 100, 1000), 50.0);
        assert_eq!(done_percent(631_946, 60, 1_354_302), 46.67);
        assert_eq!(done_percent_whole(631_946, 60, 1_354_302), 47);
        assert_eq!(done_percent_whole(465, 0, 1000), 47);
        assert_eq!(done_percent_whole(464, 0, 1000), 46);
        assert_eq!(done_percent(1, 1, 0), 0.0);
        assert_eq!(done_percent_whole(1, 1, 0), 0);
    }
}
