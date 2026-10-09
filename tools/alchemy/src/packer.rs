//! W3's packer exception is a claim about a reused output buffer, not a
//! search for equal bytes. Read the edition-selected source's generated
//! payloads, verify their placement, then follow resource-directory order.
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
Each row names its last writer: left in the buffer by SYMBOL+0xOFFSET.\n\
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
            let summary = format!("packer: OK, {count} tagged claims proven over twelve editions");
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
struct Claim {
    provider: String,
    offset: usize,
}

fn claim(comment: &str) -> Result<Claim, String> {
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
    Ok(Claim {
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
    rows: Vec<Row>,
    fill: Option<Fill>,
}

#[derive(Default)]
struct SourceBlock {
    label: Option<String>,
    bytes: Vec<u8>,
    generated: bool,
    unsupported: bool,
    rows: Vec<Row>,
    fill: Option<Fill>,
    locals: BTreeMap<String, usize>,
    offsets: Vec<(usize, String)>,
    alignment: Option<(usize, u8)>,
}

impl SourceBlock {
    fn align(&mut self) {
        if let Some((alignment, fill)) = self.alignment.take() {
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
                    rows: block.rows,
                    fill: block.fill,
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
            "{} is not an editable asset input: symlink paths under games/ are unsupported",
            path.display()
        ));
    }
    fs::read(resolved).map_err(|error| format!("{}: {error}", path.display()))
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
                block.bytes.extend(read(name)?);
                block.generated = true;
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
                ".global"
                    | ".type"
                    | ".size"
                    | ".include"
                    | ".balign"
                    | ".align"
                    | ".set"
                    | ".equ"
                    | ".equiv"
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
    let claim = claim(&row.comment)?;
    let offset = row.offset;
    if offset != claim.offset {
        return Err(format!(
            "row starts at +0x{offset:x}, not the claimed +0x{:x}",
            claim.offset
        ));
    }
    let (writer, names) = last_writer(images, order, payloads, label, offset)?;
    if !names.contains(&claim.provider) {
        return Err(format!(
            "{writer} was the last writer at +0x{offset:x}, not {}",
            claim.provider
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

fn run(root: &Path, suggest: bool) -> Result<usize, String> {
    let linked = crate::dead::Linked::read(root)?;
    let mut failures = Vec::new();
    let mut source_failures = Vec::new();
    let mut proven = 0;
    let mut fills_proven = 0;
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
            let text = fs::read_to_string(root.join(source))
                .map_err(|error| format!("{source}: {error}"))?;
            if !text.contains(".incbin") && !text.contains("PACKER:") {
                continue;
            }
            let selected = crate::compiler::assembly_source::selected_includes(
                &text,
                target.edition_define,
                &[root, &output],
            )
            .map_err(|error| format!("{source} ({id}): {error}"))?;
            let read = |name: &str| {
                if name.starts_with("overlays/") {
                    return fs::read(output.join(name)).map_err(|error| {
                        format!("{source}: {name} ({id}): {error}; rebuild this edition")
                    });
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
                    fills_proven += 1;
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
                    proven += 1;
                }
            }
        }
    }
    if failures.is_empty() && source_failures.is_empty() {
        Ok(proven + fills_proven)
    } else {
        Err(format!(
            "packer: {proven} byte rows proven, {fills_proven} source fills proven, {} byte rows rejected, {} source claims rejected over twelve editions\n{}",
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
            fill: None,
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
