use super::*;
/// The tile bank a frame input names and the UI document of its game.
fn metadata(input: &Value) -> Result<(String, String), String> {
    let sheet = json_string(&input["source"], "UI frame sheet")?;
    let directory = sheet
        .strip_suffix("/GRAPHICS/COMMON/TILE_BANK.PNG")
        .ok_or("UI frame sheet lies outside its game's common graphics")?;
    Ok((
        sheet.to_string(),
        format!("{directory}/GRAPHICS/COMMON/UI.JSON"),
    ))
}
pub(super) fn pixels(ctx: &Context, input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let (_, meta) = metadata(input)?;
    let doc = ctx.document(&ctx.root.join(meta))?;
    let offsets = doc["offset_table"]["segments"][0]["values"]
        .as_array()
        .ok_or("UI offsets absent")?;
    let recipes = ctx.document(&root_path(
        &ctx.root,
        json_string(&doc["components"][1]["plan"], "UI stream plans")?,
    )?)?;
    let plans = recipes
        .pointer(json_string(
            &doc["components"][1]["plan_section"],
            "UI recipes",
        )?)
        .and_then(Value::as_array)
        .ok_or("UI streams absent")?;
    if offsets.len() != plans.len() {
        return Err("UI offsets and streams differ".into());
    }
    let component = &doc["components"][1]["components"][0];
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&component["frame_width"])?;
    let fh = address(&component["frame_height"])?;
    let columns = address(&component["columns"])?;
    if columns == 0
        || fw == 0
        || fh == 0
        || width != columns * fw
        || height != plans.len().div_ceil(columns) * fh
    {
        return Err("UI frame geometry differs".into());
    }
    let base = address(&doc["address"])?;
    let mut pixels = vec![0; width.checked_mul(height).ok_or("UI dimensions overflow")?];
    for (frame, (offset, plan)) in offsets.iter().zip(plans).enumerate() {
        let mut definition = plan.clone();
        definition["region_address"] = json!(hex_address(base + address(offset)?));
        let decoded = decode_buffer(&definition, rom, fw * fh)?;
        for row in 0..fh {
            let start = (frame / columns * fh + row) * width + frame % columns * fw;
            pixels[start..start + fw].copy_from_slice(&decoded[row * fw..(row + 1) * fw]);
        }
    }
    if sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "UI frame digest")? {
        return Err("UI frame pixels differ".into());
    }
    Ok(pixels)
}
pub(super) fn check(ctx: &mut Context, index: &Value, input: &Value) -> Result<(), String> {
    let (sheet, meta) = metadata(input)?;
    let doc = ctx.document(&ctx.root.join(meta))?;
    let component = &doc["components"][1]["components"][0];
    if component["source"] != input["source"] || component["source_rect"] != input["source_rect"] {
        return Err("UI registry differs".into());
    }
    let image = ctx.indexed(&ctx.root.join(sheet))?;
    let (w, h, pixels) = indexed_rect(&image, component)?;
    if w != address(&input["width"])?
        || h != address(&input["height"])?
        || sha256::hex(&pixels) != json_string(&input["decoded_sha256"], "UI digest")?
    {
        return Err("UI source pixels differ".into());
    }
    let region = index["regions"]
        .as_array()
        .unwrap()
        .iter()
        .find(|r| r["address"] == input["region_address"])
        .ok_or("UI region absent")?;
    if sha256::hex(&build_entry(ctx, region)?.0)
        != json_string(&input["encoded_sha256"], "UI encoded digest")?
    {
        return Err("UI source bytes differ".into());
    }
    Ok(())
}
#[test]
fn frame_inputs_read_their_own_games_document() {
    for game in ["games/THE BROKEN SEAL/SRC", "games/THE LOST AGE/SRC"] {
        let input = json!({"source": format!("{game}/GRAPHICS/COMMON/TILE_BANK.PNG")});
        let (sheet, meta) = metadata(&input).unwrap();
        assert_eq!(sheet, format!("{game}/GRAPHICS/COMMON/TILE_BANK.PNG"));
        assert_eq!(meta, format!("{game}/GRAPHICS/COMMON/UI.JSON"));
    }
    let elsewhere = json!({"source": "games/THE LOST AGE/SRC/GRAPHICS/COMMON/TILE.PNG"});
    assert!(metadata(&elsewhere).is_err());
}
