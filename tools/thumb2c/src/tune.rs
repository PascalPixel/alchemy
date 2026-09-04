//! Greedy call-spelling trials: each call line is respelled through the
//! wrappers or as a direct call, and a respelling stays when the scorer
//! reports fewer differing halfwords.

use crate::owners::{score, Score};
use std::path::Path;

/// Alternative spellings of one statement line, or none.
pub fn alternatives(line: &str) -> Vec<String> {
    let trimmed = line.trim_start();
    let lead = &line[..line.len() - trimmed.len()];
    let mut found = Vec::new();
    if let Some((lhs, value)) = trimmed.strip_suffix(';').and_then(|s| s.split_once(" = ")) {
        // A halfword store of a small constant: through an int local the
        // constant is SImode and loads with `movs`; as a literal it is HImode
        // and comes from the pool.
        if lhs.starts_with("*(u16 *)") && value.parse::<u32>().is_ok_and(|v| v < 256) {
            found.push(format!(
                "{lead}{{\n{lead}    s32 shown = {value};\n\n{lead}    {lhs} = shown;\n{lead}}}"
            ));
        }
    }
    let Some(statement) = trimmed.strip_suffix(");") else {
        return found;
    };
    if let Some(rest) = statement.strip_prefix("Func_") {
        if let Some((_, args)) = rest.split_once('(') {
            let callee = &statement[..statement.len() - args.len() - 1];
            let n = args.split(", ").filter(|a| !a.is_empty()).count();
            if (1..=4).contains(&n) {
                found.push(format!("{lead}Value{n}({callee}, {args});"));
                found.push(format!("{lead}Call{n}({callee}, {args});"));
            }
        }
    } else if let Some((wrapper, rest)) = statement.split_once('(') {
        let flipped = if let Some(n) = wrapper.strip_prefix("Call") {
            Some(format!("Value{n}"))
        } else {
            wrapper.strip_prefix("Value").map(|n| format!("Call{n}"))
        };
        if let Some(flipped) = flipped {
            if let Some((callee, args)) = rest.split_once(", ") {
                found.push(format!("{lead}{callee}({args});"));
                found.push(format!("{lead}{flipped}({callee}, {args});"));
            }
        }
    } else if let Some((target, call)) = statement.split_once(" = Func_") {
        if let Some((callee, args)) = call.split_once('(') {
            let n = args.split(", ").filter(|a| !a.is_empty()).count();
            if (1..=4).contains(&n) {
                found.push(format!("{lead}{target} = Value{n}(Func_{callee}, {args});"));
            }
        }
    }
    found
}

/// Tries every alternative in order, keeping improvements. Returns the best
/// unit text and its score.
pub fn tune(
    root: &Path,
    scratch: &Path,
    owner: &str,
    span: u32,
    unit: &str,
    mut report: impl FnMut(&str),
) -> Result<(String, Score), String> {
    let write_and_score = |text: &str| -> Result<Score, String> {
        std::fs::write(scratch, text).map_err(|error| format!("{}: {error}", scratch.display()))?;
        score(root, scratch, owner, span)
    };
    let mut best = write_and_score(unit)?;
    let mut lines: Vec<String> = unit.lines().map(str::to_string).collect();
    report(&format!("base {}", best.differing));
    // Record accesses are volatile by default; a record whose stores the
    // reference schedules freely is plain, so each base is tried plain.
    for base in volatile_bases(&lines) {
        let stripped = strip_volatile(&lines, &base);
        let trial = write_and_score(&format!("{}\n", stripped.join("\n")));
        if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
            let trial = trial.unwrap();
            report(&format!(
                "{} -> {}  plain {base}",
                best.differing, trial.differing
            ));
            best = trial;
            lines = stripped;
        }
    }
    let mut index = 0;
    while index < lines.len() && best.differing > 0 {
        for alternative in alternatives(&lines[index]) {
            let saved = std::mem::replace(&mut lines[index], alternative.clone());
            let trial = write_and_score(&format!("{}\n", lines.join("\n")))?;
            if trial.differing < best.differing {
                report(&format!(
                    "{} -> {}  {}",
                    best.differing,
                    trial.differing,
                    alternative.trim()
                ));
                best = trial;
                if best.differing == 0 {
                    break;
                }
            } else {
                lines[index] = saved;
            }
        }
        index += 1;
    }
    let text = format!("{}\n", lines.join("\n"));
    std::fs::write(scratch, &text).map_err(|error| format!("{}: {error}", scratch.display()))?;
    Ok((text, best))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn respells_calls_both_ways() {
        assert_eq!(
            alternatives("    Func_02001234(22, 0);"),
            vec![
                "    Value2(Func_02001234, 22, 0);",
                "    Call2(Func_02001234, 22, 0);"
            ]
        );
        assert_eq!(
            alternatives("    Call3(Func_02001234, 1, 2, 3);"),
            vec![
                "    Func_02001234(1, 2, 3);",
                "    Value3(Func_02001234, 1, 2, 3);"
            ]
        );
        assert!(alternatives("    record[3] = 1;").is_empty());
    }
}

/// Call statements near each mismatch: for every differing hunk, the callee
/// of the next `bl` in the reference column and how many calls of that
/// callee precede it, so the matching unit line can be found.
/// Report offsets map to callees through `bias`, derived by `report_bias`.
pub fn mismatched_calls(report: &str, bias: u32) -> Vec<(u32, usize)> {
    let mut seen: std::collections::BTreeMap<u32, usize> = std::collections::BTreeMap::new();
    let mut pending = false;
    let mut found = Vec::new();
    for line in report.lines().skip(9) {
        let differs = line.starts_with("  -") || line.starts_with("  +") || line.starts_with("  !");
        if differs {
            pending = true;
        }
        let reference = line
            .rfind("  ")
            .map(|at| line[at + 2..].trim())
            .unwrap_or("");
        let Some(rest) = reference.strip_prefix("bl\t0x") else {
            continue;
        };
        let Ok(offset) = u32::from_str_radix(rest.trim(), 16) else {
            continue;
        };
        let callee = bias.wrapping_add(offset);
        let count = seen.entry(callee).or_insert(0);
        if pending {
            found.push((callee, *count));
            pending = false;
        }
        *count += 1;
    }
    found.dedup();
    found
}

/// The difference between the first call in the unit and the first `bl`
/// offset in the report's reference column.
pub fn report_bias(report: &str, lines: &[String]) -> Option<u32> {
    let first_callee = lines.iter().find_map(|line| {
        let trimmed = line.trim_start();
        if trimmed.starts_with("void ")
            || trimmed.starts_with("s32 ")
            || trimmed.starts_with("u8 *")
            || trimmed.starts_with("#")
        {
            return None;
        }
        let at = trimmed.find("Func_")?;
        let hex = trimmed.get(at + 5..at + 13)?;
        let after = trimmed.get(at + 13..)?;
        (after.starts_with('(') || after.starts_with(','))
            .then(|| u32::from_str_radix(hex, 16).ok())
            .flatten()
    })?;
    let first_offset = report.lines().skip(9).find_map(|line| {
        let reference = line
            .rfind("  ")
            .map(|at| line[at + 2..].trim())
            .unwrap_or("");
        u32::from_str_radix(reference.strip_prefix("bl\t0x")?, 16).ok()
    })?;
    Some(first_callee.wrapping_sub(first_offset))
}

/// The unit line holding the `ordinal`-th call of `callee`.
pub fn call_line(lines: &[String], callee: u32, ordinal: usize) -> Option<usize> {
    let symbol = format!("Func_{callee:08x}");
    let mut count = 0;
    for (index, line) in lines.iter().enumerate() {
        let trimmed = line.trim_start();
        if trimmed.starts_with("void ")
            || trimmed.starts_with("s32 ")
            || trimmed.starts_with("u8 *")
        {
            continue;
        }
        let mut rest = line.as_str();
        while let Some(at) = rest.find(&symbol) {
            let after = &rest[at + symbol.len()..];
            if after.starts_with('(') || after.starts_with(',') {
                if count == ordinal {
                    return Some(index);
                }
                count += 1;
            }
            rest = &rest[at + symbol.len()..];
        }
    }
    None
}

/// A zero-argument call right after another call may take that call's
/// result through r0: the nested spelling keeps the arity the scheduler saw.
fn pass_through(lines: &[String], index: usize) -> Option<(usize, String)> {
    let line = &lines[index];
    let trimmed = line.trim_start();
    let lead = &line[..line.len() - trimmed.len()];
    let callee = trimmed.strip_suffix("();")?;
    if !callee.starts_with("Func_") {
        return None;
    }
    let previous = lines.get(index.checked_sub(1)?)?;
    let statement = previous.trim().strip_suffix(';')?;
    let is_call = statement.starts_with("Func_")
        || statement.starts_with("Call")
        || statement.starts_with("Value");
    if !is_call || statement.contains(" = ") {
        return None;
    }
    Some((index - 1, format!("{lead}{callee}({statement});")))
}

/// Targeted trials: respell only the calls the alignment report marks.
pub fn tune_targeted(
    root: &Path,
    scratch: &Path,
    owner: &str,
    span: u32,
    unit: &str,
    mut report: impl FnMut(&str),
) -> Result<(String, Score), String> {
    let write_and_score = |text: &str| -> Result<Score, String> {
        std::fs::write(scratch, text).map_err(|error| format!("{}: {error}", scratch.display()))?;
        score(root, scratch, owner, span)
    };
    let mut best = write_and_score(unit)?;
    let mut lines: Vec<String> = unit.lines().map(str::to_string).collect();
    report(&format!("base {}", best.differing));
    // Record accesses are volatile by default; a record whose stores the
    // reference schedules freely is plain, so each base is tried plain.
    for base in volatile_bases(&lines) {
        let stripped = strip_volatile(&lines, &base);
        if stripped == lines {
            continue;
        }
        let trial = write_and_score(&format!("{}\n", stripped.join("\n")));
        if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
            let trial = trial.unwrap();
            report(&format!(
                "{} -> {}  plain {base}",
                best.differing, trial.differing
            ));
            best = trial;
            lines = stripped;
        }
    }
    // Two volatile stores in a row are output-dependent whatever their
    // offsets (alias analysis never separates volatile accesses), which
    // ranks the second below an independent instruction after the first
    // is issued. A run of adjacent stores is tried plain as a unit, which
    // lets the scheduler place them back to back as the reference may.
    let mut k = 0;
    while k < lines.len() {
        let run_end = (k..lines.len())
            .take_while(|&j| is_volatile_store(&lines[j]))
            .last()
            .map(|j| j + 1);
        let Some(run_end) = run_end else {
            k += 1;
            continue;
        };
        if run_end - k >= 2 {
            let saved: Vec<String> = lines[k..run_end].to_vec();
            for line in &mut lines[k..run_end] {
                *line = line.replace("*(volatile ", "*(");
            }
            let trial = write_and_score(&format!("{}\n", lines.join("\n")));
            if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
                let trial = trial.unwrap();
                report(&format!(
                    "{} -> {}  plain run {}",
                    best.differing,
                    trial.differing,
                    lines[k].trim()
                ));
                best = trial;
            } else {
                lines.splice(k..run_end, saved);
            }
        }
        k = run_end;
    }
    // A callee declared as returning a value makes its call set r0 even
    // where the result is unused: the next write of r0 then depends on the
    // call rather than on an earlier argument load, whose dependent count
    // drops, so a store that tied with it issues first. Each void callee
    // is tried value-returning.
    let mut k = 0;
    while k < lines.len() {
        if let Some(rest) = lines[k].strip_prefix("void Func_") {
            if rest.len() == 11 && rest.ends_with("();") {
                let saved = lines[k].clone();
                lines[k] = format!("s32 Func_{rest}");
                let trial = write_and_score(&format!("{}\n", lines.join("\n")));
                if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
                    let trial = trial.unwrap();
                    report(&format!(
                        "{} -> {}  {}",
                        best.differing,
                        trial.differing,
                        lines[k].trim()
                    ));
                    best = trial;
                } else {
                    lines[k] = saved;
                }
            }
        }
        k += 1;
    }
    // A halfword store through an int local: the store address hoisted into
    // a pointer before the local shortens the local's live range, which
    // changes which pseudo the allocator places first. Each block is tried
    // both ways and the better one stays.
    let mut k = 0;
    while k + 3 < lines.len() {
        if let Some(hoisted) = hoisted_shown_block(&lines, k) {
            let saved: Vec<String> = lines[k..k + 4].to_vec();
            lines.splice(k..k + 4, hoisted.iter().cloned());
            let trial = write_and_score(&format!("{}\n", lines.join("\n")));
            if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
                let trial = trial.unwrap();
                report(&format!(
                    "{} -> {}  hoisted {}",
                    best.differing,
                    trial.differing,
                    hoisted[0].trim()
                ));
                best = trial;
                k += hoisted.len();
                continue;
            }
            lines.splice(k..k + hoisted.len(), saved.iter().cloned());
        }
        k += 1;
    }
    let mut tried: std::collections::BTreeSet<(u32, usize)> = std::collections::BTreeSet::new();
    let mut rounds = 0;
    while best.differing > 0 && rounds < 12 {
        rounds += 1;
        let Some(bias) = report_bias(&best.report, &lines) else {
            report("no call to derive the report bias from");
            break;
        };
        let targets: Vec<(u32, usize)> = mismatched_calls(&best.report, bias)
            .into_iter()
            .filter(|t| !tried.contains(t))
            .collect();
        report(&format!(
            "targets {}",
            targets
                .iter()
                .map(|(c, k)| format!("{c:08x}#{k}"))
                .collect::<Vec<_>>()
                .join(" ")
        ));
        if targets.is_empty() {
            break;
        }
        let mut improved = false;
        for target in targets {
            tried.insert(target);
            let Some(index) = call_line(&lines, target.0, target.1) else {
                report(&format!("no line for {:08x}#{}", target.0, target.1));
                continue;
            };
            // Also the call before it: its own order depends on this arity.
            let mut sites = vec![index];
            if let Some(previous) = (0..index)
                .rev()
                .find(|k| !alternatives(&lines[*k]).is_empty())
            {
                sites.push(previous);
            }
            for site in sites {
                let mut options: Vec<Vec<(usize, String)>> = alternatives(&lines[site])
                    .into_iter()
                    .map(|a| vec![(site, a)])
                    .collect();
                if let Some((remove, merged)) = pass_through(&lines, site) {
                    options.push(vec![(site, merged), (remove, String::new())]);
                }
                for option in options {
                    let saved: Vec<(usize, String)> = option
                        .iter()
                        .map(|(k, _)| (*k, lines[*k].clone()))
                        .collect();
                    for (k, text) in &option {
                        lines[*k] = text.clone();
                    }
                    let text = lines
                        .iter()
                        .filter(|l| !l.is_empty() || true)
                        .cloned()
                        .collect::<Vec<_>>()
                        .join("\n");
                    let trial = write_and_score(&format!("{text}\n"));
                    if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
                        let trial = trial.unwrap();
                        report(&format!(
                            "{} -> {}  {}",
                            best.differing,
                            trial.differing,
                            option[0].1.trim()
                        ));
                        best = trial;
                        improved = true;
                        break;
                    }
                    for (k, text) in saved {
                        lines[k] = text;
                    }
                }
                if best.differing == 0 {
                    break;
                }
            }
            if best.differing == 0 {
                break;
            }
        }
        if !improved {
            break;
        }
    }
    // A statement after an if/else that consumes the value each branch
    // assigned may have been written in both branches by the original: the
    // compiler cross-jumps identical tails into one, where the joined
    // variable compiles to a hoisted load. Each such join is tried sunk.
    let mut k = 0;
    while k + 4 < lines.len() {
        if let Some(sunk) = sink_into_branches(&lines, k) {
            let trial = write_and_score(&format!("{}\n", sunk.join("\n")));
            if trial.as_ref().is_ok_and(|t| t.differing < best.differing) {
                let trial = trial.unwrap();
                report(&format!(
                    "{} -> {}  sunk {}",
                    best.differing,
                    trial.differing,
                    lines[k].trim()
                ));
                best = trial;
                lines = sunk;
            }
        }
        k += 1;
    }
    // Statement order: two adjacent independent statements swapped change
    // the scheduler's tie-breaks (it keeps source order among equals), so a
    // swap that scores better is kept. Three passes bound the search.
    let mut passes = 0;
    while best.differing > 0 && passes < 3 {
        passes += 1;
        let mut improved = false;
        let mut k = 0;
        while k + 1 < lines.len() {
            if swappable(&lines[k], &lines[k + 1]) {
                lines.swap(k, k + 1);
                let trial = write_and_score(&format!("{}\n", lines.join("\n")));
                match trial {
                    Ok(t) if t.differing < best.differing => {
                        report(&format!(
                            "{} -> {}  swap {}",
                            best.differing,
                            t.differing,
                            lines[k].trim()
                        ));
                        best = t;
                        improved = true;
                    }
                    _ => lines.swap(k, k + 1),
                }
            }
            k += 1;
        }
        if !improved {
            break;
        }
    }
    let text = format!("{}\n", lines.join("\n"));
    std::fs::write(scratch, &text).map_err(|error| format!("{}: {error}", scratch.display()))?;
    Ok((text, best))
}

/// A statement storing through a volatile access, `*(volatile s32 *)(rec7 + 8) = ...;`.
fn is_volatile_store(line: &str) -> bool {
    let trimmed = line.trim_start();
    trimmed.starts_with("*(volatile ")
        && trimmed.split_once(" = ").is_some_and(|(lhs, _)| {
            !lhs.contains("(volatile ") || lhs.matches("*(volatile ").count() == 1
        })
}

/// The lines with the statement after the if/else at `k` sunk into both
/// branches, when each branch ends by assigning the same local and the
/// statement reads it once: `if (c) { v = A; } else { v = B; } H(v);`
/// becomes `if (c) { v = A; H(A); } else { v = B; H(B); }`. The
/// assignments stay when the local is read anywhere else, and go otherwise.
fn sink_into_branches(lines: &[String], k: usize) -> Option<Vec<String>> {
    let head = &lines[k];
    let indent = &head[..head.len() - head.trim_start().len()];
    if !head.trim_start().starts_with("if (") || !head.trim_end().ends_with('{') {
        return None;
    }
    let at_level =
        |line: &String| line.starts_with(indent) && !line[indent.len()..].starts_with(' ');
    // The `} else {` and the closing `}` at this indent.
    let else_at = (k + 1..lines.len()).find(|&j| at_level(&lines[j]))?;
    if lines[else_at].trim() != "} else {" {
        return None;
    }
    let close_at = (else_at + 1..lines.len()).find(|&j| at_level(&lines[j]))?;
    if lines[close_at].trim() != "}" || close_at + 1 >= lines.len() {
        return None;
    }
    let (then_last, else_last) = (&lines[else_at - 1], &lines[close_at - 1]);
    let local = assigned(then_last)?;
    if assigned(else_last)? != local {
        return None;
    }
    let then_value = then_last
        .trim()
        .strip_suffix(';')?
        .split_once(" = ")?
        .1
        .to_string();
    let else_value = else_last
        .trim()
        .strip_suffix(';')?
        .split_once(" = ")?
        .1
        .to_string();
    let statement = &lines[close_at + 1];
    let trimmed = statement.trim();
    if !statement.starts_with(indent)
        || statement[indent.len()..].starts_with(' ')
        || !trimmed.ends_with(';')
        || CONTROL.iter().any(|w| trimmed.starts_with(w))
        || trimmed.matches(local).count() != 1
        || !mentions(statement, local)
    {
        return None;
    }
    let read_elsewhere = lines.iter().enumerate().any(|(j, line)| {
        j != else_at - 1 && j != close_at - 1 && j != close_at + 1 && mentions(line, local)
    });
    let substitute = |value: &str| statement.replace(local, value);
    let mut out: Vec<String> = Vec::with_capacity(lines.len() + 1);
    for (j, line) in lines.iter().enumerate() {
        if j == close_at + 1 {
            continue;
        }
        if (j == else_at - 1 || j == close_at - 1) && !read_elsewhere {
            // The assignment goes; its consumer takes the value directly.
        } else {
            out.push(line.clone());
        }
        if j == else_at - 1 {
            out.push(substitute(&then_value));
        } else if j == close_at - 1 {
            out.push(substitute(&else_value));
        }
    }
    Some(out)
}

const CONTROL: [&str; 9] = [
    "if ", "while ", "do", "for ", "switch ", "case ", "return", "goto ", "break",
];

/// The hoisted form of the `shown` block starting at `k`:
/// `{ s32 shown = K; <blank> *(u16 *)(ADDR) = shown; }` becomes
/// `{ u16 *target = (u16 *)(ADDR); s32 shown = K; <blank> *target = shown; }`.
/// None when the lines are not such a block or the address is a bare name.
fn hoisted_shown_block(lines: &[String], k: usize) -> Option<Vec<String>> {
    let open = &lines[k];
    let declaration = &lines[k + 1];
    let store = &lines[k + 3];
    if open.trim() != "{" || !lines[k + 2].trim().is_empty() {
        return None;
    }
    let lead = &declaration[..declaration.len() - declaration.trim_start().len()];
    let constant = declaration
        .trim()
        .strip_prefix("s32 shown = ")?
        .strip_suffix(';')?;
    let lhs = store.trim().strip_suffix(" = shown;")?;
    let address = lhs.strip_prefix("*(u16 *)(")?.strip_suffix(')')?;
    if !address.contains(" + ") && !address.contains("*(") {
        return None;
    }
    Some(vec![
        open.clone(),
        format!("{lead}u16 *target = (u16 *)({address});"),
        format!("{lead}s32 shown = {constant};"),
        String::new(),
        format!("{lead}*target = shown;"),
    ])
}

/// The local a simple statement assigns, `v3` in `v3 = expr;`.
fn assigned(line: &str) -> Option<&str> {
    let trimmed = line.trim();
    let (lhs, _) = trimmed.strip_suffix(';')?.split_once(" = ")?;
    let plain = lhs.chars().all(|c| c.is_ascii_alphanumeric() || c == '_');
    plain.then_some(lhs)
}

/// Whether `line` mentions the identifier `name` as a whole word.
fn mentions(line: &str, name: &str) -> bool {
    let boundary = |c: char| c.is_ascii_alphanumeric() || c == '_';
    let mut rest = line;
    while let Some(at) = rest.find(name) {
        let before = rest[..at].chars().last().is_some_and(boundary);
        let after = rest[at + name.len()..].chars().next().is_some_and(boundary);
        if !before && !after {
            return true;
        }
        rest = &rest[at + name.len()..];
    }
    false
}

/// Two adjacent statements the original could have written in either
/// order: simple statements at one indent, no control flow, no shared
/// local, and at most one of them touching memory or calling.
fn swappable(a: &str, b: &str) -> bool {
    let indent = |s: &str| s.len() - s.trim_start().len();
    if indent(a) != indent(b) || a.trim().is_empty() || b.trim().is_empty() {
        return false;
    }
    let simple = |s: &str| {
        let t = s.trim();
        t.ends_with(';')
            && !t.contains('{')
            && !t.contains('}')
            && !t.ends_with(":;")
            && ![
                "if ", "while ", "do", "for ", "switch ", "case ", "return", "goto ", "break",
                "continue", "s32 ", "u8 ", "u16 ", "u32 ",
            ]
            .iter()
            .any(|k| t.starts_with(k))
    };
    if !simple(a) || !simple(b) {
        return false;
    }
    // Two loads may swap; a call or a store keeps its order against any
    // other memory access or call.
    let calls = |s: &str| s.contains("Func_") || s.contains("Call") || s.contains("Value");
    let stores = |s: &str| {
        s.trim()
            .split_once(" = ")
            .is_some_and(|(lhs, _)| lhs.contains("*(") || lhs.contains('['))
    };
    let touches = |s: &str| s.contains("*(") || s.contains('[') || calls(s);
    let heavy = |s: &str| calls(s) || stores(s);
    if (heavy(a) && touches(b)) || (heavy(b) && touches(a)) {
        return false;
    }
    if let Some(x) = assigned(a) {
        if mentions(b, x) {
            return false;
        }
    }
    if let Some(y) = assigned(b) {
        if mentions(a, y) {
            return false;
        }
    }
    assigned(a).is_some() || assigned(b).is_some()
}

/// The base names of volatile accesses, `record` and `rec7` and `p5`, in
/// order of first appearance; `all` stands for every access at once.
pub fn volatile_bases(lines: &[String]) -> Vec<String> {
    let mut bases = vec!["all".to_string()];
    for line in lines {
        let mut rest = line.as_str();
        while let Some(at) = rest.find("*(volatile ") {
            let tail = &rest[at..];
            let Some(close) = tail.find(" *)") else { break };
            let after = &tail[close + 3..];
            let base: String = after
                .trim_start_matches('(')
                .chars()
                .take_while(|c| c.is_ascii_alphanumeric() || *c == '_')
                .collect();
            if !base.is_empty() && !base.starts_with('0') && !bases.contains(&base) {
                bases.push(base);
            }
            rest = &rest[at + 11..];
        }
    }
    bases
}

/// The lines with volatile removed from every access through `base`.
pub fn strip_volatile(lines: &[String], base: &str) -> Vec<String> {
    lines
        .iter()
        .map(|line| {
            if base == "all" {
                return line.replace("*(volatile ", "*(");
            }
            let mut out = String::new();
            let mut rest = line.as_str();
            while let Some(at) = rest.find("*(volatile ") {
                let tail = &rest[at..];
                let Some(close) = tail.find(" *)") else { break };
                let after = &tail[close + 3..];
                let found: String = after
                    .trim_start_matches('(')
                    .chars()
                    .take_while(|c| c.is_ascii_alphanumeric() || *c == '_')
                    .collect();
                out.push_str(&rest[..at]);
                if found == base {
                    out.push_str("*(");
                } else {
                    out.push_str("*(volatile ");
                }
                rest = &rest[at + 11..];
            }
            out.push_str(rest);
            out
        })
        .collect()
}
