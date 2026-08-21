//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.


use std::collections::BTreeSet;
use std::path::Path;
use std::process::ExitCode;

use compiler_core::routing::{
    cflags_for_source, cflags_for_target, uses_agbcc_compiler, CompilerTarget,
};

/// Flags every Gs1 translation unit receives before per-source routing.
fn baseline() -> Vec<String> {
    cflags_for_target(CompilerTarget::Gs1)
}

/// The per-source flags that are not part of the baseline, in routed order.
fn routed_extras(source: &str) -> Vec<String> {
    let base: BTreeSet<String> = baseline().into_iter().collect();
    cflags_for_source(source)
        .into_iter()
        .filter(|flag| !base.contains(flag))
        .collect()
}

fn live_sources() -> Vec<String> {
    let mut found = Vec::new();
    for directory in ["semantic", "exact"] {
        let Ok(entries) = std::fs::read_dir(directory) else {
            continue;
        };
        for entry in entries.filter_map(Result::ok) {
            let path = entry.path();
            if path.extension().and_then(|e| e.to_str()) != Some("c") {
                continue;
            }
            if let Some(text) = path.to_str() {
                found.push(text.to_string());
            }
        }
    }
    found.sort();
    found
}

fn emit(source: &str) {
    let extras = routed_extras(source);
    let agbcc = uses_agbcc_compiler(CompilerTarget::Gs1, source);
    println!(
        "{source}\t{}\t{}",
        if agbcc { "agbcc" } else { "gcc296" },
        extras.join(" ")
    );
}

fn self_test() -> Result<(), String> {
    let base = baseline();
    if !base.iter().any(|flag| flag == "-mthumb") {
        return Err("baseline lost -mthumb".to_string());
    }
    // A source with no routing entry must report no extras.
    if !routed_extras("recon/gs1/en/main/does_not_exist_08ffffff.c").is_empty() {
        return Err("an unrouted source reported routed flags".to_string());
    }
    // Routed extras must never duplicate a baseline flag.
    let baseline_set: BTreeSet<String> = base.into_iter().collect();
    for source in live_sources().into_iter().take(400) {
        for flag in routed_extras(&source) {
            if baseline_set.contains(&flag) {
                return Err(format!("{source}: {flag} is both baseline and routed"));
            }
        }
    }
    Ok(())
}

pub fn entry(arguments: &[String]) -> ExitCode {
    let arguments: Vec<String> = arguments.to_vec();
    if arguments.iter().any(|a| a == "--self-test") {
        return match self_test() {
            Ok(()) => {
                println!("route-dump self-test ok");
                ExitCode::SUCCESS
            }
            Err(error) => {
                eprintln!("error: {error}");
                ExitCode::FAILURE
            }
        };
    }
    if arguments.iter().any(|a| a == "--standard") {
        for flag in baseline() {
            println!("{flag}");
        }
        return ExitCode::SUCCESS;
    }
    if arguments.iter().any(|a| a == "--debt") {
        let sources = live_sources();
        let deviating = sources
            .iter()
            .filter(|source| {
                !routed_extras(source).is_empty()
                    || uses_agbcc_compiler(CompilerTarget::Gs1, source)
            })
            .count();
        println!(
            "routing debt: {deviating} of {} live sources deviate from the standard",
            sources.len()
        );
        return ExitCode::SUCCESS;
    }
    if arguments.is_empty() || arguments.iter().any(|a| a == "-h" || a == "--help") {
        eprintln!(
            "usage: route-dump <source>... | --all | --routed | --standard | --debt | --self-test"
        );
        return ExitCode::FAILURE;
    }
    let only_routed = arguments.iter().any(|a| a == "--routed");
    let sources = if arguments.iter().any(|a| a == "--all" || a == "--routed") {
        live_sources()
    } else {
        arguments
            .iter()
            .filter(|a| !a.starts_with("--"))
            .cloned()
            .collect()
    };
    for source in sources {
        if !Path::new(&source).exists() && !source.starts_with("recon/gs1/en/main/does_not_exist") {
            eprintln!("warning: {source} does not exist on disk");
        }
        if only_routed && routed_extras(&source).is_empty() {
            continue;
        }
        emit(&source);
    }
    ExitCode::SUCCESS
}
