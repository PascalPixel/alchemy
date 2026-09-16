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
/// Plans in the shared compression document use the predictor spelling.
fn shared_plan(stream: &Stream) -> Result<Value, String> {
    compression_plan::compact_plan(&stream.decoded, &stream.plan)
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

/// Review image: the composited layers over the loaded palette's backdrop.
fn render(deriver: &Deriver, preview: &Preview) -> Result<Vec<u8>, String> {
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
