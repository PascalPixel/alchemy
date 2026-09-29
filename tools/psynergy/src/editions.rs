//! Edition comparison: find one function of a linked build in other editions'
//! ROMs by its instruction shape and report which literal-pool words differ.
//! A word that changes between editions is a relocated or link-time value; a
//! word that is the same everywhere is probably a plain constant.
//!
//! ROMs are read at analysis time only; nothing here is stored or fed back.

use crate::assets::lz;
use crate::decode::{self, Kind};

const ROM_BASE: u32 = 0x0800_0000;
/// Where decoded resources (overlays) are addressed.
pub const RESOURCE_BASE: u32 = 0x0200_0000;
const DECODED_LIMIT: u64 = 0x10_0000;

/// A function's comparable shape: its halfwords, which of them are compared,
/// and the offsets of the literal-pool words its loads read.
#[derive(Clone, Debug)]
pub struct Shape {
    pub halves: Vec<u16>,
    pub compared: Vec<bool>,
    pub pool: Vec<u32>,
}

/// Masks calls (whose offsets depend on placement) and literal-pool words.
pub fn shape(code: &[u8], address: u32) -> Shape {
    let halves: Vec<u16> = code
        .as_chunks::<2>()
        .0
        .iter()
        .map(|h| u16::from_le_bytes(*h))
        .collect();
    let mut compared = vec![true; halves.len()];
    let mut pool = Vec::new();
    let end = address + halves.len() as u32 * 2;
    for ins in crate::similar::sweep(code, address, address, end) {
        let at = ((ins.addr - address) / 2) as usize;
        match ins.kind {
            Kind::Bl { .. } => {
                for c in compared.iter_mut().skip(at).take(2) {
                    *c = false;
                }
            }
            Kind::LdrPool { .. } => {
                if let Some(half) = decode::half_at(code, at * 2) {
                    let word = (ins.addr & !3) + 4 + u32::from(half & 0xff) * 4;
                    if word + 4 <= end {
                        let offset = word - address;
                        if !pool.contains(&offset) {
                            pool.push(offset);
                        }
                        let h = (offset / 2) as usize;
                        compared[h] = false;
                        compared[h + 1] = false;
                    }
                }
            }
            _ => {}
        }
    }
    pool.sort_unstable();
    Shape {
        halves,
        compared,
        pool,
    }
}

/// Offsets in `haystack` (halfword aligned, keeping the function's word
/// alignment so its pool stays aligned) where every compared halfword agrees.
pub fn find(haystack: &[u8], shape: &Shape, address: u32) -> Vec<usize> {
    let n = shape.halves.len();
    if n == 0 || haystack.len() < n * 2 {
        return Vec::new();
    }
    let Some(anchor) = shape.compared.iter().position(|c| *c) else {
        return Vec::new();
    };
    let first = shape.halves[anchor];
    let phase = (address & 2) as usize;
    let mut out = Vec::new();
    let mut start = phase;
    while start + n * 2 <= haystack.len() {
        let half =
            |i: usize| u16::from_le_bytes([haystack[start + i * 2], haystack[start + i * 2 + 1]]);
        if half(anchor) == first && (0..n).all(|i| !shape.compared[i] || half(i) == shape.halves[i])
        {
            out.push(start);
        }
        start += 4;
    }
    out
}

pub fn word(bytes: &[u8], at: usize) -> Option<u32> {
    bytes
        .get(at..at + 4)
        .map(|w| u32::from_le_bytes(w.try_into().unwrap()))
}

/// The resource directory: the word table that begins with the ROM's base
/// and a pointer to itself.
pub fn resource_directory(rom: &[u8]) -> Option<usize> {
    (0..rom.len().saturating_sub(8)).step_by(4).find(|&at| {
        word(rom, at) == Some(ROM_BASE) && word(rom, at + 4) == Some(ROM_BASE + at as u32)
    })
}

/// Every resource that decodes as a tagged LZ stream, with its id.
pub fn resources(rom: &[u8]) -> Vec<(usize, Vec<u8>)> {
    let Some(table) = resource_directory(rom) else {
        return Vec::new();
    };
    let pointer = |id: usize| {
        word(rom, table + id * 4)
            .and_then(|p| p.checked_sub(ROM_BASE))
            .map(|p| p as usize)
            .filter(|p| *p < rom.len())
    };
    let mut out = Vec::new();
    let mut id = 0;
    while let Some(start) = pointer(id) {
        let limit = pointer(id + 1).filter(|e| *e > start).unwrap_or(rom.len());
        let decoded = match rom[start] {
            0 => lz::decode_general(rom, start, limit, DECODED_LIMIT).ok(),
            1 => lz::decode_palette(rom, start + 1, limit, DECODED_LIMIT).ok(),
            _ => None,
        };
        if let Some((bytes, _)) = decoded {
            if bytes.len() >= 16 {
                out.push((id, bytes));
            }
        }
        id += 1;
    }
    out
}

/// Where one edition holds the function.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Found {
    /// `None` for the cartridge image, else the resource id.
    pub resource: Option<usize>,
    pub address: u32,
    pub pool: Vec<u32>,
}

/// Every place in `rom` (the cartridge image and each decoded resource)
/// where the shape occurs.
pub fn locate(rom: &[u8], shape: &Shape, address: u32) -> Vec<Found> {
    let read = |bytes: &[u8], at: usize| -> Vec<u32> {
        shape
            .pool
            .iter()
            .map(|o| word(bytes, at + *o as usize).unwrap_or(0))
            .collect()
    };
    let mut out: Vec<Found> = find(rom, shape, address)
        .into_iter()
        .map(|at| Found {
            resource: None,
            address: ROM_BASE + at as u32,
            pool: read(rom, at),
        })
        .collect();
    if address < ROM_BASE {
        for (id, bytes) in resources(rom) {
            for at in find(&bytes, shape, address) {
                out.push(Found {
                    resource: Some(id),
                    address: RESOURCE_BASE + at as u32,
                    pool: read(&bytes, at),
                });
            }
        }
    }
    out
}

/// How a pool word behaves across editions.
pub fn verdict(values: &[u32], addresses: &[u32]) -> &'static str {
    if values.windows(2).all(|w| w[0] == w[1]) {
        return "same";
    }
    let delta = |i: usize| values[i].wrapping_sub(addresses[i]);
    if (1..values.len()).all(|i| delta(i) == delta(0)) {
        return "moves-with-function";
    }
    "differs"
}

#[cfg(test)]
mod tests {
    use super::*;

    /// push {lr}; ldr r0, [pc, #8]; bl ...; pop {pc}; nop; pool word.
    fn function(pool: u32, call: [u8; 4]) -> Vec<u8> {
        let mut code = vec![0x00, 0xb5, 0x02, 0x48];
        code.extend(call);
        code.extend([0x00, 0xbd, 0xc0, 0x46]);
        code.extend(pool.to_le_bytes());
        code
    }

    #[test]
    fn shape_finds_the_function_elsewhere_and_reads_its_pool() {
        let here = function(0x54, [0x00, 0xf0, 0x10, 0xf8]);
        let s = shape(&here, 0x0200_1000);
        assert_eq!(s.pool, vec![12], "{s:?}");
        assert_eq!(
            s.compared,
            vec![true, true, false, false, true, true, false, false],
            "{s:?}"
        );
        let mut elsewhere = vec![0u8; 0x20];
        elsewhere.extend(function(0x4c, [0x7f, 0xf0, 0x02, 0xf9]));
        elsewhere.extend([0u8; 8]);
        let found = find(&elsewhere, &s, 0x0200_1000);
        assert_eq!(found, vec![0x20]);
        assert_eq!(word(&elsewhere, 0x20 + 12), Some(0x4c));
        assert!(find(&elsewhere[..0x2c], &s, 0x0200_1000).is_empty());
    }

    #[test]
    fn verdicts_separate_constants_relocations_and_ids() {
        assert_eq!(verdict(&[1, 1, 1], &[0, 4, 8]), "same");
        assert_eq!(
            verdict(&[0x100, 0x104, 0x108], &[0, 4, 8]),
            "moves-with-function"
        );
        assert_eq!(verdict(&[0x54, 0x55, 0x54], &[0, 0, 0]), "differs");
    }
}
