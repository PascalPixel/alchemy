//! W3's packer exception distinguishes a proven reused-buffer donor from
//! unused residue after a complete stream. Read the edition-selected source's
//! generated payloads and verify placement before proving either claim.
//! Neither the build nor this check reads the reference ROM for a proof.

use crate::elf::{Elf, Section};
use crate::targets::{target_for, TARGET_IDS};
use std::collections::BTreeMap;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check packer [--suggest]\n\
Checks PACKER byte rows and explicit source fill against selected payloads\n\
and the resource directory's reused output buffer, in all twelve builds.\n\
Rows name their last writer or declare unused post-stream residue; REASON.\n\
Unknown claims fail. --suggest prints TSV evidence for the last writer.\n\
Run make compare-editions first. This check never builds or reads a ROM.";

pub fn entry(args: &[String]) -> ExitCode {
    let suggest = match args {
        [] => false,
        [flag] if flag == "--suggest" => true,
        [flag] if flag == "--help" || flag == "-h" => {
            println!("{USAGE}");
            return ExitCode::SUCCESS;
        }
        _ => {
            eprintln!("{USAGE}");
            return ExitCode::from(2);
        }
    };
    if suggest {
        println!("source\tline\tedition\tlabel\toffset\twriter\twriter_bytes\trow_match");
    }
    match run(crate::compiler::routing::root(), suggest) {
        Ok(count) => {
            let summary = format!("packer: OK, {count} over twelve editions");
            if suggest {
                eprintln!("{summary}");
            } else {
                println!("{summary}");
            }
            ExitCode::SUCCESS
        }
        Err(error) => {
            eprintln!("error: {error}");
            ExitCode::FAILURE
        }
    }
}

#[derive(Debug, PartialEq, Eq)]
enum Claim {
    Donor { provider: String, offset: usize },
    UnusedResidue,
}

fn claim(comment: &str) -> Result<Claim, String> {
    if let Some(reason) = comment.trim().strip_prefix("unused post-stream residue;") {
        return if reason.trim().is_empty() {
            Err("unused post-stream residue needs its measured reason".into())
        } else {
            Ok(Claim::UnusedResidue)
        };
    }
    let named = comment
        .trim()
        .strip_prefix("left in the buffer by ")
        .ok_or("PACKER row needs a proven origin: left in the buffer by SYMBOL+0xOFFSET")?;
    let (provider, offset) = named
        .split_once("+0x")
        .ok_or("PACKER origin needs SYMBOL+0xOFFSET")?;
    if provider.is_empty()
        || provider.starts_with(|c: char| c.is_ascii_digit())
        || !provider
            .chars()
            .all(|c| c.is_ascii_alphanumeric() || c == '_')
    {
        return Err(format!("{provider:?} is not a provider symbol"));
    }
    Ok(Claim::Donor {
        provider: provider.into(),
        offset: usize::from_str_radix(offset.trim(), 16)
            .map_err(|_| format!("{offset:?} is not a hexadecimal buffer offset"))?,
    })
}

#[derive(Debug)]
struct Row {
    line: usize,
    offset: usize,
    bytes: Vec<u8>,
    comment: String,
}

#[derive(Debug)]
struct Fill {
    line: usize,
    offset: usize,
    end: usize,
    byte: u8,
}

#[derive(Debug)]
struct Payload {
    source: String,
    /// What this resource actually writes. Its tagged tail is never a write.
    bytes: Vec<u8>,
    /// Exactly one generated stream, without an emitted header or siblings.
    single_stream: bool,
    rows: Vec<Row>,
    fill: Option<Fill>,
    overlay: bool,
}

#[derive(Default)]
struct SourceBlock {
    label: Option<String>,
    bytes: Vec<u8>,
    generated: bool,
    single_stream: bool,
    unsupported: bool,
    rows: Vec<Row>,
    fill: Option<Fill>,
    overlay: bool,
    locals: BTreeMap<String, usize>,
    offsets: Vec<(usize, String)>,
    alignment: Option<(usize, u8)>,
}

impl SourceBlock {
    fn align(&mut self) {
        if let Some((alignment, fill)) = self.alignment.take() {
            if !self.bytes.len().is_multiple_of(alignment) {
                self.single_stream = false;
            }
            self.bytes
                .resize(self.bytes.len().next_multiple_of(alignment), fill);
        }
    }

    fn finish(
        &mut self,
        source: &str,
        found: &mut BTreeMap<String, Payload>,
    ) -> Result<(), String> {
        let mut block = std::mem::take(self);
        let Some(label) = block.label.take() else {
            return Ok(());
        };
        if block.generated && !block.unsupported {
            for (at, target) in block.offsets {
                let offset = block.locals.get(&target).ok_or_else(|| {
                    format!("{source}: {label}: missing asset sub-block {target}")
                })?;
                block.bytes[at..at + 4].copy_from_slice(&(*offset as u32).to_le_bytes());
            }
            found.insert(
                label,
                Payload {
                    source: source.into(),
                    bytes: block.bytes,
                    single_stream: block.single_stream,
                    rows: block.rows,
                    fill: block.fill,
                    overlay: block.overlay,
                },
            );
        } else if !block.rows.is_empty() || block.fill.is_some() {
            return Err(format!(
                "{source}: {label}: PACKER claim has no complete generated payload"
            ));
        }
        Ok(())
    }
}

/// Named byte ranges of the linked images, excluding absolute aliases,
/// mapping symbols and undefined names. No symbol's extent is guessed by
/// stripping trailing zeros.
struct Images {
    elves: Vec<Elf>,
    places: BTreeMap<String, Vec<(usize, u32)>>,
    starts: Vec<Vec<u32>>,
}

impl Images {
    fn load(root: &Path, output: &str, id: &str) -> Result<Self, String> {
        let directory = root.join(output);
        let mut paths = vec![directory.join(format!("{id}.elf"))];
        if let Ok(entries) = fs::read_dir(directory.join("overlays")) {
            let mut overlays: Vec<PathBuf> = entries
                .flatten()
                .map(|entry| entry.path())
                .filter(|path| path.extension().is_some_and(|ext| ext == "elf"))
                .collect();
            overlays.sort();
            paths.extend(overlays);
        }
        let elves = paths
            .iter()
            .map(|path| {
                let bytes = fs::read(path).map_err(|error| {
                    format!(
                        "{}: {error}; run make compare-editions first",
                        path.display()
                    )
                })?;
                Elf::parse(&bytes).map_err(|error| format!("{}: {error}", path.display()))
            })
            .collect::<Result<Vec<_>, _>>()?;
        Ok(Self::new(elves))
    }

    fn new(elves: Vec<Elf>) -> Self {
        let mut places: BTreeMap<String, Vec<(usize, u32)>> = BTreeMap::new();
        let mut starts = Vec::new();
        for (index, elf) in elves.iter().enumerate() {
            let mut here = Vec::new();
            for symbol in &elf.symbols {
                if matches!(symbol.section, 0 | 0xfff1)
                    || symbol.name.is_empty()
                    || symbol.name.starts_with('$')
                {
                    continue;
                }
                places
                    .entry(symbol.name.clone())
                    .or_default()
                    .push((index, symbol.value));
                here.push(symbol.value);
            }
            here.sort_unstable();
            here.dedup();
            starts.push(here);
        }
        Self {
            elves,
            places,
            starts,
        }
    }

    fn place(&self, name: &str) -> Result<Option<(usize, u32)>, String> {
        match self.places.get(name).map(Vec::as_slice) {
            None | Some([]) => Ok(None),
            Some([place]) => Ok(Some(*place)),
            Some(_) => Err(format!("{name} has several places in the linked images")),
        }
    }

    fn block(&self, name: &str) -> Result<Option<&[u8]>, String> {
        let Some((index, address)) = self.place(name)? else {
            return Ok(None);
        };
        let (section, at): (&Section, usize) = self.elves[index]
            .section_at(address)
            .ok_or_else(|| format!("{name} lies in no linked section"))?;
        let next = self.starts[index].partition_point(|start| *start <= address);
        let end = self.starts[index]
            .get(next)
            .map_or(section.bytes.len(), |start| {
                start.saturating_sub(section.address) as usize
            })
            .min(section.bytes.len());
        Ok(Some(&section.bytes[at..end]))
    }

    /// Names at each directory address. A repeated pointer is not another
    /// packing job: it names the same resource already placed once.
    fn directory(&self) -> Result<Vec<Vec<String>>, String> {
        let table = self
            .block("Resource_DirectoryTable")?
            .ok_or("Resource_DirectoryTable is absent from the built image")?;
        if !table.len().is_multiple_of(4) {
            return Err("Resource_DirectoryTable has a partial pointer".into());
        }
        let mut names: BTreeMap<u32, Vec<String>> = BTreeMap::new();
        for (name, places) in &self.places {
            if let [(0, address)] = places.as_slice() {
                names.entry(*address).or_default().push(name.clone());
            }
        }
        let mut seen = std::collections::BTreeSet::new();
        Ok(table
            .as_chunks::<4>()
            .0
            .iter()
            .filter_map(|word| {
                let address = u32::from_le_bytes(*word);
                seen.insert(address)
                    .then(|| names.get(&address).cloned().unwrap_or_default())
            })
            .collect())
    }
}

fn byte_values(arguments: &str) -> Result<Vec<u8>, String> {
    arguments
        .split(',')
        .map(|word| {
            let word = word.trim();
            let value = match word.strip_prefix("0x") {
                Some(hex) => i16::from_str_radix(hex, 16),
                None => word.parse::<i16>(),
            }
            .ok()
            .filter(|value| (-128..=255).contains(value));
            value
                .map(|value| value as u8)
                .ok_or_else(|| format!("{word:?} is not a byte"))
        })
        .collect()
}

/// Only full generated resource files may establish a buffer write. In
/// particular a baserom slice, arbitrary out/ report or private file cannot.
fn generated_name(name: &str) -> bool {
    if name
        .split('/')
        .any(|part| part.is_empty() || part == "." || part == "..")
    {
        return false;
    }
    ags::resource::is_recipe(name)
        || name
            .strip_prefix("overlays/resource_")
            .and_then(|rest| rest.strip_suffix(".lz"))
            .is_some_and(|id| !id.is_empty() && id.bytes().all(|b| b.is_ascii_hexdigit()))
}

fn editable(root: &Path, path: &Path) -> Result<Vec<u8>, String> {
    if !path.starts_with("games")
        || path
            .components()
            .any(|part| !matches!(part, std::path::Component::Normal(_)))
        || !path
            .extension()
            .is_some_and(|extension| matches!(extension.to_str(), Some("png" | "tsv" | "patch")))
    {
        return Err(format!(
            "{} is not an editable asset input under games/",
            path.display()
        ));
    }
    declared_read(root, path)
}

fn declared_read(root: &Path, path: &Path) -> Result<Vec<u8>, String> {
    if path
        .components()
        .any(|part| !matches!(part, std::path::Component::Normal(_)))
    {
        return Err(format!(
            "{} is not a declared relative input",
            path.display()
        ));
    }
    let canonical_root = root
        .canonicalize()
        .map_err(|error| format!("{}: {error}", root.display()))?;
    let expected = canonical_root.join(path);
    let resolved = expected
        .canonicalize()
        .map_err(|error| format!("{}: {error}", path.display()))?;
    // Reject file and directory aliases, even inside games/. An editable
    // name must identify the input itself, not a symlink to private bytes.
    if resolved != expected {
        return Err(format!(
            "{}: symlink input paths are unsupported",
            path.display()
        ));
    }
    fs::read(resolved).map_err(|error| format!("{}: {error}", path.display()))
}

/// Match build_overlay's objcopy, stored Thumb calls and one packer rule.
/// Cached .lz bytes never participate in this reconstruction.
fn overlay_stream(root: &Path, output: &Path, name: &str) -> Result<Vec<u8>, String> {
    let elf = output.join(name).with_extension("elf");
    let temporary = tempfile::tempdir().map_err(|error| error.to_string())?;
    let image = temporary.path().join("overlay.bin");
    crate::process::run(
        &[
            "arm-none-eabi-objcopy".into(),
            "-O".into(),
            "binary".into(),
            elf.to_string_lossy().into_owned(),
            image.to_string_lossy().into_owned(),
        ],
        root,
    )?;
    let mut block = fs::read(image).map_err(|error| error.to_string())?;
    let half = |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
    let mut at = 2;
    while at + 2 <= block.len() {
        let low = half(&block, at);
        let high = half(&block, at - 2);
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
    ags::lz::compress_tagged(&block, &ags::resource::PACKER)
}

fn include_name(line: &str) -> Option<&str> {
    let (directive, arguments) = line.trim().split_once(char::is_whitespace)?;
    (directive == ".include")
        .then(|| arguments.trim().strip_prefix('"')?.strip_suffix('"'))
        .flatten()
}

fn cpp_directive(line: &str) -> bool {
    let line = line.trim_start();
    line.starts_with('#') || line.starts_with("%:") || line.starts_with("??=")
}

fn cpp_directives(text: &str) -> Vec<usize> {
    let mut logical = String::new();
    let mut starts = Vec::new();
    let mut start = 0;
    let mut continued = false;
    for (number, row) in text.lines().enumerate() {
        if let Some(row) = row.strip_suffix('\\') {
            logical.push_str(row);
            continued = true;
        } else {
            logical.push_str(row);
            logical.push('\n');
            starts.push(start);
            start = number + 1;
            continued = false;
        }
    }
    if continued {
        logical.push('\n');
        starts.push(start);
    }
    crate::compiler::assembly_source::without_comments(&logical)
        .lines()
        .enumerate()
        .filter_map(|(number, row)| cpp_directive(row).then_some(starts[number]))
        .collect()
}

/// Give the existing selector only safe source and freshly generated
/// includes. Missing or unsupported inputs fail if their include is active.
fn prepare_includes(
    text: &str,
    root: &Path,
    output: &Path,
    private: &Path,
    read: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    seen: &mut std::collections::BTreeSet<String>,
) -> Result<(), String> {
    let visible = crate::compiler::assembly_source::without_comments(text);
    for name in visible.lines().filter_map(include_name) {
        let path = Path::new(name);
        if path
            .components()
            .any(|part| !matches!(part, std::path::Component::Normal(_)))
            || !seen.insert(name.into())
        {
            continue;
        }
        let bytes = if path.starts_with("games")
            && path.extension().is_some_and(|extension| {
                matches!(extension.to_str(), Some("s" | "S" | "inc" | "h"))
            }) {
            declared_read(root, path)
        } else if ags::resource::is_recipe(name) {
            read(name)
        } else if name == "text/messages.inc" {
            declared_read(output, path)
        } else {
            continue;
        };
        let Ok(bytes) = bytes else { continue };
        let Ok(text) = std::str::from_utf8(&bytes) else {
            continue;
        };
        let destination = private.join(path);
        fs::create_dir_all(destination.parent().unwrap()).map_err(|error| error.to_string())?;
        fs::write(destination, &bytes).map_err(|error| error.to_string())?;
        prepare_includes(text, root, output, private, read, seen)?;
    }
    Ok(())
}

fn selected_source(
    text: &str,
    source: &str,
    edition: &str,
    root: &Path,
    output: &Path,
    read: &dyn Fn(&str) -> Result<Vec<u8>, String>,
) -> Result<String, String> {
    let private = tempfile::tempdir().map_err(|error| error.to_string())?;
    prepare_includes(
        text,
        root,
        output,
        private.path(),
        read,
        &mut std::collections::BTreeSet::new(),
    )?;
    let mut expanded = text.to_owned();
    let mut origins: Vec<(String, usize, bool)> = text
        .lines()
        .enumerate()
        .map(|(number, _)| (source.into(), number + 1, false))
        .collect();
    loop {
        let selected = crate::compiler::assembly_source::selected_includes(
            &expanded,
            edition,
            &[private.path()],
        )?;
        let rows: Vec<_> = selected.lines().collect();
        if let Some(number) = cpp_directives(&selected).first() {
            let (file, line, _) = &origins[*number];
            return Err(format!("{file}:{line}: unsupported preprocessed directive"));
        }
        for (row, (file, line, included)) in rows.iter().zip(&origins) {
            let visible = crate::compiler::assembly_source::without_comments(row);
            if row.contains("PACKER:") && *included {
                return Err(format!(
                    "{file}:{line}: active included PACKER claim is unsupported ({source})"
                ));
            }
            if row.contains("PACKER:") && include_name(visible.trim()).is_some() {
                return Err(format!(
                    "{file}:{line}: unsupported tagged or preprocessed include"
                ));
            }
        }
        let visible = crate::compiler::assembly_source::without_comments(&selected);
        let Some((number, name)) = visible
            .lines()
            .enumerate()
            .find_map(|(number, line)| include_name(line).map(|name| (number, name)))
        else {
            // The payload parser still reads the original selected source;
            // only claim coverage needs the inline view and its origins.
            return crate::compiler::assembly_source::selected_includes(
                text,
                edition,
                &[private.path()],
            );
        };
        let included = fs::read_to_string(private.path().join(name))
            .map_err(|error| format!("{name}: {error}"))?;
        let mut next = rows[..number]
            .iter()
            .map(|row| (*row).to_owned())
            .collect::<Vec<_>>();
        let count = included.lines().count();
        next.extend(included.lines().map(str::to_owned));
        next.extend(rows[number + 1..].iter().map(|row| (*row).to_owned()));
        origins.splice(
            number..number + 1,
            (1..=count).map(|line| (name.into(), line, true)),
        );
        expanded = next.join("\n");
        expanded.push('\n');
    }
}

fn source_fill(directive: &str, arguments: &str, tag: &str) -> Option<u8> {
    let fill = match tag.trim() {
        "zero fill, not an earlier stream's bytes" => 0,
        "0xff fill, not an earlier stream's bytes" => 0xff,
        _ => return None,
    };
    let fields: Vec<_> = arguments.split(',').map(str::trim).collect();
    (directive == ".balign"
        && fields.len() == 2
        && fields[0].parse::<usize>().ok() == Some(4)
        && byte_values(fields[1]).ok().as_deref() == Some(&[fill]))
    .then_some(fill)
}

/// Collect the actual selected generated payload before each PACKER row.
/// These file lengths establish the stream boundary, including any source
/// prefix such as an inline palette. Literal leftovers are kept separate.
fn payloads_collect(
    text: &str,
    source: &str,
    images: &Images,
    read: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    failures: &mut Vec<String>,
) -> Result<BTreeMap<String, Payload>, String> {
    let mut found = BTreeMap::new();
    let mut block = SourceBlock::default();
    for (number, line) in text.lines().enumerate() {
        let (code, comment) = line
            .split_once('@')
            .map_or((line.trim(), ""), |(code, comment)| {
                (code.trim(), comment.trim())
            });
        let (directive, arguments) = code.split_once(char::is_whitespace).unwrap_or((code, ""));
        let mut tag = comment.strip_prefix("PACKER:");
        let mut fill = tag.and_then(|tag| source_fill(directive, arguments, tag));
        if line.contains("PACKER:")
            && (tag.is_none() || (directive != ".byte" && fill.is_none()) || block.label.is_none())
        {
            failures.push(format!(
                "{source}:{}: PACKER tag needs a .byte row or explicit source fill in a linked generated payload",
                number + 1
            ));
            // The annotation fails even when its directive is ordinary
            // alignment. Keep collecting other supported byte claims.
            if directive == ".byte" {
                block.unsupported = true;
            }
            tag = None;
            fill = None;
        }
        if let Some(name) = code
            .strip_suffix(':')
            .filter(|name| !name.is_empty() && !name.contains(char::is_whitespace))
        {
            if name.starts_with(".L") && block.label.is_some() {
                if block.fill.is_some() {
                    return Err(format!(
                        "{source}:{}: data follows tagged source fill",
                        number + 1
                    ));
                }
                block.align();
                block.locals.insert(name.into(), block.bytes.len());
                continue;
            }
            block.finish(source, &mut found)?;
            // Linked selection matters: an unlinked duplicate must not prove a row.
            if images.places.contains_key(name) {
                block.label = Some(name.into());
            }
            continue;
        }
        if directive == ".section" {
            block.finish(source, &mut found)?;
        }
        if block.label.is_none() {
            continue;
        }
        if let Some(fill) = fill {
            if block.fill.is_some() || block.alignment.is_some() {
                block.unsupported = true;
            }
            let offset = block
                .rows
                .last()
                .map_or(block.bytes.len(), |row| row.offset + row.bytes.len());
            let (_, address) = images.place(block.label.as_deref().unwrap())?.unwrap();
            let address = address as usize;
            block.fill = Some(Fill {
                line: number + 1,
                offset,
                end: (address + offset).next_multiple_of(4) - address,
                byte: fill,
            });
        } else if let (".byte", Some(tag)) = (directive, tag) {
            if block.fill.is_some() {
                return Err(format!(
                    "{source}:{}: data follows tagged source fill",
                    number + 1
                ));
            }
            let offset = block
                .rows
                .last()
                .map_or(block.bytes.len(), |row| row.offset + row.bytes.len());
            block.rows.push(Row {
                line: number + 1,
                offset,
                bytes: byte_values(arguments)?,
                comment: tag.trim().into(),
            });
        } else if directive == ".incbin" {
            let name = arguments
                .trim()
                .strip_prefix('"')
                .and_then(|name| name.strip_suffix('"'));
            if let Some(name) = name.filter(|name| generated_name(name)) {
                if !block.rows.is_empty() || block.fill.is_some() {
                    return Err(format!(
                        "{source}:{}: generated data follows tagged tail",
                        number + 1
                    ));
                }
                block.align();
                block.single_stream =
                    !block.generated && block.bytes.is_empty() && name.ends_with(".lz");
                block.bytes.extend(read(name)?);
                block.generated = true;
                block.overlay |= name.starts_with("overlays/");
            } else {
                block.unsupported = true;
            }
        } else if matches!(directive, ".byte" | "map_header" | "map_layer")
            && block.rows.is_empty()
            && block.fill.is_none()
        {
            match byte_values(arguments) {
                Ok(literals)
                    if directive == ".byte"
                        || literals.len() == if directive == "map_header" { 12 } else { 8 } =>
                {
                    block.align();
                    block.single_stream = false;
                    block.bytes.extend(literals);
                }
                _ => block.unsupported = true,
            }
        } else if directive == "map_block" && block.rows.is_empty() && block.fill.is_none() {
            let fields: Vec<&str> = arguments.split(',').map(str::trim).collect();
            if fields.first().copied() != block.label.as_deref() || fields.len() > 2 {
                block.unsupported = true;
            } else {
                block.align();
                block.single_stream = false;
                if let Some(target) = fields.get(1) {
                    block.offsets.push((block.bytes.len(), (*target).into()));
                }
                block.bytes.extend([0; 4]);
            }
        } else if matches!(directive, ".balign" | ".align") && block.rows.is_empty() {
            let fields: Vec<&str> = arguments.split(',').map(str::trim).collect();
            let alignment = fields[0]
                .parse::<u32>()
                .ok()
                .and_then(|value| {
                    if directive == ".align" {
                        1usize.checked_shl(value)
                    } else {
                        Some(value as usize)
                    }
                })
                .filter(|value| value.is_power_of_two());
            let fill = fields.get(1).map_or(Ok(vec![0]), |fill| byte_values(fill));
            if let (Some(alignment), Ok(fill)) = (alignment, fill) {
                block.alignment = Some((alignment, fill[0]));
            } else {
                block.unsupported = true;
            }
        } else if !directive.is_empty()
            && !matches!(
                directive,
                ".global" | ".type" | ".size" | ".balign" | ".align" | ".set" | ".equ" | ".equiv"
            )
        {
            block.unsupported = true;
        }
    }
    block.finish(source, &mut found)?;
    Ok(found)
}

/// Validate the emitted prefix and tail at their only possible offset;
/// there is no backwards byte search and no trailing-zero heuristic.
fn placed(payload: &Payload, block: &[u8]) -> Result<(), String> {
    if !block.starts_with(&payload.bytes) {
        return Err(
            "selected generated payload differs from its linked prefix; rebuild this edition"
                .into(),
        );
    }
    let mut offset = payload.bytes.len();
    for row in &payload.rows {
        let end = row.offset + row.bytes.len();
        if row.offset != offset
            || row.bytes.is_empty()
            || end > payload.bytes.len().next_multiple_of(4)
        {
            return Err("PACKER row is not the stream's remaining four-byte padding".into());
        }
        if block.get(row.offset..end) != Some(&row.bytes) {
            return Err(format!(
                "PACKER row is absent at the generated stream end +0x{offset:x}"
            ));
        }
        offset = end;
    }
    if let Some(fill) = &payload.fill {
        if fill.offset != offset
            || block.len() != fill.end
            || block
                .get(fill.offset..fill.end)
                .is_none_or(|bytes| bytes.iter().any(|byte| *byte != fill.byte))
        {
            return Err(format!(
                "tagged source fill at +0x{:x} differs from its linked alignment boundary or bytes",
                fill.offset
            ));
        }
    } else if !payload.rows.is_empty()
        && (block.len() != payload.bytes.len().next_multiple_of(4)
            || block[offset..].iter().any(|byte| *byte != 0))
    {
        return Err("PACKER row has further data beyond its alignment".into());
    }
    Ok(())
}

/// A single resource's last writer at the claimed buffer offset. Every
/// intervening resource matters even if it happens to write the same byte.
fn last_writer<'a>(
    images: &Images,
    order: &'a [Vec<String>],
    payloads: &BTreeMap<String, Payload>,
    label: &str,
    offset: usize,
) -> Result<(&'a str, &'a [String]), String> {
    let own = order
        .iter()
        .position(|names| names.iter().any(|name| name == label))
        .ok_or_else(|| {
            format!("{label} is not in the resource directory; its packing order is unproven")
        })?;
    for names in order[..own].iter().rev() {
        if let Some(name) = names.iter().find(|name| payloads.contains_key(*name)) {
            if payloads[name].bytes.len() > offset {
                return Ok((name, names));
            }
        } else if let Some(name) = names.first() {
            if images
                .block(name)?
                .is_some_and(|block| block.len() > offset)
            {
                return Err(format!(
                    "{name} reaches +0x{offset:x} but has no source-generated buffer-write extent"
                ));
            }
        } else {
            return Err(
                "an unnamed earlier directory resource has an unknown buffer-write extent".into(),
            );
        }
    }
    Err(format!(
        "no earlier source-generated resource wrote +0x{offset:x}"
    ))
}

/// Semantic input is confined to the generated prefix. Unlike the ARM
/// reservoir's halfword prefetch, every bit obtained here affects a token.
/// Once its zero seven-bit long length ends the stream, arbitrary later
/// bytes cannot affect the output or its length (decode_01.s).
struct PrefixBits<'a> {
    bytes: &'a [u8],
    at: usize,
}

impl PrefixBits<'_> {
    fn get(&mut self, count: usize) -> Result<usize, String> {
        let mut value = 0;
        for bit in 0..count {
            let byte = self
                .bytes
                .get(self.at / 8)
                .ok_or("kind-zero stream needs suffix bits before its native terminator")?;
            value |= usize::from((byte >> (self.at % 8)) & 1) << bit;
            self.at += 1;
        }
        Ok(value)
    }
}

fn terminated_general(prefix: &[u8]) -> Result<Vec<u8>, String> {
    if prefix.first() != Some(&0) {
        return Err("unused residue requires a generated kind-zero stream".into());
    }
    let mut bits = PrefixBits {
        bytes: prefix,
        at: 8,
    };
    let mut output = Vec::new();
    loop {
        if bits.get(1)? != 0 {
            output.push(bits.get(8)? as u8);
            continue;
        }
        let mut ones = 0;
        while ones < 5 && bits.get(1)? != 0 {
            ones += 1;
        }
        let length = match ones {
            0..=3 => ones + 2,
            4 => 6 + bits.get(1)?,
            _ => match bits.get(2)? {
                0 => match bits.get(7)? {
                    0 => {
                        if bits.at.div_ceil(8) != prefix.len() {
                            return Err(
                                "native terminator ends before the generated prefix boundary"
                                    .into(),
                            );
                        }
                        return Ok(output);
                    }
                    long => long + 10,
                },
                short => short + 7,
            },
        };
        let distance = if bits.get(1)? != 0 {
            bits.get(5)? + 1
        } else {
            let width = output
                .len()
                .checked_sub(33)
                .filter(|window| *window < 2048)
                .map_or(12, |window| (usize::BITS - window.leading_zeros()) as usize);
            bits.get(width)? + 33
        };
        if distance > output.len() {
            return Err("kind-zero stream has an invalid back-reference".into());
        }
        for _ in 0..length {
            output.push(output[output.len() - distance]);
        }
    }
}

fn unused_residue(payload: &Payload) -> Result<(), String> {
    if !payload.single_stream {
        return Err("unused residue requires exactly one complete generated stream".into());
    }
    let decoded = terminated_general(&payload.bytes)?;
    // The prefix-only token proof above covers every possible replacement,
    // including mixed suffix bytes. These contrasting patterns independently
    // cross-check its token semantics against AGS's reservoir decoder. Extra
    // guard bytes allow a native halfword prefetch past the packing boundary;
    // neither those bits nor the returned input cursor supply the proof.
    for pattern in [0x00, 0xff, 0x55, 0xaa] {
        let mut input = payload.bytes.clone();
        input.resize(input.len().next_multiple_of(4) + 2, pattern);
        let (actual, _) =
            ags::assets::lz::decode_general(&input, 0, input.len(), decoded.len() as u64)
                .map_err(|error| error.to_string())?;
        if actual != decoded {
            return Err("kind-zero decoded bytes or output length depend on the suffix".into());
        }
    }
    Ok(())
}

#[cfg(test)]
fn prove(
    images: &Images,
    order: &[Vec<String>],
    payloads: &BTreeMap<String, Payload>,
    label: &str,
) -> Result<(), String> {
    for row in &payloads[label].rows {
        prove_row(images, order, payloads, label, row)?;
    }
    Ok(())
}

fn prove_row(
    images: &Images,
    order: &[Vec<String>],
    payloads: &BTreeMap<String, Payload>,
    label: &str,
    row: &Row,
) -> Result<(), String> {
    let payload = &payloads[label];
    placed(
        payload,
        images
            .block(label)?
            .ok_or("tagged resource is not linked")?,
    )?;
    let Claim::Donor {
        provider: name,
        offset: claimed_offset,
    } = claim(&row.comment)?
    else {
        let (_, address) = images
            .place(label)?
            .ok_or("tagged resource is not linked")?;
        if !address.is_multiple_of(4) {
            return Err("unused residue stream is not four-byte aligned".into());
        }
        return unused_residue(payload);
    };
    let offset = row.offset;
    if offset != claimed_offset {
        return Err(format!(
            "row starts at +0x{offset:x}, not the claimed +0x{:x}",
            claimed_offset
        ));
    }
    let (writer, names) = last_writer(images, order, payloads, label, offset)?;
    if !names.contains(&name) {
        return Err(format!(
            "{writer} was the last writer at +0x{offset:x}, not {}",
            name
        ));
    }
    let provider = &payloads[writer];
    placed(
        provider,
        images.block(writer)?.ok_or("provider is not linked")?,
    )?;
    let end = offset + row.bytes.len();
    let held = provider
        .bytes
        .get(offset..end)
        .ok_or_else(|| format!("{writer} did not write the row's whole extent"))?;
    if held != row.bytes {
        return Err(format!(
            "{writer}+0x{offset:x} wrote {held:02x?}, not {:02x?}",
            row.bytes
        ));
    }
    Ok(())
}

#[derive(Default)]
struct Proven {
    donors: usize,
    residues: usize,
    fills: usize,
}

impl std::fmt::Display for Proven {
    fn fmt(&self, formatter: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            formatter,
            "{} named byte rows proven, {} unused residue rows proven, {} source fills proven",
            self.donors, self.residues, self.fills
        )
    }
}

fn run(root: &Path, suggest: bool) -> Result<Proven, String> {
    let linked = crate::dead::Linked::read(root)?;
    let mut failures = Vec::new();
    let mut source_failures = Vec::new();
    let mut proven = Proven::default();
    let encoded = std::cell::RefCell::new(BTreeMap::<(PathBuf, String), Vec<u8>>::new());
    for id in TARGET_IDS {
        let target = target_for(id);
        let output = root.join(target.output_dir);
        let images = Images::load(root, target.output_dir, id.as_str())?;
        let mut all = BTreeMap::new();
        let files = &linked
            .files
            .iter()
            .find(|(edition, _)| edition.id == id)
            .unwrap()
            .1;
        for source in files.iter().filter(|source| {
            source.starts_with("games/") && (source.ends_with(".s") || source.ends_with(".S"))
        }) {
            let text = declared_read(root, Path::new(source))
                .and_then(|bytes| String::from_utf8(bytes).map_err(|error| error.to_string()))
                .map_err(|error| format!("{source}: {error}"))?;
            if !text.contains(".incbin")
                && !text.contains("PACKER:")
                && !text.contains(".include")
                && cpp_directives(&text).is_empty()
            {
                continue;
            }
            let read = |name: &str| {
                if name.starts_with("overlays/") {
                    let key = (output.join(name).with_extension("elf"), "overlay".into());
                    if let Some(bytes) = encoded.borrow().get(&key) {
                        return Ok(bytes.clone());
                    }
                    let bytes = overlay_stream(root, &output, name)?;
                    encoded.borrow_mut().insert(key, bytes.clone());
                    return Ok(bytes);
                }
                // The build deletes temporary generated asset files after
                // linking. Rebuild these bytes in memory from the same recipe,
                // so a stale ELF or an edited input cannot silently prove a tag.
                let (game, recipe) = match name.strip_prefix("common/") {
                    Some(recipe) => ("games/common", recipe),
                    None if source.starts_with("games/common/") => ("games/common", name),
                    None => (target.game_dir(), name),
                };
                let input = Path::new(game)
                    .join("src")
                    .join(ags::resource::input_name(recipe)?);
                let key = (input.clone(), recipe.to_owned());
                if let Some(bytes) = encoded.borrow().get(&key) {
                    return Ok(bytes.clone());
                }
                let bytes = editable(root, &input)?;
                let bytes = ags::resource::build_file_with(recipe, &bytes, &|name| {
                    let sibling = ags::resource::sibling_path(&input, name)?;
                    editable(root, &sibling)
                })?;
                encoded.borrow_mut().insert(key, bytes.clone());
                Ok(bytes)
            };
            let selected =
                match selected_source(&text, source, target.edition_define, root, &output, &read) {
                    Ok(selected) => selected,
                    Err(error) => {
                        source_failures.push(format!("{source} ({id}): {error}"));
                        continue;
                    }
                };
            if !selected.contains(".incbin") && !selected.contains("PACKER:") {
                continue;
            }
            let mut diagnostics = Vec::new();
            let payloads = payloads_collect(&selected, source, &images, &read, &mut diagnostics);
            source_failures.extend(
                diagnostics
                    .into_iter()
                    .map(|error| format!("{error} ({id})")),
            );
            let payloads = match payloads {
                Ok(payloads) => payloads,
                Err(error) => {
                    source_failures.push(format!("{source} ({id}): {error}"));
                    continue;
                }
            };
            for (name, payload) in payloads {
                if payload.overlay
                    && images
                        .block(&name)?
                        .is_none_or(|block| !block.starts_with(&payload.bytes))
                {
                    source_failures.push(format!(
                        "{source} ({id}, {name}): regenerated overlay stream differs from its linked resource prefix"
                    ));
                }
                if all.insert(name.clone(), payload).is_some() {
                    return Err(format!(
                        "{name} has several selected generated definitions ({id})"
                    ));
                }
            }
        }
        if !all
            .values()
            .any(|payload| !payload.rows.is_empty() || payload.fill.is_some())
        {
            continue;
        }
        let order = images.directory()?;
        for (label, payload) in &all {
            if let Some(fill) = &payload.fill {
                let result = placed(
                    payload,
                    images
                        .block(label)?
                        .ok_or("tagged resource is not linked")?,
                );
                if let Err(error) = result {
                    source_failures.push(format!(
                        "{}:{} ({id}, {label}): {error}",
                        payload.source, fill.line
                    ));
                } else {
                    proven.fills += 1;
                }
            }
            for row in &payload.rows {
                if let Err(error) = prove_row(&images, &order, &all, label, row) {
                    let at = format!("{}:{} ({id}, {label})", payload.source, row.line);
                    failures.push(format!("{at}: {error}"));
                    if suggest {
                        if let Ok((writer, _)) =
                            last_writer(&images, &order, &all, label, row.offset)
                        {
                            let Some(block) = images.block(writer)? else {
                                continue;
                            };
                            if placed(
                                payload,
                                images
                                    .block(label)?
                                    .ok_or("tagged resource is not linked")?,
                            )
                            .is_err()
                                || placed(&all[writer], block).is_err()
                            {
                                continue;
                            }
                            let end = row.offset + row.bytes.len();
                            let held = all[writer].bytes.get(row.offset..end).unwrap_or(&[]);
                            let matched = if held == row.bytes {
                                "same"
                            } else if held.len() == row.bytes.len() {
                                "different"
                            } else {
                                "short"
                            };
                            let bytes = held
                                .iter()
                                .map(|byte| format!("{byte:02x}"))
                                .collect::<Vec<_>>()
                                .join(" ");
                            println!(
                                "{}\t{}\t{id}\t{label}\t0x{:x}\t{writer}\t{bytes}\t{matched}",
                                payload.source, row.line, row.offset
                            );
                        }
                    }
                } else {
                    match claim(&row.comment)? {
                        Claim::Donor { .. } => proven.donors += 1,
                        Claim::UnusedResidue => proven.residues += 1,
                    }
                }
            }
        }
    }
    if failures.is_empty() && source_failures.is_empty() {
        Ok(proven)
    } else {
        Err(format!(
            "packer: {proven}, {} byte rows rejected, {} source claims rejected over twelve editions\n{}",
            failures.len(),
            source_failures.len(),
            failures
                .into_iter()
                .chain(source_failures)
                .collect::<Vec<_>>()
                .join("\n")
        ))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::elf::Symbol;

    fn payloads(
        text: &str,
        source: &str,
        images: &Images,
        read: &dyn Fn(&str) -> Result<Vec<u8>, String>,
    ) -> Result<BTreeMap<String, Payload>, String> {
        let mut failures = Vec::new();
        let found = payloads_collect(text, source, images, read, &mut failures)?;
        if failures.is_empty() {
            Ok(found)
        } else {
            Err(failures.join("\n"))
        }
    }

    fn image(blocks: &[(&str, Vec<u8>)]) -> Images {
        let base = 0x0800_0000;
        let mut bytes = Vec::new();
        let mut symbols = Vec::new();
        let mut add = |name: &str, value| {
            symbols.push(Symbol {
                name: name.into(),
                value,
                size: 0,
                kind: 1,
                bind: 1,
                section: 1,
            })
        };
        add("Resource_DirectoryTable", base);
        let mut at = base + 4 * blocks.len() as u32;
        for (name, block) in blocks {
            add(name, at);
            bytes.extend(at.to_le_bytes());
            at += block.len().next_multiple_of(4) as u32;
        }
        for (_, block) in blocks {
            bytes.extend(block);
            bytes.resize(bytes.len().next_multiple_of(4), 0);
        }
        add("End", at);
        Images::new(vec![Elf {
            sections: vec![Section {
                address: base,
                bytes,
            }],
            symbols,
        }])
    }

    fn payload(bytes: &[u8], row: Option<(&[u8], &str)>) -> Payload {
        Payload {
            source: "games/tbs/src/a.s".into(),
            bytes: bytes.into(),
            single_stream: true,
            fill: None,
            overlay: false,
            rows: row
                .into_iter()
                .map(|(row_bytes, comment)| Row {
                    line: 1,
                    offset: bytes.len(),
                    bytes: row_bytes.into(),
                    comment: comment.into(),
                })
                .collect(),
        }
    }

    #[test]
    fn proof_requires_the_last_writer_even_when_bytes_coincide() {
        let images = image(&[
            ("First", vec![1, 2, 3, 4, 5, 6]),
            ("Second", vec![9, 9, 9, 4, 5]),
            ("Own", vec![7, 7, 7, 4]),
        ]);
        let mut payloads = BTreeMap::from([
            ("First".into(), payload(&[1, 2, 3, 4, 5, 6], None)),
            ("Second".into(), payload(&[9, 9, 9, 4, 5], None)),
            (
                "Own".into(),
                payload(&[7, 7, 7], Some((&[4], "left in the buffer by Second+0x3"))),
            ),
        ]);
        let order = images.directory().unwrap();
        assert_eq!(prove(&images, &order, &payloads, "Own"), Ok(()));
        payloads.get_mut("Own").unwrap().rows[0].comment = "left in the buffer by First+0x3".into();
        assert!(prove(&images, &order, &payloads, "Own")
            .unwrap_err()
            .contains("last writer"));
    }

    #[test]
    fn emitted_stream_length_keeps_zeros_and_rejects_coincidental_offsets() {
        let images = image(&[("First", vec![1, 2, 3, 0, 5, 6]), ("Own", vec![7, 7, 0, 0])]);
        let mut payloads = BTreeMap::from([
            ("First".into(), payload(&[1, 2, 3, 0, 5, 6], None)),
            (
                "Own".into(),
                payload(&[7, 7, 0], Some((&[0], "left in the buffer by First+0x3"))),
            ),
        ]);
        assert_eq!(
            prove(&images, &images.directory().unwrap(), &payloads, "Own"),
            Ok(())
        );
        payloads.get_mut("Own").unwrap().rows[0].comment = "left in the buffer by First+0x2".into();
        assert!(
            prove(&images, &images.directory().unwrap(), &payloads, "Own")
                .unwrap_err()
                .contains("row starts")
        );
    }

    #[test]
    fn origins_must_be_generated_ordered_and_complete() {
        let images = image(&[
            ("First", vec![1, 2, 3]),
            ("Unknown", vec![1, 2, 3, 4, 5]),
            ("Own", vec![7, 7, 7, 4]),
        ]);
        let payloads = BTreeMap::from([
            ("First".into(), payload(&[1, 2, 3], None)),
            (
                "Own".into(),
                payload(&[7, 7, 7], Some((&[4], "left in the buffer by First+0x3"))),
            ),
        ]);
        assert!(
            prove(&images, &images.directory().unwrap(), &payloads, "Own")
                .unwrap_err()
                .contains("no source-generated")
        );
        assert!(claim("left in the buffer, unverified: why").is_err());
        assert!(claim("left in the buffer by earlier streams, which differ by edition").is_err());
        assert!(!generated_name("baserom.gba"));
        assert!(!generated_name("overlays/../roms/tbs-en.gba"));
    }

    #[test]
    fn source_selection_controls_the_prefix_and_tail() {
        let images = image(&[("Own", vec![9, 8, 7, 6])]);
        let text = "Own:\n.incbin \"graphics/a.gbapal\"\n.ifdef TBS_EDITION_EN\n.incbin \"graphics/a.4bpp.lz\"\n.byte 6 @ PACKER: left in the buffer by First+0x3\n.else\n.incbin \"graphics/b.4bpp.lz\"\n.byte 1 @ PACKER: left in the buffer by First+0x3\n.endif\n";
        let selected =
            crate::compiler::assembly_source::selected_includes(text, "TBS_EDITION_EN", &[])
                .unwrap();
        let read = |name: &str| match name {
            "graphics/a.gbapal" => Ok(vec![9]),
            "graphics/a.4bpp.lz" => Ok(vec![8, 7]),
            _ => panic!("inactive edition file read"),
        };
        let found = payloads(&selected, "games/tbs/src/a.s", &images, &read).unwrap();
        assert_eq!(found["Own"].bytes, vec![9, 8, 7]);
        assert_eq!(
            placed(&found["Own"], images.block("Own").unwrap().unwrap()),
            Ok(())
        );
        assert!(placed(&found["Own"], &[9, 8, 0, 6]).is_err());
    }

    #[test]
    fn a_compound_asset_writes_its_header_offsets_and_internal_alignment() {
        let mut bytes = vec![0; 12];
        bytes.extend(20u32.to_le_bytes());
        bytes.extend(24u32.to_le_bytes());
        bytes.extend([1, 2, 3, 0, 4, 5]);
        let images = image(&[("Map", bytes.clone()), ("Table", vec![0; 32])]);
        let text = "Map:\nmap_header 0,0,0,0,0,0,0,0,0,0,0,0\nmap_block Map, .LFirst\nmap_block Map, .LSecond\n.LFirst:\n.incbin \"map/a.table.lz\"\n.balign 4, 0\n.LSecond:\n.incbin \"map/b.table.lz\"\n.balign 4, 0\nTable:\n.byte 0,0,0,0\n";
        let read = |name: &str| match name {
            "map/a.table.lz" => Ok(vec![1, 2, 3]),
            "map/b.table.lz" => Ok(vec![4, 5]),
            _ => panic!("unknown source asset"),
        };
        let found = payloads(text, "games/tbs/src/map/a.s", &images, &read).unwrap();
        assert_eq!(found["Map"].bytes, bytes);
        assert!(
            !found.contains_key("Table"),
            "a plain structure is not a packer writer"
        );
        assert_eq!(
            placed(&found["Map"], images.block("Map").unwrap().unwrap()),
            Ok(())
        );
    }

    #[test]
    fn actual_ags_streams_prove_a_reused_buffer_tail() {
        let data: Vec<u8> = (0u32..128)
            .map(|n| (n.wrapping_mul(2_654_435_761) >> 13) as u8)
            .collect();
        let provider = ags::lz::compress_general(&data, &ags::resource::PACKER).unwrap();
        let own = (1..20)
            .map(|len| ags::lz::compress_general(&data[..len], &ags::resource::PACKER).unwrap())
            .find(|stream| !stream.len().is_multiple_of(4))
            .unwrap();
        let offset = own.len();
        let tail = provider[offset..offset.next_multiple_of(4)].to_vec();
        let shipped: Vec<u8> = own.iter().chain(&tail).copied().collect();
        let images = image(&[("Provider", provider.clone()), ("Own", shipped)]);
        let payloads = BTreeMap::from([
            ("Provider".into(), payload(&provider, None)),
            (
                "Own".into(),
                payload(
                    &own,
                    Some((
                        &tail,
                        &format!("left in the buffer by Provider+0x{offset:x}"),
                    )),
                ),
            ),
        ]);
        assert_eq!(
            prove(&images, &images.directory().unwrap(), &payloads, "Own"),
            Ok(())
        );
    }

    #[test]
    fn unused_residue_proves_termination_without_a_donor_or_directory_entry() {
        let decoded = b"ABCDE";
        let prefix = ags::lz::compress_general(decoded, &ags::resource::PACKER).unwrap();
        assert_eq!(prefix.len() % 4, 1);
        let comment = "unused post-stream residue; native decoder returns only decoded RAM length";
        let tail = vec![0x51, 0x05, 0x08];
        let images = image(&[("Own", [prefix.clone(), tail.clone()].concat())]);
        let text = format!(
            "Own:\n.incbin \"graphics/a.4bpp.lz\"\n.byte 0x51,0x05,0x08 @ PACKER: {comment}\n.balign 4,0\n"
        );
        let found = payloads(&text, "games/tbs/src/a.s", &images, &|_| Ok(prefix.clone())).unwrap();
        assert!(found["Own"].single_stream);
        assert_eq!(prove(&images, &[], &found, "Own"), Ok(()));
        assert_eq!(terminated_general(&prefix).unwrap(), decoded);

        // Exercise mixed replacements, not only the uniform cross-checks.
        // The universal proof is the prefix-only reader's bit boundary.
        for at in 0..tail.len() {
            for value in 0..=u8::MAX {
                let mut input = [prefix.clone(), tail.clone(), vec![0xa5; 2]].concat();
                input[prefix.len() + at] = value;
                assert_eq!(
                    ags::assets::lz::decode_general(&input, 0, input.len(), decoded.len() as u64)
                        .unwrap()
                        .0,
                    decoded
                );
            }
        }
        assert!(claim("unused post-stream residue;").is_err());
        assert!(claim("unused post-stream residue").is_err());
        assert!(claim("left in the buffer by earlier streams, which differ by edition").is_err());
    }

    #[test]
    fn residue_rejects_wrong_kind_missing_or_early_terminator_and_bad_copies() {
        let prefix = ags::lz::compress_general(b"ABCDE", &ags::resource::PACKER).unwrap();
        let mut wrong_kind = prefix.clone();
        wrong_kind[0] = 1;
        assert!(terminated_general(&wrong_kind)
            .unwrap_err()
            .contains("kind-zero"));
        assert!(terminated_general(&prefix[..prefix.len() - 1])
            .unwrap_err()
            .contains("needs suffix bits"));
        let mut extended = prefix.clone();
        extended.push(0);
        assert!(terminated_general(&extended)
            .unwrap_err()
            .contains("before"));
        // Copy length two/distance one before any decoded byte exists.
        assert!(terminated_general(&[0, 4, 0])
            .unwrap_err()
            .contains("invalid back-reference"));
        let decoded: Vec<_> = (0..4096).map(|n| (n % 251) as u8).collect();
        let copies = ags::lz::compress_general(&decoded, &ags::resource::PACKER).unwrap();
        assert_eq!(terminated_general(&copies).unwrap(), decoded);
        assert_eq!(unused_residue(&payload(&copies, None)), Ok(()));
    }

    #[test]
    fn residue_requires_a_single_generated_stream_and_only_packing_suffix() {
        let prefix = ags::lz::compress_general(b"ABCDE", &ags::resource::PACKER).unwrap();
        let tag = "unused post-stream residue; decoded output only";
        for (body, bytes) in [
            (
                ".byte 0\n.incbin \"graphics/a.4bpp.lz\"",
                [vec![0], prefix.clone()].concat(),
            ),
            (
                ".incbin \"graphics/a.4bpp.lz\"\n.byte 0",
                [prefix.clone(), vec![0]].concat(),
            ),
            (
                ".incbin \"graphics/a.4bpp.lz\"\n.incbin \"graphics/a.4bpp.lz\"",
                [prefix.clone(), prefix.clone()].concat(),
            ),
        ] {
            let mut placed_bytes = bytes;
            placed_bytes.push(7);
            placed_bytes.resize(placed_bytes.len().next_multiple_of(4), 0);
            let images = image(&[("Own", placed_bytes)]);
            let text = format!("Own:\n{body}\n.byte 7 @ PACKER: {tag}\n");
            let found =
                payloads(&text, "games/tbs/src/a.s", &images, &|_| Ok(prefix.clone())).unwrap();
            assert!(!found["Own"].single_stream);
            assert!(prove(&images, &[], &found, "Own")
                .unwrap_err()
                .contains("exactly one"));
        }
        let own = payload(&prefix, Some((&[1, 2, 3, 4], tag)));
        let images = image(&[("Own", [prefix.clone(), vec![1, 2, 3, 4]].concat())]);
        let found = BTreeMap::from([("Own".into(), own)]);
        assert!(prove(&images, &[], &found, "Own")
            .unwrap_err()
            .contains("four-byte padding"));

        // A raw table can coincidentally parse as kind zero. Its structural
        // consumer is not the native compressed-stream decoder.
        let images = image(&[("Own", [prefix.clone(), vec![1, 2, 3]].concat())]);
        let text = format!("Own:\n.incbin \"graphics/a.table\"\n.byte 1,2,3 @ PACKER: {tag}\n");
        let found = payloads(&text, "games/tbs/src/a.s", &images, &|_| Ok(prefix.clone())).unwrap();
        assert!(prove(&images, &[], &found, "Own")
            .unwrap_err()
            .contains("exactly one"));
    }

    #[test]
    fn recipe_siblings_cannot_read_private_or_generated_inputs() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        fs::create_dir_all(root.join("games/tbs/src")).unwrap();
        fs::write(root.join("games/tbs/src/table.tsv"), "value:u8\n7\n").unwrap();
        assert!(editable(root, Path::new("games/tbs/src/table.tsv")).is_ok());
        for forbidden in [
            "roms/tbs-en.gba",
            "out/tbs-en.gba",
            "games/tbs/src/data.gba",
            "games/tbs/src/../../../../roms/tbs-en.gba",
            "games/tbs/src/../table.tsv",
        ] {
            assert!(editable(root, Path::new(forbidden))
                .unwrap_err()
                .contains("not an editable"));
        }
    }

    #[cfg(unix)]
    #[test]
    fn recipe_symlinks_cannot_supply_inputs() {
        use std::os::unix::fs::symlink;

        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        fs::create_dir_all(root.join("games/tbs/src")).unwrap();
        fs::create_dir_all(root.join("private")).unwrap();
        fs::write(root.join("private/answer.gba"), [1, 2, 3]).unwrap();
        fs::write(root.join("private/table.tsv"), "value:u8\n7\n").unwrap();
        fs::write(root.join("games/tbs/src/table.tsv"), "value:u8\n8\n").unwrap();
        symlink(
            root.join("private/answer.gba"),
            root.join("games/tbs/src/file.tsv"),
        )
        .unwrap();
        symlink(root.join("private"), root.join("games/tbs/src/directory")).unwrap();
        symlink(
            root.join("games/tbs/src/table.tsv"),
            root.join("games/tbs/src/alias.tsv"),
        )
        .unwrap();
        for alias in ["file.tsv", "directory/table.tsv", "alias.tsv"] {
            let input = Path::new("games/tbs/src").join(alias);
            assert!(editable(root, &input).unwrap_err().contains("symlink"));
        }
        assert_eq!(
            editable(root, Path::new("games/tbs/src/table.tsv")).unwrap(),
            b"value:u8\n8\n"
        );
    }

    #[test]
    fn included_claims_keep_parent_symbols_and_edition_selection() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        fs::create_dir_all(root.join("games/tbs/src")).unwrap();
        fs::write(
            root.join("games/tbs/src/parent.inc"),
            ".include \"games/tbs/src/child.h\"\n",
        )
        .unwrap();
        fs::write(
            root.join("games/tbs/src/child.h"),
            ".if ENABLE\n.ifdef TBS_EDITION_EN\n.byte 4 @ PACKER: left in the buffer by First+0x3\n.endif\n.endif\n",
        )
        .unwrap();
        let text = ".set ENABLE, 1\n.include \"games/tbs/src/parent.inc\"\n";
        assert!(!text.contains(".incbin") && !text.contains("PACKER:"));
        let read = |_: &str| -> Result<Vec<u8>, String> { panic!("not a generated include") };
        let select = |text: &str, edition| {
            selected_source(text, "games/tbs/src/wrapper.s", edition, root, root, &read)
        };
        assert!(select(text, "TBS_EDITION_EN")
            .unwrap_err()
            .contains("games/tbs/src/child.h:3: active included PACKER"));
        assert!(select(text, "TBS_EDITION_JA").is_ok());
        assert!(select(&text.replace("ENABLE, 1", "ENABLE, 0"), "TBS_EDITION_EN").is_ok());
        fs::write(
            root.join("games/tbs/src/definitions.inc"),
            ".set ENABLE, 1\n",
        )
        .unwrap();
        let inherited = ".include \"games/tbs/src/definitions.inc\"\n.if ENABLE\n.include \"games/tbs/src/parent.inc\"\n.endif\n";
        assert!(select(inherited, "TBS_EDITION_EN")
            .unwrap_err()
            .contains("child.h:3: active included PACKER"));
        fs::write(
            root.join("games/tbs/src/parent.inc"),
            ".include \"games/tbs/src/parent.inc\"\n",
        )
        .unwrap();
        assert!(select(text, "TBS_EDITION_EN")
            .unwrap_err()
            .contains("recursive GAS include"));
    }

    #[test]
    fn generated_and_cpp_includes_cannot_hide_claims() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let read = |name: &str| {
            assert_eq!(name, "graphics/foo.spriteblocks");
            Ok(b".ifdef TBS_EDITION_EN\n.byte 1 @ PACKER: unknown\n.endif\n".to_vec())
        };
        let text = ".include \"graphics/foo.spriteblocks\"\n";
        assert!(selected_source(
            text,
            "games/tbs/src/wrapper.s",
            "TBS_EDITION_EN",
            root,
            root,
            &read
        )
        .unwrap_err()
        .contains("graphics/foo.spriteblocks:2: active included PACKER"));
        assert!(selected_source(
            text,
            "games/tbs/src/wrapper.s",
            "TBS_EDITION_JA",
            root,
            root,
            &read
        )
        .is_ok());
        for directive in [
            "#include",
            "# include",
            "#include_next",
            "#import",
            "#/**/include",
            "%:include",
            "??=include",
            "#\\\ninclude",
            "#inc\\\nlude",
            "#\\\r\ninclude",
            "#inc\\\r\nlude",
        ] {
            let cpp = format!("{directive} \"games/tbs/src/hidden.h\"\n");
            assert!(!cpp_directives(&cpp).is_empty());
            assert!(selected_source(
                &cpp,
                "games/tbs/src/wrapper.S",
                "TBS_EDITION_EN",
                root,
                root,
                &read
            )
            .unwrap_err()
            .contains("unsupported preprocessed directive"));
        }
        let unsafe_include = ".include \"../private.inc\"\n";
        assert!(selected_source(
            unsafe_include,
            "games/tbs/src/wrapper.s",
            "TBS_EDITION_EN",
            root,
            root,
            &read
        )
        .unwrap_err()
        .contains("missing GAS include"));
    }

    #[test]
    fn an_emitting_include_cannot_shorten_an_intervening_writer() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        fs::create_dir_all(root.join("games/tbs/src")).unwrap();
        fs::write(root.join("games/tbs/src/writer.inc"), ".byte 8\n").unwrap();
        let images = image(&[
            ("First", vec![1, 2, 3, 7]),
            ("Second", vec![4, 5, 6, 8]),
            ("Own", vec![9, 9, 9, 7]),
        ]);
        let text = "First:\n.incbin \"graphics/first.4bpp.lz\"\nSecond:\n.incbin \"graphics/second.4bpp.lz\"\n.include \"games/tbs/src/writer.inc\"\nOwn:\n.incbin \"graphics/own.4bpp.lz\"\n.byte 7 @ PACKER: left in the buffer by First+0x3\n";
        let read = |name: &str| match name {
            "graphics/first.4bpp.lz" => Ok(vec![1, 2, 3, 7]),
            "graphics/second.4bpp.lz" => Ok(vec![4, 5, 6]),
            "graphics/own.4bpp.lz" => Ok(vec![9, 9, 9]),
            _ => panic!("unknown source asset"),
        };
        let selected = selected_source(
            text,
            "games/tbs/src/a.s",
            "TBS_EDITION_EN",
            root,
            root,
            &read,
        )
        .unwrap();
        let found = payloads(&selected, "games/tbs/src/a.s", &images, &read).unwrap();
        assert!(!found.contains_key("Second"));
        assert!(prove(&images, &images.directory().unwrap(), &found, "Own")
            .unwrap_err()
            .contains("no source-generated buffer-write extent"));
    }

    #[test]
    fn cached_overlay_bytes_cannot_prove_the_linked_prefix() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let output = root.join("out");
        fs::create_dir_all(output.join("overlays")).unwrap();
        let source = root.join("overlay.s");
        let object = root.join("overlay.o");
        let elf = output.join("overlays/resource_1.elf");
        fs::write(
            &source,
            ".section .text\n.global Overlay\nOverlay:\n.hword 0xf000, 0xf802\n.byte 1,2,3,4,5\n",
        )
        .unwrap();
        crate::process::run(
            &[
                "arm-none-eabi-as".into(),
                "-o".into(),
                object.to_string_lossy().into_owned(),
                source.to_string_lossy().into_owned(),
            ],
            root,
        )
        .unwrap();
        crate::process::run(
            &[
                "arm-none-eabi-ld".into(),
                "-Ttext=0x02008000".into(),
                "-e".into(),
                "Overlay".into(),
                "-o".into(),
                elf.to_string_lossy().into_owned(),
                object.to_string_lossy().into_owned(),
            ],
            root,
        )
        .unwrap();
        let generated = overlay_stream(root, &output, "overlays/resource_1.lz").unwrap();
        assert_eq!(
            generated,
            ags::lz::compress_tagged(&[0, 0xf0, 3, 0xf8, 1, 2, 3, 4, 5], &ags::resource::PACKER)
                .unwrap()
        );
        let mut tampered = generated.clone();
        tampered[0] ^= 1;
        fs::write(output.join("overlays/resource_1.lz"), &tampered).unwrap();
        let read = |name: &str| overlay_stream(root, &output, name);
        let text = "Own:\n.incbin \"overlays/resource_1.lz\"\n";
        let stale = image(&[("Own", tampered)]);
        let found = payloads(text, "games/tbs/src/overlays.s", &stale, &read).unwrap();
        assert!(found["Own"].overlay);
        assert!(placed(&found["Own"], stale.block("Own").unwrap().unwrap())
            .unwrap_err()
            .contains("selected generated payload differs"));
        fs::remove_file(output.join("overlays/resource_1.lz")).unwrap();
        let current = image(&[("Own", generated)]);
        let found = payloads(text, "games/tbs/src/overlays.s", &current, &read).unwrap();
        assert_eq!(
            placed(&found["Own"], current.block("Own").unwrap().unwrap()),
            Ok(())
        );
    }

    #[test]
    fn unsupported_packer_tags_cannot_disappear() {
        let images = image(&[("Own", vec![1, 2, 3, 4])]);
        let read = |_: &str| Ok(vec![1, 2, 3]);
        for row in [
            ".word 4 @ PACKER: left in the buffer by First+0x3",
            ".balign 4, 0 @ PACKER: zero fill",
            ".balign 4, 0 @ PACKER: left in the buffer by First+0x3",
            "@ PACKER: left in the buffer by First+0x3",
            ".byte 4 @ prose PACKER: left in the buffer by First+0x3",
            ".byte 4 /* PACKER: left in the buffer by First+0x3 */",
            "/* PACKER: left in the buffer by First+0x3 */",
            "Unknown:\n.byte 4 @ PACKER: left in the buffer by First+0x3",
        ] {
            let text = format!("Own:\n.incbin \"graphics/a.4bpp.lz\"\n{row}\n");
            assert!(payloads(&text, "games/tbs/src/a.s", &images, &read)
                .unwrap_err()
                .contains("PACKER tag needs a .byte row"));
        }
        let unowned = ".byte 4 @ PACKER: left in the buffer by First+0x3\n";
        assert!(payloads(unowned, "games/tbs/src/a.s", &images, &read).is_err());
        let unsupported = "Own:\n.incbin \"graphics/a.4bpp.lz\"\n.word 0\n.byte 4 @ PACKER: left in the buffer by First+0x3\n";
        assert!(payloads(unsupported, "games/tbs/src/a.s", &images, &read)
            .unwrap_err()
            .contains("no complete generated payload"));
    }

    #[test]
    fn unsupported_alignment_is_reported_without_hiding_byte_claims() {
        let images = image(&[("First", vec![1, 2, 3, 4]), ("Own", vec![7, 8, 9, 4])]);
        let text = "First:\n.incbin \"graphics/first.4bpp.lz\"\n.balign 4, 0 @ PACKER: zero fill\nOwn:\n.incbin \"graphics/own.4bpp.lz\"\n.byte 4 @ PACKER: left in the buffer by First+0x3\n";
        let read = |name: &str| match name {
            "graphics/first.4bpp.lz" => Ok(vec![1, 2, 3, 4]),
            "graphics/own.4bpp.lz" => Ok(vec![7, 8, 9]),
            _ => panic!("unknown source asset"),
        };
        let mut failures = Vec::new();
        let found =
            payloads_collect(text, "games/tbs/src/a.s", &images, &read, &mut failures).unwrap();
        assert_eq!(failures.len(), 1);
        assert!(failures[0].contains("a.s:3: PACKER tag needs a .byte row"));
        assert_eq!(
            prove(&images, &images.directory().unwrap(), &found, "Own"),
            Ok(())
        );
    }

    #[test]
    fn explicit_source_fill_is_placed_but_never_a_buffer_write() {
        for (fill, comment) in [
            (0, "zero fill, not an earlier stream's bytes"),
            (0xff, "0xff fill, not an earlier stream's bytes"),
        ] {
            let images = image(&[
                ("First", vec![1, 2, 3, 7]),
                ("Filler", vec![4, 5, 6, fill]),
                ("Own", vec![9, 9, 9, 7]),
            ]);
            let text = format!("First:\n.incbin \"graphics/first.4bpp.lz\"\nFiller:\n.incbin \"graphics/filler.4bpp.lz\"\n.balign 4, {fill} @ PACKER: {comment}\nOwn:\n.incbin \"graphics/own.4bpp.lz\"\n.byte 7 @ PACKER: left in the buffer by First+0x3\n");
            let read = |name: &str| match name {
                "graphics/first.4bpp.lz" => Ok(vec![1, 2, 3, 7]),
                "graphics/filler.4bpp.lz" => Ok(vec![4, 5, 6]),
                "graphics/own.4bpp.lz" => Ok(vec![9, 9, 9]),
                _ => panic!("unknown source asset"),
            };
            let found = payloads(&text, "games/tbs/src/a.s", &images, &read).unwrap();
            assert_eq!(found["Filler"].bytes, vec![4, 5, 6]);
            assert!(found["Filler"].rows.is_empty());
            assert_eq!(
                placed(&found["Filler"], images.block("Filler").unwrap().unwrap()),
                Ok(())
            );
            assert!(placed(&found["Filler"], &[4, 5, 6, fill ^ 0xff]).is_err());
            assert!(placed(&found["Filler"], &[4, 5, 6, fill, 0]).is_err());
            assert_eq!(
                prove(&images, &images.directory().unwrap(), &found, "Own"),
                Ok(())
            );
            for operands in [
                format!("4, {}", fill ^ 0xff),
                format!("8, {fill}"),
                format!("4, {fill}, 1"),
            ] {
                let bad = text.replace(
                    &format!(".balign 4, {fill}"),
                    &format!(".balign {operands}"),
                );
                assert!(payloads(&bad, "games/tbs/src/a.s", &images, &read).is_err());
            }
            let following = text.replace("\nOwn:", "\n.byte 0\nOwn:");
            assert!(payloads(&following, "games/tbs/src/a.s", &images, &read).is_err());
        }
    }

    #[test]
    fn adjacent_tail_rows_can_have_different_proven_origins() {
        let images = image(&[
            ("First", vec![1, 2, 4, 6]),
            ("Second", vec![5, 6, 9]),
            ("Own", vec![7, 8, 9, 6]),
        ]);
        let text = "First:\n.incbin \"graphics/first.4bpp.lz\"\nSecond:\n.incbin \"graphics/second.4bpp.lz\"\n.balign 4, 0\nOwn:\n.incbin \"graphics/own.4bpp.lz\"\n.byte 9 @ PACKER: left in the buffer by Second+0x2\n.byte 6 @ PACKER: left in the buffer by First+0x3\n";
        let read = |name: &str| match name {
            "graphics/first.4bpp.lz" => Ok(vec![1, 2, 4, 6]),
            "graphics/second.4bpp.lz" => Ok(vec![5, 6, 9]),
            "graphics/own.4bpp.lz" => Ok(vec![7, 8]),
            _ => panic!("unknown source asset"),
        };
        let mut found = payloads(text, "games/tbs/src/a.s", &images, &read).unwrap();
        assert_eq!(
            found["Own"]
                .rows
                .iter()
                .map(|row| row.offset)
                .collect::<Vec<_>>(),
            vec![2, 3]
        );
        assert_eq!(
            prove(&images, &images.directory().unwrap(), &found, "Own"),
            Ok(())
        );
        found.get_mut("Own").unwrap().rows[1].comment = "left in the buffer by Second+0x3".into();
        assert!(prove(&images, &images.directory().unwrap(), &found, "Own")
            .unwrap_err()
            .contains("First was the last writer"));
    }
}
