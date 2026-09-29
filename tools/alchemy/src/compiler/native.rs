//! Compile unchanged source modules and link the explicit object list.
use super::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use super::routing::{assembly_command, compiler_assembly_command};
use crate::targets::{decomp_target, DecompTarget};
use psynergy::process::run as command;
use std::collections::BTreeSet;
use std::fs::{self, File};
use std::io::Write;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy build native --script FILE [--target GAME-EDITION] [--image main|resource_HEX] [--archive FILE] [--output DIR] SOURCE...\nCompile unchanged whole source files with the approved toolchain and link the ordered objects.\nArchives must be rebuilt under tools/out/compiler-runtime from approved compiler source.";

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Section {
    pub name: String,
    pub size: u64,
    pub address: u64,
    pub load_address: u64,
}

#[derive(Debug, serde::Serialize, serde::Deserialize)]
pub struct Build {
    pub image: String,
    pub sources: Vec<PathBuf>,
    pub script: PathBuf,
    pub objects: Vec<PathBuf>,
    pub archives: Vec<PathBuf>,
    pub elf: PathBuf,
    pub binary: PathBuf,
    pub symbols: PathBuf,
    pub map: PathBuf,
    pub log: PathBuf,
    pub sections: Vec<Section>,
    pub functions: Vec<Function>,
}

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Function {
    pub name: String,
    pub section: String,
    pub source: PathBuf,
    pub object: PathBuf,
    pub address: u64,
    pub load_address: u64,
    /// The complete emitted section, including the pool after `.size`.
    pub size: u64,
    /// The complete linked output group checked before any member is credited.
    pub group_size: u64,
}

/// A current object section placed by the ordinary linker script.
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct Input {
    pub section: String,
    pub input_section: String,
    pub source: PathBuf,
    pub object: PathBuf,
    pub address: u64,
    pub load_address: u64,
    pub size: u64,
    pub has_instructions: bool,
}

struct Options {
    target: DecompTarget,
    sources: Vec<PathBuf>,
    script: PathBuf,
    output: PathBuf,
    image: String,
    archives: Vec<PathBuf>,
}

fn options(args: &[String]) -> Result<Options, String> {
    let mut target = decomp_target(None)?;
    let mut script = None;
    let mut output = None;
    let mut sources = Vec::new();
    let mut image = "main".to_owned();
    let mut archives = Vec::new();
    let mut args = args.iter();
    while let Some(arg) = args.next() {
        match arg.as_str() {
            "--target" => {
                target = decomp_target(Some(args.next().ok_or("--target needs a value")?))?
            }
            "--script" => {
                script = Some(PathBuf::from(args.next().ok_or("--script needs a value")?))
            }
            "--output" => {
                output = Some(PathBuf::from(args.next().ok_or("--output needs a value")?))
            }
            "--image" => image = args.next().ok_or("--image needs a value")?.clone(),
            "--archive" => {
                archives.push(PathBuf::from(args.next().ok_or("--archive needs a value")?))
            }
            "--" => {
                sources.extend(args.map(PathBuf::from));
                break;
            }
            flag if flag.starts_with('-') => {
                return Err(format!("unknown native build option: {flag}"))
            }
            source => sources.push(PathBuf::from(source)),
        }
    }
    if sources.is_empty() {
        return Err(format!("native build needs at least one source\n{USAGE}"));
    }
    if image != "main"
        && !image.strip_prefix("resource_").is_some_and(|suffix| {
            !suffix.is_empty() && suffix.bytes().all(|byte| byte.is_ascii_hexdigit())
        })
    {
        return Err("native image must be main or an explicit resource_HEX".into());
    }
    Ok(Options {
        target,
        sources,
        script: script.ok_or_else(|| format!("native build needs --script\n{USAGE}"))?,
        output: output.unwrap_or_else(|| Path::new(target.output_dir).join("native")),
        image,
        archives,
    })
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let options = options(args)?;
    let result = build_scoped(
        super::routing::root(),
        options.target,
        &options.sources,
        &options.script,
        &options.output,
        &options.image,
        &options.archives,
    )?;
    super::canonical_json::write_canonical(
        &options.output.join("native.json"),
        &serde_json::to_value(&result).map_err(|error| error.to_string())?,
    )?;
    for (name, path) in [
        ("elf", &result.elf),
        ("binary", &result.binary),
        ("symbols", &result.symbols),
        ("map", &result.map),
        ("log", &result.log),
    ] {
        println!("{name}={}", path.display());
    }
    println!("objects={}", result.objects.len());
    println!(
        "image={} functions={}",
        result.image,
        result.functions.len()
    );
    for section in &result.sections {
        println!(
            "section={} address=0x{:08x} load=0x{:08x} bytes={}",
            section.name, section.address, section.load_address, section.size
        );
    }
    Ok(())
}

fn rooted(root: &Path, path: &Path) -> PathBuf {
    if path.is_absolute() {
        path.to_path_buf()
    } else {
        root.join(path)
    }
}

fn input(root: &Path, path: &Path) -> Result<PathBuf, String> {
    let path = rooted(root, path);
    let canonical =
        fs::canonicalize(&path).map_err(|error| format!("{}: {error}", path.display()))?;
    if !canonical.is_file() {
        return Err(format!("{} is not a file", canonical.display()));
    }
    Ok(canonical)
}

pub(crate) fn maintained_sources(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
) -> Result<Vec<PathBuf>, String> {
    let root = fs::canonicalize(root).map_err(|error| error.to_string())?;
    let game = root.join(target.source_dir);
    let common = root.join("games/COMMON/SRC");
    let fallback = root.join(target.asm_dir);
    let mut seen = BTreeSet::new();
    let mut canonical = Vec::new();
    for source in sources {
        let source = input(&root, source)?;
        let assembly = matches!(
            source.extension().and_then(|suffix| suffix.to_str()),
            Some("s" | "S")
        );
        if (!source.starts_with(&game)
            && !source.starts_with(&common)
            && !(assembly && source.starts_with(&fallback)))
            || !matches!(
                source.extension().and_then(|suffix| suffix.to_str()),
                Some("c" | "C" | "s" | "S")
            )
            || !seen.insert(source.clone())
        {
            return Err("native inputs must be distinct maintained C or assembly sources for the selected game".into());
        }
        canonical.push(source);
    }
    if canonical.is_empty() {
        return Err("native build has no maintained source inputs".into());
    }
    Ok(canonical)
}

fn execute(args: &[String], root: &Path, log: &mut File) -> Result<String, String> {
    writeln!(log, "{}", args.join(" ")).map_err(|error| error.to_string())?;
    match command(args, root) {
        Ok(stdout) => {
            write!(log, "{stdout}").map_err(|error| error.to_string())?;
            Ok(stdout)
        }
        Err(error) => {
            let _ = writeln!(log, "{error}");
            Err(error)
        }
    }
}

fn build_scoped(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
    script: &Path,
    output: &Path,
    image: &str,
    archives: &[PathBuf],
) -> Result<Build, String> {
    let root = fs::canonicalize(root).map_err(|error| error.to_string())?;
    let script = input(&root, script)?;
    if !script.starts_with(root.join(target.game_dir())) {
        return Err("the linker script must be maintained inside the selected game".into());
    }
    let sources = sources
        .iter()
        .map(|source| input(&root, source))
        .collect::<Result<Vec<_>, _>>()?;
    if sources.is_empty() {
        return Err("native build needs at least one source".into());
    }
    let mut seen = BTreeSet::new();
    let mut objects = BTreeSet::new();
    for source in &sources {
        if !source.starts_with(&root) {
            return Err(format!(
                "{} is outside the source repository",
                source.display()
            ));
        }
        if !matches!(
            source.extension().and_then(|ext| ext.to_str()),
            Some("c" | "C" | "s" | "S")
        ) {
            return Err(format!("{} is not C or assembly source", source.display()));
        }
        if !seen.insert(source) {
            return Err(format!("{} is listed more than once", source.display()));
        }
        if !objects.insert(source.with_extension("o")) {
            return Err(format!(
                "{} shares an object path with another source",
                source.display()
            ));
        }
    }
    let sources = maintained_sources(&root, target, &sources)?;
    let archives = archives
        .iter()
        .map(|path| input(&root, path))
        .collect::<Result<Vec<_>, _>>()?;
    if archives.iter().any(|path| {
        !path.starts_with(root.join("tools/out/compiler-runtime"))
            || path.extension().and_then(|extension| extension.to_str()) != Some("a")
    }) {
        return Err(
            "native archives must be rebuilt under tools/out/compiler-runtime from approved source"
                .into(),
        );
    }
    let output = super::build_io::generated_directory(&root, output)?;
    let mut result = Build {
        image: image.into(),
        sources: sources.clone(),
        script: script.clone(),
        objects: Vec::new(),
        archives,
        elf: output.join("native.elf"),
        binary: output.join("native.bin"),
        symbols: output.join("native.nm"),
        map: output.join("native.map"),
        log: output.join("build.log"),
        sections: Vec::new(),
        functions: Vec::new(),
    };
    let metadata = output.join("native.json");
    for path in [
        &result.elf,
        &result.binary,
        &result.symbols,
        &result.map,
        &metadata,
    ] {
        match fs::remove_file(path) {
            Ok(()) => {}
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
            Err(error) => return Err(format!("{}: {error}", path.display())),
        }
    }
    let mut log = File::create(&result.log).map_err(|error| error.to_string())?;
    super::routing::prefer_installed_binutils();
    let build = (|| {
        for source in &sources {
            let relative = source
                .strip_prefix(&root)
                .expect("validated repository source");
            let object = output.join("obj").join(relative).with_extension("o");
            fs::create_dir_all(object.parent().expect("object directory"))
                .map_err(|error| error.to_string())?;
            match fs::remove_file(&object) {
                Ok(()) => {}
                Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
                Err(error) => return Err(error.to_string()),
            }
            let source_text = source.to_string_lossy();
            let object_text = object.to_string_lossy();
            if matches!(
                source.extension().and_then(|ext| ext.to_str()),
                Some("c" | "C")
            ) {
                let assembly = object.with_extension("s");
                let assembly_text = assembly.to_string_lossy();
                let mut options = SourceToAssemblyPlanOptions::new(
                    target.compiler,
                    relative.to_string_lossy(),
                    &*source_text,
                    &*assembly_text,
                );
                options.preprocessor_flags = vec![format!("-D{}=1", target.edition_define)];
                options.preprocessed_output =
                    Some(object.with_extension("i").to_string_lossy().into_owned());
                for step in source_to_assembly_plan(&options)? {
                    execute(&step, &root, &mut log)?;
                }
                execute(
                    &compiler_assembly_command(&assembly_text, &object_text),
                    &root,
                    &mut log,
                )?;
            } else {
                execute(
                    &assembly_command(&source_text, &object_text),
                    &root,
                    &mut log,
                )?;
            }
            result.objects.push(object);
        }
        let mut link = vec![
            "arm-none-eabi-ld".into(),
            "-T".into(),
            script.to_string_lossy().into_owned(),
            "-Map".into(),
            result.map.to_string_lossy().into_owned(),
            "-o".into(),
            result.elf.to_string_lossy().into_owned(),
        ];
        if !result.archives.is_empty() {
            // Approved legacy runtime objects use the established soft-FP ABI;
            // historical GAS flags differ from modern binutils ELF attributes.
            link.push("--no-warn-mismatch".into());
        }
        link.extend(
            result
                .objects
                .iter()
                .map(|path| path.to_string_lossy().into_owned()),
        );
        link.extend(
            result
                .archives
                .iter()
                .map(|path| path.to_string_lossy().into_owned()),
        );
        execute(&link, &root, &mut log)?;
        let symbols = execute(
            &[
                "arm-none-eabi-nm".into(),
                "-n".into(),
                "-S".into(),
                result.elf.to_string_lossy().into_owned(),
            ],
            &root,
            &mut log,
        )?;
        fs::write(&result.symbols, symbols).map_err(|error| error.to_string())?;
        let headers = execute(
            &[
                "arm-none-eabi-objdump".into(),
                "-h".into(),
                result.elf.to_string_lossy().into_owned(),
            ],
            &root,
            &mut log,
        )?;
        result.sections = loaded_sections(&headers)?;
        result.functions = inspect_functions(&root, &result)?;
        execute(
            &[
                "arm-none-eabi-objcopy".into(),
                "-O".into(),
                "binary".into(),
                result.elf.to_string_lossy().into_owned(),
                result.binary.to_string_lossy().into_owned(),
            ],
            &root,
            &mut log,
        )?;
        Ok::<_, String>(())
    })();
    if let Err(error) = build {
        let _ = writeln!(log, "{error}");
        return Err(format!(
            "{error}\nDiagnostics retained at {}",
            result.log.display()
        ));
    }
    Ok(result)
}

pub(crate) fn inspect_loaded_sections(root: &Path, elf: &Path) -> Result<Vec<Section>, String> {
    super::routing::prefer_installed_binutils();
    let headers = command(
        &[
            "arm-none-eabi-objdump".into(),
            "-h".into(),
            elf.to_string_lossy().into_owned(),
        ],
        root,
    )?;
    loaded_sections(&headers)
}

/// Read complete linked function sections and their input objects afresh.
pub(crate) fn inspect_functions(root: &Path, build: &Build) -> Result<Vec<Function>, String> {
    let sections = inspect_loaded_sections(root, &build.elf)?;
    let symbols = command(
        &[
            "arm-none-eabi-objdump".into(),
            "-t".into(),
            "--special-syms".into(),
            build.elf.to_string_lossy().into_owned(),
        ],
        root,
    )?;
    let map = fs::read_to_string(&build.map).map_err(|error| error.to_string())?;
    let objects = inspect_object_tables(root, build)?;
    linked_functions(root, build, &sections, &symbols, &map, &objects)
}

fn inspect_object_tables(root: &Path, build: &Build) -> Result<String, String> {
    let mut command_line = vec![
        "arm-none-eabi-objdump".into(),
        "-t".into(),
        "-h".into(),
        "--special-syms".into(),
    ];
    command_line.extend(
        build
            .objects
            .iter()
            .map(|path| path.to_string_lossy().into_owned()),
    );
    command(&command_line, root)
}

fn object_table<'a>(tables: &'a str, object: &Path) -> Result<&'a str, String> {
    let marker = format!("{}:     file format", object.display());
    tables
        .split_once(&marker)
        .map(|(_, table)| {
            table
                .split_once(":     file format")
                .map_or(table, |(table, _)| table)
        })
        .ok_or_else(|| format!("{} has no independent object table", object.display()))
}

fn input_source(root: &Path, build: &Build, object: &Path) -> Result<PathBuf, String> {
    let sources = build
        .sources
        .iter()
        .filter(|source| {
            source
                .strip_prefix(root)
                .ok()
                .is_some_and(|relative| object.ends_with(relative.with_extension("o")))
        })
        .collect::<Vec<_>>();
    match sources.as_slice() {
        [source] => Ok((*source).clone()),
        _ => Err(format!(
            "{} has no unique maintained source",
            object.display()
        )),
    }
}

fn linked_inputs(
    root: &Path,
    build: &Build,
    sections: &[Section],
    map: &str,
    object_tables: &str,
) -> Result<Vec<Input>, String> {
    let mut members = std::collections::BTreeMap::<(String, PathBuf, String), (u64, u64)>::new();
    let mut current = None;
    let mut input_name = None;
    for line in map.lines() {
        if !line.starts_with(char::is_whitespace) {
            current = line
                .split_whitespace()
                .next()
                .filter(|name| sections.iter().any(|section| section.name == **name))
                .map(str::to_owned);
            input_name = None;
        } else if let Some(name) = line
            .split_whitespace()
            .next()
            .filter(|name| name.starts_with('.'))
        {
            input_name = Some(name.to_owned());
        }
        if let (Some(section), Some(name)) = (&current, &input_name) {
            for object in &build.objects {
                if line.trim_end().ends_with(&*object.to_string_lossy()) {
                    let values = line
                        .split_whitespace()
                        .filter_map(|word| word.strip_prefix("0x"))
                        .take(2)
                        .map(|word| u64::from_str_radix(word, 16))
                        .collect::<Result<Vec<_>, _>>()
                        .map_err(|error| error.to_string())?;
                    if values.len() != 2 {
                        return Err(format!(
                            "{section}/{name} has incomplete placement in the linker map"
                        ));
                    }
                    if values[1] == 0 {
                        continue;
                    }
                    if members
                        .insert(
                            (section.clone(), object.clone(), name.clone()),
                            (values[0], values[1]),
                        )
                        .is_some()
                    {
                        return Err(format!(
                            "{section}/{name} has ambiguous placement in the linker map"
                        ));
                    }
                }
            }
        }
    }
    let mut result = Vec::new();
    for ((output, object, name), (address, size)) in members {
        let section = sections
            .iter()
            .find(|section| section.name == output)
            .expect("map output section was selected from loaded sections");
        let table = object_table(object_tables, &object)?;
        let input_sections = loaded_sections(table)?;
        let input = input_sections
            .iter()
            .find(|section| section.name == name)
            .ok_or_else(|| {
                format!(
                    "{name} has no complete input section in {}",
                    object.display()
                )
            })?;
        if input.size != size
            || address < section.address
            || address
                .checked_add(size)
                .is_none_or(|end| end > section.address + section.size)
        {
            return Err(format!(
                "{name}: linked input section changes the complete emitted extent"
            ));
        }
        let source = input_source(root, build, &object)?;
        let has_instructions = mapping_symbols(table, &name).any(|(start, kind)| {
            matches!(kind, 'a' | 't')
                && input.address <= start
                && start < input.address + input.size
        }) || (is_c_source(&source)
            && mapping_symbols(table, &name).next().is_none()
            && table.lines().any(|line| {
                let fields = line.split_whitespace().collect::<Vec<_>>();
                fields.contains(&"F") && fields.contains(&name.as_str())
            }));
        result.push(Input {
            section: output,
            input_section: name,
            source,
            object,
            address,
            load_address: section.load_address + address - section.address,
            size,
            has_instructions,
        });
    }
    result.sort_by(|left, right| {
        (
            left.address,
            &left.section,
            &left.object,
            &left.input_section,
        )
            .cmp(&(
                right.address,
                &right.section,
                &right.object,
                &right.input_section,
            ))
    });
    for (index, left) in result.iter().enumerate() {
        if result[index + 1..].iter().any(|right| {
            left.section == right.section
                && left.address < right.address + right.size
                && right.address < left.address + left.size
        }) {
            return Err(format!(
                "{} combines overlapping input members",
                left.section
            ));
        }
    }
    Ok(result)
}

fn mapping_symbols<'a>(table: &'a str, section: &'a str) -> impl Iterator<Item = (u64, char)> + 'a {
    table.lines().filter_map(move |line| {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        let name = *fields.last()?;
        let kind = if name == "$a" || name.starts_with("$a.") {
            'a'
        } else if name == "$t" || name.starts_with("$t.") {
            't'
        } else if name == "$d" || name.starts_with("$d.") {
            'd'
        } else {
            return None;
        };
        let start = u64::from_str_radix(fields[0], 16).ok()?;
        fields.contains(&section).then_some((start, kind))
    })
}

fn instruction_at(table: &str, section: &str, address: u64) -> bool {
    mapping_symbols(table, section)
        .filter(|(start, _)| *start <= address)
        .max_by_key(|(start, _)| *start)
        .is_some_and(|(_, kind)| matches!(kind, 'a' | 't'))
}

fn is_c_source(source: &Path) -> bool {
    matches!(
        source.extension().and_then(|extension| extension.to_str()),
        Some("c" | "C")
    )
}

fn linked_functions(
    root: &Path,
    build: &Build,
    sections: &[Section],
    symbols: &str,
    map: &str,
    object_tables: &str,
) -> Result<Vec<Function>, String> {
    let members = linked_inputs(root, build, sections, map, object_tables)?;
    let mut result = Vec::new();
    for line in symbols.lines() {
        let words = line.split_whitespace().collect::<Vec<_>>();
        if !words.contains(&"F") {
            continue;
        }
        let Some(section) = sections
            .iter()
            .find(|section| words.contains(&section.name.as_str()))
        else {
            continue;
        };
        let address = u64::from_str_radix(words[0], 16).map_err(|error| error.to_string())?;
        let owners = members
            .iter()
            .filter(|member| {
                member.section == section.name
                    && member.address <= address
                    && address < member.address + member.size
            })
            .collect::<Vec<_>>();
        if owners.len() != 1 {
            return Err(format!(
                "{} has no unique source object in the linker map",
                section.name
            ));
        }
        let member = owners[0];
        let object = &member.object;
        // Read ownership from the maintained input path. Function evidence is
        // separate from credit: assembly entries do not become matching C.
        let table = object_table(object_tables, object)?;
        let name = *words.last().expect("symbol name");
        let (input, input_address) = table
            .lines()
            .find_map(|line| {
                let fields = line.split_whitespace().collect::<Vec<_>>();
                if !fields.contains(&"F") || fields.last() != Some(&name) {
                    return None;
                }
                Some((
                    *fields
                        .iter()
                        .find(|field| **field == ".text" || field.starts_with(".text."))?,
                    u64::from_str_radix(fields[0], 16).ok()?,
                ))
            })
            .ok_or_else(|| format!("{name} is not a function in {}", object.display()))?;
        let input_sections = loaded_sections(table)?;
        let input_section = input_sections
            .iter()
            .find(|section| section.name == input)
            .ok_or_else(|| format!("{name} has no complete input section"))?;
        if input != member.input_section {
            return Err(format!("{name} has no independent input placement"));
        }
        // The approved era compiler assembler predates ARM mapping symbols.
        // Its current C function symbols retain executable evidence; maintained
        // assembly must map each individual entry as a real instruction.
        if !instruction_at(table, input, input_address)
            && !(is_c_source(&member.source) && mapping_symbols(table, input).next().is_none())
        {
            continue;
        }
        let input_end = input_section.address + input_section.size;
        let next = table
            .lines()
            .filter_map(|line| {
                let fields = line.split_whitespace().collect::<Vec<_>>();
                if !fields.contains(&"F") || !fields.contains(&input) {
                    return None;
                }
                let start = u64::from_str_radix(fields[0], 16).ok()?;
                (start > input_address).then_some(start)
            })
            .min()
            .unwrap_or(input_end);
        let size = next
            .checked_sub(input_address)
            .ok_or("function member extent overflow")?;
        if input_address < input_section.address
            || input_address >= input_end
            || next > input_end
            || size == 0
            || member.address + input_address - input_section.address != address
            || member.size != input_section.size
            || address < section.address
            || address
                .checked_add(size)
                .is_none_or(|end| end > section.address + section.size)
        {
            return Err(format!(
                "{name}: linked function section changes the complete emitted extent"
            ));
        }
        result.push(Function {
            name: name.into(),
            section: section.name.clone(),
            source: member.source.clone(),
            object: object.clone(),
            address,
            load_address: section.load_address + address - section.address,
            size,
            group_size: section.size,
        });
    }
    result.sort_by(|left, right| {
        (left.address, &left.section, &left.name).cmp(&(right.address, &right.section, &right.name))
    });
    for (index, left) in result.iter().enumerate() {
        if result[index + 1..].iter().any(|right| {
            left.section == right.section
                && left.address < right.address + right.size
                && right.address < left.address + left.size
        }) {
            return Err(format!(
                "{} combines overlapping function members",
                left.section
            ));
        }
    }
    Ok(result)
}

fn loaded_sections(text: &str) -> Result<Vec<Section>, String> {
    let lines = text.lines().collect::<Vec<_>>();
    let mut sections = Vec::new();
    for (index, line) in lines.iter().enumerate() {
        let fields = line.split_whitespace().collect::<Vec<_>>();
        if fields.len() < 7 || fields[0].parse::<usize>().is_err() {
            continue;
        }
        let flags = lines.get(index + 1).copied().unwrap_or("");
        if !["CONTENTS", "ALLOC", "LOAD"].iter().all(|flag| {
            flags
                .split(|ch: char| ch == ',' || ch.is_whitespace())
                .any(|word| word == *flag)
        }) {
            continue;
        }
        let number = |field: usize| {
            u64::from_str_radix(fields[field], 16)
                .map_err(|error| format!("section {}: {error}", fields[1]))
        };
        let size = number(2)?;
        if size > 0 {
            sections.push(Section {
                name: fields[1].into(),
                size,
                address: number(3)?,
                load_address: number(4)?,
            });
        }
    }
    if sections.is_empty() {
        return Err("linked ELF has no loaded sections".into());
    }
    Ok(sections)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn loaded_sections_distinguish_ram_execution_from_rom_storage() {
        let headers = "Idx Name Size VMA LMA File off Algn\n 0 .text 00000010 08000000 08000000 00000100 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\n 1 .iwram 00000020 03000000 08000010 00000110 2**2\n CONTENTS, ALLOC, LOAD, CODE\n 2 .bss 00000040 02000000 02000000 00000130 2**2\n ALLOC\n 3 .debug 00000010 00000000 00000000 00000130 2**0\n CONTENTS, READONLY\n";
        let sections = loaded_sections(headers).unwrap();
        assert_eq!(sections.len(), 2);
        assert_eq!(
            sections[1],
            Section {
                name: ".iwram".into(),
                size: 32,
                address: 0x0300_0000,
                load_address: 0x0800_0010
            }
        );
        assert!(
            loaded_sections(" 0 .bss 00000010 02000000 02000000 00000040 2**2\n ALLOC\n").is_err()
        );
    }

    #[test]
    fn native_build_has_no_arbitrary_compiler_flag_option() {
        let args = ["--script", "MAIN.LD", "--cflags", "-O0", "SOURCE.C"].map(str::to_owned);
        assert!(options(&args)
            .err()
            .unwrap()
            .contains("unknown native build option: --cflags"));
    }

    fn fixture() -> Build {
        let root = Path::new("/repo");
        let source = root.join("games/THE BROKEN SEAL/SRC/LEAF.C");
        let output = root.join("out/tbs-en/native");
        Build {
            image: "main".into(),
            sources: vec![source],
            script: root.join("games/THE BROKEN SEAL/MAIN.LD"),
            objects: vec![output.join("obj/games/THE BROKEN SEAL/SRC/LEAF.o")],
            archives: Vec::new(),
            elf: output.join("native.elf"),
            binary: output.join("native.bin"),
            symbols: output.join("native.nm"),
            map: output.join("native.map"),
            log: output.join("build.log"),
            sections: Vec::new(),
            functions: Vec::new(),
        }
    }

    #[test]
    fn function_accounting_includes_pool_after_symbol_size_and_checks_input_object() {
        let build = fixture();
        let sections = vec![Section {
            name: ".text.Module.Leaf".into(),
            size: 16,
            address: 0x08000100,
            load_address: 0x08000100,
        }];
        let symbols = "08000100 g F .text.Module.Leaf 00000006 Leaf\n";
        let map = format!(
            ".text.Module.Leaf 0x08000100 0x10\n .text.Leaf 0x08000100 0x10 {}\n",
            build.objects[0].display()
        );
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text.Leaf 00000010 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text.Leaf 00000000 $t\n00000000 g F .text.Leaf 00000006 Leaf\n", build.objects[0].display());
        let functions = linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object,
        )
        .unwrap();
        assert_eq!(functions.len(), 1);
        assert_eq!(functions[0].size, 16);
        assert_eq!(functions[0].group_size, 16);
        assert_eq!(functions[0].source, build.sources[0]);
        assert!(linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object.replace("00000010", "00000008")
        )
        .unwrap_err()
        .contains("complete emitted extent"));
        let symbols = format!("{symbols}08000106 l F .text.Module.Leaf 00000002 Second\n");
        assert!(linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            &symbols,
            &map,
            &object
        )
        .is_err());
    }

    #[test]
    fn ordinary_object_groups_preserve_complete_member_extents() {
        let build = fixture();
        let sections = vec![Section {
            name: ".text.Module".into(),
            size: 24,
            address: 0x08000100,
            load_address: 0x08010100,
        }];
        let symbols =
            "08000100 g F .text.Module 00000006 Leaf\n08000110 g F .text.Module 00000002 Second\n";
        let map = format!(".text.Module 0x08000100 0x18\n .text.Leaf\n 0x08000100 0x10 {}\n .text.Second 0x08000110 0x8 {}\n", build.objects[0].display(), build.objects[0].display());
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text.Leaf 00000010 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\n 1 .text.Second 00000008 00000000 00000000 00000050 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text.Leaf 00000000 $t\n00000000 l .text.Second 00000000 $t\n00000000 g F .text.Leaf 00000006 Leaf\n00000000 g F .text.Second 00000002 Second\n", build.objects[0].display());
        let functions = linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object,
        )
        .unwrap();
        assert_eq!(
            functions
                .iter()
                .map(|function| (function.size, function.group_size, function.load_address))
                .collect::<Vec<_>>(),
            [(16, 24, 0x08010100), (8, 24, 0x08010110)]
        );
        let overlapping = symbols.replace("08000110", "08000108");
        let map = map.replace("0x08000110", "0x08000108");
        assert!(linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            &overlapping,
            &map,
            &object
        )
        .unwrap_err()
        .contains("overlapping input members"));
    }

    #[test]
    fn ordinary_text_group_keeps_whole_object_extents_and_source_kind() {
        let mut build = fixture();
        build
            .sources
            .push(PathBuf::from("/repo/games/THE BROKEN SEAL/SRC/SECOND.C"));
        build.objects.push(PathBuf::from(
            "/repo/out/tbs-en/native/obj/games/THE BROKEN SEAL/SRC/SECOND.o",
        ));
        build
            .sources
            .push(PathBuf::from("/repo/recon/tbs/raw/08000118.s"));
        build.objects.push(PathBuf::from(
            "/repo/out/tbs-en/native/obj/recon/tbs/raw/08000118.o",
        ));
        let sections = vec![Section {
            name: ".text".into(),
            size: 28,
            address: 0x08000100,
            load_address: 0x08000100,
        }];
        let symbols = "08000100 g F .text 00000006 Leaf\n08000110 g F .text 00000002 Second\n08000118 g F .text 00000002 Fallback\n";
        let map = format!(".text 0x08000100 0x1c\n .data 0x08000100 0x0 {}\n .text 0x08000100 0x10 {}\n .data 0x08000110 0x0 {}\n .text 0x08000110 0x8 {}\n .text 0x08000118 0x4 {}\n", build.objects[0].display(), build.objects[0].display(), build.objects[0].display(), build.objects[1].display(), build.objects[2].display());
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text 00000010 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text 00000000 $t\n00000000 g F .text 00000006 Leaf\n\n{}:     file format elf32-littlearm\nSections:\n 0 .text 00000008 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text 00000000 $t\n00000000 g F .text 00000002 Second\n\n{}:     file format elf32-littlearm\nSections:\n 0 .text 00000004 00000000 00000000 00000040 2**1\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text 00000000 $t\n00000000 g F .text 00000002 Fallback\n", build.objects[0].display(), build.objects[1].display(), build.objects[2].display());
        let functions = linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object,
        )
        .unwrap();
        assert_eq!(functions.len(), 3);
        assert_eq!(functions[0].source, build.sources[0]);
        assert_eq!(functions[1].source, build.sources[1]);
        assert_eq!(functions[2].source, build.sources[2]);
        assert_eq!(
            functions
                .iter()
                .map(|function| (function.size, function.group_size))
                .collect::<Vec<_>>(),
            [(16, 28), (8, 28), (4, 28)]
        );
    }

    #[test]
    fn whole_text_members_use_current_next_function_boundary_including_pools() {
        let build = fixture();
        let sections = vec![Section {
            name: ".text.Module".into(),
            size: 24,
            address: 0x08000100,
            load_address: 0x08000100,
        }];
        let symbols =
            "08000100 g F .text.Module 00000006 Leaf\n08000110 g F .text.Module 00000002 Second\n";
        let map = format!(
            ".text.Module 0x08000100 0x18\n .text 0x08000100 0x18 {}\n",
            build.objects[0].display()
        );
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text 00000018 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text 00000000 $t\n00000000 g F .text 00000006 Leaf\n00000010 g F .text 00000002 Second\n", build.objects[0].display());
        let functions = linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object,
        )
        .unwrap();
        assert_eq!(
            functions
                .iter()
                .map(|function| function.size)
                .collect::<Vec<_>>(),
            [16, 8]
        );
        assert!(functions.iter().all(|function| function.group_size == 24));
    }

    #[test]
    fn directive_only_function_entries_are_not_executable_evidence() {
        let mut build = fixture();
        build.sources[0].set_extension("S");
        let sections = vec![Section {
            name: ".text".into(),
            size: 16,
            address: 0x08000100,
            load_address: 0x08000100,
        }];
        let symbols =
            "08000100 g F .text 00000002 Real\n08000108 g F .text 00000008 AlchemyC_Placeholder\n";
        let map = format!(
            ".text 0x08000100 0x10\n .text 0x08000100 0x10 {}\n",
            build.objects[0].display()
        );
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text 00000010 00000000 00000000 00000040 2**2\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 l .text 00000000 $t.0\n00000008 l .text 00000000 $d.1\n00000000 g F .text 00000002 Real\n00000008 g F .text 00000008 AlchemyC_Placeholder\n", build.objects[0].display());
        let functions = linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object,
        )
        .unwrap();
        assert_eq!(functions.len(), 1);
        assert_eq!(functions[0].name, "Real");
        assert!(linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object.replace("$t.0", "$d.0")
        )
        .unwrap()
        .is_empty());
    }

    #[test]
    fn loaded_inputs_include_data_without_function_symbols() {
        let build = fixture();
        let sections = vec![Section {
            name: ".iwram".into(),
            size: 12,
            address: 0x03000000,
            load_address: 0x08000100,
        }];
        let map = format!(".iwram 0x03000000 0xc\n .text 0x03000000 0x4 {}\n .rodata 0x03000004 0x6 {}\n .padding 0x0300000a 0x2 {}\n", build.objects[0].display(), build.objects[0].display(), build.objects[0].display());
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text 00000004 00000000 00000000 00000040 2**1\n CONTENTS, ALLOC, LOAD, READONLY, CODE\n 1 .rodata 00000006 00000000 00000000 00000044 2**1\n CONTENTS, ALLOC, LOAD, READONLY, DATA\n 2 .padding 00000002 00000000 00000000 0000004a 2**0\n CONTENTS, ALLOC, LOAD, READONLY, DATA\n", build.objects[0].display());
        let inputs = linked_inputs(Path::new("/repo"), &build, &sections, &map, &object).unwrap();
        assert_eq!(
            inputs
                .iter()
                .map(|input| (input.input_section.as_str(), input.load_address, input.size))
                .collect::<Vec<_>>(),
            [
                (".text", 0x08000100, 4),
                (".rodata", 0x08000104, 6),
                (".padding", 0x0800010a, 2)
            ]
        );
        assert!(inputs.iter().all(|input| input.source == build.sources[0]));
        assert!(inputs.iter().all(|input| !input.has_instructions));
        assert!(linked_inputs(
            Path::new("/repo"),
            &build,
            &sections,
            &map.replace("0x6", "0x4"),
            &object
        )
        .unwrap_err()
        .contains("complete emitted extent"));
    }

    #[test]
    fn approved_era_c_objects_retain_function_evidence_without_mapping_symbols() {
        let mut build = fixture();
        let sections = vec![Section {
            name: ".text".into(),
            size: 4,
            address: 0x08000100,
            load_address: 0x08000100,
        }];
        let map = format!(
            ".text 0x08000100 0x4\n .text 0x08000100 0x4 {}\n",
            build.objects[0].display()
        );
        let object = format!("{}:     file format elf32-littlearm\nSections:\n 0 .text 00000004 00000000 00000000 00000040 2**1\n CONTENTS, ALLOC, LOAD, READONLY, CODE\nSYMBOL TABLE:\n00000000 g F .text 00000002 Leaf\n", build.objects[0].display());
        let symbols = "08000100 g F .text 00000002 Leaf\n";
        assert_eq!(
            linked_functions(
                Path::new("/repo"),
                &build,
                &sections,
                symbols,
                &map,
                &object
            )
            .unwrap()
            .len(),
            1
        );
        assert!(
            linked_inputs(Path::new("/repo"), &build, &sections, &map, &object).unwrap()[0]
                .has_instructions
        );
        build.sources[0].set_extension("S");
        assert!(linked_functions(
            Path::new("/repo"),
            &build,
            &sections,
            symbols,
            &map,
            &object
        )
        .unwrap()
        .is_empty());
        assert!(
            !linked_inputs(Path::new("/repo"), &build, &sections, &map, &object).unwrap()[0]
                .has_instructions
        );
    }

    #[test]
    fn image_is_explicit_and_the_link_is_strict() {
        let args =
            ["--script", "MAIN.LD", "--image", "resource_388", "SOURCE.C"].map(str::to_owned);
        let parsed = options(&args).unwrap();
        assert_eq!(parsed.image, "resource_388");
        let args = ["--script", "MAIN.LD", "--audit", "SOURCE.C"].map(str::to_owned);
        assert!(options(&args)
            .err()
            .unwrap()
            .contains("unknown native build option"));
        let args = ["--script", "MAIN.LD", "--image", "../escape", "SOURCE.C"].map(str::to_owned);
        assert!(options(&args).is_err());
    }

    #[test]
    fn distinct_sources_cannot_overwrite_one_object() {
        let dir = tempfile::tempdir().unwrap();
        let game = "games/THE BROKEN SEAL";
        fs::create_dir_all(dir.path().join(game).join("SRC")).unwrap();
        for name in ["SRC/SOURCE.C", "SRC/SOURCE.S", "MAIN.LD"] {
            fs::write(dir.path().join(game).join(name), "").unwrap();
        }
        let error = build_scoped(
            dir.path(),
            decomp_target(Some("tbs-en")).unwrap(),
            &[
                format!("{game}/SRC/SOURCE.C").into(),
                format!("{game}/SRC/SOURCE.S").into(),
            ],
            Path::new(&format!("{game}/MAIN.LD")),
            Path::new("out/native"),
            "main",
            &[],
        )
        .err()
        .unwrap();
        assert!(error.contains("shares an object path"));
        assert!(!dir.path().join("out").exists());
    }

    #[test]
    fn generated_or_other_game_sources_cannot_become_native_inputs() {
        let root = tempfile::tempdir().unwrap();
        let target = decomp_target(Some("tbs-en")).unwrap();
        for path in [
            "out/answer.S",
            "games/THE LOST AGE/SRC/BOOT.C",
            "games/THE BROKEN SEAL/SRC/BOOT.C",
            "games/COMMON/SRC/MEMORY.C",
            "recon/tbs/raw/08000100.s",
            "recon/tbs/raw/DRAFT.C",
            "recon/tla/raw/08000100.s",
        ] {
            fs::create_dir_all(root.path().join(path).parent().unwrap()).unwrap();
            fs::write(root.path().join(path), "").unwrap();
            let result = maintained_sources(root.path(), target, &[path.into()]);
            assert_eq!(
                result.is_ok(),
                !path.starts_with("out/")
                    && !path.contains("THE LOST AGE")
                    && !path.contains("recon/tla")
                    && !path.ends_with("DRAFT.C")
            );
        }
    }
}
