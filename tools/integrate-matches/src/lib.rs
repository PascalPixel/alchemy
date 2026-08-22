//! Main-image exact-C adoption gate.
//!
//! A candidate is adoptable only when its linked bytes equal the bytes produced
//! by the corresponding hand-written assembly at the same ROM address.
//! `--apply` performs the move and cleanup only after that proof.

use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;
use std::sync::OnceLock;

use compiler_core::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use compiler_core::routing::{root, CompilerTarget};
use compiler_core::{external_symbol, external_symbol_assembly, CALL_VIA_BASE};
use regex::Regex;

pub mod entry_main;

pub const USAGE: &str = "usage: integrate-matches [-h] [--apply] directory";
pub const ROOT_OVERRIDE: &str = "ALCHEMY_INTEGRATE_ROOT";

#[derive(Clone, Copy, PartialEq, Eq)]
enum Kind {
    Asm,
    C,
}

struct Output {
    code: i32,
    stdout: String,
    stderr: String,
}

struct Cleanup {
    removed: usize,
    closed: bool,
}

fn pattern(name: &str) -> &'static Regex {
    static ADDRESS: OnceLock<Regex> = OnceLock::new();
    static CANDIDATE: OnceLock<Regex> = OnceLock::new();
    static SYMBOL: OnceLock<Regex> = OnceLock::new();
    static HELPER: OnceLock<Regex> = OnceLock::new();
    match name {
        "address" => ADDRESS.get_or_init(|| Regex::new(r"^08[0-9a-f]{6}$").unwrap()),
        "candidate" => CANDIDATE.get_or_init(|| Regex::new(r"^src_.*\.c$").unwrap()),
        "symbol" => SYMBOL.get_or_init(|| Regex::new(r"^Func_[0-9a-f]{8}$").unwrap()),
        "helper" => {
            HELPER.get_or_init(|| Regex::new(r"(?m)(?:inline_fn|^(?:static|inline)\b)").unwrap())
        }
        _ => unreachable!(),
    }
}

fn valid_address(stem: &str) -> bool {
    pattern("address").is_match(stem)
}

fn parse_hex(value: &str) -> Option<u64> {
    u64::from_str_radix(value.trim_start_matches("0x").trim_start_matches("0X"), 16).ok()
}

fn command(argv: &[String], cwd: &Path) -> Result<Output, String> {
    let program = argv.first().ok_or_else(|| "empty command".to_string())?;
    let output = Command::new(program)
        .args(&argv[1..])
        .current_dir(cwd)
        .output()
        .map_err(|error| format!("{program}: {error}"))?;
    Ok(Output {
        code: output.status.code().unwrap_or(-1),
        stdout: String::from_utf8_lossy(&output.stdout).into_owned(),
        stderr: String::from_utf8_lossy(&output.stderr).into_owned(),
    })
}

fn command_error(output: &Output) -> &str {
    if output.stderr.is_empty() {
        output.stdout.trim()
    } else {
        output.stderr.trim()
    }
}

fn argv(parts: &[&str]) -> Vec<String> {
    parts.iter().map(|part| (*part).to_string()).collect()
}

fn assemble(object: &str, listing: &str) -> Vec<String> {
    argv(&[
        "arm-none-eabi-as",
        "-mcpu=arm7tdmi",
        "-mthumb-interwork",
        "-o",
        object,
        listing,
    ])
}

fn linked_extent(output: &str, target: &str, address: u64, length: usize) -> Result<usize, String> {
    let limit = address
        .checked_add(length as u64)
        .ok_or_else(|| "compiled function extent differs".to_string())?;
    let mut entries = Vec::new();
    for line in output.lines() {
        let fields: Vec<_> = line.split_whitespace().collect();
        if fields.len() < 4 || !matches!(fields[fields.len() - 2], "T" | "t") {
            continue;
        }
        let name = fields[fields.len() - 1];
        if !pattern("symbol").is_match(name) {
            continue;
        }
        let (Some(start), Some(size)) = (parse_hex(fields[0]), parse_hex(fields[1])) else {
            continue;
        };
        if start >= address && start < limit {
            let Some(end) = start.checked_add(size) else {
                return Err("compiled function extent differs".to_string());
            };
            entries.push((start, end, name));
        }
    }
    if !entries
        .iter()
        .any(|(start, _, name)| *start == address && *name == target)
        || entries
            .iter()
            .any(|(_, end, _)| *end <= address || *end > limit)
    {
        return Err("compiled function extent differs".to_string());
    }
    let end = entries.iter().map(|(_, end, _)| *end).max().unwrap();
    usize::try_from(end - address).map_err(|_| "compiled function extent differs".to_string())
}

fn first_difference(left: &[u8], right: &[u8]) -> Option<usize> {
    left.iter()
        .zip(right)
        .position(|(left, right)| left != right)
        .or_else(|| (left.len() != right.len()).then_some(left.len().min(right.len())))
}

fn linked_bytes(
    stem: &str,
    source: &Path,
    scratch: &Path,
    kind: Kind,
    repository: &Path,
) -> Result<Vec<u8>, String> {
    let address = parse_hex(stem)
        .filter(|_| valid_address(stem))
        .ok_or_else(|| "invalid source address".to_string())?;
    fs::create_dir_all(scratch).map_err(|error| format!("{}: {error}", scratch.display()))?;
    let prefix = scratch.join(format!(
        "{stem}.{}probe",
        if kind == Kind::Asm { "asm" } else { "c" }
    ));
    let prefix = prefix.to_string_lossy().into_owned();
    let listing = format!("{prefix}.s");
    let object = format!("{prefix}.o");

    if kind == Kind::C {
        let routing_source = repository.join("games/gs1/src").join(format!("{stem}.c"));
        let mut options = SourceToAssemblyPlanOptions::new(
            CompilerTarget::Gs1,
            routing_source.to_string_lossy().to_string(),
            source.to_string_lossy().to_string(),
            listing.clone(),
        );
        options.preprocessed_output = Some(format!("{prefix}.i"));
        for step in source_to_assembly_plan(&options)?.steps {
            let result = command(&step.command, root())?;
            if result.code != 0 {
                return Err(format!(
                    "{} failed: {}",
                    step.kind.as_str(),
                    command_error(&result)
                ));
            }
        }
    } else {
        fs::copy(source, &listing).map_err(|error| format!("{}: {error}", source.display()))?;
    }

    let result = command(&assemble(&object, &listing), root())?;
    if result.code != 0 {
        return Err(format!("assembler failed: {}", command_error(&result)));
    }
    let undefined = command(&argv(&["arm-none-eabi-nm", "-u", &object]), root())?;
    if undefined.code != 0 {
        return Err(format!("nm failed: {}", command_error(&undefined)));
    }
    let names: Vec<String> = undefined
        .stdout
        .lines()
        .filter_map(|line| line.split_whitespace().last().map(str::to_owned))
        .collect();
    for name in &names {
        if external_symbol(name, CALL_VIA_BASE).is_none() {
            return Err(format!("unsupported external symbol {name}"));
        }
    }
    let symbols = format!("{prefix}.symbols");
    let symbols_source = format!("{symbols}.s");
    let symbols_object = format!("{symbols}.o");
    let mut text = String::from(".syntax unified\n.thumb\n");
    for name in &names {
        text.push_str(&external_symbol_assembly(name, CALL_VIA_BASE)?);
    }
    fs::write(&symbols_source, text).map_err(|error| format!("{symbols_source}: {error}"))?;
    let result = command(&assemble(&symbols_object, &symbols_source), root())?;
    if result.code != 0 {
        return Err(format!(
            "symbol assembler failed: {}",
            command_error(&result)
        ));
    }

    let address_text = format!("{address:08x}");
    let elf = format!("{prefix}.elf");
    let binary = format!("{prefix}.bin");
    let result = command(
        &argv(&[
            "arm-none-eabi-ld",
            &format!("-Ttext=0x{address_text}"),
            "-e",
            &format!("Func_{address_text}"),
            "-o",
            &elf,
            &object,
            &symbols_object,
        ]),
        root(),
    )?;
    if result.code != 0 {
        return Err(format!("linker failed: {}", command_error(&result)));
    }
    let result = command(
        &argv(&[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            "-j",
            ".text",
            &elf,
            &binary,
        ]),
        root(),
    )?;
    if result.code != 0 {
        return Err(format!("objcopy failed: {}", command_error(&result)));
    }
    let bytes = fs::read(&binary).map_err(|error| format!("{binary}: {error}"))?;
    if kind == Kind::Asm {
        return Ok(bytes);
    }
    let symbols = command(
        &argv(&["arm-none-eabi-nm", "-S", "--defined-only", &elf]),
        root(),
    )?;
    if symbols.code != 0 {
        return Err(format!("nm failed: {}", command_error(&symbols)));
    }
    let extent = linked_extent(
        &symbols.stdout,
        &format!("Func_{address_text}"),
        address,
        bytes.len(),
    )?;
    Ok(bytes[..extent].to_vec())
}

fn root_directory() -> PathBuf {
    std::env::var_os(ROOT_OVERRIDE)
        .filter(|value| !value.is_empty())
        .map(PathBuf::from)
        .unwrap_or_else(|| root().to_path_buf())
}

fn close_dossier(path: &Path, state: &str) -> Result<bool, String> {
    if !path.exists() {
        return Ok(false);
    }
    let source =
        fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    let mut offset = 0;
    let mut updated = None;
    for line in source.split_inclusive('\n') {
        let content = line.strip_suffix('\n').unwrap_or(line);
        let content = content.strip_suffix('\r').unwrap_or(content);
        if content.starts_with("State:") {
            let ending = &line[content.len()..];
            updated = Some(format!(
                "{}{}{}{}",
                &source[..offset],
                state,
                ending,
                &source[offset + line.len()..]
            ));
            break;
        }
        offset += line.len();
    }
    let updated = updated.unwrap_or_else(|| {
        if source.starts_with('#') {
            source.find('\n').map_or_else(
                || source.clone(),
                |end| format!("{}\n{}\n{}", &source[..end + 1], state, &source[end + 1..]),
            )
        } else {
            source.clone()
        }
    });
    fs::write(path, updated).map_err(|error| format!("{}: {error}", path.display()))?;
    Ok(true)
}

fn cleanup(stem: &str, work_root: &Path, date: &str) -> Result<Cleanup, String> {
    if !valid_address(stem) {
        return Err("invalid cleanup address".to_string());
    }
    let mut removed = 0;
    if work_root.exists() {
        let prefix = format!("{stem}.");
        for entry in
            fs::read_dir(work_root).map_err(|error| format!("{}: {error}", work_root.display()))?
        {
            let entry = entry.map_err(|error| format!("{}: {error}", work_root.display()))?;
            if !entry
                .file_type()
                .map_err(|error| error.to_string())?
                .is_file()
            {
                continue;
            }
            let name = entry.file_name().to_string_lossy().into_owned();
            if name.starts_with(&prefix) {
                fs::remove_file(entry.path())
                    .map_err(|error| format!("{}: {error}", entry.path().display()))?;
                removed += 1;
            }
        }
    }
    let hand = work_root.join("hand").join(stem);
    if hand.exists() {
        fs::remove_dir_all(&hand).map_err(|error| format!("{}: {error}", hand.display()))?;
        removed += 1;
    }
    let dossier = work_root.join("walls").join(format!("{stem}.md"));
    let closed = close_dossier(
        &dossier,
        &format!("State: CLOSED — {date}. Installed by `tools/integrate-matches`."),
    )?;
    Ok(Cleanup { removed, closed })
}

fn today_utc() -> String {
    let days = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |duration| duration.as_secs() / 86_400) as i64;
    let z = days + 719_468;
    let era = (if z >= 0 { z } else { z - 146_096 }) / 146_097;
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let y = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let month = (5 * doy + 2) / 153;
    let day = doy - (153 * month + 2) / 5 + 1;
    let month = if month < 10 { month + 3 } else { month - 9 };
    let year = if month <= 2 { y + 1 } else { y };
    format!("{year:04}-{month:02}-{day:02}")
}

fn run_pipeline(directory: &str, apply: bool) -> Result<Vec<String>, String> {
    let repository = root_directory();
    let directory = Path::new(directory);
    let gate = directory.join("gate");
    fs::create_dir_all(&gate).map_err(|error| format!("{}: {error}", gate.display()))?;
    let mut candidates = Vec::new();
    for entry in
        fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?
    {
        let entry = entry.map_err(|error| error.to_string())?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if pattern("candidate").is_match(&name) {
            candidates.push(name);
        }
    }
    candidates.sort();
    let mut accepted = Vec::new();
    let mut rejected = Vec::new();
    for name in candidates {
        let stem = name
            .strip_prefix("src_")
            .and_then(|name| name.strip_suffix(".c"))
            .unwrap();
        if repository
            .join("games/gs1/src")
            .join(format!("{stem}.c"))
            .exists()
        {
            continue;
        }
        let candidate = directory.join(&name);
        let source_bytes = fs::read(&candidate).map_err(|error| error.to_string())?;
        let source = String::from_utf8_lossy(&source_bytes);
        if pattern("helper").is_match(&source) {
            rejected.push((stem.to_string(), "carries an m2c helper".to_string()));
            continue;
        }
        let asm = repository.join("games/gs1/asm").join(format!("{stem}.s"));
        if !asm.exists() || !valid_address(stem) {
            continue;
        }
        let result = (|| {
            let wanted = linked_bytes(stem, &asm, &gate, Kind::Asm, &repository)?;
            let got = linked_bytes(stem, &candidate, &gate, Kind::C, &repository)?;
            if let Some(offset) = first_difference(&wanted, &got) {
                rejected.push((
                    stem.to_string(),
                    format!(
                        "bytes differ at +0x{offset:x} (asm={}B c={}B)",
                        wanted.len(),
                        got.len()
                    ),
                ));
            } else {
                accepted.push((stem.to_string(), wanted.len()));
            }
            Ok::<(), String>(())
        })();
        if let Err(error) = result {
            rejected.push((stem.to_string(), error));
        }
    }
    let mut lines = accepted
        .iter()
        .map(|(stem, size)| format!("accept {stem} ({size}B)"))
        .collect::<Vec<_>>();
    if apply {
        for (stem, _) in &accepted {
            let candidate = directory.join(format!("src_{stem}.c"));
            let exact = repository.join("games/gs1/src").join(format!("{stem}.c"));
            fs::copy(&candidate, &exact)
                .map_err(|error| format!("{}: {error}", exact.display()))?;
            let asm = repository.join("games/gs1/asm").join(format!("{stem}.s"));
            if asm.exists() {
                fs::remove_file(&asm).map_err(|error| format!("{}: {error}", asm.display()))?;
            }
            let date = today_utc();
            let scratch = cleanup(stem, &repository.join("scratch"), &date)?;
            let draft = cleanup(stem, &repository.join("draft"), &date)?;
            if scratch.removed + draft.removed > 0 || scratch.closed || draft.closed {
                lines.push(format!(
                    "clean {stem} scratch={} wall={}",
                    scratch.removed + draft.removed,
                    if scratch.closed || draft.closed {
                        "closed"
                    } else {
                        "absent"
                    }
                ));
            }
        }
    }
    lines.extend(
        rejected
            .iter()
            .map(|(stem, reason)| format!("reject {stem}: {reason}")),
    );
    lines.push(format!(
        "accepted={} rejected={}{}",
        accepted.len(),
        rejected.len(),
        if apply { " (applied)" } else { " (dry run)" }
    ));
    Ok(lines)
}

fn parse_arguments(arguments: &[String]) -> Result<Option<(String, bool)>, String> {
    let mut directory = None;
    let mut apply = false;
    for argument in arguments {
        match argument.as_str() {
            "-h" | "--help" => return Ok(None),
            "--apply" => apply = true,
            value if value.starts_with('-') => {
                return Err(format!("unrecognized argument: {value}"));
            }
            value => {
                if directory.replace(value.to_string()).is_some() {
                    return Err(format!("unrecognized argument: {value}"));
                }
            }
        }
    }
    match directory {
        Some(directory) => Ok(Some((directory, apply))),
        None => Err("the following arguments are required: directory".to_string()),
    }
}

pub(crate) fn entry(arguments: &[String]) -> std::process::ExitCode {
    match parse_arguments(arguments) {
        Ok(None) => {
            println!("{USAGE}");
            std::process::ExitCode::SUCCESS
        }
        Ok(Some((directory, apply))) => match run_pipeline(&directory, apply) {
            Ok(lines) => {
                for line in lines {
                    println!("{line}");
                }
                std::process::ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("{error}");
                std::process::ExitCode::FAILURE
            }
        },
        Err(error) => {
            eprintln!("{error}");
            std::process::ExitCode::FAILURE
        }
    }
}
