use super::*;

fn component(ctx: &Context, input: &Value) -> Result<Value, String> {
    let doc = ctx.document(&root_path(
        &ctx.root,
        json_string(&input["metadata"], "archive metadata")?,
    )?)?;
    doc.pointer(json_string(&input["pointer"], "archive component")?)
        .cloned()
        .ok_or("archive component missing".into())
}
pub(super) fn check(ctx: &mut Context, index: &Value, input: &Value) -> Result<(), String> {
    let component = component(ctx, input)?;
    if component["source"] != input["source"]
        || component["source_rect"] != input["source_rect"]
        || component["pixel_format"] != "indices"
    {
        return Err("archive registry differs from source component".into());
    }
    let img = ctx.indexed(&root_path(
        &ctx.root,
        json_string(&input["source"], "archive sheet")?,
    )?)?;
    let (width, height, pixels) = indexed_rect(&img, &component)?;
    if width != address(&input["width"])?
        || height != address(&input["height"])?
        || sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "archive pixels")?
    {
        return Err("archive source pixels differ".into());
    }
    check_shared_palette(&ctx.root, &component, &img)?;
    let region = index["regions"]
        .as_array()
        .ok_or("archive regions missing")?
        .iter()
        .find(|r| r["address"] == input["region_address"])
        .ok_or("archive region missing")?;
    let (encoded, _, _) = build_entry(ctx, region)?;
    if sha256::hex(&encoded) != json_string(&input["encoded_sha256"], "archive encoded hash")? {
        return Err("archive encoded bytes differ".into());
    }
    Ok(())
}
pub(super) fn pixels(ctx: &Context, input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let component = component(ctx, input)?;
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&component["frame_width"])?;
    let fh = address(&component["frame_height"])?;
    let columns = address(&component["columns"])?;
    if fw == 0
        || fh == 0
        || columns == 0
        || fw.checked_mul(columns) != Some(width)
        || height % fh != 0
    {
        return Err("archive frame grid differs".into());
    }
    let start = address(&input["region_address"])?
        .checked_sub(ROM_BASE)
        .ok_or("archive precedes ROM")?;
    let length = address(&input["decoded_length"])?;
    let (data, cursor) = psynergy::assets::lz::decode_general(rom, start, rom.len(), length as u64)
        .map_err(|e| e.to_string())?;
    if cursor > start + address(&input["encoded_length"])? + 2 {
        return Err("archive lookahead exceeds bank".into());
    }
    let offsets = input["offsets"]
        .as_array()
        .ok_or("archive frame offsets missing")?;
    if offsets.is_empty() || offsets.last() != Some(&json!(0)) {
        return Err("archive directory terminator differs".into());
    }
    let mut pixels = vec![
        0;
        width
            .checked_mul(height)
            .ok_or("archive canvas overflows")?
    ];
    for (frame, offset) in offsets[..offsets.len() - 1].iter().enumerate() {
        let bytes = data
            .get(address(offset)?..)
            .ok_or("archive frame outside stream")?;
        let end = bytes
            .iter()
            .position(|b| *b == 0)
            .ok_or("archive frame terminator missing")?;
        let decoded = character::zero_skip(
            &bytes[..=end],
            fw.checked_mul(fh).ok_or("archive frame overflows")?,
        )?;
        for row in 0..fh {
            let dst = (frame / columns * fh + row) * width + frame % columns * fw;
            pixels
                .get_mut(dst..dst + fw)
                .ok_or("archive frame outside canvas")?
                .copy_from_slice(&decoded[row * fw..(row + 1) * fw]);
        }
    }
    if sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "archive pixel digest")? {
        return Err("decoded archive pixels differ".into());
    }
    Ok(pixels)
}
