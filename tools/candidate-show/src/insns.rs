//! Turn a GAS listing into one insn per line so `git diff` is the comparison.
//!
//! gcc 2.96 `-S` and the reconstructed `asm/<addr>.s` do not share a dialect
//! (`mov` vs `movs`, `sub sp, sp, #N` vs `sub sp, #N`). The rewrite below is
//! only enough for `git diff` to see missing or extra instructions instead of
//! every line as a spelling difference.

/// Instruction lines of a `.s` file, directives and labels dropped.
pub fn gas_insns(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| {
            let trimmed = line.trim();
            if trimmed.is_empty()
                || trimmed.starts_with('.')
                || trimmed.starts_with('@')
                || trimmed.ends_with(':')
            {
                return None;
            }
            Some(canonical_insn(trimmed))
        })
        .collect()
}

fn canonical_insn(line: &str) -> String {
    // Trailing comments are annotation, not instruction.
    let line = line.split('@').next().unwrap_or(line).trim_end();
    let mut words: Vec<String> = line.split_whitespace().map(|word| word.to_string()).collect();
    if words.is_empty() {
        return String::new();
    }
    // Branch width suffixes are the assembler's choice, not the source's.
    if let Some(stripped) = words[0].strip_suffix(".n") {
        words[0] = stripped.to_string();
    }
    match words[0].as_str() {
        "mov" => words[0] = "movs".to_string(),
        "add" => words[0] = "adds".to_string(),
        "sub" => words[0] = "subs".to_string(),
        "lsl" => words[0] = "lsls".to_string(),
        "lsr" => words[0] = "lsrs".to_string(),
        "asr" => words[0] = "asrs".to_string(),
        "ror" => words[0] = "rors".to_string(),
        "neg" => words[0] = "negs".to_string(),
        "mul" => words[0] = "muls".to_string(),
        "and" => words[0] = "ands".to_string(),
        "orr" => words[0] = "orrs".to_string(),
        "eor" => words[0] = "eors".to_string(),
        "bic" => words[0] = "bics".to_string(),
        "mvn" => words[0] = "mvns".to_string(),
        "cmp" => words[0] = "cmps".to_string(),
        "cmn" => words[0] = "cmns".to_string(),
        "tst" => words[0] = "tsts".to_string(),
        _ => {}
    }
    // gcc: `subs sp, sp, #100`  /  reconstructed: `subs sp, #100`
    if words.len() == 4
        && (words[0] == "subs" || words[0] == "adds")
        && words[1].trim_end_matches(',') == "sp"
        && words[2].trim_end_matches(',') == "sp"
    {
        words.remove(2);
    }
    // gcc spells a LO-to-LO register copy `mov rd, rm`; gas encodes that as
    // `adds rd, rm, #0` (0x1Cxx) in Thumb-1, which is how the reconstructed
    // `.s` spells it. Left unfolded, every copy in the function reads as a
    // diff row -- 90 false pairs on the 6 KB owner. `movs rX, rX` is the NOP
    // padding encoding and is left alone; a copy to itself is never emitted.
    if words.len() == 3 && words[0] == "movs" {
        let rd = words[1].trim_end_matches(',');
        let rm = words[2].as_str();
        let lo = |r: &str| {
            r.len() == 2
                && r.starts_with('r')
                && r.as_bytes()[1].is_ascii_digit()
                && r.as_bytes()[1] <= b'7'
        };
        if lo(rd) && lo(rm) && rd != rm {
            return format!("adds {rd}, {rm}, #0");
        }
    }
    // `op rX, rX, Y` and `op rX, Y` are one instruction for the two-operand
    // ALU forms; both dialects appear on both sides.
    const ALU: [&str; 13] = [
        "adds", "subs", "muls", "ands", "orrs", "eors", "bics", "adcs", "sbcs", "lsls", "lsrs",
        "asrs", "rors",
    ];
    if words.len() == 4
        && ALU.contains(&words[0].as_str())
        && words[1].trim_end_matches(',') == words[2].trim_end_matches(',')
    {
        words.remove(2);
    }
    let mut text = words.join(" ");
    // `[rX]` / `[sp]` and `[rX, #0]` / `[sp, #0]` are the same addressing.
    if let Some(at) = text.find(']') {
        if let Some(open) = text.rfind('[') {
            let inner = &text[open + 1..at];
            let bare_reg = inner == "sp"
                || (inner.starts_with('r') && inner[1..].chars().all(|c| c.is_ascii_digit()));
            if bare_reg {
                text = format!("{}[{inner}, #0]{}", &text[..open], &text[at + 1..]);
            }
        }
    }
    // Pool references: the reconstructed side spells them `[pc, #N]`, gcc
    // spells them `.L508` / `.L508+4`. Branch targets are `.L` labels in two
    // unrelated numberings. All become one token; position carries the rest.
    if let Some(pc) = text.find("[pc,") {
        if let Some(close) = text[pc..].find(']') {
            text = format!("{}<pool>{}", &text[..pc], &text[pc + close + 1..]);
        }
    }
    while let Some(l) = text.find(".L") {
        let end = text[l..]
            .find(|c: char| c.is_whitespace() || c == ',')
            .map(|o| l + o)
            .unwrap_or(text.len());
        text = format!("{}<pool>{}", &text[..l], &text[end..]);
    }
    text
}

#[cfg(test)]
mod tests {
    use super::{canonical_insn, gas_insns};

    #[test]
    fn drops_directives_and_labels() {
        let src = ".thumb\nFunc_080bbb0c:\n\tmov\tr0, #0\n";
        assert_eq!(gas_insns(src), vec!["movs r0, #0".to_string()]);
    }

    #[test]
    fn folds_sp_three_operand_form() {
        assert_eq!(canonical_insn("sub sp, sp, #100"), "subs sp, #100");
    }
}
