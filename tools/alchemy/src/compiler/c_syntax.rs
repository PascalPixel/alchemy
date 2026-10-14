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

/// A conditional directive in text read before preprocessing: `#if`,
/// `#ifdef` or `#ifndef` opens, `#elif` or `#else` starts another arm,
/// `#endif` closes.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Branch {
    If,
    Else,
    Endif,
}

/// Each conditional directive with the index of the token it stands before.
pub(crate) type Branches = Vec<(usize, Branch)>;

pub(crate) fn lex(source: &str) -> Result<Vec<Token>, String> {
    lex_branches(source).map(|(tokens, _)| tokens)
}

/// The tokens of C text and its conditional directives, each directive
/// with the index of the token it stands before.
pub(crate) fn lex_branches(source: &str) -> Result<(Vec<Token>, Branches), String> {
    let bytes = source.as_bytes();
    let mut tokens = Vec::new();
    let mut branches = Vec::new();
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
            let start = index + 1;
            while index < bytes.len() && bytes[index] != b'\n' {
                if bytes[index] == b'\\' && bytes.get(index + 1) == Some(&b'\n') {
                    line += 1;
                    index += 1;
                }
                index += 1;
            }
            let directive = source[start..index].trim_start();
            let name = directive
                .find(|c: char| !c.is_ascii_alphanumeric() && c != '_')
                .map_or(directive, |end| &directive[..end]);
            let branch = match name {
                "if" | "ifdef" | "ifndef" => Some(Branch::If),
                "elif" | "else" => Some(Branch::Else),
                "endif" => Some(Branch::Endif),
                _ => None,
            };
            if let Some(branch) = branch {
                branches.push((tokens.len(), branch));
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
    Ok((tokens, branches))
}

pub(crate) fn matching(tokens: &[Token], open: usize) -> Option<usize> {
    matching_in(tokens, open, &[])
}

/// The bracket that closes the one at `open`, read as the C preprocessor
/// reads one edition: each arm of a conditional opened inside starts at
/// the depth its `#if` had, so a brace opened in both arms of an
/// `#if`/`#else` counts once; and when `open` sits in an arm, the other
/// arms of that conditional are skipped to its `#endif`. `branches` come
/// from `lex_branches`; with none every bracket counts.
pub(crate) fn matching_in(tokens: &[Token], open: usize, branches: &[(usize, Branch)]) -> Option<usize> {
    let (opening, closing) = match tokens[open].tok {
        Tok::Punct("(") => ("(", ")"),
        Tok::Punct("{") => ("{", "}"),
        Tok::Punct("[") => ("[", "]"),
        _ => return None,
    };
    let mut depth = 0usize;
    // The depth at each `#if` opened after `open`.
    let mut arms: Vec<usize> = Vec::new();
    // While skipping the other arms of a conditional around `open`: the
    // conditionals opened inside them.
    let mut skipping: Option<usize> = None;
    let mut next = branches.partition_point(|(at, _)| *at <= open);
    for (index, token) in tokens.iter().enumerate().skip(open) {
        while let Some(&(_, branch)) = branches.get(next).filter(|(at, _)| *at <= index) {
            next += 1;
            match (skipping, branch) {
                (Some(nested), Branch::If) => skipping = Some(nested + 1),
                (Some(0), Branch::Endif) => skipping = None,
                (Some(nested), Branch::Endif) => skipping = Some(nested - 1),
                (Some(_), Branch::Else) => {}
                (None, Branch::If) => arms.push(depth),
                (None, Branch::Else) => match arms.last() {
                    Some(&at_if) => depth = at_if,
                    None => skipping = Some(0),
                },
                (None, Branch::Endif) => {
                    arms.pop();
                }
            }
        }
        if skipping.is_some() {
            continue;
        }
        match &token.tok {
            Tok::Punct(p) if *p == opening => depth += 1,
            Tok::Punct(p) if *p == closing => {
                depth = depth.checked_sub(1)?;
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
    scan_definitions_in(tokens, &[])
}

/// Each function definition at file scope: its name, the token after the
/// declaration before it, its name's token, its parameters' closing
/// parenthesis and its opening brace. Each arm of a conditional starts at
/// the brace depth its `#if` had, as `matching_in` reads them.
pub(crate) fn scan_definitions_in(
    tokens: &[Token],
    branches: &[(usize, Branch)],
) -> Vec<(String, usize, usize, usize, usize)> {
    let mut depth = 0usize;
    let mut boundary = 0usize;
    let mut found = Vec::new();
    let mut arms: Vec<usize> = Vec::new();
    let mut next = 0;
    for index in 0..tokens.len() {
        while let Some(&(_, branch)) = branches.get(next).filter(|(at, _)| *at <= index) {
            next += 1;
            match branch {
                Branch::If => arms.push(depth),
                Branch::Else => depth = arms.last().copied().unwrap_or(depth),
                Branch::Endif => {
                    arms.pop();
                }
            }
        }
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
                let Some(close) = matching_in(tokens, index + 1, branches) else {
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

    /// Each function and the source line of its closing brace.
    fn bodies(source: &str) -> Vec<(String, usize)> {
        let (tokens, branches) = lex_branches(source).unwrap();
        scan_definitions_in(&tokens, &branches)
            .into_iter()
            .map(|(name, _, _, _, open)| {
                (name, tokens[matching_in(&tokens, open, &branches).unwrap()].line)
            })
            .collect()
    }

    #[test]
    fn a_brace_opened_in_both_arms_counts_once() {
        // window.c's article writer: `if (...) {` in both arms.
        let source = "\
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
        assert_eq!(bodies(source), [("Article".into(), 13), ("After".into(), 18)]);
        let tokens = lex(source).unwrap();
        assert_eq!(scan_definitions(&tokens).len(), 1, "without the arms, the second brace hides After");
    }

    #[test]
    fn arms_of_different_length_and_elif_chains_each_start_at_the_if() {
        // menu.c's item command loop: other statements before the brace in
        // each arm, a nested conditional inside, and clear.c's #elif chain.
        let source = "\
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
\t\telse
\t\t{
\t\t\tstate = 2;
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
        assert_eq!(bodies(source), [("Loop".into(), 37), ("Next".into(), 41)]);
    }

    #[test]
    fn a_body_opened_in_an_arm_skips_the_other_arms() {
        let source = "\
#if GAME_TLA
void Pick(s32 a)
{
#else
void Pick(s32 a, s32 b)
{
#endif
\tPick_Run();
}
void Last(void)
{
}
";
        assert_eq!(
            bodies(source),
            [("Pick".into(), 9), ("Pick".into(), 9), ("Last".into(), 12)]
        );
    }
}
