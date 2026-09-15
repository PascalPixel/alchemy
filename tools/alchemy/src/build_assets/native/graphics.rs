use super::*;
const COMMON: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/COMMON.json";
const SHEET: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/CHAR_COMMON.PNG";

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

pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("graphics migration needs a new directory under out".into());
    }
    let mut index = json(&root.join(INDEX))?;
    let mut manifest = json(&root.join("games/tbs/assets/manifest.json"))?;
    let series = manifest["series"]
        .as_array_mut()
        .ok_or("graphics series missing")?;
    let pos = series
        .iter()
        .position(|s| {
            s["kind"] == "golden-sun-general-lz-series"
                && s["source_prefix"] == "games/tbs/assets/graphics/resource_"
        })
        .ok_or("sprite archive series not found or already migrated")?;
    let family = series.remove(pos);
    let mut common = json(&root.join(COMMON))?;
    common["archives"] = json!({});
    let mut recipes = json(&root.join(RECIPES))?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "reference checksum")? {
        return Err("graphics reference ROM differs".into());
    }
    let mut ctx = Context::new(root);
    let img = ctx.indexed(&root.join(SHEET))?;
    let width = img.width as usize;
    let mut height = img.height as usize;
    let mut sheet = img.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
    let mut sections = BTreeMap::new();
    for input in index["private_inputs"]
        .as_array()
        .ok_or("private graphics missing")?
        .iter()
        .filter(|i| i["source"] == SHEET)
    {
        sections.insert(
            (
                address(&input["width"])?,
                address(&input["height"])?,
                json_string(&input["decoded_sha256"], "pixel digest")?.to_string(),
            ),
            input["source_rect"].clone(),
        );
    }
    let mut removed = BTreeSet::new();
    let mut inputs = vec![];
    for resource in family["resources"]
        .as_array()
        .ok_or("sprite archive resources missing")?
    {
        let id = json_string(&resource["id"], "archive ID")?.to_ascii_lowercase();
        let old = format!("games/tbs/assets/graphics/resource_{id}.json");
        let doc = json(&root.join(&old))?;
        if doc["components"].as_array().is_none_or(|c| c.len() != 2)
            || doc["components"][0]["kind"] != "le-u32-array"
            || doc["components"][1]["kind"] != "zero-skip-bytes"
            || doc["compression"]["codec"] != "golden-sun-general-lz"
        {
            return Err(format!("archive layout requires investigation: {id}"));
        }
        let entry = json!({"address":resource["address"],"size":resource["size"],"kind":"golden-sun-general-lz","plan":old,"plan_section":"compression"});
        let (encoded, _, _) = build_entry(&mut ctx, &entry)?;
        let start = address(&resource["address"])?
            .checked_sub(ROM_BASE)
            .ok_or("archive precedes ROM")?;
        if rom.get(start..start + encoded.len()) != Some(encoded.as_slice()) {
            return Err(format!("archive differs from ROM: {id}"));
        }
        let mut components = doc["components"].clone();
        let source = json_string(&components[1]["source"], "archive source")?.to_string();
        let image = ctx.indexed(&root.join(&source))?;
        check_shared_palette(root, &components[1], &image)?;
        let pixels = image.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
        let digest = sha256::hex(&pixels);
        let key = (image.width as usize, image.height as usize, digest.clone());
        if image.width as usize > width {
            return Err("archive exceeds common atlas width".into());
        }
        let rect = if let Some(rect) = sections.get(&key) {
            rect.clone()
        } else {
            let rect = json!({"x":0,"y":height,"width":image.width,"height":image.height});
            sheet.resize((height + image.height as usize) * width, 0);
            for row in 0..image.height as usize {
                sheet[(height + row) * width..(height + row) * width + image.width as usize]
                    .copy_from_slice(
                        &pixels[row * image.width as usize..(row + 1) * image.width as usize],
                    );
            }
            height += image.height as usize;
            sections.insert(key, rect.clone());
            rect
        };
        components[0]["source"] = json!(COMMON);
        components[0]["pointer"] = json!(format!("/archives/{id}/archive/offsets"));
        components[1]["source"] = json!(SHEET);
        components[1]["source_rect"] = rect.clone();
        components[1]["pixel_format"] = json!("indices");
        common["archives"][&id] = json!({"archive":doc["archive"],"components":components,"identity_status":"runtime character association not established"});
        let recipe = sha256::hex(canonical_json(&doc["compression"]).as_bytes());
        recipes["recipes"][&recipe] = doc["compression"].clone();
        index["regions"].as_array_mut().unwrap().push(json!({"address":resource["address"],"size":resource["size"],"kind":"golden-sun-general-lz","plan":RECIPES,"plan_section":format!("/recipes/{recipe}"),"components_source":COMMON,"components_pointer":format!("/archives/{id}/components")}));
        let input = json!({"kind":"archive-atlas","source":SHEET,"metadata":COMMON,"pointer":format!("/archives/{id}/components/1"),"width":image.width,"height":image.height,"source_rect":rect,"decoded_sha256":digest,"encoded_sha256":sha256::hex(&encoded),"region_address":resource["address"],"encoded_length":encoded.len(),"decoded_length":doc["compression"]["decoded_size"],"offsets":doc["archive"]["offsets"]});
        inputs.push(input);
        removed.extend([source, old]);
    }
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(inputs.clone());
    for input in index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .iter_mut()
        .filter(|i| i["source"] == SHEET)
    {
        input["atlas_width"] = json!(width);
        input["atlas_height"] = json!(height);
    }
    for (name, value) in [
        (COMMON, &common),
        (RECIPES, &recipes),
        (INDEX, &index),
        ("games/tbs/assets/manifest.json", &manifest),
    ] {
        document(&stage, name, value)?;
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    write(
        &stage,
        SHEET,
        &character::image(&sheet, width as u32, height as u32)?,
    )?;
    write(
        &stage,
        COLORS,
        &fs::read(root.join(COLORS)).map_err(|e| e.to_string())?,
    )?;
    let mut staged = Context::new(&stage);
    for input in &inputs {
        check(&mut staged, &index, input)?;
        self::pixels(&staged, input, &rom)?;
    }
    for name in &removed {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [
        COMMON,
        RECIPES,
        INDEX,
        "games/tbs/assets/manifest.json",
        SHEET,
    ] {
        write(
            root,
            name,
            &fs::read(stage.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in &removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!("sprite_archives={} removed_files={} common_atlas={width}x{height} source_and_rom=identical",inputs.len(),removed.len());
    Ok(())
}
