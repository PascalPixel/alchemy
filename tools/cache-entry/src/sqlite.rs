// One SQLite content cache replaces unbounded per-key files. WAL, a busy
// timeout, and transactions make parallel readers/writers interruption-safe.

use rusqlite::Connection;
use std::path::Path;
use std::sync::Mutex;

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

    /// Insert or replace a single (key, kind) entry, leaving any other kind
    /// stored under the same key untouched.
    pub fn upsert(&self, key: &str, kind: &str, value: &[u8]) -> Result<(), String> {
        let connection = self
            .connection
            .lock()
            .map_err(|_| "cache connection poisoned")?;
        connection
            .execute(
                "INSERT INTO entries (key, kind, value) VALUES (?1, ?2, ?3)
                 ON CONFLICT(key, kind) DO UPDATE SET value = excluded.value",
                rusqlite::params![key, kind, value],
            )
            .map_err(|error| error.to_string())?;
        Ok(())
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
    fn roundtrips_and_replaces_by_key() {
        let directory = std::env::temp_dir().join(format!(
            "cache-entry-sqlite-test-{}-{}",
            std::process::id(),
            std::time::SystemTime::now()
                .duration_since(std::time::UNIX_EPOCH)
                .unwrap()
                .as_nanos()
        ));
        let path = directory.join("cache.sqlite3");
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
        std::fs::remove_dir_all(&directory).ok();
    }
}
