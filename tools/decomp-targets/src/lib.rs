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
    Gs1En,
    Gs2En,
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
            DecompTargetId::Gs1En => "gs1-en",
            DecompTargetId::Gs2En => "gs2-en",
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
    pub rom: &'static str,
    pub rom_size: u64,
    pub compiler: DecompCompilerTarget,
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
    /// Present only after that edition has an installed, complete build
    /// surface. Reference-only targets remain valid cross-edition evidence.
    pub build_target: Option<DecompTargetId>,
}

pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::Gs1En;

macro_rules! reference_target {
    ($id:ident, $game:ident, $edition:ident, $rom:literal, $size:expr, $compiler:ident, $build:expr) => {
        ReferenceTarget {
            id: ReferenceTargetId::$id,
            game: DecompGame::$game,
            edition: DecompEdition::$edition,
            rom: $rom,
            rom_size: $size,
            compiler: DecompCompilerTarget::$compiler,
            build_target: $build,
        }
    };
}

const REFERENCE_TARGETS: [ReferenceTarget; 12] = [
    reference_target!(Gs1Ja, Gs1, Ja, "roms/gs1-ja.gba", 0x0080_0000, Gs1, None),
    reference_target!(
        Gs1En,
        Gs1,
        En,
        "roms/gs1-en.gba",
        0x0080_0000,
        Gs1,
        Some(DecompTargetId::Gs1En)
    ),
    reference_target!(Gs1De, Gs1, De, "roms/gs1-de.gba", 0x0080_0000, Gs1, None),
    reference_target!(Gs1Es, Gs1, Es, "roms/gs1-es.gba", 0x0080_0000, Gs1, None),
    reference_target!(Gs1Fr, Gs1, Fr, "roms/gs1-fr.gba", 0x0080_0000, Gs1, None),
    reference_target!(Gs1It, Gs1, It, "roms/gs1-it.gba", 0x0080_0000, Gs1, None),
    reference_target!(Gs2Ja, Gs2, Ja, "roms/gs2-ja.gba", 0x0100_0000, Gs2, None),
    reference_target!(
        Gs2En,
        Gs2,
        En,
        "roms/gs2-en.gba",
        0x0100_0000,
        Gs2,
        Some(DecompTargetId::Gs2En)
    ),
    reference_target!(Gs2De, Gs2, De, "roms/gs2-de.gba", 0x0100_0000, Gs2, None),
    reference_target!(Gs2Es, Gs2, Es, "roms/gs2-es.gba", 0x0100_0000, Gs2, None),
    reference_target!(Gs2Fr, Gs2, Fr, "roms/gs2-fr.gba", 0x0100_0000, Gs2, None),
    reference_target!(Gs2It, Gs2, It, "roms/gs2-it.gba", 0x0100_0000, Gs2, None),
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

const GS1_EN: DecompTarget = DecompTarget {
    id: DecompTargetId::Gs1En,
    rom: "roms/gs1-en.gba",
    rom_size: 0x0080_0000,
    compiler: DecompCompilerTarget::Gs1,
    source_dir: "exact",
    asm_dir: "asm",
    asset_manifest: "assets/manifest.json",
    output_dir: "out",
};

const GS2_EN: DecompTarget = DecompTarget {
    id: DecompTargetId::Gs2En,
    rom: "roms/gs2-en.gba",
    rom_size: 0x0100_0000,
    compiler: DecompCompilerTarget::Gs2,
    source_dir: "games/gs2/src",
    asm_dir: "games/gs2/asm",
    asset_manifest: "games/gs2/assets/manifest.json",
    output_dir: "out/gs2-en",
};

/// Insertion order matters: it is what the error message lists, and the TS
/// registry was an object literal in exactly this order.
const TARGETS: [DecompTarget; 2] = [GS1_EN, GS2_EN];

/// The known target ids, in registry order.
pub const TARGET_IDS: [DecompTargetId; 2] = [DecompTargetId::Gs1En, DecompTargetId::Gs2En];

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
        DecompTargetId::Gs1En => GS1_EN,
        DecompTargetId::Gs2En => GS2_EN,
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
    let mut buildable = 0;
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
        if let Some(build_id) = target.build_target {
            buildable += 1;
            if target.edition != DecompEdition::En || target_for(build_id).rom != target.rom {
                return Err(format!("{} has an invalid build-surface mapping", id));
            }
        }
    }
    if buildable != TARGET_IDS.len() || bases != 2 {
        return Err("reference/build target accounting is inconsistent".to_string());
    }

    for invalid in ["", "gs1", "GS1-en", "gs1-en ", "alchemy"] {
        if parse_reference_target(invalid).is_ok() {
            return Err(format!("invalid reference target was accepted: {invalid}"));
        }
    }

    for target in [gs1, gs2] {
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
    }

    for (field, a, b) in [
        ("sourceDir", gs1.source_dir, gs2.source_dir),
        ("asmDir", gs1.asm_dir, gs2.asm_dir),
        ("assetManifest", gs1.asset_manifest, gs2.asset_manifest),
        ("outputDir", gs1.output_dir, gs2.output_dir),
    ] {
        if a == b {
            return Err(format!("{field} is not target-isolated"));
        }
    }

    // Near-misses that must all be rejected: empty, truncated, wrong case,
    // trailing space, and a plausible-but-absent locale.
    for invalid in ["", "gs1", "GS1-en", "gs1-en ", "gs2-ja"] {
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
            "self-test=ok reference_targets=12 build_targets=2 bases=2 default=gs1-en"
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
                build_target: None,
            }
        );
    }
}
