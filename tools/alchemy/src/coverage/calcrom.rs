use super::progress::GameDone;
use crate::targets::DecompTarget;
use object::{Object, ObjectSection, ObjectSymbol, SymbolKind};
use sha1::{Digest, Sha1};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

pub(crate) type Unit = (String, String, Option<String>);

pub(crate) const MAIN_IMAGE: &str = "main";

#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub(crate) struct Counted {
    pub done: GameDone,
    pub library: i64,
    pub steered: i64,
    pub uncredited: i64,
    pub unproven_asm: i64,
}
impl std::ops::AddAssign for Counted {
    fn add_assign(&mut self, other: Counted) {
        self.done += other.done;
        self.library += other.library;
        self.steered += other.steered;
        self.uncredited += other.uncredited;
        self.unproven_asm += other.unproven_asm;
    }
}

#[derive(Clone, Debug)]
pub(crate) struct Game {
    pub english: Measurement,
    pub editions: Vec<(&'static str, Counted)>,
    /// Each other edition's English-credited units that it does not link
    /// as credit, with their English bytes and whether the edition has
    /// them at all (false: absent, outside that edition's total).
    pub missing: Vec<(&'static str, Missing)>,
}
impl Game {
    pub fn combined(&self) -> Counted {
        let mut combined = Counted::default();
        for (_, edition) in &self.editions {
            combined += *edition;
        }
        combined
    }
}

pub(crate) fn share(english: &Measurement, linked: &BTreeSet<Unit>) -> Counted {
    let mut counted = Counted::default();
    for (unit, credit) in &english.credits {
        if linked.contains(unit) {
            counted += *credit;
        }
    }
    counted.done.executable = english.done.executable;
    counted
}

/// An English-credited function is absent from an edition when that
/// edition links the function's object yet none of its images has a symbol
/// of that name: the edition's own source leaves it out, and no scaffold
/// labels it. Absent code is outside that edition's total (C2).
fn absent(english: &Measurement, edition: &Measurement, unit: &Unit) -> bool {
    let (_, object, Some(function)) = unit else {
        return false;
    };
    !edition.credits.contains_key(unit)
        && edition.objects.contains(object)
        && !edition.symbols.contains(function)
        && english.credits.contains_key(unit)
}

fn share_edition(english: &Measurement, edition: &Measurement) -> Counted {
    let linked = edition.credits.iter()
        .filter(|(_, credit)| credit.done.bytes() > 0)
        .map(|(unit, _)| unit.clone()).collect();
    let mut counted = share(english, &linked);
    counted.done.data = edition.done.data;
    counted.done.data_total = edition.done.data_total;
    counted.done.executable -= english
        .credits
        .iter()
        .filter(|(unit, _)| absent(english, edition, unit))
        .map(|(_, credit)| credit.done.bytes())
        .sum::<i64>();
    counted.steered = english
        .credits
        .iter()
        .filter(|(unit, _)| {
            edition
                .credits
                .get(*unit)
                .is_some_and(|credit| credit.steered > 0)
        })
        .map(|(_, credit)| credit.done.game_c + credit.done.common_c)
        .sum();
    counted
}

fn unlinked(english: &Measurement, edition: &Measurement) -> Vec<(Unit, i64, bool)> {
    english
        .credits
        .iter()
        .filter(|(unit, credit)| !edition.credits.contains_key(*unit) && credit.done.bytes() > 0)
        .map(|(unit, credit)| (unit.clone(), credit.done.bytes(), !absent(english, edition, unit)))
        .collect()
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Measurement {
    pub done: GameDone,
    pub library: i64,
    pub raw: i64,
    pub listings: i64,
    pub other: Vec<(String, i64)>,
    pub steered: i64,
    pub uncredited: i64,
    pub credits: BTreeMap<Unit, Counted>,
    /// Every symbol name in the edition's images, and every object its
    /// linker maps place text from (empty sections included).
    pub symbols: BTreeSet<String>,
    pub objects: BTreeSet<String>,
    /// Each steered function with its bytes and the tags that steer it.
    pub steered_units: Vec<(Unit, i64, BTreeSet<String>)>,
    /// Every function's ARM instruction bytes, from the images' mapping
    /// symbols: a breakdown of this measurement that changes no total.
    pub arm: Vec<super::arm::ArmRow>,
    /// Data words in not-yet-C listings that are likely ARM code: listed
    /// apart, never in `arm`'s tally, any total or any credit.
    pub arm_likely: Vec<super::arm::ArmRow>,
    /// Data bytes placed from recon/, by image and object: what keeps
    /// DONE data short (C2).
    pub data_pending: Vec<(String, String, i64)>,
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Mark {
    pub steered: Steered,
    pub veneer: bool,
    pub assembly_proof: Option<String>,
    pub reasons: BTreeMap<String, BTreeSet<String>>,
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) enum Steered {
    #[default]
    None,
    Whole,
    Functions(std::collections::BTreeSet<String>),
}

fn source_mark(
    root: &Path,
    expansion: &crate::compiler::preprocess::Expansion<'_>,
    stem: &str,
) -> Result<Mark, String> {
    let Some((path, text)) = crate::build_rom::source_extensions(stem).iter().find_map(|extension| {
        let path = format!("{stem}.{extension}");
        std::fs::read_to_string(root.join(&path))
            .ok()
            .map(|text| (path, text))
    }) else {
        return Ok(Mark::default());
    };
    let mut reasons = BTreeMap::new();
    let steered = if path.ends_with(".c") {
        let expanded = expansion.fresh(&path)?;
        let analysis = crate::compiler::steering::analyze(&expanded, &path)?;
        reasons = analysis.reasons.clone();
        if analysis
            .whole_owners
            .contains(&crate::compiler::steering::owner(&path))
        {
            Steered::Whole
        } else if analysis.steered.is_empty() {
            Steered::None
        } else {
            Steered::Functions(analysis.steered)
        }
    } else if text.contains("FAKEMATCH") {
        Steered::Whole
    } else {
        Steered::None
    };
    if steered == Steered::Whole {
        reasons.insert(String::new(), BTreeSet::from([format!("{path} (whole file)")]));
    }
    Ok(Mark {
        steered,
        veneer: builds_veneers(&text),
        assembly_proof: assembly_proof(&text),
        reasons,
    })
}

/// The tags behind a steered function: its own reasons, with each steered
/// helper it uses replaced by that helper's tags.
fn steering_roots(
    reasons: &BTreeMap<String, BTreeSet<String>>,
    names: &BTreeSet<String>,
) -> BTreeSet<String> {
    let mut roots = reasons.get("").cloned().unwrap_or_default();
    let mut pending = names.iter().cloned().collect::<Vec<_>>();
    let mut seen = BTreeSet::new();
    while let Some(name) = pending.pop() {
        if !seen.insert(name.clone()) {
            continue;
        }
        for reason in reasons.get(&name).into_iter().flatten() {
            match reason.strip_prefix("via ") {
                Some(helper) => pending.push(helper.to_string()),
                None => {
                    roots.insert(reason.clone());
                }
            }
        }
    }
    roots
}

/// Assembly that builds its entries with a veneer macro: the 8-byte far-call
/// and IWRAM entry stubs counted as veneers.
fn builds_veneers(text: &str) -> bool {
    text.lines().any(|line| {
        let word = line.split_whitespace().next().unwrap_or("");
        word == "overlay_veneer" || word == "IwramEntry"
    })
}

/// C2 evidence is a reviewed source assertion, not something inferred from
/// a directory or a compiler-shape heuristic. Keep it in the leading assembly
/// comments so an instruction or a string containing "C2:" cannot grant credit.
/// This records the review; it does not mechanically prove its explanation.
fn assembly_proof(text: &str) -> Option<String> {
    text.lines()
        .map(str::trim)
        .take_while(|line| line.is_empty() || line.starts_with('@'))
        .filter_map(|line| line.strip_prefix("@ C2:"))
        .map(str::trim)
        .find(|proof| !proof.is_empty())
        .map(str::to_owned)
}

/// Check the placed object itself, not the spelling of a macro invocation.
/// Every entry must be an aligned Thumb literal load, bx of that register,
/// and its target word. A mixed object cannot grant veneer credit to its
/// ordinary code merely by invoking a veneer macro somewhere in the file.
fn veneer_bytes(bytes: &[u8], address: i64) -> bool {
    address & 3 == 0 && !bytes.is_empty() && bytes.len().is_multiple_of(8)
        && bytes.as_chunks::<8>().0.iter().all(|entry| {
            let load = u16::from_le_bytes([entry[0], entry[1]]);
            let branch = u16::from_le_bytes([entry[2], entry[3]]);
            load & 0xF8FF == 0x4800 && branch == (0x4700 | (((load >> 8) & 7) << 3))
        })
}

fn placed_veneers(placed: &Placed<'_>) -> Result<bool, String> {
    let bytes = std::fs::read(placed.object)
        .map_err(|error| format!("{}: veneer metadata: {error}", placed.object))?;
    let file = object::File::parse(bytes.as_slice()).map_err(|error| error.to_string())?;
    let section = file.section_by_name(placed.name)
        .ok_or_else(|| format!("{}: missing placed section {}", placed.object, placed.name))?;
    let data = section.data().map_err(|error| error.to_string())?;
    if data.len() as i64 != placed.size {
        return Err(format!("{}: map/object veneer extent differs", placed.object));
    }
    Ok(veneer_bytes(data, placed.address))
}

#[derive(Debug)]
struct FunctionSymbol<'a> {
    offset: i64,
    size: u64,
    name: &'a str,
    public: bool,
}

fn section_function_definitions<'a>(
    bytes: &'a [u8],
    section_name: &str,
    size: i64,
) -> Result<Vec<FunctionSymbol<'a>>, String> {
    let file = object::File::parse(bytes).map_err(|error| error.to_string())?;
    let section = file
        .section_by_name(section_name)
        .ok_or_else(|| format!("missing placed section {section_name}"))?;
    if section.size() != size as u64 {
        return Err(format!("{section_name}: map/object extent differs"));
    }
    let mut symbols = file.symbols().filter(|symbol| symbol.section_index() == Some(section.index()) && (
        symbol.kind() == SymbolKind::Text || matches!(symbol.flags(), object::SymbolFlags::Elf { st_info, .. } if st_info & 0xf == 13)
    ))
        .filter_map(|symbol| symbol.name().ok().map(|name| FunctionSymbol {
            offset: (symbol.address() & !1) as i64,
            size: symbol.size(),
            name,
            public: symbol.is_global(),
        }))
        .collect::<Vec<_>>();
    symbols.sort_by_key(|symbol| symbol.offset);
    Ok(symbols)
}

#[derive(Debug, PartialEq, Eq)]
struct CFunction {
    offset: i64,
    bytes: i64,
    name: String,
    names: BTreeSet<String>,
}

fn placed_c_functions(placed: &Placed<'_>) -> Result<Vec<CFunction>, String> {
    let bytes = std::fs::read(placed.object)
        .map_err(|error| format!("{}: function metadata: {error}", placed.object))?;
    c_functions_from_object(&bytes, placed.name, placed.size)
        .map_err(|error| format!("{}: {error}", placed.object))
}

fn c_functions_from_object(
    bytes: &[u8],
    section: &str,
    size: i64,
) -> Result<Vec<CFunction>, String> {
    let symbols = section_function_definitions(bytes, section, size)?;
    if symbols.first().map(|symbol| symbol.offset) != Some(0) {
        return Err(format!(
            "{section}: missing C function definition at section start"
        ));
    }
    let mut functions = Vec::with_capacity(symbols.len());
    let mut index = 0;
    while index < symbols.len() {
        let first = &symbols[index];
        let after = symbols[index..]
            .iter()
            .position(|symbol| symbol.offset != first.offset)
            .map_or(symbols.len(), |offset| index + offset);
        let group = &symbols[index..after];
        let end = symbols.get(after).map_or(size, |symbol| symbol.offset);
        let span = end - first.offset;
        if first.offset < 0
            || end > size
            || span <= 0
            || group
                .iter()
                .any(|symbol| symbol.name.is_empty() || symbol.size > span as u64)
        {
            return Err(format!(
                "{section}: ambiguous C function extent for {}",
                first.name
            ));
        }
        let definition = if group.len() == 1 {
            first
        } else {
            let public = group
                .iter()
                .filter(|symbol| symbol.public)
                .collect::<Vec<_>>();
            if first.size == 0
                || group.iter().any(|symbol| symbol.size != first.size)
                || public.len() != 1
            {
                return Err(format!(
                    "{section}: ambiguous C function extent for {}",
                    first.name
                ));
            }
            public[0]
        };
        functions.push(CFunction {
            offset: first.offset,
            bytes: span,
            name: definition.name.to_string(),
            names: group.iter().map(|symbol| symbol.name.to_string()).collect(),
        });
        index = after;
    }
    Ok(functions)
}

pub(crate) fn uncredited_spans(nm: &str) -> Vec<(i64, i64)> {
    let mut starts = BTreeMap::new();
    let mut ends = BTreeMap::new();
    for line in nm.lines() {
        let mut fields = line.split_whitespace();
        let (Some(address), Some(_), Some(name)) = (fields.next(), fields.next(), fields.next())
        else {
            continue;
        };
        let Ok(address) = i64::from_str_radix(address, 16) else {
            continue;
        };
        if let Some(key) = name.strip_prefix("AlchemyUncreditedEnd_") {
            ends.insert(key.to_string(), address);
        } else if let Some(key) = name.strip_prefix("AlchemyUncredited_") {
            starts.insert(key.to_string(), address);
        }
    }
    let mut spans = starts
        .iter()
        .filter_map(|(key, start)| ends.get(key).map(|end| (*start, (end - start).max(0))))
        .collect::<Vec<_>>();
    spans.sort();
    spans
}

/// A span inside credited source is padding: it leaves the credit and the
/// executable total, as the linker's own fill does (C2). A span anywhere
/// else, such as a raw listing, stays in the total as not-yet-C code.
fn discredit(measurement: &mut Measurement, placed: &[(i64, i64, Unit)], spans: &[(i64, i64)]) {
    for (start, bytes) in spans {
        for (address, size, unit) in placed {
            let overlap = (start + bytes).min(address + size) - start.max(address);
            if overlap <= 0 {
                continue;
            }
            let Some(credit) = measurement.credits.get_mut(unit) else {
                continue;
            };
            measurement.uncredited += overlap;
            measurement.done.executable -= overlap;
            credit.uncredited += overlap;
            let unproven = overlap.min(credit.unproven_asm);
            credit.unproven_asm -= unproven;
            measurement.raw -= unproven;
            let mut left = overlap;
            let done = &mut measurement.done;
            for (part, total) in [
                (&mut credit.done.game_asm, &mut done.game_asm),
                (&mut credit.done.common_asm, &mut done.common_asm),
                (&mut credit.done.game_c, &mut done.game_c),
                (&mut credit.done.common_c, &mut done.common_c),
            ] {
                let taken = left.min(*part);
                *part -= taken;
                *total -= taken;
                left -= taken;
            }
            let steered = overlap.min(credit.steered);
            credit.steered -= steered;
            measurement.steered -= steered;
        }
    }
}

fn is_data(name: &str) -> bool {
    name.starts_with(".rodata")
        || name == ".data"
        || name.starts_with(".data.")
        || name.starts_with(".unidentified")
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Origin {
    CommonC,
    CommonAsm,
    GameC,
    GameAsm,
    Library,
    Raw,
    Listing,
    Other,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Language {
    C,
    Assembly,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) struct Placed<'a> {
    pub name: &'a str,
    pub address: i64,
    pub size: i64,
    pub object: &'a str,
}

pub(crate) fn sections(map: &str) -> Vec<Placed<'_>> {
    sections_in_outputs(map)
        .into_iter()
        .map(|(_, placed)| placed)
        .collect()
}

/// Each placed input section with the output section that holds it: the
/// name heading its block of the map, if any.
pub(crate) fn sections_in_outputs(map: &str) -> Vec<(Option<&str>, Placed<'_>)> {
    let mut found = Vec::new();
    let mut discarded = false;
    let mut output = None;
    let mut lines = map.lines().peekable();
    while let Some(line) = lines.next() {
        match line {
            "Discarded input sections" => discarded = true,
            "Memory Configuration" | "Linker script and memory map" => discarded = false,
            _ => {}
        }
        if !line.is_empty() && !line.starts_with(char::is_whitespace) {
            output = line.split_whitespace().next();
        }
        let Some(rest) = line.strip_prefix(" .") else {
            continue;
        };
        let name_end = rest.find(char::is_whitespace).unwrap_or(rest.len());
        let name = &line[1..name_end + 2];
        let placed = if name_end == rest.len() {
            match lines.peek().and_then(|next| placement(next)) {
                Some(placed) => {
                    lines.next();
                    placed
                }
                None => continue,
            }
        } else {
            match placement(&rest[name_end..]) {
                Some(placed) => placed,
                None => continue,
            }
        };
        if !discarded {
            found.push((
                output,
                Placed {
                    name,
                    address: placed.0,
                    size: placed.1,
                    object: placed.2,
                },
            ));
        }
    }
    found
}

fn placement(text: &str) -> Option<(i64, i64, &str)> {
    let hex = |field: &str| {
        field
            .strip_prefix("0x")
            .and_then(|digits| i64::from_str_radix(digits, 16).ok())
    };
    let text = text.trim_start();
    let (address, rest) = text.split_once(char::is_whitespace)?;
    let address = hex(address)?;
    let rest = rest.trim_start();
    let (size, object) = rest.split_once(char::is_whitespace)?;
    let object = object.trim();
    (!object.is_empty()).then_some(())?;
    Some((address, hex(size)?, object))
}

pub(crate) fn relative_object<'a>(
    object: &'a str,
    output: &str,
    overlay: bool,
) -> Option<(&'a str, bool)> {
    let relative = |marker: &str| {
        let marker = format!("{output}/{marker}/");
        object
            .rfind(&marker)
            .map(|index| &object[index + marker.len()..])
    };
    match relative("obj") {
        Some(path) => Some((path, false)),
        None if overlay => {
            relative("overlays").map(|path| (path.strip_prefix("obj/").unwrap_or(path), true))
        }
        None => None,
    }
}

fn origin(
    object: &str,
    output: &str,
    overlay: bool,
    source: &dyn Fn(&str) -> Option<Language>,
) -> Result<Origin, String> {
    if object.contains("libgcc.a(") {
        return Ok(Origin::Library);
    }
    let Some((relative, listing_directory)) = relative_object(object, output, overlay) else {
        return Ok(Origin::Other);
    };
    if relative.starts_with("games/") {
        let stem = relative
            .strip_suffix(".o")
            .ok_or_else(|| format!("{relative}: not an object"))?;
        let language =
            source(stem).ok_or_else(|| format!("{relative}: no maintained source; rebuild"))?;
        let common = relative.starts_with("games/common/");
        return Ok(match (common, language) {
            (true, Language::C) => Origin::CommonC,
            (true, Language::Assembly) => Origin::CommonAsm,
            (false, Language::C) => Origin::GameC,
            (false, Language::Assembly) => Origin::GameAsm,
        });
    }
    if listing_directory && is_listing(relative) {
        return Ok(Origin::Listing);
    }
    if relative
        .strip_prefix("recon/")
        .and_then(|rest| rest.split_once('/'))
        .is_some_and(|(_, rest)| rest.starts_with("raw/"))
    {
        return Ok(Origin::Raw);
    }
    Ok(Origin::Other)
}

pub(crate) fn is_listing(name: &str) -> bool {
    name.strip_prefix("resource_")
        .and_then(|rest| rest.strip_suffix("_overlay.o"))
        .is_some_and(|id| !id.is_empty() && id.bytes().all(|byte| byte.is_ascii_hexdigit()))
}

pub(crate) fn is_text(placed: &Placed) -> bool {
    placed.size > 0 && !is_data(placed.name) && placed.name.contains("text")
}

fn unit_object(object: &str, output: &str, overlay: bool) -> Option<String> {
    match object.rfind("libgcc.a(") {
        Some(member) => Some(object[member..].to_owned()),
        None => relative_object(object, output, overlay).map(|(path, _)| path.to_owned()),
    }
}

fn tally(
    measurement: &mut Measurement,
    map: &str,
    output: &str,
    image: &str,
    source: &dyn Fn(&str) -> Option<Language>,
    mark: &dyn Fn(&str) -> Result<Mark, String>,
) -> Result<Vec<(i64, i64, Unit)>, String> {
    let overlay = image != MAIN_IMAGE;
    let mut credited = Vec::new();
    for placed in sections(map) {
        let Placed { size, object, .. } = placed;
        if placed.name.contains("text") && !is_data(placed.name) {
            if let Some(name) = unit_object(object, output, overlay) {
                measurement.objects.insert(name);
            }
        }
        if !is_text(&placed) {
            continue;
        }
        let marked = || {
            relative_object(object, output, overlay)
                .and_then(|(path, _)| path.strip_suffix(".o"))
                .map_or_else(|| Ok(Mark::default()), mark)
        };
        measurement.done.executable += size;
        let mut credit = Counted::default();
        match origin(object, output, overlay, source)? {
            origin @ (Origin::CommonC | Origin::GameC) => {
                let object_name = unit_object(object, output, overlay)
                    .ok_or_else(|| format!("{object}: credited outside the build directory"))?;
                let mark = marked()?;
                let steered = mark.steered.clone();
                for function in placed_c_functions(&placed)? {
                    let bytes = function.bytes;
                    let mut credit = Counted::default();
                    if origin == Origin::CommonC {
                        credit.done.common_c = bytes;
                    } else {
                        credit.done.game_c = bytes;
                    }
                    credit.steered = match &steered {
                        Steered::None => 0,
                        Steered::Whole => bytes,
                        Steered::Functions(names) if !names.is_disjoint(&function.names) => bytes,
                        Steered::Functions(_) => 0,
                    };
                    if credit.steered > 0 {
                        let roots = steering_roots(&mark.reasons, &function.names);
                        let unit = (image.to_string(), object_name.clone(), Some(function.name.to_string()));
                        measurement.steered_units.push((unit, bytes, roots));
                    }
                    let unit = (image.to_string(), object_name.clone(), Some(function.name));
                    measurement.done += credit.done;
                    measurement.steered += credit.steered;
                    *measurement.credits.entry(unit.clone()).or_default() += credit;
                    credited.push((placed.address + function.offset, bytes, unit));
                }
                continue;
            }
            origin @ (Origin::CommonAsm | Origin::GameAsm) => {
                let mark = marked()?;
                let veneer = mark.veneer && placed_veneers(&placed)?;
                let proven = if mark.veneer { veneer } else { mark.assembly_proof.is_some() };
                if !proven {
                    measurement.raw += size;
                    credit.unproven_asm = size;
                } else if origin == Origin::CommonAsm {
                    credit.done.common_asm = size;
                } else {
                    credit.done.game_asm = size;
                }
                if veneer {
                    credit.done.veneers = size;
                }
            }
            Origin::Library => {
                credit.done.game_asm = size;
                credit.library = size;
            }
            Origin::Raw => {
                measurement.raw += size;
                continue;
            }
            Origin::Listing => {
                measurement.listings += size;
                continue;
            }
            Origin::Other => {
                match measurement
                    .other
                    .iter_mut()
                    .find(|(path, _)| path == object)
                {
                    Some((_, bytes)) => *bytes += size,
                    None => measurement.other.push((object.to_owned(), size)),
                }
                continue;
            }
        }
        let unit = (
            image.to_owned(),
            unit_object(object, output, overlay)
                .ok_or_else(|| format!("{object}: credited outside the build directory"))?,
            None,
        );
        measurement.done += credit.done;
        measurement.library += credit.library;
        measurement.steered += credit.steered;
        *measurement.credits.entry(unit.clone()).or_default() += credit;
        credited.push((placed.address, size, unit));
    }
    Ok(credited)
}

pub(crate) fn maintained_source(root: &Path, stem: &str) -> Option<Language> {
    [
        ("C", Language::C),
        ("c", Language::C),
        ("S", Language::Assembly),
        ("s", Language::Assembly),
        ("MID", Language::Assembly),
    ]
    .into_iter()
    .find(|(extension, _)| root.join(format!("{stem}.{extension}")).is_file())
    .map(|(_, language)| language)
}

pub(crate) fn image(target: DecompTarget) -> String {
    format!("{}/{}.gba", target.output_dir, target.id)
}

pub(crate) fn verified(root: &Path, target: DecompTarget) -> Result<Result<(), String>, String> {
    let name = image(target);
    let digests = std::fs::read_to_string(root.join("rom.sha1"))
        .map_err(|error| format!("rom.sha1: {error}"))?;
    let expected = digests
        .lines()
        .filter_map(|line| line.split_once(char::is_whitespace))
        .find(|(_, path)| path.trim().trim_start_matches('*') == name)
        .map(|(digest, _)| digest.to_ascii_lowercase())
        .ok_or_else(|| format!("rom.sha1 names no {name}"))?;
    let bytes = match std::fs::read(root.join(&name)) {
        Ok(bytes) => bytes,
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            return Ok(Err(format!("pending a build of {name}")))
        }
        Err(error) => return Err(format!("{name}: {error}")),
    };
    let actual = Sha1::digest(&bytes)
        .iter()
        .map(|byte| format!("{byte:02x}"))
        .collect::<String>();
    Ok(if actual == expected {
        Ok(())
    } else {
        Err(format!("pending: {name} differs from rom.sha1"))
    })
}

fn streamed(root: &Path, target: DecompTarget, map: &str) -> Result<Vec<String>, String> {
    let mut ids = BTreeSet::new();
    for placed in sections(map) {
        if placed.name != ".overlays" || placed.size <= 0 {
            continue;
        }
        let Some(stem) = relative_object(placed.object, target.output_dir, false)
            .and_then(|(path, _)| path.strip_suffix(".o"))
        else {
            continue;
        };
        let source = crate::build_rom::source_extensions(stem)
            .iter()
            .map(|extension| PathBuf::from(format!("{stem}.{extension}")))
            .find(|path| root.join(path).is_file())
            .ok_or_else(|| format!("{stem}.o: no overlay stream source; rebuild"))?;
        ids.extend(crate::build_rom::stream_ids(root, target, &source)?);
    }
    Ok(ids.into_iter().collect())
}

struct ImageMap {
    image: String,
    path: PathBuf,
    text: String,
}

fn maps(root: &Path, target: DecompTarget) -> Result<Result<Vec<ImageMap>, String>, String> {
    if let Err(reason) = verified(root, target)? {
        return Ok(Err(reason));
    }
    let output = target.output_dir;
    let written = |path: &Path| {
        std::fs::metadata(path)
            .and_then(|metadata| metadata.modified())
            .map_err(|error| format!("{}: {error}", path.display()))
    };
    let read = |path: &Path| {
        std::fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))
    };
    let shown = |path: &Path| {
        path.strip_prefix(root)
            .unwrap_or(path)
            .display()
            .to_string()
    };
    let linked = written(&root.join(image(target)))?;
    let main = root.join(format!("{output}/{}.map", target.id));
    let mut found = vec![ImageMap {
        image: MAIN_IMAGE.to_owned(),
        text: read(&main)?,
        path: main,
    }];
    let overlays = root.join(output).join("overlays");
    for id in streamed(root, target, &found[0].text)? {
        let path = overlays.join(format!("resource_{id}.map"));
        if !path.is_file() || !overlays.join(format!("resource_{id}.lz")).is_file() {
            return Ok(Err(format!(
                "pending: {} is not built beside the image that reads it; rebuild",
                shown(&path)
            )));
        }
        found.push(ImageMap {
            image: id,
            text: read(&path)?,
            path,
        });
    }
    for map in &found {
        if written(&map.path)? > linked {
            return Ok(Err(format!(
                "pending: {} is newer than its verified image; rebuild",
                shown(&map.path)
            )));
        }
    }
    Ok(Ok(found))
}

pub(crate) fn measure(
    root: &Path,
    target: DecompTarget,
) -> Result<Result<Measurement, String>, String> {
    let maps = match maps(root, target)? {
        Ok(maps) => maps,
        Err(reason) => return Ok(Err(reason)),
    };
    let output = target.output_dir;
    let source = |stem: &str| maintained_source(root, stem);
    let expansion = crate::compiler::preprocess::Expansion::new(root, target)?;
    let marks = std::cell::RefCell::new(std::collections::HashMap::new());
    let mark = |stem: &str| {
        marks
            .borrow_mut()
            .entry(stem.to_string())
            .or_insert_with(|| source_mark(root, &expansion, stem))
            .clone()
    };
    let mut measurement = Measurement::default();
    let mut code = 0;
    // Each overlay counts once, in its unpacked image: the ROM leaves its
    // compressed stream out of DONE data (C2).
    let mut streams = std::collections::BTreeMap::new();
    for map in maps.iter().filter(|map| map.image != MAIN_IMAGE) {
        let lz = map.path.with_extension("lz");
        let bytes = std::fs::metadata(&lz)
            .map_err(|error| format!("{}: {error}", lz.display()))?
            .len();
        streams.insert(map.image.to_ascii_lowercase(), bytes as i64);
    }
    let unpacked = std::collections::BTreeMap::new();
    for map in &maps {
        let placed = tally(
            &mut measurement,
            &map.text,
            output,
            &map.image,
            &source,
            &mark,
        )?;
        let elf = if map.image == MAIN_IMAGE {
            root.join(format!("{output}/{}.elf", target.id))
        } else {
            map.path.with_extension("elf")
        };
        let nm = std::process::Command::new("arm-none-eabi-nm")
            .arg(&elf)
            .output()
            .map_err(|error| format!("cannot inspect symbols in {}: {error}", elf.display()))?;
        if !nm.status.success() {
            return Err(format!(
                "cannot inspect symbols in {}: arm-none-eabi-nm {}: {}",
                elf.display(),
                nm.status,
                String::from_utf8_lossy(&nm.stderr).trim()
            ));
        }
        let listed = String::from_utf8_lossy(&nm.stdout);
        measurement.symbols.extend(
            listed
                .lines()
                .filter_map(|line| line.split_whitespace().nth(2))
                .map(str::to_owned),
        );
        let spans = uncredited_spans(&listed);
        discredit(&mut measurement, &placed, &spans);
        let overlay = map.image != MAIN_IMAGE;
        let describe = |placed: &Placed<'_>| -> Result<(String, &'static str), String> {
            let object = unit_object(placed.object, output, overlay)
                .unwrap_or_else(|| placed.object.to_owned());
            if !is_text(placed) {
                return Ok((object, "outside"));
            }
            let origin = match origin(placed.object, output, overlay, &source)? {
                Origin::CommonC | Origin::GameC => "C",
                Origin::CommonAsm | Origin::GameAsm => {
                    let key = (map.image.clone(), object.clone(), None);
                    if measurement.credits.get(&key).is_some_and(|credit| credit.unproven_asm > 0) {
                        "disassembly"
                    } else {
                        "assembly"
                    }
                }
                Origin::Library => "library",
                Origin::Raw => "disassembly",
                Origin::Listing => "listing",
                Origin::Other => "other",
            };
            Ok((object, origin))
        };
        let bytes =
            std::fs::read(&elf).map_err(|error| format!("{}: {error}", elf.display()))?;
        let data = super::data::loaded(&bytes)
            .and_then(|loaded| {
                let streams = if overlay { &unpacked } else { &streams };
                super::data::tally(&map.text, &loaded, output, overlay, streams)
            })
            .map_err(|error| format!("{}: {error}", map.path.display()))?;
        measurement.done.data_total += data.total;
        measurement.done.data += data.done;
        code += data.code;
        for (object, bytes) in data.pending {
            measurement
                .data_pending
                .push((map.image.clone(), object, bytes));
        }
        let image = super::arm::Image::parse(&bytes)
            .map_err(|error| format!("{}: {error}", elf.display()))?;
        // A label names code only in the object that defines it: read from
        // the object file itself, or for an archive member from the globals
        // the map lists under that input section.
        let globals = super::arm::map_globals(&map.text);
        let defined = |placed: &Placed<'_>| -> Result<Vec<super::arm::Label>, String> {
            let start = placed.address as u32;
            if !placed.object.ends_with(')') {
                if let Ok(bytes) = std::fs::read(root.join(placed.object)) {
                    return super::arm::object_labels(&bytes, placed.name, start)
                        .map_err(|error| format!("{}: {error}", placed.object));
                }
            }
            let key = (placed.name.to_owned(), placed.address, placed.object.to_owned());
            Ok(globals
                .get(&key)
                .into_iter()
                .flatten()
                .map(|(address, name)| super::arm::Label {
                    address: *address,
                    name: name.clone(),
                    global: true,
                    function: false,
                })
                .collect())
        };
        // A section's modes are what its own object marks; an archive
        // member falls back to the image's mapping symbols.
        let own = |placed: &Placed<'_>| -> Result<Option<Vec<(u32, super::arm::Mode)>>, String> {
            if placed.object.ends_with(')') {
                return Ok(None);
            }
            let Ok(bytes) = std::fs::read(root.join(placed.object)) else {
                return Ok(None);
            };
            super::arm::object_mappings(&bytes, placed.name, placed.address as u32)
                .map(Some)
                .map_err(|error| format!("{}: {error}", placed.object))
        };
        let (arm, likely) = super::arm::inventory(
            &image,
            &map.image,
            &sections(&map.text),
            &spans,
            &describe,
            &defined,
            &own,
        )?;
        measurement.arm.extend(arm);
        measurement.arm_likely.extend(likely);
    }
    // Padding a source marks as carrying no credit leaves the code total;
    // it is non-code bytes from games/, so it is done data, and code and
    // data together are every byte the images place.
    measurement.done.data += measurement.uncredited;
    measurement.done.data_total += measurement.uncredited;
    if measurement.done.executable + measurement.uncredited != code {
        return Err(format!(
            "{}: code and data do not partition its images",
            target.id
        ));
    }
    for (object, bytes) in &measurement.other {
        eprintln!(
            "{}: {bytes} text bytes from unclassified {object}",
            target.id
        );
    }
    Ok(Ok(measurement))
}

pub(crate) fn measure_game(
    root: &Path,
    target: DecompTarget,
) -> Result<Result<Game, String>, String> {
    let mut measured = std::thread::scope(|scope| {
        let threads = target
            .editions()
            .map(|edition| scope.spawn(move || (edition, measure(root, edition))));
        threads.map(|thread| thread.join().expect("measurement thread"))
    });
    let at = measured
        .iter()
        .position(|(edition, _)| edition.id == target.id)
        .ok_or("the English build is not one of its game's editions")?;
    let english = match std::mem::replace(&mut measured[at].1, Ok(Err(String::new())))? {
        Ok(measurement) => measurement,
        Err(reason) => return Ok(Err(reason)),
    };
    let mut editions = Vec::with_capacity(6);
    let mut missing = Vec::new();
    for (edition, measurement) in measured {
        let earned = if edition.id == target.id {
            share_edition(&english, &english)
        } else {
            match measurement? {
                Ok(measurement) => {
                    missing.push((edition.language(), unlinked(&english, &measurement)));
                    share_edition(&english, &measurement)
                }
                Err(reason) => return Ok(Err(reason)),
            }
        };
        if edition.id == target.id
            && (earned.done != english.done || earned.steered != english.steered)
        {
            return Err(format!(
                "{}: its credited units do not add up to its DONE",
                target.id
            ));
        }
        editions.push((edition.language(), earned));
    }
    Ok(Ok(Game {
        english,
        editions,
        missing,
    }))
}

#[cfg(test)]
mod assembly_tests {
    use super::*;

    fn assembly(mark: Mark, common: bool) -> Measurement {
        let path = if common { "common" } else { "tbs" };
        let map = format!(" .text 0x08001000 0x20 out/tbs-en/obj/games/{path}/src/routine.o\n");
        let mut measurement = Measurement::default();
        tally(&mut measurement, &map, "out/tbs-en", MAIN_IMAGE,
            &|_| Some(Language::Assembly), &|_| Ok(mark.clone())).unwrap();
        measurement
    }

    #[test]
    fn assembly_location_never_supplies_a_proof() {
        for common in [false, true] {
            let pending = assembly(Mark::default(), common);
            assert_eq!(pending.done.executable, 32);
            assert_eq!(pending.done.bytes(), 0);
            assert_eq!(pending.raw, 32);
            let reviewed = assembly(Mark {
                assembly_proof: Some("r11 is an ordinary data register".into()),
                ..Mark::default()
            }, common);
            assert_eq!(reviewed.done.bytes(), 32);
            assert_eq!(reviewed.raw, 0);
            assert_eq!(share_edition(&reviewed, &pending).done.bytes(), 0);
        }
    }

    #[test]
    fn only_a_nonempty_leading_assembly_proof_is_recorded() {
        assert_eq!(assembly_proof("\n@ C2: shift carry reaches bcs\n.arm\n"),
            Some("shift carry reaches bcs".into()));
        for text in ["@ C2: \n.arm\n", ".ascii \"C2: proof\"", ".arm\n@ C2: unrelated suffix"] {
            assert!(assembly_proof(text).is_none());
        }
    }

    #[test]
    fn veneers_require_every_complete_aligned_entry() {
        let entry = [0x03, 0x4b, 0x18, 0x47, 0, 0, 0, 0];
        // The literal must be the target word immediately after bx.
        assert!(!veneer_bytes(&entry, 0x100));
        let entry = [0x00, 0x4b, 0x18, 0x47, 0, 0, 0, 0];
        assert!(veneer_bytes(&entry, 0x100));
        assert!(!veneer_bytes(&entry, 0x102));
        assert!(!veneer_bytes(&entry[..6], 0x100));
        assert!(!veneer_bytes(&[], 0x100));
        let mut mixed = entry.to_vec();
        mixed.extend_from_slice(&[0xc0, 0x46, 0x70, 0x47, 0, 0, 0, 0]);
        assert!(!veneer_bytes(&mixed, 0x100));
        let mut wrong_register = entry;
        wrong_register[2] = 0x20;
        assert!(!veneer_bytes(&wrong_register, 0x100));
    }

    #[test]
    fn padding_in_unproven_source_stays_data_without_granting_code_credit() {
        let mut measurement = assembly(Mark::default(), false);
        let unit = measurement.credits.keys().next().unwrap().clone();
        discredit(&mut measurement, &[(0x08001000, 32, unit)], &[(0x08001008, 4)]);
        assert_eq!(measurement.done.bytes(), 0);
        assert_eq!(measurement.done.executable, 28);
        assert_eq!(measurement.raw, 28);
        assert_eq!(measurement.uncredited, 4);
    }
}

#[cfg(test)]
mod absent_tests {
    use super::*;

    fn unit(object: &str, function: &str) -> Unit {
        (MAIN_IMAGE.to_owned(), object.to_owned(), Some(function.to_owned()))
    }

    #[test]
    fn only_code_the_edition_leaves_out_leaves_its_total() {
        let mut english = Measurement::default();
        for (name, bytes) in [("Kept", 40), ("Scaffold", 30), ("Absent", 20), ("Unlinked", 10)] {
            let mut credit = Counted::default();
            credit.done.game_c = bytes;
            english.credits.insert(unit("A.o", name), credit);
            english.done += credit.done;
        }
        english.credits.insert(unit("B.o", "Elsewhere"), {
            let mut credit = Counted::default();
            credit.done.game_c = 5;
            credit
        });
        english.done.game_c += 5;
        english.done.executable = 105;
        let mut edition = Measurement::default();
        edition.credits.insert(unit("A.o", "Kept"), english.credits[&unit("A.o", "Kept")]);
        edition.objects.insert("A.o".to_owned());
        edition.symbols.insert("Scaffold".to_owned());
        // Absent: A.o is linked, no symbol. Elsewhere: B.o is not linked here.
        assert!(absent(&english, &edition, &unit("A.o", "Absent")));
        assert!(!absent(&english, &edition, &unit("A.o", "Scaffold")));
        assert!(!absent(&english, &edition, &unit("A.o", "Kept")));
        assert!(!absent(&english, &edition, &unit("B.o", "Elsewhere")));
        let counted = share_edition(&english, &edition);
        assert_eq!(counted.done.executable, 105 - 20 - 10);
        assert_eq!(counted.done.bytes(), 40);
    }
}

#[cfg(test)]
mod padding_tests {
    use super::*;

    #[test]
    fn only_padding_inside_credited_source_leaves_the_total() {
        let unit = (MAIN_IMAGE.to_owned(), "games/X.o".to_owned(), None);
        let mut measurement = Measurement::default();
        let mut credit = Counted::default();
        credit.done.game_asm = 32;
        measurement.credits.insert(unit.clone(), credit);
        measurement.done.game_asm = 32;
        measurement.done.executable = 32 + 16;
        // A 4-byte span inside the credited object, and one in a raw listing.
        discredit(&mut measurement, &[(0x100, 32, unit.clone())], &[(0x110, 4), (0x200, 4)]);
        assert_eq!(measurement.done.executable, 32 + 16 - 4);
        assert_eq!(measurement.done.game_asm, 28);
        assert_eq!(measurement.uncredited, 4);
        assert_eq!(measurement.credits[&unit].uncredited, 4);
    }
}

/// An edition's missing units: each with its English bytes and whether
/// the edition has it at all.
pub type Missing = Vec<(Unit, i64, bool)>;
