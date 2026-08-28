use candidate_show::allocator::Repair;
use compiler_core::CALL_VIA_BASE;
use regex::{Captures, Regex};
use std::{collections::HashSet, ops::Range};

pub const CATALOG_VERSION: &str = "register-wall-v2";
const MAX_CHOICES: usize = 16;
type Mutation = (String, String);
type Mutations = Result<Vec<Mutation>, String>;

#[derive(Clone, Debug, Eq, PartialEq)]
struct Variant {
    source: String,
    mutations: Vec<String>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Permutation {
    variants: Vec<Variant>,
    raw_count: usize,
    dimensions: Vec<&'static str>,
}

impl Permutation {
    pub fn count(&self) -> usize {
        self.variants.len()
    }
    pub fn raw_count(&self) -> usize {
        self.raw_count
    }
    pub fn dimensions(&self) -> &[&'static str] {
        &self.dimensions
    }
    pub fn mutations(&self, index: usize) -> Option<&[String]> {
        Some(&self.variants.get(index)?.mutations)
    }
    pub fn evaluate(&self, index: usize) -> Result<String, String> {
        self.variants
            .get(index)
            .map(|variant| variant.source.clone())
            .ok_or_else(|| format!("permutation index {index} exceeds count {}", self.count()))
    }
}

fn regex(pattern: &str) -> Regex {
    Regex::new(pattern).unwrap()
}

fn masked(source: &str) -> String {
    let pattern =
        regex(r#"(?s:/\*.*?\*/)|(?m://[^\n]*|^[ \t]*#[^\n]*)|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"#);
    let mut output = source.to_string();
    for found in pattern
        .find_iter(source)
        .collect::<Vec<_>>()
        .into_iter()
        .rev()
    {
        let blank = source[found.range()]
            .bytes()
            .map(|byte| if byte == b'\n' { '\n' } else { ' ' })
            .collect::<String>();
        output.replace_range(found.range(), &blank);
    }
    output
}

fn only<'a>(pattern: &Regex, text: &'a str, reason: &str) -> Result<Captures<'a>, String> {
    let mut found = pattern.captures_iter(text);
    let first = found.next().ok_or_else(|| reason.to_string())?;
    if found.next().is_some() {
        return Err(reason.into());
    }
    Ok(first)
}

fn capture<'a>(source: &'a str, found: &Captures, index: usize) -> &'a str {
    &source[found.get(index).unwrap().range()]
}

fn edited(source: &str, mut changes: Vec<(Range<usize>, String)>) -> Result<String, String> {
    changes.sort_by_key(|change| change.0.start);
    if changes
        .windows(2)
        .any(|pair| pair[0].0.end > pair[1].0.start)
    {
        return Err("repair edits overlap".into());
    }
    let mut output = source.to_string();
    for (range, text) in changes.into_iter().rev() {
        output.replace_range(range, &text);
    }
    Ok(output)
}

fn variants(source: &str, range: Range<usize>, forms: Vec<(&str, String)>) -> Mutations {
    forms
        .into_iter()
        .map(|(id, text)| {
            Ok((
                edited(source, vec![(range.clone(), text)])?,
                format!("{id}@{}", range.start),
            ))
        })
        .collect()
}

fn words(name: &str) -> Regex {
    regex(&format!(r"\b{}\b", regex::escape(name)))
}

fn variable_uses(code: &str, name: &str) -> usize {
    words(name)
        .find_iter(code)
        .filter(|found| {
            !matches!(
                code.as_bytes().get(found.start().wrapping_sub(1)),
                Some(b'.' | b'>')
            )
        })
        .count()
}

fn fresh(source: &str, stem: &str) -> String {
    let mut name = stem.to_string();
    while words(&name).is_match(source) {
        name.push('_');
    }
    name
}

fn pure(text: &str) -> bool {
    !["++", "--", "=", ";", ",", "\"", "'"]
        .iter()
        .any(|token| text.contains(token))
        && !regex(r"\b[A-Za-z_][A-Za-z0-9_]*\s*\(").is_match(text)
}

fn scope(code: &str, end: usize) -> Option<usize> {
    let mut stack = Vec::new();
    for (at, byte) in code.as_bytes()[..end].iter().enumerate() {
        match byte {
            b'{' => stack.push(at),
            b'}' => {
                stack.pop();
            }
            _ => {}
        }
    }
    stack.last().copied()
}

fn declaration<'a>(
    source: &'a str,
    code: &str,
    name: &str,
) -> Result<(Range<usize>, &'a str), String> {
    let pattern = regex(&format!(
        r"(?m)^[ \t]+(?:struct[ \t]+[A-Za-z_][A-Za-z0-9_]*|(?:u|s)(?:8|16|32)|void)[ \t*]+{}[ \t]*;[ \t]*(?:\n|$)",
        regex::escape(name)
    ));
    let found = only(
        &pattern,
        code,
        &format!("requires one uninitialized declaration of {name}"),
    )?;
    let range = found.get(0).unwrap().range();
    Ok((range.clone(), &source[range]))
}

fn swap(source: &str, code: &str, left: &str, right: &str) -> Mutations {
    let (a, a_line) = declaration(source, code, left)?;
    let (b, b_line) = declaration(source, code, right)?;
    if scope(code, a.start) != scope(code, b.start) {
        return Err("named declarations are not in the same lexical block".into());
    }
    let at = a.start.min(b.start);
    Ok(vec![(
        edited(source, vec![(a, b_line.into()), (b, a_line.into())])?,
        format!("declaration_order:swap_named({left},{right})@{at}"),
    )])
}

fn split(source: &str, code: &str, name: &str) -> Mutations {
    let (declaration, line) = declaration(source, code, name)?;
    if source.contains("volatile") || !line.contains('*') {
        return Err(format!(
            "split-lifetime repair requires nonvolatile pointer {name}"
        ));
    }
    let assignment = regex(&format!(
        r"(?m)^[ \t]*{}\s*=\s*([^;\n]+);\n?",
        regex::escape(name)
    ));
    let assigned = only(
        &assignment,
        code,
        &format!("requires one assignment of {name}"),
    )?;
    let expression = capture(source, &assigned, 1).trim();
    let dereference = regex(&format!(r"\*\s*\b{}\b", regex::escape(name)));
    let tail = assigned.get(0).unwrap().end();
    let uses = dereference
        .find_iter(&code[tail..])
        .map(|found| tail + found.start()..tail + found.end())
        .collect::<Vec<_>>();
    if variable_uses(&code[declaration.start..], name) != 4 || uses.len() != 2 || !pure(expression)
    {
        return Err(format!(
            "split-lifetime repair found ambiguous uses of {name}"
        ));
    }
    let mut changes = vec![
        (declaration.clone(), String::new()),
        (assigned.get(0).unwrap().range(), String::new()),
    ];
    changes.extend(
        uses.into_iter()
            .map(|range| (range, format!("*({expression})"))),
    );
    Ok(vec![(
        edited(source, changes)?,
        format!(
            "block_lifetime+loop_spelling:inline_named({name})@{}",
            declaration.start
        ),
    )])
}

fn zero(source: &str, code: &str) -> Mutations {
    let pattern = regex(
        r"(?m)^([ \t]+)([^;\n=]+?)\s*=\s*0;\n([ \t]+)([^;\n=]+?)\s*=\s*0;\n(?:[ \t]*\n)?([ \t]+)([^;\n=]+?)\s*&=\s*~0x20;",
    );
    let found = only(&pattern, code, "zero-carrier repair requires one site")?;
    if found[1] != found[3]
        || found[1] != found[5]
        || ![&found[2], &found[4], &found[6]]
            .iter()
            .all(|value| pure(value) && !value.contains('*'))
    {
        return Err("zero-carrier repair requires integral nonvolatile lvalues".into());
    }
    let name = fresh(source, "perm_zero");
    let text = format!(
        "{}s32 {name} = 0;\n{}{} = {name};\n{}{} = {name};\n\n{}{} &= {name} - 33;",
        &found[1], &found[1], &found[2], &found[3], &found[4], &found[5], &found[6]
    );
    variants(
        source,
        found.get(0).unwrap().range(),
        vec![("temporary:merge_zero_carrier", text)],
    )
}

fn inline_xor(source: &str, code: &str, found: &Captures) -> Option<Mutation> {
    let site = found.get(0)?.range();
    let tail = site.end;
    let next =
        regex(r"\A(?:[ \t]*\n)+([ \t]+)([^;\n]+?)\s*\^=\s*([^;\n]+);").captures(&code[tail..])?;
    let part = |index| {
        let range = next.get(index).unwrap().range();
        &source[tail + range.start..tail + range.end]
    };
    let (right_indent, right, right_value) = (part(1), part(2).trim(), part(3).trim());
    let end = tail + next.get(0)?.end();
    let open = scope(code, site.start)?;
    let close = end + code[end..].find('}')?;
    let line = code[..open].rfind('\n').map_or(0, |at| at + 1);
    let indent = &source[line..open];
    let left = capture(source, found, 6).trim();
    if right_indent != &found[1]
        || left == right
        || !pure(right)
        || !pure(right_value)
        || !code[open + 1..site.start].trim().is_empty()
        || !code[end..close].trim().is_empty()
        || !indent.trim().is_empty()
    {
        return None;
    }
    let insert = regex(r"(?m)^extern\b").find(code)?.start();
    if insert >= open || scope(code, insert).is_some() {
        return None;
    }
    let kind = &found[2];
    let expression = capture(source, found, 3);
    let helper = fresh(source, "perm_xor");
    let definition = format!(
        "static __inline__ {kind} {helper}({kind} tile_word, {kind} mask_word)\n{{\n    return tile_word ^ mask_word;\n}}\n\n"
    );
    let body = format!(
        "{indent}{left} = {helper}({left}, {expression});\n{indent}{right} = {helper}({right}, {right_value});"
    );
    let changes = vec![(insert..insert, definition), (open..close + 1, body)];
    let text = edited(source, changes).ok()?;
    Some((
        text,
        format!(
            "temporary+evaluation_order:inline_xor_helper@{}",
            site.start
        ),
    ))
}

fn reciprocal(source: &str, code: &str, name: &str) -> Mutations {
    if source.contains("volatile") {
        return Err("reciprocal repair refuses volatile source".into());
    }
    let pattern = regex(&format!(
        r"(?m)^([ \t]+)((?:u|s)(?:8|16|32))\s+{}\s*=\s*([^;\n]+);\n([ \t]*\n)?([ \t]+)([^;\n]+?)\s*\^=\s*{};",
        regex::escape(name),
        regex::escape(name)
    ));
    let found = only(
        &pattern,
        code,
        &format!("requires one reciprocal site for {name}"),
    )?;
    if variable_uses(&code[found.get(0).unwrap().start()..], name) != 2
        || found[1] != found[5]
        || !pure(&found[3])
        || !pure(&found[6])
    {
        return Err(format!("reciprocal repair found ambiguous uses of {name}"));
    }
    let (indent, kind, expression, gap, left) = (
        &found[1],
        &found[2],
        capture(source, &found, 3),
        capture(source, &found, 4),
        &found[6],
    );
    let declaration = format!("{indent}{kind} {name} = {expression};\n{gap}");
    let temporary = fresh(
        source,
        &format!("perm_value_{}", found.get(0).unwrap().start()),
    );
    let mut generated = variants(
        source,
        found.get(0).unwrap().range(),
        vec![
            ("temporary:merge", format!("{indent}{left} ^= {expression};")),
            ("temporary:introduce", format!("{declaration}{indent}{kind} {temporary} = {left};\n{indent}{temporary} ^= {name};\n{indent}{left} = {temporary};")),
            ("evaluation_order:left_before_right", format!("{indent}{kind} {temporary} = {left};\n{declaration}{indent}{temporary} ^= {name};\n{indent}{left} = {temporary};")),
            ("commutative_order:swap_operands", format!("{declaration}{indent}{left} = {name} ^ {left};")),
        ],
    )?;
    generated.extend(inline_xor(source, code, &found));
    Ok(generated)
}

pub fn parse(source: &str, repair: &Repair) -> Result<Permutation, String> {
    if source.contains("PERM_GENERAL(") || source.contains("PERM_INT(") {
        return Err("source annotations are retired; pass ordinary C".into());
    }
    for register in 0..14 {
        let symbol = format!("Func_{:08x}(", CALL_VIA_BASE + register * 4);
        if source.contains(&symbol) {
            return Err(format!("semantic guard: {symbol} is a main-image call-via trampoline; model the typed indirect call first"));
        }
    }
    if source.lines().any(|line| {
        matches!(
            line.trim_start().split_whitespace().next(),
            Some("#if" | "#ifdef" | "#ifndef" | "#else" | "#elif" | "#endif")
        )
    }) {
        return Err("repair catalog refuses conditional source".into());
    }
    let code = masked(source);
    let generated = match repair {
        Repair::SwapDeclarations { left, right } => swap(source, &code, left, right)?,
        Repair::SplitLifetime { name } => split(source, &code, name)?,
        Repair::MergeZeroCarrier => zero(source, &code)?,
        Repair::ReciprocalRoleSwap { name } => reciprocal(source, &code, name)?,
    };
    let raw_count = 1 + generated.len();
    if raw_count > MAX_CHOICES {
        return Err(format!(
            "finite mutation space {raw_count} exceeds cap {MAX_CHOICES}"
        ));
    }
    let mut variants = vec![Variant {
        source: source.into(),
        mutations: Vec::new(),
    }];
    let mut seen = HashSet::from([source.to_string()]);
    for (source, mutation) in generated {
        if seen.insert(source.clone()) {
            variants.push(Variant {
                source,
                mutations: vec![mutation],
            });
        }
    }
    Ok(Permutation {
        variants,
        raw_count,
        dimensions: repair.dimensions().to_vec(),
    })
}

pub fn self_test() -> Result<(), String> {
    let repair = Repair::SwapDeclarations {
        left: "a".into(),
        right: "c".into(),
    };
    let source = "void f(void)\n{\n    u32 a;\n    u32 b;\n    u32 c;\n}\n";
    let permutation = parse(source, &repair)?;
    if permutation.count() != 2
        || !permutation
            .evaluate(1)?
            .contains("u32 c;\n    u32 b;\n    u32 a;")
    {
        return Err("named non-adjacent declaration repair regressed".into());
    }
    if parse("void f(void) { PERM_GENERAL(a,b); }", &repair).is_ok()
        || parse("void f(void) { Func_080072e4(); }", &repair).is_ok()
    {
        return Err("annotation or trampoline guard regressed".into());
    }
    let repair = Repair::ReciprocalRoleSwap {
        name: "left".into(),
    };
    let source = "extern u32 m[];\n\nvoid g(void)\n{\n    {\n        u32 left = m[i].x;\n\n        a[i] ^= left;\n        a[j] ^= m[i].y;\n    }\n}\n";
    let permutation = parse(source, &repair)?;
    let helper = permutation.evaluate(5)?;
    let calls = helper.matches("perm_xor(").count();
    if permutation.count() != 6 || calls != 3 || helper.contains("u32 left =") {
        return Err("inline XOR boundary repair regressed".into());
    }
    let crowded = source.replace("a[j] ^= m[i].y;", "a[j] ^= m[i].y;\n        b ^= c;");
    if parse(&crowded, &repair)?.count() != 5 {
        return Err("inline XOR boundary singleton guard regressed".into());
    }
    Ok(())
}
