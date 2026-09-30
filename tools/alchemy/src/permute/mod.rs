//! `alchemy permute`: search equivalent spellings of one draft function
//! until its compiled code matches the function's extent in its listing.
//!
//! The listing only scores candidates, as `make compare` judges a build:
//! every candidate is ordinary C made by semantics-preserving rewrites of the
//! draft, compiled with the draft's routed compiler command. Nothing derived
//! from the listing is written into a candidate.

mod ast;
mod compile;
pub mod drafts;
mod effects;
mod lex;
mod mutate;
pub(crate) mod parse;
mod routine;
mod score;
mod types;

use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use ast::{Function, Stmt};
use compile::{assemble, Toolchain};
use mutate::{mutate, unnatural, Kind, Rng};
use parse::{locate, scan_unit, Unit};
use routine::{routine, Routine, Symbols};
use score::{score, Score};
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, AtomicU64, Ordering};
use std::sync::Mutex;
use std::time::{Duration, Instant};
use types::Env;

const USAGE: &str = "usage: alchemy permute DRAFT.c [options]\n\
Search semantics-preserving rewrites of one draft function until its code\n\
matches the function's extent in its listing, assembled as the build\n\
assembles listings. Candidates compile with the draft's routed compiler\n\
command (stock agscc, the build's flags); the listing only scores them.\n\
Scores follow decomp-permuter: per aligned instruction 1 for a stack offset,\n\
5 for registers only, 20 for another operand; 60 per reordered and 100 per\n\
inserted or deleted instruction; 0 only for identical code.\n\
\n\
  --function NAME   function to match (default: the draft's only definition)\n\
  --listing FILE    target listing (default: recon/GAME/raw/DRAFT-STEM.s)\n\
  --symbol NAME     the function's name in the listing (default: its C name)\n\
  --target ID       build target: routing, edition and build (default: tbs-en)\n\
  --route FILE      source whose compiler route applies (default: the draft)\n\
  --elf FILE        linked build naming symbol addresses (default: out/ID/ID.elf)\n\
  --focus REGEX     rewrite only statements whose own text matches (for a loop\n\
                    or an if, its header; not its body)\n\
  --seed N          job J searches with seed N+J (default: 1)\n\
  --jobs N          parallel searches (default: available cores)\n\
  --time SECONDS    stop after this many seconds (default: 60)\n\
  --iterations N    stop each job after N candidates; with it, results repeat\n\
  --write           write DRAFT-STEM.permute.c beside the draft, score in its header\n\
  --show N          differing instructions to print (default: 12)\n\
  --sample N        print N single rewrites of the draft as diffs and stop\n\
\n\
Rewrites: swap or regroup commutative operands; reorder independent\n\
statements and local declarations; introduce or remove a temporary, or\n\
share one between two statements; add or drop same-width integer casts;\n\
for, while and do-while loop forms; pointer arithmetic versus indexing;\n\
split or join compound assignments; move an assignment into or out of a\n\
condition or a call's arguments; the register keyword; invert an if/else;\n\
test a truth value or compare it with zero. A written candidate\n\
with constructs no programmer would write carries a FAKEMATCH tag.";

pub fn run(arguments: &[String]) -> Result<(), String> {
    if arguments
        .iter()
        .any(|argument| argument == "--help" || argument == "-h")
    {
        println!("{USAGE}");
        return Ok(());
    }
    let options = Options::parse(arguments)?;
    let scratch = tempfile::Builder::new()
        .prefix("alchemy-permute-")
        .tempdir()
        .map_err(|error| error.to_string())?;
    let problem = Problem::load(&options.config, scratch.path())?;
    if let Some(count) = options.sample {
        let mut rng = Rng::new(options.seed);
        for index in 0..count {
            let mut function = problem.function.clone();
            let mut env = problem.env(&function);
            let Some(kind) = mutate(&mut function, &mut env, &mut rng) else {
                break;
            };
            println!("sample {index}: {}", kind.name());
            print!(
                "{}",
                line_diff(&problem.function.print(), &function.print())
            );
        }
        return Ok(());
    }
    println!(
        "permute: {} in {} against {}",
        problem.name,
        options.config.draft.display(),
        problem
            .listing
            .strip_prefix(root())
            .unwrap_or(&problem.listing)
            .display()
    );
    let draft_score = problem.evaluate(&problem.draft, &scratch.path().join("setup"))?;
    println!("draft: score {}", draft_score.summary());
    let base = problem.baseline(&scratch.path().join("setup"))?;
    if base.score != draft_score {
        println!(
            "reprinted draft: score {} (the search starts here)",
            base.score.summary()
        );
    }
    let unresolved: Vec<&String> = problem.unresolved.iter().collect();
    if !unresolved.is_empty() {
        println!(
            "unresolved in the candidate: {} (the linked build defines no such symbol)",
            unresolved
                .iter()
                .map(|name| name.as_str())
                .collect::<Vec<_>>()
                .join(", ")
        );
    }
    let limits = Limits {
        seed: options.seed,
        jobs: options.jobs,
        deadline: Some(Instant::now() + options.time),
        iterations: options.iterations,
    };
    let started = Instant::now();
    let outcome = search(
        &problem,
        &base,
        &limits,
        scratch.path(),
        &|found: &Found| {
            eprintln!(
                "permute: job {} iteration {}: score {}",
                found.job,
                found.iteration,
                found.score.summary()
            );
        },
    );
    let best = &problem.minimized(&base, &outcome.best, &scratch.path().join("minimize"));
    println!(
        "best: score {}{}",
        best.score.summary(),
        if best.kinds.is_empty() {
            String::new()
        } else {
            format!(
                " after {} rewrites (job {}, iteration {}): {}",
                best.kinds.len(),
                best.job,
                best.iteration,
                rewrites(&best.kinds)
            )
        }
    );
    println!(
        "search: {} jobs from seed {}, {} candidates ({} scored below the draft, {} level with it; {} did not compile, {} repeated) in {:.1}s",
        options.jobs,
        options.seed,
        outcome.compiled,
        outcome.better,
        outcome.level,
        outcome.failed,
        outcome.repeated,
        started.elapsed().as_secs_f64()
    );
    for (class, expected, found) in best.score.lines.iter().take(options.show) {
        println!("  {class:<9} {expected:<32} | {found}");
    }
    if best.score.lines.len() > options.show {
        println!("  ... {} more", best.score.lines.len() - options.show);
    }
    if !best.kinds.is_empty() {
        print!(
            "{}",
            line_diff(&base.function.print(), &best.function.print())
        );
    }
    if options.write {
        let path = problem.written_path();
        let text = problem.written(best, &base);
        std::fs::write(&path, text).map_err(|error| format!("{}: {error}", path.display()))?;
        println!("wrote {}", path.display());
    }
    Ok(())
}

struct Options {
    config: Config,
    seed: u64,
    jobs: usize,
    time: Duration,
    iterations: Option<u64>,
    write: bool,
    show: usize,
    sample: Option<usize>,
}

impl Options {
    fn parse(arguments: &[String]) -> Result<Options, String> {
        let mut draft = None;
        let mut config = Config {
            draft: PathBuf::new(),
            function: None,
            listing: None,
            symbol: None,
            target: decomp_target(None)?,
            route: None,
            elf: None,
            focus: None,
        };
        let mut options = Options {
            config: config.clone(),
            seed: 1,
            jobs: std::thread::available_parallelism().map_or(4, |count| count.get()),
            time: Duration::from_secs(60),
            iterations: None,
            write: false,
            show: 12,
            sample: None,
        };
        let mut index = 0;
        while index < arguments.len() {
            let flag = arguments[index].as_str();
            let value = || {
                arguments
                    .get(index + 1)
                    .cloned()
                    .ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))
            };
            let number = |text: String| {
                text.parse::<u64>()
                    .map_err(|_| format!("{flag} needs a number, not {text}"))
            };
            match flag {
                "--function" => config.function = Some(value()?),
                "--listing" => config.listing = Some(PathBuf::from(value()?)),
                "--symbol" => config.symbol = Some(value()?),
                "--target" => config.target = decomp_target(Some(&value()?))?,
                "--route" => config.route = Some(value()?),
                "--elf" => config.elf = Some(PathBuf::from(value()?)),
                "--focus" => config.focus = Some(value()?),
                "--seed" => options.seed = number(value()?)?,
                "--jobs" => options.jobs = number(value()?)?.max(1) as usize,
                "--time" => {
                    let text = value()?;
                    let seconds: f64 = text
                        .parse()
                        .map_err(|_| format!("--time needs seconds, not {text}"))?;
                    options.time = Duration::from_secs_f64(seconds.max(0.0));
                }
                "--iterations" => options.iterations = Some(number(value()?)?),
                "--show" => options.show = number(value()?)? as usize,
                "--sample" => options.sample = Some(number(value()?)? as usize),
                "--write" => {
                    options.write = true;
                    index += 1;
                    continue;
                }
                other if !other.starts_with("--") && draft.is_none() => {
                    draft = Some(PathBuf::from(other));
                    index += 1;
                    continue;
                }
                other => return Err(format!("unknown argument {other}\n{USAGE}")),
            }
            index += 2;
        }
        config.draft = draft.ok_or_else(|| USAGE.to_string())?;
        options.config = config;
        Ok(options)
    }
}

#[derive(Clone)]
pub struct Config {
    pub draft: PathBuf,
    pub function: Option<String>,
    pub listing: Option<PathBuf>,
    pub symbol: Option<String>,
    pub target: DecompTarget,
    pub route: Option<String>,
    pub elf: Option<PathBuf>,
    pub focus: Option<String>,
}

/// Everything a search needs, read once.
pub struct Problem {
    pub name: String,
    draft_path: PathBuf,
    /// The draft's full text.
    pub draft: String,
    span: (usize, usize),
    function: Function,
    unit: Unit,
    toolchain: Toolchain,
    reference: Routine,
    symbols: Symbols,
    focus: Option<regex::Regex>,
    pub listing: PathBuf,
    /// Symbols the draft's own code references that the build does not define.
    pub unresolved: std::collections::BTreeSet<String>,
}

impl Problem {
    pub fn load(config: &Config, scratch: &Path) -> Result<Problem, String> {
        let draft = std::fs::read_to_string(&config.draft)
            .map_err(|error| format!("{}: {error}", config.draft.display()))?;
        let here = std::env::current_dir().map_err(|error| error.to_string())?;
        let absolute = here.join(&config.draft);
        let file_name = absolute
            .file_name()
            .and_then(|name| name.to_str())
            .ok_or("the draft needs a file name")?
            .to_string();
        let stem = absolute
            .file_stem()
            .and_then(|stem| stem.to_str())
            .unwrap_or("")
            .to_string();
        let toolchain = Toolchain {
            target: config.target,
            route: config
                .route
                .clone()
                .unwrap_or_else(|| absolute.to_string_lossy().into_owned()),
            file_name,
            include: absolute.parent().map(Path::to_path_buf).unwrap_or_default(),
        };
        let setup = scratch.join("setup");
        let preprocessed = toolchain.preprocess(&draft, &setup)?;
        let unit = scan_unit(&preprocessed)?;
        let located = locate(&draft, config.function.as_deref(), &unit.typedef_names())?;
        let name = located.function.name.clone();
        let listing = match &config.listing {
            Some(listing) => here.join(listing),
            None => ["s", "S"]
                .iter()
                .map(|extension| {
                    root()
                        .join(config.target.asm_dir)
                        .join(format!("{stem}.{extension}"))
                })
                .find(|path| path.is_file())
                .ok_or_else(|| {
                    format!(
                        "no listing {}/{stem}.s; name one with --listing",
                        config.target.asm_dir
                    )
                })?,
        };
        let build = root().join(config.target.output_dir);
        let elf = config.elf.clone().or_else(|| {
            let default = build.join(format!("{}.elf", config.target.id));
            default.is_file().then_some(default)
        });
        let symbols = match &elf {
            Some(elf) => Symbols::load(elf)?,
            None => {
                eprintln!(
                    "permute: no linked build; symbols compare by name (run make compare first)"
                );
                Symbols::default()
            }
        };
        let focus = match &config.focus {
            Some(pattern) => {
                Some(regex::Regex::new(pattern).map_err(|error| format!("--focus: {error}"))?)
            }
            None => None,
        };
        let symbol = config.symbol.clone().unwrap_or_else(|| name.clone());
        let linked = symbols.get(&symbol).map(|address| address & !1);
        let object = assemble(&listing, &setup, &build)?;
        let reference = routine(&object, &symbol, &symbols, linked)
            .map_err(|error| format!("{}: {error}", listing.display()))?;
        let mut problem = Problem {
            name,
            draft_path: absolute,
            draft,
            span: (located.start, located.end),
            function: located.function,
            unit,
            toolchain,
            reference,
            symbols,
            focus,
            listing,
            unresolved: Default::default(),
        };
        let object = problem.toolchain.compile(&problem.draft, &setup)?;
        problem.unresolved = routine(&object, &problem.name, &problem.symbols, None)?.unresolved;
        Ok(problem)
    }

    /// The draft with `function` in place of the original definition.
    pub fn source(&self, function: &Function) -> String {
        let mut text = String::with_capacity(self.draft.len() + 256);
        text.push_str(&self.draft[..self.span.0]);
        text.push_str(function.print().trim_end());
        text.push_str(&self.draft[self.span.1..]);
        text
    }

    /// The best candidate without the rewrites that only rode along: every
    /// changed region of its text that can go back to the draft's spelling
    /// without raising the score does, and the rest is parsed again.
    pub fn minimized(&self, base: &Found, best: &Found, directory: &Path) -> Found {
        if best.kinds.is_empty() {
            return best.clone();
        }
        let before = base.function.print();
        let after = best.function.print();
        let splice = |text: &str| {
            let mut source = String::with_capacity(self.draft.len() + 256);
            source.push_str(&self.draft[..self.span.0]);
            source.push_str(text.trim_end());
            source.push_str(&self.draft[self.span.1..]);
            source
        };
        let (text, kept, regions) = minimize(&before, &after, &mut |text: &str| {
            self.evaluate(&splice(text), directory)
                .ok()
                .map(|score| score.total)
        });
        if kept == regions {
            return best.clone();
        }
        let source = splice(&text);
        let Ok(located) = locate(&source, Some(&self.name), &self.unit.typedef_names()) else {
            return best.clone();
        };
        let Ok(score) = self.evaluate(&self.source(&located.function), directory) else {
            return best.clone();
        };
        if score.total > best.score.total {
            return best.clone();
        }
        println!("minimized: kept {kept} of {regions} changed regions");
        let env = self.env(&located.function);
        Found {
            unnatural: unnatural(&located.function, &env),
            function: located.function,
            score,
            kinds: best.kinds.clone(),
            job: best.job,
            iteration: best.iteration,
        }
    }

    pub fn evaluate(&self, text: &str, directory: &Path) -> Result<Score, String> {
        let object = self.toolchain.compile(text, directory)?;
        let candidate = routine(&object, &self.name, &self.symbols, None)?;
        Ok(score(&self.reference, &candidate))
    }

    fn env(&self, function: &Function) -> Env {
        let mut env = Env::new(&self.unit, function);
        env.focus = self.focus.clone();
        env
    }

    /// The reprinted, unmutated draft: where every search starts.
    pub fn baseline(&self, directory: &Path) -> Result<Found, String> {
        let score = self.evaluate(&self.source(&self.function), directory)?;
        let env = self.env(&self.function);
        Ok(Found {
            unnatural: unnatural(&self.function, &env),
            function: self.function.clone(),
            score,
            kinds: Vec::new(),
            job: 0,
            iteration: 0,
        })
    }

    fn written_path(&self) -> PathBuf {
        let stem = self
            .draft_path
            .file_stem()
            .and_then(|stem| stem.to_str())
            .unwrap_or("draft");
        self.draft_path.with_file_name(format!("{stem}.permute.c"))
    }

    /// The best candidate as a file: its score and search in a header, and a
    /// FAKEMATCH tag when it relies on constructs the draft did not.
    fn written(&self, best: &Found, base: &Found) -> String {
        let mut function = best.function.clone();
        if best.unnatural > base.unnatural {
            function.body.insert(
                0,
                Stmt::Comment(
                    "/* FAKEMATCH: permuter found a cast to the operand's own type, a cast of a cast or (*p).member */"
                        .into(),
                ),
            );
        }
        let rewrites = if best.kinds.is_empty() {
            "none".to_string()
        } else {
            rewrites(&best.kinds)
        };
        format!(
            "/* alchemy permute: {} against {}: score {}.\n   Job {}, iteration {}; rewrites: {}. */\n{}",
            self.name,
            self.listing
                .strip_prefix(root())
                .unwrap_or(&self.listing)
                .display(),
            best.score.summary(),
            best.job,
            best.iteration,
            rewrites,
            self.source(&function)
        )
    }
}

#[derive(Clone)]
pub struct Found {
    pub function: Function,
    pub score: Score,
    pub unnatural: usize,
    pub kinds: Vec<Kind>,
    pub job: usize,
    pub iteration: u64,
}

impl Found {
    /// Lower is better: the score, then constructs the draft did not need,
    /// then the number of rewrites.
    fn rank(&self, base_unnatural: usize) -> (u64, usize, usize) {
        (
            self.score.total,
            self.unnatural.saturating_sub(base_unnatural),
            self.kinds.len(),
        )
    }
}

pub struct Limits {
    pub seed: u64,
    pub jobs: usize,
    pub deadline: Option<Instant>,
    pub iterations: Option<u64>,
}

pub struct Outcome {
    pub best: Found,
    pub compiled: u64,
    pub failed: u64,
    pub repeated: u64,
    /// Compiled candidates that scored below, and level with, the draft.
    pub better: u64,
    pub level: u64,
}

/// Run `limits.jobs` independent random walks from `base`. Each job's walk
/// depends only on its seed, so a run bounded by iterations repeats exactly.
pub fn search(
    problem: &Problem,
    base: &Found,
    limits: &Limits,
    scratch: &Path,
    report: &(dyn Fn(&Found) + Sync),
) -> Outcome {
    let stop = AtomicBool::new(false);
    let reported = AtomicU64::new(base.score.total);
    let compiled = AtomicU64::new(0);
    let failed = AtomicU64::new(0);
    let repeated = AtomicU64::new(0);
    let better = AtomicU64::new(0);
    let level = AtomicU64::new(0);
    let results = Mutex::new(Vec::new());
    std::thread::scope(|scope| {
        for job in 0..limits.jobs {
            let (stop, reported, compiled, failed, repeated, results) =
                (&stop, &reported, &compiled, &failed, &repeated, &results);
            let (better, level) = (&better, &level);
            scope.spawn(move || {
                let directory = scratch.join(format!("job-{job:03}"));
                let mut rng = Rng::new(limits.seed.wrapping_add(job as u64));
                let mut env = problem.env(&base.function);
                let mut cache: HashMap<String, Option<Score>> = HashMap::new();
                let mut best = base.clone();
                best.job = job;
                let mut current = best.clone();
                let mut iteration = 0u64;
                loop {
                    if stop.load(Ordering::Relaxed)
                        || limits.iterations.is_some_and(|limit| iteration >= limit)
                        || limits
                            .deadline
                            .is_some_and(|deadline| Instant::now() >= deadline)
                    {
                        break;
                    }
                    iteration += 1;
                    let start = match rng.below(10) {
                        0 => base,
                        1 | 2 => &best,
                        _ => &current,
                    };
                    let mut function = start.function.clone();
                    let mut kinds = start.kinds.clone();
                    env.refresh(&function);
                    let steps = if rng.chance(1, 3) { 2 } else { 1 };
                    for _ in 0..steps {
                        if let Some(kind) = mutate(&mut function, &mut env, &mut rng) {
                            kinds.push(kind);
                        }
                    }
                    let text = problem.source(&function);
                    let result = match cache.get(&text) {
                        Some(result) => {
                            repeated.fetch_add(1, Ordering::Relaxed);
                            result.clone()
                        }
                        None => {
                            let result = problem.evaluate(&text, &directory).ok();
                            if let Some(score) = &result {
                                compiled.fetch_add(1, Ordering::Relaxed);
                                match score.total.cmp(&base.score.total) {
                                    std::cmp::Ordering::Less => {
                                        better.fetch_add(1, Ordering::Relaxed)
                                    }
                                    std::cmp::Ordering::Equal => {
                                        level.fetch_add(1, Ordering::Relaxed)
                                    }
                                    std::cmp::Ordering::Greater => 0,
                                };
                            } else {
                                failed.fetch_add(1, Ordering::Relaxed);
                            }
                            cache.insert(text, result.clone());
                            result
                        }
                    };
                    let Some(score) = result else {
                        continue;
                    };
                    let candidate = Found {
                        unnatural: unnatural(&function, &env),
                        function,
                        score,
                        kinds,
                        job,
                        iteration,
                    };
                    if candidate.rank(base.unnatural) < best.rank(base.unnatural) {
                        best = candidate.clone();
                        if reported.fetch_min(best.score.total, Ordering::Relaxed)
                            > best.score.total
                        {
                            report(&best);
                        }
                        if best.score.exact && best.unnatural <= base.unnatural {
                            // A bounded run lets every job finish, so its
                            // result never depends on which job was faster.
                            if limits.iterations.is_none() {
                                stop.store(true, Ordering::Relaxed);
                            }
                            break;
                        }
                    }
                    // Climb, with some freedom to cross worse ground.
                    if candidate.score.total <= current.score.total || rng.chance(1, 4) {
                        current = candidate;
                    }
                }
                results.lock().unwrap().push(best);
            });
        }
    });
    let mut all = results.into_inner().unwrap();
    all.sort_by_key(|found| (found.rank(base.unnatural), found.job, found.iteration));
    let best = all.into_iter().next().unwrap_or_else(|| base.clone());
    Outcome {
        best,
        compiled: compiled.into_inner(),
        failed: failed.into_inner(),
        repeated: repeated.into_inner(),
        better: better.into_inner(),
        level: level.into_inner(),
    }
}

/// The rewrites a walk applied, most frequent first, as `3x reorder ...`.
/// Later rewrites may undo earlier ones; the diff shows the net change.
fn rewrites(kinds: &[Kind]) -> String {
    let mut counts: Vec<(usize, Kind)> = Vec::new();
    for kind in kinds {
        match counts.iter_mut().find(|(_, seen)| seen == kind) {
            Some((count, _)) => *count += 1,
            None => counts.push((1, *kind)),
        }
    }
    counts.sort_by_key(|(count, kind)| (std::cmp::Reverse(*count), *kind));
    counts
        .iter()
        .map(|(count, kind)| format!("{count}x {}", kind.name()))
        .collect::<Vec<_>>()
        .join(", ")
}

/// The changed regions between two texts' lines: each replaces
/// `old[start..end]` with `new[from..to]`, as (start, end, from, to).
fn regions(old: &[&str], new: &[&str]) -> Vec<(usize, usize, usize, usize)> {
    let mut out = Vec::new();
    let (mut i, mut j) = (0, 0);
    for (a, b) in score::align(old, new)
        .into_iter()
        .chain(std::iter::once((old.len(), new.len())))
    {
        if a > i || b > j {
            out.push((i, a, j, b));
        }
        i = a + 1;
        j = b + 1;
    }
    out
}

/// Put each changed region of `after` back to its spelling in `before`
/// while `score` (lower is better; `None` when the text does not compile)
/// does not rise, over two passes. Returns the text, the regions kept and
/// the regions there were.
fn minimize(
    before: &str,
    after: &str,
    score: &mut dyn FnMut(&str) -> Option<u64>,
) -> (String, usize, usize) {
    let old: Vec<&str> = before.lines().collect();
    let new: Vec<&str> = after.lines().collect();
    let parts = regions(&old, &new);
    let build = |keep: &[bool]| {
        let mut lines: Vec<&str> = Vec::new();
        let mut at = 0;
        for (part, &(start, end, from, to)) in parts.iter().enumerate() {
            lines.extend_from_slice(&old[at..start]);
            if keep[part] {
                lines.extend_from_slice(&new[from..to]);
            } else {
                lines.extend_from_slice(&old[start..end]);
            }
            at = end;
        }
        lines.extend_from_slice(&old[at..]);
        let mut text = lines.join("\n");
        text.push('\n');
        text
    };
    let mut keep = vec![true; parts.len()];
    let Some(mut total) = score(&build(&keep)) else {
        return (after.to_string(), parts.len(), parts.len());
    };
    for _ in 0..2 {
        let mut dropped = false;
        for part in 0..parts.len() {
            if !keep[part] {
                continue;
            }
            keep[part] = false;
            match score(&build(&keep)) {
                Some(next) if next <= total => {
                    total = next;
                    dropped = true;
                }
                _ => keep[part] = true,
            }
        }
        if !dropped {
            break;
        }
    }
    let kept = keep.iter().filter(|keep| **keep).count();
    (build(&keep), kept, parts.len())
}

/// A line diff of two function texts, `-` for the draft and `+` for the
/// candidate, with two lines of context and `@@` between distant changes.
fn line_diff(before: &str, after: &str) -> String {
    let old: Vec<&str> = before.lines().collect();
    let new: Vec<&str> = after.lines().collect();
    let pairs = score::align(&old, &new);
    let mut script: Vec<(char, &str)> = Vec::new();
    let (mut i, mut j) = (0, 0);
    for (a, b) in pairs
        .into_iter()
        .chain(std::iter::once((old.len(), new.len())))
    {
        script.extend(old[i..a].iter().map(|line| ('-', *line)));
        script.extend(new[j..b].iter().map(|line| ('+', *line)));
        if a < old.len() {
            script.push((' ', old[a]));
        }
        i = a + 1;
        j = b + 1;
    }
    let changed: Vec<bool> = script.iter().map(|(tag, _)| *tag != ' ').collect();
    let mut out = String::from("--- draft\n+++ candidate\n");
    let mut last: Option<usize> = None;
    for (index, (tag, line)) in script.iter().enumerate() {
        let near = (index.saturating_sub(2)..(index + 3).min(script.len())).any(|at| changed[at]);
        if !near {
            continue;
        }
        if last.map_or(index > 0, |last| index > last + 1) {
            out.push_str("@@\n");
        }
        out.push(*tag);
        out.push_str(line);
        out.push('\n');
        last = Some(index);
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    const DRAFT: &str = "/* Two stores in the draft's order. */\nextern int gX;\nextern int gY;\n\nvoid Store_Pair(int a, int b)\n{\n    gX = a + 1;\n    gY = b;\n}\n";

    // The target: the same stores, gY first, as the compiler emits them.
    const LISTING: &str = "\t.syntax unified\n\t.thumb\n\t.global Store_Pair\n\t.thumb_func\nStore_Pair:\n\tldr\tr3, .Lgy\n\tstr\tr1, [r3]\n\tldr\tr3, .Lgx\n\tadds\tr0, #1\n\tstr\tr0, [r3]\n\tbx\tlr\n\t.align 2\n.Lgy:\n\t.4byte gY\n.Lgx:\n\t.4byte gX\n";

    #[test]
    fn permutes_a_synthetic_function_to_its_known_target() {
        let work = tempfile::tempdir().unwrap();
        let draft = work.path().join("STORE.c");
        let listing = work.path().join("STORE.s");
        std::fs::write(&draft, DRAFT).unwrap();
        std::fs::write(&listing, LISTING).unwrap();
        let config = Config {
            draft,
            function: None,
            listing: Some(listing),
            symbol: None,
            target: decomp_target(Some("tbs-en")).unwrap(),
            route: None,
            elf: None,
            focus: None,
        };
        let scratch = work.path().join("scratch");
        let problem = Problem::load(&config, &scratch).unwrap();
        let base = problem.baseline(&scratch.join("setup")).unwrap();
        assert!(
            !base.score.exact && base.score.total > 0,
            "{:?}",
            base.score
        );
        let limits = Limits {
            seed: 1,
            jobs: 2,
            deadline: None,
            iterations: Some(40),
        };
        let first = search(&problem, &base, &limits, &scratch, &|_| {});
        assert!(first.best.score.exact, "{:?}", first.best.score);
        let text = problem.source(&first.best.function);
        assert!(text.contains("    gY = b;\n    gX = a + 1;\n"), "{text}");
        assert!(text.starts_with("/* Two stores in the draft's order. */\n"));
        // The same seeds find the same candidate.
        let again = search(&problem, &base, &limits, &scratch, &|_| {});
        assert_eq!(again.best.function, first.best.function);
        assert_eq!(
            (again.best.job, again.best.iteration),
            (first.best.job, first.best.iteration)
        );
        // Minimizing keeps the match and the reordering it needs.
        let minimized = problem.minimized(&base, &first.best, &scratch.join("minimize"));
        assert!(minimized.score.exact, "{:?}", minimized.score);
        assert!(problem
            .source(&minimized.function)
            .contains("    gY = b;\n    gX = a + 1;\n"));
        let written = problem.written(&first.best, &base);
        assert!(written.starts_with("/* alchemy permute: Store_Pair against "));
        assert!(written.contains("score 0 (exact)"));
        assert!(!written.contains("FAKEMATCH"));
    }

    #[test]
    fn minimizing_reverts_regions_the_score_does_not_need() {
        // Only the B matters; D rode along, and reverting E breaks the build.
        let mut score = |text: &str| -> Option<u64> {
            if !text.contains("E") {
                return None;
            }
            Some(if text.contains("B") { 0 } else { 10 })
        };
        let (text, kept, regions) =
            minimize("a\nb\nc\nd\nx\ne\n", "a\nB\nc\nD\nx\nE\n", &mut score);
        assert_eq!(text, "a\nB\nc\nd\nx\nE\n");
        assert_eq!((kept, regions), (2, 3));
    }

    #[test]
    fn absolute_link_time_values_resolve_as_numbers() {
        // The text build defines message numbers as absolute symbols; a
        // candidate's pool word against one is the listing's plain number.
        let work = tempfile::tempdir().unwrap();
        let listing = work.path().join("VALUES.s");
        std::fs::write(
            &listing,
            "\t.global MsgExample\n\t.set MsgExample, 10\n\t.text\n\t.global Fn\nFn:\n\t.4byte 0\n",
        )
        .unwrap();
        let object = assemble(&listing, &work.path().join("out"), work.path()).unwrap();
        let path = work.path().join("VALUES.o");
        std::fs::write(&path, object).unwrap();
        let symbols = Symbols::load(&path).unwrap();
        assert_eq!(symbols.get("MsgExample"), Some(10));
        assert_eq!(symbols.get("Fn"), Some(0));
    }

    #[test]
    fn line_diff_marks_moved_lines() {
        assert_eq!(
            line_diff("a\nb\nc\n", "b\na\nc\n"),
            "--- draft\n+++ candidate\n-a\n b\n+a\n c\n"
        );
    }
}
