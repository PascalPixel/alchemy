pub fn js_parse_int_radix(text: &str, radix: u32) -> f64 {
    let mut text = text.trim();
    let negative = text.strip_prefix('-').is_some();
    if negative || text.starts_with('+') {
        text = &text[1..];
    }
    if radix == 16 {
        text = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")).unwrap_or(text);
    }
    let digits = text.bytes().take_while(|byte| digit(*byte).is_some_and(|value| u32::from(value) < radix)).count();
    if digits == 0 {
        return f64::NAN;
    }
    let value = text[..digits].bytes().fold(0.0, |value, byte| value * f64::from(radix) + f64::from(digit(byte).unwrap()));
    if negative {
        -value
    } else {
        value
    }
}
pub fn pad_end(text: &str, width: usize) -> String {
    format!("{text}{}", " ".repeat(width.saturating_sub(text.len())))
}
pub fn pad_start_zero(text: &str, width: usize) -> String {
    format!("{}{}", "0".repeat(width.saturating_sub(text.len())), text)
}
pub fn slice_utf16(text: &str, end: usize) -> String {
    if text.len() <= end {
        return text.into();
    }
    text.chars().take(end).collect()
}
fn digit(byte: u8) -> Option<u8> {
    char::from(byte).to_digit(16).map(|value| value as u8)
}
