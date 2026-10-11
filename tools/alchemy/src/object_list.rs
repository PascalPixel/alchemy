//! A game's object list, games/<game>/objects.tsv: what each image links,
//! one linker statement per row, in link order.
//!
//! Columns: `image` (`main`, or an overlay's resource id such as `3ad`),
//! `place` (the output section, or its span between two fixed placements),
//! `object` (a path from the checkout without `.o`, an archive member such
//! as `libgcc.a:_call_via_rX`, an overlay's own `resource_<id>_overlay`, or
//! `*` for every object), `sections` (blank means
//! `.text .rodata .data .padding`) and `editions` (blank means all six, else
//! the languages that link the row, such as `ja en`). An object may take
//! several rows: one for each place that takes some of its sections.
//!
//! An overlay's places are `text` (its code and data at 0x2008000), `bss`
//! and `discard`. The table beside the list, games/<game>/overlays.tsv, has
//! a row for each overlay with a `bss` place: columns `overlay` (its id)
//! and `bss`, the header of its `.bss` output section as the linker reads
//! it: `.bss`, `.bss (NOLOAD)`, `.bss ALIGN(16) (NOLOAD)` or
//! `.bss ALIGN(8)`. An overlay asks libgcc for `_call_via_r0`
//! (`EXTERN`) when a row links `libgcc.a:_call_via_rX`.
//!
//! The build links by the list. The game's one main.ld and its one
//! overlay.ld name no object: each has an `INCLUDE "<name>.ld"` for each
//! place, and the build writes those fragments for each edition from the
//! rows that edition links (out/<edition>/ld for the main image,
//! out/<edition>/ld/<id> for an overlay). `alchemy check object-list`
//! holds the list to what the linker can read.

use crate::edition::{Image, Item, Statement};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

pub(crate) const HEADER: &str = "image\tplace\tobject\tsections\teditions";
pub(crate) const DEFAULT_SECTIONS: &str = ".text .rodata .data .padding";
pub(crate) const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
pub(crate) const MAIN: &str = "main";
/// The object column of a row for every object: `*(.bss COMMON)`.
const EVERY: &str = "*";
/// The places of an overlay: its code and data at 0x2008000, its
/// uninitialized data after them, and the sections it leaves out.
const OVERLAY_PLACES: [&str; 3] = ["text", "bss", "discard"];
/// The archive member an overlay links to call through a register, and the
/// symbol that pulls it from the archive.
const CALL_VIA: &str = "libgcc.a:_call_via_rX";
pub(crate) const BSS_HEADER: &str = "overlay\tbss";

const USAGE: &str = "usage: alchemy check object-list\n\
Fails when a game's object list (games/<game>/objects.tsv) is not what its\n\
build can link: a malformed row, an object with no C, assembly or MIDI\n\
source, an input section of an object given twice to one edition, a place\n\
the game's main.ld or overlay.ld does not INCLUDE, a fragment included\n\
outside the place it is named for, or an overlay whose bss rows and .bss\n\
header in games/<game>/overlays.tsv do not go together; or when the\n\
assembler modules linked after the resource directory, with the sources\n\
they include, do not define its resources in the order its rows first name\n\
them. A place is an\n\
output section (rom_start; text, bss and discard in an overlay), or the\n\
span of one between two fixed placements, numbered from the section's start\n\
(rom_main_00, rom_main_01 ...). A game without a list is not checked.";

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Row {
    pub image: String,
    pub place: String,
    pub object: String,
    /// The input sections, with the blank default written out.
    pub sections: String,
    /// The languages that link this row, in LANGUAGES' order.
    pub editions: Vec<&'static str>,
}

impl Row {
    fn links(&self, image: &str, language: &str) -> bool {
        self.image == image && self.editions.contains(&language)
    }

    /// The row as a linker statement: `"*/OBJECT.o"(SECTIONS)`, or
    /// `"*ARCHIVE:MEMBER.o"(SECTIONS)` for an archive member.
    pub(crate) fn statement(&self) -> String {
        statement(&self.object, &self.sections)
    }

    /// The object the build makes from a source in the checkout.
    fn built(&self) -> bool {
        !self.object.contains([':', '*'])
    }
}

fn statement(object: &str, sections: &str) -> String {
    if object == EVERY {
        return format!("*({sections})");
    }
    let path = if object.contains(':') { "" } else { "/" };
    format!("\"*{path}{object}.o\"({sections})")
}

fn language(name: &str) -> Result<&'static str, String> {
    LANGUAGES
        .iter()
        .copied()
        .find(|known| *known == name)
        .ok_or_else(|| format!("{name:?} is not one of {}", LANGUAGES.join(" ")))
}

pub(crate) fn parse(text: &str) -> Result<Vec<Row>, String> {
    let mut lines = text.lines().enumerate();
    match lines.next() {
        Some((_, HEADER)) => {}
        _ => return Err(format!("line 1: the header is {HEADER:?}")),
    }
    lines
        .map(|(index, line)| row(line).map_err(|error| format!("line {}: {error}", index + 1)))
        .collect()
}

fn row(line: &str) -> Result<Row, String> {
    let fields: Vec<&str> = line.split('\t').collect();
    let (image, place, object) = match fields[..] {
        [image, place, object, ..] if fields.len() <= 5 => (image, place, object),
        _ => return Err("a row has three to five columns".into()),
    };
    let name = |text: &str| {
        !text.is_empty()
            && text
                .bytes()
                .all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
    };
    if image != MAIN && !(image.len() == 3 && image.bytes().all(|byte| byte.is_ascii_hexdigit()) && name(image)) {
        return Err(format!("image {image:?} is neither main nor an overlay id"));
    }
    if !name(place) {
        return Err(format!("place {place:?} is not a lower-case name"));
    }
    let plain = |text: &str| !text.is_empty() && !text.contains(|c: char| c.is_whitespace() || "\"*(){};".contains(c));
    if object != EVERY && (!plain(object) || object.ends_with(".o")) {
        return Err(format!("object {object:?} is not a path or archive member without .o"));
    }
    if image != MAIN && !OVERLAY_PLACES.contains(&place) {
        return Err(format!("place {place:?} is not one of an overlay's: {}", OVERLAY_PLACES.join(" ")));
    }
    let sections = match fields.get(3).copied().unwrap_or("") {
        "" => DEFAULT_SECTIONS.to_owned(),
        listed => {
            let names: Vec<&str> = listed.split(' ').collect();
            // A section name may be a pattern: `.text.*`.
            if names.iter().any(|name| !name.starts_with('.') && *name != "COMMON" || !plain(&name.replace('*', ""))) {
                return Err(format!("sections {listed:?} are not section names one space apart"));
            }
            listed.to_owned()
        }
    };
    let editions = match fields.get(4).copied().unwrap_or("") {
        "" => LANGUAGES.to_vec(),
        listed => {
            let mut found = Vec::new();
            for name in listed.split(' ') {
                let known = LANGUAGES
                    .iter()
                    .position(|known| *known == name)
                    .ok_or_else(|| format!("{name:?} is not one of {}", LANGUAGES.join(" ")))?;
                if found.last().is_some_and(|last| *last >= known) {
                    return Err(format!("editions {listed:?} are not in the order {}", LANGUAGES.join(" ")));
                }
                found.push(known);
            }
            if found.len() == LANGUAGES.len() {
                return Err("all six editions are written as a blank column".into());
            }
            found.into_iter().map(|index| LANGUAGES[index]).collect()
        }
    };
    Ok(Row {
        image: image.to_owned(),
        place: place.to_owned(),
        object: object.to_owned(),
        sections,
        editions,
    })
}

/// The list as its file's text: what `parse` reads back as the same rows.
pub(crate) fn format(rows: &[Row]) -> String {
    let mut text = format!("{HEADER}\n");
    for row in rows {
        let sections = if row.sections == DEFAULT_SECTIONS { "" } else { &row.sections };
        let editions = if row.editions.len() == LANGUAGES.len() { String::new() } else { row.editions.join(" ") };
        let mut fields = vec![row.image.as_str(), &row.place, &row.object, sections, &editions];
        while fields.last().is_some_and(|field| field.is_empty()) {
            fields.pop();
        }
        text += &fields.join("\t");
        text.push('\n');
    }
    text
}

/// A game's list, or None for a game that links by scripts naming their own
/// objects.
pub(crate) fn read(root: &Path, target: DecompTarget) -> Result<Option<Vec<Row>>, String> {
    let Some(path) = target.object_list() else {
        return Ok(None);
    };
    let text = fs::read_to_string(root.join(&path)).map_err(|error| format!("{}: {error}", path.display()))?;
    parse(&text).map(Some).map_err(|error| format!("{}: {error}", path.display()))
}

/// The overlay table beside a game's list, games/<game>/overlays.tsv: for
/// each overlay with uninitialized data, the header its `.bss` output
/// section takes. A game whose overlays have none needs no table.
pub(crate) fn read_bss(root: &Path, target: DecompTarget) -> Result<BTreeMap<String, String>, String> {
    let Some(path) = target.object_list().map(|list| list.with_file_name("overlays.tsv")) else {
        return Ok(BTreeMap::new());
    };
    match fs::read_to_string(root.join(&path)) {
        Ok(text) => parse_bss(&text).map_err(|error| format!("{}: {error}", path.display())),
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => Ok(BTreeMap::new()),
        Err(error) => Err(format!("{}: {error}", path.display())),
    }
}

pub(crate) fn parse_bss(text: &str) -> Result<BTreeMap<String, String>, String> {
    let mut lines = text.lines().enumerate();
    match lines.next() {
        Some((_, BSS_HEADER)) => {}
        _ => return Err(format!("line 1: the header is {BSS_HEADER:?}")),
    }
    let mut found = BTreeMap::new();
    for (index, line) in lines {
        let at = |error: String| format!("line {}: {error}", index + 1);
        let Some((overlay, header)) = line.split_once('\t') else {
            return Err(at("a row has an overlay and its .bss header".into()));
        };
        if !(overlay.len() == 3 && overlay.bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))) {
            return Err(at(format!("{overlay:?} is no overlay id")));
        }
        // The forms Camelot's overlays show: an alignment, and whether the
        // section is marked as never loaded.
        let rest = header.strip_prefix(".bss").ok_or_else(|| at(format!("{header:?} is no .bss header")))?;
        let rest = rest.strip_suffix(" (NOLOAD)").unwrap_or(rest);
        if !matches!(rest, "" | " ALIGN(8)" | " ALIGN(16)") {
            return Err(at(format!("{header:?} is not .bss, an optional ALIGN(8) or ALIGN(16), and an optional (NOLOAD)")));
        }
        if found.insert(overlay.to_owned(), header.to_owned()).is_some() {
            return Err(at(format!("overlay {overlay} is given twice")));
        }
    }
    Ok(found)
}

/// One edition's linker fragments for an overlay, which the shared
/// overlay.ld includes: `extern` (the symbol that pulls the call-through-
/// register member from libgcc, when a row links that member), `text` and
/// `discard` (those places' statements) and `bss` (the whole `.bss` output
/// section under the header the overlay table gives it, or nothing).
pub(crate) fn overlay_fragments(
    rows: &[Row],
    bss: &BTreeMap<String, String>,
    id: &str,
    language: &str,
) -> Result<Vec<(String, String)>, String> {
    let mut places = render(rows, id, language);
    if places.is_empty() {
        return Err(format!("the object list has no row for overlay {id}"));
    }
    let mut take = |name: &str| {
        places
            .iter()
            .position(|(place, _)| place == name)
            .map_or(String::new(), |at| places.remove(at).1)
    };
    let (text, uninitialized, discard) = (take("text"), take("bss"), take("discard"));
    let calls = rows.iter().any(|row| row.links(id, language) && row.object == CALL_VIA);
    let section = match (bss.get(id), uninitialized.is_empty()) {
        (Some(header), false) => format!("    {header} :\n    {{\n{uninitialized}    }}\n"),
        (None, true) => String::new(),
        (Some(_), true) => return Err(format!("overlay {id} has a .bss header in the overlay table and no bss row")),
        (None, false) => return Err(format!("overlay {id} has bss rows and no .bss header in the overlay table")),
    };
    Ok(vec![
        ("extern".to_owned(), if calls { "EXTERN(_call_via_r0)\n".to_owned() } else { String::new() }),
        ("text".to_owned(), text),
        ("bss".to_owned(), section),
        ("discard".to_owned(), discard),
    ])
}

/// One edition's linker fragments for an image: each place the list gives
/// the image, in the list's order, with the statements of the rows the
/// edition links, one per line. A place no row of this edition is in still
/// has its fragment, empty, for the script's INCLUDE to read.
pub(crate) fn render(rows: &[Row], image: &str, language: &str) -> Vec<(String, String)> {
    let mut fragments: Vec<(String, String)> = Vec::new();
    for row in rows.iter().filter(|row| row.image == image) {
        let at = match fragments.iter().position(|(place, _)| *place == row.place) {
            Some(at) => at,
            None => {
                fragments.push((row.place.clone(), String::new()));
                fragments.len() - 1
            }
        };
        if row.links(image, language) {
            fragments[at].1 += &format!("    {}\n", row.statement());
        }
    }
    fragments
}

/// The places of a linker script and the objects each links: an output
/// section is one place, named without its punctuation (`.rom_start` is
/// rom_start, `/DISCARD/` is discard), and a section with fixed placements
/// is one place per span, numbered from its start. A pattern for every
/// object (`*(.comment)`) is the script's own and no row. A place holds
/// nothing else: a symbol assignment inside one is refused.
pub(crate) fn places(text: &str) -> Result<Vec<(String, Vec<Statement>)>, String> {
    let mut found = Vec::new();
    for (section, items) in crate::edition::items(text)? {
        let name = section
            .trim_matches(|c: char| !c.is_ascii_alphanumeric())
            .to_ascii_lowercase();
        let pinned = items.iter().any(|item| matches!(item, Item::Placement(_)));
        let mut spans = vec![Vec::new()];
        for item in items {
            match item {
                Item::Input(statement) if statement.object != "*" => {
                    spans.last_mut().expect("one span").push(statement)
                }
                Item::Input(_) => {}
                Item::Assignment(assignment) => {
                    return Err(format!(
                        "{section} assigns {assignment:?}: a place holds objects only, and a symbol of the whole image belongs in the script the game's main.ld includes"
                    ))
                }
                Item::Placement(_) => spans.push(Vec::new()),
            }
        }
        for (index, span) in spans.into_iter().enumerate() {
            let place = if pinned { format!("{name}_{index:02}") } else { name.clone() };
            found.push((place, span));
        }
    }
    Ok(found)
}

/// The images the list has rows for, in its order.
pub(crate) fn images(rows: &[Row]) -> Vec<String> {
    let mut found: Vec<String> = Vec::new();
    for row in rows {
        if !found.contains(&row.image) {
            found.push(row.image.clone());
        }
    }
    found
}

/// One edition's rows of an image as the lines of a linker script, for the
/// tools that edit object lines: each place as a section of that name, a
/// statement per line.
pub(crate) fn view(rows: &[Row], image: &str, language: &str) -> String {
    render(rows, image, language)
        .into_iter()
        .map(|(place, fragment)| format!("{place} :\n{{\n{fragment}}}\n"))
        .collect()
}

/// Take an edited view back into the list as one edition's rows: a line
/// the view lost leaves that edition's row, and a line it gained joins the
/// same statement where another edition already has it at that point of
/// the order, or becomes a row of this edition alone. The other editions'
/// order is untouched.
pub(crate) fn apply(rows: &mut Vec<Row>, image: &str, language: &str, view: &str) -> Result<(), String> {
    let language = self::language(language)?;
    let wrapped = format!("SECTIONS\n{{\n{view}}}\n");
    let mut edited = rows.clone();
    for (place, items) in crate::edition::items(&wrapped)? {
        let mut new = Vec::new();
        for item in items {
            match item {
                Item::Input(statement) => new.push((statement.object, statement.sections)),
                other => return Err(format!("{place}: {other:?} is no object's line")),
            }
        }
        replace(&mut edited, image, language, &place, &new);
    }
    edited.retain(|row| !row.editions.is_empty());
    *rows = parse(&format(&edited)).map_err(|error| format!("the edited list: {error}"))?;
    Ok(())
}

fn replace(rows: &mut Vec<Row>, image: &str, language: &'static str, place: &str, new: &[(String, String)]) {
    let here = |row: &Row| row.image == image && row.place == place;
    let old: Vec<usize> = (0..rows.len())
        .filter(|index| here(&rows[*index]) && rows[*index].editions.contains(&language))
        .collect();
    let same = |row: &Row, new: &(String, String)| row.object == new.0 && row.sections == new.1;
    // common[i][j]: the longest common run of old[i..] and new[j..].
    let (n, m) = (old.len(), new.len());
    let mut common = vec![vec![0u32; m + 1]; n + 1];
    for i in (0..n).rev() {
        for j in (0..m).rev() {
            common[i][j] = if same(&rows[old[i]], &new[j]) {
                common[i + 1][j + 1] + 1
            } else {
                common[i + 1][j].max(common[i][j + 1])
            };
        }
    }
    let mut cursor = rows.iter().position(here).unwrap_or_else(|| {
        // A new place goes after the image's last row, a new image last.
        rows.iter().rposition(|row| row.image == image).map_or(rows.len(), |last| last + 1)
    });
    let (mut i, mut j, mut inserted) = (0, 0, 0);
    while i < n || j < m {
        if i < n && j < m && same(&rows[old[i] + inserted], &new[j]) {
            cursor = old[i] + inserted + 1;
            (i, j) = (i + 1, j + 1);
        } else if i < n && (j == m || common[i + 1][j] >= common[i][j + 1]) {
            let at = old[i] + inserted;
            rows[at].editions.retain(|known| *known != language);
            cursor = at + 1;
            i += 1;
        } else {
            // Rows this edition does not link are not in its order: the
            // same statement among them takes this edition in.
            let joined = (cursor..rows.len())
                .take_while(|index| here(&rows[*index]) && !rows[*index].editions.contains(&language))
                .find(|index| same(&rows[*index], &new[j]));
            match joined {
                Some(at) => {
                    rows[at].editions.push(language);
                    rows[at].editions.sort_by_key(|known| LANGUAGES.iter().position(|l| l == known));
                    cursor = at + 1;
                }
                None => {
                    rows.insert(
                        cursor,
                        Row {
                            image: image.to_owned(),
                            place: place.to_owned(),
                            object: new[j].0.clone(),
                            sections: new[j].1.clone(),
                            editions: vec![language],
                        },
                    );
                    inserted += 1;
                    cursor += 1;
                }
            }
            j += 1;
        }
    }
}

/// The object lines of one edition's main image, for a tool that edits
/// them: the edition's own linker script, or for a game with an object
/// list the view of its rows.
pub(crate) fn main_lines(root: &Path, game: &str, language: &str) -> Result<String, String> {
    match crate::targets::object_list_path(game) {
        Some(path) => Ok(view(&read_path(root, &path)?, MAIN, language)),
        None => {
            let path = crate::targets::edition_script_path(game, language);
            fs::read_to_string(root.join(&path)).map_err(|error| format!("{}: {error}", path.display()))
        }
    }
}

/// The object lines of one edition's main image in a file's text, as
/// `main_lines` reads the checkout's: the text of an edition's script as it
/// is, or the view of an object list's.
pub(crate) fn lines_of(text: &str, game: &str, language: &str) -> Result<String, String> {
    match crate::targets::object_list_path(game) {
        Some(_) => Ok(view(&parse(text)?, MAIN, language)),
        None => Ok(text.to_owned()),
    }
}

/// Write edited object lines back, each edition's to where `main_lines`
/// read them: the edition scripts, or the game's object list.
pub(crate) fn write_main_lines(root: &Path, game: &str, edits: &[(&str, String)]) -> Result<(), String> {
    let Some(path) = crate::targets::object_list_path(game) else {
        for (language, text) in edits {
            let path = crate::targets::edition_script_path(game, language);
            fs::write(root.join(&path), text).map_err(|error| format!("{}: {error}", path.display()))?;
        }
        return Ok(());
    };
    let mut rows = read_path(root, &path)?;
    for (language, text) in edits {
        apply(&mut rows, MAIN, language, text).map_err(|error| format!("{} {language}: {error}", path.display()))?;
    }
    fs::write(root.join(&path), format(&rows)).map_err(|error| format!("{}: {error}", path.display()))
}

/// Where a tool tells people the object lines of an edition are.
pub(crate) fn main_lines_path(game: &str, language: &str) -> std::path::PathBuf {
    crate::targets::object_list_path(game).unwrap_or_else(|| crate::targets::edition_script_path(game, language))
}

fn read_path(root: &Path, path: &Path) -> Result<Vec<Row>, String> {
    let text = fs::read_to_string(root.join(path)).map_err(|error| format!("{}: {error}", path.display()))?;
    parse(&text).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn entry(args: &[String]) -> std::process::ExitCode {
    match args {
        [] => {}
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return std::process::ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return std::process::ExitCode::from(2);
        }
    }
    match run(crate::compiler::routing::root()) {
        Ok(summary) => {
            println!("{summary}");
            std::process::ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            std::process::ExitCode::FAILURE
        }
    }
}

fn run(root: &Path) -> Result<String, String> {
    let mut summary = Vec::new();
    let mut failures = Vec::new();
    for game in TARGET_IDS.chunks(6) {
        let first = target_for(game[0]);
        let Some(list) = first.object_list() else {
            continue;
        };
        let rows = read(root, first)?.expect("a listed game has a list");
        let shown = list.display();
        failures.extend(repeats(&rows).into_iter().map(|found| format!("{shown}: {found}")));
        let mut sourced = BTreeSet::new();
        for row in rows.iter().filter(|row| row.built() && sourced.insert((row.image.as_str(), row.object.as_str()))) {
            // An overlay's own data and bss are resource_<id>_overlay.o, which
            // the build assembles from ovl_<id>.s, or empty without one.
            if row.image != MAIN && row.object == format!("resource_{}_overlay", row.image) {
                continue;
            }
            if let Err(error) = crate::build_rom::source_for(root, &row.object) {
                failures.push(format!("{shown}: {error}"));
            }
        }
        let images = images(&rows);
        let bss = read_bss(root, first)?;
        failures.extend(
            bss.keys()
                .filter(|overlay| !images.contains(overlay))
                .map(|overlay| format!("{shown}: the overlay table has overlay {overlay}, and the list no row for it")),
        );
        for id in game {
            let target = target_for(*id);
            for name in &images {
                let image = if name == MAIN { Image::Main } else { Image::Overlay(name) };
                let script = match crate::edition::image_script(root, target, image) {
                    Ok(script) => script,
                    Err(error) => {
                        failures.push(format!("{id} {name}: {error}"));
                        continue;
                    }
                };
                let found = disagreements(&rows, name, target.language(), &script)
                    .map_err(|error| format!("{id} {name}: {error}"))?;
                failures.extend(found.into_iter().map(|found| format!("{id} {name} {found}")));
            }
        }
        let (resources, found) = resources(root, &rows)?;
        failures.extend(found.into_iter().map(|found| format!("{shown}: {found}")));
        summary.push(format!(
            "{shown}: {} rows of {} images, each linked where its script includes its place; {resources} resources in their directory's order",
            rows.len(),
            images.len()
        ));
    }
    if !failures.is_empty() {
        return Err(format!("the object list is not what the build can link:\n{}", failures.join("\n")));
    }
    if summary.is_empty() {
        return Ok("no game has an object list".into());
    }
    Ok(summary.join("\n"))
}

/// The label that starts a game's resource directory, a table of pointers
/// written one `resource NAME, PLACE` row each.
const DIRECTORY: &str = "Resource_DirectoryTable";

/// The resources a game's modules define against its resource directory:
/// how many there are, and where their order is not the directory's. The
/// modules are the assembler sources linked after the directory in its
/// place; a game whose list links no directory has none.
fn resources(root: &Path, rows: &[Row]) -> Result<(usize, Vec<String>), String> {
    let assembler = |row: &Row| {
        let source = format!("{}.s", row.object);
        (row.image == MAIN && row.built() && root.join(&source).is_file()).then_some(source)
    };
    let mut directory = None;
    let mut modules = Vec::new();
    for row in rows {
        let Some(source) = assembler(row) else {
            continue;
        };
        match &directory {
            None => {
                let text = fs::read_to_string(root.join(&source)).map_err(|error| format!("{source}: {error}"))?;
                if labels_of(&text).iter().any(|label| label == DIRECTORY) {
                    directory = Some((row.place.clone(), directory_places(&text)));
                }
            }
            Some((place, _)) if *place == row.place => {
                let mut labels = Vec::new();
                module_labels(root, &source, &mut Vec::new(), &mut labels)?;
                modules.push((source, labels));
            }
            Some(_) => {}
        }
    }
    let Some((_, places)) = directory else {
        return Ok((0, Vec::new()));
    };
    let count = modules.iter().map(|(_, labels)| labels.len()).sum();
    Ok((count, out_of_order(&places, &modules)))
}

/// The global labels of an assembler source, in order.
fn labels_of(text: &str) -> Vec<String> {
    text.lines()
        .filter_map(|line| line.split('@').next().unwrap_or("").trim().strip_suffix(':'))
        .filter(|name| {
            name.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_')
                && name.bytes().all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
        })
        .map(str::to_owned)
        .collect()
}

/// The places a resource directory names, in the order it first names each:
/// a row is `resource NAME, PLACE`, and a place of 0 is an unused row.
fn directory_places(text: &str) -> Vec<String> {
    let mut places: Vec<String> = Vec::new();
    for line in text.lines() {
        let line = line.split('@').next().unwrap_or("").trim();
        let Some(row) = line.strip_prefix("resource").filter(|rest| rest.starts_with(char::is_whitespace)) else {
            continue;
        };
        let place = row.rsplit(',').next().unwrap_or("").trim();
        if place.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_') && !places.iter().any(|known| known == place) {
            places.push(place.to_owned());
        }
    }
    places
}

/// A module's global labels in the order the assembler meets them, those of
/// each assembler source it includes in that include's place.
fn module_labels(root: &Path, source: &str, open: &mut Vec<String>, labels: &mut Vec<String>) -> Result<(), String> {
    if open.iter().any(|known| known == source) {
        return Err(format!("{source} includes itself"));
    }
    let text = fs::read_to_string(root.join(source)).map_err(|error| format!("{source}: {error}"))?;
    open.push(source.to_owned());
    for line in text.lines() {
        match crate::build_rom::included_sources(line).first() {
            Some(included) => module_labels(root, included, open, labels)?,
            None => labels.extend(labels_of(line)),
        }
    }
    open.pop();
    Ok(())
}

/// Where the modules' labels are not the directory's places in the order
/// it first names them: Camelot's packer wrote each resource once, when
/// its row first came up, so that order is the order of the bytes. Places
/// no module defines (the far-call tables, the overlays) are other
/// objects'.
fn out_of_order(places: &[String], modules: &[(String, Vec<String>)]) -> Vec<String> {
    let mut found = Vec::new();
    let mut defined = BTreeSet::new();
    let mut linked = Vec::new();
    for (module, labels) in modules {
        for label in labels {
            if !defined.insert(label.as_str()) {
                found.push(format!("{module}: {label} is defined twice among the resources"));
            } else if !places.contains(label) {
                found.push(format!("{module}: {label} is linked among the resources, and the directory has no row for it"));
            } else {
                linked.push((module, label));
            }
        }
    }
    let named = places.iter().filter(|place| defined.contains(place.as_str()));
    if let Some(((module, label), place)) = linked.into_iter().zip(named).find(|((_, label), place)| label != place) {
        found.push(format!("{module}: {label} is linked where the directory's order has {place}"));
    }
    found
}

/// Input sections the list gives one edition twice: the linker places a
/// section by the first statement that names it, so a second is never read.
fn repeats(rows: &[Row]) -> Vec<String> {
    let mut seen = BTreeSet::new();
    let mut found = Vec::new();
    for (index, row) in rows.iter().enumerate() {
        for section in row.sections.split(' ') {
            for edition in &row.editions {
                if !seen.insert((row.image.as_str(), row.object.as_str(), section, *edition)) {
                    found.push(format!(
                        "line {}: {} {section} of {} is already listed for {edition}",
                        index + 2,
                        row.image,
                        row.object
                    ));
                }
            }
        }
    }
    found
}

/// Where one edition's script, as the linker reads it, and the list's rows
/// of an image differ: the first differing statement of each place. They
/// differ when the script includes a fragment outside the place it is
/// named for.
fn disagreements(rows: &[Row], image: &str, language: &str, script: &str) -> Result<Vec<String>, String> {
    let scripted = places(script)?;
    let mut listed = BTreeMap::new();
    for (place, fragment) in render(rows, image, language) {
        // Read the fragment back as the linker would, inside a section.
        let wrapped = format!("SECTIONS\n{{\n    fragment :\n    {{\n{fragment}    }}\n}}\n");
        let mut statements = crate::edition::statements(&wrapped).map_err(|error| format!("{place}: {error}"))?;
        // `places` leaves out a pattern for every object.
        statements.retain(|statement| statement.object != EVERY);
        listed.insert(place, statements);
    }
    let known: BTreeSet<&str> = scripted.iter().map(|(place, _)| place.as_str()).collect();
    let mut found: Vec<String> = listed
        .keys()
        .filter(|place| !known.contains(place.as_str()))
        .map(|place| format!("{place}: the list has a place the script has not"))
        .collect();
    let show = |statement: Option<&Statement>| {
        statement.map_or("nothing".to_owned(), |s| format!("{}({})", s.object, s.sections))
    };
    for (place, theirs) in &scripted {
        let ours = listed.get(place).map_or(&[][..], Vec::as_slice);
        let same = |index: usize| match (ours.get(index), theirs.get(index)) {
            (Some(a), Some(b)) => a.object == b.object && a.sections == b.sections,
            _ => false,
        };
        let length = ours.len().max(theirs.len());
        if let Some(index) = (0..length).find(|index| !same(*index)) {
            found.push(format!(
                "{place}: statement {} is {} in the list and {}",
                index + 1,
                show(ours.get(index)),
                show(theirs.get(index))
            ));
        }
    }
    Ok(found)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn list(rows: &str) -> String {
        format!("{HEADER}\n{rows}")
    }

    #[test]
    fn blank_columns_mean_the_usual_sections_and_all_six_editions() {
        let text = list("main\trom_start\tgames/g/a\nmain\trom_start\tgames/g/b\t.rodata\tja en\nmain\trom_start\tgames/g/c\t\tde\n");
        let rows = parse(&text).unwrap();
        assert_eq!(rows[0].sections, DEFAULT_SECTIONS);
        assert_eq!(rows[0].editions, LANGUAGES);
        assert_eq!(rows[1].sections, ".rodata");
        assert_eq!(rows[1].editions, ["ja", "en"]);
        assert_eq!(rows[2].sections, DEFAULT_SECTIONS);
        assert_eq!(rows[2].editions, ["de"]);
        let en: Vec<_> = rows.iter().filter(|row| row.links(MAIN, "en")).map(|row| row.object.as_str()).collect();
        assert_eq!(en, ["games/g/a", "games/g/b"]);
        assert_eq!(format(&rows), text);
    }

    #[test]
    fn rows_the_linker_could_not_read_are_refused() {
        let refused = |rows: &str| parse(&list(rows)).unwrap_err();
        assert!(refused("main\trom_start\tgames/g/a\t\tja pt\n").contains("\"pt\" is not one of"));
        assert!(refused("main\trom_start\tgames/g/a\t\ten ja\n").contains("order"));
        assert!(refused("main\trom_start\tgames/g/a\t\tja en de es fr it\n").contains("blank"));
        assert!(refused("main\trom_start\tgames/g/a.o\n").contains("without .o"));
        assert!(refused("main\trom_start\n").contains("three to five"));
        assert!(refused("main\trom_start\tgames/g/a\t.text\tja\textra\n").contains("three to five"));
        assert!(refused("Main\trom_start\tgames/g/a\n").contains("image"));
        assert!(refused("3AD\ttext\tgames/g/a\n").contains("image"));
        assert!(refused("main\t.rom_start\tgames/g/a\n").contains("place"));
        assert!(refused("main\trom_start\tgames/g/a\ttext\n").contains("sections"));
        assert!(refused("main\trom_start\tgames/g/a\t.text  .data\n").contains("sections"));
        assert!(parse("main\trom_start\tgames/g/a\n").unwrap_err().contains("header"));
        assert_eq!(parse(&list("3ad\ttext\tgames/g/a\t.text\n")).unwrap()[0].image, "3ad");
    }

    #[test]
    fn an_object_may_take_several_rows_and_an_archive_member_one() {
        let rows = parse(&list(
            "main\trom_main_00\tgames/g/a\t.text\n\
             main\trom_main_00\tlibgcc.a:_call_via_rX\t.text\n\
             main\trom_main_00\tgames/g/only_ja\t.text\tja\n\
             main\trom_main_01\tgames/g/a\t.rodata\n\
             main\trom_main_00\tgames/g/late\t.text\ten de es fr it\n\
             3ad\ttext\tgames/g/overlay\t.text\n\
             main\tdiscard\tgames/g/a\t.text.unused\n\
             main\trom_tail\tgames/g/japanese\t.rodata\tja\n",
        ))
        .unwrap();
        assert_eq!(
            render(&rows, MAIN, "ja"),
            [
                (
                    "rom_main_00".to_owned(),
                    "    \"*/games/g/a.o\"(.text)\n    \"*libgcc.a:_call_via_rX.o\"(.text)\n    \"*/games/g/only_ja.o\"(.text)\n".to_owned()
                ),
                ("rom_main_01".to_owned(), "    \"*/games/g/a.o\"(.rodata)\n".to_owned()),
                ("discard".to_owned(), "    \"*/games/g/a.o\"(.text.unused)\n".to_owned()),
                ("rom_tail".to_owned(), "    \"*/games/g/japanese.o\"(.rodata)\n".to_owned()),
            ]
        );
        let de = render(&rows, MAIN, "de");
        assert_eq!(
            de[0].1,
            "    \"*/games/g/a.o\"(.text)\n    \"*libgcc.a:_call_via_rX.o\"(.text)\n    \"*/games/g/late.o\"(.text)\n"
        );
        // A place only another edition fills is still a fragment to include.
        assert_eq!(de[3], ("rom_tail".to_owned(), String::new()));
        assert_eq!(render(&rows, "3ad", "de"), [("text".to_owned(), "    \"*/games/g/overlay.o\"(.text)\n".to_owned())]);
    }

    const SCRIPT: &str = "SECTIONS\n{\n    ram (NOLOAD) : { \"*/games/g/ram.o\"(.sym_ram) } > RAM\n    .rom_main :\n    {\n    \"*/games/g/a.o\"(.text .rodata .data .padding)\n    \"*libgcc.a:_call_via_rX.o\"(.text)\n    . = ABSOLUTE(0x08009000);\n    \"*/games/g/b.o\"(.text)\n    \"*/games/g/a.o\"(.rodata.late)\n    } > ROM\n    /DISCARD/ : { \"*/games/g/b.o\"(.text.unused) *(.comment) *(.note*) }\n}\n";

    #[test]
    fn a_script_has_a_place_for_each_section_and_each_span_between_placements() {
        let found = places(SCRIPT).unwrap();
        let shape: Vec<(&str, usize)> = found.iter().map(|(place, span)| (place.as_str(), span.len())).collect();
        assert_eq!(shape, [("ram", 1), ("rom_main_00", 2), ("rom_main_01", 2), ("discard", 1)]);
        let assigned = SCRIPT.replace("    . = ABSOLUTE(0x08009000);\n", "    Size = ABSOLUTE(End - Start);\n");
        assert!(places(&assigned).unwrap_err().contains(".rom_main assigns \"Size = ABSOLUTE(End - Start)\""));
    }

    const AGREEING: &str = "main\tram\tgames/g/ram\t.sym_ram\n\
         main\trom_main_00\tgames/g/a\n\
         main\trom_main_00\tlibgcc.a:_call_via_rX\t.text\n\
         main\trom_main_01\tgames/g/b\t.text\n\
         main\trom_main_01\tgames/g/a\t.rodata.late\n\
         main\tdiscard\tgames/g/b\t.text.unused\n";

    #[test]
    fn the_list_and_a_script_agree_statement_by_statement() {
        let check = |rows: &str| disagreements(&parse(&list(rows)).unwrap(), MAIN, "en", SCRIPT).unwrap();
        assert_eq!(check(AGREEING), Vec::<String>::new());
        let moved = AGREEING.replace("rom_main_01\tgames/g/b", "rom_main_00\tgames/g/b");
        assert_eq!(
            check(&moved),
            [
                "rom_main_00: statement 3 is games/g/b(.text) in the list and nothing",
                "rom_main_01: statement 1 is games/g/a(.rodata.late) in the list and games/g/b(.text)",
            ]
        );
        let other_edition = AGREEING.replace("games/g/b\t.text\n", "games/g/b\t.text\tja\n");
        assert_eq!(check(&other_edition).len(), 1);
        let sections = AGREEING.replace("main\trom_main_00\tgames/g/a\n", "main\trom_main_00\tgames/g/a\t.text\n");
        assert!(check(&sections)[0].contains("games/g/a(.text) in the list and games/g/a(.text .rodata .data .padding)"));
        let stray = format!("{AGREEING}main\trom_tail\tgames/g/c\n");
        assert_eq!(check(&stray), ["rom_tail: the list has a place the script has not"]);
    }

    #[test]
    fn a_section_listed_twice_for_an_edition_is_found() {
        let rows = |text: &str| parse(&list(text)).unwrap();
        assert_eq!(repeats(&rows(AGREEING)), Vec::<String>::new());
        let twice = format!("{AGREEING}main\trom_main_01\tgames/g/b\t.rodata .text\tde\n");
        assert_eq!(repeats(&rows(&twice)), ["line 8: main .text of games/g/b is already listed for de"]);
        let apart = "main\trom_main_00\tgames/g/a\t.text\tja\nmain\trom_main_01\tgames/g/a\t.text\ten de\n3ad\ttext\tgames/g/a\t.text\n";
        assert_eq!(repeats(&rows(apart)), Vec::<String>::new());
    }

    #[test]
    fn an_edited_view_changes_only_its_edition() {
        let text = list(
            "main\trom_start\tgames/g/a\n\
             main\trom_start\tgames/g/b\t.text\n\
             main\trom_start\tgames/g/only_ja\t.text\tja\n\
             main\trom_start\tgames/g/c\t.text\n\
             main\trom_tail\tgames/g/d\t.rodata\n",
        );
        let mut rows = parse(&text).unwrap();
        let before = view(&rows, MAIN, "en");
        assert_eq!(
            before,
            "rom_start :\n{\n    \"*/games/g/a.o\"(.text .rodata .data .padding)\n    \"*/games/g/b.o\"(.text)\n    \"*/games/g/c.o\"(.text)\n}\nrom_tail :\n{\n    \"*/games/g/d.o\"(.rodata)\n}\n"
        );
        // Unedited, a view changes nothing.
        apply(&mut rows, MAIN, "en", &before).unwrap();
        assert_eq!(format(&rows), text);
        // One edition replaces b by m and gains a line at the end.
        let edited = before
            .replace("\"*/games/g/b.o\"(.text)", "\"*/games/g/m.o\"(.text)")
            .replace("    \"*/games/g/d.o\"(.rodata)\n", "    \"*/games/g/d.o\"(.rodata)\n    \"*/games/g/e.o\"(.rodata)\n");
        apply(&mut rows, MAIN, "en", &edited).unwrap();
        assert_eq!(view(&rows, MAIN, "en"), edited);
        assert_eq!(view(&rows, MAIN, "de"), before);
        assert_eq!(
            format(&rows),
            list(
                "main\trom_start\tgames/g/a\n\
                 main\trom_start\tgames/g/b\t.text\tja de es fr it\n\
                 main\trom_start\tgames/g/m\t.text\ten\n\
                 main\trom_start\tgames/g/only_ja\t.text\tja\n\
                 main\trom_start\tgames/g/c\t.text\n\
                 main\trom_tail\tgames/g/d\t.rodata\n\
                 main\trom_tail\tgames/g/e\t.rodata\ten\n"
            )
        );
    }

    #[test]
    fn the_same_edit_in_all_six_views_is_one_row_again() {
        let text = list(
            "main\trom_start\tgames/g/a\t.text\n\
             main\trom_start\tgames/g/b\t.text\n\
             main\trom_start\tgames/g/only_ja\t.text\tja\n\
             main\trom_start\tgames/g/c\t.text\n",
        );
        let mut rows = parse(&text).unwrap();
        for language in LANGUAGES {
            let edited = view(&rows, MAIN, language)
                .replace("    \"*/games/g/a.o\"(.text)\n    \"*/games/g/b.o\"(.text)\n", "    \"*/games/g/m.o\"(.text)\n    \"*/games/g/n.o\"(.text)\n");
            apply(&mut rows, MAIN, language, &edited).unwrap();
            assert_eq!(view(&rows, MAIN, language), edited);
        }
        assert_eq!(
            format(&rows),
            list(
                "main\trom_start\tgames/g/m\t.text\n\
                 main\trom_start\tgames/g/n\t.text\n\
                 main\trom_start\tgames/g/only_ja\t.text\tja\n\
                 main\trom_start\tgames/g/c\t.text\n"
            )
        );
        // A line that is no object's is refused, and the rows stay.
        assert!(apply(&mut rows, MAIN, "en", "rom_start :\n{\n    . = ABSOLUTE(0x08009000);\n}\n").is_err());
        assert!(apply(&mut rows, MAIN, "pt", "").is_err());
    }

    #[test]
    fn an_overlay_has_its_extern_text_bss_and_discard_fragments() {
        let rows = parse(&list(
            "3ad\ttext\tgames/g/a\t.text\n\
             3ad\ttext\tlibgcc.a:_call_via_rX\t.text\n\
             3ad\ttext\tgames/g/data\t.data\n\
             3ad\tbss\tresource_3ad_overlay\t.bss\n\
             3ad\tbss\t*\t.bss COMMON\n\
             3ad\tdiscard\tgames/g/spawn\t.text.resident\n\
             3ae\ttext\tgames/g/b\t.text\n",
        ))
        .unwrap();
        assert_eq!(format(&rows).lines().nth(5), Some("3ad\tbss\t*\t.bss COMMON"));
        let bss = parse_bss("overlay\tbss\n3ad\t.bss ALIGN(16) (NOLOAD)\n").unwrap();
        let fragment = |name: &str, text: &str| (name.to_owned(), text.to_owned());
        assert_eq!(
            overlay_fragments(&rows, &bss, "3ad", "en").unwrap(),
            [
                fragment("extern", "EXTERN(_call_via_r0)\n"),
                fragment(
                    "text",
                    "    \"*/games/g/a.o\"(.text)\n    \"*libgcc.a:_call_via_rX.o\"(.text)\n    \"*/games/g/data.o\"(.data)\n"
                ),
                fragment(
                    "bss",
                    "    .bss ALIGN(16) (NOLOAD) :\n    {\n    \"*/resource_3ad_overlay.o\"(.bss)\n    *(.bss COMMON)\n    }\n"
                ),
                fragment("discard", "    \"*/games/g/spawn.o\"(.text.resident)\n"),
            ]
        );
        // An overlay with neither still has all four, for overlay.ld to include.
        assert_eq!(
            overlay_fragments(&rows, &bss, "3ae", "en").unwrap(),
            [
                fragment("extern", ""),
                fragment("text", "    \"*/games/g/b.o\"(.text)\n"),
                fragment("bss", ""),
                fragment("discard", ""),
            ]
        );
        assert!(overlay_fragments(&rows, &bss, "3af", "en").unwrap_err().contains("no row for overlay 3af"));
        assert!(overlay_fragments(&rows, &BTreeMap::new(), "3ad", "en").unwrap_err().contains("no .bss header"));
        let both = parse_bss("overlay\tbss\n3ad\t.bss\n3ae\t.bss (NOLOAD)\n").unwrap();
        assert!(overlay_fragments(&rows, &both, "3ae", "en").unwrap_err().contains("no bss row"));
        // The edited view of an overlay keeps its pattern for every object.
        let mut edited = rows.clone();
        apply(&mut edited, "3ad", "en", &view(&rows, "3ad", "en")).unwrap();
        assert_eq!(edited, rows);
    }

    #[test]
    fn the_overlay_table_takes_the_bss_headers_the_overlays_show() {
        let table = parse_bss("overlay\tbss\n36f\t.bss (NOLOAD)\n3a7\t.bss\n3b0\t.bss ALIGN(8)\n3ce\t.bss ALIGN(16) (NOLOAD)\n").unwrap();
        assert_eq!(table["3b0"], ".bss ALIGN(8)");
        assert_eq!(table.len(), 4);
        let refused = |rows: &str| parse_bss(&format!("{BSS_HEADER}\n{rows}")).unwrap_err();
        assert!(refused("36f\t.data\n").contains("no .bss header"));
        assert!(refused("36f\t.bss 0x2010000\n").contains("ALIGN(8)"));
        assert!(refused("36f\t.bss ALIGN(4)\n").contains("ALIGN(8)"));
        assert!(refused("36F\t.bss\n").contains("no overlay id"));
        assert!(refused("36f\n").contains("its .bss header"));
        assert!(refused("36f\t.bss\n36f\t.bss (NOLOAD)\n").contains("twice"));
        assert!(parse_bss("36f\t.bss\n").unwrap_err().contains("header"));
        assert!(parse(&list("3ad\trom_start\tgames/g/a\n")).unwrap_err().contains("not one of an overlay's"));
    }

    #[test]
    fn resource_modules_follow_the_directorys_first_references() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        // The directory names a far-call table no module defines, one
        // resource twice, an unused row, and its own build stamp.
        write(
            "games/g/dir.s",
            "\t.macro resource name, place:req\n\t.4byte \\place\n\t.endm\nResource_DirectoryTable:\n\
             \tresource , FarCall_Table\n\tresource Tiles, Ui_Tiles\n\tresource , Title_Sun\n\tresource , Ui_Tiles\n\
             \tresource , Field_Map\n\tresource , Tileset_Palette\n\tresource , 0\nResource_BuildStamp:\n",
        );
        write("games/g/code.s", "FarCall_Table:\n");
        write("games/common/ui.s", "\t.section .rodata\n\t.global Ui_Tiles\nUi_Tiles:\n\t.incbin \"graphics/ui/tiles.4bpp\"\n");
        write(
            "games/g/title.s",
            "\t.section .rodata\n\t.include \"games/common/ui.s\"\n\t.global Title_Sun\nTitle_Sun: @ 太陽\n\t.incbin \"graphics/title/sun.4bpp.lz\"\n",
        );
        write(
            "games/g/map.s",
            "\t.section .rodata\nField_Map:\n\tmap_block Field_Map, .LField_Map_GRID\n.LField_Map_GRID:\n\t.balign 4, 0\nTileset_Palette:\n",
        );
        let rows = |order: &str| {
            let rows: String = order.split(' ').map(|object| format!("main\trom_tail\tgames/g/{object}\t.rodata\n")).collect();
            parse(&list(&format!("main\trom_main\tgames/g/code\n{rows}"))).unwrap()
        };
        assert_eq!(resources(root.path(), &rows("dir title map")).unwrap(), (4, Vec::new()));
        // A module out of the directory's order, by its first label there.
        let (_, found) = resources(root.path(), &rows("dir map title")).unwrap();
        assert_eq!(found, ["games/g/map.s: Field_Map is linked where the directory's order has Ui_Tiles"]);
        // A label inside a module out of order.
        write("games/g/map.s", "Tileset_Palette:\nField_Map:\n");
        let (_, found) = resources(root.path(), &rows("dir title map")).unwrap();
        assert_eq!(found, ["games/g/map.s: Tileset_Palette is linked where the directory's order has Field_Map"]);
        // A resource the directory has no row for, and one defined twice.
        write("games/g/map.s", "Field_Map:\nTileset_Palette:\nField_Spare:\n\t.include \"games/common/ui.s\"\n");
        let (_, found) = resources(root.path(), &rows("dir title map")).unwrap();
        assert_eq!(
            found,
            [
                "games/g/map.s: Field_Spare is linked among the resources, and the directory has no row for it",
                "games/g/map.s: Ui_Tiles is defined twice among the resources",
            ]
        );
        // A list that links no directory has no resources to order.
        assert_eq!(resources(root.path(), &rows("map title")).unwrap(), (0, Vec::new()));
    }

    #[test]
    fn the_committed_list_is_what_the_build_links() {
        let root = crate::compiler::routing::root();
        let summary = run(root).unwrap();
        assert!(summary.contains("games/tbs/objects.tsv"), "{summary}");
        let text = fs::read_to_string(root.join("games/tbs/objects.tsv")).unwrap();
        assert_eq!(format(&parse(&text).unwrap()), text);
    }
}
