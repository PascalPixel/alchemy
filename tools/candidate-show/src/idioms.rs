use compiler_core::{overlay_call_via_base, CALL_VIA_BASE};
use regex::Regex;
use serde::{Deserialize, Serialize};
use std::{collections::BTreeMap, fs, path::Path};
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
pub fn call_via_requirements(
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
fn assigned_name(left: &str) -> Option<&str> {
    Regex::new(r"[A-Za-z_][A-Za-z0-9_]*")
        .unwrap()
        .find_iter(left)
        .last()
        .map(|value| value.as_str())
}
fn address_origin(statements: &[&str], before: usize, name: &str) -> bool {
    let address = Regex::new(r"(?i)0x0*3001eec\b").unwrap();
    for index in (0..before).rev() {
        let Some((left, right)) = assignment(statements[index]) else {
            continue;
        };
        if assigned_name(left) != Some(name) {
            continue;
        }
        if address.is_match(right) {
            return true;
        }
        let identifiers = Regex::new(r"[A-Za-z_][A-Za-z0-9_]*")
            .unwrap()
            .find_iter(right)
            .map(|value| value.as_str())
            .collect::<Vec<_>>();
        return identifiers.len() == 1 && address_origin(statements, index, identifiers[0]);
    }
    false
}
fn models_shared_work_cache_cursor(source: &str) -> bool {
    let clean = without_comments(source);
    let statements = clean.split(';').collect::<Vec<_>>();
    let postincrement = Regex::new(r"\*\s*([A-Za-z_][A-Za-z0-9_]*)\s*\+\+").unwrap();
    for (index, statement) in statements.iter().enumerate() {
        let Some((_, right)) = assignment(statement) else {
            continue;
        };
        let Some(capture) = postincrement.captures(right) else {
            continue;
        };
        let cursor = &capture[1];
        if !address_origin(&statements, index, cursor) {
            continue;
        }
        let plain_deref = Regex::new(&format!(r"\*\s*{}\b", regex::escape(cursor))).unwrap();
        if statements[index + 1..]
            .iter()
            .take(2)
            .filter_map(|next| assignment(next).map(|(_, right)| right))
            .any(|right| plain_deref.is_match(right) && !postincrement.is_match(right))
        {
            return true;
        }
    }
    false
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
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct DossierRecurrence {
    pub class: String,
    pub owners: Vec<String>,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct DossierMine {
    pub schema_version: u32,
    pub advisory_only: bool,
    pub recurrences: Vec<DossierRecurrence>,
    pub template_quality_advisories: Vec<DossierRecurrence>,
}
/// Dossier prose is evidence for smart review only. It never registers a lint
/// or routes an owner by itself.
pub fn mine_dossiers(directory: &Path) -> Result<DossierMine, String> {
    let classes = [
        (
            "call-via-is-indirect",
            ["trampoline", "call-via", "call via", "_call_via"],
        ),
        (
            "shared-work-cache",
            ["heap_cache", "work-block", "work block", "0x03001eec"],
        ),
    ];
    let shape_needles = [
        "shape-only",
        "shape only",
        "false positive",
        "weak template",
    ];
    let mut found: BTreeMap<&str, Vec<String>> = BTreeMap::new();
    let mut shape_only = Vec::new();
    let entries =
        fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| error.to_string())?;
        let path = entry.path();
        if path.extension().and_then(|value| value.to_str()) != Some("json") {
            continue;
        }
        let text = fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?
            .to_ascii_lowercase();
        let owner = path
            .file_stem()
            .and_then(|value| value.to_str())
            .unwrap_or("unknown")
            .to_string();
        for (class, needles) in &classes {
            if needles.iter().any(|needle| text.contains(needle)) {
                found.entry(class).or_default().push(owner.clone());
            }
        }
        if shape_needles.iter().any(|needle| text.contains(needle)) {
            shape_only.push(owner);
        }
    }
    let recurrences = found
        .into_iter()
        .filter(|(_, owners)| owners.len() >= 2)
        .map(|(class, mut owners)| {
            owners.sort();
            DossierRecurrence {
                class: class.to_string(),
                owners,
            }
        })
        .collect();
    shape_only.sort();
    let template_quality_advisories = (shape_only.len() >= 2)
        .then_some(DossierRecurrence {
            class: "shape-only-template".into(),
            owners: shape_only,
        })
        .into_iter()
        .collect();
    Ok(DossierMine {
        schema_version: 2,
        advisory_only: true,
        recurrences,
        template_quality_advisories,
    })
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
    #[test]
    fn rejects_fictional_high_register_callee_but_allows_indirect_and_low_register_abi() {
        let reference = "\tbl Func_080072f4\n\tbl Func_080072f0\n";
        let wrong = lint(
            "void f(void) { Func_080072f4(1,2,3,4,5); }",
            reference,
            None,
        );
        assert_eq!(wrong.findings.len(), 1);
        assert_eq!(wrong.findings[0].rule, "call-via-is-indirect");
        let correct = lint(
            "typedef void (*DrawFn)(int); void f(DrawFn draw) { draw(1); }",
            reference,
            None,
        );
        assert!(correct.passes());
        let low = lint(
            "void f(void) { Func_080072f0(1,2,3,target); }",
            reference,
            None,
        );
        assert!(low.passes());
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
        let correct = lint(
            "void f(void) { void **cache; void **cursor; void *work; void *canvas; cache = (void **)0x03001EEC; cursor = cache; work = *cursor++; canvas = *cursor; }",
            SHARED_CACHE_REFERENCE,
            None,
        );
        assert!(correct.passes());
        let independent_literals = lint(
            "void f(void) { void *work; void *canvas; work = *(void **)0x03001EEC; canvas = *(void **)0x03001EF0; }",
            SHARED_CACHE_REFERENCE,
            None,
        );
        assert_eq!(independent_literals.findings.len(), 1);
        assert_eq!(
            independent_literals.findings[0].rule,
            "shared-work-cache-cursor"
        );
        let indexed_twice = lint(
            "void f(void) { void **cache; void *work; void *canvas; cache = (void **)0x03001EEC; work = cache[0]; canvas = cache[1]; }",
            SHARED_CACHE_REFERENCE,
            None,
        );
        assert_eq!(indexed_twice.findings.len(), 1);
        assert_eq!(indexed_twice.findings[0].rule, "shared-work-cache-cursor");
    }
    #[test]
    fn shared_work_cache_lint_uses_code_and_reference_evidence_only() {
        let comment_only = lint(
            "/* cursor = (void **)0x03001EEC; work = *cursor++; canvas = *cursor; */\nvoid f(void) { void *work = *(void **)0x03001EEC; }",
            SHARED_CACHE_REFERENCE,
            None,
        );
        assert_eq!(comment_only.findings.len(), 1);
        let no_reference_idiom = lint(
            "void f(void) { void *work = *(void **)0x03001EEC; }",
            "ldr r0, [pc, #4]\nbl Func_08001234\n.4byte 0x03001eec\n",
            None,
        );
        assert!(no_reference_idiom.passes());
    }
    #[test]
    fn shape_only_recurrence_is_a_template_quality_advisory() {
        let unique = std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_nanos();
        let directory = std::env::temp_dir().join(format!(
            "alchemy-idiom-mine-{}-{unique}",
            std::process::id()
        ));
        fs::create_dir_all(&directory).unwrap();
        fs::write(
            directory.join("08000000.json"),
            r#"{"note":"shape-only template; heap_cache model"}"#,
        )
        .unwrap();
        fs::write(
            directory.join("08000004.json"),
            r#"{"note":"weak template; shared work block"}"#,
        )
        .unwrap();
        let mine = mine_dossiers(&directory).unwrap();
        fs::remove_dir_all(&directory).unwrap();
        assert!(mine
            .recurrences
            .iter()
            .all(|entry| entry.class != "shape-only-template"));
        assert_eq!(mine.template_quality_advisories.len(), 1);
        assert_eq!(
            mine.template_quality_advisories[0].class,
            "shape-only-template"
        );
    }
}
