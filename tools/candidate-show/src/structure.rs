//! Structural divergence decoder over disassembled instruction streams.
//!
//! The allocator decoder explains register-role residuals once the two
//! instruction streams already align. This module reads the same
//! `candidate.bin` / `reference.bin` pair when they do not align, names the
//! control-flow divergence it can prove from branch structure alone, and
//! proposes a catalogued repair only for the shapes the permuter can search
//! finitely. Everything else is reported as named evidence for the smart
//! queue rather than guessed at.

use std::path::Path;

use crate::allocator::{Repair, RepairPlan, Report};
use crate::disasm::disassemble;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Divergence {
    pub kind: &'static str,
    pub detail: String,
    pub advice: &'static str,
}

#[derive(Clone, Debug, Eq, PartialEq)]
struct Branch {
    offset: u32,
    op: String,
    backward: bool,
}

const CONDITIONALS: &[&str] = &[
    "beq", "bne", "bcs", "bhs", "bcc", "blo", "bmi", "bpl", "bvs", "bvc", "bhi", "bls", "bge",
    "blt", "bgt", "ble",
];

fn complement(op: &str) -> Option<&'static str> {
    Some(match op {
        "beq" => "bne",
        "bne" => "beq",
        "bcs" | "bhs" => "bcc",
        "bcc" | "blo" => "bcs",
        "bmi" => "bpl",
        "bpl" => "bmi",
        "bvs" => "bvc",
        "bvc" => "bvs",
        "bhi" => "bls",
        "bls" => "bhi",
        "bge" => "blt",
        "blt" => "bge",
        "bgt" => "ble",
        "ble" => "bgt",
        _ => return None,
    })
}

/// The comparison branches whose spelling follows the compare's operand
/// order. Mirroring `a < b` into `b > a` in the source exchanges exactly
/// these pairs without touching behaviour. Complementary pairs (`beq`/`bne`,
/// `bge`/`blt`) are a different situation — an inverted guard with swapped
/// arms — and are reported as evidence, not repaired here.
fn mirrorable(left: &str, right: &str) -> bool {
    matches!(
        (left, right),
        ("bge", "ble")
            | ("ble", "bge")
            | ("blt", "bgt")
            | ("bgt", "blt")
            | ("blo", "bhi")
            | ("bcc", "bhi")
            | ("bhi", "blo")
            | ("bhi", "bcc")
            | ("bls", "bhs")
            | ("bls", "bcs")
            | ("bhs", "bls")
            | ("bcs", "bls")
    )
}

fn mnemonic(text: &str) -> &str {
    text.split_ascii_whitespace()
        .next()
        .unwrap_or("")
        .trim_end_matches(".n")
        .trim_end_matches(".w")
}

fn branch_target(text: &str) -> Option<u32> {
    let target = text.split_ascii_whitespace().nth(1)?;
    let target = target.trim_start_matches("0x");
    u32::from_str_radix(target, 16).ok()
}

/// Instruction indexes reachable from the function entry by fallthrough and
/// direct branches. Literal-pool words disassemble as junk instructions;
/// junk conditionals would otherwise inflate the branch census on one side
/// and fake a structural divergence. Computed jumps end their path, which
/// leaves genuine post-table code unwalked on both sides alike — an accepted
/// blind spot, never a fabricated finding.
fn reachable(rows: &[(u32, u32, String)]) -> Vec<bool> {
    let index_of = |offset: u32| rows.iter().position(|(at, _, _)| *at == offset);
    let mut seen = vec![false; rows.len()];
    let mut work = vec![0_usize];
    while let Some(index) = work.pop() {
        if index >= rows.len() || seen[index] {
            continue;
        }
        seen[index] = true;
        let (_, _, text) = &rows[index];
        let op = mnemonic(text);
        if let Some(target) = branch_target(text) {
            if CONDITIONALS.contains(&op) || op == "b" || op == "bal" {
                if let Some(target) = index_of(target) {
                    work.push(target);
                }
            }
        }
        let ends = matches!(op, "b" | "bal" | "bx")
            || (op == "pop" && text.contains("pc"))
            || (op == "mov" && text.contains("pc,"))
            || (op == "add" && text.contains("pc,"));
        if !ends {
            work.push(index + 1);
        }
    }
    seen
}

fn branches(rows: &[(u32, u32, String)]) -> Vec<Branch> {
    let extent = rows
        .last()
        .map(|(offset, width, _)| offset + width)
        .unwrap_or(0);
    let live = reachable(rows);
    rows.iter()
        .enumerate()
        .filter_map(|(index, (offset, _, text))| {
            if !live[index] {
                return None;
            }
            let op = mnemonic(text);
            if !CONDITIONALS.contains(&op) {
                return None;
            }
            let target = branch_target(text)?;
            if target >= extent {
                return None;
            }
            Some(Branch {
                offset: *offset,
                op: op.to_string(),
                backward: target <= *offset,
            })
        })
        .collect()
}

fn rows_of(binary: &Path) -> Result<Vec<(u32, u32, String)>, String> {
    let rows = disassemble(&binary.to_string_lossy(), 0.0)?;
    let mut keys: Vec<f64> = rows.keys().collect();
    keys.sort_by(|left, right| left.partial_cmp(right).unwrap_or(std::cmp::Ordering::Equal));
    Ok(keys
        .iter()
        .enumerate()
        .filter_map(|(index, key)| {
            let offset = *key as u32;
            let width = keys
                .get(index + 1)
                .map(|next| (*next as u32).saturating_sub(offset))
                .unwrap_or(2);
            rows.get(*key).map(|text| (offset, width, text.to_string()))
        })
        .collect())
}

fn compare(candidate: &[Branch], reference: &[Branch]) -> Vec<Divergence> {
    let mut findings = Vec::new();
    if candidate.len() != reference.len() {
        findings.push(Divergence {
            kind: "conditional-count",
            detail: format!(
                "candidate={} reference={}",
                candidate.len(),
                reference.len()
            ),
            advice: "a guard is missing, invented, or merged; recover the branch structure before spelling experiments",
        });
        return findings;
    }
    let backward = |branches: &[Branch]| branches.iter().filter(|branch| branch.backward).count();
    if backward(candidate) != backward(reference) {
        findings.push(Divergence {
            kind: "loop-shape",
            detail: format!(
                "backward_candidate={} backward_reference={}",
                backward(candidate),
                backward(reference)
            ),
            advice: "loop count or loop form differs; recover the loop structure before spelling experiments",
        });
        return findings;
    }
    let mut mirrors = 0;
    let mut other = 0;
    for (ordinal, (left, right)) in candidate.iter().zip(reference).enumerate() {
        if left.op == right.op && left.backward == right.backward {
            continue;
        }
        if left.backward == right.backward && mirrorable(&left.op, &right.op) {
            mirrors += 1;
            findings.push(Divergence {
                kind: "guard-polarity",
                detail: format!(
                    "ordinal={ordinal} candidate={} reference={} offset={:#x}",
                    left.op, right.op, left.offset
                ),
                advice: "the compare's operand order is mirrored; respell the relational guard with swapped operands",
            });
        } else if left.backward == right.backward && complement(&left.op) == Some(right.op.as_str())
        {
            other += 1;
            findings.push(Divergence {
                kind: "guard-inversion",
                detail: format!(
                    "ordinal={ordinal} candidate={} reference={} offset={:#x}",
                    left.op, right.op, left.offset
                ),
                advice:
                    "the guard's sense is inverted; the arms are likely spelled in the other order",
            });
        } else {
            other += 1;
            findings.push(Divergence {
                kind: "branch-shape",
                detail: format!(
                    "ordinal={ordinal} candidate={}{} reference={}{}",
                    left.op,
                    if left.backward { "(back)" } else { "" },
                    right.op,
                    if right.backward { "(back)" } else { "" },
                ),
                advice: "branch direction or condition class differs; the source model around this guard is wrong",
            });
        }
    }
    if mirrors == 0 && other == 0 {
        findings.push(Divergence {
            kind: "branch-structure-equal",
            detail: "every conditional branch matches; the divergence is outside branch structure"
                .into(),
            advice: "inspect block contents rather than control flow",
        });
    }
    findings
}

fn plan(findings: &[Divergence]) -> Option<RepairPlan> {
    let mirrors = findings
        .iter()
        .filter(|finding| finding.kind == "guard-polarity")
        .count();
    let clean = findings
        .iter()
        .all(|finding| matches!(finding.kind, "guard-polarity"));
    if mirrors > 0 && clean {
        Some(RepairPlan::one(Repair::MirrorRelationalGuards))
    } else {
        None
    }
}

/// Structural divergences strong enough to overrule an allocator repair:
/// while guard counts or loop forms disagree, register-role experiments are
/// doctrine-inverted, so the allocator's proposal is suppressed as spurious.
/// Weaker per-branch findings stay evidence-only, because literal-pool words
/// can decode as stray conditionals and must not veto a valid repair.
fn structural(findings: &[Divergence]) -> bool {
    findings
        .iter()
        .any(|finding| matches!(finding.kind, "conditional-count" | "loop-shape"))
}

/// Append structural evidence to an allocator report. The candidate and
/// reference instruction streams are re-read from the scoring work
/// directory, so this covers every owner the scorer covers, including
/// adopted owners whose retained assembly is gone. Structural repair
/// proposals fill an empty report; strong structural findings suppress an
/// allocator proposal that would otherwise search a misaligned stream.
pub fn augment(report: Report, work: &Path) -> Report {
    let streams = (
        rows_of(&work.join("candidate.bin")),
        rows_of(&work.join("reference.bin")),
    );
    let (Ok(candidate), Ok(reference)) = streams else {
        return report;
    };
    let findings = compare(&branches(&candidate), &branches(&reference));
    if report.repair.is_some() && !structural(&findings) {
        return report;
    }
    let mut text = report.text;
    if report.repair.is_some() {
        text += "structure_precedence=allocator-repair-suppressed\n";
    }
    for finding in &findings {
        text += &format!(
            "structure={} {} advice={}\n",
            finding.kind, finding.detail, finding.advice
        );
    }
    match plan(&findings) {
        Some(plan) => {
            text += &format!("structure_repair={}\n", plan.label());
            let dimensions = plan.dimensions();
            Report {
                text,
                dimensions,
                repair: Some(plan),
            }
        }
        None => Report {
            text,
            dimensions: Vec::new(),
            repair: None,
        },
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn branch(offset: u32, op: &str, backward: bool) -> Branch {
        Branch {
            offset,
            op: op.into(),
            backward,
        }
    }

    #[test]
    fn names_a_mirrored_guard_and_proposes_the_repair() {
        let candidate = [branch(4, "bgt", false), branch(20, "bne", true)];
        let reference = [branch(4, "blt", false), branch(20, "bne", true)];
        let findings = compare(&candidate, &reference);
        assert_eq!(findings.len(), 1);
        assert_eq!(findings[0].kind, "guard-polarity");
        let plan = plan(&findings).expect("mirror repair");
        assert_eq!(plan.label(), "mirror_relational_guards");
    }

    #[test]
    fn refuses_a_repair_when_branch_structure_disagrees() {
        let candidate = [branch(4, "beq", false), branch(20, "bne", true)];
        let reference = [branch(4, "beq", false)];
        let findings = compare(&candidate, &reference);
        assert_eq!(findings[0].kind, "conditional-count");
        assert!(plan(&findings).is_none());
    }

    #[test]
    fn names_a_loop_shape_difference_without_a_repair() {
        let candidate = [branch(4, "bne", false), branch(20, "bge", false)];
        let reference = [branch(4, "bne", true), branch(20, "bge", false)];
        let findings = compare(&candidate, &reference);
        assert_eq!(findings[0].kind, "loop-shape");
        assert!(plan(&findings).is_none());
    }

    #[test]
    fn names_an_inverted_guard_without_a_repair() {
        let candidate = [branch(4, "bge", false)];
        let reference = [branch(4, "blt", false)];
        let findings = compare(&candidate, &reference);
        assert_eq!(findings[0].kind, "guard-inversion");
        assert!(plan(&findings).is_none());
    }

    #[test]
    fn stream_rows_recover_offsets_and_widths() {
        // No objdump in unit tests: exercise the branch extraction directly.
        let rows = vec![
            (0_u32, 2_u32, "cmp\tr0, #0".to_string()),
            (2, 2, "bge.n\t0x8".to_string()),
            (4, 4, "bl\t0x4000".to_string()),
            (8, 2, "bx\tlr".to_string()),
        ];
        let branches = branches(&rows);
        assert_eq!(branches.len(), 1);
        assert_eq!(branches[0].op, "bge");
        assert!(!branches[0].backward);
    }

    #[test]
    fn pool_junk_after_the_return_is_not_a_branch() {
        // A literal-pool word that happens to decode as a conditional must
        // not count: it is unreachable from the entry.
        let rows = vec![
            (0_u32, 2_u32, "cmp\tr0, #0".to_string()),
            (2, 2, "bne.n\t0x6".to_string()),
            (4, 2, "pop\t{pc}".to_string()),
            (6, 2, "bx\tlr".to_string()),
            (8, 2, "blt.n\t0x2".to_string()),
        ];
        let branches = branches(&rows);
        assert_eq!(branches.len(), 1);
        assert_eq!(branches[0].op, "bne");
    }
}
