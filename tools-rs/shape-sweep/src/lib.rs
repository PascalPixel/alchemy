//! Native bounded source-shape search.
//!
//! The transforms are deliberately textual and deliberately small. They are
//! not a C rewriter: each one is a witnessed source shape from `LAWS.md`, and
//! a failed compile is simply an unproductive probe. The compiler and overlay
//! paths are shared native APIs so a shape result is measured by the same
//! byte-exact machinery used by the rest of the repository.

use std::fs;
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};

use alchemy_routing::routing::{root, CompilerTarget};
use candidate_compiler::{verify_candidate, CandidateCompilerConfiguration, ROM_BASE};
use overlay_disasm::{assemble_overlay, compile_overlay_candidate, OverlaySource, OVERLAY_BASE};

pub const USAGE: &str = "usage: shape-sweep <candidate.c> [--pairs] [--top N] [--self-test]";

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TransformId {
    HoistArgsAfterFirst,
    InlineSingleUseTemp,
    NameRepeatedSubexpression,
    UnsignComparison,
    IndexToPointer,
    SinkDeclarationToBlock,
    InvertToEarlyReturn,
}

impl TransformId {
    pub const ALL: [Self; 7] = [
        Self::HoistArgsAfterFirst,
        Self::InlineSingleUseTemp,
        Self::NameRepeatedSubexpression,
        Self::UnsignComparison,
        Self::IndexToPointer,
        Self::SinkDeclarationToBlock,
        Self::InvertToEarlyReturn,
    ];

    pub const fn id(self) -> &'static str {
        match self {
            Self::HoistArgsAfterFirst => "hoist-args-after-first",
            Self::InlineSingleUseTemp => "inline-single-use-temp",
            Self::NameRepeatedSubexpression => "name-repeated-subexpression",
            Self::UnsignComparison => "unsign-comparison",
            Self::IndexToPointer => "index-to-pointer",
            Self::SinkDeclarationToBlock => "sink-declaration-to-block",
            Self::InvertToEarlyReturn => "invert-to-early-return",
        }
    }

    pub const fn law(self) -> &'static str {
        match self {
            Self::HoistArgsAfterFirst => "argument materialisation order",
            Self::InlineSingleUseTemp | Self::NameRepeatedSubexpression => {
                "minimal live-variable form"
            }
            Self::UnsignComparison => "unsigned comparison shape",
            Self::IndexToPointer => "addressing form",
            Self::SinkDeclarationToBlock => "declaration scope",
            Self::InvertToEarlyReturn => "branch polarity",
        }
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Attempt {
    pub ids: Vec<TransformId>,
    pub source: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Row {
    pub ids: Vec<TransformId>,
    pub differing: usize,
    pub size: usize,
}

fn is_ident_start(byte: u8) -> bool {
    byte.is_ascii_alphabetic() || byte == b'_'
}

fn is_ident_continue(byte: u8) -> bool {
    is_ident_start(byte) || byte.is_ascii_digit()
}

fn skip_space(source: &str, mut at: usize) -> usize {
    while source
        .as_bytes()
        .get(at)
        .is_some_and(|byte| (*byte as char).is_ascii_whitespace())
    {
        at += 1;
    }
    at
}

fn identifier_at(source: &str, at: usize) -> Option<(String, usize)> {
    let bytes = source.as_bytes();
    if !is_ident_start(*bytes.get(at)?) {
        return None;
    }
    if at > 0 && is_ident_continue(bytes[at - 1]) {
        return None;
    }
    let mut end = at + 1;
    while end < bytes.len() && is_ident_continue(bytes[end]) {
        end += 1;
    }
    Some((source[at..end].to_string(), end))
}

fn token_at(source: &str, at: usize, token: &str) -> bool {
    source.as_bytes().get(at..at + token.len()) == Some(token.as_bytes())
        && (at == 0 || !is_ident_continue(source.as_bytes()[at - 1]))
        && (at + token.len() == source.len()
            || !is_ident_continue(source.as_bytes()[at + token.len()]))
}

fn replace_first_token(source: &str, token: &str, replacement: &str) -> Option<String> {
    let mut at = 0;
    while at + token.len() <= source.len() {
        if token_at(source, at, token) {
            return Some(format!(
                "{}{}{}",
                &source[..at],
                replacement,
                &source[at + token.len()..]
            ));
        }
        at += 1;
    }
    None
}

fn replace_all(source: &str, needle: &str, replacement: &str) -> String {
    source.replace(needle, replacement)
}

fn block_at(source: &str, open: usize) -> Option<(String, usize)> {
    if source.as_bytes().get(open) != Some(&b'{') {
        return None;
    }
    let mut depth = 0usize;
    for at in open..source.len() {
        match source.as_bytes()[at] {
            b'{' => depth += 1,
            b'}' => {
                depth = depth.checked_sub(1)?;
                if depth == 0 {
                    return Some((source[open + 1..at].to_string(), at));
                }
            }
            _ => {}
        }
    }
    None
}

fn split_arguments(text: &str) -> Option<Vec<String>> {
    let mut parts = Vec::new();
    let mut depth = 0i32;
    let mut start = 0usize;
    for (at, byte) in text.as_bytes().iter().copied().enumerate() {
        match byte {
            b'(' => depth += 1,
            b')' => depth -= 1,
            b',' if depth == 0 => {
                let part = text[start..at].trim();
                if part.is_empty() {
                    return None;
                }
                parts.push(part.to_string());
                start = at + 1;
            }
            _ => {}
        }
    }
    let last = text[start..].trim();
    if last.is_empty() {
        return None;
    }
    parts.push(last.to_string());
    Some(parts)
}

fn first_call(source: &str) -> Option<(usize, usize, String, String)> {
    let bytes = source.as_bytes();
    for at in 0..bytes.len() {
        let Some((name, after_name)) = identifier_at(source, at) else {
            continue;
        };
        let open = skip_space(source, after_name);
        if bytes.get(open) != Some(&b'(') {
            continue;
        }
        let mut close = open + 1;
        while close < bytes.len() && bytes[close] != b')' {
            if bytes[close] == b'(' || bytes[close] == b';' {
                close = bytes.len();
                break;
            }
            close += 1;
        }
        if close >= bytes.len() {
            continue;
        }
        let semicolon = skip_space(source, close + 1);
        if bytes.get(semicolon) != Some(&b';') {
            continue;
        }
        return Some((at, semicolon + 1, name, source[open + 1..close].to_string()));
    }
    None
}

fn apply_hoist_args_after_first(source: &str) -> Option<String> {
    let (start, end, name, args_text) = first_call(source)?;
    let args = split_arguments(&args_text)?;
    if args.len() < 2 {
        return Some(source.to_string());
    }
    let bound: String = args
        .iter()
        .skip(1)
        .enumerate()
        .map(|(index, arg)| format!("s32 shape_arg{index} = {arg}; "))
        .collect();
    let call_args = std::iter::once(args[0].clone())
        .chain((0..args.len() - 1).map(|index| format!("shape_arg{index}")))
        .collect::<Vec<_>>()
        .join(", ");
    let replacement = format!("{{ {bound}{name}({call_args}); }}");
    Some(format!(
        "{}{}{}",
        &source[..start],
        replacement,
        &source[end..]
    ))
}

fn typed_name_start(source: &str, at: usize, types: &[&str]) -> Option<(String, usize)> {
    let mut cursor = at;
    let type_name = types.iter().find(|type_name| {
        source.as_bytes().get(cursor..cursor + type_name.len()) == Some(type_name.as_bytes())
            && (cursor + type_name.len() == source.len()
                || source.as_bytes()[cursor + type_name.len()].is_ascii_whitespace())
    })?;
    cursor += type_name.len();
    cursor = skip_space(source, cursor);
    let (name, after) = identifier_at(source, cursor)?;
    Some((name, after))
}

fn first_inline_declaration(source: &str) -> Option<(usize, usize, String, String)> {
    let types = ["s32", "u32", "s16", "u16", "s8", "u8", "int"];
    for newline in source.match_indices('\n').map(|(at, _)| at) {
        let mut cursor = skip_space(source, newline + 1);
        let Some((name, after_name)) = typed_name_start(source, cursor, &types) else {
            continue;
        };
        cursor = skip_space(source, after_name);
        if source.as_bytes().get(cursor) != Some(&b'=') {
            continue;
        }
        cursor = skip_space(source, cursor + 1);
        let Some(relative_end) = source[cursor..].find(';') else {
            continue;
        };
        let end = cursor + relative_end;
        if end == cursor {
            continue;
        }
        let value = source[cursor..end].to_string();
        return Some((newline, end + 1, name, value));
    }
    None
}

fn apply_inline_single_use_temp(source: &str) -> Option<String> {
    let (start, end, name, value) = first_inline_declaration(source)?;
    let uses = (0..source.len())
        .filter(|at| token_at(source, *at, &name))
        .count();
    if uses != 2 {
        return None;
    }
    let without = format!("{}{}", &source[..start], &source[end..]);
    replace_first_token(&without, &name, &format!("({})", value.trim()))
}

fn first_index_expr(source: &str, from: usize) -> Option<(usize, usize, String)> {
    let bytes = source.as_bytes();
    for at in from..bytes.len() {
        let Some((_base, after_base)) = identifier_at(source, at) else {
            continue;
        };
        let open = skip_space(source, after_base);
        if bytes.get(open) != Some(&b'[') {
            continue;
        }
        let index_start = skip_space(source, open + 1);
        let Some((_index, after_index)) = identifier_at(source, index_start) else {
            continue;
        };
        let close = skip_space(source, after_index);
        if bytes.get(close) != Some(&b']') {
            continue;
        }
        return Some((at, close + 1, source[at..close + 1].to_string()));
    }
    None
}

fn apply_name_repeated_subexpression(source: &str) -> Option<String> {
    let mut counts: Vec<(String, usize)> = Vec::new();
    let mut cursor = 0;
    while let Some((_, end, expression)) = first_index_expr(source, cursor) {
        if let Some((_, count)) = counts.iter_mut().find(|(seen, _)| *seen == expression) {
            *count += 1;
        } else {
            counts.push((expression, 1));
        }
        cursor = end;
    }
    let repeated = counts.into_iter().find(|(_, count)| *count >= 2)?.0;
    let open = first_function_open(source)?;
    let at = open + 1;
    let bound = replace_all(&source[at..], &repeated, "shape_common");
    Some(format!(
        "{}\n  s32 shape_common = {};{}",
        &source[..at],
        repeated,
        bound
    ))
}

fn first_comparison(source: &str) -> Option<(usize, String, String, String)> {
    let bytes = source.as_bytes();
    for at in 0..bytes.len() {
        let Some((name, after)) = identifier_at(source, at) else {
            continue;
        };
        let operator_at = skip_space(source, after);
        let operator = if source[operator_at..].starts_with("<=") {
            "<="
        } else if source[operator_at..].starts_with(">=") {
            ">="
        } else if source.as_bytes().get(operator_at) == Some(&b'<') {
            "<"
        } else if source.as_bytes().get(operator_at) == Some(&b'>') {
            ">"
        } else {
            continue;
        };
        let digit_start = skip_space(source, operator_at + operator.len());
        let mut digit_end = digit_start;
        while source
            .as_bytes()
            .get(digit_end)
            .is_some_and(u8::is_ascii_digit)
        {
            digit_end += 1;
        }
        if digit_end == digit_start
            || (digit_end < source.len() && is_ident_continue(source.as_bytes()[digit_end]))
        {
            continue;
        }
        return Some((
            at,
            name,
            operator.to_string(),
            source[digit_start..digit_end].to_string(),
        ));
    }
    None
}

fn apply_unsign_comparison(source: &str) -> Option<String> {
    let (at, name, operator, number) = first_comparison(source)?;
    let after = at + name.len();
    let operator_at = skip_space(source, after);
    let digit_at = skip_space(source, operator_at + operator.len());
    let mut digit_end = digit_at;
    while source
        .as_bytes()
        .get(digit_end)
        .is_some_and(u8::is_ascii_digit)
    {
        digit_end += 1;
    }
    Some(format!(
        "{}(u32){name} {} {}u{}",
        &source[..at],
        operator,
        number,
        &source[digit_end..]
    ))
}

fn first_function_open(source: &str) -> Option<usize> {
    let bytes = source.as_bytes();
    for at in 0..bytes.len() {
        if bytes[at] != b')' {
            continue;
        }
        let open = skip_space(source, at + 1);
        if bytes.get(open) == Some(&b'{') {
            return Some(open);
        }
    }
    None
}

fn first_plain_declaration(source: &str, from: usize) -> Option<(usize, usize, String)> {
    let types = ["s32", "u32", "s16", "u16"];
    for newline in source[from..].match_indices('\n').map(|(at, _)| from + at) {
        let cursor = skip_space(source, newline + 1);
        let Some((_name, after_name)) = typed_name_start(source, cursor, &types) else {
            continue;
        };
        let end = skip_space(source, after_name);
        if source.as_bytes().get(end) == Some(&b';') {
            return Some((newline, end + 1, source[newline..end + 1].to_string()));
        }
    }
    None
}

fn apply_sink_declaration_to_block(source: &str) -> Option<String> {
    let opening = first_function_open(source)?;
    let (start, end, declaration) = first_plain_declaration(source, opening + 1)?;
    if source[end..].find('{').is_none() {
        return None;
    }
    let stripped = format!("{}{}", &source[..start], &source[end..]);
    let insert = first_function_open(&stripped)?;
    Some(format!(
        "{}\n    {}{}",
        &stripped[..insert + 1],
        declaration.trim(),
        &stripped[insert + 1..]
    ))
}

fn first_if_guard(source: &str) -> Option<(usize, String, usize)> {
    let bytes = source.as_bytes();
    for at in 0..bytes.len() {
        if !token_at(source, at, "if") {
            continue;
        }
        let open_paren = skip_space(source, at + 2);
        if bytes.get(open_paren) != Some(&b'(') {
            continue;
        }
        let close = source[open_paren + 1..].find(')')? + open_paren + 1;
        let open_brace = skip_space(source, close + 1);
        if bytes.get(open_brace) != Some(&b'{') {
            continue;
        }
        return Some((at, source[open_paren + 1..close].to_string(), open_brace));
    }
    None
}

fn apply_invert_to_early_return(source: &str) -> Option<String> {
    let (guard_at, condition, open) = first_if_guard(source)?;
    let (body, end) = block_at(source, open)?;
    if !source[end + 1..].trim_start().starts_with('}') {
        return None;
    }
    Some(format!(
        "{}if (!({condition})) return;\n{}{}",
        &source[..guard_at],
        body,
        &source[end + 1..]
    ))
}

pub fn apply_transform(id: TransformId, source: &str) -> Option<String> {
    match id {
        TransformId::HoistArgsAfterFirst => apply_hoist_args_after_first(source),
        TransformId::InlineSingleUseTemp => apply_inline_single_use_temp(source),
        TransformId::NameRepeatedSubexpression => apply_name_repeated_subexpression(source),
        TransformId::UnsignComparison => apply_unsign_comparison(source),
        TransformId::IndexToPointer => {
            let (start, end, expression) = first_index_expr(source, 0)?;
            let (base, index) = expression.split_once('[')?;
            let index = index.strip_suffix(']')?.trim();
            Some(format!(
                "{}*({} + {}){}",
                &source[..start],
                base.trim(),
                index,
                &source[end..]
            ))
        }
        TransformId::SinkDeclarationToBlock => apply_sink_declaration_to_block(source),
        TransformId::InvertToEarlyReturn => apply_invert_to_early_return(source),
    }
}

pub fn plan(source: &str, pairs: bool) -> Vec<Attempt> {
    let mut singles = Vec::new();
    for id in TransformId::ALL {
        if let Some(mutated) = apply_transform(id, source) {
            if mutated != source {
                singles.push(Attempt {
                    ids: vec![id],
                    source: mutated,
                });
            }
        }
    }
    if !pairs {
        return singles;
    }
    let mut combined = Vec::new();
    for first in &singles {
        for id in TransformId::ALL {
            if first.ids.contains(&id) {
                continue;
            }
            if let Some(mutated) = apply_transform(id, &first.source) {
                if mutated != first.source {
                    let mut ids = first.ids.clone();
                    ids.push(id);
                    combined.push(Attempt {
                        ids,
                        source: mutated,
                    });
                }
            }
        }
    }
    singles.extend(combined);
    singles
}

pub fn differing_halfwords(actual: &[u8], expected: &[u8]) -> usize {
    let mut differing = actual.len().abs_diff(expected.len()) as f64 / 2.0;
    for at in (0..actual.len().min(expected.len())).step_by(2) {
        if at + 1 >= actual.len() || at + 1 >= expected.len() {
            break;
        }
        if u16::from_le_bytes([actual[at], actual[at + 1]])
            != u16::from_le_bytes([expected[at], expected[at + 1]])
        {
            differing += 1.0;
        }
    }
    differing.round() as usize
}

fn basename(path: &str) -> &str {
    path.trim_end_matches('/')
        .rsplit('/')
        .next()
        .unwrap_or(path)
}

fn stem(path: &str) -> String {
    let name = basename(path);
    name.strip_suffix(".c").unwrap_or(name).to_string()
}

fn overlay_name(stem: &str) -> Option<String> {
    let (prefix, address) = stem.split_once("_c_")?;
    if !prefix.starts_with("resource_")
        || prefix[9..].is_empty()
        || !prefix[9..].bytes().all(|b| b.is_ascii_hexdigit())
        || address.len() != 8
        || !address.bytes().all(|b| b.is_ascii_hexdigit())
    {
        return None;
    }
    Some(prefix.to_string())
}

struct TempDir {
    path: PathBuf,
}

impl TempDir {
    fn new() -> Result<Self, String> {
        let now = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .map_err(|error| error.to_string())?;
        let path = std::env::temp_dir().join(format!(
            "shape-sweep-{}-{}-{}",
            std::process::id(),
            now.as_nanos(),
            std::thread::current().name().unwrap_or("main")
        ));
        fs::create_dir(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Ok(Self { path })
    }

    fn path(&self) -> &Path {
        &self.path
    }
}

impl Drop for TempDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.path);
    }
}

fn subarray(data: &[u8], begin: i64, end: i64) -> Vec<u8> {
    let len = data.len() as i64;
    let resolve = |value: i64| -> usize {
        if value < 0 {
            (len + value).max(0).min(len) as usize
        } else {
            value.min(len) as usize
        }
    };
    let start = resolve(begin);
    let stop = resolve(end);
    if stop <= start {
        Vec::new()
    } else {
        data[start..stop].to_vec()
    }
}

fn parse_top(args: &[String]) -> isize {
    let Some(index) = args.iter().position(|arg| arg == "--top") else {
        return 12;
    };
    args.get(index + 1)
        .and_then(|value| value.parse::<isize>().ok())
        .filter(|value| *value != 0)
        .unwrap_or(12)
}

fn self_test() -> Result<(), String> {
    let call = "void f(void) {\n  g(a, b, c);\n}\n";
    let hoisted = apply_transform(TransformId::HoistArgsAfterFirst, call)
        .ok_or("hoist-args-after-first did not match")?;
    if !hoisted.contains("shape_arg0") || !hoisted.contains("g(a, shape_arg0, shape_arg1)") {
        return Err("hoist-args-after-first did not bind arguments after the first".to_string());
    }
    let single = "void f(void) {\n  s32 t = load();\n  use(t);\n}\n";
    let inlined = apply_transform(TransformId::InlineSingleUseTemp, single)
        .ok_or("inline-single-use-temp did not match")?;
    if inlined.contains("s32 t =") || !inlined.contains("use((load()))") {
        return Err("inline-single-use-temp did not remove the single-use temporary".to_string());
    }
    let twice = "void f(void) {\n  h(p[i]);\n  h(p[i]);\n}\n";
    let named = apply_transform(TransformId::NameRepeatedSubexpression, twice)
        .ok_or("name-repeated-subexpression did not match")?;
    if !named.contains("shape_common = p[i]") || named.contains("h(p[i])") {
        return Err("name-repeated-subexpression did not bind the repeat".to_string());
    }
    if apply_transform(TransformId::InlineSingleUseTemp, twice).is_some() {
        return Err("inline-single-use-temp fired on a multi-use temporary".to_string());
    }
    let bare = "void f(void) {\n  return;\n}\n";
    let fired: Vec<_> = TransformId::ALL
        .into_iter()
        .filter(|id| apply_transform(*id, bare).is_some())
        .collect();
    if !fired.is_empty() {
        return Err(format!("transforms fired on an empty body: {fired:?}"));
    }
    if plan(call, false).is_empty() || plan(call, true).len() < plan(call, false).len() {
        return Err("plan did not produce the expected singles and pairs".to_string());
    }
    if differing_halfwords(&[1, 0, 2, 0], &[1, 0, 3, 0]) != 1 {
        return Err("differing_halfwords miscounted".to_string());
    }
    println!(
        "shape_sweep self-test ok ({} transforms)",
        TransformId::ALL.len()
    );
    Ok(())
}

fn attempt_candidate(
    attempt: &Attempt,
    candidate: &str,
    overlay: Option<&str>,
    original: &Path,
    rom: &[u8],
    work: &TempDir,
) -> Result<Row, String> {
    let path = work.path().join(basename(candidate));
    fs::write(&path, &attempt.source).map_err(|error| error.to_string())?;
    if let Some(overlay) = overlay {
        let built = compile_overlay_candidate(&path, work.path(), overlay, Some(original), &[])?;
        let reference_path = root()
            .join("assets")
            .join("code")
            .join(format!("{overlay}_overlay.s"));
        let reference = assemble_overlay(&OverlaySource::path(reference_path), OVERLAY_BASE)?;
        let offset = built.address - OVERLAY_BASE;
        let expected = subarray(&reference, offset, offset + built.data.len() as i64);
        Ok(Row {
            ids: attempt.ids.clone(),
            differing: differing_halfwords(&built.data, &expected),
            size: built.data.len(),
        })
    } else {
        let verification = verify_candidate(
            &path.to_string_lossy(),
            rom,
            &work.path().to_string_lossy(),
            &[],
            ROM_BASE,
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration::default(),
        )?;
        Ok(Row {
            ids: attempt.ids.clone(),
            differing: differing_halfwords(&verification.actual, &verification.expected),
            size: verification.size as usize,
        })
    }
}

pub fn run(args: Vec<String>) -> Result<(), String> {
    if args.iter().any(|arg| arg == "--self-test") {
        return self_test();
    }
    let Some(candidate) = args.iter().find(|arg| !arg.starts_with("--")) else {
        return Err(USAGE.to_string());
    };
    let top = parse_top(&args);
    let root = root();
    let original = root.join(candidate);
    let source = fs::read_to_string(&original)
        .map_err(|error| format!("{}: {error}", original.display()))?;
    let attempts = plan(&source, args.iter().any(|arg| arg == "--pairs"));
    if attempts.is_empty() {
        println!("no transform matched this source; the shape axis is exhausted for it");
        return Ok(());
    }
    let rom = fs::read(root.join("roms").join("gs1-en.gba"))
        .map_err(|error| format!("roms/gs1-en.gba: {error}"))?;
    let work = TempDir::new()?;
    let candidate_stem = stem(candidate);
    let overlay = overlay_name(&candidate_stem);
    let mut rows = Vec::new();
    for attempt in &attempts {
        if let Ok(row) = attempt_candidate(
            attempt,
            candidate,
            overlay.as_deref(),
            &original,
            &rom,
            &work,
        ) {
            rows.push(row);
        }
    }
    rows.sort_by_key(|row| row.differing);
    println!(
        "candidate={candidate} attempted={} compiled={}",
        attempts.len(),
        rows.len()
    );
    let count = if top < 0 {
        rows.len().saturating_sub((-top) as usize)
    } else {
        (top as usize).min(rows.len())
    };
    for row in rows.iter().take(count) {
        let ids = row
            .ids
            .iter()
            .map(|id| id.id())
            .collect::<Vec<_>>()
            .join(" + ");
        println!("{:>4}hw size={:>4} {ids}", row.differing, row.size);
    }
    if rows.first().is_some_and(|row| row.differing == 0) {
        println!("EXACT: install this shape and re-run the owner's gate");
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes() {
        self_test().expect("shape sweep self-test");
    }

    #[test]
    fn transform_laws_are_named() {
        assert!(TransformId::ALL
            .iter()
            .all(|id| !id.id().is_empty() && !id.law().is_empty()));
    }

    #[test]
    fn spaced_index_expressions_keep_the_exact_repeat_shape() {
        let source = "void f(void) {\n  h(p [ i ]);\n  h(p [ i ]);\n}\n";
        let named = apply_transform(TransformId::NameRepeatedSubexpression, source)
            .expect("spaced index expression");
        assert!(named.contains("shape_common = p [ i ]"));
        assert!(!named.contains("h(p [ i ])"));
    }

    #[test]
    fn comparison_rewrite_starts_at_the_matched_identifier() {
        let source = "void f(void) {\n  use(value);\n  if (value > 3) return;\n}\n";
        let rewritten = apply_transform(TransformId::UnsignComparison, source).expect("comparison");
        assert!(rewritten.contains("if ((u32)value > 3u) return;"));
        assert!(rewritten.contains("use(value);"));
    }
}
