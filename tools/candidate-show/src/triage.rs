use crate::render::{alignment_key, without_pc_offset, without_register};
use crate::topology::Comparison;
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
    fn route(&self) -> (&'static str, Option<&'static str>) {
        match self {
            Self::Exact => ("exact", Some("verify-and-integrate")),
            Self::LayoutOnly => ("layout-only", Some("layout-and-literal-pool")),
            Self::AllocationCovered => ("allocation-covered", Some("decoder-named-repair")),
            Self::AllocationUncovered => ("allocation-uncovered", None),
            Self::SchedulingFloor => ("scheduling-floor", Some("statement-and-evaluation-order")),
            Self::TypeWidthMismatch => {
                ("type-width-mismatch", Some("recover-width-and-signedness"))
            }
            Self::StructuralTopology => ("structural-topology", Some("reconstruct-block-topology")),
            Self::MissingExtraCode => (
                "missing-extra-code",
                Some("reconstruct-missing-or-extra-code"),
            ),
            Self::CompilerUnemittable => ("compiler-unemittable", Some("classification-proof")),
            Self::Unclassified => ("unclassified", None),
        }
    }
    pub fn label(&self) -> &'static str {
        self.route().0
    }
    pub fn playbook(&self) -> Option<&'static str> {
        self.route().1
    }
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct TypeWidthFingerprint {
    pub candidate: String,
    pub reference: String,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "snake_case")]
pub enum TopologyStatus {
    Equal,
    Different,
    Uncovered,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct TopologyEvidence {
    pub status: TopologyStatus,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub reason: Option<String>,
}
impl Default for TopologyEvidence {
    fn default() -> Self {
        Self {
            status: TopologyStatus::Uncovered,
            reason: Some("legacy-verdict".into()),
        }
    }
}
impl From<&Comparison> for TopologyEvidence {
    fn from(value: &Comparison) -> Self {
        match value {
            Comparison::Equal => Self {
                status: TopologyStatus::Equal,
                reason: None,
            },
            Comparison::Different => Self {
                status: TopologyStatus::Different,
                reason: None,
            },
            Comparison::Uncovered(reason) => Self {
                status: TopologyStatus::Uncovered,
                reason: Some(reason.clone()),
            },
        }
    }
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct ResidualFacts {
    pub actual_bytes: usize,
    pub reference_bytes: usize,
    pub differing_halfwords: usize,
    pub branch_topology_equal: bool,
    #[serde(default)]
    pub topology: TopologyEvidence,
    pub type_width_fingerprints: Vec<TypeWidthFingerprint>,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct ResidualReport {
    pub class: ResidualClass,
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
        }
        self
    }
}
fn count_delta(
    left: &[String],
    right: &[String],
    key: impl Copy + Fn(&str) -> String,
) -> BTreeMap<String, i64> {
    let mut result = BTreeMap::new();
    for line in left {
        *result.entry(key(line)).or_default() += 1;
    }
    for line in right {
        *result.entry(key(line)).or_default() -= 1;
    }
    result
}
fn mnemonic(line: &str) -> &str {
    line.split_ascii_whitespace()
        .next()
        .unwrap_or("")
        .trim_end_matches(".n")
        .trim_end_matches(".w")
}
fn direct_branch(op: &str) -> bool {
    const BRANCHES: &[&str] = &[
        "b", "bal", "beq", "bne", "bcs", "bhs", "bcc", "blo", "bmi", "bpl", "bvs", "bvc", "bhi",
        "bls", "bge", "blt", "bgt", "ble",
    ];
    BRANCHES.contains(&op)
}
pub(crate) fn alignment_indices(
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
fn aligned_slots(
    left_len: usize,
    right_len: usize,
    pairs: &[(Option<usize>, Option<usize>)],
) -> (Vec<usize>, Vec<usize>) {
    let mut left_slots = vec![0; left_len];
    let mut right_slots = vec![0; right_len];
    for (slot, (left, right)) in pairs.iter().enumerate() {
        if let Some(index) = *left {
            left_slots[index] = slot;
        }
        if let Some(index) = *right {
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
                Some(target) => offsets
                    .iter()
                    .position(|offset| *offset as u64 == target)
                    .map(|target| format!("node:{}", slots[target]))
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
fn branch_topology_equal(
    left: &[String],
    right: &[String],
    pairs: &[(Option<usize>, Option<usize>)],
) -> bool {
    let (left_slots, right_slots) = aligned_slots(left.len(), right.len(), pairs);
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
    let pairs = alignment_indices(left, right, |left, right| {
        usize::from(alignment_key(left) == alignment_key(right))
    });
    let topology = if branch_topology_equal(left, right, &pairs) {
        Comparison::Equal
    } else {
        Comparison::Different
    };
    classify_with_topology(
        left,
        right,
        actual_bytes,
        reference_bytes,
        differing_halfwords,
        &topology,
    )
}

pub fn classify_with_topology(
    left: &[String],
    right: &[String],
    actual_bytes: usize,
    reference_bytes: usize,
    differing_halfwords: usize,
    topology: &Comparison,
) -> ResidualReport {
    let ordered_pc_normalized_equal = left
        .iter()
        .map(|line| normalized(line))
        .eq(right.iter().map(|line| normalized(line)));
    let register_erased_ordered_equal = left
        .iter()
        .map(|line| register_erased(line))
        .eq(right.iter().map(|line| register_erased(line)));
    let pairs = alignment_indices(left, right, |left, right| {
        usize::from(alignment_key(left) == alignment_key(right))
    });
    let streams_differ = pairs
        .iter()
        .any(|pair| pair.0.is_none() || pair.1.is_none());
    let width = type_width_fingerprints(left, right);
    let instruction_delta = count_delta(left, right, normalized);
    let register_delta = count_delta(left, right, register_erased);
    let instruction_multiset_equal = instruction_delta.values().all(|count| *count == 0);
    let register_erased_multiset_equal = register_delta.values().all(|count| *count == 0);
    let pool = instruction_delta.values().map(|value| value.abs()).sum();
    let branch_topology_equal = matches!(topology, Comparison::Equal);
    let branch_topology_different = matches!(topology, Comparison::Different);
    let facts = ResidualFacts {
        actual_bytes,
        reference_bytes,
        differing_halfwords,
        branch_topology_equal,
        topology: TopologyEvidence::from(topology),
        type_width_fingerprints: width,
    };
    let class = if differing_halfwords == 0 && actual_bytes == reference_bytes {
        ResidualClass::Exact
    } else if ordered_pc_normalized_equal {
        ResidualClass::LayoutOnly
    } else if right.iter().any(|line| multiple(line)) && !left.iter().any(|line| multiple(line)) {
        ResidualClass::CompilerUnemittable
    } else if register_erased_ordered_equal {
        ResidualClass::AllocationUncovered
    } else if instruction_multiset_equal {
        ResidualClass::SchedulingFloor
    } else if register_erased_multiset_equal {
        ResidualClass::AllocationUncovered
    } else if !facts.type_width_fingerprints.is_empty() && facts.branch_topology_equal {
        ResidualClass::TypeWidthMismatch
    } else if branch_topology_different {
        ResidualClass::StructuralTopology
    } else if streams_differ || actual_bytes != reference_bytes {
        ResidualClass::MissingExtraCode
    } else {
        ResidualClass::Unclassified
    };
    ResidualReport {
        class,
        wrong_instructions: pool,
        facts,
    }
}
#[cfg(test)]
mod tests {
    use super::*;
    fn triage(left: &[&str], right: &[&str], differing: usize) -> ResidualReport {
        let left = left.iter().map(ToString::to_string).collect::<Vec<_>>();
        let right = right.iter().map(ToString::to_string).collect::<Vec<_>>();
        classify(&left, &right, left.len() * 2, right.len() * 2, differing)
    }
    fn assert_route(left: &[&str], right: &[&str], differing: usize, class: ResidualClass) {
        assert_eq!(triage(left, right, differing).class, class);
    }
    #[test]
    fn routes_each_mechanical_residual() {
        use ResidualClass::*;
        assert_route(&["movs r0, #1"], &["movs r0, #1"], 0, Exact);
        assert_route(&["ldr r0, [pc, #4]"], &["ldr r0, [pc, #8]"], 1, LayoutOnly);
        assert_route(
            &["adds r0, r1, #0"],
            &["adds r2, r3, #0"],
            1,
            AllocationUncovered,
        );
        assert_route(
            &["movs r0, #1", "movs r1, #2"],
            &["movs r1, #2", "movs r0, #1"],
            2,
            SchedulingFloor,
        );
        assert_route(
            &["ldrb r0, [r1, #0]"],
            &["ldrsb r0, [r1, #0]"],
            1,
            TypeWidthMismatch,
        );
        assert_route(&["beq 0x4"], &["bne 0x4"], 1, StructuralTopology);
        assert_route(
            &["movs r0, #1"],
            &["movs r0, #1", "str r0, [r1]"],
            1,
            MissingExtraCode,
        );
    }
    #[test]
    fn branch_topology_tracks_targets_across_alignment() {
        let report = triage(
            &["beq 0x4", "movs r0, #1", "movs r1, #2", "bx lr"],
            &["beq 0x6", "movs r0, #1", "movs r1, #2", "bx lr"],
            1,
        );
        assert!(!report.facts.branch_topology_equal);
        assert_eq!(report.class, ResidualClass::StructuralTopology);
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
        assert_eq!(report.facts.type_width_fingerprints.len(), 1);
        assert_eq!(report.facts.type_width_fingerprints[0].candidate, "ldrb");
        assert_eq!(report.facts.type_width_fingerprints[0].reference, "ldrsb");
        assert_eq!(report.class, ResidualClass::TypeWidthMismatch);
    }

    #[test]
    fn uncovered_topology_never_routes_as_structural() {
        let left = vec!["beq 0x4".to_string()];
        let right = vec!["bne 0x4".to_string()];
        let report = classify_with_topology(
            &left,
            &right,
            2,
            2,
            1,
            &Comparison::Uncovered("fixture".into()),
        );
        assert_ne!(report.class, ResidualClass::StructuralTopology);
        assert!(!report.facts.branch_topology_equal);
        let value = serde_json::to_value(&report).unwrap();
        assert_eq!(value["facts"]["topology"]["status"], "uncovered");
        assert_eq!(value["facts"]["topology"]["reason"], "fixture");
    }
}
