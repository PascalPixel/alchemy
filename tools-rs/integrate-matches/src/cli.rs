//! `parseArguments` and `usage`.

pub const USAGE: &str = "usage: integrate_matches.ts [-h] [--apply] directory | --self-test";

/// `interface Options`.
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    pub directory: String,
    pub apply: bool,
    pub self_test: bool,
}

/// What `parseArguments` did.
///
/// PORT NOTE -- `-h` prints usage and calls `process.exit(0)` from INSIDE the
/// loop, so `integrate_matches.ts --apply -h --bogus` exits 0 and never reports
/// the unrecognized `--bogus`. `Help` reproduces that early exit; a port that
/// finished the loop first would turn a success into a failure.
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ParseOutcome {
    Help,
    Parsed(Options),
}

/// `parseArguments(Bun.argv.slice(2))`.
pub fn parse_arguments(arguments: &[String]) -> Result<ParseOutcome, String> {
    let mut directory: Option<String> = None;
    let mut apply = false;
    let mut self_test = false;
    for argument in arguments {
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        }
        if argument == "--apply" {
            apply = true;
            continue;
        }
        if argument == "--self-test" {
            self_test = true;
            continue;
        }
        // `argument.startsWith("-")`, so a bare `-` is an unrecognized flag and
        // never a positional directory, and a directory named `-x` is
        // unreachable. Both reproduced.
        if argument.starts_with('-') {
            return Err(format!("unrecognized argument: {argument}"));
        }
        if directory.is_some() {
            return Err(format!("unrecognized argument: {argument}"));
        }
        directory = Some(argument.clone());
    }
    if self_test {
        // PORT NOTE -- the length test is `arguments_.length !== 1`, so
        // `--self-test --self-test` is rejected even though it sets nothing
        // else. Reproduced.
        if directory.is_some() || apply || arguments.len() != 1 {
            return Err("--self-test takes no other arguments".to_string());
        }
        return Ok(ParseOutcome::Parsed(Options {
            directory: String::new(),
            apply: false,
            self_test: true,
        }));
    }
    let Some(directory) = directory else {
        return Err("the following arguments are required: directory".to_string());
    };
    Ok(ParseOutcome::Parsed(Options { directory, apply, self_test: false }))
}
