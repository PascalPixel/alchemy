use super::*;

const GRAPHICS: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS";

fn read(root: &Path, name: &str) -> Result<Value, String> {
    serde_json::from_slice(&fs::read(root.join(name)).map_err(|e| e.to_string())?)
        .map_err(|e| e.to_string())
}
fn rectangle(input: &Value) -> Value {
    let r = &input["source_rect"];
    json!([r["x"], r["y"], r["width"], r["height"]])
}
fn input(source: &Value) -> Value {
    json!({"source":source["source"],"rectangle":rectangle(source)})
}
fn banks(banks: &Value) -> Value {
    Value::Array(
        banks
            .as_array()
            .unwrap()
            .iter()
            .map(|b| json!({"reference":["bank",b,0],"count":16,"step":1}))
            .collect(),
    )
}
fn gray(count: usize, step: usize) -> Value {
    json!([{"reference":["gray",0],"count":count,"step":step}])
}
fn sheet(
    images: &mut Vec<Value>,
    palettes: &mut Vec<Value>,
    file: String,
    category: &str,
    size: Value,
    recipe: Value,
    palette: Value,
    transparent: bool,
) {
    let index = palettes
        .iter()
        .position(|p| p == &palette)
        .unwrap_or_else(|| {
            palettes.push(palette);
            palettes.len() - 1
        });
    images.push(
        json!({"file":file,"category":category,"size":size,"recipe":recipe,
        "palette":index,"transparent":transparent}),
    );
}

/// Ordinary contact sheets follow their native owners; only curated exceptions are serialized.
pub(super) fn defaults(root: &Path, palettes: &mut Vec<Value>) -> Result<Vec<Value>, String> {
    let source = read(root, "games/THE BROKEN SEAL/SOURCE.JSON")?;
    let common = read(root, &format!("{GRAPHICS}/CHARACTER/COMMON.JSON"))?;
    let compression = read(root, &format!("{GRAPHICS}/COMMON/COMPRESSION.JSON"))?;
    let portrait = read(root, &format!("{GRAPHICS}/COMMON/PORTRAIT.JSON"))?;
    let still = read(root, &format!("{GRAPHICS}/COMMON/STILL.JSON"))?;
    let obj = banks(&json!((877..891).collect::<Vec<_>>()));
    let mut images = Vec::new();
    for native in source["private_inputs"]
        .as_array()
        .ok_or("missing native inputs")?
    {
        let kind = native["kind"].as_str().unwrap_or("");
        if ![
            "tiles",
            "portrait-atlas",
            "still-atlas",
            "sprite-atlas",
            "archive-atlas",
            "tile-atlas",
        ]
        .contains(&kind)
        {
            continue;
        }
        let at = address(&native["region_address"])?;
        let length = native.get("encoded_length").or_else(|| {
            source["regions"]
                .as_array()?
                .iter()
                .find(|r| r["address"] == native["region_address"])?
                .get("size")
        });
        let range = length
            .map(|n| {
                address(n)
                    .map(|n| format!("0x{:06x}-0x{:06x}", at - 0x08000000, at + n - 0x08000000))
            })
            .transpose()?;
        let mut crop = input(native);
        let mut size = json!([native["width"], native["height"]]);
        let (file, category, recipe, palette, transparent) = match kind {
            "tiles" => {
                let png = psynergy::assets::image::indexed_png(
                    &fs::read(root.join(native["source"].as_str().ok_or("missing tile source")?))
                        .map_err(|e| e.to_string())?,
                )
                .map_err(|e| e.to_string())?;
                let cols = png.width as usize / 8;
                let offset = address(&native["tile_offset"])?;
                crop["rectangle"] = json!([offset % cols * 8, offset / cols * 8, 256, 128]);
                size = json!([256, 128]);
                (
                    format!("MAP_TILES_{}.PNG", range.ok_or("missing map extent")?),
                    "map-tile-bank",
                    Value::Null,
                    gray(16, 17),
                    false,
                )
            }
            "portrait-atlas" => {
                let i = native["pointer"]
                    .as_str()
                    .ok_or("missing portrait pointer")?
                    .rsplit('/')
                    .next()
                    .unwrap()
                    .parse::<usize>()
                    .map_err(|e| e.to_string())?;
                (
                    format!("PORTRAIT_{}.PNG", range.ok_or("missing portrait extent")?),
                    kind,
                    Value::Null,
                    banks(&portrait["components"][i - 1]["banks"]),
                    true,
                )
            }
            "still-atlas" => {
                let node = still["images"]
                    .as_object()
                    .ok_or("missing still images")?
                    .values()
                    .find(|v| v["source_rect"] == native["source_rect"])
                    .ok_or("missing still owner")?;
                let key = node["palette"].as_str().ok_or("missing still palette")?;
                let palette = json!([{"reference":["owner",format!("{GRAPHICS}/COMMON/STILL.JSON"),"palettes",key,0],"count":still["palettes"][key].as_array().ok_or("missing still colors")?.len(),"step":1}]);
                (
                    format!(
                        "BATTLE_BACKGROUND_{}.PNG",
                        range.ok_or("missing still extent")?
                    ),
                    "battle-background",
                    Value::Null,
                    palette,
                    true,
                )
            }
            "sprite-atlas" => {
                let Some((key, bank)) = common["banks"]
                    .as_object()
                    .ok_or("missing common banks")?
                    .iter()
                    .find(|(_, v)| v["address"] == native["region_address"])
                else {
                    continue;
                };
                let Some(pointer) = bank["streams"]["pointer"].as_str() else {
                    continue;
                };
                let mut component = &bank["components"][0];
                if component.get("components").is_some() {
                    component = &component["components"][0];
                }
                let count = compression
                    .pointer(pointer)
                    .and_then(Value::as_array)
                    .ok_or("missing sprite streams")?
                    .len();
                let fw = address(&component["frame_width"])?;
                let fh = address(&component["frame_height"])?;
                let cols = address(&component["columns"])?;
                size = json!([cols * fw, count.div_ceil(cols) * fh]);
                let recipe = json!({"kind":"grid","input":input(component),"frame_size":[fw,fh],"source_columns":cols,"frames":count,"columns":cols,"order":"row-major"});
                (
                    format!("SPRITE_UNCLASSIFIED_0x{key}_SHEET.PNG"),
                    "unclassified-sprite-sheet",
                    recipe,
                    obj.clone(),
                    true,
                )
            }
            "archive-atlas" => {
                let owner = common
                    .pointer(
                        native["pointer"]
                            .as_str()
                            .ok_or("missing archive pointer")?,
                    )
                    .ok_or("missing archive owner")?;
                let fw = address(&owner["frame_width"])?;
                let fh = address(&owner["frame_height"])?;
                let count = native["offsets"]
                    .as_array()
                    .ok_or("missing archive offsets")?
                    .len()
                    - 1;
                let pairs = count.div_ceil(2);
                let cols = pairs.min(8);
                size = json!([cols * fw, pairs.div_ceil(8) * 2 * fh]);
                let recipe = json!({"kind":"grid","input":crop,"frame_size":[fw,fh],"source_columns":owner["columns"],"frames":count,"columns":cols,"order":"front-back-wrapped"});
                (
                    format!(
                        "BATTLE_{}_SHEET.PNG",
                        range.ok_or("missing archive extent")?
                    ),
                    "battle-sheet",
                    recipe,
                    obj.clone(),
                    true,
                )
            }
            _ => {
                let Some(range) = range else { continue };
                (
                    format!("UNRESOLVED_{range}.PNG"),
                    "tile-atlas",
                    Value::Null,
                    gray(256, 1),
                    true,
                )
            }
        };
        let recipe = if recipe.is_null() {
            crop["at"] = json!([0, 0]);
            json!({"kind":"pieces","pieces":[crop]})
        } else {
            recipe
        };
        sheet(
            &mut images,
            palettes,
            file,
            category,
            size,
            recipe,
            palette,
            transparent,
        );
    }
    for bank in [4, 5, 6, 0] {
        let (name, path, category, palette) = if bank == 0 {
            (
                "FONT_0x032224-0x033e24_CODES_0020_00FF_SHEET.PNG".to_string(),
                format!("{GRAPHICS}/FONT/LOCALIZATION_GLYPHS_0020_00FF.1BPP.PNG"),
                "font-sheet",
                gray(2, 255),
            )
        } else {
            (
                format!("ICON_BANK_{bank:02}_SHEET.PNG"),
                format!("{GRAPHICS}/TILE/UI_MTF_{bank:02}.INDEXED.PNG"),
                "icon-sheet",
                banks(&json!([877])),
            )
        };
        let png = psynergy::assets::image::indexed_png(
            &fs::read(root.join(&path)).map_err(|e| e.to_string())?,
        )
        .map_err(|e| e.to_string())?;
        sheet(
            &mut images,
            palettes,
            name,
            category,
            json!([png.width, png.height]),
            json!({"kind":"pieces","pieces":[{"source":path,"rectangle":[0,0,png.width,png.height],"at":[0,0]}]}),
            palette,
            true,
        );
    }
    Ok(images)
}

fn expand_input(plan: &Value, row: &Value) -> Value {
    json!({"source":plan["sources"][row[0].as_u64().unwrap() as usize],
        "rectangle":[row[1],row[2],row[3],row[4]]})
}
fn expand_piece(plan: &Value, row: &Value, bytes: bool) -> Value {
    let mut piece = if bytes {
        json!({"offset":row[0],"size":[row[1],row[2]]})
    } else {
        expand_input(plan, row)
    };
    let at = if bytes { 3 } else { 5 };
    piece["at"] = json!([row[at], row[at + 1]]);
    for (offset, key) in [
        (2, "flip_x"),
        (3, "flip_y"),
        (4, "palette_offset"),
        (5, "display_size"),
    ] {
        if let Some(value) = row.get(at + offset) {
            if !value.is_null() {
                piece[key] = value.clone();
            }
        }
    }
    piece
}
fn layout(plan: &Value, row: &Value) -> Result<Value, String> {
    let kind = row[0].as_str().ok_or("missing exception layout")?;
    let mut recipe = match kind {
        "pieces" => {
            json!({"kind":kind,"pieces":row[1].as_array().ok_or("missing pieces")?.iter().map(|p|expand_piece(plan,p,false)).collect::<Vec<_>>()})
        }
        "grid" => {
            json!({"kind":kind,"input":expand_input(plan,&row[1]),"frame_size":row[2],"source_columns":row[3],"cells":row[4],"frames":row[5],"columns":row[6],"order":row[7]})
        }
        "linear-bytes" => {
            json!({"kind":kind,"input":expand_input(plan,&row[1]),"native_bpp":row[2],"decoded_length":row[3]})
        }
        "tile-frames" => {
            json!({"kind":kind,"input":expand_input(plan,&row[1]),"frame_size":row[2],"columns":row[3],"frames":row[4],"source_frame_size":row[5]})
        }
        "byte-pieces" => {
            json!({"kind":kind,"inputs":row[1].as_array().ok_or("missing byte inputs")?.iter().map(|p|expand_input(plan,p)).collect::<Vec<_>>(),"native_bpp":row[2],"output_bpp":row[3],"pieces":row[4].as_array().ok_or("missing byte pieces")?.iter().map(|p|expand_piece(plan,p,true)).collect::<Vec<_>>(),"source_frame_size":row[5]})
        }
        "camelot" => {
            json!({"kind":kind,"base":expand_input(plan,&row[1]),"animation":expand_input(plan,&row[2]),"tilemap_source":plan["sources"][address(&row[3])?],"tilemap_offset":row[4],"frames":row[5],"stride":row[6],"replace_tile":row[7],"replace_bytes":row[8]})
        }
        _ => return Err("unknown exception layout".into()),
    };
    if let Some(runs) = recipe["cells"]["runs"].as_array() {
        let mut cells = Vec::new();
        for run in runs {
            for i in 0..address(&run[1])? {
                let x = run[3].as_i64().ok_or("invalid run x")?
                    + i as i64 * run[5].as_i64().ok_or("invalid run step")?;
                let y = run[4].as_i64().ok_or("invalid run y")?
                    + i as i64 * run[6].as_i64().ok_or("invalid run step")?;
                if x < 0 || y < 0 {
                    return Err("negative run placement".into());
                }
                cells.push(json!([address(&run[0])? + i * address(&run[2])?, x, y]));
            }
        }
        recipe["cells"] = json!(cells);
    }
    recipe
        .as_object_mut()
        .unwrap()
        .retain(|_, value| !value.is_null());
    Ok(recipe)
}
pub(super) fn expand(root: &Path, plan: &mut Value) -> Result<(), String> {
    let mut palettes = plan["palettes"]
        .as_array()
        .ok_or("missing palettes")?
        .iter()
        .map(|p| {
            Value::Array(
                p.as_array()
                    .unwrap()
                    .iter()
                    .map(|r| json!({"reference":r[0],"count":r[1],"step":r[2]}))
                    .collect(),
            )
        })
        .collect::<Vec<_>>();
    let mut images = defaults(root, &mut palettes)?;
    images.retain(|image| {
        !plan["exclude"].as_array().unwrap().contains(&image["file"])
            && !plan["exceptions"]
                .as_array()
                .unwrap()
                .iter()
                .any(|r| r[0] == image["file"])
    });
    for row in plan["exceptions"].as_array().ok_or("missing exceptions")? {
        images.push(json!({"file":row[0],"category":row[1],"size":[row[2],row[3]],"palette":row[4],"transparent":row[5],"recipe":layout(plan,&plan["layouts"][address(&row[6])?])?,"evidence":plan["evidence"][address(&row[7])?]}));
    }
    images.sort_by(|a, b| a["file"].as_str().cmp(&b["file"].as_str()));
    plan["palettes"] = json!(palettes);
    plan["images"] = json!(images);
    Ok(())
}
