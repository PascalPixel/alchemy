use crate::bundle_data::{HostDigests, AGBCC_EXPECTED, EXPECTED};
use crate::routing::{agbcc_driver, bundle, driver, root, CompilerTarget};
use crate::sha256;
use fs2::FileExt;
use std::fs::{self, File, OpenOptions};
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::{Mutex, OnceLock};
pub type Result<T> = std::result::Result<T, String>;
pub fn gcc3_driver() -> PathBuf {
    bundle().join("gcc3/cc1")
}
struct SharedBundleLock {
    _file: File,
}
static SHARED_BUNDLE_LOCK: OnceLock<Result<SharedBundleLock>> = OnceLock::new();
pub fn acquire_compiler_bundle_shared_lock() -> Result<()> {
    let result = SHARED_BUNDLE_LOCK.get_or_init(|| {
        let path = bundle().join(".alchemy-gcc.lock");
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
        FileExt::lock_shared(&file).map_err(|error| {
            format!(
                "cannot acquire shared compiler bundle lock {}: {error}",
                path.display()
            )
        })?;
        Ok(SharedBundleLock { _file: file })
    });
    result.as_ref().map(|_| ()).map_err(Clone::clone)
}
const ALTERNATE_BUNDLE_ROOT_ENV_VARS: [&str; 2] = [
    "ALCHEMY_GCC_DIST_ROOT",
    "ALCHEMY_GCC296_EXPERIMENTAL_DIST_ROOT",
];
fn canonical_bundle_root() -> PathBuf {
    fs::canonicalize(bundle()).unwrap_or_else(|_| bundle())
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
        Some(format!("Alchemy compiler bundle root is fixed at {}; unset {variable} or set it to that exact path (got {})", canonical.display(), requested.display(),))
    }
}
const REFUSED_CODEGEN_ENV_VARS: [&str; 3] = [
    "ALCHEMY_NO_FOUR_WORD",
    "ALCHEMY_NO_LOOP0",
    "ALCHEMY_NO_LOOP1",
];
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
        let set = std::env::var_os(variable).is_some_and(|value| !value.is_empty());
        if let Some(error) = codegen_override_error(variable, set) {
            return Err(error);
        }
    }
    Ok(())
}
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
        format!("-I{}", root().join("games/gs1/include").display()),
    ]
}
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
fn validation_cache() -> &'static Mutex<Vec<String>> {
    static VALIDATED: OnceLock<Mutex<Vec<String>>> = OnceLock::new();
    VALIDATED.get_or_init(|| Mutex::new(Vec::new()))
}

fn validation_cached(key: &str) -> bool {
    validation_cache()
        .lock()
        .expect("validation memo is not poisoned")
        .iter()
        .any(|seen| seen == key)
}

fn cache_validation(key: &str) {
    validation_cache()
        .lock()
        .expect("validation memo is not poisoned")
        .push(key.to_string());
}
fn executable_mode(path: &Path) -> Option<bool> {
    #[cfg(unix)]
    {
        use std::os::unix::fs::PermissionsExt;
        let metadata = fs::metadata(path).ok()?;
        Some(metadata.permissions().mode() & 0o111 != 0)
    }
    #[cfg(not(unix))]
    {
        let _ = path;
        None
    }
}
fn smoke(argv: &[String]) -> std::result::Result<(), String> {
    let output = Command::new(&argv[0])
        .args(&argv[1..])
        .current_dir(root())
        .output()
        .map_err(|error| error.to_string())?;
    if output.status.success() {
        return Ok(());
    }
    let detail = if output.stderr.is_empty() {
        String::from_utf8_lossy(&output.stdout)
    } else {
        String::from_utf8_lossy(&output.stderr)
    };
    Err(detail.trim().to_string())
}
fn lookup<'a>(table: &'a [HostDigests], host: &str) -> Option<&'a [&'static str]> {
    table
        .iter()
        .find(|(key, _)| *key == host)
        .map(|(_, digests)| *digests)
}
pub fn validate_bundle(target: CompilerTarget) -> Result<()> {
    ensure_compiler_bundle_access()?;
    if validation_cached(target.as_str()) {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
    let bundle_dir = bundle();
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
    smoke(&[
        driver().to_string_lossy().into_owned(),
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
    cache_validation(target.as_str());
    Ok(())
}
pub fn validate_agbcc_bundle() -> Result<()> {
    ensure_compiler_bundle_access()?;
    if validation_cached("agbcc") {
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
    cache_validation("agbcc");
    Ok(())
}
pub fn validate_experimental_compiler(
    name: &str,
    driver: &Path,
    expected: &[HostDigests],
) -> Result<()> {
    ensure_compiler_bundle_access()?;
    let cache_key = format!("experimental:{name}");
    if validation_cached(&cache_key) {
        return Ok(());
    }
    let host = host_key().ok_or_else(|| UNSUPPORTED_HOST_MESSAGE.to_string())?;
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
    cache_validation(&cache_key);
    Ok(())
}
pub fn signature_paths() -> Vec<PathBuf> {
    let bundle_dir = bundle();
    vec![
        bundle_dir.join("xgcc"),
        bundle_dir.join("cpp"),
        bundle_dir.join("tradcpp"),
        bundle_dir.join("cc1"),
        agbcc_driver(),
        gcc3_driver(),
    ]
}
fn append_compiler_input_tree(stream: &mut Vec<u8>, directory: &Path, base: &Path) {
    let relative = directory.strip_prefix(base).unwrap_or(directory);
    let entries = match fs::read_dir(directory) {
        Ok(entries) => {
            let mut entries = entries
                .filter_map(std::result::Result::ok)
                .collect::<Vec<_>>();
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
    append_metadata_state(stream, path, false);
    append_metadata_state(stream, path, true);
    match fs::read(path) {
        Ok(bytes) => append_signature_frame(stream, &bytes),
        Err(_) => append_signature_frame(stream, b"missing"),
    }
}
fn compiler_bundle_signature_for_paths(paths: &[PathBuf], includes: &[PathBuf]) -> String {
    let mut stream: Vec<u8> = Vec::new();
    append_signature_frame(&mut stream, b"alchemy compiler bundle v2");
    for path in paths {
        append_bundle_path_signature(&mut stream, path);
    }
    append_signature_frame(&mut stream, b"alchemy compiler include trees v2");
    for include in includes {
        append_signature_frame(&mut stream, &path_bytes(include));
        append_compiler_input_tree(&mut stream, include, include);
    }
    sha256::hex(&stream)
}
pub fn compiler_bundle_signature_uncached() -> String {
    ensure_compiler_bundle_access()
        .unwrap_or_else(|error| panic!("compiler bundle access rejected: {error}"));
    compiler_bundle_signature_for_paths(
        &signature_paths(),
        &[
            root().join("games/gs1/include"),
            root().join("games/gs2/include"),
        ],
    )
}
pub fn compiler_bundle_signature() -> String {
    ensure_compiler_bundle_access()
        .unwrap_or_else(|error| panic!("compiler bundle access rejected: {error}"));
    static SIGNATURE: OnceLock<String> = OnceLock::new();
    SIGNATURE
        .get_or_init(compiler_bundle_signature_uncached)
        .clone()
}
pub fn compiler_bundle_signature_checked() -> Result<String> {
    ensure_compiler_bundle_access()?;
    Ok(compiler_bundle_signature())
}
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
    append_signature_frame(&mut stream, b"compiler-core host-executables v1");
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
pub fn compiler_command_for_target(
    target: CompilerTarget,
    arguments: &[String],
) -> Result<Vec<String>> {
    validate_bundle(target)?;
    let bundle_dir = bundle();
    let mut argv = vec![
        driver().to_string_lossy().into_owned(),
        format!("-B{}/", bundle_dir.display()),
    ];
    argv.extend(arguments.iter().cloned());
    Ok(argv)
}
