use crate::compiler::routing::CompilerTarget;
use crate::compiler::source_inputs::quoted_include;
use crate::compiler::source_paths::{
    c_identifier, lower_hex, SourceOwner, SourcePaths, SHARED_SOURCE_ROOT,
};
use serde::de::{Error, MapAccess, Visitor};
use serde::{Deserialize, Deserializer};
use std::collections::{BTreeMap, BTreeSet};
use std::path::{Component, Path, PathBuf};
pub const FORMAT: u32 = 5;
const EDITIONS: [&str; 6] = ["ja", "en", "de", "es", "fr", "it"];

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
    let path = root.join("games/THE BROKEN SEAL/semantic/regions.json");
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
#[derive(Clone, Debug, Deserialize, Default)]
#[serde(deny_unknown_fields)]
pub struct EditionLayout {
    #[serde(default, deserialize_with = "unique_keys")]
    pub owners: BTreeMap<String, EditionOwner>,
    #[serde(default, deserialize_with = "unique_keys")]
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
}
/// The unit's object linked into another image: where each member lands
/// there, and the bindings that image declares instead of pairing.
#[derive(Clone, Debug, Deserialize, Default)]
#[serde(deny_unknown_fields)]
pub struct UnitInstance {
    #[serde(deserialize_with = "unique_keys")]
    pub owners: BTreeMap<String, InstanceOwner>,
    #[serde(default, deserialize_with = "unique_keys")]
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
    #[serde(default)]
    pub compiler_gaps: Vec<CompilerGap>,
    #[serde(default, deserialize_with = "unique_keys")]
    pub editions: BTreeMap<String, EditionLayout>,
}
#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct InstanceOwner {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
    pub extent: usize,
}
/// A member where one image links it.
#[derive(Clone, Copy, Debug, Eq, PartialEq)]
pub struct PlacedMember<'a> {
    pub name: &'a str,
    /// The member's address in the canonical image.
    pub canonical: u32,
    pub address: u32,
    pub extent: usize,
    /// Owners are claimed and credited; local symbols only link.
    pub owner: bool,
}
#[derive(Clone, Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct EditionOwner {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
    pub extent: usize,
    #[serde(default)]
    pub source_variant: bool,
}
#[derive(Clone, Debug, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct CompilerGap {
    #[serde(deserialize_with = "hex32")]
    pub start: u32,
    #[serde(deserialize_with = "hex32")]
    pub end: u32,
}
/// Where an exact overlay unit's read-only data section links: the unit's
/// initialized tables compile to `.rodata`, which the placement script puts
/// at this resource address, filling the listing's `AlchemyData_` placeholder.
#[derive(Clone, Copy, Debug, Deserialize, Eq, PartialEq)]
#[serde(deny_unknown_fields)]
pub struct UnitData {
    #[serde(deserialize_with = "hex32")]
    pub address: u32,
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
    #[serde(default, deserialize_with = "unique_keys")]
    pub absolute_symbols: BTreeMap<String, AbsoluteSymbol>,
    #[serde(default, deserialize_with = "unique_keys")]
    pub editions: BTreeMap<String, EditionLayout>,
    #[serde(default, deserialize_with = "unique_keys")]
    pub instances: BTreeMap<String, UnitInstance>,
    #[serde(default)]
    pub local_symbols: Vec<TranslationSymbol>,
    pub owners: Vec<TranslationOwner>,
    #[serde(default)]
    pub compiler_gaps: Vec<CompilerGap>,
    #[serde(default)]
    pub data: Option<UnitData>,
}

impl TranslationUnit {
    fn validate_editions(&self) -> Result<(), String> {
        for image in self.images() {
            let (layouts, declared, label) = match self.instance(image) {
                Some(instance) => (
                    &instance.editions,
                    &instance.absolute_symbols,
                    format!("{image} "),
                ),
                None => (&self.editions, &self.absolute_symbols, String::new()),
            };
            for (edition, layout) in layouts {
                if !EDITIONS.contains(&edition.as_str()) {
                    return Err(format!("{}: unsupported {label}edition {edition}", self.id));
                }
                for (name, owner) in &layout.owners {
                    if !self.symbols().any(|member| member.1 == name)
                        || owner.extent == 0
                        || owner.address & 1 != 0
                        || u32::try_from(owner.extent)
                            .ok()
                            .and_then(|n| owner.address.checked_add(n))
                            .is_none()
                        || self.source_owner(image, owner.address).is_err()
                    {
                        return Err(format!(
                            "{}: invalid {label}{edition} owner {name}",
                            self.id
                        ));
                    }
                }
                for (name, symbol) in &layout.absolute_symbols {
                    if !c_identifier(name)
                        || symbol.address > u32::MAX as u64
                        || (symbol.kind != AbsoluteSymbolKind::Data && symbol.address & 1 != 0)
                        || declared
                            .get(name)
                            .or_else(|| self.absolute_symbols.get(name))
                            .is_some_and(|canonical| canonical.kind != symbol.kind)
                        || image != "main" && symbol.kind == AbsoluteSymbolKind::Arm
                    {
                        return Err(format!(
                            "{}: invalid {label}{edition} symbol {name}",
                            self.id
                        ));
                    }
                }
                let spans = layout
                    .owners
                    .values()
                    .map(|owner| (owner.address, owner.extent));
                if overlapping(spans) {
                    return Err(format!(
                        "{}: {label}{edition} declared owners overlap",
                        self.id
                    ));
                }
            }
        }
        Ok(())
    }
    /// An instance links this exact object into another overlay: it places
    /// every member once, registered under the member's name, at its
    /// canonical extent or that extent plus one trailing alignment halfword.
    fn validate_instances(&self, names: &SourcePaths) -> Result<(), String> {
        if self.instances.is_empty() {
            return Ok(());
        }
        if self.overlay.is_none() {
            return Err(format!(
                "{}: main-image units cannot have instances yet",
                self.id
            ));
        }
        if !self.exact() {
            return Err(format!(
                "{}: instances require a wholly exact unit",
                self.id
            ));
        }
        let members = self
            .symbols()
            .map(|member| member.1)
            .collect::<BTreeSet<_>>();
        if members.len() != self.symbols().count() {
            return Err(format!(
                "{}: an instanced unit needs distinct member names",
                self.id
            ));
        }
        for (image, instance) in &self.instances {
            if image == "main" {
                return Err(format!(
                    "{}: main-image instances are not supported yet",
                    self.id
                ));
            }
            if image == self.image() || SourceOwner::parse(&format!("{image}:02000000")).is_err() {
                return Err(format!(
                    "{}: instance {image:?} is not another overlay resource",
                    self.id
                ));
            }
            let placed_members = instance
                .owners
                .keys()
                .map(String::as_str)
                .collect::<BTreeSet<_>>();
            if placed_members != members {
                return Err(format!(
                    "{}: instance {image} must place each member exactly once (missing {:?}, unknown {:?})",
                    self.id,
                    members.difference(&placed_members).collect::<Vec<_>>(),
                    placed_members.difference(&members).collect::<Vec<_>>()
                ));
            }
            for (_, name, extent) in self.symbols() {
                let placed = instance.owners[name];
                let end = u32::try_from(placed.extent)
                    .ok()
                    .and_then(|n| placed.address.checked_add(n));
                if placed.extent == 0
                    || placed.address & 1 != 0
                    || end.is_none()
                    || self.source_owner(image, placed.address).is_err()
                {
                    return Err(format!(
                        "{}: instance {image} places {name} at an invalid address or extent",
                        self.id
                    ));
                }
                let aligned = end.is_some_and(|end| end & 3 == 0);
                if placed.extent != extent
                    && !(aligned && extent.checked_add(2) == Some(placed.extent))
                {
                    return Err(format!(
                        "{}: instance {image} {name} extent {} may exceed canonical extent {extent} only by one trailing alignment halfword",
                        self.id, placed.extent
                    ));
                }
            }
            if overlapping(
                instance
                    .owners
                    .values()
                    .map(|owner| (owner.address, owner.extent)),
            ) {
                return Err(format!("{}: instance {image} owners overlap", self.id));
            }
            let mut owners = self
                .owners_in(image)
                .map(|member| (member.address, member.extent))
                .collect::<Vec<_>>();
            owners.sort_unstable();
            validate_compiler_gaps(&self.id, &owners, &instance.compiler_gaps)?;
            for member in self.members_in(image) {
                let owner = self.source_owner(image, member.address)?;
                if names.registered_name(owner) != Some(member.name) {
                    return Err(format!(
                        "{}: {} must be registered as {}",
                        self.id,
                        owner.id(),
                        member.name
                    ));
                }
            }
            self.validate_absolute_symbols(image, &instance.absolute_symbols, names)?;
            for edition in EDITIONS {
                let variant = |layouts: &BTreeMap<String, EditionLayout>, name: &str| {
                    layouts
                        .get(edition)
                        .and_then(|layout| layout.owners.get(name))
                        .is_some_and(|owner| owner.source_variant)
                };
                if let Some(name) = members
                    .iter()
                    .find(|name| variant(&self.editions, name) != variant(&instance.editions, name))
                {
                    return Err(format!(
                        "{}: instance {image} {edition} {name} must keep the canonical source variant",
                        self.id
                    ));
                }
            }
        }
        Ok(())
    }
    fn validate_absolute_symbols(
        &self,
        image: &str,
        symbols: &BTreeMap<String, AbsoluteSymbol>,
        names: &SourcePaths,
    ) -> Result<(), String> {
        for (name, symbol) in symbols {
            if !c_identifier(name)
                || (symbol.kind != AbsoluteSymbolKind::Data && symbol.address & 1 != 0)
                || self
                    .absolute_symbols
                    .get(name)
                    .is_some_and(|canonical| canonical.kind != symbol.kind)
                || (image != "main"
                    && (symbol.kind == AbsoluteSymbolKind::Arm
                        || names.main_symbol(name)?.is_some()))
            {
                return Err(format!(
                    "{}: invalid {image} absolute symbol {name:?}",
                    self.id
                ));
            }
        }
        Ok(())
    }
    /// The canonical image: the overlay resource, or `main`.
    pub fn image(&self) -> &str {
        self.overlay.as_deref().unwrap_or("main")
    }
    /// The canonical image, then every instance image.
    pub fn images(&self) -> impl Iterator<Item = &str> {
        std::iter::once(self.image()).chain(self.instances.keys().map(String::as_str))
    }
    pub fn instance(&self, image: &str) -> Option<&UnitInstance> {
        self.instances.get(image)
    }
    /// Every member where `image` links it, in `symbols` order; nothing for
    /// an image that does not link this unit.
    pub fn members_in(&self, image: &str) -> impl Iterator<Item = PlacedMember<'_>> {
        let canonical = image == self.image();
        let instance = self.instances.get(image);
        let owners = self.owners.len();
        self.symbols()
            .enumerate()
            .filter_map(move |(index, (address, name, extent))| {
                let placed = match instance {
                    _ if canonical => InstanceOwner { address, extent },
                    Some(instance) => *instance.owners.get(name)?,
                    None => return None,
                };
                Some(PlacedMember {
                    name,
                    canonical: address,
                    address: placed.address,
                    extent: placed.extent,
                    owner: index < owners,
                })
            })
    }
    pub fn owners_in(&self, image: &str) -> impl Iterator<Item = PlacedMember<'_>> {
        self.members_in(image).filter(|member| member.owner)
    }
    /// The owner an instance links at `address`, with its canonical address.
    pub fn instance_owner(&self, image: &str, address: u32) -> Option<PlacedMember<'_>> {
        self.instances.get(image)?;
        self.owners_in(image)
            .find(|member| member.address == address)
    }
    /// Every owner the unit links, in its canonical image and its instances.
    pub fn placed_owners(&self) -> Result<BTreeSet<SourceOwner>, String> {
        self.images()
            .flat_map(|image| {
                self.owners_in(image)
                    .map(move |member| self.source_owner(image, member.address))
            })
            .collect()
    }
    /// Whether `image` links this unit: its canonical image or an instance.
    pub fn links(&self, image: &str) -> bool {
        image == self.image() || self.instances.contains_key(image)
    }
    /// The symbols `image` declares instead of pairing them.
    pub fn declared_symbols(&self, image: &str) -> Option<&BTreeMap<String, AbsoluteSymbol>> {
        match self.instance(image) {
            _ if image == self.image() => Some(&self.absolute_symbols),
            instance => instance.map(|instance| &instance.absolute_symbols),
        }
    }
    /// The compiler fill `image` declares between adjacent members.
    pub fn compiler_gaps_in(&self, image: &str) -> &[CompilerGap] {
        match self.instance(image) {
            _ if image == self.image() => &self.compiler_gaps,
            instance => instance.map_or(&[], |instance| &instance.compiler_gaps),
        }
    }
    pub fn edition_layout(&self, image: &str, edition: &str) -> Option<&EditionLayout> {
        match self.instance(image) {
            _ if image == self.image() => self.editions.get(edition),
            instance => instance?.editions.get(edition),
        }
    }
    /// The reviewed regional layout of the member at canonical `address`, as
    /// `image` links it in `edition`.
    pub fn edition_owner(&self, image: &str, edition: &str, address: u32) -> Option<&EditionOwner> {
        let name = self.symbols().find(|member| member.0 == address)?.1;
        self.edition_layout(image, edition)?.owners.get(name)
    }
    /// Whether the manifest as written declares a member at `address` in
    /// `image`. Unlike `members_in` it needs no loaded names, so an adoption
    /// can consult it before its own placeholder exists.
    pub fn declares(&self, image: &str, address: u32) -> bool {
        match self.instance(image) {
            _ if image == self.image() => self.symbols().any(|member| member.0 == address),
            instance => instance.is_some_and(|instance| {
                instance
                    .owners
                    .values()
                    .any(|owner| owner.address == address)
            }),
        }
    }
    pub fn target(&self) -> Result<CompilerTarget, String> {
        match self.game.as_str() {
            "tbs" => Ok(CompilerTarget::Tbs),
            "tla" => Ok(CompilerTarget::Tla),
            _ => Err(format!("{}: unsupported game {}", self.id, self.game)),
        }
    }
    /// The owner at `address` in `image`, the canonical image or an instance.
    pub fn source_owner(&self, image: &str, address: u32) -> Result<SourceOwner, String> {
        if image != self.image() && !self.instances.contains_key(image) {
            return Err(format!("{}: {image} does not link this unit", self.id));
        }
        match image {
            "main" if address >= 0x0800_0000 => Ok(SourceOwner::Main(address)),
            "main" => Err(format!("main owner address 0x{address:08x} is below ROM")),
            _ if (0x0200_0000..0x0300_0000).contains(&address) => {
                SourceOwner::parse(&format!("{image}:{address:08x}"))
            }
            _ => Err(format!("overlay owner 0x{address:08x} is outside EWRAM")),
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
                self.source_owner(self.image(), address)?.legacy_name(),
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
    /// The manifest as written, before any check across units, registers or
    /// listings: what an adoption consults while its placeholder is absent.
    pub fn declared(root: &Path) -> Result<Self, String> {
        let path = root.join("games/THE BROKEN SEAL/recon/translation-units.json");
        let document: Self = crate::compiler::build_io::read_json(&path)?;
        if document.format != FORMAT
            || document.kind != "reconstruction-composition-contracts"
            || document.original_translation_units != "unknown"
        {
            return Err(format!(
                "{}: expected reconstruction-composition format {FORMAT}",
                path.display()
            ));
        }
        Ok(document)
    }
    pub fn load(root: &Path) -> Result<Self, String> {
        let mut document = Self::declared(root)?;
        let mut ids = BTreeSet::new();
        let mut claimed = BTreeSet::new();
        let mut main_aliases = BTreeSet::new();
        let mut registers = BTreeMap::new();
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
            let game = unit.target()?.as_str();
            if !registers.contains_key(game) {
                registers.insert(game, SourcePaths::load_for_game(root, game)?);
            }
            let names = &registers[game];
            let mut canonical_names = unit
                .symbols()
                .map(|(address, _, _)| {
                    let owner = unit.source_owner(unit.image(), address)?;
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
                let source_owner = unit.source_owner(unit.image(), address)?;
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
            if !unit.compiler_gaps.is_empty() && (unit.overlay.is_none() || !unit.exact()) {
                return Err(format!(
                    "{}: compiler gaps require an exact overlay unit",
                    unit.id
                ));
            }
            let owners = unit
                .owners
                .iter()
                .map(|owner| (owner.address, owner.extent))
                .collect::<Vec<_>>();
            validate_compiler_gaps(&unit.id, &owners, &unit.compiler_gaps)?;
            if let Some(data) = unit.data {
                validate_unit_data(unit, data)?;
            }
            unit.validate_absolute_symbols(unit.image(), &unit.absolute_symbols, names)?;
            unit.validate_instances(names)?;
            unit.validate_editions()?;
            for image in unit.instances.keys() {
                for member in unit.owners_in(image) {
                    let owner = unit.source_owner(image, member.address)?;
                    if !claimed.insert(owner) {
                        return Err(format!("{}: duplicate owner {}", unit.id, owner.id()));
                    }
                }
            }
            let source = root.join(&unit.source);
            let grouped = source.starts_with(names.source_root())
                || source.starts_with(root.join(SHARED_SOURCE_ROOT));
            validate_production_state(root, unit, &source, grouped, names)?;
        }
        for (game, names) in &registers {
            for (path, owners) in names.module_paths() {
                let source = names.source_root().join(path);
                let owners = owners.iter().copied().collect::<BTreeSet<_>>();
                let units = document
                    .units
                    .iter()
                    .filter(|unit| {
                        unit.game == *game
                            && root.join(&unit.source) == source
                            && unit.placed_owners().is_ok_and(|placed| placed == owners)
                    })
                    .count();
                if units != 1 {
                    return Err(format!(
                        "{}: a source linked into several images needs exactly one translation unit whose canonical and instance owners are its {} owners",
                        source.display(),
                        owners.len()
                    ));
                }
            }
        }
        Ok(document)
    }
    pub fn unit(&self, id: &str) -> Option<&TranslationUnit> {
        self.units.iter().find(|unit| unit.id == id)
    }
    /// The unit that links `owner` in its image, as canonical owner or instance.
    pub fn unit_for_game_owner(&self, game: &str, owner: SourceOwner) -> Option<&TranslationUnit> {
        let image = owner.image();
        self.units.iter().find(|unit| {
            unit.game == game
                && unit
                    .owners_in(&image)
                    .any(|member| member.address == owner.address())
        })
    }
}
/// A data section belongs to a wholly exact overlay unit that links into one
/// image, is word aligned, stays in EWRAM and never overlaps the unit's functions.
fn validate_unit_data(unit: &TranslationUnit, data: UnitData) -> Result<(), String> {
    let end = u32::try_from(data.extent)
        .ok()
        .and_then(|extent| data.address.checked_add(extent));
    let overlaps = unit.symbols().any(|(address, _, extent)| {
        u64::from(address) < u64::from(data.address) + data.extent as u64
            && u64::from(data.address) < u64::from(address) + extent as u64
    });
    if unit.overlay.is_none()
        || !unit.exact()
        || !unit.instances.is_empty()
        || data.extent == 0
        || data.address & 3 != 0
        || !(0x0200_0000..0x0300_0000).contains(&data.address)
        || end.is_none_or(|end| end > 0x0300_0000)
        || overlaps
    {
        return Err(format!("{}: invalid unit data placement", unit.id));
    }
    Ok(())
}
/// Compiler fill between two adjacent owners, given in address order.
fn validate_compiler_gaps(
    unit: &str,
    owners: &[(u32, usize)],
    gaps: &[CompilerGap],
) -> Result<(), String> {
    let mut starts = BTreeSet::new();
    for gap in gaps {
        let adjacent = owners.windows(2).any(|pair| {
            pair[0].0.checked_add(pair[0].1 as u32) == Some(gap.start) && pair[1].0 == gap.end
        });
        if !adjacent
            || gap.end.checked_sub(gap.start) != Some(2)
            || gap.end & 3 != 0
            || !starts.insert(gap.start)
        {
            return Err(format!("{unit}: invalid compiler alignment gap"));
        }
    }
    Ok(())
}
fn overlapping(spans: impl Iterator<Item = (u32, usize)>) -> bool {
    let mut spans = spans.collect::<Vec<_>>();
    spans.sort_unstable();
    spans
        .windows(2)
        .any(|pair| u64::from(pair[0].0) + pair[0].1 as u64 > u64::from(pair[1].0))
}
/// A JSON object as a map; serde would silently keep only a repeated key's last value.
fn unique_keys<'de, D, V>(deserializer: D) -> Result<BTreeMap<String, V>, D::Error>
where
    D: Deserializer<'de>,
    V: Deserialize<'de>,
{
    struct Keys<V>(std::marker::PhantomData<V>);
    impl<'de, V: Deserialize<'de>> Visitor<'de> for Keys<V> {
        type Value = BTreeMap<String, V>;
        fn expecting(&self, formatter: &mut std::fmt::Formatter) -> std::fmt::Result {
            formatter.write_str("an object with unique keys")
        }
        fn visit_map<A: MapAccess<'de>>(self, mut map: A) -> Result<Self::Value, A::Error> {
            let mut values = BTreeMap::new();
            while let Some(key) = map.next_key::<String>()? {
                if values.contains_key(&key) {
                    return Err(A::Error::custom(format!("duplicate key {key:?}")));
                }
                values.insert(key, map.next_value()?);
            }
            Ok(values)
        }
    }
    deserializer.deserialize_map(Keys(std::marker::PhantomData))
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
                .join(crate::compiler::routing::game_directory(&unit.game))
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
        if mapped != unit.placed_owners()? {
            return Err(format!(
                "{}: grouped overlay source and owners disagree",
                unit.id
            ));
        }
    }
    for image in unit.instances.keys() {
        let placeholders = overlay_placeholders(root, &unit.game, image)
            .map_err(|error| format!("{}: instance {image}: {error}", unit.id))?;
        if let Some(member) = unit
            .owners_in(image)
            .find(|member| !placeholders.contains(&member.address))
        {
            return Err(format!(
                "{}: instance owner {} is not an AlchemyC_ placeholder in its overlay listing",
                unit.id,
                unit.source_owner(image, member.address)?.id()
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
            let text = overlay_listing(root, &unit.game, overlay)?;
            if let Some(data) = unit.data {
                if crate::compiler::overlay::data_placeholder_extent(&text, data.address)
                    != Some(data.extent)
                {
                    return Err(format!(
                        "{}: unit data needs one AlchemyData_{:08x} placeholder of {} bytes",
                        unit.id, data.address, data.extent
                    ));
                }
            }
            Ok(crate::compiler::overlay::placeholder_addresses(&text))
        })
        .transpose()?;
    let reviewed = if retained_overlay_candidate {
        reviewed_overlay_spans(root)?
    } else {
        BTreeMap::new()
    };
    for member in &unit.owners {
        let owner = unit.source_owner(unit.image(), member.address)?;
        let mapped = names.mapped_source_path(owner);
        let retained = placeholders.as_ref().map_or_else(
            || {
                root.join("games")
                    .join(crate::compiler::routing::game_directory(&unit.game))
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
fn overlay_listing(root: &Path, game: &str, overlay: &str) -> Result<String, String> {
    let assembly = root
        .join("games")
        .join(crate::compiler::routing::game_directory(game))
        .join("asm/overlays")
        .join(format!("{overlay}_overlay.s"));
    std::fs::read_to_string(&assembly).map_err(|error| format!("{}: {error}", assembly.display()))
}
fn overlay_placeholders(root: &Path, game: &str, overlay: &str) -> Result<BTreeSet<u32>, String> {
    overlay_listing(root, game, overlay)
        .map(|text| crate::compiler::overlay::placeholder_addresses(&text))
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
/// A synthetic repository shared by the instance tests of every module.
#[cfg(test)]
pub(crate) mod fixture {
    use super::*;
    use serde_json::{json, Value};

    pub const STAGED_ACTOR: &str = "FIELD/COMMON/OBJECT/STAGED_ACTOR.C";
    pub const FIND: &str = "FieldScene_FindActorRegion";
    pub const REDRAW: &str = "FieldScene_RedrawActorFootprint";
    pub const TABLE: &str = "StagedActor_RegionTable";

    /// A repository whose staged-actor unit links two exact members into
    /// resource_3bf and, as instances, into resource_389 and resource_39b.
    pub struct Repository(pub tempfile::TempDir);
    impl Repository {
        pub fn new() -> Self {
            let repository = Self(tempfile::tempdir().unwrap());
            repository.write(&format!("games/THE BROKEN SEAL/SRC/{STAGED_ACTOR}"), "\n");
            let mut owners = serde_json::Map::new();
            owners.insert("main:08092054".into(), json!({"name": "Object_GetById"}));
            for (image, find, redraw) in [
                ("resource_3bf", 0x0200_034c, 0x0200_08c0),
                ("resource_389", 0x0200_034c, 0x0200_08c0),
                ("resource_39b", 0x0200_0630, 0x0200_0ba4),
            ] {
                repository.listing(image, &[find, redraw]);
                for (address, name) in [(find, FIND), (redraw, REDRAW)] {
                    let record = json!({"name": name, "source": STAGED_ACTOR});
                    owners.insert(format!("{image}:{address:08x}"), record);
                }
            }
            repository.write(
                "games/THE BROKEN SEAL/source-paths.json",
                &json!({"format": 3, "owners": owners}).to_string(),
            );
            repository.units(json!([staged_actor()]));
            repository
        }
        pub fn write(&self, path: &str, text: &str) {
            let path = self.0.path().join(path);
            std::fs::create_dir_all(path.parent().unwrap()).unwrap();
            std::fs::write(path, text).unwrap();
        }
        pub fn listing(&self, image: &str, placeholders: &[u32]) {
            let text = placeholders
                .iter()
                .map(|address| format!("AlchemyC_{address:08x}:\n\t.space 4\n"))
                .collect::<String>();
            self.write(
                &format!("games/THE BROKEN SEAL/asm/overlays/{image}_overlay.s"),
                &text,
            );
        }
        pub fn record(&self, id: &str, record: Value) {
            let path = "games/THE BROKEN SEAL/source-paths.json";
            let text = std::fs::read_to_string(self.0.path().join(path)).unwrap();
            let mut register: Value = serde_json::from_str(&text).unwrap();
            register["owners"][id] = record;
            self.write(path, &register.to_string());
        }
        pub fn units(&self, units: Value) {
            self.write(
                "games/THE BROKEN SEAL/recon/translation-units.json",
                &json!({
                    "format": FORMAT,
                    "kind": "reconstruction-composition-contracts",
                    "original_translation_units": "unknown",
                    "units": units
                })
                .to_string(),
            );
        }
        /// The load error when one member of the staged-actor unit's JSON
        /// is written twice; the valid manifest is restored afterwards.
        pub fn repeated(&self, member: &str) -> String {
            let text = json!([staged_actor()]).to_string();
            assert!(text.contains(member), "{member}");
            let manifest = "games/THE BROKEN SEAL/recon/translation-units.json";
            let valid = std::fs::read_to_string(self.0.path().join(manifest)).unwrap();
            let units = format!("\"units\":{text}");
            assert!(valid.contains(&units));
            let repeated = text.replacen(member, &format!("{member},{member}"), 1);
            self.write(
                manifest,
                &valid.replacen(&units, &format!("\"units\":{repeated}"), 1),
            );
            let error = self.load().unwrap_err();
            self.write(manifest, &valid);
            error
        }
        pub fn load(&self) -> Result<TranslationUnits, String> {
            TranslationUnits::load(self.0.path())
        }
    }
    pub fn staged_actor() -> Value {
        json!({
            "id": "staged-actor",
            "game": "tbs",
            "source": format!("games/THE BROKEN SEAL/SRC/{STAGED_ACTOR}"),
            "compiler_route": "canonical-gcc296",
            "overlay": "resource_3bf",
            "absolute_symbols": {TABLE: {"address": "0x0200df18", "kind": "data"}},
            "owners": [
                {"address": "0x0200034c", "extent": 296, "state": "exact-c"},
                {"address": "0x020008c0", "extent": 284, "state": "exact-c"}
            ],
            "instances": {
                "resource_389": {"owners": {
                    FIND: {"address": "0x0200034c", "extent": 296},
                    REDRAW: {"address": "0x020008c0", "extent": 284}
                }},
                "resource_39b": {
                    "owners": {
                        FIND: {"address": "0x02000630", "extent": 296},
                        REDRAW: {"address": "0x02000ba4", "extent": 284}
                    },
                    "absolute_symbols": {TABLE: {"address": "0x0200e214", "kind": "data"}},
                    "editions": {"ja": {
                        "owners": {REDRAW: {"address": "0x02000bb4", "extent": 284}},
                        "absolute_symbols": {TABLE: {"address": "0x0200e224", "kind": "data"}}
                    }}
                }
            }
        })
    }
}
#[cfg(test)]
mod tests {
    use super::fixture::*;
    use super::*;
    use serde_json::json;

    /// The loaded staged-actor unit after `edit`, checked as `load` checks it.
    fn edited(
        repository: &Repository,
        edit: impl FnOnce(&mut TranslationUnit),
    ) -> Result<(), String> {
        let mut unit = repository.load().unwrap().units.remove(0);
        edit(&mut unit);
        let names = SourcePaths::load(repository.0.path()).unwrap();
        unit.validate_instances(&names)?;
        unit.validate_editions()
    }
    fn rejects(repository: &Repository, cases: &[(&str, fn(&mut TranslationUnit))]) {
        for (message, edit) in cases {
            let error = edited(repository, edit).unwrap_err();
            assert!(error.contains(message), "{message}: {error}");
        }
    }
    fn instance<'a>(unit: &'a mut TranslationUnit, image: &str) -> &'a mut UnitInstance {
        unit.instances.get_mut(image).unwrap()
    }
    fn symbol(unit: &mut TranslationUnit, image: &str, name: &str, kind: AbsoluteSymbolKind) {
        let symbol = AbsoluteSymbol {
            address: 0x0200_e214,
            kind,
        };
        instance(unit, image)
            .absolute_symbols
            .insert(name.into(), symbol);
    }
    /// A 290-byte Japanese owner in the canonical image or an instance.
    fn ja(unit: &mut TranslationUnit, image: &str, name: &str, address: u32, source_variant: bool) {
        let layouts = match unit.instances.get_mut(image) {
            Some(instance) => &mut instance.editions,
            None => &mut unit.editions,
        };
        let owner = EditionOwner {
            address,
            extent: 290,
            source_variant,
        };
        let layout = layouts.entry("ja".into()).or_default();
        layout.owners.insert(name.into(), owner);
    }
    fn copy_instance(unit: &mut TranslationUnit, image: &str) {
        let copy = unit.instances["resource_389"].clone();
        unit.instances.insert(image.into(), copy);
    }

    #[test]
    fn image_views_follow_the_canonical_unit_or_its_instance() {
        let repository = Repository::new();
        let mut unit = repository.load().unwrap().units.remove(0);
        let gap = CompilerGap {
            start: 0x0200_0ba2,
            end: 0x0200_0ba4,
        };
        instance(&mut unit, "resource_39b").compiler_gaps.push(gap);
        ja(&mut unit, "resource_3bf", REDRAW, 0x0200_08d0, false);
        for image in ["resource_3bf", "resource_389", "resource_39b"] {
            assert!(unit.links(image), "{image}");
        }
        assert!(!unit.links("resource_39c") && !unit.links("main"));
        let declared = |image| {
            unit.declared_symbols(image)
                .map(|symbols| symbols[TABLE].address)
        };
        assert_eq!(declared("resource_3bf"), Some(0x0200_df18));
        assert_eq!(declared("resource_39b"), Some(0x0200_e214));
        assert!(unit.declared_symbols("resource_389").unwrap().is_empty());
        assert!(unit.declared_symbols("resource_39c").is_none());
        assert_eq!(unit.compiler_gaps_in("resource_39b").len(), 1);
        assert!(unit.compiler_gaps_in("resource_3bf").is_empty());
        assert!(unit.compiler_gaps_in("resource_39c").is_empty());
        // A regional layout belongs to the image that declares it.
        let regional = |image| {
            unit.edition_owner(image, "ja", 0x0200_08c0)
                .map(|owner| owner.address)
        };
        assert_eq!(regional("resource_3bf"), Some(0x0200_08d0));
        assert_eq!(regional("resource_39b"), Some(0x0200_0bb4));
        assert_eq!(regional("resource_389"), None);
        assert!(unit.edition_layout("resource_39b", "de").is_none());
        // `declares` reads the manifest as written, before names are loaded.
        let written = TranslationUnits::declared(repository.0.path()).unwrap();
        let written = &written.units[0];
        assert!(written.declares("resource_39b", 0x0200_0ba4));
        assert!(written.declares("resource_3bf", 0x0200_08c0));
        assert!(!written.declares("resource_39b", 0x0200_08c0));
        assert!(!written.declares("resource_39c", 0x0200_0ba4));
    }
    #[test]
    fn instances_preserve_member_names_extents_and_symbol_kinds() {
        let repository = Repository::new();
        let manifest = repository.load().unwrap();
        let resource_39b = manifest.units[0].instance("resource_39b").unwrap();
        let owner = resource_39b.owners[REDRAW];
        assert_eq!((owner.address, owner.extent), (0x0200_0ba4, 284));
        assert_eq!(
            resource_39b.absolute_symbols[TABLE].kind,
            AbsoluteSymbolKind::Data
        );
        let ja_owner = &resource_39b.editions["ja"].owners[REDRAW];
        assert_eq!((ja_owner.address, ja_owner.extent), (0x0200_0bb4, 284));
        assert!(manifest.units[0].instance("resource_3bf").is_none());
        rejects(
            &repository,
            &[
                ("invalid resource_39b absolute symbol \"StagedActor_RegionTable\"", |unit| {
                    symbol(unit, "resource_39b", TABLE, AbsoluteSymbolKind::Thumb)
                }),
                ("invalid resource_389 absolute symbol \"Scene_Veneer\"", |unit| {
                    symbol(unit, "resource_389", "Scene_Veneer", AbsoluteSymbolKind::Arm)
                }),
                // Main names bind through each overlay's import veneers instead.
                ("invalid resource_389 absolute symbol \"Object_GetById\"", |unit| {
                    symbol(unit, "resource_389", "Object_GetById", AbsoluteSymbolKind::Thumb)
                }),
                ("invalid resource_39b ja symbol StagedActor_RegionTable", |unit| {
                    let layout = instance(unit, "resource_39b").editions.get_mut("ja").unwrap();
                    layout.absolute_symbols.get_mut(TABLE).unwrap().kind = AbsoluteSymbolKind::Thumb;
                }),
                ("invalid resource_39b ja owner FieldScene_Unknown", |unit| {
                    ja(unit, "resource_39b", "FieldScene_Unknown", 0x0200_0bb4, false)
                }),
                ("invalid resource_39b ja owner FieldScene_RedrawActorFootprint", |unit| {
                    ja(unit, "resource_39b", REDRAW, 0x0800_0bb4, false)
                }),
                ("invalid resource_39b ja owner FieldScene_RedrawActorFootprint", |unit| {
                    ja(unit, "resource_39b", REDRAW, 0x0200_0bb5, false)
                }),
                ("resource_39b ja declared owners overlap", |unit| {
                    ja(unit, "resource_39b", FIND, 0x0200_0ba0, false)
                }),
                ("unsupported resource_39b edition xx", |unit| {
                    let layout = EditionLayout::default();
                    instance(unit, "resource_39b").editions.insert("xx".into(), layout);
                }),
                // A source variant belongs to the shared source, so every image declares it.
                (
                    "instance resource_389 ja FieldScene_RedrawActorFootprint must keep the canonical source variant",
                    |unit| ja(unit, "resource_3bf", REDRAW, 0x0200_08d0, true),
                ),
            ],
        );
        edited(&repository, |unit| {
            for (image, address) in [
                ("resource_3bf", 0x0200_08d0),
                ("resource_389", 0x0200_08d0),
                ("resource_39b", 0x0200_0bb4),
            ] {
                ja(unit, image, REDRAW, address, true);
            }
        })
        .unwrap();
        // A member has one name in every image that links it.
        let renamed = json!({"name": "FieldScene_RedrawFootprint", "source": STAGED_ACTOR});
        repository.record("resource_39b:02000ba4", renamed);
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("staged-actor: resource_39b:02000ba4 must be registered as FieldScene_RedrawActorFootprint"),
            "{error}"
        );
    }
    #[test]
    fn instance_missing_or_duplicating_a_member_is_rejected() {
        let repository = Repository::new();
        rejects(
            &repository,
            &[
                (
                    "instance resource_389 must place each member exactly once (missing [\"FieldScene_RedrawActorFootprint\"], unknown [])",
                    |unit| {
                        instance(unit, "resource_389").owners.remove(REDRAW);
                    },
                ),
                ("(missing [], unknown [\"FieldScene_Unknown\"])", |unit| {
                    let owner = InstanceOwner {
                        address: 0x0200_0a00,
                        extent: 4,
                    };
                    instance(unit, "resource_389").owners.insert("FieldScene_Unknown".into(), owner);
                }),
                ("an instanced unit needs distinct member names", |unit| {
                    unit.owners[1].canonical_name = FIND.into()
                }),
            ],
        );
        // A repeated key is refused, never collapsed to its last value.
        let error = repository
            .repeated(r#""FieldScene_FindActorRegion":{"address":"0x02000630","extent":296}"#);
        assert!(
            error.contains("duplicate key \"FieldScene_FindActorRegion\""),
            "{error}"
        );
    }
    #[test]
    fn two_instances_in_one_image_are_rejected() {
        let repository = Repository::new();
        let unit = staged_actor();
        let resource_389 = format!("\"resource_389\":{}", unit["instances"]["resource_389"]);
        let error = repository.repeated(&resource_389);
        assert!(error.contains("duplicate key \"resource_389\""), "{error}");
        let table = format!("\"{TABLE}\":{}", unit["absolute_symbols"][TABLE]);
        let error = repository.repeated(&table);
        assert!(
            error.contains("duplicate key \"StagedActor_RegionTable\""),
            "{error}"
        );
        rejects(
            &repository,
            &[
                (
                    "instance \"resource_3bf\" is not another overlay resource",
                    |unit| copy_instance(unit, "resource_3bf"),
                ),
                (
                    "instance \"resource_38\" is not another overlay resource",
                    |unit| copy_instance(unit, "resource_38"),
                ),
                ("main-image instances are not supported yet", |unit| {
                    copy_instance(unit, "main")
                }),
                ("main-image units cannot have instances yet", |unit| {
                    unit.overlay = None
                }),
                ("instances require a wholly exact unit", |unit| {
                    unit.owners[0].state = OwnerState::RetainedAssembly
                }),
            ],
        );
    }
    #[test]
    fn instance_owner_claimed_by_another_unit_is_rejected() {
        let repository = Repository::new();
        // A second unit whose resource_389 instance repeats the first one's.
        repository.record("resource_39c:02000630", json!({"name": FIND}));
        repository.record("resource_39c:02000ba4", json!({"name": REDRAW}));
        let mut copy = staged_actor();
        copy["id"] = "staged-actor-copy".into();
        copy["overlay"] = "resource_39c".into();
        copy["owners"] = staged_actor()["instances"]["resource_39b"]["owners"]
            .as_object()
            .unwrap()
            .values()
            .map(|owner| json!({"address": owner["address"], "extent": owner["extent"], "state": "exact-c"}))
            .collect();
        copy["instances"]
            .as_object_mut()
            .unwrap()
            .remove("resource_39b");
        copy.as_object_mut().unwrap().remove("absolute_symbols");
        repository.units(json!([staged_actor(), copy]));
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("staged-actor-copy: duplicate owner resource_389:0200034c"),
            "{error}"
        );
        // A unit whose canonical owner is another unit's instance owner.
        let mut canonical = staged_actor();
        canonical["id"] = "staged-actor-389".into();
        canonical["overlay"] = "resource_389".into();
        canonical.as_object_mut().unwrap().remove("instances");
        repository.units(json!([staged_actor(), canonical]));
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("staged-actor-389: duplicate owner resource_389:0200034c"),
            "{error}"
        );
    }
    #[test]
    fn instance_extent_may_add_only_one_alignment_halfword() {
        let repository = Repository::new();
        let placed = |canonical: usize, address: u32, extent: usize| {
            edited(&repository, |unit| {
                unit.owners[0].extent = canonical;
                let owner = InstanceOwner { address, extent };
                instance(unit, "resource_39b")
                    .owners
                    .insert(FIND.into(), owner);
            })
        };
        // A 294-byte canonical owner ends two bytes short of a word boundary.
        for (canonical, extent) in [(294, 294), (294, 296), (296, 296)] {
            placed(canonical, 0x0200_0630, extent).unwrap();
        }
        for (canonical, address, extent) in [
            (294, 0x0200_0630, 298),
            (294, 0x0200_0630, 292),
            (294, 0x0200_0632, 296),
            (296, 0x0200_0630, 298),
        ] {
            let error = placed(canonical, address, extent).unwrap_err();
            assert!(
                error.contains("only by one trailing alignment halfword"),
                "{error}"
            );
        }
        for (address, extent) in [
            (0x0200_0631, 296),
            (0x0800_0630, 296),
            (0x0200_0630, 0),
            (0x0200_0630, usize::MAX),
        ] {
            let error = placed(296, address, extent).unwrap_err();
            assert!(error.contains("invalid address or extent"), "{error}");
        }
        let error = placed(296, 0x0200_0ba0, 296).unwrap_err();
        assert!(
            error.contains("instance resource_39b owners overlap"),
            "{error}"
        );
        // A 1394-byte first member leaves compiler fill before the second in
        // every image; each instance declares the fill at its own address.
        let gap = |start: u32, end: u32| {
            edited(&repository, |unit| {
                unit.owners[0].extent = 1394;
                for image in ["resource_389", "resource_39b"] {
                    instance(unit, image).owners.get_mut(FIND).unwrap().extent = 1394;
                }
                instance(unit, "resource_39b").compiler_gaps = vec![CompilerGap { start, end }];
            })
        };
        gap(0x0200_0ba2, 0x0200_0ba4).unwrap();
        for (start, end) in [(0x0200_0ba0, 0x0200_0ba2), (0x0200_08be, 0x0200_08c0)] {
            let error = gap(start, end).unwrap_err();
            assert!(error.contains("invalid compiler alignment gap"), "{error}");
        }
    }
    #[test]
    fn grouped_source_register_owners_must_equal_canonical_plus_instance_owners() {
        let disagree = "staged-actor: grouped overlay source and owners disagree";
        let repository = Repository::new();
        let mut unit = staged_actor();
        unit["instances"]
            .as_object_mut()
            .unwrap()
            .remove("resource_39b");
        repository.units(json!([unit]));
        let error = repository.load().unwrap_err();
        assert!(error.contains(disagree), "{error}");
        repository.units(json!([staged_actor()]));
        repository.record("resource_39b:02000630", json!({"name": FIND}));
        repository.record("resource_39b:02000ba4", json!({"name": REDRAW}));
        let error = repository.load().unwrap_err();
        assert!(error.contains(disagree), "{error}");

        let repository = Repository::new();
        repository.listing("resource_39b", &[0x0200_0630]);
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("staged-actor: instance owner resource_39b:02000ba4 is not an AlchemyC_ placeholder in its overlay listing"),
            "{error}"
        );
        let listing = "games/THE BROKEN SEAL/asm/overlays/resource_39b_overlay.s";
        std::fs::remove_file(repository.0.path().join(listing)).unwrap();
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("staged-actor: instance resource_39b: "),
            "{error}"
        );

        // A source linked into several images always has one unit.
        let repository = Repository::new();
        let orbit = json!({"name": "SceneEffect_UpdateOrbitPosition", "source": "FIELD/COMMON/EFFECT/ORBIT.C"});
        repository.record("resource_373:02005000", orbit.clone());
        repository.record("resource_38e:02000100", orbit);
        let error = repository.load().unwrap_err();
        assert!(
            error.contains("ORBIT.C: a source linked into several images needs exactly one translation unit whose canonical and instance owners are its 2 owners"),
            "{error}"
        );
    }
    #[test]
    fn edition_layouts_preserve_names_extents_and_symbol_kinds() {
        let document = TranslationUnits::load(crate::compiler::routing::root()).unwrap();
        let unit = document.unit("heidia-village-scene").unwrap();
        assert_eq!(
            unit.edition_owner("resource_3bf", "ja", 0x02001e94)
                .unwrap()
                .extent,
            460
        );
        assert!(unit
            .edition_owner("resource_3bf", "en", 0x02001e94)
            .is_none());
        assert!(unit.validate_editions().is_ok());
        for address in [0, 0x02001e95, 0x08001e94, u32::MAX] {
            let mut invalid = unit.clone();
            invalid
                .editions
                .get_mut("ja")
                .unwrap()
                .owners
                .values_mut()
                .next()
                .unwrap()
                .address = address;
            assert!(invalid.validate_editions().is_err());
        }
        for extent in [0, usize::MAX] {
            let mut invalid = unit.clone();
            invalid
                .editions
                .get_mut("ja")
                .unwrap()
                .owners
                .values_mut()
                .next()
                .unwrap()
                .extent = extent;
            assert!(invalid.validate_editions().is_err());
        }
        let mut invalid = unit.clone();
        invalid
            .editions
            .insert("xx".into(), EditionLayout::default());
        assert!(invalid.validate_editions().is_err());
        let mut invalid = unit.clone();
        invalid.editions.get_mut("ja").unwrap().owners.insert(
            "UnknownOwner".into(),
            EditionOwner {
                address: 0x02001e94,
                extent: 460,
                source_variant: true,
            },
        );
        assert!(invalid.validate_editions().is_err());
        let mut invalid = unit.clone();
        invalid.editions.get_mut("ja").unwrap().owners.insert(
            unit.owners[0].canonical_name.clone(),
            EditionOwner {
                address: 0x02001e94,
                extent: 460,
                source_variant: false,
            },
        );
        assert!(invalid.validate_editions().unwrap_err().contains("overlap"));
        let mut invalid = unit.clone();
        invalid
            .editions
            .get_mut("ja")
            .unwrap()
            .absolute_symbols
            .get_mut("Data_03001ebc")
            .unwrap()
            .kind = AbsoluteSymbolKind::Thumb;
        assert!(invalid.validate_editions().is_err());
    }
    #[test]
    fn reviewed_owner_duplicates_never_select_the_last_extent() {
        let root = tempfile::tempdir().unwrap();
        let path = root
            .path()
            .join("games/THE BROKEN SEAL/semantic/regions.json");
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
        assert!(manifest
            .unit("runtime-memory-schedule-callback-and-release-block-32-a")
            .unwrap()
            .exact());
        let overlay = manifest.unit("guarded-step-scene").unwrap();
        assert_eq!(overlay.image(), "resource_37b");
        assert_eq!(
            overlay
                .source_owner("resource_37b", 0x0200_0030)
                .unwrap()
                .id(),
            "resource_37b:02000030"
        );
        assert!(overlay.source_owner("resource_37c", 0x0200_0030).is_err());
        assert_eq!(
            overlay.absolute_symbols["SceneEventRuntime_ScriptData"].kind,
            AbsoluteSymbolKind::Data
        );
        let owner = SourceOwner::Main(0x080f_37ec);
        assert!(manifest.unit_for_game_owner("tbs", owner).is_some());
        assert!(manifest.unit_for_game_owner("tla", owner).is_none());
        let root = crate::compiler::routing::root();
        let names = SourcePaths::load_for_game(root, "tbs").unwrap();
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
        invalid.source = PathBuf::from("games/THE BROKEN SEAL/SRC/invalid-retained-overlay.c");
        assert!(invalid_state(&invalid));
        let shared = manifest.unit("audio-cgb-channel-mute").unwrap();
        assert!(shared.source.starts_with(SHARED_SOURCE_ROOT) && shared.exact());
        let shared_source = root.join(&shared.source);
        assert!(validate_production_state(root, shared, &shared_source, true, &names).is_ok());
        assert!(validate_production_state(root, shared, &shared_source, false, &names).is_err());
        let i = unconditional_quoted_includes;
        assert!(i("#define X \\\n#include \"x\"").is_empty());
        assert!(i("/* */ #if 0\n#include \"x\"").is_empty());

        let repository = Repository::new();
        let manifest = repository.load().unwrap();
        let unit = &manifest.units[0];
        let images = unit.images().collect::<Vec<_>>();
        assert_eq!(images, ["resource_3bf", "resource_389", "resource_39b"]);
        let owner = SourceOwner::parse("resource_39b:02000ba4").unwrap();
        let canonical = SourceOwner::parse("resource_3bf:020008c0").unwrap();
        for owner in [owner, canonical] {
            let found = manifest.unit_for_game_owner("tbs", owner);
            assert_eq!(found.map(|unit| unit.id.as_str()), Some("staged-actor"));
            assert!(manifest.unit_for_game_owner("tla", owner).is_none());
        }
        let unlinked = SourceOwner::parse("resource_39b:0200034c").unwrap();
        assert!(manifest.unit_for_game_owner("tbs", unlinked).is_none());
        assert_eq!(unit.source_owner("resource_39b", 0x0200_0ba4), Ok(owner));
        let placed = PlacedMember {
            name: REDRAW,
            canonical: 0x0200_08c0,
            address: 0x0200_0ba4,
            extent: 284,
            owner: true,
        };
        assert_eq!(
            unit.instance_owner("resource_39b", 0x0200_0ba4),
            Some(placed)
        );
        assert_eq!(unit.instance_owner("resource_3bf", 0x0200_08c0), None);
        assert_eq!(unit.owners_in("resource_39b").last(), Some(placed));
        assert_eq!(unit.owners_in("resource_39c").count(), 0);
        assert_eq!(unit.placed_owners().unwrap().len(), 6);
        // A local symbol is placed in every image and claimed in none.
        let mut local = unit.clone();
        let name = "FieldScene_ClearActorRegion";
        local.local_symbols.push(TranslationSymbol {
            address: 0x0200_0474,
            canonical_name: name.into(),
            extent: 8,
        });
        let symbol = InstanceOwner {
            address: 0x0200_0758,
            extent: 8,
        };
        instance(&mut local, "resource_39b")
            .owners
            .insert(name.into(), symbol);
        let member = local.members_in("resource_39b").last().unwrap();
        assert_eq!(
            (member.canonical, member.address, member.owner),
            (0x0200_0474, 0x0200_0758, false)
        );
        assert_eq!(local.owners_in("resource_39b").count(), 2);
    }
    #[test]
    fn unit_data_belongs_to_word_aligned_ewram_outside_the_functions() {
        let manifest = TranslationUnits::load(crate::compiler::routing::root()).unwrap();
        let unit = manifest.unit("kuupuappu-runpa-jail").unwrap();
        let data = unit.data.unwrap();
        assert_eq!((data.address, data.extent), (0x0200_02d0, 0x200));
        assert!(validate_unit_data(unit, data).is_ok());
        for (address, extent) in [
            (0x0200_02d2, 4),
            (0x0200_02d0, 0),
            (0x0300_0000, 4),
            (0x02ff_fffc, 8),
            (0x0200_0030, 4),
        ] {
            assert!(validate_unit_data(unit, UnitData { address, extent }).is_err());
        }
        let mut inexact = unit.clone();
        inexact.owners[0].state = OwnerState::RetainedAssembly;
        assert!(validate_unit_data(&inexact, data).is_err());
        let mut main = unit.clone();
        main.overlay = None;
        assert!(validate_unit_data(&main, data).is_err());
    }
}
