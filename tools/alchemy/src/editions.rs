//! `alchemy similar --editions`: how each ☀️ function that is not yet C reads
//! in all twelve builds. A report for people and matchers (O1): it reads the
//! linked builds under out/, writes only under out/, and nothing reads it back.

use crate::decode::Kind;
use crate::diff;
use crate::elf::Elf;
use crate::ld_map::{is_placeholder, preferred, Map, Placed};
use crate::show;
use crate::similar::{self, Function, Image, Interner, Origin};
use crate::similar_command::{classify, images_in};
use std::collections::BTreeSet;
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

pub const USAGE: &str = "usage: alchemy similar --editions [--root DIR] [--out FILE] [--top N]\n\
For every ☀️ function that is not yet C in out/tbs-en (recon/tbs/raw listings and\n\
overlay listings), finds its copy in each of the twelve builds under out/: by\n\
symbol name in the six ☀️ editions, by name or by `alchemy similar`'s nearest\n\
twin in the six ⚓️ editions. Each copy is identical, relocated (only literal-pool\n\
words or call targets differ), differs in N instructions (normalised as alchemy\n\
diff normalises), or absent. Also lists the nearest already-C twins in tbs-en\n\
and tla-en. Writes one TSV row per function and edition, and prints a summary.\n\
  --root DIR   checkout whose out/ is read (default .)\n\
  --out FILE   default out/reports/editions-tbs.tsv\n\
  --top N      already-C twins listed per function (default 3)";

const LANGUAGES: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
const MAX_RATIO: f64 = 0.35;
const MIN_INSNS: usize = 8;

struct Loaded {
    name: String,
    map: Map,
    text: String,
    elf: Elf,
}

struct Edition {
    id: String,
    images: Vec<Loaded>,
}

fn load(build: &Path) -> Result<Edition, String> {
    let mut images = Vec::new();
    for elf_path in images_in(build)? {
        let data = fs::read(&elf_path).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", elf_path.display()))?;
        let map_path = elf_path.with_extension("map");
        let text = fs::read_to_string(&map_path).map_err(|e| format!("{}: {e}", map_path.display()))?;
        let name = elf_path.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
        images.push(Loaded { name, map: Map::parse(&text), text, elf });
    }
    let id = build.file_name().map(|n| n.to_string_lossy().into_owned()).unwrap_or_default();
    Ok(Edition { id, images })
}

#[derive(Clone, Debug)]
pub struct Unit {
    pub image: String,
    pub name: String,
    pub names: Vec<String>,
    pub address: u32,
    pub bytes: Vec<u8>,
    pub arm: bool,
}

fn is_function(kind: u8) -> bool {
    matches!(kind, 2 | 13 | 15)
}

/// Function starts in a placed section: the ELF's function symbols, and each
/// Thumb mapping symbol whose first instruction pushes lr, as a function after
/// a literal pool begins in an assembled listing.
fn starts_in(elf: &Elf, placed: &Placed) -> Vec<u32> {
    let pushes = |address: u32| {
        elf.section_at(address).and_then(|(section, at)| section.bytes.get(at + 1)).is_some_and(|&high| high == 0xb5)
    };
    elf.symbols
        .iter()
        .filter(|s| (is_function(s.kind) && !s.name.starts_with('$')) || (s.name == "$t" && pushes(s.value)))
        .map(|s| s.value & !1)
        .filter(|&a| a >= placed.address && a < placed.end())
        .collect()
}

fn names_at(elf: &Elf, placed: &Placed, address: u32) -> Vec<String> {
    let mut names: Vec<String> = Vec::new();
    if let Some(first) = preferred(&placed.symbols, address) {
        names.push(first.to_string());
    }
    let from_map = placed.symbols.iter().filter(|(a, _)| *a == address).map(|(_, n)| n.clone());
    let from_elf = elf
        .symbols
        .iter()
        .filter(|s| s.value & !1 == address && s.section != 0 && !s.name.is_empty())
        .filter(|s| !s.name.starts_with('$') && !s.name.starts_with('.'))
        .map(|s| s.name.clone());
    for name in from_map.chain(from_elf) {
        if !names.contains(&name) {
            names.push(name);
        }
    }
    let (named, placeholders): (Vec<String>, Vec<String>) = names.into_iter().partition(|n| !is_placeholder(n));
    named.into_iter().chain(placeholders).collect()
}

fn is_arm(elf: &Elf, placed: &Placed, start: u32) -> bool {
    let at: Vec<_> = elf.symbols.iter().filter(|s| s.value & !1 == start && is_function(s.kind)).collect();
    if at.iter().any(|s| s.value & 1 == 1 || s.kind != 2) {
        return false;
    }
    let mapping = elf
        .symbols
        .iter()
        .filter(|s| (s.name.starts_with("$a") || s.name.starts_with("$t")) && s.value >= placed.address && s.value <= start)
        .max_by_key(|s| s.value);
    match mapping {
        Some(s) => s.name.starts_with("$a"),
        None => !at.is_empty(),
    }
}

/// Where a Thumb function ends inside a longer run: at the first `push {.., lr}`
/// that follows a return, as `alchemy similar` splits unlabelled runs.
pub fn thumb_length(bytes: &[u8], base: u32, from: usize) -> usize {
    let mut after_return = false;
    for ins in similar::sweep(bytes, base, base, base + bytes.len() as u32) {
        if after_return && ins.addr > base && (ins.addr - base) as usize >= from && matches!(ins.kind, Kind::Push { lr: true, .. }) {
            return (ins.addr - base) as usize;
        }
        let at = (ins.addr - base) as usize;
        let padding = bytes.get(at..at + 2) == Some(&[0, 0]);
        after_return = matches!(ins.kind, Kind::Bx(_) | Kind::Pop { pc: true, .. }) || (after_return && padding);
    }
    bytes.len()
}

fn read(elf: &Elf, start: u32, end: u32) -> Vec<u8> {
    elf.section_at(start)
        .map(|(section, offset)| {
            let length = (end.saturating_sub(start) as usize).min(section.bytes.len() - offset);
            section.bytes[offset..offset + length].to_vec()
        })
        .unwrap_or_default()
}

fn extent(elf: &Elf, placed: &Placed, start: u32, name: Option<&str>, from: usize) -> (u32, Vec<u8>) {
    let statics = starts_in(elf, placed);
    let size = name.and_then(|n| elf.symbols.iter().find(|s| s.name == n && is_function(s.kind)).map(|s| s.size));
    let end = show::function_end(placed, start, &statics, size);
    let mut bytes = read(elf, start, end);
    if !is_arm(elf, placed, start) {
        bytes.truncate(thumb_length(&bytes, start, from));
    }
    (start + bytes.len() as u32, bytes)
}

fn origin_of(root: &Path, placed: &Placed) -> &'static str {
    if placed.path.ends_with("/rom.o") || placed.name.starts_with(".rom.") {
        return "scaffold";
    }
    classify(root, &placed.path).0.label()
}

/// The three longest runs of at least 16 bytes the reference holds unchanged when
/// only its calls and pools move: (offset, bytes).
pub fn anchors(reference: &[u8], base: u32, arm: bool) -> Vec<(usize, Vec<u8>)> {
    let mut runs: Vec<(usize, usize)> = Vec::new();
    let mut open: Option<usize> = None;
    let close = |open: &mut Option<usize>, at: usize, runs: &mut Vec<(usize, usize)>| {
        if let Some(from) = open.take() {
            if at - from >= 16 {
                runs.push((from, at));
            }
        }
    };
    if arm {
        let (_, pool) = arm_tokens(reference);
        for i in 0..reference.len() / 4 {
            let w = arm_word(reference, i);
            if pool.contains(&i) || w & 0x0f00_0000 == 0x0b00_0000 {
                close(&mut open, i * 4, &mut runs);
            } else if open.is_none() {
                open = Some(i * 4);
            }
        }
        close(&mut open, reference.len() / 4 * 4, &mut runs);
    } else {
        let lines = show::thumb_lines(reference, base, base, base + reference.len() as u32, &show::Names::new());
        for line in &lines {
            let at = (line.address - base) as usize;
            if line.text.starts_with("bl") || line.text.starts_with(".word") || line.text.starts_with("ldr") && line.text.contains(", =") {
                close(&mut open, at, &mut runs);
            } else if open.is_none() {
                open = Some(at);
            }
        }
        close(&mut open, reference.len(), &mut runs);
    }
    runs.sort_by_key(|(from, to)| (std::cmp::Reverse(to - from), *from));
    runs.truncate(3);
    runs.sort();
    (0..runs.len()).map(|i| (runs[i].0, reference[runs[i].0..runs[i].1].to_vec())).collect()
}

fn rank(verdict: &Verdict) -> usize {
    match verdict.class {
        "identical" => 0,
        "relocated" => 1,
        _ => 2 + verdict.instructions,
    }
}

/// Finds a copy of `reference` in an edition by its unchanged runs of bytes,
/// keeping the start whose comparison is closest.
fn by_bytes(edition: &Edition, reference: &Unit, root: &Path) -> Option<(Unit, &'static str)> {
    let mut best: Option<(usize, Unit, &'static str)> = None;
    for (offset, run) in anchors(&reference.bytes, reference.address, reference.arm) {
        for image in &edition.images {
            for section in &image.elf.sections {
                let mut found = 0;
                let step = if reference.arm { 4 } else { 2 };
                let hits = section.bytes.windows(run.len()).enumerate().step_by(step).filter(|(_, w)| *w == run.as_slice());
                for at in hits.map(|(at, _)| at) {
                    found += 1;
                    if found > 4 {
                        break;
                    }
                    if at < offset {
                        continue;
                    }
                    let start = section.address + (at - offset) as u32;
                    if (start & 1) != 0 || best.as_ref().is_some_and(|(_, b, _)| b.address == start && b.image == image.name) {
                        continue;
                    }
                    let placed = image.map.at_address(start);
                    let mut end = start + (reference.bytes.len() as u32) * 5 / 4 + 16;
                    if let Some(p) = placed {
                        end = show::function_end(p, start, &starts_in(&image.elf, p), None).min(end);
                    }
                    let mut bytes = read(&image.elf, start, end);
                    if !reference.arm {
                        bytes.truncate(thumb_length(&bytes, start, reference.bytes.len() * 3 / 4));
                    }
                    let verdict = compare(&reference.bytes, reference.address, &bytes, start, reference.arm);
                    let score = rank(&verdict);
                    let units = reference.bytes.len() / if reference.arm { 4 } else { 2 };
                    if verdict.class == "differs" && verdict.instructions * 100 > units * 35 {
                        continue;
                    }
                    if best.as_ref().is_none_or(|(s, _, _)| score < *s) {
                        let name = placed
                            .and_then(|p| preferred(&p.symbols, start).map(str::to_string))
                            .unwrap_or_else(|| format!("@{start:08x}"));
                        let origin = placed.map_or("other", |p| origin_of(root, p));
                        best = Some((score, Unit { image: image.name.clone(), name: name.clone(), names: vec![name], address: start, bytes, arm: reference.arm }, origin));
                    }
                }
            }
        }
    }
    best.map(|(_, unit, origin)| (unit, origin))
}

/// The not-yet-C functions of one build: every text section its objects
/// classify as not-yet-C, split at the function symbols the ELF holds.
fn units(edition: &Edition, root: &Path) -> Vec<Unit> {
    let mut out = Vec::new();
    for image in &edition.images {
        for placed in &image.map.placed {
            if !placed.name.starts_with(".text") || placed.size == 0 || classify(root, &placed.path).0 != Origin::NotYetC {
                continue;
            }
            let starts: BTreeSet<u32> = std::iter::once(placed.address).chain(starts_in(&image.elf, placed)).collect();
            for &start in &starts {
                let names = names_at(&image.elf, placed, start);
                let name = match names.first() {
                    Some(name) => name.clone(),
                    None if start != placed.address => format!("sub_{start:08x}"),
                    None => continue,
                };
                let (_, bytes) = extent(&image.elf, placed, start, Some(&name), 0);
                if bytes.is_empty() {
                    continue;
                }
                out.push(Unit { image: image.name.clone(), name, names, address: start, arm: is_arm(&image.elf, placed, start), bytes });
            }
        }
    }
    out
}

fn by_name(edition: &Edition, unit: &Unit, placeholders: bool, root: &Path) -> Option<(Unit, &'static str)> {
    let mut images: Vec<&Loaded> = edition.images.iter().collect();
    images.sort_by_key(|i| i.name != unit.image);
    for name in unit.names.iter().filter(|n| placeholders || !is_placeholder(n)) {
        for image in &images {
            if let Some((placed, address)) = image.map.symbol(name) {
                let (_, bytes) = extent(&image.elf, placed, address, Some(name), unit.bytes.len() * 3 / 4);
                if bytes.is_empty() {
                    continue;
                }
                let found = Unit {
                    image: image.name.clone(),
                    name: name.clone(),
                    names: names_at(&image.elf, placed, address),
                    address,
                    arm: is_arm(&image.elf, placed, address),
                    bytes,
                };
                return Some((found, origin_of(root, placed)));
            }
        }
    }
    None
}

/// A copy found in one edition: the unit, how it was found, and its origin.
type Found = (Unit, &'static str, &'static str);

fn locate(edition: &Edition, reference: &Unit, placeholders: bool, root: &Path) -> Option<Found> {
    by_name(edition, reference, placeholders, root)
        .map(|(u, origin)| (u, "name", origin))
        .or_else(|| by_bytes(edition, reference, root).map(|(u, origin)| (u, "bytes", origin)))
}

#[derive(Debug, PartialEq, Eq)]
pub struct Verdict {
    pub class: &'static str,
    pub instructions: usize,
    pub detail: String,
}

fn thumb_tokens(bytes: &[u8], base: u32) -> (Vec<String>, Vec<show::Line>) {
    let lines = show::thumb_lines(bytes, base, base, base + bytes.len() as u32, &show::Names::new());
    let mut text = String::new();
    for line in &lines {
        let masked = match line.text.split_once(", =") {
            Some((head, _)) if line.text.starts_with("ldr") => format!("{head}, =pool"),
            _ => line.text.clone(),
        };
        let _ = writeln!(text, "{:x}:\t{masked}", line.address);
    }
    (diff::normalize(&text), lines)
}

fn arm_word(bytes: &[u8], i: usize) -> u32 {
    u32::from_le_bytes([bytes[i * 4], bytes[i * 4 + 1], bytes[i * 4 + 2], bytes[i * 4 + 3]])
}

/// ARM code compares word by word: pool words a pc-relative load reads are
/// dropped and calls lose their targets; branches inside stay relative.
fn arm_tokens(bytes: &[u8]) -> (Vec<String>, BTreeSet<usize>) {
    let count = bytes.len() / 4;
    let mut pool = BTreeSet::new();
    for i in 0..count {
        let w = arm_word(bytes, i);
        if w & 0x0f3f_0000 == 0x051f_0000 {
            let offset = (w & 0xfff) as i64;
            let target = i as i64 * 4 + 8 + if w & (1 << 23) != 0 { offset } else { -offset };
            if target >= 0 && target % 4 == 0 && (target / 4) < count as i64 {
                pool.insert((target / 4) as usize);
            }
        }
    }
    let tokens = (0..count)
        .filter(|i| !pool.contains(i))
        .map(|i| {
            let w = arm_word(bytes, i);
            if w & 0x0f00_0000 == 0x0b00_0000 {
                format!("bl{:x}", w >> 28)
            } else {
                format!("{w:08x}")
            }
        })
        .collect();
    (tokens, pool)
}

pub fn compare(english: &[u8], english_base: u32, other: &[u8], other_base: u32, arm: bool) -> Verdict {
    if english == other {
        return Verdict { class: "identical", instructions: 0, detail: String::new() };
    }
    let (a, b, detail) = if arm {
        let (a, a_pool) = arm_tokens(english);
        let (b, _) = arm_tokens(other);
        let (mut pool, mut calls) = (0, 0);
        for i in 0..english.len().min(other.len()) / 4 {
            if arm_word(english, i) != arm_word(other, i) {
                if a_pool.contains(&i) {
                    pool += 1;
                } else {
                    calls += 1;
                }
            }
        }
        (a, b, format!("arm words; pool {pool}, other words {calls}"))
    } else {
        let (a, a_lines) = thumb_tokens(english, english_base);
        let (b, b_lines) = thumb_tokens(other, other_base);
        let (mut pool, mut calls, mut rest) = (0, 0, 0);
        for (x, y) in a_lines.iter().zip(&b_lines) {
            if x.bytes != y.bytes {
                if x.text.starts_with(".word") {
                    pool += 1;
                } else if x.text.starts_with("bl") {
                    calls += 1;
                } else {
                    rest += 1;
                }
            }
        }
        (a, b, format!("pool words {pool}, calls {calls}, other lines {rest}"))
    };
    let sizes = if english.len() == other.len() { String::new() } else { format!("; {:#x} vs {:#x} bytes", english.len(), other.len()) };
    if a == b {
        return Verdict { class: "relocated", instructions: 0, detail: format!("{detail}{sizes}") };
    }
    let (common, _) = diff::compare(&a, &b);
    let n = a.len().max(b.len()) - common;
    Verdict { class: "differs", instructions: n, detail: format!("{} vs {} instructions{sizes}", a.len(), b.len()) }
}

struct Options {
    root: PathBuf,
    out: Option<PathBuf>,
    top: usize,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut o = Options { root: PathBuf::from("."), out: None, top: 3 };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        let value = arguments.get(i + 1).ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?;
        match flag {
            "--root" => o.root = PathBuf::from(value),
            "--out" => o.out = Some(PathBuf::from(value)),
            "--top" => o.top = value.parse::<usize>().map_err(|_| format!("--top: not a number: {value}"))?.max(1),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    Ok(o)
}

fn index(edition: &Edition, root: &Path, interner: &mut Interner, wanted: &dyn Fn(Origin) -> bool) -> Vec<Function> {
    let mut all = Vec::new();
    for image in &edition.images {
        let view = Image { build: &edition.id, name: &image.name, elf: &image.elf, map: &image.text };
        all.extend(similar::functions(&view, interner, &|object| classify(root, object), wanted));
    }
    all
}

fn twin_bytes(edition: &Edition, f: &Function) -> Vec<u8> {
    edition
        .images
        .iter()
        .find(|i| i.name == f.image)
        .and_then(|i| i.elf.section_at(f.address))
        .map(|(section, offset)| section.bytes[offset..offset + (f.bytes as usize).min(section.bytes.len() - offset)].to_vec())
        .unwrap_or_default()
}

pub const HEADER: &str = "function\timage\taddress\tbytes\tisa\tedition\trelation\tmatch_image\tmatch_address\tmatch_name\tmatch_origin\tmatch_bytes\tclass\tinstructions\tdetail\n";

pub fn row(unit: &Unit, edition: &str, relation: &str, found: Option<(&Unit, &str)>, verdict: &Verdict) -> String {
    let (image, address, name, origin, bytes) = match found {
        Some((u, origin)) => (u.image.as_str(), format!("{:08x}", u.address), u.name.as_str(), origin, u.bytes.len().to_string()),
        None => ("", String::new(), "", "", String::new()),
    };
    format!(
        "{}\t{}\t{:08x}\t{}\t{}\t{edition}\t{relation}\t{image}\t{address}\t{name}\t{origin}\t{bytes}\t{}\t{}\t{}\n",
        unit.name,
        unit.image,
        unit.address,
        unit.bytes.len(),
        if unit.arm { "arm" } else { "thumb" },
        verdict.class,
        verdict.instructions,
        verdict.detail
    )
}

fn short(verdict: &Verdict) -> String {
    match verdict.class {
        "differs" => format!("differs {}", verdict.instructions),
        class => class.to_string(),
    }
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let o = parse(arguments)?;
    let out_dir = o.root.join("out");
    let tbs: Vec<Edition> = LANGUAGES.iter().map(|l| load(&out_dir.join(format!("tbs-{l}")))).collect::<Result<_, _>>()?;
    let tla: Vec<Edition> = LANGUAGES.iter().map(|l| load(&out_dir.join(format!("tla-{l}")))).collect::<Result<_, _>>()?;
    let english = &tbs[1];
    let queries = units(english, &o.root);
    let mut interner = Interner::default();
    let mut corpus: Vec<Function> = index(english, &o.root, &mut interner, &|origin| origin == Origin::C);
    for edition in &tla {
        corpus.extend(index(edition, &o.root, &mut interner, &|origin| origin != Origin::Other));
    }
    let sorted: Vec<Vec<u32>> = corpus
        .iter()
        .map(|f| {
            let mut t = f.tokens.clone();
            t.sort_unstable();
            t
        })
        .collect();
    let candidates = |build: &str, c_only: bool| -> Vec<usize> {
        (0..corpus.len())
            .filter(|&i| corpus[i].build == build && corpus[i].tokens.len() >= MIN_INSNS && (!c_only || corpus[i].origin == Origin::C))
            .collect()
    };
    let tla_candidates: Vec<Vec<usize>> = tla.iter().map(|e| candidates(&e.id, false)).collect();
    let c_candidates: Vec<usize> = candidates(&english.id, true).into_iter().chain(candidates(&tla[1].id, true)).collect();

    let mut tsv = String::from(HEADER);
    let mut summary = String::new();
    for unit in &queries {
        let _ = writeln!(summary, "{} {} {:08x} {:#x} bytes {}", unit.name, unit.image, unit.address, unit.bytes.len(), if unit.arm { "arm" } else { "thumb" });
        let mut line = String::from("  ☀️ ");
        for edition in &tbs {
            let found = locate(edition, unit, true, &o.root);
            let verdict = report(unit, edition, found.as_ref(), &mut tsv);
            let _ = write!(line, " {} {}", &edition.id[4..], short(&verdict));
            if let Some((f, "bytes", _)) = &found {
                let _ = write!(line, " (at {:08x})", f.address);
            }
            let _ = write!(line, ";");
        }
        let _ = writeln!(summary, "{line}");
        let query = unit_function(unit, &mut interner);
        let twin_in = |edition: &Edition, candidates: &[usize]| -> Option<Found> {
            let query = query.as_ref()?;
            let best = similar::nearest(query, &corpus, &sorted, candidates, 1, MAX_RATIO).into_iter().next()?;
            let f = &corpus[best.index];
            let bytes = twin_bytes(edition, f);
            let names = vec![f.name.clone()];
            Some((Unit { image: f.image.clone(), name: f.name.clone(), names, address: f.address, bytes, arm: false }, "twin", f.origin.label()))
        };
        let english_tla = by_name(&tla[1], unit, false, &o.root)
            .map(|(u, origin)| (u, "name", origin))
            .or_else(|| twin_in(&tla[1], &tla_candidates[1]))
            .or_else(|| by_bytes(&tla[1], unit, &o.root).map(|(u, origin)| (u, "bytes", origin)));
        let mut line = String::from("  ⚓️ ");
        for (edition, candidates) in tla.iter().zip(&tla_candidates) {
            let found = if edition.id == tla[1].id {
                english_tla.clone()
            } else {
                by_name(edition, unit, false, &o.root)
                    .map(|(u, origin)| (u, "name", origin))
                    .or_else(|| {
                        let (twin, _, _) = english_tla.as_ref()?;
                        locate(edition, twin, true, &o.root).map(|(u, how, origin)| (u, if how == "name" { "twin-name" } else { "twin-bytes" }, origin))
                    })
                    .or_else(|| twin_in(edition, candidates))
                    .or_else(|| by_bytes(edition, unit, &o.root).map(|(u, origin)| (u, "bytes", origin)))
            };
            let verdict = report(unit, edition, found.as_ref(), &mut tsv);
            match &found {
                Some((f, how, origin)) => {
                    let _ = write!(line, " {} {how} {} {origin} {};", &edition.id[4..], f.name, short(&verdict));
                }
                None => {
                    let _ = write!(line, " {} absent;", &edition.id[4..]);
                }
            }
        }
        let _ = writeln!(summary, "{line}");
        if let Some(query) = &query {
            let mut line = String::from("  C twins:");
            for m in similar::nearest(query, &corpus, &sorted, &c_candidates, o.top, MAX_RATIO) {
                let f = &corpus[m.index];
                let ratio = similar::ratio(m.distance, query.tokens.len(), f.tokens.len());
                let edition = if f.build == english.id { english } else { &tla[1] };
                let found = Unit { image: f.image.clone(), name: f.name.clone(), names: vec![], address: f.address, bytes: twin_bytes(edition, f), arm: false };
                let mut verdict = compare(&unit.bytes, unit.address, &found.bytes, found.address, false);
                verdict.detail = format!("ratio {ratio:.3}, {}; {}", f.source, verdict.detail);
                tsv.push_str(&row(unit, &f.build, "c-twin", Some((&found, "c")), &verdict));
                let _ = write!(line, " {} {} {ratio:.3};", f.build, f.name);
            }
            if line.ends_with(':') {
                line.push_str(" none within ratio 0.35");
            }
            let _ = writeln!(summary, "{line}");
        }
    }
    let out = o.out.clone().unwrap_or_else(|| out_dir.join("reports/editions-tbs.tsv"));
    if let Some(parent) = out.parent().filter(|p| !p.as_os_str().is_empty()) {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    fs::write(&out, tsv).map_err(|e| format!("{}: {e}", out.display()))?;
    let _ = writeln!(summary, "{} functions; {}", queries.len(), out.display());
    Ok(summary)
}

fn report(unit: &Unit, edition: &Edition, found: Option<&Found>, tsv: &mut String) -> Verdict {
    let verdict = match found {
        Some((f, _, _)) => compare(&unit.bytes, unit.address, &f.bytes, f.address, unit.arm),
        None => Verdict { class: "absent", instructions: 0, detail: String::new() },
    };
    let relation = found.map_or("none", |(_, how, _)| how);
    tsv.push_str(&row(unit, &edition.id, relation, found.map(|(f, _, origin)| (f, *origin)), &verdict));
    verdict
}

fn unit_function(unit: &Unit, interner: &mut Interner) -> Option<Function> {
    if unit.arm {
        return None;
    }
    let ins = similar::sweep(&unit.bytes, unit.address, unit.address, unit.address + unit.bytes.len() as u32);
    let tokens: Vec<u32> = ins.iter().map(|i| interner.id(similar::normalise(i))).collect();
    (tokens.len() >= MIN_INSNS).then(|| Function {
        build: "tbs-en".into(),
        image: unit.image.clone(),
        name: unit.name.clone(),
        address: unit.address,
        bytes: unit.bytes.len() as u32,
        origin: Origin::NotYetC,
        source: String::new(),
        tokens,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn halves(words: &[u16]) -> Vec<u8> {
        words.iter().flat_map(|h| h.to_le_bytes()).collect()
    }

    /// Foo: push, eight moves, a pool load, a call, a move, pop, its pool word.
    fn foo(call: u16, ret: u16, pool: u32) -> Vec<u8> {
        let mut code = halves(&[0xb510, 0x2001, 0x2102, 0x2203, 0x2304, 0x2405, 0x2506, 0x2607, 0x2708, 0x4802, 0xf000, call, ret, 0xbd10]);
        code.extend(pool.to_le_bytes());
        code
    }

    fn bar() -> Vec<u8> {
        halves(&[0xb500, 0x2002, 0xbd00, 0x0000])
    }

    fn elf(address: u32, code: &[u8], symbols: &[(&str, u32, u8)]) -> Vec<u8> {
        let mut strings = vec![0u8];
        let mut table = vec![0u8; 16];
        for (name, value, kind) in symbols {
            table.extend((strings.len() as u32).to_le_bytes());
            strings.extend(name.as_bytes());
            strings.push(0);
            table.extend(value.to_le_bytes());
            table.extend(0u32.to_le_bytes());
            table.extend([0x10 | kind, 0]);
            table.extend(1u16.to_le_bytes());
        }
        let text_at = 52;
        let table_at = text_at + code.len();
        let strings_at = table_at + table.len();
        let headers_at = (strings_at + strings.len() + 3) & !3;
        let mut file = vec![0u8; headers_at + 40 * 4];
        file[..6].copy_from_slice(b"\x7fELF\x01\x01");
        file[0x20..0x24].copy_from_slice(&(headers_at as u32).to_le_bytes());
        file[0x2e..0x30].copy_from_slice(&40u16.to_le_bytes());
        file[0x30..0x32].copy_from_slice(&4u16.to_le_bytes());
        file[text_at..table_at].copy_from_slice(code);
        file[table_at..strings_at].copy_from_slice(&table);
        file[strings_at..strings_at + strings.len()].copy_from_slice(&strings);
        let headers = [(1u32, 6u32, address, text_at, code.len(), 0u32), (2, 0, 0, table_at, table.len(), 3), (3, 0, 0, strings_at, strings.len(), 0)];
        for (i, (kind, flags, address, offset, size, link)) in headers.into_iter().enumerate() {
            let at = headers_at + (i + 1) * 40;
            for (field, value) in [(4, kind), (8, flags), (12, address), (16, offset as u32), (20, size as u32), (24, link)] {
                file[at + field..at + field + 4].copy_from_slice(&value.to_le_bytes());
            }
        }
        file
    }

    fn edition(id: &str, map: &str, address: u32, code: &[u8], symbols: &[(&str, u32, u8)]) -> Edition {
        let text = format!("Linker script and memory map\n\n{map}");
        let image = Loaded { name: id.into(), map: Map::parse(&text), text, elf: Elf::parse(&elf(address, code, symbols)).unwrap() };
        Edition { id: id.into(), images: vec![image] }
    }

    fn english() -> Edition {
        let code: Vec<u8> = foo(0xf802, 0x2001, 0x0300_1234).into_iter().chain(bar()).collect();
        let map = ".rom_main       0x0000000008000000       0x28\n \
                   */recon/tbs/raw/08000000.o(.text)\n \
                   .text          0x0000000008000000       0x20 /x/out/tbs-en/obj/recon/tbs/raw/08000000.o\n                \
                   0x0000000008000000                Foo_Run\n \
                   .text          0x0000000008000020        0x8 /x/out/tbs-en/obj/games/X/SRC/BAR.o\n                \
                   0x0000000008000020                Bar_Run\n";
        edition("tbs-en", map, 0x0800_0000, &code, &[("Foo_Run", 0x0800_0001, 2), ("Bar_Run", 0x0800_0021, 2)])
    }

    #[test]
    fn units_are_the_not_yet_c_functions_of_a_build() {
        let found = units(&english(), Path::new("/nonexistent"));
        let named: Vec<(&str, u32, usize, bool)> = found.iter().map(|u| (u.name.as_str(), u.address, u.bytes.len(), u.arm)).collect();
        assert_eq!(named, [("Foo_Run", 0x0800_0000, 0x20, false)]);

        let code: Vec<u8> = foo(0xf802, 0x2001, 0x0300_1234).into_iter().chain(bar()).collect();
        let run = edition(
            "tbs-en",
            ".rom_main       0x0000000008000000       0x28\n \
             .text          0x0000000008000000       0x28 /x/out/tbs-en/obj/recon/tbs/raw/08000000.o\n                \
             0x0000000008000000                Foo_Run\n",
            0x0800_0000,
            &code,
            &[("$t", 0x0800_0000, 0), ("$d", 0x0800_001c, 0), ("$t", 0x0800_0020, 0)],
        );
        let found = units(&run, Path::new("/nonexistent"));
        let named: Vec<(&str, usize)> = found.iter().map(|u| (u.name.as_str(), u.bytes.len())).collect();
        assert_eq!(named, [("Foo_Run", 0x20), ("sub_08000020", 8)]);
    }

    #[test]
    fn a_counterpart_is_found_by_name_then_by_unchanged_bytes() {
        let root = Path::new("/nonexistent");
        let unit = units(&english(), root).remove(0);
        let german_code: Vec<u8> = foo(0xf804, 0x2001, 0x0300_5678).into_iter().chain(bar()).collect();
        let german = edition(
            "tbs-de",
            ".rom_main       0x0000000008000040       0x28\n \
             .rom.00000040  0x0000000008000040       0x28 /x/out/tbs-de/obj/recon/tbs/de/rom.o\n                \
             0x0000000008000040                Foo_Run\n",
            0x0800_0040,
            &german_code,
            &[("Foo_Run", 0x0800_0040, 0)],
        );
        let (found, how, origin) = locate(&german, &unit, true, root).unwrap();
        assert_eq!((found.address, found.bytes.len(), how, origin), (0x0800_0040, 0x20, "name", "scaffold"));
        let verdict = compare(&unit.bytes, unit.address, &found.bytes, found.address, false);
        assert_eq!((verdict.class, verdict.detail.as_str()), ("relocated", "pool words 1, calls 1, other lines 0"));

        let japanese_code: Vec<u8> = halves(&[0x2000, 0x4770]).into_iter().chain(foo(0xf802, 0x2005, 0x0300_1234)).chain(bar()).collect();
        let japanese = edition(
            "tbs-ja",
            ".rom_main       0x0000000008000100       0x2c\n \
             .rom.00000100  0x0000000008000100       0x2c /x/out/tbs-ja/obj/recon/tbs/ja/rom.o\n",
            0x0800_0100,
            &japanese_code,
            &[],
        );
        let (found, how, origin) = locate(&japanese, &unit, true, root).unwrap();
        assert_eq!((found.name.as_str(), found.address, found.bytes.len(), how, origin), ("@08000104", 0x0800_0104, 0x20, "bytes", "scaffold"));
        let verdict = compare(&unit.bytes, unit.address, &found.bytes, found.address, false);
        assert_eq!((verdict.class, verdict.instructions), ("differs", 1));

        let elsewhere = edition("tla-en", ".rom_main       0x0000000008000000        0x8\n", 0x0800_0000, &bar(), &[]);
        assert!(locate(&elsewhere, &unit, false, root).is_none());
    }

    #[test]
    fn thumb_copies_compare_as_identical_relocated_or_differing() {
        let a = foo(0xf802, 0x2001, 0x0300_1234);
        assert_eq!(compare(&a, 0x0800_0000, &a, 0x0800_0400, false).class, "identical");
        let moved = foo(0xf810, 0x2001, 0x0300_9999);
        assert_eq!(compare(&a, 0x0800_0000, &moved, 0x0800_0400, false).class, "relocated");
        let changed = foo(0xf802, 0x2101, 0x0300_1234);
        let verdict = compare(&a, 0x0800_0000, &changed, 0x0800_0000, false);
        assert_eq!((verdict.class, verdict.instructions), ("differs", 1));
    }

    #[test]
    fn arm_copies_drop_pools_and_call_targets() {
        let words = |w: &[u32]| -> Vec<u8> { w.iter().flat_map(|w| w.to_le_bytes()).collect() };
        let a = words(&[0xe59f_0004, 0xeb00_0010, 0xe12f_ff1e, 0x0300_1234]);
        let b = words(&[0xe59f_0004, 0xeb00_0020, 0xe12f_ff1e, 0x0300_5678]);
        let verdict = compare(&a, 0x0300_0000, &b, 0x0300_0100, true);
        assert_eq!((verdict.class, verdict.detail.as_str()), ("relocated", "arm words; pool 1, other words 1"));
        let c = words(&[0xe59f_0004, 0xeb00_0010, 0xe1a0_0000, 0x0300_1234]);
        let verdict = compare(&a, 0x0300_0000, &c, 0x0300_0000, true);
        assert_eq!((verdict.class, verdict.instructions), ("differs", 1));
        assert!(anchors(&a, 0, true).is_empty());
    }

    #[test]
    fn a_thumb_function_ends_at_the_next_entry_after_a_return() {
        let code: Vec<u8> = foo(0xf802, 0x2001, 0x0300_1234).into_iter().chain(bar()).collect();
        assert_eq!(thumb_length(&code, 0x0800_0000, 0), 0x20);
        assert_eq!(thumb_length(&code, 0x0800_0000, 0x21), code.len());
        let runs = anchors(&code[..0x20], 0x0800_0000, false);
        assert_eq!(runs.iter().map(|(at, run)| (*at, run.len())).collect::<Vec<_>>(), [(0, 18)]);
    }
}
