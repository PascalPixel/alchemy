use super::*;
const META: &str = "games/tbs/SRC/GRAPHICS/COMMON/DATA.json";
fn convert(
    ctx: &Context,
    entry: &mut Value,
    doc: &mut Value,
    removed: &mut BTreeSet<String>,
) -> Result<bool, String> {
    let kind = entry["kind"].as_str().unwrap_or("");
    if matches!(kind, "indexed-bytes" | "raw-lz-bytes")
        && entry["source"]
            .as_str()
            .is_some_and(|s| s.starts_with("games/tbs/assets/"))
    {
        let built = build_component_cached(ctx, entry)?;
        let key = sha256::hex(&built.data);
        doc["streams"][&key] = json!(built.data);
        removed.insert(json_string(&entry["source"], "byte input")?.to_string());
        entry["kind"] = json!("u8-array");
        entry["source"] = json!(META);
        entry["pointer"] = json!(format!("/streams/{key}"));
        return Ok(true);
    }
    let mut changed = false;
    if let Some(parts) = entry.get_mut("components").and_then(Value::as_array_mut) {
        for part in parts {
            changed |= convert(ctx, part, doc, removed)?;
        }
    }
    Ok(changed)
}

#[test]
fn unrelated_components_package_is_unchanged() {
    let ctx = Context::new(Path::new("."));
    let mut entry = json!({"kind":"components","source":"package.json"});
    let before = entry.clone();
    assert!(!convert(
        &ctx,
        &mut entry,
        &mut json!({"streams":{}}),
        &mut BTreeSet::new()
    )
    .unwrap());
    assert_eq!(entry, before);
}
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("data migration needs new directory under out".into());
    }
    let manifest_name = "games/tbs/assets/manifest.json";
    let mut manifest = json(&root.join(manifest_name))?;
    let series = manifest["series"].as_array_mut().ok_or("series absent")?;
    let family = series
        .iter()
        .position(|s| s["kind"] == "golden-sun-prefill-lz-series")
        .map(|pos| series.remove(pos));
    let mut ctx = Context::new(root);
    if let Some(family) = family {
        let mut entries = vec![];
        expand_series(&mut ctx, &json!({"series":[family]}), &mut entries)?;
        manifest["regions"].as_array_mut().unwrap().extend(entries);
    }
    let mut recipes = json(&root.join(RECIPES))?;
    let mut doc = if root.join(META).exists() {
        json(&root.join(META))?
    } else {
        json!({"format":1,"kind":"decoded-byte-streams","streams":{},"regions":{}})
    };
    let mut removed = BTreeSet::new();
    let mut changed = vec![];
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let index = json(&root.join(INDEX))?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "ROM checksum")? {
        return Err("data reference differs".into());
    }
    for entry in manifest["regions"].as_array_mut().ok_or("regions absent")? {
        let before_entry = entry.clone();
        if !convert(&ctx, entry, &mut doc, &mut removed)? {
            continue;
        }
        let (before, _, _) = build_entry(&mut ctx, &before_entry)?;
        let start = address(&entry["address"])?
            .checked_sub(ROM_BASE)
            .ok_or("data precedes ROM")?;
        if rom.get(start..start + before.len()) != Some(before.as_slice()) {
            return Err("data differs from ROM".into());
        }
        if entry["kind"] == "golden-sun-general-lz" {
            let old = json_string(&entry["plan"], "data plan")?.to_string();
            let plan = section(root, entry)?;
            let key = sha256::hex(canonical_json(&plan).as_bytes());
            recipes["recipes"][&key] = plan;
            entry["plan"] = json!(RECIPES);
            entry["plan_section"] = json!(format!("/recipes/{key}"));
            removed.insert(old);
        }
        doc["regions"][hex_address(start + ROM_BASE)] =
            json!({"decoded_stream":entry["pointer"],"runtime_role":null});
        changed.push((entry.clone(), before));
    }
    if changed.is_empty() {
        return Err("no byte visualizations remain".into());
    }
    for (name, value) in [
        (META, &doc),
        (manifest_name, &manifest),
        (RECIPES, &recipes),
    ] {
        document(&stage, name, value)?;
    }
    let mut staged = Context::new(&stage);
    for (entry, before) in &changed {
        if build_entry(&mut staged, entry)?.0 != *before {
            return Err("migrated byte stream differs".into());
        }
    }
    for name in removed
        .iter()
        .map(String::as_str)
        .chain([manifest_name, RECIPES])
    {
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    for name in [META, manifest_name, RECIPES] {
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
        "byte_visualizations={} source_and_rom=identical",
        changed.len()
    );
    Ok(())
}
