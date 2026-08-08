// リソース図形と人物バンクの格納先を解決する。
//
// Ported from tools/lib/asset_paths.ts. The TypeScript memoised the graphics
// listing in a module-level `let`, which meant a directory read once per
// process and never again; here the listing is read on construction and held
// by the resolver, so the caller decides the lifetime instead of the module.

use std::fs;
use std::path::{Path, PathBuf};

pub struct AssetPaths {
    root: PathBuf,
    graphics_listing: Vec<String>,
}

impl AssetPaths {
    /// `root` is the repository root. A missing `assets/graphics` yields an
    /// empty listing, which resolves every resource to its unrelocated path.
    pub fn new(root: impl AsRef<Path>) -> Self {
        let root = root.as_ref().to_path_buf();
        let graphics_listing = fs::read_dir(root.join("assets/graphics"))
            .map(|entries| {
                entries
                    .filter_map(Result::ok)
                    .map(|entry| entry.file_name().to_string_lossy().into_owned())
                    .collect()
            })
            .unwrap_or_default();
        Self { root, graphics_listing }
    }

    /// map/ へ移設済みの資源を優先し、未移設の資源は従来位置へ退避する。
    /// 返り値はROOT基準の相対パス。
    pub fn resource_graphics_dir(&self, name: &str) -> String {
        let relocated = format!("map_resource_{name}_");
        if self.graphics_listing.iter().any(|file| file.starts_with(&relocated)) {
            return format!("assets/graphics/map_resource_{name}");
        }
        format!("assets/graphics/resource_{name}")
    }

    /// 分類済みの battle/characters と field/characters を順に探し、
    /// 未移設は family 直下へ退避する。plan は "chr_NNN/bank.json" 形式。
    /// 返り値は絶対パス。
    pub fn character_bank_path(&self, family_root: impl AsRef<Path>, plan: &str) -> PathBuf {
        let flat = plan.replace('/', "_");
        for semantic in ["battle", "field"] {
            let relocated =
                self.root.join("assets/graphics").join(format!("{semantic}_characters_{flat}"));
            if relocated.exists() {
                return relocated;
            }
        }
        let family_root = family_root.as_ref();
        let local = family_root.join(&flat);
        if local.exists() {
            return local;
        }
        family_root.join(plan)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn scratch(name: &str) -> PathBuf {
        let dir = std::env::temp_dir().join(format!("alchemy-asset-paths-{name}-{}", std::process::id()));
        let _ = fs::remove_dir_all(&dir);
        fs::create_dir_all(dir.join("assets/graphics")).unwrap();
        dir
    }

    #[test]
    fn relocated_resources_win_over_the_old_layout() {
        let root = scratch("relocated");
        fs::write(root.join("assets/graphics/map_resource_37b_0.png"), b"").unwrap();
        let paths = AssetPaths::new(&root);
        assert_eq!(paths.resource_graphics_dir("37b"), "assets/graphics/map_resource_37b");
        assert_eq!(paths.resource_graphics_dir("37c"), "assets/graphics/resource_37c");
        fs::remove_dir_all(&root).unwrap();
    }

    #[test]
    fn a_missing_graphics_directory_resolves_to_the_old_layout() {
        let paths = AssetPaths::new("/nonexistent-alchemy-root");
        assert_eq!(paths.resource_graphics_dir("001"), "assets/graphics/resource_001");
    }

    #[test]
    fn character_banks_prefer_battle_then_field_then_local() {
        let root = scratch("banks");
        let family = root.join("family");
        fs::create_dir_all(&family).unwrap();
        let paths = AssetPaths::new(&root);

        // Nothing relocated: fall through to the plan under the family root.
        assert_eq!(paths.character_bank_path(&family, "chr_001/bank.json"), family.join("chr_001/bank.json"));

        // A flattened copy under the family root is preferred over the plan.
        fs::write(family.join("chr_001_bank.json"), b"").unwrap();
        assert_eq!(paths.character_bank_path(&family, "chr_001/bank.json"), family.join("chr_001_bank.json"));

        // field beats local, battle beats field.
        let graphics = root.join("assets/graphics");
        fs::write(graphics.join("field_characters_chr_001_bank.json"), b"").unwrap();
        assert_eq!(
            paths.character_bank_path(&family, "chr_001/bank.json"),
            graphics.join("field_characters_chr_001_bank.json")
        );
        fs::write(graphics.join("battle_characters_chr_001_bank.json"), b"").unwrap();
        assert_eq!(
            paths.character_bank_path(&family, "chr_001/bank.json"),
            graphics.join("battle_characters_chr_001_bank.json")
        );
        fs::remove_dir_all(&root).unwrap();
    }
}
