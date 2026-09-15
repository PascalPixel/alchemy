use super::*;

pub(super) fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let mut recipes = json(&root.join(RECIPES))?;
    let mut files = BTreeMap::new();
    let mut count = 0;
    for entry in walkdir::WalkDir::new(root.join(DIR)) {
        let entry = entry.map_err(|e| e.to_string())?;
        if !entry.file_type().is_file()
            || entry.path().extension().and_then(|s| s.to_str()) != Some("json")
        {
            continue;
        }
        let mut doc = json(entry.path())?;
        for bank in doc["banks"]
            .as_object_mut()
            .ok_or("character bank owner missing")?
            .values_mut()
        {
            if !bank["streams"].is_array() {
                continue;
            }
            let streams = bank["streams"].clone();
            let key = sha256::hex(canonical_json(&streams).as_bytes());
            let pointer = format!("/recipes/{key}");
            recipes["recipes"][&key] = streams;
            bank["streams"] = json!({"source":RECIPES,"pointer":pointer});
            bank["components"][0]["plan"] = json!(RECIPES);
            bank["components"][0]["plan_section"] = json!(pointer);
            count += 1;
        }
        raw::wrap(&mut doc)?;
        files.insert(relative(root, entry.path()), doc);
    }
    files.insert(RECIPES.into(), recipes);
    let index = json(&root.join(INDEX))?;
    let mut ctx = Context::new(root);
    let mut expected = vec![];
    for input in index["private_inputs"]
        .as_array()
        .unwrap()
        .iter()
        .filter(|p| matches!(p["kind"].as_str(), Some("sprite" | "sprite-atlas")))
    {
        let old = bank(&ctx, input)?;
        let (previous, _, _) = build_entry(&mut ctx, &old)?;
        if sha256::hex(&previous)
            != input["encoded_sha256"]
                .as_str()
                .ok_or("bank hash missing")?
        {
            return Err("bank differs before recipe compaction".into());
        }
        expected.push(input.clone());
    }
    for (name, doc) in &files {
        document(stage, name, doc)?;
        write(
            &stage.join("BEFORE"),
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    write(
        stage,
        COLORS,
        &fs::read(root.join(COLORS)).map_err(|e| e.to_string())?,
    )?;
    let mut images = BTreeSet::new();
    for input in &expected {
        images.insert(json_string(&input["source"], "sprite image")?);
    }
    for name in images {
        write(
            stage,
            name,
            &fs::read(root.join(name)).map_err(|e| e.to_string())?,
        )?;
    }
    ctx = Context::new(stage);
    for input in expected {
        check(&mut ctx, &input)?;
    }
    for (name, doc) in files {
        document(root, &name, &doc)?;
    }
    println!("character_recipes={count} owner={RECIPES} stored_banks=identical");
    Ok(())
}
