use crate::compiler::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use crate::compiler::routing::{
    assembly_command, compiler_assembly_command, is_arm, prefer_installed_binutils,
};
use crate::targets::{decomp_target, DecompTarget};
use ags::lz::{compress_tagged, LzMachine};
use crate::process::run as command;
use sha2::{Digest, Sha256};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::Mutex;

const USAGE: &str = "usage: alchemy build rom [--target GAME-EDITION] [--script FILE] [--output DIR] [--keep-going]\n\
Compile and assemble every object the linker script lists, link them in its\n\
order, write the ROM image beside the linker map and fix its cartridge header\n\
(Nintendo logo and complement check) as pret's gbafix does. Japanese TBS is\n\
the default.\n\
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
    clear_unindexed(&root.join(&output), root);
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
    println!("steps={}", linked.steps);
    println!("map={}", linked.map.display());
    println!("rom={}", linked.image.display());
    Ok(())
}

fn clear_unindexed(output: &Path, root: &Path) {
    if output.join(INDEX).is_file() || !output.starts_with(root.join("out")) {
        return;
    }
    for entry in fs::read_dir(output).into_iter().flatten().flatten() {
        if entry.file_name() != "raw" {
            let path = entry.path();
            let _ = fs::remove_dir_all(&path).or_else(|_| fs::remove_file(&path));
        }
    }
}

pub(crate) struct Linked {
    pub objects: Vec<PathBuf>,
    pub steps: usize,
    pub map: PathBuf,
    pub image: PathBuf,
}

pub(crate) fn script_objects(script: &str) -> Vec<String> {
    let pattern = regex::Regex::new(r#""\*/([^"*:]+)\.o""#).expect("static pattern");
    let mut seen = BTreeSet::new();
    pattern
        .captures_iter(script)
        .map(|capture| capture[1].to_owned())
        .filter(|path| seen.insert(path.clone()))
        .collect()
}

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
    link_with(root, target, script, output, name, keep_going, None)
}

/// link, through the matching compilers or, given `modern`, through a modern
/// GCC (`alchemy build modern`): the same objects, assets and scaffold.
pub(crate) fn link_with(
    root: &Path,
    target: DecompTarget,
    script: &Path,
    output: &Path,
    name: &str,
    keep_going: bool,
    modern: Option<&crate::modern::Modern>,
) -> Result<Linked, String> {
    prefer_installed_binutils();
    let output = root.join(output);
    fs::create_dir_all(&output).map_err(|error| error.to_string())?;
    let index = Index::open(&output);
    let build = Build { target, modern };
    let mut linked = link_indexed(root, build, script, &output, name, keep_going, &index);
    if let Ok(linked) = &mut linked {
        linked.steps = index.made.load(Ordering::Relaxed);
    }
    index.save()?;
    linked
}

#[derive(Clone, Copy)]
struct Build<'a> {
    target: DecompTarget,
    modern: Option<&'a crate::modern::Modern>,
}

fn link_indexed(
    root: &Path,
    build: Build,
    script: &Path,
    output: &Path,
    name: &str,
    keep_going: bool,
    index: &Index,
) -> Result<Linked, String> {
    let text = fs::read_to_string(root.join(script))
        .map_err(|error| format!("{}: {error}", script.display()))?;
    let sources = script_objects(&text)
        .iter()
        .map(|object| source_for(root, object))
        .collect::<Result<Vec<_>, _>>()?;
    if sources.is_empty() {
        return Err(format!("{} lists no objects", script.display()));
    }
    let target = build.target;
    base_rom(root, target, output)?;
    prepare_assets(root, target, &sources, output, index)?;
    crate::build_text::build(root, target, output)?;
    let mut streamed = Vec::new();
    let mut direct = Vec::new();
    for source in &sources {
        if stream_paths(root, target, source, output)?.is_empty() {
            direct.push(source.clone());
        } else {
            streamed.push(source.clone());
        }
    }
    compile_all(root, build, &direct, output, index)?;
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
        let pass = symbols_pass(
            root, build, script, &text, output, name, &sources, &streamed, index,
        )?;
        let table = command(&["arm-none-eabi-nm", &pass.to_string_lossy()], root)?;
        let table = format!("{:x}", Sha256::digest(table.as_bytes()));
        for source in &streamed {
            linked_objects.extend(build_overlay_streams(
                root, build, source, output, &pass, &table, index,
            )?);
        }
        compile_all(root, build, &streamed, output, index)?;
        symbols = Some(pass);
    }
    linked_objects.sort();
    linked_objects.dedup();
    // The modern compiler folds ids into neighbouring constants, which says
    // nothing about the source: the id gate judges the matching build only.
    if build.modern.is_none() {
        crate::gate::ids::check(Path::new(target.game_dir()), &linked_objects)?;
    }
    let elf = output.join(format!("{name}.elf"));
    let map = output.join(format!("{name}.map"));
    let image = output.join(format!("{name}.gba"));
    let mut arguments = link_command(root, build, script, &text, &objects, &elf);
    arguments.splice(
        1..1,
        ["-Map".to_owned(), map.to_string_lossy().into_owned()],
    );
    if keep_going {
        arguments.push("--noinhibit-exec".into());
    }
    let products = [image.clone(), elf.clone(), map.clone()];
    let key = link_key(root, build, index, &arguments, &text, &objects)?;
    if !index.fresh(&products, &key) {
        index.forget(&image);
        if let Err(error) = run_link(build, &arguments, output, name) {
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
        finish_image(&image)?;
        if !keep_going {
            index.record(&products, &key, Vec::new())?;
        }
    } else {
        // An overlay relinked to the same bytes leaves this image current:
        // mark it confirmed, so its maps never read as newer than it.
        fs::File::options()
            .write(true)
            .open(&image)
            .and_then(|file| file.set_modified(std::time::SystemTime::now()))
            .map_err(|error| format!("{}: {error}", image.display()))?;
    }
    if let Some(pass) = &symbols {
        same_addresses(root, pass, &elf)?;
    }
    if build.modern.is_none() && links_game_code(&sources, &objects)? {
        checked_entries(root, target, &elf)?;
    }
    for directory in asset_directories(output) {
        let _ = fs::remove_dir_all(directory);
    }
    Ok(Linked {
        steps: 0,
        objects,
        map,
        image,
    })
}

const LIBGCC: &str = "tools/out/compiler-runtime/libgcc.a";

fn link_key(
    root: &Path,
    build: Build,
    index: &Index,
    arguments: &[String],
    text: &str,
    objects: &[PathBuf],
) -> Result<String, String> {
    let mut hasher = Sha256::new();
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    hasher.update(env!("ALCHEMY_HEADER_FIX").as_bytes());
    hasher.update(portable(root, arguments.join("\0").as_bytes()));
    hasher.update(text.as_bytes());
    for object in objects {
        let row = index
            .row(object)
            .ok_or_else(|| format!("{} was not built", object.display()))?;
        hasher.update(row.digest.as_bytes());
    }
    if let Some(modern) = build.modern {
        hasher.update(modern.signature.as_bytes());
    }
    hasher.update(fs::read(root.join(LIBGCC)).unwrap_or_default());
    Ok(format!("{:x}", hasher.finalize()))
}

fn portable(root: &Path, text: &[u8]) -> Vec<u8> {
    let prefix = format!("{}/", root.display());
    let prefix = prefix.as_bytes();
    let mut kept = Vec::with_capacity(text.len());
    let mut at = 0;
    while at < text.len() {
        if text[at..].starts_with(prefix) {
            at += prefix.len();
        } else {
            kept.push(text[at]);
            at += 1;
        }
    }
    kept
}

const INDEX: &str = "cache.tsv";

pub(crate) struct Index {
    output: PathBuf,
    rows: Mutex<BTreeMap<String, Row>>,
    made: AtomicUsize,
}

#[derive(Clone)]
struct Row {
    key: String,
    digest: String,
    inputs: Vec<String>,
}

impl Index {
    pub(crate) fn open(output: &Path) -> Index {
        let text = fs::read_to_string(output.join(INDEX)).unwrap_or_default();
        let rows = text
            .lines()
            .filter_map(|line| {
                let mut fields = line.split('\t');
                let product = fields.next()?.to_owned();
                let key = fields.next()?.to_owned();
                let digest = fields.next()?.to_owned();
                let inputs = fields.map(str::to_owned).collect();
                Some((
                    product,
                    Row {
                        key,
                        digest,
                        inputs,
                    },
                ))
            })
            .collect();
        Index {
            output: output.to_path_buf(),
            rows: Mutex::new(rows),
            made: AtomicUsize::new(0),
        }
    }

    fn name(&self, product: &Path) -> String {
        product
            .strip_prefix(&self.output)
            .unwrap_or(product)
            .to_string_lossy()
            .into_owned()
    }

    fn row(&self, product: &Path) -> Option<Row> {
        self.rows.lock().unwrap().get(&self.name(product)).cloned()
    }

    fn fresh(&self, products: &[PathBuf], key: &str) -> bool {
        self.row(&products[0]).is_some_and(|row| {
            row.key == key && products_digest(products).is_ok_and(|actual| actual == row.digest)
        })
    }

    fn record(&self, products: &[PathBuf], key: &str, inputs: Vec<String>) -> Result<(), String> {
        let row = Row {
            key: key.to_owned(),
            digest: products_digest(products)?,
            inputs,
        };
        self.rows
            .lock()
            .unwrap()
            .insert(self.name(&products[0]), row);
        self.made.fetch_add(1, Ordering::Relaxed);
        Ok(())
    }

    fn forget(&self, product: &Path) {
        self.rows.lock().unwrap().remove(&self.name(product));
    }

    pub(crate) fn save(&self) -> Result<(), String> {
        let mut text = String::new();
        for (product, row) in self.rows.lock().unwrap().iter() {
            text.push_str(product);
            for field in [&row.key, &row.digest].into_iter().chain(&row.inputs) {
                text.push('\t');
                text.push_str(field);
            }
            text.push('\n');
        }
        let path = self.output.join(INDEX);
        let fresh = self.output.join(format!("{INDEX}.new"));
        fs::write(&fresh, text).map_err(|error| format!("{}: {error}", fresh.display()))?;
        fs::rename(&fresh, &path).map_err(|error| format!("{}: {error}", path.display()))
    }
}

fn parallel<T: Sync, R: Send>(
    items: &[T],
    step: impl Fn(&T) -> Result<R, String> + Sync,
) -> Result<Vec<R>, String> {
    let next = AtomicUsize::new(0);
    let results = Mutex::new((0..items.len()).map(|_| None).collect::<Vec<_>>());
    let errors = Mutex::new(Vec::new());
    let workers = std::thread::available_parallelism().map_or(4, |count| count.get());
    std::thread::scope(|scope| {
        for _ in 0..workers.min(items.len()) {
            scope.spawn(|| loop {
                let index = next.fetch_add(1, Ordering::Relaxed);
                let Some(item) = items.get(index) else {
                    break;
                };
                match step(item) {
                    Ok(result) => results.lock().unwrap()[index] = Some(result),
                    Err(error) => errors.lock().unwrap().push(error),
                }
            });
        }
    });
    let errors = errors.into_inner().unwrap();
    if !errors.is_empty() {
        return Err(errors.join("\n"));
    }
    Ok(results
        .into_inner()
        .unwrap()
        .into_iter()
        .map(|result| result.expect("every item ran"))
        .collect())
}

/// Finishes the image objcopy wrote: the cartridge header fix-up pret's
/// Makefiles run right after objcopy (`gbafix ROM -p --silent`), the
/// Nintendo logo and the complement check, then zero padding to the
/// cartridge's size.
fn finish_image(image: &Path) -> Result<(), String> {
    let mut bytes = fs::read(image).map_err(|error| format!("{}: {error}", image.display()))?;
    crate::gbafix::fix(&mut bytes, &crate::gbafix::Fields::default(), false);
    bytes.resize(cartridge_size(bytes.len()), 0);
    fs::write(image, bytes).map_err(|error| format!("{}: {error}", image.display()))
}

fn cartridge_size(length: usize) -> usize {
    length.next_power_of_two()
}

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

fn link_command(
    root: &Path,
    build: Build,
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
    if let Some(modern) = build.modern {
        arguments.insert(1, "--orphan-handling=warn".into());
        arguments.extend(modern.libraries.iter().map(|path| path.to_string_lossy().into_owned()));
    } else if text.contains("libgcc.a:") {
        arguments.push(
            root.join("tools/out/compiler-runtime/libgcc.a")
                .to_string_lossy()
                .into_owned(),
        );
    }
    arguments
}

fn run_link(build: Build, arguments: &[String], output: &Path, name: &str) -> Result<(), String> {
    let root = crate::compiler::routing::root();
    if build.modern.is_none() {
        return command(arguments, root).map(drop);
    }
    let warnings = crate::process::run_keeping_stderr(arguments, root)?;
    let path = output.join(format!("{name}.link-warnings"));
    fs::write(&path, warnings).map_err(|error| format!("{}: {error}", path.display()))
}

#[allow(clippy::too_many_arguments)]
fn symbols_pass(
    root: &Path,
    build: Build,
    script: &Path,
    text: &str,
    output: &Path,
    name: &str,
    sources: &[PathBuf],
    streamed: &[PathBuf],
    index: &Index,
) -> Result<PathBuf, String> {
    let pass = output.join("symbols");
    fs::create_dir_all(&pass).map_err(|error| error.to_string())?;
    let objects: Vec<PathBuf> = sources
        .iter()
        .filter(|source| !streamed.contains(source))
        .map(|source| output.join("obj").join(source).with_extension("o"))
        .collect();
    let elf = pass.join(format!("{name}.elf"));
    let mut arguments = link_command(root, build, script, text, &objects, &elf);
    arguments.insert(1, "--unresolved-symbols=ignore-all".to_owned());
    let key = link_key(root, build, index, &arguments, text, &objects)?;
    if index.fresh(&[elf.clone()], &key) {
        return Ok(elf);
    }
    command(&arguments, root)?;
    let elf_text = elf.to_string_lossy();
    command(
        &[
            "arm-none-eabi-objcopy",
            "--extract-symbol",
            &elf_text,
            &elf_text,
        ],
        root,
    )?;
    index.record(&[elf.clone()], &key, Vec::new())?;
    Ok(elf)
}

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

struct EntryHeader {
    path: &'static str,
    prefix: &'static str,
    region: &'static str,
    refused: &'static str,
    offsets: bool,
}

const IWRAM_CALLS: EntryHeader = EntryHeader {
    path: "INCLUDE/IWRAM_CALL.H",
    prefix: "Iwram_",
    region: "0?3[0-9a-fA-F]{6}",
    refused: "0?3[0-9a-fA-F]{6}",
    offsets: true,
};

const RAM_BUFFERS: EntryHeader = EntryHeader {
    path: "INCLUDE/RAM_BUFFER.H",
    prefix: "Ram_",
    region: "0?[23][0-9a-fA-F]{6}",
    refused: "0?[0-9a-fA-F]{7}",
    offsets: true,
};

const CARTRIDGE_BOOT: EntryHeader = EntryHeader {
    path: "INCLUDE/CARTBOOT.H",
    prefix: "Cart_",
    region: "0?8000000",
    refused: "0?[0-9a-fA-F]{7}",
    offsets: false,
};

fn checked_entries(root: &Path, target: DecompTarget, elf: &Path) -> Result<(), String> {
    use object::{Object, ObjectSymbol};

    let mut headers = Vec::new();
    for kind in [&IWRAM_CALLS, &RAM_BUFFERS, &CARTRIDGE_BOOT] {
        let header = Path::new(target.game_dir()).join(kind.path);
        let Ok(text) = fs::read_to_string(root.join(&header)) else {
            continue;
        };
        let entries = entry_list(kind, &text, &[target.edition_define])
            .map_err(|error| format!("{}: {error}", header.display()))?;
        if !entries.is_empty() {
            if kind.path == CARTRIDGE_BOOT.path {
                let bytes = fs::read(elf).map_err(|error| format!("{}: {error}", elf.display()))?;
                let file = object::File::parse(bytes.as_slice())
                    .map_err(|error| format!("{}: {error}", elf.display()))?;
                entries_agree(&entries, |name| {
                    file.symbols()
                        .find(|symbol| symbol.is_definition() && symbol.name() == Ok(name))
                        .and_then(|symbol| u32::try_from(symbol.address()).ok())
                })
                .map_err(|error| format!("{}: {error}", header.display()))?;
            } else {
                headers.push((header, entries));
            }
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

#[derive(Debug, PartialEq)]
struct CheckedEntry {
    name: String,
    from: Option<String>,
    value: u32,
}

fn entry_list(
    kind: &EntryHeader,
    text: &str,
    defined: &[&str],
) -> Result<Vec<CheckedEntry>, String> {
    let (prefix, region, refused) = (kind.prefix, kind.region, kind.refused);
    let entry = regex::Regex::new(&format!(
        r"^#define\s+{prefix}\w+\s+FIXED_ADDRESS\(\(.*\),\s*0x({region}),\s*(\w+)\)\s*$"
    ))
    .expect("static pattern");
    let offset = regex::Regex::new(&format!(
        r"^#define\s+{prefix}\w+Offset\s+FIXED_OFFSET\(0x([0-9a-fA-F]+),\s*(\w+),\s*(\w+)\)\s*$"
    ))
    .expect("static pattern");
    let address = regex::Regex::new(&format!("0x{refused}")).expect("static pattern");
    let offset_name =
        regex::Regex::new(&format!(r"^#define\s+{prefix}\w+Offset\b")).expect("static pattern");
    let mut entries = Vec::new();
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
                    if !kind.offsets {
                        return Err(format!("offset entries are not admitted: {line}"));
                    }
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

fn compile_all(
    root: &Path,
    build: Build,
    sources: &[PathBuf],
    output: &Path,
    index: &Index,
) -> Result<Vec<PathBuf>, String> {
    let target = build.target;
    let objects: Vec<PathBuf> = sources
        .iter()
        .map(|source| output.join("obj").join(source).with_extension("o"))
        .collect();
    let pairs: Vec<(&PathBuf, &PathBuf)> = sources.iter().zip(&objects).collect();
    let plans = parallel(&pairs, |(source, object)| {
        plan(root, build, source, object, index)
            .map_err(|error| format!("{}: {error}", source.display()))
    })?;
    let stale: Vec<Plan> = plans.into_iter().flatten().collect();
    let mut assets = Vec::new();
    for plan in &stale {
        for asset in source_assets(root, target, &plan.source)? {
            if !assets
                .iter()
                .any(|known: &Asset| known.built == asset.built)
            {
                assets.push(asset);
            }
        }
    }
    parallel(&assets, |asset| materialize(root, output, index, asset))?;
    parallel(&stale, |plan| {
        run_plan(root, plan, index).map_err(|error| format!("{}: {error}", plan.source.display()))
    })?;
    Ok(objects)
}

struct Plan {
    source: PathBuf,
    object: PathBuf,
    key: String,
    steps: Vec<Vec<String>>,
    assembly: Option<(PathBuf, String)>,
    scratch: Vec<PathBuf>,
    warnings: Option<PathBuf>,
}

fn plan(
    root: &Path,
    build: Build,
    source: &Path,
    object: &Path,
    index: &Index,
) -> Result<Option<Plan>, String> {
    let target = build.target;
    fs::create_dir_all(object.parent().expect("object directory"))
        .map_err(|error| error.to_string())?;
    let source_text = source.to_string_lossy().into_owned();
    let object_text = object.to_string_lossy().into_owned();
    let extension = source.extension().and_then(|extension| extension.to_str());
    let mut hasher = Sha256::new();
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    let mut assembly = None;
    let mut scratch = Vec::new();
    let mut warnings = None;
    let steps = if extension == Some("MID") {
        let midi = fs::read(root.join(source)).map_err(|error| error.to_string())?;
        let text = ags::sound::sequence_assembly(&midi)?;
        let path = object.with_extension("s");
        hasher.update(text.as_bytes());
        let step = assembly_command(&path.to_string_lossy(), &object_text);
        scratch.push(path.clone());
        assembly = Some((path, text));
        vec![step]
    } else if matches!(extension, Some("C" | "c")) {
        let preprocessed = object.with_extension("i");
        // Nintendo's library C keeps its own compiler in the modern build too.
        let modern = build.modern.filter(|_| crate::modern::compiles(target, source));
        let steps = match modern {
            Some(modern) => {
                hasher.update(modern.signature.as_bytes());
                warnings = Some(object.with_extension("warnings"));
                modern.c_steps(target, source, object)
            }
            None => c_steps(target, source, object)?,
        };
        let preprocess = preprocessor_only(&steps, &source_text, &preprocessed)?;
        command(&preprocess, root)?;
        hasher.update(portable(
            root,
            &fs::read(&preprocessed).map_err(|error| error.to_string())?,
        ));
        scratch.extend([preprocessed, object.with_extension("s")]);
        steps
    } else {
        let base = object
            .ancestors()
            .find(|path| path.join("baserom.gba").exists())
            .ok_or("the base ROM link is missing")?;
        assembly_inputs(&mut hasher, root, target, source, base, index)?;
        vec![target_assembly_command(
            target,
            &source_text,
            &object_text,
            base,
        )]
    };
    for step in &steps {
        hasher.update(portable(root, step.join("\0").as_bytes()));
    }
    let key = format!("{:x}", hasher.finalize());
    let mut products = vec![object.to_path_buf()];
    products.extend(warnings.clone());
    if index.fresh(&products, &key) {
        remove_scratch(&scratch);
        return Ok(None);
    }
    Ok(Some(Plan {
        source: source.to_path_buf(),
        object: object.to_path_buf(),
        key,
        steps,
        assembly,
        scratch,
        warnings,
    }))
}

fn run_plan(root: &Path, plan: &Plan, index: &Index) -> Result<(), String> {
    index.forget(&plan.object);
    if let Some((path, text)) = &plan.assembly {
        fs::write(path, text).map_err(|error| error.to_string())?;
    }
    let ran = plan.steps.iter().try_for_each(|step| match &plan.warnings {
        // A modern compile keeps its warnings beside its object.
        Some(path) => crate::process::run_keeping_stderr(step, root).and_then(|text| {
            fs::write(path, text).map_err(|error| format!("{}: {error}", path.display()))
        }),
        None => command(step, root).map(drop),
    });
    remove_scratch(&plan.scratch);
    ran?;
    let mut products = vec![plan.object.clone()];
    products.extend(plan.warnings.clone());
    index.record(&products, &plan.key, Vec::new())
}

fn remove_scratch(paths: &[PathBuf]) {
    for path in paths {
        let _ = fs::remove_file(path);
    }
}

pub(crate) fn c_steps(target: DecompTarget, source: &Path, object: &Path) -> Result<Vec<Vec<String>>, String> {
    c_steps_routed(target, source, object, None)
}

/// c_steps, optionally through a named compiler family instead of the source's
/// own route; diff uses this for its analysis-only studies.
pub(crate) fn c_steps_routed(
    target: DecompTarget,
    source: &Path,
    object: &Path,
    family: Option<crate::compiler::routing::CompilerFamily>,
) -> Result<Vec<Vec<String>>, String> {
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
    options.family = family;
    let mut steps = source_to_assembly_plan(&options)?;
    steps.push(compiler_assembly_command(
        &assembly.to_string_lossy(),
        &object.to_string_lossy(),
        family.map_or_else(|| is_arm(target.compiler, &source_text), |family| family.is_arm()),
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
    hasher: &mut Sha256,
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
    index: &Index,
) -> Result<(), String> {
    let directory = root.join(output);
    hasher.update(with_includes(&[root, &directory], &root.join(source))?);
    for input in stream_paths(root, target, source, &directory)? {
        hasher.update(fs::read(&input).map_err(|error| format!("{}: {error}", input.display()))?);
    }
    for built in asset_files(root, source) {
        let row = index
            .row(&directory.join(&built))
            .ok_or_else(|| format!("{built} was not prepared"))?;
        hasher.update(built.as_bytes());
        hasher.update(row.key.as_bytes());
    }
    Ok(())
}

fn asset_files(root: &Path, source: &Path) -> Vec<String> {
    let Ok(text) = fs::read_to_string(root.join(source)) else {
        return Vec::new();
    };
    asset_names(&text)
}

/// The directories of built assets in an edition's output: one per SRC
/// module (GRAPHICS, MAP, BATTLE ...), SOUND and COMMON, named in upper case
/// as the modules are, beside the build's own lower-case folders.
fn asset_directories(output: &Path) -> Vec<PathBuf> {
    let Ok(entries) = fs::read_dir(output) else {
        return Vec::new();
    };
    entries
        .filter_map(Result::ok)
        .filter(|entry| entry.file_type().is_ok_and(|kind| kind.is_dir()))
        .filter(|entry| asset_root(&entry.file_name().to_string_lossy()))
        .map(|entry| entry.path())
        .collect()
}

fn asset_root(name: &str) -> bool {
    name.bytes().next().is_some_and(|byte| byte.is_ascii_uppercase())
        && name
            .bytes()
            .all(|byte| byte.is_ascii_uppercase() || byte.is_ascii_digit() || byte == b'_')
}

/// The build products a source names: sound samples first, then every
/// `.incbin` or `.include` under GRAPHICS/ or MAP/ and every recipe in any
/// other module of SRC (BATTLE, GAME, MENU ...), optionally under COMMON/.
/// Paths that start in lower case (games/..., text/..., baserom.gba) and
/// other modules' names that are not a recipe (BATTLE/X.INC) are not assets.
fn asset_names(text: &str) -> Vec<String> {
    static PATTERN: std::sync::OnceLock<regex::Regex> = std::sync::OnceLock::new();
    let pattern = PATTERN.get_or_init(|| {
        regex::Regex::new(
            r#"(?m)^\s*\.(?:incbin\s+"((?:COMMON/)?SOUND/[A-Za-z0-9_./]+\.bin)"|(?:incbin|include)\s+"((?:COMMON/)?[A-Z][A-Z0-9_]*/[A-Za-z0-9_./]+)")"#,
        )
        .expect("static pattern")
    });
    let (mut sound, mut built) = (Vec::new(), Vec::new());
    for capture in pattern.captures_iter(text) {
        if let Some(path) = capture.get(1) {
            sound.push(path.as_str().to_owned());
            continue;
        }
        let path = &capture[2];
        let module = path.strip_prefix("COMMON/").unwrap_or(path);
        let graphics = module.starts_with("GRAPHICS/") || module.starts_with("MAP/");
        if graphics || (!module.starts_with("SOUND/") && ags::resource::is_recipe(path)) {
            built.push(path.to_owned());
        }
    }
    sound.extend(built);
    sound
}

struct Asset {
    built: String,
    game: PathBuf,
    recipe: String,
    sound: bool,
}

fn source_assets(root: &Path, target: DecompTarget, source: &Path) -> Result<Vec<Asset>, String> {
    let mut assets = Vec::new();
    for built in asset_files(root, source) {
        let sound = built
            .strip_prefix("COMMON/")
            .unwrap_or(&built)
            .starts_with("SOUND/");
        let recipe = if sound {
            built
                .strip_suffix(".bin")
                .expect("sound recipe ends in .bin")
        } else {
            &built
        };
        if recipe
            .split('/')
            .any(|part| part.is_empty() || part == "..")
        {
            let kind = if sound { "sound" } else { "resource" };
            return Err(format!(
                "{}: {built} is not a {kind} file path",
                source.display()
            ));
        }
        let (game, recipe) = match recipe.strip_prefix("COMMON/") {
            Some(shared) => (Path::new("games/COMMON"), shared),
            None if !sound && source.starts_with("games/COMMON") => {
                (Path::new("games/COMMON"), recipe)
            }
            None => (Path::new(target.game_dir()), recipe),
        };
        assets.push(Asset {
            game: game.to_path_buf(),
            recipe: recipe.to_owned(),
            built,
            sound,
        });
    }
    Ok(assets)
}

fn asset_input(root: &Path, asset: &Asset) -> Result<PathBuf, String> {
    if !asset.sound {
        return Ok(asset
            .game
            .join("SRC")
            .join(ags::resource::input_name(&asset.recipe)?));
    }
    let inputs: Vec<PathBuf> = ["WAV", "PCM4"]
        .iter()
        .map(|extension| asset.game.join(format!("{}.{extension}", asset.recipe)))
        .filter(|path| root.join(path).is_file())
        .collect();
    match inputs.as_slice() {
        [input] => Ok(input.clone()),
        _ => Err(format!(
            "{} needs exactly one WAV or PCM4 input",
            asset.built
        )),
    }
}

fn encode(root: &Path, asset: &Asset) -> Result<(Vec<u8>, Vec<String>), String> {
    let input = asset_input(root, asset)?;
    let bytes = fs::read(root.join(&input))
        .map_err(|error| format!("{}: {}: {error}", asset.built, input.display()))?;
    if asset.sound {
        let encoded = ags::sound::build_sound_file(&input.to_string_lossy(), &bytes)
            .map_err(|error| format!("{}: {error}", input.display()))?;
        return Ok((encoded, Vec::new()));
    }
    let read = std::cell::RefCell::new(Vec::new());
    let encoded = ags::resource::build_file_with(&asset.recipe, &bytes, &|name| {
        let sibling = ags::resource::sibling_path(&input, name)?;
        read.borrow_mut()
            .push(sibling.to_string_lossy().into_owned());
        let path = root.join(&sibling);
        fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
    })?;
    Ok((encoded, read.into_inner()))
}

fn asset_key(root: &Path, asset: &Asset, inputs: &[String]) -> Option<String> {
    let mut hasher = Sha256::new();
    hasher.update(env!("ALCHEMY_ENCODER").as_bytes());
    hasher.update(asset.built.as_bytes());
    let input = asset_input(root, asset).ok()?;
    hasher.update(fs::read(root.join(&input)).ok()?);
    for path in inputs {
        hasher.update((path.len() as u64).to_le_bytes());
        hasher.update(path.as_bytes());
        // A sibling the recipe looked for and did not find, such as an
        // optional patch file, keys the asset by its absence.
        match fs::read(root.join(path)) {
            Ok(bytes) => {
                hasher.update((bytes.len() as u64).to_le_bytes());
                hasher.update(bytes);
            }
            Err(_) => hasher.update(u64::MAX.to_le_bytes()),
        }
    }
    Some(format!("{:x}", hasher.finalize()))
}

fn prepare_assets(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
    output: &Path,
    index: &Index,
) -> Result<(), String> {
    let mut assets: Vec<Asset> = Vec::new();
    for source in sources {
        for asset in source_assets(root, target, source)? {
            if !assets.iter().any(|known| known.built == asset.built) {
                assets.push(asset);
            }
        }
    }
    parallel(&assets, |asset| {
        let path = output.join(&asset.built);
        let known = index.row(&path).is_some_and(|row| {
            asset_key(root, asset, &row.inputs).is_some_and(|key| key == row.key)
        });
        if known {
            Ok(())
        } else {
            write_asset(root, output, index, asset)
        }
    })?;
    Ok(())
}

fn materialize(root: &Path, output: &Path, index: &Index, asset: &Asset) -> Result<(), String> {
    let path = output.join(&asset.built);
    match index.row(&path) {
        Some(row) if index.fresh(&[path], &row.key) => Ok(()),
        _ => write_asset(root, output, index, asset),
    }
}

fn write_asset(root: &Path, output: &Path, index: &Index, asset: &Asset) -> Result<(), String> {
    let (encoded, inputs) = encode(root, asset)?;
    let path = output.join(&asset.built);
    fs::create_dir_all(path.parent().expect("asset file directory"))
        .map_err(|error| error.to_string())?;
    fs::write(&path, encoded).map_err(|error| error.to_string())?;
    let key = asset_key(root, asset, &inputs)
        .ok_or_else(|| format!("{}: an input changed while it was encoded", asset.built))?;
    index.record(&[path], &key, inputs)
}

const OVERLAY_MACHINE: LzMachine = ags::resource::PACKER;

pub(crate) fn stream_ids(
    root: &Path,
    target: DecompTarget,
    source: &Path,
) -> Result<Vec<String>, String> {
    stream_ids_in(root, target, source, &root.join(target.output_dir))
}

/// stream_ids, reading generated includes from the build directory `output`.
fn stream_ids_in(
    root: &Path,
    target: DecompTarget,
    source: &Path,
    output: &Path,
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
    let selected = crate::compiler::assembly_source::selected_includes(
        &text,
        target.edition_define,
        &[root, output],
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
    Ok(stream_ids_in(root, target, source, output)?
        .iter()
        .map(|id| output.join("overlays").join(format!("resource_{id}.lz")))
        .collect())
}

/// The overlay's own data source, when it has one: an overlay whose data
/// all lives in its modules links nothing of its own.
fn overlay_data(root: &Path, target: DecompTarget, id: &str) -> Option<PathBuf> {
    let path = target.overlay_data(id);
    root.join(&path).is_file().then_some(path)
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

#[allow(clippy::too_many_arguments)]
fn build_overlay_streams(
    root: &Path,
    build: Build,
    source: &Path,
    output: &Path,
    symbols: &Path,
    table: &str,
    index: &Index,
) -> Result<Vec<(PathBuf, PathBuf)>, String> {
    let target = build.target;
    let ids = &stream_ids_in(root, target, source, output)?;
    let directory = output.join("overlays");
    fs::create_dir_all(&directory).map_err(|error| error.to_string())?;
    let mut linked = Vec::with_capacity(ids.len());
    let mut sources = Vec::new();
    for id in ids {
        let script = target.overlay_script(id);
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
        linked.push((id, script, objects));
    }
    let mut asset_sources = sources.clone();
    asset_sources.extend(ids.iter().filter_map(|id| overlay_data(root, target, id)));
    prepare_assets(root, target, &asset_sources, output, index)?;
    compile_all(root, build, &sources, output, index)?;
    parallel(&linked, |(id, script, objects)| {
        let overlay = Overlay {
            root,
            build,
            output,
            id,
            script,
            objects,
            symbols,
            table,
            directory: &directory,
        };
        build_overlay(&overlay, index).map_err(|error| format!("resource_{id}: {error}"))
    })?;
    let mut objects: Vec<(PathBuf, PathBuf)> = sources
        .into_iter()
        .map(|source| {
            let object = output.join("obj").join(&source).with_extension("o");
            (source, object)
        })
        .collect();
    objects.extend(ids.iter().filter_map(|id| {
        overlay_data(root, target, id)
            .map(|data| (data, directory.join(format!("resource_{id}_overlay.o"))))
    }));
    Ok(objects)
}

struct Overlay<'a> {
    root: &'a Path,
    build: Build<'a>,
    output: &'a Path,
    id: &'a str,
    script: &'a Path,
    objects: &'a [PathBuf],
    symbols: &'a Path,
    table: &'a str,
    directory: &'a Path,
}

fn build_overlay(overlay: &Overlay, index: &Index) -> Result<(), String> {
    let Overlay {
        root,
        build,
        output,
        id,
        script,
        objects,
        symbols,
        table,
        directory,
    } = *overlay;
    let target = build.target;
    let modern_script;
    let script = match build.modern {
        Some(_) => {
            modern_script = crate::modern::overlay_script(root, script, directory, id)?;
            modern_script.as_path()
        }
        None => script,
    };
    let libraries = match build.modern {
        Some(modern) => modern.libraries.clone(),
        None => vec![PathBuf::from(LIBGCC)],
    };
    let listing = overlay_data(root, target, id);
    let path = |name: String| directory.join(name).to_string_lossy().into_owned();
    let object = path(format!("resource_{id}_overlay.o"));
    let elf = path(format!("resource_{id}.elf"));
    let map = path(format!("resource_{id}.map"));
    let image = path(format!("resource_{id}.bin"));
    let stream = directory.join(format!("resource_{id}.lz"));
    let own_symbols = path(format!("resource_{id}.symbols.elf"));
    let names = path(format!("resource_{id}.own"));
    let mut link: Vec<String> = [
        "arm-none-eabi-ld",
        "--no-warn-mismatch",
        "-n",
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
    link.extend(libraries.iter().map(|path| path.to_string_lossy().into_owned()));
    let empty = Path::new("/dev/null");
    let assemble = target_assembly_command(
        target,
        &listing.as_deref().unwrap_or(empty).to_string_lossy(),
        &object,
        output,
    );
    let mut hasher = Sha256::new();
    overlay_inputs(&mut hasher, root, output, listing.as_deref(), script, objects, index)?;
    for step in [&assemble, &link] {
        hasher.update(portable(root, step.join("\0").as_bytes()));
    }
    for library in &libraries {
        hasher.update(
            fs::read(root.join(library))
                .map_err(|error| format!("{}: {error}", library.display()))?,
        );
    }
    if let Some(modern) = build.modern {
        hasher.update(modern.signature.as_bytes());
    }
    hasher.update(table.as_bytes());
    hasher.update(format!("{OVERLAY_MACHINE:?}").as_bytes());
    hasher.update(crate::compiler::bundle::toolchain_signature().as_bytes());
    hasher.update(env!("ALCHEMY_ENCODER").as_bytes());
    hasher.update(env!("ALCHEMY_LINK_RULE").as_bytes());
    let key = format!("{:x}", hasher.finalize());
    let products = overlay_products(directory, id);
    if index.fresh(&products, &key) {
        return overlay_gate(&elf, &map, build.modern.is_some());
    }
    index.forget(&stream);
    if let Some(listing) = &listing {
        for asset in source_assets(root, target, listing)? {
            materialize(root, output, index, &asset)?;
        }
    }
    command(&assemble, root)?;
    let mut own = BTreeSet::new();
    for path in std::iter::once(PathBuf::from(&object))
        .chain(objects.iter().cloned())
        .chain(libraries.iter().cloned())
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
        own.extend(
            table
                .lines()
                .filter_map(|line| line.split_whitespace().nth(2))
                .map(str::to_owned),
        );
    }
    let list = |names: &mut dyn Iterator<Item = &String>| {
        names.fold(String::new(), |mut text, name| {
            text.push_str(name);
            text.push('\n');
            text
        })
    };
    fs::write(&names, list(&mut own.iter())).map_err(|error| error.to_string())?;
    command(
        &[
            "arm-none-eabi-objcopy",
            "--extract-symbol",
            &format!("--strip-symbols={names}"),
            &symbols.to_string_lossy(),
            &own_symbols,
        ],
        root,
    )?;
    let linked = command(&link, root);
    let _ = fs::remove_file(&own_symbols);
    linked?;
    let imported = imported_symbols(&elf, &own)?;
    fs::write(&names, list(&mut imported.iter())).map_err(|error| error.to_string())?;
    let stripped = command(
        &[
            "arm-none-eabi-objcopy",
            &format!("--strip-symbols={names}"),
            &elf,
        ],
        root,
    );
    let _ = fs::remove_file(&names);
    stripped?;
    overlay_gate(&elf, &map, build.modern.is_some())?;
    command(
        &["arm-none-eabi-objcopy", "-O", "binary", &elf, &image],
        root,
    )?;
    let decoded = fs::read(&image).map_err(|error| error.to_string());
    let _ = fs::remove_file(&image);
    let mut decoded = decoded?;
    store_thumb_calls(&mut decoded);
    let encoded = compress_tagged(&decoded, &OVERLAY_MACHINE)?;
    fs::write(&stream, encoded).map_err(|error| error.to_string())?;
    index.record(&products, &key, Vec::new())
}

fn imported_symbols(elf: &str, own: &BTreeSet<String>) -> Result<BTreeSet<String>, String> {
    use object::{Object, ObjectSymbol, SymbolSection};
    let bytes = fs::read(elf).map_err(|error| format!("{elf}: {error}"))?;
    let file = object::File::parse(&*bytes).map_err(|error| format!("{elf}: {error}"))?;
    let mut absolute = BTreeSet::new();
    let mut placed = BTreeSet::new();
    for symbol in file.symbols() {
        let Ok(name) = symbol.name() else { continue };
        if symbol.section() == SymbolSection::Absolute && symbol.is_global() {
            absolute.insert(name.to_owned());
        } else {
            placed.insert(name.to_owned());
        }
    }
    Ok(absolute
        .into_iter()
        .filter(|name| !own.contains(name) && !placed.contains(name))
        .collect())
}

pub(crate) fn overlay_products(directory: &Path, id: &str) -> [PathBuf; 4] {
    [
        format!("resource_{id}.lz"),
        format!("resource_{id}_overlay.o"),
        format!("resource_{id}.elf"),
        format!("resource_{id}.map"),
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

fn overlay_inputs(
    hasher: &mut Sha256,
    root: &Path,
    output: &Path,
    listing: Option<&Path>,
    script: &Path,
    objects: &[PathBuf],
    index: &Index,
) -> Result<(), String> {
    let Some(listing) = listing else {
        hasher.update(b"no overlay data");
        return overlay_script_inputs(hasher, root, script, objects, index);
    };
    hasher.update(with_includes(&[root, output], &root.join(listing))?);
    for built in asset_files(root, listing) {
        let row = index
            .row(&output.join(&built))
            .ok_or_else(|| format!("{built} was not prepared"))?;
        hasher.update(built.as_bytes());
        hasher.update(row.key.as_bytes());
    }
    overlay_script_inputs(hasher, root, script, objects, index)
}

fn overlay_script_inputs(
    hasher: &mut Sha256,
    root: &Path,
    script: &Path,
    objects: &[PathBuf],
    index: &Index,
) -> Result<(), String> {
    hasher.update(
        fs::read(root.join(script)).map_err(|error| format!("{}: {error}", script.display()))?,
    );
    for object in objects {
        let row = index
            .row(object)
            .ok_or_else(|| format!("{} was not built", object.display()))?;
        hasher.update(row.digest.as_bytes());
    }
    Ok(())
}

fn overlay_gate(elf: &str, map: &str, modern: bool) -> Result<(), String> {
    let image = fs::read(elf).map_err(|error| format!("{elf}: {error}"))?;
    if modern {
        return crate::gate::overlay::check_modern(&image);
    }
    let map = fs::read_to_string(map).map_err(|error| format!("{map}: {error}"))?;
    crate::gate::overlay::check(&image, &map)
}

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
            "-S" | "-c" => {}
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

fn with_includes(directories: &[&Path], source: &Path) -> Result<Vec<u8>, String> {
    let text = fs::read(source).map_err(|error| format!("{}: {error}", source.display()))?;
    let mut key = text.clone();
    for line in String::from_utf8_lossy(&text).lines() {
        let Some(rest) = line.trim().strip_prefix(".include") else {
            continue;
        };
        let include = rest.trim().trim_matches('"');
        if ["GRAPHICS/", "MAP/", "SOUND/", "COMMON/"]
            .iter()
            .any(|prefix| include.starts_with(prefix))
        {
            continue;
        }
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
    fn built_asset_directories_are_the_upper_case_ones() {
        for name in ["GRAPHICS", "MAP", "SOUND", "COMMON", "BATTLE", "MENU_2"] {
            assert!(asset_root(name), "{name}");
        }
        for name in ["obj", "overlays", "text", "symbols", "tbs-en.gba", "Map", "_X", ""] {
            assert!(!asset_root(name), "{name}");
        }
    }

    #[test]
    fn assets_live_in_any_module() {
        let source = "\t.include \"games/COMMON/INCLUDE/GAME/ED_ASM.H\"\n\
            \t.incbin \"BATTLE/ITEM/ITEMS.table\"\n\
            \t.incbin \"GAME/CLASS/CLASSES.table.lz\"\n\
            \t.include \"text/messages.inc\"\n\
            \t.include \"BATTLE/ITEM/ITEM.INC\"\n\
            \t.incbin \"baserom.gba\", 0x100, 0x10\n\
            \t.incbin \"COMMON/MENU/ICONS/ARROW.4bpp\"\n\
            \t.incbin \"MAP/MAP_095/SHAPES.table.lz\"\n\
            \t.incbin \"SOUND/SAMPLE/DRUM.PCM8.bin\"\n\
            \t.incbin \"SOUND/X.table\"\n";
        assert_eq!(
            asset_names(source),
            [
                "SOUND/SAMPLE/DRUM.PCM8.bin",
                "BATTLE/ITEM/ITEMS.table",
                "GAME/CLASS/CLASSES.table.lz",
                "COMMON/MENU/ICONS/ARROW.4bpp",
                "MAP/MAP_095/SHAPES.table.lz",
            ]
        );
    }
}
