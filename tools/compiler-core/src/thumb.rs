pub fn bl_displacement(pair: &[u8]) -> Option<i32> {
    let high = u16::from_le_bytes(pair.get(..2)?.try_into().ok()?);
    let low = u16::from_le_bytes(pair.get(2..4)?.try_into().ok()?);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return None;
    }
    let value = (i32::from(high & 0x7ff) << 12) | (i32::from(low & 0x7ff) << 1);
    Some((value << 9) >> 9)
}
/// A Thumb relocation-bearing site: kind (`b'B'` call, `b'L'` literal load),
/// instruction offset, affected byte offset, and the referenced value.
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct Reference(pub u8, pub usize, pub usize, pub u32);
/// Mask the relocation-bearing bytes of a Thumb region that starts at the
/// absolute address `base`: `bl` halfword pairs and the literal words reached
/// by PC-relative loads. Returns the mask and the decoded reference sites.
pub fn relocation_info(bytes: &[u8], base: u64) -> (Vec<bool>, Vec<Reference>) {
    let mut mask = vec![false; bytes.len()];
    let mut references = Vec::new();
    for at in (0..bytes.len().saturating_sub(3)).step_by(2) {
        if let Some(delta) = bl_displacement(&bytes[at..at + 4]) {
            mask[at..at + 4].fill(true);
            references.push(Reference(
                b'B',
                at,
                at,
                (base as i64 + at as i64 + 4 + i64::from(delta)) as u32,
            ));
        }
    }
    for at in (0..bytes.len().saturating_sub(1)).step_by(2) {
        let instruction = u16::from_le_bytes([bytes[at], bytes[at + 1]]);
        if instruction & 0xf800 != 0x4800 {
            continue;
        }
        let pc = (base as usize + at + 4) & !3;
        let target = pc + usize::from(instruction & 0xff) * 4;
        let Some(literal) = target.checked_sub(base as usize) else {
            continue;
        };
        if literal + 4 <= mask.len() {
            mask[literal..literal + 4].fill(true);
            references.push(Reference(
                b'L',
                at,
                literal,
                u32::from_le_bytes(bytes[literal..literal + 4].try_into().unwrap()),
            ));
        }
    }
    (mask, references)
}
#[derive(Clone, Debug, PartialEq, Eq)]
struct ThumbTransfer {
    load: bool,
    base: u8,
    registers: Vec<u8>,
    targeted: bool,
}
fn low_register(text: &str) -> Option<u8> {
    let register = text.trim().strip_prefix('r')?.parse::<u8>().ok()?;
    (register <= 7).then_some(register)
}
fn thumb_transfer(line: &str) -> Option<ThumbTransfer> {
    let code = line.split('@').next().unwrap_or("").trim();
    if code.is_empty() {
        return None;
    }
    let (targeted, instruction) = match code.split_once(':') {
        Some((_, instruction)) => (true, instruction.trim()),
        None => (false, code),
    };
    let mut words = instruction.split_whitespace();
    let mnemonic = words.next()?;
    let load = match mnemonic {
        "ldmia" => true,
        "stmia" => false,
        _ => return None,
    };
    let body = instruction.strip_prefix(mnemonic)?.trim_start();
    let (operands, rest) = body.split_once('{')?;
    let (registers, _) = rest.split_once('}')?;
    let base = low_register(operands.split(',').next()?.trim().trim_end_matches('!'))?;
    let mut parsed = Vec::new();
    for item in registers.split(',') {
        let item = item.trim();
        if let Some((first, last)) = item.split_once('-') {
            let first = low_register(first)?;
            let last = low_register(last)?;
            if first > last {
                return None;
            }
            parsed.extend(first..=last);
        } else {
            parsed.push(low_register(item)?);
        }
    }
    parsed.sort_unstable();
    parsed.dedup();
    Some(ThumbTransfer {
        load,
        base,
        registers: parsed,
        targeted,
    })
}
fn approved_thumb_block_copy_pair(load: &ThumbTransfer, store: &ThumbTransfer) -> bool {
    load.load
        && !store.load
        && !load.targeted
        && !store.targeted
        && matches!(load.registers.len(), 2 | 3)
        && load.registers == store.registers
        && load.base != store.base
        && !load.registers.contains(&load.base)
        && !store.registers.contains(&store.base)
}
/// Lines containing standalone three-or-more-register Thumb loads or stores.
/// Adjacent ordinary two/three-register load/store copy pairs are excluded.
pub fn standalone_wide_transfer_lines(source: &str) -> Vec<usize> {
    let significant: Vec<_> = source
        .lines()
        .enumerate()
        .filter_map(|(index, line)| {
            let code = line.split('@').next().unwrap_or("").trim();
            (!code.is_empty()).then(|| (index + 1, thumb_transfer(line)))
        })
        .collect();
    significant
        .iter()
        .enumerate()
        .filter_map(|(index, (line, transfer))| {
            let transfer = transfer.as_ref()?;
            if transfer.registers.len() < 3 {
                return None;
            }
            let paired_as_load = significant
                .get(index + 1)
                .and_then(|(_, next)| next.as_ref())
                .is_some_and(|next| approved_thumb_block_copy_pair(transfer, next));
            let paired_as_store = index
                .checked_sub(1)
                .and_then(|previous| significant.get(previous))
                .and_then(|(_, previous)| previous.as_ref())
                .is_some_and(|previous| approved_thumb_block_copy_pair(previous, transfer));
            (!paired_as_load && !paired_as_store).then_some(*line)
        })
        .collect()
}
#[cfg(test)]
mod tests {
    use super::relocation_info;
    #[test]
    fn masks_thumb_calls_and_reached_literals() {
        let bytes = [0x00, 0xf0, 0x00, 0xf8, 0x00, 0x48, 0x70, 0x47, 1, 2, 3, 4];
        let (mask, references) = relocation_info(&bytes, 0x0200_0000);
        assert_eq!(
            mask,
            [true, true, true, true, false, false, false, false, true, true, true, true]
        );
        let core = mask.iter().filter(|masked| !**masked).count();
        assert_eq!(core, 4);
        assert_eq!(references.len(), 2);
        assert_eq!(references[0].3, 0x0200_0004);
        assert_eq!(references[1].3, 0x0403_0201);
    }
}
