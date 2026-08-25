/// Read one function without letting out-of-line helpers steer its score.
pub fn gas_function_insns(source: &str, symbol: &str) -> Vec<String> {
    let label = format!("{symbol}:");
    let lines = source.lines().collect::<Vec<_>>();
    let Some(start) = lines.iter().position(|line| line.trim() == label) else {
        return gas_lines(lines);
    };
    let body = &lines[start + 1..];
    let end = body
        .iter()
        .position(|line| {
            let line = line.trim();
            line.starts_with(".size") && line.contains(symbol)
        })
        .unwrap_or(body.len());
    gas_lines(body[..end].to_vec())
}

fn gas_lines(lines: Vec<&str>) -> Vec<String> {
    lines
        .iter()
        .enumerate()
        .filter_map(|(index, line)| {
            let line = line.trim();
            if line.is_empty() || line.starts_with(['.', '@']) || line.ends_with(':') {
                return None;
            }
            let instruction = canonical(line);
            // Treat GCC `.align 2, 0` like retained `mov r0, r0` structure.
            let next_is_pool = lines[index + 1..]
                .iter()
                .map(|line| line.trim())
                .find(|line| !line.is_empty() && !line.starts_with('@'))
                .is_some_and(|line| line.starts_with(".4byte") || line.starts_with(".word"));
            (!(matches!(instruction.as_str(), "movs r0, r0" | "nop") && next_is_pool))
                .then_some(instruction)
        })
        .collect()
}
fn canonical(line: &str) -> String {
    let mut words: Vec<_> = line
        .split('@')
        .next()
        .unwrap_or(line)
        .split_whitespace()
        .map(str::to_string)
        .collect();
    if words.is_empty() {
        return String::new();
    }
    if let Some(word) = words[0].strip_suffix(".n") {
        words[0] = word.into();
    }
    let ops = [
        ("mov", "movs"),
        ("add", "adds"),
        ("sub", "subs"),
        ("lsl", "lsls"),
        ("lsr", "lsrs"),
        ("asr", "asrs"),
        ("ror", "rors"),
        ("neg", "negs"),
        ("mul", "muls"),
        ("and", "ands"),
        ("orr", "orrs"),
        ("eor", "eors"),
        ("bic", "bics"),
        ("mvn", "mvns"),
        ("cmp", "cmps"),
        ("cmn", "cmns"),
        ("tst", "tsts"),
    ];
    if let Some((_, replacement)) = ops.iter().find(|(name, _)| words[0] == *name) {
        words[0] = (*replacement).into();
    }
    if words.len() == 4
        && matches!(words[0].as_str(), "adds" | "subs")
        && words[1].trim_end_matches(',') == words[2].trim_end_matches(',')
    {
        words.remove(2);
    }
    if words.len() == 3
        && words[0] == "movs"
        && lo(words[1].trim_end_matches(','))
        && lo(&words[2])
        && words[1].trim_end_matches(',') != words[2]
    {
        return format!("adds {}, {}, #0", words[1].trim_end_matches(','), words[2]);
    }
    if words.len() == 4
        && [
            "adds", "subs", "muls", "ands", "orrs", "eors", "bics", "adcs", "sbcs", "lsls", "lsrs",
            "asrs", "rors",
        ]
        .contains(&words[0].as_str())
        && words[1].trim_end_matches(',') == words[2].trim_end_matches(',')
    {
        words.remove(2);
    }
    let mut text = words.join(" ");
    if let Some(open) = text.rfind('[') {
        if let Some(close) = text[open..].find(']') {
            let inner = &text[open + 1..open + close];
            if inner == "sp"
                || inner.starts_with('r') && inner[1..].chars().all(|c| c.is_ascii_digit())
            {
                text = format!(
                    "{}[{}, #0]{}",
                    &text[..open],
                    inner,
                    &text[open + close + 1..]
                );
            }
        }
    }
    if let Some(pc) = text.find("[pc,") {
        if let Some(close) = text[pc..].find(']') {
            text.replace_range(pc..pc + close + 1, "<pool>");
        }
    }
    while let Some(at) = text.find(".L") {
        let end = text[at..]
            .find(|c: char| c.is_whitespace() || c == ',')
            .map_or(text.len(), |n| at + n);
        text.replace_range(at..end, "<pool>");
    }
    text
}
fn lo(reg: &str) -> bool {
    reg.len() == 2
        && reg.starts_with('r')
        && reg.as_bytes()[1].is_ascii_digit()
        && reg.as_bytes()[1] <= b'7'
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn scopes_owner_and_ignores_pool_alignment_spelling() {
        let source = r#"
Func_08000000:
        mov r1, r2
        movs r0, r0
        .4byte 0x12345678
        bx lr
        .size Func_08000000,.-Func_08000000
Helper:
        bx lr
"#;
        assert_eq!(
            gas_function_insns(source, "Func_08000000"),
            ["adds r1, r2, #0", "bx lr"]
        );
    }

    #[test]
    fn falls_back_for_unsymbolized_snippets() {
        assert_eq!(
            gas_function_insns("mov r0, r1\n", "missing"),
            ["adds r0, r1, #0"]
        );
    }
}
