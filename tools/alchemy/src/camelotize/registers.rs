use super::source::{closing, is_ident, mask, next, previous, store, text, trim, type_name};
use super::stabs::Int;
use std::collections::{BTreeSet, HashMap};
use std::ops::Range;
use std::path::{Path, PathBuf};

pub(crate) const HEADERS: [&str; 2] = [
    "games/common/include/system/io_reg.h",
    "games/common/include/sound/audio_engine_types.h",
];

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Register {
    pub name: String,
    pub ty: Int,
    pub address: u32,
    pub pointer: bool,
    pub header: PathBuf,
}

pub(crate) fn defines(text: &str, header: &Path) -> Vec<Register> {
    let mut found = Vec::new();
    for line in text.lines() {
        let line = line.split("/*").next().unwrap_or("").trim();
        let Some(rest) = line.strip_prefix('#').map(str::trim_start).and_then(|r| r.strip_prefix("define ")) else {
            continue;
        };
        let rest = rest.trim_start();
        let name_end = rest.bytes().position(|b| !is_ident(b)).unwrap_or(rest.len());
        let (name, body) = (&rest[..name_end], rest[name_end..].replace(' ', ""));
        let (pointer, inner) = if let Some(inner) = body.strip_prefix("(*(volatile").and_then(|b| b.strip_suffix(')')) {
            (false, inner)
        } else if let Some(inner) = body.strip_prefix("((volatile").and_then(|b| b.strip_suffix(')')) {
            (true, inner)
        } else {
            continue;
        };
        let Some((ty, address)) = inner.split_once("*)") else {
            continue;
        };
        let (Some(ty), Some(address)) = (Int::parse(ty), number(address)) else {
            continue;
        };
        if !name.is_empty() {
            found.push(Register { name: name.to_string(), ty, address, pointer, header: header.to_path_buf() });
        }
    }
    found
}

fn number(text: &str) -> Option<u32> {
    let digits = text.trim_end_matches(['u', 'U', 'l', 'L']);
    match digits.strip_prefix("0x").or_else(|| digits.strip_prefix("0X")) {
        Some(hex) => u32::from_str_radix(hex, 16).ok(),
        None => digits.parse().ok(),
    }
}

pub(crate) struct Table {
    names: HashMap<(Int, u32, bool), Register>,
}

impl Table {
    pub(crate) fn new(registers: Vec<Register>) -> Table {
        let mut names = HashMap::new();
        for register in registers {
            names.entry((register.ty, register.address, register.pointer)).or_insert(register);
        }
        Table { names }
    }

    fn get(&self, ty: Int, address: u32, pointer: bool) -> Option<&Register> {
        self.names.get(&(ty, address, pointer))
    }

    fn value(&self, ty: Int, address: u32, store: bool) -> Option<&Register> {
        self.get(ty, address, false).or_else(|| {
            let other = Int { bytes: ty.bytes, signed: !ty.signed };
            store.then(|| self.get(other, address, false)).flatten()
        })
    }
}

fn address(mask: &[u8], at: usize) -> Option<(u32, usize)> {
    if mask[at] == b'(' {
        let close = closing(mask, at)?;
        let inner = trim(mask, at + 1..close);
        return Some((number(text(mask, inner))?, close + 1));
    }
    let end = (at..mask.len()).find(|&i| !is_ident(mask[i])).unwrap_or(mask.len());
    mask[at].is_ascii_digit().then_some(())?;
    Some((number(text(mask, at..end))?, end))
}

pub(crate) fn rewrite(text_in: &str, table: &Table) -> (String, usize, BTreeSet<PathBuf>) {
    let mask = mask(text_in);
    let mut edits: Vec<(Range<usize>, String)> = Vec::new();
    let mut headers = BTreeSet::new();
    let mut at = 0;
    while at < mask.len() {
        let found = (mask[at] == b'(').then(|| register_at(&mask, at, table)).flatten();
        match found {
            Some((span, written, header)) => {
                at = span.end;
                edits.push((span, written));
                headers.insert(header);
            }
            None => at += 1,
        }
    }
    let mut out = String::with_capacity(text_in.len());
    let mut last = 0;
    for (span, written) in &edits {
        out.push_str(&text_in[last..span.start]);
        out.push_str(written);
        last = span.end;
    }
    out.push_str(&text_in[last..]);
    (out, edits.len(), headers)
}

fn pointer_type(name: &str) -> Option<(bool, Int)> {
    let pointee = name.strip_suffix(" *")?;
    match pointee.strip_prefix("volatile ") {
        Some(rest) => Some((true, Int::parse(rest)?)),
        None => match Int::parse(pointee) {
            Some(ty) => Some((false, ty)),
            None => Some((true, Int::parse(pointee.strip_prefix('v')?)?)),
        },
    }
}

fn register_at(mask: &[u8], at: usize, table: &Table) -> Option<(Range<usize>, String, PathBuf)> {
    let (name, end) = type_name(mask, at)?;
    let (volatile, ty) = pointer_type(&name)?;
    let start = next(mask, end)?;
    let (value, after) = address(mask, start)?;
    if next(mask, after).is_some_and(|i| mask[i] == b'[') {
        return None;
    }
    if let Some(star) = previous(mask, at).filter(|&p| mask[p] == b'*') {
        let register = table.value(ty, value, store(mask, after))?;
        return Some((star..after, register.name.clone(), register.header.clone()));
    }
    if !volatile {
        return None;
    }
    if let Some(register) = table.get(ty, value, true) {
        return Some((at..after, register.name.clone(), register.header.clone()));
    }
    let register = table.get(ty, value, false)?;
    Some((at..after, format!("&{}", register.name), register.header.clone()))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn table() -> Table {
        let header = "#define REG_DISPCNT (*(volatile u16 *)0x04000000)\n#define REG_BG2X (*(volatile s32 *)0x04000028)\n#define REG_DMA0 ((volatile u16 *)0x040000b0)\n#define REG_DMA3SAD (*(volatile u32 *)0x040000d4)\n#define REG_DMA3 ((volatile u32 *)0x040000d4)\n#define REG_IME (*(volatile u16 *)0x04000208)\n#define REG_BLDCNT (*(volatile u16 *)0x04000050)\n";
        Table::new(defines(header, Path::new("io_reg.h")))
    }

    #[test]
    fn registers_are_named_by_type_and_address() {
        let (out, count, headers) = rewrite(
            "void F(void)\n{\n    *(vu16 *)0x04000000 = 0;\n    *(volatile u16 *)0x4000000 = 1;\n    G(*(u16 *)(0x04000000));\n    *(s32 *)0x4000028 = 2;\n    *(s16 *)0x4000050 = 3;\n    G(*(s16 *)0x4000050);\n    Dma((volatile u32 *)0x040000d4, (volatile u16 *)0x04000208, (volatile u16 *)0x40000b0);\n}\n",
            &table(),
        );
        assert_eq!(count, 8);
        assert!(out.contains("    REG_DISPCNT = 0;\n    REG_DISPCNT = 1;\n    G(REG_DISPCNT);\n    REG_BG2X = 2;\n    REG_BLDCNT = 3;\n"));
        assert!(out.contains("    G(*(s16 *)0x4000050);\n"));
        assert!(out.contains("Dma(REG_DMA3, &REG_IME, REG_DMA0);"));
        assert_eq!(headers.into_iter().collect::<Vec<_>>(), [PathBuf::from("io_reg.h")]);
    }

    #[test]
    fn a_register_never_takes_the_parenthesis_of_its_call() {
        let (out, count, _) = rewrite("void F(void)\n{\n    G(*(volatile u16 *)0x04000000);\n    H((u16 *)0x04000000);\n}\n", &table());
        assert_eq!(count, 1);
        assert!(out.contains("    G(REG_DISPCNT);\n    H((u16 *)0x04000000);\n"));
    }
}
