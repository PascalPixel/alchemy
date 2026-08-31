use candidate_show::allocator::{Repair, RepairPlan};
use compiler_core::CALL_VIA_BASE;
use regex::{Captures, Regex};
use std::{collections::HashSet, ops::Range};
pub const CATALOG_VERSION: &str = "structural-v1";
const MAX_CHOICES: usize = 16;
#[derive(Clone, Debug, Eq, PartialEq)]
struct Mutation {
    source: String,
    description: String,
}
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
}
impl Permutation {
    pub fn count(&self) -> usize {
        self.variants.len()
    }
    pub fn raw_count(&self) -> usize {
        self.raw_count
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
#[derive(Debug)]
struct Site {
    range: Range<usize>,
    fields: Vec<String>,
}
fn sites(source: &str, code: &str, pattern: &str) -> Vec<Site> {
    regex(pattern)
        .captures_iter(code)
        .map(|found| Site {
            range: found.get(0).unwrap().range(),
            fields: (1..found.len())
                .map(|index| capture(source, &found, index).to_string())
                .collect(),
        })
        .collect()
}
fn one_site(source: &str, code: &str, pattern: &str, reason: &str) -> Result<Site, String> {
    let mut found = sites(source, code, pattern);
    if found.len() != 1 {
        return Err(reason.into());
    }
    Ok(found.pop().unwrap())
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
fn applied(
    source: &str,
    changes: Vec<(Range<usize>, String)>,
    description: String,
) -> Result<Mutation, String> {
    Ok(Mutation {
        source: edited(source, changes)?,
        description,
    })
}
fn variants(source: &str, range: Range<usize>, forms: Vec<(&str, String)>) -> Mutations {
    forms
        .into_iter()
        .map(|(id, text)| {
            applied(
                source,
                vec![(range.clone(), text)],
                format!("{id}@{}", range.start),
            )
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
    scope_chain(code, end).last().copied()
}
fn declaration<'a>(
    source: &'a str,
    code: &str,
    name: &str,
) -> Result<(Range<usize>, &'a str), String> {
    let pattern = regex(&format!(
        r"(?m)^[ \t]+(?:(?:const|volatile)[ \t]+)*(?:struct[ \t]+[A-Za-z_][A-Za-z0-9_]*|(?:u|s)(?:8|16|32)|void)[ \t*]+{}[ \t]*(?:\[[0-9]+\])?[ \t]*;[ \t]*(?:\n|$)",
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
fn identifier(name: &str) -> bool {
    regex(r"^[A-Za-z_][A-Za-z0-9_]*$").is_match(name)
}
fn simple_lvalue(value: &str) -> bool {
    regex(r"^[A-Za-z_][A-Za-z0-9_]*(?:(?:->|\.)[A-Za-z_][A-Za-z0-9_]*)*$").is_match(value)
}
fn statement_after(code: &str, at: usize) -> Result<Range<usize>, String> {
    let found = regex(r"(?s)^\s*([^{};\s][^{};]*;)")
        .captures(&code[at..])
        .ok_or("expected one following statement without a block boundary")?;
    let range = found.get(1).unwrap().range();
    Ok(at + range.start..at + range.end)
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
    Ok(vec![applied(
        source,
        vec![(a, b_line.into()), (b, a_line.into())],
        format!("declaration_order:swap_named({left},{right})@{at}"),
    )?])
}
fn split(source: &str, code: &str, name: &str) -> Mutations {
    let (declaration, line) = declaration(source, code, name)?;
    if regex(r"\*[ \t]*volatile\b").is_match(line) || !line.contains('*') {
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
    Ok(vec![applied(
        source,
        changes,
        format!(
            "block_lifetime+loop_spelling:inline_named({name})@{}",
            declaration.start
        ),
    )?])
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
    applied(
        source,
        changes,
        format!(
            "temporary+evaluation_order:inline_xor_helper@{}",
            site.start
        ),
    )
    .ok()
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
    let names = [
        first_destination,
        first_source,
        second_destination,
        second_source,
    ];
    if source.contains("volatile")
        || !identifier(carrier)
        || words(carrier).is_match(code)
        || !names.iter().all(|name| simple_lvalue(name))
    {
        return Err("adjacent-halfword repair requires named nonvolatile lvalues".into());
    }
    let copies = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+){}[ \t]*=[ \t]*{};[ \t]*\n([ \t]+){}[ \t]*=[ \t]*{};[ \t]*(?:\n|$)",
            regex::escape(first_destination),
            regex::escape(first_source),
            regex::escape(second_destination),
            regex::escape(second_source)
        ),
        "requires two consecutive named halfword assignments",
    )?;
    let dependent = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+)([^;\n=]+?)[ \t]*=[ \t]*([A-Za-z_][A-Za-z0-9_]*)\([ \t]*{}[ \t]*<<[ \t]*([0-9]+)[ \t]*,[ \t]*{}[ \t]*\);[ \t]*(?:\n|$)",
            regex::escape(first_destination),
            regex::escape(first_destination)
        ),
        "requires one dependent signed-carrier call",
    )?;
    if copies.fields[0] != copies.fields[1]
        || dependent.range.start < copies.range.end
        || scope(code, dependent.range.start) != scope(code, copies.range.start)
        || !simple_lvalue(dependent.fields[1].trim())
    {
        return Err("named copies and dependent call must share one block".into());
    }
    let indent = &copies.fields[0];
    let copy = format!(
        "{indent}do {{\n{indent}    *(u16 *)&{first_destination} = *(u16 *)&{first_source};\n{indent}    *(u16 *)&{second_destination} = *(u16 *)&{second_source};\n{indent}}} while (0);\n"
    );
    let fields = &dependent.fields;
    let dependent_text = format!(
        "{}{{\n{}    s16 {carrier} = {first_destination};\n\n{}    {} = {}({carrier} << {}, {carrier});\n{}}}\n",
        fields[0], fields[0], fields[0], fields[1].trim(), fields[2], fields[3], fields[0],
    );
    Ok(vec![applied(
        source,
        vec![
            (copies.range.clone(), copy),
            (dependent.range, dependent_text),
        ],
        format!(
            "temporary+evaluation_order+type_width:preload_adjacent_halfwords({first_destination},{second_destination},{carrier})@{}",
            copies.range.start
        ),
    )?])
}
fn materialize_message_and_merge_count(
    source: &str,
    code: &str,
    indexed_value: &str,
    message: &str,
    coordinate: &str,
    count: &str,
) -> Mutations {
    let names = [indexed_value, message, coordinate, count];
    if source.contains("volatile")
        || !names.iter().all(|name| identifier(name))
        || words(message).is_match(code)
    {
        return Err("message/count repair requires named nonvolatile carriers".into());
    }
    let declaration = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+)u16[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
            regex::escape(indexed_value)
        ),
        "requires one indexed-value declaration",
    )?;
    let coordinate_site = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
            regex::escape(coordinate)
        ),
        "requires one coordinate assignment",
    )?;
    let count_declaration = one_site(
        source,
        code,
        &format!(
            r"(?m)^[ \t]+s32[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
            regex::escape(count)
        ),
        "requires one signed count declaration",
    )?;
    let loads = sites(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
            regex::escape(indexed_value)
        ),
    );
    if loads.len() != 2 {
        return Err("requires exactly two indexed loads".into());
    }
    let branch = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]*)if \(([^;\n]+?) == ([^)\n]+)\) \{{\n([ \t]+){} = ([^;\n]+);\n[ \t]*\}} else \{{\n([ \t]+){} = ([^;\n]+?) - 1;\n[ \t]*\}}[ \t]*(?:\n|$)",
            regex::escape(count),
            regex::escape(count)
        ),
        "requires one sentinel branch",
    )?;
    let (first, second) = (&loads[0], &loads[1]);
    let expression = first.fields[1].trim();
    let fields = &branch.fields;
    if expression != second.fields[1].trim()
        || variable_uses(&code[declaration.range.start..], indexed_value) != 5
        || !(first.range.end <= coordinate_site.range.start
            && coordinate_site.range.end <= second.range.start
            && second.range.end <= branch.range.start)
        || scope(code, first.range.start) != scope(code, branch.range.start)
        || scope(code, first.range.start) != scope(code, second.range.start)
        || scope(code, first.range.start) != scope(code, coordinate_site.range.start)
        || count_declaration.range.start > first.range.start
        || fields[1].trim() != fields[6].trim()
        || ![
            expression,
            coordinate_site.fields[1].trim(),
            fields[1].trim(),
            fields[2].trim(),
            fields[4].trim(),
        ]
        .iter()
        .all(|value| pure(value))
    {
        return Err("message/count sites are ambiguous or reordered".into());
    }
    let first_statement = statement_after(code, first.range.end)?;
    let second_statement = statement_after(code, second.range.end)?;
    let unique_word = |range: Range<usize>| {
        let found = words(indexed_value)
            .find_iter(&code[range.clone()])
            .collect::<Vec<_>>();
        (found.len() == 1).then(|| range.start + found[0].start()..range.start + found[0].end())
    };
    let first_use =
        unique_word(first_statement).ok_or("first load must feed its next statement")?;
    let message_pattern = regex(&format!(
        r"\([ \t]*{}[ \t]*&[ \t]*[^)\n]+\)[ \t]*\+[ \t]*[^,\n]+",
        regex::escape(indexed_value)
    ));
    let message_site = only(
        &message_pattern,
        &code[second_statement.clone()],
        "second load must feed its next statement",
    )?;
    let message_range = second_statement.start + message_site.get(0).unwrap().start()
        ..second_statement.start + message_site.get(0).unwrap().end();
    let message_rhs = words(indexed_value)
        .replace(&source[message_range.clone()], format!("({expression})"))
        .into_owned();
    let branch_text = format!(
        "{}{} = {};\n{}if ({} == {}) {{\n{}{} = {};\n{}}} else {{\n{}{}--;\n{}}}\n",
        fields[0],
        count,
        fields[1].trim(),
        fields[0],
        count,
        fields[2].trim(),
        fields[3],
        count,
        fields[4].trim(),
        fields[0],
        fields[5],
        count,
        fields[0],
    );
    let message_text = format!(
        "{}{} = {};\n",
        coordinate_site.fields[0], message, message_rhs
    );
    Ok(vec![applied(
        source,
        vec![
            (declaration.range.clone(), format!("{}s32 {message};\n", declaration.fields[0])),
            (first.range.clone(), String::new()),
            (first_use, format!("({expression})")),
            (coordinate_site.range.start..coordinate_site.range.start, message_text),
            (second.range.clone(), String::new()),
            (message_range, message.into()),
            (branch.range, branch_text),
        ],
        format!(
            "temporary+evaluation_order+block_lifetime:materialize_message_and_merge_count({message},{count})@{}",
            declaration.range.start
        ),
    )?])
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
    let declaration = one_site(
        source,
        code,
        &format!(
            r"(?m)^([ \t]+)u(?:8|16|32)[ \t]+{}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
            regex::escape(side)
        ),
        &format!("requires one initialized integral declaration of {side}"),
    )?;
    let scaled = one_site(
        source,
        code,
        &format!(
            r"\([ \t]*\([ \t]*1[ \t]*\^[ \t]*{}[ \t]*\)[ \t]*\*[ \t]*24[ \t]*\)",
            regex::escape(side)
        ),
        "requires one (1 ^ side) * 24 address expression",
    )?;
    if variable_uses(code, side) != 2
        || !pure(declaration.fields[1].trim())
        || scope(code, declaration.range.start) != scope(code, scaled.range.start)
        || scaled.range.start < declaration.range.end
    {
        return Err("opposite-side expression has ambiguous uses or scope".into());
    }
    let boundary = code[..scaled.range.start]
        .rfind(|character| matches!(character, ';' | '{' | '}'))
        .map_or(0, |at| at + 1);
    let statement_start = boundary
        + code[boundary..scaled.range.start]
            .find(|character: char| !character.is_ascii_whitespace())
            .ok_or("scaled expression has no containing statement")?;
    let indent = &declaration.fields[0];
    let declaration_text = format!(
        "{indent}u32 {side} = {};\n{indent}u32 {opposite} = 1;\n",
        declaration.fields[1].trim()
    );
    let schedule = format!(
        "{indent}{opposite} ^= {side};\n{indent}{side} = {opposite} << 1;\n{indent}{side} += {opposite};\n{indent}{side} <<= 3;\n"
    );
    Ok(vec![applied(
        source,
        vec![
            (declaration.range.clone(), declaration_text),
            (statement_start..statement_start, schedule),
            (scaled.range, side.into()),
        ],
        format!(
            "temporary+evaluation_order:split_opposite_side_and_scaled_offset({side},{opposite})@{}",
            declaration.range.start
        ),
    )?])
}
fn merge_carrier_phases(source: &str, code: &str, earlier: &str, later: &str) -> Mutations {
    if source.contains("volatile") || !identifier(earlier) || !identifier(later) || earlier == later
    {
        return Err("phase-carrier repair requires two distinct nonvolatile identifiers".into());
    }
    let initialized = |name: &str| {
        one_site(
            source,
            code,
            &format!(
                r"(?m)^([ \t]+)((?:struct[ \t]+[A-Za-z_][A-Za-z0-9_]*|(?:u|s)(?:8|16|32)|void)[ \t]*\*?)[ \t]*{}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
                regex::escape(name)
            ),
            &format!("requires one initialized declaration of {name}"),
        )
    };
    let first = initialized(earlier)?;
    let (second_declaration, second_site, second_indent, second_kind, second_expression) =
        match initialized(later) {
            Ok(second) => (
                second.range.clone(),
                second.range,
                second.fields[0].clone(),
                second.fields[1].trim().to_string(),
                second.fields[2].trim().to_string(),
            ),
            Err(_) => {
                let declaration = one_site(
                    source,
                    code,
                    &format!(
                        r"(?m)^([ \t]+)((?:u|s)(?:8|16|32))[ \t]+{}[ \t]*;[ \t]*(?:\n|$)",
                        regex::escape(later)
                    ),
                    &format!("requires one initialized or assigned declaration of {later}"),
                )?;
                let assignment = one_site(
                    source,
                    code,
                    &format!(
                        r"(?m)^([ \t]+){}[ \t]*=[ \t]*([^;\n]+);[ \t]*(?:\n|$)",
                        regex::escape(later)
                    ),
                    &format!("requires one assignment starting the {later} phase"),
                )?;
                if assignment.range.start <= declaration.range.end
                    || words(later).is_match(&code[declaration.range.end..assignment.range.start])
                {
                    return Err(format!(
                        "later carrier {later} is live before its named phase"
                    ));
                }
                (
                    declaration.range,
                    assignment.range,
                    assignment.fields[0].clone(),
                    declaration.fields[1].trim().to_string(),
                    assignment.fields[1].trim().to_string(),
                )
            }
        };
    if first.range.end >= second_site.start
        || second_kind.contains('*')
        || second_kind != "s32"
        || !(first.fields[1].trim().ends_with('*') || first.fields[1].trim() == "s32")
        || !pure(first.fields[2].trim())
        || !pure(&second_expression)
        || !scope_chain(code, second_site.start).contains(
            &scope(code, first.range.start).ok_or("earlier carrier is outside a lexical block")?,
        )
        || words(earlier).is_match(&code[second_site.end..])
        || words(later).is_match(&code[first.range.end..second_declaration.start])
    {
        return Err("carrier phases overlap, escape scope, or use unsupported types".into());
    }
    let kind = first.fields[1].trim();
    let expression = first.fields[2].trim();
    let pointer = kind.ends_with('*');
    let mut changes = Vec::new();
    let first_text = if pointer {
        format!("{}s32 {earlier} = (s32){};\n", first.fields[0], expression)
    } else {
        format!("{}s32 {earlier} = {expression};\n", first.fields[0])
    };
    changes.push((first.range.clone(), first_text));
    if pointer {
        for found in words(earlier).find_iter(&code[first.range.end..second_site.start]) {
            let range = first.range.end + found.start()..first.range.end + found.end();
            let tail = code[range.end..].trim_start();
            if tail.starts_with("->") {
                changes.push((range, format!("(({kind}){earlier})")));
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
    Ok(vec![applied(
        source,
        changes,
        format!(
            "temporary+block_lifetime:merge_carrier_phases({earlier},{later})@{}",
            first.range.start
        ),
    )?])
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
    match repair {
        Repair::SwapDeclarations { left, right } => swap(source, &code, left, right),
        Repair::SplitLifetime { name } => split(source, &code, name),
        Repair::MergeZeroCarrier => zero(source, &code),
        Repair::ReciprocalRoleSwap { name } => reciprocal(source, &code, name),
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
        ),
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
        ),
        Repair::SplitOppositeSideAndScaledOffset { side, opposite } => {
            split_opposite_side_and_scaled_offset(source, &code, side, opposite)
        }
        Repair::MergeCarrierPhases { earlier, later } => {
            merge_carrier_phases(source, &code, earlier, later)
        }
        Repair::MirrorRelationalGuards => mirror_guards(source, &code),
    }
}

fn mirror_operator(op: &str) -> &'static str {
    match op {
        "<" => ">",
        ">" => "<",
        "<=" => ">=",
        ">=" => "<=",
        other => unreachable!("non-relational operator {other} cannot reach the mirror"),
    }
}

/// One mirrored spelling per simple relational guard: `if (a < b)` becomes
/// `if (b > a)`. Behaviour is identical; only the compare's operand order
/// moves, which is exactly what the structural decoder's guard-polarity
/// evidence names. Compound conditions, casts, calls, shifts, and impure
/// operands are refused; the byte score selects among the offered sites.
fn mirror_guards(source: &str, code: &str) -> Mutations {
    let found = sites(
        source,
        code,
        r"((?:\bif|\bwhile)\s*\(\s*)([A-Za-z_][A-Za-z0-9_>.\[\]* -]*?)\s*(<=|>=|<|>)\s*([A-Za-z_][A-Za-z0-9_>.\[\]* -]*?)(\s*\))",
    );
    let mut mutations = Vec::new();
    for site in &found {
        let [prefix, left, op, right, suffix] = &site.fields[..] else {
            continue;
        };
        if !pure(left) || !pure(right) {
            continue;
        }
        mutations.push(applied(
            source,
            vec![(
                site.range.clone(),
                format!("{prefix}{right} {} {left}{suffix}", mirror_operator(op)),
            )],
            format!("mirror_relational_guard({left},{right})"),
        ));
    }
    if mutations.is_empty() {
        return Err("mirror_relational_guards found no simple relational guard".into());
    }
    mutations.into_iter().collect()
}
pub fn parse(source: &str, plan: &RepairPlan) -> Result<Permutation, String> {
    guard_source(source)?;
    if !(1..=2).contains(&plan.repairs().len()) {
        return Err("decoder repair plan must contain one or two repairs".into());
    }
    let mut generated = vec![Variant {
        source: source.into(),
        mutations: Vec::new(),
    }];
    for repair in plan.repairs() {
        let mut next = Vec::new();
        for variant in generated {
            for mutation in generate(&variant.source, repair)? {
                let mut mutations = variant.mutations.clone();
                mutations.push(mutation.description);
                next.push(Variant {
                    source: mutation.source,
                    mutations,
                });
            }
        }
        generated = next;
    }
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
    })
}
#[cfg(test)]
fn validate_catalog() {
    const TEXT: &str = include_str!("../../../games/gs1/recon/compiler-repair-patterns.json");
    let catalog: serde_json::Value = serde_json::from_str(TEXT).unwrap();
    assert_eq!(catalog["catalog_version"], CATALOG_VERSION);
    assert_eq!(catalog["search"]["max_edits_per_candidate"], 2);
    assert_eq!(catalog["generalization_backlog"], serde_json::json!([]));
    let recorded = catalog["recorded_repairs"].as_array().unwrap();
    assert_eq!(recorded.len(), 4);
    let known = HashSet::from([
        "preload_adjacent_halfwords_before_signed_carrier",
        "materialize_indexed_message_and_merge_sentinel_carrier",
        "split_opposite_side_and_scaled_offset_carriers",
        "merge_nonoverlapping_carrier_phases",
    ]);
    for repair in recorded {
        assert_eq!(repair["catalog_status"], "generalized");
        assert!(repair["regression_fixture"].is_string());
        let operations = repair
            .get("operations")
            .and_then(serde_json::Value::as_array)
            .map(|values| values.iter().collect::<Vec<_>>())
            .unwrap_or_else(|| vec![&repair["operation"]]);
        assert!(!operations.is_empty());
        assert!(operations.iter().all(|operation| operation
            .as_str()
            .is_some_and(|value| known.contains(value))));
    }
    let paired = recorded
        .iter()
        .find(|repair| repair["id"] == "paired-phase-carrier-merge")
        .unwrap();
    assert_eq!(paired["decoder_repairs"].as_array().unwrap().len(), 2);
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
        validate_catalog();
        assert!(
            !include_str!("../../../games/gs1/recon/compiler-repair-patterns.json")
                .contains("recorded-not-generalized")
        );
    }
    #[test]
    fn annotation_free_search_is_guarded_and_finite() {
        let plan = RepairPlan::one(Repair::SwapDeclarations {
            left: "a".into(),
            right: "b".into(),
        });
        let source = "void f(void)\n{\n    u32 a;\n    u32 b;\n}\n";
        let permutation = parse(source, &plan).unwrap();
        assert_eq!(permutation.count(), 2);
        assert!(permutation
            .evaluate(1)
            .unwrap()
            .contains("u32 b;\n    u32 a;"));
        assert!(parse("void f(void) { PERM_GENERAL(a,b); }", &plan).is_err());
        assert!(parse("void f(void) { Func_080072e4(); }", &plan).is_err());
    }
    #[test]
    fn splits_pointer_to_volatile_object() {
        let source = "void f(void)\n{\n    volatile s32 *active;\n\n    active = (volatile s32 *)0x02002080;\n    busy = *active;\n    *active = value;\n}\n";
        let output = candidate(
            source,
            Repair::SplitLifetime {
                name: "active".into(),
            },
        );
        assert!(!output.contains("*active"));
        assert_eq!(output.matches("*((volatile s32 *)0x02002080)").count(), 2);
    }
    #[test]
    fn inline_xor_operation_remains_bounded() {
        let plan = RepairPlan::one(Repair::ReciprocalRoleSwap {
            name: "left".into(),
        });
        let source = "extern u32 m[];\n\nvoid g(void)\n{\n    {\n        u32 left = m[i].x;\n\n        a[i] ^= left;\n        a[j] ^= m[i].y;\n    }\n}\n";
        let permutation = parse(source, &plan).unwrap();
        assert_eq!(permutation.count(), 6);
        assert_eq!(
            permutation
                .evaluate(5)
                .unwrap()
                .matches("perm_xor(")
                .count(),
            3
        );
        let crowded = source.replace("a[j] ^= m[i].y;", "a[j] ^= m[i].y;\n        b ^= c;");
        assert_eq!(parse(&crowded, &plan).unwrap().count(), 5);
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
        let repair = Repair::MaterializeMessageAndMergeCount {
            indexed_value: "item".into(),
            message: "message".into(),
            coordinate: "y".into(),
            count: "range".into(),
        };
        let output = candidate(source, repair.clone());
        assert!(!output.contains("u16 item;"));
        assert!(output.contains("s32 message;"));
        assert!(output.find("\n    message =").unwrap() < output.find("\n    y =").unwrap());
        assert!(output.contains("range = ability[8];"));
        assert!(output.contains("range--;"));
        let missing = source.replace("    s32 range;\n", "");
        assert!(parse(&missing, &RepairPlan::one(repair.clone())).is_err());
        let side_effect = source.replace("    ability =", "    SideEffect();\n    ability =");
        assert!(parse(&side_effect, &RepairPlan::one(repair.clone())).is_err());
        let nested = source.replace(
            "    Draw((item & mask) + message_base, y);",
            "    {\n        Draw((item & mask) + message_base, y);\n    }",
        );
        assert!(parse(&nested, &RepairPlan::one(repair)).is_err());
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
