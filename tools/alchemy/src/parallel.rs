//! Host-sized worker pools for independent build jobs.

/// Number of workers for `tasks` independent jobs.
///
/// macOS reports the online logical CPUs through `sysconf`. Rust's
/// affinity-aware fallback is useful on other hosts. `ALCHEMY_JOBS` remains a
/// deliberate override for constrained builders.
pub fn workers(tasks: usize) -> usize {
    let fallback = std::thread::available_parallelism().map_or(1, usize::from);
    #[cfg(unix)]
    let detected = {
        // SAFETY: sysconf reads one process-wide integer and owns no pointers.
        let count = unsafe { libc::sysconf(libc::_SC_NPROCESSORS_ONLN) };
        usize::try_from(count).ok().filter(|count| *count > 0)
    };
    #[cfg(not(unix))]
    let detected = None;

    std::env::var("ALCHEMY_JOBS")
        .ok()
        .and_then(|value| value.parse::<usize>().ok())
        .filter(|count| *count > 0)
        .or(detected)
        .unwrap_or(fallback)
        .min(tasks.max(1))
}

#[cfg(test)]
mod tests {
    use super::workers;

    #[test]
    fn never_starts_more_workers_than_jobs() {
        assert_eq!(workers(0), 1);
        assert_eq!(workers(1), 1);
        assert!(workers(7) <= 7);
    }
}
