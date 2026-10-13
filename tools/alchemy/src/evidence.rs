//! Reviewed source evidence, bound to source tokens rather than paths or tags.
//! Digests describe our maintained source, never reference ROMs or expected
//! compiler output. A record preserves a human review; it is not an automatic
//! proof that an arbitrary assembly algorithm was handwritten.

use regex::Regex;
use sha2::{Digest, Sha256};
use std::collections::BTreeMap;
use std::ops::Range;
use std::path::Path;
use std::sync::OnceLock;

const LEDGER: &str = "tools/alchemy/evidence.tsv";
const HEADER: &str = "kind\tsource_sha256\tevidence";

#[derive(Default)]
pub(crate) struct Reviews(BTreeMap<(String, String), String>);

impl Reviews {
    fn parse(text: &str) -> Result<Self, String> {
        let mut lines = text.lines();
        if lines.next() != Some(HEADER) { return Err("invalid source evidence header".into()); }
        let mut rows = BTreeMap::new();
        for line in lines {
            let fields = line.split('\t').collect::<Vec<_>>();
            let [kind, digest, reason] = fields.as_slice() else { return Err("invalid source evidence row".into()) };
            if !matches!(*kind, "assembly" | "inline-assembly")
                || digest.len() != 64 || !digest.bytes().all(|c| c.is_ascii_hexdigit())
                || reason.trim().is_empty() {
                return Err("source evidence needs a kind, digest and review reason".into());
            }
            if rows.insert((kind.to_string(), digest.to_ascii_lowercase()), reason.to_string()).is_some() {
                return Err("duplicate source evidence".into());
            }
        }
        Ok(Self(rows))
    }

    pub(crate) fn reviewed(&self, kind: &str, digest: &str) -> Option<&str> {
        self.0.get(&(kind.to_string(), digest.to_string())).map(String::as_str)
    }
}

fn ledger() -> &'static Reviews {
    static REVIEWS: OnceLock<Reviews> = OnceLock::new();
    REVIEWS.get_or_init(|| {
        let path = crate::compiler::routing::root().join(LEDGER);
        match std::fs::read_to_string(&path).map_err(|e| e.to_string()).and_then(|text| Reviews::parse(&text)) {
            Ok(reviews) => reviews,
            Err(error) => {
                eprintln!("{}: {error}; source review exemptions disabled", path.display());
                Reviews::default()
            }
        }
    })
}

pub(crate) fn signature() -> String {
    format!("{:x}", Sha256::digest(format!("{:?}", ledger().0).as_bytes()))
}

fn tokens(text: &str, assembly: bool) -> Vec<regex::Match<'_>> {
    static TOKENS: OnceLock<Regex> = OnceLock::new();
    TOKENS.get_or_init(|| Regex::new(r#"(?ms)//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'|@[^\n]*|[A-Za-z_][A-Za-z0-9_]*|[0-9]+|[^\s]"#).unwrap())
        .find_iter(text)
        .filter(|token| !token.as_str().starts_with("//") && !token.as_str().starts_with("/*")
            && !(assembly && token.as_str().starts_with('@')) && token.as_str() != "\\")
        .collect()
}

pub(crate) fn digest(text: &str, assembly: bool) -> String {
    let tokens = tokens(text, assembly).into_iter().map(|t| t.as_str()).collect::<Vec<_>>();
    format!("{:x}", Sha256::digest(tokens.join("\n").as_bytes()))
}

pub(crate) fn assembly_review(root: &Path, path: &str, text: &str) -> Option<String> {
    let actual = std::fs::read_to_string(root.join(path)).ok()?;
    if actual != text { return None; }
    ledger().reviewed("assembly", &assembly_digest(root, Path::new(path))?).map(str::to_owned)
}

fn assembly_digest(root: &Path, path: &Path) -> Option<String> {
    fn collect(root: &Path, path: &Path, active: &mut Vec<std::path::PathBuf>) -> Option<String> {
        let path = std::fs::canonicalize(root.join(path)).ok()?;
        let games = std::fs::canonicalize(root.join("games")).ok()?;
        if !path.starts_with(games) || active.contains(&path) { return None; }
        active.push(path.clone());
        let text = std::fs::read_to_string(&path).ok()?;
        let mut body = digest(&text, true);
        for line in text.lines() {
            let line = line.trim_start();
            if let Some(include) = line.strip_prefix(".include").or_else(|| line.strip_prefix("#include")) {
                let name = include.trim().strip_prefix('"')?.split('"').next()?;
                let child = if root.join(name).is_file() { root.join(name) } else { path.parent()?.join(name) };
                body.push_str(&collect(root, &child, active)?);
            }
        }
        active.pop();
        Some(body)
    }
    collect(root, path, &mut Vec::new()).map(|body| format!("{:x}", Sha256::digest(body.as_bytes())))
}

/// Whether an assembly source builds its entries with a veneer macro, in the
/// file itself or in a file it includes: a far-call bank (NAME.S) reads its
/// entries from its list and the macro that emits each one from beside it.
/// The spelling only nominates the object; its placed bytes decide the credit.
pub(crate) fn builds_veneers(root: &Path, path: &Path) -> bool {
    fn collect(root: &Path, path: &Path, active: &mut Vec<std::path::PathBuf>) -> bool {
        let Ok(path) = std::fs::canonicalize(root.join(path)) else { return false };
        let Ok(games) = std::fs::canonicalize(root.join("games")) else { return false };
        if !path.starts_with(games) || active.contains(&path) { return false; }
        let Ok(text) = std::fs::read_to_string(&path) else { return false };
        active.push(path.clone());
        let found = text.lines().any(|line| {
            let line = line.trim_start();
            if matches!(line.split_whitespace().next(), Some("overlay_veneer" | "IwramEntry")) { return true; }
            let Some(include) = line.strip_prefix(".include").or_else(|| line.strip_prefix("#include")) else { return false };
            let Some(name) = include.trim().strip_prefix('"').and_then(|rest| rest.split('"').next()) else { return false };
            let beside = path.parent().map(|folder| folder.join(name));
            let child = if root.join(name).is_file() { root.join(name) } else { beside.unwrap_or_else(|| root.join(name)) };
            collect(root, &child, active)
        });
        active.pop();
        found
    }
    collect(root, path, &mut Vec::new())
}

/// The exact statement, helper definition or macro following each claim.
/// Strings and comments are tokenized, so their braces/semicolons cannot
/// enlarge the scope of a review and hide an unrelated statement.
pub(crate) fn claims(text: &str) -> Vec<(Range<usize>, String)> {
    let mut claims = Vec::new();
    let all = tokens(text, false);
    let mut i = 0;
    while i + 3 < all.len() {
        if all[i].as_str() != "CAMELOT_ASM" || all[i + 1].as_str() != "("
            || !all[i + 2].as_str().starts_with('"') || all[i + 3].as_str() != ")" {
            i += 1; continue;
        }
        let reason = all[i + 2].as_str().trim_matches('"').to_string();
        let first = i + 4;
        if first >= all.len() { break; }
        let helper = all[first].as_str() == "static";
        let macro_ = all[first].as_str() == "#";
        let mut braces = 0i32;
        let mut seen_brace = false;
        let mut end = first;
        while end < all.len() {
            let word = all[end].as_str();
            if macro_ && end > first {
                let gap = &text[all[end - 1].end()..all[end].start()];
                if gap.contains('\n') && !gap.trim_end().ends_with('\\') { end -= 1; break; }
            }
            if word == "{" { braces += 1; seen_brace = true; }
            if word == "}" { braces -= 1; }
            if (helper && seen_brace && braces == 0) || (!helper && !macro_ && word == ";") { break; }
            end += 1;
        }
        if end < all.len() && !reason.is_empty() {
            claims.push((all[first].start()..all[end].end(), reason));
        }
        i += 4;
    }
    claims
}

pub(crate) fn inline_reviewed(body: &str) -> bool {
    ledger().reviewed("inline-assembly", &digest(body, false)).is_some()
        // A sole BIOS software interrupt is mechanically proven C2 evidence.
        || bios_interrupt(body)
}

fn bios_interrupt(body: &str) -> bool {
    static BIOS: OnceLock<Regex> = OnceLock::new();
    BIOS.get_or_init(|| Regex::new(r#"^\s*(?:asm|__asm__|__asm)\s*(?:volatile|__volatile__)?\s*\(\s*"swi (?:0x[0-9a-fA-F]+|[0-9]+)"\s*\)\s*;\s*$"#).unwrap()).is_match(body)
}

pub(crate) fn approved_spans(text: &str) -> Vec<Range<usize>> {
    claims(text).into_iter().filter_map(|(span, _)| inline_reviewed(&text[span.clone()]).then_some(span)).collect()
}

pub(crate) fn mask_inline(text: &str) -> String {
    let mut bytes = text.as_bytes().to_vec();
    for span in approved_spans(text) {
        for byte in &mut bytes[span] { if *byte != b'\n' { *byte = b' '; } }
    }
    String::from_utf8(bytes).expect("review scopes contain complete tokens")
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn review_follows_source_tokens_and_changes_revoke_it() {
        let hash = digest("mov r1, r2\n", true);
        let reviews = Reviews::parse(&format!("{HEADER}\nassembly\t{hash}\treviewed linkage\n")).unwrap();
        assert!(reviews.reviewed("assembly", &digest("@ different filename\n mov r1, r2 @ comment\n", true)).is_some());
        assert!(reviews.reviewed("assembly", &digest("mov r1, r2\nadd r0, r1, #1\n", true)).is_none());
        assert!(reviews.reviewed("inline-assembly", &hash).is_none());
    }

    #[test]
    fn claim_text_and_statement_strings_do_not_grant_or_expand_exemptions() {
        let source = "CAMELOT_ASM(\"swi proof\")\nasm(\"swi 2\");\nasm(\"mov r0, r1; }\");\n";
        let masked = mask_inline(source);
        assert!(!masked.contains("asm(\"swi 2\")"));
        assert!(masked.contains("asm(\"mov r0, r1; }\")"));
        let forged = "CAMELOT_ASM(\"swi proof\")\nasm(\"swi 2; mov r0, r1\");\n";
        assert_eq!(mask_inline(forged), forged);
    }

    #[test]
    fn malformed_and_duplicate_reviews_are_not_admissions() {
        let hash = digest("a", true);
        for text in ["bad".to_owned(), format!("{HEADER}\nassembly\t{hash}\t\n"),
            format!("{HEADER}\nassembly\t{hash}\tx\nassembly\t{hash}\ty\n")] {
            assert!(Reviews::parse(&text).is_err());
        }
    }

    #[test]
    fn included_instructions_are_part_of_the_review() {
        let dir = tempfile::tempdir().unwrap();
        std::fs::create_dir(dir.path().join("games")).unwrap();
        std::fs::write(dir.path().join("games/a.s"), ".include \"games/b.s\"\nmov r0,r1\n").unwrap();
        std::fs::write(dir.path().join("games/b.s"), "bx lr\n").unwrap();
        let first = assembly_digest(dir.path(), Path::new("games/a.s")).unwrap();
        std::fs::write(dir.path().join("games/b.s"), "add r0,r1,r2\nbx lr\n").unwrap();
        assert_ne!(assembly_digest(dir.path(), Path::new("games/a.s")).unwrap(), first);
        std::fs::write(dir.path().join("games/b.s"), ".include \"games/a.s\"\n").unwrap();
        assert!(assembly_digest(dir.path(), Path::new("games/a.s")).is_none());
    }

    #[test]
    fn a_bank_builds_veneers_through_the_files_it_includes() {
        let dir = tempfile::tempdir().unwrap();
        let bank = dir.path().join("games/src/far_call");
        std::fs::create_dir_all(&bank).unwrap();
        std::fs::create_dir_all(dir.path().join("games/include/farcall")).unwrap();
        std::fs::write(bank.join("MENU.S"), "#include \"stub.inc\"\nTable:\n#include \"../../include/farcall/menu.h\"\n").unwrap();
        std::fs::write(dir.path().join("games/include/farcall/menu.h"), "FAR_CALL(RunFar, Run, void, (void))\nFAR_SLOT(Draw)\n").unwrap();
        // The list names no veneer macro: the stubs come from the macro file.
        std::fs::write(bank.join("stub.inc"), "#define FAR_SLOT(target) far_slot target\n").unwrap();
        assert!(!builds_veneers(dir.path(), Path::new("games/src/far_call/MENU.S")));
        std::fs::write(bank.join("stub.inc"), "\t.macro far_slot target\n\toverlay_veneer \\target\n\t.endm\n").unwrap();
        assert!(builds_veneers(dir.path(), Path::new("games/src/far_call/MENU.S")));
        std::fs::write(bank.join("plain.s"), "\toverlay_veneer Target\n").unwrap();
        assert!(builds_veneers(dir.path(), Path::new("games/src/far_call/plain.s")));
        // Ordinary assembly, a file that includes itself and a file outside games/ build none.
        std::fs::write(bank.join("code.s"), "\tbx lr @ overlay_veneer\n.include \"games/src/far_call/code.s\"\n").unwrap();
        assert!(!builds_veneers(dir.path(), Path::new("games/src/far_call/code.s")));
        std::fs::write(dir.path().join("outside.s"), "\toverlay_veneer Target\n").unwrap();
        assert!(!builds_veneers(dir.path(), Path::new("outside.s")));
    }
}
