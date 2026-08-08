use std::path::Path;
use std::process::ExitCode;

use documented::{documented, entry_points, scanned_nothing, violations};

fn main() -> ExitCode {
    let root = Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("documented must live under tools");
    let tools = entry_points();
    let markdown = match std::fs::read_to_string(root.join("AGENTS.md")) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("error: cannot read AGENTS.md: {error}");
            return ExitCode::FAILURE;
        }
    };
    let docs = documented(&markdown);
    if let Some(why) = scanned_nothing(tools.len(), docs.len()) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }
    let problems = violations(&tools, &docs);
    if !problems.is_empty() {
        for problem in &problems {
            eprintln!("error: {problem}");
        }
        eprintln!(
            "\n{} native tool group(s) are undocumented. AGENTS.md is the list; there is no other list.",
            problems.len()
        );
        return ExitCode::FAILURE;
    }
    println!(
        "documented ok: {} native tool groups, all in AGENTS.md",
        tools.len()
    );
    ExitCode::SUCCESS
}
