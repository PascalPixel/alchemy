use std::cmp::Ordering;
use std::collections::{BTreeMap, BTreeSet};
use std::ffi::{CString, OsString};
use std::fs::{self, File};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, AtomicUsize, Ordering as AtomicOrdering};
use std::sync::{mpsc, Arc, Mutex};
use std::time::{Duration, Instant};

#[cfg(unix)]
use std::os::fd::{FromRawFd, RawFd};

use alchemy_routing::routing::root;

use crate::backend::{self, Backend, Measurement};
use crate::options::{
    Options, MAX_ITERATIONS, MAX_JOURNAL_ROW_BYTES, MAX_OUTPUT_BYTES, MAX_PLAN_BYTES,
    MAX_SOURCE_BYTES, MAX_SUMMARY_BYTES,
};
use crate::perm::Permutation;
use crate::randomize::Weights;

#[derive(Clone, Debug, Eq, PartialEq)]
struct Candidate {
    index: usize,
    manual_seed: usize,
    mutation: String,
    source: String,
    fingerprint: String,
}

#[derive(Debug)]
struct Evaluated {
    candidate: Candidate,
    measurement: Result<Measurement, String>,
    elapsed: Duration,
}

const RUN_MARKER: &str = ".permuter-run";
const ACTIVE_MARKER: &str = ".permuter-active";
const OWNERSHIP_MANIFEST: &str = ".permuter-owned";

#[cfg(unix)]
mod safe_fs {
    use super::{io, CString, File, FromRawFd, OsString, Path, RawFd};
    use std::ffi::OsStr;
    use std::io::{Read, Write};
    use std::os::raw::{c_char, c_int, c_uint};
    use std::os::unix::ffi::OsStrExt;

    const AT_FDCWD: c_int = -2;
    const O_RDONLY: c_int = 0;
    const O_WRONLY: c_int = 1;

    #[cfg(target_os = "linux")]
    const O_CREAT: c_int = 0x40;
    #[cfg(target_os = "linux")]
    const O_EXCL: c_int = 0x80;
    #[cfg(target_os = "linux")]
    const O_APPEND: c_int = 0x400;

    #[cfg(any(target_os = "macos", target_os = "ios"))]
    const O_CREAT: c_int = 0x200;
    #[cfg(any(target_os = "macos", target_os = "ios"))]
    const O_EXCL: c_int = 0x800;
    #[cfg(any(target_os = "macos", target_os = "ios"))]
    const O_APPEND: c_int = 0x8;

    #[cfg(target_os = "linux")]
    const O_DIRECTORY: c_int = 0x10_000;
    #[cfg(target_os = "linux")]
    const O_NOFOLLOW: c_int = 0x20_000;
    #[cfg(any(target_os = "macos", target_os = "ios"))]
    const O_DIRECTORY: c_int = 0x10_0000;
    #[cfg(any(target_os = "macos", target_os = "ios"))]
    const O_NOFOLLOW: c_int = 0x100;
    unsafe extern "C" {
        fn openat(dirfd: c_int, path: *const c_char, flags: c_int, ...) -> c_int;
        fn mkdirat(dirfd: c_int, path: *const c_char, mode: c_uint) -> c_int;
        fn unlinkat(dirfd: c_int, path: *const c_char, flags: c_int) -> c_int;
    }

    fn c_name(name: &OsStr) -> io::Result<CString> {
        CString::new(name.as_bytes()).map_err(|_| {
            io::Error::new(
                io::ErrorKind::InvalidInput,
                "output names must not contain NUL bytes",
            )
        })
    }

    fn open_at(dirfd: RawFd, name: &OsStr, flags: c_int, mode: c_uint) -> io::Result<File> {
        let name = c_name(name)?;
        let fd = unsafe { openat(dirfd, name.as_ptr(), flags, mode) };
        if fd < 0 {
            Err(io::Error::last_os_error())
        } else {
            Ok(unsafe { File::from_raw_fd(fd) })
        }
    }

    pub fn open_directory(path: &Path) -> io::Result<File> {
        let path = c_name(path.as_os_str())?;
        let fd = unsafe {
            openat(
                AT_FDCWD,
                path.as_ptr(),
                O_RDONLY | O_DIRECTORY | O_NOFOLLOW,
                0,
            )
        };
        if fd < 0 {
            Err(io::Error::last_os_error())
        } else {
            Ok(unsafe { File::from_raw_fd(fd) })
        }
    }

    pub fn open_directory_at(parent: &File, name: &OsStr) -> io::Result<File> {
        open_at(
            std::os::fd::AsRawFd::as_raw_fd(parent),
            name,
            O_RDONLY | O_DIRECTORY | O_NOFOLLOW,
            0,
        )
    }

    fn create_directory_at(parent: &File, name: &OsStr) -> io::Result<()> {
        let name = c_name(name)?;
        let result = unsafe {
            mkdirat(
                std::os::fd::AsRawFd::as_raw_fd(parent),
                name.as_ptr(),
                0o700,
            )
        };
        if result < 0 {
            Err(io::Error::last_os_error())
        } else {
            Ok(())
        }
    }

    pub fn open_or_create_directory(
        root: &Path,
        components: &[OsString],
    ) -> io::Result<(File, bool)> {
        let mut current = open_directory(root)?;
        let mut final_existed = true;
        for (index, component) in components.iter().enumerate() {
            let last = index + 1 == components.len();
            match open_directory_at(&current, component.as_os_str()) {
                Ok(next) => current = next,
                Err(error) if error.kind() == io::ErrorKind::NotFound => {
                    match create_directory_at(&current, component.as_os_str()) {
                        Ok(()) => {}
                        Err(error) if error.kind() == io::ErrorKind::AlreadyExists => {}
                        Err(error) => return Err(error),
                    }
                    current = open_directory_at(&current, component.as_os_str())?;
                    if last {
                        final_existed = false;
                    }
                }
                Err(error) => return Err(error),
            }
        }
        Ok((current, final_existed))
    }

    pub fn create_file_at(parent: &File, name: &str, contents: &[u8]) -> io::Result<File> {
        let file = open_at(
            std::os::fd::AsRawFd::as_raw_fd(parent),
            OsStr::new(name),
            O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW,
            0o600,
        )?;
        write_file(file, contents)
    }

    pub fn open_append_at(parent: &File, name: &str) -> io::Result<File> {
        open_at(
            std::os::fd::AsRawFd::as_raw_fd(parent),
            OsStr::new(name),
            O_WRONLY | O_APPEND | O_NOFOLLOW,
            0,
        )
    }

    pub fn read_file_at(parent: &File, name: &str) -> io::Result<Vec<u8>> {
        let mut file = open_at(
            std::os::fd::AsRawFd::as_raw_fd(parent),
            OsStr::new(name),
            O_RDONLY | O_NOFOLLOW,
            0,
        )?;
        let mut bytes = Vec::new();
        file.read_to_end(&mut bytes)?;
        Ok(bytes)
    }

    fn write_file(mut file: File, contents: &[u8]) -> io::Result<File> {
        file.write_all(contents)?;
        file.sync_all()?;
        Ok(file)
    }

    pub fn remove_file_at(parent: &File, name: &str) -> io::Result<()> {
        let name = c_name(OsStr::new(name))?;
        let result = unsafe { unlinkat(std::os::fd::AsRawFd::as_raw_fd(parent), name.as_ptr(), 0) };
        if result < 0 {
            let error = io::Error::last_os_error();
            if error.kind() == io::ErrorKind::NotFound {
                Ok(())
            } else {
                Err(error)
            }
        } else {
            Ok(())
        }
    }
}

struct RunDirectory {
    path: PathBuf,
    #[cfg(unix)]
    directory: File,
    #[cfg(unix)]
    _active: File,
    owned: BTreeSet<String>,
}

impl RunDirectory {
    fn claim(output: &Path, identity: &str, resume: bool) -> Result<Self, String> {
        validate_output_path(output)?;
        let path = normalized_absolute(output)?;
        let (trusted_root, components) = output_location(output)?;
        #[cfg(unix)]
        let (directory, existed) = safe_fs::open_or_create_directory(&trusted_root, &components)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        #[cfg(not(unix))]
        let existed = false;
        if existed && !resume {
            return Err(format!(
                "{} already exists; use a new dedicated run directory or --resume",
                path.display()
            ));
        }
        let owned = if existed {
            #[cfg(unix)]
            let marker = String::from_utf8(
                safe_fs::read_file_at(&directory, RUN_MARKER)
                    .map_err(|error| format!("{}: {error}", path.display()))?,
            )
            .map_err(|_| format!("{} contains a non-UTF-8 run marker", path.display()))?;
            if marker.trim_end() != identity {
                return Err(format!(
                    "{} belongs to a different input/compiler run; refusing to resume",
                    path.display()
                ));
            }
            #[cfg(unix)]
            let manifest = String::from_utf8(
                safe_fs::read_file_at(&directory, OWNERSHIP_MANIFEST)
                    .map_err(|error| format!("{}: {error}", path.display()))?,
            )
            .map_err(|_| format!("{} contains a non-UTF-8 ownership manifest", path.display()))?;
            parse_ownership_manifest(&manifest, &path)?
        } else {
            #[cfg(unix)]
            {
                safe_fs::create_file_at(&directory, RUN_MARKER, identity.as_bytes())
                    .map_err(|error| format!("{}: {error}", path.display()))?;
                safe_fs::create_file_at(&directory, OWNERSHIP_MANIFEST, &[])
                    .map_err(|error| format!("{}: {error}", path.display()))?;
            }
            BTreeSet::new()
        };
        #[cfg(unix)]
        let active = match safe_fs::create_file_at(&directory, ACTIVE_MARKER, &[]) {
            Ok(file) => file,
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => {
                return Err(format!(
                    "{} is already claimed by another permuter process (or has a stale claim)",
                    path.display()
                ));
            }
            Err(error) => return Err(format!("{}: {error}", path.display())),
        };
        Ok(Self {
            path,
            #[cfg(unix)]
            directory,
            #[cfg(unix)]
            _active: active,
            owned,
        })
    }

    fn path(&self) -> &Path {
        &self.path
    }

    fn register(&mut self, name: &str) -> Result<(), String> {
        validate_owned_name(name)?;
        if self.owned.insert(name.to_string()) {
            self.sync_manifest()?;
        }
        Ok(())
    }

    fn replace_owned_file(&mut self, name: &str, contents: &[u8]) -> Result<(), String> {
        validate_owned_name(name)?;
        if !self.owned.contains(name) {
            return Err(format!("refusing to write unowned output file {name}"));
        }
        #[cfg(unix)]
        {
            safe_fs::remove_file_at(&self.directory, name)
                .map_err(|error| format!("{}: {error}", self.path.display()))?;
            safe_fs::create_file_at(&self.directory, name, contents)
                .map_err(|error| format!("{}: {error}", self.path.display()))?;
        }
        #[cfg(not(unix))]
        fs::write(self.path.join(name), contents)
            .map_err(|error| format!("{}: {error}", self.path.display()))?;
        Ok(())
    }

    fn read_file_optional(&self, name: &str) -> Result<Option<Vec<u8>>, String> {
        #[cfg(unix)]
        {
            return match safe_fs::read_file_at(&self.directory, name) {
                Ok(bytes) => Ok(Some(bytes)),
                Err(error) if error.kind() == io::ErrorKind::NotFound => Ok(None),
                Err(error) => Err(format!("{}: {error}", self.path.display())),
            };
        }
        #[allow(unreachable_code)]
        match fs::read(self.path.join(name)) {
            Ok(bytes) => Ok(Some(bytes)),
            Err(error) if error.kind() == io::ErrorKind::NotFound => Ok(None),
            Err(error) => Err(format!("{}: {error}", self.path.display())),
        }
    }

    fn open_append(&self, name: &str) -> Result<File, String> {
        #[cfg(unix)]
        {
            return safe_fs::open_append_at(&self.directory, name)
                .map_err(|error| format!("{}: {error}", self.path.display()));
        }
        #[allow(unreachable_code)]
        File::options()
            .append(true)
            .open(self.path.join(name))
            .map_err(|error| format!("{}: {error}", self.path.display()))
    }

    fn write_control_file(&self, name: &str, contents: &[u8]) -> Result<(), String> {
        #[cfg(unix)]
        {
            safe_fs::remove_file_at(&self.directory, name)
                .map_err(|error| format!("{}: {error}", self.path.display()))?;
            safe_fs::create_file_at(&self.directory, name, contents)
                .map_err(|error| format!("{}: {error}", self.path.display()))?;
            return Ok(());
        }
        #[allow(unreachable_code)]
        fs::write(self.path.join(name), contents)
            .map_err(|error| format!("{}: {error}", self.path.display()))
    }

    fn remove_previous_results(&mut self) -> Result<(), String> {
        let previous = self.owned.iter().cloned().collect::<Vec<_>>();
        for name in previous {
            if (name.starts_with("candidate-") && name.ends_with(".c")) || name == "report.json" {
                #[cfg(unix)]
                safe_fs::remove_file_at(&self.directory, &name)
                    .map_err(|error| format!("{}: {error}", self.path.display()))?;
                #[cfg(not(unix))]
                fs::remove_file(self.path.join(&name))
                    .map_err(|error| format!("{}: {error}", self.path.display()))?;
                self.owned.remove(&name);
            }
        }
        self.sync_manifest()
    }

    fn sync_manifest(&self) -> Result<(), String> {
        let contents = self
            .owned
            .iter()
            .map(String::as_str)
            .collect::<Vec<_>>()
            .join("\n");
        let manifest = format!(
            "{}{}",
            contents,
            if contents.is_empty() { "" } else { "\n" }
        );
        self.write_control_file(OWNERSHIP_MANIFEST, manifest.as_bytes())
    }
}

impl Drop for RunDirectory {
    fn drop(&mut self) {
        // Production cleanup is relative to the descriptor captured at claim
        // time. A rename or symlink replacement at self.path cannot redirect
        // it to another directory.
        #[cfg(unix)]
        let _ = safe_fs::remove_file_at(&self.directory, ACTIVE_MARKER);
    }
}

struct Journal {
    cached: BTreeMap<String, Measurement>,
    identity: String,
    path: PathBuf,
    writer: Mutex<File>,
    bytes_written: Mutex<usize>,
}

impl Journal {
    fn open(run: &mut RunDirectory, identity: &str, resume: bool) -> Result<Self, String> {
        run.register("journal.tsv")?;
        let path = run.path().join("journal.tsv");
        let header = format!("permuter-journal-v3\t{identity}");
        let mut cached = BTreeMap::new();
        let existing = run.read_file_optional("journal.tsv")?;
        if resume {
            if let Some(bytes) = existing.as_ref() {
                let text = String::from_utf8(bytes.clone())
                    .map_err(|_| format!("{}: journal is not valid UTF-8", path.display()))?;
                let mut lines = text.lines();
                if lines.next() != Some(header.as_str()) {
                    return Err(format!(
                        "{}: journal identity or format does not match this run",
                        path.display()
                    ));
                }
                for (line_number, line) in lines.enumerate() {
                    let fields = line.split('\t').collect::<Vec<_>>();
                    let Some((fingerprint, measurement)) = parse_journal_row(&fields, identity)
                    else {
                        return Err(format!(
                            "{}: invalid, unauthenticated, or incomplete row {}",
                            path.display(),
                            line_number + 2
                        ));
                    };
                    cached.insert(fingerprint, measurement);
                }
            }
        }
        let bytes_written = existing.as_ref().map_or(0, Vec::len);
        if bytes_written > MAX_OUTPUT_BYTES {
            return Err(format!(
                "{} exceeds the {MAX_OUTPUT_BYTES}-byte journal budget",
                path.display()
            ));
        }
        if existing.is_none() {
            let header_line = format!("{header}\n");
            run.write_control_file("journal.tsv", header_line.as_bytes())?;
        }
        let writer = run.open_append("journal.tsv")?;
        let bytes_written = if existing.is_none() {
            header.len() + 1
        } else {
            bytes_written
        };
        Ok(Self {
            cached,
            identity: identity.to_string(),
            path,
            writer: Mutex::new(writer),
            bytes_written: Mutex::new(bytes_written),
        })
    }

    fn cached(&self, candidate: &str) -> Option<Measurement> {
        self.cached.get(candidate).cloned()
    }

    /// Imports rows from a prior run's journal whose header carries the same
    /// cache identity. Rows land in memory only; this run's file records its
    /// own measurements as they happen.
    fn import(&mut self, path: &Path) -> Result<usize, String> {
        let text = fs::read_to_string(path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        let header = format!("permuter-journal-v3\t{}", self.identity);
        let mut lines = text.lines();
        if lines.next() != Some(header.as_str()) {
            return Err(format!(
                "{}: journal identity does not match this backend; not importing",
                path.display()
            ));
        }
        let mut imported = 0usize;
        for line in lines {
            let fields = line.split('\t').collect::<Vec<_>>();
            if let Some((fingerprint, measurement)) = parse_journal_row(&fields, &self.identity) {
                if self.cached.insert(fingerprint, measurement).is_none() {
                    imported += 1;
                }
            }
        }
        Ok(imported)
    }

    fn record(&self, candidate: &str, measurement: &Measurement) -> Result<(), String> {
        let measurement = persisted_measurement(measurement);
        let first = measurement
            .first_difference
            .map_or_else(|| "-".to_string(), |value| value.to_string());
        let authentication = journal_row_auth(&self.identity, candidate, &measurement);
        let line = format!(
            "{candidate}\t{}\t{}\t{}\t{}\t{}\t{}\t{:016x}\t{}\t{}\n",
            u8::from(measurement.exact),
            measurement.score,
            measurement.differences,
            measurement.expected_size,
            measurement.actual_size,
            first,
            measurement.fingerprint,
            measurement.summary,
            authentication,
        );
        let mut bytes_written = self
            .bytes_written
            .lock()
            .map_err(|_| format!("{}: journal size lock is poisoned", self.path.display()))?;
        if bytes_written.saturating_add(line.len()) > MAX_OUTPUT_BYTES {
            return Err(format!(
                "{} would exceed the {MAX_OUTPUT_BYTES}-byte journal budget",
                self.path.display()
            ));
        }
        let mut writer = self
            .writer
            .lock()
            .map_err(|_| format!("{}: journal lock is poisoned", self.path.display()))?;
        writer
            .write_all(line.as_bytes())
            .map_err(|error| format!("{}: {error}", self.path.display()))?;
        writer
            .flush()
            .map_err(|error| format!("{}: {error}", self.path.display()))?;
        *bytes_written += line.len();
        Ok(())
    }
}

fn persisted_summary(summary: &str) -> String {
    let mut output = String::new();
    for character in summary.chars() {
        if output.len() >= MAX_SUMMARY_BYTES {
            break;
        }
        match character {
            '\t' | '\n' | '\r' => output.push(' '),
            value if value.is_control() => output.push(' '),
            value => output.push(value),
        }
    }
    output
}

fn persisted_measurement(measurement: &Measurement) -> Measurement {
    let mut persisted = measurement.clone();
    persisted.summary = persisted_summary(&measurement.summary);
    persisted
}

fn journal_row_auth(identity: &str, candidate: &str, measurement: &Measurement) -> String {
    let mut bytes = Vec::new();
    append_identity_field(&mut bytes, "permuter-journal-row-v1");
    append_identity_field(&mut bytes, identity);
    append_identity_field(&mut bytes, candidate);
    append_identity_field(&mut bytes, if measurement.exact { "1" } else { "0" });
    append_identity_field(&mut bytes, &measurement.score.to_string());
    append_identity_field(&mut bytes, &measurement.differences.to_string());
    append_identity_field(&mut bytes, &measurement.expected_size.to_string());
    append_identity_field(&mut bytes, &measurement.actual_size.to_string());
    append_identity_field(
        &mut bytes,
        &measurement
            .first_difference
            .map_or_else(|| "-".to_string(), |value| value.to_string()),
    );
    append_identity_field(&mut bytes, &format!("{:016x}", measurement.fingerprint));
    append_identity_field(&mut bytes, &measurement.summary);
    alchemy_bundle::sha256::hex(&bytes)
}

fn parse_journal_row(fields: &[&str], identity: &str) -> Option<(String, Measurement)> {
    if fields.len() != 10
        || fields[0].is_empty()
        || fields[8].len() > MAX_SUMMARY_BYTES
        || fields[8].contains(['\r', '\n'])
        || fields[9].is_empty()
    {
        return None;
    }
    let exact = match fields[1] {
        "0" => false,
        "1" => true,
        _ => return None,
    };
    let score: u64 = fields[2].parse().ok()?;
    let differences: usize = fields[3].parse().ok()?;
    let expected_size: usize = fields[4].parse().ok()?;
    let actual_size: usize = fields[5].parse().ok()?;
    let first_difference = if fields[6] == "-" {
        None
    } else {
        Some(fields[6].parse().ok()?)
    };
    let fingerprint = u64::from_str_radix(fields[7], 16).ok()?;
    if score == u64::MAX {
        return None;
    }
    if exact
        && (score != 0
            || differences != 0
            || expected_size != actual_size
            || first_difference.is_some())
    {
        return None;
    }
    if first_difference.is_some_and(|index| index > expected_size.max(actual_size)) {
        return None;
    }
    let measurement = Measurement {
        exact,
        score,
        differences,
        expected_size,
        actual_size,
        first_difference,
        fingerprint,
        heat: Vec::new(),
        summary: fields[8].to_string(),
    };
    (journal_row_auth(identity, fields[0], &measurement) == fields[9])
        .then_some((fields[0].to_string(), measurement))
}

fn source_fingerprint(source: &str) -> String {
    alchemy_bundle::sha256::hex(source.as_bytes())
}

fn candidate_plan(
    permutation: &Permutation,
    iterations: usize,
    seed: u64,
    manual_only: bool,
    weights: &Weights,
    source_bytes: usize,
) -> Result<Vec<Candidate>, String> {
    if iterations == 0 || iterations > MAX_ITERATIONS {
        return Err(format!(
            "candidate plan iterations must be between 1 and {MAX_ITERATIONS}"
        ));
    }
    if source_bytes > MAX_SOURCE_BYTES {
        return Err(format!(
            "source is {} bytes; the maximum permutation source is {MAX_SOURCE_BYTES} bytes",
            source_bytes
        ));
    }
    let estimated_bytes = iterations
        .saturating_mul(source_bytes.max(1))
        .saturating_mul(3);
    if estimated_bytes > MAX_PLAN_BYTES {
        return Err(format!(
            "{} iterations would exceed the {MAX_PLAN_BYTES}-byte source plan budget",
            iterations
        ));
    }
    let base_count = permutation.count().min(iterations).max(1);
    let randomize = permutation.is_random() && !manual_only;
    let per_base = if randomize {
        iterations.div_ceil(base_count)
    } else {
        1
    };
    let mut pools = Vec::with_capacity(base_count);
    let mut estimated_pool_bytes = 0usize;
    for manual_seed in 0..base_count {
        let (source, _) = if randomize {
            permutation.evaluate_marked(manual_seed)?
        } else {
            permutation.evaluate(manual_seed)?
        };
        if source.len() > MAX_SOURCE_BYTES {
            return Err(format!(
                "a materialized base source exceeds the {MAX_SOURCE_BYTES}-byte source budget"
            ));
        }
        estimated_pool_bytes =
            estimated_pool_bytes.saturating_add(per_base.saturating_mul(source.len()));
        if estimated_pool_bytes.saturating_mul(3) > MAX_PLAN_BYTES {
            return Err(format!(
                "materialized mutation pools exceed the {MAX_PLAN_BYTES}-byte plan budget"
            ));
        }
        let variants = if randomize {
            crate::randomize::try_mutate_marked_with_weights(
                &source,
                seed ^ (manual_seed as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15),
                per_base,
                weights,
            )?
        } else {
            vec![crate::randomize::Mutation {
                id: "manual".into(),
                source,
            }]
        };
        if variants
            .iter()
            .any(|variant| variant.source.len() > MAX_SOURCE_BYTES)
        {
            return Err(format!(
                "a generated source exceeds the {MAX_SOURCE_BYTES}-byte source budget"
            ));
        }
        pools.push((manual_seed, variants));
    }

    let mut candidates = Vec::new();
    let mut seen = BTreeSet::new();
    let mut planned_bytes = 0usize;
    let mut layer = 0usize;
    while candidates.len() < iterations {
        let mut added = false;
        for (manual_seed, variants) in &pools {
            let Some(variant) = variants.get(layer) else {
                continue;
            };
            added = true;
            let fingerprint = source_fingerprint(&variant.source);
            if seen.insert(fingerprint.clone()) {
                planned_bytes = planned_bytes.saturating_add(variant.source.len());
                if planned_bytes.saturating_mul(3) > MAX_PLAN_BYTES {
                    return Err(format!(
                        "generated source plan exceeds the {MAX_PLAN_BYTES}-byte budget"
                    ));
                }
                candidates.push(Candidate {
                    index: candidates.len(),
                    manual_seed: *manual_seed,
                    mutation: variant.id.clone(),
                    source: variant.source.clone(),
                    fingerprint,
                });
                if candidates.len() == iterations {
                    break;
                }
            }
        }
        if !added {
            break;
        }
        layer += 1;
    }
    if candidates.is_empty() {
        return Err("permutation plan produced no candidates".into());
    }
    Ok(candidates)
}

fn measurement_order(
    left: &(&Candidate, &Measurement),
    right: &(&Candidate, &Measurement),
) -> Ordering {
    left.1
        .score
        .cmp(&right.1.score)
        .then_with(|| left.1.differences.cmp(&right.1.differences))
        .then_with(|| {
            left.1
                .actual_size
                .abs_diff(left.1.expected_size)
                .cmp(&right.1.actual_size.abs_diff(right.1.expected_size))
        })
        .then_with(|| left.0.index.cmp(&right.0.index))
}

fn json_escape(text: &str) -> String {
    let mut output = String::new();
    for character in text.chars() {
        match character {
            '"' => output.push_str("\\\""),
            '\\' => output.push_str("\\\\"),
            '\n' => output.push_str("\\n"),
            '\r' => output.push_str("\\r"),
            '\t' => output.push_str("\\t"),
            value if value.is_control() => {
                output.push_str(&format!("\\u{:04x}", value as u32));
            }
            value => output.push(value),
        }
    }
    output
}

fn input_label(input: &backend::Input) -> &str {
    let path = if input.directory_mode {
        &input.requested
    } else {
        &input.source_path
    };
    path.file_stem()
        .and_then(|value| value.to_str())
        .unwrap_or("candidate")
}

fn safe_label(input: &backend::Input) -> String {
    let label = input_label(input);
    let mut safe = label
        .chars()
        .map(|character| {
            if character.is_ascii_alphanumeric() || matches!(character, '.' | '-' | '_') {
                character
            } else {
                '_'
            }
        })
        .collect::<String>();
    if safe.is_empty() || safe == "." || safe == ".." {
        safe = "candidate".into();
    }
    safe.truncate(64);
    safe
}

fn append_identity_field(stream: &mut Vec<u8>, value: &str) {
    stream.extend_from_slice(&(value.len() as u64).to_be_bytes());
    stream.extend_from_slice(value.as_bytes());
}

fn input_identity(input: &backend::Input) -> Result<String, String> {
    let requested = fs::canonicalize(&input.requested)
        .map_err(|error| format!("{}: {error}", input.requested.display()))?;
    let source = fs::canonicalize(&input.source_path)
        .map_err(|error| format!("{}: {error}", input.source_path.display()))?;
    let mut bytes = Vec::new();
    append_identity_field(&mut bytes, "permuter-input-v2");
    append_identity_field(&mut bytes, &requested.to_string_lossy());
    append_identity_field(&mut bytes, &source.to_string_lossy());
    append_identity_field(&mut bytes, &source_fingerprint(&input.source));
    Ok(alchemy_bundle::sha256::hex(&bytes))
}

fn run_identity(input: &backend::Input, target: &dyn Backend, seed: u64) -> Result<String, String> {
    let input_identity = input_identity(input)?;
    let mut bytes = Vec::new();
    append_identity_field(&mut bytes, "permuter-run-v2");
    append_identity_field(&mut bytes, &input_identity);
    append_identity_field(&mut bytes, &target.identity());
    append_identity_field(&mut bytes, &seed.to_string());
    Ok(alchemy_bundle::sha256::hex(&bytes))
}

fn default_output(input: &backend::Input, seed: u64, identity: &str) -> PathBuf {
    let stem = safe_label(input);
    root()
        .join("out")
        .join("permuter")
        .join(format!("{stem}-{identity}-seed-{seed}"))
}

fn normalized_absolute(path: &Path) -> Result<PathBuf, String> {
    let absolute = if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    };
    let mut normalized = PathBuf::new();
    for component in absolute.components() {
        match component {
            std::path::Component::CurDir => {}
            std::path::Component::ParentDir => {
                normalized.pop();
            }
            component => normalized.push(component.as_os_str()),
        }
    }
    if normalized.is_absolute() {
        Ok(normalized)
    } else {
        Err(format!("{} is not an absolute path", path.display()))
    }
}

fn canonicalize_with_missing(path: &Path) -> Result<PathBuf, String> {
    let normalized = normalized_absolute(path)?;
    let mut existing = normalized.clone();
    let mut missing = Vec::new();
    while !existing.exists() {
        let Some(name) = existing.file_name() else {
            return Err(format!("could not resolve output path {}", path.display()));
        };
        missing.push(name.to_owned());
        if !existing.pop() {
            return Err(format!("could not resolve output path {}", path.display()));
        }
    }
    let mut canonical =
        fs::canonicalize(&existing).map_err(|error| format!("{}: {error}", existing.display()))?;
    for component in missing.iter().rev() {
        canonical.push(component);
    }
    Ok(canonical)
}

fn output_location(output: &Path) -> Result<(PathBuf, Vec<OsString>), String> {
    let resolved = canonicalize_with_missing(output)?;
    let repository_root = canonicalize_with_missing(root())?;
    let repository_out = canonicalize_with_missing(&root().join("out"))?;
    if resolved.starts_with(&repository_root) && !resolved.starts_with(&repository_out) {
        return Err(format!(
            "refusing output path {}: repository outputs must be under out/",
            output.display()
        ));
    }
    let temporary_roots = [std::env::temp_dir(), PathBuf::from("/tmp")]
        .into_iter()
        .filter_map(|path| fs::canonicalize(path).ok())
        .collect::<Vec<_>>();
    let mut roots = vec![repository_out];
    roots.extend(temporary_roots);
    for trusted_root in roots {
        if resolved == trusted_root {
            return Err(format!(
                "refusing shared output root {}; choose a dedicated child directory",
                output.display()
            ));
        }
        if let Ok(relative) = resolved.strip_prefix(&trusted_root) {
            let components = relative
                .components()
                .map(|component| match component {
                    std::path::Component::Normal(value) => Ok(value.to_owned()),
                    _ => Err(format!(
                        "{} contains an unsafe output component",
                        output.display()
                    )),
                })
                .collect::<Result<Vec<_>, _>>()?;
            if components.is_empty() {
                continue;
            }
            return Ok((trusted_root, components));
        }
    }
    Err(format!(
        "refusing output path {}: it must be under repository out/ or the OS temporary directory",
        output.display()
    ))
}

fn reject_user_symlink_components(path: &Path) -> Result<(), String> {
    let normalized = normalized_absolute(path)?;
    let mut current = PathBuf::new();
    for component in normalized.components() {
        current.push(component.as_os_str());
        match fs::symlink_metadata(&current) {
            Ok(metadata) if metadata.file_type().is_symlink() => {
                if current == Path::new("/tmp") || current == Path::new("/var") {
                    continue;
                }
                return Err(format!(
                    "refusing symlink in output path: {}",
                    current.display()
                ));
            }
            Ok(_) => {}
            Err(error) if error.kind() == io::ErrorKind::NotFound => break,
            Err(error) => return Err(format!("{}: {error}", current.display())),
        }
    }
    Ok(())
}

fn validate_output_path(output: &Path) -> Result<(), String> {
    let _ = normalized_absolute(output)?;
    reject_user_symlink_components(output)?;
    output_location(output).map(|_| ())
}

fn validate_owned_name(name: &str) -> Result<(), String> {
    let valid = (name.starts_with("candidate-") && name.ends_with(".c"))
        || matches!(name, "journal.tsv" | "report.json");
    if !valid || name.contains('/') || name.contains('\\') {
        return Err(format!("invalid owned output name {name:?}"));
    }
    Ok(())
}

fn parse_ownership_manifest(text: &str, path: &Path) -> Result<BTreeSet<String>, String> {
    let mut owned = BTreeSet::new();
    for line in text.lines() {
        validate_owned_name(line).map_err(|error| format!("{}: {error}", path.display()))?;
        owned.insert(line.to_string());
    }
    Ok(owned)
}

fn save_results(
    run: &mut RunDirectory,
    backend_name: &str,
    baseline: &Measurement,
    retained: &[(&Candidate, &Measurement)],
    attempted: usize,
    failures: usize,
) -> Result<(), String> {
    let candidate_bytes = retained
        .iter()
        .map(|(candidate, _)| candidate.source.len())
        .sum::<usize>();
    if candidate_bytes > MAX_OUTPUT_BYTES {
        return Err(format!(
            "retained candidate output exceeds the {MAX_OUTPUT_BYTES}-byte budget"
        ));
    }
    run.remove_previous_results()?;
    for (rank, (candidate, measurement)) in retained.iter().enumerate() {
        let name = format!("candidate-{rank:03}-score-{}.c", measurement.score);
        run.register(&name)?;
        run.replace_owned_file(&name, candidate.source.as_bytes())?;
    }
    let mut report = format!(
        "{{\n  \"backend\": \"{}\",\n  \"baseline_score\": {},\n  \"attempted\": {},\n  \"compile_failures\": {},\n  \"results\": [",
        json_escape(backend_name), baseline.score, attempted, failures
    );
    for (index, (candidate, measurement)) in retained.iter().enumerate() {
        if index != 0 {
            report.push(',');
        }
        report.push_str(&format!(
            "\n    {{\"rank\": {index}, \"candidate\": {}, \"manual_seed\": {}, \"mutation\": \"{}\", \"source_fingerprint\": \"{}\", \"score\": {}, \"exact\": {}, \"differences\": {}, \"actual_size\": {}, \"expected_size\": {}, \"summary\": \"{}\"}}",
            candidate.index,
            candidate.manual_seed,
            json_escape(&candidate.mutation),
            candidate.fingerprint,
            measurement.score,
            measurement.exact,
            measurement.differences,
            measurement.actual_size,
            measurement.expected_size,
            json_escape(&persisted_summary(&measurement.summary)),
        ));
    }
    report.push_str("\n  ]\n}\n");
    if candidate_bytes.saturating_add(report.len()) > MAX_OUTPUT_BYTES {
        return Err(format!(
            "candidate files and report exceed the {MAX_OUTPUT_BYTES}-byte output budget"
        ));
    }
    run.register("report.json")?;
    run.replace_owned_file("report.json", report.as_bytes())?;
    run.sync_manifest()
}

fn output_budget_preflight(candidates: &[Candidate], top: usize) -> Result<(), String> {
    let mut source_sizes = candidates
        .iter()
        .map(|candidate| candidate.source.len())
        .collect::<Vec<_>>();
    source_sizes.sort_unstable_by(|left, right| right.cmp(left));
    let candidate_bytes = source_sizes.into_iter().take(top).sum::<usize>();

    let mut report_entries = candidates
        .iter()
        .map(|candidate| {
            512usize
                .saturating_add(candidate.mutation.len().saturating_mul(6))
                .saturating_add(candidate.fingerprint.len())
                .saturating_add(MAX_SUMMARY_BYTES.saturating_mul(6))
        })
        .collect::<Vec<_>>();
    report_entries.sort_unstable_by(|left, right| right.cmp(left));
    let report_bytes =
        2048usize.saturating_add(report_entries.into_iter().take(top).sum::<usize>());
    if candidate_bytes.saturating_add(report_bytes) > MAX_OUTPUT_BYTES {
        return Err(format!(
            "planned retained candidates and report exceed the {MAX_OUTPUT_BYTES}-byte output budget before compilation"
        ));
    }

    let journal_bytes =
        128usize.saturating_add(candidates.len().saturating_mul(MAX_JOURNAL_ROW_BYTES));
    if journal_bytes > MAX_OUTPUT_BYTES {
        return Err(format!(
            "the {MAX_JOURNAL_ROW_BYTES}-byte journal-row bound for {} candidates exceeds the {MAX_OUTPUT_BYTES}-byte journal budget before compilation",
            candidates.len()
        ));
    }
    Ok(())
}

fn retain_result(
    options: &Options,
    baseline: &Measurement,
    former_best: u64,
    measurement: &Measurement,
) -> bool {
    if measurement.score == u64::MAX {
        return false;
    }
    if options.better_only && measurement.score >= baseline.score {
        return false;
    }
    if options.best_only && measurement.score > former_best {
        return false;
    }
    if options
        .score_threshold
        .is_some_and(|threshold| measurement.score >= threshold)
    {
        return false;
    }
    measurement.exact || measurement.score <= baseline.score
}

fn run_workers(
    backend: Arc<dyn Backend>,
    candidates: Arc<Vec<Candidate>>,
    jobs: usize,
    stop_exact: bool,
    baseline: Measurement,
    journal: Arc<Journal>,
) -> Result<Vec<Evaluated>, String> {
    let next = Arc::new(AtomicUsize::new(0));
    let stop = Arc::new(AtomicBool::new(false));
    let (sender, receiver) = mpsc::channel();
    let worker_count = jobs.min(candidates.len()).max(1);
    let mut workers = Vec::with_capacity(worker_count);
    for _ in 0..worker_count {
        let backend = Arc::clone(&backend);
        let candidates = Arc::clone(&candidates);
        let next = Arc::clone(&next);
        let stop = Arc::clone(&stop);
        let sender = sender.clone();
        let baseline = baseline.clone();
        let journal = Arc::clone(&journal);
        workers.push(std::thread::spawn(move || loop {
            if stop_exact && stop.load(AtomicOrdering::Acquire) {
                break;
            }
            let index = next.fetch_add(1, AtomicOrdering::Relaxed);
            let Some(candidate) = candidates.get(index).cloned() else {
                break;
            };
            let started = Instant::now();
            let cached = journal.cached(&candidate.fingerprint);
            let measurement = if let Some(cached) = cached.as_ref() {
                if cached.exact {
                    // Exact rows terminate a run, so a valid-but-replayed
                    // row must still prove itself against the current
                    // compiler. The row digest catches ordinary edits; this
                    // recheck also defeats an attacker who recomputes it.
                    backend.measure(&candidate.source)
                } else {
                    Ok(cached.clone())
                }
            } else if index == 0 {
                Ok(baseline.clone())
            } else {
                backend.measure(&candidate.source)
            };
            if let Ok(measurement) = &measurement {
                let changed_exact = cached
                    .as_ref()
                    .is_some_and(|previous| previous.exact && previous != measurement);
                if cached.is_none() || changed_exact {
                    if let Err(error) = journal.record(&candidate.fingerprint, measurement) {
                        stop.store(true, AtomicOrdering::Release);
                        let _ = sender.send(Err(error));
                        break;
                    }
                }
            }
            if stop_exact
                && measurement
                    .as_ref()
                    .is_ok_and(|measurement| measurement.exact)
            {
                stop.store(true, AtomicOrdering::Release);
            }
            let _ = sender.send(Ok(Evaluated {
                candidate,
                measurement,
                elapsed: started.elapsed(),
            }));
        }));
    }
    drop(sender);
    let mut evaluated = Vec::new();
    let mut worker_error = None;
    for result in receiver {
        match result {
            Ok(item) => evaluated.push(item),
            Err(error) => {
                worker_error.get_or_insert(error);
            }
        };
    }
    for worker in workers {
        if worker.join().is_err() && worker_error.is_none() {
            worker_error = Some("permuter worker panicked".into());
        }
    }
    match worker_error {
        Some(error) => Err(error),
        None => Ok(evaluated),
    }
}

/// pret-style cumulative mutation walk: each worker keeps a persistent
/// candidate and, with probability keep-prob, stacks another randomization
/// pass on top of it; otherwise it restarts from the base. Compile failures
/// always restart. Score does not gate the walk -- depth is stochastic, which
/// is what makes coordinated multi-pass edits reachable at all.
#[allow(clippy::too_many_arguments)]
fn walk_workers(
    backend: Arc<dyn Backend>,
    base_source: Arc<String>,
    weights: Arc<Weights>,
    total: usize,
    jobs: usize,
    seed: u64,
    keep_prob_permille: u32,
    stop_exact: bool,
    heat_enabled: bool,
    journal: Arc<Journal>,
) -> Result<Vec<Evaluated>, String> {
    let counter = Arc::new(AtomicUsize::new(0));
    let stop = Arc::new(AtomicBool::new(false));
    let (sender, receiver) = mpsc::channel();
    let worker_count = jobs.max(1);
    let mut workers = Vec::with_capacity(worker_count);
    for worker in 0..worker_count {
        let backend = Arc::clone(&backend);
        let base_source = Arc::clone(&base_source);
        let _weights = Arc::clone(&weights);
        let counter = Arc::clone(&counter);
        let stop = Arc::clone(&stop);
        let sender = sender.clone();
        let journal = Arc::clone(&journal);
        workers.push(std::thread::spawn(move || {
            let mut rng = crate::randomize::SplitMix64(
                seed ^ (worker as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15) ^ 0x5851_f42d_4c95_7f2d,
            );
            let mut current: Option<(String, String)> = None; // (source, lineage)
            let mut last_heat: Vec<f32> = Vec::new();
            loop {
                if stop_exact && stop.load(AtomicOrdering::Acquire) {
                    break;
                }
                let index = counter.fetch_add(1, AtomicOrdering::Relaxed);
                if index >= total {
                    break;
                }
                let keep = current.is_some() && rng.index(1000) < keep_prob_permille as usize;
                if !keep {
                    current = Some((base_source.as_ref().clone(), String::from("base")));
                }
                let (cur_source, lineage) = current.clone().expect("walk candidate");
                // The AST engine: parse, apply one weighted pret pass, emit.
                let mutated = crate::astpass::AstRandomizer::new(
                    &cur_source,
                    rng.0 ^ index as u64,
                    None,
                )
                .and_then(|mut r| {
                    if heat_enabled {
                        r.set_heat(last_heat.clone());
                    }
                    r.randomize_named()
                });
                let (mutated_source, pass_name) = match mutated {
                    Ok(pair) => pair,
                    Err(_) => {
                        current = None;
                        continue;
                    }
                };
                let candidate = Candidate {
                    index,
                    manual_seed: worker,
                    mutation: format!("{lineage}>{pass_name}"),
                    source: mutated_source.clone(),
                    fingerprint: source_fingerprint(&mutated_source),
                };
                let started = Instant::now();
                let cached = journal.cached(&candidate.fingerprint);
                let measurement = if let Some(cached) = cached.as_ref() {
                    if cached.exact {
                        backend.measure(&candidate.source)
                    } else {
                        Ok(cached.clone())
                    }
                } else {
                    let measured = backend.measure(&candidate.source);
                    if let Ok(measurement) = &measured {
                        let _ = journal.record(&candidate.fingerprint, measurement);
                    }
                    measured
                };
                match &measurement {
                    Ok(measurement) => {
                        if measurement.exact {
                            stop.store(true, AtomicOrdering::Release);
                        }
                        current = Some((mutated_source.clone(), candidate.mutation.clone()));
                        last_heat = measurement.heat.clone();
                    }
                    Err(_) => {
                        current = None;
                    }
                }
                let elapsed = started.elapsed();
                if sender
                    .send(Ok(Evaluated {
                        candidate,
                        measurement,
                        elapsed,
                    }))
                    .is_err()
                {
                    break;
                }
            }
        }));
    }
    drop(sender);
    let mut evaluated = Vec::new();
    let mut worker_error: Option<String> = None;
    for result in receiver {
        match result {
            Ok(item) => evaluated.push(item),
            Err(error) => {
                let _: String = error;
                worker_error.get_or_insert("walk worker failed".to_string());
            }
        }
    }
    for worker in workers {
        if worker.join().is_err() && worker_error.is_none() {
            worker_error = Some("permuter walk worker panicked".into());
        }
    }
    match worker_error {
        Some(error) => Err(error),
        None => Ok(evaluated),
    }
}

fn run_one(options: &Options, candidate: &Path, multiple: bool) -> Result<(), String> {
    let input = backend::load_input(candidate)?;
    if input.source.len() > MAX_SOURCE_BYTES {
        return Err(format!(
            "{} is {} bytes; the maximum permutation source is {MAX_SOURCE_BYTES} bytes",
            input.source_path.display(),
            input.source.len()
        ));
    }
    let input_key = input_identity(&input)?;
    let output = match &options.output {
        Some(base) if multiple => base.join(format!(
            "{}-{input_key}-seed-{}",
            safe_label(&input),
            options.seed
        )),
        Some(output) => output.clone(),
        None => default_output(&input, options.seed, &input_key),
    };
    validate_output_path(&output)?;
    let permutation = crate::perm::parse(&input.source)?;
    let weights = if let Some(path) = &options.weights {
        if !path.is_file() {
            return Err(format!("--weights {}: no such file", path.display()));
        }
        Weights::from_settings(path, "gcc")?
    } else if input.directory_mode {
        Weights::from_settings(&input.requested.join("settings.toml"), "base")?
    } else {
        Weights::for_profile("gcc")
    };
    let candidates = if options.walk {
        Vec::new()
    } else {
        let planned = candidate_plan(
            &permutation,
            if options.debug { 1 } else { options.iterations },
            options.seed,
            options.manual_only,
            &weights,
            input.source.len(),
        )?;
        output_budget_preflight(&planned, options.top)?;
        planned
    };
    // The baseline is the contributor's untouched source, not whichever
    // randomized mutation happens to occupy candidate slot zero. Using the
    // first mutation here made the displayed threshold seed-dependent and
    // could discard genuine improvements against the actual starting point.
    let target = backend::prepare(&input, &input.source, options.show_errors)?;
    let baseline = target.baseline();
    let identity = run_identity(&input, target.as_ref(), options.seed)?;
    let mut run = RunDirectory::claim(&output, &identity, options.resume)?;
    // Journal rows are keyed by candidate fingerprint, and a row's meaning is
    // fixed by the backend (compiler, reference, comparison implementation)
    // alone -- not by which base source or seed produced the candidate. Keying
    // the journal on the backend identity lets chained rounds against the same
    // owner reuse each other's measurements under --resume.
    let cache_identity = target.identity();
    let mut journal = Journal::open(&mut run, &cache_identity, options.resume)?;
    if let Some(prior) = &options.journal_from {
        let imported = journal.import(prior)?;
        println!("journal: imported {imported} cached measurements from {}", prior.display());
    }
    let journal = Arc::new(journal);
    println!(
        "backend={} bases={} candidates={} jobs={} baseline={} ({})",
        target.name(),
        permutation.count(),
        if options.walk { options.iterations } else { candidates.len() },
        if options.walk { options.jobs } else { options.jobs.min(candidates.len()) },
        baseline.score,
        baseline.summary
    );
    if options.debug {
        return Ok(());
    }

    let backend_name = target.name().to_string();
    let target: Arc<dyn Backend> = Arc::from(target);
    let started = Instant::now();

    // Chained rounds: each round mutates the best source found so far, so
    // coordinated multi-edit improvements become reachable and score ties
    // let the search drift across plateaus the way pret's permuter does.
    let mut chain_source = input.source.clone();
    let mut chain_score = baseline.score;
    let mut best = baseline.score;
    let mut owned: Vec<(Candidate, Measurement)> = Vec::new();
    let mut attempted = 0usize;
    let mut failures = 0usize;
    let mut compile_time = Duration::ZERO;
    let mut exact_found = false;
    if options.walk {
        // The AST engine parses with lang-c, which needs preprocessed input.
        let walk_base = crate::astpass::preprocess_for_ast(&input.source)?;
        let evaluated = walk_workers(
            Arc::clone(&target),
            Arc::new(walk_base),
            Arc::new(weights.clone()),
            options.iterations,
            options.jobs,
            options.seed,
            options.keep_prob_permille,
            options.stop_exact,
            options.heat,
            Arc::clone(&journal),
        )?;
        attempted = evaluated.len();
        for item in evaluated.iter() {
            compile_time += item.elapsed;
            match &item.measurement {
                Ok(measurement) => {
                    let former_best = best;
                    if measurement.score < best {
                        best = measurement.score;
                        println!(
                            "new-best={} candidate={} mutation={} {}",
                            best, item.candidate.index, item.candidate.mutation, measurement.summary
                        );
                    }
                    if measurement.exact {
                        exact_found = true;
                    }
                    if retain_result(options, &baseline, former_best, measurement) {
                        owned.push((item.candidate.clone(), measurement.clone()));
                    }
                }
                Err(error) => {
                    failures += 1;
                    if options.show_errors {
                        eprintln!("candidate {}: {error}", item.candidate.index);
                    }
                }
            }
        }
        let _ = exact_found;
    } else {
    let mut round_candidates = Some(candidates);
    for round in 0..options.chain.max(1) {
        if round > 0 || round_candidates.is_none() {
            let permutation = crate::perm::parse(&chain_source)?;
            round_candidates = None;
            let planned = candidate_plan(
                &permutation,
                options.iterations,
                options.seed ^ (round as u64).wrapping_mul(0x9e37_79b9_7f4a_7c15),
                options.manual_only,
                &weights,
                chain_source.len(),
            )?;
            round_candidates = Some(planned);
        }
        let shared = Arc::new(round_candidates.take().expect("round candidates planned"));
        let mut evaluated = run_workers(
            Arc::clone(&target),
            Arc::clone(&shared),
            options.jobs,
            options.stop_exact,
            baseline.clone(),
            Arc::clone(&journal),
        )?;
        evaluated.sort_by_key(|item| item.candidate.index);
        attempted += evaluated.len();

        let mut round_best: Option<(usize, u64)> = None;
        for item in evaluated.iter() {
            compile_time += item.elapsed;
            match &item.measurement {
                Ok(measurement) => {
                    let former_best = best;
                    if measurement.score < best {
                        best = measurement.score;
                        println!(
                            "new-best={} round={} candidate={} mutation={} {}",
                            best,
                            round,
                            item.candidate.index,
                            item.candidate.mutation,
                            measurement.summary
                        );
                    }
                    if measurement.exact {
                        exact_found = true;
                    }
                    if measurement.score <= chain_score && item.candidate.mutation != "identity" {
                        let take = match round_best {
                            None => true,
                            Some((_, score)) if measurement.score < score => true,
                            // Rotate among equal-score ties so different
                            // rounds and seeds drift along different paths.
                            Some((_, score)) if measurement.score == score => (round % 3) != 0,
                            _ => false,
                        };
                        if take {
                            round_best = Some((item.candidate.index, measurement.score));
                        }
                    }
                    if retain_result(options, &baseline, former_best, measurement) {
                        owned.push((item.candidate.clone(), measurement.clone()));
                    }
                }
                Err(error) => {
                    failures += 1;
                    if options.show_errors {
                        eprintln!("candidate {}: {error}", item.candidate.index);
                    }
                }
            }
        }
        if !options.quiet {
            println!(
                "round={} attempted={} best={} chain={} failures={}",
                round,
                attempted,
                best,
                chain_score,
                failures
            );
        }
        if let Some((index, score)) = round_best {
            if let Some(item) = evaluated.iter().find(|item| item.candidate.index == index) {
                chain_source = item.candidate.source.clone();
                chain_score = score;
            }
        }
        if exact_found && options.stop_exact {
            break;
        }
    }

    }

    let mut retained: Vec<(&Candidate, &Measurement)> = owned
        .iter()
        .map(|(candidate, measurement)| (candidate, measurement))
        .collect();
    retained.sort_by(measurement_order);
    retained.truncate(options.top);
    if let Some((candidate, _)) = retained.first() {
        let best_path = run.path().join("best.c");
        fs::write(&best_path, &candidate.source)
            .map_err(|error| format!("{}: {error}", best_path.display()))?;
    }
    save_results(
        &mut run,
        &backend_name,
        &baseline,
        &retained,
        attempted,
        failures,
    )?;
    let exact = retained.iter().any(|(_, measurement)| measurement.exact);
    println!(
        "done={} attempted={} failures={} best={} exact={} wall_ms={} compiler_ms={} output={}",
        backend_name,
        attempted,
        failures,
        best,
        exact,
        started.elapsed().as_millis(),
        compile_time.as_millis(),
        output.display()
    );
    Ok(())
}

pub fn run(options: Options) -> Result<(), String> {
    let multiple = options.candidates.len() > 1;
    for candidate in &options.candidates {
        run_one(&options, candidate, multiple)?;
    }
    Ok(())
}

pub fn self_test() -> Result<(), String> {
    let permutation =
        crate::perm::parse("int f(void) { s32 a = PERM_GENERAL(1,2); return a + 0; }")?;
    let weights = Weights::for_profile("gcc");
    if candidate_plan(&permutation, MAX_ITERATIONS + 1, 9, false, &weights, 64).is_ok() {
        return Err("candidate plan accepted an unbounded iteration request".into());
    }
    let first = candidate_plan(&permutation, 20, 9, false, &weights, 64)?;
    let second = candidate_plan(&permutation, 20, 9, false, &weights, 64)?;
    if first != second || first.len() != 2 {
        return Err(format!(
            "manual runner plan drifted: {} candidates",
            first.len()
        ));
    }
    let random = crate::perm::parse("int f(void) { s32 a = 1; return a + 0; }")?;
    let mutations = candidate_plan(&random, 20, 9, false, &weights, 64)?;
    if mutations.len() < 2 || mutations[0].mutation != "identity" {
        return Err("random runner plan is degenerate".into());
    }
    if json_escape("a\n\"b") != "a\\n\\\"b" {
        return Err("JSON escaping drifted".into());
    }
    if validate_output_path(&root().join("semantic")).is_ok() {
        return Err("source directory was accepted as an output directory".into());
    }
    if validate_output_path(&root().join("out").join("permuter-self-test")).is_err() {
        return Err("repository ignored out/ was rejected as an output directory".into());
    }
    if validate_output_path(&std::env::temp_dir().join("permuter-custom-output-self-test"))
        .is_err()
    {
        return Err("custom OS temporary output was rejected".into());
    }
    let journal_dir = std::env::temp_dir().join(format!(
        "permuter-journal-self-test-{}",
        std::process::id()
    ));
    let measurement = Measurement {
        exact: false,
        score: 17,
        differences: 3,
        expected_size: 8,
        actual_size: 6,
        first_difference: Some(2),
        fingerprint: 0x1234,
        heat: Vec::new(),
        summary: "journal control".into(),
    };
    {
        let mut run = RunDirectory::claim(&journal_dir, "55aa", false)?;
        let journal = Journal::open(&mut run, "55aa", false)?;
        journal.record("77cc", &measurement)?;
    }
    if RunDirectory::claim(&journal_dir, "changed-identity", true).is_ok() {
        return Err("stale journal data was reused after an identity change".into());
    }
    let restored = {
        let mut run = RunDirectory::claim(&journal_dir, "55aa", true)?;
        Journal::open(&mut run, "55aa", true)?.cached("77cc")
    };
    let journal_path = journal_dir.join("journal.tsv");
    let mut journal_text = fs::read_to_string(&journal_path)
        .map_err(|error| format!("{}: {error}", journal_path.display()))?;
    journal_text.push_str("incomplete row\n");
    fs::write(&journal_path, journal_text)
        .map_err(|error| format!("{}: {error}", journal_path.display()))?;
    {
        let mut run = RunDirectory::claim(&journal_dir, "55aa", true)?;
        if Journal::open(&mut run, "55aa", true).is_ok() {
            return Err("corrupt journal row was accepted during resume".into());
        }
    }
    let _ = fs::remove_dir_all(&journal_dir);
    if restored != Some(measurement) {
        return Err("resume journal failed to restore a matching measurement".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::{
        candidate_plan, output_budget_preflight, save_results, validate_output_path, Journal,
        RunDirectory,
    };
    use crate::backend::Measurement;
    use crate::options::{
        MAX_ITERATIONS, MAX_JOURNAL_ROW_BYTES, MAX_OUTPUT_BYTES, MAX_PLAN_BYTES, MAX_SOURCE_BYTES,
    };
    use alchemy_routing::routing::root;
    use std::fs;
    use std::path::PathBuf;

    fn temporary_path(label: &str) -> PathBuf {
        std::env::temp_dir().join(format!(
            "permuter-runner-test-{label}-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        ))
    }

    #[test]
    fn rejects_cleanup_in_source_directory() {
        let source_directory = root().join("semantic");
        assert!(validate_output_path(&source_directory).is_err());
        assert!(RunDirectory::claim(&source_directory, "test", false).is_err());
    }

    #[test]
    fn accepts_repository_out_and_custom_temporary_paths() {
        assert!(validate_output_path(&root().join("out").join("test-run")).is_ok());
        assert!(
            validate_output_path(&std::env::temp_dir().join("permuter-test-run")).is_ok()
        );
    }

    #[test]
    fn rejects_candidate_plans_above_hard_cap() {
        let permutation = crate::perm::parse("int f(void) { return 1; }").unwrap();
        let weights = crate::randomize::Weights::for_profile("gcc");
        assert!(candidate_plan(&permutation, MAX_ITERATIONS + 1, 1, false, &weights, 64).is_err());
    }

    #[test]
    fn rejects_source_plans_above_byte_budget() {
        let permutation = crate::perm::parse("int f(void) { return 1; }").unwrap();
        let weights = crate::randomize::Weights::for_profile("gcc");
        assert!(candidate_plan(&permutation, 2, 1, false, &weights, MAX_PLAN_BYTES / 2).is_err());
        assert!(candidate_plan(&permutation, 1, 1, false, &weights, MAX_SOURCE_BYTES + 1).is_err());
    }

    #[test]
    fn claims_a_run_directory_exclusively() {
        let path = temporary_path("exclusive");
        let run = RunDirectory::claim(&path, "identity", false).unwrap();
        assert!(RunDirectory::claim(&path, "identity", true).is_err());
        drop(run);
        let resumed = RunDirectory::claim(&path, "identity", true).unwrap();
        drop(resumed);
        fs::remove_dir_all(path).unwrap();
    }

    #[test]
    fn cleanup_does_not_delete_unowned_candidates() {
        let path = temporary_path("ownership");
        let mut run = RunDirectory::claim(&path, "identity", false).unwrap();
        let unrelated = path.join("candidate-unrelated.c");
        fs::write(&unrelated, "keep me").unwrap();
        save_results(
            &mut run,
            "test",
            &Measurement::failed("baseline"),
            &[],
            0,
            0,
        )
        .unwrap();
        assert_eq!(fs::read_to_string(&unrelated).unwrap(), "keep me");
        drop(run);
        fs::remove_dir_all(path).unwrap();
    }

    #[test]
    fn journal_tampering_cannot_create_an_exact_cache_hit() {
        let path = temporary_path("journal-auth");
        let measurement = Measurement {
            exact: false,
            score: 17,
            differences: 3,
            expected_size: 8,
            actual_size: 6,
            first_difference: Some(2),
            fingerprint: 0x1234,
            heat: Vec::new(),
            summary: "journal control".into(),
        };
        {
            let mut run = RunDirectory::claim(&path, "identity", false).unwrap();
            let journal = Journal::open(&mut run, "identity", false).unwrap();
            journal
                .record("candidate-fingerprint", &measurement)
                .unwrap();
        }
        let journal_path = path.join("journal.tsv");
        let text = fs::read_to_string(&journal_path).unwrap();
        let tampered = text.replacen("\t0\t17\t", "\t1\t0\t", 1);
        assert_ne!(text, tampered);
        fs::write(journal_path, tampered).unwrap();
        let mut run = RunDirectory::claim(&path, "identity", true).unwrap();
        assert!(Journal::open(&mut run, "identity", true).is_err());
        drop(run);
        fs::remove_dir_all(path).unwrap();
    }

    #[cfg(unix)]
    #[test]
    fn descriptor_relative_writes_survive_path_rename_and_symlink_swap() {
        use std::os::unix::fs::symlink;

        let path = temporary_path("descriptor-run");
        let moved = temporary_path("descriptor-moved");
        let redirected = temporary_path("descriptor-redirected");
        fs::create_dir(&redirected).unwrap();
        let mut run = RunDirectory::claim(&path, "identity", false).unwrap();
        run.register("report.json").unwrap();
        fs::rename(&path, &moved).unwrap();
        symlink(&redirected, &path).unwrap();
        run.replace_owned_file("report.json", b"descriptor-safe")
            .unwrap();
        assert_eq!(
            fs::read(moved.join("report.json")).unwrap(),
            b"descriptor-safe"
        );
        assert!(!redirected.join("report.json").exists());
        run.remove_previous_results().unwrap();
        assert!(!moved.join("report.json").exists());
        drop(run);
        fs::remove_file(path).unwrap();
        fs::remove_dir_all(moved).unwrap();
        fs::remove_dir_all(redirected).unwrap();
    }

    #[test]
    fn output_budget_is_rejected_before_workers_start() {
        let candidate = super::Candidate {
            index: 0,
            manual_seed: 0,
            mutation: "identity".into(),
            source: "x".repeat(MAX_OUTPUT_BYTES),
            fingerprint: "f".repeat(64),
        };
        assert!(output_budget_preflight(&[candidate], 1).is_err());

        let count = MAX_OUTPUT_BYTES / MAX_JOURNAL_ROW_BYTES + 1;
        let candidates = (0..count)
            .map(|index| super::Candidate {
                index,
                manual_seed: 0,
                mutation: "identity".into(),
                source: "x".into(),
                fingerprint: format!("{index:064x}"),
            })
            .collect::<Vec<_>>();
        assert!(output_budget_preflight(&candidates, 1).is_err());
    }

    #[cfg(unix)]
    #[test]
    fn rejects_symlinked_output_components() {
        use std::os::unix::fs::symlink;

        let parent = temporary_path("symlink-parent");
        let real = temporary_path("symlink-real");
        fs::create_dir(&parent).unwrap();
        fs::create_dir(&real).unwrap();
        let link = parent.join("link");
        symlink(&real, &link).unwrap();
        assert!(validate_output_path(&link.join("run")).is_err());
        fs::remove_dir_all(parent).unwrap();
        fs::remove_dir_all(real).unwrap();
    }
}
