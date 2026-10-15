//! packer: the bytes after a resource stream that Camelot's packer left in
//! its reused buffer. Each stream was written from the start of one buffer
//! and rounded up to four bytes, so the 1-3 bytes after a stream are what
//! the most recent earlier resource long enough to reach them left at those
//! offsets (W3). This finds each such residue still on a scaffold in a built
//! edition and names the resource that left it, or says it is unexplained.

use crate::ld_map::{self, Map, Placed};
use std::path::Path;

const USAGE: &str = "usage: alchemy packer EDITION [--rows]
For each scaffold piece of EDITION's built image (make EDITION) that places
the 1-3 bytes between a resource stream's end and the next resource, prints
its address, size, whether it is the whole piece or its head, its bytes, the
stream and offset it follows, the verdict and the object before it. The
verdict is `left` with the earlier resource that wrote those buffer bytes,
`fill` for bytes all 00 or all ff that no earlier resource explains (zero
and 0xff fill, as ☀️ has), or `unexplained`. The buffer model takes the
resources in directory order, each one's extent up to the next resource in
the ROM. --rows prints instead, for each explained piece, the object before
it, the piece's section, whole or head, its size and the source row with
its PACKER tag.";

/// One resource of the directory: where it starts and where the next
/// resource in the ROM starts, so its stream rounded up to four bytes.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Stream {
    pub name: String,
    pub start: u32,
    pub end: u32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Verdict {
    /// The bytes equal `provider`'s at the same offset: the most recent
    /// earlier resource that reached that far in the buffer.
    Left { provider: usize },
    /// Bytes all 0x00 or all 0xff that no earlier resource left there.
    Fill(u8),
    Unexplained,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Finding {
    /// The stream the bytes follow, an index into the directory order.
    pub stream: usize,
    pub offset: u32,
    pub verdict: Verdict,
}

/// The directory's resources in its order, each placed once, with their
/// extents up to the next resource in the ROM.
pub fn streams(entries: &[u32], name: &dyn Fn(u32) -> String, rom_end: u32) -> Vec<Stream> {
    let mut seen = std::collections::HashSet::new();
    let ordered: Vec<u32> = entries
        .iter()
        .copied()
        .filter(|&a| (ld_map::ROM..rom_end).contains(&a) && seen.insert(a))
        .collect();
    let mut by_address = ordered.clone();
    by_address.sort_unstable();
    ordered
        .iter()
        .map(|&start| {
            let next = by_address.partition_point(|&a| a <= start);
            let end = by_address.get(next).copied().unwrap_or(rom_end);
            Stream { name: name(start), start, end }
        })
        .collect()
}

/// How many of a scaffold piece's `size` bytes at `address` lie between a
/// stream's end and the next resource: 1-3, the whole piece or its head
/// when it runs on into that resource.
pub fn residue_size(streams: &[Stream], address: u32, size: u32) -> Option<u32> {
    let end = streams.iter().filter(|s| s.start < address && address < s.end).map(|s| s.end).min()?;
    let residue = end - address;
    ((1..=3).contains(&residue) && residue <= size).then_some(residue)
}

/// Who left `bytes` at `address`, the end of a stream: `streams` in
/// directory order (the packer's), `read` the ROM bytes at an address.
pub fn explain(streams: &[Stream], address: u32, bytes: &[u8], read: &dyn Fn(u32, usize) -> Vec<u8>) -> Option<Finding> {
    let stream = streams
        .iter()
        .enumerate()
        .filter(|(_, s)| s.start < address && address < s.end)
        .max_by_key(|(_, s)| s.start)
        .map(|(index, _)| index)?;
    // Residue fills a stream up to the next resource; a gap inside a
    // resource (a code bank's alignment) is not the packer's.
    if address + bytes.len() as u32 != streams[stream].end {
        return None;
    }
    let offset = address - streams[stream].start;
    let provider = streams[..stream].iter().rposition(|s| s.end - s.start > offset);
    let verdict = match provider {
        Some(p) if read(streams[p].start + offset, bytes.len()) == bytes => Verdict::Left { provider: p },
        _ if [0x00, 0xff].iter().any(|&f| bytes.iter().all(|&b| b == f)) => Verdict::Fill(bytes[0]),
        _ => Verdict::Unexplained,
    };
    Some(Finding { stream, offset, verdict })
}

fn is_scaffold(p: &Placed) -> bool {
    p.path.contains("/recon/") && !p.is_fill() && p.size > 0 && p.address >= ld_map::ROM
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let mut edition = None;
    let mut rows = false;
    for argument in arguments {
        match argument.as_str() {
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            "--rows" => rows = true,
            value if edition.is_none() && !value.starts_with('-') => edition = Some(value.to_string()),
            value => return Err(format!("unexpected argument {value:?}\n{USAGE}")),
        }
    }
    let edition = edition.ok_or_else(|| USAGE.to_string())?;
    crate::targets::parse_decomp_target(&edition)?;
    let out = Path::new("out").join(&edition);
    let map_path = out.join(format!("{edition}.map"));
    let rom_path = out.join(format!("{edition}.gba"));
    let text = std::fs::read(&map_path).map_err(|e| format!("cannot read {}: {e}", map_path.display()))?;
    let map = Map::parse(&String::from_utf8_lossy(&text));
    let rom = std::fs::read(&rom_path).map_err(|e| format!("cannot read {}: {e}", rom_path.display()))?;
    let rom_end = ld_map::ROM + rom.len() as u32;
    let read = |address: u32, size: usize| -> Vec<u8> {
        let start = (address - ld_map::ROM) as usize;
        rom.get(start..start + size).map_or_else(Vec::new, <[u8]>::to_vec)
    };
    let (directory, table) =
        map.symbol("Resource_DirectoryTable").ok_or("the map has no Resource_DirectoryTable")?;
    let entries: Vec<u32> = (table..directory.end())
        .step_by(4)
        .map(|a| u32::from_le_bytes(read(a, 4).try_into().unwrap_or([0; 4])))
        .collect();
    let name = |address: u32| {
        map.placed
            .iter()
            .find_map(|p| ld_map::preferred(&p.symbols, address))
            .map_or_else(|| format!("Resource_{address:08x}"), str::to_string)
    };
    let streams = streams(&entries, &name, rom_end);
    let mut previous: Option<&Placed> = None;
    let mut counts = [(0usize, 0usize); 3];
    for placed in &map.placed {
        if placed.is_fill() || placed.size == 0 {
            continue;
        }
        let before = std::mem::replace(&mut previous, Some(placed));
        if !is_scaffold(placed) {
            continue;
        }
        let Some(size) = residue_size(&streams, placed.address, placed.size) else {
            continue;
        };
        let bytes = read(placed.address, size as usize);
        let Some(finding) = explain(&streams, placed.address, &bytes, &read) else {
            continue;
        };
        let after = before.filter(|b| b.end() == placed.address).map_or("", Placed::object);
        let part = if size == placed.size { "whole" } else { "head" };
        let list = bytes.iter().map(|b| format!("0x{b:02x}")).collect::<Vec<_>>().join(", ");
        let (kind, verdict, row) = match finding.verdict {
            Verdict::Left { provider } => {
                let by = format!("{}+{:#x}", streams[provider].name, finding.offset);
                (0, format!("left\t{by}"), Some(format!(".byte {list}\t@ PACKER: left in the buffer by {by}")))
            }
            Verdict::Fill(0) => (
                1,
                "fill\t-".to_string(),
                Some(".balign 4, 0\t@ PACKER: zero fill, not an earlier stream's bytes".to_string()),
            ),
            Verdict::Fill(byte) => (
                1,
                "fill\t-".to_string(),
                Some(format!(".balign 4, {byte:#04x}\t@ PACKER: {byte:#04x} fill, not an earlier stream's bytes")),
            ),
            Verdict::Unexplained => (2, "unexplained\t-".to_string(), None),
        };
        counts[kind].0 += 1;
        counts[kind].1 += bytes.len();
        if !rows {
            let stream = &streams[finding.stream];
            println!(
                "{:#010x}\t{size}\t{part}\t{list}\t{}+{:#x}\t{verdict}\t{after}",
                placed.address, stream.name, finding.offset
            );
        } else if let Some(row) = row {
            println!("{after}\t{}\t{part}\t{size}\t{row}", placed.name);
        }
    }
    let [left, fill, unexplained] = counts;
    eprintln!(
        "{edition}: {} left ({} bytes), {} fill ({} bytes), {} unexplained ({} bytes)",
        left.0, left.1, fill.0, fill.1, unexplained.0, unexplained.1
    );
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    // Resource 0 at 0x08000000 (12 bytes), resource 1 at 0x0800000c
    // (8 bytes: a stream of 6 and 2 bytes resource 0 left), resource 2 at
    // 0x08000014 (4 bytes: a stream of 1 and 3 zeros).
    const ROM: [u8; 24] = [
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, //
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 7, 8, //
        0xb0, 0, 0, 0,
    ];

    fn read(address: u32, size: usize) -> Vec<u8> {
        let start = (address - ld_map::ROM) as usize;
        ROM[start..start + size].to_vec()
    }

    fn sample() -> Vec<Stream> {
        let entries = [0x0800_0000, 0x0800_000c, 0x0800_000c, 0x0800_0014, 0x0300_0000];
        streams(&entries, &|a| format!("R{a:x}"), 0x0800_0018)
    }

    #[test]
    fn streams_take_directory_order_and_rom_extents() {
        let s = sample();
        assert_eq!(s.len(), 3);
        assert_eq!((s[0].start, s[0].end), (0x0800_0000, 0x0800_000c));
        assert_eq!((s[1].start, s[1].end), (0x0800_000c, 0x0800_0014));
        assert_eq!((s[2].start, s[2].end), (0x0800_0014, 0x0800_0018));
    }

    #[test]
    fn residue_is_a_whole_piece_or_its_head() {
        let s = sample();
        assert_eq!(residue_size(&s, 0x0800_0012, 2), Some(2));
        assert_eq!(residue_size(&s, 0x0800_0012, 0x20), Some(2));
        assert_eq!(residue_size(&s, 0x0800_0012, 1), None);
        assert_eq!(residue_size(&s, 0x0800_000c, 8), None);
        assert_eq!(residue_size(&s, 0x0800_0004, 8), None);
    }

    #[test]
    fn residue_is_the_last_long_enough_writer() {
        let found = explain(&sample(), 0x0800_0012, &[7, 8], &read).unwrap();
        assert_eq!(found, Finding { stream: 1, offset: 6, verdict: Verdict::Left { provider: 0 } });
    }

    #[test]
    fn zeros_and_strangers_are_told_apart() {
        // Resource 1 is the most recent long enough: its bytes 1-3 are
        // a1 a2 a3, not zeros.
        let zero = explain(&sample(), 0x0800_0015, &[0, 0, 0], &read).unwrap();
        assert_eq!(zero.verdict, Verdict::Fill(0));
        let ff = explain(&sample(), 0x0800_0015, &[0xff, 0xff, 0xff], &read).unwrap();
        assert_eq!(ff.verdict, Verdict::Fill(0xff));
        let other = explain(&sample(), 0x0800_0015, &[9, 9, 9], &read).unwrap();
        assert_eq!(other.verdict, Verdict::Unexplained);
        // Not at a stream's end.
        assert!(explain(&sample(), 0x0800_0004, &[5, 6], &read).is_none());
        assert!(explain(&sample(), 0x0800_0000, &[1], &read).is_none());
    }
}
