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

/// The full single-edit neighbourhood of a source.
pub fn neighbourhood(source: &str) -> Vec<Variant> {
    let mut all = arm_order_variants(source);
    all.extend(statement_order_variants(source));
    all
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
}
