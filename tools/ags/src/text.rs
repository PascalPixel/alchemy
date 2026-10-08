
use super::asm::{Data, Label, Pointer};
use encoding_rs::WINDOWS_1252;
mod huffman;
use huffman::encode_huffman_archive;
use crate::assets::po::{self, Catalog};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use unicode_normalization::UnicodeNormalization;

const BANK_SIZE: usize = 256;

#[derive(Clone, Debug)]
pub struct ArchiveSpec {
    pub target: &'static str,
    pub language: &'static str,
    pub output: &'static str,
    pub charmap: Option<&'static str>,
}

impl ArchiveSpec {
    pub(crate) fn characters(&self) -> Option<String> {
        self.charmap.map(|charmap| {
            charmap
                .lines()
                .filter(|line| !line.starts_with('#') && !line.is_empty())
                .map(|line| match line.split('\t').nth(1) {
                    Some("SPACE") => ' ',
                    Some("BOX") | None => '\u{fffd}',
                    Some(text) => text.chars().next().unwrap_or('\u{fffd}'),
                })
                .collect()
        })
    }
}

pub static ARCHIVES: [ArchiveSpec; 12] = [
    ArchiveSpec {
        target: "tbs-ja",
        language: "ja",
        output: "games/tbs/text/ja.po",
        charmap: Some(include_str!(
            "../../../games/common/src/graphics/font/charmap_ja.tsv"
        )),
    },
    ArchiveSpec {
        target: "tbs-en",
        language: "en",
        output: "games/tbs/text/en.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-de",
        language: "de",
        output: "games/tbs/text/de.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-es",
        language: "es",
        output: "games/tbs/text/es.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-fr",
        language: "fr",
        output: "games/tbs/text/fr.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-it",
        language: "it",
        output: "games/tbs/text/it.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-ja",
        language: "ja",
        output: "games/tla/text/ja.po",
        charmap: Some(include_str!(
            "../../../games/common/src/graphics/font/charmap_tla_ja.tsv"
        )),
    },
    ArchiveSpec {
        target: "tla-en",
        language: "en",
        output: "games/tla/text/en.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-de",
        language: "de",
        output: "games/tla/text/de.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-es",
        language: "es",
        output: "games/tla/text/es.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-fr",
        language: "fr",
        output: "games/tla/text/fr.po",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-it",
        language: "it",
        output: "games/tla/text/it.po",
        charmap: None,
    },
];

fn command(symbol: u16) -> (&'static str, bool) {
    match symbol {
        1 => ("page_break", false),
        2 => ("end_wait", false),
        3 => ("line_break", false),
        4 => ("pause_60", false),
        5 => ("pause_20_skippable", false),
        6 => ("pause_120_skippable", false),
        7 => ("reset_text_style", false),
        8 => ("text_color", true),
        9 => ("text_effect", true),
        12 => ("control_12", true),
        15 => ("control_15", true),
        16 => ("protagonist_name", false),
        17 => ("actor_name", true),
        18 => ("argument_actor_name", true),
        19 => ("argument_class_name", true),
        20 => ("argument_item_name", true),
        21 => ("argument_ability_name", false),
        22 => ("argument_signed_number", false),
        23 => ("argument_location_name", false),
        24 => ("em_dash", false),
        25 => ("plural_suffix", false),
        26 => ("button_icon", true),
        27 => ("possessive_suffix", false),
        28 => ("control_28", true),
        29 => ("article_class", true),
        30 => ("end_now", false),
        _ => ("control", false),
    }
}

fn command_symbol(name: &str) -> Option<(u16, bool)> {
    (1..32).find_map(|symbol| {
        let value = command(symbol);
        (value.0 == name && value.0 != "control").then_some((symbol, value.1))
    })
}

fn parse_number(value: &str) -> Result<u16, String> {
    value
        .parse::<u16>()
        .map_err(|_| format!("invalid message argument {value}"))
}

fn text_symbols(
    text: &str,
    characters: Option<&str>,
    symbol_count: usize,
) -> Result<Vec<u16>, String> {
    let mut symbols = Vec::new();
    let mut literal = String::new();
    let flush = |literal: &mut String, symbols: &mut Vec<u16>| -> Result<(), String> {
        if literal.is_empty() {
            return Ok(());
        }
        if let Some(characters) = characters {
            for character in literal.nfd() {
                let index = characters
                    .chars()
                    .position(|value| value == character)
                    .filter(|_| character != '\u{fffd}')
                    .ok_or_else(|| {
                        format!("character is not in this edition's font: {character}")
                    })?;
                symbols.push((index + 32) as u16);
            }
            literal.clear();
            return Ok(());
        }
        let (bytes, _, errors) = WINDOWS_1252.encode(literal);
        if errors {
            return Err(format!(
                "text cannot be encoded for this edition: {literal:?}"
            ));
        }
        symbols.extend(bytes.iter().map(|byte| u16::from(*byte)));
        literal.clear();
        Ok(())
    };
    let characters = text.char_indices().collect::<Vec<_>>();
    let mut at = 0;
    while at < characters.len() {
        let (offset, character) = characters[at];
        if character == '{' && characters.get(at + 1).is_some_and(|(_, next)| *next == '{') {
            literal.push('{');
            at += 2;
            continue;
        }
        if character == '}' && characters.get(at + 1).is_some_and(|(_, next)| *next == '}') {
            literal.push('}');
            at += 2;
            continue;
        }
        if character != '{' {
            literal.push(character);
            at += 1;
            continue;
        }
        flush(&mut literal, &mut symbols)?;
        let end = text[offset + 1..]
            .find('}')
            .map(|end| offset + 1 + end)
            .ok_or("unclosed message markup")?;
        let markup = &text[offset + 1..end];
        if let Some(value) = markup.strip_prefix("glyph:") {
            symbols.push(u16::from_str_radix(value, 16).map_err(|_| "invalid glyph markup")?);
        } else if let Some(value) = markup.strip_prefix("byte:") {
            symbols.push(u16::from_str_radix(value, 16).map_err(|_| "invalid byte markup")?);
        } else if let Some(value) = markup.strip_prefix("control:") {
            symbols.push(parse_number(value)?);
        } else {
            let (name, argument) = markup
                .split_once(':')
                .map_or((markup, None), |(name, value)| (name, Some(value)));
            let (opcode, takes_argument) =
                command_symbol(name).ok_or_else(|| format!("unknown message command {name}"))?;
            if !takes_argument && argument.is_some() {
                return Err(format!("message command {name} argument differs"));
            }
            symbols.push(opcode);
            if let Some(value) = argument {
                symbols.push(parse_number(value)?);
            }
        }
        while at < characters.len() && characters[at].0 <= end {
            at += 1;
        }
    }
    flush(&mut literal, &mut symbols)?;
    if symbols
        .iter()
        .any(|symbol| usize::from(*symbol) >= symbol_count)
    {
        return Err("message symbol is outside the edition alphabet".into());
    }
    Ok(symbols)
}

fn header_number(headers: &BTreeMap<String, String>, name: &str) -> Result<usize, String> {
    let value = headers
        .get(name)
        .ok_or_else(|| format!("PO header {name} is missing"))?;
    value.strip_prefix("0x").map_or_else(
        || {
            value
                .parse::<usize>()
                .map_err(|_| format!("PO header {name} is invalid"))
        },
        |hex| usize::from_str_radix(hex, 16).map_err(|_| format!("PO header {name} is invalid")),
    )
}

#[derive(Debug)]
pub struct SourceCatalog {
    pub target: String,
    pub symbol_count: usize,
    pub banks: Vec<Vec<Option<Vec<u16>>>>,
    pub names: Vec<(String, usize)>,
}

const UNNAMED: &str = "message";
/// The catalogue's end: one past its last message number. It names no row
/// and encodes nothing; code that bounds a walk over the messages uses it.
pub const END: &str = "MsgEnd";

pub fn message_name(name: &str) -> bool {
    name.strip_prefix("Msg").is_some_and(|rest| {
        rest.starts_with(|c: char| c.is_ascii_uppercase())
            && rest.chars().all(|c| c.is_ascii_alphanumeric())
    })
}

pub fn catalog_names(catalog: &Catalog) -> Result<Vec<(String, usize)>, String> {
    let mut names = Vec::new();
    for entry in &catalog.entries {
        let context = entry.context.as_deref().unwrap_or_default();
        if context == UNNAMED {
            continue;
        }
        if !message_name(context) {
            return Err(format!(
                "PO entry {} context {context:?} is neither {UNNAMED} nor a Msg name",
                entry.id
            ));
        }
        let number = entry
            .id
            .parse::<usize>()
            .map_err(|_| format!("PO message key {} is invalid", entry.id))?;
        names.push((context.to_owned(), number));
    }
    let end = catalog
        .entries
        .iter()
        .filter_map(|entry| entry.id.parse::<usize>().ok())
        .max()
        .map_or(0, |last| last + 1);
    names.push((END.to_owned(), end));
    let mut sorted: Vec<&str> = names.iter().map(|(name, _)| name.as_str()).collect();
    sorted.sort_unstable();
    if let Some(pair) = sorted.windows(2).find(|pair| pair[0] == pair[1]) {
        return Err(format!("{} names two messages", pair[0]));
    }
    Ok(names)
}

pub fn read_catalog(path: &Path) -> Result<Catalog, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    po::read(&text).map_err(|error| format!("{}: {error}", path.display()))
}

pub fn read_source(path: &Path) -> Result<SourceCatalog, String> {
    source_catalog(&read_catalog(path)?).map_err(|error| format!("{}: {error}", path.display()))
}

fn source_catalog(catalog: &Catalog) -> Result<SourceCatalog, String> {
    for name in catalog
        .headers
        .keys()
        .filter(|name| name.starts_with("X-Alchemy-"))
    {
        if !matches!(
            name.as_str(),
            "X-Alchemy-Format" | "X-Alchemy-Target" | "X-Alchemy-Symbol-Count"
        ) {
            return Err(format!(
                "PO header {name} is not an editable encoder option"
            ));
        }
    }
    if header_number(&catalog.headers, "X-Alchemy-Format")? != 1 {
        return Err("PO message format is unsupported".into());
    }
    let target = catalog
        .headers
        .get("X-Alchemy-Target")
        .ok_or("PO target header is missing")?;
    let spec = ARCHIVES
        .iter()
        .find(|spec| spec.target == target)
        .ok_or("PO target is unknown")?;
    let symbol_count = header_number(&catalog.headers, "X-Alchemy-Symbol-Count")?;
    let message_count = catalog.entries.len();
    if !(1..=65536).contains(&message_count) || !(1..=4096).contains(&symbol_count) {
        return Err("PO message count or alphabet is outside archive limits".into());
    }
    let names = catalog_names(catalog)?;
    let mut messages = vec![None; message_count];
    for entry in &catalog.entries {
        let key = entry
            .id
            .parse::<usize>()
            .map_err(|_| "PO message key is invalid")?;
        let slot = messages
            .get_mut(key)
            .ok_or("PO message key is outside archive")?;
        if slot.is_some() {
            return Err(format!("duplicate PO message key {key}"));
        }
        *slot = Some(if entry.flags.iter().any(|flag| flag == "alchemy-null") {
            if !entry.value.is_empty() {
                return Err(format!("null PO message {key} is not empty"));
            }
            None
        } else {
            Some(text_symbols(
                &entry.value,
                spec.characters().as_deref(),
                symbol_count,
            )?)
        });
    }
    if let Some(key) = messages.iter().position(Option::is_none) {
        return Err(format!("PO message key {key} is missing"));
    }
    let messages = messages.into_iter().map(Option::unwrap).collect::<Vec<_>>();
    let banks = messages
        .chunks(BANK_SIZE)
        .map(|bank| bank.to_vec())
        .collect();
    Ok(SourceCatalog {
        target: target.to_owned(),
        symbol_count,
        banks,
        names,
    })
}

pub(crate) fn archive(source: &SourceCatalog, label: &str) -> Result<Data, String> {
    let built = encode_huffman_archive(0, source.symbol_count, &source.banks)?;
    let mut data = Data::from_bytes(built.bytes);
    data.align = 4;
    let models = format!("{label}Models");
    for (name, offset) in [
        (models.clone(), 0),
        (format!("{label}Contexts"), built.context_directory),
        (format!("{label}Banks"), built.directory),
    ] {
        data.labels.push(Label {
            name,
            offset: offset as usize,
            global: true,
        });
    }
    for site in built.pointers {
        let word = u32::from_le_bytes(data.bytes[site..site + 4].try_into().unwrap());
        data.bytes[site..site + 4].fill(0);
        data.pointers.push((
            site,
            Pointer {
                symbol: models.clone(),
                addend: i64::from(word),
            },
        ));
    }
    Ok(data)
}

pub fn messages_include(
    source: &SourceCatalog,
    label: &str,
    origin: &str,
) -> Result<String, String> {
    use std::fmt::Write as _;
    let mut assembly = format!(
        "@ {}'s message archive and message numbers, built from {origin}.\n",
        source.target
    );
    assembly.push_str(&archive(source, label)?.source()?);
    for (name, number) in &source.names {
        writeln!(assembly, "\t.global {name}\n\t.set {name}, {number}").unwrap();
    }
    Ok(assembly)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::assets::po::Entry;

    fn entry(id: usize, context: &str) -> Entry {
        Entry {
            comments: Vec::new(),
            flags: Vec::new(),
            context: Some(context.to_owned()),
            id: id.to_string(),
            value: String::new(),
        }
    }

    #[test]
    fn the_catalogue_end_is_one_past_its_last_message() {
        let mut catalog = Catalog {
            entries: vec![entry(0, UNNAMED), entry(1, "MsgFirst"), entry(2, UNNAMED)],
            ..Catalog::default()
        };
        let names = catalog_names(&catalog).unwrap();
        assert_eq!(names, vec![("MsgFirst".to_owned(), 1), (END.to_owned(), 3)]);
        catalog.entries.push(entry(3, END));
        assert!(catalog_names(&catalog).unwrap_err().contains("names two messages"));
    }
}
