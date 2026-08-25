//! Seven small source-shape mutations shared by the permuter.

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
            Self::InlineSingleUseTemp | Self::NameRepeatedSubexpression => "minimal live-variable form",
            Self::UnsignComparison => "unsigned comparison shape",
            Self::IndexToPointer => "addressing form",
            Self::SinkDeclarationToBlock => "declaration scope",
            Self::InvertToEarlyReturn => "branch polarity",
        }
    }
}

fn is_ident_start(byte: u8) -> bool {
    byte.is_ascii_alphabetic() || byte == b'_'
}

fn is_ident_continue(byte: u8) -> bool {
    is_ident_start(byte) || byte.is_ascii_digit()
}

fn skip_space(source: &str, mut at: usize) -> usize {
    while source.as_bytes().get(at).is_some_and(|byte| (*byte as char).is_ascii_whitespace()) {
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
        && (at + token.len() == source.len() || !is_ident_continue(source.as_bytes()[at + token.len()]))
}

fn replace_first_token(source: &str, token: &str, replacement: &str) -> Option<String> {
    let mut at = 0;
    while at + token.len() <= source.len() {
        if token_at(source, at, token) {
            return Some(format!("{}{}{}", &source[..at], replacement, &source[at + token.len()..]));
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
    let bound: String =
        args.iter().skip(1).enumerate().map(|(index, arg)| format!("s32 shape_arg{index} = {arg}; ")).collect();
    let call_args = std::iter::once(args[0].clone())
        .chain((0..args.len() - 1).map(|index| format!("shape_arg{index}")))
        .collect::<Vec<_>>()
        .join(", ");
    let replacement = format!("{{ {bound}{name}({call_args}); }}");
    Some(format!("{}{}{}", &source[..start], replacement, &source[end..]))
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
    let uses = (0..source.len()).filter(|at| token_at(source, *at, &name)).count();
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
    Some(format!("{}\n  s32 shape_common = {};{}", &source[..at], repeated, bound))
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
        while source.as_bytes().get(digit_end).is_some_and(u8::is_ascii_digit) {
            digit_end += 1;
        }
        if digit_end == digit_start || (digit_end < source.len() && is_ident_continue(source.as_bytes()[digit_end])) {
            continue;
        }
        return Some((at, name, operator.to_string(), source[digit_start..digit_end].to_string()));
    }
    None
}

fn apply_unsign_comparison(source: &str) -> Option<String> {
    let (at, name, operator, number) = first_comparison(source)?;
    let after = at + name.len();
    let operator_at = skip_space(source, after);
    let digit_at = skip_space(source, operator_at + operator.len());
    let mut digit_end = digit_at;
    while source.as_bytes().get(digit_end).is_some_and(u8::is_ascii_digit) {
        digit_end += 1;
    }
    Some(format!("{}(u32){name} {} {}u{}", &source[..at], operator, number, &source[digit_end..]))
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
    source[end..].find('{')?;
    let stripped = format!("{}{}", &source[..start], &source[end..]);
    let insert = first_function_open(&stripped)?;
    Some(format!("{}\n    {}{}", &stripped[..insert + 1], declaration.trim(), &stripped[insert + 1..]))
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
    Some(format!("{}if (!({condition})) return;\n{}{}", &source[..guard_at], body, &source[end + 1..]))
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
            Some(format!("{}*({} + {}){}", &source[..start], base.trim(), index, &source[end..]))
        }
        TransformId::SinkDeclarationToBlock => apply_sink_declaration_to_block(source),
        TransformId::InvertToEarlyReturn => apply_invert_to_early_return(source),
    }
}
