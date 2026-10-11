//! What a C file or header declares and defines, read from its own text.

use crate::c_items::{declared, flat, Declared, Kind};

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum What {
    /// A function definition: its prototype's text, without `static`.
    Definition { prototype: String },
    /// A function declaration.
    Prototype,
    /// An `extern` object declaration.
    Extern,
    /// An object definition (tentative or initialized).
    Object,
    /// `#define NAME ((shape)NAME)`.
    ShapeMacro { shape: String },
    /// `#define NAME ...NAME...`, any other macro that names itself: the
    /// name under another type.
    ViewMacro,
    /// `#undef NAME`.
    Undef,
}

/// One declared or defined name at file scope.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Entry {
    pub name: String,
    pub what: What,
    pub local: bool,
    pub signature: String,
    /// The whole item's byte range; several declarators share one.
    pub start: usize,
    pub end: usize,
    pub declarators: usize,
    pub conditions: Vec<String>,
}

fn collapse(text: &str) -> String {
    text.split_whitespace().collect::<Vec<_>>().join(" ")
}

fn strip_comments(text: &str) -> String {
    let mask = crate::camelotize::source::mask(text);
    String::from_utf8_lossy(&mask).into_owned()
}

/// `#define NAME ((shape)NAME)` gives the name and the shape.
pub(crate) fn shape_macro(directive: &str) -> Option<(String, String)> {
    let joined = collapse(&directive.replace("\\\n", " "));
    let rest = joined.strip_prefix('#')?.trim_start().strip_prefix("define ")?;
    let (name, body) = rest.split_once(' ')?;
    if name.contains('(') {
        return None;
    }
    let inner = body.trim().strip_prefix("((")?.strip_suffix(')')?;
    let shape = inner.strip_suffix(name)?.trim_end().strip_suffix(')')?;
    (shape.contains("(*)") && !name.is_empty()).then(|| (name.to_string(), shape.to_string()))
}

/// `#define NAME ...NAME...`: an object-like macro that names itself.
pub(crate) fn self_macro(directive: &str) -> Option<String> {
    let joined = directive.replace("\\\n", " ");
    let rest = joined.trim_start().strip_prefix('#')?.trim_start().strip_prefix("define")?.trim_start();
    let end = rest.find(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))?;
    let (name, body) = rest.split_at(end);
    if body.starts_with('(') || name.is_empty() {
        return None;
    }
    let names_itself = body.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).any(|word| word == name);
    names_itself.then(|| name.to_string())
}

fn undef(directive: &str) -> Option<String> {
    let rest = directive.trim_start().strip_prefix('#')?.trim_start().strip_prefix("undef")?;
    let name = rest.split_whitespace().next()?;
    Some(name.to_string())
}

/// A definition's head as a prototype: comments out, spacing collapsed,
/// `static` and `inline` kept out of the text (the entry says `local`).
pub(crate) fn prototype(head: &str) -> String {
    let text = collapse(&strip_comments(head));
    // Macro calls before the definition (ones that define other functions)
    // are not part of it: the head starts after the last `)` before its
    // own parameter list.
    let bytes = text.as_bytes();
    let own = bytes.iter().rposition(|&b| b == b')').and_then(|close| crate::camelotize::source::opening(bytes, close));
    let text = match own.and_then(|open| text[..open].rfind(')')) {
        Some(before) => text[before + 1..].trim_start().to_string(),
        None => text,
    };
    let words: Vec<&str> = text.split(' ').filter(|w| !matches!(*w, "static" | "extern")).collect();
    format!("{};", words.join(" ").replace("( ", "(").replace(" )", ")"))
}

/// GNU C's `extern inline`: a body for inlining that defines no function.
fn inline_only(head: &str) -> bool {
    let has = |word: &str| head.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).any(|w| w == word);
    has("extern") && (has("inline") || has("__inline__") || has("__inline"))
}

pub(crate) fn entries(source: &str) -> Vec<Entry> {
    let mut out = Vec::new();
    for piece in flat(source) {
        let item = &piece.item;
        let mut push = |name: String, what: What, local: bool, signature: String, declarators: usize| {
            out.push(Entry { name, what, local, signature, start: item.start, end: item.end, declarators, conditions: piece.conditions.clone() });
        };
        match &item.kind {
            Kind::Directive => {
                if let Some((name, shape)) = shape_macro(item.text) {
                    push(name, What::ShapeMacro { shape }, true, String::new(), 1);
                } else if let Some(name) = self_macro(item.text) {
                    push(name, What::ViewMacro, true, String::new(), 1);
                } else if let Some(name) = undef(item.text) {
                    push(name, What::Undef, true, String::new(), 1);
                }
            }
            Kind::Function { body } => {
                if let Some(Declared { name, local, signature, .. }) = declared(item).into_iter().next() {
                    let head = &item.text[..*body];
                    push(name, What::Definition { prototype: prototype(head) }, local || inline_only(head), signature, 1);
                }
            }
            Kind::Declaration => {
                let all = declared(item);
                let count = all.len();
                for d in all {
                    if d.typedef {
                        continue;
                    }
                    let what = if d.function {
                        What::Prototype
                    } else if d.external && !d.initialized {
                        What::Extern
                    } else {
                        What::Object
                    };
                    push(d.name, what, d.local, d.signature, count);
                }
            }
            _ => {}
        }
    }
    out
}

/// Whether two tokens of a type are written with no space between them.
fn glued(a: &str, b: &str) -> bool {
    a == "("
        || matches!(b, ")" | "," | "[" | "]")
        || a == "["
        || (a == "*" && matches!(b, "*" | "(" | "(*)"))
        || (matches!(a, ")" | "(*)") && b == "(")
}

fn tidy(tokens: &[&str]) -> String {
    let mut out = String::new();
    for (i, token) in tokens.iter().enumerate() {
        if i > 0 && !glued(tokens[i - 1], token) {
            out.push(' ');
        }
        out.push_str(token);
    }
    out
}

/// A function's signature as the type of a pointer to it:
/// `s32 F ( s32 , u8 * )` gives `s32 (*)(s32, u8 *)`.
pub(crate) fn shape(signature: &str, name: &str) -> String {
    let mut named = false;
    let tokens: Vec<&str> = signature
        .split(' ')
        .filter(|t| !matches!(*t, "static" | "extern" | "inline" | "__inline" | "__inline__" | "auto" | "register" | ""))
        .map(|t| {
            if t == name && !named {
                named = true;
                "(*)"
            } else {
                t
            }
        })
        .collect();
    tidy(&tokens)
}

/// The type a call is cast to, worded as `shape` words a declaration.
pub(crate) fn cast_shape(cast: &str) -> Option<String> {
    let tokens = crate::compiler::c_syntax::lex(cast).ok()?;
    let texts: Vec<String> = tokens
        .iter()
        .filter_map(|t| match &t.tok {
            crate::compiler::c_syntax::Tok::Comment(_) => None,
            crate::compiler::c_syntax::Tok::Ident(s) | crate::compiler::c_syntax::Tok::Number(s) => Some(s.clone()),
            crate::compiler::c_syntax::Tok::Char(s) | crate::compiler::c_syntax::Tok::Str(s) => Some(s.clone()),
            crate::compiler::c_syntax::Tok::Punct(p) => Some(p.to_string()),
        })
        .collect();
    let at = (0..texts.len().saturating_sub(2)).find(|&i| texts[i] == "(" && texts[i + 1] == "*" && texts[i + 2] == ")")?;
    let text = format!("{} shape__ {};", texts[..at].join(" "), texts[at + 3..].join(" "));
    let item = crate::c_items::Item { start: 0, end: text.len(), text: &text, kind: Kind::Declaration };
    let found = declared(&item).into_iter().find(|d| d.name == "shape__" && d.function)?;
    Some(shape(&found.signature, "shape__"))
}

/// Whether a shape has a parameter list: `s32 (*)()` has none.
pub(crate) fn prototyped(shape: &str) -> bool {
    !shape.ends_with("(*)()")
}

/// Whether a shape is a function's; otherwise it is an object's, worded
/// the same way: `u8 (*)[]` for `u8 Name[]`.
pub(crate) fn is_function(shape: &str) -> bool {
    shape.split_once("(*)").is_some_and(|(_, after)| after.starts_with('('))
}

/// A function shape's return type and, when it has a parameter list, its
/// parameters' types.
pub(crate) fn call_parts(shape: &str) -> Option<(&str, Option<Vec<&str>>)> {
    let (returns, rest) = shape.split_once("(*)(")?;
    let inner = rest.strip_suffix(')')?;
    let mut parameters = Vec::new();
    let (mut depth, mut from) = (0usize, 0);
    for (i, byte) in inner.bytes().enumerate() {
        match byte {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth = depth.saturating_sub(1),
            b',' if depth == 0 => {
                parameters.push(inner[from..i].trim());
                from = i + 1;
            }
            _ => {}
        }
    }
    parameters.push(inner[from..].trim());
    let parameters = match parameters[..] {
        [""] => None,
        ["void"] => Some(Vec::new()),
        _ => Some(parameters),
    };
    Some((returns.trim_end(), parameters))
}

/// Whether a call compiles to the same code under two function shapes:
/// the same return type, or two pointers; and parameter lists of one
/// length whose types are the same or both a full word (a pointer, s32
/// or u32), which an argument reaches without code either way.
pub(crate) fn same_call(a: &str, b: &str) -> bool {
    let word = |t: &str| t.contains('*') || matches!(t, "s32" | "u32" | "int" | "unsigned int" | "unsigned" | "signed int");
    let (Some((a_returns, Some(a_parameters))), Some((b_returns, Some(b_parameters)))) = (call_parts(a), call_parts(b)) else {
        return false;
    };
    (a_returns == b_returns || (a_returns.contains('*') && b_returns.contains('*')))
        && a_parameters.len() == b_parameters.len()
        && a_parameters.iter().zip(&b_parameters).all(|(x, y)| x == y || (word(x) && word(y)))
}

/// A shape as two declarations are compared: an array's first bound,
/// which an `extern` may leave open, apart.
pub(crate) fn comparable(shape: &str) -> String {
    match shape.split_once("(*)[") {
        Some((before, after)) => format!("{before}(*)[{}", &after[after.find(']').unwrap_or(0)..]),
        None => shape.to_string(),
    }
}

/// A shape with its pointer written plainly where it can be:
/// `u8 (*)` as `u8 *`.
fn pointer(shape: &str) -> String {
    match shape.strip_suffix("(*)") {
        Some(before) => {
            let before = before.trim_end();
            format!("{before}{}*", if before.ends_with('*') { "" } else { " " })
        }
        None => shape.to_string(),
    }
}

/// What to write around a name to use it as the shape says: a function
/// as `((shape)Name)`, an object as `(*(type *)&Name)`, an array as
/// `(*(type (*)[])&Name)`. An array is not written as a pointer to its
/// element: an index through a pointer is summed otherwise than an array
/// reference, and the code differs.
pub(crate) fn view(shape: &str) -> (String, &'static str) {
    if is_function(shape) || !shape.contains("(*)") {
        (format!("(({shape})"), ")")
    } else {
        (format!("(*({})&", pointer(shape)), ")")
    }
}

/// The shape a view's cast gives the name: `(*(u32 *)&Name)` reads it as
/// `u32 (*)`, `((u8 *)&Name)` as the array `u8 (*)[]`.
pub(crate) fn view_shape(cast: &str, deref: bool) -> Option<String> {
    let tokens = crate::compiler::c_syntax::lex(cast).ok()?;
    let mut texts: Vec<String> = Vec::new();
    for token in &tokens {
        match &token.tok {
            crate::compiler::c_syntax::Tok::Comment(_) => {}
            crate::compiler::c_syntax::Tok::Ident(s) | crate::compiler::c_syntax::Tok::Number(s) => texts.push(s.clone()),
            crate::compiler::c_syntax::Tok::Punct(p) => texts.push(p.to_string()),
            _ => return None,
        }
    }
    let at = (0..texts.len().saturating_sub(2)).find(|&i| texts[i] == "(" && texts[i + 1] == "*" && texts[i + 2] == ")");
    match at {
        Some(at) => {
            texts.splice(at..at + 3, ["(*)".to_string()]);
        }
        None if texts.last().is_some_and(|last| last == "*") => {
            texts.pop();
            texts.push("(*)".to_string());
        }
        None => return None,
    }
    let shape = tidy(&texts.iter().map(String::as_str).collect::<Vec<_>>());
    Some(if deref { shape } else { format!("{shape}[]") })
}

/// How a function body names a name.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) enum How {
    /// `Name(...)`.
    Call,
    /// `((type)Name)(...)`: the type's text.
    Cast(String),
    /// `(*(type)&Name)` or `((type)&Name)`: the name under another type,
    /// read through the pointer or kept as the pointer.
    View { cast: String, deref: bool },
    /// Any other use of the name.
    Value,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Use {
    pub name: String,
    pub at: usize,
    pub how: How,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Scope {
    /// A function's prototype inside a body.
    Prototype,
    /// An `extern` object's declaration inside a body.
    Extern,
    /// A variable of the body, which hides a file's name of the same word.
    Variable,
}

/// A declaration inside a function body. It holds from `end` to `scope`,
/// its block's closing brace.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Local {
    pub name: String,
    pub kind: Scope,
    pub signature: String,
    pub start: usize,
    pub end: usize,
    pub scope: usize,
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Body {
    pub name: String,
    pub local: bool,
    pub signature: String,
    /// The definition's whole item, and its braces.
    pub start: usize,
    pub end: usize,
    pub open: usize,
    pub close: usize,
    pub locals: Vec<Local>,
    pub uses: Vec<Use>,
}

const STATEMENTS: &[&str] = &[
    "return", "if", "else", "while", "for", "do", "switch", "case", "goto", "break", "continue", "sizeof", "default", "typedef", "__asm__",
    "asm",
];

/// The declarations a statement starting at `at` makes and where it ends:
/// a function prototype (`type Name(...);`), an `extern`, or variables.
fn local_at(text: &str, mask: &[u8], at: usize, limit: usize) -> Option<(usize, Vec<(Declared, Scope)>)> {
    use crate::camelotize::source::{text as masked, word_at};
    let first = masked(mask, word_at(mask, at));
    if first.is_empty() || STATEMENTS.contains(&first) {
        return None;
    }
    let (mut depth, mut assigned) = (0usize, false);
    let mut end = None;
    for (i, &byte) in mask.iter().enumerate().take(limit).skip(at) {
        match byte {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth = depth.saturating_sub(1),
            // An initializer's braces; any other brace ends the statement.
            b'{' if assigned => depth += 1,
            b'}' if assigned && depth > 0 => depth -= 1,
            b'{' | b'}' => break,
            b'=' if depth == 0 => assigned = true,
            b';' if depth == 0 => {
                end = Some(i + 1);
                break;
            }
            _ => {}
        }
    }
    let end = end?;
    let item = crate::c_items::Item { start: at, end, text: &text[at..end], kind: Kind::Declaration };
    let code = masked(mask, at..end);
    if first == "extern" {
        let found: Vec<(Declared, Scope)> = declared(&item).into_iter().filter(|d| !d.typedef).map(|d| (d, Scope::Extern)).collect();
        return (!found.is_empty()).then_some((end, found));
    }
    if !assigned {
        // A prototype: type words, stars, the name, one parameter list, the semicolon.
        if let Some(open) = code.find('(') {
            let head = code[..open].trim_end();
            let name_at = head.rfind(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).map_or(0, |i| i + 1);
            let (types, name) = (head[..name_at].trim_end(), &head[name_at..]);
            let typed = types.contains(|c: char| c.is_ascii_alphabetic()) && types.chars().all(|c| c.is_ascii_alphanumeric() || c == '_' || c == '*' || c.is_ascii_whitespace());
            let whole = crate::camelotize::source::closing(mask, at + open).is_some_and(|close| masked(mask, close + 1..end).trim() == ";");
            if !name.is_empty() && typed && whole {
                let found: Vec<(Declared, Scope)> = declared(&item).into_iter().filter(|d| d.function && !d.typedef).map(|d| (d, Scope::Prototype)).collect();
                return (!found.is_empty()).then_some((end, found));
            }
        }
    }
    // Variables: type words, then a name before `;`, `=`, `,` or `[`.
    let words = |text: &str| text.chars().all(|c| c.is_ascii_alphanumeric() || c == '_' || c == '*' || c.is_ascii_whitespace());
    let stop = code.find([';', '=', ',', '[']).unwrap_or(code.len());
    let head = code[..stop].trim_end();
    let name_at = head.rfind(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).map_or(0, |i| i + 1);
    if name_at == 0 || !words(head) || !head[..name_at].contains(|c: char| c.is_ascii_alphabetic()) {
        return None;
    }
    let found: Vec<(Declared, Scope)> = declared(&item).into_iter().filter(|d| !d.function && !d.typedef).map(|d| (d, Scope::Variable)).collect();
    (!found.is_empty()).then_some((end, found))
}

fn how(text: &str, mask: &[u8], start: usize, end: usize) -> Option<How> {
    use crate::camelotize::source::{closing, next, opening, previous};
    let before = previous(mask, start);
    if before.is_some_and(|p| mask[p] == b'.' || (mask[p] == b'>' && p > 0 && mask[p - 1] == b'-')) {
        return None;
    }
    let after = next(mask, end);
    if after.is_some_and(|n| mask[n] == b'(') {
        return Some(How::Call);
    }
    if let (Some(p), Some(n)) = (before, after) {
        if mask[p] == b')' && mask[n] == b')' && next(mask, n + 1).is_some_and(|k| mask[k] == b'(') {
            if let Some(type_open) = opening(mask, p) {
                if previous(mask, type_open).is_some_and(|o| mask[o] == b'(' && closing(mask, o) == Some(n)) {
                    return Some(How::Cast(text[type_open + 1..p].to_string()));
                }
            }
        }
        // ( [*] ( type ) & Name )
        let cast_close = previous(mask, p).filter(|_| mask[p] == b'&' && mask[n] == b')');
        if let Some(type_open) = cast_close.filter(|&c| mask[c] == b')').and_then(|c| opening(mask, c)) {
            let cast = text[type_open + 1..cast_close.unwrap_or(type_open)].to_string();
            let outer = previous(mask, type_open);
            let deref = outer.is_some_and(|o| mask[o] == b'*');
            let open = if deref { outer.and_then(|o| previous(mask, o)) } else { outer };
            if open.is_some_and(|o| mask[o] == b'(' && closing(mask, o) == Some(n)) {
                return Some(How::View { cast, deref });
            }
        }
    }
    Some(How::Value)
}

/// What one body declares and names of the wanted names. `hidden` are
/// the names its parameters take.
fn body(text: &str, mask: &[u8], open: usize, close: usize, hidden: &[&str], wanted: &dyn Fn(&str) -> bool) -> (Vec<Local>, Vec<Use>) {
    use crate::camelotize::source::{closing, is_ident, word_at};
    let (mut locals, mut uses): (Vec<Local>, Vec<Use>) = (Vec::new(), Vec::new());
    let mut blocks = vec![open];
    let (mut parens, mut statement, mut i) = (0usize, true, open + 1);
    while i < close {
        let byte = mask[i];
        if byte.is_ascii_whitespace() {
            i += 1;
            continue;
        }
        if statement && parens == 0 {
            statement = false;
            if let Some((end, found)) = local_at(text, mask, i, close) {
                let scope = blocks.last().and_then(|&b| closing(mask, b)).unwrap_or(close).min(close);
                let variables = found.iter().all(|(_, kind)| *kind == Scope::Variable);
                // A variable holds from its own statement on, and its
                // initializer is read like any other code.
                let from = if variables { i } else { end };
                locals.extend(found.into_iter().map(|(d, kind)| Local { name: d.name, kind, signature: d.signature, start: i, end: from, scope }));
                if !variables {
                    i = end;
                    statement = true;
                    continue;
                }
            }
        }
        if is_ident(byte) && !byte.is_ascii_digit() && (i == 0 || !is_ident(mask[i - 1])) {
            let word = word_at(mask, i);
            let name = &text[word.clone()];
            let variable = hidden.contains(&name) || locals.iter().any(|l| l.kind == Scope::Variable && l.name == name && l.end <= word.start && word.start < l.scope);
            if wanted(name) && !variable {
                if let Some(how) = how(text, mask, word.start, word.end) {
                    uses.push(Use { name: name.to_string(), at: word.start, how });
                }
            }
            i = word.end;
            continue;
        }
        match byte {
            b'{' => {
                blocks.push(i);
                parens = 0;
                statement = true;
            }
            b'}' => {
                blocks.pop();
                parens = 0;
                statement = true;
            }
            b';' if parens == 0 => statement = true,
            b'(' => parens += 1,
            b')' => parens = parens.saturating_sub(1),
            _ => {}
        }
        i += 1;
    }
    locals.retain(|l| l.kind != Scope::Variable);
    (locals, uses)
}

/// Every function body of a text, with what it declares and where it
/// names the wanted names.
pub(crate) fn bodies(text: &str, wanted: &dyn Fn(&str) -> bool) -> Vec<Body> {
    let mask = crate::camelotize::source::mask(text);
    flat(text).iter().filter_map(|piece| body_of(text, &mask, &piece.item, wanted)).collect()
}

/// The body of one function definition item.
pub(crate) fn body_of(text: &str, mask: &[u8], item: &crate::c_items::Item, wanted: &dyn Fn(&str) -> bool) -> Option<Body> {
    let Kind::Function { body: offset } = item.kind else {
        return None;
    };
    let d = declared(item).into_iter().next()?;
    let open = item.start + offset;
    let close = mask[open..item.end].iter().rposition(|&b| b == b'}').map(|at| open + at)?;
    // A parameter hides a file's name of the same word.
    let head = &text[item.start..open];
    let hidden: Vec<&str> = head.split(|c: char| !(c.is_ascii_alphanumeric() || c == '_')).filter(|word| *word != d.name && wanted(word)).collect();
    let (locals, uses) = body(text, mask, open, close, &hidden, wanted);
    let local = d.local || inline_only(&item.text[..offset]);
    Some(Body { name: d.name, local, signature: d.signature, start: item.start, end: item.end, open, close, locals, uses })
}

/// The line ranges of a text's conditional branches: each from its `#if`,
/// `#elif` or `#else` line to the directive that ends it, both excluded.
pub(crate) fn arms(text: &str) -> Vec<(usize, usize)> {
    let mut found = Vec::new();
    let mut open: Vec<usize> = Vec::new();
    for (index, line) in text.lines().enumerate() {
        let Some(rest) = line.trim_start().strip_prefix('#') else {
            continue;
        };
        let rest = rest.trim_start();
        let word = &rest[..rest.find(|c: char| !c.is_ascii_alphanumeric()).unwrap_or(rest.len())];
        match word {
            "if" | "ifdef" | "ifndef" => open.push(index + 1),
            "elif" | "else" => {
                if let Some(start) = open.pop() {
                    found.push((start, index + 1));
                }
                open.push(index + 1);
            }
            "endif" => {
                if let Some(start) = open.pop() {
                    found.push((start, index + 1));
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
    fn a_file_lists_what_it_declares_and_defines() {
        let source = "#include \"a.h\"\nextern u8 gTable[];\ns32\t F(s32 a, struct P *p);\n#define G ((void (*)())G)\nstatic void H(void)\n{\n}\n#undef G\n#if defined(X)\nconst struct P *\nGet(s32 n) /* c */\n{\n\treturn 0;\n}\n#else\nu8 gCount = 1;\n#endif\n";
        let found: Vec<(String, What, bool, Vec<String>)> = entries(source).into_iter().map(|e| (e.name, e.what, e.local, e.conditions)).collect();
        assert_eq!(
            found,
            vec![
                ("gTable".into(), What::Extern, false, vec![]),
                ("F".into(), What::Prototype, false, vec![]),
                ("G".into(), What::ShapeMacro { shape: "void (*)()".into() }, true, vec![]),
                ("H".into(), What::Definition { prototype: "void H(void);".into() }, true, vec![]),
                ("G".into(), What::Undef, true, vec![]),
                ("Get".into(), What::Definition { prototype: "const struct P * Get(s32 n);".into() }, false, vec!["defined(X)".into()]),
                ("gCount".into(), What::Object, false, vec!["!(defined(X))".into()]),
            ]
        );
    }

    #[test]
    fn only_a_cast_of_the_name_to_a_function_shape_is_a_shape_macro() {
        assert_eq!(shape_macro("#define Run ((s32 (*)(s32, u8 *))Run)"), Some(("Run".into(), "s32 (*)(s32, u8 *)".into())));
        assert_eq!(shape_macro("#define Run \\\n\t((void (*)(void)) \\\n\t\tRun)"), Some(("Run".into(), "void (*)(void)".into())));
        assert_eq!(shape_macro("#define Run(x) ((s32 (*)(s32))Run)"), None);
        assert_eq!(shape_macro("#define Other ((s32 (*)(s32))Run)"), None);
        assert_eq!(shape_macro("#define COUNT ((s32)4)"), None);
    }

    #[test]
    fn a_shape_is_the_type_of_a_pointer_to_the_function() {
        assert_eq!(shape("s32 F ( s32 , u8 * )", "F"), "s32 (*)(s32, u8 *)");
        assert_eq!(shape("static const struct P * Get ( void )", "Get"), "const struct P *(*)(void)");
        assert_eq!(shape("void F ( void ( * ) ( s32 ) , s16 [ 4 ] )", "F"), "void (*)(void (*)(s32), s16[4])");
        assert_eq!(cast_shape("s32 * (*)(s32 index, struct P *p)").as_deref(), Some("s32 *(*)(s32, struct P *)"));
        assert_eq!(cast_shape("void (*)()").as_deref(), Some("void (*)()"));
        assert_eq!(cast_shape("u8 *"), None);
        assert!(prototyped("void (*)(void)") && !prototyped("s32 (*)()"));
    }

    #[test]
    fn a_call_is_the_same_where_only_word_sized_types_differ() {
        assert_eq!(call_parts("struct P *(*)(s32, void (*)(s32), u8 *)"), Some(("struct P *", Some(vec!["s32", "void (*)(s32)", "u8 *"]))));
        assert_eq!(call_parts("void (*)(void)"), Some(("void", Some(vec![]))));
        assert_eq!(call_parts("s32 (*)()"), Some(("s32", None)));
        assert!(same_call("void (*)(s32, s32, s32, s32)", "void (*)(s32, struct RenderInput *, u32, u32)"));
        assert!(same_call("struct BtlUnit *(*)(s32)", "void *(*)(u32)"));
        assert!(!same_call("s32 (*)(s32)", "u32 (*)(s32)"));
        assert!(!same_call("void (*)(s32, s32)", "void (*)(u32, u8)"));
        assert!(!same_call("void (*)()", "void (*)(s32)"));
        assert!(!same_call("void (*)(s32)", "void (*)(s32, s32)"));
    }

    #[test]
    fn an_object_is_viewed_through_a_pointer_to_its_type() {
        assert_eq!(shape("const u8 Table [ ]", "Table"), "const u8 (*)[]");
        assert!(is_function("void (*)(void)") && !is_function("const u8 (*)[]") && !is_function("void (*(*))(s32)"));
        assert_eq!(comparable("const u8 (*)[16]"), comparable("const u8 (*)[]"));
        assert_ne!(comparable("s16 (*)[4][2]"), comparable("s16 (*)[][3]"));
        assert_eq!(view("s32 (*)(s32)"), ("((s32 (*)(s32))".to_string(), ")"));
        assert_eq!(view("const u8 (*)[]"), ("(*(const u8 (*)[])&".to_string(), ")"));
        assert_eq!(view("s16 (*)[][2]"), ("(*(s16 (*)[][2])&".to_string(), ")"));
        assert_eq!(view("struct Work (*)"), ("(*(struct Work *)&".to_string(), ")"));
        assert_eq!(view("u8 *(*)"), ("(*(u8 **)&".to_string(), ")"));
        assert_eq!(view_shape("u32(*)", true).as_deref(), Some("u32 (*)"));
        assert_eq!(view_shape("volatile u16 *", true).as_deref(), Some("volatile u16 (*)"));
        assert_eq!(view_shape("u8(*)", false).as_deref(), Some("u8 (*)[]"));
        assert_eq!(view_shape("u8(*)[]", true).as_deref(), Some("u8 (*)[]"));
        assert_eq!(view_shape("s32", true), None);
    }

    #[test]
    fn a_body_lists_its_prototypes_and_how_it_names_functions() {
        let source = "void Run(s32 a)\n{\n\ts32 Local(s32);\n\textern u8 gWork[];\n\ts32 x = Other(a);\n\n\tif (a) {\n\t\tvoid Deep();\n\t\tDeep();\n\t}\n\tLocal(Other(1));\n\t((void (*)(s32 v))Cast)(x);\n\tp->Other = Value;\n\treturn Tail(a);\n}\n";
        let wanted = |name: &str| ["Local", "Other", "Deep", "Cast", "Value", "Tail", "gWork"].contains(&name);
        let found = bodies(source, &wanted);
        assert_eq!(found.len(), 1);
        let body = &found[0];
        assert_eq!((body.name.as_str(), body.local, &source[body.open..=body.close].len()), ("Run", false, &(source.len() - "void Run(s32 a)\n".len() - 1)));
        let locals: Vec<(&str, Scope, &str, &str)> = body.locals.iter().map(|l| (l.name.as_str(), l.kind, &source[l.start..l.end], &source[l.scope..l.scope + 1])).collect();
        assert_eq!(
            locals,
            vec![("Local", Scope::Prototype, "s32 Local(s32);", "}"), ("gWork", Scope::Extern, "extern u8 gWork[];", "}"), ("Deep", Scope::Prototype, "void Deep();", "}")]
        );
        assert!(body.locals[2].scope < body.locals[0].scope);
        let uses: Vec<(&str, How)> = body.uses.iter().map(|u| (u.name.as_str(), u.how.clone())).collect();
        assert_eq!(
            uses,
            vec![
                ("Other", How::Call),
                ("Deep", How::Call),
                ("Local", How::Call),
                ("Other", How::Call),
                ("Cast", How::Cast("void (*)(s32 v)".into())),
                ("Value", How::Value),
                ("Tail", How::Call),
            ]
        );
    }

    #[test]
    fn a_body_reads_views_and_its_own_variables() {
        let source = "void Run(s32 gParam)\n{\n\tstruct Work *work = gWork, gLocal[2] = { { gTable } };\n\n\tx = (*(u32(*)) & gCount) + ((u8 *)&gTable)[1] + &gTable + gLocal[0].a + gParam;\n\tif (x)\n\t{\n\t\ts32 gCount;\n\t\tgCount = gTable[0];\n\t}\n\tgCount++;\n}\n";
        let wanted = |name: &str| ["gWork", "gLocal", "gTable", "gCount", "gParam"].contains(&name);
        let found = bodies(source, &wanted);
        let uses: Vec<(&str, How)> = found[0].uses.iter().map(|u| (u.name.as_str(), u.how.clone())).collect();
        assert_eq!(
            uses,
            vec![
                ("gWork", How::Value),
                ("gTable", How::Value),
                ("gCount", How::View { cast: "u32(*)".into(), deref: true }),
                ("gTable", How::View { cast: "u8 *".into(), deref: false }),
                ("gTable", How::Value),
                ("gTable", How::Value),
                ("gCount", How::Value),
            ]
        );
        assert_eq!(found[0].locals, vec![]);
    }

    #[test]
    fn calls_are_not_prototypes() {
        let source = "void Run(void)\n{\n\tOuter(Inner(1));\n\telse Inner(2);\n\treturn Inner(3);\n\t*Inner(4);\n\tregister s32 pin asm(\"r4\");\n}\n";
        let found = bodies(source, &|name| name == "Inner");
        assert_eq!(found[0].locals, vec![]);
        assert_eq!(found[0].uses.len(), 4);
    }

    #[test]
    fn arms_are_the_lines_between_conditional_directives() {
        assert_eq!(arms("a\n#if X\nb\n# ifdef Y\nc\n# endif\n#else\nd\n#endif\n"), vec![(4, 6), (2, 7), (7, 9)]);
    }
}
