//! Ordinary-C policy and support utilities.
//!
//! This module centralizes the checker logic in `compiler-core` so callers can
//! invoke one policy surface through `alchemy check no-asm`.

use crate::decomp_targets::{
    target_for, DecompCompilerTarget, DecompTarget, DecompTargetId, TARGET_IDS,
};
use crate::plan::direct_preprocessor_command;
use crate::routing::{
    cflags_for_target_source, root as compiler_root, uses_agbcc_compiler, CompilerTarget,
};
use crate::source_paths::SourcePaths;
use crate::translation_units::TranslationUnits;
use regex::{Captures, Regex};
use std::collections::BTreeMap;
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::{Command, ExitCode};
use std::sync::{Mutex, OnceLock};

const ABI: &str = "naked interrupt interrupt_handler isr long_call short_call pcs target target_clones regparm stdcall fastcall";

#[derive(Debug, Clone, PartialEq, Eq, PartialOrd, Ord)]
pub struct Finding {
    pub file: String,
    pub line: usize,
    pub token: String,
}

fn regex(slot: &'static OnceLock<Regex>, pattern: &str) -> &'static Regex {
    slot.get_or_init(|| Regex::new(pattern).unwrap())
}

fn code_only(text: &str) -> String {
    static LITERALS: OnceLock<Regex> = OnceLock::new();
    regex(
        &LITERALS,
        r#"(?ms)//[^\n]*|/\*.*?\*/|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'"#,
    )
    .replace_all(text, |capture: &Captures| {
        capture[0]
            .chars()
            .map(|character| if character == '\n' { '\n' } else { ' ' })
            .collect::<String>()
    })
    .into_owned()
}

fn forbidden(word: &str, attribute: bool) -> Option<String> {
    let assembly = word == "asm"
        || word
            .strip_prefix("__asm")
            .is_some_and(|w| w.bytes().all(|byte| byte == b'_'));
    if assembly {
        return Some(word.into());
    }
    let plain = word
        .strip_prefix("__")
        .and_then(|w| w.strip_suffix("__"))
        .unwrap_or(word);
    (attribute && ABI.split_ascii_whitespace().any(|item| item == plain))
        .then(|| format!("ABI attribute {word}"))
}

pub fn find_forbidden(file: &str, text: &str) -> Vec<Finding> {
    static TOKENS: OnceLock<Regex> = OnceLock::new();
    let code = code_only(text);
    let mut findings = Vec::new();
    let (mut depth, mut pending, mut line, mut end) = (0usize, false, 1usize, 0usize);
    for matched in regex(&TOKENS, r"[A-Za-z_][A-Za-z0-9_]*|[()]").find_iter(&code) {
        let gap = &code[end..matched.start()];
        line += gap.bytes().filter(|byte| *byte == b'\n').count();
        let token = matched.as_str();
        pending = pending && (gap.trim().is_empty() || token == "(");
        match token {
            "__attribute" | "__attribute__" | "__declspec" => pending = true,
            "(" => {
                depth += usize::from(pending || depth > 0);
                pending = false;
            }
            ")" => depth = depth.saturating_sub(1),
            word => {
                if let Some(token) = forbidden(word, depth > 0) {
                    findings.push(Finding {
                        file: file.into(),
                        line,
                        token,
                    });
                }
                pending = false;
            }
        }
        end = matched.end();
    }
    findings
}

pub fn find_named_source_tool_leaks(file: &str, text: &str) -> Vec<Finding> {
    static TOOL_NAMES: OnceLock<Regex> = OnceLock::new();
    let path = file.replace('\\', "/");
    let is_game_source = path.starts_with("games/")
        && path.ends_with(".c")
        && path.contains("/src/")
        && !path.contains("/unidentified/");
    if !is_game_source {
        return Vec::new();
    }
    let code = code_only(text);
    regex(&TOOL_NAMES, r"\bM2C_[A-Za-z0-9_]*\b")
        .find_iter(&code)
        .map(|matched| Finding {
            file: file.into(),
            line: code[..matched.start()]
                .bytes()
                .filter(|byte| *byte == b'\n')
                .count()
                + 1,
            token: format!("tool identifier {}", matched.as_str()),
        })
        .collect()
}

pub fn find_preprocessed(label: &str, text: &str) -> Vec<Finding> {
    let mut findings = find_forbidden(label, text);
    for item in &mut findings {
        let marker = text
            .lines()
            .enumerate()
            .take(item.line)
            .filter_map(|(row, text)| {
                let (line, file) = text.strip_prefix("# ")?.split_once(" \"")?;
                Some((
                    row + 1,
                    line.parse::<usize>().ok()?,
                    file.split('"').next()?,
                ))
            })
            .last();
        if let Some((physical, logical, file)) = marker {
            item.file = file.into();
            item.line = logical + item.line - physical - 1;
        }
    }
    findings
}

pub fn source_files(directory: &Path) -> io::Result<Vec<PathBuf>> {
    if !directory.is_dir() {
        return Ok(Vec::new());
    }
    let mut files = Vec::new();
    for entry in fs::read_dir(directory)? {
        let path = entry?.path();
        if path.is_dir() {
            files.extend(source_files(&path)?);
        } else if matches!(path.extension().and_then(|v| v.to_str()), Some("c" | "h")) {
            files.push(path);
        }
    }
    files.sort();
    Ok(files)
}

/// Preprocess one source and report forbidden constructs from its expansion as
/// `token:line:expanded`.
pub fn expanded_forbidden(root: &Path, source: &Path) -> Result<String, String> {
    let work = tempfile::tempdir().map_err(|error| error.to_string())?;
    let output = work.path().join("ordinary.i");
    let command =
        direct_preprocessor_command(&source.to_string_lossy(), &output.to_string_lossy())?;
    let program = command
        .first()
        .ok_or_else(|| "empty preprocessor command".to_string())?;
    let status = Command::new(program)
        .args(&command[1..])
        .current_dir(root)
        .output()
        .map_err(|error| format!("{program}: {error}"))?;
    if !status.status.success() {
        let detail = String::from_utf8_lossy(&status.stderr);
        return Err(format!("{program} failed: {}", detail.trim()));
    }
    let text = fs::read_to_string(&output).map_err(|error| error.to_string())?;
    Ok(find_forbidden(&output.to_string_lossy(), &text)
        .into_iter()
        .map(|finding| format!("{}:{}:expanded", finding.token, finding.line))
        .collect::<Vec<_>>()
        .join(","))
}

/// True when the source is ordinary C both raw and preprocessed.
pub fn ordinary_source(root: &Path, source: &Path) -> Result<bool, String> {
    let text = fs::read_to_string(source).map_err(|error| error.to_string())?;
    Ok(find_forbidden(&source.to_string_lossy(), &text).is_empty()
        && expanded_forbidden(root, source)?.is_empty())
}

const USAGE: &str =
    "usage: alchemy check no-asm [--target TARGET|--self-test]\n\nScan raw and preprocessed C for instruction, register, and ABI escape hatches.";
type Job = (String, Vec<String>);

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
    crate::bundle::compiler_command_for_target(compiler, &flags)
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
    let directory = tempfile::tempdir().map_err(|error| error.to_string())?;
    let source = directory.path().join("fixture.c");
    let text = "#if __GNUC_MINOR__ == 9\n#define ABI_KIND naked\n#else\n#define ABI_KIND packed\n#endif\nvoid f(void) __attribute__((ABI_KIND));\n";
    fs::write(&source, text).map_err(|error| error.to_string())?;
    if !find_forbidden("fixture.c", text).is_empty() {
        return Err("macro fixture did not evade the raw scan".into());
    }
    let target = target_for(DecompTargetId::Gs1En);
    let paths = SourcePaths::load_for_game(root, "gs1")?;
    let registered = paths
        .all_sources()?
        .into_iter()
        .find(|source| {
            uses_agbcc_compiler(
                CompilerTarget::Gs1,
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
    let routing = registered.owner.routing_path_for_game("gs1");
    let mut command = prefix(target, &routing.to_string_lossy())?;
    command.push(source.to_string_lossy().into_owned());
    let found = run(root, &("macro-regression".into(), command))?;
    if found.len() != 1 || !found[0].token.contains("naked") {
        return Err("production AGBCC route missed macro-expanded naked ABI".into());
    }
    Ok(())
}

pub fn self_test() -> Result<(), String> {
    let source = "register int r __asm__(\"r4\"); void f(void) { __asm__(\"nop\"); __asm__ volatile(\"\" ::: \"memory\"); }\n";
    let found = find_forbidden("fixture.c", source);
    if found.len() != 3 || found.iter().any(|item| item.token != "__asm__") {
        return Err("raw scan missed an assembly escape hatch".into());
    }
    let source =
        "void f(void) __attribute__((__naked__)); struct S { int x; } __attribute__((packed));\n";
    let found = find_forbidden("fixture.c", source);
    if found.len() != 1 || found[0].token != "ABI attribute __naked__" {
        return Err("raw scan missed a forbidden ABI attribute".into());
    }
    let source =
        "# 8 \"one.c\"\nvoid a(void) __attribute__((naked));\n# 12 \"two.c\"\nvoid b(void) __attribute__((naked));\n";
    let found = find_preprocessed("batch", source);
    let locations = found
        .iter()
        .map(|f| (f.file.as_str(), f.line))
        .collect::<Vec<_>>();
    if locations != [("one.c", 8), ("two.c", 12)] {
        return Err("preprocessed findings lost source identity".into());
    }
    let source = "/* M2C_FIELD */\n#define M2C_FIELD(x) (x)\n";
    let found = find_named_source_tool_leaks("games/gs1/src/ui/example.c", source);
    if found.len() != 1 || found[0].line != 2 || !found[0].token.contains("M2C_FIELD") {
        return Err("named-source gate missed a tool-branded identifier".into());
    }
    if !find_named_source_tool_leaks("games/gs1/src/unidentified/example.c", source).is_empty() {
        return Err("named-source gate crossed its owned boundary".into());
    }
    if !find_named_source_tool_leaks("games/gs1/recon/example.c", source).is_empty() {
        return Err("named-source gate treated a reconstruction draft as production source".into());
    }
    if find_named_source_tool_leaks("games/gs2/src/example.c", source).is_empty() {
        return Err("named-source gate did not apply outside gs1".into());
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
    let files = match source_files(&root.join("games")) {
        Ok(files) if !files.is_empty() => files,
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn raw_and_macro_escape_hatches() {
        self_test().unwrap();
        macro_self_test().unwrap();
    }

    #[test]
    fn nonordinary_source_is_detected_raw() {
        let path = std::env::temp_dir().join("alchemy-nonordinary-raw.c");
        fs::write(&path, "void f(void) __attribute__((naked));\n").unwrap();
        assert!(!ordinary_source(&path, &path).unwrap());
        let _ = fs::remove_file(path);
    }
}
