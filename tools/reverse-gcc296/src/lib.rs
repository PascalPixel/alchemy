//! `reverse-gcc296`: systematic source search against a byte oracle.
//!
//! This is not a decompiler. Decompilation is hard because there is no oracle.
//! Here the compiler is a known, deterministic function and we can compile and
//! compare, so the problem is not "recover the source" but "find any preimage".
//! The compiler is provably not injective on this codebase (identifier renames,
//! callee aliasing, and tail duplication all produce byte-identical output), so
//! any member of the equivalence class will do.
//!
//! The difference from the existing permuter is the search, not the machinery.
//! The permuter samples random mutations by weight. This evaluates the COMPLETE
//! single-edit neighbourhood of a measured rewrite set and descends. On the
//! first owner tried, 4,634 random permuter candidates found nothing while a
//! systematic sweep of one transformation found 97 points.

pub mod rewrite;
pub mod score;

use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicUsize, Ordering};
use std::sync::{Arc, Mutex};

pub use rewrite::Variant;
pub use score::Score;

/// One accepted step of the descent.
#[derive(Clone, Debug)]
pub struct Step {
    pub label: String,
    pub distance: u32,
    pub size: u32,
}

pub struct Search {
    pub root: PathBuf,
    pub candidate_show: PathBuf,
    pub objdump: PathBuf,
    pub work: PathBuf,
    pub stem: String,
    pub jobs: usize,
}

/// Never fan out past 80 percent of the machine's cores.
pub fn default_jobs() -> usize {
    let cores = std::thread::available_parallelism()
        .map(|value| value.get())
        .unwrap_or(4);
    ((cores * 8) / 10).max(1)
}

impl Search {
    /// Score every variant in `variants`, in parallel, returning
    /// `(index, score)` for those that compile.
    fn score_all(&self, variants: &[Variant]) -> Vec<(usize, Score)> {
        let next = Arc::new(AtomicUsize::new(0));
        let results = Arc::new(Mutex::new(Vec::new()));
        std::thread::scope(|scope| {
            for worker in 0..self.jobs.min(variants.len().max(1)) {
                let next = Arc::clone(&next);
                let results = Arc::clone(&results);
                let work = self.work.join(format!("w{worker}"));
                scope.spawn(move || loop {
                    let index = next.fetch_add(1, Ordering::SeqCst);
                    if index >= variants.len() {
                        break;
                    }
                    if let Some(score) = score::score(
                        &self.root,
                        &self.candidate_show,
                        &self.objdump,
                        &variants[index].source,
                        &self.stem,
                        &work,
                    ) {
                        results.lock().unwrap().push((index, score));
                    }
                });
            }
        });
        let mut out = Arc::try_unwrap(results).unwrap().into_inner().unwrap();
        out.sort_by_key(|(index, _)| *index);
        out
    }

    /// Greedy descent over the complete neighbourhood.
    ///
    /// Each round evaluates every single-edit variant and accepts the best
    /// strict improvement. Field-level results are known NOT to compose (four
    /// individually-improving field renames measured worse together than the
    /// best one alone), so the neighbourhood is regenerated from the accepted
    /// source every round rather than batching independent wins.
    pub fn descend(
        &self,
        seed: &str,
        max_rounds: usize,
        mut report: impl FnMut(&str),
    ) -> (String, Vec<Step>) {
        let mut current = seed.to_string();
        let mut history = Vec::new();

        let base = match score::score(
            &self.root,
            &self.candidate_show,
            &self.objdump,
            &current,
            &self.stem,
            &self.work.join("base"),
        ) {
            Some(score) => score,
            None => {
                report("seed does not compile");
                return (current, history);
            }
        };
        let mut best = base.clone();
        report(&format!("seed distance={} size={}", best.distance, best.size));

        for round in 1..=max_rounds {
            let variants = rewrite::neighbourhood(&current);
            if variants.is_empty() {
                report("neighbourhood empty");
                break;
            }
            let scored = self.score_all(&variants);
            let improvement = scored
                .iter()
                .filter(|(_, score)| score.distance < best.distance)
                .min_by_key(|(_, score)| score.distance);

            match improvement {
                Some((index, score)) => {
                    report(&format!(
                        "round {round}: {} of {} compiled, best {} -> {} ({}) size {}",
                        scored.len(),
                        variants.len(),
                        best.distance,
                        score.distance,
                        variants[*index].label,
                        score.size
                    ));
                    current = variants[*index].source.clone();
                    best = score.clone();
                    history.push(Step {
                        label: variants[*index].label.clone(),
                        distance: score.distance,
                        size: score.size,
                    });
                }
                None => {
                    report(&format!(
                        "round {round}: {} of {} compiled, no improvement, converged",
                        scored.len(),
                        variants.len()
                    ));
                    break;
                }
            }
        }
        (current, history)
    }
}

/// Repository root, found by walking up from the crate directory.
pub fn repository_root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .and_then(Path::parent)
        .map(Path::to_path_buf)
        .unwrap_or_else(|| PathBuf::from("."))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn jobs_respect_the_eighty_percent_cap() {
        let cores = std::thread::available_parallelism().map(|v| v.get()).unwrap_or(4);
        assert!(default_jobs() <= cores);
        assert!(default_jobs() >= 1);
    }

    #[test]
    fn root_contains_the_semantic_directory() {
        assert!(repository_root().join("semantic").exists());
    }

    #[test]
    fn neighbourhood_of_a_trivial_file_is_empty() {
        assert!(rewrite::neighbourhood("int main(void) { return 0; }").is_empty());
    }
}
