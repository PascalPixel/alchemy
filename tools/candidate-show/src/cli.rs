//! `optionsOf(argv)`.

use std::path::Path;

use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};

pub const USAGE: &str = "usage: candidate_show.ts <candidate.c> [--rom FILE] [--work DIR] [--family routed|gcc296|old-agbcc|pret-early-thumb|gcc2951|gcc3] [--flags -fa,-fb] [--remove-flags -fa,-fb]";

pub const SHORT_USAGE: &str = "usage: candidate_show.ts <candidate.c> [--rom FILE]";

/// `interface Options`.
///
/// `rom` and `work` are `Option` because `--rom` with no following argument
/// assigns `undefined` in JavaScript and does NOT throw until the value is
/// used. See the `PORT NOTE` on [`options_of`].
#[derive(Debug, Clone)]
pub struct Options {
    pub source: String,
    pub rom: Option<String>,
    pub work: Option<String>,
    pub flags: Vec<String>,
    pub configuration: CandidateCompilerConfiguration,
}

#[derive(Debug)]
pub enum ParseOutcome {
    /// `-h` / `--help`: prints usage and `process.exit(0)`, mid-loop, before
    /// any later argument is examined.
    Help,
    Options(Box<Options>),
}

/// `optionsOf(Bun.argv.slice(2))`.
///
/// PORT NOTE -- option values are read as `argv[++index]`, which is `undefined`
/// past the end AND leaves `index` past the end so the loop simply terminates.
/// The four consequences are all different and all reproduced:
///
/// * `--flags` / `--remove-flags` call `.split` on `undefined` and throw
///   immediately -> `Err` here.
/// * `--family` runs `includes(undefined)`, which is `false`, so it throws the
///   `--family must be ...` message -> `Err` here.
/// * `--rom` / `--work` store `undefined` and throw only later, from
///   `readFileSync`/`mkdirSync` -> `None` here.
///
/// PORT NOTE -- there is no `--flag=value` form. `--rom=x` falls through to the
/// positional list and becomes the source path.
///
/// PORT NOTE -- `-h` is matched inside the loop, so `--rom -h` consumes `-h` as
/// the ROM path and never prints usage.
pub fn options_of(root: &Path, argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options {
        source: String::new(),
        rom: Some(root.join("roms/gs1-en.gba").to_string_lossy().into_owned()),
        work: Some(
            root.join("work/candidate-show")
                .to_string_lossy()
                .into_owned(),
        ),
        flags: Vec::new(),
        configuration: CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            add_flags: Vec::new(),
            remove_flags: Vec::new(),
        },
    };
    let mut rest: Vec<String> = Vec::new();
    let mut index = 0usize;
    while index < argv.len() {
        let argument = argv[index].as_str();
        // `argv[++index]`: advance first, then read, and `undefined` past the
        // end.
        let take = |index: &mut usize| -> Option<&String> {
            *index += 1;
            argv.get(*index)
        };
        match argument {
            "--rom" => options.rom = take(&mut index).cloned(),
            "--work" => options.work = take(&mut index).cloned(),
            // A candidate mode has to be visible before it is worth routing:
            // the native allowlists are shared, so probing one here
            // keeps a trial flag out of every other region's build.
            "--flags" => {
                let value = take(&mut index).ok_or_else(|| {
                    "undefined is not an object (evaluating 'argv[++index].split')".to_string()
                })?;
                options.flags = split_filter_boolean(value);
            }
            "--remove-flags" => {
                let value = take(&mut index).ok_or_else(|| {
                    "undefined is not an object (evaluating 'argv[++index].split')".to_string()
                })?;
                options.configuration.remove_flags = split_filter_boolean(value);
            }
            "--family" => {
                let family = take(&mut index)
                    .and_then(|text| CandidateCompilerFamily::from_str_option(text));
                match family {
                    Some(family) => options.configuration.family = Some(family),
                    None => {
                        return Err("--family must be routed, gcc296, old-agbcc, pret-early-thumb, gcc2951, or gcc3".to_string())
                    }
                }
            }
            "-h" | "--help" => return Ok(ParseOutcome::Help),
            other => rest.push(other.to_string()),
        }
        index += 1;
    }
    if rest.len() != 1 {
        return Err(SHORT_USAGE.to_string());
    }
    options.source = rest.remove(0);
    Ok(ParseOutcome::Options(Box::new(options)))
}

/// `value.split(",").filter(Boolean)`.
///
/// PORT NOTE -- `filter(Boolean)` drops the empty string ONLY. A field of
/// spaces survives, so `"--flags ' '"` passes a whitespace flag to gcc exactly
/// as the TypeScript does.
fn split_filter_boolean(value: &str) -> Vec<String> {
    value
        .split(',')
        .filter(|field| !field.is_empty())
        .map(str::to_string)
        .collect()
}

/// The union membership test `["routed", …].includes(family)`.
trait FamilyFromStr: Sized {
    fn from_str_option(text: &str) -> Option<Self>;
}

impl FamilyFromStr for CandidateCompilerFamily {
    fn from_str_option(text: &str) -> Option<Self> {
        match text {
            "routed" => Some(Self::Routed),
            "gcc296" => Some(Self::Gcc296),
            "old-agbcc" => Some(Self::OldAgbcc),
            "pret-early-thumb" => Some(Self::PretEarlyThumb),
            "gcc2951" => Some(Self::Gcc2951),
            "gcc3" => Some(Self::Gcc3),
            _ => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn parse(args: &[&str]) -> Result<ParseOutcome, String> {
        let argv: Vec<String> = args.iter().map(|s| s.to_string()).collect();
        options_of(Path::new("/repo"), &argv)
    }

    fn unwrap_options(args: &[&str]) -> Options {
        match parse(args).unwrap() {
            ParseOutcome::Options(options) => *options,
            ParseOutcome::Help => panic!("expected options"),
        }
    }

    #[test]
    fn defaults_are_repository_relative() {
        let options = unwrap_options(&["a.c"]);
        assert_eq!(options.source, "a.c");
        assert_eq!(options.rom.as_deref(), Some("/repo/roms/gs1-en.gba"));
        assert_eq!(options.work.as_deref(), Some("/repo/work/candidate-show"));
        assert_eq!(
            options.configuration.family,
            Some(CandidateCompilerFamily::Routed)
        );
    }

    #[test]
    fn flags_split_on_comma_and_drop_only_empty_fields() {
        let options = unwrap_options(&["a.c", "--flags", "-fa,,-fb,"]);
        assert_eq!(options.flags, vec!["-fa", "-fb"]);
        let empty = unwrap_options(&["a.c", "--flags", ""]);
        assert!(empty.flags.is_empty());
        let spaces = unwrap_options(&["a.c", "--flags", " "]);
        assert_eq!(spaces.flags, vec![" "]);
    }

    #[test]
    fn help_wins_only_when_it_is_not_consumed_as_a_value() {
        assert!(matches!(parse(&["-h"]).unwrap(), ParseOutcome::Help));
        // `--rom -h` eats `-h`, then there is no positional argument.
        assert_eq!(parse(&["--rom", "-h"]).unwrap_err(), SHORT_USAGE);
    }

    #[test]
    fn a_missing_value_terminates_the_loop_without_a_positional() {
        // `--rom` with no value stores `undefined` and does NOT throw here;
        // the positional is still present, so parsing succeeds and the failure
        // is deferred to `readFileSync`.
        let options = unwrap_options(&["a.c", "--rom"]);
        assert!(options.rom.is_none());
        // `--rom` alone: rom is `undefined`, and there is no positional.
        assert_eq!(parse(&["--rom"]).unwrap_err(), SHORT_USAGE);
    }

    #[test]
    fn a_missing_flags_value_throws_before_the_positional_check() {
        let error = parse(&["a.c", "--flags"]).unwrap_err();
        assert!(error.contains("undefined"), "{error}");
        let error = parse(&["a.c", "--remove-flags"]).unwrap_err();
        assert!(error.contains("undefined"), "{error}");
    }

    #[test]
    fn an_unknown_family_reports_the_union() {
        let error = parse(&["a.c", "--family", "clang"]).unwrap_err();
        assert_eq!(
            error,
            "--family must be routed, gcc296, old-agbcc, pret-early-thumb, gcc2951, or gcc3"
        );
        assert_eq!(parse(&["a.c", "--family"]).unwrap_err(), error);
    }

    #[test]
    fn there_is_no_equals_form() {
        let options = unwrap_options(&["--rom=x"]);
        assert_eq!(options.source, "--rom=x");
        assert_eq!(options.rom.as_deref(), Some("/repo/roms/gs1-en.gba"));
    }

    #[test]
    fn two_positionals_report_the_short_usage() {
        assert_eq!(parse(&["a.c", "b.c"]).unwrap_err(), SHORT_USAGE);
    }
}
