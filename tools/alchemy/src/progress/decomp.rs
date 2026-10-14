use super::calcrom::{Counted, Game};
use std::path::Path;

pub(crate) const REPORT: &str = "out/reports/decomp/combined_report/report.pb";

const PARTS: [(&str, &str); 6] = [
    ("c", "C"),
    ("assembly", "Assembly (includes library and veneers)"),
    ("library", "Compiler-library assembly"),
    ("veneers", "8-byte veneers"),
    ("remaining", "Not yet C"),
    ("data", "Data (DONE data: from games/ or fill, out of every non-code byte)"),
];

#[derive(Default)]
struct Wire(Vec<u8>);
impl Wire {
    fn varint(&mut self, mut value: u64) {
        while value >= 0x80 {
            self.0.push(value as u8 | 0x80);
            value >>= 7;
        }
        self.0.push(value as u8);
    }
    fn uint(&mut self, field: u64, value: u64) {
        if value != 0 {
            self.varint(field << 3);
            self.varint(value);
        }
    }
    fn float(&mut self, field: u64, value: f32) {
        if value != 0.0 {
            self.varint(field << 3 | 5);
            self.0.extend(value.to_le_bytes());
        }
    }
    fn bytes(&mut self, field: u64, value: &[u8]) {
        self.varint(field << 3 | 2);
        self.varint(value.len() as u64);
        self.0.extend(value);
    }
}

#[derive(Clone, Copy, Default)]
struct Measures {
    total: u64,
    matched: u64,
    total_data: u64,
    matched_data: u64,
    units: u32,
    complete: u32,
}
impl Measures {
    fn add(&mut self, other: Measures) {
        self.total += other.total;
        self.matched += other.matched;
        self.total_data += other.total_data;
        self.matched_data += other.matched_data;
        self.units += other.units;
        self.complete += other.complete;
    }
    /// objdiff's Measures: code in total_code, matched_code and
    /// complete_code; DONE data in total_data, matched_data and
    /// complete_data (C2).
    fn encode(&self) -> Vec<u8> {
        let share = |matched: u64, total: u64| {
            if total == 0 {
                0.0
            } else {
                matched as f32 / total as f32 * 100.0
            }
        };
        let percent = share(self.matched, self.total);
        let data = share(self.matched_data, self.total_data);
        let mut wire = Wire::default();
        wire.float(1, percent);
        wire.uint(2, self.total);
        wire.uint(3, self.matched);
        wire.float(4, percent);
        wire.uint(5, self.total_data);
        wire.uint(6, self.matched_data);
        wire.float(7, data);
        wire.uint(11, self.matched);
        wire.float(12, percent);
        wire.uint(13, self.matched_data);
        wire.float(14, data);
        wire.uint(15, self.units.into());
        wire.uint(16, self.complete.into());
        wire.0
    }
}

/// Each part of an edition: its name, code bytes, data bytes, the
/// categories it belongs to and whether it is done.
type Bucket = (&'static str, i64, i64, &'static [&'static str], bool);

fn buckets(counted: &Counted) -> Result<[Bucket; 7], String> {
    let d = counted.done;
    let (c, assembly) = (d.common_c + d.game_c, d.common_asm + d.game_asm);
    if [c, assembly, d.veneers, counted.library]
        .iter()
        .any(|&b| b < 0)
        || counted.library + d.veneers > assembly
        || d.bytes() > d.executable
        || d.data < 0
        || d.data > d.data_total
    {
        return Err("progress parts do not partition executable bytes".into());
    }
    Ok([
        ("C", c, 0, &["c"], true),
        (
            "Assembly excluding library and veneers",
            assembly - counted.library - d.veneers,
            0,
            &["assembly"],
            true,
        ),
        (
            "Compiler-library assembly",
            counted.library,
            0,
            &["assembly", "library"],
            true,
        ),
        ("8-byte veneers", d.veneers, 0, &["assembly", "veneers"], true),
        (
            "Not yet credited",
            d.executable - d.bytes(),
            0,
            &["remaining"],
            false,
        ),
        ("Data from source", 0, d.data, &["data"], true),
        (
            "Data not yet from source",
            0,
            d.data_total - d.data,
            &["data"],
            false,
        ),
    ])
}

fn version(id: &str, language: &str) -> String {
    let game = if id == "tbs" {
        "The Broken Seal"
    } else {
        "The Lost Age"
    };
    let flag = match language {
        "ja" => "🇯🇵",
        "en" => "🇺🇸",
        "de" => "🇩🇪",
        "es" => "🇪🇸",
        "fr" => "🇫🇷",
        _ => "🇮🇹",
    };
    format!("{game} {flag}")
}

fn report(games: [(&str, &Game); 2]) -> Result<Vec<u8>, String> {
    let mut total = Measures::default();
    let (mut units, mut categories) = (Wire::default(), Vec::new());
    for (id, game) in games {
        for (language, counted) in &game.editions {
            let name = version(id, language);
            let mut whole = Measures::default();
            let mut parts = [Measures::default(); PARTS.len()];
            for (part, bytes, data, tags, complete) in buckets(counted)? {
                if bytes == 0 && data == 0 {
                    continue;
                }
                let measures = Measures {
                    total: bytes as u64,
                    matched: if complete { bytes as u64 } else { 0 },
                    total_data: data as u64,
                    matched_data: if complete { data as u64 } else { 0 },
                    units: 1,
                    complete: complete.into(),
                };
                whole.add(measures);
                let mut metadata = Wire::default();
                metadata.varint(1 << 3);
                metadata.varint(complete.into());
                metadata.bytes(5, name.as_bytes());
                for tag in tags.iter() {
                    let at = PARTS.iter().position(|(part, _)| part == tag).unwrap();
                    parts[at].add(measures);
                    metadata.bytes(5, format!("{name}.{tag}").as_bytes());
                }
                metadata.uint(6, 1);
                let mut unit = Wire::default();
                unit.bytes(1, format!("{name}/{part}").as_bytes());
                unit.bytes(2, &measures.encode());
                unit.bytes(5, &metadata.0);
                units.bytes(2, &unit.0);
            }
            total.add(whole);
            categories.push((name.clone(), name.clone(), whole));
            for ((part, label), measures) in PARTS.iter().zip(parts) {
                categories.push((format!("{name}.{part}"), label.to_string(), measures));
            }
        }
    }
    let mut wire = Wire::default();
    wire.bytes(1, &total.encode());
    wire.0.extend(units.0);
    wire.uint(3, 2);
    for (id, name, measures) in categories {
        let mut category = Wire::default();
        category.bytes(1, id.as_bytes());
        category.bytes(2, name.as_bytes());
        category.bytes(3, &measures.encode());
        wire.bytes(4, &category.0);
    }
    Ok(wire.0)
}

pub(crate) fn write(
    root: &Path,
    games: [(&str, &Result<Game, String>); 2],
) -> Result<String, String> {
    let path = root.join(REPORT);
    let measured = games.map(|(id, game)| game.as_ref().ok().map(|game| (id, game)));
    let [Some(sun), Some(anchor)] = measured else {
        return match std::fs::remove_file(&path) {
            Err(error) if error.kind() != std::io::ErrorKind::NotFound => {
                Err(format!("{REPORT}: {error}"))
            }
            _ => Ok("decomp-report=pending".into()),
        };
    };
    let bytes = report([sun, anchor])?;
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| format!("{REPORT}: {e}"))?;
    std::fs::write(&path, bytes).map_err(|e| format!("{REPORT}: {e}"))?;
    Ok(format!("decomp-report={REPORT}"))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn data_reaches_decomp_dev_beside_code() {
        let mut counted = Counted::default();
        counted.done.game_c = 6;
        counted.done.executable = 10;
        counted.done.data = 7;
        counted.done.data_total = 9;
        let parts = buckets(&counted).unwrap();
        let code: i64 = parts.iter().map(|part| part.1).sum();
        let data: i64 = parts.iter().map(|part| part.2).sum();
        let done: i64 = parts.iter().filter(|part| part.4).map(|part| part.2).sum();
        assert_eq!((code, data, done), (10, 9, 7));
        let measures = Measures {
            total_data: 9,
            matched_data: 7,
            ..Measures::default()
        };
        // total_data is field 5 and matched_data field 6, as varints.
        let wire = measures.encode();
        assert!(wire.windows(2).any(|pair| pair == [5 << 3, 9]));
        assert!(wire.windows(2).any(|pair| pair == [6 << 3, 7]));
        counted.done.data = 10;
        assert!(buckets(&counted).is_err());
    }
}
