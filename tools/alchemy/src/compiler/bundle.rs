use crate::compiler::bundle_data::{source_record, SOURCE_RECORD};
use crate::compiler::routing::{
    agbcc_arm_driver, agbcc_driver, bundle, bundle_for, root, CompilerTarget,
};
use crate::compiler::sha256;
use fs2::FileExt;
use std::fs::{self, File, OpenOptions};
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::{Mutex, OnceLock};
pub type Result<T> = std::result::Result<T, String>;
struct SharedBundleLock {
    _file: File,
}
static SHARED_BUNDLE_LOCK: OnceLock<Result<SharedBundleLock>> = OnceLock::new();
pub fn acquire_compiler_bundle_shared_lock() -> Result<()> {
    let result = SHARED_BUNDLE_LOCK.get_or_init(|| {
        if !bundle().is_dir() {
            return Err(format!(
                "compiler toolchain is not installed at {}; run alchemy bootstrap",
                bundle().display()
            ));
        }
        let path = bundle().parent().unwrap().join(".compiler.lock");
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
pub fn validate_bundle(target: CompilerTarget) -> Result<()> {
    ensure_compiler_bundle_access()?;
    if validation_cached(target.as_str()) {
        return Ok(());
    }
    let bundle_dir = bundle_for(target);
    validate_game_directory(&bundle_dir, target)?;
    cache_validation(target.as_str());
    Ok(())
}
fn validate_game_directory(bundle_dir: &Path, target: CompilerTarget) -> Result<()> {
    validate_source_record(bundle_dir)?;
    for name in ["xgcc", "cpp0", "tradcpp0", "cc1", "as"] {
        if executable_mode(&bundle_dir.join(name)) != Some(true) {
            return Err(format!(
                "compiler {} bundle is missing executable {name}",
                target.as_str()
            ));
        }
    }
    smoke(&[
        bundle_dir.join("xgcc").to_string_lossy().into_owned(),
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
            "compiler {} smoke compile failed: {detail}",
            target.as_str()
        )
    })?;
    Ok(())
}
pub fn validate_agbcc_bundle() -> Result<()> {
    ensure_compiler_bundle_access()?;
    if validation_cached("agbcc") {
        return Ok(());
    }
    validate_source_record(&bundle())?;
    validate_agbcc_driver(&agbcc_driver())?;
    validate_agbcc_arm_driver(&agbcc_arm_driver())?;
    cache_validation("agbcc");
    Ok(())
}
/// A toolchain is admitted by the pinned source it was built from, on any
/// system, never by the bytes one host's C compiler happened to produce.
fn validate_source_record(directory: &Path) -> Result<()> {
    let record = fs::read_to_string(directory.join(SOURCE_RECORD)).unwrap_or_default();
    if record != source_record() {
        return Err(format!(
            "compiler toolchain at {} was not built from the pinned agbcc and agscc sources; run alchemy bootstrap --build",
            directory.display()
        ));
    }
    Ok(())
}
fn validate_agbcc_driver(driver: &Path) -> Result<()> {
    validate_driver(driver, "agbcc/old_agbcc", &["-mthumb-interwork", "-O2"])
}
fn validate_agbcc_arm_driver(driver: &Path) -> Result<()> {
    validate_driver(driver, "agbcc/agbcc_arm", &["-mthumb-interwork", "-O2"])
}
fn validate_driver(driver: &Path, name: &str, flags: &[&str]) -> Result<()> {
    if executable_mode(driver) != Some(true) {
        return Err(format!("compiler bundle is missing executable {name}"));
    }
    let mut command = vec![driver.to_string_lossy().into_owned(), "/dev/null".into()];
    command.extend(flags.iter().map(|flag| flag.to_string()));
    command.extend(["-o".into(), "/dev/null".into()]);
    smoke(&command)
        .map_err(|detail| format!("compiler bundle {name} smoke compile failed: {detail}"))?;
    Ok(())
}
/// Validate a prospective installation without changing routing or cache state.
pub fn validate_installation(directory: &Path) -> Result<()> {
    ensure_no_codegen_environment_overrides()?;
    validate_game_directory(directory, CompilerTarget::Tbs)?;
    validate_game_directory(directory, CompilerTarget::Tla)?;
    validate_agbcc_driver(&directory.join("agbcc/old_agbcc"))?;
    validate_agbcc_arm_driver(&directory.join("agbcc/agbcc_arm"))
}
pub fn signature_paths() -> Vec<PathBuf> {
    let bundle_dir = bundle();
    vec![
        bundle_dir.join("xgcc"),
        bundle_dir.join("cpp0"),
        bundle_dir.join("tradcpp0"),
        bundle_dir.join("cc1"),
        bundle_dir.join("as"),
        agbcc_driver(),
        agbcc_arm_driver(),
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
/// A bundle path as the signature names it: from the checkout's root when
/// inside it, so every worktree of one bundle has one signature and shares
/// the caches keyed by it. The bytes and modes are hashed as they are.
fn portable_path(path: &Path) -> Vec<u8> {
    path_bytes(path.strip_prefix(root()).unwrap_or(path))
}
fn append_bundle_path_signature(stream: &mut Vec<u8>, path: &Path) {
    append_signature_frame(stream, &portable_path(path));
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
        append_signature_frame(&mut stream, &portable_path(include));
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
            root().join("games/THE BROKEN SEAL/INCLUDE"),
            root().join("games/THE LOST AGE/INCLUDE"),
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
/// Identity of every executable a build step runs: the approved compilers
/// and their assembler, and the GNU binutils that assemble listings and
/// link. Object and overlay keys include it, so a changed tool rebuilds
/// what it made instead of reusing an object another tool wrote.
pub fn toolchain_signature() -> String {
    static SIGNATURE: OnceLock<String> = OnceLock::new();
    SIGNATURE
        .get_or_init(|| {
            let bin = root().join("tools/out/binutils/bin");
            let mut paths = signature_paths();
            paths.extend(
                [
                    "arm-none-eabi-as",
                    "arm-none-eabi-ld",
                    "arm-none-eabi-objcopy",
                ]
                .map(|name| bin.join(name)),
            );
            compiler_bundle_signature_for_paths(&paths, &[])
        })
        .clone()
}
pub fn compiler_bundle_signature_checked() -> Result<String> {
    ensure_compiler_bundle_access()?;
    Ok(compiler_bundle_signature())
}
/// Identity of the tool code that decides what a build stage caches: the
/// digest `build.rs` takes of Alchemy's build sources, every Psynergy and
/// ags source, the member crate manifests and lockfile. Alchemy's checks,
/// reports and other readers of build outputs do not contribute: changing
/// those readers rebuilds nothing. Changed build producers invalidate
/// their cached outputs; the linked overlay also keys on libgcc's bytes.
pub fn executable_signature() -> Result<String> {
    Ok(env!("ALCHEMY_BUILD_IMPLEMENTATION").to_string())
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
pub fn compiler_command_for_target(
    target: CompilerTarget,
    arguments: &[String],
) -> Result<Vec<String>> {
    validate_bundle(target)?;
    let bundle_dir = bundle_for(target);
    let mut argv = vec![
        bundle_dir.join("xgcc").to_string_lossy().into_owned(),
        format!("-B{}/", bundle_dir.display()),
    ];
    argv.extend(arguments.iter().cloned());
    Ok(argv)
}
