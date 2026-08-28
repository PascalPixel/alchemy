use std::collections::{BTreeMap, BTreeSet};

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum Comparison {
    Equal,
    Different,
    Uncovered(String),
}

impl Comparison {
    pub fn summary(&self) -> String {
        match self {
            Self::Equal => "equal".into(),
            Self::Different => "different".into(),
            Self::Uncovered(reason) => format!("uncovered:{}", reason.replace(' ', "-")),
        }
    }
}

#[derive(Clone, Debug)]
struct Instruction {
    offset: u32,
    width: u32,
    raw: String,
}

#[derive(Clone, Debug)]
struct Word {
    offset: u32,
    expression: String,
}

#[derive(Clone, Debug)]
struct Program {
    base: u32,
    extent: u32,
    instructions: Vec<Instruction>,
    labels: BTreeMap<String, u32>,
    words: Vec<Word>,
}

pub fn compare(candidate: &str, reference: &str, symbol: &str) -> Comparison {
    compare_symbols(candidate, symbol, reference, symbol)
}

pub fn compare_symbols(
    candidate: &str,
    candidate_symbol: &str,
    reference: &str,
    reference_symbol: &str,
) -> Comparison {
    let base = match u32::from_str_radix(candidate_symbol.trim_start_matches("Func_"), 16) {
        Ok(base) => base,
        Err(_) => return Comparison::Uncovered("candidate-non-address-owner-symbol".into()),
    };
    let candidate = match parse(candidate, candidate_symbol, base) {
        Ok(program) => program,
        Err(reason) => return Comparison::Uncovered(format!("candidate-{reason}")),
    };
    let reference = match parse(reference, reference_symbol, base) {
        Ok(program) => program,
        Err(reason) => return Comparison::Uncovered(format!("reference-{reason}")),
    };
    let left = match candidate.signature() {
        Ok(signature) => signature,
        Err(reason) => return Comparison::Uncovered(format!("candidate-{reason}")),
    };
    let right = match reference.signature() {
        Ok(signature) => signature,
        Err(reason) => return Comparison::Uncovered(format!("reference-{reason}")),
    };
    if left == right {
        Comparison::Equal
    } else {
        Comparison::Different
    }
}

#[derive(Clone, Debug)]
enum Destination {
    Instruction(usize),
    Exit,
    External(String),
}

#[derive(Clone, Debug)]
struct DirectEdge {
    source: usize,
    op: String,
    target: Destination,
}

#[derive(Clone, Debug)]
struct Table {
    dispatch: usize,
    targets: Vec<usize>,
}

impl Program {
    fn signature(&self) -> Result<Vec<String>, String> {
        let direct = self.direct_edges()?;
        let tables = self.tables()?;
        let mut boundaries = BTreeSet::from([0_usize]);
        for edge in &direct {
            if let Destination::Instruction(target) = &edge.target {
                boundaries.insert(*target);
            }
            if edge.source + 1 < self.instructions.len() {
                boundaries.insert(edge.source + 1);
            }
        }
        for table in &tables {
            boundaries.extend(table.targets.iter().copied());
        }
        let mut block_of = vec![0; self.instructions.len()];
        let mut block = 0;
        for index in 0..self.instructions.len() {
            if index != 0 && boundaries.contains(&index) {
                block += 1;
            }
            block_of[index] = block;
        }
        let block_count = block + 1;
        let mut edges = vec![format!("blocks:{block_count}")];
        let mut terminated = BTreeSet::new();
        for edge in &direct {
            let source = block_of[edge.source];
            let target = destination(&edge.target, &block_of);
            edges.push(format!("block:{source}:{}->{target}", edge.op));
            terminated.insert(source);
            if edge.op != "b" && edge.op != "bal" && edge.source + 1 < block_of.len() {
                edges.push(format!(
                    "block:{source}:fallthrough->block:{}",
                    block_of[edge.source + 1]
                ));
            }
        }
        for table in &tables {
            let source = block_of[table.dispatch];
            terminated.insert(source);
            for (case, target) in table.targets.iter().enumerate() {
                edges.push(format!(
                    "block:{source}:table:{case}->block:{}",
                    block_of[*target]
                ));
            }
        }
        for (index, instruction) in self.instructions.iter().enumerate() {
            if return_instruction(index, &self.instructions) {
                let source = block_of[index];
                terminated.insert(source);
                edges.push(format!("block:{source}:return->exit"));
            } else if indirect_transfer(&instruction.raw)
                && !tables.iter().any(|table| table.dispatch == index)
            {
                return Err(format!("indirect-dispatch-{:#x}", instruction.offset));
            }
        }
        for source in 0..block_count.saturating_sub(1) {
            if !terminated.contains(&source) {
                edges.push(format!("block:{source}:fallthrough->block:{}", source + 1));
            }
        }
        edges.sort();
        edges.dedup();
        Ok(edges)
    }

    fn direct_edges(&self) -> Result<Vec<DirectEdge>, String> {
        let mut edges = Vec::new();
        for (index, instruction) in self.instructions.iter().enumerate() {
            let op = mnemonic(&instruction.raw);
            if !direct_branch(op) {
                continue;
            }
            let target = instruction
                .raw
                .split_ascii_whitespace()
                .nth(1)
                .ok_or_else(|| format!("branch-without-target-{:#x}", instruction.offset))?;
            let target = self.branch_target(target)?;
            if matches!(op, "b" | "bal") && self.pool_island_skip(index, &target) {
                continue;
            }
            edges.push(DirectEdge {
                source: index,
                op: op.to_string(),
                target,
            });
        }
        Ok(edges)
    }

    fn pool_island_skip(&self, source: usize, target: &Destination) -> bool {
        let Destination::Instruction(target) = target else {
            return false;
        };
        let start = self.instructions[source].offset + self.instructions[source].width;
        let end = self.instructions[*target].offset;
        start < end
            && self
                .words
                .iter()
                .any(|word| (start..end).contains(&word.offset))
            && !self
                .instructions
                .iter()
                .any(|instruction| (start..end).contains(&instruction.offset))
    }

    fn branch_target(&self, target: &str) -> Result<Destination, String> {
        let target = target.trim_matches(',');
        if let Some(offset) = self.local_expression(target) {
            if offset == self.extent {
                return Ok(Destination::Exit);
            }
            let index = self
                .instructions
                .iter()
                .position(|instruction| instruction.offset == offset)
                .ok_or_else(|| format!("branch-target-not-code-{offset:#x}"))?;
            return Ok(Destination::Instruction(index));
        }
        if target.starts_with('.') {
            return Err(format!("unresolved-local-branch-{target}"));
        }
        Ok(Destination::External(target.into()))
    }

    fn tables(&self) -> Result<Vec<Table>, String> {
        let mut used_dispatches = BTreeSet::new();
        let mut tables = Vec::new();
        let mut index = 0;
        while index < self.words.len() {
            let start = index;
            let mut targets = Vec::new();
            while index < self.words.len()
                && (index == start || self.words[index].offset == self.words[index - 1].offset + 4)
            {
                let Some(target) = self.word_code_target(&self.words[index]) else {
                    break;
                };
                targets.push(target);
                index += 1;
            }
            if targets.len() >= 2 {
                let table_offset = self.words[start].offset;
                if let Some(dispatch) = self.dispatch_before(table_offset) {
                    used_dispatches.insert(dispatch);
                    tables.push(Table { dispatch, targets });
                }
            }
            if index == start {
                index += 1;
            }
        }
        let uncovered = self
            .instructions
            .iter()
            .enumerate()
            .find(|(index, instruction)| {
                computed_dispatch(&instruction.raw)
                    && !return_instruction(*index, &self.instructions)
                    && !used_dispatches.contains(index)
            });
        if let Some((_, instruction)) = uncovered {
            return Err(format!("indirect-dispatch-{:#x}", instruction.offset));
        }
        Ok(tables)
    }

    fn dispatch_before(&self, table_offset: u32) -> Option<usize> {
        let mut index = self
            .instructions
            .iter()
            .rposition(|instruction| instruction.offset + instruction.width <= table_offset)?;
        while alignment_nop(&self.instructions[index].raw) {
            index = index.checked_sub(1)?;
        }
        let instruction = &self.instructions[index];
        (computed_dispatch(&instruction.raw)
            && table_offset - instruction.offset - instruction.width <= 3)
            .then_some(index)
    }

    fn word_code_target(&self, word: &Word) -> Option<usize> {
        let offset = self.local_expression(&word.expression)?;
        self.instructions
            .iter()
            .position(|instruction| instruction.offset == offset)
    }

    fn local_expression(&self, expression: &str) -> Option<u32> {
        let expression = expression.replace(' ', "");
        let (atom, addend) = split_addend(&expression);
        let value = self.labels.get(atom).copied().or_else(|| number(atom))?;
        let value = i64::from(value).checked_add(addend)?;
        let value = u32::try_from(value).ok()?;
        if self.labels.contains_key(atom) {
            return Some(value);
        }
        if (self.base..=self.base.checked_add(self.extent)?).contains(&value) {
            return value.checked_sub(self.base);
        }
        (value <= self.extent).then_some(value)
    }
}

fn parse(source: &str, symbol: &str, base: u32) -> Result<Program, String> {
    let lines = source.lines().collect::<Vec<_>>();
    let global_entries = lines
        .iter()
        .filter_map(|raw| {
            let line = raw.split('@').next().unwrap_or(raw).trim();
            line.strip_prefix(".global")
                .or_else(|| line.strip_prefix(".globl"))
        })
        .flat_map(|names| names.split(',').map(str::trim))
        .filter(|name| !name.is_empty())
        .collect::<BTreeSet<_>>();
    let label = format!("{symbol}:");
    let start = lines
        .iter()
        .position(|line| line.trim() == label)
        .ok_or_else(|| "missing-owner-label".to_string())?;
    let mut thumb = None;
    for raw in &lines[..start] {
        let line = raw.split('@').next().unwrap_or(raw).trim();
        let mut fields = line.split_ascii_whitespace();
        match (fields.next(), fields.next()) {
            (Some(".arm"), _) | (Some(".code"), Some("32")) => thumb = Some(false),
            (Some(".thumb"), _) | (Some(".code"), Some("16")) => thumb = Some(true),
            _ => {}
        }
    }
    if thumb == Some(false) {
        return Err("arm-mode".into());
    }
    let mut offset = 0_u32;
    let mut instructions = Vec::new();
    let mut labels = BTreeMap::new();
    let mut words = Vec::new();
    let body = &lines[start + 1..];
    for (index, raw) in body.iter().enumerate() {
        let line = raw.split('@').next().unwrap_or(raw).trim();
        if line.is_empty() {
            continue;
        }
        if line.starts_with(".size") && line.contains(symbol) {
            break;
        }
        if let Some(name) = line.strip_suffix(':') {
            if offset != 0 && name.trim() != symbol && global_entries.contains(name.trim()) {
                return Err("multiple-owner-entries".into());
            }
            labels.insert(name.trim().to_string(), offset);
            continue;
        }
        if line.starts_with('.') {
            directive(line, &mut offset, &mut words)?;
            continue;
        }
        let width = instruction_width(line)?;
        if offset % 4 == 2 && alignment_nop(line) && next_is_pool_data(body, index + 1) {
            offset = offset
                .checked_add(width)
                .ok_or_else(|| "owner-extent-overflow".to_string())?;
            continue;
        }
        instructions.push(Instruction {
            offset,
            width,
            raw: line.to_string(),
        });
        offset = offset
            .checked_add(width)
            .ok_or_else(|| "owner-extent-overflow".to_string())?;
    }
    if instructions.is_empty() {
        return Err("empty-owner".into());
    }
    Ok(Program {
        base,
        extent: offset,
        instructions,
        labels,
        words,
    })
}

fn next_is_pool_data(lines: &[&str], start: usize) -> bool {
    for raw in &lines[start..] {
        let line = raw.split('@').next().unwrap_or(raw).trim();
        if line.is_empty() {
            continue;
        }
        return line.starts_with(".word") || line.starts_with(".4byte");
    }
    false
}

fn directive(line: &str, offset: &mut u32, words: &mut Vec<Word>) -> Result<(), String> {
    let (name, arguments) = line
        .split_once(char::is_whitespace)
        .map_or((line, ""), |(name, arguments)| (name, arguments.trim()));
    match name {
        ".align" | ".p2align" => align(offset, arguments, true),
        ".balign" => align(offset, arguments, false),
        ".word" | ".4byte" => {
            for expression in values(arguments)? {
                words.push(Word {
                    offset: *offset,
                    expression: expression.to_string(),
                });
                *offset = offset
                    .checked_add(4)
                    .ok_or_else(|| "owner-extent-overflow".to_string())?;
            }
            Ok(())
        }
        ".short" | ".hword" | ".2byte" => advance(offset, values(arguments)?.len(), 2),
        ".byte" => advance(offset, values(arguments)?.len(), 1),
        ".space" | ".skip" | ".zero" => {
            let count = arguments
                .split(',')
                .next()
                .and_then(number)
                .ok_or_else(|| format!("unsupported-{name}"))?;
            *offset = offset
                .checked_add(count)
                .ok_or_else(|| "owner-extent-overflow".to_string())?;
            Ok(())
        }
        ".arm" => Err("arm-mode".into()),
        ".code" if arguments.split(',').next().unwrap_or("").trim() != "16" => {
            Err("arm-mode".into())
        }
        ".syntax" | ".thumb" | ".code" | ".text" | ".global" | ".globl" | ".thumb_func"
        | ".type" | ".file" | ".ident" | ".section" | ".set" | ".equ" | ".extern" | ".weak"
        | ".hidden" | ".fnstart" | ".fnend" | ".cantunwind" => Ok(()),
        name if name.starts_with(".cfi_") || name.starts_with(".loc") => Ok(()),
        _ => Err(format!("unsupported-directive-{name}")),
    }
}

fn values(arguments: &str) -> Result<Vec<&str>, String> {
    let values = arguments
        .split(',')
        .map(str::trim)
        .filter(|value| !value.is_empty())
        .collect::<Vec<_>>();
    if values.is_empty() {
        Err("empty-data-directive".into())
    } else {
        Ok(values)
    }
}

fn advance(offset: &mut u32, count: usize, width: u32) -> Result<(), String> {
    let bytes = u32::try_from(count)
        .ok()
        .and_then(|count| count.checked_mul(width))
        .ok_or_else(|| "owner-extent-overflow".to_string())?;
    *offset = offset
        .checked_add(bytes)
        .ok_or_else(|| "owner-extent-overflow".to_string())?;
    Ok(())
}

fn align(offset: &mut u32, arguments: &str, power_of_two: bool) -> Result<(), String> {
    let value = arguments
        .split(',')
        .next()
        .and_then(number)
        .ok_or_else(|| "unsupported-alignment".to_string())?;
    let boundary = if power_of_two {
        1_u32
            .checked_shl(value)
            .ok_or_else(|| "unsupported-alignment".to_string())?
    } else {
        value
    };
    if boundary == 0 || !boundary.is_power_of_two() {
        return Err("unsupported-alignment".into());
    }
    *offset = offset
        .checked_add(boundary - 1)
        .map(|value| value & !(boundary - 1))
        .ok_or_else(|| "owner-extent-overflow".to_string())?;
    Ok(())
}

fn instruction_width(line: &str) -> Result<u32, String> {
    let raw = line.split_ascii_whitespace().next().unwrap_or("");
    let op = mnemonic(line);
    if raw.is_empty() {
        return Err("empty-instruction".into());
    }
    Ok(match op {
        "bl" => 4,
        "blx" if line.split_ascii_whitespace().nth(1).is_some_and(register) => 2,
        "blx" => 4,
        _ if raw.ends_with(".w") => 4,
        _ => 2,
    })
}

fn mnemonic(line: &str) -> &str {
    line.split_ascii_whitespace()
        .next()
        .unwrap_or("")
        .trim_end_matches(".n")
        .trim_end_matches(".w")
}

fn direct_branch(op: &str) -> bool {
    matches!(
        op,
        "b" | "bal"
            | "beq"
            | "bne"
            | "bcs"
            | "bhs"
            | "bcc"
            | "blo"
            | "bmi"
            | "bpl"
            | "bvs"
            | "bvc"
            | "bhi"
            | "bls"
            | "bge"
            | "blt"
            | "bgt"
            | "ble"
    )
}

fn computed_dispatch(line: &str) -> bool {
    matches!(mnemonic(line), "mov" | "movs")
        && line
            .split_ascii_whitespace()
            .nth(1)
            .is_some_and(|operand| operand.trim_matches(',') == "pc")
}

fn alignment_nop(line: &str) -> bool {
    mnemonic(line) == "nop"
        || matches!(mnemonic(line), "mov" | "movs")
            && line
                .split_ascii_whitespace()
                .skip(1)
                .map(|operand| operand.trim_matches(','))
                .eq(["r0", "r0"])
}

fn transfer_source(line: &str) -> &str {
    line.split_ascii_whitespace()
        .nth(2)
        .unwrap_or("")
        .trim_matches(',')
}

fn return_instruction(index: usize, instructions: &[Instruction]) -> bool {
    let instruction = &instructions[index].raw;
    let op = mnemonic(instruction);
    if op == "pop" && instruction.contains("pc") {
        return true;
    }
    let source = if op == "bx" {
        instruction
            .split_ascii_whitespace()
            .nth(1)
            .unwrap_or("")
            .trim_matches(',')
    } else if computed_dispatch(instruction) {
        transfer_source(instruction)
    } else {
        return false;
    };
    source == "lr"
        || index > 0
            && mnemonic(&instructions[index - 1].raw) == "pop"
            && instructions[index - 1]
                .raw
                .split(|character: char| !character.is_ascii_alphanumeric())
                .any(|word| word == source)
}

fn indirect_transfer(line: &str) -> bool {
    computed_dispatch(line) || mnemonic(line) == "bx"
}

fn destination(target: &Destination, block_of: &[usize]) -> String {
    match target {
        Destination::Instruction(index) => format!("block:{}", block_of[*index]),
        Destination::Exit => "exit".into(),
        Destination::External(symbol) => format!("external:{symbol}"),
    }
}

fn register(value: &str) -> bool {
    let value = value.trim_matches(',');
    matches!(value, "lr" | "pc" | "sp")
        || value
            .strip_prefix('r')
            .is_some_and(|number| number.parse::<u8>().is_ok())
}

fn number(value: &str) -> Option<u32> {
    let value = value.trim().trim_start_matches('#');
    value
        .strip_prefix("0x")
        .map(|hex| u32::from_str_radix(hex, 16).ok())
        .unwrap_or_else(|| value.parse().ok())
}

fn split_addend(expression: &str) -> (&str, i64) {
    if let Some((atom, value)) = expression.rsplit_once('+') {
        if let Some(value) = number(value) {
            return (atom, i64::from(value));
        }
    }
    if let Some((atom, value)) = expression.rsplit_once('-') {
        if !atom.is_empty() {
            if let Some(value) = number(value) {
                return (atom, -i64::from(value));
            }
        }
    }
    (expression, 0)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ignores_literal_data_and_resolves_direct_targets() {
        let left = r#"
Func_08000000:
    cmp r0, #0
    beq .Ldone
    .word 0
    .word 16
    .word 0xd001e001
.Ldone:
    bx lr
    .size Func_08000000,.-Func_08000000
"#;
        let right = left.replace(".Ldone", ".Lother");
        assert_eq!(compare(left, &right, "Func_08000000"), Comparison::Equal);
        let changed = right.replace("beq", "bne");
        assert_eq!(
            compare(left, &changed, "Func_08000000"),
            Comparison::Different
        );
    }

    #[test]
    fn matches_symbolic_and_absolute_jump_tables() {
        let symbolic = r#"
Func_08000000:
    cmp r0, #1
    bhi .Lend
    lsls r0, r0, #2
    ldr r3, [r0, r1]
    mov pc, r3
    .align 2, 0
.Ltable:
    .word .Lcase0
    .word .Lcase1
.Lcase0:
    movs r0, #0
    b .Lend
.Lcase1:
    movs r0, #1
.Lend:
    bx lr
    .size Func_08000000,.-Func_08000000
"#;
        let absolute = symbolic
            .replace("    .align 2, 0", "    movs r0, r0")
            .replace(".word .Lcase0", ".4byte 0x08000014")
            .replace(".word .Lcase1", ".4byte 0x08000018");
        assert_eq!(
            compare(symbolic, &absolute, "Func_08000000"),
            Comparison::Equal
        );
    }

    #[test]
    fn uncovered_indirect_dispatch_fails_closed() {
        let source = "Func_08000000:\n    mov pc, r3\n";
        assert!(matches!(
            compare(source, source, "Func_08000000"),
            Comparison::Uncovered(_)
        ));
    }

    #[test]
    fn cd260_regression_ignores_data_that_decodes_as_thumb_branches() {
        // A 080cd260 candidate was falsely classified as structural because a raw
        // disassembly treated its jump table and literal pool as instructions.
        // Keep that parser boundary covered with hand-authored synthetic input.
        let candidate = r#"
.thumb
Func_080cd260:
    cmp r0, #1
    bhi .Ldone
    lsls r0, r0, #2
    ldr r3, [r0, r1]
    mov pc, r3
    .align 2, 0
    .word .Lcase0
    .word .Lcase1
.Lcase0:
    movs r0, #0
    b .Ldone
.Lcase1:
    movs r0, #1
.Ldone:
    bx lr
    .word 0xd001e001
    .size Func_080cd260,.-Func_080cd260
"#;
        let reference = r#"
.thumb
Func_080cd260:
    cmp r0, #1
    bhi .Ldone
    lsls r0, r0, #2
    ldr r3, [r0, r1]
    mov pc, r3
    movs r0, r0
    .4byte 0x080cd274
    .4byte 0x080cd278
.Lcase0:
    movs r0, #0
    b .Ldone
.Lcase1:
    movs r0, #1
.Ldone:
    bx lr
    .4byte 0xd101e101
    .size Func_080cd260,.-Func_080cd260
"#;
        assert_eq!(
            compare(candidate, reference, "Func_080cd260"),
            Comparison::Equal
        );
    }

    #[test]
    fn pool_alignment_fill_matches_align_but_real_nop_remains_code() {
        let fill = ".thumb\nFunc_08000000:\n    bx lr\n    movs r0, r0\n    .word 0\n";
        let align = ".thumb\nFunc_08000000:\n    bx lr\n    .align 2, 0\n    .word 0\n";
        assert_eq!(compare(fill, align, "Func_08000000"), Comparison::Equal);
        let real_nop = ".thumb\nFunc_08000000:\n    nop\n    bx lr\n    .word 0\n";
        assert_eq!(
            parse(real_nop, "Func_08000000", 0x08000000)
                .unwrap()
                .instructions
                .len(),
            2
        );
    }

    #[test]
    fn pre_label_arm_mode_fails_closed() {
        let source = ".code\t  32\nFunc_08000000:\n    bx lr\n";
        assert!(
            matches!(compare(source, source, "Func_08000000"), Comparison::Uncovered(reason) if reason.contains("arm-mode"))
        );
    }

    #[test]
    fn owner_ends_at_next_global_function_without_size() {
        let left = ".thumb\n.global Func_08000000, Func_08000002\nFunc_08000000:\n    bx lr\nFunc_08000002:\n    b Func_08000002\n";
        let right = ".thumb\n.global Func_08000000, Func_08000002\nFunc_08000000:\n    bx lr\nFunc_08000002:\n    bx lr\n";
        assert!(matches!(
            compare(left, right, "Func_08000000"),
            Comparison::Uncovered(reason) if reason == "candidate-multiple-owner-entries"
        ));
    }

    #[test]
    fn colocated_global_entry_aliases_are_one_owner() {
        let semantic =
            ".thumb\n.global NamedOwner, Func_08000000\nNamedOwner:\nFunc_08000000:\n    bx lr\n";
        let legacy = ".thumb\n.global Func_08000000\nFunc_08000000:\n    bx lr\n";
        assert_eq!(
            compare_symbols(legacy, "Func_08000000", semantic, "NamedOwner"),
            Comparison::Equal
        );
    }

    #[test]
    fn live_arm_owner_fails_closed() {
        let source = include_str!("../../../games/gs1/asm/08002d5c.s");
        assert!(matches!(
            compare(source, source, "Func_08002d5c"),
            Comparison::Uncovered(reason) if reason == "candidate-arm-mode"
        ));
    }

    #[test]
    fn live_pool_fill_matches_align_spelling() {
        let source = include_str!("../../../games/gs1/asm/080908e0.s");
        let aligned = source.replace("\tmovs\tr0, r0\n\t.4byte", "\t.align\t2, 0\n\t.4byte");
        assert_ne!(source, aligned);
        assert_eq!(
            compare(source, &aligned, "Func_080908e0"),
            Comparison::Equal
        );
    }

    #[test]
    fn live_multi_entry_reference_is_owner_scoped() {
        let source = include_str!("../../../games/gs1/asm/08002dd8.s");
        let changed_later_owner = source.replace("Func_08002df0:\n", "Func_08002df0:\n\tnop\n");
        assert!(matches!(
            compare(source, &changed_later_owner, "Func_08002dd8"),
            Comparison::Uncovered(reason) if reason == "candidate-multiple-owner-entries"
        ));
    }

    #[test]
    fn candidate_and_reference_may_use_distinct_authoritative_symbols() {
        let candidate = ".thumb\nFunc_08000000:\n    bx lr\n.size Func_08000000,.-Func_08000000\n";
        let reference = ".thumb\nNamedOwner:\n    bx lr\n.size NamedOwner,.-NamedOwner\n";
        assert_eq!(
            compare_symbols(candidate, "Func_08000000", reference, "NamedOwner"),
            Comparison::Equal
        );
    }

    #[test]
    fn live_named_reference_matches_legacy_candidate_entry() {
        let reference = include_str!("../../../games/gs1/asm/080a24d0.s");
        let candidate = reference.replace("RunAssetSelectionScreen", "Func_080a24d0");
        assert_eq!(
            compare_symbols(
                &candidate,
                "Func_080a24d0",
                reference,
                "RunAssetSelectionScreen"
            ),
            Comparison::Equal
        );
    }

    #[test]
    fn live_0800307c_pool_island_skip_is_not_a_cfg_edge() {
        let reference = include_str!("../../../games/gs1/asm/0800307c.s");
        let candidate = reference.replace(
            "\torrs\tr3, r2\n.L1:",
            "\torrs\tr3, r2\n\tb\t.Lpool_done\n.Lpool:\n\t.word\t0\n.Lpool_done:\n.L1:",
        );
        assert_ne!(candidate, reference);
        assert_eq!(
            compare(&candidate, reference, "Func_0800307c"),
            Comparison::Equal
        );
    }

    #[test]
    fn unconditional_branch_over_code_remains_structural() {
        let branch = ".thumb\nFunc_08000000:\n b .Ldone\n movs r0, #1\n.Ldone:\n bx lr\n";
        let straight = ".thumb\nFunc_08000000:\n movs r0, #1\n bx lr\n";
        assert_eq!(
            compare(branch, straight, "Func_08000000"),
            Comparison::Different
        );
    }
}
