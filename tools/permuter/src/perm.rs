#[derive(Clone, Debug, Eq, PartialEq)]
enum Node {
    Text(String),
    Sequence(Vec<Node>),
    Choice(Vec<Node>),
    Integer { low: i64, high: i64 },
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Permutation {
    root: Node,
    count: usize,
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum Lex {
    Normal,
    String,
    Character,
    LineComment,
    BlockComment,
}

impl Node {
    fn count(&self) -> Result<usize, String> {
        match self {
            Self::Text(_) => Ok(1),
            Self::Sequence(parts) => parts.iter().try_fold(1usize, |total, part| {
                total
                    .checked_mul(part.count()?)
                    .ok_or_else(|| "permutation count overflow".to_string())
            }),
            Self::Choice(parts) => parts.iter().try_fold(0usize, |total, part| {
                total
                    .checked_add(part.count()?)
                    .ok_or_else(|| "permutation count overflow".to_string())
            }),
            Self::Integer { low, high } => {
                usize::try_from(i128::from(*high) - i128::from(*low) + 1)
                    .map_err(|_| "PERM_INT range is too large".to_string())
            }
        }
    }

    fn evaluate(&self, mut index: usize, output: &mut String) -> Result<(), String> {
        match self {
            Self::Text(text) => output.push_str(text),
            Self::Sequence(parts) => {
                for part in parts {
                    let count = part.count()?;
                    part.evaluate(index % count, output)?;
                    index /= count;
                }
            }
            Self::Choice(parts) => {
                for part in parts {
                    let count = part.count()?;
                    if index < count {
                        return part.evaluate(index, output);
                    }
                    index -= count;
                }
                return Err("PERM_GENERAL index is outside its range".into());
            }
            Self::Integer { low, .. } => output.push_str(
                &low.checked_add(
                    i64::try_from(index)
                        .map_err(|_| "PERM_INT index is outside its range".to_string())?,
                )
                .ok_or_else(|| "PERM_INT index is outside its range".to_string())?
                .to_string(),
            ),
        }
        Ok(())
    }
}

impl Permutation {
    pub fn count(&self) -> usize {
        self.count
    }

    pub fn evaluate(&self, index: usize) -> Result<String, String> {
        if index >= self.count {
            return Err(format!(
                "permutation index {index} exceeds count {}",
                self.count
            ));
        }
        let mut output = String::new();
        self.root.evaluate(index, &mut output)?;
        Ok(output)
    }
}

fn identifier(byte: u8) -> bool {
    byte == b'_' || byte.is_ascii_alphanumeric()
}

fn protected(bytes: &[u8], at: &mut usize, state: &mut Lex) -> bool {
    match *state {
        Lex::Normal if bytes.get(*at..*at + 2) == Some(b"//") => {
            *state = Lex::LineComment;
            *at += 2;
        }
        Lex::Normal if bytes.get(*at..*at + 2) == Some(b"/*") => {
            *state = Lex::BlockComment;
            *at += 2;
        }
        Lex::Normal if matches!(bytes[*at], b'"' | b'\'') => {
            *state = if bytes[*at] == b'"' {
                Lex::String
            } else {
                Lex::Character
            };
            *at += 1;
        }
        Lex::Normal => return false,
        Lex::String | Lex::Character => {
            let quote = if *state == Lex::String { b'"' } else { b'\'' };
            if bytes[*at] == b'\\' {
                *at = (*at + 2).min(bytes.len());
            } else {
                if bytes[*at] == quote {
                    *state = Lex::Normal;
                }
                *at += 1;
            }
        }
        Lex::LineComment => {
            if bytes[*at] == b'\n' {
                *state = Lex::Normal;
            }
            *at += 1;
        }
        Lex::BlockComment => {
            if bytes.get(*at..*at + 2) == Some(b"*/") {
                *state = Lex::Normal;
                *at += 2;
            } else {
                *at += 1;
            }
        }
    }
    true
}

fn find_macro(source: &str, from: usize) -> Option<(usize, usize, String)> {
    let bytes = source.as_bytes();
    let mut state = Lex::Normal;
    let mut at = from;
    while at < bytes.len() {
        if protected(bytes, &mut at, &mut state) {
            continue;
        }
        if bytes.get(at..at + 5) == Some(b"PERM_") && (at == 0 || !identifier(bytes[at - 1])) {
            let start = at;
            at += 5;
            while bytes.get(at).is_some_and(|byte| identifier(*byte)) {
                at += 1;
            }
            let name = source[start..at].to_string();
            while bytes.get(at).is_some_and(|byte| byte.is_ascii_whitespace()) {
                at += 1;
            }
            if bytes.get(at) == Some(&b'(') {
                return Some((start, at, name));
            }
        }
        at += 1;
    }
    None
}

fn macro_body(source: &str, open: usize) -> Result<(&str, usize), String> {
    let bytes = source.as_bytes();
    let mut state = Lex::Normal;
    let mut depth = 0;
    let mut at = open + 1;
    while at < bytes.len() {
        if protected(bytes, &mut at, &mut state) {
            continue;
        }
        match bytes[at] {
            b'(' => {
                depth += 1;
                at += 1;
            }
            b')' if depth == 0 => return Ok((&source[open + 1..at], at + 1)),
            b')' => {
                depth -= 1;
                at += 1;
            }
            _ => at += 1,
        }
    }
    Err("unterminated PERM_* argument list".into())
}

fn split_arguments(body: &str) -> Result<Vec<&str>, String> {
    let bytes = body.as_bytes();
    let mut state = Lex::Normal;
    let mut depths = [0usize; 3];
    let mut arguments = Vec::new();
    let mut start = 0;
    let mut at = 0;
    while at < bytes.len() {
        if protected(bytes, &mut at, &mut state) {
            continue;
        }
        match bytes[at] {
            b'(' | b'[' | b'{' => {
                let index = match bytes[at] {
                    b'(' => 0,
                    b'[' => 1,
                    _ => 2,
                };
                depths[index] += 1;
                at += 1;
            }
            b')' | b']' | b'}' => {
                let index = match bytes[at] {
                    b')' => 0,
                    b']' => 1,
                    _ => 2,
                };
                depths[index] = depths[index]
                    .checked_sub(1)
                    .ok_or_else(|| "mismatched delimiter in PERM_*".to_string())?;
                at += 1;
            }
            b',' if depths == [0, 0, 0] => {
                arguments.push(&body[start..at]);
                start = at + 1;
                at += 1;
            }
            _ => at += 1,
        }
    }
    if depths != [0, 0, 0] || !matches!(state, Lex::Normal | Lex::LineComment) {
        return Err("mismatched delimiter in PERM_*".into());
    }
    arguments.push(&body[start..]);
    Ok(arguments)
}

fn parse_fragment(source: &str) -> Result<Node, String> {
    let mut parts = Vec::new();
    let mut cursor = 0;
    while let Some((start, open, name)) = find_macro(source, cursor) {
        if start > cursor {
            parts.push(Node::Text(source[cursor..start].to_string()));
        }
        let (body, end) = macro_body(source, open)?;
        let arguments = split_arguments(body)?;
        let node = match name.as_str() {
            "PERM_GENERAL" => {
                if body.trim().is_empty() {
                    return Err("PERM_GENERAL requires at least one alternative".into());
                }
                Node::Choice(
                    arguments
                        .into_iter()
                        .map(parse_fragment)
                        .collect::<Result<Vec<_>, _>>()?,
                )
            }
            "PERM_INT" => {
                if arguments.len() != 2 {
                    return Err("PERM_INT takes two arguments".into());
                }
                let low = arguments[0]
                    .trim()
                    .parse::<i64>()
                    .map_err(|_| "bad PERM_INT low")?;
                let high = arguments[1]
                    .trim()
                    .parse::<i64>()
                    .map_err(|_| "bad PERM_INT high")?;
                if low > high {
                    return Err("PERM_INT low exceeds high".into());
                }
                Node::Integer { low, high }
            }
            _ => {
                return Err(format!(
                    "unsupported directive {name}; use PERM_GENERAL or PERM_INT"
                ))
            }
        };
        parts.push(node);
        cursor = end;
    }
    if cursor < source.len() || parts.is_empty() {
        parts.push(Node::Text(source[cursor..].to_string()));
    }
    Ok(if parts.len() == 1 {
        parts.pop().expect("one parsed part")
    } else {
        Node::Sequence(parts)
    })
}

pub fn parse(source: &str) -> Result<Permutation, String> {
    let root = parse_fragment(source)?;
    let count = root.count()?;
    Ok(Permutation { root, count })
}

pub fn self_test() -> Result<(), String> {
    let permutation =
        parse("return PERM_GENERAL(1,PERM_GENERAL(2,3),call(4,5)) + PERM_INT(-1,1);")?;
    if permutation.count() != 12
        || permutation.evaluate(0)? != "return 1 + -1;"
        || permutation.evaluate(11)? != "return call(4,5) + 1;"
    {
        return Err("nested mixed-radix permutation drifted".into());
    }
    let lexical =
        parse("const char *s = \"PERM_GENERAL(a,b)\"; /* PERM_INT(1,2) */ PERM_GENERAL(,x);")?;
    if lexical.count() != 2 || !lexical.evaluate(0)?.ends_with(" ;") {
        return Err("permutation lexer changed comments, strings, or empty alternatives".into());
    }
    if parse("PERM_LINESWAP(a,b)").is_ok()
        || parse("PERM_GENERAL()").is_ok()
        || parse("PERM_INT(2,1)").is_ok()
    {
        return Err("permutation parser accepted removed or invalid directives".into());
    }
    Ok(())
}
