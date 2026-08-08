//! Drives the port over the real PCM-wave catalog in `assets/audio/`.
//!
//! The 32 WAV files listed by `assets/audio/waves_index.json` are the same
//! inputs `tools/make/build_assets.ts` feeds to `buildWaveRecord`. Each record
//! is built from the catalog fields, probed back out, and rebuilt through the
//! exact-header path from the probed values; both builds must agree byte for
//! byte. The concatenated bytes and the per-wave report lines are written to
//! `$AUDIO_WAVE_DUMP` / `$AUDIO_WAVE_REPORT` when those variables are set, so
//! the same artifacts can be diffed against the TypeScript original.
//!
//! A tiny JSON reader lives at the bottom of this file: the crate has zero
//! dependencies, and the index format is a flat array of scalar-valued
//! objects.

use audio_wave::{build_wave_record, probe_wave_record, Scalar, WaveRecordSource};
use std::path::{Path, PathBuf};

fn repo_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

/// JS `JSON.stringify` for the integral f64 values this catalog produces.
fn number_text(value: f64) -> String {
    assert!(value.is_finite() && value.fract() == 0.0, "{value}");
    format!("{}", value as i64)
}

fn scalar_text(value: &Scalar) -> String {
    match value {
        Scalar::Num(number) => number_text(*number),
        Scalar::Str(text) => format!("\"{text}\""),
    }
}

#[test]
fn real_wave_catalog_builds_and_round_trips() {
    let root = repo_root();
    let index_name = "assets/audio/waves_index.json";
    let index_text = std::fs::read_to_string(root.join(index_name)).unwrap();
    let index = json::parse(&index_text);
    assert_eq!(index.get("format").unwrap().number(), 1.0);
    assert_eq!(
        index.get("engine").unwrap().string(),
        "smsh-pcm-wave-series"
    );
    let waves = index.get("waves").unwrap().array();
    assert_eq!(waves.len(), 32);

    let mut dump: Vec<u8> = Vec::new();
    let mut lines: Vec<String> = Vec::new();

    for wave in waves {
        let name = wave.get("name").unwrap().string();
        let source = format!(
            "assets/audio/waves_{}",
            wave.get("source").unwrap().string()
        );
        let wav = std::fs::read(root.join(&source)).unwrap();
        let catalog = WaveRecordSource {
            frequency: wave.get("frequency").unwrap().scalar(),
            loop_start: match wave.get("loop_start").unwrap() {
                json::Value::Null => None,
                other => Some(other.number()),
            },
            size: wave.get("size").unwrap().scalar(),
            header: None,
            padding: None,
        };
        let (built, report) = build_wave_record(&catalog, &wav).unwrap();
        assert_eq!(
            built.len() as f64,
            report.samples + 16.0 + report.padding_bytes
        );
        dump.extend_from_slice(&built);
        lines.push(format!("{name} {source} {}", report_text(&report)));

        let probed = probe_wave_record(&built, 0.0, built.len() as f64).unwrap();
        assert_eq!(probed.samples.len() as f64, report.samples);
        let exact = WaveRecordSource {
            header: Some(probed.header.clone()),
            padding: Some(probed.padding.clone()),
            ..catalog
        };
        let (rebuilt, exact_report) = build_wave_record(&exact, &wav).unwrap();
        assert_eq!(rebuilt, built, "{name} exact rebuild differs");
        dump.extend_from_slice(&rebuilt);
        lines.push(format!(
            "{name} exact {{\"control\":{},\"frequency\":{},\"loop_start\":{},\"sample_count\":{}}} {{\"size\":{},\"fill\":{}}} {}",
            scalar_text(&probed.header.control),
            scalar_text(&probed.header.frequency),
            scalar_text(&probed.header.loop_start),
            scalar_text(&probed.header.sample_count),
            scalar_text(&probed.padding.size),
            scalar_text(&probed.padding.fill),
            report_text(&exact_report),
        ));
    }

    lines.push(format!("total_bytes={}", dump.len()));
    let text = format!("{}\n", lines.join("\n"));
    if let Ok(path) = std::env::var("AUDIO_WAVE_DUMP") {
        std::fs::write(path, &dump).unwrap();
    }
    if let Ok(path) = std::env::var("AUDIO_WAVE_REPORT") {
        std::fs::write(path, &text).unwrap();
    }
    assert!(dump.len() > 100_000);
}

fn report_text(report: &audio_wave::WaveReport) -> String {
    format!(
        "{{\"samples\":{},\"rate\":{},\"frequency\":{},\"control\":{},\"looped\":{},\"loop_start\":{},\"padding_bytes\":{},\"padding_fill\":{}}}",
        number_text(report.samples),
        number_text(report.rate),
        number_text(report.frequency),
        number_text(report.control),
        report.looped,
        match report.loop_start {
            None => "null".to_string(),
            Some(value) => number_text(value),
        },
        number_text(report.padding_bytes),
        number_text(report.padding_fill),
    )
}

/// Minimal JSON reader, sufficient for the catalog index. Panics on anything
/// it does not understand, which is the right behaviour inside a test.
mod json {
    use audio_wave::Scalar;

    #[derive(Debug, Clone)]
    pub enum Value {
        Null,
        Num(f64),
        Str(String),
        Arr(Vec<Value>),
        Obj(Vec<(String, Value)>),
    }

    impl Value {
        pub fn get(&self, key: &str) -> Option<&Value> {
            match self {
                Value::Obj(entries) => entries
                    .iter()
                    .find(|(name, _)| name == key)
                    .map(|(_, value)| value),
                _ => None,
            }
        }
        pub fn array(&self) -> &[Value] {
            match self {
                Value::Arr(items) => items,
                _ => panic!("not an array"),
            }
        }
        pub fn number(&self) -> f64 {
            match self {
                Value::Num(number) => *number,
                _ => panic!("not a number"),
            }
        }
        pub fn string(&self) -> &str {
            match self {
                Value::Str(text) => text,
                _ => panic!("not a string"),
            }
        }
        /// Catalog scalars arrive as either JSON strings or JSON numbers,
        /// exactly like the TS `string | number` unions.
        pub fn scalar(&self) -> Scalar {
            match self {
                Value::Num(number) => Scalar::Num(*number),
                Value::Str(text) => Scalar::Str(text.clone()),
                _ => panic!("not a scalar"),
            }
        }
    }

    pub fn parse(text: &str) -> Value {
        let bytes: Vec<char> = text.chars().collect();
        let mut cursor = 0usize;
        let value = parse_value(&bytes, &mut cursor);
        skip_space(&bytes, &mut cursor);
        assert_eq!(cursor, bytes.len(), "trailing JSON content");
        value
    }

    fn skip_space(bytes: &[char], cursor: &mut usize) {
        while *cursor < bytes.len() && bytes[*cursor].is_whitespace() {
            *cursor += 1;
        }
    }

    fn expect(bytes: &[char], cursor: &mut usize, want: char) {
        assert_eq!(bytes[*cursor], want);
        *cursor += 1;
    }

    fn parse_value(bytes: &[char], cursor: &mut usize) -> Value {
        skip_space(bytes, cursor);
        match bytes[*cursor] {
            '{' => {
                *cursor += 1;
                let mut entries = Vec::new();
                skip_space(bytes, cursor);
                if bytes[*cursor] == '}' {
                    *cursor += 1;
                    return Value::Obj(entries);
                }
                loop {
                    skip_space(bytes, cursor);
                    let key = parse_string(bytes, cursor);
                    skip_space(bytes, cursor);
                    expect(bytes, cursor, ':');
                    entries.push((key, parse_value(bytes, cursor)));
                    skip_space(bytes, cursor);
                    match bytes[*cursor] {
                        ',' => *cursor += 1,
                        '}' => {
                            *cursor += 1;
                            return Value::Obj(entries);
                        }
                        other => panic!("unexpected {other:?} in object"),
                    }
                }
            }
            '[' => {
                *cursor += 1;
                let mut items = Vec::new();
                skip_space(bytes, cursor);
                if bytes[*cursor] == ']' {
                    *cursor += 1;
                    return Value::Arr(items);
                }
                loop {
                    items.push(parse_value(bytes, cursor));
                    skip_space(bytes, cursor);
                    match bytes[*cursor] {
                        ',' => *cursor += 1,
                        ']' => {
                            *cursor += 1;
                            return Value::Arr(items);
                        }
                        other => panic!("unexpected {other:?} in array"),
                    }
                }
            }
            '"' => Value::Str(parse_string(bytes, cursor)),
            // The catalog index contains no booleans; a `true`/`false` here
            // would mean the format changed under the port.
            't' | 'f' => panic!("unexpected boolean in the wave catalog index"),
            'n' => {
                *cursor += 4;
                Value::Null
            }
            _ => {
                let start = *cursor;
                while *cursor < bytes.len()
                    && matches!(bytes[*cursor], '0'..='9' | '-' | '+' | '.' | 'e' | 'E')
                {
                    *cursor += 1;
                }
                let text: String = bytes[start..*cursor].iter().collect();
                Value::Num(text.parse().expect("number"))
            }
        }
    }

    fn parse_string(bytes: &[char], cursor: &mut usize) -> String {
        expect(bytes, cursor, '"');
        let mut result = String::new();
        loop {
            let c = bytes[*cursor];
            *cursor += 1;
            match c {
                '"' => return result,
                '\\' => {
                    let escape = bytes[*cursor];
                    *cursor += 1;
                    result.push(match escape {
                        'n' => '\n',
                        't' => '\t',
                        'r' => '\r',
                        'b' => '\u{8}',
                        'f' => '\u{c}',
                        'u' => {
                            let hex: String = bytes[*cursor..*cursor + 4].iter().collect();
                            *cursor += 4;
                            char::from_u32(u32::from_str_radix(&hex, 16).unwrap()).unwrap()
                        }
                        other => other,
                    });
                }
                other => result.push(other),
            }
        }
    }
}
