//! The twelve supported ROM targets and their isolated build paths.

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
    Gs1Ja => ("gs1-ja", "roms/gs1-ja.gba", "GS1_EDITION_JA", "out/gs1-ja"),
    Gs1En => ("gs1-en", "roms/gs1-en.gba", "GS1_EDITION_EN", "out/gs1-en"),
    Gs1De => ("gs1-de", "roms/gs1-de.gba", "GS1_EDITION_DE", "out/gs1-de"),
    Gs1Es => ("gs1-es", "roms/gs1-es.gba", "GS1_EDITION_ES", "out/gs1-es"),
    Gs1Fr => ("gs1-fr", "roms/gs1-fr.gba", "GS1_EDITION_FR", "out/gs1-fr"),
    Gs1It => ("gs1-it", "roms/gs1-it.gba", "GS1_EDITION_IT", "out/gs1-it"),
    Gs2Ja => ("gs2-ja", "roms/gs2-ja.gba", "GS2_EDITION_JA", "out/gs2-ja"),
    Gs2En => ("gs2-en", "roms/gs2-en.gba", "GS2_EDITION_EN", "out/gs2-en"),
    Gs2De => ("gs2-de", "roms/gs2-de.gba", "GS2_EDITION_DE", "out/gs2-de"),
    Gs2Es => ("gs2-es", "roms/gs2-es.gba", "GS2_EDITION_ES", "out/gs2-es"),
    Gs2Fr => ("gs2-fr", "roms/gs2-fr.gba", "GS2_EDITION_FR", "out/gs2-fr"),
    Gs2It => ("gs2-it", "roms/gs2-it.gba", "GS2_EDITION_IT", "out/gs2-it"),
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

#[repr(u8)]
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum DecompCompilerTarget {
    Gs1,
    Gs2,
}
impl DecompCompilerTarget {
    pub fn as_str(self) -> &'static str {
        ["gs1", "gs2"][self as usize]
    }
}
impl std::fmt::Display for DecompCompilerTarget {
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
    pub compiler: DecompCompilerTarget,
    pub build_support: BuildSupport,
    pub edition_define: &'static str,
    pub source_dir: &'static str,
    pub asm_dir: &'static str,
    pub asset_manifest: &'static str,
    pub output_dir: &'static str,
}

const PRODUCTS: [(DecompCompilerTarget, u64, &str, &str, &str); 2] = [
    (
        DecompCompilerTarget::Gs1,
        0x0080_0000,
        "games/gs1/src",
        "games/gs1/asm",
        "games/gs1/assets/manifest.json",
    ),
    (
        DecompCompilerTarget::Gs2,
        0x0100_0000,
        "games/gs2/src",
        "games/gs2/asm",
        "games/gs2/assets/manifest.json",
    ),
];
pub const DEFAULT_TARGET: DecompTargetId = DecompTargetId::Gs1En;

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
    let (compiler, rom_size, source_dir, asm_dir, asset_manifest) = PRODUCTS[index / 6];
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
    }
}
fn relative_path(path: &str) -> bool {
    !path.is_empty()
        && !path.starts_with(['/', '\\'])
        && !path.split(['/', '\\']).any(|part| part == "..")
}
pub fn self_test() -> Result<String, String> {
    let mut outputs = std::collections::HashSet::new();
    for id in TARGET_IDS {
        let target = target_for(id);
        let root = match target.compiler {
            DecompCompilerTarget::Gs1 => "games/gs1/",
            DecompCompilerTarget::Gs2 => "games/gs2/",
        };
        if !relative_path(target.output_dir)
            || ![target.source_dir, target.asm_dir, target.asset_manifest]
                .iter()
                .all(|path| path.starts_with(root))
            || !outputs.insert(target.output_dir)
        {
            return Err(format!("{id} does not have isolated relative paths"));
        }
    }
    for invalid in ["", "gs1", "GS1-en", "gs1-en ", "alchemy"] {
        if parse_decomp_target(invalid).is_ok() {
            return Err(format!("invalid target was accepted: {invalid}"));
        }
    }
    Ok("self-test=ok build_targets=12 default=gs1-en".into())
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn registry_covers_isolated_targets() {
        assert_eq!(
            self_test().unwrap(),
            "self-test=ok build_targets=12 default=gs1-en"
        );
    }
}
