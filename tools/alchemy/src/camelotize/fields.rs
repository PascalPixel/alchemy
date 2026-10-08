use super::source::{
    closing, functions, identifier, is_ident, mask, next, normal_type, opening, postfix, previous,
    split_commas, store, text, trim, type_name, word_at, word_before, words, Function,
};
use super::stabs::{Int, Kind, Member, Pointee};
use std::collections::{BTreeSet, HashMap, HashSet};
use std::ops::Range;

pub(crate) struct Spec {
    pub tag: &'static str,
    pub getters: &'static [&'static str],
    pub globals: &'static [&'static str],
    pub parameters: bool,
}

pub(crate) const ACTORS: Spec = Spec {
    tag: "FieldActor",
    getters: &["Object_GetById", "Actor_Get", "Engine_ActorGet"],
    globals: &[],
    parameters: true,
};

pub(crate) const EVENTS: Spec = Spec {
    tag: "EventWork",
    getters: &[],
    globals: &["gEventWork"],
    parameters: false,
};

pub(crate) struct Unit {
    pub layouts: Vec<Vec<Member>>,
    pub types: HashMap<String, Kind>,
    pub declared: HashSet<String>,
    pub macros: Vec<Macro>,
}

impl Unit {
    fn agreed<'a>(&'a self, pick: impl Fn(&'a [Member]) -> Option<&'a Member>) -> Option<&'a Member> {
        let mut found: Option<&Member> = None;
        for layout in &self.layouts {
            let member = pick(layout)?;
            if found.is_some_and(|other| other != member) {
                return None;
            }
            found = Some(member);
        }
        found
    }

    fn value(&self, offset: u32, ty: Int, store: bool) -> Option<&Member> {
        self.agreed(|layout| {
            let at = || layout.iter().filter(move |m| m.offset == offset);
            at().find(|m| m.kind == Kind::Int(ty)).or_else(|| {
                at().find(|m| store && matches!(m.kind, Kind::Int(int) if int.bytes == ty.bytes))
            })
        })
    }

    fn address(&self, offset: u32, ty: Int) -> Option<&Member> {
        self.agreed(|layout| {
            let at = || layout.iter().filter(move |m| m.offset == offset);
            at().find(|m| m.kind == Kind::Int(ty)).or_else(|| at().next())
        })
    }

    fn points_at(&self, name: &str, tag: &str) -> bool {
        matches!(self.types.get(name), Some(Kind::Pointer(Pointee::Record(t))) if t == tag)
    }

    fn may_hold(&self, name: &str, tag: &str) -> bool {
        !matches!(self.types.get(name), Some(Kind::Pointer(Pointee::Record(t))) if t != tag)
    }
}

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct Macro {
    pub name: String,
    base: usize,
    ty: usize,
    offset: usize,
    pointer: bool,
}

pub(crate) fn macros(text: &str) -> Vec<Macro> {
    let joined = text.replace("\\\n", " ");
    let mut found = Vec::new();
    for line in joined.lines() {
        let Some(rest) = line.trim_start().strip_prefix('#') else {
            continue;
        };
        let Some(rest) = rest.trim_start().strip_prefix("define") else {
            continue;
        };
        let rest = rest.trim_start();
        let (Some(open), Some(close)) = (rest.find('('), rest.find(')')) else {
            continue;
        };
        let name = &rest[..open];
        if name.is_empty() || !name.bytes().all(is_ident) || close < open {
            continue;
        }
        let params: Vec<&str> = rest[open + 1..close].split(',').map(str::trim).collect();
        if params.len() != 3 {
            continue;
        }
        let body: String = rest[close + 1..]
            .split("/*")
            .next()
            .unwrap_or("")
            .chars()
            .filter(|c| !c.is_whitespace())
            .collect();
        for (base, ty, offset) in [(0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)] {
            for pointer in [false, true] {
                let star = if pointer { "" } else { "*" };
                let shape = format!("*({}{star})((u8*)({})+({}))", params[ty], params[base], params[offset]);
                if body == shape || body == format!("({shape})") {
                    found.push(Macro { name: name.to_string(), base, ty, offset, pointer });
                }
            }
        }
    }
    found
}

#[derive(Clone, Debug, PartialEq)]
enum Form {
    Value { ty: Int, store: bool },
    Byte { store: bool },
    Address { cast: String, ty: Int },
    Bare,
}

#[derive(Clone, Debug)]
struct Access {
    span: Range<usize>,
    core: Range<usize>,
    cast: bool,
    offset: u32,
    form: Form,
}

const INTS: [&str; 6] = ["u8", "s8", "u16", "s16", "u32", "s32"];
const BYTE_CASTS: [&str; 6] = ["u8 *", "s8 *", "char *", "void *", "s32", "u32"];
const LOCAL_TYPES: [&str; 4] = ["s32", "u32", "u8 *", "void *"];

fn int_pointer(name: &str) -> Option<Int> {
    INTS.iter().find(|t| name.strip_suffix(" *") == Some(**t)).and_then(|t| Int::parse(t))
}

fn literal(mask: &[u8], range: Range<usize>) -> Option<u32> {
    let digits = text(mask, range).trim_end_matches(['u', 'U', 'l', 'L']);
    match digits.strip_prefix("0x").or_else(|| digits.strip_prefix("0X")) {
        Some(hex) => u32::from_str_radix(hex, 16).ok(),
        None if !digits.is_empty() && digits.bytes().all(|b| b.is_ascii_digit()) => digits.parse().ok(),
        None => None,
    }
}

fn sum(mask: &[u8], range: Range<usize>) -> Option<(Range<usize>, u32)> {
    let mut range = trim(mask, range);
    while !range.is_empty() && mask[range.start] == b'(' && closing(mask, range.start) == Some(range.end - 1) {
        range = trim(mask, range.start + 1..range.end - 1);
    }
    let (mut depth, mut plus) = (0i32, None);
    for i in range.clone() {
        match mask[i] {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth -= 1,
            b'+' if depth == 0 => plus = Some(i),
            _ => {}
        }
    }
    let plus = plus?;
    if plus == range.start || mask[plus - 1] == b'+' || matches!(mask.get(plus + 1), Some(b'+' | b'=')) {
        return None;
    }
    Some((range.start..plus, literal(mask, trim(mask, plus + 1..range.end))?))
}

fn base(mask: &[u8], range: Range<usize>) -> Option<(Range<usize>, bool)> {
    let mut range = trim(mask, range);
    let mut cast = false;
    loop {
        while !range.is_empty() && mask[range.start] == b'(' && closing(mask, range.start) == Some(range.end - 1) {
            range = trim(mask, range.start + 1..range.end - 1);
        }
        match type_name(mask, range.start) {
            Some((name, end)) if !cast && BYTE_CASTS.contains(&name.as_str()) && end < range.end => {
                cast = true;
                range = trim(mask, end..range.end);
            }
            Some(_) => return None,
            None => break,
        }
    }
    if range.is_empty() {
        return None;
    }
    if mask[range.start] == b'*' {
        let (name, end) = type_name(mask, next(mask, range.start + 1)?)?;
        let ampersand = next(mask, end)?;
        let core = trim(mask, ampersand + 1..range.end);
        let reinterpreted = ["s32 *", "u32 *", "u8 **", "void **"].contains(&name.as_str());
        return (reinterpreted && mask[ampersand] == b'&' && identifier(mask, &core)).then_some((core, true));
    }
    if identifier(mask, &range) {
        return Some((range, cast));
    }
    let open = range.start + mask[range.clone()].iter().position(|&b| b == b'(')?;
    let name = trim(mask, range.start..open);
    (identifier(mask, &name) && closing(mask, open) == Some(range.end - 1)).then_some((range, cast))
}

fn member_name(mask: &[u8], at: usize) -> bool {
    previous(mask, at).is_some_and(|p| mask[p] == b'.' || (mask[p] == b'>' && p > 0 && mask[p - 1] == b'-'))
}

fn accesses(mask: &[u8], macros: &[Macro]) -> Vec<Access> {
    let mut found = Vec::new();
    for at in 0..mask.len() {
        found.extend(match mask[at] {
            b'(' => cast_access(mask, at),
            b'[' => index_access(mask, at),
            b'*' => byte_deref(mask, at),
            b'=' => bare(mask, at),
            _ => None,
        });
    }
    for m in macros {
        for at in words(mask, &m.name, 0..mask.len()) {
            found.extend(macro_access(mask, at, m));
        }
    }
    found
}

fn cast_access(mask: &[u8], at: usize) -> Option<Access> {
    let (name, end) = type_name(mask, at)?;
    let ty = int_pointer(&name)?;
    let open = next(mask, end)?;
    if mask[open] != b'(' {
        return None;
    }
    let close = closing(mask, open)?;
    if postfix(mask, close + 1) {
        return None;
    }
    let (base_range, offset) = sum(mask, open + 1..close)?;
    let (core, cast) = base(mask, base_range)?;
    Some(match previous(mask, at).filter(|&p| mask[p] == b'*') {
        Some(star) => Access {
            span: star..close + 1,
            core,
            cast,
            offset,
            form: Form::Value { ty, store: store(mask, close + 1) },
        },
        None => Access { span: at..close + 1, core, cast, offset, form: Form::Address { cast: name, ty } },
    })
}

fn index_access(mask: &[u8], at: usize) -> Option<Access> {
    let close = closing(mask, at)?;
    let index = literal(mask, trim(mask, at + 1..close))?;
    if postfix(mask, close + 1) {
        return None;
    }
    let store = store(mask, close + 1);
    let before = previous(mask, at)?;
    if mask[before] == b')' {
        let open = opening(mask, before)?;
        let (name, end) = type_name(mask, next(mask, open + 1)?)?;
        let ty = int_pointer(&name)?;
        let (core, _) = base(mask, end..before)?;
        let offset = index.checked_mul(ty.bytes)?;
        return Some(Access { span: open..close + 1, core, cast: true, offset, form: Form::Value { ty, store } });
    }
    let word = word_before(mask, at)?;
    if !identifier(mask, &word) || member_name(mask, word.start) {
        return None;
    }
    Some(Access { span: word.start..close + 1, core: word, cast: false, offset: index, form: Form::Byte { store } })
}

fn byte_deref(mask: &[u8], at: usize) -> Option<Access> {
    if let Some(p) = previous(mask, at) {
        if is_ident(mask[p]) || mask[p] == b')' || mask[p] == b']' {
            let unary = word_before(mask, at).is_some_and(|w| matches!(text(mask, w), "return" | "case" | "else" | "do"));
            if !unary {
                return None;
            }
        }
    }
    let open = next(mask, at + 1)?;
    if mask[open] != b'(' {
        return None;
    }
    let close = closing(mask, open)?;
    if postfix(mask, close + 1) {
        return None;
    }
    let (base_range, offset) = sum(mask, open + 1..close)?;
    let core = trim(mask, base_range);
    identifier(mask, &core).then(|| Access {
        span: at..close + 1,
        core,
        cast: false,
        offset,
        form: Form::Byte { store: store(mask, close + 1) },
    })
}

fn bare(mask: &[u8], at: usize) -> Option<Access> {
    if mask.get(at + 1) == Some(&b'=') || (at > 0 && b"=!<>+-*/%&|^".contains(&mask[at - 1])) {
        return None;
    }
    let word = word_at(mask, next(mask, at + 1)?);
    if !identifier(mask, &word) {
        return None;
    }
    let plus = next(mask, word.end)?;
    if mask[plus] != b'+' || matches!(mask.get(plus + 1), Some(b'+' | b'=')) {
        return None;
    }
    let number = word_at(mask, next(mask, plus + 1)?);
    let offset = literal(mask, number.clone())?;
    (mask[next(mask, number.end)?] == b';').then_some(Access {
        span: word.start..number.end,
        core: word,
        cast: false,
        offset,
        form: Form::Bare,
    })
}

fn macro_access(mask: &[u8], at: usize, m: &Macro) -> Option<Access> {
    let open = next(mask, at + m.name.len())?;
    if mask[open] != b'(' {
        return None;
    }
    let close = closing(mask, open)?;
    let args = split_commas(mask, open + 1..close);
    if args.len() != 3 || postfix(mask, close + 1) {
        return None;
    }
    let offset = literal(mask, trim(mask, args[m.offset].clone()))?;
    let ty = normal_type(text(mask, trim(mask, args[m.ty].clone())))?;
    let ty = if m.pointer { int_pointer(&ty)? } else { Int::parse(&ty)? };
    let (core, _) = base(mask, args[m.base].clone())?;
    Some(Access {
        span: at..close + 1,
        core,
        cast: true,
        offset,
        form: Form::Value { ty, store: store(mask, close + 1) },
    })
}

struct Edit {
    span: Range<usize>,
    text: String,
}

struct Group {
    edits: Vec<Edit>,
    count: usize,
}

fn member_edit<'a>(access: &Access, base: &str, unit: &'a Unit) -> Option<(Edit, &'a Member)> {
    let (text, member) = match &access.form {
        Form::Value { ty, store } => {
            let m = unit.value(access.offset, *ty, *store)?;
            (format!("{base}->{}", m.path), m)
        }
        Form::Byte { store } => {
            let m = unit.value(access.offset, Int::U8, *store)?;
            (format!("{base}->{}", m.path), m)
        }
        Form::Bare => {
            let m = unit.value(access.offset, Int::U8, false)?;
            (format!("&{base}->{}", m.path), m)
        }
        Form::Address { cast, ty } => {
            let m = unit.address(access.offset, *ty)?;
            if m.kind == Kind::Int(*ty) {
                (format!("&{base}->{}", m.path), m)
            } else {
                (format!("({cast})&{base}->{}", m.path), m)
            }
        }
    };
    Some((Edit { span: access.span.clone(), text }, member))
}

struct Declaration {
    ty: String,
    name: Range<usize>,
    span: Range<usize>,
    init: Option<Range<usize>>,
    parameter: bool,
}

fn typed_name(mask: &[u8], range: Range<usize>) -> Option<(String, Range<usize>)> {
    let range = trim(mask, range);
    let name = word_before(mask, range.end)?;
    if name.end != range.end || !identifier(mask, &name) {
        return None;
    }
    let ty = normal_type(text(mask, range.start..name.start))?;
    let structure = ty.strip_prefix("struct ").and_then(|t| t.strip_suffix(" *")).is_some_and(|t| t.bytes().all(is_ident));
    (LOCAL_TYPES.contains(&ty.as_str()) || structure).then_some((ty, name))
}

fn declarations(mask: &[u8], function: &Function) -> Vec<Declaration> {
    let mut found = Vec::new();
    for part in split_commas(mask, function.params.clone()) {
        let span = trim(mask, part);
        if let Some((ty, name)) = typed_name(mask, span.clone()) {
            found.push(Declaration { ty, name, span, init: None, parameter: true });
        }
    }
    let (mut depth, mut start) = (0i32, function.body.start + 1);
    for i in function.body.clone() {
        match mask[i] {
            b'{' | b'(' | b'[' => {
                depth += 1;
                if depth == 1 {
                    start = i + 1;
                }
            }
            b'}' | b')' | b']' => {
                depth -= 1;
                if depth == 1 && mask[i] == b'}' {
                    start = i + 1;
                }
            }
            b';' if depth == 1 => {
                let statement = trim(mask, start..i);
                start = i + 1;
                let equals = statement.clone().find(|&j| mask[j] == b'=');
                let (left, init) = match equals {
                    Some(e) if mask.get(e + 1) != Some(&b'=') => (statement.start..e, Some(trim(mask, e + 1..statement.end))),
                    Some(_) => continue,
                    None => (statement.clone(), None),
                };
                if let Some((ty, name)) = typed_name(mask, left) {
                    let span = statement.start..init.as_ref().map_or(name.end, |r| r.end);
                    found.push(Declaration { ty, name, span, init, parameter: false });
                }
            }
            _ => {}
        }
    }
    found
}

fn declarations_of(mask: &[u8], function: &Function, name: &str) -> usize {
    let typed = |at: usize| {
        let Some(mut p) = previous(mask, at) else {
            return false;
        };
        while mask[p] == b'*' {
            match previous(mask, p) {
                Some(q) => p = q,
                None => return false,
            }
        }
        is_ident(mask[p])
            && word_before(mask, p + 1).is_some_and(|w| {
                !matches!(text(mask, w), "return" | "else" | "case" | "goto" | "sizeof" | "do")
            })
    };
    words(mask, name, function.params.start..function.body.end)
        .into_iter()
        .filter(|&at| !member_name(mask, at) && typed(at))
        .count()
}

fn assignment(mask: &[u8], at: usize, length: usize) -> Option<Range<usize>> {
    let equals = next(mask, at + length)?;
    if mask[equals] != b'=' || mask.get(equals + 1) == Some(&b'=') {
        return None;
    }
    let statement = match previous(mask, at) {
        None => true,
        Some(p) => matches!(mask[p], b';' | b'{' | b'}' | b')') || word_before(mask, at).is_some_and(|w| text(mask, w) == "else"),
    };
    if !statement {
        return None;
    }
    let mut depth = 0i32;
    for i in equals + 1..mask.len() {
        match mask[i] {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth -= 1,
            b';' if depth == 0 => return Some(trim(mask, equals + 1..i)),
            _ => {}
        }
    }
    None
}

fn plain(mask: &[u8], at: usize, length: usize) -> bool {
    let (before, after) = (previous(mask, at), next(mask, at + length));
    if let Some(a) = after {
        if mask[a..].starts_with(b"!=") || mask[a..].starts_with(b"==") {
            if let Some(z) = next(mask, a + 2) {
                if matches!(text(mask, word_at(mask, z)), "0" | "NULL") {
                    return true;
                }
            }
        }
    }
    let (Some(b), Some(a)) = (before, after) else {
        return false;
    };
    if mask[b] == b'!' {
        return true;
    }
    match (mask[b], mask[a]) {
        (b'(', b')' | b',') => previous(mask, b).is_some_and(|p| is_ident(mask[p]) || mask[p] == b')'),
        (b',', b')' | b',') => true,
        _ => false,
    }
}

fn is_core(path: &str) -> bool {
    let first = path.split('.').next().unwrap_or("");
    matches!(first, "x" | "y" | "z") || first.starts_with("velocity_") || first.starts_with("target_")
}

struct Context<'a> {
    text: &'a str,
    mask: &'a [u8],
    all: &'a [Access],
    spec: &'a Spec,
    unit: &'a Unit,
}

impl Context<'_> {
    fn access(&self, at: usize, length: usize) -> Option<&Access> {
        self.all.iter().find(|a| a.core == (at..at + length))
    }

    fn uses(&self, name: &str, range: Range<usize>) -> Vec<usize> {
        words(self.mask, name, range).into_iter().filter(|&at| !member_name(self.mask, at)).collect()
    }

    fn source(&self, range: Range<usize>) -> Option<String> {
        let (core, _) = base(self.mask, range)?;
        let written = &self.text[core.clone()];
        let open = written.find('(');
        let name = written[..open.unwrap_or(written.len())].trim();
        let ours = match open {
            Some(_) => self.spec.getters.contains(&name),
            None => self.spec.globals.contains(&name),
        };
        (ours && self.unit.may_hold(name, self.spec.tag)).then(|| written.to_string())
    }

    fn inline(&self, groups: &mut Vec<Group>) {
        for access in self.all.iter().filter(|a| a.cast) {
            let core = &self.text[access.core.clone()];
            let open = core.find('(');
            let name = core[..open.unwrap_or(core.len())].trim();
            let ours = match open {
                Some(_) => self.spec.getters.contains(&name),
                None => self.spec.globals.contains(&name),
            };
            if ours && self.unit.points_at(name, self.spec.tag) {
                if let Some((edit, _)) = member_edit(access, core, self.unit) {
                    groups.push(Group { edits: vec![edit], count: 1 });
                }
            }
        }
    }

    fn typed(&self, function: &Function, name: &str, groups: &mut Vec<Group>) {
        for at in self.uses(name, function.params.start..function.body.end) {
            if let Some(access) = self.access(at, name.len()).filter(|a| a.cast) {
                if let Some((edit, _)) = member_edit(access, name, self.unit) {
                    groups.push(Group { edits: vec![edit], count: 1 });
                }
            }
        }
    }

    fn local(&self, function: &Function, declaration: &Declaration, name: &str) -> Option<Group> {
        let mut edits = Vec::new();
        let (mut count, mut sources) = (0, 0);
        let initial = match &declaration.init {
            Some(init) => {
                sources += 1;
                Some(self.source(init.clone())?)
            }
            None => None,
        };
        for at in self.uses(name, function.body.clone()) {
            if at == declaration.name.start {
                continue;
            }
            if let Some(access) = self.access(at, name.len()) {
                if matches!(access.form, Form::Byte { .. }) && declaration.ty != "u8 *" {
                    return None;
                }
                edits.push(member_edit(access, name, self.unit)?.0);
                count += 1;
            } else if let Some(expression) = assignment(self.mask, at, name.len()) {
                let written = self.source(expression.clone())?;
                edits.push(Edit { span: expression, text: written });
                sources += 1;
            } else if !plain(self.mask, at, name.len()) {
                return None;
            }
        }
        if count == 0 || sources == 0 {
            return None;
        }
        let tag = self.spec.tag;
        let declared = match initial {
            Some(source) => format!("struct {tag} *{name} = {source}"),
            None => format!("struct {tag} *{name}"),
        };
        edits.push(Edit { span: declaration.span.clone(), text: declared });
        Some(Group { edits, count })
    }

    fn parameter(&self, function: &Function, declaration: &Declaration, name: &str, top: &dyn Fn(usize) -> bool) -> Option<Group> {
        let function_name = text(self.mask, function.name.clone());
        let elsewhere = words(self.mask, function_name, 0..self.mask.len()).into_iter().any(|at| at != function.name.start && top(at));
        if self.unit.declared.contains(function_name) || elsewhere {
            return None;
        }
        let (mut edits, mut core, mut count) = (Vec::new(), BTreeSet::new(), 0);
        for at in self.uses(name, function.body.clone()) {
            if let Some(access) = self.access(at, name.len()) {
                if matches!(access.form, Form::Byte { .. }) && declaration.ty != "u8 *" {
                    return None;
                }
                let (edit, member) = member_edit(access, name, self.unit)?;
                if is_core(&member.path) {
                    core.insert(member.path.clone());
                }
                edits.push(edit);
                count += 1;
            } else if !plain(self.mask, at, name.len()) {
                return None;
            }
        }
        if count < 2 || core.len() < 2 {
            return None;
        }
        edits.push(Edit { span: declaration.span.clone(), text: format!("struct {} *{name}", self.spec.tag) });
        Some(Group { edits, count })
    }
}

fn groups(text: &str, spec: &Spec, unit: &Unit) -> Vec<Group> {
    let mask = mask(text);
    let all = accesses(&mask, &unit.macros);
    let context = Context { text, mask: &mask, all: &all, spec, unit };
    let mut groups = Vec::new();
    context.inline(&mut groups);
    let found = functions(&mask);
    let top = |at: usize| !found.iter().any(|f| f.body.contains(&at));
    let pointer = format!("struct {} *", spec.tag);
    for function in &found {
        for declaration in declarations(&mask, function) {
            let name = super::source::text(&mask, declaration.name.clone()).to_string();
            if declarations_of(&mask, function, &name) != 1 {
                continue;
            }
            if declaration.ty == pointer {
                context.typed(function, &name, &mut groups);
            } else if !LOCAL_TYPES.contains(&declaration.ty.as_str()) {
                continue;
            } else if !declaration.parameter {
                groups.extend(context.local(function, &declaration, &name));
            } else if spec.parameters {
                groups.extend(context.parameter(function, &declaration, &name, &top));
            }
        }
    }
    groups
}

fn select(groups: Vec<Group>) -> Vec<Group> {
    let mut alive = vec![true; groups.len()];
    let mut edits: Vec<(Range<usize>, usize)> = groups
        .iter()
        .enumerate()
        .flat_map(|(g, group)| group.edits.iter().map(move |e| (e.span.clone(), g)))
        .collect();
    edits.sort_by_key(|(span, _)| (span.start, span.end));
    for i in 0..edits.len() {
        for j in i + 1..edits.len() {
            if edits[j].0.start >= edits[i].0.end {
                break;
            }
            let (a, b) = (edits[i].1, edits[j].1);
            if a != b && alive[a] && alive[b] {
                alive[if edits[i].0.len() >= edits[j].0.len() { a } else { b }] = false;
            }
        }
    }
    groups.into_iter().zip(alive).filter_map(|(group, keep)| keep.then_some(group)).collect()
}

fn apply(text: &str, mut edits: Vec<Edit>) -> String {
    edits.sort_by_key(|e| e.span.start);
    let mut out = String::with_capacity(text.len());
    let mut at = 0;
    for edit in edits {
        out.push_str(&text[at..edit.span.start]);
        out.push_str(&edit.text);
        at = edit.span.end;
    }
    out.push_str(&text[at..]);
    out
}

pub(crate) fn rewrite(text: &str, spec: &Spec, unit: &Unit) -> (String, usize) {
    let mut text = text.to_string();
    let mut count = 0;
    for _ in 0..8 {
        let chosen = select(groups(&text, spec, unit));
        if chosen.is_empty() {
            break;
        }
        count += chosen.iter().map(|g| g.count).sum::<usize>();
        text = apply(&text, chosen.into_iter().flat_map(|g| g.edits).collect());
    }
    (text, count)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn int(name: &str) -> Kind {
        Kind::Int(Int::parse(name).unwrap())
    }

    fn member(path: &str, offset: u32, kind: Kind) -> Member {
        Member { path: path.into(), offset, kind }
    }

    fn actor_unit(getter: Kind) -> Unit {
        let layout = vec![
            member("facing", 6, int("u16")),
            member("x.fixed", 8, int("s32")),
            member("x.part.fraction", 8, int("u16")),
            member("x.part.pixel", 10, int("s16")),
            member("z.fixed", 16, int("s32")),
            member("velocity_x", 36, int("s32")),
            member("target_x", 56, int("s32")),
            member("sprite", 80, Kind::Pointer(Pointee::Record("FieldSprite".into()))),
            member("motion_flags", 85, int("u8")),
        ];
        let mut types = HashMap::new();
        types.insert("Object_GetById".to_string(), getter);
        Unit { layouts: vec![layout], types, declared: HashSet::new(), macros: Vec::new() }
    }

    fn actors(text: &str) -> (String, usize) {
        rewrite(text, &ACTORS, &actor_unit(Kind::Pointer(Pointee::Record("FieldActor".into()))))
    }

    #[test]
    fn inline_getters_read_and_store_members() {
        let (out, count) = actors(
            "void F(void)\n{\n    *(s16 *)((u8 *)Object_GetById(3) + 10) = 0;\n    *(u16 *)((s32)Object_GetById(3) + 10) = 1;\n    G(((u8 *)Object_GetById(2))[0x55]);\n    G(*(u16 *)((u8 *)Object_GetById(3) + 10));\n}\n",
        );
        assert_eq!(count, 3);
        assert!(out.contains("    Object_GetById(3)->x.part.pixel = 0;\n"));
        assert!(out.contains("    Object_GetById(3)->x.part.pixel = 1;\n"));
        assert!(out.contains("G(Object_GetById(2)->motion_flags);"));
        assert!(out.contains("G(*(u16 *)((u8 *)Object_GetById(3) + 10));"));
    }

    #[test]
    fn a_getter_typed_otherwise_in_this_file_keeps_its_offsets() {
        let text = "void F(void)\n{\n    G(*(s16 *)((u8 *)Object_GetById(3) + 10));\n}\n";
        let (out, count) = rewrite(text, &ACTORS, &actor_unit(Kind::Pointer(Pointee::Int(1))));
        assert_eq!((out.as_str(), count), (text, 0));
    }

    #[test]
    fn a_dereference_never_swallows_the_parenthesis_that_closes_a_call() {
        let (out, count) = actors(
            "void F(void)\n{\n    s32 a = Object_GetById(1);\n    G(*(s32 *)(a) + 8);\n    G(*(s32 *)(a + 8));\n    G((*(s32 *)((a + 16))) + 8);\n}\n",
        );
        assert_eq!(count, 2, "{out}");
        assert!(out.contains("    G(*(s32 *)(a) + 8);\n    G(a->x.fixed);\n    G((a->z.fixed) + 8);\n"));
        let text = "void F(void)\n{\n    G(*(s32 *)((u8 *)Object_GetById(1)) + 8);\n}\n";
        assert_eq!(actors(text), (text.to_string(), 0));
    }

    #[test]
    fn locals_are_retyped_when_every_use_is_named() {
        let (out, count) = actors(
            "void F(s32 id)\n{\n    s32 a;\n    u8 *b = (u8 *)Object_GetById(2);\n\n    a = (s32)Object_GetById(id);\n    if (a != 0) {\n        *(s32 *)(a + 0x24) = *(s32 *)((u8 *)a + 8);\n        G(a, (s32 *)(a + 16), b[85]);\n        b[85] = 0;\n    }\n    work->a = 3;\n}\n",
        );
        assert_eq!(count, 5);
        assert!(out.contains("    struct FieldActor *a;\n    struct FieldActor *b = Object_GetById(2);\n"));
        assert!(out.contains("    a = Object_GetById(id);\n"));
        assert!(out.contains("a->velocity_x = a->x.fixed;"));
        assert!(out.contains("G(a, &a->z.fixed, b->motion_flags);"));
        assert!(out.contains("b->motion_flags = 0;"));
        assert!(out.contains("    work->a = 3;\n"));
    }

    #[test]
    fn a_local_with_another_use_keeps_its_type_and_offsets() {
        let text = "void F(void)\n{\n    s32 a = Object_GetById(1);\n    G(*(s32 *)(a + 8));\n    H(a + 4);\n}\n";
        let (out, count) = actors(text);
        assert_eq!((out.as_str(), count), (text, 0));
    }

    #[test]
    fn parameters_read_as_actors_are_retyped() {
        let (out, count) = actors(
            "static void Move(s32 actor, s32 speed)\n{\n    *(s32 *)(actor + 36) = speed;\n    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);\n}\n",
        );
        assert_eq!(count, 3);
        assert!(out.starts_with("static void Move(struct FieldActor *actor, s32 speed)\n"));
        assert!(out.contains("actor->target_x = actor->x.fixed;"));
        let declared = "void Move(s32 actor);\nvoid Move(s32 actor)\n{\n    *(s32 *)(actor + 36) = *(s32 *)(actor + 8);\n}\n";
        assert_eq!(actors(declared).1, 0);
    }

    #[test]
    fn typed_pointers_and_offset_macros_name_members() {
        let mut unit = actor_unit(Kind::Other);
        unit.macros = macros("#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))\n");
        let (out, count) = rewrite(
            "void F(struct FieldActor *actor)\n{\n    FIELD_AT_OFFSET(actor, s32 *, 0x38) = *(s32 *)((u8 *)actor + 8);\n    G((s32 *)((u8 *)actor + 8));\n}\n",
            &ACTORS,
            &unit,
        );
        assert_eq!(count, 3);
        assert!(out.contains("actor->target_x = actor->x.fixed;"));
        assert!(out.contains("G(&actor->x.fixed);"));
    }

    #[test]
    fn events_follow_the_global_and_its_reinterpretations() {
        let layout = vec![member("touched_trigger", 0x16c, int("s16")), member("message", 0x1d8, int("u16"))];
        let mut types = HashMap::new();
        types.insert("gEventWork".to_string(), Kind::Pointer(Pointee::Record("EventWork".into())));
        let unit = Unit { layouts: vec![layout], types, declared: HashSet::new(), macros: Vec::new() };
        let (out, count) = rewrite(
            "void F(void)\n{\n    u8 *w = *(u8 **)&gEventWork;\n    *(s16 *)(w + 0x16c) = -1;\n    G(*(u16 *)((*(s32 *)&gEventWork + 0x1d8)));\n}\n",
            &EVENTS,
            &unit,
        );
        assert_eq!(count, 2);
        assert!(out.contains("    struct EventWork *w = gEventWork;\n    w->touched_trigger = -1;\n"));
        assert!(out.contains("G(gEventWork->message);"));
    }

    #[test]
    fn both_games_must_agree_on_a_member() {
        let mut unit = actor_unit(Kind::Pointer(Pointee::Record("FieldActor".into())));
        unit.layouts.push(vec![member("x", 8, int("s32"))]);
        let text = "void F(void)\n{\n    G(*(s32 *)((u8 *)Object_GetById(1) + 8));\n}\n";
        assert_eq!(rewrite(text, &ACTORS, &unit).1, 0);
    }
}
