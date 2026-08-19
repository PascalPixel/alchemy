use std::collections::BTreeMap;

pub(crate) const IGNORE_START: &str = "/*__alchemy_permuter_ignore_start__*/";
pub(crate) const IGNORE_END: &str = "/*__alchemy_permuter_ignore_end__*/";
pub(crate) const PRETEND_START: &str = "/*__alchemy_permuter_pretend_start__*/";
pub(crate) const PRETEND_END: &str = "/*__alchemy_permuter_pretend_end__*/";
pub(crate) const RANDOM_START: &str = "/*__alchemy_permuter_random_start__*/";
pub(crate) const RANDOM_END: &str = "/*__alchemy_permuter_random_end__*/";

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct EvalState {
    vars: BTreeMap<String, String>,
    once_choices: BTreeMap<String, usize>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
enum Node {
    Text(String),
    Combine(Vec<Node>),
    General(Vec<Node>),
    Once {
        id: usize,
        key: String,
        inner: Box<Node>,
    },
    Var {
        name: Box<Node>,
        value: Option<Box<Node>>,
    },
    LineSwap(Vec<Node>),
    Int {
        low: i64,
        high: i64,
    },
    Ignore(Box<Node>),
    Pretend(Box<Node>),
    Randomize(Box<Node>),
    SameLine(Box<Node>),
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Permutation {
    root: Node,
    count: usize,
    random: bool,
    once: BTreeMap<String, Vec<usize>>,
}

fn checked_add(left: usize, right: usize) -> Result<usize, String> {
    left.checked_add(right)
        .ok_or_else(|| "permutation count overflow".to_string())
}

fn checked_mul(left: usize, right: usize) -> Result<usize, String> {
    left.checked_mul(right)
        .ok_or_else(|| "permutation count overflow".to_string())
}

fn factorial(number: usize) -> Result<usize, String> {
    (1..=number).try_fold(1usize, checked_mul)
}

impl Node {
    fn count(&self) -> Result<usize, String> {
        match self {
            Self::Text(_)
            | Self::Ignore(_)
            | Self::Pretend(_)
            | Self::Randomize(_)
            | Self::SameLine(_) => match self {
                Self::Ignore(inner)
                | Self::Pretend(inner)
                | Self::Randomize(inner)
                | Self::SameLine(inner) => inner.count(),
                _ => Ok(1),
            },
            Self::Combine(parts) => parts
                .iter()
                .try_fold(1usize, |total, part| checked_mul(total, part.count()?)),
            Self::General(parts) => parts
                .iter()
                .try_fold(0usize, |total, part| checked_add(total, part.count()?)),
            Self::Once { inner, .. } => inner.count(),
            Self::Var { name, value } => {
                let mut count = name.count()?;
                if let Some(value) = value {
                    count = checked_mul(count, value.count()?)?;
                }
                Ok(count)
            }
            Self::LineSwap(lines) => {
                let variants = lines
                    .iter()
                    .try_fold(1usize, |total, line| checked_mul(total, line.count()?))?;
                checked_mul(factorial(lines.len())?, variants)
            }
            Self::Int { low, high } => usize::try_from(high - low + 1)
                .map_err(|_| "PERM_INT range is too large".to_string()),
        }
    }

    fn is_random(&self) -> bool {
        matches!(self, Self::Randomize(_))
            || match self {
                Self::Combine(parts) | Self::General(parts) | Self::LineSwap(parts) => {
                    parts.iter().any(Self::is_random)
                }
                Self::Once { inner, .. }
                | Self::Ignore(inner)
                | Self::Pretend(inner)
                | Self::SameLine(inner) => inner.is_random(),
                Self::Var { name, value } => {
                    name.is_random() || value.as_deref().is_some_and(Self::is_random)
                }
                _ => false,
            }
    }

    fn collect_once(&self, found: &mut BTreeMap<String, Vec<usize>>) {
        match self {
            Self::Once { id, key, inner } => {
                found.entry(key.clone()).or_default().push(*id);
                inner.collect_once(found);
            }
            Self::Combine(parts) | Self::General(parts) | Self::LineSwap(parts) => {
                for part in parts {
                    part.collect_once(found);
                }
            }
            Self::Var { name, value } => {
                name.collect_once(found);
                if let Some(value) = value {
                    value.collect_once(found);
                }
            }
            Self::Ignore(inner)
            | Self::Pretend(inner)
            | Self::Randomize(inner)
            | Self::SameLine(inner) => inner.collect_once(found),
            Self::Text(_) | Self::Int { .. } => {}
        }
    }

    fn evaluate(&self, seed: usize, state: &mut EvalState) -> Result<String, String> {
        match self {
            Self::Text(text) => Ok(text.replace("(,)", ",")),
            Self::Combine(parts) => {
                let mut seed = seed;
                let mut output = String::new();
                for part in parts {
                    let count = part.count()?;
                    let sub_seed = seed % count;
                    seed /= count;
                    output.push_str(&part.evaluate(sub_seed, state)?);
                }
                Ok(output)
            }
            Self::General(parts) => {
                let mut seed = seed;
                for part in parts {
                    let count = part.count()?;
                    if seed < count {
                        return part.evaluate(seed, state);
                    }
                    seed -= count;
                }
                Err("PERM_GENERAL seed outside its range".into())
            }
            Self::Once { id, key, inner } => {
                if state.once_choices.get(key) == Some(id) {
                    inner.evaluate(seed, state)
                } else {
                    Ok(String::new())
                }
            }
            Self::Var { name, value } => {
                let name_count = name.count()?;
                let key = name.evaluate(seed % name_count, state)?.trim().to_string();
                let remaining = seed / name_count;
                if let Some(value) = value {
                    let expansion = value.evaluate(remaining, state)?;
                    state.vars.insert(key, expansion);
                    Ok(String::new())
                } else {
                    state
                        .vars
                        .get(&key)
                        .cloned()
                        .ok_or_else(|| format!("undefined PERM_VAR {key}"))
                }
            }
            Self::LineSwap(lines) => {
                let permutations = factorial(lines.len())?;
                let variation = seed % permutations;
                let mut sub_seed = seed / permutations;
                let mut rendered = Vec::with_capacity(lines.len());
                for line in lines {
                    let count = line.count()?;
                    rendered.push(line.evaluate(sub_seed % count, state)?);
                    sub_seed /= count;
                }
                let mut available = rendered;
                let mut order_seed = variation;
                let mut output = Vec::new();
                while !available.is_empty() {
                    let index = order_seed % available.len();
                    order_seed /= available.len();
                    output.push(available.remove(index));
                }
                Ok(output.join("\n"))
            }
            Self::Int { low, .. } => Ok((low + seed as i64).to_string()),
            Self::Ignore(inner) => Ok(format!(
                "{IGNORE_START}{}{IGNORE_END}",
                inner.evaluate(seed, state)?
            )),
            Self::Pretend(inner) => Ok(format!(
                "{PRETEND_START}{}{PRETEND_END}",
                inner.evaluate(seed, state)?
            )),
            Self::Randomize(inner) => Ok(format!(
                "{RANDOM_START}{}{RANDOM_END}",
                inner.evaluate(seed, state)?
            )),
            Self::SameLine(inner) => Ok(collapse_whitespace(&inner.evaluate(seed, state)?)),
        }
    }
}

impl Permutation {
    pub fn count(&self) -> usize {
        self.count
    }

    pub fn is_random(&self) -> bool {
        self.random
    }

    pub(crate) fn evaluate_marked(&self, mut seed: usize) -> Result<(String, EvalState), String> {
        if self.count == 0 {
            return Err("empty permutation".into());
        }
        seed %= self.count;
        let mut state = EvalState::default();
        for (key, choices) in &self.once {
            let choice = seed % choices.len();
            seed /= choices.len();
            state.once_choices.insert(key.clone(), choices[choice]);
        }
        let source = self.root.evaluate(seed, &mut state)?;
        Ok((source, state))
    }

    pub fn evaluate(&self, seed: usize) -> Result<(String, EvalState), String> {
        let (source, state) = self.evaluate_marked(seed)?;
        Ok((materialize(&source), state))
    }
}

fn remove_pretend(mut source: String) -> String {
    while let Some(start) = source.find(PRETEND_START) {
        let content = start + PRETEND_START.len();
        let Some(relative_end) = source[content..].find(PRETEND_END) else {
            break;
        };
        let end = content + relative_end + PRETEND_END.len();
        source.replace_range(start..end, "");
    }
    source
}

pub(crate) fn materialize(source: &str) -> String {
    remove_pretend(source.to_string())
        .replace(IGNORE_START, "")
        .replace(IGNORE_END, "")
        .replace(RANDOM_START, "")
        .replace(RANDOM_END, "")
}

#[derive(Clone, Copy, Debug, Eq, PartialEq)]
enum Lex {
    Normal,
    String,
    Character,
    LineComment,
    BlockComment,
}

fn find_macro(text: &str) -> Option<(usize, usize, String)> {
    let bytes = text.as_bytes();
    let mut state = Lex::Normal;
    let mut at = 0usize;
    while at < bytes.len() {
        match state {
            Lex::Normal => {
                if bytes.get(at..at + 2) == Some(b"//") {
                    state = Lex::LineComment;
                    at += 2;
                    continue;
                }
                if bytes.get(at..at + 2) == Some(b"/*") {
                    state = Lex::BlockComment;
                    at += 2;
                    continue;
                }
                if bytes[at] == b'"' {
                    state = Lex::String;
                    at += 1;
                    continue;
                }
                if bytes[at] == b'\'' {
                    state = Lex::Character;
                    at += 1;
                    continue;
                }
                if bytes.get(at..at + 5) == Some(b"PERM_")
                    && (at == 0
                        || (!(bytes[at - 1] as char).is_ascii_alphanumeric()
                            && bytes[at - 1] != b'_'))
                {
                    let mut end = at + 5;
                    while bytes
                        .get(end)
                        .is_some_and(|byte| byte.is_ascii_alphanumeric() || *byte == b'_')
                    {
                        end += 1;
                    }
                    if bytes.get(end) == Some(&b'(') {
                        return Some((at, end, text[at..end].to_string()));
                    }
                }
                at += 1;
            }
            Lex::String | Lex::Character => {
                let quote = if state == Lex::String { b'"' } else { b'\'' };
                if bytes[at] == b'\\' {
                    at = (at + 2).min(bytes.len());
                } else {
                    if bytes[at] == quote {
                        state = Lex::Normal;
                    }
                    at += 1;
                }
            }
            Lex::LineComment => {
                if bytes[at] == b'\n' {
                    state = Lex::Normal;
                }
                at += 1;
            }
            Lex::BlockComment => {
                if bytes.get(at..at + 2) == Some(b"*/") {
                    state = Lex::Normal;
                    at += 2;
                } else {
                    at += 1;
                }
            }
        }
    }
    None
}

fn consume_parenthesized(text: &str, open: usize) -> Result<(String, usize), String> {
    let bytes = text.as_bytes();
    let mut depth = 0usize;
    let mut state = Lex::Normal;
    let mut at = open + 1;
    while at < bytes.len() {
        match state {
            Lex::Normal => match bytes[at] {
                b'(' => {
                    depth += 1;
                    at += 1;
                }
                b')' if depth == 0 => return Ok((text[open + 1..at].to_string(), at + 1)),
                b')' => {
                    depth -= 1;
                    at += 1;
                }
                b'"' => {
                    state = Lex::String;
                    at += 1;
                }
                b'\'' => {
                    state = Lex::Character;
                    at += 1;
                }
                b'/' if bytes.get(at + 1) == Some(&b'/') => {
                    state = Lex::LineComment;
                    at += 2;
                }
                b'/' if bytes.get(at + 1) == Some(&b'*') => {
                    state = Lex::BlockComment;
                    at += 2;
                }
                _ => at += 1,
            },
            Lex::String | Lex::Character => {
                let quote = if state == Lex::String { b'"' } else { b'\'' };
                if bytes[at] == b'\\' {
                    at = (at + 2).min(bytes.len());
                } else {
                    if bytes[at] == quote {
                        state = Lex::Normal;
                    }
                    at += 1;
                }
            }
            Lex::LineComment => {
                if bytes[at] == b'\n' {
                    state = Lex::Normal;
                }
                at += 1;
            }
            Lex::BlockComment => {
                if bytes.get(at..at + 2) == Some(b"*/") {
                    state = Lex::Normal;
                    at += 2;
                } else {
                    at += 1;
                }
            }
        }
    }
    Err("unterminated PERM_* argument list".into())
}

fn split_top(text: &str, delimiter: u8) -> Result<Vec<String>, String> {
    let bytes = text.as_bytes();
    let mut parts = Vec::new();
    let mut depth = 0isize;
    let mut state = Lex::Normal;
    let mut start = 0usize;
    let mut at = 0usize;
    while at < bytes.len() {
        match state {
            Lex::Normal => match bytes[at] {
                b'(' | b'[' | b'{' => {
                    depth += 1;
                    at += 1;
                }
                b')' | b']' | b'}' => {
                    depth -= 1;
                    if depth < 0 {
                        return Err("mismatched delimiter in PERM_*".into());
                    }
                    at += 1;
                }
                b'"' => {
                    state = Lex::String;
                    at += 1;
                }
                b'\'' => {
                    state = Lex::Character;
                    at += 1;
                }
                b'/' if bytes.get(at + 1) == Some(&b'/') => {
                    state = Lex::LineComment;
                    at += 2;
                }
                b'/' if bytes.get(at + 1) == Some(&b'*') => {
                    state = Lex::BlockComment;
                    at += 2;
                }
                byte if byte == delimiter && depth == 0 => {
                    parts.push(text[start..at].to_string());
                    start = at + 1;
                    at += 1;
                }
                _ => at += 1,
            },
            Lex::String | Lex::Character => {
                let quote = if state == Lex::String { b'"' } else { b'\'' };
                if bytes[at] == b'\\' {
                    at = (at + 2).min(bytes.len());
                } else {
                    if bytes[at] == quote {
                        state = Lex::Normal;
                    }
                    at += 1;
                }
            }
            Lex::LineComment => {
                if bytes[at] == b'\n' {
                    state = Lex::Normal;
                    if delimiter == b'\n' && depth == 0 {
                        parts.push(text[start..at].to_string());
                        start = at + 1;
                    }
                }
                at += 1;
            }
            Lex::BlockComment => {
                if bytes.get(at..at + 2) == Some(b"*/") {
                    state = Lex::Normal;
                    at += 2;
                } else {
                    at += 1;
                }
            }
        }
    }
    if depth != 0 {
        return Err("mismatched delimiter in PERM_*".into());
    }
    parts.push(text[start..].to_string());
    Ok(parts)
}

fn push_source_char(text: &str, output: &mut String, at: &mut usize) {
    if let Some(character) = text[*at..].chars().next() {
        output.push(character);
        *at += character.len_utf8();
    }
}

fn collapse_whitespace(text: &str) -> String {
    let bytes = text.as_bytes();
    let mut output = String::with_capacity(text.len());
    let mut state = Lex::Normal;
    let mut pending_space = false;
    let mut at = 0usize;
    while at < bytes.len() {
        match state {
            Lex::Normal => {
                if bytes[at].is_ascii_whitespace() {
                    pending_space = true;
                    at += 1;
                    continue;
                }
                if pending_space && !output.is_empty() {
                    output.push(' ');
                }
                pending_space = false;
                if bytes.get(at..at + 2) == Some(b"//") {
                    state = Lex::LineComment;
                    output.push_str("//");
                    at += 2;
                } else if bytes.get(at..at + 2) == Some(b"/*") {
                    state = Lex::BlockComment;
                    output.push_str("/*");
                    at += 2;
                } else {
                    if bytes[at] == b'"' {
                        state = Lex::String;
                    } else if bytes[at] == b'\'' {
                        state = Lex::Character;
                    }
                    push_source_char(text, &mut output, &mut at);
                }
            }
            Lex::String | Lex::Character => {
                let quote = if state == Lex::String { b'"' } else { b'\'' };
                if bytes[at] == b'\\' && at + 1 < bytes.len() {
                    output.push('\\');
                    at += 1;
                    push_source_char(text, &mut output, &mut at);
                } else {
                    if bytes[at] == quote {
                        state = Lex::Normal;
                    }
                    push_source_char(text, &mut output, &mut at);
                }
            }
            Lex::LineComment => {
                if bytes[at] == b'\n' {
                    state = Lex::Normal;
                }
                push_source_char(text, &mut output, &mut at);
            }
            Lex::BlockComment => {
                if bytes.get(at..at + 2) == Some(b"*/") {
                    output.push_str("*/");
                    state = Lex::Normal;
                    at += 2;
                } else {
                    push_source_char(text, &mut output, &mut at);
                }
            }
        }
    }
    output
}

struct Parser {
    next_once: usize,
}

impl Parser {
    fn parse(&mut self, text: &str) -> Result<Node, String> {
        let mut remaining = text;
        let mut parts = Vec::new();
        while let Some((start, name_end, name)) = find_macro(remaining) {
            if start > 0 {
                parts.push(Node::Text(remaining[..start].to_string()));
            }
            let (arguments, end) = consume_parenthesized(remaining, name_end)?;
            parts.push(self.macro_node(&name, &arguments)?);
            remaining = &remaining[end..];
        }
        if !remaining.is_empty() || parts.is_empty() {
            parts.push(Node::Text(remaining.to_string()));
        }
        Ok(if parts.len() == 1 {
            parts.remove(0)
        } else {
            Node::Combine(parts)
        })
    }

    fn comma_nodes(&mut self, text: &str) -> Result<Vec<Node>, String> {
        split_top(text, b',')?
            .into_iter()
            .map(|part| self.parse(&part))
            .collect()
    }

    fn line_nodes(&mut self, text: &str) -> Result<Vec<Node>, String> {
        split_top(text, b'\n')?
            .into_iter()
            .filter(|line| !line.trim().is_empty())
            .map(|line| self.parse(&line))
            .collect()
    }

    fn macro_node(&mut self, name: &str, text: &str) -> Result<Node, String> {
        match name {
            "PERM_GENERAL" => Ok(Node::General(self.comma_nodes(text)?)),
            "PERM_INT" => {
                let parts = split_top(text, b',')?;
                if parts.len() != 2 {
                    return Err("PERM_INT takes two arguments".into());
                }
                let low = parts[0]
                    .trim()
                    .parse::<i64>()
                    .map_err(|_| "bad PERM_INT low")?;
                let high = parts[1]
                    .trim()
                    .parse::<i64>()
                    .map_err(|_| "bad PERM_INT high")?;
                if low > high {
                    return Err("PERM_INT low exceeds high".into());
                }
                Ok(Node::Int { low, high })
            }
            "PERM_LINESWAP" | "PERM_LINESWAP_TEXT" => Ok(Node::LineSwap(self.line_nodes(text)?)),
            "PERM_ONCE" => {
                let parts = split_top(text, b',')?;
                if !(1..=2).contains(&parts.len()) {
                    return Err("PERM_ONCE takes one or two arguments".into());
                }
                let key = parts[0].trim().to_string();
                let value = parts.last().expect("one argument exists");
                let id = self.next_once;
                self.next_once += 1;
                Ok(Node::Once {
                    id,
                    key,
                    inner: Box::new(self.parse(value)?),
                })
            }
            "PERM_VAR" => {
                let parts = split_top(text, b',')?;
                if !(1..=2).contains(&parts.len()) {
                    return Err("PERM_VAR takes one or two arguments".into());
                }
                Ok(Node::Var {
                    name: Box::new(self.parse(&parts[0])?),
                    value: if parts.len() == 2 {
                        Some(Box::new(self.parse(&parts[1])?))
                    } else {
                        None
                    },
                })
            }
            "PERM_IGNORE" => Ok(Node::Ignore(Box::new(self.parse(text)?))),
            "PERM_PRETEND" => Ok(Node::Pretend(Box::new(self.parse(text)?))),
            "PERM_RANDOMIZE" => Ok(Node::Randomize(Box::new(self.parse(text)?))),
            "PERM_FORCE_SAMELINE" => Ok(Node::SameLine(Box::new(self.parse(text)?))),
            _ => Err(format!("unrecognized permutation macro {name}")),
        }
    }
}

pub fn parse(source: &str) -> Result<Permutation, String> {
    let mut parser = Parser { next_once: 0 };
    let root = parser.parse(source)?;
    let mut once = BTreeMap::new();
    root.collect_once(&mut once);
    for (key, choices) in &once {
        if choices.len() == 1 {
            return Err(format!("PERM_ONCE({key}) occurs only once"));
        }
    }
    let mut count = root.count()?;
    for choices in once.values() {
        count = checked_mul(count, choices.len())?;
    }
    let random = root.is_random() || count == 1;
    Ok(Permutation {
        root,
        count,
        random,
        once,
    })
}

pub fn self_test() -> Result<(), String> {
    let general = parse("x=PERM_GENERAL(1,2,PERM_INT(3,4));")?;
    let values = (0..general.count())
        .map(|seed| general.evaluate(seed).map(|pair| pair.0))
        .collect::<Result<Vec<_>, _>>()?;
    if values != ["x=1;", "x=2;", "x=3;", "x=4;"] {
        return Err(format!("PERM_GENERAL/INT parity failed: {values:?}"));
    }
    let nested = parse("return PERM_GENERAL(1,PERM_GENERAL(100,101),3) + PERM_GENERAL(3,6,9);")?;
    if nested.count() != 12
        || !(0..nested.count())
            .map(|seed| nested.evaluate(seed).map(|value| value.0))
            .collect::<Result<Vec<_>, _>>()?
            .iter()
            .any(|source| source == "return 101 + 9;")
    {
        return Err("nested PERM_GENERAL mixed-radix evaluation drifted".into());
    }
    let swapped = parse("PERM_LINESWAP_TEXT(a();\nb();\nc();)")?;
    if swapped.count() != 6 || swapped.evaluate(0)?.0 == swapped.evaluate(1)?.0 {
        return Err("PERM_LINESWAP did not enumerate factorial orderings".into());
    }
    let once = parse("PERM_ONCE(k,a();) middle(); PERM_ONCE(k,a();)")?;
    if once.count() != 2 {
        return Err("PERM_ONCE choice count drifted".into());
    }
    for seed in 0..2 {
        if once.evaluate(seed)?.0.matches("a();").count() != 1 {
            return Err("PERM_ONCE did not emit exactly one occurrence".into());
        }
    }
    let vars = parse("PERM_VAR(v,hello)PERM_VAR(v)")?;
    if vars.evaluate(0)?.0 != "hello" {
        return Err("PERM_VAR round trip failed".into());
    }
    let ignored = parse("\"PERM_GENERAL(a,b)\" PERM_IGNORE(x(,);)")?;
    if ignored.count() != 1 || !ignored.evaluate(0)?.0.contains("PERM_GENERAL") {
        return Err("lexer treated string contents as permutation syntax".into());
    }
    let wrappers = parse("PERM_IGNORE(kept();) PERM_PRETEND(removed();) PERM_RANDOMIZE(live();)")?;
    let rendered = wrappers.evaluate(0)?.0;
    if !rendered.contains("kept();")
        || rendered.contains("removed();")
        || !rendered.contains("live();")
    {
        return Err("PERM_IGNORE/PRETEND/RANDOMIZE materialization drifted".into());
    }
    let lexical = parse("foo_PERM_GENERAL(a,b); PERM_GENERAL(call(1, 2), call(3, 4 /* , ) */));")?;
    if lexical.count() != 2 || !lexical.evaluate(1)?.0.contains("call(3, 4") {
        return Err("PERM parser split an identifier, nested comma, or comment".into());
    }
    let sameline = parse("PERM_FORCE_SAMELINE(a = \"two  spaces ☀️\";\n b = 2;)")?;
    let rendered = sameline.evaluate(0)?.0;
    if !rendered.contains("\"two  spaces ☀️\"") || rendered.contains(";\n b") {
        return Err("PERM_FORCE_SAMELINE damaged a string or retained source newlines".into());
    }
    Ok(())
}
