pub mod sqlite;

// Writing a content-addressed cache entry so it can never be observed
// half-written.
//
// THE SECOND DEFECT BEHIND THE POISONED CACHE, and the one that actually
// explains the symptom. A hand-maintained key (see HANDOVER §5h) yields an
// entry that is WRONG BUT COMPLETE. It cannot explain one key holding 160
// bytes in one checkout and 164 in another: a content-addressed key has one
// correct content by construction, so two different LENGTHS mean one of them
// was never finished being written.
//
// A plain write to the final path is not atomic. A run killed mid-write, or
// two concurrent builds of the same overlay, leaves a truncated file that the
// next existence check serves happily for ever. Nothing downstream re-reads or
// re-checks it, which is why the damage surfaced as a mysterious LZ round-trip
// failure in a completely different tool, recurred for several runs, and
// survived across commits: switching commits does not touch `out/`.
//
// Writing to a unique temporary name in the SAME directory and renaming makes
// the entry appear whole or not at all; rename is atomic within a filesystem.
// A concurrent writer loses the race harmlessly, leaving a complete file.
//
// Ported from tools/lib/cache_entry.ts.

use std::fs;
use std::io;
use std::path::Path;
use std::sync::atomic::{AtomicU64, Ordering};
use std::time::{SystemTime, UNIX_EPOCH};

static COUNTER: AtomicU64 = AtomicU64::new(0);

/// The suffix is deliberately NOT `.bin`: a temporary left behind by a killed
/// process must never be mistaken for an entry by a `*.bin` listing, and can
/// never be found by a key lookup because the key names the final path.
fn temporary_path(final_path: &Path) -> std::path::PathBuf {
    let nanos = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map(|elapsed| elapsed.subsec_nanos() as u64)
        .unwrap_or(0);
    let unique = COUNTER.fetch_add(1, Ordering::Relaxed);
    let mut name = final_path.as_os_str().to_os_string();
    name.push(format!(
        ".{}-{nanos:x}{unique:x}.partial",
        std::process::id()
    ));
    name.into()
}

/// Write `data` to `final_path` atomically. Never leaves a partial entry there.
pub fn write_cache_entry_atomically(final_path: &Path, data: &[u8]) -> io::Result<()> {
    let temporary = temporary_path(final_path);
    match fs::write(&temporary, data).and_then(|()| fs::rename(&temporary, final_path)) {
        Ok(()) => Ok(()),
        Err(error) => {
            // Best effort. A stray `.partial` is inert by construction.
            let _ = fs::remove_file(&temporary);
            Err(error)
        }
    }
}
