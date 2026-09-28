//! Executable ranges derived from the approved image and maintained build inputs.
//!
//! This reader grants no credit. Code/data classifications are made afresh;
//! neither historical totals nor a saved inventory can supply a denominator.
//! A main-image gap remains unknown even when a private ROM composition matches.

use super::model::{bytes, normalize, subtract, Span};
use crate::overlay::assembly::{compiler_runtime_spans, OVERLAY_BASE, ROM_BASE};
use crate::overlay::flow::{alignment_spans, overlay_code, veneer_spans};
use crate::overlay::rom::{resource_table, CanonicalRom};
use crate::targets::DecompTarget;
use std::collections::BTreeSet;
use std::path::{Path, PathBuf};

const HEADER_BYTES: i64 = 0xc0;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum State {
    Exact,
    Unknown,
}

#[derive(Debug)]
pub(crate) struct Image {
    pub id: String,
    pub domain: Span,
    pub executable: Vec<Span>,
    pub excluded: Vec<Span>,
    pub unknown: Vec<Span>,
    pub reasons: Vec<String>,
}

impl Image {
    pub fn state(&self) -> State {
        if self.unknown.is_empty() && self.reasons.is_empty() {
            State::Exact
        } else {
            State::Unknown
        }
    }

    pub fn executable_bytes(&self) -> i64 {
        bytes(&self.executable)
    }
}

#[derive(Debug)]
pub(crate) struct Accounting {
    pub target: String,
    pub rom_sha256: String,
    pub main: Image,
    pub overlays: Vec<Image>,
}

impl Accounting {
    pub fn state(&self) -> State {
        if self.main.state() == State::Exact
            && self
                .overlays
                .iter()
                .all(|image| image.state() == State::Exact)
        {
            State::Exact
        } else {
            State::Unknown
        }
    }

    pub fn executable_bytes(&self) -> Option<i64> {
        (self.state() == State::Exact).then(|| {
            self.main.executable_bytes()
                + self
                    .overlays
                    .iter()
                    .map(Image::executable_bytes)
                    .sum::<i64>()
        })
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub(crate) enum SectionKind {
    /// A complete linked executable extent, including its pools and tables.
    Executable,
    Data,
    /// The maintained input explicitly establishes that these bytes never run.
    Padding,
}

/// Evidence for one main-image classification. Linked bytes must come from
/// the independently inspected build artifact; its producer verifies the
/// source, compiler route and complete extent before calling this reader.
/// No generic "verified" flag or caller-provided count grants authority.
#[derive(Debug)]
pub(crate) enum SectionProof {
    Linked(Vec<u8>),
    EncodedResource(usize),
    ResourceDirectory,
    SourceData(super::main_data::Kind),
}

#[derive(Debug)]
pub(crate) struct MainSection {
    pub span: Span,
    pub kind: SectionKind,
    pub source: PathBuf,
    pub proof: SectionProof,
}

/// Classifications supplied by a maintained layout, not by the complement of
/// the code compiled so far. Parsed resource streams and directory words are
/// data evidence even while the compressor cannot reproduce an editable asset.
#[derive(Debug, Default)]
pub(crate) struct MainLayout {
    pub sections: Vec<MainSection>,
    pub overlay_entries: Vec<OverlayEntry>,
}

/// A whole, freshly verified native function. `address` is a decoded resource
/// coordinate; an ELF runtime VMA is normalized by the loader's 0x8000
/// displacement before constructing this evidence. The source-build verifier
/// establishes the compiler route, natural symbol and complete section first.
#[derive(Debug)]
pub(crate) struct OverlayEntry {
    pub image: String,
    pub address: i64,
    pub source: PathBuf,
    pub compiled: Vec<u8>,
}

/// Read the target's approved ROM, derive every code-overlay classification,
/// and validate the main classifications. Unclassified main bytes keep the
/// whole denominator unknown. The caller independently reviews the overlay
/// method and its excluded bytes before adopting this measurement path.
pub(crate) fn derive(
    root: &Path,
    target: DecompTarget,
    layout: &MainLayout,
) -> Result<Accounting, String> {
    let rom = CanonicalRom::load_target(root, target)?;
    crate::text_catalog::verify_reference(root, target.id.as_str(), rom.bytes())?;
    let typed_data = super::main_data::read(root, target, &rom);
    let mut overlays = Vec::new();
    let mut used_entries = BTreeSet::new();
    let entries = &layout.overlay_entries;
    for resource in rom.overlay_resources(target.overlay_entry_veneers) {
        let id = format!("resource_{resource:03x}");
        let decoded = rom.overlay(&id)?;
        let runtime = crate::compiler::overlay::load(&decoded, 0)?;
        let mut seeds = Vec::new();
        let mut source_problems = Vec::new();
        for (index, entry) in entries
            .iter()
            .enumerate()
            .filter(|(_, entry)| entry.image == id)
        {
            validate_source(root, &entry.source)?;
            seeds.push(validate_overlay_entry(&runtime, entry)?);
            used_entries.insert(index);
        }
        let raw = target.overlay_assembly(&id);
        let path = root.join(&raw);
        if path.is_file() {
            validate_source(root, Path::new(&raw))?;
            let source = std::fs::read_to_string(&path).map_err(|error| error.to_string())?;
            let (retained, unmapped) = declared_overlay_entries(&source);
            seeds.extend(retained);
            source_problems.extend(unmapped.into_iter().map(|name| {
                format!("{raw}: declared function {name} needs a verified linked address")
            }));
        }
        seeds.sort_unstable();
        seeds.dedup();
        let mut image =
            classify_overlay_from_entries(&id, &decoded, target.overlay_entry_veneers, &seeds)?;
        image.reasons.extend(source_problems);
        overlays.push(image);
    }
    if used_entries.len() != entries.len() {
        return Err("source function evidence names an unknown code overlay".into());
    }
    let mut sections = Vec::new();
    for section in &layout.sections {
        validate_source(root, &section.source)?;
        validate_section(&rom, section, &typed_data.ranges)?;
        sections.push((section.span, section.kind));
    }
    for range in &typed_data.ranges {
        let section = MainSection {
            span: range.span,
            kind: SectionKind::Data,
            source: range.source.clone(),
            proof: SectionProof::SourceData(range.kind),
        };
        validate_source(root, &section.source)?;
        validate_section(&rom, &section, &typed_data.ranges)?;
        sections.push((section.span, section.kind));
    }
    let mut main = classify_main(
        Span::new(ROM_BASE + HEADER_BYTES, ROM_BASE + rom.bytes().len() as i64),
        &sections,
    )?;
    if !main.unknown.is_empty() {
        main.reasons.extend(typed_data.issues);
    }
    Ok(Accounting {
        target: target.id.as_str().into(),
        rom_sha256: crate::compiler::sha256::hex(rom.bytes()),
        main,
        overlays,
    })
}

fn validate_source(root: &Path, source: &Path) -> Result<(), String> {
    if source.is_absolute()
        || source
            .components()
            .any(|part| part == std::path::Component::ParentDir)
    {
        return Err("main classification needs a repository input path".into());
    }
    let Some(first) = source.components().next() else {
        return Err("main classification has no source".into());
    };
    if !["games", "recon", "tools"]
        .iter()
        .any(|prefix| first.as_os_str() == *prefix)
        || source.starts_with("tools/out")
    {
        return Err("generated output cannot establish a main classification".into());
    }
    let root = std::fs::canonicalize(root).map_err(|error| error.to_string())?;
    let input = std::fs::canonicalize(root.join(source)).map_err(|error| error.to_string())?;
    if !input.starts_with(&root)
        || !input.is_file()
        || input.starts_with(root.join("out"))
        || input.starts_with(root.join("tools/out"))
    {
        return Err("main classification source escapes the repository".into());
    }
    Ok(())
}

fn resource_span(rom: &CanonicalRom, resource: usize) -> Result<Span, String> {
    if resource < 2 {
        return Err("resource directory metadata is not a stream".into());
    }
    let start = rom.resource_pointer(resource)?;
    if native_veneer_head(&rom.bytes()[start..]) {
        return Err("native veneer bank is not proven compressed data".into());
    }
    // Directory order is logical: aliases and backwards entries do not
    // supply physical bounds. The next greater distinct address does.
    let limit = (0..rom.resource_count())
        .map(|id| rom.resource_pointer(id))
        .collect::<Result<Vec<_>, _>>()?
        .into_iter()
        .filter(|offset| *offset > start)
        .min()
        .unwrap_or(rom.bytes().len());
    let (_, encoded_bytes) = crate::build_assets::tagged_extent(rom.bytes(), start, limit)?;
    let end = start
        .checked_add(encoded_bytes)
        .ok_or("resource extent overflow")?;
    Ok(Span::new(ROM_BASE + start as i64, ROM_BASE + end as i64))
}

fn native_veneer_head(bytes: &[u8]) -> bool {
    bytes.get(..4) == Some(&[0x00, 0x4c, 0x20, 0x47])
        && bytes.get(4..8).is_some_and(|word| {
            matches!(
                u32::from_le_bytes(word.try_into().unwrap()) >> 24,
                0x02 | 0x03 | 0x08 | 0x09
            )
        })
}

fn validate_overlay_entry(runtime: &[u8], entry: &OverlayEntry) -> Result<i64, String> {
    let offset = entry
        .address
        .checked_sub(OVERLAY_BASE)
        .and_then(|offset| usize::try_from(offset).ok())
        .ok_or("source function entry is outside the decoded overlay")?;
    let end = offset
        .checked_add(entry.compiled.len())
        .ok_or("source function extent overflow")?;
    if entry.compiled.is_empty()
        || entry.address % 2 != 0
        || runtime.get(offset..end) != Some(entry.compiled.as_slice())
    {
        return Err("source function extent differs from the approved loaded overlay".into());
    }
    Ok(entry.address)
}

/// Normal source declarations may establish unused functions that no call or
/// pointer reaches. Address-spelled aliases place retained scaffolding; named
/// functions without such an alias require fresh linker evidence. A C hole
/// or a `.space` record never becomes code by acquiring a label.
fn declared_overlay_entries(source: &str) -> (Vec<i64>, Vec<String>) {
    let mut function_types = BTreeSet::new();
    let mut thumb = false;
    let mut names = Vec::<String>::new();
    let mut candidate = false;
    let mut seeds = Vec::new();
    let mut unmapped = Vec::new();
    for line in source.lines() {
        let line = line.split(';').next().unwrap_or("").trim();
        if line.is_empty() || line.starts_with('@') || line.starts_with("//") {
            continue;
        }
        if let Some(rest) = line.strip_prefix(".type") {
            if let Some((name, kind)) = rest.trim().split_once(',') {
                if matches!(kind.trim(), "%function" | "@function" | "function") {
                    function_types.insert(name.trim().to_owned());
                }
            }
            continue;
        }
        if line.starts_with(".thumb_func") {
            thumb = true;
            continue;
        }
        if let Some(name) = line.strip_suffix(':') {
            candidate |= thumb || function_types.contains(name);
            if candidate {
                names.push(name.to_owned());
                thumb = false;
            }
            continue;
        }
        if !candidate {
            continue;
        }
        if [".align", ".balign", ".p2align", ".global", ".globl"]
            .iter()
            .any(|directive| line.starts_with(directive))
        {
            continue;
        }
        if !line.starts_with('.') {
            if let Some(address) = names.iter().find_map(|name| {
                let (_, address) = name.rsplit_once('_')?;
                (address.len() == 8 && address.starts_with("02"))
                    .then(|| i64::from_str_radix(address, 16).ok())
                    .flatten()
            }) {
                seeds.push(address);
            } else if let Some(name) = names.first() {
                unmapped.push(name.clone());
            }
        }
        candidate = false;
        names.clear();
    }
    (seeds, unmapped)
}

fn directory_span(rom: &CanonicalRom) -> Result<Span, String> {
    let start = resource_table(rom.bytes())?;
    let end = start
        .checked_add(
            rom.resource_count()
                .checked_mul(4)
                .ok_or("directory extent overflow")?,
        )
        .ok_or("directory extent overflow")?;
    if end > rom.bytes().len() {
        return Err("resource directory lies outside the ROM".into());
    }
    Ok(Span::new(ROM_BASE + start as i64, ROM_BASE + end as i64))
}

fn validate_section(
    rom: &CanonicalRom,
    section: &MainSection,
    typed_data: &[super::main_data::Range],
) -> Result<(), String> {
    let span = section.span;
    if span.start < ROM_BASE + HEADER_BYTES
        || span.end <= span.start
        || span.end > ROM_BASE + rom.bytes().len() as i64
    {
        return Err("main section is empty or outside the main image".into());
    }
    match &section.proof {
        SectionProof::Linked(compiled) => {
            let start = (span.start - ROM_BASE) as usize;
            let end = (span.end - ROM_BASE) as usize;
            if rom.bytes().get(start..end) != Some(compiled.as_slice()) {
                return Err(
                    "linked main section differs in extent or bytes from the approved ROM".into(),
                );
            }
        }
        SectionProof::EncodedResource(resource) => {
            if section.kind != SectionKind::Data || span != resource_span(rom, *resource)? {
                return Err(
                    "encoded resource classification does not name its exact data extent".into(),
                );
            }
        }
        SectionProof::ResourceDirectory => {
            if section.kind != SectionKind::Data || span != directory_span(rom)? {
                return Err(
                    "resource directory classification does not name its exact data extent".into(),
                );
            }
        }
        SectionProof::SourceData(kind) => {
            if section.kind != SectionKind::Data
                || !typed_data.iter().any(|range| {
                    range.span == span && range.kind == *kind && range.source == section.source
                })
            {
                return Err(
                    "typed data classification differs from its current source-backed read".into(),
                );
            }
        }
    }
    Ok(())
}

/// Add every independently parsed stream and resource-directory word as
/// physical data. Code overlays are also compressed data in the main ROM;
/// their decoded executable bytes are counted separately by their own image.
/// Failed stream decodes, gaps, padding and raw directory pointers claim no
/// extent and therefore leave those main-image bytes unknown.
pub(crate) fn parsed_main_data(rom: &CanonicalRom) -> Result<Vec<MainSection>, String> {
    let mut sections = vec![MainSection {
        span: directory_span(rom)?,
        kind: SectionKind::Data,
        source: PathBuf::from("tools/alchemy/src/overlay/rom.rs"),
        proof: SectionProof::ResourceDirectory,
    }];
    let source = PathBuf::from("tools/alchemy/src/build_assets/derive_index.rs");
    // The directory's defining ROM-base and self-pointer words are metadata,
    // not stream entries, even if their targets happen to decode as LZ.
    for resource in 2..rom.resource_count() {
        let Ok(span) = resource_span(rom, resource) else {
            continue;
        };
        sections.push(MainSection {
            span,
            kind: SectionKind::Data,
            source: source.clone(),
            proof: SectionProof::EncodedResource(resource),
        });
    }
    Ok(sections)
}

fn classify_main(domain: Span, sections: &[(Span, SectionKind)]) -> Result<Image, String> {
    if domain.end <= domain.start {
        return Err("main image has an empty domain".into());
    }
    let mut sections = sections.to_vec();
    sections.sort_by_key(|(span, _)| (span.start, span.end));
    sections.dedup_by(|left, right| {
        left.0 == right.0 && left.1 == SectionKind::Data && right.1 == SectionKind::Data
    });
    if sections.iter().any(|(span, _)| {
        span.start < domain.start || span.end > domain.end || span.end <= span.start
    }) {
        return Err("main section lies outside its image domain".into());
    }
    // Resource aliases may prove overlapping physical data; executable/data
    // overlaps and duplicate executable ownership must never be reconciled.
    for (index, (left, left_kind)) in sections.iter().enumerate() {
        for (right, right_kind) in sections.iter().skip(index + 1) {
            if right.start >= left.end {
                break;
            }
            if *left_kind != SectionKind::Data || *right_kind != SectionKind::Data {
                return Err("main executable, data or padding classifications overlap".into());
            }
        }
    }
    let executable = normalize(
        &sections
            .iter()
            .filter_map(|(span, kind)| (*kind == SectionKind::Executable).then_some(*span))
            .collect::<Vec<_>>(),
    );
    let excluded = normalize(
        &sections
            .iter()
            .filter_map(|(span, kind)| (*kind != SectionKind::Executable).then_some(*span))
            .collect::<Vec<_>>(),
    );
    let classified = normalize(&sections.iter().map(|(span, _)| *span).collect::<Vec<_>>());
    let unknown = subtract(&[domain], &classified);
    let reasons = if unknown.is_empty() {
        Vec::new()
    } else {
        vec![format!(
            "{} main-image bytes have no independently verified code/data classification",
            bytes(&unknown)
        )]
    };
    Ok(Image {
        id: "main".into(),
        domain,
        executable,
        excluded,
        unknown,
        reasons,
    })
}

fn classify_overlay(id: &str, decoded: &[u8], entry_veneers: usize) -> Result<Image, String> {
    classify_overlay_from_entries(id, decoded, entry_veneers, &[])
}

fn classify_overlay_from_entries(
    id: &str,
    decoded: &[u8],
    entry_veneers: usize,
    entries: &[i64],
) -> Result<Image, String> {
    let code = overlay_code(decoded, OVERLAY_BASE, entry_veneers, entries)?;
    let domain = Span::new(OVERLAY_BASE, OVERLAY_BASE + decoded.len() as i64);
    let mut derived = code.spans;
    derived.extend(veneer_spans(decoded, OVERLAY_BASE));
    derived.extend(compiler_runtime_spans(decoded, OVERLAY_BASE));
    derived.extend(alignment_spans(decoded, OVERLAY_BASE, &derived));
    let executable = normalize(
        &derived
            .iter()
            .map(|span| Span::new(span.start, span.end))
            .collect::<Vec<_>>(),
    );
    if executable
        .iter()
        .any(|span| span.start < domain.start || span.end > domain.end)
    {
        return Err(format!("{id}: derived code lies outside its decoded image"));
    }
    let mut reasons = Vec::new();
    if !code.unresolved_jumps.is_empty() {
        reasons.push(format!(
            "{id}: {} computed jumps have unproved successors",
            code.unresolved_jumps.len()
        ));
    }
    if !code.conflicts.is_empty() {
        reasons.push(format!(
            "{id}: {} instruction/data conflicts",
            code.conflicts.len()
        ));
    }
    if !code.failed_runs.is_empty() {
        reasons.push(format!(
            "{id}: {} walks from proved entries failed",
            code.failed_runs.len()
        ));
    }
    if !code.rejected_owners.is_empty() {
        reasons.push(format!(
            "{id}: {} source function entries contradict decoded control flow",
            code.rejected_owners.len()
        ));
    }
    let mut unnamed_returns = code
        .rejected
        .iter()
        .filter(|candidate| candidate.failure.reason == "is a lone bx lr nothing names")
        .map(|candidate| candidate.failure.at)
        .collect::<BTreeSet<_>>();
    // The CFG's gap walk has code on both sides. A return-only function at
    // the tail has no following function to close that gap; its opcode and
    // normal zero alignment still cannot independently establish data.
    unnamed_returns.extend(
        decoded
            .chunks_exact(4)
            .enumerate()
            .filter(|(_, word)| *word == [0x70, 0x47, 0, 0])
            .map(|(index, _)| OVERLAY_BASE + index as i64 * 4)
            .filter(|address| {
                !executable
                    .iter()
                    .any(|span| span.start <= *address && *address < span.end)
            }),
    );
    if !unnamed_returns.is_empty() {
        reasons.push(format!(
            "{id}: {} unnamed return-only candidates need source code/data evidence",
            unnamed_returns.len()
        ));
    }
    let remainder = subtract(&[domain], &executable);
    let (excluded, unknown) = if reasons.is_empty() {
        (remainder, Vec::new())
    } else {
        (Vec::new(), remainder)
    };
    Ok(Image {
        id: id.into(),
        domain,
        executable,
        excluded,
        unknown,
        reasons,
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    fn overlay(function: &[u16], tail: &[u16]) -> Vec<u8> {
        let mut image = vec![0x00, 0x4c, 0x20, 0x47, 0x09, 0x80, 0x00, 0x02];
        image.extend(
            function
                .iter()
                .chain(tail)
                .flat_map(|half| half.to_le_bytes()),
        );
        image
    }

    #[test]
    fn complete_overlay_flow_excludes_unreachable_data_without_owner_seeds() {
        let decoded = overlay(&[0xb500, 0x2001, 0xbd00], &[0xffff, 0xffff]);
        let image = classify_overlay("fixture", &decoded, 1).unwrap();
        assert_eq!(image.state(), State::Exact);
        assert_eq!(image.executable_bytes(), 14);
        assert_eq!(
            image.excluded,
            vec![Span::new(OVERLAY_BASE + 14, OVERLAY_BASE + 18)]
        );
    }

    #[test]
    fn unresolved_overlay_jump_withholds_a_total() {
        let decoded = overlay(&[0x4687], &[0xffff]);
        let image = classify_overlay("fixture", &decoded, 1).unwrap();
        assert_eq!(image.state(), State::Unknown);
        assert!(image
            .reasons
            .iter()
            .any(|reason| reason.contains("computed jumps")));
        assert!(image.excluded.is_empty());
    }

    #[test]
    fn whole_matching_source_proves_an_unused_empty_function() {
        let decoded = overlay(&[0xb500, 0x2001, 0xbd00, 0], &[0x4770, 0]);
        let baseline = classify_overlay("fixture", &decoded, 1).unwrap();
        assert_eq!(baseline.state(), State::Unknown);
        assert!(baseline
            .reasons
            .iter()
            .any(|reason| reason.contains("return-only")));
        let mut entry = OverlayEntry {
            image: "fixture".into(),
            address: OVERLAY_BASE + 16,
            source: PathBuf::from("games/FIELD/EMPTY.C"),
            compiled: vec![0x70, 0x47, 0, 0],
        };
        let address = validate_overlay_entry(&decoded, &entry).unwrap();
        let image = classify_overlay_from_entries("fixture", &decoded, 1, &[address]).unwrap();
        assert_eq!(image.state(), State::Exact);
        assert_eq!(image.executable_bytes(), baseline.executable_bytes() + 4);
        entry.compiled[0] = 0;
        assert!(validate_overlay_entry(&decoded, &entry).is_err());
        entry.compiled.clear();
        assert!(validate_overlay_entry(&decoded, &entry).is_err());
        entry.address = OVERLAY_BASE - 2;
        assert!(validate_overlay_entry(&decoded, &entry).is_err());
    }

    #[test]
    fn compiled_overlay_evidence_matches_the_loaded_whole_image() {
        let decoded = overlay(&[0xb500, 0xf000, 0xf805, 0xbd00], &[0x4770, 0]);
        let runtime = crate::compiler::overlay::load(&decoded, 0).unwrap();
        let entry = OverlayEntry {
            image: "fixture".into(),
            address: OVERLAY_BASE + 8,
            source: PathBuf::from("games/FIELD/CALL.C"),
            compiled: runtime[8..16].to_vec(),
        };
        assert_ne!(entry.compiled, decoded[8..16]);
        validate_overlay_entry(&runtime, &entry).unwrap();
        assert!(validate_overlay_entry(&decoded, &entry).is_err());
    }

    #[test]
    fn retained_declarations_seed_real_code_but_not_placeholder_space() {
        let source = ".thumb_func\nNatural:\nFunc_02000100:\n bx lr\n.type Func_02000200, %function\nFunc_02000200:\n bx lr\n.thumb_func\nAlchemyC_02000300:\n .space 4\n.thumb_func\nUnplaced:\n bx lr\n";
        let (entries, unmapped) = declared_overlay_entries(source);
        assert_eq!(entries, vec![OVERLAY_BASE + 0x100, OVERLAY_BASE + 0x200]);
        assert_eq!(unmapped, vec!["Unplaced"]);
    }

    #[test]
    fn native_veneer_heads_do_not_gain_data_authority_from_accidental_lz() {
        assert!(native_veneer_head(&[0, 0x4c, 0x20, 0x47, 0x45, 4, 0x0b, 8]));
        assert!(native_veneer_head(&[0, 0x4c, 0x20, 0x47, 0x45, 4, 0x0b, 3]));
        assert!(!native_veneer_head(&[0, 0x4c, 0x20, 0x47]));
        assert!(!native_veneer_head(&[0, 0x4c, 0x20, 0x47, 0, 0, 0, 0]));
    }

    #[test]
    fn incomplete_layout_never_turns_the_private_remainder_into_data() {
        let image = classify_main(
            Span::new(100, 200),
            &[(Span::new(100, 116), SectionKind::Executable)],
        )
        .unwrap();
        assert_eq!(image.state(), State::Unknown);
        assert_eq!(image.unknown, vec![Span::new(116, 200)]);
        assert!(image.excluded.is_empty());
    }

    #[test]
    fn complete_main_layout_counts_code_and_pools_but_excludes_data_and_padding() {
        let image = classify_main(
            Span::new(100, 140),
            &[
                (Span::new(100, 116), SectionKind::Executable),
                (Span::new(116, 120), SectionKind::Padding),
                (Span::new(120, 140), SectionKind::Data),
            ],
        )
        .unwrap();
        assert_eq!(image.state(), State::Exact);
        assert_eq!(image.executable_bytes(), 16);
        assert_eq!(image.excluded, vec![Span::new(116, 140)]);
    }

    #[test]
    fn aliases_union_as_data_and_conflicting_classifications_fail() {
        let domain = Span::new(100, 140);
        let image = classify_main(
            domain,
            &[
                (Span::new(100, 130), SectionKind::Data),
                (Span::new(120, 140), SectionKind::Data),
            ],
        )
        .unwrap();
        assert_eq!(image.state(), State::Exact);
        assert_eq!(bytes(&image.excluded), 40);
        assert!(classify_main(
            domain,
            &[
                (Span::new(100, 130), SectionKind::Executable),
                (Span::new(120, 140), SectionKind::Data),
            ]
        )
        .is_err());
        assert!(classify_main(domain, &[(Span::new(90, 100), SectionKind::Data)]).is_err());
        assert!(classify_main(
            domain,
            &[
                (Span::new(100, 120), SectionKind::Executable),
                (Span::new(100, 120), SectionKind::Executable),
            ],
        )
        .is_err());
    }

    #[test]
    fn generated_inputs_cannot_establish_main_layout_authority() {
        let root = tempfile::tempdir().unwrap();
        for path in [
            "out/fixture.s",
            "tools/out/fixture.s",
            "recon/../out/fixture.s",
            "",
        ] {
            assert!(
                validate_source(root.path(), Path::new(path)).is_err(),
                "{path}"
            );
        }
        std::fs::create_dir(root.path().join("games")).unwrap();
        std::fs::write(root.path().join("games/MAIN.LD"), "SECTIONS {}").unwrap();
        validate_source(root.path(), Path::new("games/MAIN.LD")).unwrap();
    }

    fn fixture_rom() -> CanonicalRom {
        use psynergy::assets::lz::{encode_general, GeneralToken};
        let mut bytes = vec![0xaau8; 256];
        for (index, pointer) in [
            ROM_BASE,
            ROM_BASE + 0xc0,
            ROM_BASE + 0xe0,
            ROM_BASE + 0xe0,
            0,
        ]
        .iter()
        .enumerate()
        {
            let start = 0xc0 + index * 4;
            bytes[start..start + 4].copy_from_slice(&(*pointer as u32).to_le_bytes());
        }
        let encoded = encode_general(b"ABAB", &[GeneralToken::Literal(4)]).unwrap();
        bytes[0xe0..0xe0 + encoded.len()].copy_from_slice(&encoded);
        let directory = tempfile::tempdir().unwrap();
        let path = directory.path().join("fixture.gba");
        std::fs::write(&path, &bytes).unwrap();
        let mut target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        target.rom_size = bytes.len() as u64;
        CanonicalRom::from_file(&path, target).unwrap()
    }

    #[test]
    fn resource_aliases_prove_only_their_physical_encoding_not_directory_gaps() {
        let rom = fixture_rom();
        let resource = resource_span(&rom, 2).unwrap();
        assert_eq!(resource, resource_span(&rom, 3).unwrap());
        let sections = parsed_main_data(&rom).unwrap();
        for section in &sections {
            validate_section(&rom, section, &[]).unwrap();
        }
        let image = classify_main(
            Span::new(ROM_BASE + 0xc0, ROM_BASE + 256),
            &sections
                .iter()
                .map(|section| (section.span, section.kind))
                .collect::<Vec<_>>(),
        )
        .unwrap();
        assert_eq!(image.state(), State::Unknown);
        assert!(image
            .unknown
            .contains(&Span::new(ROM_BASE + 0xd0, ROM_BASE + 0xe0)));
        let mut forged = MainSection {
            span: Span::new(resource.start, resource.end + 1),
            kind: SectionKind::Data,
            source: PathBuf::from("tools/alchemy/src/overlay/rom.rs"),
            proof: SectionProof::EncodedResource(2),
        };
        assert!(validate_section(&rom, &forged, &[]).is_err());
        forged.span = resource;
        forged.kind = SectionKind::Executable;
        assert!(validate_section(&rom, &forged, &[]).is_err());
    }

    #[test]
    fn linked_section_must_match_the_whole_named_extent() {
        let rom = fixture_rom();
        let mut section = MainSection {
            span: Span::new(ROM_BASE + 0xd4, ROM_BASE + 0xd8),
            kind: SectionKind::Executable,
            source: PathBuf::from("games/MAIN.LD"),
            proof: SectionProof::Linked(vec![0xaa; 4]),
        };
        validate_section(&rom, &section, &[]).unwrap();
        section.proof = SectionProof::Linked(vec![0xaa; 2]);
        assert!(validate_section(&rom, &section, &[]).is_err());
        section.proof = SectionProof::Linked(vec![0xbb; 4]);
        assert!(validate_section(&rom, &section, &[]).is_err());
    }

    #[test]
    fn typed_data_proof_needs_the_current_reader_and_source() {
        let rom = fixture_rom();
        let mut section = MainSection {
            span: Span::new(ROM_BASE + 0xd4, ROM_BASE + 0xd8),
            kind: SectionKind::Data,
            source: PathBuf::from("games/FIELD/FRAME.C"),
            proof: SectionProof::SourceData(super::super::main_data::Kind::SpriteFrame),
        };
        assert!(validate_section(&rom, &section, &[]).is_err());
        let range = super::super::main_data::Range {
            span: section.span,
            kind: super::super::main_data::Kind::SpriteFrame,
            source: section.source.clone(),
        };
        validate_section(&rom, &section, &[range]).unwrap();
        section.kind = SectionKind::Executable;
        assert!(validate_section(&rom, &section, &[]).is_err());
    }

    #[test]
    #[ignore = "reads both approved local ROMs and writes disposable executable diagnostics"]
    fn approved_rom_diagnostics_keep_unclassified_main_bytes_pending() {
        use serde_json::json;
        let root = crate::compiler::routing::root();
        let spans = |values: &[Span]| {
            values
                .iter()
                .map(|span| json!({"start": span.start, "end": span.end, "bytes": span.bytes()}))
                .collect::<Vec<_>>()
        };
        for id in [
            crate::targets::DecompTargetId::TbsEn,
            crate::targets::DecompTargetId::TlaEn,
        ] {
            let target = crate::targets::target_for(id);
            let rom = CanonicalRom::load_target(root, target).unwrap();
            crate::text_catalog::verify_reference(root, target.id.as_str(), rom.bytes()).unwrap();
            let layout = MainLayout {
                sections: parsed_main_data(&rom).unwrap(),
                ..MainLayout::default()
            };
            let accounting = derive(root, target, &layout).unwrap();
            assert_eq!(accounting.state(), State::Unknown);
            assert_eq!(accounting.executable_bytes(), None);
            let mut rows = Vec::new();
            for image in &accounting.overlays {
                let decoded = rom.overlay(&image.id).unwrap();
                let code = overlay_code(&decoded, OVERLAY_BASE, target.overlay_entry_veneers, &[])
                    .unwrap();
                rows.push(json!({
                    "id": image.id, "decoded_bytes": decoded.len(),
                    "executable_bytes": image.executable_bytes(),
                    "executable": spans(&image.executable), "excluded": spans(&image.excluded),
                    "unknown": spans(&image.unknown), "reasons": image.reasons,
                    "rejected_candidates": code.rejected.iter().map(|rejection| json!({
                        "entry": rejection.seed, "evidence": rejection.evidence.name(),
                        "at": rejection.failure.at, "reason": rejection.failure.reason,
                    })).collect::<Vec<_>>(),
                }));
            }
            let report = json!({
                "format": "alchemy-executable-source-candidate-v1",
                "target": accounting.target, "rom_sha256": accounting.rom_sha256,
                "state": "unknown", "executable_bytes": serde_json::Value::Null,
                "main": {"unknown": spans(&accounting.main.unknown), "excluded": spans(&accounting.main.excluded)},
                "overlays": rows,
            });
            let directory = crate::compiler::build_io::generated_directory(
                root,
                &root.join(target.output_dir).join("reports"),
            )
            .unwrap();
            let path = directory.join("executable-source-candidate.json");
            crate::compiler::canonical_json::write_canonical(&path, &report).unwrap();
            println!(
                "target={} overlays={} overlay_executable={} main_unknown={} diagnostic={}",
                id,
                accounting.overlays.len(),
                accounting
                    .overlays
                    .iter()
                    .map(Image::executable_bytes)
                    .sum::<i64>(),
                bytes(&accounting.main.unknown),
                path.display()
            );
        }
    }

    #[test]
    #[ignore = "independently reads current ordinary TBS artifacts and writes disposable diagnostics"]
    fn approved_tbs_linked_source_diagnostic() {
        use serde_json::json;
        let root = crate::compiler::routing::root();
        let target = crate::targets::target_for(crate::targets::DecompTargetId::TbsEn);
        let rom = CanonicalRom::load_target(root, target).unwrap();
        let verified = super::super::source::verify(root, target, rom.bytes()).unwrap();
        let accounting = derive(root, target, &verified.layout).unwrap();
        let spans = |values: &[Span]| {
            values
                .iter()
                .map(|span| json!({"start": span.start, "end": span.end, "bytes": span.bytes()}))
                .collect::<Vec<_>>()
        };
        let report = json!({
            "format": "alchemy-executable-linked-source-candidate-v1",
            "target": accounting.target,
            "rom_sha256": accounting.rom_sha256,
            "state": if accounting.state() == State::Exact { "exact" } else { "unknown" },
            "executable_bytes": accounting.executable_bytes(),
            "source_bytes": verified.source_bytes,
            "main": {
                "executable": spans(&accounting.main.executable),
                "excluded": spans(&accounting.main.excluded),
                "unknown": spans(&accounting.main.unknown),
                "reasons": accounting.main.reasons,
            },
            "overlays": accounting.overlays.iter().map(|image| json!({
                "id": image.id, "executable": spans(&image.executable),
                "excluded": spans(&image.excluded), "unknown": spans(&image.unknown),
                "reasons": image.reasons,
            })).collect::<Vec<_>>(),
        });
        let directory = crate::compiler::build_io::generated_directory(
            root,
            &root.join(target.output_dir).join("reports"),
        )
        .unwrap();
        let path = directory.join("executable-linked-source-candidate.json");
        crate::compiler::canonical_json::write_canonical(&path, &report).unwrap();
        println!(
            "target={} linked_source={} main_executable={} main_unknown={} diagnostic={}",
            target.id,
            verified.source_bytes,
            accounting.main.executable_bytes(),
            bytes(&accounting.main.unknown),
            path.display()
        );
    }
}
