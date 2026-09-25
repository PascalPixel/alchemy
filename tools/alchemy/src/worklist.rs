//! `alchemy targets`: every not-yet-C owner, read from the listings that
//! still spell it, largest first.
//!
//! Overlay owners are the plain labels of the retained overlay listings
//! ([`crate::overlay::listing`]); main-image owners are the
//! `recon/<game>/raw/<address>.s` listings, sized by their rows. Each row
//! names the owner's draft when one exists, the halfword difference its
//! header records, and its wall: a draft header line `WALL: <reason>` says a
//! lane stopped on a structural obstacle that a new idea must address. Only
//! reads; nothing is written.

use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use std::path::{Path, PathBuf};

const USAGE: &str = "usage: alchemy targets [--game tbs|tla] [--min BYTES] [--max BYTES] [--range START-END] [--overlays|--main]\n\
Lists every not-yet-C owner by size: overlay owners from their listing labels, main owners from\n\
recon/<game>/raw/<address>.s. Columns: bytes, owner, name, draft, the halfword difference the draft\n\
header records (`N differing halfwords`, `N halfword edits`), and its wall (a header line `WALL: <reason>`).";

#[derive(Debug, Default, PartialEq)]
struct Options {
    game: Option<CompilerTarget>,
    min: Option<usize>,
    max: Option<usize>,
    range: Option<(u32, u32)>,
    overlays: bool,
    main: bool,
}

#[derive(Debug, PartialEq)]
pub struct Target {
    pub owner: SourceOwner,
    pub bytes: usize,
    pub name: String,
    pub draft: Option<PathBuf>,
    pub difference: Option<usize>,
    pub wall: Option<String>,
}

pub fn entry(arguments: &[String]) -> Result<(), String> {
    if arguments.iter().any(|arg| arg == "-h" || arg == "--help") {
        println!("{USAGE}");
        return Ok(());
    }
    let options = parse(arguments)?;
    let root = crate::compiler::routing::root();
    let game = options.game.unwrap_or(CompilerTarget::Tbs);
    let mut targets = collect(root, game, &options)?;
    targets.sort_by(|a, b| b.bytes.cmp(&a.bytes).then(a.owner.cmp(&b.owner)));
    print!("{}", render(root, &targets));
    Ok(())
}

fn parse(arguments: &[String]) -> Result<Options, String> {
    let mut options = Options::default();
    let mut index = 0;
    while index < arguments.len() {
        let arg = arguments[index].as_str();
        let mut value = || {
            index += 1;
            arguments
                .get(index)
                .map(String::as_str)
                .ok_or_else(|| format!("{arg} requires a value\n{USAGE}"))
        };
        match arg {
            "--game" => {
                options.game = Some(match value()? {
                    "tbs" => CompilerTarget::Tbs,
                    "tla" => CompilerTarget::Tla,
                    other => return Err(format!("--game must be tbs or tla, not {other}")),
                })
            }
            "--min" => options.min = Some(parse_count(value()?)?),
            "--max" => options.max = Some(parse_count(value()?)?),
            "--range" => {
                let text = value()?;
                let (start, end) = text
                    .split_once('-')
                    .ok_or_else(|| format!("--range takes START-END, not {text}"))?;
                options.range = Some((parse_address(start)?, parse_address(end)?));
            }
            "--overlays" => options.overlays = true,
            "--main" => options.main = true,
            other => return Err(format!("unknown argument {other}\n{USAGE}")),
        }
        index += 1;
    }
    if options.overlays && options.main {
        return Err("--overlays and --main exclude each other; omit both for every owner".into());
    }
    Ok(options)
}

fn parse_count(text: &str) -> Result<usize, String> {
    match text.strip_prefix("0x") {
        Some(hex) => usize::from_str_radix(hex, 16),
        None => text.parse(),
    }
    .map_err(|_| format!("{text}: expected a byte count"))
}

fn parse_address(text: &str) -> Result<u32, String> {
    let digits = text.trim_start_matches("0x");
    u32::from_str_radix(digits, 16).map_err(|_| format!("{text}: expected a hexadecimal address"))
}

impl Options {
    fn keeps(&self, owner: SourceOwner, bytes: usize) -> bool {
        (!self.overlays || !owner.is_main())
            && (!self.main || owner.is_main())
            && self.min.is_none_or(|min| bytes >= min)
            && self.max.is_none_or(|max| bytes <= max)
            && self
                .range
                .is_none_or(|(start, end)| (start..=end).contains(&owner.address()))
    }
}

fn collect(root: &Path, game: CompilerTarget, options: &Options) -> Result<Vec<Target>, String> {
    let paths = SourcePaths::load_for_game(root, game.as_str())?;
    let mut owners = Vec::new();
    if !options.main {
        let target = crate::overlay::owners::production_target(game);
        for (owner, (label, bytes)) in crate::overlay::owners::listed_owners(root, target)? {
            owners.push((owner, bytes, label));
        }
    }
    if !options.overlays {
        owners.extend(main_owners(root, game)?);
    }
    Ok(owners
        .into_iter()
        .filter(|(owner, bytes, _)| options.keeps(*owner, *bytes))
        .map(|(owner, bytes, label)| {
            let name = paths
                .registered_name(owner)
                .map(str::to_owned)
                .unwrap_or(label);
            let draft = draft_for(root, &paths, game, owner);
            let header = draft
                .as_ref()
                .and_then(|path| std::fs::read_to_string(path).ok())
                .map(|text| DraftHeader::read(&text))
                .unwrap_or_default();
            Target {
                owner,
                bytes,
                name,
                draft,
                difference: header.difference,
                wall: header.wall,
            }
        })
        .collect())
}

/// Every `recon/<game>/raw/<address>.s` listing: its owner, byte size and
/// first non-`Func_` label (or `Func_<address>`).
fn main_owners(
    root: &Path,
    game: CompilerTarget,
) -> Result<Vec<(SourceOwner, usize, String)>, String> {
    let directory = root.join(game.recon()).join("raw");
    let Ok(entries) = std::fs::read_dir(&directory) else {
        return Ok(Vec::new());
    };
    let mut owners = Vec::new();
    for entry in entries {
        let path = entry.map_err(|error| error.to_string())?.path();
        let Some(address) = path
            .file_name()
            .and_then(|name| name.to_str())
            .and_then(|name| name.strip_suffix(".s"))
            .filter(|stem| stem.len() == 8)
            .and_then(|stem| u32::from_str_radix(stem, 16).ok())
        else {
            continue;
        };
        let text = std::fs::read_to_string(&path)
            .map_err(|error| format!("{}: {error}", path.display()))?;
        let (bytes, label) =
            size_listing(&text).map_err(|error| format!("{}: {error}", path.display()))?;
        owners.push((
            SourceOwner::Main(address),
            bytes,
            label.unwrap_or_else(|| format!("Func_{address:08x}")),
        ));
    }
    Ok(owners)
}

/// The bytes a raw Thumb listing assembles to and its first descriptive
/// label. Every row is one Thumb halfword except `bl`/`blx` (4), data
/// directives by their width, and the assembler's bookkeeping (none).
fn size_listing(text: &str) -> Result<(usize, Option<String>), String> {
    let mut bytes = 0;
    let mut label = None;
    for line in text.lines() {
        let line = line.split('@').next().unwrap_or_default().trim();
        if line.is_empty() {
            continue;
        }
        if let Some(name) = line.strip_suffix(':') {
            if label.is_none() && !name.starts_with(".L") && !name.starts_with("Func_") {
                label = Some(name.to_string());
            }
            continue;
        }
        let mnemonic = line.split_whitespace().next().unwrap_or_default();
        bytes += match mnemonic {
            ".syntax" | ".thumb" | ".thumb_func" | ".global" | ".globl" | ".set" | ".text" => 0,
            ".4byte" | ".word" | ".inst.w" => 4,
            ".2byte" | ".hword" | ".short" | ".inst.n" | ".inst" => 2,
            ".byte" => 1,
            "bl" | "blx" => 4,
            directive if directive.starts_with('.') => {
                return Err(format!("unsupported listing directive {line:?}"))
            }
            _ => 2,
        };
    }
    Ok((bytes, label))
}

/// The draft that holds an owner's not-yet-exact C: a main owner's
/// `recon/<game>/<edition>/main/<address>.c` (English first), an overlay
/// owner's `recon/<game>/en/overlays/<resource>_c_<address>.c`, or an
/// unexact registered source.
fn draft_for(
    root: &Path,
    paths: &SourcePaths,
    game: CompilerTarget,
    owner: SourceOwner,
) -> Option<PathBuf> {
    let recon = root.join(game.recon());
    let mut candidates = Vec::new();
    if owner.is_main() {
        let file = format!("{}.c", owner.address_stem());
        candidates.push(recon.join("en/main").join(&file));
        if let Ok(entries) = std::fs::read_dir(&recon) {
            let mut editions = entries
                .filter_map(|entry| entry.ok().map(|entry| entry.path()))
                .collect::<Vec<_>>();
            editions.sort();
            candidates.extend(
                editions
                    .into_iter()
                    .map(|edition| edition.join("main").join(&file)),
            );
        }
    } else {
        candidates.push(
            recon
                .join("en/overlays")
                .join(format!("{}.c", owner.legacy_stem())),
        );
    }
    candidates.extend(paths.mapped_source_path(owner));
    candidates.into_iter().find(|path| path.is_file())
}

#[derive(Debug, Default, PartialEq)]
struct DraftHeader {
    difference: Option<usize>,
    wall: Option<String>,
}

impl DraftHeader {
    /// The first comment of a draft: the difference it records (the first
    /// `N differing halfwords`, `N halfwords differ` or `N halfword edits`)
    /// and a `WALL: <reason>` line.
    fn read(text: &str) -> Self {
        let header = text
            .find("/*")
            .and_then(|start| {
                text[start..]
                    .find("*/")
                    .map(|end| &text[start..start + end])
            })
            .unwrap_or_default();
        let words = header.split_whitespace().collect::<Vec<_>>();
        let difference = words.windows(3).find_map(|window| {
            let count = window[0].parse::<usize>().ok()?;
            let unit = |word: &str| {
                word.trim_matches(|c: char| !c.is_ascii_alphabetic())
                    .starts_with("halfword")
            };
            (unit(window[1]) || (window[1] == "differing" && unit(window[2]))).then_some(count)
        });
        let wall = header.lines().find_map(|line| {
            let line = line.trim().trim_start_matches(['/', '*']).trim();
            line.strip_prefix("WALL:")
                .map(|reason| reason.trim().to_string())
        });
        Self { difference, wall }
    }
}

fn render(root: &Path, targets: &[Target]) -> String {
    let mut out = format!(
        "{:>6}  {:<22} {:<40} {:<5} {:>5}  {}\n",
        "bytes", "owner", "name", "draft", "diff", "wall"
    );
    for target in targets {
        let draft = target.draft.as_ref().map(|path| {
            path.strip_prefix(root)
                .unwrap_or(path)
                .to_string_lossy()
                .into_owned()
        });
        out.push_str(&format!(
            "{:>6}  {:<22} {:<40} {:<5} {:>5}  {}{}\n",
            target.bytes,
            target.owner.id(),
            target.name,
            if draft.is_some() { "yes" } else { "-" },
            target
                .difference
                .map_or("-".into(), |count| count.to_string()),
            target.wall.as_deref().unwrap_or("-"),
            draft.map_or(String::new(), |path| format!("  {path}")),
        ));
    }
    let bytes = targets.iter().map(|target| target.bytes).sum::<usize>();
    let drafted = targets
        .iter()
        .filter(|target| target.draft.is_some())
        .count();
    let walled = targets
        .iter()
        .filter(|target| target.wall.is_some())
        .count();
    out.push_str(&format!(
        "owners={} bytes={bytes} drafted={drafted} walled={walled}\n",
        targets.len()
    ));
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn parses_filters() {
        let options = parse(&args(&[
            "--game",
            "tla",
            "--min",
            "0x40",
            "--max",
            "900",
            "--range",
            "0x08010000-08020000",
            "--main",
        ]))
        .unwrap();
        assert_eq!(options.game, Some(CompilerTarget::Tla));
        assert_eq!((options.min, options.max), (Some(64), Some(900)));
        assert_eq!(options.range, Some((0x0801_0000, 0x0802_0000)));
        assert!(options.main && !options.overlays);
        assert!(parse(&args(&["--main", "--overlays"])).is_err());
        assert!(parse(&args(&["--range", "08000000"])).is_err());
        assert!(parse(&args(&["--game", "gs3"])).is_err());
    }

    #[test]
    fn filters_select_space_size_and_range() {
        let options = parse(&args(&[
            "--min",
            "8",
            "--max",
            "64",
            "--range",
            "02000000-02000fff",
        ]))
        .unwrap();
        let overlay = SourceOwner::parse("resource_3ba:02000100").unwrap();
        assert!(options.keeps(overlay, 8));
        assert!(!options.keeps(overlay, 4));
        assert!(!options.keeps(overlay, 65));
        assert!(!options.keeps(SourceOwner::Main(0x0800_0100), 16));
        let main = parse(&args(&["--main"])).unwrap();
        assert!(main.keeps(SourceOwner::Main(0x0800_0100), 16));
        assert!(!main.keeps(overlay, 16));
    }

    #[test]
    fn raw_listing_sizes_thumb_rows_and_names_the_owner() {
        let listing = "\t.syntax unified\n\t.thumb\n\t.set sub_08000010, 0x08000010\n\t.global WaitFrames\n\t.global Func_08000000\n\t.thumb_func\nWaitFrames:\nFunc_08000000:\n\tpush\t{lr}\n\tbl\tsub_08000010 @ call\n.L_08000006:\n\tbne\t.L_08000006\n\t.2byte 0x4770\n\t.hword 0\n\t.4byte 0x03000000\n";
        assert_eq!(
            size_listing(listing).unwrap(),
            (16, Some("WaitFrames".into()))
        );
        assert_eq!(
            size_listing("Func_08000000:\n\tbx\tlr\n").unwrap(),
            (2, None)
        );
        assert!(size_listing("\t.align 2\n").is_err());
    }

    #[test]
    fn every_retained_main_listing_sizes() {
        let root = crate::compiler::routing::root();
        for game in [CompilerTarget::Tbs, CompilerTarget::Tla] {
            let owners = main_owners(root, game).unwrap();
            assert!(owners
                .iter()
                .all(|(owner, bytes, name)| { owner.is_main() && *bytes > 0 && !name.is_empty() }));
        }
    }

    #[test]
    fn draft_header_records_difference_and_wall() {
        let draft = "/* Draft, not exact (2026-09-24): 408 bytes for the 412-byte owner, 142\n   halfwords differ (was 192).\n   WALL: r8/sl allocation of the equipment loop */\nvoid f(void) { /* 3 halfwords */ }\n";
        assert_eq!(
            DraftHeader::read(draft),
            DraftHeader {
                difference: Some(142),
                wall: Some("r8/sl allocation of the equipment loop".into())
            }
        );
        for (text, count) in [
            ("/* NONMATCHING: 19 differing halfwords. */", 19),
            (
                "/*\n * Nonmatching: 26 halfword edits. Block layout matches.\n */",
                26,
            ),
            (
                "/* candidate=696 reference=712, 317 differing\n   halfwords. */",
                317,
            ),
        ] {
            assert_eq!(DraftHeader::read(text).difference, Some(count), "{text}");
        }
        assert_eq!(DraftHeader::read("void f(void);\n"), DraftHeader::default());
    }
}
