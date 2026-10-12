//! A function definition's text as it can be compared between files: its
//! tokens without comments or spacing, its own name and the names of its
//! parameters, local variables and labels replaced by their order of
//! appearance.

use crate::compiler::c_syntax::{lex, matching, scan_definitions, Tok, Token};
use std::collections::BTreeMap;

/// The words that are C's and not the program's.
const KEYWORDS: &[&str] = &[
    "if", "else", "for", "while", "do", "switch", "case", "default", "return", "goto", "break", "continue", "sizeof", "struct", "union", "enum",
    "typedef", "static", "extern", "register", "auto", "const", "volatile", "signed", "unsigned", "void", "char", "short", "int", "long", "float",
    "double", "inline", "asm", "__asm__", "__attribute__", "__inline__",
];
/// The keywords a declaration can start with.
const TYPES: &[&str] = &[
    "struct", "union", "enum", "static", "extern", "register", "auto", "const", "volatile", "signed", "unsigned", "void", "char", "short", "int",
    "long", "float", "double",
];
/// The marks around a directive's line.
const HASH: &str = "__hash__";
const END: &str = "__eol__";

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub(crate) struct Text {
    pub name: String,
    /// The definition's tokens, its own name as `$0` and each name it
    /// declares as `$1`, `$2` ...
    pub tokens: Vec<String>,
    /// Which tokens are the program's words: names, numbers and literals.
    pub words: Vec<bool>,
    /// An empty body or a single `return`.
    pub trivial: bool,
}

impl Text {
    /// The text with every word of the program left open: two definitions
    /// of one shape differ only in names, numbers and literals.
    pub(crate) fn shape(&self) -> String {
        let open = self.tokens.iter().zip(&self.words).map(|(token, word)| if *word { "_" } else { token.as_str() });
        open.collect::<Vec<_>>().join(" ")
    }
}

fn spelled(tok: &Tok) -> &str {
    match tok {
        Tok::Ident(s) | Tok::Number(s) | Tok::Char(s) | Tok::Str(s) | Tok::Comment(s) => s,
        Tok::Punct(p) => p,
    }
}

fn word(tok: &Tok) -> Option<&str> {
    match tok {
        Tok::Ident(name) if !KEYWORDS.contains(&name.as_str()) => Some(name),
        _ => None,
    }
}

fn is(tok: &Tok, punct: &str) -> bool {
    matches!(tok, Tok::Punct(p) if *p == punct)
}

/// The tokens of a piece of C without its comments. A directive's line is
/// kept, between two marks, where the lexer would leave it out.
pub(crate) fn tokens(text: &str) -> Vec<Token> {
    let joined = text.replace("\\\n", " ");
    let marked: Vec<String> = joined
        .lines()
        .map(|line| match line.trim_start().strip_prefix('#') {
            Some(rest) => format!("{HASH} {rest} {END}"),
            None => line.to_string(),
        })
        .collect();
    // Text the lexer cannot read is compared word by word.
    let lexed = lex(&marked.join("\n")).unwrap_or_else(|_| joined.split_whitespace().map(|word| Token { tok: Tok::Str(word.to_string()), line: 0 }).collect());
    lexed.into_iter().filter(|token| !matches!(token.tok, Tok::Comment(_))).collect()
}

/// A piece of C that is not a function, as one line of tokens.
pub(crate) fn plain(text: &str) -> String {
    tokens(text).iter().map(|token| spelled(&token.tok)).collect::<Vec<_>>().join(" ")
}

/// The name a declarator declares: the one inside `(*name)`, else the last
/// word outside brackets.
fn declared(declarator: &[Token]) -> Option<&str> {
    let end = declarator.iter().position(|t| matches!(&t.tok, Tok::Ident(w) if matches!(w.as_str(), "asm" | "__asm__" | "__attribute__")));
    let declarator = &declarator[..end.unwrap_or(declarator.len())];
    if let Some(open) = declarator.windows(2).position(|pair| is(&pair[0].tok, "(") && is(&pair[1].tok, "*")) {
        return declarator[open..].iter().find_map(|t| word(&t.tok));
    }
    let mut depth = 0usize;
    let mut last = None;
    for token in declarator {
        match &token.tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => depth = depth.saturating_sub(1),
            tok if depth == 0 => last = word(tok).or(last),
            _ => {}
        }
    }
    last
}

/// The declarators of a declaration's tokens: split at its commas, each
/// cut at its initializer.
fn declarators(declaration: &[Token]) -> Vec<&[Token]> {
    let mut found = Vec::new();
    let (mut depth, mut start, mut cut) = (0usize, 0usize, None);
    for (i, token) in declaration.iter().enumerate() {
        match &token.tok {
            Tok::Punct("(" | "[" | "{") => depth += 1,
            Tok::Punct(")" | "]" | "}") => depth = depth.saturating_sub(1),
            Tok::Punct("=") if depth == 0 && cut.is_none() => cut = Some(i),
            Tok::Punct(",") if depth == 0 => {
                found.push(&declaration[start..cut.unwrap_or(i)]);
                (start, cut) = (i + 1, None);
            }
            _ => {}
        }
    }
    found.push(&declaration[start..cut.unwrap_or(declaration.len())]);
    found
}

/// Whether the statement starting at `at` is a declaration: it starts with
/// a type's keyword, or with a type's name before a name, a star, or a
/// parenthesized pointer that is then called or indexed.
fn declares(code: &[Token], at: usize) -> bool {
    let Tok::Ident(first) = &code[at].tok else {
        return false;
    };
    if TYPES.contains(&first.as_str()) || KEYWORDS.contains(&first.as_str()) {
        return TYPES.contains(&first.as_str());
    }
    match code.get(at + 1).map(|t| &t.tok) {
        Some(Tok::Ident(next)) => !KEYWORDS.contains(&next.as_str()) || TYPES.contains(&next.as_str()),
        Some(Tok::Punct("*")) => true,
        Some(Tok::Punct("(")) => {
            let after = matching(code, at + 1).and_then(|close| code.get(close + 1));
            code.get(at + 2).is_some_and(|t| is(&t.tok, "*")) && after.is_some_and(|t| is(&t.tok, "(") || is(&t.tok, "["))
        }
        _ => false,
    }
}

/// The names a function's own text declares: its parameters between
/// `list` and `list_close`, and from its body between `open` and `close`
/// its local variables, its labels and the functions nested in it with
/// their parameters.
fn locals(code: &[Token], list: usize, list_close: usize, open: usize, close: usize) -> Vec<&str> {
    let mut found: Vec<&str> = declarators(&code[list + 1..list_close]).into_iter().filter_map(declared).collect();
    let mut depth = 0usize;
    let mut i = open + 1;
    while i < close {
        let start = depth == 0 && matches!(&code[i - 1].tok, Tok::Punct(";" | "{" | "}"));
        match &code[i].tok {
            Tok::Punct("(" | "[") => depth += 1,
            Tok::Punct(")" | "]") => depth = depth.saturating_sub(1),
            Tok::Ident(name) if start && is(&code[i + 1].tok, ":") && !KEYWORDS.contains(&name.as_str()) => found.push(name),
            Tok::Ident(_) if start && declares(code, i) => {
                // To the semicolon that ends it, or to a nested function's body.
                let (mut inner, mut end) = (0usize, i);
                while end < close {
                    match &code[end].tok {
                        Tok::Punct("{") if inner == 0 && is(&code[end - 1].tok, ")") => break,
                        Tok::Punct(";") if inner == 0 => break,
                        Tok::Punct("(" | "[" | "{") => inner += 1,
                        Tok::Punct(")" | "]" | "}") => inner = inner.saturating_sub(1),
                        _ => {}
                    }
                    end += 1;
                }
                let nested = (i..end).find(|&k| is(&code[end].tok, "{") && matching(code, k) == Some(end - 1));
                match nested {
                    Some(list) => {
                        found.extend(declared(&code[i..list]));
                        found.extend(declarators(&code[list + 1..end - 1]).into_iter().filter_map(declared));
                    }
                    None => found.extend(declarators(&code[i..end]).into_iter().filter_map(declared)),
                }
                i = end;
                continue;
            }
            _ => {}
        }
        i += 1;
    }
    found
}

/// A function definition item's comparable text; `None` when its head
/// cannot be read as a definition.
pub(crate) fn read(item: &str) -> Option<Text> {
    let code = tokens(item);
    let (name, _, at, _, _) = scan_definitions(&code).into_iter().next()?;
    // What it declares is read without its directives: a declaration may
    // follow one, and their words declare nothing.
    let mut directive = false;
    let bare = code.iter().filter(|token| {
        let spelling = spelled(&token.tok);
        directive = (directive || spelling == HASH) && spelling != END;
        !directive && spelling != END
    });
    let bare: Vec<Token> = bare.cloned().collect();
    let (_, _, list, list_close, open) = scan_definitions(&bare).into_iter().next()?;
    // The item ends with its body; the arms of a conditional may each open
    // a brace that one line closes.
    let last = |code: &[Token]| code.iter().rposition(|t| is(&t.tok, "}"));
    let (close, end) = (last(&bare).filter(|close| *close > open)?, last(&code)?);
    let local = locals(&bare, list + 1, list_close, open, close);
    // Numbered by where each first appears, so the order of two
    // declarations in one list does not tell two texts apart.
    let mut numbers: BTreeMap<&str, usize> = BTreeMap::from([(name.as_str(), 0)]);
    let mut text = Text { name: name.clone(), ..Text::default() };
    let mut directive = false;
    for (i, token) in code.iter().enumerate().take(end + 1) {
        let spelling = spelled(&token.tok);
        let mark = spelling == HASH || spelling == END;
        directive = (directive || spelling == HASH) && spelling != END;
        let member = i > 0 && matches!(&code[i - 1].tok, Tok::Punct("." | "->"));
        let own = word(&token.tok).filter(|name| i >= at && !member && !directive && (numbers.contains_key(name) || local.contains(name)));
        match own {
            Some(name) => {
                let next = numbers.len();
                text.tokens.push(format!("${}", numbers.entry(name).or_insert(next)));
                text.words.push(false);
            }
            None => {
                text.tokens.push(spelling.to_string());
                text.words.push(!mark && (word(&token.tok).is_some() || matches!(token.tok, Tok::Number(_) | Tok::Char(_) | Tok::Str(_))));
            }
        }
    }
    let body = &bare[open + 1..close];
    let single = body.iter().position(|t| is(&t.tok, ";")) == Some(body.len().saturating_sub(1));
    text.trivial = body.is_empty() || (single && matches!(&body[0].tok, Tok::Ident(w) if w == "return"));
    Some(text)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn local_names_and_comments_do_not_tell_two_texts_apart() {
        let one = read("s32 Tick(struct Actor *actor, s32 step)\n{\n\ts32 *rec = actor->rec; // 記録\n\tu8 buffer[4], *end;\n\n\tif (rec[0] == 0)\n\t\tgoto done;\n\trec[0] = Tick(actor, step - 1);\ndone:\n\treturn Other(buffer, end);\n}\n").unwrap();
        let two = read("s32 Step(struct Actor *a0, s32 n) {\n  s32 *r = a0->rec; u8 b[4], *e;\n  if (r[0] == 0) goto out; /* x */\n  r[0] = Step(a0, n - 1);\nout: return Other(b, e); }").unwrap();
        assert_eq!(one.tokens, two.tokens);
        assert_eq!((one.name.as_str(), two.name.as_str()), ("Tick", "Step"));
        assert!(one.tokens.join(" ").starts_with("s32 $0 ( struct Actor * $1 , s32 $2 ) { s32 * $3 = $1 -> rec ; u8 $4 [ 4 ] , * $5 ;"));
        // A member keeps its name though a local has it too; a called
        // function and a number are the program's words.
        let three = read("void Set(s32 rec)\n{\n\tgWork.rec = rec;\n\tApply(rec, 0x10);\n}\n").unwrap();
        assert_eq!(three.tokens.join(" "), "void $0 ( s32 $1 ) { gWork . rec = $1 ; Apply ( $1 , 0x10 ) ; }");
        assert_eq!(three.shape(), "void $0 ( _ $1 ) { _ . _ = $1 ; _ ( $1 , _ ) ; }");
    }

    #[test]
    fn declarations_are_told_from_statements() {
        let text = read("void Run(void)\n{\n\ts32 (*step)(s32) = Table[0];\n\tstruct Pair { s32 a; } pair;\n\tregister s32 kept asm(\"r5\");\n\tApply(*step);\n\tkept = pair.a * count;\n#if EDITION\n\tstep(kept);\n#endif\n}\n").unwrap();
        assert_eq!(
            text.tokens.join(" "),
            "void $0 ( void ) { s32 ( * $1 ) ( s32 ) = Table [ 0 ] ; struct Pair { s32 a ; } $2 ; register s32 $3 asm ( \"r5\" ) ; Apply ( * $1 ) ; $3 = $2 . a * count ; __hash__ if EDITION __eol__ $1 ( $3 ) ; __hash__ endif __eol__ }"
        );
        let nested = read("s32 Outer(s32 base)\n{\n\ts32 Inner(s32 add)\n\t{\n\t\treturn base + add;\n\t}\n\treturn Inner(2);\n}\n").unwrap();
        assert_eq!(nested.tokens.join(" "), "s32 $0 ( s32 $1 ) { s32 $2 ( s32 $3 ) { return $1 + $3 ; } return $2 ( 2 ) ; }");
    }

    #[test]
    fn an_empty_body_and_a_single_return_are_trivial() {
        assert!(read("void Nothing(void)\n{\n}\n").unwrap().trivial);
        assert!(read("s32 Zero(void)\n{\n\treturn 0; // none\n}\n").unwrap().trivial);
        assert!(!read("s32 Cue(void)\n{\n\tPlay(118);\n\treturn 0;\n}\n").unwrap().trivial);
        assert!(!read("void Call(void)\n{\n\tPlay(118);\n}\n").unwrap().trivial);
    }
}
