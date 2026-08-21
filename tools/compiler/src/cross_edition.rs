use objdiff_core::{
    diff::{ArmArchVersion, DiffObjConfig, DiffSide},
    obj,
};
use serde::Serialize;
use std::{
    collections::BTreeMap,
    fs,
    path::{Path, PathBuf},
    process::Command,
};

const ROM_BASE: u64 = 0x0800_0000;
const EDITIONS: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];
const USAGE: &str =
    "usage: compiler cross-edition [--calls] [--json] [--rom-dir DIR] [--object FILE] <8-digit-owner>";

#[derive(Debug, Serialize)]
struct Report {
    schema_version: u32,
    game: &'static str,
    owner: String,
    owner_edition: &'static str,
    base_edition: &'static str,
    relocation_source_edition: &'static str,
    size: usize,
    relocations: usize,
    relocation_bytes: usize,
    literal_fields: usize,
    literal_bytes: usize,
    core_bytes: usize,
    relocation_kinds: BTreeMap<String, usize>,
    call_targets: Vec<CallTarget>,
    core_identical: bool,
    editions: Vec<EditionReport>,
}

#[derive(Debug, Serialize)]
struct EditionReport {
    edition: String,
    start: String,
    shift: i64,
    raw_diff_bytes: usize,
    raw_diff_halfwords: usize,
    relocation_diff_bytes: usize,
    literal_diff_bytes: usize,
    relocation_fields_changed: usize,
    literal_fields_changed: usize,
    call_fields_changed: usize,
    absolute_fields_changed: usize,
    core_diff_bytes: usize,
    anchor_matches: usize,
    core_identical: bool,
}

#[derive(Debug, Serialize)]
struct CallTarget {
    en_symbol: String,
    sites: usize,
    editions: Vec<TargetAddress>,
}

#[derive(Debug, Serialize)]
struct TargetAddress {
    edition: String,
    address: String,
}

struct RelocationSite {
    offset: usize,
    size: usize,
    kind: String,
    symbol: String,
}

struct LiteralSite {
    offset: usize,
    size: usize,
}

struct Options {
    owner: String,
    owner_address: u64,
    object: Option<PathBuf>,
    rom_dir: PathBuf,
    json: bool,
    calls: bool,
}

struct FoundEdition {
    start: usize,
    bytes: Vec<u8>,
    anchor_matches: usize,
}

struct ComparisonContext<'a> {
    base_start: usize,
    base: &'a [u8],
    mask: &'a [bool],
    relocation_mask: &'a [bool],
    relocations: &'a [RelocationSite],
    literals: &'a [LiteralSite],
}

pub fn run(args: &[String]) -> Result<(), String> {
    let options = parse(args)?;
    let object_path = resolve_object(&options)?;
    let (size, symbol_address, relocation_mask, relocations) =
        relocation_mask(&object_path, &options.owner)?;
    let literals = literal_sites(&object_path, symbol_address, size)?;
    let mut mask = relocation_mask.clone();
    for literal in &literals {
        mask[literal.offset..literal.offset + literal.size].fill(true);
    }
    let en_path = options.rom_dir.join("gs1-en.gba");
    let en_rom = read_rom(&en_path)?;
    let en_offset = rom_offset(options.owner_address, en_rom.len())?;
    let en_owner = en_rom
        .get(en_offset..en_offset + size)
        .ok_or_else(|| format!("{}: owner extends past ROM", en_path.display()))?;

    let anchors = anchors(en_owner, &mask);
    if anchors.is_empty() {
        return Err(
            "owner has no relocation-free anchor long enough to locate counterparts".into(),
        );
    }

    let mut found = BTreeMap::new();
    found.insert(
        "en",
        FoundEdition {
            start: en_offset,
            bytes: en_owner.to_vec(),
            anchor_matches: anchors.len(),
        },
    );
    for edition in EDITIONS.into_iter().filter(|edition| *edition != "en") {
        let path = options.rom_dir.join(format!("gs1-{edition}.gba"));
        let rom = read_rom(&path)?;
        let (start, support) = locate(en_owner, &mask, &anchors, &rom)
            .map_err(|error| format!("{edition}: {error}"))?;
        found.insert(
            edition,
            FoundEdition {
                start,
                bytes: rom[start..start + size].to_vec(),
                anchor_matches: support,
            },
        );
    }

    let ja = found.get("ja").ok_or("JA counterpart was not located")?;
    let context = ComparisonContext {
        base_start: ja.start,
        base: &ja.bytes,
        mask: &mask,
        relocation_mask: &relocation_mask,
        relocations: &relocations,
        literals: &literals,
    };
    let editions = EDITIONS
        .into_iter()
        .map(|edition| {
            let entry = &found[edition];
            edition_report(
                edition,
                entry.start,
                &entry.bytes,
                &context,
                entry.anchor_matches,
            )
        })
        .collect::<Vec<_>>();
    let call_targets = if options.calls {
        call_targets(&relocations, &found)?
    } else {
        Vec::new()
    };
    let mut relocation_kinds = BTreeMap::new();
    for relocation in &relocations {
        *relocation_kinds.entry(relocation.kind.clone()).or_default() += 1;
    }

    let report = Report {
        schema_version: 1,
        game: "gs1",
        owner: options.owner,
        owner_edition: "en",
        base_edition: "ja",
        relocation_source_edition: "en",
        size,
        relocations: relocations.len(),
        relocation_bytes: relocation_mask.iter().filter(|value| **value).count(),
        literal_fields: literals.len(),
        literal_bytes: literals.iter().map(|literal| literal.size).sum(),
        core_bytes: mask.iter().filter(|value| !**value).count(),
        relocation_kinds,
        call_targets,
        core_identical: editions.iter().all(|edition| edition.core_identical),
        editions,
    };
    if options.json {
        println!(
            "{}",
            serde_json::to_string_pretty(&report)
                .map_err(|error| format!("serialize report: {error}"))?
        );
    } else {
        print_report(&report, options.calls);
    }
    if report.core_identical {
        Ok(())
    } else {
        Err("one or more editions differ outside EN relocation and literal fields".into())
    }
}

fn parse(args: &[String]) -> Result<Options, String> {
    let mut owner = None;
    let mut object = None;
    let mut rom_dir = PathBuf::from("roms");
    let mut json = false;
    let mut calls = false;
    let mut index = 0;
    while index < args.len() {
        match args[index].as_str() {
            "--json" => json = true,
            "--calls" => calls = true,
            "--rom-dir" => {
                index += 1;
                rom_dir = PathBuf::from(args.get(index).ok_or(USAGE)?);
            }
            "--object" => {
                index += 1;
                object = Some(PathBuf::from(args.get(index).ok_or(USAGE)?));
            }
            "-h" | "--help" => return Err(USAGE.into()),
            value if value.starts_with('-') => {
                return Err(format!("unknown option: {value}\n{USAGE}"))
            }
            value if owner.is_none() => owner = Some(value.to_ascii_lowercase()),
            value => return Err(format!("unexpected argument: {value}\n{USAGE}")),
        }
        index += 1;
    }
    let owner = owner.ok_or(USAGE)?;
    if owner.len() != 8 || !owner.bytes().all(|byte| byte.is_ascii_hexdigit()) {
        return Err(format!(
            "owner must be an 8-digit hexadecimal address: {owner}"
        ));
    }
    let owner_address = u64::from_str_radix(&owner, 16)
        .map_err(|error| format!("invalid owner {owner}: {error}"))?;
    if owner_address < ROM_BASE {
        return Err(format!("owner is below GBA ROM: {owner}"));
    }
    Ok(Options {
        owner,
        owner_address,
        object,
        rom_dir,
        json,
        calls,
    })
}

fn resolve_object(options: &Options) -> Result<PathBuf, String> {
    if let Some(path) = &options.object {
        return path
            .is_file()
            .then(|| path.clone())
            .ok_or_else(|| format!("missing object: {}", path.display()));
    }
    let paths = [
        PathBuf::from(format!("out/claimed/obj/{}.o", options.owner)),
        PathBuf::from(format!("out/full/claimed/obj/{}.o", options.owner)),
    ];
    paths
        .into_iter()
        .find(|path| path.is_file())
        .ok_or_else(|| {
            format!(
                "missing exact object for {}; run `make build-claimed`",
                options.owner
            )
        })
}

fn relocation_mask(
    path: &Path,
    owner: &str,
) -> Result<(usize, u64, Vec<bool>, Vec<RelocationSite>), String> {
    let config = DiffObjConfig {
        arm_arch_version: ArmArchVersion::V4t,
        ..Default::default()
    };
    let object = obj::read::read(path, &config, DiffSide::Base)
        .map_err(|error| format!("{}: {error}", path.display()))?;
    let symbol_name = format!("Func_{owner}");
    let symbol_index = object
        .symbol_by_name(&symbol_name)
        .ok_or_else(|| format!("{}: missing symbol {symbol_name}", path.display()))?;
    let symbol = &object.symbols[symbol_index];
    let section = &object.sections[symbol.section.ok_or("owner symbol has no section")?];
    let size = usize::try_from(symbol.size).map_err(|_| "owner is too large")?;
    if size == 0 {
        return Err(format!("{symbol_name} has zero size"));
    }
    let mut mask = vec![false; size];
    let mut sites = Vec::new();
    for relocation in &section.relocations {
        let Some(offset) = relocation.address.checked_sub(symbol.address) else {
            continue;
        };
        if offset >= symbol.size {
            continue;
        }
        let relocation_size = object.arch.data_reloc_size(relocation.flags);
        let start = usize::try_from(offset).map_err(|_| "relocation offset is too large")?;
        let end = start
            .checked_add(relocation_size)
            .ok_or("relocation range overflow")?
            .min(mask.len());
        mask[start..end].fill(true);
        let kind = object
            .arch
            .reloc_name(relocation.flags)
            .map(str::to_string)
            .unwrap_or_else(|| format!("{:?}", relocation.flags));
        let symbol = object
            .symbols
            .get(relocation.target_symbol)
            .map(|symbol| symbol.name.clone())
            .unwrap_or_else(|| "<missing>".into());
        sites.push(RelocationSite {
            offset: start,
            size: end - start,
            kind,
            symbol,
        });
    }
    Ok((size, symbol.address, mask, sites))
}

fn literal_sites(
    path: &Path,
    symbol_address: u64,
    size: usize,
) -> Result<Vec<LiteralSite>, String> {
    let output = Command::new("arm-none-eabi-objdump")
        .args(["-dr"])
        .arg(path)
        .output()
        .map_err(|error| format!("arm-none-eabi-objdump: {error}"))?;
    if !output.status.success() {
        return Err(format!(
            "arm-none-eabi-objdump failed: {}",
            String::from_utf8_lossy(&output.stderr).trim()
        ));
    }
    Ok(parse_literal_sites(
        &String::from_utf8_lossy(&output.stdout),
        symbol_address,
        size,
    ))
}

fn parse_literal_sites(output: &str, symbol_address: u64, size: usize) -> Vec<LiteralSite> {
    let mut sites = Vec::new();
    for line in output.lines() {
        let Some((address, text)) = line.trim_start().split_once(':') else {
            continue;
        };
        let field_size = if text.contains(".word") {
            4
        } else if text.contains(".short") {
            2
        } else {
            continue;
        };
        let Ok(address) = u64::from_str_radix(address.trim(), 16) else {
            continue;
        };
        let Some(offset) = address.checked_sub(symbol_address) else {
            continue;
        };
        let Ok(offset) = usize::try_from(offset) else {
            continue;
        };
        if offset + field_size <= size {
            sites.push(LiteralSite {
                offset,
                size: field_size,
            });
        }
    }
    sites.sort_by_key(|site| site.offset);
    sites.dedup_by_key(|site| site.offset);
    sites
}

fn read_rom(path: &Path) -> Result<Vec<u8>, String> {
    fs::read(path).map_err(|error| format!("{}: {error}", path.display()))
}

fn rom_offset(address: u64, rom_len: usize) -> Result<usize, String> {
    let offset = address
        .checked_sub(ROM_BASE)
        .ok_or("address is below ROM")?;
    let offset = usize::try_from(offset).map_err(|_| "address is too large")?;
    (offset < rom_len)
        .then_some(offset)
        .ok_or_else(|| format!("address 0x{address:08x} is outside ROM"))
}

#[derive(Clone, Copy)]
struct Anchor {
    offset: usize,
    size: usize,
}

fn anchors(owner: &[u8], mask: &[bool]) -> Vec<Anchor> {
    let mut runs = Vec::new();
    let mut start = None;
    for (index, relocated) in mask.iter().copied().chain([true]).enumerate() {
        match (start, relocated) {
            (None, false) => start = Some(index),
            (Some(run_start), true) => {
                let size = index - run_start;
                if size >= 16 {
                    runs.push((size, run_start));
                }
                start = None;
            }
            _ => {}
        }
    }
    runs.sort_by(|left, right| right.cmp(left));
    runs.into_iter()
        .take(16)
        .map(|(size, offset)| Anchor {
            offset,
            size: size.min(32).min(owner.len() - offset),
        })
        .collect()
}

fn locate(
    owner: &[u8],
    mask: &[bool],
    anchors: &[Anchor],
    rom: &[u8],
) -> Result<(usize, usize), String> {
    let mut support = BTreeMap::<usize, usize>::new();
    for anchor in anchors {
        let needle = &owner[anchor.offset..anchor.offset + anchor.size];
        for hit in find_all(rom, needle) {
            let Some(start) = hit.checked_sub(anchor.offset) else {
                continue;
            };
            if start % 2 != 0 || start + owner.len() > rom.len() {
                continue;
            }
            *support.entry(start).or_default() += 1;
        }
        let exact = support
            .keys()
            .copied()
            .filter(|start| core_diff_bytes(owner, &rom[*start..*start + owner.len()], mask) == 0)
            .collect::<Vec<_>>();
        if let [start] = exact.as_slice() {
            return Ok((*start, support[start]));
        }
    }
    let mut scored = support
        .into_iter()
        .map(|(start, anchor_matches)| {
            (
                core_diff_bytes(owner, &rom[start..start + owner.len()], mask),
                usize::MAX - anchor_matches,
                start,
                anchor_matches,
            )
        })
        .collect::<Vec<_>>();
    scored.sort_unstable();
    let Some(&(best_diff, _, best_start, best_support)) = scored.first() else {
        return Err("no relocation-free anchor found in ROM".into());
    };
    if scored
        .get(1)
        .is_some_and(|next| next.0 == best_diff && next.1 == scored[0].1)
    {
        return Err(format!(
            "ambiguous counterpart with {best_diff} core byte differences"
        ));
    }
    Ok((best_start, best_support))
}

fn find_all(haystack: &[u8], needle: &[u8]) -> Vec<usize> {
    if needle.is_empty() || needle.len() > haystack.len() {
        return Vec::new();
    }
    let first = needle[0];
    haystack
        .windows(needle.len())
        .enumerate()
        .filter_map(|(index, window)| (window[0] == first && window == needle).then_some(index))
        .collect()
}

fn edition_report(
    edition: &str,
    start: usize,
    other: &[u8],
    context: &ComparisonContext<'_>,
    anchor_matches: usize,
) -> EditionReport {
    let base = context.base;
    let raw_diff_bytes = base
        .iter()
        .zip(other)
        .filter(|(left, right)| left != right)
        .count();
    let raw_diff_halfwords = base
        .chunks_exact(2)
        .zip(other.chunks_exact(2))
        .filter(|(left, right)| left != right)
        .count();
    let relocation_diff_bytes = base
        .iter()
        .zip(other)
        .zip(context.relocation_mask)
        .filter(|((left, right), relocated)| **relocated && left != right)
        .count();
    let literal_diff_bytes = context
        .literals
        .iter()
        .map(|site| {
            base[site.offset..site.offset + site.size]
                .iter()
                .zip(&other[site.offset..site.offset + site.size])
                .filter(|(left, right)| left != right)
                .count()
        })
        .sum();
    let core_diff_bytes = core_diff_bytes(base, other, context.mask);
    let changed = context
        .relocations
        .iter()
        .filter(|site| {
            base[site.offset..site.offset + site.size]
                != other[site.offset..site.offset + site.size]
        })
        .collect::<Vec<_>>();
    EditionReport {
        edition: edition.into(),
        start: format!("0x{:08x}", ROM_BASE + start as u64),
        shift: start as i64 - context.base_start as i64,
        raw_diff_bytes,
        raw_diff_halfwords,
        relocation_diff_bytes,
        literal_diff_bytes,
        relocation_fields_changed: changed.len(),
        literal_fields_changed: context
            .literals
            .iter()
            .filter(|site| {
                base[site.offset..site.offset + site.size]
                    != other[site.offset..site.offset + site.size]
            })
            .count(),
        call_fields_changed: changed.iter().filter(|site| is_thumb_call(site)).count(),
        absolute_fields_changed: changed
            .iter()
            .filter(|site| site.kind == "R_ARM_ABS32")
            .count(),
        core_diff_bytes,
        anchor_matches,
        core_identical: core_diff_bytes == 0,
    }
}

fn is_thumb_call(site: &RelocationSite) -> bool {
    site.kind == "R_ARM_THM_PC22" || site.kind == "R_ARM_THM_XPC22"
}

fn call_targets(
    relocations: &[RelocationSite],
    found: &BTreeMap<&str, FoundEdition>,
) -> Result<Vec<CallTarget>, String> {
    let mut groups = BTreeMap::<&str, Vec<usize>>::new();
    for site in relocations.iter().filter(|site| is_thumb_call(site)) {
        groups.entry(&site.symbol).or_default().push(site.offset);
    }
    groups
        .into_iter()
        .map(|(symbol, offsets)| {
            let mut editions = Vec::with_capacity(EDITIONS.len());
            for edition in EDITIONS {
                let entry = &found[edition];
                let mut targets = offsets
                    .iter()
                    .map(|offset| {
                        thumb_bl_target(&entry.bytes, entry.start, *offset)
                            .map_err(|error| format!("{edition}: {error}"))
                    })
                    .collect::<Result<Vec<_>, _>>()?;
                targets.sort_unstable();
                targets.dedup();
                if targets.len() != 1 {
                    return Err(format!(
                        "{edition}: {symbol} resolves to {} call targets",
                        targets.len()
                    ));
                }
                editions.push(TargetAddress {
                    edition: edition.into(),
                    address: format!("0x{:08x}", targets[0]),
                });
            }
            Ok(CallTarget {
                en_symbol: symbol.into(),
                sites: offsets.len(),
                editions,
            })
        })
        .collect()
}

fn thumb_bl_target(owner: &[u8], start: usize, offset: usize) -> Result<u64, String> {
    let bytes = owner
        .get(offset..offset + 4)
        .ok_or_else(|| format!("call at 0x{offset:x} extends past owner"))?;
    let high = u16::from_le_bytes([bytes[0], bytes[1]]);
    let low = u16::from_le_bytes([bytes[2], bytes[3]]);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return Err(format!("relocation at 0x{offset:x} is not a Thumb BL"));
    }
    let mut displacement = (((high & 0x07ff) as i64) << 12) | (((low & 0x07ff) as i64) << 1);
    if displacement & (1 << 22) != 0 {
        displacement -= 1 << 23;
    }
    let pc = ROM_BASE as i64 + start as i64 + offset as i64 + 4;
    u64::try_from(pc + displacement).map_err(|_| format!("call at 0x{offset:x} is below ROM"))
}

fn core_diff_bytes(left: &[u8], right: &[u8], mask: &[bool]) -> usize {
    left.iter()
        .zip(right)
        .zip(mask)
        .filter(|((left, right), relocated)| !**relocated && left != right)
        .count()
}

fn print_report(report: &Report, calls: bool) {
    println!(
        "owner={} owner_edition={} base_edition={} relocation_source_edition={} size={} relocations={} relocation_bytes={} literal_fields={} literal_bytes={} core_bytes={}",
        report.owner,
        report.owner_edition,
        report.base_edition,
        report.relocation_source_edition,
        report.size,
        report.relocations,
        report.relocation_bytes,
        report.literal_fields,
        report.literal_bytes,
        report.core_bytes
    );
    for edition in &report.editions {
        println!(
            "edition={} start={} shift={:+#x} raw_diff_bytes={} raw_diff_halfwords={} relocation_diff_bytes={} literal_diff_bytes={} relocation_fields_changed={} literal_fields_changed={} call_fields_changed={} absolute_fields_changed={} core_diff_bytes={} core_identical={}",
            edition.edition,
            edition.start,
            edition.shift,
            edition.raw_diff_bytes,
            edition.raw_diff_halfwords,
            edition.relocation_diff_bytes,
            edition.literal_diff_bytes,
            edition.relocation_fields_changed,
            edition.literal_fields_changed,
            edition.call_fields_changed,
            edition.absolute_fields_changed,
            edition.core_diff_bytes,
            if edition.core_identical { "yes" } else { "no" },
        );
    }
    println!(
        "callee_correspondences={} core_identical={}",
        report.call_targets.len(),
        if report.core_identical { "yes" } else { "no" }
    );
    if calls {
        for call in &report.call_targets {
            let addresses = call
                .editions
                .iter()
                .map(|target| format!("{}={}", target.edition, target.address))
                .collect::<Vec<_>>()
                .join(" ");
            println!(
                "callee={} sites={} {}",
                call.en_symbol, call.sites, addresses
            );
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn builds_anchors_only_from_core_bytes() {
        let owner = vec![0x55; 80];
        let mut mask = vec![false; owner.len()];
        mask[20..24].fill(true);
        let found = anchors(&owner, &mask);
        assert_eq!(found[0].offset, 24);
        assert_eq!(found[0].size, 32);
        assert_eq!(found[1].offset, 0);
        assert_eq!(found[1].size, 20);
    }

    #[test]
    fn locates_owner_while_ignoring_relocations() {
        let owner = (0..96).map(|value| value as u8).collect::<Vec<_>>();
        let mut mask = vec![false; owner.len()];
        mask[32..36].fill(true);
        let mut counterpart = owner.clone();
        counterpart[32..36].copy_from_slice(&[9, 8, 7, 6]);
        let mut rom = vec![0xff; 256];
        rom[100..196].copy_from_slice(&counterpart);
        let found = anchors(&owner, &mask);
        assert_eq!(locate(&owner, &mask, &found, &rom), Ok((100, 1)));
        assert_eq!(core_diff_bytes(&owner, &counterpart, &mask), 0);
    }

    #[test]
    fn counts_only_unmasked_differences_as_core() {
        let left = [1, 2, 3, 4, 5, 6];
        let right = [1, 9, 3, 8, 5, 6];
        let mask = [false, true, false, false, false, false];
        assert_eq!(core_diff_bytes(&left, &right, &mask), 1);
    }

    #[test]
    fn decodes_thumb_bl_target() {
        let bytes = [0x48, 0xf7, 0xfd, 0xfe];
        assert_eq!(thumb_bl_target(&bytes, 0x0bbb3a, 0), Ok(0x08004938));
    }

    #[test]
    fn reads_literal_fields_from_objdump() {
        let output = "  1008:\t12345678 \t.word\t0x12345678\n  100c:\t0000 \t.short\t0x0000\n";
        let sites = parse_literal_sites(output, 0x1000, 16);
        assert_eq!(sites.len(), 2);
        assert_eq!((sites[0].offset, sites[0].size), (8, 4));
        assert_eq!((sites[1].offset, sites[1].size), (12, 2));
    }
}
