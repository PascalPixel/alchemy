use super::*;
use std::io::Write;
use std::process::{Command, Stdio};

fn git(root: &Path, args: &[&str], input: Option<&[u8]>) -> Result<Vec<u8>, String> {
    let mut child = Command::new("git")
        .args(args)
        .current_dir(root)
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::piped())
        .spawn()
        .map_err(|e| e.to_string())?;
    if let Some(input) = input {
        child
            .stdin
            .take()
            .ok_or("git input unavailable")?
            .write_all(input)
            .map_err(|e| e.to_string())?;
    }
    let output = child.wait_with_output().map_err(|e| e.to_string())?;
    if !output.status.success()
        && !(args.first() == Some(&"check-ignore") && output.status.code() == Some(1))
    {
        return Err(String::from_utf8_lossy(&output.stderr).into());
    }
    Ok(output.stdout)
}
fn paths(data: &[u8]) -> BTreeSet<String> {
    String::from_utf8_lossy(data)
        .split('\0')
        .filter(|p| !p.is_empty())
        .map(str::to_string)
        .collect()
}
fn source_spelling(spellings: &mut BTreeMap<String, String>, name: &str) -> Result<(), String> {
    if let Some(previous) = spellings.insert(name.to_ascii_lowercase(), name.to_string()) {
        if previous != name {
            return Err(format!(
                "source filenames differ only by case: {previous}, {name}"
            ));
        }
    }
    Ok(())
}

#[test]
fn private_sources_cannot_collide_on_case_insensitive_filesystems() {
    let mut spellings = BTreeMap::new();
    source_spelling(&mut spellings, "COMMON/CHR.png").unwrap();
    source_spelling(&mut spellings, "COMMON/CHR.png").unwrap();
    assert!(source_spelling(&mut spellings, "COMMON/CHR.PNG").is_err());
    source_spelling(&mut spellings, "COMMON/TILE_BANK.PNG").unwrap();
}
fn classify(
    files: &BTreeSet<String>,
    tracked: &BTreeSet<String>,
    ignored: &BTreeSet<String>,
    private: &BTreeSet<String>,
) -> Result<(), String> {
    for file in files {
        if private.contains(file) {
            if tracked.contains(file) || !ignored.contains(file) {
                return Err(format!(
                    "private native input must be ignored and untracked: {file}"
                ));
            }
        } else if !tracked.contains(file) || ignored.contains(file) {
            return Err(format!(
                "game input must be tracked and not ignored: {file}"
            ));
        }
    }
    Ok(())
}
pub(in crate::build_assets) fn check(root: &Path) -> Result<(), String> {
    let mut private = BTreeSet::new();
    let mut bytes: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let mut ctx = Context::new(root);
    for game in games() {
        let paths = NativePaths::of(&game);
        if !root.join(&paths.index).is_file() {
            continue;
        }
        let index = &json(&root.join(&paths.index))?;
        // A clone without this game's reference ROM cannot restore its private
        // inputs; their registration is still checked, their absent bytes are not.
        let restorable = root.join(game.rom).is_file();
        let source = paths.source;
        let colors = paths.colors.as_str();
        validate(index)?;
        let mut spellings = BTreeMap::new();
        for input in index["private_inputs"]
            .as_array()
            .ok_or("missing native private inputs")?
        {
            let name = json_string(&input["source"], "private input")?;
            let kind = json_string(&input["kind"], "private kind")?;
            let path = Path::new(name);
            if !name.starts_with(&format!("{source}/"))
                || path
                    .components()
                    .any(|c| matches!(c, std::path::Component::ParentDir))
            {
                return Err("private native path escapes source tree".into());
            }
            let allowed = match kind {
                "frame-atlas" => name == format!("{source}/GRAPHICS/COMMON/TILE_BANK.PNG"),
                "portrait-atlas" => name == format!("{source}/GRAPHICS/COMMON/PORTRAIT.PNG"),
                "tile-atlas" => {
                    name == format!("{source}/GRAPHICS/COMMON/TILE.PNG")
                        || name == format!("{source}/GRAPHICS/COMMON/TILE_BANK.PNG")
                }
                "still-atlas" => name == format!("{source}/GRAPHICS/COMMON/STILL.PNG"),
                "grid" | "metatiles" | "bytes" => name.ends_with(".BIN"),
                "tiles" => name.ends_with("/CHR.PNG") || name.ends_with("_CHR.PNG"),
                "sprite" | "sprite-atlas" | "archive-atlas" => {
                    (name.starts_with(&format!("{source}/GRAPHICS/CHARACTER/CHAR_"))
                        || name.starts_with(&format!("{source}/GRAPHICS/CHARACTER/BATTLE_")))
                        && name.ends_with(".PNG")
                }
                "palette" | "palette-raw" | "palette-buffer" | "palette-table" => name == colors,
                _ => false,
            };
            if !allowed {
                return Err(format!("unrecognized native private input {name}"));
            }
            source_spelling(&mut spellings, name)?;
            private.insert(name.to_string());
            if !restorable && !root.join(name).exists() {
                continue;
            }
            if kind == "frame-atlas" {
                frame::check(&mut ctx, index, input)?;
                continue;
            }
            if kind == "palette-table" {
                let doc = ctx.document(&root.join(name))?;
                let values = doc
                    .pointer(json_string(&input["pointer"], "palette table pointer")?)
                    .ok_or("palette table absent")?;
                let bytes = integer_array(values, "le-u16-array")?;
                if bytes.len() != address(&input["decoded_length"])?
                    || sha256::hex(&bytes)
                        != json_string(&input["decoded_sha256"], "palette table digest")?
                {
                    return Err("palette table differs".into());
                }
                continue;
            }
            if kind == "portrait-atlas" {
                portrait::check(&ctx, input)?;
                continue;
            }
            if kind == "tile-atlas" {
                tile::check(&mut ctx, index, input)?;
                continue;
            }
            if kind == "still-atlas" {
                still::check(&mut ctx, index, input)?;
                continue;
            }
            if kind == "archive-atlas" {
                graphics::check(&mut ctx, index, input)?;
                continue;
            }
            if matches!(kind, "sprite" | "sprite-atlas") {
                character::check(&mut ctx, input)?;
                continue;
            }
            if matches!(kind, "palette" | "palette-raw" | "palette-buffer") {
                let palette = ctx.document(&root.join(name))?;
                let mut data = vec![];
                for bank in input["banks"].as_array().ok_or("missing palette indices")? {
                    let values = palette["banks"]
                        .get(address(bank)?)
                        .and_then(Value::as_array)
                        .ok_or("missing palette bank")?;
                    if values.len() != 16 {
                        return Err("palette banks need sixteen colors".into());
                    }
                    for value in values {
                        let value = u16::try_from(address(value)?)
                            .map_err(|_| "palette color exceeds u16")?;
                        data.extend(value.to_le_bytes());
                    }
                }
                if sha256::hex(&data) != json_string(&input["decoded_sha256"], "input hash")? {
                    return Err(format!("private palette differs: {name}"));
                }
                continue;
            }
            if kind == "bytes" {
                if !bytes.contains_key(name) {
                    let file = fs::read(root.join(name)).map_err(|e| {
                        format!("{name}: {e}; run alchemy build assets --extract-sources ROM")
                    })?;
                    bytes.insert(name.to_string(), file);
                }
                let span = byte_span(index, input, name, &bytes[name])?;
                if sha256::hex(&span) != json_string(&input["decoded_sha256"], "input hash")? {
                    return Err(format!("private native bytes differ: {name}"));
                }
                continue;
            }
            if !bytes.contains_key(name) {
                let file = fs::read(root.join(name)).map_err(|e| {
                    format!("{name}: {e}; run alchemy build assets --extract-sources ROM")
                })?;
                let data = if kind == "tiles" {
                    psynergy::assets::image::gba_tiles_from_png(&file, GbaBpp::Bpp4)
                        .map_err(|e| e.to_string())?
                } else {
                    file
                };
                bytes.insert(name.to_string(), data);
            }
            let region = index["regions"]
                .as_array()
                .unwrap()
                .iter()
                .find(|r| r["address"] == input["region_address"])
                .ok_or("private input lacks owned region")?;
            let component = region["components"]
                .as_array()
                .ok_or("private input lacks component")?
                .first()
                .ok_or("private component missing")?;
            if component["source"] != name {
                return Err("private registry differs from component source".into());
            }
            let (start, length) = if kind == "tiles" {
                if component["tile_offset"] != input["tile_offset"]
                    || component["tile_count"] != 512
                {
                    return Err("private tile extent differs from component".into());
                }
                (
                    address(&input["tile_offset"])?
                        .checked_mul(32)
                        .ok_or("tile offset overflows")?,
                    16384,
                )
            } else {
                if component["source_offset"] != input["source_offset"] {
                    return Err("private map extent differs from component".into());
                }
                (
                    address(&input["source_offset"])?,
                    address(&component["source_length"])?,
                )
            };
            let end = start
                .checked_add(length)
                .ok_or("private source extent overflows")?;
            let data = bytes[name]
                .get(start..end)
                .ok_or("private source extent outside file")?;
            if sha256::hex(data) != json_string(&input["decoded_sha256"], "input hash")? {
                return Err(format!("private native input differs: {name}@{start}"));
            }
        }
    }
    let files = walkdir::WalkDir::new(root.join("games"))
        .into_iter()
        .filter_map(Result::ok)
        .filter(|f| f.file_type().is_file() && f.file_name() != ".DS_Store")
        .map(|f| root_relative(root, f.path()))
        .collect::<Result<BTreeSet<_>, _>>()?;
    let tracked = paths(&git(
        root,
        &["ls-files", "-z", "--cached", "--", "games"],
        None,
    )?);
    let input = files
        .iter()
        .flat_map(|p| p.as_bytes().iter().copied().chain([0]))
        .collect::<Vec<_>>();
    let ignored = paths(&git(
        root,
        &["check-ignore", "--no-index", "-z", "--stdin"],
        Some(&input),
    )?);
    classify(&files, &tracked, &ignored, &private)?;
    println!(
        "game input tracking ok: private_native_files={} hashes=verified",
        private.len()
    );
    Ok(())
}

/// The registered spans of a byte input, in the order it lists its regions.
fn byte_span(index: &Value, input: &Value, name: &str, file: &[u8]) -> Result<Vec<u8>, String> {
    let mut span = Vec::new();
    for target in byte_input_regions(input)? {
        let (_, component) = registered_component(input_region(index, target)?, name)?;
        if input
            .get("source_offset")
            .is_some_and(|offset| *offset != component["source_offset"])
        {
            return Err("private byte extent differs from component".into());
        }
        let start = address(&component["source_offset"])?;
        let end = start
            .checked_add(address(&component["source_length"])?)
            .ok_or("private source extent overflows")?;
        span.extend_from_slice(
            file.get(start..end)
                .ok_or("private source extent outside file")?,
        );
    }
    Ok(span)
}
#[test]
fn byte_inputs_digest_every_listed_span_in_order() {
    let index = serde_json::json!({"regions":[
        {"address":"0x08000010","size":2,"kind":"u8-array","format":"binary","source":"A.BIN","source_offset":0,"source_length":2},
        {"address":"0x08000020","size":6,"kind":"components","components":[
            {"kind":"byte-fill","size":4,"value":0},
            {"kind":"le-u16-array","format":"binary","source":"A.BIN","size":2,"source_offset":2,"source_length":2}]}]});
    let grouped =
        serde_json::json!({"kind":"bytes","source":"A.BIN","regions":["0x08000010","0x08000020"]});
    assert_eq!(
        byte_span(&index, &grouped, "A.BIN", &[1, 2, 3, 4]).unwrap(),
        [1, 2, 3, 4]
    );
    assert!(byte_span(&index, &grouped, "A.BIN", &[1, 2, 3]).is_err());
    assert!(byte_span(&index, &grouped, "B.BIN", &[1, 2, 3, 4]).is_err());
    let single = serde_json::json!({"kind":"bytes","source":"A.BIN","source_offset":2,"region_address":"0x08000020"});
    assert_eq!(
        byte_span(&index, &single, "A.BIN", &[1, 2, 3, 4]).unwrap(),
        [3, 4]
    );
    let moved = serde_json::json!({"kind":"bytes","source":"A.BIN","source_offset":0,"region_address":"0x08000020"});
    assert!(byte_span(&index, &moved, "A.BIN", &[1, 2, 3, 4]).is_err());
    let absent = serde_json::json!({"kind":"bytes","source":"A.BIN","region_address":"0x08000030"});
    assert!(byte_span(&index, &absent, "A.BIN", &[1, 2, 3, 4]).is_err());
}
#[test]
fn private_inputs_require_registration_ignoring_and_nonpublication() {
    let files = BTreeSet::from(["FIELD.C".into(), "MAP.bin".into()]);
    let tracked = BTreeSet::from(["FIELD.C".into()]);
    let ignored = BTreeSet::from(["MAP.bin".into()]);
    let private = ignored.clone();
    classify(&files, &tracked, &ignored, &private).unwrap();
    assert!(classify(&files, &tracked, &ignored, &BTreeSet::new()).is_err());
    assert!(classify(&files, &files, &ignored, &private).is_err());
    assert!(classify(&files, &tracked, &BTreeSet::new(), &private).is_err());
    assert!(classify(&files, &BTreeSet::new(), &ignored, &private).is_err());
}
