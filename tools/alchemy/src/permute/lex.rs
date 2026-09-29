//! C tokens for the permuter. Comments are kept as tokens so statements can
//! carry them; preprocessor lines are skipped, because the permuter edits the
//! function body as written and the compiler preprocesses every candidate.

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Tok {
    Ident(String),
    Number(String),
    Char(String),
    Str(String),
    Punct(&'static str),
    Comment(String),
}

#[derive(Clone, Debug)]
pub struct Token {
    pub tok: Tok,
    pub start: usize,
    pub end: usize,
    pub line: usize,
}

const PUNCTUATORS: &[&str] = &[
    "...", "<<=", ">>=", "->", "++", "--", "<<", ">>", "<=", ">=", "==", "!=", "&&", "||", "*=",
    "/=", "%=", "+=", "-=", "&=", "^=", "|=", "##", "[", "]", "(", ")", "{", "}", ".", "&", "*",
    "+", "-", "~", "!", "/", "%", "<", ">", "^", "|", "?", ":", ";", "=", ",", "#",
];

pub fn lex(source: &str) -> Result<Vec<Token>, String> {
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
            // A preprocessor line, with its continuations.
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
            start,
            end: index,
            line: token_line,
        });
    }
    Ok(tokens)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tokens_skip_directives_and_keep_comments() {
        let tokens =
            lex("#include \"A.H\"\n#define X \\\n  1\nint a = 0x1FFu; /* c */ a <<= 2;\n").unwrap();
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
