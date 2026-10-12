use crate::compiler::routing::root;

const USAGE: &str = r#"usage: alchemy label NAME ENGLISH_ADDRESS LENGTH [--game tbs|tla] [--function]
Finds LENGTH bytes at ENGLISH_ADDRESS (hexadecimal, 0x08......) of the
English ROM in each other edition's ROM, nearest the same address, and
splits that edition's recon/<game>/<lang>/rom.s incbin there with
".global NAME" and "NAME:". For data adopted C references that only the
English scaffold labelled. The game defaults to tbs.
--function names Thumb code instead: the copy may differ from English's in
its call targets and literal-pool words, which move with the edition; it is
looked for in the scaffold's bytes only, nearest where the builds' maps
(out/, after make compare-editions) say it lies, and the label is a Thumb function's
(".type NAME, %function" and ".thumb_func"), so a call or a pointer to it
from adopted C links as it did."#;

pub(crate) fn find_all(haystack: &[u8], needle: &[u8]) -> Vec<usize> {
    if needle.is_empty() || needle.len() > haystack.len() {
        return Vec::new();
    }
    (0..=haystack.len() - needle.len())
        .step_by(2)
        .filter(|&i| haystack[i..i + needle.len()] == *needle)
        .collect()
}

/// A copy of English code in another edition's ROM.
#[derive(Debug, PartialEq, Eq)]
pub(crate) struct Copy {
    /// Its file offset.
    pub at: usize,
    /// `identical`, or `relocated`: only call targets and pool words differ.
    pub class: &'static str,
    /// How many places qualified; `at` is the one nearest the expected place.
    pub found: usize,
}

/// The places an unchanged run was found that are weighed at most, nearest the
/// expected place first.
const WEIGHED: usize = 1024;
/// How far from the expected place code with no unchanged run is looked for.
const WINDOW: usize = 0x4000;

/// Finds the copy of `reference`, code at `base` in the English ROM, among
/// the scaffold bytes of another edition's ROM (`extents`: file offset and
/// size of each run a scaffold section holds). A copy is the same bytes, or
/// the same instructions with other call targets and literal-pool words, as
/// `alchemy similar --editions` classes them. Candidates come from the
/// reference's longest unchanged runs, or from around `expected` when it has
/// none; the copy nearest `expected` wins.
pub(crate) fn copy_of(reference: &[u8], base: u32, arm: bool, rom: &[u8], extents: &[(u32, u32)], expected: usize) -> Option<Copy> {
    let length = reference.len();
    let step = if arm { 4 } else { 2 };
    let anchors = crate::editions::anchors(reference, base, arm);
    let (mut starts, mut around) = (std::collections::BTreeSet::new(), std::collections::BTreeSet::new());
    for &(file, size) in extents {
        let (file, size) = (file as usize, size as usize);
        let Some(run) = rom.get(file..file + size).filter(|run| run.len() >= length) else {
            continue;
        };
        starts.extend(find_all(run, reference).into_iter().map(|at| file + at));
        for (offset, anchor) in &anchors {
            let hits = find_all(run, anchor).into_iter();
            starts.extend(hits.filter(|hit| hit >= offset && hit - offset + length <= size).map(|hit| file + hit - offset));
        }
        if anchors.is_empty() {
            let low = expected.saturating_sub(WINDOW).max(file).next_multiple_of(step);
            let high = (expected + WINDOW).min(file + size - length);
            around.extend((low..=high).step_by(step));
        }
    }
    let mut nearest: Vec<usize> = starts.into_iter().collect();
    nearest.sort_by_key(|start| start.abs_diff(expected));
    nearest.truncate(WEIGHED);
    nearest.extend(around);
    nearest.retain(|start| start % step == 0);
    nearest.sort_by_key(|start| start.abs_diff(expected));
    nearest.dedup();
    let mut copies = nearest.into_iter().filter_map(|start| {
        let verdict = crate::editions::compare(reference, base, &rom[start..start + length], crate::ld_map::ROM + start as u32, arm);
        matches!(verdict.class, "identical" | "relocated").then_some((start, verdict.class))
    });
    let (at, class) = copies.next()?;
    Some(Copy { at, class, found: 1 + copies.count() })
}

pub(crate) fn split_at(text: &str, offset: u32, name: &str, function: bool) -> Option<String> {
    if text.lines().any(|l| l == format!("{name}:")) {
        return None;
    }
    let mut out = Vec::new();
    let mut done = false;
    for line in text.lines() {
        let range = line
            .strip_prefix("\t.incbin \"baserom.gba\", 0x")
            .and_then(|r| r.split_once(", 0x"))
            .and_then(|(o, s)| Some((u32::from_str_radix(o, 16).ok()?, u32::from_str_radix(s.trim(), 16).ok()?)));
        match range {
            Some((start, size)) if !done && start <= offset && offset < start + size => {
                if offset > start {
                    out.push(format!("\t.incbin \"baserom.gba\", 0x{start:08x}, 0x{:08x}", offset - start));
                }
                out.push(format!("\t.global {name}"));
                if function {
                    out.push(format!("\t.type {name}, %function"));
                    out.push("\t.thumb_func".to_string());
                }
                out.push(format!("{name}:"));
                out.push(format!("\t.incbin \"baserom.gba\", 0x{offset:08x}, 0x{:08x}", start + size - offset));
                done = true;
            }
            _ => out.push(line.to_string()),
        }
    }
    done.then(|| out.join("\n") + "\n")
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let mut positional = Vec::new();
    let (mut game, mut function) = ("tbs", false);
    let mut i = 0;
    while i < arguments.len() {
        match arguments[i].as_str() {
            "--game" => {
                i += 1;
                game = match arguments.get(i).map(String::as_str) {
                    Some("tbs") => "tbs",
                    Some("tla") => "tla",
                    _ => return Err(USAGE.into()),
                };
            }
            "--function" => function = true,
            "-h" | "--help" => return Err(USAGE.into()),
            value => positional.push(value.to_string()),
        }
        i += 1;
    }
    let [name, address, length] = positional.as_slice() else {
        return Err(USAGE.into());
    };
    let address = u32::from_str_radix(address.trim_start_matches("0x"), 16).map_err(|_| USAGE.to_string())?;
    let offset = address.checked_sub(0x0800_0000).ok_or_else(|| USAGE.to_string())? as usize;
    let length: usize = length.parse().map_err(|_| USAGE.to_string())?;
    let root = root();
    let read = |edition: &str| {
        let path = root.join(format!("roms/{game}-{edition}.gba"));
        std::fs::read(&path).map_err(|e| format!("{}: {e}", path.display()))
    };
    let english = read("en")?;
    let needle = english.get(offset..offset + length).ok_or("address beyond the English ROM")?.to_vec();
    for lang in ["ja", "de", "es", "fr", "it"] {
        let scaffold = root.join(format!("recon/{game}/{lang}/rom.s"));
        let text = std::fs::read_to_string(&scaffold).map_err(|e| e.to_string())?;
        let rom = read(lang)?;
        let (at, how) = if function {
            let extents = crate::adopt_data::extents(&text).map_err(|e| format!("{}: {e}", scaffold.display()))?;
            let expected = crate::adopt::expected_place(root, game, lang, address)?;
            match copy_of(&needle, address, false, &rom, &extents, expected) {
                Some(copy) => (copy.at, format!(" ({}{})", copy.class, if copy.found > 1 { format!(", nearest of {}", copy.found) } else { String::new() })),
                None => {
                    println!("{game}-{lang}: no identical or relocated copy in its scaffold");
                    continue;
                }
            }
        } else {
            let hits = find_all(&rom, &needle);
            let Some(&at) = hits.iter().min_by_key(|&&h| h.abs_diff(offset)) else {
                println!("{game}-{lang}: bytes not found");
                continue;
            };
            (at, if hits.len() > 1 { format!(" (nearest of {})", hits.len()) } else { String::new() })
        };
        match split_at(&text, at as u32, name, function) {
            Some(new) => {
                std::fs::write(&scaffold, new).map_err(|e| e.to_string())?;
                println!("{game}-{lang}: {name} at {at:#x}{how}");
            }
            None => println!("{game}-{lang}: {name} already labelled, or {at:#x} is not scaffold"),
        }
    }
    Ok(())
}

#[cfg(test)]
pub(crate) mod tests {
    use super::*;

    #[test]
    fn splits_the_covering_incbin() {
        let text = "\t.section .rom.00000100, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000040\n";
        let new = split_at(text, 0x120, "Table", false).unwrap();
        assert_eq!(new, "\t.section .rom.00000100, \"ax\"\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000020\n\t.global Table\nTable:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000020\n");
        assert!(split_at(&new, 0x120, "Table", false).is_none());
        assert_eq!(find_all(&[1, 2, 3, 1, 2, 3, 1, 2], &[1, 2]), [0, 6]);
        let code = split_at(text, 0x100, "Run", true).unwrap();
        assert_eq!(code, "\t.section .rom.00000100, \"ax\"\n\t.global Run\n\t.type Run, %function\n\t.thumb_func\nRun:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000040\n");
    }

    /// A Thumb function `at` bytes into its image: push {lr}, `filler`
    /// times adds r0, #value, a call `reach` halfwords ahead, pop {pc},
    /// and a pool word after a load of it.
    pub(crate) fn function(filler: usize, value: u8, reach: u16, word: u32) -> Vec<u8> {
        let mut code: Vec<u16> = vec![0xb500];
        code.extend(std::iter::repeat_n(0x3000 | u16::from(value), filler));
        // ldr r1, [pc, #0] reads the word after the return, on a 4-byte boundary.
        code.extend([0xf000, 0xf800 | reach, 0x4900, 0xbd00]);
        if code.len() % 2 == 1 {
            code.insert(1, 0x3000 | u16::from(value));
        }
        let mut bytes: Vec<u8> = code.iter().flat_map(|half| half.to_le_bytes()).collect();
        bytes.extend(word.to_le_bytes());
        bytes
    }

    #[test]
    fn finds_a_copy_whose_calls_and_pool_words_moved() {
        let english = function(10, 1, 0x40, 0x0801_0000);
        let twin = function(10, 2, 0x40, 0x0801_0000);
        let moved = function(10, 1, 0x58, 0x0801_0400);
        assert_ne!(english, moved);
        let mut rom = vec![0xffu8; 0x100];
        rom.extend(&twin);
        rom.extend(&moved);
        rom.extend(&moved);
        rom.extend([0xff; 0x40]);
        let whole = [(0, rom.len() as u32)];
        let first = 0x100 + twin.len();
        let copy = copy_of(&english, 0x0800_0200, false, &rom, &whole, first).unwrap();
        assert_eq!(copy, Copy { at: first, class: "relocated", found: 2 });
        // The nearer of two copies wins, and bytes outside the scaffold are not searched.
        let second = first + moved.len();
        assert_eq!(copy_of(&english, 0x0800_0200, false, &rom, &whole, second + 2).unwrap().at, second);
        assert_eq!(copy_of(&english, 0x0800_0200, false, &rom, &[(second as u32, 0x60)], 0).unwrap().found, 1);
        assert_eq!(copy_of(&english, 0x0800_0200, false, &rom, &[(0, first as u32)], first), None);
        // The same bytes are an identical copy.
        let same = copy_of(&moved, 0x0800_0200, false, &rom, &whole, 0).unwrap();
        assert_eq!((same.at, same.class), (first, "identical"));
        // Code with no run of 16 unchanged bytes is weighed around the expected place.
        let short = function(2, 1, 0x40, 0x0801_0000);
        let mut rom = vec![0xffu8; 0x80];
        rom.extend(function(2, 1, 0x10, 0x0801_0800));
        rom.extend([0xff; 0x40]);
        let found = copy_of(&short, 0x0800_0200, false, &rom, &[(0, rom.len() as u32)], 0x90).unwrap();
        assert_eq!((found.at, found.class), (0x80, "relocated"));
    }
}
