use std::collections::{BTreeMap, BTreeSet};
use std::ops::Range;
use std::path::{Component, Path, PathBuf};

pub(crate) fn mask(text: &str) -> Vec<u8> {
    let bytes = text.as_bytes();
    let mut out = bytes.to_vec();
    let blank = |out: &mut Vec<u8>, range: Range<usize>| {
        for byte in &mut out[range] {
            if *byte != b'\n' {
                *byte = b' ';
            }
        }
    };
    let (mut i, mut line_start, mut directive) = (0, true, false);
    while i < bytes.len() {
        let byte = bytes[i];
        if bytes[i..].starts_with(b"/*") {
            let end = find(bytes, i + 2, b"*/").map_or(bytes.len(), |at| at + 2);
            blank(&mut out, i..end);
            i = end;
            continue;
        }
        if bytes[i..].starts_with(b"//") {
            let end = find(bytes, i, b"\n").unwrap_or(bytes.len());
            blank(&mut out, i..end);
            i = end;
            continue;
        }
        if byte == b'\n' {
            if !(directive && i > 0 && bytes[i - 1] == b'\\') {
                directive = false;
                line_start = true;
            }
            i += 1;
            continue;
        }
        if directive || (line_start && byte == b'#') {
            directive = true;
            out[i] = b' ';
            i += 1;
            continue;
        }
        if !byte.is_ascii_whitespace() {
            line_start = false;
        }
        if byte == b'"' || byte == b'\'' {
            let mut end = i + 1;
            while end < bytes.len() && bytes[end] != byte && bytes[end] != b'\n' {
                end += if bytes[end] == b'\\' { 2 } else { 1 };
            }
            let end = end.min(bytes.len());
            blank(&mut out, i + 1..end);
            i = end + 1;
            continue;
        }
        if !byte.is_ascii() {
            out[i] = b' ';
        }
        i += 1;
    }
    out
}

fn find(bytes: &[u8], from: usize, needle: &[u8]) -> Option<usize> {
    bytes
        .get(from..)?
        .windows(needle.len())
        .position(|window| window == needle)
        .map(|at| from + at)
}

pub(crate) fn is_ident(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

pub(crate) fn text(mask: &[u8], range: Range<usize>) -> &str {
    std::str::from_utf8(&mask[range]).unwrap_or("")
}

pub(crate) fn trim(mask: &[u8], range: Range<usize>) -> Range<usize> {
    let mut range = range;
    while range.start < range.end && mask[range.start].is_ascii_whitespace() {
        range.start += 1;
    }
    while range.end > range.start && mask[range.end - 1].is_ascii_whitespace() {
        range.end -= 1;
    }
    range
}

pub(crate) fn identifier(mask: &[u8], range: &Range<usize>) -> bool {
    !range.is_empty()
        && !mask[range.start].is_ascii_digit()
        && mask[range.clone()].iter().all(|&b| is_ident(b))
        && !KEYWORDS.contains(&text(mask, range.clone()))
}

const KEYWORDS: &[&str] = &[
    "if", "else", "while", "for", "do", "switch", "case", "return", "sizeof", "goto",
];

pub(crate) fn closing(mask: &[u8], open: usize) -> Option<usize> {
    let (o, c) = match mask.get(open)? {
        b'(' => (b'(', b')'),
        b'[' => (b'[', b']'),
        b'{' => (b'{', b'}'),
        _ => return None,
    };
    let mut depth = 0usize;
    for (i, &byte) in mask.iter().enumerate().skip(open) {
        if byte == o {
            depth += 1;
        } else if byte == c {
            depth -= 1;
            if depth == 0 {
                return Some(i);
            }
        }
    }
    None
}

pub(crate) fn opening(mask: &[u8], close: usize) -> Option<usize> {
    let (o, c) = match mask.get(close)? {
        b')' => (b'(', b')'),
        b']' => (b'[', b']'),
        b'}' => (b'{', b'}'),
        _ => return None,
    };
    let mut depth = 0usize;
    for i in (0..=close).rev() {
        if mask[i] == c {
            depth += 1;
        } else if mask[i] == o {
            depth -= 1;
            if depth == 0 {
                return Some(i);
            }
        }
    }
    None
}

pub(crate) fn next(mask: &[u8], at: usize) -> Option<usize> {
    (at..mask.len()).find(|&i| !mask[i].is_ascii_whitespace())
}

pub(crate) fn previous(mask: &[u8], at: usize) -> Option<usize> {
    (0..at.min(mask.len())).rev().find(|&i| !mask[i].is_ascii_whitespace())
}

pub(crate) fn word_before(mask: &[u8], at: usize) -> Option<Range<usize>> {
    let end = previous(mask, at)? + 1;
    let start = (0..end).rev().take_while(|&i| is_ident(mask[i])).last()?;
    let range = start..end;
    (!mask[start].is_ascii_digit()).then_some(range)
}

pub(crate) fn word_at(mask: &[u8], at: usize) -> Range<usize> {
    let end = (at..mask.len()).find(|&i| !is_ident(mask[i])).unwrap_or(mask.len());
    at..end
}

pub(crate) fn words(mask: &[u8], word: &str, range: Range<usize>) -> Vec<usize> {
    let needle = word.as_bytes();
    let mut found = Vec::new();
    let mut i = range.start;
    while i + needle.len() <= range.end.min(mask.len()) {
        if &mask[i..i + needle.len()] == needle
            && (i == 0 || !is_ident(mask[i - 1]))
            && mask.get(i + needle.len()).is_none_or(|&b| !is_ident(b))
        {
            found.push(i);
            i += needle.len();
        } else {
            i += 1;
        }
    }
    found
}

pub(crate) fn normal_type(inner: &str) -> Option<String> {
    if !inner.bytes().all(|b| is_ident(b) || b == b'*' || b.is_ascii_whitespace()) {
        return None;
    }
    let named = inner.trim_end_matches(|c: char| c == '*' || c.is_whitespace());
    if named.is_empty() || named.contains('*') {
        return None;
    }
    let mut name = named.split_whitespace().collect::<Vec<_>>().join(" ");
    let stars = inner[named.len()..].matches('*').count();
    if stars > 0 {
        name.push(' ');
        name.push_str(&"*".repeat(stars));
    }
    Some(name)
}

pub(crate) fn type_name(mask: &[u8], at: usize) -> Option<(String, usize)> {
    if mask.get(at) != Some(&b'(') {
        return None;
    }
    let close = at + mask[at..].iter().position(|&b| b == b')')?;
    Some((normal_type(text(mask, at + 1..close))?, close + 1))
}

pub(crate) fn store(mask: &[u8], end: usize) -> bool {
    next(mask, end).is_some_and(|i| mask[i] == b'=' && mask.get(i + 1) != Some(&b'='))
}

pub(crate) fn postfix(mask: &[u8], end: usize) -> bool {
    next(mask, end).is_some_and(|i| {
        matches!(mask[i], b'[' | b'(' | b'.')
            || mask[i..].starts_with(b"->")
            || mask[i..].starts_with(b"++")
            || mask[i..].starts_with(b"--")
    })
}

pub(crate) fn split_commas(mask: &[u8], range: Range<usize>) -> Vec<Range<usize>> {
    let mut parts = Vec::new();
    let (mut depth, mut start) = (0i32, range.start);
    for i in range.clone() {
        match mask[i] {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => depth -= 1,
            b',' if depth == 0 => {
                parts.push(start..i);
                start = i + 1;
            }
            _ => {}
        }
    }
    parts.push(start..range.end);
    parts
}

#[derive(Clone, Debug)]
pub(crate) struct Function {
    pub name: Range<usize>,
    pub params: Range<usize>,
    pub body: Range<usize>,
}

pub(crate) fn functions(mask: &[u8]) -> Vec<Function> {
    let mut found = Vec::new();
    let (mut depth, mut i) = (0usize, 0);
    while i < mask.len() {
        match mask[i] {
            b'{' if depth == 0 => {
                if let Some(function) = function_at(mask, i) {
                    i = function.body.end;
                    found.push(function);
                    continue;
                }
                depth += 1;
            }
            b'{' => depth += 1,
            b'}' => depth = depth.saturating_sub(1),
            _ => {}
        }
        i += 1;
    }
    found
}

pub(crate) fn function_at(mask: &[u8], brace: usize) -> Option<Function> {
    let close = previous(mask, brace)?;
    if mask[close] != b')' {
        return None;
    }
    let open = opening(mask, close)?;
    let name = word_before(mask, open)?;
    if !identifier(mask, &name) {
        return None;
    }
    let end = closing(mask, brace)?;
    Some(Function {
        name,
        params: open + 1..close,
        body: brace..end + 1,
    })
}

pub(crate) fn includes(text: &str) -> Vec<(String, bool)> {
    text.lines()
        .filter_map(|line| {
            let rest = line.trim_start().strip_prefix('#')?.trim_start();
            let rest = rest.strip_prefix("include")?.trim_start();
            if let Some(quoted) = rest.strip_prefix('"') {
                Some((quoted[..quoted.find('"')?].to_string(), true))
            } else {
                let angled = rest.strip_prefix('<')?;
                Some((angled[..angled.find('>')?].to_string(), false))
            }
        })
        .collect()
}

pub(crate) fn resolve(name: &str, quoted: bool, directory: &Path, search: &[PathBuf]) -> Option<PathBuf> {
    let beside = quoted.then(|| directory.to_path_buf());
    beside
        .iter()
        .chain(search)
        .map(|dir| dir.join(name))
        .find(|path| path.is_file())
        .and_then(|path| path.canonicalize().ok())
}

pub(crate) fn reachable(file: &Path, search: &[PathBuf]) -> BTreeSet<PathBuf> {
    include_graph(file, search, &|_| None).into_values().flatten().map(|(path, _)| path).collect()
}

/// What `file` includes and what each file it reaches includes, #if
/// ignored: each file (canonical; `file` as given) with the files its
/// includes resolve to, and whether each was found beside it. `stand_in`
/// gives the text read in place of a file's (by its canonical path), as a
/// trial compiles it.
pub(crate) fn include_graph(file: &Path, search: &[PathBuf], stand_in: &dyn Fn(&Path) -> Option<String>) -> BTreeMap<PathBuf, Vec<(PathBuf, bool)>> {
    let mut graph = BTreeMap::new();
    let mut pending = vec![file.to_path_buf()];
    while let Some(path) = pending.pop() {
        if graph.contains_key(&path) {
            continue;
        }
        let text = stand_in(&path).or_else(|| std::fs::read(&path).ok().map(|bytes| String::from_utf8_lossy(&bytes).into_owned()));
        let directory = path.parent().unwrap_or(Path::new("."));
        let mut included = Vec::new();
        for (name, quoted) in includes(text.as_deref().unwrap_or("")) {
            let beside = if quoted { resolve(&name, true, directory, &[]) } else { None };
            let found = beside.is_some();
            if let Some(header) = beside.or_else(|| resolve(&name, false, directory, search)) {
                pending.push(header.clone());
                included.push((header, found));
            }
        }
        graph.insert(path, included);
    }
    graph
}

pub(crate) fn spelling(file: &Path, header: &Path, search: &[PathBuf]) -> Option<String> {
    let directory = file.parent()?;
    let header = header.canonicalize().ok()?;
    let name = header.file_name()?.to_str()?;
    if resolve(name, true, directory, search).as_deref() == Some(header.as_path()) {
        return Some(name.to_string());
    }
    let relative = relative_path(&directory.canonicalize().ok()?, &header);
    let relative = relative.to_str()?.to_string();
    (resolve(&relative, true, directory, search).as_deref() == Some(header.as_path())).then_some(relative)
}

fn relative_path(from: &Path, to: &Path) -> PathBuf {
    let from: Vec<Component> = from.components().collect();
    let to: Vec<Component> = to.components().collect();
    let common = from.iter().zip(&to).take_while(|(a, b)| a == b).count();
    let mut path = PathBuf::new();
    for _ in common..from.len() {
        path.push("..");
    }
    for part in &to[common..] {
        path.push(part);
    }
    path
}

pub(crate) fn insert_include(text: &str, name: &str) -> String {
    let line = format!("#include \"{name}\"\n");
    let mask = mask(text);
    let (mut offset, mut depth) = (0, 0i32);
    let (mut after, mut first, mut stop) = (None, None, text.len());
    for piece in text.split_inclusive('\n') {
        let range = offset..offset + piece.len();
        offset = range.end;
        let trimmed = piece.trim_start();
        if let Some(directive) = trimmed.strip_prefix('#') {
            let directive = directive.trim_start();
            first.get_or_insert(range.start);
            if directive.starts_with("if") {
                depth += 1;
            } else if directive.starts_with("endif") {
                depth -= 1;
            } else if directive.starts_with("include") && depth == 0 {
                after = Some(range.end);
            }
        } else if !mask[range.clone()].iter().all(u8::is_ascii_whitespace) {
            stop = range.start;
            break;
        }
    }
    match after {
        Some(at) => {
            let newline = if text[..at].ends_with('\n') { "" } else { "\n" };
            format!("{}{newline}{line}{}", &text[..at], &text[at..])
        }
        None => {
            let at = first.unwrap_or(stop);
            let gap = if first.is_none() && at < text.len() { "\n" } else { "" };
            format!("{}{line}{gap}{}", &text[..at], &text[at..])
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn the_mask_keeps_offsets_and_blanks_comments_literals_and_directives() {
        let text = "#define X (1)\ns32 a = \"(\"; /* ( ☀️ */ b('[');\n";
        let mask = mask(text);
        assert_eq!(mask.len(), text.len());
        let shown = String::from_utf8(mask).unwrap();
        assert_eq!(shown.lines().next().unwrap().trim(), "");
        assert!(shown.contains("s32 a = \" \";"));
        assert!(shown.contains("b(' ');"));
        assert!(!shown.contains("/*"));
    }

    #[test]
    fn functions_are_found_with_their_parameters_and_bodies() {
        let text = "struct A { s32 b; };\nstatic s32 F(s32 a, u8 *b)\n{\n    if (a) { b[0] = 1; }\n    return 0;\n}\n";
        let mask = mask(text);
        let found = functions(&mask);
        assert_eq!(found.len(), 1);
        assert_eq!(&text[found[0].name.clone()], "F");
        assert_eq!(&text[found[0].params.clone()], "s32 a, u8 *b");
        assert!(text[found[0].body.clone()].ends_with("return 0;\n}"));
    }

    #[test]
    fn includes_go_after_the_top_block_not_after_one_further_down() {
        let text = "/* head */\n#include \"a.h\"\n#if X\n#include \"b.h\"\n#endif\n#define Y 1\n\nvoid F(void);\n#include \"late.inc\"\n";
        let out = insert_include(text, "io_reg.h");
        assert!(out.contains("#include \"a.h\"\n#include \"io_reg.h\"\n#if X"));
        assert!(out.ends_with("#include \"late.inc\"\n"));
        let bare = insert_include("/* head */\nvoid F(void);\n", "io_reg.h");
        assert_eq!(bare, "/* head */\n#include \"io_reg.h\"\n\nvoid F(void);\n");
    }

    #[test]
    fn the_include_graph_reads_stand_ins_and_tells_what_is_found_beside() {
        let root = tempfile::tempdir().unwrap();
        let base = root.path().canonicalize().unwrap();
        let (include, common) = (base.join("include"), base.join("scene/common"));
        std::fs::create_dir_all(&include).unwrap();
        std::fs::create_dir_all(&common).unwrap();
        std::fs::write(include.join("types.h"), "").unwrap();
        std::fs::write(include.join("b.h"), "").unwrap();
        std::fs::write(common.join("a.h"), "#include \"b.h\"\n#include \"types.h\"\n").unwrap();
        std::fs::write(common.join("b.h"), "").unwrap();
        let unit = base.join("scene/unit.c");
        std::fs::write(&unit, "#include \"common/a.h\"\n").unwrap();
        let search = std::slice::from_ref(&include);
        let graph = include_graph(&unit, search, &|_| None);
        assert_eq!(graph[&common.join("a.h")], [(common.join("b.h"), true), (include.join("types.h"), false)]);
        assert_eq!(reachable(&unit, search), [common.join("a.h"), common.join("b.h"), include.join("types.h")].into());
        // A stand-in's includes are the ones read.
        let stand_in = |path: &Path| (path == common.join("b.h")).then(|| "#include <b.h>\n".to_string());
        assert_eq!(include_graph(&unit, search, &stand_in)[&common.join("b.h")], [(include.join("b.h"), false)]);
    }

    #[test]
    fn library_files_name_headers_by_their_relative_path() {
        let root = tempfile::tempdir().unwrap();
        let base = root.path().canonicalize().unwrap();
        let include = base.join("games/common/include/system");
        let source = base.join("games/common/src/sound");
        std::fs::create_dir_all(&include).unwrap();
        std::fs::create_dir_all(&source).unwrap();
        std::fs::write(include.join("io_reg.h"), "").unwrap();
        let file = source.join("engine.c");
        std::fs::write(&file, "#include \"../../include/system/io_reg.h\"\n").unwrap();
        let header = include.join("io_reg.h");
        assert_eq!(spelling(&file, &header, &[]).as_deref(), Some("../../include/system/io_reg.h"));
        assert_eq!(spelling(&file, &header, std::slice::from_ref(&include)).as_deref(), Some("io_reg.h"));
        assert!(reachable(&file, &[]).contains(&header));
    }
}
