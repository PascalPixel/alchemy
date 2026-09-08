use super::owners;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy <extract|inspect|disassemble|adopt> OWNER [options]\nUse psynergy decompile for portable C recovery and alchemy diff to score.";

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

fn extract(root: &Path, options: &Options) -> Result<(), String> {
    let owner = owner_argument(options)?;
    let path = options
        .out
        .as_ref()
        .ok_or("extract requires --out FILE under ignored out/")?;
    let parent = path
        .parent()
        .filter(|path| !path.as_os_str().is_empty())
        .unwrap_or(Path::new("."));
    let parent = parent
        .canonicalize()
        .map_err(|error| format!("{}: {error}", parent.display()))?;
    let output_root = root
        .join("out")
        .canonicalize()
        .map_err(|error| error.to_string())?;
    if !parent.starts_with(output_root) {
        return Err("extracted reference bytes must stay under ignored out/".into());
    }
    let (image, base, entry, span) = owners::image_window(root, owner, options.span)?;
    let start = (entry - base) as usize;
    std::fs::OpenOptions::new()
        .write(true)
        .create_new(true)
        .open(path)
        .and_then(|mut file| file.write_all(&image[start..start + span as usize]))
        .map_err(|error| format!("{}: {error}", path.display()))?;
    println!("extracted {span} bytes; base=0x{entry:08x} entry=0x{entry:08x}");
    Ok(())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let Some(command) = arguments.first().map(String::as_str) else {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    };
    let root = owners::root();
    let result = parse(&arguments[1..]).and_then(|options| match command {
        "extract" => extract(&root, &options).map(|_| 0),
        "adopt" => adopt_owner(&root, &options).map(|_| 0),
        "inspect" => imports_owner(&root, &options),
        "disassemble" => disasm(&root, &options).map(|_| 0),
        "-h" | "--help" => {
            println!("{USAGE}");
            Ok(0)
        }
        other => Err(format!("unknown recovery operation: {other}\n{USAGE}")),
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
    let rows = crate::diff::disasm::disassemble(&binary.to_string_lossy(), entry)?;
    let calls = super::imports::imports(root, owner, Some(span))?;
    for (address, instruction) in rows {
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
        println!("{address:08x}: {instruction}{annotation}");
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

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn extraction_is_bounded_local_and_non_overwriting() {
        let root = tempfile::tempdir().unwrap();
        std::fs::create_dir(root.path().join("roms")).unwrap();
        std::fs::create_dir(root.path().join("out")).unwrap();
        let image = [1, 2, 3, 4, 5, 6];
        std::fs::write(root.path().join("roms/gs1-en.gba"), image).unwrap();
        let output = root.path().join("out/owner.bin");
        let mut options = parse(&[
            "main:08000002".into(),
            "--span".into(),
            "4".into(),
            "--out".into(),
            output.to_string_lossy().into_owned(),
        ])
        .unwrap();
        extract(root.path(), &options).unwrap();
        assert_eq!(std::fs::read(&output).unwrap(), image[2..]);
        assert!(extract(root.path(), &options).is_err());
        assert_eq!(std::fs::read(&output).unwrap(), image[2..]);
        options.out = Some(root.path().join("tracked.bin"));
        assert!(extract(root.path(), &options)
            .unwrap_err()
            .contains("ignored out/"));
        assert!(!root.path().join("tracked.bin").exists());
        options.out = Some(root.path().join("out/invalid.bin"));
        options.span = Some(8);
        assert!(extract(root.path(), &options).is_err());
        assert!(!root.path().join("out/invalid.bin").exists());
    }
}
