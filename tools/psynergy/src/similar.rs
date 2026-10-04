//! Function similarity: normalise Thumb code so that only its shape remains
//! (opcodes, registers and small constants), then rank functions by the edit
//! distance between their normalised instruction sequences.
//!
//! The index is rebuilt from linked images on every run; nothing is stored.

use crate::decode::{self, Ins, Kind};
use crate::elf::Elf;
use std::collections::{BTreeMap, BTreeSet};

/// Where a function's bytes come from in the build.
#[derive(Clone, Copy, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
pub enum Origin {
    /// Compiled from C source.
    C,
    /// Maintained assembly source.
    Assembly,
    /// Disassembly that has not been rewritten as source yet.
    NotYetC,
    /// Anything else the image links (toolchain libraries and the like).
    Other,
}

impl Origin {
    pub fn label(self) -> &'static str {
        match self {
            Origin::C => "c",
            Origin::Assembly => "assembly",
            Origin::NotYetC => "not-yet-c",
            Origin::Other => "other",
        }
    }
    pub fn parse(text: &str) -> Option<Origin> {
        Some(match text {
            "c" => Origin::C,
            "assembly" => Origin::Assembly,
            "not-yet-c" => Origin::NotYetC,
            "other" => Origin::Other,
            _ => return None,
        })
    }
}

#[derive(Clone, Debug)]
pub struct Function {
    /// The build the function was read from.
    pub build: String,
    /// The linked image within the build: the main image or one overlay,
    /// which share load addresses with each other.
    pub image: String,
    pub name: String,
    pub address: u32,
    /// Bytes from the function's start to the next function (pools included).
    pub bytes: u32,
    pub origin: Origin,
    /// The source or object the linker placed the function from.
    pub source: String,
}

/// One linked image: its ELF and the linker map that attributes each input
/// section to the object it came from.
pub struct Image<'a> {
    pub build: &'a str,
    /// The image's name within its build, such as its ELF's stem.
    pub name: &'a str,
    pub elf: &'a Elf,
    pub map: &'a str,
}

/// An input `.text*` section the map places: address, size and object path.
pub(crate) fn map_text_sections(map: &str) -> Vec<(u32, u32, String)> {
    let mut placed = Vec::new();
    let mut pending: Option<()> = None;
    for line in map.lines() {
        let fields: Vec<&str> = line.split_whitespace().collect();
        let starts_text = line.starts_with(" .text");
        let parse = |fields: &[&str]| -> Option<(u32, u32, String)> {
            let address = u32::from_str_radix(fields.first()?.strip_prefix("0x")?, 16).ok()?;
            let size = u32::from_str_radix(fields.get(1)?.strip_prefix("0x")?, 16).ok()?;
            let object = fields.get(2..)?.join(" ");
            (!object.is_empty() && size > 0).then_some((address, size, object))
        };
        if starts_text {
            if fields.len() >= 4 {
                pending = None;
                if let Some(entry) = parse(&fields[1..]) {
                    placed.push(entry);
                }
            } else {
                pending = Some(());
            }
        } else if pending.take().is_some() {
            if let Some(entry) = parse(&fields) {
                placed.push(entry);
            }
        }
    }
    placed
}

fn is_thumb_symbol(value: u32, kind: u8) -> bool {
    value & 1 != 0 || kind == 13 || kind == 15
}

fn preferred(names: &[&str]) -> String {
    names
        .iter()
        .find(|n| !(n.starts_with("Func_") || n.starts_with("Unnamed_") || n.starts_with("sub_")))
        .or_else(|| names.first())
        .map(|n| n.to_string())
        .unwrap_or_default()
}

/// Decodes `[start, end)` in one linear pass, skipping the literal-pool words
/// that earlier loads read. Linear time, whatever the number of functions.
pub(crate) fn sweep(image: &[u8], base: u32, start: u32, end: u32) -> Vec<Ins> {
    let mut pool = BTreeSet::new();
    let mut out = Vec::new();
    let mut pc = start;
    while pc + 2 <= end {
        if pool.contains(&pc) {
            pc += 2;
            continue;
        }
        let Some(ins) = decode::decode_one(image, base, pc) else {
            pc += 2;
            continue;
        };
        if let Kind::LdrPool { .. } = ins.kind {
            if let Some(half) = decode::half_at(image, (pc - base) as usize) {
                let word = (pc & !3) + 4 + u32::from(half & 0xff) * 4;
                pool.insert(word);
                pool.insert(word + 2);
            }
        }
        pc += ins.size;
        if !matches!(ins.kind, Kind::Unknown(_)) {
            out.push(ins);
        }
    }
    out
}

/// Splits every placed text section of `image` into functions with their
/// decoded instructions. `classify` turns an object path into the origin and
/// source path to report; only sections whose origin is `wanted` are decoded.
pub fn decoded(
    image: &Image,
    classify: &dyn Fn(&str) -> (Origin, String),
    wanted: &dyn Fn(Origin) -> bool,
) -> Vec<(Function, Vec<Ins>)> {
    let mut names: BTreeMap<u32, Vec<&str>> = BTreeMap::new();
    let mut thumb: BTreeSet<u32> = BTreeSet::new();
    let mut named: BTreeSet<u32> = BTreeSet::new();
    for symbol in &image.elf.symbols {
        // Absolute symbols (the linker's `*ABS*` markers, ids) name no code.
        if symbol.section == 0
            || symbol.section == 0xfff1
            || symbol.name.is_empty()
            || symbol.name.starts_with('$')
            || symbol.name.starts_with('.')
            || symbol.name.starts_with(".L")
            || !matches!(symbol.kind, 0 | 2 | 13 | 15)
        {
            continue;
        }
        let address = symbol.value & !1;
        named.insert(address);
        if is_thumb_symbol(symbol.value, symbol.kind) {
            thumb.insert(address);
            names.entry(address).or_default().push(&symbol.name);
        }
    }
    let mut out = Vec::new();
    for (start, size, object) in map_text_sections(image.map) {
        let Some((section, offset)) = image.elf.section_at(start) else {
            continue;
        };
        let end = start + size;
        // A section whose first symbol is an even untyped label is ARM code.
        if named.contains(&start) && !thumb.contains(&start) {
            continue;
        }
        let (origin, source) = classify(&object);
        if !wanted(origin) {
            continue;
        }
        let span = size.min((section.bytes.len() - offset) as u32);
        let ins = sweep(&section.bytes, section.address, start, start + span);
        if ins.is_empty() {
            continue;
        }
        let mut starts: BTreeSet<u32> = thumb.range(start..end).copied().collect();
        starts.insert(start);
        let mut after_return = false;
        for i in &ins {
            if after_return && matches!(i.kind, Kind::Push { lr: true, .. }) {
                starts.insert(i.addr);
            }
            after_return = matches!(i.kind, Kind::Bx(_) | Kind::Pop { pc: true, .. });
        }
        let bounds: Vec<u32> = starts.iter().copied().chain([end]).collect();
        for pair in bounds.windows(2) {
            let (from, to) = (pair[0], pair[1]);
            let first = ins.partition_point(|i| i.addr < from);
            let last = ins.partition_point(|i| i.addr < to);
            let body = &ins[first..last];
            if body.is_empty() {
                continue;
            }
            let name = names
                .get(&from)
                .map(|n| preferred(n))
                .unwrap_or_else(|| format!("sub_{from:08x}"));
            out.push((
                Function {
                    build: image.build.to_string(),
                    image: image.name.to_string(),
                    name,
                    address: from,
                    bytes: to - from,
                    origin,
                    source: source.clone(),
                },
                body.to_vec(),
            ));
        }
    }
    out
}
