pub mod sqlite;

// Same-directory temporary write plus atomic rename prevents truncated cache
// hits after interruption or concurrent writers. `.partial` files stay inert.

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
