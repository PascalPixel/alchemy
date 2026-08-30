use crate::{
    find_forbidden, find_named_source_tool_leaks, find_preprocessed, self_test, source_files,
    Finding,
};
use compiler_core::bundle::compiler_command_for_target;
use compiler_core::routing::{
    cflags_for_target_source, root as compiler_root, uses_agbcc_compiler, CompilerTarget,
};
use compiler_core::source_paths::SourcePaths;
use compiler_core::translation_units::TranslationUnits;
use decomp_targets::{target_for, DecompCompilerTarget, DecompTarget, DecompTargetId, TARGET_IDS};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::{Command, ExitCode};
use std::sync::Mutex;
const USAGE: &str = "usage: no-asm-c [--target TARGET|--self-test]\n\nScan raw and preprocessed C for instruction, register, and ABI escape hatches.";
type Job = (String, Vec<String>);
fn fail(message: impl std::fmt::Display) -> ExitCode {
    eprintln!("{message}");
    ExitCode::FAILURE
}
fn success(message: &str) -> ExitCode {
    println!("{message}");
    ExitCode::SUCCESS
}
fn sibling(root: &Path, source: &str) -> Option<std::path::PathBuf> {
    source
        .ends_with(".c")
        .then(|| root.join(source).with_extension("s"))
}
fn prefix(target: DecompTarget, source: &str) -> Result<Vec<String>, String> {
    let compiler = match target.compiler {
        DecompCompilerTarget::Gs1 => CompilerTarget::Gs1,
        DecompCompilerTarget::Gs2 => CompilerTarget::Gs2,
    };
    let mut flags = cflags_for_target_source(compiler, source);
    if uses_agbcc_compiler(compiler, source) {
        let include = compiler_root().join(target.source_dir);
        flags.extend([
            "-nostdinc".into(),
            "-mthumb".into(),
            format!("-I{}", include.with_file_name("include").display()),
            "-D__GNUC_MINOR__=9".into(),
        ]);
    }
    flags.push(format!("-D{}=1", target.edition_define));
    flags.extend(["-w".into(), "-E".into()]);
    compiler_command_for_target(compiler, &flags)
}
fn jobs(root: &Path, target_ids: &[DecompTargetId]) -> Result<(Vec<Job>, usize), String> {
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
    let inputs = groups.values().map(Vec::len).sum();
    let mut jobs = Vec::new();
    for ((label, prefix), sources) in groups {
        for batch in sources.chunks(128) {
            let mut command = prefix.clone();
            command.extend(batch.iter().cloned());
            jobs.push((label.clone(), command));
        }
    }
    Ok((jobs, inputs))
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
    let (jobs, inputs) = jobs(root, target_ids)?;
    let workers = std::thread::available_parallelism().map_or(1, |count| count.get().min(16));
    let results = Mutex::new(Vec::new());
    std::thread::scope(|scope| {
        for offset in 0..workers.min(jobs.len()).max(1) {
            let jobs = &jobs;
            let results = &results;
            scope.spawn(move || {
                for (index, job) in jobs.iter().enumerate().skip(offset).step_by(workers) {
                    results.lock().unwrap().push((index, run(root, job)));
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
pub(crate) fn macro_self_test() -> Result<(), String> {
    let root = compiler_root();
    let directory = std::env::temp_dir().join(format!("alchemy-no-asm-{}", std::process::id()));
    let source = directory.join("fixture.c");
    let text = "#if __GNUC_MINOR__ == 9\n#define ABI_KIND naked\n#else\n#define ABI_KIND packed\n#endif\nvoid f(void) __attribute__((ABI_KIND));\n";
    let result = (|| {
        fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
        fs::write(&source, text).map_err(|error| error.to_string())?;
        if !find_forbidden("fixture.c", text).is_empty() {
            return Err("macro fixture did not evade the raw scan".into());
        }
        let target = target_for(DecompTargetId::Gs1En);
        let paths = SourcePaths::load_for_game(root, "gs1")?;
        let human = Path::new("games/gs1/src/save/read_flash_id.c");
        if sibling(root, &human.to_string_lossy()) != Some(root.join(human).with_extension("s")) {
            return Err("relative sibling path did not resolve under repository root".into());
        }
        let owner = paths.owner_for_path(human)?.ok_or("missing AGBCC owner")?;
        let routing = owner.routing_path_for_game("gs1");
        let mut command = prefix(target, &routing.to_string_lossy())?;
        command.push(source.to_string_lossy().into_owned());
        let found = run(root, &("macro-regression".into(), command))?;
        if found.len() != 1 || !found[0].token.contains("naked") {
            return Err("production AGBCC route missed macro-expanded naked ABI".into());
        }
        Ok(())
    })();
    let _ = fs::remove_dir_all(directory);
    result
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
fn scan_repository(target_ids: &[DecompTargetId]) -> ExitCode {
    let root = compiler_root();
    let files = match source_files(&root.join("games")) {
        Ok(files) if !files.is_empty() => files,
        Ok(_) => return fail("ordinary-C gate scanned no files"),
        Err(error) => return fail(error),
    };
    let mut findings = Vec::new();
    for path in &files {
        let text = match fs::read_to_string(path) {
            Ok(text) => text,
            Err(error) => return fail(error),
        };
        let name = path.strip_prefix(root).unwrap_or(path).to_string_lossy();
        findings.extend(find_forbidden(&name, &text));
        findings.extend(find_named_source_tool_leaks(&name, &text));
    }
    let (expanded, jobs, mut more) = match scan_preprocessed(root, target_ids) {
        Ok(result) => result,
        Err(error) => return fail(error),
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
