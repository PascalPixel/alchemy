use candidate_compiler::verify::{CandidateCompilerConfiguration, CandidateCompilerFamily};
use compiler_core::routing::CompilerTarget;
use std::path::Path;
pub const USAGE: &str = "usage: candidate-show <candidate.c> [--unit ID] [--rom FILE] [--target gs1|gs2] [--owner ADDRESS] [--size BYTES] [--reference-symbols] [--work DIR] [--family routed|gcc296|old-agbcc|gcc3] [--flags -fa,-fb] [--remove-flags -fa,-fb] [--align] [--first] [--asm] [--patch FILE]";
pub const SHORT_USAGE: &str = "usage: candidate-show <candidate.c> [--rom FILE]";
#[derive(Debug, Clone)]
pub struct Options {
    pub source: String,
    pub rom: Option<String>,
    pub work: Option<String>,
    pub flags: Vec<String>,
    pub configuration: CandidateCompilerConfiguration,
    pub target: CompilerTarget,
    pub owner: Option<u32>,
    pub overlay: Option<String>,
    pub unit: Option<String>,
    pub precompiled_object: Option<String>,
    pub size: Option<usize>,
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
        target: CompilerTarget::Gs1,
        owner: None,
        overlay: None,
        unit: None,
        precompiled_object: None,
        size: None,
        align: false,
        first: false,
        asm: false,
        patch: None,
    };
    let mut rest = Vec::new();
    let mut rom_explicit = false;
    let mut index = 0;
    while index < argv.len() {
        let arg = argv[index].as_str();
        let next = |index: &mut usize| {
            *index += 1;
            argv.get(*index)
        };
        match arg {
            "--rom" => {
                options.rom = next(&mut index).cloned();
                rom_explicit = true;
            }
            "--target" => {
                options.target = match next(&mut index).map(String::as_str) {
                    Some("gs1") => CompilerTarget::Gs1,
                    Some("gs2") => CompilerTarget::Gs2,
                    _ => return Err("--target must be gs1 or gs2".into()),
                }
            }
            "--size" => {
                let value = next(&mut index).ok_or("--size requires a byte count")?;
                options.size = Some(parse_size(value)?);
            }
            "--owner" => {
                options.owner = Some(parse_address(
                    next(&mut index).ok_or("--owner requires an address")?,
                )?)
            }
            "--unit" => options.unit = next(&mut index).cloned(),
            "--reference-symbols" => options.configuration.reference_symbols = true,
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
    if (options.unit.is_none() && rest.len() != 1) || (options.unit.is_some() && !rest.is_empty()) {
        return Err(SHORT_USAGE.into());
    }
    options.source = rest.pop().unwrap_or_default();
    if !rom_explicit && options.target == CompilerTarget::Gs2 {
        options.rom = Some(root.join("roms/gs2-en.gba").to_string_lossy().into_owned());
    }
    if options.work.is_none() && options.unit.is_none() {
        options.work = Some(default_work(root, &options.source));
    }
    Ok(ParseOutcome::Options(Box::new(options)))
}
fn parse_size(value: &str) -> Result<usize, String> {
    let parsed = if let Some(hex) = value.strip_prefix("0x") {
        usize::from_str_radix(hex, 16)
    } else {
        value.parse::<usize>()
    };
    parsed
        .ok()
        .filter(|size| *size > 0)
        .ok_or_else(|| "--size must be a positive decimal or 0x-prefixed byte count".into())
}
fn parse_address(value: &str) -> Result<u32, String> {
    u32::from_str_radix(value.trim_start_matches("0x"), 16)
        .map_err(|_| "--owner must be a hexadecimal ROM address".into())
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
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn parses_cross_game_reference_options() {
        let root = Path::new("/repo");
        let args = [
            "games/gs2/recon/ja/main/08120450.c",
            "--target",
            "gs2",
            "--size",
            "0x206c",
            "--reference-symbols",
        ]
        .into_iter()
        .map(str::to_string)
        .collect::<Vec<_>>();
        let ParseOutcome::Options(options) = options_of(root, &args).unwrap() else {
            panic!("expected parsed options");
        };
        assert_eq!(options.target, CompilerTarget::Gs2);
        assert_eq!(options.size, Some(8300));
        assert_eq!(options.owner, None);
        assert!(options.configuration.reference_symbols);
        assert_eq!(options.rom.as_deref(), Some("/repo/roms/gs2-en.gba"));
    }
    #[test]
    fn size_must_be_positive() {
        assert_eq!(
            parse_size("0").unwrap_err(),
            "--size must be a positive decimal or 0x-prefixed byte count"
        );
    }
}
