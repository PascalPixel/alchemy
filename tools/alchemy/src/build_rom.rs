//! Link the whole ROM from the objects its linker script lists, in order, as
//! pret links `ld_script.ld`. Every symbol resolves from its definition; the
//! image is written for `sha1sum -c rom.sha1` and nothing is copied from a
//! reference ROM except what the script's scaffolding reads explicitly.
use crate::build_assets::LzMachine;
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::{
    assembly_command, compiler_assembly_command, prefer_installed_binutils,
};
use crate::targets::{decomp_target, DecompTarget};
use psynergy::process::run as command;
use sha2::{Digest, Sha256};
use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy build rom [--target tbs-en|tla-en] [--script FILE] [--output DIR] [--keep-going]\n\
Compile and assemble every object the linker script lists, link them in its\n\
order and write the ROM image beside the linker map. --keep-going writes the\n\
image despite link errors, for locating differences; it never passes a compare.";

pub fn run(args: &[String]) -> Result<(), String> {
    let mut target = decomp_target(None)?;
    let mut script = None;
    let mut output = None;
    let mut keep_going = false;
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "-h" | "--help" => {
                println!("{USAGE}");
                return Ok(());
            }
            "--target" => target = decomp_target(Some(args.next().ok_or(USAGE)?))?,
            "--script" => script = Some(PathBuf::from(args.next().ok_or(USAGE)?)),
            "--output" => output = Some(PathBuf::from(args.next().ok_or(USAGE)?)),
            "--keep-going" => keep_going = true,
            _ => return Err(USAGE.into()),
        }
    }
    let root = crate::compiler::routing::root();
    let script = script.unwrap_or_else(|| Path::new(target.game_dir()).join("MAIN.LD"));
    let output = output.unwrap_or_else(|| PathBuf::from(target.output_dir));
    let linked = link(
        root,
        target,
        &script,
        &output,
        target.id.as_str(),
        keep_going,
    )?;
    println!("objects={}", linked.objects.len());
    println!("map={}", linked.map.display());
    println!("rom={}", linked.image.display());
    Ok(())
}

pub(crate) struct Linked {
    pub objects: Vec<PathBuf>,
    pub elf: PathBuf,
    pub map: PathBuf,
    pub image: PathBuf,
}

/// Every object path the script names, in the order it first names them.
pub(crate) fn script_objects(script: &str) -> Vec<String> {
    let pattern = regex::Regex::new(r#""\*/([^"*:]+)\.o""#).expect("static pattern");
    let mut seen = BTreeSet::new();
    pattern
        .captures_iter(script)
        .map(|capture| capture[1].to_owned())
        .filter(|path| seen.insert(path.clone()))
        .collect()
}

/// The maintained source an object path names: C, then assembly.
fn source_for(root: &Path, object: &str) -> Result<PathBuf, String> {
    ["C", "c", "S", "s"]
        .iter()
        .map(|extension| PathBuf::from(format!("{object}.{extension}")))
        .find(|path| root.join(path).is_file())
        .ok_or_else(|| format!("{object}.o has no C or assembly source"))
}

pub(crate) fn link(
    root: &Path,
    target: DecompTarget,
    script: &Path,
    output: &Path,
    name: &str,
    keep_going: bool,
) -> Result<Linked, String> {
    prefer_installed_binutils();
    let text = fs::read_to_string(root.join(script))
        .map_err(|error| format!("{}: {error}", script.display()))?;
    let sources = script_objects(&text)
        .iter()
        .map(|object| source_for(root, object))
        .collect::<Result<Vec<_>, _>>()?;
    if sources.is_empty() {
        return Err(format!("{} lists no objects", script.display()));
    }
    let output = root.join(output);
    base_rom(root, target, &output)?;
    let objects = compile_all(root, target, &sources, &output)?;
    let elf = output.join(format!("{name}.elf"));
    let map = output.join(format!("{name}.map"));
    let image = output.join(format!("{name}.gba"));
    let mut arguments = vec![
        "arm-none-eabi-ld".to_owned(),
        "--no-warn-mismatch".into(),
        "-T".into(),
        root.join(script).to_string_lossy().into_owned(),
        "-Map".into(),
        map.to_string_lossy().into_owned(),
        "-o".into(),
        elf.to_string_lossy().into_owned(),
    ];
    if keep_going {
        arguments.push("--noinhibit-exec".into());
    }
    arguments.extend(
        objects
            .iter()
            .map(|path| path.to_string_lossy().into_owned()),
    );
    if text.contains("libgcc.a:") {
        arguments.push(
            root.join("tools/out/compiler-runtime/libgcc.a")
                .to_string_lossy()
                .into_owned(),
        );
    }
    if let Err(error) = command(&arguments, root) {
        if !keep_going || !elf.is_file() {
            return Err(error);
        }
        eprintln!("{error}");
    }
    command(
        &[
            "arm-none-eabi-objcopy",
            "-O",
            "binary",
            &elf.to_string_lossy(),
            &image.to_string_lossy(),
        ],
        root,
    )?;
    Ok(Linked {
        objects,
        elf,
        map,
        image,
    })
}

/// Scaffolding reads not-yet-sourced data from the builder's own verified ROM as
/// `baserom.gba`, as early pret builds did; the ROM itself is never tracked.
fn base_rom(root: &Path, target: DecompTarget, output: &Path) -> Result<(), String> {
    let rom = root.join(target.rom);
    let bytes = fs::read(&rom).map_err(|error| format!("{}: {error}", rom.display()))?;
    if bytes.len() as u64 != target.rom_size {
        return Err(format!("{} has the wrong size", rom.display()));
    }
    crate::text_catalog::verify_reference(root, target.id.as_str(), &bytes)?;
    fs::create_dir_all(output).map_err(|error| error.to_string())?;
    let link = output.join("baserom.gba");
    let _ = fs::remove_file(&link);
    std::os::unix::fs::symlink(&rom, &link).map_err(|error| error.to_string())
}

/// Compile or assemble every source into `output/obj`, reusing an object only
/// when its preprocessed input and command are unchanged.
fn compile_all(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
    output: &Path,
) -> Result<Vec<PathBuf>, String> {
    let objects: Vec<PathBuf> = sources
        .iter()
        .map(|source| output.join("obj").join(source).with_extension("o"))
        .collect();
    let next = AtomicUsize::new(0);
    let errors = Mutex::new(Vec::new());
    let workers = std::thread::available_parallelism().map_or(4, |count| count.get());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                let Some(source) = sources.get(index) else {
                    break;
                };
                if let Err(error) = compile(root, target, source, &objects[index]) {
                    errors
                        .lock()
                        .unwrap()
                        .push(format!("{}: {error}", source.display()));
                }
            });
        }
    });
    let errors = errors.into_inner().unwrap();
    if !errors.is_empty() {
        return Err(errors.join("\n"));
    }
    Ok(objects)
}

fn compile(root: &Path, target: DecompTarget, source: &Path, object: &Path) -> Result<(), String> {
    fs::create_dir_all(object.parent().expect("object directory"))
        .map_err(|error| error.to_string())?;
    let stamp = object.with_extension("o.key");
    let source_text = source.to_string_lossy().into_owned();
    let object_text = object.to_string_lossy().into_owned();
    let c = matches!(
        source.extension().and_then(|extension| extension.to_str()),
        Some("C" | "c")
    );
    let (key, steps) = if c {
        let assembly = object.with_extension("s");
        let preprocessed = object.with_extension("i");
        let mut options = SourceToAssemblyPlanOptions::new(
            target.compiler,
            source_text.clone(),
            source_text.clone(),
            assembly.to_string_lossy().into_owned(),
        );
        options.preprocessor_flags = vec![format!("-D{}=1", target.edition_define)];
        options.preprocessed_output = Some(preprocessed.to_string_lossy().into_owned());
        let mut steps = source_to_assembly_plan(&options)?;
        steps.push(compiler_assembly_command(
            &assembly.to_string_lossy(),
            &object_text,
        ));
        // The routed compile reads its own preprocessed text, so the key is
        // that text plus every command; headers are covered by preprocessing.
        let preprocess = preprocessor_only(&steps, &source_text, &preprocessed)?;
        command(&preprocess, root)?;
        let mut hasher = Sha256::new();
        hasher.update(fs::read(&preprocessed).map_err(|error| error.to_string())?);
        for step in &steps {
            hasher.update(step.join("\0").as_bytes());
        }
        (format!("{:x}", hasher.finalize()), steps)
    } else {
        let mut hasher = Sha256::new();
        hasher.update(with_includes(root, &root.join(source))?);
        let mut step = assembly_command(&source_text, &object_text);
        let base = object
            .ancestors()
            .find(|path| path.join("baserom.gba").exists())
            .ok_or("the base ROM link is missing")?;
        for stream in overlay_streams(root, target, source, base)? {
            hasher.update(fs::read(&stream).map_err(|error| error.to_string())?);
        }
        step.insert(1, format!("-I{}", base.display()));
        hasher.update(step.join("\0").as_bytes());
        (format!("{:x}", hasher.finalize()), vec![step])
    };
    if object.is_file() && fs::read_to_string(&stamp).ok().as_deref() == Some(key.as_str()) {
        return Ok(());
    }
    let _ = fs::remove_file(&stamp);
    for step in &steps {
        command(step, root)?;
    }
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

/// The compressor Camelot's resource packer ran on every code overlay, as
/// the streams in both games show: the general ring's window, read-ahead and
/// reach, and the palette ring's read-ahead. Each overlay takes the smaller
/// of the two encodings, the palette one on ties.
const OVERLAY_MACHINE: LzMachine = LzMachine::new(4123, 485, 4126, 272);

/// The code overlays an assembly source reads with
/// `.incbin "overlays/resource_XXX.lz"`, each linked from its listing beside
/// the source and compressed, as pret builds the compressed files its data
/// sources read.
fn overlay_streams(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
) -> Result<Vec<PathBuf>, String> {
    let text = fs::read_to_string(root.join(source))
        .map_err(|error| format!("{}: {error}", source.display()))?;
    let pattern = regex::Regex::new(r#"(?m)^\s*\.incbin\s+"overlays/resource_([0-9a-f]+)\.lz""#)
        .expect("static pattern");
    let ids: Vec<String> = pattern
        .captures_iter(&text)
        .map(|capture| capture[1].to_owned())
        .collect();
    let listings = source
        .parent()
        .ok_or("source has no directory")?
        .join("raw/overlays");
    let directory = output.join("overlays");
    let next = AtomicUsize::new(0);
    let errors = Mutex::new(Vec::new());
    let workers = std::thread::available_parallelism().map_or(4, |count| count.get());
    std::thread::scope(|scope| {
        for _ in 0..workers {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                let Some(id) = ids.get(index) else {
                    break;
                };
                if let Err(error) = build_overlay(root, target, &listings, id, &directory) {
                    errors
                        .lock()
                        .unwrap()
                        .push(format!("resource_{id}: {error}"));
                }
            });
        }
    });
    let errors = errors.into_inner().unwrap();
    if !errors.is_empty() {
        return Err(errors.join("\n"));
    }
    Ok(ids
        .iter()
        .map(|id| directory.join(format!("resource_{id}.lz")))
        .collect())
}

/// Link one overlay listing alone at its load address, with its own script
/// when it places compiler-library members and the game's otherwise, and
/// compress the image. The map stays beside it for progress.
fn build_overlay(
    root: &Path,
    target: DecompTarget,
    listings: &Path,
    id: &str,
    directory: &Path,
) -> Result<(), String> {
    fs::create_dir_all(directory).map_err(|error| error.to_string())?;
    let listing = listings.join(format!("resource_{id}_overlay.s"));
    let own = listings.join(format!("resource_{id}.ld"));
    let script = if root.join(&own).is_file() {
        own
    } else {
        Path::new(target.game_dir()).join("OVERLAY.LD")
    };
    let path = |name: String| directory.join(name).to_string_lossy().into_owned();
    let object = path(format!("resource_{id}_overlay.o"));
    let elf = path(format!("resource_{id}.elf"));
    let map = path(format!("resource_{id}.map"));
    let image = path(format!("resource_{id}.bin"));
    let stream = directory.join(format!("resource_{id}.lz"));
    let stamp = stream.with_extension("lz.key");
    let steps = [
        assembly_command(&listing.to_string_lossy(), &object),
        [
            "arm-none-eabi-ld",
            "--no-warn-mismatch",
            "-T",
            &script.to_string_lossy(),
            "-Map",
            &map,
            "-o",
            &elf,
            &object,
            "tools/out/compiler-runtime/libgcc.a",
        ]
        .map(String::from)
        .to_vec(),
        ["arm-none-eabi-objcopy", "-O", "binary", &elf, &image]
            .map(String::from)
            .to_vec(),
    ];
    let mut hasher = Sha256::new();
    hasher.update(with_includes(root, &root.join(&listing))?);
    hasher.update(fs::read(root.join(&script)).map_err(|error| error.to_string())?);
    for step in &steps {
        hasher.update(step.join("\0").as_bytes());
    }
    hasher.update(format!("{OVERLAY_MACHINE:?}").as_bytes());
    let key = format!("{:x}", hasher.finalize());
    if stream.is_file() && fs::read_to_string(&stamp).ok().as_deref() == Some(key.as_str()) {
        return Ok(());
    }
    let _ = fs::remove_file(&stamp);
    for step in &steps {
        command(step, root)?;
    }
    let decoded = fs::read(&image).map_err(|error| error.to_string())?;
    let encoded = crate::build_assets::encode_overlay_stream(&decoded, &OVERLAY_MACHINE)?;
    fs::write(&stream, encoded).map_err(|error| error.to_string())?;
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

/// The plan's preprocessing step when it has one, or a `-E` run of the same
/// driver command, so the cache key covers every included header.
fn preprocessor_only(
    steps: &[Vec<String>],
    source: &str,
    preprocessed: &Path,
) -> Result<Vec<String>, String> {
    let first = steps.first().ok_or("empty compile plan")?;
    if first
        .first()
        .is_some_and(|program| program.ends_with("cpp0"))
    {
        return Ok(first.clone());
    }
    let mut command = Vec::with_capacity(first.len() + 2);
    let mut arguments = first.iter();
    while let Some(argument) = arguments.next() {
        match argument.as_str() {
            "-S" => {}
            "-o" => {
                arguments.next();
            }
            _ => command.push(argument.clone()),
        }
    }
    command.retain(|argument| argument != source);
    command.extend([
        "-E".into(),
        "-o".into(),
        preprocessed.to_string_lossy().into_owned(),
        source.into(),
    ]);
    Ok(command)
}

/// Assembly text followed by every file it `.include`s, recursively.
fn with_includes(root: &Path, source: &Path) -> Result<Vec<u8>, String> {
    let text = fs::read(source).map_err(|error| format!("{}: {error}", source.display()))?;
    let mut key = text.clone();
    for line in String::from_utf8_lossy(&text).lines() {
        let Some(rest) = line.trim().strip_prefix(".include") else {
            continue;
        };
        let path = root.join(rest.trim().trim_matches('"'));
        key.extend_from_slice(&with_includes(root, &path)?);
    }
    Ok(key)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn script_objects_keep_first_order_and_skip_archive_members() {
        let script = r#"
            "*/games/A/SRC/X.o"(.text)
            "*/recon/tbs/raw/08000000.o"(.text)
            "*/games/A/SRC/X.o"(.rodata)
            "*libgcc.a:_call_via_rX.o"(.text)
        "#;
        assert_eq!(
            script_objects(script),
            ["games/A/SRC/X", "recon/tbs/raw/08000000"]
        );
    }
}
