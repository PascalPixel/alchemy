//! Inspect complete source functions independently from their ELF objects.
//! Every accepted extent comes from the ELF section, including its literal
//! pool. Generated metadata identifies artifacts; it cannot grant credit.
use super::executable::{MainLayout, MainSection, SectionKind, SectionProof};
use super::model::{normalize, subtract, Span};
use super::proof::Credit;
use crate::compiler::native::{self, Build, Function, Section};
use crate::targets::DecompTarget;
use serde::Serialize;
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Path, PathBuf};

pub(crate) struct ImageBuild {
    pub build: Build,
    pub bytes: BTreeMap<String, Vec<u8>>,
    pub accepted: Vec<Function>,
    pub inputs: Vec<native::Input>,
    pub libraries: Vec<Section>,
    pub rejected: Vec<Rejection>,
}

impl ImageBuild {
    pub fn function_bytes(&self, function: &Function) -> Result<&[u8], String> {
        self.member_bytes(&function.section, function.address, function.size)
    }

    pub fn input_bytes(&self, input: &native::Input) -> Result<&[u8], String> {
        self.member_bytes(&input.section, input.address, input.size)
    }

    fn member_bytes(&self, name: &str, address: u64, size: u64) -> Result<&[u8], String> {
        let section = self
            .build
            .sections
            .iter()
            .find(|section| section.name == name)
            .ok_or("source input has no output section")?;
        let offset = usize::try_from(
            address
                .checked_sub(section.address)
                .ok_or("source input precedes its group")?,
        )
        .map_err(|error| error.to_string())?;
        self.bytes[name]
            .get(
                offset
                    ..offset
                        .checked_add(size as usize)
                        .ok_or("source input extent overflow")?,
            )
            .ok_or_else(|| "source input extends beyond its complete output group".into())
    }
}

pub(crate) struct Verification {
    pub images: BTreeMap<String, ImageBuild>,
    pub layout: MainLayout,
    pub credits: Vec<Credit>,
    pub source_bytes: usize,
    pub assets: Vec<(PathBuf, u64, Vec<u8>)>,
    pub artifacts_sha256: String,
}

/// Verify the native images and assembly produced by the ordinary build.
/// Failed or nonmatching C attempts remain diagnostics and earn no credit.
pub(crate) fn verify(
    root: &Path,
    target: DecompTarget,
    reference: &[u8],
) -> Result<Verification, String> {
    crate::text_catalog::verify_reference(root, target.id.as_str(), reference)?;
    let rom = crate::overlay::rom::CanonicalRom::load_target(root, target)?;
    let native = root.join(target.output_dir).join("native");
    let mut directories = vec![("main".to_owned(), native.clone())];
    let overlays = native.join("overlays");
    if overlays.is_dir() {
        for entry in std::fs::read_dir(&overlays).map_err(|error| error.to_string())? {
            let entry = entry.map_err(|error| error.to_string())?;
            if !entry
                .file_type()
                .map_err(|error| error.to_string())?
                .is_dir()
            {
                continue;
            }
            let image = entry.file_name().to_string_lossy().into_owned();
            crate::overlay::rom::resource_id(&image)?;
            if entry.path().join("native.json").is_file() {
                directories.push((image, entry.path()))
            }
        }
    }
    directories.sort_by(|left, right| left.0.cmp(&right.0));
    let mut images = BTreeMap::new();
    let mut all_credits = Vec::new();
    let mut artifacts = BTreeMap::new();
    let mut layout = MainLayout::default();
    let mut source_ranges = Vec::new();
    let mut verified_archives = BTreeSet::new();
    for (image, directory) in directories {
        let loaded;
        let original = if image == "main" {
            reference
        } else {
            loaded = crate::compiler::overlay::load(&rom.overlay(&image)?, 0)?;
            &loaded
        };
        let inspected = inspect(root, target, &image, &directory, original)?;
        for archive in &inspected.build.archives {
            if verified_archives.insert(archive.clone()) {
                crate::compiler::runtime::verify_archive(root, archive)?;
                artifact(root, archive, &mut artifacts)?;
            }
        }
        all_credits.extend(credits(root, &image, &inspected.accepted)?);
        for section in &inspected.libraries {
            let kind = if section.name == ".library.rodata" {
                SectionKind::Data
            } else {
                SectionKind::Executable
            };
            let span = Span::new(
                section.load_address as i64,
                (section.load_address + section.size) as i64,
            );
            if kind == SectionKind::Executable {
                let bias = if image == "main" { 0 } else { 0x8000 };
                all_credits.push(Credit {
                    image: image.clone(),
                    start: span.start - bias,
                    end: span.end - bias,
                    source: "Makefile".into(),
                    kind: "assembly".into(),
                });
            }
            if image == "main" {
                source_ranges.push(span);
                layout.sections.push(MainSection {
                    span,
                    kind,
                    source: "Makefile".into(),
                    proof: SectionProof::Linked(inspected.bytes[&section.name].clone()),
                });
            }
        }
        for path in [
            &inspected.build.elf,
            &inspected.build.binary,
            &inspected.build.map,
        ]
        .into_iter()
        .chain(inspected.build.objects.iter())
        {
            artifact(root, path, &mut artifacts)?;
        }
        artifact(root, &directory.join("native.json"), &mut artifacts)?;
        if image == "main" {
            for input in &inspected.inputs {
                let span = Span::new(
                    input.load_address as i64,
                    (input.load_address + input.size) as i64,
                );
                source_ranges.push(span);
                let Some(kind) = input_kind(input)? else {
                    continue;
                };
                layout.sections.push(MainSection {
                    span,
                    kind,
                    source: input
                        .source
                        .strip_prefix(root)
                        .map_err(|error| error.to_string())?
                        .to_owned(),
                    proof: SectionProof::Linked(inspected.input_bytes(input)?.to_vec()),
                });
            }
        } else {
            for function in &inspected.accepted {
                layout
                    .overlay_entries
                    .push(super::executable::OverlayEntry {
                        image: image.clone(),
                        address: function.load_address as i64 - 0x8000,
                        source: function
                            .source
                            .strip_prefix(root)
                            .map_err(|error| error.to_string())?
                            .to_owned(),
                        compiled: inspected.function_bytes(function)?.to_vec(),
                    });
            }
        }
        images.insert(image, inspected);
    }
    // Verify canonical placements, maintained classifications and every
    // assembled region before reading any credit annotation.
    let (_, fallback) = super::proof::fallback_artifacts(root, target, reference)?;
    source_ranges.extend(
        fallback
            .into_iter()
            .map(|(start, end)| Span::new(0x0800_0000 + start as i64, 0x0800_0000 + end as i64)),
    );
    let directory = root.join(target.output_dir).join("full/asm");
    let manifest = directory.join("manifest.json");
    artifact(root, &manifest, &mut artifacts)?;
    let document: serde_json::Value =
        serde_json::from_slice(&std::fs::read(&manifest).map_err(|error| error.to_string())?)
            .map_err(|error| error.to_string())?;
    let native_spans = normalize(
        &layout
            .sections
            .iter()
            .map(|section| section.span)
            .collect::<Vec<_>>(),
    );
    for region in document["regions"]
        .as_array()
        .ok_or("assembly manifest has no regions")?
    {
        let address = region["address"]
            .as_u64()
            .ok_or("assembly has no load address")?;
        let run_address = region["run_address"]
            .as_u64()
            .ok_or("assembly has no run address")?;
        let source = PathBuf::from(region["source"].as_str().ok_or("assembly has no source")?);
        let binary = directory.join(format!("{address:08x}.bin"));
        let elf = directory.join(format!("{address:08x}.elf"));
        let object = directory.join(format!("{address:08x}.o"));
        let bytes = std::fs::read(&binary).map_err(|error| error.to_string())?;
        let sections = native::inspect_loaded_sections(root, &elf)?;
        let section_bytes = loaded_bytes(root, &elf, &sections)?;
        let symbols = psynergy::process::run(
            &[
                "arm-none-eabi-nm".into(),
                object.to_string_lossy().into_owned(),
            ],
            root,
        )?;
        let mappings = psynergy::process::run(
            &[
                "arm-none-eabi-objdump".into(),
                "--special-syms".into(),
                "-t".into(),
                elf.to_string_lossy().into_owned(),
            ],
            root,
        )?;
        let instruction_sections = instruction_sections(&mappings);
        let text =
            std::fs::read_to_string(root.join(&source)).map_err(|error| error.to_string())?;
        for (start, end) in crate::build_asm::credited_spans(
            &text,
            source.starts_with(target.source_dir),
            address,
            &bytes,
            &symbols,
            &target.overlay_macro(),
        )? {
            all_credits.push(Credit {
                image: "main".into(),
                start: start as i64,
                end: end as i64,
                source: source.to_string_lossy().into_owned(),
                kind: "assembly".into(),
            });
        }
        for section in sections {
            let offset = usize::try_from(
                section
                    .load_address
                    .checked_sub(run_address)
                    .ok_or("assembly section precedes maintained run address")?,
            )
            .map_err(|error| error.to_string())?;
            let data = &section_bytes[&section.name];
            let end = offset
                .checked_add(data.len())
                .ok_or("assembly section offset overflow")?;
            if bytes.get(offset..end) != Some(data.as_slice()) {
                return Err("assembly binary differs from its ELF section".into());
            }
            let span = Span::new(address as i64 + offset as i64, address as i64 + end as i64);
            let kind = match section.name.as_str() {
                ".text" if instruction_sections.contains(".text") => SectionKind::Executable,
                // A raw fragment containing only directives supplies bytes,
                // but neither its section name nor its contents prove code.
                ".text" => continue,
                ".data" | ".rodata" => SectionKind::Data,
                ".padding" => SectionKind::Padding,
                _ => return Err("assembly ELF has an unclassified loaded section".into()),
            };
            if layout.sections.iter().any(|native| {
                native.kind != kind && !super::model::intersect(&[span], &[native.span]).is_empty()
            }) {
                return Err(
                    "native source conflicts with maintained code/data classification".into(),
                );
            }
            // Matching C replaces fallback bytes; both may prove the same
            // code classification, but a physical byte is represented once.
            for part in subtract(&[span], &native_spans) {
                let start = (part.start - span.start) as usize;
                let end = (part.end - span.start) as usize;
                layout.sections.push(MainSection {
                    span: part,
                    kind,
                    source: source.clone(),
                    proof: SectionProof::Linked(data[start..end].to_vec()),
                });
            }
        }
        for path in [binary, elf, object] {
            artifact(root, &path, &mut artifacts)?;
        }
    }
    layout
        .sections
        .extend(super::executable::parsed_main_data(&rom)?);
    let mut assets = super::midi::build(root, target, reference)?;
    assets.push(crate::text_catalog::build_verified(
        root, target, reference,
    )?);
    artifact(
        root,
        &root.join(target.output_dir).join("text/archive.bin"),
        &mut artifacts,
    )?;
    for (source, address, bytes) in &assets {
        let span = Span::new(*address as i64, *address as i64 + bytes.len() as i64);
        source_ranges.push(span);
        layout.sections.push(MainSection {
            span,
            kind: SectionKind::Data,
            source: source.clone(),
            proof: SectionProof::Linked(bytes.clone()),
        });
    }
    let source_bytes = usize::try_from(super::model::bytes(&normalize(&source_ranges)))
        .map_err(|error| error.to_string())?;
    all_credits.sort_by(|left, right| {
        (&left.image, left.start, left.end, &left.source, &left.kind).cmp(&(
            &right.image,
            right.start,
            right.end,
            &right.source,
            &right.kind,
        ))
    });
    Ok(Verification {
        images,
        layout,
        credits: all_credits,
        source_bytes,
        assets,
        artifacts_sha256: crate::compiler::sha256::hex(
            &serde_json::to_vec(&artifacts).map_err(|error| error.to_string())?,
        ),
    })
}

fn instruction_sections(symbols: &str) -> BTreeSet<String> {
    symbols
        .lines()
        .filter_map(|line| {
            let fields = line.split_whitespace().collect::<Vec<_>>();
            let name = fields.last()?.split('.').next()?;
            if !matches!(name, "$a" | "$t") || fields.len() < 4 {
                return None;
            }
            Some(fields[fields.len() - 3].to_owned())
        })
        .collect()
}

fn input_kind(input: &native::Input) -> Result<Option<SectionKind>, String> {
    let family = input.input_section.split('.').nth(1).unwrap_or("");
    match family {
        "text" => Ok(input.has_instructions.then_some(SectionKind::Executable)),
        "rodata" | "data" => Ok(Some(SectionKind::Data)),
        "padding" => Ok(Some(SectionKind::Padding)),
        _ => Err(format!(
            "{}: loaded source input {} has no code/data classification",
            input.source.display(),
            input.input_section
        )),
    }
}

fn artifact(
    root: &Path,
    path: &Path,
    artifacts: &mut BTreeMap<String, String>,
) -> Result<(), String> {
    let relative = path
        .strip_prefix(root)
        .map_err(|error| error.to_string())?
        .to_string_lossy()
        .into_owned();
    artifacts.insert(
        relative,
        crate::compiler::sha256::hex(
            &std::fs::read(path).map_err(|error| format!("{}: {error}", path.display()))?,
        ),
    );
    Ok(())
}

#[derive(Serialize)]
pub(crate) struct Rejection {
    pub function: String,
    pub source: PathBuf,
    pub reason: String,
}

fn canonical(root: &Path, declared: &Path, expected: &Path) -> Result<(), String> {
    let declared = if declared.is_absolute() {
        declared.to_owned()
    } else {
        root.join(declared)
    };
    if std::fs::canonicalize(&declared).map_err(|error| error.to_string())?
        != std::fs::canonicalize(expected).map_err(|error| error.to_string())?
    {
        return Err("source metadata redirects a canonical artifact".into());
    }
    Ok(())
}

pub(crate) fn inspect(
    root: &Path,
    target: DecompTarget,
    image: &str,
    directory: &Path,
    reference: &[u8],
) -> Result<ImageBuild, String> {
    let metadata = directory.join("native.json");
    let build: Build = serde_json::from_slice(
        &std::fs::read(&metadata).map_err(|error| format!("{}: {error}", metadata.display()))?,
    )
    .map_err(|error| error.to_string())?;
    if build.image != image {
        return Err("native metadata names a different image".into());
    }
    let sources = native::maintained_sources(root, target, &build.sources)?;
    if sources != build.sources {
        return Err("native sources are not canonical maintained inputs".into());
    }
    let game =
        std::fs::canonicalize(root.join(target.game_dir())).map_err(|error| error.to_string())?;
    let script = std::fs::canonicalize(&build.script).map_err(|error| error.to_string())?;
    if image == "main" {
        canonical(root, &script, &game.join("MAIN.LD"))?;
    } else if !script.starts_with(game.join("LINK"))
        || script.extension().and_then(|suffix| suffix.to_str()) != Some("LD")
    {
        return Err("overlay link needs an ordinary maintained linker script".into());
    }
    canonical(root, &build.elf, &directory.join("native.elf"))?;
    canonical(root, &build.binary, &directory.join("native.bin"))?;
    canonical(root, &build.map, &directory.join("native.map"))?;
    let mut seen = BTreeSet::new();
    for object in &build.objects {
        let input = sources.iter().find(|source| {
            source.strip_prefix(root).ok().is_some_and(|relative| {
                object == &directory.join("obj").join(relative.with_extension("o"))
            })
        });
        if input.is_none() || !seen.insert(object) || !object.is_file() {
            return Err("native object has no unique maintained source".into());
        }
    }
    let archive_root = std::fs::canonicalize(root.join("tools/out/compiler-runtime")).ok();
    let mut archive_seen = BTreeSet::new();
    for archive in &build.archives {
        let actual = std::fs::canonicalize(archive).map_err(|error| error.to_string())?;
        if Some(&actual) != Some(archive)
            || archive_root
                .as_ref()
                .is_none_or(|base| !actual.starts_with(base))
            || actual.extension().and_then(|suffix| suffix.to_str()) != Some("a")
            || !archive_seen.insert(actual)
        {
            return Err("native library is not a unique canonical compiler archive".into());
        }
    }
    let sections = native::inspect_loaded_sections(root, &build.elf)?;
    if sections != build.sections {
        return Err("native metadata disagrees with ELF section extents".into());
    }
    let functions = native::inspect_functions(root, &build)?;
    if functions != build.functions {
        return Err("native metadata disagrees with compiled function ownership".into());
    }
    let bytes = loaded_bytes(root, &build.elf, &sections)?;
    let base = if image == "main" {
        0x0800_0000
    } else {
        0x0200_8000
    };
    let (accepted, mut rejected) = select(&functions, &sections, &bytes, reference, base)?;
    let inputs = native::inspect_inputs(root, &build)?
        .into_iter()
        .filter(|input| {
            let Some(section) = sections
                .iter()
                .find(|section| section.name == input.section)
            else {
                return false;
            };
            let data = &bytes[&section.name];
            section
                .load_address
                .checked_sub(base)
                .and_then(|start| usize::try_from(start).ok())
                .and_then(|start| start.checked_add(data.len()).map(|end| (start, end)))
                .is_some_and(|(start, end)| reference.get(start..end) == Some(data.as_slice()))
        })
        .collect();
    let libraries = select_libraries(&build, &sections, &bytes, reference, base)?;
    if sections
        .iter()
        .any(|section| section.name.starts_with(".library"))
        && libraries.is_empty()
    {
        rejected.push(Rejection {
            function: "compiler library".into(),
            source: PathBuf::from("Makefile"),
            reason: "the complete linked compiler library differs from the reference".into(),
        });
    }
    Ok(ImageBuild {
        build,
        bytes,
        accepted,
        inputs,
        libraries,
        rejected,
    })
}

fn select_libraries(
    build: &Build,
    sections: &[Section],
    bytes: &BTreeMap<String, Vec<u8>>,
    reference: &[u8],
    base: u64,
) -> Result<Vec<Section>, String> {
    let libraries = sections
        .iter()
        .filter(|section| section.name.starts_with(".library"))
        .collect::<Vec<_>>();
    if libraries.is_empty() {
        return Ok(Vec::new());
    }
    if build.archives.is_empty()
        || libraries
            .iter()
            .any(|section| !matches!(section.name.as_str(), ".library" | ".library.rodata"))
    {
        return Err("compiler library has no approved archive or an unclassified section".into());
    }
    let map = std::fs::read_to_string(&build.map).map_err(|error| error.to_string())?;
    let mut in_library = false;
    let mut contributors = 0;
    for line in map.lines() {
        if !line.starts_with(char::is_whitespace) {
            in_library = line
                .split_whitespace()
                .next()
                .is_some_and(|name| name.starts_with(".library"));
        } else if in_library {
            let fields = line.split_whitespace().collect::<Vec<_>>();
            if fields
                .first()
                .is_some_and(|name| name.starts_with(".text") || name.starts_with(".rodata"))
                && fields.len() >= 4
            {
                if !build
                    .archives
                    .iter()
                    .any(|archive| line.contains(&format!("{}(", archive.display())))
                {
                    return Err("compiler library contains a non-library input".into());
                }
                contributors += 1;
            }
        }
    }
    if contributors == 0 {
        return Err("compiler library has no independently linked archive member".into());
    }
    for section in &libraries {
        let data = &bytes[&section.name];
        let Some(start) = section
            .load_address
            .checked_sub(base)
            .and_then(|value| usize::try_from(value).ok())
        else {
            return Ok(Vec::new());
        };
        if start
            .checked_add(data.len())
            .is_none_or(|end| reference.get(start..end) != Some(data.as_slice()))
        {
            return Ok(Vec::new());
        }
    }
    Ok(libraries.into_iter().cloned().collect())
}

/// Read each loaded section independently from its bounded ELF extent.
pub(crate) fn loaded_bytes(
    root: &Path,
    elf: &Path,
    sections: &[Section],
) -> Result<BTreeMap<String, Vec<u8>>, String> {
    crate::compiler::routing::prefer_installed_binutils();
    let dump = psynergy::process::run(
        &[
            "arm-none-eabi-objdump".into(),
            "-s".into(),
            elf.to_string_lossy().into_owned(),
        ],
        root,
    )?;
    parse_dump(&dump, sections)
}

fn parse_dump(dump: &str, sections: &[Section]) -> Result<BTreeMap<String, Vec<u8>>, String> {
    let mut result = BTreeMap::<String, Vec<u8>>::new();
    let mut current = None;
    for line in dump.lines() {
        if let Some(name) = line
            .strip_prefix("Contents of section ")
            .and_then(|s| s.strip_suffix(':'))
        {
            current = sections.iter().find(|section| section.name == name);
            if let Some(section) = current {
                if result.insert(section.name.clone(), Vec::new()).is_some() {
                    return Err("ELF contains duplicate loaded section names".into());
                }
            }
            continue;
        }
        let Some(section) = current else { continue };
        let words = line.split_whitespace().collect::<Vec<_>>();
        if words.is_empty() {
            continue;
        }
        let address =
            u64::from_str_radix(words[0], 16).map_err(|_| "malformed ELF section data address")?;
        let data = result.get_mut(&section.name).expect("current section");
        if address != section.address + data.len() as u64 {
            return Err("ELF section dump has a gap or duplicate data".into());
        }
        let line_bytes = usize::try_from(section.size - data.len() as u64)
            .map_err(|error| error.to_string())?
            .min(16);
        let mut added = 0;
        for word in words.iter().skip(1).take(4) {
            if added == line_bytes {
                break;
            }
            if word.len() % 2 != 0 || word.len() > 8 || !word.bytes().all(|b| b.is_ascii_hexdigit())
            {
                return Err("malformed ELF section data".into());
            }
            for pair in word.as_bytes().chunks_exact(2) {
                if added == line_bytes {
                    return Err("ELF section dump exceeds its extent".into());
                }
                let pair = std::str::from_utf8(pair).map_err(|error| error.to_string())?;
                data.push(u8::from_str_radix(pair, 16).map_err(|error| error.to_string())?);
                added += 1;
            }
        }
        if added != line_bytes {
            return Err("ELF section dump truncates data".into());
        }
    }
    for section in sections {
        if result
            .get(&section.name)
            .is_none_or(|bytes| bytes.len() as u64 != section.size)
        {
            return Err(format!("{} has no complete ELF data", section.name));
        }
    }
    Ok(result)
}

fn select(
    functions: &[Function],
    definitions: &[Section],
    sections: &BTreeMap<String, Vec<u8>>,
    reference: &[u8],
    base: u64,
) -> Result<(Vec<Function>, Vec<Rejection>), String> {
    let mut accepted = Vec::<Function>::new();
    let mut rejected = Vec::new();
    for function in functions {
        let section = definitions
            .iter()
            .find(|section| section.name == function.section)
            .ok_or("function has no loaded ELF section")?;
        let data = sections
            .get(&function.section)
            .ok_or("function has no ELF section")?;
        let offset = function
            .address
            .checked_sub(section.address)
            .ok_or("function precedes its output group")?;
        if data.len() as u64 != function.group_size
            || section.size != function.group_size
            || function.size == 0
            || offset
                .checked_add(function.size)
                .is_none_or(|end| end > section.size)
            || section.load_address.checked_add(offset) != Some(function.load_address)
        {
            return Err("function metadata truncates its complete section".into());
        }
        // The complete ordered output group must match, including every pool.
        let extent = section
            .load_address
            .checked_sub(base)
            .and_then(|start| usize::try_from(start).ok())
            .and_then(|start| start.checked_add(data.len()).map(|end| (start, end)));
        let partial = ["Fragment_", "Region_", "Continuation_"]
            .iter()
            .any(|prefix| function.name.starts_with(prefix));
        let reason = if partial {
            Some("a maintained fragment cannot claim a complete C function")
        } else if extent
            .is_none_or(|(start, end)| reference.get(start..end) != Some(data.as_slice()))
        {
            Some("the complete emitted function and literal pool differ from the reference")
        } else {
            None
        };
        if let Some(reason) = reason {
            rejected.push(Rejection {
                function: function.name.clone(),
                source: function.source.clone(),
                reason: reason.into(),
            });
            continue;
        }
        let end = function
            .load_address
            .checked_add(function.size)
            .ok_or("function address overflow")?;
        if accepted.iter().any(|old| {
            function.load_address < old.load_address + old.size && old.load_address < end
        }) {
            return Err("whole matching source functions overlap".into());
        }
        accepted.push(function.clone());
    }
    Ok((accepted, rejected))
}

fn credits(root: &Path, image: &str, functions: &[Function]) -> Result<Vec<Credit>, String> {
    functions
        .iter()
        .filter(|function| {
            function
                .source
                .extension()
                .and_then(|s| s.to_str())
                .is_some_and(|s| s.eq_ignore_ascii_case("c"))
        })
        .map(|function| {
            let bias = if image == "main" { 0 } else { 0x8000 };
            Ok(Credit {
                image: image.into(),
                start: (function.load_address - bias) as i64,
                end: (function.load_address + function.size - bias) as i64,
                source: function
                    .source
                    .strip_prefix(root)
                    .map_err(|error| error.to_string())?
                    .to_string_lossy()
                    .into_owned(),
                kind: "c".into(),
            })
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn text_directives_do_not_establish_executable_code() {
        let data = "0800690e l .text 00000000 $d\n0800690e g F .text 00000000 Fragment\n";
        assert!(instruction_sections(data).is_empty());
        let code = "080000c0 l .text 00000000 $t\n08000100 l .text 00000000 $d\n03000000 l .ram 00000000 $a.1\n";
        assert_eq!(
            instruction_sections(code),
            BTreeSet::from([".text".into(), ".ram".into()])
        );
    }
    #[test]
    fn library_credit_requires_the_whole_archive_link_and_only_library_inputs() {
        let temporary = tempfile::tempdir().unwrap();
        let map = temporary.path().join("native.map");
        let archive = temporary.path().join("libgcc.a");
        let build = Build {
            image: "main".into(),
            sources: Vec::new(),
            script: PathBuf::new(),
            objects: Vec::new(),
            archives: vec![archive.clone()],
            elf: PathBuf::new(),
            binary: PathBuf::new(),
            symbols: PathBuf::new(),
            map: map.clone(),
            log: PathBuf::new(),
            sections: Vec::new(),
            functions: Vec::new(),
        };
        let sections = vec![
            section(".library", 100, 2),
            section(".library.rodata", 102, 2),
        ];
        let bytes = BTreeMap::from([
            (".library".into(), vec![1, 2]),
            (".library.rodata".into(), vec![3, 4]),
        ]);
        std::fs::write(&map, format!(".library 0x64 0x2\n .text 0x64 0x2 {}(_call_via_rX.o)\n.library.rodata 0x66 0x2\n .rodata 0x66 0x2 {}(_pack_df.o)\n", archive.display(), archive.display())).unwrap();
        assert_eq!(
            select_libraries(&build, &sections, &bytes, &[1, 2, 3, 4], 100)
                .unwrap()
                .len(),
            2
        );
        assert!(
            select_libraries(&build, &sections, &bytes, &[1, 2, 3, 9], 100)
                .unwrap()
                .is_empty()
        );
        std::fs::write(&map, ".library 0x64 0x2\n .text 0x64 0x2 game.o\n").unwrap();
        assert!(select_libraries(&build, &sections, &bytes, &[1, 2, 3, 4], 100).is_err());
    }
    fn section(name: &str, start: u64, size: u64) -> Section {
        Section {
            name: name.into(),
            address: start,
            load_address: start,
            size,
        }
    }
    fn function(name: &str, section: &str, start: u64, size: u64) -> Function {
        Function {
            name: name.into(),
            section: section.into(),
            source: PathBuf::from("games/TEST/SRC/TEST.C"),
            object: PathBuf::from("out/test.o"),
            address: start,
            load_address: start,
            size,
            group_size: size,
        }
    }
    #[test]
    fn linked_assembly_fallback_can_prove_an_entry_without_earning_c_credit() {
        let root = Path::new("/repo");
        let mut c = function("Matched", ".text", 0x08000100, 16);
        c.source = root.join(&c.source);
        let mut asm = function("Fallback", ".text", 0x08000110, 8);
        asm.source = root.join("recon/tbs/raw/08000110.s");
        let credits = credits(root, "main", &[c, asm]).unwrap();
        assert_eq!(credits.len(), 1);
        assert_eq!(credits[0].kind, "c");
        assert_eq!((credits[0].start, credits[0].end), (0x08000100, 0x08000110));
    }
    #[test]
    fn overlapping_attempts_keep_distinct_elf_bytes_and_only_whole_matches() {
        let sections = vec![
            Section {
                name: ".text.First".into(),
                size: 2,
                address: 100,
                load_address: 100,
            },
            Section {
                name: ".text.Second".into(),
                size: 4,
                address: 101,
                load_address: 101,
            },
        ];
        let bytes = parse_dump("Contents of section .text.First:\n 0064 0102 ..\nContents of section .text.Second:\n 0065 03040909 ....\n", &sections).unwrap();
        let (accepted, rejected) = select(
            &[
                function("First", ".text.First", 100, 2),
                function("Second", ".text.Second", 101, 4),
            ],
            &sections,
            &bytes,
            &[1, 2, 3, 4, 5],
            100,
        )
        .unwrap();
        assert_eq!(accepted.len(), 1);
        assert_eq!(rejected.len(), 1);
    }
    #[test]
    fn a_matching_instruction_prefix_does_not_excuse_a_different_literal_pool() {
        let bytes = BTreeMap::from([(".text.Pool".into(), vec![1, 2, 3, 9])]);
        assert!(select(
            &[function("Pool", ".text.Pool", 100, 2)],
            &[section(".text.Pool", 100, 4)],
            &bytes,
            &[1, 2, 3, 4],
            100
        )
        .is_err());
        let (accepted, rejected) = select(
            &[function("Pool", ".text.Pool", 100, 4)],
            &[section(".text.Pool", 100, 4)],
            &bytes,
            &[1, 2, 3, 4],
            100,
        )
        .unwrap();
        assert!(accepted.is_empty());
        assert_eq!(rejected.len(), 1);
    }
    #[test]
    fn fragments_and_two_exact_overlapping_functions_never_gain_credit() {
        let bytes = BTreeMap::from([
            (".text.Fragment".into(), vec![1, 2]),
            (".text.Other".into(), vec![2, 3]),
        ]);
        assert!(select(
            &[
                function("First", ".text.Fragment", 100, 2),
                function("Other", ".text.Other", 101, 2)
            ],
            &[
                section(".text.Fragment", 100, 2),
                section(".text.Other", 101, 2)
            ],
            &bytes,
            &[1, 2, 3],
            100
        )
        .is_err());
        assert!(select(
            &[function("Fragment_First", ".text.Fragment", 100, 2)],
            &[section(".text.Fragment", 100, 2)],
            &bytes,
            &[1, 2, 3],
            100
        )
        .unwrap()
        .0
        .is_empty());
    }
    #[test]
    fn a_whole_object_group_must_match_before_any_member_gets_credit() {
        let definitions = [section(".text.Group", 100, 8)];
        let mut first = function("First", ".text.Group", 100, 4);
        let mut second = function("Second", ".text.Group", 104, 4);
        first.group_size = 8;
        second.group_size = 8;
        let functions = [first, second];
        let bytes = BTreeMap::from([(".text.Group".into(), vec![1, 2, 3, 4, 5, 6, 7, 8])]);
        assert_eq!(
            select(
                &functions,
                &definitions,
                &bytes,
                &[1, 2, 3, 4, 5, 6, 7, 8],
                100
            )
            .unwrap()
            .0
            .len(),
            2
        );
        assert!(select(
            &functions,
            &definitions,
            &bytes,
            &[1, 2, 3, 4, 5, 6, 7, 9],
            100
        )
        .unwrap()
        .0
        .is_empty());
    }
}
