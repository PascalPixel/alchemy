
use crate::deps::{self, Blocker, Image, Index, Key, Symbol, Target};
use crate::elf::Elf;
use crate::ld_map::{self, Map};
use crate::similar::{self, Origin};
use std::collections::{BTreeMap, BTreeSet};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

const USAGE: &str =
    "usage: alchemy deps --build DIR [--build DIR]... --out-dir DIR [--root DIR]\n\
Maps calls and references of every function in each build directory's linked\n\
images (DIR/*.elf and DIR/overlays/*.elf with their maps), the recon listings\n\
(ROOT/recon/GAME/raw) and drafts (ROOT/recon/GAME/LANG), for a build named\n\
GAME-LANG. Writes deps-BUILD-VIEW.tsv into the output directory, one per view:\n\
  functions  every function with its callers, callees and blockers\n\
  calls      call edges (bl, far-call and overlay veneers, pointers)\n\
  references pool words and ids of not-yet-C functions, classified\n\
  blockers   unresolved names by not-yet-C bytes blocked\n\
  frontier   not-yet-C functions with nothing left to name\n\
  drafts     drafts with their score and what is left\n\
ARM listings are inventoried with their full extent and an explicit blocker;\n\
their calls and references are not yet decoded.";

// ARM listings are still work even though the dependency decoder is Thumb-only.
// A raw ARM object is one listing/run: retain its complete linked extent,
// including literal pools and a possible Thumb entry veneer, without decoding
// its ARM words as Thumb instructions or claiming it has no dependencies.
fn inventory_arm(
    image: &similar::Image,
    classify: &dyn Fn(&str) -> (Origin, String),
    functions: &mut Vec<(similar::Function, Vec<crate::decode::Ins>)>,
) -> BTreeSet<u32> {
    let mut undecoded = BTreeSet::new();
    for section in Map::parse(image.map).placed {
        if section.size == 0 || !(section.name == ".text" || section.name.starts_with(".text.")) {
            continue;
        }
        let (origin, source) = classify(&section.path);
        if origin != Origin::NotYetC || !image.elf.symbols.iter().any(|symbol| {
            symbol.section != 0 && symbol.section != 0xfff1
                && (symbol.name == "$a" || symbol.name.starts_with("$a."))
                && (section.address..section.end()).contains(&symbol.value)
        }) {
            continue;
        }
        let Some(name) = ld_map::preferred(&section.symbols, section.address) else {
            continue;
        };
        functions.retain(|(function, _)| !(section.address..section.end()).contains(&function.address));
        functions.push((similar::Function {
            build: image.build.to_string(),
            image: image.name.to_string(),
            name: name.to_string(),
            address: section.address,
            bytes: section.size,
            origin,
            source,
            tokens: Vec::new(),
        }, Vec::new()));
        undecoded.insert(section.address);
    }
    undecoded
}

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
            name: &stem,
            elf: &elf,
            map: &map,
        };
        let mut functions = similar::decoded(&image, &|o| classify(root, o), &|_| true);
        let undecoded_arm = inventory_arm(&image, &|o| classify(root, o), &mut functions);
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
            main: elf_path.parent() == Some(build),
            name: stem,
            functions,
            symbols,
            undecoded_arm,
        });
    }
    Ok(images)
}

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
    let mut dirs = vec![base.clone(), base.join("main"), base.join("overlays")];
    if let Ok(entries) = fs::read_dir(base.join("overlays")) {
        dirs.extend(entries.flatten().map(|e| e.path()).filter(|p| p.is_dir()));
    }
    for dir in dirs {
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
    let not_yet_c: BTreeMap<(&str, &str), Key> = images
        .iter()
        .enumerate()
        .flat_map(|(ii, image)| {
            image
                .functions
                .iter()
                .enumerate()
                .filter(|(_, (f, _))| f.origin == Origin::NotYetC)
                .map(move |(fi, (f, _))| ((if image.main { "" } else { image.name.as_str() }, f.name.as_str()), (ii, fi)))
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
        let place = path.strip_prefix(&base).map(|p| p.to_string_lossy().into_owned()).unwrap_or_default();
        let overlay = place
            .find("resource_")
            .and_then(|at| place.get(at + 9..at + 12))
            .filter(|id| id.chars().all(|c| c.is_ascii_hexdigit()))
            .map(|id| format!("resource_{id}"));
        let image = overlay.as_deref().unwrap_or("");
        let mut key = hex8(&file).last().and_then(|&a| {
            at(image, a).or_else(|| (!image.is_empty()).then(|| at(image, a + 0x8000)).flatten())
        });
        if key.is_none() {
            key = text
                .split(|c: char| !(c.is_ascii_alphanumeric() || c == '_'))
                .find_map(|w| {
                    not_yet_c
                        .get(&(image, w))
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
    let undecoded_arm: usize = images.iter().map(|image| image.undecoded_arm.len()).sum();
    let blocking = ranked.iter().filter(|(b, _, _)| b.blocks()).count();
    Ok(format!(
        "{name}: {} functions, {} calls, {not_yet_c} not yet C, {blocking} blockers, {} on the frontier, {} drafts; {undecoded_arm} ARM listings await dependency decoding; {}\n",
        images.iter().map(|i| i.functions.len()).sum::<usize>(),
        index.calls.len(),
        frontier_keys.len(),
        drafts.len(),
        out_dir.display()
    ))
}

fn run(arguments: &[String]) -> Result<String, String> {
    let o = parse(arguments)?;
    let mut text = String::new();
    for build in &o.builds {
        text.push_str(&report(&o, build, &o.out_dir)?);
    }
    Ok(text)
}
fn classify(root: &Path, object: &str) -> (Origin, String) {
    let file = object.rsplit('/').next().unwrap_or(object);
    let Some(at) = object.find("/obj/") else {
        if file.ends_with("_overlay.o") {
            return (Origin::NotYetC, file.to_string());
        }
        return (Origin::Other, file.to_string());
    };
    let relative = &object[at + 5..];
    let stem = relative.strip_suffix(".o").unwrap_or(relative);
    let existing = |extensions: &[&str]| {
        extensions
            .iter()
            .map(|e| format!("{stem}{e}"))
            .find(|p| root.join(p).is_file())
    };
    if relative.starts_with("games/") {
        if let Some(path) = existing(&[".c"]) {
            return (Origin::C, path);
        }
        if let Some(path) = existing(&[".S", ".s"]) {
            return (Origin::Assembly, path);
        }
        return (Origin::Other, relative.to_string());
    }
    if relative.starts_with("recon/") && relative.contains("/raw/") {
        let path = existing(&[".s", ".S"]).unwrap_or_else(|| relative.to_string());
        return (Origin::NotYetC, path);
    }
    (Origin::Other, relative.to_string())
}

fn images_in(build: &Path) -> Result<Vec<PathBuf>, String> {
    let mut elfs = Vec::new();
    for dir in [build.to_path_buf(), build.join("overlays")] {
        let Ok(entries) = fs::read_dir(&dir) else {
            continue;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.extension().is_some_and(|e| e == "map") && path.with_extension("elf").is_file()
            {
                elfs.push(path.with_extension("elf"));
            }
        }
    }
    elfs.sort();
    if elfs.is_empty() {
        return Err(format!("{}: no linked image with a map", build.display()));
    }
    Ok(elfs)
}

pub fn entry(rest: &[String]) -> std::process::ExitCode {
    if rest == ["--help"] || rest == ["-h"] {
        println!("{USAGE}");
        return std::process::ExitCode::SUCCESS;
    }
    match run(rest) {
        Ok(text) => {
            print!("{text}");
            std::process::ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("{error}");
            std::process::ExitCode::from(2)
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::elf::{Section, Symbol as ElfSymbol};

    fn symbol(name: &str, value: u32, kind: u8) -> ElfSymbol {
        ElfSymbol { name: name.into(), value, size: 0, kind, section: 1 }
    }

    #[test]
    fn arm_inventory_keeps_pools_and_aliases_without_inventing_edges_or_frontier_work() {
        let base = 0x0800_0100;
        let mut bytes = vec![0; 0x30];
        // The real Thumb call reaches ThumbCallee. The same halfwords in the
        // ARM routine must never become a second, invented Thumb call.
        for offset in [0, 0x20] {
            bytes[offset..offset + 8].copy_from_slice(&[0x00, 0xf0, 0x06, 0xf8, 0x70, 0x47, 0xc0, 0x46]);
        }
        bytes[0x10..0x14].copy_from_slice(&[0x70, 0x47, 0xc0, 0x46]);
        let elf = Elf {
            sections: vec![Section { address: base, bytes }],
            symbols: vec![
                symbol("ThumbCaller", base + 1, 2),
                symbol("ThumbCallee", base + 0x11, 2),
                symbol("Func_08000120", base + 0x20, 0),
                symbol("ArmWorker", base + 0x20, 0),
                symbol("Data_08000120", base + 0x20, 0),
                symbol("ArmWorkerEnd", base + 0x2c, 0),
                symbol("PoolOnly", base + 0x2c, 0),
                symbol("$a.1", base + 0x20, 0),
                symbol("$d", base + 0x28, 0),
                symbol("$d.1", base + 0x2c, 0),
            ],
        };
        let map = "Linker script and memory map\n\
            .text 0x08000100 0x30\n\
            \x20.text 0x08000100 0x8 /out/obj/recon/tbs/raw/08000100.o\n\
            \x20                0x08000100 ThumbCaller\n\
            \x20.text 0x08000110 0x4 /out/obj/games/x/thumb.o\n\
            \x20                0x08000110 ThumbCallee\n\
            \x20.text 0x08000120 0xc /out/obj/recon/tbs/raw/08000120.o\n\
            \x20                0x08000120 Func_08000120\n\
            \x20                0x08000120 ArmWorker\n\
            \x20                0x08000120 Data_08000120\n\
            \x20                0x0800012c ArmWorkerEnd\n\
            \x20.text 0x0800012c 0x4 /out/obj/recon/tbs/raw/0800012c.o\n\
            \x20                0x0800012c PoolOnly\n";
        let image = similar::Image { build: "tbs-en", name: "tbs-en", elf: &elf, map };
        let classify = |object: &str| {
            (if object.contains("/raw/") { Origin::NotYetC } else { Origin::C }, object.to_string())
        };
        let mut functions = similar::decoded(&image, &classify, &|_| true);
        let original = functions.clone();
        let undecoded_arm = inventory_arm(&image, &classify, &mut functions);
        let arm = functions.iter().find(|(function, _)| function.name == "ArmWorker").unwrap();
        assert_eq!((arm.0.address, arm.0.bytes), (base + 0x20, 12));
        assert!(arm.1.is_empty());
        assert_eq!(functions.len(), original.len() + 1);
        assert!(!functions.iter().any(|(function, _)| function.name == "PoolOnly"));
        let images = vec![Image { name: "tbs-en".into(), main: true, functions, symbols: vec![], undecoded_arm }];
        let index = Index::build(&images, &BTreeMap::new());
        assert_eq!(index.calls.len(), 1);
        let call = &index.calls[0];
        assert_eq!(index.function(call.caller).name, "ThumbCaller");
        let Target::Function(callee) = call.callee else { panic!("the Thumb target must resolve") };
        assert_eq!(index.function(callee).name, "ThumbCallee");
        let arm_key = (0, original.len());
        assert_eq!(index.blockers[&arm_key].iter().next().unwrap().kind, "arm-dependencies-not-decoded");
        assert!(!index.frontier().contains(&arm_key));
    }

    #[test]
    fn a_thumb_entry_into_arm_is_one_undecoded_listing() {
        let base = 0x0800_0100;
        let elf = Elf {
            sections: vec![Section { address: base, bytes: vec![0x78, 0x47, 0x00, 0x02, 0x00, 0xf0, 0x00, 0xf8] }],
            symbols: vec![
                symbol("ArmKernel", base + 1, 2),
                symbol("$t", base, 0),
                symbol("$a", base + 4, 0),
            ],
        };
        let map = "Linker script and memory map\n\
            .text 0x08000100 0x8\n\
            \x20.text 0x08000100 0x8 /out/obj/recon/tla/raw/08000100.o\n\
            \x20                0x08000100 ArmKernel\n";
        let image = similar::Image { build: "tla-en", name: "tla-en", elf: &elf, map };
        let classify = |object: &str| (Origin::NotYetC, object.to_string());
        let mut functions = similar::decoded(&image, &classify, &|_| true);
        assert!(!functions[0].1.is_empty());
        let undecoded = inventory_arm(&image, &classify, &mut functions);
        assert_eq!(undecoded, BTreeSet::from([base]));
        assert_eq!(functions.len(), 1);
        assert_eq!((functions[0].0.name.as_str(), functions[0].0.bytes), ("ArmKernel", 8));
        assert!(functions[0].1.is_empty());
    }
}
