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
use std::path::Path;

const USAGE: &str = r#"usage: alchemy rename OLD NEW [--file FILE] [--dry-run]
Writes NEW wherever the tree spells OLD as a whole word: C and headers,
assembly labels and calls, the entry and import tables' veneers, the rows
of overlay.inc, linker scripts, recon/ scaffolds, comments (PACKER rows
included), a PO catalog's msgctxt and tools/alchemy/aliases.tsv. A line
that names a file (#include, .include, .incbin, INCLUDE) keeps its words.
OLD's far-call twin OLDFar, where the tree has one, becomes NEWFar.
--file FILE limits the rename to FILE, for a static: other files keep
their OLD, and a header, assembly source or linker script that spells
OLD refuses it.
NEW is refused where the tree spells it already (with --file: FILE, or a
header, assembly source or linker script) and where a commit of any
branch added or took it away (git log --all -S; with --file, FILE's
commits): W7. Prints each file written with its count, the names that
extend OLD and keep theirs (OLDCodeSize), the files outside games/ and
recon/ that spell OLD and are left to a person (the tools' source,
AGENTS.md), and each reviewed assembly source whose evidence.tsv row the
new tokens no longer match. Then `alchemy keep`, with no PATH, judges
the rename in all twelve editions: it keeps the files when every edition
stays identical. A rename stands whole or not at all: if keep reverts one
of its files, put back the others.
--dry-run prints the files and writes nothing."#;

/// Where the tree spells the names it builds from, and the table of
/// symbols kept together beside them.
const PLACES: [&str; 3] = ["games", "recon", "tools/alchemy/aliases.tsv"];

/// What spells names the tool does not write: the tools' own source, and
/// the rules and the fans' page, whose wording is Pascal's.
const ELSEWHERE: [&str; 3] = ["tools/alchemy/src", "AGENTS.md", "README.md"];

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

/// The first commit of a branch whose diff adds or takes away one of
/// `words` whole, with the word: `git log --all -S` lists the commits that
/// change how often a file holds the first word as text, which every word
/// that holds it does, and their diffs are read for the words whole.
fn given(root: &Path, words: &[String], paths: &[&str]) -> Result<Option<(String, String)>, String> {
    let mut log = vec!["git", "log", "--all", "--no-renames", "--format=%H", "-S", words[0].as_str()];
    if !paths.is_empty() {
        log.push("--");
        log.extend(paths);
    }
    let listed = crate::process::run(&log, root)?;
    let commits: Vec<&str> = listed.lines().filter(|line| !line.is_empty()).collect();
    for batch in commits.chunks(64) {
        let mut show = vec!["git", "show", "--no-renames", "--format=commit %H", "-U0"];
        show.extend(batch);
        if !paths.is_empty() {
            show.push("--");
            show.extend(paths);
        }
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

pub fn run(arguments: &[String]) -> Result<(), String> {
    let request = request(arguments)?;
    let root = crate::compiler::routing::root();
    let all = files(root, &PLACES)?;
    // Only a file that holds OLD or NEW as text can spell either, or a
    // name with Far after it.
    let texts = texts(root, &all, &[&request.old, &request.new])?;
    let spelled = |path: &str, word: &str| words(&texts[path], kind(path).expect("a read file has a kind")).iter().any(|(_, found)| *found == word);
    // What the rename writes, and what the new names must not be already.
    let (written, guarded): (Vec<&String>, Vec<&String>) = match &request.file {
        Some(file) => {
            if !all.contains(file) || kind(file).is_none() {
                return Err(format!("{file} is not a source of games/ or recon/ this tool writes"));
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
    let fresh: Vec<String> = names.values().cloned().collect();
    for new in &fresh {
        if let Some(path) = guarded.iter().find(|path| spelled(path, new)) {
            return Err(format!("{path} spells {new} already: W7 asks for a name nothing has"));
        }
        if request.file.is_none() {
            if let Some(path) = elsewhere(root, new)?.first() {
                return Err(format!("{path} spells {new} already: W7 asks for a name nothing has"));
            }
        }
    }
    // The commits of every branch, past and present: W7.
    let mut ordered = fresh.clone();
    ordered.sort_by_key(String::len);
    let history: Vec<&str> = request.file.iter().map(String::as_str).collect();
    if let Some((commit, word)) = given(root, &ordered, &history)? {
        return Err(format!("commit {commit} already gives or takes {word} (git log --all -S): W7 asks for a name no branch has given"));
    }
    // The rename.
    let before = if request.dry { BTreeSet::new() } else { reviewed(root, &all) };
    let mut changed: Vec<(String, usize)> = Vec::new();
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
        if !request.dry {
            std::fs::write(root.join(path), &new_text).map_err(|e| format!("{path}: {e}"))?;
        }
        changed.push((path.to_string(), count));
    }
    let renamed_text: Vec<String> = names.iter().map(|(old, new)| format!("{old} to {new}")).collect();
    let total: usize = changed.iter().map(|(_, count)| count).sum();
    println!(
        "{} {} in {} files ({total} places){}",
        if request.dry { "would rename" } else { "renamed" },
        renamed_text.join(", "),
        changed.len(),
        request.file.as_ref().map_or(String::new(), |file| format!(", {file} alone")),
    );
    for (path, count) in &changed {
        println!("  {path}: {count}");
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
        println!("keeps its name, though it extends {}: {word} ({})", request.old, paths.iter().copied().collect::<Vec<_>>().join(", "));
    }
    if request.file.is_none() {
        for old in names.keys() {
            for path in elsewhere(root, old)? {
                println!("left to a person: {path} spells {old}");
            }
        }
    }
    for path in &inline_lost {
        println!("loses a CAMELOT_ASM review: {path}: evidence.tsv binds the old tokens");
    }
    if !request.dry {
        let still = reviewed(root, &before.iter().cloned().collect::<Vec<_>>());
        for path in before.difference(&still) {
            println!("loses its review: {path}: evidence.tsv binds the old tokens; carry its row to the new digest in a commit of its own, with DONE before and after (W6)");
        }
    }
    if !request.dry && !changed.is_empty() {
        println!("judge it in all twelve editions: alchemy keep, with no PATH, keeps these files when every edition stays identical; a rename stands whole or not at all, so if it reverts one, put back the others");
    }
    Ok(())
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
}
