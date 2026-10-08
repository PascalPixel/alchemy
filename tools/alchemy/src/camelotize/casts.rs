//! Casts the compiler does not need. Candidates are found on the token
//! stream: a value cast to one of the six integer types, or a pointer cast
//! `(T *)`, in front of an operand. An integer cast stays when its operand
//! is an address, string, array or function by its declared type in any
//! edition (`Env::number_of_address`), or when that type cannot be read:
//! GCC 2.96 does not warn when an address meets a parameter list declared
//! empty. Each other removal is proved by `trial`.

use super::trial::{self, Edit};
use crate::permute::ast::{Function, Stmt};
use crate::permute::lex::{lex, Tok, Token};
use crate::permute::parse::{locate, matching, parse_body, scan_definitions, scan_unit, Unit};
use crate::permute::types::{CType, Env};
use crate::targets::DecompTarget;
use std::collections::{BTreeSet, HashMap};
use std::path::Path;

const VALUES: &[&str] = &["u8", "s8", "u16", "s16", "u32", "s32"];

pub(super) fn punct(token: &Token) -> Option<&'static str> {
    match token.tok {
        Tok::Punct(p) => Some(p),
        _ => None,
    }
}

pub(super) fn ident(token: &Token) -> Option<&str> {
    match &token.tok {
        Tok::Ident(name) => Some(name),
        _ => None,
    }
}

pub(super) fn operand_start(token: &Token) -> bool {
    match &token.tok {
        Tok::Ident(_) | Tok::Number(_) | Tok::Char(_) | Tok::Str(_) => true,
        Tok::Punct(p) => matches!(*p, "(" | "*" | "-" | "+" | "~" | "!" | "&" | "++" | "--"),
        Tok::Comment(_) => false,
    }
}

/// Whether the `(` at `open` follows a callee, an array or a declarator
/// rather than starting an operand.
pub(super) fn after_callee(tokens: &[Token], open: usize) -> bool {
    let Some(before) = open.checked_sub(1).map(|k| &tokens[k]) else {
        return false;
    };
    match &before.tok {
        Tok::Ident(name) => !matches!(name.as_str(), "return" | "case" | "else"),
        Tok::Punct(p) => matches!(*p, ")" | "]"),
        _ => true,
    }
}

/// The index of the `)` closing a cast at `open`: a value type, or a type
/// with one or more `*`.
pub(super) fn cast_at(tokens: &[Token], open: usize) -> Option<usize> {
    let mut k = open + 1;
    while matches!(tokens.get(k).and_then(ident), Some("const" | "volatile")) {
        k += 1;
    }
    let name = ident(tokens.get(k)?)?;
    if matches!(name, "struct" | "union") {
        ident(tokens.get(k + 1)?)?;
        k += 1;
    }
    k += 1;
    if VALUES.contains(&name) && punct(tokens.get(k)?) == Some(")") {
        return Some(k);
    }
    let mut stars = 0;
    while matches!(tokens.get(k).and_then(punct), Some("*")) || matches!(tokens.get(k).and_then(ident), Some("const" | "volatile")) {
        if punct(&tokens[k]) == Some("*") {
            stars += 1;
        }
        k += 1;
    }
    (stars > 0 && punct(tokens.get(k)?) == Some(")")).then_some(k)
}

pub(crate) fn candidates(text: &str) -> Vec<Edit> {
    let Ok(tokens) = lex(text) else {
        return Vec::new();
    };
    let tokens = super::numbers::code(tokens);
    let mut found = Vec::new();
    for open in 0..tokens.len() {
        if punct(&tokens[open]) != Some("(") || after_callee(&tokens, open) {
            continue;
        }
        let Some(close) = cast_at(&tokens, open) else {
            continue;
        };
        if !tokens.get(close + 1).is_some_and(operand_start) {
            continue;
        }
        let pointer = !matches!(ident(&tokens[close - 1]), Some(name) if VALUES.contains(&name));
        let taken = punct(&tokens[close + 1]) == Some("&") || (punct(&tokens[close + 1]) == Some("(") && tokens.get(close + 2).and_then(punct) == Some("&"));
        if pointer && taken {
            // `*(T *)&x` may fold to `x`, when T is x's declared type
            // (`numeric` checks); nothing else of an address taken on the
            // spot loses its cast, so no `* &` or `(&x)[0]` is left.
            let deref = open.checked_sub(1).is_some_and(|k| punct(&tokens[k]) == Some("*") && (k == 0 || !operand_end(&tokens[k - 1])));
            let end = unary_end(&tokens, close + 2);
            let simple = end.is_some_and(|end| (close + 2..=end).all(|k| !matches!(punct(&tokens[k]), Some("(" | "++" | "--"))));
            if deref && simple && punct(&tokens[close + 1]) == Some("&") && end.is_some_and(|end| unary_end(&tokens, close + 1) == Some(end)) {
                found.push(Edit { start: tokens[open - 1].start, end: tokens[close + 1].end, text: String::new() });
            }
            continue;
        }
        if pointer && arithmetic(&tokens, open, close) {
            continue;
        }
        // `((T *)p)->f` becomes `p->f`, not `(p)->f`, when p is a
        // postfix expression; `numeric` keeps the cast unless p is a `T *`.
        if let Some(end) = unary_end(&tokens, close + 1) {
            let wrapped = open >= 1 && punct(&tokens[open - 1]) == Some("(") && !after_callee(&tokens, open - 1);
            if pointer && wrapped && tokens.get(end + 1).and_then(punct) == Some(")") && tokens.get(end + 2).and_then(punct) == Some("->") && ident(&tokens[close + 1]).is_some() {
                let operand = text[tokens[close + 1].start..tokens[end].end].to_string();
                found.push(Edit { start: tokens[open - 1].start, end: tokens[end + 1].end, text: operand });
                continue;
            }
        }
        found.push(Edit { start: tokens[open].start, end: tokens[close].end, text: String::new() });
    }
    trial::untagged(text, found)
}

fn operand_end(token: &Token) -> bool {
    matches!(&token.tok, Tok::Ident(_) | Tok::Number(_) | Tok::Char(_) | Tok::Str(_)) || matches!(punct(token), Some(")" | "]"))
}

/// The cast expression at `open` ending at `end`, out through the
/// parentheses that hold only it.
fn enclosed(tokens: &[Token], open: usize, end: usize) -> (usize, usize) {
    let (mut start, mut end) = (open, end);
    while start >= 1 && punct(&tokens[start - 1]) == Some("(") && tokens.get(end + 1).and_then(punct) == Some(")") && !after_callee(tokens, start - 1) {
        start -= 1;
        end += 1;
    }
    (start, end)
}

/// Whether the pointer cast at `open..=close` takes part in pointer
/// arithmetic or indexing, alone or in parentheses: without it, the
/// arithmetic may be on a `void *` (not C89) or in other units.
fn arithmetic(tokens: &[Token], open: usize, close: usize) -> bool {
    let Some(end) = unary_end(tokens, close + 1) else {
        return true;
    };
    let (start, end) = enclosed(tokens, open, end);
    let after = tokens.get(end + 1).and_then(punct).is_some_and(|p| matches!(p, "+" | "-" | "[" | "+=" | "-=" | "++" | "--"));
    let before = start >= 2 && matches!(punct(&tokens[start - 1]), Some("+" | "-")) && operand_end(&tokens[start - 2]);
    let stepped = start >= 1 && matches!(punct(&tokens[start - 1]), Some("++" | "--"));
    after || before || stepped
}

/// The index of the last token of the unary expression starting at `k`.
pub(super) fn unary_end(tokens: &[Token], k: usize) -> Option<usize> {
    let token = tokens.get(k)?;
    match &token.tok {
        Tok::Punct("-" | "+" | "~" | "!" | "*" | "&" | "++" | "--") => return unary_end(tokens, k + 1),
        Tok::Punct("(") => {
            if let Some(close) = cast_at(tokens, k) {
                return unary_end(tokens, close + 1);
            }
        }
        _ => {}
    }
    let mut end = match &token.tok {
        Tok::Punct("(") => matching(tokens, k)?,
        Tok::Ident(_) | Tok::Number(_) | Tok::Char(_) => k,
        Tok::Str(_) => {
            let mut end = k;
            while matches!(tokens.get(end + 1).map(|t| &t.tok), Some(Tok::Str(_))) {
                end += 1;
            }
            end
        }
        _ => return None,
    };
    loop {
        match tokens.get(end + 1).map(|t| &t.tok) {
            Some(Tok::Punct("[" | "(")) => end = matching(tokens, end + 1)?,
            Some(Tok::Punct("." | "->")) if matches!(tokens.get(end + 2).map(|t| &t.tok), Some(Tok::Ident(_))) => end += 2,
            Some(Tok::Punct("++" | "--")) => end += 1,
            _ => return Some(end),
        }
    }
}

/// One edition's preprocessed unit: its declarations, the functions it
/// defines, and each function's declarations once read.
pub(crate) struct Edition {
    text: String,
    unit: Unit,
    typedefs: BTreeSet<String>,
    defined: BTreeSet<String>,
    envs: HashMap<String, Option<Env>>,
}

impl Edition {
    pub(crate) fn new(preprocessed: String) -> Result<Edition, String> {
        let unit = scan_unit(&preprocessed)?;
        let defined = scan_definitions(&lex(&preprocessed)?).into_iter().map(|d| d.0).collect();
        Ok(Edition { typedefs: unit.typedef_names(), text: preprocessed, unit, defined, envs: HashMap::new() })
    }

    /// Whether the cast of `operand` in `function` (empty at file scope)
    /// makes a number of an address here: `None` when this edition does not
    /// define the function, true when the type cannot be read.
    pub(crate) fn address(&mut self, function: &str, operand: &str) -> Option<bool> {
        Some(self.typed(function, operand, |env, expr| env.number_of_address(expr))?.unwrap_or(true))
    }

    /// Whether `operand` is a function or a pointer to one, or cannot be
    /// typed: a cast to an object pointer of one stays.
    fn function(&mut self, function: &str, operand: &str) -> Option<bool> {
        let code = |ty: &CType| match ty.strip() {
            CType::Func(..) => true,
            CType::Ptr(inner) => matches!(inner.strip(), CType::Func(..)),
            _ => false,
        };
        Some(self.typed(function, operand, |env, expr| env.type_of(expr).is_none_or(|ty| code(&ty)))?.unwrap_or(true))
    }

    /// The declared type of `operand`, spelled, or none when it cannot be.
    fn spelled(&mut self, function: &str, operand: &str) -> Option<Option<String>> {
        Some(self.typed(function, operand, |env, expr| env.type_of(expr).and_then(|ty| env.spell(&ty)))?.flatten())
    }

    /// `read` of the operand in `function`'s scope: `None` when this edition
    /// does not define the function, `Some(None)` when the operand cannot be
    /// parsed or the function read.
    fn typed<R>(&mut self, function: &str, operand: &str, read: impl FnOnce(&Env, &crate::permute::ast::Expr) -> R) -> Option<Option<R>> {
        if !function.is_empty() && !self.defined.contains(function) {
            return None;
        }
        let (unit, text, typedefs) = (&self.unit, &self.text, &self.typedefs);
        let env = self.envs.entry(function.to_string()).or_insert_with(|| {
            if function.is_empty() {
                let none = Function { name: String::new(), header: String::new(), params: Vec::new(), body: Vec::new() };
                return Some(Env::new(unit, &none));
            }
            locate(text, Some(function), typedefs).ok().map(|located| Env::new(unit, &located.function))
        });
        let Some(env) = env else {
            return Some(None);
        };
        let names: Vec<&str> = self.typedefs.iter().map(String::as_str).collect();
        let expr = match parse_body(&format!("{operand};"), &names).as_deref() {
            Ok([Stmt::Expr(expr)]) => expr.clone(),
            _ => return Some(None),
        };
        Some(Some(read(env, &expr)))
    }
}

/// The candidates left once every integer cast of an address, string,
/// array or function, or of an operand whose type no edition can read, is
/// taken out. Pointer casts pass through.
pub(crate) fn numeric(text: &str, editions: &mut [Edition], found: Vec<Edit>) -> Vec<Edit> {
    let Ok(tokens) = lex(text) else {
        return Vec::new();
    };
    let tokens = super::numbers::code(tokens);
    let bodies: Vec<(String, usize, usize)> = scan_definitions(&tokens)
        .into_iter()
        .filter_map(|(name, _, _, _, open)| Some((name, tokens[open].start, tokens[matching(&tokens, open)?].end)))
        .collect();
    found
        .into_iter()
        .filter(|edit| {
            let Some(mut open) = tokens.iter().position(|t| t.start == edit.start) else {
                return false;
            };
            // `((T *)p)->f` taken out whole.
            if !edit.text.is_empty() && punct(&tokens[open]) == Some("(") && cast_at(&tokens, open).is_none() {
                open += 1;
            }
            // `*(T *)&x` folding to `x`.
            let fold = punct(&tokens[open]) == Some("*");
            if fold {
                open += 1;
            }
            let Some(close) = cast_at(&tokens, open) else {
                return false;
            };
            let function = bodies.iter().find(|(_, start, stop)| *start <= edit.start && edit.end <= *stop).map_or("", |b| b.0.as_str());
            if fold {
                // T exactly x's declared type in every edition that defines
                // the function.
                let Some(end) = unary_end(&tokens, close + 2) else {
                    return false;
                };
                let operand = &text[tokens[close + 2].start..tokens[end].end];
                let pointee: String = tokens[open + 1..close].iter().map(|t| &text[t.start..t.end]).collect::<Vec<_>>().join(" ");
                let Some(pointee) = pointee.strip_suffix(" *").map(str::to_string) else {
                    return false;
                };
                let mut read = false;
                for edition in editions.iter_mut() {
                    match edition.spelled(function, operand) {
                        Some(Some(spelled)) if spelled.split_whitespace().collect::<Vec<_>>().join(" ") == pointee => read = true,
                        Some(_) => return false,
                        None => {}
                    }
                }
                return read;
            }
            let Some(end) = unary_end(&tokens, close + 1) else {
                return false;
            };
            let operand = &text[tokens[close + 1].start..tokens[end].end];
            let pointer = !matches!(ident(&tokens[close - 1]), Some(name) if VALUES.contains(&name));
            let member = tokens.get(enclosed(&tokens, open, end).1 + 1).and_then(punct) == Some("->");
            if pointer && (member || open >= 1 && punct(&tokens[open - 1]) == Some("*") && (open < 2 || !operand_end(&tokens[open - 2]))) {
                // `*(T *)p` and `((T *)p)->f` lose their cast only when
                // every edition declares p as `T *`: otherwise the access
                // changes type (`*(u16 *)q` through an `s16 *q`) or names
                // a field of another struct.
                let cast: String = tokens[open + 1..close].iter().map(|t| &text[t.start..t.end]).collect::<Vec<_>>().join(" ");
                let mut read = false;
                for edition in editions.iter_mut() {
                    match edition.spelled(function, operand) {
                        Some(Some(spelled)) if spelled.split_whitespace().collect::<Vec<_>>().join(" ") == cast => read = true,
                        Some(_) => return false,
                        None => {}
                    }
                }
                return read;
            }
            let mut read = false;
            for edition in editions.iter_mut() {
                // A pointer cast of a function stays (`Dma_Set((void *)F, ...)`
                // passes code where an object pointer is expected); an
                // integer cast of an address stays.
                let kept = if pointer { edition.function(function, operand) } else { edition.address(function, operand) };
                match kept {
                    Some(true) => return false,
                    Some(false) => read = true,
                    None => {}
                }
            }
            read
        })
        .collect()
}

pub(crate) fn rewrite(root: &Path, file: &Path, text: &str, games: &[DecompTarget]) -> Result<(String, usize), String> {
    let found = candidates(text);
    if found.is_empty() {
        return Ok((text.to_string(), 0));
    }
    let relative = file.strip_prefix(root).unwrap_or(file).to_string_lossy().into_owned();
    let mut editions = Vec::new();
    for target in trial::editions(games) {
        let preprocessed = crate::compiler::preprocess::Expansion::new(root, target)?.fresh(&relative)?;
        editions.push(Edition::new(preprocessed)?);
    }
    let found = numeric(text, &mut editions, found);
    trial::proved(root, file, text, games, found)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn spans(text: &str) -> Vec<&str> {
        candidates(text).iter().map(|c| &text[c.start..c.end]).collect()
    }

    #[test]
    fn value_and_pointer_casts_in_front_of_operands_are_found() {
        let text = "x = (u8)y + (s16)-z;\np = (struct Foo *)q;\nr = (const u16 *)(s + 2);\nreturn (u32)f(1);\n";
        assert_eq!(spans(text), vec!["(u8)", "(s16)", "(struct Foo *)", "(const u16 *)", "(u32)"]);
    }

    #[test]
    fn calls_declarators_sizeof_and_asm_are_not_casts() {
        let text = "void F(u8);\ntypedef void (*Fn)(u8 *);\nn = sizeof(u16);\nG(u8) ;\nasm(\"\" : : \"r\"((u32)x));\n/* (u8)x */\n";
        assert!(spans(text).is_empty(), "{:?}", spans(text));
    }

    #[test]
    fn fakematch_tagged_statements_and_asm_keep_their_casts() {
        let text = "void F(void)\n{\n    if (y) {\n        z = 0;\n        /* FAKEMATCH: order */\n        a = (u8)b;\n    }\n    if (w) {\n        c = (u8)d; /* FAKEMATCH: r4 */\n    }\n    register u32 r asm(\"r4\") = (u32)x;\n    e = (u8)f;\n}\n";
        assert_eq!(spans(text), vec!["(u8)"]);
        assert_eq!(candidates(text)[0].start, text.find("(u8)f").unwrap());
        let block = "void F(void)\n{\n    /* FAKEMATCH: frame */\n    a = (u8)b;\n    if (a) {\n        c = (u16 *)d;\n    }\n}\n";
        assert!(spans(block).is_empty());
    }

    fn numeric_spans(text: &str, editions: &[&str]) -> Vec<String> {
        let mut editions: Vec<Edition> = editions.iter().map(|e| Edition::new(e.to_string()).unwrap()).collect();
        numeric(text, &mut editions, candidates(text)).iter().map(|c| text[c.start..].lines().next().unwrap().to_string()).collect()
    }

    #[test]
    fn integer_casts_of_addresses_stay_by_declared_type() {
        // rowing.c and play.c: messages and tables are arrays, and their
        // callees are declared with empty parentheses, so GCC says nothing.
        let text = "typedef int s32;\ntypedef unsigned char u8;\ntypedef unsigned short u16;\ntypedef unsigned int u32;\n\
            extern const u8 MsgFuneWhereGoingSlay[];\nextern const u8 MsgTorebiGetRidItems[];\nextern u8 FuneHeya_ProgressTableA[];\n\
            extern void UiText_ShowCenteredMessage();\nextern void Object_SetActionCallbackAndRefreshById();\nextern void Engine_EventSetMessage();\n\
            extern void Callback(void);\nstruct Pair { u8 *p; u16 n; };\ns32 gTable[] = { (s32)Callback };\n\
            void F(s32 a, struct Pair *pair)\n{\n    s32 n;\n    s32 message = (s32)MsgTorebiGetRidItems;\n\
            UiText_ShowCenteredMessage((s32)MsgFuneWhereGoingSlay, 1, 8);\n\
            Object_SetActionCallbackAndRefreshById(3, (s32)FuneHeya_ProgressTableA);\n\
            Engine_EventSetMessage((s32)MsgTorebiGetRidItems);\n\
            G((s32)\"text\", (s32)&n, (s32)Callback, (s32)pair->p, (u32)Unknown, (s32)(u8 *)pair);\n\
            G((u8)n, (u16)pair->n, (s32)-a, (u8 *)pair, (s32)message);\n}\n";
        assert_eq!(
            numeric_spans(text, &[text]),
            vec!["(u8)n, (u16)pair->n, (s32)-a, (u8 *)pair, (s32)message);", "(u16)pair->n, (s32)-a, (u8 *)pair, (s32)message);", "(s32)-a, (u8 *)pair, (s32)message);", "(u8 *)pair, (s32)message);", "(s32)message);"]
        );
    }

    #[test]
    fn every_edition_that_defines_the_function_must_read_a_number() {
        let number = "typedef int s32;\nextern s32 gX;\nvoid F(void)\n{\n    G((s32)gX);\n}\n";
        let address = "typedef int s32;\nextern char *gX;\nvoid F(void)\n{\n    G((s32)gX);\n}\n";
        let without = "typedef int s32;\nextern char *gX;\n";
        assert_eq!(numeric_spans(number, &[number]).len(), 1);
        assert!(numeric_spans(number, &[number, address]).is_empty());
        assert_eq!(numeric_spans(number, &[without, number]).len(), 1);
        assert!(numeric_spans(number, &[without]).is_empty());
    }

    /// The reviewer's findings: no `* &`, `(&x)[0]` or dereference of an
    /// address taken on the spot, and `*(T *)&x` folds only to an x of type T.
    #[test]
    fn addresses_taken_on_the_spot_keep_their_casts_or_fold_whole() {
        let text = "x = ((u8 *)&state)[0];\ny = *(u16 *)(&obj->paused);\nz = ((u8 *)&obj)->paused;\nf((u8 *)&w);\n*(u8 *)&obj->paused = 0;\n";
        let found = candidates(text);
        assert_eq!(found.iter().map(|c| &text[c.start..c.end]).collect::<Vec<_>>(), vec!["*(u8 *)&"]);
        let out = trial::apply(text, &found);
        assert_eq!(out, text.replace("*(u8 *)&obj->paused = 0", "obj->paused = 0"));
        assert!(!out.contains("* &") && !out.contains("*&") && !out.contains("(&state)") && !out.contains("(&obj)"), "{out}");
        let unit = |paused: &str| {
            format!("typedef unsigned char u8;\ntypedef unsigned short u16;\nstruct Obj {{ {paused} paused; u8 rise_counter; }};\nvoid F(struct Obj *obj)\n{{\n    *(u8 *)&obj->paused = 1;\n}}\n")
        };
        let byte = unit("u8");
        assert_eq!(numeric_spans(&byte, &[&byte]), vec!["*(u8 *)&obj->paused = 1;"]);
        let half = unit("u16");
        assert!(numeric_spans(&half, &[&half]).is_empty());
        assert!(numeric_spans(&byte, &[&byte, &half]).is_empty());
    }

    #[test]
    fn pointer_casts_in_arithmetic_and_of_functions_stay() {
        same_candidates("p = (u8 *)q + 4;\nx = ((u8 *)q)[4];\ny = *((u8 *)q + 2);\nz = 2 + (u8 *)q;\n(u8 *)q += 1;\n");
        let text = "typedef unsigned int u32;\nextern void Function(void);\nextern void Dma_Set();\nextern u32 gWords[];\nvoid F(u32 *dst)\n{\n    Dma_Set((void *)Function, dst, 0x84000003, 3);\n    Dma_Set((void *)gWords, dst, 0x84000003, 3);\n}\n";
        assert_eq!(numeric_spans(text, &[text]), vec!["(void *)gWords, dst, 0x84000003, 3);"]);
    }

    /// The session-3 dry run: ☀️ field/kuupuappu_heya/heya.c:501 stored
    /// through `*q` for `*(u16 *)q` with `s16 *q`.
    #[test]
    fn a_dereferenced_pointer_cast_goes_only_when_it_names_the_declared_type() {
        let text = "typedef short s16;\ntypedef unsigned short u16;\nvoid F(s16 *q, u16 *r, u16 n)\n{\n    *(u16 *)q = n;\n    *(u16 *)r = n;\n    *(u16 *)q = *(u16 *)q + 1;\n}\n";
        assert_eq!(numeric_spans(text, &[text]), vec!["(u16 *)r = n;"]);
    }

    /// The rules v2 dry run: a cast in front of `->` went whenever another
    /// struct had a field of that name at that offset.
    #[test]
    fn a_member_access_through_a_pointer_cast_names_the_declared_type() {
        let text = "typedef unsigned char u8;\nstruct Unit { u8 hp; };\nstruct Work { u8 hp; };\nvoid F(struct Work *w, struct Unit *u, void *v)\n{\n    ((struct Unit *)w)->hp = 1;\n    ((struct Unit *)u)->hp = 2;\n    ((struct Unit *)v)->hp = 3;\n}\n";
        assert_eq!(numeric_spans(text, &[text]), vec!["((struct Unit *)u)->hp = 2;"]);
        let found: Vec<Edit> = numeric(text, &mut [Edition::new(text.to_string()).unwrap()], candidates(text));
        assert_eq!(trial::apply(text, &found), text.replace("((struct Unit *)u)->hp = 2", "u->hp = 2"));
    }

    fn same_candidates(text: &str) {
        assert!(spans(text).is_empty(), "{:?}", spans(text));
    }

    #[test]
    fn removal_keeps_tokens_apart() {
        let text = "a = b -(s16)-c;\nreturn(u8)x;\nd = (u8) e;\nf((u8)g, (u16 *)h);\n";
        assert_eq!(trial::apply(text, &candidates(text)), "a = b - -c;\nreturn x;\nd = e;\nf(g, h);\n");
    }
}
