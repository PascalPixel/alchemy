use crate::candidate::CandidateCompilerConfiguration;
use compiler_core::routing::CompilerTarget;
use std::path::Path;
pub const USAGE: &str = "usage: alchemy diff <candidate.c|overlay:address> [--unit ID] [--rom FILE] [--target gs1|gs2] [--owner OWNER] [--symbol ADDRESS] [--size BYTES] [--reference-symbols] [--work DIR] [--align] [--first] [--allocator-order] [--asm] [--patch FILE]";
pub const SHORT_USAGE: &str = "usage: diff <candidate.c> [--rom FILE]";
#[derive(Debug, Clone)]
pub struct Options {
    pub source: String,
    pub rom: Option<String>,
    pub work: Option<String>,
    pub configuration: CandidateCompilerConfiguration,
    pub target: CompilerTarget,
    pub owner: Option<u32>,
    pub overlay: Option<String>,
    pub unit: Option<String>,
    pub precompiled_object: Option<String>,
    pub size: Option<usize>,
    pub align: bool,
    pub first: bool,
    pub allocator_order: bool,
    pub asm: bool,
    pub patch: Option<String>,
}
impl Options {
    pub fn gs1(source: String) -> Self {
        Self {
            source,
            rom: None,
            work: None,
            configuration: CandidateCompilerConfiguration::default(),
            target: CompilerTarget::Gs1,
            owner: None,
            overlay: None,
            unit: None,
            precompiled_object: None,
            size: None,
            align: false,
            first: false,
            allocator_order: false,
            asm: false,
            patch: None,
        }
    }
}
#[derive(Debug)]
pub enum ParseOutcome {
    Help,
    Options(Box<Options>),
}
pub fn options_of(root: &Path, argv: &[String]) -> Result<ParseOutcome, String> {
    let mut options = Options::gs1(String::new());
    options.rom = Some(root.join("roms/gs1-en.gba").to_string_lossy().into_owned());
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
            "--size" | "--span" => {
                let value = next(&mut index).ok_or("--size requires a byte count")?;
                options.size = Some(parse_size(value)?);
            }
            "--symbol" => {
                let address =
                    parse_address(next(&mut index).ok_or("--symbol requires an address")?)?;
                options.configuration.owner_symbol = Some(format!("Func_{address:08x}"));
            }
            "--owner" => {
                let owner = compiler_core::source_paths::SourceOwner::parse_argument(
                    next(&mut index).ok_or("--owner requires an address")?,
                )?;
                options.owner = Some(owner.address());
                options.overlay = owner.overlay_id();
            }
            "--unit" => options.unit = next(&mut index).cloned(),
            "--reference-symbols" => options.configuration.reference_symbols = true,
            "--work" => options.work = next(&mut index).cloned(),
            "--align" => options.align = true,
            "--first" | "--allocator-order" => {
                options.first |= arg == "--first";
                options.allocator_order |= arg == "--allocator-order";
                options.align = true;
            }
            "--asm" => options.asm = true,
            "--patch" => options.patch = next(&mut index).cloned(),
            "--flags" | "--remove-flags" | "--family" => {
                return Err(format!(
                    "{arg} is retired; candidates use their canonical compiler route"
                ));
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
    let owner = compiler_core::source_paths::SourceOwner::parse_argument(value)?;
    if !owner.is_main() {
        return Err("overlay owners must use the owner-aware alchemy diff route".into());
    }
    Ok(owner.address())
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
    root.join("scratch/diff")
        .join(stem)
        .to_string_lossy()
        .into_owned()
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn overlay_identity_keeps_the_shared_render_options() {
        let args = [
            "candidate.c",
            "--owner",
            "resource_3ba:02002910",
            "--size",
            "0x20",
            "--first",
            "--allocator-order",
            "--asm",
            "--patch",
            "edit.patch",
            "--work",
            "out/probe",
        ]
        .map(str::to_owned);
        let ParseOutcome::Options(options) = options_of(Path::new("/repo"), &args).unwrap() else {
            panic!("expected options")
        };
        assert_eq!(options.owner, Some(0x02002910));
        assert_eq!(options.overlay.as_deref(), Some("resource_3ba"));
        assert_eq!(options.size, Some(32));
        assert!(options.first && options.allocator_order && options.align && options.asm);
        assert_eq!(options.patch.as_deref(), Some("edit.patch"));
        assert_eq!(options.work.as_deref(), Some("out/probe"));
    }
    #[test]
    fn candidate_routes_cannot_be_overridden() {
        for flag in ["--flags", "--remove-flags", "--family"] {
            let args = ["candidate.c", flag, "override"].map(str::to_string);
            assert!(options_of(Path::new("/repo"), &args)
                .unwrap_err()
                .contains("canonical compiler route"));
        }
    }
    #[test]
    fn main_identity_uses_the_shared_argument_parser() {
        for value in ["080bbb0c", "0x080bbb0c", "main:080bbb0c", "main:080BBB0C"] {
            assert_eq!(parse_address(value).unwrap(), 0x080bbb0c);
        }
        assert!(parse_address("resource_3ba:02002910").is_err());
        assert!(parse_address("main:not-an-address").is_err());
    }
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
