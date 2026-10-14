//! The object list as the linker and make read it.
//!
//! `objlist OBJECTS.tsv OVERLAYS.tsv LANG BUILD` writes one edition's linker
//! fragments from a game's object list: BUILD/ld/PLACE.ld for each place of
//! the main image, which games/<game>/main.ld INCLUDEs, and
//! BUILD/ld/ID/{extern,text,bss,discard}.ld for each overlay, which
//! overlay.ld INCLUDEs. It also writes BUILD/objects.mk, the objects make
//! builds and links.
//!
//! `objlist --scripts MAIN.ld OVERLAY_DIR BUILD` writes only BUILD/objects.mk,
//! for a game whose linker scripts name their own objects (⚓️): the main
//! script's objects, and each overlay's from OVERLAY_DIR/resource_ID.ld.
//!
//! A row of the list is `image place object [sections] [editions]`: blank
//! sections are `.text .rodata .data .padding`, blank editions are all six.
//! A plain place (a ROM place of the main image, or an overlay's text,
//! whose rows all take the default sections) links every object's code in
//! the list's order, then every object's read-only data, then its data.

use std::fs;
use std::process::ExitCode;

const DEFAULT_SECTIONS: &str = ".text .rodata .data .padding";
const PLAIN_PASSES: [&str; 3] = [".text .padding", ".rodata", ".data .overlays"];
const MAIN: &str = "main";
const CALL_VIA: &str = "libgcc.a:_call_via_rX";

struct Row {
    image: String,
    place: String,
    object: String,
    sections: String,
    editions: Option<Vec<String>>,
}

impl Row {
    fn links(&self, image: &str, language: &str) -> bool {
        self.image == image
            && self
                .editions
                .as_ref()
                .is_none_or(|editions| editions.iter().any(|edition| edition == language))
    }
}

fn statement(object: &str, sections: &str) -> String {
    if object == "*" {
        return format!("*({sections})");
    }
    let path = if object.contains(':') { "" } else { "/" };
    format!("\"*{path}{object}.o\"({sections})")
}

fn rows(text: &str) -> Result<Vec<Row>, String> {
    let mut rows = Vec::new();
    for (number, line) in text.lines().enumerate().skip(1) {
        if line.trim().is_empty() {
            continue;
        }
        let fields: Vec<&str> = line.split('\t').collect();
        let [image, place, object, rest @ ..] = fields.as_slice() else {
            return Err(format!("line {}: a row has three to five columns", number + 1));
        };
        let sections = rest.first().copied().filter(|text| !text.is_empty()).unwrap_or(DEFAULT_SECTIONS);
        let editions = rest
            .get(1)
            .filter(|text| !text.is_empty())
            .map(|text| text.split(' ').map(str::to_owned).collect());
        rows.push(Row {
            image: (*image).to_owned(),
            place: (*place).to_owned(),
            object: (*object).to_owned(),
            sections: sections.to_owned(),
            editions,
        });
    }
    Ok(rows)
}

fn plain(rows: &[Row], image: &str, place: &str) -> bool {
    let mut own = rows.iter().filter(|row| row.image == image && row.place == place).peekable();
    (if image == MAIN { place.starts_with("rom_") } else { place == "text" })
        && own.peek().is_some()
        && own.all(|row| row.sections == DEFAULT_SECTIONS && row.object != "*")
}

/// Each place of an image in the list's order, with its statements for one
/// edition; a place this edition links nothing in is still written, empty.
fn render(rows: &[Row], image: &str, language: &str) -> Vec<(String, String)> {
    let mut places: Vec<(String, String)> = Vec::new();
    for row in rows.iter().filter(|row| row.image == image) {
        if !places.iter().any(|(place, _)| *place == row.place) {
            places.push((row.place.clone(), String::new()));
        }
    }
    for (place, text) in &mut places {
        let linked: Vec<&Row> = rows.iter().filter(|row| row.place == *place && row.links(image, language)).collect();
        if plain(rows, image, place) {
            for sections in PLAIN_PASSES {
                for row in &linked {
                    *text += &format!("    {}\n", statement(&row.object, sections));
                }
            }
        } else {
            for row in linked {
                *text += &format!("    {}\n", statement(&row.object, &row.sections));
            }
        }
    }
    places
}

fn write(path: &str, text: &str) -> Result<(), String> {
    if let Some(folder) = std::path::Path::new(path).parent() {
        fs::create_dir_all(folder).map_err(|error| format!("{}: {error}", folder.display()))?;
    }
    if fs::read_to_string(path).ok().as_deref() == Some(text) {
        return Ok(());
    }
    fs::write(path, text).map_err(|error| format!("{path}: {error}"))
}

/// The objects make builds, in link order, once each: an archive member or
/// a pattern is the linker's to find, and an overlay's own object
/// (resource_ID_overlay) the Makefile builds and links apart.
fn built<'a>(objects: impl Iterator<Item = &'a str>) -> Vec<String> {
    let mut found: Vec<String> = Vec::new();
    for object in objects {
        if object.contains([':', '*']) || !object.contains('/') || found.iter().any(|known| known == object) {
            continue;
        }
        found.push(object.to_owned());
    }
    found
}

fn makefile(main: &[String], overlays: &[(String, Vec<String>)]) -> String {
    let list = |objects: &[String]| {
        objects.iter().map(|object| format!(" \\\n\t$(BUILD)/obj/{object}.o")).collect::<String>()
    };
    let mut text = String::from("# Written by objlist from the object list: do not edit.\n");
    text += &format!("MAIN_OBJS :={}\n", list(main));
    text += &format!("OVERLAYS :={}\n", overlays.iter().map(|(id, _)| format!(" {id}")).collect::<String>());
    for (id, objects) in overlays {
        text += &format!("OVERLAY_OBJS_{id} :={}\n", list(objects));
    }
    text
}

fn listed(args: &[String]) -> Result<(), String> {
    let [list, bss, language, build] = args else {
        return Err(USAGE.into());
    };
    let rows = rows(&fs::read_to_string(list).map_err(|error| format!("{list}: {error}"))?)?;
    let bss_text = fs::read_to_string(bss).map_err(|error| format!("{bss}: {error}"))?;
    let headers: Vec<(&str, &str)> = bss_text.lines().skip(1).filter_map(|line| line.split_once('\t')).collect();
    for (place, text) in render(&rows, MAIN, language) {
        write(&format!("{build}/ld/{place}.ld"), &text)?;
    }
    let mut images: Vec<&str> = Vec::new();
    for row in rows.iter().filter(|row| row.image != MAIN) {
        if !images.contains(&row.image.as_str()) {
            images.push(&row.image);
        }
    }
    let mut overlays = Vec::new();
    for id in images {
        let mut places = render(&rows, id, language);
        let mut take = |name: &str| {
            places.iter().position(|(place, _)| place == name).map_or(String::new(), |at| places.remove(at).1)
        };
        let (text, uninitialized, discard) = (take("text"), take("bss"), take("discard"));
        let calls = rows.iter().any(|row| row.links(id, language) && row.object == CALL_VIA);
        let header = headers.iter().find(|(overlay, _)| *overlay == id).map(|(_, header)| *header);
        let section = match (header, uninitialized.is_empty()) {
            (Some(header), false) => format!("    {header} :\n    {{\n{uninitialized}    }}\n"),
            (None, true) => String::new(),
            (Some(_), true) => return Err(format!("overlay {id} has a .bss header and no bss row")),
            (None, false) => return Err(format!("overlay {id} has bss rows and no .bss header")),
        };
        write(&format!("{build}/ld/{id}/extern.ld"), if calls { "EXTERN(_call_via_r0)\n" } else { "" })?;
        write(&format!("{build}/ld/{id}/text.ld"), &text)?;
        write(&format!("{build}/ld/{id}/bss.ld"), &section)?;
        write(&format!("{build}/ld/{id}/discard.ld"), &discard)?;
        let objects = built(rows.iter().filter(|row| row.links(id, language)).map(|row| row.object.as_str()));
        overlays.push((id.to_owned(), objects));
    }
    let main = built(rows.iter().filter(|row| row.links(MAIN, language)).map(|row| row.object.as_str()));
    write(&format!("{build}/objects.mk"), &makefile(&main, &overlays))
}

/// The objects a linker script names, `"*/PATH.o"`, in its order.
fn named(script: &str) -> Result<Vec<String>, String> {
    let text = fs::read_to_string(script).map_err(|error| format!("{script}: {error}"))?;
    let objects = text.split("\"*/").skip(1).filter_map(|rest| rest.split_once(".o\"").map(|(object, _)| object));
    Ok(built(objects.filter(|object| object.contains('/'))))
}

fn scripts(args: &[String]) -> Result<(), String> {
    let [main, folder, build] = args else {
        return Err(USAGE.into());
    };
    let mut ids: Vec<String> = fs::read_dir(folder)
        .map_err(|error| format!("{folder}: {error}"))?
        .filter_map(|entry| entry.ok()?.file_name().into_string().ok())
        .filter_map(|name| Some(name.strip_prefix("resource_")?.strip_suffix(".ld")?.to_owned()))
        .collect();
    ids.sort();
    let mut overlays = Vec::new();
    for id in ids {
        let objects = named(&format!("{folder}/resource_{id}.ld"))?;
        overlays.push((id, objects));
    }
    write(&format!("{build}/objects.mk"), &makefile(&named(main)?, &overlays))
}

const USAGE: &str = "usage: objlist OBJECTS.tsv OVERLAYS.tsv LANG BUILD\n       objlist --scripts MAIN.ld OVERLAY_DIR BUILD";

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let result = match args.first().map(String::as_str) {
        Some("--scripts") => scripts(&args[1..]),
        Some("-h" | "--help") | None => Err(USAGE.into()),
        Some(_) => listed(&args),
    };
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("objlist: {error}");
            ExitCode::FAILURE
        }
    }
}
