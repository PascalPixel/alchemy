use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};
use std::path::Path;
pub const USAGE: &str = "usage: candidate-show <candidate.c> [--rom FILE] [--work DIR] [--family routed|gcc296|old-agbcc|gcc3] [--flags -fa,-fb] [--remove-flags -fa,-fb] [--align] [--first] [--asm] [--patch FILE]";
pub const SHORT_USAGE: &str = "usage: candidate-show <candidate.c> [--rom FILE]";
#[derive(Debug, Clone)]
pub struct Options {
    pub source: String,
    pub rom: Option<String>,
    pub work: Option<String>,
    pub flags: Vec<String>,
    pub configuration: CandidateCompilerConfiguration,
    pub align: bool,
    pub first: bool,
    pub asm: bool,
    pub patch: Option<String>,
}
#[derive(Debug)]
pub enum ParseOutcome {
    Help,
    Options(Box<Options>),
}
pub fn options_of(root: &Path, argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options {
        source: String::new(),
        rom: Some(root.join("roms/gs1-en.gba").to_string_lossy().into_owned()),
        work: None,
        flags: Vec::new(),
        configuration: CandidateCompilerConfiguration {
            family: Some(CandidateCompilerFamily::Routed),
            ..Default::default()
        },
        align: false,
        first: false,
        asm: false,
        patch: None,
    };
    let mut rest = Vec::new();
    let mut index = 0;
    while index < argv.len() {
        let arg = argv[index].as_str();
        let next = |index: &mut usize| {
            *index += 1;
            argv.get(*index)
        };
        match arg {
            "--rom" => options.rom = next(&mut index).cloned(),
            "--work" => options.work = next(&mut index).cloned(),
            "--align" => options.align = true,
            "--first" => {
                options.first = true;
                options.align = true;
            }
            "--asm" => options.asm = true,
            "--patch" => options.patch = next(&mut index).cloned(),
            "--flags" => options.flags = split(next(&mut index))?,
            "--remove-flags" => options.configuration.remove_flags = split(next(&mut index))?,
            "--family" => {
                options.configuration.family =
                    next(&mut index).and_then(|value| CandidateCompilerFamily::parse(value));
                if options.configuration.family.is_none() {
                    return Err("--family must be routed, gcc296, old-agbcc, or gcc3".into());
                }
            }
            "-h" | "--help" => return Ok(ParseOutcome::Help),
            other => rest.push(other.into()),
        }
        index += 1;
    }
    if rest.len() != 1 {
        return Err(SHORT_USAGE.into());
    }
    options.source = rest.remove(0);
    if options.work.is_none() {
        options.work = Some(default_work(root, &options.source));
    }
    Ok(ParseOutcome::Options(Box::new(options)))
}
fn split(value: Option<&String>) -> Result<Vec<String>, String> {
    let value = value.ok_or("undefined is not an object (evaluating 'argv[++index].split')")?;
    Ok(value
        .split(',')
        .filter(|value| !value.is_empty())
        .map(str::to_string)
        .collect())
}
fn default_work(root: &Path, source: &str) -> String {
    let stem = Path::new(source)
        .file_stem()
        .and_then(|value| value.to_str())
        .filter(|value| {
            !value.is_empty()
                && value
                    .chars()
                    .all(|c| c.is_ascii_alphanumeric() || matches!(c, '_' | '-'))
        })
        .unwrap_or("candidate");
    root.join("scratch/candidate-show")
        .join(stem)
        .to_string_lossy()
        .into_owned()
}
