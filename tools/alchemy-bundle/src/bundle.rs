//! The compiler-bundle layer: which executables a
//! bundle must contain, which digests are admissible, the once-per-process
//! validation memo, the cache-key signature over the whole toolchain, and the
//! argv builders that every compile in the repository goes through.
//!
//! Formerly the bundle section of the TypeScript compiler module:
//!
//! * `PRET_EARLY_THUMB_BUNDLE`/`_DRIVER`, `GCC2951_BUNDLE`/`_DRIVER`,
//!   `GCC3_BUNDLE`/`_DRIVER`, `GCC3_CFLAGS`
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

use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::{Mutex, OnceLock};

use alchemy_routing::routing::{
    agbcc_driver, bundle, bundle_for_target, cflags, driver_for_target, gs2_bundle, root,
    uses_agbcc_compiler, CompilerTarget,
};

use crate::bundle_data::{
    HostDigests, AGBCC_EXPECTED, EXPECTED, GCC2951_EXPECTED, GCC3_EXPECTED,
    PRET_EARLY_THUMB_EXPECTED,
};
use crate::sha256;

/// Every fallible operation here corresponds to a `throw new Error(...)` in the
/// TypeScript; the message text is reproduced verbatim because callers print it.
pub type Result<T> = std::result::Result<T, String>;

// ---------------------------------------------------------------------------
// Paths
// ---------------------------------------------------------------------------

pub fn pret_early_thumb_bundle() -> PathBuf {
    bundle().join("pret-early-thumb")
}
pub fn pret_early_thumb_driver() -> PathBuf {
    pret_early_thumb_bundle().join("cc1")
}
pub fn gcc2951_bundle() -> PathBuf {
    bundle().join("gcc2951")
}
pub fn gcc2951_driver() -> PathBuf {
    gcc2951_bundle().join("cc1")
}
pub fn gcc3_bundle() -> PathBuf {
    bundle().join("gcc3")
}
pub fn gcc3_driver() -> PathBuf {
    gcc3_bundle().join("cc1")
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
        "Admit this host: build the committed fork source (alchemy-gcc/build.sh),".to_string(),
        "stage it (alchemy-gcc/stage.sh), run the full `make verify`, and pin".to_string(),
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
    if experimental_validated()
        .lock()
        .expect("memo is not poisoned")
        .iter()
        .any(|seen| seen == name)
    {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let missing = format!("alchemy-gcc experimental {name} is missing executable cc1");
    if executable_mode(driver) != Some(true) {
        return Err(missing);
    }
    let bytes = fs::read(driver).map_err(|_| missing)?;
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

/// The three experimental compilers, as `(name, driver, expected)` triples, in
/// the order in the approved bundle table.
pub fn experimental_compilers() -> Vec<(&'static str, PathBuf, &'static [HostDigests])> {
    vec![
        (
            "pret-early-thumb",
            pret_early_thumb_driver(),
            PRET_EARLY_THUMB_EXPECTED,
        ),
        ("gcc2951", gcc2951_driver(), GCC2951_EXPECTED),
        ("gcc3", gcc3_driver(), GCC3_EXPECTED),
    ]
}

// ---------------------------------------------------------------------------
// compilerBundleSignature
// ---------------------------------------------------------------------------

/// The eleven paths `compilerBundleSignature` digests, in order.
///
/// PORT NOTE -- REAL BUG, REPRODUCED AS WRITTEN. `GCC3_DRIVER` is absent from
/// this list even though gcc3 is a routable `CompilerFamily` with its own
/// `GCC3_CFLAGS` and its own `GCC3_EXPECTED` digests. Swapping the staged gcc3
/// `cc1` therefore does NOT change the signature, so a cache entry produced by
/// one gcc3 build validates against another. That is a defect in
/// the former compiler module, not in this port, and it is left alone deliberately:
/// `compilerBundleSignature` is cache-key material in six call sites, and
/// changing its input set invalidates every cache in the repository at once.
/// Fixing it is a separate, deliberate decision that needs its own cache
/// migration. Do not "correct" this list without that plan.
///
/// (`PRET_EARLY_THUMB_DRIVER` and `GCC2951_DRIVER`, the other two experimental
/// compilers, ARE included -- so the omission is specific to gcc3 rather than a
/// consistent policy of excluding experimental compilers.)
///
/// [`SIGNATURE_PATH_EXPRESSIONS`] mirrors the same list as the TypeScript
/// expression text so `crate::drift` can gate the omission rather than trusting
/// this comment.
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
        pret_early_thumb_driver(),
        gcc2951_driver(),
    ]
}

/// The same eleven entries as [`signature_paths`], written as the TypeScript
/// writes them. Compared against the parsed `const paths = [...]` inside
/// `compilerBundleSignature` by `crate::drift`, which is what makes the
/// gcc3 omission a gated fact instead of a comment: adding `GCC3_DRIVER` on
/// either side fails the gate until both sides agree.
pub const SIGNATURE_PATH_EXPRESSIONS: [&str; 11] = [
    "join(BUNDLE, \"xgcc\")",
    "join(BUNDLE, \"cpp\")",
    "join(BUNDLE, \"tradcpp\")",
    "join(BUNDLE, \"cc1\")",
    "join(GS2_BUNDLE, \"xgcc\")",
    "join(GS2_BUNDLE, \"cpp0\")",
    "join(GS2_BUNDLE, \"tradcpp0\")",
    "join(GS2_BUNDLE, \"cc1\")",
    "AGBCC_DRIVER",
    "PRET_EARLY_THUMB_DRIVER",
    "GCC2951_DRIVER",
];

/// `compilerBundleSignature()`.
///
/// Cache-key material in six TypeScript call sites. A single differing byte
/// silently invalidates a cache entry or, worse, wrongly validates one, so this
/// function's parity with the TypeScript is the load-bearing measurement of the
/// whole crate.
///
/// PORT NOTE: `Bun.CryptoHasher("sha256")` is fed incrementally; this builds the
/// same byte stream and hashes it once. SHA-256 is a streaming construction, so
/// concatenation is bit-identical, not merely equivalent. `digest.update(path)`
/// on a JS string is UTF-8 encoded, which is what `Path`'s bytes already are.
/// The literal `"missing"` is the seven ASCII bytes, and it stands in for the
/// file contents only -- the path and both NULs are still written, so a missing
/// file and an empty file are distinguishable.
pub fn compiler_bundle_signature() -> String {
    let mut stream: Vec<u8> = Vec::new();
    for path in signature_paths() {
        stream.extend_from_slice(path.to_string_lossy().as_bytes());
        stream.push(0);
        match fs::read(&path) {
            Ok(bytes) => stream.extend_from_slice(&bytes),
            Err(_) => stream.extend_from_slice(b"missing"),
        }
        stream.push(0);
    }
    sha256::hex(&stream)
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
        for table in [
            AGBCC_EXPECTED,
            PRET_EARLY_THUMB_EXPECTED,
            GCC2951_EXPECTED,
            GCC3_EXPECTED,
        ] {
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
        assert!(count > 100, "only {count} digests, table looks truncated");
    }

    #[test]
    fn signature_omits_gcc3_driver_as_the_typescript_does() {
        // PORT NOTE guard: this pins the KNOWN BUG. If someone adds gcc3 to
        // `signature_paths`, this test fails and forces them to read the note
        // about busting every cache in the repository first.
        let paths = signature_paths();
        assert_eq!(paths.len(), 11);
        assert!(
            !paths.contains(&gcc3_driver()),
            "gcc3 cc1 must stay out of the signature until a cache migration lands"
        );
        assert!(paths.contains(&gcc2951_driver()));
        assert!(paths.contains(&pret_early_thumb_driver()));
    }

    #[test]
    fn signature_is_stable_and_is_a_sha256_hex_string() {
        let first = compiler_bundle_signature();
        let second = compiler_bundle_signature();
        assert_eq!(first, second);
        assert_eq!(first.len(), 64);
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
}
