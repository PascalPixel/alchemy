//! Native build and verification for `games/gs1/sound/residuals.tsv`.
//!
//! The residual package is deliberately small: four empty sound headers, the
//! audited reserve stream, and the zero-filled tail alignment. Sequence byte
//! emission is shared with the native `music` crate.

use std::path::Path;

use music::build_reserve_sequence;

pub type Error = String;
pub type Result<T> = std::result::Result<T, Error>;

pub const ORPHAN_STREAM_ADDRESS: u32 = 0x0818_19b0;
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
