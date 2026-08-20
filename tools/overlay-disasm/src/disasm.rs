use crate::compile::{assemble_overlay, hex, spawn_raw, split_lines, strings};
use crate::paths::OverlaySource;
use crate::regex::Regex;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;
use tempfile::tempdir;
pub const ROM_BASE: i64 = 0x0800_0000;
pub const OVERLAY_BASE: i64 = 0x0200_0000;
const ROW: &str = r"\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)";
const TARGET: &str = r"\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b";
const ERRLINE: &str = r":(\d+): Error:";
type Row = (i64, String);
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
    let row = Regex::new(ROW, "");
    let mut rows: BTreeMap<i64, Row> = BTreeMap::new();
    let text = String::from_utf8_lossy(&stdout).to_string();
    for line in split_lines(&text) {
        let Some(found) = row.exec(&line) else {
            continue;
        };
        let address = crate::compile::js_parse_int_hex(found.group(&line, 1).expect("group 1"))
            .ok_or_else(|| format!("objdump row address is not hex: {line}"))?;
        let bytes = found.group(&line, 2).expect("group 2");
        let count = crate::regex::js_trim(bytes)
            .split(|c: char| crate::regex::is_js_space(c))
            .filter(|part| !part.is_empty())
            .count() as i64;
        let mnemonic = found.group(&line, 3).expect("group 3");
        let text = crate::regex::js_trim(mnemonic.split(';').next().unwrap_or("")).to_string();
        rows.insert(address, (2 * count, text));
    }
    Ok(rows)
}
fn reachable(input: &[u8], base: i64) -> BTreeMap<i64, i64> {
    let length = input.len() as i64;
    let read_u16 = |offset: i64| -> i64 {
        let at = offset as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let inside = |address: i64, size: i64| base <= address && address + size <= base + length;
    let sign_extend = |value: i64, bits: u32| (value << (64 - bits)) >> (64 - bits);
    let mut queue = Vec::new();
    let mut offset = 0i64;
    while offset < length - 1 {
        if read_u16(offset) & 0xff00 == 0xb500 {
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
    let mut instructions = BTreeMap::new();
    let mut head = 0usize;
    while head < queue.len() {
        let mut pc = queue[head];
        head += 1;
        while inside(pc, 2) && !instructions.contains_key(&pc) {
            let half = read_u16(pc - base);
            let mut size = 2;
            let mut stop = false;
            if half & 0xf800 == 0xf000 && inside(pc, 4) {
                let low = read_u16(pc + 2 - base);
                if low & 0xf800 == 0xf800 {
                    size = 4;
                    let displacement =
                        sign_extend(((half & 0x7ff) << 12) | ((low & 0x7ff) << 1), 23);
                    let target = pc + 4 + displacement;
                    if inside(target, 2) {
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
    instructions
}
pub fn call_via_bank_base(image: &[u8], base: i64) -> Option<i64> {
    let halfword = |offset: i64| -> i64 {
        let at = offset as usize;
        image[at] as i64 | ((image[at + 1] as i64) << 8)
    };
    let length = image.len() as i64;
    let mut bank: i64 = -1;
    let mut offset = 0i64;
    while offset + 16 <= length && bank < 0 {
        let mut matched = true;
        let mut slot = 0i64;
        while slot < 4 && matched {
            matched = halfword(offset + slot * 4) == (0x4700 | (slot << 3))
                && halfword(offset + slot * 4 + 2) == 0x46c0;
            slot += 1;
        }
        if matched {
            bank = offset;
        }
        offset += 2;
    }
    if bank < 0 {
        return None;
    }
    let mut votes: Vec<(i64, i64)> = Vec::new();
    let mut site = 0i64;
    while site + 4 <= length {
        let high = halfword(site);
        let low = halfword(site + 2);
        if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
            site += 2;
            continue;
        }
        let packed = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
        let stored = i64::from(((packed as i32) << 9) >> 9);
        let slot = stored + 2 - bank;
        if !(0..=13 * 4).contains(&slot) || slot % 4 != 0 {
            site += 2;
            continue;
        }
        if halfword(bank + slot) != (0x4700 | ((slot / 4) << 3)) {
            site += 2;
            continue;
        }
        let candidate = base + site + 4 + stored - slot;
        match votes.iter_mut().find(|(key, _)| *key == candidate) {
            Some(entry) => entry.1 += 1,
            None => votes.push((candidate, 1)),
        }
        site += 2;
    }
    let mut best: Option<i64> = None;
    let mut best_votes = 0i64;
    for (candidate, count) in votes {
        if count > best_votes {
            best = Some(candidate);
            best_votes = count;
        }
    }
    best
}
pub fn build_overlay_source(input: &[u8], base: i64) -> Result<String, String> {
    let decoded = input;
    if !decoded.len().is_multiple_of(2) {
        return Err("overlay has an odd byte length".to_string());
    }
    let rows = objdump_rows(decoded, base)?;
    let instructions = reachable(decoded, base);
    let mut covered: BTreeSet<i64> = BTreeSet::new();
    for (address, size) in &instructions {
        for byte in *address..*address + *size {
            covered.insert(byte);
        }
    }
    let target_pattern = Regex::new(TARGET, "");
    let mut labels: BTreeMap<i64, String> = BTreeMap::new();
    for address in instructions.keys() {
        let Some(row) = rows.get(address) else {
            continue;
        };
        if let Some(found) = target_pattern.exec(&row.1) {
            let text = &row.1;
            let target = crate::compile::js_parse_int_hex(found.group(text, 3).expect("group 3"))
                .ok_or_else(|| format!("branch target is not hex: {text}"))?;
            if instructions.contains_key(&target)
                && instructions.get(&(target - 2)).copied() != Some(4)
                && rows.get(&(target - 2)).is_none_or(|row| row.0 != 4)
            {
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
    let error_line = Regex::new(ERRLINE, "");
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
                let retargeted = match target_pattern.exec(mnemonic) {
                    None => mnemonic.clone(),
                    Some(found) => {
                        let target = crate::compile::js_parse_int_hex(
                            found.group(mnemonic, 3).expect("group 3"),
                        )
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
                let Some(found) = error_line.exec(&error) else {
                    return Err(error);
                };
                let reported: i64 = found
                    .group(&error, 1)
                    .expect("group 1")
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
pub fn build_overlay_source_from_file(path: &Path, base: i64) -> Result<String, String> {
    let data = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    build_overlay_source(&data, base)
}
