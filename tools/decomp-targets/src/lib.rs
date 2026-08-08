//! Registry of decompilation targets (which ROM, which compiler, which trees).
//!
//! Ported from `tools/lib/decomp_targets.ts`. The whole point of this module is
//! that a second game can never quietly share a first game's output directory:
//! every path here is relative to the repo root and target-isolated, and the
//! self-test enforces both properties.

/// Target identifiers. The TS side used a string union; an enum gives us the
/// same closed set with the compiler doing the checking.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompTargetId {
    Gs1En,
    Gs2En,
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

pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::Gs1En;

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
        "self-test=ok targets={} default={}",
        TARGET_IDS.len(),
        DEFAULT_TARGET
    ))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn self_test_passes_and_reports() {
        assert_eq!(
            self_test().unwrap(),
            "self-test=ok targets=2 default=gs1-en"
        );
    }

    #[test]
    fn default_is_gs1_en() {
        let t = decomp_target(None).unwrap();
        assert_eq!(t.id, DecompTargetId::Gs1En);
        assert_eq!(t.rom, "roms/gs1-en.gba");
        assert_eq!(t.rom_size, 8 * 1024 * 1024);
        assert_eq!(t.compiler.as_str(), "gs1");
        assert_eq!(t.source_dir, "exact");
        assert_eq!(t.asm_dir, "asm");
        assert_eq!(t.asset_manifest, "assets/manifest.json");
        assert_eq!(t.output_dir, "out");
    }

    #[test]
    fn gs2_fields_are_exact() {
        let t = decomp_target(Some("gs2-en")).unwrap();
        assert_eq!(t.id, DecompTargetId::Gs2En);
        assert_eq!(t.rom, "roms/gs2-en.gba");
        assert_eq!(t.rom_size, 16 * 1024 * 1024);
        assert_eq!(t.compiler.as_str(), "gs2");
        assert_eq!(t.source_dir, "games/gs2/src");
        assert_eq!(t.asm_dir, "games/gs2/asm");
        assert_eq!(t.asset_manifest, "games/gs2/assets/manifest.json");
        assert_eq!(t.output_dir, "out/gs2-en");
    }

    #[test]
    fn unknown_target_error_message_matches_ts() {
        let err = parse_decomp_target("gs2-ja").unwrap_err();
        assert_eq!(
            err,
            "unsupported decomp target \"gs2-ja\"; expected gs1-en or gs2-en"
        );
    }

    #[test]
    fn error_message_quotes_like_json_stringify() {
        let err = parse_decomp_target("a\"b\\c\nd").unwrap_err();
        assert!(
            err.starts_with("unsupported decomp target \"a\\\"b\\\\c\\nd\";"),
            "got {err}"
        );
        // Control characters below 0x20 without a short escape use \u00XX.
        let err = parse_decomp_target("\u{1}").unwrap_err();
        assert!(err.contains("\"\\u0001\""), "got {err}");
    }

    #[test]
    fn near_miss_ids_are_rejected() {
        for invalid in [
            "", "gs1", "GS1-en", "gs1-en ", " gs1-en", "gs2-ja", "gs2-EN",
        ] {
            assert!(
                parse_decomp_target(invalid).is_err(),
                "{invalid:?} was accepted"
            );
        }
    }

    #[test]
    fn prototype_keys_are_not_targets() {
        // The TS guarded with hasOwnProperty precisely so these could not slip
        // through as "known" targets. The enum lookup gets it for free; assert
        // it anyway so a future map-based rewrite cannot regress.
        for probe in ["toString", "constructor", "__proto__", "hasOwnProperty"] {
            assert!(parse_decomp_target(probe).is_err(), "{probe} was accepted");
        }
    }

    #[test]
    fn every_registered_id_round_trips() {
        for id in TARGET_IDS {
            assert_eq!(parse_decomp_target(id.as_str()).unwrap(), id);
            assert_eq!(target_for(id).id, id);
        }
    }

    #[test]
    fn relative_path_rules() {
        assert!(relative_path("out"));
        assert!(relative_path("games/gs2/src"));
        assert!(relative_path("a..b/c"));
        assert!(relative_path("...."));
        assert!(!relative_path(""));
        assert!(!relative_path("/abs"));
        assert!(!relative_path("\\abs"));
        assert!(!relative_path(".."));
        assert!(!relative_path("../escape"));
        assert!(!relative_path("out/../../escape"));
        assert!(!relative_path("out\\..\\escape"));
    }

    #[test]
    fn targets_are_mutually_isolated() {
        let gs1 = target_for(DecompTargetId::Gs1En);
        let gs2 = target_for(DecompTargetId::Gs2En);
        assert_ne!(gs1.rom, gs2.rom);
        assert_ne!(gs1.compiler, gs2.compiler);
        assert_ne!(gs1.output_dir, gs2.output_dir);
        // gs2's output must not sit outside a directory gs1 also claims to own
        // in a way that lets one build stomp the other's tree root.
        assert!(gs2.output_dir.starts_with(&format!("{}/", gs1.output_dir)));
    }
}
