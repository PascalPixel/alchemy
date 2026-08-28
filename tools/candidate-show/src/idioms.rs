use compiler_core::{overlay_call_via_base, CALL_VIA_BASE};
use regex::Regex;
use serde::{Deserialize, Serialize};
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct CallViaRequirement {
    pub register: u8,
    pub address: u64,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct LintFinding {
    pub rule: String,
    pub playbook: String,
    pub detail: String,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct LintReport {
    pub requirements: Vec<CallViaRequirement>,
    pub findings: Vec<LintFinding>,
}
impl LintReport {
    pub fn passes(&self) -> bool {
        self.findings.is_empty()
    }
}
fn call_target(line: &str) -> Option<u64> {
    let target = line
        .split('@')
        .next()?
        .split_ascii_whitespace()
        .collect::<Vec<_>>();
    if target.first().is_none_or(|op| *op != "bl") {
        return None;
    }
    let value = target.get(1)?.trim_end_matches(',').trim_end_matches(':');
    let value = value.strip_prefix("Func_").unwrap_or(value);
    u64::from_str_radix(value.trim_start_matches("0x"), 16).ok()
}
fn call_via_requirements(
    reference_assembly: &str,
    overlay: Option<&str>,
) -> Vec<CallViaRequirement> {
    let base = overlay.map_or(CALL_VIA_BASE, overlay_call_via_base);
    let mut requirements = reference_assembly
        .lines()
        .filter_map(call_target)
        .filter_map(|address| {
            let offset = address.checked_sub(base)?;
            (offset % 4 == 0 && offset / 4 <= 13).then_some(CallViaRequirement {
                register: (offset / 4) as u8,
                address,
            })
        })
        .collect::<Vec<_>>();
    requirements.sort_by_key(|requirement| (requirement.address, requirement.register));
    requirements.dedup();
    requirements
}
fn direct_call(source: &str, address: u64) -> bool {
    let symbol = format!("Func_{address:08x}");
    Regex::new(&format!(r"\b{}\s*\(", regex::escape(&symbol)))
        .unwrap()
        .is_match(source)
}
fn without_comments(source: &str) -> String {
    let block = Regex::new(r"(?s)/\*.*?\*/").unwrap();
    let line = Regex::new(r"(?m)//[^\r\n]*").unwrap();
    line.replace_all(&block.replace_all(source, ""), "")
        .into_owned()
}
/// The reference itself has to prove this exact adjacent-load idiom. The
/// address alone is insufficient: some owners use other cache slots without
/// loading the kind-39/kind-40 pair.
fn requires_shared_work_cache_cursor(reference_assembly: &str) -> bool {
    let cache_literal = Regex::new(r"(?i)\.4byte\s+0x0*3001eec\b").unwrap();
    if !cache_literal.is_match(reference_assembly) {
        return false;
    }
    let lines = reference_assembly.lines().collect::<Vec<_>>();
    let first_call = lines
        .iter()
        .position(|line| {
            line.trim_start().starts_with("bl\t") || line.trim_start().starts_with("bl ")
        })
        .unwrap_or(lines.len());
    let load_multiple =
        Regex::new(r"^\s*ldmia\s+(r(?:1[0-5]|[0-9]))!\s*,\s*\{\s*r(?:1[0-5]|[0-9])\s*\}\s*$")
            .unwrap();
    for (index, line) in lines.iter().enumerate().take(first_call) {
        let Some(capture) = load_multiple.captures(line) else {
            continue;
        };
        let cursor = &capture[1];
        let next_word = Regex::new(&format!(
            r"^\s*ldr\s+r(?:1[0-5]|[0-9])\s*,\s*\[\s*{}\s*,\s*#0\s*\]\s*$",
            regex::escape(cursor)
        ))
        .unwrap();
        if lines[index + 1..first_call]
            .iter()
            .take(6)
            .any(|candidate| next_word.is_match(candidate))
        {
            return true;
        }
    }
    false
}
fn assignment(statement: &str) -> Option<(&str, &str)> {
    let equals = statement.char_indices().find_map(|(index, value)| {
        if value != '=' {
            return None;
        }
        let before = statement[..index].chars().next_back();
        let after = statement[index + 1..].chars().next();
        (!matches!(before, Some('!' | '<' | '>' | '=')) && after != Some('=')).then_some(index)
    })?;
    let (left, right) = statement.split_at(equals);
    Some((left, &right[1..]))
}
fn address_origin(statements: &[&str], before: usize, name: &str) -> bool {
    let identifier = Regex::new(r"[A-Za-z_][A-Za-z0-9_]*").unwrap();
    let assignment = (0..before).rev().find_map(|index| {
        let (left, right) = assignment(statements[index])?;
        (identifier.find_iter(left).last()?.as_str() == name).then_some((index, right))
    });
    let Some((index, right)) = assignment else {
        return false;
    };
    if Regex::new(r"(?i)0x0*3001eec\b").unwrap().is_match(right) {
        return true;
    }
    let mut names = identifier.find_iter(right).map(|value| value.as_str());
    names
        .next()
        .filter(|_| names.next().is_none())
        .is_some_and(|origin| address_origin(statements, index, origin))
}
fn models_shared_work_cache_cursor(source: &str) -> bool {
    let clean = without_comments(source);
    let statements = clean.split(';').collect::<Vec<_>>();
    let postincrement = Regex::new(r"\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*\+\+").unwrap();
    statements.iter().enumerate().any(|(index, statement)| {
        let cursor = assignment(statement)
            .and_then(|(_, right)| postincrement.captures(right))
            .map(|capture| capture[1].to_string());
        cursor.is_some_and(|cursor| {
            let plain_deref = Regex::new(&format!(r"\*\s*{}\b", regex::escape(&cursor))).unwrap();
            address_origin(&statements, index, &cursor)
                && statements[index + 1..]
                    .iter()
                    .take(2)
                    .filter_map(|next| assignment(next).map(|(_, right)| right))
                    .any(|right| plain_deref.is_match(right) && !postincrement.is_match(right))
        })
    })
}
/// Lint unresolved candidates before scoring. Low-register direct-call
/// spellings remain accepted: the original ABI deliberately passes the branch
/// target in r0-r3 at a matching veneer address. A direct call to an r4-r13
/// slot cannot load that register through the C calling convention and is the
/// repeated fictional-callee model this rule rejects.
pub fn lint(source: &str, reference_assembly: &str, overlay: Option<&str>) -> LintReport {
    let requirements = call_via_requirements(reference_assembly, overlay);
    let clean_source = without_comments(source);
    let mut findings = requirements
        .iter()
        .filter(|requirement| {
            requirement.register >= 4 && direct_call(&clean_source, requirement.address)
        })
        .map(|requirement| LintFinding {
            rule: "call-via-is-indirect".into(),
            playbook: "model-typed-indirect-call".into(),
            detail: format!(
                "reference branches through r{} at 0x{:08x}; an ordinary direct callee cannot place the target in that register",
                requirement.register, requirement.address
            ),
        })
        .collect::<Vec<_>>();
    if requires_shared_work_cache_cursor(reference_assembly)
        && !models_shared_work_cache_cursor(&clean_source)
    {
        findings.push(LintFinding {
            rule: "shared-work-cache-cursor".into(),
            playbook: "model-shared-work-cache-cursor".into(),
            detail: "reference loads adjacent kind-39/kind-40 cache words with a one-register ldmia writeback followed by a load from the advanced cursor; model one 0x03001eec-derived cursor and *cursor++ / *cursor".into(),
        });
    }
    LintReport {
        requirements,
        findings,
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    const SHARED_CACHE_REFERENCE: &str = "\
        ldr r5, [pc, #44]\n\
        adds r3, r5, #0\n\
        ldmia r3!, {r1}\n\
        sub sp, #16\n\
        str r1, [sp, #4]\n\
        ldr r2, [r3, #0]\n\
        bl Func_08001234\n\
        .4byte 0x03001eec\n";
    fn rules(source: &str, reference: &str) -> Vec<String> {
        lint(source, reference, None)
            .findings
            .into_iter()
            .map(|finding| finding.rule)
            .collect()
    }
    #[test]
    fn rejects_fictional_high_register_callee_but_allows_indirect_and_low_register_abi() {
        let reference = "\tbl Func_080072f4\n\tbl Func_080072f0\n";
        assert_eq!(
            rules("void f(void) { Func_080072f4(1,2,3,4,5); }", reference),
            ["call-via-is-indirect"]
        );
        assert!(rules(
            "typedef void (*DrawFn)(int); void f(DrawFn draw) { draw(1); }",
            reference
        )
        .is_empty());
        assert!(rules("void f(void) { Func_080072f0(1,2,3,target); }", reference).is_empty());
    }
    #[test]
    fn uses_overlay_bank_authority() {
        let base = overlay_call_via_base("resource_3a0");
        let reference = format!("\tbl Func_{:08x}\n", base + 20);
        let report = lint(
            &format!("void f(void) {{ Func_{:08x}(); }}", base + 20),
            &reference,
            Some("resource_3a0"),
        );
        assert_eq!(report.requirements[0].register, 5);
        assert_eq!(report.findings.len(), 1);
    }
    #[test]
    fn shared_work_cache_reference_requires_the_cursor_model() {
        let correct = "void f(void) { void **cache; void **cursor; void *work; void *canvas; cache = (void **)0x03001EEC; cursor = cache; work = *cursor++; canvas = *cursor; }";
        assert!(rules(correct, SHARED_CACHE_REFERENCE).is_empty());
        for wrong in [
            "void f(void) { void *work; void *canvas; work = *(void **)0x03001EEC; canvas = *(void **)0x03001EF0; }",
            "void f(void) { void **cache; void *work; void *canvas; cache = (void **)0x03001EEC; work = cache[0]; canvas = cache[1]; }",
        ] {
            assert_eq!(rules(wrong, SHARED_CACHE_REFERENCE), ["shared-work-cache-cursor"]);
        }
    }
    #[test]
    fn shared_work_cache_lint_uses_code_and_reference_evidence_only() {
        let commented = "/* cursor = (void **)0x03001EEC; work = *cursor++; canvas = *cursor; */\nvoid f(void) { void *work = *(void **)0x03001EEC; }";
        assert_eq!(
            rules(commented, SHARED_CACHE_REFERENCE),
            ["shared-work-cache-cursor"]
        );
        let no_idiom = "ldr r0, [pc, #4]\nbl Func_08001234\n.4byte 0x03001eec\n";
        assert!(rules(
            "void f(void) { void *work = *(void **)0x03001EEC; }",
            no_idiom
        )
        .is_empty());
    }
}
