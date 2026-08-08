// The ARM assembly renderer, ported from `armSource` and its two helpers in
// `tools/make/sentou_kouka_runtime.ts`.
//
// WHY it lives here rather than being imported: `executable_gap_sources.ts`
// imports exactly one symbol from that file, `armSource`, and nothing else in
// this crate needs the rest of the Sentou Kouka runtime tooling. Copying the
// three functions keeps this crate free of a cross-crate dependency while the
// TypeScript original stays in place.
//
// PORT NOTE: the branch pattern in this file is NOT the one in
// `executable_gap_sources.ts` -- it has no `(?:\.[nw])?` width-suffix group,
// because ARM disassembly never prints one. `js::branch_match(text, false)`
// selects this variant.

use crate::js;
use crate::toolchain;

/// `{ address, word, text }`.
pub struct InstructionRow {
    pub address: i64,
    pub word: String,
    pub text: String,
}

fn hexadecimal(value: i64) -> String {
    format!("0x{value:08x}")
}

fn local_label(address: i64) -> String {
    format!(".L_{address:08x}")
}

/// `objdumpRows(data, base)` -- the ARM (non-Thumb) form only, which is the
/// only one `armSource` calls.
fn objdump_rows(data: &[u8], base: i64) -> Result<Vec<InstructionRow>, String> {
    let listing = toolchain::objdump(data, base, false, "sentou-kouka-objdump-")?;
    let mut rows = Vec::new();
    for line in js::split_crlf_lines(&listing) {
        let Some((address, word, text)) = js::arm_objdump_row(line) else {
            continue;
        };
        let Some(address) = js::parse_int_16(address) else {
            continue;
        };
        rows.push(InstructionRow {
            address,
            word: word.to_string(),
            text: js::js_trim(js::strip_trailing_comment(text, false)).to_string(),
        });
    }
    Ok(rows)
}

/// Named addresses, kept in insertion order because `armSource` emits one
/// `.global` per entry in exactly that order.
pub type Names = Vec<(i64, String)>;

fn name_of<'a>(names: &'a Names, address: i64) -> Option<&'a str> {
    names
        .iter()
        .find(|(at, _)| *at == address)
        .map(|(_, name)| name.as_str())
}

fn render_rows(
    rows: &[InstructionRow],
    base: i64,
    end: i64,
    names: &Names,
) -> Result<Vec<String>, String> {
    // `new Map(rows.map(...))`: a later row with the same address wins.
    let by_address = |address: i64| rows.iter().rev().find(|row| row.address == address);
    let mut address = base;
    while address < end {
        if by_address(address).is_none() {
            return Err(format!("missing ARM row at {}", hexadecimal(address)));
        }
        address += 4;
    }

    let mut branch_targets: Vec<i64> = Vec::new();
    let mut literal_words: Vec<i64> = Vec::new();
    for row in rows {
        if let Some(found) = js::branch_match(&row.text, false) {
            if let Some(target) = js::parse_int_16(&found.target) {
                if base <= target && target < end && !branch_targets.contains(&target) {
                    branch_targets.push(target);
                }
            }
        }
        if let Some(displacement) = js::arm_pc_literal(&row.text) {
            let target = row.address + 8 + displacement;
            if target & 3 == 0
                && base <= target
                && target + 4 <= end
                && !literal_words.contains(&target)
            {
                literal_words.push(target);
            }
        }
    }

    let mut output = Vec::new();
    let mut address = base;
    while address < end {
        let name = name_of(names, address);
        if let Some(name) = name {
            output.push(format!("{name}:"));
        }
        // PORT NOTE: `if (name)` in JS is falsy for the empty string too, so an
        // entry mapping an address to "" would emit `:` AND still suppress the
        // local label. `symbolAddress` rejects an empty symbol, so no such
        // entry can be built; this reproduces the truthiness check anyway.
        let named = name.is_some_and(|name| !name.is_empty());
        if branch_targets.contains(&address) && !named {
            output.push(format!("{}:", local_label(address)));
        }
        let row = by_address(address).expect("checked above");
        let mut text = row.text.clone();
        let undefined_word = js::undefined_instruction_word(&text).map(str::to_string);
        if literal_words.contains(&address) {
            text = format!(".4byte 0x{}", row.word);
        } else if let Some(word) = undefined_word {
            text = format!(".4byte {word}");
        } else if let Some(word) = js::word_directive(&text) {
            text = format!(".4byte {word}");
        }
        // ARMv4T reserves the PC-writeback forms of halfword and signed loads.
        // objdump prints a mnemonic for them that GAS correctly refuses to
        // assemble, so the bounded instruction word is retained instead.
        if js::is_reserved_pc_writeback_load(&text) || js::is_reserved_tst_shift(&text) {
            text = format!(".4byte 0x{}", row.word);
        }
        text = js::replace_first_branch_with(&text, false, |whole, mnemonic, target_text| {
            let Some(target) = js::parse_int_16(target_text) else {
                return whole.to_string();
            };
            if let Some(named_target) = name_of(names, target) {
                if !named_target.is_empty() {
                    return format!("{mnemonic} {named_target}");
                }
            }
            if target < base || target >= end {
                return whole.to_string();
            }
            format!("{mnemonic} {}", local_label(target))
        });
        output.push(format!("\t{text}"));
        address += 4;
    }
    Ok(output)
}

/// `armSource(data, base, end, names)`.
pub fn arm_source(data: &[u8], base: i64, end: i64, names: &Names) -> Result<String, String> {
    let rows = objdump_rows(data, base)?;
    if rows.len() * 4 != data.len() {
        return Err("ARM fragment disassembly is incomplete".to_string());
    }
    let mut lines = vec![
        ".syntax unified".to_string(),
        ".text".to_string(),
        "\t.arm".to_string(),
    ];
    for (_, name) in names {
        lines.push(format!("\t.global {name}"));
    }
    lines.extend(render_rows(&rows, base, end, names)?);
    Ok(format!("{}\n", lines.join("\n")))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn words(values: &[u32]) -> Vec<u8> {
        values.iter().flat_map(|word| word.to_le_bytes()).collect()
    }

    #[test]
    fn a_named_arm_fragment_renders_and_reassembles() {
        // mov r0, #1 / add r0, r0, #2 / bx lr -- the same shape `selfTest` uses.
        let data = words(&[0xe3a0_0001, 0xe280_0002, 0xe12f_ff1e]);
        let names: Names = vec![(0x0800_2000, "Func_08002000".to_string())];
        let text = arm_source(&data, 0x0800_2000, 0x0800_200c, &names).unwrap();
        assert!(text.starts_with(".syntax unified\n.text\n\t.arm\n\t.global Func_08002000\n"));
        assert!(text.contains("Func_08002000:\n"));
        assert!(text.ends_with("\n"));
        let built = toolchain::assemble_overlay(&text, 0x0800_2000).unwrap();
        assert_eq!(built, data, "rendered source did not reproduce the bytes");
    }

    #[test]
    fn an_in_range_branch_becomes_a_local_label() {
        // b +4 (to 0x08002008), nop, bx lr.
        let data = words(&[0xea00_0000, 0xe1a0_0000, 0xe12f_ff1e]);
        let names: Names = vec![(0x0800_2000, "Func_08002000".to_string())];
        let text = arm_source(&data, 0x0800_2000, 0x0800_200c, &names).unwrap();
        assert!(text.contains(".L_08002008:\n"), "no local label emitted: {text}");
        assert!(text.contains("b .L_08002008"), "branch not rewritten: {text}");
        let built = toolchain::assemble_overlay(&text, 0x0800_2000).unwrap();
        assert_eq!(built, data);
    }

    #[test]
    fn a_branch_to_a_named_address_uses_the_symbol_not_a_label() {
        let data = words(&[0xea00_0000, 0xe1a0_0000, 0xe12f_ff1e]);
        let names: Names = vec![
            (0x0800_2000, "Func_08002000".to_string()),
            (0x0800_2008, "Func_08002008".to_string()),
        ];
        let text = arm_source(&data, 0x0800_2000, 0x0800_200c, &names).unwrap();
        assert!(text.contains("b Func_08002008"), "{text}");
        assert!(!text.contains(".L_08002008"), "label shadowed the symbol: {text}");
    }

    #[test]
    fn a_pc_relative_load_turns_its_target_word_into_a_literal() {
        // ldr r0, [pc, #0] reads 0x08002008; the word there must be emitted as
        // data, not disassembled.
        let data = words(&[0xe59f_0000, 0xe12f_ff1e, 0x1234_5678]);
        let names: Names = vec![(0x0800_2000, "Func_08002000".to_string())];
        let text = arm_source(&data, 0x0800_2000, 0x0800_200c, &names).unwrap();
        assert!(text.contains("\t.4byte 0x12345678\n"), "{text}");
        assert_eq!(js::count_4byte_literal_lines(&text), 1);
        let built = toolchain::assemble_overlay(&text, 0x0800_2000).unwrap();
        assert_eq!(built, data);
    }

    #[test]
    fn a_missing_row_is_reported_with_its_address() {
        // A word objdump renders as an undefined instruction still produces a
        // row; to force the error path, ask for a range wider than the data.
        let data = words(&[0xe12f_ff1e]);
        let names: Names = vec![];
        let error = arm_source(&data, 0x0800_2000, 0x0800_2008, &names).unwrap_err();
        assert_eq!(error, "missing ARM row at 0x08002004");
    }

    #[test]
    fn a_short_disassembly_is_rejected_before_rendering() {
        let data = vec![0x1e, 0xff, 0x2f];
        assert_eq!(
            arm_source(&data, 0x0800_2000, 0x0800_2004, &vec![]).unwrap_err(),
            "ARM fragment disassembly is incomplete"
        );
    }
}
