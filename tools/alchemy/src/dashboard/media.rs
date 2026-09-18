//! Optional local previews. Only catalogued media are served, never arbitrary paths.
mod music;
use super::{document, http, root, Response};
use crate::coverage::boxtree::esc;
use std::path::{Path, PathBuf};

fn game(id: &str) -> Option<&'static str> {
    match id {
        "tbs" => Some("THE BROKEN SEAL"),
        "tla" => Some("THE LOST AGE"),
        _ => None,
    }
}
fn encode(path: &str) -> String {
    path.bytes().map(|b| format!("{b:02x}")).collect()
}
fn files(directory: &Path, extension: &str) -> Vec<PathBuf> {
    let mut paths = walkdir::WalkDir::new(directory)
        .follow_links(false)
        .into_iter()
        .filter_map(Result::ok)
        .filter(|e| {
            e.file_type().is_file()
                && e.path()
                    .extension()
                    .is_some_and(|x| x.eq_ignore_ascii_case(extension))
        })
        .map(|e| e.into_path())
        .collect::<Vec<_>>();
    paths.sort();
    paths
}
fn catalog(root: &Path, target: &str) -> Vec<PathBuf> {
    let Some(game) = game(target) else {
        return vec![];
    };
    let sound = root.join("games").join(game).join("SOUND");
    let mut paths = files(&sound.join("SEQUENCE"), "mid");
    paths.extend(files(&sound.join("SAMPLE"), "wav"));
    paths
}
fn selected(root: &Path, paths: &[PathBuf], key: &str) -> Option<PathBuf> {
    paths
        .iter()
        .find(|path| {
            path.strip_prefix(root)
                .ok()
                .is_some_and(|p| encode(&p.to_string_lossy()) == key)
        })
        .cloned()
}
fn bytes(path: &Path, mime: &'static str) -> Response {
    match std::fs::read(path) {
        Ok(data) => Response::new(200, "OK", Some(mime), "no-store", data),
        Err(_) => http::not_found(),
    }
}
pub(super) fn response(path: &str) -> Option<Response> {
    let parts = path.trim_start_matches('/').split('/').collect::<Vec<_>>();
    let view = *parts.first()?;
    if view != "music" {
        return None;
    }
    let target = parts.get(1).copied().unwrap_or("tbs");
    let Some(title) = game(target) else {
        return Some(http::not_found());
    };
    if parts.len() > 4 {
        return Some(http::not_found());
    }
    let root = root();
    let paths = catalog(&root, target);
    let key = parts.get(2).copied();
    let chosen = key.and_then(|key| selected(&root, &paths, key));
    if key.is_some() && chosen.is_none() {
        return Some(http::not_found());
    }
    if parts.get(3) == Some(&"media") {
        let file = chosen.as_ref()?;
        return Some(
            if file
                .extension()
                .is_some_and(|e| e.eq_ignore_ascii_case("wav"))
            {
                bytes(file, "audio/wav")
            } else {
                match music::render(&root, game(target).unwrap(), file) {
                    Ok(data) => Response::new(200, "OK", Some("audio/wav"), "no-store", data),
                    Err(error) => Response::new(
                        422,
                        "Unprocessable Content",
                        Some("text/plain; charset=utf-8"),
                        "no-store",
                        error,
                    ),
                }
            },
        );
    }
    let repeat = parts.get(3) == Some(&"repeat");
    if parts.len() == 4 && !repeat {
        return Some(http::not_found());
    }
    let mut html=format!("<main><header><span>{title} · {} debugging</span><a class=\"refresh\" href=\"{path}\">Refresh</a></header><div class=\"debug-body\"><section class=\"debug-list\" aria-label=\"{} catalog\"><a href=\"/{view}/tbs\">The Broken Seal</a><a href=\"/{view}/tla\">The Lost Age</a><hr>",if view=="music" {"Music"} else {"Map"},view);
    for file in &paths {
        let key = encode(&file.strip_prefix(&root).unwrap().to_string_lossy());
        html.push_str(&format!(
            "<a href=\"/{view}/{target}/{key}\"{}>{}</a>",
            if chosen.as_ref() == Some(file) {
                " aria-current=\"page\""
            } else {
                ""
            },
            esc(&file.file_name().unwrap().to_string_lossy())
        ));
    }
    html.push_str("</section><section class=\"debug-detail\">");
    if let Some(file) = chosen {
        let base = format!("/{view}/{target}/{}", key.unwrap());
        let src = format!("{base}/media");
        html.push_str(&format!(
            "<h2>{}</h2>",
            esc(&file.file_name().unwrap().to_string_lossy())
        ));
        html.push_str("<p>Local audio preview. MIDI uses recovered instruments and local ROM samples where needed, but envelopes, modulation and mixing are approximate—not the in-game engine. Preview is limited to 60 seconds. Samples play directly.</p>");
        let error = if file
            .extension()
            .is_some_and(|e| e.eq_ignore_ascii_case("mid"))
        {
            music::render(&root, title, &file).err()
        } else {
            None
        };
        if let Some(error) = error {
            html.push_str(&format!(
                "<p role=\"alert\">Cannot preview this sequence: {}</p>",
                esc(&error)
            ));
        } else {
            html.push_str(&format!("<audio controls preload=\"none\"{} src=\"{src}\">Your browser does not support audio playback.</audio><p><a href=\"{}\">Repeat: {}</a> · Use the player to play, pause and seek. No autoplay.</p>",if repeat {" loop"}else{""},if repeat {base.clone()}else{format!("{base}/repeat")},if repeat {"on"}else{"off"}));
        }
    } else if paths.is_empty() {
        html.push_str("<p>No recovered sequence or sample files are currently present for this game. No substitute soundtrack is used.</p>");
    } else {
        html.push_str(&format!("<p>Select {} from the list. {} local files.</p><p>Previews stay private; they do not change DONE.</p>",if view=="music" {"a sequence or sample"}else{"a map"},paths.len()));
    }
    html.push_str("</section></div></main>");
    Some(document(path, &html))
}

#[test]
fn catalog_selection_cannot_escape_allowlist() {
    let tmp = tempfile::tempdir().unwrap();
    let root = tmp.path();
    let dir = root.join("games/THE BROKEN SEAL/SOUND/SAMPLE");
    std::fs::create_dir_all(&dir).unwrap();
    std::fs::write(dir.join("WAVE.WAV"), b"fixture").unwrap();
    std::fs::write(dir.join("PRIVATE.JSON"), b"private").unwrap();
    let list = catalog(root, "tbs");
    assert_eq!(list.len(), 1);
    assert!(selected(
        root,
        &list,
        &encode("games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE.WAV")
    )
    .is_some());
    assert!(selected(
        root,
        &list,
        &encode("games/THE BROKEN SEAL/SOUND/SAMPLE/PRIVATE.JSON")
    )
    .is_none());
    assert!(selected(root, &list, &encode("../../roms/tbs-en.gba")).is_none());
    assert!(game("../tbs").is_none());
}
