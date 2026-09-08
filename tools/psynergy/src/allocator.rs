//! Portable rendering of GCC RTL/local/global allocation dumps.
use std::collections::BTreeMap;
use std::fmt::Write;
use std::fs;
use std::path::Path;

const REGS: [&str; 16] = [
    "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "sl", "fp", "ip", "sp", "lr", "pc",
];

fn dump(work: &Path, suffix: &str) -> Result<String, String> {
    let mut matches = fs::read_dir(work)
        .map_err(|e| format!("{}: {e}", work.display()))?
        .filter_map(Result::ok)
        .map(|e| e.path())
        .filter(|p| p.extension().is_some_and(|x| x == suffix))
        .collect::<Vec<_>>();
    matches.sort();
    match matches.as_slice() {
        [path] => fs::read_to_string(path).map_err(|e| format!("{}: {e}", path.display())),
        [] => Err(format!("missing GCC .{suffix} dump in {}", work.display())),
        _ => Err(format!(
            "ambiguous GCC .{suffix} dumps in {}",
            work.display()
        )),
    }
}

pub fn inspect(work: &Path) -> Result<String, String> {
    // Pseudo -> source variable, from RTL `(reg/v:SI NN [ name ])` annotations.
    let rtl = dump(work, "rtl")?;
    let mut names: BTreeMap<u32, String> = BTreeMap::new();
    let mut creation: Vec<u32> = Vec::new();
    let mut rest = rtl.as_str();
    while let Some(at) = rest.find("(reg") {
        rest = &rest[at + 4..];
        let Some(space) = rest.find(' ') else { break };
        let after = &rest[space + 1..];
        let digits: String = after.chars().take_while(|c| c.is_ascii_digit()).collect();
        let Ok(pseudo) = digits.parse::<u32>() else {
            continue;
        };
        if pseudo < 32 {
            continue;
        }
        if !creation.contains(&pseudo) {
            creation.push(pseudo);
        }
        let tail = &after[digits.len()..];
        if let Some(name) = tail.strip_prefix(" [ ").and_then(|t| t.split(" ]").next()) {
            names
                .entry(pseudo)
                .or_insert_with(|| name.trim().to_string());
        }
    }

    // Local alloc: class costs and preferences.
    let lreg = dump(work, "lreg")?;
    let mut costs: BTreeMap<u32, String> = BTreeMap::new();
    let mut prefs: BTreeMap<u32, String> = BTreeMap::new();
    for line in lreg.lines() {
        let Some(rest) = line.strip_prefix("Register ") else {
            continue;
        };
        let digits: String = rest.chars().take_while(|c| c.is_ascii_digit()).collect();
        let Ok(pseudo) = digits.parse::<u32>() else {
            continue;
        };
        if let Some(list) = rest.split("costs: ").nth(1) {
            let interesting: Vec<&str> = list
                .split(' ')
                .filter(|c| {
                    ["LO_REGS:", "HI_REGS:", "MEM:", "GENERAL_REGS:"]
                        .iter()
                        .any(|p| c.starts_with(p))
                })
                .collect();
            costs.insert(pseudo, interesting.join(" "));
        }
        if let Some(pref) = rest.split("pref ").nth(1) {
            prefs.insert(pseudo, pref.trim_end_matches('.').trim().to_string());
        }
    }

    // Global alloc: ordering, conflicts, dispositions, spills, reloads.
    let greg = dump(work, "greg")?;
    let mut order: Vec<u32> = Vec::new();
    let mut conflicts: BTreeMap<u32, String> = BTreeMap::new();
    let mut assigned: BTreeMap<u32, u32> = BTreeMap::new();
    let mut spills: Vec<String> = Vec::new();
    let mut reloads: Vec<String> = Vec::new();
    let mut dispositions = false;
    for text in [&lreg, &greg] {
        for line in text.lines() {
            if let Some(list) = line.split("regs to allocate: ").nth(1) {
                order = list
                    .split_whitespace()
                    .filter_map(|n| n.parse().ok())
                    .collect();
            }
            if let Some(rest) = line.strip_prefix(";; ") {
                if let Some((pseudo, list)) = rest.split_once(" conflicts: ") {
                    if let Ok(p) = pseudo.parse::<u32>() {
                        conflicts.insert(p, list.trim().to_string());
                    }
                }
                if let Some(rest) = rest.strip_prefix("Register ") {
                    if let Some((p, h)) = rest.split_once(" in ") {
                        if let (Ok(p), Ok(h)) = (
                            p.trim().parse::<u32>(),
                            h.trim().trim_end_matches('.').parse::<u32>(),
                        ) {
                            assigned.insert(p, h);
                        }
                    }
                }
            }
            if line.contains("Register dispositions:") {
                dispositions = true;
                continue;
            }
            if dispositions {
                if line.trim().is_empty() || line.starts_with(";;") || line.starts_with('(') {
                    dispositions = false;
                } else {
                    let words: Vec<&str> = line.split_whitespace().collect();
                    let mut k = 0;
                    while k + 2 < words.len() {
                        if words[k + 1] == "in" {
                            if let (Ok(p), Ok(h)) =
                                (words[k].parse::<u32>(), words[k + 2].parse::<u32>())
                            {
                                assigned.insert(p, h);
                            }
                            k += 3;
                        } else {
                            k += 1;
                        }
                    }
                }
            }
            if let Some(rest) = line.strip_prefix("Spilling for insn ") {
                spills.push(rest.trim_end_matches('.').to_string());
            }
            if let Some(rest) = line.strip_prefix("Using reg ") {
                if let Some((reg, reload)) = rest.split_once(" for reload ") {
                    reloads.push(format!("r{reg} for reload {}", reload.trim()));
                }
            }
        }
    }

    let mut out = String::new();
    let _ = writeln!(
        out,
        "{}: {} pseudos, global order: {}",
        work.display(),
        creation.len(),
        order
            .iter()
            .map(|n| n.to_string())
            .collect::<Vec<_>>()
            .join(" ")
    );
    let _ = writeln!(
        out,
        "pseudo  hard  var                     pref      order  costs"
    );
    for p in &creation {
        let hard = assigned
            .get(p)
            .map(|h| {
                REGS.get(*h as usize)
                    .map(|r| (*r).to_string())
                    .unwrap_or_else(|| format!("h{h}"))
            })
            .unwrap_or_else(|| "-".into());
        let pos = order
            .iter()
            .position(|o| o == p)
            .map(|i| i.to_string())
            .unwrap_or_else(|| "-".into());
        let _ = writeln!(
            out,
            "{:<7} {:<5} {:<23} {:<9} {:<6} {}{}",
            p,
            hard,
            names.get(p).cloned().unwrap_or_default(),
            prefs.get(p).cloned().unwrap_or_default(),
            pos,
            costs.get(p).cloned().unwrap_or_default(),
            conflicts
                .get(p)
                .map(|c| format!("  conflicts: {c}"))
                .unwrap_or_default()
        );
    }
    if !spills.is_empty() {
        let _ = writeln!(out, "spill insns: {}", spills.join(" "));
    }
    if !reloads.is_empty() {
        let _ = writeln!(out, "reloads: {}", reloads.join("; "));
    }
    let _ = writeln!(out, "dumps read from {}", work.display());
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;
    #[test]
    fn renders_synthetic_dumps() {
        let d = tempfile::tempdir().unwrap();
        fs::write(d.path().join("x.rtl"), "(reg/v:V4SI 32 [ foo ])").unwrap();
        fs::write(
            d.path().join("x.lreg"),
            "Register 32 costs: GENERAL_REGS: pref 3.",
        )
        .unwrap();
        fs::write(
            d.path().join("x.greg"),
            "regs to allocate: 32\n;; Register 32 in 3.",
        )
        .unwrap();
        let s = inspect(d.path()).unwrap();
        assert!(s.contains("1 pseudos") && s.contains("foo") && s.contains("r3"));
    }
    #[test]
    fn rejects_ambiguous_suffix() {
        let d = tempfile::tempdir().unwrap();
        for n in ["a.rtl", "b.rtl"] {
            fs::write(d.path().join(n), "").unwrap();
        }
        assert!(inspect(d.path()).unwrap_err().contains("ambiguous"));
    }
}
