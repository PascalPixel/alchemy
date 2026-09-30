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

pub use flow::{partition, trace, walk, Area, Entries, Function, Image, Part, Segment};

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
    /// Defined by the linker script, not by bytes in a section: a value, or
    /// an alias of a place another name owns. It can be referred to, but no
    /// listing or scaffold defines it.
    pub absolute: bool,
}

/// A placeholder: `Func_`, `Data_` or `sub_` and the address it spells.
pub fn placeholder(name: &str) -> bool {
    ["Func_", "Data_", "sub_"]
        .iter()
        .find_map(|prefix| name.strip_prefix(prefix))
        .is_some_and(|rest| rest.len() == 8 && rest.bytes().all(|b| b.is_ascii_hexdigit()))
}

/// Whether a name spells an address, as a placeholder does or a name with
/// an address after an underscore: at a place with other names, a name
/// that says what the place is serves before it.
pub fn spells_address(name: &str) -> bool {
    name.split('_')
        .skip(1)
        .any(|part| part.len() == 8 && part.bytes().all(|b| b.is_ascii_hexdigit()))
}

/// The first of `names` that does not spell an address, or else the first.
fn preferred<T>(names: impl IntoIterator<Item = T>, spells: impl Fn(&T) -> bool) -> Option<T> {
    let mut first = None;
    for candidate in names {
        if !spells(&candidate) {
            return Some(candidate);
        }
        first.get_or_insert(candidate);
    }
    first
}

/// The names a linker script assigns (`NAME = EXPRESSION;`, bare or in a
/// `PROVIDE`): aliases of places other names own, and values the script
/// computes. No section's bytes define them, so no label may.
pub fn assigned(script: &str) -> BTreeSet<String> {
    let mut text = String::new();
    let mut rest = script;
    while let Some(at) = rest.find("/*") {
        text.push_str(&rest[..at]);
        rest = rest[at + 2..]
            .find("*/")
            .map_or("", |end| &rest[at + 2 + end + 2..]);
    }
    text.push_str(rest);
    text.split([';', '{', '}'])
        .filter_map(|statement| {
            let statement = statement.trim();
            let statement = ["PROVIDE_HIDDEN(", "PROVIDE("]
                .iter()
                .find_map(|provide| statement.strip_prefix(provide))
                .unwrap_or(statement);
            let (name, value) = statement.split_once('=')?;
            let name = name.trim();
            let symbol = name
                .chars()
                .next()
                .is_some_and(|c| c.is_ascii_alphabetic() || c == '_')
                && name
                    .chars()
                    .all(|c| c.is_ascii_alphanumeric() || matches!(c, '_' | '.' | '$'));
            (symbol && !value.starts_with('=')).then(|| name.to_string())
        })
        .collect()
}

/// What occupies a placed stretch of the image, as far as labels go.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum RegionKind {
    /// A listing: regenerated, or kept as written (hand-annotated pieces).
    Listing { regenerate: bool },
    /// A listing of data words, regenerated: it holds no code.
    Words,
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

/// What a region adds to an area, if it is relisted at all.
fn part(kind: &RegionKind) -> Option<Part> {
    match kind {
        RegionKind::Listing { regenerate: true } => Some(Part::Code),
        RegionKind::Words => Some(Part::Words),
        RegionKind::Incbin { .. } => Some(Part::Included),
        _ => None,
    }
}

/// The areas of `regions`: each maximal run of side-by-side regenerated
/// listings and included data, with the indices of its regions.
pub fn areas(regions: &[Region]) -> Vec<(Area, Vec<usize>)> {
    let mut areas: Vec<(Area, Vec<usize>)> = Vec::new();
    for (index, region) in regions.iter().enumerate() {
        let Some(part) = part(&region.kind) else {
            continue;
        };
        match areas.last_mut() {
            Some((area, members))
                if area.end() == region.start && members.last() == Some(&(index - 1)) =>
            {
                area.parts.push((region.end, part));
                members.push(index);
            }
            _ => areas.push((
                Area {
                    start: region.start,
                    parts: vec![(region.end, part)],
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
    let area_of = |address: u32| -> Option<(usize, Part)> {
        let index = runs.partition_point(|(area, _)| area.start <= address);
        let index = index.checked_sub(1)?;
        let area = &runs[index].0;
        (address < area.end()).then(|| (index, area.part(address).2))
    };
    let in_listing = |address: u32| area_of(address).is_some_and(|(_, part)| part == Part::Code);
    let mut firm: BTreeSet<u32> = runs
        .iter()
        .filter_map(|(area, _)| area.opening(image))
        .collect();
    let mut weak = BTreeSet::new();
    for name in names
        .iter()
        .filter(|name| name.thumb && in_listing(name.address))
    {
        if placeholder(&name.name) {
            // An earlier listing's cut off a word boundary is no function.
            if name.address % 4 == 0 {
                weak.insert(name.address);
            }
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
    // Thumb pointers in listings of data words, into listings of code, at
    // a word boundary where GCC begins a function; a pointer off one names
    // a place inside a function, such as where a script resumes.
    let mut tabled = BTreeSet::new();
    for (area, _) in &runs {
        let mut from = area.start;
        for &(end, part) in &area.parts {
            if part == Part::Words {
                let mut at = from.next_multiple_of(4);
                while at + 4 <= end {
                    let value = image.word(at);
                    if value & 3 == 1 && in_listing(value & !1) {
                        tabled.insert(value & !1);
                    }
                    at += 4;
                }
            }
            from = end;
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
                let Some((target_run, part)) = area_of(target) else {
                    continue;
                };
                if firm.contains(&target) || entries.far.contains(&target) {
                    continue;
                }
                let own = function.start < target && target < function.end;
                let unaligned = target % 4 != 0 && part == Part::Code && target_run == *run;
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
            // Flow runs into an entry, and a function's own extent holds
            // one, only inside one function.
            let held: Vec<u32> = entries
                .starts
                .range(function.start + 1..function.end)
                .copied()
                .chain(function.falls_into)
                .filter(|entry| !firm.contains(entry))
                .collect();
            for entry in held {
                if entries.starts.remove(&entry) {
                    refused.insert(entry);
                    changed = true;
                }
            }
        }
        // A listing of data words points at functions as pools do.
        for &target in &tabled {
            if entries.starts.contains(&target) || entries.far.contains(&target) {
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
    /// Where cartridge ROM ends: words below it point into ROM.
    pub rom_end: u32,
    /// Another image's placed regions in address order, whose names this
    /// image has as absolute ones: the main image, for an overlay.
    pub foreign: Vec<Region>,
    /// The address a new placeholder spells for the image's first byte.
    pub spell: u32,
    pub pieces: Pieces,
}

/// How new pieces are written: each its own listing, or each its own
/// section of one listing object.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Pieces {
    Files,
    Sections { object: String },
}

/// One entry of the linker script: a listing object's section, or a
/// scaffold section.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Entry {
    Listing { object: String, section: String },
    Incbin { scaffold: usize, section: String },
}

/// Everything a relisting writes.
pub struct Output {
    /// Each new listing piece by its first address, as source text.
    pub pieces: BTreeMap<u32, String>,
    /// The pieces that hold only data words.
    pub words: BTreeSet<u32>,
    pub incbin: Vec<Scaffold>,
    pub space: Vec<Scaffold>,
    /// Per area, the linker-script entries it had and the ones it has now.
    pub script: Vec<(Vec<Entry>, Vec<Entry>)>,
    /// References that stay numbers, and names defined twice at one place.
    pub notes: Vec<String>,
    pub plan: Plan,
}

/// A piece: a function with the data that follows it, leading data, or a
/// listing of data words.
struct Piece<'a> {
    start: u32,
    end: u32,
    functions: Vec<&'a Function>,
    words: bool,
}

/// Names defined at addresses of pieces and scaffolds.
#[derive(Default)]
struct Registry {
    /// Name and whether it is a Thumb function.
    defined: BTreeMap<u32, Vec<(String, bool)>>,
    /// Placeholders of listings that nothing needs yet, by address.
    dormant: BTreeMap<u32, Vec<(String, bool)>>,
    taken: BTreeSet<String>,
    /// What an address less this spells in a new placeholder.
    shift: u32,
    /// Names the script or another image gives a value.
    reserved: BTreeSet<String>,
    /// Placeholders spelled for an address while another address has them,
    /// such as a scaffold label that spells the byte after its own.
    clashes: BTreeMap<String, u32>,
    /// Scaffold addresses words point at that no object holds yet, each
    /// with where its scaffold's bytes run from; settled lowest first.
    wanted: BTreeMap<u32, u32>,
    /// Whether `wanted` has been settled: from then on a word into a
    /// scaffold that no object holds takes a label at once.
    settled: bool,
}

impl Registry {
    /// The scaffold object that holds `value`, whose scaffold's bytes run
    /// from `from`: the nearest name at or before it plus the offset, one
    /// that says what the place is before one that spells its address. A
    /// Thumb name serves only a Thumb pointer past its start, and a
    /// placeholder that spells another place holds nothing.
    fn holder(&self, from: u32, value: u32) -> Option<String> {
        let (&at, names) = self.defined.range(from..=value).next_back()?;
        let spells = |name: &str| {
            let spelled = u32::from_str_radix(&name[name.len() - 8..], 16).ok();
            spelled == Some(at) || spelled == at.checked_sub(self.shift)
        };
        let fits = names.iter().filter(|(name, thumb)| {
            (!thumb || (value & 1 == 1 && value > at)) && (!placeholder(name) || spells(name))
        });
        let (name, thumb) = preferred(fits, |(name, _)| spells_address(name))?;
        Some(expression(name, value - at - u32::from(*thumb)))
    }
    /// A word's name for `value` in a scaffold whose bytes run from `from`.
    /// Before the wanted places are settled, one no object holds yet is
    /// only noted; after, it takes a label of its own.
    fn object(&mut self, from: u32, value: u32) -> String {
        if let Some(name) = self.holder(from, value) {
            return name;
        }
        if self.settled {
            return self.name(value, Some(false));
        }
        self.wanted.insert(value, from);
        format!("0x{value:08x}")
    }
    /// Label the wanted places lowest first, so that each later one in the
    /// same unnamed stretch is held by the label before it.
    fn settle(&mut self) {
        for (value, from) in std::mem::take(&mut self.wanted) {
            if self.holder(from, value).is_none() {
                self.name(value, Some(false));
            }
        }
        self.settled = true;
    }
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
        let spell = |at: u32| match thumb {
            Some(false) => (format!("Data_{at:08x}"), false),
            _ => (format!("Func_{at:08x}"), true),
        };
        let (name, is_thumb) = revived.unwrap_or_else(|| {
            // A spelling a name of another image has would hide that name:
            // spell the address itself instead.
            let spelled = spell(address - self.shift);
            if self.reserved.contains(&spelled.0) {
                spell(address)
            } else {
                spelled
            }
        });
        self.define(address, name.clone(), is_thumb);
        let here = self
            .defined
            .get(&address)
            .is_some_and(|names| names.iter().any(|(defined, _)| *defined == name));
        if !here {
            self.clashes.insert(name.clone(), address);
        }
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
    /// Which pieces hold only data words.
    words: Vec<bool>,
    /// Function starts in pieces.
    starts: BTreeSet<u32>,
    /// Row starts of pieces: addresses a listing label can sit at.
    rows: BTreeSet<u32>,
    /// Included data (and reserved RAM) that stays scaffold, by start: end.
    labelable: BTreeMap<u32, u32>,
    /// Every name by address, for references into maintained objects.
    by_address: BTreeMap<u32, Vec<&'a Name>>,
    /// The labels of reserved RAM scaffolds.
    reserved_labels: BTreeSet<&'a str>,
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
    /// Where the scaffold bytes holding `value` run from: its stretch of
    /// included data or reserved RAM. Past the end of reserved RAM nothing
    /// is known of what lies where, so each place a word reaches there is
    /// its own.
    fn scaffold_from(&self, value: u32) -> Option<u32> {
        match self.labelable.range(..=value).next_back() {
            Some((&start, &end)) if value < end => Some(start),
            _ => self.extendable(value).map(|_| value),
        }
    }
    /// The nearest name at or before `address` inside its region that a
    /// word can use: `(name, offset)` so that `name + offset` is `value`.
    fn nearest(&self, address: u32, value: u32) -> Option<String> {
        // A ROM copy of a section that runs elsewhere has only the script's
        // names, and another image's region only that image's; any other
        // region has only names of its own bytes.
        let (region, absolute) = match region_at(&self.input.regions, address) {
            Some(index) => {
                let region = &self.input.regions[index];
                (region, region.object.starts_with("LOADADDR("))
            }
            None => (
                &self.input.foreign[region_at(&self.input.foreign, address)?],
                true,
            ),
        };
        for (&at, names) in self.by_address.range(region.start..=address).rev() {
            let fits = names
                .iter()
                .filter(|name| name.absolute == absolute && (!name.thumb || value & 1 == 1));
            if let Some(name) = preferred(fits, |name| spells_address(&name.name)) {
                return Some(expression(&name.name, value - at - u32::from(name.thumb)));
            }
        }
        None
    }
    /// Whether a word can use `name` for `address`: a name the script or
    /// another image gives a value serves only outside every region, or in
    /// a ROM copy of a section that runs elsewhere.
    fn usable(&self, name: &Name, address: u32) -> bool {
        !name.absolute
            || region_at(&self.input.regions, address)
                .is_none_or(|index| self.input.regions[index].object.starts_with("LOADADDR("))
    }
    /// The nearest name for a branch into maintained code.
    fn nearest_code(&self, target: u32) -> Option<String> {
        let region = &self.input.regions[region_at(&self.input.regions, target)?];
        self.by_address
            .range(region.start..=target)
            .rev()
            .find_map(|(&at, names)| {
                let own = names.iter().filter(|name| !name.absolute);
                let name = preferred(own, |name| spells_address(&name.name))?;
                Some(expression(&name.name, target - at))
            })
    }
    /// The reserved RAM a RAM address just past it can extend to: the space
    /// region ending last at or before `value` in the same RAM, with nothing
    /// placed between and no name up to it but reserved RAM's own: not a
    /// variable another object defines there, nor a name another image
    /// gives, such as the main image's RAM after an overlay's own.
    fn extendable(&self, value: u32) -> Option<usize> {
        let regions = &self.input.regions;
        if !(0x0200_0000..0x0400_0000).contains(&value) || region_at(regions, value).is_some() {
            return None;
        }
        let before = regions.partition_point(|region| region.start <= value);
        let index = before.checked_sub(1)?;
        let region = &regions[index];
        let named = self.by_address.range(region.end..=value).any(|(_, names)| {
            names
                .iter()
                .any(|name| name.absolute || !self.reserved_labels.contains(name.name.as_str()))
        });
        (matches!(region.kind, RegionKind::Space { .. })
            && region.end <= value
            && region.start >> 24 == value >> 24
            && !named)
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

/// The section of one listing object that holds the piece at `start`.
pub fn section(start: u32, words: bool) -> String {
    if words {
        format!(".rodata.x{start:08x}")
    } else {
        format!(".text.x{start:08x}")
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
        // A branch names the function at its target: a Thumb name that says
        // what it is before a label of data that begins there, such as a
        // table of veneers, and that before a name spelling the address.
        if let Some(name) = world.by_address.get(&target).and_then(|names| {
            let own = || names.iter().filter(|name| !name.absolute);
            own()
                .find(|name| name.thumb && !spells_address(&name.name))
                .or_else(|| preferred(own(), |name| spells_address(&name.name)))
        }) {
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
        // A listing of data words packs coordinates, ids and fixed-point
        // numbers into words that fall in ROM or RAM as often as pointers
        // do, even on another image's names (0x02000000 is 512.0 as often as
        // the first byte of EWRAM): there a word names only its own image's
        // places, the only ones its pointers are seen to reach, and never
        // reserves RAM.
        let data = world.words[self.piece];
        // One name per place: a word into a scaffold object is that
        // object's name plus the offset, never a label of its own.
        if let Some(from) = world.scaffold_from(value).filter(|_| !data) {
            return self.registry.borrow_mut().object(from, value);
        }
        let own = |name: &&&Name| !(data && name.absolute);
        if let Some(names) = world.by_address.get(&value) {
            let fits = names
                .iter()
                .filter(own)
                .filter(|name| !name.thumb && world.usable(name, value));
            if let Some(name) = preferred(fits, |name| spells_address(&name.name)) {
                return name.name.clone();
            }
        }
        if value & 1 == 1 {
            if let Some(name) = world.by_address.get(&code).and_then(|names| {
                let fits = names
                    .iter()
                    .filter(own)
                    .filter(|name| name.thumb && world.usable(name, code));
                preferred(fits, |name| spells_address(&name.name))
            }) {
                return name.name.clone();
            }
        }
        if data {
            self.notes.borrow_mut().push(format!(
                "{at:08x}: data word {value:08x} in {} stays a number",
                world.place(value)
            ));
            return format!("0x{value:08x}");
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
    /// A PC-relative load names its word only in a piece that starts on a
    /// word boundary: the assembler computes the offset from the section's
    /// start, which it takes to be word-aligned.
    fn pool(&self, word: u32) -> Option<String> {
        let aligned = self.world.pieces[self.piece].0 % 4 == 0;
        (aligned && self.same_piece(word)).then(|| {
            self.locals.borrow_mut().insert(word);
            local(word)
        })
    }
}

/// Plan every area, name every referenced address, and render the pieces
/// and scaffolds.
pub fn relist(input: &Input) -> Result<Output, String> {
    let image = input.image;
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
                        words: false,
                    });
                    open = true;
                }
                Segment::Data {
                    start,
                    end,
                    part: Part::Code,
                } => match pieces.last_mut() {
                    Some(piece) if open && piece.end == *start => piece.end = *end,
                    _ => {
                        pieces.push(Piece {
                            start: *start,
                            end: *end,
                            functions: Vec::new(),
                            words: false,
                        });
                        open = true;
                    }
                },
                Segment::Data {
                    start,
                    end,
                    part: Part::Words,
                } => {
                    pieces.push(Piece {
                        start: *start,
                        end: *end,
                        functions: Vec::new(),
                        words: true,
                    });
                    open = false;
                }
                Segment::Data {
                    part: Part::Included,
                    ..
                } => open = false,
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
    // Listing bytes no flow reaches, read as words where they are aligned.
    let mut unreached: BTreeMap<u32, u32> = BTreeMap::new();
    let mut words: BTreeMap<u32, u32> = BTreeMap::new();
    for planned in &plan.areas {
        for segment in &planned.segments {
            match segment {
                Segment::Data {
                    start,
                    end,
                    part: Part::Code,
                } => {
                    unreached.insert(*start, *end);
                }
                Segment::Data {
                    start,
                    end,
                    part: Part::Words,
                } => {
                    words.insert(*start, *end);
                }
                _ => {}
            }
        }
    }
    let within = |runs: &BTreeMap<u32, u32>, at: u32| {
        at % 4 == 0
            && runs
                .range(..=at)
                .next_back()
                .is_some_and(|(_, &end)| at + 4 <= end)
    };
    let data_word = |at: u32| within(&unreached, at) || within(&words, at);
    // Unreached listing bytes are mostly ARM routines behind a Thumb entry;
    // the words their PC-relative loads read are pool words too.
    for (&start, &end) in &unreached {
        let mut at = start.next_multiple_of(4);
        while at + 4 <= end {
            let word = image.word(at);
            if word & 0x0f7f_0000 == 0x051f_0000 {
                let offset = word & 0xfff;
                let target = if word & 0x0080_0000 != 0 {
                    at.checked_add(8 + offset)
                } else {
                    (at + 8).checked_sub(offset)
                };
                if let Some(target) = target.filter(|&target| data_word(target)) {
                    pools.insert(target);
                    named.insert(image.word(target) & !1);
                }
            }
            at += 4;
        }
    }
    // A listing of data words is read as words, each one a pointer when it
    // points into ROM or RAM.
    for (&start, &end) in &words {
        let mut at = start.next_multiple_of(4);
        while at + 4 <= end {
            let value = image.word(at);
            if addressable(value, input.rom_end) {
                pools.insert(at);
                named.insert(value & !1);
            }
            at += 4;
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
                    && (pools.contains(&cursor)
                        || tables.contains_key(&cursor)
                        || data_word(cursor)) =>
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
                part: Part::Included,
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
        rom_end: input.rom_end,
        piece_at: pieces
            .iter()
            .enumerate()
            .map(|(index, piece)| (piece.start, index))
            .collect(),
        pieces: pieces
            .iter()
            .map(|piece| (piece.start, piece.end))
            .collect(),
        words: pieces.iter().map(|piece| piece.words).collect(),
        starts,
        rows,
        labelable,
        by_address: by_address.clone(),
        reserved_labels: input
            .space
            .iter()
            .flat_map(|scaffold| &scaffold.sections)
            .flat_map(|section| &section.items)
            .filter_map(|item| match item {
                scaffold::Item::Label(name) => Some(name.as_str()),
                _ => None,
            })
            .collect(),
    };
    // Names: those of scaffolds stay; a listing's stay when they are more
    // than a placeholder or something else refers to them.
    let mut registry = Registry {
        shift: image.base - input.spell,
        reserved: input
            .names
            .iter()
            .filter(|name| name.absolute)
            .map(|name| name.name.clone())
            .collect(),
        ..Registry::default()
    };
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
        if round == 1 {
            registry.settle();
        }
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
                } else if next == cursor + 4 && pools.contains(&cursor) {
                    format!(".4byte {}", resolver.word(cursor, image.word(cursor)))
                } else if next == cursor + 4 {
                    format!(".4byte 0x{:08x}", image.word(cursor))
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
    if !registry.clashes.is_empty() {
        let list: Vec<String> = registry
            .clashes
            .iter()
            .map(|(name, address)| format!("{name} for {address:08x}"))
            .collect();
        return Err(format!(
            "placeholders another address already has: {}",
            list.join(", ")
        ));
    }
    // Scaffolds: included data outside functions and reserved RAM, with
    // every name the registry defines inside them.
    // The names the image already has, defined by the objects it links.
    let linked: BTreeSet<String> = input.names.iter().map(|name| name.name.clone()).collect();
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
                            part: Part::Included,
                        } if *start >= region.start && *end <= region.end => Some((*start, *end)),
                        _ => None,
                    })
                    .collect(),
                None => vec![(region.start, region.end)],
            }
        } else {
            // Reserved RAM runs on to the last name this relisting placed
            // past its end; names other objects define there are theirs.
            let limit = input
                .regions
                .get(index + 1)
                .map_or(u32::MAX, |next| next.start);
            let to = registry
                .defined
                .range(region.end..limit)
                .rev()
                .find(|(_, names)| names.iter().any(|(name, _)| !linked.contains(name)))
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
            // A name the script or another image gives a value is never a
            // label, even one an earlier relisting wrote.
            for names in slice.labels.values_mut() {
                names.retain(|name| !registry.reserved.contains(name));
            }
            slice.labels.retain(|_, names| !names.is_empty());
            // Reserved RAM ends at its last name, which may sit at its end.
            let last = if is_incbin { to } else { to + 1 };
            for (&at, names) in registry.defined.range(from..last) {
                let theirs = |name: &String| at >= region.end && linked.contains(name);
                for (name, _) in names.iter().filter(|(name, _)| !theirs(name)) {
                    let present = slice.labels.entry(at).or_default();
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
                _ => Entry::Listing {
                    object: region.object.clone(),
                    section: region.section.clone(),
                },
            }
        };
        let old: Vec<Entry> = planned.regions.iter().map(|&index| entry(index)).collect();
        let mut new = Vec::new();
        let mut cursor = planned.area.start;
        while cursor < planned.area.end() {
            if let Some(&index) = world.piece_at.get(&cursor) {
                new.push(match &input.pieces {
                    Pieces::Files => Entry::Listing {
                        object: format!("{cursor:08x}"),
                        section: ".text".into(),
                    },
                    Pieces::Sections { object } => Entry::Listing {
                        object: object.clone(),
                        section: section(cursor, pieces[index].words),
                    },
                });
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
        words: pieces
            .iter()
            .filter(|piece| piece.words)
            .map(|piece| piece.start)
            .collect(),
        incbin,
        space,
        script,
        notes,
        plan,
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::relist::flow::tests::halves;

    fn region(start: u32, end: u32, kind: RegionKind, object: &str, section: &str) -> Region {
        Region {
            start,
            end,
            kind,
            object: object.into(),
            section: section.into(),
        }
    }

    #[test]
    fn a_function_an_earlier_listing_cut_in_two_becomes_one_named_piece() {
        let base = 0x0800_0000;
        let mut bytes = halves(&[
            0xb500, // push {lr}
            0xf000, 0xf805, // bl 0x08000010
            0x4801, // ldr r0, [pc, #4]: the word at 0x0c
            0xbd00, // 0x08: pop {pc}; an earlier listing began a piece here
            0x0000, // padding
        ]);
        bytes.extend(0x0200_0004u32.to_le_bytes()); // 0x0c: a RAM address
        bytes.extend(halves(&[0x2001, 0x4770])); // 0x10: Beta
        bytes.extend([1, 2, 3, 4]); // 0x14: included data
        let listing = RegionKind::Listing { regenerate: true };
        let input = Input {
            image: Image {
                bytes: &bytes,
                base,
            },
            regions: vec![
                region(0x0200_0000, 0x0200_0010, RegionKind::Space { scaffold: 0 }, "sym", ".sym"),
                region(base, base + 8, listing.clone(), "08000000", ".text"),
                region(base + 8, base + 0x10, listing.clone(), "08000008", ".text"),
                region(base + 0x10, base + 0x14, listing, "08000010", ".text"),
                region(
                    base + 0x14,
                    base + 0x18,
                    RegionKind::Incbin { scaffold: 0 },
                    "unidentified",
                    ".unidentified.08000014",
                ),
            ],
            names: vec![
                Name {
                    name: "Func_08000008".into(),
                    address: base + 8,
                    thumb: true,
                    absolute: false,
                },
                Name {
                    name: "Beta".into(),
                    address: base + 0x10,
                    thumb: true,
                    absolute: false,
                },
            ],
            external: BTreeSet::new(),
            incbin: vec![scaffold::parse(
                "\t.section .unidentified.08000014,\"a\"\n\t.incbin \"baserom.gba\", 0x00000014, 0x00000004\n",
            )
            .unwrap()],
            space: vec![scaffold::parse("\t.section .sym,\"aw\",%nobits\n\t.space 0x00000010\n").unwrap()],
            rom_end: base + bytes.len() as u32,
            foreign: Vec::new(),
            spell: base,
            pieces: Pieces::Files,
        };
        let output = relist(&input).unwrap();
        assert_eq!(
            output.pieces.keys().copied().collect::<Vec<_>>(),
            vec![base, base + 0x10]
        );
        let first = &output.pieces[&base];
        assert!(first.contains("\t.thumb_func\nFunc_08000000:\n"), "{first}");
        assert!(first.contains("\tbl Beta\n"), "{first}");
        assert!(first.contains("\tldr r0, .L_0800000c\n"), "{first}");
        assert!(
            first.contains(".L_0800000c:\n\t.4byte Data_02000004\n"),
            "{first}"
        );
        assert!(!first.contains("Func_08000008"), "{first}");
        assert!(
            !first.contains("0x08") && !first.contains("0x02"),
            "{first}"
        );
        assert!(output.pieces[&(base + 0x10)].contains("\t.thumb_func\nBeta:\n"));
        let space = scaffold::render(&output.space[0]);
        assert!(
            space.contains("\t.space 0x00000004\n\t.global Data_02000004\nData_02000004:\n"),
            "{space}"
        );
        let incbin = |section: &str| Entry::Incbin {
            scaffold: 0,
            section: section.into(),
        };
        let listing = |object: &str| Entry::Listing {
            object: object.into(),
            section: ".text".into(),
        };
        assert_eq!(
            output.script,
            vec![(
                vec![
                    listing("08000000"),
                    listing("08000008"),
                    listing("08000010"),
                    incbin(".unidentified.08000014"),
                ],
                vec![
                    listing("08000000"),
                    listing("08000010"),
                    incbin(".unidentified.08000014"),
                ],
            )]
        );
    }

    #[test]
    fn a_script_assigns_aliases_and_values_but_not_sections_or_the_location() {
        let script = "/* A = B; in a comment */\nSECTIONS\n{\n    .text 0x2008000 :\n    {\n        \"*/x.o\"(.text)\n        . = ALIGN(4);\n        Size = ABSOLUTE(End - Start);\n    }\n    Alias = Owner;\n    PROVIDE(Spare = 0x10);\n    Rom = LOADADDR(.iwram) + (A - ADDR(.iwram));\n    Count += 1;\n}\n";
        assert_eq!(
            assigned(script),
            BTreeSet::from(["Alias", "Rom", "Size", "Spare"].map(String::from))
        );
    }

    #[test]
    fn a_word_into_a_scaffold_object_is_its_name_plus_the_offset() {
        let base = 0x0800_0000;
        let mut bytes = halves(&[
            0x4802, // ldr r0, [pc, #8]: the word at 0x0c
            0x4903, // ldr r1, [pc, #12]: the word at 0x10
            0x4a03, // ldr r2, [pc, #12]: the word at 0x14
            0x4b04, // ldr r3, [pc, #16]: the word at 0x18
            0x4770, // bx lr
            0x0000, // padding
        ]);
        bytes.extend(0x0200_0004u32.to_le_bytes()); // inside gBuffer
        bytes.extend(0x0200_000cu32.to_le_bytes()); // inside the variable after it
        bytes.extend(0x0800_0024u32.to_le_bytes()); // unnamed data, the later place first
        bytes.extend(0x0800_0020u32.to_le_bytes()); // and the earlier
        bytes.extend([0; 16]); // 0x1c: included data no label opens
        let input = Input {
            image: Image {
                bytes: &bytes,
                base,
            },
            regions: vec![
                region(0x0200_0000, 0x0200_0010, RegionKind::Space { scaffold: 0 }, "sym", ".sym"),
                region(
                    base,
                    base + 0x1c,
                    RegionKind::Listing { regenerate: true },
                    "08000000",
                    ".text",
                ),
                region(
                    base + 0x1c,
                    base + 0x2c,
                    RegionKind::Incbin { scaffold: 0 },
                    "unidentified",
                    ".unidentified.0800001c",
                ),
            ],
            names: vec![
                Name {
                    name: "gBuffer".into(),
                    address: 0x0200_0000,
                    thumb: false,
                    absolute: false,
                },
                // The script's alias of gBuffer's place.
                Name {
                    name: "gBufferAlias".into(),
                    address: 0x0200_0000,
                    thumb: false,
                    absolute: true,
                },
                Name {
                    name: "Data_02000008".into(),
                    address: 0x0200_0008,
                    thumb: false,
                    absolute: false,
                },
            ],
            external: BTreeSet::new(),
            incbin: vec![scaffold::parse(
                "\t.section .unidentified.0800001c,\"a\"\n\t.incbin \"baserom.gba\", 0x0000001c, 0x00000010\n",
            )
            .unwrap()],
            space: vec![scaffold::parse(
                // An earlier relisting wrote the alias as a label too.
                "\t.section .sym,\"aw\",%nobits\n\t.global gBuffer\ngBuffer:\n\t.global gBufferAlias\ngBufferAlias:\n\t.space 0x00000008\n\t.global Data_02000008\nData_02000008:\n\t.space 0x00000008\n",
            )
            .unwrap()],
            rom_end: base + bytes.len() as u32,
            foreign: Vec::new(),
            spell: base,
            pieces: Pieces::Files,
        };
        let output = relist(&input).unwrap();
        let piece = &output.pieces[&base];
        assert!(
            piece.contains(
                "\t.4byte gBuffer + 0x4\n.L_08000010:\n\t.4byte Data_02000008 + 0x4\n.L_08000014:\n\t.4byte Data_08000020 + 0x4\n.L_08000018:\n\t.4byte Data_08000020\n"
            ),
            "{piece}"
        );
        let space = scaffold::render(&output.space[0]);
        assert_eq!(
            space,
            "\t.section .sym,\"aw\",%nobits\n\t.global gBuffer\ngBuffer:\n\t.space 0x00000008\n\t.global Data_02000008\nData_02000008:\n\t.space 0x00000008\n"
        );
        let incbin = scaffold::render(&output.incbin[0]);
        assert_eq!(
            incbin,
            "\t.section .unidentified.0800001c,\"a\"\n\t.incbin \"baserom.gba\", 0x0000001c, 0x00000004\n\t.global Data_08000020\nData_08000020:\n\t.incbin \"baserom.gba\", 0x00000020, 0x0000000c\n"
        );
    }

    #[test]
    fn a_placeholder_another_address_has_stops_the_relisting() {
        let base = 0x0800_0000;
        let mut bytes = halves(&[
            0x4800, // ldr r0, [pc, #0]: the word at 0x04
            0x4770, // bx lr
        ]);
        bytes.extend(0x0800_0009u32.to_le_bytes()); // 0x04: the byte after 0x08
        bytes.extend([1, 2, 3, 4]); // 0x08: included data
        let input = Input {
            image: Image {
                bytes: &bytes,
                base,
            },
            regions: vec![
                region(
                    base,
                    base + 8,
                    RegionKind::Listing { regenerate: true },
                    "08000000",
                    ".text",
                ),
                region(
                    base + 8,
                    base + 0x0c,
                    RegionKind::Incbin { scaffold: 0 },
                    "unidentified",
                    ".unidentified.08000008",
                ),
            ],
            // An earlier tool put the name for 0x08000009 on 0x08000008.
            names: vec![Name {
                name: "Data_08000009".into(),
                address: base + 8,
                thumb: false,
                absolute: false,
            }],
            external: BTreeSet::new(),
            incbin: vec![scaffold::parse(
                "\t.section .unidentified.08000008,\"a\"\n\t.global Data_08000009\nData_08000009:\n\t.incbin \"baserom.gba\", 0x00000008, 0x00000004\n",
            )
            .unwrap()],
            space: Vec::new(),
            rom_end: base + bytes.len() as u32,
            foreign: Vec::new(),
            spell: base,
            pieces: Pieces::Files,
        };
        let error = relist(&input).err().unwrap();
        assert!(error.contains("Data_08000009 for 08000009"), "{error}");
    }

    #[test]
    fn an_overlay_reserves_the_ram_past_its_image_up_to_the_main_images_next_name() {
        let base = 0x0200_8000;
        let mut bytes = halves(&[
            0x4802, // ldr r0, [pc, #8]: the word at 0x0c
            0x4903, // ldr r1, [pc, #12]: the word at 0x10
            0x4a03, // ldr r2, [pc, #12]: the word at 0x14
            0x4770, // bx lr
            0x0000, 0x0000,
        ]);
        bytes.extend(0x0200_8024u32.to_le_bytes()); // its own RAM, past its image
        bytes.extend(0x0200_8804u32.to_le_bytes()); // the main image's RAM after it
        bytes.extend(0x0200_8040u32.to_le_bytes()); // a variable C code defines there
        let object = "resource_x_overlay";
        let end = base + bytes.len() as u32;
        let input = Input {
            image: Image {
                bytes: &bytes,
                base,
            },
            regions: vec![
                region(
                    base,
                    end,
                    RegionKind::Listing { regenerate: true },
                    object,
                    ".text",
                ),
                region(end, end, RegionKind::Space { scaffold: 0 }, object, ".bss"),
            ],
            names: vec![
                Name {
                    name: "gOverlayArea".into(),
                    address: base,
                    thumb: false,
                    absolute: true,
                },
                Name {
                    name: "gMapBuffer".into(),
                    address: 0x0200_8800,
                    thumb: false,
                    absolute: true,
                },
                Name {
                    name: "gCommonVar".into(),
                    address: 0x0200_8040,
                    thumb: false,
                    absolute: false,
                },
            ],
            external: BTreeSet::new(),
            incbin: Vec::new(),
            space: vec![scaffold::parse("\t.section .bss,\"aw\",%nobits\n").unwrap()],
            rom_end: 0x0900_0000,
            foreign: vec![region(
                0x0200_0000,
                0x0204_0000,
                RegionKind::Other,
                "sym_ewram",
                ".sym_ewram",
            )],
            spell: 0x0200_0000,
            pieces: Pieces::Sections {
                object: object.into(),
            },
        };
        let output = relist(&input).unwrap();
        let piece = &output.pieces[&base];
        assert!(
            piece.contains(
                "\t.4byte Data_02000024\n.L_02008010:\n\t.4byte gMapBuffer + 0x4\n.L_02008014:\n\t.4byte gCommonVar\n"
            ),
            "{piece}"
        );
        assert_eq!(
            scaffold::render(&output.space[0]),
            "\t.section .bss,\"aw\",%nobits\n\t.space 0x0000000c\n\t.global Data_02000024\nData_02000024:\n"
        );
    }

    #[test]
    fn an_overlay_listing_becomes_one_section_per_piece_with_its_data_words_named() {
        let base = 0x0200_8000;
        let mut bytes = halves(&[
            0x4800, // ldr r0, [pc, #0]: the word at 0x04
            0x4770, // bx lr
        ]);
        bytes.extend(0x0200_0010u32.to_le_bytes()); // 0x04: the main image's RAM
        bytes.extend(halves(&[0x2001, 0x4770])); // 0x08: reached only from the table
        bytes.extend(0x0200_8009u32.to_le_bytes()); // 0x0c: the table
        bytes.extend(0x0200_800bu32.to_le_bytes()); // a place inside that function
        bytes.extend(0x0800_0001u32.to_le_bytes()); // the main image's function, by chance
        bytes.extend(0x0800_0004u32.to_le_bytes()); // inside it: no name reaches
        bytes.extend(0x0200_0010u32.to_le_bytes()); // packed data, in RAM by chance
        bytes.extend(0x1234_5678u32.to_le_bytes());
        let object = "resource_x_overlay";
        let input = Input {
            image: Image {
                bytes: &bytes,
                base,
            },
            regions: vec![
                region(
                    base,
                    base + 0x0c,
                    RegionKind::Listing { regenerate: true },
                    object,
                    ".text",
                ),
                region(
                    base + 0x0c,
                    base + 0x24,
                    RegionKind::Words,
                    object,
                    ".rodata",
                ),
            ],
            names: vec![
                Name {
                    name: "gMainBuffer".into(),
                    address: 0x0200_0008,
                    thumb: false,
                    absolute: true,
                },
                Name {
                    name: "Main_Function".into(),
                    address: 0x0800_0000,
                    thumb: true,
                    absolute: true,
                },
            ],
            external: BTreeSet::new(),
            incbin: Vec::new(),
            space: Vec::new(),
            rom_end: 0x0900_0000,
            foreign: vec![
                region(
                    0x0200_0000,
                    0x0204_0000,
                    RegionKind::Other,
                    "sym_ewram",
                    ".sym_ewram",
                ),
                region(0x0800_0000, 0x0800_0100, RegionKind::Other, "main", ".text"),
            ],
            spell: 0x0200_0000,
            pieces: Pieces::Sections {
                object: object.into(),
            },
        };
        let output = relist(&input).unwrap();
        assert_eq!(
            output.pieces.keys().copied().collect::<Vec<_>>(),
            vec![base, base + 8, base + 0x0c]
        );
        assert_eq!(output.words, BTreeSet::from([base + 0x0c]));
        let first = &output.pieces[&base];
        assert!(first.contains("\t.thumb_func\nFunc_02000000:\n"), "{first}");
        assert!(first.contains("\t.4byte gMainBuffer + 0x8\n"), "{first}");
        let second = &output.pieces[&(base + 8)];
        assert!(
            second.contains("\t.thumb_func\nFunc_02000008:\n"),
            "{second}"
        );
        assert!(second.contains("Data_0200000a:\n\tbx lr\n"), "{second}");
        let table = &output.pieces[&(base + 0x0c)];
        assert!(
            table.contains(
                "\t.4byte Func_02000008\n\t.4byte Data_0200000a + 0x1\n\t.4byte 0x08000001\n\t.4byte 0x08000004\n\t.4byte 0x02000010\n\t.4byte 0x12345678\n"
            ),
            "{table}"
        );
        assert_eq!(
            output.notes,
            vec![
                "02008014: data word 08000001 in nothing placed stays a number".to_string(),
                "02008018: data word 08000004 in nothing placed stays a number".to_string(),
                "0200801c: data word 02000010 in nothing placed stays a number".to_string(),
            ]
        );
        let listing = |section: &str| Entry::Listing {
            object: object.into(),
            section: section.into(),
        };
        assert_eq!(
            output.script,
            vec![(
                vec![listing(".text"), listing(".rodata")],
                vec![
                    listing(".text.x02008000"),
                    listing(".text.x02008008"),
                    listing(".rodata.x0200800c"),
                ],
            )]
        );
    }
}
