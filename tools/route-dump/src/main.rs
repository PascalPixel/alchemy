//! Print the compiler flags a source is actually routed with.
//!
//! The routing tables are the source of truth for how each translation unit is
//! built, but that truth is only reachable by calling into the routing crate:
//! flags are pushed from several distinct code shapes, and the agbcc family
//! routes through a different function entirely. Reconstructing the mapping by
//! reading `routing.rs` with a text scanner silently under-matches, and the
//! missing flags then read as "this source has no routing" rather than as a
//! gap in the scan. That mistake produced a corpus-wide measurement in which
//! 456 overlay sources looked flag-free and pruning them broke resource_3ce.
//!
//! This binary exists so that measurement asks the routing crate instead.
//!
//! Usage:
//!   route-dump <source> [<source> ...]   flags for the named sources
//!   route-dump --all                     every live C source under the tree
//!   route-dump --routed                  only sources with non-baseline flags
//!   route-dump --standard                the baseline flag set, one per line
//!   route-dump --debt                    count of sources deviating from it
//!   route-dump --self-test
//!
//! `--standard` exists so the Makefile can assert that the flag set it
//! documents is the flag set the build actually uses. Camelot's own build was
//! a makefile, not a per-file flag database, and `Coaltergeist/camelot-gcc` --
//! a separate project, not an upstream of this one -- reproduces the whole ROM
//! against one such set.
//! Every deviation Alchemy still carries is debt, and `--debt` is what
//! measures it shrinking.

use std::collections::BTreeSet;
use std::path::Path;
use std::process::ExitCode;

use alchemy_routing::routing::{
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
        let Ok(entries) = std::fs::read_dir(directory) else { continue };
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
    if !routed_extras("semantic/does_not_exist_08ffffff.c").is_empty() {
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

fn main() -> ExitCode {
    let arguments: Vec<String> = std::env::args().skip(1).collect();
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
        if !Path::new(&source).exists() && !source.starts_with("semantic/does_not_exist") {
            eprintln!("warning: {source} does not exist on disk");
        }
        if only_routed && routed_extras(&source).is_empty() {
            continue;
        }
        emit(&source);
    }
    ExitCode::SUCCESS
}
