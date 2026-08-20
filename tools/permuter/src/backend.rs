//! Compiler/scorer boundary for permutation jobs.
//!
//! The permutation engine depends on this interface rather than on Golden Sun
//! paths.  The native Alchemy adapter compares linked bytes directly; the
//! directory adapter preserves decomp-permuter's portable `base.c` /
//! `target.o` / `compile.sh` workflow for other compilers and architectures.

use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::{Command, Stdio};
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

use alchemy_routing::routing::root;

use crate::compile::{PreparedTarget, Score as ByteScore};

static TEMP_COUNTER: AtomicU64 = AtomicU64::new(0);

/// The host tools used by both Alchemy candidate-verification paths.
///
/// `objdump` is intentionally absent: Alchemy candidates are scored from the
/// linked bytes, while the directory backend below owns its own command and
/// identity.
const ALCHEMY_HOST_TOOLS: [&str; 4] = [
    "arm-none-eabi-as",
    "arm-none-eabi-nm",
    "arm-none-eabi-ld",
    "arm-none-eabi-objcopy",
];

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Measurement {
    pub exact: bool,
    pub score: u64,
    pub differences: usize,
    pub expected_size: usize,
    pub actual_size: usize,
    pub first_difference: Option<usize>,
    pub fingerprint: u64,
    pub summary: String,
}

impl Measurement {
    pub fn failed(message: &str) -> Self {
        Self {
            exact: false,
            score: u64::MAX,
            differences: usize::MAX,
            expected_size: 0,
            actual_size: 0,
            first_difference: None,
            fingerprint: 0,
            summary: message.to_string(),
        }
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
            summary: format!(
                "{} differing halfwords, {} / {} bytes",
                score.differing_halfwords, score.actual_size, score.expected_size
            ),
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
    pub requested: PathBuf,
    pub source_path: PathBuf,
    pub source: String,
    pub directory_mode: bool,
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
        let preferred = requested.join("base.c");
        let source_path = if preferred.is_file() {
            preferred
        } else {
            let mut sources = fs::read_dir(&requested)
                .map_err(|error| format!("{}: {error}", requested.display()))?
                .flatten()
                .map(|entry| entry.path())
                .filter(|entry| entry.extension().and_then(|value| value.to_str()) == Some("c"))
                .collect::<Vec<_>>();
            sources.sort();
            match sources.len() {
                0 => return Err(format!("{} contains no C source", requested.display())),
                1 => sources.remove(0),
                _ => {
                    return Err(format!(
                        "{} contains multiple C sources and no base.c",
                        requested.display()
                    ))
                }
            }
        };
        let source = fs::read_to_string(&source_path)
            .map_err(|error| format!("{}: {error}", source_path.display()))?;
        Ok(Input {
            requested,
            source_path,
            source,
            directory_mode: true,
        })
    } else {
        let source = fs::read_to_string(&requested)
            .map_err(|error| format!("{}: {error}", requested.display()))?;
        Ok(Input {
            requested: requested.clone(),
            source_path: requested,
            source,
            directory_mode: false,
        })
    }
}

pub fn prepare(
    input: &Input,
    base_source: &str,
    show_errors: bool,
) -> Result<Box<dyn Backend>, String> {
    if input.directory_mode {
        Ok(Box::new(DirectoryBackend::prepare(
            input.requested.clone(),
            base_source,
            show_errors,
        )?))
    } else {
        Ok(Box::new(AlchemyBackend::prepare(
            input.source_path.clone(),
            base_source,
        )?))
    }
}

#[derive(Clone)]
struct AlchemyBackend {
    name: String,
    identity: String,
    target: PreparedTarget,
    target_instructions: Vec<Instruction>,
    baseline_measurement: Measurement,
}

impl AlchemyBackend {
    fn prepare(path: PathBuf, base_source: &str) -> Result<Self, String> {
        let name = path
            .file_name()
            .and_then(|value| value.to_str())
            .unwrap_or("alchemy")
            .to_string();
        let target = PreparedTarget::prepare(path, base_source)?;
        let target_instructions = disassemble_bytes(target.expected())?;
        let baseline_measurement =
            alchemy_measurement(target.baseline(), &target_instructions)?;
        let implementation_signature = current_executable_signature()?;
        let compiler_signature = alchemy_bundle::bundle::compiler_bundle_signature();
        let host_signature = alchemy_bundle::bundle::host_executable_signature(&ALCHEMY_HOST_TOOLS)
            .map_err(|error| format!("Alchemy host tool signature: {error}"))?;
        Ok(Self {
            name,
            identity: alchemy_identity(
                &target.identity(),
                &implementation_signature,
                &compiler_signature,
                &host_signature,
            ),
            target,
            target_instructions,
            baseline_measurement,
        })
    }
}

fn current_executable_signature() -> Result<String, String> {
    let path = std::env::current_exe()
        .map_err(|error| format!("cannot locate current executable: {error}"))?;
    let bytes = fs::read(&path)
        .map_err(|error| format!("cannot read current executable {}: {error}", path.display()))?;
    if bytes.is_empty() {
        return Err(format!("current executable {} is empty", path.display()));
    }
    Ok(alchemy_bundle::sha256::hex(&bytes))
}

fn append_identity_field(stream: &mut Vec<u8>, value: &str) {
    stream.extend_from_slice(&(value.len() as u64).to_be_bytes());
    stream.extend_from_slice(value.as_bytes());
}

fn alchemy_identity(
    target_identity: &str,
    implementation_signature: &str,
    compiler_signature: &str,
    host_signature: &str,
) -> String {
    let mut stream = Vec::new();
    append_identity_field(&mut stream, "permuter-alchemy-backend-v3-insns");
    append_identity_field(&mut stream, target_identity);
    append_identity_field(&mut stream, implementation_signature);
    append_identity_field(&mut stream, compiler_signature);
    append_identity_field(&mut stream, host_signature);
    alchemy_bundle::sha256::hex(&stream)
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
        let score = self.target.compile(source)?;
        alchemy_measurement(&score, &self.target_instructions)
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
fn alchemy_measurement(
    score: &ByteScore,
    target_instructions: &[Instruction],
) -> Result<Measurement, String> {
    let actual_instructions = disassemble_bytes(&score.actual)?;
    // Whole-row LCS: a row either matches exactly (mnemonic and operands) or
    // it is a differing row. The mnemonic-anchored scorer traded real rows
    // for operand-level partial credit and walked candidates away from the
    // reference while its number improved; byte-exactness needs row
    // identity, so row identity is the fitness.
    let a: Vec<&str> = actual_instructions.iter().map(|i| i.row.as_str()).collect();
    let e: Vec<&str> = target_instructions.iter().map(|i| i.row.as_str()).collect();
    let width = e.len() + 1;
    let mut lcs = vec![0u32; (a.len() + 1) * width];
    for left in (0..a.len()).rev() {
        for right in (0..e.len()).rev() {
            lcs[left * width + right] = if a[left] == e[right] {
                1 + lcs[(left + 1) * width + right + 1]
            } else {
                lcs[(left + 1) * width + right].max(lcs[left * width + right + 1])
            };
        }
    }
    let common = lcs[0] as usize;
    let differing = (a.len() - common) + (e.len() - common);
    let byte: Measurement = score.into();
    Ok(Measurement {
        exact: byte.exact,
        score: (differing as u64)
            .saturating_mul(100_000)
            .saturating_add((byte.differences as u64).min(99_999)),
        differences: differing,
        expected_size: byte.expected_size,
        actual_size: byte.actual_size,
        first_difference: byte.first_difference,
        fingerprint: byte.fingerprint,
        summary: format!(
            "{differing} differing rows ({} ours, {} reference); {}",
            a.len() - common,
            e.len() - common,
            byte.summary
        ),
    })
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
    let command: Vec<String> = [
        "arm-none-eabi-objdump",
        "-D",
        "-b",
        "binary",
        "-m",
        "armv4t",
        "-M",
        "force-thumb",
    ]
    .into_iter()
    .map(String::from)
    .collect();
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
        if instruction.mnemonic == "bl"
            || instruction.mnemonic == "blx"
            || instruction.mnemonic.starts_with('b')
                && !instruction.mnemonic.starts_with("bic")
                && instruction.mnemonic != "bkpt"
        {
            if let Some(space) = instruction.row.find(' ') {
                let target = &instruction.row[space + 1..];
                if target
                    .trim_start_matches("0x")
                    .chars()
                    .all(|c| c.is_ascii_hexdigit())
                    && !target.is_empty()
                {
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

struct TempDir(PathBuf);

impl TempDir {
    fn new(label: &str) -> Result<Self, String> {
        let time = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .map_err(|error| error.to_string())?
            .as_nanos();
        let count = TEMP_COUNTER.fetch_add(1, Ordering::Relaxed);
        let path = std::env::temp_dir().join(format!(
            "permuter-generic-{label}-{}-{time}-{count}",
            std::process::id()
        ));
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

#[derive(Clone)]
struct DirectoryBackend {
    name: String,
    directory: PathBuf,
    compile_command: Vec<String>,
    objdump_command: Vec<String>,
    target: Vec<Instruction>,
    baseline: Measurement,
    identity: String,
    show_errors: bool,
}

fn unquote(value: &str) -> String {
    let value = value.trim();
    if value.len() >= 2
        && ((value.starts_with('"') && value.ends_with('"'))
            || (value.starts_with('\'') && value.ends_with('\'')))
    {
        value[1..value.len() - 1].to_string()
    } else {
        value.to_string()
    }
}

fn settings(path: &Path) -> Result<BTreeMap<String, String>, String> {
    if !path.is_file() {
        return Ok(BTreeMap::new());
    }
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut values = BTreeMap::new();
    for line in text.lines() {
        let line = line.split('#').next().unwrap_or("").trim();
        if line.is_empty() || line.starts_with('[') {
            continue;
        }
        if let Some((key, value)) = line.split_once('=') {
            values.insert(key.trim().to_string(), unquote(value));
        }
    }
    Ok(values)
}

fn split_command(text: &str) -> Result<Vec<String>, String> {
    let mut args = Vec::new();
    let mut current = String::new();
    let mut quote = None;
    let mut escaped = false;
    for character in text.chars() {
        if escaped {
            current.push(character);
            escaped = false;
            continue;
        }
        if character == '\\' && quote != Some('\'') {
            escaped = true;
            continue;
        }
        if matches!(character, '\'' | '"') {
            if quote == Some(character) {
                quote = None;
            } else if quote.is_none() {
                quote = Some(character);
            } else {
                current.push(character);
            }
            continue;
        }
        if character.is_whitespace() && quote.is_none() {
            if !current.is_empty() {
                args.push(std::mem::take(&mut current));
            }
        } else {
            current.push(character);
        }
    }
    if escaped || quote.is_some() {
        return Err("unterminated escape or quote in command".into());
    }
    if !current.is_empty() {
        args.push(current);
    }
    if args.is_empty() {
        return Err("empty command".into());
    }
    Ok(args)
}

fn command_output(
    command: &[String],
    extra: &[&Path],
    directory: &Path,
) -> Result<Vec<u8>, String> {
    let mut process = Command::new(&command[0]);
    process
        .args(&command[1..])
        .args(extra)
        .current_dir(directory);
    let output = process
        .output()
        .map_err(|error| format!("cannot run {}: {error}", command.join(" ")))?;
    if !output.status.success() {
        return Err(format!(
            "{} failed: {}",
            command.join(" "),
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(output.stdout)
}

fn executable_path(program: &str, directory: &Path) -> Option<PathBuf> {
    let direct = PathBuf::from(program);
    if direct.is_absolute() && direct.is_file() {
        return Some(direct);
    }
    if program.contains('/') {
        let relative = directory.join(program);
        return relative.is_file().then_some(relative);
    }
    std::env::var_os("PATH").and_then(|path| {
        std::env::split_paths(&path)
            .map(|directory| directory.join(program))
            .find(|candidate| candidate.is_file())
    })
}

fn is_hex_word(value: &str) -> bool {
    !value.is_empty() && value.bytes().all(|byte| byte.is_ascii_hexdigit())
}

fn parse_disassembly(text: &str) -> Vec<Instruction> {
    let mut instructions: Vec<Instruction> = Vec::new();
    for line in text.lines() {
        let Some((address, rest)) = line.trim().split_once(':') else {
            continue;
        };
        if !is_hex_word(address.trim()) {
            continue;
        }
        let fields = rest.split_whitespace().collect::<Vec<_>>();
        if fields.is_empty() {
            continue;
        }
        // GNU objdump prints either one instruction word (ARM/MIPS), a run of
        // byte columns (PowerPC/x86), or no raw encoding when explicitly asked.
        // Do not find the mnemonic by looking for the first non-hex token:
        // perfectly ordinary mnemonics such as `add`, `adc`, and `b` consist
        // entirely of hexadecimal letters.
        let mut index = 0usize;
        let first = fields[0].trim_end_matches(':');
        if is_hex_word(first) && matches!(first.len(), 2 | 4 | 8 | 16) {
            index = 1;
            while fields
                .get(index)
                .is_some_and(|field| field.len() == 2 && is_hex_word(field))
            {
                index += 1;
            }
        }
        let Some(field) = fields.get(index) else {
            continue;
        };
        let mnemonic = field.trim().to_ascii_lowercase();
        if mnemonic.starts_with("r_") {
            // Relocation identity is part of exact instruction identity. If it
            // is discarded, references to two different symbols can compare
            // equal because their placeholder instruction bytes are the same.
            let relocation = fields[index..].join(" ");
            if let Some(previous) = instructions.last_mut() {
                previous.row.push_str(" | ");
                previous.row.push_str(&relocation);
            } else {
                instructions.push(Instruction {
                    mnemonic: "<relocation>".to_string(),
                    row: relocation,
                });
            }
            continue;
        }
        if mnemonic == "..." {
            continue;
        }
        let operands = fields[index + 1..].join(" ");
        let row = if operands.is_empty() {
            mnemonic.clone()
        } else {
            format!("{mnemonic} {operands}")
        };
        instructions.push(Instruction { mnemonic, row });
    }
    instructions
}

fn disassemble(
    command: &[String],
    object: &Path,
    directory: &Path,
) -> Result<Vec<Instruction>, String> {
    let output = command_output(command, &[object], directory)?;
    let text = String::from_utf8(output).map_err(|_| "objdump emitted non-UTF-8 output")?;
    let instructions = parse_disassembly(&text);
    if instructions.is_empty() {
        return Err(format!("{} emitted no instructions", command.join(" ")));
    }
    Ok(instructions)
}

fn row_fingerprint(lines: &[Instruction]) -> u64 {
    let mut bytes = Vec::new();
    for line in lines {
        bytes.extend_from_slice(line.row.as_bytes());
        bytes.push(b'\n');
    }
    fingerprint(&bytes)
}

fn instruction_score(actual: &[Instruction], expected: &[Instruction]) -> Measurement {
    let rows_equal = actual == expected;
    let width = expected.len() + 1;
    let mut lcs = vec![0usize; (actual.len() + 1) * width];
    for left in (0..actual.len()).rev() {
        for right in (0..expected.len()).rev() {
            lcs[left * width + right] = if actual[left].mnemonic == expected[right].mnemonic {
                1 + lcs[(left + 1) * width + right + 1]
            } else {
                lcs[(left + 1) * width + right].max(lcs[left * width + right + 1])
            };
        }
    }

    let mut left = 0usize;
    let mut right = 0usize;
    let mut operand_differences = 0usize;
    let mut inserted = Vec::new();
    let mut deleted = Vec::new();
    let mut first_difference = None;
    while left < actual.len() && right < expected.len() {
        if actual[left].mnemonic == expected[right].mnemonic {
            if actual[left].row != expected[right].row {
                operand_differences += 1;
                first_difference.get_or_insert(right);
            }
            left += 1;
            right += 1;
        } else if lcs[(left + 1) * width + right] >= lcs[left * width + right + 1] {
            inserted.push(actual[left].row.clone());
            first_difference.get_or_insert(right);
            left += 1;
        } else {
            deleted.push(expected[right].row.clone());
            first_difference.get_or_insert(right);
            right += 1;
        }
    }
    inserted.extend(actual[left..].iter().map(|line| line.row.clone()));
    deleted.extend(expected[right..].iter().map(|line| line.row.clone()));
    if first_difference.is_none() && (!inserted.is_empty() || !deleted.is_empty()) {
        first_difference = Some(left.min(right));
    }

    let mut inserted_counts = BTreeMap::<String, usize>::new();
    let mut deleted_counts = BTreeMap::<String, usize>::new();
    for row in inserted {
        *inserted_counts.entry(row).or_default() += 1;
    }
    for row in deleted {
        *deleted_counts.entry(row).or_default() += 1;
    }
    let reorderings = inserted_counts
        .iter()
        .map(|(row, count)| count.min(deleted_counts.get(row).unwrap_or(&0)))
        .sum::<usize>();
    let insertions = inserted_counts.values().sum::<usize>() - reorderings;
    let deletions = deleted_counts.values().sum::<usize>() - reorderings;
    let differences = operand_differences + reorderings + insertions + deletions;
    let score = operand_differences as u64 * 5
        + reorderings as u64 * 60
        + insertions as u64 * 100
        + deletions as u64 * 100;
    Measurement {
        exact: rows_equal,
        score,
        differences,
        expected_size: expected.len(),
        actual_size: actual.len(),
        first_difference,
        fingerprint: row_fingerprint(actual),
        summary: format!(
            "{operand_differences} operand, {reorderings} reordered, {insertions} inserted, {deletions} deleted instructions"
        ),
    }
}

impl DirectoryBackend {
    fn prepare(directory: PathBuf, base_source: &str, show_errors: bool) -> Result<Self, String> {
        let values = settings(&directory.join("settings.toml"))?;
        let compile_path = directory.join("compile.sh");
        if !compile_path.is_file() {
            return Err(format!("{} is missing", compile_path.display()));
        }
        let compile_command = if let Some(command) = values.get("compile_command") {
            split_command(command)?
        } else {
            vec![compile_path.to_string_lossy().into_owned()]
        };
        let objdump_command = split_command(
            values
                .get("objdump_command")
                .map(String::as_str)
                .unwrap_or("objdump -drz"),
        )?;
        let target_path = directory.join("target.o");
        if !target_path.is_file() {
            return Err(format!("{} is missing", target_path.display()));
        }
        let target = disassemble(&objdump_command, &target_path, &directory)?;
        let mut identity_bytes = fs::read(&target_path)
            .map_err(|error| format!("{}: {error}", target_path.display()))?;
        identity_bytes.extend_from_slice(
            &fs::read(&compile_path)
                .map_err(|error| format!("{}: {error}", compile_path.display()))?,
        );
        identity_bytes.extend_from_slice(compile_command.join("\0").as_bytes());
        identity_bytes.extend_from_slice(objdump_command.join("\0").as_bytes());
        for command in [&compile_command, &objdump_command] {
            if let Some(path) = executable_path(&command[0], &directory) {
                if let Ok(bytes) = fs::read(path) {
                    identity_bytes.extend_from_slice(&bytes);
                }
            }
        }
        let settings_path = directory.join("settings.toml");
        if let Ok(bytes) = fs::read(settings_path) {
            identity_bytes.extend_from_slice(&bytes);
        }
        let identity = alchemy_bundle::sha256::hex(&identity_bytes);
        let mut backend = Self {
            name: directory
                .file_name()
                .and_then(|value| value.to_str())
                .unwrap_or("directory")
                .to_string(),
            directory,
            compile_command,
            objdump_command,
            target,
            baseline: Measurement::failed("baseline not prepared"),
            identity,
            show_errors,
        };
        backend.baseline = backend.measure(base_source)?;
        Ok(backend)
    }

    fn compile(&self, source: &str) -> Result<Vec<Instruction>, String> {
        let temp = TempDir::new("compile")?;
        let input = temp.0.join("input.c");
        let output = temp.0.join("output.o");
        fs::write(&input, source).map_err(|error| format!("{}: {error}", input.display()))?;
        let mut command = Command::new(&self.compile_command[0]);
        command
            .args(&self.compile_command[1..])
            .arg(&input)
            .arg("-o")
            .arg(&output)
            .current_dir(&self.directory);
        if self.show_errors {
            command.stdout(Stdio::inherit()).stderr(Stdio::inherit());
        } else {
            command.stdout(Stdio::null()).stderr(Stdio::null());
        }
        let status = command
            .status()
            .map_err(|error| format!("cannot run {}: {error}", self.compile_command.join(" ")))?;
        if !status.success() {
            return Err(format!("compiler exited with {status}"));
        }
        disassemble(&self.objdump_command, &output, &self.directory)
    }
}

impl Backend for DirectoryBackend {
    fn name(&self) -> &str {
        &self.name
    }

    fn identity(&self) -> String {
        self.identity.clone()
    }

    fn baseline(&self) -> Measurement {
        self.baseline.clone()
    }

    fn measure(&self, source: &str) -> Result<Measurement, String> {
        Ok(instruction_score(&self.compile(source)?, &self.target))
    }
}

pub fn fingerprint(bytes: &[u8]) -> u64 {
    bytes.iter().fold(0xcbf2_9ce4_8422_2325, |hash, byte| {
        (hash ^ *byte as u64).wrapping_mul(0x100_0000_01b3)
    })
}

pub fn self_test() -> Result<(), String> {
    let command = split_command("objdump -d 'file with spaces.o'")?;
    if command != ["objdump", "-d", "file with spaces.o"] {
        return Err(format!("command parser drifted: {command:?}"));
    }
    let parsed = parse_disassembly(
        "00000000 <f>:\n   0:\te0800001\tadd r0, r0, r1\n   4:\tea000001\tb 10 <f+0x10>\n",
    );
    if parsed.len() != 2
        || parsed[0].mnemonic != "add"
        || parsed[1].mnemonic != "b"
        || !parsed[1].row.contains("10 <f+0x10>")
    {
        return Err(format!("generic disassembly parser drifted: {parsed:?}"));
    }
    let exact = instruction_score(&parsed, &parsed);
    if !exact.exact || exact.score != 0 {
        return Err("generic scorer rejected identical instructions".into());
    }
    let changed = parse_disassembly("0: e0801001 add r1, r0, r1\n");
    let measured = instruction_score(&changed, &parsed[..1]);
    if measured.exact || measured.score != 5 {
        return Err(format!("operand penalty drifted: {measured:?}"));
    }
    let portable = parse_disassembly(
        "0: 27bdffe0 addiu sp,sp,-32\n4: R_MIPS_26 target\n8: 94 21 ff e0 stwu r1,-32(r1)\n",
    );
    if portable
        .iter()
        .map(|line| line.mnemonic.as_str())
        .collect::<Vec<_>>()
        != ["addiu", "stwu"]
    {
        return Err(format!(
            "MIPS/PowerPC disassembly portability drifted: {portable:?}"
        ));
    }
    let target_relocation =
        parse_disassembly("0: e59f0000 ldr r0, [pc]\n0: R_ARM_ABS32 expected_symbol\n");
    let candidate_relocation =
        parse_disassembly("0: e59f0000 ldr r0, [pc]\n0: R_ARM_ABS32 other_symbol\n");
    if instruction_score(&candidate_relocation, &target_relocation).exact {
        return Err("generic scorer discarded relocation identity".into());
    }
    let base = alchemy_identity("target", "implementation-a", "compiler-a", "host-a");
    if base == alchemy_identity("target", "implementation-b", "compiler-a", "host-a")
        || base == alchemy_identity("target", "implementation-a", "compiler-b", "host-a")
        || base == alchemy_identity("target", "implementation-a", "compiler-a", "host-b")
    {
        return Err("Alchemy backend identity did not invalidate a changed input".into());
    }
    if current_executable_signature()?.len() != 64 {
        return Err("current executable signature is not SHA-256".into());
    }
    Ok(())
}
