use crate::overlay::compile::{assemble_overlay, hex, spawn_raw, split_lines, strings};
use crate::overlay::source::OverlaySource;
use regex::Regex;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use tempfile::tempdir;
pub const ROM_BASE: i64 = 0x0800_0000;
pub const OVERLAY_BASE: i64 = 0x0200_0000;
const ROW: &str = r"\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)";
const TARGET: &str = r"\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b";
const ERRLINE: &str = r":(\d+): Error:";
type Row = (i64, String);

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn binutils_patterns_and_decoded_source_roundtrip() {
        let branch = Regex::new(TARGET).unwrap();
        assert_eq!(&branch.captures("beq.n 0x02000008").unwrap()[3], "02000008");
        assert!(branch.captures("notbl 0x02000008").is_none());
        assert_eq!(
            &Regex::new(ERRLINE)
                .unwrap()
                .captures("input.s:12: Error: bad instruction")
                .unwrap()[1],
            "12"
        );
        // push {lr}; movs r0, #1; pop {pc}
        let bytes = [0x00, 0xb5, 0x01, 0x20, 0x00, 0xbd];
        let text = build_region_source(&bytes, OVERLAY_BASE).unwrap();
        assert!(text.contains("push") && text.contains("pop"));
        assert_eq!(
            assemble_overlay(&OverlaySource::text(text), OVERLAY_BASE).unwrap(),
            bytes
        );
    }
}

fn objdump_rows(data: &[u8], base: i64) -> Result<BTreeMap<i64, Row>, String> {
    let work = tempdir().map_err(|error| error.to_string())?;
    let binary = work.path().join("overlay.bin");
    fs::write(&binary, data).map_err(|error| error.to_string())?;
    let stdout = spawn_raw(
        &{
            let mut command = strings(&[
                "arm-none-eabi-objdump",
                "-D",
                "-b",
                "binary",
                "-marmv4t",
                "-Mforce-thumb",
            ]);
            command.push(format!("--adjust-vma=0x{}", hex(base, 8)));
            command.push(binary.to_string_lossy().to_string());
            command
        },
        work.path(),
    )?;
    let row = Regex::new(ROW).unwrap();
    let mut rows: BTreeMap<i64, Row> = BTreeMap::new();
    let text = String::from_utf8_lossy(&stdout).to_string();
    for line in split_lines(&text) {
        let Some(found) = row.captures(&line) else {
            continue;
        };
        let address = crate::overlay::compile::js_parse_int_hex(&found[1])
            .ok_or_else(|| format!("objdump row address is not hex: {line}"))?;
        let count = found[2].split_whitespace().count() as i64;
        let mnemonic = &found[3];
        let text = mnemonic.split(';').next().unwrap_or("").trim().to_string();
        rows.insert(address, (2 * count, text));
    }
    Ok(rows)
}
/// Instructions reachable from the image's prologues and veneers, plus the
/// given `seeds`. With `sweep`, every halfword left uncovered that is not a
/// pool word some reached `ldr rd, [pc, #k]` loads is seeded in turn, so a
/// region known to be code (a main-image owner's audited extent) is decoded
/// through leaf functions without `push {lr}` and past computed branches;
/// the emitter's byte check demotes anything that was really data.
fn reachable(input: &[u8], base: i64, seeds: &[i64], sweep: bool) -> BTreeMap<i64, i64> {
    let length = input.len() as i64;
    let read_u16 = |offset: i64| -> i64 {
        let at = offset as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let inside = |address: i64, size: i64| base <= address && address + size <= base + length;
    let sign_extend = |value: i64, bits: u32| (value << (64 - bits)) >> (64 - bits);
    let mut queue: Vec<i64> = seeds.iter().copied().filter(|s| inside(*s, 2)).collect();
    let mut offset = 0i64;
    while offset < length - 1 {
        // A veneer's target word is not a prologue, whatever its low half.
        let veneer_target = offset % 4 == 0
            && offset >= 4
            && read_u16(offset - 4) == 0x4c00
            && read_u16(offset - 2) == 0x4720;
        if read_u16(offset) & 0xff00 == 0xb500 && !veneer_target {
            queue.push(base + offset);
        }
        offset += 2;
    }
    let mut offset = 0i64;
    while offset < length - 8 {
        let word = read_u16(offset);
        let following = read_u16(offset + 2);
        if (0x4800..=0x48ff).contains(&word) && (0x4700..=0x47ff).contains(&following) {
            let at = (offset + 4) as usize;
            let target = input[at] as i64
                | ((input[at + 1] as i64) << 8)
                | ((input[at + 2] as i64) << 16)
                | ((input[at + 3] as i64) << 24);
            let target = target & !1;
            if inside(target, 2) {
                queue.push(target);
            }
        }
        offset += 2;
    }
    let read_u32 = |offset: i64| -> i64 {
        let at = offset as usize;
        input[at] as i64
            | ((input[at + 1] as i64) << 8)
            | ((input[at + 2] as i64) << 16)
            | ((input[at + 3] as i64) << 24)
    };
    let mut instructions = BTreeMap::new();
    // Bytes of switch jump tables and of literal pools: data the emitter
    // must keep as words, and that no linear walk may run into.
    let mut tables: BTreeSet<i64> = BTreeSet::new();
    let mut pool: BTreeSet<i64> = BTreeSet::new();
    let mut head = 0usize;
    loop {
        while head < queue.len() {
            let mut pc = queue[head];
            head += 1;
            // The last few instructions of this run, for the switch idiom:
            // `cmp rN, #K` ... `ldr rT, [pc, #k]` ... `mov pc, rX`, where the
            // pool word holds a table of K+1 absolute code addresses.
            while inside(pc, 2)
                && !instructions.contains_key(&pc)
                && !tables.contains(&pc)
                && !pool.contains(&pc)
            {
                let half = read_u16(pc - base);
                let mut size = 2;
                let mut stop = false;
                if half & 0xff87 == 0x4687 && sweep {
                    // The guard and the table load sit just before the
                    // dispatch by address, the guard usually in the block
                    // that falls through an unconditional branch.
                    let preceding: Vec<(i64, i64)> = (1..=12)
                        .map(|k| pc - 2 * k)
                        .filter(|at| inside(*at, 2))
                        .map(|at| (at, read_u16(at - base)))
                        .collect();
                    let limit = preceding
                        .iter()
                        .find(|(_, h)| h & 0xf800 == 0x2800)
                        .map(|(_, h)| (h & 0xff) + 1);
                    let table = preceding.iter().find_map(|(at, h)| {
                        if h & 0xf800 != 0x4800 {
                            return None;
                        }
                        let word = ((at + 4) & !3) + ((h & 0xff) << 2);
                        inside(word, 4).then(|| read_u32(word - base))
                    });
                    if let (Some(count), Some(table)) = (limit, table) {
                        if table % 4 == 0 && inside(table, count * 4) {
                            for entry in 0..count {
                                let at = table + entry * 4;
                                for byte in at..at + 4 {
                                    tables.insert(byte);
                                }
                                let target = read_u32(at - base) & !1;
                                if inside(target, 2) {
                                    queue.push(target);
                                }
                            }
                        }
                    }
                }
                if half & 0xf800 == 0xf000 && inside(pc, 4) {
                    let low = read_u16(pc + 2 - base);
                    if low & 0xf800 == 0xf800 {
                        size = 4;
                        let displacement =
                            sign_extend(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
                        let target = pc + 4 + displacement;
                        // A whole overlay's functions are seeded by their
                        // prologues; a call names its target without walking
                        // it, so veneers and data a call lands on stay data.
                        if inside(target, 2) && sweep {
                            queue.push(target);
                        }
                    }
                } else if half & 0xf800 == 0xe000 {
                    let target = pc + 4 + (sign_extend(half & 0x7ff, 11) << 1);
                    if inside(target, 2) {
                        queue.push(target);
                    }
                    stop = true;
                } else if half & 0xf000 == 0xd000 && ((half >> 8) & 0xf) < 0xe {
                    let target = pc + 4 + (sign_extend(half & 0xff, 8) << 1);
                    if inside(target, 2) {
                        queue.push(target);
                    }
                } else if half & 0xff87 == 0x4700
                    || half & 0xff00 == 0xbd00
                    || (half & 0xfc00 == 0x4400 && half & 0x0087 == 0x0087)
                {
                    stop = true;
                }
                instructions.insert(pc, size);
                if stop {
                    break;
                }
                pc += size;
            }
        }
        if !sweep {
            break;
        }
        // Pool words: the targets of every reached pc-relative load.
        for (&pc, &size) in &instructions {
            if size == 2 {
                let half = read_u16(pc - base);
                if half & 0xf800 == 0x4800 {
                    let word = ((pc + 4) & !3) + ((half & 0xff) << 2);
                    for byte in word..word + 4 {
                        pool.insert(byte);
                    }
                }
            }
        }
        let mut covered: BTreeSet<i64> = BTreeSet::new();
        for (&pc, &size) in &instructions {
            for byte in pc..pc + size {
                covered.insert(byte);
            }
        }
        let mut next = None;
        let mut cursor = base;
        while cursor + 2 <= base + length {
            if !covered.contains(&cursor) && !pool.contains(&cursor) && !tables.contains(&cursor) {
                next = Some(cursor);
                break;
            }
            cursor += 2;
        }
        match next {
            Some(seed) => queue.push(seed),
            None => break,
        }
    }
    instructions
}
pub fn build_overlay_source(input: &[u8], base: i64) -> Result<String, String> {
    build_source(input, base, &[], false)
}
/// A main-image owner's audited extent: the entry is code whatever its
/// prologue looks like, and every byte not proved to be a pool word is
/// decoded as code before the byte check has its say.
pub fn build_region_source(input: &[u8], base: i64) -> Result<String, String> {
    build_source(input, base, &[base], true)
}
fn build_source(input: &[u8], base: i64, seeds: &[i64], sweep: bool) -> Result<String, String> {
    let decoded = input;
    if !decoded.len().is_multiple_of(2) {
        return Err("overlay has an odd byte length".to_string());
    }
    let rows = objdump_rows(decoded, base)?;
    let instructions = reachable(decoded, base, seeds, sweep);
    let mut covered: BTreeSet<i64> = BTreeSet::new();
    for (address, size) in &instructions {
        for byte in *address..*address + *size {
            covered.insert(byte);
        }
    }
    let target_pattern = Regex::new(TARGET).unwrap();
    let mut labels: BTreeMap<i64, String> = BTreeMap::new();
    for address in instructions.keys() {
        let Some(row) = rows.get(address) else {
            continue;
        };
        if let Some(found) = target_pattern.captures(&row.1) {
            let text = &row.1;
            let target = crate::overlay::compile::js_parse_int_hex(&found[3])
                .ok_or_else(|| format!("branch target is not hex: {text}"))?;
            // A label cannot sit inside a four-byte row emitted as code; a
            // row left as data is written in halfwords around the label.
            let swallowed = instructions.contains_key(&(target - 2))
                && (instructions.get(&(target - 2)).copied() == Some(4)
                    || rows.get(&(target - 2)).is_some_and(|row| row.0 == 4));
            if instructions.contains_key(&target) && !swallowed {
                labels
                    .entry(target)
                    .or_insert_with(|| format!(".L_{}", hex(target, 8)));
            }
        }
    }
    let end = base + decoded.len() as i64;
    let mut raw: BTreeSet<i64> = BTreeSet::new();
    let read_u32 = |offset: i64| -> i64 {
        let at = offset as usize;
        decoded[at] as i64
            | ((decoded[at + 1] as i64) << 8)
            | ((decoded[at + 2] as i64) << 16)
            | ((decoded[at + 3] as i64) << 24)
    };
    let read_u16 = |offset: i64| -> i64 {
        let at = offset as usize;
        decoded[at] as i64 | ((decoded[at + 1] as i64) << 8)
    };
    let error_line = Regex::new(ERRLINE).unwrap();
    for _attempt in 0..decoded.len() {
        let mut externals: BTreeMap<i64, String> = BTreeMap::new();
        let mut body: Vec<(i64, &'static str, String)> = Vec::new();
        let mut cursor = base;
        while cursor < end {
            if let Some(local) = labels.get(&cursor) {
                body.push((cursor, "label", format!("{local}:")));
            }
            let row = rows.get(&cursor);
            let whole_row_covered =
                row.is_some_and(|row| (0..row.0).all(|byte| covered.contains(&(cursor + byte))));
            if !raw.contains(&cursor) && instructions.contains_key(&cursor) && whole_row_covered {
                let row = row.expect("checked above");
                let mnemonic = &row.1;
                let retargeted = match target_pattern.captures(mnemonic) {
                    None => mnemonic.clone(),
                    Some(found) => {
                        let target = crate::overlay::compile::js_parse_int_hex(&found[3])
                            .ok_or_else(|| format!("branch target is not hex: {mnemonic}"))?;
                        let replacement = match labels.get(&target) {
                            Some(local) => local.clone(),
                            None => {
                                let symbol = format!("sub_{}", hex(target, 8));
                                externals.insert(target, symbol.clone());
                                symbol
                            }
                        };
                        let cut = mnemonic.rfind("0x").ok_or_else(|| {
                            format!("branch mnemonic matched but has no 0x: {mnemonic}")
                        })?;
                        format!("{}{replacement}", &mnemonic[..cut])
                    }
                };
                body.push((cursor, "code", format!("\t{retargeted}")));
                cursor += row.0;
                continue;
            }
            let aligned = cursor % 4 == 0
                && cursor + 4 <= end
                && (0..4).all(|byte| !covered.contains(&(cursor + byte)))
                && !labels.contains_key(&(cursor + 2))
                && !instructions.contains_key(&(cursor + 4));
            if aligned {
                body.push((
                    cursor,
                    "data",
                    format!("\t.4byte 0x{}", hex(read_u32(cursor - base), 8)),
                ));
                cursor += 4;
            } else {
                body.push((
                    cursor,
                    "data",
                    format!("\t.2byte 0x{}", hex(read_u16(cursor - base), 4)),
                ));
                cursor += 2;
            }
        }
        let mut head = vec![".syntax unified".to_string(), "\t.thumb".to_string()];
        for (target, name) in &externals {
            head.push(format!("\t.set {name}, 0x{}", hex(*target, 8)));
        }
        head.push(format!("\t.global Overlay_{}", hex(base, 8)));
        head.push(format!("Overlay_{}:", hex(base, 8)));
        let mut lines: Vec<String> = head.clone();
        lines.extend(body.iter().map(|row| row.2.clone()));
        let text = format!("{}\n", lines.join("\n"));
        let built = match assemble_overlay(&OverlaySource::text(text.clone()), base) {
            Ok(built) => built,
            Err(error) => {
                let Some(found) = error_line.captures(&error) else {
                    return Err(error);
                };
                let reported: i64 = found[1]
                    .parse()
                    .map_err(|parse: std::num::ParseIntError| parse.to_string())?;
                let index = reported - head.len() as i64 - 1;
                if index < 0 || index >= body.len() as i64 || raw.contains(&body[index as usize].0)
                {
                    return Err(format!("cannot reconstruct near line {reported}"));
                }
                raw.insert(body[index as usize].0);
                continue;
            }
        };
        if built == decoded {
            return Ok(text);
        }
        let mut difference = 0usize;
        while difference < built.len().min(decoded.len())
            && built[difference] == decoded[difference]
        {
            difference += 1;
        }
        let owner = base + difference as i64;
        let mut forced = false;
        for (address, size) in &instructions {
            if *address <= owner && owner < *address + *size && !raw.contains(address) {
                raw.insert(*address);
                forced = true;
                break;
            }
        }
        if !forced {
            return Err(format!("irreducible mismatch at 0x{}", hex(owner, 8)));
        }
    }
    Err("overlay reconstruction did not converge".to_string())
}
/// Retained assembly for a whole code overlay as an export writes it: the
/// reconstructed listing, with the game's `overlay_veneer` macro included and
/// every run of fixed veneers written through it. The leading `entry_veneers`
/// are the overlay's entry table; any later run is an import table. The
/// macro emits the same eight bytes per veneer, so reassembly is unchanged.
pub fn export_overlay_source(
    input: &[u8],
    base: i64,
    veneer_macro: &str,
    entry_veneers: usize,
) -> Result<String, String> {
    let listing = build_overlay_source(input, base)?;
    let lines: Vec<&str> = listing.lines().collect();
    let entry_label = format!("Overlay_{}:", hex(base, 8));
    let opening = lines
        .iter()
        .position(|line| *line == entry_label)
        .ok_or("reconstructed listing has no overlay entry label")?
        + 1;
    if lines.first() != Some(&".syntax unified") {
        return Err("reconstructed listing does not open with .syntax unified".into());
    }
    let mut output = vec![lines[0].to_string(), format!(".include \"{veneer_macro}\"")];
    let mut index = 1;
    while index < lines.len() {
        let mut run = veneer_run(&lines[index..]);
        if index == opening {
            if run.len() < entry_veneers {
                return Err(format!(
                    "overlay opens with {} fixed entry veneers; the game has {entry_veneers}",
                    run.len()
                ));
            }
            run.truncate(entry_veneers);
        }
        if run.is_empty() {
            output.push(lines[index].to_string());
            index += 1;
            continue;
        }
        let targets: Vec<String> = run
            .iter()
            .map(|(target, _)| format!("0x{target:08x}"))
            .collect();
        output.push(format!("\t.irp EntryTarget, {}", targets.join(", ")));
        output.push("\toverlay_veneer \\EntryTarget".to_string());
        output.push("\t.endr".to_string());
        index += run.iter().map(|(_, lines)| lines).sum::<usize>();
    }
    Ok(format!("{}\n", output.join("\n")))
}
/// The fixed veneers the listing opens with, as (target, listing lines).
/// Each is `.4byte 0x47204c00` then its target word, which the emitter
/// writes as one `.4byte` or, before a label or code, as two `.2byte`
/// halves. A target is Thumb, word-aligned ARM, or IWRAM/EWRAM/ROM.
fn veneer_run(lines: &[&str]) -> Vec<(u32, usize)> {
    let mut run = Vec::new();
    let mut at = 0;
    while lines.get(at) == Some(&"\t.4byte 0x47204c00") {
        let hex = |line: Option<&&str>, prefix: &str, width: usize| {
            line.and_then(|line| line.strip_prefix(prefix))
                .filter(|digits| digits.len() == width)
                .and_then(|digits| u32::from_str_radix(digits, 16).ok())
        };
        let (target, used) = match hex(lines.get(at + 1), "\t.4byte 0x", 8) {
            Some(target) => (target, 2),
            None => match (
                hex(lines.get(at + 1), "\t.2byte 0x", 4),
                hex(lines.get(at + 2), "\t.2byte 0x", 4),
            ) {
                (Some(low), Some(high)) => (low | high << 16, 3),
                _ => break,
            },
        };
        let addressable = matches!(target >> 24, 0x02 | 0x03 | 0x08);
        if !addressable || (target & 1 == 0 && target & 3 != 0) {
            break;
        }
        run.push((target, used));
        at += used;
    }
    run
}
#[cfg(test)]
mod export_tests {
    use super::*;

    #[test]
    fn exported_veneers_go_through_the_game_macro_and_reassemble() {
        // Two entry veneers, a leaf `bx lr`, one import veneer.
        let mut image = Vec::new();
        for target in [0x0200_8011u32, 0x0200_8015, 0x0800_00c1] {
            if target == 0x0800_00c1 {
                image.extend([0x70, 0x47, 0x00, 0x00]);
            }
            image.extend([0x00, 0x4c, 0x20, 0x47]);
            image.extend(target.to_le_bytes());
        }
        let text = export_overlay_source(
            &image,
            OVERLAY_BASE,
            "games/THE BROKEN SEAL/SRC/COMMON/OVERLAY.INC",
            2,
        )
        .unwrap();
        assert!(text.contains("\t.irp EntryTarget, 0x02008011, 0x02008015\n"));
        assert!(text.contains("\t.irp EntryTarget, 0x080000c1\n"));
        assert!(!text.contains("0x47204c00"));
        assert_eq!(
            crate::overlay::compile::assemble_overlay_raw(&OverlaySource::text(text), OVERLAY_BASE)
                .unwrap(),
            image
        );
        assert!(export_overlay_source(&image, OVERLAY_BASE, "OVERLAY.INC", 3).is_err());
    }
}
