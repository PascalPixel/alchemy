use crate::routing::CompilerTarget;
use crate::source_paths::{c_identifier, lower_hex, SourceOwner, SourcePaths};
use serde::{de::Error, Deserialize, Deserializer};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Component, Path, PathBuf};

pub const FORMAT: u32 = 2;

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
    pub alias: String,
    pub extent: usize,
    pub state: OwnerState,
}

#[derive(Clone, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct TranslationSymbol {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
    pub alias: String,
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
        let owner = match &self.overlay {
            Some(resource) => SourceOwner::parse(&format!("{resource}:{address:08x}"))?,
            None => SourceOwner::Main(address),
        };
        match owner {
            SourceOwner::Overlay { .. } if !(0x0200_0000..0x0300_0000).contains(&address) => Err(
                format!("overlay owner address 0x{address:08x} is outside EWRAM"),
            ),
            SourceOwner::Main(_) if address < 0x0800_0000 => {
                Err(format!("main owner address 0x{address:08x} is below ROM"))
            }
            _ => Ok(owner),
        }
    }

    pub fn exact(&self) -> bool {
        self.owners
            .iter()
            .all(|owner| owner.state == OwnerState::ExactC)
    }
}

#[derive(Clone, Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct TranslationUnits {
    format: u32,
    pub units: Vec<TranslationUnit>,
}

impl TranslationUnits {
    pub fn load(root: &Path) -> Result<Self, String> {
        let path = root.join("games/gs1/recon/translation-units.json");
        let document: Self = serde_json::from_str(
            &std::fs::read_to_string(&path)
                .map_err(|error| format!("{}: {error}", path.display()))?,
        )
        .map_err(|error| format!("{}: {error}", path.display()))?;
        if document.format != FORMAT {
            return Err(format!(
                "{}: expected translation-unit format {FORMAT}",
                path.display()
            ));
        }
        let mut ids = BTreeSet::new();
        let mut claimed = BTreeSet::new();
        for unit in &document.units {
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
            if unit
                .owners
                .windows(2)
                .any(|pair| pair[1].address <= pair[0].address)
            {
                return Err(format!("{}: owners are not strictly ordered", unit.id));
            }
            let owners = unit
                .owners
                .iter()
                .map(|member| (member.address, member.alias.as_str(), member.extent, true));
            let symbols = unit
                .local_symbols
                .iter()
                .map(|member| (member.address, member.alias.as_str(), member.extent, false));
            let mut members = BTreeSet::new();
            for (address, alias, extent, is_owner) in owners.chain(symbols) {
                let source_owner = unit.source_owner(address)?;
                validate_member(unit, source_owner, alias, extent, &names)?;
                if !members.insert(source_owner) || (is_owner && !claimed.insert(source_owner)) {
                    let kind = if is_owner { "owner" } else { "local symbol" };
                    return Err(format!(
                        "{}: duplicate {kind} {}",
                        unit.id,
                        source_owner.id()
                    ));
                }
            }
            let mut spans = unit
                .owners
                .iter()
                .map(|member| (member.address, member.extent))
                .chain(
                    unit.local_symbols
                        .iter()
                        .map(|member| (member.address, member.extent)),
                )
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
                {
                    return Err(format!("{}: invalid absolute symbol {name:?}", unit.id));
                }
            }
        }
        Ok(document)
    }

    pub fn unit(&self, id: &str) -> Option<&TranslationUnit> {
        self.units.iter().find(|unit| unit.id == id)
    }

    pub fn unit_for_owner(&self, owner: SourceOwner) -> Option<&TranslationUnit> {
        let overlay = owner.overlay_id();
        self.units.iter().find(|unit| {
            unit.overlay.as_deref() == overlay.as_deref()
                && unit
                    .owners
                    .iter()
                    .any(|member| member.address == owner.address())
        })
    }
}

fn validate_member(
    unit: &TranslationUnit,
    owner: SourceOwner,
    alias: &str,
    extent: usize,
    names: &SourcePaths,
) -> Result<(), String> {
    if extent == 0 || !c_identifier(alias) || names.registered_name(owner) != Some(alias) {
        return Err(format!(
            "{}: {} has a noncanonical alias or extent",
            unit.id,
            owner.id()
        ));
    }
    Ok(())
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
    }
}
