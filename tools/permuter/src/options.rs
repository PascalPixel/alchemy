use std::path::PathBuf;

pub const MAX_ITERATIONS: usize = 100_000;
pub const MAX_JOBS: usize = 64;
pub const MAX_SOURCE_BYTES: usize = 8 * 1024 * 1024;

pub const USAGE: &str = "usage: permuter <candidate.c> [options]\n\
  Decodes allocator evidence and searches a finite catalog over ordinary C.\n\
\n\
  --iterations N       candidates to compile (default 1000; max 100000)\n\
  --jobs N, -j N       parallel compiler workers (default CPUs-2; max 64)\n\
  --seed N             deterministic starting order (default 1)\n\
  --output DIR         fresh run directory\n\
  --acceptance-test    perturb, decode, and recover exact owners";

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Options {
    pub candidate: PathBuf,
    pub iterations: usize,
    pub jobs: usize,
    pub seed: u64,
    pub output: Option<PathBuf>,
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
            candidate: PathBuf::new(),
            iterations: 1000,
            jobs: std::thread::available_parallelism()
                .map(|count| count.get().saturating_sub(2).clamp(1, 16))
                .unwrap_or(1),
            seed: 1,
            output: None,
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
                "--output" => {
                    options.output = Some(PathBuf::from(value(args, at, "--output")?));
                    at += 2;
                }
                "-h" | "--help" => return Err(USAGE.to_string()),
                flag if flag.starts_with('-') => {
                    return Err(format!("unknown option {flag}\n{USAGE}"));
                }
                candidate => {
                    if !options.candidate.as_os_str().is_empty() {
                        return Err("permuter accepts one candidate at a time".into());
                    }
                    options.candidate = PathBuf::from(candidate);
                    at += 1;
                }
            }
        }
        if options.candidate.as_os_str().is_empty() {
            return Err(USAGE.to_string());
        }
        Ok(options)
    }
}
