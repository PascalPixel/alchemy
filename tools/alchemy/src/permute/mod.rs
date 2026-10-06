//! `alchemy permute`: search equivalent spellings of one draft until its
//! code matches its listing. Every candidate compiles through `alchemy
//! diff`'s own steps and is compared with the same normalized rows, so a
//! score of 0 is exactly `alchemy diff` differ 0. The listing only scores
//! candidates; nothing derived from it is written into one, and the permuter
//! writes only under out/permute/.

mod ast;
mod effects;
mod lex;
mod mutate;
mod parse;
pub(crate) mod perm;
mod score;
mod types;

use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use ast::Function;
use mutate::{mutate, unnatural, Kind, Rng};
use parse::{locate, parse_body, scan_unit, Unit};
use perm::{Expanded, Node};
use score::Score;
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};
use std::time::{Duration, Instant};
use types::Env;

const USAGE: &str = r#"usage: alchemy permute DRAFT.c [ADDRESS[,ADDRESS...]] [--edition EDITION] [--minutes N] [--jobs J] [--seed S] [--function NAME] [--iterations N]
Searches semantics-preserving rewrites of DRAFT.c until its code matches the
listing alchemy diff compares it with (ADDRESS defaults to the file name;
build the edition first). Each candidate compiles with alchemy diff's steps
in its job's own directory under out/permute/STEM/, and scores two ways:
differ, alchemy diff's count, and a weighted score as pret's decomp-permuter
weighs rows: 10 for an instruction that differs only in registers or a
distance, 60 for one found elsewhere (70 with other registers), 100 for an
insertion or deletion. The search lowers the weighted score; 0 means differ 0.
PERM macros in the draft are expanded only here, as pret's are:
  PERM_GENERAL(a, b, ...) or PERM_ONEOF(a, b, ...)  one of the alternatives
  PERM_LINESWAP(lines)                               any order of the lines
  PERM_RANDOMIZE(code)       random rewrites only in these statements
  PERM_IGNORE(code)          passed through, never parsed or rewritten
Without PERM macros the whole function is rewritten at random; with them,
only PERM_RANDOMIZE regions are. Writes out/permute/STEM/best-N.c (best
first) and best-N.diff against the draft, and prints the score trajectory.
Defaults: --minutes 10, --jobs cores - 2, --seed 1. --function names the
definition to rewrite when the draft has several; --iterations bounds each
job instead of the clock, so a run repeats exactly."#;

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    target: DecompTarget,
    minutes: f64,
    jobs: usize,
    seed: u64,
    function: Option<String>,
    iterations: Option<u64>,
}

fn parse_options(arguments: &[String]) -> Result<Options, String> {
    let mut draft = None;
    let mut addresses = Vec::new();
    let mut target = None;
    let mut minutes = 10.0;
    let mut jobs = std::thread::available_parallelism().map_or(1, |n| n.get().saturating_sub(2).max(1));
    let mut seed = 1;
    let mut function = None;
    let mut iterations = None;
    let mut index = 0;
    let value = |index: usize| arguments.get(index).cloned().ok_or_else(|| USAGE.to_string());
    while index < arguments.len() {
        match arguments[index].as_str() {
            "-h" | "--help" => return Err(USAGE.into()),
            "--edition" => {
                index += 1;
                target = Some(decomp_target(Some(&value(index)?))?);
            }
            "--minutes" => {
                index += 1;
                minutes = value(index)?.parse().map_err(|_| format!("--minutes: a number\n{USAGE}"))?;
            }
            "--jobs" => {
                index += 1;
                jobs = value(index)?.parse::<usize>().map_err(|_| format!("--jobs: a count\n{USAGE}"))?.max(1);
            }
            "--seed" => {
                index += 1;
                seed = value(index)?.parse().map_err(|_| format!("--seed: a number\n{USAGE}"))?;
            }
            "--iterations" => {
                index += 1;
                iterations = Some(value(index)?.parse().map_err(|_| format!("--iterations: a count\n{USAGE}"))?);
            }
            "--function" => {
                index += 1;
                function = Some(value(index)?);
            }
            other if other.starts_with('-') => return Err(format!("unknown permute option: {other}\n{USAGE}")),
            other if draft.is_none() => draft = Some(PathBuf::from(other)),
            other if addresses.is_empty() => addresses = other.split(',').map(str::to_string).collect(),
            _ => return Err(USAGE.into()),
        }
        index += 1;
    }
    let draft = draft.ok_or_else(|| USAGE.to_string())?;
    let text = draft.to_string_lossy().into_owned();
    if text.contains("/overlays/") {
        return Err("permute compares main-image listings; overlay drafts are not supported".into());
    }
    if addresses.is_empty() {
        let stem = draft.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
        if stem.len() != 8 || !stem.chars().all(|c| c.is_ascii_hexdigit()) {
            return Err(format!("{}: name the listing address\n{USAGE}", draft.display()));
        }
        addresses.push(stem);
    }
    let game = match target {
        Some(target) if target.id.as_str().starts_with("tla-") => "tla",
        Some(_) => "tbs",
        None if text.contains("/tla/") || text.contains("THE LOST AGE") => "tla",
        None => "tbs",
    };
    let target = match target {
        Some(target) => target,
        None => decomp_target(Some(&format!("{game}-en")))?,
    };
    Ok(Options { draft, addresses, game, target, minutes, jobs, seed, function, iterations })
}

/// One job's scratch: its candidate file and alchemy diff's steps for it.
struct Job {
    candidate: PathBuf,
    object: PathBuf,
    steps: Vec<Vec<String>>,
}

impl Job {
    fn new(problem: &Problem, directory: &Path) -> Result<Job, String> {
        let object = directory.join("obj").join(format!("{}.o", problem.stem));
        std::fs::create_dir_all(object.parent().unwrap()).map_err(|e| format!("{}: {e}", directory.display()))?;
        let candidate = directory.join(problem.draft.file_name().unwrap_or_default());
        // The draft's own steps (its routing and options), aimed at the
        // job's copy and the edition's generated imports.
        let mut steps = crate::diff::draft_steps(problem.target, &problem.draft, &object, false)?;
        let draft = problem.draft.to_string_lossy().into_owned();
        let candidate_text = candidate.to_string_lossy().into_owned();
        let import_from = format!("-I{}", directory.display());
        let import_to = format!("-I{}", root().join(problem.target.output_dir).display());
        for step in &mut steps {
            for argument in step.iter_mut() {
                if *argument == import_from {
                    *argument = import_to.clone();
                } else if argument.contains(&draft) {
                    *argument = argument.replace(&draft, &candidate_text);
                }
            }
        }
        Ok(Job { candidate, object, steps })
    }

    /// The translation unit the compiler sees: the compile step with -E
    /// for -S (a library route already writes it before compiling).
    fn preprocess(&self) -> Result<String, String> {
        let preprocessed = self.object.with_extension("i");
        if !preprocessed.exists() {
            let assembly = self.object.with_extension("s").to_string_lossy().into_owned();
            let mut step = self.steps[0].clone();
            if !step.iter().any(|argument| argument == "-S") {
                return Err("no compile step to preprocess with".into());
            }
            for argument in step.iter_mut() {
                if argument == "-S" {
                    *argument = "-E".into();
                } else if *argument == assembly {
                    *argument = preprocessed.to_string_lossy().into_owned();
                }
            }
            crate::process::run(&step, root())?;
        }
        std::fs::read_to_string(&preprocessed).map_err(|e| format!("{}: {e}", preprocessed.display()))
    }

    fn compile(&self, text: &str) -> Result<(), String> {
        std::fs::write(&self.candidate, text).map_err(|e| format!("{}: {e}", self.candidate.display()))?;
        let _ = std::fs::remove_file(&self.object);
        for step in &self.steps {
            crate::process::run(step, root())?;
        }
        Ok(())
    }
}

/// A candidate: one expansion of the PERM macros and, when its function
/// parses, the rewritten function.
#[derive(Clone)]
struct State {
    expanded: Arc<Expanded>,
    parsed: Option<(usize, usize, Function)>,
    kinds: Vec<Kind>,
}

struct Problem {
    draft: PathBuf,
    text: String,
    stem: String,
    target: DecompTarget,
    nodes: Vec<Node>,
    choices: bool,
    listing: Vec<String>,
    symbol: Option<String>,
    unit: Unit,
    function: Option<String>,
}

impl Problem {
    fn state(&self, expanded: Expanded) -> State {
        let parsed = locate(&expanded.text, self.function.as_deref(), &self.unit.typedef_names())
            .ok()
            .map(|located| (located.start, located.end, located.function));
        State { expanded: Arc::new(expanded), parsed, kinds: Vec::new() }
    }

    fn source(&self, state: &State) -> String {
        let text = &state.expanded.text;
        let spelled = match &state.parsed {
            Some((start, end, function)) => {
                format!("{}{}{}", &text[..*start], function.print().trim_end(), &text[*end..])
            }
            None => text.clone(),
        };
        state.expanded.restore(&spelled)
    }

    fn evaluate(&self, job: &Job, text: &str) -> Result<Score, String> {
        job.compile(text)?;
        let compiled = crate::diff::disassemble(&job.object, self.symbol.as_deref())?;
        Ok(score::score(&self.listing, &compiled))
    }

    /// Random rewrites stay inside PERM_RANDOMIZE's statements, as
    /// `--focus` once limited them: a statement is admitted by its text.
    fn focus(&self, expanded: &Expanded) -> Option<regex::Regex> {
        if expanded.randomize.is_empty() {
            return None;
        }
        let names = self.unit.typedef_names();
        let typedefs: Vec<&str> = names.iter().map(String::as_str).collect();
        let mut texts = Vec::new();
        for region in &expanded.randomize {
            for stmt in parse_body(&expanded.restore(region), &typedefs).unwrap_or_default() {
                texts.push(match &stmt {
                    ast::Stmt::Decl(decl) => ast::print_decl(decl),
                    other => other.own_exprs().iter().map(|expr| ast::print_expr(expr)).collect::<Vec<_>>().join("; "),
                });
            }
        }
        let alternatives: Vec<String> = texts.iter().map(|text| regex::escape(text)).collect();
        regex::Regex::new(&format!("^(?:{})$", if alternatives.is_empty() { "$^".into() } else { alternatives.join("|") })).ok()
    }
}

#[derive(Clone)]
struct Found {
    score: Score,
    unnatural: usize,
    text: String,
    kinds: Vec<Kind>,
    job: usize,
    iteration: u64,
}

impl Found {
    fn rank(&self, base_unnatural: usize) -> (u64, usize, usize, usize) {
        (self.score.weighted, self.score.differ, self.unnatural.saturating_sub(base_unnatural), self.kinds.len())
    }
}

fn reference(options: &Options) -> Result<(Vec<String>, Option<String>), String> {
    let mut listing = Vec::new();
    let mut scaffold = None;
    for address in &options.addresses {
        let object = root()
            .join(options.target.output_dir)
            .join(format!("obj/recon/{}/raw/{address}.o", options.game));
        if object.exists() {
            listing.extend(crate::diff::disassemble(&object, None)?);
        } else {
            let (tokens, name) = crate::diff::scaffold_reference(options.target, address)?;
            listing.extend(tokens);
            if options.addresses.len() == 1 {
                scaffold = Some(name);
            }
        }
    }
    Ok((listing, scaffold))
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse_options(arguments)?;
    let draft = std::fs::canonicalize(&options.draft).map_err(|e| format!("{}: {e}", options.draft.display()))?;
    let text = std::fs::read_to_string(&draft).map_err(|e| format!("{}: {e}", draft.display()))?;
    let stem = draft.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
    let output = root().join("out/permute").join(&stem);
    let nodes = perm::parse(&text)?;
    let (listing, scaffold) = reference(&options)?;
    let mut problem = Problem {
        draft,
        text,
        stem,
        target: options.target,
        choices: perm::has_choices(&nodes),
        nodes,
        listing,
        symbol: None,
        unit: Unit::default(),
        function: options.function.clone(),
    };
    // The written spelling first: its preprocessed unit gives the parser
    // its declarations, and its score is where the search starts.
    let setup = Job::new(&problem, &output.join("setup"))?;
    let original = perm::expand(&problem.nodes, None);
    setup.compile(&original.restore(&original.text))?;
    if let Some(name) = scaffold {
        if crate::diff::text_symbols(&setup.object)?.contains(&name) {
            problem.symbol = Some(name);
        }
    }
    problem.unit = scan_unit(&setup.preprocess()?)?;
    let base_state = problem.state(original);
    let random = base_state.parsed.is_some() && (!perm::has_macros(&problem.text) || perm::randomizes(&problem.nodes));
    if !random && !problem.choices {
        return Err(match problem.function.as_deref() {
            Some(name) => format!("{name}: the function does not parse for rewriting and the draft has no PERM choices"),
            None => format!(
                "{}: no function to rewrite ({}) and no PERM choices",
                options.draft.display(),
                locate(&base_state.expanded.text, None, &problem.unit.typedef_names()).err().unwrap_or_else(|| "only PERM_IGNORE regions".into())
            ),
        });
    }
    let base_text = problem.source(&base_state);
    let base_score = problem.evaluate(&setup, &base_text)?;
    let base_unnatural = match &base_state.parsed {
        Some((_, _, function)) => unnatural(function, &Env::new(&problem.unit, function)),
        None => 0,
    };
    let base = Found { score: base_score, unnatural: base_unnatural, text: base_text, kinds: Vec::new(), job: 0, iteration: 0 };
    println!(
        "{}: listing {} instructions; draft {}; {} job(s), {}, seed {}; rewrites: {}",
        problem.stem,
        problem.listing.len(),
        base.score.summary(),
        options.jobs,
        match options.iterations {
            Some(count) => format!("{count} iterations each"),
            None => format!("{} minutes", options.minutes),
        },
        options.seed,
        match (random, problem.choices) {
            (true, true) => "PERM choices and random",
            (true, false) => "random",
            _ => "PERM choices",
        }
    );
    let started = Instant::now();
    let deadline = options.iterations.is_none().then(|| started + Duration::from_secs_f64(options.minutes * 60.0));
    let found = search(&problem, &base_state, &base, random, &options, deadline, &output, started)?;
    write_best(&problem, &base, &found, &output)
}

#[allow(clippy::too_many_arguments)]
fn search(
    problem: &Problem,
    base_state: &State,
    base: &Found,
    random: bool,
    options: &Options,
    deadline: Option<Instant>,
    output: &Path,
    started: Instant,
) -> Result<Vec<Found>, String> {
    let stop = AtomicBool::new(false);
    let global = Mutex::new((base.score.weighted, base.score.differ));
    let found = Mutex::new(Vec::new());
    let failures = Mutex::new(Vec::new());
    std::thread::scope(|scope| {
        for job_index in 0..options.jobs {
            let (stop, global, found, failures) = (&stop, &global, &found, &failures);
            scope.spawn(move || {
                let job = match Job::new(problem, &output.join(format!("job-{job_index:02}"))) {
                    Ok(job) => job,
                    Err(error) => {
                        failures.lock().unwrap().push(error);
                        return;
                    }
                };
                let mut rng = Rng::new(options.seed.wrapping_mul(1_000_003).wrapping_add(job_index as u64));
                let mut env = base_state.parsed.as_ref().map(|(_, _, function)| Env::new(&problem.unit, function));
                // Keyed by a hash of the text: a long run sees many thousands.
                let mut cache: HashMap<u64, Option<Score>> = HashMap::new();
                let (mut best, mut current) = ((base_state.clone(), base.clone()), (base_state.clone(), base.clone()));
                let mut iteration = 0u64;
                while !stop.load(Ordering::Relaxed)
                    && options.iterations.is_none_or(|limit| iteration < limit)
                    && deadline.is_none_or(|deadline| Instant::now() < deadline)
                {
                    iteration += 1;
                    let start = match rng.below(10) {
                        0 => (base_state, base),
                        1 | 2 => (&best.0, &best.1),
                        _ => (&current.0, &current.1),
                    };
                    let mut state = if problem.choices && (!random || rng.chance(1, 3)) {
                        problem.state(perm::expand(&problem.nodes, Some(&mut rng)))
                    } else {
                        start.0.clone()
                    };
                    if random {
                        if let (Some((_, _, function)), Some(env)) = (state.parsed.as_mut(), env.as_mut()) {
                            env.refresh(function);
                            env.focus = problem.focus(&state.expanded);
                            for _ in 0..if rng.chance(1, 3) { 2 } else { 1 } {
                                if let Some(kind) = mutate(function, env, &mut rng) {
                                    state.kinds.push(kind);
                                }
                            }
                        }
                    }
                    let text = problem.source(&state);
                    let key = {
                        let mut hasher = std::collections::hash_map::DefaultHasher::new();
                        std::hash::Hash::hash(&text, &mut hasher);
                        std::hash::Hasher::finish(&hasher)
                        };
                    let result = match cache.get(&key) {
                        Some(result) => result.clone(),
                        None => {
                            let result = problem.evaluate(&job, &text).ok();
                            cache.insert(key, result.clone());
                            result
                        }
                    };
                    let Some(score) = result else { continue };
                    let candidate = Found {
                        unnatural: match (&state.parsed, env.as_mut()) {
                            (Some((_, _, function)), Some(env)) => {
                                env.refresh(function);
                                unnatural(function, env)
                            }
                            _ => 0,
                        },
                        score,
                        text,
                        kinds: state.kinds.clone(),
                        job: job_index,
                        iteration,
                    };
                    if candidate.rank(base.unnatural) < best.1.rank(base.unnatural) {
                        best = (state.clone(), candidate.clone());
                        found.lock().unwrap().push(candidate.clone());
                        let mut global = global.lock().unwrap();
                        if (candidate.score.weighted, candidate.score.differ) < *global {
                            *global = (candidate.score.weighted, candidate.score.differ);
                            println!(
                                "{:>8.1}s  job {job_index:>2}  iteration {iteration:>6}  {}",
                                started.elapsed().as_secs_f64(),
                                candidate.score.summary()
                            );
                        }
                        if candidate.score.weighted == 0 && candidate.unnatural <= base.unnatural && options.iterations.is_none() {
                            stop.store(true, Ordering::Relaxed);
                        }
                    }
                    // Climb, with some freedom to cross worse ground.
                    if candidate.score.weighted <= current.1.score.weighted || rng.chance(1, 4) {
                        current = (state, candidate);
                    }
                }
                let failed = cache.values().filter(|result| result.is_none()).count();
                println!("job {job_index:>2}: {iteration} iterations, {} distinct candidates, {failed} did not compile", cache.len());
            });
        }
    });
    let failures = failures.into_inner().unwrap();
    if let Some(error) = failures.first() {
        return Err(error.clone());
    }
    let mut all = found.into_inner().unwrap();
    all.sort_by(|a, b| {
        a.rank(base.unnatural).cmp(&b.rank(base.unnatural)).then(a.job.cmp(&b.job)).then(a.iteration.cmp(&b.iteration))
    });
    let mut distinct: Vec<Found> = Vec::new();
    for candidate in all {
        if !distinct.iter().any(|seen| seen.text == candidate.text) {
            distinct.push(candidate);
        }
    }
    Ok(distinct)
}

/// The rewrites a walk applied, most frequent first, as `3x reorder ...`.
fn rewrites(kinds: &[Kind]) -> String {
    let mut counts: Vec<(usize, Kind)> = Vec::new();
    for kind in kinds {
        match counts.iter_mut().find(|(_, seen)| seen == kind) {
            Some((count, _)) => *count += 1,
            None => counts.push((1, *kind)),
        }
    }
    counts.sort_by_key(|(count, kind)| (std::cmp::Reverse(*count), *kind));
    let text: Vec<String> = counts.iter().map(|(count, kind)| format!("{count}x {}", kind.name())).collect();
    if text.is_empty() { "PERM choices only".into() } else { text.join(", ") }
}

/// A line diff, `-` for the draft and `+` for the candidate, with two lines
/// of context and `@@` between distant changes.
fn line_diff(before: &str, after: &str) -> String {
    let old: Vec<String> = before.lines().map(str::to_string).collect();
    let new: Vec<String> = after.lines().map(str::to_string).collect();
    let (_, script) = crate::diff::compare(&old, &new);
    let changed: Vec<bool> = script.iter().map(|line| !line.starts_with("  ")).collect();
    let mut out = String::from("--- draft\n+++ candidate\n");
    let mut last: Option<usize> = None;
    for (index, line) in script.iter().enumerate() {
        if !(index.saturating_sub(2)..(index + 3).min(script.len())).any(|at| changed[at]) {
            continue;
        }
        if last.map_or(index > 0, |last| index > last + 1) {
            out.push_str("@@\n");
        }
        let (tag, text) = line.split_at(2);
        out.push(tag.chars().next().unwrap_or(' '));
        out.push_str(text);
        out.push('\n');
        last = Some(index);
    }
    out
}

fn write_best(problem: &Problem, base: &Found, found: &[Found], output: &Path) -> Result<(), String> {
    for entry in std::fs::read_dir(output).map_err(|e| format!("{}: {e}", output.display()))?.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with("best-") {
            let _ = std::fs::remove_file(entry.path());
        }
    }
    println!("draft:  {}", base.score.summary());
    if found.is_empty() {
        println!("no candidate beat the draft");
        return Ok(());
    }
    for (index, best) in found.iter().take(3).enumerate() {
        let path = output.join(format!("best-{}.c", index + 1));
        let fakematch = if best.unnatural > base.unnatural {
            "\n   It relies on a cast to an operand's own type, a cast of a cast or (*p).member: tag it FAKEMATCH (S2) or reshape it."
        } else {
            ""
        };
        let header = format!(
            "/* alchemy permute {}: {}; draft {}.\n   Job {}, iteration {}; rewrites: {}.{fakematch} */\n",
            problem.stem,
            best.score.summary(),
            base.score.summary(),
            best.job,
            best.iteration,
            rewrites(&best.kinds)
        );
        std::fs::write(&path, format!("{header}{}", best.text)).map_err(|e| format!("{}: {e}", path.display()))?;
        let diff = line_diff(&problem.text, &best.text);
        std::fs::write(path.with_extension("diff"), &diff).map_err(|e| format!("{}: {e}", path.display()))?;
        println!("best-{}: {}  {}", index + 1, best.score.summary(), path.strip_prefix(root()).unwrap_or(&path).display());
        if index == 0 {
            print!("{diff}");
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn options_default_and_parse() {
        let args = |items: &[&str]| items.iter().map(|item| item.to_string()).collect::<Vec<_>>();
        let parsed = parse_options(&args(&["recon/tbs/en/main/080cbc0c.c"])).unwrap();
        assert_eq!((parsed.addresses.as_slice(), parsed.minutes, parsed.seed), (["080cbc0c".to_string()].as_slice(), 10.0, 1));
        assert!(parsed.jobs >= 1);
        assert_eq!(parsed.target.id.as_str(), "tbs-en");
        let parsed = parse_options(&args(&["x.c", "08000000,08000010", "--edition", "tla-de", "--minutes", "0.5", "--jobs", "3", "--seed", "9"])).unwrap();
        assert_eq!((parsed.addresses.len(), parsed.game, parsed.jobs, parsed.seed, parsed.minutes), (2, "tla", 3, 9, 0.5));
        assert!(parse_options(&args(&["x.c"])).is_err());
        assert!(parse_options(&args(&["recon/tbs/en/overlays/resource_383_c_020048c8.c"])).is_err());
        assert!(parse_options(&args(&["08000000.c", "--bogus"])).is_err());
    }

    #[test]
    fn line_diff_marks_changed_lines_with_context() {
        let diff = line_diff("a\nb\nc\nd\ne\nf\ng\n", "a\nb\nc\nD\ne\nf\ng\n");
        assert_eq!(diff, "--- draft\n+++ candidate\n@@\n b\n c\n+D\n-d\n e\n f\n");
    }

    #[test]
    fn parsed_function_round_trips_through_the_printer() {
        let source = "extern int gX;\n\nint Sum(int *p, int n)\n{\n    int i;\n    int total = 0;\n    for (i = 0; i < n; i++) {\n        total += p[i] << 2;\n    }\n    if (total > 3 && n != 0)\n        gX = total;\n    return total;\n}\n";
        let typedefs = Default::default();
        let first = locate(source, None, &typedefs).unwrap();
        let printed = format!("{}{}{}", &source[..first.start], first.function.print().trim_end(), &source[first.end..]);
        let second = locate(&printed, None, &typedefs).unwrap();
        assert_eq!(first.function.print(), second.function.print());
        assert_eq!(second.function.print(), first.function.print());
    }

    #[test]
    fn mutations_repeat_exactly_under_a_seed() {
        let source = "extern int gX;\nextern int gY;\n\nvoid Store(int a, int b, int *p)\n{\n    int t = a + 1;\n    gX = t;\n    gY = b * 2;\n    if (a > b)\n        p[0] = a - b;\n    p[1] = a & 3;\n}\n";
        let unit = scan_unit(source).unwrap();
        let run = |seed: u64| {
            let mut function = locate(source, None, &unit.typedef_names()).unwrap().function;
            let mut env = Env::new(&unit, &function);
            let mut rng = Rng::new(seed);
            let mut trail = Vec::new();
            for _ in 0..40 {
                env.refresh(&function);
                let kind = mutate(&mut function, &mut env, &mut rng);
                trail.push((kind, function.print()));
            }
            trail
        };
        let first = run(5);
        assert_eq!(first, run(5));
        assert!(first.iter().any(|(kind, _)| kind.is_some()), "some rewrite applies");
        assert_ne!(first, run(6));
    }

    #[test]
    fn each_mutation_kind_repeats_exactly_under_a_seed() {
        let source = "extern int gX;\nextern int gY;\n\nint Walk(int a, int b, int *p, unsigned char *q)\n{\n    int i;\n    int t = a + 1;\n    int u;\n    gX = t;\n    gY = b * 2;\n    for (i = 0; i < a; i++) {\n        p[i] = p[i] + (int)q[i];\n    }\n    u = a & 3;\n    if (u != 0)\n        p[1] = u;\n    else\n        p[2] = b;\n    gX += b;\n    return t;\n}\n";
        let unit = scan_unit(source).unwrap();
        let base = locate(source, None, &unit.typedef_names()).unwrap().function;
        let mut applied = 0;
        for (kind, _) in Kind::ALL {
            let once = |seed: u64| {
                let mut function = base.clone();
                let env = Env::new(&unit, &function);
                let mut rng = Rng::new(seed);
                let changed = mutate::apply(kind, &mut function, &env, &mut rng);
                (changed, function.print())
            };
            for seed in [1, 2, 3] {
                assert_eq!(once(seed), once(seed), "{}", kind.name());
            }
            applied += usize::from(once(1).0);
        }
        assert!(applied >= 8, "{applied} kinds found a site");
    }

    #[test]
    fn perm_expansions_parse_and_rewrite_only_randomized_statements() {
        let draft = "extern int gX;\nextern int gY;\n\nvoid Pair(int a, int b)\n{\n    PERM_LINESWAP(\n    gX = a;\n    gY = b;\n    )\n    PERM_RANDOMIZE(gX = a + b * 2;)\n    gY = a - b;\n}\n";
        let nodes = perm::parse(draft).unwrap();
        let problem = Problem {
            draft: PathBuf::from("08000000.c"),
            text: draft.into(),
            stem: "08000000".into(),
            target: decomp_target(Some("tbs-en")).unwrap(),
            choices: perm::has_choices(&nodes),
            nodes,
            listing: Vec::new(),
            symbol: None,
            unit: scan_unit(draft).unwrap_or_default(),
            function: None,
        };
        let state = problem.state(perm::expand(&problem.nodes, None));
        assert!(state.parsed.is_some());
        let focus = problem.focus(&state.expanded).unwrap();
        assert!(focus.is_match("gX = a + b * 2") && !focus.is_match("gY = a - b"));
        let mut rng = Rng::new(4);
        let mut texts = std::collections::BTreeSet::new();
        let (_, _, base) = state.parsed.clone().unwrap();
        let mut env = Env::new(&problem.unit, &base);
        for _ in 0..60 {
            let mut state = problem.state(perm::expand(&problem.nodes, Some(&mut rng)));
            let (_, _, function) = state.parsed.as_mut().unwrap();
            env.refresh(function);
            env.focus = problem.focus(&state.expanded);
            mutate(function, &mut env, &mut rng);
            let text = problem.source(&state);
            assert!(!perm::has_macros(&text));
            assert!(text.contains("gY = a - b;"), "{text}");
            texts.insert(text);
        }
        assert!(texts.iter().any(|text| text.find("gY = b;") < text.find("gX = a;")));
        assert!(texts.iter().any(|text| !text.contains("gX = a + b * 2;")), "the randomized statement is rewritten");
    }

    #[test]
    fn rewrites_count_kinds_and_name_perm_only_runs() {
        assert_eq!(rewrites(&[]), "PERM choices only");
    }
}
