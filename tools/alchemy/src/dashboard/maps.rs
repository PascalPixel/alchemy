//! Live field layers from the maintained scene index and verified local ROM.
mod client;
mod filter;
use super::{chrome, http, root, Response};
use crate::coverage::boxtree::esc;
use serde_json::Value;
use std::sync::Mutex;

/// The last few assembled room families, dropped when a watched input changes.
static FAMILY_CACHE: Mutex<Vec<(String, u64, Vec<u8>)>> = Mutex::new(Vec::new());

fn family(game: &str, scene: usize) -> Result<Vec<u8>, String> {
    let key = format!("{game}:{scene}");
    let generation = super::cache::generation();
    if let Some(bytes) = FAMILY_CACHE
        .lock()
        .unwrap_or_else(|error| error.into_inner())
        .iter()
        .find(|(cached, made, _)| cached == &key && *made == generation)
        .map(|(_, _, bytes)| bytes.clone())
    {
        return Ok(bytes);
    }
    let bytes = crate::build_assets::live_family(&root(), &format!("{game}-en"), scene)?;
    let mut cache = FAMILY_CACHE
        .lock()
        .unwrap_or_else(|error| error.into_inner());
    cache.retain(|(cached, made, _)| cached != &key && *made == generation);
    cache.push((key, generation, bytes.clone()));
    if cache.len() > 4 {
        cache.remove(0);
    }
    Ok(bytes)
}

pub(super) fn response(path: &str) -> Option<Response> {
    if path == "/maps/client.js" {
        return Some(Response::new(
            200,
            "OK",
            Some("text/javascript; charset=utf-8"),
            "no-store",
            client::CLIENT,
        ));
    }
    if path != "/maps" && !path.starts_with("/maps/") {
        return None;
    }
    if let Some(id) = path.strip_prefix("/maps/filter/") {
        return Some(match filter::PROFILES.iter().find(|p| p.id == id) {
            Some(profile) => Response::new(
                200,
                "OK",
                Some("application/octet-stream"),
                "no-store",
                filter::table(profile),
            ),
            None => http::not_found(),
        });
    }
    let parts = path.trim_matches('/').split('/').collect::<Vec<_>>();
    let game = parts.get(1).copied().unwrap_or("tbs");
    let title = match game {
        "tbs" => "THE BROKEN SEAL",
        "tla" => "THE LOST AGE",
        _ => return Some(http::not_found()),
    };
    let index = std::fs::read(
        root()
            .join("games")
            .join(title)
            .join("SRC/FIELD/COMMON/SCENE_TABLE.JSON"),
    )
    .ok()
    .and_then(|b| serde_json::from_slice::<Value>(&b).ok());
    let rows = index
        .as_ref()
        .and_then(|v| v["segments"][0]["records"].as_array());
    let scene = parts
        .get(2)
        .and_then(|s| s.parse::<usize>().ok())
        .unwrap_or(if game == "tbs" { 5 } else { 236 });
    if parts.get(3) == Some(&"layers") && parts.len() == 4 {
        if parts[2].parse::<usize>().is_err() {
            return Some(http::not_found());
        }
        let result = crate::build_assets::live_scene(&root(), &format!("{game}-en"), scene);
        return Some(match result {
            Ok(bytes) => Response::new(
                200,
                "OK",
                Some("application/octet-stream"),
                "no-store",
                bytes,
            ),
            Err(error) => Response::new(
                422,
                "Unprocessable Content",
                Some("text/plain; charset=utf-8"),
                "no-store",
                error,
            ),
        });
    }
    if parts.get(3) == Some(&"family") && parts.len() == 4 {
        if parts[2].parse::<usize>().is_err() {
            return Some(http::not_found());
        }
        return Some(match family(game, scene) {
            Ok(bytes) => Response::new(
                200,
                "OK",
                Some("application/octet-stream"),
                "no-store",
                bytes,
            ),
            Err(error) => Response::new(
                422,
                "Unprocessable Content",
                Some("text/plain; charset=utf-8"),
                "no-store",
                error,
            ),
        });
    }
    if parts.len() > 3 {
        return Some(http::not_found());
    }
    let mut options = String::new();
    for (id, row) in rows.into_iter().flatten().enumerate() {
        let container = row["map_index"].to_string();
        options.push_str(&format!(
            "<option value=\"{id}\"{}>Scene {id} · map {}</option>",
            if id == scene { " selected" } else { "" },
            esc(&container)
        ));
    }
    let filters = filter::PROFILES
        .iter()
        .map(|p| format!("<option value=\"{}\">{}</option>", p.id, esc(p.label)))
        .collect::<String>();
    let pressed = |id: &str| {
        if id == game {
            " aria-pressed=\"true\""
        } else {
            ""
        }
    };
    Some(chrome::page(
        path,
        &format!(
            r#"<main class="map-live" data-game="{game}" aria-label="{title} maps"><div class="toolbar"><a class="button" href="/maps/tbs"{}>The Broken Seal</a><a class="button" href="/maps/tla"{}>The Lost Age</a><span class="separator"></span><label>View <select id="mode"><option value="network">Connected rooms</option><option value="world">Stacked floors</option><option value="scene">Scene layers</option></select></label><label>Scene <select id="scene">{options}</select></label><span class="separator"></span><button id="fit">Fit</button><button id="actual">1×</button><button id="smaller" aria-label="Zoom out">−</button><button id="larger" aria-label="Zoom in">+</button><span class="separator"></span><label title="Measured screen colours from the Handheld Color Space Project">Filter <select id="filter"><option value="">None</option>{filters}</select></label><label class="scene-control"><input id="grid" type="checkbox">Grid</label><label class="scene-control">Palette <select id="palette"><option value="-1">Loaded palettes</option></select></label><span class="scene-control" id="layers"></span><a class="button refresh" href="{path}">Reload</a></div><div class="map-stage well"><canvas id="map" tabindex="0" aria-label="Interactive map: drag to pan; wheel or plus/minus to zoom; zero to fit"></canvas></div><div class="map-status"><span id="map-progress" class="progress busy" role="progressbar" aria-label="Map loading"><span></span></span><span id="map-status" role="status">Assembling rooms…</span><span class="map-inspector" id="inspect"></span></div></main><script type="module" src="/maps/client.js"></script>"#,
            pressed("tbs"),
            pressed("tla"),
        ),
        "Decoded and assembled live from the local verified ROM",
    ))
}

#[test]
fn rejects_invalid_map_routes() {
    for path in [
        "/maps/../../roms/tbs-en.gba",
        "/maps/unknown",
        "/maps/tbs/5/private",
        "/maps/tbs/not-a-scene/layers",
        "/maps/tbs/not-a-scene/family",
        "/maps/filter/unknown",
        "/maps/filter/../agb-001",
    ] {
        assert_eq!(response(path).unwrap().status, 404);
    }
}

#[test]
fn serves_one_colour_table_per_screen_filter() {
    for profile in &filter::PROFILES {
        let response = response(&format!("/maps/filter/{}", profile.id)).unwrap();
        assert_eq!(response.status, 200, "{}", profile.id);
        assert_eq!(response.body.len(), 32768 * 3, "{}", profile.id);
    }
}
