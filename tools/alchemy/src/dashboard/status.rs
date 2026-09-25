//! Each game's DONE for the status bar, read only from its verified build
//! receipt and verified executable inventory. Anything short of both is `?`.
use super::cache;
use crate::coverage::boxtree::esc;
use crate::coverage::jsnum::commas;
use crate::coverage::progress::{measured, GameDone};
use serde_json::{json, Value};
use std::{path::Path, sync::Mutex};

/// Each game's edition, Weyard UI mark and its colour, and title.
pub(super) const GAMES: [(&str, (&str, &str), &str); 2] = [
    ("tbs-en", ("tbs", "gold"), "The Broken Seal"),
    ("tla-en", ("tla", "blue"), "The Lost Age"),
];

#[derive(Clone, Debug, PartialEq)]
pub(super) enum Done {
    /// The receipts have not been read yet this session.
    Checking,
    Measured(GameDone),
    /// Pending or unreadable evidence; the reason is shown, never a number.
    Unknown(String),
}
static DONE: Mutex<[Option<Done>; 2]> = Mutex::new([None, None]);

fn reason(root: &Path, target: &str, error: Option<String>) -> String {
    let inventory = root.join(format!("out/{target}/reports/executable.json"));
    let pending = std::fs::read(&inventory)
        .ok()
        .and_then(|bytes| serde_json::from_slice::<Value>(&bytes).ok())
        .and_then(|document| {
            document["pending"].as_array().map(|reasons| {
                reasons
                    .iter()
                    .filter_map(Value::as_str)
                    .collect::<Vec<_>>()
                    .join("; ")
            })
        })
        .filter(|text| !text.is_empty());
    match (error, pending) {
        (Some(error), _) => error,
        (None, Some(pending)) => format!("executable audit pending: {pending}"),
        (None, None) => "executable audit pending".into(),
    }
}
/// Re-read both games' receipts; a background job, since verifying a receipt
/// hashes every build input.
pub(super) fn refresh(root: &Path) -> Result<String, String> {
    let mut notes = Vec::new();
    for (index, (target, _, title)) in GAMES.iter().enumerate() {
        cache::advance("progress", index, format!("Checking {target}"));
        let done = match measured(root, target) {
            Ok(Some(done)) => Done::Measured(done),
            Ok(None) => Done::Unknown(reason(root, target, None)),
            Err(error) => Done::Unknown(reason(root, target, Some(error))),
        };
        notes.push(format!("{title} {}", label(&done)));
        DONE.lock().unwrap_or_else(|e| e.into_inner())[index] = Some(done);
    }
    Ok(notes.join(" "))
}
fn label(done: &Done) -> String {
    match done {
        Done::Measured(done) => format!("{:.2}%", done.percent()),
        Done::Checking => "…".into(),
        Done::Unknown(_) => "?".into(),
    }
}
fn detail(title: &str, done: &Done) -> String {
    match done {
        Done::Measured(d) => format!(
            "{title} DONE: {} / {} executable bytes · exact C {} · permanent assembly {}",
            commas(d.bytes()),
            commas(d.executable),
            commas(d.common_c + d.game_c),
            commas(d.common_asm + d.game_asm)
        ),
        Done::Checking => format!("{title}: reading the verified build receipt"),
        Done::Unknown(reason) => format!("{title} DONE is ?: {reason}"),
    }
}
fn current() -> [Done; 2] {
    let done = DONE.lock().unwrap_or_else(|e| e.into_inner());
    [0, 1].map(|index| done[index].clone().unwrap_or(Done::Checking))
}
/// Status-bar fields, one per game.
pub(super) fn fields() -> String {
    current()
        .iter()
        .zip(GAMES)
        .map(|(done, (_, (mark, color), title))| {
            format!(
                "<span class=\"field done\" title=\"{}\">{}<span class=\"sr\">{title}</span>{}</span>",
                esc(&detail(title, done)),
                super::chrome::logo(mark, color),
                label(done)
            )
        })
        .collect()
}
pub(super) fn snapshot() -> Value {
    let mut games = serde_json::Map::new();
    for (done, (target, _, _)) in current().iter().zip(GAMES) {
        games.insert(
            target.into(),
            match done {
                Done::Measured(d) => json!({"state":"verified","donePercent":d.percent(),
                    "doneBytes":d.bytes(),"executableBytes":d.executable,
                    "exactCBytes":d.common_c + d.game_c,
                    "permanentAssemblyBytes":d.common_asm + d.game_asm}),
                Done::Checking => json!({"state":"checking","donePercent":null}),
                Done::Unknown(reason) => {
                    json!({"state":"pending","donePercent":null,"reason":reason})
                }
            },
        );
    }
    Value::Object(games)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn pending_inventories_show_their_reasons_and_never_a_number() {
        let root = tempfile::tempdir().unwrap();
        let reports = root.path().join("out/tbs-en/reports");
        std::fs::create_dir_all(&reports).unwrap();
        std::fs::write(
            reports.join("executable.json"),
            br#"{"state":"pending","pending":["the main image needs a full build"]}"#,
        )
        .unwrap();
        assert_eq!(
            reason(root.path(), "tbs-en", None),
            "executable audit pending: the main image needs a full build"
        );
        assert_eq!(
            reason(root.path(), "tla-en", Some("receipt missing".into())),
            "receipt missing"
        );
        let unknown = Done::Unknown("receipt missing".into());
        assert_eq!(label(&unknown), "?");
        assert!(detail("The Lost Age", &unknown).contains("DONE is ?"));
        let measured = Done::Measured(GameDone {
            game_c: 25,
            game_asm: 25,
            executable: 100,
            ..GameDone::default()
        });
        assert_eq!(label(&measured), "50.00%");
    }
}
