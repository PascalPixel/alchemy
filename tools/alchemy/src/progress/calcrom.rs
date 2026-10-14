//! DONE code and data from the linker maps, as pret's calcrom counts them
//! (C2): every byte each image places, the ROM and each overlay in its
//! unpacked form, sorted by the folder of the object that placed it. Code
//! is the bytes of `.text` sections; from games/ it is done, as C when its
//! source is C, as assembly otherwise (8-byte far-call stubs and the
//! compiler library counted apart); from recon/ it is not yet. Every other
//! byte is data: done from games/, from the compiler library or as the
//! linker's fill, not yet from recon/. An overlay's packed stream in the
//! ROM is outside the totals. An unfinished edition's code total is the
//! larger of its own and the English build's: its code not yet matched may
//! still be bytes of its scaffold, which the English build has as listings.
//! An edition counts only when its ROM matches rom.sha1.

use super::floor_percent;
use sha1::{Digest, Sha1};
use std::path::Path;

pub(crate) const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];

/// One game's DONE parts, summed over its six editions.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub struct GameDone {
    pub common_asm: i64,
    pub common_c: i64,
    /// Assembly from the game's folder, with the stubs and the library.
    pub game_asm: i64,
    pub game_c: i64,
    pub executable: i64,
    pub veneers: i64,
    /// DONE data: non-code bytes placed from games/, the library or fill,
    /// out of every non-code byte the images place.
    pub data: i64,
    pub data_total: i64,
}

impl GameDone {
    pub fn parts(&self) -> (f64, f64, f64) {
        (
            floor_percent(self.common_c + self.game_c, self.executable),
            floor_percent(self.common_asm + self.game_asm - self.veneers, self.executable),
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

/// One edition's count, with the compiler library apart for the report.
#[derive(Clone, Copy, Debug, Default)]
pub(crate) struct Counted {
    pub done: GameDone,
    pub library: i64,
}

#[derive(Clone, Debug)]
pub(crate) struct Game {
    pub editions: Vec<(&'static str, Counted)>,
}

impl Game {
    pub fn combined(&self) -> GameDone {
        let mut combined = GameDone::default();
        for (_, edition) in &self.editions {
            combined += edition.done;
        }
        combined
    }
}

#[derive(Default)]
struct Tally {
    span: i64,
    streams: i64,
    code: i64,
    data_todo: i64,
    counted: Counted,
}

fn number(text: &str) -> Option<i64> {
    i64::from_str_radix(text.strip_prefix("0x")?, 16).ok()
}

/// An 8-byte far-call stub: a far-call bank, or an overlay's entry or
/// import table.
fn stub(stem: &str) -> bool {
    stem.contains("/far_call/")
        || stem.contains("/entry/")
        || stem.contains("/import/")
        || stem.ends_with("/entry")
        || stem.ends_with("/import")
}

/// Counts one linker map. The images' placed sections are the ROM's (their
/// load address in the cartridge) or an overlay's `.text` and `.data`.
fn tally(root: &Path, map: &str, overlay: bool, tally: &mut Tally) {
    let Some(body) = map.split("Linker script and memory map").nth(1) else { return };
    let (mut first, mut last) = (i64::MAX, 0);
    let (mut placed, mut wrapped) = (false, None::<String>);
    for line in body.lines() {
        if !line.starts_with(' ') && !line.is_empty() {
            let fields: Vec<&str> = line.split_whitespace().collect();
            let (address, size) = (fields.get(1).and_then(|f| number(f)), fields.get(2).and_then(|f| number(f)));
            let load = line.split("load address ").nth(1).and_then(|rest| number(rest.trim()));
            placed = match (address, size) {
                (Some(address), Some(size)) if size > 0 => {
                    let at = load.unwrap_or(address);
                    let image = if overlay {
                        matches!(fields[0], ".text" | ".data")
                    } else {
                        (0x0800_0000..0x0a00_0000).contains(&at)
                    };
                    if image {
                        first = first.min(at);
                        last = last.max(at + size);
                    }
                    image
                }
                _ => false,
            };
            continue;
        }
        if !placed {
            continue;
        }
        let fields: Vec<&str> = line.split_whitespace().collect();
        if let [name] = fields.as_slice() {
            if name.starts_with('.') {
                wrapped = Some((*name).to_owned());
            }
            continue;
        }
        let (name, rest) = match wrapped.take() {
            Some(name) => (name, &fields[..]),
            None if fields.first().is_some_and(|name| name.starts_with('.')) => (fields[0].to_owned(), &fields[1..]),
            None => continue,
        };
        let (Some(_), Some(size)) = (rest.first().and_then(|f| number(f)), rest.get(1).and_then(|f| number(f))) else {
            continue;
        };
        let object = rest.get(2).copied().unwrap_or("");
        if size == 0 {
            continue;
        }
        if name == ".overlays" {
            tally.streams += size;
            continue;
        }
        let stem = object.split_once("/obj/").map(|(_, stem)| stem.trim_end_matches(".o"));
        let games = stem.is_some_and(|stem| stem.starts_with("games/"));
        let library = object.contains("libgcc.a");
        let own = object.contains("/overlays/resource_");
        let done = &mut tally.counted.done;
        if !name.starts_with(".text") {
            if !(games || library || own) {
                tally.data_todo += size;
            }
            continue;
        }
        tally.code += size;
        match stem {
            _ if library => {
                done.game_asm += size;
                tally.counted.library += size;
            }
            Some(stem) if games => {
                let common = stem.starts_with("games/common/");
                if root.join(format!("{stem}.c")).is_file() {
                    *if common { &mut done.common_c } else { &mut done.game_c } += size;
                } else {
                    *if common { &mut done.common_asm } else { &mut done.game_asm } += size;
                    if stub(stem) {
                        done.veneers += size;
                    }
                }
            }
            _ => {}
        }
    }
    if last > first {
        tally.span += last - first;
    }
}

/// Whether an edition's ROM is the one rom.sha1 names.
fn verified(root: &Path, edition: &str) -> Result<(), String> {
    let rom = format!("out/{edition}/{edition}.gba");
    let list = std::fs::read_to_string(root.join("rom.sha1")).map_err(|error| format!("rom.sha1: {error}"))?;
    let expected = list
        .lines()
        .find_map(|line| line.strip_suffix(&format!("  {rom}")))
        .ok_or_else(|| format!("rom.sha1 names no {rom}"))?;
    let bytes = std::fs::read(root.join(&rom)).map_err(|_| format!("{rom} is not built: make compare-{edition}"))?;
    if format!("{:x}", Sha1::digest(bytes)) != expected {
        return Err(format!("{rom} does not match rom.sha1"));
    }
    Ok(())
}

pub(crate) fn measure_edition(root: &Path, edition: &str) -> Result<Counted, String> {
    verified(root, edition)?;
    let mut counted = Tally::default();
    let main = root.join(format!("out/{edition}/{edition}.map"));
    let text = std::fs::read_to_string(&main).map_err(|error| format!("{}: {error}", main.display()))?;
    tally(root, &text, false, &mut counted);
    let mut maps: Vec<_> = std::fs::read_dir(root.join(format!("out/{edition}/overlays")))
        .map_err(|error| format!("out/{edition}/overlays: {error}"))?
        .filter_map(|entry| Some(entry.ok()?.path()))
        .filter(|path| path.extension().is_some_and(|extension| extension == "map"))
        .collect();
    maps.sort();
    for map in maps {
        let text = std::fs::read_to_string(&map).map_err(|error| format!("{}: {error}", map.display()))?;
        tally(root, &text, true, &mut counted);
    }
    let done = &mut counted.counted.done;
    done.executable = counted.code;
    done.data_total = counted.span - counted.code - counted.streams;
    done.data = done.data_total - counted.data_todo;
    Ok(counted.counted)
}

/// A game's six editions, or why it is pending.
pub(crate) fn measure_game(root: &Path, game: &str) -> Result<Game, String> {
    let mut editions = Vec::new();
    for language in LANGUAGES {
        editions.push((language, measure_edition(root, &format!("{game}-{language}"))?));
    }
    let english = editions[1].1.done.executable;
    for (_, counted) in &mut editions {
        if !counted.done.complete() {
            counted.done.executable = counted.done.executable.max(english);
        }
    }
    Ok(Game { editions })
}

#[cfg(test)]
mod tests {
    use super::*;

    const MAP: &str = "\
Linker script and memory map

ewram           0x02000000    0x38000
 .sym_ewram     0x02000000    0x38000 out/x/obj/games/tbs/src/system/ewram.o
.rom_start      0x08000000      0x200
 .text          0x08000000      0x100 out/x/obj/games/tbs/src/main.o
 .text          0x08000100       0x40 out/x/obj/games/tbs/src/system/far_call/RUNTIME.o
 .rodata        0x08000140       0x30 out/x/obj/games/tbs/src/main.o
 *fill*         0x08000170       0x10 
 .rom.00000180  0x08000180       0x40 out/x/obj/recon/tla/ja/rom.o
 .text          0x080001c0       0x20 out/x/obj/recon/tla/raw/080001c0.o
 .overlays      0x080001e0       0x20 out/x/obj/games/tbs/src/system/overlay/overlays.o
";

    #[test]
    fn a_map_counts_by_folder() {
        let mut counted = Tally::default();
        tally(Path::new("/nowhere"), MAP, false, &mut counted);
        let done = counted.counted.done;
        assert_eq!(counted.span, 0x200);
        assert_eq!(counted.code, 0x160);
        assert_eq!((done.game_asm, done.veneers), (0x140, 0x40));
        assert_eq!(counted.streams, 0x20);
        assert_eq!(counted.data_todo, 0x40);
    }
}
