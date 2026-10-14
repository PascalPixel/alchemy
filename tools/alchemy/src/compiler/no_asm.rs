
use regex::{Captures, Regex};
use sha2::{Digest, Sha256};
use std::fs;
use std::io;
use std::path::{Path, PathBuf};
use std::sync::OnceLock;

const DMA_HEADER: &str = "games/common/include/system/dma.h";
const DMA_BODY_SHA256: &str = "85462569a1fe487dd3033778e787524ca91ddc9bff061d3a85cb78fdbaca736e";
const IWRAM_CALL_HEADER: &str = "games/tbs/include/iwram_call.h";
const IWRAM_CALL_BODY_SHA256: &str =
    "110abd5ad8313dcfe0dce7ed49bb246d04883fe6b9ad61f29bd290c7abc80882";

fn source_tokens(text: &str) -> Vec<regex::Match<'_>> {
    static TOKENS: OnceLock<Regex> = OnceLock::new();
    regex(&TOKENS, r#"(?ms)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[^\s]"#)
        .find_iter(text)
        .filter(|token| !token.as_str().starts_with("//") && !token.as_str().starts_with("/*"))
        .collect()
}

fn reviewed_body(source: &str, digest: &str) -> Option<Vec<String>> {
    let start = source.find("static __inline__")?;
    let end = source.rfind('}')? + 1;
    let tokens: Vec<String> = source_tokens(&source[start..end])
        .iter()
        .map(|token| token.as_str().to_owned())
        .collect();
    let hash = format!("{:x}", Sha256::digest(tokens.join("\n").as_bytes()));
    (hash == digest).then_some(tokens)
}

fn mask_body(text: &str, body: Option<&[String]>) -> String {
    let Some(body) = body else {
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
    String::from_utf8(bytes).expect("only complete reviewed token spans are masked")
}

fn dma_body() -> Option<&'static [String]> {
    static BODY: OnceLock<Option<Vec<String>>> = OnceLock::new();
    BODY.get_or_init(|| reviewed_file_body(super::routing::root(), DMA_HEADER, DMA_BODY_SHA256))
        .as_deref()
}

fn iwram_call_body() -> Option<&'static [String]> {
    static BODY: OnceLock<Option<Vec<String>>> = OnceLock::new();
    BODY.get_or_init(|| reviewed_file_body(super::routing::root(), IWRAM_CALL_HEADER, IWRAM_CALL_BODY_SHA256))
        .as_deref()
}

fn reviewed_file_body(root: &Path, header: &str, digest: &str) -> Option<Vec<String>> {
    let source = fs::read_to_string(root.join(header)).ok()?;
    reviewed_body(&source, digest)
}

/// Cache identity follows which reviewed bodies this invocation can admit.
/// Missing or edited headers disable their exemptions without rebuilding the
/// checker, and must invalidate earlier clean results for copied helper bodies.
pub(crate) fn reviewed_helpers_signature() -> String {
    helpers_signature(dma_body(), iwram_call_body())
}

fn helpers_signature(dma: Option<&[String]>, iwram: Option<&[String]>) -> String {
    format!("{:x}", Sha256::digest(format!("{dma:?}{iwram:?}").as_bytes()))
}

fn mask_dma_body(text: &str) -> String {
    mask_body(text, dma_body())
}

fn mask_iwram_call_body(text: &str) -> String {
    mask_body(text, iwram_call_body())
}

/// Blanks the branches only the modern build compiles (`#ifdef MODERN`, `#if
/// MODERN`, `#if defined(MODERN)` and the `#else` of `#ifndef MODERN`): no
/// matching build sees them, and pret's MODERN guards are not devices.
fn mask_modern(text: &str) -> String {
    let mut kept = String::with_capacity(text.len());
    // Per open conditional: whether it is a MODERN test, whether its current
    // branch is modern-only, and whether this level masks.
    let mut open: Vec<(bool, bool)> = Vec::new();
    for line in text.split_inclusive('\n') {
        let directive = line.trim_start().strip_prefix('#').map(str::trim_start);
        let word = directive.map(|rest| rest.split(|c: char| !c.is_ascii_alphanumeric() && c != '_').next().unwrap_or(""));
        let rest = directive.map(|rest| rest[word.unwrap_or("").len()..].trim());
        let masked_before = open.iter().any(|(_, masks)| *masks);
        match word {
            Some("if" | "ifdef" | "ifndef") => {
                let test = rest.unwrap_or("");
                let modern = matches!(test, "MODERN" | "defined(MODERN)" | "defined MODERN");
                let masks = modern && word != Some("ifndef");
                open.push((modern, masks));
            }
            Some("else" | "elif") => {
                if let Some((modern, masks)) = open.last_mut() {
                    if *modern {
                        *masks = !*masks;
                    }
                }
            }
            Some("endif") => {
                open.pop();
            }
            _ => {}
        }
        let masked = masked_before || open.iter().any(|(_, masks)| *masks);
        if masked {
            kept.extend(line.chars().map(|c| if c == '\n' { c } else { ' ' }));
        } else {
            kept.push_str(line);
        }
    }
    kept
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

pub fn camelot_admitted(text: &str, line: usize) -> bool {
    if crate::evidence::approved_spans(text).iter().any(|span| {
        let first = text[..span.start].bytes().filter(|b| *b == b'\n').count() + 1;
        let last = text[..span.end].bytes().filter(|b| *b == b'\n').count() + 1;
        (first..=last).contains(&line)
    }) {
        // Findings carry source lines. An approved statement must not exempt
        // an unrelated instruction placed later on that same line.
        let masked = crate::evidence::mask_inline(text);
        if let Some(body) = line.checked_sub(1).and_then(|row| masked.lines().nth(row)) {
            if scan_forbidden("review scope", body, false).is_empty() { return true; }
        }
    }
    static TAG: OnceLock<Regex> = OnceLock::new();
    let tag = regex(&TAG, r#"/\*\s*FAKEMATCH:\s*[^\s*]"#);
    let lines: Vec<&str> = text.lines().collect();
    let adjacent = [line.checked_sub(1), line.checked_sub(2)]
        .into_iter()
        .flatten()
        .filter_map(|row| lines.get(row))
        .any(|text| tag.is_match(text));
    if adjacent {
        return true;
    }

    // A formatter may wrap the comma-separated registers of one tagged
    // declaration. Admit its continuation, never the following statement.
    let Some(row) = line.checked_sub(1) else { return false };
    let Some(current) = lines.get(row) else { return false };
    let start = text.split_inclusive('\n').take(row).map(str::len).sum::<usize>();
    let end = start + current.len();
    if end > text.len() {
        return false;
    }
    let boundary = source_tokens(&text[..start]).into_iter().rev()
        .find(|token| matches!(token.as_str(), ";" | "{" | "}"))
        .map_or(0, |token| token.end());
    let declaration = &text[boundary..end];
    let Some(register) = source_tokens(declaration).into_iter().find(|token| token.as_str() == "register") else { return false };
    let Some(proof) = tag.find_iter(&declaration[..register.start()]).last() else { return false };
    let proof_end = if proof.as_str().starts_with("/*") {
        declaration[proof.start()..register.start()].find("*/").map(|at| proof.start() + at + 2)
    } else {
        Some(proof.end())
    };
    let Some(proof_end) = proof_end else { return false };
    if !declaration[proof_end..register.start()].chars().all(|c| c.is_whitespace() || c == '\\') {
        return false;
    }
    let body = code_only(&declaration[register.start()..]);
    !body.contains(['{', '}']) && body.find(';').is_none_or(|at| body[at + 1..].chars().all(|c| c.is_whitespace() || c == '\\'))
}

fn admit_camelot(findings: Vec<Finding>, text: impl Fn(&str) -> Option<String>) -> Vec<Finding> {
    findings
        .into_iter()
        .filter(|item| {
            item.token.starts_with("ABI")
                || !text(&item.file).is_some_and(|source| camelot_admitted(&source, item.line))
        })
        .collect()
}

pub fn find_forbidden(file: &str, text: &str) -> Vec<Finding> {
    let text = &mask_modern(text);
    let text = if Path::new(file).ends_with(DMA_HEADER) {
        mask_dma_body(text)
    } else if Path::new(file).ends_with(IWRAM_CALL_HEADER) {
        mask_iwram_call_body(text)
    } else {
        text.into()
    };
    let source = text.clone();
    admit_camelot(scan_forbidden(file, &text, false), |_| Some(source.clone()))
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
        && crate::c_items::c_text(&path, text)
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
    let text = mask_iwram_call_body(&mask_dma_body(&mask_modern(text)));
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
    admit_camelot(findings, |file| std::fs::read_to_string(file).ok())
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
            Some("c" | "h")
        ) {
            files.push(path);
        }
    }
    files.sort();
    Ok(files)
}

#[cfg(test)]
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
        "games/tbs/src/graphics/window/example.c",
        source,
    );
    if found.len() != 1 || found[0].line != 2 || !found[0].token.contains("M2C_FIELD") {
        return Err("named-source gate missed a tool-branded identifier".into());
    }
    if !find_named_source_tool_leaks("games/tbs/src/unidentified/example.c", source)
        .is_empty()
    {
        return Err("named-source gate crossed its owned boundary".into());
    }
    if !find_named_source_tool_leaks("recon/tbs/example.c", source).is_empty() {
        return Err("named-source gate treated a reconstruction draft as production source".into());
    }
    if find_named_source_tool_leaks("games/tla/src/example.c", source).is_empty() {
        return Err("named-source gate did not apply outside tbs".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn runtime_helper_edits_and_missing_files_revoke_admission_and_cache_identity() {
        let work = tempfile::tempdir().unwrap();
        let source = fs::read_to_string(super::super::routing::root().join(DMA_HEADER)).unwrap();
        let header = "helper.h";
        assert!(reviewed_file_body(work.path(), header, DMA_BODY_SHA256).is_none());
        fs::write(work.path().join(header), &source).unwrap();
        let admitted = reviewed_file_body(work.path(), header, DMA_BODY_SHA256).unwrap();
        let clean = helpers_signature(Some(&admitted), None);
        fs::write(work.path().join(header), source.replace("stmia", "ldmia")).unwrap();
        let edited = reviewed_file_body(work.path(), header, DMA_BODY_SHA256);
        assert!(edited.is_none());
        assert_ne!(clean, helpers_signature(edited.as_deref(), None));
        fs::remove_file(work.path().join(header)).unwrap();
        assert!(reviewed_file_body(work.path(), header, DMA_BODY_SHA256).is_none());
        assert_eq!(mask_body(&source, None), source);
    }

    #[test]
    fn reviewed_dma_is_admitted_only_as_the_complete_shared_body() {
        let dma_source = fs::read_to_string(super::super::routing::root().join(DMA_HEADER)).unwrap();
        assert!(dma_body().is_some());
        assert!(find_forbidden(DMA_HEADER, &dma_source).is_empty());
        assert!(find_forbidden("caller.c", &dma_source).is_empty(), "review follows the exact body, not a filename");
        assert!(find_preprocessed("expanded", &dma_source).is_empty());
        for source in [
            dma_source.clone(),
            dma_source.replacen("register u32", "register u32\t", 1),
        ] {
            assert!(find_forbidden(DMA_HEADER, &source).is_empty());
            assert!(find_preprocessed("expanded", &source).is_empty());
            for changed in [
                source.replace("stmia", "ldmia"),
                source.replace("r0", "r4"),
                source.replace("#12", "#8"),
                source.replace("\"memory\", \"cc\"", "\"memory\""),
                source.replacen("register u32", "volatile register u32", 1),
            ] {
                assert_ne!(changed, source, "DMA mutation must change its fixture");
                assert!(!find_forbidden(DMA_HEADER, &changed).is_empty());
                assert!(!find_preprocessed("expanded", &changed).is_empty());
            }
        }
    }

    #[test]
    fn dma_admission_does_not_hide_extra_assembly_or_abi_attributes() {
        let dma_source = fs::read_to_string(super::super::routing::root().join(DMA_HEADER)).unwrap();
        let extra = format!(
            "{}\nvoid f(void) {{ __asm__(\"nop\"); }}\nvoid g(void) __attribute__((naked));",
            dma_source
        );
        assert_eq!(find_forbidden(DMA_HEADER, &extra).len(), 2);
        assert_eq!(find_preprocessed("expanded", &extra).len(), 2);
        let standalone = "register unsigned int src __asm__(\"r0\");";
        assert_eq!(find_preprocessed("expanded", standalone).len(), 1);
    }

    #[test]
    fn reviewed_iwram_call_is_admitted_only_as_the_complete_shared_body() {
        let iwram_source = fs::read_to_string(super::super::routing::root().join(IWRAM_CALL_HEADER)).unwrap();
        assert!(iwram_call_body().is_some());
        assert!(find_forbidden(IWRAM_CALL_HEADER, &iwram_source).is_empty());
        assert!(find_forbidden("caller.c", &iwram_source).is_empty(), "review follows the exact body, not a filename");
        assert!(find_preprocessed("expanded", &iwram_source).is_empty());
        for changed in [
            iwram_source.replace("mov ip, pc", "mov lr, pc"),
            iwram_source.replace("bx %2", "bx %1"),
            iwram_source.replace("\"r2\", \"ip\", \"cc\"", "\"r2\", \"ip\""),
        ] {
            assert!(!find_forbidden(IWRAM_CALL_HEADER, &changed).is_empty());
            assert!(!find_preprocessed("expanded", &changed).is_empty());
        }
    }

    #[test]
    fn only_the_modern_branches_go_unscanned() {
        let modern = "#ifdef MODERN\nextern char a[] __asm__(\"a\");\n#else\nasm(\"\");\n#endif\n";
        let found = find_forbidden("fixture.c", modern);
        assert_eq!(found.len(), 1, "the matching branch is still scanned");
        assert_eq!(found[0].line, 4);
        let negated = "#ifndef MODERN\nasm(\"\");\n#else\nasm(\"\");\n#endif\nasm(\"\");\n";
        let lines: Vec<usize> = find_forbidden("fixture.c", negated)
            .iter()
            .map(|item| item.line)
            .collect();
        assert_eq!(lines, [2, 6]);
        let nested = "#if MODERN\n#if X\nasm(\"\");\n#endif\nasm(\"\");\n#endif\n";
        assert!(find_forbidden("fixture.c", nested).is_empty());
        assert!(find_preprocessed("expanded", nested).is_empty());
        let other = "#ifdef MODERNISH\nasm(\"\");\n#endif\n";
        assert_eq!(find_forbidden("fixture.c", other).len(), 1);
    }

    #[test]
    fn camelot_asm_needs_its_proof_tag() {
        let bare = "void f(void) {\n    asm(\"swi 0x5\");\n}\n";
        assert_eq!(find_forbidden("fixture.c", bare).len(), 1);
        let tagged =
            "void f(void) {\n    CAMELOT_ASM(\"VBlankIntrWait swi\")\n    asm(\"swi 0x5\");\n}\n";
        assert!(find_forbidden("fixture.c", tagged).is_empty());
        let workaround =
            "void f(void) {\n    /* FAKEMATCH: keeps r9 until the struct is known */\n    asm(\"\");\n}\n";
        assert!(find_forbidden("fixture.c", workaround).is_empty());
        let empty = "void f(void) {\n    CAMELOT_ASM(\"\")\n    asm(\"swi 0x5\");\n}\n";
        let bare_fake = "void f(void) {\n    /* FAKEMATCH: */\n    asm(\"\");\n}\n";
        assert_eq!(find_forbidden("fixture.c", bare_fake).len(), 1);
        assert_eq!(find_forbidden("fixture.c", empty).len(), 1);
        let adjacent = "void f(void) { CAMELOT_ASM(\"BIOS wait\") asm(\"swi 2\"); asm(\"mov r0, r1\"); }\n";
        assert!(!find_forbidden("fixture.c", adjacent).is_empty(), "a shared line cannot enlarge a review scope");
    }

    #[test]
    fn wrapped_proven_register_declarations_keep_their_tag() {
        let wrapped = "void f(void) {\nCAMELOT_ASM(\"identity row registers\")\nregister int a __asm__(\"r1\"),\n    b __asm__(\"r2\"),\n    c __asm__(\"r3\");\n}\n";
        assert!(!find_forbidden("fixture.h", wrapped).is_empty());
        let macro_body = wrapped.replace('\n', "\\\n");
        assert!(!find_forbidden("fixture.h", &macro_body).is_empty());
        assert!(find_forbidden("fixture.h", &wrapped.replace("CAMELOT_ASM(\"identity row registers\")", "/* FAKEMATCH: native row registers */")).is_empty());
        assert!(!find_forbidden("fixture.h", &format!("// 行列\n{}", wrapped.replace('\n', "\r\n"))).is_empty());
        for unmarked in ["\nregister int d __asm__(\"r4\");", " register int d __asm__(\"r4\");", " asm(\"swi 0x5\");"] {
            let escaped = wrapped.replace("c __asm__(\"r3\");", &format!("c __asm__(\"r3\");{unmarked}"));
            assert!(!find_forbidden("fixture.h", &escaped).is_empty(), "{escaped}");
        }
        assert!(!find_forbidden("fixture.h", &wrapped.replace("CAMELOT_ASM(\"identity row registers\")", "CAMELOT_ASM(\"\")")).is_empty());
    }

    #[test]
    fn raw_escape_hatches() {
        self_test().unwrap();
    }

    #[test]
    fn atlas_source_names_keep_tool_leak_checks() {
        let source = "void f(void) { M2C_ERROR(); }";
        let lower =
            find_named_source_tool_leaks("games/tbs/src/field/example.c", source);
        let upper =
            find_named_source_tool_leaks("games/tbs/src/field/example.c", source);
        assert!(!lower.is_empty());
        assert_eq!(lower.len(), upper.len());
    }
}
