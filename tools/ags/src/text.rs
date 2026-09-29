//! The twelve shipped message archives as ordinary GNU gettext PO catalogs,
//! as pret's preproc turns its text into data: each catalog is encoded into a
//! context-modelled Huffman archive whose internal addresses are words naming
//! the archive's own label, so the linker places it.

use super::asm::{Data, Label, Pointer};
use encoding_rs::WINDOWS_1252;
use psynergy::assets::huffman_archive::encode_huffman_archive;
use psynergy::assets::po::{self, Catalog};
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
    /// The edition's alphabet from its font's character map: one character
    /// per code from 0x20, U+FFFD for a slot that draws no character.
    pub fn characters(&self) -> Option<String> {
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

/// Each edition's catalog and alphabet; the alphabets map font slots,
/// retaining unused and duplicate glyphs for lossless PO conversion.
pub static ARCHIVES: [ArchiveSpec; 12] = [
    ArchiveSpec {
        target: "tbs-ja",
        language: "ja",
        output: "games/THE BROKEN SEAL/TEXT/JA.PO",
        charmap: Some(include_str!(
            "../../../games/COMMON/SRC/GRAPHICS/FONT/CHARMAP_JA.TSV"
        )),
    },
    ArchiveSpec {
        target: "tbs-en",
        language: "en",
        output: "games/THE BROKEN SEAL/TEXT/EN.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-de",
        language: "de",
        output: "games/THE BROKEN SEAL/TEXT/DE.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-es",
        language: "es",
        output: "games/THE BROKEN SEAL/TEXT/ES.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-fr",
        language: "fr",
        output: "games/THE BROKEN SEAL/TEXT/FR.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tbs-it",
        language: "it",
        output: "games/THE BROKEN SEAL/TEXT/IT.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-ja",
        language: "ja",
        output: "games/THE LOST AGE/TEXT/JA.PO",
        charmap: Some(include_str!(
            "../../../games/COMMON/SRC/GRAPHICS/FONT/CHARMAP_TLA_JA.TSV"
        )),
    },
    ArchiveSpec {
        target: "tla-en",
        language: "en",
        output: "games/THE LOST AGE/TEXT/EN.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-de",
        language: "de",
        output: "games/THE LOST AGE/TEXT/DE.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-es",
        language: "es",
        output: "games/THE LOST AGE/TEXT/ES.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-fr",
        language: "fr",
        output: "games/THE LOST AGE/TEXT/FR.PO",
        charmap: None,
    },
    ArchiveSpec {
        target: "tla-it",
        language: "it",
        output: "games/THE LOST AGE/TEXT/IT.PO",
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

fn literal_escape(text: &str) -> String {
    text.replace('{', "{{").replace('}', "}}")
}

pub fn symbols_text(symbols: &[u16], characters: Option<&str>) -> String {
    let mut output = String::new();
    let mut bytes = Vec::<u8>::new();
    let characters = characters.map(|text| text.chars().collect::<Vec<_>>());
    let flush = |output: &mut String, bytes: &mut Vec<u8>| {
        if bytes.is_empty() {
            return;
        }
        let (text, _, errors) = WINDOWS_1252.decode(bytes);
        if errors {
            for byte in bytes.iter() {
                output.push_str(&format!("{{byte:{byte:02x}}}"));
            }
        } else {
            output.push_str(&literal_escape(&text));
        }
        bytes.clear();
    };
    let mut index = 0;
    while index < symbols.len() {
        let symbol = symbols[index];
        if (1..32).contains(&symbol) {
            flush(&mut output, &mut bytes);
            let (name, argument) = command(symbol);
            if name == "control" {
                output.push_str(&format!("{{control:{symbol}}}"));
            } else if argument && index + 1 < symbols.len() {
                index += 1;
                output.push_str(&format!("{{{name}:{}}}", symbols[index]));
            } else {
                output.push_str(&format!("{{{name}}}"));
            }
        } else if let Some(characters) = &characters {
            let character = symbol
                .checked_sub(32)
                .and_then(|index| characters.get(index as usize));
            if let Some(&character) = character.filter(|&&character| {
                character != '\u{fffd}'
                    && characters.iter().position(|&value| value == character)
                        == Some(symbol as usize - 32)
            }) {
                output.push_str(&literal_escape(&character.to_string()));
            } else if symbol < 256 {
                output.push_str(&format!("{{byte:{symbol:02x}}}"));
            } else {
                output.push_str(&format!("{{glyph:{symbol:03x}}}"));
            }
        } else if let Ok(byte) = u8::try_from(symbol) {
            bytes.push(byte);
        } else {
            flush(&mut output, &mut bytes);
            output.push_str(&format!("{{glyph:{symbol:03x}}}"));
        }
        index += 1;
    }
    flush(&mut output, &mut bytes);
    if characters.is_some() {
        output.nfc().collect()
    } else {
        output
    }
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

/// A PO catalog's messages and encoder options. It records no placement: the
/// linker places the archive and resolves the addresses it holds.
#[derive(Debug)]
pub struct SourceCatalog {
    pub target: String,
    pub symbol_count: usize,
    pub banks: Vec<Vec<Option<Vec<u16>>>>,
    /// Each message the code names, `Msg<Name>`, with its number: the
    /// message's position in the catalog, which differs between editions.
    pub names: Vec<(String, usize)>,
}

/// The context of every message the code does not name.
const UNNAMED: &str = "message";

/// Whether `name` is a message name: `Msg`, a capital and plain letters or
/// digits, so `Msg_Show` style functions are never mistaken for one.
pub fn message_name(name: &str) -> bool {
    name.strip_prefix("Msg").is_some_and(|rest| {
        rest.starts_with(|c: char| c.is_ascii_uppercase())
            && rest.chars().all(|c| c.is_ascii_alphanumeric())
    })
}

/// Each named entry of a PO catalog: its `msgctxt` name and number.
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

/// The catalog's message archive, word aligned: its context models (labelled
/// `<label>Models`), offset table, context directory (`<label>Contexts`, which
/// the symbol decoder reads), message banks and bank directory
/// (`<label>Banks`, which the message lookup reads). Every address the
/// archive holds is a word naming `<label>Models`, which the linker resolves.
pub fn archive(source: &SourceCatalog, label: &str) -> Result<Data, String> {
    // Laid out from a word-aligned start, its padding holds wherever the
    // linker places it on a word boundary.
    let built = encode_huffman_archive(0, source.symbol_count, &source.banks)
        .map_err(|error| error.to_string())?;
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

/// A catalog's message archive and its message numbers as assembly: the
/// archive under `label`, then each named message as an absolute symbol,
/// the file the build includes and po2ags writes.
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
    use psynergy::assets::huffman_archive::MessageReader;
    use psynergy::assets::po::Entry;

    const ROM_BASE: u32 = 0x0800_0000;

    fn small_catalog() -> Catalog {
        let mut catalog = Catalog::default();
        for (name, value) in [
            ("X-Alchemy-Format", "1"),
            ("X-Alchemy-Target", "tbs-en"),
            ("X-Alchemy-Symbol-Count", "256"),
        ] {
            catalog.headers.insert(name.into(), value.into());
        }
        catalog.entries = ["Hi{line_break}Sun{end_now}", "Sun{end_now}", ""]
            .into_iter()
            .enumerate()
            .map(|(index, value)| Entry {
                comments: Vec::new(),
                flags: if index == 2 {
                    vec!["alchemy-null".into()]
                } else {
                    Vec::new()
                },
                context: Some("message".into()),
                id: index.to_string(),
                value: value.into(),
            })
            .collect();
        catalog
    }

    #[test]
    fn source_messages_derive_the_complete_archive_layout() {
        let catalog = small_catalog();
        let source = source_catalog(&catalog).unwrap();
        let archive = archive(&source, "Message").unwrap();
        assert_eq!(
            source.banks.iter().map(Vec::len).sum::<usize>(),
            catalog.entries.len()
        );
        assert_eq!(archive.align, 4);
        let text = archive.source().unwrap();
        assert!(text.starts_with("\t.balign 4\n\t.global MessageModels\nMessageModels:\n"));
        assert!(
            text.contains("\t.global MessageContexts\nMessageContexts:\n\t.4byte MessageModels\n")
        );
        // Linked on any word boundary, the archive reads back every message.
        let base = ROM_BASE + 0x44;
        let label = |name: &str| {
            let offset = archive
                .labels
                .iter()
                .find(|label| label.name == name)
                .unwrap()
                .offset;
            base + offset as u32
        };
        let mut rom = vec![0; (base - ROM_BASE) as usize];
        rom.extend_from_slice(&archive.bytes_at(base).unwrap());
        let mut reader = MessageReader::new(
            &rom,
            ROM_BASE,
            label("MessageContexts"),
            label("MessageBanks"),
            source.symbol_count,
        )
        .unwrap();
        assert_eq!(reader.message(0).unwrap().symbols, source.banks[0][0]);
        assert_eq!(reader.message(1).unwrap().symbols, source.banks[0][1]);
        assert_eq!(reader.message(2).unwrap().symbols, None);
    }

    #[test]
    fn source_catalog_refuses_bookkeeping_and_incomplete_message_keys() {
        let mut catalog = small_catalog();
        for bookkeeping in ["X-Alchemy-Archive-Size", "X-Alchemy-Archive-Address"] {
            catalog
                .headers
                .insert(bookkeeping.into(), "0x08000040".into());
            assert!(source_catalog(&catalog)
                .unwrap_err()
                .contains("encoder option"));
            catalog.headers.remove(bookkeeping);
        }
        catalog.entries[2].id = "3".into();
        assert!(source_catalog(&catalog).is_err());
        catalog.entries[2].id = "0".into();
        assert!(source_catalog(&catalog).is_err());
        catalog.entries[2].id = "2".into();
        catalog.entries[2].value = "not null".into();
        assert!(source_catalog(&catalog).is_err());
    }

    #[test]
    fn named_messages_take_their_number_from_the_catalog() {
        let mut catalog = small_catalog();
        catalog.entries[1].context = Some("MsgSunRises".into());
        let source = source_catalog(&catalog).unwrap();
        assert_eq!(source.names, [("MsgSunRises".to_owned(), 1)]);
        // A name is a Msg identifier, and names one message only.
        for context in ["Msg_Show", "sunrise", "MsgSun Rises", "Msg"] {
            catalog.entries[1].context = Some(context.into());
            assert!(source_catalog(&catalog).is_err(), "{context}");
        }
        catalog.entries[0].context = Some("MsgSunRises".into());
        catalog.entries[1].context = Some("MsgSunRises".into());
        assert!(source_catalog(&catalog)
            .unwrap_err()
            .contains("names two messages"));
        assert!(message_name("MsgHpRecover2") && !message_name("Msg_ShowAndWait"));
    }

    #[test]
    fn message_markup_round_trips_controls_braces_and_western_text() {
        let symbols = vec![72, 233, 3, 8, 5, 123, 125, 30];
        let text = symbols_text(&symbols, None);
        assert_eq!(text_symbols(&text, None, 256).unwrap(), symbols);
    }

    #[test]
    fn trailing_control_and_japanese_glyphs_round_trip() {
        for symbols in [
            vec![65, 19],
            vec![0xa1, 0xdf, 0x100, 0x169],
            vec![123, 125, 0x80],
        ] {
            assert_eq!(
                text_symbols(
                    &symbols_text(&symbols, ARCHIVES[0].characters().as_deref()),
                    ARCHIVES[0].characters().as_deref(),
                    512
                )
                .unwrap(),
                symbols
            );
        }
    }

    #[test]
    fn japanese_uses_the_rom_alphabet_and_composes_voiced_kana() {
        let symbols = [
            0x28, 0xc3, 0xde, 0xb0, 0xc0, 0x96, 0xde, 0x9a, 0xfc, 0xfa, 0xe3, 0x92, 0xef, 0x9d,
            0x29,
        ];
        for spec in ARCHIVES.iter().filter(|spec| spec.charmap.is_some()) {
            let alphabet = spec.characters();
            let characters = alphabet.as_deref();
            let text = symbols_text(&symbols, characters);
            assert_eq!(text, "(データがこわれています)");
            assert_eq!(text_symbols(&text, characters, 512).unwrap(), symbols);
            assert_eq!(
                text_symbols(&text.nfd().collect::<String>(), characters, 512).unwrap(),
                symbols
            );
            assert_eq!(symbols_text(&[0x100, 0x101], characters), "神殿");
            assert_eq!(
                symbols_text(&[8, 0xde, 0xca, 0xdf], characters),
                "{text_color:222}パ"
            );
            assert!(text_symbols("😀", characters, 512).is_err());
        }
        assert_eq!(
            symbols_text(&[0x102], ARCHIVES[0].characters().as_deref()),
            "名"
        );
        assert_eq!(
            symbols_text(&[0x102], ARCHIVES[6].characters().as_deref()),
            "後"
        );
    }

    #[test]
    fn every_japanese_glyph_including_unused_slots_round_trips() {
        for (spec, count) in [(&ARCHIVES[0], 370), (&ARCHIVES[6], 408)] {
            let alphabet = spec.characters();
            let characters = alphabet.as_deref();
            assert_eq!(characters.unwrap().chars().count(), count - 32);
            for symbol in 32..count as u16 {
                let text = symbols_text(&[symbol], characters);
                assert_eq!(
                    text_symbols(&text, characters, count).unwrap(),
                    [symbol],
                    "{} {symbol:x}",
                    spec.target
                );
            }
        }
        for spec in ARCHIVES.iter() {
            assert_eq!(spec.charmap.is_some(), spec.language == "ja");
        }
    }
}
