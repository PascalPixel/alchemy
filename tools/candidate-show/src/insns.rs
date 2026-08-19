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
    let mut words: Vec<String> = line.split_whitespace().map(|word| word.to_string()).collect();
    if words.is_empty() {
        return String::new();
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
    words.join(" ")
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
