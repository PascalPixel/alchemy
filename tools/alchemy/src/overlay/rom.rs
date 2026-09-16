use crate::overlay::assembly::ROM_BASE;
use crate::targets::DecompTarget;
use psynergy::assets::lz::{GeneralToken, PaletteGroup};
use std::path::Path;

/// The loader bound on one decoded resource.
const DECODED_LIMIT: u64 = 0x10_0000;
/// `ldr r4, [pc, #0]; bx r4`, the first word of every overlay veneer.
pub const VENEER: [u8; 4] = [0x00, 0x4c, 0x20, 0x47];

pub struct CanonicalRom(Vec<u8>, usize);
impl CanonicalRom {
    /// The production default target's ROM (`tbs-en`).
    pub fn load(root: &Path) -> Result<Self, String> {
        Self::load_target(
            root,
            crate::targets::target_for(crate::targets::DEFAULT_TARGET),
        )
    }
    /// Any registered target's ROM, located through its resource-directory self-pointer.
    pub fn load_target(root: &Path, target: DecompTarget) -> Result<Self, String> {
        let path = root.join(target.rom);
        let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        if bytes.len() as u64 != target.rom_size {
            return Err(format!(
                "{}: {} bytes, expected {} for {}",
                path.display(),
                bytes.len(),
                target.rom_size,
                target.id
            ));
        }
        resource_table(&bytes).map(|table| Self(bytes, table))
    }
    pub fn overlay(&self, overlay: &str) -> Result<Vec<u8>, String> {
        decode_overlay(&self.0, self.1, overlay)
    }
    /// One resource's compressed stream, decoded with its token trace.
    pub fn stream(&self, resource: usize) -> Result<Stream, String> {
        decode_stream(&self.0, self.1, resource)
    }
    /// The ROM bytes the directory points into.
    pub fn bytes(&self) -> &[u8] {
        &self.0
    }
    /// Every resource that decodes to a code overlay of the target's shape:
    /// an even-length image opening with exactly `entry_veneers` veneers into
    /// the overlay's own load window. The range is read off the decoded
    /// images, never assumed.
    pub fn overlay_resources(&self, entry_veneers: usize) -> Vec<usize> {
        (0..directory_length(&self.0, self.1))
            .filter(|resource| {
                decode_stream(&self.0, self.1, *resource).is_ok_and(|stream| {
                    stream.decoded.len().is_multiple_of(2)
                        && leading_entry_veneers(&stream.decoded) == entry_veneers
                })
            })
            .collect()
    }
}
/// The general-LZ token plan or the palette-LZ group plan of one stream.
pub enum Trace {
    General(Vec<GeneralToken>),
    Palette(Vec<PaletteGroup>),
}
pub struct Stream {
    pub resource: usize,
    /// ROM offset of the stream, and of the next resource or, for the
    /// directory's last resource, of the end of its exact encoding.
    pub start: usize,
    pub end: usize,
    /// Whether a following resource bounds the stream, so the bytes between
    /// the encoding and that resource belong to this stream's plan.
    pub bounded: bool,
    pub decoded: Vec<u8>,
    pub trace: Trace,
}
impl Stream {
    /// The exact encoding of the traced plan, tag byte included, without
    /// any trailing alignment bytes the ROM places before the next resource.
    pub fn encoded(&self) -> Result<Vec<u8>, String> {
        match &self.trace {
            Trace::General(tokens) => psynergy::assets::lz::encode_general(&self.decoded, tokens),
            Trace::Palette(groups) => psynergy::assets::lz::encode_palette(&self.decoded, groups)
                .map(|mut body| {
                    body.insert(0, 1);
                    body
                }),
        }
        .map_err(|error| error.0)
    }
    /// The stream's leading tag byte: 0 general-LZ, 1 palette-LZ.
    pub fn tag(&self) -> u8 {
        match self.trace {
            Trace::General(_) => 0,
            Trace::Palette(_) => 1,
        }
    }
}
/// Count of leading `ldr r4, [pc, #0]; bx r4; .4byte 0x02xxxxxx` entry veneers.
pub fn leading_entry_veneers(image: &[u8]) -> usize {
    image
        .chunks_exact(8)
        .take_while(|pair| pair[..4] == VENEER && pair[7] == 0x02)
        .count()
}
pub fn resource_table(rom: &[u8]) -> Result<usize, String> {
    (0..rom.len().saturating_sub(8))
        .step_by(4)
        .find(|offset| {
            u32::from_le_bytes(rom[*offset..*offset + 4].try_into().unwrap()) as i64 == ROM_BASE
                && u32::from_le_bytes(rom[*offset + 4..*offset + 8].try_into().unwrap()) as usize
                    == ROM_BASE as usize + *offset
        })
        .ok_or("resource directory self-pointer was not found".into())
}
/// The directory ends at its first pointer outside the ROM.
fn directory_length(rom: &[u8], table: usize) -> usize {
    (0..)
        .find(|resource| resource_pointer(rom, table, *resource).is_err())
        .unwrap_or(0)
}
fn resource_pointer(rom: &[u8], table: usize, resource: usize) -> Result<usize, String> {
    let at = table
        .checked_add(resource * 4)
        .ok_or("resource directory offset overflow")?;
    let address = u32::from_le_bytes(
        rom.get(at..at + 4)
            .ok_or("resource directory extends past ROM")?
            .try_into()
            .unwrap(),
    ) as usize;
    address
        .checked_sub(ROM_BASE as usize)
        .filter(|offset| *offset < rom.len())
        .ok_or_else(|| format!("resource {resource:03x} points outside ROM"))
}
pub fn resource_id(overlay: &str) -> Result<usize, String> {
    overlay
        .strip_prefix("resource_")
        .filter(|value| !value.is_empty() && value.bytes().all(|b| b.is_ascii_hexdigit()))
        .and_then(|value| usize::from_str_radix(value, 16).ok())
        .ok_or_else(|| format!("invalid overlay name {overlay:?}"))
}
fn decode_stream(rom: &[u8], table: usize, resource: usize) -> Result<Stream, String> {
    let start = resource_pointer(rom, table, resource)?;
    let next = resource_pointer(rom, table, resource + 1)
        .ok()
        .filter(|end| *end > start);
    let limit = next.unwrap_or(rom.len());
    let (decoded, trace) = match rom[start] {
        0 => psynergy::assets::lz::decode_general_trace(rom, start, limit, DECODED_LIMIT)
            .map(|(decoded, _, tokens)| (decoded, Trace::General(tokens))),
        1 => psynergy::assets::lz::decode_palette_trace(rom, start + 1, limit, DECODED_LIMIT)
            .map(|(decoded, _, groups)| (decoded, Trace::Palette(groups))),
        tag => return Err(format!("resource {resource:03x} has tag {tag}")),
    }
    .map_err(|error| error.0)?;
    let mut stream = Stream {
        resource,
        start,
        end: limit,
        bounded: next.is_some(),
        decoded,
        trace,
    };
    if !stream.bounded {
        // The general decoder reads whole words; the stream is its encoding.
        stream.end = start + stream.encoded()?.len();
    }
    Ok(stream)
}
pub fn decode_overlay(rom: &[u8], table: usize, overlay: &str) -> Result<Vec<u8>, String> {
    decode_stream(rom, table, resource_id(overlay)?).map(|stream| stream.decoded)
}
pub fn canonical_overlay(root: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    CanonicalRom::load(root)?.overlay(overlay)
}
/// One decoded overlay resource from any registered target's ROM.
pub fn canonical_overlay_for(
    root: &Path,
    target: DecompTarget,
    overlay: &str,
) -> Result<Vec<u8>, String> {
    CanonicalRom::load_target(root, target)?.overlay(overlay)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn overlay_bytes_come_from_the_rom_directory() {
        const RESOURCE: usize = 0x36f;
        let table = 0x100usize;
        let start = 0x1000usize;
        let stream = [1, 0x30, b'A', b'B', 0x01, 0x02, 0, 0];
        let mut rom = vec![0u8; start + stream.len() + 16];
        rom[table..table + 4].copy_from_slice(&(ROM_BASE as u32).to_le_bytes());
        rom[table + 4..table + 8]
            .copy_from_slice(&((ROM_BASE as usize + table) as u32).to_le_bytes());
        let pointer = table + RESOURCE * 4;
        rom[pointer..pointer + 4]
            .copy_from_slice(&((ROM_BASE as usize + start) as u32).to_le_bytes());
        rom[pointer + 4..pointer + 8]
            .copy_from_slice(&((ROM_BASE as usize + start + stream.len()) as u32).to_le_bytes());
        rom[start..start + stream.len()].copy_from_slice(&stream);
        assert_eq!(
            decode_overlay(&rom, table, "resource_36f").unwrap(),
            b"ABAB"
        );
    }
}
