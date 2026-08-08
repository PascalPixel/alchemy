//! `parseArguments` and `usage`.

use decomp_targets::{parse_decomp_target, DecompTargetId, DEFAULT_TARGET};

#[derive(Debug, Clone)]
pub struct Options {
    pub target: DecompTargetId,
    pub rom: String,
    pub drafts: String,
    pub jobs: usize,
}

pub const USAGE: &str =
    "usage: match_m2c.ts [-h] [--target gs1-en|gs2-en] [--jobs JOBS] rom drafts";

/// The `-h`/`--help` path, which prints usage and exits 0 before any other
/// argument is examined -- including an invalid one later in the same argv.
#[derive(Debug)]
pub enum ParseOutcome {
    Help,
    Options(Box<Options>),
}

/// `navigator.hardwareConcurrency || 1`, then `Math.min(16, …)`.
///
/// PORT NOTE -- `||` treats 0 as falsy, so a reported concurrency of 0 becomes
/// one. `available_parallelism` returns a `NonZero`, so the fallback is only
/// reachable when the query itself fails.
pub fn default_jobs() -> usize {
    let concurrency = std::thread::available_parallelism()
        .map(|n| n.get())
        .unwrap_or(1);
    std::cmp::min(16, concurrency)
}

/// `parseArguments(Bun.argv.slice(2))`.
///
/// PORT NOTE -- `--jobs` uses `Number.parseInt(value, 10)` and then
/// `Number.isInteger`. `parseInt` stops at the first non-digit, so `"8x"` is 8
/// and passes, `"1.5"` is 1 and passes, and `""` / `"x"` are NaN and fail. A
/// Rust `str::parse::<usize>()` would reject `"8x"` and `"1.5"`, which is a
/// different program. `parse_int_radix_10` reproduces the JavaScript.
///
/// PORT NOTE -- `--jobs` with no following argument reads `arguments_[++index]`
/// as `undefined` and throws; but the `++` has already advanced past the end,
/// so the loop terminates. Reproduced by the explicit bounds check.
pub fn parse_arguments(arguments: &[String]) -> Result<ParseOutcome, String> {
    let mut positional: Vec<String> = Vec::new();
    let mut jobs = default_jobs() as f64;
    let mut target = DEFAULT_TARGET;
    let mut index = 0usize;
    while index < arguments.len() {
        let argument = &arguments[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        }
        if argument == "--jobs" || argument.starts_with("--jobs=") {
            let value = match inline_value(argument, arguments, &mut index) {
                Some(value) => value,
                None => return Err("argument --jobs: expected one argument".to_string()),
            };
            jobs = parse_int_radix_10(&value);
            // `!Number.isInteger(jobs) || jobs < 1`. NaN fails the first test.
            if jobs.is_nan() || jobs.fract() != 0.0 || !jobs.is_finite() || jobs < 1.0 {
                return Err(format!("invalid int value: {value}"));
            }
            index += 1;
            continue;
        }
        if argument == "--target" || argument.starts_with("--target=") {
            let value = match inline_value(argument, arguments, &mut index) {
                Some(value) => value,
                None => return Err("argument --target: expected one argument".to_string()),
            };
            target = parse_decomp_target(&value)?;
            index += 1;
            continue;
        }
        if argument.starts_with('-') {
            return Err(format!("unrecognized argument: {argument}"));
        }
        positional.push(argument.clone());
        index += 1;
    }
    if positional.len() != 2 {
        return Err("the following arguments are required: rom, drafts".to_string());
    }
    Ok(ParseOutcome::Options(Box::new(Options {
        target,
        rom: positional[0].clone(),
        drafts: positional[1].clone(),
        jobs: jobs as usize,
    })))
}

/// `argument.includes("=") ? argument.slice(argument.indexOf("=") + 1) : arguments_[++index]`.
///
/// PORT NOTE -- the test is `includes("=")` on the WHOLE argument and the slice
/// starts after the FIRST `=`, so `--jobs=4=5` yields `"4=5"`. It also means a
/// bare `--jobs` can never take the inline branch, and a `--jobs=` yields the
/// empty string, which `parseInt` turns into NaN and the caller rejects.
fn inline_value(argument: &str, arguments: &[String], index: &mut usize) -> Option<String> {
    if let Some(position) = argument.find('=') {
        return Some(argument[position + 1..].to_string());
    }
    *index += 1;
    arguments.get(*index).cloned()
}

/// `Number.parseInt(value, 10)`.
///
/// PORT NOTE -- leading JavaScript whitespace is skipped, an optional sign is
/// consumed, then digits are taken until the first non-digit and the rest is
/// DISCARDED rather than being an error. No digits at all is NaN.
pub fn parse_int_radix_10(value: &str) -> f64 {
    let text: Vec<char> = value.chars().collect();
    let mut position = 0usize;
    while position < text.len() && crate::jsstring::is_js_space(text[position]) {
        position += 1;
    }
    let mut sign = 1.0f64;
    if position < text.len() && (text[position] == '+' || text[position] == '-') {
        if text[position] == '-' {
            sign = -1.0;
        }
        position += 1;
    }
    let start = position;
    let mut accumulator = 0.0f64;
    while position < text.len() && text[position].is_ascii_digit() {
        accumulator = accumulator * 10.0 + f64::from(text[position] as u32 - u32::from(b'0'));
        position += 1;
    }
    if position == start {
        return f64::NAN;
    }
    sign * accumulator
}
