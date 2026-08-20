pub fn gas_insns(source: &str) -> Vec<String> {
    source
        .lines()
        .filter_map(|line| {
            let line = line.trim();
            (!line.is_empty() && !line.starts_with(['.', '@']) && !line.ends_with(':'))
                .then(|| canonical(line))
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
