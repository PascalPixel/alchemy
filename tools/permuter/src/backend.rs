//! Native Alchemy compiler/scorer boundary for permutation jobs.

use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

use compiler_core::routing::root;

use crate::compile::{PreparedTarget, Score as ByteScore};

static TEMP_COUNTER: AtomicU64 = AtomicU64::new(0);

/// Alchemy candidates are scored from linked bytes, so objdump is not an input.
const ALCHEMY_HOST_TOOLS: [&str; 4] = ["arm-none-eabi-as", "arm-none-eabi-nm", "arm-none-eabi-ld", "arm-none-eabi-objcopy"];

#[derive(Clone, Debug, PartialEq)]
pub struct Measurement {
    pub exact: bool,
    pub score: u64,
    pub differences: usize,
    pub expected_size: usize,
    pub actual_size: usize,
    pub first_difference: Option<usize>,
    pub fingerprint: u64,
    pub summary: String,
    /// Fractional positions (0..1) of our-side differing rows in the
    /// candidate instruction stream; empty when the backend does not
    /// compute a row diff. Guides heat-biased mutation; never persisted.
    pub heat: Vec<f32>,
    /// Differing rows between the candidate's and the reference's
    /// branch-and-link sequences. A candidate that moves a call scores well
    /// by phase and is semantically false; the walk refuses to keep one
    /// whose divergence exceeds the run baseline's.
    pub bl_divergence: usize,
    /// Multiset difference of store instructions (mnemonic and offset
    /// operand). Catches stores moved between arms or deleted -- the second
    /// measured lie class.
    pub store_divergence: usize,
    /// Fingerprints of the candidate's own ordered `bl` row sequence and of
    /// its sorted store-row multiset. Divergence counts alone are
    /// baseline-relative: a lateral call or store move can hide under
    /// existing divergence by trading one differing row for another at the
    /// same count. At equal divergence the guards therefore also require
    /// the candidate's rows to be identical to the baseline's. Zero means
    /// unknown (failed builds, byte-only backends, legacy journal rows);
    /// unknown never satisfies an equal-divergence comparison.
    pub bl_signature: u64,
    pub store_signature: u64,
}

#[cfg(test)]
impl Measurement {
    pub fn failed(message: &str) -> Self {
        Self { exact: false, score: u64::MAX, differences: usize::MAX, expected_size: 0, actual_size: 0, first_difference: None, fingerprint: 0, summary: message.to_string(), heat: Vec::new(), bl_divergence: 0, store_divergence: 0, bl_signature: 0, store_signature: 0 }
    }
}

impl From<&ByteScore> for Measurement {
    fn from(score: &ByteScore) -> Self {
        let size_penalty = score.actual_size.abs_diff(score.expected_size) as u64;
        Self {
            exact: score.exact,
            score: score.differing_halfwords as u64 * 100 + size_penalty,
            differences: score.differing_halfwords,
            expected_size: score.expected_size,
            actual_size: score.actual_size,
            first_difference: score.first_difference,
            fingerprint: fingerprint(&score.actual),
            heat: Vec::new(),
            bl_divergence: 0,
            store_divergence: 0,
            bl_signature: 0,
            store_signature: 0,
            summary: format!("{} differing halfwords, {} / {} bytes", score.differing_halfwords, score.actual_size, score.expected_size),
        }
    }
}

pub trait Backend: Send + Sync {
    fn name(&self) -> &str;
    fn identity(&self) -> String;
    fn baseline(&self) -> Measurement;
    fn measure(&self, source: &str) -> Result<Measurement, String>;
}

pub struct Input {
    pub source_path: PathBuf,
    pub source: String,
}

fn resolve(path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root().join(path)
    }
}

pub fn load_input(path: &Path) -> Result<Input, String> {
    let requested = resolve(path);
    if requested.is_dir() {
        return Err(format!("{} is a directory; pass an Alchemy candidate C file", requested.display()));
    } else {
        let source = fs::read_to_string(&requested).map_err(|error| format!("{}: {error}", requested.display()))?;
        let source = expand_local_c_includes(&requested, &source, &mut BTreeSet::new())?;
        Ok(Input { source_path: requested, source })
    }
}

fn expand_local_c_includes(path: &Path, source: &str, active: &mut BTreeSet<PathBuf>) -> Result<String, String> {
    let canonical = fs::canonicalize(path).map_err(|error| format!("{}: {error}", path.display()))?;
    if !active.insert(canonical.clone()) {
        return Err(format!("recursive C include through {}", canonical.display()));
    }
    let mut expanded = String::new();
    for line in source.lines() {
        let include = line.trim().strip_prefix("#include").map(str::trim).and_then(|value| value.strip_prefix('"')).and_then(|value| value.split_once('"').map(|(name, _)| name)).filter(|name| name.ends_with(".c"));
        if let Some(include) = include {
            let included = canonical.parent().unwrap_or(Path::new("")).join(include);
            let body = fs::read_to_string(&included).map_err(|error| format!("{}: {error}", included.display()))?;
            expanded.push_str(&expand_local_c_includes(&included, &body, active)?);
        } else {
            expanded.push_str(line);
            expanded.push('\n');
        }
    }
    active.remove(&canonical);
    Ok(expanded)
}

pub fn prepare(input: &Input, base_source: &str, _show_errors: bool) -> Result<Box<dyn Backend>, String> {
    Ok(Box::new(AlchemyBackend::prepare(input.source_path.clone(), base_source)?))
}

#[derive(Clone)]
struct AlchemyBackend {
    name: String,
    symbol: String,
    identity: String,
    target: PreparedTarget,
    target_instructions: Vec<Instruction>,
    target_source_instructions: Option<Vec<String>>,
    baseline_measurement: Measurement,
}

impl AlchemyBackend {
    fn prepare(path: PathBuf, base_source: &str) -> Result<Self, String> {
        let name = path.file_name().and_then(|value| value.to_str()).unwrap_or("alchemy").to_string();
        let target = PreparedTarget::prepare(path, base_source)?;
        let stem = target.owner_stem();
        let symbol = format!("Func_{stem}");
        let target_instructions = disassemble_bytes(target.expected())?;
        let target_source_instructions = if target.baseline_assembly().is_some() {
            let reference = root().join("games/gs1/asm").join(format!("{stem}.s"));
            fs::read_to_string(&reference).ok().map(|source| candidate_show::insns::gas_function_insns(&source, &symbol))
        } else {
            None
        };
        let baseline_source_instructions = target.baseline_assembly().map(|assembly| candidate_show::insns::gas_function_insns(assembly, &symbol));
        let baseline_measurement = alchemy_measurement(target.baseline(), &target_instructions, baseline_source_instructions.as_deref(), target_source_instructions.as_deref())?;
        let implementation_signature = current_executable_signature()?;
        let compiler_signature = compiler_core::bundle::compiler_bundle_signature();
        let host_signature = compiler_core::bundle::host_executable_signature(&ALCHEMY_HOST_TOOLS).map_err(|error| format!("Alchemy host tool signature: {error}"))?;
        Ok(Self { name, symbol, identity: alchemy_identity(&target.identity(), &implementation_signature, &compiler_signature, &host_signature), target, target_instructions, target_source_instructions, baseline_measurement })
    }
}

fn current_executable_signature() -> Result<String, String> {
    let path = std::env::current_exe().map_err(|error| format!("cannot locate current executable: {error}"))?;
    let bytes = fs::read(&path).map_err(|error| format!("cannot read current executable {}: {error}", path.display()))?;
    if bytes.is_empty() {
        return Err(format!("current executable {} is empty", path.display()));
    }
    Ok(compiler_core::sha256::hex(&bytes))
}

fn append_identity_field(stream: &mut Vec<u8>, value: &str) {
    stream.extend_from_slice(&(value.len() as u64).to_be_bytes());
    stream.extend_from_slice(value.as_bytes());
}

fn alchemy_identity(target_identity: &str, implementation_signature: &str, compiler_signature: &str, host_signature: &str) -> String {
    let mut stream = Vec::new();
    append_identity_field(&mut stream, "permuter-alchemy-backend-v5-owner-scoped-insns");
    append_identity_field(&mut stream, target_identity);
    append_identity_field(&mut stream, implementation_signature);
    append_identity_field(&mut stream, compiler_signature);
    append_identity_field(&mut stream, host_signature);
    compiler_core::sha256::hex(&stream)
}

impl Backend for AlchemyBackend {
    fn name(&self) -> &str {
        &self.name
    }

    fn identity(&self) -> String {
        self.identity.clone()
    }

    fn baseline(&self) -> Measurement {
        self.baseline_measurement.clone()
    }

    fn measure(&self, source: &str) -> Result<Measurement, String> {
        let (score, assembly) = self.target.compile(source)?;
        let candidate_source_instructions = assembly.as_deref().map(|assembly| candidate_show::insns::gas_function_insns(assembly, &self.symbol));
        alchemy_measurement(&score, &self.target_instructions, candidate_source_instructions.as_deref(), self.target_source_instructions.as_deref())
    }
}

/// Score an Alchemy candidate on its instruction stream, not its byte phase.
///
/// The raw halfword count collapses to noise the moment sizes diverge: one
/// missing instruction shifts every later byte, so a structural improvement
/// can read as a regression. Disassembling both sides and scoring the LCS
/// instruction diff (operand differences cheap, insertions and deletions
/// expensive) keeps the fitness aligned with what a reconstruction session
/// actually steers by. Byte equality remains the sole meaning of `exact`;
/// the halfword count survives as a tie-break within equal instruction
/// scores and in the summary.
fn rows_matching<'a>(rows: &[&'a str], prefix: &str) -> Vec<&'a str> {
    rows.iter().copied().filter(|r| r.starts_with(prefix)).collect()
}

fn rows_matching_any<'a>(rows: &[&'a str], prefixes: &[&str]) -> Vec<&'a str> {
    rows.iter().copied().filter(|r| prefixes.iter().any(|p| r.starts_with(p))).collect()
}

/// Ordered divergence: rows outside the longest common subsequence.
fn sequence_divergence(a: &[&str], e: &[&str]) -> usize {
    let width = e.len() + 1;
    let mut lcs = vec![0u32; (a.len() + 1) * width];
    for left in (0..a.len()).rev() {
        for right in (0..e.len()).rev() {
            lcs[left * width + right] = if a[left] == e[right] { 1 + lcs[(left + 1) * width + right + 1] } else { lcs[(left + 1) * width + right].max(lcs[left * width + right + 1]) };
        }
    }
    let common = lcs[0] as usize;
    (a.len() - common) + (e.len() - common)
}

/// FNV fingerprint over a row list, order-sensitive. Sort the rows first
/// for multiset identity.
fn rows_signature(rows: &[&str]) -> u64 {
    let mut hash = 0xcbf2_9ce4_8422_2325u64;
    for row in rows {
        for byte in row.as_bytes() {
            hash = (hash ^ *byte as u64).wrapping_mul(0x100_0000_01b3);
        }
        hash = (hash ^ u64::from(b'\n')).wrapping_mul(0x100_0000_01b3);
    }
    hash
}

/// Unordered divergence: symmetric multiset difference.
fn multiset_divergence(a: &[&str], e: &[&str]) -> usize {
    let mut counts: std::collections::BTreeMap<&str, i64> = std::collections::BTreeMap::new();
    for r in a {
        *counts.entry(r).or_insert(0) += 1;
    }
    for r in e {
        *counts.entry(r).or_insert(0) -= 1;
    }
    counts.values().map(|v| v.unsigned_abs() as usize).sum()
}

/// Fold allocator-owned spelling while preserving the instruction and source
/// shape. This is the same normalization contributors use when deciding
/// whether a large-owner edit changed structure: general registers and spill
/// slot numbers follow global coloring and must not lead the search.
fn structural_row(row: &str) -> String {
    fn allocator_register(token: &str) -> bool {
        if matches!(token, "sl" | "fp" | "ip") {
            return true;
        }
        token.strip_prefix('r').and_then(|digits| digits.parse::<u8>().ok()).is_some_and(|register| register <= 12)
    }

    let mut normalized = String::with_capacity(row.len());
    let bytes = row.as_bytes();
    let mut at = 0usize;
    while at < bytes.len() {
        if bytes[at].is_ascii_alphanumeric() || bytes[at] == b'_' {
            let start = at;
            at += 1;
            while at < bytes.len() && (bytes[at].is_ascii_alphanumeric() || bytes[at] == b'_') {
                at += 1;
            }
            let token = &row[start..at];
            if allocator_register(token) {
                normalized.push_str("rr");
            } else {
                normalized.push_str(token);
            }
        } else {
            normalized.push(bytes[at] as char);
            at += 1;
        }
    }

    let mut from = 0usize;
    while let Some(relative) = normalized[from..].find("[sp, #") {
        let start = from + relative;
        let Some(relative_end) = normalized[start..].find(']') else {
            break;
        };
        let end = start + relative_end;
        let offset = &normalized[start + "[sp, #".len()..end];
        if !offset.is_empty() && offset.strip_prefix('-').unwrap_or(offset).bytes().all(|byte| byte.is_ascii_digit()) {
            normalized.replace_range(start..=end, "[sp,N]");
            from = start + "[sp,N]".len();
        } else {
            from = end + 1;
        }
    }
    normalized
}

fn packed_alchemy_score(structural: usize, raw: usize, halfwords: usize) -> u64 {
    // The normalized source-instruction edit distance is the compass. Raw
    // canonical rows break structural ties, and linked halfwords break only
    // those remaining ties. Each field has a disjoint decimal range so a
    // downstream metric can never outweigh an upstream one.
    (structural as u64).saturating_mul(1_000_000_000).saturating_add((raw as u64).min(9_999).saturating_mul(100_000)).saturating_add((halfwords as u64).min(99_999))
}

/// Exact insertion/deletion distance and the candidate-side rows omitted by
/// one shortest edit script.
///
/// The old scorer materialised the full `(a.len() + 1) * (e.len() + 1)` LCS
/// matrix for every compilation.  A large owner has roughly 2,500 rows, so
/// eight workers repeatedly allocated about 200 MiB and visited more than six
/// million cells per candidate.  Myers visits only the edit frontier.  The
/// returned distance is exactly `a.len() + e.len() - 2 * LCS`, preserving the
/// fitness function, while the backtrack supplies the same kind of heat map:
/// candidate rows not selected by a shortest common subsequence.
fn row_diff(a: &[&str], e: &[&str]) -> (usize, Vec<usize>) {
    // One million frontier cells is about 8 MiB on a 64-bit host. Eight
    // workers therefore remain comfortably bounded even when a candidate is
    // unrelated to the reference. The exact distance needs only the prior
    // frontier; only optional heat backtracking is abandoned beyond the cap.
    const MAX_TRACE_CELLS: usize = 1_048_576;

    fn band_get(band: &[isize], depth: usize, diagonal: isize) -> isize {
        debug_assert!(diagonal >= -(depth as isize));
        debug_assert!(diagonal <= depth as isize);
        band[(diagonal + depth as isize) as usize]
    }

    let (a_len, e_len) = (a.len(), e.len());
    let max_depth = a_len + e_len;
    let mut trace: Vec<Vec<isize>> = Vec::new();
    let mut trace_cells = 0usize;
    let mut previous = Vec::new();
    let mut keeping_trace = true;

    for depth in 0..=max_depth {
        let depth_i = depth as isize;
        // Diagonals have the same parity as `depth`; the unused slots keep
        // indexing by `diagonal + depth` simple and branch-free in the loop.
        let mut current = vec![0isize; depth.saturating_mul(2) + 1];
        for step in 0..=depth {
            let diagonal = -depth_i + (step as isize * 2);
            let mut x = if depth == 0 {
                0
            } else if diagonal == -depth_i {
                let prior = if keeping_trace { trace.last().expect("prior Myers frontier") } else { &previous };
                band_get(prior, depth - 1, diagonal + 1)
            } else if diagonal == depth_i {
                let prior = if keeping_trace { trace.last().expect("prior Myers frontier") } else { &previous };
                band_get(prior, depth - 1, diagonal - 1) + 1
            } else {
                let prior = if keeping_trace { trace.last().expect("prior Myers frontier") } else { &previous };
                let left = band_get(prior, depth - 1, diagonal - 1);
                let down = band_get(prior, depth - 1, diagonal + 1);
                if left < down {
                    down
                } else {
                    left + 1
                }
            };
            let mut y = x - diagonal;
            while x < a_len as isize && y < e_len as isize && a[x as usize] == e[y as usize] {
                x += 1;
                y += 1;
            }
            current[(diagonal + depth_i) as usize] = x;
            if x == a_len as isize && y == e_len as isize {
                if !keeping_trace || trace_cells.saturating_add(current.len()) > MAX_TRACE_CELLS {
                    return (depth, Vec::new());
                }
                trace.push(current);

                let mut unmatched = Vec::new();
                let (mut bx, mut by) = (a_len as isize, e_len as isize);
                for back_depth in (1..=depth).rev() {
                    let back_i = back_depth as isize;
                    let diagonal = bx - by;
                    let prior = &trace[back_depth - 1];
                    let prior_diagonal = if diagonal == -back_i || (diagonal != back_i && band_get(prior, back_depth - 1, diagonal - 1) < band_get(prior, back_depth - 1, diagonal + 1)) { diagonal + 1 } else { diagonal - 1 };
                    let prior_x = band_get(prior, back_depth - 1, prior_diagonal);
                    let prior_y = prior_x - prior_diagonal;

                    // Walk back over the equal-row snake, then consume the
                    // one insertion or deletion represented by this depth.
                    while bx > prior_x && by > prior_y {
                        bx -= 1;
                        by -= 1;
                    }
                    if bx == prior_x {
                        by -= 1;
                    } else {
                        bx -= 1;
                        unmatched.push(bx as usize);
                    }
                }
                unmatched.reverse();
                return (depth, unmatched);
            }
        }
        if keeping_trace && trace_cells.saturating_add(current.len()) <= MAX_TRACE_CELLS {
            trace_cells += current.len();
            trace.push(current);
        } else {
            keeping_trace = false;
            trace.clear();
            previous = current;
        }
    }
    unreachable!("an insertion/deletion path always exists")
}

fn alchemy_measurement(score: &ByteScore, target_instructions: &[Instruction], candidate_source_instructions: Option<&[String]>, target_source_instructions: Option<&[String]>) -> Result<Measurement, String> {
    let (raw_a_owned, raw_e_owned, metric) = match (candidate_source_instructions, target_source_instructions) {
        (Some(candidate), Some(reference)) => (candidate.to_vec(), reference.to_vec(), "source"),
        _ => {
            let actual = disassemble_bytes(&score.actual)?;
            (actual.into_iter().map(|row| row.row).collect(), target_instructions.iter().map(|row| row.row.clone()).collect(), "binary")
        }
    };
    let raw_a: Vec<&str> = raw_a_owned.iter().map(String::as_str).collect();
    let raw_e: Vec<&str> = raw_e_owned.iter().map(String::as_str).collect();
    let normalized_a: Vec<String> = raw_a.iter().map(|row| structural_row(row)).collect();
    let normalized_e: Vec<String> = raw_e.iter().map(|row| structural_row(row)).collect();
    let a: Vec<&str> = normalized_a.iter().map(String::as_str).collect();
    let e: Vec<&str> = normalized_e.iter().map(String::as_str).collect();
    let (differing, unmatched) = row_diff(&a, &e);
    let (raw_differing, _) = row_diff(&raw_a, &raw_e);
    let common = (a.len() + e.len() - differing) / 2;
    let bl_rows = rows_matching(&raw_a, "bl ");
    let bl_divergence = sequence_divergence(&bl_rows, &rows_matching(&raw_e, "bl "));
    let bl_signature = rows_signature(&bl_rows);
    let store_rows = rows_matching_any(&a, &["strh ", "strb ", "str "]);
    let store_divergence = multiset_divergence(&store_rows, &rows_matching_any(&e, &["strh ", "strb ", "str "]));
    let store_signature = {
        let mut sorted = store_rows;
        sorted.sort_unstable();
        rows_signature(&sorted)
    };
    // Backtrack the LCS to mark our-side rows that are not part of the
    // common subsequence; their fractional positions steer heat-biased
    // mutation toward the regions that still differ.
    let heat = unmatched.into_iter().map(|left| left as f32 / a.len().max(1) as f32).collect();
    let byte: Measurement = score.into();
    Ok(Measurement {
        exact: byte.exact,
        // Exact linked bytes are the objective and must sort ahead of every
        // heuristic score even if source labels canonicalize differently.
        score: if byte.exact { 0 } else { packed_alchemy_score(differing, raw_differing, byte.differences) },
        differences: differing,
        expected_size: byte.expected_size,
        actual_size: byte.actual_size,
        first_difference: byte.first_difference,
        fingerprint: byte.fingerprint,
        heat,
        bl_divergence,
        store_divergence,
        bl_signature,
        store_signature,
        summary: format!("{differing} structural rows/{metric} ({} ours, {} reference); {raw_differing} raw rows; {}", a.len() - common, e.len() - common, byte.summary),
    })
}

struct TempDir(PathBuf);

impl TempDir {
    fn new(label: &str) -> Result<Self, String> {
        let time = SystemTime::now().duration_since(UNIX_EPOCH).map_err(|error| error.to_string())?.as_nanos();
        let count = TEMP_COUNTER.fetch_add(1, Ordering::Relaxed);
        let path = std::env::temp_dir().join(format!("permuter-{label}-{}-{time}-{count}", std::process::id()));
        fs::create_dir(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Ok(Self(path))
    }
}

impl Drop for TempDir {
    fn drop(&mut self) {
        let _ = fs::remove_dir_all(&self.0);
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
struct Instruction {
    mnemonic: String,
    row: String,
}

fn command_output(command: &[String], extra: &[&Path], directory: &Path) -> Result<Vec<u8>, String> {
    let output = Command::new(&command[0]).args(&command[1..]).args(extra).current_dir(directory).output().map_err(|error| format!("cannot run {}: {error}", command.join(" ")))?;
    if output.status.success() {
        Ok(output.stdout)
    } else {
        Err(format!("{} failed: {}", command.join(" "), String::from_utf8_lossy(&output.stderr).trim()))
    }
}

fn parse_disassembly(text: &str) -> Vec<Instruction> {
    text.lines()
        .filter_map(|line| {
            let (address, rest) = line.trim().split_once(':')?;
            if address.is_empty() || !address.bytes().all(|byte| byte.is_ascii_hexdigit()) {
                return None;
            }
            let fields = rest.split_whitespace().collect::<Vec<_>>();
            let first = fields.first()?.trim_end_matches(':');
            let mut index = usize::from(first.bytes().all(|byte| byte.is_ascii_hexdigit()));
            while fields.get(index).is_some_and(|field| field.len() == 2 && field.bytes().all(|byte| byte.is_ascii_hexdigit())) {
                index += 1;
            }
            let mnemonic = fields.get(index)?.trim().to_ascii_lowercase();
            if mnemonic == "..." || mnemonic.starts_with("r_") {
                return None;
            }
            let operands = fields[index + 1..].join(" ");
            let row = if operands.is_empty() { mnemonic.clone() } else { format!("{mnemonic} {operands}") };
            Some(Instruction { mnemonic, row })
        })
        .collect()
}

/// Disassemble a raw Thumb byte image for scoring. Pool words decode as
/// `.word` rows and count like instructions, which is correct: a changed
/// pool value is a real difference. PC-relative literal offsets and the
/// disassembler's address annotations are stripped, because they shift with
/// any size change and would charge one insertion as dozens of operand
/// differences.
fn disassemble_bytes(bytes: &[u8]) -> Result<Vec<Instruction>, String> {
    let temp = TempDir::new("insns")?;
    let path = temp.0.join("image.bin");
    fs::write(&path, bytes).map_err(|error| format!("{}: {error}", path.display()))?;
    let command: Vec<String> = ["arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "armv4t", "-M", "force-thumb"].into_iter().map(String::from).collect();
    let output = command_output(&command, &[path.as_path()], &temp.0)?;
    let text = String::from_utf8(output).map_err(|_| "objdump emitted non-UTF-8 output")?;
    let mut instructions = parse_disassembly(&text);
    for instruction in &mut instructions {
        if let Some(at) = instruction.row.find(" @") {
            instruction.row.truncate(at);
        }
        // Width suffixes follow branch distance, not source.
        if let Some(stripped) = instruction.mnemonic.strip_suffix(".n") {
            let bare = stripped.to_string();
            instruction.row = instruction.row.replacen(&instruction.mnemonic, &bare, 1);
            instruction.mnemonic = bare;
        } else if let Some(stripped) = instruction.mnemonic.strip_suffix(".w") {
            let bare = stripped.to_string();
            instruction.row = instruction.row.replacen(&instruction.mnemonic, &bare, 1);
            instruction.mnemonic = bare;
        }
        while let Some(start) = instruction.row.find("[pc, #") {
            let Some(length) = instruction.row[start..].find(']') else {
                break;
            };
            instruction.row.replace_range(start..start + length + 1, "[pc]");
        }
        // Branch and call targets are file offsets that shift with any size
        // change upstream; fold them so a moved block is not billed as a
        // difference on every branch row.
        if instruction.mnemonic == "bl" || instruction.mnemonic == "blx" || instruction.mnemonic.starts_with('b') && !instruction.mnemonic.starts_with("bic") && instruction.mnemonic != "bkpt" {
            if let Some(space) = instruction.row.find(' ') {
                let target = &instruction.row[space + 1..];
                if target.trim_start_matches("0x").chars().all(|c| c.is_ascii_hexdigit()) && !target.is_empty() {
                    instruction.row.truncate(space);
                    instruction.row.push_str(" <t>");
                }
            }
        }
    }
    if instructions.is_empty() {
        return Err("candidate image disassembled to no instructions".to_string());
    }
    Ok(instructions)
}

pub fn fingerprint(bytes: &[u8]) -> u64 {
    bytes.iter().fold(0xcbf2_9ce4_8422_2325, |hash, byte| (hash ^ *byte as u64).wrapping_mul(0x100_0000_01b3))
}

pub fn self_test() -> Result<(), String> {
    let base = alchemy_identity("target", "implementation-a", "compiler-a", "host-a");
    if base == alchemy_identity("target", "implementation-b", "compiler-a", "host-a") || base == alchemy_identity("target", "implementation-a", "compiler-b", "host-a") || base == alchemy_identity("target", "implementation-a", "compiler-a", "host-b") {
        return Err("Alchemy backend identity did not invalidate a changed input".into());
    }
    if current_executable_signature()?.len() != 64 {
        return Err("current executable signature is not SHA-256".into());
    }
    Ok(())
}
