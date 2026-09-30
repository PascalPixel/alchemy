//! Thumb flow: where each function of a listing begins and ends, which
//! words its loads read, which switch tables it jumps through, and what it
//! calls and branches to.

use crate::decode::{decode_one, Ins, Kind};
use std::collections::{BTreeMap, BTreeSet};

/// A bounded view of the image by address.
#[derive(Clone, Copy)]
pub struct Image<'a> {
    pub bytes: &'a [u8],
    pub base: u32,
}

impl Image<'_> {
    pub fn contains(&self, address: u32, size: u32) -> bool {
        address >= self.base
            && u64::from(address - self.base) + u64::from(size) <= self.bytes.len() as u64
    }
    pub fn half(&self, address: u32) -> u16 {
        let at = (address - self.base) as usize;
        u16::from_le_bytes([self.bytes[at], self.bytes[at + 1]])
    }
    pub fn word(&self, address: u32) -> u32 {
        let at = (address - self.base) as usize;
        u32::from_le_bytes(self.bytes[at..at + 4].try_into().unwrap())
    }
    pub fn decode(&self, address: u32) -> Option<Ins> {
        decode_one(self.bytes, self.base, address)
    }
}

/// The word a `ldr rd, [pc, #k]` at `pc` reads.
pub fn pool_address(pc: u32, half: u16) -> u32 {
    ((pc + 4) & !3) + u32::from(half & 0xff) * 4
}

/// Known function entries, and the targets of GCC's far jumps: a `bl` that
/// a large function uses as an unconditional branch to its own code.
#[derive(Clone, Debug, Default)]
pub struct Entries {
    pub starts: BTreeSet<u32>,
    pub far: BTreeSet<u32>,
}

/// The flow of one function, from its entry to its last byte.
#[derive(Clone, Debug, Default)]
pub struct Function {
    pub start: u32,
    /// One past its last instruction, pool word, table word or padding.
    pub end: u32,
    pub instructions: BTreeMap<u32, Ins>,
    /// Word addresses its PC-relative loads read, inside or outside it.
    pub pools: BTreeSet<u32>,
    /// Jump-table words and the case each one names.
    pub tables: BTreeMap<u32, u32>,
    /// Branch targets inside the function, far jumps' included.
    pub labels: BTreeSet<u32>,
    /// `bl` targets that are calls.
    pub calls: BTreeSet<u32>,
    /// `b` and `b<cond>` targets outside the function.
    pub exits: BTreeSet<u32>,
    /// Every path ends in a return, a jump or a tail branch.
    pub terminated: bool,
    /// Some path returns with `bx` or `pop {pc}`.
    pub returns: bool,
    /// The entry its flow runs into, when it does not end on its own.
    pub falls_into: Option<u32>,
}

impl Function {
    /// Whether `address` is covered by one of its instructions.
    pub fn covers(&self, address: u32) -> bool {
        self.instructions
            .range(..=address)
            .next_back()
            .is_some_and(|(at, ins)| address < at + ins.size)
    }
}

/// Follow the flow of the function at `start`, which must stay inside
/// `[start, limit)`. Flow that reaches a known entry ends there, and a branch
/// to one is a tail branch. `None` when the bytes are not a function: an
/// undefined instruction, flow that runs into a word its own loads read, or
/// flow off the end of `limit`.
pub fn walk(image: Image, start: u32, limit: u32, entries: &Entries) -> Option<Function> {
    trace(image, start, limit, entries).ok()
}

/// Why bytes are not a function: where the flow broke, and how.
pub type Reject = (u32, &'static str);

/// As [`walk`], saying where and why the flow is not a function.
pub fn trace(image: Image, start: u32, limit: u32, entries: &Entries) -> Result<Function, Reject> {
    let mut barriers = BTreeSet::new();
    for _ in 0..8 {
        let function = walk_once(image, start, limit, entries, &barriers)?;
        let read: BTreeSet<u32> = function
            .pools
            .iter()
            .chain(function.tables.keys())
            .flat_map(|&word| [word, word + 2])
            .collect();
        if read.iter().all(|&address| !function.covers(address)) {
            return Ok(function);
        }
        let before = barriers.len();
        barriers.extend(read);
        if barriers.len() == before {
            return Err((start, "its flow runs through words its loads read"));
        }
    }
    Err((start, "its flow keeps running through words its loads read"))
}

fn walk_once(
    image: Image,
    start: u32,
    limit: u32,
    entries: &Entries,
    barriers: &BTreeSet<u32>,
) -> Result<Function, Reject> {
    let inside = |address: u32| start <= address && address < limit;
    let local =
        |address: u32| inside(address) && !(address != start && entries.starts.contains(&address));
    let mut function = Function {
        start,
        terminated: true,
        ..Function::default()
    };
    let mut queue = vec![start];
    while let Some(mut pc) = queue.pop() {
        let mut after_call = false;
        loop {
            if function.instructions.contains_key(&pc) {
                break;
            }
            if pc != start && entries.starts.contains(&pc) {
                function.terminated = false;
                function.falls_into = Some(pc);
                break;
            }
            if !inside(pc) || !image.contains(pc, 2) {
                return Err((pc, "flow leaves the listing"));
            }
            // A call that alignment padding or its own words follow does not
            // return: a far jump, or a call that never comes back.
            if after_call && pc % 4 == 2 && image.contains(pc, 2) && image.half(pc) == 0 {
                break;
            }
            if barriers.contains(&pc) {
                if after_call {
                    break;
                }
                return Err((pc, "flow runs into a word its loads read"));
            }
            let ins = image.decode(pc).ok_or((pc, "undecodable"))?;
            if pc + ins.size > limit {
                return Err((pc, "an instruction crosses the end of the listing"));
            }
            let mut next = true;
            match ins.kind {
                Kind::Unknown(half) => {
                    // swi, and the lone second half of a bl that calls
                    // through lr, continue; anything else is not code.
                    if half & 0xff00 != 0xdf00 && half & 0xf800 != 0xf800 {
                        return Err((pc, "undefined instruction"));
                    }
                }
                Kind::LdrPool { .. } => {
                    let word = pool_address(pc, image.half(pc));
                    if !image.contains(word, 4) {
                        return Err((pc, "a load reads outside the image"));
                    }
                    function.pools.insert(word);
                }
                Kind::B { target } => {
                    if local(target) {
                        function.labels.insert(target);
                        queue.push(target);
                    } else {
                        function.exits.insert(target);
                    }
                    next = false;
                }
                Kind::Bcond { target, .. } => {
                    if local(target) {
                        function.labels.insert(target);
                        queue.push(target);
                    } else {
                        function.exits.insert(target);
                    }
                }
                Kind::Bl { target } => {
                    // GCC begins every function on a word boundary, so a call
                    // off one inside the listing is a far jump.
                    let far = entries.far.contains(&target) || target % 4 != 0;
                    if far && local(target) {
                        function.labels.insert(target);
                        queue.push(target);
                        next = false;
                    } else {
                        function.calls.insert(target);
                    }
                }
                Kind::Bx(_) | Kind::Pop { pc: true, .. } => {
                    function.returns = true;
                    next = false;
                }
                Kind::MovHi { rd: 15, .. } => {
                    for (word, case) in jump_table(image, &function, pc, start, limit) {
                        function.tables.insert(word, case);
                        function.labels.insert(case);
                        queue.push(case);
                    }
                    next = false;
                }
                Kind::AddHi { rd: 15, .. } => next = false,
                _ => {}
            }
            let size = ins.size;
            after_call = next && matches!(ins.kind, Kind::Bl { .. });
            function.instructions.insert(pc, ins);
            if !next {
                break;
            }
            pc += size;
        }
    }
    let last = function
        .instructions
        .iter()
        .map(|(at, ins)| at + ins.size)
        .max()
        .ok_or((start, "no instruction"))?;
    let own = |word: &u32| inside(*word) && *word + 4 <= limit;
    function.end = function
        .pools
        .iter()
        .chain(function.tables.keys())
        .filter(|word| own(word))
        .map(|word| word + 4)
        .fold(last, u32::max);
    Ok(function)
}

/// The cases of a switch that ends in `mov pc, rN` at `pc`, as GCC emits
/// it: the table's address comes from a pool word loaded just before, the
/// table lies after the jump, and a `cmp rN, #n` before it bounds the index.
fn jump_table(
    image: Image,
    function: &Function,
    pc: u32,
    start: u32,
    limit: u32,
) -> Vec<(u32, u32)> {
    let before: Vec<&Ins> = function
        .instructions
        .range(..pc)
        .rev()
        .take(16)
        .map(|(_, ins)| ins)
        .collect();
    let Some(table) = before.iter().take(8).find_map(|ins| match ins.kind {
        Kind::LdrPool { word, .. } if word % 4 == 0 && word > pc && word < limit => Some(word),
        _ => None,
    }) else {
        return Vec::new();
    };
    let bound = before.iter().find_map(|ins| match ins.kind {
        Kind::CmpImm { imm, .. } => Some(imm + 1),
        _ => None,
    });
    let mut cases = Vec::new();
    let mut first_case = limit;
    let mut word = table;
    while word + 4 <= first_case && word + 4 <= limit && cases.len() < 1024 {
        if bound.is_some_and(|n| cases.len() as u32 >= n) {
            break;
        }
        let case = image.word(word);
        if case % 2 != 0 || case < start || case >= limit || case <= pc {
            break;
        }
        first_case = first_case.min(case);
        cases.push((word, case));
        word += 4;
    }
    cases
}

/// One stretch of an area: a function, or bytes no flow reaches, which
/// keep the form they came in (listing rows or included data).
#[derive(Clone, Debug)]
pub enum Segment {
    Function(Function),
    Data { start: u32, end: u32, listing: bool },
}

impl Segment {
    pub fn start(&self) -> u32 {
        match self {
            Segment::Function(function) => function.start,
            Segment::Data { start, .. } => *start,
        }
    }
    pub fn end(&self) -> u32 {
        match self {
            Segment::Function(function) => function.end,
            Segment::Data { end, .. } => *end,
        }
    }
}

/// A contiguous stretch of listings and included data, in placement order:
/// each part's end and whether it is a listing.
#[derive(Clone, Debug)]
pub struct Area {
    pub start: u32,
    pub parts: Vec<(u32, bool)>,
}

impl Area {
    pub fn end(&self) -> u32 {
        self.parts.last().map_or(self.start, |part| part.0)
    }
    /// Where its first function begins, when it opens with a listing: its
    /// first byte, or after the zero halfword that aligns a listing that
    /// opens off a word boundary.
    pub fn opening(&self, image: Image) -> Option<u32> {
        self.parts.first().filter(|part| part.1)?;
        let padded = self.start % 4 == 2 && image.half(self.start) == 0;
        Some(self.start + if padded { 2 } else { 0 })
    }
    /// The part holding `address`: its start, end and whether it is a listing.
    pub fn part(&self, address: u32) -> (u32, u32, bool) {
        let mut from = self.start;
        for &(end, listing) in &self.parts {
            if address < end {
                return (from, end, listing);
            }
            from = end;
        }
        (self.end(), self.end(), false)
    }
}

/// Split an area into functions and data. Each known entry begins a
/// function, which may run on through later parts; after each function,
/// zero halfwords up to the next word boundary are its alignment padding.
/// In a listing, whatever follows begins another function when its flow is
/// complete and returns, or data up to the next entry or part; included data
/// begins a function only at a known entry.
pub fn partition(image: Image, area: &Area, entries: &Entries) -> Vec<Segment> {
    let end = area.end();
    let opening = area.opening(image);
    let mut segments: Vec<Segment> = Vec::new();
    let mut cursor = area.start;
    while cursor < end {
        let (_, part_end, listing) = area.part(cursor);
        // A zero halfword off a word boundary aligns what follows; GCC
        // begins every function on a word boundary.
        if listing && cursor % 4 == 2 && cursor + 2 <= part_end && image.half(cursor) == 0 {
            segments.push(Segment::Data {
                start: cursor,
                end: cursor + 2,
                listing,
            });
            cursor += 2;
            continue;
        }
        let known = Some(cursor) == opening || entries.starts.contains(&cursor);
        let next = entries
            .starts
            .range(cursor + 1..end)
            .next()
            .copied()
            .unwrap_or(end);
        let function = (cursor % 2 == 0)
            .then(|| walk(image, cursor, end, entries))
            .flatten()
            .filter(|function| {
                function.end <= next
                    && if listing {
                        known || unnamed_function(image, function)
                    } else {
                        known && unnamed_function(image, function)
                    }
            });
        if let Some(function) = function {
            cursor = function.end;
            segments.push(Segment::Function(function));
            // Alignment padding belongs to the function before it.
            while cursor % 4 == 2 && cursor < end && image.half(cursor) == 0 {
                cursor += 2;
                if let Some(Segment::Function(last)) = segments.last_mut() {
                    last.end = cursor;
                }
            }
            continue;
        }
        // Unreached bytes run to the next entry or part, or in a listing to
        // the next word boundary where a function can begin.
        let limit = next.min(part_end);
        let mut stop = (cursor + 2).min(limit);
        if listing && cursor % 2 == 0 {
            while stop < limit
                && !(stop % 4 == 0
                    && walk(image, stop, end, entries).is_some_and(|function| {
                        function.end <= next && unnamed_function(image, &function)
                    }))
            {
                stop += 2;
            }
        } else {
            stop = limit;
        }
        match segments.last_mut() {
            Some(Segment::Data {
                end: last,
                listing: same,
                ..
            }) if *last == cursor
                && *same == listing
                && area.part(cursor - 1).0 == area.part(cursor).0 =>
            {
                *last = stop
            }
            _ => segments.push(Segment::Data {
                start: cursor,
                end: stop,
                listing,
            }),
        }
        cursor = stop;
    }
    segments
}

/// Whether flow nobody names reads as a function: it opens with neither
/// zero padding nor the second half of a `bl`, runs more than one
/// instruction, and every path ends, some in a return.
pub fn unnamed_function(image: Image, function: &Function) -> bool {
    let half = image.half(function.start);
    half != 0
        && half & 0xf800 != 0xf800
        && function.instructions.len() > 1
        && function.terminated
        && function.returns
}

#[cfg(test)]
pub(crate) mod tests {
    use super::*;

    pub(crate) fn halves(values: &[u16]) -> Vec<u8> {
        values.iter().flat_map(|half| half.to_le_bytes()).collect()
    }

    fn listing(start: u32, end: u32) -> Area {
        Area {
            start,
            parts: vec![(end, true)],
        }
    }

    #[test]
    fn functions_split_at_their_returns_pools_and_padding() {
        let base = 0x0800_0000;
        let bytes = halves(&[
            0xb500, // push {lr}
            0x4802, // ldr r0, [pc, #8]: the word at 0x0c
            0xf000, 0xf804, // bl 0x08000010
            0xbd00, // pop {pc}
            0x0000, // never reached
            0x0011, 0x0800, // pool: 0x08000011
            0x2001, // movs r0, #1
            0x4770, // bx lr
        ]);
        let image = Image {
            bytes: &bytes,
            base,
        };
        let area = listing(base, base + bytes.len() as u32);
        let segments = partition(image, &area, &Entries::default());
        let bounds: Vec<(u32, u32)> = segments.iter().map(|s| (s.start(), s.end())).collect();
        assert_eq!(
            bounds,
            vec![(base, base + 0x10), (base + 0x10, base + 0x14)]
        );
        let Segment::Function(first) = &segments[0] else {
            panic!("a function");
        };
        assert_eq!(first.pools, BTreeSet::from([base + 0xc]));
        assert_eq!(first.calls, BTreeSet::from([base + 0x10]));
    }

    #[test]
    fn a_switch_table_after_mov_pc_is_read_as_its_cases() {
        let base = 0x0800_0000;
        let mut bytes = halves(&[
            0x2801, // cmp r0, #1
            0xd80f, // bhi 0x08000024
            0x0080, // lsls r0, r0, #2
            0x4902, // ldr r1, [pc, #8]: the word at 0x10
            0x1840, // adds r0, r0, r1
            0x6800, // ldr r0, [r0]
            0x4687, // mov pc, r0
            0x0000, // padding
        ]);
        bytes.extend(0x0800_0014u32.to_le_bytes()); // 0x10: the table's address
        bytes.extend(0x0800_001cu32.to_le_bytes()); // 0x14: case 0
        bytes.extend(0x0800_0020u32.to_le_bytes()); // 0x18: case 1
        bytes.extend(halves(&[0x2001, 0x4770, 0x2002, 0x4770, 0x2000, 0x4770]));
        let image = Image {
            bytes: &bytes,
            base,
        };
        let function = walk(image, base, base + bytes.len() as u32, &Entries::default()).unwrap();
        assert_eq!(
            function.tables,
            BTreeMap::from([(base + 0x14, base + 0x1c), (base + 0x18, base + 0x20)])
        );
        assert_eq!(function.pools, BTreeSet::from([base + 0x10]));
        assert_eq!(function.end, base + 0x28);
        assert!(function.terminated);
    }

    #[test]
    fn a_far_jump_is_a_branch_that_ends_its_block() {
        let base = 0x0800_0000;
        let bytes = halves(&[
            0xb500, // push {lr}
            0xf000, 0xf801, // bl 0x08000008: a far jump
            0x0000, // never reached
            0xbd00, // 0x08: pop {pc}
        ]);
        let image = Image {
            bytes: &bytes,
            base,
        };
        let limit = base + bytes.len() as u32;
        let call = walk(image, base, limit, &Entries::default()).unwrap();
        assert_eq!(call.calls, BTreeSet::from([base + 8]));
        let entries = Entries {
            starts: BTreeSet::new(),
            far: BTreeSet::from([base + 8]),
        };
        let jump = walk(image, base, limit, &entries).unwrap();
        assert!(jump.calls.is_empty());
        assert!(jump.labels.contains(&(base + 8)));
        assert!(!jump.instructions.contains_key(&(base + 6)));
        assert_eq!(jump.end, base + 0xa);
    }

    #[test]
    fn included_data_begins_a_function_only_at_an_entry() {
        let base = 0x0800_0000;
        // A listing function that runs on into included data, then data that
        // reads as code but that nothing names.
        let bytes = halves(&[0x2001, 0x2002, 0x4770, 0x0000, 0x2003, 0x4770]);
        let image = Image {
            bytes: &bytes,
            base,
        };
        let area = Area {
            start: base,
            parts: vec![(base + 2, true), (base + 12, false)],
        };
        let segments = partition(image, &area, &Entries::default());
        assert!(matches!(&segments[0], Segment::Function(f) if f.end == base + 8));
        assert!(matches!(
            segments[1],
            Segment::Data { start, end, listing: false } if start == base + 8 && end == base + 12
        ));
        let entries = Entries {
            starts: BTreeSet::from([base + 8]),
            far: BTreeSet::new(),
        };
        let segments = partition(image, &area, &entries);
        assert!(matches!(&segments[1], Segment::Function(f) if f.start == base + 8));
    }

    #[test]
    fn a_call_off_a_word_boundary_is_a_far_jump_and_padding_ends_a_call() {
        let base = 0x0800_0000;
        let mut bytes = halves(&[
            0xb500, // push {lr}
            0x4802, // ldr r0, [pc, #8]: the word at 0x0c
            0xf000, 0xf805, // bl 0x08000012, off a word boundary: a far jump
            0xf000, 0xf804, // 0x08: bl 0x08000014, never reached
        ]);
        bytes.extend(0x1234_5678u32.to_le_bytes()); // 0x0c: pool
        bytes.extend(halves(&[
            0x0000, // 0x10: padding
            0xf7ff, 0xfff5, // 0x12: bl 0x08000000, a call that padding follows
            0x0000, // 0x16: padding
        ]));
        let image = Image {
            bytes: &bytes,
            base,
        };
        let function = walk(image, base, base + bytes.len() as u32, &Entries::default()).unwrap();
        assert!(function.labels.contains(&(base + 0x12)));
        assert!(!function.instructions.contains_key(&(base + 8)));
        assert!(!function.instructions.contains_key(&(base + 0x16)));
        assert_eq!(function.calls, BTreeSet::from([base]));
    }
}
