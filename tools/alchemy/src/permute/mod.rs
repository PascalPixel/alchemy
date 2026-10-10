//! Source-hypothesis search for a single C function.
//! Candidates compile through Alchemy's approved game routes. Weighted scoring
//! and instruction differences are separate measurements; listings judge
//! candidates and never supply their source. The default engine has 37 mutation
//! families, manual choices and replay. The optional heat-directed engine keeps
//! its own source-line mapping. Output lives under out/permute.

mod aim;
mod c;
pub(crate) mod ast;
mod effects;
pub(crate) mod lex;
mod mutate;
pub(crate) mod parse;
pub(crate) mod perm;
mod score;
mod rng;
mod randomize;
pub(crate) mod types;
#[cfg(test)]
mod compatibility_tests;

use crate::compiler::routing::root;
use crate::targets::{decomp_target, DecompTarget};
use ast::Function;
#[cfg(test)]
use mutate::mutate;
use mutate::{mutate_with, unnatural, Kind, Rng, Step};
use parse::{locate, scan_unit, Unit};
use perm::{Expanded, Node};
use score::Score;
use std::collections::HashMap;
use std::path::{Path, PathBuf};
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};
use std::time::{Duration, Instant};
use types::Env;

const USAGE: &str = r#"usage: alchemy permute DRAFT.c [ADDRESS[,ADDRESS...]] [--edition EDITION] [--minutes N] [--jobs J] [--seed S] [--function NAME] [--iterations N] [--heat] [--prefix] [--arm] [--stack-diffs] [--ignore-branch-targets] [--keep-prob P] [--stop-on-zero]
Searches source variations of DRAFT.c until its code matches the
listing alchemy diff compares it with (ADDRESS defaults to the file name;
build the edition first). Each candidate compiles with alchemy diff's steps
in its job's own directory under out/permute/STEM/, and scores two ways:
differ, alchemy diff's count, and a weighted score. The weighted score aligns
mnemonics with difflib, then charges 5 per differing
operand, 1 per differing branch target, 60 per exactly reordered row and
100 per insertion or deletion. Stack offsets, branch targets and trailing nops are ignored
by default: weighted 0 alone does not establish a match. --stack-diffs counts
stack distance; --no-ignore-branch-targets counts branch destinations.
The default search uses the whole C tree: temporary reuse, type and signature
changes, inline helpers, assignments, expressions and statement placement.
These are matching hypotheses, not a proof of equivalent behavior. Review
results before adoption and tag compiler-steering devices under S2.
PERM macros in the draft are expanded only here:
  PERM_GENERAL(a, b, ...) or PERM_ONEOF(a, b, ...)  one of the alternatives
  PERM_LINESWAP(lines)       reorder parsed statements
  PERM_LINESWAP_TEXT(lines)  reorder text lines
  PERM_INT(low, high)        inclusive integer range
  PERM_ONCE(key, code)       emit one occurrence per key
  PERM_VAR(key, code)        define a shared expansion; PERM_VAR(key) reads it
  PERM_RANDOMIZE(code)       random rewrites only in these statements
  PERM_IGNORE(code)          passed through, never parsed or rewritten
  PERM_PRETEND(code)         parser-only declarations
  PERM_FORCE_SAMELINE(code)  print this region on one line
Manual choices are enumerated without repeating seeds, subject to the time
or iteration limit. With a single base
source randomization is automatic; otherwise PERM_RANDOMIZE enables it.
Random runs keep the preceding candidate with --keep-prob (default 0.6),
regardless of improvement, restarting after failure or zero. --stop-on-zero
ends on weighted zero. After the search, each best candidate is
minimized: its rewrites are undone one at a time, last first, and each
removal that leaves the weighted score no higher is kept. Writes
out/permute/STEM/best-N.c (best first), its remaining rewrites in its
header, and best-N.diff, a unified diff against the draft file that
`patch -p0` applies from the checkout: PERM macros become their chosen
alternatives as the draft writes them, and the rewrites replace only the
draft lines they change (or the whole function as printed, when they do not
carry over with the same score). The run's scratch is removed however it
ends. Defaults: --minutes 10, --jobs cores - 2, --search-seed 1.
--function names the definition to rewrite when the draft has several;
--iterations bounds each job instead of the clock; --jobs 1 makes seeded
runs deterministic.
--seed RNG or --seed PERM,RNG forces a randomizer seed. RNG is a
signed decimal integer of any size; PERM defaults to zero. A zero RNG uses
fresh randomizer seeds on restarts.
--search-seed controls Alchemy's scheduler and manual-choice enumeration.
--rng-seed INTEGER replays a randomizer stream, selects manual choice
zero, and keeps mutating the candidate even after a failed compile or match.
Use --jobs 1 to reproduce one walk. This explicit option also forces seed zero.
--heat aims the rewrites as alchemy
diff --heat shows: the draft's printing is compiled once with -g, and each
rewrite's site is drawn in proportion to 1 plus the heat of the lines it
changes (4 per differing instruction, 24 at the first divergence, 8 on each
of the 3 lines before it). It uses the older conservative function-body
engine; the default whole-tree engine explores a broader search space.
--prefix ranks candidates by how many
instructions match from the start first, then by weighted score.
--algorithm selects difflib (default) or Levenshtein alignment.
Qualifying candidates are saved immediately in output-SCORE-N directories
with source.c, score.txt and diff.txt, in addition to minimized best-N files.
--better-only excludes ties with the draft; --best-only excludes scores above
the best already saved. --only-if-below N requires a score strictly below N.
--no-context-output trims source.c before the first target function occurrence.
--arm compiles each candidate as alchemy diff --arm does:
through the game ARM route (GAME_ARM_SOURCES, AGENTS.md K1): stock agbcc_arm."#;

struct Options {
    draft: PathBuf,
    addresses: Vec<String>,
    game: &'static str,
    target: DecompTarget,
    minutes: f64,
    jobs: usize,
    seed: u64,
    rng_seed: Option<String>,
    perm_seed: Option<u64>,
    function: Option<String>,
    iterations: Option<u64>,
    arm: bool,
    heat: bool,
    prefix: bool,
    score_options: score::Options,
    keep_prob: f64,
    stop_on_zero: bool,
}

fn parse_options(arguments: &[String]) -> Result<Options, String> {
    let mut draft = None;
    let mut addresses = Vec::new();
    let mut target = None;
    let mut minutes = 10.0_f64;
    let mut jobs = std::thread::available_parallelism().map_or(1, |n| n.get().saturating_sub(2).max(1));
    let mut seed = 1;
    let mut rng_seed = None;
    let mut perm_seed = None;
    let mut function = None;
    let mut iterations = None;
    let mut arm = false;
    let (mut heat, mut prefix) = (false, false);
    let mut score_options = score::Options::default();
    let mut keep_prob = 0.6;
    let mut stop_on_zero = false;
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
                let text = value(index)?;
                let parts: Vec<_> = text.split(',').collect();
                let (manual, random) = match parts.as_slice() {
                    [random] => (0, *random),
                    [manual, random] => (manual.trim().parse::<u64>().map_err(|_| "--seed: PERM must be a nonnegative integer")?, *random),
                    _ => return Err("--seed: expected RNG or PERM,RNG".into()),
                };
                Rng::from_decimal(random).map_err(|e| format!("--seed: {e}"))?;
                perm_seed = Some(manual);
                rng_seed = (!random.trim().trim_start_matches(['-', '+']).bytes().all(|b| b == b'0')).then(|| random.to_string());
            }
            "--search-seed" => {
                index += 1;
                seed = value(index)?.parse().map_err(|_| format!("--search-seed: a number\n{USAGE}"))?;
            }
            "--rng-seed" => {
                index += 1;
                let text = value(index)?;
                Rng::from_decimal(&text).map_err(|e| format!("--rng-seed: {e}\n{USAGE}"))?;
                rng_seed = Some(text);
                perm_seed = Some(0);
            }
            "--iterations" => {
                index += 1;
                iterations = Some(value(index)?.parse().map_err(|_| format!("--iterations: a count\n{USAGE}"))?);
            }
            "--function" => {
                index += 1;
                function = Some(value(index)?);
            }
            "--arm" => arm = true,
            "--heat" => heat = true,
            "--prefix" => prefix = true,
            "--stack-diffs" => score_options.stack_differences = true,
            "--algorithm" => {
                index += 1;
                score_options.algorithm = score::Algorithm::parse(&value(index)?)?;
            }
            "--ignore-branch-targets" => score_options.ignore_branch_targets = true,
            "--no-ignore-branch-targets" => score_options.ignore_branch_targets = false,
            "--stop-on-zero" => stop_on_zero = true,
            "--keep-prob" => {
                index += 1;
                keep_prob = value(index)?.parse::<f64>().map_err(|_| "--keep-prob: a number in 0..1")?;
                if !(0.0..=1.0).contains(&keep_prob) { return Err("--keep-prob: a number in 0..1".into()); }
            },
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
        None if text.contains("/tla/") => "tla",
        None => "tbs",
    };
    let target = match target {
        Some(target) => target,
        None => decomp_target(Some(&format!("{game}-en")))?,
    };
    if !minutes.is_finite() || minutes < 0.0 { return Err("--minutes must be finite and nonnegative".into()); }
    Ok(Options { draft, addresses, game, target, minutes, jobs, seed, rng_seed, perm_seed, function, iterations, arm, heat, prefix, score_options, keep_prob, stop_on_zero })
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
        let mut steps = if problem.arm {
            crate::diff::arm_steps(problem.target, &problem.draft, &object)?
        } else {
            crate::diff::draft_steps(problem.target, &problem.draft, &object)?
        };
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
        let assembly = self.object.with_extension("s").to_string_lossy().into_owned();
        let mut step = self.steps[0].clone();
        if !step.iter().any(|argument| argument == "-S")
            && !step.iter().any(|argument| argument == &preprocessed.to_string_lossy()) {
            return Err("no compile step to preprocess with".into());
        }
        for argument in step.iter_mut() {
            if argument == "-S" {
                *argument = "-E".into();
            } else if *argument == assembly {
                *argument = preprocessed.to_string_lossy().into_owned();
            }
        }
        // ARM compilation also leaves an .i file. It belongs to the previous
        // candidate, not necessarily to the expansion being prepared now.
        crate::process::run(&step, root())?;
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
    steps: Vec<Step>,
    full: Option<c::Node>,
}

struct Problem {
    draft: PathBuf,
    text: String,
    stem: String,
    target: DecompTarget,
    nodes: Vec<Node>,
    choices: bool,
    listing: Vec<String>,
    listing_rows: Option<Vec<score::Line>>,
    score_options: score::Options,
    symbol: Option<String>,
    unit: Unit,
    function: Option<String>,
    arm: bool,
    /// --prefix: a longer matched prefix ranks first.
    prefix: bool,
    /// --heat: where rewrites are aimed.
    aim: Option<aim::Aim>,
}

impl Problem {
    fn state(&self, expanded: Expanded) -> State {
        self.state_of(Arc::new(expanded))
    }

    fn state_of(&self, expanded: Arc<Expanded>) -> State {
        let parsed = locate(&expanded.text, self.function.as_deref(), &self.unit.typedef_names())
            .ok()
            .map(|mut located| {
                expanded.apply_ast(&mut located.function);
                (located.start, located.end, located.function)
            });
        let full = expanded.unit.as_deref().cloned();
        State { expanded, parsed, steps: Vec::new(), full }
    }

    fn source(&self, state: &State) -> String {
        if let Some(full) = &state.full { return state.expanded.restore(&full.print()); }
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
        let mut score = score::score_with(&self.listing, &compiled, self.score_options);
        if let Some(listing) = &self.listing_rows {
            let candidate = score::disassemble(&job.object, self.symbol.as_deref())?;
            score::weighted(listing, &candidate, self.score_options, &mut score);
        }
        Ok(score)
    }

}

#[derive(Clone)]
struct Found {
    score: Score,
    unnatural: usize,
    text: String,
    steps: Vec<Step>,
    expanded: Arc<Expanded>,
    job: usize,
    iteration: u64,
}

impl Found {
    fn rank(&self, base_unnatural: usize, prefix: bool) -> ((std::cmp::Reverse<usize>, u64), usize, usize, usize) {
        (self.score.key(prefix), self.score.differ, self.unnatural.saturating_sub(base_unnatural), self.steps.len())
    }
}

struct Reference {
    listing: Vec<String>,
    scaffold: Option<String>,
    rows: Option<Vec<score::Line>>,
}

fn reference(options: &Options) -> Result<Reference, String> {
    let mut listing = Vec::new();
    let mut rows = Some(Vec::new());
    let mut scaffold = None;
    for address in &options.addresses {
        let object = root()
            .join(options.target.output_dir)
            .join(format!("obj/recon/{}/raw/{address}.o", options.game));
        if object.exists() {
            // Listings can contain several functions (for example moji.o).
            // A named target must not be scored against its neighbours.
            let symbol = options.function.as_deref().filter(|name| crate::diff::text_symbols(&object).is_ok_and(|symbols| symbols.iter().any(|s| s == name)));
            listing.extend(crate::diff::disassemble(&object, symbol)?);
            if let Some(rows) = &mut rows { rows.extend(score::disassemble(&object, symbol)?); }
        } else {
            rows = None;
            let (tokens, name) = crate::diff::scaffold_reference(options.target, address, options.arm)?;
            listing.extend(tokens);
            if options.addresses.len() == 1 {
                scaffold = Some(name);
            }
        }
    }
    Ok(Reference { listing, scaffold, rows })
}

pub fn run(arguments: &[String]) -> Result<(), String> {
    let options = parse_options(arguments)?;
    if options.arm {
        println!("{}", crate::diff::GAME_ARM_NOTICE);
    }
    let draft = std::fs::canonicalize(&options.draft).map_err(|e| format!("{}: {e}", options.draft.display()))?;
    let text = std::fs::read_to_string(&draft).map_err(|e| format!("{}: {e}", draft.display()))?;
    let stem = draft.file_stem().map(|s| s.to_string_lossy().into_owned()).unwrap_or_default();
    let output = root().join("out/permute").join(&stem);
    let nodes = perm::parse(&text)?;
    let Reference { listing, scaffold, rows: listing_rows } = reference(&options)?;
    let problem = Problem {
        draft,
        text,
        stem,
        target: options.target,
        choices: perm::has_choices(&nodes),
        nodes,
        listing,
        listing_rows,
        score_options: options.score_options,
        symbol: None,
        prefix: options.prefix,
        aim: None,
        unit: Unit::default(),
        function: options.function.clone(),
        arm: options.arm,
    };
    execute(problem, &options, scaffold, &output)
}

/// A run's own scratch, removed however the run ends: an error, a panic
/// unwinding through the search, or success.
struct Scratch(PathBuf);

impl Drop for Scratch {
    fn drop(&mut self) {
        let _ = std::fs::remove_dir_all(&self.0);
        // The draft's folder too, when a failed first run left it empty.
        if let Some(parent) = self.0.parent() {
            let _ = std::fs::remove_dir(parent);
        }
    }
}

fn select_function(unit: &c::Node, selected: Option<&str>) -> Result<String, String> {
    if let Some(name) = selected {
        return randomize::target_index(unit, name).map(|_| name.to_string()).ok_or_else(|| format!("the draft does not define {name}"));
    }
    let c::Node::FileAst { ext } = unit else { return Err("expected a translation unit".into()) };
    let names: Vec<_> = ext.iter().filter_map(|n| match n {
        c::Node::FuncDef { decl, .. } => match decl.as_ref() {
            c::Node::Decl { name: Some(name), funcspec, .. } if !funcspec.iter().any(|s| s == "inline") => Some(name.clone()), _ => None,
        }, _ => None,
    }).collect();
    if names.len() == 1 { Ok(names[0].clone()) } else { Err(format!("choose a definition with --function (found: {})", names.join(", "))) }
}

fn prepare(job: &Job, expanded: &mut Expanded, target: &str) -> Result<(), String> {
    std::fs::write(&job.candidate, expanded.preprocess_text()).map_err(|e| e.to_string())?;
    let mut unit = c::parse(&job.preprocess()?)?;
    unit.extract_function(target)?;
    expanded.apply_c_ast(&mut unit);
    expanded.unit = Some(Arc::new(unit));
    Ok(())
}

/// Set by the first SIGINT or SIGTERM: the search and the minimizer stop
/// where they stand, the best so far is written and the scratch removed.
/// A second signal ends the process at once; the next run sweeps its scratch.
static INTERRUPTED: AtomicBool = AtomicBool::new(false);

extern "C" fn interrupted(_: libc::c_int) {
    INTERRUPTED.store(true, Ordering::Relaxed);
}

fn stop_on_signals() {
    for signal in [libc::SIGINT, libc::SIGTERM] {
        // SAFETY: the handler only stores to an atomic, which is
        // async-signal-safe; SA_RESETHAND restores the default for a second.
        unsafe {
            let mut action: libc::sigaction = std::mem::zeroed();
            action.sa_sigaction = interrupted as *const () as libc::sighandler_t;
            action.sa_flags = libc::SA_RESETHAND;
            libc::sigemptyset(&mut action.sa_mask);
            libc::sigaction(signal, &action, std::ptr::null_mut());
        }
    }
}

/// Remove the scratch of earlier runs on this draft whose process has gone
/// (killed before its guard could run); a live run's scratch stays.
fn sweep(output: &Path) {
    let Ok(entries) = std::fs::read_dir(output) else { return };
    for entry in entries.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        let Some(pid) = name.strip_prefix("run-").and_then(|pid| pid.parse::<libc::pid_t>().ok()) else { continue };
        // SAFETY: signal 0 only asks whether the process exists.
        let gone = unsafe { libc::kill(pid, 0) } != 0 && std::io::Error::last_os_error().raw_os_error() == Some(libc::ESRCH);
        if gone {
            let _ = std::fs::remove_dir_all(entry.path());
        }
    }
}

fn execute(mut problem: Problem, options: &Options, scaffold: Option<String>, output: &Path) -> Result<(), String> {
    // The written spelling first: its preprocessed unit gives the parser
    // its declarations, and its score is where the search starts.
    // This run's own scratch, so two runs on one draft never share files.
    sweep(output);
    stop_on_signals();
    let guard = Scratch(output.join(format!("run-{}", std::process::id())));
    let scratch = guard.0.clone();
    let setup = Job::new(&problem, &scratch.join("setup"))?;
    let mut original = perm::expand(&problem.nodes, None)?;
    // Preprocess before AST permutations: their temporary grouping blocks
    // intentionally do not have the final C declaration scopes.
    std::fs::write(&setup.candidate, original.preprocess_text()).map_err(|e| e.to_string())?;
    let preprocessed = setup.preprocess()?;
    let mut full_unit = c::parse(&preprocessed)?;
    let target = select_function(&full_unit, problem.function.as_deref())?;
    problem.function = Some(target.clone());
    full_unit.extract_function(&target)?;
    original.apply_c_ast(&mut full_unit);
    problem.unit = scan_unit(&full_unit.print())?;
    if !options.heat {
        original.unit = Some(Arc::new(full_unit));
        problem.symbol = Some(target);
    }
    let base_state = problem.state(original);
    let base_text = problem.source(&base_state);
    setup.compile(&base_text)?;
    if let Some(name) = scaffold {
        if crate::diff::text_symbols(&setup.object)?.contains(&name) {
            problem.symbol = Some(name);
        }
    }
    let random = (base_state.full.is_some() || base_state.parsed.is_some()) && (!problem.choices || perm::randomizes(&problem.nodes));
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
    let base_score = problem.evaluate(&setup, &base_text)?;
    if options.heat && random {
        problem.aim = aim_at(&problem, &setup, &base_state, &base_text)?;
    }
    let base_unnatural = problem.unnatural(&base_state);
    let base = Found { score: base_score, unnatural: base_unnatural, text: base_text, steps: Vec::new(), expanded: base_state.expanded.clone(), job: 0, iteration: 0 };
    println!(
        "{}: listing {} instructions; draft {}; {} job(s), {}, search seed {}; rewrites: {}",
        problem.stem,
        problem.listing.len(),
        base.score.summary_with(problem.prefix),
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
    if let Some(seed) = options.perm_seed {
        println!("forced PERM seed {seed}, RNG seed {}", options.rng_seed.as_deref().unwrap_or("0 (fresh seeds on restart)"));
    }
    let started = Instant::now();
    let deadline = options.iterations.is_none().then(|| started + Duration::from_secs_f64(options.minutes * 60.0));
    let found = search(&problem, &base_state, &base, random, options, deadline, &scratch, started)?;
    let best = minimize_best(&problem, &base, &found, &scratch.join("minimize"))?;
    write_best(&problem, &base, &best, output)
}

/// The base's heat on its printed function: its text compiled once more
/// with -g, its rows charged to lines as `alchemy diff --heat` charges them.
fn aim_at(problem: &Problem, setup: &Job, base_state: &State, base_text: &str) -> Result<Option<aim::Aim>, String> {
    let Some((_, _, function)) = &base_state.parsed else { return Ok(None) };
    let printed = function.print();
    let Some(at) = base_text.find(printed.trim_end()) else {
        println!("heat: the printed function is not in the compiled text; rewrites are not aimed");
        return Ok(None);
    };
    std::fs::write(&setup.candidate, base_text).map_err(|e| format!("{}: {e}", setup.candidate.display()))?;
    let name = setup.candidate.file_name().unwrap_or_default().to_string_lossy().into_owned();
    let located = crate::diff::located(&setup.steps, &setup.object, problem.symbol.as_deref(), &name)?;
    let heat = crate::diff::heat(&problem.listing, &located);
    let first_line = base_text[..at].matches('\n').count() as u32 + 1;
    let aim = aim::Aim::new(&printed, first_line, &heat);
    println!(
        "heat: {} differing rows on {} lines; first divergence at printed line {}",
        heat.lines.values().sum::<usize>(),
        heat.lines.len(),
        heat.first.map_or("none".into(), |line| (i64::from(line) + 1 - i64::from(first_line)).to_string())
    );
    Ok(aim.aims().then_some(aim))
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
    let global = Mutex::new((base.score.key(problem.prefix), base.score.differ));
    let found = Mutex::new(Vec::new());
    let failures = Mutex::new(Vec::new());
    let compiled = std::sync::atomic::AtomicU64::new(0);
    let count = perm::count(&problem.nodes)?;
    if options.perm_seed.is_some_and(|seed| seed >= count) {
        return Err(format!("--seed: PERM seed must be in 0..{count}"));
    }
    let seeds = Mutex::new(perm::Seeds::new(count, random, options.seed));
    let running = std::sync::atomic::AtomicUsize::new(options.jobs);
    std::thread::scope(|scope| {
        // Each minute, the trajectory notes where the search stands.
        let (global_ref, compiled_ref, running_ref) = (&global, &compiled, &running);
        scope.spawn(move || {
            let mut minute = 1u64;
            while running_ref.load(Ordering::Relaxed) > 0 {
                std::thread::sleep(Duration::from_millis(200));
                if started.elapsed() >= Duration::from_secs(60 * minute) {
                    let ((std::cmp::Reverse(prefix), weighted), differ) = *global_ref.lock().unwrap();
                    let prefix = if problem.prefix { format!("prefix {prefix}, ") } else { String::new() };
                    println!("{:>8.1}s  {} candidates compiled; best {prefix}weighted {weighted}, differ {differ}", started.elapsed().as_secs_f64(), compiled_ref.load(Ordering::Relaxed));
                    minute += 1;
                }
            }
        });
        for job_index in 0..options.jobs {
            let (stop, global, found, failures) = (&stop, &global, &found, &failures);
            let (compiled, running) = (&compiled, &running);
            let seeds = &seeds;
            scope.spawn(move || {
                let job = match Job::new(problem, &output.join(format!("job-{job_index:02}"))) {
                    Ok(job) => job,
                    Err(error) => {
                        failures.lock().unwrap().push(error);
                        running.fetch_sub(1, Ordering::Relaxed);
                        return;
                    }
                };
                let forced_rng = options.rng_seed.as_ref().map(|seed| Rng::from_decimal(seed).expect("validated randomizer seed"));
                let mut walk = rng::WalkRng::new(options.seed.wrapping_mul(1_000_003).wrapping_add(job_index as u64), forced_rng);
                let mut env = base_state.parsed.as_ref().map(|(_, _, function)| Env::new(&problem.unit, function));
                // Keyed by a hash of the text: a long run sees many thousands.
                let mut cache: HashMap<SourceHash, Option<Score>> = HashMap::new();
                let mut prepared = HashMap::new();
                prepared.insert(base_state.expanded.key(), base_state.expanded.clone());
                let (mut best, mut current) = ((base_state.clone(), base.clone()), (base_state.clone(), base.clone()));
                let mut iteration = 0u64;
                let mut last_compiled = false;
                while !stop.load(Ordering::Relaxed)
                    && !INTERRUPTED.load(Ordering::Relaxed)
                    && options.iterations.is_none_or(|limit| iteration < limit)
                    && deadline.is_none_or(|deadline| Instant::now() < deadline)
                {
                    let seed = if let Some(seed) = options.perm_seed {
                        if !random && iteration != 0 { break; }
                        seed
                    } else {
                        let Some(seed) = seeds.lock().unwrap().next() else { break };
                        seed
                    };
                    iteration += 1;
                    // 0.6 regardless of whether its score improved. A failed
                    // compile or a zero score starts a fresh candidate.
                    let last_score = (iteration > 1).then_some(if last_compiled { current.1.score.weighted } else { u64::MAX });
                    let keep = walk.keep(random, last_score, options.keep_prob);
                    let rng = &mut walk.mutation;
                    let mut state = if !keep {
                        match perm::expand_seed(&problem.nodes, seed) {
                            Ok(mut expanded) => {
                                if base_state.full.is_some() {
                                    let key = expanded.key();
                                    if let Some(expanded) = prepared.get(&key) { problem.state_of(expanded.clone()) }
                                    else {
                                        if prepare(&job, &mut expanded, problem.function.as_deref().unwrap()).is_err() { last_compiled = false; continue; }
                                        let expanded = Arc::new(expanded);
                                        if prepared.len() < 128 { prepared.insert(key, expanded.clone()); }
                                        problem.state_of(expanded)
                                    }
                                } else { problem.state(expanded) }
                            },
                            Err(_) => { last_compiled = false; continue; },
                        }
                    } else {
                        current.0.clone()
                    };
                    if random {
                        if let Some(full) = &mut state.full {
                            if let Some(step) = randomize::mutate(full, problem.function.as_deref().unwrap(), rng) { state.steps.push(step); }
                        } else if let (Some((_, _, function)), Some(env)) = (state.parsed.as_mut(), env.as_mut()) {
                            env.refresh(function);
                            let step = match &problem.aim {
                                Some(aim) => aim::aimed(function, env, rng, &Kind::ALL, aim),
                                None => mutate_with(function, env, rng, &Kind::ALL),
                            };
                            if let Some(step) = step {
                                state.steps.push(step);
                            }
                        }
                    }
                    let text = problem.source(&state);
                    let key = text_key(&text);
                    let result = match cache.get(&key) {
                        Some(result) => result.clone(),
                        None => {
                            let result = problem.evaluate(&job, &text).ok();
                            compiled.fetch_add(1, Ordering::Relaxed);
                            if cache.len() < 100_000 { cache.insert(key, result.clone()); }
                            result
                        }
                    };
                    last_compiled = result.is_some();
                    // compilation fails. Score storage only tracks successes.
                    if options.rng_seed.is_some() { current.0 = state.clone(); }
                    let Some(score) = result else { continue };
                    let candidate = Found {
                        unnatural: problem.unnatural(&state),
                        score,
                        text,
                        steps: state.steps.clone(),
                        expanded: state.expanded.clone(),
                        job: job_index,
                        iteration,
                    };
                    if candidate.rank(base.unnatural, problem.prefix) < best.1.rank(base.unnatural, problem.prefix) {
                        best = (state.clone(), candidate.clone());
                        found.lock().unwrap().push(candidate.clone());
                        let mut global = global.lock().unwrap();
                        if (candidate.score.key(problem.prefix), candidate.score.differ) < *global {
                            *global = (candidate.score.key(problem.prefix), candidate.score.differ);
                            println!(
                                "{:>8.1}s  job {job_index:>2}  iteration {iteration:>6}  {}",
                                started.elapsed().as_secs_f64(),
                                candidate.score.summary_with(problem.prefix)
                            );
                        }
                    }
                    if candidate.score.weighted == 0 && options.stop_on_zero {
                        stop.store(true, Ordering::Relaxed);
                    }
                    current = (state, candidate);
                }
                let failed = cache.values().filter(|result| result.is_none()).count();
                println!("job {job_index:>2}: {iteration} iterations, {} distinct candidates, {failed} did not compile", cache.len());
                running.fetch_sub(1, Ordering::Relaxed);
            });
        }
    });
    let failures = failures.into_inner().unwrap();
    if let Some(error) = failures.first() {
        return Err(error.clone());
    }
    let mut all = found.into_inner().unwrap();
    all.sort_by(|a, b| {
        a.rank(base.unnatural, problem.prefix).cmp(&b.rank(base.unnatural, problem.prefix)).then(a.job.cmp(&b.job)).then(a.iteration.cmp(&b.iteration))
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

type SourceHash = [u8; 32];

fn text_key(text: &str) -> SourceHash {
    use sha2::Digest;
    sha2::Sha256::digest(text.as_bytes()).into()
}

/// A score from the cache, or from one compile.
fn scored(problem: &Problem, job: &Job, text: &str, cache: &mut HashMap<SourceHash, Option<Score>>) -> Option<Score> {
    cache.entry(text_key(text)).or_insert_with(|| problem.evaluate(job, text).ok()).clone()
}

impl Problem {
    /// One expansion's function with `steps` applied in order, each at the
    /// site its generator chose; a step whose site is gone is dropped.
    fn replay(&self, expanded: &Arc<Expanded>, steps: &[Step]) -> State {
        let mut state = self.state_of(expanded.clone());
        if let Some(full) = &mut state.full {
            for step in steps {
                if let Kind::Full(kind) = step.kind {
                    if randomize::replay(full, self.function.as_deref().unwrap(), kind, &mut step.rng.clone(), step.region) { state.steps.push(step.clone()); }
                }
            }
        } else if let Some((_, _, function)) = state.parsed.as_mut() {
            let mut env = Env::new(&self.unit, function);
            for step in steps {
                env.refresh(function);
                if mutate::apply(step.kind, function, &env, &mut step.rng.clone()) {
                    state.steps.push(step.clone());
                }
            }
        }
        state
    }

    fn unnatural(&self, state: &State) -> usize {
        if state.full.is_some() { return 0; }
        match &state.parsed {
            Some((_, _, function)) => unnatural(function, &Env::new(&self.unit, function)),
            None => 0,
        }
    }
}

/// Undo a candidate's rewrites one at a time, last first, keeping each
/// removal that leaves its weighted score (then differ, then its unnatural
/// spellings) no higher, until a whole pass removes nothing. A trail that
/// does not replay to the candidate's own text is left as found.
fn minimize(problem: &Problem, job: &Job, base: &Found, best: &Found, cache: &mut HashMap<SourceHash, Option<Score>>) -> Found {
    let replayed = problem.replay(&best.expanded, &best.steps);
    if replayed.steps.len() != best.steps.len() || problem.source(&replayed) != best.text {
        return best.clone();
    }
    let measure = |found: &Found| (found.score.key(problem.prefix), found.score.differ, found.unnatural.saturating_sub(base.unnatural));
    let mut current = best.clone();
    loop {
        let mut removed = false;
        let mut index = current.steps.len();
        while index > 0 {
            if INTERRUPTED.load(Ordering::Relaxed) {
                return current;
            }
            index -= 1;
            let mut trial = current.steps.clone();
            trial.remove(index);
            let state = problem.replay(&current.expanded, &trial);
            let text = problem.source(&state);
            // A rewrite that changes no text costs no compile.
            let score = if text == current.text { Some(current.score.clone()) } else { scored(problem, job, &text, cache) };
            let Some(score) = score else { continue };
            let candidate = Found { score, unnatural: problem.unnatural(&state), text, steps: state.steps, ..current.clone() };
            if measure(&candidate) <= measure(&current) {
                current = candidate;
                removed = true;
                index = index.min(current.steps.len());
            }
        }
        if !removed {
            return current;
        }
    }
}

/// Which line of `to` each line of `from` is, by their longest common
/// subsequence.
fn align(from: &[String], to: &[String]) -> Vec<Option<usize>> {
    let (_, script) = crate::diff::compare(from, to);
    let (mut out, mut j) = (Vec::with_capacity(from.len()), 0);
    for line in &script {
        match line.as_bytes()[0] {
            b' ' => {
                out.push(Some(j));
                j += 1;
            }
            b'+' => j += 1,
            _ => out.push(None),
        }
    }
    out
}

fn lines(text: &str) -> Vec<String> {
    text.split_inclusive('\n').map(str::to_string).collect()
}

/// Carry the rewrites between two printings of one function (`before`,
/// with none, and `after`) into the draft's own lines: each changed run of
/// printed lines replaces the draft lines it aligns with, and every other
/// draft line keeps its spelling. `None` when a change touches a line the
/// printer spelled differently from the draft.
fn transplant(draft: &str, before: &str, after: &str) -> Option<String> {
    let squeeze = |lines: &[String]| -> Vec<String> { lines.iter().map(|line| line.split_whitespace().collect()).collect() };
    let (d, p0, p1) = (lines(draft), lines(before), lines(after));
    let at = align(&squeeze(&p0), &squeeze(&d));
    let (_, script) = crate::diff::compare(&p0, &p1);
    // Runs of changes: (first printed line, lines removed, lines inserted).
    let mut edits: Vec<(usize, usize, Vec<String>)> = Vec::new();
    let (mut i, mut open) = (0, false);
    for line in &script {
        let (tag, text) = (line.as_bytes()[0], &line[2..]);
        if tag == b' ' {
            i += 1;
            open = false;
            continue;
        }
        if !open {
            edits.push((i, 0, Vec::new()));
            open = true;
        }
        let edit = edits.last_mut().unwrap();
        if tag == b'-' {
            edit.1 += 1;
            i += 1;
        } else {
            edit.2.push(text.to_string());
        }
    }
    let (mut out, mut next) = (String::new(), 0);
    for (start, removed, inserted) in edits {
        let (from, to) = if removed > 0 {
            let first = at[start]?;
            if (0..removed).any(|k| at[start + k] != Some(first + k)) {
                return None;
            }
            (first, first + removed)
        } else if start > 0 {
            let after = at[start - 1]? + 1;
            (after, after)
        } else {
            let first = at.first().copied().flatten()?;
            (first, first)
        };
        if from < next {
            return None;
        }
        out.extend(d[next..from].iter().map(String::as_str));
        out.extend(inserted.iter().map(String::as_str));
        next = to;
    }
    out.extend(d[next..].iter().map(String::as_str));
    Some(out)
}

/// A unified diff from `old` to `new` with three lines of context, both
/// named `name`, which `patch` applies.
fn unified(old: &str, new: &str, name: &str) -> String {
    let (a, b) = (lines(old), lines(new));
    let (_, script) = crate::diff::compare(&a, &b);
    // (tag, text, old lines before it, new lines before it)
    let mut entries = Vec::with_capacity(script.len());
    let (mut i, mut j) = (0, 0);
    for line in &script {
        let tag = line.as_bytes()[0];
        entries.push((tag, &line[2..], i, j));
        match tag {
            b' ' => (i, j) = (i + 1, j + 1),
            b'+' => j += 1,
            _ => i += 1,
        }
    }
    let changed: Vec<usize> = (0..entries.len()).filter(|&at| entries[at].0 != b' ').collect();
    if changed.is_empty() {
        return String::new();
    }
    let push = |out: &mut String, tag: char, text: &str| {
        out.push(tag);
        out.push_str(text);
        if !text.ends_with('\n') {
            out.push_str("\n\\ No newline at end of file\n");
        }
    };
    const CONTEXT: usize = 3;
    let mut out = format!("--- {name}\n+++ {name}\n");
    let mut k = 0;
    while k < changed.len() {
        let start = changed[k].saturating_sub(CONTEXT);
        let mut end = changed[k] + 1;
        k += 1;
        while k < changed.len() && changed[k] - end <= 2 * CONTEXT {
            end = changed[k] + 1;
            k += 1;
        }
        let range = &entries[start..(end + CONTEXT).min(entries.len())];
        let old_count = range.iter().filter(|entry| entry.0 != b'+').count();
        let new_count = range.iter().filter(|entry| entry.0 != b'-').count();
        out.push_str(&format!(
            "@@ -{},{old_count} +{},{new_count} @@\n",
            range[0].2 + usize::from(old_count > 0),
            range[0].3 + usize::from(new_count > 0)
        ));
        let mut r = 0;
        while r < range.len() {
            if range[r].0 == b' ' {
                push(&mut out, ' ', range[r].1);
                r += 1;
                continue;
            }
            // Each run of changes: its removals, then its insertions.
            let run = r + range[r..].iter().take_while(|entry| entry.0 != b' ').count();
            for tag in *b"-+" {
                for entry in range[r..run].iter().filter(|entry| entry.0 == tag) {
                    push(&mut out, tag as char, entry.1);
                }
            }
            r = run;
        }
    }
    out
}

/// One written candidate: minimized, then spelled as the draft spells it.
struct Best {
    found: Found,
    /// The rewrites the search applied, before minimizing.
    searched: Vec<Kind>,
    /// its function when the rewrites do not carry into them.
    text: String,
    respelled: bool,
}

/// The candidate in the draft's spelling: PERM macros at its choices as the
/// draft writes them, and its rewrites carried into the draft's lines, kept
/// only when that text scores exactly as the printed candidate does.
fn respell(problem: &Problem, job: &Job, found: &Found, cache: &mut HashMap<SourceHash, Option<Score>>) -> (String, bool) {
    let draft = found.expanded.restore(&found.expanded.text);
    let expected = if found.expanded.unit.is_some() {
        job.compile(&found.text).and_then(|()| score::disassemble(&job.object, problem.symbol.as_deref())).ok()
    } else { None };
    let verifies = |text: &str, cache: &mut HashMap<SourceHash, Option<Score>>| {
        if found.expanded.unit.is_some() {
            // Equal scalar scores do not imply equal instructions. Projection
            // must preserve the candidate's actual disassembly, including
            // relocations and literal data, before saving a smaller draft.
            expected.is_some() && problem.evaluate(job, text).ok().as_ref() == Some(&found.score)
                && score::disassemble(&job.object, problem.symbol.as_deref()).ok() == expected
        } else { scored(problem, job, text, cache).as_ref() == Some(&found.score) }
    };
    // Body/signature changes and new inline helpers usually fit back into
    // the original includes and comments. A changed external declaration
    // may have come from a header: keep the self-contained candidate then.
    if let Some(before) = found.expanded.unit.as_deref() {
        let state = problem.replay(&found.expanded, &found.steps);
        if let Some(after) = &state.full {
            if let Some((text, preserved)) = project_function(problem, &draft, before, after) {
                let text = found.expanded.restore(&text);
                if verifies(&text, cache) {
                    return (text, preserved);
                }
            }
        }
    }
    let plain = problem.source(&problem.state_of(found.expanded.clone()));
    let text = if found.text == plain { Some(draft) } else { transplant(&draft, &plain, &found.text) };
    let printed = |text: &str| locate(text, problem.function.as_deref(), &problem.unit.typedef_names()).ok().map(|located| located.function.print());
    match text {
        Some(text) if text == found.text => (text, true),
        Some(text)
            if printed(&text).zip(printed(&found.text)).is_none_or(|(a, b)| a == b)
                && verifies(&text, cache) =>
        {
            (text, true)
        }
        _ => (found.text.clone(), false),
    }
}

fn project_function(problem: &Problem, draft: &str, before: &c::Node, after: &c::Node) -> Option<(String, bool)> {
    let target = problem.function.as_deref()?;
    let old_index = randomize::target_index(before, target)?;
    let new_index = randomize::target_index(after, target)?;
    let (c::Node::FileAst { ext: old }, c::Node::FileAst { ext: new }) = (before, after) else { return None };
    // A PRETEND/SAMELINE region may start outside the target and end inside
    // it. The restored draft has already consumed the external marker;
    // projecting the printed function would leave an unmatched end marker.
    // Keep the complete candidate, whose markers still share their context.
    for kind in ["pretend", "sameline"] {
        let mut depth = 0usize;
        for line in old[old_index].print().lines() {
            match line.trim().strip_prefix(&format!("#pragma _permuter {kind} ")) {
                Some("start") => depth += 1,
                Some("end") => depth = depth.checked_sub(1)?,
                _ => {}
            }
        }
        if depth != 0 { return None; }
    }
    let old_context: Vec<_> = old.iter().enumerate().filter(|(i, _)| *i != old_index).map(|(_, n)| n).collect();
    let mut new_context = Vec::new();
    let mut helpers = String::new();
    for (i, item) in new.iter().enumerate() {
        if i == new_index { continue; }
        if !old_context.contains(&item) && matches!(item, c::Node::FuncDef { .. }) { helpers.push_str(&item.print()); }
        else { new_context.push(item); }
    }
    if old_context != new_context { return None; }
    let located = locate(draft, Some(target), &problem.unit.typedef_names()).ok()?;
    let original = &draft[located.start..located.end];
    let transplanted = transplant(original, &old[old_index].print(), &new[new_index].print());
    let preserved = transplanted.is_some();
    let function = transplanted.unwrap_or_else(|| new[new_index].print());
    Some((format!("{}{helpers}{}{}", &draft[..located.start], function.trim_end(), &draft[located.end..]), preserved))
}

/// Minimize and respell the search's best, until three distinct remain.
fn minimize_best(problem: &Problem, base: &Found, found: &[Found], directory: &Path) -> Result<Vec<Best>, String> {
    if found.is_empty() {
        return Ok(Vec::new());
    }
    let job = Job::new(problem, directory)?;
    let mut cache = HashMap::new();
    let mut best: Vec<Best> = Vec::new();
    let mut assemblies = std::collections::HashSet::new();
    for candidate in found {
        if best.len() == 3 {
            break;
        }
        let minimized = minimize(problem, &job, base, candidate, &mut cache);
        let (text, respelled) = respell(problem, &job, &minimized, &mut cache);
        if best.iter().any(|seen| seen.text == text) {
            continue;
        }
        // Different spellings of the same compiled result should not fill
        // all three output slots. Keep relocations and literal data in the
        // key, including distinctions ignored by the weighted score.
        job.compile(&text)?;
        let assembly = score::disassemble(&job.object, problem.symbol.as_deref())?;
        if !assemblies.insert(text_key(&format!("{assembly:?}"))) { continue; }
        let searched = candidate.steps.iter().map(|step| step.kind).collect();
        best.push(Best { found: minimized, searched, text, respelled });
    }
    best.sort_by(|a, b| {
        let (a, b) = (&a.found, &b.found);
        a.rank(base.unnatural, problem.prefix).cmp(&b.rank(base.unnatural, problem.prefix)).then(a.job.cmp(&b.job)).then(a.iteration.cmp(&b.iteration))
    });
    Ok(best)
}

fn write_best(problem: &Problem, base: &Found, found: &[Best], output: &Path) -> Result<(), String> {
    for entry in std::fs::read_dir(output).map_err(|e| format!("{}: {e}", output.display()))?.flatten() {
        let name = entry.file_name().to_string_lossy().into_owned();
        if name.starts_with("best-") {
            let _ = std::fs::remove_file(entry.path());
        }
    }
    if INTERRUPTED.load(Ordering::Relaxed) {
        println!("interrupted: the best so far, minimized as far as it got");
    }
    println!("draft:  {}", base.score.summary_with(problem.prefix));
    if found.is_empty() {
        println!("no candidate beat the draft");
        return Ok(());
    }
    let name = problem.draft.strip_prefix(root()).unwrap_or(&problem.draft).to_string_lossy().into_owned();
    for (index, entry) in found.iter().take(3).enumerate() {
        let best = &entry.found;
        let path = output.join(format!("best-{}.c", index + 1));
        let fakematch = if best.unnatural > base.unnatural {
            "\n   It relies on a cast to an operand's own type, a cast of a cast or (*p).member: tag it FAKEMATCH (S2) or reshape it."
        } else {
            ""
        };
        let kinds: Vec<Kind> = best.steps.iter().map(|step| step.kind).collect();
        let spelling = if entry.respelled { "" } else { "\n   Its rewrites do not carry into the draft's lines: this is the permuter's printed source." };
        let review = if kinds.iter().any(|k| matches!(k, Kind::Full(_))) { "\n   Source hypothesis: review type and behavior changes, and tag compiler-steering devices under S2 before adoption." } else { "" };
        let header = format!(
            "/* alchemy permute {}: {}; draft {}.\n   Job {}, iteration {}; rewrites: {} (minimized from {}: {}).{spelling}{fakematch}{review} */\n",
            problem.stem,
            best.score.summary_with(problem.prefix),
            base.score.summary_with(problem.prefix),
            best.job,
            best.iteration,
            rewrites(&kinds),
            entry.searched.len(),
            rewrites(&entry.searched)
        );
        std::fs::write(&path, format!("{header}{}", entry.text)).map_err(|e| format!("{}: {e}", path.display()))?;
        // Against the draft file itself, so `patch -p0` from the checkout
        // applies it; the header comment stays in best-N.c only.
        let diff = unified(&problem.text, &entry.text, &name);
        std::fs::write(path.with_extension("diff"), &diff).map_err(|e| format!("{}: {e}", path.display()))?;
        println!(
            "best-{}: {}  {}\n        rewrites {} -> {}: {}",
            index + 1,
            best.score.summary_with(problem.prefix),
            path.strip_prefix(root()).unwrap_or(&path).display(),
            entry.searched.len(),
            kinds.len(),
            rewrites(&kinds)
        );
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
        assert!(parsed.rng_seed.is_none());
        assert_eq!(parse_options(&args(&["08000000.c", "--rng-seed", "-99999999999999999999"])).unwrap().rng_seed.as_deref(), Some("-99999999999999999999"));
        assert!(parse_options(&args(&["08000000.c", "--rng-seed", "1,2"])).is_err());
        assert_eq!(parsed.score_options.algorithm, score::Algorithm::Difflib);
        assert_eq!(parse_options(&args(&["08000000.c", "--algorithm", "levenshtein"])).unwrap().score_options.algorithm, score::Algorithm::Levenshtein);
        assert!(parse_options(&args(&["08000000.c", "--algorithm", "lcs"])).is_err());
        assert_eq!(parsed.target.id.as_str(), "tbs-en");
        let parsed = parse_options(&args(&["x.c", "08000000,08000010", "--edition", "tla-de", "--minutes", "0.5", "--jobs", "3", "--search-seed", "9"])).unwrap();
        assert_eq!((parsed.addresses.len(), parsed.game, parsed.jobs, parsed.seed, parsed.minutes), (2, "tla", 3, 9, 0.5));
        for (text, manual, forced) in [("9", 0, Some("9")), ("1,-99999999999999999999", 1, Some("-99999999999999999999")), ("2,-000", 2, None)] {
            let parsed = parse_options(&args(&["08000000.c", "--seed", text])).unwrap();
            assert_eq!(parsed.perm_seed, Some(manual));
            assert_eq!(parsed.rng_seed.as_deref(), forced);
        }
        for text in ["1,2,3", "bad", "1,", "-1,2"] {
            assert!(parse_options(&args(&["08000000.c", "--seed", text])).is_err());
        }
        assert!(parse_options(&args(&["x.c"])).is_err());
        assert!(parse_options(&args(&["recon/tbs/en/overlays/resource_383_c_020048c8.c"])).is_err());
        assert!(parse_options(&args(&["08000000.c", "--bogus"])).is_err());
        assert!(!parse_options(&args(&["08000000.c"])).unwrap().arm);
        assert!(parse_options(&args(&["recon/tbs/en/main/0800a0f8.c", "--arm"])).unwrap().arm);
        assert!(parse_options(&args(&["08000000.c", "--edition", "tla-en", "--arm"])).unwrap().arm);
        let parsed = parse_options(&args(&["08000000.c", "--stack-diffs", "--ignore-branch-targets", "--keep-prob", "0.25", "--stop-on-zero"])).unwrap();
        assert!(parsed.score_options.stack_differences && parsed.score_options.ignore_branch_targets && parsed.stop_on_zero);
        assert_eq!(parsed.keep_prob, 0.25);
        for value in ["NaN", "-1", "1.1"] {
            assert!(parse_options(&args(&["08000000.c", "--keep-prob", value])).is_err());
        }
        for value in ["NaN", "inf", "-1"] {
            assert!(parse_options(&args(&["08000000.c", "--minutes", value])).is_err());
        }
    }

    #[test]
    fn arm_jobs_compile_with_stock_agbcc_arm() {
        let work = tempfile::tempdir().unwrap();
        let draft = PathBuf::from("recon/tbs/en/main/0800a0f8.c");
        let mut problem = test_problem(&draft, "void F(void)\n{\n}\n", Vec::new(), None);
        let thumb = Job::new(&problem, work.path()).unwrap();
        problem.arm = true;
        let arm = Job::new(&problem, work.path()).unwrap();
        let compiler = |job: &Job| job.steps.iter().flatten().any(|argument| argument.ends_with("agbcc/agbcc_arm"));
        assert!(!compiler(&thumb));
        assert!(compiler(&arm));
        std::fs::write(&arm.candidate, "int F(int x) { return x + 1; }\n").unwrap();
        assert!(arm.preprocess().unwrap().contains("return x + 1"));
    }

    #[test]
    fn compatibility_c_tree_compiles_through_both_game_routes() {
        crate::compiler::routing::prefer_installed_binutils();
        let source = "#include \"types.h\"\ntypedef struct { u32 word; s16 pair[2]; } Item;\nstatic inline u32 Twice(u32 x) { return x + x; }\nu32 F(Item *p) { u32 n = p->word; if (n) return Twice(n); return (u16)p->pair[1]; }\n";
        for arm in [false, true] {
            let work = tempfile::tempdir().unwrap();
            let mut problem = test_problem(Path::new("recon/tbs/en/main/08000000.c"), source, Vec::new(), Some("F"));
            problem.arm = arm;
            let job = Job::new(&problem, work.path()).unwrap();
            job.compile(source).unwrap();
            let expected = crate::diff::disassemble(&job.object, Some("F")).unwrap();
            let mut unit = c::parse(&job.preprocess().unwrap()).unwrap();
            unit.extract_function("F").unwrap();
            job.compile(&unit.print()).unwrap();
            assert_eq!(crate::diff::disassemble(&job.object, Some("F")).unwrap(), expected);
        }
    }

    #[test]
    fn unified_diff_has_numbered_hunks_with_removals_first() {
        let diff = unified("a\nb\nc\nd\ne\nf\ng\n", "a\nb\nc\nD\ne\nf\ng\n", "x.c");
        assert_eq!(diff, "--- x.c\n+++ x.c\n@@ -1,7 +1,7 @@\n a\n b\n c\n-d\n+D\n e\n f\n g\n");
        let old: String = (1..=20).map(|n| format!("{n}\n")).collect();
        let new = old.replace("\n2\n", "\ntwo\n").replace("\n18\n", "\n18\neighteen\n");
        let diff = unified(&old, &new, "x.c");
        assert_eq!(diff.matches("@@ -").count(), 2, "{diff}");
        assert!(diff.contains("@@ -1,5 +1,5 @@\n 1\n-2\n+two\n") && diff.contains("@@ -16,5 +16,6 @@\n"), "{diff}");
        assert_eq!(apply_unified(&old, &diff), new);
        assert_eq!(unified(&old, &old, "x.c"), "");
    }

    /// What `patch` does with one of our diffs, for texts that end in a
    /// newline: copy, check context and removals, insert.
    fn apply_unified(old: &str, diff: &str) -> String {
        let old = lines(old);
        let (mut out, mut at) = (String::new(), 0);
        for line in diff.split_inclusive('\n').skip(2) {
            if let Some(header) = line.strip_prefix("@@ -") {
                let (start, count) = header.split(' ').next().unwrap().split_once(',').unwrap();
                let (start, count): (usize, usize) = (start.parse().unwrap(), count.parse().unwrap());
                let begin = if count == 0 { start } else { start - 1 };
                out.extend(old[at..begin].iter().map(String::as_str));
                at = begin;
                continue;
            }
            let (tag, text) = line.split_at(1);
            match tag {
                " " | "-" => {
                    assert_eq!(old[at], text, "context or removal at line {}", at + 1);
                    if tag == " " {
                        out.push_str(text);
                    }
                    at += 1;
                }
                _ => out.push_str(text),
            }
        }
        out.extend(old[at..].iter().map(String::as_str));
        out
    }

    pub(super) fn test_problem(draft: &Path, text: &str, listing: Vec<String>, function: Option<&str>) -> Problem {
        let nodes = perm::parse(text).unwrap();
        Problem {
            draft: draft.to_path_buf(),
            text: text.into(),
            stem: "08000000".into(),
            target: decomp_target(Some("tbs-en")).unwrap(),
            choices: perm::has_choices(&nodes),
            nodes,
            listing,
            listing_rows: None,
            score_options: score::Options::default(),
            symbol: None,
            prefix: false,
            aim: None,
            unit: scan_unit(text).unwrap_or_default(),
            function: function.map(str::to_string),
            arm: false,
        }
    }

    #[test]
    fn rewrites_carry_into_the_draft_lines_and_the_diff_patches_the_draft() {
        // The draft's own layout: a comment, odd spacing and a PERM choice.
        let draft = "extern int gX;\nextern int gY;\n\n/* kept as written */\nvoid Set(int a, int b)\n{\n    gX   =   a;   /* x */\n    gY = PERM_ONEOF(b, b + 1);\n    if (a)\n        gX = b;\n}\n";
        let problem = test_problem(Path::new("recon/tbs/en/main/08000000.c"), draft, Vec::new(), None);
        let mut rng = Rng::new(2);
        let expanded = Arc::new(perm::expand(&problem.nodes, Some(&mut rng)).unwrap());
        let plain = problem.source(&problem.state_of(expanded.clone()));
        // Some seed's rewrite touches one statement only.
        let (text, respelled) = (1..200u64)
            .find_map(|seed| {
                let mut state = problem.state_of(expanded.clone());
                let (_, _, function) = state.parsed.as_mut().unwrap();
                let mut env = Env::new(&problem.unit, function);
                let step = mutate(function, &mut env, &mut Rng::new(seed))?;
                state.steps.push(step);
                let after = problem.source(&state);
                let spelled = transplant(&expanded.restore(&expanded.text), &plain, &after)?;
                (after != plain && spelled.contains("gX   =   a;   /* x */")).then_some((spelled, after))
            })
            .expect("a rewrite that leaves the first statement alone");
        assert!(text.contains("/* kept as written */") && !perm::has_macros(&text), "{text}");
        // The respelled text holds the rewritten function exactly.
        let printed = |text: &str| locate(text, None, &problem.unit.typedef_names()).unwrap().function.print();
        assert_eq!(printed(&text), printed(&respelled));
        let diff = unified(draft, &text, "recon/tbs/en/main/08000000.c");
        assert_eq!(apply_unified(draft, &diff), text);
        // And `patch` itself, where the system has it.
        let work = tempfile::tempdir().unwrap();
        let file = work.path().join("08000000.c");
        std::fs::write(&file, draft).unwrap();
        std::fs::write(work.path().join("best.diff"), &diff).unwrap();
        let patched = std::process::Command::new("patch")
            .args(["-s", "08000000.c", "best.diff"])
            .current_dir(work.path())
            .status();
        if let Ok(status) = patched {
            assert!(status.success());
            assert_eq!(std::fs::read_to_string(&file).unwrap(), text);
        }
        // A change on a line the printer spells differently does not carry.
        assert_eq!(transplant("int a;\nx  =  1 ;\n", "int a;\nx = 1;\n", "int a;\nx = 2;\n"), Some("int a;\nx = 2;\n".into()));
        assert_eq!(transplant("int a; x = 1;\n", "int a;\nx = 1;\n", "int a;\nx = 2;\n"), None);
    }

    /// A compiled reference for `listing`, as alchemy diff compiles drafts.
    fn listing_of(work: &Path, source: &str) -> Vec<String> {
        let target = decomp_target(Some("tbs-en")).unwrap();
        let reference = work.join("reference/08000000.c");
        let object = work.join("reference/obj/08000000.o");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        std::fs::write(&reference, source).unwrap();
        for step in crate::diff::draft_steps(target, &reference, &object).unwrap() {
            crate::process::run(&step, root()).unwrap();
        }
        crate::diff::disassemble(&object, None).unwrap()
    }

    #[test]
    fn compatibility_scoring_reads_real_compiler_objects() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let text = "int F(int x) { return x + 2; }\n";
        let listing = listing_of(work.path(), text);
        let draft = work.path().join("draft/08000000.c");
        let mut problem = test_problem(&draft, text, listing, None);
        problem.listing_rows = Some(score::disassemble(&work.path().join("reference/obj/08000000.o"), None).unwrap());
        let job = Job::new(&problem, &work.path().join("job")).unwrap();
        assert_eq!(problem.evaluate(&job, text).unwrap().weighted, 0);
        let score = problem.evaluate(&job, "int F(int x) { return x + 3; }\n").unwrap();
        assert_eq!((score.weighted, score.regalloc), (5, 1));
        assert!(score.differ > 0);
    }

    #[test]
    fn whole_tree_type_search_and_minimizer_reach_a_byte_load_on_both_routes() {
        crate::compiler::routing::prefer_installed_binutils();
        let source = "extern int input; int F(void) { return input; }\n";
        for arm in [false, true] {
            let work = tempfile::tempdir().unwrap();
            let draft = work.path().join("draft/08000000.c");
            let mut problem = test_problem(&draft, source, Vec::new(), Some("F"));
            problem.arm = arm;
            problem.symbol = Some("F".into());
            let job = Job::new(&problem, &work.path().join("job")).unwrap();
            job.compile("extern unsigned char input; int F(void) { return input; }\n").unwrap();
            problem.listing = crate::diff::disassemble(&job.object, Some("F")).unwrap();
            problem.listing_rows = Some(score::disassemble(&job.object, Some("F")).unwrap());
            let mut expanded = perm::expand(&problem.nodes, None).unwrap();
            prepare(&job, &mut expanded, "F").unwrap();
            let base_state = problem.state(expanded);
            let base_text = problem.source(&base_state);
            let base = Found { score: problem.evaluate(&job, &base_text).unwrap(), unnatural: 0, text: base_text, steps: vec![], expanded: base_state.expanded.clone(), job: 0, iteration: 0 };
            assert!(base.score.differ > 0 && base.score.weighted > 0, "arm={arm}: {}\nsource: {}\nreference: {:?}\ncandidate: {:?}", base.score.summary_with(false), base.text, problem.listing, crate::diff::disassemble(&job.object, Some("F")).unwrap());
            let found = (0..80).find_map(|seed| {
                let mut state = base_state.clone();
                let kind = randomize::Kind::GlobalType;
                if !randomize::apply(state.full.as_mut().unwrap(), "F", kind, &mut Rng::new(seed)) { return None; }
                let text = problem.source(&state);
                let score = problem.evaluate(&job, &text).ok()?;
                if score.differ != 0 { return None; }
                Some(Found { score, unnatural: 0, text, steps: vec![Step { kind: Kind::Full(kind), rng: Rng::new(seed), region: None }], expanded: state.expanded, job: 0, iteration: seed })
            }).expect("changing a global type reaches the byte load the old body engine cannot generate");
            let minimized = minimize(&problem, &job, &base, &found, &mut HashMap::new());
            assert_eq!(minimized.steps.len(), 1);
            let replay = problem.replay(&minimized.expanded, &minimized.steps);
            assert_eq!(problem.source(&replay), minimized.text);
            assert_eq!(problem.evaluate(&job, &minimized.text).unwrap().differ, 0);
            let (text, _) = respell(&problem, &job, &minimized, &mut HashMap::new());
            assert_eq!(problem.evaluate(&job, &text).unwrap().differ, 0);
        }
    }

    #[test]
    fn whole_tree_manual_expansions_survive_preprocessing_and_keep_separate_orders() {
        crate::compiler::routing::prefer_installed_binutils();
        let source = "int F(int x) { PERM_PRETEND(typedef int T;) PERM_LINESWAP(x += 1;\nx *= 2;) PERM_RANDOMIZE(x += 3;) PERM_IGNORE(asm(\"\");) return x; }\n";
        let work = tempfile::tempdir().unwrap();
        let problem = test_problem(&work.path().join("draft/08000000.c"), source, vec![], Some("F"));
        let job = Job::new(&problem, &work.path().join("job")).unwrap();
        let mut first = perm::expand_seed(&problem.nodes, 0).unwrap();
        let mut second = perm::expand_seed(&problem.nodes, 1).unwrap();
        assert_eq!(first.text, second.text);
        assert_ne!(first.key(), second.key());
        for expansion in [&mut first, &mut second] {
            prepare(&job, expansion, "F").unwrap();
            let tree = expansion.unit.as_deref().unwrap();
            assert!(tree.print().contains("typedef int T;") && tree.print().contains("#pragma _permuter randomizer start"));
            let source = expansion.restore(&tree.print());
            assert!(!source.contains("typedef int T;") && !source.contains("_permuter"), "{source}");
            assert!(source.contains("asm(\"\");"));
            job.compile(&source).unwrap();
            let c::Node::FileAst { ext } = tree else { panic!() };
            let c::Node::FuncDef { body, .. } = &ext[0] else { panic!() };
            let c::Node::Compound { block_items } = body.as_ref() else { panic!() };
            assert!(!block_items.iter().any(|n| matches!(n, c::Node::Compound { .. })), "line swap must not introduce a declaration scope");
        }
        assert_ne!(first.unit, second.unit);
    }

    #[test]
    fn whole_tree_outputs_keep_includes_and_comments_when_only_the_function_changes() {
        crate::compiler::routing::prefer_installed_binutils();
        let source = "#include \"types.h\"\n/* Keep this explanation. */\nint F(int a, int b) { return a + b; }\n";
        let work = tempfile::tempdir().unwrap();
        let mut problem = test_problem(&work.path().join("draft/08000000.c"), source, vec![], Some("F"));
        problem.symbol = Some("F".into());
        let job = Job::new(&problem, &work.path().join("job")).unwrap();
        let mut expanded = perm::expand(&problem.nodes, None).unwrap();
        prepare(&job, &mut expanded, "F").unwrap();
        let mut state = problem.state(expanded);
        let kind = randomize::Kind::Commutative;
        assert!(randomize::apply(state.full.as_mut().unwrap(), "F", kind, &mut Rng::new(0)));
        state.steps.push(Step { kind: Kind::Full(kind), rng: Rng::new(0), region: None });
        let text = problem.source(&state);
        let found = Found { score: problem.evaluate(&job, &text).unwrap(), unnatural: 0, text, steps: state.steps, expanded: state.expanded, job: 0, iteration: 1 };
        let (output, _) = respell(&problem, &job, &found, &mut HashMap::new());
        assert!(output.contains("#include \"types.h\"") && output.contains("/* Keep this explanation. */") && output.contains("b + a"), "{output}");
        assert!(!output.contains("typedef"), "output should not inline all headers");
    }

    #[test]
    fn minimizing_drops_rewrites_the_score_does_not_need() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        // The listing stores gY first; the draft stores gX first.
        let listing = listing_of(work.path(), "int gX;\nint gY;\nvoid Set(int a, int b)\n{\n    gY = b;\n    gX = a;\n}\n");
        let draft = work.path().join("draft/08000000.c");
        std::fs::create_dir_all(draft.parent().unwrap()).unwrap();
        let text = "int gX;\nint gY;\nvoid Set(int a, int b)\n{\n    gX = a;\n    gY = b;\n}\n";
        std::fs::write(&draft, text).unwrap();
        let problem = test_problem(&draft, text, listing, None);
        let job = Job::new(&problem, &work.path().join("run/minimize")).unwrap();
        let base_state = problem.state(perm::expand(&problem.nodes, None).unwrap());
        let base_text = problem.source(&base_state);
        let mut cache = HashMap::new();
        let base_score = scored(&problem, &job, &base_text, &mut cache).unwrap();
        assert!(base_score.weighted > 0);
        let base = Found { score: base_score, unnatural: 0, text: base_text, steps: Vec::new(), expanded: base_state.expanded.clone(), job: 0, iteration: 0 };
        // A long walk under a fixed seed that ends with gY stored first.
        let walk = (1..500u64)
            .find_map(|seed| {
                let mut state = base_state.clone();
                let (_, _, function) = state.parsed.as_mut().unwrap();
                let mut env = Env::new(&problem.unit, function);
                let mut rng = Rng::new(seed);
                for _ in 0..8 {
                    env.refresh(function);
                    if let Some(step) = mutate(function, &mut env, &mut rng) {
                        state.steps.push(step);
                    }
                }
                let text = problem.source(&state);
                (state.steps.len() >= 6 && text.find("gY =") < text.find("gX =")).then_some((state, text))
            })
            .expect("a walk that reorders the stores");
        let (state, walked) = walk;
        let score = scored(&problem, &job, &walked, &mut cache).unwrap();
        let found = Found { score, unnatural: problem.unnatural(&state), text: walked, steps: state.steps.clone(), expanded: state.expanded.clone(), job: 0, iteration: 1 };
        let minimized = minimize(&problem, &job, &base, &found, &mut cache);
        assert!(minimized.steps.len() < found.steps.len(), "{} of {} rewrites remain", minimized.steps.len(), found.steps.len());
        assert!((minimized.score.weighted, minimized.score.differ) <= (found.score.weighted, found.score.differ));
        // The minimized trail replays to its text, and that text scores as recorded.
        assert_eq!(problem.source(&problem.replay(&minimized.expanded, &minimized.steps)), minimized.text);
        assert_eq!(problem.evaluate(&job, &minimized.text).unwrap(), minimized.score);
        // Every remaining rewrite is needed: dropping any one raises the score.
        for index in 0..minimized.steps.len() {
            let mut fewer = minimized.steps.clone();
            fewer.remove(index);
            let state = problem.replay(&minimized.expanded, &fewer);
            let text = problem.source(&state);
            assert_ne!(text, minimized.text, "rewrite {index} changes nothing");
            if let Ok(score) = problem.evaluate(&job, &text) {
                let measure = (score.weighted, score.differ, problem.unnatural(&state));
                assert!(measure > (minimized.score.weighted, minimized.score.differ, minimized.unnatural), "rewrite {index} is not needed");
            }
        }
        // The same minimization twice is the same.
        assert_eq!(minimize(&problem, &job, &base, &found, &mut cache).text, minimized.text);
    }

    #[test]
    fn failed_runs_leave_no_scratch() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let listing = listing_of(work.path(), "int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return a - b;\n}\n");
        let options = |draft: &Path, function: Option<&str>| Options {
            draft: draft.to_path_buf(),
            addresses: Vec::new(),
            game: "tbs",
            target: decomp_target(Some("tbs-en")).unwrap(),
            minutes: 0.0,
            jobs: 1,
            seed: 1,
            rng_seed: None,
            perm_seed: None,
            function: function.map(str::to_string),
            arm: false,
            iterations: Some(2),
            heat: false,
            prefix: false,
            score_options: score::Options::default(),
            keep_prob: 0.6,
            stop_on_zero: false,
        };
        let draft = work.path().join("draft/08000000.c");
        std::fs::create_dir_all(draft.parent().unwrap()).unwrap();
        let output = work.path().join("out/permute/08000000");
        let cases = [
            ("int gX;\nint Sub(int a, int b)\n{\n    gX = a\n    return a - b;\n}\n", None),
            ("int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return a - b;\n}\n", Some("Missing")),
        ];
        for (text, function) in cases {
            std::fs::write(&draft, text).unwrap();
            let problem = test_problem(&draft, text, listing.clone(), function);
            assert!(execute(problem, &options(&draft, function), None, &output).is_err());
            assert!(!output.exists(), "{:?}", std::fs::read_dir(&output).map(|d| d.flatten().map(|e| e.file_name()).collect::<Vec<_>>()));
        }
        // A killed run's scratch goes at the next run; a live one's stays.
        let mut child = std::process::Command::new("true").spawn().unwrap();
        child.wait().unwrap();
        let (dead, live) = (output.join(format!("run-{}", child.id())), output.join(format!("run-{}", std::process::id())));
        for directory in [&dead, &live] {
            std::fs::create_dir_all(directory.join("job-00")).unwrap();
        }
        sweep(&output);
        assert!(!dead.exists() && live.exists());
        std::fs::remove_dir_all(&output).unwrap();
        // A panic unwinding through a run removes its scratch too.
        let scratch = work.path().join("out/permute/x/run-1");
        std::fs::create_dir_all(scratch.join("job-00")).unwrap();
        let caught = std::panic::catch_unwind(|| {
            let _guard = Scratch(scratch.clone());
            panic!("search failed");
        });
        assert!(caught.is_err() && !scratch.exists() && !scratch.parent().unwrap().exists());
        // A finished run keeps its results beside the removed scratch.
        let text = "int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return PERM_ONEOF(b - a, a - b);\n}\n";
        std::fs::write(&draft, text).unwrap();
        let problem = test_problem(&draft, text, listing, None);
        execute(problem, &options(&draft, None), None, &output).unwrap();
        let names: Vec<String> = std::fs::read_dir(&output).unwrap().flatten().map(|entry| entry.file_name().to_string_lossy().into_owned()).collect();
        assert!(names.iter().all(|name| name.starts_with("best-")) && names.contains(&"best-1.diff".to_string()), "{names:?}");
        // PERM choices only: the diff resolves the macro in the draft's spelling.
        let diff = std::fs::read_to_string(output.join("best-1.diff")).unwrap();
        assert!(diff.contains("-    return PERM_ONEOF(b - a, a - b);\n+    return a - b;\n"), "{diff}");
        assert_eq!(apply_unified(text, &diff), "int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return a - b;\n}\n");
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
            listing_rows: None,
            score_options: score::Options::default(),
            symbol: None,
            prefix: false,
            aim: None,
            unit: scan_unit(draft).unwrap_or_default(),
            function: None,
            arm: false,
        };
        let state = problem.state(perm::expand(&problem.nodes, None).unwrap());
        assert!(state.parsed.is_some());
        let mut rng = Rng::new(4);
        let mut texts = std::collections::BTreeSet::new();
        let (_, _, base) = state.parsed.clone().unwrap();
        let mut env = Env::new(&problem.unit, &base);
        for _ in 0..60 {
            let mut state = problem.state(perm::expand(&problem.nodes, Some(&mut rng)).unwrap());
            let (_, _, function) = state.parsed.as_mut().unwrap();
            env.refresh(function);
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
    fn randomized_regions_use_identity_and_survive_repeated_rewrites() {
        let text = "void F(int a, int b) { gX = a + b; PERM_RANDOMIZE(gX = a + b;) gX = a + b; }";
        let problem = test_problem(Path::new("08000000.c"), text, Vec::new(), None);
        for seed in 0..40 {
            let mut state = problem.state(perm::expand(&problem.nodes, None).unwrap());
            let (_, _, function) = state.parsed.as_mut().unwrap();
            let env = Env::new(&problem.unit, function);
            let before = function.body[0].clone();
            let after = function.body.last().unwrap().clone();
            for _ in 0..4 {
                assert!(mutate::apply(Kind::SwapOperands, function, &env, &mut Rng::new(seed)));
                assert_eq!(function.body[0], before);
                assert_eq!(function.body.last().unwrap(), &after);
            }
        }
    }

    #[test]
    fn search_reaches_differ_zero_through_diffs_own_compile() {
        crate::compiler::routing::prefer_installed_binutils();
        let work = tempfile::tempdir().unwrap();
        let target = decomp_target(Some("tbs-en")).unwrap();
        // The listing: one spelling, compiled as alchemy diff compiles drafts.
        let reference = work.path().join("reference/08000000.c");
        let object = work.path().join("reference/obj/08000000.o");
        std::fs::create_dir_all(object.parent().unwrap()).unwrap();
        std::fs::write(&reference, "int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return a - b;\n}\n").unwrap();
        for step in crate::diff::draft_steps(target, &reference, &object).unwrap() {
            crate::process::run(&step, root()).unwrap();
        }
        let listing = crate::diff::disassemble(&object, None).unwrap();
        // The draft offers that spelling only as its second choice.
        let draft = work.path().join("draft/08000000.c");
        std::fs::create_dir_all(draft.parent().unwrap()).unwrap();
        let text = "int gX;\nint Sub(int a, int b)\n{\n    gX = a;\n    return PERM_ONEOF(b - a, a - b);\n}\n";
        std::fs::write(&draft, text).unwrap();
        let nodes = perm::parse(text).unwrap();
        let problem = Problem {
            draft: draft.clone(),
            text: text.into(),
            stem: "08000000".into(),
            target,
            choices: perm::has_choices(&nodes),
            nodes,
            listing,
            listing_rows: None,
            score_options: score::Options::default(),
            symbol: None,
            prefix: false,
            aim: None,
            unit: Unit::default(),
            function: None,
            arm: false,
        };
        let base_state = problem.state(perm::expand(&problem.nodes, None).unwrap());
        let setup = Job::new(&problem, &work.path().join("run/setup")).unwrap();
        let base_text = problem.source(&base_state);
        let score = problem.evaluate(&setup, &base_text).unwrap();
        assert!(score.weighted > 0 && score.differ > 0, "{score:?}");
        // diff's own comparison of the same objects gives the same differ.
        let compiled = crate::diff::disassemble(&setup.object, None).unwrap();
        let (common, _) = crate::diff::compare(&problem.listing, &compiled);
        assert_eq!(score.differ, problem.listing.len() + compiled.len() - 2 * common);
        let base = Found { score, unnatural: 0, text: base_text, steps: Vec::new(), expanded: base_state.expanded.clone(), job: 0, iteration: 0 };
        let options = Options {
            draft,
            addresses: Vec::new(),
            game: "tbs",
            target,
            minutes: 0.0,
            jobs: 2,
            seed: 1,
            rng_seed: None,
            perm_seed: None,
            function: None,
            arm: false,
            iterations: Some(8),
            heat: false,
            prefix: false,
            score_options: score::Options::default(),
            keep_prob: 0.6,
            stop_on_zero: false,
        };
        let found = search(&problem, &base_state, &base, false, &options, None, &work.path().join("run"), Instant::now()).unwrap();
        assert_eq!((found[0].score.weighted, found[0].score.differ), (0, 0));
        assert!(found[0].text.contains("return a - b;") && !perm::has_macros(&found[0].text));
        // nonrandom sources are evaluated only once, despite the limit of 8.
        let args = [options.draft.to_str().unwrap(), "--seed", "1,-99999999999999999999", "--jobs", "1", "--iterations", "8"].map(str::to_string);
        let forced = parse_options(&args).unwrap();
        let found = search(&problem, &base_state, &base, false, &forced, None, &work.path().join("forced"), Instant::now()).unwrap();
        assert_eq!((found[0].score.weighted, found[0].iteration), (0, 1));
    }

    #[test]
    fn rewrites_count_kinds_and_name_perm_only_runs() {
        assert_eq!(rewrites(&[]), "PERM choices only");
    }
}
