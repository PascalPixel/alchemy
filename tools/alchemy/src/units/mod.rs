//! `alchemy units`: the analysis half of recovering Camelot's translation
//! units. Reads one build's linked images, judges every gap between
//! adjacent functions, groups the functions into proposed units with their
//! data, validates the grouping against what we already know, and can
//! compile a few proposed units as one merged file to see whether the bytes
//! hold. Read-only toward games/ and the linker scripts; reports for people.

mod cmerge;
mod dryrun;
mod flatten;
mod floor;
mod ldscript;
mod split;
pub use split::run as unit;
mod load;
mod model;

use crate::targets::decomp_target;
use load::{Build, Loaded};
use model::{Gap, Image, Lang, Pad, Unit, Verdict};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::fs;
use std::path::PathBuf;

const USAGE: &str = "usage: alchemy units [--build EDITION] [--out-dir DIR] [--dry-run N|UNIT,UNIT...]
       alchemy units evidence [--build EDITION] [--out FILE] [--devices FILE]
Proposes Camelot's translation units for one built edition (default tbs-en;
its sizes are the English build's, as DONE's are). Reads the edition's main
and overlay ELFs, their linker maps and the objects they name; build the
edition first. For every gap between adjacent functions of each image it
records the forced boundaries (data placed between, another region, another
language, compiler or instruction set, padding one file cannot produce),
the forced joins (a plain branch into the other side, a helper reached only
from a short run around it) and the scored evidence (weighted Jaccard of
the reference sets, calls between neighbours, a wider run of private
referrers, and, weakly, our names, folders and statics), then groups the
functions into units with their data. Writes into DIR (default out/reports):
  units-EDITION-gaps.tsv        every gap with its verdict, probability and reasons
  units-EDITION-units.tsv       proposed units: placeholder name, confidence, extent, today's objects
  units-EDITION-summary.tsv     per image: functions, today's files, units, forced and judged gaps
  units-EDITION-validation.tsv  agreement with overlays, merged files and modules with data
  units-EDITION-dryrun.tsv      with --dry-run: merged units compiled and compared
--dry-run N picks N proposed units of whole C objects; UNIT names them by id
(main:U0042). Merged sources and objects stay in out/EDITION/obj/units.
evidence writes out/reports/floor-evidence-<game>.tsv: what the source and the
compile of each C file show at every gap between two of its functions
(alchemy units evidence --help).";

struct Options {
    build: String,
    out_dir: PathBuf,
    dry_run: Option<String>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options { build: "tbs-en".into(), out_dir: PathBuf::from("out/reports"), dry_run: None };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        if flag == "-h" || flag == "--help" {
            return Err(USAGE.into());
        }
        let value = arguments.get(i + 1).ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?.clone();
        match flag {
            "--build" => options.build = value,
            "--out-dir" => options.out_dir = PathBuf::from(value),
            "--dry-run" => options.dry_run = Some(value),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    Ok(options)
}

pub struct Judged {
    pub loaded: Loaded,
    pub gaps: Vec<Gap>,
    pub units: Vec<Unit>,
    pub binary_units: Vec<Unit>,
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if arguments.first().is_some_and(|a| a == "evidence") {
        return floor::run(&arguments[1..]);
    }
    let options = parse(arguments)?;
    let target = decomp_target(Some(&options.build))?;
    let root = crate::compiler::routing::root().to_path_buf();
    let build = Build { root: root.clone(), dir: root.join(target.output_dir), name: options.build.clone(), compiler: target.compiler };
    let judged: Vec<Judged> = load::load(&build)?
        .into_iter()
        .map(|loaded| {
            let gaps = model::judge(&loaded.image);
            let units = model::group(&loaded.image, &gaps, true);
            let binary_units = model::group(&loaded.image, &gaps, false);
            Judged { loaded, gaps, units, binary_units }
        })
        .collect();
    let out = root.join(&options.out_dir);
    fs::create_dir_all(&out).map_err(|e| format!("{}: {e}", out.display()))?;
    let write = |view: &str, text: String| -> Result<(), String> {
        let path = out.join(format!("units-{}-{view}.tsv", options.build));
        fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))
    };
    write("gaps", gaps_report(&judged))?;
    write("functions", functions_report(&judged))?;
    let signals = signals_report(&judged);
    write("signals", signals.clone())?;
    write("units", units_report(&judged))?;
    let summary = summary_report(&judged);
    write("summary", summary.clone())?;
    let validation = validation_report(&judged);
    write("validation", validation.clone())?;
    print!("{}", totals(&summary));
    print!("{}", validation.lines().filter(|l| l.starts_with("check") || !l.contains("\tdetail\t")).collect::<Vec<_>>().join("\n"));
    println!();
    if let Some(choice) = options.dry_run {
        let report = dryrun::run(&root, target, &judged, &choice)?;
        print!("{report}");
        write("dryrun", report)?;
    }
    println!("reports: {}/units-{}-*.tsv", options.out_dir.display(), options.build);
    Ok(())
}

fn clean(text: &str) -> String {
    text.replace(['\t', '\n'], " ")
}

fn short(object: &str) -> &str {
    let object = object.trim_end_matches(".o");
    object.split_once("/src/").map_or(object, |(_, tail)| tail)
}

fn gaps_report(judged: &[Judged]) -> String {
    let mut out = String::from("image\tgap\tleft\tright\tleft_object\tright_object\tsame_object\tverdict\tp\tp_binary\tsimilarity\tpad\tforced_boundary\tforced_join\tsource_join\tsignals\n");
    for j in judged {
        let image = &j.loaded.image;
        for g in &j.gaps {
            let (a, b) = (&image.functions[g.index], &image.functions[g.index + 1]);
            let pad = Pad::of(&a.pad);
            let _ = writeln!(
                out,
                "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{:.3}\t{:.3}\t{:.3}\t{}:{}\t{}\t{}\t{}\t{}",
                image.name,
                g.index,
                a.name,
                b.name,
                short(&a.object),
                short(&b.object),
                if a.object == b.object { "yes" } else { "no" },
                g.verdict.label(),
                g.probability,
                g.binary,
                g.similarity,
                pad.label(),
                a.pad.len(),
                clean(&g.forced_boundary.join(" ")),
                clean(&g.forced_join.join(" ")),
                clean(&g.source_join.join(" ")),
                clean(&g.signals.join(" ")),
            );
        }
    }
    out
}

fn unit_id(index: usize) -> String {
    format!("U{:04}", index + 1)
}

/// Today's objects a unit shares with functions outside it.
fn split_objects(image: &Image, unit: &Unit) -> usize {
    unit.objects(image)
        .iter()
        .filter(|o| image.functions.iter().enumerate().any(|(i, f)| f.object == **o && (i < unit.first || i > unit.last)))
        .count()
}

fn unit_bytes(image: &Image, unit: &Unit) -> (u32, u32) {
    let text = image.functions[unit.last].end - image.functions[unit.first].start;
    let data = unit.data.iter().map(|&d| image.data[d].end - image.data[d].start).sum();
    (text, data)
}

fn units_report(judged: &[Judged]) -> String {
    let mut out = String::from("image\tunit\tname\tconfidence\tfunctions\ttext_bytes\tdata_bytes\tstart\tend\tfirst\tlast\tlanguage\ttoday_objects\tsplits_objects\tobjects\n");
    for j in judged {
        let image = &j.loaded.image;
        for (k, u) in j.units.iter().enumerate() {
            let (text, data) = unit_bytes(image, u);
            let end = u.data.iter().map(|&d| image.data[d].end).max().unwrap_or(0).max(image.functions[u.last].end);
            let objects = u.objects(image);
            let langs: BTreeSet<&str> = image.functions[u.first..=u.last].iter().map(|f| f.lang.label()).collect();
            let _ = writeln!(
                out,
                "{}\t{}\t{}\t{:.3}\t{}\t{}\t{}\t{:08x}\t{:08x}\t{}\t{}\t{}\t{}\t{}\t{}",
                image.name,
                unit_id(k),
                u.name,
                u.confidence,
                u.last - u.first + 1,
                text,
                data,
                image.functions[u.first].start,
                end,
                image.functions[u.first].name,
                image.functions[u.last].name,
                langs.into_iter().collect::<Vec<_>>().join(","),
                objects.len(),
                split_objects(image, u),
                objects.iter().map(|o| short(o)).collect::<Vec<_>>().join(","),
            );
        }
    }
    out
}

fn today_files(image: &Image) -> usize {
    image.functions.iter().map(|f| f.object.as_str()).collect::<BTreeSet<_>>().len()
}

fn summary_report(judged: &[Judged]) -> String {
    let mut out = String::from("image\tfunctions\ttoday_files\tunits\tunits_binary\tforced_boundaries\tforced_joins\tconflicts\tjudged_boundaries\tjudged_joins\tlargest_unit\tsmallest_unit\tunclaimed_data\n");
    for j in judged {
        let image = &j.loaded.image;
        let count = |v: Verdict| j.gaps.iter().filter(|g| g.verdict == v).count();
        let sizes = |u: &Unit| (u.last - u.first + 1, unit_bytes(image, u).0);
        let largest = j.units.iter().max_by_key(|u| sizes(u).1);
        let smallest = j.units.iter().min_by_key(|u| sizes(u).1);
        let describe = |u: Option<&Unit>| u.map_or_else(String::new, |u| format!("{} {}f {}B", u.name, sizes(u).0, sizes(u).1));
        let _ = writeln!(
            out,
            "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
            image.name,
            image.functions.len(),
            today_files(image),
            j.units.len(),
            j.binary_units.len(),
            count(Verdict::ForcedBoundary),
            count(Verdict::ForcedJoin),
            count(Verdict::Conflict),
            count(Verdict::JudgedBoundary),
            count(Verdict::JudgedJoin),
            describe(largest),
            describe(smallest),
            {
                let blocks = model::unclaimed(image);
                format!("{} blocks {}B", blocks.len(), blocks.iter().map(|&d| image.data[d].end - image.data[d].start).sum::<u32>())
            },
        );
    }
    out
}

/// The summary's main row and the overlays added together.
fn totals(summary: &str) -> String {
    let mut out = String::new();
    let mut overlay = [0usize; 9];
    let mut overlays = 0;
    let header = summary.lines().next().unwrap_or("");
    let columns: Vec<&str> = header.split('\t').skip(1).take(9).collect();
    for line in summary.lines().skip(1) {
        let fields: Vec<&str> = line.split('\t').collect();
        let numbers: Vec<usize> = fields.iter().skip(1).take(9).filter_map(|f| f.parse().ok()).collect();
        if fields[0].starts_with("resource_") {
            overlays += 1;
            for (sum, n) in overlay.iter_mut().zip(&numbers) {
                *sum += n;
            }
        } else {
            let pairs: Vec<String> = columns.iter().zip(&numbers).map(|(c, n)| format!("{c} {n}")).collect();
            let _ = writeln!(out, "{}: {} | largest {} | smallest {}", fields[0], pairs.join(", "), fields[10], fields[11]);
        }
    }
    let pairs: Vec<String> = columns.iter().zip(&overlay).map(|(c, n)| format!("{c} {n}")).collect();
    let _ = writeln!(out, "{overlays} overlays: {}", pairs.join(", "));
    out
}

fn validation_report(judged: &[Judged]) -> String {
    let mut out = String::from("check\tscope\tagree\ttotal\tnote\n");
    let mut details = String::new();
    // Overlays: one unit of code each, or a few.
    for binary in [false, true] {
        let mode = if binary { "binary" } else { "all" };
        let mut one = 0;
        let mut few = 0;
        let mut total = 0;
        for j in judged.iter().filter(|j| j.loaded.image.name.starts_with("resource_")) {
            let image = &j.loaded.image;
            let units = if binary { &j.binary_units } else { &j.units };
            let code = units.iter().filter(|u| image.functions[u.first..=u.last].iter().any(|f| f.lang != Lang::Veneer)).count();
            total += 1;
            one += usize::from(code == 1);
            few += usize::from(code <= 3);
            if !binary && code > 3 {
                let _ = writeln!(details, "overlay\t{}\tdetail\t{code}\tcode units", image.name);
            }
        }
        let _ = writeln!(out, "overlays-one-code-unit\t{mode}\t{one}\t{total}\toverlays whose code (veneers aside) is one unit");
        let _ = writeln!(out, "overlays-at-most-three\t{mode}\t{few}\t{total}\toverlays whose code is three units or fewer");
    }
    // title.c, merged earlier from five files.
    for j in judged {
        let image = &j.loaded.image;
        let members: Vec<usize> = image.functions.iter().enumerate().filter(|(_, f)| f.object.ends_with("src/menu/title/title.o")).map(|(i, _)| i).collect();
        if members.is_empty() {
            continue;
        }
        for binary in [false, true] {
            let units = if binary { &j.binary_units } else { &j.units };
            let holding: BTreeSet<usize> = members.iter().filter_map(|&m| units.iter().position(|u| u.first <= m && m <= u.last)).collect();
            let foreign: usize = holding
                .iter()
                .map(|&u| image.functions[units[u].first..=units[u].last].iter().filter(|f| f.lang == Lang::C && !f.object.ends_with("title.o")).count())
                .sum();
            let internal = members.len() - 1;
            let joined = members.windows(2).filter(|w| holding.len() == 1 || units.iter().any(|u| u.first <= w[0] && w[1] <= u.last)).count();
            let _ = writeln!(
                out,
                "title\t{}\t{joined}\t{internal}\t{} functions of title.c in {} unit(s); {foreign} other C functions share them",
                if binary { "binary" } else { "all" },
                members.len(),
                holding.len()
            );
        }
    }
    // Modules whose files hold several functions and data of their own.
    for binary in [false, true] {
        let mut objects = 0;
        let mut kept = 0;
        let mut internal = 0;
        let mut joined = 0;
        for j in judged {
            let image = &j.loaded.image;
            let units = if binary { &j.binary_units } else { &j.units };
            let mut by_object: BTreeMap<&str, Vec<usize>> = BTreeMap::new();
            for (i, f) in image.functions.iter().enumerate() {
                if f.lang == Lang::C {
                    by_object.entry(&f.object).or_default().push(i);
                }
            }
            for (object, members) in by_object {
                if members.len() < 2 {
                    continue;
                }
                if !image.data.iter().any(|d| d.object == object) {
                    continue;
                }
                objects += 1;
                let gaps = members.windows(2).count();
                let together = members.windows(2).filter(|w| units.iter().any(|u| u.first <= w[0] && w[1] <= u.last)).count();
                internal += gaps;
                joined += together;
                if together == gaps {
                    kept += 1;
                } else if !binary {
                    let _ = writeln!(details, "module\t{}:{}\tdetail\t{}\tof {} internal gaps judged boundaries", image.name, short(object), gaps - together, gaps);
                }
            }
        }
        let mode = if binary { "binary" } else { "all" };
        let _ = writeln!(out, "modules-with-data-kept-whole\t{mode}\t{kept}\t{objects}\tmulti-function C files that also hold data, not split");
        let _ = writeln!(out, "modules-with-data-internal-gaps\t{mode}\t{joined}\t{internal}\tinternal gaps of those files judged joins");
    }
    // Today's file edges: how many the proposal keeps (not ground truth).
    for binary in [false, true] {
        let mut edges = 0;
        let mut kept = 0;
        let mut inside = 0;
        let mut inside_cut = 0;
        for j in judged {
            let image = &j.loaded.image;
            for g in &j.gaps {
                let (a, b) = (&image.functions[g.index], &image.functions[g.index + 1]);
                let cut = if binary {
                    match g.verdict {
                        Verdict::JudgedBoundary | Verdict::JudgedJoin => g.binary >= 0.5,
                        v => v.is_boundary(),
                    }
                } else {
                    g.verdict.is_boundary()
                };
                if a.object != b.object {
                    edges += 1;
                    kept += usize::from(cut);
                } else {
                    inside += 1;
                    inside_cut += usize::from(cut);
                }
            }
        }
        let mode = if binary { "binary" } else { "all" };
        let _ = writeln!(out, "today-file-edges-kept\t{mode}\t{kept}\t{edges}\tgaps between today's files the proposal also cuts (today's files are ours, not ground truth)");
        let _ = writeln!(out, "today-file-insides-cut\t{mode}\t{inside_cut}\t{inside}\tgaps inside today's files the proposal cuts");
    }
    // Link order: today's objects with code and trailing data in one region
    // place both in the same order, as one object list for .text then .rodata.
    let mut agree = 0;
    let mut pairs = 0;
    let mut objects_seen = 0;
    for j in judged {
        let image = &j.loaded.image;
        let owner = model::data_owner(image);
        let mut first_code: BTreeMap<(usize, &str), usize> = BTreeMap::new();
        for (i, f) in image.functions.iter().enumerate() {
            first_code.entry((f.region, f.object.as_str())).or_insert(i);
        }
        let mut first_data: BTreeMap<(usize, &str), u32> = BTreeMap::new();
        for (d, data) in image.data.iter().enumerate() {
            if owner[d].is_none() {
                let e = first_data.entry((data.region, data.object.as_str())).or_insert(data.start);
                *e = (*e).min(data.start);
            }
        }
        let both: Vec<(usize, usize, u32)> = first_code.iter().filter_map(|(k, &i)| first_data.get(k).map(|&a| (k.0, i, a))).collect();
        objects_seen += both.len();
        for (x, a) in both.iter().enumerate() {
            for b in &both[x + 1..] {
                if a.0 == b.0 {
                    pairs += 1;
                    agree += usize::from((a.1 < b.1) == (a.2 < b.2));
                }
            }
        }
    }
    let _ = writeln!(out, "link-order-pairs\tall\t{agree}\t{pairs}\tpairs of today's objects ({objects_seen} with code and trailing data in one region) whose data order follows their code order");
    // Padding between functions against today's files.
    let mut pads: BTreeMap<(String, &str), usize> = BTreeMap::new();
    for j in judged {
        let image = &j.loaded.image;
        for g in &j.gaps {
            let (a, b) = (&image.functions[g.index], &image.functions[g.index + 1]);
            if g.data_bytes > 0 || a.lang != Lang::C || b.lang != Lang::C {
                continue;
            }
            let key = format!("{}:{}", Pad::of(&a.pad).label(), a.pad.len());
            *pads.entry((key, if a.object == b.object { "same-object" } else { "other-object" })).or_default() += 1;
        }
    }
    for ((pad, side), n) in pads {
        let _ = writeln!(out, "pad\t{side}\t{n}\t\t{pad} between two C functions with no data between");
    }
    out.push_str(&details);
    out
}

fn functions_report(judged: &[Judged]) -> String {
    let mut out = String::from("image\tindex\tname\tstart\tend\tobject\tlanguage\tisa\tstatic\texternal\tcallees\tbranches\trefs\n");
    for j in judged {
        let image = &j.loaded.image;
        for (i, f) in image.functions.iter().enumerate() {
            let names = |set: &BTreeSet<usize>| set.iter().map(|&c| image.functions[c].name.as_str()).collect::<Vec<_>>().join(" ");
            let _ = writeln!(
                out,
                "{}\t{i}\t{}\t{:08x}\t{:08x}\t{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
                image.name,
                f.name,
                f.start,
                f.end,
                short(&f.object),
                f.lang.label(),
                if f.arm { "arm" } else { "thumb" },
                if f.is_static { "yes" } else { "no" },
                if f.external { "yes" } else { "no" },
                names(&f.callees),
                names(&f.branches),
                f.refs.iter().cloned().collect::<Vec<_>>().join(" "),
            );
        }
    }
    out
}

/// Area under the ROC curve: the chance a boundary's value exceeds a join's.
pub fn auc(boundaries: &[f64], joins: &[f64]) -> f64 {
    if boundaries.is_empty() || joins.is_empty() {
        return 0.5;
    }
    let mut all: Vec<(f64, bool)> = boundaries.iter().map(|&v| (v, true)).chain(joins.iter().map(|&v| (v, false))).collect();
    all.sort_by(|a, b| a.0.total_cmp(&b.0));
    // Mann-Whitney with tied ranks averaged.
    let mut rank_sum = 0.0;
    let mut i = 0;
    while i < all.len() {
        let mut j = i;
        while j < all.len() && all[j].0 == all[i].0 {
            j += 1;
        }
        let rank = (i + j + 1) as f64 / 2.0;
        rank_sum += rank * all[i..j].iter().filter(|x| x.1).count() as f64;
        i = j;
    }
    let (p, q) = (boundaries.len() as f64, joins.len() as f64);
    (rank_sum - p * (p + 1.0) / 2.0) / (p * q)
}

/// How well each scored signal separates the gaps the binary already settles:
/// data placed between two C functions (a boundary) against a plain branch or
/// a private helper across the gap (a join), and code linked into several
/// images against code that recurs only with its neighbour.
fn signals_report(judged: &[Judged]) -> String {
    let mut out = String::from("label_set\tfeature\tauc\tdirection\tboundaries\tjoins\tmean_boundary\tmean_join\n");
    type Label = fn(&Gap, &Image) -> Option<bool>;
    let forced: Label = |g, image| {
        let (a, b) = (&image.functions[g.index], &image.functions[g.index + 1]);
        if a.lang != Lang::C || b.lang != Lang::C {
            return None;
        }
        let edge = g.features.iter().any(|(k, v)| *k == "shared-edge" && *v > 0.0);
        match g.verdict {
            Verdict::ForcedBoundary if g.forced_boundary.iter().all(|r| r.starts_with("data:")) => Some(true),
            Verdict::JudgedBoundary | Verdict::JudgedJoin if edge => Some(true),
            Verdict::ForcedJoin => Some(false),
            _ => None,
        }
    };
    let shared: Label = |g, image| {
        let (a, b) = (&image.functions[g.index], &image.functions[g.index + 1]);
        if a.lang != Lang::C || b.lang != Lang::C || g.verdict.is_forced() {
            return None;
        }
        let edge = g.features.iter().any(|(k, v)| *k == "shared-edge" && *v > 0.0);
        let pair = g.features.iter().any(|(k, v)| *k == "shared-pair" && *v > 0.0);
        if edge {
            Some(true)
        } else if pair {
            Some(false)
        } else {
            None
        }
    };
    for (name, label) in [("shared-edge-or-data-vs-forced-join", forced), ("shared-edge-vs-shared-pair", shared)] {
        let mut values: BTreeMap<&str, (Vec<f64>, Vec<f64>)> = BTreeMap::new();
        for j in judged {
            for g in &j.gaps {
                let Some(boundary) = label(g, &j.loaded.image) else {
                    continue;
                };
                for (k, v) in &g.features {
                    let entry = values.entry(k).or_default();
                    if boundary {
                        entry.0.push(*v);
                    } else {
                        entry.1.push(*v);
                    }
                }
            }
        }
        for (feature, (b, jn)) in values {
            let a = auc(&b, &jn);
            let mean = |v: &[f64]| if v.is_empty() { 0.0 } else { v.iter().sum::<f64>() / v.len() as f64 };
            let _ = writeln!(
                out,
                "{name}\t{feature}\t{:.3}\t{}\t{}\t{}\t{:.3}\t{:.3}",
                a.max(1.0 - a),
                if a >= 0.5 { "boundary" } else { "join" },
                b.len(),
                jn.len(),
                mean(&b),
                mean(&jn)
            );
        }
    }
    out
}
