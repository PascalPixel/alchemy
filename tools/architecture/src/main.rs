use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

use architecture::{
    authoritative_sources, dispatch_targets, native_command_roots, native_paths, scan_crates,
    scanned_nothing, unreachable_crates, valid_dispatch_targets, valid_paths, workspace_members,
};
use documented::{documented, entry_points, scanned_nothing as documented_scan, violations};

fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("architecture must live under tools")
        .to_path_buf()
}

fn markdown(root: &Path) -> Vec<(String, String)> {
    let mut files = Vec::new();
    for directory in [root.to_path_buf(), root.join("docs")] {
        let Ok(entries) = fs::read_dir(&directory) else {
            continue;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.extension().and_then(|extension| extension.to_str()) != Some("md") {
                continue;
            }
            let name = path
                .strip_prefix(root)
                .unwrap_or(&path)
                .to_string_lossy()
                .replace('\\', "/");
            if let Ok(text) = fs::read_to_string(&path) {
                files.push((name, text));
            }
        }
    }
    files.sort_by(|left, right| left.0.as_bytes().cmp(right.0.as_bytes()));
    files
}

fn main() -> ExitCode {
    if std::env::args().any(|argument| argument == "--self-test") {
        return match architecture::self_test() {
            Ok(()) => {
                println!("architecture self-test ok");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }

    let root = root();
    let crates = match scan_crates(&root) {
        Ok(crates) => crates,
        Err(error) => {
            eprintln!("error: {error}");
            return ExitCode::FAILURE;
        }
    };
    let markdown = markdown(&root);
    let tools = entry_points();
    let targets = dispatch_targets();
    if let Some(why) = scanned_nothing(crates.len(), targets.len(), markdown.len()) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }
    if let Some(why) = documented_scan(tools.len(), documented(&read_agents(&root)).len()) {
        eprintln!("error: {why}");
        return ExitCode::FAILURE;
    }

    let mut problems = Vec::new();
    problems.extend(crates.iter().flat_map(|native| {
        native
            .invalid_paths
            .iter()
            .map(|path| format!("{path} is missing"))
    }));
    let (mut roots, target_problems) = valid_dispatch_targets(&targets, &crates);
    problems.extend(target_problems);
    problems.extend(violations(&tools, &documented(&read_agents(&root))));

    let sources = authoritative_sources(&root, &markdown);
    problems.extend(valid_paths(&root, &sources, &crates));
    for text in sources.iter().map(|(_, text)| text) {
        for path in native_paths(text) {
            let candidate = if path.starts_with("tools/target/release/") {
                architecture::target_crate(&path, &crates).ok()
            } else {
                path.strip_prefix("tools/")
                    .and_then(|path| path.split('/').next())
                    .filter(|name| crates.iter().any(|native| native.directory == *name))
                    .map(str::to_string)
            };
            if let Some(candidate) = candidate {
                roots.insert(candidate);
            }
        }
    }
    let workspace_manifest = fs::read_to_string(root.join("tools/Cargo.toml")).unwrap_or_default();
    roots.extend(workspace_members(&workspace_manifest));
    roots.extend(native_command_roots(&root, &crates));
    problems.extend(unreachable_crates(&crates, &roots));

    if !problems.is_empty() {
        for problem in problems {
            eprintln!("error: {problem}");
        }
        return ExitCode::FAILURE;
    }
    println!(
        "architecture ok: {} native tools, {} dispatch entries, {} crates, all paths valid and reachable",
        tools.len(),
        targets.len(),
        crates.len()
    );
    ExitCode::SUCCESS
}

fn read_agents(root: &Path) -> String {
    fs::read_to_string(root.join("AGENTS.md")).unwrap_or_default()
}
