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

/// A halfword with its placement-dependent bits removed: calls and pool words
/// vanish, and branch offsets are dropped so inserted code does not hide an
/// otherwise equal instruction.
fn key(half: u16, compared: bool) -> u16 {
    if !compared {
        return 0;
    }
    match half >> 11 {
        0x1a | 0x1b | 0x1c => half & 0xff00,
        _ => half,
    }
}

/// Four consecutive keys packed into one value.
fn grams(keys: &[u16]) -> Vec<u64> {
    let mut out: Vec<u64> = keys
        .windows(4)
        .filter(|w| w.iter().all(|k| *k != 0))
        .map(|w| w.iter().fold(0u64, |a, k| a << 16 | u64::from(*k)))
        .collect();
    out.sort_unstable();
    out
}

/// Shared entries of two sorted lists, counting repeats.
fn shared(a: &[u64], b: &[u64]) -> usize {
    let (mut i, mut j, mut n) = (0, 0, 0);
    while i < a.len() && j < b.len() {
        match a[i].cmp(&b[j]) {
            std::cmp::Ordering::Less => i += 1,
            std::cmp::Ordering::Greater => j += 1,
            std::cmp::Ordering::Equal => {
                n += 1;
                i += 1;
                j += 1;
            }
        }
    }
    n
}

/// The closest place for a function whose shape differs in `haystack`, as in
/// another game: starts that repeat its first `prologue` compared halfwords
/// (its register saves) are ranked by the share of its four-instruction
/// sequences they contain over the function's length. Best first.
pub fn nearest(haystack: &[u8], shape: &Shape, prologue: usize) -> Vec<(usize, f64)> {
    let n = shape.halves.len();
    let keys: Vec<u16> = (0..n)
        .map(|i| key(shape.halves[i], shape.compared[i]))
        .collect();
    let head: Vec<usize> = (0..n)
        .filter(|i| shape.compared[*i])
        .take(prologue)
        .collect();
    let mine = grams(&keys);
    if head.is_empty() || mine.is_empty() || haystack.len() < n * 2 {
        return Vec::new();
    }
    let half = |at: usize| u16::from_le_bytes([haystack[at], haystack[at + 1]]);
    let mut out = Vec::new();
    let mut start = 0;
    while start + n * 2 <= haystack.len() {
        if head.iter().all(|i| half(start + i * 2) == shape.halves[*i]) {
            let theirs: Vec<u16> = (0..n).map(|i| key(half(start + i * 2), true)).collect();
            let score = shared(&mine, &grams(&theirs)) as f64 / mine.len() as f64;
            out.push((start, score));
        }
        start += 2;
    }
    out.sort_by(|a, b| b.1.total_cmp(&a.1));
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
    fn nearest_ranks_a_changed_twin_above_a_same_prologue_stranger() {
        let halves = |hs: &[u16]| hs.iter().flat_map(|h| h.to_le_bytes()).collect::<Vec<u8>>();
        // push {r5, lr}; adds r5, r0, #0; movs r0, #1; adds r1, r5, #0;
        // lsls r2, r1, #2; adds r3, r2, r5; strh r3, [r5, #2]; movs r0, #0;
        // pop {r5}; pop {r1}; bx r1
        let body = [
            0xb520, 0x1c05, 0x2001, 0x1c29, 0x008a, 0x1953, 0x806b, 0x2000, 0xbc20, 0xbc02, 0x4708,
        ];
        let s = shape(&halves(&body), 0x0800_1000);
        let mut twin = body.to_vec();
        twin.insert(3, 0x3001); // adds r0, #1
        let stranger = [
            0xb520, 0x2207, 0x4352, 0x1e52, 0xd1fd, 0x6812, 0x6013, 0x2301, 0x3304, 0x3305, 0x4708,
        ];
        let mut rom = halves(&stranger);
        rom.extend(halves(&[0; 4]));
        let twin_at = rom.len();
        rom.extend(halves(&twin));
        rom.extend(halves(&[0; 8]));
        let ranked = nearest(&rom, &s, 1);
        assert_eq!(ranked[0].0, twin_at, "{ranked:?}");
        assert!(ranked[0].1 >= 0.5 && ranked[1].1 < 0.2, "{ranked:?}");
        assert!(find(&rom, &s, 0x0800_1000).is_empty());
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
