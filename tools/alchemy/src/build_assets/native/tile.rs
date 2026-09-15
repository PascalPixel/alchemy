use super::*;
pub(super) fn image(pixels: &[u8], width: u32, height: u32) -> Result<Vec<u8>, String> {
    if pixels.len() != width as usize * height as usize {
        return Err("tile image extent differs".into());
    }
    let mut bytes = vec![];
    {
        let mut encoder = png::Encoder::new(&mut bytes, width, height);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette((0..=255u8).flat_map(|p| [p, p, p]).collect::<Vec<_>>());
        encoder.set_trns(vec![0]);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(pixels)
            .map_err(|e| e.to_string())?;
    }
    Ok(bytes)
}

pub(super) fn pixels(input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    if width % 8 != 0 || height % 8 != 0 {
        return Err("tile dimensions differ".into());
    }
    let count = width
        .checked_mul(height)
        .ok_or("tile dimensions overflow")?;
    let bpp = input.get("bpp").map(address).transpose()?.unwrap_or(8);
    if bpp != 4 && bpp != 8 {
        return Err("tile depth differs".into());
    }
    let canvas = count * bpp / 8;
    let length = input
        .get("decoded_length")
        .map(address)
        .transpose()?
        .unwrap_or(canvas);
    if length > canvas {
        return Err("tile decoded extent exceeds canvas".into());
    }
    let buffer = decode_buffer(input, rom, length)?;
    let offset = input
        .get("component_offset")
        .map(address)
        .transpose()?
        .unwrap_or(0);
    let mut decoded = buffer
        .get(
            offset
                ..offset
                    .checked_add(length)
                    .ok_or("tile component overflows")?,
        )
        .ok_or("tile component exceeds decoded buffer")?
        .to_vec();
    decoded.resize(canvas, 0);
    let decoded = if bpp == 4 {
        decoded
            .into_iter()
            .flat_map(|p| [p & 15, p >> 4])
            .collect::<Vec<_>>()
    } else {
        decoded
    };
    let mut pixels = vec![0; count];
    let mut cursor = 0;
    for (x, y) in tile_coordinates(width, height, input)? {
        for row in 0..8 {
            pixels[(y + row) * width + x..(y + row) * width + x + 8].copy_from_slice(
                decoded
                    .get(cursor..cursor + 8)
                    .ok_or("tile stream truncated")?,
            );
            cursor += 8;
        }
    }
    if sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "tile digest")? {
        return Err("tile pixels differ".into());
    }
    Ok(pixels)
}
pub(super) fn check(ctx: &mut Context, index: &Value, input: &Value) -> Result<(), String> {
    let region = index["regions"]
        .as_array()
        .ok_or("tile regions absent")?
        .iter()
        .find(|r| r["address"] == input["region_address"])
        .ok_or("tile region absent")?;
    let component_index = input
        .get("component_index")
        .map(address)
        .transpose()?
        .unwrap_or(0);
    let component = region
        .get("components")
        .and_then(Value::as_array)
        .and_then(|c| c.get(component_index))
        .unwrap_or(region);
    if component["source"] != input["source"] || component["source_rect"] != input["source_rect"] {
        return Err("tile registry differs".into());
    }
    let image = ctx.indexed(&root_path(
        &ctx.root,
        json_string(&input["source"], "tile source")?,
    )?)?;
    let (w, h, pixels) = indexed_rect(&image, component)?;
    if w != address(&input["width"])?
        || h != address(&input["height"])?
        || sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "tile digest")?
    {
        return Err("tile source differs".into());
    }
    let (encoded, _, _) = build_entry(ctx, region)?;
    if sha256::hex(&encoded) != json_string(&input["encoded_sha256"], "tile encoded digest")? {
        return Err("tile encoded bytes differ".into());
    }
    Ok(())
}
