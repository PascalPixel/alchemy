//! A built object's functions as code that can be compared between objects:
//! each function's bytes with every relocated field cleared, and what each
//! relocation names. A relocation against a section is read back to the
//! symbol the field points into, so a function's own jump table, a static
//! function and a static table are known by name and not by where their
//! object happened to place them.

use object::{Object, ObjectSection, ObjectSymbol, RelocationFlags, RelocationTarget, SectionIndex, SectionKind, SymbolKind};
use std::collections::BTreeMap;

const R_ARM_PC24: u32 = 1;
const R_ARM_ABS32: u32 = 2;
const R_ARM_THM_CALL: u32 = 10;

/// What a relocation names when it names its own function.
pub(crate) const OWN: &str = ".";

#[derive(Clone, Debug, Default, PartialEq, Eq, Hash)]
pub(crate) struct Code {
    /// The function's extent, literal pool included, relocated fields cleared.
    pub bytes: Vec<u8>,
    /// Each relocation's place, type and offset from what it names.
    pub fields: Vec<(u32, u32, i64)>,
    /// What each relocation names, in the order of `fields`.
    pub targets: Vec<String>,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Function {
    pub name: String,
    /// Whether other objects can name it.
    pub global: bool,
    pub code: Code,
}

/// GCC numbers a nested function's symbol (`Step.12`) by its place in the
/// file; the name without the number is the source's.
fn plain(name: &str) -> &str {
    match name.rsplit_once('.') {
        Some((head, digits)) if !head.is_empty() && !digits.is_empty() && digits.bytes().all(|b| b.is_ascii_digit()) => head,
        _ => name,
    }
}

/// The value a relocated field holds before linking, and the field cleared.
fn take(kind: u32, field: &mut [u8]) -> Option<i64> {
    match kind {
        R_ARM_ABS32 => {
            let value = u32::from_le_bytes(field.try_into().ok()?);
            field.fill(0);
            Some(i64::from(value))
        }
        // A Thumb call's displacement counts from four bytes past its place.
        R_ARM_THM_CALL => {
            let displacement = crate::thumb::bl_displacement(field)?;
            field.copy_from_slice(&[0x00, 0xf0, 0x00, 0xf8]);
            Some(i64::from(displacement) + 4)
        }
        // An ARM branch's counts in words from eight bytes past.
        R_ARM_PC24 => {
            let word = u32::from_le_bytes(field.try_into().ok()?);
            field[..3].fill(0);
            Some(i64::from(((word << 8) as i32) >> 6) + 8)
        }
        _ => None,
    }
}

struct Place {
    start: u64,
    end: u64,
    name: String,
    global: bool,
}

/// The functions of an object file in the order of its code. A symbol
/// with no size, as assembly leaves a label, runs to the next symbol that
/// has one or that other objects can name.
pub(crate) fn functions(bytes: &[u8]) -> Result<Vec<Function>, String> {
    let file = object::File::parse(bytes).map_err(|e| e.to_string())?;
    let text = |section: SectionIndex| file.section_by_index(section).is_ok_and(|s| s.kind() == SectionKind::Text);
    // Every named place of every section, by address.
    let mut places: BTreeMap<usize, Vec<Place>> = BTreeMap::new();
    for symbol in file.symbols() {
        let (Some(section), Ok(name)) = (symbol.section_index(), symbol.name()) else {
            continue;
        };
        if matches!(symbol.kind(), SymbolKind::Section | SymbolKind::File) || name.is_empty() || name.starts_with(['$', '.']) {
            continue;
        }
        let start = if text(section) { symbol.address() & !1 } else { symbol.address() };
        places.entry(section.0).or_default().push(Place { start, end: start + symbol.size(), name: name.to_string(), global: symbol.is_global() });
    }
    for (section, list) in &mut places {
        list.sort_by(|a, b| (a.start, &a.name).cmp(&(b.start, &b.name)));
        let Ok(section) = file.section_by_index(SectionIndex(*section)) else {
            continue;
        };
        if section.kind() == SectionKind::Text {
            let limit = section.address() + section.size();
            for at in 0..list.len() {
                if list[at].end == list[at].start {
                    let next = list[at + 1..].iter().find(|other| other.start > list[at].start && (other.global || other.end > other.start));
                    list[at].end = next.map_or(limit, |other| other.start);
                }
            }
        }
    }
    let named = |section: SectionIndex, at: u64| -> Option<&Place> {
        let list = places.get(&section.0)?;
        let before = &list[..list.partition_point(|place| place.start <= at)];
        before.iter().rev().find(|place| at < place.end || place.start == place.end)
    };
    let mut found = Vec::new();
    for section in file.sections().filter(|s| s.kind() == SectionKind::Text) {
        let data = section.data().map_err(|e| e.to_string())?;
        let base = section.address();
        let mut relocations: Vec<_> = section.relocations().collect();
        relocations.sort_by_key(|(offset, _)| *offset);
        for place in places.get(&section.index().0).into_iter().flatten() {
            let (from, to) = ((place.start - base) as usize, ((place.end - base) as usize).min(data.len()));
            let mut code = Code { bytes: data.get(from..to).unwrap_or_default().to_vec(), ..Code::default() };
            for (offset, relocation) in relocations.iter().filter(|(offset, _)| (place.start..place.end).contains(&(offset + base))) {
                let at = (offset + base - place.start) as usize;
                let RelocationFlags::Elf { r_type: kind } = relocation.flags() else {
                    return Err(format!("{}: a relocation that is not ELF's", place.name));
                };
                let RelocationTarget::Symbol(index) = relocation.target() else {
                    return Err(format!("{}: a relocation with no symbol", place.name));
                };
                let symbol = file.symbol_by_index(index).map_err(|e| e.to_string())?;
                let value = code.bytes.get_mut(at..at + 4).and_then(|field| take(kind, field));
                let (target, value) = match (symbol.kind(), symbol.section_index(), value) {
                    // Into a section: the symbol that holds the place.
                    (SymbolKind::Section, Some(into), Some(value)) => match named(into, value as u64) {
                        Some(holder) => (holder.name.as_str(), value - holder.start as i64),
                        None => (file.section_by_index(into).ok().and_then(|s| s.name().ok()).unwrap_or(""), value),
                    },
                    (_, _, value) => (symbol.name().unwrap_or(""), value.unwrap_or(0)),
                };
                code.fields.push((at as u32, kind, value));
                code.targets.push(if target == place.name { OWN.to_string() } else { plain(target).to_string() });
            }
            found.push(((section.index().0, place.start), Function { name: plain(&place.name).to_string(), global: place.global, code }));
        }
    }
    found.sort_by_key(|(at, _)| *at);
    Ok(found.into_iter().map(|(_, function)| function).collect())
}

#[cfg(test)]
mod tests {
    use super::*;

    const TEXT: u16 = 1;
    const DATA: u16 = 2;
    const LOCAL: u8 = 0x00;
    const GLOBAL_FUNCTION: u8 = 0x12;
    const GLOBAL: u8 = 0x10;

    /// A section's name, type, flags, link, info and entry size, and its contents.
    type Section<'a> = (u32, u32, u32, u32, u32, u32, &'a [u8]);

    /// A relocatable ARM object of a `.text`, a `.data` of sixteen bytes,
    /// `.text`'s relocations (place, symbol, type) and the symbols after
    /// the two sections' own (name, value, size, kind, section).
    fn object(text: &[u8], relocations: &[(u32, u32, u32)], symbols: &[(&str, u32, u32, u8, u16)]) -> Vec<u8> {
        let words = |values: &[u32]| values.iter().flat_map(|value| value.to_le_bytes()).collect::<Vec<u8>>();
        let mut strings = vec![0u8];
        let mut table = vec![0u8; 16];
        for section in [TEXT, DATA] {
            table.extend([words(&[0, 0, 0]), vec![3, 0], section.to_le_bytes().to_vec()].concat());
        }
        for (name, value, size, kind, section) in symbols {
            table.extend([words(&[strings.len() as u32, *value, *size]), vec![*kind, 0], section.to_le_bytes().to_vec()].concat());
            strings.extend(name.bytes().chain([0]));
        }
        let relocated: Vec<u8> = relocations.iter().flat_map(|(place, symbol, kind)| words(&[*place, (symbol << 8) | kind])).collect();
        let names = b"\0.text\0.data\0.rel.text\0.symtab\0.strtab\0.shstrtab\0";
        let sections: [Section; 7] = [
            (0, 0, 0, 0, 0, 0, &[]),
            (1, 1, 6, 0, 0, 0, text),
            (7, 1, 3, 0, 0, 0, &[0; 16]),
            (13, 9, 0, 4, 1, 8, &relocated),
            (23, 2, 0, 5, 3, 16, &table),
            (31, 3, 0, 0, 0, 0, &strings),
            (39, 3, 0, 0, 0, 0, names),
        ];
        let mut body = Vec::new();
        let mut headers = Vec::new();
        for (name, kind, flags, link, info, entry, contents) in sections {
            headers.extend(words(&[name, kind, flags, 0, 52 + body.len() as u32, contents.len() as u32, link, info, 4, entry]));
            body.extend_from_slice(contents);
            body.resize(body.len().next_multiple_of(4), 0);
        }
        let mut file = vec![0x7f, b'E', b'L', b'F', 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 40, 0];
        file.extend(words(&[1, 0, 0, 52 + body.len() as u32, 0]));
        file.extend([52u16, 0, 0, 40, 7, 6].iter().flat_map(|value| value.to_le_bytes()));
        [file, body, headers].concat()
    }

    #[test]
    fn a_relocation_is_read_back_to_the_name_it_points_into() {
        let mut text = Vec::new();
        // Walk: a call of Step, two instructions, a pointer eight bytes
        // into .data: four into sTable.
        text.extend([0xff, 0xf7, 0xfe, 0xff, 0x00, 0x20, 0x70, 0x47, 8, 0, 0, 0]);
        // Step: a branch with link to .text + 16, which is its own fifth
        // byte, and a pointer to .text + 0: Walk.
        text.extend([0x00, 0xf0, 0x06, 0xf8, 0, 0, 0, 0]);
        // Two labels with no size.
        text.extend([0x00, 0x4c, 0x20, 0x47, 0, 0, 0, 0, 0x00, 0x4c, 0x20, 0x47, 0, 0, 0, 0]);
        let relocations = [(0, 5, R_ARM_THM_CALL), (8, 2, R_ARM_ABS32), (12, 1, R_ARM_THM_CALL), (16, 1, R_ARM_ABS32), (24, 8, R_ARM_ABS32), (32, 8, R_ARM_ABS32)];
        let symbols = [
            ("sTable", 4, 0, LOCAL, DATA),
            ("Walk", 0, 12, GLOBAL_FUNCTION, TEXT),
            ("Step.3", 12, 8, GLOBAL_FUNCTION, TEXT),
            ("Blend", 20, 0, GLOBAL, TEXT),
            ("SetBlend", 28, 0, GLOBAL, TEXT),
            ("FarBlend", 0, 0, GLOBAL, 0),
        ];
        let found = functions(&object(&text, &relocations, &symbols)).unwrap();
        let named: Vec<(&str, usize, Vec<&str>)> = found.iter().map(|f| (f.name.as_str(), f.code.bytes.len(), f.code.targets.iter().map(String::as_str).collect())).collect();
        assert_eq!(named, [("Walk", 12, vec!["Step", "sTable"]), ("Step", 8, vec![OWN, "Walk"]), ("Blend", 8, vec!["FarBlend"]), ("SetBlend", 8, vec!["FarBlend"])]);
        assert_eq!(found[0].code.fields, [(0, R_ARM_THM_CALL, 0), (8, R_ARM_ABS32, 4)]);
        assert_eq!(found[0].code.bytes, [0x00, 0xf0, 0x00, 0xf8, 0x00, 0x20, 0x70, 0x47, 0, 0, 0, 0]);
        assert_eq!(found[1].code.fields, [(0, R_ARM_THM_CALL, 4), (4, R_ARM_ABS32, 0)]);
        // The two veneers are the same code under two names.
        assert_eq!(found[2].code, found[3].code);
        assert!(found.iter().all(|function| function.global));
    }

    #[test]
    fn an_arm_branch_counts_in_words_from_eight_bytes_past() {
        let mut field = [0xfe, 0xff, 0xff, 0xeb];
        assert_eq!(take(R_ARM_PC24, &mut field), Some(0));
        assert_eq!(field, [0, 0, 0, 0xeb]);
        assert_eq!(take(99, &mut field), None);
    }
}
