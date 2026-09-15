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
    let index = if root.join(INDEX).is_file() {
        Some(json(&root.join(INDEX))?)
    } else {
        None
    };
    let mut private = BTreeSet::new();
    let mut bytes: BTreeMap<String, Vec<u8>> = BTreeMap::new();
    let ctx = Context::new(root);
    if let Some(index) = &index {
        validate(index)?;
        for input in index["private_inputs"]
            .as_array()
            .ok_or("missing native private inputs")?
        {
            let name = json_string(&input["source"], "private input")?;
            let kind = json_string(&input["kind"], "private kind")?;
            let path = Path::new(name);
            if !name.starts_with("games/tbs/SRC/")
                || path
                    .components()
                    .any(|c| matches!(c, std::path::Component::ParentDir))
            {
                return Err("private native path escapes source tree".into());
            }
            let allowed = match kind {
                "grid" | "metatiles" => name.ends_with(".bin"),
                "tiles" => name.ends_with("/CHR.png") || name.ends_with("_CHR.png"),
                "palette" => name == COLORS,
                _ => false,
            };
            if !allowed {
                return Err(format!("unrecognized native private input {name}"));
            }
            private.insert(name.to_string());
            if kind == "palette" {
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
