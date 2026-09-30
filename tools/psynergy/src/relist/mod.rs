//! Pret-shape disassembly. Regenerates the not-yet-C listings of a linked
//! Thumb image so that every function starts in its own labelled piece at
//! its real start, and every branch target and every word pointing into ROM
//! or RAM is written as a symbol, never a number.
//!
//! The caller supplies the image's bytes, its symbols, what the link map
//! placed where, the names other objects refer to, and the scaffolds that
//! hold included data and reserved RAM. Listings and included data that sit
//! side by side form an area; each area is partitioned into functions and
//! data again from the image's own flow, so a function is found wherever it
//! really begins, even when an earlier listing cut it in two or left it in
//! included data. Rendering restates the image's own bytes piece by piece in
//! the same places, so the rebuilt image stays byte-identical.
//!
//! Names that exist are kept where anything outside the listings refers to
//! them, or where they are more than a placeholder. Every other referenced
//! address gets a placeholder: `Func_` for code, `Data_` for anything else,
//! defined as a label where its bytes are: in a listing, or by splitting an
//! `.incbin` or `.space` of the scaffold that holds the address.

pub mod flow;
pub mod render;
pub mod scaffold;

pub use flow::{partition, trace, walk, Area, Entries, Function, Image, Segment};

use crate::decode::Kind;
use scaffold::{Layout, Scaffold};
use std::collections::{BTreeMap, BTreeSet};

/// One input section the link map places.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Placed {
    pub section: String,
    pub address: u32,
    pub size: u32,
    pub object: String,
}

fn hex(field: &str) -> Option<u64> {
    u64::from_str_radix(field.strip_prefix("0x")?, 16).ok()
}

/// Every input section with bytes that the map places, in map order. Only
/// the memory map proper is read; discarded sections come before it.
pub fn placed_sections(map: &str) -> Vec<Placed> {
    let body = map
        .find("Linker script and memory map")
        .map_or(map, |at| &map[at..]);
    let mut placed = Vec::new();
    let mut pending: Option<&str> = None;
    let mut push = |section: &str, fields: &[&str]| {
        let (Some(address), Some(size)) = (
            fields.first().and_then(|f| hex(f)),
            fields.get(1).and_then(|f| hex(f)),
        ) else {
            return;
        };
        let object = fields
            .get(2..)
            .map(|rest| rest.join(" "))
            .unwrap_or_default();
        if size > 0 && !object.is_empty() && address <= u64::from(u32::MAX) {
            placed.push(Placed {
                section: section.to_string(),
                address: address as u32,
                size: size as u32,
                object,
            });
        }
    };
    for line in body.lines() {
        if let Some(section) = pending.take() {
            let fields: Vec<&str> = line.split_whitespace().collect();
            if fields.first().is_some_and(|f| f.starts_with("0x")) {
                push(section, &fields);
                continue;
            }
        }
        let Some(rest) = line.strip_prefix(' ') else {
            continue;
        };
        if !rest.starts_with('.') {
            continue;
        }
        let fields: Vec<&str> = rest.split_whitespace().collect();
        match fields.len() {
            1 => pending = Some(fields[0]),
            n if n >= 4 => push(fields[0], &fields[1..]),
            _ => {}
        }
    }
    placed
}

/// A defined name from the image's symbol table.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Name {
    pub name: String,
    pub address: u32,
    /// A Thumb function: a word naming it gets the Thumb bit from the linker.
    pub thumb: bool,
    /// Defined by the linker script with a value, not by bytes in a section:
    /// it can be referred to, but no listing or scaffold defines it.
    pub absolute: bool,
}

/// A placeholder: `Func_`, `Data_` or `sub_` and the address it spells.
pub fn placeholder(name: &str) -> bool {
    ["Func_", "Data_", "sub_"]
        .iter()
        .find_map(|prefix| name.strip_prefix(prefix))
        .is_some_and(|rest| rest.len() == 8 && rest.bytes().all(|b| b.is_ascii_hexdigit()))
}

/// What occupies a placed stretch of the image, as far as labels go.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum RegionKind {
    /// A listing: regenerated, or kept as written (hand-annotated pieces).
    Listing { regenerate: bool },
    /// Included data: a section of the incbin scaffold with this index.
    Incbin { scaffold: usize },
    /// Reserved RAM: a section of the space scaffold with this index.
    Space { scaffold: usize },
    /// Maintained code: its calls and pointers are read, it takes no labels.
    Code,
    /// Anything else placed: it takes no labels.
    Other,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Region {
    pub start: u32,
    pub end: u32,
    pub kind: RegionKind,
    /// The object's source stem the map names (a listing's file stem).
    pub object: String,
    /// The input section's name.
    pub section: String,
}

pub fn region_at(regions: &[Region], address: u32) -> Option<usize> {
    let index = regions.partition_point(|region| region.start <= address);
    index
        .checked_sub(1)
        .filter(|&index| address < regions[index].end)
}

/// Whether a word points into cartridge ROM (below `rom_end`) or either RAM.
pub fn addressable(value: u32, rom_end: u32) -> bool {
    (0x0200_0000..0x0204_0000).contains(&value)
        || (0x0300_0000..0x0300_8000).contains(&value)
        || (0x0800_0000..rom_end).contains(&value)
}

fn relisted(kind: &RegionKind) -> bool {
    matches!(
        kind,
        RegionKind::Listing { regenerate: true } | RegionKind::Incbin { .. }
    )
}

/// The areas of `regions`: each maximal run of side-by-side regenerated
/// listings and included data, with the indices of its regions.
pub fn areas(regions: &[Region]) -> Vec<(Area, Vec<usize>)> {
    let mut areas: Vec<(Area, Vec<usize>)> = Vec::new();
    for (index, region) in regions.iter().enumerate() {
        if !relisted(&region.kind) {
            continue;
        }
        let listing = region.kind == RegionKind::Listing { regenerate: true };
        match areas.last_mut() {
            Some((area, members))
                if area.end() == region.start && members.last() == Some(&(index - 1)) =>
            {
                area.parts.push((region.end, listing));
                members.push(index);
            }
            _ => areas.push((
                Area {
                    start: region.start,
                    parts: vec![(region.end, listing)],
                },
                vec![index],
            )),
        }
    }
    areas
}

/// One area, partitioned.
#[derive(Clone, Debug)]
pub struct Planned {
    pub area: Area,
    pub regions: Vec<usize>,
    pub segments: Vec<Segment>,
}

/// Every area partitioned, with the entries that decided them.
pub struct Plan {
    pub areas: Vec<Planned>,
    pub entries: Entries,
    /// Call and pointer targets that land inside a function, not at its entry.
    pub inner: BTreeSet<u32>,
    /// The entries no flow can refuse.
    pub firm: BTreeSet<u32>,
}

impl Plan {
    pub fn functions(&self) -> impl Iterator<Item = &Function> {
        self.areas
            .iter()
            .flat_map(|planned| &planned.segments)
            .filter_map(|segment| match segment {
                Segment::Function(function) => Some(function),
                Segment::Data { .. } => None,
            })
    }
}

/// Partition every area. Firm entries are the first byte of each area that
/// opens with a listing, the listings' named Thumb functions, and the calls
/// (and, into listings, Thumb pointers) of maintained code and kept
/// listings. A placeholder function name in a listing is an entry until
/// flow runs into it. The listings' own calls and pointers then add entries
/// until none appears, except that a call into the caller's own extent, or
/// to a halfword that is not word-aligned in the caller's area, is a far
/// jump.
pub fn plan(image: Image, regions: &[Region], names: &[Name]) -> Result<Plan, String> {
    let runs = areas(regions);
    let area_of = |address: u32| -> Option<(usize, bool)> {
        let index = runs.partition_point(|(area, _)| area.start <= address);
        let index = index.checked_sub(1)?;
        let area = &runs[index].0;
        (address < area.end()).then(|| (index, area.part(address).2))
    };
    let in_listing = |address: u32| area_of(address).is_some_and(|(_, listing)| listing);
    let mut firm: BTreeSet<u32> = runs
        .iter()
        .filter(|(area, _)| area.parts.first().is_some_and(|part| part.1))
        .map(|(area, _)| area.start)
        .collect();
    let mut weak = BTreeSet::new();
    for name in names
        .iter()
        .filter(|name| name.thumb && in_listing(name.address))
    {
        if placeholder(&name.name) {
            weak.insert(name.address);
        } else {
            firm.insert(name.address);
        }
    }
    for region in regions {
        let kept = region.kind == RegionKind::Code
            || region.kind == RegionKind::Listing { regenerate: false };
        if !kept || !image.contains(region.start, region.end - region.start) {
            continue;
        }
        for ins in crate::similar::sweep(image.bytes, image.base, region.start, region.end) {
            match ins.kind {
                Kind::Bl { target } if area_of(target).is_some() => {
                    firm.insert(target);
                }
                Kind::LdrPool { word, .. } if word & 1 == 1 && in_listing(word & !1) => {
                    firm.insert(word & !1);
                }
                _ => {}
            }
        }
    }
    let mut entries = Entries {
        starts: firm.union(&weak).copied().collect(),
        far: BTreeSet::new(),
    };
    let mut refused = BTreeSet::new();
    for _ in 0..64 {
        // Every entry's own flow, whether or not a partition keeps it.
        let walks: BTreeMap<u32, (usize, Function)> = entries
            .starts
            .iter()
            .filter_map(|&start| {
                let (run, _) = area_of(start)?;
                let function = walk(image, start, runs[run].0.end(), &entries)?;
                Some((start, (run, function)))
            })
            .collect();
        let inside_function = |address: u32| {
            walks
                .range(..address)
                .next_back()
                .is_some_and(|(_, (_, function))| address < function.end)
        };
        let mut changed = false;
        let mut inner = BTreeSet::new();
        for (run, function) in walks.values() {
            for &target in &function.calls {
                let Some((target_run, listing)) = area_of(target) else {
                    continue;
                };
                if firm.contains(&target) || entries.far.contains(&target) {
                    continue;
                }
                let own = function.start < target && target < function.end;
                let unaligned = target % 4 != 0 && listing && target_run == *run;
                if own || unaligned {
                    entries.far.insert(target);
                    entries.starts.remove(&target);
                    changed = true;
                } else if !entries.starts.contains(&target) {
                    if refused.contains(&target) || inside_function(target) {
                        inner.insert(target);
                    } else {
                        entries.starts.insert(target);
                        changed = true;
                    }
                }
            }
            let pointers = function
                .pools
                .iter()
                .filter(|&&word| image.contains(word, 4))
                .map(|&word| image.word(word))
                .filter(|value| value & 1 == 1)
                .map(|value| value & !1);
            for target in pointers {
                if !in_listing(target)
                    || entries.starts.contains(&target)
                    || entries.far.contains(&target)
                {
                    continue;
                }
                if refused.contains(&target) || inside_function(target) {
                    inner.insert(target);
                    continue;
                }
                let run_end = runs[area_of(target).unwrap().0].0.end();
                if walk(image, target, run_end, &entries)
                    .is_some_and(|candidate| flow::unnamed_function(image, &candidate))
                {
                    entries.starts.insert(target);
                    changed = true;
                }
            }
            // Flow runs into an entry only inside one function.
            if let Some(entry) = function.falls_into {
                if !firm.contains(&entry) && entries.starts.remove(&entry) {
                    refused.insert(entry);
                    changed = true;
                }
            }
        }
        if !changed {
            let planned = runs
                .iter()
                .map(|(area, members)| Planned {
                    area: area.clone(),
                    regions: members.clone(),
                    segments: partition(image, area, &entries),
                })
                .collect();
            return Ok(Plan {
                areas: planned,
                entries,
                inner,
                firm,
            });
        }
    }
    Err("listing entries did not settle in 64 rounds".into())
}

/// Everything a relisting reads.
pub struct Input<'a> {
    pub image: Image<'a>,
    /// Placed regions in address order.
    pub regions: Vec<Region>,
    pub names: Vec<Name>,
    /// Names that objects other than the regenerated listings refer to.
    pub external: BTreeSet<String>,
    pub incbin: Vec<Scaffold>,
    pub space: Vec<Scaffold>,
}

/// One entry of the linker script: a listing object, or a scaffold section.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Entry {
    Listing(String),
    Incbin { scaffold: usize, section: String },
}

/// Everything a relisting writes.
pub struct Output {
    /// Each new listing piece by its first address, as source text.
    pub pieces: BTreeMap<u32, String>,
    pub incbin: Vec<Scaffold>,
    pub space: Vec<Scaffold>,
    /// Per area, the linker-script entries it had and the ones it has now.
    pub script: Vec<(Vec<Entry>, Vec<Entry>)>,
    /// References that stay numbers, and names defined twice at one place.
    pub notes: Vec<String>,
    pub plan: Plan,
}

/// A piece: a function with the data that follows it, or leading data.
struct Piece<'a> {
    start: u32,
    end: u32,
    functions: Vec<&'a Function>,
}

/// Names defined at addresses of pieces and scaffolds.
#[derive(Default)]
struct Registry {
    /// Name and whether it is a Thumb function.
    defined: BTreeMap<u32, Vec<(String, bool)>>,
    /// Placeholders of listings that nothing needs yet, by address.
    dormant: BTreeMap<u32, Vec<(String, bool)>>,
    taken: BTreeSet<String>,
}

impl Registry {
    fn define(&mut self, address: u32, name: String, thumb: bool) {
        if self.taken.insert(name.clone()) {
            self.defined.entry(address).or_default().push((name, thumb));
        }
    }
    /// A name at `address`: a Thumb one when `thumb` is `Some(true)`, a
    /// plain one when `Some(false)`, either for `None` (code labels).
    fn name(&mut self, address: u32, thumb: Option<bool>) -> String {
        let fits = |candidate: &&(String, bool)| thumb.is_none_or(|want| candidate.1 == want);
        if let Some((name, _)) = self
            .defined
            .get(&address)
            .and_then(|names| names.iter().find(fits))
        {
            return name.clone();
        }
        let revived = self
            .dormant
            .get(&address)
            .and_then(|names| names.iter().find(fits))
            .cloned();
        let (name, is_thumb) = revived.unwrap_or_else(|| match thumb {
            Some(false) => (format!("Data_{address:08x}"), false),
            _ => (format!("Func_{address:08x}"), true),
        });
        self.define(address, name.clone(), is_thumb);
        name
    }
}

/// How references resolve: the pieces, scaffolds and names they can use.
struct World<'a> {
    input: &'a Input<'a>,
    rom_end: u32,
    /// Piece index by first address.
    piece_at: BTreeMap<u32, usize>,
    pieces: Vec<(u32, u32)>,
    /// Function starts in pieces.
    starts: BTreeSet<u32>,
    /// Row starts of pieces: addresses a listing label can sit at.
    rows: BTreeSet<u32>,
    /// Included data (and reserved RAM) that stays scaffold, by start: end.
    labelable: BTreeMap<u32, u32>,
    /// Every name by address, for references into maintained objects.
    by_address: BTreeMap<u32, Vec<&'a Name>>,
}

impl World<'_> {
    fn piece(&self, address: u32) -> Option<usize> {
        let (_, &index) = self.piece_at.range(..=address).next_back()?;
        (address < self.pieces[index].1).then_some(index)
    }
    fn in_scaffold(&self, address: u32) -> bool {
        self.labelable
            .range(..=address)
            .next_back()
            .is_some_and(|(_, &end)| address < end)
    }
    /// The nearest name at or before `address` inside its region that a
    /// word can use: `(name, offset)` so that `name + offset` is `value`.
    fn nearest(&self, address: u32, value: u32) -> Option<String> {
        let region = &self.input.regions[region_at(&self.input.regions, address)?];
        // A ROM copy of a section that runs elsewhere has only the script's
        // names; any other region has only names of its own bytes.
        let copy = region.object.starts_with("LOADADDR(");
        for (&at, names) in self.by_address.range(region.start..=address).rev() {
            for name in names.iter().filter(|name| name.absolute == copy) {
                if !name.thumb {
                    return Some(expression(&name.name, value - at));
                }
                if value & 1 == 1 {
                    return Some(expression(&name.name, value - at - 1));
                }
            }
        }
        None
    }
    /// The nearest name for a branch into maintained code.
    fn nearest_code(&self, target: u32) -> Option<String> {
        let region = &self.input.regions[region_at(&self.input.regions, target)?];
        self.by_address
            .range(region.start..=target)
            .rev()
            .find_map(|(&at, names)| {
                let name = names.iter().find(|name| !name.absolute)?;
                Some(expression(&name.name, target - at))
            })
    }
    /// The reserved RAM a RAM address just past it can extend to: the space
    /// region ending last at or before `value` in the same RAM, with nothing
    /// placed between.
    fn extendable(&self, value: u32) -> Option<usize> {
        let regions = &self.input.regions;
        if !(0x0200_0000..0x0400_0000).contains(&value) || region_at(regions, value).is_some() {
            return None;
        }
        let before = regions.partition_point(|region| region.start <= value);
        let index = before.checked_sub(1)?;
        let region = &regions[index];
        (matches!(region.kind, RegionKind::Space { .. })
            && region.end <= value
            && region.start >> 24 == value >> 24)
            .then_some(index)
    }
    /// Where `address` is, for a note.
    fn place(&self, address: u32) -> String {
        region_at(&self.input.regions, address).map_or_else(
            || "nothing placed".into(),
            |index| {
                let region = &self.input.regions[index];
                format!("{} {}", region.object, region.section)
            },
        )
    }
}

fn expression(name: &str, offset: u32) -> String {
    if offset == 0 {
        name.to_string()
    } else {
        format!("{name} + 0x{offset:x}")
    }
}

fn local(address: u32) -> String {
    format!(".L_{address:08x}")
}

/// What one piece's rows refer to, resolved.
struct Resolver<'a, 'w> {
    world: &'a World<'w>,
    registry: std::cell::RefCell<&'a mut Registry>,
    piece: usize,
    locals: std::cell::RefCell<BTreeSet<u32>>,
    notes: std::cell::RefCell<Vec<String>>,
}

impl Resolver<'_, '_> {
    fn same_piece(&self, address: u32) -> bool {
        self.world.piece(address) == Some(self.piece) && self.world.rows.contains(&address)
    }
    fn code(&self, site: u32, target: u32) -> String {
        let world = self.world;
        if world.starts.contains(&target) {
            return self.registry.borrow_mut().name(target, Some(true));
        }
        if self.same_piece(target) {
            self.locals.borrow_mut().insert(target);
            return local(target);
        }
        if (world.piece(target).is_some() && world.rows.contains(&target))
            || world.in_scaffold(target)
        {
            return self.registry.borrow_mut().name(target, None);
        }
        if let Some(name) = world
            .by_address
            .get(&target)
            .and_then(|names| names.iter().find(|name| !name.absolute))
        {
            return name.name.clone();
        }
        if let Some(name) = world.nearest_code(target) {
            return name;
        }
        self.notes.borrow_mut().push(format!(
            "{site:08x}: branch to {target:08x} in {} stays a number",
            world.place(target)
        ));
        format!("0x{target:08x}")
    }
    /// The expression for the word `value` stored at `at`.
    fn word(&self, at: u32, value: u32) -> String {
        let world = self.world;
        if !addressable(value, world.rom_end) {
            return format!("0x{value:08x}");
        }
        let code = value & !1;
        if value & 1 == 1 && world.starts.contains(&code) {
            return self.registry.borrow_mut().name(code, Some(true));
        }
        if let Some(index) = world.piece(value) {
            // A label sits on a row; an odd byte is its row's label plus one.
            let row = if world.rows.contains(&value) {
                value
            } else {
                code
            };
            if !world.rows.contains(&row) {
                self.notes
                    .borrow_mut()
                    .push(format!("{at:08x}: word {value:08x} points inside a row"));
                return format!("0x{value:08x}");
            }
            let name = if index == self.piece && !world.starts.contains(&row) {
                self.locals.borrow_mut().insert(row);
                local(row)
            } else if index == self.piece && value == row {
                self.locals.borrow_mut().insert(row);
                local(row)
            } else {
                self.registry.borrow_mut().name(row, Some(false))
            };
            return expression(&name, value - row);
        }
        if world.in_scaffold(value) || world.extendable(value).is_some() {
            return self.registry.borrow_mut().name(value, Some(false));
        }
        if let Some(names) = world.by_address.get(&value) {
            if let Some(name) = names.iter().find(|name| !name.thumb) {
                return name.name.clone();
            }
        }
        if value & 1 == 1 {
            if let Some(name) = world
                .by_address
                .get(&code)
                .and_then(|names| names.iter().find(|name| name.thumb))
            {
                return name.name.clone();
            }
        }
        if let Some(name) = world.nearest(value, value) {
            return name;
        }
        self.notes.borrow_mut().push(format!(
            "{at:08x}: word {value:08x} in {} names no place",
            world.place(value)
        ));
        format!("0x{value:08x}")
    }
}

impl render::Refer for Resolver<'_, '_> {
    fn branch(&self, site: u32, target: u32) -> String {
        self.code(site, target)
    }
    fn pool(&self, word: u32) -> Option<String> {
        self.same_piece(word).then(|| {
            self.locals.borrow_mut().insert(word);
            local(word)
        })
    }
}

/// Plan every area, name every referenced address, and render the pieces
/// and scaffolds.
pub fn relist(input: &Input) -> Result<Output, String> {
    let image = input.image;
    let rom_end = image.base + image.bytes.len() as u32;
    let plan = plan(image, &input.regions, &input.names)?;
    // Pieces: each function with the listing data after it.
    let mut pieces: Vec<Piece> = Vec::new();
    for planned in &plan.areas {
        let mut open = false;
        for segment in &planned.segments {
            match segment {
                Segment::Function(function) => {
                    pieces.push(Piece {
                        start: function.start,
                        end: function.end,
                        functions: vec![function],
                    });
                    open = true;
                }
                Segment::Data {
                    start,
                    end,
                    listing: true,
                } => match pieces.last_mut() {
                    Some(piece) if open && piece.end == *start => piece.end = *end,
                    _ => {
                        pieces.push(Piece {
                            start: *start,
                            end: *end,
                            functions: Vec::new(),
                        });
                        open = true;
                    }
                },
                Segment::Data { listing: false, .. } => open = false,
            }
        }
    }
    let mut instructions = BTreeMap::new();
    let mut pools = BTreeSet::new();
    let mut tables = BTreeMap::new();
    // Every address something names must start a row of its piece.
    let mut named: BTreeSet<u32> = input
        .names
        .iter()
        .filter(|name| !name.absolute)
        .map(|name| name.address)
        .collect();
    named.extend(plan.inner.iter().copied());
    for function in plan.functions() {
        instructions.extend(function.instructions.iter().map(|(at, ins)| (*at, ins)));
        pools.extend(function.pools.iter().copied());
        tables.extend(function.tables.iter().map(|(at, case)| (*at, *case)));
        named.extend(function.labels.iter().copied());
        named.extend(function.exits.iter().copied());
        named.extend(function.calls.iter().copied());
        named.extend(function.tables.values().copied());
        for &word in &function.pools {
            if image.contains(word, 4) {
                named.insert(image.word(word) & !1);
            }
        }
    }
    let mut rows = BTreeSet::new();
    for piece in &pieces {
        let mut cursor = piece.start;
        while cursor < piece.end {
            rows.insert(cursor);
            let size = match instructions.get(&cursor) {
                Some(ins) => ins.size,
                None if cursor % 4 == 0
                    && (pools.contains(&cursor) || tables.contains_key(&cursor)) =>
                {
                    4
                }
                None => 2,
            };
            let whole = cursor + size <= piece.end
                && named.range(cursor + 1..cursor + size).next().is_none();
            cursor += if whole { size } else { 2 };
        }
    }
    // Scaffold layouts at their placed addresses.
    let placed_layout =
        |scaffolds: &[Scaffold], index: usize, region: &Region| -> Result<Layout, String> {
            let section = scaffolds[index]
                .sections
                .iter()
                .find(|section| section.name == region.section)
                .ok_or_else(|| format!("{} has no section {}", region.object, region.section))?;
            let layout = Layout::new(section, region.start);
            if layout.end != region.end {
                return Err(format!(
                    "{} {} lays out to {:08x}, placed to {:08x}",
                    region.object, region.section, layout.end, region.end
                ));
            }
            Ok(layout)
        };
    let mut layouts: BTreeMap<usize, Layout> = BTreeMap::new();
    for (index, region) in input.regions.iter().enumerate() {
        match region.kind {
            RegionKind::Incbin { scaffold } => {
                layouts.insert(index, placed_layout(&input.incbin, scaffold, region)?);
            }
            RegionKind::Space { scaffold } => {
                layouts.insert(index, placed_layout(&input.space, scaffold, region)?);
            }
            _ => {}
        }
    }
    // What stays scaffold: included data outside functions, and all space.
    let mut labelable = BTreeMap::new();
    for planned in &plan.areas {
        for segment in &planned.segments {
            if let Segment::Data {
                start,
                end,
                listing: false,
            } = segment
            {
                labelable.insert(*start, *end);
            }
        }
    }
    for (index, region) in input.regions.iter().enumerate() {
        if matches!(region.kind, RegionKind::Space { .. })
            || (matches!(region.kind, RegionKind::Incbin { .. })
                && !plan
                    .areas
                    .iter()
                    .any(|planned| planned.regions.contains(&index)))
        {
            labelable.insert(region.start, region.end);
        }
    }
    let mut by_address: BTreeMap<u32, Vec<&Name>> = BTreeMap::new();
    for name in &input.names {
        by_address.entry(name.address).or_default().push(name);
    }
    let starts: BTreeSet<u32> = plan.functions().map(|function| function.start).collect();
    let world = World {
        input,
        rom_end,
        piece_at: pieces
            .iter()
            .enumerate()
            .map(|(index, piece)| (piece.start, index))
            .collect(),
        pieces: pieces
            .iter()
            .map(|piece| (piece.start, piece.end))
            .collect(),
        starts,
        rows,
        labelable,
        by_address: by_address.clone(),
    };
    // Names: those of scaffolds stay; a listing's stay when they are more
    // than a placeholder or something else refers to them.
    let mut registry = Registry::default();
    let in_pieces = |address: u32| world.piece(address).is_some();
    for name in input.names.iter().filter(|name| !name.absolute) {
        let keep = !in_pieces(name.address)
            || !placeholder(&name.name)
            || input.external.contains(&name.name);
        if keep {
            registry.define(name.address, name.name.clone(), name.thumb);
        } else {
            registry
                .dormant
                .entry(name.address)
                .or_default()
                .push((name.name.clone(), name.thumb));
        }
    }
    let mut notes = Vec::new();
    for piece in &pieces {
        if !piece.functions.is_empty() {
            registry.name(piece.start, Some(true));
        }
        if let Some(names) = registry.defined.get(&piece.start) {
            if names.len() > 1 {
                let list: Vec<&str> = names.iter().map(|(name, _)| name.as_str()).collect();
                notes.push(format!(
                    "{:08x}: several names: {}",
                    piece.start,
                    list.join(" ")
                ));
            }
        }
    }
    // Render each piece twice: first to learn every name it needs, then
    // with every piece's names known.
    let mut texts = BTreeMap::new();
    for round in 0..2 {
        for (index, piece) in pieces.iter().enumerate() {
            let resolver = Resolver {
                world: &world,
                registry: std::cell::RefCell::new(&mut registry),
                piece: index,
                locals: std::cell::RefCell::new(BTreeSet::new()),
                notes: std::cell::RefCell::new(Vec::new()),
            };
            let mut lines = Vec::new();
            let mut cursor = piece.start;
            while cursor < piece.end {
                let next = world
                    .rows
                    .range(cursor + 1..)
                    .next()
                    .copied()
                    .unwrap_or(piece.end)
                    .min(piece.end);
                let text = if let Some(ins) = instructions
                    .get(&cursor)
                    .filter(|ins| cursor + ins.size == next)
                {
                    render::instruction(image, ins, &resolver)
                } else if next == cursor + 4 && tables.contains_key(&cursor) {
                    let case = tables[&cursor];
                    format!(".4byte {}", resolver.code(cursor, case))
                } else if next == cursor + 4 {
                    format!(".4byte {}", resolver.word(cursor, image.word(cursor)))
                } else {
                    let mut rows = Vec::new();
                    let mut at = cursor;
                    while at < next {
                        rows.push(render::halfword(image.half(at)));
                        at += 2;
                    }
                    rows.join("\n\t")
                };
                lines.push((cursor, text));
                cursor = next;
            }
            if round == 0 {
                continue;
            }
            let locals = resolver.locals.into_inner();
            notes.extend(resolver.notes.into_inner());
            let registry = resolver.registry.into_inner();
            let mut text = String::from(".syntax unified\n\t.thumb\n");
            for (at, line) in lines {
                if let Some(names) = registry.defined.get(&at) {
                    for (name, thumb) in names {
                        text.push_str(&format!("\t.global {name}\n"));
                        if *thumb {
                            text.push_str("\t.thumb_func\n");
                        }
                        text.push_str(&format!("{name}:\n"));
                    }
                }
                if locals.contains(&at) {
                    text.push_str(&format!("{}:\n", local(at)));
                }
                text.push('\t');
                text.push_str(&line);
                text.push('\n');
            }
            texts.insert(piece.start, text);
        }
    }
    // Scaffolds: included data outside functions and reserved RAM, with
    // every name the registry defines inside them.
    let mut incbin = input.incbin.clone();
    let mut space = input.space.clone();
    let mut replaced: BTreeMap<(usize, String), Vec<Layout>> = BTreeMap::new();
    for (index, region) in input.regions.iter().enumerate() {
        let (scaffold, is_incbin) = match region.kind {
            RegionKind::Incbin { scaffold } => (scaffold, true),
            RegionKind::Space { scaffold } => (scaffold, false),
            _ => continue,
        };
        let layout = &layouts[&index];
        let slices: Vec<(u32, u32)> = if is_incbin {
            match plan
                .areas
                .iter()
                .find(|planned| planned.regions.contains(&index))
            {
                Some(planned) => planned
                    .segments
                    .iter()
                    .filter_map(|segment| match segment {
                        Segment::Data {
                            start,
                            end,
                            listing: false,
                        } if *start >= region.start && *end <= region.end => Some((*start, *end)),
                        _ => None,
                    })
                    .collect(),
                None => vec![(region.start, region.end)],
            }
        } else {
            // Reserved RAM runs on to the last name placed past its end.
            let limit = input
                .regions
                .get(index + 1)
                .map_or(u32::MAX, |next| next.start);
            let to = registry
                .defined
                .range(region.end..limit)
                .next_back()
                .map_or(region.end, |(at, _)| (*at).max(region.end));
            vec![(region.start, to)]
        };
        let prefix = region
            .section
            .rsplit_once('.')
            .map_or(region.section.as_str(), |(prefix, _)| prefix);
        let mut result = Vec::new();
        for (from, to) in slices {
            let name = if from == region.start {
                region.section.clone()
            } else {
                format!("{prefix}.{from:08x}")
            };
            let mut slice = layout.slice(from, to, name);
            for (&at, names) in registry.defined.range(from..to) {
                let present = slice.labels.entry(at).or_default();
                for (name, _) in names {
                    if !present.contains(name) {
                        present.push(name.clone());
                    }
                }
            }
            result.push(slice);
        }
        replaced.insert(
            (
                scaffold + if is_incbin { 0 } else { usize::MAX / 2 },
                region.section.clone(),
            ),
            result,
        );
    }
    let rebuild = |scaffolds: &mut Vec<Scaffold>, offset: usize| {
        for (index, scaffold) in scaffolds.iter_mut().enumerate() {
            let mut sections = Vec::new();
            for section in &scaffold.sections {
                match replaced.get(&(index + offset, section.name.clone())) {
                    Some(layouts) => sections.extend(layouts.iter().map(Layout::section)),
                    None => sections.push(section.clone()),
                }
            }
            scaffold.sections = sections;
        }
    };
    rebuild(&mut incbin, 0);
    rebuild(&mut space, usize::MAX / 2);
    // The linker script: each area's old entries and its new ones.
    let mut script = Vec::new();
    for planned in &plan.areas {
        let entry = |index: usize| -> Entry {
            let region = &input.regions[index];
            match region.kind {
                RegionKind::Incbin { scaffold } => Entry::Incbin {
                    scaffold,
                    section: region.section.clone(),
                },
                _ => Entry::Listing(region.object.clone()),
            }
        };
        let old: Vec<Entry> = planned.regions.iter().map(|&index| entry(index)).collect();
        let mut new = Vec::new();
        let mut cursor = planned.area.start;
        while cursor < planned.area.end() {
            if let Some(&index) = world.piece_at.get(&cursor) {
                new.push(Entry::Listing(format!("{cursor:08x}")));
                cursor = pieces[index].end;
                continue;
            }
            let region =
                region_at(&input.regions, cursor).ok_or("an area address outside every region")?;
            let RegionKind::Incbin { scaffold } = input.regions[region].kind else {
                return Err(format!("{cursor:08x} is neither a piece nor included data"));
            };
            let layouts = &replaced[&(scaffold, input.regions[region].section.clone())];
            let slice = layouts
                .iter()
                .find(|layout| layout.start == cursor)
                .ok_or_else(|| format!("no piece or data begins at {cursor:08x}"))?;
            new.push(Entry::Incbin {
                scaffold,
                section: slice.name.clone(),
            });
            cursor = slice.end;
        }
        if old != new {
            script.push((old, new));
        }
    }
    notes.sort();
    notes.dedup();
    Ok(Output {
        pieces: texts,
        incbin,
        space,
        script,
        notes,
        plan,
    })
}
