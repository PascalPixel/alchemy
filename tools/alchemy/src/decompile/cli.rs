use super::lift_owner;
use super::owners;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy decompile OWNER [--span BYTES] [--name NAME] [--out FILE]\nUse alchemy diff to score and match for catalogued repairs.";

struct Options {
    positional: Vec<String>,
    span: Option<u32>,
    name: Option<String>,
    out: Option<PathBuf>,
    path: Option<String>,
    source: Option<PathBuf>,
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options {
        positional: Vec::new(),
        span: None,
        name: None,
        out: None,
        path: None,
        source: None,
    };
    let mut iter = arguments.iter();
    while let Some(argument) = iter.next() {
        let mut value = |flag: &str| {
            iter.next()
                .cloned()
                .ok_or_else(|| format!("{flag} needs a value"))
        };
        match argument.as_str() {
            "--span" => {
                options.span = Some(
                    value("--span")?
                        .parse()
                        .map_err(|_| "--span wants a byte count")?,
                )
            }
            "--name" => options.name = Some(value("--name")?),
            "--out" => options.out = Some(PathBuf::from(value("--out")?)),
            "--path" => options.path = Some(value("--path")?),
            "--source" => options.source = Some(PathBuf::from(value("--source")?)),
            other if other.starts_with("--") => return Err(format!("unknown flag {other}")),
            other => options.positional.push(other.to_string()),
        }
    }
    Ok(options)
}

fn owner_argument(options: &Options) -> Result<&str, String> {
    options
        .positional
        .first()
        .map(String::as_str)
        .ok_or_else(|| "an <overlay>:<addressHex> owner is required".to_string())
}

fn draft(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let (unit, _) = lift_owner(root, owner, options.span, options.name.as_deref())?;
    match &options.out {
        Some(path) => {
            std::fs::write(path, unit).map_err(|error| format!("{}: {error}", path.display()))
        }
        None => {
            print!("{unit}");
            Ok(())
        }
    }
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let root = owners::root();
    let result = parse(&arguments[1..]).and_then(|options| match command {
        "decompile" => draft(&root, &options).map(|_| 0),
        "adopt" => adopt_owner(&root, &options).map(|_| 0),
        "inspect" => imports_owner(&root, &options),
        "disassemble" => disasm(&root, &options).map(|_| 0),
        "-h" | "--help" => {
            println!("{USAGE}");
            Ok(0)
        }
        other => Err(format!("unknown decompiler operation: {other}\n{USAGE}")),
    });
    match result {
        Ok(0) => ExitCode::SUCCESS,
        Ok(_) => ExitCode::FAILURE,
        Err(error) => {
            eprintln!("{error}");
            ExitCode::FAILURE
        }
    }
}

/// Native disassembly of the complete bounded owner, with resolved call names.
fn disasm(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let (image, base, entry, span) = owners::image_window(root, owner, options.span)?;
    let directory = root.join("out/disassemble");
    std::fs::create_dir_all(&directory).map_err(|e| e.to_string())?;
    let work = tempfile::tempdir_in(directory).map_err(|e| e.to_string())?;
    let binary = work.path().join("owner.bin");
    let start = (entry - base) as usize;
    std::fs::write(&binary, &image[start..start + span as usize]).map_err(|e| e.to_string())?;
    let rows = diff::disasm::disassemble(&binary.to_string_lossy(), f64::from(entry))?;
    let calls = super::imports::imports(root, owner, Some(span))?;
    for offset in rows.keys() {
        let address = entry + offset as u32;
        let annotation = calls
            .iter()
            .find(|call| call.site == address)
            .map(|call| {
                let target = call.main.unwrap_or(call.target);
                format!(
                    " ; {} (0x{target:08x})",
                    call.name.as_deref().unwrap_or("resolved call")
                )
            })
            .unwrap_or_default();
        println!("{address:08x}: {}{annotation}", rows.get(offset).unwrap());
    }
    Ok(())
}

fn adopt_owner(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let request = super::adopt::Request {
        owner,
        span: options.span,
        name: options.name.as_deref(),
        path: options.path.as_deref(),
        source: options.source.as_deref(),
    };
    for line in super::adopt::adopt(root, &request)? {
        println!("{line}");
    }
    Ok(())
}

/// Prints every call site of an owner resolved to its real target, one JSON
/// object per line, for the humanizing passes that annotate the units.
fn imports_owner(root: &Path, options: &Options) -> Result<i32, String> {
    let owner = owner_argument(options)?;
    for import in super::imports::imports(root, owner, options.span)? {
        println!(
            "{}",
            serde_json::to_string(&import).map_err(|error| error.to_string())?
        );
    }
    Ok(0)
}
