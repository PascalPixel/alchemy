// How many things this repository may run at once.
//
// 80% of the cores, never all of them. Every parallel runner here spawns a full
// child process that loads a large part of the tool tree, so "cores - 2" is not
// a 2-core reserve, it is 16 concurrent runtimes. Doing that to probe 95
// subcommands drove an 18-core, 128 GB machine into swap and a watchdog panic.
// The headroom is not for the scheduler, it is for the rest of the machine.
//
//   cargo run -p jobs                 # print the cap
//   cargo run -p jobs -- --self-test
//
// Native shared job-cap helper. The self-test expectations below are
// verbatim from that file so the two agree by construction while both exist.

use std::thread;

pub const SHARE: f64 = 0.8;

pub fn jobs_for(cores: usize, share: f64) -> usize {
    let scaled = (cores as f64 * share).floor() as usize;
    scaled.max(1)
}

/// Callers accept --jobs; this clamps whatever they were given. An explicit
/// number above the cap is still a mistake, just a deliberate-looking one.
pub fn resolve_jobs(requested: Option<i64>, cores: usize) -> usize {
    let cap = jobs_for(cores, SHARE);
    match requested {
        Some(n) if n >= 1 => (n as usize).min(cap),
        _ => cap,
    }
}

pub fn available_parallelism() -> usize {
    thread::available_parallelism()
        .map(|n| n.get())
        .unwrap_or(1)
}

fn self_test() {
    assert_eq!(jobs_for(18, SHARE), 14, "18 cores must give 14");
    assert_eq!(
        jobs_for(1, SHARE),
        1,
        "a single core must still run one job"
    );
    assert_eq!(jobs_for(2, SHARE), 1, "two cores must give one job");
    assert_eq!(
        resolve_jobs(Some(4), 18),
        4,
        "a request under the cap must be honoured"
    );
    assert_eq!(
        resolve_jobs(Some(64), 18),
        14,
        "a request over the cap must be clamped"
    );
    assert_eq!(
        resolve_jobs(Some(0), 18),
        14,
        "a nonsense request must fall back to the cap"
    );
    assert_eq!(resolve_jobs(None, 18), 14, "no request must give the cap");
    println!("jobs self-test ok");
}

fn main() {
    if std::env::args().any(|a| a == "--self-test") {
        self_test();
    } else {
        let cores = available_parallelism();
        println!(
            "{} of {} cores ({}%)",
            resolve_jobs(None, cores),
            cores,
            SHARE * 100.0
        );
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn matches_typescript_expectations() {
        assert_eq!(jobs_for(18, SHARE), 14);
        assert_eq!(jobs_for(1, SHARE), 1);
        assert_eq!(jobs_for(2, SHARE), 1);
        assert_eq!(resolve_jobs(Some(4), 18), 4);
        assert_eq!(resolve_jobs(Some(64), 18), 14);
        assert_eq!(resolve_jobs(Some(0), 18), 14);
        assert_eq!(resolve_jobs(None, 18), 14);
    }
}
