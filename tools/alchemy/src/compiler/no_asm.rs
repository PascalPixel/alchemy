//! Ordinary-C policy shared by candidate and production verification.

use crate::compiler::plan::direct_preprocessor_command;
use regex::{Captures, Regex};
use sha2::{Digest, Sha256};
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::OnceLock;

// Reviewed shared machine-interface bodies are admitted only by exact token
// identity, including constraints, instructions, operands and clobbers.
// Compiler flags and ordinary caller C remain subject to the existing policy.
const DMA_HEADER: &str = "games/THE BROKEN SEAL/INCLUDE/DMA.H";
const DMA_SOURCE: &str = include_str!(concat!(
    env!("CARGO_MANIFEST_DIR"),
    "/../../games/THE BROKEN SEAL/INCLUDE/DMA.H"
));
const DMA_BODY_SHA256: &str = "85462569a1fe487dd3033778e787524ca91ddc9bff061d3a85cb78fdbaca736e";
const IWRAM_CALL_HEADER: &str = "games/THE BROKEN SEAL/INCLUDE/IWRAM_CALL.H";
const IWRAM_CALL_SOURCE: &str = include_str!(concat!(
    env!("CARGO_MANIFEST_DIR"),
    "/../../games/THE BROKEN SEAL/INCLUDE/IWRAM_CALL.H"
));
const IWRAM_CALL_BODY_SHA256: &str =
    "110abd5ad8313dcfe0dce7ed49bb246d04883fe6b9ad61f29bd290c7abc80882";

fn source_tokens(text: &str) -> Vec<regex::Match<'_>> {
    static TOKENS: OnceLock<Regex> = OnceLock::new();
    regex(&TOKENS, r#"(?ms)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[^\s]"#)
        .find_iter(text)
        .filter(|token| !token.as_str().starts_with("//") && !token.as_str().starts_with("/*"))
        .collect()
}

fn dma_body() -> Option<&'static [String]> {
    static BODY: OnceLock<Option<Vec<String>>> = OnceLock::new();
    BODY.get_or_init(|| {
        let start = DMA_SOURCE.find("static __inline__")?;
        let end = DMA_SOURCE.rfind('}')? + 1;
        let tokens: Vec<String> = source_tokens(&DMA_SOURCE[start..end])
            .iter()
            .map(|token| token.as_str().to_owned())
            .collect();
        let hash = format!("{:x}", Sha256::digest(tokens.join("\n").as_bytes()));
        (hash == DMA_BODY_SHA256).then_some(tokens)
    })
    .as_deref()
}

fn mask_dma_body(text: &str) -> String {
    let Some(body) = dma_body() else {
        return text.into();
    };
    let tokens = source_tokens(text);
    let mut bytes = text.as_bytes().to_vec();
    for window in tokens.windows(body.len()) {
        if window
            .iter()
            .zip(body)
            .all(|(token, expected)| token.as_str() == expected)
        {
            for byte in &mut bytes[window[0].start()..window[window.len() - 1].end()] {
                if *byte != b'\n' {
                    *byte = b' ';
                }
            }
        }
    }
    String::from_utf8(bytes).expect("only complete DMA token spans are masked")
}

fn iwram_call_body() -> Option<&'static [String]> {
    static BODY: OnceLock<Option<Vec<String>>> = OnceLock::new();
    BODY.get_or_init(|| {
        let start = IWRAM_CALL_SOURCE.find("static __inline__")?;
        let end = IWRAM_CALL_SOURCE.rfind('}')? + 1;
        let tokens: Vec<String> = source_tokens(&IWRAM_CALL_SOURCE[start..end])
            .iter()
            .map(|token| token.as_str().to_owned())
            .collect();
        let hash = format!("{:x}", Sha256::digest(tokens.join("\n").as_bytes()));
        (hash == IWRAM_CALL_BODY_SHA256).then_some(tokens)
    })
    .as_deref()
}

fn mask_iwram_call_body(text: &str) -> String {
    let Some(body) = iwram_call_body() else {
        return text.into();
    };
    let tokens = source_tokens(text);
    let mut bytes = text.as_bytes().to_vec();
    for window in tokens.windows(body.len()) {
        if window
            .iter()
            .zip(body)
            .all(|(token, expected)| token.as_str() == expected)
        {
            for byte in &mut bytes[window[0].start()..window[window.len() - 1].end()] {
                if *byte != b'\n' {
                    *byte = b' ';
                }
            }
        }
    }
    String::from_utf8(bytes).expect("only complete fixed-math token spans are masked")
}

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
    let text = if Path::new(file).ends_with(DMA_HEADER) {
        mask_dma_body(text)
    } else if Path::new(file).ends_with(IWRAM_CALL_HEADER) {
        mask_iwram_call_body(text)
    } else {
        text.into()
    };
    scan_forbidden(file, &text, false)
}

fn scan_forbidden(file: &str, text: &str, admit_dma: bool) -> Vec<Finding> {
    static TOKENS: OnceLock<Regex> = OnceLock::new();
    let text = if admit_dma {
        mask_dma_body(text)
    } else {
        text.into()
    };
    let code = code_only(&text);
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
    let path = file.replace('\\', "/").to_ascii_lowercase();
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
    let text = mask_iwram_call_body(&mask_dma_body(text));
    let mut findings = scan_forbidden(label, &text, false);
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
        } else if matches!(
            path.extension().and_then(|v| v.to_str()),
            Some("c" | "h" | "C" | "H")
        ) {
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
    Ok(find_preprocessed(&output.to_string_lossy(), &text)
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
    let found = find_named_source_tool_leaks(
        "games/THE BROKEN SEAL/SRC/graphics/window/example.c",
        source,
    );
    if found.len() != 1 || found[0].line != 2 || !found[0].token.contains("M2C_FIELD") {
        return Err("named-source gate missed a tool-branded identifier".into());
    }
    if !find_named_source_tool_leaks("games/THE BROKEN SEAL/SRC/unidentified/example.c", source)
        .is_empty()
    {
        return Err("named-source gate crossed its owned boundary".into());
    }
    if !find_named_source_tool_leaks("recon/tbs/example.c", source).is_empty() {
        return Err("named-source gate treated a reconstruction draft as production source".into());
    }
    if find_named_source_tool_leaks("games/THE LOST AGE/SRC/example.c", source).is_empty() {
        return Err("named-source gate did not apply outside tbs".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn reviewed_dma_is_admitted_only_as_the_complete_shared_body() {
        assert!(dma_body().is_some());
        assert!(find_forbidden(DMA_HEADER, DMA_SOURCE).is_empty());
        assert!(!find_forbidden("caller.c", DMA_SOURCE).is_empty());
        assert!(find_preprocessed("expanded", DMA_SOURCE).is_empty());
        for changed in [
            DMA_SOURCE.replace("stmia", "ldmia"),
            DMA_SOURCE.replace("r0", "r4"),
            DMA_SOURCE.replace("#12", "#8"),
            DMA_SOURCE.replace("\"memory\", \"cc\"", "\"memory\""),
            DMA_SOURCE.replace("register u32 src", "volatile register u32 src"),
        ] {
            assert!(!find_forbidden(DMA_HEADER, &changed).is_empty());
            assert!(!find_preprocessed("expanded", &changed).is_empty());
        }
    }

    #[test]
    fn dma_admission_does_not_hide_extra_assembly_or_abi_attributes() {
        let extra = format!(
            "{}\nvoid f(void) {{ __asm__(\"nop\"); }}\nvoid g(void) __attribute__((naked));",
            DMA_SOURCE
        );
        assert_eq!(find_forbidden(DMA_HEADER, &extra).len(), 2);
        assert_eq!(find_preprocessed("expanded", &extra).len(), 2);
        let standalone = "register unsigned int src __asm__(\"r0\");";
        assert_eq!(find_preprocessed("expanded", standalone).len(), 1);
    }

    #[test]
    fn reviewed_iwram_call_is_admitted_only_as_the_complete_shared_body() {
        assert!(iwram_call_body().is_some());
        assert!(find_forbidden(IWRAM_CALL_HEADER, IWRAM_CALL_SOURCE).is_empty());
        assert!(!find_forbidden("caller.c", IWRAM_CALL_SOURCE).is_empty());
        assert!(find_preprocessed("expanded", IWRAM_CALL_SOURCE).is_empty());
        for changed in [
            IWRAM_CALL_SOURCE.replace("mov ip, pc", "mov lr, pc"),
            IWRAM_CALL_SOURCE.replace("bx %2", "bx %1"),
            IWRAM_CALL_SOURCE.replace("\"r2\", \"ip\", \"cc\"", "\"r2\", \"ip\""),
        ] {
            assert!(!find_forbidden(IWRAM_CALL_HEADER, &changed).is_empty());
            assert!(!find_preprocessed("expanded", &changed).is_empty());
        }
    }

    #[test]
    fn raw_escape_hatches() {
        self_test().unwrap();
    }

    #[test]
    fn atlas_source_names_keep_tool_leak_checks() {
        let source = "void f(void) { M2C_ERROR(); }";
        let lower =
            find_named_source_tool_leaks("games/THE BROKEN SEAL/SRC/field/example.c", source);
        let upper =
            find_named_source_tool_leaks("games/THE BROKEN SEAL/SRC/FIELD/EXAMPLE.C", source);
        assert!(!lower.is_empty());
        assert_eq!(lower.len(), upper.len());
    }

    #[test]
    fn nonordinary_source_is_detected_raw() {
        let path = std::env::temp_dir().join("alchemy-nonordinary-raw.c");
        fs::write(&path, "void f(void) __attribute__((naked));\n").unwrap();
        assert!(!ordinary_source(&path, &path).unwrap());
        let _ = fs::remove_file(path);
    }
}
