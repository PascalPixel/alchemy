pub mod cli;

use serde_json::Value;

pub const ROM_BASE: f64 = 0x0800_0000 as f64;

pub type Res<T> = Result<T, String>;

fn is_js_whitespace(c: char) -> bool {
    matches!(c, '\t' | '\n' | '\u{0b}' | '\u{0c}' | '\r' | ' ' | '\u{a0}' | '\u{1680}' | '\u{2000}'..='\u{200a}' | '\u{2028}' | '\u{2029}' | '\u{202f}' | '\u{205f}' | '\u{3000}' | '\u{feff}')
}

pub fn js_number(text: &str) -> f64 {
    let trimmed = text.trim_matches(is_js_whitespace);
    if trimmed.is_empty() {
        return 0.0;
    }
    if trimmed.len() > 2 {
        let radix = match &trimmed[..2] {
            "0x" | "0X" => 16u32,
            "0o" | "0O" => 8,
            "0b" | "0B" => 2,
            _ => 0,
        };
        if radix != 0 {
            let mut value = 0f64;
            for c in trimmed[2..].chars() {
                match c.to_digit(radix) {
                    Some(digit) => value = value * f64::from(radix) + f64::from(digit),
                    None => return f64::NAN,
                }
            }
            return value;
        }
    }
    match trimmed {
        "Infinity" | "+Infinity" => return f64::INFINITY,
        "-Infinity" => return f64::NEG_INFINITY,
        _ => {}
    }
    let lowered = trimmed.to_ascii_lowercase();
    if lowered.contains("inf") || lowered.contains("nan") {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

pub fn js_number_string(value: f64) -> String {
    if value.is_finite() && value.fract() == 0.0 && value.abs() < 1e21 {
        let integral = value.abs();
        let text = format!("{}", integral as u64);
        return if value.is_sign_negative() && integral != 0.0 { format!("-{text}") } else { text };
    }
    format!("{value}")
}

fn integer(value: Option<&Value>, label: &str) -> Res<f64> {
    let parsed = match value {
        Some(Value::String(text)) => js_number(text),
        Some(Value::Number(number)) => number.as_f64().unwrap_or(f64::NAN),
        _ => f64::NAN,
    };
    if !is_safe_integer(parsed) {
        return Err(format!("{label} must be an integer"));
    }
    Ok(parsed)
}

pub fn integer_text(text: Option<&str>, label: &str) -> Res<f64> {
    integer(text.map(|t| Value::String(t.to_string())).as_ref(), label)
}

fn address(value: Option<&Value>, label: &str) -> Res<f64> {
    let parsed = integer(value, label)?;
    address_check(parsed, label)
}

fn address_check(parsed: f64, label: &str) -> Res<f64> {
    if !(ROM_BASE..=4_294_967_295.0).contains(&parsed) {
        return Err(format!("{label} is outside the ROM address space"));
    }
    Ok(parsed)
}

pub fn hex(value: f64) -> String {
    let digits = format!("{:x}", value as u64);
    if digits.len() >= 8 {
        format!("0x{digits}")
    } else {
        format!("0x{}{}", "0".repeat(8 - digits.len()), digits)
    }
}

pub fn id(value: f64, slots: f64) -> String {
    let width = std::cmp::max(3, format!("{:x}", (slots - 1.0) as u64).len());
    let digits = format!("{:x}", value as u64);
    if digits.len() >= width {
        format!("0x{digits}")
    } else {
        format!("0x{}{}", "0".repeat(width - digits.len()), digits)
    }
}

fn is_canonical_hex_id(value: &str) -> bool {
    let Some(rest) = value.strip_prefix("0x") else {
        return false;
    };
    !rest.is_empty() && rest.chars().all(|c| matches!(c, '0'..='9' | 'a'..='f'))
}

fn parse_id(value: &str, slots: f64, label: &str) -> Res<f64> {
    if !is_canonical_hex_id(value) {
        return Err(format!("{label} is not a canonical hexadecimal ID"));
    }
    let mut parsed = 0f64;
    for c in value[2..].chars() {
        parsed = parsed * 16.0 + f64::from(c.to_digit(16).expect("validated"));
    }
    if parsed < 0.0 || parsed >= slots || id(parsed, slots) != value {
        return Err(format!("{label} is outside the directory"));
    }
    Ok(parsed)
}

pub fn document(value: &Value) -> Res<(f64, f64)> {
    let Some(source) = value.as_object() else {
        return Err("resource directory source must be an object".to_string());
    };
    let format_ok = matches!(source.get("format"), Some(Value::Number(n)) if n.as_f64() == Some(1.0));
    let kind_ok = matches!(source.get("kind"), Some(Value::String(k)) if k == "golden-sun-resource-directory");
    if !format_ok || !kind_ok {
        return Err("unsupported resource directory source".to_string());
    }
    let start = address(source.get("address"), "resource directory address")?;
    if Some(&Value::String(hex(start))) != source.get("address") {
        return Err("resource directory address is not canonical hexadecimal".to_string());
    }
    let count = integer(source.get("slot_count"), "resource directory slot count")?;
    if !(2.0..=65536.0).contains(&count) {
        return Err("resource directory slot count is outside the supported range".to_string());
    }
    match source.get("slots") {
        Some(Value::Array(slots)) if slots.len() as f64 == count => {}
        _ => return Err("resource directory slots do not match slot_count".to_string()),
    }
    Ok((start, count))
}

pub fn build_resource_directory(value: &Value) -> Res<Vec<u8>> {
    let (start, count) = document(value)?;
    let slots = value["slots"].as_array().expect("validated");
    let total = count as usize;
    let mut result = vec![0u8; total * 4];
    let mut resolved: Vec<f64> = Vec::with_capacity(total);
    for index in 0..total {
        let slot = slots[index].as_str();
        let value = match slot {
            Some("rom-base") => ROM_BASE,
            Some("directory-self") => start,
            Some("reserved-null") => 0.0,
            Some(text) if text.starts_with("resource:") => {
                let literal = &text["resource:".len()..];
                let target = address(Some(&Value::String(literal.to_string())), &format!("resource pointer {}", id(index as f64, count)))?;
                if hex(target) != literal {
                    return Err(format!("resource pointer {} is not canonical hexadecimal", id(index as f64, count)));
                }
                target
            }
            Some(text) if text.starts_with("alias:") => {
                let literal = &text["alias:".len()..];
                let target = parse_id(literal, count, &format!("alias {}", id(index as f64, count)))?;
                if target >= index as f64 {
                    return Err(format!("alias {} must reference an earlier slot", id(index as f64, count)));
                }
                let value = resolved[target as usize];
                if value == 0.0 {
                    return Err(format!("alias {} references a reserved slot", id(index as f64, count)));
                }
                value
            }
            _ => return Err(format!("invalid resource directory slot {}", id(index as f64, count))),
        };
        resolved.push(value);
        result[index * 4..index * 4 + 4].copy_from_slice(&(value as u32).to_le_bytes());
    }
    Ok(result)
}
