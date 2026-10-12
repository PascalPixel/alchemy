//! A game's object list, games/<game>/objects.tsv: what each image links,
//! one linker statement per row (three for an overlay's `unit` row), in
//! link order.
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
//! An overlay begins with three objects: its entry table, the object of its
//! C file and its import table. One row says so: a row whose place is
//! `unit` names the object DIR/NAME between the two tables and stands for
//! those three rows of the `text` place, each a whole object, for the row's
//! editions. The entry table is DIR/entry/NAME where the checkout has that
//! assembler source and DIR/entry otherwise, and the import table is
//! DIR/import/NAME or DIR/import the same way. So an overlay with a folder
//! of its own keeps `entry.s` and `import.s` beside its C file
//! (games/tbs/src/menu/title), and overlays whose C files share one folder
//! (games/tbs/src/scene) keep their tables in its `entry` and `import`
//! folders, each named as its C file. The row names no sections. What an
//! overlay links besides (a library member, a picture, its `.bss`) has its
//! own rows after it. The list is read as the rows it stands for, so every
//! tool sees one row per statement, and it is written with a `unit` row
//! wherever three rows are one.
//!
//! A row whose object is `@TABLE` names no object: it stands for the
//! modules TABLE names, a row each with its place, sections and editions,
//! in the order TABLE first names a label of each. TABLE.s is a resource
//! directory (`resource NAME, PLACE` rows) or a song table
//! (`song HEADER, PLAYER` rows); a module is an assembler source, or a
//! song's MIDI, under the table's game or games/common that no other
//! source includes and no other row links; the game's own module comes
//! before a games/common one of the same names. The data's order is then
//! written once, in its table. Every reader has those rows written out,
//! and `alchemy check object-list --rows` prints the list that way.
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
const OVERLAY_PLACES: [&str; 3] = [TEXT, "bss", "discard"];
const TEXT: &str = "text";
/// The place of a row that names an overlay's C object and stands for
/// three rows of its text place: its entry table, the object, and its
/// import table.
const UNIT: &str = "unit";
/// The two tables of a unit DIR/NAME: DIR/entry/NAME and DIR/import/NAME
/// where the checkout has those sources, DIR/entry and DIR/import otherwise.
const ENTRY: &str = "entry";
const IMPORT: &str = "import";
/// The archive member an overlay links to call through a register, and the
/// symbol that pulls it from the archive.
const CALL_VIA: &str = "libgcc.a:_call_via_rX";
pub(crate) const BSS_HEADER: &str = "overlay\tbss";

const USAGE: &str = "usage: alchemy check object-list [--rows]\n\
--rows prints each list as the build reads it, a row for every object: a\n\
row `@TABLE` written out as the modules that table names, in its order.\n\
\n\
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
(rom_main_00, rom_main_01 ...). An overlay's row of the place unit names\n\
its C object DIR/NAME and is three rows of its text place: its entry\n\
table, the object, and its import table. The entry table is DIR/entry/NAME\n\
where the checkout has that assembler source and DIR/entry otherwise; the\n\
import table is DIR/import/NAME or DIR/import the same way. A game without\n\
a list is not checked.";

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

/// The rows a list's text stands for, a unit row's tables found in the
/// checkout at `root`.
pub(crate) fn parse(root: &Path, text: &str) -> Result<Vec<Row>, String> {
    Ok(parse_lines(text, &sourced_in(root))?.into_iter().map(|(_, row)| row).collect())
}

/// Whether the checkout has an assembler source for an object: how a unit
/// row's tables are found.
type Sourced<'a> = &'a dyn Fn(&str) -> bool;

fn sourced_in(root: &Path) -> impl Fn(&str) -> bool + '_ {
    move |object| root.join(format!("{object}.s")).is_file()
}

/// The rows a list's text stands for, each with the line it is written on:
/// the three rows of a unit row share its line.
fn parse_lines(text: &str, sourced: Sourced) -> Result<Vec<(usize, Row)>, String> {
    let mut lines = text.lines().enumerate();
    match lines.next() {
        Some((_, HEADER)) => {}
        _ => return Err(format!("line 1: the header is {HEADER:?}")),
    }
    let mut rows = Vec::new();
    for (index, line) in lines {
        let written =
            row(line).and_then(|row| unit_rows(row, sourced)).map_err(|error| format!("line {}: {error}", index + 1))?;
        rows.extend(written.into_iter().map(|row| (index + 1, row)));
    }
    Ok(rows)
}

/// The entry table and the import table of an object DIR/NAME a unit row
/// can name: DIR/entry/NAME and DIR/import/NAME where the checkout has
/// those sources, DIR/entry and DIR/import otherwise. An archive member is
/// no unit, and neither is a folder's own table.
fn unit_tables(object: &str, sourced: Sourced) -> Option<[String; 2]> {
    let (folder, name) = object.rsplit_once('/')?;
    if object.contains(':') || matches!(name, "" | ENTRY | IMPORT) {
        return None;
    }
    Some([ENTRY, IMPORT].map(|table| {
        let own = format!("{folder}/{table}/{name}");
        if sourced(&own) {
            own
        } else {
            format!("{folder}/{table}")
        }
    }))
}

/// The entry table and the import table of the unit an object of the
/// checkout is, as a unit row finds them.
pub(crate) fn tables(root: &Path, object: &str) -> Option<[String; 2]> {
    unit_tables(object, &sourced_in(root))
}

/// Whether an object is an import table by its name: a folder's `import`,
/// or one of the tables in a folder of that name.
pub(crate) fn import_table(object: &str) -> bool {
    let mut names = object.rsplit('/');
    names.next() == Some(IMPORT) || names.next() == Some(IMPORT)
}

/// The rows a written row stands for: itself, or for a unit row the named
/// object's entry table, the object and its import table, each a whole
/// object of the text place.
fn unit_rows(row: Row, sourced: Sourced) -> Result<Vec<Row>, String> {
    if row.place != UNIT {
        return Ok(vec![row]);
    }
    let [entry, import] = unit_tables(&row.object, sourced).ok_or_else(|| {
        format!("unit {:?} is not an object of a folder, between its {ENTRY} and {IMPORT} tables", row.object)
    })?;
    Ok([entry, row.object.clone(), import]
        .into_iter()
        .map(|object| Row { place: TEXT.to_owned(), object, ..row.clone() })
        .collect())
}

/// The row that names the unit the first three rows are: an overlay's entry
/// table, the object the table is of and that object's import table, each
/// whole in the text place and for the same editions.
fn unit<'a>(rows: &'a [Row], sourced: Sourced) -> Option<&'a Row> {
    let [entry, named, import, ..] = rows else {
        return None;
    };
    let tables = unit_tables(&named.object, sourced)?;
    let whole = |row: &Row, object: &str| {
        row.image == named.image
            && row.place == TEXT
            && row.object == object
            && row.sections == DEFAULT_SECTIONS
            && row.editions == named.editions
    };
    (named.image != MAIN && whole(entry, &tables[0]) && whole(named, &named.object) && whole(import, &tables[1]))
        .then_some(named)
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
    if place == UNIT {
        if image == MAIN {
            return Err("a unit row is an overlay's: the main image has a row for each object".into());
        }
        if fields.get(3).is_some_and(|sections| !sections.is_empty()) {
            return Err("a unit row names no sections: its entry table, object and import table link whole".into());
        }
    } else if image != MAIN && !OVERLAY_PLACES.contains(&place) {
        return Err(format!("place {place:?} is not one of an overlay's: {}, or {UNIT}", OVERLAY_PLACES.join(" ")));
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

/// The list as its file's text: what `parse` reads back as the same rows
/// in the same checkout. Three rows that are one unit are written as its
/// row.
pub(crate) fn format(root: &Path, rows: &[Row]) -> String {
    write_rows(rows, Some(&sourced_in(root)))
}

/// The rows as a list's text; with `units`, which finds a unit's tables,
/// three rows that are one unit are written as its one row.
fn write_rows(rows: &[Row], units: Option<Sourced>) -> String {
    let mut text = format!("{HEADER}\n");
    let mut at = 0;
    while at < rows.len() {
        let (row, place, count) = match units.and_then(|sourced| unit(&rows[at..], sourced)) {
            Some(named) => (named, UNIT, 3),
            None => (&rows[at], rows[at].place.as_str(), 1),
        };
        let sections = if row.sections == DEFAULT_SECTIONS { "" } else { &row.sections };
        let editions = if row.editions.len() == LANGUAGES.len() { String::new() } else { row.editions.join(" ") };
        let mut fields = vec![row.image.as_str(), place, &row.object, sections, &editions];
        while fields.last().is_some_and(|field| field.is_empty()) {
            fields.pop();
        }
        text += &fields.join("\t");
        text.push('\n');
        at += count;
    }
    text
}

/// A game's list, or None for a game that links by scripts naming their own
/// objects: every row an object's, a `@TABLE` row written out as the
/// modules its table names.
pub(crate) fn read(root: &Path, target: DecompTarget) -> Result<Option<Vec<Row>>, String> {
    let Some(path) = target.object_list() else {
        return Ok(None);
    };
    read_path(root, &path).map(Some)
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

/// The input sections a plain place takes from each of its objects, one
/// pass after another: code, read-only data, data.
const PLAIN_PASSES: [&str; 3] = [".text .padding", ".rodata", ".data .overlays"];

/// A plain place is a ROM place of the main image, or an overlay's text
/// place, whose rows all name a whole object or a whole library member and
/// no sections: an ordered list of objects, as a makefile has them with
/// its libraries last, linked as `*(.text) *(.rodata) *(.data)` would link
/// them.
fn plain(image: &str, place: &str, rows: &[Row]) -> bool {
    let mut own = rows.iter().filter(|row| row.image == image && row.place == place).peekable();
    (if image == MAIN { place.starts_with("rom_") } else { place == TEXT })
        && own.peek().is_some()
        && own.all(|row| row.sections == DEFAULT_SECTIONS && row.object != EVERY)
}

/// One edition's linker fragments for an image: each place the list gives
/// the image, in the list's order. A plain place links every object's code
/// in the list's order, then every object's read-only data in that order,
/// then its data; any other place has the statements of its rows, one per
/// line. A place no row of this edition is in still has its fragment,
/// empty, for the script's INCLUDE to read.
pub(crate) fn render(rows: &[Row], image: &str, language: &str) -> Vec<(String, String)> {
    render_rows(rows, image, language)
        .into_iter()
        .map(|(place, fragment)| {
            if !plain(image, &place, rows) {
                return (place, fragment);
            }
            let mut passes = String::new();
            for sections in PLAIN_PASSES {
                for row in rows.iter().filter(|row| row.place == place && row.links(image, language)) {
                    passes += &format!("    {}\n", statement(&row.object, sections));
                }
            }
            (place, passes)
        })
        .collect()
}

/// The rows as they are written: each place with one statement per row.
pub(crate) fn render_rows(rows: &[Row], image: &str, language: &str) -> Vec<(String, String)> {
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
    render_rows(rows, image, language)
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
    // Read back from their own text, a row each: no row is a unit's, so no
    // table is looked for.
    let read = parse_lines(&write_rows(&edited, None), &|_| false).map_err(|error| format!("the edited list: {error}"))?;
    *rows = read.into_iter().map(|(_, row)| row).collect();
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
pub(crate) fn lines_of(root: &Path, text: &str, game: &str, language: &str) -> Result<String, String> {
    match crate::targets::object_list_path(game) {
        Some(_) => Ok(view(&parse(root, text)?, MAIN, language)),
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
    let before = read_written(root, &path)?;
    let mut rows = expand(root, before.clone()).map_err(|error| format!("{}: {error}", path.display()))?;
    for (language, text) in edits {
        apply(&mut rows, MAIN, language, text).map_err(|error| format!("{} {language}: {error}", path.display()))?;
    }
    let rows = collapse(root, &before, rows).map_err(|error| format!("{}: {error}", path.display()))?;
    fs::write(root.join(&path), format(root, &rows)).map_err(|error| format!("{}: {error}", path.display()))
}

/// Where a tool tells people the object lines of an edition are.
pub(crate) fn main_lines_path(game: &str, language: &str) -> std::path::PathBuf {
    crate::targets::object_list_path(game).unwrap_or_else(|| crate::targets::edition_script_path(game, language))
}

fn read_path(root: &Path, path: &Path) -> Result<Vec<Row>, String> {
    expand(root, read_written(root, path)?).map_err(|error| format!("{}: {error}", path.display()))
}

/// The rows as the list's file has them, a `@TABLE` row still one row.
fn read_written(root: &Path, path: &Path) -> Result<Vec<Row>, String> {
    let text = fs::read_to_string(root.join(path)).map_err(|error| format!("{}: {error}", path.display()))?;
    parse(root, &text).map_err(|error| format!("{}: {error}", path.display()))
}

/// Before a table's path in the object column: the row stands for the
/// modules that table names.
const NAMED_BY: char = '@';

/// A module a table names: its object, and the table's names it defines.
struct Named {
    object: String,
    labels: Vec<String>,
}

/// What each row stands for: itself, or for a `@TABLE` row a row for each
/// module the table names, with that row's place, sections and editions, in
/// the order the table first names a label of each. A module another row
/// links is that row's, and is left out; so is a games/common module
/// whose every name the game's own modules define, which is the other
/// game's.
fn stands_for(root: &Path, rows: &[Row]) -> Result<Vec<Vec<Row>>, String> {
    let listed: BTreeSet<&str> = rows.iter().map(|row| row.object.as_str()).collect();
    let mut found = Vec::with_capacity(rows.len());
    for row in rows {
        let Some(table) = row.object.strip_prefix(NAMED_BY) else {
            found.push(vec![row.clone()]);
            continue;
        };
        let mut owners: BTreeMap<&str, &str> = BTreeMap::new();
        let mut run = Vec::new();
        let modules = named(root, table)?;
        let shared = |module: &Named| module.object.starts_with("games/common/");
        let own: BTreeSet<&str> = modules
            .iter()
            .filter(|module| !shared(module) || listed.contains(module.object.as_str()))
            .flat_map(|module| module.labels.iter().map(String::as_str))
            .collect();
        for module in modules.iter().filter(|module| !listed.contains(module.object.as_str())) {
            if shared(module) && module.labels.iter().all(|label| own.contains(label.as_str())) {
                continue;
            }
            for label in &module.labels {
                if let Some(other) = owners.insert(label, &module.object) {
                    return Err(format!("{table} names {label}, which {other} and {} both define", module.object));
                }
            }
            run.push(Row { object: module.object.clone(), ..row.clone() });
        }
        if run.is_empty() {
            return Err(format!("{table} names no module that is not a row already"));
        }
        found.push(run);
    }
    Ok(found)
}

/// The rows with each `@TABLE` row written out as the rows it stands for.
fn expand(root: &Path, rows: Vec<Row>) -> Result<Vec<Row>, String> {
    if !rows.iter().any(|row| row.object.starts_with(NAMED_BY)) {
        return Ok(rows);
    }
    Ok(stands_for(root, &rows)?.into_iter().flatten().collect())
}

/// Rows a tool edited, as the list's file writes them: each run of rows a
/// `@TABLE` row of `written` stood for is that one row again. A tool
/// cannot edit such a run, which follows the table and its modules: an
/// edit the written list would not read back is refused.
fn collapse(root: &Path, written: &[Row], rows: Vec<Row>) -> Result<Vec<Row>, String> {
    let refused = |table: &Row| {
        format!(
            "{} {}: the rows {} stands for were edited; they follow that table and the modules it names",
            table.image, table.place, table.object
        )
    };
    let runs = stands_for(root, written)?;
    let tables: Vec<(&Row, &Vec<Row>)> =
        written.iter().zip(&runs).filter(|(row, _)| row.object.starts_with(NAMED_BY)).collect();
    let mut found = rows.clone();
    for (table, run) in &tables {
        let at = found.windows(run.len()).position(|window| window == run.as_slice()).ok_or_else(|| refused(table))?;
        found.splice(at..at + run.len(), [(*table).clone()]);
    }
    match tables.first() {
        Some((table, _)) if expand(root, found.clone())? != rows => Err(refused(table)),
        _ => Ok(found),
    }
}

/// A game's rows as its list's file writes them, for a tool that changed
/// the rows `read` gave it.
pub(crate) fn written(root: &Path, target: DecompTarget, rows: Vec<Row>) -> Result<String, String> {
    let path = target.object_list().ok_or("the game has no object list")?;
    let before = read_written(root, &path)?;
    collapse(root, &before, rows).map(|rows| format(root, &rows)).map_err(|error| format!("{}: {error}", path.display()))
}

/// The modules a table names, in the order it first names a label of each.
/// The table is an assembler source, TABLE.s: a resource directory, whose
/// `resource NAME, PLACE` rows name places, or a song table, whose
/// `song HEADER, PLAYER` rows name song headers. A module is an assembler
/// source, or for a song table a MIDI too, under the table's game or
/// games/common that no assembler source includes; its labels are its own
/// and those of the sources it includes. One scan serves a process while
/// the table's file is as it was.
fn named(root: &Path, table: &str) -> Result<std::sync::Arc<Vec<Named>>, String> {
    type Stamp = (std::time::SystemTime, u64);
    type Known = BTreeMap<std::path::PathBuf, (Stamp, std::sync::Arc<Vec<Named>>)>;
    static KNOWN: std::sync::Mutex<Known> = std::sync::Mutex::new(BTreeMap::new());
    let source = format!("{table}.s");
    let path = root.join(&source);
    let stamp = fs::metadata(&path)
        .and_then(|found| Ok((found.modified()?, found.len())))
        .map_err(|error| format!("{source}: {error}"))?;
    if let Some((known, modules)) = KNOWN.lock().expect("the scans").get(&path) {
        if *known == stamp {
            return Ok(modules.clone());
        }
    }
    let text = fs::read_to_string(&path).map_err(|error| format!("{source}: {error}"))?;
    let (names, songs) = match directory_places(&text) {
        places if places.is_empty() => (song_headers(&text), true),
        places => (places, false),
    };
    if names.is_empty() {
        return Err(format!("{source} has no resource or song row to name a module"));
    }
    let game = match table.split('/').collect::<Vec<_>>()[..] {
        ["games", game, _, ..] => game.to_owned(),
        _ => return Err(format!("{table} is no source under games/<game>")),
    };
    let mut sources = Vec::new();
    for folder in BTreeSet::from([format!("games/{game}"), "games/common".to_owned()]) {
        module_sources(root, &folder, songs, &mut sources)?;
    }
    let mut included = BTreeSet::new();
    for source in sources.iter().filter(|source| source.ends_with(".s")) {
        let text = fs::read_to_string(root.join(source)).map_err(|error| format!("{source}: {error}"))?;
        included.extend(crate::build_rom::included_sources(&text));
    }
    let mut modules = Vec::new();
    for source in sources.iter().filter(|source| !included.contains(*source)) {
        let (object, kind) = source.rsplit_once('.').expect("a source has an extension");
        if object == table {
            continue;
        }
        let mut labels = Vec::new();
        if kind == "s" {
            module_labels(root, source, &mut Vec::new(), &mut labels)?;
        } else {
            let midi = fs::read(root.join(source)).map_err(|error| format!("{source}: {error}"))?;
            labels = ags::sound::sequence_labels(&midi).map_err(|error| format!("{source}: {error}"))?;
        }
        let mut order: Vec<usize> =
            labels.iter().filter_map(|label| names.iter().position(|name| name == label)).collect();
        order.sort_unstable();
        if let Some(first) = order.first().copied() {
            let labels = order.into_iter().map(|at| names[at].clone()).collect();
            modules.push((first, Named { object: object.to_owned(), labels }));
        }
    }
    modules.sort_by(|a, b| (a.0, &a.1.object).cmp(&(b.0, &b.1.object)));
    let modules = std::sync::Arc::new(modules.into_iter().map(|(_, module)| module).collect::<Vec<_>>());
    KNOWN.lock().expect("the scans").insert(path, (stamp, modules.clone()));
    Ok(modules)
}

/// The assembler sources under a folder of the checkout, and with `midi`
/// the MIDI sources, as paths from the checkout.
fn module_sources(root: &Path, folder: &str, midi: bool, found: &mut Vec<String>) -> Result<(), String> {
    let Ok(entries) = fs::read_dir(root.join(folder)) else {
        return Ok(());
    };
    for entry in entries {
        let entry = entry.map_err(|error| format!("{folder}: {error}"))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        let path = format!("{folder}/{name}");
        let kind = entry.file_type().map_err(|error| format!("{path}: {error}"))?;
        if kind.is_dir() {
            module_sources(root, &path, midi, found)?;
        } else if name.ends_with(".s") || midi && name.ends_with(".mid") {
            found.push(path);
        }
    }
    Ok(())
}

/// The song headers a song table names, in the order it first names each:
/// a row is `song HEADER, PLAYER`.
fn song_headers(text: &str) -> Vec<String> {
    let mut headers: Vec<String> = Vec::new();
    for line in text.lines() {
        let Some(row) = line.trim().strip_prefix("song").filter(|rest| rest.starts_with(char::is_whitespace)) else {
            continue;
        };
        let header = row.split(',').next().unwrap_or("").trim();
        if header.starts_with(|c: char| c.is_ascii_alphabetic() || c == '_') && !headers.iter().any(|known| known == header) {
            headers.push(header.to_owned());
        }
    }
    headers
}

pub fn entry(args: &[String]) -> std::process::ExitCode {
    match args {
        [] => {}
        [flag] if flag == "-h" || flag == "--help" => {
            println!("{USAGE}");
            return std::process::ExitCode::SUCCESS;
        }
        [flag] if flag == "--rows" => {
            return match rows_text(crate::compiler::routing::root()) {
                Ok(text) => {
                    print!("{text}");
                    std::process::ExitCode::SUCCESS
                }
                Err(error) => {
                    eprintln!("error: {error}");
                    std::process::ExitCode::FAILURE
                }
            };
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

/// Each game's list with every row an object's: what the build links.
fn rows_text(root: &Path) -> Result<String, String> {
    let mut text = String::new();
    for game in TARGET_IDS.chunks(6) {
        if let Some(rows) = read(root, target_for(game[0]))? {
            text += &write_rows(&rows, None);
        }
    }
    Ok(text)
}

fn run(root: &Path) -> Result<String, String> {
    let mut summary = Vec::new();
    let mut failures = Vec::new();
    for game in TARGET_IDS.chunks(6) {
        let first = target_for(game[0]);
        let Some(list) = first.object_list() else {
            continue;
        };
        let shown = list.display();
        let text = fs::read_to_string(root.join(&list)).map_err(|error| format!("{shown}: {error}"))?;
        // The rows as written, a unit row as its three, and then each table
        // row as the modules it names, on the table row's line.
        let written_lines = parse_lines(&text, &sourced_in(root)).map_err(|error| format!("{shown}: {error}"))?;
        let written: Vec<Row> = written_lines.iter().map(|(_, row)| row.clone()).collect();
        let stood = stands_for(root, &written).map_err(|error| format!("{shown}: {error}"))?;
        let lines: Vec<(usize, Row)> = written_lines
            .iter()
            .zip(stood)
            .flat_map(|((line, _), rows)| rows.into_iter().map(move |row| (*line, row)))
            .collect();
        let rows: Vec<Row> = lines.iter().map(|(_, row)| row.clone()).collect();
        failures.extend(repeats(&lines).into_iter().map(|found| format!("{shown}: {found}")));
        failures.extend(unsourced(root, &rows).into_iter().map(|found| format!("{shown}: {found}")));
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
        let tables = written.iter().filter(|row| row.object.starts_with(NAMED_BY)).count();
        let named = match tables {
            0 => String::new(),
            _ => format!(", {} of them named by {tables} tables", rows.len() + tables - written.len()),
        };
        summary.push(format!(
            "{shown}: {} rows standing for {} of {} images{named}, each linked where its script includes its place; {resources} resources in their directory's order",
            text.lines().count() - 1,
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

/// The objects the checkout has no source for, each once for an image. A
/// unit whose table is missing is found here: with no table of its own, the
/// row stands for its folder's, and that has no source either.
fn unsourced(root: &Path, rows: &[Row]) -> Vec<String> {
    let mut found = Vec::new();
    let mut seen = BTreeSet::new();
    for row in rows.iter().filter(|row| row.built() && seen.insert((row.image.as_str(), row.object.as_str()))) {
        // An overlay's own data and bss are resource_<id>_overlay.o, which
        // the build assembles from ovl_<id>.s, or empty without one.
        if row.image != MAIN && row.object == format!("resource_{}_overlay", row.image) {
            continue;
        }
        let Err(error) = crate::build_rom::source_for(root, &row.object) else {
            continue;
        };
        match row.object.rsplit_once('/') {
            Some((_, table @ (ENTRY | IMPORT))) if row.image != MAIN => found.push(format!(
                "{error}: a unit's {table} table is {}/NAME.s for its object NAME, or {}.s for its folder",
                row.object, row.object
            )),
            _ => found.push(error),
        }
    }
    found
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

/// The directory's rows as the assembler sees them: the file's own group
/// macros and `.irp`, `.irpc`, `.rept` and `.ifnc` expanded, down to the
/// `resource` rows they stand for.
fn directory_rows(text: &str) -> Vec<String> {
    let lines: Vec<String> =
        text.lines().map(|line| line.split('@').next().unwrap_or("").trim().to_owned()).filter(|line| !line.is_empty()).collect();
    let mut macros: BTreeMap<String, (Vec<String>, Vec<String>)> = BTreeMap::new();
    let mut body = Vec::new();
    let mut at = 0;
    while at < lines.len() {
        if let Some(rest) = lines[at].strip_prefix(".macro") {
            let mut words = rest.split(|c: char| c == ',' || c.is_whitespace()).filter(|word| !word.is_empty());
            let name = words.next().unwrap_or("").to_owned();
            let parameters = words.map(|word| word.split(':').next().unwrap_or("").to_owned()).collect();
            let mut inner = Vec::new();
            at += 1;
            while at < lines.len() && lines[at] != ".endm" {
                inner.push(lines[at].clone());
                at += 1;
            }
            macros.insert(name, (parameters, inner));
        } else {
            body.push(lines[at].clone());
        }
        at += 1;
    }
    // `resource` is the row itself.
    macros.remove("resource");
    let mut rows = Vec::new();
    expand_rows(&body, &macros, &mut rows, 0);
    rows
}

fn expand_rows(lines: &[String], macros: &BTreeMap<String, (Vec<String>, Vec<String>)>, rows: &mut Vec<String>, depth: usize) {
    if depth > 8 {
        return;
    }
    let bound = |inner: &[String], names: &[(String, String)]| -> Vec<String> { inner.iter().map(|line| substitute(line, names)).collect() };
    let mut at = 0;
    while at < lines.len() {
        let line = &lines[at];
        let (word, rest) = line.split_once(char::is_whitespace).map_or((line.as_str(), ""), |(word, rest)| (word, rest.trim()));
        if matches!(word, ".irp" | ".irpc" | ".rept" | ".ifnc") {
            let repeats = word != ".ifnc";
            let opens = |w: &str| if repeats { matches!(w, ".irp" | ".irpc" | ".rept") } else { w == ".ifnc" };
            let close = if repeats { ".endr" } else { ".endif" };
            let (mut level, mut end) = (1, at + 1);
            while end < lines.len() {
                let first = lines[end].split_whitespace().next().unwrap_or("");
                if opens(first) {
                    level += 1;
                } else if first == close {
                    level -= 1;
                    if level == 0 {
                        break;
                    }
                }
                end += 1;
            }
            let inner = &lines[at + 1..end.min(lines.len())];
            let (first, others) = rest.split_once(',').map_or((rest, ""), |(first, others)| (first.trim(), others.trim()));
            match word {
                ".rept" => {
                    for _ in 0..rest.parse::<usize>().unwrap_or(0) {
                        expand_rows(inner, macros, rows, depth + 1);
                    }
                }
                ".irp" => {
                    for value in others.split(',').map(str::trim) {
                        expand_rows(&bound(inner, &[(first.to_owned(), value.to_owned())]), macros, rows, depth + 1);
                    }
                }
                ".irpc" => {
                    for value in others.chars() {
                        expand_rows(&bound(inner, &[(first.to_owned(), value.to_string())]), macros, rows, depth + 1);
                    }
                }
                _ => {
                    if first != others {
                        expand_rows(inner, macros, rows, depth + 1);
                    }
                }
            }
            at = end + 1;
            continue;
        }
        if let Some((parameters, inner)) = macros.get(word) {
            let arguments: Vec<&str> = rest.split(',').map(str::trim).collect();
            let names: Vec<(String, String)> = parameters
                .iter()
                .enumerate()
                .map(|(index, parameter)| {
                    // The last parameter takes what is left: a list.
                    let value = if index + 1 == parameters.len() {
                        arguments.get(index..).map_or(String::new(), |left| left.join(", "))
                    } else {
                        arguments.get(index).copied().unwrap_or("").to_owned()
                    };
                    (parameter.clone(), value)
                })
                .collect();
            expand_rows(&bound(inner, &names), macros, rows, depth + 1);
        } else if word == "resource" {
            rows.push(line.clone());
        }
        at += 1;
    }
}

/// `line` with each `\name` (and `\name\()`) replaced by its value, longer
/// names first.
fn substitute(line: &str, names: &[(String, String)]) -> String {
    let mut order: Vec<&(String, String)> = names.iter().collect();
    order.sort_by_key(|(name, _)| std::cmp::Reverse(name.len()));
    let mut out = line.to_owned();
    for (name, value) in order {
        out = out.replace(&format!("\\{name}\\()"), value).replace(&format!("\\{name}"), value);
    }
    out
}

/// The places a resource directory names, in the order it first names each:
/// a row is `resource NAME, PLACE`, and a place of 0 is an unused row.
fn directory_places(text: &str) -> Vec<String> {
    let mut places: Vec<String> = Vec::new();
    for line in directory_rows(text) {
        let line = line.as_str();
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
fn repeats(rows: &[(usize, Row)]) -> Vec<String> {
    let mut seen = BTreeSet::new();
    let mut found = Vec::new();
    for (line, row) in rows {
        for section in row.sections.split(' ') {
            for edition in &row.editions {
                if !seen.insert((row.image.as_str(), row.object.as_str(), section, *edition)) {
                    found.push(format!(
                        "line {line}: {} {section} of {} is already listed for {edition}",
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

    /// The list read in a checkout where no unit has a table of its own:
    /// every unit row is its folder's entry and import tables.
    fn parse(text: &str) -> Result<Vec<Row>, String> {
        Ok(parse_lines(text)?.into_iter().map(|(_, row)| row).collect())
    }

    fn parse_lines(text: &str) -> Result<Vec<(usize, Row)>, String> {
        super::parse_lines(text, &|_| false)
    }

    fn format(rows: &[Row]) -> String {
        write_rows(rows, Some(&|_| false))
    }

    #[test]
    fn a_directory_s_group_statements_expand_to_its_rows() {
        let text = "\t.macro resource name, place:req\n\t.4byte \\place\n\t.endm\n\t.macro area set:req, sheets:req, maps:vararg\n\t.irp m, \\maps\n\tresource , Field_Map\\m\n\t.endr\n\tresource , Tileset_\\set\\()Palette\n\t.ifnc \\sheets, -\n\t.irpc s, \\sheets\n\tresource , Tileset_\\set\\()Tiles\\s\n\t.endr\n\t.endif\n\t.endm\n\t.macro unused count:req\n\t.rept \\count\n\tresource , 0\n\t.endr\n\t.endm\nTable:\n\tresource First, Rom_Start\n\tarea Set01, AB, 002, 003\n\tarea Set02, -, 004\n\tunused 2\n";
        assert_eq!(
            directory_places(text),
            ["Rom_Start", "Field_Map002", "Field_Map003", "Tileset_Set01Palette", "Tileset_Set01TilesA", "Tileset_Set01TilesB", "Field_Map004", "Tileset_Set02Palette"]
        );
        assert_eq!(directory_rows(text).len(), 10);
    }

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
        let rows = |text: &str| parse_lines(&list(text)).unwrap();
        assert_eq!(repeats(&rows(AGREEING)), Vec::<String>::new());
        let twice = format!("{AGREEING}main\trom_main_01\tgames/g/b\t.rodata .text\tde\n");
        assert_eq!(repeats(&rows(&twice)), ["line 8: main .text of games/g/b is already listed for de"]);
        let apart = "main\trom_main_00\tgames/g/a\t.text\tja\nmain\trom_main_01\tgames/g/a\t.text\ten de\n3ad\ttext\tgames/g/a\t.text\n";
        assert_eq!(repeats(&rows(apart)), Vec::<String>::new());
        // A unit row gives its three objects, and the line named is the file's.
        let unit = "3ad\tunit\tgames/g/a/a\n3ad\tbss\tgames/g/a/a\t.bss\n3ad\ttext\tgames/g/a/import\t.rodata\tja\n";
        assert_eq!(repeats(&rows(unit)), ["line 4: 3ad .rodata of games/g/a/import is already listed for ja"]);
    }

    /// `text` with each unit row written out as the three rows it stands
    /// for, by the convention's words alone: the entry table, the object,
    /// the import table. A table is the object's own, in the folder named
    /// `entry` or `import`, where `sourced` has it, and else that name in
    /// the object's folder.
    fn written_out(text: &str, sourced: Sourced) -> String {
        let mut out = String::new();
        for line in text.lines() {
            let fields: Vec<&str> = line.split('\t').collect();
            if fields.get(1) != Some(&"unit") {
                out += &format!("{line}\n");
                continue;
            }
            let (folder, name) = fields[2].rsplit_once('/').unwrap();
            let table = |table: &str| {
                let own = format!("{table}/{name}");
                if sourced(&format!("{folder}/{own}")) {
                    own
                } else {
                    table.to_owned()
                }
            };
            for object in [table("entry"), name.to_owned(), table("import")] {
                let mut row = vec![fields[0].to_owned(), "text".to_owned(), format!("{folder}/{object}")];
                row.extend(fields[3..].iter().map(|field| (*field).to_owned()));
                out += &format!("{}\n", row.join("\t"));
            }
        }
        out
    }

    #[test]
    fn a_unit_row_is_its_folders_entry_table_the_object_and_the_import_table() {
        let written = list(
            "3a5\tunit\tgames/g/sabaku/sabaku\n\
             3a5\ttext\tlibgcc.a:_call_via_rX\n\
             3a5\ttext\tgames/g/graphics/sand\n\
             3a5\tbss\tresource_3a5_overlay\t.bss\n\
             3ae\tunit\tgames/g/torebi/torebi\t\tja\n\
             3ae\ttext\tgames/g/torebi/late\t.text\ten de es fr it\n",
        );
        let explicit = list(
            "3a5\ttext\tgames/g/sabaku/entry\n\
             3a5\ttext\tgames/g/sabaku/sabaku\n\
             3a5\ttext\tgames/g/sabaku/import\n\
             3a5\ttext\tlibgcc.a:_call_via_rX\n\
             3a5\ttext\tgames/g/graphics/sand\n\
             3a5\tbss\tresource_3a5_overlay\t.bss\n\
             3ae\ttext\tgames/g/torebi/entry\t\tja\n\
             3ae\ttext\tgames/g/torebi/torebi\t\tja\n\
             3ae\ttext\tgames/g/torebi/import\t\tja\n\
             3ae\ttext\tgames/g/torebi/late\t.text\ten de es fr it\n",
        );
        assert_eq!(written_out(&written, &|_| false), explicit);
        // The rows are those of the list written out: no row is a unit's.
        let rows = parse(&written).unwrap();
        assert_eq!(rows, parse(&explicit).unwrap());
        assert_eq!(rows.len(), 10);
        assert!(rows.iter().all(|row| row.place != UNIT));
        let lines: Vec<usize> = parse_lines(&written).unwrap().into_iter().map(|(line, _)| line).collect();
        assert_eq!(lines, [2, 2, 2, 3, 4, 5, 6, 6, 6, 7]);
        // The statements are the same: the place is still a list of whole
        // objects, linked pass by pass with the library member after the
        // import table.
        let bss = parse_bss("overlay\tbss\n3a5\t.bss ALIGN(16)\n").unwrap();
        let fragment = |name: &str, text: &str| (name.to_owned(), text.to_owned());
        assert_eq!(
            overlay_fragments(&rows, &bss, "3a5", "en").unwrap(),
            [
                fragment("extern", "EXTERN(_call_via_r0)\n"),
                fragment(
                    "text",
                    "    \"*/games/g/sabaku/entry.o\"(.text .padding)\n\
                     \x20   \"*/games/g/sabaku/sabaku.o\"(.text .padding)\n\
                     \x20   \"*/games/g/sabaku/import.o\"(.text .padding)\n\
                     \x20   \"*libgcc.a:_call_via_rX.o\"(.text .padding)\n\
                     \x20   \"*/games/g/graphics/sand.o\"(.text .padding)\n\
                     \x20   \"*/games/g/sabaku/entry.o\"(.rodata)\n\
                     \x20   \"*/games/g/sabaku/sabaku.o\"(.rodata)\n\
                     \x20   \"*/games/g/sabaku/import.o\"(.rodata)\n\
                     \x20   \"*libgcc.a:_call_via_rX.o\"(.rodata)\n\
                     \x20   \"*/games/g/graphics/sand.o\"(.rodata)\n\
                     \x20   \"*/games/g/sabaku/entry.o\"(.data .overlays)\n\
                     \x20   \"*/games/g/sabaku/sabaku.o\"(.data .overlays)\n\
                     \x20   \"*/games/g/sabaku/import.o\"(.data .overlays)\n\
                     \x20   \"*libgcc.a:_call_via_rX.o\"(.data .overlays)\n\
                     \x20   \"*/games/g/graphics/sand.o\"(.data .overlays)\n"
                ),
                fragment("bss", "    .bss ALIGN(16) :\n    {\n    \"*/resource_3a5_overlay.o\"(.bss)\n    }\n"),
                fragment("discard", ""),
            ]
        );
        // A unit row's editions are its three rows'.
        let all = ".text .rodata .data .padding";
        assert_eq!(
            render(&rows, "3ae", "ja")[0].1,
            format!("    \"*/games/g/torebi/entry.o\"({all})\n    \"*/games/g/torebi/torebi.o\"({all})\n    \"*/games/g/torebi/import.o\"({all})\n")
        );
        assert_eq!(render(&rows, "3ae", "en")[0].1, "    \"*/games/g/torebi/late.o\"(.text)\n");
        // Written back, three rows that are one are its unit row again.
        assert_eq!(format(&rows), written);
        assert_eq!(format(&parse(&explicit).unwrap()), written);
    }

    #[test]
    fn only_three_whole_rows_of_one_overlay_folder_are_a_unit() {
        let kept = |rows: &str| {
            let text = list(rows);
            assert_eq!(format(&parse(&text).unwrap()), text);
        };
        // The main image has no units.
        kept("main\trom_start\tgames/g/a/entry\nmain\trom_start\tgames/g/a/a\nmain\trom_start\tgames/g/a/import\n");
        // An edition that leaves one of the three out.
        kept("3ad\ttext\tgames/g/a/entry\n3ad\ttext\tgames/g/a/a\n3ad\ttext\tgames/g/a/import\t\tja\n");
        // One of the three placed by its sections.
        kept("3ad\ttext\tgames/g/a/entry\n3ad\ttext\tgames/g/a/a\t.text .rodata\n3ad\ttext\tgames/g/a/import\n");
        // Tables of another folder, an object between, another overlay's.
        kept("3ad\ttext\tgames/g/b/entry\n3ad\ttext\tgames/g/a/a\n3ad\ttext\tgames/g/a/import\n");
        kept("3ad\ttext\tgames/g/a/entry\n3ad\ttext\tgames/g/a/a\n3ad\ttext\tgames/g/a/b\n3ad\ttext\tgames/g/a/import\n");
        kept("3ad\ttext\tgames/g/a/entry\n3ad\ttext\tgames/g/a/a\n3ae\ttext\tgames/g/a/import\n");
        // A library member is no unit, and the tables are not each other's.
        kept("3ad\ttext\tgames/g/entry\n3ad\ttext\tgames/g/libgcc.a:_call_via_rX\n3ad\ttext\tgames/g/import\n");
        kept("3ad\ttext\tgames/g/a/entry\n3ad\ttext\tgames/g/a/entry\t.rodata.late\n3ad\ttext\tgames/g/a/import\n");
        // A unit after an entry table of its own is still found.
        let late = list("3ad\ttext\tgames/g/a/entry\t.text.first\n3ad\tunit\tgames/g/a/a\n");
        assert_eq!(parse(&late).unwrap().len(), 4);
        kept(late.strip_prefix(&list("")).unwrap());

        let refused = |rows: &str| parse(&list(rows)).unwrap_err();
        assert!(refused("main\tunit\tgames/g/a/a\n").contains("line 2: a unit row is an overlay's"));
        assert!(refused("3ad\tunit\tgames/g/a/a\t.text\n").contains("names no sections"));
        assert!(refused("3ad\tunit\tgames/g/a/a\t.text .rodata .data .padding\n").contains("names no sections"));
        for object in ["a", "libgcc.a:_call_via_rX", "games/g/libgcc.a:_call_via_rX", "*", "games/g/a/entry", "games/g/a/import", "games/g/a/"] {
            let error = refused(&format!("3ad\tunit\t{object}\n"));
            assert!(error.contains("is not an object of a folder") || error.contains("is not a path"), "{object}: {error}");
        }
        assert!(refused("3ad\tunits\tgames/g/a/a\n").contains("not one of an overlay's: text bss discard, or unit"));
        // The editions of a unit row are read as any row's.
        assert!(refused("3ad\tunit\tgames/g/a/a\t\ten ja\n").contains("order"));
    }

    /// A checkout of three overlays: a title with a folder of its own, and
    /// two scenes whose C files share one folder, with their tables in its
    /// `entry` and `import` folders.
    fn checkout() -> tempfile::TempDir {
        let root = tempfile::tempdir().unwrap();
        for source in [
            "games/g/title/entry.s",
            "games/g/title/title.c",
            "games/g/title/import.s",
            "games/g/scene/entry/haidia_mura.s",
            "games/g/scene/haidia_mura.c",
            "games/g/scene/import/haidia_mura.s",
            "games/g/scene/entry/fune_heya.s",
            "games/g/scene/fune_heya.c",
            "games/g/scene/import/fune_heya.s",
        ] {
            let path = root.path().join(source);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, "\n").unwrap();
        }
        root
    }

    const BOTH_LAYOUTS: &str = "36f\tunit\tgames/g/title/title\n\
         373\tunit\tgames/g/scene/haidia_mura\n\
         373\ttext\tlibgcc.a:_call_via_rX\n\
         3b1\tunit\tgames/g/scene/fune_heya\t\tja en\n\
         3b1\tbss\tgames/g/scene/fune_heya\t.bss COMMON\n";

    #[test]
    fn a_unit_row_finds_its_tables_in_a_folder_of_its_own_or_in_one_all_units_share() {
        let root = checkout();
        let written = list(BOTH_LAYOUTS);
        let rows = super::parse(root.path(), &written).unwrap();
        let objects: Vec<&str> = rows.iter().map(|row| row.object.as_str()).collect();
        assert_eq!(
            objects,
            [
                // A folder of its own: the tables beside the C file.
                "games/g/title/entry",
                "games/g/title/title",
                "games/g/title/import",
                // One folder for all: each table named as its C file.
                "games/g/scene/entry/haidia_mura",
                "games/g/scene/haidia_mura",
                "games/g/scene/import/haidia_mura",
                "libgcc.a:_call_via_rX",
                "games/g/scene/entry/fune_heya",
                "games/g/scene/fune_heya",
                "games/g/scene/import/fune_heya",
                "games/g/scene/fune_heya",
            ]
        );
        assert!(rows[..10].iter().all(|row| row.place == TEXT && row.sections == DEFAULT_SECTIONS));
        assert_eq!(rows[7].editions, ["ja", "en"]);
        let sourced = sourced_in(root.path());
        assert_eq!(rows, super::parse(root.path(), &written_out(&written, &sourced)).unwrap());
        assert_eq!(unsourced(root.path(), &rows), Vec::<String>::new());
        assert_eq!(repeats(&super::parse_lines(&written, &sourced).unwrap()), Vec::<String>::new());
        // Written back, each three rows are their unit row again, and the
        // rows written out one by one are read as the same units.
        assert_eq!(super::format(root.path(), &rows), written);
        assert_eq!(write_rows(&rows, None).lines().count(), 12);
        assert_eq!(super::format(root.path(), &super::parse(root.path(), &write_rows(&rows, None)).unwrap()), written);
        // The linker's statements name each table where it is.
        assert_eq!(
            render(&rows, "373", "en")[0].1.lines().take(4).collect::<Vec<_>>(),
            [
                "    \"*/games/g/scene/entry/haidia_mura.o\"(.text .padding)",
                "    \"*/games/g/scene/haidia_mura.o\"(.text .padding)",
                "    \"*/games/g/scene/import/haidia_mura.o\"(.text .padding)",
                "    \"*libgcc.a:_call_via_rX.o\"(.text .padding)",
            ]
        );
        assert_eq!(tables(root.path(), "games/g/scene/fune_heya").unwrap(), ["games/g/scene/entry/fune_heya", "games/g/scene/import/fune_heya"]);
        assert_eq!(tables(root.path(), "games/g/title/title").unwrap(), ["games/g/title/entry", "games/g/title/import"]);
        assert_eq!(tables(root.path(), "games/g/scene/entry"), None);
        assert_eq!(tables(root.path(), "libgcc.a:_call_via_rX"), None);
        // In this checkout a scene's three rows by the other layout's names
        // are no unit: its tables are its own.
        let folder_form = list("373\ttext\tgames/g/scene/entry\n373\ttext\tgames/g/scene/haidia_mura\n373\ttext\tgames/g/scene/import\n");
        assert_eq!(super::format(root.path(), &super::parse(root.path(), &folder_form).unwrap()), folder_form);
        // The same text in a checkout without those tables is the folder's.
        let elsewhere: Vec<String> = parse(&written).unwrap().into_iter().map(|row| row.object).collect();
        assert_eq!(elsewhere[3..6], ["games/g/scene/entry", "games/g/scene/haidia_mura", "games/g/scene/import"]);
        assert!(import_table("games/g/title/import") && import_table("games/g/scene/import/fune_heya"));
        assert!(!import_table("games/g/scene/entry/fune_heya") && !import_table("games/g/scene/fune_heya") && !import_table("import/a/b"));
    }

    #[test]
    fn a_unit_without_a_table_is_refused() {
        let root = checkout();
        let rows = |root: &Path| super::parse(root, &list(BOTH_LAYOUTS)).unwrap();
        // Neither a table of its own nor its folder's: the entry table.
        fs::remove_file(root.path().join("games/g/scene/entry/fune_heya.s")).unwrap();
        assert_eq!(
            unsourced(root.path(), &rows(root.path())),
            ["games/g/scene/entry.o has no C, assembly or MIDI source: a unit's entry table is games/g/scene/entry/NAME.s for its object NAME, or games/g/scene/entry.s for its folder"]
        );
        // The import table of a unit with a folder of its own.
        fs::write(root.path().join("games/g/scene/entry/fune_heya.s"), "\n").unwrap();
        fs::remove_file(root.path().join("games/g/title/import.s")).unwrap();
        assert_eq!(
            unsourced(root.path(), &rows(root.path())),
            ["games/g/title/import.o has no C, assembly or MIDI source: a unit's import table is games/g/title/import/NAME.s for its object NAME, or games/g/title/import.s for its folder"]
        );
        // The C file itself, and an object of the main image, as before.
        fs::write(root.path().join("games/g/title/import.s"), "\n").unwrap();
        fs::remove_file(root.path().join("games/g/scene/haidia_mura.c")).unwrap();
        let main = super::parse(root.path(), &list("main\trom_start\tgames/g/entry\n")).unwrap();
        assert_eq!(unsourced(root.path(), &[rows(root.path()), main].concat()), [
            "games/g/scene/haidia_mura.o has no C, assembly or MIDI source",
            "games/g/entry.o has no C, assembly or MIDI source",
        ]);
    }

    #[test]
    fn one_import_table_may_serve_two_overlays_and_not_one_overlay_twice() {
        // Two scenes with entry tables of their own and no import table of
        // their own: both rows stand for the folder's.
        let sourced: Sourced = &|object| object.starts_with("games/g/scene/entry/");
        let rows = |text: &str| super::parse_lines(&list(text), sourced).unwrap();
        let shared = "373\tunit\tgames/g/scene/haidia_mura\n3b1\tunit\tgames/g/scene/fune_heya\n";
        let objects: Vec<(String, String)> = rows(shared).into_iter().map(|(_, row)| (row.image, row.object)).collect();
        assert_eq!(objects[2], ("373".to_owned(), "games/g/scene/import".to_owned()));
        assert_eq!(objects[5], ("3b1".to_owned(), "games/g/scene/import".to_owned()));
        assert_eq!(repeats(&rows(shared)), Vec::<String>::new());
        assert_eq!(write_rows(&rows(shared).into_iter().map(|(_, row)| row).collect::<Vec<_>>(), Some(sourced)), list(shared));
        // The same table a second time in one image is a repeat, on its line.
        let doubled = format!("{shared}3b1\ttext\tgames/g/scene/import\t.text\tja\n");
        assert_eq!(repeats(&rows(&doubled)), ["line 4: 3b1 .text of games/g/scene/import is already listed for ja"]);
        // So is a unit row given twice, each of its three objects.
        let twice = format!("{shared}373\tunit\tgames/g/scene/haidia_mura\t\tde\n");
        assert_eq!(
            repeats(&rows(&twice)).iter().filter(|found| found.contains(".text of")).collect::<Vec<_>>(),
            [
                "line 4: 373 .text of games/g/scene/entry/haidia_mura is already listed for de",
                "line 4: 373 .text of games/g/scene/haidia_mura is already listed for de",
                "line 4: 373 .text of games/g/scene/import is already listed for de",
            ]
        );
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
    fn a_place_of_whole_objects_links_code_then_constants_then_data() {
        let rows = parse(&list(
            "3ad\ttext\tgames/g/entry\n\
             3ad\ttext\tgames/g/a\n\
             3ad\ttext\tgames/g/import\n\
             3ad\ttext\tlibgcc.a:_call_via_rX\n\
             3ad\ttext\tgames/g/only_ja\t\tja\n\
             3ad\tbss\tgames/g/a\t.bss COMMON\n\
             3ae\ttext\tgames/g/b\n\
             3ae\ttext\tgames/g/b_data\t.data\n",
        ))
        .unwrap();
        let passes = |objects: &[&str]| -> String {
            PLAIN_PASSES
                .iter()
                .flat_map(|sections| objects.iter().map(move |object| format!("    \"*{object}.o\"({sections})\n")))
                .collect()
        };
        // Every object's code in the list's order, the library member's
        // after the import table's, then the constants, then the data.
        let listed = ["/games/g/entry", "/games/g/a", "/games/g/import", "libgcc.a:_call_via_rX"];
        assert_eq!(render(&rows, "3ad", "en")[0], ("text".to_owned(), passes(&listed)));
        let japanese = [&listed[..], &["/games/g/only_ja"]].concat();
        assert_eq!(render(&rows, "3ad", "ja")[0].1, passes(&japanese));
        // The bss place is its rows, and the member still asks libgcc for it.
        assert_eq!(render(&rows, "3ad", "en")[1].1, "    \"*/games/g/a.o\"(.bss COMMON)\n");
        let bss = parse_bss("overlay\tbss\n3ad\t.bss\n").unwrap();
        assert_eq!(overlay_fragments(&rows, &bss, "3ad", "en").unwrap()[0].1, "EXTERN(_call_via_r0)\n");
        // One row that names its sections, and the place is its rows as written.
        assert_eq!(
            render(&rows, "3ae", "en")[0].1,
            "    \"*/games/g/b.o\"(.text .rodata .data .padding)\n    \"*/games/g/b_data.o\"(.data)\n"
        );
        // A place outside the ROM is never a plain list.
        let ram = parse(&list("main\tiwram_runtime\tgames/g/a\n")).unwrap();
        assert_eq!(render(&ram, MAIN, "en")[0].1, "    \"*/games/g/a.o\"(.text .rodata .data .padding)\n");
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
    fn a_table_row_stands_for_the_modules_its_table_names_in_its_order() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        let directory = "Resource_DirectoryTable:\n\tresource , FarCall_Table\n\tresource , Ui_Tiles\n\
                         \tresource , Title_Sun\n\tresource , Field_Map\n\tresource , Title_Sun\n\tresource , Title_Peak\n";
        write("games/g/dir.s", directory);
        // A row of its own, a source another includes, the other game's
        // module of a name this game defines, and a module the table does
        // not name: none is the table's.
        write("games/g/code.s", "FarCall_Table:\n");
        write("games/common/ui.s", "Ui_Tiles:\n");
        write("games/common/peak.s", "Title_Peak:\n");
        write("games/g/spare.s", "Spare:\n");
        write("games/g/title.s", "\t.include \"games/common/ui.s\"\nTitle_Sun:\nTitle_Peak:\n");
        write("games/g/map/map.s", "Field_Map:\n");
        let text = list("main\trom_main\tgames/g/code\nmain\trom_tail\tgames/g/dir\nmain\trom_tail\t@games/g/dir\t.rodata\tja en\n");
        let written = parse(&text).unwrap();
        let rows = expand(root.path(), written.clone()).unwrap();
        let objects: Vec<&str> = rows.iter().map(|row| row.object.as_str()).collect();
        assert_eq!(objects, ["games/g/code", "games/g/dir", "games/g/title", "games/g/map/map"]);
        assert_eq!((rows[2].place.as_str(), rows[2].sections.as_str()), ("rom_tail", ".rodata"));
        assert_eq!(rows[3].editions, ["ja", "en"]);
        // A tool's rows are written with the table's row again, beside its
        // own edit; an edit of the table's rows is refused.
        assert_eq!(format(&collapse(root.path(), &written, rows.clone()).unwrap()), text);
        let mut edited = rows.clone();
        edited.push(Row { object: "games/g/spare".into(), ..rows[0].clone() });
        let kept = collapse(root.path(), &written, edited).unwrap();
        assert_eq!(format(&kept), format!("{text}main\trom_main\tgames/g/spare\n"));
        let mut edited = rows.clone();
        edited.swap(2, 3);
        assert!(collapse(root.path(), &written, edited).unwrap_err().contains("@games/g/dir stands for were edited"));
        let mut edited = rows.clone();
        edited.remove(3);
        assert!(collapse(root.path(), &written, edited).unwrap_err().contains("were edited"));
        // Two of the game's modules define one name: the table cannot choose.
        write("games/g/twice.s", "Field_Map:\n");
        write("games/g/dir.s", &format!("{directory}\tresource , 0\n"));
        assert_eq!(
            expand(root.path(), written.clone()).unwrap_err(),
            "games/g/dir names Field_Map, which games/g/map/map and games/g/twice both define"
        );
        // A table with nothing left to name, and a source that is no table.
        let alone = parse(&list("main\trom_tail\t@games/g/spare\n")).unwrap();
        assert!(expand(root.path(), alone).unwrap_err().contains("has no resource or song row"));
    }

    #[test]
    fn a_song_table_names_its_sequences_by_their_headers() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        let songs = "\t.macro song header:req, player:req\n\t.4byte \\header\n\t.endm\nSound_SongTable:\n\
                     \tsong sound_001, 1\n\tsong Sound_EmptySong, 0\n\tsong sound_000, 1\n\tsong sound_001, 0\n";
        assert_eq!(song_headers(songs), ["sound_001", "Sound_EmptySong", "sound_000"]);
        write("games/g/sound/songs.s", songs);
        write("games/g/sound/players.s", "Sound_EmptySong:\n");
        write("games/g/sound/sequence/bgm_000.s", "sound_000_track:\nsound_000:\n");
        write("games/common/sound/sequence/se_shop.s", "sound_001:\n");
        let written = parse(&list(
            "main\trom_tail\tgames/g/sound/players\t.rodata\nmain\trom_tail\tgames/g/sound/songs\t.rodata\nmain\trom_tail\t@games/g/sound/songs\t.rodata\n",
        ))
        .unwrap();
        let rows = expand(root.path(), written).unwrap();
        let objects: Vec<&str> = rows.iter().map(|row| row.object.as_str()).collect();
        assert_eq!(
            objects,
            ["games/g/sound/players", "games/g/sound/songs", "games/common/sound/sequence/se_shop", "games/g/sound/sequence/bgm_000"]
        );
    }

    #[test]
    fn the_committed_list_is_what_the_build_links() {
        let root = crate::compiler::routing::root();
        let summary = run(root).unwrap();
        assert!(summary.contains("games/tbs/objects.tsv"), "{summary}");
        let text = fs::read_to_string(root.join("games/tbs/objects.tsv")).unwrap();
        let rows = super::parse(root, &text).unwrap();
        assert_eq!(super::format(root, &rows), text);
        // Every overlay's unit row written out as its three rows is the same
        // list: the same fragments for every image and edition.
        let explicit = written_out(&text, &sourced_in(root));
        assert!(explicit.lines().count() > text.lines().count(), "the list has unit rows");
        let rows_written_out = super::parse(root, &explicit).unwrap();
        assert_eq!(rows, rows_written_out);
        let bss = read_bss(root, target_for(TARGET_IDS[0])).unwrap();
        for image in images(&rows) {
            for language in LANGUAGES {
                assert_eq!(render(&rows, &image, language), render(&rows_written_out, &image, language));
                if image != MAIN {
                    assert_eq!(
                        overlay_fragments(&rows, &bss, &image, language),
                        overlay_fragments(&rows_written_out, &bss, &image, language)
                    );
                }
            }
        }
    }
}
