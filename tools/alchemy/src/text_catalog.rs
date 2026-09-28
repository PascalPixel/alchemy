//! The twelve shipped message archives as ordinary GNU gettext PO catalogs.

use encoding_rs::WINDOWS_1252;
use psynergy::assets::huffman_archive::{encode_huffman_archive, HuffmanArchive};
use psynergy::assets::po::{self, Catalog};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use unicode_normalization::UnicodeNormalization;

const BANK_SIZE: usize = 256;

#[derive(Clone, Debug)]
pub(crate) struct ArchiveSpec {
    pub target: &'static str,
    pub language: &'static str,
    pub rom: &'static str,
    pub output: &'static str,
    pub rom_sha256: &'static str,
    pub characters: Option<&'static str>,
}

/// Edition identities; the alphabets map font slots,
/// retaining unused and duplicate glyphs for lossless PO conversion.
pub(crate) static ARCHIVES: [ArchiveSpec; 12] = [
    ArchiveSpec {
        target: "tbs-ja",
        language: "ja",
        rom: "roms/tbs-ja.gba",
        output: "games/THE BROKEN SEAL/TEXT/JA.PO",
        rom_sha256: "088bedae4bad8b67e87ff10035a898d3639f3182d486fe5a5d113bab223e0a26",
        characters: Some(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[¥]^_`abcdefghijklmnopqrstuvwxyz{|}~�������をぁぃぅぇぉゃゅょっ�あいうえおかきくけこさしすせそ�。｢｣、・ヲァィゥェォャュョッーアイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワン゙゚たちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわん��神殿名前中武器長剣発動呪使水火風地毒見宝石町行炎船海氷道具島男女力土大上玉山気目入口岩天空防母北戦手出下品同死木像以宮村東南森西灯台寺分先遺跡立人方時様主者陸説明士世光知伝金売客商屋子×年兄川官錬日民父冬古代夜雪春「」草原黄文○"),
    },
    ArchiveSpec {
        target: "tbs-en",
        language: "en",
        rom: "roms/tbs-en.gba",
        output: "games/THE BROKEN SEAL/TEXT/EN.PO",
        rom_sha256: "c14f1151897e8d73f25ffdd67e21eebb6dc57973ff2458872ee89fa9060aaca1",
        characters: None,
    },
    ArchiveSpec {
        target: "tbs-de",
        language: "de",
        rom: "roms/tbs-de.gba",
        output: "games/THE BROKEN SEAL/TEXT/DE.PO",
        rom_sha256: "d7a61803600a002bc80be8063a7d8d281bc77c2261cfb812cea552d3f95f3dd1",
        characters: None,
    },
    ArchiveSpec {
        target: "tbs-es",
        language: "es",
        rom: "roms/tbs-es.gba",
        output: "games/THE BROKEN SEAL/TEXT/ES.PO",
        rom_sha256: "c067f04d05a65677eca3b8e3609a6ef9b86898604ef252ccf54c2b41d49f2eb8",
        characters: None,
    },
    ArchiveSpec {
        target: "tbs-fr",
        language: "fr",
        rom: "roms/tbs-fr.gba",
        output: "games/THE BROKEN SEAL/TEXT/FR.PO",
        rom_sha256: "5eb59f508c25548fb0ef72911cc75a81867f16b0ef8fca2a22cb6d026a862cd8",
        characters: None,
    },
    ArchiveSpec {
        target: "tbs-it",
        language: "it",
        rom: "roms/tbs-it.gba",
        output: "games/THE BROKEN SEAL/TEXT/IT.PO",
        rom_sha256: "fc6ef60c1c271de7352be610eb4dca29ab5edea1e4b33f05a548a65f522a452d",
        characters: None,
    },
    ArchiveSpec {
        target: "tla-ja",
        language: "ja",
        rom: "roms/tla-ja.gba",
        output: "games/THE LOST AGE/TEXT/JA.PO",
        rom_sha256: "19dd48b74726f323cd829e226b60aa1b373c51fb2e840804efe2d2dc2891a890",
        characters: Some(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[¥]^_`abcdefghijklmnopqrstuvwxyz{|}~�������をぁぃぅぇぉゃゅょっ�あいうえおかきくけこさしすせそ�。｢｣、・ヲァィゥェォャュョッーアイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワン゙゚たちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわん��神殿後名前黄金太陽開封印失時代力手武器長剣発動呪使水火風地毒見宝石中炎船海氷道具灯台運光様目下山男女土大上玉気左右入口岩天空防母行北戦出品同以死木像人先島陸小村寺高原町遺跡東西南古屋頂通信世界錬文明者年日士兄「」生知方売客商子主王父一官白茶×説分今立川伝森○…足夜買門冬雪月民宮春多草床正店星城外絵心闘体"),
    },
    ArchiveSpec {
        target: "tla-en",
        language: "en",
        rom: "roms/tla-en.gba",
        output: "games/THE LOST AGE/TEXT/EN.PO",
        rom_sha256: "4199d82f845edf3e2e92f3783bca00190b5bc102d7c8aa339b951de280b1e6cc",
        characters: None,
    },
    ArchiveSpec {
        target: "tla-de",
        language: "de",
        rom: "roms/tla-de.gba",
        output: "games/THE LOST AGE/TEXT/DE.PO",
        rom_sha256: "993cfc34b6b28f6a9bfb135dc04023ee2b64693841ce90ab89536b97fbb4afed",
        characters: None,
    },
    ArchiveSpec {
        target: "tla-es",
        language: "es",
        rom: "roms/tla-es.gba",
        output: "games/THE LOST AGE/TEXT/ES.PO",
        rom_sha256: "c6bb68229971c36febe8bdf5081a4aa658c5c5417a2f4d3c7e0c3762c3ecb18a",
        characters: None,
    },
    ArchiveSpec {
        target: "tla-fr",
        language: "fr",
        rom: "roms/tla-fr.gba",
        output: "games/THE LOST AGE/TEXT/FR.PO",
        rom_sha256: "8f9a854618332d4a2886170a03ab0b10624d1202ea8562dabf0002807f7b56a8",
        characters: None,
    },
    ArchiveSpec {
        target: "tla-it",
        language: "it",
        rom: "roms/tla-it.gba",
        output: "games/THE LOST AGE/TEXT/IT.PO",
        rom_sha256: "7f3fbb2ee3e493784e63069899b5a53cf79742cb1a64560094c570b0ed3a05c2",
        characters: None,
    },
];

/// The approved reference identity is independent of local extraction output.
pub(crate) fn reference_sha256(_root: &Path, target: &str) -> Result<String, String> {
    ARCHIVES
        .iter()
        .find(|spec| spec.target == target)
        .map(|spec| spec.rom_sha256.to_owned())
        .ok_or_else(|| format!("no approved reference ROM for {target}"))
}
/// Refuse `rom` unless it is `target`'s approved reference ROM.
pub(crate) fn verify_reference(root: &Path, target: &str, rom: &[u8]) -> Result<(), String> {
    if crate::compiler::sha256::hex(rom) != reference_sha256(root, target)? {
        return Err(format!(
            "ROM differs from the approved {target} reference ROM"
        ));
    }
    Ok(())
}

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

pub(crate) fn symbols_text(symbols: &[u16], characters: Option<&str>) -> String {
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
/// build supplies the address the archive is encoded at.
#[derive(Debug)]
pub(crate) struct SourceCatalog {
    pub target: String,
    pub symbol_count: usize,
    pub banks: Vec<Vec<Option<Vec<u16>>>>,
}

pub(crate) fn read_source(path: &Path) -> Result<SourceCatalog, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let catalog = po::read(&text).map_err(|error| error.to_string())?;
    source_catalog(&catalog)
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
    let mut messages = vec![None; message_count];
    for entry in &catalog.entries {
        if entry.context.as_deref() != Some("message") {
            return Err("PO entry context must be message".into());
        }
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
                spec.characters.as_deref(),
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
    })
}

/// The archive encoded at `base`, the address its build places it at.
pub(crate) fn encode(source: &SourceCatalog, base: u32) -> Result<HuffmanArchive, String> {
    encode_huffman_archive(base, source.symbol_count, &source.banks)
        .map_err(|error| error.to_string())
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
        let base = ROM_BASE + 0x41;
        let archive = encode(&source, base).unwrap();
        assert_eq!(
            source.banks.iter().map(Vec::len).sum::<usize>(),
            catalog.entries.len()
        );
        // The archive placed at `base` in an image reads back every message.
        let mut rom = vec![0; (base - ROM_BASE) as usize];
        rom.extend_from_slice(&archive.bytes);
        let mut reader = MessageReader::new(
            &rom,
            ROM_BASE,
            archive.context_directory,
            archive.directory,
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
                    &symbols_text(&symbols, ARCHIVES[0].characters.as_deref()),
                    ARCHIVES[0].characters.as_deref(),
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
        for spec in ARCHIVES.iter().filter(|spec| spec.characters.is_some()) {
            let characters = spec.characters.as_deref();
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
            symbols_text(&[0x102], ARCHIVES[0].characters.as_deref()),
            "名"
        );
        assert_eq!(
            symbols_text(&[0x102], ARCHIVES[6].characters.as_deref()),
            "後"
        );
    }

    #[test]
    fn every_japanese_glyph_including_unused_slots_round_trips() {
        for (spec, count) in [(&ARCHIVES[0], 370), (&ARCHIVES[6], 408)] {
            let characters = spec.characters.as_deref();
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
            assert_eq!(spec.characters.is_some(), spec.language == "ja");
        }
    }

    #[test]
    fn layouts_cover_each_registered_target_once() {
        let mut ids = ARCHIVES.iter().map(|spec| spec.target).collect::<Vec<_>>();
        ids.sort_unstable();
        ids.dedup();
        assert_eq!(ARCHIVES.len(), ids.len());
        assert_eq!(ids.len(), crate::targets::TARGET_IDS.len());
        let mut identities = std::collections::HashSet::new();
        for id in crate::targets::TARGET_IDS {
            let target = crate::targets::target_for(id);
            let spec = ARCHIVES
                .iter()
                .find(|spec| spec.target == id.as_str())
                .unwrap();
            assert_eq!(spec.rom, target.rom);
            assert_eq!(
                spec.output,
                format!(
                    "{}/TEXT/{}.PO",
                    target.game_dir(),
                    spec.language.to_uppercase()
                )
            );
            assert_eq!(spec.rom_sha256.len(), 64);
            assert!(spec.rom_sha256.bytes().all(|byte| byte.is_ascii_hexdigit()));
            assert!(identities.insert(spec.rom_sha256));
        }
    }

    #[test]
    fn local_metadata_cannot_authorize_a_different_reference_rom() {
        let root = tempfile::tempdir().unwrap();
        let manifest = root.path().join("recon/tbs/text.json");
        fs::create_dir_all(manifest.parent().unwrap()).unwrap();
        let wrong_rom = b"a local ROM is not an approved reference";
        fs::write(
            manifest,
            serde_json::json!([{
                "target": "tbs-en",
                "rom_sha256": crate::compiler::sha256::hex(wrong_rom)
            }])
            .to_string(),
        )
        .unwrap();
        let identity = reference_sha256(root.path(), "tbs-en").unwrap();
        assert_ne!(identity, crate::compiler::sha256::hex(wrong_rom));
        assert_eq!(identity, ARCHIVES[1].rom_sha256);
        assert!(verify_reference(root.path(), "tbs-en", wrong_rom).is_err());
    }

    #[test]
    fn reference_identity_requires_an_exact_registered_target() {
        for target in ["", "tbs", "TBS-en", "tbs-en ", "tbs-us", "tla-us"] {
            assert!(
                reference_sha256(Path::new("."), target).is_err(),
                "{target}"
            );
            assert!(
                verify_reference(Path::new("."), target, b"").is_err(),
                "{target}"
            );
        }
    }
}
