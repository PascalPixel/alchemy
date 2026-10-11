//! A game's object list, games/<game>/objects.tsv: what each image links,
//! one linker statement per row, in link order.
//!
//! Columns: `image` (`main`, or an overlay's resource id such as `3ad`),
//! `place` (the output section, or its span between two fixed placements),
//! `object` (a path from the checkout without `.o`, or an archive member
//! such as `libgcc.a:_call_via_rX`), `sections` (blank means
//! `.text .rodata .data .padding`) and `editions` (blank means all six, else
//! the languages that link the row, such as `ja en`). An object may take
//! several rows: one for each place that takes some of its sections.
//!
//! The build links by the list. The game's one main.ld names no object: it
//! has an `INCLUDE "<place>.ld"` for each place, and the build writes those
//! fragments for each edition from the rows that edition links
//! (out/<edition>/ld). `alchemy check object-list` holds the list to what
//! the linker can read.

use crate::edition::{Image, Item, Statement};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

pub(crate) const HEADER: &str = "image\tplace\tobject\tsections\teditions";
pub(crate) const DEFAULT_SECTIONS: &str = ".text .rodata .data .padding";
pub(crate) const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
pub(crate) const MAIN: &str = "main";

const USAGE: &str = "usage: alchemy check object-list\n\
Fails when a game's object list (games/<game>/objects.tsv) is not what its\n\
build can link: a malformed row, an object with no C, assembly or MIDI\n\
source, an input section of an object given twice to one edition, a place\n\
the game's main.ld does not INCLUDE, or a fragment included outside the\n\
place it is named for. A place is an output section (rom_start), or the\n\
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
    if !plain(object) || object.ends_with(".o") {
        return Err(format!("object {object:?} is not a path or archive member without .o"));
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
                Item::Input(statement) if statement.object != "*" => new.push((statement.object, statement.sections)),
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
        for row in rows.iter().filter(|row| row.built() && sourced.insert(row.object.as_str())) {
            if let Err(error) = crate::build_rom::source_for(root, &row.object) {
                failures.push(format!("{shown}: {error}"));
            }
        }
        if let Some(row) = rows.iter().find(|row| row.image != MAIN) {
            return Err(format!("{shown}: overlay {} has rows, and only the main image links by the list", row.image));
        }
        for id in game {
            let target = target_for(*id);
            let script = crate::edition::image_script(root, target, Image::Main)?;
            let found = disagreements(&rows, MAIN, target.language(), &script)
                .map_err(|error| format!("{}: {error}", target.script().display()))?;
            failures.extend(found.into_iter().map(|found| format!("{id} {found} in {}", target.script().display())));
        }
        summary.push(format!("{shown}: {} rows, each linked where {} includes its place", rows.len(), first.script().display()));
    }
    if !failures.is_empty() {
        return Err(format!("the object list is not what the build can link:\n{}", failures.join("\n")));
    }
    if summary.is_empty() {
        return Ok("no game has an object list".into());
    }
    Ok(summary.join("\n"))
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
        let statements = crate::edition::statements(&wrapped).map_err(|error| format!("{place}: {error}"))?;
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
    fn the_committed_list_is_what_the_build_links() {
        let root = crate::compiler::routing::root();
        let summary = run(root).unwrap();
        assert!(summary.contains("games/tbs/objects.tsv"), "{summary}");
        let text = fs::read_to_string(root.join("games/tbs/objects.tsv")).unwrap();
        assert_eq!(format(&parse(&text).unwrap()), text);
    }
}
