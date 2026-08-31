//! Native build and verification for `games/gs1/sound/residuals.tsv`.
//!
//! The residual package is deliberately small: four empty sound headers, the
//! audited reserve stream, and the zero-filled tail alignment. Sequence byte
//! emission is shared with the native `music` crate.

pub mod cli;

use std::path::Path;

use music::build_reserve_sequence;

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x0080_0000;
pub const SOUND_TABLE_ADDRESS: u32 = 0x080f_c684;
pub const ORPHAN_STREAM_ADDRESS: u32 = 0x0818_19b0;
pub const ORPHAN_STREAM_END: u32 = 0x0818_19c2;
pub const ALIGNMENT_ADDRESS: u32 = 0x0818_4698;
pub const ALIGNMENT_END: u32 = 0x0818_5000;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct BuiltMusicResidual {
    pub address: u32,
    pub data: Vec<u8>,
}

const RESIDUALS_TSV: &str = "# Audited GS1 music bytes outside ordinary MIDI sources.\nkind\tname\taddress\tend\tpriority\treverb\ttone_bank\tfill\nempty_header\tsound_empty\t0x080fd044\t0x080fd048\t0\t0\nempty_header\tsound_019\t0x081652d8\t0x081652e0\t0\t178\t0x080fba78\nempty_header\tsound_095\t0x081810b8\t0x081810c0\t0\t178\t0x080fba78\nreserve_stream\torphan_stream_after_item_break\t0x081819b0\t0x081819c2\nempty_header\tsound_288\t0x081841f8\t0x08184200\t120\t0\t0x080fc138\nempty_header\tsound_298\t0x08184358\t0x08184360\t120\t0\t0x080fc138\nalignment\ttail_alignment\t0x08184698\t0x08185000\t\t\t\t0\n";

fn validate_index(index_path: &Path) -> Result<()> {
    let text = std::fs::read_to_string(index_path)
        .map_err(|error| format!("{}: {error}", index_path.display()))?;
    if text != RESIDUALS_TSV {
        return Err("music residual index differs from the audited catalog".into());
    }
    Ok(())
}

fn header(address: u32, priority: u8, reverb: u8, tone_bank: u32) -> BuiltMusicResidual {
    let mut data = vec![0, 0, priority, reverb];
    data.extend_from_slice(&tone_bank.to_le_bytes());
    BuiltMusicResidual { address, data }
}

pub fn build_music_residuals(index_path: &Path) -> Result<Vec<BuiltMusicResidual>> {
    validate_index(index_path)?;
    let (orphan_stream, report) = build_reserve_sequence(ORPHAN_STREAM_ADDRESS);
    if report.base != ORPHAN_STREAM_ADDRESS
        || report.bytes != 18
        || report.streams != 1
        || report.tracks != 0
        || report.events != 11
        || orphan_stream.len() != 18
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
            address: ORPHAN_STREAM_ADDRESS,
            data: orphan_stream,
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
        (138, ORPHAN_STREAM_ADDRESS - 12),
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
    let sound138 = (ORPHAN_STREAM_ADDRESS - 12 - ROM_BASE) as usize;
    let sound139 = (0x0818_19d4 - ROM_BASE) as usize;
    if rom.get(sound138) != Some(&1) || read_u32(rom, sound138 + 8)? != 0x0818_1988 {
        return Err("reserve sound stream does not follow sound 138".into());
    }
    if rom.get(sound139) != Some(&1) || read_u32(rom, sound139 + 8)? != ORPHAN_STREAM_END {
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
    let (bytes, report) = build_reserve_sequence(ORPHAN_STREAM_ADDRESS);
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
