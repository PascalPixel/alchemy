use std::collections::{BTreeMap, BTreeSet};
use std::path::Path;

const USAGE: &str = r#"usage: alchemy merge-driver scaffold|lines BASE OURS THEIRS
       alchemy merge-driver links
scaffold and lines are git merge drivers (see .gitattributes; make bootstrap
and make worktree register them): they merge BASE, OURS and THEIRS and
write the result to OURS. links makes every edition's linker script name
exactly the sections its rom.s defines, after a merge."#;

struct Scaffold {
    header: Vec<String>,
    ranges: Vec<(u32, u32)>,
    pre: BTreeSet<(u32, String)>,
    post: BTreeSet<(u32, String)>,
    sections: BTreeMap<u32, String>,
}

fn incbin(line: &str) -> Option<(u32, u32)> {
    let rest = line.strip_prefix("\t.incbin \"baserom.gba\", 0x")?;
    let (offset, size) = rest.split_once(", 0x")?;
    Some((
        u32::from_str_radix(offset, 16).ok()?,
        u32::from_str_radix(size.trim(), 16).ok()?,
    ))
}

fn section(line: &str) -> Option<&str> {
    line.strip_prefix("\t.section .rom.")?.strip_suffix(", \"ax\"")
}

fn parse(text: &str) -> Scaffold {
    let mut s = Scaffold {
        header: Vec::new(),
        ranges: Vec::new(),
        pre: BTreeSet::new(),
        post: BTreeSet::new(),
        sections: BTreeMap::new(),
    };
    let (mut pending, mut started, mut next, mut last_end) = (Vec::<String>::new(), false, None, 0u32);
    for line in text.lines() {
        if let Some(name) = section(line) {
            started = true;
            next = Some(name.to_string());
            for l in tag(std::mem::take(&mut pending)) {
                s.post.insert((last_end, l));
            }
            continue;
        }
        if !started {
            if !line.is_empty() {
                s.header.push(line.to_string());
            }
            continue;
        }
        if let Some((offset, size)) = incbin(line) {
            if let Some(name) = next.take() {
                s.sections.insert(offset, name);
            }
            s.ranges.push((offset, offset + size));
            last_end = offset + size;
            for l in tag(std::mem::take(&mut pending)) {
                s.pre.insert((offset, l));
            }
        } else if line.starts_with("\t.size ") {
            s.post.insert((last_end, format!("\u{1}{line}")));
        } else if !line.is_empty() {
            pending.push(line.to_string());
        }
    }
    s
}

fn tag(lines: Vec<String>) -> Vec<String> {
    let mut owner = String::new();
    let mut out: Vec<String> = lines
        .into_iter()
        .rev()
        .map(|line| {
            if line.ends_with(':') && !line.starts_with(['\t', ' ', '@']) {
                owner = line.trim_end_matches(':').to_string();
            }
            format!("{owner}\u{1}{line}")
        })
        .collect();
    out.reverse();
    out
}

fn emit(out: &mut Vec<String>, lines: Vec<&String>) {
    let rank = |l: &str| {
        if l.starts_with('@') { 0 } else if l.starts_with("\t.balign") { 1 } else if l.starts_with("\t.global") { 2 }
        else if l.starts_with("\t.type") { 3 } else if l.starts_with("\t.thumb_func") { 4 } else if l.ends_with(':') { 9 } else { 5 }
    };
    let mut groups: BTreeMap<&str, Vec<&str>> = BTreeMap::new();
    for tagged in &lines {
        let (owner, line) = tagged.split_once('\u{1}').unwrap_or(("", tagged));
        groups.entry(owner).or_default().push(line);
    }
    for (_, mut group) in groups {
        group.sort_by_key(|l| rank(l));
        out.extend(group.into_iter().map(str::to_string));
    }
}

fn intersect(mut a: Vec<(u32, u32)>, mut b: Vec<(u32, u32)>) -> Vec<(u32, u32)> {
    a.sort();
    b.sort();
    let (mut out, mut i, mut j) = (Vec::<(u32, u32)>::new(), 0, 0);
    while i < a.len() && j < b.len() {
        let (lo, hi) = (a[i].0.max(b[j].0), a[i].1.min(b[j].1));
        if lo < hi {
            match out.last_mut() {
                Some(last) if last.1 == lo => last.1 = hi,
                _ => out.push((lo, hi)),
            }
        }
        if a[i].1 < b[j].1 {
            i += 1;
        } else {
            j += 1;
        }
    }
    out
}

fn union(a: Vec<(u32, u32)>, b: Vec<(u32, u32)>) -> Vec<(u32, u32)> {
    let mut all: Vec<(u32, u32)> = a.into_iter().chain(b).filter(|r| r.0 < r.1).collect();
    all.sort();
    let mut out: Vec<(u32, u32)> = Vec::new();
    for (lo, hi) in all {
        match out.last_mut() {
            Some(last) if lo <= last.1 => last.1 = last.1.max(hi),
            _ => out.push((lo, hi)),
        }
    }
    out
}

fn subtract(a: Vec<(u32, u32)>, b: Vec<(u32, u32)>) -> Vec<(u32, u32)> {
    let b = union(b, Vec::new());
    let mut out = Vec::new();
    for (mut lo, hi) in union(a, Vec::new()) {
        for &(blo, bhi) in &b {
            if bhi <= lo || blo >= hi {
                continue;
            }
            if blo > lo {
                out.push((lo, blo));
            }
            lo = lo.max(bhi);
        }
        if lo < hi {
            out.push((lo, hi));
        }
    }
    out
}

fn three_way<T: Ord + Clone>(base: &BTreeSet<T>, ours: &BTreeSet<T>, theirs: &BTreeSet<T>) -> BTreeSet<T> {
    ours.union(theirs)
        .filter(|k| (ours.contains(k) && theirs.contains(k)) || !base.contains(k))
        .cloned()
        .collect()
}

pub fn merge_scaffold(base: &str, ours: &str, theirs: &str) -> String {
    let (b, o, t) = (parse(base), parse(ours), parse(theirs));
    let cover = union(
        intersect(o.ranges.clone(), t.ranges.clone()),
        union(subtract(o.ranges.clone(), b.ranges.clone()), subtract(t.ranges.clone(), b.ranges.clone())),
    );
    let inside = |at: u32| cover.iter().any(|&(lo, hi)| at >= lo && at < hi);
    let pre = three_way(&b.pre, &o.pre, &t.pre);
    let post = three_way(&b.post, &o.post, &t.post);
    let pairs = |s: &Scaffold| s.sections.iter().map(|(&k, v)| (k, v.clone())).collect::<BTreeSet<_>>();
    let mut sections = BTreeMap::new();
    for (at, name) in three_way(&pairs(&b), &pairs(&o), &pairs(&t)) {
        if inside(at) {
            sections.entry(at).or_insert(name);
        }
    }
    let mut out = o.header.clone();
    for &(lo, hi) in &cover {
        let mut cuts: BTreeSet<u32> = sections.keys().chain(pre.iter().map(|(at, _)| at)).copied().filter(|&c| c > lo && c < hi).collect();
        cuts.insert(lo);
        let cuts: Vec<u32> = cuts.into_iter().collect();
        for (k, &at) in cuts.iter().enumerate() {
            let end = cuts.get(k + 1).copied().unwrap_or(hi);
            if k == 0 || sections.contains_key(&at) {
                let name = sections.get(&at).cloned().unwrap_or_else(|| format!("{at:08x}"));
                out.push(format!("\t.section .rom.{name}, \"ax\""));
            }
            let lines: Vec<&String> = pre.iter().filter(|(o, _)| *o == at).map(|(_, l)| l).collect();
            emit(&mut out, lines);
            out.push(format!("\t.incbin \"baserom.gba\", 0x{at:08x}, 0x{:08x}", end - at));
            emit(&mut out, post.iter().filter(|(o, _)| *o == end).map(|(_, l)| l).collect());
        }
    }
    out.join("\n") + "\n"
}

pub fn merge_lines(base: &str, ours: &str, theirs: &str) -> String {
    let base: Vec<&str> = base.lines().collect();
    let align = |side: &str| {
        let side: Vec<&str> = side.lines().collect();
        let (n, m) = (base.len(), side.len());
        let mut table = vec![vec![0u32; m + 1]; n + 1];
        for i in (0..n).rev() {
            for j in (0..m).rev() {
                table[i][j] = if base[i] == side[j] { table[i + 1][j + 1] + 1 } else { table[i + 1][j].max(table[i][j + 1]) };
            }
        }
        let (mut kept, mut inserted) = (vec![false; n], vec![Vec::<String>::new(); n + 1]);
        let (mut i, mut j) = (0, 0);
        while i < n || j < m {
            if i < n && j < m && base[i] == side[j] {
                kept[i] = true;
                i += 1;
                j += 1;
            } else if j < m && (i == n || table[i][j + 1] >= table[i + 1][j]) {
                inserted[i].push(side[j].to_string());
                j += 1;
            } else {
                i += 1;
            }
        }
        (kept, inserted)
    };
    let ((ok, oi), (tk, ti)) = (align(ours), align(theirs));
    let mut out = Vec::new();
    for i in 0..=base.len() {
        out.extend(oi[i].iter().cloned());
        if ti[i] != oi[i] {
            out.extend(ti[i].iter().cloned());
        }
        if i < base.len() && ok[i] && tk[i] {
            out.push(base[i].to_string());
        }
    }
    out.join("\n") + "\n"
}

pub fn reconcile_links(scaffold: &str, script: &str) -> (String, usize, Vec<String>) {
    let names: Vec<String> = scaffold.lines().filter_map(section).map(str::to_string).collect();
    let present: BTreeSet<&str> = names.iter().map(String::as_str).collect();
    let reference = |line: &str| -> Option<String> {
        let at = line.find("(.rom.")? + 6;
        let end = line[at..].find(')')? + at;
        Some(line[at..end].to_string())
    };
    let mut lines: Vec<String> = script.lines().map(str::to_string).collect();
    let before = lines.len();
    lines.retain(|l| reference(l).is_none_or(|r| present.contains(r.as_str())));
    let dropped = before - lines.len();
    let mut unplaced = Vec::new();
    for (k, name) in names.iter().enumerate() {
        if lines.iter().any(|l| reference(l).as_deref() == Some(name)) {
            continue;
        }
        let previous = k.checked_sub(1).map(|p| &names[p]);
        match previous.and_then(|p| lines.iter().position(|l| reference(l).as_deref() == Some(p))) {
            Some(at) => {
                let line = lines[at].replace(&format!("(.rom.{})", names[k - 1]), &format!("(.rom.{name})"));
                lines.insert(at + 1, line);
            }
            None => unplaced.push(name.clone()),
        }
    }
    (lines.join("\n") + "\n", dropped, unplaced)
}

fn read(path: &str) -> Result<String, String> {
    std::fs::read_to_string(path).map_err(|e| format!("{path}: {e}"))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    match arguments {
        [kind, base, ours, theirs] if kind == "scaffold" || kind == "lines" => {
            let (b, o, t) = (read(base)?, read(ours)?, read(theirs)?);
            let merged = if kind == "scaffold" { merge_scaffold(&b, &o, &t) } else { merge_lines(&b, &o, &t) };
            std::fs::write(ours, merged).map_err(|e| format!("{ours}: {e}"))
        }
        [kind] if kind == "links" => {
            let root = crate::compiler::routing::root();
            for game in ["tbs", "tla"] {
                for lang in ["ja", "de", "es", "fr", "it"] {
                    let rom = root.join(format!("recon/{game}/{lang}/rom.s"));
                    let script = root.join(crate::targets::edition_script_path(game, lang));
                    if !Path::new(&rom).exists() {
                        continue;
                    }
                    let (scaffold, text) = (read(&rom.to_string_lossy())?, read(&script.to_string_lossy())?);
                    let (fixed, dropped, unplaced) = reconcile_links(&scaffold, &text);
                    if fixed != text {
                        std::fs::write(&script, &fixed).map_err(|e| e.to_string())?;
                    }
                    if dropped > 0 || !unplaced.is_empty() || fixed != text {
                        println!("{game}-{lang}: dropped {dropped}, placed {}", if unplaced.is_empty() { "all".into() } else { format!("all but {}", unplaced.join(", ")) });
                    }
                }
            }
            Ok(())
        }
        _ => Err(USAGE.into()),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    const BASE: &str = "@ header\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000010\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000010\n";

    #[test]
    fn adoptions_on_both_sides_both_leave_the_scaffold() {
        let ours = "@ header\n\t.section .rom.00000110, \"ax\"\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000010\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000010\n";
        let theirs = "@ header\n\t.section .rom.00000100, \"ax\"\n\t.global A\nA:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000010\n";
        let merged = merge_scaffold(BASE, ours, theirs);
        assert_eq!(merged, "@ header\n\t.section .rom.00000110, \"ax\"\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000010\n");
        let script = "  \"*/rom.o\"(.rom.00000100)\n  \"*/A.o\"(.text)\n";
        let (fixed, dropped, unplaced) = reconcile_links(&merged, script);
        assert_eq!(dropped, 1);
        assert_eq!(unplaced, ["00000110"]);
        assert_eq!(fixed, "  \"*/A.o\"(.text)\n");
    }

    #[test]
    fn bytes_one_side_adds_back_survive() {
        let ours = "@ header\n\t.section .rom.00000110, \"ax\"\n\t.global B\nB:\n\t.incbin \"baserom.gba\", 0x00000110, 0x00000010\n\t.global C\nC:\n\t.incbin \"baserom.gba\", 0x00000120, 0x00000010\n";
        let theirs = format!("{BASE}\t.global D\nD:\n\t.incbin \"baserom.gba\", 0x00000130, 0x00000008\n");
        let merged = merge_scaffold(BASE, ours, &theirs);
        assert!(merged.contains("D:\n\t.incbin \"baserom.gba\", 0x00000130, 0x00000008"), "{merged}");
        assert!(!merged.contains("A:"), "{merged}");
    }

    #[test]
    fn two_labels_at_one_address_keep_their_own_thumb_marks() {
        let base = "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global F\n\t.thumb_func\nF:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n";
        let ours = "@ h\n\t.section .rom.00000100, \"ax\"\n\t.global F\n\t.thumb_func\nF:\n\t.global E\n\t.thumb_func\nE:\n\t.incbin \"baserom.gba\", 0x00000100, 0x00000010\n";
        let merged = merge_scaffold(base, ours, base);
        assert_eq!(merged.matches(".thumb_func\nE:").count() + merged.matches(".thumb_func\nF:").count(), 2, "{merged}");
    }

    #[test]
    fn line_merge_keeps_both_insertions_and_both_deletions() {
        let base = "a\nb\nc\nd\n";
        let ours = "a\nX\nc\nd\n";
        let theirs = "a\nb\nc\nY\n";
        assert_eq!(merge_lines(base, ours, theirs), "a\nX\nc\nY\n");
    }
}
