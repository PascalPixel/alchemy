use crate::render::{align_streams, alignment_key, without_pc_offset, without_register};
use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "snake_case")]
pub enum ResidualClass {
    Exact,
    LayoutOnly,
    AllocationCovered,
    AllocationUncovered,
    SchedulingFloor,
    TypeWidthMismatch,
    StructuralTopology,
    MissingExtraCode,
    CompilerUnemittable,
    Unclassified,
}
impl ResidualClass {
    pub fn label(&self) -> &'static str {
        match self {
            Self::Exact => "exact",
            Self::LayoutOnly => "layout-only",
            Self::AllocationCovered => "allocation-covered",
            Self::AllocationUncovered => "allocation-uncovered",
            Self::SchedulingFloor => "scheduling-floor",
            Self::TypeWidthMismatch => "type-width-mismatch",
            Self::StructuralTopology => "structural-topology",
            Self::MissingExtraCode => "missing-extra-code",
            Self::CompilerUnemittable => "compiler-unemittable",
            Self::Unclassified => "unclassified",
        }
    }
    pub fn playbook(&self) -> Option<&'static str> {
        match self {
            Self::Exact => Some("verify-and-integrate"),
            Self::LayoutOnly => Some("layout-and-literal-pool"),
            Self::AllocationCovered => Some("decoder-named-repair"),
            Self::AllocationUncovered => None,
            Self::SchedulingFloor => Some("statement-and-evaluation-order"),
            Self::TypeWidthMismatch => Some("recover-width-and-signedness"),
            Self::StructuralTopology => Some("reconstruct-block-topology"),
            Self::MissingExtraCode => Some("reconstruct-missing-or-extra-code"),
            Self::CompilerUnemittable => Some("classification-proof"),
            Self::Unclassified => None,
        }
    }
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct TypeWidthFingerprint {
    pub candidate: String,
    pub reference: String,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct ResidualFacts {
    pub actual_bytes: usize,
    pub reference_bytes: usize,
    pub differing_halfwords: usize,
    pub candidate_instructions: usize,
    pub reference_instructions: usize,
    pub ordered_pc_normalized_equal: bool,
    pub instruction_multiset_equal: bool,
    pub register_erased_ordered_equal: bool,
    pub register_erased_multiset_equal: bool,
    pub branch_topology_equal: bool,
    pub insertions: usize,
    pub deletions: usize,
    pub type_width_fingerprints: Vec<TypeWidthFingerprint>,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct ResidualReport {
    pub class: ResidualClass,
    pub playbook: Option<String>,
    pub wrong_instructions: i64,
    pub facts: ResidualFacts,
}
impl ResidualReport {
    pub fn with_decoder_coverage(mut self, covered: bool) -> Self {
        if matches!(
            self.class,
            ResidualClass::AllocationCovered | ResidualClass::AllocationUncovered
        ) {
            self.class = if covered {
                ResidualClass::AllocationCovered
            } else {
                ResidualClass::AllocationUncovered
            };
            self.playbook = self.class.playbook().map(str::to_string);
        }
        self
    }
}
fn counts(lines: &[String], key: impl Fn(&str) -> String) -> BTreeMap<String, i64> {
    let mut result = BTreeMap::new();
    for line in lines {
        *result.entry(key(line)).or_default() += 1;
    }
    result
}
fn balanced(left: &[String], right: &[String], key: impl Copy + Fn(&str) -> String) -> bool {
    counts(left, key) == counts(right, key)
}
fn mnemonic(line: &str) -> &str {
    line.split_ascii_whitespace()
        .next()
        .unwrap_or("")
        .trim_end_matches(".n")
        .trim_end_matches(".w")
}
fn direct_branch(op: &str) -> bool {
    matches!(
        op,
        "b" | "bal"
            | "beq"
            | "bne"
            | "bcs"
            | "bhs"
            | "bcc"
            | "blo"
            | "bmi"
            | "bpl"
            | "bvs"
            | "bvc"
            | "bhi"
            | "bls"
            | "bge"
            | "blt"
            | "bgt"
            | "ble"
    )
}
fn alignment_indices(
    left: &[String],
    right: &[String],
    score: impl Copy + Fn(&str, &str) -> usize,
) -> Vec<(Option<usize>, Option<usize>)> {
    let mut table = vec![vec![0; right.len() + 1]; left.len() + 1];
    for i in (0..left.len()).rev() {
        for j in (0..right.len()).rev() {
            let pair = score(&left[i], &right[j]);
            let diagonal = (pair != 0).then(|| table[i + 1][j + 1] + pair);
            table[i][j] = diagonal
                .unwrap_or_default()
                .max(table[i + 1][j])
                .max(table[i][j + 1]);
        }
    }
    let (mut i, mut j) = (0, 0);
    let mut pairs = Vec::new();
    while i < left.len() && j < right.len() {
        let pair = score(&left[i], &right[j]);
        if pair != 0 && table[i][j] == table[i + 1][j + 1] + pair {
            pairs.push((Some(i), Some(j)));
            i += 1;
            j += 1;
        } else if table[i + 1][j] >= table[i][j + 1] {
            pairs.push((Some(i), None));
            i += 1;
        } else {
            pairs.push((None, Some(j)));
            j += 1;
        }
    }
    pairs.extend((i..left.len()).map(|index| (Some(index), None)));
    pairs.extend((j..right.len()).map(|index| (None, Some(index))));
    pairs
}
fn aligned_slots(left: &[String], right: &[String]) -> (Vec<usize>, Vec<usize>) {
    let pairs = alignment_indices(left, right, |left, right| {
        usize::from(alignment_key(left) == alignment_key(right))
    });
    let mut left_slots = vec![0; left.len()];
    let mut right_slots = vec![0; right.len()];
    for (slot, (left, right)) in pairs.into_iter().enumerate() {
        if let Some(index) = left {
            left_slots[index] = slot;
        }
        if let Some(index) = right {
            right_slots[index] = slot;
        }
    }
    (left_slots, right_slots)
}
fn instruction_width(line: &str) -> usize {
    let raw = line.split_ascii_whitespace().next().unwrap_or("");
    match mnemonic(line) {
        "bl" => 4,
        "blx"
            if line.split_ascii_whitespace().nth(1).is_some_and(|arg| {
                arg.trim_end_matches(',')
                    .strip_prefix('r')
                    .is_some_and(|register| register.parse::<u8>().is_ok())
            }) =>
        {
            2
        }
        "blx" => 4,
        _ if raw.ends_with(".w") => 4,
        ".word" => 4,
        _ => 2,
    }
}
fn instruction_offsets(lines: &[String]) -> (Vec<usize>, usize) {
    let mut offset = 0;
    let offsets = lines
        .iter()
        .map(|line| {
            let current = offset;
            offset += instruction_width(line);
            current
        })
        .collect();
    (offsets, offset)
}
fn branch_target(line: &str) -> Option<u64> {
    let target = line.split_ascii_whitespace().nth(1)?;
    let target = target
        .trim_start_matches('#')
        .trim_end_matches(',')
        .trim_start_matches("0x");
    u64::from_str_radix(target, 16).ok()
}
fn branch_edges(lines: &[String], slots: &[usize]) -> Vec<String> {
    let (offsets, extent) = instruction_offsets(lines);
    let instruction_at = offsets
        .iter()
        .enumerate()
        .map(|(index, offset)| (*offset as u64, index))
        .collect::<BTreeMap<_, _>>();
    lines
        .iter()
        .enumerate()
        .filter_map(|(index, line)| {
            let op = mnemonic(line);
            if !direct_branch(op) {
                return None;
            }
            let target = match branch_target(line) {
                Some(target) if target == extent as u64 => "exit".to_string(),
                Some(target) => instruction_at
                    .get(&target)
                    .map(|target| format!("node:{}", slots[*target]))
                    .unwrap_or_else(|| format!("external:{target:x}")),
                None => format!(
                    "symbol:{}",
                    line.split_ascii_whitespace()
                        .skip(1)
                        .collect::<Vec<_>>()
                        .join(" ")
                ),
            };
            Some(format!("node:{}:{op}->{target}", slots[index]))
        })
        .collect()
}
fn branch_topology_equal(left: &[String], right: &[String]) -> bool {
    let (left_slots, right_slots) = aligned_slots(left, right);
    branch_edges(left, &left_slots) == branch_edges(right, &right_slots)
}
fn multiple(line: &str) -> bool {
    line.split(|character: char| !character.is_ascii_alphanumeric())
        .any(|word| matches!(word, "stmia" | "ldmia" | "stmdb" | "ldmdb" | "stm" | "ldm"))
}
fn width_family(op: &str) -> Option<&'static str> {
    match op {
        "ldr" | "ldrb" | "ldrh" | "ldrsb" | "ldrsh" => Some("load"),
        "str" | "strb" | "strh" => Some("store"),
        "lsls" | "lsrs" | "asrs" => Some("subregister"),
        _ => None,
    }
}
fn operands_without_registers(line: &str) -> String {
    let normalized = without_register(&without_pc_offset(line));
    normalized
        .split_ascii_whitespace()
        .skip(1)
        .collect::<Vec<_>>()
        .join(" ")
}
fn type_width_pair(left: &str, right: &str) -> bool {
    let (left_op, right_op) = (mnemonic(left), mnemonic(right));
    left_op != right_op
        && width_family(left_op).is_some()
        && width_family(left_op) == width_family(right_op)
        && operands_without_registers(left) == operands_without_registers(right)
}
fn type_width_fingerprints(left: &[String], right: &[String]) -> Vec<TypeWidthFingerprint> {
    alignment_indices(left, right, |left, right| {
        if alignment_key(left) == alignment_key(right) {
            2
        } else if type_width_pair(left, right) {
            1
        } else {
            0
        }
    })
    .into_iter()
    .filter_map(|(left_index, right_index)| {
        let left = left.get(left_index?)?;
        let right = right.get(right_index?)?;
        type_width_pair(left, right).then(|| TypeWidthFingerprint {
            candidate: mnemonic(left).to_string(),
            reference: mnemonic(right).to_string(),
        })
    })
    .collect()
}
fn normalized(line: &str) -> String {
    without_pc_offset(line)
}
fn register_erased(line: &str) -> String {
    without_register(&without_pc_offset(line))
}
pub fn classify(
    left: &[String],
    right: &[String],
    actual_bytes: usize,
    reference_bytes: usize,
    differing_halfwords: usize,
) -> ResidualReport {
    let left_pc = left.iter().map(|line| normalized(line)).collect::<Vec<_>>();
    let right_pc = right
        .iter()
        .map(|line| normalized(line))
        .collect::<Vec<_>>();
    let left_regs = left
        .iter()
        .map(|line| register_erased(line))
        .collect::<Vec<_>>();
    let right_regs = right
        .iter()
        .map(|line| register_erased(line))
        .collect::<Vec<_>>();
    let pairs = align_streams(left, right);
    let insertions = pairs.iter().filter(|(left, _)| left.is_none()).count();
    let deletions = pairs.iter().filter(|(_, right)| right.is_none()).count();
    let width = type_width_fingerprints(left, right);
    let instruction_multiset_equal = balanced(left, right, normalized);
    let register_erased_multiset_equal = balanced(left, right, register_erased);
    let pool = {
        let mut values = counts(left, normalized);
        for (key, value) in counts(right, normalized) {
            *values.entry(key).or_default() -= value;
        }
        values.values().map(|value| value.abs()).sum()
    };
    let branch_topology_equal = branch_topology_equal(left, right);
    let facts = ResidualFacts {
        actual_bytes,
        reference_bytes,
        differing_halfwords,
        candidate_instructions: left.len(),
        reference_instructions: right.len(),
        ordered_pc_normalized_equal: left_pc == right_pc,
        instruction_multiset_equal,
        register_erased_ordered_equal: left_regs == right_regs,
        register_erased_multiset_equal,
        branch_topology_equal,
        insertions,
        deletions,
        type_width_fingerprints: width,
    };
    let class = if differing_halfwords == 0 && actual_bytes == reference_bytes {
        ResidualClass::Exact
    } else if facts.ordered_pc_normalized_equal {
        ResidualClass::LayoutOnly
    } else if right.iter().any(|line| multiple(line)) && !left.iter().any(|line| multiple(line)) {
        ResidualClass::CompilerUnemittable
    } else if facts.register_erased_ordered_equal {
        ResidualClass::AllocationUncovered
    } else if facts.instruction_multiset_equal {
        ResidualClass::SchedulingFloor
    } else if facts.register_erased_multiset_equal {
        ResidualClass::AllocationUncovered
    } else if !facts.type_width_fingerprints.is_empty() && facts.branch_topology_equal {
        ResidualClass::TypeWidthMismatch
    } else if !facts.branch_topology_equal {
        ResidualClass::StructuralTopology
    } else if facts.insertions != 0 || facts.deletions != 0 || actual_bytes != reference_bytes {
        ResidualClass::MissingExtraCode
    } else {
        ResidualClass::Unclassified
    };
    ResidualReport {
        playbook: class.playbook().map(str::to_string),
        class,
        wrong_instructions: pool,
        facts,
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    fn triage(left: &[&str], right: &[&str], differing: usize) -> ResidualReport {
        classify(
            &left.iter().map(|line| line.to_string()).collect::<Vec<_>>(),
            &right
                .iter()
                .map(|line| line.to_string())
                .collect::<Vec<_>>(),
            left.len() * 2,
            right.len() * 2,
            differing,
        )
    }
    #[test]
    fn routes_each_mechanical_residual() {
        assert_eq!(
            triage(&["movs r0, #1"], &["movs r0, #1"], 0).class,
            ResidualClass::Exact
        );
        assert_eq!(
            triage(&["ldr r0, [pc, #4]"], &["ldr r0, [pc, #8]"], 1).class,
            ResidualClass::LayoutOnly
        );
        assert_eq!(
            triage(&["adds r0, r1, #0"], &["adds r2, r3, #0"], 1).class,
            ResidualClass::AllocationUncovered
        );
        assert_eq!(
            triage(
                &["movs r0, #1", "movs r1, #2"],
                &["movs r1, #2", "movs r0, #1"],
                2
            )
            .class,
            ResidualClass::SchedulingFloor
        );
        assert_eq!(
            triage(&["ldrb r0, [r1, #0]"], &["ldrsb r0, [r1, #0]"], 1).class,
            ResidualClass::TypeWidthMismatch
        );
        assert_eq!(
            triage(&["beq 0x4"], &["bne 0x4"], 1).class,
            ResidualClass::StructuralTopology
        );
        assert_eq!(
            triage(&["movs r0, #1"], &["movs r0, #1", "str r0, [r1]"], 1).class,
            ResidualClass::MissingExtraCode
        );
    }
    #[test]
    fn branch_topology_distinguishes_same_mnemonic_with_different_target() {
        let report = triage(
            &["beq 0x4", "movs r0, #1", "movs r1, #2", "bx lr"],
            &["beq 0x6", "movs r0, #1", "movs r1, #2", "bx lr"],
            1,
        );
        assert!(!report.facts.branch_topology_equal);
        assert_eq!(report.class, ResidualClass::StructuralTopology);
    }
    #[test]
    fn branch_topology_normalizes_targets_across_an_inserted_instruction() {
        let report = triage(
            &["beq 0x4", "movs r0, #1", "bx lr"],
            &["beq 0x6", "nop", "movs r0, #1", "bx lr"],
            2,
        );
        assert!(report.facts.branch_topology_equal);
    }
    #[test]
    fn type_width_fingerprint_uses_aligned_pairs_after_an_insertion() {
        let report = triage(
            &["nop", "ldrb r0, [r1, #0]", "bx lr"],
            &["ldrsb r0, [r1, #0]", "bx lr"],
            2,
        );
        assert_eq!(
            report.facts.type_width_fingerprints,
            vec![TypeWidthFingerprint {
                candidate: "ldrb".to_string(),
                reference: "ldrsb".to_string(),
            }]
        );
        assert_eq!(report.class, ResidualClass::TypeWidthMismatch);
    }
}
