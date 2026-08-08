//! `parseOptions(argv)` and the usage text.

use crate::config::compiler_configuration_of;
use crate::corpus::{hexadecimal_stem, join};
use crate::jsparse::{is_js_integer, parse_int};
use crate::jsvalue::parse;
use alchemy_plan::plan::CompilerFamily;
use candidate_compiler::verify::CandidateCompilerConfiguration;

/// `interface Options`.
///
/// PORT NOTE -- `sources: Set<string>` is an INSERTION-ORDERED set in
/// JavaScript, and the order is observable: the report writes
/// `[...options.sources].sort()`. A `HashSet` is banned outright; a `Vec` with
/// an explicit membership test is used instead. `Set` also SILENTLY DEDUPES,
/// which for a corpus tool means a repeated `--sources a,a` shrinks nothing
/// but a repeated stem across two `--sources` flags must not double-count.
/// The `Vec` reproduces both.
#[derive(Debug, Clone)]
pub struct Options {
    pub flags: Vec<String>,
    pub config_path: Option<String>,
    pub compiler_config: CandidateCompilerConfiguration,
    pub rom: String,
    pub manifest: String,
    pub cache: String,
    pub sample: usize,
    pub seed: String,
    pub jobs: usize,
    pub family: Option<String>,
    pub near: Option<f64>,
    pub radius: f64,
    pub sources: Vec<String>,
    pub report: Option<String>,
}

pub const USAGE: &str = concat!(
    "usage: compiler_corpus_regression.ts (--flags FLAG[,FLAG...] | --config FILE) [options]\n",
    "  --config FILE       CandidateCompilerConfiguration JSON:\n",
    "                      {\"family\":\"routed|gcc296|old-agbcc|pret-early-thumb|gcc2951|gcc3\",\n",
    "                       \"addFlags\":[...],\"removeFlags\":[...]}\n",
    "  --flags FLAGS       compatibility shorthand for additional flags\n",
    "  --sample N          deterministic sample size; 0 selects all (default 64)\n",
    "  --seed TEXT         deterministic sampling seed\n",
    "  --family PREFIX     restrict source stems to an address prefix\n",
    "  --near ADDRESS      restrict to address neighbors\n",
    "  --radius BYTES      neighbor radius (default 0x10000)\n",
    "  --sources A,B       restrict to explicit source stems\n",
    "  --jobs N            parallel compiler jobs (default 4)\n",
    "  --report FILE       also write the canonical JSON report",
);

/// `parseOptions` either produces options or asks the caller to print usage and
/// exit 0.
#[derive(Debug)]
pub enum ParseOutcome {
    Options(Box<Options>),
    Help,
}

fn defaults() -> Options {
    let root = alchemy_routing::routing::root();
    Options {
        flags: Vec::new(),
        config_path: None,
        compiler_config: CandidateCompilerConfiguration {
            family: Some(CompilerFamily::Routed),
            add_flags: Vec::new(),
            remove_flags: Vec::new(),
        },
        rom: join(root, "roms/gs1-en.gba"),
        manifest: join(root, "out/full/claimed/manifest.json"),
        cache: join(root, "out/compiler-corpus-regression"),
        sample: 64,
        seed: "exact-c-corpus-v1".to_string(),
        jobs: 4,
        family: None,
        near: None,
        radius: 0x10000 as f64,
        sources: Vec::new(),
        report: None,
    }
}

/// `parseOptions(argv)`.
///
/// PORT NOTE -- `sample`, `jobs` and `radius` are `Number.parseInt` results and
/// are validated with `Number.isInteger`, which is FALSE for NaN. Every one of
/// those parses is kept as `Option<f64>` until after its guard, so a
/// non-numeric `--sample x` produces the TypeScript's "--sample must be >= 0"
/// and not a Rust parse error with different prose.
///
/// PORT NOTE -- `--radius` uses `Number.parseInt(text, 0)`, the AUTO radix:
/// `0x10000` is 65,536 and `10000` is ten thousand. The other two use radix 10,
/// where `0x10` is 0 (the `0` parses, `x` stops the scan). All three go
/// through the one `parse_int` helper.
pub fn parse_arguments(argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = defaults();
    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].clone();
        // PORT NOTE -- the TypeScript writes `argv[++index]`, which is
        // `undefined` past the end and would flow into `String(undefined)`.
        // A missing value is reported here instead of silently parsing the
        // string "undefined"; see the hard-fail note in the report.
        macro_rules! take {
            () => {
                match argv.get(index + 1) {
                    Some(value) => {
                        index += 1;
                        value.clone()
                    }
                    None => return Err(format!("missing value after {argument}")),
                }
            };
        }
        match argument.as_str() {
            "--flags" => {
                // `take().split(",").filter(Boolean)` -- `filter(Boolean)` drops
                // the empty strings that `",".split(",")` produces, so
                // `--flags ,,-O2,` contributes one flag.
                for flag in take!().split(',').filter(|part| !part.is_empty()) {
                    options.flags.push(flag.to_string());
                }
            }
            "--config" => options.config_path = Some(take!()),
            "--rom" => options.rom = take!(),
            "--manifest" => options.manifest = take!(),
            "--cache" => options.cache = take!(),
            "--sample" => {
                let value = parse_int(&take!(), 10);
                if !is_js_integer(value) || value.is_some_and(|v| v < 0.0) {
                    return Err("--sample must be >= 0".to_string());
                }
                options.sample = value.unwrap_or(0.0) as usize;
            }
            "--seed" => options.seed = take!(),
            "--jobs" => {
                let value = parse_int(&take!(), 10);
                if !is_js_integer(value)
                    || value.is_some_and(|v| !(1.0..=32.0).contains(&v))
                {
                    return Err("--jobs must be between 1 and 32".to_string());
                }
                options.jobs = value.unwrap_or(0.0) as usize;
            }
            // `take().toLowerCase()` -- full Unicode lowering, not ASCII.
            "--family" => options.family = Some(take!().to_lowercase()),
            "--near" => {
                let stem = hexadecimal_stem(&take!())?;
                options.near = parse_int(&stem, 16);
            }
            "--radius" => {
                let value = parse_int(&take!(), 0);
                if !is_js_integer(value) || value.is_some_and(|v| v < 0.0) {
                    return Err("--radius must be >= 0".to_string());
                }
                options.radius = value.unwrap_or(0.0);
            }
            "--sources" => {
                for source in take!().split(',').filter(|part| !part.is_empty()) {
                    let stem = hexadecimal_stem(source)?;
                    if !options.sources.iter().any(|seen| seen == &stem) {
                        options.sources.push(stem);
                    }
                }
            }
            "--report" => options.report = Some(take!()),
            "-h" | "--help" => return Ok(ParseOutcome::Help),
            other => return Err(format!("unknown argument: {other}")),
        }
        index += 1;
    }

    if let Some(path) = &options.config_path {
        let text = std::fs::read_to_string(path).map_err(|error| format!("{path}: {error}"))?;
        options.compiler_config = compiler_configuration_of(&parse(&text)?)?;
    }
    // `options.flags = [...new Set(options.flags)]` -- insertion-ordered
    // dedupe, applied AFTER every `--flags` has been accumulated, so a flag
    // repeated across two `--flags` occurrences collapses to its FIRST
    // position. gcc is later-flag-wins, so the position is behaviour.
    let mut deduped: Vec<String> = Vec::new();
    for flag in &options.flags {
        if !deduped.iter().any(|seen| seen == flag) {
            deduped.push(flag.clone());
        }
    }
    options.flags = deduped;

    let unchanged = options.config_path.is_none()
        && options.flags.is_empty()
        && options.compiler_config.family == Some(CompilerFamily::Routed)
        && options.compiler_config.add_flags.is_empty()
        && options.compiler_config.remove_flags.is_empty();
    if unchanged {
        return Err(
            "provide --flags or a compiler configuration that changes the routed baseline"
                .to_string(),
        );
    }
    Ok(ParseOutcome::Options(Box::new(options)))
}
