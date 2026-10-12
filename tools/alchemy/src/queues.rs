use crate::compiler::routing::root;
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;

const USAGE: &str = r#"usage: alchemy queues --workers N --out-dir DIR [--game tbs|tla] [--main | --overlays]
Reads out/reports/deps-<game>-en-functions.tsv (run make deps first) and the
English linker order, groups the not-yet-C functions by module run or
overlay, drops functions any other local branch has already adopted, and
writes DIR/queue-1.tsv ... queue-N.tsv balanced by bytes, each listing
image, address, name, bytes, draft and group. The game defaults to tbs."#;

pub struct Function {
    pub image: String,
    pub address: String,
    pub name: String,
    pub bytes: u64,
    pub draft: String,
    pub listing: String,
    pub callers: u64,
}

fn listing_stem(source: &str) -> &str {
    let file = source.rsplit('/').next().unwrap_or(source);
    [".s", ".S", ".o"].iter().find_map(|suffix| file.strip_suffix(suffix)).unwrap_or(file)
}

pub(crate) fn runs(script: &str, game: &str) -> BTreeMap<String, usize> {
    let mut run = 0;
    let mut in_raw = false;
    let mut out = BTreeMap::new();
    let marker = format!("/recon/{game}/raw/");
    for line in script.lines() {
        if let Some(at) = line.find(&marker) {
            let rest = &line[at + marker.len()..];
            let address = rest.split('.').next().unwrap_or("").to_string();
            if !in_raw {
                run += 1;
                in_raw = true;
            }
            out.insert(address, run);
        } else if line.contains("/games/") {
            in_raw = false;
        }
    }
    out
}

pub(crate) fn balance(groups: BTreeMap<String, Vec<Function>>, workers: usize) -> Vec<Vec<Function>> {
    let mut groups: Vec<(String, Vec<Function>)> = groups.into_iter().collect();
    groups.sort_by_key(|(_, fs)| std::cmp::Reverse(fs.iter().map(|f| f.bytes).sum::<u64>()));
    let mut queues: Vec<(u64, Vec<Function>)> = (0..workers).map(|_| (0, Vec::new())).collect();
    for (_, group) in groups {
        let bytes: u64 = group.iter().map(|f| f.bytes).sum();
        let target = queues.iter_mut().min_by_key(|(b, _)| *b).unwrap();
        target.0 += bytes;
        target.1.extend(group);
    }
    queues.into_iter().map(|(_, q)| q).collect()
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let (mut workers, mut out_dir, mut game, mut part) = (0usize, None, "tbs", "all");
    let mut i = 0;
    while i < arguments.len() {
        let value = || arguments.get(i + 1).cloned().ok_or_else(|| USAGE.to_string());
        match arguments[i].as_str() {
            "--workers" => { workers = value()?.parse().map_err(|_| USAGE.to_string())?; i += 1; }
            "--out-dir" => { out_dir = Some(value()?); i += 1; }
            "--game" => { game = if value()? == "tla" { "tla" } else { "tbs" }; i += 1; }
            "--main" => part = "main",
            "--overlays" => part = "overlays",
            _ => return Err(USAGE.into()),
        }
        i += 1;
    }
    let out_dir = out_dir.ok_or_else(|| USAGE.to_string())?;
    if workers == 0 {
        return Err(USAGE.into());
    }
    let root = root();
    let report = root.join(format!("out/reports/deps-{game}-en-functions.tsv"));
    let text = std::fs::read_to_string(&report).map_err(|e| format!("{}: {e} (run make deps)", report.display()))?;
    let mut lines = text.lines();
    let header: Vec<&str> = lines.next().unwrap_or("").split('\t').collect();
    let column = |name: &str| header.iter().position(|h| *h == name).ok_or(format!("{name} missing from {}", report.display()));
    let (ci, ca, cn, co, cb, cc, cd, cs) = (column("image")?, column("address")?, column("name")?, column("origin")?, column("bytes")?, column("callers")?, column("draft")?, column("source")?);
    // The English link order: the edition's script, or for a game with an
    // object list its rows, on this branch and on each other.
    let script_path = crate::object_list::main_lines_path(game, "en").to_string_lossy().into_owned();
    let script = crate::object_list::main_lines(root, game, "en")?;
    let run_of = runs(&script, game);

    let current = crate::process::run(&["git", "branch", "--show-current"], root)?.trim().to_string();
    let branches = crate::process::run(&["git", "branch", "--format=%(refname:short)"], root)?;
    let mut taken = BTreeSet::new();
    for branch in branches.lines().filter(|b| !b.is_empty() && *b != current) {
        let Ok(theirs) = crate::process::run(&["git", "show", &format!("{branch}:{script_path}")], root) else { continue };
        let Ok(theirs) = crate::object_list::lines_of(root, &theirs, game, "en") else { continue };
        let theirs = runs(&theirs, game);
        taken.extend(run_of.keys().filter(|a| !theirs.contains_key(*a)).cloned());
    }

    let english = format!("{game}-en");
    let mut groups: BTreeMap<String, Vec<Function>> = BTreeMap::new();
    for line in lines {
        let c: Vec<&str> = line.split('\t').collect();
        if c.get(co) != Some(&"not-yet-c") || c.get(cb).and_then(|b| b.parse::<u64>().ok()).unwrap_or(0) <= 2 {
            continue;
        }
        let main = c[ci] == english;
        if (part == "main" && !main) || (part == "overlays" && main) {
            continue;
        }
        let source = c[cs].to_string();
        let listing = listing_stem(&source).to_string();
        if main && taken.contains(&listing) {
            continue;
        }
        let group = if main {
            run_of.get(&listing).map(|r| format!("run-{r:04}")).unwrap_or_else(|| format!("listing-{listing}"))
        } else {
            c[ci].to_string()
        };
        groups.entry(group).or_default().push(Function {
            image: c[ci].into(),
            address: c[ca].into(),
            name: c[cn].into(),
            bytes: c[cb].parse().unwrap_or(0),
            draft: c[cd].into(),
            listing,
            callers: c[cc].parse().unwrap_or(0),
        });
    }
    let total: usize = groups.values().map(Vec::len).sum();
    let queues = balance(groups, workers);
    std::fs::create_dir_all(&out_dir).map_err(|e| e.to_string())?;
    for (k, mut queue) in queues.into_iter().enumerate() {
        queue.sort_by(|a, b| b.callers.cmp(&a.callers).then(a.address.cmp(&b.address)));
        let mut tsv = String::from("image\taddress\tname\tbytes\tdraft\tlisting\n");
        let bytes: u64 = queue.iter().map(|f| f.bytes).sum();
        for f in &queue {
            let _ = writeln!(tsv, "{}\t{}\t{}\t{}\t{}\t{}", f.image, f.address, f.name, f.bytes, f.draft, f.listing);
        }
        let path = format!("{out_dir}/queue-{}.tsv", k + 1);
        std::fs::write(&path, tsv).map_err(|e| e.to_string())?;
        println!("{path}: {} functions, {bytes} bytes", queue.len());
    }
    println!("{total} functions in {workers} queues; {} listings already adopted on other branches left out", taken.len());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn runs_break_at_c_objects() {
        let script = "\"*/recon/tbs/raw/08000100.o\"(.text)\n\"*/recon/tbs/raw/08000200.o\"(.text)\n\"*/games/x/a.o\"(.text)\n\"*/recon/tbs/raw/08000300.o\"(.text)\n";
        let r = runs(script, "tbs");
        assert_eq!(r["08000100"], r["08000200"]);
        assert_ne!(r["08000200"], r["08000300"]);
    }

    #[test]
    fn arm_listings_use_the_same_module_run_and_adoption_key() {
        let script = "\"*/recon/tbs/raw/080155d0.o\"(.text)\n\"*/recon/tbs/raw/080158e8.o\"(.text)\n";
        let run_of = runs(script, "tbs");
        let arm = listing_stem("recon/tbs/raw/080155d0.s");
        let next = listing_stem("recon/tbs/raw/080158e8.s");
        assert_eq!(arm, "080155d0");
        assert_eq!(run_of[arm], run_of[next]);
        let adopted = BTreeSet::from(["080155d0".to_string()]);
        assert!(adopted.contains(arm));
        assert_eq!(listing_stem("recon/tbs/raw/08000100.s"), "08000100");
        assert_eq!(listing_stem("resource_370_overlay.o"), "resource_370_overlay");
    }

    #[test]
    fn groups_stay_whole_and_queues_balance() {
        let f = |a: &str, b| Function { image: "x".into(), address: a.into(), name: a.into(), bytes: b, draft: String::new(), listing: a.into(), callers: 0 };
        let mut groups = BTreeMap::new();
        groups.insert("g1".to_string(), vec![f("1", 100), f("2", 100)]);
        groups.insert("g2".to_string(), vec![f("3", 150)]);
        groups.insert("g3".to_string(), vec![f("4", 50)]);
        let queues = balance(groups, 2);
        let sizes: Vec<u64> = queues.iter().map(|q| q.iter().map(|f| f.bytes).sum()).collect();
        assert_eq!(sizes.iter().sum::<u64>(), 400);
        assert!(queues.iter().any(|q| q.iter().any(|f| f.address == "1") && q.iter().any(|f| f.address == "2")));
    }
}
