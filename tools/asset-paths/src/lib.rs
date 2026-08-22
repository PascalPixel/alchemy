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
    /// `root` is the repository root. A missing `games/gs1/assets/graphics` yields an
    /// empty listing, which resolves every resource to its unrelocated path.
    pub fn new(root: impl AsRef<Path>) -> Self {
        let root = root.as_ref().to_path_buf();
        let graphics_listing = fs::read_dir(root.join("games/gs1/assets/graphics"))
            .map(|entries| {
                entries
                    .filter_map(Result::ok)
                    .map(|entry| entry.file_name().to_string_lossy().into_owned())
                    .collect()
            })
            .unwrap_or_default();
        Self {
            root,
            graphics_listing,
        }
    }

    /// map/ へ移設済みの資源を優先し、未移設の資源は従来位置へ退避する。
    /// 返り値はROOT基準の相対パス。
    pub fn resource_graphics_dir(&self, name: &str) -> String {
        let relocated = format!("map_resource_{name}_");
        if self
            .graphics_listing
            .iter()
            .any(|file| file.starts_with(&relocated))
        {
            return format!("games/gs1/assets/graphics/map_resource_{name}");
        }
        format!("games/gs1/assets/graphics/resource_{name}")
    }

    /// 分類済みの battle/characters と field/characters を順に探し、
    /// 未移設は family 直下へ退避する。plan は "chr_NNN/bank.json" 形式。
    /// 返り値は絶対パス。
    pub fn character_bank_path(&self, family_root: impl AsRef<Path>, plan: &str) -> PathBuf {
        let flat = plan.replace('/', "_");
        for semantic in ["battle", "field"] {
            let relocated = self
                .root
                .join("games/gs1/assets/graphics")
                .join(format!("{semantic}_characters_{flat}"));
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
