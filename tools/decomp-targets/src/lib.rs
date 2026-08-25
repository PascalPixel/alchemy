//! Registry of decompilation targets (which ROM, which compiler, which trees).
//!
//! Ported from `tools/lib/decomp_targets.ts`. The whole point of this module is
//! that a second game can never quietly share a first game's output directory:
//! every path here is relative to the repo root and target-isolated, and the
//! self-test enforces both properties.

pub mod cli;

/// A reconstructed product. Editions belong to a product; GS1 and GS2 are
/// deliberately not modelled as editions of one ROM.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompGame {
    Gs1,
    Gs2,
}

impl DecompGame {
    pub fn as_str(self) -> &'static str {
        match self {
            DecompGame::Gs1 => "gs1",
            DecompGame::Gs2 => "gs2",
        }
    }
}

impl std::fmt::Display for DecompGame {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

/// The six approved historical editions of each product. Japanese is the
/// source base; the remaining editions are measured deltas from it.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompEdition {
    Ja,
    En,
    De,
    Es,
    Fr,
    It,
}

impl DecompEdition {
    pub fn as_str(self) -> &'static str {
        match self {
            DecompEdition::Ja => "ja",
            DecompEdition::En => "en",
            DecompEdition::De => "de",
            DecompEdition::Es => "es",
            DecompEdition::Fr => "fr",
            DecompEdition::It => "it",
        }
    }
}

impl std::fmt::Display for DecompEdition {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

/// Target identifiers. The TS side used a string union; an enum gives us the
/// same closed set with the compiler doing the checking.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompTargetId {
    Gs1Ja,
    Gs1En,
    Gs1De,
    Gs1Es,
    Gs1Fr,
    Gs1It,
    Gs2Ja,
    Gs2En,
    Gs2De,
    Gs2Es,
    Gs2Fr,
    Gs2It,
}

/// Every reference ROM known to the reconstruction. This is intentionally a
/// wider set than `DecompTargetId`: a reference target can locate and prove an
/// owner before the complete build surface for that edition exists.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum ReferenceTargetId {
    Gs1Ja,
    Gs1En,
    Gs1De,
    Gs1Es,
    Gs1Fr,
    Gs1It,
    Gs2Ja,
    Gs2En,
    Gs2De,
    Gs2Es,
    Gs2Fr,
    Gs2It,
}

impl ReferenceTargetId {
    pub fn as_str(self) -> &'static str {
        match self {
            ReferenceTargetId::Gs1Ja => "gs1-ja",
            ReferenceTargetId::Gs1En => "gs1-en",
            ReferenceTargetId::Gs1De => "gs1-de",
            ReferenceTargetId::Gs1Es => "gs1-es",
            ReferenceTargetId::Gs1Fr => "gs1-fr",
            ReferenceTargetId::Gs1It => "gs1-it",
            ReferenceTargetId::Gs2Ja => "gs2-ja",
            ReferenceTargetId::Gs2En => "gs2-en",
            ReferenceTargetId::Gs2De => "gs2-de",
            ReferenceTargetId::Gs2Es => "gs2-es",
            ReferenceTargetId::Gs2Fr => "gs2-fr",
            ReferenceTargetId::Gs2It => "gs2-it",
        }
    }
}

impl std::fmt::Display for ReferenceTargetId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

impl DecompTargetId {
    pub fn as_str(self) -> &'static str {
        match self {
            DecompTargetId::Gs1Ja => "gs1-ja",
            DecompTargetId::Gs1En => "gs1-en",
            DecompTargetId::Gs1De => "gs1-de",
            DecompTargetId::Gs1Es => "gs1-es",
            DecompTargetId::Gs1Fr => "gs1-fr",
            DecompTargetId::Gs1It => "gs1-it",
            DecompTargetId::Gs2Ja => "gs2-ja",
            DecompTargetId::Gs2En => "gs2-en",
            DecompTargetId::Gs2De => "gs2-de",
            DecompTargetId::Gs2Es => "gs2-es",
            DecompTargetId::Gs2Fr => "gs2-fr",
            DecompTargetId::Gs2It => "gs2-it",
        }
    }
}

impl std::fmt::Display for DecompTargetId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

/// Which compiler configuration a target builds with.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompCompilerTarget {
    Gs1,
    Gs2,
}

/// The strongest build the repository can currently prove for a target.
/// Compile-only targets deliberately have no edition address map, retained
/// assembly ownership, or source assets wired into a link yet.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum BuildSupport {
    CompileOnly,
    Full,
}

impl BuildSupport {
    pub fn as_str(self) -> &'static str {
        match self {
            BuildSupport::CompileOnly => "compile-only",
            BuildSupport::Full => "full",
        }
    }
}

impl DecompCompilerTarget {
    pub fn as_str(self) -> &'static str {
        match self {
            DecompCompilerTarget::Gs1 => "gs1",
            DecompCompilerTarget::Gs2 => "gs2",
        }
    }
}

impl std::fmt::Display for DecompCompilerTarget {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct DecompTarget {
    pub id: DecompTargetId,
    pub game: DecompGame,
    pub edition: DecompEdition,
    pub rom: &'static str,
    pub rom_size: u64,
    pub compiler: DecompCompilerTarget,
    pub build_support: BuildSupport,
    pub edition_define: &'static str,
    pub source_dir: &'static str,
    pub asm_dir: &'static str,
    pub asset_manifest: &'static str,
    pub output_dir: &'static str,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct ReferenceTarget {
    pub id: ReferenceTargetId,
    pub game: DecompGame,
    pub edition: DecompEdition,
    pub rom: &'static str,
    pub rom_size: u64,
    pub compiler: DecompCompilerTarget,
    pub build_target: DecompTargetId,
}

pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::Gs1En;

macro_rules! reference_target {
    ($id:ident, $game:ident, $edition:ident, $rom:literal, $size:expr, $compiler:ident) => {
        ReferenceTarget {
            id: ReferenceTargetId::$id,
            game: DecompGame::$game,
            edition: DecompEdition::$edition,
            rom: $rom,
            rom_size: $size,
            compiler: DecompCompilerTarget::$compiler,
            build_target: DecompTargetId::$id,
        }
    };
}

const REFERENCE_TARGETS: [ReferenceTarget; 12] = [
    reference_target!(Gs1Ja, Gs1, Ja, "roms/gs1-ja.gba", 0x0080_0000, Gs1),
    reference_target!(Gs1En, Gs1, En, "roms/gs1-en.gba", 0x0080_0000, Gs1),
    reference_target!(Gs1De, Gs1, De, "roms/gs1-de.gba", 0x0080_0000, Gs1),
    reference_target!(Gs1Es, Gs1, Es, "roms/gs1-es.gba", 0x0080_0000, Gs1),
    reference_target!(Gs1Fr, Gs1, Fr, "roms/gs1-fr.gba", 0x0080_0000, Gs1),
    reference_target!(Gs1It, Gs1, It, "roms/gs1-it.gba", 0x0080_0000, Gs1),
    reference_target!(Gs2Ja, Gs2, Ja, "roms/gs2-ja.gba", 0x0100_0000, Gs2),
    reference_target!(Gs2En, Gs2, En, "roms/gs2-en.gba", 0x0100_0000, Gs2),
    reference_target!(Gs2De, Gs2, De, "roms/gs2-de.gba", 0x0100_0000, Gs2),
    reference_target!(Gs2Es, Gs2, Es, "roms/gs2-es.gba", 0x0100_0000, Gs2),
    reference_target!(Gs2Fr, Gs2, Fr, "roms/gs2-fr.gba", 0x0100_0000, Gs2),
    reference_target!(Gs2It, Gs2, It, "roms/gs2-it.gba", 0x0100_0000, Gs2),
];

pub const REFERENCE_TARGET_IDS: [ReferenceTargetId; 12] = [
    ReferenceTargetId::Gs1Ja,
    ReferenceTargetId::Gs1En,
    ReferenceTargetId::Gs1De,
    ReferenceTargetId::Gs1Es,
    ReferenceTargetId::Gs1Fr,
    ReferenceTargetId::Gs1It,
    ReferenceTargetId::Gs2Ja,
    ReferenceTargetId::Gs2En,
    ReferenceTargetId::Gs2De,
    ReferenceTargetId::Gs2Es,
    ReferenceTargetId::Gs2Fr,
    ReferenceTargetId::Gs2It,
];

macro_rules! decomp_target {
    ($name:ident, $id:ident, $game:ident, $edition:ident, $target:literal, $size:expr, $compiler:ident, $support:ident, $define:literal, $root:literal) => {
        const $name: DecompTarget = DecompTarget {
            id: DecompTargetId::$id,
            game: DecompGame::$game,
            edition: DecompEdition::$edition,
            rom: concat!("roms/", $target, ".gba"),
            rom_size: $size,
            compiler: DecompCompilerTarget::$compiler,
            build_support: BuildSupport::$support,
            edition_define: $define,
            source_dir: concat!($root, "/src"),
            asm_dir: concat!($root, "/asm"),
            asset_manifest: concat!($root, "/assets/manifest.json"),
            output_dir: concat!("out/", $target),
        };
    };
}

decomp_target!(
    GS1_JA,
    Gs1Ja,
    Gs1,
    Ja,
    "gs1-ja",
    0x0080_0000,
    Gs1,
    CompileOnly,
    "GS1_EDITION_JA",
    "games/gs1"
);
decomp_target!(
    GS1_EN,
    Gs1En,
    Gs1,
    En,
    "gs1-en",
    0x0080_0000,
    Gs1,
    Full,
    "GS1_EDITION_EN",
    "games/gs1"
);
decomp_target!(
    GS1_DE,
    Gs1De,
    Gs1,
    De,
    "gs1-de",
    0x0080_0000,
    Gs1,
    CompileOnly,
    "GS1_EDITION_DE",
    "games/gs1"
);
decomp_target!(
    GS1_ES,
    Gs1Es,
    Gs1,
    Es,
    "gs1-es",
    0x0080_0000,
    Gs1,
    CompileOnly,
    "GS1_EDITION_ES",
    "games/gs1"
);
decomp_target!(
    GS1_FR,
    Gs1Fr,
    Gs1,
    Fr,
    "gs1-fr",
    0x0080_0000,
    Gs1,
    CompileOnly,
    "GS1_EDITION_FR",
    "games/gs1"
);
decomp_target!(
    GS1_IT,
    Gs1It,
    Gs1,
    It,
    "gs1-it",
    0x0080_0000,
    Gs1,
    CompileOnly,
    "GS1_EDITION_IT",
    "games/gs1"
);
decomp_target!(
    GS2_JA,
    Gs2Ja,
    Gs2,
    Ja,
    "gs2-ja",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_JA",
    "games/gs2"
);
decomp_target!(
    GS2_EN,
    Gs2En,
    Gs2,
    En,
    "gs2-en",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_EN",
    "games/gs2"
);
decomp_target!(
    GS2_DE,
    Gs2De,
    Gs2,
    De,
    "gs2-de",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_DE",
    "games/gs2"
);
decomp_target!(
    GS2_ES,
    Gs2Es,
    Gs2,
    Es,
    "gs2-es",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_ES",
    "games/gs2"
);
decomp_target!(
    GS2_FR,
    Gs2Fr,
    Gs2,
    Fr,
    "gs2-fr",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_FR",
    "games/gs2"
);
decomp_target!(
    GS2_IT,
    Gs2It,
    Gs2,
    It,
    "gs2-it",
    0x0100_0000,
    Gs2,
    CompileOnly,
    "GS2_EDITION_IT",
    "games/gs2"
);

const TARGETS: [DecompTarget; 12] = [
    GS1_JA, GS1_EN, GS1_DE, GS1_ES, GS1_FR, GS1_IT, GS2_JA, GS2_EN, GS2_DE, GS2_ES, GS2_FR, GS2_IT,
];

pub const TARGET_IDS: [DecompTargetId; 12] = [
    DecompTargetId::Gs1Ja,
    DecompTargetId::Gs1En,
    DecompTargetId::Gs1De,
    DecompTargetId::Gs1Es,
    DecompTargetId::Gs1Fr,
    DecompTargetId::Gs1It,
    DecompTargetId::Gs2Ja,
    DecompTargetId::Gs2En,
    DecompTargetId::Gs2De,
    DecompTargetId::Gs2Es,
    DecompTargetId::Gs2Fr,
    DecompTargetId::Gs2It,
];

/// Serialize a string the way `JSON.stringify` would, so the error text is
/// byte-for-byte what the TS tool produced (quotes included).
fn json_quote(value: &str) -> String {
    let mut out = String::with_capacity(value.len() + 2);
    out.push('"');
    for ch in value.chars() {
        match ch {
            '"' => out.push_str("\\\""),
            '\\' => out.push_str("\\\\"),
            '\u{8}' => out.push_str("\\b"),
            '\u{c}' => out.push_str("\\f"),
            '\n' => out.push_str("\\n"),
            '\r' => out.push_str("\\r"),
            '\t' => out.push_str("\\t"),
            c if (c as u32) < 0x20 => out.push_str(&format!("\\u{:04x}", c as u32)),
            c => out.push(c),
        }
    }
    out.push('"');
    out
}

fn target_ids_joined() -> String {
    TARGET_IDS
        .iter()
        .map(|id| id.as_str())
        .collect::<Vec<_>>()
        .join(" or ")
}

/// Parse any of the twelve historical ROM ids. Unlike `parse_decomp_target`,
/// this does not imply that a complete build surface has already been
/// installed for the edition.
pub fn parse_reference_target(value: &str) -> Result<ReferenceTargetId, String> {
    REFERENCE_TARGETS
        .iter()
        .find(|target| target.id.as_str() == value)
        .map(|target| target.id)
        .ok_or_else(|| format!("unsupported reference target {}", json_quote(value)))
}

pub fn reference_target(id: ReferenceTargetId) -> ReferenceTarget {
    *REFERENCE_TARGETS
        .iter()
        .find(|target| target.id == id)
        .expect("REFERENCE_TARGET_IDS and REFERENCE_TARGETS diverged")
}

/// PORT NOTE: the TS threw an `Error`; Rust returns `Err(String)` carrying the
/// identical message so callers can decide whether to panic or report.
pub fn parse_decomp_target(value: &str) -> Result<DecompTargetId, String> {
    TARGETS
        .iter()
        .find(|t| t.id.as_str() == value)
        .map(|t| t.id)
        .ok_or_else(|| {
            format!(
                "unsupported decomp target {}; expected {}",
                json_quote(value),
                target_ids_joined()
            )
        })
}

/// Look up a target by id string. `None` means "use the default target",
/// mirroring the TS default parameter.
pub fn decomp_target(id: Option<&str>) -> Result<DecompTarget, String> {
    let id = match id {
        Some(id) => parse_decomp_target(id)?,
        None => DEFAULT_TARGET,
    };
    Ok(target_for(id))
}

/// Infallible lookup when the id is already known-good.
pub fn target_for(id: DecompTargetId) -> DecompTarget {
    match id {
        DecompTargetId::Gs1Ja => GS1_JA,
        DecompTargetId::Gs1En => GS1_EN,
        DecompTargetId::Gs1De => GS1_DE,
        DecompTargetId::Gs1Es => GS1_ES,
        DecompTargetId::Gs1Fr => GS1_FR,
        DecompTargetId::Gs1It => GS1_IT,
        DecompTargetId::Gs2Ja => GS2_JA,
        DecompTargetId::Gs2En => GS2_EN,
        DecompTargetId::Gs2De => GS2_DE,
        DecompTargetId::Gs2Es => GS2_ES,
        DecompTargetId::Gs2Fr => GS2_FR,
        DecompTargetId::Gs2It => GS2_IT,
    }
}

/// A path is usable only if it is non-empty, not absolute in either slash
/// style, and never escapes the repo root via a `..` segment.
fn relative_path(path: &str) -> bool {
    !path.is_empty()
        && !path.starts_with('/')
        && !path.starts_with('\\')
        && !path.split(['\\', '/']).any(|seg| seg == "..")
}

/// The registry's invariants, checked at runtime. Kept as a function (not only
/// as unit tests) because the TS shipped it as a CLI entry point.
pub fn self_test() -> Result<String, String> {
    let gs1 = decomp_target(None)?;
    let gs2 = decomp_target(Some("gs2-en"))?;

    if gs1.id != DEFAULT_TARGET
        || gs1.rom != "roms/gs1-en.gba"
        || gs1.rom_size != 0x0080_0000
        || gs1.compiler != DecompCompilerTarget::Gs1
    {
        return Err("GS1 target registry self-test failed".to_string());
    }
    if gs2.rom != "roms/gs2-en.gba"
        || gs2.rom_size != 0x0100_0000
        || gs2.compiler != DecompCompilerTarget::Gs2
    {
        return Err("GS2 target registry self-test failed".to_string());
    }

    let mut roms = std::collections::HashSet::new();
    let mut bases = 0;
    for (index, id) in REFERENCE_TARGET_IDS.iter().copied().enumerate() {
        let target = reference_target(id);
        if target != REFERENCE_TARGETS[index]
            || parse_reference_target(id.as_str())? != id
            || !relative_path(target.rom)
            || !roms.insert(target.rom)
        {
            return Err(format!("{} reference registry entry is inconsistent", id));
        }

        let (size, compiler) = match target.game {
            DecompGame::Gs1 => (0x0080_0000, DecompCompilerTarget::Gs1),
            DecompGame::Gs2 => (0x0100_0000, DecompCompilerTarget::Gs2),
        };
        if target.rom_size != size || target.compiler != compiler {
            return Err(format!("{} has the wrong product configuration", id));
        }
        if target.edition == DecompEdition::Ja {
            bases += 1;
        }
        if target_for(target.build_target).rom != target.rom {
            return Err(format!("{} has an invalid build-surface mapping", id));
        }
    }
    if REFERENCE_TARGET_IDS.len() != TARGET_IDS.len() || bases != 2 {
        return Err("reference/build target accounting is inconsistent".to_string());
    }

    for invalid in ["", "gs1", "GS1-en", "gs1-en ", "alchemy"] {
        if parse_reference_target(invalid).is_ok() {
            return Err(format!("invalid reference target was accepted: {invalid}"));
        }
    }

    let mut outputs = std::collections::HashSet::new();
    let mut full_builds = 0usize;
    for target in TARGETS {
        for path in [
            target.rom,
            target.source_dir,
            target.asm_dir,
            target.asset_manifest,
            target.output_dir,
        ] {
            if !relative_path(path) {
                return Err(format!("{} has a non-relative path", target.id));
            }
        }
        if !outputs.insert(target.output_dir) {
            return Err(format!("{} does not have an isolated output", target.id));
        }
        if target_for(parse_decomp_target(target.id.as_str())?) != target {
            return Err(format!(
                "{} cannot round-trip through the registry",
                target.id
            ));
        }
        if target.build_support == BuildSupport::Full {
            full_builds += 1;
        }
    }
    if full_builds != 1 || gs1.build_support != BuildSupport::Full {
        return Err("full-build target accounting is inconsistent".into());
    }

    for (field, a, b) in [
        ("sourceDir", gs1.source_dir, gs2.source_dir),
        ("asmDir", gs1.asm_dir, gs2.asm_dir),
        ("assetManifest", gs1.asset_manifest, gs2.asset_manifest),
    ] {
        if a == b {
            return Err(format!("{field} is not target-isolated"));
        }
    }

    for invalid in ["", "gs1", "GS1-en", "gs1-en ", "alchemy"] {
        if parse_decomp_target(invalid).is_ok() {
            return Err(format!("invalid target was accepted: {invalid}"));
        }
    }

    Ok(format!(
        "self-test=ok reference_targets={} build_targets={} bases={} default={}",
        REFERENCE_TARGET_IDS.len(),
        TARGET_IDS.len(),
        bases,
        DEFAULT_TARGET
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn registry_covers_two_six_edition_products() {
        assert_eq!(
            self_test().unwrap(),
            "self-test=ok reference_targets=12 build_targets=12 bases=2 default=gs1-en"
        );
        assert_eq!(
            reference_target(parse_reference_target("gs2-it").unwrap()),
            ReferenceTarget {
                id: ReferenceTargetId::Gs2It,
                game: DecompGame::Gs2,
                edition: DecompEdition::It,
                rom: "roms/gs2-it.gba",
                rom_size: 0x0100_0000,
                compiler: DecompCompilerTarget::Gs2,
                build_target: DecompTargetId::Gs2It
            }
        );
    }
}
