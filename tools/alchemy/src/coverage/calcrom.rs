//! pret's calcrom over the linker's own maps. Every executable byte of a
//! verified build is an input `text` section the map places; the object that
//! supplied it says whether it is maintained source under `games/`, a proven
//! compiler-library member, or disassembly not yet in C. Nothing else is read:
//! no catalog, no receipt and no guess at what is code.
//!
//! DONE counts a game's six editions together, so 100% means every language
//! builds all its code from source. The English build gives each credited
//! function's bytes and the executable total E. An edition earns a C function's
//! English bytes only when its own verified build links that definition from
//! source in the same image: the main image, or the same code overlay built
//! from source in that edition. DONE is the sum over the six editions out of
//! 6 × E, and a game is published only while all six builds are verified.
//!
//! Known limit: sizes are the English build's, and code that exists only in
//! another edition is not in the total.
use super::progress::GameDone;
use crate::targets::DecompTarget;
use object::{Object, ObjectSection, ObjectSymbol, SymbolKind};
use sha1::{Digest, Sha1};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

/// One source contribution in one image: its image and object path, and
/// for C, the actual function definition. Assembly and library members keep
/// their existing whole-object identity. C names stay object-scoped so a
/// same-name static helper in another module cannot earn the function.
pub(crate) type Unit = (String, String, Option<String>);

/// A placed object's image and portable build-relative identity.
pub(crate) type ObjectKey = (String, String);

/// One placed initialized-data input section: image, portable object and
/// section name. Localized variants keep this identity even when their
/// lengths differ; another section in the same object earns no credit.
pub(crate) type DataKey = (String, String, String);

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct ObjectBytes {
    pub executable: i64,
    pub data_source: i64,
    pub data_scaffold: i64,
    pub source_path: Option<String>,
}

/// The image every build links first; the others are its code overlays.
pub(crate) const MAIN_IMAGE: &str = "main";

/// DONE bytes with the parts shown beside them: one object's in the English
/// build, an edition's, or a game's in all six editions together.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub(crate) struct Counted {
    pub done: GameDone,
    /// Compiler-library members, counted within `done.game_asm`.
    pub library: i64,
    /// English text bytes of C functions this edition's FAKEMATCH tags steer.
    pub steered: i64,
    /// Padding a source marks as carrying no credit, left out of DONE.
    pub uncredited: i64,
}
impl std::ops::AddAssign for Counted {
    fn add_assign(&mut self, other: Counted) {
        self.done += other.done;
        self.library += other.library;
        self.steered += other.steered;
        self.uncredited += other.uncredited;
    }
}

/// A game's DONE in all six of its editions together.
#[derive(Clone, Debug)]
pub(crate) struct Game {
    /// The English build: every credited object's bytes and the executable
    /// total.
    pub english: Measurement,
    /// Each edition by its language, Japanese first: the English bytes of
    /// the credited objects its own build links, out of the English total.
    pub editions: Vec<(&'static str, Counted)>,
    /// The same verified definitions used to calculate each edition's share.
    pub edition_credits: BTreeMap<&'static str, BTreeMap<Unit, Counted>>,
    /// Positive initialized-data sections each verified edition links from
    /// maintained source, with their own lengths. Reports use English weights.
    pub edition_data: BTreeMap<&'static str, BTreeMap<DataKey, i64>>,
}
impl Game {
    /// The six editions added up: DONE out of six times the English total.
    pub fn combined(&self) -> Counted {
        let mut combined = Counted::default();
        for (_, edition) in &self.editions {
            combined += *edition;
        }
        combined
    }
}

/// What an edition earns: the English bytes of every credited unit its own
/// build links, less the stray padding, out of the English executable total.
pub(crate) fn share(english: &Measurement, linked: &BTreeSet<Unit>) -> Counted {
    let mut counted = Counted::default();
    for (unit, credit) in &english.credits {
        if linked.contains(unit) {
            counted += *credit;
        }
    }
    counted.done.game_asm -= english.stray.min(counted.done.game_asm);
    counted.uncredited += english.stray;
    counted.done.executable = english.done.executable;
    counted
}

/// Keep English sizes and source credit, but use the tags in the edition
/// whose build links each function. Conditional localized devices must not
/// inherit the English branch's unsteered classification, or vice versa.
fn share_edition(english: &Measurement, edition: &Measurement) -> Counted {
    let linked = edition.credits.keys().cloned().collect();
    let mut counted = share(english, &linked);
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

/// One game's executable bytes by the object that supplied them.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Measurement {
    /// DONE: maintained C and assembly under `games/`, the library in game assembly.
    pub done: GameDone,
    /// Compiler-library members, counted within `done.game_asm`.
    pub library: i64,
    /// Main-image disassembly under `recon/<game>/raw`, not yet C.
    pub raw: i64,
    /// Code overlays still linked from their listings, not yet C.
    pub listings: i64,
    /// Any other object with text, not yet C, shown so it is never hidden.
    pub other: Vec<(String, i64)>,
    /// Data the maps place from `games/` sources, as pret's calcrom --data
    /// counts `src` rodata: built assets, tables and C data.
    pub data_source: i64,
    /// The source input sections behind `data_source`, before object totals
    /// erase which parts of a partially adopted object are actually linked.
    pub data_sections: BTreeMap<DataKey, i64>,
    /// Data still placed from scaffolding: baserom ranges and listing data.
    pub data_scaffold: i64,
    /// The main image's symbol names, as pret's calcrom counts them.
    pub names: Names,
    /// Text of the C functions a FAKEMATCH tag steers: counted in DONE until
    /// both games are done (AGENTS.md S2), shown on its own.
    pub steered: i64,
    /// Padding a source marks as carrying no credit, between its
    /// `AlchemyUncredited_X` and `AlchemyUncreditedEnd_X` labels: placed,
    /// but not counted in DONE (C2).
    pub uncredited: i64,
    /// The part of `uncredited` that sits in no credited unit. It is taken
    /// from the game's own assembly as a whole, in every edition alike.
    pub stray: i64,
    /// DONE by the unit that supplied it, without its own uncredited padding:
    /// what an edition earns when its own build links the unit.
    pub credits: BTreeMap<Unit, Counted>,
    /// Every placed text/data object, including uncredited raw/listing objects.
    pub objects: BTreeMap<ObjectKey, ObjectBytes>,
}

/// What a `games/` source says about its object's bytes.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Mark {
    /// C steered by a FAKEMATCH workaround.
    pub steered: Steered,
    /// Assembly credited as whole 8-byte stubs (`@ credit: reconstructed_veneer`).
    pub veneer: bool,
}

/// Which of a C source's functions its FAKEMATCH tags steer.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) enum Steered {
    #[default]
    None,
    /// Source-owned file-scope steering: the whole object counts.
    Whole,
    /// Tagged definitions and callers of tagged inline helpers.
    Functions(std::collections::BTreeSet<String>),
}

/// Read the marks of the maintained source a `games/` object was built from.
fn source_mark(
    root: &Path,
    expansion: &crate::compiler::preprocess::Expansion<'_>,
    stem: &str,
) -> Result<Mark, String> {
    let Some((path, text)) = ["C", "c", "S", "s"].iter().find_map(|extension| {
        let path = format!("{stem}.{extension}");
        std::fs::read_to_string(root.join(&path))
            .ok()
            .map(|text| (path, text))
    }) else {
        return Ok(Mark::default());
    };
    let steered = if path.ends_with(".C") || path.ends_with(".c") {
        let expanded = expansion.fresh(&path)?;
        let analysis = crate::compiler::steering::analyze(&expanded, &path)?;
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
    } else if !text.contains("FAKEMATCH") {
        Steered::None
    } else {
        crate::permute::parse::tagged_functions(&text, "FAKEMATCH")
            .map_or(Steered::Whole, Steered::Functions)
    };
    Ok(Mark {
        steered,
        veneer: text.contains("@ credit: reconstructed_veneer"),
    })
}

/// The bytes of `names` in one object's text section of `size` bytes: each
/// function runs to the next function or the section end, including its pool.
/// Section offsets are local: unrelated .text.* sections must never mix.
#[cfg(test)]
fn function_bytes_from_object(
    bytes: &[u8],
    section_name: &str,
    names: &std::collections::BTreeSet<String>,
    size: i64,
) -> Result<i64, String> {
    let symbols = section_function_symbols(bytes, section_name, size)?;
    Ok(function_spans(&symbols, names, size))
}

#[derive(Debug)]
struct FunctionSymbol<'a> {
    offset: i64,
    size: u64,
    name: &'a str,
    public: bool,
}

#[cfg(test)]
fn section_function_symbols<'a>(
    bytes: &'a [u8],
    section_name: &str,
    size: i64,
) -> Result<Vec<(i64, &'a str)>, String> {
    Ok(section_function_definitions(bytes, section_name, size)?
        .into_iter()
        .map(|symbol| (symbol.offset, symbol.name))
        .collect())
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

/// The C definitions in a placed section, with their complete spans including
/// pools and alignment through the next function or section end. A positive
/// C section without usable definition metadata cannot earn object credit.
#[derive(Debug, PartialEq, Eq)]
struct CFunction {
    offset: i64,
    bytes: i64,
    name: String,
    /// Compiler-local and exported names of the same physical definition.
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
        // GCC's nested definition has a local assembler name and can also
        // export one public name for its same body. It is one extent, not two
        // C functions. Only identical nonzero symbol extents and one public
        // identity are usable; conflicting public definitions stay refused.
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

#[cfg(test)]
/// Sum the spans of `names` among `symbols`, sorted by offset in a section
/// of `size` bytes.
fn function_spans(
    symbols: &[(i64, &str)],
    names: &std::collections::BTreeSet<String>,
    size: i64,
) -> i64 {
    let mut intervals = std::collections::BTreeSet::new();
    for (index, (start, name)) in symbols.iter().enumerate() {
        if names.contains(*name) {
            let end = symbols
                .iter()
                .skip(index + 1)
                .map(|(next, _)| *next)
                .find(|next| next > start)
                .unwrap_or(size);
            intervals.insert((*start, end.min(size)));
        }
    }
    intervals
        .into_iter()
        .map(|(start, end)| (end - start).max(0))
        .sum()
}

/// The span between each `AlchemyUncredited_X` label and its
/// `AlchemyUncreditedEnd_X` in one image's `nm` output: where it starts and
/// its bytes, in address order.
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

/// Take one image's uncredited padding out of DONE: each span leaves the
/// credit of the unit whose placed text holds it, assembly first. A span in
/// no credited unit (a listing marks its padding before it is adopted) is
/// stray: it leaves the game's own assembly as a whole, as it always has.
fn discredit(measurement: &mut Measurement, placed: &[(i64, i64, Unit)], spans: &[(i64, i64)]) {
    for (start, bytes) in spans {
        measurement.uncredited += bytes;
        let mut stray = *bytes;
        for (address, size, unit) in placed {
            let overlap = (start + bytes).min(address + size) - start.max(address);
            if overlap <= 0 {
                continue;
            }
            let Some(credit) = measurement.credits.get_mut(unit) else {
                continue;
            };
            stray -= overlap;
            credit.uncredited += overlap;
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
        measurement.stray += stray;
    }
}

/// Symbol names by how much they say, after pret's calcrom: a placeholder
/// that is only an address, a word with an address in it, or documented.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub(crate) struct Names {
    pub total: i64,
    pub undocumented: i64,
    pub partial: i64,
}

impl Names {
    pub fn documented(&self) -> i64 {
        self.total - self.undocumented - self.partial
    }
}

/// Count an image's names from `nm` output, skipping short names and those
/// starting with `_`, `$` or `.`, as pret's filter does.
pub(crate) fn names(nm: &str) -> Names {
    let address = regex::Regex::new(r"_0[238][0-9A-Fa-f]{6}").expect("static pattern");
    let placeholder = regex::Regex::new(
        r"^(?:[Ff]unc|[Dd]ata|Unnamed|Value|Entry|[Uu]nknown|[Ss]ub|[Uu]nk)_0[238][0-9A-Fa-f]{6}$",
    )
    .expect("static pattern");
    let mut seen = std::collections::BTreeSet::new();
    let mut counted = Names::default();
    for name in nm.lines().filter_map(|line| line.split_whitespace().nth(2)) {
        if name.len() < 5 || name.starts_with(['_', '$', '.']) || !seen.insert(name) {
            continue;
        }
        counted.total += 1;
        if placeholder.is_match(name) {
            counted.undocumented += 1;
        } else if address.is_match(name) {
            counted.partial += 1;
        }
    }
    counted
}

/// A section whose bytes are data the image carries: not code, RAM layout
/// or a packed code overlay.
fn is_data(name: &str) -> bool {
    name.starts_with(".rodata")
        || name == ".data"
        || name.starts_with(".data.")
        || name.starts_with(".unidentified")
}

/// Where a text section's object came from.
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

/// The language of the maintained source an object under `games/` was built
/// from, as `build rom` chooses it: C first, then assembly.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Language {
    C,
    Assembly,
}

/// An input section a map places.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) struct Placed<'a> {
    pub name: &'a str,
    pub address: i64,
    pub size: i64,
    pub object: &'a str,
}

/// Every input section the map places. The discarded sections listed before
/// the memory map are skipped.
pub(crate) fn sections(map: &str) -> Vec<Placed<'_>> {
    let mut found = Vec::new();
    let mut discarded = false;
    let mut lines = map.lines().peekable();
    while let Some(line) = lines.next() {
        match line {
            "Discarded input sections" => discarded = true,
            "Memory Configuration" | "Linker script and memory map" => discarded = false,
            _ => {}
        }
        let Some(rest) = line.strip_prefix(" .") else {
            continue;
        };
        let name_end = rest.find(char::is_whitespace).unwrap_or(rest.len());
        let name = &line[1..name_end + 2];
        let placed = if name_end == rest.len() {
            // A long section name stands alone; its placement follows.
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
            found.push(Placed {
                name,
                address: placed.0,
                size: placed.1,
                object: placed.2,
            });
        }
    }
    found
}

/// `  0xADDRESS  0xSIZE  object`, as the map places an input section.
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

/// An object's path under its build directory's `obj/` (or, in an overlay
/// map, under `overlays/`), and whether it sits in the overlay directory.
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

/// Classify one object by its path alone. `output` is the build directory,
/// such as `out/tbs-en`; `source` finds the language of a `games/` object.
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
        let common = relative.starts_with("games/COMMON/");
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

/// `resource_XXX_overlay.o`, an overlay assembled from its listing.
fn is_listing(name: &str) -> bool {
    name.strip_prefix("resource_")
        .and_then(|rest| rest.strip_suffix("_overlay.o"))
        .is_some_and(|id| !id.is_empty() && id.bytes().all(|byte| byte.is_ascii_hexdigit()))
}

/// Whether a placed section is executable bytes.
fn is_text(placed: &Placed) -> bool {
    placed.size > 0 && !is_data(placed.name) && placed.name.contains("text")
}

/// The name a credited object keeps in every edition's build: its path under
/// `obj/`, or its compiler-library member.
fn unit_object(object: &str, output: &str, overlay: bool) -> Option<String> {
    match object.rfind("libgcc.a(") {
        Some(member) => Some(object[member..].to_owned()),
        None => relative_object(object, output, overlay).map(|(path, _)| path.to_owned()),
    }
}

/// Add one map's text sections to `measurement`, and return where the map
/// places each credited unit's text. `image` names the map's image.
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
        let Placed {
            name, size, object, ..
        } = placed;
        if size <= 0 || (!is_data(name) && !is_text(&placed)) {
            continue;
        }
        let object_name = unit_object(object, output, overlay)
            .unwrap_or_else(|| "Unattributed objects outside build directory".into());
        let bytes = measurement
            .objects
            .entry((image.to_string(), object_name.clone()))
            .or_default();
        if size > 0 && is_data(name) {
            match origin(object, output, overlay, source)? {
                Origin::CommonC | Origin::CommonAsm | Origin::GameC | Origin::GameAsm => {
                    let key = (image.to_string(), object_name, name.to_string());
                    if measurement.data_sections.insert(key, size).is_some() {
                        return Err(format!(
                            "{image}/{object}: duplicate placed data section {name}"
                        ));
                    }
                    measurement.data_source += size;
                    bytes.data_source += size;
                }
                _ => {
                    measurement.data_scaffold += size;
                    bytes.data_scaffold += size;
                }
            }
            continue;
        }
        bytes.executable += size;
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
                let steered = marked()?.steered;
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
                    let unit = (image.to_string(), object_name.clone(), Some(function.name));
                    measurement.done += credit.done;
                    measurement.steered += credit.steered;
                    *measurement.credits.entry(unit.clone()).or_default() += credit;
                    credited.push((placed.address + function.offset, bytes, unit));
                }
                continue;
            }
            origin @ (Origin::CommonAsm | Origin::GameAsm) => {
                if origin == Origin::CommonAsm {
                    credit.done.common_asm = size;
                } else {
                    credit.done.game_asm = size;
                }
                if marked()?.veneer {
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

/// The language `build rom` compiled a `games/` object from.
pub(crate) fn maintained_source(root: &Path, stem: &str) -> Option<Language> {
    [
        ("C", Language::C),
        ("c", Language::C),
        ("S", Language::Assembly),
        ("s", Language::Assembly),
        // A sequence's assembly is converted from its MIDI in every build.
        ("MID", Language::Assembly),
    ]
    .into_iter()
    .find(|(extension, _)| root.join(format!("{stem}.{extension}")).is_file())
    .map(|(_, language)| language)
}

/// The image `build rom` writes for a target, as `rom.sha1` names it.
pub(crate) fn image(target: DecompTarget) -> String {
    format!("{}/{}.gba", target.output_dir, target.id)
}

/// Whether the target's linked image is byte-identical to its reference:
/// `Ok(Err(reason))` while it is missing or differs.
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

/// The resource ids of the code overlays a main image builds from source:
/// each object its map places in `.overlays` is assembled from a source that
/// reads the built stream `overlays/resource_XXX.lz` of every such overlay,
/// found as `build rom` finds them. An overlay an edition still copies from
/// its scaffold has no stream.
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
        let source = ["S", "s"]
            .iter()
            .map(|extension| PathBuf::from(format!("{stem}.{extension}")))
            .find(|path| root.join(path).is_file())
            .ok_or_else(|| format!("{stem}.o: no overlay stream source; rebuild"))?;
        ids.extend(crate::build_rom::stream_ids(root, target, &source)?);
    }
    Ok(ids.into_iter().collect())
}

/// One image of a build with its linker map.
struct ImageMap {
    /// `main`, or the overlay's resource id.
    image: String,
    path: PathBuf,
    text: String,
}

/// The linker maps of a target's verified build, or why it is pending: its
/// main image's, then the map of every code overlay the main image builds
/// from source. A map an older build left beside an overlay the edition no
/// longer builds is not read.
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
        // A link that failed after writing its map leaves an older image.
        if written(&map.path)? > linked {
            return Ok(Err(format!(
                "pending: {} is newer than its verified image; rebuild",
                shown(&map.path)
            )));
        }
    }
    Ok(Ok(found))
}

/// An edition's measurement from its verified image, or why it is
/// pending.
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
    for map in &maps {
        let placed = tally(
            &mut measurement,
            &map.text,
            output,
            &map.image,
            &source,
            &mark,
        )?;
        // The main image names its symbols; every image may place padding its
        // source marks as uncredited, which leaves DONE (C2).
        let elf = if map.image == MAIN_IMAGE {
            root.join(format!("{output}/{}.elf", target.id))
        } else {
            map.path.with_extension("elf")
        };
        let Ok(nm) = std::process::Command::new("arm-none-eabi-nm")
            .arg(elf)
            .output()
        else {
            continue;
        };
        if !nm.status.success() {
            continue;
        }
        let listed = String::from_utf8_lossy(&nm.stdout);
        if map.image == MAIN_IMAGE {
            measurement.names = names(&listed);
        }
        discredit(&mut measurement, &placed, &uncredited_spans(&listed));
    }
    measurement.done.game_asm -= measurement.stray.min(measurement.done.game_asm);
    validate_data_sections(&measurement)?;
    for ((_, object), bytes) in &mut measurement.objects {
        if !object.starts_with("games/") && !object.starts_with("recon/") {
            continue;
        }
        if let Some(stem) = object.strip_suffix(".o") {
            bytes.source_path = ["C", "c", "S", "s", "MID"]
                .into_iter()
                .map(|extension| format!("{stem}.{extension}"))
                .find(|path| root.join(path).is_file());
        }
    }
    for (object, bytes) in &measurement.other {
        eprintln!(
            "{}: {bytes} text bytes from unclassified {object}",
            target.id
        );
    }
    Ok(Ok(measurement))
}

/// Keep the section inventory and the existing map/object totals identical.
/// The report may change a section's weight, never its source provenance.
fn validate_data_sections(measurement: &Measurement) -> Result<(), String> {
    let mut source = BTreeMap::<ObjectKey, i64>::new();
    for ((image, object, section), size) in &measurement.data_sections {
        if *size <= 0 || !is_data(section) {
            return Err(format!(
                "{image}/{object}: invalid source data section {section}"
            ));
        }
        *source.entry((image.clone(), object.clone())).or_default() += size;
    }
    let mut total_source = 0;
    let mut total_scaffold = 0;
    for (key, bytes) in &measurement.objects {
        if source.remove(key).unwrap_or_default() != bytes.data_source {
            return Err(format!(
                "{}/{}: source data sections do not add up",
                key.0, key.1
            ));
        }
        total_source += bytes.data_source;
        total_scaffold += bytes.data_scaffold;
    }
    if !source.is_empty()
        || total_source != measurement.data_source
        || total_scaffold != measurement.data_scaffold
    {
        return Err("data section and object totals do not add up to the measurement".into());
    }
    Ok(())
}

/// A game's DONE in all six editions together, measured on its English
/// build `target`, or why it is pending: every edition's build must be
/// verified and current.
pub(crate) fn measure_game(
    root: &Path,
    target: DecompTarget,
) -> Result<Result<Game, String>, String> {
    let english = match measure(root, target)? {
        Ok(measurement) => measurement,
        Err(reason) => return Ok(Err(reason)),
    };
    let mut editions = Vec::with_capacity(6);
    let mut edition_credits = BTreeMap::new();
    let mut edition_data = BTreeMap::new();
    for edition in target.editions() {
        let earned = if edition.id == target.id {
            edition_credits.insert(edition.language(), english.credits.clone());
            edition_data.insert(edition.language(), english.data_sections.clone());
            share_edition(&english, &english)
        } else {
            let measurement = match measure(root, edition)? {
                Ok(measurement) => measurement,
                Err(reason) => return Ok(Err(reason)),
            };
            let earned = share_edition(&english, &measurement);
            edition_credits.insert(edition.language(), measurement.credits);
            edition_data.insert(edition.language(), measurement.data_sections);
            earned
        };
        // The English build links every unit it credits: its share is its
        // own measurement, byte for byte.
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
        edition_credits,
        edition_data,
    }))
}

#[cfg(test)]
mod tests {
    use super::*;

    const MAIN: &str = "\
Archive member included to satisfy reference by file (symbol)

/r/tools/out/compiler-runtime/libgcc.a(_call_via_rX.o)
                              /r/out/tbs-en/obj/games/G/SRC/A.o (_call_via_r3)

Discarded input sections

 .text          0x0000000000000000       0x40 /r/out/tbs-en/obj/games/G/SRC/A.o
 .ARM.attributes
                0x0000000000000000       0x20 /r/out/tbs-en/obj/recon/tbs/raw/08000000.o

Memory Configuration

Name             Origin             Length             Attributes

Linker script and memory map

.text           0x0000000008000000     0x1000
 */games/G/SRC/A.o(.text .rodata)
 .text          0x0000000008000000      0x100 /r/out/tbs-en/obj/games/G/SRC/A.o
                0x0000000008000000                A_Main
 .rodata        0x0000000008000100       0x80 /r/out/tbs-en/obj/games/G/SRC/A.o
 .text          0x0000000008000180       0x20 /r/out/tbs-en/obj/games/G/SRC/B.o
 .text          0x00000000080001a0       0x10 /r/out/tbs-en/obj/games/COMMON/SRC/C.o
 .text          0x00000000080001b0        0x8 /r/out/tbs-en/obj/games/COMMON/SRC/D.o
 .text          0x00000000080001b8        0x0 /r/out/tbs-en/obj/games/G/SRC/EMPTY.o
 *fill*         0x00000000080001b8        0x8
 .text          0x00000000080001c0      0x200 /r/out/tbs-en/obj/recon/tbs/raw/080001c0.o
 .text.unlikely
                0x00000000080003c0       0x40 /r/out/tbs-en/obj/recon/tbs/raw/080003c0.o
 .text          0x0000000008000400       0x3c /r/tools/out/compiler-runtime/libgcc.a(_call_via_rX.o)
 .unidentified.08000440
                0x0000000008000440      0x100 /r/out/tbs-en/obj/recon/tbs/unidentified.o
 .text          0x0000000008000540        0x4 /r/out/tbs-en/obj/recon/tbs/stray.o
 .data          0x0000000003000000       0x10 /r/out/tbs-en/obj/games/G/SRC/A.o
";

    const OVERLAY: &str = "\
Linker script and memory map

.text           0x0000000002000000      0x652
 *(.text .text.*)
 .text          0x0000000002000000      0x600 /r/out/tbs-en/overlays/resource_36f_overlay.o
 .text          0x0000000002000600       0x30 /r/out/tbs-en/overlays/obj/games/G/SRC/FIELD/F.o
 .text          0x0000000002000630       0x22 /r/tools/out/compiler-runtime/libgcc.a(_lshrdi3.o)
";

    fn fixture_object(path: &Path, function: &str, size: i64) {
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        let source = path.with_extension("s");
        std::fs::write(&source, format!(".syntax unified\n.thumb\n.text\n.global {function}\n.type {function},%function\n.thumb_func\n{function}:\n bx lr\n .space {}\n", size - 2)).unwrap();
        let assembler = crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-as");
        let output = std::process::Command::new(assembler)
            .args(["-mcpu=arm7tdmi", "-meabi=gnu"])
            .arg(source)
            .arg("-o")
            .arg(path)
            .output()
            .unwrap();
        assert!(
            output.status.success(),
            "{}",
            String::from_utf8_lossy(&output.stderr)
        );
    }

    fn fixture_c_objects(map: &str, output: &str, source: &dyn Fn(&str) -> Option<Language>) {
        for placed in sections(map).into_iter().filter(is_text) {
            let Some((relative, _)) = relative_object(placed.object, output, true) else {
                continue;
            };
            let Some(stem) = relative.strip_suffix(".o") else {
                continue;
            };
            if source(stem) == Some(Language::C) {
                fixture_object(
                    Path::new(placed.object),
                    stem.rsplit('/').next().unwrap(),
                    placed.size,
                );
            }
        }
    }

    fn language(stem: &str) -> Option<Language> {
        match stem {
            "games/G/SRC/A" | "games/COMMON/SRC/C" | "games/G/SRC/FIELD/F" => Some(Language::C),
            "games/G/SRC/B" | "games/COMMON/SRC/D" | "games/G/SRC/EMPTY" => {
                Some(Language::Assembly)
            }
            _ => None,
        }
    }

    #[test]
    fn placed_text_is_counted_by_its_object_and_nothing_else() {
        let work = tempfile::tempdir().unwrap();
        let main = MAIN.replace("/r/", &format!("{}/", work.path().display()));
        let overlay = OVERLAY.replace("/r/", &format!("{}/", work.path().display()));
        fixture_c_objects(&main, "out/tbs-en", &language);
        fixture_c_objects(&overlay, "out/tbs-en", &language);
        let mut measurement = Measurement::default();
        let mark = |stem: &str| {
            Ok(Mark {
                steered: if stem == "games/G/SRC/A" {
                    Steered::Whole
                } else {
                    Steered::None
                },
                veneer: stem == "games/COMMON/SRC/D",
            })
        };
        let placed = tally(
            &mut measurement,
            &main,
            "out/tbs-en",
            MAIN_IMAGE,
            &language,
            &mark,
        )
        .unwrap();
        // Each credited unit's text, where the map places it.
        let unit = |image: &str, object: &str| {
            let stem = object.strip_suffix(".o").unwrap_or(object);
            let function = (language(stem) == Some(Language::C))
                .then(|| stem.rsplit('/').next().unwrap().to_string());
            (image.to_string(), object.to_string(), function)
        };
        assert_eq!(
            placed,
            [
                (0x0800_0000, 0x100, unit("main", "games/G/SRC/A.o")),
                (0x0800_0180, 0x20, unit("main", "games/G/SRC/B.o")),
                (0x0800_01a0, 0x10, unit("main", "games/COMMON/SRC/C.o")),
                (0x0800_01b0, 0x8, unit("main", "games/COMMON/SRC/D.o")),
                (0x0800_0400, 0x3c, unit("main", "libgcc.a(_call_via_rX.o)")),
            ]
        );
        tally(
            &mut measurement,
            &overlay,
            "out/tbs-en",
            "36f",
            &language,
            &mark,
        )
        .unwrap();
        // DONE is exactly the credits, unit by unit.
        let credits = std::mem::take(&mut measurement.credits);
        validate_data_sections(&measurement).unwrap();
        let data_sections = std::mem::take(&mut measurement.data_sections);
        let objects = std::mem::take(&mut measurement.objects);
        assert_eq!(
            data_sections,
            BTreeMap::from([
                (
                    ("main".into(), "games/G/SRC/A.o".into(), ".rodata".into()),
                    0x80
                ),
                (
                    ("main".into(), "games/G/SRC/A.o".into(), ".data".into()),
                    0x10
                ),
            ])
        );
        assert_eq!(
            objects
                .values()
                .map(|object| object.executable)
                .sum::<i64>(),
            measurement.done.executable
        );
        assert_eq!(
            objects
                .values()
                .map(|object| object.data_source)
                .sum::<i64>(),
            measurement.data_source
        );
        assert_eq!(
            objects
                .values()
                .map(|object| object.data_scaffold)
                .sum::<i64>(),
            measurement.data_scaffold
        );
        assert_eq!(
            objects[&("main".into(), "games/G/SRC/A.o".into())],
            ObjectBytes {
                executable: 0x100,
                data_source: 0x90,
                ..ObjectBytes::default()
            }
        );
        assert_eq!(
            objects[&("main".into(), "recon/tbs/raw/080001c0.o".into())].executable,
            0x200
        );
        assert_eq!(
            objects[&("36f".into(), "resource_36f_overlay.o".into())].executable,
            0x600
        );
        let mut sum = Counted::default();
        for credit in credits.values() {
            sum += *credit;
        }
        assert_eq!(
            (sum.done, sum.library, sum.steered),
            (
                GameDone {
                    executable: 0,
                    ..measurement.done
                },
                measurement.library,
                measurement.steered
            )
        );
        assert_eq!(
            credits.keys().cloned().collect::<Vec<_>>(),
            [
                unit("36f", "games/G/SRC/FIELD/F.o"),
                unit("36f", "libgcc.a(_lshrdi3.o)"),
                unit("main", "games/COMMON/SRC/C.o"),
                unit("main", "games/COMMON/SRC/D.o"),
                unit("main", "games/G/SRC/A.o"),
                unit("main", "games/G/SRC/B.o"),
                unit("main", "libgcc.a(_call_via_rX.o)"),
            ]
        );
        assert_eq!(
            credits[&unit("main", "games/G/SRC/A.o")],
            Counted {
                done: GameDone {
                    game_c: 0x100,
                    ..GameDone::default()
                },
                steered: 0x100,
                ..Counted::default()
            }
        );
        assert_eq!(
            measurement,
            Measurement {
                done: GameDone {
                    common_c: 0x10,
                    common_asm: 0x8,
                    game_c: 0x100 + 0x30,
                    game_asm: 0x20 + 0x3c + 0x22,
                    executable: 0x100
                        + 0x20
                        + 0x10
                        + 0x8
                        + 0x200
                        + 0x40
                        + 0x3c
                        + 0x4
                        + 0x600
                        + 0x30
                        + 0x22,
                    veneers: 0x8,
                },
                library: 0x3c + 0x22,
                raw: 0x240,
                listings: 0x600,
                other: vec![(
                    format!("{}/out/tbs-en/obj/recon/tbs/stray.o", work.path().display()),
                    4
                )],
                // A's rodata and data come from source; the baserom range
                // is scaffolding.
                data_source: 0x80 + 0x10,
                data_sections: BTreeMap::new(),
                data_scaffold: 0x100,
                names: Names::default(),
                steered: 0x100,
                uncredited: 0,
                stray: 0,
                credits: BTreeMap::new(),
                objects: BTreeMap::new(),
            }
        );
    }

    #[test]
    fn data_identity_keeps_partial_sections_images_and_localized_lengths_separate() {
        let map = |output: &str, rows: &str| {
            format!("Linker script and memory map\n{rows}").replace("OUT", output)
        };
        let english_main = map(
            "out/tbs-en",
            " \
             .rodata 0x08000000 0x80 OUT/obj/games/G/SRC/A.o\n \
             .data.values 0x03000000 0x10 OUT/obj/games/G/SRC/A.o\n \
             .unidentified.08000080 0x08000080 0x20 OUT/obj/recon/tbs/raw/scaffold.o\n",
        );
        let english_overlay = map(
            "out/tbs-en",
            " \
             .rodata 0x02000000 0x30 OUT/overlays/obj/games/G/SRC/FIELD/F.o\n",
        );
        let localized_main = map(
            "out/tbs-ja",
            " \
             .rodata 0x08000010 0x18 OUT/obj/games/G/SRC/A.o\n \
             .data.values 0x03000000 0x0 OUT/obj/games/G/SRC/A.o\n \
             .data.other 0x03000000 0x10 OUT/obj/games/G/SRC/A.o\n \
             .data.values 0x03000010 0x10 OUT/obj/recon/tbs/raw/scaffold.o\n \
             .rodata 0x08000030 0x30 OUT/obj/games/G/SRC/FIELD/F.o\n",
        );
        let mut english = Measurement::default();
        let mut localized = Measurement::default();
        let mark = |_: &str| Ok(Mark::default());
        tally(
            &mut english,
            &english_main,
            "out/tbs-en",
            MAIN_IMAGE,
            &language,
            &mark,
        )
        .unwrap();
        tally(
            &mut english,
            &english_overlay,
            "out/tbs-en",
            "36f",
            &language,
            &mark,
        )
        .unwrap();
        tally(
            &mut localized,
            &localized_main,
            "out/tbs-ja",
            MAIN_IMAGE,
            &language,
            &mark,
        )
        .unwrap();
        validate_data_sections(&english).unwrap();
        validate_data_sections(&localized).unwrap();
        let key = |image: &str, object: &str, section: &str| {
            (image.to_string(), object.to_string(), section.to_string())
        };
        assert_eq!(english.data_source, 0xc0);
        assert_eq!(english.data_scaffold, 0x20);
        assert_eq!(localized.data_source, 0x58);
        assert_eq!(
            localized.data_sections[&key("main", "games/G/SRC/A.o", ".rodata")],
            0x18
        );
        assert!(!localized.data_sections.contains_key(&key(
            "main",
            "games/G/SRC/A.o",
            ".data.values"
        )));
        assert!(!localized.data_sections.contains_key(&key(
            "36f",
            "games/G/SRC/FIELD/F.o",
            ".rodata"
        )));
        // The smaller complete localized variant earns the English weight;
        // another section, image, or scaffold cannot substitute for a missing one.
        let earned: i64 = english
            .data_sections
            .iter()
            .filter(|(key, _)| localized.data_sections.contains_key(*key))
            .map(|(_, size)| size)
            .sum();
        assert_eq!(earned, 0x80);
        localized
            .data_sections
            .insert(key("main", "games/G/SRC/A.o", ".rodata"), 0x19);
        assert!(validate_data_sections(&localized).is_err());
    }

    #[test]
    fn duplicate_source_data_placements_are_ambiguous_and_refused() {
        let mut measurement = Measurement::default();
        let map = "Linker script and memory map\n \
             .rodata 0x08000000 0x20 out/tbs-en/obj/games/G/SRC/A.o\n \
             .rodata 0x08000020 0x10 out/tbs-en/obj/games/G/SRC/A.o\n";
        assert!(tally(
            &mut measurement,
            map,
            "out/tbs-en",
            MAIN_IMAGE,
            &language,
            &|_| Ok(Mark::default())
        )
        .unwrap_err()
        .contains("duplicate placed data section .rodata"));
    }

    #[test]
    fn uncredited_padding_is_the_span_between_its_two_labels() {
        let nm = "\
080f0100 t AlchemyUncredited_080f0100
080f0104 t AlchemyUncreditedEnd_080f0100
08009bd4 t AlchemyUncredited_08009bd4
08009bda t AlchemyUncreditedEnd_08009bd4
08001000 t AlchemyUncredited_08001000
08000000 T Battle_Start
";
        // Two closed spans of 6 and 4 bytes; a start with no end counts nothing.
        let spans = uncredited_spans(nm);
        assert_eq!(spans, [(0x0800_9bd4, 6), (0x080f_0100, 4)]);
        // Each span leaves the credit of the unit whose text holds it, so an
        // edition that links the unit earns it without the padding.
        let unit = |object: &str| (MAIN_IMAGE.to_string(), object.to_string(), None);
        let assembly = |bytes| Counted {
            done: GameDone {
                game_asm: bytes,
                ..GameDone::default()
            },
            ..Counted::default()
        };
        let mut measurement = Measurement {
            done: GameDone {
                game_asm: 0x60,
                executable: 0x100,
                ..GameDone::default()
            },
            credits: BTreeMap::from([(unit("A.o"), assembly(0x40)), (unit("B.o"), assembly(0x20))]),
            ..Measurement::default()
        };
        let placed = [
            (0x0800_9bc0, 0x40, unit("A.o")),
            (0x080f_0000, 0x20, unit("B.o")),
        ];
        discredit(&mut measurement, &placed, &spans);
        // The second span starts past B's text, in no credited unit: stray.
        assert_eq!((measurement.uncredited, measurement.stray), (10, 4));
        assert_eq!(measurement.done.game_asm, 0x5a);
        assert_eq!(
            measurement.credits[&unit("A.o")],
            Counted {
                uncredited: 6,
                ..assembly(0x3a)
            }
        );
        assert_eq!(measurement.credits[&unit("B.o")], assembly(0x20));
        // Stray padding leaves the game's assembly in every edition alike,
        // whichever units the edition links.
        let only_b = BTreeSet::from([unit("B.o")]);
        let earned = share(&measurement, &only_b);
        assert_eq!((earned.done.game_asm, earned.uncredited), (0x1c, 4));
        let both = measurement.credits.keys().cloned().collect();
        let earned = share(&measurement, &both);
        assert_eq!((earned.done.game_asm, earned.uncredited), (0x56, 10));
    }

    #[test]
    fn edition_tags_use_english_sizes_and_only_its_linked_source() {
        let unit = |name: &str| {
            (
                "36f".to_string(),
                "games/G/SRC/TITLE.o".to_string(),
                Some(name.to_string()),
            )
        };
        let c = |bytes, steered| Counted {
            done: GameDone {
                game_c: bytes,
                ..GameDone::default()
            },
            steered,
            ..Counted::default()
        };
        let english = Measurement {
            done: GameDone {
                game_c: 336,
                executable: 1000,
                ..GameDone::default()
            },
            steered: 120,
            credits: BTreeMap::from([
                (unit("Reveal"), c(176, 0)),
                (unit("EnglishDevice"), c(120, 120)),
                (unit("ScaffoldHere"), c(40, 0)),
            ]),
            ..Measurement::default()
        };
        let localized = Measurement {
            credits: BTreeMap::from([
                (unit("Reveal"), c(196, 196)),
                (unit("EnglishDevice"), c(100, 0)),
                (unit("LocalizedOnly"), c(300, 300)),
            ]),
            ..Measurement::default()
        };
        let linked = localized.credits.keys().cloned().collect();
        let before = share(&english, &linked);
        let earned = share_edition(&english, &localized);
        assert_eq!(earned.done, before.done);
        assert_eq!(earned.library, before.library);
        assert_eq!(earned.uncredited, before.uncredited);
        assert_eq!(earned.done.game_c, 176 + 120);
        assert_eq!(earned.done.executable, 1000);
        assert_eq!(earned.steered, 176);
        assert_eq!(share_edition(&english, &english).steered, 120);
    }

    #[test]
    fn an_object_linked_in_four_of_six_editions_earns_four_sixths() {
        let unit = |image: &str, object: &str| (image.to_string(), object.to_string(), None);
        let c = |bytes, steered| Counted {
            done: GameDone {
                game_c: bytes,
                ..GameDone::default()
            },
            steered,
            ..Counted::default()
        };
        let stub = Counted {
            done: GameDone {
                common_asm: 8,
                veneers: 8,
                ..GameDone::default()
            },
            ..Counted::default()
        };
        let english = Measurement {
            done: GameDone {
                game_c: 600,
                common_asm: 8,
                veneers: 8,
                executable: 1000,
                ..GameDone::default()
            },
            steered: 60,
            raw: 392,
            credits: BTreeMap::from([
                (unit("main", "games/G/SRC/A.o"), c(300, 0)),
                (unit("main", "games/G/SRC/B.o"), c(240, 60)),
                (unit("36f", "games/G/SRC/B.o"), c(60, 0)),
                (unit("main", "games/COMMON/SRC/D.o"), stub),
            ]),
            ..Measurement::default()
        };
        let everything = english.credits.keys().cloned().collect::<BTreeSet<_>>();
        // Two editions still take A from their scaffold; one of them links B
        // in its main image but copies overlay 36f, and so earns nothing for
        // B there. A unit only another edition links is not in the total.
        let without_a = everything
            .iter()
            .filter(|unit| unit.1 != "games/G/SRC/A.o")
            .cloned()
            .collect::<BTreeSet<_>>();
        let mut without_a_or_overlay = without_a.clone();
        without_a_or_overlay.remove(&unit("36f", "games/G/SRC/B.o"));
        without_a_or_overlay.insert(unit("main", "games/G/SRC/ONLY_HERE.o"));
        let linked = [
            ("ja", &without_a_or_overlay),
            ("en", &everything),
            ("de", &everything),
            ("es", &everything),
            ("fr", &without_a),
            ("it", &everything),
        ];
        let game = Game {
            editions: linked
                .iter()
                .map(|(language, units)| (*language, share(&english, units)))
                .collect(),
            english,
            edition_credits: BTreeMap::new(),
            edition_data: BTreeMap::new(),
        };
        let bytes = |language: &str| {
            let edition = game.editions.iter().find(|(name, _)| *name == language);
            edition.unwrap().1.done.bytes()
        };
        assert_eq!((bytes("en"), bytes("fr"), bytes("ja")), (608, 308, 248));
        assert!(game
            .editions
            .iter()
            .all(|(_, edition)| edition.done.executable == 1000));
        let all = game.combined();
        // A earns four sixths of its 300 bytes; overlay B five sixths of 60.
        assert_eq!(all.done.game_c, 4 * 300 + 6 * 240 + 5 * 60);
        assert_eq!((all.done.common_asm, all.done.veneers), (48, 48));
        assert_eq!(all.done.executable, 6 * 1000);
        assert_eq!(all.steered, 6 * 60);
        assert_eq!(all.done.bytes(), 4 * 608 + 308 + 248);
        // The published parts still add up to the published whole.
        let (c, assembly, stubs) = all.done.parts();
        assert_eq!((c, assembly, stubs), (49.0, 0.0, 0.8));
        assert_eq!(all.done.percent(), 49.8);
    }

    fn native_module(root: &Path, target: DecompTarget) -> String {
        use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
        let source = root.join("games/G/SRC/MODULE.C");
        let object = root
            .join(target.output_dir)
            .join("obj/games/G/SRC/MODULE.o");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        let assembly = object.with_extension("s");
        let mut options = SourceToAssemblyPlanOptions::new(
            target.compiler,
            "games/G/SRC/MODULE.C",
            source.to_string_lossy(),
            assembly.to_string_lossy(),
        );
        options
            .preprocessor_flags
            .push(format!("-D{}=1", target.edition_define));
        for step in source_to_assembly_plan(&options).unwrap() {
            psynergy::process::run(&step, root).unwrap();
        }
        let mut assemble = crate::compiler::routing::assembly_command(
            &assembly.to_string_lossy(),
            &object.to_string_lossy(),
        );
        assemble[0] = crate::compiler::routing::binutils_prefix()
            .join("bin/arm-none-eabi-as")
            .to_string_lossy()
            .into_owned();
        psynergy::process::run(&assemble, root).unwrap();

        let scaffold = root
            .join(target.output_dir)
            .join("obj/recon/tbs/raw/scaffold.o");
        std::fs::create_dir_all(scaffold.parent().unwrap()).unwrap();
        let raw = scaffold.with_extension("s");
        let pending = if target.language() == "en" {
            ""
        } else {
            ".text\n.global Module_Pending\n.type Module_Pending,%function\n.thumb_func\nModule_Pending:\n bx lr\n.align 2\n"
        };
        std::fs::write(&raw, format!(".syntax unified\n.thumb\n{pending}.data\n.global gModulePendingValue\ngModulePendingValue:\n.word 7\n")).unwrap();
        let mut assemble = crate::compiler::routing::assembly_command(
            &raw.to_string_lossy(),
            &scaffold.to_string_lossy(),
        );
        assemble[0] = crate::compiler::routing::binutils_prefix()
            .join("bin/arm-none-eabi-as")
            .to_string_lossy()
            .into_owned();
        psynergy::process::run(&assemble, root).unwrap();
        let script = root.join("fixture.ld");
        std::fs::write(
            &script,
            "SECTIONS { . = 0x08000000; .text : { *(.text) } .data : { *(.data) } }\n",
        )
        .unwrap();
        let map = root.join(target.output_dir).join("fixture.map");
        let elf = map.with_extension("elf");
        let linker = crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-ld");
        psynergy::process::run(
            &[
                linker.to_string_lossy().into_owned(),
                "-T".into(),
                script.to_string_lossy().into_owned(),
                "-Map".into(),
                map.to_string_lossy().into_owned(),
                "-o".into(),
                elf.to_string_lossy().into_owned(),
                object.to_string_lossy().into_owned(),
                scaffold.to_string_lossy().into_owned(),
            ],
            root,
        )
        .unwrap();
        let bytes = std::fs::read(elf).unwrap();
        let file = object::File::parse(bytes.as_slice()).unwrap();
        assert!(file
            .symbols()
            .any(|symbol| symbol.name() == Ok("Module_Pending") && symbol.is_definition()));
        std::fs::read_to_string(map).unwrap()
    }

    #[test]
    fn a_partial_c_module_earns_only_its_source_definitions_in_the_same_image() {
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        std::fs::create_dir_all(root.join("games/G/SRC")).unwrap();
        std::fs::write(root.join("games/G/SRC/MODULE.C"),
            "int Module_Always(int x) {\n#if defined(TBS_EDITION_DE)\n/* FAKEMATCH: localized fixture device. */\n#endif\nreturn x + 1; }\n#if defined(TBS_EDITION_EN)\nextern int gModulePendingValue;\nint Module_Pending(int x) { /* FAKEMATCH: English fixture device. */ return gModulePendingValue + x; }\n#endif\n").unwrap();
        let en = crate::targets::decomp_target(Some("tbs-en")).unwrap();
        let de = crate::targets::decomp_target(Some("tbs-de")).unwrap();
        let english_map = native_module(root, en);
        let german_map = native_module(root, de);
        let source = |stem: &str| maintained_source(root, stem);
        let en_expansion = crate::compiler::preprocess::Expansion::new(root, en).unwrap();
        let de_expansion = crate::compiler::preprocess::Expansion::new(root, de).unwrap();
        let mark = |stem: &str| source_mark(root, &en_expansion, stem);
        let mut english = Measurement::default();
        let placed = tally(
            &mut english,
            &english_map,
            en.output_dir,
            MAIN_IMAGE,
            &source,
            &mark,
        )
        .unwrap();
        let always = english
            .credits
            .iter()
            .find(|(unit, _)| unit.2.as_deref() == Some("Module_Always"))
            .unwrap()
            .1
            .done
            .bytes();
        let pending = english
            .credits
            .iter()
            .find(|(unit, _)| unit.2.as_deref() == Some("Module_Pending"))
            .unwrap()
            .1
            .done
            .bytes();
        // Pending loads a global through its literal pool; its credited span
        // includes the complete compiler-emitted pool and section alignment.
        let object = root.join(en.output_dir).join("obj/games/G/SRC/MODULE.o");
        let bytes = std::fs::read(object).unwrap();
        let file = object::File::parse(bytes.as_slice()).unwrap();
        assert_eq!(
            always + pending,
            file.section_by_name(".text").unwrap().size() as i64
        );
        assert!(pending >= 8);
        let mut german = Measurement::default();
        tally(
            &mut german,
            &german_map,
            de.output_dir,
            MAIN_IMAGE,
            &source,
            &|stem| source_mark(root, &de_expansion, stem),
        )
        .unwrap();
        let earned = share_edition(&english, &german);
        assert_eq!(earned.done.game_c, always);
        assert_eq!(earned.steered, always);
        assert_eq!(earned.done.executable, always + pending);
        let mut other_image = Measurement::default();
        tally(
            &mut other_image,
            &german_map,
            de.output_dir,
            "36f",
            &source,
            &|stem| source_mark(root, &de_expansion, stem),
        )
        .unwrap();
        assert_eq!(share_edition(&english, &other_image).done.game_c, 0);
        assert_eq!(share_edition(&english, &other_image).steered, 0);
        assert_eq!(
            share_edition(&english, &english).done.game_c,
            always + pending
        );
        assert_eq!(share_edition(&english, &english).steered, pending);

        // A source mark spanning two contributions is removed from both,
        // including the steered part, rather than from just the first one.
        let pending_start = placed
            .iter()
            .find(|(_, _, unit)| unit.2.as_deref() == Some("Module_Pending"))
            .unwrap()
            .0;
        discredit(&mut english, &placed, &[(pending_start - 2, 4)]);
        assert_eq!(share_edition(&english, &german).done.game_c, always - 2);
        assert_eq!(share_edition(&english, &german).steered, always - 2);
        assert_eq!(
            share_edition(&english, &english).done.game_c,
            always + pending - 4
        );
        assert_eq!(share_edition(&english, &english).steered, pending - 2);
        assert_eq!(english.steered, pending - 2);
    }

    #[test]
    fn a_native_nested_definition_and_its_single_export_earn_one_extent() {
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        std::fs::create_dir_all(root.join("games/G/SRC")).unwrap();
        // The current compiler emits the nested body's local name as well
        // as the requested public definition. This fixture tests metadata,
        // without approving such aliases as game source under O2.
        std::fs::write(root.join("games/G/SRC/MODULE.C"),
            "extern int gModulePendingValue;\nextern void Module_Pending(void) __attribute__((alias(\"Nested.0\")));\nstatic __inline__ int Scope(void) { void Nested(void) { gModulePendingValue += 1; } return 0; }\n").unwrap();
        let target = crate::targets::decomp_target(Some("tbs-en")).unwrap();
        let map = native_module(root, target);
        let object = root
            .join(target.output_dir)
            .join("obj/games/G/SRC/MODULE.o");
        let bytes = std::fs::read(&object).unwrap();
        let file = object::File::parse(bytes.as_slice()).unwrap();
        let size = file.section_by_name(".text").unwrap().size() as i64;
        let functions = c_functions_from_object(&bytes, ".text", size).unwrap();
        assert_eq!(functions.len(), 1);
        assert_eq!(functions[0].name, "Module_Pending");
        assert_eq!(functions[0].bytes, size);
        assert_eq!(
            functions[0].names,
            ["Module_Pending".to_string(), "Nested.0".to_string()].into()
        );
        let source = |stem: &str| maintained_source(root, stem);
        let mut english = Measurement::default();
        tally(
            &mut english,
            &map,
            target.output_dir,
            MAIN_IMAGE,
            &source,
            &|_| {
                Ok(Mark {
                    steered: Steered::Functions(["Nested.0".to_string()].into()),
                    ..Mark::default()
                })
            },
        )
        .unwrap();
        assert_eq!(english.credits.len(), 1);
        assert_eq!(english.done.game_c, size);
        assert_eq!(english.steered, size);
        assert_eq!(share_edition(&english, &english).done.game_c, size);
        assert_eq!(share_edition(&english, &english).steered, size);
    }

    #[test]
    fn conflicting_native_public_names_or_function_extents_stay_refused() {
        let work = tempfile::tempdir().unwrap();
        for (name, metadata) in [
            (
                "two_public",
                ".global Other\n.thumb_set Other,Local\n.type Other,%function\n.size Other,8\n",
            ),
            ("different_size", ".size Public,4\n"),
            ("zero_size", ".size Public,0\n.size Local,0\n"),
            ("oversized", ".size Public,12\n.size Local,12\n"),
        ] {
            let source = work.path().join(format!("{name}.s"));
            let object = source.with_extension("o");
            std::fs::write(&source, format!(".syntax unified\n.thumb\n.text\n.type Local,%function\n.thumb_func\nLocal:\n bx lr\n .space 6\n.size Local,8\n.global Public\n.thumb_set Public,Local\n.type Public,%function\n.size Public,8\n{metadata}")).unwrap();
            let mut command = crate::compiler::routing::assembly_command(
                &source.to_string_lossy(),
                &object.to_string_lossy(),
            );
            command[0] = crate::compiler::routing::binutils_prefix()
                .join("bin/arm-none-eabi-as")
                .to_string_lossy()
                .into_owned();
            psynergy::process::run(&command, work.path()).unwrap();
            let bytes = std::fs::read(object).unwrap();
            assert!(
                c_functions_from_object(&bytes, ".text", 8)
                    .unwrap_err()
                    .contains("ambiguous C function extent"),
                "{name}"
            );
        }
    }

    #[test]
    fn c_credit_requires_complete_unambiguous_placed_function_metadata() {
        let work = tempfile::tempdir().unwrap();
        let object = work.path().join("fixture.o");
        fixture_object(&object, "Present", 8);
        let bytes = std::fs::read(&object).unwrap();
        assert_eq!(
            c_functions_from_object(&bytes, ".text", 8).unwrap(),
            [CFunction {
                offset: 0,
                bytes: 8,
                name: "Present".to_string(),
                names: ["Present".to_string()].into(),
            }]
        );
        assert!(c_functions_from_object(&bytes, ".text", 12)
            .unwrap_err()
            .contains("map/object extent differs"));
        assert!(c_functions_from_object(&bytes, ".text.absent", 8).is_err());
        let missing_path = work.path().join("missing.o");
        let missing = Placed {
            name: ".text",
            address: 0,
            size: 8,
            object: missing_path.to_str().unwrap(),
        };
        assert!(placed_c_functions(&missing).is_err());
        let no_functions = work.path().join("data.o");
        let source = no_functions.with_extension("s");
        std::fs::write(&source, ".text\n.word 0\n.word 0\n").unwrap();
        let assembler = crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-as");
        psynergy::process::run(
            &[
                assembler.to_string_lossy().into_owned(),
                source.to_string_lossy().into_owned(),
                "-o".into(),
                no_functions.to_string_lossy().into_owned(),
            ],
            work.path(),
        )
        .unwrap();
        assert!(
            c_functions_from_object(&std::fs::read(no_functions).unwrap(), ".text", 8)
                .unwrap_err()
                .contains("missing C function definition")
        );
    }

    #[test]
    fn a_tag_steers_only_the_function_it_sits_in_or_before() {
        let source = "\
#include \"GLOBAL.H\"
void Plain(void) { }
/* FAKEMATCH: order. */
void Before(void) { }
void Inside(void)
{
    /* FAKEMATCH: registers. */
    do { } while (0);
}
void After(void) { }
";
        let steered = crate::permute::parse::tagged_functions(source, "FAKEMATCH").unwrap();
        assert_eq!(
            steered.into_iter().collect::<Vec<_>>(),
            ["Before", "Inside"]
        );
        // A tag at file scope before a declaration, or inside a macro,
        // steers the whole object.
        let file_scope =
            "/* FAKEMATCH: pinned. */\nregister int r asm(\"r4\");\nvoid F(void) { }\n";
        assert_eq!(
            crate::permute::parse::tagged_functions(file_scope, "FAKEMATCH"),
            None
        );
        let macro_tag = "#define W(x) /* FAKEMATCH: wrap */ (x)\nvoid F(void) { }\n";
        assert_eq!(
            crate::permute::parse::tagged_functions(macro_tag, "FAKEMATCH"),
            None
        );
    }

    #[test]
    fn a_steered_function_runs_to_the_next_symbol_or_the_section_end() {
        let symbols = [
            (0, "Plain"),
            (0x10, "Before"),
            (0x24, "Inside"),
            (0x40, "After"),
        ];
        let names = ["Before", "After"].map(String::from).into_iter().collect();
        assert_eq!(function_spans(&symbols, &names, 0x50), 0x14 + 0x10);
        let aliases = [(0, "First"), (0, "Alias"), (8, "Next")];
        assert_eq!(
            function_spans(&aliases, &["First", "Alias"].map(String::from).into(), 12),
            8
        );
    }

    #[test]
    fn placed_section_selection_keeps_pools_and_ignores_other_offset_zero_functions() {
        let work = tempfile::tempdir().unwrap();
        let source = work.path().join("fixture.s");
        let object = work.path().join("fixture.o");
        std::fs::write(&source, ".syntax unified\n.thumb\n.section .text.first,\"ax\",%progbits\n.global First\n.type First,%function\n.thumb_func\nFirst:\n bx lr\n .align 2\nPool:\n .word 0\n.global Alias\n.type Alias,%function\n.set Alias,First\n.global Plain\n.type Plain,%function\n.thumb_func\nPlain:\n bx lr\n .align 2\n.section .text.other,\"ax\",%progbits\n.global Other\n.type Other,%function\n.thumb_func\nOther:\n bx lr\n .align 2\n .word 0\n").unwrap();
        let assembler = crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-as");
        let output = std::process::Command::new(assembler)
            .args(["-mcpu=arm7tdmi", "-meabi=gnu"])
            .arg(&source)
            .arg("-o")
            .arg(&object)
            .output()
            .unwrap();
        assert!(
            output.status.success(),
            "{}",
            String::from_utf8_lossy(&output.stderr)
        );
        let bytes = std::fs::read(object).unwrap();
        let names = ["First", "Alias", "Other"].map(String::from).into();
        assert_eq!(
            function_bytes_from_object(&bytes, ".text.first", &names, 12).unwrap(),
            8
        );
        assert_eq!(
            function_bytes_from_object(&bytes, ".text.other", &names, 8).unwrap(),
            8
        );
        assert!(function_bytes_from_object(&bytes, ".text.missing", &names, 8).is_err());
        assert!(function_bytes_from_object(&bytes, ".text.first", &names, 13).is_err());
    }

    #[test]
    fn names_are_counted_as_pret_counts_them() {
        let nm = "\
08000000 T Battle_Start
08000010 T Func_08000010
08000020 t Scene_Table_0800a0f0
08000024 T _call_via_r3
08000030 t $t
08000040 T Func_08000010
0200a000 D Data_0200a000
";
        let counted = names(nm);
        assert_eq!(
            counted,
            Names {
                total: 4,
                undocumented: 2,
                partial: 1
            }
        );
        assert_eq!(counted.documented(), 1);
    }

    #[test]
    fn listings_count_only_in_overlay_maps_and_sources_must_exist() {
        let source = |_: &str| None;
        let listing = "/r/out/tbs-en/overlays/resource_3a0_overlay.o";
        assert_eq!(
            origin(listing, "out/tbs-en", true, &source).unwrap(),
            Origin::Listing
        );
        assert_eq!(
            origin(listing, "out/tbs-en", false, &source).unwrap(),
            Origin::Other
        );
        assert_eq!(
            origin(
                "/r/out/tbs-en/overlays/resource_x_overlay.o",
                "out/tbs-en",
                true,
                &source
            )
            .unwrap(),
            Origin::Other
        );
        assert!(origin(
            "/r/out/tbs-en/obj/games/G/SRC/GONE.o",
            "out/tbs-en",
            false,
            &source
        )
        .unwrap_err()
        .contains("no maintained source"));
        // Another target's objects are never this target's source.
        assert_eq!(
            origin(
                "/r/out/tla-en/obj/games/G/SRC/A.o",
                "out/tbs-en",
                false,
                &language
            )
            .unwrap(),
            Origin::Other
        );
    }

    #[test]
    fn a_game_is_measured_only_from_its_verified_image() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
        let image = b"linked image";
        let digest = Sha1::digest(image)
            .iter()
            .map(|byte| format!("{byte:02x}"))
            .collect::<String>();
        assert!(verified(root, target).is_err());
        std::fs::write(
            root.join("rom.sha1"),
            format!("{digest}  out/tla-en/tla-en.gba\n"),
        )
        .unwrap();
        assert!(measure(root, target).unwrap().is_err());
        let output = root.join("out/tla-en");
        std::fs::create_dir_all(output.join("overlays")).unwrap();
        std::fs::create_dir_all(root.join("games/G/SRC")).unwrap();
        std::fs::create_dir_all(root.join("recon/tla")).unwrap();
        std::fs::write(root.join("games/G/SRC/A.C"), "void A(void) {}\n").unwrap();
        // The main image reads overlay 001 as a built stream.
        std::fs::write(
            root.join("recon/tla/overlays.s"),
            ".section .overlays,\"a\"\n.incbin \"overlays/resource_001.lz\"\n",
        )
        .unwrap();
        let main = "Linker script and memory map\n \
             .text 0x08000000 0x30 /x/out/tla-en/obj/games/G/SRC/A.o\n \
             .text 0x08000030 0x10 /x/out/tla-en/obj/recon/tla/raw/08000030.o\n \
             .overlays 0x08000040 0x20 /x/out/tla-en/obj/recon/tla/overlays.o\n";
        let overlay = "Linker script and memory map\n \
             .text 0x02000000 0x40 /x/out/tla-en/overlays/resource_001_overlay.o\n";
        let main = main.replace("/x/", &format!("{}/", root.display()));
        let overlay = overlay.replace("/x/", &format!("{}/", root.display()));
        fixture_c_objects(&main, "out/tla-en", &|stem| maintained_source(root, stem));
        std::fs::write(output.join("tla-en.map"), &main).unwrap();
        std::fs::write(output.join("overlays/resource_001.map"), &overlay).unwrap();
        std::fs::write(output.join("overlays/resource_001.lz"), b"stream").unwrap();
        // A map an older build left beside an overlay no stream reads.
        std::fs::write(output.join("overlays/resource_002.map"), &overlay).unwrap();
        std::fs::write(output.join("tla-en.gba"), b"another image").unwrap();
        assert_eq!(
            measure(root, target).unwrap().unwrap_err(),
            "pending: out/tla-en/tla-en.gba differs from rom.sha1"
        );
        std::fs::write(output.join("tla-en.gba"), image).unwrap();
        let measured = measure(root, target).unwrap().unwrap();
        assert_eq!(
            measured.done,
            GameDone {
                game_c: 0x30,
                executable: 0x80,
                ..GameDone::default()
            }
        );
        assert_eq!((measured.raw, measured.listings), (0x10, 0x40));
        assert_eq!(measured.done.percent(), 37.5);
        // A map rewritten by a link that did not produce a new image.
        std::thread::sleep(std::time::Duration::from_millis(20));
        std::fs::write(output.join("tla-en.map"), &main).unwrap();
        assert!(measure(root, target)
            .unwrap()
            .unwrap_err()
            .contains("newer than its verified image"));
    }

    /// Write a verified build of `id` under `root` and return its `rom.sha1`
    /// line: its main map, the map and stream of each overlay, and last its
    /// image. `OUT` in a map stands for the build's own directory.
    fn write_build(root: &Path, id: &str, main: &str, overlays: &[(&str, &str)]) -> String {
        let output = root.join(format!("out/{id}"));
        let own = |map: &str| map.replace("OUT", &format!("{}/out/{id}", root.display()));
        std::fs::create_dir_all(output.join("overlays")).unwrap();
        let main = own(main);
        fixture_c_objects(&main, &format!("out/{id}"), &|stem| {
            maintained_source(root, stem)
        });
        std::fs::write(output.join(format!("{id}.map")), main).unwrap();
        for (overlay, map) in overlays {
            let path = output.join(format!("overlays/resource_{overlay}"));
            let map = own(map);
            fixture_c_objects(&map, &format!("out/{id}"), &|stem| {
                maintained_source(root, stem)
            });
            std::fs::write(path.with_extension("map"), map).unwrap();
            std::fs::write(path.with_extension("lz"), b"stream").unwrap();
        }
        let image = format!("linked image of {id}");
        std::fs::write(output.join(format!("{id}.gba")), &image).unwrap();
        let digest = Sha1::digest(image.as_bytes())
            .iter()
            .map(|byte| format!("{byte:02x}"))
            .collect::<String>();
        format!("{digest}  out/{id}/{id}.gba\n")
    }

    #[test]
    fn a_game_counts_its_six_editions_and_is_pending_while_any_is_stale() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        std::fs::create_dir_all(root.join("games/G/SRC")).unwrap();
        for source in ["A", "B", "F"] {
            let text = format!("void {source}(void) {{}}\n");
            std::fs::write(root.join(format!("games/G/SRC/{source}.C")), text).unwrap();
        }
        // Every edition but German reads overlay 001 as a built stream;
        // German still copies it from its scaffold.
        let stream = ".section .overlays,\"a\"\n.incbin \"overlays/resource_001.lz\"\n";
        let copied = ".section .overlays,\"a\"\n.incbin \"baserom.gba\", 0x60, 0x20\n";
        std::fs::create_dir_all(root.join("recon/tla")).unwrap();
        std::fs::write(root.join("recon/tla/overlays.s"), stream).unwrap();
        for language in ["ja", "de", "es", "fr", "it"] {
            let directory = root.join(format!("recon/tla/{language}"));
            std::fs::create_dir_all(&directory).unwrap();
            let text = if language == "de" { copied } else { stream };
            std::fs::write(directory.join("overlays.s"), text).unwrap();
        }
        let main = |b: &str, streams: &str| {
            format!(
                "Linker script and memory map\n \
                 .text 0x08000000 0x30 OUT/obj/games/G/SRC/A.o\n \
                 {b}\n \
                 .text 0x08000050 0x10 OUT/obj/recon/tla/raw/08000050.o\n \
                 .overlays 0x08000060 0x20 OUT/obj/recon/tla/{streams}.o\n"
            )
        };
        let from_source = ".text 0x08000030 0x20 OUT/obj/games/G/SRC/B.o";
        let overlay = "Linker script and memory map\n \
             .text 0x02000000 0x20 OUT/overlays/resource_001_overlay.o\n \
             .text 0x02000020 0x40 OUT/obj/games/G/SRC/F.o\n \
             .rodata 0x02000060 0x18 OUT/obj/games/G/SRC/F.o\n";
        let built = [("001", overlay)];
        // Japanese still takes B from its scaffold.
        let japanese = main(
            ".rom.08000030 0x08000030 0x20 OUT/obj/recon/tla/ja/rom.o",
            "ja/overlays",
        );
        let mut digests =
            String::from("0000000000000000000000000000000000000000  out/tbs-en/tbs-en.gba\n");
        digests += &write_build(root, "tla-ja", &japanese, &built);
        digests += &write_build(root, "tla-en", &main(from_source, "overlays"), &built);
        // German's overlay map is what an older build left: nothing reads it.
        digests += &write_build(root, "tla-de", &main(from_source, "de/overlays"), &built);
        for language in ["es", "fr", "it"] {
            let streams = format!("{language}/overlays");
            let id = format!("tla-{language}");
            digests += &write_build(root, &id, &main(from_source, &streams), &built);
        }
        std::fs::write(root.join("rom.sha1"), &digests).unwrap();
        let target = crate::targets::decomp_target(Some("tla-en")).unwrap();
        let game = measure_game(root, target).unwrap().unwrap();
        let data_key = (
            "001".to_string(),
            "games/G/SRC/F.o".to_string(),
            ".rodata".to_string(),
        );
        assert_eq!(game.english.data_source, 0x18);
        for language in ["ja", "en", "es", "fr", "it"] {
            assert_eq!(game.edition_data[language][&data_key], 0x18);
        }
        // The old German overlay map and built file still exist, but its
        // main image copies the scaffold. Neither code nor data earns credit.
        assert!(game.edition_data["de"].is_empty());
        assert_eq!(game.english.done.executable, 0xc0);
        assert_eq!(
            game.editions
                .iter()
                .map(|(language, edition)| (*language, edition.done.bytes()))
                .collect::<Vec<_>>(),
            [
                ("ja", 0x30 + 0x40),
                ("en", 0x30 + 0x20 + 0x40),
                ("de", 0x30 + 0x20),
                ("es", 0x90),
                ("fr", 0x90),
                ("it", 0x90)
            ]
        );
        let all = game.combined().done;
        assert_eq!((all.bytes(), all.executable), (768, 6 * 0xc0));
        assert_eq!(all.percent(), 66.66);
        let subject = || crate::coverage::progress::subject(root).unwrap();
        assert_eq!(subject(), "☀️ pending ⚓️ 66.66% –");
        let pending = || measure_game(root, target).unwrap().unwrap_err();
        // One edition whose image is not its reference leaves the game unpublished.
        let italian = root.join("out/tla-it/tla-it.gba");
        std::fs::write(&italian, "another image").unwrap();
        assert_eq!(
            pending(),
            "pending: out/tla-it/tla-it.gba differs from rom.sha1"
        );
        assert_eq!(subject(), "☀️ pending ⚓️ pending –");
        std::fs::write(&italian, "linked image of tla-it").unwrap();
        assert_eq!(subject(), "☀️ pending ⚓️ 66.66% –");
        // So does a map newer than its image, a stream without its map, or
        // an edition that was never built.
        std::thread::sleep(std::time::Duration::from_millis(20));
        let french = root.join("out/tla-fr/tla-fr.map");
        std::fs::write(&french, std::fs::read(&french).unwrap()).unwrap();
        assert_eq!(
            pending(),
            "pending: out/tla-fr/tla-fr.map is newer than its verified image; rebuild"
        );
        std::fs::write(root.join("out/tla-fr/tla-fr.gba"), "linked image of tla-fr").unwrap();
        std::fs::remove_file(root.join("out/tla-es/overlays/resource_001.map")).unwrap();
        assert_eq!(
            pending(),
            "pending: out/tla-es/overlays/resource_001.map is not built beside the image that reads it; rebuild"
        );
        std::fs::remove_file(root.join("out/tla-ja/tla-ja.gba")).unwrap();
        assert_eq!(pending(), "pending a build of out/tla-ja/tla-ja.gba");
        // An edition rom.sha1 does not name is an error, never a number.
        std::fs::write(
            root.join("rom.sha1"),
            digests.replace("out/tla-ja/", "out/tla-xx/"),
        )
        .unwrap();
        assert!(measure_game(root, target).is_err());
    }
}
