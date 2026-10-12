//! One object in place of several in a linker script. Each line names one
//! object and its sections; a run of adjacent lines naming the merged
//! objects with the same sections becomes one line for the new object, in
//! the place of the first. The result must still place every section of the
//! new object once, so objects whose pieces lie apart in a script, or a
//! script that links some of them and not the others, are refused.

/// The object a script line names, and the rest of the line after it.
fn named(line: &str) -> Option<(&str, &str)> {
    let start = line.find("\"*/")? + 3;
    let end = start + line[start..].find(".o\"")?;
    Some((&line[start..end], &line[end + 3..]))
}

/// The section names a line's rest places, `(.text .rodata)` -> [.text, .rodata].
fn sections(rest: &str) -> Vec<&str> {
    let inner = rest.trim().trim_start_matches('(').split(')').next().unwrap_or("");
    inner.split_whitespace().collect()
}

/// `script` with `objects` (paths without `.o`, in the merged source's
/// order) replaced by `merged`. None when the script names none of them.
pub fn rewrite(script: &str, objects: &[String], merged: &str) -> Result<Option<String>, String> {
    let lines: Vec<&str> = script.split_inclusive('\n').collect();
    let hits: Vec<(usize, usize, &str)> = lines
        .iter()
        .enumerate()
        .filter_map(|(n, line)| {
            let (object, rest) = named(line)?;
            objects.iter().position(|o| o == object).map(|k| (n, k, rest))
        })
        .collect();
    if hits.is_empty() {
        return Ok(None);
    }
    let present: std::collections::BTreeSet<usize> = hits.iter().map(|h| h.1).collect();
    if present.len() != objects.len() {
        let missing: Vec<&str> = (0..objects.len()).filter(|k| !present.contains(k)).map(|k| objects[k].as_str()).collect();
        return Err(format!("links {} but not {}", objects[*present.iter().next().unwrap()], missing.join(", ")));
    }
    // Runs: consecutive hit lines (blank lines between allowed) with one rest.
    let mut runs: Vec<Vec<(usize, usize, &str)>> = Vec::new();
    for hit in hits {
        let joins = runs.last().and_then(|run| run.last()).is_some_and(|last| {
            last.2.trim() == hit.2.trim() && lines[last.0 + 1..hit.0].iter().all(|l| l.trim().is_empty())
        });
        if joins {
            runs.last_mut().unwrap().push(hit);
        } else {
            runs.push(vec![hit]);
        }
    }
    let mut placed: Vec<&str> = Vec::new();
    let mut redundant: Vec<bool> = Vec::new();
    for run in &runs {
        // Uninitialized data has no bytes in the image and its order is the
        // compiler's; the full build judges where it lands.
        let uninitialized = sections(run[0].2).iter().all(|s| matches!(*s, ".bss" | "COMMON" | ".sbss"));
        // A later line of uninitialized sections the merged object already
        // places is already said.
        let said = uninitialized && sections(run[0].2).iter().all(|s| placed.contains(s));
        redundant.push(said);
        if said {
            continue;
        }
        if !uninitialized && run.windows(2).any(|w| w[0].1 >= w[1].1) {
            return Err(format!("names {} out of the merged order", run.iter().map(|h| objects[h.1].as_str()).collect::<Vec<_>>().join(", ")));
        }
        for section in sections(run[0].2) {
            if placed.contains(&section) {
                return Err(format!("places {section} of the merged objects in two places"));
            }
            placed.push(section);
        }
    }
    let mut out = String::with_capacity(script.len());
    let mut skip: std::collections::BTreeSet<usize> = std::collections::BTreeSet::new();
    let mut replace: std::collections::BTreeMap<usize, String> = std::collections::BTreeMap::new();
    for (run, said) in runs.iter().zip(&redundant) {
        if *said {
            skip.extend(run.iter().map(|h| h.0));
            continue;
        }
        let first = run[0].0;
        let line = lines[first];
        let (object, _) = named(line).expect("a hit names an object");
        let at = line.find(object).expect("object in line");
        replace.insert(first, format!("{}{merged}{}", &line[..at], &line[at + object.len()..]));
        for hit in &run[1..] {
            for n in run[0].0 + 1..=hit.0 {
                skip.insert(n);
            }
        }
    }
    for (n, line) in lines.iter().enumerate() {
        if skip.contains(&n) {
            continue;
        }
        match replace.get(&n) {
            Some(text) => out.push_str(text),
            None => out.push_str(line),
        }
    }
    Ok(Some(out))
}

/// The forced rewrite of an experiment: every line naming one of `objects`
/// names `merged` instead, and a later line whose sections the merged
/// object already places goes. It keeps no promise about order or about
/// editions that linked only some of the objects; the build judges.
pub fn rewrite_forced(script: &str, objects: &[String], merged: &str) -> Option<String> {
    let mut out = String::with_capacity(script.len());
    let mut placed: Vec<String> = Vec::new();
    let mut hit = false;
    for line in script.split_inclusive('\n') {
        let Some((object, rest)) = named(line).filter(|(object, _)| objects.iter().any(|o| o == object)) else {
            out.push_str(line);
            continue;
        };
        hit = true;
        let mut wanted: Vec<String> = sections(rest).iter().map(|s| s.to_string()).collect();
        if wanted.is_empty() {
            wanted.push(String::new());
        }
        if wanted.iter().all(|s| placed.contains(s)) {
            continue;
        }
        placed.extend(wanted);
        let at = line.find(object).expect("object in line");
        out.push_str(&format!("{}{merged}{}", &line[..at], &line[at + object.len()..]));
    }
    hit.then_some(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn a_forced_rewrite_names_the_merged_object_once_per_section() {
        let apart = "\"*/g/A.o\"(.text)\n\"*/g/X.o\"(.text)\n\"*/g/B.o\"(.text)\n\"*/g/B.o\"(.rodata)\n";
        assert_eq!(rewrite_forced(apart, &objects(&["g/A", "g/B"]), "g/M").unwrap(), "\"*/g/M.o\"(.text)\n\"*/g/X.o\"(.text)\n\"*/g/M.o\"(.rodata)\n");
        assert_eq!(rewrite_forced(apart, &objects(&["g/Q"]), "g/M"), None);
    }

    fn objects(list: &[&str]) -> Vec<String> {
        list.iter().map(|s| s.to_string()).collect()
    }

    #[test]
    fn adjacent_lines_become_one_per_section_list() {
        let script = "{\n    \"*/g/X.o\"(.text)\n    \"*/g/A.o\"(.text)\n    \"*/g/B.o\"(.text)\n    \"*/g/Y.o\"(.text)\n    \"*/g/A.o\"(.rodata)\n    \"*/g/B.o\"(.rodata)\n}\n";
        let out = rewrite(script, &objects(&["g/A", "g/B"]), "g/M").unwrap().unwrap();
        assert_eq!(out, "{\n    \"*/g/X.o\"(.text)\n    \"*/g/M.o\"(.text)\n    \"*/g/Y.o\"(.text)\n    \"*/g/M.o\"(.rodata)\n}\n");
        assert_eq!(rewrite(script, &objects(&["g/Q"]), "g/M").unwrap(), None);
    }

    #[test]
    fn pieces_apart_or_missing_or_reordered_are_refused() {
        let apart = "\"*/g/A.o\"(.text)\n\"*/g/X.o\"(.text)\n\"*/g/B.o\"(.text)\n";
        assert!(rewrite(apart, &objects(&["g/A", "g/B"]), "g/M").unwrap_err().contains("two places"));
        let missing = "\"*/g/A.o\"(.text)\n";
        assert!(rewrite(missing, &objects(&["g/A", "g/B"]), "g/M").unwrap_err().contains("not g/B"));
        let reordered = "\"*/g/B.o\"(.text)\n\"*/g/A.o\"(.text)\n";
        assert!(rewrite(reordered, &objects(&["g/A", "g/B"]), "g/M").unwrap_err().contains("order"));
        let bss = "\"*/g/A.o\"(.text)\n\"*/g/B.o\"(.text)\n\"*/g/B.o\"(.bss COMMON)\n\"*/g/A.o\"(.bss COMMON)\n";
        assert_eq!(rewrite(bss, &objects(&["g/A", "g/B"]), "g/M").unwrap().unwrap(), "\"*/g/M.o\"(.text)\n\"*/g/M.o\"(.bss COMMON)\n");
        let commons = "\"*/g/A.o\"(.data .bss COMMON)\n\"*/g/C.o\"(COMMON)\n\"*/g/B.o\"(COMMON)\n";
        assert_eq!(rewrite(commons, &objects(&["g/A", "g/B", "g/C"]), "g/M").unwrap().unwrap(), "\"*/g/M.o\"(.data .bss COMMON)\n");
        // A data object between two code objects of the unit is another section list.
        let data = "\"*/g/A.o\"(.text)\n\"*/g/D.o\"(.data)\n\"*/g/B.o\"(.text)\n";
        assert!(rewrite(data, &objects(&["g/A", "g/B"]), "g/M").is_err());
    }
}
