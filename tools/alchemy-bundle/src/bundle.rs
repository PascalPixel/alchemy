//! The compiler-bundle layer: which executables a
//! bundle must contain, which digests are admissible, the once-per-process
//! validation memo, the cache-key signature over the whole toolchain, and the
//! argv builders that every compile in the repository goes through.
//!
//! Formerly the bundle section of the TypeScript compiler module:
//!
//! * `GCC3_BUNDLE`/`_DRIVER`, `GCC3_CFLAGS`
//! * `HostKey`, `hostKey`, `UNSUPPORTED_HOST_MESSAGE`, `hostAdmissionMessage`
//! * `validateBundle`, `validateAgbccBundle`, `validateExperimentalCompiler`
//! * `compilerBundleSignature`
//! * `compilerCommand`, `compilerCommandForTarget`,
//!   `compilerCommandForTargetSource`
//!
//! The path constants it builds on (`ROOT`, `BUNDLE`, `DRIVER`, `GS2_*`,
//! `AGBCC_*`, `CFLAGS`, `GS2_CFLAGS`, `AGBCC_CFLAGS`) and `usesAgbccCompiler`
//! come from `alchemy_routing`; a second copy of them here is the hand-sync
//! problem this effort exists to end.

use std::fs::{self, File, OpenOptions};
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::{Mutex, OnceLock};

use alchemy_routing::routing::{
    agbcc_driver, bundle, bundle_for_target, cflags, driver_for_target, gs2_bundle, root,
    uses_agbcc_compiler, CompilerTarget,
};

use crate::bundle_data::{
    HostDigests, AGBCC_EXPECTED, EXPECTED, GCC3_EXPECTED,
};
use crate::sha256;

/// Every fallible operation here corresponds to a `throw new Error(...)` in the
/// TypeScript; the message text is reproduced verbatim because callers print it.
pub type Result<T> = std::result::Result<T, String>;

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------

pub fn gcc3_bundle() -> PathBuf {
    bundle().join("gcc3")
}
pub fn gcc3_driver() -> PathBuf {
    gcc3_bundle().join("cc1")
}

/// The lock file shared with the compiler repository's staging command. It is
/// intentionally outside `signature_paths()`: the lock is synchronization
/// state, not compiler input.
pub const BUNDLE_LOCK_FILE_NAME: &str = ".alchemy-gcc.lock";

pub fn bundle_lock_path() -> PathBuf {
    bundle().join(BUNDLE_LOCK_FILE_NAME)
}

#[cfg(unix)]
const LOCK_SH: i32 = 1;
#[cfg(all(unix, test))]
const LOCK_EX: i32 = 2;
#[cfg(all(unix, test))]
const LOCK_NB: i32 = 4;

#[cfg(unix)]
unsafe extern "C" {
    fn flock(file_descriptor: i32, operation: i32) -> i32;
}

#[cfg(unix)]
fn flock_file(file: &File, operation: i32) -> std::io::Result<()> {
    use std::os::unix::io::AsRawFd;

    // SAFETY: `file` owns a valid open descriptor for the duration of the
    // call, and `flock` only borrows that descriptor while applying the
    // requested POSIX advisory lock.
    let result = unsafe { flock(file.as_raw_fd(), operation) };
    if result == 0 {
        Ok(())
    } else {
        Err(std::io::Error::last_os_error())
    }
}

#[cfg(unix)]
struct SharedBundleLock {
    // Keeping the descriptor in the process-global OnceLock keeps the shared
    // flock held until process exit.
    _file: File,
}

#[cfg(unix)]
static SHARED_BUNDLE_LOCK: OnceLock<Result<SharedBundleLock>> = OnceLock::new();

/// Acquire the canonical bundle's process-lifetime shared POSIX lock.
///
/// The matching staging command takes an exclusive lock on the same file.
/// This call blocks behind a stage, then retains the descriptor so a later
/// stage cannot mutate files after validation or signature memoization.
pub fn acquire_compiler_bundle_shared_lock() -> Result<()> {
    #[cfg(unix)]
    {
        let result = SHARED_BUNDLE_LOCK.get_or_init(|| {
            let path = bundle_lock_path();
            let file = OpenOptions::new()
                .create(true)
                .read(true)
                .write(true)
                .truncate(false)
                .open(&path)
                .map_err(|error| {
                    format!(
                        "cannot open compiler bundle lock {}: {error}",
                        path.display()
                    )
                })?;
            flock_file(&file, LOCK_SH).map_err(|error| {
                format!(
                    "cannot acquire shared compiler bundle lock {}: {error}",
                    path.display()
                )
            })?;
            Ok(SharedBundleLock { _file: file })
        });
        result.as_ref().map(|_| ()).map_err(Clone::clone)
    }

    #[cfg(not(unix))]
    {
        // Alchemy's admitted hosts are POSIX. Keep unsupported hosts
        // buildable without pretending to provide a non-POSIX flock.
        Ok(())
    }
}

/// Environment variables understood by the standalone compiler repository but
/// deliberately not by Alchemy's consumer-side routing. A non-canonical value
/// here would make `stage --check` describe one tree while Alchemy reads
/// another, so cache/admission entry points reject it instead of silently
/// proceeding with the fixed consumer path.
const ALTERNATE_BUNDLE_ROOT_ENV_VARS: [&str; 2] = [
    "ALCHEMY_GCC_DIST_ROOT",
    "ALCHEMY_GCC296_EXPERIMENTAL_DIST_ROOT",
];

fn canonical_bundle_root() -> PathBuf {
    let path = bundle();
    fs::canonicalize(&path).unwrap_or(path)
}

fn resolved_environment_path(path: &Path) -> Result<PathBuf> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        std::env::current_dir()
            .map_err(|error| format!("cannot resolve compiler bundle root: {error}"))?
            .join(path)
    };
    Ok(fs::canonicalize(&path).unwrap_or(path))
}

fn root_override_error(variable: &str, requested: &Path, canonical: &Path) -> Option<String> {
    if requested == canonical {
        None
    } else {
        Some(format!(
            "Alchemy compiler bundle root is fixed at {}; unset {variable} or set it to that exact path (got {})",
            canonical.display(),
            requested.display(),
        ))
    }
}

/// Variables that once switched code generation from the ambient environment.
///
/// `thumb_order_grouped_dma_store` gated three sub-behaviours on `getenv`, so
/// exporting one of these silently changed emitted Thumb ordering with no
/// diagnostic, no digest change, and no cache key covering it. alchemy-gcc
/// a3b1837 removed the gates, but this ledger is additive and still admits
/// every `cc1` approved before that, so a stale staged binary plus a leftover
/// export reproduces the defect on a bundle that validates perfectly clean.
const REFUSED_CODEGEN_ENV_VARS: [&str; 3] = [
    "ALCHEMY_NO_FOUR_WORD",
    "ALCHEMY_NO_LOOP0",
    "ALCHEMY_NO_LOOP1",
];

/// Refuse a build whose environment can still reach a gated compiler.
///
/// This closes the stale-binary path without invalidating a single approved
/// digest. Pruning the pre-fix digests would also work, but it would force a
/// compiler rebuild on every other checkout to remove a hazard that only bites
/// when one of these is exported; refusing the variable is the cheaper half of
/// the same guarantee. A current binary ignores these entirely, so the only
/// build this rejects is one that was never reproducible anyway.
fn codegen_override_error(variable: &str, set: bool) -> Option<String> {
    if set {
        Some(format!(
            "{variable} is set. It switched compiler behaviour in bundles built before \
             alchemy-gcc a3b1837, which the approved-digest ledger still admits, so this \
             build would not be reproducible and its result would be cached under a key \
             that does not mention it. Unset it."
        ))
    } else {
        None
    }
}

fn ensure_no_codegen_environment_overrides() -> Result<()> {
    for variable in REFUSED_CODEGEN_ENV_VARS {
        // An empty value is treated as unset, matching how the staging-root
        // check above reads its own variables.
        let set = std::env::var_os(variable).is_some_and(|value| !value.is_empty());
        if let Some(error) = codegen_override_error(variable, set) {
            return Err(error);
        }
    }
    Ok(())
}

/// Reject alternate staging roots before any compiler admission or cache-key
/// work. The consumer-side routing intentionally has one canonical bundle
/// path; an environment override belongs to `alchemy-gcc`, not to Alchemy.
pub fn ensure_canonical_bundle_root() -> Result<()> {
    let canonical = canonical_bundle_root();
    for variable in ALTERNATE_BUNDLE_ROOT_ENV_VARS {
        let Some(value) = std::env::var_os(variable) else {
            continue;
        };
        if value.is_empty() {
            continue;
        }
        let requested = resolved_environment_path(Path::new(&value))?;
        if let Some(error) = root_override_error(variable, &requested, &canonical) {
            return Err(error);
        }
    }
    Ok(())
}

fn ensure_compiler_bundle_access() -> Result<()> {
    ensure_no_codegen_environment_overrides()?;
    ensure_canonical_bundle_root()?;
    acquire_compiler_bundle_shared_lock()
}

/// `GCC3_CFLAGS`. Stock gcc-3.0 has no `-fcall-used-r4` patch (that is
/// gcc-2.96's Camelot ABI fork); it reserves the same register class the README
/// documents as needed for gs2's Camelot backend mode: `-ffixed-r7`.
pub fn gcc3_cflags() -> Vec<String> {
    vec![
        "-O2".into(),
        "-mthumb".into(),
        "-mthumb-interwork".into(),
        "-mcpu=arm7tdmi".into(),
        "-fno-builtin".into(),
        "-nostdinc".into(),
        "-ffreestanding".into(),
        "-ffixed-r7".into(),
        format!("-I{}", root().join("include").display()),
    ]
}

// ---------------------------------------------------------------------------
// Host admission
// ---------------------------------------------------------------------------

/// `HostKey`. Every darwin/linux x arm64/x86_64 combination is a first-class
/// host. A host whose digest set is still empty is not "unsupported": it is
/// admissible the moment someone builds the committed fork source on it,
/// reproduces the ROM byte-identically under `make verify`, and pins the
/// resulting digests.
pub const HOST_KEYS: [&str; 4] = ["darwin-arm64", "darwin-x64", "linux-x64", "linux-arm64"];

/// `hostKey()`.
///
/// PORT NOTE: the TypeScript reads `process.platform`/`process.arch` at call
/// time; Rust resolves the target triple at compile time. Both name the running
/// binary's platform, and neither can change during a process, so the observable
/// behaviour is the same. `null` becomes `None`.
pub fn host_key() -> Option<&'static str> {
    match (std::env::consts::OS, std::env::consts::ARCH) {
        ("macos", "aarch64") => Some("darwin-arm64"),
        ("macos", "x86_64") => Some("darwin-x64"),
        ("linux", "x86_64") => Some("linux-x64"),
        ("linux", "aarch64") => Some("linux-arm64"),
        _ => None,
    }
}

pub const UNSUPPORTED_HOST_MESSAGE: &str =
    "alchemy-gcc supports darwin/linux on arm64/x86_64; this platform is none of those";

/// `hostAdmissionMessage`. The message a not-yet-admitted host gets. It names
/// the admission procedure instead of implying the platform is out of scope.
pub fn host_admission_message(host: &str, what: &str) -> String {
    [
        format!("alchemy-gcc has no approved {what} digests for host {host} yet."),
        "Admit this host: build and stage the committed compiler source".to_string(),
        "(`make` in alchemy-gcc), run the full `make verify`, and pin".to_string(),
        "the digests from that green verify -- the same admission every listed".to_string(),
        "digest already passed.".to_string(),
    ]
    .join(" ")
}

// ---------------------------------------------------------------------------
// Validation memo
// ---------------------------------------------------------------------------
//
// PORT NOTE: `const validated = new Set<CompilerTarget>()`, `let agbccValidated
// = false` and `const experimentalValidated = new Set<string>()` are
// module-level mutable state in the TypeScript, i.e. per-process. The
// memoisation IS observable: once a target validates, a later call returns
// without re-stat-ing, re-hashing or re-smoking, so replacing the binary
// mid-process is not noticed. That is preserved deliberately, not improved --
// the whole point of the memo is that a long compile sweep pays the digest cost
// once.
//
// `Vec` rather than `HashSet` throughout this crate: nothing here may introduce
// an unordered container, because the sibling flag pipeline is later-flag-wins
// and a `HashSet` that leaks into it destroys correctness silently. There are
// at most two targets and three experimental compilers, so linear search is
// also simply faster.

fn validated() -> &'static Mutex<Vec<&'static str>> {
    static VALIDATED: OnceLock<Mutex<Vec<&'static str>>> = OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(Vec::new()))
}

fn agbcc_validated() -> &'static Mutex<bool> {
    static VALIDATED: OnceLock<Mutex<bool>> = OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(false))
}

fn experimental_validated() -> &'static Mutex<Vec<String>> {
    static VALIDATED: OnceLock<Mutex<Vec<String>>> = OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(Vec::new()))
}

// ---------------------------------------------------------------------------
// Filesystem and process helpers
// ---------------------------------------------------------------------------

/// `statSync(path).mode & 0o111`, split into "could not stat" and "not
/// executable" exactly as the TypeScript's try/catch-then-test does.
///
/// PORT NOTE: `statSync` follows symlinks (unlike `lstatSync`); `fs::metadata`
/// does too. `lstat` here would change which bundles validate.
fn executable_mode(path: &Path) -> Option<bool> {
    #[cfg(unix)]
    {
        use std::os::unix::fs::PermissionsExt;
        let metadata = fs::metadata(path).ok()?;
        Some(metadata.permissions().mode() & 0o111 != 0)
    }
    #[cfg(not(unix))]
    {
        // host_key() already returned None on any non-unix platform, so this
        // arm is unreachable in practice; it exists so the crate still compiles.
        let _ = path;
        None
    }
}

/// `outputText(value)` == `Buffer.from(value).toString("utf8")`.
///
/// PORT NOTE: Node's `toString("utf8")` replaces invalid sequences with U+FFFD,
/// which is exactly `String::from_utf8_lossy`. It is NOT `from_utf8().unwrap()`;
/// a compiler that dies mid-write can emit a truncated multi-byte sequence and
/// the TypeScript prints it rather than throwing.
fn output_text(value: &[u8]) -> String {
    String::from_utf8_lossy(value).into_owned()
}

/// `Bun.spawnSync(argv, { cwd: ROOT, stdout: "pipe", stderr: "pipe" })`, reduced
/// to what the three smoke tests read: success, or the trimmed detail text.
fn smoke(argv: &[String]) -> std::result::Result<(), String> {
    let output = Command::new(&argv[0])
        .args(&argv[1..])
        .current_dir(root())
        .output();
    let output = match output {
        Ok(output) => output,
        // PORT NOTE: when the executable cannot be spawned at all, Bun's
        // `spawnSync` yields a non-zero `exitCode` with empty pipes, so the
        // TypeScript reports an empty detail. Rust surfaces the OS error
        // instead. Both are a failure with a message; the message text differs
        // only in this unreachable-after-stat case.
        Err(error) => return Err(error.to_string()),
    };
    if output.status.success() {
        return Ok(());
    }
    let stderr = output_text(&output.stderr);
    let detail = if stderr.is_empty() {
        output_text(&output.stdout)
    } else {
        stderr
    };
    Err(detail.trim().to_string())
}

fn lookup<'a>(table: &'a [HostDigests], host: &str) -> Option<&'a [&'static str]> {
    table
        .iter()
        .find(|(key, _)| *key == host)
        .map(|(_, digests)| *digests)
}

// ---------------------------------------------------------------------------
// validateBundle / validateAgbccBundle / validateExperimentalCompiler
// ---------------------------------------------------------------------------

/// `validateBundle(target = "gs1")`.
pub fn validate_bundle(target: CompilerTarget) -> Result<()> {
    ensure_compiler_bundle_access()?;
    if validated()
        .lock()
        .expect("validation memo is not poisoned")
        .contains(&target.as_str())
    {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let bundle_dir = bundle_for_target(target);
    // PORT NOTE: `Object.entries(EXPECTED[host][target])` throws a TypeError if
    // either key is absent. Every `HostKey` and every `CompilerTarget` is
    // present in the literal, so the lookup cannot miss; the admission message
    // here is a defensive arm, not a behaviour difference.
    let entries = EXPECTED
        .iter()
        .find(|(key, _)| *key == host)
        .and_then(|(_, targets)| targets.iter().find(|(key, _)| *key == target.as_str()))
        .map(|(_, entries)| *entries)
        .ok_or_else(|| host_admission_message(host, target.as_str()))?;
    if entries.iter().all(|(_, expected)| expected.is_empty()) {
        return Err(host_admission_message(host, target.as_str()));
    }
    for (name, expected) in entries {
        let path = bundle_dir.join(name);
        let missing = format!(
            "alchemy-gcc {} bundle is missing executable {name}",
            target.as_str()
        );
        if executable_mode(&path) != Some(true) {
            return Err(missing);
        }
        let bytes = fs::read(&path).map_err(|_| missing)?;
        let actual = sha256::hex(&bytes);
        if !expected.contains(&actual.as_str()) {
            return Err(format!(
                "alchemy-gcc {}/{name} has an unapproved digest",
                target.as_str()
            ));
        }
    }
    // 起動確認。並列処理の前に移設済み補助実行体を一つずつ起動する。
    // 初回起動を同時に行うとmacOSの検証処理が競合する。
    smoke(&[
        driver_for_target(target).to_string_lossy().into_owned(),
        format!("-B{}/", bundle_dir.display()),
        "-S".into(),
        "-x".into(),
        "c".into(),
        "-o".into(),
        "/dev/null".into(),
        "/dev/null".into(),
    ])
    .map_err(|detail| {
        format!(
            "alchemy-gcc {} smoke compile failed: {detail}",
            target.as_str()
        )
    })?;
    validated()
        .lock()
        .expect("validation memo is not poisoned")
        .push(target.as_str());
    Ok(())
}

/// `validateAgbccBundle()`.
pub fn validate_agbcc_bundle() -> Result<()> {
    ensure_compiler_bundle_access()?;
    if *agbcc_validated().lock().expect("memo is not poisoned") {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let driver = agbcc_driver();
    let missing = "alchemy-gcc agbcc bundle is missing executable old_agbcc".to_string();
    if executable_mode(&driver) != Some(true) {
        return Err(missing);
    }
    let bytes = fs::read(&driver).map_err(|_| missing)?;
    let actual = sha256::hex(&bytes);
    // PORT NOTE: order matters and is preserved -- the TypeScript hashes the
    // driver BEFORE consulting `AGBCC_EXPECTED[host]`, so an unreadable driver
    // reports "missing executable" even on a host with no approved digests.
    let expected = lookup(AGBCC_EXPECTED, host).unwrap_or(&[]);
    if expected.is_empty() {
        return Err(host_admission_message(host, "agbcc/old_agbcc"));
    }
    if !expected.contains(&actual.as_str()) {
        return Err("alchemy-gcc agbcc/old_agbcc has an unapproved digest".to_string());
    }
    smoke(&[
        driver.to_string_lossy().into_owned(),
        "/dev/null".into(),
        "-mthumb-interwork".into(),
        "-O2".into(),
        "-o".into(),
        "/dev/null".into(),
    ])
    .map_err(|detail| format!("alchemy-gcc agbcc smoke compile failed: {detail}"))?;
    *agbcc_validated().lock().expect("memo is not poisoned") = true;
    Ok(())
}

/// `validateExperimentalCompiler(name, driver, expected)`.
pub fn validate_experimental_compiler(
    name: &str,
    driver: &Path,
    expected: &[HostDigests],
) -> Result<()> {
    ensure_compiler_bundle_access()?;
    if experimental_validated()
        .lock()
        .expect("memo is not poisoned")
        .iter()
        .any(|seen| seen == name)
    {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    // ABSENT is not a failure, but PRESENT-AND-UNAPPROVED still is.
    //
    // This compiler is a comparison probe. Nothing routes to it, it is not on
    // the ROM build path, and `GCC3_EXPECTED` is empty for every host -- so
    // demanding it made `make verify` unpassable everywhere, and unpassable in
    // a way no host could fix: admitting a digest requires a green verify, and
    // verify required the digest. Skipping when it is not staged keeps the
    // gate's actual job (reject a binary we did not approve) and drops the
    // circular half.
    if executable_mode(driver) != Some(true) {
        return Ok(());
    }
    let bytes = fs::read(driver)
        .map_err(|_| format!("alchemy-gcc experimental {name} is missing executable cc1"))?;
    let actual = sha256::hex(&bytes);
    let approved = lookup(expected, host).unwrap_or(&[]);
    if approved.is_empty() {
        return Err(host_admission_message(
            host,
            &format!("experimental {name}/cc1"),
        ));
    }
    if !approved.contains(&actual.as_str()) {
        return Err(format!(
            "alchemy-gcc experimental {name}/cc1 has an unapproved digest"
        ));
    }
    smoke(&[
        driver.to_string_lossy().into_owned(),
        "/dev/null".into(),
        "-quiet".into(),
        "-O2".into(),
        "-o".into(),
        "/dev/null".into(),
    ])
    .map_err(|detail| format!("alchemy-gcc experimental {name} smoke compile failed: {detail}"))?;
    experimental_validated()
        .lock()
        .expect("memo is not poisoned")
        .push(name.to_string());
    Ok(())
}

/// The experimental compilers, as `(name, driver, expected)` triples, in the
/// order in the approved bundle table.
pub fn experimental_compilers() -> Vec<(&'static str, PathBuf, &'static [HostDigests])> {
    vec![("gcc3", gcc3_driver(), GCC3_EXPECTED)]
}

// ---------------------------------------------------------------------------
// compilerBundleSignature
// ---------------------------------------------------------------------------

/// The ten paths `compilerBundleSignature` digests, in order.
///
/// `GCC3_DRIVER` is included because gcc3 has its own staged driver, flags, and
/// approved digests; leaving it out would let a cache entry produced by one
/// gcc3 build validate against another.
///
/// `PRET_EARLY_THUMB_DRIVER` and `GCC2951_DRIVER` used to be here too. They
/// named binaries no source in any repo could rebuild, so the compiler cache
/// was partly keyed on two orphans. Removing them invalidates the cache once.
/// [`SIGNATURE_PATH_EXPRESSIONS`] mirrors this order as the source expression
/// text used by the parity checks.
pub fn signature_paths() -> Vec<PathBuf> {
    let bundle_dir = bundle();
    let gs2 = gs2_bundle();
    vec![
        bundle_dir.join("xgcc"),
        bundle_dir.join("cpp"),
        bundle_dir.join("tradcpp"),
        bundle_dir.join("cc1"),
        gs2.join("xgcc"),
        gs2.join("cpp0"),
        gs2.join("tradcpp0"),
        gs2.join("cc1"),
        agbcc_driver(),
        gcc3_driver(),
    ]
}

/// The same ten entries as [`signature_paths`], written as the TypeScript
/// writes them, in the same order.
pub const SIGNATURE_PATH_EXPRESSIONS: [&str; 10] = [
    "join(BUNDLE, \"xgcc\")",
    "join(BUNDLE, \"cpp\")",
    "join(BUNDLE, \"tradcpp\")",
    "join(BUNDLE, \"cc1\")",
    "join(GS2_BUNDLE, \"xgcc\")",
    "join(GS2_BUNDLE, \"cpp0\")",
    "join(GS2_BUNDLE, \"tradcpp0\")",
    "join(GS2_BUNDLE, \"cc1\")",
    "AGBCC_DRIVER",
    "GCC3_DRIVER",
];

fn append_compiler_input_tree(stream: &mut Vec<u8>, directory: &Path, base: &Path) {
    let relative = directory.strip_prefix(base).unwrap_or(directory);
    let entries = match fs::read_dir(directory) {
        Ok(entries) => {
            let mut entries = entries.filter_map(std::result::Result::ok).collect::<Vec<_>>();
            entries.sort_by_key(|entry| entry.file_name());
            entries
        }
        Err(_) => {
            append_signature_frame(stream, relative.to_string_lossy().as_bytes());
            append_signature_frame(stream, b"unreadable-directory");
            return;
        }
    };
    for entry in entries {
        let path = entry.path();
        let relative = path.strip_prefix(base).unwrap_or(&path);
        match entry.file_type() {
            Ok(kind) if kind.is_dir() => append_compiler_input_tree(stream, &path, base),
            Ok(kind) if kind.is_file() => {
                append_signature_frame(stream, relative.to_string_lossy().as_bytes());
                match fs::read(&path) {
                    Ok(bytes) => append_signature_frame(stream, &bytes),
                    Err(_) => append_signature_frame(stream, b"unreadable-file"),
                }
            }
            Ok(_) => {
                append_signature_frame(stream, relative.to_string_lossy().as_bytes());
                append_signature_frame(stream, b"unsupported-entry");
            }
            Err(_) => {
                append_signature_frame(stream, relative.to_string_lossy().as_bytes());
                append_signature_frame(stream, b"unreadable-entry");
            }
        }
    }
}

fn file_type_tag(file_type: &fs::FileType) -> &'static [u8] {
    if file_type.is_file() {
        b"file"
    } else if file_type.is_dir() {
        b"directory"
    } else if file_type.is_symlink() {
        b"symlink"
    } else {
        b"other"
    }
}

#[cfg(unix)]
fn permission_bits(metadata: &fs::Metadata) -> u32 {
    use std::os::unix::fs::PermissionsExt;

    metadata.permissions().mode() & 0o7777
}

#[cfg(not(unix))]
fn permission_bits(metadata: &fs::Metadata) -> u32 {
    u32::from(metadata.permissions().readonly())
}

fn append_metadata_state(stream: &mut Vec<u8>, path: &Path, follow_links: bool) {
    append_signature_frame(stream, if follow_links { b"stat" } else { b"lstat" });
    let metadata = if follow_links {
        fs::metadata(path)
    } else {
        fs::symlink_metadata(path)
    };
    match metadata {
        Ok(metadata) => {
            append_signature_frame(stream, b"present");
            append_signature_frame(stream, file_type_tag(&metadata.file_type()));
            append_signature_frame(stream, &permission_bits(&metadata).to_be_bytes());
        }
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {
            append_signature_frame(stream, b"missing");
        }
        Err(_) => {
            append_signature_frame(stream, b"unreadable");
        }
    }
}

fn append_bundle_path_signature(stream: &mut Vec<u8>, path: &Path) {
    append_signature_frame(stream, &path_bytes(path));
    // `validate_bundle` follows links, while a link-versus-regular-file change
    // is still a stable filesystem input. Record both views so chmod on the
    // target and file-type changes invalidate the same cache namespace.
    append_metadata_state(stream, path, false);
    append_metadata_state(stream, path, true);
    match fs::read(path) {
        Ok(bytes) => append_signature_frame(stream, &bytes),
        Err(_) => append_signature_frame(stream, b"missing"),
    }
}

fn compiler_bundle_signature_for_paths(paths: &[PathBuf], include: &Path) -> String {
    let mut stream: Vec<u8> = Vec::new();
    append_signature_frame(&mut stream, b"alchemy compiler bundle v2");
    for path in paths {
        append_bundle_path_signature(&mut stream, path);
    }
    // Every compiler plan adds the repository's `include/` directory. Header
    // contents therefore belong to the compiler cache identity too.
    append_signature_frame(&mut stream, b"alchemy compiler include tree v1");
    append_compiler_input_tree(&mut stream, include, include);
    sha256::hex(&stream)
}

/// Computes `compilerBundleSignature()` directly from the staged files.
///
/// This intentionally bypasses the process-wide memo and is useful for tests
/// and diagnostics. Production callers should use [`compiler_bundle_signature`]
/// so the staged bundle is hashed only once.
pub fn compiler_bundle_signature_uncached() -> String {
    ensure_compiler_bundle_access()
        .unwrap_or_else(|error| panic!("compiler bundle access rejected: {error}"));
    compiler_bundle_signature_for_paths(&signature_paths(), &root().join("include"))
}

/// `compilerBundleSignature()`.
///
/// Cache-key material for compilation. A single differing byte in an admitted
/// compiler executable or tracked project header must move the signature, or a
/// stale cache entry could be accepted.
///
/// PORT NOTE: `Bun.CryptoHasher("sha256")` is fed incrementally; this builds the
/// same byte stream and hashes it once. SHA-256 is a streaming construction,
/// so concatenation is bit-identical, not merely equivalent. `Path` bytes are
/// framed directly, and the literal `"missing"` is the seven ASCII bytes used
/// for an absent file. The length frames keep a missing file, an empty file,
/// and metadata changes distinguishable.
///
/// The staged compiler and tracked include tree are treated as immutable for
/// the lifetime of a process. The shared process-lifetime flock is the
/// synchronization boundary for the cooperating stage command; direct
/// mutation that bypasses that lock remains unsupported, and the memo does not
/// observe it.
pub fn compiler_bundle_signature() -> String {
    ensure_compiler_bundle_access()
        .unwrap_or_else(|error| panic!("compiler bundle access rejected: {error}"));
    static SIGNATURE: OnceLock<String> = OnceLock::new();
    SIGNATURE
        .get_or_init(compiler_bundle_signature_uncached)
        .clone()
}

/// Fallible entry point for cache/admission callers. The historical
/// infallible function remains available to lower-level callers, while paths
/// that can accept a cache hit must reject an alternate staging root first.
pub fn compiler_bundle_signature_checked() -> Result<String> {
    ensure_compiler_bundle_access()?;
    Ok(compiler_bundle_signature())
}

// ---------------------------------------------------------------------------
// PATH executable signatures
// ---------------------------------------------------------------------------

type HostExecutableSignatureCache = Vec<(Vec<String>, Result<String>)>;

fn host_executable_signature_cache() -> &'static Mutex<HostExecutableSignatureCache> {
    static CACHE: OnceLock<Mutex<HostExecutableSignatureCache>> = OnceLock::new();
    CACHE.get_or_init(|| Mutex::new(Vec::new()))
}

fn path_bytes(path: &Path) -> Vec<u8> {
    #[cfg(unix)]
    {
        use std::os::unix::ffi::OsStrExt;
        path.as_os_str().as_bytes().to_vec()
    }
    #[cfg(not(unix))]
    {
        path.to_string_lossy().as_bytes().to_vec()
    }
}

fn append_signature_frame(stream: &mut Vec<u8>, bytes: &[u8]) {
    stream.extend_from_slice(&(bytes.len() as u64).to_be_bytes());
    stream.extend_from_slice(bytes);
}

fn resolve_host_executable(name: &str) -> Result<PathBuf> {
    if name.is_empty() {
        return Err("host executable name is empty".to_string());
    }
    let path = std::env::var_os("PATH")
        .ok_or_else(|| "PATH is unset; cannot resolve host executable".to_string())?;
    for directory in std::env::split_paths(&path) {
        let candidate = directory.join(name);
        if executable_mode(&candidate) == Some(true) {
            return fs::canonicalize(&candidate).map_err(|error| {
                format!(
                    "host executable {name} resolved at {} but its path cannot be read: {error}",
                    candidate.display()
                )
            });
        }
    }
    Err(format!("host executable {name} cannot be resolved on PATH"))
}

fn host_executable_signature_uncached_names(names: &[String]) -> Result<String> {
    let mut stream = Vec::new();
    append_signature_frame(&mut stream, b"alchemy-bundle host-executables v1");
    append_signature_frame(&mut stream, &(names.len() as u64).to_be_bytes());
    for name in names {
        let path = resolve_host_executable(name)?;
        let bytes = fs::read(&path).map_err(|error| {
            format!(
                "host executable {name} resolved at {} but cannot be read: {error}",
                path.display()
            )
        })?;
        append_signature_frame(&mut stream, name.as_bytes());
        append_signature_frame(&mut stream, &path_bytes(&path));
        append_signature_frame(&mut stream, &bytes);
    }
    Ok(sha256::hex(&stream))
}

/// Computes a SHA-256 signature for an ordered list of host executables.
///
/// Each name is resolved through `PATH`, and the signature frames the request
/// order, requested name, resolved path, and complete file bytes with lengths.
/// A missing or unreadable executable returns an error. Successful and failed
/// results are memoized per ordered name list while the process runs; callers
/// must treat `PATH`, its resolutions, and the resolved files as immutable after
/// the first request for a given list.
pub fn host_executable_signature(executables: &[&str]) -> Result<String> {
    let names: Vec<String> = executables.iter().map(|name| (*name).to_string()).collect();
    let mut cache = host_executable_signature_cache()
        .lock()
        .expect("host executable signature memo is not poisoned");
    if let Some((_, result)) = cache.iter().find(|(cached, _)| *cached == names) {
        return result.clone();
    }
    let result = host_executable_signature_uncached_names(&names);
    cache.push((names, result.clone()));
    result
}

/// Computes [`host_executable_signature`] without consulting its memo.
///
/// This is intended for tests and diagnostics; production cache keys should
/// use the memoized function and its immutable-input invariant.
pub fn host_executable_signature_uncached(executables: &[&str]) -> Result<String> {
    let names: Vec<String> = executables.iter().map(|name| (*name).to_string()).collect();
    host_executable_signature_uncached_names(&names)
}

// ---------------------------------------------------------------------------
// compilerCommand*
// ---------------------------------------------------------------------------

/// `compilerCommand(...arguments)`.
pub fn compiler_command(arguments: &[String]) -> Result<Vec<String>> {
    compiler_command_for_target(CompilerTarget::Gs1, arguments)
}

/// `compilerCommandForTarget(target, ...arguments)`.
///
/// PORT NOTE: the TypeScript accepts `Array<string | number>` and applies
/// `String` to each element. Numbers would need ECMAScript `ToString(Number)`,
/// which disagrees with Rust's `Display` for floats (`1` vs `1.0`, and
/// exponential below 1e-6). Rather than carry that hazard, this takes
/// already-rendered `String`s and pushes the formatting decision onto the
/// caller, where the value's type is known. Every call site in the tree passes
/// integers or string literals.
pub fn compiler_command_for_target(
    target: CompilerTarget,
    arguments: &[String],
) -> Result<Vec<String>> {
    validate_bundle(target)?;
    let bundle_dir = bundle_for_target(target);
    let mut argv = vec![
        driver_for_target(target).to_string_lossy().into_owned(),
        format!("-B{}/", bundle_dir.display()),
    ];
    argv.extend(arguments.iter().cloned());
    Ok(argv)
}

/// `compilerCommandForTargetSource(target, source, ...arguments)`.
///
/// PORT NOTE: the agbcc branch drops `-S` and does NOT prepend `-B`; old_agbcc
/// is a bare cc1-style compiler with no driver search path. The TypeScript
/// filters with `!==` BEFORE `String`, so a numeric argument could never be
/// mistaken for the `-S` flag; taking `&[String]` preserves that because the
/// comparison is against the same rendered text either way.
pub fn compiler_command_for_target_source(
    target: CompilerTarget,
    source: &str,
    arguments: &[String],
) -> Result<Vec<String>> {
    if !uses_agbcc_compiler(target, source) {
        return compiler_command_for_target(target, arguments);
    }
    validate_agbcc_bundle()?;
    let mut argv = vec![agbcc_driver().to_string_lossy().into_owned()];
    argv.extend(arguments.iter().filter(|a| *a != "-S").cloned());
    Ok(argv)
}

/// `cflagsForTarget` is re-exported from `alchemy_routing`; this is the gcc3
/// counterpart that has no routing-crate home because gcc3 is not a
/// `CompilerTarget`.
pub fn cflags_for_family_gcc3() -> Vec<String> {
    gcc3_cflags()
}

/// `CFLAGS`, re-exported so a caller needing the whole bundle layer does not
/// also have to name `alchemy_routing`.
pub fn base_cflags() -> Vec<String> {
    cflags()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn host_key_is_one_of_the_four_or_none() {
        if let Some(host) = host_key() {
            assert!(HOST_KEYS.contains(&host), "unknown host key {host}");
        }
    }

    #[test]
    fn every_digest_table_lists_every_host_exactly_once() {
        // The TypeScript indexes these with `EXPECTED[host]`, which is a total
        // function over `HostKey`. A mirror that dropped a host would turn a
        // "no approved digests, here is the admission procedure" message into a
        // lookup miss.
        //
        // Completeness is asserted here, not order: `EXPECTED` lists
        // `linux-arm64` before `linux-x64` and the other four tables list them
        // the other way round. That asymmetry exists in the TypeScript and is
        // mirrored deliberately (see the PORT NOTE in `bundle_data.rs`); the
        // order that does matter -- executables within a target, and digests
        // within a list -- is gated element-wise by `crate::drift`.
        let mut hosts: Vec<&str> = EXPECTED.iter().map(|(host, _)| *host).collect();
        hosts.sort_unstable();
        let mut keys = HOST_KEYS.to_vec();
        keys.sort_unstable();
        assert_eq!(hosts, keys);
        for table in [AGBCC_EXPECTED, GCC3_EXPECTED] {
            let hosts: Vec<&str> = table.iter().map(|(host, _)| *host).collect();
            assert_eq!(hosts, HOST_KEYS.to_vec());
        }
    }

    #[test]
    fn every_expected_digest_is_lowercase_sha256_hex() {
        let mut count = 0;
        for (_, targets) in EXPECTED {
            for (_, executables) in *targets {
                for (_, digests) in *executables {
                    for digest in *digests {
                        assert_eq!(digest.len(), 64, "not a sha256 digest: {digest}");
                        assert!(
                            digest
                                .bytes()
                                .all(|b| b.is_ascii_digit() || (b'a'..=b'f').contains(&b)),
                            "not lowercase hex: {digest}"
                        );
                        count += 1;
                    }
                }
            }
        }
        assert!(count > 0, "the digest table is empty");
    }

    /// The amnesty gate. Admitting a new digest must REPLACE the old one, not
    /// join it: a list of length two approves two different compilers, and the
    /// whole point of a digest ledger is that exactly one binary is the
    /// approved one. This is what kept 121 stale `cc1' digests alive.
    #[test]
    fn no_executable_approves_more_than_one_digest_per_host() {
        for (host, targets) in EXPECTED {
            for (target, executables) in *targets {
                for (name, digests) in *executables {
                    assert!(
                        digests.len() <= 1,
                        "{host}/{target}/{name} approves {} digests; admitting a new \
                         one must replace the old, not join it",
                        digests.len()
                    );
                }
            }
        }
        for (label, table) in [("agbcc", AGBCC_EXPECTED), ("gcc3", GCC3_EXPECTED)] {
            for (host, digests) in table {
                assert!(
                    digests.len() <= 1,
                    "{label}/{host} approves {} digests",
                    digests.len()
                );
            }
        }
    }

    #[test]
    fn signature_includes_routable_gcc3_driver_in_order() {
        let paths = signature_paths();
        assert_eq!(paths.len(), 10);
        assert_eq!(paths[9], gcc3_driver());
        assert_eq!(SIGNATURE_PATH_EXPRESSIONS.len(), 10);
        assert_eq!(SIGNATURE_PATH_EXPRESSIONS[9], "GCC3_DRIVER");
    }

    #[test]
    fn compiler_input_tree_signature_moves_with_header_content() {
        let directory = std::env::temp_dir().join(format!(
            "alchemy-bundle-headers-{}",
            std::process::id()
        ));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(directory.join("nested")).unwrap();
        let header = directory.join("nested/test.h");
        fs::write(&header, b"aa").unwrap();
        let mut before = Vec::new();
        append_compiler_input_tree(&mut before, &directory, &directory);
        fs::write(&header, b"ab").unwrap();
        let mut after = Vec::new();
        append_compiler_input_tree(&mut after, &directory, &directory);
        assert_ne!(sha256::hex(&before), sha256::hex(&after));
        fs::remove_dir_all(directory).unwrap();
    }

    #[test]
    fn cached_signature_matches_uncached_signature() {
        let cached = compiler_bundle_signature();
        assert_eq!(cached, compiler_bundle_signature_uncached());
        assert_eq!(cached.len(), 64);
    }

    #[cfg(unix)]
    #[test]
    fn bundle_signature_changes_for_mode_and_file_type() {
        use std::os::unix::fs::{symlink, PermissionsExt};

        let directory = std::env::temp_dir().join(format!(
            "alchemy-bundle-file-metadata-{}",
            std::process::id()
        ));
        let _ = fs::remove_dir_all(&directory);
        let include = directory.join("include");
        fs::create_dir_all(&include).expect("create metadata include directory");
        let executable = directory.join("cc1");
        fs::write(&executable, b"compiler bytes").expect("write metadata compiler");

        let mut permissions = fs::metadata(&executable)
            .expect("stat metadata compiler")
            .permissions();
        permissions.set_mode(0o644);
        fs::set_permissions(&executable, permissions)
            .expect("make metadata compiler non-executable");
        let non_executable =
            compiler_bundle_signature_for_paths(std::slice::from_ref(&executable), &include);

        let mut permissions = fs::metadata(&executable)
            .expect("restat metadata compiler")
            .permissions();
        permissions.set_mode(0o755);
        fs::set_permissions(&executable, permissions).expect("make metadata compiler executable");
        let executable_signature =
            compiler_bundle_signature_for_paths(std::slice::from_ref(&executable), &include);
        assert_ne!(non_executable, executable_signature);

        let link = directory.join("cc1-link");
        symlink(&executable, &link).expect("create metadata compiler symlink");
        let symlink_signature =
            compiler_bundle_signature_for_paths(std::slice::from_ref(&link), &include);
        assert_ne!(executable_signature, symlink_signature);

        fs::remove_dir_all(directory).expect("remove metadata test directory");
    }

    #[test]
    fn alternate_root_override_is_rejected_by_the_canonical_guard() {
        let canonical = Path::new("/repo/alchemy-gcc/dist");
        let requested = Path::new("/tmp/alchemy-gcc/dist");
        let error = root_override_error("ALCHEMY_GCC_DIST_ROOT", requested, canonical)
            .expect("alternate root must be rejected");
        assert!(error.contains("ALCHEMY_GCC_DIST_ROOT"));
        assert!(error.contains(canonical.to_string_lossy().as_ref()));
        assert!(root_override_error("ALCHEMY_GCC_DIST_ROOT", canonical, canonical).is_none());
    }

    #[cfg(unix)]
    #[test]
    fn canonical_root_guard_rejects_an_alternate_environment_in_a_child() {
        for variable in [
            "ALCHEMY_GCC_DIST_ROOT",
            "ALCHEMY_GCC296_EXPERIMENTAL_DIST_ROOT",
        ] {
            let status = Command::new(std::env::current_exe().expect("locate test binary"))
                .args([
                    "--exact",
                    "bundle::tests::canonical_root_guard_probe",
                    "--nocapture",
                ])
                .env(variable, "/tmp/alchemy-gcc-alternate/dist")
                .status()
                .expect("run canonical-root probe child");
            assert!(
                status.success(),
                "canonical-root probe child failed for {variable}: {status}"
            );
        }
    }

    #[cfg(unix)]
    #[test]
    fn canonical_root_guard_probe() {
        let alternate = "/tmp/alchemy-gcc-alternate/dist";
        if [
            "ALCHEMY_GCC_DIST_ROOT",
            "ALCHEMY_GCC296_EXPERIMENTAL_DIST_ROOT",
        ]
        .iter()
        .any(|variable| std::env::var(variable).as_deref() == Ok(alternate))
        {
            assert!(ensure_canonical_bundle_root().is_err());
        }
    }

    #[cfg(unix)]
    #[test]
    fn shared_lock_blocks_an_exclusive_child_without_wedging_the_test_process() {
        let directory =
            std::env::temp_dir().join(format!("alchemy-bundle-flock-{}", std::process::id()));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(&directory).expect("create flock test directory");
        let path = directory.join("lock");
        let file = OpenOptions::new()
            .create(true)
            .read(true)
            .write(true)
            .truncate(false)
            .open(&path)
            .expect("open flock test file");
        flock_file(&file, LOCK_SH).expect("take parent shared flock");

        let status = Command::new(std::env::current_exe().expect("locate test binary"))
            .args([
                "--exact",
                "bundle::tests::exclusive_lock_probe",
                "--nocapture",
            ])
            .env("ALCHEMY_BUNDLE_FLOCK_PROBE", &path)
            .status()
            .expect("run flock probe child");
        assert!(status.success(), "flock probe child failed: {status}");

        fs::remove_dir_all(directory).expect("remove flock test directory");
    }

    #[cfg(unix)]
    #[test]
    fn exclusive_lock_probe() {
        let Some(path) = std::env::var_os("ALCHEMY_BUNDLE_FLOCK_PROBE") else {
            return;
        };
        let file = OpenOptions::new()
            .read(true)
            .write(true)
            .open(path)
            .expect("open flock probe file");
        let error = flock_file(&file, LOCK_EX | LOCK_NB)
            .expect_err("exclusive non-blocking flock must see the parent's shared lock");
        assert_eq!(error.kind(), std::io::ErrorKind::WouldBlock);
    }

    #[test]
    fn signature_cache_is_thread_safe() {
        let expected = compiler_bundle_signature();
        let threads: Vec<_> = (0..8)
            .map(|_| std::thread::spawn(compiler_bundle_signature))
            .collect();
        for thread in threads {
            assert_eq!(
                thread.join().expect("signature thread did not panic"),
                expected
            );
        }
    }

    #[cfg(unix)]
    #[test]
    fn path_executable_signature_is_order_and_content_sensitive() {
        use std::os::unix::fs::PermissionsExt;

        static PATH_TEST_LOCK: OnceLock<Mutex<()>> = OnceLock::new();
        let _lock = PATH_TEST_LOCK
            .get_or_init(|| Mutex::new(()))
            .lock()
            .expect("PATH test lock is not poisoned");
        static TEST_ID: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(0);
        let id = TEST_ID.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
        let directory = std::env::temp_dir().join(format!(
            "alchemy-bundle-host-signature-{}-{id}",
            std::process::id()
        ));
        fs::create_dir_all(&directory).expect("create host signature test directory");
        let as_name = format!("alchemy-test-as-{id}");
        let ld_name = format!("alchemy-test-ld-{id}");
        let as_path = directory.join(&as_name);
        let ld_path = directory.join(&ld_name);
        fs::write(&as_path, b"as-bytes").expect("write as test executable");
        fs::write(&ld_path, b"ld-bytes").expect("write ld test executable");
        for path in [&as_path, &ld_path] {
            let mut permissions = fs::metadata(path)
                .expect("stat test executable")
                .permissions();
            permissions.set_mode(0o755);
            fs::set_permissions(path, permissions).expect("make test executable executable");
        }

        let previous_path = std::env::var_os("PATH");
        std::env::set_var("PATH", &directory);
        let names = [as_name.as_str(), ld_name.as_str()];
        let ordered = host_executable_signature(&names).expect("sign ordered executables");
        let reversed = host_executable_signature_uncached(&[names[1], names[0]])
            .expect("sign reversed executables");
        assert_ne!(ordered, reversed);

        fs::write(&as_path, b"changed-as-bytes").expect("change test executable");
        let changed =
            host_executable_signature_uncached(&names).expect("sign changed executable contents");
        assert_ne!(ordered, changed);
        assert!(host_executable_signature_uncached(&["missing-host-executable"]).is_err());

        match previous_path {
            Some(path) => std::env::set_var("PATH", path),
            None => std::env::remove_var("PATH"),
        }
        fs::remove_dir_all(directory).expect("remove host signature test directory");
    }

    #[test]
    fn admission_message_names_the_procedure() {
        let message = host_admission_message("darwin-x64", "gs1");
        assert!(message.starts_with(
            "alchemy-gcc has no approved gs1 digests for host darwin-x64 yet. Admit this host:"
        ));
        assert!(message.ends_with("digest already passed."));
    }

    #[test]
    fn output_text_is_lossy_not_panicking() {
        assert_eq!(output_text(&[0xff, b'a']), "\u{fffd}a");
    }

    #[test]
    fn a_set_codegen_variable_is_refused_and_names_itself() {
        let error = codegen_override_error("ALCHEMY_NO_LOOP1", true)
            .expect("a set codegen override must be refused");
        assert!(error.starts_with("ALCHEMY_NO_LOOP1 is set."));
        assert!(error.ends_with("Unset it."));
        assert_eq!(codegen_override_error("ALCHEMY_NO_LOOP1", false), None);
    }

    /// The list is the guard. A gate added to the compiler without its variable
    /// added here would be switchable from the environment again.
    #[test]
    fn every_refused_codegen_variable_is_distinct_and_alchemy_scoped() {
        for variable in REFUSED_CODEGEN_ENV_VARS {
            assert!(variable.starts_with("ALCHEMY_"), "{variable}");
        }
        let mut seen: Vec<&str> = REFUSED_CODEGEN_ENV_VARS.to_vec();
        seen.sort_unstable();
        seen.dedup();
        assert_eq!(seen.len(), REFUSED_CODEGEN_ENV_VARS.len());
    }
}
