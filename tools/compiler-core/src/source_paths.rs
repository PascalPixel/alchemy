//! Canonical owner register for each historical game.
//!
//! Exact C historically lived in one flat directory and encoded ownership in
//! each filename. `games/<game>/source-paths.json` maps stable address-qualified
//! owner identities to canonical human symbols and descriptive source paths.
//! GS1 remains the default resolver for existing callers; target-aware build
//! paths select the corresponding game registry. Owners absent from a manifest
//! retain their legacy flat path during the migration.

use serde_json::Value;
use std::collections::{BTreeMap, BTreeSet};
use std::fs;
use std::path::{Component, Path, PathBuf};

pub const SOURCE_DIRECTORY: &str = "games/gs1/src";
pub const SOURCE_PATHS_MANIFEST: &str = "games/gs1/source-paths.json";

#[derive(Clone, Copy, Debug, Eq, Ord, PartialEq, PartialOrd)]
pub enum SourceOwner {
    Main(u32),
    Overlay { resource: u16, address: u32 },
}

impl SourceOwner {
    pub fn parse(id: &str) -> Result<Self, String> {
        if let Some(address) = id.strip_prefix("main:") {
            return Ok(Self::Main(parse_lower_hex(address, 8, id)?));
        }
        let Some((resource, address)) = id.split_once(':') else {
            return Err(format!("invalid source owner {id:?}"));
        };
        let resource = resource
            .strip_prefix("resource_")
            .ok_or_else(|| format!("invalid source owner {id:?}"))?;
        Ok(Self::Overlay {
            resource: parse_lower_hex(resource, 3, id)? as u16,
            address: parse_lower_hex(address, 8, id)?,
        })
    }

    pub fn from_legacy_stem(stem: &str) -> Option<Self> {
        if stem.len() == 8 && lower_hex(stem) {
            return u32::from_str_radix(stem, 16).ok().map(Self::Main);
        }
        let (resource, address) = stem.split_once("_c_")?;
        let resource = resource.strip_prefix("resource_")?;
        if resource.len() != 3 || address.len() != 8 || !lower_hex(resource) || !lower_hex(address)
        {
            return None;
        }
        Some(Self::Overlay {
            resource: u16::from_str_radix(resource, 16).ok()?,
            address: u32::from_str_radix(address, 16).ok()?,
        })
    }

    pub fn id(self) -> String {
        match self {
            Self::Main(address) => format!("main:{address:08x}"),
            Self::Overlay { resource, address } => {
                format!("resource_{resource:03x}:{address:08x}")
            }
        }
    }

    pub fn address(self) -> u32 {
        match self {
            Self::Main(address) | Self::Overlay { address, .. } => address,
        }
    }

    pub fn address_stem(self) -> String {
        format!("{:08x}", self.address())
    }

    pub fn overlay_id(self) -> Option<String> {
        match self {
            Self::Main(_) => None,
            Self::Overlay { resource, .. } => Some(format!("resource_{resource:03x}")),
        }
    }

    pub fn is_main(self) -> bool {
        matches!(self, Self::Main(_))
    }

    pub fn legacy_stem(self) -> String {
        match self {
            Self::Main(address) => format!("{address:08x}"),
            Self::Overlay { resource, address } => {
                format!("resource_{resource:03x}_c_{address:08x}")
            }
        }
    }

    pub fn legacy_relative_path(self) -> PathBuf {
        PathBuf::from(format!("{}.c", self.legacy_stem()))
    }

    /// Stable synthetic route used by the compiler tables. Source location is
    /// presentation; compiler routing belongs to the owner.
    pub fn routing_path(self) -> PathBuf {
        self.routing_path_for_game("gs1")
    }

    pub fn routing_path_for_game(self, game: &str) -> PathBuf {
        Path::new("games")
            .join(game)
            .join("src")
            .join(self.legacy_relative_path())
    }
}

fn lower_hex(value: &str) -> bool {
    value
        .bytes()
        .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
}

fn c_identifier(value: &str) -> bool {
    let mut bytes = value.bytes();
    if !matches!(bytes.next(), Some(first) if first == b'_' || first.is_ascii_alphabetic()) {
        return false;
    }
    bytes.all(|byte| byte == b'_' || byte.is_ascii_alphanumeric())
}

fn parse_lower_hex(value: &str, width: usize, owner: &str) -> Result<u32, String> {
    if value.len() != width || !lower_hex(value) {
        return Err(format!(
            "source owner {owner:?} is not canonical lowercase hex"
        ));
    }
    u32::from_str_radix(value, 16)
        .map_err(|error| format!("invalid source owner {owner:?}: {error}"))
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct SourceFile {
    pub owner: SourceOwner,
    pub path: PathBuf,
}

#[derive(Clone, Debug)]
pub struct SourcePaths {
    repository: PathBuf,
    source_directory: PathBuf,
    manifest: PathBuf,
    by_owner: BTreeMap<SourceOwner, PathBuf>,
    by_path: BTreeMap<PathBuf, Vec<SourceOwner>>,
    names: BTreeMap<SourceOwner, String>,
}

impl SourcePaths {
    pub fn load(repository: &Path) -> Result<Self, String> {
        Self::load_for_game(repository, "gs1")
    }

    pub fn load_for_game(repository: &Path, game: &str) -> Result<Self, String> {
        let (source_directory, manifest) = game_paths(game)?;
        let path = repository.join(&manifest);
        if !path.exists() {
            return Ok(Self::empty(repository, source_directory, manifest));
        }
        let text =
            fs::read_to_string(&path).map_err(|error| format!("{}: {error}", path.display()))?;
        Self::parse_for_game(repository, game, &text)
            .map_err(|error| format!("{}: {error}", path.display()))
    }

    pub fn parse(repository: &Path, text: &str) -> Result<Self, String> {
        Self::parse_for_game(repository, "gs1", text)
    }

    pub fn parse_for_game(repository: &Path, game: &str, text: &str) -> Result<Self, String> {
        let (source_directory, manifest) = game_paths(game)?;
        let value: Value = serde_json::from_str(text).map_err(|error| error.to_string())?;
        if value.get("format").and_then(Value::as_u64) != Some(2) {
            return Err("format must be 2".into());
        }
        let owners = value
            .get("owners")
            .and_then(Value::as_object)
            .ok_or("owners must be an object")?;
        let mut by_owner = BTreeMap::new();
        let mut by_path = BTreeMap::<PathBuf, Vec<SourceOwner>>::new();
        let mut names = BTreeMap::new();
        for (id, value) in owners {
            let owner = SourceOwner::parse(id)?;
            let record = value
                .as_object()
                .ok_or_else(|| format!("{id}: owner record must be an object"))?;
            let name = record
                .get("name")
                .and_then(Value::as_str)
                .ok_or_else(|| format!("{id}: owner name must be a string"))?;
            if !c_identifier(name) {
                return Err(format!("{id}: owner name {name:?} is not a C identifier"));
            }
            names.insert(owner, name.to_string());
            let Some(source) = record.get("source") else {
                continue;
            };
            let source = source
                .as_str()
                .ok_or_else(|| format!("{id}: source path must be a string"))?;
            let path = validate_source_path(source)?;
            if by_owner.insert(owner, path.clone()).is_some() {
                return Err(format!("duplicate source owner {id}"));
            }
            by_path.entry(path).or_default().push(owner);
        }
        for (path, owners) in &by_path {
            if owners.len() <= 1 {
                continue;
            }
            let address = owners[0].address();
            if owners
                .iter()
                .any(|owner| owner.is_main() || owner.address() != address)
            {
                return Err(format!(
                    "{} may be shared only by overlay owners at one load address",
                    path.display()
                ));
            }
        }
        Ok(Self {
            repository: repository.to_path_buf(),
            source_directory,
            manifest,
            by_owner,
            by_path,
            names,
        })
    }

    fn empty(repository: &Path, source_directory: PathBuf, manifest: PathBuf) -> Self {
        Self {
            repository: repository.to_path_buf(),
            source_directory,
            manifest,
            by_owner: BTreeMap::new(),
            by_path: BTreeMap::new(),
            names: BTreeMap::new(),
        }
    }

    pub fn source_root(&self) -> PathBuf {
        self.repository.join(&self.source_directory)
    }

    pub fn manifest_path(&self) -> PathBuf {
        self.repository.join(&self.manifest)
    }

    pub fn mapped_relative_path(&self, owner: SourceOwner) -> Option<&Path> {
        self.by_owner.get(&owner).map(PathBuf::as_path)
    }

    pub fn mapped_source_path(&self, owner: SourceOwner) -> Option<PathBuf> {
        self.mapped_relative_path(owner)
            .map(|path| self.source_root().join(path))
    }

    pub fn registered_name(&self, owner: SourceOwner) -> Option<&str> {
        self.names.get(&owner).map(String::as_str)
    }

    /// Destination for a new exact-source adoption. Legacy fallback is
    /// intentionally excluded: new writes must never recreate the flat,
    /// address-named layout.
    pub fn registered_source_path(&self, owner: SourceOwner) -> Result<PathBuf, String> {
        self.mapped_source_path(owner).ok_or_else(|| {
            format!(
                "register a descriptive path for {} in {} before adopting exact C",
                owner.id(),
                self.manifest_path().display()
            )
        })
    }

    /// Remove an owner's exact-C path after parking it back to assembly. Its
    /// canonical name remains registered.
    pub fn unregister_owner(&self, owner: SourceOwner) -> Result<bool, String> {
        let manifest = self.manifest_path();
        if !manifest.exists() {
            return Ok(false);
        }
        let text = fs::read_to_string(&manifest)
            .map_err(|error| format!("{}: {error}", manifest.display()))?;
        let mut value: Value = serde_json::from_str(&text)
            .map_err(|error| format!("{}: {error}", manifest.display()))?;
        let owners = value
            .get_mut("owners")
            .and_then(Value::as_object_mut)
            .ok_or_else(|| format!("{}: owners must be an object", manifest.display()))?;
        let Some(record) = owners.get_mut(&owner.id()).and_then(Value::as_object_mut) else {
            return Ok(false);
        };
        if record.remove("source").is_none() {
            return Ok(false);
        }
        let mut rendered =
            serde_json::to_string_pretty(&value).map_err(|error| error.to_string())?;
        rendered.push('\n');
        let temporary = manifest.with_extension("json.tmp");
        fs::write(&temporary, rendered)
            .map_err(|error| format!("{}: {error}", temporary.display()))?;
        fs::rename(&temporary, &manifest)
            .map_err(|error| format!("{}: {error}", manifest.display()))?;
        Ok(true)
    }

    pub fn relative_path(&self, owner: SourceOwner) -> PathBuf {
        self.by_owner
            .get(&owner)
            .cloned()
            .unwrap_or_else(|| owner.legacy_relative_path())
    }

    pub fn source_path(&self, owner: SourceOwner) -> PathBuf {
        self.source_root().join(self.relative_path(owner))
    }

    pub fn repository_relative_path(&self, owner: SourceOwner) -> PathBuf {
        self.source_directory.join(self.relative_path(owner))
    }

    pub fn owners_for_path(&self, path: &Path) -> Vec<SourceOwner> {
        let Some(relative) = self.path_within_source_root(path) else {
            return Vec::new();
        };
        if let Some(owners) = self.by_path.get(&relative) {
            return owners.clone();
        }
        if relative.components().count() != 1 {
            return Vec::new();
        }
        relative
            .file_stem()
            .and_then(|stem| stem.to_str())
            .and_then(SourceOwner::from_legacy_stem)
            .into_iter()
            .collect()
    }

    pub fn owner_for_path(&self, path: &Path) -> Result<Option<SourceOwner>, String> {
        let owners = self.owners_for_path(path);
        match owners.as_slice() {
            [] => Ok(None),
            [owner] => Ok(Some(*owner)),
            _ => Err(format!(
                "{} is shared by multiple source owners; supply an owner context",
                path.display()
            )),
        }
    }

    pub fn overlay_owner_for_path(
        &self,
        overlay: &str,
        path: &Path,
    ) -> Result<Option<SourceOwner>, String> {
        let owners = self
            .owners_for_path(path)
            .into_iter()
            .filter(|owner| owner.overlay_id().as_deref() == Some(overlay))
            .collect::<Vec<_>>();
        match owners.as_slice() {
            [] => Ok(None),
            [owner] => Ok(Some(*owner)),
            _ => Err(format!(
                "{} maps to multiple owners in {overlay}",
                path.display()
            )),
        }
    }

    pub fn main_sources(&self) -> Result<Vec<SourceFile>, String> {
        self.sources(Some(true), None, true)
    }

    /// Missing mapped files are omitted so `overlay-adopt` can resolve a new
    /// nested destination before it copies the proved candidate into place.
    pub fn overlay_sources(&self, overlay: &str) -> Result<Vec<SourceFile>, String> {
        self.sources(Some(false), Some(overlay), false)
    }

    pub fn all_sources(&self) -> Result<Vec<SourceFile>, String> {
        self.sources(None, None, true)
    }

    fn sources(
        &self,
        main: Option<bool>,
        overlay: Option<&str>,
        mapped_files_required: bool,
    ) -> Result<Vec<SourceFile>, String> {
        let mut found = BTreeMap::<SourceOwner, PathBuf>::new();
        let source_root = self.source_root();
        if source_root.exists() {
            let entries = fs::read_dir(&source_root)
                .map_err(|error| format!("{}: {error}", source_root.display()))?;
            for entry in entries {
                let entry = entry.map_err(|error| error.to_string())?;
                if !entry
                    .file_type()
                    .map_err(|error| error.to_string())?
                    .is_file()
                {
                    continue;
                }
                let path = entry.path();
                let Some(stem) = path.file_stem().and_then(|stem| stem.to_str()) else {
                    continue;
                };
                let Some(owner) = SourceOwner::from_legacy_stem(stem) else {
                    continue;
                };
                if !matches_filter(owner, main, overlay) {
                    continue;
                }
                if self.by_owner.contains_key(&owner) {
                    return Err(format!(
                        "{} exists at both its legacy path and its mapped source path",
                        owner.id()
                    ));
                }
                found.insert(owner, path);
            }
        }
        for (owner, relative) in &self.by_owner {
            if !matches_filter(*owner, main, overlay) {
                continue;
            }
            let path = source_root.join(relative);
            if !path.is_file() {
                if mapped_files_required {
                    return Err(format!(
                        "{} maps to missing source {}",
                        owner.id(),
                        path.display()
                    ));
                }
                continue;
            }
            if found.insert(*owner, path).is_some() {
                return Err(format!("duplicate source owner {}", owner.id()));
            }
        }
        Ok(found
            .into_iter()
            .map(|(owner, path)| SourceFile { owner, path })
            .collect())
    }

    fn path_within_source_root(&self, path: &Path) -> Option<PathBuf> {
        if path.is_absolute() {
            return path
                .strip_prefix(self.source_root())
                .ok()
                .map(Path::to_path_buf);
        }
        path.strip_prefix(&self.source_directory)
            .ok()
            .map(Path::to_path_buf)
            .or_else(|| {
                // A manifest-relative path is useful to in-memory tooling and
                // tests, but an arbitrary path elsewhere in the repository is
                // never inferred from its basename.
                validate_source_path(&path.to_string_lossy()).ok()
            })
    }

    pub fn validate_tree(&self) -> Result<(), String> {
        let known: BTreeSet<PathBuf> = self
            .all_sources()?
            .into_iter()
            .map(|source| source.path)
            .collect();
        let mut unowned = Vec::new();
        visit_c_files(&self.source_root(), &mut |path| {
            if !known.contains(path) {
                unowned.push(path.to_path_buf());
            }
        })?;
        if let Some(path) = unowned.first() {
            return Err(format!(
                "nested exact source is absent from {}: {}",
                self.manifest.display(),
                path.display()
            ));
        }
        Ok(())
    }
}

fn game_paths(game: &str) -> Result<(PathBuf, PathBuf), String> {
    if game.is_empty()
        || !game
            .bytes()
            .all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit())
    {
        return Err(format!("invalid game id {game:?}"));
    }
    let root = Path::new("games").join(game);
    Ok((root.join("src"), root.join("source-paths.json")))
}

fn matches_filter(owner: SourceOwner, main: Option<bool>, overlay: Option<&str>) -> bool {
    if main.is_some_and(|main| owner.is_main() != main) {
        return false;
    }
    overlay.is_none_or(|wanted| owner.overlay_id().as_deref() == Some(wanted))
}

fn validate_source_path(source: &str) -> Result<PathBuf, String> {
    let path = Path::new(source);
    let address_named = path
        .file_stem()
        .and_then(|stem| stem.to_str())
        .and_then(SourceOwner::from_legacy_stem)
        .is_some();
    if path.is_absolute()
        || path.components().count() < 2
        || path.extension().and_then(|value| value.to_str()) != Some("c")
        || address_named
        || path
            .components()
            .any(|component| !matches!(component, Component::Normal(_)))
    {
        return Err(format!(
            "source path {source:?} must be a nested, relative .c path"
        ));
    }
    Ok(path.to_path_buf())
}

fn visit_c_files(directory: &Path, visit: &mut impl FnMut(&Path)) -> Result<(), String> {
    if !directory.exists() {
        return Ok(());
    }
    for entry in
        fs::read_dir(directory).map_err(|error| format!("{}: {error}", directory.display()))?
    {
        let entry = entry.map_err(|error| error.to_string())?;
        let path = entry.path();
        if path.is_dir() {
            visit_c_files(&path, visit)?;
        } else if path.extension().and_then(|value| value.to_str()) == Some("c") {
            visit(&path);
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;
    use tempfile::tempdir;

    fn manifest() -> &'static str {
        r#"{
  "format": 2,
  "owners": {
    "main:080bbb0c": {"name":"resolve_action","source":"battle/resolve_action.c"},
    "resource_39c:0200013c": {"name":"spawn_configured_effect","source":"battle/effects/spawn_configured_effect.c"}
  }
}"#
    }

    #[test]
    fn parses_main_and_overlay_owner_ids() {
        assert_eq!(
            SourceOwner::parse("main:080bbb0c").unwrap(),
            SourceOwner::Main(0x080b_bb0c)
        );
        assert_eq!(
            SourceOwner::parse("resource_39c:0200013c").unwrap(),
            SourceOwner::Overlay {
                resource: 0x39c,
                address: 0x0200_013c,
            }
        );
        assert!(SourceOwner::parse("main:080BBB0C").is_err());
    }

    #[test]
    fn mapped_paths_and_legacy_fallback_share_one_resolver() {
        let root = tempdir().unwrap();
        let paths = SourcePaths::parse(root.path(), manifest()).unwrap();
        assert_eq!(
            paths.relative_path(SourceOwner::Main(0x080b_bb0c)),
            PathBuf::from("battle/resolve_action.c")
        );
        assert_eq!(
            paths.relative_path(SourceOwner::Main(0x0800_2efc)),
            PathBuf::from("08002efc.c")
        );
        assert!(paths
            .registered_source_path(SourceOwner::Main(0x0800_2efc))
            .unwrap_err()
            .contains("register a descriptive path"));
        assert_eq!(
            paths
                .registered_source_path(SourceOwner::Main(0x080b_bb0c))
                .unwrap(),
            root.path()
                .join(SOURCE_DIRECTORY)
                .join("battle/resolve_action.c")
        );
    }

    #[test]
    fn reverse_lookup_understands_nested_and_legacy_sources() {
        let root = tempdir().unwrap();
        let paths = SourcePaths::parse(root.path(), manifest()).unwrap();
        assert_eq!(
            paths
                .owner_for_path(Path::new("games/gs1/src/battle/resolve_action.c"))
                .unwrap(),
            Some(SourceOwner::Main(0x080b_bb0c))
        );
        assert_eq!(
            paths
                .owner_for_path(Path::new("games/gs1/src/resource_382_c_0200013c.c"))
                .unwrap(),
            Some(SourceOwner::Overlay {
                resource: 0x382,
                address: 0x0200_013c,
            })
        );
    }

    #[test]
    fn paths_cannot_escape_or_recreate_the_flat_address_convention() {
        let root = tempdir().unwrap();
        for path in [
            "../outside.c",
            "080bbb0c.c",
            "battle/080bbb0c.c",
            "/tmp/source.c",
            "battle/readme.md",
        ] {
            let text = format!("{{\"format\":2,\"owners\":{{\"main:080bbb0c\":{{\"name\":\"resolve_action\",\"source\":{path:?}}}}}}}");
            assert!(SourcePaths::parse(root.path(), &text).is_err(), "{path}");
        }
    }

    #[test]
    fn one_source_can_own_the_same_address_in_related_overlays() {
        let root = tempdir().unwrap();
        let text = r#"{
  "format": 2,
  "owners": {
    "resource_39b:0200013c": {"name":"spawn_configured_effect","source":"battle/effects/spawn_configured_effect.c"},
    "resource_39c:0200013c": {"name":"spawn_configured_effect","source":"battle/effects/spawn_configured_effect.c"}
  }
}"#;
        let paths = SourcePaths::parse(root.path(), text).unwrap();
        assert_eq!(
            paths
                .overlay_owner_for_path(
                    "resource_39b",
                    Path::new("games/gs1/src/battle/effects/spawn_configured_effect.c")
                )
                .unwrap(),
            Some(SourceOwner::Overlay {
                resource: 0x39b,
                address: 0x0200_013c,
            })
        );
    }

    #[test]
    fn unregistering_one_shared_owner_preserves_the_other_registration() {
        let root = tempdir().unwrap();
        let manifest_path = root.path().join(SOURCE_PATHS_MANIFEST);
        fs::create_dir_all(manifest_path.parent().unwrap()).unwrap();
        fs::write(
            &manifest_path,
            r#"{
  "format": 2,
  "owners": {
    "resource_39b:02000104": {"name":"integrate_effect_motion","source":"overlays/shared/integrate_effect_motion.c"},
    "resource_39c:02000104": {"name":"integrate_effect_motion","source":"overlays/shared/integrate_effect_motion.c"}
  }
}"#,
        )
        .unwrap();
        let paths = SourcePaths::load(root.path()).unwrap();
        assert!(paths
            .unregister_owner(SourceOwner::Overlay {
                resource: 0x39b,
                address: 0x0200_0104,
            })
            .unwrap());
        let reloaded = SourcePaths::load(root.path()).unwrap();
        assert_eq!(
            reloaded.owners_for_path(Path::new(
                "games/gs1/src/overlays/shared/integrate_effect_motion.c"
            )),
            vec![SourceOwner::Overlay {
                resource: 0x39c,
                address: 0x0200_0104,
            }]
        );
    }

    #[test]
    fn each_game_owns_an_independent_descriptive_registry() {
        let root = tempdir().unwrap();
        let manifest_path = root.path().join("games/gs2/source-paths.json");
        fs::create_dir_all(manifest_path.parent().unwrap()).unwrap();
        fs::write(
            &manifest_path,
            r#"{"format":2,"owners":{"main:080132cc":{"name":"constant_zero_result","source":"runtime/constant_zero_result.c"}}}"#,
        )
        .unwrap();
        let paths = SourcePaths::load_for_game(root.path(), "gs2").unwrap();
        assert_eq!(
            paths.repository_relative_path(SourceOwner::Main(0x0801_32cc)),
            PathBuf::from("games/gs2/src/runtime/constant_zero_result.c")
        );
        assert_eq!(
            SourceOwner::Main(0x0801_32cc).routing_path_for_game("gs2"),
            PathBuf::from("games/gs2/src/080132cc.c")
        );
    }
}
