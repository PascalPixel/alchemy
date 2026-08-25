use crate::ROM_BASE;
use std::path::Path;

pub struct CanonicalRom(Vec<u8>, usize);
impl CanonicalRom {
    pub fn load(root: &Path) -> Result<Self, String> {
        let path = root.join("roms/gs1-en.gba");
        let bytes = std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        resource_table(&bytes).map(|table| Self(bytes, table))
    }
    pub fn overlay(&self, overlay: &str) -> Result<Vec<u8>, String> {
        decode_overlay(&self.0, self.1, overlay)
    }
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
pub fn decode_overlay(rom: &[u8], table: usize, overlay: &str) -> Result<Vec<u8>, String> {
    let resource = overlay
        .strip_prefix("resource_")
        .and_then(|value| usize::from_str_radix(value, 16).ok())
        .ok_or_else(|| format!("invalid overlay name {overlay:?}"))?;
    let start = resource_pointer(rom, table, resource)?;
    let end = resource_pointer(rom, table, resource + 1)
        .ok()
        .filter(|end| *end > start)
        .unwrap_or(rom.len());
    let (bytes, _) = match rom[start] {
        0 => extract_resource::decode_general(rom, start, end, 0x10_0000),
        1 => extract_resource::decode_palette(rom, start + 1, end, 0x10_0000),
        tag => return Err(format!("resource {resource:03x} has tag {tag}")),
    }
    .map_err(|error| error.0)?;
    Ok(bytes)
}
pub fn canonical_overlay(root: &Path, overlay: &str) -> Result<Vec<u8>, String> {
    CanonicalRom::load(root)?.overlay(overlay)
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
