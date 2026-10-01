//! Golden Sun repository scan orchestration; lexical policy lives in compiler::no_asm.

use crate::compiler::no_asm::{
    find_forbidden, find_named_source_tool_leaks, find_preprocessed, self_test, source_files,
    Finding,
};
use crate::compiler::routing::root as compiler_root;
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

/// Keep fresh catalog metadata alive for every batch in an edition.
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
    (source.ends_with(".c") || source.ends_with(".C"))
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
    for &id in target_ids {
        let target = target_for(id);
        let mut sources =
            source_files(&root.join(target.source_dir)).map_err(|error| error.to_string())?;
        sources.extend(
            source_files(&root.join("games/COMMON/SRC")).map_err(|error| error.to_string())?,
        );
        for path in sources {
            if !path
                .extension()
                .and_then(|ext| ext.to_str())
                .is_some_and(|ext| ext.eq_ignore_ascii_case("c"))
            {
                continue;
            }
            let source = path
                .strip_prefix(root)
                .unwrap_or(&path)
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

/// The identity of one source's expansion: the command, the source with
/// every header it includes, the compiler bundle and the scanner. A source
/// whose expansion was clean under the same identity is not expanded again,
/// as make skips an up-to-date object; any change to it or a header it
/// includes scans it afresh.
fn clean_key(
    root: &Path,
    label: &str,
    prefix: &[String],
    source: &str,
    imports: Option<&MessageImports>,
) -> Option<String> {
    let tree = crate::compiler::source_inputs::compiler_source_tree_signature(
        root,
        Path::new(source),
        &[prefix.to_vec()],
    )
    .ok()?;
    // A temporary include directory is not a compiler option. Its current
    // catalog contents are part of every key, even when this source does not
    // import any messages. Imported headers also stay in the source tree
    // signature; their temporary path may cause a safe extra expansion.
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
            "no-asm-clean-v3-message-imports",
            crate::compiler::bundle::executable_signature().ok()?,
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

/// Preprocessing batches for every source not already known clean, with
/// the clean keys each batch would record, and the number of inputs.
fn jobs(
    root: &Path,
    groups: Vec<Group>,
    cache: Option<&psynergy::cache::SqliteCache>,
    imports: &BTreeMap<String, MessageImports>,
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
                    let metadata = imports.get(label);
                    scope.spawn(move || {
                        part.iter()
                            .map(|source| clean_key(root, label, prefix, source, metadata))
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
    // GCC 2.96 -C preserves macro-body tags but its logical line spacing can
    // drift through multiline comments. Keep the existing plain expansion
    // for assembly admission and use the comment expansion for helper bodies.
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
    let mut raw_definitions = BTreeMap::new();
    findings.extend(
        crate::compiler::steering::forwarding_findings(&comments, &job.0)?
            .into_iter()
            .map(|mut item| {
                let lines = raw_definitions.entry(item.file.clone()).or_insert_with(|| {
                    fs::read_to_string(root.join(&item.file))
                        .ok()
                        .and_then(|source| {
                            let tokens = crate::permute::lex::lex(&source).ok()?;
                            Some(
                                crate::permute::parse::scan_definitions(&tokens)
                                    .into_iter()
                                    .map(|(name, _, at, _, _)| (name, tokens[at].line))
                                    .collect::<BTreeMap<_, _>>(),
                            )
                        })
                        .unwrap_or_default()
                });
                // Diagnostic mapping never admits a helper or suppresses a finding.
                if let Some(line) = lines.get(&item.name) {
                    item.line = *line;
                }
                Finding {
                    file: item.file,
                    line: item.line,
                    token: format!(
                        "forwarding static inline {} needs FAKEMATCH inside its body",
                        item.name
                    ),
                }
            }),
    );
    Ok(findings)
}

fn scan_preprocessed(
    root: &Path,
    target_ids: &[DecompTargetId],
) -> Result<(usize, usize, Vec<Finding>), String> {
    let cache = psynergy::cache::SqliteCache::open(&root.join("out/cache/no-asm.sqlite3")).ok();
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
    let text = "#if __GNUC__ == 2\n#define ABI_KIND naked\n#else\n#define ABI_KIND packed\n#endif\nvoid f(void) __attribute__((ABI_KIND));\n";
    fs::write(&source, text).map_err(|error| error.to_string())?;
    if !find_forbidden("fixture.c", text).is_empty() {
        return Err("macro fixture did not evade the raw scan".into());
    }
    let target = target_for(DecompTargetId::TbsEn);
    let human = Path::new("games/THE BROKEN SEAL/SRC/SYSTEM/MEMORY/CLEAR_WORD_IF_SET.C");
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
        if name.starts_with("games/") {
            match crate::compiler::steering::forwarding_findings(&text, &name) {
                Ok(items) => findings.extend(items.into_iter().map(|item| Finding {
                    file: item.file,
                    line: item.line,
                    token: format!(
                        "forwarding static inline {} needs FAKEMATCH inside its body",
                        item.name
                    ),
                })),
                Err(error) => return fail(error),
            }
        }
    }
    let (expanded, jobs, mut more) = match scan_preprocessed(root, target_ids) {
        Ok(result) => result,
        Err(error) => {
            eprintln!("error: {error}");
            return ExitCode::FAILURE;
        }
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
fn batched_audit_uses_current_catalog_and_invalidates_clean_keys() {
    let directory = tempfile::tempdir().unwrap();
    let root = directory.path();
    let id = DecompTargetId::TbsEn;
    let target = target_for(id);
    let catalog = root.join(format!("{}/TEXT/EN.PO", target.game_dir()));
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
    let source = root.join("IMPORT.C");
    fs::write(
        &source,
        "#include \"text/MSG_IDS.H\"\nTEXT_MESSAGE_ENUM(MsgImported);\n\
         #if TEXT_MESSAGE_NUMBER_MsgImported == 8\n#define KIND naked\n\
         #else\n#define KIND packed\n#endif\n\
         void Imported(void) __attribute__((KIND));\n",
    )
    .unwrap();
    let plain = root.join("PLAIN.C");
    fs::write(&plain, "void Plain(void) {}\n").unwrap();
    let first = message_imports(root, &[id]).unwrap();
    let command = |imports: &BTreeMap<String, MessageImports>| {
        let mut command = prefix(target, source.to_str().unwrap()).unwrap();
        command.insert(1, imports[id.as_str()].flag.clone());
        command
    };
    let key = |imports: &BTreeMap<String, MessageImports>, path: &Path| {
        clean_key(
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
