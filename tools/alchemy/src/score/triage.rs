use psynergy::compare::topology::Comparison;
use psynergy::compare::{
    alignment_indices,
    insns::{alignment_key, without_pc_offset, without_register},
};
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
    InstructionSelection,
    FrameContext,
    CallTargetMismatch,
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
            Self::InstructionSelection => ("instruction-selection", Some("name-the-wall")),
            Self::FrameContext => ("frame-context", Some("recover-stack-local-context")),
            Self::CallTargetMismatch => ("call-target-mismatch", None),
            Self::Unclassified => ("unclassified", None),
        }
    }
    pub fn label(&self) -> &'static str {
        self.route().0
    }
    pub fn playbook(&self) -> Option<&'static str> {
        self.route().1
    }
    /// Next command; {source} is the candidate path, {owner} the qualified owner.
    pub fn next_command(&self) -> &'static str {
        match self {
            Self::Exact => "alchemy adopt {owner} --source {source} (complete-owner and production verification still required)",
            Self::LayoutOnly => "alchemy score {source} --align (layout drift: inspect pools and padding, not code)",
            Self::AllocationCovered => "alchemy match {source} (the decoder names a catalogued repair; do not hand-edit first)",
            Self::AllocationUncovered => {
                "route to the smart queue; do not probe register roles by respelling source (measured: such probes regress)"
            }
            Self::SchedulingFloor => {
                "alchemy build allocator {owner}, then psynergy inspect allocator DUMP_DIR (read the scheduling decision; no automatic repair is established)"
            }
            Self::TypeWidthMismatch => {
                "alchemy score {source} --align (recover widths and signedness from the access-width evidence)"
            }
            Self::StructuralTopology => {
                "smart queue: reconstruct the divergent blocks in the complete translation unit; only a decoder-named repair may enter search"
            }
            Self::InstructionSelection => {
                "read the residual= and wall= lines: same code and size, instructions selected differently; no lever fires for a measured wall, record it instead of respelling"
            }
            Self::MissingExtraCode => {
                "smart queue: audit the complete owner extent and translation-unit membership before reconstructing missing or extra statements"
            }
            Self::FrameContext => {
                "alchemy score {source} --asm (build a stack-slot ledger and recover the missing local or translation-unit context)"
            }
            Self::Unclassified => "route to the smart queue with the full --align diff attached",
            Self::CallTargetMismatch => "smart queue: inspect call bindings and veneers in the full --align diff; no automatic repair",
        }
    }
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct TypeWidthFingerprint {
    pub candidate: String,
    pub reference: String,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct RepairHint {
    pub signal: String,
    pub playbook: String,
    pub detail: String,
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
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub repair_hints: Vec<RepairHint>,
    #[serde(default)]
    pub census: ResidualCensus,
}
/// The shapes of the unpaired runs between aligned instructions, named so a
/// reader knows which residual class an owner is in without reading the
/// listing, and which of them are measured walls.
#[derive(Clone, Debug, Default, Deserialize, Eq, PartialEq, Serialize)]
pub struct ResidualCensus {
    pub runs: usize,
    pub register_only: usize,
    pub order_only: usize,
    pub pool_versus_immediate: usize,
    pub copy_versus_rematerialise: usize,
    pub pool_placement: usize,
    pub other: usize,
}
impl ResidualCensus {
    pub fn line(&self) -> String {
        format!(
            "residual=runs:{} register_only:{} order_only:{} pool_versus_immediate:{} copy_versus_rematerialise:{} pool_placement:{} other:{}",
            self.runs, self.register_only, self.order_only, self.pool_versus_immediate,
            self.copy_versus_rematerialise, self.pool_placement, self.other
        )
    }
    /// Measured boundaries, so nobody spends the hours finding them again.
    pub fn walls(&self) -> Vec<&'static str> {
        let mut walls = Vec::new();
        if self.copy_versus_rematerialise > 0 {
            walls.push("copy_versus_rematerialise: whole-function constant sharing is decided after CSE and allocation; four source spellings on two owners moved nothing (380:02000f8c, 374). Stop; record the boundary.");
        }
        if self.pool_placement > 0 {
            walls.push("pool_placement: both sides pool the value and only the pool split point differs; no source lever is known. Stop; record the boundary.");
        }
        walls
    }
}
fn is_pool_load(line: &str) -> bool {
    mnemonic(line) == "ldr" && line.contains("[pc")
}
fn is_synthesis(line: &str) -> bool {
    matches!(mnemonic(line), "movs" | "lsls" | "adds") && line.contains('#')
}
fn is_register_copy(line: &str) -> bool {
    mnemonic(line) == "mov" && !line.contains('#') && !line.contains('[')
}
/// Buckets every unpaired run of the alignment by shape.
fn residual_census(
    left: &[String],
    right: &[String],
    pairs: &[(Option<usize>, Option<usize>)],
) -> ResidualCensus {
    let mut census = ResidualCensus::default();
    let mut run: (Vec<&str>, Vec<&str>) = (Vec::new(), Vec::new());
    let close = |run: &mut (Vec<&str>, Vec<&str>), census: &mut ResidualCensus| {
        if run.0.is_empty() && run.1.is_empty() {
            return;
        }
        census.runs += 1;
        let (a, b) = (&run.0, &run.1);
        let sorted = |xs: &[&str], key: fn(&str) -> String| {
            let mut v: Vec<String> = xs.iter().map(|x| key(x)).collect();
            v.sort();
            v
        };
        let shape = if a.len() == b.len()
            && a.iter()
                .zip(b)
                .all(|(x, y)| register_erased(x) == register_erased(y))
        {
            &mut census.register_only
        } else if a.len() == b.len() && sorted(a, normalized) == sorted(b, normalized) {
            &mut census.order_only
        } else if !a.is_empty()
            && !b.is_empty()
            && a.iter().all(|x| is_pool_load(x))
            && b.iter().all(|x| is_pool_load(x))
        {
            &mut census.pool_placement
        } else if (a.iter().any(|x| is_pool_load(x)) && b.iter().all(|x| is_synthesis(x)))
            || (b.iter().any(|x| is_pool_load(x)) && a.iter().all(|x| is_synthesis(x)))
        {
            &mut census.pool_versus_immediate
        } else if (a.is_empty() || b.is_empty())
            && a.iter()
                .chain(b.iter())
                .all(|x| is_synthesis(x) || is_register_copy(x) || is_pool_load(x))
        {
            // A constant produced at a different program point: one side
            // holds it in a register from an earlier materialisation, the
            // other rematerialises it where it is used. Decided by CSE and
            // allocation over the whole function; no source lever reaches it.
            &mut census.copy_versus_rematerialise
        } else {
            &mut census.other
        };
        *shape += 1;
        run.0.clear();
        run.1.clear();
    };
    for (l, r) in pairs {
        match (l, r) {
            (Some(i), Some(j)) if alignment_key(&left[*i]) == alignment_key(&right[*j]) => {
                close(&mut run, &mut census)
            }
            (Some(i), None) => run.0.push(&left[*i]),
            (None, Some(j)) => run.1.push(&right[*j]),
            (Some(i), Some(j)) => {
                run.0.push(&left[*i]);
                run.1.push(&right[*j]);
            }
            (None, None) => {}
        }
    }
    close(&mut run, &mut census);
    census
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
fn stack_adjustment(line: &str) -> bool {
    let mut fields = line.split_ascii_whitespace();
    matches!(fields.next(), Some("sub" | "add"))
        && fields.next().is_some_and(|field| field == "sp,")
        && fields.next().is_some_and(|field| field.starts_with('#'))
}
fn frame_context_only(left: &[String], right: &[String]) -> bool {
    left.len() == right.len()
        && left.iter().zip(right).all(|(left, right)| {
            normalized(left) == normalized(right)
                || (stack_adjustment(left)
                    && stack_adjustment(right)
                    && mnemonic(left) == mnemonic(right))
        })
        && left
            .iter()
            .zip(right)
            .any(|(left, right)| normalized(left) != normalized(right))
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
fn opposite_condition(left: &str, right: &str) -> bool {
    matches!(
        (left, right),
        ("beq", "bne")
            | ("bne", "beq")
            | ("bcs", "bcc")
            | ("bcc", "bcs")
            | ("bhs", "blo")
            | ("blo", "bhs")
            | ("bmi", "bpl")
            | ("bpl", "bmi")
            | ("bvs", "bvc")
            | ("bvc", "bvs")
            | ("bhi", "bls")
            | ("bls", "bhi")
            | ("bge", "blt")
            | ("blt", "bge")
            | ("bgt", "ble")
            | ("ble", "bgt")
    )
}
fn branch_polarity_hints(left: &[String], right: &[String]) -> Vec<RepairHint> {
    let branches = |lines: &[String]| {
        lines
            .iter()
            .filter_map(|line| {
                let op = mnemonic(line);
                direct_branch(op).then(|| (op.to_string(), branch_target(line)))
            })
            .collect::<Vec<_>>()
    };
    let left = branches(left);
    let right = branches(right);
    if left.len() != right.len() {
        return Vec::new();
    }
    left.iter()
        .zip(right)
        .enumerate()
        .filter(|(_, ((left_op, left_target), (right_op, right_target)))| {
            left_target.is_some()
                && left_target == right_target
                && opposite_condition(left_op, right_op)
        })
        .map(|(index, ((left_op, _), (right_op, _)))| RepairHint {
            signal: "opposite-condition-same-target".into(),
            playbook: "invert-source-condition".into(),
            detail: format!(
                "branch {} is {left_op} in the candidate and {right_op} in the reference with the same destination",
                index + 1
            ),
        })
        .collect()
}
fn load_without_destination(line: &str) -> Option<String> {
    let op = mnemonic(line);
    if !matches!(op, "ldr" | "ldrb" | "ldrh" | "ldrsb" | "ldrsh") {
        return None;
    }
    let compact = line.replace(' ', "");
    if compact.contains("[pc,") || compact.contains("[sp,") {
        return None;
    }
    Some(register_erased(line))
}
fn reload_after_call_hints(left: &[String], right: &[String]) -> Vec<RepairHint> {
    let mut hints = Vec::new();
    for (index, line) in right.iter().enumerate() {
        let Some(load) = load_without_destination(line) else {
            continue;
        };
        let repeated_before_call = right[..index].iter().enumerate().any(|(earlier, line)| {
            load_without_destination(line).as_deref() == Some(load.as_str())
                && right[earlier + 1..index]
                    .iter()
                    .any(|middle| mnemonic(middle) == "bl")
        });
        if !repeated_before_call {
            continue;
        }
        let reference_count = right
            .iter()
            .filter(|line| load_without_destination(line).as_deref() == Some(load.as_str()))
            .count();
        let candidate_count = left
            .iter()
            .filter(|line| load_without_destination(line).as_deref() == Some(load.as_str()))
            .count();
        if candidate_count + 1 == reference_count
            && !hints
                .iter()
                .any(|hint: &RepairHint| hint.detail.contains(&load))
        {
            hints.push(RepairHint {
                signal: "reference-reloads-memory-after-call".into(),
                playbook: "reload-memory-derived-value-across-call".into(),
                detail: format!(
                    "reference emits {reference_count} `{load}` loads around a call; candidate emits {candidate_count}"
                ),
            });
        }
    }
    hints
}
fn repair_hints(
    left: &[String],
    right: &[String],
    actual_bytes: usize,
    reference_bytes: usize,
    topology: &Comparison,
) -> Vec<RepairHint> {
    let mut hints = branch_polarity_hints(left, right);
    if actual_bytes == reference_bytes && matches!(topology, Comparison::Equal) {
        hints.extend(reload_after_call_hints(left, right));
    }
    hints
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
        repair_hints: repair_hints(left, right, actual_bytes, reference_bytes, topology),
        census: residual_census(left, right, &pairs),
    };
    let class = if differing_halfwords == 0 && actual_bytes == reference_bytes {
        ResidualClass::Exact
    } else if ordered_pc_normalized_equal {
        ResidualClass::LayoutOnly
    } else if frame_context_only(left, right) {
        ResidualClass::FrameContext
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
    } else if streams_differ && actual_bytes == reference_bytes && branch_topology_equal {
        // Same code, same size, same shape: instructions were selected or
        // placed differently. Not missing code; the census names the runs.
        ResidualClass::InstructionSelection
    } else if streams_differ || actual_bytes != reference_bytes {
        ResidualClass::MissingExtraCode
    } else if branch_topology_equal
        && left.iter().zip(right).any(|(a, b)| {
            mnemonic(a) == "bl"
                && mnemonic(b) == "bl"
                && branch_target(a)
                    .zip(branch_target(b))
                    .is_some_and(|(a, b)| a != b)
        })
    {
        ResidualClass::CallTargetMismatch
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
    fn census_names_moved_constants_and_selection_stays_distinct_from_missing_code() {
        use ResidualClass::*;
        // The measured wall on 380:02000f8c: the candidate hoists movs r5,#0
        // early and the reference rematerialises movs r5,#1 late.
        let left = [
            "bl 0x20",
            "movs r5, #0",
            "ldr r3, [r6, #12]",
            "adds r0, #35",
            "ldrb r3, [r0, #0]",
            "orrs r5, r3",
        ];
        let right = [
            "bl 0x20",
            "ldr r3, [r6, #12]",
            "adds r0, #35",
            "ldrb r3, [r0, #0]",
            "movs r5, #1",
            "orrs r5, r3",
        ];
        let report = triage(&left, &right, 4);
        assert_eq!(report.class, InstructionSelection);
        assert_eq!(report.facts.census.copy_versus_rematerialise, 2);
        assert_eq!(report.facts.census.other, 0);
        assert_eq!(report.facts.census.walls().len(), 1);
        // A pool load against an inline synthesis of the same value.
        let left = ["ldr r1, [pc, #8]", "cmp r0, r1"];
        let right = [
            "movs r1, #159",
            "lsls r1, r1, #1",
            "adds r1, #255",
            "cmp r0, r1",
        ];
        let report = triage(&left, &right, 3);
        assert_eq!(report.facts.census.pool_versus_immediate, 1);
        assert_eq!(
            report.class, MissingExtraCode,
            "sizes differ, so not selection"
        );
    }

    #[test]
    fn routes_each_mechanical_residual() {
        use ResidualClass::*;
        assert_route(&["bl 0x20"], &["bl 0x24"], 1, CallTargetMismatch);
        assert_route(&["movs r0, #1"], &["movs r0, #2"], 1, Unclassified);
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
        assert_route(
            &["sub sp, #8", "str r0, [sp, #0]", "add sp, #8"],
            &["sub sp, #16", "str r0, [sp, #0]", "add sp, #16"],
            2,
            FrameContext,
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
    fn names_an_inverted_source_condition_with_the_same_destination() {
        let report = triage(
            &["cmp r0, #0", "bne 0x6", "movs r0, #1"],
            &["cmp r0, #0", "beq 0x6", "movs r0, #1"],
            1,
        );
        assert_eq!(report.facts.repair_hints.len(), 1);
        assert_eq!(
            report.facts.repair_hints[0].signal,
            "opposite-condition-same-target"
        );
        assert_eq!(
            report.facts.repair_hints[0].playbook,
            "invert-source-condition"
        );
    }

    #[test]
    fn names_a_reference_reload_separated_by_a_call() {
        let report = triage(
            &["ldrh r0, [r4, #2]", "bl 0x20", "adds r1, r0, #0", "bx lr"],
            &["ldrh r0, [r4, #2]", "bl 0x20", "ldrh r1, [r4, #2]", "bx lr"],
            2,
        );
        assert_eq!(report.facts.repair_hints.len(), 1);
        assert_eq!(
            report.facts.repair_hints[0].signal,
            "reference-reloads-memory-after-call"
        );
        assert_eq!(
            report.facts.repair_hints[0].playbook,
            "reload-memory-derived-value-across-call"
        );
    }

    #[test]
    fn does_not_call_allocated_address_registers_a_missing_reload() {
        let report = triage(
            &["ldrh r3, [r0, r5]", "bl 0x20", "ldrh r5, [r3, r5]", "bx lr"],
            &["ldrh r3, [r0, r6]", "bl 0x20", "ldrh r0, [r3, r6]", "bx lr"],
            2,
        );
        assert!(report.facts.repair_hints.is_empty());
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

    #[test]
    fn multiple_transfer_does_not_hide_a_structural_draft() {
        assert_route(
            &["movs r0, #0", "bne 0x6", "bx lr"],
            &["ldmia r0!, {r1}", "beq 0x6", "bx lr"],
            2,
            ResidualClass::StructuralTopology,
        );
    }

    #[test]
    fn multiple_transfer_is_a_mismatch_not_compiler_impossibility() {
        let candidate = vec!["ldr r1, [r0]".into(), "bx lr".into()];
        let reference = vec!["ldmia r0!, {r1}".into(), "bx lr".into()];
        let report = classify_with_topology(&candidate, &reference, 4, 4, 1, &Comparison::Equal);
        assert_eq!(report.class, ResidualClass::InstructionSelection);
        assert!(!serde_json::to_string(&report)
            .unwrap()
            .contains("unemittable"));
    }

    #[test]
    fn exact_route_is_owner_qualified_and_not_a_corpus_wide_write() {
        let command = ResidualClass::Exact.next_command();
        assert!(command.starts_with("alchemy adopt {owner} --source {source}"));
        assert!(!command.contains("recon/en/main"));
    }
}
