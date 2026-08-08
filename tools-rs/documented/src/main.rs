// Every tool must be documented in AGENTS.md, or it must not exist.
//
//   cargo run -p documented
//   cargo run -p documented -- --self-test
use std::path::Path;
use std::process::ExitCode;

use documented::{documented, entry_points, violations};

fn self_test() {
    // PORT NOTE: the real suite is `cargo test`; this flag survives for parity
    // with `bun tools/check/documented.ts --self-test`, which callers and the
    // check runner may still invoke.
    let names = ["overlay", "lib", "gcc296", "verify.ts", "notes.md"]
        .iter()
        .map(|name| name.to_string())
        .collect::<Vec<_>>();
    let tools = entry_points(
        &names,
        &|name: &str| !name.contains('.'),
        &|name: &str| name == "overlay",
    );
    assert_eq!(tools.join(","), "overlay,verify", "entryPoints gave {tools:?}");
    let docs = documented(&format!(
        "## overlay\n\n{}\n\n## verify\n\ntoo short\n",
        "word ".repeat(30)
    ));
    assert!(
        docs.get("overlay").unwrap_or(0) >= documented::MINIMUM_WORDS,
        "a real paragraph must count"
    );
    let tools = ["overlay", "verify", "ghost"]
        .iter()
        .map(|name| name.to_string())
        .collect::<Vec<_>>();
    let problems = violations(&tools, &docs, Path::new("/nonexistent-root"));
    assert!(
        problems.iter().any(|p| p.starts_with("ghost:")),
        "an undocumented tool must fail"
    );
    assert!(
        problems.iter().any(|p| p.starts_with("verify:")),
        "a stub paragraph must fail"
    );
    assert!(
        !problems.iter().any(|p| p.starts_with("overlay:")),
        "a documented tool must pass"
    );
    assert!(
        documented(&format!("## overlay — the code overlays\n\n{}", "word ".repeat(30)))
            .get("overlay")
            .is_some(),
        "a heading with a dash suffix must still match"
    );
    println!("documented self-test ok");
}

fn main() -> ExitCode {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap();

    if std::env::args().any(|argument| argument == "--self-test") {
        self_test();
        return ExitCode::SUCCESS;
    }

    let tools_dir = root.join("tools");
    let mut names: Vec<String> = match std::fs::read_dir(&tools_dir) {
        Ok(entries) => entries
            .filter_map(|entry| entry.ok())
            .map(|entry| entry.file_name().to_string_lossy().into_owned())
            .collect(),
        Err(error) => {
            eprintln!("error: cannot read {}: {error}", tools_dir.display());
            return ExitCode::FAILURE;
        }
    };
    names.sort();

    let tools = entry_points(
        &names,
        &|name: &str| !name.contains('.'),
        &|name: &str| tools_dir.join(name).join("index.ts").exists(),
    );

    let markdown = match std::fs::read_to_string(root.join("AGENTS.md")) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("error: cannot read AGENTS.md: {error}");
            return ExitCode::FAILURE;
        }
    };

    let problems = violations(&tools, &documented(&markdown), root);
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        eprintln!(
            "\n{} tool(s) undocumented. AGENTS.md is the list; there is no other list.",
            problems.len()
        );
        return ExitCode::FAILURE;
    }
    println!("documented ok: {} tools, all in AGENTS.md", tools.len());
    ExitCode::SUCCESS
}
