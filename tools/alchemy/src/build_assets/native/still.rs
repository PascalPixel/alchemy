use super::*;

pub(super) fn pixels(input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let start = address(&input["region_address"])?
        .checked_sub(ROM_BASE)
        .ok_or("still precedes ROM")?;
    let size = address(&input["encoded_length"])?;
    let stream = rom
        .get(start..start.checked_add(size).ok_or("still extent overflows")?)
        .ok_or("still exceeds ROM")?;
    if sha256::hex(stream) != json_string(&input["encoded_sha256"], "still encoded digest")? {
        return Err("still ROM digest differs".into());
    }
    let palette = address(&input["palette_entries"])? * 2;
    let count = address(&input["width"])?
        .checked_mul(address(&input["height"])?)
        .ok_or("still dimensions overflow")?;
    let pixels = psynergy::assets::compression::decode_delta7(
        stream
            .get(palette..)
            .ok_or("still palette exceeds stream")?,
        count,
    )
    .map_err(|e| e.to_string())?;
    if sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "still pixel digest")? {
        return Err("still decoded pixels differ".into());
    }
    Ok(pixels)
}
pub(super) fn check(ctx: &mut Context, index: &Value, input: &Value) -> Result<(), String> {
    let region = index["regions"]
        .as_array()
        .ok_or("still regions absent")?
        .iter()
        .find(|r| r["address"] == input["region_address"])
        .ok_or("still region absent")?;
    if region["source"] != input["source"] || region["source_rect"] != input["source_rect"] {
        return Err("still registry differs".into());
    }
    let image = ctx.indexed(&root_path(
        &ctx.root,
        json_string(&input["source"], "still source")?,
    )?)?;
    let (w, h, pixels) = indexed_rect(&image, region)?;
    if w != address(&input["width"])?
        || h != address(&input["height"])?
        || sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "still digest")?
    {
        return Err("still source pixels differ".into());
    }
    let (built, _, _) = build_entry(ctx, region)?;
    if sha256::hex(&built) != json_string(&input["encoded_sha256"], "still encoded digest")? {
        return Err("still encoded bytes differ".into());
    }
    Ok(())
}
