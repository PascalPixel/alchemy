//! decomp.dev's objdiff report from the same verified count as README's
//! progress line: twelve versions, one per game and edition, each split into
//! accounting units by part. No functions, addresses or game bytes.
//!
//! Wire fields from objdiff/report.proto, version 2, revision
//! 0c48d711c7bd51f791b353d7d85ba948b277e2f2. decomp.dev splits the combined
//! report into versions by its `version` and `version.category` ids.
use super::calcrom::{Counted, Game};
use std::path::Path;

/// The one report make land prepares and pre-push uploads.
pub(crate) const REPORT: &str = "out/reports/decomp/combined_report/report.pb";

const PARTS: [(&str, &str); 6] = [
    ("c", "C (includes FAKEMATCH)"),
    ("fakematch", "FAKEMATCH-steered C (cleanup remains)"),
    ("assembly", "Assembly (includes library and veneers)"),
    ("library", "Compiler-library assembly"),
    ("veneers", "8-byte veneers"),
    ("remaining", "Not yet C"),
];

/// Protobuf fields in field order; proto3 leaves zero scalars out.
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

/// Code bytes and units, matched or not; data stays unmeasured.
#[derive(Clone, Copy, Default)]
struct Measures {
    total: u64,
    matched: u64,
    units: u32,
    complete: u32,
}
impl Measures {
    fn add(&mut self, other: Measures) {
        self.total += other.total;
        self.matched += other.matched;
        self.units += other.units;
        self.complete += other.complete;
    }
    fn encode(&self) -> Vec<u8> {
        let percent = if self.total == 0 {
            0.0
        } else {
            self.matched as f32 / self.total as f32 * 100.0
        };
        let mut wire = Wire::default();
        wire.float(1, percent); // fuzzy_match_percent: no partial credit
        wire.uint(2, self.total);
        wire.uint(3, self.matched);
        wire.float(4, percent);
        wire.uint(11, self.matched); // complete_code
        wire.float(12, percent);
        wire.uint(15, self.units.into());
        wire.uint(16, self.complete.into());
        wire.0
    }
}

/// An edition's DONE as units: (part, bytes, categories, complete).
fn buckets(
    counted: &Counted,
) -> Result<[(&'static str, i64, &'static [&'static str], bool); 6], String> {
    let d = counted.done;
    let (c, assembly) = (d.common_c + d.game_c, d.common_asm + d.game_asm);
    if [c, assembly, d.veneers, counted.library, counted.steered]
        .iter()
        .any(|&b| b < 0)
        || counted.steered > c
        || counted.library + d.veneers > assembly
        || d.bytes() > d.executable
    {
        return Err("progress parts do not partition executable bytes".into());
    }
    Ok([
        ("C without FAKEMATCH", c - counted.steered, &["c"], true),
        (
            "FAKEMATCH-steered C",
            counted.steered,
            &["c", "fakematch"],
            true,
        ),
        (
            "Proven assembly excluding library and veneers",
            assembly - counted.library - d.veneers,
            &["assembly"],
            true,
        ),
        (
            "Compiler-library assembly",
            counted.library,
            &["assembly", "library"],
            true,
        ),
        ("8-byte veneers", d.veneers, &["assembly", "veneers"], true),
        (
            "Not yet credited",
            d.executable - d.bytes(),
            &["remaining"],
            false,
        ),
    ])
}

/// A version's name as decomp.dev shows it, such as `The Broken Seal 🇯🇵`.
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
            for (part, bytes, tags, complete) in buckets(counted)? {
                if bytes == 0 {
                    continue;
                }
                let measures = Measures {
                    total: bytes as u64,
                    matched: if complete { bytes as u64 } else { 0 },
                    units: 1,
                    complete: complete.into(),
                };
                whole.add(measures);
                let mut metadata = Wire::default();
                metadata.varint(1 << 3); // complete, present even when false
                metadata.varint(complete.into());
                metadata.bytes(5, name.as_bytes());
                for tag in tags.iter() {
                    let at = PARTS.iter().position(|(part, _)| part == tag).unwrap();
                    parts[at].add(measures);
                    metadata.bytes(5, format!("{name}.{tag}").as_bytes());
                }
                metadata.uint(6, 1); // auto_generated
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
    wire.uint(3, 2); // version
    for (id, name, measures) in categories {
        let mut category = Wire::default();
        category.bytes(1, id.as_bytes());
        category.bytes(2, name.as_bytes());
        category.bytes(3, &measures.encode());
        wire.bytes(4, &category.0);
    }
    Ok(wire.0)
}

/// Write the report when both games are measured; otherwise remove any
/// earlier one, so a landing never uploads a stale count.
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
