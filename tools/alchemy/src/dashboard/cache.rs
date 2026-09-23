//! The dashboard's disposable cache under `out/dashboard/`: the game font, the
//! tab icons, ROM index summaries and rendered views. Everything here is
//! derived, rebuildable and never tracked; deleting the folder only costs time.
use sha1::{Digest, Sha1};
use std::{
    collections::BTreeMap,
    path::{Path, PathBuf},
    sync::Mutex,
    time::{Instant, UNIX_EPOCH},
};

pub(super) const DIRECTORY: &str = "out/dashboard";

/// The running executable's size and modification time. A rebuilt dashboard
/// writes fresh derived files instead of trusting ones an older build made.
pub(super) fn code_identity() -> String {
    std::env::current_exe()
        .ok()
        .and_then(|path| std::fs::metadata(path).ok())
        .map(|meta| format!("{}:{}", meta.len(), modified(&meta)))
        .unwrap_or_default()
}
fn modified(meta: &std::fs::Metadata) -> u128 {
    meta.modified()
        .ok()
        .and_then(|time| time.duration_since(UNIX_EPOCH).ok())
        .map_or(0, |duration| duration.as_nanos())
}
/// A short content stamp for cache names and immutable asset URLs.
pub(super) fn stamp(parts: &[&[u8]]) -> String {
    let mut digest = Sha1::new();
    for part in parts {
        digest.update((part.len() as u64).to_le_bytes());
        digest.update(part);
    }
    format!("{:x}", digest.finalize())[..16].into()
}
/// Size and modification time of each path, for inputs too large to hash on
/// every change. Missing files contribute their absence.
pub(super) fn metadata_stamp(paths: &[PathBuf]) -> String {
    let lines = paths
        .iter()
        .map(|path| match std::fs::metadata(path) {
            Ok(meta) => format!("{}\0{}\0{}\n", path.display(), meta.len(), modified(&meta)),
            Err(_) => format!("{}\0missing\n", path.display()),
        })
        .collect::<String>();
    stamp(&[code_identity().as_bytes(), lines.as_bytes()])
}

/// One derived file per name: `<name>-<stamp>.<extension>`. Storing a new stamp
/// removes the superseded file so the folder never accumulates history.
pub(super) struct Store {
    directory: PathBuf,
}
impl Store {
    pub(super) fn at(root: &Path) -> Self {
        Self {
            directory: root.join(DIRECTORY),
        }
    }
    fn path(&self, name: &str, stamp: &str, extension: &str) -> PathBuf {
        self.directory.join(format!("{name}-{stamp}.{extension}"))
    }
    pub(super) fn load(&self, name: &str, stamp: &str, extension: &str) -> Option<Vec<u8>> {
        std::fs::read(self.path(name, stamp, extension)).ok()
    }
    pub(super) fn save(
        &self,
        name: &str,
        stamp: &str,
        extension: &str,
        bytes: &[u8],
    ) -> Result<(), String> {
        std::fs::create_dir_all(&self.directory)
            .map_err(|error| format!("{}: {error}", self.directory.display()))?;
        let path = self.path(name, stamp, extension);
        let partial = path.with_extension(format!("{extension}.partial"));
        std::fs::write(&partial, bytes).map_err(|error| format!("{}: {error}", path.display()))?;
        std::fs::rename(&partial, &path).map_err(|error| format!("{}: {error}", path.display()))?;
        let keep = path.file_name().map(|name| name.to_owned());
        for entry in std::fs::read_dir(&self.directory)
            .into_iter()
            .flatten()
            .flatten()
        {
            let file = entry.file_name();
            let text = file.to_string_lossy();
            if Some(&file) != keep.as_ref()
                && text.starts_with(&format!("{name}-"))
                && text.ends_with(&format!(".{extension}"))
                && text[name.len() + 1..text.len() - extension.len() - 1]
                    .bytes()
                    .all(|byte| byte.is_ascii_hexdigit())
            {
                let _ = std::fs::remove_file(entry.path());
            }
        }
        Ok(())
    }
    /// Load a derived file or build and keep it.
    pub(super) fn get_or_build(
        &self,
        name: &str,
        stamp: &str,
        extension: &str,
        build: impl FnOnce() -> Result<Vec<u8>, String>,
    ) -> Result<Vec<u8>, String> {
        if let Some(bytes) = self.load(name, stamp, extension) {
            return Ok(bytes);
        }
        let bytes = build()?;
        self.save(name, stamp, extension, &bytes)?;
        Ok(bytes)
    }
}

/// What a background job is doing, for the status bar and loading dialogs.
#[derive(Clone, Debug, PartialEq, Eq)]
pub(super) enum Phase {
    Running,
    Ready,
    Failed,
}
#[derive(Clone, Debug)]
pub(super) struct Job {
    pub id: &'static str,
    pub label: &'static str,
    pub done: usize,
    pub total: usize,
    pub phase: Phase,
    pub note: String,
    pub millis: u128,
    started: Option<Instant>,
}
impl Job {
    pub(super) fn fraction(&self) -> f64 {
        match self.phase {
            Phase::Running if self.total > 0 => self.done as f64 / self.total as f64,
            Phase::Running => 0.0,
            _ => 1.0,
        }
    }
}
static JOBS: Mutex<Vec<Job>> = Mutex::new(Vec::new());
fn jobs_mut<R>(f: impl FnOnce(&mut Vec<Job>) -> R) -> R {
    f(&mut JOBS.lock().unwrap_or_else(|error| error.into_inner()))
}
pub(super) fn begin(id: &'static str, label: &'static str, total: usize) {
    jobs_mut(|jobs| {
        let job = Job {
            id,
            label,
            done: 0,
            total,
            phase: Phase::Running,
            note: String::new(),
            millis: 0,
            started: Some(Instant::now()),
        };
        match jobs.iter_mut().find(|job| job.id == id) {
            Some(existing) => *existing = job,
            None => jobs.push(job),
        }
    });
}
pub(super) fn advance(id: &str, done: usize, note: impl Into<String>) {
    jobs_mut(|jobs| {
        if let Some(job) = jobs.iter_mut().find(|job| job.id == id) {
            job.done = done.min(job.total);
            job.note = note.into();
        }
    });
}
pub(super) fn finish(id: &str, result: Result<String, String>) {
    jobs_mut(|jobs| {
        if let Some(job) = jobs.iter_mut().find(|job| job.id == id) {
            job.millis = job
                .started
                .take()
                .map_or(0, |start| start.elapsed().as_millis());
            job.done = job.total;
            (job.phase, job.note) = match result {
                Ok(note) => (Phase::Ready, note),
                Err(error) => (Phase::Failed, error),
            };
        }
    });
}
pub(super) fn jobs() -> Vec<Job> {
    jobs_mut(|jobs| jobs.clone())
}
pub(super) fn job(id: &str) -> Option<Job> {
    jobs_mut(|jobs| jobs.iter().find(|job| job.id == id).cloned())
}

/// Rendered view bodies, valid until any watched input changes. Views read
/// only the working tree, generated reports and the local ROMs, so one
/// generation counter bumped by the watcher invalidates them all at once.
static GENERATION: Mutex<u64> = Mutex::new(0);
/// A rendered view body and its status-bar note.
pub(super) type View = (String, String);
static VIEWS: Mutex<BTreeMap<String, (u64, View)>> = Mutex::new(BTreeMap::new());
const VIEW_LIMIT: usize = 64;
pub(super) fn generation() -> u64 {
    *GENERATION.lock().unwrap_or_else(|error| error.into_inner())
}
pub(super) fn invalidate() {
    *GENERATION.lock().unwrap_or_else(|error| error.into_inner()) += 1;
}
pub(super) fn view<E>(key: &str, render: impl FnOnce() -> Result<View, E>) -> Result<View, E> {
    let current = generation();
    if let Some((generation, view)) = VIEWS
        .lock()
        .unwrap_or_else(|error| error.into_inner())
        .get(key)
    {
        if *generation == current {
            return Ok(view.clone());
        }
    }
    let view = render()?;
    let mut views = VIEWS.lock().unwrap_or_else(|error| error.into_inner());
    views.retain(|_, (generation, _)| *generation == current);
    if views.len() >= VIEW_LIMIT {
        if let Some(oldest) = views.keys().next().cloned() {
            views.remove(&oldest);
        }
    }
    views.insert(key.into(), (current, view.clone()));
    Ok(view)
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn store_keeps_one_file_per_name_and_rebuilds_only_new_stamps() {
        let root = tempfile::tempdir().unwrap();
        let store = Store::at(root.path());
        let mut builds = 0;
        for stamp in ["0123456789abcdef", "0123456789abcdef", "fedcba9876543210"] {
            let bytes = store
                .get_or_build("font", stamp, "ttf", || {
                    builds += 1;
                    Ok(stamp.as_bytes().to_vec())
                })
                .unwrap();
            assert_eq!(bytes, stamp.as_bytes());
        }
        assert_eq!(builds, 2);
        store
            .save("fonts", "0000000000000000", "ttf", b"x")
            .unwrap();
        let mut names = std::fs::read_dir(root.path().join(DIRECTORY))
            .unwrap()
            .map(|entry| entry.unwrap().file_name().into_string().unwrap())
            .collect::<Vec<_>>();
        names.sort();
        assert_eq!(
            names,
            ["font-fedcba9876543210.ttf", "fonts-0000000000000000.ttf"]
        );
    }
    #[test]
    fn metadata_stamps_change_with_contents_and_absence() {
        let root = tempfile::tempdir().unwrap();
        let path = root.path().join("input.json");
        let missing = metadata_stamp(&[path.clone()]);
        std::fs::write(&path, b"{}").unwrap();
        let present = metadata_stamp(&[path.clone()]);
        std::fs::write(&path, b"{\"a\":1}").unwrap();
        assert_ne!(missing, present);
        assert_ne!(present, metadata_stamp(&[path]));
    }
    #[test]
    fn jobs_report_progress_then_completion() {
        begin("test-job", "Testing", 4);
        advance("test-job", 2, "half");
        let running = job("test-job").unwrap();
        assert_eq!(
            (running.phase.clone(), running.fraction()),
            (Phase::Running, 0.5)
        );
        finish("test-job", Err("broken".into()));
        let failed = job("test-job").unwrap();
        assert_eq!(
            (failed.phase, failed.note.as_str()),
            (Phase::Failed, "broken")
        );
    }
    #[test]
    fn views_render_once_per_generation() {
        let mut renders = 0;
        for _ in 0..2 {
            let view = view::<()>("/test-view", || {
                renders += 1;
                Ok(("<i>x</i>".into(), "note".into()))
            });
            assert_eq!(view.unwrap().0, "<i>x</i>");
        }
        assert_eq!(renders, 1);
        invalidate();
        view::<()>("/test-view", || {
            renders += 1;
            Ok(Default::default())
        })
        .unwrap();
        assert_eq!(renders, 2);
    }
}
