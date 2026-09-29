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
    // Sources that read built overlay streams wait for the overlays, and the
    // overlays link against the main image's symbols: a first pass links the
    // main image with empty streams, which move nothing the overlays can see.
    let (streamed, direct): (Vec<PathBuf>, Vec<PathBuf>) = sources
        .iter()
        .cloned()
        .partition(|source| !stream_paths(root, source, &output).is_empty());
    compile_all(root, target, &direct, &output)?;
    let objects: Vec<PathBuf> = sources
        .iter()
        .map(|source| output.join("obj").join(source).with_extension("o"))
        .collect();
    let mut symbols = None;
    if !streamed.is_empty() {
        let pass = symbols_pass(root, script, &text, &output, name, &sources, &streamed)?;
        for source in &streamed {
            build_overlay_streams(root, target, source, &output, &pass)?;
        }
        compile_all(root, target, &streamed, &output)?;
        symbols = Some(pass);
    }
    let elf = output.join(format!("{name}.elf"));
    let map = output.join(format!("{name}.map"));
    let image = output.join(format!("{name}.gba"));
    let mut arguments = link_command(root, script, &text, &objects, &elf);
    arguments.splice(
        1..1,
        ["-Map".to_owned(), map.to_string_lossy().into_owned()],
    );
    if keep_going {
        arguments.push("--noinhibit-exec".into());
    }
    let linked = command(&arguments, root);
    if linked.is_ok() {
        if let Some(pass) = &symbols {
            same_addresses(root, pass, &elf)?;
        }
        iwram_entries(root, target, &elf)?;
    }
    if let Err(error) = linked {
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

/// The linker command for the whole image.
fn link_command(
    root: &Path,
    script: &Path,
    text: &str,
    objects: &[PathBuf],
    elf: &Path,
) -> Vec<String> {
    let mut arguments = vec![
        "arm-none-eabi-ld".to_owned(),
        "--no-warn-mismatch".into(),
        "-T".into(),
        root.join(script).to_string_lossy().into_owned(),
        "-o".into(),
        elf.to_string_lossy().into_owned(),
    ];
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
    arguments
}

/// The main image linked with every stream-reading object empty, for the
/// overlays' links: every symbol before the streams keeps its address.
fn symbols_pass(
    root: &Path,
    script: &Path,
    text: &str,
    output: &Path,
    name: &str,
    sources: &[PathBuf],
    streamed: &[PathBuf],
) -> Result<PathBuf, String> {
    let pass = output.join("symbols");
    let mut objects = Vec::with_capacity(sources.len());
    for source in sources {
        if streamed.contains(source) {
            let stub = pass.join(source).with_extension("o");
            fs::create_dir_all(stub.parent().expect("stub directory"))
                .map_err(|error| error.to_string())?;
            command(
                &assembly_command("/dev/null", &stub.to_string_lossy()),
                root,
            )?;
            objects.push(stub);
        } else {
            objects.push(output.join("obj").join(source).with_extension("o"));
        }
    }
    let elf = pass.join(format!("{name}.elf"));
    command(&link_command(root, script, text, &objects, &elf), root)?;
    Ok(elf)
}

/// Every symbol the overlays could see keeps its address in the final image.
fn same_addresses(root: &Path, pass: &Path, elf: &Path) -> Result<(), String> {
    let table = |path: &Path| -> Result<std::collections::BTreeMap<String, String>, String> {
        let text = command(
            &[
                "arm-none-eabi-nm",
                "--defined-only",
                &path.to_string_lossy(),
            ],
            root,
        )?;
        Ok(text
            .lines()
            .filter_map(|line| {
                let mut parts = line.split_whitespace();
                let (value, _, name) = (parts.next()?, parts.next()?, parts.next()?);
                Some((name.to_owned(), value.to_owned()))
            })
            .collect())
    };
    let before = table(pass)?;
    let after = table(elf)?;
    let moved: Vec<&String> = before
        .iter()
        .filter(|(name, value)| {
            after
                .get(*name)
                .is_some_and(|final_value| final_value != *value)
        })
        .map(|(name, _)| name)
        .collect();
    if moved.is_empty() {
        Ok(())
    } else {
        Err(format!(
            "symbols moved after the overlays linked: {moved:?}"
        ))
    }
}

/// ROM code calls the resident IWRAM routines through fixed entry addresses,
/// as Camelot's did: a call through a label compiles to a direct `bl`, which
/// cannot reach IWRAM. Each game's IWRAM_CALL.H lists those entries, each
/// naming its routine, and every routine must sit where the linker put it.
fn iwram_entries(root: &Path, target: DecompTarget, elf: &Path) -> Result<(), String> {
    let header = Path::new(target.game_dir()).join("INCLUDE/IWRAM_CALL.H");
    let Ok(text) = fs::read_to_string(root.join(&header)) else {
        return Ok(());
    };
    let entries =
        iwram_entry_list(&text).map_err(|error| format!("{}: {error}", header.display()))?;
    if entries.is_empty() {
        return Ok(());
    }
    let symbols = command(
        &["arm-none-eabi-nm", "--defined-only", &elf.to_string_lossy()],
        root,
    )?;
    let placed: std::collections::BTreeMap<&str, u32> = symbols
        .lines()
        .filter_map(|line| {
            let mut parts = line.split_whitespace();
            let (value, _, name) = (parts.next()?, parts.next()?, parts.next()?);
            Some((name, u32::from_str_radix(value, 16).ok()?))
        })
        .collect();
    for (routine, address) in &entries {
        match placed.get(routine.as_str()) {
            Some(value) if value == address => {}
            Some(value) => {
                return Err(format!(
                    "{}: {routine} is listed at {address:#010x} but linked at {value:#010x}",
                    header.display()
                ))
            }
            None => return Err(format!("{}: {routine} is not linked", header.display())),
        }
    }
    Ok(())
}

/// The (routine, address) pairs of `#define Iwram_X ((type)0x03......) /* Routine */`
/// lines; any other definition spelling an IWRAM address is refused.
fn iwram_entry_list(text: &str) -> Result<Vec<(String, u32)>, String> {
    let entry = regex::Regex::new(
        r"^#define\s+Iwram_\w+\s+\(\(.*\)\s*0x(0?3[0-9a-fA-F]{6})\)\s*/\*\s*(\w+)\s*\*/\s*$",
    )
    .expect("static pattern");
    let address = regex::Regex::new(r"0x0?3[0-9a-fA-F]{6}").expect("static pattern");
    let mut entries = Vec::new();
    for line in text
        .lines()
        .filter(|line| line.trim_start().starts_with("#define"))
    {
        if let Some(capture) = entry.captures(line) {
            let value = u32::from_str_radix(&capture[1], 16).expect("hex digits");
            entries.push((capture[2].to_owned(), value));
        } else if address.is_match(line) {
            return Err(format!("an IWRAM address without its routine: {line}"));
        }
    }
    Ok(entries)
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
        for stream in stream_paths(root, source, base) {
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
fn stream_ids(root: &Path, source: &Path) -> Vec<String> {
    let Ok(text) = fs::read_to_string(root.join(source)) else {
        return Vec::new();
    };
    let pattern = regex::Regex::new(r#"(?m)^\s*\.incbin\s+"overlays/resource_([0-9a-f]+)\.lz""#)
        .expect("static pattern");
    pattern
        .captures_iter(&text)
        .map(|capture| capture[1].to_owned())
        .collect()
}

fn stream_paths(root: &Path, source: &Path, output: &Path) -> Vec<PathBuf> {
    stream_ids(root, source)
        .iter()
        .map(|id| output.join("overlays").join(format!("resource_{id}.lz")))
        .collect()
}

fn build_overlay_streams(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
    symbols: &Path,
) -> Result<(), String> {
    let ids = stream_ids(root, source);
    let listings = source
        .parent()
        .ok_or("source has no directory")?
        .join("raw/overlays");
    let directory = output.join("overlays");
    // The sources each overlay's own script links beside its listing, compiled
    // once for every overlay that shares them.
    let mut linked = Vec::with_capacity(ids.len());
    let mut sources = Vec::new();
    for id in &ids {
        let script = overlay_script(&listings, id);
        let text = fs::read_to_string(root.join(&script))
            .map_err(|error| format!("{}: {error}", script.display()))?;
        let own = format!("resource_{id}_overlay");
        let mut objects = Vec::new();
        for object in script_objects(&text)
            .into_iter()
            .filter(|object| *object != own)
        {
            let source = source_for(root, &object)?;
            objects.push(output.join("obj").join(&source).with_extension("o"));
            if !sources.contains(&source) {
                sources.push(source);
            }
        }
        linked.push((script, objects));
    }
    compile_all(root, target, &sources, output)?;
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
                let (script, objects) = &linked[index];
                if let Err(error) =
                    build_overlay(root, &listings, id, script, objects, symbols, &directory)
                {
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
    Ok(())
}

/// Link one overlay listing alone at its load address, with its own script
/// when it places compiler-library members and the game's otherwise, and
/// compress the image. The map stays beside it for progress.
/// Each overlay's own script, beside its listing, places its objects.
fn overlay_script(listings: &Path, id: &str) -> PathBuf {
    listings.join(format!("resource_{id}.ld"))
}

fn build_overlay(
    root: &Path,
    listings: &Path,
    id: &str,
    script: &Path,
    objects: &[PathBuf],
    symbols: &Path,
    directory: &Path,
) -> Result<(), String> {
    fs::create_dir_all(directory).map_err(|error| error.to_string())?;
    let listing = listings.join(format!("resource_{id}_overlay.s"));
    let path = |name: String| directory.join(name).to_string_lossy().into_owned();
    let object = path(format!("resource_{id}_overlay.o"));
    let elf = path(format!("resource_{id}.elf"));
    let map = path(format!("resource_{id}.map"));
    let image = path(format!("resource_{id}.bin"));
    let stream = directory.join(format!("resource_{id}.lz"));
    let stamp = stream.with_extension("lz.key");
    let own_symbols = path(format!("resource_{id}.symbols.elf"));
    let mut link: Vec<String> = [
        "arm-none-eabi-ld",
        "--no-warn-mismatch",
        "-T",
        &script.to_string_lossy(),
        "-Map",
        &map,
        "-R",
        &own_symbols,
        "-o",
        &elf,
        &object,
    ]
    .map(String::from)
    .to_vec();
    link.extend(
        objects
            .iter()
            .map(|path| path.to_string_lossy().into_owned()),
    );
    link.push("tools/out/compiler-runtime/libgcc.a".into());
    let assemble = assembly_command(&listing.to_string_lossy(), &object);
    let steps = [
        link,
        ["arm-none-eabi-objcopy", "-O", "binary", &elf, &image]
            .map(String::from)
            .to_vec(),
    ];
    let mut hasher = Sha256::new();
    hasher.update(with_includes(root, &root.join(&listing))?);
    hasher.update(fs::read(root.join(script)).map_err(|error| error.to_string())?);
    for object in objects {
        let stamp = object.with_extension("o.key");
        hasher.update(fs::read(&stamp).map_err(|error| format!("{}: {error}", stamp.display()))?);
    }
    for step in std::iter::once(&assemble).chain(&steps) {
        hasher.update(step.join("\0").as_bytes());
    }
    hasher.update(command(&["arm-none-eabi-nm", &symbols.to_string_lossy()], root)?.as_bytes());
    hasher.update(format!("{OVERLAY_MACHINE:?}").as_bytes());
    // The steps between the commands (the names an overlay hides, the
    // packer's transform) belong to this build implementation.
    hasher.update(env!("ALCHEMY_BUILD_IMPLEMENTATION").as_bytes());
    let key = format!("{:x}", hasher.finalize());
    if stream.is_file() && fs::read_to_string(&stamp).ok().as_deref() == Some(key.as_str()) {
        return Ok(());
    }
    let _ = fs::remove_file(&stamp);
    command(&assemble, root)?;
    // The overlay's own names (its import veneers carry the names of the main
    // functions they reach) hide the main image's: the linker sees one each.
    // An overlay reaches only its own copies of the compiler library too.
    let mut own = String::new();
    let library = PathBuf::from("tools/out/compiler-runtime/libgcc.a");
    for path in std::iter::once(PathBuf::from(&object))
        .chain(objects.iter().cloned())
        .chain(std::iter::once(library))
    {
        let table = command(
            &[
                "arm-none-eabi-nm",
                "-g",
                "--defined-only",
                &path.to_string_lossy(),
            ],
            root,
        )?;
        for name in table
            .lines()
            .filter_map(|line| line.split_whitespace().nth(2))
        {
            own.push_str(name);
            own.push('\n');
        }
    }
    let names = directory.join(format!("resource_{id}.own"));
    fs::write(&names, own).map_err(|error| error.to_string())?;
    command(
        &[
            "arm-none-eabi-objcopy",
            &format!("--strip-symbols={}", names.display()),
            &symbols.to_string_lossy(),
            &own_symbols,
        ],
        root,
    )?;
    for step in &steps {
        command(step, root)?;
    }
    let mut decoded = fs::read(&image).map_err(|error| error.to_string())?;
    store_thumb_calls(&mut decoded);
    let encoded = crate::build_assets::encode_overlay_stream(&decoded, &OVERLAY_MACHINE)?;
    fs::write(&stream, encoded).map_err(|error| error.to_string())?;
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

/// The resource packer's form of a code block's Thumb calls: every bl pair
/// holds its target as an offset from the block (less two) instead of from
/// the call, and the loader's PATCH_THUMB_BRANCH kernel turns each pair back
/// into a call from where the block lands. Pairs are found as the kernel finds
/// them, a 0xf800 halfword after a 0xf000 halfword, so the kernel undoes this
/// exactly.
fn store_thumb_calls(block: &mut [u8]) {
    let half = |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
    let mut at = 2;
    while at + 2 <= block.len() {
        let low = half(block, at);
        let high = half(block, at - 2);
        at += 2;
        if low >> 11 != 31 || high >> 11 != 30 {
            continue;
        }
        let call = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
        let stored = call.wrapping_add((at - 2) as u32) & 0x7f_fffe;
        let high = 0xf000 | (stored >> 12);
        let low = 0xf800 | ((stored >> 1) & 0x7ff);
        block[at - 4..at - 2].copy_from_slice(&(high as u16).to_le_bytes());
        block[at - 2..at].copy_from_slice(&(low as u16).to_le_bytes());
    }
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

    /// The loader's kernel, as PATCH_THUMB_BRANCH.S runs it on a block.
    fn patch_thumb_calls(block: &mut [u8]) {
        let half =
            |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
        let mut at = 2;
        while at + 2 <= block.len() {
            let low = half(block, at);
            at += 2;
            if low >> 11 != 31 {
                continue;
            }
            let high = half(block, at - 4);
            if high >> 11 != 30 {
                continue;
            }
            let stored = (((low & 0x7ff) | ((high & 0x7ff) << 11)) << 1) as i64;
            let call = (stored - (at as i64 - 2)) as u32;
            let high = 0xf000 | ((call >> 12) & 0x7ff);
            let low = 0xf800 | ((call >> 1) & 0x7ff);
            block[at - 4..at - 2].copy_from_slice(&(high as u16).to_le_bytes());
            block[at - 2..at].copy_from_slice(&(low as u16).to_le_bytes());
        }
    }

    #[test]
    fn every_iwram_entry_names_its_routine() {
        let header = "/* 0x03000000 is only prose here */\n\
            #define Iwram_CopyWords ((s32 (*)(void *, const void *, s32))0x03001388) /* IwramCopyWords */\n\
            #define Iwram_Sqrt ((s32 (*)(s32))0x030001d8) /* IwramSqrt */\n\
            #define SCREEN_WIDTH 240\n";
        assert_eq!(
            iwram_entry_list(header).unwrap(),
            [
                ("IwramCopyWords".to_owned(), 0x0300_1388),
                ("IwramSqrt".to_owned(), 0x0300_01d8)
            ]
        );
        for unnamed in [
            "#define Iwram_Sqrt ((s32 (*)(s32))0x030001d8)\n",
            "#define SQRT ((s32 (*)(s32))0x030001d8) /* IwramSqrt */\n",
            "#define WORK ((u8 *)0x03001ebc)\n",
        ] {
            assert!(iwram_entry_list(unnamed).is_err(), "{unnamed}");
        }
    }

    #[test]
    fn stored_calls_are_what_the_loader_kernel_undoes() {
        // bl from 0x2d4 to 0x6c in a block: the packer stores the target, less
        // two, as the ROM's overlay 373 does (bl 0x342 read from 0x2d4).
        let mut block = vec![0u8; 0x2d8];
        let call = (0x6c_i64 - (0x2d4 + 4)) as u32;
        block[0x2d4..0x2d6]
            .copy_from_slice(&((0xf000 | ((call >> 12) & 0x7ff)) as u16).to_le_bytes());
        block[0x2d6..0x2d8]
            .copy_from_slice(&((0xf800 | ((call >> 1) & 0x7ff)) as u16).to_le_bytes());
        let linked = block.clone();
        store_thumb_calls(&mut block);
        let stored = u32::from(u16::from_le_bytes([block[0x2d4], block[0x2d5]])) & 0x7ff;
        let stored = (stored << 12)
            | ((u32::from(u16::from_le_bytes([block[0x2d6], block[0x2d7]])) & 0x7ff) << 1);
        assert_eq!(stored, 0x6a);
        patch_thumb_calls(&mut block);
        assert_eq!(block, linked);
        // Every halfword pattern round-trips, pairs in data included.
        let mut noise: Vec<u8> = (0..4096u32)
            .flat_map(|i| (((i * 40503) >> 3) as u16 | 0xf000).to_le_bytes())
            .collect();
        let original = noise.clone();
        store_thumb_calls(&mut noise);
        patch_thumb_calls(&mut noise);
        assert_eq!(noise, original);
    }

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
