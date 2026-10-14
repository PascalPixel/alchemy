//! alchemy rename: one name written anew wherever the tree spells it (S3),
//! once no other branch has given the new name (W7).
//!
//! A name of the games lives in many kinds of file: the C that defines and
//! calls it, its header's prototype, the labels and calls of assembly, the
//! veneers of the entry and import tables and their rows in overlay.inc,
//! the linker scripts, ⚓️'s recon/ scaffolds, the comments that cite it
//! (PACKER rows cite `NAME+0x..`), a PO catalog's msgctxt, and
//! tools/alchemy/aliases.tsv. A far-call bank's entry for NAME is NAMEFar,
//! and it takes the new name with Far. The bytes judge the result: the
//! tool prints the files it wrote, and `alchemy keep` builds all twelve
//! editions.

use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

const USAGE: &str = r#"usage: alchemy rename OLD NEW [--file FILE.c] [--dry-run]
Writes NEW wherever the tree spells OLD as a whole word: C and headers,
assembly labels and calls, the entry and import tables' veneers, the rows
of overlay.inc, linker scripts, recon/ scaffolds, comments (PACKER rows
included), a PO catalog's msgctxt and tools/alchemy/aliases.tsv. A line
that names a file (#include, .include, .incbin, INCLUDE) keeps its words.
OLD's far-call twin OLDFar, where the tree has one, becomes NEWFar.
--file FILE.c limits the rename to that C unit, for a static: other files
keep their OLD. It refuses a FILE that is not a C unit (a static in a
header reaches every unit that includes it), and an OLD that a file
other than a C unit spells: a header, assembly source, linker script,
catalog or aliases.tsv.
NEW is refused where those places spell it already (with --file: FILE,
or a file other than a C unit) and where a commit of any branch added or
took it away there (git log --all -S over games/, recon/ and aliases.tsv;
with --file, over FILE and the files other than C units): W7 is about
the games' names. The tools' source, CONTRIBUTING.md and README.md give no
name to the games, so a NEW they spell is noted, not refused.
Prints each file written with its count, the names that extend OLD and
keep theirs (OLDCodeSize), the files outside games/ and recon/ that spell
OLD and are left to a person (the tools' source, CONTRIBUTING.md), and each
reviewed assembly source whose evidence.tsv row the new tokens no longer
match. Then `alchemy keep`, with no PATH, judges the rename in all twelve
editions: it keeps the files when every edition stays identical. A rename
stands whole or not at all: if keep reverts one of its files, put back
the others.
--dry-run prints the same, the reviews it would cost included, and
writes nothing."#;

/// Where the tree spells the names it builds from, and the table of
/// symbols kept together beside them.
const PLACES: [&str; 3] = ["games", "recon", "tools/alchemy/aliases.tsv"];

/// What spells names the tool does not write: the tools' own source, and
/// the rules and the fans' page, whose wording is Pascal's.
const ELSEWHERE: [&str; 3] = ["tools/alchemy/src", "CONTRIBUTING.md", "README.md"];

/// The suffix of a far-call bank's entry for a routine.
const FAR: &str = "Far";

/// How a file spells names.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
enum Kind {
    /// C, assembly, linker scripts and tables: any whole word.
    Words,
    /// A PO catalog: the message's symbol, its `msgctxt` line.
    Catalog,
}

fn kind(path: &str) -> Option<Kind> {
    let (_, extension) = path.rsplit_once('.')?;
    match extension {
        "c" | "h" | "s" | "S" | "inc" | "ld" => Some(Kind::Words),
        "po" => Some(Kind::Catalog),
        "tsv" if PLACES.contains(&path) => Some(Kind::Words),
        _ => None,
    }
}

/// Whether a file declares or defines for others: anything but a C unit.
fn shared(path: &str) -> bool {
    !path.ends_with(".c")
}

/// The files `shared` holds among those `kind` reads, as git pathspecs
/// (whose `*` crosses folders): the history a static's NEW is checked
/// against covers the files the tree check reads.
const SHARED: [&str; 13] = [
    "games/*.h",
    "games/*.s",
    "games/*.S",
    "games/*.inc",
    "games/*.ld",
    "games/*.po",
    "recon/*.h",
    "recon/*.s",
    "recon/*.S",
    "recon/*.inc",
    "recon/*.ld",
    "recon/*.po",
    "tools/alchemy/aliases.tsv",
];

fn is_identifier(word: &str) -> bool {
    let mut bytes = word.bytes();
    bytes.next().is_some_and(|first| first.is_ascii_alphabetic() || first == b'_')
        && bytes.all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
}

fn word_byte(byte: u8) -> bool {
    byte.is_ascii_alphanumeric() || byte == b'_'
}

/// Whether a line names a file rather than a symbol.
fn names_a_file(line: &str) -> bool {
    let line = line.trim_start();
    let directive = line.strip_prefix('#').map(str::trim_start);
    directive.is_some_and(|rest| rest.starts_with("include"))
        || [".include", ".incbin", "INCLUDE"].iter().any(|word| {
            line.strip_prefix(word).is_some_and(|rest| rest.starts_with(|c: char| c.is_whitespace() || c == '"'))
        })
}

/// Each whole word of a text, with where it starts, on the lines a file of
/// its kind spells names on.
fn words(text: &str, kind: Kind) -> Vec<(usize, &str)> {
    let mut found = Vec::new();
    let mut start = 0;
    for line in text.split_inclusive('\n') {
        let read = !names_a_file(line)
            && (kind == Kind::Words || line.trim_start().starts_with("msgctxt"));
        if read {
            let bytes = line.as_bytes();
            let mut i = 0;
            while i < bytes.len() {
                if !word_byte(bytes[i]) {
                    i += 1;
                    continue;
                }
                let end = (i..bytes.len()).find(|&k| !word_byte(bytes[k])).unwrap_or(bytes.len());
                found.push((start + i, &line[i..end]));
                i = end;
            }
        }
        start += line.len();
    }
    found
}

/// A text with each whole word `names` maps written as its new one, and
/// how many it wrote.
fn renamed(text: &str, kind: Kind, names: &BTreeMap<String, String>) -> (String, usize) {
    let mut out = String::with_capacity(text.len() + 64);
    let (mut copied, mut count) = (0, 0);
    for (at, word) in words(text, kind) {
        if let Some(new) = names.get(word) {
            out.push_str(&text[copied..at]);
            out.push_str(new);
            copied = at + word.len();
            count += 1;
        }
    }
    out.push_str(&text[copied..]);
    (out, count)
}

/// Whether a line spells a word whole.
fn spells(line: &str, word: &str) -> bool {
    line.match_indices(word).any(|(at, _)| {
        let bytes = line.as_bytes();
        (at == 0 || !word_byte(bytes[at - 1])) && bytes.get(at + word.len()).is_none_or(|next| !word_byte(*next))
    })
}

/// The paths whose history W7 reads for a new name: where the games' names
/// are given (PLACES), or with --file, FILE and the files the tree check
/// reads beside it (SHARED). The tools' source and its test fixtures give
/// no name to the games.
fn history(file: Option<&str>) -> Vec<&str> {
    match file {
        Some(file) => std::iter::once(file).chain(SHARED).collect(),
        None => PLACES.to_vec(),
    }
}

/// The first commit of a branch whose diff adds or takes away one of
/// `words` whole in `paths`, with the word: `git log --all -S` lists the
/// commits that change how often a file holds the first word as text,
/// which every word that holds it does, and their diffs are read for the
/// words whole.
fn given(root: &Path, words: &[String], paths: &[&str]) -> Result<Option<(String, String)>, String> {
    let mut log = vec!["git", "log", "--all", "--no-renames", "--format=%H", "-S", words[0].as_str(), "--"];
    log.extend(paths);
    let listed = crate::process::run(&log, root)?;
    let commits: Vec<&str> = listed.lines().filter(|line| !line.is_empty()).collect();
    for batch in commits.chunks(64) {
        let mut show = vec!["git", "show", "--no-renames", "--format=commit %H", "-U0"];
        show.extend(batch);
        show.push("--");
        show.extend(paths);
        if let Some(found) = first_given(&crate::process::run(&show, root)?, words) {
            return Ok(Some(found));
        }
    }
    Ok(None)
}

/// In `git show` output, the first commit with an added or removed line
/// that spells one of `words` whole.
fn first_given(shown: &str, words: &[String]) -> Option<(String, String)> {
    let mut commit = "";
    for line in shown.lines() {
        if let Some(hash) = line.strip_prefix("commit ") {
            commit = hash;
            continue;
        }
        let changed = (line.starts_with('+') && !line.starts_with("+++")) || (line.starts_with('-') && !line.starts_with("---"));
        if let Some(word) = words.iter().find(|word| changed && spells(&line[1..], word)) {
            return Some((commit.chars().take(10).collect(), word.clone()));
        }
    }
    None
}

/// The files of `places` the tree holds, tracked or new, by path.
fn files(root: &Path, places: &[&str]) -> Result<Vec<String>, String> {
    let mut command = vec!["git", "ls-files", "--cached", "--others", "--exclude-standard", "--"];
    command.extend(places);
    let listed = crate::process::run(&command, root)?;
    let found: BTreeSet<&str> = listed.lines().filter(|path| !path.is_empty() && root.join(path).is_file()).collect();
    Ok(found.into_iter().map(str::to_string).collect())
}

/// The files `ELSEWHERE` that spell a word whole.
fn elsewhere(root: &Path, word: &str) -> Result<Vec<String>, String> {
    let mut found = Vec::new();
    for path in files(root, &ELSEWHERE)? {
        let Ok(text) = std::fs::read_to_string(root.join(&path)) else {
            continue;
        };
        if text.lines().any(|line| spells(line, word)) {
            found.push(path);
        }
    }
    Ok(found)
}

/// The assembly sources of games/ that evidence.tsv credits as they stand
/// now: its rows bind a source's tokens and those of what it includes.
fn reviewed(root: &Path, paths: &[String]) -> BTreeSet<String> {
    paths
        .iter()
        .filter(|path| path.starts_with("games/") && (path.ends_with(".s") || path.ends_with(".S")))
        .filter(|path| {
            std::fs::read_to_string(root.join(path))
                .is_ok_and(|text| crate::evidence::assembly_review(root, path, &text).is_some())
        })
        .cloned()
        .collect()
}

/// The files, by path from the root, whose tokens evidence.tsv's digest of
/// an assembly source reads: the source and what it includes, through any
/// depth, resolved as crate::evidence resolves them (from the root, else
/// beside the including file) and kept to games/.
fn digested(root: &Path, path: &str) -> BTreeSet<String> {
    fn collect(root: &Path, games: &Path, path: &Path, found: &mut BTreeSet<PathBuf>) {
        let Ok(path) = std::fs::canonicalize(path) else {
            return;
        };
        if !path.starts_with(games) || !found.insert(path.clone()) {
            return;
        }
        let Ok(text) = std::fs::read_to_string(&path) else {
            return;
        };
        for line in text.lines() {
            let line = line.trim_start();
            let Some(include) = line.strip_prefix(".include").or_else(|| line.strip_prefix("#include")) else {
                continue;
            };
            let Some(name) = include.trim().strip_prefix('"').and_then(|rest| rest.split('"').next()) else {
                continue;
            };
            let child = match path.parent() {
                Some(folder) if !root.join(name).is_file() => folder.join(name),
                _ => root.join(name),
            };
            collect(root, games, &child, found);
        }
    }
    let (Ok(base), Ok(games)) = (std::fs::canonicalize(root), std::fs::canonicalize(root.join("games"))) else {
        return BTreeSet::new();
    };
    let mut found = BTreeSet::new();
    collect(root, &games, &root.join(path), &mut found);
    found
        .iter()
        .filter_map(|read| read.strip_prefix(&base).ok()?.to_str().map(str::to_string))
        .collect()
}

/// Whether a rename changes the tokens evidence.tsv's digest reads in a
/// file: a renamed comment changes none.
fn retokens(text: &str, new_text: &str) -> bool {
    crate::evidence::digest(text, true) != crate::evidence::digest(new_text, true)
}

/// The reviewed sources among `before` whose digest reads a file the rename
/// retokens: their evidence.tsv rows bind the old tokens.
fn losing<'a>(root: &Path, before: &'a BTreeSet<String>, retokened: &BTreeSet<String>) -> Vec<&'a String> {
    if retokened.is_empty() {
        return Vec::new();
    }
    before.iter().filter(|source| !digested(root, source).is_disjoint(retokened)).collect()
}

/// The texts of the files among `paths` this tool writes that hold one of
/// `needles` at all, by path. A file that is not UTF-8 is refused if it
/// holds one: its words cannot be read.
fn texts(root: &Path, paths: &[String], needles: &[&str]) -> Result<BTreeMap<String, String>, String> {
    let mut found = BTreeMap::new();
    for path in paths.iter().filter(|path| kind(path).is_some()) {
        let bytes = std::fs::read(root.join(path)).map_err(|e| format!("{path}: {e}"))?;
        match String::from_utf8(bytes) {
            Ok(text) => {
                if needles.iter().any(|needle| text.contains(needle)) {
                    found.insert(path.clone(), text);
                }
            }
            Err(error) => {
                let bytes = error.into_bytes();
                if let Some(needle) = needles.iter().find(|needle| bytes.windows(needle.len()).any(|window| window == needle.as_bytes())) {
                    return Err(format!("{path} holds {needle} but is not UTF-8: its words cannot be read"));
                }
            }
        }
    }
    Ok(found)
}

struct Request {
    old: String,
    new: String,
    file: Option<String>,
    dry: bool,
}

fn request(arguments: &[String]) -> Result<Request, String> {
    let mut positional = Vec::new();
    let (mut file, mut dry) = (None, false);
    let mut at = 0;
    while at < arguments.len() {
        match arguments[at].as_str() {
            "--file" => {
                at += 1;
                file = Some(arguments.get(at).ok_or(USAGE)?.trim_start_matches("./").to_string());
            }
            "--dry-run" => dry = true,
            "-h" | "--help" => return Err(USAGE.into()),
            flag if flag.starts_with("--") => return Err(USAGE.into()),
            word => positional.push(word.to_string()),
        }
        at += 1;
    }
    let [old, new] = <[String; 2]>::try_from(positional).map_err(|_| USAGE.to_string())?;
    for name in [&old, &new] {
        if !is_identifier(name) {
            return Err(format!("{name} is not a C identifier"));
        }
    }
    if old == new {
        return Err(format!("{old} is its name already"));
    }
    Ok(Request { old, new, file, dry })
}

/// What a rename writes: each old name with its new one, and the files
/// that spell them.
#[derive(Debug)]
struct Plan<'a> {
    names: BTreeMap<String, String>,
    written: Vec<&'a String>,
}

/// The rename the tree allows, or why it does not. `all` is every file of
/// PLACES; `texts` holds those `kind` reads that hold OLD or NEW as text,
/// by path, for only such a file can spell either, or either with Far.
fn plan<'a>(request: &Request, all: &[String], texts: &'a BTreeMap<String, String>) -> Result<Plan<'a>, String> {
    let spelled = |path: &str, word: &str| words(&texts[path], kind(path).expect("a read file has a kind")).iter().any(|(_, found)| *found == word);
    // What the rename writes, and what the new names must not be already.
    let (written, guarded): (Vec<&String>, Vec<&String>) = match &request.file {
        Some(file) => {
            if !all.contains(file) || kind(file).is_none() {
                return Err(format!("{file} is not a source of games/ or recon/ this tool writes"));
            }
            if shared(file) {
                return Err(format!("{file} is not a C unit: a static in it reaches every unit that includes it; rename without --file"));
            }
            if let Some(other) = texts.keys().find(|path| *path != file && shared(path) && spelled(path, &request.old)) {
                return Err(format!("{other} spells {} too: it is not a static of {file} alone; rename it without --file", request.old));
            }
            (texts.keys().filter(|path| *path == file).collect(), texts.keys().filter(|path| *path == file || shared(path)).collect())
        }
        None => (texts.keys().collect(), texts.keys().collect()),
    };
    let mut names = BTreeMap::from([(request.old.clone(), request.new.clone())]);
    let twin = format!("{}{FAR}", request.old);
    if request.file.is_none() && texts.keys().any(|path| spelled(path, &twin)) {
        names.insert(twin, format!("{}{FAR}", request.new));
    }
    if !written.iter().any(|path| spelled(path, &request.old)) {
        return Err(format!("{} does not spell {}", request.file.as_deref().unwrap_or("the tree"), request.old));
    }
    for new in names.values() {
        if let Some(path) = guarded.iter().find(|path| spelled(path, new)) {
            return Err(format!("{path} spells {new} already: W7 asks for a name nothing has"));
        }
    }
    Ok(Plan { names, written })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let request = request(arguments)?;
    for line in rename(crate::compiler::routing::root(), &request)? {
        println!("{line}");
    }
    Ok(())
}

/// The rename a request asks for in the tree at `root`, written unless it
/// is a dry run, and the lines that report it.
fn rename(root: &Path, request: &Request) -> Result<Vec<String>, String> {
    let all = files(root, &PLACES)?;
    let texts = texts(root, &all, &[&request.old, &request.new])?;
    let Plan { names, written } = plan(request, &all, &texts)?;
    // The commits of every branch, past and present, where the games' names
    // are given: W7.
    let mut ordered: Vec<String> = names.values().cloned().collect();
    ordered.sort_by_key(String::len);
    if let Some((commit, word)) = given(root, &ordered, &history(request.file.as_deref()))? {
        return Err(format!("commit {commit} already gives or takes {word} (git log --all -S): W7 asks for a name no branch has given"));
    }
    // The rename, and the reviews it costs.
    let before = reviewed(root, &all);
    let mut changed: Vec<(String, usize)> = Vec::new();
    let mut retokened: BTreeSet<String> = BTreeSet::new();
    let mut inline_lost: Vec<String> = Vec::new();
    for path in &written {
        let text = &texts[*path];
        let (new_text, count) = renamed(text, kind(path).expect("a read file has a kind"), &names);
        if count == 0 {
            continue;
        }
        if crate::evidence::approved_spans(&new_text).len() < crate::evidence::approved_spans(text).len() {
            inline_lost.push(path.to_string());
        }
        if retokens(text, &new_text) {
            retokened.insert(path.to_string());
        }
        if !request.dry {
            std::fs::write(root.join(path), &new_text).map_err(|e| format!("{path}: {e}"))?;
        }
        changed.push((path.to_string(), count));
    }
    let mut report = Vec::new();
    let renamed_text: Vec<String> = names.iter().map(|(old, new)| format!("{old} to {new}")).collect();
    let total: usize = changed.iter().map(|(_, count)| count).sum();
    report.push(format!(
        "{} {} in {} files ({total} places){}",
        if request.dry { "would rename" } else { "renamed" },
        renamed_text.join(", "),
        changed.len(),
        request.file.as_ref().map_or(String::new(), |file| format!(", {file} alone")),
    ));
    for (path, count) in &changed {
        report.push(format!("  {path}: {count}"));
    }
    // Names that only begin with OLD are other names, and keep theirs.
    let mut extended: BTreeMap<&str, BTreeSet<&str>> = BTreeMap::new();
    for path in &written {
        for (_, word) in words(&texts[*path], kind(path).expect("a read file has a kind")) {
            if word.len() > request.old.len() && word.starts_with(request.old.as_str()) && !names.contains_key(word) {
                extended.entry(word).or_default().insert(path.as_str());
            }
        }
    }
    for (word, paths) in &extended {
        report.push(format!("keeps its name, though it extends {}: {word} ({})", request.old, paths.iter().copied().collect::<Vec<_>>().join(", ")));
    }
    if request.file.is_none() {
        for old in names.keys() {
            for path in elsewhere(root, old)? {
                report.push(format!("left to a person: {path} spells {old}"));
            }
        }
        // The tools and the rules give no name to the games, so a NEW they
        // spell is no W7 refusal; a check that singles out that name now
        // singles out this one.
        for new in names.values() {
            for path in elsewhere(root, new)? {
                report.push(format!("noted, not refused: {path} spells {new}; see that it means this name"));
            }
        }
    }
    let lose = if request.dry { "would lose" } else { "loses" };
    for path in &inline_lost {
        report.push(format!("{lose} a CAMELOT_ASM review: {path}: evidence.tsv binds the old tokens"));
    }
    // A dry run reads the digests the new tokens would give; a rename reads
    // the ledger again over the files it wrote.
    let lost: Vec<&String> = if request.dry {
        losing(root, &before, &retokened)
    } else {
        let still = reviewed(root, &before.iter().cloned().collect::<Vec<_>>());
        before.iter().filter(|path| !still.contains(*path)).collect()
    };
    for path in lost {
        report.push(format!("{lose} its review: {path}: evidence.tsv binds the old tokens; carry its row to the new digest in a commit of its own, with DONE before and after (W6)"));
    }
    if !request.dry && !changed.is_empty() {
        report.push("judge it in all twelve editions: alchemy keep, with no PATH, keeps these files when every edition stays identical; a rename stands whole or not at all, so if it reverts one, put back the others".to_string());
    }
    Ok(report)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn names(pairs: &[(&str, &str)]) -> BTreeMap<String, String> {
        pairs.iter().map(|(old, new)| (old.to_string(), new.to_string())).collect()
    }

    #[test]
    fn a_name_is_written_anew_wherever_it_is_a_whole_word() {
        let map = names(&[("Fixture_Old", "Fixture_New"), ("Fixture_OldFar", "Fixture_NewFar")]);
        let c = "#include \"Fixture_Old.h\"\n// Fixture_Old の揺れ\nvoid Fixture_Old(s32 x);\nextern u8 Table[] __asm__(\"Fixture_Old\");\nx = Fixture_OldX + MyFixture_Old + Fixture_Old(1)/Fixture_Old;\n";
        assert_eq!(
            renamed(c, Kind::Words, &map),
            ("#include \"Fixture_Old.h\"\n// Fixture_New の揺れ\nvoid Fixture_New(s32 x);\nextern u8 Table[] __asm__(\"Fixture_New\");\nx = Fixture_OldX + MyFixture_Old + Fixture_New(1)/Fixture_New;\n".to_string(), 5)
        );
        let s = "\t.include \"Fixture_Old.inc\"\n\timport Fixture_Other, Fixture_OldFar\n\tbl Fixture_Old\n\t.byte 1\t@ PACKER: left in the buffer by Fixture_Old+0x45e\nFAR_CALL(Fixture_OldFar, Fixture_Old, void, (void))\n";
        assert_eq!(
            renamed(s, Kind::Words, &map),
            ("\t.include \"Fixture_Old.inc\"\n\timport Fixture_Other, Fixture_NewFar\n\tbl Fixture_New\n\t.byte 1\t@ PACKER: left in the buffer by Fixture_New+0x45e\nFAR_CALL(Fixture_NewFar, Fixture_New, void, (void))\n".to_string(), 5)
        );
        let ld = "INCLUDE \"Fixture_Old.ld\"\nFixture_OldCodeSize = Fixture_Next - Fixture_Old;\n";
        assert_eq!(renamed(ld, Kind::Words, &map).0, "INCLUDE \"Fixture_Old.ld\"\nFixture_OldCodeSize = Fixture_Next - Fixture_New;\n");
    }

    #[test]
    fn a_catalog_spells_names_on_its_msgctxt_lines_alone() {
        let map = names(&[("Fixture_OldMsg", "Fixture_NewMsg")]);
        let po = "msgctxt \"Fixture_OldMsg\"\nmsgid \"00000\"\nmsgstr \"Fixture_OldMsg\"\n";
        assert_eq!(renamed(po, Kind::Catalog, &map), ("msgctxt \"Fixture_NewMsg\"\nmsgid \"00000\"\nmsgstr \"Fixture_OldMsg\"\n".to_string(), 1));
    }

    #[test]
    fn a_file_kind_and_a_name_are_read_strictly() {
        assert_eq!(kind("games/tbs/src/field/field.c"), Some(Kind::Words));
        assert_eq!(kind("games/tbs/src/system/far_call/BATTLE.S"), Some(Kind::Words));
        assert_eq!(kind("games/tbs/text/en.po"), Some(Kind::Catalog));
        assert_eq!(kind("tools/alchemy/aliases.tsv"), Some(Kind::Words));
        assert_eq!(kind("games/tbs/src/battle/presets.tsv"), None);
        assert_eq!(kind("games/tbs/src/graphics/a.png"), None);
        assert!(is_identifier("EV08_KAGI") && is_identifier("_x1"));
        assert!(!is_identifier("8EV") && !is_identifier("Fixture-Old") && !is_identifier("") && !is_identifier("坊主"));
        assert!(names_a_file("#  include \"a.h\"") && names_a_file("\t.incbin \"overlays/resource_36f.lz\"") && names_a_file("INCLUDE \"rom_tail_02.ld\""));
        assert!(!names_a_file("\t.include_x Foo") && !names_a_file("Foo_Include = 1;"));
        assert!(shared("games/tbs/include/event.h") && shared("games/tbs/src/system/overlay.inc") && !shared("games/tbs/src/scene/a.c"));
    }

    #[test]
    fn a_requests_names_and_flags_are_read() {
        let args = |list: &[&str]| list.iter().map(|word| word.to_string()).collect::<Vec<_>>();
        let read = request(&args(&["Old_Name", "New_Name", "--file", "./games/tbs/src/scene/a.c", "--dry-run"])).unwrap();
        assert_eq!((read.old.as_str(), read.new.as_str(), read.file.as_deref(), read.dry), ("Old_Name", "New_Name", Some("games/tbs/src/scene/a.c"), true));
        assert!(request(&args(&["Old_Name"])).is_err());
        assert!(request(&args(&["Old_Name", "Old_Name"])).is_err());
        assert!(request(&args(&["Old_Name", "New-Name"])).is_err());
        assert!(request(&args(&["Old_Name", "New_Name", "--force"])).is_err());
    }

    #[test]
    fn history_counts_a_word_only_where_a_changed_line_spells_it_whole() {
        let shown = "commit 0123456789abcdef\n\ndiff --git a/x.c b/x.c\n--- a/x.c\n+++ b/x.c\n@@ -1 +1 @@\n-Fixture_OldX();\n+Fixture_OldY();\ncommit fedcba9876543210\n\ndiff --git a/y.c b/y.c\n@@ -0,0 +1 @@\n+\tbl Fixture_OldFar\n";
        let words = ["Fixture_Old".to_string(), "Fixture_OldFar".to_string()];
        assert_eq!(first_given(shown, &words), Some(("fedcba9876".to_string(), "Fixture_OldFar".to_string())));
        assert_eq!(first_given(shown, &words[..1]), None);
        assert!(spells("x = Fixture_Old;", "Fixture_Old") && !spells("x = Fixture_OldX;", "Fixture_Old"));
    }

    fn asked(old: &str, new: &str, file: Option<&str>) -> Request {
        Request { old: old.into(), new: new.into(), file: file.map(Into::into), dry: true }
    }

    /// Every file of a tree, and the texts of those the tool reads.
    fn tree(files: &[(&str, &str)]) -> (Vec<String>, BTreeMap<String, String>) {
        let all = files.iter().map(|(path, _)| path.to_string()).collect();
        let texts = files.iter().filter(|(path, _)| kind(path).is_some()).map(|(path, text)| (path.to_string(), text.to_string())).collect();
        (all, texts)
    }

    #[test]
    fn a_far_call_twin_follows_only_where_the_tree_spells_it() {
        let (all, texts) = tree(&[("games/a.c", "void Fixture_Old(void) {}\n"), ("games/far.S", "FAR_CALL(Fixture_OldFar, Fixture_Old, void, (void))\n")]);
        let both = plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).unwrap();
        assert_eq!(both.names, names(&[("Fixture_Old", "Fixture_New"), ("Fixture_OldFar", "Fixture_NewFar")]));
        assert_eq!(both.written, [&all[0], &all[1]]);
        // A word that only begins with the twin is not one.
        let (all, texts) = tree(&[("games/a.c", "void Fixture_Old(void) {}\nint Fixture_OldFarther;\n")]);
        assert_eq!(plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).unwrap().names, names(&[("Fixture_Old", "Fixture_New")]));
        // A twin the new name has already is refused with it.
        let (all, texts) = tree(&[("games/a.c", "Fixture_Old(); Fixture_OldFar();\n"), ("games/b.s", "\tbl Fixture_NewFar\n")]);
        assert!(plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).unwrap_err().contains("games/b.s spells Fixture_NewFar already"));
    }

    #[test]
    fn a_new_name_the_games_spell_is_refused_and_an_old_one_they_lack() {
        let (all, texts) = tree(&[("games/a.c", "void Fixture_Old(void) {}\n"), ("recon/tla/en/b.s", "Fixture_New:\n")]);
        assert!(plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).unwrap_err().contains("recon/tla/en/b.s spells Fixture_New already"));
        // A longer word that holds NEW is another name.
        let (all, texts) = tree(&[("games/a.c", "void Fixture_Old(void) {}\nint Fixture_NewX;\n")]);
        assert!(plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).is_ok());
        assert!(plan(&asked("Fixture_Gone", "Fixture_New", None), &all, &texts).unwrap_err().contains("the tree does not spell Fixture_Gone"));
    }

    #[test]
    fn file_takes_a_static_of_one_c_unit_alone() {
        let (all, texts) = tree(&[
            ("games/a.c", "static void Fixture_Old(void) {}\n"),
            ("games/b.c", "static void Fixture_Old(void) {}\nstatic void Fixture_New(void) {}\n"),
            ("games/c.h", "static void Fixture_Other(void) {}\n"),
        ]);
        // Another unit's statics are its own: b.c keeps its Fixture_Old and
        // may hold a Fixture_New.
        let one = plan(&asked("Fixture_Old", "Fixture_New", Some("games/a.c")), &all, &texts).unwrap();
        assert_eq!(one.written, [&all[0]]);
        assert_eq!(one.names, names(&[("Fixture_Old", "Fixture_New")]));
        assert!(plan(&asked("Fixture_Old", "Fixture_New", None), &all, &texts).unwrap_err().contains("games/b.c spells Fixture_New already"));
        // A header is no unit: its static reaches every unit that includes it.
        assert!(plan(&asked("Fixture_Other", "Fixture_New", Some("games/c.h")), &all, &texts).unwrap_err().contains("games/c.h is not a C unit"));
        // A header that spells OLD makes it no static of the unit alone.
        let (all, texts) = tree(&[("games/a.c", "Fixture_Old();\n"), ("games/a.h", "void Fixture_Old(void);\n")]);
        assert!(plan(&asked("Fixture_Old", "Fixture_New", Some("games/a.c")), &all, &texts).unwrap_err().contains("games/a.h spells Fixture_Old too"));
        // Nor may a static take a name a header gives.
        let (all, texts) = tree(&[("games/a.c", "static void Fixture_Old(void) {}\n"), ("games/a.h", "void Fixture_New(void);\n")]);
        assert!(plan(&asked("Fixture_Old", "Fixture_New", Some("games/a.c")), &all, &texts).unwrap_err().contains("games/a.h spells Fixture_New already"));
        // The far-call twin is no static's.
        let (all, texts) = tree(&[("games/a.c", "static void Fixture_Old(void) {}\nvoid Fixture_OldFar(void);\n")]);
        assert_eq!(plan(&asked("Fixture_Old", "Fixture_New", Some("games/a.c")), &all, &texts).unwrap().names.len(), 1);
    }

    fn git(root: &Path, args: &[&str]) {
        let output = std::process::Command::new("git")
            .arg("-C")
            .arg(root)
            .args(["-c", "user.name=fixture", "-c", "user.email=fixture@example.invalid", "-c", "commit.gpgsign=false"])
            .args(args)
            .output()
            .unwrap();
        assert!(output.status.success(), "{}", String::from_utf8_lossy(&output.stderr));
    }

    /// A commit that writes each file with text and removes each without.
    fn commit(root: &Path, files: &[(&str, Option<&str>)]) {
        for (path, text) in files {
            let file = root.join(path);
            match text {
                Some(text) => {
                    std::fs::create_dir_all(file.parent().unwrap()).unwrap();
                    std::fs::write(file, text).unwrap();
                }
                None => std::fs::remove_file(file).unwrap(),
            }
        }
        git(root, &["add", "--all"]);
        git(root, &["commit", "--quiet", "--message", "fixture"]);
    }

    #[test]
    fn history_reads_where_the_games_names_are_given() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"]);
        commit(root, &[("games/x/a.c", Some("void Fixture_Old(void) {}\n")), ("tools/alchemy/src/t.rs", Some("// Fixture_New, a test's word\n"))]);
        // The tools' source gives no name to the games, now or in history.
        let report = rename(root, &asked("Fixture_Old", "Fixture_New", None)).unwrap();
        assert_eq!(report[..2], ["would rename Fixture_Old to Fixture_New in 1 files (1 places)", "  games/x/a.c: 1"]);
        assert!(report.contains(&"noted, not refused: tools/alchemy/src/t.rs spells Fixture_New; see that it means this name".to_string()));
        assert_eq!(std::fs::read_to_string(root.join("games/x/a.c")).unwrap(), "void Fixture_Old(void) {}\n");
        // A name a commit gave in games/ and a later one took away is refused.
        commit(root, &[("games/x/b.c", Some("void Fixture_Given(void) {}\n"))]);
        commit(root, &[("games/x/b.c", None)]);
        assert!(rename(root, &asked("Fixture_Old", "Fixture_Given", None)).unwrap_err().contains("gives or takes Fixture_Given"));
        // For a static, another unit's history is its own, and a header's is
        // every unit's.
        commit(root, &[("games/x/c.c", Some("static void Fixture_Static(void) {}\n"))]);
        commit(root, &[("games/x/c.c", None)]);
        assert!(rename(root, &asked("Fixture_Old", "Fixture_Static", Some("games/x/a.c"))).is_ok());
        assert!(rename(root, &asked("Fixture_Old", "Fixture_Static", None)).unwrap_err().contains("gives or takes Fixture_Static"));
        commit(root, &[("games/x/d.h", Some("void Fixture_Shared(void);\n"))]);
        commit(root, &[("games/x/d.h", None)]);
        assert!(rename(root, &asked("Fixture_Old", "Fixture_Shared", Some("games/x/a.c"))).unwrap_err().contains("gives or takes Fixture_Shared"));
        // A rename writes what its dry run reported.
        let request = Request { dry: false, ..asked("Fixture_Old", "Fixture_New", None) };
        assert_eq!(rename(root, &request).unwrap()[0], "renamed Fixture_Old to Fixture_New in 1 files (1 places)");
        assert_eq!(std::fs::read_to_string(root.join("games/x/a.c")).unwrap(), "void Fixture_New(void) {}\n");
    }

    #[test]
    fn a_review_is_lost_where_the_tokens_its_digest_reads_change() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        for (path, text) in [
            ("games/x/a.S", "\t.include \"games/x/b.inc\"\n\tbl Fixture_Old\n"),
            ("games/x/b.inc", "\t.include \"c.inc\"\n"),
            ("games/x/c.inc", "Fixture_Old:\n\tbx lr\n"),
            ("games/x/d.s", "\tbx lr\n"),
            ("games/x/e.s", "\tbx lr\n"),
        ] {
            std::fs::create_dir_all(root.join(path).parent().unwrap()).unwrap();
            std::fs::write(root.join(path), text).unwrap();
        }
        // The digest reads a source and what it includes, at any depth.
        let read: Vec<String> = digested(root, "games/x/a.S").into_iter().collect();
        assert_eq!(read, ["games/x/a.S", "games/x/b.inc", "games/x/c.inc"]);
        // A renamed label retokens its file; a renamed comment does not.
        assert!(retokens("Fixture_Old:\n\tbx lr\n", "Fixture_New:\n\tbx lr\n"));
        assert!(!retokens("\tbx lr\t@ Fixture_Old\n", "\tbx lr\t@ Fixture_New\n"));
        let before: BTreeSet<String> = ["games/x/a.S", "games/x/d.s"].into_iter().map(String::from).collect();
        let retokened = |paths: &[&str]| paths.iter().map(|path| path.to_string()).collect::<BTreeSet<_>>();
        assert_eq!(losing(root, &before, &retokened(&["games/x/c.inc"])), [&"games/x/a.S".to_string()]);
        assert!(losing(root, &before, &retokened(&["games/x/e.s"])).is_empty());
        assert!(losing(root, &before, &retokened(&[])).is_empty());
    }

    #[test]
    fn a_dry_run_reports_the_reviews_the_rename_costs() {
        // A reviewed assembly source of the tree, copied to its path in a
        // fixture, is reviewed there too: the ledger binds its tokens.
        let tree = crate::compiler::routing::root();
        let (path, label) = reviewed(tree, &files(tree, &["games"]).unwrap())
            .into_iter()
            .find_map(|path| {
                let text = std::fs::read_to_string(tree.join(&path)).ok()?;
                let label = text.lines().find_map(|line| line.trim().strip_prefix(".global ")).map(|name| name.trim().to_string())?;
                (digested(tree, &path).len() == 1 && is_identifier(&label)).then_some((path, label))
            })
            .expect("a reviewed assembly source that includes nothing");
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        git(root, &["init", "--quiet"]);
        commit(root, &[(path.as_str(), Some(&std::fs::read_to_string(tree.join(&path)).unwrap()))]);
        assert_eq!(reviewed(root, std::slice::from_ref(&path)), BTreeSet::from([path.clone()]));
        // The dry run reads the digest the new tokens would give; the rename
        // reads the ledger again over what it wrote.
        let dry = rename(root, &asked(&label, "Fixture_New", None)).unwrap();
        assert!(dry.iter().any(|line| line.starts_with(&format!("would lose its review: {path}: "))), "{dry:?}");
        let written = rename(root, &Request { dry: false, ..asked(&label, "Fixture_New", None) }).unwrap();
        assert!(written.iter().any(|line| line.starts_with(&format!("loses its review: {path}: "))), "{written:?}");
    }
}
