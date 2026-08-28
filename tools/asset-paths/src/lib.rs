use std::fs;
use std::path::{Path, PathBuf};

pub struct AssetPaths {
    root: PathBuf,
    graphics: Vec<String>,
}

impl AssetPaths {
    pub fn new(root: impl AsRef<Path>) -> Self {
        let root = root.as_ref().to_path_buf();
        let graphics = fs::read_dir(root.join("games/gs1/assets/graphics"))
            .map(|entries| {
                entries
                    .filter_map(Result::ok)
                    .map(|entry| entry.file_name().to_string_lossy().into_owned())
                    .collect()
            })
            .unwrap_or_default();
        Self { root, graphics }
    }

    pub fn resource_graphics_dir(&self, name: &str) -> String {
        let relocated = format!("map_resource_{name}_");
        if self
            .graphics
            .iter()
            .any(|file| file.starts_with(&relocated))
        {
            return format!("games/gs1/assets/graphics/map_resource_{name}");
        }
        format!("games/gs1/assets/graphics/resource_{name}")
    }

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
