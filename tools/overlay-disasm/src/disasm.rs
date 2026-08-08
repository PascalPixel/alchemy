//! Disassembly, reachability, the `_call_via_rN` bank, and the reconstruction
//! loop.

use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;

use discover::{Discovery, Mode};

use crate::compile::{assemble_overlay, hex, spawn_raw, split_lines, strings, TempDir};
use crate::paths::OverlaySource;
use crate::regex::Regex;

pub const ROM_BASE: i64 = 0x0800_0000;
pub const OVERLAY_BASE: i64 = 0x0200_0000;

/// `ROW`, `TARGET` and `ERRLINE`, verbatim.
///
/// PORT NOTE: NONE of the three carries the `i` flag, and that is deliberate on
/// all three. `ROW`'s `[0-9a-f]` must not match objdump's upper-case symbolic
/// operands, `TARGET`'s mnemonic alternation must not match a `BL` inside a
/// comment, and `ERRLINE` must match GNU as's exact `Error:` spelling and not
/// `error:`. A missing `i` flag next to a `gi` sibling is a defect this tree has
/// really had, so each of the three is pinned by a test below.
const ROW: &str = r"\s*([0-9a-f]+):\t((?:[0-9a-f]{2,4} )+)\s*(\S.*)";
const TARGET: &str = r"\b(b|bl|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[nw])?\s+0x([0-9a-f]+)\b";
const ERRLINE: &str = r":(\d+): Error:";

/// `[size, text]`.
type Row = (i64, String);

/// `objdumpRows(data, base)`.
///
/// PORT NOTE: the alternation in `TARGET` is LEFTMOST, not longest. `b` precedes
/// `bl`, so on `bl 0x2000240` the engine tries `b` first, then requires
/// `(\.[nw])?\s+`, sees `l`, and backtracks into `bl`. The hand-written engine
/// in [`crate::regex`] backtracks the same way; a longest-alternation engine
/// would too, here, but only by luck. Pinned by a test.
fn objdump_rows(data: &[u8], base: i64) -> Result<BTreeMap<i64, Row>, String> {
    let work = TempDir::new("alchemy-overlay-objdump-").map_err(|error| error.to_string())?;
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
    // PORT NOTE: the TypeScript builds an insertion-ordered `Map` here, but the
    // only operation on it is `get`, so an ascending `BTreeMap` is
    // indistinguishable. Recorded rather than assumed.
    let mut rows: BTreeMap<i64, Row> = BTreeMap::new();
    let text = String::from_utf8_lossy(&stdout).to_string();
    for line in split_lines(&text) {
        let Some(found) = row.exec(&line) else { continue };
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

/// `reachable(input, base)`.
fn reachable(input: &[u8], base: i64) -> BTreeMap<i64, i64> {
    let length = input.len() as i64;
    let mut discovery = Discovery::new(input, base);
    // The seeds, in the order the TypeScript inserts them. `functions.keys()` is
    // insertion order there and the walk below consumes it in that order, so the
    // order is BEHAVIOUR, not incidental: `walk_function` mutates shared state.
    // The Rust `Discovery` keeps `functions` private, so the same list is
    // rebuilt here from `add_seed`'s "newly inserted" return, which is exactly
    // what `functions.order` records.
    let mut order: Vec<i64> = Vec::new();
    let read_u16 = |offset: i64| -> i64 {
        let at = offset as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let mut offset = 0i64;
    while offset < length - 1 {
        // THE ROOT PROLOGUE DOOR, stated because it is a design constraint and
        // not a one-line bug (2026-08-01). Discovery is seeded on
        // `push {..., lr}` and on the veneer shape below, and on nothing else,
        // so the inventory this produces STRUCTURALLY CANNOT contain a leaf, and
        // every consumer that filters on `starts_with_prologue` inherits that
        // blindness rather than adding it. Four real leaves were adopted on
        // 2026-08-01 and not one of them has an inventory row.
        //
        // It is not fixable here. A leaf offers no entry signature at all; you
        // cannot walk backwards from a `bx lr` to a function start. The
        // addresses that DO find leaves come from outside this file, published
        // pointer words and resolved `bl` targets, which sweeps A and B scan, so
        // the sanctioned path for a leaf is a hand-written `manual_regions`
        // entry with a ROM-measured span.
        if read_u16(offset) & 0xff00 == 0xb500 && discovery.add_seed(base + offset, Mode::Thumb, "prologue") {
            order.push((base + offset) & !1);
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
            if base <= target && target < base + length && discovery.add_seed(target & !1, Mode::Thumb, "stub") {
                order.push(target & !1);
            }
        }
        offset += 2;
    }
    for entry in order {
        // PORT NOTE: the TypeScript wraps this in a bare `try {} catch {}`. The
        // Rust walker signals the same class of failure by panicking, so the
        // swallow is reproduced rather than allowed to abort the run. A panic
        // here leaves `discovery` exactly as partially-mutated as the thrown
        // TypeScript did.
        let caught = std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| {
            discovery.walk_function(entry);
        }));
        drop(caught);
    }
    discovery
        .instructions
        .iter()
        .filter(|(address, _)| base <= **address && **address < base + length)
        .map(|(address, info)| (*address, info.size))
        .collect()
}

/// `callViaBankBase(image, base)`.
pub fn call_via_bank_base(image: &[u8], base: i64) -> Option<i64> {
    let halfword = |offset: i64| -> i64 {
        let at = offset as usize;
        image[at] as i64 | ((image[at + 1] as i64) << 8)
    };
    let length = image.len() as i64;

    // The bank itself, found by its shape: `bx r0 / nop ... bx r3 / nop`. A `bx`
    // of a different register or a missing `nop` breaks the run, and it has to
    // start at r0, so the match is unambiguous.
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

    // The bank's address in the image is not the address a `bl` to it encodes.
    // An overlay `bl` stores `target - 2` as a displacement that is not the
    // assembler's PC-relative one, so the two disagree by a per-overlay constant
    // and the assembler needs the LINKING address to reproduce the bytes.
    //
    // Recover it from any call the overlay already makes into its own bank. One
    // site gives both readings of the same branch: the stored rule says which
    // slot it reaches, which names the register, and the assembler's rule says
    // what address that slot must have for these bytes to come out. The base is
    // then the linking address of slot zero. Verified against two overlays whose
    // constants differ (resource_373 +0x60, resource_3bc +0x19e), so a fixed
    // offset would have been wrong.
    //
    // Take the CONSENSUS over every such site rather than the first. A rehearsal
    // blanks the span it is about to replace, so the first matching site can be
    // the one inside the placeholder, and a lone stray `bl` whose displacement
    // happens to land on a slot would otherwise decide the whole overlay.
    //
    // PORT NOTE: `votes` is an insertion-ordered `Map` and the winner is chosen
    // with a STRICT `>`, so a tie is won by whichever candidate was SEEN FIRST,
    // which is the lowest site offset. A `BTreeMap` iterates by candidate
    // address instead, which is a different tie-break, so the first-seen order
    // is reconstructed explicitly below.
    let mut votes: Vec<(i64, i64)> = Vec::new();
    let mut site = 0i64;
    while site + 4 <= length {
        let high = halfword(site);
        let low = halfword(site + 2);
        if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
            site += 2;
            continue;
        }
        // PORT NOTE: `<< 9 >> 9` is a 23-bit SIGN EXTENSION performed by JS's
        // 32-bit bitwise operators. `as i32` would saturate rather than wrap, so
        // the shifts are done in i32 and widened back to i64.
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

/// `buildOverlaySource(input, base)`.
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
    // PORT NOTE: `labels` is read-only after construction and is built in
    // ascending key order anyway (the loop sorts), so a `BTreeMap` is exact.
    let mut labels: BTreeMap<i64, String> = BTreeMap::new();
    for address in instructions.keys() {
        let Some(row) = rows.get(address) else { continue };
        if let Some(found) = target_pattern.exec(&row.1) {
            let text = &row.1;
            let target = crate::compile::js_parse_int_hex(found.group(text, 3).expect("group 3"))
                .ok_or_else(|| format!("branch target is not hex: {text}"))?;
            if instructions.contains_key(&target) {
                labels.entry(target).or_insert_with(|| format!(".L_{}", hex(target, 8)));
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
        // `externals` is rebuilt from scratch on every attempt: a row forced raw
        // may have been the only reference to a `sub_` symbol, and emitting a
        // `.set` for a symbol nothing mentions would change the output.
        //
        // PORT NOTE: insertion-ordered `Map` in the TypeScript, but every read
        // of it is `[...externals].sort((a, b) => a[0] - b[0])`, so a
        // `BTreeMap`'s ascending order is the same list.
        let mut externals: BTreeMap<i64, String> = BTreeMap::new();
        let mut body: Vec<(i64, &'static str, String)> = Vec::new();
        let mut cursor = base;
        while cursor < end {
            if let Some(local) = labels.get(&cursor) {
                body.push((cursor, "label", format!("{local}:")));
            }
            let row = rows.get(&cursor);
            let whole_row_covered = row.is_some_and(|row| (0..row.0).all(|byte| covered.contains(&(cursor + byte))));
            if !raw.contains(&cursor) && instructions.contains_key(&cursor) && whole_row_covered {
                let row = row.expect("checked above");
                let mnemonic = &row.1;
                let retargeted = match target_pattern.exec(mnemonic) {
                    None => mnemonic.clone(),
                    Some(found) => {
                        let target =
                            crate::compile::js_parse_int_hex(found.group(mnemonic, 3).expect("group 3"))
                                .ok_or_else(|| format!("branch target is not hex: {mnemonic}"))?;
                        let replacement = match labels.get(&target) {
                            Some(local) => local.clone(),
                            None => {
                                let symbol = format!("sub_{}", hex(target, 8));
                                externals.insert(target, symbol.clone());
                                symbol
                            }
                        };
                        // PORT NOTE: `lastIndexOf("0x")` on a string that the
                        // regex proved contains `0x`. It cannot be -1 here; if
                        // it were, JS `slice(0, -1)` would silently drop the
                        // last character where Rust would panic, so the
                        // impossible case is named rather than indexed blindly.
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
                && !instructions.contains_key(&(cursor + 4));
            if aligned {
                body.push((cursor, "data", format!("\t.4byte 0x{}", hex(read_u32(cursor - base), 8))));
                cursor += 4;
            } else {
                body.push((cursor, "data", format!("\t.2byte 0x{}", hex(read_u16(cursor - base), 4))));
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
                let Some(found) = error_line.exec(&error) else { return Err(error) };
                // PORT NOTE: `Number(match[1])`, decimal. `\d` is ASCII-only in
                // JS, so the capture cannot contain a non-ASCII digit that
                // `parse` would also reject differently.
                let reported: i64 = found
                    .group(&error, 1)
                    .expect("group 1")
                    .parse()
                    .map_err(|parse: std::num::ParseIntError| parse.to_string())?;
                let index = reported - head.len() as i64 - 1;
                if index < 0
                    || index >= body.len() as i64
                    || raw.contains(&body[index as usize].0)
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
        while difference < built.len().min(decoded.len()) && built[difference] == decoded[difference] {
            difference += 1;
        }
        let owner = base + difference as i64;
        let mut forced = false;
        // PORT NOTE: the TypeScript iterates `discovery.instructions` in
        // INSERTION order (walk order) and takes the FIRST instruction whose
        // span contains `owner` and which is not already raw. This map is
        // ascending. Instruction spans in the inventory do not overlap, so at
        // most one entry can contain `owner` and the two orders select the same
        // row; the divergence is recorded because it is real but unobservable,
        // not because it is safe by construction.
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

/// Convenience: reconstruct straight from an overlay image on disk.
pub fn build_overlay_source_from_file(path: &Path, base: i64) -> Result<String, String> {
    let data = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    build_overlay_source(&data, base)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn target() -> Regex {
        Regex::new(TARGET, "")
    }

    #[test]
    fn target_prefers_the_longer_mnemonic_by_backtracking() {
        let text = "bl\t0x2000240";
        let found = target().exec(text).expect("matches");
        assert_eq!(found.group(text, 1), Some("bl"));
        assert_eq!(found.group(text, 3), Some("2000240"));
    }

    #[test]
    fn target_reads_the_wide_suffix() {
        let text = "b.w\t0x2000100";
        let found = target().exec(text).expect("matches");
        assert_eq!(found.group(text, 2), Some(".w"));
        assert_eq!(found.group(text, 3), Some("2000100"));
    }

    /// TRAP: no `i` flag. objdump prints lower case, and an upper-case operand
    /// inside a symbol name must not be read as a branch target.
    #[test]
    fn target_is_case_sensitive() {
        assert!(target().exec("BL\t0x2000240").is_none());
        // Upper-case hex digits are not part of the target either. Verified
        // against Bun: the whole match FAILS rather than truncating, because
        // the trailing `\b` cannot sit between `0` and `A`.
        assert!(target().exec("bl\t0x200ABCD").is_none());
    }

    /// TRAP: `\b` in JS is ASCII-only, so it is a boundary before a non-ASCII
    /// letter where a Unicode-aware `\b` would not be.
    #[test]
    fn target_word_boundary_is_ascii_only() {
        assert!(target().exec("\u{00e9}bl 0x2000240").is_some());
        assert!(target().exec("xbl 0x2000240").is_none());
    }

    #[test]
    fn row_rejects_upper_case_and_requires_the_tab() {
        let row = Regex::new(ROW, "");
        let line = "  2000240:\t4770      \tbx\tlr";
        let found = row.exec(line).expect("matches");
        assert_eq!(found.group(line, 1), Some("2000240"));
        assert_eq!(crate::regex::js_trim(found.group(line, 2).expect("g2")), "4770");
        assert!(row.exec("  2000240: 4770      bx lr").is_none(), "the tab is required");
        assert!(row.exec("  2000ABC:\t4770 \tbx\tlr").is_none(), "upper-case address must not match");
    }

    #[test]
    fn errline_is_case_sensitive_on_error() {
        let errline = Regex::new(ERRLINE, "");
        let text = "/tmp/o.s:41: Error: junk at end of line";
        assert_eq!(errline.exec(text).expect("matches").group(text, 1), Some("41"));
        assert!(errline.exec("/tmp/o.s:41: error: junk").is_none());
    }

    #[test]
    fn call_via_bank_base_returns_none_without_a_bank() {
        assert_eq!(call_via_bank_base(&[0u8; 64], OVERLAY_BASE), None);
        assert_eq!(call_via_bank_base(&[], OVERLAY_BASE), None);
    }

    /// The bank shape plus one `bl` into slot 2, built by hand.
    #[test]
    fn call_via_bank_base_votes_on_a_synthetic_bank() {
        let mut image = vec![0u8; 0x200];
        let bank = 0x100i64;
        for slot in 0..4i64 {
            let at = (bank + slot * 4) as usize;
            let bx = (0x4700 | (slot << 3)) as u16;
            image[at] = (bx & 0xff) as u8;
            image[at + 1] = (bx >> 8) as u8;
            image[at + 2] = 0xc0;
            image[at + 3] = 0x46;
        }
        // Encode a `bl` at site 0 whose stored displacement reaches slot 8.
        let site = 0i64;
        let stored = bank + 8 - 2;
        let high = 0xf000u16 | (((stored >> 12) & 0x7ff) as u16);
        let low = 0xf800u16 | (((stored >> 1) & 0x7ff) as u16);
        image[site as usize] = (high & 0xff) as u8;
        image[site as usize + 1] = (high >> 8) as u8;
        image[site as usize + 2] = (low & 0xff) as u8;
        image[site as usize + 3] = (low >> 8) as u8;
        let expected = OVERLAY_BASE + site + 4 + stored - 8;
        assert_eq!(call_via_bank_base(&image, OVERLAY_BASE), Some(expected));
    }

    /// TRAP: the winner is chosen with a strict `>`, so a tie goes to the
    /// FIRST-SEEN candidate, not the numerically smallest. A `BTreeMap` tally
    /// would silently change that.
    #[test]
    fn a_tie_is_won_by_the_first_seen_candidate() {
        let mut votes: Vec<(i64, i64)> = vec![(0x0200_0100, 1), (0x0200_0000, 1)];
        let mut best: Option<i64> = None;
        let mut best_votes = 0;
        for (candidate, count) in votes.drain(..) {
            if count > best_votes {
                best = Some(candidate);
                best_votes = count;
            }
        }
        assert_eq!(best, Some(0x0200_0100), "first seen wins, not smallest");
    }

    #[test]
    fn odd_length_images_are_rejected() {
        assert_eq!(
            build_overlay_source(&[0u8; 3], OVERLAY_BASE).unwrap_err(),
            "overlay has an odd byte length"
        );
    }

    /// TRAP: `<< 9 >> 9` is a 32-bit wrapping sign extension. `as i32` on an
    /// out-of-range i64 SATURATES, which would give a different displacement.
    #[test]
    fn displacement_sign_extension_wraps_like_js() {
        let packed: i64 = 0x7f_ffff; // the largest 23-bit value: negative once extended
        let stored = i64::from(((packed as i32) << 9) >> 9);
        assert_eq!(stored, -1);
        let positive: i64 = 0x3f_ffff;
        assert_eq!(i64::from(((positive as i32) << 9) >> 9), 0x3f_ffff);
    }
}
