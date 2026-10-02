//! decomp.dev's objdiff protobuf reports, from calcrom's verified six-edition counts.
use super::calcrom::{measure_game, Counted, Game, ObjectKey};
use prost::Message;
use std::collections::BTreeMap;
use std::io::Write;
use std::path::Path;

const EDITIONS: [(&str, &str); 6] = [
    ("ja", "Japanese"),
    ("en", "English"),
    ("de", "German"),
    ("es", "Spanish"),
    ("fr", "French"),
    ("it", "Italian"),
];
const PARTS: [(&str, &str); 7] = [
    ("c", "C (includes FAKEMATCH)"),
    ("fakematch", "FAKEMATCH-steered C (cleanup remains)"),
    ("assembly", "Assembly (includes library and veneers)"),
    ("library", "Compiler-library assembly"),
    ("veneers", "8-byte veneers"),
    ("remaining", "Not yet C"),
    (
        "initialized-data",
        "Initialized data (English byte weights)",
    ),
];

// Wire fields from objdiff/report.proto, version 2, revision 0c48d711c7bd51f791b353d7d85ba948b277e2f2:
// https://github.com/encounter/objdiff/blob/0c48d711c7bd51f791b353d7d85ba948b277e2f2/objdiff-core/protos/report.proto
// Unmeasured functions and addresses are omitted, retaining their protobuf defaults.
#[derive(Clone, PartialEq, Message)]
struct Report {
    #[prost(message, optional, tag = "1")]
    measures: Option<Measures>,
    #[prost(message, repeated, tag = "2")]
    units: Vec<Unit>,
    #[prost(uint32, tag = "3")]
    version: u32,
    #[prost(message, repeated, tag = "4")]
    categories: Vec<Category>,
}

#[derive(Clone, Copy, PartialEq, Message)]
struct Measures {
    #[prost(float, tag = "1")]
    fuzzy_match_percent: f32,
    #[prost(uint64, tag = "2")]
    total_code: u64,
    #[prost(uint64, tag = "3")]
    matched_code: u64,
    #[prost(float, tag = "4")]
    matched_code_percent: f32,
    #[prost(uint64, tag = "5")]
    total_data: u64,
    #[prost(uint64, tag = "6")]
    matched_data: u64,
    #[prost(float, tag = "7")]
    matched_data_percent: f32,
    #[prost(uint64, tag = "11")]
    complete_code: u64,
    #[prost(float, tag = "12")]
    complete_code_percent: f32,
    #[prost(uint64, tag = "13")]
    complete_data: u64,
    #[prost(float, tag = "14")]
    complete_data_percent: f32,
    #[prost(uint32, tag = "15")]
    total_units: u32,
    #[prost(uint32, tag = "16")]
    complete_units: u32,
}

#[derive(Clone, PartialEq, Message)]
struct Unit {
    #[prost(string, tag = "1")]
    name: String,
    #[prost(message, optional, tag = "2")]
    measures: Option<Measures>,
    #[prost(message, optional, tag = "5")]
    metadata: Option<Metadata>,
}

#[derive(Clone, PartialEq, Message)]
struct Metadata {
    #[prost(bool, optional, tag = "1")]
    complete: Option<bool>,
    #[prost(string, optional, tag = "4")]
    source_path: Option<String>,
    #[prost(string, repeated, tag = "5")]
    progress_categories: Vec<String>,
    #[prost(bool, optional, tag = "6")]
    auto_generated: Option<bool>,
}

#[derive(Clone, PartialEq, Message)]
struct Category {
    #[prost(string, tag = "1")]
    id: String,
    #[prost(string, tag = "2")]
    name: String,
    #[prost(message, optional, tag = "3")]
    measures: Option<Measures>,
}

impl Measures {
    fn sum<'a>(measures: impl Iterator<Item = &'a Measures>) -> Self {
        let mut sum = Self::default();
        for measure in measures {
            sum.total_code += measure.total_code;
            sum.matched_code += measure.matched_code;
            sum.complete_code += measure.complete_code;
            sum.total_data += measure.total_data;
            sum.matched_data += measure.matched_data;
            sum.complete_data += measure.complete_data;
            sum.total_units += measure.total_units;
            sum.complete_units += measure.complete_units;
        }
        let percent = if sum.total_code == 0 {
            0.0
        } else {
            sum.matched_code as f32 / sum.total_code as f32 * 100.0
        };
        sum.matched_code_percent = percent;
        sum.complete_code_percent = percent;
        // objdiff weights fuzzy by code bytes only; Alchemy grants no partial credit.
        sum.fuzzy_match_percent = percent;
        let data_percent = if sum.total_data == 0 {
            // No data denominator means unmeasured, so omit the wire fields.
            0.0
        } else {
            sum.matched_data as f32 / sum.total_data as f32 * 100.0
        };
        sum.matched_data_percent = data_percent;
        sum.complete_data_percent = data_percent;
        sum
    }
}

fn units(
    language: &str,
    name: &str,
    source_path: Option<&str>,
    counted: Counted,
) -> Result<Vec<Unit>, String> {
    let d = counted.done;
    if [
        d.common_c,
        d.game_c,
        d.common_asm,
        d.game_asm,
        d.executable,
        d.veneers,
        counted.library,
        counted.steered,
        counted.uncredited,
    ]
    .iter()
    .any(|&bytes| bytes < 0)
    {
        return Err(format!("{language}: negative progress bytes"));
    }
    let c = d.common_c + d.game_c;
    let assembly = d.common_asm + d.game_asm;
    if counted.steered > c || counted.library + d.veneers > assembly || d.bytes() > d.executable {
        return Err(format!(
            "{language}: progress parts do not partition executable bytes"
        ));
    }
    let buckets: [(&str, i64, &[&str], bool); 6] = [
        ("C without FAKEMATCH", c - counted.steered, &["c"], true),
        (
            "FAKEMATCH-steered C (cleanup remains)",
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
            "Not yet credited (includes uncredited padding)",
            d.executable - d.bytes(),
            &["remaining"],
            false,
        ),
    ];
    let mut units = buckets
        .into_iter()
        .filter(|(_, code, _, _)| *code > 0)
        .map(|(name, code, categories, complete)| {
            accounting_unit(language, name, code as u64, 0, categories, complete)
        })
        .collect::<Vec<_>>();
    let divided = units.len() > 1;
    for unit in &mut units {
        unit.name = if divided {
            format!("{name}/{}", unit.name)
        } else {
            name.to_string()
        };
        unit.metadata.as_mut().unwrap().source_path = source_path.map(str::to_string);
    }
    Ok(units)
}

fn accounting_unit(
    language: &str,
    name: &str,
    code: u64,
    data: u64,
    categories: &[&str],
    complete: bool,
) -> Unit {
    let code_percent = if code > 0 && complete { 100.0 } else { 0.0 };
    let data_percent = if data > 0 && complete { 100.0 } else { 0.0 };
    let matched_code = if complete { code } else { 0 };
    let matched_data = if complete { data } else { 0 };
    Unit {
        name: name.to_string(),
        measures: Some(Measures {
            total_code: code,
            matched_code,
            complete_code: matched_code,
            matched_code_percent: code_percent,
            complete_code_percent: code_percent,
            fuzzy_match_percent: code_percent,
            total_data: data,
            matched_data,
            complete_data: matched_data,
            matched_data_percent: data_percent,
            complete_data_percent: data_percent,
            total_units: 1,
            complete_units: u32::from(complete),
        }),
        metadata: Some(Metadata {
            complete: Some(complete),
            source_path: None,
            progress_categories: std::iter::once(language)
                .chain(categories.iter().copied())
                .map(str::to_string)
                .collect(),
            auto_generated: Some(true),
        }),
    }
}

/// Whole linked source sections earn English weights, including differently sized
/// localized variants. This measures coverage of the English section inventory,
/// not each edition's native byte total.
fn data_credits(language: &str, game: &Game) -> Result<BTreeMap<ObjectKey, i64>, String> {
    let linked = game
        .edition_data
        .get(language)
        .ok_or_else(|| format!("{language}: missing verified data sections"))?;
    if linked.values().any(|&bytes| bytes <= 0) {
        return Err(format!("{language}: invalid linked data section size"));
    }
    let mut inventory = BTreeMap::<ObjectKey, i64>::new();
    let mut credits = BTreeMap::<ObjectKey, i64>::new();
    for (key @ (image, object, _), &bytes) in &game.english.data_sections {
        if bytes <= 0 {
            return Err("English data inventory contains an invalid section size".into());
        }
        let object = (image.clone(), object.clone());
        *inventory.entry(object.clone()).or_default() += bytes;
        if linked.contains_key(key) {
            *credits.entry(object).or_default() += bytes;
        }
    }
    for (key, bytes) in &game.english.objects {
        if inventory.remove(key).unwrap_or_default() != bytes.data_source {
            return Err("English data sections disagree with their placed source objects".into());
        }
    }
    if !inventory.is_empty() {
        return Err("English data section is missing its placed object".into());
    }
    if language == "en" && linked != &game.english.data_sections {
        return Err("English edition data disagrees with its reference inventory".into());
    }
    Ok(credits)
}

fn object_units(language: &str, game: &Game, expected: Counted) -> Result<Vec<Unit>, String> {
    let linked = game
        .edition_credits
        .get(language)
        .ok_or_else(|| format!("{language}: missing verified edition credits"))?;
    let data = data_credits(language, game)?;
    let mut objects = BTreeMap::<ObjectKey, Counted>::new();
    let mut total = Counted::default();
    for ((image, object, function), english) in &game.english.credits {
        let Some(local) = linked.get(&(image.clone(), object.clone(), function.clone())) else {
            continue;
        };
        let mut credit = *english;
        credit.steered = if local.steered > 0 {
            english.done.common_c + english.done.game_c
        } else {
            0
        };
        *objects.entry((image.clone(), object.clone())).or_default() += credit;
        total += credit;
    }
    let deduction = game.english.stray.min(total.done.game_asm);
    total.done.game_asm -= deduction;
    total.uncredited += game.english.stray;
    total.done.executable = game.english.done.executable;
    if total != expected {
        return Err(format!(
            "{language}: object credits disagree with canonical calcrom counts"
        ));
    }
    if objects
        .keys()
        .any(|key| !game.english.objects.contains_key(key))
    {
        return Err(format!(
            "{language}: credited object missing from English map inventory"
        ));
    }
    let mut found = Vec::new();
    let mut pooled = Counted::default();
    for ((image, object), bytes) in &game.english.objects {
        let name = format!("{image}/{object}");
        let mut counted = objects
            .get(&(image.clone(), object.clone()))
            .copied()
            .unwrap_or_default();
        counted.done.executable = bytes.executable;
        if deduction > 0 && counted.done.game_asm > 0 {
            // A global deduction has no owning source object. Pool ordinary
            // game assembly instead of assigning the loss to an arbitrary file.
            let ordinary = counted.done.game_asm - counted.library - counted.done.veneers;
            if ordinary < 0 {
                return Err(format!("{name}: invalid assembly subsets"));
            }
            pooled.done.game_asm += ordinary;
            pooled.done.executable += ordinary;
            counted.done.game_asm -= ordinary;
            counted.done.executable -= ordinary;
        }
        found.extend(units(
            language,
            &name,
            bytes.source_path.as_deref(),
            counted,
        )?);
        let source = data
            .get(&(image.clone(), object.clone()))
            .copied()
            .unwrap_or_default();
        if bytes.data_source < 0 || bytes.data_scaffold < 0 || source > bytes.data_source {
            return Err(format!("{name}: invalid initialized data bytes"));
        }
        for (part, data, complete) in [
            (
                "Initialized data from source (English byte weights)",
                source,
                true,
            ),
            (
                "Initialized data pending source (English byte weights)",
                bytes.data_source + bytes.data_scaffold - source,
                false,
            ),
        ] {
            if data == 0 {
                continue;
            }
            let mut unit = accounting_unit(
                language,
                &format!("{name}/{part}"),
                0,
                data as u64,
                &["initialized-data"],
                complete,
            );
            unit.metadata
                .as_mut()
                .unwrap()
                .source_path
                .clone_from(&bytes.source_path);
            found.push(unit);
        }
    }
    if deduction > 0 {
        if deduction > pooled.done.game_asm {
            return Err(format!(
                "{language}: global uncredited padding exceeds attributable ordinary assembly"
            ));
        }
        pooled.done.game_asm -= deduction;
        pooled.uncredited = deduction;
        found.extend(units(
            language,
            "Accounting residual/Game assembly with global uncredited padding",
            None,
            pooled,
        )?);
    }
    Ok(found)
}

fn report(game: &Game) -> Result<Report, String> {
    if game.english.done.executable <= 0
        || game.english.stray < 0
        || game.editions.len() != EDITIONS.len()
        || game
            .editions
            .iter()
            .zip(EDITIONS)
            .any(|((language, counted), (expected, _))| {
                *language != expected || counted.done.executable != game.english.done.executable
            })
    {
        return Err(
            "report requires six editions in Japanese-first order with English byte weights".into(),
        );
    }
    let inventory = game.english.objects.values();
    let (code, source, scaffold) = inventory.fold((0, 0, 0), |(code, source, scaffold), object| {
        (
            code + object.executable,
            source + object.data_source,
            scaffold + object.data_scaffold,
        )
    });
    if (code, source, scaffold)
        != (
            game.english.done.executable,
            game.english.data_source,
            game.english.data_scaffold,
        )
    {
        return Err("English object inventory disagrees with canonical code or data counts".into());
    }
    let units = game
        .editions
        .iter()
        .map(|(language, counted)| object_units(language, game, *counted))
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .flatten()
        .collect::<Vec<_>>();
    let report = summarize(units);
    let measures = report.measures.unwrap();
    let counted = game.combined();
    let data_source = EDITIONS
        .iter()
        .map(|(language, _)| {
            data_credits(language, game).map(|objects| objects.values().sum::<i64>())
        })
        .collect::<Result<Vec<_>, _>>()?
        .iter()
        .sum::<i64>();
    if measures.total_code != counted.done.executable as u64
        || measures.matched_code != counted.done.bytes() as u64
        || measures.total_data
            != ((game.english.data_source + game.english.data_scaffold) * EDITIONS.len() as i64)
                as u64
        || measures.matched_data != data_source as u64
    {
        return Err(
            "report units disagree with calcrom code or English-weighted data counts".into(),
        );
    }
    Ok(report)
}

fn summarize(units: Vec<Unit>) -> Report {
    let measures = Measures::sum(units.iter().filter_map(|unit| unit.measures.as_ref()));
    let categories = EDITIONS
        .into_iter()
        .chain(PARTS)
        .filter(|(id, _)| {
            PARTS.iter().any(|(part, _)| id == part)
                || units.iter().any(|unit| in_category(unit, id))
        })
        .map(|(id, name)| Category {
            id: id.to_string(),
            name: name.to_string(),
            measures: Some(Measures::sum(units.iter().filter_map(|unit| {
                in_category(unit, id)
                    .then_some(unit.measures.as_ref())
                    .flatten()
            }))),
        })
        .collect();
    Report {
        measures: Some(measures),
        units,
        version: 2,
        categories,
    }
}

fn in_category(unit: &Unit, id: &str) -> bool {
    unit.metadata.as_ref().is_some_and(|metadata| {
        metadata
            .progress_categories
            .iter()
            .any(|category| category == id)
    })
}

fn versions(id: &str, game: &Game) -> Result<Vec<(String, Report)>, String> {
    let aggregate = report(game)?;
    let mut reports = Vec::new();
    for (language, _) in EDITIONS {
        let units = aggregate
            .units
            .iter()
            .filter(|unit| in_category(unit, language))
            .cloned()
            .collect::<Vec<_>>();
        reports.push((format!("{id}-{language}"), summarize(units)));
    }
    Ok(reports)
}

fn version_name(id: &str) -> Result<String, String> {
    let (game, language) = id
        .split_once('-')
        .ok_or_else(|| format!("unknown report edition {id}"))?;
    let game = match game {
        "tbs" => "The Broken Seal",
        "tla" => "The Lost Age",
        _ => return Err(format!("unknown report game {game}")),
    };
    let flag = match language {
        "ja" => "🇯🇵",
        "en" => "🇺🇸",
        "de" => "🇩🇪",
        "es" => "🇪🇸",
        "fr" => "🇫🇷",
        "it" => "🇮🇹",
        _ => return Err(format!("unknown report language {language}")),
    };
    Ok(format!("{game} {flag}"))
}

// decomp.dev splits combined_report using objdiff's version/version.category
// convention. Version IDs become the native menu labels, including spaces/flags.
// https://github.com/encounter/decomp.dev/blob/e9c086adb74d2fe569541cd715cd9312d7641313/crates/github/src/lib.rs
fn combine(reports: &[(String, Report)]) -> Result<Report, String> {
    let mut combined = Report {
        measures: Some(Measures::sum(
            reports
                .iter()
                .filter_map(|(_, report)| report.measures.as_ref()),
        )),
        units: Vec::new(),
        version: 2,
        categories: Vec::new(),
    };
    for (id, report) in reports {
        let name = version_name(id)?;
        if combined
            .categories
            .iter()
            .any(|category| category.id == name)
        {
            return Err(format!("duplicate report edition {id}"));
        }
        combined.categories.push(Category {
            id: name.clone(),
            name: name.clone(),
            measures: report.measures,
        });
        for category in &report.categories {
            let mut category = category.clone();
            category.id = format!("{name}.{}", category.id);
            combined.categories.push(category);
        }
        for unit in &report.units {
            let mut unit = unit.clone();
            unit.name = format!("{name}/{}", unit.name);
            let metadata = unit
                .metadata
                .as_mut()
                .ok_or("report unit is missing metadata")?;
            metadata.progress_categories = std::iter::once(name.clone())
                .chain(
                    metadata
                        .progress_categories
                        .iter()
                        .map(|id| format!("{name}.{id}")),
                )
                .collect();
            combined.units.push(unit);
        }
    }
    Ok(combined)
}

fn publish(root: &Path, games: [(&str, Result<Game, String>); 2]) -> Result<String, String> {
    // A pending second game must not leave a newly published first game.
    let reports = games
        .into_iter()
        .map(|(id, game)| {
            let game = game.map_err(|reason| format!("{id}: {reason}"))?;
            versions(id, &game).map_err(|reason| format!("{id}: {reason}"))
        })
        .collect::<Result<Vec<_>, String>>()?
        .into_iter()
        .flatten()
        .collect::<Vec<_>>();
    let combined = combine(&reports)?;
    let mut paths = Vec::new();
    for (id, report) in reports
        .into_iter()
        .chain(std::iter::once(("combined".into(), combined)))
    {
        let bytes = report.encode_to_vec();
        let path = root.join(format!("out/reports/decomp/{id}_report/report.pb"));
        let parent = path.parent().unwrap();
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
        let mut file =
            tempfile::NamedTempFile::new_in(parent).map_err(|error| error.to_string())?;
        file.write_all(&bytes).map_err(|error| error.to_string())?;
        file.persist(&path).map_err(|error| error.to_string())?;
        paths.push(path.strip_prefix(root).unwrap().display().to_string());
    }
    // Earlier exports owned these two files. Remove only those obsolete reports,
    // after every current edition has validated and published successfully.
    for id in ["tbs", "tla"] {
        let path = root.join(format!("out/reports/decomp/{id}_report/report.pb"));
        match std::fs::remove_file(&path) {
            Ok(()) => {}
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
            Err(error) => return Err(format!("{}: {error}", path.display())),
        }
    }
    Ok(format!(
        "decomp-reports={} metric=DONE-six-editions data=English-weighted-initialized-sections",
        paths.join(",")
    ))
}

pub(crate) fn write(root: &Path) -> Result<String, String> {
    // The build owns source freshness; image hashes alone cannot prove that
    // today's source and its FAKEMATCH tags produced a previously valid image.
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let status = std::process::Command::new("make")
        .arg("compare-editions")
        .arg(format!("ALCHEMY={}", executable.display()))
        .current_dir(root)
        .status()
        .map_err(|error| format!("cannot compare editions before export: {error}"))?;
    if !status.success() {
        return Err("all twelve editions must build and compare before report export".into());
    }
    write_verified(root)
}

/// Only the exporter and the completed landing gate may call this after comparison.
pub(crate) fn write_verified(root: &Path) -> Result<String, String> {
    let sun = measure_game(root, crate::targets::decomp_target(Some("tbs-en"))?)?;
    let anchor = measure_game(root, crate::targets::decomp_target(Some("tla-en"))?)?;
    publish(root, [("tbs", sun), ("tla", anchor)])
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::coverage::calcrom::{Measurement, ObjectBytes};
    use crate::coverage::progress::GameDone;

    fn game() -> Game {
        let common = "games/COMMON/SRC/COMMON.o";
        let source = "games/G/SRC/A.o";
        let mut english = Measurement {
            raw: 200,
            listings: 142,
            data_source: 600,
            data_scaffold: 400,
            uncredited: 8,
            ..Measurement::default()
        };
        for (object, function, bytes, steered) in [
            (common, "Common", 50, 0),
            (source, "Steered", 100, 100),
            (source, "Plain", 200, 0),
            (source, "English", 200, 0),
        ] {
            let mut credit = Counted {
                steered,
                ..Counted::default()
            };
            if object == common {
                credit.done.common_c = bytes
            } else {
                credit.done.game_c = bytes
            }
            english.credits.insert(
                ("main".into(), object.into(), Some(function.into())),
                credit,
            );
        }
        for (object, common_asm, game_asm, library, veneers, uncredited) in [
            ("games/G/SRC/ASM.o", 0, 56, 0, 0, 8),
            ("libgcc.a(member.o)", 0, 24, 24, 0, 0),
            ("games/COMMON/SRC/ASM.o", 4, 0, 0, 0, 0),
            ("games/COMMON/SRC/VENEERS.o", 16, 0, 0, 16, 0),
        ] {
            english.credits.insert(
                ("main".into(), object.into(), None),
                Counted {
                    done: GameDone {
                        common_asm,
                        game_asm,
                        veneers,
                        ..GameDone::default()
                    },
                    library,
                    uncredited,
                    ..Counted::default()
                },
            );
        }
        for ((image, object, _), credit) in &english.credits {
            english.done += credit.done;
            english.library += credit.library;
            english.steered += credit.steered;
            let bytes = english
                .objects
                .entry((image.clone(), object.clone()))
                .or_default();
            bytes.executable += credit.done.bytes() + credit.uncredited;
        }
        english
            .objects
            .get_mut(&("main".into(), source.into()))
            .unwrap()
            .data_source = 600;
        english
            .objects
            .get_mut(&("main".into(), source.into()))
            .unwrap()
            .source_path = Some("games/G/SRC/A.C".into());
        english.objects.insert(
            ("main".into(), "recon/g/raw/pending.o".into()),
            ObjectBytes {
                executable: 200,
                data_scaffold: 400,
                ..ObjectBytes::default()
            },
        );
        english.objects.insert(
            ("36f".into(), "resource_36f_overlay.o".into()),
            ObjectBytes {
                executable: 142,
                ..ObjectBytes::default()
            },
        );
        english.done.executable = 1000;
        english.data_sections = [
            (("main".into(), source.into(), ".rodata".into()), 400),
            (("main".into(), source.into(), ".data".into()), 200),
        ]
        .into_iter()
        .collect();
        let edition_data = EDITIONS
            .into_iter()
            .map(|(language, _)| {
                let mut linked = english.data_sections.clone();
                if language != "en" {
                    // Linking one source section does not earn another from that object.
                    linked.remove(&("main".into(), source.into(), ".data".into()));
                    *linked
                        .get_mut(&("main".into(), source.into(), ".rodata".into()))
                        .unwrap() = 480;
                }
                (language, linked)
            })
            .collect();
        let edition_credits = EDITIONS
            .into_iter()
            .map(|(language, _)| {
                let mut linked = english.credits.clone();
                if language != "en" {
                    linked.remove(&("main".into(), source.into(), Some("English".into())));
                    // Different native sizes still earn the original English weights.
                    linked
                        .get_mut(&("main".into(), source.into(), Some("Plain".into())))
                        .unwrap()
                        .done
                        .game_c = 212;
                }
                if language == "ja" {
                    linked
                        .get_mut(&("main".into(), source.into(), Some("Steered".into())))
                        .unwrap()
                        .steered = 0;
                    linked
                        .get_mut(&("main".into(), common.into(), Some("Common".into())))
                        .unwrap()
                        .steered = 50;
                }
                (language, linked)
            })
            .collect();
        Game {
            english,
            edition_credits,
            edition_data,
            editions: EDITIONS
                .into_iter()
                .map(|(language, _)| {
                    let c = if language == "en" { 500 } else { 300 };
                    (
                        language,
                        Counted {
                            done: GameDone {
                                common_c: 50,
                                game_c: c,
                                common_asm: 20,
                                game_asm: 80,
                                executable: 1000,
                                veneers: 16,
                            },
                            library: 24,
                            steered: if language == "ja" { 50 } else { 100 },
                            uncredited: 8,
                        },
                    )
                })
                .collect(),
        }
    }

    #[test]
    fn report_partitions_six_editions_and_discloses_overlapping_subsets() {
        let game = game();
        let report = report(&game).unwrap();
        let measures = report.measures.unwrap();
        let done = game.combined();
        assert_eq!((measures.total_code, measures.matched_code), (6000, 2900));
        assert_eq!(measures.matched_code, done.done.bytes() as u64);
        assert_eq!(measures.complete_code, measures.matched_code);
        assert_eq!(measures.fuzzy_match_percent, measures.matched_code_percent);
        assert_eq!(
            measures.complete_code_percent,
            measures.matched_code_percent
        );
        assert!(measures.total_units > 36);
        assert_eq!((measures.matched_data, measures.total_data), (2600, 6000));
        assert_eq!(measures.complete_data, measures.matched_data);
        assert_eq!(
            measures.complete_data_percent,
            measures.matched_data_percent
        );
        let category = |id| {
            report
                .categories
                .iter()
                .find(|c| c.id == id)
                .unwrap()
                .measures
                .unwrap()
        };
        assert_eq!(category("en").matched_code, 650);
        assert_eq!(category("ja").matched_code, 450);
        assert_eq!(
            (category("ja").matched_data, category("ja").total_data),
            (400, 1000)
        );
        assert_eq!(
            (category("en").matched_data, category("en").total_data),
            (600, 1000)
        );
        assert_eq!(category("c").matched_code, 2300);
        assert_eq!(category("fakematch").matched_code, done.steered as u64);
        assert_eq!(category("fakematch").matched_code, 550);
        assert_eq!(category("assembly").matched_code, 600);
        assert_eq!(category("library").matched_code, 144);
        assert_eq!(category("veneers").matched_code, 96);
        assert_eq!(category("remaining").total_code, 3100);
        assert_eq!(category("remaining").matched_code, 0);
        assert_eq!(category("c").total_data, 0);
        assert_eq!(category("initialized-data").total_data, measures.total_data);
        assert_eq!(
            category("initialized-data").matched_data,
            measures.matched_data
        );
        assert_eq!(category("initialized-data").total_code, 0);
        assert_eq!(category("initialized-data").matched_code_percent, 0.0);
        assert_eq!(
            Report::decode(report.encode_to_vec().as_slice()).unwrap(),
            report
        );
    }

    #[test]
    fn rejects_incomplete_editions_and_impossible_subsets() {
        let mut missing = game();
        missing.editions.pop();
        assert!(report(&missing).is_err());
        let mut wrong_weight = game();
        wrong_weight.editions[0].1.done.executable = 1200;
        assert!(report(&wrong_weight).is_err());
        let mut duplicate = game();
        duplicate.editions[0].0 = "en";
        assert!(report(&duplicate).is_err());
        for bad in [
            Counted {
                steered: 9999,
                ..game().editions[0].1
            },
            Counted {
                library: 9999,
                ..game().editions[0].1
            },
            Counted {
                steered: -1,
                ..game().editions[0].1
            },
        ] {
            assert!(units("ja", "fixture.o", None, bad).is_err());
        }
        for (source, scaffold) in [(-1, 400), (600, -1)] {
            let mut invalid = game();
            invalid.english.data_source = source;
            invalid.english.data_scaffold = scaffold;
            assert!(versions("tbs", &invalid).is_err());
        }
    }

    #[test]
    fn all_editions_report_their_own_source_coverage_of_the_english_data_inventory() {
        let game = game();
        let versions = versions("tbs", &game).unwrap();
        assert_eq!(
            versions
                .iter()
                .map(|(id, _)| id.as_str())
                .collect::<Vec<_>>(),
            ["tbs-ja", "tbs-en", "tbs-de", "tbs-es", "tbs-fr", "tbs-it"]
        );
        let edition_sum = Measures::sum(
            versions
                .iter()
                .map(|(_, report)| report.measures.as_ref().unwrap()),
        );
        let aggregate = report(&game).unwrap().measures.unwrap();
        assert_eq!(edition_sum.total_code, aggregate.total_code);
        assert_eq!(edition_sum.matched_code, aggregate.matched_code);
        assert_eq!(edition_sum.complete_code, aggregate.complete_code);
        assert_eq!(
            edition_sum.fuzzy_match_percent,
            aggregate.fuzzy_match_percent
        );
        for ((_, report), (language, _)) in versions.iter().zip(EDITIONS) {
            assert!(report.units.iter().all(|unit| in_category(unit, language)));
            assert_eq!(
                report
                    .categories
                    .iter()
                    .filter(|category| EDITIONS.iter().any(|(id, _)| *id == category.id))
                    .count(),
                1
            );
        }
        for (id, report) in &versions {
            assert_eq!(
                report.measures.unwrap(),
                Measures::sum(
                    report
                        .units
                        .iter()
                        .map(|unit| unit.measures.as_ref().unwrap())
                )
            );
            let measures = report.measures.unwrap();
            assert_eq!(measures.total_data, 1000);
            assert_eq!(
                measures.matched_data,
                if id == "tbs-en" { 600 } else { 400 }
            );
            assert_eq!(measures.complete_data, measures.matched_data);
            assert_eq!(
                measures.complete_data_percent,
                measures.matched_data_percent
            );
            for unit in &report.units {
                if unit.measures.unwrap().total_code > 0 {
                    assert_no_data(unit.measures.unwrap());
                } else {
                    assert!(in_category(unit, "initialized-data"));
                    assert_eq!(unit.measures.unwrap().matched_code_percent, 0.0);
                }
            }
            for category in &report.categories {
                if PARTS.iter().any(|(part, _)| *part == category.id)
                    && category.id != "initialized-data"
                {
                    assert_no_data(category.measures.unwrap());
                }
            }
        }
        let english = versions[1].1.measures.unwrap();
        assert_eq!(english.total_code, 1000);
        assert_eq!(english.matched_data, game.english.data_source as u64);
        assert_eq!(
            english.total_data,
            (game.english.data_source + game.english.data_scaffold) as u64
        );
        assert_eq!(english.complete_data, english.matched_data);
        assert!((english.matched_data_percent - 60.0).abs() < 0.00001);
        assert_eq!(english.complete_data_percent, english.matched_data_percent);
        assert_eq!(english.fuzzy_match_percent, english.matched_code_percent);
    }

    #[test]
    fn data_credit_requires_the_same_linked_image_and_input_section() {
        let mut game = game();
        let source = "games/G/SRC/A.o";
        let japanese = game.edition_data.get_mut("ja").unwrap();
        japanese.clear();
        japanese.insert(("main".into(), source.into(), ".rodata.other".into()), 400);
        japanese.insert(("36f".into(), source.into(), ".rodata".into()), 400);
        japanese.insert(
            ("main".into(), "games/G/SRC/JA.o".into(), ".data".into()),
            900,
        );
        let report = &versions("tbs", &game).unwrap()[0].1;
        assert_eq!(report.measures.unwrap().total_data, 1000);
        assert_eq!(report.measures.unwrap().matched_data, 0);
        game.edition_data.remove("ja");
        assert!(versions("tbs", &game)
            .unwrap_err()
            .contains("missing verified data sections"));
    }

    #[test]
    fn data_inventory_must_reconcile_with_source_objects_and_english_edition() {
        let mut missing = game();
        missing.english.data_sections.clear();
        assert!(versions("tbs", &missing).is_err());
        let mut invalid = game();
        for bytes in invalid.edition_data.get_mut("ja").unwrap().values_mut() {
            *bytes = 0;
        }
        assert!(versions("tbs", &invalid).is_err());
        let mut wrong_english = game();
        wrong_english.edition_data.get_mut("en").unwrap().clear();
        assert!(versions("tbs", &wrong_english).is_err());
    }

    fn assert_no_data(measures: Measures) {
        assert_eq!(measures.total_data, 0);
        assert_eq!(measures.matched_data, 0);
        assert_eq!(measures.complete_data, 0);
        assert_eq!(measures.matched_data_percent, 0.0);
        assert_eq!(measures.complete_data_percent, 0.0);
    }

    #[test]
    fn objects_keep_english_sizes_local_tags_and_pending_extents() {
        let reports = versions("tbs", &game()).unwrap();
        let japanese = &reports[0].1;
        let english = &reports[1].1;
        let object = |report: &Report, path: &str| {
            Measures::sum(
                report
                    .units
                    .iter()
                    .filter(|unit| unit.name.starts_with(path))
                    .map(|unit| unit.measures.as_ref().unwrap()),
            )
        };
        for report in [japanese, english] {
            let raw = object(report, "main/recon/g/raw/pending.o");
            assert_eq!((raw.total_code, raw.matched_code), (200, 0));
            let listing = object(report, "36f/resource_36f_overlay.o");
            assert_eq!((listing.total_code, listing.matched_code), (142, 0));
            let assembly = object(report, "main/games/G/SRC/ASM.o");
            assert_eq!((assembly.total_code, assembly.matched_code), (64, 56));
            assert_eq!(object(report, "main/libgcc.a(member.o)").matched_code, 24);
            assert_eq!(
                object(report, "main/games/COMMON/SRC/VENEERS.o").matched_code,
                16
            );
        }
        let localized = object(japanese, "main/games/G/SRC/A.o");
        let source = object(english, "main/games/G/SRC/A.o");
        assert_eq!((localized.total_code, localized.matched_code), (500, 300));
        assert_eq!(
            (source.total_code, source.matched_code, source.matched_data),
            (500, 500, 600)
        );
        assert!(!japanese
            .units
            .iter()
            .any(|unit| unit.name.starts_with("main/games/G/SRC/A.o")
                && in_category(unit, "fakematch")));
        assert_eq!(
            english
                .units
                .iter()
                .filter(|unit| unit.name.starts_with("main/games/G/SRC/A.o")
                    && in_category(unit, "fakematch"))
                .map(|unit| unit.measures.unwrap().matched_code)
                .sum::<u64>(),
            100
        );
        for unit in english
            .units
            .iter()
            .filter(|unit| unit.name.starts_with("main/games/G/SRC/A.o"))
        {
            assert_eq!(
                unit.metadata.as_ref().unwrap().source_path.as_deref(),
                Some("games/G/SRC/A.C")
            );
        }
    }

    #[test]
    fn stray_padding_stays_in_an_explicit_pool_without_debiting_an_arbitrary_file() {
        let mut game = game();
        let adjust = |game: &mut Game, bytes| {
            game.english.stray += bytes;
            game.english.uncredited += bytes;
            game.english.done.game_asm -= bytes;
            for (_, edition) in &mut game.editions {
                edition.done.game_asm -= bytes;
                edition.uncredited += bytes;
            }
        };
        adjust(&mut game, 12);
        for (_, report) in versions("tla", &game).unwrap() {
            let pool = report
                .units
                .iter()
                .filter(|unit| unit.name.starts_with("Accounting residual/"));
            let pooled = Measures::sum(pool.clone().map(|unit| unit.measures.as_ref().unwrap()));
            assert_eq!((pooled.total_code, pooled.matched_code), (56, 44));
            assert!(pool
                .clone()
                .all(|unit| unit.metadata.as_ref().unwrap().source_path.is_none()));
            assert!(report
                .units
                .iter()
                .filter(|unit| unit.name.starts_with("main/games/G/SRC/ASM.o"))
                .all(|unit| unit.measures.unwrap().matched_code == 0));
            for (category, bytes) in [("assembly", 88), ("library", 24), ("veneers", 16)] {
                assert_eq!(
                    report
                        .categories
                        .iter()
                        .find(|part| part.id == category)
                        .unwrap()
                        .measures
                        .unwrap()
                        .matched_code,
                    bytes
                );
            }
            assert_eq!(report.measures.unwrap().total_code, 1000);
        }
        adjust(&mut game, 45);
        assert!(versions("tla", &game)
            .unwrap_err()
            .contains("global uncredited padding exceeds"));
    }

    #[test]
    fn combined_report_namespaces_each_edition_without_changing_its_measures() {
        let reports = ["tbs", "tla"]
            .into_iter()
            .flat_map(|id| versions(id, &game()).unwrap())
            .collect::<Vec<_>>();
        let combined = combine(&reports).unwrap();
        let names = combined
            .categories
            .iter()
            .filter(|category| !category.id.contains('.'))
            .map(|category| category.id.as_str())
            .collect::<Vec<_>>();
        assert_eq!(
            names,
            [
                "The Broken Seal 🇯🇵",
                "The Broken Seal 🇺🇸",
                "The Broken Seal 🇩🇪",
                "The Broken Seal 🇪🇸",
                "The Broken Seal 🇫🇷",
                "The Broken Seal 🇮🇹",
                "The Lost Age 🇯🇵",
                "The Lost Age 🇺🇸",
                "The Lost Age 🇩🇪",
                "The Lost Age 🇪🇸",
                "The Lost Age 🇫🇷",
                "The Lost Age 🇮🇹",
            ]
        );
        for ((_, report), name) in reports.iter().zip(names) {
            let category = combined
                .categories
                .iter()
                .find(|category| category.id == name)
                .unwrap();
            assert_eq!(category.measures, report.measures);
            let units = combined
                .units
                .iter()
                .filter(|unit| in_category(unit, name))
                .collect::<Vec<_>>();
            assert_eq!(units.len(), report.units.len());
            assert_eq!(
                Measures::sum(units.iter().map(|unit| unit.measures.as_ref().unwrap())),
                report.measures.unwrap()
            );
            for (wrapped, original) in units.iter().zip(&report.units) {
                assert_eq!(wrapped.measures, original.measures);
                assert_eq!(
                    wrapped.name.strip_prefix(&format!("{name}/")),
                    Some(original.name.as_str())
                );
                let metadata = wrapped.metadata.as_ref().unwrap();
                assert_eq!(
                    metadata.source_path,
                    original.metadata.as_ref().unwrap().source_path
                );
                assert_eq!(
                    metadata
                        .progress_categories
                        .iter()
                        .filter(|id| !id.contains('.'))
                        .collect::<Vec<_>>(),
                    [name]
                );
                assert!(metadata
                    .progress_categories
                    .iter()
                    .all(|id| id == name || id.starts_with(&format!("{name}."))));
            }
        }
        assert_eq!(combined.measures.unwrap().total_code, 12000);
        assert_eq!(combined.measures.unwrap().total_data, 12000);
        assert_eq!(
            Report::decode(combined.encode_to_vec().as_slice()).unwrap(),
            combined
        );
    }

    #[test]
    fn combined_report_rejects_ambiguous_or_unroutable_editions() {
        let report = versions("tbs", &game()).unwrap().remove(0).1;
        assert!(combine(&[
            ("tbs-ja".into(), report.clone()),
            ("tbs-ja".into(), report.clone())
        ])
        .is_err());
        for id in ["tbs", "tbs-ko", "other-ja"] {
            assert!(combine(&[(id.into(), report.clone())]).is_err());
        }
        let mut missing = report;
        missing.units[0].metadata = None;
        assert!(combine(&[("tbs-ja".into(), missing)]).is_err());
    }

    #[test]
    fn a_pending_or_invalid_game_publishes_no_reports() {
        let root = tempfile::tempdir().unwrap();
        assert!(publish(
            root.path(),
            [("tbs", Ok(game())), ("tla", Err("pending".into()))]
        )
        .is_err());
        assert!(!root.path().join("out").exists());
        let mut invalid = game();
        invalid.editions[0].1.steered = 9999;
        assert!(publish(root.path(), [("tbs", Ok(game())), ("tla", Ok(invalid))]).is_err());
        assert!(!root.path().join("out").exists());
        let mut invalid = game();
        invalid.english.data_scaffold = -1;
        assert!(publish(root.path(), [("tbs", Ok(game())), ("tla", Ok(invalid))]).is_err());
        assert!(!root.path().join("out").exists());
        publish(root.path(), [("tbs", Ok(game())), ("tla", Ok(game()))]).unwrap();
        assert_eq!(
            std::fs::read_dir(root.path().join("out/reports/decomp"))
                .unwrap()
                .count(),
            13
        );
        for id in ["tbs", "tla"] {
            for (version, expected) in versions(id, &game()).unwrap() {
                let path = root
                    .path()
                    .join(format!("out/reports/decomp/{version}_report/report.pb"));
                let bytes = std::fs::read(path).unwrap();
                let report = Report::decode(bytes.as_slice()).unwrap();
                assert_eq!(report, expected);
            }
        }
        let bytes = std::fs::read(
            root.path()
                .join("out/reports/decomp/combined_report/report.pb"),
        )
        .unwrap();
        let combined = Report::decode(bytes.as_slice()).unwrap();
        assert_eq!(
            combined
                .categories
                .iter()
                .filter(|category| !category.id.contains('.'))
                .count(),
            12
        );
    }

    #[test]
    fn removes_only_legacy_report_files_after_successful_publication() {
        let root = tempfile::tempdir().unwrap();
        let output = root.path().join("out/reports/decomp");
        for id in ["tbs", "tla", "other"] {
            let directory = output.join(format!("{id}_report"));
            std::fs::create_dir_all(&directory).unwrap();
            std::fs::write(directory.join("report.pb"), b"previous report").unwrap();
            std::fs::write(directory.join("keep.txt"), b"unrelated file").unwrap();
        }
        assert!(publish(
            root.path(),
            [("tbs", Ok(game())), ("tla", Err("pending".into()))]
        )
        .is_err());
        for id in ["tbs", "tla", "other"] {
            assert_eq!(
                std::fs::read(output.join(format!("{id}_report/report.pb"))).unwrap(),
                b"previous report"
            );
        }
        assert!(!output.join("tbs-ja_report").exists());
        publish(root.path(), [("tbs", Ok(game())), ("tla", Ok(game()))]).unwrap();
        for id in ["tbs", "tla"] {
            assert!(!output.join(format!("{id}_report/report.pb")).exists());
        }
        assert_eq!(
            std::fs::read(output.join("other_report/report.pb")).unwrap(),
            b"previous report"
        );
        for id in ["tbs", "tla", "other"] {
            assert_eq!(
                std::fs::read(output.join(format!("{id}_report/keep.txt"))).unwrap(),
                b"unrelated file"
            );
        }
    }

    #[test]
    fn exporter_stops_when_the_canonical_comparison_gate_fails() {
        let root = tempfile::tempdir().unwrap();
        std::fs::write(
            root.path().join("Makefile"),
            "compare-editions:\n\t$(error expected comparison failure)\n",
        )
        .unwrap();
        assert_eq!(
            write(root.path()).unwrap_err(),
            "all twelve editions must build and compare before report export"
        );
        assert!(!root.path().join("out").exists());
    }
}
