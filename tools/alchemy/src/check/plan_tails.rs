//! A compression plan may keep at most three trailing stream bytes as
//! `lookahead` until the encoder derives them; a longer tail is ROM data.

use serde_json::Value;
use std::path::Path;
use std::process::ExitCode;

const USAGE: &str = "usage: alchemy check plan-tails\n\nReject any lookahead in tracked game JSON that is not a hex string of at most three bytes.";
/// The most trailing stream bytes a plan may record.
const TAIL_BYTES_MAX: usize = 3;

fn valid_tail(value: &Value) -> bool {
    value.as_str().is_some_and(|text| {
        text.len().is_multiple_of(2)
            && text.len() <= 2 * TAIL_BYTES_MAX
            && text.bytes().all(|byte| byte.is_ascii_hexdigit())
    })
}

/// JSON pointers to every `lookahead` in `document` that breaks the cap.
fn long_tails(document: &Value) -> Vec<String> {
    fn walk(value: &Value, pointer: &mut String, found: &mut Vec<String>) {
        let children: Vec<(String, &Value)> = match value {
            Value::Object(object) => object
                .iter()
                .map(|(key, child)| (key.replace('~', "~0").replace('/', "~1"), child))
                .collect(),
            Value::Array(items) => items
                .iter()
                .enumerate()
                .map(|(index, child)| (index.to_string(), child))
                .collect(),
            _ => return,
        };
        for (key, child) in children {
            let length = pointer.len();
            pointer.push('/');
            pointer.push_str(&key);
            if key == "lookahead" && value.is_object() && !valid_tail(child) {
                found.push(pointer.clone());
            }
            walk(child, pointer, found);
            pointer.truncate(length);
        }
    }
    let mut found = Vec::new();
    walk(document, &mut String::new(), &mut found);
    found
}

/// Check every tracked game JSON file that mentions a lookahead.
fn check(root: &Path) -> Result<usize, String> {
    let files =
        super::tracked_game_files(root, |path| path.to_ascii_lowercase().ends_with(".json"))?;
    let mut failures = Vec::new();
    let mut planned = 0;
    for (path, data) in files {
        if !data.windows(9).any(|window| window == b"lookahead") {
            continue;
        }
        planned += 1;
        match serde_json::from_slice::<Value>(&data) {
            Ok(document) => failures.extend(long_tails(&document).into_iter().map(|pointer| {
                format!("{path}#{pointer}: lookahead must be at most {TAIL_BYTES_MAX} hex bytes")
            })),
            Err(error) => failures.push(format!("{path}: {error}")),
        }
    }
    if failures.is_empty() {
        Ok(planned)
    } else {
        Err(failures.join("\n"))
    }
}

pub(super) fn entry(arguments: &[String]) -> ExitCode {
    if !arguments.is_empty() {
        eprintln!("{USAGE}");
        return ExitCode::from(2);
    }
    super::report(check(crate::compiler::routing::root()).map(|files| {
        println!("plan tails ok: files={files} lookahead<={TAIL_BYTES_MAX} bytes");
    }))
}

#[test]
fn lookahead_tails_are_capped_at_three_hex_bytes() {
    let tail =
        |value: Value| serde_json::json!({"codec": "golden-sun-general-lz", "lookahead": value});
    let document = serde_json::json!({
        "recipes": {"a/b": tail("".into()), "c": tail("0aF1".into()), "d": tail("00ff10".into())},
        "streams": [tail("00ff1022".into()), [tail("0".into()), tail("zz".into())], tail(serde_json::json!([1, 2]))],
        "notes": ["lookahead"]
    });
    assert_eq!(
        long_tails(&document),
        [
            "/streams/0/lookahead",
            "/streams/1/0/lookahead",
            "/streams/1/1/lookahead",
            "/streams/2/lookahead"
        ]
    );
    assert!(
        long_tails(&serde_json::json!({"recipes": {"a~b/c": tail("aabbccdd".into())}}))
            .contains(&"/recipes/a~0b~1c/lookahead".to_string())
    );
}

#[test]
fn tracked_plans_are_checked_and_untracked_ones_are_not() {
    let long = r#"{"codec": "golden-sun-general-lz", "lookahead": "0102030405"}"#;
    let short = r#"{"codec": "golden-sun-general-lz", "lookahead": "010203"}"#;
    let root = super::fixture_repository(&[
        (
            "games/X/asm/overlays/resource_1_stream.lz.json",
            short,
            true,
        ),
        ("games/X/SRC/GRAPHICS/COMMON/COMPRESSION.JSON", long, true),
        ("games/X/SRC/UNTRACKED.JSON", long, false),
        ("tools/PLAN.JSON", long, true),
    ]);
    let error = check(root.path()).unwrap_err();
    assert_eq!(
        error,
        "games/X/SRC/GRAPHICS/COMMON/COMPRESSION.JSON#/lookahead: lookahead must be at most 3 hex bytes"
    );
    std::fs::write(
        root.path()
            .join("games/X/SRC/GRAPHICS/COMMON/COMPRESSION.JSON"),
        short,
    )
    .unwrap();
    assert_eq!(check(root.path()), Ok(2));
}
