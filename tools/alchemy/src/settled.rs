//! A build that has nothing to do.
//!
//! Every build re-reads its inputs and products to prove its cache still
//! holds: right when anything may have changed, pure waste when nothing has.
//! After a build that ends with every product in place, this records how
//! every file it could have read looked from the outside (size, time
//! written, time changed), and how its products looked. The next build
//! takes that census again first. If not one file under the game sources,
//! the scaffolds, the ROMs, the toolchain or the edition's own output
//! differs, and this program is the same bytes, it prints what the last
//! build printed and stops. Any difference at all and the build runs in
//! full, by content, as before.
//!
//! This is the one place the build trusts a file's times instead of its
//! bytes, as make, ninja, cargo and git do. Four things bound that trust.
//! The time the file system itself records for any change to a file, which
//! no tool can set back, is part of the census. Nothing is recorded while an
//! input is younger than two seconds, or when an input changed during the
//! build. `ALCHEMY_REVERIFY=1` skips the shortcut, and `alchemy verify` sets
//! it, so a commit or a landing never rests on it. And every compare still
//! hashes the ROM itself against rom.sha1.

use sha2::{Digest, Sha256};
use std::fs;
use std::os::unix::fs::MetadataExt;
use std::path::{Path, PathBuf};
use std::time::{Duration, SystemTime, UNIX_EPOCH};

const FILE: &str = "settled.tsv";
const NEW: &str = "settled.tsv.new";
/// Everything a build reads besides its own output and this program.
const INPUTS: [&str; 6] =
    ["games", "recon", "roms", "tools/out/compilers", "tools/out/binutils", "tools/out/compiler-runtime"];
/// An input written this recently could change again inside the same tick of
/// a coarse file system clock without looking any different.
pub(crate) const SETTLING: Duration = Duration::from_secs(2);

/// How a set of files looks from the outside.
pub(crate) struct Census {
    digest: String,
    /// The latest time any of them was written or changed, in nanoseconds.
    newest: i128,
}

/// One line per file under each root: its name, size and both times. A root
/// that is a link is followed once; links below it are recorded, not
/// followed, so a folder that links back to itself ends the walk.
fn census(roots: &[(String, PathBuf)], skip: &[&str]) -> Census {
    let mut lines = Vec::new();
    let mut newest = 0;
    for (label, root) in roots {
        let mut folders = vec![fs::canonicalize(root).unwrap_or_else(|_| root.clone())];
        let top = folders[0].clone();
        while let Some(folder) = folders.pop() {
            let Ok(entries) = fs::read_dir(&folder) else {
                // A root that is one file, or is missing.
                if let Ok(seen) = fs::symlink_metadata(&folder) {
                    newest = newest.max(times(&seen).0.max(times(&seen).1));
                    lines.push(line(label, Path::new(""), &seen));
                } else {
                    lines.push(format!("{label}\0missing"));
                }
                continue;
            };
            for entry in entries.flatten() {
                let path = entry.path();
                // Skipped names are files of the top folder itself.
                if folder == top && entry.file_name().to_str().is_some_and(|name| skip.contains(&name)) {
                    continue;
                }
                let Ok(seen) = fs::symlink_metadata(&path) else { continue };
                if seen.is_dir() {
                    folders.push(path);
                    continue;
                }
                newest = newest.max(times(&seen).0.max(times(&seen).1));
                lines.push(line(label, path.strip_prefix(&top).unwrap_or(&path), &seen));
            }
        }
    }
    lines.sort_unstable();
    let mut hasher = Sha256::new();
    for line in &lines {
        hasher.update(line.as_bytes());
        hasher.update(b"\n");
    }
    Census { digest: format!("{:x}", hasher.finalize()), newest }
}

fn times(seen: &fs::Metadata) -> (i128, i128) {
    let nanoseconds = |seconds: i64, rest: i64| i128::from(seconds) * 1_000_000_000 + i128::from(rest);
    (nanoseconds(seen.mtime(), seen.mtime_nsec()), nanoseconds(seen.ctime(), seen.ctime_nsec()))
}

fn line(label: &str, name: &Path, seen: &fs::Metadata) -> String {
    let (written, changed) = times(seen);
    format!("{label}\0{}\0{}\0{written}\0{changed}", name.display(), seen.size())
}

/// The census of everything a build reads: the input folders, and this
/// program by its bytes. Its times cannot serve: cargo links the finished
/// program into place again on every `make`, which changes them each time.
pub(crate) fn inputs(root: &Path) -> Census {
    let roots: Vec<(String, PathBuf)> =
        INPUTS.iter().map(|folder| ((*folder).to_owned(), root.join(folder))).collect();
    let mut seen = census(&roots, &[]);
    let program = std::env::current_exe().and_then(fs::read).map_or_else(
        |_| "unreadable".to_owned(),
        |bytes| format!("{:x}", Sha256::digest(bytes)),
    );
    // And the two variables that add folders to the preprocessor's search.
    let search = ["CPATH", "C_INCLUDE_PATH"].map(|name| std::env::var(name).unwrap_or_default()).join("\0");
    seen.digest = format!("{:x}", Sha256::digest(format!("{}\nprogram {program}\n{search}", seen.digest)));
    seen
}

fn products(output: &Path) -> Census {
    census(&[("output".to_owned(), output.to_path_buf())], &[FILE, NEW])
}

/// What the last build of `name` into `output` printed, when nothing it read
/// and nothing it made looks any different since.
pub(crate) fn holds(output: &Path, name: &str, inputs: &Census) -> Option<String> {
    let text = fs::read_to_string(output.join(FILE)).ok()?;
    let mut lines = text.splitn(4, '\n');
    let same = lines.next()? == format!("name\t{name}")
        && lines.next()? == format!("inputs\t{}", inputs.digest)
        && lines.next()? == format!("products\t{}", products(output).digest);
    same.then(|| lines.next().unwrap_or_default().to_owned())
}

/// Before a build runs in full: whatever happens next, the last record is
/// no longer the last build.
pub(crate) fn forget(output: &Path) {
    let _ = fs::remove_file(output.join(FILE));
}

/// Records a finished build, with the `printed` lines a settled build is to
/// print again: only when the inputs still look as they did `before` it
/// started, and none of them is younger than `settling`.
pub(crate) fn record(root: &Path, output: &Path, name: &str, before: &Census, printed: &str, settling: Duration) {
    let path = output.join(FILE);
    let after = inputs(root);
    let now = SystemTime::now().duration_since(UNIX_EPOCH).map_or(0, |since| since.as_nanos() as i128);
    if after.digest != before.digest || after.newest + settling.as_nanos() as i128 > now {
        let _ = fs::remove_file(&path);
        return;
    }
    let text = format!("name\t{name}\ninputs\t{}\nproducts\t{}\n{printed}", after.digest, products(output).digest);
    let fresh = output.join(NEW);
    if fs::write(&fresh, text).and_then(|()| fs::rename(&fresh, &path)).is_err() {
        let _ = fs::remove_file(&path);
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A checkout with one input file, one product and a finished build.
    fn built() -> (tempfile::TempDir, PathBuf, PathBuf) {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path().to_path_buf();
        let output = root.join("out/tbs-en");
        fs::create_dir_all(root.join("games/tbs/src")).unwrap();
        fs::create_dir_all(output.join("obj")).unwrap();
        fs::write(root.join("games/tbs/src/a.c"), "int a;\n").unwrap();
        fs::write(output.join("obj/a.o"), "object").unwrap();
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\nrom=a\n", Duration::ZERO);
        (directory, root, output)
    }

    fn settled(root: &Path, output: &Path) -> Option<String> {
        holds(output, "tbs-en", &inputs(root))
    }

    #[test]
    fn a_build_with_nothing_changed_prints_what_the_last_one_printed() {
        let (_directory, root, output) = built();
        assert_eq!(settled(&root, &output).as_deref(), Some("steps=0\nrom=a\n"));
        assert_eq!(holds(&output, "tbs-de", &inputs(&root)), None);
    }

    #[test]
    fn any_input_that_looks_different_ends_it() {
        let (_directory, root, output) = built();
        let source = root.join("games/tbs/src/a.c");
        // The same size and the same bytes, written again: the times moved.
        fs::write(&source, "int a;\n").unwrap();
        assert_eq!(settled(&root, &output), None);
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\n", Duration::ZERO);
        assert!(settled(&root, &output).is_some());
        fs::write(root.join("games/tbs/src/b.h"), "").unwrap();
        assert_eq!(settled(&root, &output), None);
        fs::remove_file(root.join("games/tbs/src/b.h")).unwrap();
        // Gone again, but the folder is not what the census saw: the file's
        // line is absent either way, so the census is the recorded one.
        assert!(settled(&root, &output).is_some());
        fs::remove_file(&source).unwrap();
        assert_eq!(settled(&root, &output), None);
    }

    #[test]
    fn any_product_that_looks_different_ends_it() {
        let (_directory, root, output) = built();
        fs::write(output.join("obj/a.o"), "object").unwrap();
        assert_eq!(settled(&root, &output), None);
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\n", Duration::ZERO);
        fs::remove_file(output.join("obj/a.o")).unwrap();
        assert_eq!(settled(&root, &output), None);
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\n", Duration::ZERO);
        fs::write(output.join("new.map"), "").unwrap();
        assert_eq!(settled(&root, &output), None);
    }

    #[test]
    fn nothing_is_recorded_for_inputs_still_settling_or_changed_during_the_build() {
        let (_directory, root, output) = built();
        // The files of this test are a moment old: inside the real margin.
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\n", SETTLING);
        assert!(!output.join(FILE).exists());
        let before = inputs(&root);
        fs::write(root.join("games/tbs/src/a.c"), "int b;\n").unwrap();
        record(&root, &output, "tbs-en", &before, "steps=0\n", Duration::ZERO);
        assert!(!output.join(FILE).exists());
    }

    #[test]
    fn a_folder_that_links_to_itself_ends_the_walk() {
        let (_directory, root, output) = built();
        fs::create_dir_all(root.join("roms")).unwrap();
        std::os::unix::fs::symlink(root.join("roms"), root.join("roms/roms")).unwrap();
        fs::write(root.join("roms/tbs-en.gba"), "rom").unwrap();
        record(&root, &output, "tbs-en", &inputs(&root), "steps=0\n", Duration::ZERO);
        assert!(settled(&root, &output).is_some());
        fs::write(root.join("roms/tbs-en.gba"), "mor").unwrap();
        assert_eq!(settled(&root, &output), None);
    }
}
