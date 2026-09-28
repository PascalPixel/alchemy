//! Derive native source index rows for field scenes from a ROM.
//!
//! A scene record names its code overlay and a map load record; the load record
//! names one map container, its palette stream and its tile banks. This reads
//! those tables, traces every stream into a token plan, stages the native
//! sources the plans encode, rebuilds each region through `build_entry`, and
//! prints the rows only when every rebuilt region equals the ROM. It never
//! writes a game's private-inputs.json; the registrar merges the printed rows.
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
    /// Whether the game's committed private-inputs.json indexes this edition's ROM.
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
/// Trace the stream at `start` whose extent is `span` bytes, at most three
/// of them alignment. The plan's encoding must reproduce the stream bytes.
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
    // The bytes after the stream are its file's alignment, which the packer
    // replay derives; the plan records only the extent they complete.
    if span - body.len() > 3 {
        return Err(format!(
            "{codec} stream is followed by more than three alignment bytes"
        ));
    }
    // The Broken Seal's plans name the tag right after the codec.
    let mut plan = json!({"format":1,"codec":codec});
    if codec == "golden-sun-tagged-palette-lz" {
        plan["tag"] = json!(1);
    }
    plan["decoded_size"] = json!(decoded.len());
    plan["encoded_size"] = json!(span);
    plan["tokens"] = tokens;
    Ok(Stream { plan, decoded })
}
/// A stream's plan names only its codec and extents. The build compresses the
/// source input with the target's compressor; the traced tokens only proved
/// the stored extent and are never exported.
fn stream_plan(stream: &Stream) -> Value {
    let mut plan = stream.plan.clone();
    if let Some(plan) = plan.as_object_mut() {
        plan.remove("tokens");
    }
    plan
}
fn little_words(decoded: &[u8]) -> Option<Vec<u16>> {
    (decoded.len() % 2 == 0).then(|| {
        decoded
            .chunks_exact(2)
            .map(|w| u16::from_le_bytes([w[0], w[1]]))
            .collect()
    })
}
fn hex_word(word: u16) -> Value {
    json!(format!("0x{word:04x}"))
}
/// Animation queue words as the queue reader (The Lost Age 0x0802cc9c) walks
/// them: each 0xfdNN header opens queue N, pairs of words follow until a
/// 0xfeNN control word, and 0xffff ends the list. The reader only records
/// headers, so a body without one is kept as the pairs it stores.
fn queue_words(words: &[u16]) -> Option<Value> {
    let (&last, body) = words.split_last()?;
    if last != 0xffff {
        return None;
    }
    let mut groups = Vec::new();
    let mut at = 0;
    while at < body.len() {
        let mut group = Vec::new();
        if body[at] >> 8 == 0xfd {
            group.push(hex_word(body[at]));
            at += 1;
        }
        loop {
            let word = *body.get(at)?;
            if word >> 8 == 0xfe {
                group.push(hex_word(word));
                at += 1;
                break;
            }
            group.push(json!([word, *body.get(at + 1)?]));
            at += 2;
        }
        groups.push(Value::Array(group));
    }
    groups.push(hex_word(last));
    Some(Value::Array(groups))
}
/// Blend animation commands: a 0x3xxx blend control, 0xfeNN control and
/// 0xffff reset stand alone; any other word is a blend value and its duration.
fn blend_words(words: &[u16]) -> Option<Value> {
    let mut commands = Vec::new();
    let mut at = 0;
    while at < words.len() {
        let word = words[at];
        if word == 0xffff || word >> 8 == 0xfe || word & 0xf000 == 0x3000 {
            commands.push(json!([hex_word(word)]));
            at += 1;
        } else {
            commands.push(json!([hex_word(word), *words.get(at + 1)?]));
            at += 2;
        }
    }
    Some(Value::Array(commands))
}
/// The Lost Age's last container slot as the typed lists its readers walk.
/// Three u16 offsets and an unread zero word precede four lists, each ending
/// at the next offset: the points 0x080ca9cc finds by number (a signed
/// number, u16 x and y in pixels, ended by 0xff); the entrances 0x080cc7c4
/// finds by number (a signed number, signed x and y in pixels, and a byte
/// whose low four bits are the arrival facing), whose 0xff number ends the
/// reader's search but not the stored list; the flag-gated cell patches of
/// 0x0802b63c (eight-byte records ended by 0xffff); and the camera bounds
/// (four signed 16-pixel cell coordinates, ended by 0xff).
fn positions_table(decoded: &[u8]) -> Option<Value> {
    let word = |at: usize| {
        Some(u16::from_le_bytes([
            *decoded.get(at)?,
            *decoded.get(at + 1)?,
        ]))
    };
    let offsets = [word(0)?, word(2)?, word(4)?].map(usize::from);
    if word(6)? != 0 {
        return None;
    }
    let bounds = [8, offsets[0], offsets[1], offsets[2], decoded.len()];
    let signed = |byte: u8| i64::from(byte as i8);
    let s16 = |at: usize| i64::from(i16::from_le_bytes([decoded[at], decoded[at + 1]]));
    let u16_at = |at: usize| i64::from(u16::from_le_bytes([decoded[at], decoded[at + 1]]));
    struct List {
        name: &'static str,
        stride: usize,
        terminator: &'static [u8],
        fields: Value,
    }
    let lists = [
        List {
            name: "points",
            stride: 5,
            terminator: &[0xff],
            fields: json!([{"name":"point","element":"s8"},{"name":"x","element":"le-u16"},{"name":"y","element":"le-u16"}]),
        },
        List {
            name: "entrances",
            stride: 6,
            terminator: &[0xff],
            fields: json!([{"name":"entrance","element":"s8"},{"name":"x","element":"le-s16"},{"name":"y","element":"le-s16"},
                {"name":"arrival","element":"u8","bits":{"facing":4,"options":4}}]),
        },
        List {
            name: "cell_patches",
            stride: 8,
            terminator: &[0xff, 0xff],
            fields: json!([{"name":"condition","element":"le-u16","bits":{"flag":12,"mode":4}},{"name":"cells","element":"u8"}]),
        },
        List {
            name: "camera_bounds",
            stride: 4,
            terminator: &[0xff],
            fields: json!([{"name":"left","element":"s8"},{"name":"top","element":"s8"},{"name":"right","element":"s8"},{"name":"bottom","element":"s8"}]),
        },
    ];
    let hex = |at: usize| json!(format!("0x{at:x}"));
    let mut segments = vec![
        json!({"name":"offsets","end":hex(6),"stride":2,"element":"le-u16","values":[]}),
        json!({"name":"reserved","end":hex(8),"stride":2,"element":"le-u16","values":[0]}),
    ];
    let mut starts = Vec::new();
    for (list, window) in lists.iter().zip(bounds.windows(2)) {
        let (start, end) = (window[0], window[1]);
        let body = end.checked_sub(start + list.terminator.len())?;
        if body % list.stride != 0 || decoded.get(start + body..end)? != list.terminator {
            return None;
        }
        let end_name = format!("{}_end", list.name);
        if body == 0 {
            starts.push(json!(end_name));
        } else {
            starts.push(json!(list.name));
            let records = decoded[start..start + body]
                .chunks_exact(list.stride)
                .enumerate()
                .map(|(index, record)| {
                    let at = start + index * list.stride;
                    match list.name {
                        "points" => json!({"point":signed(record[0]),"x":u16_at(at + 1),"y":u16_at(at + 3)}),
                        "entrances" => json!({"entrance":signed(record[0]),"x":s16(at + 1),"y":s16(at + 3),
                            "arrival":[record[5] & 15, record[5] >> 4]}),
                        "cell_patches" => json!({"condition":[u16_at(at) & 0xfff, u16_at(at) >> 12],"cells":&record[2..]}),
                        _ => json!({"left":signed(record[0]),"top":signed(record[1]),"right":signed(record[2]),"bottom":signed(record[3])}),
                    }
                })
                .collect::<Vec<_>>();
            segments.push(
                json!({"name":list.name,"end":hex(start + body),"stride":list.stride,
                "element":"record","fields":list.fields,"records":records}),
            );
        }
        let (element, value) = if list.terminator.len() == 2 {
            ("le-u16", 0xffff)
        } else {
            ("u8", 0xff)
        };
        segments.push(
            json!({"name":end_name,"end":hex(end),"stride":list.terminator.len(),
            "element":element,"values":[value]}),
        );
    }
    // The first list follows the header; the offsets name the other three.
    segments[0]["values"] = json!(starts[1..]);
    Some(
        json!({"format":1,"kind":"typed-table","address":hex(0),"size":hex(decoded.len()),"segments":segments}),
    )
}

/// The table record a request derives from: a scene, or a map load record no
/// scene selects.
#[derive(Clone, Copy)]
enum Record {
    Scene(usize),
    Load(usize),
}
struct SceneRequest {
    record: Record,
    /// The source folder: a name under FIELD, or a path under SRC.
    name: Option<String>,
}
impl SceneRequest {
    pub(super) fn scene(index: usize) -> Self {
        Self {
            record: Record::Scene(index),
            name: None,
        }
    }
}
fn parse_scenes(text: &str) -> Result<Vec<SceneRequest>, String> {
    text.split(',')
        .map(|item| {
            let (index, name) = item
                .split_once('=')
                .map_or((item, None), |(index, name)| (index, Some(name)));
            let name = name
                .map(|name| {
                    if name.split('/').any(|part| {
                        part.is_empty()
                            || !part
                                .bytes()
                                .all(|b| b.is_ascii_uppercase() || b.is_ascii_digit() || b == b'_')
                    }) {
                        Err(format!("scene name {name:?} must be upper-case romaji"))
                    } else {
                        Ok(name.to_string())
                    }
                })
                .transpose()?;
            let index = index.trim();
            let (load, number) = match index.strip_prefix('L') {
                Some(number) => (true, number),
                None => (false, index),
            };
            let number = number
                .parse()
                .map_err(|_| format!("invalid scene index {index:?}"))?;
            Ok(SceneRequest {
                record: if load {
                    Record::Load(number)
                } else {
                    Record::Scene(number)
                },
                name,
            })
        })
        .collect()
}

/// Hexadecimal resource ids, as `--leave` lists them.
fn parse_resources(text: &str) -> Result<BTreeSet<usize>, String> {
    text.split(',')
        .map(|item| {
            let item = item.trim();
            usize::from_str_radix(item, 16)
                .ok()
                .filter(|_| !item.is_empty() && item.len() <= 3)
                .ok_or_else(|| format!("invalid resource id {item:?}"))
        })
        .collect()
}

/// Where one area keeps its map document, map binary and tile sheet.
#[derive(Clone, PartialEq, Eq, PartialOrd, Ord)]
struct Home {
    directory: String,
    stem: String,
}
impl Home {
    fn document(&self) -> String {
        format!("{}/{}.JSON", self.directory, self.stem)
    }
    fn map(&self) -> String {
        format!("{}/{}.BIN", self.directory, self.stem)
    }
    fn tiles(&self) -> String {
        format!("{}/{}_CHR.PNG", self.directory, self.stem)
    }
}

/// A private map component awaiting its place in the private map binary.
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
    banks: Vec<Vec<u16>>,
}
#[derive(Default)]
struct Output {
    regions: Vec<Value>,
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
    /// Each loaded bank's sheet and first tile, or none for a bank left out.
    tiles: Vec<Option<(String, usize)>>,
    banks: Vec<usize>,
    /// Header record origins: each layer's (x | y << 8) in half-cell units.
    origins: [usize; 3],
}

struct Deriver<'a> {
    rom: &'a [u8],
    target: DecompTarget,
    paths: NativePaths,
    directory: Directory,
    staged: Staged,
    seen: BTreeSet<usize>,
    chr_banks: BTreeMap<String, usize>,
    /// The homes of every derived request that names each resource. A
    /// resource named from more than one home is shared: containers go to the
    /// common field map, tile banks to the common tile sheet, and both keep
    /// their plans, with shared palettes', in the common field document.
    consumers: BTreeMap<usize, BTreeSet<Home>>,
    /// Resources the derivation leaves unregistered: streams the target's
    /// compressor or packer replay does not reproduce, and the resources whose
    /// alignment only their unreproduced bytes would supply.
    leave: BTreeSet<usize>,
    output: Output,
}
impl<'a> Deriver<'a> {
    fn new(rom: &'a [u8], target: DecompTarget) -> Result<Self, String> {
        Ok(Self {
            rom,
            directory: Directory::read(rom)?,
            paths: NativePaths::of(&target),
            target,
            staged: Staged::default(),
            seen: BTreeSet::new(),
            chr_banks: BTreeMap::new(),
            consumers: BTreeMap::new(),
            leave: BTreeSet::new(),
            output: Output::default(),
        })
    }
    /// Append private layers after every container's grid and metatiles, so a
    /// map binary's layouts keep their adjacent, derivation-order extents.
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
            self.output.private_inputs.push(json!({"kind":"bytes","source":layer.map,"source_offset":offset,"region_address":layer.address}));
        }
        Ok(())
    }
    fn bank_index(&mut self, values: Vec<u16>) -> usize {
        if let Some(index) = self.staged.banks.iter().position(|b| *b == values) {
            return index;
        }
        self.staged.banks.push(values);
        self.staged.banks.len() - 1
    }
    /// The shared field map document and binary.
    fn common(&self) -> Home {
        Home {
            directory: format!("{}/FIELD/COMMON", self.paths.source),
            stem: "MAP".into(),
        }
    }
    fn common_tiles(&self) -> String {
        format!("{}/GRAPHICS/COMMON/CHR.PNG", self.paths.source)
    }
    fn shared(&self, id: usize) -> bool {
        self.consumers.get(&id).is_some_and(|homes| homes.len() > 1)
    }
    /// A request's load record index.
    fn load_index(&self, record: Record) -> Result<usize, String> {
        match record {
            Record::Load(load) => Ok(load),
            Record::Scene(index) => {
                let tables = field_tables(&self.target)?;
                Ok(u16_at(self.rom, tables.scenes - ROM_BASE + index * 8 + 4)? as usize)
            }
        }
    }
    /// The six resources a load record names: container, palette, then the
    /// four tile banks.
    fn load_row(&self, load: usize) -> Result<Vec<usize>, String> {
        let tables = field_tables(&self.target)?;
        (0..6)
            .map(|k| {
                Ok(
                    u16_at(self.rom, tables.loads - ROM_BASE + load * 12 + k * 2)? as usize
                        + tables.bias,
                )
            })
            .collect()
    }
    /// A named request lives in FIELD/NAME, or at a path under SRC; COMMON
    /// names the common field map, for maps no proved place owns. An unnamed
    /// request is named after its container.
    fn home(&self, request: &SceneRequest, container: usize) -> Home {
        let name = request
            .name
            .clone()
            .unwrap_or_else(|| format!("MAP_{container:03X}"));
        if name == "COMMON" {
            return self.common();
        }
        let directory = if name.contains('/') {
            format!("{}/{name}", self.paths.source)
        } else {
            format!("{}/FIELD/{name}", self.paths.source)
        };
        let stem = name.rsplit('/').next().unwrap_or(&name).to_string();
        Home { directory, stem }
    }
    /// Record which homes name each resource before any is placed.
    fn plan_homes(&mut self, requests: &[SceneRequest]) -> Result<(), String> {
        for request in requests {
            let row = self.load_row(self.load_index(request.record)?)?;
            let home = self.home(request, row[0]);
            for id in row {
                self.consumers.entry(id).or_default().insert(home.clone());
            }
        }
        Ok(())
    }
    fn document(&mut self, name: &str) -> &mut Value {
        self.staged
            .documents
            .entry(name.to_string())
            .or_insert_with(|| json!({"format":1,"maps":{}}))
    }
    fn scene(&mut self, request: &SceneRequest) -> Result<(), String> {
        let load = self.load_index(request.record)?;
        let row = self.load_row(load)?;
        let home = self.home(request, row[0]);
        let banks = self.palette(row[1], &home)?;
        let mut tiles = Vec::new();
        for (bank, id) in row[2..].iter().enumerate() {
            let purpose = if loader_fields(&self.target)[bank] == "animation_source" {
                "map-animation-source"
            } else {
                "map-charblock-source"
            };
            tiles.push(self.tiles(*id, &home, purpose)?);
        }
        self.container(row[0], &home, tiles, banks)
    }
    /// The palette's colour banks, none when the palette is left out.
    fn palette(&mut self, id: usize, home: &Home) -> Result<Vec<usize>, String> {
        if self.leave.contains(&id) {
            return Ok(Vec::new());
        }
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
        let document = if self.shared(id) {
            self.common().document()
        } else {
            home.document()
        };
        let key = resource_name(id);
        let pointer = format!("/palettes/{key}");
        let plan = stream_plan(&stream);
        let sections = self.document(&document);
        if sections.get("palettes").is_none() {
            sections["palettes"] = json!({});
        }
        sections["palettes"][&key] = plan;
        let colors = self.paths.colors.clone();
        let address = hex_address(offset + ROM_BASE);
        self.output.regions.push(json!({"address":address,"size":small_hex(span),"kind":"golden-sun-general-lz","plan":document,"plan_section":pointer,
            "components":banks.iter().map(|bank| json!({"kind":"le-u16-array","source":colors,"pointer":format!("/banks/{bank}"),"size":32})).collect::<Vec<_>>()}));
        self.output
            .private_inputs
            .push(json!({"kind":"palette","source":colors,"banks":banks,"region_address":address}));
        Ok(banks)
    }
    /// The bank's sheet and first tile, none when the bank is left out.
    fn tiles(
        &mut self,
        id: usize,
        home: &Home,
        purpose: &str,
    ) -> Result<Option<(String, usize)>, String> {
        if self.leave.contains(&id) {
            return Ok(None);
        }
        let (offset, span) = self.directory.resource(id)?;
        let address = hex_address(offset + ROM_BASE);
        if self.seen.contains(&offset) {
            return self
                .output
                .private_inputs
                .iter()
                .find(|input| input["region_address"] == address.as_str())
                .and_then(|input| {
                    Some((
                        input["source"].as_str()?.to_string(),
                        input["tile_offset"].as_u64()? as usize,
                    ))
                })
                .map(Some)
                .ok_or_else(|| format!("tile bank {id:03x} lost its placement"));
        }
        let stream = trace_stream(self.rom, offset, span)?;
        if stream.plan["codec"] != "golden-sun-kind2-lz" || stream.decoded.len() != TILE_BANK {
            return Err(format!("tile bank {id:03x} is not one tag-2 charblock"));
        }
        self.seen.insert(offset);
        let (source, document) = if self.shared(id) || *home == self.common() {
            (self.common_tiles(), self.common().document())
        } else {
            (home.tiles(), home.document())
        };
        let next = self.chr_banks.entry(source.clone()).or_default();
        let tile_offset = *next * TILES_PER_BANK;
        *next += 1;
        let key = resource_name(id);
        let pointer = format!("/charblocks/{key}");
        let layout = json!({"format":"sequential-gba-4bpp-tiles","purpose":purpose,"tile_count":TILES_PER_BANK,"tile_width":8,"tile_height":8,"columns":32,"rows":16});
        let mut plan = stream_plan(&stream);
        plan["layout"] = layout.clone();
        let sections = self.document(&document);
        if sections.get("charblocks").is_none() {
            sections["charblocks"] = json!({});
        }
        sections["charblocks"][&key] = plan;
        let sheet = self.staged.tiles.entry(source.clone()).or_default();
        let start = tile_offset * 32;
        if sheet.len() < start + TILE_BANK {
            sheet.resize(start + TILE_BANK, 0);
        }
        sheet[start..start + TILE_BANK].copy_from_slice(&stream.decoded);
        self.output.regions.push(json!({"address":address,"size":small_hex(span),"kind":"golden-sun-kind2-lz","plan":document,"plan_section":pointer,"layout":layout,
            "components":[{"kind":"gba-4bpp-tiles","source":source,"tile_offset":tile_offset,"tile_count":TILES_PER_BANK,"size":TILE_BANK}]}));
        self.output.private_inputs.push(json!({"kind":"tiles","source":source,"tile_offset":tile_offset,"region_address":address}));
        Ok(Some((source, tile_offset)))
    }
    fn container(
        &mut self,
        id: usize,
        home: &Home,
        tiles: Vec<Option<(String, usize)>>,
        banks: Vec<usize>,
    ) -> Result<(), String> {
        if self.leave.contains(&id) {
            return Ok(());
        }
        let (base, size) = self.directory.resource(id)?;
        if !self.seen.insert(base) {
            return Ok(());
        }
        let home = if self.shared(id) {
            self.common()
        } else {
            home.clone()
        };
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
        let source = home.document();
        let map = home.map();
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
        document.insert("header".into(), json!({"format":1,"kind":"typed-table","size":small_hex(header),"segments":[
            {"name":"parameters","end":at(12),"stride":12,"element":"u8","values":params},
            {"name":"records","end":at(0x24),"stride":8,"element":"le-u16","values":records},
            {"name":"component_offsets","end":at(header),"stride":4,"element":"le-u32","values":offsets.iter().map(|o| small_hex(*o)).collect::<Vec<_>>()}]}));
        let mut regions = vec![
            json!({"address":at(0),"size":small_hex(header),"kind":"typed-table","source":source,"pointer":format!("/maps/{key}/header")}),
        ];
        let binary = self.staged.binaries.entry(map.clone()).or_default();
        let grid_offset = binary.len();
        let (mut metatile_offset, mut metatile_length) = (0, 0);
        let mut grid = None;
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
                    document.insert(section.into(), json!({"format":1,"kind":"typed-table","size":small_hex(span),
                        "segments":[{"name":"values","end":at(offset + span),"stride":span,"element":"u8","values":values}]}));
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"typed-table","source":source,"pointer":pointer}));
                    continue;
                }
                (_, Ok(stream)) => stream,
            };
            let decoded = &stream.decoded;
            let mut plan = stream_plan(&stream);
            let region = |components: Value| json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,"components":components});
            let words = little_words(decoded);
            match section {
                "metatiles" => {
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
                    plan["tilemap_length"] = json!(metatile_length);
                    plan["component"] = json!("map-metatiles-2x2");
                    plan["transform_mode"] = json!(mode);
                    plan["metatiles"] = json!(words.len() / 8);
                    plan["tilemap_source"] = json!(map);
                    plan["tilemap_offset"] = json!(metatile_offset);
                    regions.push(region(
                        json!([{"kind":"gba-tilemap16","size":decoded.len(),"delta_mode":mode,"source":map,"format":"binary","source_offset":metatile_offset,"source_length":metatile_length}]),
                    ));
                    self.output.private_inputs.push(json!({"kind":"metatiles","source":map,"source_offset":metatile_offset,"region_address":at(offset),"transform_mode":mode}));
                }
                "grid" => {
                    if decoded.len() != 65536 {
                        return Err(format!("container {id:03x} grid is not 128x128 cells"));
                    }
                    let binary = self.staged.binaries.get_mut(&map).unwrap();
                    binary.resize(binary.len().max(grid_offset + 65536), 0);
                    binary[grid_offset..grid_offset + 65536].copy_from_slice(decoded);
                    let pointer = format!("/maps/{key}/grid_compression");
                    grid = Some(plan);
                    regions.push(json!({"address":at(offset),"size":small_hex(span),"kind":"golden-sun-general-lz","plan":source,"plan_section":pointer,
                        "components":[{"kind":"golden-sun-map-grid","source":map,"width":128,"height":128,"source_offset":grid_offset,"source_length":65536,"size":65536}]}));
                    self.output.private_inputs.push(json!({"kind":"grid","source":map,"source_offset":grid_offset,"region_address":at(offset)}));
                    continue;
                }
                "descriptors" => {
                    let size = if decoded.len() % 4 == 0 { 4 } else { 1 };
                    plan["records"] = json!(decoded.chunks(size).collect::<Vec<_>>());
                    plan["component"] = json!("map-descriptors-4byte");
                    plan["record_size"] = json!(size);
                    regions.push(region(
                        json!([{"kind":"u8-array","pointer":format!("{pointer}/records"),"source":source,"size":decoded.len()}]),
                    ));
                }
                "animation_queues" | "blend_animation"
                    if words
                        .as_deref()
                        .and_then(|words| {
                            if section == "animation_queues" {
                                queue_words(words)
                            } else {
                                blend_words(words)
                            }
                        })
                        .is_some() =>
                {
                    let words = words.as_deref().unwrap();
                    if section == "animation_queues" {
                        plan["words"] = queue_words(words).unwrap();
                        plan["component"] = json!("map-animation-queues");
                        plan["word_size"] = json!(2);
                        plan["terminators"] = json!(["0xfe00", "0xffff"]);
                    } else {
                        plan["words"] = blend_words(words).unwrap();
                        plan["component"] = json!("map-blend-animation");
                        plan["word_size"] = json!(2);
                    }
                    regions.push(region(
                        json!([{"kind":"le-u16-array","pointer":format!("{pointer}/words"),"source":source,"size":decoded.len()}]),
                    ));
                }
                "positions" if positions_table(decoded).is_some() => {
                    plan["table"] = positions_table(decoded).unwrap();
                    plan["component"] = json!("map-positions");
                    regions.push(region(
                        json!([{"kind":"typed-table","pointer":format!("{pointer}/table"),"source":source,"size":decoded.len()}]),
                    ));
                }
                _ => {
                    // The descriptor grid, like the cell grid, is private map
                    // payload placed in the map binary once every container's
                    // grid and metatiles have their offsets; so are bytes whose
                    // structure is still unknown.
                    if section == "descriptor_grid" {
                        if decoded.len() != 128 * 128 {
                            return Err(format!(
                                "container {id:03x} descriptor grid is not 128x128 cells"
                            ));
                        }
                        plan["component"] = json!("map-descriptor-grid");
                        plan["width"] = json!(128);
                        plan["height"] = json!(128);
                    }
                    self.staged.layers.push(Layer {
                        map: map.clone(),
                        region: self.output.regions.len() + regions.len(),
                        address: at(offset),
                        bytes: decoded.clone(),
                    });
                    regions.push(region(
                        json!([{"kind":"u8-array","format":"binary","source":map,"size":decoded.len()}]),
                    ));
                }
            }
            document.insert(section.into(), plan);
        }
        let Some(grid) = grid.filter(|_| metatile_length != 0) else {
            return Err(format!("container {id:03x} lacks metatiles or a grid"));
        };
        // The grid's fields close the map, as in The Broken Seal's documents.
        document.insert("grid_source".into(), json!(map));
        document.insert("grid_offset".into(), json!(grid_offset));
        document.insert("grid_compression".into(), grid);
        self.document(&source)["maps"][&key] = Value::Object(document);
        self.output.regions.extend(regions);
        self.output.previews.push(Preview {
            name: home.stem.clone(),
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

/// A derived region the target's compressor does not reproduce from its input.
struct Failure {
    address: String,
    reason: String,
}
/// Write the staged native sources under `stage` and rebuild every region.
/// A region matches when its build equals the stored stream and leaves at most
/// the three alignment bytes the packer replay supplies in the complete build.
fn verify(
    deriver: &Deriver,
    stage: &Path,
    machine: LzMachine,
) -> Result<(usize, Vec<Failure>), String> {
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
    native::validate(
        &json!({"format":"camelot-style-golden-sun-native","regions":deriver.output.regions}),
    )?;
    let mut ctx = Context::new(stage);
    ctx.lz_machine = Some(machine);
    let mut matched = 0;
    let mut failures = Vec::new();
    for region in &deriver.output.regions {
        let address = number(&region["address"], "region address")? - ROM_BASE;
        let size = number(&region["size"], "region size")?;
        let stored = deriver
            .rom
            .get(address..address + size)
            .ok_or("region lies beyond the ROM")?;
        let reason = match build_entry(&mut ctx, region) {
            Err(error) => Some(error),
            Ok((built, _, _))
                if built.len() > size
                    || size - built.len() > 3
                    || stored[..built.len()] != built[..] =>
            {
                let first = built.iter().zip(stored).position(|(l, r)| l != r);
                Some(match first {
                    Some(at) => format!("encoded bytes differ at +0x{at:x} of 0x{size:x}"),
                    None => format!("built 0x{:x} of stored 0x{size:x}", built.len()),
                })
            }
            Ok(_) => None,
        };
        match reason {
            None => matched += 1,
            Some(reason) => failures.push(Failure {
                address: region["address"].as_str().unwrap_or_default().to_string(),
                reason,
            }),
        }
    }
    Ok((matched, failures))
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
fn decoded_field(
    deriver: &Deriver,
    preview: &Preview,
) -> Result<(FieldRender, Vec<Vec<u16>>), String> {
    let staged = &deriver.staged;
    let binary = &staged.binaries[&preview.map];
    let unpacked = preview.tiles[..3]
        .iter()
        .map(|bank| {
            let (source, offset) = bank.as_ref().ok_or("a loaded charblock is left out")?;
            Ok(staged.tiles[source][offset * 32..offset * 32 + TILE_BANK]
                .iter()
                .flat_map(|byte| [byte & 15, byte >> 4])
                .collect::<Vec<u8>>())
        })
        .collect::<Result<Vec<_>, String>>()?;
    if preview.banks.is_empty() {
        return Err("the loaded palette is left out".into());
    }
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
    let (mut target, mut scenes, mut output, mut stage, mut preview, mut leave) =
        (None, None, None, None, None, None);
    let mut index = 0;
    while index < arguments.len() {
        let argument = arguments[index].as_str();
        let slot = match argument {
            "--target" => &mut target,
            "--scenes" => &mut scenes,
            "-o" | "--output" => &mut output,
            "--stage" => &mut stage,
            "--preview" => &mut preview,
            "--leave" => &mut leave,
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
    if index.is_some() {
        crate::text_catalog::verify_reference(root, target.id.as_str(), &rom)
            .map_err(|e| format!("{rom_path}: {e}"))?;
    }
    let machine = super::target_lz_machine(root, &target)?;
    let mut deriver = Deriver::new(&rom, target)?;
    deriver.leave = leave
        .as_deref()
        .map(parse_resources)
        .transpose()?
        .unwrap_or_default();
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
    let requests = parse_scenes(&scenes)?;
    deriver.plan_homes(&requests)?;
    for request in &requests {
        deriver.scene(request).map_err(|e| match request.record {
            Record::Scene(index) => format!("scene {index}: {e}"),
            Record::Load(load) => format!("load record {load}: {e}"),
        })?;
    }
    deriver.place_layers()?;
    let temporary = tempfile::tempdir().map_err(|e| e.to_string())?;
    let stage_root = stage.unwrap_or_else(|| temporary.path().to_path_buf());
    let (matched, failures) = verify(&deriver, &stage_root, machine)?;
    for failure in &failures {
        eprintln!(
            "unreproduced region {}: {}",
            failure.address, failure.reason
        );
    }
    if let Some(directory) = preview {
        fs::create_dir_all(&directory).map_err(|e| e.to_string())?;
        for item in &deriver.output.previews {
            let path = directory.join(format!(
                "{}_{}.PNG",
                item.name,
                item.container.to_ascii_uppercase()
            ));
            match render(&deriver, item) {
                Ok(png) => {
                    fs::write(&path, png).map_err(|e| e.to_string())?;
                    eprintln!("preview={}", path.display());
                }
                Err(error) => eprintln!("no preview for {}: {error}", path.display()),
            }
        }
    }
    let result = json!({
        "format":"alchemy-native-index-rows-v1",
        "target":deriver.target.id.as_str(),
        "index":deriver.paths.index,
        "reference_sha256":reference,
        "tables":field_tables(&deriver.target).map(|t| json!({"scenes":hex_address(t.scenes),"loads":hex_address(t.loads),"resource_bias":small_hex(t.bias),"indexed_edition":t.indexed}))?,
        "regions":deriver.output.regions,
        "private_inputs":deriver.output.private_inputs,
        "documents":deriver.staged.documents,
        "left":deriver.leave.iter().map(|id| resource_name(*id)).collect::<Vec<_>>(),
        "verification":{"regions":deriver.output.regions.len(),"byte_exact":matched,
            "unreproduced":failures.iter().map(|f| json!({"address":f.address,"reason":f.reason})).collect::<Vec<_>>()},
    });
    let text = format!("{}\n", canonical_json(&result));
    match output {
        Some(path) => {
            if let Some(parent) = path.parent() {
                fs::create_dir_all(parent).map_err(|e| e.to_string())?;
            }
            fs::write(&path, text).map_err(|e| e.to_string())?;
            eprintln!(
                "derived scenes={} regions={} byte_exact={matched} unreproduced={} output={}",
                requests.len(),
                deriver.output.regions.len(),
                failures.len(),
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
        let rows = parse_scenes("4,187=VINASU_CHOJO,L321=DEBUG/TEST_ROOMS,9=COMMON").unwrap();
        let record = |row: &SceneRequest| match row.record {
            Record::Scene(index) => (false, index),
            Record::Load(load) => (true, load),
        };
        assert_eq!(
            (record(&rows[0]), rows[0].name.as_deref()),
            ((false, 4), None)
        );
        assert_eq!(
            (record(&rows[1]), rows[1].name.as_deref()),
            ((false, 187), Some("VINASU_CHOJO"))
        );
        // An L-prefixed number names a load record that no scene selects.
        assert_eq!(
            (record(&rows[2]), rows[2].name.as_deref()),
            ((true, 321), Some("DEBUG/TEST_ROOMS"))
        );
        assert_eq!(record(&rows[3]), (false, 9));
        for invalid in [
            "", "x", "4=vinasu", "4=", "L", "4=DEBUG/", "4=/FIELD", "4=A//B",
        ] {
            assert!(parse_scenes(invalid).is_err(), "{invalid}");
        }
    }
    #[test]
    fn animation_queues_group_by_their_headers_and_controls() {
        let words = [
            0xfd00, 1600, 2, 520, 0, 0xfe00, 0xfd01, 7, 1, 0xfe03, 0xffff,
        ];
        assert_eq!(
            queue_words(&words).unwrap(),
            json!([
                ["0xfd00", [1600, 2], [520, 0], "0xfe00"],
                ["0xfd01", [7, 1], "0xfe03"],
                "0xffff"
            ])
        );
        // A body without a header keeps the pairs it stores.
        assert_eq!(
            queue_words(&[3, 4, 0xfe00, 0xffff]).unwrap(),
            json!([[[3, 4], "0xfe00"], "0xffff"])
        );
        // The list ends with 0xffff and every group with a control word.
        assert!(queue_words(&[0xfd00, 1, 2]).is_none());
        assert!(queue_words(&[0xfd00, 1, 2, 0xffff]).is_none());
        assert!(queue_words(&[0xfd00, 1, 0xffff]).is_none());
    }
    #[test]
    fn blend_animation_pairs_values_with_durations() {
        assert_eq!(
            blend_words(&[0x3f44, 0x0e08, 10, 0xfe00, 0xffff]).unwrap(),
            json!([["0x3f44"], ["0x0e08", 10], ["0xfe00"], ["0xffff"]])
        );
        assert!(blend_words(&[0x0e08]).is_none());
    }
    #[test]
    fn positions_split_into_the_four_lists_their_readers_walk() {
        let mut data = vec![0u8; 8];
        // Points: one record and its 0xff end.
        data.extend([2, 0x10, 0, 0x20, 0, 0xff]);
        let entrances = data.len();
        // Entrances: one record facing 3 with option 1.
        data.extend([0xfe, 0xf0, 0xff, 8, 0, 0x13, 0xff]);
        let patches = data.len();
        // Cell patches: flag 0x123 in mode 2, then the two-byte end.
        data.extend([0x23, 0x21, 1, 2, 3, 4, 5, 6, 0xff, 0xff]);
        let bounds = data.len();
        data.extend([0xff, 1, 30, 20, 0xff]);
        for (at, value) in [(0, entrances), (2, patches), (4, bounds)] {
            data[at..at + 2].copy_from_slice(&(value as u16).to_le_bytes());
        }
        let table = positions_table(&data).unwrap();
        let segments = table["segments"].as_array().unwrap();
        let named = |name: &str| segments.iter().find(|s| s["name"] == name).unwrap();
        assert_eq!(
            named("offsets")["values"],
            json!(["entrances", "cell_patches", "camera_bounds"])
        );
        assert_eq!(
            named("points")["records"],
            json!([{"point":2,"x":16,"y":32}])
        );
        assert_eq!(
            named("entrances")["records"],
            json!([{"entrance":-2,"x":-16,"y":8,"arrival":[3,1]}])
        );
        assert_eq!(
            named("cell_patches")["records"],
            json!([{"condition":[0x123,2],"cells":[1,2,3,4,5,6]}])
        );
        assert_eq!(named("cell_patches_end")["values"], json!([0xffff]));
        assert_eq!(
            named("camera_bounds")["records"],
            json!([{"left":-1,"top":1,"right":30,"bottom":20}])
        );
        // The typed table serializes back to the same bytes.
        assert_eq!(typed_table(&table).unwrap(), data);
        // An empty list's offset names its end marker.
        let mut empty = data[..patches].to_vec();
        empty.extend([0xff, 0xff]);
        let moved = empty.len();
        empty.extend([0xff]);
        empty[2..4].copy_from_slice(&(patches as u16).to_le_bytes());
        empty[4..6].copy_from_slice(&(moved as u16).to_le_bytes());
        let table = positions_table(&empty).unwrap();
        assert_eq!(
            table["segments"][0]["values"],
            json!(["entrances", "cell_patches_end", "camera_bounds_end"])
        );
        assert_eq!(typed_table(&table).unwrap(), empty);
        // A list that does not end at the next offset is not this layout.
        let mut wrong = data.clone();
        wrong[bounds - 1] = 0;
        assert!(positions_table(&wrong).is_none());
        let mut reserved = data;
        reserved[6] = 1;
        assert!(positions_table(&reserved).is_none());
    }
    #[test]
    fn traced_tag2_tokens_reencode_their_stream() {
        let decoded = (0..400u32).map(|i| (i * 7 % 23) as u8).collect::<Vec<_>>();
        let plan = json!({"codec":"golden-sun-kind2-lz","tokens":{"predictor":"greedy-lz-v1","exceptions":[]}});
        let tokens = compression_plan::materialize(&decoded, &plan, None).unwrap();
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
    fn exported_plans_name_the_tag_after_the_codec_and_keep_no_tokens() {
        use psynergy::assets::lz::{
            PaletteGroup,
            PaletteOperation::{End, Literal},
        };
        let decoded = [1u8, 2, 3, 4];
        let mut stream = vec![1];
        stream.extend(
            psynergy::assets::lz::encode_palette(
                &decoded,
                &[PaletteGroup::Group(vec![
                    Literal, Literal, Literal, Literal, End,
                ])],
            )
            .unwrap(),
        );
        stream.resize(stream.len().next_multiple_of(4), 0);
        let traced = trace_stream(&stream, 0, stream.len()).unwrap();
        assert_eq!(traced.decoded, decoded);
        let plan = stream_plan(&traced);
        assert_eq!(
            plan.as_object().unwrap().keys().collect::<Vec<_>>(),
            ["format", "codec", "tag", "decoded_size", "encoded_size"]
        );
        assert_eq!(plan["encoded_size"], stream.len());
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
    fn left_resources_leave_no_rows_sections_or_sheet_slots() {
        assert_eq!(
            parse_resources("274, 3cc,2ef").unwrap(),
            BTreeSet::from([0x274, 0x3cc, 0x2ef])
        );
        for invalid in ["", "x", "1000", "3cc,"] {
            assert!(parse_resources(invalid).is_err(), "{invalid}");
        }
        // A directory whose resource 2 is one blank tag-2 charblock.
        let decoded = vec![0u8; TILE_BANK];
        let mut tokens = vec![Mtf4LzToken::Literal { width: 2 }];
        let mut covered = 1;
        while covered < TILE_BANK {
            let length = (TILE_BANK - covered).min(137) as u32;
            tokens.push(Mtf4LzToken::Copy {
                length,
                distance: 1,
            });
            covered += length as usize;
        }
        let mut rom = vec![0u8; 0x40];
        for (slot, value) in [(0usize, 0x0800_0000u32), (1, 0x0800_0000), (2, 0x0800_0040)] {
            rom[slot * 4..slot * 4 + 4].copy_from_slice(&value.to_le_bytes());
        }
        rom.extend(psynergy::assets::lz::encode_mtf4_lz(&decoded, &tokens).unwrap());
        rom.resize(rom.len().next_multiple_of(4), 0);
        let target = decomp_target(Some("tla-en")).unwrap();
        let home = Home {
            directory: "SRC/FIELD/AREA".into(),
            stem: "AREA".into(),
        };
        let mut left = Deriver::new(&rom, target).unwrap();
        left.leave = BTreeSet::from([2]);
        assert_eq!(left.tiles(2, &home, "map-charblock-source").unwrap(), None);
        assert_eq!(left.palette(2, &home).unwrap(), Vec::<usize>::new());
        assert!(left.output.regions.is_empty() && left.output.private_inputs.is_empty());
        assert!(left.staged.tiles.is_empty() && left.staged.documents.is_empty());
        let mut derived = Deriver::new(&rom, target).unwrap();
        assert_eq!(
            derived.tiles(2, &home, "map-charblock-source").unwrap(),
            Some((home.tiles(), 0))
        );
        assert_eq!(derived.output.regions.len(), 1);
        assert!(derived.staged.documents[&home.document()]["charblocks"]["002"].is_object());
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
