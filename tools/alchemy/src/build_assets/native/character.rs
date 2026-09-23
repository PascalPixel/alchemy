use super::*;
mod atlas;
mod catalog;
pub(in crate::build_assets) use catalog::inventory;
mod raw;
pub(super) use catalog::{catalog, Catalog, Descriptor};

fn gray(entries: usize) -> Vec<[u8; 3]> {
    (0..entries).map(|i| [i as u8; 3]).collect()
}
pub(super) fn image(pixels: &[u8], width: u32, height: u32) -> Result<Vec<u8>, String> {
    if pixels.len() != width as usize * height as usize || pixels.iter().any(|p| *p >= 224) {
        return Err("character sheet dimensions or indices differ".into());
    }
    let mut data = vec![];
    {
        let mut encoder = png::Encoder::new(&mut data, width, height);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(gray(224).concat());
        encoder.set_trns(vec![0]);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(pixels)
            .map_err(|e| e.to_string())?;
    }
    Ok(data)
}
pub(super) fn zero_skip(bytes: &[u8], length: usize) -> Result<Vec<u8>, String> {
    let mut output = vec![];
    for (i, byte) in bytes.iter().enumerate() {
        if *byte == 0 {
            if i + 1 != bytes.len() || output.len() != length {
                return Err("sprite terminator or frame extent differs".into());
            }
            return Ok(output);
        }
        let count = if *byte >= 0xe0 {
            usize::from(*byte - 0xdf)
        } else {
            1
        };
        if output
            .len()
            .checked_add(count)
            .is_none_or(|end| end > length)
        {
            return Err("sprite zero run exceeds frame".into());
        }
        if *byte >= 0xe0 {
            output.resize(output.len() + count, 0);
        } else {
            output.push(*byte);
        }
    }
    Err("sprite stream lacks terminator".into())
}
fn bank(ctx: &Context, input: &Value) -> Result<Value, String> {
    let path = root_path(
        &ctx.root,
        json_string(&input["metadata"], "character metadata")?,
    )?;
    let metadata = ctx.document(&path)?;
    metadata
        .pointer(json_string(&input["pointer"], "character bank pointer")?)
        .cloned()
        .ok_or("character bank missing".into())
}
fn component(bank: &Value) -> &Value {
    if bank["kind"] == "zero-skip-bank" {
        bank
    } else if matches!(
        bank["components"][0]["kind"].as_str(),
        Some("zero-skip-bank" | "zero-skip-bytes")
    ) {
        &bank["components"][0]
    } else {
        &bank["components"][0]["components"][0]
    }
}
fn pixels(ctx: &Context, input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let bank = bank(ctx, input)?;
    if component(&bank)["kind"] == "zero-skip-bank" {
        return raw::pixels(input, component(&bank), rom);
    }
    let component = component(&bank);
    let absent = absent_frames(component)?;
    // Uncompressed frames (The Lost Age codec 0) lie back to back in the bank.
    let raw = bank["components"][0]["kind"] == "zero-skip-bytes";
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&component["frame_width"])?;
    let fh = address(&component["frame_height"])?;
    let columns = address(&component["columns"])?;
    if fw == 0 || fh == 0 || columns == 0 || width != fw * columns || height % fh != 0 {
        return Err("character frame geometry differs".into());
    }
    let slots = bank["directory"]["slots"]
        .as_array()
        .ok_or("character directory missing")?;
    let plans = if raw {
        Value::Null
    } else if bank["streams"].is_array() {
        bank["streams"].clone()
    } else {
        let document = ctx.document(&root_path(
            &ctx.root,
            json_string(&bank["streams"]["source"], "sprite recipes")?,
        )?)?;
        document
            .pointer(json_string(
                &bank["streams"]["pointer"],
                "sprite recipe pointer",
            )?)
            .cloned()
            .ok_or("sprite recipes missing")?
    };
    // The Broken Seal closes each directory with a null slot. The Lost Age
    // packs directories back to back, so every declared slot is a frame.
    let slots = match slots.split_last() {
        Some((last, rest)) if last == "null" => rest,
        _ if slots.len() == address(&bank["directory"]["slot_count"])? => &slots[..],
        _ => return Err("character directory terminator differs".into()),
    };
    let pointers = slots
        .iter()
        .map(address)
        .collect::<Result<BTreeSet<_>, _>>()?;
    let streams = match plans.as_array() {
        Some(streams) => streams.clone(),
        None if raw => vec![Value::Null; pointers.len()],
        None => return Err("character streams missing".into()),
    };
    if pointers.len() != streams.len() {
        return Err("character directory aliases differ from physical streams".into());
    }
    let base = address(&bank["address"])?;
    let end = base
        .checked_sub(ROM_BASE)
        .and_then(|v| v.checked_add(address(&bank["size"]).ok()?))
        .ok_or("character extent overflows")?;
    // A general-LZ reader may fetch past its stream's stored extent.
    let whole = rom;
    let rom = rom.get(..end).ok_or("character bank outside ROM")?;
    let mut output = vec![0; width.checked_mul(height).ok_or("sheet extent overflows")?];
    let ends = pointers
        .iter()
        .skip(1)
        .map(|next| next.checked_sub(ROM_BASE))
        .chain(std::iter::once(Some(end)))
        .collect::<Option<Vec<_>>>()
        .ok_or("character frame precedes ROM")?;
    for (frame, (slot, stream)) in pointers.iter().zip(&streams).enumerate() {
        let start = slot
            .checked_sub(ROM_BASE)
            .ok_or("character frame precedes ROM")?;
        // An absent frame stores no zero-skip bytes: its arena stream is a
        // bare raw split, and its sheet cell stays blank.
        if absent.contains(&frame) {
            if stream["codec"] != "golden-sun-arena-lz"
                || address(&stream["decoded_size"])? != 0
                || address(&stream["encoded_size"])? != 2
                || rom.get(start..start + 2) != Some(&[0u8, 0][..])
                || ends[frame] != start + 2
            {
                return Err("absent character frame is not a bare arena split".into());
            }
            continue;
        }
        let (data, size) = match stream["codec"].as_str() {
            None if raw => {
                let data = rom
                    .get(start..ends[frame])
                    .ok_or("character frame outside bank")?
                    .to_vec();
                if data.last() != Some(&0) {
                    return Err("raw character frame does not end at the next frame".into());
                }
                (data, 0)
            }
            Some("golden-sun-arena-lz") => {
                let (data, size, _) =
                    psynergy::assets::lz::decode_arena(rom, start).map_err(|e| e.to_string())?;
                (data, size)
            }
            Some("golden-sun-general-lz") => {
                let (data, cursor) = psynergy::assets::lz::decode_general(
                    rom,
                    start,
                    rom.len(),
                    address(&stream["decoded_size"])? as u64,
                )
                .map_err(|e| e.to_string())?;
                if cursor > start + address(&stream["encoded_size"])? + 2 {
                    return Err("sprite lookahead exceeds stream boundary".into());
                }
                (data, address(&stream["encoded_size"])?)
            }
            // The leading tag names each frame's stream: 0 general, 1 palette.
            Some("golden-sun-tagged-lz") => {
                crate::build_assets::derive_index::tagged_extent(whole, start, whole.len())?
            }
            Some("golden-sun-tagged-palette-lz" | "golden-sun-palette-lz") => {
                let offset = if stream["codec"] == "golden-sun-tagged-palette-lz" {
                    1
                } else {
                    0
                };
                let (data, cursor) = psynergy::assets::lz::decode_palette(
                    rom,
                    start + offset,
                    rom.len(),
                    address(&stream["decoded_size"])? as u64,
                )
                .map_err(|e| e.to_string())?;
                if cursor > start + address(&stream["encoded_size"])? + 2 {
                    return Err("sprite palette lookahead exceeds stream boundary".into());
                }
                (data, address(&stream["encoded_size"])?)
            }
            _ => return Err("unregistered character frame codec".into()),
        };
        if !raw
            && (data.len() != address(&stream["decoded_size"])?
                || size != address(&stream["encoded_size"])?)
        {
            return Err("character stream extent differs".into());
        }
        let frame_size = fw.checked_mul(fh).ok_or("frame extent overflows")?;
        let decoded = if component["kind"] == "zero-skip-bytes" {
            zero_skip(&data, frame_size)?
        } else if component["kind"] == "indexed-bytes" && data.len() == frame_size {
            data
        } else {
            return Err("character frame codec or size differs".into());
        };
        for y in 0..fh {
            let dst = (frame / columns * fh + y) * width + frame % columns * fw;
            output
                .get_mut(dst..dst + fw)
                .ok_or("frame outside sheet")?
                .copy_from_slice(&decoded[y * fw..(y + 1) * fw]);
        }
    }
    if sha256::hex(&output) != json_string(&input["decoded_sha256"], "sprite digest")? {
        return Err("character pixels differ from registered input".into());
    }
    Ok(output)
}
/// One descriptor's decoded frames, as rendered into a character sheet.
pub(super) struct Sheet {
    pub frames: usize,
    pub unique_frames: usize,
    pub codec: &'static str,
    pub png: Vec<u8>,
}
/// Decode every frame each descriptor of `target` names and colour it with the
/// target's sprite palette. Descriptors whose directory is loaded at runtime
/// (a null frame directory) have no sheet.
pub(super) fn sheets(
    target: &DecompTarget,
    rom: &[u8],
) -> Result<(Catalog, Vec<Descriptor>, BTreeMap<usize, Sheet>), String> {
    let catalog = catalog(target)?;
    let descriptors = catalog.descriptors(rom)?;
    let directories = catalog.directories(rom, &descriptors)?;
    let palette = catalog.palette(rom)?;
    let mut sheets = BTreeMap::new();
    for descriptor in &descriptors {
        let Some(slots) = directories.get(&descriptor.frame_directory) else {
            continue;
        };
        let codec = catalog::frame_codec(catalog.game, descriptor.frame_codec)
            .ok_or_else(|| format!("descriptor {} frame codec is unregistered", descriptor.id))?;
        let frames = slots
            .iter()
            .map(|slot| {
                catalog::frame(
                    rom,
                    catalog.game,
                    descriptor.frame_codec,
                    *slot,
                    descriptor.width,
                    descriptor.height,
                )
                .map_err(|e| format!("descriptor {}: {e}", descriptor.id))
            })
            .collect::<Result<Vec<_>, _>>()?;
        let (width, height, pixels) =
            catalog::sheet(&frames, descriptor.width, descriptor.height, 8);
        sheets.insert(
            descriptor.id,
            Sheet {
                frames: frames.len(),
                unique_frames: slots.iter().collect::<BTreeSet<_>>().len(),
                codec,
                png: catalog::preview(&pixels, width, height, &palette)?,
            },
        );
    }
    Ok((catalog, descriptors, sheets))
}
pub(super) fn extract_all(root: &Path, inputs: &Value, rom: &[u8]) -> Result<(), String> {
    atlas::extract(root, inputs, rom)
}
pub(super) fn check(ctx: &mut Context, input: &Value) -> Result<(), String> {
    let root = ctx.root.clone();
    let name = json_string(&input["source"], "sprite source")?;
    let bank = bank(ctx, input)?;
    let component = component(&bank);
    if component["source"] != name || component["pixel_format"] != "indices" {
        return Err("character registry differs from pixel component".into());
    }
    let sheet = root_path(&root, name)?;
    let img = ctx.indexed(&sheet)?;
    let (width, height, data) = indexed_rect(&img, component)?;
    if width != address(&input["width"])?
        || height != address(&input["height"])?
        || sha256::hex(&data) != json_string(&input["decoded_sha256"], "sprite hash")?
    {
        return Err(format!("private character input differs: {name}"));
    }
    ctx.shared_palette(&sheet, component, &img)?;
    if component.get("source_rect") != input.get("source_rect") {
        return Err("private character rectangle differs from component".into());
    }
    let (encoded, _, _) = build_entry(ctx, &bank)?;
    if sha256::hex(&encoded) != json_string(&input["encoded_sha256"], "character bank hash")? {
        return Err("character bank encoding differs".into());
    }
    Ok(())
}

#[test]
fn sprite_zero_runs_require_exact_frame_and_terminator() {
    let pixels = [vec![0; 33], vec![1, 0xdf], vec![0; 8]].concat();
    let encoded = psynergy::assets::compression::encode_zero_skip(&pixels).unwrap();
    assert_eq!(zero_skip(&encoded, pixels.len()).unwrap(), pixels);
    assert!(zero_skip(&encoded, pixels.len() - 1).is_err());
    assert!(zero_skip(&encoded[..encoded.len() - 1], pixels.len()).is_err());
    assert!(zero_skip(&[0, 1], 0).is_err());
    assert!(zero_skip(&[0xe0, 0], 32).is_err());
}

#[test]
fn absent_arena_frames_are_bare_splits_with_blank_cells() {
    let root = tempfile::tempdir().unwrap();
    let frame = [vec![3; 4], vec![0; 12]].concat();
    let zero_skip = psynergy::assets::compression::encode_zero_skip(&frame).unwrap();
    let stream = psynergy::assets::lz::compress_arena(&zero_skip, &[]).unwrap();
    let mut rom = vec![0xaa; 8];
    let first = ROM_BASE + rom.len();
    rom.extend(&stream);
    let second = ROM_BASE + rom.len();
    rom.extend([0, 0]);
    let size = rom.len() - 8;
    rom.extend([0, 0xaa]);
    let document = |absent: Value| {
        json!({"banks":{"field":{"format":1,"kind":"components","address":first,"size":size,
            "components":[{"kind":"golden-sun-general-lz","plan":"BANK.JSON","plan_section":"/banks/field/streams",
                "size":size,"components":[{"kind":"zero-skip-bytes","source":"CHAR.PNG","frame_width":4,
                "frame_height":4,"columns":2,"pixel_format":"indices","absent_frames":absent}]}],
            "streams":[{"codec":"golden-sun-arena-lz","decoded_size":zero_skip.len(),"encoded_size":stream.len()},
                {"codec":"golden-sun-arena-lz","decoded_size":0,"encoded_size":2}],
            "directory":{"format":1,"kind":"pointer-table","base_address":first,"address":ROM_BASE,
                "slot_count":2,"slots":[format!("{first:#x}"), format!("{second:#x}")]}}}})
    };
    fs::write(
        root.path().join("BANK.JSON"),
        document(json!([1])).to_string(),
    )
    .unwrap();
    let expected = (0..4)
        .flat_map(|row| [&frame[row * 4..row * 4 + 4], &[0; 4][..]].concat())
        .collect::<Vec<_>>();
    let input = json!({"metadata":"BANK.JSON","pointer":"/banks/field","width":8,"height":4,
        "decoded_sha256": sha256::hex(&expected)});
    assert_eq!(
        pixels(&Context::new(root.path()), &input, &rom).unwrap(),
        expected
    );
    // Unnamed, the bare split has no stream of its own inside the bank.
    fs::write(
        root.path().join("BANK.JSON"),
        document(json!([0])).to_string(),
    )
    .unwrap();
    assert!(pixels(&Context::new(root.path()), &input, &rom).is_err());
}

#[test]
fn packed_raw_frames_decode_between_directory_slots() {
    let root = tempfile::tempdir().unwrap();
    let frames = [[vec![3; 4], vec![0; 12]].concat(), vec![5; 16]];
    let streams = frames
        .iter()
        .map(|frame| psynergy::assets::compression::encode_zero_skip(frame).unwrap())
        .collect::<Vec<_>>();
    let mut rom = vec![0xaa; 8];
    let first = ROM_BASE + rom.len();
    rom.extend(&streams[0]);
    let second = ROM_BASE + rom.len();
    rom.extend(&streams[1]);
    let size = rom.len() - 8;
    rom.push(0xaa);
    let bank = |slots: Value| {
        json!({"banks":{"field":{"format":1,"kind":"components","address":first,"size":size,
            "components":[{"kind":"zero-skip-bytes","source":"CHAR.PNG","frame_width":4,"frame_height":4,
                "columns":2,"frames":2,"size":size,"pixel_format":"indices"}],
            "directory":{"format":1,"kind":"pointer-table","base_address":first,"address":ROM_BASE,
                "slot_count":2,"slots":slots}}}})
    };
    let slots = json!([format!("{first:#x}"), format!("{second:#x}")]);
    fs::write(root.path().join("PACKED.JSON"), bank(slots).to_string()).unwrap();
    let ctx = Context::new(root.path());
    let expected = (0..4)
        .flat_map(|row| {
            [
                &frames[0][row * 4..row * 4 + 4],
                &frames[1][row * 4..row * 4 + 4],
            ]
            .concat()
        })
        .collect::<Vec<_>>();
    let mut input = json!({"metadata":"PACKED.JSON","pointer":"/banks/field","width":8,"height":4,
        "decoded_sha256": sha256::hex(&expected)});
    assert_eq!(pixels(&ctx, &input, &rom).unwrap(), expected);
    input["decoded_sha256"] = json!(sha256::hex(&[0u8; 32]));
    assert!(pixels(&ctx, &input, &rom).is_err());
    // A slot count that disagrees with the unterminated slots is refused.
    let mut document = bank(json!([format!("{first:#x}")]));
    document["banks"]["field"]["directory"]["slot_count"] = json!(2);
    fs::write(root.path().join("SHORT.JSON"), document.to_string()).unwrap();
    input["metadata"] = json!("SHORT.JSON");
    assert!(pixels(&Context::new(root.path()), &input, &rom).is_err());
}
