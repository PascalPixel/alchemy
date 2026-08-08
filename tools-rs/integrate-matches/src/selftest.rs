//! `selfTest()` -- the tool's own `--self-test`, reproduced case for case.
//!
//! This is not the crate's test suite; it is a port of a function that the
//! repository's tooling gate invokes. The assertions, their order and their
//! failure messages are the TypeScript's.

use std::fs;
use std::path::{Path, PathBuf};

use crate::cleanup::cleanup_installed_scratch;
use crate::extent::{linked_function_extent, mismatch};

/// `mkdtempSync(join(tmpdir(), "alchemy-integrate-test-"))`.
///
/// PORT NOTE -- the name is random on both sides. It never reaches the output
/// (the self-test prints only `self-test=ok`), so there is nothing to
/// normalise; it is generated here from the process id and the clock rather
/// than by pulling in a dependency, and the directory is removed in every exit
/// path.
fn make_temporary_directory() -> Result<PathBuf, String> {
    let base = std::env::temp_dir();
    let nanos = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |d| d.subsec_nanos());
    for attempt in 0..64u32 {
        let path = base.join(format!(
            "alchemy-integrate-test-{}{nanos:09}{attempt}",
            std::process::id()
        ));
        match fs::create_dir(&path) {
            Ok(()) => return Ok(path),
            Err(error) if error.kind() == std::io::ErrorKind::AlreadyExists => continue,
            Err(error) => return Err(format!("{}: {error}", path.display())),
        }
    }
    Err("could not create a temporary directory".to_string())
}

pub fn self_test() -> Result<String, String> {
    if mismatch(&[1, 2, 3], &[1, 2, 3]).is_some() {
        return Err("identical source bytes differ".to_string());
    }
    let changed = mismatch(&[1, 2, 3], &[1, 4, 3]);
    match &changed {
        Some(m) if m.offset == 1 && m.left == Some(2) && m.right == Some(4) => {}
        _ => return Err("source-byte mismatch offset differs".to_string()),
    }
    let short = mismatch(&[1], &[1, 2]);
    match &short {
        Some(m) if m.offset == 1 && m.left.is_none() && m.right == Some(2) => {}
        _ => return Err("source-byte extent mismatch differs".to_string()),
    }
    // The second row has THREE fields, so `fields.length >= 4` drops it before
    // its `A` type column is consulted. That is the case this fixture exists to
    // pin: an absolute external symbol must not widen the compiled extent.
    let symbols = "08021360 00000030 T Func_08021360\n080770c1 A Func_080770c0";
    if linked_function_extent(symbols, "Func_08021360", 0x0802_1360 as f64, 48)? != 48 {
        return Err("absolute external symbol changed compiled extent".to_string());
    }

    let temporary = make_temporary_directory()?;
    let outcome = scratch_cleanup_case(&temporary);
    // `finally { rmSync(temporary, { recursive: true, force: true }) }`. The
    // path is the one this function created two statements ago and nothing
    // else; it is never assembled from a caller-supplied value.
    let _ = fs::remove_dir_all(&temporary);
    outcome?;
    Ok("self-test=ok".to_string())
}

fn scratch_cleanup_case(temporary: &Path) -> Result<(), String> {
    let write = |path: PathBuf, body: &str| -> Result<(), String> {
        fs::write(&path, body).map_err(|error| format!("{}: {error}", path.display()))
    };
    let mkdir = |path: PathBuf| -> Result<(), String> {
        fs::create_dir_all(&path).map_err(|error| format!("{}: {error}", path.display()))
    };
    mkdir(temporary.join("walls"))?;
    mkdir(temporary.join("hand").join("08021360"))?;
    write(
        temporary.join("hand").join("08021360").join("shape.c"),
        "champion\n",
    )?;
    write(temporary.join("08021360.c"), "candidate\n")?;
    write(temporary.join("08021360.txt"), "notes\n")?;
    write(temporary.join("unrelated.c"), "keep\n")?;
    write(
        temporary.join("walls").join("08021360.md"),
        "# 08021360\n\nState: OPEN — test.\n",
    )?;

    let cleaned = cleanup_installed_scratch("08021360", temporary, "2026-07-22")?;
    let dossier = fs::read_to_string(temporary.join("walls").join("08021360.md"))
        .map_err(|error| format!("{error}"))?;
    let ok = cleaned.removed.join(",") == "08021360.c,08021360.txt,hand/08021360/"
        && cleaned.dossier_closed
        && !temporary.join("08021360.c").exists()
        && !temporary.join("hand").join("08021360").exists()
        && temporary.join("unrelated.c").exists()
        && dossier.contains("State: CLOSED — 2026-07-22");
    if !ok {
        return Err("installed scratch cleanup differs".to_string());
    }
    Ok(())
}
