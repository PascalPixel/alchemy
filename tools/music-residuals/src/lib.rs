//! Native build and verification for `assets/audio/residuals_index.json`.
//!
//! The residual package is deliberately small: four empty sound headers, the
//! audited reserve stream, and the zero-filled tail alignment. Sequence byte
//! emission is shared with the native `music` crate.

use std::path::{Path, PathBuf};

use canonical_json::is_canonical_json_text;
use music::build_reserve_sequence;
use serde_json::{json, Value};

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x0080_0000;
pub const SOUND_TABLE_ADDRESS: u32 = 0x080f_c684;
pub const YOBI_ADDRESS: u32 = 0x0818_19b0;
pub const YOBI_END: u32 = 0x0818_19c2;
pub const ALIGNMENT_ADDRESS: u32 = 0x0818_4698;
pub const ALIGNMENT_END: u32 = 0x0818_5000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltMusicResidual {
    pub address: u32,
    pub data: Vec<u8>,
}

fn read_json(path: &Path, label: &str) -> Result<(String, Value)> {
    let text =
        std::fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let value = serde_json::from_str(&text).map_err(|error| format!("{label}: {error}"))?;
    if !is_canonical_json_text(&text, &value) {
        return Err(format!("{label} is not canonical JSON"));
    }
    Ok((text, value))
}

fn fixed_index() -> Value {
    json!({
        "format": 1,
        "kind": "golden-sun-music-residuals",
        "shared_empty_header": {
            "name": "sound_empty",
            "address": "0x080fd044",
            "track_count": 0,
            "block_count": 0,
            "priority": 0,
            "reverb": 0
        },
        "empty_headers": [
            {"name":"sound_019","address":"0x081652d8","track_count":0,"block_count":0,"priority":0,"reverb":178,"tone_bank":"0x080fba78"},
            {"name":"sound_095","address":"0x081810b8","track_count":0,"block_count":0,"priority":0,"reverb":178,"tone_bank":"0x080fba78"},
            {"name":"sound_288","address":"0x081841f8","track_count":0,"block_count":0,"priority":120,"reverb":0,"tone_bank":"0x080fc138"},
            {"name":"sound_298","address":"0x08184358","track_count":0,"block_count":0,"priority":120,"reverb":0,"tone_bank":"0x080fc138"}
        ],
        "orphan_stream": {
            "address":"0x081819b0", "size":18,
            "source":"sound_138_yobi.json", "between":["sound_138","sound_139"]
        },
        "tail_alignment": {
            "address":"0x08184698", "end":"0x08185000", "boundary":4096, "fill":0
        }
    })
}

fn fixed_yobi() -> Value {
    json!({
        "format": 1,
        "engine": "smsh-sequence",
        "base": "0x081819b0",
        "externals": {},
        "layout": [{
            "kind": "stream",
            "label": "yobi_track",
            "events": [
                ["volume",120], ["key_shift",0], ["tempo",30], ["voice",21],
                ["note",1,61,127], ["wait",1], ["note_running",1,66], ["wait",1],
                ["note",44,69], ["wait",48], ["fine"]
            ]
        }]
    })
}

fn validate_index(index_path: &Path) -> Result<PathBuf> {
    let (_, value) = read_json(index_path, "music residual index")?;
    if value != fixed_index() {
        return Err("music residual index differs from the audited catalog".into());
    }
    let prefix = index_path
        .file_name()
        .and_then(|name| name.to_str())
        .and_then(|name| name.strip_suffix("index.json"))
        .ok_or_else(|| "music residual index name is invalid".to_string())?;
    let source = index_path
        .parent()
        .unwrap_or_else(|| Path::new("."))
        .join(format!("{prefix}sound_138_yobi.json"));
    let (_, yobi) = read_json(&source, "reserve sound stream")?;
    if yobi != fixed_yobi() {
        return Err("reserve sound stream differs from the audited events".into());
    }
    Ok(source)
}

fn header(address: u32, priority: u8, reverb: u8, tone_bank: u32) -> BuiltMusicResidual {
    let mut data = vec![0, 0, priority, reverb];
    data.extend_from_slice(&tone_bank.to_le_bytes());
    BuiltMusicResidual { address, data }
}

pub fn build_music_residuals(index_path: &Path) -> Result<Vec<BuiltMusicResidual>> {
    validate_index(index_path)?;
    let (yobi, report) = build_reserve_sequence(YOBI_ADDRESS);
    if report.base != YOBI_ADDRESS
        || report.bytes != 18
        || report.streams != 1
        || report.tracks != 0
        || report.events != 11
        || yobi.len() != 18
    {
        return Err("reserve sound stream has an unexpected report".into());
    }
    let mut regions = vec![
        BuiltMusicResidual {
            address: 0x080f_d044,
            data: vec![0; 4],
        },
        header(0x0816_52d8, 0, 178, 0x080f_ba78),
        header(0x0818_10b8, 0, 178, 0x080f_ba78),
        BuiltMusicResidual {
            address: YOBI_ADDRESS,
            data: yobi,
        },
        header(0x0818_41f8, 120, 0, 0x080f_c138),
        header(0x0818_4358, 120, 0, 0x080f_c138),
        BuiltMusicResidual {
            address: ALIGNMENT_ADDRESS,
            data: vec![0; (ALIGNMENT_END - ALIGNMENT_ADDRESS) as usize],
        },
    ];
    regions.sort_by_key(|region| region.address);
    Ok(regions)
}

fn read_u16(rom: &[u8], offset: usize) -> Result<u16> {
    let bytes = rom
        .get(offset..offset + 2)
        .ok_or_else(|| "sound-table entry lies outside ROM".to_string())?;
    Ok(u16::from_le_bytes([bytes[0], bytes[1]]))
}

fn read_u32(rom: &[u8], offset: usize) -> Result<u32> {
    let bytes = rom
        .get(offset..offset + 4)
        .ok_or_else(|| "sound-table entry lies outside ROM".to_string())?;
    Ok(u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]))
}

fn verify_sound_table_links(rom: &[u8]) -> Result<()> {
    for (sound, header) in [
        (19u32, 0x0816_52d8),
        (95, 0x0818_10b8),
        (138, YOBI_ADDRESS - 12),
        (139, 0x0818_19d4),
        (288, 0x0818_41f8),
        (298, 0x0818_4358),
    ] {
        let offset = (SOUND_TABLE_ADDRESS - ROM_BASE + sound * 8) as usize;
        if read_u32(rom, offset)? != header {
            return Err(format!(
                "sound-table entry {sound} does not select its residual header"
            ));
        }
        if read_u16(rom, offset + 4)? != read_u16(rom, offset + 6)? {
            return Err(format!(
                "sound-table entry {sound} does not mirror its player selector"
            ));
        }
    }
    let sound138 = (YOBI_ADDRESS - 12 - ROM_BASE) as usize;
    let sound139 = (0x0818_19d4 - ROM_BASE) as usize;
    if rom.get(sound138) != Some(&1) || read_u32(rom, sound138 + 8)? != 0x0818_1988 {
        return Err("reserve sound stream does not follow sound 138".into());
    }
    if rom.get(sound139) != Some(&1) || read_u32(rom, sound139 + 8)? != YOBI_END {
        return Err("reserve sound stream does not precede sound 139".into());
    }
    Ok(())
}

fn verify_regions(rom: &[u8], regions: &[BuiltMusicResidual]) -> Result<usize> {
    let mut bytes = 0;
    for region in regions {
        let start = region
            .address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| "music residual address is below ROM".to_string())?
            as usize;
        let original = rom
            .get(start..start + region.data.len())
            .ok_or_else(|| "music residual lies outside ROM".to_string())?;
        if original != region.data.as_slice() {
            return Err(format!(
                "music residual at 0x{:x} differs from ROM",
                region.address
            ));
        }
        bytes += region.data.len();
    }
    Ok(bytes)
}

pub fn verify_music_residuals(rom_path: &Path, index_path: &Path) -> Result<String> {
    let rom =
        std::fs::read(rom_path).map_err(|error| format!("{}: {error}", rom_path.display()))?;
    if rom.len() != ROM_SIZE {
        return Err("music residual verifier requires the 8 MiB canonical ROM".into());
    }
    verify_sound_table_links(&rom)?;
    let bytes = verify_regions(&rom, &build_music_residuals(index_path)?)?;
    Ok(format!("identical=true regions=7 source_bytes={bytes}"))
}

pub fn self_test() -> Result<()> {
    let (bytes, report) = build_reserve_sequence(YOBI_ADDRESS);
    if bytes.len() != 18
        || report.events != 11
        || bytes
            != vec![
                0xbe, 120, 0xbc, 0, 0xbb, 30, 0xbd, 21, 0xd0, 61, 127, 0x81, 66, 0x81, 0xee, 69,
                0xa0, 0xb1,
            ]
    {
        return Err("reserve sound stream self-test failed".into());
    }
    if ALIGNMENT_END - ALIGNMENT_ADDRESS != 0x968 {
        return Err("music tail alignment self-test failed".into());
    }
    Ok(())
}

pub fn build_region(index_path: &Path, address: u32) -> Result<Vec<u8>> {
    build_music_residuals(index_path)?
        .into_iter()
        .find(|region| region.address == address)
        .map(|region| region.data)
        .ok_or_else(|| format!("music residual address 0x{address:x} is not a produced region"))
}
