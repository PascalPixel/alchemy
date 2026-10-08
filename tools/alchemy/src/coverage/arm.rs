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
    /// stack save or one that opens the function and saves lr. agscc -marm
    /// and agbcc_arm push lr whenever they save registers, leaf functions
    /// included, so a save without lr is a hand marker.
    pub fn compiled_frame(&self) -> bool {
        let returns = |kinds: &[&str]| kinds.iter().any(|exit| self.exits.contains_key(exit));
        returns(&["bx-ip", "bx-lr", "ldm-pc", "mov-pc-lr", "pop-pc"])
            && match self.save {
                None => true,
                Some(list) => self.save_opens && list & LR != 0,
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
                // ldmfd sp!, {..., ip}; bx ip: GCC 2.9's interworking epilogue.
                12 if index > 0 && words[index - 1].1 & 0x0FFF_1000 == 0x08BD_1000 => {
                    bump(&mut e.exits, "bx-ip")
                }
                // mov lr, pc; bx ip: a compiled call through ip.
                _ if after_link => {}
                12 => bump(&mut e.markers, "ip-return"),
                rm if holds(adr, rm) => bump(&mut e.markers, "pc-interwork"),
                _ => bump(&mut e.exits, "bx-reg"),
            }
        } else if w & 0x0FBF_0FFF == 0x010F_0000 {
            bump(&mut e.markers, "psr");
            written = 1 << rd;
        } else if w & 0x0FB0_FFF0 == 0x0120_F000 || w & 0x0FB0_F000 == 0x0320_F000 {
            bump(&mut e.markers, "psr");
        } else if w & 0x0FFF_0000 == 0x092D_0000 || w & 0x0FFF_FFFF == 0x052D_E004 {
            let list = if bit(26) { LR } else { w as u16 };
            // A save of argument registers alone is a varargs spill.
            if list & LR == 0 && list & !0xF != 0 {
                bump(&mut e.markers, "save-no-lr");
            }
            if e.save.is_none() {
                e.save = Some(list);
                e.save_opens = index == 0 || (index == 1 && opened_with_ip);
                e.frame = opened_with_ip && list & FP != 0;
            }
        } else if w & 0x0E00_0000 == 0x0800_0000 {
            let list = w as u16;
            // Off sp, Camelot's compilers write a multiple back only in a
            // block copy, whose list runs from r0, or for a doubleword pair.
            let count = list.count_ones();
            let from_r0 = u32::from(list) == (1 << count) - 1;
            let pair = count == 2 && list >> list.trailing_zeros() == 0b11;
            if bit(21) && rn != 13 && !from_r0 && !pair {
                bump(&mut e.markers, "multiple-writeback");
            }
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
                if bit(23) && unpaired_halves(w, &words[index + 1..]) {
                    bump(&mut e.markers, "multiply-unpaired");
                }
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
                    // movs rd, rm: their copy-and-test is subs rd, rm, #0.
                    if w & 0x0FF0_0FF0 == 0x01B0_0000 {
                        bump(&mut e.markers, "movs-unshifted");
                    }
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

/// A long multiply that keeps both halves outside a register pair. Both
/// compilers keep both halves only through mulsidi3 or its accumulating
/// forms, which print a DImode register as RdLo, RdLo + 1, early-clobbered
/// apart from smull's inputs; smulsi3_highpart frees RdLo but discards it,
/// so an smull or umull counts only when a later instruction reads RdLo.
fn unpaired_halves(w: u32, after: &[(u32, u32)]) -> bool {
    let (hi, lo, rs, rm) = ((w >> 16) & 15, (w >> 12) & 15, (w >> 8) & 15, w & 15);
    if w & (1 << 21) != 0 {
        return hi != lo + 1;
    }
    let outside = hi != lo + 1 || [rm, rs].contains(&hi) || [rm, rs].contains(&lo);
    outside && reads_before_writing(lo, after)
}

/// Whether straight-line code reads register `r` before an unconditional
/// write replaces it. A conditional write hides `r` from reads on the same
/// condition until the flags change. Anything it cannot follow (a branch,
/// an exchange, a coprocessor or status instruction) answers no.
fn reads_before_writing(r: u32, after: &[(u32, u32)]) -> bool {
    let mut hidden: Option<u32> = None;
    for &(_, w) in after {
        let field = |shift: u32| (w >> shift) & 15;
        let condition = w >> 28;
        let sets_flags = w & 0x0C00_0000 == 0 && w & (1 << 20) != 0;
        let (reads, writes): (u16, u16) = if w & 0x0FFF_FFF0 == 0x012F_FF10 || w & 0x0E00_0000 == 0x0A00_0000 {
            return false;
        } else if w & 0x0F00_00F0 == 0x0000_0090 {
            let long = w & (1 << 23) != 0;
            let accumulate = w & (1 << 21) != 0;
            let mut reads = 1 << field(0) | 1 << field(8);
            if accumulate {
                reads |= 1 << field(12) | if long { 1 << field(16) } else { 0 };
            }
            (reads, 1 << field(16) | if long { 1 << field(12) } else { 0 })
        } else if w & 0x0E00_0090 == 0x0000_0090 && w & 0x60 != 0 {
            let load = w & (1 << 20) != 0;
            let mut reads = 1 << field(16) | if w & (1 << 22) == 0 { 1 << field(0) } else { 0 };
            if !load {
                reads |= 1 << field(12);
            }
            (reads, if load { 1 << field(12) } else { 0 })
        } else if w & 0x0C00_0000 == 0 {
            if w & 0x0D90_0000 == 0x0100_0000 {
                return false;
            }
            let opcode = (w >> 21) & 15;
            let mut reads = if opcode == 13 || opcode == 15 { 0 } else { 1 << field(16) };
            if w & (1 << 25) == 0 {
                reads |= 1 << field(0) | if w & 0x10 != 0 { 1 << field(8) } else { 0 };
            }
            (reads, if (8..=11).contains(&opcode) { 0 } else { 1 << field(12) })
        } else if w & 0x0C00_0000 == 0x0400_0000 {
            let load = w & (1 << 20) != 0;
            let mut reads = 1 << field(16) | if w & (1 << 25) != 0 { 1 << field(0) } else { 0 };
            if !load {
                reads |= 1 << field(12);
            }
            (reads, if load { 1 << field(12) } else { 0 })
        } else if w & 0x0E00_0000 == 0x0800_0000 {
            let load = w & (1 << 20) != 0;
            let list = w as u16;
            (1 << field(16) | if load { 0 } else { list }, if load { list } else { 0 })
        } else {
            return false;
        };
        if reads & (1 << r) != 0 && hidden != Some(condition) {
            return true;
        }
        if writes & (1 << r) != 0 {
            if condition == 0xE {
                return false;
            }
            hidden = Some(condition);
        }
        if sets_flags {
            hidden = None;
        }
    }
    false
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

/// A label one object defines in one of its input sections, at its run
/// address in the image.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Label {
    pub address: u32,
    pub name: String,
    pub global: bool,
    pub function: bool,
}

impl Label {
    /// Which of two labels at one address names what starts there: a
    /// function before a plain label, a global before a local, and any
    /// label before one that marks an end (`...End`).
    fn rank(&self) -> (u32, bool, bool, bool, &str) {
        (self.address, !self.function, !self.global, self.name.ends_with("End"), &self.name)
    }
}

/// The labels an object file defines in its input section `section`,
/// placed at `start`. Mapping symbols, section and file symbols are not
/// labels; a Thumb function's address loses its mode bit.
pub(crate) fn object_labels(bytes: &[u8], section: &str, start: u32) -> Result<Vec<Label>, String> {
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let Some(index) = file.sections().find(|s| s.name() == Ok(section)).map(|s| s.index()) else {
        return Ok(Vec::new());
    };
    let mut labels = Vec::new();
    for symbol in file.symbols() {
        if symbol.section_index() != Some(index) || matches!(symbol.kind(), SymbolKind::Section | SymbolKind::File) {
            continue;
        }
        let Ok(name) = symbol.name() else { continue };
        if name.is_empty() || mapping_mode(name).is_some() {
            continue;
        }
        let function = symbol.kind() == SymbolKind::Text;
        let offset = if function { symbol.address() as u32 & !1 } else { symbol.address() as u32 };
        labels.push(Label { address: start + offset, name: name.to_owned(), global: symbol.is_global(), function });
    }
    Ok(labels)
}

/// The mapping runs an object file marks in its input section `section`,
/// placed at `start`: its own mapping symbols, or, in a section that has
/// none (code from the compilers' own assembler), its function symbols,
/// Thumb at an odd address and ARM at an even one. A mapping symbol's
/// address in the linked image never tells which object marked it: one
/// another object leaves at its own end sits where the next section starts.
pub(crate) fn object_mappings(bytes: &[u8], section: &str, start: u32) -> Result<Vec<(u32, Mode)>, String> {
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let Some(index) = file.sections().find(|s| s.name() == Ok(section)).map(|s| s.index()) else {
        return Ok(Vec::new());
    };
    let symbols = file
        .symbols()
        .filter(|symbol| symbol.section_index() == Some(index))
        .map(|symbol| {
            let kind = match symbol.flags() {
                object::SymbolFlags::Elf { st_info, .. } => st_info & 0xf,
                _ => object::elf::STT_NOTYPE,
            };
            (symbol.address() as u32, symbol.name().unwrap_or(""), kind)
        })
        .collect::<Vec<_>>();
    Ok(section_mappings(start, &symbols))
}

/// `object_mappings` over one section's symbols in symbol-table order, as
/// (offset, name, ELF symbol type): the old assemblers mark a Thumb
/// function `STT_ARM_TFUNC`, the current one with an odd `STT_FUNC`.
pub(crate) fn section_mappings(start: u32, symbols: &[(u32, &str, u8)]) -> Vec<(u32, Mode)> {
    let mut marked = Vec::new();
    let mut functions = Vec::new();
    for (index, &(offset, name, kind)) in symbols.iter().enumerate() {
        if let Some(mode) = mapping_mode(name) {
            marked.push((start + offset, index, mode));
        } else if kind == object::elf::STT_ARM_TFUNC || (kind == object::elf::STT_FUNC && offset & 1 == 1) {
            functions.push((start + (offset & !1), index, Mode::Thumb));
        } else if kind == object::elf::STT_FUNC {
            functions.push((start + offset, index, Mode::Arm));
        }
    }
    let mut found = if marked.is_empty() { functions } else { marked };
    found.sort_by_key(|(address, index, _)| (*address, *index));
    found.into_iter().map(|(address, _, mode)| (address, mode)).collect()
}

/// The global symbols a linker map lists under each input section, keyed
/// by section name, address and object as `sections` reads them: the
/// fallback for an object that cannot be read on its own (an archive
/// member). A map lists no local labels.
pub(crate) fn map_globals(map: &str) -> BTreeMap<(String, i64, String), Vec<(u32, String)>> {
    let hex = |field: &str| field.strip_prefix("0x").and_then(|d| i64::from_str_radix(d, 16).ok());
    let mut found: BTreeMap<(String, i64, String), Vec<(u32, String)>> = BTreeMap::new();
    let mut current: Option<(String, i64, String)> = None;
    let mut pending: Option<String> = None;
    for line in map.lines() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if let Some(name) = pending.take() {
            current = match fields.as_slice() {
                [address, size, ..] if hex(address).is_some() && hex(size).is_some() => {
                    let object = line.trim_start()[address.len()..].trim_start()[size.len()..].trim();
                    Some((name, hex(address).unwrap_or(0), object.to_owned()))
                }
                _ => None,
            };
            continue;
        }
        if line.starts_with(" .") {
            let name = fields[0].to_owned();
            match fields.as_slice() {
                [_] => pending = Some(name),
                [_, address, size, ..] if hex(address).is_some() && hex(size).is_some() => {
                    let rest = line.trim_start()[fields[0].len()..].trim_start();
                    let object = rest[address.len()..].trim_start()[size.len()..].trim();
                    current = Some((name, hex(address).unwrap_or(0), object.to_owned()));
                }
                _ => current = None,
            }
            continue;
        }
        match (fields.as_slice(), &current) {
            ([address, symbol], Some(key)) if line.starts_with("    ") && !symbol.starts_with("0x") => {
                if let Some(address) = hex(address) {
                    found.entry(key.clone()).or_default().push((address as u32, (*symbol).to_owned()));
                }
            }
            _ => current = None,
        }
    }
    found
}

/// Whether an ARM word is a prologue, a return or a software interrupt
/// that is always executed: the anchors that make a run of data words
/// likely ARM code.
fn arm_anchor(w: u32) -> bool {
    w & 0xFFFF_4000 == 0xE92D_4000 // stmfd sp!, {..., lr}
        || w == 0xE52D_E004 // str lr, [sp, #-4]!
        || w & 0xFFFF_8000 == 0xE8BD_8000 // ldmfd sp!, {..., pc}
        || w == 0xE49D_F004 // ldr pc, [sp], #4
        || w == 0xE12F_FF1E // bx lr
        || w == 0xE1A0_F00E // mov pc, lr
        || w & 0xFF00_0000 == 0xEF00_0000 // swi
}

/// Not-yet-C listings may write ARM code as data words, which the
/// assembler marks `$d`. Of consecutive word-aligned data words, the spans
/// that are likely ARM: a run of words that all carry a valid condition
/// field (never 0xF on the ARM7TDMI), broken where more than
/// `LIKELY_GAP` words in a row are not always-executed, that holds a
/// prologue, return or swi; taken from its first always-executed word to
/// its last, and only when that is two words or more and at least half
/// its words are always-executed.
pub(crate) fn likely_arm(words: &[(u32, u32)]) -> Vec<(u32, u32)> {
    const LIKELY_GAP: usize = 16;
    let always = |w: u32| w >> 28 == 0xE;
    let mut spans = Vec::new();
    let mut group: Vec<(u32, u32)> = Vec::new();
    let mut flush = |group: &mut Vec<(u32, u32)>| {
        let first = group.iter().position(|(_, w)| always(*w));
        let last = group.iter().rposition(|(_, w)| always(*w));
        if let (Some(first), Some(last)) = (first, last) {
            let span = &group[first..=last];
            let executed = span.iter().filter(|(_, w)| always(*w)).count();
            if span.len() >= 2 && span.iter().any(|(_, w)| arm_anchor(*w)) && 2 * executed >= span.len() {
                spans.push((span[0].0, span[span.len() - 1].0 + 4));
            }
        }
        group.clear();
    };
    let mut quiet = 0;
    for &(at, w) in words {
        let follows = group.last().is_some_and(|(previous, _)| previous + 4 == at);
        if w >> 28 == 0xF || !follows || quiet > LIKELY_GAP {
            flush(&mut group);
            quiet = 0;
        }
        if w >> 28 == 0xF {
            continue;
        }
        quiet = if always(w) { 0 } else { quiet + 1 };
        group.push((at, w));
    }
    flush(&mut group);
    spans
}

fn overlap(from: u32, to: u32, spans: &[(i64, i64)]) -> i64 {
    spans
        .iter()
        .map(|(start, bytes)| ((to as i64).min(start + bytes) - (from as i64).max(*start)).max(0))
        .sum()
}

/// Every function's ARM bytes in one image, and apart from them the data
/// words in not-yet-C listings that are likely ARM (`likely_arm`), which
/// are never part of any total. `describe` names an input section's object
/// and origin (C, assembly, library, disassembly, listing, other, or
/// outside when the count does not read the section); `defined` gives the
/// labels that section's own object defines in it, so a label another
/// object ends at the same address never names its code; `own` gives the
/// mapping runs that section's own object marks (`object_mappings`), or
/// None when the object cannot be read and the image's mapping symbols
/// stand in.
pub(crate) fn inventory(
    image: &Image<'_>,
    name: &str,
    placed: &[Placed<'_>],
    uncredited: &[(i64, i64)],
    describe: &dyn Fn(&Placed<'_>) -> Result<(String, &'static str), String>,
    defined: &dyn Fn(&Placed<'_>) -> Result<Vec<Label>, String>,
    own: &dyn Fn(&Placed<'_>) -> Result<Option<Vec<(u32, Mode)>>, String>,
) -> Result<(Vec<ArmRow>, Vec<ArmRow>), String> {
    let mut pieces = Vec::new();
    for p in placed.iter().filter(|p| p.size > 0 && p.address >= 0 && image.executable(p.address as u32)) {
        let (start, end) = (p.address as u32, (p.address + p.size) as u32);
        let found = match own(p)? {
            Some(mappings) => runs(start, end, &mappings),
            None => runs(start, end, &image.mappings),
        };
        pieces.push((p, found));
    }
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
    let words_in = |from: u32, to: u32| {
        let mut words = Vec::new();
        let mut at = (from + 3) & !3;
        while at + 4 <= to {
            if let Some(word) = image.word(at) {
                words.push((at, word));
            }
            at += 4;
        }
        words
    };
    let mut rows = Vec::new();
    let mut likely_rows = Vec::new();
    for (piece, found) in &pieces {
        let arm = found
            .iter()
            .filter(|(_, _, mode)| *mode == Mode::Arm)
            .map(|&(from, to, _)| (from, to))
            .collect::<Vec<_>>();
        let data = found.iter().filter(|(_, _, mode)| *mode == Mode::Data).collect::<Vec<_>>();
        // Data words are read for ARM only in not-yet-C listings, and only
        // when one of them could anchor a likely run.
        let anchored = data
            .iter()
            .any(|(from, to, _)| words_in(*from, *to).iter().any(|(_, w)| arm_anchor(*w)));
        if arm.is_empty() && !anchored {
            continue;
        }
        let (object, origin) = describe(piece)?;
        let likely = if anchored && matches!(origin, "disassembly" | "listing") {
            data.iter().flat_map(|(from, to, _)| likely_arm(&words_in(*from, *to))).collect()
        } else {
            Vec::new()
        };
        if arm.is_empty() && likely.is_empty() {
            continue;
        }
        let (start, end) = (piece.address as u32, (piece.address + piece.size) as u32);
        let mut inside = defined(piece)?
            .into_iter()
            .filter(|label| label.address >= start && label.address < end)
            .filter(|label| !label.name.starts_with("AlchemyUncredited") && !label.name.starts_with(".L"))
            .collect::<Vec<_>>();
        inside.sort_by(|a, b| a.rank().cmp(&b.rank()));
        let globals = inside.iter().filter(|label| label.global).collect::<Vec<_>>();
        // Code before the first global takes the local label before it.
        let leading = |at: u32| {
            inside.iter().rev().find(|label| label.address <= at).map_or("-", |label| label.name.as_str())
        };
        let chosen = if globals.is_empty() { inside.iter().collect() } else { globals };
        let mut bounds: Vec<(u32, &str)> = Vec::new();
        for label in chosen {
            if bounds.last().is_none_or(|(at, _)| *at != label.address) {
                bounds.push((label.address, label.name.as_str()));
            }
        }
        // Likely ARM sits beside the listing's Thumb, never under `$a`.
        let thumb = modes[piece.object].1 > 0;
        let both = [modes[piece.object].0 > 0 && thumb, thumb];
        for ((ranges, out), mixed) in [(arm, &mut rows), (likely, &mut likely_rows)].into_iter().zip(both) {
            let mut functions: Vec<(&str, Vec<(u32, u32)>)> = Vec::new();
            for (from, to) in ranges {
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
                let words = ranges.iter().flat_map(|&(from, to)| words_in(from, to)).collect::<Vec<_>>();
                let evidence = analyze(&words, &literal, &in_code);
                let run = ranges[0].0;
                out.push(ArmRow {
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
    }
    Ok((rows, likely_rows))
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
        // A leaf saves lr too: push {r5-r9, sl, lr} ... pop {r5-r9, sl, lr}; bx lr.
        let leaf = plain(&[0xE92D_47E0, MOV_R0_R1, 0xE8BD_47E0, BX_LR]);
        assert!(leaf.markers.is_empty());
        assert_eq!(shape(&leaf, false), "compiled");
        // GCC 2.9's interworking return: push {lr} ... pop {ip}; bx ip.
        let interworking = plain(&[0xE92D_4000, MOV_R0_R1, 0xE8BD_1000, 0xE12F_FF1C]);
        assert!(interworking.markers.is_empty());
        assert_eq!(shape(&interworking, false), "compiled");
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
        // Their copy-and-test is subs rd, rm, #0; a shifted movs is theirs.
        assert_eq!(markers(&[0xE1B0_1000, BX_LR]), ["movs-unshifted"]); // movs r1, r0
        assert!(markers(&[0xE250_1000, 0xE1B0_1120, BX_LR]).is_empty()); // subs r1, r0, #0; movs r1, r0, lsr #2
        // Off sp they write a multiple back only in a block copy from r0
        // or for a doubleword pair.
        assert_eq!(markers(&[0xE8A2_00F0, BX_LR]), ["multiple-writeback"]); // stmia r2!, {r4-r7}
        assert_eq!(markers(&[0xE8B2_000E, BX_LR]), ["multiple-writeback"]); // ldmia r2!, {r1-r3}
        // ldmia r1!, {r0-r3}; stmia ip!, {r0-r3}; ldmia r1!, {r2, r3}; ldmia ip, {r0-r3}
        assert!(markers(&[0xE8B1_000F, 0xE8AC_000F, 0xE8B1_000C, 0xE89C_000F, BX_LR]).is_empty());
        // A compiled call through a register and a switch table are not marks.
        assert!(markers(&[0xE1A0_E00F, 0xE12F_FF13, 0x979F_F100, BX_LR]).is_empty());
        assert!(markers(&[0xE1A0_E00F, 0xE12F_FF1C, BX_LR]).is_empty()); // mov lr, pc; bx ip
        // Both halves kept outside mulsidi3's pair (RdLo, RdLo + 1, apart
        // from the inputs); lsl r3, r3, #16 then orr reads the low half.
        let unpaired = |smull: u32, orr: u32| markers(&[smull, 0xE1A0_3803, orr, BX_LR]);
        assert_eq!(unpaired(0xE0C3_1696, 0xE183_3821), ["multiply-unpaired"]); // smull r1, r3, r6, r6
        assert_eq!(unpaired(0xE0C3_2293, 0xE183_3822), ["multiply-unpaired"]); // smull r2, r3, r3, r2
        assert!(unpaired(0xE0C3_2696, 0xE183_3822).is_empty()); // smull r2, r3, r6, r6
        // smulsi3_highpart: any RdLo, but nothing reads it.
        assert!(markers(&[0xE0C3_1696, 0xE1A0_0003, BX_LR]).is_empty()); // smull r1, r3, r6, r6; mov r0, r3
        assert!(markers(&[0xE0C3_1696, 0xE3A0_1000, 0xE1A0_0001, BX_LR]).is_empty()); // ...; mov r1, #0; mov r0, r1
        // umulls r3, r0, r1, r3; mulne r3, r0, r2; subne r1, r1, r3: the
        // only read of RdLo follows a write on the same condition.
        assert!(markers(&[0xE090_3391, 0x1003_0290, 0x1041_1003, 0x112F_FF1E, BX_LR]).is_empty());
        // smlal reads and writes both halves.
        assert_eq!(markers(&[0xE0E3_1696, BX_LR]), ["multiply-unpaired"]); // smlal r1, r3, r6, r6
        assert!(markers(&[0xE0E3_2696, BX_LR]).is_empty()); // smlal r2, r3, r6, r6
        let e = plain(&[PUSH_R4_LR, 0xE08F_F100, POP_R4_PC]);
        assert_eq!(shape(&e, false), "hand");
        // Writing r5-r11 without saving them breaks the calling convention;
        // r4 is call-used under -fcall-used-r4.
        let clobbers = plain(&[0xE1A0_5000, BX_LR]); // mov r5, r0
        assert_eq!((register_list(clobbers.unsaved).as_str(), shape(&clobbers, false)), ("{r5}", "hand"));
        let call_used = plain(&[0xE1A0_4000, BX_LR]); // mov r4, r0
        assert_eq!((register_list(call_used.unsaved).as_str(), shape(&call_used, false)), ("{r4}", "compiled"));
        // Camelot's compilers push lr whenever they save registers, leaf
        // functions included: a save without lr is a hand marker.
        let no_lr = plain(&[0xE92D_0030, MOV_R0_R1, 0xE8BD_0030, BX_LR]); // push {r4, r5} ... pop {r4, r5}; bx lr
        assert_eq!(no_lr.markers.keys().copied().collect::<Vec<_>>(), ["save-no-lr"]);
        assert!(!no_lr.compiled_frame());
        assert_eq!(shape(&no_lr, false), "hand");
        // A varargs spill of argument registers alone is no such save.
        assert!(plain(&[0xE92D_000E, PUSH_R4_LR, POP_R4_PC]).markers.is_empty()); // push {r1-r3}; push {r4, lr}
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
        let defined = |p: &Placed<'_>| {
            Ok::<_, String>(match p.object {
                "a/A.o" => vec![label(0x0300_0000, "First", true, true), label(0x0300_0004, "loop", false, false)],
                _ => vec![label(0x0300_0010, "entry", false, false), label(0x0300_0014, "Second", true, true)],
            })
        };
        let (rows, likely) = inventory(&image, "main", &placed, &[(0x0300_0014, 4)], &describe, &defined, &from_image).unwrap();
        assert!(likely.is_empty());
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

    fn label(address: u32, name: &str, global: bool, function: bool) -> Label {
        Label { address, name: name.into(), global, function }
    }

    fn from_image(_: &Placed<'_>) -> Result<Option<Vec<(u32, Mode)>>, String> {
        Ok(None)
    }

    #[test]
    fn a_section_without_mapping_symbols_takes_its_modes_from_its_functions() {
        use object::elf::{STT_ARM_TFUNC, STT_FUNC, STT_NOTYPE};
        // agscc's and gcc_arm's own assemblers write no mapping symbols.
        let arm = [(0, ".gcc2_compiled.", STT_NOTYPE), (0, "Tile_BuildMetatiles", STT_FUNC)];
        assert_eq!(section_mappings(0x0801_58e8, &arm), [(0x0801_58e8, Mode::Arm)]);
        let thumb = [(0, "A", STT_ARM_TFUNC), (0x40, "B", STT_ARM_TFUNC), (0x81, "C", STT_FUNC)];
        assert_eq!(
            section_mappings(0x0800_0000, &thumb),
            [(0x0800_0000, Mode::Thumb), (0x0800_0040, Mode::Thumb), (0x0800_0080, Mode::Thumb)]
        );
        // A section's own mapping symbols win over its function symbols.
        let marked = [(0, "Main", STT_FUNC), (0, "$a", STT_NOTYPE), (0x10, "$d", STT_NOTYPE)];
        assert_eq!(section_mappings(0x100, &marked), [(0x100, Mode::Arm), (0x110, Mode::Data)]);
        assert!(section_mappings(0x100, &[(0, "table", STT_NOTYPE)]).is_empty());
    }

    #[test]
    fn a_mapping_symbol_another_object_leaves_at_its_end_never_sets_the_next_section() {
        // 080155d0.o ends with `$d` where BUILD_METATILES.o, compiled by
        // gcc_arm's own assembler, starts: the image reads all of
        // Tile_BuildMetatiles as data, its own object as ARM.
        let bytes = le(&[MOV_R0_R1, PUSH_R4_LR, MOV_R0_R1, POP_R4_PC, 0x0300_0000]);
        let image = Image {
            code: vec![(0x0801_58e4, &bytes)],
            segments: vec![],
            mappings: vec![(0x0801_58e4, Mode::Arm), (0x0801_58e8, Mode::Data)],
            labels: vec![(0x0801_58e4, "Func_080155d0".into(), true), (0x0801_58e8, "Tile_BuildMetatiles".into(), true)],
        };
        let listing = "obj/recon/tbs/raw/080155d0.o";
        let placed = [
            Placed { name: ".text", address: 0x0801_58e4, size: 4, object: listing },
            Placed { name: ".text", address: 0x0801_58e8, size: 16, object: "obj/games/X/BUILD_METATILES.o" },
        ];
        let describe = |p: &Placed<'_>| {
            Ok::<_, String>((p.object.to_owned(), if p.object == listing { "disassembly" } else { "C" }))
        };
        let defined = |p: &Placed<'_>| {
            Ok::<_, String>(if p.object == listing {
                vec![label(0x0801_58e4, "Func_080155d0", true, true)]
            } else {
                vec![label(0x0801_58e8, "Tile_BuildMetatiles", true, true)]
            })
        };
        let own = |p: &Placed<'_>| {
            Ok::<_, String>((p.object != listing).then(|| {
                section_mappings(p.address as u32, &[(0, "Tile_BuildMetatiles", object::elf::STT_FUNC)])
            }))
        };
        let (rows, _) = inventory(&image, "main", &placed, &[], &describe, &defined, &from_image).unwrap();
        assert_eq!(ArmTally::of(&rows).c, 0);
        let (rows, _) = inventory(&image, "main", &placed, &[], &describe, &defined, &own).unwrap();
        let shown = rows.iter().map(|r| (r.function.as_str(), r.bytes, r.origin)).collect::<Vec<_>>();
        assert_eq!(shown, [("Func_080155d0", 4, "disassembly"), ("Tile_BuildMetatiles", 16, "C")]);
        assert_eq!(ArmTally::of(&rows).c, 16);
    }

    fn le(list: &[u32]) -> Vec<u8> {
        list.iter().flat_map(|w| w.to_le_bytes()).collect()
    }

    #[test]
    fn a_label_another_object_ends_at_never_names_the_next_function() {
        // DECODE_2_LOOP.o ends with the global Resource_Decode2End at the
        // address where EXECUTE_TRANSFER.o's Resource_ExecuteTransfer
        // starts; the image holds both labels there, and the End label
        // sorts first.
        let bytes = le(&[BX_LR, PUSH_R4_LR, MOV_R0_R1, POP_R4_PC]);
        let image = Image {
            code: vec![(0x0800_2cf0, &bytes)],
            segments: vec![],
            mappings: vec![(0x0800_2cf0, Mode::Arm), (0x0800_2cf4, Mode::Arm)],
            labels: vec![
                (0x0800_2cf0, "Resource_DecodeType2Loop".into(), true),
                (0x0800_2cf4, "Resource_Decode2End".into(), true),
                (0x0800_2cf4, "Resource_ExecuteTransfer".into(), true),
            ],
        };
        let loop_o = "obj/games/X/SRC/SYSTEM/RESOURCE/DECODE_2_LOOP.o";
        let transfer_o = "obj/games/X/SRC/SYSTEM/RESOURCE/EXECUTE_TRANSFER.o";
        let placed = [
            Placed { name: ".text", address: 0x0800_2cf0, size: 4, object: loop_o },
            Placed { name: ".text", address: 0x0800_2cf4, size: 12, object: transfer_o },
        ];
        let describe = |p: &Placed<'_>| Ok::<_, String>((p.object.to_owned(), "assembly"));
        let defined = |p: &Placed<'_>| {
            Ok::<_, String>(if p.object == loop_o {
                vec![label(0x0800_2cf0, "Resource_DecodeType2Loop", true, true), label(0x0800_2cf4, "Resource_Decode2End", true, false)]
            } else {
                vec![label(0x0800_2cf4, "Resource_ExecuteTransfer", true, true)]
            })
        };
        let (rows, _) = inventory(&image, "main", &placed, &[], &describe, &defined, &from_image).unwrap();
        let named = rows.iter().map(|r| (r.function.as_str(), r.bytes)).collect::<Vec<_>>();
        assert_eq!(named, [("Resource_DecodeType2Loop", 4), ("Resource_ExecuteTransfer", 12)]);
    }

    #[test]
    fn at_one_address_a_function_or_global_names_the_code_before_an_end_label() {
        let mut labels = [
            label(0x10, "AEnd", true, false),
            label(0x10, "Zeta", true, true),
            label(0x10, "Beta", false, true),
            label(0x10, "Alpha", true, false),
        ];
        labels.sort_by(|a, b| a.rank().cmp(&b.rank()));
        assert_eq!(labels.iter().map(|l| l.name.as_str()).collect::<Vec<_>>(), ["Zeta", "Beta", "Alpha", "AEnd"]);
    }

    #[test]
    fn map_globals_are_kept_under_their_own_input_section() {
        let map = " .text          0x0000000008002850      0x4a4 /o/DECODE_2_LOOP.o
                0x0000000008002910                Resource_DecodeType2Loop
                0x0000000008002cf4                Resource_Decode2End
 .data          0x0000000008002cf4        0x0 /o/DECODE_2_LOOP.o
 .text          0x0000000008002cf4       0x68 /o/EXECUTE_TRANSFER.o
                0x0000000008002cf4                Resource_ExecuteTransfer
 .text.long_section_name
                0x0000000008002d5c       0x10 /o/LONG.o
                0x0000000008002d5c                Long_Entry
                0x0000000008002d6c                __end = .
";
        let found = map_globals(map);
        let key = |name: &str, address: i64, object: &str| (name.to_owned(), address, object.to_owned());
        assert_eq!(
            found[&key(".text", 0x0800_2850, "/o/DECODE_2_LOOP.o")],
            [(0x0800_2910, "Resource_DecodeType2Loop".to_owned()), (0x0800_2cf4, "Resource_Decode2End".to_owned())]
        );
        assert_eq!(found[&key(".text", 0x0800_2cf4, "/o/EXECUTE_TRANSFER.o")], [(0x0800_2cf4, "Resource_ExecuteTransfer".to_owned())]);
        assert_eq!(found[&key(".text.long_section_name", 0x0800_2d5c, "/o/LONG.o")], [(0x0800_2d5c, "Long_Entry".to_owned())]);
        assert_eq!(found.len(), 3);
    }

    fn at(list: &[u32]) -> Vec<(u32, u32)> {
        list.iter().enumerate().map(|(i, w)| (0x0802_12dc + 4 * i as u32, *w)).collect()
    }

    #[test]
    fn data_words_that_read_as_arm_code_are_likely_arm() {
        // recon/tla/raw/080212da.s: push {r5-r10, lr} ... pop {r5-r10, lr};
        // bx lr, with a literal word (an address) inside.
        let function = [0xe92d47e0, 0xe3a05000, 0xe24dd04c, 0x0300_1234, 0x5affffca, 0xe28dd008, 0xe8bd47e0, 0xe12fff1e];
        assert_eq!(likely_arm(&at(&function)), [(0x0802_12dc, 0x0802_12fc)]);
        // A swi with its return: 08013164.s.
        assert_eq!(likely_arm(&at(&[0xef030000, BX_LR])), [(0x0802_12dc, 0x0802_12e4)]);
        // A literal pool after the last executed word is not counted.
        let pooled = [PUSH_R4_LR, MOV_R0_R1, POP_R4_PC, 0x0800_1000, 0x0300_0000];
        assert_eq!(likely_arm(&at(&pooled)), [(0x0802_12dc, 0x0802_12e8)]);
    }

    #[test]
    fn data_that_does_not_read_as_arm_code_is_not_likely_arm() {
        // A pointer table, halfword pairs, zeros: no prologue or return.
        assert!(likely_arm(&at(&[0x0800_1000, 0x0800_2000, 0x0000_0000, 0x0012_0034, MOV_R0_R1])).is_empty());
        // A condition field of 0xF ends the run: the anchor stands alone
        // beside words that are not ARM7TDMI instructions.
        let broken = [0xffff_0000, MOV_R0_R1, BX_LR, 0xf000_0001];
        assert_eq!(likely_arm(&at(&broken)), [(0x0802_12e0, 0x0802_12e8)]);
        // One anchor word alone (a data word that reads as swi) is not enough.
        assert!(likely_arm(&at(&[0x0000_0001, 0xef12_3456, 0x0000_0002])).is_empty());
        // Words that never execute always are trimmed from both ends, and a
        // span where fewer than half execute always is not likely ARM.
        let lone = [0x1234_5678, 0x2345_6789, 0x3456_789a, MOV_R0_R1, BX_LR, 0x0000_0001, 0x4567_89ab];
        assert_eq!(likely_arm(&at(&lone)), [(0x0802_12dc + 12, 0x0802_12dc + 20)]);
        assert!(likely_arm(&at(&[0xe000_0001, 0x1234_5678, 0x2345_6789, 0x3456_789a, BX_LR])).is_empty());
        // A gap of more than 16 words that never execute always separates
        // two runs, so a table between two functions is not counted.
        let mut gapped = vec![PUSH_R4_LR, POP_R4_PC];
        gapped.extend([0x0800_0000; 20]);
        gapped.extend([MOV_R0_R1, BX_LR]);
        let spans = likely_arm(&at(&gapped));
        assert_eq!(spans, [(0x0802_12dc, 0x0802_12e4), (0x0802_12dc + 88, 0x0802_12dc + 96)]);
        // Words that do not follow each other are separate runs.
        let apart = [(0x100, PUSH_R4_LR), (0x104, POP_R4_PC), (0x200, MOV_R0_R1), (0x204, 0x1000_0000), (0x208, MOV_R0_R1)];
        assert_eq!(likely_arm(&apart), [(0x100, 0x108)]);
    }

    #[test]
    fn likely_arm_is_read_only_in_not_yet_c_listings_and_never_tallied() {
        // Thumb, then ARM written as data words in a raw listing.
        let bytes = le(&[0x47e0_0200, PUSH_R4_LR, MOV_R0_R1, POP_R4_PC, 0x0300_0000]);
        let image = Image {
            code: vec![(0x0802_12d8, &bytes)],
            segments: vec![],
            mappings: vec![(0x0802_12d8, Mode::Thumb), (0x0802_12dc, Mode::Data)],
            labels: vec![(0x0802_12d8, "Func_080212d8".into(), true)],
        };
        let raw = Placed { name: ".text", address: 0x0802_12d8, size: 20, object: "obj/recon/tla/raw/080212d8.o" };
        let defined = |_: &Placed<'_>| Ok::<_, String>(vec![label(0x0802_12d8, "Func_080212d8", true, true)]);
        let as_origin = |origin: &'static str| {
            move |p: &Placed<'_>| Ok::<_, String>((p.object.to_owned(), origin))
        };
        let (rows, likely) = inventory(&image, "main", &[raw], &[], &as_origin("disassembly"), &defined, &from_image).unwrap();
        assert!(rows.is_empty());
        let shown = likely.iter().map(|r| (r.function.as_str(), r.run, r.bytes, r.origin, r.shape)).collect::<Vec<_>>();
        assert_eq!(shown, [("Func_080212d8", 0x0802_12dc, 12, "disassembly", "mixed")]);
        assert_eq!(register_list(likely[0].evidence.save.unwrap()), "{r4,lr}");
        assert_eq!(ArmTally::of(&rows), ArmTally::default());
        // Data in maintained assembly or C is data, whatever it holds.
        let (rows, likely) = inventory(&image, "main", &[raw], &[], &as_origin("assembly"), &defined, &from_image).unwrap();
        assert!(rows.is_empty() && likely.is_empty());
    }
}
