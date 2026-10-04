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
    } else if text.contains("FAKEMATCH") {
        // A tagged assembly source counts as steered as a whole.
        Steered::Whole
    } else {
        Steered::None
    };
    Ok(Mark {
        steered,
        veneer: text.contains("@ credit: reconstructed_veneer"),
    })
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
        let Placed { size, object, .. } = placed;
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
        discredit(&mut measurement, &placed, &uncredited_spans(&listed));
    }
    measurement.done.game_asm -= measurement.stray.min(measurement.done.game_asm);
    for (object, bytes) in &measurement.other {
        eprintln!(
            "{}: {bytes} text bytes from unclassified {object}",
            target.id
        );
    }
    Ok(Ok(measurement))
}

/// A game's DONE in all six editions together, measured on its English
/// build `target`, or why it is pending: every edition's build must be
/// verified and current. The six editions are measured at once; the first
/// failure in edition order, English first, is the one reported.
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
    for (edition, measurement) in measured {
        let earned = if edition.id == target.id {
            share_edition(&english, &english)
        } else {
            match measurement? {
                Ok(measurement) => share_edition(&english, &measurement),
                Err(reason) => return Ok(Err(reason)),
            }
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
    Ok(Ok(Game { english, editions }))
}
