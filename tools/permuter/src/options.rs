use std::path::PathBuf;

pub const MAX_ITERATIONS: usize = 100_000;
pub const MAX_JOBS: usize = 64;
pub const MAX_SOURCE_BYTES: usize = 8 * 1024 * 1024;
pub const MAX_TOP: usize = 256;

pub const USAGE: &str = "usage: permuter <candidate.c>... [options]\n\
  Decodes allocator evidence and searches a finite catalog over ordinary C.\n\
\n\
  --iterations N       candidates to compile (default 1000; max 100000)\n\
  --jobs N, -j N       parallel compiler workers (default CPUs-2; max 64)\n\
  --seed N             deterministic starting order (default 1)\n\
  --top N              ranked sources to retain (default 12; max 256)\n\
  --output DIR         fresh run directory, or parent for multiple inputs\n\
  --show-errors        print failed candidate diagnostics\n\
  --continue-on-error  process every input before returning failure\n\
  --self-test          run parser, ordering, and byte-score checks";

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Options {
    pub candidates: Vec<PathBuf>,
    pub iterations: usize,
    pub jobs: usize,
    pub seed: u64,
    pub top: usize,
    pub output: Option<PathBuf>,
    pub show_errors: bool,
    pub continue_on_error: bool,
}

fn value<'a>(args: &'a [String], at: usize, flag: &str) -> Result<&'a str, String> {
    args.get(at + 1)
        .map(String::as_str)
        .ok_or_else(|| format!("{flag} requires a value"))
}

fn bounded(value: &str, flag: &str, maximum: usize) -> Result<usize, String> {
    let number = value
        .parse::<usize>()
        .ok()
        .filter(|number| *number > 0)
        .ok_or_else(|| format!("{flag} must be a positive integer"))?;
    if number > maximum {
        return Err(format!("{flag} must not exceed {maximum}"));
    }
    Ok(number)
}

impl Options {
    pub fn parse(args: &[String]) -> Result<Self, String> {
        let mut options = Self {
            candidates: Vec::new(),
            iterations: 1000,
            jobs: std::thread::available_parallelism()
                .map(|count| count.get().saturating_sub(2).clamp(1, 16))
                .unwrap_or(1),
            seed: 1,
            top: 12,
            output: None,
            show_errors: false,
            continue_on_error: false,
        };
        let mut at = 0;
        while at < args.len() {
            match args[at].as_str() {
                "--iterations" => {
                    options.iterations = bounded(
                        value(args, at, "--iterations")?,
                        "--iterations",
                        MAX_ITERATIONS,
                    )?;
                    at += 2;
                }
                "--jobs" | "-j" => {
                    options.jobs = bounded(value(args, at, "--jobs")?, "--jobs", MAX_JOBS)?;
                    at += 2;
                }
                "--seed" => {
                    options.seed = value(args, at, "--seed")?
                        .parse()
                        .map_err(|_| "--seed must be an unsigned integer".to_string())?;
                    at += 2;
                }
                "--top" => {
                    options.top = bounded(value(args, at, "--top")?, "--top", MAX_TOP)?;
                    at += 2;
                }
                "--output" => {
                    options.output = Some(PathBuf::from(value(args, at, "--output")?));
                    at += 2;
                }
                "--show-errors" => {
                    options.show_errors = true;
                    at += 1;
                }
                "--continue-on-error" => {
                    options.continue_on_error = true;
                    at += 1;
                }
                "-h" | "--help" | "--self-test" => return Err(USAGE.to_string()),
                flag if flag.starts_with('-') => {
                    return Err(format!("unknown option {flag}\n{USAGE}"));
                }
                candidate => {
                    options.candidates.push(PathBuf::from(candidate));
                    at += 1;
                }
            }
        }
        if options.candidates.is_empty() {
            return Err(USAGE.to_string());
        }
        Ok(options)
    }
}

pub fn self_test() -> Result<(), String> {
    let args = [
        "x.c",
        "--iterations",
        "9",
        "-j",
        "2",
        "--seed",
        "7",
        "--top",
        "3",
    ]
    .into_iter()
    .map(str::to_string)
    .collect::<Vec<_>>();
    let parsed = Options::parse(&args)?;
    if parsed.iterations != 9 || parsed.jobs != 2 || parsed.seed != 7 || parsed.top != 3 {
        return Err("option parser lost a supplied value".into());
    }
    if Options::parse(&["x.c".into(), "--walk".into()]).is_ok()
        || Options::parse(&["x.c".into(), "--jobs".into(), "0".into()]).is_ok()
    {
        return Err("option parser accepted a removed mode or an invalid bound".into());
    }
    Ok(())
}
