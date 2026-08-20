use std::path::PathBuf;

pub const MAX_ITERATIONS: usize = 100_000;
pub const MAX_SOURCE_BYTES: usize = 8 * 1024 * 1024;
pub const MAX_PLAN_BYTES: usize = 128 * 1024 * 1024;
pub const MAX_OUTPUT_BYTES: usize = 32 * 1024 * 1024;
pub const MAX_TOP: usize = 256;
pub const MAX_SUMMARY_BYTES: usize = 1024;
pub const MAX_JOURNAL_ROW_BYTES: usize = 2 * 1024;

pub const USAGE: &str = "usage: permuter <candidate.c|legacy-directory>... [options]\n\
  --iterations N   bounded generated candidates (default 1000)\n\
  --jobs N, -j N   parallel compiler workers (default CPUs-2; explicit max 64)\n\
  --seed N         deterministic random seed (default 1)\n\
  --top N          retained improving candidates (default 12; max 256)\n\
  --output DIR     new, dedicated ignored run directory (or parent for many inputs)\n\
  --weights FILE   settings.toml-format randomization weights for any input kind\n\
  --chain N        rounds that re-seed mutation from the best candidate so far\n\
  --walk           pret-style cumulative mutation walk (uses --iterations as total evals)\n\
  --keep-prob P    walk continuation probability in 0..1 (default 0.6)\n\
  --manual-only    evaluate PERM_* choices without random mutation\n\
  --show-errors    display failed compiler diagnostics\n\
  --better-only    retain only candidates better than the baseline\n\
  --best-only      retain only ties with or improvements over the current best\n\
  --only-if-below N retain only scores below N\n\
  --quiet          suppress periodic progress output\n\
  --debug          compile and report only the baseline\n\
  --resume         reuse completed candidates from a matching journal\n\
  --journal-from FILE  import cached measurements from a prior run's journal\n\
  --heat           bias mutation sites toward currently-differing rows\n\
  --stop-on-zero   stop when an exact candidate is found (default)\n\
  --keep-going     do not stop at the first byte-exact candidate\n\
  --self-test      run parser, mutation, scoring, and runner checks";

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Options {
    pub candidates: Vec<PathBuf>,
    pub iterations: usize,
    pub jobs: usize,
    pub seed: u64,
    pub top: usize,
    pub output: Option<PathBuf>,
    pub weights: Option<PathBuf>,
    pub chain: usize,
    pub walk: bool,
    pub keep_prob_permille: u32,
    pub manual_only: bool,
    pub stop_exact: bool,
    pub show_errors: bool,
    pub better_only: bool,
    pub best_only: bool,
    pub score_threshold: Option<u64>,
    pub quiet: bool,
    pub debug: bool,
    pub resume: bool,
    pub journal_from: Option<std::path::PathBuf>,
    pub heat: bool,
}

fn positive(value: Option<&String>, flag: &str) -> Result<usize, String> {
    value
        .ok_or_else(|| format!("{flag} requires a value"))?
        .parse::<usize>()
        .ok()
        .filter(|number| *number > 0)
        .ok_or_else(|| format!("{flag} must be a positive integer"))
}

impl Options {
    pub fn parse(args: &[String]) -> Result<Self, String> {
        let mut candidates = Vec::new();
        let mut iterations = 1000usize;
        let mut jobs = std::thread::available_parallelism()
            .map(|count| count.get().saturating_sub(2).clamp(1, 16))
            .unwrap_or(1);
        let mut seed = 1u64;
        let mut top = 12usize;
        let mut output = None;
        let mut weights = None;
        let mut chain = 1usize;
        let mut walk = false;
        let mut keep_prob_permille = 600u32;
        let mut manual_only = false;
        let mut stop_exact = true;
        let mut show_errors = false;
        let mut better_only = false;
        let mut best_only = false;
        let mut score_threshold = None;
        let mut quiet = false;
        let mut debug = false;
        let mut resume = false;
        let mut journal_from: Option<std::path::PathBuf> = None;
        let mut heat = false;
        let mut at = 0usize;
        while at < args.len() {
            match args[at].as_str() {
                "--weights" => {
                    let value = args
                        .get(at + 1)
                        .ok_or_else(|| "--weights requires a path".to_string())?;
                    weights = Some(PathBuf::from(value));
                    at += 2;
                }
                "--walk" => {
                    walk = true;
                    at += 1;
                }
                "--keep-prob" => {
                    let value = args
                        .get(at + 1)
                        .and_then(|v| v.parse::<f64>().ok())
                        .filter(|v| (0.0..=1.0).contains(v))
                        .ok_or_else(|| "--keep-prob requires a float in 0..1".to_string())?;
                    keep_prob_permille = (value * 1000.0).round() as u32;
                    at += 2;
                }
                "--chain" => {
                    chain = positive(args.get(at + 1), "--chain")?.min(10_000);
                    at += 2;
                }
                "--iterations" => {
                    iterations = positive(args.get(at + 1), "--iterations")?;
                    if iterations > MAX_ITERATIONS {
                        return Err(format!("--iterations must not exceed {MAX_ITERATIONS}"));
                    }
                    at += 2;
                }
                "--jobs" | "-j" => {
                    jobs = positive(args.get(at + 1), "--jobs")?.min(64);
                    at += 2;
                }
                "--seed" => {
                    seed = args
                        .get(at + 1)
                        .ok_or("--seed requires a value")?
                        .parse::<u64>()
                        .map_err(|_| "--seed must be an unsigned integer")?;
                    at += 2;
                }
                "--top" => {
                    top = positive(args.get(at + 1), "--top")?;
                    if top > MAX_TOP {
                        return Err(format!("--top must not exceed {MAX_TOP}"));
                    }
                    at += 2;
                }
                "--output" => {
                    output = Some(PathBuf::from(
                        args.get(at + 1).ok_or("--output requires a value")?,
                    ));
                    at += 2;
                }
                "--manual-only" => {
                    manual_only = true;
                    at += 1;
                }
                "--show-errors" => {
                    show_errors = true;
                    at += 1;
                }
                "--better-only" => {
                    better_only = true;
                    at += 1;
                }
                "--best-only" => {
                    best_only = true;
                    at += 1;
                }
                "--only-if-below" => {
                    score_threshold = Some(
                        args.get(at + 1)
                            .ok_or("--only-if-below requires a value")?
                            .parse::<u64>()
                            .map_err(|_| "--only-if-below must be an unsigned integer")?,
                    );
                    at += 2;
                }
                "--quiet" => {
                    quiet = true;
                    at += 1;
                }
                "--debug" => {
                    debug = true;
                    at += 1;
                }
                "--journal-from" => {
                    journal_from = Some(std::path::PathBuf::from(
                        args.get(at + 1).ok_or("--journal-from requires a value")?,
                    ));
                    at += 2;
                }
                "--heat" => {
                    heat = true;
                    at += 1;
                }
                "--resume" => {
                    resume = true;
                    at += 1;
                }
                "--stop-on-zero" => {
                    stop_exact = true;
                    at += 1;
                }
                "--keep-going" => {
                    stop_exact = false;
                    at += 1;
                }
                "--help" | "-h" => return Err(USAGE.to_string()),
                value if value.starts_with('-') => {
                    return Err(format!("unknown option {value}\n{USAGE}"));
                }
                value => {
                    candidates.push(PathBuf::from(value));
                    at += 1;
                }
            }
        }
        if candidates.is_empty() {
            return Err(USAGE.to_string());
        }
        Ok(Self {
            candidates,
            iterations,
            jobs,
            seed,
            top,
            output,
            weights,
            chain,
            walk,
            keep_prob_permille,
            manual_only,
            stop_exact,
            show_errors,
            better_only,
            best_only,
            score_threshold,
            quiet,
            debug,
            resume,
            journal_from,
            heat,
        })
    }
}

pub fn self_test() -> Result<(), String> {
    let args = [
        "semantic/resource_373_c_02005b48.c",
        "--iterations",
        "9",
        "--jobs",
        "2",
        "--seed",
        "7",
        "--manual-only",
    ]
    .into_iter()
    .map(str::to_string)
    .collect::<Vec<_>>();
    let options = Options::parse(&args)?;
    if options.iterations != 9 || options.jobs != 2 || options.seed != 7 || !options.manual_only {
        return Err("option parser lost a supplied value".into());
    }
    if options.candidates != [PathBuf::from("semantic/resource_373_c_02005b48.c")] {
        return Err("option parser lost its candidate list".into());
    }
    if Options::parse(&["--jobs".into(), "0".into(), "x.c".into()]).is_ok() {
        return Err("option parser accepted zero jobs".into());
    }
    if Options::parse(&[
        "--iterations".into(),
        (MAX_ITERATIONS + 1).to_string(),
        "x.c".into(),
    ])
    .is_ok()
    {
        return Err("option parser accepted an unbounded iteration request".into());
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::{Options, MAX_ITERATIONS, MAX_TOP};

    #[test]
    fn rejects_iterations_above_hard_cap() {
        let args = [
            "--iterations".to_string(),
            (MAX_ITERATIONS + 1).to_string(),
            "candidate.c".to_string(),
        ];
        assert!(Options::parse(&args).is_err());
    }

    #[test]
    fn rejects_unbounded_retained_output() {
        let args = [
            "--top".to_string(),
            (MAX_TOP + 1).to_string(),
            "candidate.c".to_string(),
        ];
        assert!(Options::parse(&args).is_err());
    }
}
