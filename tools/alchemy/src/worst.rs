
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

fn strip_comment(line: &str, in_block: &mut bool) -> String {
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

fn definition_name(line: &str) -> Option<String> {
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

/// One function of C text read line by line, by line index from 0: the
/// line that names it and the lines from its opening brace to its closing
/// one.
pub(crate) struct Body {
    pub name: String,
    pub named: usize,
    pub open: usize,
    pub close: usize,
}

/// C text read line by line, the one reading `make worst` and `alchemy
/// fakematch` share: each line without its comments (a FAKEMATCH inside
/// one kept as a word), for each line the function whose body holds it or
/// that was named last before it, and each function whose braces close.
pub(crate) struct Reading {
    pub code: Vec<String>,
    pub before: Vec<Option<String>>,
    pub bodies: Vec<Body>,
}

/// A conditional directive on a line without its comments.
fn branch(code: &str) -> Option<crate::compiler::c_syntax::Branch> {
    use crate::compiler::c_syntax::Branch;
    let directive = code.trim_start().strip_prefix('#')?.trim_start();
    let end = directive
        .find(|c: char| !c.is_ascii_alphanumeric() && c != '_')
        .unwrap_or(directive.len());
    match &directive[..end] {
        "if" | "ifdef" | "ifndef" => Some(Branch::If),
        "elif" | "else" => Some(Branch::Else),
        "endif" => Some(Branch::Endif),
        _ => None,
    }
}

/// Reads C text as the C preprocessor reads one edition: each arm of a
/// conditional starts at the brace depth its `#if` had, so a brace opened
/// in both arms of an `#if`/`#else` counts once, a function opened in both
/// arms is one function, and one whose closing brace sits in each arm
/// stays open into the next.
pub(crate) fn read(text: &str) -> Reading {
    use crate::compiler::c_syntax::Branch;
    let mut reading = Reading {
        code: Vec::new(),
        before: Vec::new(),
        bodies: Vec::new(),
    };
    let mut closed = Vec::new();
    let mut in_block = false;
    let mut depth = 0i32;
    let mut pending: Option<(String, usize)> = None;
    let mut current: Option<usize> = None;
    // The depth and the open function at each open `#if`.
    let mut arms: Vec<(i32, Option<usize>)> = Vec::new();
    for (index, raw) in text.lines().enumerate() {
        reading.before.push(
            current
                .map(|at| reading.bodies[at].name.clone())
                .or_else(|| pending.as_ref().map(|(name, _)| name.clone())),
        );
        let line = strip_comment(raw, &mut in_block);
        match branch(&line) {
            Some(Branch::If) => arms.push((depth, current)),
            Some(Branch::Else) => {
                if let Some(&(at_if, open)) = arms.last() {
                    depth = at_if;
                    if let (None, Some(open)) = (current, open) {
                        closed[open] = false;
                        current = Some(open);
                    }
                }
            }
            Some(Branch::Endif) => {
                arms.pop();
            }
            None => {}
        }
        if depth == 0 && current.is_none() {
            if let Some(name) = definition_name(raw) {
                pending = Some((name, index));
            }
        }
        let opens = line.matches('{').count() as i32;
        let closes = line.matches('}').count() as i32;
        if depth == 0 && opens > 0 && current.is_none() {
            if let Some((name, named)) = pending.take() {
                reading.bodies.push(Body {
                    name,
                    named,
                    open: index,
                    close: index,
                });
                closed.push(false);
                current = Some(reading.bodies.len() - 1);
            }
        }
        if let Some(at) = current {
            reading.bodies[at].close = index;
        }
        depth += opens - closes;
        if depth <= 0 {
            depth = 0;
            // A function opened in an arm sits at depth 0 between the next
            // arm's start and its brace; only a closing brace ends it.
            if closes > 0 {
                if let Some(at) = current.take() {
                    closed[at] = true;
                }
            }
            if line.trim_end().ends_with(';') {
                pending = None;
            }
        }
        reading.code.push(line);
    }
    let mut closed = closed.into_iter();
    reading.bodies.retain(|_| closed.next().unwrap_or(false));
    reading
}

pub fn rank_source(file: &str, text: &str) -> Vec<Ranked> {
    let reading = read(text);
    let mut ranked = Vec::new();
    for body in reading.bodies {
        let mut function = Ranked {
            file: file.to_string(),
            line: body.named + 1,
            name: body.name,
            lines: body.close - body.open + 1,
            score: 0,
            signals: SIGNALS.iter().map(|s| (s.name, 0)).collect(),
        };
        let head = (body.named != body.open).then_some(body.named);
        for line in head.into_iter().chain(body.open..=body.close) {
            for (slot, signal) in function.signals.iter_mut().zip(SIGNALS) {
                let found = (signal.find)(&reading.code[line]);
                slot.1 += found;
                function.score += signal.weight * found as u32;
            }
        }
        ranked.push(function);
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

    /// Each function with its lines, as `make worst` ranks them.
    fn ranked(text: &str) -> Vec<(String, usize, usize)> {
        let mut found: Vec<_> = rank_source("test.c", text)
            .into_iter()
            .map(|r| (r.name, r.line, r.lines))
            .collect();
        found.sort_by_key(|(_, line, _)| *line);
        found
    }

    #[test]
    fn a_brace_opened_in_both_arms_counts_once() {
        // window.c's article writer: `if (...) {` in both arms.
        let text = "\
u32 Article(s32 no)
{
#if defined(TBS_EDITION_ES)
\tif (count == 0)
\t{
#else
\tif (no == 0)
\t{
#endif
\t\tname += 2;
\t}
\treturn 0;
}

void After(void)
{
\tAfter_Run();
}
";
        assert_eq!(
            ranked(text),
            [("Article".into(), 1, 12), ("After".into(), 15, 3)]
        );
    }

    #[test]
    fn arms_of_different_length_and_elif_chains_each_start_at_the_if() {
        // menu.c's item command loop: other statements before the brace in
        // one arm, a nested conditional, and clear.c's #elif chain.
        let text = "\
void Loop(void)
{
\tswitch (state)
\t{
\tcase 1:
#if EDITION_INTERNATIONAL
\t\tif (Select(0) != -1)
\t\t{
#else
\t\tchosen = Select(0);
\t\tif (chosen != -1)
\t\t{
#endif
#if !EDITION_INTERNATIONAL
\t\t\tRedraw();
#endif
\t\t}
\t\tbreak;
\tcase 2:
#if defined(TBS_EDITION_DE)
\t\tif (code == 69)
\t\t{
#elif defined(TBS_EDITION_FR)
\t\tif (code == 70)
\t\t{
#else
\t\tif (code == 85)
\t\t{
#endif
\t\t\tbreak;
\t\t}
\t}
}

void Next(void)
{
}
";
        assert_eq!(ranked(text), [("Loop".into(), 1, 32), ("Next".into(), 35, 2)]);
    }

    #[test]
    fn a_function_opened_or_closed_in_each_arm_is_one_function() {
        let text = "\
#if GAME_TLA
void Pick(s32 a)
{
#else
void Pick(s32 a, s32 b)
{
#endif
\tPick_Run();
#if GAME_TLA
\treturn;
}
#else
\tPick_End();
}
#endif

void Last(void)
{
}
";
        assert_eq!(ranked(text), [("Pick".into(), 2, 12), ("Last".into(), 17, 2)]);
        let reading = read(text);
        assert_eq!(reading.before[12].as_deref(), Some("Pick"), "the second arm stays in Pick");
    }
}
