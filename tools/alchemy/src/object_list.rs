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
//! The build still links by the linker scripts. `alchemy check object-list`
//! holds the list to them: for each edition and place, the statements the
//! list renders are the statements the edition's script has.

use crate::edition::{Item, Statement};
use crate::targets::{target_for, DecompTarget, TARGET_IDS};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};

pub(crate) const HEADER: &str = "image\tplace\tobject\tsections\teditions";
pub(crate) const DEFAULT_SECTIONS: &str = ".text .rodata .data .padding";
pub(crate) const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
pub(crate) const MAIN: &str = "main";

const USAGE: &str = "usage: alchemy check object-list\n\
Fails when a game's object list (games/<game>/objects.tsv) and its linker\n\
scripts disagree: for each of the game's six editions and each place of the\n\
main image, the statements the list renders must be the statements of the\n\
edition's script, in order. A place is an output section (rom_start), or the\n\
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
        let path = if self.object.contains(':') { "" } else { "/" };
        format!("\"*{path}{}.o\"({})", self.object, self.sections)
    }
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
            for language in listed.split(' ') {
                let known = LANGUAGES
                    .iter()
                    .position(|known| *known == language)
                    .ok_or_else(|| format!("{language:?} is not one of {}", LANGUAGES.join(" ")))?;
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

/// One edition's linker fragments for an image: each place the list gives
/// it, in the list's order, with the place's statements one per line.
pub(crate) fn render(rows: &[Row], image: &str, language: &str) -> Vec<(String, String)> {
    let mut fragments: Vec<(String, String)> = Vec::new();
    for row in rows.iter().filter(|row| row.links(image, language)) {
        let at = match fragments.iter().position(|(place, _)| *place == row.place) {
            Some(at) => at,
            None => {
                fragments.push((row.place.clone(), String::new()));
                fragments.len() - 1
            }
        };
        fragments[at].1 += &format!("    {}\n", row.statement());
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

pub(crate) fn list_path(target: DecompTarget) -> PathBuf {
    Path::new(target.game_dir()).join("objects.tsv")
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
        let list = list_path(target_for(game[0]));
        if !root.join(&list).is_file() {
            continue;
        }
        let text = fs::read_to_string(root.join(&list)).map_err(|error| format!("{}: {error}", list.display()))?;
        let rows = parse(&text).map_err(|error| format!("{}: {error}", list.display()))?;
        if let Some(row) = rows.iter().find(|row| row.image != MAIN) {
            return Err(format!(
                "{}: overlay {} has rows, and only the main image is checked against its script",
                list.display(),
                row.image
            ));
        }
        for id in game {
            let target = target_for(*id);
            let script = crate::edition::image_script(root, target, crate::edition::Image::Main)?;
            let shown = target.edition_script().unwrap_or_else(|| target.script());
            let found = disagreements(&rows, target.language(), &script)
                .map_err(|error| format!("{}: {error}", shown.display()))?;
            failures.extend(found.into_iter().map(|found| format!("{id} {found} in {}", shown.display())));
        }
        summary.push(format!("{}: {} rows agree with the six scripts", list.display(), rows.len()));
    }
    if !failures.is_empty() {
        return Err(format!("the object list and the linker scripts disagree:\n{}", failures.join("\n")));
    }
    if summary.is_empty() {
        return Ok("no game has an object list".into());
    }
    Ok(summary.join("\n"))
}

/// Where one edition's script and the list's main image differ: the first
/// differing statement of each place.
fn disagreements(rows: &[Row], language: &str, script: &str) -> Result<Vec<String>, String> {
    let scripted = places(script)?;
    let mut listed = BTreeMap::new();
    for (place, fragment) in render(rows, MAIN, language) {
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
        let rows = parse(&list("main\trom_start\tgames/g/a\nmain\trom_start\tgames/g/b\t.rodata\tja en\nmain\trom_start\tgames/g/c\t\tde\n")).unwrap();
        assert_eq!(rows[0].sections, DEFAULT_SECTIONS);
        assert_eq!(rows[0].editions, LANGUAGES);
        assert_eq!(rows[1].sections, ".rodata");
        assert_eq!(rows[1].editions, ["ja", "en"]);
        assert_eq!(rows[2].sections, DEFAULT_SECTIONS);
        assert_eq!(rows[2].editions, ["de"]);
        let en: Vec<_> = rows.iter().filter(|row| row.links(MAIN, "en")).map(|row| row.object.as_str()).collect();
        assert_eq!(en, ["games/g/a", "games/g/b"]);
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
             main\tdiscard\tgames/g/a\t.text.unused\n",
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
            ]
        );
        let de = render(&rows, MAIN, "de");
        assert_eq!(
            de[0].1,
            "    \"*/games/g/a.o\"(.text)\n    \"*libgcc.a:_call_via_rX.o\"(.text)\n    \"*/games/g/late.o\"(.text)\n"
        );
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

    #[test]
    fn the_list_and_a_script_agree_statement_by_statement() {
        let agreeing = "main\tram\tgames/g/ram\t.sym_ram\n\
             main\trom_main_00\tgames/g/a\n\
             main\trom_main_00\tlibgcc.a:_call_via_rX\t.text\n\
             main\trom_main_01\tgames/g/b\t.text\n\
             main\trom_main_01\tgames/g/a\t.rodata.late\n\
             main\tdiscard\tgames/g/b\t.text.unused\n";
        let check = |rows: &str| disagreements(&parse(&list(rows)).unwrap(), "en", SCRIPT).unwrap();
        assert_eq!(check(agreeing), Vec::<String>::new());
        let moved = agreeing.replace("rom_main_01\tgames/g/b", "rom_main_00\tgames/g/b");
        assert_eq!(
            check(&moved),
            [
                "rom_main_00: statement 3 is games/g/b(.text) in the list and nothing",
                "rom_main_01: statement 1 is games/g/a(.rodata.late) in the list and games/g/b(.text)",
            ]
        );
        let other_edition = agreeing.replace("games/g/b\t.text\n", "games/g/b\t.text\tja\n");
        assert_eq!(check(&other_edition).len(), 1);
        let sections = agreeing.replace("main\trom_main_00\tgames/g/a\n", "main\trom_main_00\tgames/g/a\t.text\n");
        assert!(check(&sections)[0].contains("games/g/a(.text) in the list and games/g/a(.text .rodata .data .padding)"));
        let stray = format!("{agreeing}main\trom_tail\tgames/g/c\n");
        assert_eq!(check(&stray), ["rom_tail: the list has a place the script has not"]);
    }

    #[test]
    fn the_committed_list_agrees_with_every_script() {
        let summary = run(crate::compiler::routing::root()).unwrap();
        assert!(summary.contains("games/tbs/objects.tsv"), "{summary}");
    }
}
