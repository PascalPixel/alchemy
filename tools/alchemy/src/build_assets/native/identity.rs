use super::*;
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
pub(super) fn party_names(rom: &[u8]) -> Result<Vec<Value>, String> {
    let mut reader = MessageReader::new(rom, ROM_BASE as u32, 0x0803bb68, 0x0806c040, 371)
        .map_err(|e| e.to_string())?;
    (0..4).map(|actor| {
        let message=reader.message(102+actor).map_err(|e|e.to_string())?;
        let symbols=message.symbols.ok_or("party name is empty")?;
        let decoded=text(&symbols);
        let romaji=romaji(&decoded)?;
        Ok(json!({"actor":actor,"romaji":romaji,"japanese":decoded,"message_id":102+actor,"message_offset":message.offset,"message_encoded_size":message.bytes,"symbols":symbols,"rom":"roms/tbs-ja.gba","rom_sha256":sha256::hex(rom)}))
    }).collect()
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
        }
        if chars.get(pos + 1) == Some(&'ｪ') && syllable == "JI" {
            syllable = "JE".into();
            pos += 1;
        }
        output.push_str(&syllable);
        pos += 1;
    }
    Ok(output)
}
pub fn audit(root: &Path, output: &Path) -> Result<(), String> {
    let output = if output.is_absolute() {
        output.to_path_buf()
    } else {
        root.join(output)
    };
    if !output.starts_with(root.join("out")) {
        return Err("identity audit contains private ROM messages; use out".into());
    }
    let english = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let japanese = fs::read(root.join("roms/tbs-ja.gba")).map_err(|e| e.to_string())?;
    let archive = json(&root.join("games/THE BROKEN SEAL/TEXT/MESSAGE_ARCHIVE.JSON"))?;
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
            let expected = message_symbols(value, commands, address(&archive["symbol_count"])?)?;
            if reader
                .message(bank * 256 + row)
                .map_err(|e| e.to_string())?
                .symbols
                != expected
            {
                return Err("English inverse message decoding differs from source archive".into());
            }
            count += 1;
        }
    }
    let mut reader = MessageReader::new(&japanese, ROM_BASE as u32, 0x0803bb68, 0x0806c040, 371)
        .map_err(|e| e.to_string())?;
    let mut messages = vec![];
    for index in 0..count {
        let message = reader
            .message(index)
            .map_err(|e| format!("Japanese message {index}: {e}"))?;
        messages.push(json!({"id":index,"offset":message.offset,"encoded_size":message.bytes,"text":message.symbols.as_ref().map(|s|text(s)),"symbols":message.symbols}));
    }
    let source = json(&root.join("games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER/CATALOG.JSON"))?;
    let descriptors = source["segments"][0]["records"]
        .as_array()
        .ok_or("descriptor table missing")?;
    let en_base = address(&source["segments"][0]["address"])? - ROM_BASE;
    let first = &english[en_base..en_base + 12];
    let candidates = japanese
        .windows(12)
        .enumerate()
        .filter(|(offset, bytes)| {
            bytes == &first
                && *offset + 80 <= japanese.len()
                && (0..4).all(|row| {
                    japanese[*offset + row * 20..*offset + row * 20 + 12]
                        == english[en_base + row * 20..en_base + row * 20 + 12]
                })
        })
        .map(|(offset, _)| offset)
        .collect::<Vec<_>>();
    if candidates.len() != 1 {
        return Err(format!(
            "Japanese descriptor table ambiguous: {} matches",
            candidates.len()
        ));
    }
    let ja_base = candidates[0];
    let mut banks = BTreeMap::new();
    for entry in walkdir::WalkDir::new(root.join("games/THE BROKEN SEAL/SRC/GRAPHICS/CHARACTER")) {
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
    let mut rows = vec![];
    for (id, descriptor) in descriptors.iter().enumerate() {
        let start = ja_base + id * 20;
        let native = japanese
            .get(start..start + 20)
            .ok_or("Japanese descriptor outside ROM")?;
        let identical = english[en_base + id * 20..en_base + id * 20 + 12] == native[..12];
        let frame_directory = u32::from_le_bytes(native[12..16].try_into().unwrap());
        let animation = u32::from_le_bytes(native[16..20].try_into().unwrap());
        rows.push(json!({"descriptor":id,"source_pointer":format!("/segments/0/records/{id}"),"english_frame_directory":descriptor["frame_directory"],"japanese_record_address":ROM_BASE+start,"japanese_frame_directory":frame_directory,"japanese_animation_address":animation,"non_pointer_fields_match":identical,"bank":banks.get(&address(&descriptor["frame_directory"])?),"identity":null,"identity_status":"runtime name association not established"}));
    }
    document(
        &output,
        "MESSAGES.json",
        &json!({"rom":"roms/tbs-ja.gba","sha256":sha256::hex(&japanese),"context_directory":"0x0803bb68","bank_directory":"0x0806c040","symbol_count":371,"messages":messages}),
    )?;
    document(
        &output,
        "CHARACTERS.json",
        &json!({"rom_sha256":sha256::hex(&japanese),"descriptor_table":ROM_BASE+ja_base,"descriptors":rows,"party_pose_source":"games/THE BROKEN SEAL/SRC/GRAPHICS/COMMON/TABLES.JSON#/tables/0x080c2a0a","party_names":party_names(&japanese)?,"names":messages[102..109]}),
    )?;
    println!(
        "inverse_english_messages={count} japanese_messages={} descriptors={} japanese_table={:#x}",
        messages.len(),
        descriptors.len(),
        ROM_BASE + ja_base
    );
    for message in &messages[102..109] {
        println!(
            "{}: {}",
            message["id"],
            message["text"].as_str().unwrap_or("")
        );
    }
    Ok(())
}
