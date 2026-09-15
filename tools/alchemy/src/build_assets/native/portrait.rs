use super::*;
const SHEET: &str = "games/tbs/SRC/GRAPHICS/COMMON/PORTRAIT.PNG";
const META: &str = "games/tbs/SRC/GRAPHICS/COMMON/PORTRAIT.json";
const OLD: &str = "games/tbs/assets/graphics/resource_f0_archive.json";
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
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("portrait migration needs new directory under out".into());
    }
    let mut doc = json(&root.join(OLD))?;
    let mut index = json(&root.join(INDEX))?;
    let manifest_name = "games/tbs/assets/manifest.json";
    let mut manifest = json(&root.join(manifest_name))?;
    let entry = manifest["regions"]
        .as_array_mut()
        .ok_or("regions absent")?
        .iter_mut()
        .find(|r| r["source"] == OLD)
        .ok_or("portrait archive absent or already migrated")?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("portrait reference differs".into());
    }
    let mut ctx = Context::new(root);
    let (before, _, _) = build_entry(&mut ctx, entry)?;
    let start = address(&entry["address"])? - ROM_BASE;
    if rom.get(start..start + before.len()) != Some(before.as_slice()) {
        return Err("portrait archive differs from ROM".into());
    }
    let mut colors = json(&root.join(COLORS))?;
    let mut removed = BTreeSet::from([OLD.to_string()]);
    let mut sheet = vec![];
    let mut inputs = vec![];
    let mut portraits = json!({});
    let mut cursor = address(&entry["address"])?;
    for (n, component) in doc["components"]
        .as_array_mut()
        .ok_or("portrait components absent")?
        .iter_mut()
        .enumerate()
    {
        let size = address(&component["size"])?;
        if component["kind"] == "typed-table" {
            component["source"] = json!(META);
        } else if component["kind"] == "gba-palette" {
            let old = json_string(&component["source"], "portrait palette")?.to_string();
            let bytes = build_component_cached(&ctx, component)?.data;
            if bytes.len() != 32 {
                return Err("portrait palette size differs".into());
            }
            let bank = json!(bytes
                .chunks_exact(2)
                .map(|p| u16::from_le_bytes([p[0], p[1]]))
                .collect::<Vec<_>>());
            let banks = colors["banks"]
                .as_array_mut()
                .ok_or("palette pool absent")?;
            let slot = banks.iter().position(|b| b == &bank).unwrap_or_else(|| {
                banks.push(bank);
                banks.len() - 1
            });
            *component = json!({"kind":"bgr555-banks","source":COLORS,"banks":[slot],"size":32});
            index["private_inputs"].as_array_mut().unwrap().push(json!({"kind":"palette-buffer","source":COLORS,"banks":[slot],"codec":"raw","region_address":hex_address(cursor),"decoded_length":bytes.len(),"decoded_sha256":sha256::hex(&bytes)}));
            removed.insert(old);
        } else if component["kind"] == "mtf4-bytes" {
            let old = json_string(&component["source"], "portrait input")?.to_string();
            let image = ctx.indexed(&root.join(&old))?;
            if image.width != 32 || image.height != 32 {
                return Err("portrait dimensions differ".into());
            }
            let pixels = image.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
            let encoded = build_component_cached(&ctx, component)?.data;
            let number = inputs.len();
            let rect = json!({"x":0,"y":number*32,"width":32,"height":32});
            sheet.extend(&pixels);
            component["source"] = json!(SHEET);
            component["source_rect"] = rect.clone();
            let input = json!({"kind":"portrait-atlas","source":SHEET,"source_rect":rect,"width":32,"height":32,"atlas_width":32,"region_address":hex_address(cursor),"encoded_length":size,"encoded_sha256":sha256::hex(&encoded),"decoded_sha256":sha256::hex(&pixels),"pointer":format!("/components/{n}")});
            self::pixels(&input, &rom)?;
            inputs.push(input);
            portraits[number.to_string()] =
                json!({"identity":null,"source_rect":rect,"component":n});
            removed.insert(old);
        } else if component["kind"] != "byte-fill" {
            return Err("portrait component requires investigation".into());
        }
        cursor += size;
    }
    doc["portraits"] = portraits;
    entry["source"] = json!(META);
    let height = inputs.len() * 32;
    for input in &mut inputs {
        input["atlas_height"] = json!(height);
    }
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(inputs.clone());
    for (name, value) in [
        (META, &doc),
        (COLORS, &colors),
        (INDEX, &index),
        (manifest_name, &manifest),
    ] {
        document(&stage, name, value)?;
    }
    write(&stage, SHEET, &character::image(&sheet, 32, height as u32)?)?;
    let mut staged = Context::new(&stage);
    for input in &inputs {
        check(&staged, input)?;
    }
    let new_entry = manifest["regions"]
        .as_array()
        .unwrap()
        .iter()
        .find(|r| r["source"] == META)
        .unwrap();
    if build_entry(&mut staged, new_entry)?.0 != before {
        return Err("migrated portrait archive differs".into());
    }
    for name in removed
        .iter()
        .map(String::as_str)
        .chain([COLORS, INDEX, manifest_name])
    {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [META, COLORS, INDEX, manifest_name, SHEET] {
        write(
            root,
            name,
            &fs::read(stage.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!(
        "portraits={} atlas=32x{height} source_and_rom=identical",
        inputs.len()
    );
    Ok(())
}
