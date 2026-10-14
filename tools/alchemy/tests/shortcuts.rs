//! The build's two shortcuts, attacked on a real build.
//!
//! `alchemy build rom` skips work in two places. A C source is not
//! preprocessed again while every file its preprocessor read is the same
//! (reads.rs), and an edition is not built at all while no file it reads and
//! none it made looks any different on disk (settled.rs). Either one, wrong,
//! would keep a ROM these sources no longer build, and a compare would print
//! OK for it (W5).
//!
//! Each test copies this checkout's sources and its built English edition of
//! The Broken Seal into out/test/NAME, changes something there, runs the
//! built `alchemy` and hashes the ROM against rom.sha1, as
//! `make compare-tbs-en` does. Nothing is timed: the tests read what the
//! build says it did. Run them with `make test-integration`, after
//! `make compare-tbs-en`.

use sha1::{Digest, Sha1};
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::Mutex;

const EDITION: &str = "tbs-en";
const OUTPUT: &str = "out/tbs-en";
const IMAGE: &str = "out/tbs-en/tbs-en.gba";
const READS: &str = "out/tbs-en/reads.tsv";
/// The showcase (S3) and the object the build makes of it.
const SOURCE: &str = "games/tbs/src/battle/battle.c";
const OBJECT: &str = "out/tbs-en/obj/games/tbs/src/battle/battle.o";
const TOOLCHAIN: [&str; 3] = ["tools/out/binutils", "tools/out/compiler-runtime", "tools/out/compilers"];

/// One build at a time: each runs its steps on every core.
static ONE: Mutex<()> = Mutex::new(());

/// A build keeps no record of an input younger than two seconds
/// (settled::SETTLING), so a test waits them out after it writes one.
fn settle() {
    std::thread::sleep(std::time::Duration::from_millis(2100));
}

fn copy(from: &Path, into: &Path, skip: &[&str]) {
    fs::create_dir_all(into).unwrap();
    for entry in fs::read_dir(from).unwrap() {
        let entry = entry.unwrap();
        let name = entry.file_name();
        if name.to_str().is_some_and(|name| skip.contains(&name)) {
            continue;
        }
        let kind = entry.file_type().unwrap();
        if kind.is_dir() {
            copy(&entry.path(), &into.join(&name), skip);
        } else if kind.is_file() {
            fs::copy(entry.path(), into.join(&name)).unwrap();
        }
    }
}

/// A copy of this checkout's sources and built edition, with the installed
/// toolchain linked as a worktree links it.
struct Checkout {
    root: PathBuf,
}

/// What one build said and left.
struct Built {
    passed: bool,
    stdout: String,
    stderr: String,
    /// Whether the ROM on disk is the one rom.sha1 names.
    identical: bool,
}

impl Built {
    fn field(&self, name: &str) -> Option<usize> {
        self.stdout.lines().find_map(|line| line.strip_prefix(name)?.strip_prefix('=')?.parse().ok())
    }

    /// Whether the build stopped at once, on the record of the last one.
    fn settled(&self) -> bool {
        self.field("settled") == Some(1)
    }

    fn steps(&self) -> usize {
        self.field("steps").unwrap_or_else(|| panic!("no steps line: {}{}", self.stdout, self.stderr))
    }

    fn preprocessed(&self) -> usize {
        self.field("preprocessed").unwrap_or_else(|| panic!("no preprocessed line: {}{}", self.stdout, self.stderr))
    }

    /// What `make compare-tbs-en` prints OK for: a build that finished, and
    /// the reference's ROM.
    fn ok(&self) -> bool {
        self.passed && self.identical
    }

    fn said(&self) -> String {
        format!("passed={} identical={}\n{}{}", self.passed, self.identical, self.stdout, self.stderr)
    }
}

impl Checkout {
    fn new(name: &str) -> Checkout {
        let real = Path::new(env!("CARGO_MANIFEST_DIR")).parent().unwrap().parent().unwrap();
        assert!(real.join(OUTPUT).join("cache.tsv").is_file(), "run make compare-tbs-en first");
        let root = real.join("out/test").join(name);
        if root.exists() {
            fs::remove_dir_all(&root).unwrap();
        }
        // What marks a checkout's top for alchemy, beside games/.
        fs::create_dir_all(root.join("tools/alchemy")).unwrap();
        fs::write(root.join("tools/alchemy/Cargo.toml"), "").unwrap();
        fs::create_dir_all(root.join("tools/out")).unwrap();
        copy(&real.join("games"), &root.join("games"), &[".DS_Store"]);
        // Not the RTL dumps `alchemy diff --rtl` leaves there.
        copy(&real.join(OUTPUT), &root.join(OUTPUT), &["drafts"]);
        fs::copy(real.join("rom.sha1"), root.join("rom.sha1")).unwrap();
        for linked in TOOLCHAIN {
            let installed = fs::canonicalize(real.join(linked)).unwrap_or_else(|_| panic!("{linked}: run make bootstrap"));
            std::os::unix::fs::symlink(installed, root.join(linked)).unwrap();
        }
        settle();
        Checkout { root }
    }

    fn run(&self, reverify: bool) -> Built {
        let mut command = Command::new(env!("CARGO_BIN_EXE_alchemy"));
        command.args(["build", "rom", "--target", EDITION]).current_dir(&self.root).env_remove("ALCHEMY_REVERIFY");
        if reverify {
            command.env("ALCHEMY_REVERIFY", "1");
        }
        let output = command.output().unwrap();
        Built {
            passed: output.status.success(),
            stdout: String::from_utf8_lossy(&output.stdout).into_owned(),
            stderr: String::from_utf8_lossy(&output.stderr).into_owned(),
            identical: self.identical(),
        }
    }

    fn build(&self) -> Built {
        self.run(false)
    }

    /// The build as `alchemy verify` runs it: everything by content.
    fn reverified(&self) -> Built {
        self.run(true)
    }

    /// The copy built and its build recorded: the next build of it, with
    /// nothing changed, stops at once.
    fn built(name: &str) -> Checkout {
        let checkout = Checkout::new(name);
        let first = checkout.build();
        assert!(first.ok(), "the first build of a copy: {}", first.said());
        let second = checkout.build();
        assert!(second.ok() && second.settled(), "a build with nothing changed: {}", second.said());
        checkout
    }

    fn identical(&self) -> bool {
        let listed = self.read("rom.sha1");
        let expected = listed
            .lines()
            .filter_map(|line| line.split_once(char::is_whitespace))
            .find(|(_, path)| path.trim().trim_start_matches('*') == IMAGE)
            .map(|(digest, _)| digest.to_ascii_lowercase())
            .expect("rom.sha1 names the edition");
        fs::read(self.path(IMAGE)).is_ok_and(|bytes| format!("{:x}", Sha1::digest(bytes)) == expected)
    }

    fn path(&self, path: &str) -> PathBuf {
        self.root.join(path)
    }

    fn read(&self, path: &str) -> String {
        fs::read_to_string(self.path(path)).unwrap()
    }

    /// Writes a file and waits until a build may record it.
    fn write(&self, path: &str, text: &str) {
        fs::write(self.path(path), text).unwrap();
        settle();
    }

    fn append(&self, path: &str, text: &str) {
        self.write(path, &(self.read(path) + text));
    }

    /// Writes a file and puts the time it was written back, as a tool that
    /// keeps times does: with the same size it looks the same to a listing.
    fn write_keeping_time(&self, path: &str, bytes: &[u8]) {
        let file = self.path(path);
        let before = fs::metadata(&file).unwrap();
        fs::write(&file, bytes).unwrap();
        fs::File::options().write(true).open(&file).unwrap().set_modified(before.modified().unwrap()).unwrap();
        let after = fs::metadata(&file).unwrap();
        assert_eq!((after.len(), after.modified().unwrap()), (before.len(), before.modified().unwrap()));
        settle();
    }

    /// A header few objects read, and how many: the fewest above one, the
    /// first by name among those.
    fn header_with_few_readers(&self) -> (String, usize) {
        let mut readers: BTreeMap<&str, usize> = BTreeMap::new();
        let reads = self.read(READS);
        for row in reads.lines() {
            for field in row.split('\t').filter(|field| field.starts_with("games/") && field.ends_with(".h")) {
                *readers.entry(field).or_default() += 1;
            }
        }
        let (header, count) = readers
            .into_iter()
            .filter(|(_, count)| *count > 1)
            .min_by_key(|(_, count)| *count)
            .expect("a header two objects read");
        (header.to_owned(), count)
    }
}

impl Drop for Checkout {
    /// A failed test leaves its copy to be looked at.
    fn drop(&mut self) {
        if !std::thread::panicking() {
            let _ = fs::remove_dir_all(&self.root);
        }
    }
}

/// The showcase with one case label of BtlEfxDeadChk changed: a function of
/// the same size, so the image links and is another ROM, from a file of the
/// same length.
fn changed(original: &str) -> String {
    let function = original.find("s32 BtlEfxDeadChk(s32 efx)\n{").expect("the showcase defines BtlEfxDeadChk");
    let label = function + original[function..].find("case 31:").expect("BtlEfxDeadChk tests effect 31");
    let mut text = original.to_owned();
    text.replace_range(label..label + "case 31:".len(), "case 33:");
    text
}

#[test]
#[ignore = "builds in a copy of this checkout"]
fn a_copied_build_is_warm_and_only_an_untouched_one_is_settled() {
    let _one = ONE.lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    let checkout = Checkout::new("settled");
    // The copy brought the record of the checkout it was copied from. Its
    // files are other files, so that record vouches for nothing here; by
    // content every product is what these sources build.
    let first = checkout.build();
    assert!(first.ok() && !first.settled(), "the first build of a copy: {}", first.said());
    assert_eq!(first.steps(), 0, "{}", first.said());
    let second = checkout.build();
    assert!(second.ok() && second.settled(), "a build with nothing changed: {}", second.said());
    assert_eq!(second.field("objects"), first.field("objects"));

    // Verification never rests on the record.
    let verified = checkout.reverified();
    assert!(verified.ok() && !verified.settled(), "{}", verified.said());
    assert_eq!(verified.steps(), 0, "{}", verified.said());

    // A source written again with the same bytes looks different, so the
    // build runs; by content it has nothing to do.
    checkout.write(SOURCE, &checkout.read(SOURCE));
    let touched = checkout.build();
    assert!(touched.ok() && !touched.settled(), "a source written again: {}", touched.said());
    assert_eq!((touched.preprocessed(), touched.steps()), (0, 0), "{}", touched.said());
    let after = checkout.build();
    assert!(after.ok() && after.settled(), "{}", after.said());
}

#[test]
#[ignore = "builds in a copy of this checkout"]
fn a_changed_source_is_built_again_however_it_looks() {
    let _one = ONE.lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    let checkout = Checkout::built("changed");
    let original = checkout.read(SOURCE);

    // Another function in a file of the same size, written at the same time.
    checkout.write_keeping_time(SOURCE, changed(&original).as_bytes());
    let built = checkout.build();
    assert!(!built.settled(), "a changed source of the same size and time: {}", built.said());
    assert!(built.passed && !built.identical, "its ROM is not the reference's: {}", built.said());
    // Its object, and the image that links it.
    assert!(built.preprocessed() == 1 && built.steps() >= 2, "{}", built.said());

    // A mismatch is the build's result, not a reason to build again, and no
    // later build forgets it.
    let again = checkout.build();
    assert!(again.settled() && again.passed && !again.identical, "{}", again.said());
    let verified = checkout.reverified();
    assert!(verified.passed && !verified.identical && verified.steps() == 0, "{}", verified.said());

    // Back to the first text: built again, never taken from the other's.
    checkout.write_keeping_time(SOURCE, original.as_bytes());
    let back = checkout.build();
    assert!(!back.settled() && back.steps() >= 2 && back.ok(), "the first text again: {}", back.said());
}

#[test]
#[ignore = "builds in a copy of this checkout"]
fn a_damaged_product_is_made_again() {
    let _one = ONE.lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    let checkout = Checkout::built("damaged");

    // A truncated object is not what its row records: compiled again, to
    // the same bytes, so nothing is linked.
    let object = fs::read(checkout.path(OBJECT)).unwrap();
    fs::write(checkout.path(OBJECT), &object[..object.len() / 2]).unwrap();
    let built = checkout.build();
    assert!(!built.settled() && built.ok(), "a truncated object: {}", built.said());
    assert_eq!(built.steps(), 1, "{}", built.said());
    assert!(fs::read(checkout.path(OBJECT)).unwrap() == object);

    // A ROM written over, its time put back, is linked again.
    let mut rom = fs::read(checkout.path(IMAGE)).unwrap();
    rom[0x1000] ^= 0xff;
    checkout.write_keeping_time(IMAGE, &rom);
    assert!(!checkout.identical());
    let built = checkout.build();
    assert!(!built.settled() && built.ok(), "a ROM written over: {}", built.said());
    assert_eq!(built.steps(), 1, "{}", built.said());

    // Without the record of what each C source read, every one is
    // preprocessed to learn it again, and none is compiled.
    fs::remove_file(checkout.path(READS)).unwrap();
    let built = checkout.build();
    assert!(!built.settled() && built.ok(), "no record of the files read: {}", built.said());
    assert!(built.preprocessed() > 1 && built.steps() == 0, "{}", built.said());
}

#[test]
#[ignore = "builds in a copy of this checkout"]
fn a_header_reaches_its_readers_and_a_failed_build_is_no_result() {
    let _one = ONE.lock().unwrap_or_else(|poisoned| poisoned.into_inner());
    let checkout = Checkout::built("header");

    // A comment in one source: preprocessed, found to be the same text,
    // and nothing is compiled.
    checkout.append(SOURCE, "// a comment\n");
    let built = checkout.build();
    assert!(!built.settled() && built.ok(), "a comment in a source: {}", built.said());
    assert_eq!((built.preprocessed(), built.steps()), (1, 0), "{}", built.said());

    // A comment in a header: exactly the objects whose preprocessor read it.
    let (header, readers) = checkout.header_with_few_readers();
    checkout.append(&header, "// a comment\n");
    let built = checkout.build();
    assert!(!built.settled() && built.ok(), "a comment in {header}: {}", built.said());
    assert_eq!((built.preprocessed(), built.steps()), (readers, 0), "{header}: {}", built.said());

    // A source that does not compile: the build fails, and fails again. The
    // ROM on disk is the last good one, and is not these sources' ROM.
    checkout.append(SOURCE, "#error stop here\n");
    let failed = checkout.build();
    assert!(!failed.passed && !failed.settled(), "{}", failed.said());
    assert!(failed.stderr.contains("stop here"), "{}", failed.said());
    settle();
    let again = checkout.build();
    assert!(!again.passed && !again.settled(), "a failed build is tried again, never skipped: {}", again.said());
}
