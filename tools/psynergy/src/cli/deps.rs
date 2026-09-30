//! `psynergy deps`: the dependency map of linked builds, one TSV per view.
//! Rebuilt from the builds' ELF images and maps, the recon listings and the
//! drafts on every run; a report for people that nothing else reads.

use super::similar::{classify, images_in};
use psynergy::deps::{self, Blocker, Image, Index, Key, Symbol, Target};
use psynergy::elf::Elf;
use psynergy::similar::{self, Origin};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

pub const USAGE: &str =
    "usage: psynergy deps --build DIR [--build DIR]... --out-dir DIR [--root DIR]\n\
Maps calls and references of every function in each build directory's linked\n\
images (DIR/*.elf and DIR/overlays/*.elf with their maps), the recon listings\n\
(ROOT/recon/GAME/raw) and drafts (ROOT/recon/GAME/LANG), for a build named\n\
GAME-LANG. Writes deps-BUILD-VIEW.tsv into the output directory, one per view:\n\
  functions  every function with its callers, callees and blockers\n\
  calls      call edges (bl, far-call and overlay veneers, pointers)\n\
  references pool words and ids of not-yet-C functions, classified\n\
  blockers   unresolved names by not-yet-C bytes blocked\n\
  frontier   not-yet-C functions with nothing left to name\n\
  drafts     drafts with their score and what is left";

struct Options {
    builds: Vec<PathBuf>,
    root: PathBuf,
    out_dir: PathBuf,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut o = Options {
        builds: Vec::new(),
        root: PathBuf::from("."),
        out_dir: PathBuf::new(),
    };
    let mut out = None;
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        let value = arguments
            .get(i + 1)
            .ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?;
        match flag {
            "--build" => o.builds.push(PathBuf::from(value)),
            "--root" => o.root = PathBuf::from(value),
            "--out-dir" => out = Some(PathBuf::from(value)),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    if o.builds.is_empty() {
        return Err(format!("at least one --build is required\n{USAGE}"));
    }
    o.out_dir = out.ok_or_else(|| format!("--out-dir is required\n{USAGE}"))?;
    Ok(o)
}

fn load(root: &Path, build: &Path, name: &str) -> Result<Vec<Image>, String> {
    let mut images = Vec::new();
    for elf_path in images_in(build)? {
        let data = fs::read(&elf_path).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let map_path = elf_path.with_extension("map");
        let map =
            fs::read_to_string(&map_path).map_err(|e| format!("{}: {e}", map_path.display()))?;
        let stem = elf_path
            .file_stem()
            .map(|s| s.to_string_lossy().into_owned())
            .unwrap_or_default();
        let image = similar::Image {
            build: name,
            elf: &elf,
            map: &map,
        };
        let mut functions = similar::decoded(&image, &|o| classify(root, o), &|_| true);
        functions.sort_by_key(|(f, _)| f.address);
        functions.dedup_by_key(|(f, _)| f.address);
        let symbols = elf
            .symbols
            .iter()
            .filter(|s| {
                s.section != 0
                    && !s.name.is_empty()
                    && !s.name.starts_with('$')
                    && !s.name.starts_with('.')
            })
            .filter(|s| matches!(s.kind, 0..=2 | 13 | 15))
            .map(|s| Symbol {
                name: s.name.clone(),
                value: s.value,
                size: s.size,
                absolute: s.section == 0xfff1,
            })
            .collect();
        images.push(Image {
            build: name.to_string(),
            main: elf_path.parent() == Some(build),
            name: stem,
            functions,
            symbols,
        });
    }
    Ok(images)
}

/// Identifiers the listings use, by the function each label starts.
fn listing_ids(
    root: &Path,
    game: &str,
    images: &[Image],
) -> BTreeMap<(usize, u32), BTreeSet<String>> {
    let mut out: BTreeMap<(usize, u32), BTreeSet<String>> = BTreeMap::new();
    let Some(main) = images.iter().position(|i| i.main) else {
        return out;
    };
    let by_name: BTreeMap<&str, u32> = images[main]
        .symbols
        .iter()
        .filter(|s| !s.absolute)
        .map(|s| (s.name.as_str(), s.value & !1))
        .collect();
    let starts: BTreeSet<u32> = images[main]
        .functions
        .iter()
        .map(|(f, _)| f.address)
        .collect();
    let mut files = Vec::new();
    for dir in [
        root.join("recon").join(game).join("raw"),
        root.join("recon").join(game),
    ] {
        if let Ok(entries) = fs::read_dir(&dir) {
            files.extend(
                entries
                    .flatten()
                    .map(|e| e.path())
                    .filter(|p| p.extension().is_some_and(|e| e == "s" || e == "S")),
            );
        }
    }
    files.sort();
    for path in files {
        let Ok(text) = fs::read_to_string(&path) else {
            continue;
        };
        let mut current = None;
        for (label, used) in deps::listing_identifiers(&text) {
            if let Some(&address) = by_name.get(label.as_str()) {
                if starts.contains(&address) {
                    current = Some(address);
                }
            }
            if let Some(address) = current {
                out.entry((main, address)).or_default().extend(used);
            }
        }
    }
    out
}

struct Draft {
    path: String,
    key: Option<Key>,
    score: Option<u32>,
    status: &'static str,
}

fn hex8(text: &str) -> Vec<u32> {
    text.split(|c: char| !c.is_ascii_hexdigit())
        .filter(|w| w.len() == 8)
        .filter_map(|w| u32::from_str_radix(w, 16).ok())
        .collect()
}

fn drafts(root: &Path, game: &str, lang: &str, images: &[Image]) -> Vec<Draft> {
    let base = root.join("recon").join(game).join(lang);
    let mut files = Vec::new();
    for dir in [base.clone(), base.join("main"), base.join("overlays")] {
        if let Ok(entries) = fs::read_dir(&dir) {
            files.extend(
                entries
                    .flatten()
                    .map(|e| e.path())
                    .filter(|p| p.extension().is_some_and(|e| e == "c")),
            );
        }
    }
    files.sort();
    let not_yet_c: BTreeMap<&str, Key> = images
        .iter()
        .enumerate()
        .flat_map(|(ii, image)| {
            image
                .functions
                .iter()
                .enumerate()
                .filter(|(_, (f, _))| f.origin == Origin::NotYetC)
                .map(move |(fi, (f, _))| (f.name.as_str(), (ii, fi)))
        })
        .collect();
    let at = |image: &str, address: u32| -> Option<Key> {
        let ii = images.iter().position(|i| {
            if image.is_empty() {
                i.main
            } else {
                i.name == image
            }
        })?;
        let fi = images[ii]
            .functions
            .iter()
            .position(|(f, _)| f.address == address)?;
        Some((ii, fi))
    };
    let mut out = Vec::new();
    for path in files {
        let Ok(text) = fs::read_to_string(&path) else {
            continue;
        };
        let file = path
            .file_stem()
            .map(|s| s.to_string_lossy().into_owned())
            .unwrap_or_default();
        let overlay = file
            .strip_prefix("resource_")
            .and_then(|r| r.get(..3))
            .map(|id| format!("resource_{id}"));
        let mut key = hex8(&file)
            .last()
            .and_then(|&a| at(overlay.as_deref().unwrap_or(""), a));
        if key.is_none() {
            // Otherwise the not-yet-C function the draft defines.
            key = text
                .split(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                .find_map(|w| {
                    not_yet_c
                        .get(w)
                        .copied()
                        .filter(|_| text.contains(&format!("{w}(")))
                });
        }
        let notes = deps::comments(&text);
        out.push(Draft {
            path: path
                .strip_prefix(root)
                .unwrap_or(&path)
                .display()
                .to_string(),
            key,
            score: deps::draft_score(&notes),
            status: deps::draft_status(&notes),
        });
    }
    out
}

fn write(dir: &Path, name: &str, text: &str) -> Result<(), String> {
    let path = dir.join(name);
    fs::write(&path, text).map_err(|e| format!("{}: {e}", path.display()))
}

fn names(index: &Index, keys: &[Key], limit: usize) -> String {
    let mut list: Vec<&str> = keys
        .iter()
        .take(limit)
        .map(|k| index.function(*k).name.as_str())
        .collect();
    if keys.len() > limit {
        list.push("...");
    }
    list.join(",")
}

fn report(o: &Options, build: &Path, out_dir: &Path) -> Result<String, String> {
    let name = build
        .file_name()
        .map(|n| n.to_string_lossy().into_owned())
        .unwrap_or_default();
    let (game, lang) = name.split_once('-').unwrap_or((&name, "en"));
    let images = load(&o.root, build, &name)?;
    let ids = listing_ids(&o.root, game, &images);
    let index = Index::build(&images, &ids);
    let drafts = drafts(&o.root, game, lang, &images);
    let draft_of: BTreeMap<Key, &Draft> = drafts
        .iter()
        .filter_map(|d| d.key.map(|k| (k, d)))
        .collect();
    let label = |key: Key| {
        let f = index.function(key);
        format!(
            "{}\t{:08x}\t{}\t{}",
            images[key.0].name,
            f.address,
            f.name,
            f.origin.label()
        )
    };
    let target = |t: &Target| match t {
        Target::Function(k) => label(*k),
        Target::Unnamed(a) => format!("\t{a:08x}\t\tunnamed"),
    };

    let mut callers: BTreeMap<Key, BTreeSet<Key>> = BTreeMap::new();
    let mut callees: BTreeMap<Key, usize> = BTreeMap::new();
    let mut calls = String::from("image\taddress\tname\torigin\tcallee_image\tcallee_address\tcallee\tcallee_origin\tkind\tvia\n");
    for c in &index.calls {
        *callees.entry(c.caller).or_default() += 1;
        if let Target::Function(k) = c.callee {
            callers.entry(k).or_default().insert(c.caller);
        }
        let _ = writeln!(
            calls,
            "{}\t{}\t{}\t{}",
            label(c.caller),
            target(&c.callee),
            c.kind,
            c.via.as_deref().unwrap_or("")
        );
    }

    let mut functions = String::from("image\taddress\tname\torigin\tbytes\tcallers\tcallees\tblockers\tdraft\tdraft_status\tsource\n");
    for (ii, image) in images.iter().enumerate() {
        for (fi, (f, _)) in image.functions.iter().enumerate() {
            let key = (ii, fi);
            let blockers = index
                .blockers
                .get(&key)
                .map_or(0, |s| s.iter().filter(|b| b.blocks()).count());
            let draft = draft_of.get(&key);
            let _ = writeln!(
                functions,
                "{}\t{}\t{}\t{}\t{}\t{}\t{}\t{}",
                label(key),
                f.bytes,
                callers.get(&key).map_or(0, |s| s.len()),
                callees.get(&key).copied().unwrap_or(0),
                blockers,
                draft.map_or("", |d| d.path.as_str()),
                draft.map_or("", |d| d.status),
                f.source
            );
        }
    }

    let mut references = String::from("image\taddress\tname\torigin\tclass\treference\tvalue\n");
    for r in &index.references {
        if index.function(r.function).origin == Origin::NotYetC {
            let _ = writeln!(
                references,
                "{}\t{}\t{}\t{:08x}",
                label(r.function),
                r.class.label(),
                r.name,
                r.value
            );
        }
    }

    let ranked = index.ranked_blockers();
    let mut blockers = String::from("rank\tkind\tscope\titem\tfunctions\tbytes\tblocked\n");
    for (rank, (b, keys, bytes)) in ranked.iter().enumerate() {
        let rank = if b.blocks() {
            (rank + 1).to_string()
        } else {
            "info".into()
        };
        let _ = writeln!(
            blockers,
            "{rank}\t{}\t{}\t{}\t{}\t{bytes}\t{}",
            b.kind,
            b.scope,
            b.item,
            keys.len(),
            names(&index, keys, 20)
        );
    }

    let frontier_keys = index.frontier();
    let mut frontier = String::from(
        "rank\timage\taddress\tname\torigin\tbytes\tnamed_not_yet_c_callees\tdraft\tdraft_status\n",
    );
    for (rank, key) in frontier_keys.iter().enumerate() {
        let info: Vec<&str> = index.blockers[key]
            .iter()
            .map(|b: &Blocker| b.item.as_str())
            .collect();
        let draft = draft_of.get(key);
        let _ = writeln!(
            frontier,
            "{}\t{}\t{}\t{}\t{}\t{}",
            rank + 1,
            label(*key),
            index.function(*key).bytes,
            info.join(","),
            draft.map_or("", |d| d.path.as_str()),
            draft.map_or("", |d| d.status)
        );
    }

    let mut draft_rows =
        String::from("draft\timage\taddress\tname\torigin\tbytes\tscore\tstatus\tblockers\n");
    for d in &drafts {
        let (function, bytes, blocking) = match d.key {
            Some(k) => (
                label(k),
                index.function(k).bytes.to_string(),
                index
                    .blockers
                    .get(&k)
                    .map_or(0, |s| s.iter().filter(|b| b.blocks()).count())
                    .to_string(),
            ),
            None => ("\t\t\t".into(), String::new(), String::new()),
        };
        let score = d.score.map(|s| s.to_string()).unwrap_or_default();
        let _ = writeln!(
            draft_rows,
            "{}\t{function}\t{bytes}\t{score}\t{}\t{blocking}",
            d.path, d.status
        );
    }

    fs::create_dir_all(out_dir).map_err(|e| format!("{}: {e}", out_dir.display()))?;
    write(out_dir, &format!("deps-{name}-functions.tsv"), &functions)?;
    write(out_dir, &format!("deps-{name}-calls.tsv"), &calls)?;
    write(out_dir, &format!("deps-{name}-references.tsv"), &references)?;
    write(out_dir, &format!("deps-{name}-blockers.tsv"), &blockers)?;
    write(out_dir, &format!("deps-{name}-frontier.tsv"), &frontier)?;
    write(out_dir, &format!("deps-{name}-drafts.tsv"), &draft_rows)?;
    let not_yet_c = index.blockers.len();
    let blocking = ranked.iter().filter(|(b, _, _)| b.blocks()).count();
    Ok(format!(
        "{name}: {} functions, {} calls, {not_yet_c} not yet C, {blocking} blockers, {} on the frontier, {} drafts; {}\n",
        images.iter().map(|i| i.functions.len()).sum::<usize>(),
        index.calls.len(),
        frontier_keys.len(),
        drafts.len(),
        out_dir.display()
    ))
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let o = parse(arguments)?;
    let mut text = String::new();
    for build in &o.builds {
        text.push_str(&report(&o, build, &o.out_dir)?);
    }
    Ok(text)
}
