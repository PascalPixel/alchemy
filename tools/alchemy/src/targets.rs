//! Golden Sun's twelve ROM targets and their isolated build paths.

use crate::compiler::routing::CompilerTarget;

macro_rules! target_registry {
    ($($id:ident => ($name:literal, $rom:literal, $define:literal, $output:literal, $sha256:literal)),+ $(,)?) => {
        #[repr(u8)]
        #[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
        pub enum DecompTargetId { $($id),+ }

        pub const TARGET_IDS: [DecompTargetId; 12] = [$(DecompTargetId::$id),+];
        const TARGETS: [(&str, &str, &str, &str, &str); 12] = [$(
            ($name, $rom, $define, $output, $sha256)
        ),+];
    };
}

// Each edition's approved reference ROM is known by its SHA-256 alone: an
// intentional record that admits the builder's own copy, never a source of
// build input.
target_registry! {
    TbsJa => ("tbs-ja", "roms/tbs-ja.gba", "TBS_EDITION_JA", "out/tbs-ja",
        "088bedae4bad8b67e87ff10035a898d3639f3182d486fe5a5d113bab223e0a26"),
    TbsEn => ("tbs-en", "roms/tbs-en.gba", "TBS_EDITION_EN", "out/tbs-en",
        "c14f1151897e8d73f25ffdd67e21eebb6dc57973ff2458872ee89fa9060aaca1"),
    TbsDe => ("tbs-de", "roms/tbs-de.gba", "TBS_EDITION_DE", "out/tbs-de",
        "d7a61803600a002bc80be8063a7d8d281bc77c2261cfb812cea552d3f95f3dd1"),
    TbsEs => ("tbs-es", "roms/tbs-es.gba", "TBS_EDITION_ES", "out/tbs-es",
        "c067f04d05a65677eca3b8e3609a6ef9b86898604ef252ccf54c2b41d49f2eb8"),
    TbsFr => ("tbs-fr", "roms/tbs-fr.gba", "TBS_EDITION_FR", "out/tbs-fr",
        "5eb59f508c25548fb0ef72911cc75a81867f16b0ef8fca2a22cb6d026a862cd8"),
    TbsIt => ("tbs-it", "roms/tbs-it.gba", "TBS_EDITION_IT", "out/tbs-it",
        "fc6ef60c1c271de7352be610eb4dca29ab5edea1e4b33f05a548a65f522a452d"),
    TlaJa => ("tla-ja", "roms/tla-ja.gba", "TLA_EDITION_JA", "out/tla-ja",
        "19dd48b74726f323cd829e226b60aa1b373c51fb2e840804efe2d2dc2891a890"),
    TlaEn => ("tla-en", "roms/tla-en.gba", "TLA_EDITION_EN", "out/tla-en",
        "4199d82f845edf3e2e92f3783bca00190b5bc102d7c8aa339b951de280b1e6cc"),
    TlaDe => ("tla-de", "roms/tla-de.gba", "TLA_EDITION_DE", "out/tla-de",
        "993cfc34b6b28f6a9bfb135dc04023ee2b64693841ce90ab89536b97fbb4afed"),
    TlaEs => ("tla-es", "roms/tla-es.gba", "TLA_EDITION_ES", "out/tla-es",
        "c6bb68229971c36febe8bdf5081a4aa658c5c5417a2f4d3c7e0c3762c3ecb18a"),
    TlaFr => ("tla-fr", "roms/tla-fr.gba", "TLA_EDITION_FR", "out/tla-fr",
        "8f9a854618332d4a2886170a03ab0b10624d1202ea8562dabf0002807f7b56a8"),
    TlaIt => ("tla-it", "roms/tla-it.gba", "TLA_EDITION_IT", "out/tla-it",
        "7f3fbb2ee3e493784e63069899b5a53cf79742cb1a64560094c570b0ed3a05c2"),
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

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct DecompTarget {
    pub id: DecompTargetId,
    pub rom: &'static str,
    /// The approved reference ROM's SHA-256.
    pub rom_sha256: &'static str,
    pub rom_size: u64,
    pub compiler: CompilerTarget,
    pub edition_define: &'static str,
    pub source_dir: &'static str,
    pub asm_dir: &'static str,
    pub output_dir: &'static str,
    /// Fixed `ldr r4, [pc, #0]; bx r4` entry veneers every code overlay of
    /// this game opens with: six in The Broken Seal, seven in The Lost Age.
    pub overlay_entry_veneers: usize,
}

impl DecompTarget {
    /// Refuse `rom` unless it is this edition's approved reference ROM.
    pub fn verify_reference(&self, rom: &[u8]) -> Result<(), String> {
        if crate::compiler::sha256::hex(rom) != self.rom_sha256 {
            return Err(format!(
                "ROM differs from the approved {} reference ROM",
                self.id
            ));
        }
        Ok(())
    }
    /// The game's physical root, `games/THE BROKEN SEAL` or `games/THE LOST AGE`.
    pub fn game_dir(&self) -> &'static str {
        self.source_dir
            .strip_suffix("/SRC")
            .expect("source_dir ends with /SRC")
    }
}

const PRODUCTS: [(CompilerTarget, u64, &str, &str, usize); 2] = [
    (
        CompilerTarget::Tbs,
        0x0080_0000,
        "games/THE BROKEN SEAL/SRC",
        "recon/tbs/raw",
        6,
    ),
    (
        CompilerTarget::Tla,
        0x0100_0000,
        "games/THE LOST AGE/SRC",
        "recon/tla/raw",
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
    let (name, rom, edition_define, output_dir, rom_sha256) = TARGETS[index];
    let (compiler, rom_size, source_dir, asm_dir, overlay_entry_veneers) = PRODUCTS[index / 6];
    debug_assert_eq!(name, id.as_str());
    DecompTarget {
        id,
        rom,
        rom_sha256,
        rom_size,
        compiler,
        edition_define,
        source_dir,
        asm_dir,
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
        let (root, recon) = match target.compiler {
            CompilerTarget::Tbs => ("games/THE BROKEN SEAL/", "recon/tbs/"),
            CompilerTarget::Tla => ("games/THE LOST AGE/", "recon/tla/"),
        };
        if !relative_path(target.output_dir)
            || !target.source_dir.starts_with(root)
            || !target.asm_dir.starts_with(recon)
            || !outputs.insert(target.output_dir)
            || !target.game_dir().starts_with(root.trim_end_matches('/'))
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
    fn games_name_their_roots_and_entry_veneers() {
        let tla = target_for(DecompTargetId::TlaEn);
        assert_eq!(tla.game_dir(), "games/THE LOST AGE");
        assert_eq!(tla.overlay_entry_veneers, 7);
        assert_eq!(target_for(DEFAULT_TARGET).overlay_entry_veneers, 6);
    }
    #[test]
    fn each_edition_admits_only_its_own_reference_rom() {
        let mut identities = std::collections::HashSet::new();
        for id in TARGET_IDS {
            let target = target_for(id);
            assert_eq!(target.rom, format!("roms/{id}.gba"));
            assert_eq!(target.rom_sha256.len(), 64);
            assert!(target
                .rom_sha256
                .bytes()
                .all(|byte| matches!(byte, b'0'..=b'9' | b'a'..=b'f')));
            assert!(identities.insert(target.rom_sha256));
            assert!(target
                .verify_reference(b"a local ROM is not an approved reference")
                .is_err());
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
