use candidate_show::disasm::{disassemble, Rows};
use compiler_core::{routing::root, sha256};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::{
    collections::{BTreeMap, BTreeSet},
    fs,
    path::{Path, PathBuf},
    sync::OnceLock,
};

const DERIVATION: &str = "literal-anchored-reference-load-store-width-v6";
const DEBT: [&str; 5] = [
    "c_candidate",
    "split_first",
    "merge_with_owner",
    "merge_with_function_owner",
    "merge_with_continuations",
];

#[derive(Clone, Debug, Deserialize, Eq, Ord, PartialEq, PartialOrd, Serialize)]
struct Access {
    anchor: String,
    offset: i32,
    width: u8,
    signed: Option<bool>,
    write: bool,
    owner: String,
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq, Serialize)]
#[serde(rename_all = "kebab-case")]
enum Status {
    Resolved,
    ResolvedStoreOnly,
    RejectedWidthConflict,
    RejectedSignednessConflict,
    RejectedOverlap,
    RejectedOutOfRange,
    RejectedMisaligned,
}
impl Status {
    fn emitted(self) -> bool {
        matches!(self, Self::Resolved | Self::ResolvedStoreOnly)
    }
}

#[derive(Clone, Debug, Deserialize, Serialize)]
struct Field {
    offset: i32,
    widths: Vec<u8>,
    read_signedness: Vec<bool>,
    read_count: usize,
    write_count: usize,
    owners: Vec<String>,
    status: Status,
    selected_width: Option<u8>,
    selected_signedness: Option<bool>,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
struct Proposal {
    anchor: String,
    owners: Vec<String>,
    accesses: Vec<Access>,
    fields: Vec<Field>,
}

#[derive(Clone, Debug, Default, Deserialize, Serialize)]
struct Counts {
    observed_offsets: usize,
    resolved_fields: usize,
    emitted_fields: usize,
    emitted_structs: usize,
    resolved_store_only: usize,
    rejected_width_conflicts: usize,
    rejected_signedness_conflicts: usize,
    rejected_overlaps: usize,
    rejected_out_of_range: usize,
    rejected_misaligned: usize,
}

#[derive(Clone, Debug, Deserialize, Serialize)]
struct Report {
    schema_version: u32,
    derivation: String,
    manifest_sha256: String,
    inputs_sha256: String,
    provenance: Value,
    owners_scanned: usize,
    proposals: Vec<Proposal>,
    field_counts: Counts,
    header_sha256: String,
}

#[derive(Deserialize)]
struct Manifest {
    regions: Vec<Region>,
}

#[derive(Deserialize)]
struct Region {
    address: u64,
    output: PathBuf,
    retention: String,
}
impl Region {
    fn is_debt(&self) -> bool {
        DEBT.contains(&self.retention.as_str())
    }

    fn path(&self, repository: &Path) -> PathBuf {
        if self.output.is_absolute() {
            self.output.clone()
        } else {
            repository.join(&self.output)
        }
    }
}

#[derive(Clone)]
pub(crate) struct Artifacts {
    pub report: PathBuf,
    pub report_sha256: String,
    pub header: PathBuf,
    pub header_sha256: String,
    pub proposal_count: usize,
    pub struct_count: usize,
    pub rejected_misaligned_fields: usize,
    pub roots: BTreeMap<u32, String>,
}

#[derive(Clone)]
struct Base {
    anchor: String,
    offset: i32,
}

static CACHE: OnceLock<Result<Artifacts, String>> = OnceLock::new();

pub(crate) fn artifacts() -> Result<Artifacts, String> {
    CACHE.get_or_init(build).clone()
}

fn build() -> Result<Artifacts, String> {
    let repository = root();
    let manifest = repository.join("out/gs1-en/full/asm/manifest.json");
    let manifest_bytes = read(&manifest)?;
    let manifest: Manifest =
        serde_json::from_slice(&manifest_bytes).map_err(|error| error.to_string())?;
    let manifest_sha256 = sha256::hex(&manifest_bytes);
    let inputs_sha256 = input_hash(repository, &manifest.regions)?;
    let directory = repository.join("out/workbench/shared-aggregates");
    let report_path = directory.join("report.json");
    let header_path = directory.join("shared-aggregates.h");
    if let (Ok(report_bytes), Ok(header_bytes)) = (read(&report_path), read(&header_path)) {
        if let Ok(report) = serde_json::from_slice::<Report>(&report_bytes) {
            if report.derivation == DERIVATION
                && report.manifest_sha256 == manifest_sha256
                && report.inputs_sha256 == inputs_sha256
                && report.header_sha256 == sha256::hex(&header_bytes)
            {
                return Ok(receipt(report, report_path, &report_bytes, header_path));
            }
        }
    }
    let mut report = mine(
        repository,
        &manifest.regions,
        manifest_sha256,
        inputs_sha256,
    )?;
    let header = header(&report.proposals);
    report.header_sha256 = sha256::hex(header.as_bytes());
    fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    fs::write(&header_path, header).map_err(|error| error.to_string())?;
    let mut report_bytes = serde_json::to_vec_pretty(&report).map_err(|error| error.to_string())?;
    report_bytes.push(b'\n');
    fs::write(&report_path, &report_bytes).map_err(|error| error.to_string())?;
    Ok(receipt(report, report_path, &report_bytes, header_path))
}

fn receipt(
    report: Report,
    report_path: PathBuf,
    report_bytes: &[u8],
    header: PathBuf,
) -> Artifacts {
    let roots = report
        .proposals
        .iter()
        .filter(|proposal| emittable(proposal))
        .filter_map(|proposal| {
            proposal
                .anchor
                .strip_prefix("absolute_")
                .and_then(|value| u32::from_str_radix(value, 16).ok())
                .map(|address| (address, proposal.anchor.clone()))
        })
        .collect();
    Artifacts {
        report: report_path,
        report_sha256: sha256::hex(&report_bytes),
        header,
        header_sha256: report.header_sha256,
        proposal_count: report.proposals.len(),
        struct_count: report.field_counts.emitted_structs,
        rejected_misaligned_fields: report.field_counts.rejected_misaligned,
        roots,
    }
}

fn mine(
    repository: &Path,
    regions: &[Region],
    manifest_sha256: String,
    inputs_sha256: String,
) -> Result<Report, String> {
    let mut grouped: BTreeMap<String, Vec<Access>> = BTreeMap::new();
    let mut owners_scanned = 0;
    for region in regions.iter().filter(|region| region.is_debt()) {
        let path = region.path(repository);
        let bytes = read(&path)?;
        for access in analyze(
            &format!("main:{:08x}", region.address),
            &disassemble(&path.to_string_lossy(), 0.0)?,
            &bytes,
        ) {
            grouped
                .entry(access.anchor.clone())
                .or_default()
                .push(access);
        }
        owners_scanned += 1;
    }
    let proposals = grouped
        .into_iter()
        .filter_map(|(anchor, mut accesses)| {
            let owners = unique(accesses.iter().map(|access| access.owner.clone()));
            if owners.len() < 2 {
                return None;
            }
            accesses.sort();
            accesses.dedup();
            Some(Proposal {
                anchor,
                owners,
                fields: fields(&accesses),
                accesses,
            })
        })
        .collect::<Vec<_>>();
    Ok(Report {
        schema_version: 4,
        derivation: DERIVATION.into(),
        manifest_sha256,
        inputs_sha256,
        provenance: json!({
            "authority":"evidence-only",
            "evidence":"canonical retained owner instruction bytes",
            "limitation":"names and aggregate semantics remain hypotheses until reviewed; conflicting field evidence is rejected rather than guessed"
        }),
        owners_scanned,
        field_counts: counts(&proposals),
        proposals,
        header_sha256: String::new(),
    })
}

fn input_hash(repository: &Path, regions: &[Region]) -> Result<String, String> {
    let mut input = Vec::new();
    for region in regions.iter().filter(|region| region.is_debt()) {
        input.extend_from_slice(&region.address.to_le_bytes());
        input.extend(read(&region.path(repository))?);
    }
    Ok(sha256::hex(&input))
}

fn unique<T: Ord>(values: impl Iterator<Item = T>) -> Vec<T> {
    values.collect::<BTreeSet<_>>().into_iter().collect()
}

fn analyze(owner: &str, rows: &Rows, bytes: &[u8]) -> Vec<Access> {
    let mut addresses = rows.keys().collect::<Vec<_>>();
    addresses.sort_by(|left, right| left.partial_cmp(right).unwrap());
    let mut bases: BTreeMap<u8, Base> = BTreeMap::new();
    let mut accesses = Vec::new();
    for address in addresses {
        let line = rows.get(address).unwrap_or("");
        let mut terms = line
            .split('@')
            .next()
            .unwrap_or(line)
            .split_ascii_whitespace();
        let op = terms.next().unwrap_or("").trim_end_matches(".n");
        let operands = terms.collect::<Vec<_>>();
        let dest = operands.first().and_then(|value| register(value));
        if op == "ldr" && line.contains("[pc,") {
            if let Some(dest) = dest {
                bases.remove(&dest);
                if let Some(anchor) = literal_target(line)
                    .and_then(|target| word(bytes, target))
                    .and_then(absolute_anchor)
                {
                    bases.insert(dest, Base { anchor, offset: 0 });
                }
            }
            continue;
        }
        if matches!(op, "ldmia" | "stmia") {
            let Some((base_register, writeback, registers)) = multiple_transfer(line) else {
                bases.clear();
                continue;
            };
            let origin = bases.get(&base_register).cloned();
            if op == "ldmia" {
                for register in &registers {
                    bases.remove(register);
                }
            }
            for (index, register) in registers.iter().enumerate() {
                let Some(origin) = origin.as_ref() else {
                    continue;
                };
                let offset = origin.offset.saturating_add((index as i32) * 4);
                accesses.push(Access {
                    anchor: origin.anchor.clone(),
                    offset,
                    width: 4,
                    signed: None,
                    write: op == "stmia",
                    owner: owner.into(),
                });
                if op == "ldmia" && *register != base_register {
                    bases.insert(
                        *register,
                        Base {
                            anchor: format!("deref_{}_{offset:x}", origin.anchor),
                            offset: 0,
                        },
                    );
                }
            }
            if writeback {
                if op == "ldmia" && registers.contains(&base_register) {
                    bases.remove(&base_register);
                } else if let Some(base) = bases.get_mut(&base_register) {
                    base.offset = base.offset.saturating_add((registers.len() as i32) * 4);
                }
            }
            continue;
        }
        if let Some((width, signed, write)) = access_kind(op) {
            let origin = memory(line).and_then(|(base, offset)| {
                bases.get(&base).cloned().map(|origin| (origin, offset))
            });
            if !write {
                if let Some(dest) = dest {
                    bases.remove(&dest);
                }
            }
            if let Some((origin, memory_offset)) = origin {
                let offset = origin.offset.saturating_add(memory_offset);
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
            continue;
        }
        match operands.as_slice() {
            [to, from] if matches!(op, "mov" | "movs") => {
                if let Some(to) = register(to) {
                    let propagated = register(from).and_then(|from| bases.get(&from).cloned());
                    if let Some(base) = propagated {
                        bases.insert(to, base);
                    } else {
                        bases.remove(&to);
                    }
                }
            }
            [to, from, amount] if matches!(op, "add" | "adds") => {
                if let Some(to) = register(to) {
                    let propagated =
                        register(from)
                            .zip(number(amount))
                            .and_then(|(from, amount)| {
                                bases.get(&from).cloned().map(|base| (base, amount))
                            });
                    if let Some((mut base, amount)) = propagated {
                        base.offset = base.offset.saturating_add(amount);
                        bases.insert(to, base);
                    } else {
                        bases.remove(&to);
                    }
                }
            }
            [to, amount] if matches!(op, "add" | "adds") => {
                if let Some(to) = register(to) {
                    if let (Some(base), Some(amount)) = (bases.get_mut(&to), number(amount)) {
                        base.offset = base.offset.saturating_add(amount);
                    } else {
                        bases.remove(&to);
                    }
                }
            }
            _ if op == "bl" => (0..=3).for_each(|register| {
                bases.remove(&register);
            }),
            _ => {
                if let Some(dest) = dest {
                    if access_kind(op).is_none() && !matches!(op, "cmp" | "cmn" | "tst") {
                        bases.remove(&dest);
                    }
                }
            }
        }
    }
    accesses
}

fn fields(accesses: &[Access]) -> Vec<Field> {
    let mut grouped: BTreeMap<i32, Vec<&Access>> = BTreeMap::new();
    for access in accesses {
        grouped.entry(access.offset).or_default().push(access);
    }
    let mut fields = grouped
        .into_iter()
        .map(|(offset, observations)| {
            let widths = unique(observations.iter().map(|access| access.width));
            let read_signedness = unique(
                observations
                    .iter()
                    .filter(|access| !access.write)
                    .filter_map(|access| access.signed),
            );
            let owners = unique(observations.iter().map(|access| access.owner.clone()));
            let read_count = observations.iter().filter(|access| !access.write).count();
            let write_count = observations.len() - read_count;
            let (status, selected_width, selected_signedness) = if !(0..=0x4000).contains(&offset) {
                (Status::RejectedOutOfRange, None, None)
            } else if widths.len() != 1 {
                (Status::RejectedWidthConflict, None, None)
            } else if read_signedness.len() > 1 {
                (Status::RejectedSignednessConflict, None, None)
            } else if offset % i32::from(widths[0]) != 0 {
                (Status::RejectedMisaligned, None, None)
            } else {
                (
                    if read_count == 0 {
                        Status::ResolvedStoreOnly
                    } else {
                        Status::Resolved
                    },
                    widths.first().copied(),
                    read_signedness.first().copied(),
                )
            };
            Field {
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
    let mut range_end = None;
    for field in &mut fields {
        let Some(width) = field.selected_width.filter(|_| field.status.emitted()) else {
            continue;
        };
        let end = field.offset.saturating_add(i32::from(width));
        if range_end.is_some_and(|previous| field.offset < previous) {
            field.status = Status::RejectedOverlap;
            field.selected_width = None;
            field.selected_signedness = None;
        } else {
            range_end = Some(end);
        }
    }
    fields
}

fn counts(proposals: &[Proposal]) -> Counts {
    let mut counts = Counts::default();
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
                Status::Resolved => {}
                Status::ResolvedStoreOnly => counts.resolved_store_only += 1,
                Status::RejectedWidthConflict => counts.rejected_width_conflicts += 1,
                Status::RejectedSignednessConflict => counts.rejected_signedness_conflicts += 1,
                Status::RejectedOverlap => counts.rejected_overlaps += 1,
                Status::RejectedOutOfRange => counts.rejected_out_of_range += 1,
                Status::RejectedMisaligned => counts.rejected_misaligned += 1,
            }
        }
    }
    counts
}

fn header(proposals: &[Proposal]) -> String {
    let emitted = proposals
        .iter()
        .filter(|proposal| emittable(proposal))
        .collect::<Vec<_>>();
    let children = emitted
        .iter()
        .filter_map(|proposal| {
            let tail = proposal.anchor.strip_prefix("deref_")?;
            let (parent, offset) = tail.rsplit_once('_')?;
            Some((
                (parent, i32::from_str_radix(offset, 16).ok()?),
                proposal.anchor.as_str(),
            ))
        })
        .collect::<BTreeMap<_, _>>();
    let roots = emitted
        .iter()
        .filter(|proposal| proposal.anchor.starts_with("absolute_"))
        .collect::<Vec<_>>();
    let mut text = String::from(
        "/* Generated evidence context; not semantic source authority. */\n\
         #ifndef ALCHEMY_M2C_SHARED_AGGREGATES_H\n\
         #define ALCHEMY_M2C_SHARED_AGGREGATES_H\n\
         #include \"types.h\"\n\n",
    );
    for proposal in &emitted {
        text.push_str(&format!("struct M2cAggregate_{};\n", proposal.anchor));
    }
    text.push('\n');
    for proposal in &emitted {
        text.push_str(&format!(
            "/* {} observed in {} unresolved owners. */\nstruct M2cAggregate_{} {{\n",
            proposal.anchor,
            proposal.owners.len(),
            proposal.anchor
        ));
        let mut cursor = 0;
        for field in proposal
            .fields
            .iter()
            .filter(|field| field.status.emitted() && c_type(field).is_some())
        {
            let width = field.selected_width.unwrap();
            if field.offset > cursor {
                text.push_str(&format!(
                    "    u8 unknown_{cursor:04x}[0x{:x}];\n",
                    field.offset - cursor
                ));
            }
            if let Some(child) = (width == 4)
                .then(|| children.get(&(proposal.anchor.as_str(), field.offset)))
                .flatten()
            {
                text.push_str(&format!(
                    "    struct M2cAggregate_{child} *field_{:04x};\n",
                    field.offset
                ));
            } else {
                text.push_str(&format!(
                    "    {} field_{:04x};\n",
                    c_type(field).unwrap(),
                    field.offset
                ));
            }
            cursor = field.offset + i32::from(width);
        }
        text.push_str("};\n\n");
    }
    text.push_str("#ifdef ALCHEMY_M2C_CONTEXT\n");
    for proposal in &roots {
        text.push_str(&format!(
            "extern struct M2cAggregate_{0} {0};\n",
            proposal.anchor
        ));
    }
    text.push_str("#else\n");
    for proposal in roots {
        text.push_str(&format!(
            "#define {0} (*(struct M2cAggregate_{0} *)0x{1}u)\n",
            proposal.anchor,
            proposal.anchor.trim_start_matches("absolute_")
        ));
    }
    text.push_str("#endif\n#endif\n");
    text
}

fn emittable(proposal: &Proposal) -> bool {
    proposal
        .fields
        .iter()
        .filter(|field| field.status.emitted() && c_type(field).is_some())
        .count()
        >= 2
}

fn c_type(field: &Field) -> Option<&'static str> {
    Some(match (field.selected_width?, field.selected_signedness) {
        (1, Some(true)) => "s8",
        (1, _) => "u8",
        (2, Some(true)) => "s16",
        (2, _) => "u16",
        (4, _) => "u32",
        _ => return None,
    })
}

fn number(text: &str) -> Option<i32> {
    let text = text.trim().trim_start_matches('#');
    text.strip_prefix("0x")
        .and_then(|hex| i32::from_str_radix(hex, 16).ok())
        .or_else(|| text.parse().ok())
}
fn register(text: &str) -> Option<u8> {
    match text.trim().trim_end_matches(',').trim_end_matches('!') {
        "sp" => Some(13),
        "lr" => Some(14),
        "pc" => Some(15),
        "sl" => Some(10),
        "fp" => Some(11),
        "ip" => Some(12),
        value => value.strip_prefix('r')?.parse().ok(),
    }
}
fn multiple_transfer(line: &str) -> Option<(u8, bool, Vec<u8>)> {
    let instruction = line.split('@').next().unwrap_or(line);
    let before_list = instruction.split('{').next()?;
    let base = before_list.split_ascii_whitespace().nth(1)?;
    Some((
        register(base)?,
        base.trim_end_matches(',').ends_with('!'),
        register_list(instruction)?,
    ))
}
fn register_list(line: &str) -> Option<Vec<u8>> {
    let list = line.split('{').nth(1)?.split('}').next()?;
    let mut registers = Vec::new();
    for item in list.split(',') {
        let item = item.trim();
        if let Some((first, last)) = item.split_once('-') {
            let (first, last) = (register(first)?, register(last)?);
            if first > last {
                return None;
            }
            registers.extend(first..=last);
        } else {
            registers.push(register(item)?);
        }
    }
    (!registers.is_empty()).then_some(registers)
}
fn literal_target(line: &str) -> Option<usize> {
    let target = line.split("@ (").nth(1)?.split(')').next()?;
    usize::from_str_radix(target.trim_start_matches("0x"), 16).ok()
}
fn memory(line: &str) -> Option<(u8, i32)> {
    let mut fields = line.split('[').nth(1)?.split(']').next()?.split(',');
    Some((
        register(fields.next()?)?,
        fields.next().and_then(number).unwrap_or(0),
    ))
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
fn word(bytes: &[u8], at: usize) -> Option<u32> {
    Some(u32::from_le_bytes(bytes.get(at..at + 4)?.try_into().ok()?))
}
fn absolute_anchor(value: u32) -> Option<String> {
    (matches!(value >> 24, 0x02..=0x07)).then(|| format!("absolute_{value:08x}"))
}
fn read(path: &Path) -> Result<Vec<u8>, String> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn analyze_fixture(lines: &[&str]) -> Vec<Access> {
        let mut rows = Rows::default();
        for (index, line) in lines.iter().enumerate() {
            rows.set((index * 2) as f64, (*line).into());
        }
        let mut bytes = vec![0; 0x24];
        bytes[0x20..0x24].copy_from_slice(&0x0300_1000u32.to_le_bytes());
        analyze("main:08000000", &rows, &bytes)
    }

    fn field(offset: i32) -> Field {
        Field {
            offset,
            widths: vec![4],
            read_signedness: vec![],
            read_count: 1,
            write_count: 0,
            owners: vec!["main:08000000".into(), "main:08000010".into()],
            status: Status::Resolved,
            selected_width: Some(4),
            selected_signedness: None,
        }
    }

    fn proposals() -> Vec<Proposal> {
        vec![
            Proposal {
                anchor: "absolute_03001000".into(),
                owners: vec!["a".into(), "b".into()],
                accesses: vec![],
                fields: vec![field(0), field(4)],
            },
            Proposal {
                anchor: "deref_absolute_03001000_4".into(),
                owners: vec!["a".into(), "b".into()],
                accesses: vec![],
                fields: vec![field(0), field(8)],
            },
        ]
    }

    #[test]
    fn header_links_dereferences_and_binds_absolute_roots() {
        let output = header(&proposals());
        assert!(!output.contains("packed"));
        assert!(output.contains("struct M2cAggregate_deref_absolute_03001000_4;"));
        assert!(output.contains("struct M2cAggregate_deref_absolute_03001000_4 *field_0004;"));
        assert!(output.contains("extern struct M2cAggregate_absolute_03001000 absolute_03001000;"));
        assert!(output.contains(
            "#define absolute_03001000 (*(struct M2cAggregate_absolute_03001000 *)0x03001000u)"
        ));
    }

    #[test]
    fn compile_header_preserves_aligned_word_load() {
        use candidate_compiler::{
            compile_to_assembly, CandidateCompilerConfiguration, CandidateCompilerFamily,
        };
        use compiler_core::{routing::CompilerTarget, source_paths::SourceOwner};
        let directory =
            std::env::temp_dir().join(format!("alchemy-aggregate-header-{}", std::process::id()));
        let _ = fs::remove_dir_all(&directory);
        fs::create_dir_all(&directory).unwrap();
        let source = directory.join("candidate.c");
        fs::write(directory.join("shared-aggregates.h"), header(&proposals())).unwrap();
        fs::write(&source, b"#include \"shared-aggregates.h\"\nu32 ReadRoot(void) { return absolute_03001000.field_0000; }\n").unwrap();
        let assembly = compile_to_assembly(
            source.to_str().unwrap(),
            SourceOwner::Main(0x0800_00c0)
                .routing_path()
                .to_str()
                .unwrap(),
            directory.join("build").to_str().unwrap(),
            &[],
            CompilerTarget::Gs1,
            &CandidateCompilerConfiguration {
                family: Some(CandidateCompilerFamily::Routed),
                ..Default::default()
            },
        )
        .unwrap();
        let assembly = fs::read_to_string(assembly).unwrap();
        assert!(assembly.contains("\tldr\tr0, [r3]"));
        assert!(!assembly.contains("ldrb"));
        fs::remove_dir_all(directory).unwrap();
    }

    #[test]
    fn misaligned_fields_are_rejected_before_header_generation() {
        let access = |owner: &str| Access {
            anchor: "absolute_03001000".into(),
            offset: 1,
            width: 4,
            signed: None,
            write: false,
            owner: owner.into(),
        };
        let fields = fields(&[access("a"), access("b")]);
        assert_eq!(fields[0].status, Status::RejectedMisaligned);
        assert_eq!(fields[0].selected_width, None);
    }

    #[test]
    fn narrow_load_kills_pointer_provenance_in_its_destination() {
        let accesses = analyze_fixture(&[
            "ldr r0, [pc, #0] @ (0x20)",
            "ldrb r0, [r0, #0]",
            "ldr r1, [r0, #4]",
        ]);
        assert_eq!(accesses.len(), 1);
        assert_eq!((accesses[0].offset, accesses[0].width), (0, 1));
    }

    #[test]
    fn load_from_untracked_base_kills_old_destination_provenance() {
        let accesses = analyze_fixture(&[
            "ldr r0, [pc, #0] @ (0x20)",
            "ldr r0, [r2, #0]",
            "ldr r1, [r0, #4]",
        ]);
        assert!(accesses.is_empty());
    }

    #[test]
    fn failed_add_propagation_kills_old_destination_provenance() {
        let accesses = analyze_fixture(&[
            "ldr r0, [pc, #0] @ (0x20)",
            "mov r1, r0",
            "adds r1, r2, #4",
            "ldr r3, [r1, #0]",
            "ldr r0, [pc, #0] @ (0x20)",
            "adds r0, r0, r2",
            "ldr r3, [r0, #0]",
            "ldr r0, [pc, #0] @ (0x20)",
            "adds r0, r2",
            "ldr r3, [r0, #0]",
        ]);
        assert!(accesses.is_empty());
    }

    #[test]
    fn ldmia_models_loaded_word_and_base_writeback() {
        let accesses = analyze_fixture(&[
            "ldr r3, [pc, #0] @ (0x20)",
            "ldmia r3!, {r2}",
            "ldr r0, [r3, #4]",
            "ldr r1, [r2, #4]",
        ]);
        assert_eq!(
            accesses
                .iter()
                .map(|access| (access.anchor.as_str(), access.offset, access.write))
                .collect::<Vec<_>>(),
            [
                ("absolute_03001000", 0, false),
                ("absolute_03001000", 8, false),
                ("deref_absolute_03001000_0", 4, false),
            ]
        );
    }

    #[test]
    fn stmia_models_each_word_and_base_writeback() {
        let accesses = analyze_fixture(&[
            "ldr r3, [pc, #0] @ (0x20)",
            "stmia r3!, {r0, r1}",
            "str r2, [r3, #4]",
        ]);
        assert_eq!(
            accesses
                .iter()
                .map(|access| (access.offset, access.write))
                .collect::<Vec<_>>(),
            [(0, true), (4, true), (12, true)]
        );
    }

    #[test]
    fn malformed_multiple_transfer_discards_all_provenance() {
        let accesses = analyze_fixture(&[
            "ldr r3, [pc, #0] @ (0x20)",
            "ldmia r3!, {not-a-register}",
            "ldr r0, [r3, #4]",
        ]);
        assert!(accesses.is_empty());
    }
}
