#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum Tok {
    Ident(String),
    Number(String),
    Char(String),
    Str(String),
    Punct(&'static str),
    Comment(String),
}

#[derive(Clone, Debug)]
pub(crate) struct Token {
    pub tok: Tok,
    pub line: usize,
}

const PUNCTUATORS: &[&str] = &[
    "...", "<<=", ">>=", "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "*=",
    "/=", "%=", "+=", "-=", "&=", "^=", "|=", "##", "[", "]", "(", ")", "{", "}", ".", "&", "*",
    "+", "-", "~", "!", "/", "%", "<", ">", "^", "|", "?", ":", ";", "=", ",", "#",
];

pub(crate) fn lex(source: &str) -> Result<Vec<Token>, String> {
    let bytes = source.as_bytes();
    let mut tokens = Vec::new();
    let mut index = 0;
    let mut line = 1;
    let mut line_start = true;
    while index < bytes.len() {
        let byte = bytes[index];
        if byte == b'\n' {
            line += 1;
            line_start = true;
            index += 1;
            continue;
        }
        if byte.is_ascii_whitespace() {
            index += 1;
            continue;
        }
        if line_start && byte == b'#' {
            while index < bytes.len() && bytes[index] != b'\n' {
                if bytes[index] == b'\\' && bytes.get(index + 1) == Some(&b'\n') {
                    line += 1;
                    index += 1;
                }
                index += 1;
            }
            continue;
        }
        line_start = false;
        let start = index;
        let token_line = line;
        let tok = if source[index..].starts_with("/*") {
            let end = source[index + 2..]
                .find("*/")
                .map(|at| index + 2 + at + 2)
                .ok_or_else(|| format!("line {line}: unterminated comment"))?;
            line += source[index..end].matches('\n').count();
            index = end;
            Tok::Comment(source[start..end].to_string())
        } else if source[index..].starts_with("//") {
            while index < bytes.len() && bytes[index] != b'\n' {
                index += 1;
            }
            Tok::Comment(source[start..index].to_string())
        } else if byte.is_ascii_alphabetic() || byte == b'_' {
            while index < bytes.len()
                && (bytes[index].is_ascii_alphanumeric() || bytes[index] == b'_')
            {
                index += 1;
            }
            Tok::Ident(source[start..index].to_string())
        } else if byte.is_ascii_digit()
            || (byte == b'.' && bytes.get(index + 1).is_some_and(u8::is_ascii_digit))
        {
            while index < bytes.len() {
                let c = bytes[index];
                let exponent = matches!(c, b'+' | b'-')
                    && matches!(bytes[index - 1], b'e' | b'E')
                    && !source[start..index].starts_with("0x")
                    && !source[start..index].starts_with("0X");
                if c.is_ascii_alphanumeric() || c == b'_' || c == b'.' || exponent {
                    index += 1;
                } else {
                    break;
                }
            }
            Tok::Number(source[start..index].to_string())
        } else if byte == b'\'' || byte == b'"' {
            index += 1;
            while index < bytes.len() && bytes[index] != byte {
                if bytes[index] == b'\\' {
                    index += 1;
                }
                if bytes.get(index) == Some(&b'\n') {
                    return Err(format!("line {line}: unterminated literal"));
                }
                index += 1;
            }
            if index >= bytes.len() {
                return Err(format!("line {line}: unterminated literal"));
            }
            index += 1;
            let text = source[start..index].to_string();
            if byte == b'\'' {
                Tok::Char(text)
            } else {
                Tok::Str(text)
            }
        } else {
            let punct = PUNCTUATORS
                .iter()
                .find(|punct| source[index..].starts_with(**punct))
                .ok_or_else(|| {
                    format!(
                        "line {line}: unexpected character {:?}",
                        source[index..].chars().next().unwrap_or(' ')
                    )
                })?;
            index += punct.len();
            Tok::Punct(punct)
        };
        tokens.push(Token {
            tok,
            line: token_line,
        });
    }
    Ok(tokens)
}

pub(crate) fn matching(tokens: &[Token], open: usize) -> Option<usize> {
    let (opening, closing) = match tokens[open].tok {
        Tok::Punct("(") => ("(", ")"),
        Tok::Punct("{") => ("{", "}"),
        Tok::Punct("[") => ("[", "]"),
        _ => return None,
    };
    let mut depth = 0usize;
    for (index, token) in tokens.iter().enumerate().skip(open) {
        match &token.tok {
            Tok::Punct(p) if *p == opening => depth += 1,
            Tok::Punct(p) if *p == closing => {
                depth -= 1;
                if depth == 0 {
                    return Some(index);
                }
            }
            _ => {}
        }
    }
    None
}

pub(crate) fn scan_definitions(tokens: &[Token]) -> Vec<(String, usize, usize, usize, usize)> {
    let mut depth = 0usize;
    let mut boundary = 0usize;
    let mut found = Vec::new();
    for index in 0..tokens.len() {
        match &tokens[index].tok {
            Tok::Punct("{") => depth += 1,
            Tok::Punct("}") => {
                depth = depth.saturating_sub(1);
                if depth == 0 {
                    boundary = index + 1;
                }
            }
            Tok::Punct(";") if depth == 0 => boundary = index + 1,
            Tok::Ident(ident) if depth == 0 => {
                if !matches!(tokens.get(index + 1).map(|t| &t.tok), Some(Tok::Punct("("))) {
                    continue;
                }
                let Some(close) = matching(tokens, index + 1) else {
                    continue;
                };
                let open = tokens[close + 1..]
                    .iter()
                    .position(|token| !matches!(token.tok, Tok::Comment(_)))
                    .map(|at| close + 1 + at);
                if let Some(open) = open.filter(|open| {
                    matches!(tokens[*open].tok, Tok::Punct("{"))
                        && !tokens[boundary..index]
                            .iter()
                            .any(|token| matches!(token.tok, Tok::Punct("=")))
                }) {
                    found.push((ident.clone(), boundary, index, close, open));
                }
            }
            _ => {}
        }
    }
    found
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tokens_skip_directives_and_keep_comments() {
        let tokens =
            lex("#include \"a.h\"\n#define X \\\n  1\nint a = 0x1FFu; /* c */ a <<= 2;\n").unwrap();
        let kinds: Vec<Tok> = tokens.into_iter().map(|token| token.tok).collect();
        assert_eq!(
            kinds,
            vec![
                Tok::Ident("int".into()),
                Tok::Ident("a".into()),
                Tok::Punct("="),
                Tok::Number("0x1FFu".into()),
                Tok::Punct(";"),
                Tok::Comment("/* c */".into()),
                Tok::Ident("a".into()),
                Tok::Punct("<<="),
                Tok::Number("2".into()),
                Tok::Punct(";"),
            ]
        );
    }
}
