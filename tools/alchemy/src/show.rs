use crate::compiler::routing::root;
use crate::decode::{decode_one, half_at, word_at, Kind};
use crate::elf::Elf;
use crate::ld_map::{preferred, Map, Placed, ROM};
use crate::process::run as command;
use crate::targets::decomp_target;
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::path::Path;

const USAGE: &str = r#"usage: alchemy show EDITION FUNCTION [--arm]
After a build of EDITION (tbs-ja ... tla-it), prints FUNCTION (a symbol,
or its run-time address in hex) as out/EDITION/EDITION.gba holds it beside
the reference ROM roms/EDITION.gba at the same place, one
instruction a line, and marks with * each line whose bytes differ. Literal
pool words and jump tables print as .word, and calls, branches and pool
words that hit a symbol carry its name. The function ends at the next
symbol in its section, a static function the ELF names or its ELF size, or
the section end. Code that runs from IWRAM is read where the ROM holds it.
ARM code is disassembled with arm-none-eabi-objdump. Use --arm for an ARM
scaffold whose incbin has no instruction-set mapping symbols; this affects
only the displayed disassembly, never compilation or the count."#;

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct Line {
    pub address: u32,
    pub bytes: Vec<u8>,
    pub text: String,
    pub wide: bool,
}

impl Line {
    fn hex(&self) -> String {
        match self.bytes.len() {
            4 if self.wide => format!("{:08x}", u32::from_le_bytes(self.bytes[..4].try_into().unwrap())),
            _ => self
                .bytes
                .chunks(2)
                .map(|c| if c.len() == 2 { format!("{:04x}", u16::from_le_bytes([c[0], c[1]])) } else { format!("{:02x}", c[0]) })
                .collect::<Vec<_>>()
                .join(" "),
        }
    }
}

pub(crate) type Names = BTreeMap<u32, String>;

fn names(map: &Map) -> Names {
    let mut names = Names::new();
    for p in &map.placed {
        for (address, _) in &p.symbols {
            if let Some(name) = preferred(&p.symbols, *address) {
                names.entry(*address).or_insert_with(|| name.to_string());
            }
        }
    }
    names
}

fn named(names: &Names, value: u32) -> String {
    names
        .get(&value)
        .or_else(|| names.get(&(value & !1)).filter(|_| value & 1 == 1))
        .map_or(String::new(), |n| format!(" <{n}>"))
}

fn unknown(half: u16) -> String {
    match half >> 8 {
        0xdf => format!("swi {:#x}", half & 0xff),
        0xbe => format!("bkpt {:#x}", half & 0xff),
        _ if half & 0xf800 == 0xf800 => match half & 0x7ff {
            0 => "bl lr".to_string(),
            offset => format!("bl lr+{:#x}", offset << 1),
        },
        _ => format!(".2byte {half:#06x}"),
    }
}

pub(crate) fn thumb_lines(image: &[u8], base: u32, start: u32, end: u32, names: &Names) -> Vec<Line> {
    let mut lines = Vec::new();
    let mut pool = BTreeSet::new();
    let mut tables = BTreeSet::new();
    let at = |address: u32| address.wrapping_sub(base) as usize;
    let mut pc = start;
    while pc < end {
        if tables.remove(&pc) {
            let mut lowest = end;
            while pc + 4 <= end && pc < lowest {
                let Some(word) = word_at(image, at(pc)) else { break };
                if word <= pc || word >= end {
                    break;
                }
                lowest = lowest.min(word & !1);
                lines.push(Line { address: pc, bytes: image[at(pc)..at(pc) + 4].to_vec(), text: format!(".word {word:#010x}"), wide: true });
                pc += 4;
            }
            continue;
        }
        if pool.contains(&pc) && pc + 4 <= end {
            if let Some(word) = word_at(image, at(pc)) {
                lines.push(Line { address: pc, bytes: image[at(pc)..at(pc) + 4].to_vec(), text: format!(".word {word:#010x}{}", named(names, word)), wide: true });
                pc += 4;
                continue;
            }
        }
        let Some(half) = half_at(image, at(pc)) else { break };
        let decoded = decode_one(image, base, pc).filter(|ins| pc + ins.size <= end);
        let (size, text) = match decoded {
            None => (2, unknown(half)),
            Some(ins) => match ins.kind {
                Kind::LdrPool { word, .. } => {
                    let target = (pc & !3) + 4 + u32::from(half & 0xff) * 4;
                    if target < end {
                        pool.insert(target);
                        if word > pc && word < end && word & 3 == 0 {
                            tables.insert(word);
                        }
                    }
                    (2, format!("{}{}", ins.text, named(names, word)))
                }
                Kind::Bl { target } | Kind::B { target } | Kind::Bcond { target, .. } => (ins.size, format!("{}{}", ins.text, named(names, target))),
                Kind::Unknown(half) => (2, unknown(half)),
                Kind::SpAdjust(k) if k > 0 => (2, format!("add sp, #{k}")),
                _ => (ins.size, ins.text),
            },
        };
        lines.push(Line { address: pc, bytes: image[at(pc)..at(pc) + size as usize].to_vec(), text, wide: false });
        pc += size;
    }
    lines
}

fn arm_lines(file: &Path, image: &[u8], base: u32, start: u32, end: u32) -> Result<Vec<Line>, String> {
    let file = file.to_string_lossy();
    let output = command(
        &[
            "arm-none-eabi-objdump",
            "-D",
            "-b",
            "binary",
            "-marm",
            &format!("--adjust-vma={base:#x}"),
            &format!("--start-address={start:#x}"),
            &format!("--stop-address={end:#x}"),
            &file,
        ],
        root(),
    )?;
    Ok(parse_objdump(&output, image, base))
}

pub(crate) fn parse_objdump(output: &str, image: &[u8], base: u32) -> Vec<Line> {
    output
        .lines()
        .filter_map(|line| {
            let (address, rest) = line.trim_start().split_once(":\t")?;
            let address = u32::from_str_radix(address, 16).ok()?;
            let mut fields = rest.split('\t');
            let raw = fields.next()?.trim();
            let text: Vec<&str> = fields.map(str::trim).filter(|f| !f.is_empty() && !f.starts_with(';') && !f.starts_with('@')).collect();
            let offset = address.wrapping_sub(base) as usize;
            let bytes = image.get(offset..offset + raw.len() / 2)?.to_vec();
            Some(Line { address, bytes, text: text.join(" "), wide: true })
        })
        .collect()
}

pub(crate) fn pair<'a>(built: &'a [Line], reference: &'a [Line]) -> Vec<(Option<&'a Line>, Option<&'a Line>)> {
    let (mut i, mut j, mut rows) = (0, 0, Vec::new());
    while i < built.len() || j < reference.len() {
        match (built.get(i), reference.get(j)) {
            (Some(b), Some(r)) if b.address == r.address => {
                rows.push((Some(b), Some(r)));
                i += 1;
                j += 1;
            }
            (Some(b), r) if r.is_none_or(|r| b.address < r.address) => {
                rows.push((Some(b), None));
                i += 1;
            }
            (_, r) => {
                rows.push((None, r));
                j += 1;
            }
        }
    }
    rows
}

pub(crate) fn render(built: &[Line], reference: &[Line]) -> (String, usize, usize) {
    let rows = pair(built, reference);
    let mut out = String::new();
    let mut differ = 0;
    let side = |l: Option<&Line>| l.map_or(String::new(), |l| format!("{:<9} {}", l.hex(), l.text));
    for (b, r) in &rows {
        let same = matches!((b, r), (Some(b), Some(r)) if b.bytes == r.bytes);
        differ += usize::from(!same);
        let address = b.or(*r).map_or(0, |l| l.address);
        let _ = writeln!(out, "{} {address:08x}  {:<46}  {}", if same { ' ' } else { '*' }, side(*b), side(*r));
    }
    (out.lines().map(str::trim_end).collect::<Vec<_>>().join("\n") + "\n", differ, rows.len())
}

pub(crate) fn function_end(section: &Placed, start: u32, statics: &[u32], size: Option<u32>) -> u32 {
    let mut end = section.end();
    for &address in section.symbols.iter().map(|(a, _)| a).chain(statics) {
        if address > start && address < end {
            end = address;
        }
    }
    match size.filter(|&s| s > 0) {
        Some(size) => end.min(start + size),
        None => end,
    }
}

const STT_FUNC: u8 = 2;

fn is_mapping(name: &str) -> bool {
    name.starts_with('$')
}

pub(crate) fn resolve<'m>(map: &'m Map, elf: Option<&Elf>, functions: &[(u32, String)], wanted: &str) -> Result<(&'m Placed, u32, String), String> {
    if let Some((section, start)) = map.symbol(wanted) {
        return Ok((section, start, wanted.to_string()));
    }
    let in_section = |address: u32| map.at_address(address).ok_or_else(|| format!("{address:#010x} is in no linked section"));
    let from_elf = functions.iter().find(|(_, n)| n == wanted).map(|(a, _)| *a).or_else(|| {
        elf.and_then(|e| e.symbols.iter().find(|s| s.name == wanted && !is_mapping(&s.name)).map(|s| s.value & !1))
    });
    if let Some(start) = from_elf {
        return Ok((in_section(start)?, start, wanted.to_string()));
    }
    let digits = wanted.trim_start_matches("0x");
    let address = (digits.len() == 8)
        .then(|| u32::from_str_radix(digits, 16).ok())
        .flatten()
        .ok_or_else(|| format!("no symbol {wanted} in the map or the ELF"))?;
    let section = in_section(address)?;
    let start = section
        .symbols
        .iter()
        .map(|(a, _)| *a)
        .chain(functions.iter().map(|(a, _)| *a).filter(|&a| a >= section.address))
        .filter(|&a| a <= address)
        .max()
        .unwrap_or(address);
    let name = preferred(&section.symbols, start)
        .map(str::to_string)
        .or_else(|| functions.iter().find(|(a, _)| *a == start).map(|(_, n)| n.clone()))
        .unwrap_or_else(|| format!("{start:#010x}"));
    Ok((section, start, name))
}

fn is_arm(elf: Option<&Elf>, section: &Placed, name: &str, start: u32) -> bool {
    let Some(elf) = elf else { return false };
    if let Some(symbol) = elf.symbols.iter().find(|s| s.name == name && s.kind == STT_FUNC) {
        return symbol.value & 1 == 0;
    }
    elf.symbols
        .iter()
        .filter(|s| (s.name.starts_with("$a") || s.name.starts_with("$t")) && s.value >= section.address && s.value <= start)
        .max_by_key(|s| s.value)
        .is_some_and(|s| s.name.starts_with("$a"))
}

fn parse(arguments: &[String]) -> Result<(&str, &str, bool), String> {
    let positional: Vec<&String> = arguments.iter().filter(|a| !a.starts_with('-')).collect();
    if positional.len() != 2 || arguments.iter().any(|a| a.starts_with('-') && a != "--arm") {
        return Err(USAGE.into());
    }
    Ok((positional[0].as_str(), positional[1].as_str(), arguments.iter().any(|a| a == "--arm")))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let (edition, wanted, force_arm) = parse(arguments)?;
    let target = decomp_target(Some(edition))?;
    let out = root().join(target.output_dir);
    let read = |name: &str| std::fs::read(out.join(name)).map_err(|e| format!("{}: {e}; build {edition} first", out.join(name).display()));
    let map = Map::parse(&String::from_utf8_lossy(&read(&format!("{edition}.map"))?));
    let built = read(&format!("{edition}.gba"))?;
    let reference_path = root().join(target.rom);
    let reference = std::fs::read(&reference_path).map_err(|e| format!("{}: {e}", reference_path.display()))?;
    let elf = read(&format!("{edition}.elf")).ok().and_then(|data| Elf::parse(&data).ok());
    let functions: Vec<(u32, String)> = elf
        .iter()
        .flat_map(|e| e.symbols.iter())
        .filter(|s| s.kind == STT_FUNC && !is_mapping(&s.name))
        .map(|s| (s.value & !1, s.name.clone()))
        .collect();
    let (section, start, name) = resolve(&map, elf.as_ref(), &functions, wanted)?;
    if !section.loaded || section.load < ROM {
        return Err(format!("{name} is in {}, which has no bytes in the ROM", section.label()));
    }
    let statics: Vec<u32> = functions.iter().map(|(a, _)| *a).collect();
    let size = elf.as_ref().and_then(|e| e.symbols.iter().find(|s| s.name == name && s.kind == STT_FUNC).map(|s| s.size));
    let end = function_end(section, start, &statics, size);
    let offset = section.load - ROM + (start - section.address);
    let base = start.wrapping_sub(offset);
    if (offset + (end - start)) as usize > built.len().min(reference.len()) {
        return Err(format!("{name} runs past the end of an image"));
    }
    let arm = force_arm || is_arm(elf.as_ref(), section, &name, start);
    if arm && (start & 3 != 0 || end & 3 != 0) {
        return Err(format!("{name}: ARM disassembly needs word-aligned bounds"));
    }
    let names = names(&map);
    let (ours, theirs) = if arm {
        (arm_lines(&out.join(format!("{edition}.gba")), &built, base, start, end)?, arm_lines(&reference_path, &reference, base, start, end)?)
    } else {
        (thumb_lines(&built, base, start, end, &names), thumb_lines(&reference, base, start, end, &names))
    };
    let loaded = if section.load == section.address { String::new() } else { format!(", loaded from {:#010x}", ROM + offset) };
    println!(
        "{edition} {name}: {start:#010x}..{end:#010x}, {:#x} bytes of {}{loaded}, in {}",
        end - start,
        if arm { "ARM" } else { "Thumb" },
        section.label()
    );
    println!("  address   {:<46}  reference", "built");
    let (listing, differ, rows) = render(&ours, &theirs);
    print!("{listing}");
    let range = offset as usize..(offset + end - start) as usize;
    match range.clone().find(|&k| built[k] != reference[k]) {
        None => println!("identical to the reference"),
        Some(first) => {
            println!("{differ} of {rows} lines differ");
            if let Some(shift) = crate::objects::apparent_shift(&reference, &built, first, range.end) {
                println!(
                    "from {:#010x} on the reference holds the same bytes {} bytes {}: the code looks placed {} bytes {}",
                    start + (first - range.start) as u32,
                    shift.abs(),
                    if shift > 0 { "later" } else { "earlier" },
                    shift.abs(),
                    if shift > 0 { "early" } else { "late" }
                );
            }
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn explicit_arm_mode_is_for_display_only() {
        let args = ["tbs-ja", "Glyph", "--arm"].map(String::from);
        assert_eq!(parse(&args).unwrap(), ("tbs-ja", "Glyph", true));
        assert_eq!(parse(&args[..2]).unwrap(), ("tbs-ja", "Glyph", false));
        let invalid = ["tbs-ja", "Glyph", "--typo"].map(String::from);
        assert!(parse(&invalid).is_err());
        assert!(parse(&["--help".into()]).is_err());
    }

    fn halves(words: &[u16]) -> Vec<u8> {
        words.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    fn texts(lines: &[Line]) -> Vec<String> {
        lines.iter().map(|l| format!("{:x} {}", l.address, l.text)).collect()
    }

    #[test]
    fn reads_thumb_with_pools_calls_and_names() {
        let mut image = halves(&[
            0xb510,
            0x4802,
            0xf000, 0xf804,
            0xbd10,
            0xf800,
        ]);
        image.extend(0x0300_0164u32.to_le_bytes());
        image.extend(halves(&[0x4770, 0x0000]));
        let names: Names = [(0x0800_0010, "Next".to_string()), (0x0300_0164, "gWork".to_string())].into();
        let lines = thumb_lines(&image, ROM, ROM, ROM + 0x10, &names);
        assert_eq!(
            texts(&lines),
            [
                "8000000 push {r4, lr}",
                "8000002 ldr r0, =0x3000164 <gWork>",
                "8000004 bl 0x08000010 <Next>",
                "8000008 pop {r4, pc}",
                "800000a bl lr",
                "800000c .word 0x03000164 <gWork>",
            ]
        );
        assert_eq!(lines[2].hex(), "f000 f804");
        assert_eq!(lines[5].hex(), "03000164");
    }

    #[test]
    fn a_jump_table_reads_as_words() {
        let mut image = halves(&[0x4a05, 0x00bb, 0x589b, 0x469f]);
        image.extend(0x0800_0010u32.to_le_bytes());
        image.extend(0x0800_0014u32.to_le_bytes());
        image.extend(halves(&[0x2001, 0x4770, 0x2002, 0x4770]));
        image.extend(0x0800_0008u32.to_le_bytes());
        let lines = thumb_lines(&image, ROM, ROM, ROM + 0x1c, &Names::new());
        assert_eq!(
            texts(&lines),
            [
                "8000000 ldr r2, =0x8000008",
                "8000002 lsls r3, r7, #2",
                "8000004 ldr r3, [r3, r2]",
                "8000006 mov pc, r3",
                "8000008 .word 0x08000010",
                "800000c .word 0x08000014",
                "8000010 movs r0, #1",
                "8000012 bx lr",
                "8000014 movs r0, #2",
                "8000016 bx lr",
                "8000018 .word 0x08000008",
            ]
        );
    }

    #[test]
    fn an_address_stands_for_the_function_that_holds_it() {
        let map = Map::parse(crate::ld_map::tests::MAP);
        let functions = vec![(0x0800_0014, "Input_Static".to_string())];
        let found = |wanted: &str| resolve(&map, None, &functions, wanted).map(|(p, a, n)| (p.label(), a, n));
        let io = "games/tbs/src/system/io.o".to_string();
        assert_eq!(found("Input_Poll"), Ok((io.clone(), 0x0800_0018, "Input_Poll".to_string())));
        assert_eq!(found("08000012"), Ok((io.clone(), 0x0800_0010, "Input_Handle".to_string())));
        assert_eq!(found("0x08000016"), Ok((io.clone(), 0x0800_0014, "Input_Static".to_string())));
        assert_eq!(found("Input_Static"), Ok((io, 0x0800_0014, "Input_Static".to_string())));
        assert!(found("Missing").is_err());
    }

    #[test]
    fn rows_pair_by_address_and_mark_differences() {
        let built = thumb_lines(&halves(&[0xb500, 0xf000, 0xf801, 0xbd00]), ROM, ROM, ROM + 8, &Names::new());
        let reference = thumb_lines(&halves(&[0xb500, 0x2001, 0x2002, 0xbd00]), ROM, ROM, ROM + 8, &Names::new());
        let (text, differ, rows) = render(&built, &reference);
        assert_eq!((differ, rows), (2, 4));
        let lines: Vec<&str> = text.lines().collect();
        assert!(lines[0].starts_with("  08000000  b500      push {lr}"), "{text}");
        assert!(lines[1].starts_with("* 08000002  f000 f801 bl 0x08000008") && lines[1].ends_with("2001      movs r0, #1"), "{text}");
        assert!(lines[2].starts_with("* 08000004  ") && lines[2].ends_with("2002      movs r0, #2"), "{text}");
        assert!(lines[3].starts_with("  08000006  bd00"), "{text}");
    }

    #[test]
    fn a_function_ends_at_the_next_symbol_in_its_own_section() {
        let section = Placed {
            name: ".text".into(),
            path: "/x/obj/games/A.o".into(),
            address: 0x0800_0100,
            size: 0x40,
            load: 0x0800_0100,
            loaded: true,
            symbols: vec![(0x0800_0100, "First".into()), (0x0800_0120, "Second".into())],
        };
        assert_eq!(function_end(&section, 0x0800_0100, &[], None), 0x0800_0120);
        assert_eq!(function_end(&section, 0x0800_0120, &[0x0800_0200], None), 0x0800_0140);
        assert_eq!(function_end(&section, 0x0800_0100, &[0x0800_0110], None), 0x0800_0110);
        assert_eq!(function_end(&section, 0x0800_0100, &[], Some(0x18)), 0x0800_0118);
    }

    #[test]
    fn reads_objdump_arm_lines() {
        let image: Vec<u8> = [0xe0u8, 0x01, 0x2d, 0xe9, 0xff, 0x54, 0xa0, 0xe3].to_vec();
        let output = "\n0800a37c <.data+0xa37c>:\n 800a37c:\te92d01e0 \tpush\t{r5, r6, r7, r8}\n 800a380:\te3a054ff \tmov\tr5, #-16777216\t; 0xff000000\n";
        let lines = parse_objdump(output, &image, 0x0800_a37c);
        assert_eq!(texts(&lines), ["800a37c push {r5, r6, r7, r8}", "800a380 mov r5, #-16777216"]);
        assert_eq!(lines[1].hex(), "e3a054ff");
    }
}
