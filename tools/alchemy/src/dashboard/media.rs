//! Local music previews. Only catalogued media are served, never arbitrary paths.
mod music;
use super::{cache, chrome, http, root, Response};
use crate::coverage::boxtree::esc;
use std::{
    path::{Path, PathBuf},
    sync::Mutex,
};

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
fn is_midi(path: &Path) -> bool {
    path.extension()
        .is_some_and(|e| e.eq_ignore_ascii_case("mid"))
}
/// The last few rendered previews, dropped whenever a watched input changes.
static AUDIO: Mutex<Vec<(PathBuf, u64, Result<Vec<u8>, Vec<u8>>)>> = Mutex::new(Vec::new());
fn audio(root: &Path, title: &str, file: &Path) -> Result<Vec<u8>, Vec<u8>> {
    let generation = cache::generation();
    let mut previews = AUDIO.lock().unwrap_or_else(|e| e.into_inner());
    previews.retain(|(_, made, _)| *made == generation);
    if let Some((_, _, audio)) = previews.iter().find(|(path, _, _)| path == file) {
        return audio.clone();
    }
    let audio = if is_midi(file) {
        music::render(root, title, file).map_err(String::into_bytes)
    } else {
        std::fs::read(file).map_err(|error| error.to_string().into_bytes())
    };
    previews.push((file.to_path_buf(), generation, audio.clone()));
    if previews.len() > 4 {
        let _ = previews.remove(0);
    }
    audio
}
fn button(href: &str, label: &str, pressed: bool) -> String {
    format!(
        "<a class=\"button\" href=\"{href}\"{}>{label}</a>",
        if pressed {
            " aria-pressed=\"true\""
        } else {
            ""
        }
    )
}
fn render(
    root: &Path,
    target: &str,
    title: &str,
    paths: &[PathBuf],
    chosen: Option<(&Path, &str)>,
    repeat: bool,
) -> (String, String) {
    let mut html = format!(
        "<main class=\"music\"><div class=\"toolbar\">{}{}</div><div class=\"split\"><nav class=\"well list\" aria-label=\"Music catalog\">",
        button("/music/tbs", "The Broken Seal", target == "tbs"),
        button("/music/tla", "The Lost Age", target == "tla"),
    );
    for file in paths {
        let key = encode(&file.strip_prefix(root).unwrap().to_string_lossy());
        html.push_str(&format!(
            "<a href=\"/music/{target}/{key}\"{}>{}</a>",
            if chosen.is_some_and(|(path, _)| path == file) {
                " aria-current=\"page\""
            } else {
                ""
            },
            esc(&file.file_name().unwrap().to_string_lossy())
        ));
    }
    html.push_str("</nav><section class=\"player\">");
    let note = match chosen {
        Some((file, key)) => {
            let name = file.file_name().unwrap().to_string_lossy();
            let base = format!("/music/{target}/{key}");
            html.push_str(&format!(
                "<fieldset class=\"group\"><legend>{}</legend>",
                esc(&name)
            ));
            match audio(root, title, file) {
                Err(error) => html.push_str(&format!(
                    "<p class=\"error\" role=\"alert\">Cannot preview: {}</p>",
                    esc(&String::from_utf8_lossy(&error))
                )),
                Ok(_) => html.push_str(&format!(
                    "<audio controls preload=\"none\"{} src=\"{base}/media\"></audio><div class=\"buttons\">{}</div>",
                    if repeat { " loop" } else { "" },
                    button(
                        &if repeat { base.clone() } else { format!("{base}/repeat") },
                        "Repeat",
                        repeat
                    )
                )),
            }
            html.push_str("</fieldset>");
            if is_midi(file) {
                "MIDI · approximate synthesis from recovered instruments · first 60 s"
            } else {
                "Sample · played as stored"
            }
        }
        None => "Select a sequence or sample",
    };
    html.push_str("</section></div></main>");
    (html, format!("{} · {} files", note, paths.len()))
}
pub(super) fn response(path: &str) -> Option<Response> {
    let parts = path.trim_start_matches('/').split('/').collect::<Vec<_>>();
    if parts.first() != Some(&"music") {
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
        return Some(match audio(&root, title, file) {
            Ok(data) => Response::new(200, "OK", Some("audio/wav"), "no-store", data),
            Err(error) => Response::new(
                422,
                "Unprocessable Content",
                Some("text/plain; charset=utf-8"),
                "no-store",
                error,
            ),
        });
    }
    let repeat = parts.get(3) == Some(&"repeat");
    if parts.len() == 4 && !repeat {
        return Some(http::not_found());
    }
    let (html, note) = cache::view::<()>(path, || {
        Ok(render(
            &root,
            target,
            title,
            &paths,
            chosen.as_deref().zip(key),
            repeat,
        ))
    })
    .ok()?;
    Some(chrome::page(path, &html, &note))
}

#[cfg(test)]
mod tests {
    use super::*;
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
        let key = encode("games/THE BROKEN SEAL/SOUND/SAMPLE/WAVE.WAV");
        assert!(selected(root, &list, &key).is_some());
        assert!(selected(
            root,
            &list,
            &encode("games/THE BROKEN SEAL/SOUND/SAMPLE/PRIVATE.JSON")
        )
        .is_none());
        assert!(selected(root, &list, &encode("../../roms/tbs-en.gba")).is_none());
        assert!(game("../tbs").is_none());
        let (html, note) = render(
            root,
            "tbs",
            "THE BROKEN SEAL",
            &list,
            Some((&list[0], &key)),
            true,
        );
        assert!(html.contains("<audio controls preload=\"none\" loop"));
        assert!(html.contains("aria-pressed=\"true\">Repeat</a>"));
        assert!(!html.contains("autoplay"));
        assert_eq!(note, "Sample · played as stored · 1 files");
    }
}
