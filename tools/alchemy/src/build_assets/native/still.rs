use super::*;
const META: &str = "games/tbs/SRC/GRAPHICS/COMMON/STILL.json";
const SHEET: &str = "games/tbs/SRC/GRAPHICS/COMMON/STILL.PNG";

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
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("still migration needs new directory under out".into());
    }
    let manifest_name = "games/tbs/assets/manifest.json";
    let mut manifest = json(&root.join(manifest_name))?;
    let series = manifest["series"]
        .as_array_mut()
        .ok_or("asset series absent")?;
    let pos = series
        .iter()
        .position(|s| s["kind"] == "golden-sun-delta7-still-series")
        .ok_or("still series absent or already migrated")?;
    let family = series.remove(pos);
    let old_index_name = json_string(&family["index"], "still index")?;
    let old_index = json(&root.join(old_index_name))?;
    let mut index = json(&root.join(INDEX))?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("still reference differs".into());
    }
    let ctx = Context::new(root);
    let width = address(&family["width"])?;
    let height = address(&family["height"])?;
    let colors = address(&family["palette_entries"])?;
    let mut sheet = vec![];
    let mut meta = json!({"format":1,"kind":"indexed-still-atlas","images":{},"palettes":{}});
    let mut removed = BTreeSet::from([old_index_name.to_string()]);
    let mut inputs = vec![];
    for resource in family["resources"]
        .as_array()
        .ok_or("still resources absent")?
    {
        let id = json_string(&resource["id"], "still ID")?;
        let item = old_index["resources"]
            .as_array()
            .ok_or("still index resources absent")?
            .iter()
            .find(|i| i["id"] == id)
            .ok_or("still identity absent")?;
        let old = root
            .join(old_index_name)
            .parent()
            .unwrap()
            .join(json_string(&item["file"], "still filename")?);
        let old_name = root_relative(root, &old)?;
        let bytes = fs::read(&old).map_err(|e| e.to_string())?;
        let encoded = psynergy::assets::compression::delta7_image(&bytes, width, height, colors)
            .map_err(|e| e.to_string())?;
        let start = address(&resource["address"])? - ROM_BASE;
        if encoded.len() != address(&resource["size"])?
            || rom.get(start..start + encoded.len()) != Some(encoded.as_slice())
        {
            return Err(format!("still differs from ROM: {id}"));
        }
        let image = ctx.indexed(&old)?;
        let pixels = image.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
        let rect = json!({"x":0,"y":sheet.len()/width,"width":width,"height":height});
        sheet.extend(&pixels);
        let palette_bytes = &encoded[..colors * 2];
        let key = sha256::hex(palette_bytes);
        meta["palettes"][&key] = json!(palette_bytes
            .chunks_exact(2)
            .map(|p| u16::from_le_bytes([p[0], p[1]]))
            .collect::<Vec<_>>());
        let region = json!({"address":resource["address"],"size":resource["size"],"kind":"golden-sun-delta7-still","source":SHEET,"source_rect":rect,"width":width,"height":height,"palette_entries":colors,"palette":{"kind":"le-u16-array","source":META,"pointer":format!("/palettes/{key}")}});
        meta["images"][id] =
            json!({"source_rect":rect,"palette":key,"identity":null,"runtime_role":null});
        let input = json!({"kind":"still-atlas","source":SHEET,"source_rect":rect,"width":width,"height":height,"atlas_width":width,"region_address":resource["address"],"encoded_length":encoded.len(),"encoded_sha256":sha256::hex(&encoded),"decoded_sha256":sha256::hex(&pixels),"palette_entries":colors});
        self::pixels(&input, &rom)?;
        inputs.push(input);
        index["regions"].as_array_mut().unwrap().push(region);
        removed.insert(old_name);
    }
    let atlas_height = sheet.len() / width;
    for input in &mut inputs {
        input["atlas_height"] = json!(atlas_height);
    }
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(inputs.clone());
    for (name, doc) in [(INDEX, &index), (manifest_name, &manifest), (META, &meta)] {
        document(&stage, name, doc)?;
    }
    write(
        &stage,
        SHEET,
        &character::image(&sheet, width as u32, atlas_height as u32)?,
    )?;
    let mut staged = Context::new(&stage);
    for input in &inputs {
        check(&mut staged, &index, input)?;
    }
    for name in removed
        .iter()
        .map(String::as_str)
        .chain([INDEX, manifest_name])
    {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [INDEX, manifest_name, META, SHEET] {
        write(
            root,
            name,
            &fs::read(stage.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in &removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!(
        "stills={} atlas={width}x{atlas_height} source_and_rom=identical",
        inputs.len()
    );
    Ok(())
}
