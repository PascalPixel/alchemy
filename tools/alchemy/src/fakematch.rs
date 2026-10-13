
use crate::worst::{definition_name, strip_comment};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy fakematch [--show N]\n\
Scans every FAKEMATCH tag under games/, classifies the device each marks\n\
(call wrapper, one-pass block, volatile, asm barrier, fixed register, call\n\
type, forced temporary, frame local, other), groups the tags into families\n\
by device and helper or reason, and weighs each family by the bytes of the\n\
functions it steers in the English builds (make compare-editions first).\n\
Writes out/reports/fakematch-tags.tsv and out/reports/fakematch-families.tsv\n\
and prints the N largest families (default 30). Reasons that describe a\n\
different instruction count rather than order or registers are marked\n\
count?: S2 allows a device only for order or register choice.";

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Tag {
    pub file: String,
    pub line: usize,
    pub function: String,
    pub reason: String,
    pub kind: &'static str,
    pub family: String,
    pub count_suspect: bool,
}

fn kind(reason: &str, code: &str) -> &'static str {
    let reason = reason.to_ascii_lowercase();
    let has = |needles: &[&str], text: &str| needles.iter().any(|n| text.contains(n));
    if code.contains("asm(\"r") || code.contains("asm (\"r") {
        "fixed register"
    } else if has(&["asm(", "asm (", "__asm__"], code) {
        "asm barrier"
    } else if code.contains("volatile") || reason.contains("volatile") {
        "volatile"
    } else if code.contains("do {")
        || has(
            &[
                "one-pass",
                "one pass",
                "run once",
                "runs once",
                "blocks that run",
                "do/while",
                "boundary",
                "barrier",
            ],
            &reason,
        )
    {
        "one-pass block"
    } else if called(code).is_some_and(|name| is_call_wrapper(&name))
        || has(
            &[
                "wrapper",
                "forward",
                "direct call",
                "helper",
                "call3",
                "call2",
                "callN",
            ],
            &reason,
        )
    {
        "call wrapper"
    } else if has(
        &[
            "returning int",
            "void result",
            "void call",
            "prototype",
            "cast",
        ],
        &reason,
    ) {
        "call type"
    } else if has(&["unused local", "frame"], &reason) {
        "frame local"
    } else if has(
        &["temporary", "temp ", "its own int", "copy", "local"],
        &reason,
    ) {
        "forced temporary"
    } else {
        "other"
    }
}

fn is_call_wrapper(name: &str) -> bool {
    name.strip_prefix("Call")
        .is_some_and(|rest| !rest.is_empty() && rest.chars().all(|c| c.is_ascii_digit()))
}

fn called(code: &str) -> Option<String> {
    const KEYWORDS: &[&str] = &[
        "if", "while", "for", "switch", "return", "sizeof", "do", "asm", "__asm__",
    ];
    let bytes = code.as_bytes();
    let mut start = None;
    for (i, &b) in bytes.iter().enumerate() {
        let word = b.is_ascii_alphanumeric() || b == b'_';
        match (word, start) {
            (true, None) => start = Some(i),
            (false, Some(s)) => {
                let name = &code[s..i];
                let rest = code[i..].trim_start();
                if rest.starts_with('(')
                    && !KEYWORDS.contains(&name)
                    && !name.chars().next().is_some_and(|c| c.is_ascii_digit())
                {
                    return Some(name.to_string());
                }
                start = None;
            }
            _ => {}
        }
    }
    None
}

fn count_suspect(reason: &str) -> bool {
    let reason = reason.to_ascii_lowercase();
    [
        "retain",
        "dead",
        "extra",
        "missing",
        "instruction count",
        "extension",
        "becomes",
        "survive",
        "vanish",
        "keeps the literal",
        "literal load",
        "pool load",
        "duplicate",
    ]
    .iter()
    .any(|needle| reason.contains(needle))
}

fn trench(file: &str) -> String {
    let parts = file.split('/').collect::<Vec<_>>();
    let game = parts.get(1).copied().unwrap_or("");
    let at = parts
        .iter()
        .position(|p| *p == "src" || *p == "include")
        .unwrap_or(parts.len());
    let folders = parts[(at + 1).min(parts.len())..parts.len().saturating_sub(1)]
        .iter()
        .take(2)
        .copied()
        .collect::<Vec<_>>();
    let place = if folders.is_empty() {
        parts.get(at).copied().unwrap_or("").to_string()
    } else {
        folders.join("/")
    };
    format!("{game}/{place}")
}

fn family(kind: &'static str, file: &str, code: &str) -> String {
    match (kind, called(code)) {
        ("call wrapper", Some(name)) if is_call_wrapper(&name) => format!("{kind}: {name}"),
        _ => format!("{kind} in {}", trench(file)),
    }
}

pub(crate) fn scan_source(file: &str, text: &str) -> Vec<Tag> {
    let lines = text.lines().collect::<Vec<_>>();
    let mut tags = Vec::new();
    let (mut in_block, mut depth) = (false, 0i32);
    let mut pending: Option<String> = None;
    let mut current: Option<String> = None;
    for (index, raw) in lines.iter().enumerate() {
        if let Some(at) = raw.find("FAKEMATCH") {
            let mut reason = raw[at + "FAKEMATCH".len()..]
                .trim_start_matches(':')
                .to_string();
            let mut end = index;
            while !reason.contains("*/") && end + 1 < lines.len() {
                end += 1;
                reason.push(' ');
                reason.push_str(lines[end].trim().trim_start_matches('*'));
            }
            let reason = reason
                .split("*/")
                .next()
                .unwrap_or("")
                .split_whitespace()
                .collect::<Vec<_>>()
                .join(" ");
            let mut code = String::new();
            for l in lines[end + 1..]
                .iter()
                .map(|l| l.trim())
                .filter(|l| !l.is_empty() && !l.starts_with("/*") && !l.starts_with('*'))
                .take(3)
            {
                code.push_str(l);
                code.push(' ');
                if l.ends_with(';') || l.ends_with('{') || l.ends_with('}') {
                    break;
                }
            }
            let tail = raw[..at].to_string();
            let code = if tail.trim_start().starts_with("/*") || tail.trim().is_empty() {
                code
            } else {
                format!("{tail} {code}")
            };
            let kind = kind(&reason, &code);
            tags.push(Tag {
                file: file.to_string(),
                line: index + 1,
                function: current
                    .clone()
                    .or_else(|| pending.clone())
                    .unwrap_or_else(|| "(file scope)".into()),
                family: family(kind, file, &code),
                count_suspect: count_suspect(&reason),
                reason,
                kind,
            });
        }
        let line = strip_comment(raw, &mut in_block);
        if depth == 0 && current.is_none() {
            if let Some(name) = definition_name(raw) {
                pending = Some(name);
            }
        }
        let (opens, closes) = (
            line.matches('{').count() as i32,
            line.matches('}').count() as i32,
        );
        if depth == 0 && opens > 0 {
            current = pending.take();
        }
        depth += opens - closes;
        if depth <= 0 {
            depth = 0;
            current = None;
            if line.trim_end().ends_with(';') {
                pending = None;
            }
        }
    }
    tags
}

fn function_calls(text: &str) -> Vec<(String, BTreeSet<String>)> {
    let mut found = Vec::new();
    let (mut in_block, mut depth) = (false, 0i32);
    let mut pending: Option<String> = None;
    let mut current: Option<(String, BTreeSet<String>)> = None;
    for raw in text.lines() {
        let line = strip_comment(raw, &mut in_block);
        if depth == 0 && current.is_none() {
            if let Some(name) = definition_name(raw) {
                pending = Some(name);
            }
        }
        let (opens, closes) = (
            line.matches('{').count() as i32,
            line.matches('}').count() as i32,
        );
        if depth == 0 && opens > 0 {
            current = pending.take().map(|name| (name, BTreeSet::new()));
        }
        if let Some((_, calls)) = current.as_mut() {
            let mut rest = line.as_str();
            while let Some(name) = called(rest) {
                let at = rest.find(&name.to_string()).unwrap_or(0) + name.len();
                calls.insert(name);
                rest = &rest[at..];
            }
        }
        depth += opens - closes;
        if depth <= 0 {
            depth = 0;
            if let Some(function) = current.take() {
                found.push(function);
            }
            if line.trim_end().ends_with(';') {
                pending = None;
            }
        }
    }
    found
}

fn sources(root: &Path) -> Result<Vec<PathBuf>, String> {
    let mut found = Vec::new();
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
                found.push(path);
            }
        }
    }
    found.sort();
    Ok(found)
}

fn sizes(root: &Path, game: &str) -> BTreeMap<String, u64> {
    let build = root.join(format!("out/{game}-en"));
    let mut elves = vec![build.join(format!("{game}-en.elf"))];
    if let Ok(entries) = std::fs::read_dir(build.join("overlays")) {
        elves.extend(
            entries
                .filter_map(|e| e.ok().map(|e| e.path()))
                .filter(|p| p.extension().is_some_and(|e| e == "elf")),
        );
    }
    let nm = crate::compiler::routing::binutils_prefix().join("bin/arm-none-eabi-nm");
    let Ok(output) = std::process::Command::new(nm)
        .args(["-S", "--defined-only"])
        .args(&elves)
        .output()
    else {
        return BTreeMap::new();
    };
    let mut sizes = BTreeMap::new();
    for line in String::from_utf8_lossy(&output.stdout).lines() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        let [_, size, "T" | "t", name] = fields[..] else {
            continue;
        };
        if let Ok(size) = u64::from_str_radix(size, 16) {
            *sizes.entry(name.to_string()).or_default() += size;
        }
    }
    sizes
}

fn games(file: &str) -> &'static [&'static str] {
    if file.contains("games/tbs/") {
        &["tbs"]
    } else if file.contains("games/tla/") {
        &["tla"]
    } else {
        &["tbs", "tla"]
    }
}

#[derive(Default)]
struct Family {
    kind: &'static str,
    tags: usize,
    suspect: usize,
    functions: BTreeSet<(String, String)>,
    bytes: [u64; 2],
    example: String,
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let show = match arguments {
        [] => 30,
        [flag, n] if flag == "--show" => n.parse().map_err(|_| USAGE.to_string())?,
        _ => return Err(USAGE.into()),
    };
    let root = crate::compiler::routing::root();
    let mut tags = Vec::new();
    let mut callers = Vec::new();
    for path in sources(root)? {
        let text =
            std::fs::read_to_string(&path).map_err(|e| format!("{}: {e}", path.display()))?;
        let name = path
            .strip_prefix(root)
            .unwrap_or(&path)
            .to_string_lossy()
            .into_owned();
        tags.extend(scan_source(&name, &text));
        if name.ends_with(".c") {
            for (function, calls) in function_calls(&text) {
                callers.push((name.clone(), function, calls));
            }
        }
    }
    let sizes = [sizes(root, "tbs"), sizes(root, "tla")];
    let size = |file: &str, function: &str| {
        let mut bytes = [0u64; 2];
        for game in games(file) {
            let at = usize::from(*game == "tla");
            bytes[at] = sizes[at].get(function).copied().unwrap_or(0);
        }
        bytes
    };
    let mut tag_rows =
        String::from("file\tline\tfunction\ttbs_bytes\ttla_bytes\tkind\tcount?\tfamily\treason\n");
    let mut families = BTreeMap::<String, Family>::new();
    for tag in &tags {
        let bytes = size(&tag.file, &tag.function);
        let _ = writeln!(
            tag_rows,
            "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
            tag.file,
            tag.line,
            tag.function,
            bytes[0],
            bytes[1],
            tag.kind,
            if tag.count_suspect { "count?" } else { "" },
            tag.family,
            tag.reason
        );
        let family = families.entry(tag.family.clone()).or_default();
        family.kind = tag.kind;
        family.tags += 1;
        family.suspect += usize::from(tag.count_suspect);
        if family
            .functions
            .insert((tag.file.clone(), tag.function.clone()))
        {
            family.bytes[0] += bytes[0];
            family.bytes[1] += bytes[1];
        }
        if family.example.is_empty() {
            family.example = format!("{}:{}", tag.file, tag.line);
        }
    }
    let mut ranked = families.into_iter().collect::<Vec<_>>();
    ranked.sort_by(|(a_name, a), (b_name, b)| {
        (b.bytes[0] + b.bytes[1])
            .cmp(&(a.bytes[0] + a.bytes[1]))
            .then(b.tags.cmp(&a.tags))
            .then(a_name.cmp(b_name))
    });
    let mut family_rows = String::from(
        "rank\ttags\tcount?\tfunctions\ttbs_bytes\ttla_bytes\tkind\tfamily\texample\n",
    );
    for (rank, (name, f)) in ranked.iter().enumerate() {
        let _ = writeln!(
            family_rows,
            "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
            rank + 1,
            f.tags,
            f.suspect,
            f.functions.len(),
            f.bytes[0],
            f.bytes[1],
            f.kind,
            name,
            f.example
        );
    }
    let helpers = tags
        .iter()
        .filter(|t| t.file.ends_with(".h") && t.function != "(file scope)")
        .map(|t| (t.function.clone(), t.file.clone()))
        .collect::<BTreeMap<_, _>>();
    let mut spread = BTreeMap::<&str, (usize, [u64; 2])>::new();
    let mut steered = BTreeSet::new();
    // A helper reaches the callers of its own game (a games/common header,
    // both): a same-named function of the other game is not its caller.
    let game = |path: &str| path.split('/').take(2).collect::<Vec<_>>().join("/");
    let reaches = |header: &str, caller: &str| {
        let (header, caller) = (game(header), game(caller));
        header == "games/common" || caller == "games/common" || header == caller
    };
    for (file, function, calls) in &callers {
        for helper in calls.iter().filter(|c| helpers.get(*c).is_some_and(|header| reaches(header, file))) {
            let bytes = size(file, function);
            let entry = spread.entry(helper.as_str()).or_default();
            entry.0 += 1;
            entry.1[0] += bytes[0];
            entry.1[1] += bytes[1];
            steered.insert((file.clone(), function.clone()));
        }
    }
    let mut spread = spread.into_iter().collect::<Vec<_>>();
    spread.sort_by(|a, b| (b.1 .1[0] + b.1 .1[1]).cmp(&(a.1 .1[0] + a.1 .1[1])));
    let mut helper_rows = String::from("helper\theader\tcallers\ttbs_bytes\ttla_bytes\n");
    for (helper, (n, bytes)) in &spread {
        let _ = writeln!(
            helper_rows,
            "{helper}\t{}\t{n}\t{}\t{}",
            helpers[*helper], bytes[0], bytes[1]
        );
    }
    let caller_bytes = steered.iter().fold([0u64; 2], |mut sum, (file, function)| {
        let bytes = size(file, function);
        sum[0] += bytes[0];
        sum[1] += bytes[1];
        sum
    });
    let reports = root.join("out/reports");
    std::fs::create_dir_all(&reports).map_err(|e| e.to_string())?;
    for (name, text) in [
        ("fakematch-tags.tsv", &tag_rows),
        ("fakematch-families.tsv", &family_rows),
        ("fakematch-helpers.tsv", &helper_rows),
    ] {
        std::fs::write(reports.join(name), text).map_err(|e| format!("{name}: {e}"))?;
    }
    let mut kinds = BTreeMap::<&str, (usize, usize)>::new();
    for tag in &tags {
        let entry = kinds.entry(tag.kind).or_default();
        entry.0 += 1;
        entry.1 += usize::from(tag.count_suspect);
    }
    println!(
        "{} tags in {} families; {} read as a count difference",
        tags.len(),
        ranked.len(),
        tags.iter().filter(|t| t.count_suspect).count()
    );
    for (kind, (n, suspect)) in &kinds {
        println!("  {kind:<16} {n:>5} tags, {suspect} count?");
    }
    for line in family_rows.lines().take(show + 1) {
        println!("{line}");
    }
    println!(
        "{} functions call a tagged inline helper: {} ☀️ and {} ⚓️ English bytes",
        steered.len(),
        caller_bytes[0],
        caller_bytes[1]
    );
    for line in helper_rows.lines().take(16) {
        println!("{line}");
    }
    println!("wrote out/reports/fakematch-tags.tsv, -families.tsv and -helpers.tsv");
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tags_are_classified_and_grouped_by_their_root_device() {
        let text = "\
void Scene_Run(struct Work *work)
{
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments. */
    Call3(Actor_Move, work, 3, 4);
    /* FAKEMATCH: direct calls let GCC 2.96 share constant arguments. */
    Call3(Actor_Turn, work, 3, 5);
    /* FAKEMATCH: removing this one-pass block changes scheduling. */
    do {
        work->count = 0;
    } while (0);
}

static u8 Value_Read(void)
{
    /* FAKEMATCH: the volatile read keeps the RAM load first. */
    return *(volatile u8 *)&gValue;
}

void Table_Clear(void)
{
    /* FAKEMATCH: retain the count's literal load. */
    gTable.count = gTable.count;
}
";
        let tags = scan_source("games/common/src/test.c", text);
        assert_eq!(tags.len(), 5);
        assert_eq!(
            tags.iter()
                .map(|t| (t.function.as_str(), t.kind))
                .collect::<Vec<_>>(),
            [
                ("Scene_Run", "call wrapper"),
                ("Scene_Run", "call wrapper"),
                ("Scene_Run", "one-pass block"),
                ("Value_Read", "volatile"),
                ("Table_Clear", "other"),
            ]
        );
        assert_eq!(tags[0].family, "call wrapper: Call3");
        assert_eq!(tags[2].family, "one-pass block in common/src");
        assert_eq!(tags[0].family, tags[1].family);
        assert!(tags[4].count_suspect && !tags[0].count_suspect);
    }
}
