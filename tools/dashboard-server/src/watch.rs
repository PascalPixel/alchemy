// Repository polling for live coverage and native source files.
//
// A dependency-free server cannot use the platform's recursive filesystem
// notifications portably, so polling is the deliberate mechanism here. A
// change under a coverage path schedules a rebuild, and a native Rust source
// change exits so the service supervisor can restart the binary. Unwatched
// files do none of these things.
//
// The 250ms interval is also the dashboard's debounce window, so a change is
// observed within one window rather than instantly.

use std::path::{Path, PathBuf};
use std::time::{Duration, SystemTime};

use crate::paths;
use crate::state::mtime;

/// A watch target reduced to a single comparable number.
#[derive(Debug, Clone, PartialEq)]
pub struct Fingerprint {
    pub newest: f64,
    pub entries: usize,
}

/// The newest mtime under `path`, and how many entries were seen.
///
/// PORT NOTE on NaN: `mtime` returns 0.0 for an unreadable entry, never NaN,
/// so the running maximum cannot be poisoned. `js_max` is used anyway rather
/// than `f64::max`, because `f64::max` IGNORES a NaN operand where JS
/// `Math.max` PROPAGATES it, and a silent divergence there would make a
/// watcher that never fires look identical to one with nothing to do.
pub fn fingerprint(path: &Path) -> Fingerprint {
    let mut newest = 0.0f64;
    let mut entries = 0usize;
    let mut stack = vec![path.to_path_buf()];
    while let Some(current) = stack.pop() {
        let Ok(metadata) = std::fs::symlink_metadata(&current) else {
            continue;
        };
        entries += 1;
        newest = js_max(newest, mtime(&current));
        if metadata.is_dir() {
            let Ok(listing) = std::fs::read_dir(&current) else {
                continue;
            };
            for entry in listing.flatten() {
                stack.push(entry.path());
            }
        }
    }
    Fingerprint { newest, entries }
}

/// `Math.max(a, b)`: NaN propagates, and `+0` beats `-0`.
///
/// Clippy would suggest `a.max(b)` here. That is WRONG for JS semantics, so
/// the helper is named rather than suppressed.
pub fn js_max(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left > right {
        left
    } else if right > left {
        right
    } else if left == 0.0 && right == 0.0 {
        // +0 > -0 for Math.max.
        if left.is_sign_positive() {
            left
        } else {
            right
        }
    } else {
        left
    }
}

pub struct Watcher {
    coverage: Vec<(PathBuf, Fingerprint)>,
    restart: Vec<(PathBuf, Fingerprint)>,
}

pub enum Tick {
    Idle,
    /// A watched coverage directory or build-evidence file changed.
    Coverage,
    /// A native Rust source changed: exit so the service supervisor restarts.
    Restart,
}

impl Watcher {
    pub fn new(root: &Path) -> Watcher {
        let mut coverage: Vec<(PathBuf, Fingerprint)> = paths::COVERAGE_DIRECTORIES
            .iter()
            .map(|name| root.join(name))
            .filter(|path| path.exists())
            .map(|path| {
                let seen = fingerprint(&path);
                (path, seen)
            })
            .collect();
        // Completed-build evidence is another coverage input, but does not
        // require a process restart.
        for file in paths::coverage_build_files_at(root) {
            if file.exists() {
                let seen = fingerprint(&file);
                coverage.push((file, seen));
            }
        }
        Watcher {
            coverage,
            restart: paths::restart_files_at(root)
                .into_iter()
                .map(|f| {
                    let seen = fingerprint(&f);
                    (f, seen)
                })
                .collect(),
        }
    }

    /// One poll. Restart wins, then page, then coverage. A native source
    /// change exits synchronously and nothing after it runs.
    pub fn tick(&mut self) -> Tick {
        for (file, seen) in &mut self.restart {
            let now = fingerprint(file);
            if now != *seen {
                *seen = now;
                return Tick::Restart;
            }
        }
        let mut coverage_changed = false;
        for (path, seen) in &mut self.coverage {
            let now = fingerprint(path);
            if now != *seen {
                *seen = now;
                coverage_changed = true;
            }
        }
        if coverage_changed {
            Tick::Coverage
        } else {
            Tick::Idle
        }
    }
}

pub const POLL: Duration = Duration::from_millis(250);

/// Bump a file's mtime without changing its content, for the watcher proof.
pub fn touch(path: &Path) -> std::io::Result<()> {
    let body = std::fs::read(path)?;
    std::fs::write(path, body)?;
    Ok(())
}

pub fn now() -> SystemTime {
    SystemTime::now()
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::sync::Mutex;

    fn scratch(name: &str) -> PathBuf {
        // Suffixed with the pid so every run starts cold WITHOUT deleting
        // anything.
        let dir = Path::new(env!("CARGO_MANIFEST_DIR"))
            .join("target")
            .join("fixtures")
            .join(format!("{name}-{}", std::process::id()));
        std::fs::create_dir_all(dir.join("asm")).unwrap();
        std::fs::create_dir_all(dir.join("tools").join("dashboard-server").join("src")).unwrap();
        std::fs::create_dir_all(dir.join("tools").join("coverage-map").join("src")).unwrap();
        std::fs::write(dir.join("asm").join("main.s"), "one\n").unwrap();
        std::fs::write(
            dir.join("tools/dashboard-server/src/main.rs"),
            "// server\n",
        )
        .unwrap();
        std::fs::write(
            dir.join("tools/coverage-map/src/lib.rs"),
            "// coverage\n",
        )
        .unwrap();
        std::fs::write(dir.join("unwatched.txt"), "nothing\n").unwrap();
        dir
    }

    /// `ALCHEMY_DASHBOARD_ROOT` is PROCESS-WIDE state, and `cargo test` runs
    /// these cases on parallel threads. Without this lock one case's root
    /// leaks into another's watcher and the negative control fails for a
    /// reason that has nothing to do with the watcher. Found by the test
    /// itself, which is the point of writing the negative control first.
    static ROOT_LOCK: Mutex<()> = Mutex::new(());

    fn with_root<R>(root: &Path, body: impl FnOnce() -> R) -> R {
        let guard = ROOT_LOCK
            .lock()
            .unwrap_or_else(|poison| poison.into_inner());
        std::env::set_var("ALCHEMY_DASHBOARD_ROOT", root);
        let out = body();
        std::env::remove_var("ALCHEMY_DASHBOARD_ROOT");
        drop(guard);
        out
    }

    // Sleep past the filesystem's mtime granularity so a change is visible.
    fn settle() {
        std::thread::sleep(Duration::from_millis(30));
    }

    #[test]
    fn a_change_to_a_watched_restart_file_fires_and_an_unwatched_file_does_not() {
        let root = scratch("watch-restart");
        with_root(&root, || {
            let mut watcher = Watcher::new(&root);
            assert!(
                matches!(watcher.tick(), Tick::Idle),
                "a quiet tree must be idle"
            );

            // NEGATIVE CONTROL FIRST: an unwatched file must produce nothing.
            settle();
            touch(&root.join("unwatched.txt")).unwrap();
            settle();
            assert!(
                matches!(watcher.tick(), Tick::Idle),
                "an UNWATCHED file must not wake the watcher"
            );

            // Now the real thing: a native coverage source.
            settle();
            touch(&root.join("tools/coverage-map/src/lib.rs")).unwrap();
            settle();
            assert!(
                matches!(watcher.tick(), Tick::Restart),
                "native coverage Rust sources must restart the server"
            );
            settle();
            touch(&root.join("tools/dashboard-server/src/main.rs")).unwrap();
            settle();
            assert!(
                matches!(watcher.tick(), Tick::Restart),
                "the Rust server source restarts too"
            );
        });
    }

    #[test]
    fn a_change_under_a_watched_coverage_directory_schedules_a_rebuild() {
        let root = scratch("watch-coverage");
        with_root(&root, || {
            let mut watcher = Watcher::new(&root);
            assert!(matches!(watcher.tick(), Tick::Idle));
            settle();
            std::fs::write(root.join("asm").join("added.s"), "two\n").unwrap();
            settle();
            assert!(matches!(watcher.tick(), Tick::Coverage));
            assert!(
                matches!(watcher.tick(), Tick::Idle),
                "a change is reported once"
            );
        });
    }

    #[test]
    fn js_max_propagates_nan_where_the_rust_method_swallows_it() {
        assert!(js_max(f64::NAN, 1.0).is_nan());
        assert!(js_max(1.0, f64::NAN).is_nan());
        // The thing a naive port would have written, shown to differ.
        assert_eq!(f64::NAN.max(1.0), 1.0);
        assert_eq!(js_max(1.0, 2.0), 2.0);
        assert!(js_max(0.0, -0.0).is_sign_positive());
    }

    #[test]
    fn a_missing_watch_target_fingerprints_as_empty_rather_than_panicking() {
        let seen = fingerprint(Path::new("/nonexistent/alchemy/asm"));
        assert_eq!(seen.entries, 0);
        assert_eq!(seen.newest, 0.0);
    }
}
