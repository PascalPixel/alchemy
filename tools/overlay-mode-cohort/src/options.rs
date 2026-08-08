//! `optionsOf(argv)` and the help text.

use crate::jsops::{js_is_integer, js_parse_int};
use crate::paths::root;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Scope {
    Families,
    Singles,
}

/// PORT NOTE: every numeric field is `f64`, not `usize`. `Number.parseInt`
/// returns NaN for `--jobs banana`, and the guards below are written as
/// `!Number.isInteger(x) || x < 1`, which NaN fails on the FIRST clause. A
/// `usize` port would have had to invent a parse error with different text.
#[derive(Debug, Clone)]
pub struct Options {
    pub candidates: Vec<String>,
    pub inventory: String,
    pub output: String,
    pub scope: Scope,
    pub pairs: f64,
    pub triple_seeds: Vec<String>,
    pub triples: f64,
    pub jobs: f64,
    pub top: f64,
}

pub const HELP: &str = concat!(
    "usage: overlay_mode_cohort.ts --candidate OVERLAY:OFFSET=PATH [--candidate ...] [options]\n",
    "  --candidate, -c SPEC  target and existing C path, e.g. resource_394:07e0=semantic/overlays/resource_394_c_020007e0.c\n",
    "  --inventory FILE      overlay inventory containing measured owner spans (default out/decomp/overlays.json)\n",
    "  --scope families|singles  compiler families only (default) or every exposed one-mode configuration\n",
    "  --pairs N             additionally test at most N deterministic compatible pairs (default 0)\n",
    "  --triple-seeds IDS    comma-separated mode ids with pair/single evidence; enables evidence-gated triples\n",
    "  --triples N           cap on seeded triples (default 0 = all seeded triples when seeds given)\n",
    "  --jobs N              concurrent compiler jobs (default min(12, CPUs-2))\n",
    "  --output, -o DIR      ignored cache and report directory (default out/overlay-mode-cohort)\n",
    "  --top N               per-region and aggregate rows to print (default 12)\n",
    "  --self-test           run parser, scoring, and cache-key checks without compiling",
);

/// `navigator.hardwareConcurrency || 8`.
///
/// PORT NOTE: `||`, not `??`, so a reported concurrency of 0 falls back to 8.
fn hardware_concurrency() -> f64 {
    let reported = std::thread::available_parallelism()
        .map(|value| value.get() as f64)
        .unwrap_or(0.0);
    if reported == 0.0 {
        8.0
    } else {
        reported
    }
}

/// The default `jobs`, reproduced exactly.
///
/// PORT NOTE: this tool caps itself at 12 rather than at the repository's usual
/// 80%-of-cores rule. That cap is the TypeScript's, and changing it here would
/// change how many compiler processes run concurrently relative to the mirror,
/// which is the one thing a benchmark comparison cannot tolerate.
pub fn default_jobs() -> f64 {
    js_max(1.0, js_min(12.0, hardware_concurrency() - 2.0))
}

/// `Math.min(a, b)` and `Math.max(a, b)`.
///
/// PORT NOTE -- NOT `f64::clamp`, WHICH CLIPPY WILL SUGGEST. `clamp` PANICS if
/// either bound is NaN and RETURNS NaN for a NaN input; `Math.max` propagates
/// NaN while Rust's `f64::max` swallows it. Writing the two operations out
/// separately keeps the JavaScript order of evaluation (`min` first, then
/// `max`) visible, which is the order that decides the result when the bounds
/// cross.
fn js_min(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left < right {
        left
    } else {
        right
    }
}

fn js_max(left: f64, right: f64) -> f64 {
    if left.is_nan() || right.is_nan() {
        return f64::NAN;
    }
    if left > right {
        left
    } else {
        right
    }
}

/// The outcome of parsing. `Help` is `process.exit(0)` after printing.
pub enum Parsed {
    Options(Box<Options>),
    Help,
}

pub fn options_of(argv: &[String]) -> Result<Parsed, String> {
    let root = root();
    let mut options = Options {
        candidates: Vec::new(),
        inventory: root.join("out/decomp/overlays.json").to_string_lossy().into_owned(),
        output: root.join("out/overlay-mode-cohort").to_string_lossy().into_owned(),
        scope: Scope::Families,
        pairs: 0.0,
        triple_seeds: Vec::new(),
        triples: 0.0,
        jobs: default_jobs(),
        top: 12.0,
    };
    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].clone();
        // `take()` increments the shared cursor, so an option that consumes a
        // value skips it on the next iteration. Reproduced with the same shared
        // index rather than a lookahead, because `--candidate --inventory x`
        // must consume `--inventory` as the candidate value, not treat it as a
        // flag.
        let flag = argument.clone();
        let take = |index: &mut usize| -> Result<String, String> {
            *index += 1;
            match argv.get(*index) {
                Some(value) => Ok(value.clone()),
                None => Err(format!("missing value after {flag}")),
            }
        };
        match argument.as_str() {
            "--candidate" | "-c" => options.candidates.push(take(&mut index)?),
            "--inventory" => options.inventory = take(&mut index)?,
            "--output" | "-o" => options.output = take(&mut index)?,
            "--scope" => {
                let scope = take(&mut index)?;
                options.scope = match scope.as_str() {
                    "families" => Scope::Families,
                    "singles" => Scope::Singles,
                    _ => return Err("--scope must be families or singles".into()),
                };
            }
            "--pairs" => options.pairs = js_parse_int(&take(&mut index)?, 10),
            "--triple-seeds" => {
                // `.split(",").filter(Boolean)` drops empty fields, so a
                // trailing comma and a doubled comma are both harmless. It does
                // NOT trim, so `" a"` stays `" a"` and simply matches no mode.
                options.triple_seeds = take(&mut index)?
                    .split(',')
                    .filter(|field| !field.is_empty())
                    .map(|field| field.to_string())
                    .collect();
            }
            "--triples" => options.triples = js_parse_int(&take(&mut index)?, 10),
            "--jobs" => options.jobs = js_parse_int(&take(&mut index)?, 10),
            "--top" => options.top = js_parse_int(&take(&mut index)?, 10),
            "-h" | "--help" => return Ok(Parsed::Help),
            other => return Err(format!("unknown argument: {other}")),
        }
        index += 1;
    }
    if options.candidates.is_empty() {
        return Err("at least one --candidate is required".into());
    }
    if !js_is_integer(options.pairs) || options.pairs < 0.0 {
        return Err("--pairs must be >= 0".into());
    }
    if !js_is_integer(options.triples) || options.triples < 0.0 {
        return Err("--triples must be >= 0".into());
    }
    if options.triples > 0.0 && options.triple_seeds.len() < 2 {
        return Err(
            "--triples needs --triple-seeds with at least two evidenced mode ids (tripleConfigs is evidence-gated)"
                .into(),
        );
    }
    if !js_is_integer(options.jobs) || options.jobs < 1.0 || options.jobs > 32.0 {
        return Err("--jobs must be 1..32".into());
    }
    if !js_is_integer(options.top) || options.top < 1.0 {
        return Err("--top must be positive".into());
    }
    Ok(Parsed::Options(Box::new(options)))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn parse(argv: &[&str]) -> Result<Options, String> {
        match options_of(&argv.iter().map(|a| a.to_string()).collect::<Vec<_>>())? {
            Parsed::Options(options) => Ok(*options),
            Parsed::Help => Err("help".into()),
        }
    }

    #[test]
    fn a_non_numeric_count_fails_the_integer_guard_not_a_parse_error() {
        // `Number.parseInt("banana", 10)` is NaN and `Number.isInteger(NaN)` is
        // false, so the message is the range message, not a parse message.
        assert_eq!(parse(&["-c", "a:0000=x.c", "--jobs", "banana"]).unwrap_err(), "--jobs must be 1..32");
        // `parseInt("3.9")` is 3, which IS an integer and IS accepted.
        assert_eq!(parse(&["-c", "a:0000=x.c", "--jobs", "3.9"]).unwrap().jobs, 3.0);
    }

    #[test]
    fn triples_require_two_seeds() {
        let error = parse(&["-c", "a:0000=x.c", "--triples", "2", "--triple-seeds", "one"]).unwrap_err();
        assert!(error.starts_with("--triples needs --triple-seeds"));
        // Seeds without `--triples` are fine: 0 means "all seeded triples".
        assert!(parse(&["-c", "a:0000=x.c", "--triple-seeds", "one,two"]).is_ok());
    }

    #[test]
    fn seed_split_drops_empty_fields_without_trimming() {
        let options = parse(&["-c", "a:0000=x.c", "--triple-seeds", "a,, b,"]).unwrap();
        assert_eq!(options.triple_seeds, vec!["a".to_string(), " b".to_string()]);
    }

    #[test]
    fn a_flag_shaped_value_is_still_consumed_as_a_value() {
        // The shared cursor means `take()` swallows the next argv element
        // whatever it looks like. `--inventory` becomes the candidate spec and
        // is rejected later by `candidate_spec`, NOT here as an unknown
        // argument. A lookahead port that refused `-`-prefixed values would
        // fail with different text.
        let options = parse(&["--candidate", "--inventory"]).unwrap();
        assert_eq!(options.candidates, vec!["--inventory".to_string()]);
        assert!(crate::spec::candidate_spec("--inventory").is_err());
    }

    #[test]
    fn help_short_circuits_before_the_required_candidate_check() {
        assert!(matches!(
            options_of(&["--help".to_string()]).unwrap(),
            Parsed::Help
        ));
    }

    #[test]
    fn a_missing_value_names_the_flag_it_followed() {
        assert_eq!(parse(&["-c"]).unwrap_err(), "missing value after -c");
    }

    #[test]
    fn default_jobs_stays_within_one_and_twelve() {
        let jobs = default_jobs();
        assert!((1.0..=12.0).contains(&jobs), "jobs was {jobs}");
    }
}
