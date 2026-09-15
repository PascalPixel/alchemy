use super::*;

const COMMON: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/COMMON.json";
const SHEET: &str = "games/tbs/SRC/GRAPHICS/CHARACTER/CHAR_COMMON.PNG";

pub(super) fn extract(root: &Path, inputs: &Value, rom: &[u8]) -> Result<(), String> {
    let ctx = Context::new(root);
    let mut sheets: BTreeMap<String, (usize, usize, Vec<u8>, Vec<bool>)> = BTreeMap::new();
    for input in inputs.as_array().ok_or("private input list missing")? {
        if !matches!(
            input["kind"].as_str(),
            Some(
                "sprite"
                    | "sprite-atlas"
                    | "archive-atlas"
                    | "still-atlas"
                    | "tile-atlas"
                    | "portrait-atlas"
            )
        ) {
            continue;
        }
        let decoded = if input["kind"] == "portrait-atlas" {
            super::super::portrait::pixels(input, rom)?
        } else if input["kind"] == "tile-atlas" {
            super::super::tile::pixels(input, rom)?
        } else if input["kind"] == "still-atlas" {
            super::super::still::pixels(input, rom)?
        } else if input["kind"] == "archive-atlas" {
            super::super::graphics::pixels(&ctx, input, rom)?
        } else {
            pixels(&ctx, input, rom)?
        };
        let source = json_string(&input["source"], "sprite source")?;
        let w = address(&input["width"])?;
        let h = address(&input["height"])?;
        let (x, y, width, height) = if let Some(rect) = input.get("source_rect") {
            (
                address(&rect["x"])?,
                address(&rect["y"])?,
                address(&input["atlas_width"])?,
                address(&input["atlas_height"])?,
            )
        } else {
            (0, 0, w, h)
        };
        if x.checked_add(w).is_none_or(|v| v > width) || y.checked_add(h).is_none_or(|v| v > height)
        {
            return Err("sprite atlas extent differs".into());
        }
        let length = width.checked_mul(height).ok_or("sprite atlas overflows")?;
        let sheet = sheets
            .entry(source.into())
            .or_insert_with(|| (width, height, vec![0; length], vec![false; length]));
        if sheet.0 != width || sheet.1 != height {
            return Err("sprite atlas dimensions disagree".into());
        }
        for row in 0..h {
            for col in 0..w {
                let dst = (y + row) * width + x + col;
                let pixel = decoded[row * w + col];
                if sheet.3[dst] && sheet.2[dst] != pixel {
                    return Err("shared sprite sections disagree".into());
                }
                sheet.2[dst] = pixel;
                sheet.3[dst] = true;
            }
        }
    }
    for (source, (width, height, pixels, _)) in sheets {
        write(root, &source, &image(&pixels, width as u32, height as u32)?)?;
    }
    Ok(())
}
pub(super) fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let mut index = json(&root.join(INDEX))?;
    if index.get("character_catalog").is_some() {
        let common = json(&root.join(COMMON))?;
        if common["regions"].as_object().is_some_and(|regions| {
            regions.values().any(|region| {
                region["components"]
                    .as_array()
                    .is_some_and(|items| items.iter().any(|item| item["kind"] == "zero-skip-bank"))
            })
        }) {
            return raw::migrate(root, stage);
        }
        return profile::migrate(root, stage);
    }
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "reference checksum")?
        || index.get("character_catalog").is_some()
    {
        return Err("character atlas already installed or reference differs".into());
    }
    let mut ctx = Context::new(root);
    let mut banks = vec![];
    let mut roots = vec![];
    for entry in walkdir::WalkDir::new(root.join(DIR)) {
        let entry = entry.map_err(|e| e.to_string())?;
        if !entry.file_type().is_file()
            || entry.path().extension().and_then(|s| s.to_str()) != Some("json")
        {
            continue;
        }
        let doc = json(entry.path())?;
        if doc["directory"]["address"].is_string() {
            banks.push((relative(root, entry.path()), doc));
        } else if doc["layout"] == "golden-sun-static-sprite-series" {
            roots.push((relative(root, entry.path()), doc));
        }
    }
    banks.sort_by_key(|(_, doc)| address(&doc["address"]).unwrap_or(usize::MAX));
    let mut known = BTreeMap::new();
    for input in index["private_inputs"]
        .as_array()
        .unwrap()
        .iter()
        .filter(|p| p["kind"] == "sprite")
    {
        let img = ctx.indexed(&root_path(root, input["source"].as_str().unwrap())?)?;
        let key = sha256::hex(
            &[
                img.width.to_le_bytes().to_vec(),
                img.height.to_le_bytes().to_vec(),
                img.pixels.iter().map(|p| *p as u8).collect(),
            ]
            .concat(),
        );
        known.insert(key, (input["source"].as_str().unwrap().to_string(), None));
    }
    let mut width = 0;
    let mut height = 0;
    let mut unique = BTreeMap::new();
    let mut names = BTreeMap::new();
    let mut common = json!({"format":"golden-sun-character-banks","identity_policy":"Names require a Japanese message and runtime actor binding; a descriptor or shared image alone does not establish a character identity.","banks":{},"regions":{}});
    let mut inputs = vec![];
    let mut removed = vec![];
    for (old, mut bank) in banks {
        let (encoded, _, _) = build_entry(&mut ctx, &bank)?;
        let start = address(&bank["address"])? - ROM_BASE;
        if rom.get(start..start + encoded.len()) != Some(encoded.as_slice()) {
            return Err("character bank does not match ROM".into());
        }
        let component = &bank["components"][0]["components"][0];
        let old_image = json_string(&component["source"], "legacy character image")?.to_string();
        let img = ctx.indexed(&root_path(root, &old_image)?)?;
        check_shared_palette(root, component, &img)?;
        let decoded = img.pixels.iter().map(|p| *p as u8).collect::<Vec<_>>();
        let key = sha256::hex(
            &[
                img.width.to_le_bytes().to_vec(),
                img.height.to_le_bytes().to_vec(),
                decoded.clone(),
            ]
            .concat(),
        );
        let (source, rect) = if let Some((source, rect)) = known.get(&key) {
            (source.clone(), rect.clone())
        } else {
            let rect = unique
                .entry(key)
                .or_insert_with(|| {
                    let rect = json!({"x":0,"y":height,"width":img.width,"height":img.height});
                    height += img.height as usize;
                    width = width.max(img.width as usize);
                    (rect, decoded.clone())
                })
                .0
                .clone();
            (SHEET.to_string(), Some(rect))
        };
        let key = format!("{:08x}", address(&bank["directory"]["address"])?);
        let pointer = format!("/banks/{key}");
        bank["identity"] = Value::Null;
        bank["identity_status"] = json!("runtime name association not established");
        bank["components"][0]["components"][0]["source"] = json!(source);
        bank["components"][0]["components"][0]["pixel_format"] = json!("indices");
        if let Some(rect) = &rect {
            bank["components"][0]["components"][0]["source_rect"] = rect.clone();
        }
        names.insert(old.clone(), (COMMON.into(), pointer.clone()));
        names.insert(old_image.clone(), (source.clone(), String::new()));
        names.insert(
            Path::new(&old_image)
                .file_name()
                .unwrap()
                .to_str()
                .unwrap()
                .into(),
            (
                Path::new(&source)
                    .file_name()
                    .unwrap()
                    .to_str()
                    .unwrap()
                    .into(),
                String::new(),
            ),
        );
        let mut input = json!({"kind":if rect.is_some(){"sprite-atlas"}else{"sprite"},"source":source,"metadata":COMMON,"pointer":pointer,"width":img.width,"height":img.height,"decoded_sha256":sha256::hex(&decoded),"encoded_sha256":sha256::hex(&encoded),"region_address":bank["address"]});
        if let Some(rect) = rect {
            input["source_rect"] = rect;
        }
        inputs.push(input);
        common["banks"][key] = bank;
        removed.extend([old, old_image]);
    }
    for input in &mut inputs {
        if input["kind"] == "sprite-atlas" {
            input["atlas_width"] = json!(width);
            input["atlas_height"] = json!(height);
        }
    }
    let mut sheet = vec![0; width.checked_mul(height).ok_or("sprite atlas overflows")?];
    for (rect, pixels) in unique.values() {
        let y = address(&rect["y"])?;
        let w = address(&rect["width"])?;
        let h = address(&rect["height"])?;
        for row in 0..h {
            sheet[(y + row) * width..(y + row) * width + w]
                .copy_from_slice(&pixels[row * w..(row + 1) * w]);
        }
    }
    let mut documents = BTreeMap::new();
    let mut manifest = json(&root.join("games/tbs/assets/manifest.json"))?;
    for (name, doc) in roots {
        let key = format!("{:08x}", address(&doc["address"])?);
        common["regions"][&key] = doc;
        let entries = manifest["regions"]
            .as_array_mut()
            .ok_or("asset manifest missing")?;
        let pos = entries
            .iter()
            .position(|r| r["source"] == name)
            .ok_or("character root ownership missing")?;
        let mut entry = entries.remove(pos);
        entry["source"] = json!(COMMON);
        entry["pointer"] = json!(format!("/regions/{key}/components"));
        index["regions"].as_array_mut().unwrap().push(entry);
        removed.push(name);
    }
    rewrite(&mut common, &names, &[]);
    for entry in walkdir::WalkDir::new(root.join("games/tbs")) {
        let entry = entry.map_err(|e| e.to_string())?;
        if !entry.file_type().is_file()
            || entry.path().extension().and_then(|e| e.to_str()) != Some("json")
        {
            continue;
        }
        let name = relative(root, entry.path());
        if removed.contains(&name)
            || name == INDEX
            || name == "games/tbs/assets/manifest.json"
            || name.ends_with("dossiers.json")
        {
            continue;
        }
        let text = fs::read_to_string(entry.path()).map_err(|e| e.to_string())?;
        if !names.keys().any(|key| text.contains(key)) {
            continue;
        }
        let mut doc: Value = serde_json::from_str(&text).map_err(|e| e.to_string())?;
        rewrite(&mut doc, &names, &[]);
        documents.insert(name, doc);
    }
    rewrite(&mut index, &names, &[]);
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(inputs.clone());
    index["character_catalog"] = json!({"source":COMMON,"sheet":SHEET,"banks":inputs.len()+8,"common_banks":inputs.len(),"common_unique_sheets":unique.len(),"identity_policy":"ROM messages plus runtime binding; unknown identities remain explicit"});
    documents.insert(COMMON.into(), common);
    documents.insert(INDEX.into(), index);
    documents.insert("games/tbs/assets/manifest.json".into(), manifest);
    documents.insert(COLORS.into(), json(&root.join(COLORS))?);
    for (name, value) in &documents {
        document(stage, name, value)?;
    }
    write(stage, SHEET, &image(&sheet, width as u32, height as u32)?)?;
    for input in &inputs {
        if input["kind"] == "sprite" {
            write(
                stage,
                input["source"].as_str().unwrap(),
                &fs::read(root.join(input["source"].as_str().unwrap()))
                    .map_err(|e| e.to_string())?,
            )?;
        }
    }
    let mut check_ctx = Context::new(stage);
    for input in &inputs {
        let _ = pixels(&check_ctx, input, &rom)?;
        check(&mut check_ctx, input)?;
    }
    extract(stage, &json!(inputs), &rom)?;
    let rebuilt = indexed_png(&fs::read(stage.join(SHEET)).map_err(|e| e.to_string())?)
        .map_err(|e| e.to_string())?;
    if rebuilt.pixels != sheet.iter().map(|p| u32::from(*p)).collect::<Vec<_>>() {
        return Err("regenerated common sprite atlas differs".into());
    }
    for name in documents.keys().chain(removed.iter()) {
        if root.join(name).is_file() {
            write(
                &stage.join("BEFORE"),
                name,
                &fs::read(root.join(name)).map_err(|e| e.to_string())?,
            )?;
        }
    }
    for (name, value) in documents {
        document(root, &name, &value)?;
    }
    write(
        root,
        SHEET,
        &fs::read(stage.join(SHEET)).map_err(|e| e.to_string())?,
    )?;
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!(
        "sprite_banks={} unique_common_sections={} common_sheet={}x{} regeneration=identical",
        inputs.len() + 8,
        unique.len(),
        width,
        height
    );
    Ok(())
}
