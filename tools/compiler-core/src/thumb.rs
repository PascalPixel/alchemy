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
