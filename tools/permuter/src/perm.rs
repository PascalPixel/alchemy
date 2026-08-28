use candidate_show::allocator::{Repair, RepairPlan};
use compiler_core::CALL_VIA_BASE;
use regex::{Captures, Regex};
use std::{collections::HashSet, ops::Range};

pub const CATALOG_VERSION: &str = "register-wall-v3";
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

#[derive(Debug)]
struct InitializedDeclaration<'a> {
    range: Range<usize>,
    indent: &'a str,
    kind: &'a str,
    expression: &'a str,
}

fn initialized_declaration<'a>(
    source: &'a str,
    code: &str,
    name: &str,
) -> Result<InitializedDeclaration<'a>, String> {
    let pattern = regex(&format!(
        r"(?m)^([ \t]+)((?:struct[ \t]+[A-Za-z_][A-Za-z0-9_]*|(?:u|s)(?:8|16|32)|void)[ \t]*\*?)[ \t]*{}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
        regex::escape(name)
    ));
    let found = only(
        &pattern,
        code,
        &format!("requires one initialized declaration of {name}"),
    )?;
    Ok(InitializedDeclaration {
        range: found.get(0).unwrap().range(),
        indent: capture(source, &found, 1),
        kind: capture(source, &found, 2).trim(),
        expression: capture(source, &found, 3).trim(),
    })
}

fn identifier(name: &str) -> bool {
    regex(r"^[A-Za-z_][A-Za-z0-9_]*$").is_match(name)
}

fn simple_lvalue(value: &str) -> bool {
    regex(r"^[A-Za-z_][A-Za-z0-9_]*(?:(?:->|\.)[A-Za-z_][A-Za-z0-9_]*)*$").is_match(value)
}

fn statement_after(code: &str, at: usize) -> Result<Range<usize>, String> {
    let start = at
        + code[at..]
            .find(|character: char| !character.is_ascii_whitespace())
            .ok_or("expected a following statement")?;
    let tail = &code[start..];
    let end = tail
        .find(';')
        .ok_or("following statement has no terminator")?;
    if tail[..end].contains('{') || tail[..end].contains('}') {
        return Err("following source shape crosses a block boundary".into());
    }
    Ok(start..start + end + 1)
}

fn scope_chain(code: &str, end: usize) -> Vec<usize> {
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
    stack
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

fn preload_adjacent_halfwords(
    source: &str,
    code: &str,
    first_destination: &str,
    first_source: &str,
    second_destination: &str,
    second_source: &str,
    carrier: &str,
) -> Mutations {
    if source.contains("volatile")
        || !identifier(carrier)
        || words(carrier).is_match(code)
        || ![
            first_destination,
            first_source,
            second_destination,
            second_source,
        ]
        .iter()
        .all(|value| simple_lvalue(value))
    {
        return Err(
            "adjacent-halfword repair requires four named nonvolatile lvalues and a fresh carrier"
                .into(),
        );
    }
    let assignment = |left: &str, right: &str| {
        let pattern = regex(&format!(
            r"(?m)^([ \t]+){}[ \t]*=[ \t]*{};[ \t]*(?:\n|$)",
            regex::escape(left),
            regex::escape(right)
        ));
        only(
            &pattern,
            code,
            &format!("requires one assignment {left} = {right}"),
        )
        .map(|found| (found.get(0).unwrap().range(), capture(source, &found, 1)))
    };
    let (first, first_indent) = assignment(first_destination, first_source)?;
    let (second, second_indent) = assignment(second_destination, second_source)?;
    if first.end > second.start
        || !code[first.end..second.start].trim().is_empty()
        || first_indent != second_indent
        || scope(code, first.start) != scope(code, second.start)
    {
        return Err(
            "adjacent-halfword assignments must be consecutive in one lexical block".into(),
        );
    }
    let dependent = regex(&format!(
        r"(?m)^([ \t]+)([^;\n=]+?)[ \t]*=[ \t]*([A-Za-z_][A-Za-z0-9_]*)\([ \t]*{}[ \t]*<<[ \t]*([0-9]+)[ \t]*,[ \t]*{}[ \t]*\);[ \t]*(?:\n|$)",
        regex::escape(first_destination),
        regex::escape(first_destination)
    ));
    let dependent = only(
        &dependent,
        code,
        "requires one dependent two-argument signed-carrier call",
    )?;
    let dependent_range = dependent.get(0).unwrap().range();
    if dependent_range.start < second.end
        || scope(code, dependent_range.start) != scope(code, first.start)
        || !simple_lvalue(capture(source, &dependent, 2).trim())
    {
        return Err("dependent signed-carrier call is outside the copy block".into());
    }
    let copy = format!(
        "{first_indent}do {{\n{first_indent}    *(u16 *)&{first_destination} = *(u16 *)&{first_source};\n{first_indent}    *(u16 *)&{second_destination} = *(u16 *)&{second_source};\n{first_indent}}} while (0);\n"
    );
    let dependent_text = format!(
        "{}{{\n{}    s16 {carrier} = {first_destination};\n\n{}    {} = {}({carrier} << {}, {carrier});\n{}}}\n",
        capture(source, &dependent, 1),
        capture(source, &dependent, 1),
        capture(source, &dependent, 1),
        capture(source, &dependent, 2).trim(),
        capture(source, &dependent, 3),
        capture(source, &dependent, 4),
        capture(source, &dependent, 1),
    );
    Ok(vec![(
        edited(
            source,
            vec![
                (first.start..second.end, copy),
                (dependent_range, dependent_text),
            ],
        )?,
        format!(
            "temporary+evaluation_order+type_width:preload_adjacent_halfwords({first_destination},{second_destination},{carrier})@{}",
            first.start
        ),
    )])
}

fn materialize_message_and_merge_count(
    source: &str,
    code: &str,
    indexed_value: &str,
    message: &str,
    coordinate: &str,
    count: &str,
) -> Mutations {
    if source.contains("volatile")
        || ![indexed_value, message, coordinate, count]
            .iter()
            .all(|name| identifier(name))
        || words(message).is_match(code)
    {
        return Err("message/count repair requires four named nonvolatile identifiers and a fresh message name".into());
    }
    let item_declaration = regex(&format!(
        r"(?m)^([ \t]+)u16[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
        regex::escape(indexed_value)
    ));
    let item_declaration = only(
        &item_declaration,
        code,
        &format!("requires one u16 declaration of {indexed_value}"),
    )?;
    let count_declaration = regex(&format!(
        r"(?m)^[ \t]+s32[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
        regex::escape(count)
    ));
    let count_declaration = only(
        &count_declaration,
        code,
        &format!("requires one s32 declaration of {count}"),
    )?;
    let coordinate_assignment = regex(&format!(
        r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
        regex::escape(coordinate)
    ));
    let coordinate_assignment = only(
        &coordinate_assignment,
        code,
        &format!("requires one coordinate assignment to {coordinate}"),
    )?;
    if !pure(capture(source, &coordinate_assignment, 2).trim()) {
        return Err(
            "coordinate assignment must be side-effect-free before message scheduling".into(),
        );
    }
    let item_assignment = regex(&format!(
        r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
        regex::escape(indexed_value)
    ));
    let assignments = item_assignment.captures_iter(code).collect::<Vec<_>>();
    if assignments.len() != 2 {
        return Err(format!(
            "message/count repair requires exactly two loads into {indexed_value}"
        ));
    }
    let first = assignments[0].get(0).unwrap().range();
    let second = assignments[1].get(0).unwrap().range();
    let first_expression = capture(source, &assignments[0], 2).trim();
    let second_expression = capture(source, &assignments[1], 2).trim();
    if first_expression != second_expression
        || !pure(first_expression)
        || variable_uses(
            &code[item_declaration.get(0).unwrap().start()..],
            indexed_value,
        ) != 5
        || scope(code, first.start) != scope(code, second.start)
        || scope(code, first.start) != scope(code, coordinate_assignment.get(0).unwrap().start())
        || !(first.end <= coordinate_assignment.get(0).unwrap().start()
            && coordinate_assignment.get(0).unwrap().end() <= second.start)
    {
        return Err(
            "indexed-message repair found ambiguous loads, uses, or scheduling order".into(),
        );
    }
    let first_statement = statement_after(code, first.end)?;
    let second_statement = statement_after(code, second.end)?;
    let first_uses = words(indexed_value)
        .find_iter(&code[first_statement.clone()])
        .collect::<Vec<_>>();
    if first_uses.len() != 1 {
        return Err("first indexed load must feed exactly one following statement".into());
    }
    let first_use =
        first_statement.start + first_uses[0].start()..first_statement.start + first_uses[0].end();
    let message_expression = regex(&format!(
        r"\([ \t]*{}[ \t]*&[ \t]*[^)\n]+\)[ \t]*\+[ \t]*[^,\n]+",
        regex::escape(indexed_value)
    ));
    let message_expression = only(
        &message_expression,
        &code[second_statement.clone()],
        "second indexed load must feed one masked message expression",
    )?;
    let message_range = second_statement.start + message_expression.get(0).unwrap().start()
        ..second_statement.start + message_expression.get(0).unwrap().end();
    let message_rhs = words(indexed_value)
        .replace(
            &source[message_range.clone()],
            format!("({first_expression})"),
        )
        .into_owned();
    let branch = regex(&format!(
        r"(?m)^([ \t]*)if \(([^;\n]+?) == ([^)\n]+)\) \{{\n([ \t]+){} = ([^;\n]+);\n([ \t]*)\}} else \{{\n([ \t]+){} = ([^;\n]+?) - 1;\n([ \t]*)\}}[ \t]*(?:\n|$)",
        regex::escape(count),
        regex::escape(count)
    ));
    let branch = only(
        &branch,
        code,
        &format!("requires one sentinel branch assigning {count}"),
    )?;
    if capture(source, &branch, 2).trim() != capture(source, &branch, 8).trim()
        || capture(source, &branch, 1) != capture(source, &branch, 6)
        || capture(source, &branch, 1) != capture(source, &branch, 9)
        || capture(source, &branch, 4) != capture(source, &branch, 7)
        || scope(code, branch.get(0).unwrap().start()) != scope(code, first.start)
        || count_declaration.get(0).unwrap().start() > first.start
        || ![
            capture(source, &branch, 2),
            capture(source, &branch, 3),
            capture(source, &branch, 5),
        ]
        .iter()
        .all(|expression| pure(expression.trim()))
    {
        return Err("sentinel branch does not have one reusable count carrier".into());
    }
    let branch_text = format!(
        "{}{} = {};\n{}if ({} == {}) {{\n{}{} = {};\n{}}} else {{\n{}{}--;\n{}}}\n",
        capture(source, &branch, 1),
        count,
        capture(source, &branch, 2).trim(),
        capture(source, &branch, 1),
        count,
        capture(source, &branch, 3).trim(),
        capture(source, &branch, 4),
        count,
        capture(source, &branch, 5).trim(),
        capture(source, &branch, 1),
        capture(source, &branch, 7),
        count,
        capture(source, &branch, 1),
    );
    let declaration_text = format!("{}s32 {message};\n", capture(source, &item_declaration, 1));
    let coordinate_start = coordinate_assignment.get(0).unwrap().start();
    let message_text = format!(
        "{}{} = {};\n",
        capture(source, &coordinate_assignment, 1),
        message,
        message_rhs
    );
    Ok(vec![(
        edited(
            source,
            vec![
                (item_declaration.get(0).unwrap().range(), declaration_text),
                (first, String::new()),
                (first_use, format!("({first_expression})")),
                (coordinate_start..coordinate_start, message_text),
                (second, String::new()),
                (message_range, message.into()),
                (branch.get(0).unwrap().range(), branch_text),
            ],
        )?,
        format!(
            "temporary+evaluation_order+block_lifetime:materialize_message_and_merge_count({message},{count})@{}",
            item_declaration.get(0).unwrap().start()
        ),
    )])
}

fn split_opposite_side_and_scaled_offset(
    source: &str,
    code: &str,
    side: &str,
    opposite: &str,
) -> Mutations {
    if source.contains("volatile")
        || !identifier(side)
        || !identifier(opposite)
        || words(opposite).is_match(code)
    {
        return Err("opposite-side repair requires two named nonvolatile identifiers and a fresh opposite-side name".into());
    }
    let declaration = regex(&format!(
        r"(?m)^([ \t]+)u(?:8|16|32)[ \t]+{}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
        regex::escape(side)
    ));
    let declaration = only(
        &declaration,
        code,
        &format!("requires one initialized integral declaration of {side}"),
    )?;
    let scaled = regex(&format!(
        r"\([ \t]*\([ \t]*1[ \t]*\^[ \t]*{}[ \t]*\)[ \t]*\*[ \t]*24[ \t]*\)",
        regex::escape(side)
    ));
    let scaled = only(
        &scaled,
        code,
        "requires one (1 ^ side) * 24 address expression",
    )?;
    if variable_uses(code, side) != 2
        || !pure(capture(source, &declaration, 2).trim())
        || scope(code, declaration.get(0).unwrap().start())
            != scope(code, scaled.get(0).unwrap().start())
        || scaled.get(0).unwrap().start() < declaration.get(0).unwrap().end()
    {
        return Err("opposite-side expression has ambiguous uses or scope".into());
    }
    let statement_start = code[..scaled.get(0).unwrap().start()]
        .rfind(|character| matches!(character, ';' | '{' | '}'))
        .map_or(0, |at| at + 1)
        + code[code[..scaled.get(0).unwrap().start()]
            .rfind(|character| matches!(character, ';' | '{' | '}'))
            .map_or(0, |at| at + 1)..scaled.get(0).unwrap().start()]
            .find(|character: char| !character.is_ascii_whitespace())
            .ok_or("scaled expression has no containing statement")?;
    let indent = capture(source, &declaration, 1);
    let declaration_text = format!(
        "{indent}u32 {side} = {};\n{indent}u32 {opposite} = 1;\n",
        capture(source, &declaration, 2).trim()
    );
    let schedule = format!(
        "{indent}{opposite} ^= {side};\n{indent}{side} = {opposite} << 1;\n{indent}{side} += {opposite};\n{indent}{side} <<= 3;\n"
    );
    Ok(vec![(
        edited(
            source,
            vec![
                (declaration.get(0).unwrap().range(), declaration_text),
                (statement_start..statement_start, schedule),
                (scaled.get(0).unwrap().range(), side.into()),
            ],
        )?,
        format!(
            "temporary+evaluation_order:split_opposite_side_and_scaled_offset({side},{opposite})@{}",
            declaration.get(0).unwrap().start()
        ),
    )])
}

fn merge_carrier_phases(source: &str, code: &str, earlier: &str, later: &str) -> Mutations {
    if source.contains("volatile") || !identifier(earlier) || !identifier(later) || earlier == later
    {
        return Err("phase-carrier repair requires two distinct nonvolatile identifiers".into());
    }
    let first = initialized_declaration(source, code, earlier)?;
    let (second_declaration, second_site, second_indent, second_kind, second_expression) =
        match initialized_declaration(source, code, later) {
            Ok(second) => (
                second.range.clone(),
                second.range,
                second.indent.to_string(),
                second.kind.to_string(),
                second.expression.to_string(),
            ),
            Err(_) => {
                let pattern = regex(&format!(
                    r"(?m)^([ \t]+)((?:u|s)(?:8|16|32))[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
                    regex::escape(later)
                ));
                let declaration = only(
                    &pattern,
                    code,
                    &format!("requires one initialized or assigned declaration of {later}"),
                )?;
                let assignment = regex(&format!(
                    r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
                    regex::escape(later)
                ));
                let assignment = only(
                    &assignment,
                    code,
                    &format!("requires one assignment starting the {later} phase"),
                )?;
                if assignment.get(0).unwrap().start() <= declaration.get(0).unwrap().end()
                    || words(later).is_match(
                        &code
                            [declaration.get(0).unwrap().end()..assignment.get(0).unwrap().start()],
                    )
                {
                    return Err(format!(
                        "later carrier {later} is live before its named phase"
                    ));
                }
                (
                    declaration.get(0).unwrap().range(),
                    assignment.get(0).unwrap().range(),
                    capture(source, &assignment, 1).to_string(),
                    capture(source, &declaration, 2).trim().to_string(),
                    capture(source, &assignment, 2).trim().to_string(),
                )
            }
        };
    if first.range.end >= second_site.start
        || second_kind.contains('*')
        || second_kind != "s32"
        || !(first.kind.ends_with('*') || first.kind == "s32")
        || !pure(first.expression)
        || !pure(&second_expression)
        || !scope_chain(code, second_site.start).contains(
            &scope(code, first.range.start).ok_or("earlier carrier is outside a lexical block")?,
        )
        || words(earlier).is_match(&code[second_site.end..])
        || words(later).is_match(&code[first.range.end..second_declaration.start])
    {
        return Err("carrier phases overlap, escape scope, or use unsupported types".into());
    }
    let pointer = first.kind.ends_with('*');
    let mut changes = Vec::new();
    let first_text = if pointer {
        format!(
            "{}s32 {earlier} = (s32){};\n",
            first.indent, first.expression
        )
    } else {
        format!("{}s32 {earlier} = {};\n", first.indent, first.expression)
    };
    changes.push((first.range.clone(), first_text));
    if pointer {
        for found in words(earlier).find_iter(&code[first.range.end..second_site.start]) {
            let range = first.range.end + found.start()..first.range.end + found.end();
            let tail = code[range.end..].trim_start();
            if tail.starts_with("->") {
                changes.push((range, format!("(({}){earlier})", first.kind)));
            } else if !(tail.starts_with('+')
                || tail.starts_with('-')
                || tail.starts_with("==")
                || tail.starts_with("!="))
            {
                return Err(format!(
                    "pointer carrier {earlier} has an unguarded pre-merge use"
                ));
            }
        }
    }
    if second_declaration != second_site {
        changes.push((second_declaration, String::new()));
    }
    changes.push((
        second_site.clone(),
        format!("{}{earlier} = {};\n", second_indent, second_expression),
    ));
    for found in words(later).find_iter(&code[second_site.end..]) {
        changes.push((
            second_site.end + found.start()..second_site.end + found.end(),
            earlier.into(),
        ));
    }
    Ok(vec![(
        edited(source, changes)?,
        format!(
            "temporary+block_lifetime:merge_carrier_phases({earlier},{later})@{}",
            first.range.start
        ),
    )])
}

fn guard_source(source: &str) -> Result<(), String> {
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
    Ok(())
}

fn generate(source: &str, repair: &Repair) -> Mutations {
    let code = masked(source);
    let generated = match repair {
        Repair::SwapDeclarations { left, right } => swap(source, &code, left, right)?,
        Repair::SplitLifetime { name } => split(source, &code, name)?,
        Repair::MergeZeroCarrier => zero(source, &code)?,
        Repair::ReciprocalRoleSwap { name } => reciprocal(source, &code, name)?,
        Repair::PreloadAdjacentHalfwords {
            first_destination,
            first_source,
            second_destination,
            second_source,
            carrier,
        } => preload_adjacent_halfwords(
            source,
            &code,
            first_destination,
            first_source,
            second_destination,
            second_source,
            carrier,
        )?,
        Repair::MaterializeMessageAndMergeCount {
            indexed_value,
            message,
            coordinate,
            count,
        } => materialize_message_and_merge_count(
            source,
            &code,
            indexed_value,
            message,
            coordinate,
            count,
        )?,
        Repair::SplitOppositeSideAndScaledOffset { side, opposite } => {
            split_opposite_side_and_scaled_offset(source, &code, side, opposite)?
        }
        Repair::MergeCarrierPhases { earlier, later } => {
            merge_carrier_phases(source, &code, earlier, later)?
        }
    };
    Ok(generated)
}

pub fn parse(source: &str, plan: &RepairPlan) -> Result<Permutation, String> {
    guard_source(source)?;
    if !(1..=2).contains(&plan.repairs().len()) {
        return Err("decoder repair plan must contain one or two repairs".into());
    }
    let generated = if plan.repairs().len() == 1 {
        generate(source, &plan.repairs()[0])?
            .into_iter()
            .map(|(source, mutation)| Variant {
                source,
                mutations: vec![mutation],
            })
            .collect::<Vec<_>>()
    } else {
        let mut complete = Vec::new();
        for (first_source, first_mutation) in generate(source, &plan.repairs()[0])? {
            for (second_source, second_mutation) in generate(&first_source, &plan.repairs()[1])? {
                complete.push(Variant {
                    source: second_source,
                    mutations: vec![first_mutation.clone(), second_mutation],
                });
            }
        }
        complete
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
    for variant in generated {
        if seen.insert(variant.source.clone()) {
            variants.push(variant);
        }
    }
    Ok(Permutation {
        variants,
        raw_count,
        dimensions: plan.dimensions(),
    })
}

pub fn validate_catalog() -> Result<(), String> {
    const TEXT: &str = include_str!("../../../games/gs1/recon/compiler-repair-patterns.json");
    let catalog: serde_json::Value =
        serde_json::from_str(TEXT).map_err(|error| error.to_string())?;
    if catalog["catalog_version"] != CATALOG_VERSION
        || catalog["search"]["max_edits_per_candidate"] != 2
        || catalog["generalization_backlog"]
            .as_array()
            .is_none_or(|backlog| !backlog.is_empty())
    {
        return Err("repair catalog version, edit bound, or generalization backlog drifted".into());
    }
    let recorded = catalog["recorded_repairs"]
        .as_array()
        .ok_or("repair catalog has no recorded_repairs array")?;
    if recorded.len() != 4 {
        return Err(format!(
            "repair catalog must contain four generalized recorded repairs, got {}",
            recorded.len()
        ));
    }
    let known = HashSet::from([
        "preload_adjacent_halfwords_before_signed_carrier",
        "materialize_indexed_message_and_merge_sentinel_carrier",
        "split_opposite_side_and_scaled_offset_carriers",
        "merge_nonoverlapping_carrier_phases",
    ]);
    for repair in recorded {
        let id = repair["id"].as_str().unwrap_or("unnamed");
        if repair["catalog_status"] != "generalized"
            || repair["regression_fixture"].as_str().is_none()
        {
            return Err(format!("recorded repair {id} is not gated and generalized"));
        }
        let operations = repair
            .get("operations")
            .and_then(serde_json::Value::as_array)
            .map(|values| values.iter().collect::<Vec<_>>())
            .unwrap_or_else(|| vec![&repair["operation"]]);
        if operations.is_empty()
            || operations.iter().any(|operation| {
                !operation
                    .as_str()
                    .is_some_and(|value| known.contains(value))
            })
        {
            return Err(format!("recorded repair {id} names an unknown operation"));
        }
    }
    let paired = recorded
        .iter()
        .find(|repair| repair["id"] == "paired-phase-carrier-merge")
        .ok_or("paired-phase repair is missing")?;
    if paired["decoder_repairs"]
        .as_array()
        .is_none_or(|repairs| repairs.len() != 2)
    {
        return Err("paired-phase repair must name exactly two decoder repairs".into());
    }
    Ok(())
}

pub fn self_test() -> Result<(), String> {
    validate_catalog()?;
    let repair = Repair::SwapDeclarations {
        left: "a".into(),
        right: "c".into(),
    };
    let source = "void f(void)\n{\n    u32 a;\n    u32 b;\n    u32 c;\n}\n";
    let permutation = parse(source, &RepairPlan::one(repair.clone()))?;
    if permutation.count() != 2
        || !permutation
            .evaluate(1)?
            .contains("u32 c;\n    u32 b;\n    u32 a;")
    {
        return Err("named non-adjacent declaration repair regressed".into());
    }
    if parse(
        "void f(void) { PERM_GENERAL(a,b); }",
        &RepairPlan::one(repair.clone()),
    )
    .is_ok()
        || parse(
            "void f(void) { Func_080072e4(); }",
            &RepairPlan::one(repair),
        )
        .is_ok()
    {
        return Err("annotation or trampoline guard regressed".into());
    }
    let repair = Repair::ReciprocalRoleSwap {
        name: "left".into(),
    };
    let source = "extern u32 m[];\n\nvoid g(void)\n{\n    {\n        u32 left = m[i].x;\n\n        a[i] ^= left;\n        a[j] ^= m[i].y;\n    }\n}\n";
    let permutation = parse(source, &RepairPlan::one(repair.clone()))?;
    let helper = permutation.evaluate(5)?;
    let calls = helper.matches("perm_xor(").count();
    if permutation.count() != 6 || calls != 3 || helper.contains("u32 left =") {
        return Err("inline XOR boundary repair regressed".into());
    }
    let crowded = source.replace("a[j] ^= m[i].y;", "a[j] ^= m[i].y;\n        b ^= c;");
    if parse(&crowded, &RepairPlan::one(repair))?.count() != 5 {
        return Err("inline XOR boundary singleton guard regressed".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn candidate(source: &str, repair: Repair) -> String {
        let permutation = parse(source, &RepairPlan::one(repair)).unwrap();
        assert_eq!(permutation.count(), 2);
        assert_eq!(permutation.mutations(1).unwrap().len(), 1);
        permutation.evaluate(1).unwrap()
    }

    #[test]
    fn catalog_has_no_ungeneralized_recorded_repairs() {
        validate_catalog().unwrap();
        assert!(
            !include_str!("../../../games/gs1/recon/compiler-repair-patterns.json")
                .contains("recorded-not-generalized")
        );
    }

    #[test]
    fn preloads_adjacent_halfwords_before_signed_carrier() {
        let source = "void f(void)\n{\n    s16 hp;\n    s16 pp;\n    s16 max_hp;\n    s16 max_pp;\n    s32 ratio;\n\n    hp = max_hp;\n    pp = max_pp;\n    ratio = FixedPoint_Ratio(hp << 14, hp);\n}\n";
        let output = candidate(
            source,
            Repair::PreloadAdjacentHalfwords {
                first_destination: "hp".into(),
                first_source: "max_hp".into(),
                second_destination: "pp".into(),
                second_source: "max_pp".into(),
                carrier: "max_hp_carrier".into(),
            },
        );
        assert!(output.contains("*(u16 *)&hp = *(u16 *)&max_hp;"));
        assert!(output.contains("s16 max_hp_carrier = hp;"));
        assert!(output.contains("FixedPoint_Ratio(max_hp_carrier << 14, max_hp_carrier)"));
    }

    #[test]
    fn materializes_message_and_merges_count_carrier() {
        let source = "void f(void)\n{\n    u8 *ability;\n    s32 y;\n    s32 range;\n    u16 item;\n\n    item = *(u16 *)(base + offset);\n    ability = Ability_GetData(mask & item);\n    y = row * 16 + 16;\n    item = *(u16 *)(base + offset);\n    Draw((item & mask) + message_base, y);\n    if (ability[8] == 255) {\n        range = 11;\n    } else {\n        range = ability[8] - 1;\n    }\n    DrawRange(range);\n}\n";
        let output = candidate(
            source,
            Repair::MaterializeMessageAndMergeCount {
                indexed_value: "item".into(),
                message: "message".into(),
                coordinate: "y".into(),
                count: "range".into(),
            },
        );
        assert!(!output.contains("u16 item;"));
        assert!(output.contains("s32 message;"));
        assert!(output.find("\n    message =").unwrap() < output.find("\n    y =").unwrap());
        assert!(output.contains("range = ability[8];"));
        assert!(output.contains("range--;"));
    }

    #[test]
    fn splits_opposite_side_and_scaled_offset_carriers() {
        let source = "void f(void)\n{\n    u8 side = work->side;\n    peer = (u16 *)(base + ((1 ^ side) * 24));\n}\n";
        let output = candidate(
            source,
            Repair::SplitOppositeSideAndScaledOffset {
                side: "side".into(),
                opposite: "other".into(),
            },
        );
        assert!(output.contains("u32 side = work->side;\n    u32 other = 1;"));
        assert!(output.contains("other ^= side;"));
        assert!(output.contains("side <<= 3;"));
        assert!(output.contains("base + side"));
    }

    #[test]
    fn composes_only_the_two_named_phase_merges() {
        let source = "struct Node { s32 value; };\n\nvoid f(void)\n{\n    struct Node *state = base;\n    struct Node *resource = *(struct Node **)(state + offset);\n    s32 spawn_z;\n\n    if (resource == 0)\n        return;\n    out = resource->value;\n    spawn_z = z;\n    if (*(s16 *)(state + 4) != 0) {\n        s32 count = 63;\n        do {\n            count--;\n        } while (count >= 0);\n    }\n    Use(spawn_z);\n}\n";
        let plan = RepairPlan::two(
            Repair::MergeCarrierPhases {
                earlier: "state".into(),
                later: "count".into(),
            },
            Repair::MergeCarrierPhases {
                earlier: "resource".into(),
                later: "spawn_z".into(),
            },
        );
        let permutation = parse(source, &plan).unwrap();
        assert_eq!(
            permutation.count(),
            2,
            "one-sided candidates leaked into the plan"
        );
        assert!(permutation.mutations(0).unwrap().is_empty());
        assert_eq!(permutation.mutations(1).unwrap().len(), 2);
        let output = permutation.evaluate(1).unwrap();
        assert!(!output.contains("s32 count ="));
        assert!(!output.contains("s32 spawn_z"));
        assert!(output.contains("s32 state = (s32)base;"));
        assert!(output.contains("s32 resource = (s32)*(struct Node **)(state + offset);"));
    }

    #[test]
    fn repair_plan_bound_is_enforced() {
        assert!(RepairPlan::try_from_repairs(Vec::new()).is_err());
        let repair = Repair::MergeZeroCarrier;
        assert!(
            RepairPlan::try_from_repairs(vec![repair.clone(), repair.clone(), repair]).is_err()
        );
    }
}
