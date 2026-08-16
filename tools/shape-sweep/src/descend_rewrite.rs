//! Equivalence-preserving source rewrites.
//!
//! Every rewrite here must preserve behaviour by construction. The search has a
//! byte oracle for *closeness*, but nothing checks semantics, so a rewrite that
//! can change meaning is a defect in this file, not something the driver can
//! catch later.
//!
//! The rewrite set is deliberately small and *measured*. Transformations that
//! were measured to lose on real owners (natural indexed `for` loops, clean loop
//! rotation, wholesale struct typing) are intentionally absent: spending
//! compiles on them is spending them on a known negative.

/// One concrete edit: the source it produces, plus a label for reporting.
#[derive(Clone, Debug)]
pub struct Variant {
    pub label: String,
    pub source: String,
}

fn indent_of(line: &str) -> usize {
    line.len() - line.trim_start().len()
}

/// An `if (...) { ... } else { ... }` whose arms can be exchanged.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct IfElse {
    /// line holding `if (`
    pub head: usize,
    /// line holding the `{` that opens the then-arm
    pub open: usize,
    /// line holding `} else {`
    pub els: usize,
    /// line holding the closing `}`
    pub end: usize,
}

/// Find the line holding the `{` matched by the leading `}` of `close_line`.
///
/// This must match braces at CHARACTER level, walking right to left. A
/// line-granular net-depth count cannot see `} else if (b) {` as an opener,
/// because its braces cancel, and it will then attach an inner `else` to the
/// wrong `if`.
fn match_opening_brace(lines: &[String], close_line: usize) -> Option<usize> {
    // Depth 1 accounts for the `}` we are matching, which we start just before.
    let mut depth = 1i32;
    let start_column = lines[close_line].find('}')?;
    let mut line = close_line;
    loop {
        let characters: Vec<char> = lines[line].chars().collect();
        // On the first line, begin at the `}` itself; afterwards, at line end.
        let mut index = if line == close_line { start_column } else { characters.len() };
        while index > 0 {
            index -= 1;
            match characters[index] {
                '}' => depth += 1,
                '{' => {
                    depth -= 1;
                    if depth == 0 {
                        return Some(line);
                    }
                }
                _ => {}
            }
        }
        if line == 0 {
            return None;
        }
        line -= 1;
    }
}

/// Locate every exchangeable if/else.
///
/// The brace walk starts at depth 1 to account for the `}` on the `} else {`
/// line. Starting at 0 makes that line's own braces cancel, which silently
/// matches the first *body* line as the if-header and produces spliced,
/// mid-statement garbage. That defect yielded 44 candidates with 32 compile
/// failures; the fix yields 53 with 6.
pub fn find_if_else(lines: &[String]) -> Vec<IfElse> {
    let mut found = Vec::new();
    for (j, line) in lines.iter().enumerate() {
        if line.trim() != "} else {" {
            continue;
        }
        let Some(open) = match_opening_brace(lines, j) else { continue };

        // The condition may span lines; walk back to the line holding `if (`.
        let mut head = open;
        loop {
            if lines[head].contains("if (") || lines[head].contains("if(") {
                break;
            }
            if head == 0 {
                break;
            }
            head -= 1;
        }
        let trimmed = lines[head].trim_start();
        if !trimmed.starts_with("if (") && !trimmed.starts_with("if(") {
            continue; // excludes `} else if (` chains
        }
        if indent_of(&lines[head]) != indent_of(line) {
            continue;
        }

        let mut depth: i32 = 1;
        let mut end = None;
        for q in (j + 1)..lines.len() {
            depth += lines[q].matches('{').count() as i32;
            depth -= lines[q].matches('}').count() as i32;
            if depth == 0 {
                end = Some(q);
                break;
            }
        }
        let Some(end) = end else { continue };
        found.push(IfElse { head, open, els: j, end });
    }
    found
}

/// Exchange the arms of one if/else, negating the condition.
///
/// `if (C) { A } else { B }` becomes `if (!(C)) { B } else { A }`, which is
/// behaviour-preserving for any side-effect-free-or-not condition, since the
/// condition is still evaluated exactly once and in the same place.
///
/// Arm ORDER is the axis that pays. Condition polarity alone is free; which arm
/// is emitted first was worth about 97 points on the first owner swept.
pub fn swap_arms(lines: &[String], site: IfElse) -> Option<String> {
    let header = lines[site.head..=site.open].join("\n");
    let negated = negate_condition(&header)?;
    let then_arm = &lines[site.open + 1..site.els];
    let else_arm = &lines[site.els + 1..site.end];

    let mut out: Vec<String> = Vec::with_capacity(lines.len());
    out.extend_from_slice(&lines[..site.head]);
    out.extend(negated.split('\n').map(str::to_string));
    out.extend_from_slice(else_arm);
    out.push(lines[site.els].clone());
    out.extend_from_slice(then_arm);
    out.extend_from_slice(&lines[site.end..]);
    Some(out.join("\n"))
}

/// Wrap a condition in `!( ... )`, or peel an existing wrap back off so that
/// repeated application does not pile up negations.
fn negate_condition(header: &str) -> Option<String> {
    let open = header.find("if (")? + 4;
    let close = header.rfind(") {")?;
    if close <= open {
        return None;
    }
    let condition = &header[open..close];
    let inner = condition.trim();
    let rewritten = if let Some(stripped) = inner.strip_prefix("!(") {
        // Only unwrap when the leading `!(` encloses the whole condition.
        if balanced_prefix(stripped) {
            stripped[..stripped.len() - 1].to_string()
        } else {
            format!("!({})", condition)
        }
    } else {
        format!("!({})", condition)
    };
    Some(format!("{}{}{}", &header[..open], rewritten, &header[close..]))
}

/// True when `text` ends exactly where its opening parenthesis closes, meaning
/// the surrounding `!( ... )` wraps the entire condition.
fn balanced_prefix(text: &str) -> bool {
    let mut depth = 1i32;
    for (index, character) in text.char_indices() {
        match character {
            '(' => depth += 1,
            ')' => {
                depth -= 1;
                if depth == 0 {
                    return index == text.len() - 1;
                }
            }
            _ => {}
        }
    }
    false
}

/// Every arm-order variant of a source.
pub fn arm_order_variants(source: &str) -> Vec<Variant> {
    let lines: Vec<String> = source.split('\n').map(str::to_string).collect();
    find_if_else(&lines)
        .into_iter()
        .filter_map(|site| {
            swap_arms(&lines, site).map(|source| Variant {
                label: format!("arm@{}", site.head + 1),
                source,
            })
        })
        .collect()
}

/// A simple statement: one line, ends in `;`, no control flow, no braces.
fn is_simple_statement(line: &str) -> bool {
    let t = line.trim();
    if !t.ends_with(';') || t.contains('{') || t.contains('}') {
        return false;
    }
    for keyword in [
        "return", "goto", "break", "continue", "case", "default", "do", "while", "for", "if",
        "else", "switch",
    ] {
        if t == keyword || t.starts_with(&format!("{} ", keyword)) || t.starts_with(&format!("{}(", keyword)) {
            return false;
        }
    }
    true
}

/// Identifiers referenced by a statement, and the one it assigns to (if any).
fn reads_and_write(line: &str) -> (Vec<String>, Option<String>) {
    let t = line.trim().trim_end_matches(';').trim();
    let mut names = Vec::new();
    let mut current = String::new();
    for character in t.chars() {
        if character.is_alphanumeric() || character == '_' {
            current.push(character);
        } else {
            if !current.is_empty() {
                names.push(std::mem::take(&mut current));
            }
        }
    }
    if !current.is_empty() {
        names.push(current);
    }

    // A plain `lhs = rhs` (not ==, !=, <=, >=, +=, and friends) writes lhs.
    let mut write = None;
    if let Some(position) = find_plain_assignment(t) {
        let lhs = t[..position].trim();
        if lhs.chars().all(|c| c.is_alphanumeric() || c == '_') && !lhs.is_empty() {
            write = Some(lhs.to_string());
        } else {
            // Compound lvalue (a->b, *p, a[i]): treat as writing nothing we can
            // prove, which makes the hazard check refuse the swap.
            write = Some(String::from("\u{0}opaque"));
        }
    }
    (names, write)
}

fn find_plain_assignment(text: &str) -> Option<usize> {
    let bytes = text.as_bytes();
    let mut depth = 0i32;
    for index in 0..bytes.len() {
        match bytes[index] {
            b'(' | b'[' => depth += 1,
            b')' | b']' => depth -= 1,
            b'=' if depth == 0 => {
                let previous = if index > 0 { bytes[index - 1] } else { b' ' };
                let next = if index + 1 < bytes.len() { bytes[index + 1] } else { b' ' };
                let compound = matches!(previous, b'!' | b'<' | b'>' | b'+' | b'-' | b'*' | b'/' | b'%' | b'&' | b'|' | b'^' | b'=');
                if !compound && next != b'=' {
                    return Some(index);
                }
            }
            _ => {}
        }
    }
    None
}

/// True when two adjacent simple statements can be exchanged without changing
/// meaning. Conservative on purpose: any function call, any pointer or member
/// lvalue, or any shared identifier blocks the swap.
pub fn can_swap(first: &str, second: &str) -> bool {
    if !is_simple_statement(first) || !is_simple_statement(second) {
        return false;
    }
    if indent_of(first) != indent_of(second) {
        return false;
    }
    // A call may touch anything. Refuse.
    if first.contains('(') && !first.contains("(u") && !first.contains("(s") {
        return false;
    }
    if second.contains('(') && !second.contains("(u") && !second.contains("(s") {
        return false;
    }
    let (reads_a, write_a) = reads_and_write(first);
    let (reads_b, write_b) = reads_and_write(second);
    let opaque = String::from("\u{0}opaque");
    if write_a.as_ref() == Some(&opaque) || write_b.as_ref() == Some(&opaque) {
        return false;
    }
    // Read-after-write, write-after-read, write-after-write.
    if let Some(w) = &write_a {
        if reads_b.contains(w) {
            return false;
        }
    }
    if let Some(w) = &write_b {
        if reads_a.contains(w) {
            return false;
        }
    }
    if write_a.is_some() && write_a == write_b {
        return false;
    }
    true
}

/// Every adjacent-statement exchange. This axis is untested on real owners;
/// arm order was worth ~97 points and is the same shape of problem.
pub fn statement_order_variants(source: &str) -> Vec<Variant> {
    let lines: Vec<String> = source.split('\n').map(str::to_string).collect();
    let mut variants = Vec::new();
    for index in 0..lines.len().saturating_sub(1) {
        if !can_swap(&lines[index], &lines[index + 1]) {
            continue;
        }
        let mut swapped = lines.clone();
        swapped.swap(index, index + 1);
        variants.push(Variant {
            label: format!("stmt@{}", index + 1),
            source: swapped.join("\n"),
        });
    }
    variants
}

/// Identifiers appearing in a fragment of C.
fn identifiers(text: &str) -> Vec<String> {
    let mut names = Vec::new();
    let mut current = String::new();
    for character in text.chars() {
        if character.is_alphanumeric() || character == '_' {
            current.push(character);
        } else if !current.is_empty() {
            names.push(std::mem::take(&mut current));
        }
    }
    if !current.is_empty() {
        names.push(current);
    }
    names
}

fn contains_call(text: &str) -> bool {
    // A `(` that is not a cast and not the start of a grouped expression
    // following an operator. Cheap and deliberately over-eager.
    let bytes: Vec<char> = text.chars().collect();
    for index in 0..bytes.len() {
        if bytes[index] != '(' {
            continue;
        }
        let mut back = index;
        while back > 0 && bytes[back - 1] == ' ' {
            back -= 1;
        }
        if back == 0 {
            continue;
        }
        let previous = bytes[back - 1];
        if previous.is_alphanumeric() || previous == '_' {
            // `name(` -- a call, unless it is a known type keyword.
            let mut start = back;
            while start > 0 && (bytes[start - 1].is_alphanumeric() || bytes[start - 1] == '_') {
                start -= 1;
            }
            let word: String = bytes[start..back].iter().collect();
            if !matches!(word.as_str(), "if" | "while" | "for" | "switch" | "return" | "sizeof") {
                return true;
            }
        }
    }
    false
}

/// True when the expression reads memory, so an intervening call could change it.
fn reads_memory(text: &str) -> bool {
    text.contains("->") || text.contains('[') || text.contains('*')
}

/// Un-cache a single-assignment local: replace its uses with its defining
/// expression and drop the declaration and the assignment.
///
/// This axis is a measured win. On the first owner swept, un-caching one local
/// was worth 98 halfwords, and it was found only after the *generator* was
/// widened, not the axis. An exhausted generator is not an exhausted axis.
///
/// Safety, which is this function's whole job:
/// the expression must be call-free; no identifier it reads may be assigned
/// between the definition and the last use; and if it reads memory, no call may
/// appear in that window either, since a call can write through any pointer.
pub fn uncache_variants(source: &str) -> Vec<Variant> {
    let lines: Vec<String> = source.split('\n').map(str::to_string).collect();
    let mut variants = Vec::new();

    // Locals declared one-per-line at the top of the function.
    let mut declared: Vec<(usize, String)> = Vec::new();
    for (index, line) in lines.iter().enumerate() {
        let t = line.trim();
        let Some(name) = t.strip_suffix(';') else { continue };
        let mut parts = name.split_whitespace();
        let (Some(type_word), Some(identifier)) = (parts.next(), parts.next()) else { continue };
        if parts.next().is_some() {
            continue;
        }
        if !matches!(type_word, "u8" | "s8" | "u16" | "s16" | "u32" | "s32" | "int") {
            continue;
        }
        let identifier = identifier.trim_start_matches('*');
        if identifier.chars().all(|c| c.is_alphanumeric() || c == '_') && !identifier.is_empty() {
            declared.push((index, identifier.to_string()));
        }
    }

    for (declaration_line, name) in declared {
        // Exactly one plain assignment to it, and at least two later uses.
        let mut assignment = None;
        let mut assignment_count = 0;
        let mut use_lines = Vec::new();
        for (index, line) in lines.iter().enumerate() {
            let mentions = identifiers(line).iter().any(|w| *w == name);
            if !mentions {
                continue;
            }
            let t = line.trim();
            if let Some(position) = find_plain_assignment(t.trim_end_matches(';')) {
                if t[..position].trim() == name {
                    assignment_count += 1;
                    assignment = Some((index, t[position + 1..].trim_end_matches(';').trim().to_string()));
                    continue;
                }
            }
            if index != declaration_line {
                use_lines.push(index);
            }
        }
        if assignment_count != 1 || use_lines.len() < 2 {
            continue;
        }
        let Some((assignment_line, expression)) = assignment else { continue };
        if use_lines.iter().any(|&u| u < assignment_line) {
            continue; // used before defined; not a simple cache
        }
        if contains_call(&expression) || expression.len() > 90 {
            continue;
        }

        let operands: Vec<String> = identifiers(&expression);
        let last_use = *use_lines.iter().max().unwrap();
        let mut hazard = false;
        for line in lines.iter().take(last_use + 1).skip(assignment_line + 1) {
            let t = line.trim();
            if reads_memory(&expression) && contains_call(t) {
                hazard = true;
                break;
            }
            if let Some(position) = find_plain_assignment(t.trim_end_matches(';')) {
                let written = t[..position].trim();
                if operands.iter().any(|operand| written.contains(operand.as_str())) {
                    hazard = true;
                    break;
                }
            }
        }
        if hazard {
            continue;
        }

        let mut out: Vec<String> = Vec::with_capacity(lines.len());
        for (index, line) in lines.iter().enumerate() {
            if index == declaration_line || index == assignment_line {
                continue;
            }
            if use_lines.contains(&index) {
                out.push(replace_identifier(line, &name, &format!("({expression})")));
            } else {
                out.push(line.clone());
            }
        }
        variants.push(Variant {
            label: format!("uncache:{name}"),
            source: out.join("\n"),
        });
    }
    variants
}

/// Replace whole-word occurrences of `name`, leaving `foo_name` and `name_bar`
/// alone.
fn replace_identifier(line: &str, name: &str, replacement: &str) -> String {
    let characters: Vec<char> = line.chars().collect();
    let target: Vec<char> = name.chars().collect();
    let mut out = String::with_capacity(line.len());
    let mut index = 0;
    while index < characters.len() {
        let matches_here = index + target.len() <= characters.len()
            && characters[index..index + target.len()] == target[..];
        let left_ok = index == 0 || !(characters[index - 1].is_alphanumeric() || characters[index - 1] == '_');
        let right = index + target.len();
        let right_ok = right >= characters.len()
            || !(characters[right].is_alphanumeric() || characters[right] == '_');
        if matches_here && left_ok && right_ok {
            out.push_str(replacement);
            index = right;
        } else {
            out.push(characters[index]);
            index += 1;
        }
    }
    out
}

/// True for a bare local declaration line such as `    s32 name;`.
fn is_declaration(line: &str) -> bool {
    let t = line.trim();
    let Some(body) = t.strip_suffix(';') else { return false };
    let mut parts = body.split_whitespace();
    let (Some(type_word), Some(name)) = (parts.next(), parts.next()) else { return false };
    if parts.next().is_some() {
        return false; // initialisers and multi-declarators are not reorderable here
    }
    if !matches!(
        type_word,
        "u8" | "s8" | "u16" | "s16" | "u32" | "s32" | "int" | "void" | "unsigned" | "struct"
    ) {
        return false;
    }
    let name = name.trim_start_matches('*');
    !name.is_empty() && name.chars().all(|c| c.is_alphanumeric() || c == '_')
}

/// Exchange two adjacent local declarations.
///
/// Declaration order is trivially behaviour-preserving for uninitialised locals,
/// and it is the ONLY move in this set that reaches register allocation, which
/// is otherwise a global property no source shuffling touches. Six owners in the
/// corpus sit at structural distance 0 while still not being byte-exact, meaning
/// they differ from the reference in allocation alone; nothing else here can
/// move them.
pub fn declaration_order_variants(source: &str) -> Vec<Variant> {
    let lines: Vec<String> = source.split('\n').map(str::to_string).collect();
    let mut variants = Vec::new();
    for index in 0..lines.len().saturating_sub(1) {
        if !is_declaration(&lines[index]) || !is_declaration(&lines[index + 1]) {
            continue;
        }
        let mut swapped = lines.clone();
        swapped.swap(index, index + 1);
        variants.push(Variant {
            label: format!("decl@{}", index + 1),
            source: swapped.join("\n"),
        });
    }
    variants
}

/// NOTE ON LAYERING. This module deliberately does NOT call
/// `alchemy-permuter::mutate`. The permuter already depends on this crate, so
/// depending back on it is a dependency cycle. The permuter is the right place
/// to combine random sampling with this descent, not the other way round.
///
/// The plausibility rule that lived with that integration is kept, because it is
/// about output quality, not about where variants come from: a byte-exact match
/// built from a degenerate construct is a cheat, not a result. Owner 08078144
/// reached byte-exact through `if (owner) { X; } else { X; }` with identical
/// arms, and was rejected.
pub fn is_plausible(variant: &Variant) -> bool {
    let lines: Vec<String> = variant.source.split('\n').map(str::to_string).collect();
    for site in find_if_else(&lines) {
        let then_arm: Vec<&str> = lines[site.open + 1..site.els].iter().map(|l| l.trim()).collect();
        let else_arm: Vec<&str> = lines[site.els + 1..site.end].iter().map(|l| l.trim()).collect();
        if !then_arm.is_empty() && then_arm == else_arm {
            return false;
        }
    }
    if variant.source.contains("} else {") {
        for line in variant.source.split('\n') {
            if let Some((then_part, else_part)) = line.split_once("} else {") {
                if let Some((_, body)) = then_part.split_once(") {") {
                    if body.trim() == else_part.trim_end_matches('}').trim() && !body.trim().is_empty() {
                        return false;
                    }
                }
            }
        }
    }
    true
}

/// The exhaustive part of the neighbourhood: every site of every local rewrite.
pub fn local_neighbourhood(source: &str) -> Vec<Variant> {
    let mut all = arm_order_variants(source);
    all.extend(statement_order_variants(source));
    all.extend(uncache_variants(source));
    all.extend(declaration_order_variants(source));
    all
}

/// The full single-edit neighbourhood. `_permuter_seeds` is accepted and
/// ignored: see the layering note above.
pub fn neighbourhood_with(source: &str, _permuter_seeds: usize, _limit: usize) -> Vec<Variant> {
    local_neighbourhood(source)
}

/// The full single-edit neighbourhood of a source, local rewrites only.
pub fn neighbourhood(source: &str) -> Vec<Variant> {
    local_neighbourhood(source)
}

/// Which line an edit touches, used to keep batched edits from overlapping.
pub fn touched_line(label: &str) -> Option<usize> {
    let (_, tail) = label.split_once('@')?;
    tail.parse().ok()
}

#[cfg(test)]
mod tests {
    use super::*;

    fn lines(text: &str) -> Vec<String> {
        text.split('\n').map(str::to_string).collect()
    }

    #[test]
    fn finds_a_simple_if_else() {
        let source = "    if (a == 1) {\n        b = 2;\n    } else {\n        b = 3;\n    }";
        let sites = find_if_else(&lines(source));
        assert_eq!(sites.len(), 1);
        assert_eq!(sites[0].head, 0);
        assert_eq!(sites[0].els, 2);
        assert_eq!(sites[0].end, 4);
    }

    #[test]
    fn brace_walk_does_not_match_the_body_line() {
        // Regression: a depth-0 walk matches line 1 (the body) as the header.
        let source = "    if (a == 1) {\n        b = 2;\n    } else {\n        b = 3;\n    }";
        let sites = find_if_else(&lines(source));
        assert_eq!(sites[0].open, 0, "open must be the `if` line, never the body");
    }

    #[test]
    fn skips_else_if_chains() {
        let source = "    if (a) {\n        x = 1;\n    } else if (b) {\n        x = 2;\n    } else {\n        x = 3;\n    }";
        // The `} else {` here belongs to an `} else if (` head, which is excluded.
        assert!(find_if_else(&lines(source)).is_empty());
    }

    #[test]
    fn swapping_arms_negates_and_exchanges() {
        let source = "    if (a == 1) {\n        b = 2;\n    } else {\n        b = 3;\n    }";
        let sites = find_if_else(&lines(source));
        let out = swap_arms(&lines(source), sites[0]).unwrap();
        assert!(out.contains("if (!(a == 1)) {"));
        let then_first = out.find("b = 3;").unwrap();
        let else_first = out.find("b = 2;").unwrap();
        assert!(then_first < else_first, "arms must actually exchange");
    }

    #[test]
    fn double_swap_returns_to_the_original() {
        let source = "    if (a == 1) {\n        b = 2;\n    } else {\n        b = 3;\n    }";
        let once = swap_arms(&lines(source), find_if_else(&lines(source))[0]).unwrap();
        let twice = swap_arms(&lines(&once), find_if_else(&lines(&once))[0]).unwrap();
        assert_eq!(twice, source, "negation must peel, not pile up");
    }

    #[test]
    fn independent_statements_swap() {
        assert!(can_swap("    a = 1;", "    b = 2;"));
    }

    #[test]
    fn dependent_statements_do_not_swap() {
        assert!(!can_swap("    a = 1;", "    b = a;"), "read-after-write");
        assert!(!can_swap("    b = a;", "    a = 1;"), "write-after-read");
        assert!(!can_swap("    a = 1;", "    a = 2;"), "write-after-write");
    }

    #[test]
    fn calls_and_compound_lvalues_never_swap() {
        assert!(!can_swap("    a = Func_08000000(1);", "    b = 2;"));
        assert!(!can_swap("    p->field = 1;", "    b = 2;"));
        assert!(!can_swap("    a = 1;", "    goto done;"));
    }

    #[test]
    fn casts_are_not_mistaken_for_calls() {
        assert!(can_swap("    a = (u32) x;", "    b = (s16) y;"));
    }

    const CACHE: &str = "    s32 level;\n    level = base + 1;\n    a = level;\n    b = level;";

    #[test]
    fn uncaches_a_single_assignment_local() {
        let variants = uncache_variants(CACHE);
        assert_eq!(variants.len(), 1, "expected one un-cache candidate");
        let out = &variants[0].source;
        assert!(!out.contains("s32 level;"), "declaration must go");
        assert!(!out.contains("level = base + 1;"), "assignment must go");
        assert_eq!(out.matches("(base + 1)").count(), 2, "both uses inlined");
    }

    #[test]
    fn refuses_when_an_operand_is_reassigned_between_uses() {
        let source = "    s32 level;\n    level = base + 1;\n    a = level;\n    base = 9;\n    b = level;";
        assert!(uncache_variants(source).is_empty(), "base changes in the window");
    }

    #[test]
    fn refuses_when_a_call_could_clobber_a_memory_read() {
        let source = "    s32 level;\n    level = unit->hp;\n    a = level;\n    Recalculate(id);\n    b = level;";
        assert!(uncache_variants(source).is_empty(), "call may write through the pointer");
    }

    #[test]
    fn a_call_is_harmless_when_the_expression_reads_no_memory() {
        let source = "    s32 level;\n    level = base + 1;\n    a = level;\n    Recalculate(id);\n    b = level;";
        assert_eq!(uncache_variants(source).len(), 1, "pure expression is call-immune");
    }

    #[test]
    fn refuses_a_defining_expression_that_calls() {
        let source = "    s32 level;\n    level = Compute(base);\n    a = level;\n    b = level;";
        assert!(uncache_variants(source).is_empty(), "would duplicate a call");
    }

    #[test]
    fn refuses_when_assigned_more_than_once() {
        let source = "    s32 level;\n    level = base + 1;\n    a = level;\n    level = base + 2;\n    b = level;";
        assert!(uncache_variants(source).is_empty());
    }

    #[test]
    fn replacement_respects_word_boundaries() {
        assert_eq!(replace_identifier("a = level + level_max;", "level", "(X)"), "a = (X) + level_max;");
        assert_eq!(replace_identifier("a = max_level;", "level", "(X)"), "a = max_level;");
    }

    #[test]
    fn call_detection_ignores_casts_and_keywords() {
        assert!(!contains_call("(u32) x + 1"));
        assert!(!contains_call("(a + b) * c"));
        assert!(contains_call("Func_08000000(1)"));
    }

    #[test]
    fn touched_line_parses_positional_labels_only() {
        assert_eq!(touched_line("arm@990"), Some(990));
        assert_eq!(touched_line("stmt@12"), Some(12));
        assert_eq!(touched_line("uncache:level"), None);
    }
}

#[cfg(test)]
mod plausibility_tests {
    use super::*;

    fn variant(label: &str, source: &str) -> Variant {
        Variant { label: label.to_string(), source: source.to_string() }
    }

    #[test]
    fn rejects_identical_arms() {
        let source = "    if (owner) {\n        a = 1;\n    } else {\n        a = 1;\n    }";
        assert!(!is_plausible(&variant("arm@1", source)), "duplicated arms are not source");
    }

    #[test]
    fn rejects_the_single_line_degenerate_form() {
        let source = "    if (owner) { a = 1; } else { a = 1; }";
        assert!(!is_plausible(&variant("perm:x", source)));
    }

    #[test]
    fn accepts_genuine_differing_arms() {
        let source = "    if (owner) {\n        a = 1;\n    } else {\n        a = 2;\n    }";
        assert!(is_plausible(&variant("arm@1", source)));
    }

    #[test]
    fn accepts_ordinary_structural_passes() {
        assert!(is_plausible(&variant("perm:region-0:perm_reorder_stmts:s-1", "    a = 1;")));
        assert!(is_plausible(&variant("uncache:limit", "    a = 1;")));
    }
}
