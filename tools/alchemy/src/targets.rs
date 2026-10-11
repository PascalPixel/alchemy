
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
    pub rom_sha256: &'static str,
    pub rom_size: u64,
    pub compiler: CompilerTarget,
    pub edition_define: &'static str,
    pub source_dir: &'static str,
    pub overlay_dir: &'static str,
    pub output_dir: &'static str,
    pub overlay_entry_veneers: usize,
}

impl DecompTarget {
    pub fn verify_reference(&self, rom: &[u8]) -> Result<(), String> {
        if crate::compiler::sha256::hex(rom) != self.rom_sha256 {
            return Err(format!(
                "ROM differs from the approved {} reference ROM",
                self.id
            ));
        }
        Ok(())
    }
    pub fn script(&self) -> std::path::PathBuf {
        std::path::Path::new(self.game_dir()).join("main.ld")
    }
    /// The edition's own linker script, which replaces or derives the
    /// game's output sections; the Japanese base edition has none, and
    /// neither has any edition of a game with an object list.
    pub fn edition_script(&self) -> Option<std::path::PathBuf> {
        match self.id.as_str().split_once('-') {
            Some((_, "ja")) | None => None,
            Some((game, _)) if LISTED.contains(&game) => None,
            Some((game, lang)) => Some(edition_script_path(game, lang)),
        }
    }
    /// The game's object list, games/<game>/objects.tsv, when all six of
    /// its editions link from the one main.ld by it.
    pub fn object_list(&self) -> Option<std::path::PathBuf> {
        object_list_path(self.id.as_str().split_once('-').map_or("", |(game, _)| game))
    }
    /// The directory of the edition's assembly scaffold, recon/<game>/<lang>.
    pub fn scaffold_dir(&self) -> std::path::PathBuf {
        let (game, lang) = self.id.as_str().split_once('-').expect("an edition id names its game");
        ["recon", game, lang].iter().collect()
    }
    /// An overlay's link script: games/<game>/src/system/overlay/overlay.ld,
    /// the one script every overlay of a game with an object list links by,
    /// or recon/<game>/raw/overlays/resource_<id>.ld for a game whose
    /// overlays are still listings.
    pub fn overlay_script(&self, id: &str) -> std::path::PathBuf {
        let name = if self.object_list().is_some() {
            "overlay.ld".to_owned()
        } else {
            format!("resource_{id}.ld")
        };
        std::path::Path::new(self.overlay_dir).join(name)
    }
    /// The overlay's own data and bss, assembled as resource_<id>_overlay.o:
    /// ovl_<id>.s beside its script, or the recon listing resource_<id>_overlay.s.
    pub fn overlay_data(&self, id: &str) -> std::path::PathBuf {
        let name = if self.overlay_dir.starts_with("games/") {
            format!("ovl_{}.s", id.to_ascii_lowercase())
        } else {
            format!("resource_{id}_overlay.s")
        };
        std::path::Path::new(self.overlay_dir).join(name)
    }
    pub fn editions(&self) -> [DecompTarget; 6] {
        let first = self.id as usize / 6 * 6;
        std::array::from_fn(|index| target_for(TARGET_IDS[first + index]))
    }
    pub fn language(&self) -> &'static str {
        self.id
            .as_str()
            .split_once('-')
            .map_or("", |(_, lang)| lang)
    }
    pub fn game_dir(&self) -> &'static str {
        self.source_dir
            .strip_suffix("/src")
            .expect("source_dir ends with /src")
    }
}

const PRODUCTS: [(CompilerTarget, u64, &str, &str, usize); 2] = [
    (
        CompilerTarget::Tbs,
        0x0080_0000,
        "games/tbs/src",
        "games/tbs/src/system/overlay",
        6,
    ),
    (
        CompilerTarget::Tla,
        0x0100_0000,
        "games/tla/src",
        "recon/tla/raw/overlays",
        7,
    ),
];
pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::TbsJa;

/// Games that link all six editions from their one main.ld, which names no
/// object: games/<game>/objects.tsv lists them. The others keep an edition
/// script in recon/<game>/<lang>/main.ld.
const LISTED: [&str; 1] = ["tbs"];

/// The object list of a game ("tbs" or "tla"), when it links by one.
pub fn object_list_path(game: &str) -> Option<std::path::PathBuf> {
    LISTED
        .contains(&game)
        .then(|| ["games", game, "objects.tsv"].iter().collect())
}

/// The linker script of one edition of a game ("tbs" or "tla"), relative to
/// the checkout: the game's main.ld for Japanese and for every edition of a
/// game with an object list, else the edition's own.
pub fn edition_script_path(game: &str, lang: &str) -> std::path::PathBuf {
    let dir = match game {
        "tla" => "games/tla",
        _ => "games/tbs",
    };
    if lang == "ja" || LISTED.contains(&game) {
        [dir, "main.ld"].iter().collect()
    } else {
        ["recon", game, lang, "main.ld"].iter().collect()
    }
}

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
    let (compiler, rom_size, source_dir, overlay_dir, overlay_entry_veneers) = PRODUCTS[index / 6];
    debug_assert_eq!(name, id.as_str());
    DecompTarget {
        id,
        rom,
        rom_sha256,
        rom_size,
        compiler,
        edition_define,
        source_dir,
        overlay_dir,
        output_dir,
        overlay_entry_veneers,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn edition_scripts_live_with_their_game() {
        let path = |game, lang| edition_script_path(game, lang).to_string_lossy().into_owned();
        assert_eq!(path("tbs", "ja"), "games/tbs/main.ld");
        assert_eq!(path("tbs", "en"), "games/tbs/main.ld");
        assert_eq!(path("tbs", "it"), "games/tbs/main.ld");
        assert_eq!(path("tla", "ja"), "games/tla/main.ld");
        assert_eq!(path("tla", "de"), "recon/tla/de/main.ld");
        let de = target_for(DecompTargetId::TbsDe);
        assert_eq!(de.edition_script(), None);
        assert_eq!(de.object_list().unwrap().to_string_lossy(), "games/tbs/objects.tsv");
        let tla_de = target_for(DecompTargetId::TlaDe);
        assert_eq!(tla_de.edition_script().unwrap(), edition_script_path("tla", "de"));
        assert_eq!(tla_de.object_list(), None);
        assert_eq!(de.scaffold_dir().to_string_lossy(), "recon/tbs/de");
        assert_eq!(target_for(DecompTargetId::TbsJa).edition_script(), None);
        assert_eq!(
            de.overlay_script("3ad").to_string_lossy(),
            "games/tbs/src/system/overlay/overlay.ld"
        );
        assert_eq!(
            de.overlay_data("3ad").to_string_lossy(),
            "games/tbs/src/system/overlay/ovl_3ad.s"
        );
        let tla = target_for(DecompTargetId::TlaEn);
        assert_eq!(tla.overlay_script("4a0").to_string_lossy(), "recon/tla/raw/overlays/resource_4a0.ld");
        assert_eq!(tla.overlay_data("4a0").to_string_lossy(), "recon/tla/raw/overlays/resource_4a0_overlay.s");
    }
}
