//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.

use std::fs;
use std::path::Path;
use std::process::ExitCode;

use crate::{buckets, classify, render, source_path};

const USAGE: &str = "Usage: remaining-survey [--help|-h] [--self-test]\n\nClassify remaining c_candidate assembly regions.\n";

fn repository_root() -> &'static Path {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .expect("the crate must live at tools/remaining-survey")
}

fn self_test() -> Result<(), String> {
    if classify("\tmov\tip, pc\n") != 0 {
        return Err("ip return-address classification is wrong".into());
    }
    if classify("\tmovs\tr0, #0\n") != 6 {
        return Err("plain classification is wrong".into());
    }
    println!("self-test=ok");
    Ok(())
}

fn validate_arguments(arguments: &[String]) -> Result<(), String> {
    if let Some(argument) = arguments
        .iter()
        .find(|argument| !matches!(argument.as_str(), "-h" | "--help" | "--self-test"))
    {
        return Err(format!("unknown option: {argument}\n{USAGE}"));
    }
    Ok(())
}

fn run(arguments: &[String]) -> Result<(), String> {
    validate_arguments(arguments)?;
    if arguments
        .iter()
        .any(|argument| matches!(argument.as_str(), "-h" | "--help"))
    {
        print!("{USAGE}");
        return Ok(());
    }
    if arguments.iter().any(|argument| argument == "--self-test") {
        return self_test();
    }

    let root = repository_root();
    let manifest_path = root.join("out/full/asm/manifest.json");
    let raw = fs::read_to_string(&manifest_path)
        .unwrap_or_else(|e| panic!("cannot read {}: {e}", manifest_path.display()));
    let manifest: serde_json::Value =
        serde_json::from_str(&raw).expect("manifest.json is not valid JSON");

    let mut buckets = buckets();
    let regions = manifest["regions"].as_array().cloned().unwrap_or_default();
    for region in regions {
        if region["retention"].as_str() != Some("c_candidate") {
            continue;
        }
        let Some(source) = region["source"].as_str() else {
            continue;
        };
        let path = source_path(root, source);
        if !path.exists() {
            continue;
        }
        let bytes = fs::read(&path).unwrap_or_else(|e| panic!("cannot read {source}: {e}"));
        let text = String::from_utf8_lossy(&bytes);
        let stem = Path::new(source)
            .file_name()
            .map(|n| n.to_string_lossy().to_string())
            .unwrap_or_default();
        let stem = stem.strip_suffix(".s").unwrap_or(&stem).to_string();
        buckets[classify(&text)].stems.push(stem);
    }

    print!("{}", render(&buckets));
    Ok(())
}

pub fn entry(arguments: &[String]) -> std::process::ExitCode {
    let arguments: Vec<String> = arguments.to_vec();
    match run(&arguments) {
        Ok(()) => ExitCode::SUCCESS,
        Err(message) => {
            eprintln!("error: {message}");
            ExitCode::FAILURE
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn unknown_options_are_rejected_before_the_scan() {
        let error = validate_arguments(&["--not-an-option".into()]).unwrap_err();
        assert!(error.contains("unknown option: --not-an-option"));
    }

    #[test]
    fn documented_arguments_remain_valid() {
        validate_arguments(&["--help".into()]).unwrap();
        validate_arguments(&["--self-test".into()]).unwrap();
    }
}
