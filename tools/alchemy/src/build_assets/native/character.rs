use super::*;
mod atlas;
mod profile;
mod raw;

const DIR: &str = "games/tbs/SRC/GRAPHICS/CHARACTER";
const OLD_PALETTE: &str = "games/tbs/SRC/GRAPHICS/PALETTE/0800777c.gbapal.png";

fn gray(entries: usize) -> Vec<[u8; 3]> {
    (0..entries).map(|i| [i as u8; 3]).collect()
}
fn image(pixels: &[u8], width: u32, height: u32) -> Result<Vec<u8>, String> {
    if pixels.len() != width as usize * height as usize || pixels.iter().any(|p| *p >= 224) {
        return Err("character sheet dimensions or indices differ".into());
    }
    let mut data = vec![];
    {
        let mut encoder = png::Encoder::new(&mut data, width, height);
        encoder.set_color(png::ColorType::Indexed);
        encoder.set_depth(png::BitDepth::Eight);
        encoder.set_palette(gray(224).concat());
        encoder.set_trns(vec![0]);
        encoder
            .write_header()
            .map_err(|e| e.to_string())?
            .write_image_data(pixels)
            .map_err(|e| e.to_string())?;
    }
    Ok(data)
}
fn zero_skip(bytes: &[u8], length: usize) -> Result<Vec<u8>, String> {
    let mut output = vec![];
    for (i, byte) in bytes.iter().enumerate() {
        if *byte == 0 {
            if i + 1 != bytes.len() || output.len() != length {
                return Err("sprite terminator or frame extent differs".into());
            }
            return Ok(output);
        }
        let count = if *byte >= 0xe0 {
            usize::from(*byte - 0xdf)
        } else {
            1
        };
        if output
            .len()
            .checked_add(count)
            .is_none_or(|end| end > length)
        {
            return Err("sprite zero run exceeds frame".into());
        }
        if *byte >= 0xe0 {
            output.resize(output.len() + count, 0);
        } else {
            output.push(*byte);
        }
    }
    Err("sprite stream lacks terminator".into())
}
fn bank(ctx: &Context, input: &Value) -> Result<Value, String> {
    let path = root_path(
        &ctx.root,
        json_string(&input["metadata"], "character metadata")?,
    )?;
    let metadata = ctx.document(&path)?;
    metadata
        .pointer(json_string(&input["pointer"], "character bank pointer")?)
        .cloned()
        .ok_or("character bank missing".into())
}
fn component(bank: &Value) -> &Value {
    if bank["kind"] == "zero-skip-bank" {
        bank
    } else if bank["components"][0]["kind"] == "zero-skip-bank" {
        &bank["components"][0]
    } else {
        &bank["components"][0]["components"][0]
    }
}
fn pixels(ctx: &Context, input: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let bank = bank(ctx, input)?;
    if component(&bank)["kind"] == "zero-skip-bank" {
        return raw::pixels(input, component(&bank), rom);
    }
    let component = &bank["components"][0]["components"][0];
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&component["frame_width"])?;
    let fh = address(&component["frame_height"])?;
    let columns = address(&component["columns"])?;
    if fw == 0 || fh == 0 || columns == 0 || width != fw * columns || height % fh != 0 {
        return Err("character frame geometry differs".into());
    }
    let slots = bank["directory"]["slots"]
        .as_array()
        .ok_or("character directory missing")?;
    let plans = if bank["streams"].is_array() {
        bank["streams"].clone()
    } else {
        let document = ctx.document(&root_path(
            &ctx.root,
            json_string(&bank["streams"]["source"], "sprite recipes")?,
        )?)?;
        document
            .pointer(json_string(
                &bank["streams"]["pointer"],
                "sprite recipe pointer",
            )?)
            .cloned()
            .ok_or("sprite recipes missing")?
    };
    let streams = plans.as_array().ok_or("character streams missing")?;
    if slots.last().and_then(Value::as_str) != Some("null") {
        return Err("character directory terminator differs".into());
    }
    let pointers = slots[..slots.len() - 1]
        .iter()
        .map(address)
        .collect::<Result<BTreeSet<_>, _>>()?;
    if pointers.len() != streams.len() {
        return Err("character directory aliases differ from physical streams".into());
    }
    let base = address(&bank["address"])?;
    let end = base
        .checked_sub(ROM_BASE)
        .and_then(|v| v.checked_add(address(&bank["size"]).ok()?))
        .ok_or("character extent overflows")?;
    let rom = rom.get(..end).ok_or("character bank outside ROM")?;
    let mut output = vec![0; width.checked_mul(height).ok_or("sheet extent overflows")?];
    for (frame, (slot, stream)) in pointers.iter().zip(streams).enumerate() {
        let start = slot
            .checked_sub(ROM_BASE)
            .ok_or("character frame precedes ROM")?;
        let (data, size) = match stream["codec"].as_str() {
            Some("golden-sun-arena-lz") => {
                let (data, size, _) =
                    psynergy::assets::lz::decode_arena(rom, start).map_err(|e| e.to_string())?;
                (data, size)
            }
            Some("golden-sun-general-lz") => {
                let (data, cursor) = psynergy::assets::lz::decode_general(
                    rom,
                    start,
                    rom.len(),
                    address(&stream["decoded_size"])? as u64,
                )
                .map_err(|e| e.to_string())?;
                if cursor > start + address(&stream["encoded_size"])? + 2 {
                    return Err("sprite lookahead exceeds stream boundary".into());
                }
                (data, address(&stream["encoded_size"])?)
            }
            Some("golden-sun-tagged-palette-lz" | "golden-sun-palette-lz") => {
                let offset = if stream["codec"] == "golden-sun-tagged-palette-lz" {
                    1
                } else {
                    0
                };
                let (data, cursor) = psynergy::assets::lz::decode_palette(
                    rom,
                    start + offset,
                    rom.len(),
                    address(&stream["decoded_size"])? as u64,
                )
                .map_err(|e| e.to_string())?;
                if cursor > start + address(&stream["encoded_size"])? + 2 {
                    return Err("sprite palette lookahead exceeds stream boundary".into());
                }
                (data, address(&stream["encoded_size"])?)
            }
            _ => return Err("unregistered character frame codec".into()),
        };
        if data.len() != address(&stream["decoded_size"])?
            || size != address(&stream["encoded_size"])?
        {
            return Err("character stream extent differs".into());
        }
        let frame_size = fw.checked_mul(fh).ok_or("frame extent overflows")?;
        let decoded = if component["kind"] == "zero-skip-bytes" {
            zero_skip(&data, frame_size)?
        } else if component["kind"] == "indexed-bytes" && data.len() == frame_size {
            data
        } else {
            return Err("character frame codec or size differs".into());
        };
        for y in 0..fh {
            let dst = (frame / columns * fh + y) * width + frame % columns * fw;
            output
                .get_mut(dst..dst + fw)
                .ok_or("frame outside sheet")?
                .copy_from_slice(&decoded[y * fw..(y + 1) * fw]);
        }
    }
    if sha256::hex(&output) != json_string(&input["decoded_sha256"], "sprite digest")? {
        return Err("character pixels differ from registered input".into());
    }
    Ok(output)
}
pub(super) fn extract_all(root: &Path, inputs: &Value, rom: &[u8]) -> Result<(), String> {
    atlas::extract(root, inputs, rom)
}
pub(super) fn check(ctx: &mut Context, input: &Value) -> Result<(), String> {
    let root = ctx.root.clone();
    let name = json_string(&input["source"], "sprite source")?;
    let bank = bank(ctx, input)?;
    let component = component(&bank);
    if component["source"] != name || component["pixel_format"] != "indices" {
        return Err("character registry differs from pixel component".into());
    }
    let img = ctx.indexed(&root_path(&root, name)?)?;
    let (width, height, data) = indexed_rect(&img, component)?;
    if width != address(&input["width"])?
        || height != address(&input["height"])?
        || sha256::hex(&data) != json_string(&input["decoded_sha256"], "sprite hash")?
    {
        return Err(format!("private character input differs: {name}"));
    }
    check_shared_palette(&root, component, &img)?;
    if component.get("source_rect") != input.get("source_rect") {
        return Err("private character rectangle differs from component".into());
    }
    let (encoded, _, _) = build_entry(ctx, &bank)?;
    if sha256::hex(&encoded) != json_string(&input["encoded_sha256"], "character bank hash")? {
        return Err("character bank encoding differs".into());
    }
    Ok(())
}
fn rewrite(value: &mut Value, names: &BTreeMap<String, (String, String)>, palette: &[usize]) {
    if let Some(object) = value.as_object_mut() {
        if object.get("source").and_then(Value::as_str) == Some(OLD_PALETTE) {
            if object.get("kind").and_then(Value::as_str) == Some("gba-palette") {
                object.insert("kind".into(), json!("bgr555-banks"));
                object.insert("source".into(), json!(COLORS));
                object.insert("banks".into(), json!(palette));
            } else if object.get("entries").and_then(Value::as_u64) == Some(224)
                && object.get("offset").and_then(Value::as_u64) == Some(16)
            {
                object.insert("source".into(), json!(COLORS));
                object.insert("banks".into(), json!(&palette[1..]));
                object.remove("offset");
                object.remove("entries");
            }
        }
        for key in ["source", "plan"] {
            if let Some((name, pointer)) = object
                .get(key)
                .and_then(Value::as_str)
                .and_then(|old| names.get(old))
            {
                let name = name.clone();
                let pointer = pointer.clone();
                let kind = object.get("kind").and_then(Value::as_str).unwrap_or("");
                let selection = if key == "plan" {
                    Some(("plan_section", format!("{pointer}/streams")))
                } else if kind == "components" {
                    Some(("pointer", format!("{pointer}/components")))
                } else if kind == "pointer-table" {
                    Some(("pointer", format!("{pointer}/directory")))
                } else {
                    None
                };
                object.insert(key.into(), json!(name));
                if let Some((key, value)) = selection {
                    object.insert(key.into(), json!(value));
                }
            }
        }
        for child in object.values_mut() {
            rewrite(child, names, palette);
        }
    } else if let Some(array) = value.as_array_mut() {
        for child in array {
            rewrite(child, names, palette);
        }
    } else if let Some(text) = value.as_str() {
        if let Some((name, _)) = names.get(text) {
            *value = json!(name);
        }
    }
}
pub fn migrate(root: &Path, stage: &Path) -> Result<(), String> {
    let stage = if stage.is_absolute() {
        stage.to_path_buf()
    } else {
        root.join(stage)
    };
    if !stage.starts_with(root.join("out")) || stage.exists() {
        return Err("character migration needs a new staging directory under out".into());
    }
    let rom = fs::read(root.join("roms/tbs-en.gba")).map_err(|e| e.to_string())?;
    let japanese = fs::read(root.join("roms/tbs-ja.gba")).map_err(|e| e.to_string())?;
    let party = super::identity::party_names(&japanese)?;
    let mut index = json(&root.join(INDEX))?;
    if index.get("characters").is_some() {
        return atlas::migrate(root, &stage);
    }
    if sha256::hex(&rom)
        != index["reference_sha256"]
            .as_str()
            .ok_or("reference missing")?
        || index.get("characters").is_some()
    {
        return Err("character migration already installed or reference differs".into());
    }
    let mut colors = json(&root.join(COLORS))?;
    let raw = psynergy::assets::image::bgr555_palette_from_png(
        &fs::read(root.join(OLD_PALETTE)).map_err(|e| e.to_string())?,
    )
    .map_err(|e| e.to_string())?;
    if raw != rom[0x777c..0x795c] {
        return Err("character palette differs from ROM".into());
    }
    let mut palette = vec![];
    for chunk in raw.chunks_exact(32) {
        let bank = json!(chunk
            .chunks_exact(2)
            .map(|b| u16::from_le_bytes([b[0], b[1]]))
            .collect::<Vec<_>>());
        let banks = colors["banks"]
            .as_array_mut()
            .ok_or("palette banks missing")?;
        let slot = banks
            .iter()
            .position(|existing| *existing == bank)
            .unwrap_or(banks.len());
        if slot == banks.len() {
            banks.push(bank);
        }
        palette.push(slot);
    }
    let heroes = [
        ("ROBIN_BATTLE", "isaac"),
        ("battle_characters_chr_001_bank", "garet"),
        ("battle_characters_chr_002_bank", "ivan"),
        ("battle_characters_chr_003_bank", "mia"),
    ];
    let mut ctx = Context::new(root);
    let catalog = json(&root.join("games/tbs/assets/data/chr_catalog.json"))?;
    let descriptors = catalog["segments"][0]["records"]
        .as_array()
        .ok_or("character descriptor rows missing")?;
    let mut names = BTreeMap::new();
    let mut documents = BTreeMap::new();
    let mut private = vec![];
    let mut removed = vec![OLD_PALETTE.to_string()];
    let mut identities = vec![];
    for (actor, (field, battle)) in heroes.into_iter().enumerate() {
        let name = party[actor]["romaji"]
            .as_str()
            .ok_or("decoded character name missing")?;
        let metadata = format!("{DIR}/{name}.json");
        let mut identity = json!({"format":"golden-sun-character","identity":party[actor],"association_evidence":{"field":"party actor ID; REMAP_CHARACTER_ID_BY_FLAGS; 0808b3ec placement to Object_CreateFar; 0800c150 descriptor lookup","battle":"party_sprites entries 0-3; sentou_hyouji_kihon actor_pose_rows","scope":"actors 0-3 only; dense name IDs do not establish actor 5 identity"},"banks":{}});
        for (role, stem, descriptor) in [
            ("field", field.to_string(), actor),
            (
                "battle",
                format!("battle_characters_{battle}_bank"),
                0x140 + actor,
            ),
        ] {
            let old = format!("{DIR}/{stem}.json");
            let mut bank = json(&root.join(&old))?;
            let descriptor = &descriptors[descriptor];
            let component = &bank["components"][0]["components"][0];
            if component["kind"] != "zero-skip-bytes"
                || descriptor["frame_directory"] != bank["directory"]["address"]
                || descriptor["width"] != component["frame_width"]
                || descriptor["height"] != component["frame_height"]
            {
                return Err("character descriptor differs from export".into());
            }
            let (encoded, _, _) = build_entry(&mut ctx, &bank)?;
            let address = address(&bank["address"])? - ROM_BASE;
            if rom.get(address..address + encoded.len()) != Some(encoded.as_slice()) {
                return Err("legacy character bank differs from ROM".into());
            }
            let old_image = json_string(&component["source"], "character image")?.to_string();
            let img = indexed_png(&fs::read(root.join(&old_image)).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
            check_shared_palette(root, component, &img)?;
            let decoded = img
                .pixels
                .iter()
                .map(|p| u8::try_from(*p).map_err(|_| "sprite index exceeds byte"))
                .collect::<Result<Vec<_>, _>>()?;
            let source = format!(
                "{DIR}/{}_{name}.PNG",
                if role == "field" { "CHAR" } else { "BATTLE" }
            );
            let pointer = format!("/banks/{role}");
            bank["components"][0]["components"][0]["source"] = json!(source);
            bank["components"][0]["components"][0]["pixel_format"] = json!("indices");
            bank["descriptor"] = json!({"id":if role == "field" {actor} else {0x140+actor},"source":"games/tbs/assets/data/chr_catalog.json","pointer":format!("/segments/0/records/{}", if role == "field" {actor} else {0x140+actor}),"frame_codec":"arena-lz/zero-skip"});
            names.insert(old.clone(), (metadata.clone(), pointer.clone()));
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
            write(&stage, &source, &image(&decoded, img.width, img.height)?)?;
            private.push(json!({"kind":"sprite","source":source,"metadata":metadata,"pointer":pointer,"width":img.width,"height":img.height,"decoded_sha256":sha256::hex(&decoded),"encoded_sha256":sha256::hex(&encoded),"region_address":bank["address"]}));
            removed.extend([old, old_image]);
            identity["banks"][role] = bank;
        }
        identities.push(json!({"source":metadata,"identity":name,"field_descriptor":actor,"battle_descriptor":0x140+actor}));
        documents.insert(metadata, identity);
    }
    for document in documents.values_mut() {
        rewrite(document, &names, &palette);
    }
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
            || name == COLORS
            || name.ends_with("dossiers.json")
        {
            continue;
        }
        let text = fs::read_to_string(entry.path()).map_err(|e| e.to_string())?;
        if !text.contains(OLD_PALETTE) && !names.keys().any(|key| text.contains(key)) {
            continue;
        }
        let mut document: Value = serde_json::from_str(&text).map_err(|e| e.to_string())?;
        rewrite(&mut document, &names, &palette);
        documents.insert(name, document);
    }
    rewrite(&mut index, &names, &palette);
    index["characters"] = json!(identities);
    index["private_inputs"]
        .as_array_mut()
        .unwrap()
        .extend(private.clone());
    index["private_inputs"].as_array_mut().unwrap().push(json!({"kind":"palette-raw","source":COLORS,"region_address":"0x0800777c","banks":palette,"decoded_sha256":sha256::hex(&raw)}));
    let manifest_name = "games/tbs/assets/manifest.json";
    let manifest = documents
        .get_mut(manifest_name)
        .ok_or("palette region manifest missing")?;
    let regions = manifest["regions"].as_array_mut().unwrap();
    let position = regions
        .iter()
        .position(|r| r["address"] == "0x0800777c")
        .ok_or("palette ownership missing")?;
    index["regions"]
        .as_array_mut()
        .unwrap()
        .push(regions.remove(position));
    documents.insert(INDEX.into(), index);
    documents.insert(COLORS.into(), colors);
    for (name, value) in &documents {
        document(&stage, name, value)?;
    }
    // Verify regeneration before changing the live corpus.
    for input in &private {
        let decoded = pixels(&Context::new(&stage), input, &rom)?;
        let img = indexed_png(
            &fs::read(stage.join(input["source"].as_str().unwrap())).map_err(|e| e.to_string())?,
        )
        .map_err(|e| e.to_string())?;
        if img.pixels != decoded.iter().map(|p| u32::from(*p)).collect::<Vec<_>>() {
            return Err("regenerated character sheet differs".into());
        }
    }
    let backup = stage.join("BEFORE");
    for name in documents.keys().chain(removed.iter()) {
        if root.join(name).is_file() {
            write(
                &backup,
                name,
                &fs::read(root.join(name)).map_err(|e| e.to_string())?,
            )?;
        }
    }
    for (name, value) in documents {
        document(root, &name, &value)?;
    }
    for input in &private {
        write(
            root,
            input["source"].as_str().unwrap(),
            &fs::read(stage.join(input["source"].as_str().unwrap())).map_err(|e| e.to_string())?,
        )?;
    }
    for name in removed {
        fs::remove_file(root.join(name)).map_err(|e| e.to_string())?;
    }
    println!("characters=4 banks=8 descriptor_geometry=verified regenerated_pixels=identical");
    Ok(())
}

#[test]
fn sprite_zero_runs_require_exact_frame_and_terminator() {
    let pixels = [vec![0; 33], vec![1, 0xdf], vec![0; 8]].concat();
    let encoded = psynergy::assets::compression::encode_zero_skip(&pixels).unwrap();
    assert_eq!(zero_skip(&encoded, pixels.len()).unwrap(), pixels);
    assert!(zero_skip(&encoded, pixels.len() - 1).is_err());
    assert!(zero_skip(&encoded[..encoded.len() - 1], pixels.len()).is_err());
    assert!(zero_skip(&[0, 1], 0).is_err());
    assert!(zero_skip(&[0xe0, 0], 32).is_err());
}
