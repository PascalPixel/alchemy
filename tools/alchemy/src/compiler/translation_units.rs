use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_inputs::quoted_include;
use crate::compiler::source_paths::{c_identifier, lower_hex, SourceOwner, SourcePaths};
use serde::{de::Error, Deserialize, Deserializer};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Component, Path, PathBuf};
pub const FORMAT: u32 = 4;

#[derive(Deserialize)]
struct ReviewedRegions {
    manual_regions: Vec<ReviewedRegion>,
}
#[derive(Deserialize)]
struct ReviewedRegion {
    overlay: String,
    entry: String,
    span_bytes: usize,
}
pub fn reviewed_overlay_spans(root: &Path) -> Result<BTreeMap<SourceOwner, usize>, String> {
    let path = root.join("games/gs1/semantic/regions.json");
    let document: ReviewedRegions = crate::compiler::build_io::read_json(path)?;
    let mut spans = BTreeMap::new();
    for region in document.manual_regions {
        let owner = SourceOwner::parse(&format!(
            "{}:{}",
            region.overlay,
            region.entry.trim_start_matches("0x")
        ))?;
        if region.span_bytes == 0 {
            return Err(format!(
                "{} has no positive reviewed span_bytes",
                owner.id()
            ));
        }
        if spans.insert(owner, region.span_bytes).is_some() {
            return Err(format!(
                "{} has duplicate reviewed owner entries",
                owner.id()
            ));
        }
    }
    Ok(spans)
}

/// A requested span is a constraint, never evidence of a function boundary.
/// `installed_span` must come from a source-backed production C placeholder.
pub fn resolve_overlay_span(
    reviewed: &BTreeMap<SourceOwner, usize>,
    owner: SourceOwner,
    installed_span: Option<usize>,
    requested_span: Option<usize>,
) -> Result<usize, String> {
    let overlay = owner.overlay_id().ok_or("expected an overlay owner")?;
    let reviewed_span = reviewed.get(&owner).copied();
    let span = installed_span.or(reviewed_span).ok_or_else(|| {
        format!(
            "{} has no reviewed complete owner boundary; --span cannot establish one",
            owner.id()
        )
    })?;
    let end = u32::try_from(span)
        .ok()
        .filter(|span| *span > 0)
        .and_then(|span| owner.address().checked_add(span))
        .ok_or_else(|| format!("{} has an invalid owner extent", owner.id()))?;
    if requested_span.is_some_and(|requested| requested != span) {
        return Err(format!(
            "{}: requested span {} differs from complete {} extent {span}",
            owner.id(),
            requested_span.unwrap(),
            if installed_span.is_some() {
                "installed"
            } else {
                "reviewed"
            }
        ));
    }
    if let Some((other, _)) = reviewed.iter().find(|(other, extent)| {
        **other != owner
            && other.overlay_id().as_deref() == Some(&overlay)
            && other.address() < end
            && u64::from(owner.address()) < u64::from(other.address()) + **extent as u64
    }) {
        return Err(format!(
            "{} overlaps reviewed owner {}",
            owner.id(),
            other.id()
        ));
    }
    if installed_span.is_some() && reviewed_span.is_some_and(|reviewed| reviewed != span) {
        eprintln!("{}: installed extent {span} differs from reviewed extent {}; preserving existing C extent pending boundary audit", owner.id(), reviewed_span.unwrap());
    }
    Ok(span)
}

#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq)]
#[serde(rename_all = "kebab-case")]
pub enum AbsoluteSymbolKind {
    Data,
    Thumb,
    Arm,
}
#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct AbsoluteSymbol {
    #[serde(deserialize_with = "hex64")]
    pub address: u64,
    pub kind: AbsoluteSymbolKind,
}
#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq)]
#[serde(rename_all = "kebab-case")]
pub enum OwnerState {
    ExactC,
    RetainedAssembly,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct TranslationOwner {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
    #[serde(skip)]
    pub canonical_name: String,
    pub extent: usize,
    pub state: OwnerState,
}
#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct TranslationSymbol {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
    #[serde(skip)]
    pub canonical_name: String,
    pub extent: usize,
}
#[derive(Clone, Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct TranslationUnit {
    pub id: String,
    pub game: String,
    pub source: PathBuf,
    pub compiler_route: String,
    pub overlay: Option<String>,
    #[serde(default)]
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
    #[serde(default)]
    pub local_symbols: Vec<TranslationSymbol>,
    pub owners: Vec<TranslationOwner>,
}

impl TranslationUnit {
    pub fn target(&self) -> Result<CompilerTarget, String> {
        match self.game.as_str() {
            "gs1" => Ok(CompilerTarget::Gs1),
            "gs2" => Ok(CompilerTarget::Gs2),
            _ => Err(format!("{}: unsupported game {}", self.id, self.game)),
        }
    }
    pub fn source_owner(&self, address: u32) -> Result<SourceOwner, String> {
        match &self.overlay {
            Some(resource) if (0x0200_0000..0x0300_0000).contains(&address) => {
                SourceOwner::parse(&format!("{resource}:{address:08x}"))
            }
            Some(_) => Err(format!("overlay owner 0x{address:08x} is outside EWRAM")),
            None if address >= 0x0800_0000 => Ok(SourceOwner::Main(address)),
            None => Err(format!("main owner address 0x{address:08x} is below ROM")),
        }
    }
    pub fn exact(&self) -> bool {
        self.exact_owner_count() == self.owners.len()
    }
    pub fn exact_owner_count(&self) -> usize {
        self.owners
            .iter()
            .filter(|owner| owner.state == OwnerState::ExactC)
            .count()
    }
    pub fn symbols(&self) -> impl Iterator<Item = (u32, &str, usize)> {
        self.owners
            .iter()
            .map(|symbol| {
                (
                    symbol.address,
                    symbol.canonical_name.as_str(),
                    symbol.extent,
                )
            })
            .chain(self.local_symbols.iter().map(|symbol| {
                (
                    symbol.address,
                    symbol.canonical_name.as_str(),
                    symbol.extent,
                )
            }))
    }
    pub fn composition_sections(&self) -> BTreeMap<String, Vec<String>> {
        let mut ordered = self.symbols().collect::<Vec<_>>();
        ordered.sort_unstable_by_key(|member| member.0);
        BTreeMap::from([(
            ".text".into(),
            ordered
                .into_iter()
                .map(|(_, name, _)| name.into())
                .collect(),
        )])
    }
    pub fn canonical_symbols(&self) -> Result<BTreeMap<String, AbsoluteSymbol>, String> {
        let mut symbols = self.absolute_symbols.clone();
        for (address, _, _) in self.symbols() {
            symbols.insert(
                self.source_owner(address)?.legacy_name(),
                AbsoluteSymbol {
                    address: u64::from(address)
                        + if self.overlay.is_some() {
                            u64::from(
                                crate::compiler::overlay::RUNTIME_BASE
                                    - crate::compiler::overlay::RESOURCE_BASE,
                            )
                        } else {
                            0
                        },
                    kind: AbsoluteSymbolKind::Thumb,
                },
            );
        }
        Ok(symbols)
    }
}

#[derive(Clone, Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct TranslationUnits {
    format: u32,
    kind: String,
    original_translation_units: String,
    pub units: Vec<TranslationUnit>,
}
impl TranslationUnits {
    pub fn load(root: &Path) -> Result<Self, String> {
        let path = root.join("games/gs1/recon/translation-units.json");
        let mut document: Self = crate::compiler::build_io::read_json(&path)?;
        if document.format != FORMAT
            || document.kind != "reconstruction-composition-contracts"
            || document.original_translation_units != "unknown"
        {
            return Err(format!(
                "{}: expected reconstruction-composition format {FORMAT}",
                path.display()
            ));
        }
        let mut ids = BTreeSet::new();
        let mut claimed = BTreeSet::new();
        let mut main_aliases = BTreeSet::new();
        for unit in &mut document.units {
            if !unit_id(&unit.id)
                || !ids.insert(&unit.id)
                || unit.compiler_route != "canonical-gcc296"
                || unit.owners.is_empty()
            {
                return Err(format!(
                    "{}: invalid id, route, or empty owner list",
                    unit.id
                ));
            }
            if unit.source.is_absolute()
                || unit
                    .source
                    .components()
                    .any(|component| matches!(component, Component::ParentDir))
                || !root.join(&unit.source).is_file()
            {
                return Err(format!(
                    "{}: source must be an existing repository-relative file",
                    unit.id
                ));
            }
            let names = SourcePaths::load_for_game(root, unit.target()?.as_str())?;
            let mut canonical_names = unit
                .symbols()
                .map(|(address, _, _)| {
                    let owner = unit.source_owner(address)?;
                    names
                        .registered_name(owner)
                        .map(str::to_owned)
                        .ok_or_else(|| format!("{}: {} is not registered", unit.id, owner.id()))
                })
                .collect::<Result<Vec<_>, String>>()?
                .into_iter();
            for member in &mut unit.owners {
                member.canonical_name = canonical_names.next().expect("one name per symbol");
            }
            for member in &mut unit.local_symbols {
                member.canonical_name = canonical_names.next().expect("one name per symbol");
            }
            if unit
                .owners
                .windows(2)
                .any(|pair| pair[1].address <= pair[0].address)
            {
                return Err(format!("{}: owners are not strictly ordered", unit.id));
            }
            let mut members = BTreeSet::new();
            let owner_count = unit.owners.len();
            for (index, (address, alias, extent)) in unit.symbols().enumerate() {
                let is_owner = index < owner_count;
                let source_owner = unit.source_owner(address)?;
                if extent == 0
                    || !c_identifier(alias)
                    || names.registered_name(source_owner) != Some(alias)
                {
                    return Err(format!(
                        "{}: {} has a noncanonical alias or extent",
                        unit.id,
                        source_owner.id()
                    ));
                }
                if !members.insert(source_owner) || (is_owner && !claimed.insert(source_owner)) {
                    let kind = if is_owner { "owner" } else { "local symbol" };
                    return Err(format!(
                        "{}: duplicate {kind} {}",
                        unit.id,
                        source_owner.id()
                    ));
                }
                if unit.overlay.is_none() && !main_aliases.insert(alias) {
                    return Err(format!("duplicate main symbol alias {alias}"));
                }
            }
            let mut spans = unit
                .symbols()
                .map(|(address, _, extent)| (address, extent))
                .collect::<Vec<_>>();
            spans.sort_unstable();
            let end = |(address, extent): &(u32, usize)| {
                u32::try_from(*extent)
                    .ok()
                    .and_then(|extent| address.checked_add(extent))
            };
            if spans.iter().any(|span| end(span).is_none())
                || spans
                    .windows(2)
                    .any(|pair| end(&pair[0]).is_some_and(|end| end > pair[1].0))
            {
                return Err(format!(
                    "{}: declared symbol extents overlap or overflow",
                    unit.id
                ));
            }
            for (name, symbol) in &unit.absolute_symbols {
                if !c_identifier(name)
                    || (symbol.kind != AbsoluteSymbolKind::Data && symbol.address & 1 != 0)
                    || (unit.overlay.is_some()
                        && (symbol.kind == AbsoluteSymbolKind::Arm
                            || names.main_symbol(name)?.is_some()))
                {
                    return Err(format!("{}: invalid absolute symbol {name:?}", unit.id));
                }
            }
            let source = root.join(&unit.source);
            let grouped = source.starts_with(names.source_root());
            validate_production_state(root, unit, &source, grouped, &names)?;
        }
        Ok(document)
    }
    pub fn unit(&self, id: &str) -> Option<&TranslationUnit> {
        self.units.iter().find(|unit| unit.id == id)
    }
    pub fn unit_for_game_owner(&self, game: &str, owner: SourceOwner) -> Option<&TranslationUnit> {
        let overlay = owner.overlay_id();
        self.units.iter().find(|unit| {
            unit.game == game
                && unit.overlay.as_deref() == overlay.as_deref()
                && unit
                    .owners
                    .iter()
                    .any(|member| member.address == owner.address())
        })
    }
}
fn validate_production_state(
    root: &Path,
    unit: &TranslationUnit,
    source: &Path,
    grouped: bool,
    names: &SourcePaths,
) -> Result<(), String> {
    let retained_overlay_candidate = unit.overlay.is_some()
        && !grouped
        && unit.local_symbols.is_empty()
        && unit
            .owners
            .iter()
            .all(|owner| owner.state == OwnerState::RetainedAssembly)
        && source.starts_with(
            root.join("games")
                .join(&unit.game)
                .join("recon/en/overlays"),
        );
    if unit.exact() && !grouped {
        return Err(format!(
            "{}: complete exact C must use its declared TU source",
            unit.id
        ));
    }
    if unit.overlay.is_some() && !(grouped && unit.exact()) && !retained_overlay_candidate {
        return Err(format!(
            "{}: overlay units must be wholly exact grouped C under the source root",
            unit.id
        ));
    }
    if grouped && unit.overlay.is_some() {
        let mapped = names
            .owners_for_path(source)
            .into_iter()
            .collect::<BTreeSet<_>>();
        let declared = unit
            .owners
            .iter()
            .map(|member| unit.source_owner(member.address))
            .collect::<Result<BTreeSet<_>, _>>()?;
        if mapped != declared {
            return Err(format!(
                "{}: grouped overlay source and owners disagree",
                unit.id
            ));
        }
    }
    let requires_direct = unit.overlay.is_none() && !unit.exact();
    let direct_includes = if requires_direct {
        let parent = source.parent().unwrap_or(root);
        let text = std::fs::read_to_string(source)
            .map_err(|error| format!("{}: {error}", source.display()))?;
        unconditional_quoted_includes(&text)
            .into_iter()
            .filter_map(|name| parent.join(name).canonicalize().ok())
            .collect()
    } else {
        BTreeSet::new()
    };
    let placeholders = unit
        .overlay
        .as_ref()
        .map(|overlay| {
            let assembly = root
                .join("games")
                .join(&unit.game)
                .join("asm/overlays")
                .join(format!("{overlay}_overlay.s"));
            std::fs::read_to_string(&assembly)
                .map_err(|error| format!("{}: {error}", assembly.display()))
                .map(|text| crate::compiler::overlay::placeholder_addresses(&text))
        })
        .transpose()?;
    let reviewed = if retained_overlay_candidate {
        reviewed_overlay_spans(root)?
    } else {
        BTreeMap::new()
    };
    for member in &unit.owners {
        let owner = unit.source_owner(member.address)?;
        let mapped = names.mapped_source_path(owner);
        let retained = placeholders.as_ref().map_or_else(
            || {
                root.join("games")
                    .join(&unit.game)
                    .join("asm")
                    .join(format!("{:08x}.s", member.address))
                    .is_file()
            },
            |set| !set.contains(&member.address),
        );
        if retained_overlay_candidate {
            resolve_overlay_span(&reviewed, owner, None, Some(member.extent))?;
            if mapped.is_some() || !retained {
                return Err(format!(
                    "{}: {} is not a complete unmapped reviewed retained overlay owner",
                    unit.id,
                    owner.id()
                ));
            }
        }
        let exact_source = if grouped && !requires_direct {
            mapped.as_ref().is_some_and(|path| path == source)
                || (unit.overlay.is_none() && mapped.is_none())
        } else {
            mapped.as_ref().is_some_and(|path| {
                path.is_file()
                    && (!requires_direct
                        || path
                            .canonicalize()
                            .is_ok_and(|path| direct_includes.contains(&path)))
            })
        };
        if requires_direct && matches!(member.state, OwnerState::RetainedAssembly) {
            let parent = source.parent().unwrap_or(root);
            let candidate = parent.join(format!("../main/{:08x}.c", member.address));
            if !candidate
                .canonicalize()
                .is_ok_and(|path| direct_includes.contains(&path))
            {
                return Err(format!(
                    "{}: {} retained C body must be included from ../main/{:08x}.c",
                    unit.id,
                    owner.id(),
                    member.address
                ));
            }
        }
        let valid = match member.state {
            OwnerState::ExactC => exact_source && !retained,
            OwnerState::RetainedAssembly => mapped.is_none() && retained,
        };
        if !valid {
            return Err(format!(
                "{}: {} state disagrees with production C/assembly ownership",
                unit.id,
                owner.id()
            ));
        }
    }
    Ok(())
}
fn unconditional_quoted_includes(source: &str) -> Vec<&str> {
    let (mut comments, mut continued, mut names) = (0usize, false, Vec::new());
    for raw in source.lines() {
        let was_comment = comments != 0;
        comments = comments
            .saturating_add(raw.matches("/*").count())
            .saturating_sub(raw.matches("*/").count());
        let line = raw.trim_start();
        let directive = line
            .strip_prefix('#')
            .and_then(|line| line.trim_start().split_ascii_whitespace().next());
        let allowed = matches!(directive, Some("include" | "define" | "undef"));
        if line.contains("%:")
            || line.contains("??=")
            || (line.contains('#') && (was_comment || comments != 0 || continued || !allowed))
        {
            return Vec::new();
        }
        if !was_comment && comments == 0 && !continued {
            names.extend(quoted_include(line));
        }
        continued = raw.trim_end().ends_with('\\');
    }
    names
}
fn unit_id(value: &str) -> bool {
    !value.is_empty()
        && value
            .bytes()
            .all(|byte| byte == b'-' || byte.is_ascii_lowercase() || byte.is_ascii_digit())
}
fn hex32<'de, D: Deserializer<'de>>(deserializer: D) -> Result<u32, D::Error> {
    u32::try_from(hex64(deserializer)?).map_err(D::Error::custom)
}
fn hex64<'de, D: Deserializer<'de>>(deserializer: D) -> Result<u64, D::Error> {
    let value = String::deserialize(deserializer)?;
    let body = value
        .strip_prefix("0x")
        .filter(|body| !body.is_empty() && lower_hex(body))
        .ok_or_else(|| D::Error::custom("expected 0x-prefixed lowercase hexadecimal"))?;
    u64::from_str_radix(body, 16).map_err(D::Error::custom)
}
#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn reviewed_owner_duplicates_never_select_the_last_extent() {
        let root = tempfile::tempdir().unwrap();
        let path = root.path().join("games/gs1/semantic/regions.json");
        std::fs::create_dir_all(path.parent().unwrap()).unwrap();
        for sizes in [[4, 4], [4, 8]] {
            let rows = sizes.map(|span| {
                serde_json::json!({
                    "overlay": "resource_371", "entry": "0x02000100", "span_bytes": span
                })
            });
            std::fs::write(
                &path,
                serde_json::json!({"manual_regions": rows}).to_string(),
            )
            .unwrap();
            assert!(reviewed_overlay_spans(root.path())
                .unwrap_err()
                .contains("duplicate"));
        }
    }
    #[test]
    fn supplied_overlay_spans_cannot_establish_or_resize_owners() {
        let owner = SourceOwner::parse("resource_371:02000100").unwrap();
        let other = SourceOwner::parse("resource_371:02000120").unwrap();
        let reviewed = BTreeMap::from([(owner, 0x20), (other, 0x10)]);
        let resolve = |entry, installed, requested| {
            resolve_overlay_span(&reviewed, entry, installed, requested)
        };
        assert_eq!(resolve(owner, None, None).unwrap(), 0x20);
        assert_eq!(resolve(owner, None, Some(0x20)).unwrap(), 0x20);
        for span in [0, 2, 0x1e, 0x22, 0x30, usize::MAX] {
            assert!(resolve(owner, None, Some(span)).is_err());
        }
        for id in [
            "resource_371:02000110",
            "resource_372:02000100",
            "main:08000100",
        ] {
            assert!(resolve(SourceOwner::parse(id).unwrap(), None, Some(0x10)).is_err());
        }
        // Existing installed alignment extents remain explicit, not silent review rewrites.
        assert_eq!(resolve(owner, Some(0x1e), Some(0x1e)).unwrap(), 0x1e);
        assert!(resolve(owner, Some(0x1e), Some(0x20)).is_err());
        assert!(resolve(owner, Some(0x22), None)
            .unwrap_err()
            .contains("overlaps"));
        let interior = SourceOwner::parse("resource_371:02000110").unwrap();
        assert!(resolve(interior, Some(4), None)
            .unwrap_err()
            .contains("overlaps"));
        let leaf = SourceOwner::parse("resource_371:02000200").unwrap();
        assert_eq!(resolve(leaf, Some(8), Some(8)).unwrap(), 8);
        assert!(resolve(leaf, None, Some(8)).is_err());
        assert!(resolve(leaf, Some(0), None).is_err());
        assert!(resolve(leaf, Some(usize::MAX), None).is_err());
    }
    #[test]
    fn loads_typed_main_and_overlay_units() {
        let manifest = TranslationUnits::load(crate::compiler::routing::root()).unwrap();
        assert!(manifest.unit("scheduler").unwrap().exact());
        let overlay = manifest.unit("scene-event-runtime").unwrap();
        assert_eq!(
            overlay.source_owner(0x0200_0030).unwrap().id(),
            "resource_37b:02000030"
        );
        assert_eq!(
            overlay.absolute_symbols["SceneEventRuntime_ScriptData"].kind,
            AbsoluteSymbolKind::Data
        );
        let owner = SourceOwner::Main(0x0800_40e8);
        assert!(manifest.unit_for_game_owner("gs1", owner).is_some());
        assert!(manifest.unit_for_game_owner("gs2", owner).is_none());
        let root = crate::compiler::routing::root();
        let names = SourcePaths::load_for_game(root, "gs1").unwrap();
        let candidate = manifest
            .unit("overlay-candidate-bindings-373-020015dc")
            .unwrap();
        assert!(!candidate.exact());
        let invalid_state = |unit: &TranslationUnit| {
            validate_production_state(root, unit, &root.join(&unit.source), false, &names).is_err()
        };
        let mut invalid = candidate.clone();
        invalid.owners[0].extent += 2;
        assert!(invalid_state(&invalid));
        let mut mixed = candidate.clone();
        mixed.owners[0].state = OwnerState::ExactC;
        assert!(invalid_state(&mixed));
        let mut installed = candidate.clone();
        installed.overlay = Some("resource_373".into());
        installed.owners[0].address = 0x0200_0f5c;
        installed.owners[0].extent = 0x30;
        assert!(invalid_state(&installed));
        invalid.source = PathBuf::from("games/gs1/src/invalid-retained-overlay.c");
        assert!(invalid_state(&invalid));
        let i = unconditional_quoted_includes;
        assert!(i("#define X \\\n#include \"x\"").is_empty());
        assert!(i("/* */ #if 0\n#include \"x\"").is_empty());
    }
}
