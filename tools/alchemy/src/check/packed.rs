//! S1 and A1 for packed data: a decoder reads an asset the build packed,
//! never a C object written as numbers. A stream copied out as an array of
//! words is copied bytes however its file is named, and no count of
//! numbers finds it: the statue hall's sprite rows were 21 words.
//!
//! The rule reads maintained C under games/. A decoder is a routine whose
//! name says it unpacks what it is given (`DECODER_WORDS`), a function or
//! function-like macro that hands one of its parameters to a decoder, a
//! helper that is given a decoder to call, or a call through a pointer cast
//! to a decoder's type. No argument of such a call may name a C object
//! whose initializer is numbers alone, in this file or any other, nor a
//! table of pointers that leads to one, nor a string of escaped bytes.
//! An element taken by value (`Ids[n]` of a one-row table) is a number the
//! decoder is given, not the object, and an initializer of zeros alone is a
//! buffer.

use crate::compiler::c_syntax::{lex, matching, scan_definitions, Tok, Token};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};
use std::sync::{Arc, Mutex, OnceLock};

pub(super) const REASON: &str = "decoder reads a C object written as numbers (S1, A1): packed data is an asset the build packs; draw it as a picture or a table and .incbin its product";

/// A name with one of these in it says its routine unpacks what it is
/// given. 展開 (tenkai) is the game's own word for it.
const DECODER_WORDS: [&str; 4] = ["decod", "decompress", "unpack", "tenkai"];
/// How far a table of pointers is followed to the objects it names.
const TABLE_DEPTH: usize = 3;
/// Rounds in which a function that hands a parameter to a decoder makes
/// its own callers' arguments a decoder's.
const WRAPPER_ROUNDS: usize = 6;

fn decoder(name: &str) -> bool {
    let lower = name.to_ascii_lowercase();
    DECODER_WORDS.iter().any(|word| lower.contains(word))
}

#[derive(Clone, Debug, Default, PartialEq, Eq)]
struct Object {
    /// The `[...]` groups of its declarator.
    rank: usize,
    /// Visible to other files: defined at file scope and not `static`.
    external: bool,
    /// Its initializer is numbers alone, one of them not zero, or a string
    /// of escaped bytes.
    numbers: bool,
    /// The names its initializer holds.
    names: BTreeSet<String>,
}

/// One name in an argument a decoder is given.
#[derive(Clone, Debug, PartialEq, Eq)]
struct Mention {
    name: String,
    /// `&name`.
    address: bool,
    /// The `[...]` groups that follow it.
    subscripts: usize,
}

#[derive(Clone, Debug, Default)]
struct Function {
    name: String,
    parameters: Vec<String>,
    body: Vec<Token>,
}

/// What one C source defines and calls.
#[derive(Clone, Debug, Default)]
struct Facts {
    objects: BTreeMap<String, Object>,
    functions: Vec<Function>,
    /// Every name followed by `(`: a call, a prototype or a definition.
    callable: BTreeSet<String>,
}

type Wrappers = BTreeMap<String, BTreeSet<usize>>;

fn punct(token: Option<&Token>, text: &str) -> bool {
    matches!(token.map(|token| &token.tok), Some(Tok::Punct(p)) if *p == text)
}

fn ident(token: Option<&Token>) -> Option<&str> {
    match token.map(|token| &token.tok) {
        Some(Tok::Ident(name)) => Some(name),
        _ => None,
    }
}

/// The opening bracket that `close` closes.
fn matching_back(tokens: &[Token], close: usize) -> Option<usize> {
    let (opening, closing) = match tokens.get(close)?.tok {
        Tok::Punct(")") => ("(", ")"),
        Tok::Punct("]") => ("[", "]"),
        Tok::Punct("}") => ("{", "}"),
        _ => return None,
    };
    let mut depth = 0usize;
    for index in (0..=close).rev() {
        match &tokens[index].tok {
            Tok::Punct(p) if *p == closing => depth += 1,
            Tok::Punct(p) if *p == opening => {
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

fn zero(number: &str) -> bool {
    let digits = number.trim_end_matches(['u', 'U', 'l', 'L']);
    let digits = digits
        .strip_prefix("0x")
        .or_else(|| digits.strip_prefix("0X"))
        .unwrap_or(digits);
    !digits.is_empty() && digits.bytes().all(|byte| byte == b'0' || byte == b'.')
}

/// A string that spells bytes: four or more hex or octal escapes.
fn escaped_bytes(literal: &str) -> bool {
    let bytes = literal.as_bytes();
    (0..bytes.len().saturating_sub(1))
        .filter(|at| {
            bytes[*at] == b'\\'
                && (*at == 0 || bytes[*at - 1] != b'\\')
                && (bytes[*at + 1] == b'x' || bytes[*at + 1].is_ascii_digit())
        })
        .count()
        >= 4
}

/// Function-like macros written out as functions, so a macro that hands a
/// parameter to a decoder is read as the function it stands for.
fn macros_as_functions(text: &str) -> String {
    static DEFINE: OnceLock<regex::Regex> = OnceLock::new();
    let define = DEFINE.get_or_init(|| {
        regex::Regex::new(r"(?m)^[ \t]*#[ \t]*define[ \t]+([A-Za-z_][A-Za-z0-9_]*)\(([^()\n]*)\)(.*)$")
            .expect("function-like macro pattern")
    });
    let joined = text.replace("\\\r\n", " ").replace("\\\n", " ");
    let mut out = String::new();
    for found in define.captures_iter(&joined) {
        let parameters: Vec<String> = found[2]
            .split(',')
            .map(str::trim)
            .filter(|name| !name.is_empty() && *name != "...")
            .map(|name| format!("int {name}"))
            .collect();
        // `#` and `##` are no C tokens of a function.
        let body = found[3].replace('#', " ");
        out.push_str(&format!("\nvoid {}({}) {{ {body} ; }}\n", &found[1], parameters.join(", ")));
    }
    out
}

fn code(text: &str) -> Option<Vec<Token>> {
    let tokens = lex(text).ok()?;
    Some(tokens.into_iter().filter(|token| !matches!(token.tok, Tok::Comment(_))).collect())
}

fn facts(text: &str) -> Option<Facts> {
    let mut tokens = code(text)?;
    // A macro body that does not read as C is left out; the file's own
    // code still is read.
    if let Some(macros) = code(&macros_as_functions(text)) {
        let balanced = |open: &str, close: &str| {
            macros.iter().filter(|token| punct(Some(token), open)).count()
                == macros.iter().filter(|token| punct(Some(token), close)).count()
        };
        if balanced("(", ")") && balanced("{", "}") && balanced("[", "]") {
            tokens.extend(macros);
        }
    }
    let mut facts = Facts::default();
    for index in 0..tokens.len() {
        if let (Some(name), true) = (ident(tokens.get(index)), punct(tokens.get(index + 1), "(")) {
            facts.callable.insert(name.to_string());
        }
    }
    let mut depth = 0usize;
    let mut statement = 0usize;
    let mut index = 0;
    while index < tokens.len() {
        match &tokens[index].tok {
            Tok::Punct("{") => {
                depth += 1;
                statement = index + 1;
            }
            Tok::Punct("}") => {
                depth = depth.saturating_sub(1);
                statement = index + 1;
            }
            Tok::Punct(";") => statement = index + 1,
            Tok::Punct("=") => {
                let brace = punct(tokens.get(index + 1), "{");
                let string = matches!(tokens.get(index + 1).map(|t| &t.tok), Some(Tok::Str(_)));
                if brace || string {
                    let end = if brace { matching(&tokens, index + 1).unwrap_or(tokens.len() - 1) } else { index + 1 };
                    if let Some((name, rank)) = declarator(&tokens, statement, index) {
                        let local = tokens[statement..index].iter().any(|token| ident(Some(token)) == Some("static"));
                        if depth == 0 || local {
                            let body = &tokens[index + 1..=end];
                            let names: BTreeSet<String> =
                                body.iter().filter_map(|token| ident(Some(token)).map(str::to_string)).collect();
                            let bytes = body.iter().any(|token| matches!(&token.tok, Tok::Str(s) if escaped_bytes(s)));
                            let nonzero = body.iter().any(|token| match &token.tok {
                                Tok::Number(number) => !zero(number),
                                Tok::Char(_) => true,
                                _ => false,
                            });
                            let object = facts.objects.entry(name).or_default();
                            object.rank = object.rank.max(rank).max(usize::from(string));
                            object.external |= depth == 0 && !local;
                            object.numbers |= names.is_empty() && (nonzero || bytes);
                            object.names.extend(names);
                        }
                    }
                    if brace {
                        index = end;
                    }
                }
            }
            _ => {}
        }
        index += 1;
    }
    for (name, _, at, close, open) in scan_definitions(&tokens) {
        let Some(end) = matching(&tokens, open) else {
            continue;
        };
        let parameters = arguments(&tokens[at + 2..close])
            .into_iter()
            .filter_map(|group| group.iter().rev().find_map(|token| ident(Some(token))).map(str::to_string))
            .collect();
        facts.functions.push(Function { name, parameters, body: tokens[open..=end].to_vec() });
    }
    Some(facts)
}

/// The name an initializer's `=` at `equals` defines and the `[...]`
/// groups of its declarator, read back over them and over an attribute.
fn declarator(tokens: &[Token], statement: usize, equals: usize) -> Option<(String, usize)> {
    let mut at = equals.checked_sub(1)?;
    let mut rank = 0;
    loop {
        if at < statement {
            return None;
        }
        match &tokens[at].tok {
            Tok::Punct("]") => {
                at = matching_back(tokens, at)?.checked_sub(1)?;
                rank += 1;
            }
            // `name[] __attribute__((section("x"))) = {`
            Tok::Punct(")") => {
                let open = matching_back(tokens, at)?;
                let attribute = ident(tokens.get(open.checked_sub(1)?)).is_some_and(|name| name.starts_with("__attribute"));
                if !attribute {
                    return None;
                }
                at = open.checked_sub(2)?;
            }
            Tok::Ident(name) => return Some((name.clone(), rank)),
            _ => return None,
        }
    }
}

/// The arguments between a call's parentheses, cut at its own commas.
fn arguments(tokens: &[Token]) -> Vec<&[Token]> {
    let mut found = Vec::new();
    let (mut depth, mut start) = (0usize, 0usize);
    for (index, token) in tokens.iter().enumerate() {
        match &token.tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => depth = depth.saturating_sub(1),
            Tok::Punct(",") if depth == 0 => {
                found.push(&tokens[start..index]);
                start = index + 1;
            }
            _ => {}
        }
    }
    if start < tokens.len() {
        found.push(&tokens[start..]);
    }
    found
}

/// What the arguments a decoder is given name, and whether one of them is a
/// string of escaped bytes.
fn mentions(body: &[Token], wrappers: &Wrappers, callable: &BTreeSet<String>) -> (Vec<Mention>, bool) {
    let mut found = Vec::new();
    let mut bytes = false;
    for open in 1..body.len() {
        if !punct(body.get(open), "(") {
            continue;
        }
        let Some(close) = matching(body, open) else {
            continue;
        };
        let given = arguments(&body[open + 1..close]);
        let handed = |argument: &&[Token]| {
            matches!(argument, [only] if ident(Some(only)).is_some_and(|name| decoder(name) && callable.contains(name)))
        };
        let read: Vec<&[Token]> = match &body[open - 1].tok {
            Tok::Ident(name) if decoder(name) => given,
            // A helper that is handed the decoder to call.
            Tok::Ident(_) if given.iter().any(handed) => given.into_iter().filter(|argument| !handed(argument)).collect(),
            Tok::Ident(name) => match wrappers.get(name) {
                Some(indices) => indices.iter().filter_map(|index| given.get(*index).copied()).collect(),
                None => continue,
            },
            // A call through a pointer cast to a decoder's type.
            Tok::Punct(")") => {
                let Some(start) = matching_back(body, open - 1) else {
                    continue;
                };
                if !body[start..open].iter().any(|token| ident(Some(token)).is_some_and(decoder)) {
                    continue;
                }
                given
            }
            _ => continue,
        };
        for argument in read {
            for (index, token) in argument.iter().enumerate() {
                match &token.tok {
                    Tok::Str(literal) => bytes |= escaped_bytes(literal),
                    Tok::Ident(name) => {
                        let member = index > 0 && (punct(argument.get(index - 1), ".") || punct(argument.get(index - 1), "->"));
                        if member || punct(argument.get(index + 1), "(") {
                            continue;
                        }
                        let mut after = index + 1;
                        let mut subscripts = 0;
                        while punct(argument.get(after), "[") {
                            let Some(end) = matching(argument, after) else {
                                break;
                            };
                            subscripts += 1;
                            after = end + 1;
                        }
                        if punct(argument.get(after), ".") || punct(argument.get(after), "->") {
                            continue;
                        }
                        // `&name` and `(type)&name`, and not the `&` of
                        // `a & name`.
                        let address = index > 0
                            && punct(argument.get(index - 1), "&")
                            && !index.checked_sub(2).and_then(|before| argument.get(before)).is_some_and(|before| {
                                matches!(before.tok, Tok::Ident(_) | Tok::Number(_) | Tok::Punct("]"))
                            });
                        found.push(Mention { name: name.clone(), address, subscripts });
                    }
                    _ => {}
                }
            }
        }
    }
    (found, bytes)
}

/// Each function that hands a parameter to a decoder, with the positions
/// it hands on, until no round finds another.
fn wrappers<'a>(functions: impl Iterator<Item = &'a Function> + Clone, callable: &BTreeSet<String>, known: &Wrappers) -> Wrappers {
    let mut found = known.clone();
    for _ in 0..WRAPPER_ROUNDS {
        let mut grew = false;
        for function in functions.clone() {
            if function.parameters.is_empty() {
                continue;
            }
            let (named, _) = mentions(&function.body, &found, callable);
            for mention in named {
                if let Some(position) = function.parameters.iter().position(|parameter| *parameter == mention.name) {
                    grew |= found.entry(function.name.clone()).or_default().insert(position);
                }
            }
        }
        if !grew {
            break;
        }
    }
    found
}

/// The maintained C of a checkout's games/, read once.
#[derive(Default)]
struct Tree {
    files: BTreeMap<String, Facts>,
    callable: BTreeSet<String>,
    wrappers: Wrappers,
    /// The files that define each object other files can see.
    external: BTreeMap<String, Vec<String>>,
    /// Each object, by file and name, that a decoder call of another file
    /// reads as numbers or reaches them through.
    reached: BTreeSet<(String, String)>,
}

fn sources(directory: &Path, found: &mut Vec<PathBuf>) {
    let Ok(entries) = std::fs::read_dir(directory) else {
        return;
    };
    let mut paths: Vec<PathBuf> = entries.flatten().map(|entry| entry.path()).collect();
    paths.sort();
    for path in paths {
        if path.is_dir() {
            sources(&path, found);
        } else if path.extension().is_some_and(|extension| extension == "c" || extension == "h") {
            found.push(path);
        }
    }
}

/// The object a file's code means by a name, with the file that defines
/// it: the file's own, or another file's that is visible to it.
type Lookup<'a> = dyn Fn(&str, &str) -> Option<(String, Object)> + 'a;

/// The objects through which what a decoder is given is numbers: the
/// object written as numbers itself, behind any tables of pointers that
/// lead to it. Empty when the decoder reads none.
fn chain(mention: &Mention, from: &str, lookup: &Lookup, depth: usize) -> Vec<(String, String)> {
    let Some((file, found)) = lookup(&mention.name, from) else {
        return Vec::new();
    };
    let here = (file.clone(), mention.name.clone());
    if found.numbers {
        return if mention.address || mention.subscripts < found.rank { vec![here] } else { Vec::new() };
    }
    if depth == 0 || mention.subscripts > found.rank {
        return Vec::new();
    }
    for name in &found.names {
        let pointed = Mention { name: name.clone(), address: true, subscripts: 0 };
        let mut rest = chain(&pointed, &file, lookup, depth - 1);
        if !rest.is_empty() {
            rest.push(here);
            return rest;
        }
    }
    Vec::new()
}

fn read_tree(root: &Path) -> Tree {
    let mut tree = Tree::default();
    let mut paths = Vec::new();
    sources(&root.join("games"), &mut paths);
    for path in paths {
        let Ok(relative) = path.strip_prefix(root) else {
            continue;
        };
        let data = std::fs::read(&path).unwrap_or_default();
        let text = String::from_utf8_lossy(&data);
        if std::env::var_os("PACKED_DEBUG").is_some() {
            match code(&text) {
                None => eprintln!("UNLEXED {}", path.display()),
                Some(tokens) => {
                    let open = tokens.iter().filter(|token| punct(Some(token), "{")).count();
                    let close = tokens.iter().filter(|token| punct(Some(token), "}")).count();
                    if open != close {
                        eprintln!("UNBALANCED {} {open} {close}", path.display());
                    }
                }
            }
        }
        let Some(facts) = facts(&text) else {
            continue;
        };
        let relative = relative.to_string_lossy().replace('\\', "/");
        tree.callable.extend(facts.callable.iter().cloned());
        for (name, object) in &facts.objects {
            if object.external {
                tree.external.entry(name.clone()).or_default().push(relative.clone());
            }
        }
        tree.files.insert(relative, facts);
    }
    tree.wrappers = wrappers(tree.files.values().flat_map(|facts| &facts.functions), &tree.callable, &Wrappers::new());
    let lookup = |name: &str, from: &str| tree.object(name, from, None);
    let mut reached = BTreeSet::new();
    for (path, facts) in &tree.files {
        for function in &facts.functions {
            for mention in mentions(&function.body, &tree.wrappers, &tree.callable).0 {
                reached.extend(chain(&mention, path, &lookup, TABLE_DEPTH).into_iter().filter(|(file, _)| file != path));
            }
        }
    }
    tree.reached = reached;
    tree
}

impl Tree {
    /// The object `name` means in the file `from`. The file `own` names is
    /// read as the facts beside it, whatever the checkout holds there.
    fn object(&self, name: &str, from: &str, own: Option<(&str, &Facts)>) -> Option<(String, Object)> {
        let facts = |path: &str| match own {
            Some((mine, facts)) if mine == path => Some(facts),
            _ => self.files.get(path),
        };
        if let Some(found) = facts(from).and_then(|facts| facts.objects.get(name)) {
            return Some((from.to_string(), found.clone()));
        }
        let mine = own.filter(|(_, facts)| facts.objects.get(name).is_some_and(|object| object.external)).map(|(path, _)| path);
        mine.into_iter()
            .chain(self.external.get(name).into_iter().flatten().map(String::as_str))
            .filter(|path| *path != from)
            .find_map(|path| {
                let found = facts(path)?.objects.get(name).filter(|object| object.external)?;
                Some((path.to_string(), found.clone()))
            })
    }
}

fn tree(root: &Path) -> Arc<Tree> {
    static TREES: OnceLock<Mutex<BTreeMap<PathBuf, Arc<Tree>>>> = OnceLock::new();
    let mut trees = TREES.get_or_init(Default::default).lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    trees.entry(root.to_path_buf()).or_insert_with(|| Arc::new(read_tree(root))).clone()
}

/// The rule for the C source `text` at `path` of the checkout at `root`:
/// no decoder call of this file reads numbers, and no decoder call of
/// another file reads the numbers this file holds.
pub(super) fn reason(root: &Path, path: &str, text: &str) -> Option<&'static str> {
    let own = facts(text)?;
    if own.functions.is_empty() && !own.objects.values().any(|object| object.numbers) {
        return None;
    }
    let tree = tree(root);
    let mut callable = tree.callable.clone();
    callable.extend(own.callable.iter().cloned());
    let known = wrappers(own.functions.iter(), &callable, &tree.wrappers);
    let lookup = |name: &str, from: &str| tree.object(name, from, Some((path, &own)));
    for function in &own.functions {
        let (named, bytes) = mentions(&function.body, &known, &callable);
        if bytes || named.iter().any(|mention| !chain(mention, path, &lookup, TABLE_DEPTH).is_empty()) {
            return Some(REASON);
        }
    }
    // Another file's call: refused here while this file still holds the
    // numbers, or the table that leads to them.
    let held = |name: &String| own.objects.get(name).is_some_and(|object| object.numbers || !object.names.is_empty());
    tree.reached.iter().any(|(file, name)| file.as_str() == path && held(name)).then_some(REASON)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn checkout(files: &[(&str, &str)]) -> tempfile::TempDir {
        let temp = tempfile::tempdir().unwrap();
        for (path, text) in files {
            let path = temp.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        }
        temp
    }

    fn alone(text: &str) -> Option<&'static str> {
        let temp = checkout(&[]);
        reason(temp.path(), "games/x/src/a.c", text)
    }

    const STREAM: &str = "u8 Stream[] = { 0x41, 0x00, 0x02, 0x01, 0xc0, 0xcc, 0x0c, 0x00 };\n";

    #[test]
    fn a_decoder_reads_no_object_written_as_numbers() {
        for call in [
            "Resource_DecodeByteLz(Stream, buf);",
            "Resource_DecodeByteLz((s32)Stream, buf);",
            "Resource_DecodeType01((const u8 *)&Stream[0], buf);",
            "Resource_DecodeType01(Stream + 4, buf);",
            "value = Iwram_Decompress(Stream, buf);",
            "Resource_DecompressHalfwords(buf, Stream);",
            "Unpack(Stream);",
            "SentouKouka_Tenkai(Stream, vram, fill);",
            // a helper handed the decoder to call
            "Value2(Resource_DecodeType01, (s32)Stream, (s32)buf);",
            // a call through a pointer cast to a decoder's type
            "((BitDecoder)WORK_SLOT(49))(Stream + offset, vram);",
            "((PackedDecoder)((union HeapState *)gWorkSlot)->slots[49])(Stream, vram, fill);",
        ] {
            let text = format!("{STREAM}void Resource_DecodeType01();\nvoid f(void)\n{{\n\t{call}\n}}\n");
            assert_eq!(alone(&text), Some(REASON), "{call}");
        }
        // Defined after its use, static, inside the function, or as words.
        for text in [
            "void f(void) { Resource_DecodeByteLz(Stream, buf); }\nstatic const s32 Stream[] = { 0x1020041, 0xcccc0, -56540, 354 };\n",
            "void f(void) { static const u8 stream[] = { 1, 2, 3 }; Resource_DecodeByteLz(stream, buf); }\n",
            "const u8 Rows[2][4] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };\nvoid f(s32 n) { Resource_DecodeByteLz(Rows[n], buf); }\n",
            "const char Stream[] = \"\\x41\\x00\\x02\\x01\\xc0\";\nvoid f(void) { Resource_DecodeByteLz(Stream, buf); }\n",
            "void f(void) { Resource_DecodeByteLz(\"\\x41\\x00\\x02\\x01\\xc0\", buf); }\n",
        ] {
            assert_eq!(alone(text), Some(REASON), "{text}");
        }
    }

    #[test]
    fn a_table_of_pointers_and_a_wrapper_lead_to_the_numbers() {
        let table = format!(
            "{STREAM}const u8 *const Streams[] = {{ Built, Stream }};\nvoid f(s32 n) {{ Resource_DecodeByteLz(Streams[n], buf); }}\n"
        );
        assert_eq!(alone(&table), Some(REASON));
        // A function, and a macro, that hand a parameter on.
        for wrapper in [
            "static void Load(s32 slot, const void *source) { Resource_DecodeType01(source, gBuffer); }",
            "#define Load(slot, source) \\\n\tResource_DecodeType01((source), gBuffer)",
            "static void Inner(const u8 *from) { Resource_DecodeByteLz(from, gBuffer); }\nstatic void Load(s32 slot, const u8 *source) { Inner(source + 4); }",
        ] {
            let text = format!("{STREAM}{wrapper}\nvoid f(void) {{ Load(3, Stream); }}\n");
            assert_eq!(alone(&text), Some(REASON), "{wrapper}");
            // The position it hands on, and no other.
            let other = format!("{STREAM}{wrapper}\nvoid f(void) {{ Load(Stream[2], Built); }}\n");
            assert_eq!(alone(&other), None, "{wrapper}");
        }
    }

    #[test]
    fn tables_buffers_ids_and_built_assets_pass() {
        for text in [
            // An asset: declared here, defined by an .incbin elsewhere.
            "extern u8 Built[];\nvoid f(void) { Resource_DecodeByteLz((s32)Built, buf); }\n",
            // A resource of the directory.
            "void f(void) { Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Sheet), buf); }\n",
            // A table of resource numbers: the decoder is given one number.
            "const u16 Sheets[] = { 12, 13, 14 };\nvoid f(s32 n) { Resource_LoadAndDecompress(Sheets[n], buf, 1, 0); }\n",
            "const struct Pair Pairs[] = { { 12, 1 }, { 13, 2 } };\nvoid f(s32 n) { Resource_LoadAndDecompress(Pairs[n].sheet, buf, 1, 0); }\n",
            // A buffer of zeros, and a destination with names in it.
            "static u8 Buffer[64] = { 0 };\nvoid f(void) { Resource_DecodeByteLz(Built, Buffer); }\n",
            "s32 Work[] = { LIMIT, 0 };\nvoid f(void) { Resource_DecodeByteLz(Built, Work); }\n",
            // Numbers no decoder is given.
            "const u8 Widths[] = { 5, 4, 7, 7 };\nvoid f(s32 n) { Resource_DecodeByteLz(Built, buf); Draw(Widths, n); }\n",
            "const u8 Widths[] = { 5, 4, 7, 7 };\nvoid f(s32 n) { Resource_DecodeByteLz(Built + (n & Mask), buf); }\n",
            // A local the decoder is given is no object of the file.
            "void f(void) { u8 *data = Resource_GetTableEntry(3); Resource_DecodeByteLz(data, buf); }\n",
            // Comments and strings name nothing.
            "u8 Stream[] = { 1, 2, 3 };\n// Resource_DecodeByteLz(Stream, buf);\nconst char *note = \"Resource_DecodeByteLz(Stream, buf)\";\n",
        ] {
            assert_eq!(alone(text), None, "{text}");
        }
    }

    #[test]
    fn the_numbers_and_the_decoder_may_stand_in_two_files() {
        let caller = "extern u8 Stream[];\nvoid f(void) { Resource_DecodeByteLz(Stream, buf); }\n";
        let table = "extern const u8 *const Streams[];\nvoid f(s32 n) { Resource_DecodeByteLz(Streams[n], buf); }\n";
        let pointers = format!("{STREAM}const u8 *const Streams[] = {{ Stream }};\n");
        // The caller is refused for the other file's numbers, and the file
        // of the numbers for the other file's call.
        let temp = checkout(&[("games/x/src/a.c", caller), ("games/x/src/b.c", STREAM)]);
        assert_eq!(reason(temp.path(), "games/x/src/a.c", caller), Some(REASON));
        assert_eq!(reason(temp.path(), "games/x/src/b.c", STREAM), Some(REASON));
        let temp = checkout(&[("games/x/src/a.c", table), ("games/common/src/b.c", &pointers)]);
        assert_eq!(reason(temp.path(), "games/x/src/a.c", table), Some(REASON));
        assert_eq!(reason(temp.path(), "games/common/src/b.c", &pointers), Some(REASON));
        // A static object is its own file's, and a wrapper may live in a header.
        let hidden = STREAM.replace("u8 Stream", "static u8 Stream");
        let temp = checkout(&[("games/x/src/a.c", caller), ("games/x/src/b.c", &hidden)]);
        assert_eq!(reason(temp.path(), "games/x/src/a.c", caller), None);
        assert_eq!(reason(temp.path(), "games/x/src/b.c", &hidden), None);
        let header = "static inline void Load(const void *source) { Resource_DecodeType01(source, gBuffer); }\n";
        let user = format!("{STREAM}void f(void) {{ Load(Stream); }}\n");
        let temp = checkout(&[("games/x/include/load.h", header), ("games/x/src/a.c", &user)]);
        assert_eq!(reason(temp.path(), "games/x/src/a.c", &user), Some(REASON));
        // The file is read as given, not as the checkout holds it.
        let fixed = "extern u8 Stream[];\n";
        let temp = checkout(&[("games/x/src/a.c", caller), ("games/x/src/b.c", STREAM)]);
        assert_eq!(reason(temp.path(), "games/x/src/b.c", fixed), None);
    }
}
