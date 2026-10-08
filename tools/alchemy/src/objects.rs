use crate::compiler::routing::root;
use crate::ld_map::{Map, Placed, ROM};
use crate::targets::decomp_target;
use std::collections::{BTreeSet, HashMap};
use std::fmt::Write as _;
use std::path::Path;

const USAGE: &str = r#"usage: alchemy objects EDITION... [--all]
After a build of EDITION (tbs-ja ... tla-it), compares out/EDITION/
EDITION.gba with the reference ROM roms/EDITION.gba through
out/EDITION/EDITION.map. Prints the first differing ROM offset and what
is there, with where it belongs when its scaffold or its bytes say; each
run of scaffold sections (placed where the .incbin that opens them does
not read) or English raw listings (placed off their file name's address)
that sit off by the same distance, with the objects just before the run;
then each linked object or section whose bytes differ, with the functions
in it that differ (the first 40 unless --all). Code that runs from IWRAM
is compared where the ROM holds it. Overlays are compared as the
compressed blob that holds them; compare their code with alchemy diff."#;

const SHOWN: usize = 40;

pub(crate) fn scaffold_offsets(source: &str) -> HashMap<String, u32> {
    let mut found = HashMap::new();
    let mut open: Option<String> = None;
    for line in source.lines() {
        let line = line.trim();
        if let Some(rest) = line.strip_prefix(".section") {
            let name = rest.trim().split(|c: char| c == ',' || c.is_whitespace()).next().unwrap_or("");
            open = (!name.is_empty()).then(|| name.to_string());
            continue;
        }
        let Some(name) = open.as_ref() else { continue };
        let directive = line.split_whitespace().next().unwrap_or("");
        let silent = line.is_empty()
            || line.starts_with('@')
            || (line.ends_with(':') && !line.starts_with('.'))
            || matches!(
                directive,
                ".global" | ".globl" | ".type" | ".thumb_func" | ".size" | ".hidden" | ".weak" | ".balign" | ".align" | ".p2align" | ".syntax" | ".thumb" | ".arm" | ".code"
            );
        if silent {
            continue;
        }
        if let Some(rest) = line.strip_prefix(".incbin") {
            let mut parts = rest.split(',').map(str::trim);
            if parts.next() == Some("\"baserom.gba\"") {
                let offset = parts.next().and_then(|o| u32::from_str_radix(o.trim_start_matches("0x"), 16).ok());
                if let Some(offset) = offset {
                    found.insert(name.clone(), offset);
                }
            }
        }
        open = None;
    }
    found
}

pub(crate) struct Anchors {
    scaffold: HashMap<String, HashMap<String, u32>>,
    english: bool,
}

fn is_ordinary(section: &str) -> bool {
    matches!(section, ".text" | ".rodata" | ".data" | ".padding")
}

impl Anchors {
    pub(crate) fn from_sources<'a>(sources: impl IntoIterator<Item = (&'a str, &'a str)>, english: bool) -> Anchors {
        let scaffold = sources.into_iter().map(|(object, text)| (object.to_string(), scaffold_offsets(text))).collect();
        Anchors { scaffold, english }
    }

    pub(crate) fn read(root: &Path, map: &Map, english: bool) -> Anchors {
        let objects: BTreeSet<&str> = map
            .placed
            .iter()
            .filter(|p| !p.is_fill() && !is_ordinary(&p.name) && p.object().starts_with("recon/"))
            .map(Placed::object)
            .collect();
        let texts: Vec<(&str, String)> = objects
            .into_iter()
            .filter_map(|o| Some((o, std::fs::read_to_string(root.join(o.strip_suffix(".o")?.to_string() + ".s")).ok()?)))
            .collect();
        Anchors::from_sources(texts.iter().map(|(o, t)| (*o, t.as_str())), english)
    }

    pub(crate) fn expected(&self, p: &Placed) -> Option<u32> {
        if let Some(&offset) = self.scaffold.get(p.object()).and_then(|sections| sections.get(&p.name)) {
            return Some((ROM + offset).wrapping_add(p.address).wrapping_sub(p.load));
        }
        if !self.english || p.name != ".text" {
            return None;
        }
        let (directory, file) = p.object().rsplit_once('/')?;
        let address = file.strip_suffix(".o").filter(|a| a.len() == 8)?;
        (directory.starts_with("recon/") && directory.ends_with("/raw")).then(|| u32::from_str_radix(address, 16).ok()).flatten()
    }
}

#[derive(Debug, PartialEq, Eq)]
pub(crate) struct Difference {
    pub label: String,
    pub first: u32,
    pub functions: Vec<String>,
}

fn rom_range(p: &Placed) -> std::ops::Range<usize> {
    let start = (p.load - ROM) as usize;
    start..start + p.size as usize
}

pub(crate) fn differences(map: &Map, reference: &[u8], built: &[u8]) -> Vec<Difference> {
    let length = reference.len().min(built.len());
    let mut sections: Vec<&Placed> = map.in_image(length).collect();
    sections.sort_by_key(|p| p.load);
    let mut found = Vec::new();
    for p in sections {
        let range = rom_range(p);
        let Some(first) = range.clone().find(|&k| reference[k] != built[k]) else { continue };
        let mut functions: Vec<String> = Vec::new();
        if !p.is_fill() {
            let mut starts: Vec<u32> = p.symbols.iter().map(|(a, _)| *a).filter(|&a| a < p.end()).collect();
            starts.sort_unstable();
            starts.dedup();
            if starts.first() != Some(&p.address) {
                starts.insert(0, p.address);
            }
            for (i, &start) in starts.iter().enumerate() {
                let end = starts.get(i + 1).copied().unwrap_or(p.end());
                let from = range.start + (start - p.address) as usize;
                let to = range.start + (end - p.address) as usize;
                if (from..to).any(|k| reference[k] != built[k]) {
                    let name = crate::ld_map::preferred(&p.symbols, start).unwrap_or("(before its first symbol)");
                    functions.push(name.to_string());
                }
            }
        }
        found.push(Difference { label: p.label(), first: ROM + first as u32, functions });
    }
    found
}

pub(crate) fn first_offset(reference: &[u8], built: &[u8]) -> Option<usize> {
    let length = reference.len().min(built.len());
    (0..length)
        .find(|&k| reference[k] != built[k])
        .or_else(|| (reference.len() != built.len()).then_some(length))
}

pub(crate) fn apparent_shift(reference: &[u8], built: &[u8], start: usize, end: usize) -> Option<i64> {
    let start = start & !1;
    let end = end.min(start + 512).min(built.len());
    let halves = end.saturating_sub(start) / 2;
    if halves < 4 {
        return None;
    }
    let equal = |shift: i64| -> usize {
        (0..halves)
            .filter(|i| {
                let at = start + 2 * i;
                let there = at as i64 + shift;
                there >= 0 && reference.get(there as usize..there as usize + 2) == built.get(at..at + 2)
            })
            .count()
    };
    let here = equal(0);
    let mut best = (0, 0);
    for distance in (2..=16).step_by(2) {
        for shift in [distance, -distance] {
            let count = equal(shift);
            if count > best.0 {
                best = (count, shift);
            }
        }
    }
    (best.0 * 4 >= halves * 3 && best.0 > here * 2).then_some(best.1)
}

fn bytes(n: i64) -> String {
    let n = n.unsigned_abs();
    if n == 1 { "1 byte".to_string() } else { format!("{n} bytes") }
}

fn displacement(shift: i64) -> String {
    format!("{} {}", bytes(shift), if shift < 0 { "early" } else { "late" })
}

pub(crate) fn describe_first(map: &Map, anchors: &Anchors, reference: &[u8], built: &[u8]) -> Option<String> {
    let offset = first_offset(reference, built)?;
    let load = ROM + offset as u32;
    let mut text = format!("first difference at {load:#010x} (ROM offset {offset:#x})");
    let length = reference.len().min(built.len());
    if offset >= length {
        let _ = write!(text, ": the built ROM is {:#x} bytes, the reference {:#x}", built.len(), reference.len());
        return Some(text);
    }
    let Some(p) = map.at_load(load, length) else {
        text.push_str(", outside every linked section");
        return Some(text);
    };
    let address = p.address + (load - p.load);
    if p.is_fill() {
        let next = map.in_image(length).filter(|q| !q.is_fill() && q.load >= p.load + p.size).min_by_key(|q| q.load);
        let _ = write!(text, ", in {} of alignment fill before {}", bytes(i64::from(p.size)), next.map_or("the end".to_string(), Placed::label));
    } else {
        let _ = write!(text, ", in {}", p.label());
        if let Some((name, into)) = p.symbol_at(address) {
            let _ = write!(text, " at {name}+{into:#x}");
        }
    }
    if let Some(expected) = anchors.expected(p).filter(|&e| e != p.address) {
        let shift = i64::from(p.address) - i64::from(expected);
        let _ = write!(text, "\n  placed at {:#010x}, its bytes belong at {expected:#010x}: {}", p.address, displacement(shift));
    } else if let Some(shift) = apparent_shift(reference, built, offset, rom_range(p).end) {
        let _ = write!(
            text,
            "\n  from there the reference holds the same bytes {} {}: it looks placed {}",
            bytes(shift),
            if shift > 0 { "later" } else { "earlier" },
            displacement(-shift)
        );
    }
    Some(text)
}

#[derive(Debug, PartialEq, Eq)]
pub(crate) struct Run {
    pub shift: i64,
    pub first: usize,
    pub last: usize,
    pub count: usize,
    pub before: Option<usize>,
}

pub(crate) fn misplaced(map: &Map, anchors: &Anchors) -> Vec<Run> {
    let mut anchored: Vec<(usize, i64)> = map
        .placed
        .iter()
        .enumerate()
        .filter(|(_, p)| p.size > 0 && p.loaded)
        .filter_map(|(i, p)| anchors.expected(p).map(|e| (i, i64::from(p.address) - i64::from(e))))
        .collect();
    anchored.sort_by_key(|&(i, _)| map.placed[i].load);
    let mut runs: Vec<Run> = Vec::new();
    let mut good = None;
    let mut previous_shift = 0;
    for (i, shift) in anchored {
        if shift == 0 {
            good = Some(i);
        } else if previous_shift == shift {
            let run = runs.last_mut().expect("a run is open while the shift holds");
            run.last = i;
            run.count += 1;
        } else {
            runs.push(Run { shift, first: i, last: i, count: 1, before: good });
        }
        previous_shift = shift;
    }
    runs
}

fn between(map: &Map, before: Option<usize>, first: usize) -> Vec<String> {
    let from = before.map_or(0, |b| map.placed[b].load + map.placed[b].size);
    let to = map.placed[first].load;
    let mut found: Vec<&Placed> = map.placed.iter().filter(|p| p.size > 0 && p.loaded && p.load >= from && p.load < to).collect();
    found.sort_by_key(|p| p.load);
    found
        .iter()
        .map(|p| if p.is_fill() { format!("{} of fill at {:#010x}", bytes(i64::from(p.size)), p.load) } else { p.label() })
        .collect()
}

pub(crate) fn report(map: &Map, anchors: &Anchors, reference: &[u8], built: &[u8], all: bool) -> String {
    let mut out = String::new();
    let Some(first) = describe_first(map, anchors, reference, built) else {
        return "identical to the reference ROM\n".to_string();
    };
    let _ = writeln!(out, "{first}");
    let runs = misplaced(map, anchors);
    for run in runs.iter().take(10) {
        let (a, b) = (&map.placed[run.first], &map.placed[run.last]);
        let expected = anchors.expected(a).unwrap_or(a.address);
        let _ = write!(
            out,
            "{} {}: from {} at {:#010x} (its bytes belong at {expected:#010x})",
            if run.count == 1 { "1 scaffold section sits".to_string() } else { format!("{} scaffold sections sit", run.count) },
            displacement(run.shift),
            a.label(),
            a.address
        );
        if run.count > 1 {
            let _ = write!(out, " to {} at {:#010x}", b.label(), b.address);
        }
        out.push('\n');
        let near = between(map, run.before, run.first);
        let shown = &near[near.len().saturating_sub(4)..];
        let earlier = run.before.map_or("the image start".to_string(), |i| map.placed[i].label());
        let _ = writeln!(
            out,
            "  after {earlier}, which sits right; just before the run: {}{}",
            if near.len() > shown.len() { "..., " } else { "" },
            if shown.is_empty() { "nothing".to_string() } else { shown.join(", ") }
        );
    }
    if runs.len() > 10 {
        let _ = writeln!(out, "and {} more runs of misplaced scaffold", runs.len() - 10);
    }
    let found = differences(map, reference, built);
    let _ = writeln!(out, "{} linked {}:", found.len(), if found.len() == 1 { "section differs" } else { "sections differ" });
    let mut lines: Vec<(String, Vec<String>)> = Vec::new();
    let mut line_of = HashMap::new();
    for d in &found {
        let at = *line_of.entry(d.label.clone()).or_insert_with(|| {
            lines.push((d.label.clone(), Vec::new()));
            lines.len() - 1
        });
        let functions = &mut lines[at].1;
        for f in &d.functions {
            if !functions.contains(f) {
                functions.push(f.clone());
            }
        }
    }
    let limit = if all { usize::MAX } else { SHOWN };
    for (label, functions) in lines.iter().take(limit) {
        let mut line = format!("  {label}  {}", functions.iter().take(8).cloned().collect::<Vec<_>>().join(" "));
        if functions.len() > 8 {
            let _ = write!(line, " (+{} more)", functions.len() - 8);
        }
        let _ = writeln!(out, "{}", line.trim_end());
    }
    if lines.len() > limit {
        let _ = writeln!(out, "  ... and {} more (--all lists them)", lines.len() - limit);
    }
    out
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let all = arguments.iter().any(|a| a == "--all");
    let editions: Vec<&String> = arguments.iter().filter(|a| !a.starts_with("--")).collect();
    if editions.is_empty() || arguments.iter().any(|a| a == "-h" || a == "--help") {
        return Err(USAGE.into());
    }
    let root = root();
    for edition in editions {
        let target = decomp_target(Some(edition))?;
        let out = root.join(target.output_dir);
        let read = |name: String| std::fs::read(out.join(&name)).map_err(|e| format!("{}: {e}; build {edition} first", out.join(&name).display()));
        let map = Map::parse(&String::from_utf8_lossy(&read(format!("{edition}.map"))?));
        let reference = std::fs::read(root.join(target.rom)).map_err(|e| format!("{}: {e}", target.rom))?;
        let built = read(format!("{edition}.gba"))?;
        let anchors = Anchors::read(root, &map, target.language() == "en");
        print!("{edition}: {}", report(&map, &anchors, &reference, &built, all));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::ld_map::tests::MAP;

    const ROM_S: &str = "@ scaffold\n\t.section .rom.00000000, \"ax\"\n\t.global Rom_Start\nRom_Start:\n\t.incbin \"baserom.gba\", 0x00000000, 0x00000010\n\t.section .rom.00000020, \"ax\"\n\t.balign 4\n\t.global Func_08000020\n\t.thumb_func\nFunc_08000020:\n\t.incbin \"baserom.gba\", 0x00000020, 0x00000010\n\t.section .rom.00000060, \"ax\"\n\t.byte 0\n\t.incbin \"baserom.gba\", 0x00000061, 0x00000001\n";
    const UNIDENTIFIED_S: &str = "\t.section .unidentified.08000030,\"a\"\n\t.global Math_Table\nMath_Table:\n\t.incbin \"baserom.gba\", 0x00000038, 0x00000010\n";

    fn anchors(english: bool) -> Anchors {
        Anchors::from_sources([("recon/tbs/de/rom.o", ROM_S), ("recon/tbs/unidentified.o", UNIDENTIFIED_S)], english)
    }

    fn image() -> Vec<u8> {
        (0..0x58u32).map(|i| (i * 7 + 3) as u8).collect()
    }

    #[test]
    fn scaffold_sections_belong_where_their_incbin_reads() {
        let offsets = scaffold_offsets(ROM_S);
        assert_eq!(offsets.get(".rom.00000000"), Some(&0));
        assert_eq!(offsets.get(".rom.00000020"), Some(&0x20));
        assert_eq!(offsets.get(".rom.00000060"), None);
        assert_eq!(scaffold_offsets(UNIDENTIFIED_S).get(".unidentified.08000030"), Some(&0x38));
        let map = Map::parse(MAP);
        let english = anchors(true);
        let expected: Vec<Option<u32>> = map.placed.iter().map(|p| english.expected(p)).collect();
        assert_eq!(expected, [Some(0x0800_0000), None, None, Some(0x0800_0020), None, None, None, Some(0x0800_0038), Some(0x0800_0048)]);
        assert_eq!(anchors(false).expected(&map.placed[8]), None);
        assert!(misplaced(&map, &english).is_empty());
    }

    #[test]
    fn identical_images_report_nothing() {
        let map = Map::parse(MAP);
        assert_eq!(report(&map, &anchors(false), &image(), &image(), false), "identical to the reference ROM\n");
    }

    #[test]
    fn names_the_sections_and_functions_that_differ() {
        let map = Map::parse(MAP);
        let reference = image();
        let mut built = reference.clone();
        built[0x1a] ^= 1;
        built[0x34] ^= 1;
        built[0x4a] ^= 1;
        let found = differences(&map, &reference, &built);
        let summary: Vec<(&str, u32, &[String])> = found.iter().map(|d| (d.label.as_str(), d.first, d.functions.as_slice())).collect();
        assert_eq!(
            summary,
            [
                ("games/THE BROKEN SEAL/SRC/SYSTEM/IO.o", 0x0800_001a, &["Input_Poll".to_string()][..]),
                ("games/THE BROKEN SEAL/SRC/SYSTEM/RUNTIME.o", 0x0800_0034, &["IwramIrqMain".to_string()][..]),
                ("recon/tbs/raw/08000048.o", 0x0800_004a, &["Func_08000048".to_string()][..]),
            ]
        );
        let text = describe_first(&map, &anchors(false), &reference, &built).unwrap();
        assert_eq!(text, "first difference at 0x0800001a (ROM offset 0x1a), in games/THE BROKEN SEAL/SRC/SYSTEM/IO.o at Input_Poll+0x2");
        built.truncate(0x40);
        assert_eq!(first_offset(&reference[..0x40], &built), Some(0x1a));
        assert_eq!(first_offset(&reference, &reference[..0x40]), Some(0x40));
    }

    #[test]
    fn a_scaffold_section_two_bytes_early_is_named_with_the_object_before_it() {
        let shifted = MAP
            .replace(" *fill*         0x000000000800001e        0x2\n", "")
            .replace(" .rom.00000020  0x0000000008000020", " .rom.00000020  0x000000000800001e")
            .replace("                0x0000000008000020                Func_08000020", "                0x000000000800001e                Func_08000020")
            .replace(".unidentified.08000030\n                0x0000000008000038", ".unidentified.08000030\n                0x0000000008000036");
        let map = Map::parse(&shifted);
        let anchors = anchors(true);
        let runs = misplaced(&map, &anchors);
        assert_eq!(runs.len(), 1);
        assert_eq!((runs[0].shift, runs[0].count, runs[0].before), (-2, 2, Some(0)));
        assert_eq!(map.placed[runs[0].first].label(), "recon/tbs/de/rom.o(.rom.00000020)");
        let reference = image();
        let mut built = reference.clone();
        built.copy_within(0x20..0x30, 0x1e);
        let text = report(&map, &anchors, &reference, &built, false);
        assert!(text.starts_with("first difference at 0x0800001e (ROM offset 0x1e), in recon/tbs/de/rom.o(.rom.00000020) at Func_08000020+0x0\n  placed at 0x0800001e, its bytes belong at 0x08000020: 2 bytes early\n"), "{text}");
        assert!(text.contains("2 scaffold sections sit 2 bytes early: from recon/tbs/de/rom.o(.rom.00000020) at 0x0800001e (its bytes belong at 0x08000020) to recon/tbs/unidentified.o(.unidentified.08000030) at 0x08000036\n  after recon/tbs/de/rom.o(.rom.00000000), which sits right; just before the run: games/THE BROKEN SEAL/SRC/SYSTEM/IO.o\n"), "{text}");
    }

    #[test]
    fn bytes_that_moved_show_their_distance() {
        let reference: Vec<u8> = (0..64u32).map(|i| (i * 13 + 5) as u8).collect();
        let mut built = reference.clone();
        built.copy_within(18..60, 16);
        assert_eq!(apparent_shift(&reference, &built, 16, 56), Some(2));
        assert_eq!(apparent_shift(&reference, &reference, 16, 56), None);
        let mut noisy = reference.clone();
        for b in &mut noisy[16..56] {
            *b ^= 0x5a;
        }
        assert_eq!(apparent_shift(&reference, &noisy, 16, 56), None);
    }
}
