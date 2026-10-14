//! The scene layer and the scene commands (S2, S3), written in by two
//! rewrites proved as casts are.
//!
//! `layer`: a call of a routine that an inline of a layer header
//! (`compiler::steering::SCRIPT_LAYER`) forwards its parameters to, in
//! order, goes through that inline (one with a FAKEMATCH tag or asm is a
//! device, and takes no calls); so does a call of another inline of the
//! same routine that the chosen one is preferred over. The chosen inline is
//! the one an event.h command expands to, else the outermost (an inline of
//! an inline), else the first defined. Only overlay units take the layer,
//! and only scenes the scene script layer (field_event.h, field_service.h,
//! scene/runpa_jo.h); other overlays take the engine call and IWRAM layers.
//! A header the inline needs is included with the first includes, above
//! the shared scene source a scene includes after them: event.h
//! for a scene when it holds the inline, else the inline's own header. When
//! the include alone changes a compile, only the inlines the file already
//! reaches are used.
//!
//! `commands`: a call that an event.h command expands to is written as the
//! command, the vocabulary being whatever event.h defines. A command
//! argument the command shifts into fixed point (`(x) << 16`) is written
//! back in pixels from a literal that is a whole number of pixels,
//! `PIXELS(e)` or `e << 16`; every other shape stays a call. A command
//! argument the command casts takes the source's operand without that cast,
//! and an argument the command fixes must be that value. Only scenes take
//! commands, and event.h is included with the first includes, above the
//! shared scene source.
//!
//! Both rewrite calls in function bodies, outside FAKEMATCH-tagged code and
//! asm, and never inside a #define. A file's edits are proved by
//! `trial::proved_groups`, one function's calls as a group, in every ☀️
//! edition; a file other files include is proved in every C file that
//! reaches it, directly or through other headers, and refused when one of
//! them compiles without reading it (`trial::Judge`).

use super::casts::{cast_at, ident, punct};
use super::source;
use super::trial::{self, Edit, Judge};
use crate::compiler::c_syntax;
use crate::compiler::routing::cflags_for_target_source;
use crate::compiler::steering::{self, SCRIPT_LAYER};
use crate::permute::lex::{lex, Tok, Token};
use crate::permute::parse::{matching, scan_definitions};
use crate::targets::DecompTarget;
use std::cmp::Reverse;
use std::collections::BTreeSet;
use std::ops::Range;
use std::path::{Path, PathBuf};

/// ☀️'s scene command vocabulary.
pub(crate) const EVENT_H: &str = "games/tbs/include/event.h";
/// ☀️'s scenes: the scene overlays and the files they share.
const SCENES: &str = "games/tbs/src/scene/";
/// The scene script layer's headers (S2): only scenes go through them.
const SCENE_LAYER: &[&str] = &["games/tbs/include/field_event.h", "games/tbs/include/field_service.h", "games/tbs/src/scene/runpa_jo.h"];
/// Where the layer headers a file may be given an include of are.
const INCLUDABLE: &str = "games/tbs/include/";

/// An inline of a layer header that forwards its parameters, in order, to
/// one routine.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Helper {
    /// The name its definition has.
    pub name: String,
    /// The name a caller writes: the name of an object-like #define of the
    /// header that stands for it, or its own.
    pub spelling: String,
    pub callee: String,
    pub arity: usize,
    /// Its header, as a path under the root.
    pub header: String,
}

/// The tokens of a range with the parentheses that enclose all of it taken
/// off.
fn whole(mut tokens: &[Token]) -> &[Token] {
    while tokens.len() >= 2 && punct(&tokens[0]) == Some("(") && matching(tokens, 0) == Some(tokens.len() - 1) {
        tokens = &tokens[1..tokens.len() - 1];
    }
    tokens
}

/// The pieces of `tokens` between its top-level commas; none when it is
/// empty.
fn pieces(tokens: &[Token]) -> Vec<Range<usize>> {
    if tokens.is_empty() {
        return Vec::new();
    }
    let mut out = Vec::new();
    let (mut start, mut depth) = (0, 0i32);
    for (k, token) in tokens.iter().enumerate() {
        match punct(token) {
            Some("(" | "[" | "{") => depth += 1,
            Some(")" | "]" | "}") => depth -= 1,
            Some(",") if depth == 0 => {
                out.push(start..k);
                start = k + 1;
            }
            _ => {}
        }
    }
    out.push(start..tokens.len());
    out
}

/// The routine a function body forwards to by name, and the names it
/// passes: a body that is one call of a named routine, returned or not.
/// A body that holds a FAKEMATCH tag or asm is no forwarder: what it
/// steers is a device, not a layer (S2).
fn forwarded(body: &[c_syntax::Token]) -> Option<(String, Vec<String>)> {
    if steering::has_reason(body) {
        return None;
    }
    let code = uncommented(body);
    let (callee, names) = steering::forwarded_call(&code)?;
    let start = usize::from(matches!(code.first(), Some(c_syntax::Tok::Ident(word)) if word == "return"));
    match (code.get(start), code.get(start + 1)) {
        (Some(c_syntax::Tok::Ident(name)), Some(c_syntax::Tok::Punct("("))) if *name == callee => Some((callee, names)),
        _ => None,
    }
}

fn uncommented(tokens: &[c_syntax::Token]) -> Vec<&c_syntax::Tok> {
    tokens.iter().map(|t| &t.tok).filter(|t| !matches!(t, c_syntax::Tok::Comment(_))).collect()
}

/// The object-like #defines of a text that name another name: `A B`.
fn renames(text: &str) -> Vec<(String, String)> {
    let word = |w: &str| !w.is_empty() && w.chars().all(|c| c.is_ascii_alphanumeric() || c == '_') && !w.starts_with(|c: char| c.is_ascii_digit());
    text.lines()
        .filter_map(|line| {
            let rest = line.trim_start().strip_prefix('#')?.trim_start().strip_prefix("define")?;
            let mut words = rest.split_whitespace();
            let (from, to) = (words.next()?, words.next()?);
            (words.next().is_none() && word(from) && word(to)).then(|| (from.to_string(), to.to_string()))
        })
        .collect()
}

/// The forwarding inlines a layer header defines: `static inline`
/// functions whose body is one call passing their parameters in order,
/// with no FAKEMATCH tag. An inline defined in two branches of an #if is
/// one inline, a forwarder only when every definition forwards alike.
pub(crate) fn helpers(header: &str, text: &str) -> Vec<Helper> {
    let Ok((tokens, branches)) = c_syntax::lex_branches(text) else {
        return Vec::new();
    };
    let renamed = renames(text);
    let mut out: Vec<Helper> = Vec::new();
    let mut refused = BTreeSet::new();
    for (name, boundary, name_index, close, open) in c_syntax::scan_definitions_in(&tokens, &branches) {
        let head = &tokens[boundary..name_index];
        let has = |words: &[&str]| head.iter().any(|t| matches!(&t.tok, c_syntax::Tok::Ident(w) if words.contains(&w.as_str())));
        if !has(&["static"]) || !has(&["inline", "__inline", "__inline__"]) || refused.contains(&name) {
            continue;
        }
        let parameters = uncommented(&tokens[name_index + 2..close]);
        let found = c_syntax::matching_in(&tokens, open, &branches)
            .and_then(|end| Some((forwarded(&tokens[open + 1..end])?, steering::parameter_names(&parameters)?)))
            .filter(|((callee, passed), parameters)| passed == parameters && !parameters.contains(callee));
        let earlier = out.iter().position(|h| h.name == name);
        match (found, earlier) {
            (Some(((callee, _), parameters)), Some(at)) if out[at].callee == callee && out[at].arity == parameters.len() => {}
            (Some(((callee, _), parameters)), None) => {
                let spelling = renamed.iter().find(|(_, to)| *to == name).map_or_else(|| name.clone(), |(from, _)| from.clone());
                out.push(Helper { spelling, name, callee, arity: parameters.len(), header: header.to_string() });
            }
            (_, earlier) => {
                if let Some(at) = earlier {
                    out.remove(at);
                }
                refused.insert(name);
            }
        }
    }
    out
}

/// The layer: every forwarding inline of ☀️'s layer headers, in the order
/// of `SCRIPT_LAYER` and of each header, and the names event.h's commands
/// expand to.
pub(crate) struct Layer {
    helpers: Vec<Helper>,
    commanded: BTreeSet<String>,
}

impl Layer {
    pub(crate) fn new(helpers: Vec<Helper>, commanded: BTreeSet<String>) -> Layer {
        Layer { helpers, commanded }
    }

    pub(crate) fn load(root: &Path) -> Layer {
        let mut helpers = Vec::new();
        for header in SCRIPT_LAYER.iter().filter(|h| h.starts_with("games/tbs/")) {
            if let Ok(text) = std::fs::read_to_string(root.join(header)) {
                helpers.extend(self::helpers(header, &text));
            }
        }
        let event = std::fs::read_to_string(root.join(EVENT_H)).unwrap_or_default();
        Layer::new(helpers, commands(&event).into_iter().map(|c| c.callee).collect())
    }

    fn helper(&self, name: &str) -> Option<&Helper> {
        self.helpers.iter().find(|h| h.spelling == name || h.name == name)
    }

    /// The routine a name finally calls, through every inline.
    fn base(&self, name: &str) -> String {
        let mut name = name.to_string();
        for _ in 0..16 {
            match self.helper(&name) {
                Some(helper) => name.clone_from(&helper.callee),
                None => break,
            }
        }
        name
    }

    fn depth(&self, helper: &Helper) -> usize {
        let mut depth = 1;
        let mut name = helper.callee.as_str();
        while let Some(next) = self.helper(name).filter(|_| depth < 16) {
            depth += 1;
            name = &next.callee;
        }
        depth
    }

    /// Which of two inlines of one routine a call goes through: the one an
    /// event.h command expands to, then the outermost, then the first.
    fn rank(&self, helper: &Helper) -> (bool, Reverse<usize>, usize) {
        let order = self.helpers.iter().position(|h| h.name == helper.name).unwrap_or(usize::MAX);
        (!self.commanded.contains(&helper.spelling), Reverse(self.depth(helper)), order)
    }

    /// The inline a call of `name` with `arity` arguments should go
    /// through, among those `visible` admits, when the call does not go
    /// through it already or through one preferred to it.
    pub(crate) fn route(&self, name: &str, arity: usize, visible: &dyn Fn(&Helper) -> bool) -> Option<&Helper> {
        let base = self.base(name);
        let best = self.helpers.iter().filter(|h| h.arity == arity && visible(h) && self.base(&h.name) == base).min_by_key(|h| self.rank(h))?;
        match self.helper(name) {
            Some(current) if self.rank(current) <= self.rank(best) => None,
            _ => Some(best),
        }
    }
}

/// A call in a function body: its callee's token, its parentheses, its
/// arguments (token ranges) and the function it is in, by index and name.
struct Call {
    name: usize,
    open: usize,
    close: usize,
    args: Vec<Range<usize>>,
    function: usize,
    within: String,
}

/// Whether the name at `k`, before a `(`, is a member or what a
/// declaration declares rather than a callee: `void Actor_FaceDirection();`
/// or `struct FieldActor *Engine_ActorLookup(s32);` in a body.
fn not_called(tokens: &[Token], body: usize, k: usize) -> bool {
    let before = &tokens[k - 1];
    if matches!(punct(before), Some("." | "->")) {
        return true;
    }
    if let Some(word) = ident(before) {
        return !matches!(word, "return" | "else" | "case" | "do");
    }
    if punct(before) != Some("*") {
        return false;
    }
    let start = (body..k).rev().find(|&j| matches!(punct(&tokens[j]), Some(";" | "{" | "}"))).map_or(body, |j| j + 1);
    start < k - 1 && tokens[start..k - 1].iter().all(|t| ident(t).is_some() || punct(t) == Some("*"))
}

/// Every call in the function bodies of the code tokens.
fn calls(tokens: &[Token]) -> Vec<Call> {
    let mut out = Vec::new();
    for (function, (within, _, _, _, open)) in scan_definitions(tokens).into_iter().enumerate() {
        let Some(end) = matching(tokens, open) else {
            continue;
        };
        for k in open + 1..end {
            if ident(&tokens[k]).is_none() || tokens.get(k + 1).and_then(punct) != Some("(") || not_called(tokens, open, k) {
                continue;
            }
            let Some(close) = matching(tokens, k + 1) else {
                continue;
            };
            let args = pieces(&tokens[k + 2..close]).into_iter().map(|r| r.start + k + 2..r.end + k + 2).collect();
            out.push(Call { name: k, open: k + 1, close, args, function, within: within.clone() });
        }
    }
    out
}

/// The source text of a token range, as written.
fn spelled<'a>(text: &'a str, tokens: &[Token], range: &Range<usize>) -> &'a str {
    if range.is_empty() {
        return "";
    }
    &text[tokens[range.start].start..tokens[range.end - 1].end]
}

/// An integer literal's value: decimal, octal or hex, with no suffix.
fn integer(literal: &str) -> Option<i64> {
    if let Some(hex) = literal.strip_prefix("0x").or_else(|| literal.strip_prefix("0X")) {
        i64::from_str_radix(hex, 16).ok()
    } else if literal.len() > 1 && literal.starts_with('0') {
        i64::from_str_radix(&literal[1..], 8).ok()
    } else {
        literal.parse().ok()
    }
}

fn number(token: &Token) -> Option<i64> {
    match &token.tok {
        Tok::Number(n) => integer(n),
        _ => None,
    }
}

/// Tokens compared by meaning: integers by value.
fn normal(tokens: &[Token]) -> Vec<String> {
    tokens
        .iter()
        .map(|t| match &t.tok {
            Tok::Number(n) => integer(n).map_or_else(|| n.clone(), |v| v.to_string()),
            Tok::Ident(n) | Tok::Char(n) | Tok::Str(n) => n.clone(),
            Tok::Punct(p) => (*p).to_string(),
            Tok::Comment(_) => String::new(),
        })
        .collect()
}

/// The operators that bind more loosely than a shift.
const LOOSER: &[&str] = &[
    "<", ">", "<=", ">=", "==", "!=", "&", "^", "|", "&&", "||", "?", ":", "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", ",",
];

/// A fixed-point coordinate written in pixels: a literal that is a whole
/// number of pixels, `PIXELS(e)` or `e << 16` (each perhaps in
/// parentheses); none for every other shape.
fn pixels(text: &str, tokens: &[Token]) -> Option<String> {
    let tokens = whole(tokens);
    let range = |r: Range<usize>| spelled(text, tokens, &r).to_string();
    match tokens {
        [n] | [_, n] if number(n).is_some() && (tokens.len() == 1 || punct(&tokens[0]) == Some("-")) => {
            let value = number(n)?;
            // A literal past s32's range is a negative coordinate written
            // as its bits: no number of pixels says it.
            if value % 0x10000 != 0 || value > i64::from(i32::MAX) {
                return None;
            }
            let pixels = value / 0x10000;
            Some(if tokens.len() == 2 && pixels != 0 { format!("-{pixels}") } else { pixels.to_string() })
        }
        [name, open, .., _] if ident(name) == Some("PIXELS") && punct(open) == Some("(") && matching(tokens, 1) == Some(tokens.len() - 1) => {
            Some(range(2..tokens.len() - 1))
        }
        [.., shift, sixteen] if punct(shift) == Some("<<") && number(sixteen) == Some(16) => {
            let shifted = &tokens[..tokens.len() - 2];
            let mut depth = 0i32;
            for token in shifted {
                match punct(token) {
                    Some("(" | "[") => depth += 1,
                    Some(")" | "]") => depth -= 1,
                    Some(p) if depth == 0 && LOOSER.contains(&p) => return None,
                    _ => {}
                }
            }
            let inner = whole(shifted);
            (!inner.is_empty()).then(|| spelled(text, inner, &(0..inner.len())).to_string())
        }
        _ => None,
    }
}

/// How a command's argument comes from one of its parameters.
#[derive(Clone, Debug, PartialEq, Eq)]
enum Template {
    /// The parameter itself.
    Parameter(usize),
    /// The parameter shifted into 16.16 fixed point: pixels.
    Pixels(usize),
    /// The parameter under a cast, whose tokens are kept.
    Cast(Vec<String>, usize),
    /// A value the command fixes.
    Constant(Vec<String>),
}

/// An event.h command that expands to one call.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Command {
    pub name: String,
    pub parameters: Vec<String>,
    pub callee: String,
    args: Vec<Template>,
}

fn template(tokens: &[Token], parameters: &[String]) -> Option<Template> {
    let parameter = |tokens: &[Token]| match whole(tokens) {
        [token] => ident(token).and_then(|name| parameters.iter().position(|p| p == name)),
        _ => None,
    };
    let tokens = whole(tokens);
    if let Some(p) = parameter(tokens) {
        return Some(Template::Parameter(p));
    }
    if let [shifted @ .., shift, sixteen] = tokens {
        if punct(shift) == Some("<<") && number(sixteen) == Some(16) {
            return parameter(shifted).map(Template::Pixels);
        }
    }
    if tokens.first().and_then(punct) == Some("(") {
        if let Some(close) = cast_at(tokens, 0) {
            if let Some(p) = parameter(&tokens[close + 1..]) {
                return Some(Template::Cast(normal(&tokens[..=close]), p));
            }
        }
    }
    let named = tokens.iter().any(|t| ident(t).is_some_and(|n| parameters.iter().any(|p| p == n)));
    (!named).then(|| Template::Constant(normal(tokens)))
}

/// The commands an event.h defines that expand to exactly one call whose
/// arguments are their parameters, perhaps shifted into fixed point or
/// cast, or fixed values; a command that drops a parameter is left out.
pub(crate) fn commands(event_h: &str) -> Vec<Command> {
    let mut lines: Vec<String> = Vec::new();
    let mut joined = String::new();
    for line in event_h.lines() {
        if let Some(rest) = line.strip_suffix('\\') {
            joined.push_str(rest);
            joined.push(' ');
            continue;
        }
        joined.push_str(line);
        lines.push(std::mem::take(&mut joined));
    }
    let mut out = Vec::new();
    for line in lines {
        let Some(rest) = line.trim_start().strip_prefix('#').map(str::trim_start).and_then(|r| r.strip_prefix("define")).map(str::trim_start) else {
            continue;
        };
        let name: String = rest.chars().take_while(|c| c.is_ascii_alphanumeric() || *c == '_').collect();
        let Some((list, body)) = rest[name.len()..].strip_prefix('(').and_then(|r| r.split_once(')')) else {
            continue;
        };
        let parameters: Vec<String> = list.split(',').map(str::trim).filter(|p| !p.is_empty()).map(str::to_string).collect();
        let Ok(tokens) = lex(body) else {
            continue;
        };
        let tokens = super::numbers::code(tokens);
        let Some(callee) = tokens.first().and_then(ident) else {
            continue;
        };
        if tokens.get(1).and_then(punct) != Some("(") || matching(&tokens, 1) != Some(tokens.len() - 1) {
            continue;
        }
        let inside = &tokens[2..tokens.len() - 1];
        let Some(args) = pieces(inside).into_iter().map(|piece| template(&inside[piece], &parameters)).collect::<Option<Vec<_>>>() else {
            continue;
        };
        let used = |p: usize| args.iter().any(|a| matches!(a, Template::Parameter(q) | Template::Pixels(q) | Template::Cast(_, q) if *q == p));
        if (0..parameters.len()).all(used) {
            out.push(Command { name, parameters, callee: callee.to_string(), args });
        }
    }
    out
}

/// The command a call is, as an edit, or none when the call has another
/// shape.
fn command_edit(text: &str, tokens: &[Token], call: &Call, command: &Command) -> Option<Edit> {
    if ident(&tokens[call.name]) != Some(command.callee.as_str()) || call.args.len() != command.args.len() {
        return None;
    }
    // Each parameter's text and the argument it is written in place of.
    let mut bound: Vec<Option<(String, usize)>> = vec![None; command.parameters.len()];
    for (j, (arg, template)) in call.args.iter().zip(&command.args).enumerate() {
        let tokens_of = &tokens[arg.clone()];
        let (p, value) = match template {
            Template::Constant(fixed) => {
                if normal(tokens_of) != *fixed {
                    return None;
                }
                continue;
            }
            Template::Parameter(p) => (*p, spelled(text, tokens, arg).to_string()),
            Template::Pixels(p) => (*p, pixels(text, tokens_of)?),
            Template::Cast(cast, p) => {
                let close = (tokens_of.first().and_then(punct) == Some("("))
                    .then(|| cast_at(tokens_of, 0))
                    .flatten()
                    .filter(|&c| normal(&tokens_of[..=c]) == *cast && super::casts::unary_end(tokens_of, c + 1) == Some(tokens_of.len() - 1));
                (*p, close.map_or_else(|| spelled(text, tokens, arg).to_string(), |c| spelled(text, tokens, &(arg.start + c + 1..arg.end)).to_string()))
            }
        };
        if value.is_empty() {
            return None;
        }
        let squash = |s: &str| s.split_whitespace().collect::<String>();
        match bound[p].as_ref().map(|(before, _)| squash(before) == squash(&value)) {
            Some(false) => return None,
            Some(true) => {}
            None => bound[p] = Some((value, j)),
        }
    }
    let bound: Vec<(String, usize)> = bound.into_iter().collect::<Option<_>>()?;
    let name = &tokens[call.name];
    let unchanged = bound.iter().enumerate().all(|(p, (value, j))| *j == p && value == spelled(text, tokens, &call.args[*j])) && bound.len() == call.args.len();
    if unchanged {
        return Some(Edit { start: name.start, end: name.end, text: command.name.clone() });
    }
    let mut out = command.name.clone();
    out.push_str(&text[name.end..tokens[call.open].end]);
    if bound.windows(2).all(|w| w[0].1 < w[1].1) {
        // In place: each kept argument keeps the separator before it.
        let mut last = None;
        for (value, j) in &bound {
            let from = match last {
                None => tokens[call.open].end,
                Some(_) => tokens[call.args[j - 1].end - 1].end,
            };
            out.push_str(&text[from..tokens[call.args[*j].start].start]);
            out.push_str(value);
            last = Some(*j);
        }
        let from = call.args.last().map_or(tokens[call.open].end, |a| tokens[a.end - 1].end);
        out.push_str(&text[from..tokens[call.close].end]);
    } else {
        out.push_str(&bound.iter().map(|(value, _)| value.as_str()).collect::<Vec<_>>().join(", "));
        out.push(')');
    }
    Some(Edit { start: name.start, end: tokens[call.close].end, text: out })
}

/// The include lines a text needs, as one edit: each name inserted with the
/// first includes, in order, above the first include of code (`code` tells
/// it by its name and whether it is quoted).
fn include_edit(text: &str, names: &[String], code: &dyn Fn(&str, bool) -> bool) -> Option<Edit> {
    let mut out = text.to_string();
    for name in names {
        out = source::insert_include_before_code(&out, name, code);
    }
    let added = out.len().checked_sub(text.len()).filter(|&n| n > 0)?;
    let mut at = text.bytes().zip(out.bytes()).take_while(|(a, b)| a == b).count();
    while at > 0 && text.as_bytes()[at - 1] != b'\n' {
        at -= 1;
    }
    let edit = Edit { start: at, end: at, text: out.get(at..at + added)?.to_string() };
    (trial::apply(text, std::slice::from_ref(&edit)) == out).then_some(edit)
}

/// Where a file stands: the C files that compile it, the headers every one
/// of them reaches, and whether they are all overlay units, all scenes.
struct Place {
    units: Vec<PathBuf>,
    reached: BTreeSet<PathBuf>,
    overlay: bool,
    scene: bool,
    search: Vec<PathBuf>,
}

/// ☀️'s overlay units: the C files of the object list's `unit` rows.
fn overlay_units(root: &Path) -> BTreeSet<PathBuf> {
    let text = std::fs::read_to_string(root.join("games/tbs/objects.tsv")).unwrap_or_default();
    text.lines()
        .filter_map(|line| {
            let mut columns = line.split('\t');
            let (_, place, object) = (columns.next()?, columns.next()?, columns.next()?);
            (place == "unit").then(|| root.join(format!("{object}.c")))
        })
        .filter_map(|path| path.canonicalize().ok())
        .collect()
}

impl Place {
    fn of(root: &Path, file: &Path, games: &[DecompTarget]) -> Result<Place, String> {
        let search: Vec<PathBuf> = games
            .iter()
            .flat_map(|game| cflags_for_target_source(game.compiler, "unit.c"))
            .filter_map(|flag| flag.strip_prefix("-I").map(PathBuf::from))
            .collect();
        let units: Vec<PathBuf> = if file.extension().is_some_and(|e| e == "c") { Vec::new() } else { trial::including(root, file, games) };
        let compiled: Vec<PathBuf> = if units.is_empty() { vec![file.canonicalize().map_err(|e| e.to_string())?] } else { units.clone() };
        if file.extension().is_some_and(|e| e != "c") && units.is_empty() {
            return Err("no C file includes it".into());
        }
        let mut reached: Option<BTreeSet<PathBuf>> = None;
        for unit in &compiled {
            let here = source::reachable(unit, &search);
            reached = Some(match reached {
                None => here,
                Some(before) => before.intersection(&here).cloned().collect(),
            });
        }
        let overlays = overlay_units(root);
        let scenes = root.join(SCENES);
        let in_scenes = |path: &Path| path.canonicalize().is_ok_and(|p| p.starts_with(scenes.canonicalize().unwrap_or_else(|_| scenes.clone())));
        Ok(Place {
            overlay: compiled.iter().all(|u| overlays.contains(&u.canonicalize().unwrap_or_else(|_| u.clone()))),
            scene: in_scenes(file) && compiled.iter().all(|u| in_scenes(u)),
            reached: reached.unwrap_or_default(),
            units,
            search,
        })
    }

    fn reaches(&self, root: &Path, header: &str) -> bool {
        root.join(header).canonicalize().is_ok_and(|h| self.reached.contains(&h))
    }

    /// Whether an include of `file` brings in code rather than a header:
    /// a C file, or a header that defines functions (C text), such as the
    /// shared scene source a scene includes under its header includes.
    fn code(&self, file: &Path, name: &str, quoted: bool) -> bool {
        let folder = file.parent().unwrap_or(Path::new("."));
        source::resolve(name, quoted, folder, &self.search).is_some_and(|path| crate::c_items::c_text_file(&path))
    }
}

/// The edits of one rewrite, in one group per function, with the edits
/// FAKEMATCH-tagged code covers left out.
fn grouped(text: &str, found: Vec<(usize, Edit)>) -> Vec<Vec<Edit>> {
    let mut groups: Vec<Vec<Edit>> = Vec::new();
    for (function, edit) in found {
        if groups.len() <= function {
            groups.resize_with(function + 1, Vec::new);
        }
        groups[function].push(edit);
    }
    groups.into_iter().map(|group| trial::disjoint(trial::untagged(text, group))).filter(|g| !g.is_empty()).collect()
}

/// The layer edits of a text: each call routed through the inline `layer`
/// prefers among those `visible` admits, and the headers of the inlines
/// used.
fn layer_edits(text: &str, layer: &Layer, visible: &dyn Fn(&Helper) -> bool) -> (Vec<Vec<Edit>>, BTreeSet<String>) {
    let Ok(tokens) = lex(text) else {
        return (Vec::new(), BTreeSet::new());
    };
    let tokens = super::numbers::code(tokens);
    let mut found = Vec::new();
    let mut headers = BTreeSet::new();
    for call in calls(&tokens) {
        let name = ident(&tokens[call.name]).unwrap_or("");
        // An inline the file defines itself never goes through itself.
        if let Some(helper) = layer.route(name, call.args.len(), visible).filter(|h| h.name != call.within && h.spelling != call.within) {
            let token = &tokens[call.name];
            found.push((call.function, Edit { start: token.start, end: token.end, text: helper.spelling.clone() }));
            headers.insert(helper.header.clone());
        }
    }
    (grouped(text, found), headers)
}

/// The command edits of a text: each call an event.h command expands to,
/// as the first command whose shape it has.
pub(crate) fn command_edits(text: &str, commands: &[Command]) -> Vec<Vec<Edit>> {
    let Ok(tokens) = lex(text) else {
        return Vec::new();
    };
    let tokens = super::numbers::code(tokens);
    let found = calls(&tokens)
        .iter()
        .filter_map(|call| Some((call.function, commands.iter().find_map(|command| command_edit(text, &tokens, call, command))?)))
        .collect();
    grouped(text, found)
}

fn threads() -> usize {
    std::thread::available_parallelism().map_or(1, |n| (n.get() / 2).max(1))
}

/// The text with its layer edits (`layer`) or command edits (`commands`)
/// that leave every compile unchanged, and how many there were.
pub(crate) fn rewrite(root: &Path, file: &Path, text: &str, which: &str, games: &[DecompTarget]) -> Result<(String, usize), String> {
    let relative = file.strip_prefix(root).map_err(|_| format!("{} is outside {}", file.display(), root.display()))?.to_string_lossy().replace('\\', "/");
    if !relative.starts_with("games/tbs/") {
        return Err("the layer and the commands are ☀️'s".into());
    }
    if relative == EVENT_H || SCRIPT_LAYER.contains(&relative.as_str()) {
        return Err("a layer header defines the layer".into());
    }
    let place = Place::of(root, file, games)?;
    let event = std::fs::read_to_string(root.join(EVENT_H)).map_err(|e| format!("{EVENT_H}: {e}"))?;
    let commands = commands(&event);
    let event_path = root.join(EVENT_H).canonicalize().map_err(|e| e.to_string())?;
    let (groups, includes, fallback) = match which {
        "layer" => {
            if !place.overlay {
                return Err("not an overlay unit, or included by one that is not".into());
            }
            let layer = Layer::load(root);
            let allowed = |h: &Helper| place.scene || !SCENE_LAYER.contains(&h.header.as_str());
            let reached: BTreeSet<&str> = SCRIPT_LAYER.iter().copied().filter(|h| place.reaches(root, h)).collect();
            let (groups, needed) = layer_edits(text, &layer, &|h| allowed(h) && (reached.contains(h.header.as_str()) || h.header.starts_with(INCLUDABLE)));
            let needed: Vec<String> = needed.into_iter().filter(|h| !reached.contains(h.as_str())).collect();
            let through_event = source::reachable(&event_path, &place.search);
            let event_holds = |h: &String| root.join(h).canonicalize().is_ok_and(|h| h == event_path || through_event.contains(&h));
            let includes: Vec<PathBuf> = if needed.is_empty() {
                Vec::new()
            } else if place.scene && !place.reached.contains(&event_path) && needed.iter().all(event_holds) {
                vec![event_path.clone()]
            } else {
                let paths: Vec<PathBuf> = needed.iter().filter_map(|h| root.join(h).canonicalize().ok()).collect();
                paths.iter().filter(|h| !paths.iter().any(|other| other != *h && source::reachable(other, &place.search).contains(*h))).cloned().collect()
            };
            let (fallback, _) = layer_edits(text, &layer, &|h| allowed(h) && reached.contains(h.header.as_str()));
            (groups, includes, Some(fallback))
        }
        "commands" => {
            if !place.scene {
                return Err("not a scene, or included by a file that is not".into());
            }
            let includes = if place.reached.contains(&event_path) { Vec::new() } else { vec![event_path.clone()] };
            (command_edits(text, &commands), includes, None)
        }
        other => return Err(format!("unknown rewrite {other}")),
    };
    if groups.is_empty() && fallback.as_ref().is_none_or(Vec::is_empty) {
        return Ok((text.to_string(), 0));
    }
    let mut names = Vec::new();
    for header in &includes {
        names.push(source::spelling(file, header, &place.search).ok_or_else(|| format!("cannot spell an include of {}", header.display()))?);
    }
    let code = |name: &str, quoted: bool| place.code(file, name, quoted);
    let required: Vec<Edit> = if names.is_empty() { Vec::new() } else { vec![include_edit(text, &names, &code).ok_or("cannot place the include")?] };
    let mut judge = Judge::new(root, file, text, games, &place.units, threads())?;
    // What the includes bring in for the first time warns about its own
    // lines as it would anywhere; the file's own messages still count.
    let brought: BTreeSet<PathBuf> = includes.iter().flat_map(|h| source::reachable(h, &place.search).into_iter().chain([h.clone()])).filter(|h| !place.reached.contains(h)).collect();
    judge.quiet(brought.iter().filter_map(|h| h.strip_prefix(root).ok()).map(|h| h.to_string_lossy().into_owned()).collect());
    let passes = |candidate: &str| judge.passes(root, candidate);
    if let Some(done) = trial::proved_groups(text, &required, groups, &passes)? {
        return Ok(done);
    }
    let why = judge.difference(root, &trial::apply(text, &required)).unwrap_or_default();
    let names = names.iter().map(|n| format!("#include \"{n}\"")).collect::<Vec<_>>().join(", ");
    match fallback {
        Some(groups) => {
            eprintln!("{relative}: {names} would change a compile ({why}); only the inlines it reaches are used");
            Ok(trial::proved_groups(text, &[], groups, &passes)?.unwrap_or_else(|| (text.to_string(), 0)))
        }
        None => Err(format!("{names} changes a compile ({why})")),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const HEADER: &str = "\
void Engine_EventWait(s32 frames);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
static inline void Event_Wait(s32 frames)
{
\tEngine_EventWait(frames);
}
static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
\treturn FieldScheduler_AddCallback(callback, priority);
}
static inline s32 FieldScheduler_AddOrUpdateCallback(s32 a0, s32 a1)
{
\treturn FieldScheduler_AddCallback(a0, a1);
}
static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
\tEngine_ActorSetDestination(actor, x, z);
}
static inline void FieldObjectMotion_ResetAndSetPosition(s32 a0, s32 a1, s32 a2)
{
\tEngine_ActorSetDestination(a0, a1, a2);
}
static inline s32 FieldObject_CheckMovementCollision(struct FieldActor *actor, struct FieldPosition *position)
{
\treturn Object_CheckMovementCollision((struct ObjectRuntime *)actor, position);
}
#define GameFlag_Set FieldEvent_GameFlagSet
static inline void FieldEvent_GameFlagSet(s32 flag)
{
\tEngine_GameFlagSet(flag);
}
static inline void Event_SkipMessages(s32 count)
{
\tu16 *message = &gEventWork->message;

\t*message = *message + count;
}
static inline s32 Swapped(s32 a, s32 b)
{
\treturn Target(b, a);
}
static inline void Pointer(void (*f)(s32), s32 x)
{
\tf(x);
}
#ifndef SWITCH
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
\tEngine_ActorSetSpeed(actor, speed, acceleration);
}
#else
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
\tEngine_ActorSetSpeed(actor, speed, acceleration);
}
#endif
";

    const ENGCALL: &str = "\
static inline s32 FieldScheduler_AddCallback(SchedulerCallback callback, s32 priority)
{
\treturn Scheduler_AddOrUpdateCallback(callback, priority);
}
";

    const EVENT: &str = "\
#define WAIT(n) Event_Wait(n)
#define MES_SET(m) Event_SetMessage((s32)(m))
#define OBJ_POS_SET(no, x, z) Actor_SetPosition((no), (x) << 16, (z) << 16)
#define OBJ_MOVE(no, x, z) Actor_SetDestination((no), (x), (z))
#define CAM_MOVE(x, z, f) Camera_MoveTo((x) << 16, -1, (z) << 16, (f))
#define OBJ_GET(no) ((union FieldObject *)Object_GetById(no))
#define FLAG_SET(f) GameFlag_Set(f)
#define TASK_ADD(fn, pri) Task_AddCallback((fn), (pri))
#define EV_BEGIN() Event_Begin()
#define LONG(a, \\
\tb) Long((a), (b))
#define HALF(no, x) Half((no))
";

    fn layer() -> Layer {
        let mut helpers = self::helpers("games/tbs/include/field_service.h", HEADER);
        helpers.extend(self::helpers("games/tbs/include/engcall.h", ENGCALL));
        Layer::new(helpers, commands(EVENT).into_iter().map(|c| c.callee).collect())
    }

    fn routed(text: &str, visible: &dyn Fn(&Helper) -> bool) -> String {
        let (groups, _) = layer_edits(text, &layer(), visible);
        trial::apply(text, &groups.concat())
    }

    fn commanded(text: &str) -> String {
        trial::apply(text, &command_edits(text, &commands(EVENT)).concat())
    }

    #[test]
    fn forwarders_are_inlines_that_pass_their_parameters_in_order() {
        let found = helpers("h", HEADER);
        let names: Vec<&str> = found.iter().map(|h| h.name.as_str()).collect();
        assert_eq!(
            names,
            [
                "Event_Wait",
                "Task_AddCallback",
                "FieldScheduler_AddOrUpdateCallback",
                "Actor_SetDestination",
                "FieldObjectMotion_ResetAndSetPosition",
                "FieldObject_CheckMovementCollision",
                "FieldEvent_GameFlagSet",
                "Actor_SetSpeed"
            ]
        );
        let flag = found.iter().find(|h| h.name == "FieldEvent_GameFlagSet").unwrap();
        assert_eq!((flag.spelling.as_str(), flag.callee.as_str(), flag.arity), ("GameFlag_Set", "Engine_GameFlagSet", 1));
        let task = found.iter().find(|h| h.name == "Task_AddCallback").unwrap();
        assert_eq!((task.callee.as_str(), task.arity), ("FieldScheduler_AddCallback", 2));
    }

    #[test]
    fn calls_go_through_the_preferred_inline_of_their_routine() {
        let all = |_: &Helper| true;
        let text = "void F(void)\n{\n\tEngine_EventWait(30);\n\tEngine_ActorSetDestination(8, 192, 264);\n\tFieldObjectMotion_ResetAndSetPosition(9, 1, 2);\n\tEngine_GameFlagSet(0x200);\n\tScheduler_AddOrUpdateCallback(Step, 0xc80);\n\tFieldScheduler_AddCallback(Step, 0xc80);\n\tFieldScheduler_AddOrUpdateCallback((s32)Step, 0xc80);\n\tEngine_ActorSetPosition(1, 2);\n\tEvent_Wait(1);\n}\n";
        assert_eq!(
            routed(text, &all),
            "void F(void)\n{\n\tEvent_Wait(30);\n\tActor_SetDestination(8, 192, 264);\n\tActor_SetDestination(9, 1, 2);\n\tGameFlag_Set(0x200);\n\tTask_AddCallback(Step, 0xc80);\n\tTask_AddCallback(Step, 0xc80);\n\tTask_AddCallback((s32)Step, 0xc80);\n\tEngine_ActorSetPosition(1, 2);\n\tEvent_Wait(1);\n}\n"
        );
        // Without the scene layer, the engine call layer alone.
        let engine = |h: &Helper| h.header.ends_with("engcall.h");
        assert_eq!(routed("void F(void)\n{\n\tScheduler_AddOrUpdateCallback(Step, 3200);\n\tEngine_EventWait(1);\n}\n", &engine), "void F(void)\n{\n\tFieldScheduler_AddCallback(Step, 3200);\n\tEngine_EventWait(1);\n}\n");
    }

    #[test]
    fn declarations_members_definitions_and_tagged_code_are_not_routed() {
        let all = |_: &Helper| true;
        for text in [
            "void Engine_EventWait(s32 frames);\n",
            "void F(void)\n{\n\tvoid Engine_EventWait();\n\tstruct FieldActor *Engine_EventWait(s32);\n\to->Engine_EventWait(1);\n}\n",
            "void F(void)\n{\n\t/* FAKEMATCH: the direct call keeps r4 */\n\tEngine_EventWait(1);\n}\n",
            "#define W(n) Engine_EventWait(n)\n",
        ] {
            assert_eq!(routed(text, &all), text);
        }
        assert_eq!(routed("void F(void)\n{\n\tx = a * Engine_EventWait(1);\n}\n", &all), "void F(void)\n{\n\tx = a * Event_Wait(1);\n}\n");
    }

    #[test]
    fn commands_invert_event_h() {
        let found = commands(EVENT);
        let names: Vec<&str> = found.iter().map(|c| c.name.as_str()).collect();
        assert_eq!(names, ["WAIT", "MES_SET", "OBJ_POS_SET", "OBJ_MOVE", "CAM_MOVE", "FLAG_SET", "TASK_ADD", "EV_BEGIN", "LONG"]);
        let text = "void F(s32 a0)\n{\n\tEvent_Begin();\n\tEvent_Wait(30);\n\tEvent_SetMessage((s32)MsgHello);\n\tEvent_SetMessage(message + 1);\n\tGameFlag_Set(0x200);\n\tif (Task_AddCallback(Step, 0xc80) == 0)\n\t\tActor_SetDestination(a0, 1, 2);\n\tCamera_MoveTo(0x400000, -1, PIXELS(30), 1);\n\tCamera_MoveTo(0x400000, 0, 0x100000, 1);\n\tObject_GetById(3);\n}\n";
        assert_eq!(
            commanded(text),
            "void F(s32 a0)\n{\n\tEV_BEGIN();\n\tWAIT(30);\n\tMES_SET(MsgHello);\n\tMES_SET(message + 1);\n\tFLAG_SET(0x200);\n\tif (TASK_ADD(Step, 0xc80) == 0)\n\t\tOBJ_MOVE(a0, 1, 2);\n\tCAM_MOVE(64, 30, 1);\n\tCamera_MoveTo(0x400000, 0, 0x100000, 1);\n\tObject_GetById(3);\n}\n"
        );
    }

    /// The coordinate shapes left_unconverted.tsv and the scenes hold.
    #[test]
    fn positions_are_written_in_pixels_or_left_as_calls() {
        for (call, command) in [
            ("Actor_SetPosition(8, 0xC00000, 0x1080000)", "OBJ_POS_SET(8, 192, 264)"),
            ("Actor_SetPosition(8, -0x400000, 0)", "OBJ_POS_SET(8, -64, 0)"),
            ("Actor_SetPosition(MEARI, 12582912, -0x0)", "OBJ_POS_SET(MEARI, 192, 0)"),
            ("Actor_SetPosition(12, PIXELS(248), PIXELS(0x178))", "OBJ_POS_SET(12, 248, 0x178)"),
            ("Actor_SetPosition(a0, (p9 << 16), (p10 << 16))", "OBJ_POS_SET(a0, p9, p10)"),
            ("Actor_SetPosition(a0, x << 16, (z + 8) << 16)", "OBJ_POS_SET(a0, x, z + 8)"),
            ("Actor_SetPosition(a0, x * 16 << 16, z - 1 << 16)", "OBJ_POS_SET(a0, x * 16, z - 1)"),
        ] {
            let text = format!("void F(void)\n{{\n\t{call};\n}}\n");
            assert_eq!(commanded(&text), text.replace(call, command), "{call}");
        }
        for call in [
            "Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed)",
            "Actor_SetPosition(10, 206 << 18, 150 << 18)",
            "Actor_SetPosition(a0, (p9 << 16), ((p10 << 16) - 0x500000))",
            "Actor_SetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000)",
            "Actor_SetPosition(8, 0x18000, 0x100000)",
            "Actor_SetPosition(8, 0x100000u, 0x100000)",
            "Actor_SetPosition(8, a < b << 16, 0)",
            "Actor_SetPosition(8, 0x100000)",
            "Actor_SetPosition(8, 0xFFFF0000, 0)",
        ] {
            let text = format!("void F(void)\n{{\n\t{call};\n}}\n");
            assert_eq!(commanded(&text), text, "{call}");
        }
    }

    #[test]
    fn a_call_over_lines_keeps_its_line_breaks() {
        let text = "void F(void)\n{\n\tCamera_MoveTo(0x400000,\n\t\t-1, 0x100000,\n\t\t1);\n\tActor_SetPosition(\n\t\t8, 0x100000, 0x200000);\n}\n";
        assert_eq!(commanded(text), "void F(void)\n{\n\tCAM_MOVE(64, 16,\n\t\t1);\n\tOBJ_POS_SET(\n\t\t8, 16, 32);\n}\n");
    }

    /// kuupuappu_mura_sai.c's head: its header includes, then the shared
    /// scene source in ROM order under its comment, then code, and three
    /// headers below its first functions.
    const KUUPUAPPU_MURA_SAI: &str = "\
#include \"game_state.h\"
#include \"unit/scene/kuupuappu_mura_sai.h\"
#include \"types.h\"
#include \"field_event.h\"
#include \"edition.h\"
#include \"call.h\"

// ここに共通ソースが入る（ROM の並びのとおり）
#include \"common/effect_create.h\"
#include \"common/effect_spawn.h\"
#include \"common/object_distance.h\"
#include \"common/kuupuappu_range.h\"
#include \"common/kuupuappu_nearby.h\"

const void *SceneData_GetScriptTable(void)
{
\treturn KuupuappuMuraSai_Scripts;
}

#include \"common/kuupuappu_mura_scene_cells.h\"
#include \"ability_ids.h\"
#include \"item_ids.h\"
#include \"objcmd.h\"
";

    #[test]
    fn includes_go_with_the_header_includes_above_the_shared_source() {
        let code = |name: &str, _: bool| name.starts_with("common/");
        for header in ["event.h", "field_service.h"] {
            let edit = include_edit(KUUPUAPPU_MURA_SAI, &[header.to_string()], &code).unwrap();
            let expected = KUUPUAPPU_MURA_SAI.replace("#include \"call.h\"\n", &format!("#include \"call.h\"\n#include \"{header}\"\n"));
            assert_eq!(trial::apply(KUUPUAPPU_MURA_SAI, &[edit]), expected, "{header}");
        }
        let text = "#include \"event.hh\"\nvoid F(void);\n";
        let edit = include_edit(text, &["event.h".to_string(), "engcall.h".to_string()], &code).unwrap();
        assert_eq!(trial::apply(text, &[edit]), "#include \"event.hh\"\n#include \"event.h\"\n#include \"engcall.h\"\nvoid F(void);\n");
    }

    /// The tree tells code from headers: what the scene includes from
    /// scene/common defines functions, what it includes from the include
    /// folder does not.
    #[test]
    fn a_scene_includes_its_shared_source_as_code() {
        let root = crate::compiler::routing::root();
        let file = root.join("games/tbs/src/scene/kuupuappu_mura_sai.c");
        let tbs = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let place = Place::of(root, &file, &[tbs]).unwrap();
        assert!(place.code(&file, "common/effect_create.h", true));
        assert!(!place.code(&file, "call.h", true));
        assert!(!place.code(&file, "field_event.h", true));
    }

    /// A main-image file and a header the main image includes take no
    /// layer; an overlay that is no scene takes no commands.
    #[test]
    fn the_layer_is_for_overlays_and_the_commands_for_scenes() {
        let root = crate::compiler::routing::root();
        let tbs = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let refused = |path: &str, which: &str| {
            let file = root.join(path);
            let text = std::fs::read_to_string(&file).unwrap();
            rewrite(root, &file, &text, which, &[tbs]).err().unwrap_or_default()
        };
        assert!(refused("games/tbs/src/menu/menu.c", "layer").starts_with("not an overlay unit"));
        assert!(refused("games/tbs/src/scene/common/scroll_initialize_hblank_dma.h", "layer").starts_with("not an overlay unit"));
        assert!(refused("games/tbs/src/menu/title.c", "commands").starts_with("not a scene"));
        assert!(refused("games/tbs/include/field_service.h", "layer").starts_with("a layer header"));
        assert!(refused("games/tla/src/battle/actor.c", "layer").starts_with("the layer and the commands are"));
    }

    /// A tagged inline is a device, not the layer: no call is routed into
    /// it, though it forwards its parameters like the others.
    #[test]
    fn a_tagged_inline_takes_no_calls() {
        let header = "\
static inline void Event_Wait(s32 frames)
{
\tEngine_EventWait(frames);
}
static inline void Event_WaitLong(s32 frames)
{
\t/* FAKEMATCH: the inline keeps frames in r4 */
\tEvent_Wait(frames);
}
static inline void Actor_Tagged(s32 actor)
{
\t// FAKEMATCH: measured against the direct call
\tEngine_ActorTagged(actor);
}
static inline void Actor_Barrier(s32 actor)
{
\tasm(\"\");
\tEngine_ActorBarrier(actor);
}
#ifdef X
static inline void Actor_Split(s32 actor)
{
\tEngine_ActorSplit(actor);
}
#else
static inline void Actor_Split(s32 actor)
{
\t/* FAKEMATCH: measured */
\tEngine_ActorSplit(actor);
}
#endif
";
        let found = helpers("games/tbs/include/field_service.h", header);
        assert_eq!(found.iter().map(|h| h.name.as_str()).collect::<Vec<_>>(), ["Event_Wait"]);
        let layer = Layer::new(found, BTreeSet::new());
        let all = |_: &Helper| true;
        assert_eq!(layer.route("Engine_EventWait", 1, &all).map(|h| h.name.as_str()), Some("Event_Wait"));
        for name in ["Event_Wait", "Event_WaitLong", "Engine_ActorTagged", "Actor_Tagged", "Engine_ActorBarrier", "Engine_ActorSplit"] {
            assert!(layer.route(name, 1, &all).is_none(), "{name}");
        }
    }

    #[test]
    fn edits_group_by_function() {
        let text = "void F(void)\n{\n\tEvent_Wait(1);\n\tEvent_Wait(2);\n}\nvoid G(void)\n{\n\tEvent_Wait(3);\n}\n";
        let groups = command_edits(text, &commands(EVENT));
        assert_eq!(groups.iter().map(Vec::len).collect::<Vec<_>>(), [2, 1]);
    }

    /// Edits `a`, `b` and `c` match only together (EV21_TALK's calls), `x`
    /// never matches (a stay-direct site in the same function), `d` matches
    /// alone in another function.
    #[test]
    fn calls_that_match_only_together_are_kept_together() {
        let text = "a b x c d";
        let at = |k: usize, to: &str| Edit { start: k, end: k + 1, text: to.to_string() };
        let (a, b, x, c, d) = (at(0, "A"), at(2, "B"), at(4, "X"), at(6, "C"), at(8, "D"));
        let passes = |candidate: &str| {
            let together = ["A", "B", "C"].iter().filter(|e| candidate.contains(**e)).count();
            !candidate.contains('X') && (together == 0 || together == 3)
        };
        let (out, count) = trial::proved_groups(text, &[], vec![vec![a, b, x, c], vec![d]], &passes).unwrap().unwrap();
        assert_eq!((out.as_str(), count), ("A B x C D", 4));
        // A required edit that fails alone keeps nothing.
        let (y, z) = (at(0, "Y"), at(2, "Z"));
        assert_eq!(trial::proved_groups("a b", &[y], vec![vec![z]], &|candidate: &str| !candidate.contains('Y')).unwrap(), None);
    }

    /// A fixture of its own in a temporary folder, beside the headers it
    /// includes, judged in every ☀️ edition.
    struct Fixture {
        _folder: tempfile::TempDir,
        file: PathBuf,
    }

    impl Fixture {
        fn new(files: &[(&str, &str)]) -> Fixture {
            let folder = tempfile::tempdir().unwrap();
            for (name, text) in files {
                std::fs::write(folder.path().join(name), text).unwrap();
            }
            let file = folder.path().join("scene.c");
            Fixture { _folder: folder, file }
        }

        fn judge(&self, text: &str) -> Judge {
            let root = crate::compiler::routing::root();
            let tbs = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
            Judge::new(root, &self.file, text, &[tbs], &[], threads()).unwrap()
        }

        /// The edits the compiler proves, with these required edits.
        fn prove(&self, text: &str, required: &[Edit], groups: Vec<Vec<Edit>>) -> (String, usize) {
            let root = crate::compiler::routing::root();
            let judge = self.judge(text);
            trial::proved_groups(text, required, groups, &|candidate: &str| judge.passes(root, candidate)).unwrap().unwrap()
        }
    }

    /// The layer and the routines of the measured sites (korosseo_kawa.c's
    /// EV21_TALK and SceneState_SetMode66AndPassOpeningSequence,
    /// karei_machi.c's KareiMachi_SpeedUpActors, fune_heya.c's
    /// FuneHeya_ApplyEntryState and haidia_babi.c's MES_PUT), as the scene
    /// headers declare them.
    const MEASURED_LAYER: &str = "\
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef void (*SchedulerCallback)(void);
#define PIXELS(value) ((value) << 16)
struct FieldActor
{
\tu8 unknown_00[8];
\ts32 x;
\ts32 y;
\ts32 z;
\tu8 unknown_14[0x1c];
\ts32 speed;
\ts32 acceleration;
\tu8 unknown_38[0x1d];
\tu8 motion_flags;
};
struct GameState
{
\tu8 unknown_00[0x20];
\ts16 entrance;
};
extern struct GameState gGameState;
extern const u8 MsgPipeworks[];
extern const u8 MsgMakeGood[];
extern s32 KorosseoKawa_Countdown;
extern s32 KareiMachi_DanceStep;
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_TaskWait(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
void Engine_EventShowMessageAndWait(s32 speaker, s32 flags, s32 frames);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorSetPosition();
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraWaitForMove(void);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
struct FieldActor *Object_GetById(s32 actor);
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 priority);
void Object_CommitPosition(s32 object);
void Korosseo_FinishSoloRound(s32 id);
s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor);
void Korosseo_ShowFollowUpPrompt(s32 speaker, s32 competitor);
void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);
void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style);
void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration);
void SceneEffect_RemoveMarkerTask(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void KareiMachi_AlternateDance(void);
void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);
void FieldScene_RunSixStepSequence17e4(void);

static inline struct FieldActor *Actor_Get(s32 actor)
{
\treturn Object_GetById(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
\tEngine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
\tEngine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
\tEngine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Camera_WaitForMove(void)
{
\tEngine_CameraWaitForMove();
}

static inline void Event_Begin(void)
{
\tEngine_EventBegin();
}

static inline void Event_End(void)
{
\tEngine_EventEnd();
}

static inline void Event_SetMessage(s32 message)
{
\tEngine_EventSetMessage(message);
}

static inline void Event_ShowMessage(s32 speaker, s32 flags)
{
\tEngine_EventShowMessage(speaker, flags);
}

static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames)
{
\tEngine_EventShowMessageAndWait(speaker, flags, frames);
}

static inline void Event_Wait(s32 frames)
{
\tEngine_EventWait(frames);
}

static inline void Task_Wait(s32 frames)
{
\tEngine_TaskWait(frames);
}

static inline void FieldObject_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
\tEngine_ObjectSetPosition(object, fixed_x, fixed_y, fixed_z);
}

static inline s32 FieldScheduler_AddCallback(SchedulerCallback callback, s32 priority)
{
\treturn Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline s32 Task_AddCallback(void (*callback)(void), s32 priority)
{
\treturn FieldScheduler_AddCallback(callback, priority);
}
";

    /// EV21_TALK as korosseo_kawa.c has it: one camera move through the
    /// layer, every other service called directly.
    const EV21_TALK: &str = "\
void EV21_TALK(s32 a0)
{
\ts32\t\t\t\t   rec4;
\tstruct FieldActor *second;
\tstruct FieldActor *first;

\tif (gGameState.entrance == 2)
\t{
\t\tKorosseo_FinishSoloRound(a0);
\t}
\telse
\t{
\t\tEngine_EventBegin();
\t\trec4 = SceneDialogue_RunFlagGatedPromptInteraction(a0, 2);
\t\tif (rec4 == 0)
\t\t{
\t\t\tEngine_EventSetMessage((s32)MsgPipeworks);
\t\t\tEngine_CameraSetSpeed(0x30000, 0x6000);
\t\t\tCamera_MoveTo(0x2500000, -1, 0x780000, 1);
\t\t\tEngine_CameraWaitForMove();
\t\t\tEngine_EventWait(60);
\t\t\tEngine_CameraSetSpeed(0x18000, 0x3000);
\t\t\tEngine_CameraMoveTo(0x2600000, -1, 0xd80000, 1);
\t\t\tEngine_CameraWaitForMove();
\t\t\tEngine_EventShowMessage(a0, 0);
\t\t\tSceneEffect_SeedMarkerAndInstallTask(56, 64, 0);
\t\t\tEngine_EventWait(60);
\t\t\tSceneState_StartMarkerMove(160, 96, 10);
\t\t\tEngine_EventWait(70);
\t\t\tEngine_EventShowMessage(a0, 0);
\t\t\tSceneEffect_RemoveMarkerTask();
\t\t\tEngine_TaskWait(2);
\t\t\tfirst = Object_GetById(13);
\t\t\tfirst->motion_flags = 0;
\t\t\tfirst->acceleration = 0x6666;
\t\t\tfirst->speed = 0xcccc;
\t\t\tEngine_ObjectSetPosition(first, first->x, 0x80000, first->z);
\t\t\tsecond = Object_GetById(14);
\t\t\tsecond->motion_flags = 0;
\t\t\tsecond->acceleration = 0x6666;
\t\t\tsecond->speed = 0xcccc;
\t\t\tEngine_ObjectSetPosition(second, second->x, 0x200000, second->z);
\t\t\tObject_CommitPosition((s32)second);
\t\t\tEngine_EventWait(45);
\t\t\tfirst = Object_GetById(13);
\t\t\tfirst->motion_flags = 0;
\t\t\tfirst->acceleration = 0x6666;
\t\t\tfirst->speed = 0xcccc;
\t\t\tEngine_ObjectSetPosition(first, first->x, 0x180000, first->z);
\t\t\tsecond = Actor_Get(14);
\t\t\tsecond->motion_flags = 0;
\t\t\tsecond->acceleration = 0x6666;
\t\t\tsecond->speed = 0xcccc;
\t\t\tEngine_ObjectSetPosition(second, second->x, 0, second->z);
\t\t\tObject_CommitPosition((s32)second);
\t\t\tEngine_EventWait(15);
\t\t\tEngine_EventShowMessage(a0, 0);
\t\t\tSceneEffect_SeedMarkerAndInstallTask(56, 64, 0);
\t\t\tEngine_EventWait(30);
\t\t\tSceneState_StartMarkerMove(160, 96, 10);
\t\t\tEngine_EventWait(40);
\t\t\tSceneState_StartMarkerMove(56, 64, 10);
\t\t\tEngine_EventWait(70);
\t\t\tEngine_EventShowMessage(a0, 0);
\t\t\tSceneEffect_RemoveMarkerTask();
\t\t\tEngine_TaskWait(2);
\t\t\tEngine_CameraFollowActor(0, 0);
\t\t\tKorosseo_ShowFollowUpPrompt(a0, 2);
\t\t}
\t\telse if (rec4 == 1)
\t\t{
\t\t\tEngine_EventSetMessage((s32)MsgMakeGood);
\t\t\tEngine_EventShowMessage(a0, 0);
\t\t}
\t\tFieldScene_RunMiddleSequence(rec4, a0, 2);
\t\tEngine_EventEnd();
\t}
}
";

    /// The three TASK_ADD sites stay_direct.tsv measured, as the tree has
    /// them since the engine call layer: each calls FieldScheduler_AddCallback.
    const TASK_SITES: &str = "\
void SceneState_SetMode66AndPassOpeningSequence(void)
{
\tKorosseoKawa_Countdown = 66;
\tFieldScheduler_AddCallback((s32)FieldScene_RunOpeningAuxiliarySequence, 0xC80);
}

void KareiMachi_SpeedUpActors(void)
{
\tKareiMachi_DanceStep = 0;
\tFieldScheduler_AddCallback(KareiMachi_AlternateDance, 0xc80);
}

void FuneHeya_StartCueTimer(void)
{
\tFieldScheduler_AddCallback(Scene_UpdateCueTimer, 0xc80);
}
";

    /// haidia_babi.c's speaker 8 above its window: through the inline the
    /// speaker is loaded for each call; called directly it stays in r5.
    const PINNED: &str = "\
void HaidiaBabi_Wake(void)
{
\tEvent_ShowMessageAndWait(36872, 0, 20);
\tEngine_EventWait(20);
\tEvent_ShowMessage(36872, 0);
\tFieldScene_RunSixStepSequence17e4();
\tEngine_ActorStartRepeatedMotion(8, 2);
\tEvent_ShowMessageAndWait(36872, 0, 20);
}
";

    const MEASURED_EVENT: &str = "\
#define WAIT(n) Event_Wait(n)
#define FRAME_WAIT(n) Task_Wait(n)
#define MES_SET(m) Event_SetMessage(m)
#define MES_PUT(no, f) Event_ShowMessage((no), (f))
#define MES_PUT_WAIT(no, f, t) Event_ShowMessageAndWait((no), (f), (t))
#define OBJ_POS_SET(no, x, z) Actor_SetPosition((no), (x) << 16, (z) << 16)
#define CAM_SPD(s, a) Camera_SetSpeed((s), (a))
#define CAM_WAIT() Camera_WaitForMove()
#define TASK_ADD(fn, pri) Task_AddCallback((fn), (pri))
#define EV_BEGIN() Event_Begin()
#define EV_END() Event_End()
";

    fn measured_layer() -> Layer {
        Layer::new(helpers("layer.h", MEASURED_LAYER), commands(MEASURED_EVENT).into_iter().map(|c| c.callee).collect())
    }

    /// The measured cases, compiled: EV21_TALK keeps all of its calls on
    /// the layer though no smaller set of them matches; the two
    /// FieldScheduler_AddCallback sites whose callback's type GCC warns
    /// about stay, since the warning would name Task_AddCallback; the plain
    /// one goes through Task_AddCallback; the pinned MES_PUT's routine
    /// called directly stays direct.
    #[test]
    fn the_compiler_proves_the_measured_layer_sites() {
        let pinned_direct = PINNED.replace("\tEvent_ShowMessage(36872, 0);", "\tEngine_EventShowMessage(36872, 0);");
        let text = format!("#include \"layer.h\"\n\n{EV21_TALK}\n{TASK_SITES}\n{pinned_direct}");
        let fixture = Fixture::new(&[("layer.h", MEASURED_LAYER)]);
        let (groups, _) = layer_edits(&text, &measured_layer(), &|_| true);
        let (out, _) = fixture.prove(&text, &[], groups);
        let expected_ev21 = EV21_TALK
            .replace("Engine_EventBegin", "Event_Begin")
            .replace("Engine_EventEnd", "Event_End")
            .replace("Engine_EventWait", "Event_Wait")
            .replace("Engine_TaskWait", "Task_Wait")
            .replace("Engine_EventSetMessage", "Event_SetMessage")
            .replace("Engine_EventShowMessage", "Event_ShowMessage")
            .replace("Engine_CameraSetSpeed", "Camera_SetSpeed")
            .replace("Engine_CameraMoveTo", "Camera_MoveTo")
            .replace("Engine_CameraWaitForMove", "Camera_WaitForMove")
            .replace("Engine_ObjectSetPosition", "FieldObject_SetPosition")
            .replace("Object_GetById", "Actor_Get");
        let expected_tasks = TASK_SITES.replace("FieldScheduler_AddCallback(KareiMachi", "Task_AddCallback(KareiMachi");
        let expected_pinned = pinned_direct.replace("Engine_EventWait", "Event_Wait");
        assert_eq!(out, format!("#include \"layer.h\"\n\n{expected_ev21}\n{expected_tasks}\n{expected_pinned}"));
        // No smaller set of EV21_TALK's camera calls matches.
        let judge = fixture.judge(&text);
        let root = crate::compiler::routing::root();
        assert!(!judge.passes(root, &text.replacen("Engine_CameraSetSpeed(0x30000", "Camera_SetSpeed(0x30000", 1)));
        assert!(!judge.passes(root, &text.replace("Engine_CameraSetSpeed", "Camera_SetSpeed").replace("Engine_CameraMoveTo", "Camera_MoveTo")));
    }

    /// Commands over the layer, compiled: each call is its command, event.h
    /// goes with the first includes, and coordinates are pixels. A MES_PUT
    /// that expands to the direct call is no command for an inline call:
    /// the pinned site would change.
    #[test]
    fn the_compiler_proves_the_commands() {
        let positions = "void Place(s32 x, s32 z)\n{\n\tActor_SetPosition(8, 0xC00000, 0x1080000);\n\tActor_SetPosition(9, PIXELS(248), -0x400000);\n\tActor_SetPosition(10, x << 16, (z + 8) << 16);\n\tActor_SetPosition(11, 0x18000, 0);\n}\n";
        let text = format!("#include \"layer.h\"\n\n{PINNED}\n{positions}");
        let fixture = Fixture::new(&[("layer.h", MEASURED_LAYER), ("event.h", MEASURED_EVENT)]);
        let required = [include_edit(&text, &["event.h".to_string()], &|_, _| false).unwrap()];
        let (out, _) = fixture.prove(&text, &required, command_edits(&text, &commands(MEASURED_EVENT)));
        assert_eq!(
            out,
            "#include \"layer.h\"\n#include \"event.h\"\n\nvoid HaidiaBabi_Wake(void)\n{\n\tMES_PUT_WAIT(36872, 0, 20);\n\tEngine_EventWait(20);\n\tMES_PUT(36872, 0);\n\tFieldScene_RunSixStepSequence17e4();\n\tEngine_ActorStartRepeatedMotion(8, 2);\n\tMES_PUT_WAIT(36872, 0, 20);\n}\n\nvoid Place(s32 x, s32 z)\n{\n\tOBJ_POS_SET(8, 192, 264);\n\tOBJ_POS_SET(9, 248, -64);\n\tOBJ_POS_SET(10, x, z + 8);\n\tActor_SetPosition(11, 0x18000, 0);\n}\n"
        );
        let direct = "#define MES_PUT(no, f) Engine_EventShowMessage((no), (f))\n";
        assert!(command_edits(&text, &commands(direct)).is_empty());
        let root = crate::compiler::routing::root();
        assert!(!fixture.judge(&text).passes(root, &text.replace("\tEvent_ShowMessage(36872, 0);", "\tEngine_EventShowMessage(36872, 0);")));
    }
}
