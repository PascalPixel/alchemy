use std::path::PathBuf;

pub const MAX_ITERATIONS: usize = 100_000;
pub const MAX_SOURCE_BYTES: usize = 8 * 1024 * 1024;
pub const MAX_PLAN_BYTES: usize = 128 * 1024 * 1024;
pub const MAX_OUTPUT_BYTES: usize = 32 * 1024 * 1024;
pub const MAX_TOP: usize = 256;
pub const MAX_SUMMARY_BYTES: usize = 1024;
pub const MAX_JOURNAL_ROW_BYTES: usize = 2 * 1024;

pub const USAGE: &str = "usage: permuter <candidate.c>... [options]\n\
  --iterations N   bounded generated candidates (default 1000)\n\
  --jobs N, -j N   parallel compiler workers (default CPUs-2; explicit max 64)\n\
  --seed N         deterministic random seed (default 1)\n\
  --top N          retained improving candidates (default 12; max 256)\n\
  --output DIR     new, dedicated ignored run directory (or parent for many inputs)\n\
  --weights FILE   settings.toml weights for planned search (not AST --walk)\n\
  --chain N        planned-search rounds re-seeded from the best candidate\n\
  --walk           pret-style cumulative mutation walk (uses --iterations as total evals)\n\
  --classic        enable heuristic pret passes; trust exact winners only\n\
  --keep-prob P    AST-walk continuation probability in 0..1 (default 0.6)\n\
  --manual-only    planned search: evaluate PERM_* choices without mutation\n\
  --show-errors    display failed compiler diagnostics\n\
  --continue-on-error  finish all inputs, then fail if any input failed\n\
  --better-only    retain only candidates better than the baseline\n\
  --best-only      retain only ties with or improvements over the current best\n\
  --only-if-below N retain only scores below N\n\
  --quiet          suppress periodic progress output\n\
  --debug          compile and report only the baseline\n\
  --resume         reuse completed candidates from a matching journal\n\
  --journal-from FILE  import cached measurements from a prior run's journal\n\
  --family-template FILE  bias AST mutations from an exact family member\n\
  --heat           AST walk: bias sites toward currently-differing rows\n\
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
    pub classic: bool,
    pub keep_prob_permille: u32,
    pub manual_only: bool,
    pub stop_exact: bool,
    pub show_errors: bool,
    pub continue_on_error: bool,
    pub better_only: bool,
    pub best_only: bool,
    pub score_threshold: Option<u64>,
    pub quiet: bool,
    pub debug: bool,
    pub resume: bool,
    pub journal_from: Option<std::path::PathBuf>,
    pub family_template: Option<std::path::PathBuf>,
    pub heat: bool,
}

fn positive(value: Option<&String>, flag: &str) -> Result<usize, String> {
    value.ok_or_else(|| format!("{flag} requires a value"))?.parse::<usize>().ok().filter(|number| *number > 0).ok_or_else(|| format!("{flag} must be a positive integer"))
}

impl Options {
    pub fn parse(args: &[String]) -> Result<Self, String> {
        let mut candidates = Vec::new();
        let mut iterations = 1000usize;
        let mut jobs = std::thread::available_parallelism().map(|count| count.get().saturating_sub(2).clamp(1, 16)).unwrap_or(1);
        let mut seed = 1u64;
        let mut top = 12usize;
        let mut output = None;
        let mut weights = None;
        let mut chain = 1usize;
        let mut chain_supplied = false;
        let mut walk = false;
        let mut classic = false;
        let mut keep_prob_permille = 600u32;
        let mut keep_prob_supplied = false;
        let mut manual_only = false;
        let mut stop_exact = true;
        let mut show_errors = false;
        let mut continue_on_error = false;
        let mut better_only = false;
        let mut best_only = false;
        let mut score_threshold = None;
        let mut quiet = false;
        let mut debug = false;
        let mut resume = false;
        let mut journal_from: Option<std::path::PathBuf> = None;
        let mut family_template: Option<std::path::PathBuf> = None;
        let mut heat = false;
        let mut at = 0usize;
        while at < args.len() {
            match args[at].as_str() {
                "--weights" => {
                    let value = args.get(at + 1).ok_or_else(|| "--weights requires a path".to_string())?;
                    weights = Some(PathBuf::from(value));
                    at += 2;
                }
                "--walk" => {
                    walk = true;
                    at += 1;
                }
                "--classic" => {
                    classic = true;
                    at += 1;
                }
                "--keep-prob" => {
                    let value = args.get(at + 1).and_then(|v| v.parse::<f64>().ok()).filter(|v| (0.0..=1.0).contains(v)).ok_or_else(|| "--keep-prob requires a float in 0..1".to_string())?;
                    keep_prob_permille = (value * 1000.0).round() as u32;
                    keep_prob_supplied = true;
                    at += 2;
                }
                "--chain" => {
                    chain = positive(args.get(at + 1), "--chain")?.min(10_000);
                    chain_supplied = true;
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
                    seed = args.get(at + 1).ok_or("--seed requires a value")?.parse::<u64>().map_err(|_| "--seed must be an unsigned integer")?;
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
                    output = Some(PathBuf::from(args.get(at + 1).ok_or("--output requires a value")?));
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
                "--continue-on-error" => {
                    continue_on_error = true;
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
                    score_threshold = Some(args.get(at + 1).ok_or("--only-if-below requires a value")?.parse::<u64>().map_err(|_| "--only-if-below must be an unsigned integer")?);
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
                    journal_from = Some(std::path::PathBuf::from(args.get(at + 1).ok_or("--journal-from requires a value")?));
                    at += 2;
                }
                "--family-template" => {
                    family_template = Some(std::path::PathBuf::from(args.get(at + 1).ok_or("--family-template requires a value")?));
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
        if walk && weights.is_some() {
            return Err("--weights is not supported with --walk; choose safe or --classic mode".to_string());
        }
        if walk && chain_supplied {
            return Err("--chain is not supported with --walk; the walk is already cumulative".to_string());
        }
        if walk && manual_only {
            return Err("--manual-only is not supported with --walk".to_string());
        }
        if classic && !walk {
            return Err("--classic requires --walk".to_string());
        }
        if !walk && keep_prob_supplied {
            return Err("--keep-prob requires --walk".to_string());
        }
        if !walk && heat {
            return Err("--heat requires --walk".to_string());
        }
        if !walk && family_template.is_some() {
            return Err("--family-template requires --walk".to_string());
        }
        Ok(Self { candidates, iterations, jobs, seed, top, output, weights, chain, walk, classic, keep_prob_permille, manual_only, stop_exact, show_errors, continue_on_error, better_only, best_only, score_threshold, quiet, debug, resume, journal_from, family_template, heat })
    }
}

pub fn self_test() -> Result<(), String> {
    let args = ["games/gs1/recon/en/overlays/resource_373_c_02005b48.c", "--iterations", "9", "--jobs", "2", "--seed", "7", "--walk", "--classic", "--continue-on-error", "--heat", "--family-template", "template.c"].into_iter().map(str::to_string).collect::<Vec<_>>();
    let options = Options::parse(&args)?;
    if options.iterations != 9 || options.jobs != 2 || options.seed != 7 || !options.classic || !options.continue_on_error || !options.heat || options.family_template.as_deref() != Some(std::path::Path::new("template.c")) {
        return Err("option parser lost a supplied value".into());
    }
    if Options::parse(&["x.c".into(), "--family-template".into(), "t.c".into()]).is_ok() {
        return Err("option parser accepted a family template outside AST walk".into());
    }
    if options.candidates != [PathBuf::from("games/gs1/recon/en/overlays/resource_373_c_02005b48.c")] {
        return Err("option parser lost its candidate list".into());
    }
    if Options::parse(&["--jobs".into(), "0".into(), "x.c".into()]).is_ok() {
        return Err("option parser accepted zero jobs".into());
    }
    if Options::parse(&["--iterations".into(), (MAX_ITERATIONS + 1).to_string(), "x.c".into()]).is_ok() {
        return Err("option parser accepted an unbounded iteration request".into());
    }
    if Options::parse(&["x.c".into(), "--walk".into(), "--weights".into(), "settings.toml".into()]).is_ok() {
        return Err("option parser silently ignored walk weights".into());
    }
    Ok(())
}
