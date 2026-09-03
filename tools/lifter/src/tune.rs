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
    let text = format!("{}\n", lines.join("\n"));
    std::fs::write(scratch, &text).map_err(|error| format!("{}: {error}", scratch.display()))?;
    Ok((text, best))
}
