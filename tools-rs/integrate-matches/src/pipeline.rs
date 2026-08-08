//! `main()` -- gate every `src_*.c` draft in a directory and, with `--apply`,
//! install the ones whose linked bytes reproduce the hand-written assembly.

use std::fs;
use std::path::{Path, PathBuf};

use alchemy_plan::nodepath::basename;
use alchemy_routing::routing::root;
use match_m2c::jsstring::utf16_cmp;

use crate::cleanup::{cleanup_installed_scratch, today_utc};
use crate::extent::mismatch;
use crate::jsregex::{carries_helper, is_candidate_name};
use crate::link::{linked_bytes, Kind};

/// The repository root this run treats as `ROOT`.
///
/// PORT NOTE -- ADDED BY THE PORT, not present in the TypeScript. `--apply`
/// copies into the tracked `exact/` and deletes from the tracked `asm/`, and
/// the TypeScript hard-codes both. The override defaults to the production
/// root, so an unset environment behaves identically; it exists so the apply
/// path can be exercised against a fixture tree instead of the working copy.
/// The DRY-RUN path, which is what the parity harness compares, never reads it
/// differently from `root()`.
pub const ROOT_OVERRIDE: &str = "ALCHEMY_INTEGRATE_ROOT";

pub fn root_directory() -> PathBuf {
    match std::env::var_os(ROOT_OVERRIDE) {
        Some(value) if !value.is_empty() => PathBuf::from(value),
        _ => root().to_path_buf(),
    }
}

/// One line of the tool's output.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Report {
    pub lines: Vec<String>,
    pub accepted: usize,
    pub rejected: usize,
}

/// `main()` minus argument parsing, returning the lines it would print so that
/// the harness can compare a whole artifact rather than a captured stream.
pub fn run_pipeline(directory: &str, apply: bool) -> Result<Report, String> {
    let root_directory = root_directory();
    let directory_path = Path::new(directory);
    let scratch = directory_path.join("gate");
    fs::create_dir_all(&scratch).map_err(|error| format!("{}: {error}", scratch.display()))?;

    let mut accepted: Vec<(String, usize)> = Vec::new();
    let mut rejected: Vec<(String, String)> = Vec::new();

    // `readdirSync(options.directory).filter(...).sort()`. The sort has no
    // comparator, so it is UTF-16 code-unit order.
    let mut candidates: Vec<String> = Vec::new();
    let entries = fs::read_dir(directory_path)
        .map_err(|error| format!("{}: {error}", directory_path.display()))?;
    for entry in entries {
        let entry = entry.map_err(|error| format!("{}: {error}", directory_path.display()))?;
        let name = entry.file_name().to_string_lossy().into_owned();
        if is_candidate_name(&name) {
            candidates.push(name);
        }
    }
    candidates.sort_by(|left, right| utf16_cmp(left, right));

    for name in &candidates {
        let candidate = directory_path.join(name);
        // `basename(name, ".c").replace("src_", "")`. `String#replace` with a
        // STRING pattern replaces the FIRST occurrence only, so
        // `src_src_08000000.c` yields `src_08000000`, which then fails the
        // address guard inside `linkedBytes`. Reproduced with `replacen(_, 1)`;
        // a `replace_all` port would silently accept that name.
        //
        // PORT NOTE -- `basename(path, ext)` strips `ext` only when it is a
        // PROPER suffix: Node's `basename(".c", ".c")` is `".c"`, not `""`. The
        // filter above guarantees a `src_` prefix so the equal-length case is
        // unreachable, but the helper must not disagree with Node about it.
        let base = basename(name);
        let stripped = if base.len() > 2 && base.ends_with(".c") {
            &base[..base.len() - 2]
        } else {
            base
        };
        let stem = stripped.replacen("src_", "", 1);

        if root_directory.join("exact").join(format!("{stem}.c")).exists() {
            continue;
        }
        // `readFileSync(candidate, "utf8")`. Node REPLACES invalid UTF-8 with
        // U+FFFD rather than throwing, so this is a lossy decode, not
        // `read_to_string`, which would turn a mis-encoded candidate into a
        // hard error where the TypeScript keeps going.
        let raw = fs::read(&candidate).map_err(|error| format!("{}: {error}", candidate.display()))?;
        let text = String::from_utf8_lossy(&raw);
        if carries_helper(&text) {
            rejected.push((stem, "carries an m2c helper".to_string()));
            continue;
        }
        let source = root_directory.join("asm").join(format!("{stem}.s"));
        if !source.exists() {
            continue;
        }
        let source = source.to_string_lossy().into_owned();
        let candidate = candidate.to_string_lossy().into_owned();
        // The `try`/`catch` wraps BOTH linked_bytes calls and the comparison,
        // so a failure on the C side is reported with the same `reject` shape
        // as a byte difference and the asm-side work is discarded.
        let outcome = (|| -> Result<(), String> {
            let wanted = linked_bytes(&stem, &source, &scratch, Kind::Asm, &root_directory)?;
            let got = linked_bytes(&stem, &candidate, &scratch, Kind::C, &root_directory)?;
            match mismatch(&wanted, &got) {
                Some(difference) => rejected.push((
                    stem.clone(),
                    // `.toString(16)` -- lowercase, UNPADDED. `{:x}` agrees;
                    // `{:#x}` would add a `0x` the TypeScript writes by hand.
                    format!(
                        "bytes differ at +0x{:x} (asm={}B c={}B)",
                        difference.offset,
                        wanted.len(),
                        got.len()
                    ),
                )),
                None => accepted.push((stem.clone(), wanted.len())),
            }
            Ok(())
        })();
        if let Err(message) = outcome {
            rejected.push((stem, message));
        }
    }

    let mut lines: Vec<String> = Vec::new();
    for (stem, size) in &accepted {
        lines.push(format!("accept {stem} ({size}B)"));
        if apply {
            let from = directory_path.join(format!("src_{stem}.c"));
            let to = root_directory.join("exact").join(format!("{stem}.c"));
            fs::copy(&from, &to).map_err(|error| format!("{}: {error}", to.display()))?;
            let asm = root_directory.join("asm").join(format!("{stem}.s"));
            // `rmSync(path, { force: true })` -- a missing file is not an
            // error.
            if asm.exists() {
                fs::remove_file(&asm).map_err(|error| format!("{}: {error}", asm.display()))?;
            }
            let cleaned = cleanup_installed_scratch(
                stem,
                &root_directory.join("work"),
                &today_utc(),
            )?;
            if !cleaned.removed.is_empty() || cleaned.dossier_closed {
                lines.push(format!(
                    "clean {stem} scratch={} wall={}",
                    cleaned.removed.len(),
                    if cleaned.dossier_closed { "closed" } else { "absent" }
                ));
            }
        }
    }
    for (stem, reason) in &rejected {
        lines.push(format!("reject {stem}: {reason}"));
    }
    lines.push(format!(
        "accepted={} rejected={}{}",
        accepted.len(),
        rejected.len(),
        if apply { " (applied)" } else { " (dry run)" }
    ));
    Ok(Report { lines, accepted: accepted.len(), rejected: rejected.len() })
}
