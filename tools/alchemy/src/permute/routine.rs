//! One function's extent in an assembled object, as rows that compare the
//! way decomp-permuter compares objdump output: instruction words modulo
//! relocations, and relocation targets by symbol. A linked build names the
//! symbols, so an alias, or a constant the listing spells as a number, is the
//! same target as the symbol a candidate relocates against.

use object::{Object, ObjectSection, ObjectSymbol, RelocationFlags, RelocationTarget, SymbolKind};
use psynergy::decode::{decode_one, text_of, Kind};
use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

const R_ARM_ABS32: u32 = 2;
const R_ARM_THM_CALL: u32 = 10;
const R_ARM_THM_JUMP11: u32 = 102;
const R_ARM_THM_JUMP8: u32 = 103;

/// What an operand refers to once relocations and constants are resolved.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Value {
    /// An address or number.
    Abs(u32),
    /// A symbol the linked build does not define, plus an addend.
    Sym(String, i64),
    /// An offset inside this function.
    Local(u32),
}

impl Value {
    fn text(&self) -> String {
        match self {
            Value::Abs(value) => format!("{value:#x}"),
            Value::Sym(name, 0) => name.clone(),
            Value::Sym(name, addend) => format!("{name}{addend:+}"),
            Value::Local(_) => ".".into(),
        }
    }
}

/// The addresses a linked build gives its symbols.
#[derive(Default)]
pub struct Symbols {
    addresses: BTreeMap<String, u32>,
}

impl Symbols {
    pub fn load(path: &Path) -> Result<Symbols, String> {
        let data = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
        let file =
            object::File::parse(&*data).map_err(|error| format!("{}: {error}", path.display()))?;
        let mut addresses = BTreeMap::new();
        for symbol in file.symbols() {
            let Ok(name) = symbol.name() else { continue };
            if name.is_empty() || name.starts_with('$') || !defined(&symbol) {
                continue;
            }
            let st_type = match symbol.flags() {
                object::SymbolFlags::Elf { st_info, .. } => st_info & 0xf,
                _ => 0,
            };
            addresses
                .entry(name.to_string())
                .or_insert(linked_value(symbol.address() as u32, st_type));
        }
        Ok(Symbols { addresses })
    }

    pub fn get(&self, name: &str) -> Option<u32> {
        self.addresses.get(name).copied()
    }

    #[cfg(test)]
    pub fn from_pairs(pairs: &[(&str, u32)]) -> Symbols {
        Symbols {
            addresses: pairs
                .iter()
                .map(|(name, address)| (name.to_string(), *address))
                .collect(),
        }
    }
}

#[derive(Clone, Debug)]
pub struct Row {
    pub offset: u32,
    /// The mnemonic family the alignment matches rows by.
    pub key: String,
    pub text: String,
    /// A branch, table or address target inside the function.
    pub target: Option<u32>,
    pub raw: Vec<u8>,
    /// Whether the bytes themselves must match: false where a relocation or
    /// a resolved address decides them.
    pub literal: bool,
}

pub struct Routine {
    pub rows: Vec<Row>,
    pub size: u32,
    /// Alignment padding, compared only for exactness.
    pub padding: Vec<(u32, Vec<u8>)>,
    /// Literal pool words and what they hold, compared only for exactness.
    pub pool: Vec<(u32, String)>,
    /// Symbols the linked build does not define.
    pub unresolved: BTreeSet<String>,
}

struct Relocation {
    kind: u32,
    value: Value,
}

struct Reader<'a> {
    bytes: &'a [u8],
    start: u32,
    end: u32,
    linked: Option<u32>,
    relocations: BTreeMap<u32, Relocation>,
    own: BTreeMap<u32, String>,
}

impl Reader<'_> {
    fn word(&self, offset: u32) -> u32 {
        let at = offset as usize;
        self.bytes
            .get(at..at + 4)
            .map_or(0, |b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
    }

    fn inside(&self, offset: u32) -> bool {
        offset >= self.start && offset < self.end
    }

    /// The data word at `offset`: its relocation, or its constant.
    fn data(&self, offset: u32) -> Value {
        if let Some(relocation) = self.relocations.get(&offset) {
            return relocation.value.clone();
        }
        let constant = self.word(offset);
        if let Some(linked) = self.linked {
            let address = constant & !1;
            if address >= linked && address < linked + (self.end - self.start) {
                return Value::Local(address - linked);
            }
        }
        Value::Abs(constant)
    }

    /// A branch or call to `offset` in this object, without a relocation.
    fn code_target(&self, offset: u32, symbols: &Symbols) -> Value {
        if self.inside(offset) {
            return Value::Local(offset - self.start);
        }
        match self.own.get(&offset) {
            Some(name) => match symbols.get(name) {
                Some(address) => Value::Abs(address & !1),
                None => Value::Sym(name.clone(), 0),
            },
            None => Value::Sym(".text".into(), offset as i64),
        }
    }
}

fn resolve(
    file: &object::File,
    target: RelocationTarget,
    addend: i64,
    code: bool,
    section: object::SectionIndex,
    start: u32,
    end: u32,
    symbols: &Symbols,
) -> Value {
    let RelocationTarget::Symbol(index) = target else {
        return Value::Sym("?".into(), addend);
    };
    let Ok(symbol) = file.symbol_by_index(index) else {
        return Value::Sym("?".into(), addend);
    };
    let name = symbol.name().unwrap_or("?").to_string();
    let same_section = symbol.section_index() == Some(section);
    if same_section {
        let offset = (symbol.address() as i64 + addend) as u32 & !1;
        if offset >= start && offset < end {
            return Value::Local(offset - start);
        }
    }
    if symbol.kind() == SymbolKind::Section {
        return Value::Sym(name, addend);
    }
    match symbols.get(&name) {
        Some(address) => {
            let address = if code { address & !1 } else { address };
            Value::Abs((address as i64 + addend) as u32)
        }
        None => Value::Sym(name, addend),
    }
}

/// The rows of `function` in the object `data`. `linked` is the function's
/// address in the linked build, which turns constants that point into the
/// function (jump tables) into local targets.
pub fn routine(
    data: &[u8],
    function: &str,
    symbols: &Symbols,
    linked: Option<u32>,
) -> Result<Routine, String> {
    let file = object::File::parse(data).map_err(|error| error.to_string())?;
    let symbol = file
        .symbols()
        .find(|symbol| symbol.name() == Ok(function) && defined(symbol))
        .ok_or_else(|| {
            let names: Vec<&str> = file
                .symbols()
                .filter(|symbol| defined(symbol) && symbol.is_global())
                .filter_map(|symbol| symbol.name().ok())
                .collect();
            format!(
                "the object does not define {function}; it defines {}",
                names.join(", ")
            )
        })?;
    let section_index = symbol
        .section_index()
        .ok_or_else(|| format!("{function} has no section"))?;
    let section = file
        .section_by_index(section_index)
        .map_err(|error| error.to_string())?;
    let bytes = section.data().map_err(|error| error.to_string())?;
    let start = (symbol.address() & !1) as u32;
    let own: BTreeMap<u32, String> = file
        .symbols()
        .filter(|other| {
            other.section_index() == Some(section_index)
                && other.kind() != SymbolKind::Section
                && other
                    .name()
                    .is_ok_and(|name| !name.starts_with('$') && !name.starts_with(".L"))
        })
        .map(|other| {
            (
                (other.address() & !1) as u32,
                other.name().unwrap_or("").to_string(),
            )
        })
        .collect();
    let end = if symbol.size() > 0 {
        start + symbol.size() as u32
    } else {
        own.keys()
            .copied()
            .find(|address| *address > start)
            .unwrap_or(bytes.len() as u32)
    };
    let mut relocations = BTreeMap::new();
    let mut unresolved = BTreeSet::new();
    for (offset, relocation) in section.relocations() {
        let offset = offset as u32;
        let RelocationFlags::Elf { r_type } = relocation.flags() else {
            continue;
        };
        let code = r_type != R_ARM_ABS32;
        let addend = if relocation.has_implicit_addend() {
            match r_type {
                R_ARM_ABS32 => {
                    let at = offset as usize;
                    bytes
                        .get(at..at + 4)
                        .map_or(0, |b| i32::from_le_bytes([b[0], b[1], b[2], b[3]]) as i64)
                }
                R_ARM_THM_CALL => bytes
                    .get(offset as usize..offset as usize + 4)
                    .and_then(psynergy::thumb::bl_displacement)
                    .map_or(0, |displacement| displacement as i64 + 4),
                _ => 0,
            }
        } else {
            relocation.addend()
        };
        let value = resolve(
            &file,
            relocation.target(),
            addend,
            code,
            section_index,
            start,
            end,
            symbols,
        );
        if let Value::Sym(name, _) = &value {
            if offset >= start && offset < end && name != ".text" && name != "?" {
                unresolved.insert(name.clone());
            }
        }
        relocations.insert(
            offset,
            Relocation {
                kind: r_type,
                value,
            },
        );
    }
    let reader = Reader {
        bytes,
        start,
        end,
        linked,
        relocations,
        own,
    };
    let (rows, padding, pool) = sweep(&reader, symbols);
    Ok(Routine {
        rows,
        size: end - start,
        padding,
        pool,
        unresolved,
    })
}

/// Defined in a section. Old-ABI Thumb functions (`STT_ARM_TFUNC`) and Thumb
/// labels have processor-specific types that `is_definition` rejects.
/// The value a data word relocated against a symbol holds. Labels an
/// assembly listing marks as Thumb code (STT_ARM_TFUNC, or the older
/// STT_ARM_16BIT) keep an even symbol value, and the linker sets the Thumb
/// bit when it stores their address; compiled functions already carry it.
fn linked_value(value: u32, st_type: u8) -> u32 {
    const STT_ARM_TFUNC: u8 = 13;
    const STT_ARM_16BIT: u8 = 15;
    match st_type {
        STT_ARM_TFUNC | STT_ARM_16BIT => value | 1,
        _ => value,
    }
}

fn defined(symbol: &object::Symbol) -> bool {
    symbol.section_index().is_some() && symbol.kind() != SymbolKind::Section
}

fn raw(reader: &Reader, offset: u32, size: u32) -> Vec<u8> {
    reader
        .bytes
        .get(offset as usize..(offset + size) as usize)
        .map_or_else(Vec::new, <[u8]>::to_vec)
}

fn transfers(kind: &Kind) -> bool {
    matches!(
        kind,
        Kind::B { .. } | Kind::Bx(_) | Kind::Pop { pc: true, .. } | Kind::MovHi { rd: 15, .. }
    )
}

type Layout = (Vec<Row>, Vec<(u32, Vec<u8>)>, Vec<(u32, String)>);

fn sweep(reader: &Reader, symbols: &Symbols) -> Layout {
    let mut rows = Vec::new();
    let mut padding = Vec::new();
    let mut pool = Vec::new();
    let mut data = BTreeSet::new();
    let mut tables = BTreeSet::new();
    let mut after_transfer = false;
    let mut pc = reader.start;
    let local = |offset: u32| offset - reader.start;
    while pc < reader.end {
        if data.contains(&pc) || tables.contains(&pc) {
            let value = reader.data(pc);
            if tables.contains(&pc) && !data.contains(&pc) && !matches!(value, Value::Local(_)) {
                tables.remove(&pc);
                continue;
            }
            if tables.contains(&pc) && pc + 4 < reader.end {
                tables.insert(pc + 4);
            }
            let target = match value {
                Value::Local(offset) => Some(offset),
                _ => None,
            };
            if let Value::Local(offset) = value {
                let absolute = reader.start + offset;
                if absolute > pc && absolute % 4 == 0 && !data.contains(&absolute) {
                    tables.insert(absolute);
                }
            }
            if data.contains(&pc) && !tables.contains(&pc) {
                // A literal pool word: its load already carries the value,
                // so the word counts only toward exactness, with its place.
                let text = match value {
                    Value::Local(offset) => format!(".+{offset:#x}"),
                    other => other.text(),
                };
                pool.push((local(pc), text));
                pc += 4;
                continue;
            }
            let literal = !reader.relocations.contains_key(&pc) && target.is_none();
            rows.push(Row {
                offset: local(pc),
                key: ".word".into(),
                text: format!(".word {}", value.text()),
                target,
                raw: raw(reader, pc, 4),
                literal,
            });
            pc += 4;
            continue;
        }
        let half = reader.word(pc) & 0xffff;
        let next_is_data = data.contains(&(pc + 2)) || tables.contains(&(pc + 2));
        if after_transfer && pc % 4 == 2 && next_is_data && (half == 0 || half == 0x46c0) {
            padding.push((local(pc), raw(reader, pc, 2)));
            pc += 2;
            continue;
        }
        let Some(ins) = decode_one(reader.bytes, 0, pc) else {
            break;
        };
        let size = ins.size;
        after_transfer = transfers(&ins.kind);
        let relocation = reader.relocations.get(&pc);
        let (key, text, target, literal) = match &ins.kind {
            Kind::LdrPool { rd, .. } => {
                let pool = ((pc + 4) & !3) + (half & 0xff) * 4;
                data.insert(pool);
                let value = reader.data(pool);
                if let Value::Local(offset) = value {
                    let absolute = reader.start + offset;
                    if absolute > pc && absolute % 4 == 0 {
                        tables.insert(absolute);
                    }
                }
                let target = match value {
                    Value::Local(offset) => Some(offset),
                    _ => None,
                };
                (
                    "ldr=".to_string(),
                    format!("ldr r{rd}, ={}", value.text()),
                    target,
                    true,
                )
            }
            Kind::AddPc { rd, imm } => {
                let address = ((pc + 4) & !3) + imm;
                let target = reader.inside(address).then(|| local(address));
                ("adr".into(), format!("adr r{rd}"), target, true)
            }
            Kind::B { target } | Kind::Bcond { target, .. } => {
                let mnemonic = match &ins.kind {
                    Kind::Bcond { cond, .. } => cond.mnemonic().to_string(),
                    _ => "b".to_string(),
                };
                match relocation {
                    Some(relocation)
                        if matches!(relocation.kind, R_ARM_THM_JUMP11 | R_ARM_THM_JUMP8) =>
                    {
                        let target = match relocation.value {
                            Value::Local(offset) => Some(offset),
                            _ => None,
                        };
                        (
                            mnemonic.clone(),
                            format!("{mnemonic} {}", relocation.value.text()),
                            target,
                            false,
                        )
                    }
                    _ => match reader.code_target(*target, symbols) {
                        Value::Local(offset) => (mnemonic.clone(), mnemonic, Some(offset), true),
                        other => (
                            mnemonic.clone(),
                            format!("{mnemonic} {}", other.text()),
                            None,
                            false,
                        ),
                    },
                }
            }
            Kind::Bl { target } => {
                let value = match relocation {
                    Some(relocation) => relocation.value.clone(),
                    None => reader.code_target(*target, symbols),
                };
                let target = match value {
                    Value::Local(offset) => Some(offset),
                    _ => None,
                };
                ("bl".into(), format!("bl {}", value.text()), target, false)
            }
            other => {
                let text = text_of(other);
                let key = text.split_whitespace().next().unwrap_or("").to_string();
                (key, text, None, true)
            }
        };
        rows.push(Row {
            offset: local(pc),
            key,
            text,
            target,
            raw: raw(reader, pc, size),
            literal,
        });
        pc += size;
    }
    (rows, padding, pool)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn thumb_labels_are_stored_with_the_thumb_bit() {
        // An assembly `.thumb_func` label: the pool word holds address | 1.
        assert_eq!(linked_value(0x0809_4bbc, 13), 0x0809_4bbd);
        assert_eq!(linked_value(0x0809_4bbc, 15), 0x0809_4bbd);
        // A compiled Thumb function already has the bit; data has none.
        assert_eq!(linked_value(0x0808_b869, 2), 0x0808_b869);
        assert_eq!(linked_value(0x0202_c000, 0), 0x0202_c000);
        assert_eq!(linked_value(0x0202_c000, 1), 0x0202_c000);
    }
}
