//! `alchemy unit`: applying the recovered translation units to the tree.
//!
//!   unit candidates  every run of today's C files the analysis joins, with
//!                    the evidence at its edges (TSV)
//!   unit flatten     one such run merged into one file in ROM order: the
//!                    merged source, every linker script and object list
//!                    and the routing tables updated, the superseded
//!                    files removed
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
       alchemy unit flatten (IMAGE:UNIT | --objects PATH.c,PATH.c...) [--build EDITION]
                            [--name NAME.c] [--static] [--dry-run]
       alchemy unit flatten --all --force [--build EDITION]
candidates writes DIR/units-EDITION-merges.tsv (default out/reports): each run
of today's C files the analysis joins, with the units it covers, the
evidence at its two edges and its weakest join. An edge is forced (another
language or compiler, data, a region, the image's end), strong (code shared
with other images ends there) or weak (only the scored valley, names or
folders say so); apply is yes when neither edge is weak and the weakest
join is at least 0.7 (runs joined below that were rejected on review).
flatten merges the runs of one proposed unit (IMAGE:UNIT as in the units
TSV; main is the main image), or the named sources, into one C file in ROM
order, in the folder holding most of their functions and named after its
main module file (the largest 8.3-named one), else after the folder;
--name chooses. A scene's file (directly in games/<game>/src/scene) keeps
its place's full name, the one name that may be longer than 8.3. Every
linker script under games/ and recon/ and every game's object list
(games/<game>/objects.tsv) that names the objects, and the compiler
routing tables, follow; the old files and local headers only they
include are removed. Every edition that links the
objects is compiled and compared first, from today's builds; --dry-run
stops there. --static also makes functions no other file names static,
kept only when the bytes stay identical. Build every edition first.
--all --force is an experiment, never a landing: every proposed unit's C files
become one file whatever divides them today (a weak edge, the COMMON/game
divide, link order, editions that link only some), applied even when the
bytes change; each line of its output names what was refused or forced.";

struct Options {
    build: String,
    out_dir: PathBuf,
    unit: Option<String>,
    objects: Option<Vec<String>>,
    name: Option<String>,
    dry_run: bool,
    make_static: bool,
    force: bool,
    all: bool,
    hard: bool,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options { build: "tbs-en".into(), out_dir: PathBuf::from("out/reports"), unit: None, objects: None, name: None, dry_run: false, make_static: false, force: false, all: false, hard: false };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        match flag {
            "--dry-run" => options.dry_run = true,
            "--static" => options.make_static = true,
            "--force" => options.force = true,
            "--all" => options.all = true,
            "--hard" => {
                options.all = true;
                options.hard = true;
            }
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
    let path = format!("{}.c", stem(object));
    root.join(&path).is_file().then_some(path)
}

pub(super) fn judged(root: &Path, edition: &str) -> Result<(DecompTarget, Vec<super::Judged>), String> {
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

/// The weakest internal join a run may have and still be applied: runs
/// offered at 0.696, 0.670 and 0.516 were applied and rejected on review.
const MIN_JOIN: f64 = 0.7;

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

/// Whether a run is offered to apply: neither edge weak, no join below MIN_JOIN.
fn applies(chain: &Chain) -> bool {
    !matches!(chain.left, Edge::Weak(_)) && !matches!(chain.right, Edge::Weak(_)) && chain.weakest >= MIN_JOIN
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
        Some((family(root, target, &source), f.object.starts_with("games/common/"), linked.get(&f.object).cloned().unwrap_or_default()))
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
            let apply = applies(&c);
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
                c.objects.iter().map(|o| format!("{}.c", stem(o))).collect::<Vec<_>>().join(",")
            );
        }
    }
    let out_dir = root.join(&options.out_dir);
    fs::create_dir_all(&out_dir).map_err(|e| format!("{}: {e}", out_dir.display()))?;
    let path = out_dir.join(format!("units-{}-merges.tsv", options.build));
    fs::write(&path, out).map_err(|e| format!("{}: {e}", path.display()))?;
    println!("{yes} runs to apply, {no} with a weak edge or a join below {MIN_JOIN}: {}", path.strip_prefix(&root).unwrap_or(&path).display());
    Ok(())
}

/// The editions that build a source: all six of its game, and both games'
/// for COMMON.
fn editions(source: &str) -> Vec<String> {
    let games: &[&str] = if source.starts_with("games/common/") {
        &["tbs", "tla"]
    } else if source.starts_with("games/tla/") {
        &["tla"]
    } else {
        &["tbs"]
    };
    games.iter().flat_map(|g| ["ja", "en", "de", "es", "fr", "it"].iter().map(move |l| format!("{g}-{l}"))).collect()
}

fn linker_scripts(root: &Path) -> Result<Vec<PathBuf>, String> {
    let listed = crate::process::run(&["git", "ls-files", "--", "*.ld"], root)?;
    Ok(listed.lines().filter(|l| l.starts_with("games/") || l.starts_with("recon/")).map(|l| root.join(l)).collect())
}

/// Each game's object list that names some of `objects`, with its text once
/// `merged` takes their place in every image and edition.
fn rewritten_lists(root: &Path, objects: &[String], merged: &str, force: bool) -> Result<Vec<(PathBuf, String)>, String> {
    let mut found = Vec::new();
    for game in crate::targets::TARGET_IDS.chunks(6) {
        let target = crate::targets::target_for(game[0]);
        let (Some(path), Some(before)) = (target.object_list(), crate::object_list::read(root, target)?) else {
            continue;
        };
        let rows = rewritten_rows(&before, objects, merged, force).map_err(|e| format!("{}: {e}", path.display()))?;
        if rows != before {
            found.push((path.clone(), crate::object_list::written(root, target, rows)?));
        }
    }
    Ok(found)
}

fn rewritten_rows(before: &[crate::object_list::Row], objects: &[String], merged: &str, force: bool) -> Result<Vec<crate::object_list::Row>, String> {
    let mut rows = before.to_vec();
    for image in crate::object_list::images(before) {
        for language in crate::object_list::LANGUAGES {
            let view = crate::object_list::view(&rows, &image, language);
            let new = if force { super::ldscript::rewrite_forced(&view, objects, merged) } else { super::ldscript::rewrite(&view, objects, merged).map_err(|e| format!("{image} {language}: {e}"))? };
            if let Some(new) = new {
                crate::object_list::apply(&mut rows, &image, language, &new)?;
            }
        }
    }
    Ok(rows)
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
            if !root.join(path).is_file() || !path.ends_with(".c") {
                return Err(format!("{path}: not a C source of this tree"));
            }
            out.push(format!("{}.o", path.trim_end_matches(".c")));
        }
        // Given in ROM order, as the candidates list them; today's build must link each.
        // The first names the unit; a file of tables alone may follow it.
        for o in out.iter().take(1) {
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
    !stem.is_empty() && stem.len() <= 8 && stem.chars().all(|c| c.is_ascii_lowercase() || c.is_ascii_digit() || c == '_')
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
        Some(n) => n.trim_end_matches(".c").to_string(),
        None => match area_word(root, &folder, objects) {
            Some(word) => word,
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
        },
    };
    let path = format!("{folder}/{file}.c");
    // Eight letters, or a scene's own file under its place's full name (S3).
    if !crate::check::hygiene::name_length_fits(&path) {
        return Err(format!("{file}: not a lower-case 8.3 name; pass --name"));
    }
    let theirs = objects.iter().any(|o| format!("{}.c", stem(o)) == path);
    if !theirs {
        for extension in ["c", "s", "S", "mid", "h"] {
            let other = format!("{folder}/{file}.{extension}");
            if root.join(&other).exists() {
                return Err(format!("{other} already exists; pass --name"));
            }
        }
    }
    Ok(path)
}

/// An area's main unit, the one defining what its entry table's veneers
/// name. Where the area has a folder of its own, with entry.s in it, the
/// unit takes the last word of the folder (haidia_mura/mura.c), or the 8.3
/// abbreviation of it the folder's files already use. Where areas share
/// one folder, each with an entry table of its own (scene/entry/NAME.s, as
/// a unit row of the object list finds it), the unit keeps its name, the
/// area's. None when the run is not the main unit or another file already
/// has that name.
fn area_word(root: &Path, folder: &str, objects: &[String]) -> Option<String> {
    let own = objects.iter().filter_map(|o| stem(o).strip_prefix(folder)?.strip_prefix('/')).find(|name| {
        crate::object_list::tables(root, &format!("{folder}/{name}")).is_some_and(|[entry, _]| entry == format!("{folder}/entry/{name}"))
    });
    if let Some(own) = own {
        return Some(own.to_owned());
    }
    let entry = read(&root.join(folder).join("entry.s")).ok()?;
    let entries: Vec<&str> = entry.lines().filter_map(|l| l.trim().strip_prefix("overlay_veneer")).map(str::trim).filter(|n| !n.is_empty()).collect();
    let main = objects.iter().any(|o| {
        let text = read(&root.join(format!("{}.c", stem(o)))).unwrap_or_default();
        text.lines().any(|line| {
            !line.starts_with([' ', '\t', '#', '/', '*']) && !line.trim_end().ends_with(';') && entries.iter().any(|e| line.split(|c: char| !c.is_ascii_alphanumeric() && c != '_').any(|w| w == *e) && line.contains(&format!("{e}(")))
        })
    });
    if !main {
        return None;
    }
    let leaf = Path::new(folder).file_name()?.to_string_lossy().into_owned();
    let word = leaf.rsplit('_').next()?.to_string();
    let word = if word.len() <= 8 {
        word
    } else {
        let mut known: Vec<String> = fs::read_dir(root.join(folder))
            .ok()?
            .filter_map(|e| e.ok())
            .filter_map(|e| e.path().file_stem().map(|s| s.to_string_lossy().into_owned()))
            .filter(|s| is_eight_three(s) && s.len() >= 4 && word.starts_with(s.as_str()))
            .collect();
        known.sort_by_key(|s| std::cmp::Reverse(s.len()));
        known.into_iter().next().unwrap_or_else(|| word.chars().take(8).collect())
    };
    let path = format!("{folder}/{word}.c");
    let ours = objects.iter().any(|o| format!("{}.c", stem(o)) == path);
    (ours || !root.join(&path).exists()).then_some(word)
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
            // Only a header beside the source is local; a path reaching into
            // include/ names a shared header.
            let header = directory.join(name).to_string_lossy().into_owned();
            if !name.contains('/') && root.join(&header).is_file() && depth < 8 {
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
    // Includes may stand before the guard; they stay.
    let first = code.iter().position(|&i| !lines[i].trim_start().starts_with("#include")).unwrap_or(code.len());
    let code = &code[first..];
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

fn plan(root: &Path, objects: &[String], judged: &[super::Judged], target: DecompTarget, name: Option<&str>, views: u8, force: bool) -> Result<Plan, String> {
    let sources: Vec<String> = objects.iter().map(|o| source_path(root, o).ok_or_else(|| format!("{o} has no C source"))).collect::<Result<_, _>>()?;
    let families: BTreeSet<String> = sources.iter().map(|s| format!("{:?}", family(root, target, s))).collect();
    if families.len() > 1 {
        return Err(format!("the sources compile with different compilers ({})", families.into_iter().collect::<Vec<_>>().join(", ")));
    }
    let common: BTreeSet<bool> = sources.iter().map(|s| s.starts_with("games/common/")).collect();
    if common.len() > 1 && !force {
        return Err("COMMON and game-only sources never share a file".into());
    }
    let mut forced_notes: Vec<String> = Vec::new();
    // A forced merge across the COMMON divide is the game's own file.
    let own = sources.iter().find(|s| !s.starts_with("games/common/")).cloned();
    let destination = match destination(root, objects, judged, name) {
        Ok(found) => match (&own, common.len() > 1 && found.starts_with("games/common/")) {
            (Some(own), true) => {
                let game = own.split('/').nth(1).unwrap_or("tbs");
                let moved = found.replacen("games/common/", &format!("games/{game}/"), 1);
                if root.join(&moved).exists() && !sources.contains(&moved) {
                    forced_notes.push(format!("forced: {moved} already exists, so the merged file takes {own}"));
                    own.clone()
                } else {
                    forced_notes.push(format!("forced: COMMON and game-only sources share {moved}"));
                    moved
                }
            }
            _ => found,
        },
        Err(e) if force => {
            let fallback = own.clone().unwrap_or_else(|| sources[0].clone());
            forced_notes.push(format!("forced: {e}; the merged file takes {fallback}"));
            fallback
        }
        Err(e) => return Err(e),
    };
    let group: BTreeSet<String> = sources.iter().cloned().collect();
    let mut expanded = BTreeSet::new();
    let mut inputs: Vec<Source> = Vec::new();
    for s in &sources {
        let text = expand_local(root, s, &read(&root.join(s))?, &group, &mut expanded, 0)?;
        let own = if views > 0 {
            let lines: String = crate::c_items::items(&text).into_iter().filter(|it| matches!(it.kind, crate::c_items::Kind::Include(_))).map(|it| format!("{}\n", it.text.trim())).collect();
            Some(crate::adopt_into::header_names(root, target, &root.join(s), &lines)?)
        } else {
            None
        };
        inputs.push(Source { path: s.clone(), text, own });
    }
    // Local includes left as includes must still resolve from the new folder.
    let dest_dir = Path::new(&destination).parent().unwrap_or(Path::new("")).to_path_buf();
    for input in inputs.iter_mut() {
        let dir = Path::new(&input.path).parent().unwrap_or(Path::new("")).to_path_buf();
        if dir == dest_dir {
            continue;
        }
        let mut out = String::new();
        let mut changed = false;
        for line in input.text.split_inclusive('\n') {
            if let Some(name) = line.trim().strip_prefix("#include").map(str::trim).and_then(|r| r.strip_prefix('"')).and_then(|r| r.split('"').next()) {
                if root.join(&dir).join(name).is_file() && !root.join(&dest_dir).join(name).is_file() {
                    if !force {
                        return Err(format!("{}: its local header {name} is shared with files outside the unit and would not resolve from {}", input.path, dest_dir.display()));
                    }
                    // Forced: the header stays where it is and is named from the new folder.
                    let header = dir.join(name);
                    let (from, to): (Vec<_>, Vec<_>) = (dest_dir.components().collect(), header.components().collect());
                    let shared = from.iter().zip(&to).take_while(|(a, b)| a == b).count();
                    let mut parts: Vec<String> = vec!["..".to_string(); from.len() - shared];
                    parts.extend(to[shared..].iter().map(|c| c.as_os_str().to_string_lossy().into_owned()));
                    out.push_str(&format!("#include \"{}\"\n", parts.join("/")));
                    forced_notes.push(format!("forced: {} names its local header {name} from {}", input.path, dest_dir.display()));
                    changed = true;
                    continue;
                }
            }
            out.push_str(line);
        }
        if changed {
            input.text = out;
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
    let merged = cmerge::merge(&inputs, &headers, &cmerge::Options { views: views > 0, unprototyped: views > 1, every: views > 2 }).map_err(|e| format!("cannot merge:\n  {}", e.join("\n  ")))?;
    let mut notes = merged.notes;
    notes.extend(forced_notes);
    Ok(Plan { objects: objects.to_vec(), sources, destination, text: merged.text, notes, headers: expanded })
}

fn plan_views(root: &Path, objects: &[String], judged: &[super::Judged], target: DecompTarget, name: Option<&str>, level: u8, force: bool) -> Result<Plan, String> {
    plan(root, objects, judged, target, name, level, force)
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
    for edition in editions(&plan.destination) {
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
            let outcome = dryrun::compare_source(root, *target, loaded, &images[0], &objects, &absolute, &id, family(root, *target, &plan.sources[0]));
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
    let groups = if options.all { all_groups(&root, &judged, target, options.hard) } else { group(&root, options, &judged, target)? };
    let mut cache = BTreeMap::new();
    for objects in groups {
        match flatten_one(&root, options, &judged, target, &objects, &mut cache) {
            Ok(()) => {}
            Err(e) if options.force => println!("REFUSED\t{}\t{}", objects.join(","), e.replace('\n', " | ")),
            Err(e) => return Err(e),
        }
    }
    Ok(())
}

/// Every proposed unit's C objects as one group, whatever divides them
/// today: units that share a file join, and a group splits only where two
/// compilers build it. An experiment's grouping (--all with --force).
fn all_groups(root: &Path, judged: &[super::Judged], target: DecompTarget, hard: bool) -> Vec<Vec<String>> {
    let mut every: Vec<Vec<String>> = Vec::new();
    for j in judged {
        let mut groups: Vec<Vec<String>> = Vec::new();
        // With --hard a unit ends only where the bytes prove an edge: every
        // scored boundary inside such a run is ignored.
        let runs: Vec<(usize, usize)> = if hard {
            let count = j.loaded.image.functions.len();
            let mut runs = Vec::new();
            let mut first = 0;
            for gap in &j.gaps {
                if matches!(gap.verdict, super::model::Verdict::ForcedBoundary | super::model::Verdict::Conflict) {
                    runs.push((first, gap.index));
                    first = gap.index + 1;
                }
            }
            if first < count {
                runs.push((first, count - 1));
            }
            runs
        } else {
            j.units.iter().map(|u| (u.first, u.last)).collect()
        };
        for (run_first, run_last) in runs {
            let mut objects: Vec<String> = Vec::new();
            for f in &j.loaded.image.functions[run_first..=run_last] {
                if matches!(f.lang, Lang::C | Lang::Library) && f.object.starts_with("games/") && !(hard && f.object.contains("/system/save/")) && source_path(root, &f.object).is_some() && !objects.contains(&f.object) {
                    objects.push(f.object.clone());
                }
            }
            let (joined, rest): (Vec<Vec<String>>, Vec<Vec<String>>) = groups.drain(..).partition(|g| g.iter().any(|o| objects.contains(o)));
            groups = rest;
            if !joined.is_empty() {
                println!("NOTE\tshared\t{}\ta file is shared with an earlier unit, so they become one file: {}", j.loaded.image.name, objects.join(","));
            }
            let mut merged: Vec<String> = joined.into_iter().flatten().collect();
            for o in objects {
                if !merged.contains(&o) {
                    merged.push(o);
                }
            }
            if !merged.is_empty() {
                groups.push(merged);
            }
        }
        every.extend(groups);
    }
    // A file several images link is its own unit: it joins none of them.
    let mut seen: BTreeMap<String, usize> = BTreeMap::new();
    for g in &every {
        for o in g {
            *seen.entry(o.clone()).or_default() += 1;
        }
    }
    for (o, n) in seen.iter().filter(|(_, n)| **n > 1) {
        println!("NOTE\tseveral-images\t{o} is linked into {n} images and stays its own file");
    }
    let mut out = Vec::new();
    for g in every {
        let g: Vec<String> = g.into_iter().filter(|o| seen.get(o).copied().unwrap_or(0) < 2).collect();
        let mut by: Vec<(String, Vec<String>)> = Vec::new();
        for o in g {
            let key = format!("{:?}", family(root, target, &source_path(root, &o).unwrap_or_default()));
            match by.iter_mut().find(|(k, _)| *k == key) {
                Some((_, v)) => v.push(o),
                None => by.push((key, vec![o])),
            }
        }
        if by.len() > 1 {
            println!("NOTE\tcompilers\ta unit spans {} compilers and stays that many files: {}", by.len(), by.iter().map(|(k, v)| format!("{k}: {}", v.join(","))).collect::<Vec<_>>().join(" / "));
        }
        out.extend(by.into_iter().map(|(_, v)| v).filter(|v| v.len() >= 2));
    }
    out
}

fn flatten_one(root: &Path, options: &Options, judged: &[super::Judged], target: DecompTarget, objects: &[String], cache: &mut BTreeMap<String, (DecompTarget, Vec<Loaded>)>) -> Result<(), String> {
    {
        let mut plan = plan(root, objects, judged, target, options.name.as_deref(), 0, options.force)?;
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
        let scripts = linker_scripts(root)?;
        let merged_object = plan.destination.trim_end_matches(".c").to_string();
        let stems: Vec<String> = plan.objects.iter().map(|o| stem(o).to_string()).collect();
        // A forced merge into one game's file leaves the other game's scripts alone.
        let one_game = options.force && !plan.destination.starts_with("games/common/");
        let mut rewritten = Vec::new();
        for script in &scripts {
            let relative = script.strip_prefix(root).unwrap_or(script).to_string_lossy().into_owned();
            if one_game && relative.split('/').nth(1) != plan.destination.split('/').nth(1) {
                continue;
            }
            let text = read(script)?;
            let new = if options.force { super::ldscript::rewrite_forced(&text, &stems, &merged_object) } else { super::ldscript::rewrite(&text, &stems, &merged_object).map_err(|e| format!("{relative}: {e}"))? };
            if let Some(new) = new {
                rewritten.push((script.clone(), new));
            }
        }
        // A game with an object list names its objects there: the same
        // rewrite, over each image's rows as each edition links them.
        for (list, text) in rewritten_lists(root, &stems, &merged_object, options.force)? {
            rewritten.push((root.join(list), text));
        }
        // Plainly first; failing that, each file keeps its own shapes as casts.
        let mut forced = false;
        let lines = match verify(root, &plan, &plan.text, cache) {
            Ok(lines) => lines,
            Err(first) => {
                let mut found = None;
                let mut tried: Vec<(Option<Plan>, String)> = vec![(None, first)];
                // Levels 1 and 2 cast only shapes a caller could tell from
                // the governing one; 3 casts every differing shape, so no
                // run the merge took before is refused now.
                for level in [1, 2, 3] {
                    match plan_views(root, objects, judged, target, options.name.as_deref(), level, options.force) {
                        Ok(second) => match verify(root, &second, &second.text, cache) {
                            Ok(lines) => {
                                found = Some((second, lines));
                                break;
                            }
                            Err(again) => tried.push((Some(second), again)),
                        },
                        Err(e) if options.force => tried.push((None, e)),
                        Err(e) => return Err(e),
                    }
                }
                match found {
                    Some((second, lines)) => {
                        println!("  kept by casts:");
                        for n in second.notes.iter().filter(|n| n.contains("as a cast")) {
                            println!("    {n}");
                        }
                        plan = second;
                        lines
                    }
                    None => {
                        if !options.force {
                            let reasons: Vec<&str> = tried.iter().map(|(_, r)| r.as_str()).collect();
                            return Err(format!(
                                "{}\nwith each file's own prototyped shapes as casts, {}\nwith all its own shapes as casts, {}\nwith even the shapes that say the same as casts, {}",
                                reasons[0],
                                reasons.get(1).unwrap_or(&""),
                                reasons.get(2).unwrap_or(&""),
                                reasons.get(3).unwrap_or(&"")
                            ));
                        }
                        // The plain merge, unless only a view with casts compiles.
                        let compiles = |reason: &str| reason.lines().skip(1).all(|l| l.trim().split('\t').nth(2) == Some("differ"));
                        let mut chosen = 0;
                        if !compiles(&tried[0].1) {
                            if let Some(k) = (1..tried.len()).find(|k| tried[*k].0.is_some() && compiles(&tried[*k].1)) {
                                chosen = k;
                            }
                        }
                        let (other, reason) = tried.swap_remove(chosen);
                        if let Some(other) = other {
                            plan = other;
                        }
                        forced = true;
                        println!("FORCED\t{}\t{}\t{}", plan.destination, if chosen == 0 { "plain" } else { "casts" }, reason.replace('\n', " | "));
                        Vec::new()
                    }
                }
            }
        };
        let mut text = plan.text.clone();
        for l in &lines {
            println!("  {l}");
        }
        if options.make_static && !forced {
            let private = private_functions(root, &plan, &text);
            if !private.is_empty() {
                let candidate = with_static(&text, &private);
                match verify(root, &plan, &candidate, cache) {
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
            println!("  dry run: {} ({} linker scripts and object lists would change)", path.strip_prefix(root).unwrap_or(&path).display(), rewritten.len());
            return Ok(());
        }
        apply(root, &plan, &text, &rewritten, options.force)?;
        println!("{}\t{}\t{} files, {} linker scripts and object lists", if forced { "APPLIED-FORCED" } else { "APPLIED-IDENTICAL" }, plan.destination, plan.sources.len(), rewritten.len());
    }
    Ok(())
}

fn apply(root: &Path, plan: &Plan, text: &str, scripts: &[(PathBuf, String)], force: bool) -> Result<(), String> {
    for (path, new) in scripts {
        fs::write(path, new).map_err(|e| format!("{}: {e}", path.display()))?;
    }
    let mut gone: Vec<String> = plan.sources.iter().filter(|s| **s != plan.destination).cloned().collect();
    let mut kept = false;
    if force {
        // A source another game's script still links stays for that game.
        let mut named = String::new();
        for script in linker_scripts(root)? {
            named.push_str(&read(&script)?);
        }
        gone.retain(|s| {
            let still = named.contains(&format!("{}.o\"", s.trim_end_matches(".c")));
            if still {
                kept = true;
                println!("  kept for the other game: {s}");
            }
            !still
        });
    }
    if !kept {
        gone.extend(plan.headers.iter().cloned());
    }
    for g in &gone {
        fs::remove_file(root.join(g)).map_err(|e| format!("{g}: {e}"))?;
    }
    let destination = root.join(&plan.destination);
    if let Some(parent) = destination.parent() {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
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
    fn an_object_list_takes_the_merged_object_in_every_image_and_edition() {
        let list = |rows: &str| crate::object_list::parse(Path::new(""), &format!("{}\n{rows}", crate::object_list::HEADER)).unwrap();
        let before = list(
            "main\trom_start\tgames/g/x\t.text\n\
             main\trom_start\tgames/g/a\t.text\n\
             main\trom_start\tgames/g/b\t.text\n\
             main\trom_start\tgames/g/only_ja\t.text\tja\n\
             main\trom_tail\tgames/g/a\t.rodata\n\
             main\trom_tail\tgames/g/b\t.rodata\n\
             3ad\ttext\tgames/g/a\t.text\n\
             3ad\ttext\tgames/g/b\t.text\n",
        );
        let objects = ["games/g/a".to_owned(), "games/g/b".to_owned()];
        let rows = rewritten_rows(&before, &objects, "games/g/m", false).unwrap();
        assert_eq!(
            rows,
            list(
                "main\trom_start\tgames/g/x\t.text\n\
                 main\trom_start\tgames/g/m\t.text\n\
                 main\trom_start\tgames/g/only_ja\t.text\tja\n\
                 main\trom_tail\tgames/g/m\t.rodata\n\
                 3ad\ttext\tgames/g/m\t.text\n"
            )
        );
        assert_eq!(rewritten_rows(&before, &["games/g/q".to_owned()], "games/g/m", false).unwrap(), before);
        // An edition that links one of the objects and not the other is refused.
        let partial = list("main\trom_start\tgames/g/a\t.text\nmain\trom_start\tgames/g/b\t.text\tja\n");
        assert!(rewritten_rows(&partial, &objects, "games/g/m", false).unwrap_err().contains("main en"));
    }

    #[test]
    fn runs_apply_only_with_strong_edges_and_joins() {
        let chain = |left: Edge, right: Edge, weakest: f64| Chain {
            image: "main".into(),
            objects: vec!["a".into(), "b".into()],
            first: 0,
            last: 1,
            units: vec![0],
            left,
            right,
            weakest,
        };
        let forced = || Edge::Forced("data".into());
        assert!(applies(&chain(Edge::End, forced(), 0.7)));
        assert!(applies(&chain(Edge::Strong("p=0.90 shared-edge".into()), Edge::End, 0.95)));
        // U0054 and U0067: no weak edge, but a weak join inside.
        assert!(!applies(&chain(forced(), forced(), 0.696)));
        assert!(!applies(&chain(forced(), Edge::End, 0.670)));
        assert!(!applies(&chain(forced(), forced(), 0.516)));
        assert!(!applies(&chain(Edge::Weak("p=0.40".into()), forced(), 0.99)));
    }

    #[test]
    fn names_are_eight_three() {
        assert!(is_eight_three("clear"));
        assert!(is_eight_three("rom_head"));
        assert!(!is_eight_three("tilegrids"));
        assert!(!is_eight_three("Clear"));
        assert!(!is_eight_three("CLEAR"));
    }

    #[test]
    fn guards_of_inlined_headers_go() {
        assert_eq!(strip_guard("#ifndef A_H\n#define A_H\n\nvoid f(void);\n\n#endif\n"), "void f(void);\n");
        assert_eq!(strip_guard("void f(void);\n"), "void f(void);\n");
        assert_eq!(strip_guard("#include \"a.h\"\n#ifndef B_H\n#define B_H\nvoid f(void);\n#endif\n"), "#include \"a.h\"\nvoid f(void);\n");
    }

    #[test]
    fn statics_are_written_on_definitions_and_prototypes() {
        let text = "void f(void);\nvoid f(void)\n{\n}\nvoid g(void)\n{\n\tf();\n}\n";
        assert_eq!(with_static(text, &["f".into()]), "static void f(void);\nstatic void f(void)\n{\n}\nvoid g(void)\n{\n\tf();\n}\n");
    }

    #[test]
    fn an_areas_main_unit_takes_its_folder_word() {
        let root = tempfile::tempdir().unwrap();
        let folder = "games/g/src/field/haidia_mura";
        fs::create_dir_all(root.path().join(folder)).unwrap();
        fs::write(root.path().join(folder).join("entry.s"), "\toverlay_veneer HaidiaMura_Setup\n").unwrap();
        fs::write(root.path().join(folder).join("setup.c"), "s32 HaidiaMura_Setup(void)\n{\n}\n").unwrap();
        fs::write(root.path().join(folder).join("other.c"), "void Other(void)\n{\n}\n").unwrap();
        let o = |n: &str| format!("{folder}/{n}.o");
        assert_eq!(area_word(root.path(), folder, &[o("setup"), o("other")]).as_deref(), Some("mura"));
        assert_eq!(area_word(root.path(), folder, &[o("other")]), None);
        fs::write(root.path().join(folder).join("mura.c"), "").unwrap();
        assert_eq!(area_word(root.path(), folder, &[o("setup")]), None);
    }

    #[test]
    fn an_area_in_a_shared_folder_keeps_its_name() {
        let root = tempfile::tempdir().unwrap();
        let folder = "games/g/src/scene";
        fs::create_dir_all(root.path().join(folder).join("entry")).unwrap();
        fs::write(root.path().join(folder).join("entry/haidia_mura.s"), "\toverlay_veneer HaidiaMura_Setup\n").unwrap();
        fs::write(root.path().join(folder).join("haidia_mura.c"), "s32 HaidiaMura_Setup(void)\n{\n}\n").unwrap();
        fs::write(root.path().join(folder).join("other.c"), "void Other(void)\n{\n}\n").unwrap();
        let o = |n: &str| format!("{folder}/{n}.o");
        // The file with the entry table of its own is the area's, whichever comes first.
        assert_eq!(area_word(root.path(), folder, &[o("other"), o("haidia_mura")]).as_deref(), Some("haidia_mura"));
        // A run without it is no area's main unit: the folder has no entry.s.
        assert_eq!(area_word(root.path(), folder, &[o("other")]), None);
    }

    #[test]
    fn shared_headers_reached_by_path_stay_includes() {
        let root = tempfile::tempdir().unwrap();
        fs::create_dir_all(root.path().join("games/common/src/sound")).unwrap();
        fs::create_dir_all(root.path().join("games/common/include/system")).unwrap();
        fs::write(root.path().join("games/common/include/system/io_reg.h"), "#define REG 1\n").unwrap();
        let source = "games/common/src/sound/engine.c";
        let text = "#include \"../../include/system/io_reg.h\"\nvoid f(void)\n{\n}\n";
        let group: BTreeSet<String> = [source.to_string()].into();
        let mut expanded = BTreeSet::new();
        assert_eq!(expand_local(root.path(), source, text, &group, &mut expanded, 0).unwrap(), text);
        assert!(expanded.is_empty());
    }

    #[test]
    fn merged_copies_compile_with_their_sources_compiler() {
        // The copy verify compiles lives under out/ and routes as game code;
        // library sources must still reach their own compiler.
        let target = decomp_target(Some("tbs-en")).unwrap();
        assert_eq!(family(Path::new("."), target, "games/common/src/sound/engine.c"), CompilerFamily::Agbcc);
        assert_eq!(family_for_source(target.compiler, "out/tbs-en/obj/units/flatten-engine.c"), CompilerFamily::Game);
    }

    #[test]
    fn editions_follow_the_game() {
        assert_eq!(editions("games/common/src/a.c").len(), 12);
        assert_eq!(editions("games/tbs/src/a.c")[0], "tbs-ja");
    }
}
