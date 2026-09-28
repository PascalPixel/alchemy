//! Compile unchanged source modules and link the explicit object list.
use super::plan::{source_to_assembly_plan, SourceToAssemblyPlanOptions};
use super::routing::{assembly_command, compiler_assembly_command};
use crate::targets::{decomp_target, DecompTarget};
use psynergy::process::run as command;
use std::collections::BTreeSet;
use std::fs::{self, File};
use std::io::Write;
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy build native --script FILE [--target GAME-EDITION] [--output DIR] SOURCE...\nCompile whole C or assembly files with the approved toolchain and link their natural symbols.\nThe linker script supplies placement and imports; outputs stay under ignored out/.";

#[derive(Debug, Clone, PartialEq, Eq, serde::Serialize, serde::Deserialize)]
pub struct Section {
    pub name: String,
    pub size: u64,
    pub address: u64,
    pub load_address: u64,
}

#[derive(Debug, serde::Serialize, serde::Deserialize)]
pub struct Build {
    pub sources: Vec<PathBuf>,
    pub script: PathBuf,
    pub objects: Vec<PathBuf>,
    pub elf: PathBuf,
    pub binary: PathBuf,
    pub symbols: PathBuf,
    pub map: PathBuf,
    pub log: PathBuf,
    pub sections: Vec<Section>,
}

struct Options {
    target: DecompTarget,
    sources: Vec<PathBuf>,
    script: PathBuf,
    output: PathBuf,
}

fn options(args: &[String]) -> Result<Options, String> {
    let mut target = decomp_target(None)?;
    let mut script = None;
    let mut output = None;
    let mut sources = Vec::new();
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
    Ok(Options {
        target,
        sources,
        script: script.ok_or_else(|| format!("native build needs --script\n{USAGE}"))?,
        output: output.unwrap_or_else(|| Path::new(target.output_dir).join("native")),
    })
}

pub fn run(args: &[String]) -> Result<(), String> {
    if args == ["--help"] || args == ["-h"] {
        println!("{USAGE}");
        return Ok(());
    }
    let options = options(args)?;
    let result = build(
        super::routing::root(),
        options.target,
        &options.sources,
        &options.script,
        &options.output,
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
    let mut seen = BTreeSet::new();
    let mut canonical = Vec::new();
    for source in sources {
        let source = input(&root, source)?;
        if (!source.starts_with(&game) && !source.starts_with(&common))
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

pub fn build(
    root: &Path,
    target: DecompTarget,
    sources: &[PathBuf],
    script: &Path,
    output: &Path,
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
    let output = super::build_io::generated_directory(&root, output)?;
    let mut result = Build {
        sources: sources.clone(),
        script: script.clone(),
        objects: Vec::new(),
        elf: output.join("native.elf"),
        binary: output.join("native.bin"),
        symbols: output.join("native.nm"),
        map: output.join("native.map"),
        log: output.join("build.log"),
        sections: Vec::new(),
    };
    for path in [&result.elf, &result.binary, &result.symbols, &result.map] {
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
        link.extend(
            result
                .objects
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

    #[test]
    fn distinct_sources_cannot_overwrite_one_object() {
        let dir = tempfile::tempdir().unwrap();
        let game = "games/THE BROKEN SEAL";
        fs::create_dir_all(dir.path().join(game).join("SRC")).unwrap();
        for name in ["SRC/SOURCE.C", "SRC/SOURCE.S", "MAIN.LD"] {
            fs::write(dir.path().join(game).join(name), "").unwrap();
        }
        let error = build(
            dir.path(),
            decomp_target(Some("tbs-en")).unwrap(),
            &[
                format!("{game}/SRC/SOURCE.C").into(),
                format!("{game}/SRC/SOURCE.S").into(),
            ],
            Path::new(&format!("{game}/MAIN.LD")),
            Path::new("out/native"),
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
        ] {
            fs::create_dir_all(root.path().join(path).parent().unwrap()).unwrap();
            fs::write(root.path().join(path), "").unwrap();
            let result = maintained_sources(root.path(), target, &[path.into()]);
            assert_eq!(
                result.is_ok(),
                !path.starts_with("out/") && !path.contains("THE LOST AGE")
            );
        }
    }
}
