//! Golden Sun's twelve ROM targets and their isolated build paths.

use crate::compiler::routing::CompilerTarget;

macro_rules! target_registry {
    ($($id:ident => ($name:literal, $rom:literal, $define:literal, $output:literal)),+ $(,)?) => {
        #[repr(u8)]
        #[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
        pub enum DecompTargetId { $($id),+ }

        pub const TARGET_IDS: [DecompTargetId; 12] = [$(DecompTargetId::$id),+];
        const TARGETS: [(&str, &str, &str, &str); 12] = [$(
            ($name, $rom, $define, $output)
        ),+];
    };
}

target_registry! {
    TbsJa => ("tbs-ja", "roms/tbs-ja.gba", "TBS_EDITION_JA", "out/tbs-ja"),
    TbsEn => ("tbs-en", "roms/tbs-en.gba", "TBS_EDITION_EN", "out/tbs-en"),
    TbsDe => ("tbs-de", "roms/tbs-de.gba", "TBS_EDITION_DE", "out/tbs-de"),
    TbsEs => ("tbs-es", "roms/tbs-es.gba", "TBS_EDITION_ES", "out/tbs-es"),
    TbsFr => ("tbs-fr", "roms/tbs-fr.gba", "TBS_EDITION_FR", "out/tbs-fr"),
    TbsIt => ("tbs-it", "roms/tbs-it.gba", "TBS_EDITION_IT", "out/tbs-it"),
    TlaJa => ("tla-ja", "roms/tla-ja.gba", "TLA_EDITION_JA", "out/tla-ja"),
    TlaEn => ("tla-en", "roms/tla-en.gba", "TLA_EDITION_EN", "out/tla-en"),
    TlaDe => ("tla-de", "roms/tla-de.gba", "TLA_EDITION_DE", "out/tla-de"),
    TlaEs => ("tla-es", "roms/tla-es.gba", "TLA_EDITION_ES", "out/tla-es"),
    TlaFr => ("tla-fr", "roms/tla-fr.gba", "TLA_EDITION_FR", "out/tla-fr"),
    TlaIt => ("tla-it", "roms/tla-it.gba", "TLA_EDITION_IT", "out/tla-it"),
}

impl DecompTargetId {
    pub fn as_str(self) -> &'static str {
        TARGETS[self as usize].0
    }
}
impl std::fmt::Display for DecompTargetId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(self.as_str())
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum BuildSupport {
    CompileOnly,
    Full,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct DecompTarget {
    pub id: DecompTargetId,
    pub rom: &'static str,
    pub rom_size: u64,
    pub compiler: CompilerTarget,
    pub build_support: BuildSupport,
    pub edition_define: &'static str,
    pub source_dir: &'static str,
    pub asm_dir: &'static str,
    pub asset_manifest: &'static str,
    pub output_dir: &'static str,
    /// Fixed `ldr r4, [pc, #0]; bx r4` entry veneers every code overlay of
    /// this game opens with: six in The Broken Seal, seven in The Lost Age.
    pub overlay_entry_veneers: usize,
}

impl DecompTarget {
    /// The game's physical root, `games/THE BROKEN SEAL` or `games/THE LOST AGE`.
    pub fn game_dir(&self) -> &'static str {
        self.source_dir
            .strip_suffix("/SRC")
            .expect("source_dir ends with /SRC")
    }
    /// Retained overlay assembly for one resource-qualified overlay.
    pub fn overlay_assembly(&self, overlay: &str) -> String {
        format!("{}/{overlay}_overlay.s", self.overlay_dir())
    }
    /// The directory of retained overlay assembly and stream plans.
    pub fn overlay_dir(&self) -> String {
        format!("{}/overlays", self.asm_dir)
    }
    /// The game's shared `overlay_veneer` macro, included by every overlay.
    pub fn overlay_macro(&self) -> String {
        format!("{}/SYSTEM/OVERLAY.INC", self.source_dir)
    }
}

const PRODUCTS: [(CompilerTarget, u64, &str, &str, &str, usize); 2] = [
    (
        CompilerTarget::Tbs,
        0x0080_0000,
        "games/THE BROKEN SEAL/SRC",
        "games/THE BROKEN SEAL/raw",
        "games/THE BROKEN SEAL/recon/assets.json",
        6,
    ),
    (
        CompilerTarget::Tla,
        0x0100_0000,
        "games/THE LOST AGE/SRC",
        "games/THE LOST AGE/raw",
        "games/THE LOST AGE/recon/assets.json",
        7,
    ),
];
pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::TbsEn;

pub fn parse_decomp_target(value: &str) -> Result<DecompTargetId, String> {
    TARGET_IDS
        .into_iter()
        .find(|id| id.as_str() == value)
        .ok_or_else(|| {
            let expected = TARGETS.map(|target| target.0).join(" or ");
            format!("unsupported decomp target {value:?}; expected {expected}")
        })
}

pub fn decomp_target(id: Option<&str>) -> Result<DecompTarget, String> {
    Ok(target_for(
        id.map(parse_decomp_target)
            .transpose()?
            .unwrap_or(DEFAULT_TARGET),
    ))
}

pub fn target_for(id: DecompTargetId) -> DecompTarget {
    let index = id as usize;
    let (name, rom, edition_define, output_dir) = TARGETS[index];
    let (compiler, rom_size, source_dir, asm_dir, asset_manifest, overlay_entry_veneers) =
        PRODUCTS[index / 6];
    debug_assert_eq!(name, id.as_str());
    DecompTarget {
        id,
        rom,
        rom_size,
        compiler,
        build_support: if id == DEFAULT_TARGET {
            BuildSupport::Full
        } else {
            BuildSupport::CompileOnly
        },
        edition_define,
        source_dir,
        asm_dir,
        asset_manifest,
        output_dir,
        overlay_entry_veneers,
    }
}
#[cfg(test)]
fn relative_path(path: &str) -> bool {
    !path.is_empty()
        && !path.starts_with(['/', '\\'])
        && !path.split(['/', '\\']).any(|part| part == "..")
}
#[cfg(test)]
fn self_test() -> Result<String, String> {
    let mut outputs = std::collections::HashSet::new();
    for id in TARGET_IDS {
        let target = target_for(id);
        let root = match target.compiler {
            CompilerTarget::Tbs => "games/THE BROKEN SEAL/",
            CompilerTarget::Tla => "games/THE LOST AGE/",
        };
        if !relative_path(target.output_dir)
            || ![target.source_dir, target.asm_dir, target.asset_manifest]
                .iter()
                .all(|path| path.starts_with(root))
            || !outputs.insert(target.output_dir)
            || !target.game_dir().starts_with(root.trim_end_matches('/'))
            || !target.overlay_macro().starts_with(root)
            || target.overlay_assembly("resource_649")
                != format!("{root}raw/overlays/resource_649_overlay.s")
        {
            return Err(format!("{id} does not have isolated relative paths"));
        }
    }
    for invalid in ["", "tbs", "TBS-en", "tbs-en ", "alchemy"] {
        if parse_decomp_target(invalid).is_ok() {
            return Err(format!("invalid target was accepted: {invalid}"));
        }
    }
    Ok("self-test=ok build_targets=12 default=tbs-en".into())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn games_name_their_roots_macro_and_entry_veneers() {
        let tla = target_for(DecompTargetId::TlaEn);
        assert_eq!(tla.game_dir(), "games/THE LOST AGE");
        assert_eq!(
            tla.overlay_macro(),
            "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
        );
        assert_eq!(tla.overlay_entry_veneers, 7);
        assert_eq!(target_for(DEFAULT_TARGET).overlay_entry_veneers, 6);
        for id in [DecompTargetId::TbsEn, DecompTargetId::TlaEn] {
            let target = target_for(id);
            assert_eq!(
                target.asset_manifest,
                format!("{}/recon/assets.json", target.game_dir())
            );
        }
    }
    #[test]
    fn registry_covers_isolated_targets() {
        assert_eq!(
            self_test().unwrap(),
            "self-test=ok build_targets=12 default=tbs-en"
        );
    }
}
