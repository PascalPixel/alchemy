//! Link the whole ROM from the objects its linker script lists, in order, as
//! pret links `ld_script.ld`. Every symbol resolves from its definition; the
//! image is written for `sha1sum -c rom.sha1` and nothing is copied from a
//! reference ROM except what the script's scaffolding reads explicitly.
use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::{
    assembly_command, compiler_assembly_command, is_arm, prefer_installed_binutils,
};
use crate::targets::{decomp_target, DecompTarget};
use ags::lz::{compress_tagged, LzMachine};
use psynergy::process::run as command;
use sha2::{Digest, Sha256};
use std::collections::BTreeSet;
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy build rom [--target GAME-EDITION] [--script FILE] [--output DIR] [--keep-going]\n\
Compile and assemble every object the linker script lists, link them in its\n\
order and write the ROM image beside the linker map. Japanese TBS is the default.\n\
--keep-going writes the\n\
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
    let output = output.unwrap_or_else(|| PathBuf::from(target.output_dir));
    let script = match script {
        Some(script) => script,
        None => crate::edition::script(root, target, &output)?,
    };
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

/// The maintained source an object path names: C, then assembly, then a
/// sequence MIDI, which the build converts to assembly as pret's mid2agb does.
fn source_for(root: &Path, object: &str) -> Result<PathBuf, String> {
    ["C", "c", "S", "s", "MID"]
        .iter()
        .map(|extension| PathBuf::from(format!("{object}.{extension}")))
        .find(|path| root.join(path).is_file())
        .ok_or_else(|| format!("{object}.o has no C, assembly or MIDI source"))
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
    prepare_assets(root, target, &sources, &output)?;
    crate::build_text::build(root, target, &output)?;
    // Sources that read built overlay streams wait for the overlays, and the
    // overlays link against the main image's symbols: a first pass links the
    // main image with empty streams, which move nothing the overlays can see.
    let mut streamed = Vec::new();
    let mut direct = Vec::new();
    for source in &sources {
        if stream_paths(root, target, source, &output)?.is_empty() {
            direct.push(source.clone());
        } else {
            streamed.push(source.clone());
        }
    }
    compile_all(root, target, &direct, &output)?;
    let objects: Vec<PathBuf> = sources
        .iter()
        .map(|source| output.join("obj").join(source).with_extension("o"))
        .collect();
    let mut linked_objects: Vec<(PathBuf, PathBuf)> = sources
        .iter()
        .cloned()
        .zip(objects.iter().cloned())
        .collect();
    let mut symbols = None;
    if !streamed.is_empty() {
        let pass = symbols_pass(root, script, &text, &output, name, &sources, &streamed)?;
        for source in &streamed {
            linked_objects.extend(build_overlay_streams(root, target, source, &output, &pass)?);
        }
        compile_all(root, target, &streamed, &output)?;
        symbols = Some(pass);
    }
    linked_objects.sort();
    linked_objects.dedup();
    crate::gate::ids::check(Path::new(target.game_dir()), &linked_objects)?;
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
        // Every build that links game code checks its fixed addresses; a
        // build that is still only the original ROM places none of them.
        if links_game_code(&sources, &objects)? {
            checked_entries(root, target, &elf)?;
        }
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
    pad_to_cartridge(&image)?;
    Ok(Linked {
        objects,
        map,
        image,
    })
}

/// Fill the image with zeros to the next power of two, the size of the ROM
/// chip that holds it, as pret's `gbafix -p` pads a build to its cartridge
/// (with 0xff there). The size follows from the image alone.
fn pad_to_cartridge(image: &Path) -> Result<(), String> {
    let mut bytes = fs::read(image).map_err(|error| format!("{}: {error}", image.display()))?;
    let size = cartridge_size(bytes.len());
    if size != bytes.len() {
        bytes.resize(size, 0);
        fs::write(image, bytes).map_err(|error| format!("{}: {error}", image.display()))?;
    }
    Ok(())
}

/// The smallest power-of-two cartridge that holds `length` bytes.
fn cartridge_size(length: usize) -> usize {
    length.next_power_of_two()
}

/// Whether the link places any code: a C source, or an object with a
/// non-empty `.text`. Pictures and data are not code.
fn links_game_code(sources: &[PathBuf], objects: &[PathBuf]) -> Result<bool, String> {
    for (source, object) in sources.iter().zip(objects) {
        let bytes = fs::read(object).map_err(|error| format!("{}: {error}", object.display()))?;
        if carries_code(source, &bytes)? {
            return Ok(true);
        }
    }
    Ok(false)
}

fn carries_code(source: &Path, object: &[u8]) -> Result<bool, String> {
    use object::{Object, ObjectSection};
    if matches!(source.extension().and_then(|e| e.to_str()), Some("C" | "c")) {
        return Ok(true);
    }
    let file =
        object::File::parse(object).map_err(|error| format!("{}: {error}", source.display()))?;
    Ok(file
        .sections()
        .any(|section| section.name() == Ok(".text") && section.size() > 0))
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
    // The stubs define nothing, so the labels of the streamed sources, such as
    // the directory rows naming each overlay, stay unresolved here; each is a
    // word whose size does not depend on its value, and the final link
    // resolves them all.
    let elf = pass.join(format!("{name}.elf"));
    let mut arguments = link_command(root, script, text, &objects, &elf);
    arguments.insert(1, "--unresolved-symbols=ignore-all".to_owned());
    command(&arguments, root)?;
    Ok(elf)
}

/// Every global symbol, the only kind the overlays can see, keeps its
/// address in the final image.
fn same_addresses(root: &Path, pass: &Path, elf: &Path) -> Result<(), String> {
    let table = |path: &Path| -> Result<std::collections::BTreeMap<String, String>, String> {
        let text = command(
            &[
                "arm-none-eabi-nm",
                "--defined-only",
                "--extern-only",
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

/// A game header of Camelot's own fixed addresses: each `#define <prefix>X`
/// spells one address its code used as a constant, beside the name the
/// linker places there, and the build checks every entry against the link.
struct EntryHeader {
    /// The header, relative to the game directory.
    path: &'static str,
    /// The prefix of every entry's macro name.
    prefix: &'static str,
    /// The address digits an entry may spell, after `0x`.
    region: &'static str,
    /// Digits, after `0x`, that no definition but an entry may spell.
    refused: &'static str,
}

/// ROM code calls the resident IWRAM routines through fixed entry addresses,
/// as Camelot's did: a call through a label compiles to a direct `bl`, which
/// cannot reach IWRAM. Some callers instead add a routine's offset to the
/// bank's first label.
const IWRAM_CALLS: EntryHeader = EntryHeader {
    path: "INCLUDE/IWRAM_CALL.H",
    prefix: "Iwram_",
    region: "0?3[0-9a-fA-F]{6}",
    refused: "0?3[0-9a-fA-F]{6}",
};

/// Camelot's code also reached fixed RAM buffers through constant addresses,
/// which GCC folds with their offsets where it cannot fold a label: into
/// strength-reduced loop bounds and literals shared between neighbours.
const RAM_BUFFERS: EntryHeader = EntryHeader {
    path: "INCLUDE/RAM_BUFFER.H",
    prefix: "Ram_",
    region: "0?[23][0-9a-fA-F]{6}",
    refused: "0?[0-9a-fA-F]{7}",
};

/// Every entry of the game's IWRAM_CALL.H and RAM_BUFFER.H must agree with
/// where the linker put the names beside it.
fn checked_entries(root: &Path, target: DecompTarget, elf: &Path) -> Result<(), String> {
    let mut headers = Vec::new();
    for kind in [&IWRAM_CALLS, &RAM_BUFFERS] {
        let header = Path::new(target.game_dir()).join(kind.path);
        let Ok(text) = fs::read_to_string(root.join(&header)) else {
            continue;
        };
        let entries = entry_list(kind, &text, &[target.edition_define])
            .map_err(|error| format!("{}: {error}", header.display()))?;
        if !entries.is_empty() {
            headers.push((header, entries));
        }
    }
    if headers.is_empty() {
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
    for (header, entries) in &headers {
        entries_agree(entries, |name| placed.get(name).copied())
            .map_err(|error| format!("{}: {error}", header.display()))?;
    }
    Ok(())
}

/// Each entry against `linked`, the address the link gave a name.
fn entries_agree(
    entries: &[CheckedEntry],
    linked: impl Fn(&str) -> Option<u32>,
) -> Result<(), String> {
    let linked = |name: &str| linked(name).ok_or_else(|| format!("{name} is not linked"));
    for entry in entries {
        let value = match &entry.from {
            None => linked(&entry.name)?,
            Some(from) => linked(&entry.name)?.wrapping_sub(linked(from)?),
        };
        if value != entry.value {
            let listed = match &entry.from {
                None => format!("{} is listed at", entry.name),
                Some(from) => format!("{} - {from} is listed as", entry.name),
            };
            return Err(format!(
                "{listed} {:#x} but linked as {value:#x}",
                entry.value
            ));
        }
    }
    Ok(())
}

/// One checked entry: the named place's address, or its offset from another
/// name when `from` names that one.
#[derive(Debug, PartialEq)]
struct CheckedEntry {
    name: String,
    from: Option<String>,
    value: u32,
}

/// The `#define <prefix>X ((type)0x........) /* Name */` entries and the
/// `#define <prefix>XOffset 0x.... /* Name - FromName */` offsets that apply
/// when only `defined` macros are defined: an edition whose layout moves a
/// buffer lists it under `#if defined(EDITION)` beside the others' entry.
/// Any definition, in any branch, spelling an address the header refuses or
/// an offset without its names is refused, as is a condition other than
/// `defined` tests joined by `||`.
fn entry_list(
    kind: &EntryHeader,
    text: &str,
    defined: &[&str],
) -> Result<Vec<CheckedEntry>, String> {
    let (prefix, region, refused) = (kind.prefix, kind.region, kind.refused);
    let entry = regex::Regex::new(&format!(
        r"^#define\s+{prefix}\w+\s+\(\(.*\)\s*0x({region})\)\s*/\*\s*(\w+)\s*\*/\s*$"
    ))
    .expect("static pattern");
    let offset = regex::Regex::new(&format!(
        r"^#define\s+{prefix}\w+Offset\s+0x([0-9a-fA-F]+)\s*/\*\s*(\w+)\s*-\s*(\w+)\s*\*/\s*$"
    ))
    .expect("static pattern");
    let address = regex::Regex::new(&format!("0x{refused}")).expect("static pattern");
    let offset_name =
        regex::Regex::new(&format!(r"^#define\s+{prefix}\w+Offset\b")).expect("static pattern");
    let mut entries = Vec::new();
    // Each open conditional: whether its enclosing text applies, whether its
    // current branch applies, and whether an earlier branch already did.
    let mut open: Vec<(bool, bool, bool)> = Vec::new();
    let applies = |open: &[(bool, bool, bool)]| open.last().is_none_or(|state| state.1);
    for line in text.lines() {
        let directive = line.trim_start();
        let Some(directive) = directive.strip_prefix('#') else {
            continue;
        };
        let directive = directive.trim_start();
        let (word, rest) = directive
            .split_once(char::is_whitespace)
            .unwrap_or((directive, ""));
        match word {
            "if" | "ifdef" | "ifndef" => {
                let outer = applies(&open);
                let holds = match word {
                    "if" => condition(rest, defined)?,
                    "ifdef" => defined.contains(&rest.trim()),
                    _ => !defined.contains(&rest.trim()),
                };
                open.push((outer, outer && holds, holds));
            }
            "elif" => {
                let holds = condition(rest, defined)?;
                let state = open.last_mut().ok_or("#elif without #if")?;
                state.1 = state.0 && !state.2 && holds;
                state.2 |= holds;
            }
            "else" => {
                let state = open.last_mut().ok_or("#else without #if")?;
                state.1 = state.0 && !state.2;
                state.2 = true;
            }
            "endif" => {
                open.pop().ok_or("#endif without #if")?;
            }
            "define" => {
                let found = if let Some(capture) = entry.captures(line) {
                    CheckedEntry {
                        name: capture[2].to_owned(),
                        from: None,
                        value: u32::from_str_radix(&capture[1], 16).expect("hex digits"),
                    }
                } else if let Some(capture) = offset.captures(line) {
                    CheckedEntry {
                        name: capture[2].to_owned(),
                        from: Some(capture[3].to_owned()),
                        value: u32::from_str_radix(&capture[1], 16).expect("hex digits"),
                    }
                } else if address.is_match(line) || offset_name.is_match(line) {
                    return Err(format!("an entry without its linked name: {line}"));
                } else {
                    continue;
                };
                if applies(&open) {
                    entries.push(found);
                }
            }
            _ => {}
        }
    }
    if !open.is_empty() {
        return Err("#if without #endif".into());
    }
    Ok(entries)
}

/// A header condition: `defined(NAME)` or `defined NAME` tests, optionally
/// negated with `!`, joined by `||`.
fn condition(text: &str, defined: &[&str]) -> Result<bool, String> {
    let test = regex::Regex::new(r"^(!?)\s*defined\s*(?:\(\s*(\w+)\s*\)|\s(\w+))$")
        .expect("static pattern");
    let mut holds = false;
    for part in text.split("||") {
        let capture = test
            .captures(part.trim())
            .ok_or_else(|| format!("unsupported condition: {}", text.trim()))?;
        let name = capture.get(2).or(capture.get(3)).expect("a name").as_str();
        holds |= defined.contains(&name) != (&capture[1] == "!");
    }
    Ok(holds)
}

/// Scaffolding reads not-yet-sourced data from the builder's own verified ROM as
/// `baserom.gba`, as early pret builds did; the ROM itself is never tracked.
fn base_rom(root: &Path, target: DecompTarget, output: &Path) -> Result<(), String> {
    let rom = root.join(target.rom);
    let bytes = fs::read(&rom).map_err(|error| format!("{}: {error}", rom.display()))?;
    if bytes.len() as u64 != target.rom_size {
        return Err(format!("{} has the wrong size", rom.display()));
    }
    target.verify_reference(&bytes)?;
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
    let extension = source.extension().and_then(|extension| extension.to_str());
    if extension == Some("MID") {
        return compile_sequence(root, source, object);
    }
    let c = matches!(extension, Some("C" | "c"));
    let (key, steps) = if c {
        let preprocessed = object.with_extension("i");
        let steps = c_steps(target, source, object)?;
        // The routed compile reads its own preprocessed text, so the key is
        // that text plus every command; headers are covered by preprocessing.
        let preprocess = preprocessor_only(&steps, &source_text, &preprocessed)?;
        command(&preprocess, root)?;
        let mut hasher = Sha256::new();
        hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
        hasher.update(fs::read(&preprocessed).map_err(|error| error.to_string())?);
        for step in &steps {
            hasher.update(step.join("\0").as_bytes());
        }
        (format!("{:x}", hasher.finalize()), steps)
    } else {
        let base = object
            .ancestors()
            .find(|path| path.join("baserom.gba").exists())
            .ok_or("the base ROM link is missing")?;
        let mut hasher = assembly_inputs(root, target, source, base)?;
        let step = target_assembly_command(target, &source_text, &object_text, base);
        hasher.update(step.join("\0").as_bytes());
        (format!("{:x}", hasher.finalize()), vec![step])
    };
    if compiled_cache(object, &stamp, &key) {
        return Ok(());
    }
    let _ = fs::remove_file(&stamp);
    for step in &steps {
        command(step, root)?;
    }
    fs::write(object.with_extension("o.digest"), object_digest(object)?)
        .map_err(|error| error.to_string())?;
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

pub(crate) fn compiled_cache(object: &Path, stamp: &Path, key: &str) -> bool {
    if fs::read_to_string(stamp).ok().as_deref() != Some(key) {
        return false;
    }
    let Some(saved) = fs::read_to_string(object.with_extension("o.digest")).ok() else {
        return false;
    };
    object_digest(object).is_ok_and(|actual| actual == saved)
}

fn object_digest(object: &Path) -> Result<String, String> {
    fs::read(object)
        .map(|bytes| format!("{:x}", Sha256::digest(bytes)))
        .map_err(|error| format!("{}: {error}", object.display()))
}

fn c_steps(target: DecompTarget, source: &Path, object: &Path) -> Result<Vec<Vec<String>>, String> {
    let source_text = source.to_string_lossy().into_owned();
    let assembly = object.with_extension("s");
    let mut options = SourceToAssemblyPlanOptions::new(
        target.compiler,
        source_text.clone(),
        source_text.clone(),
        assembly.to_string_lossy().into_owned(),
    );
    options.preprocessor_flags = vec![
        format!("-D{}=1", target.edition_define),
        format!("-I{}", c_import_directory(object)?.display()),
    ];
    options.preprocessed_output = Some(object.with_extension("i").to_string_lossy().into_owned());
    let mut steps = source_to_assembly_plan(&options)?;
    steps.push(compiler_assembly_command(
        &assembly.to_string_lossy(),
        &object.to_string_lossy(),
        is_arm(target.compiler, &source_text),
    ));
    Ok(steps)
}

fn c_import_directory(object: &Path) -> Result<&Path, String> {
    object
        .ancestors()
        .find(|path| path.file_name().is_some_and(|name| name == "obj"))
        .and_then(Path::parent)
        .ok_or_else(|| format!("{} is not under a build's obj directory", object.display()))
}

fn assembly_inputs(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
) -> Result<Sha256, String> {
    let directory = root.join(output);
    let mut hasher = Sha256::new();
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    hasher.update(with_includes(&[root, &directory], &root.join(source))?);
    // Built editable assets and code streams are actual assembler inputs.
    let built = sound_files(root, source)
        .into_iter()
        .chain(graphics_files(root, source))
        .map(|built| directory.join(built));
    for input in stream_paths(root, target, source, &directory)?
        .into_iter()
        .chain(built)
    {
        hasher.update(fs::read(&input).map_err(|error| format!("{}: {error}", input.display()))?);
    }
    Ok(hasher)
}

/// Convert a sequence MIDI to assembly beside its object and assemble it,
/// reusing the object while the converted text and command are unchanged.
fn compile_sequence(root: &Path, source: &Path, object: &Path) -> Result<(), String> {
    let midi = fs::read(root.join(source)).map_err(|error| error.to_string())?;
    let text = ags::sound::sequence_assembly(&midi)?;
    let assembly = object.with_extension("s");
    let step = assembly_command(&assembly.to_string_lossy(), &object.to_string_lossy());
    let mut hasher = Sha256::new();
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    hasher.update(text.as_bytes());
    hasher.update(step.join("\0").as_bytes());
    let key = format!("{:x}", hasher.finalize());
    let stamp = object.with_extension("o.key");
    if compiled_cache(object, &stamp, &key) {
        return Ok(());
    }
    let _ = fs::remove_file(&stamp);
    fs::write(&assembly, text).map_err(|error| error.to_string())?;
    command(&step, root)?;
    fs::write(object.with_extension("o.digest"), object_digest(object)?)
        .map_err(|error| error.to_string())?;
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

/// The sound files a game's data source reads, as pret's data files read the
/// `.bin` files its build makes: `.incbin "SOUND/SAMPLE/WAVE_00.PCM8.bin"`
/// names the file the build writes from `SOUND/SAMPLE/WAVE_00.PCM8.WAV`,
/// relative to the build directory. `COMMON/SOUND/...` names a file built
/// from the input of that path in games/COMMON, one both games share.
fn sound_files(root: &Path, source: &Path) -> Vec<String> {
    let Ok(text) = fs::read_to_string(root.join(source)) else {
        return Vec::new();
    };
    let pattern =
        regex::Regex::new(r#"(?m)^\s*\.incbin\s+"((?:COMMON/)?SOUND/[A-Za-z0-9_./]+\.bin)""#)
            .expect("static pattern");
    pattern
        .captures_iter(&text)
        .map(|capture| capture[1].to_owned())
        .collect()
}

/// Write every sound file `source` reads from the game's editable input of
/// the same name, rewriting a file only when its bytes change.
fn build_sound_files(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
) -> Result<(), String> {
    for built in sound_files(root, source) {
        let stem = built.strip_suffix(".bin").expect("pattern ends in .bin");
        if stem.split('/').any(|part| part.is_empty() || part == "..") {
            return Err(format!(
                "{}: {built} is not a sound file path",
                source.display()
            ));
        }
        let (game, stem) = match stem.strip_prefix("COMMON/") {
            Some(shared) => ("games/COMMON", shared),
            None => (target.game_dir(), stem),
        };
        let inputs: Vec<PathBuf> = ["WAV", "PCM4"]
            .iter()
            .map(|extension| Path::new(game).join(format!("{stem}.{extension}")))
            .filter(|path| root.join(path).is_file())
            .collect();
        let [input] = inputs.as_slice() else {
            return Err(format!(
                "{}: {built} needs exactly one WAV or PCM4 input",
                source.display()
            ));
        };
        let bytes = fs::read(root.join(input)).map_err(|error| error.to_string())?;
        let encoded = ags::sound::build_sound_file(&input.to_string_lossy(), &bytes)
            .map_err(|error| format!("{}: {error}", input.display()))?;
        let path = output.join(&built);
        if fs::read(&path).ok().as_deref() == Some(encoded.as_slice()) {
            continue;
        }
        fs::create_dir_all(path.parent().expect("sound file directory"))
            .map_err(|error| error.to_string())?;
        fs::write(&path, encoded).map_err(|error| error.to_string())?;
    }
    Ok(())
}

fn prepare_assets(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
    output: &Path,
) -> Result<(), String> {
    for source in sources {
        build_sound_files(root, target, source, output)?;
        build_graphics_files(root, target, source, output)?;
    }
    Ok(())
}

/// The compressor Camelot's resource packer ran on every code overlay. Each
/// overlay takes the smaller of the two encodings, the palette one on ties.
const OVERLAY_MACHINE: LzMachine = ags::resource::PACKER;

/// The resource files an assembly source reads with
/// `.incbin "GRAPHICS/..."`, or as assembler source with `.include`, each
/// named by its recipe. `COMMON/GRAPHICS/...` names a file built from the
/// input of that path in games/COMMON, one both games share.
fn graphics_files(root: &Path, source: &Path) -> Vec<String> {
    let Ok(text) = fs::read_to_string(root.join(source)) else {
        return Vec::new();
    };
    let pattern = regex::Regex::new(
        r#"(?m)^\s*\.(?:incbin|include)\s+"((?:COMMON/)?(?:GRAPHICS|MAP)/[A-Za-z0-9_./]+)""#,
    )
    .expect("static pattern");
    pattern
        .captures_iter(&text)
        .map(|capture| capture[1].to_owned())
        .collect()
}

/// Write every resource file `source` reads from the input its name gives
/// under the `SRC` of the source's game or of `COMMON`, as pret's graphics rules build what its data sources read,
/// rewriting a file only when its bytes change.
fn build_graphics_files(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
) -> Result<(), String> {
    for built in graphics_files(root, source) {
        if built.split('/').any(|part| part.is_empty() || part == "..") {
            return Err(format!(
                "{}: {built} is not a resource file path",
                source.display()
            ));
        }
        // Inputs sit under the SRC of the game, or COMMON, whose source reads
        // them, or of COMMON when the name says so.
        let (game, recipe) = match built.strip_prefix("COMMON/") {
            Some(shared) => (Path::new("games/COMMON"), shared),
            None if source.starts_with("games/COMMON") => (Path::new("games/COMMON"), &*built),
            None => (Path::new(target.game_dir()), &*built),
        };
        let image = game.join("SRC").join(ags::resource::input_name(recipe)?);
        let png = fs::read(root.join(&image))
            .map_err(|error| format!("{}: {}: {error}", source.display(), image.display()))?;
        let encoded = ags::resource::build_file_with(recipe, &png, &|name| {
            let path = root.join(ags::resource::sibling_path(&image, name)?);
            fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
        })?;
        let path = output.join(&built);
        if fs::read(&path).ok().as_deref() == Some(encoded.as_slice()) {
            continue;
        }
        fs::create_dir_all(path.parent().expect("resource file directory"))
            .map_err(|error| error.to_string())?;
        fs::write(&path, encoded).map_err(|error| error.to_string())?;
    }
    Ok(())
}

/// The code overlays an assembly source reads with
/// `.incbin "overlays/resource_XXX.lz"`, each linked from the game's common
/// listing directory and compressed, as pret builds compressed files its data
/// sources read.
pub(crate) fn stream_ids(
    root: &Path,
    target: DecompTarget,
    source: &Path,
) -> Result<Vec<String>, String> {
    if !matches!(
        source.extension().and_then(|ext| ext.to_str()),
        Some("S" | "s")
    ) {
        return Ok(Vec::new());
    }
    let text = fs::read_to_string(root.join(source))
        .map_err(|error| format!("{}: {error}", source.display()))?;
    if !text.contains("overlays/resource_") {
        return Ok(Vec::new());
    }
    let output = root.join(target.output_dir);
    let selected = crate::compiler::assembly_source::selected_includes(
        &text,
        target.edition_define,
        &[root, &output],
    )
    .map_err(|error| format!("{}: {error}", source.display()))?;
    let selected = crate::compiler::assembly_source::without_comments(&selected);
    let pattern = regex::Regex::new(r#"(?m)^\s*\.incbin\s+"overlays/resource_([0-9a-f]+)\.lz""#)
        .expect("static pattern");
    let mut found = Vec::new();
    let mut seen = BTreeSet::new();
    let mut macro_depth = 0usize;
    for row in selected.lines() {
        let operation = row.trim().split_whitespace().next().unwrap_or("");
        if operation == ".macro" {
            macro_depth += 1;
        }
        if operation == ".endm" {
            macro_depth = macro_depth.saturating_sub(1);
        }
        if let Some(capture) = pattern.captures(row) {
            if macro_depth > 0 {
                return Err(format!(
                    "{}: overlay stream inside an unexpanded macro",
                    source.display()
                ));
            }
            let id = capture[1].to_string();
            if seen.insert(id.clone()) {
                found.push(id);
            }
        }
    }
    if !found.is_empty() {
        dedicated_stream_source(&selected)
            .map_err(|error| format!("{}: {error}", source.display()))?;
    }
    Ok(found)
}

/// symbols_pass removes the whole stream owner, so it may carry only the
/// terminal overlay section. Resident bytes must keep their own object.
fn dedicated_stream_source(text: &str) -> Result<(), String> {
    let mut overlays = false;
    for row in text.lines().map(str::trim).filter(|row| !row.is_empty()) {
        let operation = row.split_whitespace().next().unwrap();
        if operation == ".section" {
            overlays = row[operation.len()..].trim().split(',').next() == Some(".overlays");
            if !overlays {
                return Err("stream source also selects a resident section; extract terminal streams into a dedicated .overlays source".into());
            }
        } else if matches!(
            operation,
            ".text" | ".data" | ".bss" | ".pushsection" | ".popsection" | ".previous" | ".include"
        ) || (!operation.starts_with('.') && !row.ends_with(':'))
            || (!overlays
                && !row.ends_with(':')
                && !matches!(
                    operation,
                    ".syntax"
                        | ".arch"
                        | ".cpu"
                        | ".thumb"
                        | ".arm"
                        | ".global"
                        | ".globl"
                        | ".type"
                        | ".size"
                ))
        {
            return Err("stream source may remove resident bytes in symbols_pass; extract terminal streams into a dedicated .overlays source".into());
        }
    }
    Ok(())
}

fn stream_paths(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
) -> Result<Vec<PathBuf>, String> {
    Ok(stream_ids(root, target, source)?
        .iter()
        .map(|id| output.join("overlays").join(format!("resource_{id}.lz")))
        .collect())
}

fn overlay_listings(target: DecompTarget) -> PathBuf {
    Path::new(target.asm_dir).join("overlays")
}

fn target_assembly_command(
    target: DecompTarget,
    source: &str,
    object: &str,
    include: &Path,
) -> Vec<String> {
    let mut step = assembly_command(source, object);
    step.splice(
        1..1,
        [
            "--defsym".into(),
            format!("{}=1", target.edition_define),
            format!("-I{}", include.display()),
        ],
    );
    step
}

/// Link and compress every overlay `source` reads, and return each object
/// the overlays link with its source: their maintained objects and listings.
fn build_overlay_streams(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
    symbols: &Path,
) -> Result<Vec<(PathBuf, PathBuf)>, String> {
    let ids = &stream_ids(root, target, source)?;
    let listings = overlay_listings(target);
    let directory = output.join("overlays");
    // The sources each overlay's own script links beside its listing, compiled
    // once for every overlay that shares them.
    let mut linked = Vec::with_capacity(ids.len());
    let mut sources = Vec::new();
    for id in ids {
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
    let mut asset_sources = sources.clone();
    asset_sources.extend(
        ids.iter()
            .map(|id| listings.join(format!("resource_{id}_overlay.s"))),
    );
    prepare_assets(root, target, &asset_sources, output)?;
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
                if let Err(error) = build_overlay(
                    root, target, output, &listings, id, script, objects, symbols, &directory,
                ) {
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
    let mut objects: Vec<(PathBuf, PathBuf)> = sources
        .into_iter()
        .map(|source| {
            let object = output.join("obj").join(&source).with_extension("o");
            (source, object)
        })
        .collect();
    objects.extend(ids.iter().map(|id| {
        (
            listings.join(format!("resource_{id}_overlay.s")),
            directory.join(format!("resource_{id}_overlay.o")),
        )
    }));
    Ok(objects)
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
    target: DecompTarget,
    output: &Path,
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
    let library = PathBuf::from("tools/out/compiler-runtime/libgcc.a");
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
    link.push(library.to_string_lossy().into_owned());
    let assemble = target_assembly_command(target, &listing.to_string_lossy(), &object, output);
    let steps = [
        link,
        ["arm-none-eabi-objcopy", "-O", "binary", &elf, &image]
            .map(String::from)
            .to_vec(),
    ];
    let mut hasher = overlay_inputs(root, output, &listing, script, objects, &library)?;
    for step in std::iter::once(&assemble).chain(&steps) {
        hasher.update(step.join("\0").as_bytes());
    }
    hasher.update(command(&["arm-none-eabi-nm", &symbols.to_string_lossy()], root)?.as_bytes());
    hasher.update(format!("{OVERLAY_MACHINE:?}").as_bytes());
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    // The steps between the commands (the names an overlay hides, the
    // packer's transform) belong to this build implementation.
    hasher.update(env!("ALCHEMY_BUILD_IMPLEMENTATION").as_bytes());
    let key = format!("{:x}", hasher.finalize());
    // The gates run on every build, on a reused link too: this one on the
    // linked image, the id gate on the listing's object.
    if fs::read_to_string(&stamp).ok().as_deref() == Some(key.as_str())
        && valid_overlay_products(directory, id)
    {
        return overlay_gate(&elf, &map);
    }
    let _ = fs::remove_file(&stamp);
    command(&assemble, root)?;
    // The overlay's own names (its import veneers carry the names of the main
    // functions they reach) hide the main image's: the linker sees one each.
    // An overlay reaches only its own copies of the compiler library too.
    let mut own = String::new();
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
    overlay_gate(&elf, &map)?;
    let mut decoded = fs::read(&image).map_err(|error| error.to_string())?;
    store_thumb_calls(&mut decoded);
    let encoded = compress_tagged(&decoded, &OVERLAY_MACHINE)?;
    fs::write(&stream, encoded).map_err(|error| error.to_string())?;
    fs::write(
        stream.with_extension("lz.digest"),
        products_digest(&overlay_products(directory, id))?,
    )
    .map_err(|error| error.to_string())?;
    fs::write(&stamp, key).map_err(|error| error.to_string())
}

/// Every kept product used by the build, gates or diagnostic maps belongs to
/// one completed source link and compression. The input key stays separate.
pub(crate) fn overlay_products(directory: &Path, id: &str) -> [PathBuf; 5] {
    [
        format!("resource_{id}_overlay.o"),
        format!("resource_{id}.elf"),
        format!("resource_{id}.map"),
        format!("resource_{id}.bin"),
        format!("resource_{id}.lz"),
    ]
    .map(|name| directory.join(name))
}

fn products_digest(paths: &[PathBuf]) -> Result<String, String> {
    let mut hasher = Sha256::new();
    for path in paths {
        let name = path
            .file_name()
            .ok_or("product has no file name")?
            .to_string_lossy();
        let bytes = fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?;
        hasher.update((name.len() as u64).to_le_bytes());
        hasher.update(name.as_bytes());
        hasher.update((bytes.len() as u64).to_le_bytes());
        hasher.update(bytes);
    }
    Ok(format!("{:x}", hasher.finalize()))
}

pub(crate) fn valid_overlay_products(directory: &Path, id: &str) -> bool {
    let receipt = directory.join(format!("resource_{id}.lz.digest"));
    let Ok(saved) = fs::read_to_string(receipt) else {
        return false;
    };
    products_digest(&overlay_products(directory, id)).is_ok_and(|actual| actual == saved)
}

/// Every file that feeds an overlay's link, including the compiler runtime
/// archive. Keeping the same archive path does not keep its linked bytes.
fn overlay_inputs(
    root: &Path,
    output: &Path,
    listing: &Path,
    script: &Path,
    objects: &[PathBuf],
    library: &Path,
) -> Result<Sha256, String> {
    let mut hasher = Sha256::new();
    hasher.update(with_includes(&[root, output], &root.join(listing))?);
    for built in sound_files(root, listing)
        .into_iter()
        .chain(graphics_files(root, listing))
    {
        let path = output.join(&built);
        hasher.update(fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?);
    }
    for path in std::iter::once(root.join(script))
        .chain(objects.iter().map(|object| object.with_extension("o.key")))
        .chain(std::iter::once(root.join(library)))
    {
        hasher.update(fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))?);
    }
    Ok(hasher)
}

/// Overlay code never branches straight into the main image: the gate reads
/// the linked ELF and its map, before the packer's call transform.
fn overlay_gate(elf: &str, map: &str) -> Result<(), String> {
    let image = fs::read(elf).map_err(|error| format!("{elf}: {error}"))?;
    let map = fs::read_to_string(map).map_err(|error| format!("{map}: {error}"))?;
    crate::gate::overlay::check(&image, &map)
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
pub(crate) fn preprocessor_only(
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

/// Assembly text followed by every file it `.include`s, recursively, each
/// found as the assembler finds it: in the first of `directories` (the
/// working directory, then the build directory) that holds it.
fn with_includes(directories: &[&Path], source: &Path) -> Result<Vec<u8>, String> {
    let text = fs::read(source).map_err(|error| format!("{}: {error}", source.display()))?;
    let mut key = text.clone();
    for line in String::from_utf8_lossy(&text).lines() {
        let Some(rest) = line.trim().strip_prefix(".include") else {
            continue;
        };
        let include = rest.trim().trim_matches('"');
        let path = directories
            .iter()
            .map(|directory| directory.join(include))
            .find(|path| path.is_file())
            .unwrap_or_else(|| directories[0].join(include));
        key.extend_from_slice(&with_includes(directories, &path)?);
    }
    Ok(key)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn kept_overlay_products_are_rebuilt_after_any_preserved_timestamp_overwrite() {
        prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let listings = Path::new("recon/tbs/raw/overlays");
        fs::create_dir_all(root.join(listings)).unwrap();
        fs::write(root.join(listings).join("resource_36f_overlay.s"), ".section .text,\"ax\"\n.thumb\n.global Candidate\n.type Candidate,%function\n.thumb_func\nCandidate:\nmovs r0,#7\nbx lr\n.size Candidate,.-Candidate\n").unwrap();
        let script = listings.join("resource_36f.ld");
        fs::write(
            root.join(&script),
            "SECTIONS { .text 0x02008000 : { *(.text) } }\n",
        )
        .unwrap();
        let runtime = root.join("tools/out/compiler-runtime/libgcc.a");
        fs::create_dir_all(runtime.parent().unwrap()).unwrap();
        fs::write(runtime, "!<arch>\n").unwrap();
        let main = root.join("MAIN.S");
        let object = root.join("main.o");
        let symbols = root.join("main.elf");
        fs::write(
            &main,
            ".text\n.thumb\n.global Resident\n.thumb_func\nResident:\nbx lr\n",
        )
        .unwrap();
        command(
            &assembly_command(&main.to_string_lossy(), &object.to_string_lossy()),
            root,
        )
        .unwrap();
        command(
            &[
                "arm-none-eabi-ld",
                "-Ttext=0x08000000",
                "-o",
                &symbols.to_string_lossy(),
                &object.to_string_lossy(),
            ],
            root,
        )
        .unwrap();
        let output = root.join(target.output_dir);
        let directory = output.join("overlays");
        let replay = || {
            build_overlay(
                root,
                target,
                &output,
                listings,
                "36f",
                &script,
                &[],
                &symbols,
                &directory,
            )
            .unwrap()
        };
        replay();
        let stamp = directory.join("resource_36f.lz.key");
        let key = fs::read(&stamp).unwrap();
        let products = overlay_products(&directory, "36f");
        let original = products
            .iter()
            .map(|path| fs::read(path).unwrap())
            .collect::<Vec<_>>();
        assert!(valid_overlay_products(&directory, "36f"));
        for (index, path) in products.iter().enumerate() {
            let time = fs::metadata(path).unwrap().modified().unwrap();
            let mut overwritten = original[index].clone();
            overwritten[0] ^= 0xff;
            fs::write(path, overwritten).unwrap();
            fs::File::open(path)
                .unwrap()
                .set_times(fs::FileTimes::new().set_modified(time))
                .unwrap();
            assert_eq!(fs::read(&stamp).unwrap(), key);
            assert!(
                !valid_overlay_products(&directory, "36f"),
                "{}",
                path.display()
            );
            replay();
            assert!(valid_overlay_products(&directory, "36f"));
            for (actual, expected) in products.iter().zip(&original) {
                assert_eq!(
                    fs::read(actual).unwrap(),
                    *expected,
                    "{} after {} overwrite",
                    actual.display(),
                    path.display()
                );
            }
            assert_eq!(fs::read(&stamp).unwrap(), key);
        }
        fs::remove_file(directory.join("resource_36f.lz.digest")).unwrap();
        assert!(!valid_overlay_products(&directory, "36f"));
        replay();
        assert!(valid_overlay_products(&directory, "36f"));
    }

    #[test]
    fn edition_streams_use_the_games_common_listing_root_and_active_composition() {
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let source = Path::new("recon/tbs/ja/rom.s");
        fs::create_dir_all(root.join(source).parent().unwrap()).unwrap();
        fs::write(root.join(source), ".section .overlays,\"a\"\n.ifdef TBS_EDITION_JA\n.incbin \"overlays/resource_36f.lz\"\n.incbin \"overlays/resource_36f.lz\"\n.else\n.incbin \"overlays/resource_370.lz\"\n.endif\n@ .incbin \"overlays/resource_dead.lz\"\n/*\n.incbin \"overlays/resource_bad.lz\"\n*/\n").unwrap();
        let ja = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let en = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        assert_eq!(stream_ids(root, ja, source).unwrap(), ["36f"]);
        assert_eq!(stream_ids(root, en, source).unwrap(), ["370"]);
        assert_eq!(overlay_listings(ja), Path::new("recon/tbs/raw/overlays"));
        let tla = crate::targets::target_for(crate::targets::DecompTargetId::TlaDe);
        assert_eq!(overlay_listings(tla), Path::new("recon/tla/raw/overlays"));
        fs::write(
            root.join(source),
            ".macro stream\n.incbin \"overlays/resource_36f.lz\"\n.endm\n",
        )
        .unwrap();
        assert!(stream_ids(root, ja, source)
            .unwrap_err()
            .contains("unexpanded macro"));
        fs::write(root.join(source), ".section .resident\nResident:\n.incbin \"baserom.gba\", 0, 4\n.section .overlays\n.incbin \"overlays/resource_36f.lz\"\n").unwrap();
        assert!(stream_ids(root, ja, source)
            .unwrap_err()
            .contains("dedicated .overlays"));
    }

    #[test]
    fn raw_listing_assembles_with_its_edition_and_nested_generated_include_key() {
        use object::{Object, ObjectSection};
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let output = root.join("out/ja");
        fs::create_dir_all(&output).unwrap();
        fs::write(output.join("outer.inc"), ".include \"values.inc\"\n").unwrap();
        fs::write(output.join("values.inc"), ".set Pick, 7\n").unwrap();
        let listing = Path::new("listing.s");
        fs::write(root.join(listing), ".section .rodata\n.include \"outer.inc\"\n.ifdef TBS_EDITION_JA\n.byte Pick\n.else\n.byte 3\n.endif\n").unwrap();
        let script = Path::new("overlay.ld");
        let library = Path::new("libgcc.a");
        fs::write(root.join(script), "SECTIONS { .text : { *(.text) } }\n").unwrap();
        fs::write(root.join(library), "synthetic runtime content").unwrap();
        let ja = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let en = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        let key = |target| {
            let mut hash = overlay_inputs(root, &output, listing, script, &[], library).unwrap();
            hash.update(
                target_assembly_command(target, "listing.s", "listing.o", &output).join("\0"),
            );
            format!("{:x}", hash.finalize())
        };
        let before = key(ja);
        assert_ne!(before, key(en));
        let assemble = |target| {
            let object = root.join("listing.o");
            let mut step = target_assembly_command(
                target,
                &root.join(listing).to_string_lossy(),
                &object.to_string_lossy(),
                &output,
            );
            step[0] = crate::compiler::routing::binutils_prefix()
                .join("bin/arm-none-eabi-as")
                .to_string_lossy()
                .into_owned();
            command(&step, root).unwrap();
            let bytes = fs::read(&object).unwrap();
            object::File::parse(&*bytes)
                .unwrap()
                .section_by_name(".rodata")
                .unwrap()
                .data()
                .unwrap()
                .to_vec()
        };
        assert_eq!(assemble(ja), [7]);
        assert_eq!(assemble(en), [3]);
        fs::write(output.join("values.inc"), ".set Pick, 9\n").unwrap();
        assert_ne!(before, key(ja));
        assert_eq!(assemble(ja), [9]);
    }

    #[test]
    fn replay_compiles_current_dependencies_and_rejects_an_overwritten_cached_object() {
        prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TlaEs);
        let source = Path::new("OWNER.C");
        let object = root.join("obj/OWNER.o");
        let stamp = object.with_extension("o.key");
        fs::write(root.join("OWN.H"), "#define VALUE 1\n").unwrap();
        fs::write(
            root.join(source),
            "#include \"OWN.H\"\nint Owner(int n) { return n + VALUE; }\n",
        )
        .unwrap();
        compile(root, target, source, &object).unwrap();
        let before = fs::read(&object).unwrap();
        fs::write(root.join("OWN.H"), "#define VALUE 2\n").unwrap();
        compile(root, target, source, &object).unwrap();
        let current = fs::read(&object).unwrap();
        assert_ne!(current, before);
        let key = fs::read_to_string(&stamp).unwrap();
        let original_time = fs::metadata(&object).unwrap().modified().unwrap();
        fs::write(&object, b"later experiment, current input key retained").unwrap();
        fs::File::options()
            .write(true)
            .open(&object)
            .unwrap()
            .set_times(fs::FileTimes::new().set_modified(original_time))
            .unwrap();
        assert!(!compiled_cache(&object, &stamp, &key));
        compile(root, target, source, &object).unwrap();
        assert_eq!(fs::read(object).unwrap(), current);
    }

    #[test]
    fn overlay_owned_editable_assets_are_prepared_and_raw_incbin_changes_key() {
        let work = tempfile::tempdir().unwrap();
        let root = work.path();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsJa);
        let source = PathBuf::from("games/COMMON/SRC/OVERLAY/DATA.S");
        let listing = PathBuf::from("recon/tbs/raw/overlays/resource_36f_overlay.s");
        for path in [&source, &listing] {
            fs::create_dir_all(root.join(path).parent().unwrap()).unwrap();
        }
        fs::write(
            root.join(&source),
            ".incbin \"COMMON/GRAPHICS/OWN.table\"\n",
        )
        .unwrap();
        fs::write(
            root.join(&listing),
            ".incbin \"COMMON/GRAPHICS/RAW.table\"\n",
        )
        .unwrap();
        let input = root.join("games/COMMON/SRC/GRAPHICS");
        fs::create_dir_all(&input).unwrap();
        fs::write(input.join("OWN.TSV"), "value:u16\n2\n").unwrap();
        fs::write(input.join("RAW.TSV"), "value:u16\n3\n").unwrap();
        let output = root.join("out/tbs-ja");
        prepare_assets(root, target, &[source, listing.clone()], &output).unwrap();
        assert_eq!(
            fs::read(output.join("COMMON/GRAPHICS/OWN.table")).unwrap(),
            [2, 0]
        );
        assert_eq!(
            fs::read(output.join("COMMON/GRAPHICS/RAW.table")).unwrap(),
            [3, 0]
        );
        fs::write(root.join("overlay.ld"), "SECTIONS { .text : { *(.text) } }").unwrap();
        fs::write(root.join("libgcc.a"), "synthetic runtime content").unwrap();
        let key = || {
            format!(
                "{:x}",
                overlay_inputs(
                    root,
                    &output,
                    &listing,
                    Path::new("overlay.ld"),
                    &[],
                    Path::new("libgcc.a")
                )
                .unwrap()
                .finalize()
            )
        };
        let before = key();
        fs::write(input.join("RAW.TSV"), "value:u16\n4\n").unwrap();
        prepare_assets(root, target, &[listing.clone()], &output).unwrap();
        assert_ne!(before, key());
    }

    #[test]
    fn overlay_cache_changes_when_the_runtime_archive_is_rebuilt_in_place() {
        let dir = tempfile::tempdir().unwrap();
        let root = dir.path();
        let listing = Path::new("overlay.s");
        let script = Path::new("overlay.ld");
        let library = Path::new("libgcc.a");
        let objects = [root.join("code.o")];
        fs::write(root.join(listing), ".thumb\nbx lr\n").unwrap();
        fs::write(root.join(script), "SECTIONS { .text : { *(.text) } }\n").unwrap();
        fs::write(objects[0].with_extension("o.key"), "unchanged-object-key").unwrap();
        fs::write(root.join(library), b"first runtime archive").unwrap();
        let key = || {
            format!(
                "{:x}",
                overlay_inputs(root, root, listing, script, &objects, library)
                    .unwrap()
                    .finalize()
            )
        };
        let before = key();
        assert_eq!(key(), before);
        fs::write(root.join(library), b"rebuilt runtime archive").unwrap();
        assert_ne!(key(), before);
        fs::remove_file(root.join(library)).unwrap();
        assert!(overlay_inputs(root, root, listing, script, &objects, library).is_err());
    }

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
    fn an_image_fills_the_smallest_power_of_two_cartridge_with_zeros() {
        assert_eq!(cartridge_size(0x7f_d4bc), 0x80_0000);
        assert_eq!(cartridge_size(0x80_0000), 0x80_0000);
        assert_eq!(cartridge_size(0x80_0001), 0x100_0000);
        let directory = tempfile::tempdir().unwrap();
        let image = directory.path().join("a.gba");
        fs::write(&image, [1u8, 2, 3]).unwrap();
        pad_to_cartridge(&image).unwrap();
        assert_eq!(fs::read(&image).unwrap(), [1, 2, 3, 0]);
    }

    #[test]
    fn every_iwram_entry_names_its_routine() {
        let header = "/* 0x03000000 is only prose here */\n\
            #define Iwram_CopyWords ((s32 (*)(void *, const void *, s32))0x03001388) /* IwramCopyWords */\n\
            #define Iwram_Sqrt ((s32 (*)(s32))0x030001d8) /* IwramSqrt */\n\
            #define Iwram_CopyWordsOffset 0x1388 /* IwramCopyWords - IwramIrqMain */\n\
            #define SCREEN_WIDTH 240\n";
        assert_eq!(
            entry_list(&IWRAM_CALLS, header, &[]).unwrap(),
            [
                entry("IwramCopyWords", None, 0x0300_1388),
                entry("IwramSqrt", None, 0x0300_01d8),
                entry("IwramCopyWords", Some("IwramIrqMain"), 0x1388),
            ]
        );
        for unnamed in [
            "#define Iwram_Sqrt ((s32 (*)(s32))0x030001d8)\n",
            "#define SQRT ((s32 (*)(s32))0x030001d8) /* IwramSqrt */\n",
            "#define WORK ((u8 *)0x03001ebc)\n",
            "#define Iwram_CopyWordsOffset 0x1388\n",
            "#define Iwram_CopyWordsOffset 0x1388 /* IwramCopyWords */\n",
        ] {
            assert!(entry_list(&IWRAM_CALLS, unnamed, &[]).is_err(), "{unnamed}");
        }
    }

    fn entry(name: &str, from: Option<&str>, value: u32) -> CheckedEntry {
        CheckedEntry {
            name: name.to_owned(),
            from: from.map(str::to_owned),
            value,
        }
    }

    #[test]
    fn every_ram_buffer_names_its_linked_object() {
        let header = "/* 0x02010000 is only prose here */\n\
            #define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */\n\
            #define Ram_IwramHeapEnd ((u8 *)0x03007800) /* gIwramHeapEnd */\n\
            #define SCREEN_WIDTH 240\n";
        assert_eq!(
            entry_list(&RAM_BUFFERS, header, &[]).unwrap(),
            [
                entry("gMapCellBuffer", None, 0x0201_0000),
                entry("gIwramHeapEnd", None, 0x0300_7800),
            ]
        );
        for unnamed in [
            "#define Ram_MapCellBuffer ((u8 *)0x02010000)\n",
            "#define MAP_CELLS ((u8 *)0x02010000) /* gMapCellBuffer */\n",
            "#define PARTICLES ((struct EffectStep *)0x02010000)\n",
            "#define CELL_ADDR 0x03001F30\n",
            "#define Ram_MapCellBufferOffset 0x82\n",
            // A ROM address is no RAM buffer: ROM data has labels.
            "#define Ram_Table ((u8 *)0x0809e8a0) /* Table */\n",
        ] {
            assert!(entry_list(&RAM_BUFFERS, unnamed, &[]).is_err(), "{unnamed}");
        }
    }

    #[test]
    fn an_edition_lists_the_buffers_its_layout_moves() {
        let header = "#ifndef ALCHEMY_RAM_BUFFER_H\n\
            #define ALCHEMY_RAM_BUFFER_H\n\
            #define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */\n\
            #if defined(TBS_EDITION_DE)\n\
            #define Ram_Disp ((u8 **)0x03001f08) /* gDisp */\n\
            #elif defined(TBS_EDITION_JA) || defined TBS_EDITION_IT\n\
            #define Ram_Disp ((u8 **)0x03001e78) /* gDisp */\n\
            #else\n\
            #define Ram_Disp ((u8 **)0x03001ef8) /* gDisp */\n\
            #endif\n\
            #endif\n";
        let disp = |defined: &[&str]| {
            entry_list(&RAM_BUFFERS, header, defined)
                .unwrap()
                .into_iter()
                .map(|entry| entry.value)
                .collect::<Vec<_>>()
        };
        assert_eq!(disp(&["TBS_EDITION_EN"]), [0x0201_0000, 0x0300_1ef8]);
        assert_eq!(disp(&["TBS_EDITION_DE"]), [0x0201_0000, 0x0300_1f08]);
        assert_eq!(disp(&["TBS_EDITION_JA"]), [0x0201_0000, 0x0300_1e78]);
        assert_eq!(disp(&["TBS_EDITION_IT"]), [0x0201_0000, 0x0300_1e78]);
        // An unnamed address is refused even in a branch this edition skips,
        // and conditions beyond defined tests are not guessed.
        for refused in [
            "#if defined(TBS_EDITION_DE)\n#define Ram_Disp ((u8 **)0x03001f08)\n#endif\n",
            "#if TBS_EDITION_DE\n#endif\n",
            "#if defined(TBS_EDITION_DE) && defined(TBS_EDITION_JA)\n#endif\n",
            "#if defined(TBS_EDITION_DE)\n",
            "#endif\n",
        ] {
            assert!(
                entry_list(&RAM_BUFFERS, refused, &["TBS_EDITION_EN"]).is_err(),
                "{refused}"
            );
        }
    }

    #[test]
    fn entries_must_agree_with_the_link() {
        let placed = |name: &str| match name {
            "gMapCellBuffer" => Some(0x0201_0000),
            "IwramIrqMain" => Some(0x0300_0000),
            "IwramCopyWords" => Some(0x0300_1388),
            _ => None,
        };
        let agreeing = [
            entry("gMapCellBuffer", None, 0x0201_0000),
            entry("IwramCopyWords", Some("IwramIrqMain"), 0x1388),
        ];
        assert_eq!(entries_agree(&agreeing, placed), Ok(()));
        assert_eq!(
            entries_agree(&[entry("gMapCellBuffer", None, 0x0201_0002)], placed),
            Err("gMapCellBuffer is listed at 0x2010002 but linked as 0x2010000".into())
        );
        assert_eq!(
            entries_agree(
                &[entry("IwramCopyWords", Some("IwramIrqMain"), 0x1380)],
                placed
            ),
            Err("IwramCopyWords - IwramIrqMain is listed as 0x1380 but linked as 0x1388".into())
        );
        assert_eq!(
            entries_agree(&[entry("gMapBlocks", None, 0x0202_0000)], placed),
            Err("gMapBlocks is not linked".into())
        );
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
    fn fixed_addresses_are_checked_whenever_game_code_links() {
        prefer_installed_binutils();
        let dir = tempfile::tempdir().unwrap();
        let assemble = |name: &str, text: &str| {
            let (source, object) = (
                dir.path().join(name),
                dir.path().join(name).with_extension("o"),
            );
            fs::write(&source, text).unwrap();
            command(
                &assembly_command(&source.to_string_lossy(), &object.to_string_lossy()),
                dir.path(),
            )
            .unwrap();
            (source, object)
        };
        let rom = assemble(
            "rom.s",
            "\t.section .rom.00000000, \"a\"\n\t.byte 1, 2, 3, 4\n",
        );
        let picture = assemble("PICTURE.S", "\t.section .rodata\n\t.byte 5, 6, 7, 8\n");
        let code = assemble("CODE.S", "\t.text\n\t.thumb\n\tbx lr\n");
        let links = |parts: &[&(PathBuf, PathBuf)]| {
            let (sources, objects): (Vec<_>, Vec<_>) = parts.iter().map(|p| (*p).clone()).unzip();
            links_game_code(&sources, &objects).unwrap()
        };
        // A build that is still only the original ROM skips the check.
        assert!(!links(&[&rom]));
        // Pictures and data are not code.
        assert!(!links(&[&rom, &picture]));
        // Any object with code, or any C source, is checked.
        assert!(links(&[&rom, &picture, &code]));
        assert!(
            carries_code(Path::new("games/A/SRC/X.C"), &fs::read(&picture.1).unwrap()).unwrap()
        );
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
#[test]
fn every_c_plan_searches_its_build_message_metadata() {
    let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
    for source in [
        "games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IE.C",
        "games/COMMON/SRC/SYSTEM/SAVE/READ_FLASH_ID.C",
    ] {
        let object = Path::new("/tmp/build-metadata/obj")
            .join(source)
            .with_extension("o");
        let plan = c_steps(target, Path::new(source), &object).unwrap();
        assert!(
            plan[0].iter().any(|flag| flag == "-I/tmp/build-metadata"),
            "{plan:?}"
        );
    }
}
