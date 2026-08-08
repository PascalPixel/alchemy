// Port of tools/make/resource_directory.ts (the resource directory builder,
// exporter and verifier for the 1000-slot pointer table at 0x08320000).
//
// PORT NOTE -- inlined dependencies
//   * `canonicalJson` from tools/lib/canonical_json.ts is inlined below as
//     `canonical_json` (objects one key per line, arrays of primitives inline).
//     `isCanonicalJsonText` from that module is NOT used by this tool and is
//     not ported.
//   * Nothing else is imported by the TypeScript original.
//
// PORT NOTE -- deliberate differences
//   * Errors: bun prints an uncaught `Error` as a source excerpt plus a stack
//     trace and exits 1. The Rust binary prints `error: <message>` on stderr
//     and exits 1. The message text is identical; the framing is not.
//   * File-open failures carry the OS message rather than bun's `ENOENT: ...`
//     text. Both are stderr-only failures with exit code 1.
//   * `build_resource_directory` resolves slots iteratively instead of via the
//     recursive `resolve_slot` closure. This is observably identical: an alias
//     must reference a strictly earlier slot (checked before recursing), and
//     the driving loop resolves indices in ascending order, so every recursive
//     call in the TypeScript hits the memo on its first line. The `resolving`
//     cycle guard is therefore unreachable there and is omitted here; the
//     `alias ... must reference an earlier slot` error is what actually fires.
//   * JS `JSON.stringify` of a non-integral or >=1e21 number is emulated with
//     Rust's shortest round-trip float formatting. No such number occurs in a
//     resource directory document (every field is an integer or a string).
//
// PORT NOTE -- JS/Rust traps pinned by tests below
//   * `Number(string)` accepts `0x`/`0o`/`0b` prefixes, JS whitespace padding
//     and the empty string (-> 0). `js_number` reproduces that; a plain Rust
//     `str::parse::<f64>` would reject `"0x08320000"` and accept `"inf"`/`"nan"`.
//   * `Number.isSafeInteger` accepts integral floats, so a JSON `slot_count`
//     of `8.0` is a valid slot count. serde_json parses that as an f64.
//   * `readFileSync(..., "utf8")` / `Bun.file().text()` replace invalid UTF-8
//     with U+FFFD; `String::from_utf8_lossy` is used for JSON input.
//   * The `/^0x[0-9a-f]+$/` test is hand-rolled. JS `$` (no `m` flag) does not
//     match before a trailing newline, unlike a naive line-wise check.
//   * The first-occurrence `Map` in the exporter is keyed by pointer; only
//     lookups matter, so a hash map is order-equivalent. Output order is
//     carried by the slots vector, which is append-only.

use std::collections::HashMap;

use serde_json::{Map, Value};

pub const ROM_BASE: f64 = 0x0800_0000 as f64;
pub const DIRECTORY_ADDRESS: f64 = 0x0832_0000 as f64;
pub const DIRECTORY_SLOTS: f64 = 1000.0;

pub type Res<T> = Result<T, String>;

// ---------------------------------------------------------------------------
// JS number semantics
// ---------------------------------------------------------------------------

fn is_js_whitespace(c: char) -> bool {
    matches!(
        c,
        '\t' | '\n'
            | '\u{0b}'
            | '\u{0c}'
            | '\r'
            | ' '
            | '\u{a0}'
            | '\u{1680}'
            | '\u{2000}'..='\u{200a}'
            | '\u{2028}'
            | '\u{2029}'
            | '\u{202f}'
            | '\u{205f}'
            | '\u{3000}'
            | '\u{feff}'
    )
}

/// `Number(text)` as ECMAScript defines it, for the cases this tool can reach.
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
    // Rust accepts "inf", "infinity" and "nan" in any case; JS does not.
    let lowered = trimmed.to_ascii_lowercase();
    if lowered.contains("inf") || lowered.contains("nan") {
        return f64::NAN;
    }
    trimmed.parse::<f64>().unwrap_or(f64::NAN)
}

/// `Number.isSafeInteger` -- integral floats included.
pub fn is_safe_integer(value: f64) -> bool {
    value.is_finite() && value.fract() == 0.0 && value.abs() <= 9_007_199_254_740_991.0
}

/// `String(value)` for a JS number, restricted to the finite values this tool
/// can print (all of them safe integers).
pub fn js_number_string(value: f64) -> String {
    if value.is_finite() && value.fract() == 0.0 && value.abs() < 1e21 {
        let integral = value.abs();
        let text = format!("{}", integral as u64);
        return if value.is_sign_negative() && integral != 0.0 {
            format!("-{text}")
        } else {
            text
        };
    }
    format!("{value}")
}

// ---------------------------------------------------------------------------
// canonical_json (inlined from tools/lib/canonical_json.ts)
// ---------------------------------------------------------------------------

fn is_primitive(value: &Value) -> bool {
    matches!(
        value,
        Value::Null | Value::Bool(_) | Value::Number(_) | Value::String(_)
    )
}

fn stringify_scalar(value: &Value) -> String {
    match value {
        Value::Null => "null".to_string(),
        Value::Bool(true) => "true".to_string(),
        Value::Bool(false) => "false".to_string(),
        Value::Number(number) => match number.as_f64() {
            Some(raw) => js_number_string(raw),
            None => number.to_string(),
        },
        Value::String(text) => stringify_string(text),
        _ => unreachable!("stringify_scalar on a container"),
    }
}

fn stringify_string(text: &str) -> String {
    let mut out = String::with_capacity(text.len() + 2);
    out.push('"');
    for c in text.chars() {
        match c {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

fn reflow(value: &Value, indent: &str) -> String {
    if is_primitive(value) {
        return stringify_scalar(value);
    }
    if let Value::Array(items) = value {
        if items.is_empty() {
            return "[]".to_string();
        }
        if items.iter().all(is_primitive) {
            let inner: Vec<String> = items.iter().map(stringify_scalar).collect();
            return format!("[{}]", inner.join(", "));
        }
        let deeper = format!("{indent}  ");
        let inner: Vec<String> = items
            .iter()
            .map(|item| format!("{deeper}{}", reflow(item, &deeper)))
            .collect();
        return format!("[\n{}\n{indent}]", inner.join(",\n"));
    }
    let record = value.as_object().expect("object");
    if record.is_empty() {
        return "{}".to_string();
    }
    let deeper = format!("{indent}  ");
    let inner: Vec<String> = record
        .iter()
        .map(|(key, item)| {
            format!(
                "{deeper}{}: {}",
                stringify_string(key),
                reflow(item, &deeper)
            )
        })
        .collect();
    format!("{{\n{}\n{indent}}}", inner.join(",\n"))
}

pub fn canonical_json(value: &Value) -> String {
    reflow(value, "")
}

// ---------------------------------------------------------------------------
// helpers
// ---------------------------------------------------------------------------

/// `integer(value, label)` -- strings go through `Number`, everything else is
/// handed straight to `Number.isSafeInteger`.
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

/// `integer(...)` applied to a command-line option, which is a string or
/// `undefined`.
pub fn integer_text(text: Option<&str>, label: &str) -> Res<f64> {
    integer(text.map(|t| Value::String(t.to_string())).as_ref(), label)
}

fn integer_number(value: f64, label: &str) -> Res<f64> {
    if !is_safe_integer(value) {
        return Err(format!("{label} must be an integer"));
    }
    Ok(value)
}

fn address(value: Option<&Value>, label: &str) -> Res<f64> {
    let parsed = integer(value, label)?;
    address_check(parsed, label)
}

fn address_number(value: f64, label: &str) -> Res<f64> {
    let parsed = integer_number(value, label)?;
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

/// `/^0x[0-9a-f]+$/.test(value)` -- no `m` flag, so `$` is the true end of the
/// string and a trailing newline fails.
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

// ---------------------------------------------------------------------------
// document validation
// ---------------------------------------------------------------------------

/// `document(value)` -- returns the validated `(address, slot_count)` pair.
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

// ---------------------------------------------------------------------------
// build / export
// ---------------------------------------------------------------------------

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
                let target = address(
                    Some(&Value::String(literal.to_string())),
                    &format!("resource pointer {}", id(index as f64, count)),
                )?;
                if hex(target) != literal {
                    return Err(format!(
                        "resource pointer {} is not canonical hexadecimal",
                        id(index as f64, count)
                    ));
                }
                target
            }
            Some(text) if text.starts_with("alias:") => {
                let literal = &text["alias:".len()..];
                let target = parse_id(
                    literal,
                    count,
                    &format!("alias {}", id(index as f64, count)),
                )?;
                if target >= index as f64 {
                    return Err(format!(
                        "alias {} must reference an earlier slot",
                        id(index as f64, count)
                    ));
                }
                let value = resolved[target as usize];
                if value == 0.0 {
                    return Err(format!(
                        "alias {} references a reserved slot",
                        id(index as f64, count)
                    ));
                }
                value
            }
            _ => {
                return Err(format!(
                    "invalid resource directory slot {}",
                    id(index as f64, count)
                ))
            }
        };
        resolved.push(value);
        result[index * 4..index * 4 + 4].copy_from_slice(&(value as u32).to_le_bytes());
    }
    Ok(result)
}

pub fn export_resource_directory(
    rom: &[u8],
    directory_address: f64,
    slot_count: f64,
) -> Res<Value> {
    let start = address_number(directory_address, "resource directory address")? - ROM_BASE;
    if !is_safe_integer(slot_count) || !(2.0..=65536.0).contains(&slot_count) {
        return Err("resource directory slot count is outside the supported range".to_string());
    }
    if start < 0.0 || start + slot_count * 4.0 > rom.len() as f64 {
        return Err("resource directory lies outside the ROM".to_string());
    }
    let start = start as usize;
    let total = slot_count as usize;
    let mut first: HashMap<u32, usize> = HashMap::new();
    let mut slots: Vec<Value> = Vec::with_capacity(total);
    for index in 0..total {
        let at = start + index * 4;
        let pointer = u32::from_le_bytes([rom[at], rom[at + 1], rom[at + 2], rom[at + 3]]);
        let seen = first.contains_key(&pointer);
        let slot = if pointer == 0 {
            "reserved-null".to_string()
        } else if f64::from(pointer) == ROM_BASE && !seen {
            "rom-base".to_string()
        } else if f64::from(pointer) == directory_address && !seen {
            "directory-self".to_string()
        } else if seen {
            format!("alias:{}", id(first[&pointer] as f64, slot_count))
        } else {
            if f64::from(pointer) < ROM_BASE || f64::from(pointer) >= ROM_BASE + rom.len() as f64 {
                return Err(format!(
                    "slot {} is not a ROM pointer",
                    id(index as f64, slot_count)
                ));
            }
            format!("resource:{}", hex(f64::from(pointer)))
        };
        slots.push(Value::String(slot));
        if pointer != 0 && !seen {
            first.insert(pointer, index);
        }
    }
    let mut result = Map::new();
    result.insert("format".to_string(), Value::from(1u64));
    result.insert(
        "kind".to_string(),
        Value::String("golden-sun-resource-directory".to_string()),
    );
    result.insert("address".to_string(), Value::String(hex(directory_address)));
    result.insert("slot_count".to_string(), Value::from(slot_count));
    result.insert("slots".to_string(), Value::Array(slots));
    let result = Value::Object(result);
    let rebuilt = build_resource_directory(&result)?;
    if rebuilt != rom[start..start + total * 4] {
        return Err("resource directory export does not round-trip".to_string());
    }
    Ok(result)
}

// ---------------------------------------------------------------------------
// self test (port of `self_test` in the TypeScript)
// ---------------------------------------------------------------------------

pub fn sample_document() -> Value {
    let mut source = Map::new();
    source.insert("format".to_string(), Value::from(1u64));
    source.insert(
        "kind".to_string(),
        Value::String("golden-sun-resource-directory".to_string()),
    );
    source.insert(
        "address".to_string(),
        Value::String("0x08000100".to_string()),
    );
    source.insert("slot_count".to_string(), Value::from(8u64));
    source.insert(
        "slots".to_string(),
        Value::Array(
            [
                "rom-base",
                "directory-self",
                "resource:0x08000200",
                "alias:0x002",
                "reserved-null",
                "resource:0x08000300",
                "alias:0x005",
                "reserved-null",
            ]
            .iter()
            .map(|s| Value::String((*s).to_string()))
            .collect(),
        ),
    );
    Value::Object(source)
}

pub fn self_test() -> Res<()> {
    let source = sample_document();
    let built = build_resource_directory(&source)?;
    let expected: [u32; 8] = [
        ROM_BASE as u32,
        0x0800_0100,
        0x0800_0200,
        0x0800_0200,
        0,
        0x0800_0300,
        0x0800_0300,
        0,
    ];
    let mismatch = expected.iter().enumerate().any(|(index, value)| {
        u32::from_le_bytes([
            built[index * 4],
            built[index * 4 + 1],
            built[index * 4 + 2],
            built[index * 4 + 3],
        ]) != *value
    });
    if built.len() != 32 || mismatch {
        return Err("resource directory builder self-test failed".to_string());
    }
    let mut rom = vec![0u8; 0x400];
    rom[0x100..0x100 + built.len()].copy_from_slice(&built);
    let exported = export_resource_directory(&rom, 0x0800_0100 as f64, 8.0)?;
    if build_resource_directory(&exported)? != built
        || exported["slots"][3] != Value::String("alias:0x002".to_string())
        || exported["slots"][6] != Value::String("alias:0x005".to_string())
    {
        return Err("resource directory exporter self-test failed".to_string());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn parse(text: &str) -> Value {
        serde_json::from_str(text).expect("valid json")
    }

    #[test]
    fn ported_self_test_passes() {
        self_test().expect("self test");
    }

    // --- JS number traps ---------------------------------------------------

    #[test]
    fn js_number_reads_radix_prefixes_and_padding() {
        assert_eq!(js_number("0x08320000"), 137_494_528.0);
        assert_eq!(js_number("0X10"), 16.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("  1000\n"), 1000.0);
        assert_eq!(js_number("\u{feff}12"), 12.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("   "), 0.0);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        // Rust's f64 parser accepts these spellings; JS does not.
        assert!(js_number("inf").is_nan());
        assert!(js_number("Infinity ").is_infinite()); // padding trimmed first
        assert!(js_number("infinity").is_nan());
        assert!(js_number("nan").is_nan());
        assert!(js_number("NaN").is_nan());
        assert!(js_number("0x").is_nan());
        assert!(js_number("12abc").is_nan());
    }

    #[test]
    fn safe_integer_accepts_integral_floats() {
        assert!(is_safe_integer(8.0));
        assert!(is_safe_integer(-0.0));
        assert!(!is_safe_integer(8.5));
        assert!(!is_safe_integer(f64::INFINITY));
        assert!(!is_safe_integer(f64::NAN));
        assert!(is_safe_integer(9_007_199_254_740_991.0));
        assert!(!is_safe_integer(9_007_199_254_740_992.0));
    }

    #[test]
    fn integral_float_slot_count_is_accepted() {
        // JSON `8.0` reaches serde_json as an f64; Number.isSafeInteger(8.0) is
        // true in JS, so the document must validate exactly as `8` does.
        let text = r#"{"format":1.0,"kind":"golden-sun-resource-directory",
            "address":"0x08000100","slot_count":8.0,
            "slots":["rom-base","directory-self","reserved-null","reserved-null",
                     "reserved-null","reserved-null","reserved-null","reserved-null"]}"#;
        let value = parse(text);
        let (start, count) = document(&value).expect("valid");
        assert_eq!(start, 0x0800_0100 as f64);
        assert_eq!(count, 8.0);
        assert_eq!(build_resource_directory(&value).expect("built").len(), 32);
    }

    #[test]
    fn non_string_non_number_fields_are_not_coerced() {
        // JS only routes strings through Number(); `true` fails isSafeInteger.
        assert_eq!(
            integer(Some(&Value::Bool(true)), "x").unwrap_err(),
            "x must be an integer"
        );
        assert_eq!(integer(None, "x").unwrap_err(), "x must be an integer");
        assert_eq!(
            integer(Some(&Value::Null), "x").unwrap_err(),
            "x must be an integer"
        );
    }

    // --- hex / id / parse_id ----------------------------------------------

    #[test]
    fn hex_and_id_widths_match_the_typescript() {
        assert_eq!(hex(ROM_BASE), "0x08000000");
        assert_eq!(hex(0x0832_0000 as f64), "0x08320000");
        assert_eq!(id(2.0, 8.0), "0x002");
        assert_eq!(id(2.0, 1000.0), "0x002");
        assert_eq!(id(0x3e7 as f64, 1000.0), "0x3e7");
        // 0x10000 slots -> (slots-1).toString(16) is "ffff", width 4.
        assert_eq!(id(1.0, 65536.0), "0x0001");
    }

    #[test]
    fn hex_uses_the_full_eight_digit_pad() {
        // hex() is only ever called on ROM addresses, but the pad must not
        // truncate a value that is already wider than eight digits.
        assert_eq!(hex(4_294_967_295.0), "0xffffffff");
    }

    #[test]
    fn canonical_hex_id_rejects_trailing_newline_and_uppercase() {
        // JS `$` without the `m` flag does not match before a newline.
        assert!(is_canonical_hex_id("0x002"));
        assert!(!is_canonical_hex_id("0x002\n"));
        assert!(!is_canonical_hex_id("0x00A"));
        assert!(!is_canonical_hex_id("0X002"));
        assert!(!is_canonical_hex_id("0x"));
        assert!(!is_canonical_hex_id("002"));
        assert!(!is_canonical_hex_id(" 0x002"));
    }

    #[test]
    fn parse_id_requires_the_canonical_width() {
        assert_eq!(parse_id("0x002", 8.0, "alias 0x003").expect("ok"), 2.0);
        assert_eq!(
            parse_id("0x2", 8.0, "alias 0x003").unwrap_err(),
            "alias 0x003 is outside the directory"
        );
        assert_eq!(
            parse_id("0x009", 8.0, "alias 0x003").unwrap_err(),
            "alias 0x003 is outside the directory"
        );
        assert_eq!(
            parse_id("0x00A", 8.0, "alias 0x003").unwrap_err(),
            "alias 0x003 is not a canonical hexadecimal ID"
        );
        // A hex id far past 2^53 must still be rejected, not wrap.
        assert!(parse_id("0xfffffffffffffffffff", 8.0, "alias 0x003").is_err());
    }

    // --- document validation ----------------------------------------------

    #[test]
    fn document_rejects_malformed_sources() {
        assert_eq!(
            document(&parse("[]")).unwrap_err(),
            "resource directory source must be an object"
        );
        assert_eq!(
            document(&parse("null")).unwrap_err(),
            "resource directory source must be an object"
        );
        assert_eq!(
            document(&parse(r#"{"format":2,"kind":"golden-sun-resource-directory"}"#)).unwrap_err(),
            "unsupported resource directory source"
        );
        assert_eq!(
            document(&parse(r#"{"format":1,"kind":"other"}"#)).unwrap_err(),
            "unsupported resource directory source"
        );
        assert_eq!(
            document(&parse(
                r#"{"format":1,"kind":"golden-sun-resource-directory","address":"0x8000100"}"#
            ))
            .unwrap_err(),
            "resource directory address is not canonical hexadecimal"
        );
        assert_eq!(
            document(&parse(
                r#"{"format":1,"kind":"golden-sun-resource-directory","address":"0x00000100"}"#
            ))
            .unwrap_err(),
            "resource directory address is outside the ROM address space"
        );
        assert_eq!(
            document(&parse(
                r#"{"format":1,"kind":"golden-sun-resource-directory","address":"0x08000100","slot_count":1}"#
            ))
            .unwrap_err(),
            "resource directory slot count is outside the supported range"
        );
        assert_eq!(
            document(&parse(
                r#"{"format":1,"kind":"golden-sun-resource-directory","address":"0x08000100","slot_count":2,"slots":["rom-base"]}"#
            ))
            .unwrap_err(),
            "resource directory slots do not match slot_count"
        );
    }

    #[test]
    fn a_numeric_address_is_not_canonical() {
        // hex(start) is a string; `!==` against the number 134217984 is true.
        assert_eq!(
            document(&parse(
                r#"{"format":1,"kind":"golden-sun-resource-directory","address":134217984,"slot_count":2,"slots":["rom-base","reserved-null"]}"#
            ))
            .unwrap_err(),
            "resource directory address is not canonical hexadecimal"
        );
    }

    // --- build -------------------------------------------------------------

    #[test]
    fn build_rejects_bad_slots() {
        let mut source = sample_document();
        source["slots"][3] = Value::String("alias:0x004".to_string());
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "alias 0x003 must reference an earlier slot"
        );

        let mut source = sample_document();
        source["slots"][5] = Value::String("alias:0x004".to_string());
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "alias 0x005 references a reserved slot"
        );

        let mut source = sample_document();
        source["slots"][2] = Value::String("resource:0x8000200".to_string());
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "resource pointer 0x002 is not canonical hexadecimal"
        );

        let mut source = sample_document();
        source["slots"][2] = Value::String("resource:0x00000200".to_string());
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "resource pointer 0x002 is outside the ROM address space"
        );

        let mut source = sample_document();
        source["slots"][2] = Value::from(7u64);
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "invalid resource directory slot 0x002"
        );

        let mut source = sample_document();
        source["slots"][2] = Value::String("unknown".to_string());
        assert_eq!(
            build_resource_directory(&source).unwrap_err(),
            "invalid resource directory slot 0x002"
        );
    }

    #[test]
    fn build_writes_little_endian_pointers() {
        let built = build_resource_directory(&sample_document()).expect("built");
        assert_eq!(&built[..4], &[0x00, 0x00, 0x00, 0x08]);
        assert_eq!(&built[4..8], &[0x00, 0x01, 0x00, 0x08]);
        assert_eq!(&built[16..20], &[0, 0, 0, 0]);
    }

    // --- export ------------------------------------------------------------

    #[test]
    fn export_names_the_first_occurrence_and_aliases_the_rest() {
        let built = build_resource_directory(&sample_document()).expect("built");
        let mut rom = vec![0u8; 0x400];
        rom[0x100..0x100 + built.len()].copy_from_slice(&built);
        let exported = export_resource_directory(&rom, 0x0800_0100 as f64, 8.0).expect("export");
        let slots: Vec<&str> = exported["slots"]
            .as_array()
            .unwrap()
            .iter()
            .map(|v| v.as_str().unwrap())
            .collect();
        assert_eq!(
            slots,
            vec![
                "rom-base",
                "directory-self",
                "resource:0x08000200",
                "alias:0x002",
                "reserved-null",
                "resource:0x08000300",
                "alias:0x005",
                "reserved-null",
            ]
        );
    }

    #[test]
    fn export_treats_a_repeated_rom_base_as_an_alias() {
        let mut rom = vec![0u8; 0x400];
        for index in 0..4 {
            rom[0x100 + index * 4..0x104 + index * 4]
                .copy_from_slice(&0x0800_0000u32.to_le_bytes());
        }
        let exported = export_resource_directory(&rom, 0x0800_0100 as f64, 4.0).expect("export");
        let slots: Vec<&str> = exported["slots"]
            .as_array()
            .unwrap()
            .iter()
            .map(|v| v.as_str().unwrap())
            .collect();
        assert_eq!(
            slots,
            vec!["rom-base", "alias:0x000", "alias:0x000", "alias:0x000"]
        );
    }

    #[test]
    fn export_refuses_a_non_rom_pointer() {
        let mut rom = vec![0u8; 0x400];
        rom[0x100..0x104].copy_from_slice(&0x0000_1234u32.to_le_bytes());
        rom[0x104..0x108].copy_from_slice(&0u32.to_le_bytes());
        assert_eq!(
            export_resource_directory(&rom, 0x0800_0100 as f64, 2.0).unwrap_err(),
            "slot 0x000 is not a ROM pointer"
        );
    }

    #[test]
    fn export_refuses_to_compare_nothing() {
        // A zero or one slot window would let "verified nothing" pass as clean.
        let rom = vec![0u8; 0x400];
        for count in [0.0, 1.0, 65537.0, 8.5] {
            assert_eq!(
                export_resource_directory(&rom, 0x0800_0100 as f64, count).unwrap_err(),
                "resource directory slot count is outside the supported range"
            );
        }
    }

    #[test]
    fn export_refuses_a_window_past_the_end_of_the_rom() {
        let rom = vec![0u8; 0x110];
        assert_eq!(
            export_resource_directory(&rom, 0x0800_0100 as f64, 8.0).unwrap_err(),
            "resource directory lies outside the ROM"
        );
    }

    // --- canonical json ----------------------------------------------------

    #[test]
    fn canonical_json_inlines_primitive_arrays() {
        let value = parse(r#"{"a":[1,2,3],"b":[],"c":{},"d":[[1],[2]],"e":"x"}"#);
        assert_eq!(
            canonical_json(&value),
            "{\n  \"a\": [1, 2, 3],\n  \"b\": [],\n  \"c\": {},\n  \"d\": [\n    [1],\n    [2]\n  ],\n  \"e\": \"x\"\n}"
        );
    }

    #[test]
    fn canonical_json_preserves_insertion_order() {
        // serde_json without `preserve_order` would sort these keys and
        // silently rewrite every committed document.
        let value = parse(r#"{"format":1,"kind":"k","address":"a","slot_count":2,"slots":[]}"#);
        assert_eq!(
            canonical_json(&value),
            "{\n  \"format\": 1,\n  \"kind\": \"k\",\n  \"address\": \"a\",\n  \"slot_count\": 2,\n  \"slots\": []\n}"
        );
    }

    #[test]
    fn canonical_json_escapes_like_json_stringify() {
        let value = Value::String("a\"b\\c\nd\te\u{1}f\u{7f}g".to_string());
        assert_eq!(
            canonical_json(&value),
            "\"a\\\"b\\\\c\\nd\\te\\u0001f\u{7f}g\""
        );
    }

    #[test]
    fn canonical_json_prints_integral_floats_without_a_point() {
        assert_eq!(canonical_json(&parse("8.0")), "8");
        assert_eq!(canonical_json(&parse("-1.0")), "-1");
        assert_eq!(canonical_json(&parse("0.5")), "0.5");
        assert_eq!(canonical_json(&parse("true")), "true");
        assert_eq!(canonical_json(&parse("null")), "null");
    }

    #[test]
    fn exported_document_serialises_in_field_order() {
        let built = build_resource_directory(&sample_document()).expect("built");
        let mut rom = vec![0u8; 0x400];
        rom[0x100..0x100 + built.len()].copy_from_slice(&built);
        let exported = export_resource_directory(&rom, 0x0800_0100 as f64, 8.0).expect("export");
        let text = canonical_json(&exported);
        assert!(text.starts_with(
            "{\n  \"format\": 1,\n  \"kind\": \"golden-sun-resource-directory\",\n  \"address\": \"0x08000100\",\n  \"slot_count\": 8,\n  \"slots\": ["
        ));
        assert!(text.ends_with("\"reserved-null\"]\n}"));
    }

    // --- lossy utf-8 -------------------------------------------------------

    #[test]
    fn invalid_utf8_input_is_replaced_not_rejected() {
        let raw = b"{\"kind\":\"\xff\"}";
        let text = String::from_utf8_lossy(raw);
        assert_eq!(text, "{\"kind\":\"\u{fffd}\"}");
        let value: Value = serde_json::from_str(&text).expect("json");
        assert_eq!(value["kind"], Value::String("\u{fffd}".to_string()));
    }

    #[test]
    fn js_number_string_matches_string_of_a_number() {
        assert_eq!(js_number_string(1000.0), "1000");
        assert_eq!(js_number_string(8.0), "8");
        assert_eq!(js_number_string(0.0), "0");
        assert_eq!(js_number_string(-0.0), "0");
        assert_eq!(js_number_string(4000.0), "4000");
    }
}
