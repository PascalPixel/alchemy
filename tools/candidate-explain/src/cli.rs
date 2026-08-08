//! `optionsOf(argv)` and the `node:path` primitives it leans on.

use std::path::{Path, PathBuf};

use alchemy_routing::routing::root;
use overlay_disasm::OVERLAY_BASE;
use thumb_disasm::js_number;

use crate::jsnum::{pad_start, parse_int_16, to_string_16};
use crate::matchers::overlay_id;

pub const USAGE: &str =
    "usage: candidate_explain.ts OVERLAY:OFFSET --source FILE [--routing-source FILE] [--span BYTES] [--work DIR]\n\
     \x20      candidate_explain.ts semantic/main/08xxxxxx.c [--work DIR]";

#[derive(Debug, Clone, PartialEq)]
pub enum Options {
    Overlay {
        id: String,
        overlay: String,
        offset: f64,
        source: PathBuf,
        routing_source: PathBuf,
        work: PathBuf,
        /// `span?: number`.
        ///
        /// PORT NOTE -- `--span` with no following argument evaluates
        /// `Number(undefined)`, which is `NaN`, NOT `undefined`. The later
        /// `if (span === undefined)` guard therefore does NOT fire, and a
        /// `NaN` span flows into `image.subarray(offset, offset + NaN)`, which
        /// JavaScript clamps to an empty slice. `Option<f64>` reproduces that
        /// exactly: `Some(NaN)` is a distinct state from `None`. Modelling the
        /// span as `Option<u32>` and rejecting the unparsable case would be a
        /// bug fix, and this port fixes nothing.
        span: Option<f64>,
    },
    Main {
        source: PathBuf,
        work: PathBuf,
    },
}

/// The three ways argument parsing can end.
#[derive(Debug)]
pub enum ParseOutcome {
    Parsed(Box<Options>),
    /// `-h` / `--help`: the usage text is printed and the process exits 0.
    Help,
    Error(String),
}

/// `optionsOf(argv)`.
pub fn parse_arguments(argv: &[String], cwd: &Path) -> ParseOutcome {
    let mut id = String::new();
    let mut source = String::new();
    let mut routing_source = String::new();
    let mut work = String::new();
    let mut span: Option<f64> = None;

    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].as_str();
        // `argv[++index] ?? ""` -- the index advances even when it walks off
        // the end, which is what terminates the loop on a dangling flag.
        match argument {
            "--source" => {
                index += 1;
                source = argv.get(index).cloned().unwrap_or_default();
            }
            "--routing-source" => {
                index += 1;
                routing_source = argv.get(index).cloned().unwrap_or_default();
            }
            "--work" => {
                index += 1;
                work = argv.get(index).cloned().unwrap_or_default();
            }
            "--span" => {
                index += 1;
                // `Number(argv[++index])`: no `?? ""` here, so a missing value
                // is `Number(undefined)` == NaN, not `Number("")` == 0.
                span = Some(match argv.get(index) {
                    Some(text) => js_number(text),
                    None => f64::NAN,
                });
            }
            "-h" | "--help" => return ParseOutcome::Help,
            _ => {
                if id.is_empty() {
                    id = argument.to_string();
                } else {
                    return ParseOutcome::Error(format!("unrecognised argument: {argument}"));
                }
            }
        }
        index += 1;
    }

    if let Some((overlay, offset_digits)) = overlay_id(&id) {
        if source.is_empty() {
            return ParseOutcome::Error("overlay mode requires --source FILE".to_string());
        }
        let offset = parse_int_16(offset_digits);
        let routing_source = if routing_source.is_empty() {
            let stem = pad_start(&to_string_16(OVERLAY_BASE as f64 + offset), 8, '0');
            root()
                .join("exact")
                .join(format!("{overlay}_c_{stem}.c"))
                .to_string_lossy()
                .into_owned()
        } else {
            routing_source
        };
        return ParseOutcome::Parsed(Box::new(Options::Overlay {
            id: id.clone(),
            overlay: overlay.to_string(),
            offset,
            source: resolve(cwd, &source),
            routing_source: resolve(cwd, &routing_source),
            work: resolve_work(cwd, &work, "work/candidate-explain-overlay"),
            span,
        }));
    }

    if id.is_empty() {
        return ParseOutcome::Error(
            "usage: candidate_explain.ts OVERLAY:OFFSET --source FILE | candidate_explain.ts FILE.c"
                .to_string(),
        );
    }
    ParseOutcome::Parsed(Box::new(Options::Main {
        source: resolve(cwd, &id),
        work: resolve_work(cwd, &work, "work/candidate-explain-main"),
    }))
}

/// `resolve(work || join(ROOT, fallback))` -- `||` on the empty string, so an
/// explicit `--work ""` falls back exactly as an absent flag does.
fn resolve_work(cwd: &Path, work: &str, fallback: &str) -> PathBuf {
    if work.is_empty() {
        return resolve(cwd, &root().join(fallback).to_string_lossy());
    }
    resolve(cwd, work)
}

/// `node:path`'s `resolve(path)` against a single base directory.
///
/// PORT NOTE -- `std::fs::canonicalize` is NOT this function: it resolves
/// symlinks and fails when the path does not exist yet, and `--work` names a
/// directory that is usually created afterwards. `resolve` is purely lexical.
pub fn resolve(cwd: &Path, path: &str) -> PathBuf {
    let joined = if path.starts_with('/') {
        PathBuf::from(path)
    } else {
        cwd.join(path)
    };
    let mut parts: Vec<String> = Vec::new();
    for piece in joined.to_string_lossy().split('/') {
        match piece {
            "" | "." => {}
            ".." => {
                parts.pop();
            }
            other => parts.push(other.to_string()),
        }
    }
    PathBuf::from(format!("/{}", parts.join("/")))
}

/// `basename(path, ".c")` -- `node:path`'s two-argument form, which strips the
/// suffix only when the remainder would be non-empty.
pub fn basename_without_c(path: &str) -> String {
    let base = alchemy_plan::nodepath::basename(path);
    match base.strip_suffix(".c") {
        Some(stem) if !stem.is_empty() => stem.to_string(),
        _ => base.to_string(),
    }
}
