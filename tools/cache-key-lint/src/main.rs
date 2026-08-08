use cache_key_lint::{find_violations, scannable_files, Finding};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::ExitCode;

fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .to_path_buf()
}

fn scan(root: &Path, directory: &str) -> Result<(usize, Vec<Finding>), String> {
    let base = root.join(directory);
    let names = scannable_files(&base).map_err(|error| format!("{directory}: {error}"))?;
    let mut findings = Vec::new();
    for name in &names {
        let text = fs::read_to_string(base.join(name))
            .map_err(|error| format!("{directory}/{name}: {error}"))?;
        findings.extend(find_violations(&format!("{directory}/{name}"), &text));
    }
    Ok((names.len(), findings))
}

fn main() -> ExitCode {
    let root = repository_root();
    let mut scanned = 0;
    let mut findings = Vec::new();
    match scan(&root, "tools") {
        Ok((count, found)) => {
            scanned += count;
            findings.extend(found);
        }
        Err(message) => {
            eprintln!("{message}");
            return ExitCode::FAILURE;
        }
    }

    for finding in &findings {
        println!("{}:{}  {}", finding.file, finding.line, finding.text);
    }
    println!("\nscanned={scanned} violations={}", findings.len());

    // SCANNING NOTHING IS NOT PASSING. `violations=0` over an empty file list
    // reads exactly like a clean run, and a lint that can pass without looking
    // is the defect it polices.
    if scanned == 0 {
        println!(
            "NOTHING SCANNED — this is a FAILURE, not a pass.\n  No Rust sources under {}.",
            root.display()
        );
        return ExitCode::FAILURE;
    }
    if !findings.is_empty() {
        println!(
            "HAND-MAINTAINED CACHE KEY — this is a FAILURE.\n\
             \x20 A cache key must derive from its inputs, not from a literal describing\n\
             \x20 the logic. Mix in a digest of the tool's own source instead:\n\
             \x20     hasher.update(format!(\"name:{{}}:...\", self_digest()).as_bytes());\n\
             \x20 See the native overlay-disasm crate and HANDOVER §5h for why."
        );
        return ExitCode::FAILURE;
    }
    ExitCode::SUCCESS
}
