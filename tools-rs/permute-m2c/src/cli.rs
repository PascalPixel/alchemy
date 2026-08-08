//! `parseArguments` and `usage`.

use crate::paths::root;

pub const USAGE: &str = "usage: permute_m2c.ts [-h] [--drafts DRAFTS] [--report REPORT] [rom]";

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    /// PORT NOTE: the default is the **relative** string `roms/gs1-en.gba`,
    /// resolved against the process working directory, while `drafts` and
    /// `report` default to absolute paths under the repository root. That
    /// asymmetry is in the TypeScript and is reproduced: running the tool from
    /// anywhere but the repository root fails to open the ROM.
    pub rom: String,
    pub drafts: String,
    pub report: String,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum ParseOutcome {
    /// `-h` / `--help`: print the usage line and exit 0.
    Help,
    Parsed(Options),
}

pub fn parse_arguments(arguments: &[String]) -> Result<ParseOutcome, String> {
    let mut rom = "roms/gs1-en.gba".to_string();
    let mut drafts = root().join("work/m2c-all").to_string_lossy().into_owned();
    let mut report = root()
        .join("work/matches/m2c.json")
        .to_string_lossy()
        .into_owned();
    let mut found_rom = false;

    let mut index = 0usize;
    while index < arguments.len() {
        let argument = &arguments[index];
        if argument == "-h" || argument == "--help" {
            return Ok(ParseOutcome::Help);
        }
        if argument == "--drafts" || argument == "--report" {
            // PORT NOTE: the value is consumed unconditionally, so
            // `--drafts --report x` sets `drafts` to the literal `--report`.
            index += 1;
            let Some(value) = arguments.get(index) else {
                return Err(format!("argument {argument}: expected one argument"));
            };
            if argument == "--drafts" {
                drafts = value.clone();
            } else {
                report = value.clone();
            }
            index += 1;
            continue;
        }
        if argument.starts_with("--drafts=") || argument.starts_with("--report=") {
            // `argument.slice(argument.indexOf("=") + 1)`: everything after the
            // FIRST `=`, so `--drafts=a=b` yields `a=b`.
            let value = argument
                .split_once('=')
                .map(|(_, tail)| tail.to_string())
                .unwrap_or_default();
            if argument.starts_with("--drafts=") {
                drafts = value;
            } else {
                report = value;
            }
            index += 1;
            continue;
        }
        if argument.starts_with('-') {
            return Err(format!("unrecognized argument: {argument}"));
        }
        if found_rom {
            return Err(format!("unrecognized argument: {argument}"));
        }
        rom = argument.clone();
        found_rom = true;
        index += 1;
    }
    Ok(ParseOutcome::Parsed(Options { rom, drafts, report }))
}

#[cfg(test)]
mod tests {
    use super::*;

    fn parse(arguments: &[&str]) -> Result<ParseOutcome, String> {
        let owned: Vec<String> = arguments.iter().map(|value| (*value).to_string()).collect();
        parse_arguments(&owned)
    }

    fn options(arguments: &[&str]) -> Options {
        match parse(arguments).expect("parses") {
            ParseOutcome::Parsed(options) => options,
            ParseOutcome::Help => panic!("expected options"),
        }
    }

    #[test]
    fn defaults_match_the_typescript() {
        let parsed = options(&[]);
        assert_eq!(parsed.rom, "roms/gs1-en.gba");
        assert!(parsed.drafts.ends_with("/work/m2c-all"));
        assert!(parsed.report.ends_with("/work/matches/m2c.json"));
    }

    #[test]
    fn help_short_circuits_before_any_other_argument_is_checked() {
        // `-h` wins even when a later argument would have thrown.
        assert_eq!(parse(&["-h", "--nope"]).expect("parses"), ParseOutcome::Help);
        assert_eq!(parse(&["--help"]).expect("parses"), ParseOutcome::Help);
    }

    #[test]
    fn a_flag_value_is_consumed_even_when_it_looks_like_a_flag() {
        let parsed = options(&["--drafts", "--report", "r"]);
        assert_eq!(parsed.drafts, "--report");
        assert_eq!(parsed.rom, "r");
    }

    #[test]
    fn equals_form_splits_on_the_first_equals() {
        assert_eq!(options(&["--drafts=a=b"]).drafts, "a=b");
        assert_eq!(options(&["--report="]).report, "");
    }

    #[test]
    fn positional_and_unknown_flags() {
        assert_eq!(options(&["custom.gba"]).rom, "custom.gba");
        assert_eq!(
            parse(&["a.gba", "b.gba"]).unwrap_err(),
            "unrecognized argument: b.gba"
        );
        assert_eq!(parse(&["-x"]).unwrap_err(), "unrecognized argument: -x");
        assert_eq!(
            parse(&["--drafts"]).unwrap_err(),
            "argument --drafts: expected one argument"
        );
    }
}
