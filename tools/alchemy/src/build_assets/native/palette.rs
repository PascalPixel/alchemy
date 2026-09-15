use super::*;
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("palette migration needs new directory under out".into());
    }
    let manifest_name = "games/tbs/assets/manifest.json";
    let mut manifest = json(&root.join(manifest_name))?;
    let mut ctx = Context::new(root);
    let series = manifest["series"].as_array_mut().ok_or("series absent")?;
    if let Some(pos) = series
        .iter()
        .position(|s| s["kind"] == "golden-sun-color-table-series")
    {
        let family = series.remove(pos);
        let mut entries = vec![];
        expand_series(&mut ctx, &json!({"series":[family]}), &mut entries)?;
        manifest["regions"].as_array_mut().unwrap().extend(entries);
    }
    let mut index = json(&root.join(INDEX))?;
    let mut colors = json(&root.join(COLORS))?;
    if colors.get("tables").is_none() {
        colors["tables"] = json!({});
    }
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("palette reference differs".into());
    }
    let mut changed = vec![];
    let mut removed = BTreeSet::new();
    for entry in manifest["regions"].as_array_mut().unwrap() {
        if !matches!(
            entry["kind"].as_str(),
            Some("gba-palette-rgba" | "gba-palette")
        ) {
            continue;
        }
        let source = json_string(&entry["source"], "palette source")?.to_string();
        if !source.starts_with("games/tbs/assets/graphics/") {
            continue;
        }
        let (before, _, _) = build_entry(&mut ctx, entry)?;
        let start = address(&entry["address"])? - ROM_BASE;
        if before.len() != address(&entry["size"])?
            || rom.get(start..start + before.len()) != Some(before.as_slice())
        {
            return Err("palette table differs from ROM".into());
        }
        let key = sha256::hex(&before);
        let pointer = format!("/tables/{key}");
        colors["tables"][&key] = json!(before
            .chunks_exact(2)
            .map(|p| u16::from_le_bytes([p[0], p[1]]))
            .collect::<Vec<_>>());
        entry["kind"] = json!("le-u16-array");
        entry["source"] = json!(COLORS);
        entry["pointer"] = json!(pointer);
        index["private_inputs"].as_array_mut().unwrap().push(json!({"kind":"palette-table","source":COLORS,"pointer":pointer,"region_address":entry["address"],"decoded_length":before.len(),"decoded_sha256":key}));
        changed.push((entry.clone(), before));
        if source.ends_with(".rgba.png") || source.contains("direct_palettes_") {
            removed.insert(source);
        }
    }
    if changed.is_empty() {
        return Err("no palette images remain".into());
    }
    for (name, value) in [
        (INDEX, &index),
        (COLORS, &colors),
        (manifest_name, &manifest),
    ] {
        document(&stage, name, value)?;
    }
    let mut staged = Context::new(&stage);
    for (entry, before) in &changed {
        if build_entry(&mut staged, entry)?.0 != *before {
            return Err("migrated palette differs".into());
        }
    }
    for name in removed
        .iter()
        .map(String::as_str)
        .chain([INDEX, COLORS, manifest_name])
    {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [INDEX, COLORS, manifest_name] {
        write(
            root,
            name,
            &fs::read(stage.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!("palette_tables={} source_and_rom=identical", changed.len());
    Ok(())
}
pub fn register(root: &Path) -> Result<(), String> {
    let mut index = json(&root.join(INDEX))?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("palette reference differs".into());
    }
    let mut candidates = vec![];
    let portrait = json(&root.join("games/tbs/SRC/GRAPHICS/COMMON/PORTRAIT.json"))?;
    let mut cursor = address(&portrait["address"])?;
    for component in portrait["components"]
        .as_array()
        .ok_or("portrait components absent")?
    {
        if component["kind"] == "bgr555-banks" {
            candidates.push((
                cursor,
                address(&component["size"])?,
                "raw",
                component["banks"].clone(),
            ));
        }
        cursor += address(&component["size"])?;
    }
    let tile = json(&root.join("games/tbs/SRC/GRAPHICS/COMMON/TILE.json"))?;
    for (addr, bank) in tile["banks"].as_object().ok_or("tile banks absent")? {
        if let Some(slots) = bank.get("palette_banks") {
            candidates.push((
                address(&json!(addr))?,
                slots.as_array().ok_or("palette banks absent")?.len() * 32,
                "palette-lz",
                slots.clone(),
            ));
        }
    }
    let ctx = Context::new(root);
    for (target, length, codec, slots) in candidates {
        let expected = palette_banks(root, &json!({"source":COLORS,"banks":slots}))?;
        let start = target.checked_sub(ROM_BASE).ok_or("palette precedes ROM")?;
        let decoded = if codec == "raw" {
            rom.get(start..start + length)
                .ok_or("palette exceeds ROM")?
                .to_vec()
        } else {
            psynergy::assets::lz::decode_palette(&rom, start, rom.len(), length as u64)
                .map_err(|e| e.to_string())?
                .0
        };
        if decoded != expected {
            return Err("palette registration differs from ROM".into());
        }
        let input = json!({"kind":"palette-buffer","source":COLORS,"region_address":hex_address(target),"decoded_length":length,"decoded_sha256":sha256::hex(&decoded),"banks":slots,"codec":codec});
        if !index["private_inputs"].as_array().unwrap().contains(&input) {
            index["private_inputs"].as_array_mut().unwrap().push(input);
        }
    }
    let _ = ctx;
    write(
        root,
        "out/graphics-palettes/BEFORE/SOURCE.json",
        &fs::read(root.join(INDEX)).map_err(|e| e.to_string())?,
    )?;
    document(root, INDEX, &index)?;
    println!("palette_buffers=registered source_and_rom=identical");
    Ok(())
}
