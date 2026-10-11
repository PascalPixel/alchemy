//! What the preprocessor read to make each C object's cache key.
//!
//! A C object's key covers its preprocessed text, so forming the key used to
//! mean preprocessing every source of every edition on every build, changed
//! or not. A row here keeps, for one object, the key that text gave together
//! with every file the preprocessor read to produce it (its own dependency
//! list, the one `make` and `ninja` use) and the names in every folder it
//! searched. While the command is the same, every one of those files has the
//! same content and no searched folder has gained or lost a name, the text
//! would be the same, so the key is the one recorded and the preprocessor
//! does not run. The evidence is content only: no timestamp and no size.
//!
//! The folder names close the one hole of a dependency list: a new file that
//! an `#include` would now find earlier on the search path.

use sha2::{Digest, Sha256};
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering};
use std::sync::{Mutex, OnceLock};

const FILE: &str = "reads.tsv";
const MISSING: &str = "-";

/// How many sources this process had to preprocess to learn their keys.
pub(crate) static PREPROCESSED: AtomicUsize = AtomicUsize::new(0);

pub(crate) struct Reads {
    output: PathBuf,
    rows: Mutex<BTreeMap<String, Row>>,
    changed: AtomicBool,
}

#[derive(Clone, Debug, PartialEq)]
struct Row {
    guard: String,
    key: String,
    folders: String,
    files: Vec<(String, String)>,
}

impl Reads {
    pub(crate) fn open(output: &Path) -> Reads {
        // The output folder is itself searched for generated headers, so the
        // table must be one of its names before the first row is recorded:
        // appearing afterwards, it would end every key it holds.
        if output.is_dir() {
            let _ = fs::File::options().create(true).append(true).open(output.join(FILE));
        }
        let text = fs::read_to_string(output.join(FILE)).unwrap_or_default();
        let rows = text
            .lines()
            .filter_map(|line| {
                let mut fields = line.split('\t');
                let object = fields.next()?.to_owned();
                let guard = fields.next()?.to_owned();
                let key = fields.next()?.to_owned();
                let folders = fields.next()?.to_owned();
                let rest: Vec<&str> = fields.collect();
                if rest.is_empty() || !rest.len().is_multiple_of(2) {
                    return None;
                }
                let files = rest.chunks(2).map(|pair| (pair[0].to_owned(), pair[1].to_owned())).collect();
                Some((object, Row { guard, key, folders, files }))
            })
            .collect();
        Reads { output: output.to_path_buf(), rows: Mutex::new(rows), changed: AtomicBool::new(false) }
    }

    fn name(&self, object: &Path) -> String {
        object.strip_prefix(&self.output).unwrap_or(object).to_string_lossy().into_owned()
    }

    /// The key recorded for `object`, when nothing its text was made from
    /// has changed since: `guard` names the toolchain and the commands, and
    /// `command` is the preprocessing command whose folders are searched.
    pub(crate) fn key(&self, root: &Path, object: &Path, guard: &str, command: &[String]) -> Option<String> {
        let row = self.rows.lock().unwrap().get(&self.name(object)).cloned()?;
        let same = row.guard == guard
            && row.files.iter().all(|(file, hash)| content(root, file) == *hash)
            && row.folders == folders(root, command, row.files.iter().map(|(file, _)| file.as_str()));
        same.then_some(row.key)
    }

    /// Records `key` for `object` with the files `listed` (the dependency
    /// rule the preprocessor wrote) names. Without a usable list nothing is
    /// recorded, and the object is preprocessed again next time.
    pub(crate) fn record(
        &self,
        root: &Path,
        object: &Path,
        guard: &str,
        key: &str,
        command: &[String],
        listed: &Path,
    ) {
        let name = self.name(object);
        let files = fs::read_to_string(listed).map(|rule| rule_files(root, &rule)).unwrap_or_default();
        let mut rows = self.rows.lock().unwrap();
        self.changed.store(true, Ordering::Relaxed);
        if files.is_empty() {
            rows.remove(&name);
            return;
        }
        let row = Row {
            guard: guard.to_owned(),
            key: key.to_owned(),
            folders: folders(root, command, files.iter().map(String::as_str)),
            files: files.iter().map(|file| (file.clone(), content(root, file))).collect(),
        };
        rows.insert(name, row);
    }

    pub(crate) fn save(&self) -> Result<(), String> {
        if !self.changed.load(Ordering::Relaxed) {
            return Ok(());
        }
        let mut text = String::new();
        for (object, row) in self.rows.lock().unwrap().iter() {
            text.push_str(&format!("{object}\t{}\t{}\t{}", row.guard, row.key, row.folders));
            for (file, hash) in &row.files {
                text.push_str(&format!("\t{file}\t{hash}"));
            }
            text.push('\n');
        }
        let path = self.output.join(FILE);
        let fresh = self.output.join(format!("{FILE}.new"));
        fs::write(&fresh, text).map_err(|error| format!("{}: {error}", fresh.display()))?;
        fs::rename(&fresh, &path).map_err(|error| format!("{}: {error}", path.display()))
    }
}

/// `command` asking the preprocessor to write its dependency rule to `listed`
/// as well. The three programs the build preprocesses with take it three
/// ways: the bare cpp0 as its own option, the old driver handed through to
/// its cpp0 (its own -MD would name the file after the source, in the
/// working directory), and a current driver with -MF.
pub(crate) fn listing(command: &[String], listed: &Path, modern: bool) -> Vec<String> {
    let listed = listed.to_string_lossy().into_owned();
    let mut command = command.to_vec();
    if command.first().is_some_and(|program| program.ends_with("cpp0")) {
        command.splice(1..1, ["-MD".to_owned(), listed]);
    } else if modern {
        command.extend(["-MD".to_owned(), "-MF".to_owned(), listed]);
    } else {
        command.push(format!("-Wp,-MD,{listed}"));
    }
    command
}

/// The files a dependency rule (`target: file file \` and so on) names, each
/// relative to `root` when it lies under it.
fn rule_files(root: &Path, rule: &str) -> BTreeSet<String> {
    let rule = rule.replace("\\\n", " ");
    let Some((_, files)) = rule.split_once(':') else {
        return BTreeSet::new();
    };
    files
        .split_whitespace()
        .map(|file| Path::new(file).strip_prefix(root).unwrap_or(Path::new(file)).to_string_lossy().into_owned())
        .collect()
}

/// The folders `command` searches for an include: each -I, -isystem, -iquote
/// and -idirafter folder, and the folder of every file read, where a quoted
/// include looks first.
fn searched<'a>(command: &[String], files: impl Iterator<Item = &'a str>) -> BTreeSet<String> {
    let mut found: BTreeSet<String> =
        files.map(|file| Path::new(file).parent().unwrap_or(Path::new("")).to_string_lossy().into_owned()).collect();
    let mut arguments = command.iter();
    while let Some(argument) = arguments.next() {
        let folder = match argument.as_str() {
            "-I" | "-isystem" | "-iquote" | "-idirafter" => arguments.next().map(String::as_str),
            other => other.strip_prefix("-I"),
        };
        found.extend(folder.filter(|folder| !folder.is_empty()).map(str::to_owned));
    }
    found
}

/// One digest of the names in every searched folder, each folder named
/// relative to `root` when it lies under it, so a moved checkout agrees.
fn folders<'a>(root: &Path, command: &[String], files: impl Iterator<Item = &'a str>) -> String {
    let shown: BTreeSet<String> = searched(command, files)
        .iter()
        .map(|folder| Path::new(folder).strip_prefix(root).unwrap_or(Path::new(folder)).to_string_lossy().into_owned())
        .collect();
    let mut hasher = Sha256::new();
    for shown in shown {
        hasher.update(shown.as_bytes());
        hasher.update([0]);
        hasher.update(names(root, &shown).as_bytes());
        hasher.update(b"\n");
    }
    format!("{:x}", hasher.finalize())
}

/// Only what the build never writes is remembered for the life of the
/// process: the sources under games/ and files outside the checkout. A file
/// or folder under any other part of the checkout (build output) is read
/// again every time it is asked for.
fn remembered(file: &str) -> bool {
    Path::new(file).is_absolute() || file.starts_with("games/")
}

fn memo() -> &'static Mutex<HashMap<String, String>> {
    static MEMO: OnceLock<Mutex<HashMap<String, String>>> = OnceLock::new();
    MEMO.get_or_init(Default::default)
}

fn recalled(kind: char, root: &Path, file: &str, read: impl Fn(&Path) -> String) -> String {
    if !remembered(file) {
        return read(&root.join(file));
    }
    let name = format!("{kind}{}\0{file}", root.display());
    if let Some(known) = memo().lock().unwrap().get(&name) {
        return known.clone();
    }
    let value = read(&root.join(file));
    memo().lock().unwrap().insert(name, value.clone());
    value
}

/// The SHA-256 of a file's content, or MISSING.
fn content(root: &Path, file: &str) -> String {
    recalled('f', root, file, |path| {
        fs::read(path).map_or_else(|_| MISSING.to_owned(), |bytes| format!("{:x}", Sha256::digest(bytes)))
    })
}

/// The sorted names in a folder, or MISSING.
fn names(root: &Path, folder: &str) -> String {
    recalled('d', root, folder, |path| {
        let Ok(entries) = fs::read_dir(path) else {
            return MISSING.to_owned();
        };
        let names: BTreeSet<String> =
            entries.flatten().map(|entry| entry.file_name().to_string_lossy().into_owned()).collect();
        names.into_iter().collect::<Vec<_>>().join("\0")
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A small tree outside games/, so nothing is remembered between steps:
    /// a source, a header beside it and one in an -I folder.
    fn tree() -> (tempfile::TempDir, PathBuf, Vec<String>) {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path().join("checkout");
        for (file, text) in [("src/a.c", "#include \"a.h\"\n"), ("src/a.h", "int a;\n"), ("include/b.h", "int b;\n")] {
            fs::create_dir_all(root.join(file).parent().unwrap()).unwrap();
            fs::write(root.join(file), text).unwrap();
        }
        fs::create_dir_all(root.join("out/obj")).unwrap();
        let rule = format!("a.o: src/a.c \\\n  src/a.h {}/include/b.h\n", root.display());
        fs::write(root.join("out/obj/a.d"), rule).unwrap();
        let command = ["cc", "-Iinclude", "-E", "src/a.c"].map(str::to_owned).to_vec();
        (directory, root, command)
    }

    fn recorded(root: &Path, command: &[String]) -> Reads {
        let reads = Reads::open(&root.join("out"));
        reads.record(root, &root.join("out/obj/a.o"), "guard", "key-a", command, &root.join("out/obj/a.d"));
        reads
    }

    fn key(reads: &Reads, root: &Path, command: &[String]) -> Option<String> {
        reads.key(root, &root.join("out/obj/a.o"), "guard", command)
    }

    #[test]
    fn a_key_holds_while_every_file_read_keeps_its_content() {
        let (_directory, root, command) = tree();
        let reads = recorded(&root, &command);
        assert_eq!(key(&reads, &root, &command).as_deref(), Some("key-a"));
        // Rewritten with the same bytes: a new timestamp is not a change.
        fs::write(root.join("src/a.h"), "int a;\n").unwrap();
        assert_eq!(key(&reads, &root, &command).as_deref(), Some("key-a"));
        for file in ["src/a.c", "src/a.h", "include/b.h"] {
            let before = fs::read(root.join(file)).unwrap();
            fs::write(root.join(file), "changed\n").unwrap();
            assert_eq!(key(&reads, &root, &command), None, "{file} changed");
            fs::write(root.join(file), before).unwrap();
            assert_eq!(key(&reads, &root, &command).as_deref(), Some("key-a"), "{file} restored");
        }
        fs::remove_file(root.join("include/b.h")).unwrap();
        assert_eq!(key(&reads, &root, &command), None);
    }

    #[test]
    fn a_new_name_in_a_searched_folder_ends_the_key() {
        let (_directory, root, command) = tree();
        let reads = recorded(&root, &command);
        // Beside the source, where a quoted include looks first.
        fs::write(root.join("src/b.h"), "").unwrap();
        assert_eq!(key(&reads, &root, &command), None);
        fs::remove_file(root.join("src/b.h")).unwrap();
        assert_eq!(key(&reads, &root, &command).as_deref(), Some("key-a"));
        // In an -I folder.
        fs::write(root.join("include/a.h"), "").unwrap();
        assert_eq!(key(&reads, &root, &command), None);
        fs::remove_file(root.join("include/a.h")).unwrap();
        // A folder nothing searches does not matter.
        fs::write(root.join("out/obj/new.o"), "").unwrap();
        assert_eq!(key(&reads, &root, &command).as_deref(), Some("key-a"));
    }

    #[test]
    fn another_command_or_toolchain_ends_the_key() {
        let (_directory, root, command) = tree();
        let reads = recorded(&root, &command);
        let object = root.join("out/obj/a.o");
        assert_eq!(reads.key(&root, &object, "another guard", &command), None);
        // A folder added to the search path.
        fs::create_dir_all(root.join("more")).unwrap();
        let mut longer = command.clone();
        longer.insert(1, "-Imore".into());
        assert_eq!(reads.key(&root, &object, "guard", &longer), None);
        assert_eq!(reads.key(&root, &root.join("out/obj/other.o"), "guard", &command), None);
    }

    #[test]
    fn rows_survive_a_save_and_a_move_of_the_checkout() {
        let (directory, root, command) = tree();
        let reads = recorded(&root, &command);
        reads.save().unwrap();
        let moved = directory.path().join("moved");
        fs::rename(&root, &moved).unwrap();
        let reads = Reads::open(&moved.join("out"));
        assert_eq!(key(&reads, &moved, &command).as_deref(), Some("key-a"));
        assert!(!fs::read_to_string(moved.join("out").join(FILE)).unwrap().contains("checkout"));
    }

    #[test]
    fn nothing_is_recorded_without_a_dependency_list() {
        let (_directory, root, command) = tree();
        let reads = recorded(&root, &command);
        for rule in [None, Some(""), Some("no rule here\n")] {
            match rule {
                Some(rule) => fs::write(root.join("out/obj/a.d"), rule).unwrap(),
                None => fs::remove_file(root.join("out/obj/a.d")).unwrap(),
            }
            reads.record(&root, &root.join("out/obj/a.o"), "guard", "key-b", &command, &root.join("out/obj/a.d"));
            assert_eq!(key(&reads, &root, &command), None);
        }
    }

    #[test]
    fn each_preprocessor_is_asked_for_its_list_its_own_way() {
        let listed = Path::new("out/a.d");
        let run = |command: &[&str], modern| {
            let command: Vec<String> = command.iter().map(|argument| argument.to_string()).collect();
            listing(&command, listed, modern).join(" ")
        };
        assert_eq!(run(&["bin/cpp0", "-lang-c", "a.c", "a.i"], false), "bin/cpp0 -MD out/a.d -lang-c a.c a.i");
        assert_eq!(run(&["bin/xgcc", "-E", "a.c"], false), "bin/xgcc -E a.c -Wp,-MD,out/a.d");
        assert_eq!(run(&["arm-none-eabi-gcc", "-E", "a.c"], true), "arm-none-eabi-gcc -E a.c -MD -MF out/a.d");
    }

    #[test]
    fn the_searched_folders_are_the_include_path_and_the_files_own_folders() {
        let command = ["cc", "-Ia", "-I", "b", "-isystem", "c", "-iquote", "d", "-idirafter", "e", "-DI=1", "x.c"]
            .map(str::to_owned);
        let found = searched(&command, ["src/x.c", "inc/y.h", "top.h"].into_iter());
        assert_eq!(found.into_iter().collect::<Vec<_>>(), ["", "a", "b", "c", "d", "e", "inc", "src"]);
    }
}
