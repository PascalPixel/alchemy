mod ninja;
mod normalized;
mod structural;
mod symbolize;

use candidate_compiler::verify::{
    compile_to_assembly, CandidateCompilerConfiguration, CandidateCompilerFamily,
};
use compiler_core::{
    plan::direct_preprocessor_command,
    routing::{root, uses_agbcc_compiler, CompilerTarget},
};
use ninja::NinjaPlan;
use serde::{Deserialize, Serialize};
use std::{
    env,
    ffi::OsStr,
    path::{Path, PathBuf},
    process::Command,
};
use structural::StructuralReport;
use walkdir::WalkDir;

const USAGE: &str = "usage: compiler workbench <recon/gs1/en/main/ADDRESS.c> [--m2c PATH] [--output DIR] [--no-run]";

#[derive(Debug)]
struct Options {
    source: PathBuf,
    m2c: Option<PathBuf>,
    output: Option<PathBuf>,
    run: bool,
}

#[derive(Debug, Deserialize, Serialize)]
struct ProbeReport {
    schema_version: u32,
    compile_ok: bool,
    error: Option<String>,
    structural: Option<StructuralReport>,
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse_options(arguments)?;
    let repository = root();
    let source = absolute_existing(repository, &options.source)?;
    let stem = owner_stem(&source)?;
    let symbol = format!("Func_{stem}");
    let reference_asm = repository.join("asm").join(format!("{stem}.s"));
    if !reference_asm.is_file() {
        return Err(format!(
            "{}: missing main-image reference assembly",
            reference_asm.display()
        ));
    }
    let m2c = locate_m2c(repository, options.m2c.as_deref())?;
    let m2c_macros = m2c.parent().unwrap_or(Path::new(".")).join("m2c_macros.h");
    if !m2c_macros.is_file() {
        return Err(format!("{}: missing m2c macros", m2c_macros.display()));
    }
    let output = prepare_output(repository, options.output.as_deref(), &stem)?;
    let executable = env::current_exe().map_err(|error| format!("current executable: {error}"))?;
    let plan = NinjaPlan {
        executable,
        source,
        reference_asm: reference_asm
            .canonicalize()
            .map_err(|error| format!("{}: {error}", reference_asm.display()))?,
        output: output.clone(),
        stem,
        symbol,
        m2c,
        m2c_macros,
        headers: headers(repository)?,
    };
    let ninja = plan.render()?;
    write_if_changed(&output.join("build.ninja"), ninja.as_bytes())?;
    println!("workbench={}", output.display());
    if !options.run {
        println!("generated={}", output.join("build.ninja").display());
        return Ok(());
    }
    let status = Command::new("ninja")
        .args([OsStr::new("-C"), output.as_os_str()])
        .status()
        .map_err(|error| format!("ninja: {error}"))?;
    if !status.success() {
        return Err(format!("ninja failed with {status}"));
    }
    print_reports(&plan)
}

pub fn run_step(arguments: &[String]) -> Result<(), String> {
    let command = arguments
        .first()
        .map(String::as_str)
        .ok_or("missing workbench step")?;
    let rest = &arguments[1..];
    match (command, rest) {
        ("compile", [source, routing, work]) => {
            compile_object(Path::new(source), Path::new(routing), Path::new(work)).map(|_| ())
        }
        ("target", [source, object, listing]) => {
            assemble_target(Path::new(source), Path::new(object), Path::new(listing))
        }
        ("symbolize", [source, listing, output]) => {
            let (text, stats) = symbolize::symbolize(Path::new(source), Path::new(listing), true)?;
            write(Path::new(output), text.as_bytes())?;
            println!(
                "symbolized={} tables={} entries={} references={}",
                output, stats.jump_tables, stats.table_entries, stats.references
            );
            Ok(())
        }
        ("m2c", [m2c, assembly, context, symbol, output]) => run_m2c(
            Path::new(m2c),
            Path::new(assembly),
            Path::new(context),
            symbol,
            Path::new(output),
        ),
        ("structural", [target, candidate, symbol, output]) => {
            let report = structural::compare(Path::new(target), Path::new(candidate), symbol)?;
            write_json(Path::new(output), &report)
        }
        ("normalized", [target, candidate, symbol, output]) => {
            let report = normalized::compare(Path::new(target), Path::new(candidate), symbol)?;
            write_json(Path::new(output), &report)
        }
        ("probe", [source, routing, work, target, symbol, output]) => probe_m2c(
            Path::new(source),
            Path::new(routing),
            Path::new(work),
            Path::new(target),
            symbol,
            Path::new(output),
        ),
        _ => Err(format!("unknown or malformed workbench step: {command}")),
    }
}

fn parse_options(arguments: &[String]) -> Result<Options, String> {
    let mut source = None;
    let mut m2c = None;
    let mut output = None;
    let mut run = true;
    let mut index = 0;
    while index < arguments.len() {
        match arguments[index].as_str() {
            "--m2c" => {
                index += 1;
                m2c = arguments.get(index).map(PathBuf::from);
                if m2c.is_none() {
                    return Err("--m2c requires a path".into());
                }
            }
            "--output" => {
                index += 1;
                output = arguments.get(index).map(PathBuf::from);
                if output.is_none() {
                    return Err("--output requires a directory".into());
                }
            }
            "--no-run" => run = false,
            option if option.starts_with('-') => return Err(format!("unknown option: {option}")),
            value if source.is_none() => source = Some(PathBuf::from(value)),
            value => return Err(format!("unexpected argument: {value}")),
        }
        index += 1;
    }
    Ok(Options {
        source: source.ok_or(USAGE)?,
        m2c,
        output,
        run,
    })
}

fn owner_stem(path: &Path) -> Result<String, String> {
    let stem = path
        .file_stem()
        .and_then(OsStr::to_str)
        .ok_or("non-UTF-8 owner name")?;
    if stem.len() != 8 || !stem.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!(
            "{}: expected an eight-digit main-image owner",
            path.display()
        ));
    }
    Ok(stem.to_ascii_lowercase())
}

fn absolute_existing(repository: &Path, path: &Path) -> Result<PathBuf, String> {
    let path = if path.is_absolute() {
        path.to_path_buf()
    } else {
        repository.join(path)
    };
    path.canonicalize()
        .map_err(|error| format!("{}: {error}", path.display()))
}

fn locate_m2c(repository: &Path, requested: Option<&Path>) -> Result<PathBuf, String> {
    let mut candidates = Vec::new();
    if let Some(path) = requested {
        candidates.push(if path.is_absolute() {
            path.to_path_buf()
        } else {
            repository.join(path)
        });
    } else {
        if let Some(path) = env::var_os("M2C") {
            candidates.push(PathBuf::from(path));
        }
        candidates.push(repository.join("m2c/m2c.py"));
        if let Some(paths) = env::var_os("PATH") {
            candidates.extend(env::split_paths(&paths).map(|path| path.join("m2c.py")));
        }
    }
    candidates
        .into_iter()
        .find(|path| path.is_file())
        .and_then(|path| path.canonicalize().ok())
        .ok_or_else(|| {
            "m2c.py not found; clone upstream m2c into ignored m2c/ or pass --m2c PATH".into()
        })
}

fn prepare_output(
    repository: &Path,
    requested: Option<&Path>,
    stem: &str,
) -> Result<PathBuf, String> {
    let repository_out = repository.join("out");
    std::fs::create_dir_all(&repository_out)
        .map_err(|error| format!("{}: {error}", repository_out.display()))?;
    let path = match requested {
        Some(path) if path.is_absolute() => path.to_path_buf(),
        Some(path) => repository.join(path),
        None => repository_out.join("workbench").join(stem),
    };
    std::fs::create_dir_all(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    let resolved = path
        .canonicalize()
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let trusted_out = repository_out
        .canonicalize()
        .map_err(|error| format!("{}: {error}", repository_out.display()))?;
    let temp = env::temp_dir()
        .canonicalize()
        .unwrap_or_else(|_| env::temp_dir());
    if resolved == trusted_out || resolved == temp {
        return Err(format!(
            "refusing shared output root {}",
            resolved.display()
        ));
    }
    if !resolved.starts_with(&trusted_out) && !resolved.starts_with(&temp) {
        return Err(format!(
            "workbench output must be under out/ or {}",
            temp.display()
        ));
    }
    Ok(resolved)
}

fn headers(repository: &Path) -> Result<Vec<PathBuf>, String> {
    let include = repository.join("include");
    let mut paths = WalkDir::new(&include)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|entry| entry.file_type().is_file())
        .filter(|entry| entry.path().extension() == Some(OsStr::new("h")))
        .map(|entry| entry.into_path())
        .collect::<Vec<_>>();
    paths.sort();
    Ok(paths)
}

fn compile_object(source: &Path, routing: &Path, work: &Path) -> Result<PathBuf, String> {
    std::fs::create_dir_all(work).map_err(|error| format!("{}: {error}", work.display()))?;
    let source = text_path(source)?;
    let routing = text_path(routing)?;
    let work_text = text_path(work)?;
    let config = CandidateCompilerConfiguration {
        family: Some(CandidateCompilerFamily::Routed),
        ..Default::default()
    };
    let assembly = compile_to_assembly(
        &source,
        &routing,
        &work_text,
        &[],
        CompilerTarget::Gs1,
        &config,
    )?;
    let context = work.join(format!("{}.i", owner_stem(Path::new(&source))?));
    // gcc296's driver compiles the original source directly, whereas old-agbcc
    // already consumes and retains a preprocessed file. Produce the same
    // approved gcc296 preprocessor stream explicitly for m2c's type context.
    if !uses_agbcc_compiler(CompilerTarget::Gs1, &routing) {
        let context = text_path(&context)?;
        let command = direct_preprocessor_command(&source, &context)?;
        candidate_compiler::verify::run(&command, root())?;
    }
    if !context.is_file() {
        return Err(format!(
            "{}: compiler did not produce m2c context",
            context.display()
        ));
    }
    let object = work.join(format!("{}.o", owner_stem(Path::new(&source))?));
    assemble(Path::new(&assembly), &object, None)?;
    Ok(object)
}

fn assemble_target(source: &Path, object: &Path, listing: &Path) -> Result<(), String> {
    assemble(source, object, Some(listing))
}

fn assemble(source: &Path, object: &Path, listing: Option<&Path>) -> Result<(), String> {
    if let Some(parent) = object.parent() {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    if let Some(listing) = listing {
        if let Some(parent) = listing.parent() {
            std::fs::create_dir_all(parent)
                .map_err(|error| format!("{}: {error}", parent.display()))?;
        }
    }
    let mut command = Command::new("arm-none-eabi-as");
    command.args(["-mcpu=arm7tdmi", "-mthumb-interwork"]);
    if let Some(listing) = listing {
        command.arg(format!("-alhnd={}", listing.display()));
    }
    let output = command
        .arg("-o")
        .arg(object)
        .arg(source)
        .current_dir(root())
        .output()
        .map_err(|error| format!("arm-none-eabi-as: {error}"))?;
    if !output.status.success() {
        return Err(process_error("arm-none-eabi-as", &output));
    }
    Ok(())
}

fn run_m2c(
    m2c: &Path,
    assembly: &Path,
    context: &Path,
    symbol: &str,
    output: &Path,
) -> Result<(), String> {
    let attempt = |with_context: bool| {
        let mut command = Command::new("python3");
        command
            .arg(m2c)
            .args([
                "-t",
                "gba-gcc-c",
                "-f",
                symbol,
                "--valid-syntax",
                "--deterministic-vars",
            ])
            .args(["--globals", "none", "--no-cache"]);
        if with_context {
            command.arg("--context").arg(context);
        }
        command.arg(assembly).current_dir(root()).output()
    };
    let first = attempt(true).map_err(|error| format!("python3: {error}"))?;
    let result = if first.status.success() {
        first
    } else {
        eprintln!(
            "m2c context rejected; retrying without context: {}",
            process_error("m2c", &first)
        );
        attempt(false).map_err(|error| format!("python3: {error}"))?
    };
    if !result.status.success() {
        return Err(process_error("m2c", &result));
    }
    let macros = std::fs::read(m2c.parent().unwrap_or(Path::new(".")).join("m2c_macros.h"))
        .map_err(|error| format!("m2c_macros.h: {error}"))?;
    let mut source = b"#include \"types.h\"\n\n".to_vec();
    source.extend(macros);
    source.push(b'\n');
    source.extend(&result.stdout);
    write(output, &source)?;
    if !result.stderr.is_empty() {
        eprintln!("{}", String::from_utf8_lossy(&result.stderr).trim());
    }
    Ok(())
}

fn probe_m2c(
    source: &Path,
    routing: &Path,
    work: &Path,
    target: &Path,
    symbol: &str,
    output: &Path,
) -> Result<(), String> {
    let report = match compile_object(source, routing, work) {
        Ok(candidate) => match structural::compare(target, &candidate, symbol) {
            Ok(structural) => ProbeReport {
                schema_version: 1,
                compile_ok: true,
                error: None,
                structural: Some(structural),
            },
            Err(error) => ProbeReport {
                schema_version: 1,
                compile_ok: true,
                error: Some(error),
                structural: None,
            },
        },
        Err(error) => ProbeReport {
            schema_version: 1,
            compile_ok: false,
            error: Some(error),
            structural: None,
        },
    };
    write_json(output, &report)
}

fn print_reports(plan: &NinjaPlan) -> Result<(), String> {
    let structural: StructuralReport = read_json(&plan.structural_report())?;
    println!(
        "objdiff={:.5}% instructions={} arg={} op={} replace={} delete={} insert={}",
        structural.match_percent,
        structural.instructions,
        structural.argument_mismatches,
        structural.opcode_mismatches,
        structural.replacements,
        structural.deletions,
        structural.insertions,
    );
    let probe: ProbeReport = read_json(&plan.m2c_probe_report())?;
    match probe.structural {
        Some(report) => println!("m2c_compile=ok m2c_objdiff={:.5}%", report.match_percent),
        None if probe.compile_ok => println!("m2c_compile=ok m2c_objdiff=unavailable"),
        None => println!("m2c_compile=needs-types"),
    }
    if let Some(error) = probe.error {
        let first = error.lines().next().unwrap_or(&error);
        println!("m2c_note={first}");
    }
    Ok(())
}

fn text_path(path: &Path) -> Result<String, String> {
    path.to_str()
        .map(str::to_string)
        .ok_or_else(|| format!("{}: non-UTF-8 path", path.display()))
}

fn process_error(name: &str, output: &std::process::Output) -> String {
    let stderr = String::from_utf8_lossy(&output.stderr);
    let stdout = String::from_utf8_lossy(&output.stdout);
    let detail = if stderr.trim().is_empty() {
        stdout.trim()
    } else {
        stderr.trim()
    };
    if detail.is_empty() {
        format!("{name} failed")
    } else {
        format!("{name} failed: {detail}")
    }
}

fn write(path: &Path, bytes: &[u8]) -> Result<(), String> {
    if let Some(parent) = path.parent() {
        std::fs::create_dir_all(parent)
            .map_err(|error| format!("{}: {error}", parent.display()))?;
    }
    std::fs::write(path, bytes).map_err(|error| format!("{}: {error}", path.display()))
}

fn write_if_changed(path: &Path, bytes: &[u8]) -> Result<(), String> {
    if std::fs::read(path).ok().as_deref() == Some(bytes) {
        return Ok(());
    }
    write(path, bytes)
}

fn write_json<T: Serialize>(path: &Path, value: &T) -> Result<(), String> {
    let mut bytes = serde_json::to_vec_pretty(value).map_err(|error| error.to_string())?;
    bytes.push(b'\n');
    write(path, &bytes)
}

fn read_json<T: for<'de> Deserialize<'de>>(path: &Path) -> Result<T, String> {
    let bytes = std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_slice(&bytes).map_err(|error| format!("{}: {error}", path.display()))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parses_workbench_options() {
        let options = parse_options(&[
            "recon/gs1/en/main/080ab5e4.c".into(),
            "--m2c".into(),
            "m2c/m2c.py".into(),
            "--no-run".into(),
        ])
        .unwrap();
        assert_eq!(
            options.source,
            PathBuf::from("recon/gs1/en/main/080ab5e4.c")
        );
        assert_eq!(options.m2c, Some(PathBuf::from("m2c/m2c.py")));
        assert!(!options.run);
    }

    #[test]
    fn validates_owner_stems() {
        assert_eq!(
            owner_stem(Path::new("recon/gs1/en/main/080AB5E4.c")).unwrap(),
            "080ab5e4"
        );
        assert!(owner_stem(Path::new("recon/gs1/en/main/best.c")).is_err());
    }
}
