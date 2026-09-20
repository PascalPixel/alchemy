//! Derive native source index rows for field scenes from a ROM.
//!
//! A scene record names its code overlay and a map load record; the load record
//! names one map container, its palette stream and its tile banks. This reads
//! those tables, traces every stream into a token plan, stages the native
//! sources the plans encode, rebuilds each region through `build_entry`, and
//! prints the rows only when every rebuilt region equals the ROM. It never
//! writes a game's SOURCE.JSON; the registrar merges the printed rows.
use super::native::NativePaths;
use super::*;
use crate::targets::{decomp_target, DecompTarget, DecompTargetId};
use psynergy::assets::lz::{GeneralToken, Mtf4LzToken};
use serde_json::json;

pub(super) mod network;

const DECODED_LIMIT: u64 = 0x2_0000;
const TILE_BANK: usize = 0x4000;
const TILES_PER_BANK: usize = 512;

/// ROM tables of one edition, verified against the committed indexes.
struct FieldTables {
    /// 8-byte records: u16 overlay, u8 group, u8 variant, u16 load record, u16 flag.
    scenes: usize,
    /// 12-byte records: six u16 resource ids less `bias`.
    loads: usize,
    /// The first map container's resource id; load fields are relative to it.
    bias: usize,
    /// Whether the game's committed SOURCE.JSON indexes this edition's ROM.
    /// Another edition's rows are derived without consulting its placements.
    indexed: bool,
}
fn field_tables(target: &DecompTarget) -> Result<FieldTables, String> {
    match target.id {
        // The Broken Seal scene 187 is VINASU_CHOJO on container 32f.
        DecompTargetId::TbsEn => Ok(FieldTables {
            scenes: 0x0809_f1a8,
            loads: 0x0801_3784,
            bias: 0x128,
            indexed: true,
        }),
        // Loaded by 0x0802a650 and 0x0802a6b8.
        DecompTargetId::TlaEn => Ok(FieldTables {
            scenes: 0x080f_17a8,
            loads: 0x0802_f380,
            bias: 0x26c,
            indexed: true,
        }),
        // Byte-identical to tla-en's tables, moved by the edition's text:
        // loaded by 0x0802a5cc and 0x0802a634, whose pools name 0x0802f2fc
        // and the same 0x26c bias; 0x080c9930 names the scene table.
        DecompTargetId::TlaJa => Ok(FieldTables {
            scenes: 0x080f_16b4,
            loads: 0x0802_f2fc,
            bias: 0x26c,
            indexed: false,
        }),
        id => Err(format!(
            "field scene and load tables are verified only for tbs-en, tla-en and tla-ja, not {id}"
        )),
    }
}
/// Load record fields after the container and palette.
fn loader_fields(target: &DecompTarget) -> [&'static str; 4] {
    match target.compiler {
        CompilerTarget::Tbs => [
            "vram_charblock1",
            "vram_charblock2",
            "vram_charblock3",
            "animation_source",
        ],
        CompilerTarget::Tla => [
            "vram_charblock1",
            "vram_charblock2",
            "vram_charblock3",
            "vram_charblock4",
        ],
    }
}
fn component_slots(target: &DecompTarget) -> usize {
    match target.compiler {
        CompilerTarget::Tbs => 6,
        CompilerTarget::Tla => 7,
    }
}

fn u16_at(rom: &[u8], offset: usize) -> Result<u16, String> {
    rom.get(offset..offset + 2)
        .map(|b| u16::from_le_bytes([b[0], b[1]]))
        .ok_or_else(|| format!("0x{:08x} lies beyond the ROM", offset + ROM_BASE))
}
fn u32_at(rom: &[u8], offset: usize) -> Result<u32, String> {
    rom.get(offset..offset + 4)
        .map(|b| u32::from_le_bytes([b[0], b[1], b[2], b[3]]))
        .ok_or_else(|| format!("0x{:08x} lies beyond the ROM", offset + ROM_BASE))
}
fn small_hex(value: usize) -> String {
    format!("0x{value:x}")
}
fn resource_name(id: usize) -> String {
    format!("{id:03x}")
}
fn hex_text(bytes: &[u8]) -> String {
    bytes.iter().map(|b| format!("{b:02x}")).collect()
}

/// The resource directory: each id's ROM offset and the extent to the next
/// greater directory address.
struct Directory {
    offsets: Vec<usize>,
    sorted: Vec<usize>,
    rom_size: usize,
}
impl Directory {
    fn read(rom: &[u8]) -> Result<Self, String> {
        let table = crate::overlay::rom::resource_table(rom)?;
        let mut offsets = Vec::new();
        loop {
            let at = table + offsets.len() * 4;
            let Ok(address) = u32_at(rom, at) else { break };
            match (address as usize).checked_sub(ROM_BASE) {
                Some(offset) if offset < rom.len() => offsets.push(offset),
                _ => break,
            }
        }
        let mut sorted = offsets.clone();
        sorted.sort_unstable();
        sorted.dedup();
        Ok(Self {
            offsets,
            sorted,
            rom_size: rom.len(),
        })
    }
    fn resource(&self, id: usize) -> Result<(usize, usize), String> {
        let offset = *self
            .offsets
            .get(id)
            .ok_or_else(|| format!("resource {id:03x} is outside the directory"))?;
        let next = self.sorted[self.sorted.partition_point(|a| *a <= offset)..]
            .first()
            .copied()
            .unwrap_or(self.rom_size);
        Ok((offset, next - offset))
    }
}

/// One traced stream: its explicit plan and decoded bytes.
struct Stream {
    plan: Value,
    decoded: Vec<u8>,
}

/// Exact stored extent, excluding decoder read-ahead and inter-resource gaps.
pub(crate) fn tagged_extent(
    rom: &[u8],
    start: usize,
    end: usize,
) -> Result<(Vec<u8>, usize), String> {
    let (decoded, encoded) = match rom.get(start) {
        Some(0) => {
            [0usize, 0x1000]
                .into_iter()
                .find_map(|prefill| {
                    let (data, _, tokens) =
                        psynergy::assets::lz::decode_general_prefill_trace(
                            rom,
                            start,
                            end,
                            DECODED_LIMIT,
                            prefill,
                            1,
                        )
                        .ok()?;
                    let encoded = psynergy::assets::lz::encode_general_prefill(
                        &data, &tokens, prefill, 1,
                    )
                    .ok()?;
                    (rom.get(start..start + encoded.len()) == Some(encoded.as_slice()))
                        .then_some((data, encoded))
                })
                .ok_or("general stream does not decode and re-encode with an empty or 4 KB zero dictionary")?
        }
        Some(1) => {
            let (data, _, groups) =
                psynergy::assets::lz::decode_palette_trace(rom, start + 1, end, DECODED_LIMIT)
                    .map_err(|e| e.to_string())?;
            let mut encoded = vec![1];
            encoded.extend(
                psynergy::assets::lz::encode_palette(&data, &groups).map_err(|e| e.to_string())?,
            );
            (data, encoded)
        }
        Some(2) => {
            let (data, tokens) = trace_mtf4(rom, start, end)?;
            let encoded =
                psynergy::assets::lz::encode_mtf4_lz(&data, &tokens).map_err(|e| e.to_string())?;
            (data, encoded)
        }
        _ => return Err("stream has no supported compression tag".into()),
    };
    if rom.get(start..start + encoded.len()) != Some(encoded.as_slice())
        || start + encoded.len() > end
    {
        return Err("stream does not reproduce its physical encoding".into());
    }
    Ok((decoded, encoded.len()))
}

/// Exact extent of the engine's untagged byte-LZ stream. The ARM reader at
/// 0x0800165c uses the same flag groups, copy fields and terminator as tagged
/// kind 1, but several system loaders pass it the resource start directly.
fn untagged_byte_lz_extent(
    rom: &[u8],
    start: usize,
    end: usize,
) -> Result<(Vec<u8>, usize), String> {
    let (decoded, _, groups) =
        psynergy::assets::lz::decode_palette_trace(rom, start, end, DECODED_LIMIT)
            .map_err(|error| error.to_string())?;
    let encoded = psynergy::assets::lz::encode_palette(&decoded, &groups)
        .map_err(|error| error.to_string())?;
    if rom.get(start..start + encoded.len()) != Some(encoded.as_slice())
        || start + encoded.len() > end
    {
        return Err("untagged byte-LZ stream does not reproduce its physical encoding".into());
    }
    Ok((decoded, encoded.len()))
}

pub(super) fn inventory(
    root: &Path,
    rom: &[u8],
    target: &DecompTarget,
) -> Result<(Vec<Value>, Vec<Value>), String> {
    let tables = field_tables(target)?;
    let directory = Directory::read(rom)?;
    let scene_path = format!("{}/FIELD/COMMON/SCENE_TABLE.JSON", target.source_dir);
    let scenes = json(&root.join(&scene_path))?;
    let count = scenes["segments"][0]["records"]
        .as_array()
        .ok_or("missing scene records")?
        .len();
    let mut roles = BTreeMap::<usize, (&str, usize)>::new();
    for index in 0..count {
        let load = u16_at(rom, tables.scenes - ROM_BASE + index * 8 + 4)? as usize;
        for slot in 0..6 {
            let id =
                u16_at(rom, tables.loads - ROM_BASE + load * 12 + slot * 2)? as usize + tables.bias;
            let kind = match slot {
                0 => "golden-sun-map-container",
                1 => "bgr555-banks",
                _ => "gba-4bpp-tiles",
            };
            roles.entry(id).or_insert((kind, load));
        }
    }
    let mut rows = vec![];
    let mut failures = vec![];
    for (id, (kind, load)) in roles {
        let result = (|| -> Result<Vec<(usize, usize, &str)>, String> {
            let (base, size) = directory.resource(id)?;
            if kind != "golden-sun-map-container" {
                let (data, size) = tagged_extent(rom, base, base + size)?;
                if (kind == "bgr555-banks" && (data.is_empty() || data.len() % 32 != 0))
                    || (kind == "gba-4bpp-tiles" && data.len() != TILE_BANK)
                {
                    return Err("decoded field resource has wrong dimensions".into());
                }
                return Ok(vec![(base, size, kind)]);
            }
            let slots = component_slots(target);
            let header = 0x24 + slots * 4;
            let offsets = (0..slots)
                .map(|k| u32_at(rom, base + 0x24 + k * 4).map(|n| n as usize))
                .collect::<Result<Vec<_>, _>>()?;
            if offsets[0] != header
                || offsets
                    .iter()
                    .any(|o| *o != 0 && (*o < header || *o >= size))
            {
                return Err("invalid map component directory".into());
            }
            let mut spans = vec![(base, header, kind)];
            for offset in offsets.into_iter().filter(|o| *o != 0) {
                let (_, length) = tagged_extent(rom, base + offset, base + size)?;
                spans.push((base + offset, length, kind));
            }
            Ok(spans)
        })();
        match result {
            Ok(spans) => {
                for (start, size, kind) in spans {
                    rows.push(json!({"start":start + ROM_BASE,"end":start + ROM_BASE + size,"kind":kind,"label":match kind {"bgr555-banks"=>"Field palette","gba-4bpp-tiles"=>"Field tiles",_=>"Map component"},"resource":format!("{id:03x}"),"evidence":format!("field load record {load}, resource {id:03x}; decoded and re-encoded extent")}));
                }
            }
            Err(error) => failures.push(json!({"resource":format!("{id:03x}"),"reason":error})),
        }
    }
    // Try only layouts already reconstructed in the sibling game's index.
    // Exact re-encoding up to the next directory pointer establishes a format
    // match, not the image's scene or purpose.
    let sibling = json(&root.join("games/THE BROKEN SEAL/SOURCE.JSON"))?;
    let sibling_layouts = sibling["private_inputs"]
        .as_array()
        .ok_or("missing private input index")?
        .iter()
        .filter(|r| r["kind"] == "still-atlas")
        .filter_map(|r| {
            Some((
                r["width"].as_u64()? as usize,
                r["height"].as_u64()? as usize,
                r["palette_entries"].as_u64()? as usize,
            ))
        })
        .collect::<BTreeSet<_>>();
    let mut layouts = sibling_layouts.clone();
    // TLA resources 070-078 continue TBS's exact 256x120, 128-colour still
    // family. The following resources retain the palette and delta7 grammar
    // with exactly twice that pixel count before the family changes to raw
    // tiles at 081.
    layouts.insert((256, 240, 128));
    let mut still_anchors = BTreeSet::new();
    for id in 0..directory.offsets.len() {
        let (start, size) = directory.resource(id)?;
        if size > 0x10000 {
            continue;
        }
        for &(width, height, colors) in &layouts {
            let palette = colors * 2;
            if size <= palette
                || rom[start..start + palette]
                    .chunks_exact(2)
                    .any(|c| c[1] & 0x80 != 0)
            {
                continue;
            }
            let body = &rom[start + palette..start + size];
            let Ok(pixels) = psynergy::assets::compression::decode_delta7(body, width * height)
            else {
                continue;
            };
            let Ok(encoded) = psynergy::assets::compression::encode_delta7(&pixels) else {
                continue;
            };
            if encoded.len() > body.len()
                || body.len() - encoded.len() > 3
                || body[..encoded.len()] != encoded
            {
                continue;
            }
            let source = if sibling_layouts.contains(&(width, height, colors)) {
                "sibling layout"
            } else {
                "adjacent exact still family and doubled 256x120 canvas"
            };
            rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+palette+encoded.len(),"kind":"golden-sun-delta7-still","label":"Indexed still image","resource":format!("{id:03x}"),"evidence":format!("{width}x{height} delta7 pixels and {colors} BGR555 colours; {source}, exact re-encoding to directory boundary; scene not established")}));
            still_anchors.insert(id);
            break;
        }
    }
    // Localized and sequel-only stills need not share dimensions with an
    // already reconstructed image. Extend only through the uninterrupted
    // physical family around exact re-encoding anchors: every member begins
    // with the same complete 128-colour BGR555 palette, and the run stops at
    // the first resource which violates that invariant.
    if !still_anchors.is_empty() {
        let belongs = |id: usize| {
            directory.resource(id).is_ok_and(|(start, size)| {
                size > 256
                    && rom.get(start..start + 256).is_some_and(|palette| {
                        palette.chunks_exact(2).all(|word| word[1] & 0x80 == 0)
                    })
            })
        };
        let mut claimed = BTreeSet::new();
        for anchor in still_anchors.iter().copied() {
            if claimed.contains(&anchor) {
                continue;
            }
            let mut first = anchor;
            let mut last = anchor;
            while first > 0 && belongs(first - 1) {
                first -= 1;
            }
            while last + 1 < directory.offsets.len() && belongs(last + 1) {
                last += 1;
            }
            let anchors = still_anchors.range(first..=last).count();
            for id in first..=last {
                claimed.insert(id);
                let (start, size) = directory.resource(id)?;
                rows.push(json!({
                    "start":start+ROM_BASE,
                    "end":start+ROM_BASE+size,
                    "kind":"golden-sun-delta7-still",
                    "label":"Indexed still-image resource",
                    "resource":format!("{id:03x}"),
                    "evidence":format!("uninterrupted 128-colour still family {first:03x}-{last:03x}, anchored by {anchors} exact delta7 re-encodings; individual layout not asserted")
                }));
            }
        }
    }
    // Some system resources are directories of independently delta-coded
    // 16-bit tilemaps. The first offset is also the complete directory size;
    // accepting the package requires every offset to increase and every
    // member to decode and re-encode byte for byte within its boundary.
    for id in 0..directory.offsets.len() {
        let (start, size) = directory.resource(id)?;
        let Ok(first) = u32_at(rom, start).map(|value| value as usize) else {
            continue;
        };
        if first < 8 || first > size || first % 4 != 0 || first / 4 > 512 {
            continue;
        }
        let count = first / 4;
        let offsets = (0..count)
            .map(|index| u32_at(rom, start + index * 4).map(|value| value as usize))
            .collect::<Result<Vec<_>, _>>()?;
        if offsets[0] != first
            || offsets.iter().any(|offset| *offset >= size)
            || offsets.windows(2).any(|pair| pair[0] >= pair[1])
        {
            continue;
        }
        let mut members = vec![];
        for (member, offset) in offsets.iter().copied().enumerate() {
            let end = offsets.get(member + 1).copied().unwrap_or(size);
            let stored = &rom[start + offset..start + end];
            let exact = (0..=3).find_map(|tail| {
                if stored.len() <= tail
                    || stored[stored.len() - tail..].iter().any(|byte| *byte != 0)
                {
                    return None;
                }
                let encoded = &stored[..stored.len() - tail];
                let decoded = psynergy::assets::compression::decode_tilemap_delta(encoded).ok()?;
                (psynergy::assets::compression::encode_tilemap_delta(&decoded, encoded[0]).ok()?
                    == encoded)
                    .then_some(encoded.len())
            });
            let Some(encoded) = exact else {
                members.clear();
                break;
            };
            members.push((start + offset, encoded));
        }
        if members.len() != count {
            continue;
        }
        rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+first,"kind":"record-table","label":"Tilemap resource directory","resource":format!("{id:03x}"),"evidence":format!("{count} increasing offsets; first offset equals directory size")}));
        for (member, (address, encoded)) in members.into_iter().enumerate() {
            rows.push(json!({"start":address+ROM_BASE,"end":address+ROM_BASE+encoded,"kind":"gba-tilemap16","label":"Delta-coded tilemap","resource":format!("{id:03x}"),"evidence":format!("tilemap bundle member {member}; exact decode and mode-preserving re-encode within the next offset")}));
        }
    }
    // Portrait resources begin with a u16 offset directory. Empty character
    // slots are zero; every populated member is one 16-colour palette followed
    // by an independently terminated MTF4 32x32 image. The runtime portrait
    // loader selects the u16 offset, DMA-copies 32 palette bytes, then gives
    // member+32 to the portrait reader. Require every member to round-trip.
    for id in 0..directory.offsets.len() {
        let (start, size) = directory.resource(id)?;
        let first = usize::from(u16_at(rom, start)?);
        if first < 4 || first > size || first % 2 != 0 || first / 2 > 512 {
            continue;
        }
        let slots = first / 2;
        let offsets = (0..slots)
            .map(|slot| u16_at(rom, start + slot * 2).map(usize::from))
            .collect::<Result<Vec<_>, _>>()?;
        let populated = offsets
            .iter()
            .copied()
            .filter(|offset| *offset != 0)
            .collect::<Vec<_>>();
        if populated.len() < 2
            || populated[0] != first
            || populated.iter().any(|offset| *offset >= size)
            || populated.windows(2).any(|pair| pair[0] >= pair[1])
        {
            continue;
        }
        let mut members = Vec::new();
        for (member, offset) in populated.iter().copied().enumerate() {
            let end = populated.get(member + 1).copied().unwrap_or(size);
            if end < offset + 34
                || rom[start + offset..start + offset + 32]
                    .chunks_exact(2)
                    .any(|word| word[1] & 0x80 != 0)
            {
                members.clear();
                break;
            }
            let stored = &rom[start + offset + 32..start + end];
            let Ok(pixels) = psynergy::assets::compression::decode_mtf4(stored, 32 * 32) else {
                members.clear();
                break;
            };
            let Ok(encoded) = psynergy::assets::compression::encode_mtf4(&pixels) else {
                members.clear();
                break;
            };
            if encoded.len() > stored.len()
                || stored[..encoded.len()] != encoded
                || stored[encoded.len()..].len() > 3
                || stored[encoded.len()..].iter().any(|byte| *byte != 0)
            {
                members.clear();
                break;
            }
            members.push((offset, encoded.len(), stored.len() - encoded.len()));
        }
        if members.len() != populated.len() {
            continue;
        }
        rows.push(json!({
            "start":start+ROM_BASE,
            "end":start+ROM_BASE+first,
            "kind":"record-table",
            "label":"Portrait offset directory",
            "resource":format!("{id:03x}"),
            "evidence":format!("{slots} u16 slots; {} increasing populated offsets; first offset equals directory size",populated.len())
        }));
        for (member, (offset, encoded, padding)) in members.into_iter().enumerate() {
            let palette = start + offset;
            let pixels = palette + 32;
            rows.push(json!({"start":palette+ROM_BASE,"end":pixels+ROM_BASE,"kind":"bgr555-banks","label":"Portrait palette","resource":format!("{id:03x}"),"evidence":format!("portrait member {member}; 16 complete BGR555 entries selected independently by its offset") }));
            rows.push(json!({"start":pixels+ROM_BASE,"end":pixels+ROM_BASE+encoded,"kind":"gba-4bpp-tiles","label":"MTF4 portrait","resource":format!("{id:03x}"),"evidence":format!("portrait member {member}; exactly 1024 four-bit pixels and exact MTF4 re-encoding") }));
            if padding != 0 {
                rows.push(json!({"start":pixels+ROM_BASE+encoded,"end":pixels+ROM_BASE+encoded+padding,"kind":"byte-fill","label":"Portrait alignment","resource":format!("{id:03x}"),"evidence":"one-to-three zero bytes between exact portrait members"}));
            }
        }
    }
    // UI and still packages place a BGR555 palette directly before one tagged
    // pixel stream. Infer neither the picture dimensions nor its runtime role:
    // the palette domain and the stream's exact inverse establish only the
    // stored palette-plus-pixels format.
    for id in 0..directory.offsets.len() {
        let (start, size) = directory.resource(id)?;
        for colors in [16usize, 32, 64, 128, 192, 224, 240, 256] {
            let palette = colors * 2;
            if size <= palette + 4
                || rom[start..start + palette]
                    .chunks_exact(2)
                    .any(|word| word[1] & 0x80 != 0)
            {
                continue;
            }
            let Ok((pixels, encoded)) = tagged_extent(rom, start + palette, start + size) else {
                continue;
            };
            let tail = size - palette - encoded;
            // The bit reader refills whole words and may consume up to three
            // nonzero bytes beyond the encoder's end marker. The tail pass
            // below records those bytes explicitly as bounded lookahead.
            if pixels.is_empty() || tail > 3 {
                continue;
            }
            let tag = rom[start + palette];
            let kind = if tag != 0 || pixels.iter().all(|pixel| *pixel < 16) {
                "gba-4bpp-tiles"
            } else {
                "gba-8bpp-tiles"
            };
            rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+palette,"kind":"bgr555-banks","label":"Indexed graphics palette","resource":format!("{id:03x}"),"evidence":format!("{colors} complete BGR555 entries preceding an exact tagged pixel stream")}));
            rows.push(json!({"start":start+ROM_BASE+palette,"end":start+ROM_BASE+palette+encoded,"kind":kind,"label":"Tagged indexed graphics","resource":format!("{id:03x}"),"evidence":format!("tag {tag}; {} decoded indices; exact re-encoding to resource boundary",pixels.len())}));
            break;
        }
    }
    // A second untagged image package used by adjacent resources keeps a
    // 240-entry BGR555 palette before a shared raw 8bpp image prologue. Two
    // consecutive packages must agree on that prologue; a lone palette-like
    // prefix is not sufficient evidence for raw pixels.
    let raw8 = (0..directory.offsets.len())
        .filter_map(|id| {
            let (start, size) = directory.resource(id).ok()?;
            (size > 512
                && rom[start..start + 480]
                    .chunks_exact(2)
                    .all(|word| word[1] & 0x80 == 0))
            .then_some((id, start, size))
        })
        .collect::<Vec<_>>();
    for pair in raw8.windows(2) {
        if pair[1].0 != pair[0].0 + 1
            || rom[pair[0].1 + 480..pair[0].1 + 512] != rom[pair[1].1 + 480..pair[1].1 + 512]
        {
            continue;
        }
        for &(id, start, size) in pair {
            rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+480,"kind":"bgr555-banks","label":"Raw indexed-image palette","resource":format!("{id:03x}"),"evidence":"240 valid BGR555 entries; adjacent package has the same raw pixel prologue"}));
            rows.push(json!({"start":start+ROM_BASE+480,"end":start+ROM_BASE+size,"kind":"gba-8bpp-tiles","label":"Raw indexed graphics","resource":format!("{id:03x}"),"evidence":"adjacent 240-colour package has the same 32-byte raw pixel prologue"}));
        }
    }
    // Some raw 8bpp siblings share the palette itself rather than a pixel
    // prologue. Require two consecutive resources to share a complete large
    // engine palette (128, 224, 240 or 256 colours) before their bodies split.
    for id in 0..directory.offsets.len().saturating_sub(1) {
        let (left, left_size) = directory.resource(id)?;
        let (right, right_size) = directory.resource(id + 1)?;
        let common = rom[left..left + left_size]
            .iter()
            .zip(&rom[right..right + right_size])
            .take_while(|(a, b)| a == b)
            .count();
        let palette = [512usize, 480, 448, 256].into_iter().find(|bytes| {
            common >= *bytes
                && left_size > *bytes
                && right_size > *bytes
                && rom[left..left + *bytes]
                    .chunks_exact(2)
                    .all(|word| word[1] & 0x80 == 0)
        });
        let Some(palette) = palette else { continue };
        for (resource, start, size) in [(id, left, left_size), (id + 1, right, right_size)] {
            rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+palette,"kind":"bgr555-banks","label":"Shared indexed-image palette","resource":format!("{resource:03x}"),"evidence":format!("{} exact BGR555 colours shared by two consecutive resources",palette/2)}));
            rows.push(json!({"start":start+ROM_BASE+palette,"end":start+ROM_BASE+size,"kind":"gba-8bpp-tiles","label":"Raw indexed graphics","resource":format!("{resource:03x}"),"evidence":format!("body follows a shared {}-colour engine palette",palette/2)}));
        }
    }
    // Tag 2 is the engine's MTF4 pixel codec, so unlike a generic LZ stream it
    // identifies its payload as four-bit indexed graphics without a consumer.
    for id in 0..directory.offsets.len() {
        let (start, size) = directory.resource(id)?;
        if rom[start] != 2 {
            continue;
        }
        let Ok((pixels, encoded)) = tagged_extent(rom, start, start + size) else {
            continue;
        };
        let tail = size - encoded;
        if pixels.is_empty() || tail > 3 {
            continue;
        }
        rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+encoded,"kind":"gba-4bpp-tiles","label":"MTF4 indexed graphics","resource":format!("{id:03x}"),"evidence":format!("tag 2; {} packed 4bpp bytes; exact re-encoding with at most three bounded lookahead bytes",pixels.len())}));
    }
    // TLA's early system loader at 0x081a6b90 establishes one indexed-image
    // family independently of byte shape. It copies resource 01b whole to BG
    // palette RAM; 01c-01f are mutually selected pictures decoded through the
    // untagged byte-LZ wrapper at 0x0801591c into the same 0x3000-byte buffer.
    // Resource 01a is the sibling form: a 16-colour palette followed by that
    // same stream, with the two parts sent to palette RAM and 0x02010000.
    if matches!(target.id, DecompTargetId::TlaEn) {
        let (start, size) = directory.resource(0x01a)?;
        if size > 32
            && rom[start..start + 32]
                .chunks_exact(2)
                .all(|word| word[1] & 0x80 == 0)
        {
            if let Ok((pixels, encoded)) = untagged_byte_lz_extent(rom, start + 32, start + size) {
                if !pixels.is_empty() && size - 32 - encoded <= 3 {
                    rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+32,"kind":"bgr555-banks","label":"System image palette","resource":"01a","evidence":"loader 0x081a6edc skips 32 bytes before byte-LZ decode and 0x081a70a6 copies those 16 BGR555 colours to palette RAM"}));
                    rows.push(json!({"start":start+ROM_BASE+32,"end":start+ROM_BASE+32+encoded,"kind":"gba-4bpp-tiles","label":"System indexed graphics","resource":"01a","evidence":format!("loader-selected graphics beside a 16-colour palette; {} decoded bytes; exact untagged byte-LZ re-encoding",pixels.len())}));
                }
            }
        }

        let (palette, palette_size) = directory.resource(0x01b)?;
        if palette_size == 512
            && rom[palette..palette + palette_size]
                .chunks_exact(2)
                .all(|word| word[1] & 0x80 == 0)
        {
            rows.push(json!({"start":palette+ROM_BASE,"end":palette+ROM_BASE+palette_size,"kind":"bgr555-banks","label":"System image palette","resource":"01b","evidence":"loader 0x081a6bf4 copies all 256 BGR555 entries directly to BG palette RAM"}));
        }
        for id in 0x01c..=0x01f {
            let (start, size) = directory.resource(id)?;
            let Ok((pixels, encoded)) = untagged_byte_lz_extent(rom, start, start + size) else {
                continue;
            };
            if pixels.len() != 0x3000 || size - encoded > 3 {
                continue;
            }
            rows.push(json!({"start":start+ROM_BASE,"end":start+ROM_BASE+encoded,"kind":"gba-8bpp-tiles","label":"System indexed graphics","resource":format!("{id:03x}"),"evidence":"loader family 0x081a6c0e-0x081a6e68 selects resources 01c-01f beside the 256-colour resource 01b and decodes exactly 0x3000 bytes through 0x0801591c; exact untagged byte-LZ re-encoding"}));
        }
    }

    // Exact encoders stop at the end marker, while the decoder's word reader
    // can consume up to three lookahead bytes before the next resource. Once
    // a resource has an identified payload, retain nonzero lookahead as that
    // payload's storage and zero lookahead as alignment. An unowned tail or
    // an internal gap remains unidentified.
    for id in 0..directory.offsets.len() {
        let resource = resource_name(id);
        let last = rows
            .iter()
            .filter(|row| row["resource"].as_str() == Some(&resource))
            .filter_map(|row| row["end"].as_u64().map(|end| (end, row)))
            .max_by_key(|(end, _)| *end);
        let Some((end, last)) = last else { continue };
        let Some(end) = usize::try_from(end)
            .ok()
            .and_then(|address| address.checked_sub(ROM_BASE))
        else {
            continue;
        };
        let (start, size) = directory.resource(id)?;
        let boundary = start + size;
        let tail = boundary.saturating_sub(end);
        if !(1..=3).contains(&tail) {
            continue;
        }
        let Some(bytes) = rom.get(end..boundary) else {
            continue;
        };
        let zeros = bytes.iter().all(|byte| *byte == 0);
        let kind = if zeros {
            "byte-fill"
        } else {
            last["kind"].as_str().unwrap_or("unresolved-data")
        };
        if kind == "unresolved-data" {
            continue;
        }
        rows.push(json!({
            "start":end+ROM_BASE,
            "end":boundary+ROM_BASE,
            "kind":kind,
            "label":if zeros {"Resource alignment"} else {"Compression lookahead"},
            "resource":resource,
            "evidence":if zeros {
                "one-to-three zero bytes after an exactly identified resource payload, ending at the next directory pointer"
            } else {
                "one-to-three bounded bytes read past an exactly decoded and re-encoded stream before the next directory pointer; inherits the stream payload type"
            }
        }));
    }
    Ok((rows, failures))
}
fn general_tokens(tokens: &[GeneralToken]) -> Value {
    let mut rows: Vec<Value> = Vec::new();
    let mut literals = 0u32;
    for token in tokens {
        match token {
            GeneralToken::Literal(count) => literals += count,
            GeneralToken::Copy { length, distance } => {
                if literals > 0 {
                    rows.push(json!(["l", literals]));
                    literals = 0;
                }
                rows.push(json!(["c", length, distance]));
            }
        }
    }
    if literals > 0 {
        rows.push(json!(["l", literals]));
    }
    json!(rows)
}
fn palette_groups(groups: &[PaletteGroup]) -> Value {
    json!(groups
        .iter()
        .map(|group| match group {
            PaletteGroup::Zeros => json!(["z"]),
            PaletteGroup::Group(operations) => json!([
                "g",
                operations
                    .iter()
                    .map(|operation| match operation {
                        PaletteOperation::Literal => json!(["l"]),
                        PaletteOperation::End => json!(["e"]),
                        PaletteOperation::Copy { length, distance } =>
                            json!(["c", length, distance]),
                    })
                    .collect::<Vec<_>>()
            ]),
        })
        .collect::<Vec<_>>())
}

/// LSB-first reader over little-endian words, aligned to absolute ROM offsets
/// the way the tag-2 decoder reads them.
struct Bits<'a> {
    data: &'a [u8],
    cursor: usize,
    end: usize,
    value: u64,
    count: u32,
}
impl<'a> Bits<'a> {
    fn new(data: &'a [u8], cursor: usize, end: usize) -> Result<Self, String> {
        let mut bits = Self {
            data,
            cursor,
            end,
            value: 0,
            count: 0,
        };
        if cursor & 1 == 1 {
            bits.value = u64::from(*data.get(cursor).ok_or("tag-2 input ended")?);
            bits.cursor += 1;
            bits.count = 8;
        }
        bits.fill()?;
        Ok(bits)
    }
    fn fill(&mut self) -> Result<(), String> {
        if self.cursor + 2 > self.end {
            return Err("tag-2 input ended".into());
        }
        let word = u16::from_le_bytes([self.data[self.cursor], self.data[self.cursor + 1]]);
        self.value |= u64::from(word) << self.count;
        self.cursor += 2;
        self.count += 16;
        Ok(())
    }
    fn get(&mut self, width: u32) -> Result<u32, String> {
        while self.count < width {
            self.fill()?;
        }
        let value = (self.value & ((1u64 << width) - 1)) as u32;
        self.value >>= width;
        self.count -= width;
        Ok(value)
    }
}
/// The tag-2 stream's tokens, in the plan spelling: a bare literal width or
/// `[distance, length]`.
fn trace_mtf4(
    data: &[u8],
    start: usize,
    end: usize,
) -> Result<(Vec<u8>, Vec<Mtf4LzToken>), String> {
    if data.get(start) != Some(&2) {
        return Err("tag-2 header missing".into());
    }
    let mut bits = Bits::new(data, start + 1, end)?;
    let mut table: [u8; 16] = std::array::from_fn(|i| i as u8);
    let mut output: Vec<u8> = Vec::new();
    let mut tokens = Vec::new();
    loop {
        if output.len() as u64 > DECODED_LIMIT {
            return Err("tag-2 stream exceeds the decode limit".into());
        }
        if bits.get(1)? != 0 {
            let width = if bits.get(1)? != 0 {
                2
            } else if bits.get(1)? != 0 {
                3
            } else {
                4
            };
            let mut nibble = |index: u32| -> Result<u8, String> {
                let index = index as usize;
                if index >= 16 {
                    return Err("tag-2 literal index exceeds its table".into());
                }
                let value = table[index];
                table[..=index].rotate_right(1);
                Ok(value)
            };
            let low = nibble(bits.get(width)?)?;
            let high = nibble(bits.get(width)?)?;
            output.push(low | high << 4);
            tokens.push(Mtf4LzToken::Literal { width });
            continue;
        }
        let length = if bits.get(1)? == 0 {
            2
        } else if bits.get(1)? == 0 {
            3
        } else if bits.get(1)? == 0 {
            4
        } else if bits.get(1)? == 0 {
            5
        } else if bits.get(1)? == 0 {
            if bits.get(1)? != 0 {
                7
            } else {
                6
            }
        } else {
            match bits.get(2)? {
                0 => match bits.get(7)? {
                    0 => return Ok((output, tokens)),
                    long => long + 10,
                },
                short => short + 7,
            }
        };
        let distance = if bits.get(1)? != 0 {
            bits.get(5)? + 1
        } else {
            let written = output.len().checked_sub(33);
            let width = match written {
                Some(w) if w < 2048 => usize::BITS - w.leading_zeros(),
                _ => 12,
            };
            bits.get(width)? + 33
        };
        let from = output
            .len()
            .checked_sub(distance as usize)
            .ok_or("tag-2 copy precedes its output")?;
        for index in 0..length as usize {
            output.push(output[from + index]);
        }
        tokens.push(Mtf4LzToken::Copy { length, distance });
    }
}
/// Trace the stream at `start` whose extent is `span` bytes. The plan's
/// encoding must reproduce the ROM bytes, including trailing lookahead.
fn trace_stream(rom: &[u8], start: usize, span: usize) -> Result<Stream, String> {
    let end = start + span;
    let bytes = rom.get(start..end).ok_or("stream lies beyond the ROM")?;
    let (codec, decoded, tokens, body) = match bytes.first() {
        Some(0) => {
            let (decoded, _, tokens) =
                psynergy::assets::lz::decode_general_trace(rom, start, end, DECODED_LIMIT)
                    .map_err(|e| e.to_string())?;
            let body = psynergy::assets::lz::encode_general(&decoded, &tokens)
                .map_err(|e| e.to_string())?;
            (
                "golden-sun-general-lz",
                decoded,
                general_tokens(&tokens),
                body,
            )
        }
        Some(1) => {
            let (decoded, _, groups) =
                psynergy::assets::lz::decode_palette_trace(rom, start + 1, end, DECODED_LIMIT)
                    .map_err(|e| e.to_string())?;
            let mut body = vec![1];
            body.extend(
                psynergy::assets::lz::encode_palette(&decoded, &groups)
                    .map_err(|e| e.to_string())?,
            );
            (
                "golden-sun-tagged-palette-lz",
                decoded,
                palette_groups(&groups),
                body,
            )
        }
        Some(2) => {
            let (decoded, tokens) = trace_mtf4(rom, start, end)?;
            let body = psynergy::assets::lz::encode_mtf4_lz(&decoded, &tokens)
                .map_err(|e| e.to_string())?;
            let rows = tokens
                .iter()
                .map(|token| match token {
                    Mtf4LzToken::Literal { width } => json!(width),
                    Mtf4LzToken::Copy { length, distance } => json!([distance, length]),
                })
                .collect::<Vec<_>>();
            ("golden-sun-kind2-lz", decoded, json!(rows), body)
        }
        _ => return Err("stream has no LZ tag".into()),
    };
    if body.len() > span || bytes[..body.len()] != body[..] {
        return Err(format!("{codec} tokens do not re-encode the stream"));
    }
    let lookahead = &bytes[body.len()..];
    if codec == "golden-sun-kind2-lz" && lookahead.len() > 3 {
        return Err("tag-2 stream is followed by more than three bytes".into());
    }
    let mut plan = json!({"format":1,"codec":codec,"decoded_size":decoded.len(),"encoded_size":span,"tokens":tokens,"lookahead":hex_text(lookahead)});
    if codec == "golden-sun-tagged-palette-lz" {
        plan["tag"] = json!(1);
    }
    Ok(Stream { plan, decoded })
}
/// Export only controls independently reproduced by the compressor.
fn shared_plan(stream: &Stream) -> Result<Value, String> {
    compression_plan::checked_plan(&stream.decoded, &stream.plan)
}

struct SceneRequest {
    index: usize,
    name: Option<String>,
}
fn parse_scenes(text: &str) -> Result<Vec<SceneRequest>, String> {
    text.split(',')
        .map(|item| {
            let (index, name) = item
                .split_once('=')
                .map_or((item, None), |(index, name)| (index, Some(name)));
            let name = name
                .map(|name| {
                    if name.is_empty()
                        || !name
                            .bytes()
                            .all(|b| b.is_ascii_uppercase() || b.is_ascii_digit() || b == b'_')
                    {
                        Err(format!("scene name {name:?} must be upper-case romaji"))
                    } else {
                        Ok(name.to_string())
                    }
                })
                .transpose()?;
            Ok(SceneRequest {
                index: index
                    .trim()
                    .parse()
                    .map_err(|_| format!("invalid scene index {index:?}"))?,
                name,
            })
        })
        .collect()
}

/// An unidentified map component awaiting its place in the private map binary.
struct Layer {
    map: String,
    /// Index of its region in `Output::regions`.
    region: usize,
    address: String,
    bytes: Vec<u8>,
}
#[derive(Default)]
struct Staged {
    layers: Vec<Layer>,
    binaries: BTreeMap<String, Vec<u8>>,
    tiles: BTreeMap<String, Vec<u8>>,
    documents: BTreeMap<String, Value>,
    compression: serde_json::Map<String, Value>,
    banks: Vec<Vec<u16>>,
}
struct Output {
    scenes: Vec<Value>,
    layouts: Vec<Value>,
    regions: Vec<Value>,
    bindings: Vec<Value>,
    private_inputs: Vec<Value>,
    previews: Vec<Preview>,
}
struct Preview {
    name: String,
    container: String,
    map: String,
    grid_offset: usize,
    metatile_offset: usize,
    metatile_length: usize,
    /// The twelve header bytes: sizes, BG priorities and charblock bases.
    parameters: Vec<u8>,
    tiles: Vec<(String, usize)>,
    banks: Vec<usize>,
    /// Header record origins: each layer's (x | y << 8) in half-cell units.
    origins: [usize; 3],
}

struct Deriver<'a> {
    rom: &'a [u8],
    target: DecompTarget,
    paths: NativePaths,
    directory: Directory,
    index: Option<Value>,
    staged: Staged,
    seen: BTreeSet<usize>,
    chr_banks: BTreeMap<String, usize>,
    output: Output,
}
impl Deriver<'_> {
    /// Append unidentified layers after every container's grid and metatiles,
    /// so a map binary's layouts keep their adjacent, derivation-order extents.
    fn place_layers(&mut self) -> Result<(), String> {
        for layer in std::mem::take(&mut self.staged.layers) {
            let binary = self
                .staged
                .binaries
                .get_mut(&layer.map)
                .ok_or_else(|| format!("layer {} has no map binary", layer.address))?;
            let offset = binary.len();
            binary.extend(&layer.bytes);
            let component = self
                .output
                .regions
                .get_mut(layer.region)
                .map(|region| &mut region["components"][0])
                .ok_or_else(|| format!("layer {} lost its region", layer.address))?;
            component["source_offset"] = json!(offset);
            component["source_length"] = json!(layer.bytes.len());
            self.output.private_inputs.push(json!({"kind":"bytes","source":layer.map,"source_offset":offset,"region_address":layer.address,"decoded_sha256":sha256::hex(&layer.bytes)}));
        }
        Ok(())
    }
    fn existing_region(&self, address: usize) -> Option<&Value> {
        let address = hex_address(address + ROM_BASE);
        self.index.as_ref()?["regions"]
            .as_array()?
            .iter()
            .find(|r| r["address"] == address.as_str())
    }
    fn existing_input(&self, address: usize) -> Option<&Value> {
        let address = hex_address(address + ROM_BASE);
        self.index.as_ref()?["private_inputs"]
            .as_array()?
            .iter()
            .find(|r| r["region_address"] == address.as_str())
    }
    /// A shared compression section keeps its committed key; a new one is
    /// named by the digest of the stream it plans.
    fn section(&mut self, offset: usize, span: usize, stream: &Stream) -> Result<String, String> {
        let key = self
            .existing_region(offset)
            .and_then(|r| r["plan_section"].as_str())
            .filter(|key| !key.starts_with('/'))
            .map(str::to_string)
            .unwrap_or_else(|| sha256::hex(&self.rom[offset..offset + span]));
        self.staged
            .compression
            .insert(key.clone(), shared_plan(stream)?);
        Ok(key)
    }
    fn bank_index(&mut self, values: Vec<u16>) -> usize {
        if let Some(index) = self.staged.banks.iter().position(|b| *b == values) {
            return index;
        }
        self.staged.banks.push(values);
        self.staged.banks.len() - 1
    }
    fn scene(&mut self, request: &SceneRequest) -> Result<(), String> {
        let tables = field_tables(&self.target)?;
        let record = tables.scenes - ROM_BASE + request.index * 8;
        let overlay = u16_at(self.rom, record)? as usize;
        let load = u16_at(self.rom, record + 4)? as usize;
        let row = (0..6)
            .map(|k| {
                Ok(
                    u16_at(self.rom, tables.loads - ROM_BASE + load * 12 + k * 2)? as usize
                        + tables.bias,
                )
            })
            .collect::<Result<Vec<_>, String>>()?;
        let container = row[0];
        let name = request
            .name
            .clone()
            .unwrap_or_else(|| format!("MAP_{:03X}", container));
        let mut loader = serde_json::Map::new();
        loader.insert("map_index".into(), json!(load));
        loader.insert("container".into(), json!(resource_name(container)));
        loader.insert("palette".into(), json!(resource_name(row[1])));
        for (field, id) in loader_fields(&self.target).iter().zip(&row[2..]) {
            loader.insert((*field).into(), json!(resource_name(*id)));
        }
        self.output.scenes.push(
            json!({"scene_index":request.index,"overlay":resource_name(overlay),"loader":loader}),
        );
        let owner = format!("{}/FIELD/{name}", self.paths.source);
        let banks = self.palette(row[1])?;
        let mut tiles = Vec::new();
        for (bank, id) in row[2..].iter().enumerate() {
            let purpose = if loader_fields(&self.target)[bank] == "animation_source" {
                "map-animation-source"
            } else {
                "map-charblock-source"
            };
            tiles.push(self.tiles(*id, &owner, &name, purpose)?);
        }
        self.container(container, &owner, &name, tiles, banks)
    }
    fn palette(&mut self, id: usize) -> Result<Vec<usize>, String> {
        let (offset, span) = self.directory.resource(id)?;
        let stream = trace_stream(self.rom, offset, span)?;
        if stream.decoded.is_empty() || stream.decoded.len() % 32 != 0 {
            return Err(format!("palette {id:03x} is not whole sixteen-color banks"));
        }
        let banks = stream
            .decoded
            .chunks_exact(32)
            .map(|bank| {
                let values = bank
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>();
                self.bank_index(values)
            })
            .collect::<Vec<_>>();
        if !self.seen.insert(offset) {
            return Ok(banks);
        }
        let key = self.section(offset, span, &stream)?;
        let colors = self.paths.colors.clone();
        let address = hex_address(offset + ROM_BASE);
        self.output.regions.push(json!({"address":address,"size":small_hex(span),"kind":"golden-sun-general-lz","plan":self.paths.recipes,"plan_section":key,
            "components":banks.iter().map(|bank| json!({"kind":"le-u16-array","source":colors,"pointer":format!("/banks/{bank}"),"size":32})).collect::<Vec<_>>()}));
        self.output.bindings.push(json!({"address":address,"compression":self.paths.recipes,"compression_section":key,"sources":[colors],"palette_source":colors,"palette_banks":banks,"resource":resource_name(id)}));
        self.output.private_inputs.push(json!({"kind":"palette","source":colors,"banks":banks,"region_address":address,"decoded_sha256":sha256::hex(&stream.decoded)}));
        Ok(banks)
    }
    fn tiles(
        &mut self,
        id: usize,
        owner: &str,
        name: &str,
        purpose: &str,
    ) -> Result<(String, usize), String> {
        let (offset, span) = self.directory.resource(id)?;
        let stream = trace_stream(self.rom, offset, span)?;
        if stream.plan["codec"] != "golden-sun-kind2-lz" || stream.decoded.len() != TILE_BANK {
            return Err(format!("tile bank {id:03x} is not one tag-2 charblock"));
        }
        let placed = self.existing_input(offset).and_then(|input| {
            Some((
                input["source"].as_str()?.to_string(),
                input["tile_offset"].as_u64()? as usize,
            ))
        });
        let fresh = !self.seen.contains(&offset);
        let (source, tile_offset) = match placed {
            Some(placed) => placed,
            None if fresh => {
                let source = format!("{owner}/{name}_CHR.PNG");
                let next = self.chr_banks.entry(source.clone()).or_default();
                let tile_offset = *next * TILES_PER_BANK;
                *next += 1;
                (source, tile_offset)
            }
            None => self
                .output
                .private_inputs
                .iter()
                .find(|input| input["region_address"] == hex_address(offset + ROM_BASE).as_str())
                .map(|input| {
                    (
                        input["source"].as_str().unwrap().to_string(),
                        input["tile_offset"].as_u64().unwrap() as usize,
                    )
                })
                .ok_or("shared tile bank lost its placement")?,
        };
        if !self.seen.insert(offset) {
            return Ok((source, tile_offset));
        }
        let mut plan = stream.plan.clone();
        let layout = json!({"format":"sequential-gba-4bpp-tiles","purpose":purpose,"tile_count":TILES_PER_BANK,"tile_width":8,"tile_height":8,"columns":32,"rows":16});
        plan["layout"] = layout.clone();
        let key = self.section(
            offset,
            span,
            &Stream {
                plan,
                decoded: stream.decoded.clone(),
            },
        )?;
        let sheet = self.staged.tiles.entry(source.clone()).or_default();
        let start = tile_offset * 32;
        if sheet.len() < start + TILE_BANK {
            sheet.resize(start + TILE_BANK, 0);
        }
        sheet[start..start + TILE_BANK].copy_from_slice(&stream.decoded);
        let address = hex_address(offset + ROM_BASE);
        self.output.regions.push(json!({"address":address,"size":small_hex(span),"kind":"golden-sun-kind2-lz","plan":self.paths.recipes,"plan_section":key,"layout":layout,
            "components":[{"kind":"gba-4bpp-tiles","source":source,"tile_offset":tile_offset,"tile_count":TILES_PER_BANK,"size":TILE_BANK}]}));
        self.output.bindings.push(json!({"address":address,"compression":self.paths.recipes,"compression_section":key,"sources":[source],"tile_offset":tile_offset,"tile_count":TILES_PER_BANK,"resource":resource_name(id)}));
        self.output.private_inputs.push(json!({"kind":"tiles","source":source,"tile_offset":tile_offset,"region_address":address,"decoded_sha256":sha256::hex(&stream.decoded)}));
        Ok((source, tile_offset))
    }
    fn container(
        &mut self,
        id: usize,
        owner: &str,
        name: &str,
        tiles: Vec<(String, usize)>,
        banks: Vec<usize>,
    ) -> Result<(), String> {
        let (base, size) = self.directory.resource(id)?;
        if !self.seen.insert(base) {
            return Ok(());
        }
        let slots = component_slots(&self.target);
        let header = 0x24 + 4 * slots;
        let offsets = (0..slots)
            .map(|k| Ok(u32_at(self.rom, base + 0x24 + 4 * k)? as usize))
            .collect::<Result<Vec<_>, String>>()?;
        if offsets[0] != header
            || offsets
                .iter()
                .any(|o| *o != 0 && (*o < header || *o >= size))
        {
            return Err(format!(
                "container {id:03x} does not hold {slots} component offsets after a 0x{header:x} header"
            ));
        }
        let mut ordered = offsets
            .iter()
            .copied()
            .filter(|o| *o != 0)
            .collect::<Vec<_>>();
        ordered.sort_unstable();
        let extent = |offset: usize| {
            ordered
                .iter()
                .find(|o| **o > offset)
                .copied()
                .unwrap_or(size)
                - offset
        };
        let key = resource_name(id);
        let source = format!("{owner}/{name}.JSON");
        let map = format!("{owner}/{name}.BIN");
        let at = |offset: usize| hex_address(base + offset + ROM_BASE);
        let rom = self.rom;
        let params = &rom[base..base + 12];
        let records = (0..3)
            .map(|r| {
                (0..4)
                    .map(|w| u16_at(self.rom, base + 12 + r * 8 + w * 2))
                    .collect::<Result<Vec<_>, _>>()
            })
            .collect::<Result<Vec<_>, _>>()?;
        let mut document = serde_json::Map::new();
        document.insert("format".into(), json!(1));
        document.insert("kind".into(), json!("golden-sun-map-container"));
        document.insert("header".into(), json!({"format":1,"kind":"typed-table","address":at(0),"size":small_hex(header),"segments":[
            {"name":"parameters","address":at(0),"end":at(12),"stride":12,"element":"u8","values":params},
            {"name":"records","address":at(12),"end":at(0x24),"stride":8,"element":"le-u16","values":records},
            {"name":"component_offsets","address":at(0x24),"end":at(header),"stride":4,"element":"le-u32","values":offsets.iter().map(|o| small_hex(*o)).collect::<Vec<_>>()}]}));
        let mut regions = vec![
            json!({"address":at(0),"size":small_hex(header),"kind":"typed-table","source":source,"pointer":format!("/maps/{key}/header")}),
        ];
        let binary = self.staged.binaries.entry(map.clone()).or_default();
        let grid_offset = binary.len();
        let (mut metatile_offset, mut metatile_length) = (0, 0);
        for (slot, offset) in offsets.iter().copied().enumerate() {
            if offset == 0 {
                continue;
            }
            let span = extent(offset);
            let section = map_component_section(slots, slot);
            let pointer = format!("/maps/{key}/{section}");
            let traced = trace_stream(self.rom, base + offset, span);
            let stream = match (slot, traced) {
                (0..=2, Err(error)) => {
                    return Err(format!("container {id:03x} slot {slot}: {error}"))
                }
                (_, Err(_)) => {
                    // Not a stream: keep the bytes as a typed segment until identified.
                    let values = &rom[base + offset..base + offset + span];
                    document.insert(section.into(), json!({"format":1,"kind":"typed-table","address":at(offset),"size":small_hex(span),
                        "segments":[{"name":"values","address":at(offset),"end":at(offset + span),"stride":span,"element":"u8","values":values}]}));
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"typed-table","source":source,"pointer":pointer}));
                    continue;
                }
                (_, Ok(stream)) => stream,
            };
            let decoded = &stream.decoded;
            let mut plan = stream.plan.clone();
            match slot {
                0 => {
                    let mode = *decoded.first().ok_or("empty metatile stream")?;
                    let words = native::decode_metatile_words(decoded, mode)?;
                    if words.is_empty() || words.len() % 8 != 0 {
                        return Err(format!(
                            "container {id:03x} metatiles are not whole 2x2 definitions"
                        ));
                    }
                    let binary = self.staged.binaries.get_mut(&map).unwrap();
                    binary.resize(grid_offset + 65536, 0);
                    metatile_offset = binary.len();
                    metatile_length = words.len();
                    binary.extend(&words);
                    plan["component"] = json!("map-metatiles-2x2");
                    plan["transform_mode"] = json!(mode);
                    plan["metatiles"] = json!(words.len() / 8);
                    plan["tilemap_source"] = json!(map);
                    plan["tilemap_offset"] = json!(metatile_offset);
                    plan["tilemap_length"] = json!(metatile_length);
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,
                        "components":[{"kind":"gba-tilemap16","size":decoded.len(),"delta_mode":mode,"source":map,"format":"binary","source_offset":metatile_offset,"source_length":metatile_length}]}));
                    self.output.private_inputs.push(json!({"kind":"metatiles","source":map,"source_offset":metatile_offset,"region_address":at(offset),"transform_mode":mode,"decoded_sha256":sha256::hex(&words)}));
                }
                2 => {
                    if decoded.len() != 65536 {
                        return Err(format!("container {id:03x} grid is not 128x128 cells"));
                    }
                    let section_key = self.section(base + offset, span, &stream)?;
                    let binary = self.staged.binaries.get_mut(&map).unwrap();
                    binary.resize(binary.len().max(grid_offset + 65536), 0);
                    binary[grid_offset..grid_offset + 65536].copy_from_slice(decoded);
                    document.insert("grid_source".into(), json!(map));
                    document.insert("grid_offset".into(), json!(grid_offset));
                    document.insert(
                        "grid_compression".into(),
                        json!({"source":self.paths.recipes,"section":section_key}),
                    );
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":self.paths.recipes,"plan_section":section_key,
                        "components":[{"kind":"golden-sun-map-grid","source":map,"width":128,"height":128,"source_offset":grid_offset,"source_length":65536,"size":65536}]}));
                    self.output.private_inputs.push(json!({"kind":"grid","source":map,"source_offset":grid_offset,"region_address":at(offset),"decoded_sha256":sha256::hex(decoded)}));
                    continue;
                }
                1 => {
                    let size = if decoded.len() % 4 == 0 { 4 } else { 1 };
                    plan["component"] = json!("map-descriptors-4byte");
                    plan["record_size"] = json!(size);
                    plan["records"] = json!(decoded.chunks(size).collect::<Vec<_>>());
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,
                        "components":[{"kind":"u8-array","pointer":format!("{pointer}/records"),"source":source,"size":decoded.len()}]}));
                }
                _ if slots == 6 && decoded.len() % 2 == 0 => {
                    plan["word_size"] = json!(2);
                    plan["words"] = json!(decoded
                        .chunks_exact(2)
                        .map(|w| u16::from_le_bytes([w[0], w[1]]))
                        .collect::<Vec<_>>());
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,
                        "components":[{"kind":"le-u16-array","pointer":format!("{pointer}/words"),"source":source,"size":decoded.len()}]}));
                }
                _ => {
                    // Bytes of unknown structure are not source: the layer is a
                    // private input placed in the map binary once every
                    // container's grid and metatiles have their offsets.
                    self.staged.layers.push(Layer {
                        map: map.clone(),
                        region: self.output.regions.len() + regions.len(),
                        address: at(offset),
                        bytes: decoded.clone(),
                    });
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,
                        "components":[{"kind":"u8-array","format":"binary","source":map,"size":decoded.len()}]}));
                }
            }
            document.insert(section.into(), plan);
        }
        if metatile_length == 0 || !document.contains_key("grid_source") {
            return Err(format!("container {id:03x} lacks metatiles or a grid"));
        }
        let (width, height) = (params[2] as usize * 8, params[3] as usize * 8);
        let bytes = &self.staged.binaries[&map];
        let payload = sha256::hex(&bytes[grid_offset..metatile_offset + metatile_length]);
        self.output.layouts.push(json!({"container":key,"name":name,"owner":owner,"source":source,"source_pointer":format!("/maps/{key}"),
            "map":map,"grid_offset":grid_offset,"grid_length":65536,"metatiles":map,"metatile_offset":metatile_offset,"metatile_length":metatile_length,
            "width":width,"height":height,"payload_sha256":payload}));
        self.staged
            .documents
            .entry(source)
            .or_insert_with(|| json!({"format":1,"maps":{}}))["maps"][&key] =
            Value::Object(document);
        self.output.regions.extend(regions);
        self.output.previews.push(Preview {
            name: name.into(),
            container: key,
            map,
            grid_offset,
            metatile_offset,
            metatile_length,
            parameters: params.to_vec(),
            tiles,
            banks,
            origins: [0, 1, 2].map(|layer| records[layer][0] as usize),
        });
        Ok(())
    }
}

/// Write the staged native sources under `stage` and rebuild every region.
fn verify(deriver: &Deriver, stage: &Path) -> Result<usize, String> {
    let staged = &deriver.staged;
    for (name, bytes) in &staged.binaries {
        native::write_source(stage, name, bytes)?;
    }
    let gray: Vec<u8> = (0u16..16)
        .flat_map(|i| (i | i << 5 | i << 10).to_le_bytes())
        .collect();
    for (name, tiles) in &staged.tiles {
        let png = psynergy::assets::image::png_from_gba_tiles(tiles, &gray, GbaBpp::Bpp4, 32)
            .map_err(|e| e.to_string())?;
        native::write_source(stage, name, &png)?;
    }
    for (name, document) in &staged.documents {
        native::write_source(
            stage,
            name,
            format!("{}\n", canonical_json(document)).as_bytes(),
        )?;
    }
    native::write_source(
        stage,
        &deriver.paths.colors,
        format!("{}\n", canonical_json(&json!({"format":"bgr555-banks","colors_per_bank":16,"banks":staged.banks,"tables":{}}))).as_bytes(),
    )?;
    native::write_source(
        stage,
        &deriver.paths.recipes,
        format!(
            "{}\n",
            canonical_json(&Value::Object(staged.compression.clone()))
        )
        .as_bytes(),
    )?;
    native::validate(
        &json!({"format":"camelot-style-golden-sun-native","layouts":deriver.output.layouts,"regions":deriver.output.regions}),
    )?;
    let mut ctx = Context::new(stage);
    let mut matched = 0;
    for region in &deriver.output.regions {
        let address = number(&region["address"], "region address")? - ROM_BASE;
        let size = number(&region["size"], "region size")?;
        let (built, _, _) = build_entry(&mut ctx, region)
            .map_err(|e| format!("region {}: {e}", region["address"]))?;
        if deriver.rom.get(address..address + size) != Some(built.as_slice()) {
            return Err(format!(
                "region {} does not rebuild byte-exactly",
                region["address"]
            ));
        }
        matched += 1;
    }
    Ok(matched)
}

fn rgba_color(color: u16) -> [u8; 4] {
    let channel = |shift: u16| ((color >> shift & 31) as u8) << 3;
    [channel(0), channel(5), channel(10), 255]
}

/// One field map as the loader at 0x0802a6b8 hands it to the three text BGs.
///
/// Header bytes 4..7 are the BG3, BG2 and BG1 priorities and bytes 7..10 their
/// charblock bases; record `i` places the layer shown on BG `3 - i`. The first
/// three tile resources decompress into charblocks 1..=3. The grid's second
/// half holds one u16 per cell of a 128x128 plane; a cell whose low twelve bits
/// are 0xfff takes the next unseen metatile, in plane order.
pub(in crate::build_assets) struct FieldMap<'a> {
    pub parameters: &'a [u8],
    pub origins: [usize; 3],
    pub grid: &'a [u8],
    pub metatiles: &'a [u8],
    /// Charblocks 1..=3, one 4-bit pixel index per byte, 512 tiles of 64.
    pub charblocks: [&'a [u8]; 3],
    pub palettes: usize,
}
pub(in crate::build_assets) struct FieldLayer {
    pub bg: usize,
    pub priority: u8,
    pub charblock: u8,
    /// `palette * 16 + color`; zero is transparent.
    pub pixels: Vec<u8>,
    pub opaque: bool,
}
pub(in crate::build_assets) struct FieldRender {
    pub width: usize,
    pub height: usize,
    pub layers: Vec<FieldLayer>,
    /// Screen entries naming a charblock or palette the loader did not fill.
    pub unresolved: usize,
}
impl FieldRender {
    /// Back-to-front order: lower priority first, then higher BG number. A layer
    /// opaque over the whole map is a backdrop the scene script selects by
    /// scroll, window or blend, so only the rearmost such layer is composited.
    pub fn order(&self) -> Vec<usize> {
        let mut order = (0..self.layers.len()).collect::<Vec<_>>();
        order.sort_by_key(|&i| {
            (
                std::cmp::Reverse(self.layers[i].priority),
                std::cmp::Reverse(self.layers[i].bg),
            )
        });
        let backdrop = order.iter().copied().find(|&i| self.layers[i].opaque);
        order.retain(|&i| !self.layers[i].opaque || Some(i) == backdrop);
        order
    }
    pub fn composite(&self) -> Vec<u8> {
        let mut pixels = vec![0u8; self.width * self.height];
        for layer in self.order() {
            for (out, &value) in pixels.iter_mut().zip(&self.layers[layer].pixels) {
                if value != 0 {
                    *out = value;
                }
            }
        }
        pixels
    }
}
pub(in crate::build_assets) fn render_field(map: &FieldMap) -> Result<FieldRender, String> {
    if map.parameters.len() != 12 || map.grid.len() != 65536 || map.metatiles.len() % 8 != 0 {
        return Err("field map header, grid or metatiles are incomplete".into());
    }
    if map
        .charblocks
        .iter()
        .any(|bank| bank.len() != TILES_PER_BANK * 64)
    {
        return Err("field charblocks must hold 512 tiles".into());
    }
    let words = map
        .metatiles
        .chunks_exact(2)
        .map(|w| u16::from_le_bytes([w[0], w[1]]) as usize)
        .collect::<Vec<_>>();
    let count = words.len() / 4;
    let mut next = 0;
    let cells = map.grid[32768..]
        .chunks_exact(2)
        .map(|w| {
            let word = u16::from_le_bytes([w[0], w[1]]) as usize & 0xfff;
            if word == 0xfff {
                next += 1;
                next - 1
            } else {
                word
            }
        })
        .collect::<Vec<_>>();
    if next != count {
        return Err(format!(
            "field grid introduces {next} metatiles but the container defines {count}"
        ));
    }
    let (columns, rows) = (
        map.parameters[2] as usize / 2,
        map.parameters[3] as usize / 2,
    );
    let (width, height) = (columns * 16, rows * 16);
    let mut unresolved = 0;
    let mut layers = Vec::new();
    for (index, origin) in map.origins.iter().enumerate() {
        let charblock = map.parameters[7 + index];
        let mut pixels = vec![0u8; width * height];
        let (ox, oy) = ((origin & 0xff) / 2, (origin >> 8) / 2);
        for cy in 0..rows.min(128usize.saturating_sub(oy)) {
            for cx in 0..columns.min(128usize.saturating_sub(ox)) {
                let cell = cells[(oy + cy) * 128 + ox + cx];
                if cell >= count {
                    return Err(format!("field cell names metatile {cell} of {count}"));
                }
                for quadrant in 0..4 {
                    let entry = words[cell * 4 + quadrant];
                    let (tile, palette) = (entry & 0x3ff, entry >> 12);
                    let block = charblock as usize + (tile >> 9);
                    if !(1..=3).contains(&block) || palette >= map.palettes {
                        unresolved += 1;
                        continue;
                    }
                    let bank = map.charblocks[block - 1];
                    let base = (tile & 511) * 64;
                    for py in 0..8 {
                        for px in 0..8 {
                            let sx = if entry & 0x400 != 0 { 7 - px } else { px };
                            let sy = if entry & 0x800 != 0 { 7 - py } else { py };
                            let value = bank[base + sy * 8 + sx];
                            if value == 0 {
                                continue;
                            }
                            let at = (cy * 16 + (quadrant >> 1) * 8 + py) * width
                                + cx * 16
                                + (quadrant & 1) * 8
                                + px;
                            pixels[at] = (palette * 16) as u8 | value;
                        }
                    }
                }
            }
        }
        let opaque = pixels.iter().all(|&p| p != 0);
        layers.push(FieldLayer {
            bg: 3 - index,
            priority: map.parameters[4 + index],
            charblock,
            pixels,
            opaque,
        });
    }
    Ok(FieldRender {
        width,
        height,
        layers,
        unresolved,
    })
}

/// Live transport: u32 JSON-header length, header, then indexed layer planes.
/// Never persisted or published; the same decoder also feeds image exports.
pub(crate) fn live_scene(root: &Path, target: &str, scene: usize) -> Result<Vec<u8>, String> {
    let target = decomp_target(Some(target))?;
    let paths = NativePaths::of(&target);
    let index = json(&root.join(&paths.index))?;
    let scenes = json(
        &root
            .join(target.source_dir)
            .join("FIELD/COMMON/SCENE_TABLE.JSON"),
    )?;
    if !scenes["segments"][0]["records"]
        .as_array()
        .is_some_and(|rows| scene < rows.len())
    {
        return Err("Scene is not in the maintained scene index".into());
    }
    let rom = fs::read(root.join(target.rom)).map_err(|e| e.to_string())?;
    if index["reference_sha256"] != sha256::hex(&rom) {
        return Err("ROM differs from SOURCE.JSON checksum".into());
    }
    let mut deriver = Deriver {
        rom: &rom,
        directory: Directory::read(&rom)?,
        target,
        paths,
        index: None,
        staged: Staged::default(),
        seen: BTreeSet::new(),
        chr_banks: BTreeMap::new(),
        output: Output {
            scenes: vec![],
            layouts: vec![],
            regions: vec![],
            bindings: vec![],
            private_inputs: vec![],
            previews: vec![],
        },
    };
    deriver.scene(&SceneRequest {
        index: scene,
        name: None,
    })?;
    let preview = deriver
        .output
        .previews
        .first()
        .ok_or("Scene has no supported field map")?;
    let (field, colors) = decoded_field(&deriver, preview)?;
    let blend = network::blend(
        &deriver,
        usize::from_str_radix(&preview.container, 16).map_err(|e| e.to_string())?,
    );
    let header = json!({"format":1,"scene":scene,"container":preview.container,"width":field.width,"height":field.height,"unresolved":field.unresolved,"palettes":colors,"order":field.order(),"blend":blend.map(|b|json!({"control":b.control,"alpha":b.alpha})),"layers":field.layers.iter().map(|l|json!({"bg":l.bg,"priority":l.priority,"charblock":l.charblock,"opaque":l.opaque})).collect::<Vec<_>>()});
    let json = serde_json::to_vec(&header).map_err(|e| e.to_string())?;
    let mut bytes = (json.len() as u32).to_le_bytes().to_vec();
    bytes.extend(json);
    for layer in field.layers {
        bytes.extend(layer.pixels)
    }
    Ok(bytes)
}

/// Review image: the composited layers over the loaded palette's backdrop.
fn decoded_field(
    deriver: &Deriver,
    preview: &Preview,
) -> Result<(FieldRender, Vec<Vec<u16>>), String> {
    let staged = &deriver.staged;
    let binary = &staged.binaries[&preview.map];
    let unpacked = preview.tiles[..3]
        .iter()
        .map(|(source, offset)| {
            staged.tiles[source][offset * 32..offset * 32 + TILE_BANK]
                .iter()
                .flat_map(|byte| [byte & 15, byte >> 4])
                .collect::<Vec<u8>>()
        })
        .collect::<Vec<_>>();
    let field = render_field(&FieldMap {
        parameters: &preview.parameters,
        origins: preview.origins,
        grid: &binary[preview.grid_offset..preview.grid_offset + 65536],
        metatiles: &binary
            [preview.metatile_offset..preview.metatile_offset + preview.metatile_length],
        charblocks: [&unpacked[0], &unpacked[1], &unpacked[2]],
        palettes: preview.banks.len(),
    })?;
    let colors = preview
        .banks
        .iter()
        .map(|bank| staged.banks[*bank].clone())
        .collect::<Vec<_>>();
    Ok((field, colors))
}

fn render(deriver: &Deriver, preview: &Preview) -> Result<Vec<u8>, String> {
    let (field, colors) = decoded_field(deriver, preview)?;
    let mut rgba = Vec::with_capacity(field.width * field.height * 4);
    for value in field.composite() {
        let color = colors
            .get(usize::from(value >> 4))
            .and_then(|bank| bank.get(usize::from(value & 15)))
            .copied()
            .unwrap_or(0);
        rgba.extend(rgba_color(color));
    }
    let mut bytes = Vec::new();
    {
        let mut encoder = png::Encoder::new(&mut bytes, field.width as u32, field.height as u32);
        encoder.set_color(png::ColorType::Rgba);
        encoder.set_depth(png::BitDepth::Eight);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(&rgba)
            .map_err(|e| e.to_string())?;
    }
    Ok(bytes)
}

pub(super) fn run(root: &Path, arguments: &[String]) -> Result<(), String> {
    let mut rom_path = None;
    let (mut target, mut scenes, mut output, mut stage, mut preview) =
        (None, None, None, None, None);
    let mut index = 0;
    while index < arguments.len() {
        let argument = arguments[index].as_str();
        let slot = match argument {
            "--target" => &mut target,
            "--scenes" => &mut scenes,
            "-o" | "--output" => &mut output,
            "--stage" => &mut stage,
            "--preview" => &mut preview,
            _ if !argument.starts_with('-') && rom_path.is_none() => {
                rom_path = Some(argument.to_string());
                index += 1;
                continue;
            }
            _ => return Err(USAGE.into()),
        };
        index += 1;
        let value = arguments.get(index).ok_or(USAGE)?.clone();
        if slot.replace(value).is_some() {
            return Err(format!("{argument} was given more than once"));
        }
        index += 1;
    }
    let (Some(rom_path), Some(target), Some(scenes)) = (rom_path, target, scenes) else {
        return Err(USAGE.into());
    };
    // Refuse an output outside the ignored out/ directory before deriving.
    let output = output
        .map(|path| ignored_output_path(root, Path::new(&path), "derived index rows"))
        .transpose()?;
    let stage = stage
        .map(|dir| ignored_output_path(root, Path::new(&dir), "derived stage"))
        .transpose()?;
    let preview = preview
        .map(|dir| ignored_output_path(root, Path::new(&dir), "map previews"))
        .transpose()?;
    let target = decomp_target(Some(&target))?;
    let rom = fs::read(&rom_path).map_err(|e| format!("{rom_path}: {e}"))?;
    let paths = NativePaths::of(&target);
    let indexed = field_tables(&target)?.indexed;
    let index = root
        .join(&paths.index)
        .is_file()
        .then(|| json(&root.join(&paths.index)))
        .transpose()?
        .filter(|_| indexed);
    let reference = sha256::hex(&rom);
    if let Some(index) = &index {
        if index["reference_sha256"] != reference.as_str() {
            return Err(format!(
                "{rom_path} differs from {}'s reference ROM",
                paths.index
            ));
        }
    }
    let mut deriver = Deriver {
        rom: &rom,
        directory: Directory::read(&rom)?,
        target,
        paths,
        index,
        staged: Staged::default(),
        seen: BTreeSet::new(),
        chr_banks: BTreeMap::new(),
        output: Output {
            scenes: vec![],
            layouts: vec![],
            regions: vec![],
            bindings: vec![],
            private_inputs: vec![],
            previews: vec![],
        },
    };
    // Existing palette banks keep their indices so derived rows merge in place.
    if let Some(colors) = root
        .join(&deriver.paths.colors)
        .is_file()
        .then(|| json(&root.join(&deriver.paths.colors)))
        .transpose()?
    {
        for bank in colors["banks"].as_array().ok_or("palette banks absent")? {
            deriver.staged.banks.push(
                bank.as_array()
                    .ok_or("palette bank malformed")?
                    .iter()
                    .map(|v| number(v, "color").map(|c| c as u16))
                    .collect::<Result<_, _>>()?,
            );
        }
    }
    for request in parse_scenes(&scenes)? {
        deriver
            .scene(&request)
            .map_err(|e| format!("scene {}: {e}", request.index))?;
    }
    deriver.place_layers()?;
    let temporary = tempfile::tempdir().map_err(|e| e.to_string())?;
    let stage_root = stage.unwrap_or_else(|| temporary.path().to_path_buf());
    let matched = verify(&deriver, &stage_root)?;
    if let Some(directory) = preview {
        fs::create_dir_all(&directory).map_err(|e| e.to_string())?;
        for item in &deriver.output.previews {
            let path = directory.join(format!(
                "{}_{}.PNG",
                item.name,
                item.container.to_ascii_uppercase()
            ));
            fs::write(&path, render(&deriver, item)?).map_err(|e| e.to_string())?;
            eprintln!("preview={}", path.display());
        }
    }
    let result = json!({
        "format":"alchemy-native-index-rows-v1",
        "target":deriver.target.id.as_str(),
        "index":deriver.paths.index,
        "reference_sha256":reference,
        "tables":field_tables(&deriver.target).map(|t| json!({"scenes":hex_address(t.scenes),"loads":hex_address(t.loads),"resource_bias":small_hex(t.bias),"indexed_edition":t.indexed}))?,
        "scenes":deriver.output.scenes,
        "layouts":deriver.output.layouts,
        "bindings":deriver.output.bindings,
        "regions":deriver.output.regions,
        "private_inputs":deriver.output.private_inputs,
        "documents":deriver.staged.documents,
        "compression":deriver.staged.compression,
        "verification":{"regions":deriver.output.regions.len(),"byte_exact":matched},
    });
    let text = format!("{}\n", canonical_json(&result));
    match output {
        Some(path) => {
            if let Some(parent) = path.parent() {
                fs::create_dir_all(parent).map_err(|e| e.to_string())?;
            }
            fs::write(&path, text).map_err(|e| e.to_string())?;
            eprintln!(
                "derived scenes={} regions={} byte_exact={matched} output={}",
                deriver.output.scenes.len(),
                deriver.output.regions.len(),
                path.display()
            );
        }
        None => print!("{text}"),
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn tagged_general_stream_accepts_zero_dictionary_and_excludes_read_ahead() {
        let decoded = [0, 0, 1];
        let tokens = [
            GeneralToken::Copy {
                length: 2,
                distance: 1,
            },
            GeneralToken::Literal(1),
        ];
        let mut stored =
            psynergy::assets::lz::encode_general_prefill(&decoded, &tokens, 0x1000, 1).unwrap();
        let encoded = stored.len();
        stored.push(0x39);
        assert_eq!(
            tagged_extent(&stored, 0, stored.len()).unwrap(),
            (decoded.to_vec(), encoded)
        );
    }
    #[test]
    fn previews_stages_and_rows_are_refused_outside_out_before_reading_the_rom() {
        let directory = tempfile::tempdir().unwrap();
        let root = directory.path();
        let arguments = |option: &str, path: &str| {
            [
                "missing.gba",
                "--target",
                "tla-en",
                "--scenes",
                "4",
                option,
                path,
            ]
            .map(String::from)
        };
        for option in ["--preview", "--stage", "-o"] {
            for path in [
                "games/THE LOST AGE/PREVIEW",
                "games/THE LOST AGE/SRC/FIELD",
                "tools/alchemy/GRAPHICS",
                "out/../games/THE LOST AGE",
            ] {
                let error = run(root, &arguments(option, path)).unwrap_err();
                assert!(
                    error.contains("output") || error.contains("out/"),
                    "{option} {path}: {error}"
                );
            }
        }
        let error = run(root, &arguments("--preview", "out/tla-en/preview")).unwrap_err();
        assert!(error.contains("missing.gba"), "{error}");
    }

    #[test]
    fn scene_requests_accept_optional_romaji_names() {
        let rows = parse_scenes("4,187=VINASU_CHOJO").unwrap();
        assert_eq!((rows[0].index, rows[0].name.as_deref()), (4, None));
        assert_eq!(
            (rows[1].index, rows[1].name.as_deref()),
            (187, Some("VINASU_CHOJO"))
        );
        for invalid in ["", "x", "4=vinasu", "4="] {
            assert!(parse_scenes(invalid).is_err(), "{invalid}");
        }
    }
    #[test]
    fn traced_tag2_tokens_reencode_their_stream() {
        let decoded = (0..400u32).map(|i| (i * 7 % 23) as u8).collect::<Vec<_>>();
        let plan = json!({"codec":"golden-sun-kind2-lz","tokens":{"predictor":"greedy-lz-v1","exceptions":[]}});
        let tokens = compression_plan::materialize(&decoded, &plan, &[]).unwrap();
        let tokens = tokens
            .as_array()
            .unwrap()
            .iter()
            .map(|t| match t.as_array() {
                Some(pair) => Mtf4LzToken::Copy {
                    distance: pair[0].as_u64().unwrap() as u32,
                    length: pair[1].as_u64().unwrap() as u32,
                },
                None => Mtf4LzToken::Literal {
                    width: t.as_u64().unwrap() as u32,
                },
            })
            .collect::<Vec<_>>();
        let mut stream = psynergy::assets::lz::encode_mtf4_lz(&decoded, &tokens).unwrap();
        for padding in 0..2 {
            let mut data = vec![0xaa; padding];
            data.extend(&stream);
            data.extend([0, 0]);
            let traced = trace_stream(&data, padding, data.len() - padding).unwrap();
            assert_eq!(traced.decoded, decoded);
            assert_eq!(
                traced.plan["tokens"].as_array().unwrap().len(),
                tokens.len()
            );
        }
        stream[1] ^= 0xff;
        assert!(trace_stream(&stream, 0, stream.len())
            .map(|s| s.decoded != decoded)
            .unwrap_or(true));
    }
    #[test]
    fn field_layers_follow_loader_charblocks_priorities_and_sequential_cells() {
        // Two cells wide, one tall: 32x16 pixels per layer.
        let mut parameters = [0u8, 0, 4, 2, 3, 3, 1, 0, 1, 2, 0, 0];
        let origins = [0, 4, 4 << 8];
        let mut grid = vec![0u8; 65536];
        let mut cell = |x: usize, y: usize, word: u16| {
            let at = 32768 + (y * 128 + x) * 2;
            grid[at..at + 2].copy_from_slice(&word.to_le_bytes());
        };
        // BG3 introduces metatiles 0 and 1; BG2 reuses 1 and BG1 reuses 0.
        cell(0, 0, 0x0fff);
        cell(1, 0, 0x8fff);
        cell(2, 0, 1);
        cell(3, 0, 1);
        cell(0, 2, 0);
        cell(1, 2, 0);
        let entry = |tile: u16, palette: u16| (palette << 12 | tile).to_le_bytes();
        let metatiles = [entry(512, 1), entry(512, 1), entry(512, 1), entry(512, 1)]
            .into_iter()
            .chain([entry(512, 2), entry(512, 2), entry(512, 2), entry(512, 2)])
            .flatten()
            .collect::<Vec<u8>>();
        let blocks = [1u8, 2, 3].map(|value| vec![value; 512 * 64]);
        let map = |parameters: &[u8]| {
            render_field(&FieldMap {
                parameters,
                origins,
                grid: &grid,
                metatiles: &metatiles,
                charblocks: [&blocks[0], &blocks[1], &blocks[2]],
                palettes: 14,
            })
            .unwrap()
        };
        let field = map(&parameters);
        assert_eq!((field.width, field.height, field.unresolved), (32, 16, 0));
        // Tile 512 lies one charblock past each layer's base.
        assert_eq!(field.layers[0].pixels[0], 0x11);
        assert_eq!(field.layers[0].pixels[16], 0x21);
        assert_eq!(field.layers[1].pixels[0], 0x22);
        assert_eq!(field.layers[2].pixels[0], 0x13);
        assert!(field.layers.iter().all(|layer| layer.opaque));
        // Every layer covers the map, so only the rearmost is composited.
        assert_eq!(field.order(), vec![0]);
        parameters[7] = 3;
        assert_eq!(map(&parameters).unresolved, 8);
        assert!(render_field(&FieldMap {
            parameters: &parameters,
            origins,
            grid: &grid,
            metatiles: &metatiles[..8],
            charblocks: [&blocks[0], &blocks[1], &blocks[2]],
            palettes: 14,
        })
        .is_err());
    }
    #[test]
    fn directory_extents_run_to_the_next_greater_address() {
        let mut rom = vec![0u8; 0x40];
        for (slot, value) in [
            (0usize, 0x0800_0000u32),
            (1, 0x0800_0000),
            (2, 0x0800_0030),
            (3, 0x0800_0020),
            (4, 0x0800_0030),
            (5, 0),
        ] {
            rom[slot * 4..slot * 4 + 4].copy_from_slice(&value.to_le_bytes());
        }
        let directory = Directory::read(&rom).unwrap();
        assert_eq!(directory.resource(3).unwrap(), (0x20, 0x10));
        assert_eq!(directory.resource(4).unwrap(), (0x30, 0x10));
        assert!(directory.resource(5).is_err());
    }
}
