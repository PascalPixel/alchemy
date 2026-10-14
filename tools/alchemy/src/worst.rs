
use std::path::Path;

struct Signal {
    name: &'static str,
    weight: u32,
    find: fn(&str) -> usize,
}

fn count(line: &str, needle: &str) -> usize {
    line.matches(needle).count()
}

fn words_with_prefix(line: &str, prefixes: &[&str]) -> usize {
    line.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
        .filter(|word| prefixes.iter().any(|prefix| word.starts_with(prefix)))
        .count()
}

fn address_names(line: &str) -> usize {
    line.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
        .filter(|word| {
            [
                "Func_",
                "Data_",
                "Unnamed_",
                "sub_",
                "FunctionHead_",
                "Unk_",
            ]
            .iter()
            .any(|prefix| {
                word.strip_prefix(prefix).is_some_and(|rest| {
                    rest.len() >= 6 && rest.chars().all(|c| c.is_ascii_hexdigit())
                })
            })
        })
        .count()
}

fn byte_offsets(line: &str) -> usize {
    let mut found = 0;
    for cast in ["(u8 *)", "(s8 *)", "(char *)"] {
        let mut rest = line;
        while let Some(at) = rest.find(cast) {
            rest = &rest[at + cast.len()..];
            let tail: String = rest.chars().take(40).collect();
            if tail.contains(" + ") || tail.contains(")+") {
                found += 1;
            }
        }
    }
    found
}

fn deref_casts(line: &str) -> usize {
    let mut found = 0;
    for width in ["u8", "s8", "u16", "s16", "u32", "s32"] {
        found += count(line, &format!("*({width} *)"));
        found += count(line, &format!("*(volatile {width} *)"));
        found += count(line, &format!("**({width} **)"));
    }
    found
}

fn value_casts(line: &str) -> usize {
    ["(u8)", "(s8)", "(u16)", "(s16)", "(u32)", "(s32)"]
        .iter()
        .map(|cast| count(line, cast))
        .sum()
}

fn magic_numbers(line: &str) -> usize {
    let bytes = line.as_bytes();
    let mut found = 0;
    let mut i = 0;
    while i + 1 < bytes.len() {
        if bytes[i] == b'0' && (bytes[i + 1] == b'x' || bytes[i + 1] == b'X') {
            let digits = bytes[i + 2..]
                .iter()
                .take_while(|b| b.is_ascii_hexdigit())
                .count();
            if digits >= 3 {
                found += 1;
            }
            i += 2 + digits;
        } else {
            i += 1;
        }
    }
    found
}

const SIGNALS: &[Signal] = &[
    Signal {
        name: "fakematch",
        weight: 10,
        find: |l| count(l, "FAKEMATCH"),
    },
    Signal {
        name: "asm",
        weight: 6,
        find: |l| count(l, "asm(") + count(l, "asm (") + count(l, "__asm__"),
    },
    Signal {
        name: "byte_offset",
        weight: 5,
        find: byte_offsets,
    },
    Signal {
        name: "placeholder",
        weight: 3,
        find: |l| {
            words_with_prefix(l, &["unknown_", "padding", "field_", "unk_"]) + address_names(l)
        },
    },
    Signal {
        name: "deref_cast",
        weight: 3,
        find: deref_casts,
    },
    Signal {
        name: "one_pass",
        weight: 3,
        find: |l| count(l, "while (0)"),
    },
    Signal {
        name: "goto",
        weight: 2,
        find: |l| words_with_prefix(l, &["goto"]),
    },
    Signal {
        name: "volatile",
        weight: 2,
        find: |l| count(l, "volatile"),
    },
    Signal {
        name: "magic",
        weight: 0,
        find: magic_numbers,
    },
    Signal {
        name: "cast",
        weight: 1,
        find: value_casts,
    },
];

#[derive(Debug)]
pub struct Ranked {
    pub file: String,
    pub line: usize,
    pub name: String,
    pub lines: usize,
    pub score: u32,
    pub signals: Vec<(&'static str, usize)>,
}

impl Ranked {
    pub fn density(&self) -> f64 {
        f64::from(self.score) * 10.0 / self.lines.max(1) as f64
    }
}

pub(crate) fn strip_comment(line: &str, in_block: &mut bool) -> String {
    let mut out = String::new();
    let mut rest = line;
    loop {
        if *in_block {
            match rest.find("*/") {
                Some(end) => {
                    if rest[..end].contains("FAKEMATCH") {
                        out.push_str(" FAKEMATCH ");
                    }
                    rest = &rest[end + 2..];
                    *in_block = false;
                }
                None => {
                    if rest.contains("FAKEMATCH") {
                        out.push_str(" FAKEMATCH ");
                    }
                    return out;
                }
            }
        } else if let Some(start) = rest.find("/*") {
            out.push_str(&rest[..start]);
            rest = &rest[start + 2..];
            *in_block = true;
        } else {
            out.push_str(rest);
            return out;
        }
    }
}

pub(crate) fn definition_name(line: &str) -> Option<String> {
    let first = line.chars().next()?;
    if !(first.is_ascii_alphabetic() || first == '_') || line.trim_end().ends_with(';') {
        return None;
    }
    if line.starts_with("typedef") || line.starts_with("struct") && !line.contains('(') {
        return None;
    }
    let open = line.find('(')?;
    let name: String = line[..open]
        .trim_end()
        .chars()
        .rev()
        .take_while(|c| c.is_ascii_alphanumeric() || *c == '_')
        .collect::<Vec<_>>()
        .into_iter()
        .rev()
        .collect();
    (!name.is_empty()
        && !["if", "while", "for", "switch", "return", "sizeof"].contains(&name.as_str()))
    .then_some(name)
}

pub fn rank_source(file: &str, text: &str) -> Vec<Ranked> {
    let mut ranked = Vec::new();
    let mut in_block = false;
    let mut depth = 0i32;
    let mut pending: Option<(String, usize, String)> = None;
    let mut current: Option<Ranked> = None;
    for (index, raw) in text.lines().enumerate() {
        let line = strip_comment(raw, &mut in_block);
        if depth == 0 && current.is_none() {
            if let Some(name) = definition_name(raw) {
                pending = Some((name, index + 1, line.clone()));
            }
        }
        let opens = line.matches('{').count() as i32;
        let closes = line.matches('}').count() as i32;
        if depth == 0 && opens > 0 {
            if let Some((name, at, head)) = pending.take() {
                let mut function = Ranked {
                    file: file.to_string(),
                    line: at,
                    name,
                    lines: 0,
                    score: 0,
                    signals: SIGNALS.iter().map(|s| (s.name, 0)).collect(),
                };
                if at != index + 1 {
                    for (slot, signal) in function.signals.iter_mut().zip(SIGNALS) {
                        let found = (signal.find)(&head);
                        slot.1 += found;
                        function.score += signal.weight * found as u32;
                    }
                }
                current = Some(function);
            }
        }
        if let Some(function) = current.as_mut() {
            function.lines += 1;
            for (slot, signal) in function.signals.iter_mut().zip(SIGNALS) {
                let found = (signal.find)(&line);
                slot.1 += found;
                function.score += signal.weight * found as u32;
            }
        }
        depth += opens - closes;
        if depth <= 0 {
            depth = 0;
            if let Some(function) = current.take() {
                ranked.push(function);
            }
            if line.trim_end().ends_with(';') {
                pending = None;
            }
        }
    }
    ranked
}

pub fn rank_tree(root: &Path) -> Result<Vec<Ranked>, String> {
    let mut ranked = Vec::new();
    let mut stack = vec![root.join("games")];
    while let Some(dir) = stack.pop() {
        for entry in std::fs::read_dir(&dir).map_err(|e| format!("{}: {e}", dir.display()))? {
            let path = entry.map_err(|e| e.to_string())?.path();
            if path.is_dir() {
                stack.push(path);
            } else if path
                .extension()
                .is_some_and(|e| e.eq_ignore_ascii_case("c") || e.eq_ignore_ascii_case("h"))
            {
                let text = std::fs::read_to_string(&path)
                    .map_err(|e| format!("{}: {e}", path.display()))?;
                let name = path
                    .strip_prefix(root)
                    .unwrap_or(&path)
                    .to_string_lossy()
                    .into_owned();
                // C files and the headers that are C text.
                if crate::c_items::c_text(&name, &text) {
                    ranked.extend(rank_source(&name, &text));
                }
            }
        }
    }
    ranked.sort_by(|a, b| {
        b.score
            .cmp(&a.score)
            .then(a.file.cmp(&b.file))
            .then(a.line.cmp(&b.line))
    });
    Ok(ranked)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn mechanical_code_outranks_plain_code() {
        let text = "\
void Plain_Run(struct Work *work)
{
    work->count = 0;
    if (work->ready)
        Work_Start(work);
}

void Func_08012345(void)
{
    /* FAKEMATCH: pinned as the native allocates it. */
    register u32 v asm(\"r4\");
    *(u16 *)((u8 *)gWork + 0x1c0) = 0x952;
    do {
        v = Data_03001ec8;
    } while (0);
}
";
        let ranked = rank_source("test.c", text);
        assert_eq!(ranked.len(), 2);
        let plain = ranked.iter().find(|r| r.name == "Plain_Run").unwrap();
        let worst = ranked.iter().find(|r| r.name == "Func_08012345").unwrap();
        assert_eq!(plain.score, 0);
        assert!(worst.score > 30, "{worst:?}");
        let signal = |name| worst.signals.iter().find(|(n, _)| *n == name).unwrap().1;
        assert_eq!(signal("fakematch"), 1);
        assert_eq!(signal("byte_offset"), 1);
        assert_eq!(signal("one_pass"), 1);
        assert_eq!(signal("placeholder"), 2);
    }
}
