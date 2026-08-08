//! Port of `tools/make/music.ts`: GBA sound-table extract/build.
//!
//! PORT NOTE: `music.ts` imports `canonicalJson` from `tools/lib/canonical_json.ts`.
//! Only the writer half is needed here, so the `reflow`/`canonicalJson` pair is
//! inlined below as [`Json`] + [`canonical_json`] (module-private helpers plus a
//! public `canonical_json`). `isCanonicalJsonText` is NOT ported: `music.ts`
//! never calls it. The later port of `canonical_json.ts` owns that function and
//! should treat this copy as the same algorithm, character for character:
//! objects one key per line, arrays of all-primitive elements inline with
//! `", "` separators, empty containers as `[]` / `{}`.
//!
//! PORT NOTE: JSON is emitted through a small hand-rolled [`Json`] value with an
//! insertion-ordered object (`Vec<(String, Json)>`) rather than a dependency,
//! because the tool only ever writes JSON and never parses it. Key order is
//! therefore exactly the JS object insertion order.

use std::collections::{HashMap, HashSet};
use std::fmt::Write as _;

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;

// ---------------------------------------------------------------------------
// canonical JSON (inlined from tools/lib/canonical_json.ts)
// ---------------------------------------------------------------------------

#[derive(Debug, Clone, PartialEq)]
pub enum Json {
    Null,
    Bool(bool),
    Number(f64),
    String(String),
    Array(Vec<Json>),
    Object(Vec<(String, Json)>),
}

impl Json {
    fn is_primitive(&self) -> bool {
        matches!(
            self,
            Json::Null | Json::Bool(_) | Json::Number(_) | Json::String(_)
        )
    }
}

/// `JSON.stringify` for a primitive value.
fn stringify_primitive(value: &Json) -> String {
    match value {
        Json::Null => "null".to_string(),
        Json::Bool(b) => b.to_string(),
        Json::Number(n) => format_number(*n),
        Json::String(s) => quote(s),
        _ => unreachable!("stringify_primitive on a container"),
    }
}

/// Number formatting matching `JSON.stringify` for the values this tool emits
/// (finite integers and plain decimals). Non-finite numbers become `null`, as
/// in JS.
fn format_number(n: f64) -> String {
    if !n.is_finite() {
        return "null".to_string();
    }
    if n == n.trunc() && n.abs() < 1e21 {
        // -0 stringifies as "0" in JS.
        let i = n as i64;
        return i.to_string();
    }
    let mut s = format!("{n}");
    if s.ends_with(".0") {
        s.truncate(s.len() - 2);
    }
    s
}

/// `JSON.stringify` for a string: minimal escapes, `\u00xx` for other controls.
fn quote(s: &str) -> String {
    let mut out = String::with_capacity(s.len() + 2);
    out.push('"');
    for ch in s.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            c if (c as u32) < 0x20 => {
                let _ = write!(out, "\\u{:04x}", c as u32);
            }
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

fn reflow(value: &Json, indent: &str) -> String {
    if value.is_primitive() {
        return stringify_primitive(value);
    }
    match value {
        Json::Array(items) => {
            if items.is_empty() {
                return "[]".to_string();
            }
            if items.iter().all(Json::is_primitive) {
                let parts: Vec<String> = items.iter().map(stringify_primitive).collect();
                return format!("[{}]", parts.join(", "));
            }
            let inner = format!("{indent}  ");
            let parts: Vec<String> = items
                .iter()
                .map(|x| format!("{inner}{}", reflow(x, &inner)))
                .collect();
            format!("[\n{}\n{indent}]", parts.join(",\n"))
        }
        Json::Object(pairs) => {
            if pairs.is_empty() {
                return "{}".to_string();
            }
            let inner = format!("{indent}  ");
            let parts: Vec<String> = pairs
                .iter()
                .map(|(k, v)| format!("{inner}{}: {}", quote(k), reflow(v, &inner)))
                .collect();
            format!("{{\n{}\n{indent}}}", parts.join(",\n"))
        }
        _ => unreachable!(),
    }
}

pub fn canonical_json(value: &Json) -> String {
    reflow(value, "")
}

// ---------------------------------------------------------------------------
// sound table
// ---------------------------------------------------------------------------

/// `[header, player]`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct SoundTableEntry {
    pub header: String,
    pub player: i64,
}

/// PORT NOTE: `format`, `fields` and `auxiliary` are validated in the TS because
/// the source can arrive as untrusted JSON. They are kept as real fields here so
/// a hand-built or future JSON-parsed source is validated identically.
#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableSource {
    pub format: i64,
    pub fields: Vec<String>,
    pub auxiliary: String,
    /// Insertion-ordered, mirroring the JS object.
    pub symbols: Vec<(String, SymbolValue)>,
    pub entries: Vec<SoundTableEntry>,
}

/// A symbol address is `string | number` in the TS source.
#[derive(Debug, Clone, PartialEq)]
pub enum SymbolValue {
    Text(String),
    Number(f64),
}

#[derive(Debug, Clone, PartialEq)]
pub struct SoundTableReport {
    pub entries: usize,
    pub unique_headers: usize,
    /// Insertion-ordered player histogram, keyed by the decimal player number.
    pub players: Vec<(String, u64)>,
    pub mirrored_auxiliary: bool,
}

impl SoundTableSource {
    pub fn to_json(&self) -> Json {
        Json::Object(vec![
            ("format".into(), Json::Number(self.format as f64)),
            (
                "fields".into(),
                Json::Array(
                    self.fields
                        .iter()
                        .map(|f| Json::String(f.clone()))
                        .collect(),
                ),
            ),
            ("auxiliary".into(), Json::String(self.auxiliary.clone())),
            (
                "symbols".into(),
                Json::Object(
                    self.symbols
                        .iter()
                        .map(|(k, v)| {
                            (
                                k.clone(),
                                match v {
                                    SymbolValue::Text(s) => Json::String(s.clone()),
                                    SymbolValue::Number(n) => Json::Number(*n),
                                },
                            )
                        })
                        .collect(),
                ),
            ),
            (
                "entries".into(),
                Json::Array(
                    self.entries
                        .iter()
                        .map(|e| {
                            Json::Array(vec![
                                Json::String(e.header.clone()),
                                Json::Number(e.player as f64),
                            ])
                        })
                        .collect(),
                ),
            ),
        ])
    }
}

/// `Number(value)` semantics for the string form, then `Number.isInteger`.
pub fn js_number(text: &str) -> f64 {
    let t = text.trim();
    if t.is_empty() {
        return 0.0;
    }
    let (sign, body) = match t.strip_prefix('-') {
        Some(rest) => (-1.0, rest),
        None => (1.0, t.strip_prefix('+').unwrap_or(t)),
    };
    let radix_parse = |digits: &str, radix: u32| -> f64 {
        if digits.is_empty() || !digits.chars().all(|c| c.is_digit(radix)) {
            return f64::NAN;
        }
        let mut acc = 0.0f64;
        for c in digits.chars() {
            acc = acc * f64::from(radix) + f64::from(c.to_digit(radix).unwrap());
        }
        acc
    };
    // JS rejects a sign in front of 0x/0o/0b literals.
    if let Some(d) = body.strip_prefix("0x").or_else(|| body.strip_prefix("0X")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 16)
        };
    }
    if let Some(d) = body.strip_prefix("0o").or_else(|| body.strip_prefix("0O")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 8)
        };
    }
    if let Some(d) = body.strip_prefix("0b").or_else(|| body.strip_prefix("0B")) {
        return if sign < 0.0 {
            f64::NAN
        } else {
            radix_parse(d, 2)
        };
    }
    if body == "Infinity" {
        return sign * f64::INFINITY;
    }
    match body.parse::<f64>() {
        // Rust's f64 parser accepts "inf"/"nan"/"1e5"; JS's Number() does not
        // accept the first two spellings.
        Ok(v) if !body.eq_ignore_ascii_case("inf")
            && !body.eq_ignore_ascii_case("infinity")
            && !body.eq_ignore_ascii_case("nan") => sign * v,
        _ => f64::NAN,
    }
}

fn integer_from_str(value: &str, label: &str) -> Result<i64> {
    integer_from_f64(js_number(value), label)
}

fn integer_from_f64(result: f64, label: &str) -> Result<i64> {
    if !result.is_finite() || result != result.trunc() {
        return Err(format!("{label} must be an integer"));
    }
    Ok(result as i64)
}

fn u16_check(value: i64, label: &str) -> Result<u16> {
    if !(0..=0xffff).contains(&value) {
        return Err(format!("{label} does not fit u16"));
    }
    Ok(value as u16)
}

fn address_from_i64(result: i64, label: &str) -> Result<u32> {
    if !(0x0800_0000..0x0a00_0000).contains(&result) || result % 4 != 0 {
        return Err(format!("{label} is not an aligned ROM address"));
    }
    Ok(result as u32)
}

fn address(value: &SymbolValue, label: &str) -> Result<u32> {
    let result = match value {
        SymbolValue::Text(s) => integer_from_str(s, label)?,
        SymbolValue::Number(n) => integer_from_f64(*n, label)?,
    };
    address_from_i64(result, label)
}

fn symbol_name(index: usize) -> String {
    format!("sound_{index:03}")
}

fn valid_symbol(name: &str) -> bool {
    let mut chars = name.chars();
    match chars.next() {
        Some(c) if c.is_ascii_lowercase() => {}
        _ => return false,
    }
    chars.all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
}

pub fn build_sound_table(source: &SoundTableSource) -> Result<(Vec<u8>, SoundTableReport)> {
    if source.format != 1 || source.fields != ["header", "player"] || source.auxiliary != "copy-player"
    {
        return Err("unsupported sound-table source".to_string());
    }
    let mut symbols: HashMap<&str, u32> = HashMap::new();
    let mut order: Vec<&str> = Vec::new();
    let mut addresses: HashSet<u32> = HashSet::new();
    for (name, value) in &source.symbols {
        if !valid_symbol(name) {
            return Err(format!("invalid sound symbol: {name}"));
        }
        let resolved = address(value, &format!("symbol {name}"))?;
        if addresses.contains(&resolved) {
            return Err(format!("duplicate sound-header address: {name}"));
        }
        if symbols.insert(name.as_str(), resolved).is_none() {
            order.push(name.as_str());
        }
        addresses.insert(resolved);
    }
    let mut used: HashSet<&str> = HashSet::new();
    let mut players: Vec<(String, u64)> = Vec::new();
    let mut built = vec![0u8; source.entries.len() * 8];
    for (index, entry) in source.entries.iter().enumerate() {
        let Some(&header) = symbols.get(entry.header.as_str()) else {
            return Err(format!("entry {index} references an unknown sound header"));
        };
        let label = format!("entry {index} player");
        let player = u16_check(integer_from_f64(entry.player as f64, &label)?, &label)?;
        let base = index * 8;
        built[base..base + 4].copy_from_slice(&header.to_le_bytes());
        built[base + 4..base + 6].copy_from_slice(&player.to_le_bytes());
        built[base + 6..base + 8].copy_from_slice(&player.to_le_bytes());
        used.insert(entry.header.as_str());
        let key = player.to_string();
        match players.iter_mut().find(|(k, _)| *k == key) {
            Some((_, count)) => *count += 1,
            None => players.push((key, 1)),
        }
    }
    let unused: Vec<&str> = order
        .iter()
        .copied()
        .filter(|name| !used.contains(name))
        .collect();
    if !unused.is_empty() {
        return Err(format!("unused sound symbols: {}", unused.join(", ")));
    }
    let report = SoundTableReport {
        entries: source.entries.len(),
        unique_headers: symbols.len(),
        players,
        mirrored_auxiliary: true,
    };
    Ok((built, report))
}

fn read_u32_le(data: &[u8], at: usize) -> u32 {
    u32::from_le_bytes([data[at], data[at + 1], data[at + 2], data[at + 3]])
}

fn read_u16_le(data: &[u8], at: usize) -> u16 {
    u16::from_le_bytes([data[at], data[at + 1]])
}

pub fn extract_sound_table(rom: &[u8], start: i64, count: i64) -> Result<SoundTableSource> {
    let offset = start - 0x0800_0000;
    let end = offset.checked_add(count.saturating_mul(8));
    if count <= 0 || offset < 0 || end.is_none_or(|e| e > rom.len() as i64) {
        return Err("sound table lies outside ROM".to_string());
    }
    let offset = offset as usize;
    let mut by_address: HashMap<u32, String> = HashMap::new();
    let mut symbols: Vec<(String, SymbolValue)> = Vec::new();
    let mut entries: Vec<SoundTableEntry> = Vec::new();
    for index in 0..count as usize {
        let entry_offset = offset + index * 8;
        let raw = read_u32_le(rom, entry_offset);
        let header_address =
            address_from_i64(i64::from(raw), &format!("entry {index} header"))?;
        let name = match by_address.get(&header_address) {
            Some(name) => name.clone(),
            None => {
                let header_offset = i64::from(header_address) - 0x0800_0000;
                let empty = header_offset >= 0
                    && header_offset + 4 <= rom.len() as i64
                    && read_u32_le(rom, header_offset as usize) == 0;
                let name = if empty && !symbols.iter().any(|(k, _)| k == "sound_empty") {
                    "sound_empty".to_string()
                } else {
                    symbol_name(index)
                };
                by_address.insert(header_address, name.clone());
                let text = SymbolValue::Text(format!("0x{header_address:08x}"));
                match symbols.iter_mut().find(|(k, _)| *k == name) {
                    Some(slot) => slot.1 = text,
                    None => symbols.push((name.clone(), text)),
                }
                name
            }
        };
        let player = read_u16_le(rom, entry_offset + 4);
        let auxiliary = read_u16_le(rom, entry_offset + 6);
        if auxiliary != player {
            return Err(format!(
                "entry {index} does not mirror its player selector"
            ));
        }
        entries.push(SoundTableEntry {
            header: name,
            player: i64::from(player),
        });
    }
    Ok(SoundTableSource {
        format: 1,
        fields: vec!["header".into(), "player".into()],
        auxiliary: "copy-player".into(),
        symbols,
        entries,
    })
}

/// Mirrors `self_test()` in the TS (which prints `self-test=ok` on success).
pub fn self_test() -> Result<()> {
    let source = SoundTableSource {
        format: 1,
        fields: vec!["header".into(), "player".into()],
        auxiliary: "copy-player".into(),
        symbols: vec![
            ("sound_000".into(), SymbolValue::Text("0x08000100".into())),
            ("sound_001".into(), SymbolValue::Text("0x08000200".into())),
        ],
        entries: vec![
            SoundTableEntry { header: "sound_000".into(), player: 1 },
            SoundTableEntry { header: "sound_001".into(), player: 7 },
            SoundTableEntry { header: "sound_000".into(), player: 1 },
        ],
    };
    let (built, report) = build_sound_table(&source)?;
    let mut expected = vec![0u8; 24];
    expected[0..4].copy_from_slice(&0x0800_0100u32.to_le_bytes());
    expected[4..6].copy_from_slice(&1u16.to_le_bytes());
    expected[6..8].copy_from_slice(&1u16.to_le_bytes());
    expected[8..12].copy_from_slice(&0x0800_0200u32.to_le_bytes());
    expected[12..14].copy_from_slice(&7u16.to_le_bytes());
    expected[14..16].copy_from_slice(&7u16.to_le_bytes());
    expected[16..20].copy_from_slice(&0x0800_0100u32.to_le_bytes());
    expected[20..22].copy_from_slice(&1u16.to_le_bytes());
    expected[22..24].copy_from_slice(&1u16.to_le_bytes());
    if built != expected || report.entries != 3 || report.unique_headers != 2 {
        return Err("sound-table self-test failed".to_string());
    }
    let mut rom = vec![0u8; 0x100];
    rom.extend_from_slice(&built);
    let extracted = extract_sound_table(&rom, 0x0800_0100, 3)?;
    let (round_trip, _) = build_sound_table(&extracted)?;
    if round_trip != built {
        return Err("sound-table round-trip failed".to_string());
    }
    Ok(())
}

// ---------------------------------------------------------------------------
// CLI helpers
// ---------------------------------------------------------------------------

pub fn option(args: &[String], name: &str) -> Result<String> {
    match args.iter().position(|a| a == name) {
        Some(index) if index + 1 < args.len() => Ok(args[index + 1].clone()),
        _ => Err(format!("{name} is required")),
    }
}

pub fn cli_integer(value: &str, label: &str) -> Result<i64> {
    integer_from_str(value, label)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn text(name: &str, addr: &str) -> (String, SymbolValue) {
        (name.to_string(), SymbolValue::Text(addr.to_string()))
    }

    fn entry(header: &str, player: i64) -> SoundTableEntry {
        SoundTableEntry { header: header.to_string(), player }
    }

    fn demo() -> SoundTableSource {
        SoundTableSource {
            format: 1,
            fields: vec!["header".into(), "player".into()],
            auxiliary: "copy-player".into(),
            symbols: vec![text("sound_000", "0x08000100"), text("sound_001", "0x08000200")],
            entries: vec![entry("sound_000", 1), entry("sound_001", 7), entry("sound_000", 1)],
        }
    }

    #[test]
    fn ts_self_test_passes() {
        self_test().unwrap();
    }

    #[test]
    fn build_emits_exact_bytes_and_report() {
        let (built, report) = build_sound_table(&demo()).unwrap();
        assert_eq!(
            built,
            vec![
                0x00, 0x01, 0x00, 0x08, 1, 0, 1, 0, //
                0x00, 0x02, 0x00, 0x08, 7, 0, 7, 0, //
                0x00, 0x01, 0x00, 0x08, 1, 0, 1, 0,
            ]
        );
        assert_eq!(report.entries, 3);
        assert_eq!(report.unique_headers, 2);
        assert_eq!(report.players, vec![("1".to_string(), 2), ("7".to_string(), 1)]);
        assert!(report.mirrored_auxiliary);
    }

    #[test]
    fn build_accepts_numeric_symbol_values() {
        let mut source = demo();
        source.symbols[0].1 = SymbolValue::Number(0x0800_0100 as f64);
        let (built, _) = build_sound_table(&source).unwrap();
        assert_eq!(&built[0..4], &0x0800_0100u32.to_le_bytes());
    }

    #[test]
    fn build_rejects_bad_sources() {
        let mut bad = demo();
        bad.format = 2;
        assert_eq!(build_sound_table(&bad).unwrap_err(), "unsupported sound-table source");

        let mut bad = demo();
        bad.fields = vec!["player".into(), "header".into()];
        assert_eq!(build_sound_table(&bad).unwrap_err(), "unsupported sound-table source");

        let mut bad = demo();
        bad.auxiliary = "mirror".into();
        assert_eq!(build_sound_table(&bad).unwrap_err(), "unsupported sound-table source");

        let mut bad = demo();
        bad.symbols[0].0 = "Sound_000".into();
        assert_eq!(build_sound_table(&bad).unwrap_err(), "invalid sound symbol: Sound_000");

        let mut bad = demo();
        bad.symbols[1].1 = SymbolValue::Text("0x08000100".into());
        assert_eq!(
            build_sound_table(&bad).unwrap_err(),
            "duplicate sound-header address: sound_001"
        );

        let mut bad = demo();
        bad.symbols[0].1 = SymbolValue::Text("0x08000102".into());
        assert_eq!(
            build_sound_table(&bad).unwrap_err(),
            "symbol sound_000 is not an aligned ROM address"
        );

        let mut bad = demo();
        bad.symbols[0].1 = SymbolValue::Text("0x07fffffc".into());
        assert_eq!(
            build_sound_table(&bad).unwrap_err(),
            "symbol sound_000 is not an aligned ROM address"
        );

        let mut bad = demo();
        bad.symbols[0].1 = SymbolValue::Text("1.5".into());
        assert_eq!(build_sound_table(&bad).unwrap_err(), "symbol sound_000 must be an integer");

        let mut bad = demo();
        bad.entries[0].header = "sound_009".into();
        assert_eq!(
            build_sound_table(&bad).unwrap_err(),
            "entry 0 references an unknown sound header"
        );

        let mut bad = demo();
        bad.entries[0].player = 0x10000;
        assert_eq!(build_sound_table(&bad).unwrap_err(), "entry 0 player does not fit u16");

        let mut bad = demo();
        bad.entries[0].player = -1;
        assert_eq!(build_sound_table(&bad).unwrap_err(), "entry 0 player does not fit u16");
    }

    #[test]
    fn build_reports_unused_symbols_in_insertion_order() {
        let mut source = demo();
        source
            .symbols
            .push(text("sound_zzz", "0x08000300"));
        source.symbols.push(text("sound_aaa", "0x08000400"));
        assert_eq!(
            build_sound_table(&source).unwrap_err(),
            "unused sound symbols: sound_zzz, sound_aaa"
        );
    }

    fn rom_with(table: &[u8]) -> Vec<u8> {
        let mut rom = vec![0u8; 0x100];
        rom.extend_from_slice(table);
        rom
    }

    #[test]
    fn extract_round_trips_bytes_exactly() {
        let (built, _) = build_sound_table(&demo()).unwrap();
        let rom = rom_with(&built);
        let extracted = extract_sound_table(&rom, 0x0800_0100, 3).unwrap();
        let (round_trip, report) = build_sound_table(&extracted).unwrap();
        assert_eq!(round_trip, built);
        assert_eq!(report.unique_headers, 2);
        // 0x08000200 lies past the end of this short ROM, so the emptiness
        // probe fails and the index-derived name is used.
        assert_eq!(
            extracted.symbols,
            vec![
                text("sound_000", "0x08000100"),
                text("sound_001", "0x08000200"),
            ]
        );
        assert_eq!(
            extracted.entries,
            vec![entry("sound_000", 1), entry("sound_001", 7), entry("sound_000", 1)]
        );
    }

    #[test]
    fn extract_uses_sound_empty_only_once() {
        // Two distinct addresses both pointing at zeroed ROM.
        let mut table = Vec::new();
        for (addr, player) in [(0x0800_0400u32, 3u16), (0x0800_0500, 4)] {
            table.extend_from_slice(&addr.to_le_bytes());
            table.extend_from_slice(&player.to_le_bytes());
            table.extend_from_slice(&player.to_le_bytes());
        }
        let mut rom = rom_with(&table);
        rom.resize(0x1000, 0);
        let extracted = extract_sound_table(&rom, 0x0800_0100, 2).unwrap();
        assert_eq!(
            extracted.symbols,
            vec![
                text("sound_empty", "0x08000400"),
                text("sound_001", "0x08000500"),
            ]
        );
    }

    #[test]
    fn extract_rejects_unmirrored_player() {
        let mut table = Vec::new();
        table.extend_from_slice(&0x0800_0400u32.to_le_bytes());
        table.extend_from_slice(&5u16.to_le_bytes());
        table.extend_from_slice(&6u16.to_le_bytes());
        let rom = rom_with(&table);
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, 1).unwrap_err(),
            "entry 0 does not mirror its player selector"
        );
    }

    #[test]
    fn extract_rejects_out_of_range() {
        let rom = rom_with(&[0u8; 8]);
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, 0).unwrap_err(),
            "sound table lies outside ROM"
        );
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, -1).unwrap_err(),
            "sound table lies outside ROM"
        );
        assert_eq!(
            extract_sound_table(&rom, 0x07ff_ffff, 1).unwrap_err(),
            "sound table lies outside ROM"
        );
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, 2).unwrap_err(),
            "sound table lies outside ROM"
        );
        // No overflow panic on an absurd count.
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, i64::MAX).unwrap_err(),
            "sound table lies outside ROM"
        );
    }

    #[test]
    fn extract_rejects_unaligned_header_pointer() {
        let mut table = Vec::new();
        table.extend_from_slice(&0x0800_0402u32.to_le_bytes());
        table.extend_from_slice(&0u16.to_le_bytes());
        table.extend_from_slice(&0u16.to_le_bytes());
        let rom = rom_with(&table);
        assert_eq!(
            extract_sound_table(&rom, 0x0800_0100, 1).unwrap_err(),
            "entry 0 header is not an aligned ROM address"
        );
    }

    #[test]
    fn canonical_json_matches_the_ts_writer() {
        let (built, _) = build_sound_table(&demo()).unwrap();
        let rom = rom_with(&built);
        let extracted = extract_sound_table(&rom, 0x0800_0100, 3).unwrap();
        assert_eq!(
            canonical_json(&extracted.to_json()),
            concat!(
                "{\n",
                "  \"format\": 1,\n",
                "  \"fields\": [\"header\", \"player\"],\n",
                "  \"auxiliary\": \"copy-player\",\n",
                "  \"symbols\": {\n",
                "    \"sound_000\": \"0x08000100\",\n",
                "    \"sound_001\": \"0x08000200\"\n",
                "  },\n",
                "  \"entries\": [\n",
                "    [\"sound_000\", 1],\n",
                "    [\"sound_001\", 7],\n",
                "    [\"sound_000\", 1]\n",
                "  ]\n",
                "}"
            )
        );
    }

    #[test]
    fn canonical_json_edge_shapes() {
        assert_eq!(canonical_json(&Json::Array(vec![])), "[]");
        assert_eq!(canonical_json(&Json::Object(vec![])), "{}");
        assert_eq!(
            canonical_json(&Json::Object(vec![(
                "a\"b\n".into(),
                Json::Array(vec![Json::Null, Json::Bool(true), Json::Number(-0.0)])
            )])),
            "{\n  \"a\\\"b\\n\": [null, true, 0]\n}"
        );
    }

    #[test]
    fn js_number_parses_like_javascript() {
        assert_eq!(js_number("0x08000100"), 134_217_984.0);
        assert_eq!(js_number(" 42 "), 42.0);
        assert_eq!(js_number(""), 0.0);
        assert_eq!(js_number("0b101"), 5.0);
        assert_eq!(js_number("0o17"), 15.0);
        assert_eq!(js_number("1e3"), 1000.0);
        assert_eq!(js_number("-8"), -8.0);
        assert!(js_number("-0x10").is_nan());
        assert!(js_number("nan").is_nan());
        assert!(js_number("inf").is_nan());
        assert!(js_number("12abc").is_nan());
        assert_eq!(js_number("Infinity"), f64::INFINITY);
        assert_eq!(cli_integer("0x08000100", "--address").unwrap(), 0x0800_0100);
        assert_eq!(cli_integer("Infinity", "--count").unwrap_err(), "--count must be an integer");
        assert_eq!(cli_integer("1.5", "--count").unwrap_err(), "--count must be an integer");
    }

    #[test]
    fn option_lookup_matches_ts() {
        let args: Vec<String> = ["a", "-o", "out.json"].iter().map(|s| s.to_string()).collect();
        assert_eq!(option(&args, "-o").unwrap(), "out.json");
        assert_eq!(option(&args, "--address").unwrap_err(), "--address is required");
        let trailing: Vec<String> = vec!["-o".to_string()];
        assert_eq!(option(&trailing, "-o").unwrap_err(), "-o is required");
    }
}
