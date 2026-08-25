//! CLI for this crate, moved out of `main.rs` so the command can be linked
//! into a shared entry point instead of shipping its own executable.


use std::collections::BTreeSet;
use std::path::Path;
use std::process::ExitCode;

use compiler_core::routing::{cflags_for_source, cflags_for_target, uses_agbcc_compiler, CompilerTarget};
use compiler_core::source_paths::SourcePaths;

#[derive(Clone, Debug, Eq, Ord, PartialEq, PartialOrd)]
struct LiveSource {
    display: String,
    owner: Option<String>,
    routing: String,
}

/// Flags every Gs1 translation unit receives before per-source routing.
fn baseline() -> Vec<String> {
    cflags_for_target(CompilerTarget::Gs1)
}

/// The per-source flags that are not part of the baseline, in routed order.
fn routed_extras(routing: &str) -> Vec<String> {
    let base: BTreeSet<String> = baseline().into_iter().collect();
    cflags_for_source(routing).into_iter().filter(|flag| !base.contains(flag)).collect()
}

fn source_records(paths: Option<&SourcePaths>, source: &str) -> Vec<LiveSource> {
    let owners = paths.map(|paths| paths.owners_for_path(Path::new(source))).unwrap_or_default();
    if owners.is_empty() {
        return vec![LiveSource { display: source.to_string(), owner: None, routing: source.to_string() }];
    }
    owners.into_iter().map(|owner| LiveSource { display: source.to_string(), owner: Some(owner.id()), routing: owner.routing_path().to_string_lossy().into_owned() }).collect()
}

fn live_sources() -> Vec<LiveSource> {
    let mut found = Vec::new();
    for directory in ["games/gs1/semantic"] {
        let Ok(entries) = std::fs::read_dir(directory) else {
            continue;
        };
        for entry in entries.filter_map(Result::ok) {
            let path = entry.path();
            if path.extension().and_then(|e| e.to_str()) != Some("c") {
                continue;
            }
            if let Some(text) = path.to_str() {
                found.push(LiveSource { display: text.to_string(), owner: None, routing: text.to_string() });
            }
        }
    }
    if let Ok(paths) = SourcePaths::load(compiler_core::routing::root()) {
        if let Ok(sources) = paths.all_sources() {
            found.extend(sources.into_iter().map(|source| LiveSource { display: paths.repository_relative_path(source.owner).to_string_lossy().into_owned(), owner: Some(source.owner.id()), routing: source.owner.routing_path().to_string_lossy().into_owned() }));
        }
    }
    found.sort();
    found
}

fn emit(source: &LiveSource) {
    let extras = routed_extras(&source.routing);
    let agbcc = uses_agbcc_compiler(CompilerTarget::Gs1, &source.routing);
    println!("{}\t{}\t{}\t{}", source.display, source.owner.as_deref().unwrap_or("-"), if agbcc { "agbcc" } else { "gcc296" }, extras.join(" "));
}

fn self_test() -> Result<(), String> {
    let base = baseline();
    if !base.iter().any(|flag| flag == "-mthumb") {
        return Err("baseline lost -mthumb".to_string());
    }
    // A source with no routing entry must report no extras.
    if !routed_extras("games/gs1/recon/en/main/does_not_exist_08ffffff.c").is_empty() {
        return Err("an unrouted source reported routed flags".to_string());
    }
    // Routed extras must never duplicate a baseline flag.
    let baseline_set: BTreeSet<String> = base.into_iter().collect();
    for source in live_sources().into_iter().take(400) {
        for flag in routed_extras(&source.routing) {
            if baseline_set.contains(&flag) {
                return Err(format!("{}: {flag} is both baseline and routed", source.display));
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
        let deviating = sources.iter().filter(|source| !routed_extras(&source.routing).is_empty() || uses_agbcc_compiler(CompilerTarget::Gs1, &source.routing)).count();
        println!("routing debt: {deviating} of {} live sources deviate from the standard", sources.len());
        return ExitCode::SUCCESS;
    }
    if arguments.is_empty() || arguments.iter().any(|a| a == "-h" || a == "--help") {
        eprintln!("usage: route-dump <source>... | --all | --routed | --standard | --debt | --self-test");
        return ExitCode::FAILURE;
    }
    let only_routed = arguments.iter().any(|a| a == "--routed");
    let sources = if arguments.iter().any(|a| a == "--all" || a == "--routed") {
        live_sources()
    } else {
        let paths = SourcePaths::load(compiler_core::routing::root()).ok();
        arguments.iter().filter(|a| !a.starts_with("--")).flat_map(|source| source_records(paths.as_ref(), source)).collect()
    };
    for source in sources {
        if !Path::new(&source.display).exists() && !source.display.starts_with("games/gs1/recon/en/main/does_not_exist") {
            eprintln!("warning: {} does not exist on disk", source.display);
        }
        if only_routed && routed_extras(&source.routing).is_empty() {
            continue;
        }
        emit(&source);
    }
    ExitCode::SUCCESS
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn shared_source_emits_one_owner_specific_route_per_overlay() {
        let paths = SourcePaths::parse(
            Path::new("/repo"),
            r#"{
  "format": 3,
  "owners": {
    "resource_39b:02000104": "overlays/shared/integrate_effect_motion.c",
    "resource_39c:02000104": "overlays/shared/integrate_effect_motion.c"
  }
}"#,
        )
        .unwrap();
        let records = source_records(Some(&paths), "games/gs1/src/overlays/shared/integrate_effect_motion.c");
        assert_eq!(records.len(), 2);
        assert_eq!(records[0].owner.as_deref(), Some("resource_39b:02000104"));
        assert_eq!(records[0].routing, "games/gs1/src/resource_39b_c_02000104.c");
        assert_eq!(records[1].owner.as_deref(), Some("resource_39c:02000104"));
        assert_eq!(records[1].routing, "games/gs1/src/resource_39c_c_02000104.c");
    }
}
