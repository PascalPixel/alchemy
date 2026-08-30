pub mod cli;
use regex::{Captures, Regex};
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::sync::OnceLock;
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
        r#"(?ms)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"#,
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
    if !path.starts_with("games/gs1/src/") || path.starts_with("games/gs1/src/unidentified/") {
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
/// Preprocess one source with the routed direct preprocessor and report the
/// forbidden constructs its expansion contains, as `token:line:expanded`.
pub fn expanded_forbidden(root: &Path, source: &Path) -> Result<String, String> {
    let work = tempfile::tempdir().map_err(|error| error.to_string())?;
    let output = work.path().join("ordinary.i");
    let command = compiler_core::plan::direct_preprocessor_command(
        &source.to_string_lossy(),
        &output.to_string_lossy(),
    )?;
    let program = command
        .first()
        .ok_or_else(|| "empty preprocessor command".to_string())?;
    let status = std::process::Command::new(program)
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
/// True when the source is ordinary C both as written and after expansion.
pub fn ordinary_source(root: &Path, source: &Path) -> Result<bool, String> {
    let text = fs::read_to_string(source).map_err(|error| error.to_string())?;
    Ok(find_forbidden(&source.to_string_lossy(), &text).is_empty()
        && expanded_forbidden(root, source)?.is_empty())
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
    let source = "# 8 \"one.c\"\nvoid a(void) __attribute__((naked));\n# 12 \"two.c\"\nvoid b(void) __attribute__((naked));\n";
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
    if !find_named_source_tool_leaks("games/gs1/src/unidentified/example.c", source).is_empty()
        || !find_named_source_tool_leaks("games/gs2/src/example.c", source).is_empty()
    {
        return Err("named-source gate crossed its owned boundary".into());
    }
    Ok(())
}
#[cfg(test)]
#[test]
fn raw_and_macro_escape_hatches() {
    self_test().unwrap();
    cli::macro_self_test().unwrap();
}
#[cfg(test)]
#[test]
fn nonordinary_source_is_detected_raw() {
    let path = std::env::temp_dir().join("alchemy-nonordinary-raw.c");
    fs::write(&path, "void f(void) __attribute__((naked));\n").unwrap();
    assert!(!ordinary_source(&path, &path).unwrap());
    let _ = fs::remove_file(path);
}
