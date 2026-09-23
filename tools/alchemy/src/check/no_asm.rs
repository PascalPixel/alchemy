//! Golden Sun repository scan orchestration; lexical policy lives in compiler::no_asm.

use crate::compiler::no_asm::{
    find_forbidden, find_named_source_tool_leaks, find_preprocessed, self_test, source_files,
    Finding,
};
use crate::compiler::routing::{
    cflags_for_target_source, root as compiler_root, uses_agbcc_compiler, CompilerTarget,
};
use crate::compiler::source_paths::SourcePaths;
use crate::compiler::translation_units::TranslationUnits;
use crate::targets::{target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::{Command, ExitCode};
use std::sync::Mutex;

const USAGE: &str =
    "usage: alchemy check no-asm [--target TARGET|--self-test]\n\nScan raw and preprocessed C for instruction, register, and ABI escape hatches.";
type Job = (String, Vec<String>);
/// One preprocessing command prefix of a target and the sources it expands.
type Group = ((String, Vec<String>), Vec<String>);

fn sibling(root: &Path, source: &str) -> Option<std::path::PathBuf> {
    (source.ends_with(".c") || source.ends_with(".C"))
        .then(|| root.join(source).with_extension("s"))
}

fn prefix(target: DecompTarget, source: &str) -> Result<Vec<String>, String> {
    let compiler = target.compiler;
    let mut flags = cflags_for_target_source(compiler, source);
    if uses_agbcc_compiler(compiler, source) {
        let include = compiler_root().join(target.source_dir);
        flags.extend([
            "-nostdinc".into(),
            "-mthumb".into(),
            format!(
                "-I{}",
                include
                    .with_file_name(if compiler == CompilerTarget::Tbs {
                        "INCLUDE"
                    } else {
                        "include"
                    })
                    .display()
            ),
            "-D__GNUC_MINOR__=9".into(),
        ]);
    }
    flags.push(format!("-D{}=1", target.edition_define));
    flags.extend(["-w".into(), "-E".into(), "-x".into(), "c".into()]);
    crate::compiler::bundle::compiler_command_for_target(compiler, &flags)
}

fn groups(root: &Path, target_ids: &[DecompTargetId]) -> Result<Vec<Group>, String> {
    let mut groups = BTreeMap::<(String, Vec<String>), Vec<String>>::new();
    let units = TranslationUnits::load(root)?;
    for &id in target_ids {
        let target = target_for(id);
        let paths = SourcePaths::load_for_game(root, target.compiler.as_str())?;
        paths.validate_tree()?;
        for source in paths.all_sources()? {
            let routing = source.owner.routing_path_for_game(target.compiler.as_str());
            let path = source.path.strip_prefix(root).unwrap_or(&source.path);
            let command = prefix(target, &routing.to_string_lossy())?;
            let group = groups.entry((id.as_str().into(), command)).or_default();
            group.push(path.to_string_lossy().into_owned());
        }
        for unit in &units.units {
            if unit.game != target.compiler.as_str() {
                continue;
            }
            let source = unit.source.to_string_lossy().into_owned();
            let command = prefix(target, &source)?;
            let group = groups.entry((id.as_str().into(), command)).or_default();
            group.push(source);
        }
    }
    for sources in groups.values_mut() {
        sources.sort();
        sources.dedup();
    }
    Ok(groups.into_iter().collect())
}

/// The identity of one source's expansion: the command, the source with
/// every header it includes, the compiler bundle and the scanner. A source
/// whose expansion was clean under the same identity is not expanded again,
/// as make skips an up-to-date object; any change to it or a header it
/// includes scans it afresh.
fn clean_key(root: &Path, label: &str, prefix: &[String], source: &str) -> Option<String> {
    let tree = crate::compiler::source_inputs::compiler_source_tree_signature(
        root,
        Path::new(source),
        &[prefix.to_vec()],
    )
    .ok()?;
    let identity = serde_json::to_vec(&(
        "no-asm-clean-v1",
        crate::compiler::bundle::executable_signature().ok()?,
        crate::compiler::bundle::compiler_bundle_signature(),
        label,
        crate::compiler::source_inputs::portable_commands(root, &[prefix.to_vec()]),
        source,
        crate::compiler::sha256::hex(&tree),
    ))
    .ok()?;
    Some(crate::compiler::sha256::hex(&identity))
}

/// Preprocessing batches for every source not already known clean, with
/// the clean keys each batch would record, and the number of inputs.
fn jobs(
    root: &Path,
    groups: Vec<Group>,
    cache: Option<&psynergy::cache::SqliteCache>,
) -> (Vec<(Job, Vec<String>)>, usize) {
    let inputs = groups.iter().map(|(_, sources)| sources.len()).sum();
    let mut jobs = Vec::new();
    for ((label, prefix), sources) in groups {
        let workers = std::thread::available_parallelism().map_or(1, |count| count.get().min(16));
        let chunk = sources.len().div_ceil(workers).max(1);
        let keys = std::thread::scope(|scope| {
            let handles = sources
                .chunks(chunk)
                .map(|part| {
                    let (label, prefix) = (&label, &prefix);
                    scope.spawn(move || {
                        part.iter()
                            .map(|source| clean_key(root, label, prefix, source))
                            .collect::<Vec<_>>()
                    })
                })
                .collect::<Vec<_>>();
            handles
                .into_iter()
                .flat_map(|handle| handle.join().expect("no-asm key worker panicked"))
                .collect::<Vec<_>>()
        });
        let pending = sources
            .into_iter()
            .zip(keys)
            .filter(|(_, key)| {
                let known = key
                    .as_deref()
                    .and_then(|key| cache?.get(key).ok().flatten());
                known.is_none()
            })
            .collect::<Vec<_>>();
        for batch in pending.chunks(128) {
            let mut command = prefix.clone();
            command.extend(batch.iter().map(|(source, _)| source.clone()));
            let keys = batch.iter().filter_map(|(_, key)| key.clone()).collect();
            jobs.push(((label.clone(), command), keys));
        }
    }
    (jobs, inputs)
}

fn run(root: &Path, job: &Job) -> Result<Vec<Finding>, String> {
    let output = Command::new(&job.1[0])
        .args(&job.1[1..])
        .current_dir(root)
        .output()
        .map_err(|error| error.to_string())?;
    let generated = job
        .1
        .iter()
        .filter_map(|arg| sibling(root, arg))
        .find(|path| path.exists());
    if let Some(path) = generated {
        return Err(format!("preprocessing created {}", path.display()));
    }
    let detail = String::from_utf8_lossy(&output.stderr);
    if !output.status.success() {
        return Err(format!("{} preprocessing failed: {}", job.0, detail.trim()));
    }
    let text = String::from_utf8_lossy(&output.stdout);
    Ok(find_preprocessed(&job.0, &text))
}

fn scan_preprocessed(
    root: &Path,
    target_ids: &[DecompTargetId],
) -> Result<(usize, usize, Vec<Finding>), String> {
    let cache = psynergy::cache::SqliteCache::open(&root.join("out/cache/no-asm.sqlite3")).ok();
    let (jobs, inputs) = jobs(root, groups(root, target_ids)?, cache.as_ref());
    let workers = std::thread::available_parallelism().map_or(1, |count| count.get().min(16));
    let results = Mutex::new(Vec::new());
    std::thread::scope(|scope| {
        for offset in 0..workers.min(jobs.len()).max(1) {
            let jobs = &jobs;
            let results = &results;
            let cache = cache.as_ref();
            scope.spawn(move || {
                for (index, (job, keys)) in jobs.iter().enumerate().skip(offset).step_by(workers) {
                    let result = run(root, job);
                    // Only a batch that expanded without findings is known clean.
                    if let (Ok(findings), Some(cache)) = (&result, cache) {
                        if findings.is_empty() {
                            for key in keys {
                                let _ = cache.put(key, &[("clean", b"1")]);
                            }
                        }
                    }
                    results.lock().unwrap().push((index, result));
                }
            });
        }
    });
    let mut results = results.into_inner().unwrap();
    results.sort_by_key(|(index, _)| *index);
    let mut findings = Vec::new();
    for (_, result) in results {
        findings.extend(result?);
    }
    findings.sort();
    findings.dedup();
    Ok((inputs, jobs.len(), findings))
}

fn macro_self_test() -> Result<(), String> {
    let root = compiler_root();
    let directory = tempfile::tempdir().map_err(|error| error.to_string())?;
    let source = directory.path().join("fixture.c");
    let text = "#if __GNUC_MINOR__ == 9\n#define ABI_KIND naked\n#else\n#define ABI_KIND packed\n#endif\nvoid f(void) __attribute__((ABI_KIND));\n";
    fs::write(&source, text).map_err(|error| error.to_string())?;
    if !find_forbidden("fixture.c", text).is_empty() {
        return Err("macro fixture did not evade the raw scan".into());
    }
    let target = target_for(DecompTargetId::TbsEn);
    let paths = SourcePaths::load_for_game(root, "tbs")?;
    let registered = paths
        .all_sources()?
        .into_iter()
        .find(|source| {
            uses_agbcc_compiler(
                CompilerTarget::Tbs,
                &source.owner.routing_path().to_string_lossy(),
            )
        })
        .ok_or("missing registered AGBCC source")?;
    let human = registered
        .path
        .strip_prefix(root)
        .map_err(|error| error.to_string())?;
    if sibling(root, &human.to_string_lossy()) != Some(root.join(human).with_extension("s")) {
        return Err("relative sibling path did not resolve under repository root".into());
    }
    let routing = registered.owner.routing_path_for_game("tbs");
    let mut command = prefix(target, &routing.to_string_lossy())?;
    command.push(source.to_string_lossy().into_owned());
    let found = run(root, &("macro-regression".into(), command))?;
    if found.len() != 1 || !found[0].token.contains("naked") {
        return Err("production AGBCC route missed macro-expanded naked ABI".into());
    }
    Ok(())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    match arguments {
        [arg] if arg == "-h" || arg == "--help" => success(USAGE),
        [arg] if arg == "--self-test" => match self_test().and_then(|_| macro_self_test()) {
            Ok(()) => success("self-test=ok"),
            Err(error) => fail(error),
        },
        [option, value] if option == "--target" => {
            let Some(id) = TARGET_IDS.iter().copied().find(|id| id.as_str() == value) else {
                return fail(format!("unknown target: {value}"));
            };
            scan_repository(&[id])
        }
        [] => scan_repository(&TARGET_IDS),
        _ => fail(USAGE),
    }
}

fn fail(message: impl std::fmt::Display) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}

fn success(message: &str) -> ExitCode {
    println!("{message}");
    ExitCode::SUCCESS
}

fn scan_repository(target_ids: &[DecompTargetId]) -> ExitCode {
    let root = compiler_root();
    // Maintained source under games/ and the drafts under recon/.
    let files = match ["games", "recon"]
        .into_iter()
        .map(|tree| source_files(&root.join(tree)))
        .collect::<Result<Vec<_>, _>>()
    {
        Ok(trees) if trees.iter().any(|files| !files.is_empty()) => trees.concat(),
        Ok(_) => return fail("ordinary-C gate scanned no files"),
        Err(error) => return fail(error),
    };
    let mut findings = Vec::new();
    for path in &files {
        let text = match fs::read_to_string(path) {
            Ok(text) => text,
            Err(error) => {
                eprintln!("{error}");
                return ExitCode::FAILURE;
            }
        };
        let name = path.strip_prefix(root).unwrap_or(path).to_string_lossy();
        findings.extend(find_forbidden(&name, &text));
        findings.extend(find_named_source_tool_leaks(&name, &text));
    }
    let (expanded, jobs, mut more) = match scan_preprocessed(root, target_ids) {
        Ok(result) => result,
        Err(error) => {
            eprintln!("error: {error}");
            return ExitCode::FAILURE;
        }
    };
    findings.append(&mut more);
    for item in &findings {
        eprintln!("{}:{}: forbidden {}", item.file, item.line, item.token);
    }
    let forbidden = findings.len();
    println!(
        "raw={} preprocessed={expanded} jobs={jobs} forbidden={forbidden}",
        files.len()
    );
    if findings.is_empty() {
        ExitCode::SUCCESS
    } else {
        fail("NONORDINARY C — use ordinary C or retain assembly")
    }
}

#[test]
fn production_macro_escape_hatches() {
    self_test().unwrap();
    macro_self_test().unwrap();
}
