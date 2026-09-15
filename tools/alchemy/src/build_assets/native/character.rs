use super::*;
mod atlas;
mod raw;

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
    } else if bank["components"][0]["kind"] == "zero-skip-bank" {
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
    let component = &bank["components"][0]["components"][0];
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
    let plans = if bank["streams"].is_array() {
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
    let streams = plans.as_array().ok_or("character streams missing")?;
    if slots.last().and_then(Value::as_str) != Some("null") {
        return Err("character directory terminator differs".into());
    }
    let pointers = slots[..slots.len() - 1]
        .iter()
        .map(address)
        .collect::<Result<BTreeSet<_>, _>>()?;
    if pointers.len() != streams.len() {
        return Err("character directory aliases differ from physical streams".into());
    }
    let base = address(&bank["address"])?;
    let end = base
        .checked_sub(ROM_BASE)
        .and_then(|v| v.checked_add(address(&bank["size"]).ok()?))
        .ok_or("character extent overflows")?;
    let rom = rom.get(..end).ok_or("character bank outside ROM")?;
    let mut output = vec![0; width.checked_mul(height).ok_or("sheet extent overflows")?];
    for (frame, (slot, stream)) in pointers.iter().zip(streams).enumerate() {
        let start = slot
            .checked_sub(ROM_BASE)
            .ok_or("character frame precedes ROM")?;
        let (data, size) = match stream["codec"].as_str() {
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
        if data.len() != address(&stream["decoded_size"])?
            || size != address(&stream["encoded_size"])?
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
    let img = ctx.indexed(&root_path(&root, name)?)?;
    let (width, height, data) = indexed_rect(&img, component)?;
    if width != address(&input["width"])?
        || height != address(&input["height"])?
        || sha256::hex(&data) != json_string(&input["decoded_sha256"], "sprite hash")?
    {
        return Err(format!("private character input differs: {name}"));
    }
    check_shared_palette(&root, component, &img)?;
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
