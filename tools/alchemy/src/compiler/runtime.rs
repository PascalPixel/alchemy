use crate::compiler::build_io::{read, text, write};
use crate::compiler::routing::{bundle, runtime_library_cflags};
use crate::compiler::sha256;
use fs2::FileExt;
use crate::process::run;
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};

const CONTAINER: &str = "agscc";
const TARGET_RULES: &str = "gcc/config/arm/t-arm-elf";
const LIBRARY_DEFINES: &[&str] = &[
    "-DCROSS_COMPILE",
    "-DIN_GCC",
    "-DIN_LIBGCC2",
    "-D__GCC_FLOAT_NOT_NEEDED",
];
fn member_name(name: &str) -> bool {
    name.starts_with('_')
        && name.len() > 1
        && name
            .bytes()
            .all(|byte| byte.is_ascii_alphanumeric() || byte == b'_')
}
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
    Assembly(PathBuf),
    Generated(String),
    Target(PathBuf),
}
fn source_of(container: &Path, source: &str) -> Source {
    if !source.contains('/') {
        Source::Generated(source.to_string())
    } else if source.ends_with(".asm") {
        Source::Assembly(container.join(source))
    } else {
        Source::Target(container.join(source))
    }
}

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
        Source::Target(path) => {
            for name in ["tconfig.h", "tm.h"] {
                write(
                    work.join(name),
                    read(container.join("config/arm").join(name))?,
                )?;
            }
            command.push(format!("-I{}", work.display()));
            command.extend(includes.iter().map(|path| format!("-I{}", path.display())));
            command.extend([format!("-DL{member}"), text(path)]);
        }
    }
    Ok(command)
}

const ARCHIVE_FORMAT: u32 = 2;
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
    fn parse(text: &str) -> Result<Self, String> {
        let mut lines = text.lines();
        let mut field = |name: &str| -> Result<String, String> {
            lines.next().and_then(|line| line.strip_prefix(&format!("# {name}\t")))
                .map(str::to_owned).ok_or_else(|| format!("runtime receipt missing {name}"))
        };
        let format = field("format")?.parse::<u32>().map_err(|e| e.to_string())?;
        if format != ARCHIVE_FORMAT { return Err("unsupported runtime receipt".into()); }
        let compiler_sha256 = field("compiler_sha256")?;
        let ar_sha256 = field("ar_sha256")?;
        let archive_sha256 = field("archive_sha256")?;
        if lines.next() != Some("member\tsource\tsource_sha256\tplan_sha256\tobject_sha256") {
            return Err("invalid runtime member header".into());
        }
        let members = lines.map(|line| {
            let fields = line.split('\t').collect::<Vec<_>>();
            let [member, source, source_sha256, plan_sha256, object_sha256] = fields.as_slice()
                else { return Err("invalid runtime member receipt".to_owned()) };
            Ok(MemberReceipt { member: member.to_string(), source: source.to_string(),
                source_sha256: source_sha256.to_string(), plan_sha256: plan_sha256.to_string(), object_sha256: object_sha256.to_string() })
        }).collect::<Result<Vec<_>, _>>()?;
        Ok(Self { format, compiler_sha256, ar_sha256, archive_sha256, members })
    }
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

/// Ordinary archive names grant nothing. Check our source-built receipt and
/// every actual archive member once per measurement process.
pub(crate) fn credited_member(root: &Path, object: &str) -> Result<bool, String> {
    let Some((archive, member)) = object.rsplit_once('(') else { return Ok(false) };
    let Some(member) = member.strip_suffix(')') else { return Ok(false) };
    if !archive.ends_with(".a") { return Ok(false); }
    let Ok(archive) = archive_path(root, Path::new(archive), false) else { return Ok(false) };
    type VerifiedArchives = std::collections::BTreeMap<PathBuf, Result<BTreeSet<String>, String>>;
    static VERIFIED: std::sync::OnceLock<std::sync::Mutex<VerifiedArchives>> = std::sync::OnceLock::new();
    let mut verified = VERIFIED.get_or_init(Default::default).lock().unwrap();
    let members = verified.entry(archive.clone()).or_insert_with(|| verify_receipt(root, &archive));
    members.as_ref().map(|members| members.contains(member)).map_err(Clone::clone)
}

fn verify_receipt(root: &Path, archive: &Path) -> Result<BTreeSet<String>, String> {
    check_archive_outputs(root, archive)?;
    let receipt = ArchiveReceipt::parse(&std::fs::read_to_string(archive.with_extension("a.provenance.tsv")).map_err(|e| e.to_string())?)?;
    let ar = root.join("tools/out/binutils/bin/arm-none-eabi-ar");
    if receipt.archive_sha256 != sha256::hex(&read(archive)?)
        || receipt.ar_sha256 != sha256::hex(&read(&ar)?)
        || receipt.compiler_sha256 != crate::compiler::bundle::runtime_bundle_signature_checked()? {
        return Err("runtime archive or toolchain differs from source-built receipt; rebuild".into());
    }
    let members = receipt.members.iter().map(|m| (m.member.clone(), m.source.clone())).collect::<Vec<_>>();
    validate_members(root, &members)?;
    let work = archive.with_extension("objects");
    let mut objects = Vec::new();
    for member in &receipt.members {
        let (_, source, plan) = member_plan(root, &work, &member.member, &member.source)?;
        let object = work.join(format!("{}.o", member.member));
        if source != member.source_sha256 || plan != member.plan_sha256 || sha256::hex(&read(&object)?) != member.object_sha256 {
            return Err(format!("runtime member {} differs from source-built receipt; rebuild", member.member));
        }
        objects.push(object);
    }
    check_archive_members(&ar, archive, &objects)?;
    Ok(receipt.members.iter().map(|m| format!("{}.o", m.member)).collect())
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
    crate::compiler::bundle::validate_bundle()?;
    let ar = root.join("tools/out/binutils/bin/arm-none-eabi-ar");
    let ar_sha256 = sha256::hex(&read(&ar)?);
    Ok((
        crate::compiler::bundle::runtime_bundle_signature_checked()?,
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
mod evidence_tests {
    use super::*;

    #[test]
    fn archive_names_without_build_provenance_grant_nothing() {
        let root = tempfile::tempdir().unwrap();
        assert!(!credited_member(root.path(), "/elsewhere/libgcc.a(_pretend.o)").unwrap());
        std::fs::create_dir_all(root.path().join("tools/out/compiler-runtime")).unwrap();
        let archive = std::fs::canonicalize(root.path()).unwrap().join("tools/out/compiler-runtime/renamed.a");
        std::fs::write(&archive, "an unrelated archive").unwrap();
        assert!(credited_member(root.path(), &format!("{}(_pretend.o)", archive.display())).is_err());
    }

    #[test]
    fn archive_receipts_round_trip_and_reject_malformed_records() {
        let receipt = ArchiveReceipt { format: ARCHIVE_FORMAT, compiler_sha256: "compiler".into(),
            ar_sha256: "archiver".into(), archive_sha256: "archive".into(), members: vec![MemberReceipt {
                member: "_example".into(), source: "gcc/example.c".into(), source_sha256: "source".into(),
                plan_sha256: "plan".into(), object_sha256: "object".into() }] };
        let text = String::from_utf8(receipt.table()).unwrap();
        assert_eq!(ArchiveReceipt::parse(&text).unwrap().table(), receipt.table());
        assert!(ArchiveReceipt::parse(&text.replace("\tobject\n", "\n")).is_err());
        assert!(ArchiveReceipt::parse(&text.replace("# format\t2", "# format\t999")).is_err());
    }
}
