//! PERM macros, as pret's decomp-permuter reads them in a draft:
//! `PERM_GENERAL(a, b, ...)` and `PERM_ONEOF(a, b, ...)` choose one
//! alternative, `PERM_LINESWAP(lines)` chooses an order of its lines,
//! `PERM_RANDOMIZE(code)` limits random rewrites to that code and
//! `PERM_IGNORE(code)` passes its code through untouched. Only the permuter
//! expands them; a draft that still holds one is not C the build accepts.

use super::mutate::Rng;

const NAMES: &[&str] = &["PERM_GENERAL", "PERM_ONEOF", "PERM_LINESWAP", "PERM_RANDOMIZE", "PERM_IGNORE"];

#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Node {
    Text(String),
    OneOf(Vec<Vec<Node>>),
    LineSwap(Vec<Vec<Node>>),
    Randomize(Vec<Node>),
    Ignore(String),
}

/// One expansion: the C text with each ignored region as a placeholder
/// call, the placeholders' code, and the randomized regions' text.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Expanded {
    pub text: String,
    pub ignored: Vec<(String, String)>,
    pub randomize: Vec<String>,
}

impl Expanded {
    /// The text with every placeholder replaced by its code.
    pub fn restore(&self, text: &str) -> String {
        let mut out = text.to_string();
        for (placeholder, code) in &self.ignored {
            out = out.replace(placeholder, code);
            let call = placeholder.trim_end_matches(';');
            out = out.replace(call, code.trim_end_matches(';'));
        }
        out
    }
}

/// Byte index just past the parenthesis that closes the one at `open`,
/// skipping strings, characters and comments.
fn closing(text: &str, open: usize) -> Option<usize> {
    let bytes = text.as_bytes();
    let mut depth = 0usize;
    let mut index = open;
    while index < bytes.len() {
        match bytes[index] {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => {
                depth = depth.checked_sub(1)?;
                if depth == 0 {
                    return Some(index + 1);
                }
            }
            quote @ (b'"' | b'\'') => {
                index += 1;
                while index < bytes.len() && bytes[index] != quote {
                    index += if bytes[index] == b'\\' { 2 } else { 1 };
                }
            }
            b'/' if bytes.get(index + 1) == Some(&b'*') => {
                index = text[index + 2..].find("*/").map(|at| index + 2 + at + 1)?;
            }
            b'/' if bytes.get(index + 1) == Some(&b'/') => {
                index = text[index..].find('\n').map_or(bytes.len(), |at| index + at);
            }
            _ => {}
        }
        index += 1;
    }
    None
}

/// The next macro call at or after `from`, outside comments: (start, name).
fn next_macro(text: &str, from: usize) -> Option<(usize, &'static str)> {
    let bytes = text.as_bytes();
    let mut index = from;
    while index < bytes.len() {
        if text[index..].starts_with("/*") {
            index = text[index + 2..].find("*/").map_or(bytes.len(), |at| index + 2 + at + 2);
            continue;
        }
        if text[index..].starts_with("//") {
            index = text[index..].find('\n').map_or(bytes.len(), |at| index + at);
            continue;
        }
        if bytes[index] == b'"' || bytes[index] == b'\'' {
            let quote = bytes[index];
            index += 1;
            while index < bytes.len() && bytes[index] != quote {
                index += if bytes[index] == b'\\' { 2 } else { 1 };
            }
            index += 1;
            continue;
        }
        let boundary = index == 0 || !(bytes[index - 1].is_ascii_alphanumeric() || bytes[index - 1] == b'_');
        if boundary {
            for name in NAMES {
                if text[index..].starts_with(name) {
                    let after = &text[index + name.len()..];
                    let next = after.as_bytes().first().copied();
                    if !next.is_some_and(|c| c.is_ascii_alphanumeric() || c == b'_')
                        && after.trim_start().starts_with('(')
                    {
                        return Some((index, name));
                    }
                }
            }
        }
        index += 1;
    }
    None
}

/// Whether `text` still uses a PERM macro outside comments and strings.
pub fn has_macros(text: &str) -> bool {
    next_macro(text, 0).is_some()
}

/// Split a macro's arguments at top-level commas.
fn arguments(inner: &str) -> Vec<&str> {
    let bytes = inner.as_bytes();
    let (mut depth, mut start, mut out) = (0i32, 0, Vec::new());
    let mut index = 0;
    while index < bytes.len() {
        match bytes[index] {
            b'(' | b'[' | b'{' => depth += 1,
            b')' | b']' | b'}' => depth -= 1,
            b',' if depth == 0 => {
                out.push(&inner[start..index]);
                start = index + 1;
            }
            quote @ (b'"' | b'\'') => {
                index += 1;
                while index < bytes.len() && bytes[index] != quote {
                    index += if bytes[index] == b'\\' { 2 } else { 1 };
                }
            }
            b'/' if bytes.get(index + 1) == Some(&b'*') => {
                index = inner[index + 2..].find("*/").map_or(bytes.len(), |at| index + 2 + at + 1);
            }
            _ => {}
        }
        index += 1;
    }
    out.push(&inner[start..]);
    out
}

pub fn parse(text: &str) -> Result<Vec<Node>, String> {
    let mut nodes = Vec::new();
    let mut at = 0;
    while let Some((start, name)) = next_macro(text, at) {
        if start > at {
            nodes.push(Node::Text(text[at..start].to_string()));
        }
        let open = start + name.len() + text[start + name.len()..].find('(').expect("macro has (");
        let end = closing(text, open).ok_or_else(|| format!("{name}: unbalanced parentheses"))?;
        let inner = &text[open + 1..end - 1];
        nodes.push(match name {
            "PERM_GENERAL" | "PERM_ONEOF" => {
                Node::OneOf(arguments(inner).into_iter().map(|argument| parse(argument.trim())).collect::<Result<_, _>>()?)
            }
            "PERM_LINESWAP" => Node::LineSwap(
                inner
                    .lines()
                    .filter(|line| !line.trim().is_empty())
                    .map(|line| parse(&format!("{line}\n")))
                    .collect::<Result<_, _>>()?,
            ),
            "PERM_RANDOMIZE" => Node::Randomize(parse(inner)?),
            _ => Node::Ignore(inner.to_string()),
        });
        at = end;
    }
    if at < text.len() {
        nodes.push(Node::Text(text[at..].to_string()));
    }
    Ok(nodes)
}

/// Whether any macro offers a choice (alternatives or line orders).
pub fn has_choices(nodes: &[Node]) -> bool {
    nodes.iter().any(|node| match node {
        Node::OneOf(options) => options.len() > 1 || options.iter().any(|o| has_choices(o)),
        Node::LineSwap(lines) => lines.len() > 1 || lines.iter().any(|l| has_choices(l)),
        Node::Randomize(inner) => has_choices(inner),
        _ => false,
    })
}

pub fn randomizes(nodes: &[Node]) -> bool {
    nodes.iter().any(|node| match node {
        Node::Randomize(_) => true,
        Node::OneOf(options) | Node::LineSwap(options) => options.iter().any(|o| randomizes(o)),
        _ => false,
    })
}

/// Expand with the first alternative and the written line order, or with
/// choices drawn from `rng`.
pub fn expand(nodes: &[Node], mut rng: Option<&mut Rng>) -> Expanded {
    let mut out = Expanded::default();
    walk(nodes, &mut rng, &mut out);
    out
}

fn walk(nodes: &[Node], rng: &mut Option<&mut Rng>, out: &mut Expanded) {
    for node in nodes {
        let before = out.text.len();
        match node {
            Node::Text(text) => out.text.push_str(text),
            Node::OneOf(options) => {
                let pick = match rng {
                    Some(rng) => rng.below(options.len()),
                    None => 0,
                };
                walk(&options[pick], rng, out);
            }
            Node::LineSwap(lines) => {
                let mut order: Vec<usize> = (0..lines.len()).collect();
                if let Some(rng) = rng {
                    for i in (1..order.len()).rev() {
                        order.swap(i, rng.below(i + 1));
                    }
                }
                for index in order {
                    walk(&lines[index], rng, out);
                }
            }
            Node::Randomize(inner) => {
                walk(inner, rng, out);
                let region = out.text[before..].to_string();
                out.randomize.push(region);
            }
            Node::Ignore(code) => {
                let statement = matches!(code.trim_end().chars().last(), Some(';' | '}'));
                let placeholder = format!("__PERM_IGNORE_{}(){}", out.ignored.len(), if statement { ";" } else { "" });
                out.text.push_str(&placeholder);
                out.ignored.push((placeholder, code.trim().to_string()));
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const DRAFT: &str = "void f(int *p)\n{\n    PERM_LINESWAP(\n    p[0] = 1;\n    p[1] = 2;\n    )\n    p[2] = PERM_GENERAL(3, 4 + PERM_ONEOF(5, 6));\n    PERM_IGNORE(asm(\"\" ::: \"r5\");)\n    PERM_RANDOMIZE(p[3] = 7;)\n    /* PERM_GENERAL(x) in a comment stays */\n}\n";

    #[test]
    fn default_expansion_takes_first_choices_and_written_order() {
        let nodes = parse(DRAFT).unwrap();
        assert!(has_choices(&nodes) && randomizes(&nodes));
        let expanded = expand(&nodes, None);
        assert!(expanded.text.contains("    p[0] = 1;\n    p[1] = 2;\n"));
        assert!(expanded.text.contains("p[2] = 3;"));
        assert!(expanded.text.contains("__PERM_IGNORE_0();"));
        assert_eq!(expanded.randomize, ["p[3] = 7;"]);
        let restored = expanded.restore(&expanded.text);
        assert!(restored.contains("asm(\"\" ::: \"r5\");"));
        assert!(!has_macros(&restored));
        assert!(restored.contains("/* PERM_GENERAL(x) in a comment stays */"));
    }

    #[test]
    fn random_expansions_reach_every_choice_and_repeat_under_a_seed() {
        let nodes = parse(DRAFT).unwrap();
        let mut seen = std::collections::BTreeSet::new();
        let mut rng = Rng::new(7);
        for _ in 0..200 {
            seen.insert(expand(&nodes, Some(&mut rng)).text);
        }
        // Two line orders times three values.
        assert_eq!(seen.len(), 6);
        assert!(seen.iter().any(|text| text.contains("p[2] = 4 + 6;")));
        assert!(seen.iter().any(|text| text.contains("    p[1] = 2;\n    p[0] = 1;\n")));
        let (mut a, mut b) = (Rng::new(3), Rng::new(3));
        for _ in 0..20 {
            assert_eq!(expand(&nodes, Some(&mut a)), expand(&nodes, Some(&mut b)));
        }
    }

    #[test]
    fn macros_are_found_only_as_calls_outside_comments_and_strings() {
        assert!(has_macros("x = PERM_ONEOF(1, 2);"));
        assert!(!has_macros("/* PERM_ONEOF(1, 2) */ x = \"PERM_IGNORE(a)\";"));
        assert!(!has_macros("int PERM_GENERALLY(int);"));
        assert!(parse("PERM_GENERAL(a, (b").is_err());
    }
}
