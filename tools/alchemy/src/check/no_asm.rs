
#[cfg(test)]
use crate::compiler::no_asm::self_test;
use crate::compiler::no_asm::{
    find_forbidden, find_named_source_tool_leaks, find_preprocessed, source_files, Finding,
};
use crate::compiler::routing::root as compiler_root;
use crate::targets::{target_for, DecompTarget, DecompTargetId, TARGET_IDS};
use std::collections::BTreeMap;
use std::fs;
use std::path::Path;
use std::process::{Command, ExitCode};
use std::sync::Mutex;

const USAGE: &str =
    "usage: alchemy check no-asm [--target TARGET]...\n\nScan raw and preprocessed C for instruction, register, and ABI escape hatches.";
type Job = (String, Vec<String>);
type Group = ((String, Vec<String>), Vec<String>);

struct MessageImports {
    _directory: tempfile::TempDir,
    flag: String,
    signature: String,
}

fn message_imports(
    root: &Path,
    target_ids: &[DecompTargetId],
) -> Result<BTreeMap<String, MessageImports>, String> {
    let mut imports = BTreeMap::new();
    for &id in target_ids {
        if let Some(directory) = crate::build_text::fresh_c_imports(root, target_for(id))? {
            let header = fs::read(directory.path().join(crate::build_text::C_INCLUDE))
                .map_err(|error| error.to_string())?;
            imports.insert(
                id.as_str().into(),
                MessageImports {
                    flag: format!("-I{}", directory.path().display()),
                    signature: crate::compiler::sha256::hex(&header),
                    _directory: directory,
                },
            );
        }
    }
    Ok(imports)
}

fn sibling(root: &Path, source: &str) -> Option<std::path::PathBuf> {
    source.ends_with(".c")
        .then(|| root.join(source).with_extension("s"))
}

fn prefix(target: DecompTarget, source: &str) -> Result<Vec<String>, String> {
    crate::compiler::preprocess::command(target, source, false)
}

fn groups(
    root: &Path,
    target_ids: &[DecompTargetId],
    imports: &BTreeMap<String, MessageImports>,
) -> Result<Vec<Group>, String> {
    let mut groups = BTreeMap::<(String, Vec<String>), Vec<String>>::new();
    let mut inventories = BTreeMap::new();
    for &id in target_ids {
        let target = target_for(id);
        for directory in [target.source_dir, "games/common/src"] {
            if !inventories.contains_key(directory) {
                let sources = source_files(&root.join(directory)).map_err(|error| error.to_string())?;
                inventories.insert(directory, sources);
            }
        }
        for path in inventories[target.source_dir].iter().chain(&inventories["games/common/src"]) {
            if !path
                .extension()
                .and_then(|ext| ext.to_str())
                .is_some_and(|ext| ext.eq_ignore_ascii_case("c"))
            {
                continue;
            }
            let source = path
                .strip_prefix(root)
                .unwrap_or(path)
                .to_string_lossy()
                .into_owned();
            let mut command = prefix(target, &source)?;
            if let Some(metadata) = imports.get(id.as_str()) {
                command.insert(1, metadata.flag.clone());
            }
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

fn scanner() -> &'static str {
    static DIGEST: std::sync::OnceLock<String> = std::sync::OnceLock::new();
    DIGEST.get_or_init(|| {
        crate::compiler::sha256::hex(
            ([
                include_str!("no_asm.rs"),
                include_str!("../compiler/no_asm.rs"),
                include_str!("../compiler/steering.rs"),
                include_str!("../compiler/c_syntax.rs"),
                include_str!("../evidence.rs"),
            ]
            .concat()
            + &crate::compiler::no_asm::reviewed_helpers_signature()
            + &crate::evidence::signature()).as_bytes(),
        )
    })
}

fn clean_key(
    inputs: &crate::compiler::source_inputs::SourceInputCache,
    root: &Path,
    label: &str,
    prefix: &[String],
    source: &str,
    imports: Option<&MessageImports>,
) -> Option<String> {
    let tree = inputs.signature(
        root,
        Path::new(source),
        &[prefix.to_vec()],
    )
    .ok()?;
    let command = prefix
        .iter()
        .map(|part| {
            if imports.is_some_and(|metadata| part == &metadata.flag) {
                "-I<current-message-catalog>".into()
            } else {
                part.clone()
            }
        })
        .collect::<Vec<_>>();
    let identity = format!(
        "{:?}",
        (
            "no-asm-clean-v4-scanner-source",
            scanner(),
            crate::compiler::bundle::compiler_bundle_signature(),
            label,
            crate::compiler::source_inputs::portable_commands(root, &[command]),
            imports.map(|metadata| metadata.signature.as_str()),
            source,
            crate::compiler::sha256::hex(&tree),
        )
    );
    Some(crate::compiler::sha256::hex(identity.as_bytes()))
}

fn jobs(
    root: &Path,
    groups: Vec<Group>,
    cache: Option<&crate::cache::Cache>,
    imports: &BTreeMap<String, MessageImports>,
) -> (Vec<(Job, Vec<String>)>, usize) {
    let inputs = groups.iter().map(|(_, sources)| sources.len()).sum();
    let source_inputs = crate::compiler::source_inputs::SourceInputCache::default();
    let mut jobs = Vec::new();
    for ((label, prefix), sources) in groups {
        let workers = std::thread::available_parallelism().map_or(1, |count| count.get().min(16));
        let chunk = sources.len().div_ceil(workers).max(1);
        let keys = std::thread::scope(|scope| {
            let handles = sources
                .chunks(chunk)
                .map(|part| {
                    let (label, prefix) = (&label, &prefix);
                    let source_inputs = &source_inputs;
                    let metadata = imports.get(label);
                    scope.spawn(move || {
                        part.iter()
                            .map(|source| clean_key(source_inputs, root, label, prefix, source, metadata))
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
    let mut findings = find_preprocessed(&job.0, &text);
    let mut command = job.1.clone();
    command.push("-C".into());
    let output = Command::new(&command[0])
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| error.to_string())?;
    if !output.status.success() {
        return Err(format!(
            "{} comment preprocessing failed: {}",
            job.0,
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    let comments = String::from_utf8_lossy(&output.stdout);
    findings.extend(
        crate::compiler::steering::forwarding_findings(&comments, &job.0)?
            .into_iter()
            .map(|item| Finding {
                file: item.file,
                line: item.line,
                token: format!(
                    "forwarding static inline {} needs FAKEMATCH inside its body",
                    item.name
                ),
            }),
    );
    Ok(findings)
}

fn scan_preprocessed(
    root: &Path,
    target_ids: &[DecompTargetId],
) -> Result<(usize, usize, Vec<Finding>), String> {
    let cache = crate::cache::Cache::open(&root.join("out/cache/no-asm.tsv")).ok();
    let imports = message_imports(root, target_ids)?;
    let (jobs, inputs) = jobs(
        root,
        groups(root, target_ids, &imports)?,
        cache.as_ref(),
        &imports,
    );
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

#[cfg(test)]
fn macro_self_test() -> Result<(), String> {
    let root = compiler_root();
    let directory = tempfile::tempdir().map_err(|error| error.to_string())?;
    let source = directory.path().join("fixture.c");
    let text = "#if __GNUC__ == 2\n#define ABI_KIND naked\n#else\n#define ABI_KIND packed\n#endif\nvoid f(void) __attribute__((ABI_KIND));\n";
    fs::write(&source, text).map_err(|error| error.to_string())?;
    if !find_forbidden("fixture.c", text).is_empty() {
        return Err("macro fixture did not evade the raw scan".into());
    }
    let target = target_for(DecompTargetId::TbsEn);
    let human = Path::new("games/tbs/src/system/memory/clear_word_if_set.c");
    if sibling(root, &human.to_string_lossy()) != Some(root.join(human).with_extension("s")) {
        return Err("relative sibling path did not resolve under repository root".into());
    }
    let mut command = prefix(target, &human.to_string_lossy())?;
    command.push(source.to_string_lossy().into_owned());
    let found = run(root, &("macro-regression".into(), command))?;
    if found.len() != 1 || !found[0].token.contains("naked") {
        return Err("production compiler route missed macro-expanded naked ABI".into());
    }
    Ok(())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    if arguments.is_empty() {
        return scan_repository(&TARGET_IDS);
    }
    let mut ids = Vec::new();
    for pair in arguments.chunks(2) {
        let [option, value] = pair else {
            return fail(USAGE);
        };
        let Some(id) = TARGET_IDS.iter().copied().find(|id| id.as_str() == value) else {
            return fail(format!("unknown target: {value}"));
        };
        if option != "--target" {
            return fail(USAGE);
        }
        ids.push(id);
    }
    scan_repository(&ids)
}

fn fail(message: impl std::fmt::Display) -> ExitCode {
    eprintln!("error: {message}");
    ExitCode::FAILURE
}

fn raw_findings(root: &Path, path: &Path) -> Result<Vec<Finding>, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let name = path.strip_prefix(root).unwrap_or(path).to_string_lossy();
    let mut findings = find_forbidden(&name, &text);
    findings.extend(find_named_source_tool_leaks(&name, &text));
    if name.starts_with("games/") {
        findings.extend(
            crate::compiler::steering::forwarding_findings(&text, &name)?
                .into_iter()
                .map(|item| Finding {
                    file: item.file,
                    line: item.line,
                    token: format!(
                        "forwarding static inline {} needs FAKEMATCH inside its body",
                        item.name
                    ),
                }),
        );
    }
    Ok(findings)
}

fn scan_repository(target_ids: &[DecompTargetId]) -> ExitCode {
    let root = compiler_root();
    let files = match ["games", "recon"]
        .into_iter()
        .map(|tree| source_files(&root.join(tree)))
        .collect::<Result<Vec<_>, _>>()
    {
        Ok(trees) if trees.iter().any(|files| !files.is_empty()) => trees.concat(),
        Ok(_) => return fail("ordinary-C gate scanned no files"),
        Err(error) => return fail(error),
    };
    let workers = std::thread::available_parallelism().map_or(1, |count| count.get());
    let chunk = files.len().div_ceil(workers).max(1);
    let (raw, preprocessed) = std::thread::scope(|scope| {
        let preprocessed = scope.spawn(|| scan_preprocessed(root, target_ids));
        let raw = files
            .chunks(chunk)
            .map(|part| {
                scope.spawn(move || {
                    part.iter()
                        .map(|path| raw_findings(root, path))
                        .collect::<Result<Vec<_>, String>>()
                })
            })
            .collect::<Vec<_>>()
            .into_iter()
            .map(|handle| handle.join().expect("no-asm raw worker"))
            .collect::<Result<Vec<_>, String>>();
        (raw, preprocessed.join().expect("no-asm preprocessing"))
    });
    let mut findings = match raw {
        Ok(found) => found.into_iter().flatten().flatten().collect::<Vec<_>>(),
        Err(error) => return fail(error),
    };
    let (expanded, jobs, mut more) = match preprocessed {
        Ok(result) => result,
        Err(error) => return fail(error),
    };
    findings.append(&mut more);
    findings.sort();
    findings.dedup();
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

#[test]
fn expanded_typed_forwarders_cannot_be_suppressed_by_raw_definitions() {
    let directory = tempfile::tempdir().unwrap();
    let source = directory.path().join("fixture.c");
    let target = target_for(DecompTargetId::TbsEn);
    let text = "typedef int s32;\n#define MAKE(Name) static inline void Name(s32 v) { Target(v); }\nMAKE(Expanded)\nstatic inline void Raw(s32 v) { Target(v); }\n";
    fs::write(&source, text).unwrap();
    let mut command = prefix(target, source.to_str().unwrap()).unwrap();
    command.push(source.to_string_lossy().into_owned());
    let found = run(
        compiler_root(),
        &("forwarder-regression".into(), command.clone()),
    )
    .unwrap();
    assert_eq!(found.len(), 2);
    assert!(found.iter().any(|item| item.token.contains("Expanded")));
    assert!(found.iter().any(|item| item.token.contains("Raw")));
    fs::write(
        &source,
        text.replace(
            "{ Target(v); }",
            "{ /* FAKEMATCH: measured adapter. */ Target(v); }",
        ),
    )
    .unwrap();
    assert!(
        run(compiler_root(), &("forwarder-regression".into(), command))
            .unwrap()
            .is_empty()
    );
}

#[test]
fn shared_inventories_preserve_target_commands_imports_and_new_files() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let write = |name: &str| {
        let path = root.join(name);
        fs::create_dir_all(path.parent().unwrap()).unwrap();
        fs::write(path, "source\n").unwrap();
    };
    write("games/tbs/src/one.c");
    write("games/tla/src/two.c");
    write("games/common/src/nested/shared.c");
    write("games/common/src/header.h");
    let ids = [DecompTargetId::TbsJa, DecompTargetId::TbsEn, DecompTargetId::TlaJa, DecompTargetId::TlaEn, DecompTargetId::TbsEn];
    let mut imports = BTreeMap::new();
    let imported = tempfile::tempdir().unwrap();
    imports.insert("tbs-en".into(), MessageImports {
        flag: format!("-I{}", imported.path().display()),
        signature: "catalog".into(),
        _directory: imported,
    });
    let check = |extra: bool| {
        let grouped = groups(root, &ids, &imports).unwrap();
        let actual = grouped.iter().flat_map(|((label, command), sources)| {
            sources.iter().map(move |source| ((label.clone(), source.clone()), command.clone()))
        }).collect::<BTreeMap<_, _>>();
        let mut expected = BTreeMap::new();
        for id in &ids[..4] {
            let target = target_for(*id);
            let game_source = if target.source_dir.contains("/tbs/") { "games/tbs/src/one.c" } else { "games/tla/src/two.c" };
            let mut sources = vec![game_source, "games/common/src/nested/shared.c"];
            if extra { sources.push("games/common/src/new.c"); }
            for source in sources {
                let mut command = prefix(target, source).unwrap();
                if let Some(metadata) = imports.get(id.as_str()) {
                    command.insert(1, metadata.flag.clone());
                }
                expected.insert((id.as_str().to_string(), source.to_string()), command);
            }
        }
        assert_eq!(actual, expected);
    };
    check(false);
    write("games/common/src/new.c");
    check(true);
}

#[test]
fn batched_audit_uses_current_catalog_and_invalidates_clean_keys() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let id = DecompTargetId::TbsEn;
    let target = target_for(id);
    let catalog = root.join(format!("{}/text/en.po", target.game_dir()));
    fs::create_dir_all(catalog.parent().unwrap()).unwrap();
    let write_catalog = |number| {
        fs::write(
            &catalog,
            format!(
                "msgid \"\"\nmsgstr \"\"\n\"X-Alchemy-Target: tbs-en\\n\"\n\n\
                 msgctxt \"MsgImported\"\nmsgid \"{number:05}\"\nmsgstr \"one\"\n"
            ),
        )
        .unwrap();
    };
    write_catalog(7);
    crate::build_text::write_c_imports(
        &root.join(target.output_dir),
        "#error stale generated header\n",
    )
    .unwrap();
    let source = root.join("import.c");
    fs::write(
        &source,
        "#include \"text/msg_ids.h\"\nTEXT_MESSAGE_ENUM(MsgImported);\n\
         #if TEXT_MESSAGE_NUMBER_MsgImported == 8\n#define KIND naked\n\
         #else\n#define KIND packed\n#endif\n\
         void Imported(void) __attribute__((KIND));\n",
    )
    .unwrap();
    let plain = root.join("plain.c");
    fs::write(&plain, "void Plain(void) {}\n").unwrap();
    let first = message_imports(root, &[id]).unwrap();
    let command = |imports: &BTreeMap<String, MessageImports>| {
        let mut command = prefix(target, source.to_str().unwrap()).unwrap();
        command.insert(1, imports[id.as_str()].flag.clone());
        command
    };
    let key = |imports: &BTreeMap<String, MessageImports>, path: &Path| {
        clean_key(
            &crate::compiler::source_inputs::SourceInputCache::default(),
            root,
            id.as_str(),
            &command(imports),
            path.to_str().unwrap(),
            imports.get(id.as_str()),
        )
        .unwrap()
    };
    let mut batch = command(&first);
    batch.push(source.to_string_lossy().into_owned());
    assert!(run(root, &(id.as_str().into(), batch)).unwrap().is_empty());
    let same = message_imports(root, &[id]).unwrap();
    assert_eq!(key(&first, &plain), key(&same, &plain));
    write_catalog(8);
    let changed = message_imports(root, &[id]).unwrap();
    assert_ne!(key(&first, &plain), key(&changed, &plain));
    assert_ne!(key(&first, &source), key(&changed, &source));
    let mut batch = command(&changed);
    batch.push(source.to_string_lossy().into_owned());
    let found = run(root, &(id.as_str().into(), batch)).unwrap();
    assert_eq!(found.len(), 1);
    assert!(found[0].token.contains("naked"));
    assert_eq!(
        fs::read_to_string(
            root.join(target.output_dir)
                .join(crate::build_text::C_INCLUDE)
        )
        .unwrap(),
        "#error stale generated header\n"
    );
}
