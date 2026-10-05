//! `alchemy keep`: of the source changed in the working tree, keep only what
//! builds byte-identical. A rewrite of maintained C is tried as a batch of
//! changed files; this reverts to HEAD the files the builds reject and
//! bisects the rest against the Japanese builds of both games, then checks
//! what survives in all twelve editions. The builds are the Makefile's own
//! compare targets run with this executable; the reference ROMs only judge.
use crate::compiler::routing::root;
use crate::targets::TARGET_IDS;
use regex::Regex;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::OnceLock;
use std::time::Instant;

const USAGE: &str = r#"usage: alchemy keep [PATH...]
Keeps only the changes under games/ (the working tree against HEAD) that
leave every edition byte-identical and reverts the others to HEAD. Files
under a PATH are never reverted: headers or linker scripts the other
changes need; a changed file whose HEAD copy no longer compiles beside
them is kept as well. First it reverts what the Japanese builds of either
game cannot compile, then bisects the rest, headers first, with make
compare and compare-tla; then it builds all twelve editions, reverts kept
files another edition cannot compile, bisects a byte difference only
another edition shows, and names any edition that still differs (exit 1).
Prints the counts and each reverted path with its reason. When the
Japanese builds fail even with every change reverted, it restores every
change and stops. The changed files as they were stay under
out/keep/saved; each edition's last build log is out/keep/EDITION.log."#;

const JAPANESE: [&str; 2] = ["tbs-ja", "tla-ja"];

fn editions() -> Vec<&'static str> {
    TARGET_IDS.iter().map(|id| id.as_str()).collect()
}

/// What one edition's compare target said.
#[derive(Clone, Debug, Default, PartialEq)]
pub(crate) struct Outcome {
    /// The image was compared with the reference ROM.
    compared: bool,
    identical: bool,
    /// Each source the build could not make.
    failures: Vec<Failure>,
    /// The first error line, for people.
    error: Option<String>,
}

#[derive(Clone, Debug, PartialEq)]
pub(crate) struct Failure {
    source: String,
    /// Files under games/ that its error lines (not warnings) name.
    locations: Vec<String>,
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum Reason {
    Compile,
    Bytes,
    EditionCompile,
    EditionBytes,
}

impl Reason {
    fn text(self) -> &'static str {
        match self {
            Reason::Compile => "does not compile",
            Reason::Bytes => "bytes differ",
            Reason::EditionCompile => "another edition cannot compile it",
            Reason::EditionBytes => "bytes differ in another edition",
        }
    }
}

#[derive(Debug, Default)]
pub(crate) struct Report {
    kept: Vec<String>,
    /// Each reverted file and why, in the order decided.
    reverted: Vec<(Reason, String)>,
    /// Kept files the Japanese builds cannot compile at HEAD beside the
    /// PATHs' changes, so never bisected.
    needed: Vec<String>,
    /// Editions that differ even with every change reverted.
    differ_without: Vec<&'static str>,
    /// The last build of all twelve editions, of the kept changes.
    last: Vec<(&'static str, Outcome)>,
}

/// Split `candidates` into those that pass together and those that fail, by
/// delta debugging: a group joins the good set when the good set with it
/// passes; otherwise its halves are tried in turn, and a single file that
/// fails is bad. When the first half joins whole, the good set with the
/// second half is the set that just failed, so that half is split without
/// asking again. `passes` must hold for the empty set.
pub(crate) fn bisect<T: Clone, E>(
    candidates: &[T],
    mut passes: impl FnMut(&[T]) -> Result<bool, E>,
) -> Result<(Vec<T>, Vec<T>), E> {
    let (mut good, mut bad) = (Vec::new(), Vec::new());
    split(candidates, false, &mut good, &mut bad, &mut passes)?;
    Ok((good, bad))
}

fn split<T: Clone, E>(
    group: &[T],
    failing: bool,
    good: &mut Vec<T>,
    bad: &mut Vec<T>,
    passes: &mut impl FnMut(&[T]) -> Result<bool, E>,
) -> Result<(), E> {
    if group.is_empty() {
        return Ok(());
    }
    if !failing {
        let trial: Vec<T> = good.iter().chain(group).cloned().collect();
        if passes(&trial)? {
            good.extend_from_slice(group);
            return Ok(());
        }
    }
    if let [one] = group {
        bad.push(one.clone());
        return Ok(());
    }
    let (first, second) = group.split_at(group.len() / 2);
    let before = good.len();
    split(first, false, good, bad, passes)?;
    split(
        second,
        good.len() - before == first.len(),
        good,
        bad,
        passes,
    )
}

/// The builds `decide` asks for, remembered by the set of changes applied:
/// the builds are deterministic, so an edition is built once for a set.
struct Builds<B> {
    build: B,
    known: BTreeMap<Vec<String>, BTreeMap<&'static str, Outcome>>,
}

impl<B> Builds<B>
where
    B: FnMut(&[&'static str], &[String]) -> Result<Vec<Outcome>, String>,
{
    fn run(&mut self, editions: &[&'static str], set: &[String]) -> Result<Vec<Outcome>, String> {
        let mut key = set.to_vec();
        key.sort();
        let Builds { build, known } = self;
        let known = known.entry(key).or_default();
        let missing: Vec<&'static str> = editions
            .iter()
            .copied()
            .filter(|edition| !known.contains_key(edition))
            .collect();
        if !missing.is_empty() {
            let outcomes = build(&missing, set)?;
            known.extend(missing.into_iter().zip(outcomes));
        }
        Ok(editions
            .iter()
            .map(|edition| known[edition].clone())
            .collect())
    }
}

fn identical(outcomes: &[Outcome]) -> bool {
    outcomes.iter().all(|outcome| outcome.identical)
}

fn failing(editions: &[&'static str], outcomes: &[Outcome]) -> Vec<&'static str> {
    editions
        .iter()
        .zip(outcomes)
        .filter(|(_, outcome)| !outcome.identical)
        .map(|(edition, _)| *edition)
        .collect()
}

/// The applied changes the builds rejected: a failing source that changed
/// when its own lines carry the errors (or none do), else the changed files
/// the error lines name, else the changed source. A failure that names no
/// changed file is left to the bisect.
fn blame(outcomes: &[Outcome], applied: &[String]) -> Vec<String> {
    let mut blamed = BTreeSet::new();
    for failure in outcomes.iter().flat_map(|outcome| &outcome.failures) {
        let changed = |path: &String| applied.contains(path);
        let own = failure.locations.is_empty() || failure.locations.contains(&failure.source);
        if changed(&failure.source) && own {
            blamed.insert(failure.source.clone());
            continue;
        }
        let named: Vec<&String> = failure
            .locations
            .iter()
            .filter(|path| changed(path))
            .collect();
        if !named.is_empty() {
            blamed.extend(named.into_iter().cloned());
        } else if changed(&failure.source) {
            blamed.insert(failure.source.clone());
        }
    }
    blamed.into_iter().collect()
}

fn revert(kept: &mut Vec<String>, report: &mut Report, files: Vec<String>, reason: Reason) {
    kept.retain(|path| !files.contains(path));
    report
        .reverted
        .extend(files.into_iter().map(|path| (reason, path)));
}

/// The changed files the Japanese builds cannot do without once every
/// other change is reverted: sources that no longer compile at HEAD beside
/// what the PATHs keep. Any other failure there leaves nothing to bisect.
fn needed<B>(builds: &mut Builds<B>, kept: &[String]) -> Result<Vec<String>, String>
where
    B: FnMut(&[&'static str], &[String]) -> Result<Vec<Outcome>, String>,
{
    let mut needed: Vec<String> = Vec::new();
    loop {
        let outcomes = builds.run(&JAPANESE, &needed)?;
        if identical(&outcomes) {
            return Ok(needed);
        }
        let more: BTreeSet<String> = outcomes
            .iter()
            .flat_map(|outcome| &outcome.failures)
            .map(|failure| &failure.source)
            .filter(|source| kept.contains(source) && !needed.contains(source))
            .cloned()
            .collect();
        if more.is_empty() {
            let but = if needed.is_empty() {
                String::new()
            } else {
                format!(" but {}", needed.join(", "))
            };
            return Err(format!(
                "with every change{but} reverted the Japanese builds still fail ({}); fix HEAD, or name what the other changes need as PATHs",
                describe(&JAPANESE, &outcomes)
            ));
        }
        needed.extend(more);
    }
}

/// The kept files whose changes `editions` show as a byte difference: a
/// bisect of all but `needed`, which stay applied throughout.
fn differing_bytes<B>(
    builds: &mut Builds<B>,
    editions: &[&'static str],
    kept: &[String],
    needed: &[String],
) -> Result<Vec<String>, String>
where
    B: FnMut(&[&'static str], &[String]) -> Result<Vec<Outcome>, String>,
{
    let rest: Vec<String> = kept
        .iter()
        .filter(|path| !needed.contains(path))
        .cloned()
        .collect();
    let (_, bad) = bisect(&rest, |set: &[String]| {
        let trial: Vec<String> = needed.iter().chain(set).cloned().collect();
        builds
            .run(editions, &trial)
            .map(|outcomes| identical(&outcomes))
    })?;
    Ok(bad)
}

/// Which `candidates` to keep, given `build`, which applies a set of them
/// (the others at HEAD) and compares the editions it is given.
pub(crate) fn decide<B>(candidates: &[String], build: B) -> Result<Report, String>
where
    B: FnMut(&[&'static str], &[String]) -> Result<Vec<Outcome>, String>,
{
    let all = editions();
    let mut builds = Builds {
        build,
        known: BTreeMap::new(),
    };
    let mut report = Report::default();
    let mut kept = candidates.to_vec();
    // What the Japanese builds cannot compile, then a bisect by bytes.
    loop {
        let outcomes = builds.run(&JAPANESE, &kept)?;
        let rejected = blame(&outcomes, &kept);
        if rejected.is_empty() {
            break;
        }
        revert(&mut kept, &mut report, rejected, Reason::Compile);
    }
    if !identical(&builds.run(&JAPANESE, &kept)?) {
        report.needed = needed(&mut builds, &kept)?;
        let bad = differing_bytes(&mut builds, &JAPANESE, &kept, &report.needed)?;
        revert(&mut kept, &mut report, bad, Reason::Bytes);
    }
    // All twelve: what another edition cannot compile, then, once, a bisect
    // against the editions that differ only with the changes.
    let mut bisected = false;
    let last = loop {
        let outcomes = builds.run(&all, &kept)?;
        let rejected = blame(&outcomes, &kept);
        if !rejected.is_empty() {
            revert(&mut kept, &mut report, rejected, Reason::EditionCompile);
            continue;
        }
        let differing = failing(&all, &outcomes);
        if differing.is_empty() || bisected {
            break outcomes;
        }
        bisected = true;
        let needed: Vec<String> = report
            .needed
            .iter()
            .filter(|path| kept.contains(path))
            .cloned()
            .collect();
        let baseline = builds.run(&differing, &needed)?;
        let (suspects, without): (Vec<_>, Vec<_>) = differing
            .into_iter()
            .zip(&baseline)
            .partition(|(_, outcome)| outcome.identical);
        report.differ_without = without.into_iter().map(|(edition, _)| edition).collect();
        let suspects: Vec<&'static str> =
            suspects.into_iter().map(|(edition, _)| edition).collect();
        if suspects.is_empty() {
            break outcomes;
        }
        let bad = differing_bytes(&mut builds, &suspects, &kept, &needed)?;
        revert(&mut kept, &mut report, bad, Reason::EditionBytes);
    };
    report.needed.retain(|path| kept.contains(path));
    report.last = all.into_iter().zip(last).collect();
    report.kept = kept;
    Ok(report)
}

fn status(outcome: &Outcome) -> String {
    if outcome.identical {
        "identical".into()
    } else if !outcome.failures.is_empty() {
        let sources: BTreeSet<&str> = outcome.failures.iter().map(|f| f.source.as_str()).collect();
        let elsewhere: BTreeSet<&str> = outcome
            .failures
            .iter()
            .flat_map(|f| &f.locations)
            .map(String::as_str)
            .filter(|path| !sources.contains(path))
            .collect();
        let mut text = format!("cannot compile {}", some(&sources));
        if !elsewhere.is_empty() {
            text += &format!(" (errors in {})", some(&elsewhere));
        }
        text
    } else if outcome.compared {
        "differs".into()
    } else {
        format!(
            "does not build: {}",
            outcome.error.as_deref().unwrap_or("see its log")
        )
    }
}

/// Up to three paths, then how many more.
fn some(paths: &BTreeSet<&str>) -> String {
    let shown: Vec<&str> = paths.iter().take(3).copied().collect();
    match paths.len() {
        0..=3 => shown.join(", "),
        n => format!("{} and {} more", shown.join(", "), n - 3),
    }
}

fn describe(editions: &[&'static str], outcomes: &[Outcome]) -> String {
    let lines: Vec<String> = editions
        .iter()
        .zip(outcomes)
        .filter(|(_, outcome)| !outcome.identical)
        .map(|(edition, outcome)| format!("{edition} {}", status(outcome)))
        .collect();
    lines.join("; ")
}

/// `PATH` from the command line as a path from the checkout's root.
fn kept_path(cwd: &Path, root: &Path, argument: &str) -> Result<String, String> {
    let path: PathBuf = cwd.join(argument).components().collect();
    let relative = path
        .strip_prefix(root)
        .map_err(|_| format!("{argument}: not in this checkout"))?;
    let text = relative.to_string_lossy().into_owned();
    if text.is_empty() {
        return Err(format!("{argument}: names the whole checkout"));
    }
    Ok(text)
}

fn is_kept(path: &str, kept: &[String]) -> bool {
    kept.iter().any(|k| {
        path == k
            || path
                .strip_prefix(k.as_str())
                .is_some_and(|rest| rest.starts_with('/'))
    })
}

fn is_header(path: &str) -> bool {
    matches!(
        Path::new(path).extension().and_then(|e| e.to_str()),
        Some("H" | "h" | "INC" | "inc")
    )
}

fn failed_source(line: &str) -> Option<(String, &str)> {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    let pattern = PATTERN
        .get_or_init(|| Regex::new(r"^(?:error: )?(games/[^:]+): [^\s:]+ failed(?:: )?").unwrap());
    let found = pattern.captures(line)?;
    Some((found[1].to_string(), &line[found.get(0)?.end()..]))
}

/// The file an error line names: `games/PATH:LINE: message`, not a warning.
fn error_location(text: &str) -> Option<String> {
    static PATTERN: OnceLock<Regex> = OnceLock::new();
    let pattern = PATTERN.get_or_init(|| Regex::new(r"(?:^|\s)(games/[^:]+):\d+: (.*)$").unwrap());
    let found = pattern.captures(text)?;
    let message = found[2].to_ascii_lowercase();
    (!message.starts_with("warning")).then(|| found[1].to_string())
}

/// One edition's compare log, with this checkout's root taken out of its
/// paths: the verdict on its image and each source it could not make.
pub(crate) fn read_log(edition: &str, log: &str) -> Outcome {
    let image = format!("out/{edition}/{edition}.gba: ");
    let mut outcome = Outcome::default();
    for line in log.lines() {
        if let Some(verdict) = line.find(&image).map(|at| &line[at + image.len()..]) {
            outcome.compared |= verdict.starts_with("OK") || verdict.starts_with("FAILED");
            outcome.identical = verdict == "OK";
        }
        if outcome.error.is_none() && line.starts_with("error: ") {
            outcome.error = Some(line.to_string());
        }
        let rest = match failed_source(line) {
            Some((source, rest)) => {
                outcome.failures.push(Failure {
                    source,
                    locations: Vec::new(),
                });
                rest
            }
            None => line,
        };
        if let (Some(path), Some(failure)) = (error_location(rest), outcome.failures.last_mut()) {
            if !failure.locations.contains(&path) {
                failure.locations.push(path);
            }
        }
    }
    outcome
}

/// A changed file: its bytes at HEAD and in the working tree, none where
/// it does not exist.
struct Change {
    path: String,
    head: Option<Vec<u8>>,
    worked: Option<Vec<u8>>,
}

fn git(root: &Path, arguments: &[&str]) -> Result<Vec<u8>, String> {
    let output = Command::new("git")
        .args(arguments)
        .current_dir(root)
        .stdin(Stdio::null())
        .output()
        .map_err(|error| format!("git: {error}"))?;
    if !output.status.success() {
        let detail = String::from_utf8_lossy(&output.stderr);
        return Err(format!(
            "git {} failed: {}",
            arguments.join(" "),
            detail.trim()
        ));
    }
    Ok(output.stdout)
}

/// The files under games/ that differ from HEAD, except under `kept`.
fn changes(root: &Path, kept: &[String]) -> Result<Vec<Change>, String> {
    let listing = git(
        root,
        &[
            "diff",
            "--no-ext-diff",
            "--no-renames",
            "--name-status",
            "-z",
            "HEAD",
            "--",
            "games",
        ],
    )?;
    let listing = String::from_utf8(listing).map_err(|_| "git diff: paths are not UTF-8")?;
    let fields: Vec<&str> = listing
        .split('\0')
        .filter(|field| !field.is_empty())
        .collect();
    let mut found = Vec::new();
    for pair in fields.chunks(2) {
        let [status, path] = pair else {
            return Err("git diff: unexpected listing".into());
        };
        if is_kept(path, kept) {
            continue;
        }
        let head = if status.starts_with('A') {
            None
        } else {
            Some(git(root, &["cat-file", "blob", &format!("HEAD:{path}")])?)
        };
        let worked = match fs::read(root.join(path)) {
            Ok(bytes) => Some(bytes),
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => None,
            Err(error) => return Err(format!("{path}: {error}")),
        };
        found.push(Change {
            path: path.to_string(),
            head,
            worked,
        });
    }
    Ok(found)
}

fn put(file: &Path, content: Option<&[u8]>) -> Result<(), String> {
    if fs::read(file).ok().as_deref() == content {
        return Ok(());
    }
    let written = match content {
        Some(bytes) => file
            .parent()
            .map_or(Ok(()), fs::create_dir_all)
            .and_then(|()| fs::write(file, bytes)),
        None => fs::remove_file(file),
    };
    written.map_err(|error| format!("{}: {error}", file.display()))
}

/// The working tree with the changes in `set` and every other change at HEAD.
fn apply(root: &Path, changes: &[Change], set: &[String]) -> Result<(), String> {
    for change in changes {
        let wanted = if set.contains(&change.path) {
            &change.worked
        } else {
            &change.head
        };
        put(&root.join(&change.path), wanted.as_deref())?;
    }
    Ok(())
}

fn save(directory: &Path, changes: &[Change]) -> Result<(), String> {
    if directory.exists() {
        fs::remove_dir_all(directory)
            .map_err(|error| format!("{}: {error}", directory.display()))?;
    }
    for change in changes {
        if let Some(bytes) = &change.worked {
            put(&directory.join(&change.path), Some(bytes))?;
        }
    }
    Ok(())
}

/// `make compare-EDITION` with this executable, into the edition's log.
fn compare_edition(
    root: &Path,
    logs: &Path,
    executable: &Path,
    edition: &str,
) -> Result<Outcome, String> {
    let log = logs.join(format!("{edition}.log"));
    let file = fs::File::create(&log).map_err(|error| format!("{}: {error}", log.display()))?;
    let error_file = file.try_clone().map_err(|error| error.to_string())?;
    Command::new("make")
        .current_dir(root)
        .args([
            "--no-print-directory".to_string(),
            format!("ALCHEMY={}", executable.display()),
            format!("compare-{edition}"),
        ])
        .stdin(Stdio::null())
        .stdout(file)
        .stderr(error_file)
        .status()
        .map_err(|error| format!("cannot run make compare-{edition}: {error}"))?;
    let text = fs::read(&log).map_err(|error| format!("{}: {error}", log.display()))?;
    let text = String::from_utf8_lossy(&text).replace(&format!("{}/", root.display()), "");
    Ok(read_log(edition, &text))
}

/// Apply `set`, then compare `editions` side by side.
fn compare(
    root: &Path,
    changes: &[Change],
    logs: &Path,
    executable: &Path,
    editions: &[&'static str],
    set: &[String],
) -> Result<Vec<Outcome>, String> {
    apply(root, changes, set)?;
    eprint!(
        "{} with {} of {} changes: ",
        editions.join(" "),
        set.len(),
        changes.len()
    );
    let started = Instant::now();
    let outcomes = std::thread::scope(|scope| {
        let handles: Vec<_> = editions
            .iter()
            .map(|&edition| scope.spawn(move || compare_edition(root, logs, executable, edition)))
            .collect();
        handles
            .into_iter()
            .map(|handle| handle.join().expect("build thread"))
            .collect::<Result<Vec<_>, String>>()
    })?;
    let verdict = if identical(&outcomes) {
        "identical".to_string()
    } else {
        describe(editions, &outcomes)
    };
    eprintln!("{verdict} ({:.0}s)", started.elapsed().as_secs_f64());
    Ok(outcomes)
}

/// The object a map line places at `address`, its whole path.
fn object_at(map: &str, address: u32) -> Option<String> {
    map.lines().find_map(|line| {
        let line = line.trim_start();
        let rest = [".text", ".rodata", ".data"]
            .iter()
            .find_map(|s| line.strip_prefix(s))?;
        let (start, rest) = rest.trim_start().split_once(char::is_whitespace)?;
        let (size, path) = rest.trim_start().split_once(char::is_whitespace)?;
        let start = u32::from_str_radix(start.trim_start_matches("0x"), 16).ok()?;
        let size = u32::from_str_radix(size.trim_start_matches("0x"), 16).ok()?;
        let path = path.trim();
        (start == address && size != 0).then(|| {
            path.rsplit_once("/obj/")
                .map_or(path, |(_, p)| p)
                .to_string()
        })
    })
}

fn first_difference(root: &Path, edition: &str) -> Option<String> {
    let read = |name: String| fs::read(root.join("out").join(edition).join(name)).ok();
    let map = String::from_utf8_lossy(&read(format!("{edition}.map"))?).into_owned();
    let reference = read("baserom.gba".into())?;
    let built = read(format!("{edition}.gba"))?;
    let (address, offset, _) = crate::integrate::first_difference(&map, &reference, &built)?;
    let object = object_at(&map, address).unwrap_or_default();
    Some(format!("first at {address:#010x}+{offset:#x} in {object}"))
}

fn print(root: &Path, report: &Report) -> Result<(), String> {
    let count = |reasons: &[Reason]| {
        report
            .reverted
            .iter()
            .filter(|(reason, _)| reasons.contains(reason))
            .count()
    };
    println!(
        "kept {}, compile-reverted {}, byte-reverted {}, edition-reverted {}",
        report.kept.len(),
        count(&[Reason::Compile]),
        count(&[Reason::Bytes]),
        count(&[Reason::EditionCompile, Reason::EditionBytes])
    );
    for (reason, path) in &report.reverted {
        println!("reverted ({}) {path}", reason.text());
    }
    if !report.reverted.is_empty() {
        println!("their changes are saved under out/keep/saved");
    }
    for path in &report.needed {
        println!("kept (HEAD's copy does not compile with the PATHs) {path}");
    }
    let mut differing = Vec::new();
    for (edition, outcome) in report.last.iter().filter(|(_, outcome)| !outcome.identical) {
        differing.push(*edition);
        let detail = if report.differ_without.contains(edition) {
            "differs with every change reverted too".to_string()
        } else if outcome.compared && outcome.failures.is_empty() {
            let at = first_difference(root, edition).unwrap_or_default();
            format!("differs {at}").trim_end().to_string()
        } else {
            format!("{} (out/keep/{edition}.log)", status(outcome))
        };
        println!("{edition}: {detail}");
    }
    if differing.is_empty() {
        println!("all twelve editions OK");
        Ok(())
    } else {
        Err(format!("editions still differing: {}", differing.join(" ")))
    }
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|a| a == "-h" || a == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    if let Some(option) = arguments.iter().find(|a| a.starts_with('-')) {
        return Err(format!("unknown option {option}\n{USAGE}"));
    }
    let root = root();
    let cwd = std::env::current_dir().map_err(|error| error.to_string())?;
    let kept = arguments
        .iter()
        .map(|argument| kept_path(&cwd, root, argument))
        .collect::<Result<Vec<_>, _>>()?;
    let directory = root.join("out/keep");
    let unfinished = directory.join("unfinished");
    if unfinished.exists() {
        return Err("an earlier alchemy keep stopped before it finished: the changed files as they were are under out/keep/saved; copy back what you need, then delete out/keep/unfinished".into());
    }
    let mut changes = changes(root, &kept)?;
    if changes.is_empty() {
        println!("no changes under games/ to check");
        return Ok(());
    }
    // Headers first, so the bisect accepts a declaration before its users.
    changes.sort_by_key(|change| !is_header(&change.path));
    save(&directory.join("saved"), &changes)?;
    fs::write(&unfinished, "").map_err(|error| format!("{}: {error}", unfinished.display()))?;
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let paths: Vec<String> = changes.iter().map(|change| change.path.clone()).collect();
    let decided = decide(&paths, |editions, set| {
        compare(root, &changes, &directory, &executable, editions, set)
    });
    let settled = match &decided {
        Ok(report) => apply(root, &changes, &report.kept),
        Err(_) => apply(root, &changes, &paths),
    };
    settled?;
    fs::remove_file(&unfinished).map_err(|error| format!("{}: {error}", unfinished.display()))?;
    print(root, &decided?)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn names(range: std::ops::Range<usize>) -> Vec<String> {
        range.map(|n| format!("games/F{n:02}.C")).collect()
    }

    #[test]
    fn bisect_finds_two_bad_files_among_many() {
        let files = names(0..20);
        let bad = ["games/F04.C", "games/F13.C"];
        let mut asked = 0;
        let (good, rejected) = bisect(&files, |set: &[String]| {
            asked += 1;
            Ok::<_, ()>(!set.iter().any(|f| bad.contains(&f.as_str())))
        })
        .unwrap();
        assert_eq!(rejected, bad);
        let expected: Vec<String> = files
            .iter()
            .filter(|f| !bad.contains(&f.as_str()))
            .cloned()
            .collect();
        assert_eq!(good, expected);
        // One question for the whole set, then at most two on each of the
        // five levels above each bad file.
        assert!(asked <= 1 + 2 * 2 * 5, "asked {asked} times");
    }

    #[test]
    fn bisect_asks_once_when_all_pass_and_never_twice_for_a_known_failure() {
        let files = names(0..8);
        let mut asked = 0;
        let (good, bad) = bisect(&files, |_: &[String]| {
            asked += 1;
            Ok::<_, ()>(true)
        })
        .unwrap();
        assert_eq!((good.len(), bad.len(), asked), (8, 0, 1));
        // [a b] fails, [a] passes, so [b] is known to fail without a build.
        let mut asked = 0;
        let pair = names(0..2);
        let (good, bad) = bisect(&pair, |set: &[String]| {
            asked += 1;
            Ok::<_, ()>(!set.contains(&pair[1]))
        })
        .unwrap();
        assert_eq!(
            (good, bad, asked),
            (vec![pair[0].clone()], vec![pair[1].clone()], 2)
        );
    }

    #[test]
    fn bisect_stops_at_the_first_error() {
        let files = names(0..4);
        let result = bisect(&files, |set: &[String]| {
            if set.len() < 4 {
                Err("broken")
            } else {
                Ok(false)
            }
        });
        assert_eq!(result, Err("broken"));
    }

    fn pass() -> Outcome {
        Outcome {
            compared: true,
            identical: true,
            ..Outcome::default()
        }
    }

    fn differ() -> Outcome {
        Outcome {
            compared: true,
            ..Outcome::default()
        }
    }

    fn reject(source: &str) -> Outcome {
        Outcome {
            failures: vec![Failure {
                source: source.into(),
                locations: vec![source.into()],
            }],
            error: Some(format!("error: {source}: xgcc failed")),
            ..Outcome::default()
        }
    }

    /// A fake set of builds: `judge` gives one edition's outcome for a set.
    fn builds<'a>(
        judge: impl Fn(&str, &[String]) -> Outcome + 'a,
        log: &'a std::cell::RefCell<Vec<Vec<&'static str>>>,
    ) -> impl FnMut(&[&'static str], &[String]) -> Result<Vec<Outcome>, String> + 'a {
        move |editions, set| {
            log.borrow_mut().push(editions.to_vec());
            Ok(editions.iter().map(|edition| judge(edition, set)).collect())
        }
    }

    #[test]
    fn keeps_everything_with_two_builds_when_all_match() {
        let log = Default::default();
        let report = decide(&names(0..5), builds(|_, _| pass(), &log)).unwrap();
        assert_eq!(report.kept, names(0..5));
        assert!(report.reverted.is_empty());
        // The twelve-edition build reuses the Japanese results of the same set.
        let log = log.borrow();
        assert_eq!(log.len(), 2);
        assert_eq!(log[0], JAPANESE);
        assert_eq!(log[1].len(), 10);
    }

    #[test]
    fn reverts_compile_failures_then_bisects_bytes_then_another_editions_rejection() {
        let files = names(0..12);
        let (broken, wrong, english_only) = (files[2].clone(), files[7].clone(), files[9].clone());
        let log = Default::default();
        let judge = |edition: &str, set: &[String]| {
            if set.contains(&broken) && JAPANESE.contains(&edition) {
                reject(&broken)
            } else if set.contains(&english_only) && edition == "tbs-en" {
                reject(&english_only)
            } else if set.contains(&wrong) && edition == "tla-ja" {
                differ()
            } else {
                pass()
            }
        };
        let report = decide(&files, builds(judge, &log)).unwrap();
        assert_eq!(
            report.reverted,
            [
                (Reason::Compile, broken.clone()),
                (Reason::Bytes, wrong.clone()),
                (Reason::EditionCompile, english_only.clone())
            ]
        );
        let expected: Vec<String> = files
            .iter()
            .filter(|f| ![&broken, &wrong, &english_only].contains(f))
            .cloned()
            .collect();
        assert_eq!(report.kept, expected);
        assert!(report.last.iter().all(|(_, outcome)| outcome.identical));
        assert_eq!(report.last.len(), 12);
    }

    #[test]
    fn bisects_bytes_only_another_edition_shows() {
        let files = names(0..6);
        let french = files[4].clone();
        let log = Default::default();
        let judge = |edition: &str, set: &[String]| {
            if edition == "tla-fr" && set.contains(&french) {
                differ()
            } else {
                pass()
            }
        };
        let report = decide(&files, builds(judge, &log)).unwrap();
        assert_eq!(report.reverted, [(Reason::EditionBytes, french)]);
        assert_eq!(report.kept.len(), 5);
        assert!(report.differ_without.is_empty());
        assert!(report.last.iter().all(|(_, outcome)| outcome.identical));
    }

    #[test]
    fn blames_no_change_for_an_edition_that_differs_without_them() {
        let log = Default::default();
        let judge = |edition: &str, _: &[String]| {
            if edition == "tbs-de" {
                differ()
            } else {
                pass()
            }
        };
        let report = decide(&names(0..3), builds(judge, &log)).unwrap();
        assert!(report.reverted.is_empty());
        assert_eq!(report.differ_without, ["tbs-de"]);
        assert_eq!(report.kept.len(), 3);
    }

    #[test]
    fn keeps_without_bisecting_a_file_head_cannot_compile_beside_the_paths() {
        let files = names(0..4);
        let (renamed, wrong) = (files[1].clone(), files[3].clone());
        let log = Default::default();
        let judge = |edition: &str, set: &[String]| {
            if !set.contains(&renamed) {
                reject(&renamed)
            } else if set.contains(&wrong) && edition == "tbs-ja" {
                differ()
            } else {
                pass()
            }
        };
        let report = decide(&files, builds(judge, &log)).unwrap();
        assert_eq!(report.needed, [renamed.clone()]);
        assert_eq!(report.reverted, [(Reason::Bytes, wrong)]);
        assert_eq!(report.kept, [files[0].clone(), renamed, files[2].clone()]);
    }

    #[test]
    fn remembers_every_set_it_built() {
        let files = names(0..2);
        let wrong = files[1].clone();
        let log = Default::default();
        let judge = |_: &str, set: &[String]| {
            if set.contains(&wrong) {
                differ()
            } else {
                pass()
            }
        };
        let report = decide(&files, builds(judge, &log)).unwrap();
        assert_eq!(report.reverted, [(Reason::Bytes, wrong)]);
        // Both, then none, then the first alone; the pair is known to fail
        // and the second alone is known by the bisect; then the other ten.
        let log = log.borrow();
        assert_eq!(log.len(), 4, "{log:?}");
    }

    #[test]
    fn refuses_when_the_japanese_builds_fail_without_the_changes() {
        let log = Default::default();
        let judge = |edition: &str, _: &[String]| {
            if edition == "tbs-ja" {
                differ()
            } else {
                pass()
            }
        };
        let error = decide(&names(0..3), builds(judge, &log)).unwrap_err();
        assert!(error.contains("tbs-ja differs"), "{error}");
    }

    #[test]
    fn blames_the_changed_header_or_the_changed_source() {
        let header = "games/X/INCLUDE/A.H".to_string();
        let source = "games/X/SRC/B.C".to_string();
        let failure = |source: &str, locations: &[&str]| Outcome {
            failures: vec![Failure {
                source: source.into(),
                locations: locations.iter().map(|l| l.to_string()).collect(),
            }],
            ..Outcome::default()
        };
        let other = "games/X/SRC/C.C";
        // An unchanged file fails on a changed header's line.
        assert_eq!(
            blame(&[failure(other, &[&header])], &[header.clone()]),
            [header.clone()]
        );
        // A changed file's own error, even beside a note in a changed header.
        let both = [header.clone(), source.clone()];
        assert_eq!(
            blame(&[failure(&source, &[&source, &header])], &both),
            [source.clone()]
        );
        // A changed file that breaks an unchanged header.
        assert_eq!(
            blame(&[failure(&source, &["games/X/INCLUDE/Z.H"])], &both),
            [source.clone()]
        );
        // Nothing changed is named: left to the bisect.
        assert!(blame(&[failure(other, &[other])], &both).is_empty());
    }

    #[test]
    fn reads_failures_and_verdicts_from_a_compare_log() {
        let log = "\
cargo run -- build rom --target tbs-en
error: games/THE BROKEN SEAL/SRC/A.C: xgcc failed: games/THE BROKEN SEAL/SRC/A.C: In function `f':
games/THE BROKEN SEAL/SRC/A.C:58: warning: passing arg 2 of `g' makes pointer from integer
games/THE BROKEN SEAL/SRC/A.C:174: `nope' undeclared (first use in this function)
games/THE BROKEN SEAL/SRC/B.C: xgcc failed: In file included from games/THE BROKEN SEAL/SRC/B.C:2:
games/THE BROKEN SEAL/INCLUDE/AFFINE.H:32: parse error before `}'
games/THE BROKEN SEAL/INCLUDE/AFFINE.H:33: warning: This file contains more `}'s than `{'s.
games/COMMON/SRC/C.C: xgcc failed: games/COMMON/SRC/C.C:3: parse error before `int'
make: *** [compare-tbs-en] Error 1
";
        let outcome = read_log("tbs-en", log);
        assert!(!outcome.compared && !outcome.identical);
        assert_eq!(outcome.error.as_deref(), log.lines().nth(1));
        let read: Vec<(&str, Vec<&str>)> = outcome
            .failures
            .iter()
            .map(|f| {
                (
                    f.source.as_str(),
                    f.locations.iter().map(String::as_str).collect(),
                )
            })
            .collect();
        assert_eq!(
            read,
            [
                (
                    "games/THE BROKEN SEAL/SRC/A.C",
                    vec!["games/THE BROKEN SEAL/SRC/A.C"]
                ),
                (
                    "games/THE BROKEN SEAL/SRC/B.C",
                    vec!["games/THE BROKEN SEAL/INCLUDE/AFFINE.H"]
                ),
                ("games/COMMON/SRC/C.C", vec!["games/COMMON/SRC/C.C"]),
            ]
        );
        assert_eq!(
            status(&outcome),
            "cannot compile games/COMMON/SRC/C.C, games/THE BROKEN SEAL/SRC/A.C, games/THE BROKEN SEAL/SRC/B.C (errors in games/THE BROKEN SEAL/INCLUDE/AFFINE.H)"
        );
        let ok = read_log("tla-ja", "objects=1\nout/tla-ja/tla-ja.gba: OK\n");
        assert!(ok.compared && ok.identical && ok.failures.is_empty());
        let differs = read_log(
            "tla-ja",
            "out/tla-ja/tla-ja.gba: FAILED\nWARNING: 1 computed checksum did NOT match\n",
        );
        assert!(differs.compared && !differs.identical);
    }

    #[test]
    fn paths_to_keep_are_from_the_root_and_cover_directories() {
        let (root, cwd) = (Path::new("/r"), Path::new("/r/games"));
        assert_eq!(
            kept_path(Path::new("/r"), root, "./games/A/INCLUDE/").unwrap(),
            "games/A/INCLUDE"
        );
        assert_eq!(
            kept_path(cwd, root, "COMMON/X.H").unwrap(),
            "games/COMMON/X.H"
        );
        assert_eq!(kept_path(cwd, root, "/r/games/B.C").unwrap(), "games/B.C");
        assert!(kept_path(cwd, root, "/elsewhere/B.C").is_err());
        assert!(kept_path(Path::new("/r"), root, ".").is_err());
        let kept = ["games/A/INCLUDE".to_string()];
        assert!(is_kept("games/A/INCLUDE/X.H", &kept));
        assert!(!is_kept("games/A/INCLUDED.H", &kept));
        assert!(is_header("games/A/INCLUDE/X.H") && !is_header("games/A/SRC/X.C"));
    }

    #[test]
    fn names_the_whole_object_path_at_an_address() {
        let map = " .text          0x0000000008000000        0x4 /r/out/tbs-en/obj/games/THE BROKEN SEAL/SRC/A.o\n .data          0x0000000008000004        0x0 /r/out/tbs-en/obj/games/B.o\n .text          0x0000000008000004        0x8 /r/out/tbs-en/obj/games/THE BROKEN SEAL/SRC/C D.o\n";
        assert_eq!(
            object_at(map, 0x0800_0000).as_deref(),
            Some("games/THE BROKEN SEAL/SRC/A.o")
        );
        assert_eq!(
            object_at(map, 0x0800_0004).as_deref(),
            Some("games/THE BROKEN SEAL/SRC/C D.o")
        );
        assert_eq!(object_at(map, 0x0800_0010), None);
    }
}
