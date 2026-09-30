//! Function similarity: normalise Thumb code so that only its shape remains
//! (opcodes, registers and small constants), then rank functions by the edit
//! distance between their normalised instruction sequences.
//!
//! The index is rebuilt from linked images on every run; nothing is stored.

use crate::decode::{self, Ins, Kind};
use crate::elf::Elf;
use std::collections::{BTreeMap, BTreeSet, HashMap};

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
    /// Normalised instructions, interned.
    pub tokens: Vec<u32>,
}

/// Branch, call and literal-pool addresses, and constants that are addresses,
/// are masked; opcodes, registers and other constants stay.
pub fn normalise(ins: &Ins) -> String {
    match &ins.kind {
        Kind::B { .. } => "b".into(),
        Kind::Bcond { cond, .. } => format!("b{}", cond.mnemonic()),
        Kind::Bl { .. } => "bl".into(),
        Kind::LdrPool { rd, word } => {
            if is_address(*word) {
                format!("ldr {}, =addr", decode::reg(*rd))
            } else {
                format!("ldr {}, ={word:#x}", decode::reg(*rd))
            }
        }
        Kind::AddPc { rd, .. } => format!("add {}, pc, addr", decode::reg(*rd)),
        kind => decode::text_of(kind),
    }
}

/// Values in the GBA's memory map from EWRAM to the end of cartridge space.
pub fn is_address(value: u32) -> bool {
    (0x0200_0000..0x0e00_0000).contains(&value)
}

/// Token ids shared by every function of one run.
#[derive(Default)]
pub struct Interner {
    ids: HashMap<String, u32>,
}

impl Interner {
    pub fn id(&mut self, token: String) -> u32 {
        let next = self.ids.len() as u32;
        *self.ids.entry(token).or_insert(next)
    }
}

/// Levenshtein distance over token sequences, or `None` once it must exceed
/// `limit` (only a band of width 2 * limit + 1 is computed).
pub fn distance(a: &[u32], b: &[u32], limit: usize) -> Option<usize> {
    let (a, b) = if a.len() <= b.len() { (a, b) } else { (b, a) };
    if b.len() - a.len() > limit {
        return None;
    }
    let big = limit + 1;
    let mut previous: Vec<usize> = (0..=b.len()).map(|j| j.min(big)).collect();
    let mut current = vec![big; b.len() + 1];
    for i in 1..=a.len() {
        let low = i.saturating_sub(limit).max(1);
        let high = (i + limit).min(b.len());
        current[0] = i.min(big);
        if low > 1 {
            current[low - 1] = big;
        }
        let mut best = current[0];
        for j in low..=high {
            let substitute = previous[j - 1] + usize::from(a[i - 1] != b[j - 1]);
            let value = substitute
                .min(previous[j] + 1)
                .min(current[j - 1] + 1)
                .min(big);
            current[j] = value;
            best = best.min(value);
        }
        if high < b.len() {
            current[high + 1] = big;
        }
        if best > limit {
            return None;
        }
        std::mem::swap(&mut previous, &mut current);
    }
    (previous[b.len()] <= limit).then_some(previous[b.len()])
}

/// A lower bound on the edit distance from the token multisets alone.
fn multiset_bound(a: &[u32], b: &[u32]) -> usize {
    let (mut i, mut j, mut only_a, mut only_b) = (0, 0, 0, 0);
    while i < a.len() && j < b.len() {
        match a[i].cmp(&b[j]) {
            std::cmp::Ordering::Equal => {
                i += 1;
                j += 1;
            }
            std::cmp::Ordering::Less => {
                only_a += 1;
                i += 1;
            }
            std::cmp::Ordering::Greater => {
                only_b += 1;
                j += 1;
            }
        }
    }
    only_a += a.len() - i;
    only_b += b.len() - j;
    only_a.max(only_b)
}

pub struct Match {
    pub index: usize,
    pub distance: usize,
}

/// A distance relative to the longer sequence: 0 is identical, 1 unrelated.
pub fn ratio(distance: usize, a: usize, b: usize) -> f64 {
    distance as f64 / a.max(b).max(1) as f64
}

/// The `top` nearest candidates to `query` whose relative distance is at
/// most `max_ratio`, nearest first.
pub fn nearest(
    query: &Function,
    corpus: &[Function],
    sorted: &[Vec<u32>],
    candidates: &[usize],
    top: usize,
    max_ratio: f64,
) -> Vec<Match> {
    let mut sorted_query = query.tokens.clone();
    sorted_query.sort_unstable();
    let mut found: Vec<Match> = Vec::new();
    for &index in candidates {
        let candidate = &corpus[index];
        if candidate.build == query.build
            && candidate.image == query.image
            && candidate.address == query.address
        {
            continue;
        }
        let longest = query.tokens.len().max(candidate.tokens.len());
        let mut limit = (max_ratio * longest as f64).floor() as usize;
        if found.len() >= top {
            limit = limit.min(found[top - 1].distance);
        }
        if query.tokens.len().abs_diff(candidate.tokens.len()) > limit
            || multiset_bound(&sorted_query, &sorted[index]) > limit
        {
            continue;
        }
        if let Some(distance) = distance(&query.tokens, &candidate.tokens, limit) {
            let at = found.partition_point(|m| m.distance <= distance);
            found.insert(at, Match { index, distance });
            found.truncate(top);
        }
    }
    found
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
pub fn map_text_sections(map: &str) -> Vec<(u32, u32, String)> {
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
pub fn sweep(image: &[u8], base: u32, start: u32, end: u32) -> Vec<Ins> {
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

/// Splits every placed text section of `image` into functions. `classify`
/// turns an object path into the origin and source path to report; only
/// sections whose origin is `wanted` are decoded.
pub fn functions(
    image: &Image,
    interner: &mut Interner,
    classify: &dyn Fn(&str) -> (Origin, String),
    wanted: &dyn Fn(Origin) -> bool,
) -> Vec<Function> {
    decoded(image, classify, wanted)
        .into_iter()
        .map(|(mut f, ins)| {
            f.tokens = ins.iter().map(|i| interner.id(normalise(i))).collect();
            f
        })
        .collect()
}

/// As `functions`, with each function's decoded instructions; its `tokens`
/// are left empty.
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
                    tokens: Vec::new(),
                },
                body.to_vec(),
            ));
        }
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn ins(kind: Kind) -> Ins {
        Ins {
            addr: 0,
            size: 2,
            text: decode::text_of(&kind),
            kind,
        }
    }

    #[test]
    fn normalising_masks_addresses_but_keeps_registers_and_constants() {
        assert_eq!(
            normalise(&ins(Kind::Bl {
                target: 0x0800_1234
            })),
            "bl"
        );
        assert_eq!(
            normalise(&ins(Kind::B {
                target: 0x0800_1234
            })),
            "b"
        );
        assert_eq!(
            normalise(&ins(Kind::LdrPool {
                rd: 3,
                word: 0x0300_1ee0
            })),
            "ldr r3, =addr"
        );
        assert_eq!(
            normalise(&ins(Kind::LdrPool { rd: 3, word: 0x1ff })),
            "ldr r3, =0x1ff"
        );
        assert_eq!(
            normalise(&ins(Kind::MovImm { rd: 2, imm: 36 })),
            decode::text_of(&Kind::MovImm { rd: 2, imm: 36 })
        );
        assert_ne!(
            normalise(&ins(Kind::MovImm { rd: 2, imm: 36 })),
            normalise(&ins(Kind::MovImm { rd: 1, imm: 36 }))
        );
    }

    #[test]
    fn banded_distance_matches_the_full_distance_within_its_limit() {
        let a = [1, 2, 3, 4, 5, 6];
        let b = [1, 3, 4, 9, 5, 6, 7];
        assert_eq!(distance(&a, &b, 10), Some(3));
        assert_eq!(distance(&a, &b, 3), Some(3));
        assert_eq!(distance(&a, &b, 2), None);
        assert_eq!(distance(&a, &a, 0), Some(0));
        assert_eq!(distance(&[], &b, 7), Some(7));
        let mut sorted_a = a.to_vec();
        let mut sorted_b = b.to_vec();
        sorted_a.sort();
        sorted_b.sort();
        assert!(multiset_bound(&sorted_a, &sorted_b) <= 3);
    }

    #[test]
    fn nearest_ranks_the_closest_function_first_and_skips_itself() {
        let function = |address, tokens: &[u32]| Function {
            build: "test".into(),
            image: "test".into(),
            name: format!("f{address}"),
            address,
            bytes: tokens.len() as u32 * 2,
            origin: Origin::C,
            source: String::new(),
            tokens: tokens.to_vec(),
        };
        let corpus = vec![
            function(0, &[1, 2, 3, 4, 5, 6, 7, 8]),
            function(1, &[1, 2, 3, 4, 5, 6, 7, 9]),
            function(2, &[1, 2, 3, 0, 0, 6, 7, 8]),
            function(3, &[9, 9, 9, 9]),
        ];
        let sorted: Vec<Vec<u32>> = corpus
            .iter()
            .map(|f| {
                let mut t = f.tokens.clone();
                t.sort();
                t
            })
            .collect();
        let all: Vec<usize> = (0..corpus.len()).collect();
        let found = nearest(&corpus[0], &corpus, &sorted, &all, 5, 0.5);
        let order: Vec<(usize, usize)> = found.iter().map(|m| (m.index, m.distance)).collect();
        assert_eq!(order, vec![(1, 1), (2, 2)]);
    }

    #[test]
    fn a_twin_at_the_same_address_in_another_overlay_is_a_match() {
        let overlay = |image: &str| Function {
            build: "tla-en".into(),
            image: image.into(),
            name: "Func_02008000".into(),
            address: 0x0200_8000,
            bytes: 16,
            origin: Origin::NotYetC,
            source: String::new(),
            tokens: vec![1, 2, 3, 4, 5, 6, 7, 8],
        };
        let corpus = vec![overlay("resource_649"), overlay("resource_64a")];
        let sorted: Vec<Vec<u32>> = corpus.iter().map(|f| f.tokens.clone()).collect();
        let found = nearest(&corpus[0], &corpus, &sorted, &[0, 1], 5, 0.35);
        let order: Vec<(usize, usize)> = found.iter().map(|m| (m.index, m.distance)).collect();
        assert_eq!(order, vec![(1, 0)]);
    }

    #[test]
    fn map_sections_read_both_line_layouts() {
        let map = " .text          0x08000100       0x20 /a/obj/games/X/SRC/A.o\n\
                   \x20                0x08000100                A\n\
                   \x20.text.x02009ca4\n\
                   \x20               0x02009ca4      0x5d0 /a/overlays/resource_371_overlay.o\n\
                   \x20.text          0x08000120        0x0 /a/obj/empty.o\n";
        let placed = map_text_sections(map);
        assert_eq!(
            placed,
            vec![
                (0x0800_0100, 0x20, "/a/obj/games/X/SRC/A.o".to_string()),
                (
                    0x0200_9ca4,
                    0x5d0,
                    "/a/overlays/resource_371_overlay.o".to_string()
                ),
            ]
        );
    }
}
