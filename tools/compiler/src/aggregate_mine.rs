use candidate_compiler::{
    compile_to_assembly, CandidateCompilerConfiguration, CandidateCompilerFamily,
};
use candidate_show::disasm::{disassemble, Rows};
use compiler_core::routing::CompilerTarget;
use serde::{Deserialize, Serialize};
use serde_json::Value;
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Path, PathBuf},
};

const DEFAULT_PROOF_COUNT: usize = 3;
const DEBT: [&str; 5] = [
    "c_candidate",
    "split_first",
    "merge_with_owner",
    "merge_with_function_owner",
    "merge_with_continuations",
];

#[derive(Clone, Debug, Deserialize, Eq, Ord, PartialEq, PartialOrd, Serialize)]
pub struct Access {
    pub anchor: String,
    pub offset: i32,
    pub width: u8,
    pub signed: Option<bool>,
    pub write: bool,
    pub owner: String,
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "kebab-case")]
pub enum FieldStatus {
    Resolved,
    ResolvedStoreOnly,
    RejectedWidthConflict,
    RejectedSignednessConflict,
    RejectedOverlap,
    RejectedOutOfRange,
}
impl FieldStatus {
    fn emitted(self) -> bool {
        matches!(self, Self::Resolved | Self::ResolvedStoreOnly)
    }
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct FieldEvidence {
    pub offset: i32,
    pub widths: Vec<u8>,
    /// Signedness is derived only from loads; stores do not reveal it.
    pub read_signedness: Vec<bool>,
    pub read_count: usize,
    pub write_count: usize,
    pub owners: Vec<String>,
    pub status: FieldStatus,
    pub selected_width: Option<u8>,
    pub selected_signedness: Option<bool>,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct Proposal {
    pub anchor: String,
    pub owners: Vec<String>,
    pub accesses: Vec<Access>,
    pub fields: Vec<FieldEvidence>,
}

#[derive(Clone, Debug, Default, Deserialize, Serialize)]
pub struct FieldCounts {
    pub observed_offsets: usize,
    pub resolved_fields: usize,
    pub emitted_fields: usize,
    pub emitted_structs: usize,
    pub resolved_store_only: usize,
    pub rejected_width_conflicts: usize,
    pub rejected_signedness_conflicts: usize,
    pub rejected_overlaps: usize,
    pub rejected_out_of_range: usize,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct Provenance {
    pub authority: String,
    pub evidence: String,
    pub limitation: String,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct CompileProofResult {
    pub owner: String,
    pub source: String,
    pub owner_bytes: usize,
    pub baseline_compiled: bool,
    pub context_compiled: bool,
    pub codegen_neutral: bool,
    pub baseline_text_bytes: Option<usize>,
    pub context_text_bytes: Option<usize>,
    pub diagnostic: Option<String>,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct CompileProof {
    pub status: String,
    pub selection_policy: String,
    pub candidates_available: usize,
    pub requested: usize,
    pub attempted: usize,
    pub passed: usize,
    pub failed: usize,
    pub results: Vec<CompileProofResult>,
}
impl CompileProof {
    fn pending(available: usize, requested: usize) -> Self {
        Self {
            status: "pending".into(),
            selection_policy: "largest-open-standalone-drafts-by-owner-bytes".into(),
            candidates_available: available,
            requested,
            attempted: 0,
            passed: 0,
            failed: 0,
            results: Vec::new(),
        }
    }
}

#[derive(Clone, Debug, Deserialize, Serialize)]
pub struct MineReport {
    pub schema_version: u32,
    pub derivation: String,
    pub provenance: Provenance,
    pub owners_scanned: usize,
    pub proposals: Vec<Proposal>,
    pub field_counts: FieldCounts,
    pub compile_proof: CompileProof,
}

#[derive(Clone, Debug)]
struct Base {
    anchor: String,
    offset: i32,
}
#[derive(Clone, Debug, Eq, Ord, PartialEq, PartialOrd)]
struct Draft {
    owner_bytes: usize,
    owner: String,
    source: PathBuf,
}

fn number(text: &str) -> Option<i32> {
    let text = text.trim().trim_start_matches('#');
    text.strip_prefix("0x")
        .and_then(|hex| i32::from_str_radix(hex, 16).ok())
        .or_else(|| text.parse().ok())
}
fn register(text: &str) -> Option<u8> {
    match text.trim().trim_end_matches(',') {
        "sl" => Some(10),
        "fp" => Some(11),
        "ip" => Some(12),
        value => value.strip_prefix('r')?.parse().ok(),
    }
}
fn literal_target(line: &str) -> Option<usize> {
    let target = line.split("@ (").nth(1)?.split(')').next()?;
    usize::from_str_radix(target.trim_start_matches("0x"), 16).ok()
}
fn memory(line: &str) -> Option<(u8, i32)> {
    let inside = line.split('[').nth(1)?.split(']').next()?;
    let mut fields = inside.split(',');
    Some((
        register(fields.next()?)?,
        fields.next().and_then(number).unwrap_or(0),
    ))
}
fn opcode(line: &str) -> &str {
    line.split_ascii_whitespace()
        .next()
        .unwrap_or("")
        .trim_end_matches(".n")
}
fn access_kind(op: &str) -> Option<(u8, Option<bool>, bool)> {
    Some(match op {
        "ldr" => (4, None, false),
        "ldrb" => (1, Some(false), false),
        "ldrsb" => (1, Some(true), false),
        "ldrh" => (2, Some(false), false),
        "ldrsh" => (2, Some(true), false),
        "str" => (4, None, true),
        "strb" => (1, None, true),
        "strh" => (2, None, true),
        _ => return None,
    })
}
fn destination(line: &str) -> Option<u8> {
    register(line.split_ascii_whitespace().nth(1)?)
}
fn word(bytes: &[u8], at: usize) -> Option<u32> {
    Some(u32::from_le_bytes(bytes.get(at..at + 4)?.try_into().ok()?))
}
fn absolute_anchor(value: u32) -> Option<String> {
    (matches!(value >> 24, 0x02..=0x07)).then(|| format!("absolute_{value:08x}"))
}

pub fn analyze(owner: &str, rows: &Rows, bytes: &[u8]) -> Vec<Access> {
    let mut addresses = rows.keys().collect::<Vec<_>>();
    addresses.sort_by(|left, right| left.partial_cmp(right).unwrap());
    let mut bases: BTreeMap<u8, Base> = BTreeMap::new();
    let mut accesses = Vec::new();
    for address in addresses {
        let line = rows.get(address).unwrap_or("");
        let op = opcode(line);
        let dest = destination(line);
        if op == "ldr" && line.contains("[pc,") {
            if let (Some(dest), Some(target)) = (dest, literal_target(line)) {
                match word(bytes, target).and_then(absolute_anchor) {
                    Some(anchor) => {
                        bases.insert(dest, Base { anchor, offset: 0 });
                    }
                    None => {
                        bases.remove(&dest);
                    }
                }
            }
            continue;
        }
        if let Some((width, signed, write)) = access_kind(op) {
            if let Some((base, offset)) = memory(line) {
                if let Some(origin) = bases.get(&base).cloned() {
                    let offset = origin.offset.saturating_add(offset);
                    accesses.push(Access {
                        anchor: origin.anchor.clone(),
                        offset,
                        width,
                        signed,
                        write,
                        owner: owner.into(),
                    });
                    if !write && width == 4 {
                        if let Some(dest) = dest {
                            bases.insert(
                                dest,
                                Base {
                                    anchor: format!("deref_{}_{offset:x}", origin.anchor),
                                    offset: 0,
                                },
                            );
                        }
                    }
                }
            }
        }
        let fields = line
            .split('@')
            .next()
            .unwrap_or(line)
            .replace(',', " ")
            .split_ascii_whitespace()
            .map(str::to_string)
            .collect::<Vec<_>>();
        match fields.as_slice() {
            [op, to, from] if matches!(op.as_str(), "mov" | "movs") => {
                if let (Some(to), Some(from)) = (register(to), register(from)) {
                    match bases.get(&from).cloned() {
                        Some(base) => {
                            bases.insert(to, base);
                        }
                        None => {
                            bases.remove(&to);
                        }
                    }
                }
            }
            [op, to, from, amount] if matches!(op.as_str(), "add" | "adds") => {
                if let (Some(to), Some(from), Some(amount)) =
                    (register(to), register(from), number(amount))
                {
                    if let Some(mut base) = bases.get(&from).cloned() {
                        base.offset = base.offset.saturating_add(amount);
                        bases.insert(to, base);
                    }
                }
            }
            [op, to, amount] if matches!(op.as_str(), "add" | "adds") => {
                if let (Some(to), Some(amount)) = (register(to), number(amount)) {
                    if let Some(base) = bases.get_mut(&to) {
                        base.offset = base.offset.saturating_add(amount);
                    }
                }
            }
            _ if op == "bl" => (0..=3).for_each(|register| {
                bases.remove(&register);
            }),
            _ => {
                if let Some(dest) = dest {
                    if !matches!(
                        op,
                        "cmp"
                            | "cmn"
                            | "tst"
                            | "ldr"
                            | "ldrb"
                            | "ldrsb"
                            | "ldrh"
                            | "ldrsh"
                            | "str"
                            | "strb"
                            | "strh"
                    ) {
                        bases.remove(&dest);
                    }
                }
            }
        }
    }
    accesses.sort();
    accesses.dedup();
    accesses
}

fn field_evidence(accesses: &[Access]) -> Vec<FieldEvidence> {
    let mut grouped: BTreeMap<i32, Vec<&Access>> = BTreeMap::new();
    accesses
        .iter()
        .for_each(|access| grouped.entry(access.offset).or_default().push(access));
    let mut fields = grouped
        .into_iter()
        .map(|(offset, observations)| {
            let widths = observations
                .iter()
                .map(|access| access.width)
                .collect::<BTreeSet<_>>()
                .into_iter()
                .collect::<Vec<_>>();
            let read_signedness = observations
                .iter()
                .filter(|access| !access.write)
                .filter_map(|access| access.signed)
                .collect::<BTreeSet<_>>()
                .into_iter()
                .collect::<Vec<_>>();
            let owners = observations
                .iter()
                .map(|access| access.owner.clone())
                .collect::<BTreeSet<_>>()
                .into_iter()
                .collect::<Vec<_>>();
            let read_count = observations.iter().filter(|access| !access.write).count();
            let write_count = observations.len() - read_count;
            let (status, selected_width, selected_signedness) = if !(0..=0x4000).contains(&offset) {
                (FieldStatus::RejectedOutOfRange, None, None)
            } else if widths.len() != 1 {
                (FieldStatus::RejectedWidthConflict, None, None)
            } else if read_signedness.len() > 1 {
                (FieldStatus::RejectedSignednessConflict, None, None)
            } else {
                (
                    if read_count == 0 {
                        FieldStatus::ResolvedStoreOnly
                    } else {
                        FieldStatus::Resolved
                    },
                    widths.first().copied(),
                    read_signedness.first().copied(),
                )
            };
            FieldEvidence {
                offset,
                widths,
                read_signedness,
                read_count,
                write_count,
                owners,
                status,
                selected_width,
                selected_signedness,
            }
        })
        .collect::<Vec<_>>();
    let mut ranges: Vec<(i32, i32)> = Vec::new();
    for field in &mut fields {
        let Some(width) = field.selected_width.filter(|_| field.status.emitted()) else {
            continue;
        };
        let end = field.offset.saturating_add(i32::from(width));
        if ranges
            .iter()
            .any(|(start, stop)| field.offset < *stop && end > *start)
        {
            field.status = FieldStatus::RejectedOverlap;
            field.selected_width = None;
            field.selected_signedness = None;
        } else {
            ranges.push((field.offset, end));
        }
    }
    fields
}

fn field_counts(proposals: &[Proposal]) -> FieldCounts {
    let mut counts = FieldCounts::default();
    for proposal in proposals {
        let emitted = proposal
            .fields
            .iter()
            .filter(|field| field.status.emitted())
            .count();
        counts.observed_offsets += proposal.fields.len();
        counts.resolved_fields += emitted;
        if emitted >= 2 {
            counts.emitted_fields += emitted;
            counts.emitted_structs += 1;
        }
        for field in &proposal.fields {
            match field.status {
                FieldStatus::Resolved => {}
                FieldStatus::ResolvedStoreOnly => counts.resolved_store_only += 1,
                FieldStatus::RejectedWidthConflict => counts.rejected_width_conflicts += 1,
                FieldStatus::RejectedSignednessConflict => {
                    counts.rejected_signedness_conflicts += 1
                }
                FieldStatus::RejectedOverlap => counts.rejected_overlaps += 1,
                FieldStatus::RejectedOutOfRange => counts.rejected_out_of_range += 1,
            }
        }
    }
    counts
}

fn report(root: &Path, proof_count: usize) -> Result<(MineReport, Vec<Draft>), String> {
    let manifest = root.join("out/gs1-en/full/asm/manifest.json");
    let document: Value = serde_json::from_slice(
        &fs::read(&manifest).map_err(|error| format!("{}: {error}", manifest.display()))?,
    )
    .map_err(|error| error.to_string())?;
    let regions = document["regions"]
        .as_array()
        .ok_or("asm manifest has no regions")?;
    let mut all = Vec::new();
    let mut drafts = Vec::new();
    let mut owners = 0;
    for region in regions {
        if !region["retention"]
            .as_str()
            .is_some_and(|value| DEBT.contains(&value))
        {
            continue;
        }
        let address = region["address"].as_u64().ok_or("region address missing")?;
        let owner = format!("main:{address:08x}");
        let path = PathBuf::from(region["output"].as_str().ok_or("region output missing")?);
        let path = if path.is_absolute() {
            path
        } else {
            root.join(path)
        };
        let bytes = fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        let rows = disassemble(&path.to_string_lossy(), 0.0)?;
        all.extend(analyze(&owner, &rows, &bytes));
        let source = root.join(format!("games/gs1/recon/en/main/{address:08x}.c"));
        if source.is_file() {
            drafts.push(Draft {
                owner_bytes: region["size"]
                    .as_u64()
                    .and_then(|value| usize::try_from(value).ok())
                    .unwrap_or(bytes.len()),
                owner: owner.clone(),
                source,
            });
        }
        owners += 1;
    }
    let mut grouped: BTreeMap<String, Vec<Access>> = BTreeMap::new();
    all.into_iter().for_each(|access| {
        grouped
            .entry(access.anchor.clone())
            .or_default()
            .push(access)
    });
    let proposals = grouped
        .into_iter()
        .filter_map(|(anchor, mut accesses)| {
            let owners = accesses
                .iter()
                .map(|access| access.owner.clone())
                .collect::<BTreeSet<_>>();
            (owners.len() >= 2).then(|| {
                accesses.sort();
                accesses.dedup();
                Proposal {
                    anchor,
                    owners: owners.into_iter().collect(),
                    fields: field_evidence(&accesses),
                    accesses,
                }
            })
        })
        .collect::<Vec<_>>();
    drafts.sort_by(|left, right| {
        right
            .owner_bytes
            .cmp(&left.owner_bytes)
            .then_with(|| left.owner.cmp(&right.owner))
    });
    Ok((
        MineReport {
            schema_version: 2,
            derivation: "literal-anchored-reference-load-store-width-v2".into(),
            provenance: Provenance {
                authority: "evidence-only".into(),
                evidence: "canonical retained owner instruction bytes".into(),
                limitation: "names and aggregate semantics remain hypotheses until reviewed; conflicting field evidence is rejected rather than guessed".into(),
            },
            owners_scanned: owners,
            field_counts: field_counts(&proposals),
            proposals,
            compile_proof: CompileProof::pending(drafts.len(), proof_count),
        },
        drafts,
    ))
}

fn c_type(field: &FieldEvidence) -> Option<&'static str> {
    Some(match (field.selected_width?, field.selected_signedness) {
        (1, Some(true)) => "s8",
        (1, _) => "u8",
        (2, Some(true)) => "s16",
        (2, _) => "u16",
        (4, _) => "u32",
        _ => return None,
    })
}
fn context(report: &MineReport) -> String {
    let mut text = String::from(
        "/* Generated evidence context; not semantic source authority.\n\
         * Conflicting widths, signedness, and overlaps stay in report.json. */\n\
         #ifndef ALCHEMY_M2C_SHARED_AGGREGATES_H\n\
         #define ALCHEMY_M2C_SHARED_AGGREGATES_H\n\
         #include \"types.h\"\n\n",
    );
    for proposal in &report.proposals {
        let fields = proposal
            .fields
            .iter()
            .filter(|field| field.status.emitted() && c_type(field).is_some())
            .collect::<Vec<_>>();
        if fields.len() < 2 {
            continue;
        }
        let name = proposal
            .anchor
            .chars()
            .map(|character| {
                if character.is_ascii_alphanumeric() {
                    character
                } else {
                    '_'
                }
            })
            .collect::<String>();
        text.push_str(&format!(
            "/* {} observed in {} unresolved owners. */\nstruct M2cAggregate_{name} {{\n",
            proposal.anchor,
            proposal.owners.len()
        ));
        let mut cursor = 0;
        for field in fields {
            if field.offset > cursor {
                text.push_str(&format!(
                    "    u8 unknown_{cursor:04x}[0x{:x}];\n",
                    field.offset - cursor
                ));
            }
            text.push_str(&format!(
                "    {} field_{:04x};\n",
                c_type(field).unwrap(),
                field.offset
            ));
            cursor = field.offset + i32::from(field.selected_width.unwrap());
        }
        text.push_str("} __attribute__((packed));\n\n");
    }
    text.push_str("#endif\n");
    text
}

fn include_line(path: &Path) -> Result<String, String> {
    let path = path
        .to_str()
        .ok_or_else(|| format!("{} is not UTF-8", path.display()))?;
    if path.contains(['\n', '\r', '"']) {
        return Err(format!("{path} cannot be written as a C include"));
    }
    Ok(format!("#include \"{path}\"\n"))
}
fn compile_text(source: &Path, routing: &Path, output: &Path) -> Result<Vec<u8>, String> {
    fs::create_dir_all(output).map_err(|error| format!("{}: {error}", output.display()))?;
    let assembly = compile_to_assembly(
        &source.to_string_lossy(),
        &routing.to_string_lossy(),
        &output.to_string_lossy(),
        &[],
        CompilerTarget::Gs1,
        &CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            ..Default::default()
        },
    )?;
    let object = output.join("candidate.o");
    let binary = output.join("candidate.bin");
    candidate_compiler::verify::assemble(&assembly, &object.to_string_lossy())?;
    candidate_compiler::verify::copy_text(&object.to_string_lossy(), &binary.to_string_lossy())?;
    fs::read(&binary).map_err(|error| format!("{}: {error}", binary.display()))
}
fn proof_result(draft: &Draft, header: &Path, output: &Path) -> CompileProofResult {
    let directory = output.join(draft.owner.replace(':', "-"));
    let wrapper = directory.join("with-shared-aggregates.c");
    let mut result = CompileProofResult {
        owner: draft.owner.clone(),
        source: draft.source.to_string_lossy().into_owned(),
        owner_bytes: draft.owner_bytes,
        baseline_compiled: false,
        context_compiled: false,
        codegen_neutral: false,
        baseline_text_bytes: None,
        context_text_bytes: None,
        diagnostic: None,
    };
    let setup = fs::create_dir_all(&directory)
        .map_err(|error| format!("{}: {error}", directory.display()))
        .and_then(|_| {
            Ok(format!(
                "{}{}",
                include_line(header)?,
                include_line(&draft.source)?
            ))
        })
        .and_then(|text| {
            fs::write(&wrapper, text).map_err(|error| format!("{}: {error}", wrapper.display()))
        });
    if let Err(error) = setup {
        result.diagnostic = Some(error);
        return result;
    }
    let baseline = match compile_text(&draft.source, &draft.source, &directory.join("baseline")) {
        Ok(bytes) => {
            result.baseline_compiled = true;
            result.baseline_text_bytes = Some(bytes.len());
            bytes
        }
        Err(error) => {
            result.diagnostic = Some(format!("baseline compile failed: {error}"));
            return result;
        }
    };
    let included = match compile_text(&wrapper, &draft.source, &directory.join("with-context")) {
        Ok(bytes) => {
            result.context_compiled = true;
            result.context_text_bytes = Some(bytes.len());
            bytes
        }
        Err(error) => {
            result.diagnostic = Some(format!("shared-context compile failed: {error}"));
            return result;
        }
    };
    result.codegen_neutral = baseline == included;
    if !result.codegen_neutral {
        result.diagnostic = Some("shared context changed emitted .text".into());
    }
    result
}
fn prove_context(drafts: &[Draft], header: &Path, output: &Path, requested: usize) -> CompileProof {
    let mut proof = CompileProof::pending(drafts.len(), requested);
    proof.results = drafts
        .iter()
        .take(requested)
        .map(|draft| proof_result(draft, header, output))
        .collect();
    proof.attempted = proof.results.len();
    proof.passed = proof
        .results
        .iter()
        .filter(|result| {
            result.baseline_compiled && result.context_compiled && result.codegen_neutral
        })
        .count();
    proof.failed = proof.attempted - proof.passed;
    proof.status = if proof.attempted > 0 && proof.failed == 0 {
        "passed"
    } else {
        "failed"
    }
    .into();
    proof
}

fn options(arguments: &[String]) -> Result<(PathBuf, usize), String> {
    let mut output = PathBuf::from("out/workbench/shared-aggregates");
    let mut proof_count = DEFAULT_PROOF_COUNT;
    let mut index = 0;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--output" => {
                index += 1;
                output = PathBuf::from(
                    arguments
                        .get(index)
                        .ok_or("--output requires a directory")?,
                );
            }
            "--proof-count" => {
                index += 1;
                proof_count = arguments
                    .get(index)
                    .ok_or("--proof-count requires a positive integer")?
                    .parse()
                    .map_err(|_| "--proof-count requires a positive integer")?;
                if proof_count == 0 {
                    return Err("--proof-count must be at least 1".into());
                }
            }
            _ => {
                return Err(
                    "usage: compiler aggregate-mine [--output DIR] [--proof-count COUNT]".into(),
                )
            }
        }
        index += 1;
    }
    Ok((output, proof_count))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let (output, proof_count) = options(arguments)?;
    let root = compiler_core::routing::root();
    let output = if output.is_absolute() {
        output
    } else {
        root.join(output)
    };
    if !output.starts_with(root.join("out")) {
        return Err("aggregate output must be under out/".into());
    }
    fs::create_dir_all(&output).map_err(|error| format!("{}: {error}", output.display()))?;
    let (mut report, drafts) = report(root, proof_count)?;
    let header = output.join("shared-aggregates.h");
    fs::write(&header, context(&report)).map_err(|error| error.to_string())?;
    report.compile_proof = prove_context(&drafts, &header, &output.join("proof"), proof_count);
    fs::write(
        output.join("report.json"),
        serde_json::to_string_pretty(&report).map_err(|error| error.to_string())? + "\n",
    )
    .map_err(|error| error.to_string())?;
    println!(
        "aggregate-mine owners={} proposals={} fields={}/{} width_conflicts={} signedness_conflicts={} overlaps={} proof={}/{} output={}",
        report.owners_scanned,
        report.proposals.len(),
        report.field_counts.emitted_fields,
        report.field_counts.observed_offsets,
        report.field_counts.rejected_width_conflicts,
        report.field_counts.rejected_signedness_conflicts,
        report.field_counts.rejected_overlaps,
        report.compile_proof.passed,
        report.compile_proof.attempted,
        output.display()
    );
    if report.compile_proof.status != "passed" {
        return Err(format!(
            "shared aggregate compile proof failed for {} of {} representative drafts; see {}",
            report.compile_proof.failed,
            report.compile_proof.attempted,
            output.join("report.json").display()
        ));
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    fn access(offset: i32, width: u8, signed: Option<bool>, write: bool) -> Access {
        Access {
            anchor: "absolute_03001000".into(),
            offset,
            width,
            signed,
            write,
            owner: format!("main:080000{:02x}", usize::from(write) * 0x10),
        }
    }
    fn fixture_report(accesses: Vec<Access>) -> MineReport {
        let proposals = vec![Proposal {
            anchor: "absolute_03001000".into(),
            owners: vec!["main:08000000".into(), "main:08000010".into()],
            fields: field_evidence(&accesses),
            accesses,
        }];
        MineReport {
            schema_version: 2,
            derivation: "fixture".into(),
            provenance: Provenance {
                authority: "evidence-only".into(),
                evidence: "fixture".into(),
                limitation: "fixture".into(),
            },
            owners_scanned: 2,
            field_counts: field_counts(&proposals),
            compile_proof: CompileProof::pending(0, 1),
            proposals,
        }
    }
    #[test]
    fn tracks_literal_anchored_width_and_signedness() {
        let mut rows = Rows::default();
        rows.set(0.0, "ldr r4, [pc, #4] @ (0x8)".into());
        rows.set(2.0, "ldrsb r0, [r4, #3]".into());
        rows.set(4.0, "strh r1, [r4, #6]".into());
        let mut bytes = vec![0; 12];
        bytes[8..12].copy_from_slice(&0x0300_1000u32.to_le_bytes());
        let accesses = analyze("main:08000000", &rows, &bytes);
        assert_eq!(accesses.len(), 2);
        assert_eq!(
            (accesses[0].offset, accesses[0].width, accesses[0].signed),
            (3, 1, Some(true))
        );
        assert!(accesses[1].write);
    }
    #[test]
    fn conflicts_are_rejected_and_counted() {
        let report = fixture_report(vec![
            access(0, 1, Some(false), false),
            access(0, 2, None, true),
            access(4, 1, Some(false), false),
            access(4, 1, Some(true), false),
            access(8, 4, None, false),
            access(12, 1, None, true),
        ]);
        assert_eq!(
            report.proposals[0].fields[0].status,
            FieldStatus::RejectedWidthConflict
        );
        assert_eq!(
            report.proposals[0].fields[1].status,
            FieldStatus::RejectedSignednessConflict
        );
        assert_eq!(report.field_counts.rejected_width_conflicts, 1);
        assert_eq!(report.field_counts.rejected_signedness_conflicts, 1);
        assert_eq!(report.field_counts.resolved_store_only, 1);
        let header = context(&report);
        assert!(!header.contains("field_0000"));
        assert!(!header.contains("field_0004"));
        assert!(header.contains("field_0008"));
        assert!(header.contains("field_000c"));
    }
    #[test]
    fn overlaps_require_a_reviewed_union() {
        let report = fixture_report(vec![
            access(0, 4, None, false),
            access(2, 2, Some(false), false),
            access(4, 2, Some(false), false),
        ]);
        assert_eq!(
            report.proposals[0].fields[1].status,
            FieldStatus::RejectedOverlap
        );
        assert_eq!(report.field_counts.rejected_overlaps, 1);
        assert!(!context(&report).contains("field_0002"));
    }
    #[test]
    fn context_is_explicitly_evidence_only() {
        let report = fixture_report(vec![
            access(1, 4, None, false),
            access(5, 1, Some(false), false),
        ]);
        let header = context(&report);
        assert!(header.contains("not semantic source authority"));
        assert!(header.contains("struct M2cAggregate_absolute_03001000"));
        assert!(header.contains("unknown_0000[0x1]"));
    }
    #[test]
    fn compile_proof_is_enabled_and_bounded() {
        assert_eq!(options(&[]).unwrap().1, DEFAULT_PROOF_COUNT);
        assert_eq!(options(&["--proof-count".into(), "5".into()]).unwrap().1, 5);
        assert!(options(&["--proof-count".into(), "0".into()]).is_err());
    }
}
