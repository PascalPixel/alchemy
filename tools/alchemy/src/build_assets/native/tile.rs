use super::*;
const SHEET: &str = "games/tbs/SRC/GRAPHICS/COMMON/TILE.PNG";
const META: &str = "games/tbs/SRC/GRAPHICS/COMMON/TILE.json";
pub(super) fn pixels(input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let start = address(&input["region_address"])?
        .checked_sub(ROM_BASE)
        .ok_or("tile precedes ROM")?;
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    if width % 8 != 0 || height % 8 != 0 {
        return Err("tile dimensions differ".into());
    }
    let count = width
        .checked_mul(height)
        .ok_or("tile dimensions overflow")?;
    let (decoded, _) = psynergy::assets::lz::decode_general(rom, start, rom.len(), count as u64)
        .map_err(|e| e.to_string())?;
    let mut pixels = vec![0; count];
    let mut cursor = 0;
    for y in (0..height).step_by(8) {
        for x in (0..width).step_by(8) {
            for row in 0..8 {
                pixels[(y + row) * width + x..(y + row) * width + x + 8].copy_from_slice(
                    decoded
                        .get(cursor..cursor + 8)
                        .ok_or("tile stream truncated")?,
                );
                cursor += 8;
            }
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
    let component = &region["components"][0];
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
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("tile migration needs new directory under out".into());
    }
    let manifest_name = "games/tbs/assets/manifest.json";
    let mut manifest = json(&root.join(manifest_name))?;
    let regions = manifest["regions"].as_array_mut().ok_or("regions absent")?;
    let entries = regions
        .iter()
        .filter(|r| {
            r["plan"]
                .as_str()
                .is_some_and(|p| p.contains("resources_d8_e3_"))
        })
        .cloned()
        .collect::<Vec<_>>();
    if entries.len() != 12 {
        return Err("tile archive set differs or already migrated".into());
    }
    regions.retain(|r| !entries.contains(r));
    let mut index = json(&root.join(INDEX))?;
    let mut recipes = json(&root.join(RECIPES))?;
    let mut colors = json(&root.join(COLORS))?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("tile reference differs".into());
    }
    let mut ctx = Context::new(root);
    let mut sheet = vec![];
    let mut width = 0;
    let mut inputs = vec![];
    let mut removed = BTreeSet::new();
    let mut meta = json!({"format":1,"kind":"indexed-tile-atlas","banks":{}});
    for old_entry in entries {
        let (encoded, _, _) = build_entry(&mut ctx, &old_entry)?;
        let start = address(&old_entry["address"])? - ROM_BASE;
        if encoded.len() != address(&old_entry["size"])?
            || rom.get(start..start + encoded.len()) != Some(encoded.as_slice())
        {
            return Err("tile archive differs from ROM".into());
        }
        let old_plan = json_string(&old_entry["plan"], "tile plan")?;
        let plan = json(&root.join(old_plan))?;
        let key = sha256::hex(canonical_json(&plan).as_bytes());
        recipes["recipes"][&key] = plan;
        let mut entry = old_entry.clone();
        entry["plan"] = json!(RECIPES);
        entry["plan_section"] = json!(format!("/recipes/{key}"));
        let component = &old_entry["components"][0];
        let old_source = json_string(&component["source"], "tile input")?;
        let image = ctx.indexed(&root.join(old_source))?;
        let id = hex_address(start + ROM_BASE);
        if component["kind"] == "gba-palette" {
            let (_, palette, _) = gba_graphics(
                &fs::read(root.join(old_source)).map_err(|e| e.to_string())?,
                GbaBpp::Bpp8,
            )
            .map_err(|e| e.to_string())?;
            let mut banks = vec![];
            for bytes in palette.chunks_exact(32) {
                let bank = json!(bytes
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>());
                let values = colors["banks"]
                    .as_array_mut()
                    .ok_or("palette pool absent")?;
                let slot = values.iter().position(|v| v == &bank).unwrap_or_else(|| {
                    values.push(bank);
                    values.len() - 1
                });
                banks.push(slot);
            }
            entry["components"] = json!([{"kind":"bgr555-banks","source":COLORS,"banks":banks,"size":component["size"]}]);
            index["private_inputs"].as_array_mut().unwrap().push(json!({"kind":"palette-buffer","source":COLORS,"banks":banks,"codec":"palette-lz","region_address":entry["address"],"decoded_length":palette.len(),"decoded_sha256":sha256::hex(&palette)}));
            meta["banks"][&id] = json!({"palette_banks":banks,"runtime_role":null});
        } else if component["kind"] == "gba-8bpp-tiles" {
            let w = image.width as usize;
            let h = image.height as usize;
            if width == 0 {
                width = w;
            }
            if width != w {
                return Err("tile widths differ".into());
            }
            let pixels = image.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
            let rect = json!({"x":0,"y":sheet.len()/width,"width":w,"height":h});
            sheet.extend(&pixels);
            entry["components"][0]["source"] = json!(SHEET);
            entry["components"][0]["source_rect"] = rect.clone();
            let input = json!({"kind":"tile-atlas","source":SHEET,"source_rect":rect,"width":w,"height":h,"atlas_width":width,"region_address":entry["address"],"encoded_sha256":sha256::hex(&encoded),"decoded_sha256":sha256::hex(&pixels)});
            self::pixels(&input, &rom)?;
            inputs.push(input);
            meta["banks"][&id] = json!({"source_rect":rect,"runtime_role":null,"identity":null});
        } else {
            return Err("tile component requires investigation".into());
        }
        index["regions"].as_array_mut().unwrap().push(entry);
        removed.extend([old_plan.to_string(), old_source.to_string()]);
    }
    let height = sheet.len() / width;
    for input in &mut inputs {
        input["atlas_height"] = json!(height);
    }
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(inputs.clone());
    for (name, doc) in [
        (INDEX, &index),
        (manifest_name, &manifest),
        (RECIPES, &recipes),
        (COLORS, &colors),
        (META, &meta),
    ] {
        document(&stage, name, doc)?;
    }
    write(
        &stage,
        SHEET,
        &character::image(&sheet, width as u32, height as u32)?,
    )?;
    let mut staged = Context::new(&stage);
    for input in &inputs {
        check(&mut staged, &index, input)?;
    }
    for entry in index["regions"].as_array().unwrap().iter().filter(|r| {
        r["plan"] == RECIPES
            && r["components"][0]["kind"] == "bgr555-banks"
            && r["components"][0]["banks"].is_array()
    }) {
        build_entry(&mut staged, entry)?;
    }
    for name in removed
        .iter()
        .map(String::as_str)
        .chain([INDEX, manifest_name, RECIPES, COLORS])
    {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [INDEX, manifest_name, RECIPES, COLORS, META, SHEET] {
        write(
            root,
            name,
            &fs::read(stage.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!("tile_archives=12 atlas={width}x{height} source_and_rom=identical");
    Ok(())
}
