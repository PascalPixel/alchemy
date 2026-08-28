use crate::routing::CompilerTarget;
use crate::source_inputs::quoted_include;
use crate::source_paths::{c_identifier, lower_hex, SourceOwner, SourcePaths};
use serde::{de::Error, Deserialize, Deserializer};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Component, Path, PathBuf};
pub const FORMAT: u32 = 4;

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
                    address: u64::from(address),
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
        let mut document: Self = serde_json::from_str(
            &std::fs::read_to_string(&path)
                .map_err(|error| format!("{}: {error}", path.display()))?,
        )
        .map_err(|error| format!("{}: {error}", path.display()))?;
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
                        && (symbol.kind != AbsoluteSymbolKind::Data
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
    if unit.exact() && !grouped {
        return Err(format!(
            "{}: complete exact C must use its declared TU source",
            unit.id
        ));
    }
    if unit.overlay.is_some() && !(grouped && unit.exact()) {
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
                .join("assets/code")
                .join(format!("{overlay}_overlay.s"));
            std::fs::read_to_string(&assembly)
                .map_err(|error| format!("{}: {error}", assembly.display()))
                .map(|text| {
                    text.lines()
                        .filter_map(|line| {
                            line.trim()
                                .strip_prefix("AlchemyC_")?
                                .strip_suffix(':')
                                .and_then(|value| u32::from_str_radix(value, 16).ok())
                        })
                        .collect::<BTreeSet<_>>()
                })
        })
        .transpose()?;
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
    fn loads_typed_main_and_overlay_units() {
        let manifest = TranslationUnits::load(crate::routing::root()).unwrap();
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
        let i = unconditional_quoted_includes;
        assert!(i("#define X \\\n#include \"x\"").is_empty());
        assert!(i("/* */ #if 0\n#include \"x\"").is_empty());
    }
}
