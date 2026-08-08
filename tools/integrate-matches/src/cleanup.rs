//! `cleanupInstalledScratch` -- retire the working files of an address that has
//! just been installed into `exact/`, and close its dossier.
//!
//! THIS FUNCTION DELETES FILES. Its only guard is the `/^08[0-9a-f]{6}$/` test
//! on the stem, and everything it removes is derived from that stem by prefix
//! match inside `workRoot`. The guard is reproduced without widening, the
//! prefix is the literal `${stem}.` including the dot, and the recursive
//! removal is confined to `workRoot/hand/${stem}`.

use std::fs;
use std::path::{Path, PathBuf};

use candidate_compiler::jsstring::utf16_cmp;

use crate::jsregex::{
    has_state_line, insert_state_after_heading, is_source_address, replace_state_line,
};

/// The return of `cleanupInstalledScratch`.
#[derive(Debug, Clone, PartialEq, Eq, Default)]
pub struct Cleanup {
    pub removed: Vec<String>,
    pub dossier_closed: bool,
}

/// `new Date().toISOString().slice(0, 10)` -- the UTC calendar date.
///
/// PORT NOTE -- UTC, not local. A run at 23:30 in Amsterdam writes tomorrow's
/// date into the dossier on both sides, and a port using a local-time clock
/// would disagree with the TypeScript for half an hour every day and pass every
/// test run outside that window. The civil-from-days conversion is written out
/// rather than pulled from a date crate to keep the dependency count at zero.
pub fn today_utc() -> String {
    let seconds = std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map_or(0, |d| d.as_secs());
    civil_date(seconds as i64 / 86_400)
}

/// Howard Hinnant's `civil_from_days`, for a day count since 1970-01-01.
pub fn civil_date(days: i64) -> String {
    let z = days + 719_468;
    let era = if z >= 0 { z } else { z - 146_096 } / 146_097;
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1460 + doe / 36_524 - doe / 146_096) / 365;
    let y = yoe + era * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let d = doy - (153 * mp + 2) / 5 + 1;
    let m = if mp < 10 { mp + 3 } else { mp - 9 };
    let year = if m <= 2 { y + 1 } else { y };
    format!("{year:04}-{m:02}-{d:02}")
}

/// `cleanupInstalledScratch(stem, workRoot = join(ROOT, "work"), date = today)`.
///
/// PORT NOTE -- `removed.sort()` with no comparator, which is ECMAScript's
/// UTF-16 code-unit order, not `str::cmp` (UTF-8 bytes) and not
/// `localeCompare`. The three agree on the ASCII filenames seen so far and
/// diverge on any pair straddling U+E000..U+FFFF against the astral planes.
///
/// PORT NOTE -- the `hand/${stem}/` entry is pushed AFTER the loose files and
/// then sorted with them, so it lands wherever `h` falls among the filenames
/// rather than always last. The self-test's expected
/// `08021360.c,08021360.txt,hand/08021360/` depends on that.
///
/// PORT NOTE -- when the dossier exists but has no `State:` line AND does not
/// begin with `#`, the insert branch's regex fails, `writeFileSync` rewrites
/// the file with identical content, and `dossierClosed` is reported `true`
/// anyway. Reproduced, including the pointless write; see the bug list.
pub fn cleanup_installed_scratch(
    stem: &str,
    work_root: &Path,
    date: &str,
) -> Result<Cleanup, String> {
    if !is_source_address(stem) {
        return Err("invalid cleanup address".to_string());
    }
    let mut removed: Vec<String> = Vec::new();
    if work_root.exists() {
        let prefix = format!("{stem}.");
        // `readdirSync(..., { withFileTypes: true })` -- Node returns entries in
        // the order the platform's `readdir` yields them, which is why the
        // TypeScript sorts afterwards rather than relying on it.
        let entries =
            fs::read_dir(work_root).map_err(|error| format!("{}: {error}", work_root.display()))?;
        for entry in entries {
            let entry = entry.map_err(|error| format!("{}: {error}", work_root.display()))?;
            let file_type = entry
                .file_type()
                .map_err(|error| format!("{}: {error}", entry.path().display()))?;
            // `entry.isFile()` is false for a symlink, a directory and a
            // socket alike; Node reports the type of the entry itself, not of
            // its target, so this is `file_type()`, never `metadata()`.
            if !file_type.is_file() {
                continue;
            }
            let name = entry.file_name().to_string_lossy().into_owned();
            if !name.starts_with(&prefix) {
                continue;
            }
            let path = work_root.join(&name);
            fs::remove_file(&path).map_err(|error| format!("{}: {error}", path.display()))?;
            removed.push(name);
        }
    }
    let hand_directory = work_root.join("hand").join(stem);
    if hand_directory.exists() {
        fs::remove_dir_all(&hand_directory)
            .map_err(|error| format!("{}: {error}", hand_directory.display()))?;
        removed.push(format!("hand/{stem}/"));
    }
    removed.sort_by(|left, right| utf16_cmp(left, right));

    let dossier: PathBuf = work_root.join("walls").join(format!("{stem}.md"));
    if !dossier.exists() {
        return Ok(Cleanup {
            removed,
            dossier_closed: false,
        });
    }
    let source =
        fs::read_to_string(&dossier).map_err(|error| format!("{}: {error}", dossier.display()))?;
    let state = format!("State: CLOSED — {date}. Installed by `tools/lib/integrate_matches.ts`.");
    let updated = if has_state_line(&source) {
        replace_state_line(&source, &state)
    } else {
        insert_state_after_heading(&source, &state)
    };
    fs::write(&dossier, updated).map_err(|error| format!("{}: {error}", dossier.display()))?;
    Ok(Cleanup {
        removed,
        dossier_closed: true,
    })
}
