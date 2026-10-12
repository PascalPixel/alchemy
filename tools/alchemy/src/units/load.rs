//! Reads one build's linked images (the main ELF and each overlay ELF with
//! their maps) and the objects the maps name, into the boundary model.
//! Nothing here reads a ROM: the built images are our own output.

use super::model::{Data, Function, Image, Lang, Pad};
use crate::compiler::routing::{family_for_source, CompilerFamily, CompilerTarget};
use crate::elf::Elf;
use crate::ld_map::{self, Map, Placed};
use object::{Object, ObjectSection, ObjectSymbol, RelocationFlags, RelocationTarget, SymbolFlags};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fs;
use std::path::{Path, PathBuf};

/// Functions shorter than this say nothing by appearing twice.
pub const MIN_SHAPE: u32 = 128;

pub struct Loaded {
    pub image: Image,
    pub elf: Elf,
    /// Placed input sections of the image in load order: (object, section, run address, size).
    pub pieces: Vec<Piece>,
}

#[derive(Clone, Debug)]
pub struct Piece {
    pub object: String,
    pub path: String,
    pub section: String,
    pub address: u32,
    pub size: u32,
    pub region: usize,
}

#[derive(Clone, Debug)]
struct Sym {
    name: String,
    offset: u32,
    size: u32,
    kind: u8,
    local: bool,
}

#[derive(Clone, Debug)]
struct Reloc {
    offset: u32,
    kind: u32,
    local: bool,
}

#[derive(Clone, Debug, Default)]
struct Section {
    name: String,
    size: u32,
    symbols: Vec<Sym>,
    mapping: Vec<(u32, char)>,
    relocs: Vec<Reloc>,
}

fn read_object(path: &Path) -> Option<Vec<Section>> {
    let bytes = fs::read(path).ok()?;
    let file = object::File::parse(&*bytes).ok()?;
    let mut sections: Vec<Section> = Vec::new();
    let mut by_index = BTreeMap::new();
    for section in file.sections() {
        let name = section.name().unwrap_or("").to_string();
        by_index.insert(section.index().0, sections.len());
        let mut relocs = Vec::new();
        for (offset, relocation) in section.relocations() {
            let RelocationFlags::Elf { r_type } = relocation.flags() else {
                continue;
            };
            let local = match relocation.target() {
                RelocationTarget::Symbol(index) => file.symbol_by_index(index).map_or(true, |symbol| symbol.is_local()),
                _ => true,
            };
            relocs.push(Reloc { offset: offset as u32, kind: r_type, local });
        }
        sections.push(Section { name, size: section.size() as u32, relocs, ..Section::default() });
    }
    for symbol in file.symbols() {
        let Some(index) = symbol.section_index() else {
            continue;
        };
        let Some(&at) = by_index.get(&index.0) else {
            continue;
        };
        let name = symbol.name().unwrap_or("").to_string();
        let SymbolFlags::Elf { st_info, .. } = symbol.flags() else {
            continue;
        };
        let offset = symbol.address() as u32;
        if let Some(tag) = name.strip_prefix('$') {
            if let Some(c) = tag.chars().next() {
                sections[at].mapping.push((offset, c));
            }
            continue;
        }
        if name.is_empty() || name.starts_with(".L") || name.starts_with('.') {
            continue;
        }
        sections[at].symbols.push(Sym { name, offset: offset & !1, size: symbol.size() as u32, kind: st_info & 0xf, local: symbol.is_local() });
    }
    for s in &mut sections {
        s.mapping.sort();
        s.symbols.sort_by_key(|s| s.offset);
    }
    Some(sections)
}

fn is_function_kind(kind: u8) -> bool {
    matches!(kind, 2 | 13 | 15)
}

fn sign_extend(value: u32, bits: u32) -> i64 {
    let shift = 32 - bits;
    i64::from(((value << shift) as i32) >> shift)
}

fn half(elf: &Elf, address: u32) -> Option<u16> {
    let (section, offset) = elf.section_at(address)?;
    let b = section.bytes.get(offset..offset + 2)?;
    Some(u16::from_le_bytes([b[0], b[1]]))
}

fn word(elf: &Elf, address: u32) -> Option<u32> {
    let (section, offset) = elf.section_at(address)?;
    let b = section.bytes.get(offset..offset + 4)?;
    Some(u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
}

fn bytes(elf: &Elf, start: u32, end: u32) -> Vec<u8> {
    (start..end).filter_map(|a| elf.section_at(a).and_then(|(s, o)| s.bytes.get(o).copied())).collect()
}

/// The target of a Thumb BL pair at `site`.
pub fn thumb_call(first: u16, second: u16, site: u32) -> Option<u32> {
    if first & 0xf800 != 0xf000 || second & 0xf800 != 0xf800 {
        return None;
    }
    let offset = sign_extend((u32::from(first & 0x7ff) << 12) | (u32::from(second & 0x7ff) << 1), 23);
    Some((i64::from(site) + 4 + offset) as u32)
}

/// The target of a Thumb B (11-bit) or conditional B at `site`.
pub fn thumb_branch(ins: u16, site: u32) -> Option<u32> {
    let offset = if ins & 0xf800 == 0xe000 {
        sign_extend(u32::from(ins & 0x7ff) << 1, 12)
    } else if ins & 0xf000 == 0xd000 && ins & 0x0f00 < 0x0e00 {
        sign_extend(u32::from(ins & 0xff) << 1, 9)
    } else {
        return None;
    };
    Some((i64::from(site) + 4 + offset) as u32)
}

pub fn arm_branch(ins: u32, site: u32) -> Option<u32> {
    (ins & 0x0e00_0000 == 0x0a00_0000).then(|| (i64::from(site) + 8 + sign_extend((ins & 0x00ff_ffff) << 2, 26)) as u32)
}

/// An 8-byte far-call or overlay veneer: `ldr rN, [pc, #0]; bx rN; .word target`.
pub fn veneer_target(code: &[u8]) -> Option<u32> {
    if code.len() != 8 {
        return None;
    }
    let first = u16::from_le_bytes([code[0], code[1]]);
    let second = u16::from_le_bytes([code[2], code[3]]);
    let register = (first >> 8) & 7;
    (first & 0xf8ff == 0x4800 && second == 0x4700 | (register << 3))
        .then(|| u32::from_le_bytes([code[4], code[5], code[6], code[7]]))
}

/// A run of whole veneers, as an overlay's entry table assembles into one block.
pub fn is_veneer_run(code: &[u8]) -> bool {
    !code.is_empty() && code.len().is_multiple_of(8) && code.chunks(8).all(|c| veneer_target(c).is_some())
}

fn relative(path: &str) -> String {
    path.rsplit_once("/obj/").map_or_else(|| path.rsplit('/').next().unwrap_or(path).to_string(), |(_, tail)| tail.to_string())
}

struct Origin {
    lang: Lang,
    source: Option<PathBuf>,
    arm: bool,
}

fn origin(root: &Path, compiler: CompilerTarget, path: &str) -> Origin {
    if path.contains(".a(") {
        return Origin { lang: Lang::Library, source: None, arm: false };
    }
    let rel = relative(path);
    if !rel.starts_with("games/") {
        return Origin { lang: Lang::Listing, source: None, arm: false };
    }
    let stem = rel.trim_end_matches(".o");
    for ext in crate::build_rom::source_extensions(stem) {
        let source = root.join(format!("{stem}.{ext}"));
        if source.exists() {
            let relative_source = format!("{stem}.{ext}");
            let family = family_for_source(compiler, &relative_source);
            let lang = match family {
                CompilerFamily::Agbcc | CompilerFamily::AgbccFlash => Lang::Library,
                _ if ext.eq_ignore_ascii_case("s") => Lang::Assembly,
                _ => Lang::C,
            };
            return Origin { lang, source: Some(source), arm: family.is_arm() };
        }
    }
    Origin { lang: Lang::Listing, source: None, arm: false }
}

/// Old agbcc objects have no $a/$t mapping symbols. Their compiler route
/// supplies the fallback; explicit mappings still describe mixed-mode code.
fn mode_arm(section: Option<&Section>, offset: u32, fallback: bool) -> bool {
    section
        .and_then(|s| s.mapping.iter().rev().find(|(o, c)| *o <= offset && matches!(c, 'a' | 't')))
        .map_or(fallback, |(_, c)| *c == 'a')
}

/// Names of `static inline` functions a C file defines itself.
pub fn inline_helpers(text: &str) -> BTreeSet<String> {
    let pattern = regex::Regex::new(r"(?m)^\s*static\s+(?:__inline__|__inline|inline)\b[^;{()]*?\b([A-Za-z_]\w*)\s*\([^;{]*\)\s*\{").expect("pattern");
    pattern.captures_iter(text).map(|c| c[1].to_string()).collect()
}

struct Symbols {
    by_address: BTreeMap<u32, Vec<String>>,
}

impl Symbols {
    fn new(elf: &Elf) -> Symbols {
        let mut by_address: BTreeMap<u32, Vec<String>> = BTreeMap::new();
        for s in &elf.symbols {
            if s.section == 0 || s.name.is_empty() || s.name.starts_with('$') || s.name.starts_with('.') || !matches!(s.kind, 0 | 1 | 2 | 13 | 15) {
                continue;
            }
            by_address.entry(s.value & !1).or_default().push(s.name.clone());
        }
        Symbols { by_address }
    }

    fn name_at_or_below(&self, address: u32) -> Option<&str> {
        let (_, names) = self.by_address.range(..=address).next_back()?;
        let mut sorted: Vec<&String> = names.iter().collect();
        sorted.sort_by_key(|n| (ld_map::is_placeholder(n), n.len()));
        sorted.first().map(|n| n.as_str())
    }
}

fn entity(address: u32, symbols: &Symbols, main: Option<&Symbols>) -> String {
    let class = match address >> 24 {
        2 | 3 => "ram",
        4 => return format!("io:{address:08x}"),
        5..=7 => return format!("vram:{address:08x}"),
        8 | 9 => "rom",
        _ => return format!("value:{address:08x}"),
    };
    let name = symbols
        .name_at_or_below(address)
        .or_else(|| main.and_then(|m| m.name_at_or_below(address)))
        .map_or_else(|| format!("{address:08x}"), str::to_string);
    format!("{class}:{name}")
}

pub struct Build {
    pub root: PathBuf,
    pub dir: PathBuf,
    pub name: String,
    pub compiler: CompilerTarget,
}

impl Build {
    pub fn images(&self) -> Result<Vec<PathBuf>, String> {
        let main = self.dir.join(format!("{}.elf", self.name));
        if !main.exists() {
            return Err(format!("{} is missing: build {} first", main.display(), self.name));
        }
        let mut overlays: Vec<PathBuf> = fs::read_dir(self.dir.join("overlays"))
            .map(|entries| entries.filter_map(|e| e.ok().map(|e| e.path())).filter(|p| p.extension().is_some_and(|x| x == "elf")).collect())
            .unwrap_or_default();
        overlays.sort();
        Ok(std::iter::once(main).chain(overlays).collect())
    }
}

pub fn load(build: &Build) -> Result<Vec<Loaded>, String> {
    let mut objects: HashMap<String, Option<Vec<Section>>> = HashMap::new();
    let mut inline: HashMap<String, BTreeSet<String>> = HashMap::new();
    let mut loaded = Vec::new();
    let mut external_targets: BTreeSet<u32> = BTreeSet::new();
    let mut main_symbols: Option<Symbols> = None;
    for elf_path in build.images()? {
        let data = fs::read(&elf_path).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let map_path = elf_path.with_extension("map");
        let map = Map::parse(&fs::read_to_string(&map_path).map_err(|e| format!("{}: {e}", map_path.display()))?);
        let name = elf_path.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
        let symbols = Symbols::new(&elf);
        let is_main = main_symbols.is_none();
        let (image, pieces, external) = load_image(build, &name, &elf, &map, &symbols, main_symbols.as_ref(), &mut objects, &mut inline);
        if !is_main {
            external_targets.extend(external);
        }
        loaded.push(Loaded { image, elf, pieces });
        if is_main {
            main_symbols = Some(symbols);
        }
    }
    copies(&mut loaded);
    if let Some(main) = loaded.first_mut() {
        for f in &mut main.image.functions {
            if external_targets.contains(&f.start) {
                f.external = true;
            }
        }
    }
    Ok(loaded)
}

#[allow(clippy::too_many_arguments)]
fn load_image(
    build: &Build,
    name: &str,
    elf: &Elf,
    map: &Map,
    symbols: &Symbols,
    main: Option<&Symbols>,
    objects: &mut HashMap<String, Option<Vec<Section>>>,
    inline: &mut HashMap<String, BTreeSet<String>>,
) -> (Image, Vec<Piece>, BTreeSet<u32>) {
    let mut placed: Vec<&Placed> = map.placed.iter().filter(|p| p.loaded && p.size > 0 && !p.is_fill()).collect();
    placed.sort_by_key(|p| (p.load, p.address));
    let mut pieces = Vec::new();
    let mut region = 0;
    let mut previous: Option<&Placed> = None;
    for p in placed {
        if let Some(q) = previous {
            let delta = |x: &Placed| i64::from(x.address) - i64::from(x.load);
            if delta(q) != delta(p) || p.load >= q.load + q.size + 32 {
                region += 1;
            }
        }
        previous = Some(p);
        pieces.push(Piece { object: relative(&p.path), path: p.path.clone(), section: p.name.clone(), address: p.address, size: p.size, region });
    }

    let mut image = Image { name: name.to_string(), ..Image::default() };
    // Per piece: its object's section, and the items it holds.
    struct Located {
        piece: usize,
        section: Option<Section>,
    }
    let mut located = Vec::new();
    let mut origins: HashMap<String, (Lang, Option<PathBuf>, bool)> = HashMap::new();
    for (k, piece) in pieces.iter().enumerate() {
        let sections = objects.entry(piece.path.clone()).or_insert_with(|| read_object(Path::new(&piece.path)));
        let section = sections.as_ref().and_then(|s| {
            let mut candidates = s.iter().filter(|x| x.name == piece.section);
            let first = candidates.clone().find(|x| x.size == piece.size);
            first.or_else(|| candidates.next()).cloned()
        });
        let (lang, source, source_arm) = origins
            .entry(piece.object.clone())
            .or_insert_with(|| {
                let o = origin(&build.root, build.compiler, &piece.path);
                (o.lang, o.source, o.arm)
            })
            .clone();
        if lang == Lang::C {
            if let Some(source) = &source {
                let helpers = inline.entry(piece.object.clone()).or_insert_with(|| inline_helpers(&fs::read_to_string(source).unwrap_or_default()));
                if !helpers.is_empty() {
                    image.inline_helpers.insert(piece.object.clone(), helpers.clone());
                }
            }
        }
        let code_mapping = section.as_ref().is_some_and(|s| s.mapping.iter().any(|(_, c)| *c == 't' || *c == 'a'));
        let mut starts: Vec<(u32, Option<Sym>, bool)> = Vec::new(); // offset, symbol, is function
        if let Some(s) = &section {
            let mut functions: BTreeMap<u32, Vec<&Sym>> = BTreeMap::new();
            for sym in &s.symbols {
                if is_function_kind(sym.kind) {
                    functions.entry(sym.offset).or_default().push(sym);
                }
            }
            if functions.is_empty() && code_mapping && piece.section.starts_with(".text") {
                for sym in &s.symbols {
                    if !sym.local && s.mapping.iter().any(|(o, c)| *o == sym.offset && (*c == 't' || *c == 'a')) {
                        functions.entry(sym.offset).or_default().push(sym);
                    }
                }
                if functions.is_empty() && s.mapping.first().is_some_and(|(o, c)| *o == 0 && *c != 'd') {
                    functions.entry(0).or_default();
                }
            }
            for (offset, names) in functions {
                let mut names = names;
                names.sort_by_key(|n| (ld_map::is_placeholder(&n.name), n.local));
                starts.push((offset, names.first().map(|s| (*s).clone()), true));
            }
            for sym in &s.symbols {
                if sym.kind == 1 && !starts.iter().any(|(o, _, _)| *o == sym.offset) {
                    starts.push((sym.offset, Some(sym.clone()), false));
                }
            }
        } else {
            // An archive member the object reader cannot open: the image's own symbols.
            for s in &elf.symbols {
                let value = s.value & !1;
                if s.section != 0 && is_function_kind(s.kind) && value >= piece.address && value < piece.address + piece.size && !s.name.starts_with('$') && !s.name.starts_with('.') {
                    let sym = Sym { name: s.name.clone(), offset: value - piece.address, size: s.size, kind: s.kind, local: false };
                    starts.push((sym.offset, Some(sym), true));
                }
            }
        }
        starts.sort_by_key(|(o, _, f)| (*o, !*f));
        starts.dedup_by_key(|(o, _, _)| *o);
        if starts.first().is_none_or(|(o, _, _)| *o > 0) {
            let leading_code = piece.section.starts_with(".text")
                && section.as_ref().is_none_or(|s| s.mapping.first().is_some_and(|(o, c)| *o == 0 && *c != 'd'));
            starts.insert(0, (0, None, leading_code));
        }
        for (j, (offset, sym, is_function)) in starts.iter().enumerate() {
            let next = starts.get(j + 1).map_or(piece.size, |(o, _, _)| *o);
            if next <= *offset {
                continue;
            }
            let start = piece.address + offset;
            if *is_function {
                let size = sym.as_ref().map_or(0, |s| s.size);
                let end = if size > 0 && offset + size <= next { start + size } else { piece.address + next };
                let mode_arm = mode_arm(section.as_ref(), *offset, source_arm);
                let code = bytes(elf, start, end);
                let lang = if is_veneer_run(&code) { Lang::Veneer } else { lang };
                let name = sym.as_ref().map_or_else(|| format!("sub_{start:08x}"), |s| s.name.clone());
                image.functions.push(Function {
                    name,
                    start,
                    end,
                    object: piece.object.clone(),
                    lang,
                    arm: mode_arm,
                    is_static: sym.as_ref().is_some_and(|s| s.local),
                    region: piece.region,
                    refs: BTreeSet::new(),
                    callees: BTreeSet::new(),
                    branches: BTreeSet::new(),
                    data_refs: BTreeSet::new(),
            static_data: BTreeSet::new(),
                    external: false,
                    pad: Vec::new(),
            shape: 0,
            copies: 0,
            pair_copies: 0,
                });
            } else {
                let block = bytes(elf, start, piece.address + next);
                if Pad::of(&block) != Pad::Other && piece.section == ".padding" {
                    continue;
                }
                let names = sym.iter().map(|s| s.name.clone()).collect();
                image.data.push(Data { start, end: piece.address + next, object: piece.object.clone(), region: piece.region, names, targets: BTreeSet::new() });
            }
        }
        located.push(Located { piece: k, section });
    }
    image.functions.sort_by_key(|f| f.start);
    image.data.sort_by_key(|d| d.start);
    // Name data blocks from the image's own symbols where the object gave none.
    for d in &mut image.data {
        if d.names.is_empty() {
            if let Some(names) = symbols.by_address.get(&d.start) {
                d.names = names.clone();
            }
        }
    }
    // Pads between functions, where no data lies between.
    for i in 0..image.functions.len().saturating_sub(1) {
        let (end, next) = (image.functions[i].end, image.functions[i + 1].start);
        if next > end && !image.data.iter().any(|d| d.start >= end && d.end <= next) {
            image.functions[i].pad = bytes(elf, end, next);
        }
    }
    // Displaced data: an object whose data lies anywhere but right after its text.
    let mut text_end: BTreeMap<&str, u32> = BTreeMap::new();
    for f in &image.functions {
        let e = text_end.entry(&f.object).or_default();
        *e = (*e).max(f.end);
    }
    for d in &image.data {
        if let Some(&end) = text_end.get(d.object.as_str()) {
            if d.start < end || d.start > ((end + 3) & !3) + 16 && !image.data.iter().any(|e| e.object == d.object && e.end <= d.start && e.start <= ((end + 3) & !3) + 16) {
                image.displaced_data.insert(d.object.clone());
            }
        }
    }

    let function_at = |address: u32, functions: &[Function]| -> Option<usize> {
        let at = functions.partition_point(|f| f.start <= address);
        at.checked_sub(1).filter(|&i| address < functions[i].end.max(functions.get(i + 1).map_or(functions[i].end, |n| n.start)))
    };
    let data_at = |address: u32, data: &[Data]| -> Option<usize> {
        let at = data.partition_point(|d| d.start <= address);
        at.checked_sub(1).filter(|&i| address < data[i].end)
    };
    let starts: BTreeMap<u32, usize> = image.functions.iter().enumerate().map(|(i, f)| (f.start, i)).collect();
    // Veneers lead to their targets.
    let mut veneer: BTreeMap<usize, u32> = BTreeMap::new();
    for (i, f) in image.functions.iter().enumerate() {
        if f.lang == Lang::Veneer {
            if let Some(t) = veneer_target(&bytes(elf, f.start, f.end)) {
                veneer.insert(i, t & !1);
            }
        }
    }
    let mut masked: BTreeSet<u32> = BTreeSet::new();
    let mut external = BTreeSet::new();
    for l in &located {
        let piece = &pieces[l.piece];
        let Some(section) = &l.section else {
            continue;
        };
        for r in &section.relocs {
            let site = piece.address + r.offset;
            let width = if matches!(r.kind, 102 | 103) { 2 } else { 4 };
            masked.extend(site..site + width);
            let (call, branch, target) = match r.kind {
                2 => (false, false, word(elf, site)),
                10 => (true, false, half(elf, site).zip(half(elf, site + 2)).and_then(|(a, b)| thumb_call(a, b, site))),
                102 | 103 => (false, true, half(elf, site).and_then(|h| thumb_branch(h, site))),
                1 | 28 => (true, false, word(elf, site).and_then(|w| arm_branch(w, site))),
                29 => (false, true, word(elf, site).and_then(|w| arm_branch(w, site))),
                _ => continue,
            };
            let Some(target) = target else {
                continue;
            };
            let target = if (target >> 24) == 8 || (target >> 24) == 2 || (target >> 24) == 3 {
                if starts.contains_key(&(target & !1)) { target & !1 } else { target }
            } else {
                target
            };
            let from_function = function_at(site, &image.functions).filter(|&i| site < image.functions[i].end || image.functions[i].lang != Lang::C);
            let from_data = if from_function.is_none() { data_at(site, &image.data) } else { None };
            let to_function = starts.get(&target).copied().or_else(|| {
                if branch || call {
                    function_at(target, &image.functions).filter(|&k| target < image.functions[k].end)
                } else {
                    None
                }
            });
            match (from_function, from_data, to_function) {
                (Some(f), _, Some(t)) => {
                    let resolved = veneer.get(&t).and_then(|v| starts.get(v)).copied();
                    if let Some(v) = veneer.get(&t) {
                        if !starts.contains_key(v) {
                            image.functions[f].refs.insert(entity(*v, symbols, main));
                        }
                    }
                    if branch && t != f {
                        image.functions[f].branches.insert(t);
                    } else if t != f {
                        image.functions[f].callees.insert(t);
                        if let Some(real) = resolved {
                            if real != f {
                                image.functions[f].callees.insert(real);
                            }
                        }
                    }
                }
                (Some(f), _, None) => {
                    if let Some(d) = data_at(target, &image.data) {
                        let name = image.data[d].names.first().cloned().unwrap_or_else(|| format!("{:08x}", image.data[d].start));
                        image.functions[f].refs.insert(format!("data:{name}"));
                        image.functions[f].data_refs.insert(d);
                        if r.local {
                            image.functions[f].static_data.insert(d);
                        }
                    } else {
                        if !starts.is_empty() && (target >> 24) == 8 && main.is_some() {
                            external.insert(target & !1);
                        }
                        image.functions[f].refs.insert(entity(target, symbols, main));
                    }
                }
                (None, Some(d), Some(t)) => {
                    image.data[d].targets.insert(t);
                }
                _ => {}
            }
        }
    }
    // Hardware registers loaded from literal pools.
    for f in &mut image.functions {
        if f.arm || f.lang == Lang::Veneer {
            continue;
        }
        let code = bytes(elf, f.start, f.end);
        for ins in crate::similar::sweep(&code, f.start, f.start, f.end) {
            if let crate::decode::Kind::LdrPool { word, .. } = ins.kind {
                if (0x0400_0000..0x0400_0800).contains(&word) {
                    f.refs.insert(format!("io:{word:08x}"));
                }
            }
        }
    }
    // The shape of each function: its bytes with every relocated place blanked.
    for f in &mut image.functions {
        if f.lang == Lang::Veneer {
            continue;
        }
        let mut code = bytes(elf, f.start, f.end);
        for (k, b) in code.iter_mut().enumerate() {
            if masked.contains(&(f.start + k as u32)) {
                *b = 0;
            }
        }
        let mut hasher = std::collections::hash_map::DefaultHasher::new();
        std::hash::Hash::hash(&code, &mut hasher);
        f.shape = std::hash::Hasher::finish(&hasher);
    }
    (image, pieces, external)
}

/// How often each function's shape, and each adjacent pair of shapes, recurs
/// in the build's other images: code linked into several overlays is a file
/// of its own.
fn copies(loaded: &mut [Loaded]) {
    let mut seen: HashMap<u64, Vec<(usize, usize)>> = HashMap::new();
    for (m, l) in loaded.iter().enumerate() {
        for (i, f) in l.image.functions.iter().enumerate() {
            if f.shape != 0 {
                seen.entry(f.shape).or_default().push((m, i));
            }
        }
    }
    let shapes: Vec<Vec<u64>> = loaded.iter().map(|l| l.image.functions.iter().map(|f| f.shape).collect()).collect();
    for (m, l) in loaded.iter_mut().enumerate() {
        let n = l.image.functions.len();
        for i in 0..n {
            let f = &l.image.functions[i];
            let Some(places) = seen.get(&f.shape).filter(|_| f.shape != 0) else {
                continue;
            };
            let others: BTreeSet<usize> = places.iter().filter(|(o, _)| *o != m).map(|(o, _)| *o).collect();
            let copies = if f.end - f.start >= MIN_SHAPE { others.len() } else { 0 };
            let next = shapes[m].get(i + 1).copied().unwrap_or(0);
            let pairs: BTreeSet<usize> = places
                .iter()
                .filter(|(o, x)| *o != m && next != 0 && shapes[*o].get(x + 1) == Some(&next))
                .map(|(o, _)| *o)
                .collect();
            let f = &mut l.image.functions[i];
            f.copies = copies;
            f.pair_copies = pairs.len();
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn arm_c_without_mapping_symbols_uses_its_compiler_route() {
        let root = tempfile::tempdir().unwrap();
        let source = root.path().join("games/common/src/window/metatile.c");
        fs::create_dir_all(source.parent().unwrap()).unwrap();
        fs::write(&source, "void Tile_BuildMetatiles(void) {}\n").unwrap();
        let o = origin(root.path(), CompilerTarget::Tbs, "/build/obj/games/common/src/window/metatile.o");
        assert_eq!(o.lang, Lang::C);
        assert!(mode_arm(Some(&Section::default()), 0, o.arm));
        assert!(!mode_arm(None, 0, false));

        let mixed = Section { mapping: vec![(0, 't'), (8, 'a'), (16, 'd')], ..Section::default() };
        assert!(!mode_arm(Some(&mixed), 0, o.arm));
        assert!(mode_arm(Some(&mixed), 8, false));
        assert!(mode_arm(Some(&mixed), 16, false));
    }

    #[test]
    fn branches_and_calls_decode_to_their_targets() {
        // bl from 0x08000100 to 0x08000200: offset 0xfc.
        let offset = 0x200 - 0x104;
        let first = 0xf000 | ((offset >> 12) & 0x7ff) as u16;
        let second = 0xf800 | ((offset >> 1) & 0x7ff) as u16;
        assert_eq!(thumb_call(first, second, 0x0800_0100), Some(0x0800_0200));
        // b backwards by 4 from 0x100: imm11 = -4 >> 1.
        assert_eq!(thumb_branch(0xe7fe, 0x100), Some(0x100));
        assert_eq!(thumb_branch(0xd0fe, 0x100), Some(0x100));
        assert_eq!(thumb_branch(0xdf00, 0x100), None);
        assert_eq!(arm_branch(0xeaff_fffe, 0x100), Some(0x100));
    }

    #[test]
    fn a_veneer_is_a_pc_load_and_a_register_branch() {
        assert_eq!(veneer_target(&[0x00, 0x4c, 0x20, 0x47, 0x45, 0x23, 0x01, 0x08]), Some(0x0801_2345));
        assert_eq!(veneer_target(&[0x00, 0x4c, 0x18, 0x47, 0x45, 0x23, 0x01, 0x08]), None);
        assert_eq!(veneer_target(&[0x00, 0x4c, 0x20, 0x47]), None);
        assert!(is_veneer_run(&[0x00, 0x4c, 0x20, 0x47, 1, 0, 0, 8, 0x00, 0x4b, 0x18, 0x47, 2, 0, 0, 8]));
        assert!(!is_veneer_run(&[0x00, 0x4c, 0x20, 0x47, 1, 0, 0, 8, 0x70, 0x47, 0, 0, 0, 0, 0, 0]));
    }

    #[test]
    fn inline_helpers_are_the_file_s_own_static_inline_definitions() {
        let text = "static inline void Set(int a)\n{\n}\nstatic __inline__ u16 *Slot(void) {\n}\nstatic inline int Proto(void);\nstatic void Plain(void)\n{\n}\n";
        let names: Vec<String> = inline_helpers(text).into_iter().collect();
        assert_eq!(names, ["Set", "Slot"]);
    }

    #[test]
    fn objects_are_named_from_the_build_s_obj_directory() {
        assert_eq!(relative("/x/out/tbs-en/obj/games/a/src/b.o"), "games/a/src/b.o");
        assert_eq!(relative("/x/out/tbs-en/overlays/resource_36f_overlay.o"), "resource_36f_overlay.o");
    }
}
