//! `psynergy similar`: rank functions of linked builds by the edit distance
//! between their normalised instructions. The index is rebuilt from the
//! builds' ELF images and linker maps on every run.

use psynergy::elf::Elf;
use psynergy::similar::{self, Function, Image, Interner, Origin};
use std::fmt::Write as _;
use std::fs;
use std::path::{Path, PathBuf};

pub const USAGE: &str =
    "usage: psynergy similar --build DIR [--build DIR]... [options] [FUNCTION]\n\
Indexes every function of each build directory's linked images (DIR/*.elf and\n\
DIR/overlays/*.elf, each with its .map beside it) and ranks the nearest ones.\n\
FUNCTION is a symbol name or a hexadecimal address; without it, --out writes\n\
the best matches of every --from function as TSV.\n\
  --root DIR          where object paths' sources are looked up (default .)\n\
  --from ORIGINS      batch queries: c, assembly, not-yet-c, other (default not-yet-c)\n\
  --to ORIGINS        candidates, comma separated (default c)\n\
  --from-build NAME   batch queries only from this build (a directory name)\n\
  --to-build NAME     candidates only from this build\n\
  --top N             matches per function (default 5)\n\
  --max-ratio F       largest distance / longer length to report (default 0.35)\n\
  --min-insns N       ignore functions shorter than this (default 8)\n\
  --out FILE          write the batch TSV here";

struct Options {
    builds: Vec<PathBuf>,
    root: PathBuf,
    from: Vec<Origin>,
    to: Vec<Origin>,
    from_build: Option<String>,
    to_build: Option<String>,
    top: usize,
    max_ratio: f64,
    min_insns: usize,
    out: Option<PathBuf>,
    query: Option<String>,
}

fn origins(text: &str) -> Result<Vec<Origin>, String> {
    text.split(',')
        .map(|o| Origin::parse(o).ok_or_else(|| format!("unknown origin: {o}")))
        .collect()
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut o = Options {
        builds: Vec::new(),
        root: PathBuf::from("."),
        from: vec![Origin::NotYetC],
        to: vec![Origin::C],
        from_build: None,
        to_build: None,
        top: 5,
        max_ratio: 0.35,
        min_insns: 8,
        out: None,
        query: None,
    };
    let mut i = 0;
    while i < arguments.len() {
        let flag = arguments[i].as_str();
        if !flag.starts_with("--") {
            if o.query.replace(flag.to_string()).is_some() {
                return Err(format!("more than one function given\n{USAGE}"));
            }
            i += 1;
            continue;
        }
        let value = arguments
            .get(i + 1)
            .ok_or_else(|| format!("{flag} needs a value"))?
            .as_str();
        let number = |v: &str| {
            v.parse::<usize>()
                .map_err(|_| format!("{flag}: not a number: {v}"))
        };
        match flag {
            "--build" => o.builds.push(PathBuf::from(value)),
            "--root" => o.root = PathBuf::from(value),
            "--from" => o.from = origins(value)?,
            "--to" => o.to = origins(value)?,
            "--from-build" => o.from_build = Some(value.to_string()),
            "--to-build" => o.to_build = Some(value.to_string()),
            "--top" => o.top = number(value)?.max(1),
            "--min-insns" => o.min_insns = number(value)?,
            "--max-ratio" => {
                o.max_ratio = value
                    .parse::<f64>()
                    .map_err(|_| format!("--max-ratio: not a number: {value}"))?
            }
            "--out" => o.out = Some(PathBuf::from(value)),
            _ => return Err(format!("unknown option {flag}\n{USAGE}")),
        }
        i += 2;
    }
    if o.builds.is_empty() {
        return Err(format!("at least one --build is required\n{USAGE}"));
    }
    if o.query.is_none() && o.out.is_none() {
        return Err(format!("give a FUNCTION or --out\n{USAGE}"));
    }
    Ok(o)
}

/// The origin and reported source of an object path the linker map names.
pub fn classify(root: &Path, object: &str) -> (Origin, String) {
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
        if let Some(path) = existing(&[".C", ".c"]) {
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

pub fn images_in(build: &Path) -> Result<Vec<PathBuf>, String> {
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

fn index(o: &Options) -> Result<Vec<Function>, String> {
    let mut interner = Interner::default();
    let mut all = Vec::new();
    for build in &o.builds {
        let name = build
            .file_name()
            .map(|n| n.to_string_lossy().into_owned())
            .unwrap_or_else(|| build.display().to_string());
        for elf_path in images_in(build)? {
            let data = fs::read(&elf_path).map_err(|e| format!("{}: {e}", elf_path.display()))?;
            let elf = Elf::parse(&data).map_err(|e| format!("{}: {e}", elf_path.display()))?;
            let map_path = elf_path.with_extension("map");
            let map = fs::read_to_string(&map_path)
                .map_err(|e| format!("{}: {e}", map_path.display()))?;
            let stem = elf_path
                .file_stem()
                .map(|s| s.to_string_lossy().into_owned())
                .unwrap_or_default();
            let image = Image {
                build: &name,
                name: &stem,
                elf: &elf,
                map: &map,
            };
            let wanted = |origin: Origin| {
                (o.query.is_some() && origin != Origin::Other)
                    || o.from.contains(&origin)
                    || o.to.contains(&origin)
            };
            all.extend(similar::functions(
                &image,
                &mut interner,
                &|object| classify(&o.root, object),
                &wanted,
            ));
        }
    }
    Ok(unique(all))
}

/// One function per place: a build's overlays share load addresses, so a
/// place is its build, its image and its address, and twins in different
/// overlays stay apart.
fn unique(mut all: Vec<Function>) -> Vec<Function> {
    all.sort_by(|a, b| {
        (&a.build, &a.image, a.address, &a.name).cmp(&(&b.build, &b.image, b.address, &b.name))
    });
    all.dedup_by(|a, b| a.build == b.build && a.image == b.image && a.address == b.address);
    all
}

fn row(f: &Function) -> String {
    format!(
        "{}\t{}\t{:08x}\t{}\t{}\t{}\t{}\t{}",
        f.build,
        f.image,
        f.address,
        f.name,
        f.origin.label(),
        f.bytes,
        f.tokens.len(),
        f.source
    )
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let o = parse(arguments)?;
    let corpus = index(&o)?;
    let sorted: Vec<Vec<u32>> = corpus
        .iter()
        .map(|f| {
            let mut t = f.tokens.clone();
            t.sort_unstable();
            t
        })
        .collect();
    let candidates: Vec<usize> = (0..corpus.len())
        .filter(|&i| {
            let f = &corpus[i];
            o.to.contains(&f.origin)
                && f.tokens.len() >= o.min_insns
                && o.to_build.as_ref().is_none_or(|b| &f.build == b)
        })
        .collect();
    if let Some(query) = &o.query {
        let address = u32::from_str_radix(query.trim_start_matches("0x"), 16).ok();
        let queries: Vec<&Function> = corpus
            .iter()
            .filter(|f| &f.name == query || Some(f.address) == address)
            .collect();
        if queries.is_empty() {
            return Err(format!("no indexed function is {query}"));
        }
        let mut text = String::new();
        for q in queries {
            let _ = writeln!(text, "{}", row(q));
            for m in similar::nearest(q, &corpus, &sorted, &candidates, o.top, o.max_ratio) {
                let f = &corpus[m.index];
                let _ = writeln!(
                    text,
                    "  {:>4} {:.3}  {}",
                    m.distance,
                    similar::ratio(m.distance, q.tokens.len(), f.tokens.len()),
                    row(f)
                );
            }
        }
        return Ok(text);
    }
    let queries: Vec<usize> = (0..corpus.len())
        .filter(|&i| {
            let f = &corpus[i];
            o.from.contains(&f.origin)
                && f.tokens.len() >= o.min_insns
                && o.from_build.as_ref().is_none_or(|b| &f.build == b)
        })
        .collect();
    let threads = std::thread::available_parallelism().map_or(4, |n| n.get());
    let chunk = queries.len().div_ceil(threads).max(1);
    let mut rows: Vec<(f64, u32, String)> = std::thread::scope(|scope| {
        let handles: Vec<_> = queries
            .chunks(chunk)
            .map(|part| {
                let (corpus, sorted, candidates, o) = (&corpus, &sorted, &candidates, &o);
                scope.spawn(move || {
                    let mut rows = Vec::new();
                    for &qi in part {
                        let q = &corpus[qi];
                        for m in similar::nearest(q, corpus, sorted, candidates, o.top, o.max_ratio)
                        {
                            let f = &corpus[m.index];
                            let ratio = similar::ratio(m.distance, q.tokens.len(), f.tokens.len());
                            rows.push((
                                ratio,
                                q.bytes,
                                format!("{}\t{}\t{ratio:.3}\t{}", row(q), m.distance, row(f)),
                            ));
                        }
                    }
                    rows
                })
            })
            .collect();
        handles
            .into_iter()
            .flat_map(|h| h.join().unwrap())
            .collect()
    });
    rows.sort_by(|a, b| a.0.total_cmp(&b.0).then(b.1.cmp(&a.1)).then(a.2.cmp(&b.2)));
    let mut tsv = String::from(
        "build\timage\taddress\tname\torigin\tbytes\tinsns\tsource\tdistance\tratio\tmatch_build\tmatch_image\tmatch_address\tmatch_name\tmatch_origin\tmatch_bytes\tmatch_insns\tmatch_source\n",
    );
    for (_, _, line) in &rows {
        tsv.push_str(line);
        tsv.push('\n');
    }
    let out = o.out.as_ref().expect("batch mode has --out");
    if let Some(parent) = out.parent().filter(|parent| !parent.as_os_str().is_empty()) {
        fs::create_dir_all(parent).map_err(|e| format!("{}: {e}", parent.display()))?;
    }
    fs::write(out, tsv).map_err(|e| format!("{}: {e}", out.display()))?;
    Ok(format!(
        "indexed {} functions; {} queries, {} candidates; {} rows in {}\n",
        corpus.len(),
        queries.len(),
        candidates.len(),
        rows.len(),
        out.display()
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn function(image: &str, address: u32) -> Function {
        Function {
            build: "tla-en".into(),
            image: image.into(),
            name: format!("Func_{address:08x}"),
            address,
            bytes: 4,
            origin: Origin::NotYetC,
            source: String::new(),
            tokens: vec![1, 2],
        }
    }

    #[test]
    fn overlays_sharing_a_load_address_keep_every_function() {
        let all = vec![
            function("resource_64a", 0x0200_8000),
            function("resource_649", 0x0200_8000),
            function("resource_649", 0x0200_8000),
            function("tla-en", 0x0800_1000),
        ];
        let places: Vec<(String, u32)> = unique(all)
            .into_iter()
            .map(|f| (f.image, f.address))
            .collect();
        assert_eq!(
            places,
            vec![
                ("resource_649".into(), 0x0200_8000),
                ("resource_64a".into(), 0x0200_8000),
                ("tla-en".into(), 0x0800_1000),
            ]
        );
    }
}
