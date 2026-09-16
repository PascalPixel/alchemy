use super::*;
use crate::compiler::routing::CompilerTarget;
use psynergy::assets::huffman_archive::MessageReader;

pub(super) fn text(symbols: &[u16]) -> String {
    const KANA: &str = "｡｢｣､･ｦｧｨｩｪｫｬｭｮｯｰｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝﾞﾟ";
    symbols
        .iter()
        .map(|s| match *s {
            32..=126 => char::from_u32(*s as u32).unwrap().to_string(),
            0xa1..=0xdf => KANA.chars().nth((*s - 0xa1) as usize).unwrap().to_string(),
            _ => format!("<{s:03x}>"),
        })
        .collect()
}
fn romaji(kana: &str) -> Result<String, String> {
    const BASE: &str = "ｱｲｳｴｵｶｷｸｹｺｻｼｽｾｿﾀﾁﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓﾔﾕﾖﾗﾘﾙﾚﾛﾜﾝ";
    const LATIN: [&str; 46] = [
        "A", "I", "U", "E", "O", "KA", "KI", "KU", "KE", "KO", "SA", "SHI", "SU", "SE", "SO", "TA",
        "CHI", "TSU", "TE", "TO", "NA", "NI", "NU", "NE", "NO", "HA", "HI", "FU", "HE", "HO", "MA",
        "MI", "MU", "ME", "MO", "YA", "YU", "YO", "RA", "RI", "RU", "RE", "RO", "WA", "N", "",
    ];
    let chars = kana.chars().collect::<Vec<_>>();
    let mut output = String::new();
    let mut pos = 0;
    while pos < chars.len() {
        let ch = chars[pos];
        if ch == 'ｰ' {
            let vowel = output.chars().last().ok_or("long vowel lacks base")?;
            output.push(vowel);
            pos += 1;
            continue;
        }
        if let Some(vowel) = match ch {
            'ｧ' => Some("A"),
            'ｨ' => Some("I"),
            'ｩ' => Some("U"),
            'ｪ' => Some("E"),
            'ｫ' => Some("O"),
            _ => None,
        } {
            output.push_str(vowel);
            pos += 1;
            continue;
        }
        let index = BASE
            .chars()
            .position(|c| c == ch)
            .ok_or_else(|| format!("romaji requires an evidenced kana mapping for {ch}"))?;
        let mut syllable = LATIN[index].to_string();
        if chars.get(pos + 1) == Some(&'ﾞ') {
            syllable = match syllable.as_str() {
                "KA" => "GA",
                "KI" => "GI",
                "KU" => "GU",
                "KE" => "GE",
                "KO" => "GO",
                "SA" => "ZA",
                "SHI" => "JI",
                "SU" => "ZU",
                "SE" => "ZE",
                "SO" => "ZO",
                "TA" => "DA",
                "CHI" => "JI",
                "TSU" => "ZU",
                "TE" => "DE",
                "TO" => "DO",
                "HA" => "BA",
                "HI" => "BI",
                "FU" => "BU",
                "HE" => "BE",
                "HO" => "BO",
                _ => return Err("unsupported voiced kana".into()),
            }
            .into();
            pos += 1;
        } else if chars.get(pos + 1) == Some(&'ﾟ') {
            syllable = match syllable.as_str() {
                "HA" => "PA",
                "HI" => "PI",
                "FU" => "PU",
                "HE" => "PE",
                "HO" => "PO",
                _ => return Err("unsupported semi-voiced kana".into()),
            }
            .into();
            pos += 1;
        }
        if chars.get(pos + 1) == Some(&'ｪ') && syllable == "JI" {
            syllable = "JE".into();
            pos += 1;
        }
        if let Some(glide) = match chars.get(pos + 1) {
            Some('ｬ') => Some('A'),
            Some('ｭ') => Some('U'),
            Some('ｮ') => Some('O'),
            _ => None,
        } {
            syllable = match syllable.as_str() {
                "SHI" | "CHI" | "JI" => format!("{}{glide}", &syllable[..syllable.len() - 1]),
                "TE" | "DE" => format!("{}Y{glide}", &syllable[..1]),
                other if other.ends_with('I') => {
                    format!("{}Y{glide}", &other[..other.len() - 1])
                }
                _ => return Err("small ya, yu or yo lacks an i-row base".into()),
            };
            pos += 1;
        }
        if chars.get(pos + 1) == Some(&'ｨ') && matches!(syllable.as_str(), "TE" | "DE") {
            syllable = format!("{}I", &syllable[..1]);
            pos += 1;
        }
        output.push_str(&syllable);
        pos += 1;
    }
    Ok(output)
}

/// One game's evidence for naming its character descriptors: the English
/// message archive (inverse-checked against the tracked source), the Japanese
/// archive the names are read from, and the dense party-name messages.
struct Edition {
    english: DecompTargetId,
    japanese: DecompTargetId,
    archive: &'static str,
    /// The recovered descriptor catalog the English ROM table must equal.
    catalog_source: Option<&'static str>,
    /// Recovered character bank documents, by frame directory.
    character_dir: Option<&'static str>,
    /// The Japanese archive's context directory and bank directory, read from
    /// the literal pool of the Japanese message reader.
    japanese_contexts: u32,
    japanese_banks: u32,
    /// First party-name message and how many actors own a name there.
    party_names: usize,
    party_actors: usize,
    /// Party actors whose romaji the audit derives.
    romaji_actors: usize,
}

fn edition(target: &DecompTarget) -> Edition {
    match target.compiler {
        CompilerTarget::Tbs => Edition {
            english: DecompTargetId::TbsEn,
            japanese: DecompTargetId::TbsJa,
            archive: "games/THE BROKEN SEAL/TEXT/MESSAGE_ARCHIVE.JSON",
            catalog_source: Some("games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER/CATALOG.JSON"),
            character_dir: Some("games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER"),
            japanese_contexts: 0x0803_bb68,
            japanese_banks: 0x0806_c040,
            party_names: 102,
            party_actors: 7,
            romaji_actors: 4,
        },
        CompilerTarget::Tla => Edition {
            english: DecompTargetId::TlaEn,
            japanese: DecompTargetId::TlaJa,
            archive: "games/THE LOST AGE/TEXT/MESSAGE_ARCHIVE.JSON",
            catalog_source: None,
            character_dir: None,
            japanese_contexts: 0x0806_4c3c,
            japanese_banks: 0x0809_cf40,
            party_names: 131,
            party_actors: 8,
            romaji_actors: 8,
        },
    }
}

/// A context directory opens with the tree and leaf-offset addresses; the
/// offset table (two bytes per symbol) ends where the directory begins.
fn japanese_reader<'a>(
    rom: &'a [u8],
    edition: &Edition,
) -> Result<(MessageReader<'a>, usize), String> {
    let start = edition.japanese_contexts as usize - ROM_BASE;
    let offsets = rom
        .get(start + 4..start + 8)
        .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()))
        .ok_or("Japanese context directory outside ROM")?;
    let symbols = edition
        .japanese_contexts
        .checked_sub(offsets)
        .filter(|span| span % 2 == 0 && *span != 0)
        .ok_or("Japanese symbol offset table does not precede its directory")?
        as usize
        / 2;
    let reader = MessageReader::new(
        rom,
        ROM_BASE as u32,
        edition.japanese_contexts,
        edition.japanese_banks,
        symbols,
    )
    .map_err(|e| e.to_string())?;
    Ok((reader, symbols))
}

fn party_names(rom: &[u8], edition: &Edition) -> Result<Vec<Value>, String> {
    let (mut reader, _) = japanese_reader(rom, edition)?;
    let rom_path = target_for(edition.japanese).rom;
    (0..edition.romaji_actors).map(|actor| {
        let id = edition.party_names + actor;
        let message=reader.message(id).map_err(|e|e.to_string())?;
        let symbols=message.symbols.ok_or("party name is empty")?;
        let decoded=text(&symbols);
        let romaji=romaji(&decoded)?;
        Ok(json!({"actor":actor,"romaji":romaji,"japanese":decoded,"message_id":id,"message_offset":message.offset,"message_encoded_size":message.bytes,"symbols":symbols,"rom":rom_path,"rom_sha256":sha256::hex(rom)}))
    }).collect()
}

/// Identities reviewed for The Lost Age descriptors: descriptor, the Japanese
/// message that spells the name, the English message that spells it, and the
/// binding. A name message proves only the spelling; every binding below comes
/// from reviewing the decoded ROM sheet (SHEETS/NNN.PNG), because no runtime
/// actor placement naming these descriptors has been read yet.
const SHEET_REVIEW: &str = "sheet review; runtime actor binding not established";
const LOST_AGE_IDENTITIES: &[(usize, usize, usize, &str)] = &[
    (4, 135, 135, SHEET_REVIEW),
    (5, 136, 136, SHEET_REVIEW),
    (6, 137, 137, SHEET_REVIEW),
    (22, 1413, 1413, SHEET_REVIEW),
    (23, 1416, 1416, SHEET_REVIEW),
    (28, 4953, 4967, SHEET_REVIEW),
    (31, 4952, 4966, SHEET_REVIEW),
];

pub fn audit(root: &Path, arguments: &[String]) -> Result<(), String> {
    let (output, target) = match arguments {
        [output] => (output.clone(), crate::targets::decomp_target(None)?),
        [output, flag, target] | [flag, target, output] if flag == "--target" => {
            (output.clone(), crate::targets::decomp_target(Some(target))?)
        }
        _ => return Err("usage: --audit-characters OUTPUT [--target TARGET]".into()),
    };
    let output = Path::new(&output);
    let output = if output.is_absolute() {
        output.to_path_buf()
    } else {
        root.join(output)
    };
    if !output.starts_with(root.join("out")) {
        return Err("identity audit contains private ROM messages; use out".into());
    }
    let edition = edition(&target);
    let english_target = target_for(edition.english);
    let japanese_target = target_for(edition.japanese);
    let english = fs::read(root.join(english_target.rom)).map_err(|e| e.to_string())?;
    let japanese = fs::read(root.join(japanese_target.rom)).map_err(|e| e.to_string())?;
    let archive = json(&root.join(edition.archive))?;
    let mut reader = MessageReader::new(
        &english,
        ROM_BASE as u32,
        (address(&archive["message_address"])? - 8) as u32,
        address(&archive["directory_address"])? as u32,
        address(&archive["symbol_count"])?,
    )
    .map_err(|e| e.to_string())?;
    let commands = archive["commands"]
        .as_object()
        .ok_or("message commands missing")?;
    let mut count = 0;
    let mut english_names = BTreeMap::new();
    for (bank, messages) in archive["banks"]
        .as_array()
        .ok_or("message banks missing")?
        .iter()
        .enumerate()
    {
        for (row, value) in messages
            .as_array()
            .ok_or("message bank missing")?
            .iter()
            .enumerate()
        {
            let expected = message_symbols(
                value,
                commands,
                archive["glyphs"].as_object(),
                address(&archive["symbol_count"])?,
            )?;
            if reader
                .message(bank * 256 + row)
                .map_err(|e| e.to_string())?
                .symbols
                != expected
            {
                return Err("English inverse message decoding differs from source archive".into());
            }
            english_names.insert(bank * 256 + row, value.clone());
            count += 1;
        }
    }
    let (mut reader, symbol_count) = japanese_reader(&japanese, &edition)?;
    let mut messages = vec![];
    for index in 0..count {
        let message = reader
            .message(index)
            .map_err(|e| format!("Japanese message {index}: {e}"))?;
        messages.push(json!({"id":index,"offset":message.offset,"encoded_size":message.bytes,"text":message.symbols.as_ref().map(|s|text(s)),"symbols":message.symbols}));
    }
    let english_catalog = character::catalog(&english_target)?;
    let japanese_catalog = character::catalog(&japanese_target)?;
    let english_descriptors = english_catalog.descriptors(&english)?;
    let japanese_descriptors = japanese_catalog.descriptors(&japanese)?;
    if japanese_descriptors.len() != english_descriptors.len() {
        return Err("Japanese and English descriptor counts differ".into());
    }
    let recovered = edition
        .catalog_source
        .map(|source| json(&root.join(source)))
        .transpose()?;
    if let Some(recovered) = &recovered {
        let records = recovered["segments"][0]["records"]
            .as_array()
            .ok_or("descriptor table missing")?;
        if records.len() != english_descriptors.len()
            || address(&recovered["segments"][0]["address"])? != english_catalog.table
        {
            return Err("recovered descriptor catalog differs from the registered table".into());
        }
        for (record, descriptor) in records.iter().zip(&english_descriptors) {
            if address(&record["frame_directory"])? != descriptor.frame_directory
                || address(&record["frame_codec"])? != usize::from(descriptor.frame_codec)
            {
                return Err(format!(
                    "recovered descriptor {} differs from ROM",
                    descriptor.id
                ));
            }
        }
    }
    let mut banks = BTreeMap::new();
    if let Some(directory) = edition.character_dir {
        for entry in walkdir::WalkDir::new(root.join(directory)) {
            let entry = entry.map_err(|e| e.to_string())?;
            if !entry.file_type().is_file()
                || !entry
                    .path()
                    .extension()
                    .and_then(|e| e.to_str())
                    .is_some_and(|e| e.eq_ignore_ascii_case("json"))
            {
                continue;
            }
            let doc = json(entry.path())?;
            if doc["directory"]["address"].is_string() {
                banks.insert(
                    address(&doc["directory"]["address"])?,
                    relative(root, entry.path()),
                );
            }
            if let Some(owned) = doc["banks"].as_object() {
                for (key, bank) in owned {
                    let directory = if bank["directory"].is_string() {
                        &bank["directory"]
                    } else {
                        &bank["directory"]["address"]
                    };
                    banks.insert(
                        address(directory)?,
                        format!("{}#/banks/{key}", relative(root, entry.path())),
                    );
                }
            }
        }
    }
    let sheets = if recovered.is_none() {
        let (_, _, sheets) = character::sheets(&english_target, &english)?;
        for (id, sheet) in &sheets {
            write(&output, &format!("SHEETS/{id:03}.PNG"), &sheet.png)?;
        }
        Some(sheets)
    } else {
        None
    };
    let mut rows = vec![];
    for (english_row, native) in english_descriptors.iter().zip(&japanese_descriptors) {
        let id = english_row.id;
        let fields = |d: &character::Descriptor| {
            (
                d.width,
                d.height,
                d.scale,
                d.draw_kind,
                d.animation_count,
                d.adjustment,
                d.anchor,
                d.frame_codec,
            )
        };
        let identical = fields(english_row) == fields(native);
        let mut row = json!({"descriptor":id,"english_frame_directory":format!("0x{:08x}",english_row.frame_directory),"japanese_record_address":native.address,"japanese_frame_directory":native.frame_directory,"japanese_animation_address":native.animation_table,"non_pointer_fields_match":identical,"bank":banks.get(&english_row.frame_directory),"identity":null,"identity_status":"runtime name association not established"});
        if recovered.is_some() {
            row["source_pointer"] = json!(format!("/segments/0/records/{id}"));
        } else {
            row["width"] = json!(english_row.width);
            row["height"] = json!(english_row.height);
            row["draw_kind"] = json!(english_row.draw_kind);
            row["animation_count"] = json!(english_row.animation_count);
            row["frame_codec"] = json!(english_row.frame_codec);
            row["english_animation_table"] =
                json!(format!("0x{:08x}", english_row.animation_table));
            match sheets.as_ref().and_then(|sheets| sheets.get(&id)) {
                Some(sheet) => {
                    row["frames"] = json!(sheet.frames);
                    row["unique_frames"] = json!(sheet.unique_frames);
                    row["codec"] = json!(sheet.codec);
                    row["sheet"] = json!(format!("SHEETS/{id:03}.PNG"));
                }
                None => row["frame_source"] = json!("runtime-loaded directory"),
            }
        }
        rows.push(row);
    }
    let mut identities = vec![];
    for (descriptor, message, english_message, binding) in LOST_AGE_IDENTITIES
        .iter()
        .filter(|_| target.compiler == CompilerTarget::Tla)
    {
        let japanese_text = messages
            .get(*message)
            .and_then(|m| m["text"].as_str())
            .ok_or("identity message missing")?;
        // A speaker-name message ends in its wait command, symbol 2.
        let name = japanese_text.strip_suffix("<002>").unwrap_or(japanese_text);
        let romaji = romaji(name)?;
        identities.push(json!({"descriptor":descriptor,"romaji":romaji,"japanese_message":message,"japanese_text":name,"english_message":english_message,"english_text":english_names.get(english_message),"binding":binding,"sheet":format!("SHEETS/{descriptor:03}.PNG")}));
        rows[*descriptor]["identity"] = json!(romaji);
        rows[*descriptor]["identity_status"] = json!(binding);
    }
    let names_end = edition.party_names + edition.party_actors;
    document(
        &output,
        "MESSAGES.json",
        &json!({"rom":japanese_target.rom,"sha256":sha256::hex(&japanese),"context_directory":format!("0x{:08x}",edition.japanese_contexts),"bank_directory":format!("0x{:08x}",edition.japanese_banks),"symbol_count":symbol_count,"messages":messages}),
    )?;
    let mut characters = json!({"rom_sha256":sha256::hex(&japanese),"descriptor_table":japanese_catalog.table,"descriptors":rows,"party_names":party_names(&japanese, &edition)?,"names":messages[edition.party_names..names_end]});
    if target.compiler == CompilerTarget::Tbs {
        characters["party_pose_source"] =
            json!("games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/TABLES.JSON#/tables/0x080c2a0a");
    } else {
        characters["english_descriptor_table"] = json!(english_catalog.table);
        characters["sprite_palette"] = json!(format!("0x{:08x}", english_catalog.palette));
        characters["identities"] = json!(identities);
    }
    document(&output, "CHARACTERS.json", &characters)?;
    println!(
        "inverse_english_messages={count} japanese_messages={} descriptors={} japanese_table={:#x}",
        messages.len(),
        rows.len(),
        japanese_catalog.table
    );
    for message in &messages[edition.party_names..names_end] {
        println!(
            "{}: {}",
            message["id"],
            message["text"].as_str().unwrap_or("")
        );
    }
    for identity in &identities {
        println!(
            "descriptor {:03}: {} ({})",
            identity["descriptor"],
            identity["romaji"].as_str().unwrap_or(""),
            identity["binding"].as_str().unwrap_or("")
        );
    }
    Ok(())
}
