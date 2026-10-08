//! `alchemy unit`: applying the recovered translation units to the tree.
//!
//!   unit candidates  every run of today's C files the analysis joins, with
//!                    the evidence at its edges (TSV)
//!   unit flatten     one such run merged into one file in ROM order: the
//!                    merged source, every linker script and the routing
//!                    tables updated, the superseded files removed
//!
//! A run is a chain of adjacent C objects of one image where every gap
//! between two of them is judged or forced a join; it never crosses
//! another language, compiler, a COMMON/game divide or data placed between.

use super::cmerge::{self, Source};
use super::dryrun;
use super::load::{self, Build, Loaded};
use super::model::{self, Gap, Image, Lang, Unit, Verdict};
use crate::compiler::routing::{family_for_source, CompilerFamily};
use crate::targets::{decomp_target, DecompTarget};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy unit candidates [--build EDITION] [--out-dir DIR]
       alchemy unit flatten (IMAGE:UNIT | --objects PATH.C,PATH.C...) [--build EDITION]
                            [--name NAME.C] [--static] [--dry-run]
candidates writes DIR/units-EDITION-merges.tsv (default out/reports): each run
of today's C files the analysis joins, with the units it covers, the
evidence at its two edges and its weakest join. An edge is forced (another
language or compiler, data, a region, the image's end), strong (code shared
with other images ends there) or weak (only the scored valley, names or
folders say so); apply is yes when neither edge is weak.
flatten merges the runs of one proposed unit (IMAGE:UNIT as in the units
TSV; main is the main image), or the named sources, into one C file in ROM
order, in the folder holding most of their functions and named after its
main module file (the largest 8.3-named one), else after the folder;
--name chooses. Every linker script under games/ and recon/ that names the
objects, and the compiler routing tables, follow; the old files and local
headers only they include are removed. Every edition that links the
objects is compiled and compared first, from today's builds; --dry-run
stops there. --static also makes functions no other file names static,
kept only when the bytes stay identical. Build every edition first.";

struct Options {
    build: String,
    out_dir: PathBuf,
    unit: Option<String>,
    objects: Option<Vec<String>>,
    name: Option<String>,
    dry_run: bool,
    make_static: bool,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options { build: "tbs-en".into(), out_dir: PathBuf::from("out/reports"), unit: None, objects: None, name: None, dry_run: false, make_static: false };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        match flag {
            "--dry-run" => options.dry_run = true,
            "--static" => options.make_static = true,
            "--build" | "--out-dir" | "--objects" | "--name" => {
                let value = arguments.get(i + 1).ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?.clone();
                match flag {
                    "--build" => options.build = value,
                    "--out-dir" => options.out_dir = PathBuf::from(value),
                    "--objects" => options.objects = Some(value.split(',').map(|s| s.trim().to_string()).filter(|s| !s.is_empty()).collect()),
                    _ => options.name = Some(value),
                }
                i += 1;
            }
            _ if !flag.starts_with('-') && options.unit.is_none() => options.unit = Some(flag.to_string()),
            _ => return Err(format!("unknown argument {flag}\n{USAGE}")),
        }
        i += 1;
    }
    Ok(options)
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let Some((command, rest)) = arguments.split_first() else {
        return Err(USAGE.into());
    };
    if matches!(command.as_str(), "-h" | "--help") || rest.iter().any(|a| a == "-h" || a == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(rest)?;
    match command.as_str() {
        "candidates" => candidates(&options),
        "flatten" => flatten(&options),
        _ => Err(USAGE.into()),
    }
}

/// Today's object path without `.o`, relative to the repository.
fn stem(object: &str) -> &str {
    object.trim_end_matches(".o")
}

fn source_path(root: &Path, object: &str) -> Option<String> {
    let path = format!("{}.C", stem(object));
    root.join(&path).is_file().then_some(path)
}

fn judged(root: &Path, edition: &str) -> Result<(DecompTarget, Vec<super::Judged>), String> {
    let target = decomp_target(Some(edition))?;
    let build = Build { root: root.to_path_buf(), dir: root.join(target.output_dir), name: edition.to_string(), compiler: target.compiler };
    let judged = load::load(&build)?
        .into_iter()
        .map(|loaded| {
            let gaps = model::judge(&loaded.image);
            let units = model::group(&loaded.image, &gaps, true);
            let binary_units = model::group(&loaded.image, &gaps, false);
            super::Judged { loaded, gaps, units, binary_units }
        })
        .collect();
    Ok((target, judged))
}

#[derive(Clone, Debug, PartialEq)]
enum Edge {
    End,
    Forced(String),
    Strong(String),
    Weak(String),
}

impl Edge {
    fn label(&self) -> String {
        match self {
            Edge::End => "end".into(),
            Edge::Forced(r) => format!("forced:{r}"),
            Edge::Strong(r) => format!("strong:{r}"),
            Edge::Weak(r) => format!("weak:{r}"),
        }
    }
}

fn edge(gap: Option<&Gap>) -> Edge {
    let Some(g) = gap else {
        return Edge::End;
    };
    match g.verdict {
        Verdict::ForcedBoundary | Verdict::Conflict => Edge::Forced(g.forced_boundary.join(" ").chars().take(60).collect()),
        _ if g.signals.iter().any(|s| s.starts_with("shared-edge")) => Edge::Strong(format!("p={:.2} shared-edge", g.probability)),
        _ => Edge::Weak(format!("p={:.2} {}", g.probability, g.signals.iter().filter(|s| !s.starts_with("pair") && !s.starts_with("cohesion")).cloned().collect::<Vec<_>>().join(" "))),
    }
}

/// A run of joined C objects in one image.
struct Chain {
    image: String,
    objects: Vec<String>,
    first: usize,
    last: usize,
    units: Vec<usize>,
    left: Edge,
    right: Edge,
    weakest: f64,
}

fn family(root: &Path, target: DecompTarget, source: &str) -> CompilerFamily {
    let _ = root;
    family_for_source(target.compiler, source)
}

/// The images that link each object: code linked into several overlays joins
/// only code linked into exactly the same ones.
fn linked_in(judged: &[super::Judged]) -> BTreeMap<String, BTreeSet<String>> {
    let mut out: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for j in judged {
        for f in &j.loaded.image.functions {
            out.entry(f.object.clone()).or_default().insert(j.loaded.image.name.clone());
        }
    }
    out
}

fn chains(root: &Path, target: DecompTarget, image: &Image, gaps: &[Gap], units: &[Unit], linked: &BTreeMap<String, BTreeSet<String>>) -> Vec<Chain> {
    // Today's objects as runs of functions.
    let mut runs: Vec<(usize, usize)> = Vec::new();
    for (i, f) in image.functions.iter().enumerate() {
        match runs.last_mut() {
            Some(run) if image.functions[run.0].object == f.object => run.1 = i,
            _ => runs.push((i, i)),
        }
    }
    let mergeable = |run: &(usize, usize)| -> Option<(CompilerFamily, bool, BTreeSet<String>)> {
        let f = &image.functions[run.0];
        if !matches!(f.lang, Lang::C | Lang::Library) || !f.object.starts_with("games/") {
            return None;
        }
        let source = source_path(root, &f.object)?;
        Some((family(root, target, &source), f.object.starts_with("games/COMMON/"), linked.get(&f.object).cloned().unwrap_or_default()))
    };
    let unit_of = |i: usize| units.iter().position(|u| u.first <= i && i <= u.last).unwrap_or(0);
    let mut out = Vec::new();
    let mut k = 0;
    while k < runs.len() {
        let Some(kind) = mergeable(&runs[k]) else {
            k += 1;
            continue;
        };
        let mut objects = vec![image.functions[runs[k].0].object.clone()];
        let mut end = k;
        let mut weakest: f64 = 1.0;
        while end + 1 < runs.len() {
            let next = &runs[end + 1];
            let gap = &gaps[runs[end].1];
            let object = &image.functions[next.0].object;
            if mergeable(next).as_ref() != Some(&kind) || gap.verdict.is_boundary() || objects.contains(object) {
                break;
            }
            weakest = weakest.min(1.0 - gap.probability);
            objects.push(object.clone());
            end += 1;
        }
        if objects.len() >= 2 {
            let (first, last) = (runs[k].0, runs[end].1);
            let (u0, u1) = (unit_of(first), unit_of(last));
            let left = edge(units[u0].first.checked_sub(1).map(|g| &gaps[g]));
            let right = edge((units[u1].last + 1 < image.functions.len()).then(|| &gaps[units[u1].last]));
            out.push(Chain { image: image.name.clone(), objects, first, last, units: (u0..=u1).collect(), left, right, weakest });
        }
        k = end + 1;
    }
    out
}

fn candidates(options: &Options) -> Result<(), String> {
    let root = crate::compiler::routing::root().to_path_buf();
    let (target, judged) = judged(&root, &options.build)?;
    let mut out = String::from("image\tunits\tobjects\tfunctions\tleft\tright\tweakest_join\tapply\tsources\n");
    let mut seen: BTreeSet<Vec<String>> = BTreeSet::new();
    let linked = linked_in(&judged);
    let (mut yes, mut no) = (0, 0);
    for j in &judged {
        for c in chains(&root, target, &j.loaded.image, &j.gaps, &j.units, &linked) {
            if !seen.insert(c.objects.clone()) {
                continue;
            }
            let apply = !matches!(c.left, Edge::Weak(_)) && !matches!(c.right, Edge::Weak(_));
            if apply {
                yes += 1;
            } else {
                no += 1;
            }
            let units: Vec<String> = c.units.iter().map(|u| format!("U{:04}", u + 1)).collect();
            let _ = writeln!(
                out,
                "{}\t{}\t{}\t{}\t{}\t{}\t{:.3}\t{}\t{}",
                c.image,
                units.join(","),
                c.objects.len(),
                c.last - c.first + 1,
                c.left.label(),
                c.right.label(),
                c.weakest,
                if apply { "yes" } else { "no" },
                c.objects.iter().map(|o| format!("{}.C", stem(o))).collect::<Vec<_>>().join(",")
            );
        }
    }
    let out_dir = root.join(&options.out_dir);
    fs::create_dir_all(&out_dir).map_err(|e| format!("{}: {e}", out_dir.display()))?;
    let path = out_dir.join(format!("units-{}-merges.tsv", options.build));
    fs::write(&path, out).map_err(|e| format!("{}: {e}", path.display()))?;
    println!("{yes} runs to apply, {no} with a weak edge: {}", path.strip_prefix(&root).unwrap_or(&path).display());
    Ok(())
}

/// The editions that build a source: all six of its game, and both games'
/// for COMMON.
fn editions(source: &str) -> Vec<String> {
    let games: &[&str] = if source.starts_with("games/COMMON/") {
        &["tbs", "tla"]
    } else if source.starts_with("games/THE LOST AGE/") {
        &["tla"]
    } else {
        &["tbs"]
    };
    games.iter().flat_map(|g| ["ja", "en", "de", "es", "fr", "it"].iter().map(move |l| format!("{g}-{l}"))).collect()
}

fn linker_scripts(root: &Path) -> Result<Vec<PathBuf>, String> {
    let listed = crate::process::run(&["git", "ls-files", "--", "*.LD"], root)?;
    Ok(listed.lines().filter(|l| l.starts_with("games/") || l.starts_with("recon/")).map(|l| root.join(l)).collect())
}

fn read(path: &Path) -> Result<String, String> {
    fs::read(path).map(|b| String::from_utf8_lossy(&b).into_owned()).map_err(|e| format!("{}: {e}", path.display()))
}

/// The sources to merge, in ROM order, from a unit or a list.
fn group(root: &Path, options: &Options, judged: &[super::Judged], target: DecompTarget) -> Result<Vec<Vec<String>>, String> {
    if let Some(list) = &options.objects {
        let mut out = Vec::new();
        for path in list {
            let path = path.trim_start_matches("./");
            if !root.join(path).is_file() || !path.ends_with(".C") {
                return Err(format!("{path}: not a C source of this tree"));
            }
            out.push(format!("{}.o", path.trim_end_matches(".C")));
        }
        // Given in ROM order, as the candidates list them; today's build must link each.
        for o in &out {
            if !judged.iter().any(|j| j.loaded.image.functions.iter().any(|f| f.object == *o)) {
                return Err(format!("{o}: today's build links no function from it"));
            }
        }
        return Ok(vec![out]);
    }
    let name = options.unit.as_deref().ok_or(USAGE)?;
    let (image, unit) = name.split_once(':').ok_or_else(|| format!("{name}: name a unit as IMAGE:UNIT, main:U0042"))?;
    let j = judged
        .iter()
        .enumerate()
        .find(|(i, j)| j.loaded.image.name == image || (image == "main" && *i == 0))
        .map(|(_, j)| j)
        .ok_or_else(|| format!("{image}: no such image"))?;
    let k = unit.trim_start_matches('U').parse::<usize>().map_err(|_| format!("{unit}: not a unit id"))?;
    if k == 0 || k > j.units.len() {
        return Err(format!("{name}: no such unit"));
    }
    let found: Vec<Vec<String>> = chains(root, target, &j.loaded.image, &j.gaps, &j.units, &linked_in(judged)).into_iter().filter(|c| c.units.contains(&(k - 1))).map(|c| c.objects).collect();
    if found.is_empty() {
        return Err(format!("{name}: no run of two or more C files to merge"));
    }
    Ok(found)
}

fn is_eight_three(stem: &str) -> bool {
    !stem.is_empty() && stem.len() <= 8 && stem.chars().all(|c| c.is_ascii_uppercase() || c.is_ascii_digit() || c == '_')
}

/// Where the merged file goes and what it is called.
fn destination(root: &Path, objects: &[String], judged: &[super::Judged], name: Option<&str>) -> Result<String, String> {
    let mut count: BTreeMap<String, usize> = BTreeMap::new();
    let mut bytes: BTreeMap<String, u32> = BTreeMap::new();
    for j in judged {
        for f in &j.loaded.image.functions {
            if objects.contains(&f.object) {
                let folder = Path::new(stem(&f.object)).parent().unwrap_or(Path::new("")).to_string_lossy().into_owned();
                *count.entry(folder).or_default() += 1;
                *bytes.entry(f.object.clone()).or_default() += f.end - f.start;
            }
        }
    }
    // Most functions; ties go to the earliest object's folder.
    let order: Vec<String> = objects.iter().map(|o| Path::new(stem(o)).parent().unwrap_or(Path::new("")).to_string_lossy().into_owned()).collect();
    let folder = order.iter().max_by_key(|f| (count.get(*f).copied().unwrap_or(0), std::cmp::Reverse(order.iter().position(|x| x == *f)))).cloned().unwrap_or_default();
    let file = match name {
        Some(n) => n.trim_end_matches(".C").to_string(),
        None => {
            let module = objects
                .iter()
                .filter(|o| Path::new(stem(o)).parent().is_some_and(|p| p.to_string_lossy() == folder))
                .filter(|o| is_eight_three(&Path::new(stem(o)).file_name().unwrap_or_default().to_string_lossy()))
                .max_by_key(|o| (bytes.get(*o).copied().unwrap_or(0), std::cmp::Reverse(objects.iter().position(|x| x == *o))));
            match module {
                Some(o) => Path::new(stem(o)).file_name().unwrap_or_default().to_string_lossy().into_owned(),
                None => {
                    let leaf = Path::new(&folder).file_name().unwrap_or_default().to_string_lossy().into_owned();
                    leaf.chars().take(8).collect::<String>().trim_end_matches('_').to_string()
                }
            }
        }
    };
    if !is_eight_three(&file) {
        return Err(format!("{file}: not an uppercase 8.3 name; pass --name"));
    }
    let path = format!("{folder}/{file}.C");
    let theirs = objects.iter().any(|o| format!("{}.C", stem(o)) == path);
    if !theirs {
        for extension in ["C", "S", "s", "c", "MID", "H"] {
            let other = format!("{folder}/{file}.{extension}");
            if root.join(&other).exists() {
                return Err(format!("{other} already exists; pass --name"));
            }
        }
    }
    Ok(path)
}

/// Headers beside the sources that only the merged files include, expanded in
/// place; others stay as includes.
fn expand_local(root: &Path, source: &str, text: &str, group: &BTreeSet<String>, expanded: &mut BTreeSet<String>, depth: usize) -> Result<String, String> {
    let directory = Path::new(source).parent().unwrap_or(Path::new(""));
    let mut out = String::new();
    for line in text.split_inclusive('\n') {
        let trimmed = line.trim();
        let local = trimmed.strip_prefix("#include").map(str::trim).and_then(|r| r.strip_prefix('"')).and_then(|r| r.split('"').next());
        if let Some(name) = local {
            let header = directory.join(name).to_string_lossy().into_owned();
            if root.join(&header).is_file() && depth < 8 {
                let includers = includers_of(root, &header)?;
                if includers.iter().all(|i| group.contains(i) || expanded.contains(i)) {
                    let mut body = read(&root.join(&header))?;
                    body = strip_guard(&body);
                    expanded.insert(header.clone());
                    out.push_str(&expand_local(root, &header, &body, group, expanded, depth + 1)?);
                    if !out.ends_with('\n') {
                        out.push('\n');
                    }
                    continue;
                }
            }
        }
        out.push_str(line);
    }
    Ok(out)
}

fn includers_of(root: &Path, header: &str) -> Result<Vec<String>, String> {
    let path = Path::new(header);
    let name = path.file_name().unwrap_or_default().to_string_lossy().into_owned();
    let directory = path.parent().unwrap_or(Path::new("")).to_string_lossy().into_owned();
    let found = crate::process::run(&["git", "grep", "-l", "-F", &format!("\"{name}\""), "--", &format!("{directory}/")], root).unwrap_or_default();
    Ok(found.lines().filter(|l| Path::new(l).parent().is_some_and(|p| p.to_string_lossy() == directory)).map(str::to_string).collect())
}

fn strip_guard(text: &str) -> String {
    let lines: Vec<&str> = text.lines().collect();
    let code: Vec<usize> = (0..lines.len()).filter(|&i| !lines[i].trim().is_empty() && !lines[i].trim_start().starts_with("//")).collect();
    if code.len() >= 3 {
        let (a, b, z) = (code[0], code[1], *code.last().unwrap());
        let guard = lines[a].trim().strip_prefix("#ifndef ").map(str::trim);
        let defined = lines[b].trim().strip_prefix("#define ").map(str::trim);
        if guard.is_some() && guard == defined && lines[z].trim().starts_with("#endif") {
            let mut kept: Vec<&str> = Vec::new();
            for (i, l) in lines.iter().enumerate() {
                if i != a && i != b && i != z {
                    kept.push(l);
                }
            }
            return kept.join("\n").trim_matches('\n').to_string() + "\n";
        }
    }
    text.to_string()
}

struct Plan {
    objects: Vec<String>,
    sources: Vec<String>,
    destination: String,
    text: String,
    notes: Vec<String>,
    headers: BTreeSet<String>,
}

fn plan(root: &Path, objects: &[String], judged: &[super::Judged], target: DecompTarget, name: Option<&str>) -> Result<Plan, String> {
    let sources: Vec<String> = objects.iter().map(|o| source_path(root, o).ok_or_else(|| format!("{o} has no C source"))).collect::<Result<_, _>>()?;
    let families: BTreeSet<String> = sources.iter().map(|s| format!("{:?}", family(root, target, s))).collect();
    if families.len() > 1 {
        return Err(format!("the sources compile with different compilers ({})", families.into_iter().collect::<Vec<_>>().join(", ")));
    }
    let common: BTreeSet<bool> = sources.iter().map(|s| s.starts_with("games/COMMON/")).collect();
    if common.len() > 1 {
        return Err("COMMON and game-only sources never share a file".into());
    }
    let destination = destination(root, objects, judged, name)?;
    let group: BTreeSet<String> = sources.iter().cloned().collect();
    let mut expanded = BTreeSet::new();
    let mut inputs = Vec::new();
    for s in &sources {
        let text = expand_local(root, s, &read(&root.join(s))?, &group, &mut expanded, 0)?;
        inputs.push(Source { path: s.clone(), text });
    }
    // Local includes left as includes must still resolve from the new folder.
    let dest_dir = Path::new(&destination).parent().unwrap_or(Path::new("")).to_path_buf();
    for input in &inputs {
        let dir = Path::new(&input.path).parent().unwrap_or(Path::new(""));
        if dir == dest_dir {
            continue;
        }
        for line in input.text.lines() {
            if let Some(name) = line.trim().strip_prefix("#include").map(str::trim).and_then(|r| r.strip_prefix('"')).and_then(|r| r.split('"').next()) {
                if root.join(dir).join(name).is_file() && !root.join(&dest_dir).join(name).is_file() {
                    return Err(format!("{}: its local header {name} is shared with files outside the unit and would not resolve from {}", input.path, dest_dir.display()));
                }
            }
        }
    }
    // What the merged include set declares.
    let include_text: String = inputs
        .iter()
        .flat_map(|i| crate::c_items::items(&i.text).into_iter().filter(|it| matches!(it.kind, crate::c_items::Kind::Include(_))).map(|it| it.text.trim().to_string()).collect::<Vec<_>>())
        .collect::<BTreeSet<_>>()
        .into_iter()
        .collect::<Vec<_>>()
        .join("\n");
    let headers = crate::adopt_into::header_names(root, target, &root.join(&destination), &format!("{include_text}\n"))?;
    let merged = cmerge::merge(&inputs, &headers).map_err(|e| format!("cannot merge:\n  {}", e.join("\n  ")))?;
    Ok(Plan { objects: objects.to_vec(), sources, destination, text: merged.text, notes: merged.notes, headers: expanded })
}

/// Compile the merged text for every edition that links the objects and
/// compare it with today's placement. Returns one line per image checked.
fn verify(root: &Path, plan: &Plan, text: &str, cache: &mut BTreeMap<String, (DecompTarget, Vec<Loaded>)>) -> Result<Vec<String>, String> {
    let mut lines = Vec::new();
    let mut failures = Vec::new();
    let directory = Path::new(&plan.destination).parent().unwrap_or(Path::new("")).to_path_buf();
    // Local includes resolve beside the merged file; the copy compiled under out/ names them whole.
    let absolute: String = text
        .split_inclusive('\n')
        .map(|line| match line.trim().strip_prefix("#include").map(str::trim).and_then(|r| r.strip_prefix('"')).and_then(|r| r.split('"').next()) {
            Some(name) if root.join(&directory).join(name).is_file() => format!("#include \"{}\"\n", root.join(&directory).join(name).display()),
            _ => line.to_string(),
        })
        .collect();
    let objects: Vec<&str> = plan.objects.iter().map(String::as_str).collect();
    for edition in editions(&plan.sources[0]) {
        if !cache.contains_key(&edition) {
            let target = decomp_target(Some(&edition))?;
            let build = Build { root: root.to_path_buf(), dir: root.join(target.output_dir), name: edition.clone(), compiler: target.compiler };
            cache.insert(edition.clone(), (target, load::load(&build)?));
        }
        let (target, images) = &cache[&edition];
        let mut checked = 0;
        for loaded in images {
            let linked = loaded.pieces.iter().any(|p| objects.contains(&p.object.as_str()));
            if !linked {
                continue;
            }
            checked += 1;
            let id = format!("flatten-{}", Path::new(&plan.destination).file_stem().unwrap_or_default().to_string_lossy());
            let outcome = dryrun::compare_source(root, *target, loaded, &images[0], &objects, &absolute, &id);
            let line = format!("{edition}\t{}\t{}\t{}", loaded.image.name, outcome.verdict, outcome.detail);
            if outcome.verdict != "pass" {
                failures.push(line.clone());
            }
            lines.push(line);
        }
        if checked == 0 {
            lines.push(format!("{edition}\t-\tnot linked\t"));
        }
    }
    if !failures.is_empty() {
        return Err(format!("the merged file does not keep the bytes:\n  {}", failures.join("\n  ")));
    }
    Ok(lines)
}

/// Functions the merged file defines that nothing else names.
fn private_functions(root: &Path, plan: &Plan, text: &str) -> Vec<String> {
    let mut out = Vec::new();
    for item in crate::c_items::items(text) {
        if !matches!(item.kind, crate::c_items::Kind::Function { .. }) {
            continue;
        }
        let head = &item.text[..item.text.find('{').unwrap_or(0)];
        if head.split_whitespace().any(|w| w == "static" || w == "__inline__" || w == "inline") {
            continue;
        }
        let Some(name) = crate::c_items::names(&item).into_iter().next().map(|n| n.name) else {
            continue;
        };
        let found = crate::process::run(&["git", "grep", "-l", "-w", "-F", &name, "--", "games", "recon", "tools"], root).unwrap_or_default();
        if found.lines().all(|l| plan.sources.iter().any(|s| s == l) || l == plan.destination) {
            out.push(name);
        }
    }
    out
}

fn with_static(text: &str, names: &[String]) -> String {
    let mut out = String::with_capacity(text.len() + 16 * names.len());
    for line in text.split_inclusive('\n') {
        let top = !line.starts_with([' ', '\t', '#', '/', '*', '}', '{']) && !line.trim().is_empty();
        let trimmed = line.trim_start();
        let declares = top
            && !trimmed.starts_with("static ")
            && !trimmed.starts_with("extern ")
            && !line.contains('=')
            && names.iter().any(|name| {
                line.match_indices(name.as_str()).any(|(at, _)| {
                    let before = line[..at].chars().last().is_none_or(|c| !(c.is_ascii_alphanumeric() || c == '_'));
                    let after = line[at + name.len()..].trim_start().starts_with('(');
                    before && after && !line[..at].contains('(')
                })
            });
        if declares {
            out.push_str("static ");
        }
        out.push_str(line);
    }
    out
}

fn flatten(options: &Options) -> Result<(), String> {
    crate::compiler::routing::prefer_installed_binutils();
    let root = crate::compiler::routing::root().to_path_buf();
    let (target, judged) = judged(&root, &options.build)?;
    let groups = group(&root, options, &judged, target)?;
    let mut cache = BTreeMap::new();
    for objects in groups {
        let plan = plan(&root, &objects, &judged, target, options.name.as_deref())?;
        println!("{} files -> {}", plan.sources.len(), plan.destination);
        for s in &plan.sources {
            println!("  {s}");
        }
        for h in &plan.headers {
            println!("  expanded {h}");
        }
        for n in &plan.notes {
            println!("  note: {n}");
        }
        // The scripts first: a merge the link cannot express stops here.
        let scripts = linker_scripts(&root)?;
        let merged_object = plan.destination.trim_end_matches(".C").to_string();
        let stems: Vec<String> = plan.objects.iter().map(|o| stem(o).to_string()).collect();
        let mut rewritten = Vec::new();
        for script in &scripts {
            let text = read(script)?;
            match super::ldscript::rewrite(&text, &stems, &merged_object) {
                Ok(Some(new)) => rewritten.push((script.clone(), new)),
                Ok(None) => {}
                Err(e) => return Err(format!("{}: {e}", script.strip_prefix(&root).unwrap_or(script).display())),
            }
        }
        let mut text = plan.text.clone();
        let lines = verify(&root, &plan, &text, &mut cache)?;
        for l in &lines {
            println!("  {l}");
        }
        if options.make_static {
            let private = private_functions(&root, &plan, &text);
            if !private.is_empty() {
                let candidate = with_static(&text, &private);
                match verify(&root, &plan, &candidate, &mut cache) {
                    Ok(_) => {
                        println!("  static: {}", private.join(", "));
                        text = candidate;
                    }
                    Err(e) => println!("  static refused, bytes change: {}", e.lines().nth(1).unwrap_or("").trim()),
                }
            }
        }
        if options.dry_run {
            let out = root.join("out/unit-flatten");
            fs::create_dir_all(&out).map_err(|e| e.to_string())?;
            let path = out.join(Path::new(&plan.destination).file_name().unwrap_or_default());
            fs::write(&path, &text).map_err(|e| format!("{}: {e}", path.display()))?;
            println!("  dry run: {} ({} linker scripts would change)", path.strip_prefix(&root).unwrap_or(&path).display(), rewritten.len());
            continue;
        }
        apply(&root, &plan, &text, &rewritten)?;
        println!("  applied: {} linker scripts", rewritten.len());
    }
    Ok(())
}

fn apply(root: &Path, plan: &Plan, text: &str, scripts: &[(PathBuf, String)]) -> Result<(), String> {
    for (path, new) in scripts {
        fs::write(path, new).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    let mut gone: Vec<String> = plan.sources.iter().filter(|s| **s != plan.destination).cloned().collect();
    gone.extend(plan.headers.iter().cloned());
    for g in &gone {
        crate::process::run(&["git", "rm", "-q", "-f", "--", g], root)?;
    }
    let destination = root.join(&plan.destination);
    fs::write(&destination, text).map_err(|e| format!("{}: {e}", destination.display()))?;
    routing(root, &plan.sources, &plan.destination)?;
    Ok(())
}

/// The routing tables name library and ARM sources by path.
fn routing(root: &Path, sources: &[String], destination: &str) -> Result<(), String> {
    let path = root.join("tools/alchemy/src/compiler/routing_data.rs");
    let text = read(&path)?;
    let mut out = String::new();
    let mut placed = false;
    let mut touched = false;
    for line in text.split_inclusive('\n') {
        let quoted = line.trim().trim_end_matches(',').trim_matches('"');
        if sources.iter().any(|s| s == quoted) {
            touched = true;
            if !placed {
                out.push_str(&line.replace(quoted, destination));
                placed = true;
            }
            continue;
        }
        if line.contains("];") {
            placed = false;
        }
        out.push_str(line);
    }
    if touched {
        fs::write(&path, out).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn names_are_eight_three() {
        assert!(is_eight_three("CLEAR"));
        assert!(is_eight_three("ROM_HEAD"));
        assert!(!is_eight_three("TILEGRIDS"));
        assert!(!is_eight_three("Clear"));
    }

    #[test]
    fn guards_of_inlined_headers_go() {
        assert_eq!(strip_guard("#ifndef A_H\n#define A_H\n\nvoid f(void);\n\n#endif\n"), "void f(void);\n");
        assert_eq!(strip_guard("void f(void);\n"), "void f(void);\n");
    }

    #[test]
    fn statics_are_written_on_definitions_and_prototypes() {
        let text = "void f(void);\nvoid f(void)\n{\n}\nvoid g(void)\n{\n\tf();\n}\n";
        assert_eq!(with_static(text, &["f".into()]), "static void f(void);\nstatic void f(void)\n{\n}\nvoid g(void)\n{\n\tf();\n}\n");
    }

    #[test]
    fn editions_follow_the_game() {
        assert_eq!(editions("games/COMMON/SRC/A.C").len(), 12);
        assert_eq!(editions("games/THE BROKEN SEAL/SRC/A.C")[0], "tbs-ja");
    }
}
