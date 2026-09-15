use super::*;
const SHEET: &str = "games/tbs/SRC/GRAPHICS/COMMON/PORTRAIT.PNG";
const META: &str = "games/tbs/SRC/GRAPHICS/COMMON/PORTRAIT.JSON";
pub(super) fn pixels(input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let start = address(&input["region_address"])?
        .checked_sub(ROM_BASE)
        .ok_or("portrait precedes ROM")?;
    let length = address(&input["encoded_length"])?;
    let stream = rom
        .get(
            start
                ..start
                    .checked_add(length)
                    .ok_or("portrait extent overflow")?,
        )
        .ok_or("portrait exceeds ROM")?;
    if sha256::hex(stream) != json_string(&input["encoded_sha256"], "portrait encoded digest")? {
        return Err("portrait ROM differs".into());
    }
    let count = address(&input["width"])?
        .checked_mul(address(&input["height"])?)
        .ok_or("portrait dimension overflow")?;
    let pixels =
        psynergy::assets::compression::decode_mtf4(stream, count).map_err(|e| e.to_string())?;
    if sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "portrait digest")? {
        return Err("portrait pixels differ".into());
    }
    Ok(pixels)
}
pub(super) fn check(ctx: &Context, input: &Value) -> Result<(), String> {
    let doc = ctx.document(&ctx.root.join(META))?;
    let component = doc
        .pointer(json_string(&input["pointer"], "portrait component")?)
        .ok_or("portrait component absent")?;
    if component["source"] != input["source"] || component["source_rect"] != input["source_rect"] {
        return Err("portrait registry differs".into());
    }
    let image = ctx.indexed(&ctx.root.join(SHEET))?;
    let (w, h, pixels) = indexed_rect(&image, component)?;
    if w != address(&input["width"])?
        || h != address(&input["height"])?
        || sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "portrait digest")?
    {
        return Err("portrait source pixels differ".into());
    }
    let encoded = build_component_cached(ctx, component)?;
    if sha256::hex(&encoded.data)
        != json_string(&input["encoded_sha256"], "portrait encoded digest")?
    {
        return Err("portrait source bytes differ".into());
    }
    Ok(())
}
