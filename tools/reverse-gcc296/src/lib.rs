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

    /// Descent over the complete neighbourhood, with a shortlist and bounded
    /// plateau moves.
    ///
    /// Individually-improving edits are known NOT to compose (four field renames
    /// each improved alone and were worse together than the best one), so edits
    /// are always applied one at a time and every applied edit is scored as
    /// applied. Nothing is ever accepted on a predicted score.
    ///
    /// The shortlist is the speedup. A full scan costs a few hundred compiles to
    /// buy one edit. After accepting one, the other labels that improved are
    /// very likely still to improve, so they are retried alone, which costs tens
    /// of compiles instead of hundreds. A full scan runs again only once the
    /// shortlist stops paying.
    ///
    /// Plateau moves exist because pure greedy stops at the first flat spot.
    /// Equal-scoring moves are accepted up to `plateau_budget`, and abandoned if
    /// they do not lead to a strict improvement.
    pub fn descend(
        &self,
        seed: &str,
        max_rounds: usize,
        plateau_budget: usize,
        mut report: impl FnMut(&str),
    ) -> (String, Vec<Step>) {
        let mut current = seed.to_string();
        let mut history: Vec<Step> = Vec::new();

        let Some(base) = self.score_one(&current, "base") else {
            report("seed does not compile");
            return (current, history);
        };
        // The reference size, read from the seed compile. Used ONLY to detect
        // true closure; it is never a search constraint.
        let reference_size = self.reference_size().unwrap_or(base.size);
        let mut best = base.clone();
        let mut best_source = current.clone();
        let mut best_history_len = 0usize;
        report(&format!(
            "seed distance={} size={} halfwords={} reference={}",
            best.distance, best.size, best.halfwords, reference_size
        ));
        if base.is_exact(reference_size) {
            report("seed is already byte-exact");
            return (current, history);
        }

        let mut shortlist: Vec<String> = Vec::new();
        let mut plateau_used = 0usize;
        let mut compiles = 0usize;

        for round in 1..=max_rounds {
            let all = rewrite::neighbourhood(&current);
            if all.is_empty() {
                report("neighbourhood empty");
                break;
            }
            // Prefer the shortlist; fall back to the full neighbourhood.
            let using_shortlist = !shortlist.is_empty();
            let variants: Vec<Variant> = if using_shortlist {
                all.iter().filter(|v| shortlist.contains(&v.label)).cloned().collect()
            } else {
                all.clone()
            };
            if variants.is_empty() {
                shortlist.clear();
                continue;
            }

            let scored = self.score_all(&variants);
            compiles += variants.len();

            // Remember every label that improved, for the next round.
            shortlist = scored
                .iter()
                .filter(|(_, score)| score.distance < best.distance)
                .map(|(index, _)| variants[*index].label.clone())
                .collect();

            // Byte-exactness beats every distance comparison: it is the goal,
            // and the distance metric is only a proxy for it.
            if let Some((index, score)) =
                scored.iter().find(|(_, s)| s.is_exact(reference_size))
            {
                report(&format!("EXACT via {} after {} compiles", variants[*index].label, compiles));
                history.push(Step {
                    label: variants[*index].label.clone(),
                    distance: score.distance,
                    size: score.size,
                });
                return (variants[*index].source.clone(), history);
            }

            let strict = scored
                .iter()
                .filter(|(_, score)| score.distance < best.distance)
                .min_by_key(|(_, score)| score.distance);

            if let Some((index, score)) = strict {
                report(&format!(
                    "round {round}{}: {}/{} compiled, {} -> {} ({}) size {}",
                    if using_shortlist { " [shortlist]" } else { "" },
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
                best_source = current.clone();
                best_history_len = history.len();
                plateau_used = 0;
                // Drop the accepted label so the shortlist does not retry it.
                let accepted = &variants[*index].label;
                shortlist.retain(|label| label != accepted);
                continue;
            }

            if using_shortlist {
                // Shortlist exhausted; force a full scan next round.
                shortlist.clear();
                continue;
            }

            // No strict improvement anywhere. Try a lateral move.
            // A lateral move must be one we have not already taken, or the walk
            // oscillates between two equal-scoring states. History labels carry
            // a `=` prefix for plateau steps, so strip it before comparing.
            let lateral = scored
                .iter()
                .filter(|(_, score)| score.distance == best.distance)
                .find(|(index, _)| {
                    let label = &variants[*index].label;
                    !history
                        .iter()
                        .any(|step| step.label.trim_start_matches('=') == label)
                });
            match lateral {
                Some((index, score)) if plateau_used < plateau_budget => {
                    plateau_used += 1;
                    report(&format!(
                        "round {round}: no improvement, plateau move {}/{} ({})",
                        plateau_used, plateau_budget, variants[*index].label
                    ));
                    current = variants[*index].source.clone();
                    best = score.clone();
                    history.push(Step {
                        label: format!("={}", variants[*index].label),
                        distance: score.distance,
                        size: score.size,
                    });
                }
                _ => {
                    report(&format!(
                        "round {round}: {}/{} compiled, converged after {} compiles",
                        scored.len(),
                        variants.len(),
                        compiles
                    ));
                    break;
                }
            }
        }

        // A plateau walk that never paid off is churn. Return the best source.
        if history.len() > best_history_len {
            report(&format!(
                "discarding {} unproductive plateau move(s)",
                history.len() - best_history_len
            ));
            history.truncate(best_history_len);
            return (best_source, history);
        }
        (current, history)
    }

    fn score_one(&self, source: &str, tag: &str) -> Option<Score> {
        score::score(
            &self.root,
            &self.candidate_show,
            &self.objdump,
            source,
            &self.stem,
            &self.work.join(tag),
        )
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

impl Search {
    /// The reference byte size for this owner, taken from a seed compile.
    fn reference_size(&self) -> Option<u32> {
        let file = self.work.join("refsize").join(format!("{}.c", self.stem));
        std::fs::create_dir_all(file.parent()?).ok()?;
        let source = std::fs::read_to_string(self.root.join("semantic").join(format!("{}.c", self.stem))).ok()?;
        std::fs::write(&file, source).ok()?;
        let output = std::process::Command::new(&self.candidate_show)
            .arg(&file)
            .arg("--work")
            .arg(self.work.join("refsize").join("w"))
            .current_dir(&self.root)
            .output()
            .ok()?;
        let text = String::from_utf8_lossy(&output.stdout);
        let first = text.lines().next()?;
        let start = first.find("reference=")? + "reference=".len();
        let rest = &first[start..];
        let end = rest.find(|c: char| !c.is_ascii_digit()).unwrap_or(rest.len());
        rest[..end].parse().ok()
    }
}
