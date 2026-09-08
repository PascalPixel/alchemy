// One SQLite content cache replaces unbounded per-key files. WAL, a busy
// timeout, and transactions make parallel readers/writers interruption-safe.

use rusqlite::Connection;
use std::fs;
use std::io;
use std::path::Path;
use std::sync::atomic::{AtomicU64, Ordering};
use std::sync::Mutex;
use std::time::{SystemTime, UNIX_EPOCH};

pub struct SqliteCache {
    connection: Mutex<Connection>,
}

impl SqliteCache {
    pub fn open(path: &Path) -> Result<Self, String> {
        if let Some(parent) = path.parent() {
            std::fs::create_dir_all(parent)
                .map_err(|error| format!("{}: {error}", parent.display()))?;
        }
        let connection =
            Connection::open(path).map_err(|error| format!("{}: {error}", path.display()))?;
        connection
            .pragma_update(None, "journal_mode", "WAL")
            .map_err(|error| error.to_string())?;
        connection
            .busy_timeout(std::time::Duration::from_secs(30))
            .map_err(|error| error.to_string())?;
        connection
            .execute_batch(
                "CREATE TABLE IF NOT EXISTS entries (
                    key TEXT NOT NULL,
                    kind TEXT NOT NULL,
                    value BLOB NOT NULL,
                    PRIMARY KEY (key, kind)
                ) WITHOUT ROWID;",
            )
            .map_err(|error| error.to_string())?;
        Ok(Self {
            connection: Mutex::new(connection),
        })
    }

    /// Every (kind, value) pair stored for `key`, or `None` if the key has no entries.
    pub fn get(&self, key: &str) -> Result<Option<Vec<(String, Vec<u8>)>>, String> {
        let connection = self
            .connection
            .lock()
            .map_err(|_| "cache connection poisoned")?;
        let mut statement = connection
            .prepare_cached("SELECT kind, value FROM entries WHERE key = ?1")
            .map_err(|error| error.to_string())?;
        let rows = statement
            .query_map([key], |row| {
                Ok((row.get::<_, String>(0)?, row.get::<_, Vec<u8>>(1)?))
            })
            .map_err(|error| error.to_string())?
            .collect::<Result<Vec<_>, _>>()
            .map_err(|error| error.to_string())?;
        Ok((!rows.is_empty()).then_some(rows))
    }

    /// Atomically replace every entry for `key` with `items`, in one transaction.
    pub fn put(&self, key: &str, items: &[(&str, &[u8])]) -> Result<(), String> {
        let mut connection = self
            .connection
            .lock()
            .map_err(|_| "cache connection poisoned")?;
        let transaction = connection
            .transaction()
            .map_err(|error| error.to_string())?;
        transaction
            .execute("DELETE FROM entries WHERE key = ?1", [key])
            .map_err(|error| error.to_string())?;
        for (kind, value) in items {
            transaction
                .execute(
                    "INSERT INTO entries (key, kind, value) VALUES (?1, ?2, ?3)",
                    rusqlite::params![key, kind, value],
                )
                .map_err(|error| error.to_string())?;
        }
        transaction.commit().map_err(|error| error.to_string())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn failed_replacement_preserves_previous_entries() {
        let directory = tempfile::tempdir().unwrap();
        let cache = SqliteCache::open(&directory.path().join("cache.sqlite3")).unwrap();
        cache.put("a", &[("object", b"original")]).unwrap();
        assert!(cache
            .put("a", &[("object", b"new"), ("object", b"duplicate")])
            .is_err());
        assert_eq!(
            cache.get("a").unwrap().unwrap(),
            vec![("object".into(), b"original".to_vec())]
        );
    }

    #[test]
    fn atomic_file_replacement_leaves_no_temporary_entries() {
        let directory = tempfile::tempdir().unwrap();
        let path = directory.path().join("entry.bin");
        write_cache_entry_atomically(&path, b"old").unwrap();
        write_cache_entry_atomically(&path, b"replacement").unwrap();
        assert_eq!(fs::read(&path).unwrap(), b"replacement");
        assert_eq!(fs::read_dir(directory.path()).unwrap().count(), 1);
    }

    #[test]
    fn roundtrips_and_replaces_by_key() {
        let directory = tempfile::tempdir().unwrap();
        let path = directory.path().join("cache.sqlite3");
        let cache = SqliteCache::open(&path).unwrap();
        assert_eq!(cache.get("missing").unwrap(), None);
        cache
            .put("a", &[("object", b"one"), ("assembly", b"two")])
            .unwrap();
        let items = cache.get("a").unwrap().unwrap();
        assert_eq!(items.len(), 2);
        assert!(items.contains(&("object".to_string(), b"one".to_vec())));
        assert!(items.contains(&("assembly".to_string(), b"two".to_vec())));
        cache.put("a", &[("object", b"three")]).unwrap();
        let replaced = cache.get("a").unwrap().unwrap();
        assert_eq!(replaced, vec![("object".to_string(), b"three".to_vec())]);
    }
}

// Same-directory temporary write plus atomic rename prevents truncated cache
// hits after interruption or concurrent writers. `.partial` files stay inert.

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
