use super::*;
mod character;
mod data;
mod palette;
pub(super) use palette::migrate as migrate_palettes;
pub(super) use palette::register as register_palettes;
mod graphics;
pub(super) use data::migrate as migrate_data;
mod portrait;
mod still;
mod tile;
pub(super) use portrait::migrate as migrate_portraits;
pub(super) use still::migrate as migrate_stills;
pub(super) use tile::migrate as migrate_tiles;
mod identity;
pub(super) use graphics::migrate as migrate_graphics;
mod tracking;
pub(super) use character::migrate as migrate_characters;
pub(super) use identity::audit as audit_characters;
use serde_json::json;
use std::collections::BTreeSet;
pub(super) use tracking::check as check_tracking;

const INDEX: &str = "games/tbs/SOURCE.json";
const COLORS: &str = "games/tbs/SRC/GRAPHICS/COMMON/PALETTE.json";
const RECIPES: &str = "games/tbs/SRC/GRAPHICS/COMMON/COMPRESSION.json";

fn write(root: &Path, name: &str, bytes: &[u8]) -> Result<(), String> {
    let path = root_path(root, name)?;
    fs::create_dir_all(path.parent().ok_or("source has no parent")?).map_err(|e| e.to_string())?;
    fs::write(path, bytes).map_err(|e| e.to_string())
}
fn document(root: &Path, name: &str, value: &Value) -> Result<(), String> {
    write(
        root,
        name,
        format!("{}\n", canonical_json(value)).as_bytes(),
    )
}
fn moved(name: &str) -> String {
    name.replace("/FIELD/SHARED/", "/FIELD/COMMON/")
}
fn address(value: &Value) -> Result<usize, String> {
    number(value, "resource address")
}
fn resource_pointer(directory: &Value, mut slot: usize) -> Result<Option<usize>, String> {
    let mut seen = BTreeSet::new();
    loop {
        if !seen.insert(slot) {
            return Err("resource alias cycle".into());
        }
        let value = directory["slots"]
            .get(slot)
            .ok_or("missing directory slot")?;
        let Some(text) = value.as_str() else {
            return Ok(None);
        };
        if let Some(alias) = text.strip_prefix("alias:") {
            slot = address(&json!(alias))?;
            continue;
        }
        if matches!(text, "base" | "self" | "null") {
            return Ok(None);
        }
        return address(value).map(Some);
    }
}
fn section(root: &Path, entry: &Value) -> Result<Value, String> {
    let plan = json(&root_path(root, json_string(&entry["plan"], "plan")?)?)?;
    match entry.get("plan_section").and_then(Value::as_str) {
        Some(key) => (if key.starts_with('/') {
            plan.pointer(key)
        } else {
            plan.get(key)
        })
        .cloned()
        .ok_or_else(|| format!("missing compression section {key}")),
        None => Ok(plan),
    }
}
fn recipe(recipes: &mut BTreeMap<String, Value>, plan: Value) -> String {
    let key = sha256::hex(canonical_json(&plan).as_bytes());
    recipes.entry(key.clone()).or_insert(plan);
    key
}
fn resource_users(root: &Path) -> Result<(BTreeMap<usize, BTreeSet<String>>, Value), String> {
    let paths = json(&root.join(SOURCE_PATHS_MANIFEST))?;
    let mut overlays: BTreeMap<usize, BTreeSet<String>> = BTreeMap::new();
    for (id, value) in paths["owners"].as_object().ok_or("missing source owners")? {
        let Some((resource, _)) = id.split_once(':') else {
            continue;
        };
        let Some(resource) = resource.strip_prefix("resource_") else {
            continue;
        };
        let Some(source) = value
            .get("source")
            .and_then(Value::as_str)
            .filter(|s| s.starts_with("FIELD/"))
        else {
            continue;
        };
        let resource = usize::from_str_radix(resource.trim_start_matches("resource_"), 16)
            .map_err(|e| e.to_string())?;
        let parent = source.rsplit_once('/').ok_or("source has no parent")?.0;
        overlays
            .entry(resource)
            .or_default()
            .insert(moved(&format!("games/tbs/SRC/{parent}")));
    }
    let table = json(&root.join("games/tbs/assets/data/battle_effect_tail.json"))?;
    let directory = json(&root.join("games/tbs/assets/data/resource_directory.json"))?;
    let loads = json(&root.join("games/tbs/assets/maps/map_load_table.json"))?;
    let scenes = table["segments"]
        .as_array()
        .ok_or("missing scene segments")?
        .iter()
        .find(|s| s["address"] == "0x0809f1a8")
        .ok_or("missing scene selectors")?;
    let mut users: BTreeMap<usize, BTreeSet<String>> = BTreeMap::new();
    let mut bindings = vec![];
    for (index, scene) in scenes["records"]
        .as_array()
        .ok_or("missing scene records")?
        .iter()
        .take(201)
        .enumerate()
    {
        let overlay = address(&scene["resource_id"])?;
        let owners = overlays
            .get(&overlay)
            .cloned()
            .unwrap_or_else(|| BTreeSet::from(["games/tbs/SRC/GRAPHICS/COMMON".into()]));
        let load = loads["records"]
            .as_array()
            .ok_or("missing map loads")?
            .iter()
            .find(|l| l["map_index"] == scene["effect_id"])
            .ok_or("scene has no loader")?;
        for field in loads["fields"].as_array().ok_or("missing loader fields")? {
            let field = json_string(field, "loader field")?;
            let slot = usize::from_str_radix(
                json_string(&load[field], "resource slot")?.trim_start_matches("0x"),
                16,
            )
            .map_err(|e| e.to_string())?;
            let Some(target) = resource_pointer(&directory, slot)? else {
                continue;
            };
            users
                .entry(target)
                .or_default()
                .extend(owners.iter().cloned());
        }
        bindings.push(json!({"scene_index":index,"overlay":format!("{overlay:x}"),"loader":load}));
    }
    Ok((users, Value::Array(bindings)))
}
fn owner(users: Option<&BTreeSet<String>>, fallback: &str, graphics: bool) -> String {
    if let Some(users) = users.filter(|u| u.len() == 1) {
        let path = users.first().unwrap();
        if path.starts_with("games/tbs/SRC/FIELD/") {
            return path.clone();
        }
    }
    if !graphics && fallback.contains("/FIELD/SHARED/") {
        return moved(fallback);
    }
    if !graphics && !fallback.contains("/GRAPHICS/") {
        return moved(fallback);
    }
    if graphics {
        "games/tbs/SRC/GRAPHICS/COMMON".into()
    } else {
        "games/tbs/SRC/FIELD/COMMON".into()
    }
}
fn stem(owner: &str, graphics: bool) -> String {
    if owner.ends_with("/COMMON") {
        return if graphics { "CHR" } else { "MAP" }.into();
    }
    let name = owner.rsplit('/').next().unwrap();
    if graphics {
        format!("{name}_CHR")
    } else {
        name.into()
    }
}

pub fn migrate(root: &Path, output: &Path) -> Result<(), String> {
    let output = if output.is_absolute() {
        output.to_path_buf()
    } else {
        root.join(output)
    };
    if output.exists() {
        return Err("migration output must be new".into());
    }
    let mut manifest = json(&root.join("games/tbs/assets/manifest.json"))?;
    let mut ctx = Context::new(root);
    let mut maps = vec![];
    let mut graphics = vec![];
    let mut retained = vec![];
    for series in manifest["series"]
        .as_array()
        .ok_or("missing asset series")?
    {
        let kind = json_string(&series["kind"], "series kind")?;
        if kind == "golden-sun-map-component-series" {
            maps.push(series.clone());
        } else if matches!(
            kind,
            "golden-sun-map-charblock-series"
                | "golden-sun-standalone-palette-series"
                | "golden-sun-standalone-tile-series"
        ) {
            graphics.push(series.clone());
        } else {
            retained.push(series.clone());
        }
    }
    if maps.is_empty() {
        return Err("no legacy map sources to migrate".into());
    }
    let mut gfx = vec![];
    expand_series(&mut ctx, &json!({"series":graphics}), &mut gfx)
        .map_err(|e| format!("graphics expansion: {e}"))?;
    let (users, scene_bindings) =
        resource_users(root).map_err(|e| format!("resource users: {e}"))?;
    let directory = json(&root.join("games/tbs/assets/data/resource_directory.json"))?;
    let mut recipes = BTreeMap::new();
    let mut regions = vec![];
    let mut removed = BTreeSet::new();
    let mut layouts = vec![];
    let mut map_files: BTreeMap<String, (Value, Vec<u8>)> = BTreeMap::new();
    let mut private = vec![];
    for series in &maps {
        let prefix = json_string(&series["source_prefix"], "map prefix")?;
        for family in series["families"]
            .as_array()
            .ok_or("missing map families")?
        {
            let id = json_string(&family[0], "map container")?;
            let old = format!("{prefix}{id}.json");
            let mut data = json(&root.join(&old))?;
            let group = owner(
                users.get(&address(&family[1])?),
                Path::new(&old).parent().unwrap().to_str().unwrap(),
                false,
            );
            let name = stem(&group, false);
            let source = format!("{group}/{name}.json");
            let packed = format!("{group}/{name}.bin");
            let pointer = format!("/maps/{id}");
            let (metadata, bytes) = map_files
                .entry(source.clone())
                .or_insert_with(|| (json!({"format":1,"maps":{}}), vec![]));
            let grid_offset = bytes.len();
            let old_grid = format!("{prefix}{id}_grid_content.png");
            let grid = indexed_png(&fs::read(root.join(&old_grid)).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?
                .pixels
                .into_iter()
                .map(|p| u8::try_from(p).map_err(|_| "grid index exceeds u8".to_string()))
                .collect::<Result<Vec<_>, _>>()?;
            if grid.len() != 65536 {
                return Err(format!("invalid grid extent for {old}"));
            }
            bytes.extend(&grid);
            let metatile_offset = bytes.len();
            let words = import_tilemap(json_string(
                &data["metatiles"]["tilemap"],
                "metatile words",
            )?)
            .map_err(|e| e.to_string())?;
            bytes.extend(&words);
            data["metatiles"]
                .as_object_mut()
                .ok_or("missing metatiles")?
                .remove("tilemap");
            data["metatiles"]["tilemap_source"] = json!(packed);
            data["metatiles"]["tilemap_offset"] = json!(metatile_offset);
            data["metatiles"]["tilemap_length"] = json!(words.len());
            data["grid_source"] = json!(packed);
            data["grid_offset"] = json!(grid_offset);
            let old_recipe = format!("{prefix}{id}_grid.lz.json");
            let key = recipe(&mut recipes, json(&root.join(&old_recipe))?);
            data["grid_compression"] = json!({"source":RECIPES,"section":key});
            removed.extend([old, old_grid, old_recipe]);
            metadata["maps"][id] = data.clone();
            layouts.push(json!({"container":id,"name":name,"owner":group,"source":source,"source_pointer":pointer,"map":packed,"grid_offset":grid_offset,"grid_length":65536,"metatiles":packed,"metatile_offset":metatile_offset,"metatile_length":words.len(),"width":address(&data["header"]["segments"][0]["values"][2])?*8,"height":address(&data["header"]["segments"][0]["values"][3])?*8}));
            let mut expanded = vec![];
            expand_series(
                &mut ctx,
                &json!({"series":[{"kind":"golden-sun-map-component-series","source_prefix":prefix,"families":[family]}]}),
                &mut expanded,
            )?;
            for mut entry in expanded {
                let entry_address = entry["address"].clone();
                if entry.get("source").is_some() {
                    entry["source"] = json!(source);
                    entry["pointer"] = json!(format!(
                        "{pointer}{}",
                        json_string(&entry["pointer"], "map pointer")?
                    ));
                } else if entry["plan"] == format!("{prefix}{id}_grid.lz.json") {
                    entry["plan"] = json!(RECIPES);
                    entry["plan_section"] = json!(key);
                    entry["components"] = json!([{"kind":"golden-sun-map-grid","source":packed,"width":128,"height":128,"source_offset":grid_offset,"source_length":65536,"size":65536}]);
                    private.push(json!({"kind":"grid","source":packed,"source_offset":grid_offset,"region_address":entry_address,"decoded_sha256":sha256::hex(&grid)}));
                } else {
                    let section = json_string(&entry["plan_section"], "map section")?.to_string();
                    entry["plan"] = json!(source);
                    entry["plan_section"] = json!(format!("/maps/{id}/{section}"));
                    for component in entry["components"]
                        .as_array_mut()
                        .ok_or("missing map components")?
                    {
                        if component["kind"] == "gba-tilemap16" {
                            component.as_object_mut().unwrap().remove("pointer");
                            component["source"] = json!(packed);
                            component["format"] = json!("binary");
                            component["source_offset"] = json!(metatile_offset);
                            component["source_length"] = json!(words.len());
                            private.push(json!({"kind":"metatiles","source":packed,"source_offset":metatile_offset,"region_address":entry_address,"transform_mode":data["metatiles"]["transform_mode"],"decoded_sha256":sha256::hex(&words)}));
                        } else {
                            component["source"] = json!(source);
                            component["pointer"] = json!(format!(
                                "{pointer}{}",
                                json_string(&component["pointer"], "component pointer")?
                            ));
                        }
                    }
                }
                regions.push(entry);
            }
        }
    }
    for (name, (metadata, bytes)) in map_files {
        document(&output, &name, &metadata)?;
        write(
            &output,
            &format!("{}.bin", name.trim_end_matches(".json")),
            &bytes,
        )?;
    }
    let mut banks: Vec<Vec<u16>> = vec![];
    let mut bank_indices: BTreeMap<Vec<u16>, usize> = BTreeMap::new();
    let mut bindings = vec![];
    let mut tile_groups: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut tile_owners: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    let mut tile_data: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut decoded_gfx = vec![];
    for entry in gfx {
        let component = &entry["components"][0];
        let built = build_component(root, component)?;
        removed.extend(built.sources);
        removed.insert(json_string(&entry["plan"], "gfx plan")?.into());
        let hash = sha256::hex(&built.data);
        let palette = component["kind"] == "gba-palette";
        if !palette {
            if built.data.len() != 16384 {
                return Err("graphics bank is not 512 tiles".into());
            }
            tile_owners.entry(hash.clone()).or_default().extend(
                users
                    .get(&address(&entry["address"])?)
                    .cloned()
                    .unwrap_or_else(|| BTreeSet::from(["games/tbs/SRC/GRAPHICS/COMMON".into()])),
            );
            tile_data.entry(hash.clone()).or_insert(built.data.clone());
        }
        decoded_gfx.push((entry, built.data, hash, palette));
    }
    let mut tile_sections = BTreeMap::new();
    for (hash, bytes) in tile_data {
        let group = owner(tile_owners.get(&hash), "", true);
        let source = format!("{group}/{}.png", stem(&group, true));
        let data = tile_groups.entry(source.clone()).or_default();
        let offset = data.len() / 32;
        data.extend(bytes);
        tile_sections.insert(hash, (source, offset));
    }
    let gray: Vec<u8> = (0u16..16)
        .flat_map(|i| (i | i << 5 | i << 10).to_le_bytes())
        .collect();
    for (source, bytes) in &tile_groups {
        let png = psynergy::assets::image::png_from_gba_tiles(bytes, &gray, GbaBpp::Bpp4, 32)
            .map_err(|e| e.to_string())?;
        write(&output, source, &png)?;
    }
    for (mut entry, data, hash, palette) in decoded_gfx {
        let key = recipe(&mut recipes, section(root, &entry)?);
        entry["plan"] = json!(RECIPES);
        entry["plan_section"] = json!(key);
        let mut binding =
            json!({"address":entry["address"],"compression":RECIPES,"compression_section":key});
        if palette {
            if data.len() != 448 {
                return Err("map palette must contain fourteen banks".into());
            }
            let mut sequence = vec![];
            for bank in data.chunks_exact(32) {
                let values: Vec<u16> = bank
                    .chunks_exact(2)
                    .map(|v| u16::from_le_bytes([v[0], v[1]]))
                    .collect();
                let index = if let Some(index) = bank_indices.get(&values) {
                    *index
                } else {
                    let index = banks.len();
                    banks.push(values.clone());
                    bank_indices.insert(values, index);
                    index
                };
                sequence.push(index);
            }
            binding["sources"] = json!([COLORS]);
            binding["palette_source"] = json!(COLORS);
            binding["palette_banks"] = json!(sequence);
            entry["components"]=json!(sequence.iter().map(|bank|json!({"kind":"le-u16-array","source":COLORS,"pointer":format!("/banks/{bank}"),"size":32})).collect::<Vec<_>>());
            private.push(json!({"kind":"palette","source":COLORS,"banks":sequence,"region_address":entry["address"],"decoded_sha256":hash}));
        } else {
            let (source, offset) = &tile_sections[&hash];
            binding["sources"] = json!([source]);
            binding["tile_offset"] = json!(offset);
            binding["tile_count"] = json!(512);
            entry["components"] = json!([{"kind":"gba-4bpp-tiles","source":source,"tile_offset":offset,"tile_count":512,"size":16384}]);
            private.push(json!({"kind":"tiles","source":source,"tile_offset":offset,"region_address":entry["address"],"decoded_sha256":hash}));
        }
        let target = address(&entry["address"])?;
        for slot in 0..directory["slots"]
            .as_array()
            .ok_or("missing directory")?
            .len()
        {
            if resource_pointer(&directory, slot)? == Some(target) {
                let mut alias = binding.clone();
                alias["resource"] = json!(format!("{slot:x}"));
                bindings.push(alias);
            }
        }
        regions.push(entry);
    }
    document(
        &output,
        COLORS,
        &json!({"format":"bgr555-banks","colors_per_bank":16,"banks":banks}),
    )?;
    document(&output, RECIPES, &json!(recipes))?;
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let index = json!({"format":"camelot-style-golden-sun-native","reference_sha256":sha256::hex(&rom),"layouts":layouts,"bindings":bindings,"scenes":scene_bindings,"regions":regions,"private_inputs":private});
    validate(&index)?;
    document(&output, INDEX, &index)?;
    retained.push(json!({"kind":"golden-sun-native-source-series","index":INDEX}));
    manifest["series"] = json!(retained);
    document(&output, "games/tbs/assets/manifest.json", &manifest)?;
    document(&output, "REMOVED.json", &json!(removed))?;
    println!(
        "maps={} graphics_regions={} palettes={} sheets={} output={}",
        index["layouts"].as_array().unwrap().len(),
        decoded_gfx_count(&index),
        banks.len(),
        tile_groups.len(),
        output.display()
    );
    Ok(())
}
fn decoded_gfx_count(index: &Value) -> usize {
    index["private_inputs"]
        .as_array()
        .unwrap()
        .iter()
        .filter(|p| matches!(p["kind"].as_str(), Some("tiles" | "palette")))
        .count()
}

pub fn install(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) {
        return Err("migration stage must be under repository out".into());
    }
    let index = json(&stage.join(INDEX))?;
    validate(&index)?;
    let rom_path = root.join("roms/tbs-en.gba");
    let rom = fs::read(&rom_path).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != index["reference_sha256"] {
        return Err("migration reference checksum differs".into());
    }
    let mut ctx = Context::new(&stage);
    for entry in index["regions"].as_array().unwrap() {
        let (data, _, _) = build_entry(&mut ctx, entry)?;
        let start = address(&entry["address"])? - ROM_BASE;
        if rom.get(start..start + data.len()) != Some(data.as_slice()) {
            return Err(format!("migration region differs at {}", entry["address"]));
        }
    }
    let mut originals = BTreeMap::new();
    for input in index["private_inputs"].as_array().unwrap() {
        let source = json_string(&input["source"], "input source")?;
        originals.insert(
            source.to_string(),
            fs::read(stage.join(source)).map_err(|e| e.to_string())?,
        );
    }
    extract(&stage, &rom_path)?;
    for (source, bytes) in originals {
        if fs::read(stage.join(&source)).map_err(|e| e.to_string())? != bytes {
            return Err(format!("native extraction differs for {source}"));
        }
    }
    let baseline = json(&root.join("out/tbs-en/full/assets/manifest.json"))?;
    let native_addresses = index["regions"]
        .as_array()
        .unwrap()
        .iter()
        .map(|r| address(&r["address"]))
        .collect::<Result<BTreeSet<_>, _>>()?;
    let mut keep = BTreeSet::new();
    for entry in baseline["regions"]
        .as_array()
        .ok_or("missing production baseline")?
    {
        if native_addresses.contains(&address(&entry["address"])?) {
            continue;
        }
        keep.extend(
            entry["sources"]
                .as_array()
                .ok_or("missing baseline sources")?
                .iter()
                .filter_map(Value::as_str)
                .map(str::to_string),
        );
    }
    let removed = json(&stage.join("REMOVED.json"))?;
    let backup = stage.join("BEFORE");
    for file in removed.as_array().ok_or("missing removal inventory")? {
        let file = json_string(file, "removed source")?;
        let path = root.join(file);
        if keep.contains(file) || !path.is_file() {
            continue;
        }
        write(&backup, file, &fs::read(&path).map_err(|e| e.to_string())?)?;
    }
    if !backup.join("games/tbs/assets/manifest.json").exists() {
        write(
            &backup,
            "games/tbs/assets/manifest.json",
            &fs::read(root.join("games/tbs/assets/manifest.json")).map_err(|e| e.to_string())?,
        )?;
    }
    for file in walkdir::WalkDir::new(stage.join("games"))
        .into_iter()
        .filter_map(Result::ok)
        .filter(|f| f.file_type().is_file())
    {
        let relative = file
            .path()
            .strip_prefix(&stage)
            .map_err(|e| e.to_string())?
            .to_str()
            .ok_or("non-UTF8 source path")?;
        write(
            root,
            relative,
            &fs::read(file.path()).map_err(|e| e.to_string())?,
        )?;
    }
    for file in removed.as_array().unwrap() {
        let file = json_string(file, "removed source")?;
        if !keep.contains(file) && root.join(file).is_file() {
            fs::remove_file(root.join(file)).map_err(|e| e.to_string())?;
        }
    }
    normalize(root)?;
    compact_maps(root)?;
    println!(
        "installed=true verified_regions={} retained_external_inputs={}",
        native_addresses.len(),
        keep.len()
    );
    Ok(())
}

fn compact_maps(root: &Path) -> Result<(), String> {
    let mut index = json(&root.join(INDEX))?;
    let mut payloads = BTreeMap::new();
    let mut users: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    let mut original_files = BTreeSet::new();
    let mut keys = vec![];
    for layout in index["layouts"].as_array().unwrap() {
        let source = json_string(&layout["map"], "map source")?;
        original_files.insert(source.to_string());
        let data = fs::read(root.join(source)).map_err(|e| e.to_string())?;
        let start = address(&layout["grid_offset"])?;
        let end = address(&layout["metatile_offset"])? + address(&layout["metatile_length"])?;
        let payload = data
            .get(start..end)
            .ok_or("map payload outside file")?
            .to_vec();
        let key = sha256::hex(&payload);
        payloads.entry(key.clone()).or_insert(payload);
        users
            .entry(key.clone())
            .or_default()
            .insert(source.to_string());
        keys.push(key);
    }
    let mut files: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut sections = BTreeMap::new();
    for (key, payload) in payloads {
        let paths = &users[&key];
        let source = if paths.len() == 1 {
            paths.first().unwrap().clone()
        } else {
            "games/tbs/SRC/FIELD/COMMON/MAP.bin".to_string()
        };
        let data = files.entry(source.clone()).or_default();
        let offset = data.len();
        data.extend(payload);
        sections.insert(key, (source, offset));
    }
    let mut remapped = BTreeMap::new();
    let mut metadata = BTreeMap::new();
    for (layout, key) in index["layouts"]
        .as_array_mut()
        .unwrap()
        .iter_mut()
        .zip(keys)
    {
        let (source, offset) = &sections[&key];
        let old = json_string(&layout["map"], "map source")?.to_string();
        let grid_offset = address(&layout["grid_offset"])?;
        let metatile_offset = address(&layout["metatile_offset"])?;
        remapped.insert((old.clone(), grid_offset), (source.clone(), *offset));
        remapped.insert((old, metatile_offset), (source.clone(), offset + 65536));
        layout["map"] = json!(source);
        layout["metatiles"] = json!(source);
        layout["grid_offset"] = json!(offset);
        layout["metatile_offset"] = json!(offset + 65536);
        layout["payload_sha256"] = json!(key);
        let name = json_string(&layout["source"], "map metadata")?.to_string();
        if !metadata.contains_key(&name) {
            metadata.insert(name.clone(), json(&root.join(&name))?);
        }
        let map = metadata
            .get_mut(&name)
            .unwrap()
            .pointer_mut(json_string(&layout["source_pointer"], "map pointer")?)
            .ok_or("missing map metadata section")?;
        map["grid_source"] = json!(source);
        map["grid_offset"] = json!(offset);
        map["metatiles"]["tilemap_source"] = json!(source);
        map["metatiles"]["tilemap_offset"] = json!(offset + 65536);
    }
    for region in index["regions"].as_array_mut().unwrap() {
        for component in region
            .get_mut("components")
            .and_then(Value::as_array_mut)
            .into_iter()
            .flatten()
        {
            if !matches!(
                component["kind"].as_str(),
                Some("golden-sun-map-grid" | "gba-tilemap16")
            ) {
                continue;
            }
            let key = (
                json_string(&component["source"], "map component")?.to_string(),
                address(&component["source_offset"])?,
            );
            let (source, offset) = remapped
                .get(&key)
                .ok_or("unresolved packed map component")?;
            component["source"] = json!(source);
            component["source_offset"] = json!(offset);
        }
    }
    for input in index["private_inputs"].as_array_mut().unwrap() {
        if !matches!(input["kind"].as_str(), Some("grid" | "metatiles")) {
            continue;
        }
        let key = (
            json_string(&input["source"], "map input")?.to_string(),
            address(&input["source_offset"])?,
        );
        let (source, offset) = remapped.get(&key).ok_or("unresolved map input")?;
        input["source"] = json!(source);
        input["source_offset"] = json!(offset);
    }
    validate(&index)?;
    for (source, data) in &files {
        write(root, source, data)?;
    }
    for (source, data) in metadata {
        document(root, &source, &data)?;
    }
    document(root, INDEX, &index)?;
    for source in original_files {
        if !files.contains_key(&source) {
            fs::remove_file(root.join(source)).map_err(|e| e.to_string())?;
        }
    }
    println!("unique_map_payloads={}", sections.len());
    Ok(())
}

fn normalize(root: &Path) -> Result<(), String> {
    for (from, to) in [
        ("games/tbs/SRC/FIELD/SHARED", "games/tbs/SRC/FIELD/COMMON"),
        ("games/tbs/GRAPHICS", "games/tbs/SRC/GRAPHICS"),
    ] {
        let files = walkdir::WalkDir::new(root.join(from))
            .into_iter()
            .filter_map(Result::ok)
            .filter(|f| f.file_type().is_file())
            .map(|f| f.into_path())
            .collect::<Vec<_>>();
        for source in &files {
            let destination = root.join(to).join(
                source
                    .strip_prefix(root.join(from))
                    .map_err(|e| e.to_string())?,
            );
            if destination.exists()
                && fs::read(source).map_err(|e| e.to_string())?
                    != fs::read(&destination).map_err(|e| e.to_string())?
            {
                return Err(format!(
                    "source move would overwrite {}",
                    destination.display()
                ));
            }
        }
        for source in files {
            let destination = format!(
                "{to}/{}",
                source
                    .strip_prefix(root.join(from))
                    .map_err(|e| e.to_string())?
                    .to_string_lossy()
            );
            write(
                root,
                &destination,
                &fs::read(&source).map_err(|e| e.to_string())?,
            )?;
            fs::remove_file(source).map_err(|e| e.to_string())?;
        }
    }
    let mut callers = vec![
        root.join(SOURCE_PATHS_MANIFEST),
        root.join("games/tbs/recon/translation-units.json"),
        root.join("games/tbs/recon/source-bindings.json"),
        root.join("games/tbs/assets/manifest.json"),
    ];
    for directory in ["games/tbs/SRC", "games/tbs/INCLUDE", "games/tbs/assets"] {
        callers.extend(
            walkdir::WalkDir::new(root.join(directory))
                .into_iter()
                .filter_map(Result::ok)
                .filter(|f| f.file_type().is_file())
                .map(|f| f.into_path())
                .filter(|p| {
                    p.extension().is_some_and(|e| {
                        ["json", "C", "c", "H", "h", "tsv"].iter().any(|v| e == *v)
                    })
                }),
        );
    }
    for path in callers {
        let original = fs::read_to_string(&path).map_err(|e| e.to_string())?;
        let updated = original
            .replace("FIELD/SHARED/", "FIELD/COMMON/")
            .replace("games/tbs/GRAPHICS/", "games/tbs/SRC/GRAPHICS/");
        if original != updated {
            fs::write(&path, updated).map_err(|e| e.to_string())?;
        }
    }
    let paths = json(&root.join(SOURCE_PATHS_MANIFEST))?;
    let mut overlays: BTreeMap<String, BTreeSet<String>> = BTreeMap::new();
    for (id, value) in paths["owners"].as_object().unwrap() {
        let Some((id, _)) = id.split_once(':') else {
            continue;
        };
        if !id.starts_with("resource_") {
            continue;
        }
        let Some(source) = value.get("source").and_then(Value::as_str) else {
            continue;
        };
        overlays.entry(id.into()).or_default().insert(format!(
            "SRC/{}",
            source
                .rsplit_once('/')
                .ok_or("owner source has no parent")?
                .0
        ));
    }
    let locations = root.join("games/tbs/locations.tsv");
    let original = fs::read_to_string(&locations).map_err(|e| e.to_string())?;
    let mut updated = String::new();
    for line in original.lines() {
        let separator = if line.contains('\t') { "\t" } else { "\\t" };
        let mut fields = line
            .split(separator)
            .map(str::to_string)
            .collect::<Vec<_>>();
        if fields.len() == 7 {
            if let Some(dirs) = overlays.get(&fields[0]).filter(|d| d.len() == 1) {
                fields[6] = dirs.first().unwrap().clone();
            }
            fields[6] = fields[6].replace("FIELD/SHARED/", "FIELD/COMMON/");
        }
        updated.push_str(&fields.join(separator));
        updated.push('\n');
    }
    fs::write(locations, updated).map_err(|e| e.to_string())?;
    let mut dirs = walkdir::WalkDir::new(root.join("games/tbs"))
        .into_iter()
        .filter_map(Result::ok)
        .filter(|f| f.file_type().is_dir())
        .map(|f| f.into_path())
        .collect::<Vec<_>>();
    dirs.sort_by_key(|p| std::cmp::Reverse(p.components().count()));
    for dir in dirs {
        if fs::read_dir(&dir)
            .map_err(|e| e.to_string())?
            .next()
            .is_none()
        {
            fs::remove_dir(dir).map_err(|e| e.to_string())?;
        }
    }
    Ok(())
}

pub fn validate(index: &Value) -> Result<(), String> {
    if index["format"] != "camelot-style-golden-sun-native" {
        return Err("unsupported native source format".into());
    }
    let mut extents: BTreeMap<String, Vec<(usize, usize, Option<String>)>> = BTreeMap::new();
    for layout in index["layouts"].as_array().ok_or("missing layouts")? {
        let source = json_string(&layout["map"], "map source")?.to_string();
        let start = address(&layout["grid_offset"])?;
        if address(&layout["grid_length"])? != 65536 {
            return Err("invalid native grid length".into());
        }
        let middle = start.checked_add(65536).ok_or("grid extent overflows")?;
        if middle != address(&layout["metatile_offset"])? {
            return Err("grid and metatiles must be adjacent".into());
        }
        let length = address(&layout["metatile_length"])?;
        if length == 0 || length % 8 != 0 {
            return Err("native metatiles need complete 2x2 definitions".into());
        }
        let end = middle
            .checked_add(length)
            .ok_or("metatile extent overflows")?;
        let spans = extents.entry(source).or_default();
        let digest = layout
            .get("payload_sha256")
            .and_then(Value::as_str)
            .map(str::to_string);
        if spans.iter().any(|(a, b, existing)| {
            start < *b
                && *a < end
                && !(start == *a && end == *b && digest.is_some() && &digest == existing)
        }) {
            return Err("native map sections overlap".into());
        }
        spans.push((start, end, digest));
    }
    let mut addresses = BTreeSet::new();
    for region in index["regions"]
        .as_array()
        .ok_or("missing native regions")?
    {
        if !addresses.insert(address(&region["address"])?) {
            return Err("duplicate native region address".into());
        }
    }
    Ok(())
}

pub fn extract(root: &Path, rom_path: &Path) -> Result<(), String> {
    let index = json(&root.join(INDEX))?;
    validate(&index)?;
    let rom = fs::read(rom_path).map_err(|e| e.to_string())?;
    if sha256::hex(&rom) != json_string(&index["reference_sha256"], "reference checksum")? {
        return Err("ROM checksum differs from native source reference".into());
    }
    let regions = index["regions"].as_array().unwrap();
    let ctx = Context::new(root);
    character::extract_all(root, &index["private_inputs"], &rom)?;
    let mut maps: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut tiles: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut banks: BTreeMap<usize, Vec<u16>> = BTreeMap::new();
    let mut tables = serde_json::Map::new();
    for input in index["private_inputs"]
        .as_array()
        .ok_or("missing private inputs")?
    {
        let source = json_string(&input["source"], "private source")?;
        let kind = json_string(&input["kind"], "private kind")?;
        if matches!(
            kind,
            "sprite"
                | "sprite-atlas"
                | "archive-atlas"
                | "still-atlas"
                | "tile-atlas"
                | "portrait-atlas"
        ) {
            continue;
        }
        let target = address(&input["region_address"])?;
        if kind == "palette-table" {
            let start = target
                .checked_sub(ROM_BASE)
                .ok_or("palette table precedes ROM")?;
            let length = address(&input["decoded_length"])?;
            let data = rom
                .get(start..start.checked_add(length).ok_or("palette table overflows")?)
                .ok_or("palette table exceeds ROM")?;
            if length % 2 != 0
                || sha256::hex(data)
                    != json_string(&input["decoded_sha256"], "palette table digest")?
            {
                return Err("palette table differs".into());
            }
            let pointer = json_string(&input["pointer"], "palette table pointer")?;
            let key = pointer
                .strip_prefix("/tables/")
                .ok_or("palette table pointer differs")?;
            tables.insert(
                key.into(),
                json!(data
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>()),
            );
            continue;
        }
        if kind == "palette-buffer" {
            let start = target.checked_sub(ROM_BASE).ok_or("palette precedes ROM")?;
            let length = address(&input["decoded_length"])?;
            let data = if input["codec"] == "raw" {
                rom.get(
                    start
                        ..start
                            .checked_add(length)
                            .ok_or("palette extent overflows")?,
                )
                .ok_or("palette exceeds ROM")?
                .to_vec()
            } else if input["codec"] == "general-lz" {
                psynergy::assets::lz::decode_general(&rom, start, rom.len(), length as u64)
                    .map_err(|e| e.to_string())?
                    .0
            } else if input["codec"] == "palette-lz" {
                psynergy::assets::lz::decode_palette(&rom, start, rom.len(), length as u64)
                    .map_err(|e| e.to_string())?
                    .0
            } else {
                return Err("unrecognized palette buffer codec".into());
            };
            if sha256::hex(&data) != json_string(&input["decoded_sha256"], "palette buffer digest")?
            {
                return Err("palette buffer differs".into());
            }
            let slots = input["banks"].as_array().ok_or("palette slots absent")?;
            if data.len() != slots.len() * 32 {
                return Err("palette buffer dimensions differ".into());
            }
            for (slot, bytes) in slots.iter().zip(data.chunks_exact(32)) {
                let values = bytes
                    .chunks_exact(2)
                    .map(|p| u16::from_le_bytes([p[0], p[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|b| *b != values) {
                    return Err("shared palette buffer differs".into());
                }
                banks.insert(slot, values);
            }
            continue;
        }
        let region = regions
            .iter()
            .find(|r| address(&r["address"]).ok() == Some(target))
            .ok_or("missing input region")?;
        if kind == "palette-raw" {
            let start = target.checked_sub(ROM_BASE).ok_or("input precedes ROM")?;
            let end = start
                .checked_add(address(&region["size"])?)
                .ok_or("palette extent overflows")?;
            let data = rom.get(start..end).ok_or("palette outside ROM")?;
            if sha256::hex(data) != json_string(&input["decoded_sha256"], "palette digest")? {
                return Err("raw palette differs".into());
            }
            for (slot, bank) in input["banks"]
                .as_array()
                .ok_or("missing palette banks")?
                .iter()
                .zip(data.chunks_exact(32))
            {
                let values = bank
                    .chunks_exact(2)
                    .map(|b| u16::from_le_bytes([b[0], b[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|existing| *existing != values) {
                    return Err("shared raw palette bank differs".into());
                }
                banks.insert(slot, values);
            }
            continue;
        }
        let plan_path = root_path(root, json_string(&region["plan"], "input plan")?)?;
        let plan_document = ctx.document(&plan_path)?;
        let plan = select_plan(&plan_document, region)?;
        let start = target.checked_sub(ROM_BASE).ok_or("input precedes ROM")?;
        let end = start
            .checked_add(address(&region["size"])?)
            .ok_or("input extent overflows")?;
        let bytes = rom.get(start..end).ok_or("input outside ROM")?;
        let maximum = address(&plan["decoded_size"])? as u64;
        let mut decoded = match json_string(&plan["codec"], "codec")? {
            "golden-sun-general-lz" => {
                psynergy::assets::lz::decode_general(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-tagged-palette-lz" => {
                psynergy::assets::lz::decode_palette(bytes, 1, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-palette-lz" => {
                psynergy::assets::lz::decode_palette(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            "golden-sun-kind2-lz" => {
                psynergy::assets::lz::decode_mtf4_lz(bytes, 0, bytes.len(), maximum).map(|v| v.0)
            }
            codec => return Err(format!("unsupported private input codec {codec}")),
        }
        .map_err(|e| e.to_string())?;
        if kind == "metatiles" {
            decoded = decode_metatiles(&decoded, address(&input["transform_mode"])? as u8)?;
        }
        if sha256::hex(&decoded) != json_string(&input["decoded_sha256"], "input digest")? {
            return Err(format!("decoded input differs at {target:#x}"));
        }
        if kind == "palette" {
            for (slot, bank) in input["banks"]
                .as_array()
                .ok_or("missing palette sequence")?
                .iter()
                .zip(decoded.chunks_exact(32))
            {
                let values = bank
                    .chunks_exact(2)
                    .map(|b| u16::from_le_bytes([b[0], b[1]]))
                    .collect::<Vec<_>>();
                let slot = address(slot)?;
                if banks.get(&slot).is_some_and(|existing| *existing != values) {
                    return Err("shared palette bank differs".into());
                }
                banks.insert(slot, values);
            }
        } else {
            let (files, offset) = if kind == "tiles" {
                (&mut tiles, address(&input["tile_offset"])? * 32)
            } else {
                (&mut maps, address(&input["source_offset"])?)
            };
            let data = files.entry(source.into()).or_default();
            let end = offset
                .checked_add(decoded.len())
                .ok_or("decoded extent overflows")?;
            if data.len() < end {
                data.resize(end, 0);
            }
            data[offset..end].copy_from_slice(&decoded);
        }
    }
    let gray: Vec<u8> = (0u16..16)
        .flat_map(|i| (i | i << 5 | i << 10).to_le_bytes())
        .collect();
    for (source, data) in maps {
        write(root, &source, &data)?;
    }
    for (source, data) in tiles {
        write(
            root,
            &source,
            &psynergy::assets::image::png_from_gba_tiles(&data, &gray, GbaBpp::Bpp4, 32)
                .map_err(|e| e.to_string())?,
        )?;
    }
    let mut colors = vec![];
    for (slot, bank) in banks {
        if slot != colors.len() {
            return Err("palette bank indices have holes".into());
        }
        colors.push(bank);
    }
    document(
        root,
        COLORS,
        &json!({"format":"bgr555-banks","colors_per_bank":16,"banks":colors,"tables":tables}),
    )?;
    println!(
        "extracted={} checksum=verified",
        index["private_inputs"].as_array().unwrap().len()
    );
    Ok(())
}
fn decode_metatiles(data: &[u8], mode: u8) -> Result<Vec<u8>, String> {
    if data.first() != Some(&mode) || data.len() % 2 != 1 {
        return Err("invalid metatile transform header".into());
    }
    let mut output = vec![];
    let mut previous = 0u16;
    let count = (data.len() - 1) / 2;
    for i in 0..count {
        let value = match mode {
            0 | 2 => u16::from_le_bytes([data[1 + i * 2], data[2 + i * 2]]),
            1 => u16::from_be_bytes([data[1 + i], data[1 + count + i]]),
            _ => return Err("unsupported metatile transform".into()),
        };
        let value = if mode == 0 { value } else { value ^ previous };
        previous = value;
        output.extend(value.to_le_bytes());
    }
    Ok(output)
}

#[test]
fn map_sections_reject_overlap_and_incomplete_metatiles() {
    let layout = json!({"map":"map.bin","grid_offset":0,"grid_length":65536,"metatile_offset":65536,"metatile_length":8});
    let mut index =
        json!({"format":"camelot-style-golden-sun-native","layouts":[layout],"regions":[]});
    validate(&index).unwrap();
    index["layouts"][0]["metatile_length"] = json!(6);
    assert!(validate(&index).is_err());
    index["layouts"][0]["metatile_length"] = json!(8);
    let duplicate = index["layouts"][0].clone();
    index["layouts"].as_array_mut().unwrap().push(duplicate);
    assert!(validate(&index).is_err());
}
#[test]
fn extracted_metatile_transforms_preserve_palette_and_flip_bits() {
    let words = [0x21, 0xf4, 0x32, 0x18, 0, 0x80, 0xff, 0xff];
    for mode in 0..3 {
        let encoded = psynergy::assets::compression::encode_tilemap_delta(&words, mode).unwrap();
        assert_eq!(decode_metatiles(&encoded, mode).unwrap(), words);
    }
}
