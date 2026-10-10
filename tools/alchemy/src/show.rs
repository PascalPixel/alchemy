use crate::compiler::routing::root;
use crate::decode::{decode_one, half_at, word_at, Kind};
use crate::elf::Elf;
use crate::ld_map::{preferred, Map, Placed, ROM};
use crate::process::run as command;
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::path::Path;

const USAGE: &str = r#"usage: alchemy show EDITION FUNCTION [--arm] [--overlay resource_XXX] [--baseline DIRECTORY]
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
only the displayed disassembly, never compilation or the count.
--overlay compares the linked overlay ELF with that resource decoded from
the reference ROM, including the loader's Thumb-call restoration.
--baseline names a saved, byte-identical edition output directory. Its map
and ELF preserve the reference function's bounds when a trial changes size
or placement. The saved ROM must match the selected edition's reference.
An aligned instruction summary uses diff's normalization and LCS distance;
the displayed bytes remain the authority for the comparison."#;

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
            4 if self.wide => format!(
                "{:08x}",
                u32::from_le_bytes(self.bytes[..4].try_into().unwrap())
            ),
            _ => self
                .bytes
                .chunks(2)
                .map(|c| {
                    if c.len() == 2 {
                        format!("{:04x}", u16::from_le_bytes([c[0], c[1]]))
                    } else {
                        format!("{:02x}", c[0])
                    }
                })
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

pub(crate) fn thumb_lines(
    image: &[u8],
    base: u32,
    start: u32,
    end: u32,
    names: &Names,
) -> Vec<Line> {
    let mut lines = Vec::new();
    let mut pool = BTreeSet::new();
    let mut tables = BTreeSet::new();
    let at = |address: u32| address.wrapping_sub(base) as usize;
    let mut pc = start;
    while pc < end {
        if tables.remove(&pc) {
            let mut lowest = end;
            while pc + 4 <= end && pc < lowest {
                let Some(word) = word_at(image, at(pc)) else {
                    break;
                };
                if word <= pc || word >= end {
                    break;
                }
                lowest = lowest.min(word & !1);
                lines.push(Line {
                    address: pc,
                    bytes: image[at(pc)..at(pc) + 4].to_vec(),
                    text: format!(".word {word:#010x}"),
                    wide: true,
                });
                pc += 4;
            }
            continue;
        }
        if pool.contains(&pc) && pc + 4 <= end {
            if let Some(word) = word_at(image, at(pc)) {
                lines.push(Line {
                    address: pc,
                    bytes: image[at(pc)..at(pc) + 4].to_vec(),
                    text: format!(".word {word:#010x}{}", named(names, word)),
                    wide: true,
                });
                pc += 4;
                continue;
            }
        }
        let Some(half) = half_at(image, at(pc)) else {
            break;
        };
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
                Kind::Bl { target } | Kind::B { target } | Kind::Bcond { target, .. } => {
                    (ins.size, format!("{}{}", ins.text, named(names, target)))
                }
                Kind::Unknown(half) => (2, unknown(half)),
                Kind::SpAdjust(k) if k > 0 => (2, format!("add sp, #{k}")),
                _ => (ins.size, ins.text),
            },
        };
        lines.push(Line {
            address: pc,
            bytes: image[at(pc)..at(pc) + size as usize].to_vec(),
            text,
            wide: false,
        });
        pc += size;
    }
    lines
}

fn arm_lines(
    file: &Path,
    image: &[u8],
    base: u32,
    start: u32,
    end: u32,
) -> Result<Vec<Line>, String> {
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
            let text: Vec<&str> = fields
                .map(str::trim)
                .filter(|f| !f.is_empty() && !f.starts_with(';') && !f.starts_with('@'))
                .collect();
            let offset = address.wrapping_sub(base) as usize;
            let bytes = image.get(offset..offset + raw.len() / 2)?.to_vec();
            Some(Line {
                address,
                bytes,
                text: text.join(" "),
                wide: true,
            })
        })
        .collect()
}

pub(crate) fn pair<'a>(
    built: &'a [Line],
    reference: &'a [Line],
) -> Vec<(Option<&'a Line>, Option<&'a Line>)> {
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
        let _ = writeln!(
            out,
            "{} {address:08x}  {:<46}  {}",
            if same { ' ' } else { '*' },
            side(*b),
            side(*r)
        );
    }
    (
        out.lines()
            .map(str::trim_end)
            .collect::<Vec<_>>()
            .join("\n")
            + "\n",
        differ,
        rows.len(),
    )
}

pub(crate) fn function_end(
    section: &Placed,
    start: u32,
    statics: &[u32],
    size: Option<u32>,
) -> u32 {
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

pub(crate) fn resolve<'m>(
    map: &'m Map,
    elf: Option<&Elf>,
    functions: &[(u32, String)],
    wanted: &str,
) -> Result<(&'m Placed, u32, String), String> {
    if let Some((section, start)) = map.symbol(wanted) {
        return Ok((section, start, wanted.to_string()));
    }
    let in_section = |address: u32| {
        map.at_address(address)
            .ok_or_else(|| format!("{address:#010x} is in no linked section"))
    };
    let from_elf = functions
        .iter()
        .find(|(_, n)| n == wanted)
        .map(|(a, _)| *a)
        .or_else(|| {
            elf.and_then(|e| {
                e.symbols
                    .iter()
                    .find(|s| s.name == wanted && !is_mapping(&s.name))
                    .map(|s| s.value & !1)
            })
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
        .chain(
            functions
                .iter()
                .map(|(a, _)| *a)
                .filter(|&a| a >= section.address),
        )
        .filter(|&a| a <= address)
        .max()
        .unwrap_or(address);
    let name = preferred(&section.symbols, start)
        .map(str::to_string)
        .or_else(|| {
            functions
                .iter()
                .find(|(a, _)| *a == start)
                .map(|(_, n)| n.clone())
        })
        .unwrap_or_else(|| format!("{start:#010x}"));
    Ok((section, start, name))
}

fn is_arm(elf: Option<&Elf>, section: &Placed, name: &str, start: u32) -> bool {
    let Some(elf) = elf else { return false };
    if let Some(symbol) = elf
        .symbols
        .iter()
        .find(|s| s.name == name && s.kind == STT_FUNC)
    {
        return symbol.value & 1 == 0;
    }
    elf.symbols
        .iter()
        .filter(|s| {
            (s.name.starts_with("$a") || s.name.starts_with("$t"))
                && s.value >= section.address
                && s.value <= start
        })
        .max_by_key(|s| s.value)
        .is_some_and(|s| s.name.starts_with("$a"))
}

#[derive(Debug, PartialEq)]
struct Options<'a> {
    edition: &'a str,
    wanted: &'a str,
    arm: bool,
    overlay: Option<String>,
    baseline: Option<&'a str>,
}

fn parse(arguments: &[String]) -> Result<Options<'_>, String> {
    let mut positional = Vec::new();
    let (mut arm, mut overlay, mut baseline) = (false, None, None);
    let mut args = arguments.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "--arm" => arm = true,
            "--overlay" if overlay.is_none() => {
                let name = args.next().ok_or(USAGE)?;
                let digits = name.strip_prefix("resource_").ok_or(USAGE)?;
                if digits.is_empty() || !digits.bytes().all(|byte| byte.is_ascii_hexdigit()) {
                    return Err(USAGE.into());
                }
                let id = u32::from_str_radix(digits, 16).map_err(|_| USAGE)?;
                overlay = Some(format!("resource_{id:03x}"));
            }
            "--baseline" if baseline.is_none() => {
                baseline = Some(args.next().ok_or(USAGE)?.as_str())
            }
            option if option.starts_with('-') => return Err(USAGE.into()),
            _ => positional.push(arg.as_str()),
        }
    }
    if positional.len() != 2 || (arm && overlay.is_some()) {
        return Err(USAGE.into());
    }
    Ok(Options {
        edition: positional[0],
        wanted: positional[1],
        arm,
        overlay,
        baseline,
    })
}

struct LinkedImage {
    map: Map,
    elf: Option<Elf>,
    bytes: Vec<u8>,
    overlay_base: Option<u32>,
}

impl LinkedImage {
    fn read(directory: &Path, edition: &str, overlay: Option<&str>) -> Result<Self, String> {
        let prefix = overlay.map_or_else(
            || directory.join(edition),
            |id| directory.join("overlays").join(id),
        );
        let read = |extension: &str| {
            let path = prefix.with_extension(extension);
            std::fs::read(&path)
                .map_err(|error| format!("{}: {error}; build {edition} first", path.display()))
        };
        let map = Map::parse(&String::from_utf8_lossy(&read("map")?));
        let elf = read("elf").ok().map(|data| Elf::parse(&data)).transpose()?;
        let (bytes, overlay_base) = if overlay.is_some() {
            let elf = elf
                .as_ref()
                .ok_or("an overlay comparison needs its linked ELF")?;
            let (base, bytes) = overlay_bytes(elf)?;
            (bytes, Some(base))
        } else {
            (read("gba")?, None)
        };
        Ok(Self {
            map,
            elf,
            bytes,
            overlay_base,
        })
    }

    fn functions(&self) -> Vec<(u32, String)> {
        self.elf
            .iter()
            .flat_map(|elf| &elf.symbols)
            .filter(|symbol| {
                symbol.kind == STT_FUNC
                    && symbol.section != 0
                    && symbol.section != 0xfff1
                    && !is_mapping(&symbol.name)
            })
            .map(|symbol| (symbol.value & !1, symbol.name.clone()))
            .collect()
    }

    fn span(&self, wanted: &str) -> Result<(&Placed, u32, u32, u32, String), String> {
        let functions = self.functions();
        let (section, start, name) = resolve(&self.map, self.elf.as_ref(), &functions, wanted)?;
        let statics = functions
            .iter()
            .map(|(address, _)| *address)
            .collect::<Vec<_>>();
        let size = self.elf.as_ref().and_then(|elf| {
            elf.symbols
                .iter()
                .find(|symbol| symbol.name == name && symbol.kind == STT_FUNC)
                .map(|symbol| symbol.size)
        });
        let end = function_end(section, start, &statics, size);
        let base = match self.overlay_base {
            Some(base) => base,
            None if section.loaded && section.load >= ROM => {
                section.address.wrapping_sub(section.load - ROM)
            }
            None => {
                return Err(format!(
                    "{name} is in {}, which has no bytes in the ROM",
                    section.label()
                ))
            }
        };
        Ok((section, start, end, base, name))
    }
}

pub(crate) struct OverlayReference {
    pub bytes: Vec<u8>,
    pub start: u32,
    pub end: u32,
    pub name: String,
}

fn overlay_reference_code(built: &LinkedImage, reference: &[u8], wanted: &str) -> Result<OverlayReference, String> {
    let (section, start, end, base, name) = built.span(wanted)?;
    if !section.name.starts_with(".text") || end <= start {
        return Err(format!("{name}: no overlay code extent"));
    }
    let range = image_range(reference.len(), base, start, end)?;
    Ok(OverlayReference { bytes: reference[range].to_vec(), start, end, name })
}

/// Analysis only: the linked image supplies bounds, the ROM supplies code.
pub(crate) fn overlay_function_reference(target: DecompTarget, overlay: &str, wanted: &str) -> Result<OverlayReference, String> {
    let out = root().join(target.output_dir);
    let built = LinkedImage::read(&out, target.id.as_str(), Some(overlay))?;
    let map_path = out.join(format!("{}.map", target.id));
    let main_map = Map::parse(&std::fs::read_to_string(&map_path)
        .map_err(|error| format!("{}: {error}", map_path.display()))?);
    let rom_path = root().join(target.rom);
    let rom = std::fs::read(&rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    target.verify_reference(&rom)?;
    let reference = reference_overlay(&rom, &main_map, overlay)?;
    overlay_reference_code(&built, &reference, wanted)
}

fn overlay_bytes(elf: &Elf) -> Result<(u32, Vec<u8>), String> {
    let base = elf
        .sections
        .iter()
        .map(|section| section.address)
        .min()
        .ok_or("overlay ELF has no placed bytes")?;
    let end = elf
        .sections
        .iter()
        .map(|section| {
            section
                .address
                .checked_add(section.bytes.len() as u32)
                .ok_or("overlay section address overflows")
        })
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .max()
        .unwrap();
    let size = end
        .checked_sub(base)
        .filter(|size| *size <= 0x100000)
        .ok_or("overlay ELF exceeds the diagnostic image limit")?;
    let mut image = vec![0; size as usize];
    for section in &elf.sections {
        let at = (section.address - base) as usize;
        image[at..at + section.bytes.len()].copy_from_slice(&section.bytes);
    }
    Ok((base, image))
}

fn reference_overlay(rom: &[u8], main_map: &Map, id: &str) -> Result<Vec<u8>, String> {
    let index = usize::from_str_radix(
        id.strip_prefix("resource_").ok_or("invalid overlay id")?,
        16,
    )
    .map_err(|_| "invalid overlay id")?;
    let (section, address) = main_map
        .symbol("Resource_DirectoryTable")
        .ok_or("the main map has no Resource_DirectoryTable")?;
    if !section.loaded || section.load < ROM {
        return Err("resource directory has no ROM placement".into());
    }
    let table = section
        .load
        .checked_sub(ROM)
        .and_then(|offset| offset.checked_add(address - section.address))
        .ok_or("resource directory offset overflows")? as usize;
    let word = |at: usize| {
        rom.get(at..at + 4)
            .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()))
            .ok_or("resource directory is truncated")
    };
    if word(table + 4)? != address {
        return Err("reference resource directory disagrees with the main map".into());
    }
    let table_end = word(table + 8)?
        .checked_sub(ROM)
        .ok_or("resource directory end is outside the ROM")? as usize;
    if table_end <= table + 12 || table_end > rom.len() || !(table_end - table).is_multiple_of(4) {
        return Err("reference resource directory has invalid bounds".into());
    }
    let count = (table_end - table) / 4;
    if index >= count {
        return Err(format!("{id} is outside the reference resource directory"));
    }
    let start = word(table + 4 * index)?
        .checked_sub(ROM)
        .ok_or("overlay stream is outside the ROM")? as usize;
    if start >= rom.len() {
        return Err("overlay stream is outside the ROM".into());
    }
    let end = (0..count)
        .filter_map(|index| {
            word(table + 4 * index)
                .ok()?
                .checked_sub(ROM)
                .map(|offset| offset as usize)
        })
        .filter(|&offset| offset > start && offset <= rom.len())
        .min()
        .unwrap_or(rom.len());
    let decoded = match rom[start] {
        0 => ags::assets::lz::decode_general(rom, start, end, 0x100000),
        1 => ags::assets::lz::decode_palette(rom, start + 1, end, 0x100000),
        kind => return Err(format!("overlay stream has unsupported kind {kind}")),
    };
    let (mut decoded, _) = decoded.map_err(|error| error.0)?;
    restore_thumb_calls(&mut decoded);
    Ok(decoded)
}

fn restore_thumb_calls(block: &mut [u8]) {
    let half = |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
    let mut low_at = 2;
    while low_at + 2 <= block.len() {
        let (low, high) = (half(block, low_at), half(block, low_at - 2));
        if low >> 11 == 31 && high >> 11 == 30 {
            let stored = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
            let call = stored.wrapping_sub(low_at as u32) & 0x7f_fffe;
            block[low_at - 2..low_at]
                .copy_from_slice(&((0xf000 | (call >> 12)) as u16).to_le_bytes());
            block[low_at..low_at + 2]
                .copy_from_slice(&((0xf800 | ((call >> 1) & 0x7ff)) as u16).to_le_bytes());
        }
        low_at += 2;
    }
}

fn aligned(built: &[Line], reference: &[Line]) -> (usize, usize, usize, usize) {
    let tokens = |lines: &[Line]| {
        let text = lines
            .iter()
            .map(|line| format!("{:x}:\t{}\n", line.address, line.text))
            .collect::<String>();
        crate::diff::normalize(&text)
    };
    let (ours, theirs) = (tokens(built), tokens(reference));
    let (common, _) = crate::diff::compare(&theirs, &ours);
    (
        theirs.len(),
        ours.len(),
        common,
        theirs.len() + ours.len() - 2 * common,
    )
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse(arguments)?;
    let (edition, wanted) = (options.edition, options.wanted);
    let target = decomp_target(Some(edition))?;
    let out = root().join(target.output_dir);
    let built = LinkedImage::read(&out, edition, options.overlay.as_deref())?;
    let reference_path = root().join(target.rom);
    let rom =
        std::fs::read(&reference_path).map_err(|e| format!("{}: {e}", reference_path.display()))?;
    let baseline_dir = options.baseline.map(Path::new);
    let baseline = baseline_dir
        .map(|directory| {
            let saved_rom = std::fs::read(directory.join(format!("{edition}.gba")))
                .map_err(|error| format!("baseline ROM: {error}"))?;
            target
                .verify_reference(&saved_rom)
                .map_err(|error| format!("baseline: {error}"))?;
            LinkedImage::read(directory, edition, options.overlay.as_deref())
        })
        .transpose()?;
    let native = baseline.as_ref().unwrap_or(&built);
    let (section, start, end, base, name) = built.span(wanted)?;
    let (native_section, native_start, native_end, native_base, _) = native.span(wanted)?;
    let directory = baseline_dir.unwrap_or(&out);
    let main_map = if options.overlay.is_some() {
        let path = directory.join(format!("{edition}.map"));
        Map::parse(
            &std::fs::read_to_string(&path)
                .map_err(|error| format!("{}: {error}", path.display()))?,
        )
    } else {
        Map::parse("")
    };
    let reference = match options.overlay.as_deref() {
        Some(id) => reference_overlay(&rom, &main_map, id)?,
        None => rom,
    };
    let range = image_range(built.bytes.len(), base, start, end)?;
    let native_range = image_range(reference.len(), native_base, native_start, native_end)?;
    let arm = options.arm || is_arm(built.elf.as_ref(), section, &name, start);
    if arm && options.overlay.is_some() {
        return Err("overlay display currently supports Thumb code".into());
    }
    if arm
        && [start, end, native_start, native_end]
            .iter()
            .any(|address| address & 3 != 0)
    {
        return Err(format!("{name}: ARM disassembly needs word-aligned bounds"));
    }
    let mut names = names(&main_map);
    names.extend(self::names(&built.map));
    let (ours, theirs) = if arm {
        (
            arm_lines(
                &out.join(format!("{edition}.gba")),
                &built.bytes,
                base,
                start,
                end,
            )?,
            arm_lines(
                &reference_path,
                &reference,
                native_base,
                native_start,
                native_end,
            )?,
        )
    } else {
        (
            thumb_lines(&built.bytes, base, start, end, &names),
            thumb_lines(&reference, native_base, native_start, native_end, &names),
        )
    };
    let loaded = if options.overlay.is_some() || section.load == section.address {
        String::new()
    } else {
        format!(", loaded from {:#010x}", ROM + range.start as u32)
    };
    println!(
        "{edition} {name}: {start:#010x}..{end:#010x}, {:#x} bytes of {}{loaded}, in {}",
        end - start,
        if arm { "ARM" } else { "Thumb" },
        section.label()
    );
    if (start, end, base) != (native_start, native_end, native_base) {
        println!(
            "reference bounds: {native_start:#010x}..{native_end:#010x}, in {}",
            native_section.label()
        );
    }
    println!("  address   {:<46}  reference", "built");
    let (listing, differ, rows) = render(&ours, &theirs);
    print!("{listing}");
    let (native_count, built_count, common, distance) = aligned(&ours, &theirs);
    println!("aligned instructions: reference {native_count}, built {built_count}, common {common}, differ {distance}");
    if built.bytes[range.clone()] == reference[native_range.clone()] {
        println!("identical to the reference");
    } else {
        println!("{differ} of {rows} displayed lines differ");
        if options.overlay.is_none() && range == native_range {
            let first = range
                .clone()
                .find(|&k| built.bytes[k] != reference[k])
                .unwrap();
            if let Some(shift) =
                crate::objects::apparent_shift(&reference, &built.bytes, first, range.end)
            {
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

fn image_range(
    length: usize,
    base: u32,
    start: u32,
    end: u32,
) -> Result<std::ops::Range<usize>, String> {
    let range = start
        .checked_sub(base)
        .zip(end.checked_sub(base))
        .filter(|&(start, end)| start <= end && end as usize <= length)
        .ok_or("function runs past the end of an image")?;
    Ok(range.0 as usize..range.1 as usize)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn explicit_arm_mode_is_for_display_only() {
        let args = ["tbs-ja", "Glyph", "--arm"].map(String::from);
        assert_eq!(
            parse(&args).unwrap(),
            Options {
                edition: "tbs-ja",
                wanted: "Glyph",
                arm: true,
                overlay: None,
                baseline: None
            }
        );
        assert!(!parse(&args[..2]).unwrap().arm);
        let invalid = ["tbs-ja", "Glyph", "--typo"].map(String::from);
        assert!(parse(&invalid).is_err());
        assert!(parse(&["--help".into()]).is_err());
    }

    #[test]
    fn overlay_and_baseline_options_are_explicit_and_validated() {
        let args = [
            "tbs-en",
            "Scene",
            "--overlay",
            "resource_03A5",
            "--baseline",
            "out/saved",
        ]
        .map(String::from);
        let options = parse(&args).unwrap();
        assert_eq!(options.overlay.as_deref(), Some("resource_3a5"));
        assert_eq!(options.baseline, Some("out/saved"));
        for tail in [
            vec!["--overlay"],
            vec!["--overlay", "resource_../x"],
            vec!["--baseline"],
            vec!["--overlay", "resource_3a5", "--arm"],
            vec!["--overlay", "resource_3a5", "--overlay", "resource_3a5"],
        ] {
            let args = [vec!["tbs-en", "Scene"], tail]
                .concat()
                .into_iter()
                .map(String::from)
                .collect::<Vec<_>>();
            assert!(parse(&args).is_err(), "{args:?}");
        }
    }

    #[test]
    fn loader_restoration_inverts_stored_calls_including_backwards_and_wrap() {
        for displacement in [0, 2, 0x1234, 0x3ffffe, 0x400000, 0x7ffffe] {
            let original = halves(&[
                0x2001,
                (0xf000 | (displacement >> 12)) as u16,
                (0xf800 | ((displacement >> 1) & 0x7ff)) as u16,
                0x4770,
            ]);
            let stored = (displacement + 4) & 0x7ffffe;
            let mut packed = halves(&[
                0x2001,
                (0xf000 | (stored >> 12)) as u16,
                (0xf800 | ((stored >> 1) & 0x7ff)) as u16,
                0x4770,
            ]);
            restore_thumb_calls(&mut packed);
            assert_eq!(packed, original);
        }
        let mut unrelated = halves(&[0x2001, 0xf800, 0xf000]);
        unrelated.push(0xab);
        let original = unrelated.clone();
        restore_thumb_calls(&mut unrelated);
        assert_eq!(unrelated, original);
    }

    #[test]
    fn overlay_image_preserves_section_gaps_and_rejects_overflow() {
        let elf = Elf {
            symbols: vec![],
            sections: vec![
                crate::elf::Section {
                    address: 0x2000,
                    bytes: vec![1, 2],
                },
                crate::elf::Section {
                    address: 0x2004,
                    bytes: vec![3, 4],
                },
            ],
        };
        assert_eq!(
            overlay_bytes(&elf).unwrap(),
            (0x2000, vec![1, 2, 0, 0, 3, 4])
        );
        let overflow = Elf {
            symbols: vec![],
            sections: vec![crate::elf::Section {
                address: u32::MAX,
                bytes: vec![1],
            }],
        };
        assert!(overlay_bytes(&overflow).is_err());
        assert!(image_range(6, 0x2000, 0x2004, 0x2008).is_err());
        assert!(image_range(6, 0x2000, 0x1ffe, 0x2002).is_err());
    }

    #[test]
    fn overlay_bounds_ignore_imported_absolute_functions() {
        let symbol = |name: &str, value, section| crate::elf::Symbol {
            name: name.into(),
            value,
            size: 0,
            kind: STT_FUNC,
            bind: 1,
            section,
        };
        let image = LinkedImage {
            map: Map::parse(""),
            bytes: vec![],
            overlay_base: Some(0x2000),
            elf: Some(Elf {
                sections: vec![],
                symbols: vec![
                    symbol("Defined", 0x2001, 1),
                    symbol("Imported", 0x2003, 0xfff1),
                    symbol("Undefined", 0, 0),
                ],
            }),
        };
        assert_eq!(image.functions(), vec![(0x2000, "Defined".into())]);
    }

    #[test]
    fn adopted_overlay_comparison_reads_native_code_and_checks_its_extent() {
        let base = 0x02008000;
        let image = LinkedImage {
            map: Map { placed: vec![Placed {
                name: ".text".into(), path: "games/tla/src/field/scene.o".into(),
                address: base, size: 8, load: base, loaded: true,
                symbols: vec![(base + 2, "Adopted".into())],
            }] },
            elf: Some(Elf { sections: vec![], symbols: vec![crate::elf::Symbol {
                name: "Adopted".into(), value: base + 3, size: 4,
                kind: STT_FUNC, bind: 1, section: 1,
            }] }),
            bytes: vec![0; 8], overlay_base: Some(base),
        };
        let reference = halves(&[0x2000, 0x2007, 0x4770, 0xffff]);
        let code = overlay_reference_code(&image, &reference, "Adopted").unwrap();
        assert_eq!((code.start, code.end, code.name.as_str()), (base + 2, base + 6, "Adopted"));
        assert_eq!(code.bytes, halves(&[0x2007, 0x4770]));
        assert!(overlay_reference_code(&image, &reference[..4], "Adopted").is_err());
        assert!(overlay_reference_code(&image, &reference, "Missing").is_err());
    }

    #[test]
    fn reference_overlay_uses_directory_bounds_with_duplicate_and_unsorted_rows() {
        let map = Map {
            placed: vec![Placed {
                name: ".rodata".into(),
                path: "games/tbs/directory.o".into(),
                address: ROM + 0x20,
                size: 24,
                load: ROM + 0x20,
                loaded: true,
                symbols: vec![(ROM + 0x20, "Resource_DirectoryTable".into())],
            }],
        };
        let payload = halves(&[0x2001, 0x4770, 0x2002, 0x4770]);
        let packed = ags::lz::compress_general(&payload, &ags::resource::PACKER).unwrap();
        let mut rom = vec![0; 0x100];
        let directory = [
            ROM,
            ROM + 0x20,
            ROM + 0x38,
            ROM + 0xc0,
            ROM + 0x80,
            ROM + 0x80,
        ];
        for (index, word) in directory.iter().enumerate() {
            rom[0x20 + index * 4..0x24 + index * 4].copy_from_slice(&word.to_le_bytes());
        }
        rom[0x80..0x80 + packed.len()].copy_from_slice(&packed);
        assert_eq!(
            reference_overlay(&rom, &map, "resource_004").unwrap(),
            payload
        );
        assert_eq!(
            reference_overlay(&rom, &map, "resource_005").unwrap(),
            payload
        );
        let palette = ags::lz::compress_tagged_palette(&payload, &ags::resource::PACKER).unwrap();
        rom[0x80..0x80 + palette.len()].copy_from_slice(&palette);
        assert_eq!(
            reference_overlay(&rom, &map, "resource_004").unwrap(),
            payload
        );
        assert!(reference_overlay(&rom, &map, "resource_006").is_err());
        rom[0x24] ^= 1;
        assert!(reference_overlay(&rom, &map, "resource_004").is_err());
    }

    #[test]
    fn alignment_measures_insertions_without_counting_shifted_rows_or_data() {
        let lines = |words: &[u16]| {
            let image = halves(words);
            thumb_lines(&image, ROM, ROM, ROM + image.len() as u32, &Names::new())
        };
        let reference = lines(&[0xb500, 0x2001, 0x2102, 0xbd00]);
        let built = lines(&[0xb500, 0x2001, 0x4684, 0x2102, 0xbd00]);
        assert_eq!(aligned(&built, &reference), (4, 5, 4, 1));
        let changed = lines(&[0xb500, 0x2003, 0x2102, 0xbd00]);
        assert_eq!(aligned(&changed, &reference), (4, 4, 3, 2));
        let mut padded = reference.clone();
        padded.push(Line {
            address: ROM + 8,
            bytes: vec![0; 4],
            text: ".word 0x00000000".into(),
            wide: true,
        });
        assert_eq!(aligned(&padded, &reference), (4, 4, 4, 0));
    }

    fn halves(words: &[u16]) -> Vec<u8> {
        words.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    fn texts(lines: &[Line]) -> Vec<String> {
        lines
            .iter()
            .map(|l| format!("{:x} {}", l.address, l.text))
            .collect()
    }

    #[test]
    fn reads_thumb_with_pools_calls_and_names() {
        let mut image = halves(&[0xb510, 0x4802, 0xf000, 0xf804, 0xbd10, 0xf800]);
        image.extend(0x0300_0164u32.to_le_bytes());
        image.extend(halves(&[0x4770, 0x0000]));
        let names: Names = [
            (0x0800_0010, "Next".to_string()),
            (0x0300_0164, "gWork".to_string()),
        ]
        .into();
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
        let found = |wanted: &str| {
            resolve(&map, None, &functions, wanted).map(|(p, a, n)| (p.label(), a, n))
        };
        let io = "games/tbs/src/system/io.o".to_string();
        assert_eq!(
            found("Input_Poll"),
            Ok((io.clone(), 0x0800_0018, "Input_Poll".to_string()))
        );
        assert_eq!(
            found("08000012"),
            Ok((io.clone(), 0x0800_0010, "Input_Handle".to_string()))
        );
        assert_eq!(
            found("0x08000016"),
            Ok((io.clone(), 0x0800_0014, "Input_Static".to_string()))
        );
        assert_eq!(
            found("Input_Static"),
            Ok((io, 0x0800_0014, "Input_Static".to_string()))
        );
        assert!(found("Missing").is_err());
    }

    #[test]
    fn rows_pair_by_address_and_mark_differences() {
        let built = thumb_lines(
            &halves(&[0xb500, 0xf000, 0xf801, 0xbd00]),
            ROM,
            ROM,
            ROM + 8,
            &Names::new(),
        );
        let reference = thumb_lines(
            &halves(&[0xb500, 0x2001, 0x2002, 0xbd00]),
            ROM,
            ROM,
            ROM + 8,
            &Names::new(),
        );
        let (text, differ, rows) = render(&built, &reference);
        assert_eq!((differ, rows), (2, 4));
        let lines: Vec<&str> = text.lines().collect();
        assert!(
            lines[0].starts_with("  08000000  b500      push {lr}"),
            "{text}"
        );
        assert!(
            lines[1].starts_with("* 08000002  f000 f801 bl 0x08000008")
                && lines[1].ends_with("2001      movs r0, #1"),
            "{text}"
        );
        assert!(
            lines[2].starts_with("* 08000004  ") && lines[2].ends_with("2002      movs r0, #2"),
            "{text}"
        );
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
            symbols: vec![
                (0x0800_0100, "First".into()),
                (0x0800_0120, "Second".into()),
            ],
        };
        assert_eq!(function_end(&section, 0x0800_0100, &[], None), 0x0800_0120);
        assert_eq!(
            function_end(&section, 0x0800_0120, &[0x0800_0200], None),
            0x0800_0140
        );
        assert_eq!(
            function_end(&section, 0x0800_0100, &[0x0800_0110], None),
            0x0800_0110
        );
        assert_eq!(
            function_end(&section, 0x0800_0100, &[], Some(0x18)),
            0x0800_0118
        );
    }

    #[test]
    fn reads_objdump_arm_lines() {
        let image: Vec<u8> = [0xe0u8, 0x01, 0x2d, 0xe9, 0xff, 0x54, 0xa0, 0xe3].to_vec();
        let output = "\n0800a37c <.data+0xa37c>:\n 800a37c:\te92d01e0 \tpush\t{r5, r6, r7, r8}\n 800a380:\te3a054ff \tmov\tr5, #-16777216\t; 0xff000000\n";
        let lines = parse_objdump(output, &image, 0x0800_a37c);
        assert_eq!(
            texts(&lines),
            [
                "800a37c push {r5, r6, r7, r8}",
                "800a380 mov r5, #-16777216"
            ]
        );
        assert_eq!(lines[1].hex(), "e3a054ff");
    }
}
