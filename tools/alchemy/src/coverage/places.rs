//! Field places: the scenes each field overlay serves, the location names the
//! ROM's name resolver gives them, and the source directory each overlay
//! belongs in. Everything is read from the tracked tables; only a home that
//! neither a map nor a source file names yet is registered, in
//! `recon/<game>/overlay-homes.tsv`, and that list shrinks as overlays land.
use crate::compiler::routing::CompilerTarget;
use crate::coverage::tree::SourceTree;
use crate::targets::DecompTarget;
use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};

/// One field overlay and what the scene and name tables say about it.
#[derive(Debug, PartialEq, Eq)]
pub(crate) struct Place {
    pub overlay: usize,
    /// Scene indices that load the overlay, ascending.
    pub scenes: Vec<usize>,
    /// Location message ids in resolver order, without repeats.
    pub messages: Vec<usize>,
    /// The overlay's source directory under the game directory, such as
    /// `SRC/FIELD/HAIDIA_MURA`.
    pub home: Option<String>,
}

struct Game {
    /// Scene records the loader reads. The Broken Seal bounds its selector at
    /// 201 (0x08029094); the records after it are never loaded.
    scenes: Option<usize>,
    /// The first location message. The Broken Seal's Menu_DrawSelectionRow
    /// adds 0x99b to the resolved name; The Lost Age's resolver result plus
    /// 0xe58 is the English name.
    names: usize,
}
fn game(target: &DecompTarget) -> Game {
    match target.compiler {
        CompilerTarget::Tbs => Game {
            scenes: Some(201),
            names: 0x99b,
        },
        CompilerTarget::Tla => Game {
            scenes: None,
            names: 0xe58,
        },
    }
}

fn number(value: &Value) -> Option<i64> {
    value.as_i64().or_else(|| {
        let text = value.as_str()?;
        match text.strip_prefix("0x") {
            Some(digits) => i64::from_str_radix(digits, 16).ok(),
            None => text.parse().ok(),
        }
    })
}
fn read(tree: &SourceTree, path: &str) -> Option<Value> {
    serde_json::from_str(&tree.read(path)?).ok()
}
fn records(document: &Value) -> &[Value] {
    document["segments"][0]["records"]
        .as_array()
        .map_or(&[], Vec::as_slice)
}

/// A name rule: `key` is a scene when `by_scene`, else an area; `entry`
/// 0x7fff matches every entry.
struct Rule {
    key: i64,
    by_scene: bool,
    entry: i64,
    name: usize,
}
fn rules(document: &Value) -> Vec<Rule> {
    records(document)
        .iter()
        .filter_map(|rule| {
            // The Broken Seal spells the selector as one signed halfword.
            if let Some(selector) = rule["selector_b"].as_i64() {
                return Some(Rule {
                    key: rule["selector_a"].as_i64()?,
                    by_scene: selector & 0x8000 != 0,
                    entry: selector & 0x7fff,
                    name: rule["result"].as_u64()? as usize,
                });
            }
            let selector = rule["selector"].as_array()?;
            Some(Rule {
                key: rule["key"].as_i64()?,
                by_scene: selector.get(1)?.as_i64()? == 1,
                entry: selector.first()?.as_i64()?,
                name: rule["name"].as_u64()? as usize,
            })
        })
        .collect()
}

/// Every field overlay the scene table loads, ascending by overlay id.
pub(crate) fn places(tree: &SourceTree, target: &DecompTarget) -> Result<Vec<Place>, String> {
    let game = game(target);
    let field = format!("{}/SRC/FIELD", target.game_dir());
    let scene_path = format!("{field}/COMMON/SCENE_TABLE.JSON");
    let scenes = read(tree, &scene_path).ok_or_else(|| format!("missing {scene_path}"))?;
    let loads = read(tree, &format!("{field}/COMMON/LOAD_TABLE.JSON")).unwrap_or_default();
    let rules = read(tree, &format!("{field}/COMMON/NAME_RULES.JSON"))
        .map(|document| rules(&document))
        .unwrap_or_default();
    let containers: BTreeMap<i64, &str> = loads["records"]
        .as_array()
        .into_iter()
        .flatten()
        .filter_map(|row| Some((row["map_index"].as_i64()?, row["container"].as_str()?)))
        .collect();
    let map_homes = map_homes(tree, &field);
    let mut places: BTreeMap<usize, Place> = BTreeMap::new();
    let mut maps: BTreeMap<usize, BTreeSet<&str>> = BTreeMap::new();
    let records = records(&scenes);
    for (scene, record) in records
        .iter()
        .enumerate()
        .take(game.scenes.unwrap_or(records.len()))
    {
        let overlay = number(&record["resource_id"])
            .or_else(|| number(&record["overlay"]))
            .ok_or_else(|| format!("scene {scene} names no overlay"))?
            as usize;
        let area = record["group"].as_i64().or_else(|| record["area"].as_i64());
        let place = places.entry(overlay).or_insert_with(|| Place {
            overlay,
            scenes: Vec::new(),
            messages: Vec::new(),
            home: None,
        });
        place.scenes.push(scene);
        for rule in &rules {
            if Some(rule.key)
                != if rule.by_scene {
                    Some(scene as i64)
                } else {
                    area
                }
            {
                continue;
            }
            let message = game.names + rule.name;
            if !place.messages.contains(&message) {
                place.messages.push(message);
            }
            if rule.entry == 0x7fff {
                break;
            }
        }
        let container = record["map_index"]
            .as_i64()
            .and_then(|index| containers.get(&index));
        for home in container
            .and_then(|container| map_homes.get(*container))
            .into_iter()
            .flatten()
        {
            maps.entry(overlay).or_default().insert(home);
        }
    }
    let sources = source_homes(tree, target);
    let registered = registered_homes(tree, target);
    for (overlay, place) in &mut places {
        let maps = maps.remove(overlay).unwrap_or_default();
        let sources = sources.get(overlay);
        let shared: Vec<_> = sources
            .into_iter()
            .flat_map(BTreeMap::keys)
            .filter(|home| maps.contains(home.as_str()))
            .collect();
        place.home = if let [home] = shared.as_slice() {
            Some(home.to_string())
        } else if maps.len() == 1 {
            maps.first().map(|home| home.to_string())
        } else if let Some(sources) = sources {
            // The directory that holds most of the overlay's functions.
            sources
                .iter()
                .max_by_key(|(home, count)| (**count, std::cmp::Reverse(*home)))
                .map(|(home, _)| home.clone())
        } else {
            registered.get(overlay).cloned()
        };
    }
    Ok(places.into_values().collect())
}

/// The directory of each map container's area document, for maps that belong
/// to one area; the shared `COMMON/MAP.JSON` names no home.
fn map_homes(tree: &SourceTree, field: &str) -> BTreeMap<String, Vec<String>> {
    let mut homes: BTreeMap<String, Vec<String>> = BTreeMap::new();
    let mut directories: Vec<String> = tree
        .list(field)
        .into_iter()
        .filter(|name| name != "COMMON")
        .collect();
    directories.extend(
        tree.list(&format!("{field}/COMMON"))
            .into_iter()
            .map(|name| format!("COMMON/{name}")),
    );
    for directory in directories {
        let name = directory.rsplit('/').next().unwrap_or(&directory);
        let Some(document) = read(tree, &format!("{field}/{directory}/{name}.JSON")) else {
            continue;
        };
        for container in document["maps"]
            .as_object()
            .into_iter()
            .flatten()
            .map(|m| m.0)
        {
            homes
                .entry(container.clone())
                .or_default()
                .push(format!("SRC/FIELD/{directory}"));
        }
    }
    homes
}

/// The directories holding each overlay's registered functions, with counts.
fn source_homes(
    tree: &SourceTree,
    target: &DecompTarget,
) -> BTreeMap<usize, BTreeMap<String, usize>> {
    let mut homes: BTreeMap<usize, BTreeMap<String, usize>> = BTreeMap::new();
    let Some(register) = read(tree, &format!("{}/source-paths.json", target.recon_dir())) else {
        return homes;
    };
    for (key, owner) in register["owners"].as_object().into_iter().flatten() {
        let Some(overlay) = key
            .strip_prefix("resource_")
            .and_then(|rest| rest.split(':').next())
            .and_then(|id| usize::from_str_radix(id, 16).ok())
        else {
            continue;
        };
        let Some((directory, _)) = owner["source"].as_str().and_then(|s| s.rsplit_once('/')) else {
            continue;
        };
        *homes
            .entry(overlay)
            .or_default()
            .entry(format!("SRC/{directory}"))
            .or_default() += 1;
    }
    homes
}

/// Homes chosen for overlays that no map or source file places yet.
fn registered_homes(tree: &SourceTree, target: &DecompTarget) -> BTreeMap<usize, String> {
    tree.read(&format!("{}/overlay-homes.tsv", target.recon_dir()))
        .unwrap_or_default()
        .lines()
        .filter(|line| !line.is_empty() && !line.starts_with('#'))
        .filter_map(|line| {
            let (overlay, home) = line.split_once('\t')?;
            let overlay = usize::from_str_radix(overlay.strip_prefix("resource_")?, 16).ok()?;
            Some((overlay, home.trim().to_string()))
        })
        .collect()
}

/// The place table as TSV, with each location's English name.
fn table(root: &std::path::Path, target: &DecompTarget) -> Result<String, String> {
    let tree = SourceTree::Work {
        id: "work".into(),
        root: root.into(),
    };
    let path = format!("{}/TEXT/EN.PO", target.game_dir());
    let catalog = psynergy::assets::po::read(
        &std::fs::read_to_string(root.join(&path)).map_err(|e| format!("{path}: {e}"))?,
    )
    .map_err(|e| format!("{path}: {e}"))?;
    let names: BTreeMap<usize, &str> = catalog
        .entries
        .iter()
        .filter_map(|entry| Some((entry.id.parse().ok()?, entry.value.as_str())))
        .collect();
    let mut text = String::from("resource_id\tlocations\tscene_ids\tname_message_ids\thome\n");
    for place in places(&tree, target)? {
        let mut locations: Vec<&str> = Vec::new();
        for message in &place.messages {
            let name = names.get(message).copied().unwrap_or("?");
            if !locations.contains(&name) {
                locations.push(name);
            }
        }
        let join = |items: Vec<String>| items.join(",");
        text.push_str(&format!(
            "resource_{:x}\t{}\t{}\t{}\t{}\n",
            place.overlay,
            locations.join("; "),
            join(place.scenes.iter().map(usize::to_string).collect()),
            join(place.messages.iter().map(|m| format!("{m:#x}")).collect()),
            place.home.as_deref().unwrap_or(""),
        ));
    }
    Ok(text)
}

/// Write `out/<target>/locations.tsv` and return its path.
pub(crate) fn write(root: &std::path::Path, target: &DecompTarget) -> Result<String, String> {
    let path = format!("{}/locations.tsv", target.output_dir);
    let text = table(root, target)?;
    std::fs::create_dir_all(root.join(target.output_dir)).map_err(|e| e.to_string())?;
    std::fs::write(root.join(&path), text).map_err(|e| format!("{path}: {e}"))?;
    Ok(path)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::targets::{target_for, DecompTargetId};
    use serde_json::json;

    #[test]
    fn places_follow_scene_name_and_map_tables_and_registered_homes() {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: String| {
            let path = root.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        };
        let field = "games/THE LOST AGE/SRC/FIELD";
        write(
            &format!("{field}/COMMON/SCENE_TABLE.JSON"),
            json!({"segments":[{"records":[
                {"overlay":"0x650","area":9,"map_index":0},
                {"overlay":"0x650","area":9,"map_index":0},
                {"overlay":"0x651","area":10,"map_index":1},
                {"overlay":"0x652","area":11,"map_index":1}]}]})
            .to_string(),
        );
        write(
            &format!("{field}/COMMON/LOAD_TABLE.JSON"),
            json!({"records":[{"map_index":0,"container":"280"},{"map_index":1,"container":"26c"}]})
                .to_string(),
        );
        write(
            &format!("{field}/COMMON/NAME_RULES.JSON"),
            json!({"segments":[{"records":[
                {"key":1,"selector":[7,1],"name":1},
                {"key":9,"selector":[32767,0],"name":6},
                {"key":10,"selector":[32767,0],"name":8},
                {"key":11,"selector":[32767,0],"name":9}]}]})
            .to_string(),
        );
        write(
            &format!("{field}/IDEJIMA/IDEJIMA.JSON"),
            json!({"maps":{"280":{}}}).to_string(),
        );
        write(
            &format!("{field}/COMMON/MAP.JSON"),
            json!({"maps":{"26c":{}}}).to_string(),
        );
        write(
            "recon/tla/source-paths.json",
            json!({"owners":{"resource_651:02008000":{"source":"FIELD/DERI_MURA/A.C"}}})
                .to_string(),
        );
        write(
            "recon/tla/overlay-homes.tsv",
            "# homes\nresource_652\tSRC/FIELD/NIRI_HEYA\nresource_651\tSRC/ELSEWHERE\n".into(),
        );
        let tree = SourceTree::Work {
            id: "fixture".into(),
            root: root.path().into(),
        };
        let places = places(&tree, &target_for(DecompTargetId::TlaEn)).unwrap();
        let rows: Vec<_> = places
            .iter()
            .map(|p| {
                (
                    p.overlay,
                    p.scenes.clone(),
                    p.messages.clone(),
                    p.home.as_deref(),
                )
            })
            .collect();
        assert_eq!(
            rows,
            [
                (
                    0x650,
                    vec![0, 1],
                    vec![0xe5e, 0xe59],
                    Some("SRC/FIELD/IDEJIMA")
                ),
                (0x651, vec![2], vec![0xe60], Some("SRC/FIELD/DERI_MURA")),
                (0x652, vec![3], vec![0xe61], Some("SRC/FIELD/NIRI_HEYA")),
            ]
        );
    }
}
