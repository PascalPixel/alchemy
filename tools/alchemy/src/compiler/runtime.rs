//! Compiler runtime members built from the approved licensed agscc source.
//! Ordinary archives supply the linker; generated provenance stays ignored.
use crate::compiler::build_io::{read, text, write};
use crate::compiler::routing::{bundle, runtime_library_cflags, CompilerTarget};
use crate::compiler::sha256;
use fs2::FileExt;
use psynergy::process::run;
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};
use std::sync::Mutex;

const CONTAINER: &str = "agscc";
/// The container's target makefile fragment: generated sources and flags.
const TARGET_RULES: &str = "gcc/config/arm/t-arm-elf";
/// `LIBGCC2_CFLAGS` and `INTERNAL_CFLAGS` defines of the container's library build.
const LIBRARY_DEFINES: &[&str] = &[
    "-DCROSS_COMPILE",
    "-DIN_GCC",
    "-DIN_LIBGCC2",
    "-D__GCC_FLOAT_NOT_NEEDED",
];
/// The options `agscc/build.sh` configures the container with.
const CONFIGURE: &[&str] = &[
    "--build=i686-unknown-linux-gnu",
    "--host=i686-unknown-linux-gnu",
    "--target=arm-elf",
    "--with-cpu=arm7tdmi",
    "--enable-multilib",
    "--enable-interwork",
    "--enable-languages=c",
    "--without-headers",
    "--disable-shared",
    "--disable-threads",
    "--disable-nls",
    "--with-gnu-as",
    "--with-gnu-ld",
    "--disable-checking",
];
const CONFIGURE_CFLAGS: &str =
    "-O2 -std=gnu17 -Wno-error -Wno-implicit-int -Wno-implicit-function-declaration";

fn member_name(name: &str) -> bool {
    name.starts_with('_')
        && name.len() > 1
        && name
            .bytes()
            .all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
}
/// A generated library source and the container rule that writes it:
/// `name:` followed by `echo '...' > name` and `cat $(srcdir)/path >> name`.
fn generated_source(container: &Path, rules: &str, name: &str) -> Result<String, String> {
    let mut lines = rules.lines().skip_while(|line| {
        line.split_once(':')
            .is_none_or(|(target, _)| target.trim() != name)
    });
    if lines.next().is_none() {
        return Err(format!("{TARGET_RULES} has no rule for {name}"));
    }
    let mut output = String::new();
    for line in lines.take_while(|line| line.starts_with('\t')) {
        let command = line.trim();
        let redirect = [format!(" >> {name}"), format!(" > {name}")]
            .into_iter()
            .find_map(|suffix| command.strip_suffix(suffix.as_str()))
            .ok_or_else(|| format!("{TARGET_RULES}: unsupported {name} command: {command}"))?;
        if let Some(echoed) = redirect
            .strip_prefix("echo '")
            .and_then(|rest| rest.strip_suffix('\''))
        {
            output.push_str(echoed);
            output.push('\n');
        } else if let Some(path) = redirect.strip_prefix("cat $(srcdir)/") {
            let file = container.join("gcc").join(path);
            output.push_str(&String::from_utf8_lossy(&read(&file)?));
        } else {
            return Err(format!(
                "{TARGET_RULES}: unsupported {name} command: {command}"
            ));
        }
    }
    if output.is_empty() {
        return Err(format!("{TARGET_RULES}: empty rule for {name}"));
    }
    Ok(output)
}
/// A makefile variable's words, as the target fragment assigns them.
fn rule_variable(rules: &str, name: &str) -> Result<Vec<String>, String> {
    rules
        .lines()
        .find_map(|line| {
            let (key, value) = line.split_once('=')?;
            (key.trim() == name).then(|| value.split_whitespace().map(str::to_string).collect())
        })
        .ok_or_else(|| format!("{TARGET_RULES} does not assign {name}"))
}

enum Source {
    /// `-xassembler-with-cpp` source in the container.
    Assembly(PathBuf),
    /// A fine-grained source a container rule generates.
    Generated(String),
    /// C that includes the configured `tconfig.h`.
    Configured(PathBuf),
}
fn source_of(container: &Path, source: &str) -> Source {
    if !source.contains('/') {
        Source::Generated(source.to_string())
    } else if source.ends_with(".asm") {
        Source::Assembly(container.join(source))
    } else {
        Source::Configured(container.join(source))
    }
}

/// The configured container headers (`tconfig.h`, `tm.h`), produced once by
/// the container's own `configure` under ignored output.
fn configured(root: &Path) -> Result<PathBuf, String> {
    static LOCK: Mutex<()> = Mutex::new(());
    let container = root.join(CONTAINER);
    let script = container.join("gcc/configure");
    let identity = sha256::hex(
        &[
            read(&script)?,
            CONFIGURE.join("\0").into_bytes(),
            CONFIGURE_CFLAGS.as_bytes().to_vec(),
        ]
        .concat(),
    );
    let parent = root.join("tools/out/compiler-build/runtime");
    let directory = parent.join(format!("configure-{}", &identity[..16]));
    if directory.join("tconfig.h").is_file() {
        return Ok(directory);
    }
    let _guard = LOCK.lock().map_err(|_| "runtime configure lock poisoned")?;
    std::fs::create_dir_all(&parent).map_err(|error| format!("{}: {error}", parent.display()))?;
    let lock_path = parent.join(".configure.lock");
    let lock = std::fs::File::create(&lock_path)
        .map_err(|error| format!("{}: {error}", lock_path.display()))?;
    lock.lock_exclusive()
        .map_err(|error| format!("{}: {error}", lock_path.display()))?;
    if directory.join("tconfig.h").is_file() {
        return Ok(directory);
    }
    let work = tempfile::Builder::new()
        .prefix(".configure-")
        .tempdir_in(&parent)
        .map_err(|error| error.to_string())?;
    let output = std::process::Command::new("sh")
        .arg(&script)
        .arg(format!("--srcdir={}", container.join("gcc").display()))
        .args(CONFIGURE)
        .env("CFLAGS", CONFIGURE_CFLAGS)
        .current_dir(work.path())
        .output()
        .map_err(|error| format!("runtime configure: {error}"))?;
    if !output.status.success() || !work.path().join("tconfig.h").is_file() {
        let log = String::from_utf8_lossy(&output.stderr);
        let tail: Vec<_> = log.lines().rev().take(12).collect();
        return Err(format!(
            "runtime configure of {CONTAINER} failed: {}",
            tail.into_iter().rev().collect::<Vec<_>>().join("\n")
        ));
    }
    let staged = work.keep();
    std::fs::rename(&staged, &directory)
        .map_err(|error| format!("{}: {error}", directory.display()))?;
    Ok(directory)
}

/// The compile command of one member, without its output option.
fn member_command(
    root: &Path,
    source: &str,
    member: &str,
    work: &Path,
) -> Result<Vec<String>, String> {
    let container = root.join(CONTAINER);
    let rules = String::from_utf8_lossy(&read(container.join(TARGET_RULES))?).into_owned();
    let mut command = vec![
        text(bundle().join("xgcc")),
        format!("-B{}/", bundle().display()),
    ];
    command.extend(runtime_library_cflags());
    command.extend(LIBRARY_DEFINES.iter().map(|flag| (*flag).to_string()));
    command.extend(rule_variable(&rules, "TARGET_LIBGCC2_CFLAGS")?);
    command.extend(["-isystem".to_string(), text(container.join("gcc/ginclude"))]);
    let includes = [
        container.join("gcc"),
        container.join("gcc/config"),
        container.join("include"),
    ];
    match source_of(&container, source) {
        Source::Assembly(path) => {
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([
                format!("-DL{member}"),
                "-xassembler-with-cpp".into(),
                text(path),
            ]);
        }
        Source::Generated(name) => {
            let generated = work.join(&name);
            if !generated.is_file() {
                write(&generated, generated_source(&container, &rules, &name)?)?;
            }
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([
                "-DFINE_GRAINED_LIBRARIES".into(),
                format!("-DL{member}"),
                text(generated),
            ]);
        }
        Source::Configured(path) => {
            command.push(format!("-I{}", configured(root)?.display()));
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([format!("-DL{member}"), text(path)]);
        }
    }
    Ok(command)
}

const ARCHIVE_FORMAT: u32 = 1;
const ARCHIVE_USAGE: &str = "usage: alchemy build runtime --output tools/out/compiler-runtime/.../libgcc.a MEMBER=SOURCE...\nBuild an ordinary archive from approved agscc sources using their existing library rules.";

#[derive(Debug)]
struct ArchiveReceipt {
    format: u32,
    compiler_sha256: String,
    ar_sha256: String,
    archive_sha256: String,
    members: Vec<MemberReceipt>,
}

#[derive(Debug)]
struct MemberReceipt {
    member: String,
    source: String,
    source_sha256: String,
    plan_sha256: String,
    object_sha256: String,
}

impl ArchiveReceipt {
    /// The receipt as TSV: its settings as `# name value` lines, then one
    /// row per member under a header row.
    fn table(&self) -> Vec<u8> {
        let mut table = format!(
            "# format\t{}\n# compiler_sha256\t{}\n# ar_sha256\t{}\n# archive_sha256\t{}\nmember\tsource\tsource_sha256\tplan_sha256\tobject_sha256\n",
            self.format, self.compiler_sha256, self.ar_sha256, self.archive_sha256
        );
        for member in &self.members {
            table += &format!(
                "{}\t{}\t{}\t{}\t{}\n",
                member.member,
                member.source,
                member.source_sha256,
                member.plan_sha256,
                member.object_sha256
            );
        }
        table.into_bytes()
    }
}

fn validate_members(root: &Path, members: &[(String, String)]) -> Result<(), String> {
    if members.is_empty() {
        return Err("runtime archive needs at least one member".into());
    }
    let mut seen = BTreeSet::new();
    let container = std::fs::canonicalize(root.join(CONTAINER)).map_err(|e| e.to_string())?;
    for (member, source) in members {
        if !member_name(member) || !seen.insert(member) {
            return Err(format!("invalid or duplicate runtime member {member}"));
        }
        if source == "dp-bit.c" {
            continue;
        }
        let path = Path::new(source);
        if !path.starts_with("gcc")
            || path
                .components()
                .any(|part| !matches!(part, std::path::Component::Normal(_)))
            || !matches!(
                path.extension().and_then(|ext| ext.to_str()),
                Some("c" | "asm")
            )
        {
            return Err(format!(
                "runtime source {source} is not approved agscc source"
            ));
        }
        let path = std::fs::canonicalize(container.join(path)).map_err(|e| e.to_string())?;
        if !path.starts_with(&container) || !path.is_file() {
            return Err(format!(
                "runtime source {source} escapes approved agscc source"
            ));
        }
        run(
            &[
                "git",
                "-C",
                CONTAINER,
                "ls-files",
                "--error-unmatch",
                "--",
                source,
            ],
            root,
        )
        .map_err(|_| format!("runtime source {source} is not part of approved agscc source"))?;
    }
    Ok(())
}

fn archive_path(root: &Path, output: &Path, create: bool) -> Result<PathBuf, String> {
    let root = std::fs::canonicalize(root).map_err(|e| e.to_string())?;
    let output = crate::compiler::build_io::rooted(&root, output);
    let allowed = root.join("tools/out/compiler-runtime");
    if !output.starts_with(&allowed)
        || output
            .components()
            .any(|part| matches!(part, std::path::Component::ParentDir))
        || output.extension().and_then(|ext| ext.to_str()) != Some("a")
    {
        return Err("runtime archives must stay under ignored tools/out/compiler-runtime/".into());
    }
    let parent = output.parent().ok_or("runtime archive has no parent")?;
    let mut existing = parent;
    while std::fs::symlink_metadata(existing).is_err() {
        existing = existing
            .parent()
            .ok_or("runtime archive has no existing parent")?;
    }
    let existing = std::fs::canonicalize(existing).map_err(|e| e.to_string())?;
    if !existing.starts_with(&allowed) && !allowed.starts_with(&existing) {
        return Err("runtime output symlink escapes ignored output".into());
    }
    if create {
        std::fs::create_dir_all(parent).map_err(|e| e.to_string())?;
    }
    let parent = std::fs::canonicalize(parent).map_err(|e| e.to_string())?;
    if !parent.starts_with(&allowed) {
        return Err("runtime output escapes ignored output".into());
    }
    let output = parent.join(
        output
            .file_name()
            .ok_or("runtime archive has no filename")?,
    );
    if std::fs::symlink_metadata(&output).is_ok() {
        let existing = std::fs::canonicalize(&output).map_err(|e| e.to_string())?;
        if !existing.starts_with(&allowed) {
            return Err("runtime archive symlink escapes ignored output".into());
        }
    }
    Ok(output)
}

fn runtime_tools(root: &Path) -> Result<(String, PathBuf, String), String> {
    run(
        &[
            "make",
            "--no-print-directory",
            "--silent",
            "compiler-source-check",
        ],
        root,
    )?;
    crate::compiler::bundle::validate_bundle(CompilerTarget::Tbs)?;
    let ar = root.join("tools/out/binutils/bin/arm-none-eabi-ar");
    let ar_sha256 = sha256::hex(&read(&ar)?);
    Ok((
        crate::compiler::bundle::compiler_bundle_signature_checked()?,
        ar,
        ar_sha256,
    ))
}

fn archive_lock(path: &Path) -> Result<std::fs::File, String> {
    std::fs::OpenOptions::new()
        .create(true)
        .truncate(false)
        .read(true)
        .write(true)
        .open(path.with_extension("lock"))
        .map_err(|e| e.to_string())
}

fn check_archive_outputs(root: &Path, archive: &Path) -> Result<(), String> {
    let allowed = root.join("tools/out/compiler-runtime");
    for path in [
        archive.with_extension("lock"),
        archive.with_extension("objects"),
        archive.with_extension("a.provenance.tsv"),
    ] {
        if std::fs::symlink_metadata(&path).is_ok()
            && !std::fs::canonicalize(&path)
                .map_err(|e| e.to_string())?
                .starts_with(&allowed)
        {
            return Err("runtime provenance or object symlink escapes ignored output".into());
        }
    }
    Ok(())
}

fn member_plan(
    root: &Path,
    work: &Path,
    member: &str,
    source: &str,
) -> Result<(Vec<String>, String, String), String> {
    let command = member_command(root, source, member, work)?;
    let input = Path::new(
        command
            .last()
            .ok_or("runtime compile command has no source")?,
    );
    let source_sha256 = sha256::hex(&read(input)?);
    let mut preprocess = command.clone();
    preprocess.extend(["-E".into(), "-P".into()]);
    let expanded =
        run(&preprocess, work).map_err(|error| format!("runtime member {member}: {error}"))?;
    let portable: Vec<_> = command
        .iter()
        .map(|part| {
            part.replace(&text(work), "<work>")
                .replace(&text(root), "<root>")
        })
        .collect();
    let plan = format!(
        "runtime-archive-v1\n{}\n{}\n",
        portable.join("\t"),
        sha256::hex(expanded.as_bytes())
    );
    Ok((command, source_sha256, sha256::hex(plan.as_bytes())))
}

fn compile_member(command: &[String], member: &str, work: &Path) -> Result<PathBuf, String> {
    let object = work.join(format!("{member}.o"));
    let mut compile = command.to_vec();
    compile.extend(["-c".into(), "-o".into(), text(&object)]);
    run(&compile, work).map_err(|error| format!("runtime member {member}: {error}"))?;
    Ok(object)
}

fn check_archive_members(ar: &Path, archive: &Path, objects: &[PathBuf]) -> Result<(), String> {
    let work = archive.parent().ok_or("archive has no parent")?;
    let names = run(&[text(ar), "t".into(), text(archive)], work)?;
    let expected: Vec<_> = objects
        .iter()
        .map(|p| text(Path::new(p.file_name().unwrap())))
        .collect();
    if names.lines().collect::<Vec<_>>() != expected.iter().map(String::as_str).collect::<Vec<_>>()
    {
        return Err("runtime archive member order or names differ".into());
    }
    for (name, object) in expected.iter().zip(objects) {
        let output = std::process::Command::new(ar)
            .args(["p", &text(archive), name])
            .current_dir(work)
            .output()
            .map_err(|e| e.to_string())?;
        if !output.status.success() || output.stdout != read(object)? {
            return Err(format!(
                "runtime archive member {name} differs from its source-built object"
            ));
        }
    }
    Ok(())
}

/// Build the requested archive in the given member order, without a game registry.
pub fn build_archive(
    root: &Path,
    output: &Path,
    members: &[(String, String)],
) -> Result<PathBuf, String> {
    let root = std::fs::canonicalize(root).map_err(|e| e.to_string())?;
    validate_members(&root, members)?;
    let output = archive_path(&root, output, true)?;
    check_archive_outputs(&root, &output)?;
    let lock = archive_lock(&output)?;
    lock.lock_exclusive().map_err(|e| e.to_string())?;
    let (compiler_sha256, ar, ar_sha256) = runtime_tools(&root)?;
    let work = tempfile::Builder::new()
        .prefix(".runtime-")
        .tempdir_in(output.parent().unwrap())
        .map_err(|e| e.to_string())?;
    let mut receipts = Vec::new();
    let mut objects = Vec::new();
    for (member, source) in members {
        let (command, source_sha256, plan_sha256) =
            member_plan(&root, work.path(), member, source)?;
        let object = compile_member(&command, member, work.path())?;
        receipts.push(MemberReceipt {
            member: member.clone(),
            source: source.clone(),
            source_sha256,
            plan_sha256,
            object_sha256: sha256::hex(&read(&object)?),
        });
        objects.push(object);
    }
    let archive = work.path().join(output.file_name().unwrap());
    let mut command = vec![text(&ar), "crsD".into(), text(&archive)];
    command.extend(objects.iter().map(text));
    run(&command, work.path())?;
    check_archive_members(&ar, &archive, &objects)?;
    let receipt = ArchiveReceipt {
        format: ARCHIVE_FORMAT,
        compiler_sha256,
        ar_sha256,
        archive_sha256: sha256::hex(&read(&archive)?),
        members: receipts,
    };
    std::fs::rename(&archive, &output).map_err(|e| e.to_string())?;
    let directory = output.with_extension("objects");
    if directory.exists() {
        std::fs::remove_dir_all(&directory).map_err(|e| e.to_string())?;
    }
    std::fs::rename(work.keep(), directory).map_err(|e| e.to_string())?;
    write(output.with_extension("a.provenance.tsv"), receipt.table())?;
    Ok(output)
}

pub fn entry(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("{ARCHIVE_USAGE}");
        return Ok(());
    }
    let mut output = None;
    let mut members = Vec::new();
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        if arg == "--output" {
            output = Some(PathBuf::from(args.next().ok_or("--output needs a value")?));
        } else if let Some(value) = arg.strip_prefix("--output=") {
            output = Some(PathBuf::from(value));
        } else {
            let (member, source) = arg.split_once('=').ok_or(ARCHIVE_USAGE)?;
            members.push((member.into(), source.into()));
        }
    }
    let archive = build_archive(
        crate::compiler::routing::root(),
        &output.ok_or(ARCHIVE_USAGE)?,
        &members,
    )?;
    println!("archive={} members={}", archive.display(), members.len());
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn archive_rejects_outside_generated_and_duplicate_members() {
        let root = tempfile::tempdir().unwrap();
        std::fs::create_dir(root.path().join(CONTAINER)).unwrap();
        for source in ["other.c", "../other.c", "/tmp/other.c", "gcc/../../other.c"] {
            assert!(validate_members(root.path(), &[("_first".into(), source.into())]).is_err());
        }
        let valid = vec![("_first".into(), "dp-bit.c".into())];
        assert!(validate_members(root.path(), &valid).is_ok());
        let mut duplicate = valid;
        duplicate.push(("_first".into(), "dp-bit.c".into()));
        assert!(validate_members(root.path(), &duplicate)
            .unwrap_err()
            .contains("duplicate"));
        for path in [
            "libgcc.a",
            "out/libgcc.a",
            "tools/out/compiler-runtime/../../source.a",
        ] {
            assert!(archive_path(root.path(), Path::new(path), true).is_err());
        }
    }
    #[cfg(unix)]
    #[test]
    fn archive_rejects_source_and_output_symlink_escapes() {
        let root = tempfile::tempdir().unwrap();
        let outside = tempfile::tempdir().unwrap();
        std::fs::create_dir_all(root.path().join("agscc/gcc")).unwrap();
        std::fs::write(outside.path().join("runtime.c"), "int outside;\n").unwrap();
        std::os::unix::fs::symlink(
            outside.path().join("runtime.c"),
            root.path().join("agscc/gcc/runtime.c"),
        )
        .unwrap();
        assert!(
            validate_members(root.path(), &[("_first".into(), "gcc/runtime.c".into())]).is_err()
        );
        std::fs::create_dir_all(root.path().join("tools/out")).unwrap();
        std::os::unix::fs::symlink(
            outside.path(),
            root.path().join("tools/out/compiler-runtime"),
        )
        .unwrap();
        assert!(archive_path(
            root.path(),
            Path::new("tools/out/compiler-runtime/new/libgcc.a"),
            true
        )
        .is_err());
        assert!(!outside.path().join("new").exists());
    }
    #[test]
    fn generated_sources_follow_the_container_rule() {
        let container = tempfile::tempdir().unwrap();
        std::fs::create_dir_all(container.path().join("gcc/config")).unwrap();
        std::fs::write(container.path().join("gcc/config/body.c"), "int body;\n").unwrap();
        let rules = "X = 1\n\nwide.c: $(srcdir)/config/body.c\n\techo '#define WIDE' > wide.c\n\techo '#define ORDER' >> wide.c\n\tcat $(srcdir)/config/body.c >> wide.c\n\nother.c:\n\techo x > other.c\n";
        assert_eq!(
            generated_source(container.path(), rules, "wide.c").unwrap(),
            "#define WIDE\n#define ORDER\nint body;\n"
        );
        assert!(generated_source(container.path(), rules, "other.c").is_err());
        assert!(generated_source(container.path(), rules, "absent.c").is_err());
        assert_eq!(
            rule_variable("FLAGS = -Da -fb\n", "FLAGS").unwrap(),
            ["-Da", "-fb"]
        );
        assert!(rule_variable("FLAGS = -Da\n", "OTHER").is_err());
    }
}
