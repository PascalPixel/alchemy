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

struct Reachability {
    instructions: BTreeMap<i64, i64>,
    tables: BTreeSet<i64>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct ExecutableSpan {
    pub start: i64,
    pub end: i64,
    pub kind: &'static str,
}

/// Recover a framed function's first halfword when the byte-identical source
/// emitter had to leave only that halfword as data (usually because objdump
/// grouped the following wide instruction differently). The following
/// halfword must already be independently classified as executable; a
/// prologue-shaped value in an unrelated table is not enough.
pub fn adjacent_prologue_spans(
    input: &[u8],
    base: i64,
    classified: &[ExecutableSpan],
) -> Vec<ExecutableSpan> {
    let covered = |address: i64| {
        classified
            .iter()
            .any(|span| span.start <= address && address < span.end)
    };
    (0..input.len().saturating_sub(1))
        .step_by(2)
        .filter_map(|offset| {
            let address = base + offset as i64;
            let half = u16::from_le_bytes([input[offset], input[offset + 1]]);
            (half & 0xff00 == 0xb500 && !covered(address) && covered(address + 2)).then_some(
                ExecutableSpan {
                    start: address,
                    end: address + 2,
                    kind: "thumb",
                },
            )
        })
        .collect()
}

/// GCC's unframed five-pair object integrator. It is emitted without a stack
/// frame, reached through data rather than a direct call in many field
/// overlays, and therefore has neither of the ordinary discovery anchors.
/// Match the complete instruction sequence (including its return) so nearby
/// object tables cannot be promoted merely for resembling one load/store.
pub fn compiler_idiom_spans(input: &[u8], base: i64) -> Vec<ExecutableSpan> {
    const OBJECT_INTEGRATOR: [u16; 27] = [
        0x6883, 0x6c42, 0x189b, 0x6083, 0x6c82, 0x68c3, 0x189b, 0x60c3, 0x6cc2, 0x6903, 0x189b,
        0x6103, 0x6b02, 0x6983, 0x189b, 0x6183, 0x6b42, 0x69c3, 0x189b, 0x61c3, 0x6d01, 0x3064,
        0x8bcb, 0x8802, 0x189b, 0x83cb, 0x4770,
    ];
    const STATUS_NIBBLE_UPDATE: [u16; 11] = [
        0x6d00, 0x2303, 0x7a42, 0x4019, 0x230d, 0x425b, 0x0089, 0x4013, 0x430b, 0x7243, 0x4770,
    ];
    let bytes = OBJECT_INTEGRATOR
        .iter()
        .flat_map(|half| half.to_le_bytes())
        .collect::<Vec<_>>();
    let mut spans = input
        .windows(bytes.len())
        .enumerate()
        .filter(|(offset, window)| offset % 2 == 0 && *window == bytes)
        .map(|(offset, _)| ExecutableSpan {
            start: base + offset as i64,
            end: base + offset as i64 + bytes.len() as i64,
            kind: "thumb",
        })
        .collect::<Vec<_>>();
    let status_bytes = STATUS_NIBBLE_UPDATE
        .iter()
        .flat_map(|half| half.to_le_bytes())
        .collect::<Vec<_>>();
    spans.extend(
        input
            .windows(status_bytes.len())
            .enumerate()
            .filter(|(offset, window)| offset % 2 == 0 && *window == status_bytes)
            .map(|(offset, _)| ExecutableSpan {
                start: base + offset as i64,
                end: base + offset as i64 + status_bytes.len() as i64,
                kind: "thumb",
            }),
    );
    // Stock GCC 2.96's Thumb interworking bank is fifteen consecutive
    // `bx rN; nop` pairs, r0 through lr. It has no framed entry and is often
    // linked into an overlay without any in-image pointer to its first pair.
    // The complete sixty-byte signature is source-defined by
    // gcc/config/arm/lib1funcs.asm; shorter bx/nop runs are not sufficient.
    let call_via = (0u16..15)
        .flat_map(|register| [0x4700 | (register << 3), 0x46c0])
        .flat_map(u16::to_le_bytes)
        .collect::<Vec<_>>();
    spans.extend(
        input
            .windows(call_via.len())
            .enumerate()
            .filter(|(offset, window)| offset % 2 == 0 && *window == call_via)
            .map(|(offset, _)| ExecutableSpan {
                start: base + offset as i64,
                end: base + offset as i64 + call_via.len() as i64,
                kind: "compiler_runtime",
            }),
    );
    let read_half = |offset: usize| u16::from_le_bytes([input[offset], input[offset + 1]]);
    let functions = spans.clone();
    for function in functions {
        if function.kind != "thumb" {
            continue;
        }
        let start = (function.start - base) as usize;
        let end = (function.end - base) as usize;
        if start >= 2 && read_half(start - 2) == 0 {
            spans.push(ExecutableSpan {
                start: function.start - 2,
                end: function.start,
                kind: "executable_alignment",
            });
        }
        if end + 2 <= input.len() && read_half(end) == 0 {
            spans.push(ExecutableSpan {
                start: function.end,
                end: function.end + 2,
                kind: "executable_alignment",
            });
        }
    }
    spans
}

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
            executable_spans(&bytes, OVERLAY_BASE)
                .unwrap()
                .iter()
                .map(|span| span.end - span.start)
                .sum::<i64>(),
            bytes.len() as i64
        );
        assert_eq!(
            assemble_overlay(&OverlaySource::text(text), OVERLAY_BASE).unwrap(),
            bytes
        );
    }

    #[test]
    fn trusted_switches_require_the_gcc_register_chain() {
        let mut image = vec![0u8; 0x60];
        image[0..4].copy_from_slice(&[0x00, 0x4c, 0x20, 0x47]);
        image[4..8].copy_from_slice(&0x0200_8021u32.to_le_bytes());
        for (at, half) in [
            (0x20, 0x2b01u16), // cmp r3, #1
            (0x22, 0x4a03),    // ldr r2, [pc, #12] -> 0x30
            (0x24, 0x009b),    // lsl r3, r3, #2
            (0x26, 0x589b),    // ldr r3, [r3, r2]
            (0x28, 0x469f),    // mov pc, r3
            (0x50, 0x4770),    // case 0
            (0x54, 0x4770),    // case 1
        ] {
            image[at..at + 2].copy_from_slice(&half.to_le_bytes());
        }
        image[0x30..0x34].copy_from_slice(&0x0200_8040u32.to_le_bytes());
        image[0x40..0x44].copy_from_slice(&0x0200_8051u32.to_le_bytes());
        image[0x44..0x48].copy_from_slice(&0x0200_8055u32.to_le_bytes());
        let spans = trusted_overlay_spans(&image, OVERLAY_BASE, 1).unwrap();
        assert!(spans.iter().any(|span| span.start == OVERLAY_BASE + 0x50));
        assert!(spans.iter().any(|span| span.start == OVERLAY_BASE + 0x54));
        assert!(!spans
            .iter()
            .any(|span| span.start < OVERLAY_BASE + 0x48 && span.end > OVERLAY_BASE + 0x40));

        // Change the scaled index to r1 while the comparison and indexed
        // load still use r3. The old nearby-opcode heuristic accepted this.
        image[0x24..0x26].copy_from_slice(&0x0089u16.to_le_bytes());
        let spans = trusted_overlay_spans(&image, OVERLAY_BASE, 1).unwrap();
        assert!(!spans.iter().any(|span| span.start == OVERLAY_BASE + 0x50));

        // The comparison reads the unscaled source, not necessarily the
        // destination of lsl (cmp r0; lsl r3, r0, #2).
        image[0x20..0x22].copy_from_slice(&0x2801u16.to_le_bytes());
        image[0x24..0x26].copy_from_slice(&0x0083u16.to_le_bytes());
        let spans = overlay_flow_spans(&image, OVERLAY_BASE, 1, true).unwrap();
        assert!(spans.iter().any(|span| span.start == OVERLAY_BASE + 0x50));
        assert!(spans.iter().any(|span| span.kind == "jump_table"
            && span.start == OVERLAY_BASE + 0x40
            && span.end == OVERLAY_BASE + 0x48));
    }

    #[test]
    fn complete_stock_call_via_bank_is_executable() {
        let bank = (0u16..15)
            .flat_map(|register| [0x4700 | (register << 3), 0x46c0])
            .flat_map(u16::to_le_bytes)
            .collect::<Vec<_>>();
        let spans = compiler_idiom_spans(&bank, OVERLAY_BASE);
        assert!(spans.iter().any(|span| {
            span.start == OVERLAY_BASE
                && span.end == OVERLAY_BASE + 60
                && span.kind == "compiler_runtime"
        }));
        assert!(compiler_idiom_spans(&bank[..56], OVERLAY_BASE).is_empty());
    }

    #[test]
    fn isolated_prologue_requires_adjacent_classified_code() {
        let image = [0xe0, 0xb5, 0x00, 0x20, 0x00, 0xbd, 0xe0, 0xb5];
        let classified = [ExecutableSpan {
            start: OVERLAY_BASE + 2,
            end: OVERLAY_BASE + 6,
            kind: "thumb",
        }];
        assert_eq!(
            adjacent_prologue_spans(&image, OVERLAY_BASE, &classified),
            [ExecutableSpan {
                start: OVERLAY_BASE,
                end: OVERLAY_BASE + 2,
                kind: "thumb",
            }]
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
fn reachable(
    input: &[u8],
    base: i64,
    seeds: &[i64],
    discover_prologues: bool,
    follow_calls: bool,
    sweep: bool,
    follow_switches: bool,
) -> Reachability {
    let length = input.len() as i64;
    let read_u16 = |offset: i64| -> i64 {
        let at = offset as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let inside = |address: i64, size: i64| base <= address && address + size <= base + length;
    let sign_extend = |value: i64, bits: u32| (value << (64 - bits)) >> (64 - bits);
    let mut queue: Vec<i64> = seeds.iter().copied().filter(|s| inside(*s, 2)).collect();
    if discover_prologues {
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
    }
    let mut offset = 0i64;
    while offset < length - 8 {
        let word = read_u16(offset);
        let following = read_u16(offset + 2);
        let fixed_r0 = word == 0x4800 && following == 0x4700;
        let fixed_r4 = word == 0x4c00 && following == 0x4720;
        if fixed_r0 || fixed_r4 {
            let at = (offset + 4) as usize;
            let target = input[at] as i64
                | ((input[at + 1] as i64) << 8)
                | ((input[at + 2] as i64) << 16)
                | ((input[at + 3] as i64) << 24);
            let mut target = target & !1;
            if base == OVERLAY_BASE && !inside(target, 2) && inside(target - 0x8000, 2) {
                target -= 0x8000;
            }
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
                if half & 0xff87 == 0x4687 && follow_switches && pc >= base + 6 {
                    // GCC's Thumb switch tail is an exact register chain:
                    //
                    //   cmp  index, #last
                    //   ldr  table, [pc, #pool]
                    //   lsl  index, index, #2
                    //   ldr  target, [table, index]
                    //   mov  pc, target
                    //
                    // Requiring those data-flow relationships matters: a
                    // loose nearby-opcode search can turn scene tables into
                    // thousands of false instructions.
                    let preceding: Vec<(i64, i64)> = (1..=12)
                        .map(|k| pc - 2 * k)
                        .filter(|at| inside(*at, 2))
                        .map(|at| (at, read_u16(at - base)))
                        .collect();
                    let load_target = read_u16(pc - 2 - base);
                    let scale_index = read_u16(pc - 4 - base);
                    let load_table = read_u16(pc - 6 - base);
                    let target_register = (half >> 3) & 0xf;
                    let target_load_register = load_target & 7;
                    let first_address_register = (load_target >> 3) & 7;
                    let second_address_register = (load_target >> 6) & 7;
                    let scaled_destination = scale_index & 7;
                    let scaled_source = (scale_index >> 3) & 7;
                    let scaled_amount = (scale_index >> 6) & 0x1f;
                    let loaded_table_register = (load_table >> 8) & 7;
                    let exact_tail = load_target & 0xfe00 == 0x5800
                        && scale_index & 0xf800 == 0
                        && scaled_amount == 2
                        && load_table & 0xf800 == 0x4800
                        && target_load_register == target_register
                        && scaled_destination != loaded_table_register
                        && ((first_address_register == loaded_table_register
                            && second_address_register == scaled_destination)
                            || (second_address_register == loaded_table_register
                                && first_address_register == scaled_destination));
                    let limit = preceding
                        .iter()
                        .find(|(_, h)| h & 0xf800 == 0x2800 && ((h >> 8) & 7) == scaled_source)
                        .map(|(_, h)| (h & 0xff) + 1);
                    let table = exact_tail
                        .then(|| {
                            let at = pc - 6;
                            let word = ((at + 4) & !3) + ((load_table & 0xff) << 2);
                            inside(word, 4).then(|| {
                                let table = read_u32(word - base);
                                if base == OVERLAY_BASE && inside(table - 0x8000, 4) {
                                    table - 0x8000
                                } else {
                                    table
                                }
                            })
                        })
                        .flatten();
                    if let (Some(count), Some(table)) = (limit, table) {
                        if table % 4 == 0 && inside(table, count * 4) {
                            let targets = (0..count)
                                .map(|entry| {
                                    let mut target = read_u32(table + entry * 4 - base) & !1;
                                    if base == OVERLAY_BASE && inside(target - 0x8000, 2) {
                                        target -= 0x8000;
                                    }
                                    target
                                })
                                .collect::<Vec<_>>();
                            let plausible_block = |target: i64| {
                                if !inside(target, 8) {
                                    return false;
                                }
                                let half = |index: i64| read_u16(target + index * 2 - base);
                                let high_words = [half(1), half(3)];
                                let address_words = high_words
                                    .iter()
                                    .all(|high| matches!(*high, 0x0200 | 0x0300 | 0x0800));
                                let small_value_words =
                                    high_words == [0, 0] && half(0) < 0x1000 && half(2) < 0x1000;
                                half(0) != 0 && !address_words && !small_value_words
                            };
                            if targets.iter().all(|target| plausible_block(*target)) {
                                for entry in 0..count {
                                    let at = table + entry * 4;
                                    for byte in at..at + 4 {
                                        tables.insert(byte);
                                    }
                                }
                                queue.extend(targets);
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
                        let target = if base == OVERLAY_BASE {
                            // Stored overlays carry the loader's pre-relocation
                            // displacement: destination = resource base + the
                            // encoded value + the Thumb bias. The loader later
                            // rewrites it for the 0x02008000 runtime address.
                            base + displacement + 2
                        } else {
                            pc + 4 + displacement
                        };
                        // A whole overlay's functions are seeded by their
                        // prologues; a call names its target without walking
                        // it, so veneers and data a call lands on stay data.
                        if inside(target, 2) && follow_calls {
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
    Reachability {
        instructions,
        tables,
    }
}

/// Conservatively inventory the executable bytes proved by the same decoder
/// that emits a byte-identical overlay listing. Instructions, their referenced
/// literal words, fixed loader veneers, and two-byte alignment holes between
/// executable spans are counted; every other byte remains data.
pub fn executable_spans(input: &[u8], base: i64) -> Result<Vec<ExecutableSpan>, String> {
    if !input.len().is_multiple_of(2) {
        return Err("overlay has an odd byte length".into());
    }
    let mut pointers = Vec::new();
    for offset in (0..input.len().saturating_sub(3)).step_by(4) {
        let word = u32::from_le_bytes(input[offset..offset + 4].try_into().unwrap());
        if word & 1 == 0 {
            continue;
        }
        let target = i64::from(word & !1);
        if base <= target && target + 2 <= base + input.len() as i64 {
            pointers.push(target);
        }
    }
    let instructions = reachable(input, base, &pointers, true, true, false, false).instructions;
    let read_u16 = |address: i64| {
        let at = (address - base) as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let mut spans = Vec::new();
    for (&start, &size) in &instructions {
        spans.push(ExecutableSpan {
            start,
            end: start + size,
            kind: "thumb",
        });
        if size == 2 {
            let half = read_u16(start);
            if half & 0xf800 == 0x4800 {
                let word = ((start + 4) & !3) + ((half & 0xff) << 2);
                if base <= word && word + 4 <= base + input.len() as i64 {
                    spans.push(ExecutableSpan {
                        start: word,
                        end: word + 4,
                        kind: "literal_pool",
                    });
                }
            }
        }
    }
    for offset in (0..input.len().saturating_sub(7)).step_by(4) {
        if input[offset..offset + 4] != [0x00, 0x4c, 0x20, 0x47] {
            continue;
        }
        let target = u32::from_le_bytes(input[offset + 4..offset + 8].try_into().unwrap());
        if matches!(target >> 24, 0x02 | 0x03 | 0x08 | 0x09) {
            spans.push(ExecutableSpan {
                start: base + offset as i64,
                end: base + offset as i64 + 8,
                kind: "veneer",
            });
        }
    }
    spans.sort_by_key(|span| (span.start, span.end));
    let mut merged: Vec<ExecutableSpan> = Vec::new();
    for span in spans {
        if let Some(last) = merged.last_mut() {
            if span.start <= last.end && span.kind == last.kind {
                last.end = last.end.max(span.end);
                continue;
            }
        }
        merged.push(span);
    }
    let mut union: Vec<(i64, i64)> = Vec::new();
    for span in &merged {
        if let Some(last) = union.last_mut() {
            if span.start <= last.1 {
                last.1 = last.1.max(span.end);
                continue;
            }
        }
        union.push((span.start, span.end));
    }
    for pair in union.windows(2) {
        if pair[1].0 - pair[0].1 == 2 && read_u16(pair[0].1) == 0 {
            merged.push(ExecutableSpan {
                start: pair[0].1,
                end: pair[1].0,
                kind: "executable_alignment",
            });
        }
    }
    merged.sort_by_key(|span| (span.start, span.end));
    Ok(merged)
}

/// Code reached from the loader's opening veneers, calls, branches and
/// compiler switch tables. It deliberately does not seed prologue-shaped
/// bytes, so it can safely complement the conservative prologue decoder.
pub fn trusted_overlay_spans(
    input: &[u8],
    base: i64,
    entry_veneers: usize,
) -> Result<Vec<ExecutableSpan>, String> {
    overlay_flow_spans(input, base, entry_veneers, false)
}

/// Executable-image accounting includes compiler-owned jump tables, just as
/// it includes literal pools and a matched function's complete linked extent.
pub fn overlay_flow_spans(
    input: &[u8],
    base: i64,
    entry_veneers: usize,
    include_tables: bool,
) -> Result<Vec<ExecutableSpan>, String> {
    if input.len() < entry_veneers * 8 {
        return Err("overlay is shorter than its entry veneer table".into());
    }
    let mut seeds = Vec::with_capacity(entry_veneers + 1);
    // Camelot overlays place their first local routine immediately after the
    // fixed loader veneer table. It is not necessarily named by an entry
    // veneer (several tables point only at later public entry points), and it
    // is often an unframed leaf, so neither ordinary discovery route sees it.
    seeds.push(base + (entry_veneers * 8) as i64);
    for entry in 0..entry_veneers {
        let at = entry * 8;
        if input[at..at + 4] != [0x00, 0x4c, 0x20, 0x47] {
            return Err(format!("overlay entry {entry} is not a fixed veneer"));
        }
        let raw = u32::from_le_bytes(input[at + 4..at + 8].try_into().unwrap());
        let mut target = i64::from(raw & !1);
        if base == OVERLAY_BASE {
            target -= 0x8000;
        }
        if target < base || target + 2 > base + input.len() as i64 {
            return Err(format!("overlay entry {entry} target is outside the image"));
        }
        seeds.push(target);
    }
    let flow = reachable(input, base, &seeds, false, true, false, true);
    let mut spans = spans_from_instructions(input, base, flow.instructions)?;
    if include_tables {
        let mut tables: Vec<ExecutableSpan> = Vec::new();
        for address in flow.tables {
            if let Some(last) = tables.last_mut() {
                if last.end == address {
                    last.end += 1;
                    continue;
                }
            }
            tables.push(ExecutableSpan {
                start: address,
                end: address + 1,
                kind: "jump_table",
            });
        }
        for table in &tables {
            // The compiler aligns an inline table after mov pc, rN.
            if table.start >= base + 4 {
                let offset = (table.start - base) as usize;
                let half = |at: usize| u16::from_le_bytes([input[at], input[at + 1]]);
                if half(offset - 2) == 0 && half(offset - 4) & 0xff87 == 0x4687 {
                    spans.push(ExecutableSpan {
                        start: table.start - 2,
                        end: table.start,
                        kind: "executable_alignment",
                    });
                }
            }
        }
        spans.extend(tables);
    }
    Ok(spans)
}

/// Candidate main-image inventory seeded only by direct call destinations
/// that have a framed Thumb prologue. Unlike an overlay-wide prologue sweep,
/// random `push`-shaped data cannot seed itself. Function-pointer-only entry
/// points remain intentionally missing and are measured by TBS calibration.
pub fn main_executable_spans(input: &[u8], base: i64) -> Result<Vec<ExecutableSpan>, String> {
    if !input.len().is_multiple_of(2) {
        return Err("main image has an odd byte length".into());
    }
    let half = |offset: usize| u16::from_le_bytes([input[offset], input[offset + 1]]);
    let mut seeds = BTreeSet::new();
    for offset in (0..input.len().saturating_sub(3)).step_by(2) {
        let high = i64::from(half(offset));
        let low = i64::from(half(offset + 2));
        if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
            continue;
        }
        let displacement = (((high & 0x7ff) << 12) | ((low & 0x7ff) << 1)) << 41 >> 41;
        let target = base + offset as i64 + 4 + displacement;
        let relative = target - base;
        if relative >= 0
            && relative + 2 <= input.len() as i64
            && i64::from(half(relative as usize)) & 0xff00 == 0xb500
        {
            seeds.insert(target);
        }
    }
    // Static callback tables are the other ordinary way into a function.
    // Require a word-aligned in-image Thumb pointer whose destination begins
    // with a framed prologue; neither condition alone is enough to seed code.
    for offset in (0..input.len().saturating_sub(3)).step_by(4) {
        let word = u32::from_le_bytes(input[offset..offset + 4].try_into().unwrap());
        if word & 1 == 0 {
            continue;
        }
        let target = i64::from(word & !1);
        let relative = target - base;
        if relative >= 0
            && relative + 2 <= input.len() as i64
            && i64::from(half(relative as usize)) & 0xff00 == 0xb500
        {
            seeds.insert(target);
        }
    }
    let instructions = reachable(
        input,
        base,
        &seeds.into_iter().collect::<Vec<_>>(),
        false,
        true,
        false,
        false,
    );
    spans_from_instructions(input, base, instructions.instructions)
}

fn spans_from_instructions(
    input: &[u8],
    base: i64,
    instructions: BTreeMap<i64, i64>,
) -> Result<Vec<ExecutableSpan>, String> {
    let read_u16 = |address: i64| {
        let at = (address - base) as usize;
        input[at] as i64 | ((input[at + 1] as i64) << 8)
    };
    let mut spans = Vec::new();
    for (&start, &size) in &instructions {
        spans.push(ExecutableSpan {
            start,
            end: start + size,
            kind: "thumb",
        });
        if size == 2 {
            let half = read_u16(start);
            if half & 0xf800 == 0x4800 {
                let word = ((start + 4) & !3) + ((half & 0xff) << 2);
                if base <= word && word + 4 <= base + input.len() as i64 {
                    spans.push(ExecutableSpan {
                        start: word,
                        end: word + 4,
                        kind: "literal_pool",
                    });
                }
            }
        }
    }
    let mut union = spans
        .iter()
        .map(|span| (span.start, span.end))
        .collect::<Vec<_>>();
    union.sort_unstable();
    let mut merged: Vec<(i64, i64)> = Vec::new();
    for span in union {
        if let Some(last) = merged.last_mut() {
            if span.0 <= last.1 {
                last.1 = last.1.max(span.1);
                continue;
            }
        }
        merged.push(span);
    }
    for pair in merged.windows(2) {
        if pair[1].0 - pair[0].1 == 2 && read_u16(pair[0].1) == 0 {
            spans.push(ExecutableSpan {
                start: pair[0].1,
                end: pair[1].0,
                kind: "executable_alignment",
            });
        }
    }
    Ok(spans)
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
    let instructions = reachable(decoded, base, seeds, true, sweep, sweep, sweep).instructions;
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
    loop {
        let hex = |line: Option<&&str>, prefix: &str, width: usize| {
            line.and_then(|line| line.strip_prefix(prefix))
                .filter(|digits| digits.len() == width)
                .and_then(|digits| u32::from_str_radix(digits, 16).ok())
        };
        let encoded = lines.get(at) == Some(&"\t.4byte 0x47204c00");
        let decoded = lines
            .get(at)
            .is_some_and(|line| line.starts_with("\tldr\tr4, [pc, #0]"))
            && lines.get(at + 1) == Some(&"\tbx\tr4");
        let (target, used) = match (encoded, decoded) {
            (true, _) => match hex(lines.get(at + 1), "\t.4byte 0x", 8) {
                Some(target) => (target, 2),
                None => match (
                    hex(lines.get(at + 1), "\t.2byte 0x", 4),
                    hex(lines.get(at + 2), "\t.2byte 0x", 4),
                ) {
                    (Some(low), Some(high)) => (low | high << 16, 3),
                    _ => break,
                },
            },
            (false, true) => match hex(lines.get(at + 2), "\t.4byte 0x", 8) {
                Some(target) => (target, 3),
                None => break,
            },
            _ => break,
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
            "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC",
            2,
        )
        .unwrap();
        assert!(text.contains("\t.irp EntryTarget, 0x02008011, 0x02008015\n"));
        assert!(text.contains("\t.irp EntryTarget, 0x080000c1\n"), "{text}");
        assert!(!text.contains("0x47204c00"));
        assert_eq!(
            crate::overlay::compile::assemble_overlay_raw(&OverlaySource::text(text), OVERLAY_BASE)
                .unwrap(),
            image
        );
        assert!(export_overlay_source(&image, OVERLAY_BASE, "OVERLAY.INC", 3).is_err());
    }
}
