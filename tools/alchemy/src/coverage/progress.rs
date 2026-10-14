use super::calcrom::Game;
use super::{commas, floor_percent, measure_both};

const USAGE: &str = "usage: alchemy check progress\n\
Reports each game's DONE, code and data, in all six of its editions together, with\n\
code in its parts, then each edition's two shares. A game is 100% only when both are.";

#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct GameDone {
    pub common_asm: i64,
    pub common_c: i64,
    pub game_asm: i64,
    pub game_c: i64,
    pub executable: i64,
    pub veneers: i64,
    /// DONE data: non-code bytes the images place from games/ or as fill,
    /// out of every non-code byte they place.
    pub data: i64,
    pub data_total: i64,
}
impl GameDone {
    pub fn parts(&self) -> (f64, f64, f64) {
        (
            floor_percent(self.common_c + self.game_c, self.executable),
            floor_percent(
                self.common_asm + self.game_asm - self.veneers,
                self.executable,
            ),
            floor_percent(self.veneers, self.executable),
        )
    }
    pub fn bytes(&self) -> i64 {
        self.common_asm + self.common_c + self.game_asm + self.game_c
    }
    pub fn percent(&self) -> f64 {
        floor_percent(self.bytes(), self.executable)
    }
    pub fn data_percent(&self) -> f64 {
        floor_percent(self.data, self.data_total)
    }
    /// A game is 100% only when both its code and its data are.
    pub fn complete(&self) -> bool {
        self.executable > 0 && self.bytes() == self.executable && self.data == self.data_total
    }
}
impl std::ops::AddAssign for GameDone {
    fn add_assign(&mut self, other: GameDone) {
        self.common_asm += other.common_asm;
        self.common_c += other.common_c;
        self.game_asm += other.game_asm;
        self.game_c += other.game_c;
        self.executable += other.executable;
        self.veneers += other.veneers;
        self.data += other.data;
        self.data_total += other.data_total;
    }
}

fn done_line(mark: &str, game: &str, status: &Result<Game, String>) -> String {
    match status {
        Ok(measured) => {
            let all = measured.combined();
            let (d, english) = (all.done, &measured.english);
            let (c, assembly, stubs) = d.parts();
            let mut line = format!(
                "{mark} {game} DONE {}: code {:.2}% · data {:.2}% ({} / {} data bytes in six editions, {} of them placed from recon/ in the English build). Code: {} / {} executable bytes in six editions ({:.2}%) = C {} ({c:.2} points) + assembly {} ({assembly:.2}, compiler library {} of it) + 8-byte stubs {} ({stubs:.2}); FAKEMATCH-steered C {} ({:.2} points, removed last); uncredited padding {} not counted; not yet C in the English build: disassembly {} + overlay listings {}",
                if d.complete() { "100%" } else { "not yet" }, d.percent(), d.data_percent(),
                commas(d.data), commas(d.data_total), commas(english.data_pending.iter().map(|row| row.2).sum::<i64>()),
                commas(d.bytes()), commas(d.executable), d.percent(),
                commas(d.common_c + d.game_c), commas(d.common_asm + d.game_asm - d.veneers), commas(all.library), commas(d.veneers),
                commas(all.steered), floor_percent(all.steered, d.executable), commas(all.uncredited),
                commas(english.raw), commas(english.listings)
            );
            for (object, bytes) in &english.other {
                line.push_str(&format!(" + unclassified {} ({object})", commas(*bytes)));
            }
            line.push_str(&arm_part(&super::arm::ArmTally::of(&english.arm)));
            line.push_str(&likely_part(&english.arm_likely));
            line
        }
        Err(reason) => format!("{mark} {game} DONE: {reason}"),
    }
}

/// The English build's ARM bytes inside the line: a breakdown of the
/// totals before it, which it leaves unchanged.
fn arm_part(t: &super::arm::ArmTally) -> String {
    let mut part = format!(
        "; ARM in the English build {} = credited {} (assembly {} + compiler library {} + C {}) + not yet {} (disassembly {} + overlay listings {}) + uncredited padding {}",
        commas(t.total), commas(t.credited()), commas(t.assembly), commas(t.library), commas(t.c),
        commas(t.disassembly + t.listings), commas(t.disassembly), commas(t.listings), commas(t.padding)
    );
    if t.outside > 0 {
        part.push_str(&format!(" + outside the executable total {}", commas(t.outside)));
    }
    if t.other > 0 {
        part.push_str(&format!(" + unclassified {}", commas(t.other)));
    }
    part
}

/// Data words in the English build's not-yet-C listings that read as ARM
/// code: apart from the ARM part, never added to it or to any total.
fn likely_part(rows: &[super::arm::ArmRow]) -> String {
    let bytes: i64 = rows.iter().map(|row| row.bytes).sum();
    if bytes == 0 {
        return String::new();
    }
    let listings = rows.iter().map(|row| row.object.as_str()).collect::<std::collections::BTreeSet<_>>();
    format!(
        "; likely ARM written as data in {} not-yet-C listings {} (not in the ARM part)",
        listings.len(),
        commas(bytes)
    )
}

/// out/reports/arm-GAME.tsv: every function's ARM bytes in the English
/// build, in image and address order, with what its instructions show;
/// arm-likely-GAME.tsv: the same for data words in not-yet-C listings that
/// are likely ARM.
fn write_arm(root: &std::path::Path, id: &str, english: &super::calcrom::Measurement) -> Result<(), String> {
    for (file, rows) in [(format!("arm-{id}.tsv"), &english.arm), (format!("arm-likely-{id}.tsv"), &english.arm_likely)] {
        let mut text = format!("{}\n", super::arm::TSV_HEADER);
        for row in rows {
            text.push_str(&row.tsv());
            text.push('\n');
        }
        let path = root.join("out/reports").join(file);
        std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
        std::fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    Ok(())
}

fn edition_lines(mark: &str, status: &Result<Game, String>) -> Vec<String> {
    let Ok(game) = status else {
        return Vec::new();
    };
    game.editions
        .iter()
        .map(|(language, edition)| {
            let d = edition.done;
            format!(
                "{mark}   {language} code {:.2}% ({} / {}) · data {:.2}% ({} / {})",
                d.percent(),
                commas(d.bytes()),
                commas(d.executable),
                d.data_percent(),
                commas(d.data),
                commas(d.data_total)
            )
        })
        .collect()
}

fn run(argv: &[String]) -> Result<String, String> {
    if !argv.is_empty() {
        return match argv {
            [flag] if flag == "-h" || flag == "--help" => Ok(USAGE.into()),
            _ => Err(format!(
                "unrecognized arguments: {}\n{USAGE}",
                argv.join(" ")
            )),
        };
    }
    let root = crate::compiler::routing::root();
    let games = measure_both(root)?;
    for (id, game) in ["tbs", "tla"].iter().zip(&games) {
        if let Ok(game) = game {
            write_steered(root, id, &game.english)?;
            write_missing(root, id, game)?;
            write_arm(root, id, &game.english)?;
            write_data(root, id, &game.english)?;
        }
    }
    let devices = crate::devices::count(root)?;
    let linked = crate::dead::Linked::read(root)?;
    let mut lines = Vec::new();
    for ((mark, game, id), state) in [("☀️", "The Broken Seal", "tbs"), ("⚓️", "The Lost Age", "tla")]
        .iter()
        .zip(&games)
    {
        let mut line = done_line(mark, game, state);
        line.push_str(&patch_part(root, id, &linked.files)?);
        line.push_str(&crate::devices::summary(&devices, id));
        lines.push(line);
        lines.extend(edition_lines(mark, state));
    }
    Ok(lines.join("\n"))
}

/// out/reports/data-GAME.tsv: the English build's data bytes placed from
/// recon/, largest first: what keeps DONE data short.
fn write_data(
    root: &std::path::Path,
    id: &str,
    english: &super::calcrom::Measurement,
) -> Result<(), String> {
    let mut rows = english.data_pending.clone();
    rows.sort_by(|a, b| b.2.cmp(&a.2).then_with(|| a.cmp(b)));
    let mut text = String::from("bytes\timage\tobject\n");
    for (image, object, bytes) in rows {
        text.push_str(&format!("{bytes}\t{image}\t{object}\n"));
    }
    let path = root.join(format!("out/reports/data-{id}.tsv"));
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    std::fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))
}

/// out/reports/steered-GAME.tsv: each FAKEMATCH-steered function of the
/// English build, largest first, with the tags that steer it; a function
/// stops counting as steered only when every tag listed goes.
fn write_steered(root: &std::path::Path, id: &str, english: &super::calcrom::Measurement) -> Result<(), String> {
    let mut rows = english.steered_units.clone();
    rows.sort_by(|a, b| b.1.cmp(&a.1).then_with(|| a.0.cmp(&b.0)));
    let mut text = String::from("bytes\ttags\tobject\tfunction\tsteered_by\n");
    for ((_, object, function), bytes, roots) in rows {
        let tags = roots.iter().cloned().collect::<Vec<_>>();
        text.push_str(&format!(
            "{bytes}\t{}\t{object}\t{}\t{}\n",
            tags.len(),
            function.unwrap_or_default(),
            tags.join("; ")
        ));
    }
    let path = root.join(format!("out/reports/steered-{id}.tsv"));
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    std::fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))
}

/// out/reports/missing-GAME.tsv: each edition's English-credited functions
/// that edition does not link as credit, largest first: "scaffold" when the
/// edition still has them to write, "absent" when its source leaves them out.
fn write_missing(root: &std::path::Path, id: &str, game: &Game) -> Result<(), String> {
    let mut text = String::from("edition\tstate\tbytes\tobject\tfunction\n");
    for (language, units) in &game.missing {
        let mut rows = units.clone();
        rows.sort_by(|a, b| b.2.cmp(&a.2).then_with(|| b.1.cmp(&a.1)).then_with(|| a.0.cmp(&b.0)));
        for ((_, object, function), bytes, present) in rows {
            let state = if present { "scaffold" } else { "absent" };
            text.push_str(&format!(
                "{language}\t{state}\t{bytes}\t{object}\t{}\n",
                function.unwrap_or_default()
            ));
        }
    }
    let path = root.join(format!("out/reports/missing-{id}.tsv"));
    std::fs::create_dir_all(path.parent().unwrap()).map_err(|e| e.to_string())?;
    std::fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))
}

pub fn entry(arguments: &[String]) {
    match run(arguments) {
        Ok(output) => println!("{output}"),
        Err(error) => {
            eprintln!("error: {error}");
            std::process::exit(1);
        }
    }
}

/// Post-pack patches (W3) a game's build applies: the shipped literal bytes
/// Camelot wrote into streams after packing, reported apart from every total.
/// A patch counts for a game when a source one of its editions links, or a
/// source that one includes, names the asset the patch sits beside, so a
/// games/common picture both games build counts in both and a copy no
/// edition names counts in neither.
fn patch_part(
    root: &std::path::Path,
    id: &str,
    linked: &[(crate::targets::DecompTarget, std::collections::BTreeSet<String>)],
) -> Result<String, String> {
    let game = format!("games/{id}");
    let mut sources = std::collections::BTreeMap::new();
    for (target, reached) in linked.iter().filter(|(target, _)| target.game_dir() == game) {
        for source in reached {
            sources.entry(source.as_str()).or_insert(*target);
        }
    }
    let mut files = std::collections::BTreeSet::new();
    for (source, target) in sources {
        for path in crate::build_rom::patch_paths(root, target, std::path::Path::new(source))? {
            if root.join(&path).is_file() {
                files.insert(path);
            }
        }
    }
    let mut literals = 0;
    for file in &files {
        let path = root.join(file);
        let text = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        literals += ags::patch::Patch::parse(&file.to_string_lossy(), &text)?.literals();
    }
    Ok(format!(
        "; post-pack patch literals {literals} in {} assets (data, not counted)",
        files.len()
    ))
}

#[cfg(test)]
mod tests {
    use super::{patch_part, GameDone};
    use crate::targets::{target_for, DecompTargetId};

    #[test]
    fn a_patch_counts_for_each_game_whose_link_names_its_asset() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let patch = "position\tpack\tship\tproof\n\
            4\t0x02\t0x01\tliteral: 254 of 255 stand-ins reproduce the stream\n\
            6\t0x02\t0x01\tcopy of 4\n\
            9\t0x03\t0x01\tliteral: 250 of 255 stand-ins reproduce the stream\n";
        // games/common's picture, which one game includes and the other links.
        write("games/common/src/graphics/title/peak.s", "\t.incbin \"graphics/title/peak.parts.plz\"\n");
        write("games/common/src/graphics/title/peak_patch.tsv", patch);
        write("games/tbs/src/graphics/title/title.s", "\t.include \"games/common/src/graphics/title/peak.s\"\n");
        // A game's own picture, and a copy beside an asset no edition names.
        write("games/tla/src/map/m1/m1.s", "\t.incbin \"map/m1/grid.plane.lz\"\n");
        write("games/tla/src/map/m1/grid_patch.tsv", patch);
        write(
            "games/tbs/src/graphics/title/peak_patch.tsv",
            "position\tpack\tship\tproof\n4\t0x02\t0x01\tliteral\n",
        );
        let reached = |id, files: &[&str]| {
            (target_for(id), files.iter().map(|file| (*file).to_owned()).collect())
        };
        let linked = [
            reached(DecompTargetId::TbsEn, &["games/tbs/src/graphics/title/title.s"]),
            reached(DecompTargetId::TbsJa, &["games/tbs/src/graphics/title/title.s"]),
            reached(
                DecompTargetId::TlaEn,
                &["games/common/src/graphics/title/peak.s", "games/tla/src/map/m1/m1.s"],
            ),
        ];
        assert_eq!(
            patch_part(root.path(), "tbs", &linked).unwrap(),
            "; post-pack patch literals 2 in 1 assets (data, not counted)"
        );
        assert_eq!(
            patch_part(root.path(), "tla", &linked).unwrap(),
            "; post-pack patch literals 4 in 2 assets (data, not counted)"
        );
    }

    #[test]
    fn a_game_is_complete_only_when_code_and_data_both_are() {
        let code = GameDone {
            game_c: 10,
            executable: 10,
            data: 9,
            data_total: 10,
            ..GameDone::default()
        };
        assert_eq!((code.percent(), code.data_percent()), (100.0, 90.0));
        assert!(!code.complete());
        assert!(GameDone { data: 10, ..code }.complete());
        assert!(!GameDone {
            game_c: 9,
            data: 10,
            ..code
        }
        .complete());
    }
}
