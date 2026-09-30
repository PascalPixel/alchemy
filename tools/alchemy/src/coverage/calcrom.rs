//! pret's calcrom over the linker's own maps. Every executable byte of a
//! verified build is an input `text` section the map places; the object that
//! supplied it says whether it is maintained source under `games/`, a proven
//! compiler-library member, or disassembly not yet in C. Nothing else is read:
//! no catalog, no receipt and no guess at what is code.
use super::progress::GameDone;
use crate::targets::DecompTarget;
use sha1::{Digest, Sha1};
use std::path::Path;

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
    /// Data still placed from scaffolding: baserom ranges and listing data.
    pub data_scaffold: i64,
    /// The main image's symbol names, as pret's calcrom counts them.
    pub names: Names,
    /// Text of C objects whose source carries a FAKEMATCH tag: counted in
    /// DONE until both games are done (AGENTS.md S2), shown on its own.
    pub steered: i64,
    /// Padding a source marks as carrying no credit, between its
    /// `AlchemyUncredited_X` and `AlchemyUncreditedEnd_X` labels: placed,
    /// but not counted in DONE (C2).
    pub uncredited: i64,
}

/// What a `games/` source says about its object's bytes.
#[derive(Clone, Copy, Debug, Default, PartialEq, Eq)]
pub(crate) struct Mark {
    /// C steered by a FAKEMATCH workaround.
    pub steered: bool,
    /// Assembly credited as whole 8-byte stubs (`@ credit: reconstructed_veneer`).
    pub veneer: bool,
}

/// Read the marks of the maintained source a `games/` object was built from.
fn source_mark(root: &Path, stem: &str) -> Mark {
    ["C", "c", "S", "s"]
        .iter()
        .find_map(|extension| {
            std::fs::read_to_string(root.join(format!("{stem}.{extension}"))).ok()
        })
        .map_or_else(Mark::default, |text| Mark {
            steered: text.contains("FAKEMATCH"),
            veneer: text.contains("@ credit: reconstructed_veneer"),
        })
}

/// The bytes between each `AlchemyUncredited_X` label and its
/// `AlchemyUncreditedEnd_X` in one image's `nm` output.
pub(crate) fn uncredited_bytes(nm: &str) -> i64 {
    let mut starts = std::collections::HashMap::new();
    let mut ends = std::collections::HashMap::new();
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
    starts
        .iter()
        .filter_map(|(key, start)| ends.get(key).map(|end| (end - start).max(0)))
        .sum()
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

/// Every input section the map places: name, size and object path. The
/// discarded sections listed before the memory map are skipped.
fn sections(map: &str) -> Vec<(&str, i64, &str)> {
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
            found.push((name, placed.0, placed.1));
        }
    }
    found
}

/// `  0xADDRESS  0xSIZE  object`, as the map places an input section.
fn placement(text: &str) -> Option<(i64, &str)> {
    let hex = |field: &str| {
        field
            .strip_prefix("0x")
            .and_then(|digits| i64::from_str_radix(digits, 16).ok())
    };
    let text = text.trim_start();
    let (address, rest) = text.split_once(char::is_whitespace)?;
    hex(address)?;
    let rest = rest.trim_start();
    let (size, object) = rest.split_once(char::is_whitespace)?;
    let object = object.trim();
    (!object.is_empty()).then_some(())?;
    Some((hex(size)?, object))
}

/// An object's path under its build directory's `obj/` (or, in an overlay
/// map, under `overlays/`), and whether it sits in the overlay directory.
fn relative_object<'a>(object: &'a str, output: &str, overlay: bool) -> Option<(&'a str, bool)> {
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

/// Add one map's text sections to `measurement`.
fn tally(
    measurement: &mut Measurement,
    map: &str,
    output: &str,
    overlay: bool,
    source: &dyn Fn(&str) -> Option<Language>,
    mark: &dyn Fn(&str) -> Mark,
) -> Result<(), String> {
    for (name, size, object) in sections(map) {
        if size > 0 && is_data(name) {
            match origin(object, output, overlay, source)? {
                Origin::CommonC | Origin::CommonAsm | Origin::GameC | Origin::GameAsm => {
                    measurement.data_source += size
                }
                _ => measurement.data_scaffold += size,
            }
            continue;
        }
        if size <= 0 || !name.contains("text") {
            continue;
        }
        let marked = || {
            relative_object(object, output, overlay)
                .and_then(|(path, _)| path.strip_suffix(".o"))
                .map_or_else(Mark::default, mark)
        };
        match origin(object, output, overlay, source)? {
            origin @ (Origin::CommonC | Origin::GameC) => {
                if origin == Origin::CommonC {
                    measurement.done.common_c += size;
                } else {
                    measurement.done.game_c += size;
                }
                if marked().steered {
                    measurement.steered += size;
                }
            }
            origin @ (Origin::CommonAsm | Origin::GameAsm) => {
                if origin == Origin::CommonAsm {
                    measurement.done.common_asm += size;
                } else {
                    measurement.done.game_asm += size;
                }
                if marked().veneer {
                    measurement.done.veneers += size;
                }
            }
            Origin::Library => {
                measurement.done.game_asm += size;
                measurement.library += size;
            }
            Origin::Raw => measurement.raw += size,
            Origin::Listing => measurement.listings += size,
            Origin::Other => match measurement
                .other
                .iter_mut()
                .find(|(path, _)| path == object)
            {
                Some((_, bytes)) => *bytes += size,
                None => measurement.other.push((object.to_owned(), size)),
            },
        }
        measurement.done.executable += size;
    }
    Ok(())
}

/// The language `build rom` compiled a `games/` object from.
fn maintained_source(root: &Path, stem: &str) -> Option<Language> {
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

/// A game's measurement from its verified build, or why it is pending.
pub(crate) fn measure(
    root: &Path,
    target: DecompTarget,
) -> Result<Result<Measurement, String>, String> {
    if let Err(reason) = verified(root, target)? {
        return Ok(Err(reason));
    }
    let output = target.output_dir;
    let image = root.join(image(target));
    let written = |path: &Path| {
        std::fs::metadata(path)
            .and_then(|metadata| metadata.modified())
            .map_err(|error| format!("{}: {error}", path.display()))
    };
    let linked = written(&image)?;
    let mut maps = vec![(root.join(format!("{output}/{}.map", target.id)), false)];
    let overlays = root.join(output).join("overlays");
    if overlays.is_dir() {
        let mut names = std::fs::read_dir(&overlays)
            .map_err(|error| format!("{}: {error}", overlays.display()))?
            .filter_map(|entry| entry.ok())
            .map(|entry| entry.file_name().to_string_lossy().into_owned())
            .filter(|name| name.starts_with("resource_") && name.ends_with(".map"))
            .collect::<Vec<_>>();
        names.sort();
        maps.extend(names.into_iter().map(|name| (overlays.join(name), true)));
    }
    let source = |stem: &str| maintained_source(root, stem);
    let marks = std::cell::RefCell::new(std::collections::HashMap::new());
    let mark = |stem: &str| {
        *marks
            .borrow_mut()
            .entry(stem.to_string())
            .or_insert_with(|| source_mark(root, stem))
    };
    let mut measurement = Measurement::default();
    let mut images = vec![root.join(format!("{output}/{}.elf", target.id))];
    for (path, overlay) in maps {
        if overlay {
            images.push(path.with_extension("elf"));
        }
        // A link that failed after writing its map leaves an older image.
        if written(&path)? > linked {
            return Ok(Err(format!(
                "pending: {} is newer than its verified image; rebuild",
                path.strip_prefix(root).unwrap_or(&path).display()
            )));
        }
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        tally(&mut measurement, &text, output, overlay, &source, &mark)?;
    }
    // The main image names its symbols; every image may place padding its
    // source marks as uncredited, which leaves DONE (C2).
    for (index, image) in images.iter().enumerate() {
        let Ok(nm) = std::process::Command::new("arm-none-eabi-nm")
            .arg(image)
            .output()
        else {
            continue;
        };
        if !nm.status.success() {
            continue;
        }
        let listed = String::from_utf8_lossy(&nm.stdout);
        if index == 0 {
            measurement.names = names(&listed);
        }
        measurement.uncredited += uncredited_bytes(&listed);
    }
    measurement.done.game_asm -= measurement.uncredited.min(measurement.done.game_asm);
    for (object, bytes) in &measurement.other {
        eprintln!(
            "{}: {bytes} text bytes from unclassified {object}",
            target.id
        );
    }
    Ok(Ok(measurement))
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
        let mut measurement = Measurement::default();
        let mark = |stem: &str| Mark {
            steered: stem == "games/G/SRC/A",
            veneer: stem == "games/COMMON/SRC/D",
        };
        tally(
            &mut measurement,
            MAIN,
            "out/tbs-en",
            false,
            &language,
            &mark,
        )
        .unwrap();
        tally(
            &mut measurement,
            OVERLAY,
            "out/tbs-en",
            true,
            &language,
            &mark,
        )
        .unwrap();
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
                other: vec![("/r/out/tbs-en/obj/recon/tbs/stray.o".into(), 4)],
                // A's rodata and data come from source; the baserom range
                // is scaffolding.
                data_source: 0x80 + 0x10,
                data_scaffold: 0x100,
                names: Names::default(),
                steered: 0x100,
                uncredited: 0,
            }
        );
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
        // Two closed spans of 4 and 6 bytes; a start with no end counts nothing.
        assert_eq!(uncredited_bytes(nm), 10);
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
        std::fs::write(root.join("games/G/SRC/A.C"), "void A(void) {}\n").unwrap();
        let main = "Linker script and memory map\n \
             .text 0x08000000 0x30 /x/out/tla-en/obj/games/G/SRC/A.o\n \
             .text 0x08000030 0x10 /x/out/tla-en/obj/recon/tla/raw/08000030.o\n";
        let overlay = "Linker script and memory map\n \
             .text 0x02000000 0x40 /x/out/tla-en/overlays/resource_001_overlay.o\n";
        std::fs::write(output.join("tla-en.map"), main).unwrap();
        std::fs::write(output.join("overlays/resource_001.map"), overlay).unwrap();
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
        std::fs::write(output.join("tla-en.map"), main).unwrap();
        assert!(measure(root, target)
            .unwrap()
            .unwrap_err()
            .contains("newer than its verified image"));
    }
}
