use super::*;
const COMMON: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/COMMON.json";
const SHEET: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/CHAR_COMMON.PNG";

pub(super) fn wrap(common: &mut Value) -> Result<(), String> {
    let mut pointers = BTreeMap::new();
    for (key, bank) in common["banks"]
        .as_object_mut()
        .ok_or("sprite banks missing")?
    {
        if bank["kind"] != "zero-skip-bank" {
            continue;
        }
        let component = bank.clone();
        *bank = json!({"kind":"components","address":component["address"],"size":component["size"],"directory":component["directory"],"identity":null,"identity_status":"runtime name association not established","components":[component]});
        pointers.insert(format!("/banks/{key}"), format!("/banks/{key}/components"));
    }
    if let Some(regions) = common["regions"].as_object_mut() {
        for region in regions.values_mut() {
            for component in region["components"]
                .as_array_mut()
                .ok_or("sprite region components missing")?
            {
                if let Some(replacement) =
                    component["pointer"].as_str().and_then(|p| pointers.get(p))
                {
                    component["pointer"] = json!(replacement);
                }
            }
        }
    }
    Ok(())
}

pub(super) fn pixels(input: &Value, bank: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&bank["frame_width"])?;
    let fh = address(&bank["frame_height"])?;
    let columns = address(&bank["columns"])?;
    let frames = address(&bank["frames"])?;
    let directory = address(&bank["directory"])? - ROM_BASE;
    if fw == 0 || fh == 0 || columns == 0 || width != fw * columns || height % fh != 0 {
        return Err("raw sprite frame geometry differs".into());
    }
    let table = rom
        .get(directory..directory + (frames + 1) * 4)
        .ok_or("raw sprite directory outside ROM")?;
    let pointers = table
        .chunks_exact(4)
        .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()) as usize)
        .collect::<Vec<_>>();
    if pointers[frames] != 0 || pointers[0] != address(&bank["address"])? {
        return Err("raw sprite directory terminator or base differs".into());
    }
    let mut output = vec![
        0;
        width
            .checked_mul(height)
            .ok_or("raw sprite sheet overflows")?
    ];
    for frame in 0..frames {
        let start = pointers[frame]
            .checked_sub(ROM_BASE)
            .ok_or("raw sprite pointer precedes ROM")?;
        let end = if frame + 1 == frames {
            directory
        } else {
            pointers[frame + 1]
                .checked_sub(ROM_BASE)
                .ok_or("raw sprite pointer precedes ROM")?
        };
        let bytes = rom.get(start..end).ok_or("raw sprite frame outside bank")?;
        let terminator = bytes
            .iter()
            .position(|b| *b == 0)
            .ok_or("raw sprite frame lacks terminator")?;
        let decoded = zero_skip(
            &bytes[..=terminator],
            fw.checked_mul(fh).ok_or("raw frame overflows")?,
        )?;
        if bytes[terminator + 1..].iter().any(|b| *b != 0) {
            return Err("raw sprite padding differs".into());
        }
        for row in 0..fh {
            let dst = (frame / columns * fh + row) * width + frame % columns * fw;
            output
                .get_mut(dst..dst + fw)
                .ok_or("raw frame outside sheet")?
                .copy_from_slice(&decoded[row * fw..(row + 1) * fw]);
        }
    }
    if sha256::hex(&output) != json_string(&input["decoded_sha256"], "raw sprite digest")? {
        return Err("raw sprite pixels differ".into());
    }
    Ok(output)
}

pub(super) fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let mut common = json(&root.join(COMMON))?;
    let mut index = json(&root.join(INDEX))?;
    let mut ctx = Context::new(root);
    let atlas = ctx.indexed(&root.join(SHEET))?;
    let width = atlas.width as usize;
    let mut height = atlas.height as usize;
    let mut sheet = atlas.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
    let mut additions = vec![];
    let mut removed = BTreeSet::new();
    let mut banks = vec![];
    let mut sections = BTreeMap::new();
    for input in index["private_inputs"]
        .as_array()
        .ok_or("private sprite inputs missing")?
        .iter()
        .filter(|input| input["source"] == SHEET)
    {
        sections.insert(
            (
                address(&input["width"])?,
                address(&input["height"])?,
                json_string(&input["decoded_sha256"], "sprite hash")?.to_string(),
            ),
            input["source_rect"].clone(),
        );
    }
    let original_sections = sections.len();
    for region in common["regions"]
        .as_object_mut()
        .ok_or("raw sprite regions missing")?
        .values_mut()
    {
        for component in region["components"]
            .as_array_mut()
            .ok_or("raw sprite components missing")?
        {
            if component["kind"] != "zero-skip-bank" {
                continue;
            }
            let mut bank = component.clone();
            let old = json_string(&bank["source"], "raw sprite source")?.to_string();
            let image = ctx.indexed(&root.join(&old))?;
            check_shared_palette(root, &bank, &image)?;
            let (encoded, _, _) = build_entry(&mut ctx, &bank)?;
            let start = address(&bank["address"])? - ROM_BASE;
            if rom.get(start..start + encoded.len()) != Some(encoded.as_slice()) {
                return Err("raw sprite bank differs from ROM".into());
            }
            let pixels = image.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
            if image.width as usize > width {
                return Err("raw sprite exceeds common atlas width".into());
            }
            let digest = sha256::hex(&pixels);
            let section_key = (image.width as usize, image.height as usize, digest);
            let rect = if let Some(rect) = sections.get(&section_key) {
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
                sections.insert(section_key, rect.clone());
                rect
            };
            let key = format!("{:08x}", address(&bank["directory"])?);
            bank["source"] = json!(SHEET);
            bank["image"] = json!("CHAR_COMMON.PNG");
            bank["source_rect"] = rect.clone();
            bank["pixel_format"] = json!("indices");
            bank["identity"] = Value::Null;
            bank["identity_status"] = json!("runtime name association not established");
            let input = json!({"kind":"sprite-atlas","source":SHEET,"metadata":COMMON,"pointer":format!("/banks/{key}"),"width":image.width,"height":image.height,"source_rect":rect,"decoded_sha256":sha256::hex(&pixels),"encoded_sha256":sha256::hex(&encoded),"region_address":bank["address"]});
            self::pixels(&input, &bank, &rom)?;
            additions.push(input);
            *component = json!({"kind":"components","address":bank["address"],"size":bank["size"],"source":COMMON,"image":"CHAR_COMMON.PNG","pointer":format!("/banks/{key}")});
            banks.push((key, bank));
            removed.insert(old);
        }
    }
    for (key, bank) in banks {
        common["banks"][key] = bank;
    }
    wrap(&mut common)?;
    let count = additions.len();
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(additions);
    for input in index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .iter_mut()
        .filter(|input| input["source"] == SHEET)
    {
        input["atlas_width"] = json!(width);
        input["atlas_height"] = json!(height);
    }
    index["character_catalog"]["banks"] =
        json!(address(&index["character_catalog"]["banks"])? + count);
    index["character_catalog"]["common_banks"] =
        json!(address(&index["character_catalog"]["common_banks"])? + count);
    index["character_catalog"]["common_unique_sheets"] = json!(sections.len());
    for name in [COMMON, INDEX, SHEET] {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    document(stage, COMMON, &common)?;
    document(stage, INDEX, &index)?;
    write(
        stage,
        RECIPES,
        &fs::read(root.join(RECIPES)).map_err(|e| e.to_string())?,
    )?;
    write(stage, SHEET, &image(&sheet, width as u32, height as u32)?)?;
    write(
        stage,
        COLORS,
        &fs::read(root.join(COLORS)).map_err(|e| e.to_string())?,
    )?;
    let mut staged = Context::new(stage);
    for input in index["private_inputs"]
        .as_array()
        .unwrap()
        .iter()
        .filter(|input| input["source"] == SHEET)
    {
        check(&mut staged, input)?;
    }
    for name in &removed {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    document(root, COMMON, &common)?;
    document(root, INDEX, &index)?;
    write(
        root,
        SHEET,
        &fs::read(stage.join(SHEET)).map_err(|e| e.to_string())?,
    )?;
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!("raw_sprite_banks={count} new_sections={} common_atlas={width}x{height} stored_banks=identical", sections.len() - original_sections);
    Ok(())
}
