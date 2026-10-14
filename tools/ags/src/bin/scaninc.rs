//! `scaninc OUT.mk BUILD OBJDIR [-I DIR]... [-G NAME]... -- SOURCE...` writes the
//! dependencies of each source as make rules, as pret's scaninc finds them:
//! `OBJDIR/SOURCE.o: FILE...` for every file the source reads through
//! `#include`, `.include` and `.incbin`, and the files those read in turn.
//! A path from the checkout (games/..., recon/...) is a source or an asset
//! the Makefile builds beside its source. A name found in no folder that
//! starts with a -G name is one the build writes for the edition, under
//! BUILD (text/, overlays/, baserom.gba); any other is left out, as an
//! include in an #if arm this game does not compile. Each file also gets an empty rule,
//! as `gcc -MP` writes, so a header taken away does not stop make.

use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::Path;
use std::process::ExitCode;

struct Scan<'a> {
    build: &'a str,
    folders: Vec<&'a str>,
    generated: Vec<&'a str>,
    read: BTreeMap<String, Vec<String>>,
}

/// The names a file reads, with whether each is a C include.
fn named(text: &str) -> Vec<(String, bool)> {
    let mut names = Vec::new();
    for line in text.lines() {
        let line = line.trim_start();
        let (rest, c) = if let Some(rest) = line.strip_prefix('#') {
            match rest.trim_start().strip_prefix("include") {
                Some(rest) => (rest, true),
                None => continue,
            }
        } else if let Some(rest) = line.strip_prefix(".include").or_else(|| line.strip_prefix(".incbin")) {
            (rest, false)
        } else {
            continue;
        };
        let rest = rest.trim_start();
        let close = match rest.chars().next() {
            Some('"') => '"',
            Some('<') => '>',
            _ => continue,
        };
        if let Some(name) = rest[1..].split(close).next() {
            names.push((name.to_owned(), c));
        }
    }
    names
}

fn readable(path: &str) -> bool {
    matches!(Path::new(path).extension().and_then(|extension| extension.to_str()), Some("c" | "h" | "s" | "S" | "inc"))
}

impl Scan<'_> {
    /// Where a name resolves: beside the file that names it (C only), from
    /// the checkout, in an include folder, or else in the build.
    fn resolve(&self, from: &str, name: &str, c: bool) -> Option<String> {
        if name.starts_with("games/") || name.starts_with("recon/") {
            return Some(name.to_owned());
        }
        if c {
            if let Some(folder) = Path::new(from).parent() {
                let beside = folder.join(name);
                if beside.is_file() {
                    return Some(beside.to_string_lossy().into_owned());
                }
            }
        }
        for folder in &self.folders {
            let path = format!("{folder}/{name}");
            if Path::new(&path).is_file() {
                return Some(path);
            }
        }
        self.generated
            .iter()
            .any(|prefix| name.starts_with(prefix))
            .then(|| format!("{}/{name}", self.build))
    }

    fn files(&mut self, path: &str) -> Vec<String> {
        if let Some(known) = self.read.get(path) {
            return known.clone();
        }
        self.read.insert(path.to_owned(), Vec::new());
        let text = fs::read(path).map(|bytes| String::from_utf8_lossy(&bytes).into_owned()).unwrap_or_default();
        let mut found = Vec::new();
        for (name, c) in named(&text) {
            let Some(resolved) = self.resolve(path, &name, c) else { continue };
            found.push(resolved.clone());
            if readable(&resolved) && Path::new(&resolved).is_file() {
                found.extend(self.files(&resolved));
            }
        }
        found.sort();
        found.dedup();
        self.read.insert(path.to_owned(), found.clone());
        found
    }
}

fn run(args: &[String]) -> Result<(), String> {
    let usage = "usage: scaninc OUT.mk BUILD OBJDIR [-I DIR]... [-G NAME]... -- SOURCE...";
    let [output, build, objects, rest @ ..] = args else {
        return Err(usage.into());
    };
    let split = rest.iter().position(|arg| arg == "--").ok_or(usage)?;
    let (mut folders, mut generated) = (Vec::new(), Vec::new());
    let mut options = rest[..split].iter();
    while let Some(option) = options.next() {
        let list = match &option[..option.len().min(2)] {
            "-I" => &mut folders,
            "-G" => &mut generated,
            _ => return Err(usage.into()),
        };
        match &option[2..] {
            "" => list.push(options.next().ok_or(usage)?.as_str()),
            value => list.push(value),
        }
    }
    let mut scan = Scan { build, folders, generated, read: BTreeMap::new() };
    let mut text = String::from("# Written by scaninc: do not edit.\n");
    let mut every = BTreeSet::new();
    for source in &rest[split + 1..] {
        let stem = source.rsplit_once('.').map_or(source.as_str(), |(stem, _)| stem);
        let files = scan.files(source);
        text += &format!("{objects}/{stem}.o:");
        for file in &files {
            text += &format!(" {file}");
        }
        text += "\n";
        every.extend(files);
    }
    for file in every.iter().filter(|file| !file.starts_with(build.as_str())) {
        text += &format!("{file}:\n");
    }
    fs::write(output, text).map_err(|error| format!("{output}: {error}"))
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    match run(&args) {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("scaninc: {error}");
            ExitCode::FAILURE
        }
    }
}
