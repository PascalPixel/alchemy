//! ARM code in a linked image, for people and compiler studies (K3): where
//! the ELF's mapping symbols ($a, $t, $d and their $a.N forms) place ARM
//! instructions, which object and function owns each byte, and what the
//! instructions themselves show about how they were written. It is a
//! breakdown of the measurement and never changes a total or a credit.

use super::calcrom::Placed;
use object::read::elf::{ElfFile32, ProgramHeader};
use object::{Object, ObjectSection, ObjectSymbol, SymbolKind};
use std::collections::BTreeMap;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Mode {
    Arm,
    Thumb,
    Data,
}

/// The mode a mapping symbol starts: `$a`, `$t`, `$d`, or a `.N` form.
pub(crate) fn mapping_mode(name: &str) -> Option<Mode> {
    let (head, tail) = (name.get(..2)?, name.get(2..)?);
    if !(tail.is_empty() || tail.starts_with('.')) {
        return None;
    }
    match head {
        "$a" => Some(Mode::Arm),
        "$t" => Some(Mode::Thumb),
        "$d" => Some(Mode::Data),
        _ => None,
    }
}

/// What the inventory reads from a linked ELF: executable bytes by run
/// address, each loaded segment's run and load address, the mapping
/// symbols in address order (the later of two at one address wins), and
/// every other label.
#[derive(Debug, Default)]
pub(crate) struct Image<'a> {
    pub code: Vec<(u32, &'a [u8])>,
    pub segments: Vec<(u32, u32, u32)>,
    pub mappings: Vec<(u32, Mode)>,
    pub labels: Vec<(u32, String, bool)>,
}

impl<'a> Image<'a> {
    pub fn parse(bytes: &'a [u8]) -> Result<Image<'a>, String> {
        let file = ElfFile32::<object::Endianness>::parse(bytes).map_err(|e| e.to_string())?;
        let endian = file.endian();
        let mut image = Image::default();
        for header in file.elf_program_headers() {
            if header.p_type(endian) == object::elf::PT_LOAD && header.p_filesz(endian) > 0 {
                image.segments.push((
                    header.p_vaddr(endian),
                    header.p_paddr(endian),
                    header.p_filesz(endian),
                ));
            }
        }
        for section in file.sections() {
            let executable = matches!(section.flags(),
                object::SectionFlags::Elf { sh_flags } if sh_flags & u64::from(object::elf::SHF_EXECINSTR) != 0);
            let data = section.data().map_err(|e| e.to_string())?;
            if executable && !data.is_empty() {
                image.code.push((section.address() as u32, data));
            }
        }
        let mut mappings = Vec::new();
        for symbol in file.symbols() {
            if matches!(symbol.kind(), SymbolKind::Section | SymbolKind::File)
                || symbol.section_index().is_none()
            {
                continue;
            }
            let Ok(name) = symbol.name() else { continue };
            let address = symbol.address() as u32;
            match mapping_mode(name) {
                Some(mode) => mappings.push((address, symbol.index().0, mode)),
                None if !name.is_empty() => {
                    let address = if symbol.kind() == SymbolKind::Text { address & !1 } else { address };
                    image.labels.push((address, name.to_owned(), symbol.is_global()));
                }
                None => {}
            }
        }
        mappings.sort_by_key(|(address, index, _)| (*address, *index));
        image.mappings = mappings.into_iter().map(|(address, _, mode)| (address, mode)).collect();
        image.labels.sort();
        Ok(image)
    }

    fn word(&self, address: u32) -> Option<u32> {
        self.code.iter().find_map(|(start, data)| {
            let at = address.checked_sub(*start)? as usize;
            data.get(at..at + 4).map(|b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
        })
    }

    fn load(&self, address: u32) -> u32 {
        self.segments
            .iter()
            .find(|(run, _, size)| address >= *run && address - run < *size)
            .map_or(address, |(run, load, _)| load + (address - run))
    }

    fn executable(&self, address: u32) -> bool {
        self.code
            .iter()
            .any(|(start, data)| address >= *start && ((address - start) as usize) < data.len())
    }
}

/// The mapping runs inside one input section: a section starts with no
/// known mode until its own first mapping symbol.
pub(crate) fn runs(start: u32, end: u32, mappings: &[(u32, Mode)]) -> Vec<(u32, u32, Mode)> {
    let first = mappings.partition_point(|(address, _)| *address < start);
    let mut found = Vec::new();
    let mut current: Option<(u32, Mode)> = None;
    for &(address, mode) in mappings[first..].iter().take_while(|(a, _)| *a < end) {
        if let Some((from, was)) = current {
            if address > from {
                found.push((from, address, was));
            }
        }
        current = Some((address, mode));
    }
    if let Some((from, was)) = current {
        if end > from {
            found.push((from, end, was));
        }
    }
    found
}

/// What one function's ARM instructions show.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Evidence {
    /// The first stack save's register list (bit n = rn).
    pub save: Option<u16>,
    /// That save opens the function, after `mov ip, sp` at most.
    pub save_opens: bool,
    /// `mov ip, sp` then a save that includes fp: a frame pointer.
    pub frame: bool,
    /// Callee-saved registers (r4-r11) written but never saved. Writing r4
    /// is legal under -fcall-used-r4; writing r5-r11 is a hand marker.
    pub unsaved: u16,
    pub exits: BTreeMap<&'static str, u32>,
    pub markers: BTreeMap<&'static str, u32>,
}

const LR: u16 = 1 << 14;
const FP: u16 = 1 << 11;
const CALL_CLOBBERED: u16 = 0b0101_0000_0000_1111;
const CALLEE_SAVED: u16 = 0b0000_1111_1111_0000;

impl Evidence {
    /// A prologue and epilogue a compiler writes: a return, and either no
    /// stack save or one that opens the function; a save without lr (an
    /// interworking leaf) returns through lr itself.
    pub fn compiled_frame(&self) -> bool {
        let returns = |kinds: &[&str]| kinds.iter().any(|exit| self.exits.contains_key(exit));
        returns(&["bx-lr", "ldm-pc", "mov-pc-lr", "pop-pc"])
            && match self.save {
                None => true,
                Some(list) => {
                    self.save_opens && (list & LR != 0 || returns(&["bx-lr", "mov-pc-lr"]))
                }
            }
    }
}

fn bump(tally: &mut BTreeMap<&'static str, u32>, key: &'static str) {
    *tally.entry(key).or_default() += 1;
}

/// Reads ARM instructions (address, word) in order. `literal` reads a word
/// of the image; `in_code` says whether an address lies in code, so a
/// store through a register that holds one writes code.
pub(crate) fn analyze(
    words: &[(u32, u32)],
    literal: &dyn Fn(u32) -> Option<u32>,
    in_code: &dyn Fn(u32) -> bool,
) -> Evidence {
    let mut e = Evidence::default();
    let mut code: u16 = 0;
    let mut adr: u16 = 0;
    let mut link = false;
    let mut opened_with_ip = false;
    let mut all_written: u16 = 0;
    for (index, &(at, w)) in words.iter().enumerate() {
        let after_link = std::mem::take(&mut link);
        if w >> 28 == 0xF {
            continue;
        }
        let rd = (w >> 12) & 15;
        let rn = (w >> 16) & 15;
        let bit = |n: u32| w & (1 << n) != 0;
        let holds = |set: u16, r: u32| set & (1 << r) != 0;
        let mut written: u16 = 0;
        let mut now_code: u16 = 0;
        let mut now_adr: u16 = 0;
        if w & 0x0F00_0000 == 0x0F00_0000 {
            bump(&mut e.markers, "swi");
        } else if w & 0x0FFF_FFF0 == 0x012F_FF10 {
            match w & 15 {
                14 => bump(&mut e.exits, "bx-lr"),
                12 => bump(&mut e.markers, "ip-return"),
                rm if holds(adr, rm) => bump(&mut e.markers, "pc-interwork"),
                _ if after_link => {}
                _ => bump(&mut e.exits, "bx-reg"),
            }
        } else if w & 0x0FBF_0FFF == 0x010F_0000 {
            bump(&mut e.markers, "psr");
            written = 1 << rd;
        } else if w & 0x0FB0_FFF0 == 0x0120_F000 || w & 0x0FB0_F000 == 0x0320_F000 {
            bump(&mut e.markers, "psr");
        } else if w & 0x0FFF_0000 == 0x092D_0000 || w & 0x0FFF_FFFF == 0x052D_E004 {
            let list = if bit(26) { LR } else { w as u16 };
            if e.save.is_none() {
                e.save = Some(list);
                e.save_opens = index == 0 || (index == 1 && opened_with_ip);
                e.frame = opened_with_ip && list & FP != 0;
            }
        } else if w & 0x0E00_0000 == 0x0800_0000 {
            let list = w as u16;
            if bit(20) {
                if list & (1 << 15) != 0 {
                    if bit(22) {
                        bump(&mut e.markers, "psr");
                    } else {
                        bump(&mut e.exits, "ldm-pc");
                    }
                }
                written = list | if bit(21) { 1 << rn } else { 0 };
            } else if rn != 13 && holds(code, rn) {
                bump(&mut e.markers, "code-store");
            }
        } else if w & 0x0C00_0000 == 0x0400_0000 {
            let load = bit(20);
            if load && rd == 15 {
                if w & 0x0FFF_FFFF == 0x049D_F004 {
                    bump(&mut e.exits, "pop-pc");
                } else if w & 0x0FFF_FFF0 == 0x079F_F100 {
                    bump(&mut e.exits, "switch");
                } else {
                    bump(&mut e.markers, "pc-computed");
                }
            }
            if !load && (rn == 15 || holds(code, rn)) {
                bump(&mut e.markers, "code-store");
            }
            if load {
                written |= 1 << rd;
                if rn == 15 && !bit(25) && bit(24) {
                    let offset = w & 0xFFF;
                    let base = at.wrapping_add(8);
                    let source = if bit(23) { base.wrapping_add(offset) } else { base.wrapping_sub(offset) };
                    if literal(source).is_some_and(|value| in_code(value & !1)) {
                        now_code |= 1 << rd;
                    }
                }
            }
            if bit(21) || !bit(24) {
                if !load && holds(code, rn) {
                    now_code |= 1 << rn;
                }
                written |= 1 << rn;
            }
        } else if w & 0x0E00_0000 == 0x0A00_0000 {
            if bit(24) {
                written = CALL_CLOBBERED;
            }
        } else if w & 0x0C00_0000 == 0 {
            if w & 0x0F00_00F0 == 0x0000_0090 {
                written = 1 << rn | if bit(23) { 1 << rd } else { 0 };
            } else if w & 0x0FB0_0FF0 == 0x0100_0090 {
                written = 1 << rd;
            } else if w & 0x0E00_0090 == 0x0000_0090 && w & 0x60 != 0 {
                if bit(20) {
                    written |= 1 << rd;
                } else if holds(code, rn) || rn == 15 {
                    bump(&mut e.markers, "code-store");
                }
                if bit(21) || !bit(24) {
                    written |= 1 << rn;
                    if !bit(20) && holds(code, rn) {
                        now_code |= 1 << rn;
                    }
                }
            } else {
                let opcode = (w >> 21) & 15;
                let immediate = bit(25);
                if (8..=11).contains(&opcode) {
                } else if rd == 15 {
                    if bit(20) {
                        bump(&mut e.markers, "psr");
                    } else if w & 0x0FFF_FFFF == 0x01A0_F00E {
                        bump(&mut e.exits, "mov-pc-lr");
                    } else if w & 0x0FFF_FFF0 == 0x01A0_F000 && w & 15 == 12 {
                        bump(&mut e.markers, "ip-return");
                    } else if w & 0x0FFF_FFF0 == 0x01A0_F000 && after_link {
                    } else {
                        bump(&mut e.markers, "pc-computed");
                    }
                } else {
                    written = 1 << rd;
                    if w & 0x0FFF_FFFF == 0x01A0_E00F {
                        link = true;
                    } else if w & 0x0FFF_FFFF == 0x01A0_C00D && index == 0 {
                        opened_with_ip = true;
                    } else if immediate && rn == 15 && (opcode == 4 || opcode == 2) {
                        now_code |= 1 << rd;
                        now_adr |= 1 << rd;
                    } else if w & 0x0FEF_0FF0 == 0x01A0_0000 {
                        let rm = w & 15;
                        if holds(code, rm) {
                            now_code |= 1 << rd;
                        }
                        if holds(adr, rm) {
                            now_adr |= 1 << rd;
                        }
                    } else if immediate && (opcode == 4 || opcode == 2) && holds(code, rn) {
                        now_code |= 1 << rd;
                    }
                }
            }
        }
        code = (code & !written) | now_code;
        adr = (adr & !written) | now_adr;
        all_written |= written;
    }
    e.unsaved = all_written & CALLEE_SAVED & !e.save.unwrap_or(0);
    if e.unsaved & CALLEE_SAVED & !(1 << 4) != 0 {
        bump(&mut e.markers, "unsaved-reg");
    }
    e
}

pub(crate) fn shape(evidence: &Evidence, mixed: bool) -> &'static str {
    if mixed {
        "mixed"
    } else if !evidence.markers.is_empty() {
        "hand"
    } else if evidence.compiled_frame() {
        "compiled"
    } else {
        "fragment"
    }
}

/// `{r4-r7,lr}`: a register list as an assembler writes it.
pub(crate) fn register_list(list: u16) -> String {
    let names = ["fp", "ip", "sp", "lr", "pc"];
    let mut parts = Vec::new();
    let mut r = 0;
    while r < 11 {
        if list & (1 << r) == 0 {
            r += 1;
            continue;
        }
        let first = r;
        while r + 1 < 11 && list & (1 << (r + 1)) != 0 {
            r += 1;
        }
        parts.push(if r > first { format!("r{first}-r{r}") } else { format!("r{first}") });
        r += 1;
    }
    for (offset, name) in names.iter().enumerate() {
        if list & (1 << (11 + offset)) != 0 {
            parts.push(name.to_string());
        }
    }
    format!("{{{}}}", parts.join(","))
}

fn tally_text(tally: &BTreeMap<&'static str, u32>) -> String {
    if tally.is_empty() {
        return "-".into();
    }
    tally.iter().map(|(key, count)| format!("{key}:{count}")).collect::<Vec<_>>().join(",")
}

/// One function's (or unnamed run's) ARM bytes in one object.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct ArmRow {
    pub image: String,
    pub object: String,
    pub function: String,
    pub run: u32,
    pub load: u32,
    pub bytes: i64,
    /// Of those, bytes inside spans the source marks as carrying no credit.
    pub padding: i64,
    pub origin: &'static str,
    pub shape: &'static str,
    pub evidence: Evidence,
}

pub(crate) const TSV_HEADER: &str =
    "image\tobject\tfunction\trun\tload\tarm_bytes\tpadding\torigin\tshape\tfirst_save\tframe\tunsaved_writes\texits\tmarkers";

impl ArmRow {
    pub fn tsv(&self) -> String {
        let e = &self.evidence;
        format!(
            "{}\t{}\t{}\t{:08x}\t{:08x}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
            self.image,
            self.object,
            self.function,
            self.run,
            self.load,
            self.bytes,
            self.padding,
            self.origin,
            self.shape,
            e.save.map_or("-".into(), register_list),
            if e.frame { "fp" } else { "-" },
            if e.unsaved == 0 { "-".into() } else { register_list(e.unsaved) },
            tally_text(&e.exits),
            tally_text(&e.markers),
        )
    }
}

fn overlap(from: u32, to: u32, spans: &[(i64, i64)]) -> i64 {
    spans
        .iter()
        .map(|(start, bytes)| ((to as i64).min(start + bytes) - (from as i64).max(*start)).max(0))
        .sum()
}

/// Every function's ARM bytes in one image. `describe` names an input
/// section's object and origin (C, assembly, library, disassembly, listing,
/// other, or outside when the count does not read the section); it is
/// asked only about sections that hold ARM code.
pub(crate) fn inventory(
    image: &Image<'_>,
    name: &str,
    placed: &[Placed<'_>],
    uncredited: &[(i64, i64)],
    describe: &dyn Fn(&Placed<'_>) -> Result<(String, &'static str), String>,
) -> Result<Vec<ArmRow>, String> {
    let pieces = placed
        .iter()
        .filter(|p| p.size > 0 && p.address >= 0 && image.executable(p.address as u32))
        .map(|p| {
            let (start, end) = (p.address as u32, (p.address + p.size) as u32);
            (p, runs(start, end, &image.mappings))
        })
        .collect::<Vec<_>>();
    let mut modes: BTreeMap<&str, (i64, i64)> = BTreeMap::new();
    for (piece, found) in &pieces {
        let entry = modes.entry(piece.object).or_default();
        for (from, to, mode) in found {
            match mode {
                Mode::Arm => entry.0 += i64::from(to - from),
                Mode::Thumb => entry.1 += i64::from(to - from),
                Mode::Data => {}
            }
        }
    }
    let global_at = |address: u32| {
        let first = image.labels.partition_point(|(a, _, _)| *a < address);
        image.labels[first..]
            .iter()
            .take_while(|(a, _, _)| *a == address)
            .any(|(_, _, global)| *global)
    };
    let mut code_ranges = Vec::new();
    for (_, found) in pieces.iter().filter(|(p, _)| modes[p.object] != (0, 0)) {
        for &(from, to, mode) in found {
            if mode != Mode::Data || !global_at(from) {
                code_ranges.push((from, to));
            }
        }
    }
    code_ranges.sort();
    let in_code = |address: u32| {
        let at = code_ranges.partition_point(|(from, _)| *from <= address);
        at > 0 && address < code_ranges[at - 1].1
    };
    let literal = |address: u32| image.word(address);
    let mut rows = Vec::new();
    for (piece, found) in &pieces {
        if !found.iter().any(|(_, _, mode)| *mode == Mode::Arm) {
            continue;
        }
        let (start, end) = (piece.address as u32, (piece.address + piece.size) as u32);
        let first = image.labels.partition_point(|(a, _, _)| *a < start);
        let inside = image.labels[first..]
            .iter()
            .take_while(|(a, _, _)| *a < end)
            .filter(|(_, label, _)| !label.starts_with("AlchemyUncredited") && !label.starts_with(".L"))
            .collect::<Vec<_>>();
        let globals = inside.iter().filter(|(_, _, global)| *global).copied().collect::<Vec<_>>();
        // Code before the first global takes the local label before it.
        let leading = |at: u32| {
            inside
                .iter()
                .rev()
                .find(|(address, _, _)| *address <= at)
                .map_or("-", |(_, label, _)| label.as_str())
        };
        let chosen = if globals.is_empty() { inside.clone() } else { globals };
        let mut bounds: Vec<(u32, &str)> = Vec::new();
        for (address, label, _) in chosen {
            if bounds.last().is_none_or(|(at, _)| at != address) {
                bounds.push((*address, label.as_str()));
            }
        }
        let (object, origin) = describe(piece)?;
        let mixed = modes[piece.object].0 > 0 && modes[piece.object].1 > 0;
        let mut functions: Vec<(&str, Vec<(u32, u32)>)> = Vec::new();
        for &(from, to, mode) in found {
            if mode != Mode::Arm {
                continue;
            }
            let mut at = from;
            while at < to {
                let owner = bounds.partition_point(|(a, _)| *a <= at);
                let label = owner.checked_sub(1).map_or_else(|| leading(at), |i| bounds[i].1);
                let until = bounds.get(owner).map_or(to, |(a, _)| (*a).min(to));
                match functions.iter_mut().find(|(f, _)| *f == label) {
                    Some((_, ranges)) => ranges.push((at, until)),
                    None => functions.push((label, vec![(at, until)])),
                }
                at = until;
            }
        }
        for (function, ranges) in functions {
            let mut words = Vec::new();
            for &(from, to) in &ranges {
                let mut at = (from + 3) & !3;
                while at + 4 <= to {
                    if let Some(word) = image.word(at) {
                        words.push((at, word));
                    }
                    at += 4;
                }
            }
            let evidence = analyze(&words, &literal, &in_code);
            let run = ranges[0].0;
            rows.push(ArmRow {
                image: name.to_owned(),
                object: object.clone(),
                function: function.to_owned(),
                run,
                load: image.load(run),
                bytes: ranges.iter().map(|(from, to)| i64::from(to - from)).sum(),
                padding: ranges.iter().map(|(from, to)| overlap(*from, *to, uncredited)).sum(),
                origin,
                shape: shape(&evidence, mixed),
                evidence,
            });
        }
    }
    Ok(rows)
}

/// The English build's ARM bytes by what the count makes of them.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub(crate) struct ArmTally {
    pub total: i64,
    pub c: i64,
    pub assembly: i64,
    pub library: i64,
    pub disassembly: i64,
    pub listings: i64,
    pub padding: i64,
    pub outside: i64,
    pub other: i64,
}

impl ArmTally {
    pub fn of(rows: &[ArmRow]) -> ArmTally {
        let mut t = ArmTally::default();
        for row in rows {
            t.total += row.bytes;
            let earned = row.bytes - row.padding;
            let credited = match row.origin {
                "C" => Some(&mut t.c),
                "assembly" => Some(&mut t.assembly),
                "library" => Some(&mut t.library),
                _ => None,
            };
            if let Some(part) = credited {
                *part += earned;
                t.padding += row.padding;
                continue;
            }
            match row.origin {
                "disassembly" => t.disassembly += row.bytes,
                "listing" => t.listings += row.bytes,
                "outside" => t.outside += row.bytes,
                _ => t.other += row.bytes,
            }
        }
        t
    }
    pub fn credited(&self) -> i64 {
        self.c + self.assembly + self.library
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const PUSH_R4_LR: u32 = 0xE92D_4010; // push {r4, lr}
    const POP_R4_PC: u32 = 0xE8BD_8010; // pop {r4, pc}
    const BX_LR: u32 = 0xE12F_FF1E;
    const MOV_R0_R1: u32 = 0xE1A0_0001;

    fn words(list: &[u32]) -> Vec<(u32, u32)> {
        list.iter().enumerate().map(|(i, w)| (0x0300_0000 + 4 * i as u32, *w)).collect()
    }
    fn plain(list: &[u32]) -> Evidence {
        analyze(&words(list), &|_| None, &|_| false)
    }

    #[test]
    fn mapping_symbols_and_their_numbered_forms() {
        assert_eq!(mapping_mode("$a"), Some(Mode::Arm));
        assert_eq!(mapping_mode("$a.12"), Some(Mode::Arm));
        assert_eq!(mapping_mode("$t"), Some(Mode::Thumb));
        assert_eq!(mapping_mode("$d.0"), Some(Mode::Data));
        assert_eq!(mapping_mode("$ab"), None);
        assert_eq!(mapping_mode("Main"), None);
        assert_eq!(mapping_mode("$"), None);
    }

    #[test]
    fn a_section_has_no_mode_before_its_own_first_symbol() {
        let mappings = [(0x00, Mode::Thumb), (0x14, Mode::Arm), (0x20, Mode::Data), (0x20, Mode::Arm), (0x40, Mode::Thumb)];
        // The run that starts before the section does not leak into it,
        // and the later of two symbols at one address wins.
        assert_eq!(runs(0x10, 0x30, &mappings), vec![(0x14, 0x20, Mode::Arm), (0x20, 0x30, Mode::Arm)]);
        assert!(runs(0x30, 0x40, &mappings).is_empty());
    }

    #[test]
    fn a_compiled_function_saves_from_r4_and_returns() {
        let e = plain(&[PUSH_R4_LR, MOV_R0_R1, POP_R4_PC]);
        assert_eq!(e.save, Some(0x4010));
        assert!(e.save_opens && !e.frame);
        assert_eq!(register_list(e.save.unwrap()), "{r4,lr}");
        assert_eq!(shape(&e, false), "compiled");
        assert_eq!(shape(&plain(&[MOV_R0_R1, BX_LR]), false), "compiled");
        assert_eq!(shape(&e, true), "mixed");
        // An interworking leaf saves without lr and returns through it.
        let leaf = plain(&[0xE92D_0030, MOV_R0_R1, 0xE8BD_0030, BX_LR]);
        assert_eq!((register_list(leaf.save.unwrap()).as_str(), shape(&leaf, false)), ("{r4-r5}", "compiled"));
        // mov ip, sp; push {r4-r7, fp, ip, lr, pc}; sub fp, ip, #4 ... ldmea fp, {...}
        let framed = plain(&[0xE1A0_C00D, 0xE92D_D8F0, 0xE24C_B004, 0xE91B_A8F0]);
        assert!(framed.frame && framed.save_opens);
        assert_eq!(register_list(framed.save.unwrap()), "{r4-r7,fp,ip,lr,pc}");
        assert_eq!(shape(&framed, false), "compiled");
    }

    #[test]
    fn hand_markers() {
        let markers = |list: &[u32]| plain(list).markers.keys().copied().collect::<Vec<_>>();
        assert_eq!(markers(&[0xE10F_0000, 0xE129_F000, BX_LR]), ["psr"]); // mrs r0, cpsr; msr cpsr_fc, r0
        assert_eq!(markers(&[0xEF03_0000]), ["swi"]);
        assert_eq!(markers(&[0xE12F_FF1C]), ["ip-return"]); // bx ip
        assert_eq!(markers(&[0xE08F_F100]), ["pc-computed"]); // add pc, pc, r0, lsl #2
        assert_eq!(markers(&[0xE28F_0001, 0xE12F_FF10]), ["pc-interwork"]); // add r0, pc, #1; bx r0
        assert_eq!(markers(&[0xE25E_F004]), ["psr"]); // subs pc, lr, #4
        // A compiled call through a register and a switch table are not marks.
        assert!(markers(&[0xE1A0_E00F, 0xE12F_FF13, 0x979F_F100, BX_LR]).is_empty());
        let e = plain(&[PUSH_R4_LR, 0xE08F_F100, POP_R4_PC]);
        assert_eq!(shape(&e, false), "hand");
        // Writing r5-r11 without saving them breaks the calling convention;
        // r4 is call-used under -fcall-used-r4.
        let clobbers = plain(&[0xE1A0_5000, BX_LR]); // mov r5, r0
        assert_eq!((register_list(clobbers.unsaved).as_str(), shape(&clobbers, false)), ("{r5}", "hand"));
        let call_used = plain(&[0xE1A0_4000, BX_LR]); // mov r4, r0
        assert_eq!((register_list(call_used.unsaved).as_str(), shape(&call_used, false)), ("{r4}", "compiled"));
        // A save that does not open the function is no compiled prologue.
        assert_eq!(shape(&plain(&[MOV_R0_R1, PUSH_R4_LR, POP_R4_PC]), false), "fragment");
        assert_eq!(shape(&plain(&[MOV_R0_R1]), false), "fragment");
    }

    #[test]
    fn a_store_through_a_code_address_writes_code() {
        // ldr r1, [pc, #0] reads 0x03000100, which lies in code; str r0, [r1]
        let literal = |address: u32| (address == 0x0300_0008).then_some(0x0300_0100);
        let in_code = |address: u32| (0x0300_0100..0x0300_0200).contains(&address);
        let e = analyze(&words(&[0xE59F_1000, 0xE581_0000, BX_LR]), &literal, &in_code);
        assert_eq!(e.markers.keys().copied().collect::<Vec<_>>(), ["code-store"]);
        // Once r1 is reloaded from elsewhere, a store through it is plain.
        let e = analyze(&words(&[0xE59F_1000, 0xE591_1000, 0xE581_0000, BX_LR]), &literal, &in_code);
        assert!(e.markers.is_empty());
    }

    #[test]
    fn inventory_attributes_arm_bytes_to_functions_and_objects() {
        let mut bytes = Vec::new();
        for w in [PUSH_R4_LR, MOV_R0_R1, POP_R4_PC, 0, BX_LR, 0xE1A0_0000, 0, 0] {
            bytes.extend_from_slice(&u32::to_le_bytes(w));
        }
        let image = Image {
            code: vec![(0x0300_0000, &bytes)],
            segments: vec![(0x0300_0000, 0x0800_0770, 0x20)],
            mappings: vec![(0x0300_0000, Mode::Arm), (0x0300_000c, Mode::Data), (0x0300_0010, Mode::Arm), (0x0300_0018, Mode::Thumb)],
            labels: vec![
                (0x0300_0000, "First".into(), true),
                (0x0300_0004, "loop".into(), false),
                (0x0300_0010, "entry".into(), false),
                (0x0300_0014, "Second".into(), true),
            ],
        };
        let placed = [
            Placed { name: ".text", address: 0x0300_0000, size: 0x10, object: "a/A.o" },
            Placed { name: ".text", address: 0x0300_0010, size: 0x10, object: "a/B.o" },
        ];
        let describe = |p: &Placed<'_>| {
            let origin = if p.object.ends_with("A.o") { "assembly" } else { "disassembly" };
            Ok::<_, String>((p.object.to_owned(), origin))
        };
        let rows = inventory(&image, "main", &placed, &[(0x0300_0014, 4)], &describe).unwrap();
        let shown = rows.iter().map(|r| (r.function.as_str(), r.run, r.load, r.bytes, r.padding, r.origin, r.shape)).collect::<Vec<_>>();
        // Code before B.o's first global takes the local label before it.
        assert_eq!(shown, [
            ("First", 0x0300_0000, 0x0800_0770, 12, 0, "assembly", "compiled"),
            ("entry", 0x0300_0010, 0x0800_0780, 4, 0, "disassembly", "mixed"),
            ("Second", 0x0300_0014, 0x0800_0784, 4, 4, "disassembly", "mixed"),
        ]);
        assert_eq!(rows[0].tsv(), "main\ta/A.o\tFirst\t03000000\t08000770\t12\t0\tassembly\tcompiled\t{r4,lr}\t-\t-\tldm-pc:1\t-");
        let tally = ArmTally::of(&rows);
        assert_eq!((tally.total, tally.credited(), tally.disassembly), (20, 12, 8));
        let mut shapes = BTreeMap::new();
        for row in &rows {
            *shapes.entry(row.shape).or_insert(0) += row.bytes;
        }
        assert_eq!(shapes, BTreeMap::from([("compiled", 12), ("mixed", 8)]));
    }
}
