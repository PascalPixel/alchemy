//! `alchemy overlay trial`: open one overlay owner for C work.
//!
//! A trial is the overlay recipe's working state. It registers the owner's
//! source path and a single-overlay translation unit, splices the owner's
//! `AlchemyC_` placeholder into its listing (a unit under the source root
//! must be exact placed C to load), and writes a first draft: the owner's
//! recon draft when there is one, otherwise `psynergy decompile` output with
//! every call named (see `names`) and the literal pool listed. The unit binds
//! whatever names the source uses at their runtime addresses.
//!
//! Everything a trial displaces is recorded in `out/trial/<stem>.json`, so
//! `--undo` puts back exactly the listing lines, register record and units
//! it replaced; `alchemy overlay adopt` adopts a trial in place and
//! `alchemy overlay draft` parks it as a committed draft.

use crate::compiler::build_io::{relative, Snapshot};
use crate::compiler::source_paths::{SourceOwner, SourcePaths};
use crate::overlay::assembly::OVERLAY_BASE;
use crate::overlay::names::{self, Dictionary};
use crate::overlay::rom::CanonicalRom;
use crate::targets::{decomp_target, DecompTarget};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

const USAGE: &str = "usage: alchemy overlay trial OWNER [--name NAME] [--source PATH] [--decompile] [--target tbs-en|tla-en]\n\
       alchemy overlay trial OWNER --undo\n\
Opens OWNER (resource_OOO:AAAAAAAA) for C work: registers NAME at PATH (under the game's SRC; by\n\
default the registered name and path, else <Place>_Func<address> in the overlay's most used\n\
directory), declares the unit scene-OOO-AAAAAAAA, splices the owner's placeholder into its listing\n\
and writes the first draft: the owner's recon draft, or with --decompile (or without a draft)\n\
psynergy decompile output with calls named Engine_*, Main_*, Local_* or registered names and the\n\
literal pool listed from the ROM. Then it scores. --undo removes the trial and restores what it\n\
displaced. Work with alchemy overlay try, then alchemy overlay adopt or alchemy overlay draft.";

/// What one open trial displaced, recorded beside the build outputs.
#[derive(Clone, Debug, Deserialize, Eq, PartialEq, Serialize)]
pub struct Trial {
    pub owner: String,
    pub target: String,
    pub span: i64,
    pub name: String,
    /// The trial's C source, repository-relative.
    pub source: String,
    /// The trial's translation unit id.
    pub unit: String,
    /// The owner's source-path record before the trial, if any.
    pub record: Option<Value>,
    /// Units that declared the owner before the trial, with their index.
    pub units: Vec<(usize, Value)>,
    /// The placeholder block the trial put into the listing...
    pub placeholder: Vec<String>,
    /// ...in place of these retained lines.
    pub removed: Vec<String>,
}

fn record_path(root: &Path, owner: SourceOwner) -> PathBuf {
    root.join("out/trial")
        .join(format!("{}.json", owner.legacy_stem()))
}

fn read_json(path: &Path) -> Result<Value, String> {
    let text = fs::read_to_string(path).map_err(|error| format!("{}: {error}", path.display()))?;
    serde_json::from_str(&text).map_err(|error| format!("{}: {error}", path.display()))
}

/// Registers are written as they are kept: two-space JSON, keys in order.
pub(crate) fn write_json(path: &Path, value: &Value) -> Result<(), String> {
    let rendered = serde_json::to_string_pretty(value).map_err(|error| error.to_string())?;
    fs::write(path, format!("{rendered}\n")).map_err(|error| format!("{}: {error}", path.display()))
}

pub(crate) fn units_path(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.recon_dir()).join("translation-units.json")
}

fn register_path(root: &Path, target: DecompTarget) -> PathBuf {
    root.join(target.recon_dir()).join("source-paths.json")
}

/// A unit's canonical declaration of `address` in `overlay`, or an error
/// when it links the owner as an instance of a shared module.
fn declares(unit: &Value, overlay: &str, address: &str) -> Result<bool, String> {
    let owns = |owners: &Value| {
        owners
            .as_array()
            .into_iter()
            .flatten()
            .any(|owner| owner["address"].as_str() == Some(address))
    };
    if unit["overlay"].as_str() == Some(overlay) && owns(&unit["owners"]) {
        return Ok(true);
    }
    let instance = unit["instances"][overlay]["owners"]
        .as_object()
        .into_iter()
        .flatten()
        .any(|(_, owner)| owner["address"].as_str() == Some(address));
    if instance {
        return Err(format!(
            "{overlay}:{} is an instance member of unit {}; work on it through that unit",
            address.trim_start_matches("0x"),
            unit["id"].as_str().unwrap_or("?")
        ));
    }
    Ok(false)
}

/// The listing with a trial's placeholder replaced by the lines it displaced.
pub fn unsplice(text: &str, placeholder: &[String], removed: &[String]) -> Result<String, String> {
    let lines: Vec<&str> = text.split('\n').collect();
    let head = placeholder.first().ok_or("empty placeholder")?;
    let starts = lines
        .iter()
        .enumerate()
        .filter(|(_, line)| **line == head.as_str())
        .map(|(index, _)| index)
        .collect::<Vec<_>>();
    let [start] = starts[..] else {
        return Err(format!(
            "the listing holds {} {head} lines, not one",
            starts.len()
        ));
    };
    let block = lines.get(start..start + placeholder.len());
    if block.is_none_or(|block| block.iter().zip(placeholder).any(|(a, b)| *a != b)) {
        return Err(format!(
            "the listing's {head} block is not the trial's placeholder"
        ));
    }
    let mut out = lines[..start]
        .iter()
        .map(|line| line.to_string())
        .collect::<Vec<_>>();
    out.extend(removed.iter().cloned());
    out.extend(
        lines[start + placeholder.len()..]
            .iter()
            .map(|line| line.to_string()),
    );
    Ok(out.join("\n"))
}

impl Trial {
    /// The open trial for `owner`, if any.
    pub fn open(root: &Path, owner: SourceOwner) -> Result<Option<Self>, String> {
        let path = record_path(root, owner);
        if !path.is_file() {
            return Ok(None);
        }
        let record = read_json(&path)?;
        serde_json::from_value(record)
            .map(Some)
            .map_err(|error| format!("{}: {error}", path.display()))
    }

    /// The open trial for `owner`, or an error that says how to open one.
    pub fn require(root: &Path, owner: SourceOwner) -> Result<Self, String> {
        Self::open(root, owner)?.ok_or_else(|| {
            format!(
                "{} has no open trial; start one with alchemy overlay trial {}",
                owner.id(),
                owner.id()
            )
        })
    }

    pub fn owner(&self) -> Result<SourceOwner, String> {
        SourceOwner::parse(&self.owner)
    }

    pub fn target(&self) -> Result<DecompTarget, String> {
        decomp_target(Some(&self.target))
    }

    fn listing(&self, root: &Path) -> Result<PathBuf, String> {
        let overlay = self.owner()?.overlay_id().ok_or("not an overlay owner")?;
        Ok(root.join(self.target()?.overlay_assembly(&overlay)))
    }

    /// `text` with this trial's placeholder back to the retained lines.
    pub fn restored_listing(&self, text: &str) -> Result<String, String> {
        unsplice(text, &self.placeholder, &self.removed)
            .map_err(|error| format!("{}: {error}", self.owner))
    }

    fn files(&self, root: &Path) -> Result<Vec<PathBuf>, String> {
        let target = self.target()?;
        Ok(vec![
            units_path(root, target),
            register_path(root, target),
            self.listing(root)?,
            root.join(&self.source),
            record_path(root, self.owner()?),
        ])
    }

    /// Restores everything the trial displaced and removes its source.
    pub fn undo(&self, root: &Path) -> Result<(), String> {
        let snapshot = Snapshot::take(&self.files(root)?)?;
        self.undo_steps(root).map_err(|error| {
            snapshot.restore();
            format!("{error}; the trial is left as it was")
        })
    }

    fn undo_steps(&self, root: &Path) -> Result<(), String> {
        let target = self.target()?;
        let listing = self.listing(root)?;
        let text =
            fs::read_to_string(&listing).map_err(|e| format!("{}: {e}", listing.display()))?;
        fs::write(&listing, self.restored_listing(&text)?).map_err(|error| error.to_string())?;
        let units = units_path(root, target);
        let mut document = read_json(&units)?;
        let list = document["units"]
            .as_array_mut()
            .ok_or("translation units must be an array")?;
        list.retain(|unit| unit["id"].as_str() != Some(&self.unit));
        for (index, unit) in &self.units {
            list.insert((*index).min(list.len()), unit.clone());
        }
        write_json(&units, &document)?;
        let register = register_path(root, target);
        let mut paths = read_json(&register)?;
        let owners = paths["owners"]
            .as_object_mut()
            .ok_or("source-path owners must be an object")?;
        match &self.record {
            Some(record) => {
                owners.insert(self.owner.clone(), record.clone());
            }
            None => {
                owners.shift_remove(&self.owner);
            }
        }
        write_json(&register, &paths)?;
        remove_source(root, target, &root.join(&self.source))?;
        self.close(root)
    }

    /// Forgets the trial, leaving its registration as it stands (adopted).
    pub fn close(&self, root: &Path) -> Result<(), String> {
        let path = record_path(root, self.owner()?);
        match fs::remove_file(&path) {
            Ok(()) => Ok(()),
            Err(error) if error.kind() == std::io::ErrorKind::NotFound => Ok(()),
            Err(error) => Err(format!("{}: {error}", path.display())),
        }
    }

    /// Binds the names the trial source and `extra` sources use in the
    /// trial's unit; returns how many symbols it declares.
    pub fn rebind(&self, root: &Path, extra: &[String]) -> Result<usize, String> {
        let context = Context::load(root, self.target()?, self.owner()?)?;
        let own = fs::read_to_string(root.join(&self.source))
            .map_err(|error| format!("{}: {error}", self.source))?;
        let texts = std::iter::once(own.as_str()).chain(extra.iter().map(String::as_str));
        let kept = names::declared_symbols(
            &json!({"units": self.units.iter().map(|(_, unit)| unit).collect::<Vec<_>>()}),
        )
        .into_iter()
        .map(|symbol| (symbol.name, symbol.bound))
        .collect();
        let bound = context.bind(texts, &kept, &self.name);
        let units = units_path(root, context.target);
        let mut document = read_json(&units)?;
        let unit = document["units"]
            .as_array_mut()
            .and_then(|units| {
                units
                    .iter_mut()
                    .find(|unit| unit["id"].as_str() == Some(&self.unit))
            })
            .ok_or_else(|| format!("the trial's unit {} is gone; undo the trial", self.unit))?;
        let symbols = names::symbols_json(&bound);
        if unit["absolute_symbols"] != symbols {
            unit["absolute_symbols"] = symbols;
            write_json(&units, &document)?;
        }
        Ok(bound.len())
    }
}

/// Removes a trial's source and the directories it alone occupied.
fn remove_source(root: &Path, target: DecompTarget, source: &Path) -> Result<(), String> {
    match fs::remove_file(source) {
        Ok(()) => {}
        Err(error) if error.kind() == std::io::ErrorKind::NotFound => {}
        Err(error) => return Err(format!("{}: {error}", source.display())),
    }
    let top = root.join(target.game_dir()).join("SRC");
    let mut directory = source.parent();
    while let Some(current) =
        directory.filter(|current| current.starts_with(&top) && *current != top)
    {
        if fs::remove_dir(current).is_err() {
            break;
        }
        directory = current.parent();
    }
    Ok(())
}

/// One overlay owner with the ROM image and the names the repository knows.
pub struct Context {
    pub target: DecompTarget,
    pub owner: SourceOwner,
    pub overlay: String,
    pub image: Vec<u8>,
    pub paths: SourcePaths,
    pub dictionary: Dictionary,
}

impl Context {
    pub fn load(root: &Path, target: DecompTarget, owner: SourceOwner) -> Result<Self, String> {
        let overlay = owner.overlay_id().ok_or("trials are for overlay owners")?;
        let units = read_json(&units_path(root, target))?;
        let declared = names::declared_symbols(&units);
        let rom = CanonicalRom::load_target(root, target)?;
        let main = crate::compiler::overlay::main_image(target.compiler)?;
        let images = declared
            .iter()
            .map(|symbol| symbol.image.as_str())
            .chain([overlay.as_str()])
            .filter(|image| image.starts_with("resource_"))
            .collect::<BTreeSet<_>>();
        let mut veneers = BTreeMap::new();
        let mut image = Vec::new();
        for name in images {
            let bytes = rom.overlay(name)?;
            let table = crate::compiler::overlay::import_veneers(&bytes, main)
                .into_iter()
                .filter_map(|(runtime, chain)| Some((runtime, *chain.first()?)))
                .collect::<BTreeMap<_, _>>();
            veneers.insert(name.to_string(), table);
            if name == overlay {
                image = bytes;
            }
        }
        let dictionary = Dictionary::new(&overlay, image.len(), &declared, &veneers);
        Ok(Self {
            target,
            owner,
            paths: SourcePaths::load_for_game(root, target.compiler.as_str())?,
            overlay,
            image,
            dictionary,
        })
    }

    pub fn bind<'a>(
        &self,
        sources: impl IntoIterator<Item = &'a str>,
        kept: &BTreeMap<String, names::Bound>,
        own: &str,
    ) -> BTreeMap<String, names::Bound> {
        names::bind(sources, kept, &self.dictionary, |name| {
            name == own || names::link_binds(&self.paths, &self.overlay, name)
        })
    }

    /// `psynergy decompile` of the owner with its calls named and its
    /// literal pool listed; returns the source and the named call count.
    pub fn decompile(&self, root: &Path, span: u32, name: &str) -> Result<(String, usize), String> {
        let entry = self.owner.address();
        let imports =
            crate::recovery::imports_for(root, self.target, &self.owner.id(), Some(span))?;
        let calls = imports
            .into_iter()
            .map(|import| {
                let named = match (import.kind, import.main) {
                    ("veneer", Some(main)) => self.dictionary.veneer_name(
                        u64::from(import.target + names::RUNTIME_DISPLACEMENT),
                        main,
                        names::registered_main(&self.paths, main).as_deref(),
                    ),
                    _ => self
                        .dictionary
                        .local_name(import.target, import.name.as_deref()),
                };
                ResolvedCall {
                    site: import.site,
                    target: import.target,
                    name: named,
                }
            })
            .collect::<Vec<_>>();
        decompile_source(&self.image, entry, span, &calls, &self.dictionary, name)
    }
}

/// Lift the image the game executes, including loader-transformed pool words.
fn decompile_source(
    stored: &[u8],
    entry: u32,
    span: u32,
    imports: &[ResolvedCall],
    dictionary: &Dictionary,
    name: &str,
) -> Result<(String, usize), String> {
    let image = crate::compiler::overlay::load(stored, 0)?;
    let mut ins = psynergy::decode::decode_window_at(&image, OVERLAY_BASE as u32, entry, span);
    let calls = resolve_calls(&mut ins, imports);
    let (body, tables) = psynergy::unit::bodies(&ins, &|_, _| None);
    let decompiled = psynergy::unit::compose(entry, "Function", &body, &tables);
    let decompiled = decompiled.replacen("#include \"types.h\"", "#include \"TYPES.H\"", 1);
    let mut text = names::convert(&decompiled, &calls, name);
    let pool = names::pool_comment(&names::pool_words(&image, entry, span), dictionary);
    let include = "#include \"TYPES.H\"\n";
    if let Some(at) = text.find(include) {
        text.insert_str(at + include.len(), &format!("\n{pool}"));
    } else {
        text.insert_str(0, &pool);
    }
    Ok((text, calls.len()))
}

#[derive(Debug, Clone, PartialEq, Eq)]
struct ResolvedCall {
    site: u32,
    target: u32,
    name: String,
}

/// Restore each overlay call's true target before lifting, then key its name
/// by that target so site-specific pseudo-target collisions stay distinct.
fn resolve_calls(
    ins: &mut [psynergy::decode::Ins],
    imports: &[ResolvedCall],
) -> BTreeMap<String, String> {
    let mut targets = BTreeMap::new();
    let mut names = BTreeMap::new();
    for import in imports {
        targets.insert(import.site, import.target);
        names.insert(format!("Func_{:08x}", import.target), import.name.clone());
    }
    for instruction in ins {
        if let psynergy::decode::Kind::Bl { target } = &mut instruction.kind {
            if let Some(resolved) = targets.get(&instruction.addr) {
                *target = *resolved;
            }
        }
    }
    names
}

/// The directory most of the overlay's registered sources share, else a
/// placeholder place the lane renames.
fn default_place(paths: &SourcePaths, overlay: &str) -> String {
    let mut counts = BTreeMap::<String, usize>::new();
    for owner in paths.registered_owners() {
        if owner.overlay_id().as_deref() != Some(overlay) {
            continue;
        }
        if let Some(directory) = paths.mapped_relative_path(owner).and_then(Path::parent) {
            *counts
                .entry(directory.to_string_lossy().into_owned())
                .or_default() += 1;
        }
    }
    counts
        .into_iter()
        .max_by(|(a, left), (b, right)| left.cmp(right).then(b.cmp(a)))
        .map(|(directory, _)| directory)
        .unwrap_or_else(|| {
            format!(
                "FIELD/TMP_{}",
                overlay.trim_start_matches("resource_").to_uppercase()
            )
        })
}

/// The name and SRC path an earlier `alchemy overlay draft` recorded: the
/// draft's function, unless it spells an address, and the path its header
/// says it is meant for.
fn drafted_as(text: &str) -> (Option<String>, Option<String>) {
    let name = names::exported_function(text)
        .filter(|name| !name.starts_with("Func_") && !name.starts_with("Local_"));
    let meant = regex::Regex::new(r"meant for[\s*]+([A-Z0-9_]+(?:/[A-Z0-9_]+)*\.C)\b").unwrap();
    let path = meant.captures(text).map(|captures| captures[1].to_string());
    (name, path)
}

/// `<Place>_Func<address>` from the place's last directory: `BABI_FUNE`
/// gives `BabiFune_Func02000194`.
fn default_name(place: &str, owner: SourceOwner) -> String {
    let last = place.rsplit('/').next().unwrap_or(place);
    let words = last
        .split('_')
        .filter(|word| !word.is_empty())
        .map(|word| {
            let lower = word.to_lowercase();
            let mut letters = lower.chars();
            letters
                .next()
                .map(|first| first.to_uppercase().chain(letters).collect::<String>())
                .unwrap_or_default()
        })
        .collect::<String>();
    format!("{words}_Func{}", owner.address_stem())
}

/// `F_<last five address digits>.C` in the place.
fn default_source(place: &str, owner: SourceOwner) -> String {
    format!("{place}/F_{}.C", &owner.address_stem()[3..].to_uppercase())
}

#[derive(Debug, Default, PartialEq)]
struct Options {
    owner: String,
    name: Option<String>,
    source: Option<String>,
    decompile: bool,
    undo: bool,
    target: Option<String>,
}

fn parse(arguments: &[String]) -> Result<Option<Options>, String> {
    let mut options = Options::default();
    let mut rest = arguments.iter();
    while let Some(argument) = rest.next() {
        let mut value = || {
            rest.next()
                .cloned()
                .ok_or_else(|| format!("{argument} requires a value"))
        };
        match argument.as_str() {
            "--name" => options.name = Some(value()?),
            "--source" => options.source = Some(value()?),
            "--target" => options.target = Some(value()?),
            "--decompile" => options.decompile = true,
            "--undo" => options.undo = true,
            "-h" | "--help" => return Ok(None),
            flag if flag.starts_with('-') => return Err(format!("unknown option {flag}\n{USAGE}")),
            _ if options.owner.is_empty() => options.owner = argument.clone(),
            other => return Err(format!("unexpected argument {other}\n{USAGE}")),
        }
    }
    if options.owner.is_empty() {
        return Err(USAGE.into());
    }
    Ok(Some(options))
}

/// An owner argument, `resource_OOO:AAAAAAAA`, as an overlay owner.
pub fn overlay_owner(argument: &str) -> Result<SourceOwner, String> {
    let owner = SourceOwner::parse_argument(argument)?;
    owner
        .overlay_id()
        .ok_or_else(|| format!("{argument}: not an overlay owner"))?;
    Ok(owner)
}

/// A source path under the game's SRC, accepted with or without that prefix.
fn source_relative(target: DecompTarget, path: &str) -> String {
    let prefix = format!("{}/SRC/", target.game_dir());
    path.strip_prefix(&prefix).unwrap_or(path).to_string()
}

pub fn run(root: &Path, arguments: &[String]) -> Result<i32, String> {
    let Some(options) = parse(arguments)? else {
        println!("{USAGE}");
        return Ok(0);
    };
    let owner = overlay_owner(&options.owner)?;
    if options.undo {
        Trial::require(root, owner)?.undo(root)?;
        println!("trial=undone {}", owner.id());
        return Ok(0);
    }
    if let Some(open) = Trial::open(root, owner)? {
        return Err(format!(
            "{} already has a trial at {}; continue it or run alchemy overlay trial {} --undo",
            owner.id(),
            open.source,
            owner.id()
        ));
    }
    let target = decomp_target(options.target.as_deref())?;
    let (trial, from, calls) = open(root, target, owner, &options)?;
    let symbols = trial.rebind(root, &[])?;
    println!(
        "trial=open {} span={} name={} source={} unit={} from={from} calls={calls} symbols={symbols}",
        trial.owner, trial.span, trial.name, trial.source, trial.unit
    );
    print!("{}", score_summary(root, &trial.source, &trial.owner)?);
    println!(
        "next: edit {}; alchemy overlay try {} [FILE...]; when exact alchemy overlay adopt {} --apply, else alchemy overlay draft {} --note TEXT",
        trial.source, trial.owner, trial.owner, trial.owner
    );
    Ok(0)
}

fn open(
    root: &Path,
    target: DecompTarget,
    owner: SourceOwner,
    options: &Options,
) -> Result<(Trial, &'static str, usize), String> {
    let overlay = owner.overlay_id().unwrap_or_default();
    let span = *crate::overlay::owners::reviewed_spans(root, target)?
        .get(&owner)
        .ok_or_else(|| format!("{}: no reviewed span in semantic/regions.json", owner.id()))?
        as i64;
    let paths = SourcePaths::load_for_game(root, target.compiler.as_str())?;
    let place = default_place(&paths, &overlay);
    let draft = root
        .join(target.recon_dir())
        .join("en/overlays")
        .join(format!("{}.c", owner.legacy_stem()));
    let drafted = if draft.is_file() {
        Some(fs::read_to_string(&draft).map_err(|e| format!("{}: {e}", draft.display()))?)
    } else {
        None
    };
    let (drafted_name, drafted_source) = drafted.as_deref().map(drafted_as).unwrap_or_default();
    let name = options
        .name
        .clone()
        .or_else(|| paths.registered_name(owner).map(str::to_string))
        .or(drafted_name)
        .unwrap_or_else(|| default_name(&place, owner));
    let relative_source = options
        .source
        .as_deref()
        .map(|path| source_relative(target, path))
        .or_else(|| {
            paths
                .mapped_relative_path(owner)
                .map(|path| path.to_string_lossy().into_owned())
        })
        .or(drafted_source)
        .unwrap_or_else(|| default_source(&place, owner));
    let source = root
        .join(target.game_dir())
        .join("SRC")
        .join(&relative_source);
    if source.exists() {
        return Err(format!(
            "{} exists; a trial writes a new source (choose --source)",
            source.display()
        ));
    }
    let (text, from, calls) = match drafted.filter(|_| !options.decompile) {
        Some(text) => {
            let text = names::rename_function(&text, owner.address(), &name).ok_or_else(|| {
                format!("{} defines no function; use --decompile", draft.display())
            })?;
            (text, "draft", 0)
        }
        None => {
            let context = Context::load(root, target, owner)?;
            let (text, calls) = context.decompile(root, span as u32, &name)?;
            (text, "decompile", calls)
        }
    };
    let listing = root.join(target.overlay_assembly(&overlay));
    let units = units_path(root, target);
    let register = register_path(root, target);
    let record = record_path(root, owner);
    let snapshot = Snapshot::take(&[
        units.clone(),
        register.clone(),
        listing.clone(),
        source.clone(),
        record.clone(),
    ])?;
    let unit_id = format!(
        "scene-{}-{}",
        overlay.trim_start_matches("resource_"),
        owner.address_stem()
    );
    let result = (|| {
        let listing_text =
            fs::read_to_string(&listing).map_err(|e| format!("{}: {e}", listing.display()))?;
        let splice = crate::overlay::splice_placeholder(&listing, &listing_text, owner, span)?;
        fs::write(&listing, &splice.text).map_err(|error| error.to_string())?;
        let mut document = read_json(&units)?;
        let list = document["units"]
            .as_array_mut()
            .ok_or("translation units must be an array")?;
        let address = format!("0x{}", owner.address_stem());
        let mut displaced = Vec::new();
        let mut index = 0;
        while index < list.len() {
            if declares(&list[index], &overlay, &address)? {
                displaced.push((index + displaced.len(), list.remove(index)));
            } else {
                index += 1;
            }
        }
        if list
            .iter()
            .any(|unit| unit["id"].as_str() == Some(&unit_id))
        {
            return Err(format!("unit {unit_id} already exists for another owner"));
        }
        list.push(json!({
            "id": unit_id,
            "source": relative(root, &source),
            "overlay": overlay,
            "absolute_symbols": {},
            "owners": [{"address": address}],
        }));
        write_json(&units, &document)?;
        let mut register_document = read_json(&register)?;
        let owners = register_document["owners"]
            .as_object_mut()
            .ok_or("source-path owners must be an object")?;
        let previous = owners.get(&owner.id()).cloned();
        let mut entry = match &previous {
            Some(Value::Object(record)) => record.clone(),
            _ => serde_json::Map::new(),
        };
        entry.insert("name".into(), Value::String(name.clone()));
        entry.insert("source".into(), Value::String(relative_source.clone()));
        owners.insert(owner.id(), Value::Object(entry));
        write_json(&register, &register_document)?;
        SourcePaths::load_for_game(root, target.compiler.as_str())?;
        if let Some(parent) = source.parent() {
            fs::create_dir_all(parent).map_err(|error| error.to_string())?;
        }
        fs::write(&source, &text).map_err(|error| format!("{}: {error}", source.display()))?;
        let trial = Trial {
            owner: owner.id(),
            target: target.id.as_str().to_string(),
            span,
            name: name.clone(),
            source: relative(root, &source),
            unit: unit_id.clone(),
            record: previous,
            units: displaced,
            placeholder: splice.placeholder,
            removed: splice.removed,
        };
        fs::create_dir_all(record.parent().unwrap()).map_err(|error| error.to_string())?;
        write_json(
            &record,
            &serde_json::to_value(&trial).map_err(|e| e.to_string())?,
        )?;
        crate::compiler::translation_units::TranslationUnits::load_game(root, target.compiler)?;
        Ok(trial)
    })();
    match result {
        Ok(trial) => Ok((trial, from, calls)),
        Err(error) => {
            snapshot.restore();
            let _ = remove_source(root, target, &source);
            Err(format!("{error}; nothing was changed"))
        }
    }
}

/// The score lines of one `alchemy score` of `source` against `owner`.
pub fn score_summary(root: &Path, source: &str, owner: &str) -> Result<String, String> {
    let executable = std::env::current_exe().map_err(|error| error.to_string())?;
    let output = Command::new(executable)
        .current_dir(root)
        .args(["score", source, "--owner", owner])
        .output()
        .map_err(|error| error.to_string())?;
    let text = format!(
        "{}{}",
        String::from_utf8_lossy(&output.stdout),
        String::from_utf8_lossy(&output.stderr)
    );
    Ok(text
        .lines()
        .filter(|line| {
            line.starts_with("candidate=")
                || line.contains("halfword_edits=")
                || (line.contains("error") && !line.starts_with("warning"))
                || line.contains("undefined")
        })
        .take(8)
        .map(|line| format!("{line}\n"))
        .collect())
}

#[cfg(test)]
mod tests {
    use super::*;

    fn args(items: &[&str]) -> Vec<String> {
        items.iter().map(|item| item.to_string()).collect()
    }

    #[test]
    fn parses_owner_name_source_and_modes() {
        let options = parse(&args(&[
            "resource_3ca:02000194",
            "--name",
            "A_B",
            "--source",
            "FIELD/X/Y.C",
            "--decompile",
        ]))
        .unwrap()
        .unwrap();
        assert_eq!(options.owner, "resource_3ca:02000194");
        assert_eq!(options.name.as_deref(), Some("A_B"));
        assert_eq!(options.source.as_deref(), Some("FIELD/X/Y.C"));
        assert!(options.decompile && !options.undo);
        assert!(
            parse(&args(&["resource_3ca:02000194", "--undo"]))
                .unwrap()
                .unwrap()
                .undo
        );
        assert!(parse(&args(&["--help"])).unwrap().is_none());
        assert!(parse(&args(&[])).is_err());
        assert!(parse(&args(&["a", "b"])).is_err());
        assert!(parse(&args(&["a", "--force"])).is_err());
        assert!(overlay_owner("main:08001000").is_err());
        assert!(overlay_owner("resource_3ca:02000194").is_ok());
    }

    #[test]
    fn decompile_uses_loaded_literal_values() {
        // A negative constant has the bit pattern of a Thumb BL pair; the
        // overlay encoder transforms it even though code only loads it.
        let mut runtime = vec![0; 0x10c];
        for (at, half) in [
            (0x100, 0xb500u16),
            (0x102, 0x4801),
            (0x104, 0xbc02),
            (0x106, 0x4708),
        ] {
            runtime[at..at + 2].copy_from_slice(&half.to_le_bytes());
        }
        runtime[0x108..].copy_from_slice(&(-0xc00i32).to_le_bytes());
        let stored = crate::compiler::overlay::encode(&runtime, 0).unwrap();
        assert_ne!(&stored[0x108..], &runtime[0x108..]);
        let (source, calls) = decompile_source(
            &stored,
            0x02000100,
            12,
            &[],
            &Dictionary::default(),
            "Scene_ReadStep",
        )
        .unwrap();
        assert_eq!(calls, 0);
        assert!(source.contains("0xfffff400"), "{source}");
        assert!(source.contains("-0xc00"), "{source}");
    }

    #[test]
    fn decompile_imports_resolve_per_site_before_lifting() {
        use psynergy::decode::{Ins, Kind};

        let pseudo = 0x02000380;
        let mut ins = vec![
            Ins {
                addr: 0x02000100,
                size: 2,
                kind: Kind::Push { lr: true, list: 0 },
                text: "push {lr}".into(),
            },
            Ins {
                addr: 0x02000102,
                size: 4,
                kind: Kind::Bl { target: pseudo },
                text: format!("bl 0x{pseudo:08x}"),
            },
            Ins {
                addr: 0x02000106,
                size: 4,
                kind: Kind::Bl { target: pseudo },
                text: format!("bl 0x{pseudo:08x}"),
            },
            Ins {
                addr: 0x0200010a,
                size: 4,
                kind: Kind::Bl { target: pseudo },
                text: format!("bl 0x{pseudo:08x}"),
            },
            Ins {
                addr: 0x0200010e,
                size: 4,
                kind: Kind::Bl { target: pseudo },
                text: format!("bl 0x{pseudo:08x}"),
            },
            Ins {
                addr: 0x02000112,
                size: 2,
                kind: Kind::Pop { pc: true, list: 0 },
                text: "pop {pc}".into(),
            },
        ];
        let calls = [
            ResolvedCall {
                site: 0x02000102,
                target: 0x02005000,
                name: "Engine_EventWait".into(),
            },
            ResolvedCall {
                site: 0x02000106,
                target: 0x02005020,
                name: "Engine_ActorGet".into(),
            },
            ResolvedCall {
                site: 0x0200010a,
                target: 0x02005000,
                name: "Engine_EventWait".into(),
            },
        ];

        let names = resolve_calls(&mut ins, &calls);

        assert_eq!(ins[1].kind, Kind::Bl { target: 0x02005000 });
        assert_eq!(ins[2].kind, Kind::Bl { target: 0x02005020 });
        assert_eq!(ins[3].kind, Kind::Bl { target: 0x02005000 });
        assert_eq!(ins[4].kind, Kind::Bl { target: pseudo });
        assert_eq!(names.len(), 2);
        assert_eq!(names["Func_02005000"], "Engine_EventWait");
        assert_eq!(names["Func_02005020"], "Engine_ActorGet");

        let (body, tables) = psynergy::unit::bodies(&ins, &|_, _| None);
        let source = psynergy::unit::compose(0x02000100, "Function", &body, &tables);
        let source = names::convert(&source, &names, "Scene_TestImports");
        let function = source.split("void Scene_TestImports(void)").nth(1).unwrap();
        assert_eq!(function.matches("Engine_EventWait();").count(), 2);
        assert_eq!(function.matches("Engine_ActorGet();").count(), 1);
        assert_eq!(
            function.matches(&format!("Func_{pseudo:08x}();")).count(),
            1
        );
    }

    #[test]
    fn defaults_follow_the_overlays_place() {
        let root = tempfile::tempdir().unwrap();
        let register = json!({"format": 3, "owners": {
            "resource_3ca:02000100": {"name": "BabiFune_Setup", "source": "FIELD/BABI_FUNE/SETUP.C"},
            "resource_3ca:02000200": "FIELD/BABI_FUNE/RUN.C",
            "resource_3ca:02000300": "FIELD/OTHER/X.C",
            "resource_3cb:02000100": "MENU/LINK_LOBBY/Y.C"
        }});
        let paths = SourcePaths::parse(root.path(), &register.to_string()).unwrap();
        let owner = SourceOwner::parse("resource_3ca:02000194").unwrap();
        let place = default_place(&paths, "resource_3ca");
        assert_eq!(place, "FIELD/BABI_FUNE");
        assert_eq!(default_name(&place, owner), "BabiFune_Func02000194");
        assert_eq!(default_source(&place, owner), "FIELD/BABI_FUNE/F_00194.C");
        assert_eq!(default_place(&paths, "resource_3cc"), "FIELD/TMP_3CC");
        let tbs = decomp_target(None).unwrap();
        assert_eq!(
            source_relative(tbs, "games/THE BROKEN SEAL/SRC/FIELD/A.C"),
            "FIELD/A.C"
        );
        assert_eq!(source_relative(tbs, "FIELD/A.C"), "FIELD/A.C");
    }

    #[test]
    fn drafts_give_back_the_name_and_path_they_were_meant_for() {
        let draft = "/* NONMATCHING: 554 bytes (2026-09-25). BabiFune_Run, meant for\n * FIELD/BABI_FUNE/RUN.C as a single-overlay unit. */\nstatic void helper(void)\n{\n}\nvoid BabiFune_Run(void)\n{\n}\n";
        assert_eq!(
            drafted_as(draft),
            (
                Some("BabiFune_Run".into()),
                Some("FIELD/BABI_FUNE/RUN.C".into())
            )
        );
        assert_eq!(drafted_as("void Func_02000194(void)\n{\n}\n"), (None, None));
    }

    #[test]
    fn unsplice_restores_exactly_the_displaced_lines() {
        let listing = "a:\n\tpush {lr}\n\tbx lr\nb:\n\tnop";
        let placeholder = vec!["AlchemyC_02000004:".to_string(), "\t.space 0x4".to_string()];
        let removed = vec!["\tpush {lr}".to_string(), "\tbx lr".to_string()];
        let spliced = "a:\nAlchemyC_02000004:\n\t.space 0x4\nb:\n\tnop";
        assert_eq!(unsplice(spliced, &placeholder, &removed).unwrap(), listing);
        assert!(unsplice(listing, &placeholder, &removed)
            .unwrap_err()
            .contains("0 AlchemyC_02000004: lines"));
        let changed = "a:\nAlchemyC_02000004:\n\t.space 0x6\nb:";
        assert!(unsplice(changed, &placeholder, &removed)
            .unwrap_err()
            .contains("not the trial's placeholder"));
    }

    #[test]
    fn instance_members_are_refused_and_canonical_owners_found() {
        let unit = json!({"id": "u", "overlay": "resource_3bf", "owners": [{"address": "0x0200034c"}],
            "instances": {"resource_389": {"owners": {"F": {"address": "0x0200034c"}}}}});
        assert!(declares(&unit, "resource_3bf", "0x0200034c").unwrap());
        assert!(!declares(&unit, "resource_3bf", "0x02000400").unwrap());
        assert!(declares(&unit, "resource_389", "0x0200034c")
            .unwrap_err()
            .contains("instance member of unit u"));
    }

    /// A repository with one overlay listing, register and unit file.
    fn repository() -> tempfile::TempDir {
        let root = tempfile::tempdir().unwrap();
        let write = |path: &str, text: &str| {
            let path = root.path().join(path);
            fs::create_dir_all(path.parent().unwrap()).unwrap();
            fs::write(path, text).unwrap();
        };
        write(
            "recon/tbs/raw/overlays/resource_3ca_overlay.s",
            "\tpush {lr}\n\tbx lr\n",
        );
        write(
            "recon/tbs/source-paths.json",
            &(serde_json::to_string_pretty(&json!({"format": 3, "owners": {
                "resource_3ca:02000000": {"name": "Named", "call_via": "02000100"},
                "resource_3ca:02000200": "FIELD/X/KEEP.C"
            }}))
            .unwrap()
                + "\n"),
        );
        write(
            "recon/tbs/translation-units.json",
            &(serde_json::to_string_pretty(&json!({"format": 1, "units": [
                {"id": "first", "source": "a", "owners": [{"address": "0x08000000"}]},
                {"id": "retained-overlay-3ca-02000000", "source": "recon/tbs/en/overlays/resource_3ca_c_02000000.c",
                 "overlay": "resource_3ca", "owners": [{"address": "0x02000000"}]},
                {"id": "last", "source": "b", "owners": [{"address": "0x08000010"}]}
            ]}))
            .unwrap() + "\n"),
        );
        root
    }

    #[test]
    fn undo_restores_listing_register_units_and_removes_the_source() {
        let root = repository();
        let root = root.path();
        let target = decomp_target(None).unwrap();
        let owner = SourceOwner::parse("resource_3ca:02000000").unwrap();
        let before = [units_path(root, target), register_path(root, target)]
            .map(|path| fs::read_to_string(path).unwrap());
        let listing = root.join("recon/tbs/raw/overlays/resource_3ca_overlay.s");
        // What `open` does, with the placeholder computed by hand.
        let mut units = read_json(&units_path(root, target)).unwrap();
        let displaced = units["units"].as_array_mut().unwrap().remove(1);
        units["units"].as_array_mut().unwrap().push(json!({"id": "scene-3ca-02000000", "source": "games/THE BROKEN SEAL/SRC/FIELD/NEW/F_00000.C", "overlay": "resource_3ca", "absolute_symbols": {}, "owners": [{"address": "0x02000000"}]}));
        write_json(&units_path(root, target), &units).unwrap();
        let mut register = read_json(&register_path(root, target)).unwrap();
        let record = register["owners"]["resource_3ca:02000000"].clone();
        register["owners"]["resource_3ca:02000000"] =
            json!({"name": "Named", "call_via": "02000100", "source": "FIELD/NEW/F_00000.C"});
        write_json(&register_path(root, target), &register).unwrap();
        fs::write(&listing, "AlchemyC_02000000:\n\t.space 0x4\n").unwrap();
        let source = root.join("games/THE BROKEN SEAL/SRC/FIELD/NEW/F_00000.C");
        fs::create_dir_all(source.parent().unwrap()).unwrap();
        fs::write(&source, "void Named(void) {}\n").unwrap();
        let trial = Trial {
            owner: owner.id(),
            target: "tbs-en".into(),
            span: 4,
            name: "Named".into(),
            source: "games/THE BROKEN SEAL/SRC/FIELD/NEW/F_00000.C".into(),
            unit: "scene-3ca-02000000".into(),
            record: Some(record),
            units: vec![(1, displaced)],
            placeholder: vec!["AlchemyC_02000000:".into(), "\t.space 0x4".into()],
            removed: vec!["\tpush {lr}".into(), "\tbx lr".into()],
        };
        fs::create_dir_all(root.join("out/trial")).unwrap();
        write_json(
            &record_path(root, owner),
            &serde_json::to_value(&trial).unwrap(),
        )
        .unwrap();
        assert_eq!(Trial::open(root, owner).unwrap().as_ref(), Some(&trial));
        trial.undo(root).unwrap();
        let after = [units_path(root, target), register_path(root, target)]
            .map(|path| fs::read_to_string(path).unwrap());
        assert_eq!(after, before);
        assert_eq!(
            fs::read_to_string(&listing).unwrap(),
            "\tpush {lr}\n\tbx lr\n"
        );
        assert!(!source.exists() && !root.join("games/THE BROKEN SEAL/SRC/FIELD").exists());
        assert!(Trial::open(root, owner).unwrap().is_none());
        assert!(Trial::require(root, owner)
            .unwrap_err()
            .contains("no open trial"));
    }
}
