//! The resource loader at 08002d5c rewrites every Thumb BL-shaped halfword pair,
//! including pairs in literal pools. Owner addresses are resource coordinates.
//! An overlay link binds each undefined name in one image: declared absolute
//! symbols, registered names, then legacy call keys, hex names and call-via
//! banks.
use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::SourcePaths;
use crate::compiler::translation_units::{AbsoluteSymbol, AbsoluteSymbolKind};
use psynergy::decode::MAIN_BASE;
use psynergy::thumb::veneer_target;
use std::collections::{BTreeMap, BTreeSet};
use std::sync::OnceLock;

pub const RESOURCE_BASE: u32 = 0x0200_0000;
pub const RUNTIME_BASE: u32 = 0x0200_8000;

use psynergy::thumb::bl_displacement as displacement;

pub fn placeholder_addresses(assembly: &str) -> BTreeSet<u32> {
    assembly
        .lines()
        .filter_map(|line| {
            u32::from_str_radix(
                line.trim().strip_prefix("AlchemyC_")?.strip_suffix(':')?,
                16,
            )
            .ok()
        })
        .collect()
}

#[derive(Clone, Copy)]
pub struct Placeholder {
    pub start: usize,
    pub end: usize,
    pub span: i64,
}
pub fn space_size(line: &str) -> Option<i64> {
    let value = line.trim().strip_prefix(".space ")?.trim();
    value
        .strip_prefix("0x")
        .map_or_else(
            || value.parse().ok(),
            |hex| i64::from_str_radix(hex, 16).ok(),
        )
        .filter(|size| *size > 0)
}
pub fn placeholder_block(lines: &[&str], address: i64) -> Option<Placeholder> {
    let tag = format!("AlchemyC_{address:08x}:");
    let mut matches = lines
        .iter()
        .enumerate()
        .filter(|(_, line)| line.trim() == tag);
    let (start, _) = matches.next()?;
    if matches.next().is_some() {
        return None;
    }
    let (mut end, mut span) = (start + 1, 0i64);
    while let Some(line) = lines.get(end).map(|line| line.trim()) {
        if line.starts_with(".space ") {
            span = span.checked_add(space_size(line)?)?;
        } else if !(line.starts_with(".L_") && line.ends_with(':')) {
            break;
        }
        end += 1;
    }
    (span > 0).then_some(Placeholder { start, end, span })
}
pub fn placeholder_extent(text: &str, address: u32) -> Option<usize> {
    let lines = text.lines().collect::<Vec<_>>();
    usize::try_from(placeholder_block(&lines, i64::from(address))?.span).ok()
}
/// The one `AlchemyData_<address>:` block a unit's read-only data fills: a
/// label followed only by positive `.space` lines, as for C placeholders.
pub fn data_placeholder_extent(text: &str, address: u32) -> Option<usize> {
    let tag = format!("AlchemyData_{address:08x}:");
    let lines = text.lines().map(str::trim).collect::<Vec<_>>();
    let mut matches = (0..lines.len()).filter(|index| lines[*index] == tag);
    let start = matches.next()?;
    if matches.next().is_some() {
        return None;
    }
    let mut span = 0usize;
    for line in lines[start + 1..]
        .iter()
        .take_while(|line| line.starts_with(".space "))
    {
        span = span.checked_add(usize::try_from(space_size(line)?).ok()?)?;
    }
    (span > 0).then_some(span)
}

fn transform(bytes: &[u8], offset: usize, encode: bool) -> Result<Vec<u8>, String> {
    if !offset.is_multiple_of(2) || !bytes.len().is_multiple_of(2) {
        return Err("overlay image and offset must be halfword aligned".into());
    }
    let mut result = bytes.to_vec();
    for suffix in (2..result.len()).step_by(2) {
        let Some(value) = displacement(&result[suffix - 2..suffix + 2]) else {
            continue;
        };
        let site = u32::try_from(
            offset
                .checked_add(suffix)
                .ok_or("overlay offset overflow")?,
        )
        .map_err(|_| "overlay offset exceeds address space")?;
        let value = if encode {
            (value as u32).wrapping_add(site)
        } else {
            (value as u32).wrapping_sub(site)
        };
        let high = 0xf000 | ((value >> 12) & 0x7ff) as u16;
        let low = 0xf800 | ((value >> 1) & 0x7ff) as u16;
        result[suffix - 2..suffix].copy_from_slice(&high.to_le_bytes());
        result[suffix..suffix + 2].copy_from_slice(&low.to_le_bytes());
    }
    Ok(result)
}

pub fn load(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    transform(bytes, offset, false)
}

pub fn encode(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    let encoded = transform(bytes, offset, true)?;
    if load(&encoded, offset)? != bytes {
        return Err("overlay serialization failed the loader round trip".into());
    }
    Ok(encoded)
}

/// Bind legacy disassembler names once from reference sites. Candidate offsets
/// are deliberately not an input: moving a call must not change its destination.
pub fn call_symbols(
    reference: &[u8],
    offset: usize,
    extent: usize,
) -> Result<BTreeMap<String, BTreeSet<u64>>, String> {
    let mut symbols = BTreeMap::<String, BTreeSet<u64>>::new();
    for (_, name, target) in call_sites(reference, offset, extent)? {
        symbols.entry(name).or_default().insert(target);
    }
    Ok(symbols)
}

pub fn call_sites(
    reference: &[u8],
    offset: usize,
    extent: usize,
) -> Result<Vec<(usize, String, u64)>, String> {
    let end = offset
        .checked_add(extent)
        .ok_or("overlay extent overflow")?;
    let body = reference
        .get(offset..end)
        .ok_or("overlay owner exceeds reference")?;
    if load(reference, 0)?[offset..end] != load(body, offset)? {
        return Err("overlay owner boundary splits a loader relocation".into());
    }
    let (_, references) =
        psynergy::thumb::relocation_info(body, u64::from(RESOURCE_BASE) + offset as u64);
    let literals = references
        .iter()
        .filter(|site| site.0 == b'L')
        .flat_map(|site| site.2..site.2 + 4)
        .collect::<BTreeSet<_>>();
    let mut sites = Vec::new();
    for site in (0..body.len().saturating_sub(3)).step_by(2) {
        if (site..site + 4).any(|at| literals.contains(&at)) {
            continue;
        }
        let Some(value) = displacement(&body[site..site + 4]) else {
            continue;
        };
        let target = i64::from(value) + 2;
        if target < 0 || target as usize >= reference.len() {
            continue;
        }
        let legacy = i64::from(RESOURCE_BASE) + (offset + site + 4) as i64 + i64::from(value);
        sites.push((
            site,
            format!("Func_{legacy:08x}"),
            u64::from(RUNTIME_BASE) + target as u64,
        ));
    }
    Ok(sites)
}

pub fn resolve_call(
    name: &str,
    symbols: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<AbsoluteSymbol, String> {
    let targets = symbols.get(name).ok_or_else(|| {
        format!("{name}: no stable overlay call binding; declare the runtime symbol in its translation unit")
    })?;
    if targets.len() != 1 {
        return Err(format!("{name}: ambiguous overlay call identity ({targets:x?}); separate the C declarations and declare their runtime symbols"));
    }
    Ok(AbsoluteSymbol {
        address: *targets.first().unwrap(),
        kind: AbsoluteSymbolKind::Thumb,
    })
}

pub fn external(
    name: &str,
    is_call: bool,
    reference: &[u8],
    calls: &BTreeMap<String, BTreeSet<u64>>,
) -> Result<AbsoluteSymbol, String> {
    if name.starts_with("_call_via_") {
        let register = crate::compiler::symbols::external_symbol(name, 0)
            .ok_or("invalid call-via register")?
            .address
            / 4;
        let mut banks = BTreeSet::new();
        for target in calls.values().flatten() {
            let offset = (*target - u64::from(RUNTIME_BASE)) as usize;
            let instruction = u16::from_le_bytes(
                reference
                    .get(offset..offset + 2)
                    .ok_or("call-via target exceeds reference")?
                    .try_into()
                    .unwrap(),
            );
            if instruction & 0xff87 != 0x4700 {
                continue;
            }
            let reg = usize::from((instruction >> 3) & 15);
            let Some(base) = offset.checked_sub(reg * 4) else {
                continue;
            };
            if (0..14).all(|r| {
                reference.get(base + r * 4..base + r * 4 + 2)
                    == Some(&(0x4700u16 | (r as u16) << 3).to_le_bytes()[..])
            }) {
                banks.insert(u64::from(RUNTIME_BASE) + base as u64);
            }
        }
        if banks.len() != 1 {
            return Err(format!(
                "{name}: expected one reference-proven call-via bank, found {}",
                banks.len()
            ));
        }
        return Ok(AbsoluteSymbol {
            address: banks.first().unwrap() + register * 4,
            kind: AbsoluteSymbolKind::Thumb,
        });
    }
    if is_call && name.starts_with("Func_02") {
        return resolve_call(name, calls);
    }
    let symbol =
        crate::compiler::symbols::external_symbol(name, crate::compiler::symbols::CALL_VIA_BASE)
            .ok_or_else(|| format!("unsupported overlay symbol: {name}"))?;
    Ok(AbsoluteSymbol {
        address: symbol.address,
        kind: if symbol.thumb {
            AbsoluteSymbolKind::Thumb
        } else {
            AbsoluteSymbolKind::Data
        },
    })
}

/// A game's English main image, read once: the ROM its overlay import
/// veneers reach.
pub fn main_image(target: CompilerTarget) -> Result<&'static [u8], String> {
    static IMAGES: [OnceLock<Result<Vec<u8>, String>>; 2] = [OnceLock::new(), OnceLock::new()];
    let (index, id) = match target {
        CompilerTarget::Tbs => (0, crate::targets::DecompTargetId::TbsEn),
        CompilerTarget::Tla => (1, crate::targets::DecompTargetId::TlaEn),
    };
    IMAGES[index]
        .get_or_init(|| {
            let path = crate::compiler::routing::root().join(crate::targets::target_for(id).rom);
            std::fs::read(&path).map_err(|error| format!("{}: {error}", path.display()))
        })
        .as_deref()
        .map_err(Clone::clone)
}

/// Each main-image import veneer of an overlay, by runtime address, with the
/// main addresses a call through it passes: its target, then each far-call
/// veneer (`raw/0808a080.s`) up to the final function. The loader never
/// rewrites a veneer, so either image form serves.
pub fn import_veneers(reference: &[u8], main: &[u8]) -> Vec<(u64, Vec<u32>)> {
    (0..reference.len().saturating_sub(7))
        .step_by(4)
        .filter_map(|offset| {
            let chain = main_call_chain(main, veneer_target(&reference[offset..])?);
            (!chain.is_empty()).then(|| (u64::from(RUNTIME_BASE) + offset as u64, chain))
        })
        .collect()
}

fn main_call_chain(main: &[u8], mut target: u32) -> Vec<u32> {
    let mut chain = Vec::new();
    loop {
        let address = target & !1;
        let Some(offset) = address
            .checked_sub(MAIN_BASE)
            .map(|offset| offset as usize)
            .filter(|offset| *offset < main.len())
        else {
            break;
        };
        if chain.contains(&address) {
            break;
        }
        chain.push(address);
        match veneer_target(&main[offset..]) {
            Some(next) if target & 1 != 0 && offset.is_multiple_of(4) => target = next,
            _ => break,
        }
    }
    chain
}

/// How an object's relocations reach one undefined symbol.
#[derive(Clone, Copy, Debug, Default, Eq, PartialEq)]
pub struct SymbolUses {
    /// A `bl` reaches it (`R_ARM_THM_CALL`).
    pub call: bool,
    /// Another relocation reaches it, such as a literal-pool pointer.
    pub value: bool,
}
impl SymbolUses {
    pub fn record(&mut self, relocation: &str) {
        if relocation == "R_ARM_THM_CALL" {
            self.call = true;
        } else {
            self.value = true;
        }
    }
}

/// The image one overlay link binds its undefined names in.
pub struct OverlayImage<'a> {
    /// Its resource id, such as `resource_3bf`.
    pub overlay: &'a str,
    /// Its resource-form bytes: call sites, call-via banks and import veneers.
    pub reference: &'a [u8],
    /// The main image its import veneers reach, from `0x08000000`.
    pub main: &'a [u8],
    /// The owner register whose names bind here.
    pub names: &'a SourcePaths,
}
impl OverlayImage<'_> {
    /// Bind one undefined name: a declared absolute symbol, then a registered
    /// name, then the legacy call keys, hex names and call-via banks.
    pub fn resolve(
        &self,
        name: &str,
        uses: SymbolUses,
        absolute_symbols: &BTreeMap<String, AbsoluteSymbol>,
        calls: &BTreeMap<String, BTreeSet<u64>>,
    ) -> Result<AbsoluteSymbol, String> {
        if let Some(symbol) = absolute_symbols.get(name) {
            return Ok(*symbol);
        }
        match self.registered(name, uses)? {
            Some(symbol) => Ok(symbol),
            None => external(name, uses.call, self.reference, calls),
        }
    }

    /// A registered name bound in this image: its one owner's runtime address,
    /// a called main function's one import veneer, or a main value's address.
    /// Address-encoded and call-via spellings keep their legacy meaning.
    pub fn registered(
        &self,
        name: &str,
        uses: SymbolUses,
    ) -> Result<Option<AbsoluteSymbol>, String> {
        let thumb = |address| AbsoluteSymbol {
            address,
            kind: AbsoluteSymbolKind::Thumb,
        };
        let legacy = crate::compiler::symbols::external_symbol(
            name,
            crate::compiler::symbols::CALL_VIA_BASE,
        );
        if legacy.is_some() {
            return Ok(None);
        }
        match self
            .names
            .overlay_owners_named(self.overlay, name)
            .as_slice()
        {
            [] => {}
            [owner] => {
                return Ok(Some(thumb(
                    u64::from(owner.address()) + u64::from(RUNTIME_BASE - RESOURCE_BASE),
                )))
            }
            owners => {
                let owners = owners.iter().map(|owner| owner.id()).collect::<Vec<_>>();
                return Err(format!(
                    "{name}: registered for several {} owners ({}); give each owner a distinct name",
                    self.overlay,
                    owners.join(", ")
                ));
            }
        }
        let Some(function) = self.names.main_symbol(name)? else {
            return Ok(None);
        };
        if !uses.call {
            return Ok(Some(thumb(u64::from(function))));
        }
        if uses.value {
            return Err(format!(
                "{name}: main:{function:08x} is both called and used as a value; a call binds through an import veneer and a value to the main address"
            ));
        }
        let veneers = import_veneers(self.reference, self.main)
            .into_iter()
            .filter(|(_, chain)| chain.contains(&function))
            .map(|(veneer, _)| veneer)
            .collect::<Vec<_>>();
        match veneers.as_slice() {
            [veneer] => Ok(Some(thumb(*veneer))),
            [] => Err(format!(
                "{name}: no import veneer in {} reaches main:{function:08x}; its imports are {}",
                self.overlay,
                self.import_list()
            )),
            _ => Err(format!(
                "{name}: import veneers {} in {} all reach main:{function:08x}; see {}",
                veneers
                    .iter()
                    .map(|veneer| format!("0x{veneer:08x}"))
                    .collect::<Vec<_>>()
                    .join(", "),
                self.overlay,
                self.import_list()
            )),
        }
    }

    /// The verified import list this image's retained listing includes.
    fn import_list(&self) -> String {
        let listing = self
            .names
            .source_root()
            .with_file_name("raw")
            .join("overlays")
            .join(format!("{}_overlay.s", self.overlay));
        std::fs::read_to_string(&listing)
            .ok()
            .and_then(|text| {
                text.lines().find_map(|line| {
                    let path = line.trim().strip_prefix(".include")?.trim();
                    let path = path.strip_prefix('"')?.strip_suffix('"')?;
                    path.ends_with("/IMPORT.INC").then(|| path.to_owned())
                })
            })
            .unwrap_or_else(|| format!("the IMPORT.INC that {} includes", listing.display()))
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn placeholder_extents_are_unique_positive_and_include_local_aliases() {
        let text = "AlchemyC_02000100:\n .space 2\n.L_02000102:\n .space 0x6\n bx lr\n";
        let lines = text.lines().collect::<Vec<_>>();
        let block = placeholder_block(&lines, 0x02000100).unwrap();
        assert_eq!((block.start, block.end, block.span), (0, 4, 8));
        assert_eq!(placeholder_extent(text, 0x02000100), Some(8));
        assert_eq!(placeholder_extent(text, 0x02000102), None);
        for body in [
            ".space 0",
            ".space -2",
            ".space nope",
            ".L_02000100:",
            ".space 9223372036854775807\n.space 1",
        ] {
            assert_eq!(
                placeholder_extent(&format!("AlchemyC_02000100:\n{body}\n"), 0x02000100),
                None
            );
        }
        assert_eq!(
            placeholder_extent(&format!("{text}{text}"), 0x02000100),
            None
        );
    }

    #[test]
    fn data_placeholders_are_unique_labelled_space_blocks() {
        let text = "AlchemyData_020002d0:\n\t.space 0x100\n\t.space 0x100\n";
        assert_eq!(data_placeholder_extent(text, 0x020002d0), Some(0x200));
        assert_eq!(data_placeholder_extent(text, 0x020002d4), None);
        assert_eq!(
            data_placeholder_extent(&format!("{text}{text}"), 0x020002d0),
            None
        );
        assert_eq!(
            data_placeholder_extent("AlchemyData_020002d0:\n\t.4byte 1\n", 0x020002d0),
            None
        );
        assert_eq!(
            data_placeholder_extent("AlchemyC_020002d0:\n\t.space 4\n", 0x020002d0),
            None
        );
    }

    fn pair(value: i32) -> Vec<u8> {
        [
            0xf000 | (((value as u32) >> 12) & 0x7ff) as u16,
            0xf800 | (((value as u32) >> 1) & 0x7ff) as u16,
        ]
        .into_iter()
        .flat_map(u16::to_le_bytes)
        .collect()
    }

    #[test]
    fn loader_transforms_literals_and_preserves_ordinary_data() {
        let mut data = pair(0x1400);
        data.extend_from_slice(&0x02008101u32.to_le_bytes());
        let runtime = load(&data, 0x23fe).unwrap();
        assert_eq!(&runtime[..4], &pair(-0x1000));
        assert_eq!(&runtime[4..], &data[4..]);
        assert_eq!(encode(&runtime, 0x23fe).unwrap(), data);
    }

    #[test]
    fn codec_round_trips_signed_calls_at_different_owner_offsets() {
        for offset in [0, 2, 0x104, 0x2000, 0xfffe] {
            for value in [-0x400000, -0x1000, -2, 0, 2, 0x1400, 0x3ffffe] {
                let runtime = pair(value);
                assert_eq!(
                    load(&encode(&runtime, offset).unwrap(), offset).unwrap(),
                    runtime
                );
            }
        }
        assert!(load(&[0], 0).is_err());
        assert!(encode(&[0, 0], 1).is_err());
    }

    #[test]
    fn call_names_are_bound_from_reference_not_candidate_positions() {
        let mut reference = vec![0; 0x200];
        reference[0x20..0x24].copy_from_slice(&pair(0x7e));
        let bindings = call_symbols(&reference, 0x20, 4).unwrap();
        let target = resolve_call("Func_020000a2", &bindings).unwrap();
        assert_eq!(target.address, u64::from(RUNTIME_BASE) + 0x80);
        let moved = pair(target.address as i32 - (RUNTIME_BASE + 0x40 + 4) as i32);
        assert_eq!(displacement(&moved), Some(0x3c));
        assert_ne!(encode(&moved, 0x20).unwrap(), reference[0x20..0x24]);
    }

    #[test]
    fn colliding_legacy_names_require_explicit_distinct_bindings() {
        let mut reference = vec![0; 0x200];
        reference[0x20..0x24].copy_from_slice(&pair(0x7e));
        reference[0x40..0x44].copy_from_slice(&pair(0x5e));
        let bindings = call_symbols(&reference, 0, reference.len()).unwrap();
        assert!(resolve_call("Func_020000a2", &bindings)
            .unwrap_err()
            .contains("ambiguous"));
        assert!(resolve_call("Func_020000a2_a", &bindings).is_err());
    }

    fn veneer(target: u32) -> Vec<u8> {
        [0x4720_4c00, target]
            .into_iter()
            .flat_map(u32::to_le_bytes)
            .collect()
    }

    #[test]
    fn import_veneer_chain_resolves_through_main_far_call_veneer() {
        let mut main = vec![0; 0x100];
        main[0x40..0x48].copy_from_slice(&veneer(0x0800_0081));
        main[0x80..0x82].copy_from_slice(&0xb500u16.to_le_bytes());
        main[0xc0..0xc8].copy_from_slice(&veneer(0x0800_00c1));
        main[0xe2..0xea].copy_from_slice(&veneer(0x0800_0081));
        let mut reference = vec![0; 0x40];
        for (offset, target) in [
            (0x00, 0x0200_8101),
            (0x10, 0x0800_0041),
            (0x18, 0x0800_0081),
            (0x20, 0x0800_00c1),
            (0x28, 0x0800_00e3),
            (0x30, 0x0800_0040),
        ] {
            reference[offset..offset + 8].copy_from_slice(&veneer(target));
        }
        // Veneers start on word boundaries; a halfword-offset shape loads
        // the wrong word and is not one.
        let mut shifted = vec![0; 2];
        shifted.extend_from_slice(&veneer(0x0800_0081));
        shifted.resize(0x10, 0);
        assert_eq!(
            import_veneers(&reference, &main),
            [
                (0x0200_8010, vec![0x0800_0040, 0x0800_0080]),
                (0x0200_8018, vec![0x0800_0080]),
                (0x0200_8020, vec![0x0800_00c0]),
                (0x0200_8028, vec![0x0800_00e2]),
                (0x0200_8030, vec![0x0800_0040]),
            ]
        );
        assert!(import_veneers(&shifted, &main).is_empty());
    }

    #[test]
    fn import_veneer_scan_finds_exactly_each_verified_import_list() {
        let root = crate::compiler::routing::root();
        let rom = crate::overlay::rom::CanonicalRom::load(root).unwrap();
        let main = main_image(CompilerTarget::Tbs).unwrap();
        let names = SourcePaths::load(root).unwrap();
        let listings = root.join("games/THE BROKEN SEAL/raw/overlays");
        let mut overlays = 0;
        for entry in std::fs::read_dir(listings).unwrap() {
            let file = entry.unwrap().file_name().into_string().unwrap();
            let Some(overlay) = file.strip_suffix("_overlay.s") else {
                continue;
            };
            let reference = rom.overlay(overlay).unwrap();
            let image = OverlayImage {
                overlay,
                reference: &reference,
                main,
                names: &names,
            };
            let list = std::fs::read_to_string(root.join(image.import_list())).unwrap();
            // Imports of IWRAM routines are veneers too, but reach no main owner.
            let listed = list
                .split(|c: char| c == ',' || c.is_whitespace())
                .filter_map(|word| u32::from_str_radix(word.strip_prefix("0x")?, 16).ok())
                .map(|target| target & !1)
                .filter(|target| (MAIN_BASE..MAIN_BASE + main.len() as u32).contains(target))
                .collect::<Vec<_>>();
            let found = import_veneers(&reference, main)
                .into_iter()
                .map(|(_, chain)| chain[0])
                .collect::<Vec<_>>();
            assert_eq!(found, listed, "{overlay}");
            overlays += 1;
        }
        assert!(overlays > 0);
    }

    #[test]
    fn a_pool_pair_is_loaded_but_never_becomes_a_call_binding() {
        let mut reference = vec![0; 0x100];
        reference[..2].copy_from_slice(&0x4800u16.to_le_bytes());
        reference[2..4].copy_from_slice(&0x4770u16.to_le_bytes());
        reference[4..8].copy_from_slice(&pair(0x7e));
        assert!(call_symbols(&reference, 0, 8).unwrap().is_empty());
        assert_ne!(load(&reference, 0).unwrap(), reference);
        assert!(call_symbols(&reference, 6, 2)
            .unwrap_err()
            .contains("boundary"));
    }
}
