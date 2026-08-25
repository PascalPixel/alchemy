use serde_json::Value;
use std::path::{Path, PathBuf};
use std::process::Command;

pub const ROM_BASE: i64 = 0x0800_0000;

pub fn root() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .into()
}

#[derive(Clone, Debug)]
pub enum SourceTree {
    Work { id: String, root: PathBuf },
    Ref { id: String },
}

impl SourceTree {
    pub fn id(&self) -> &str {
        match self {
            Self::Work { id, .. } | Self::Ref { id } => id,
        }
    }
    pub fn read(&self, path: &str) -> Option<String> {
        match self {
            Self::Work { root, .. } => std::fs::read(root.join(path))
                .ok()
                .map(|b| String::from_utf8_lossy(&b).into_owned()),
            Self::Ref { id } => Command::new("git")
                .args(["show", &format!("{id}:{path}")])
                .current_dir(root())
                .output()
                .ok()
                .filter(|out| out.status.success())
                .map(|out| String::from_utf8_lossy(&out.stdout).into_owned()),
        }
    }
    pub fn list(&self, directory: &str) -> Vec<String> {
        match self {
            Self::Work { root, .. } => std::fs::read_dir(root.join(directory))
                .ok()
                .into_iter()
                .flatten()
                .filter_map(|e| e.ok().map(|e| e.file_name().to_string_lossy().into_owned()))
                .collect(),
            Self::Ref { id } => Command::new("git")
                .args(["ls-tree", "--name-only", &format!("{id}:{directory}")])
                .current_dir(root())
                .output()
                .ok()
                .filter(|out| out.status.success())
                .map(|out| {
                    String::from_utf8_lossy(&out.stdout)
                        .lines()
                        .filter_map(|line| Path::new(line).file_name())
                        .map(|name| name.to_string_lossy().into_owned())
                        .collect()
                })
                .unwrap_or_default(),
        }
    }
}

pub fn work_tree() -> SourceTree {
    SourceTree::Work {
        id: "worktree".into(),
        root: root(),
    }
}
pub fn work_tree_at(path: PathBuf) -> SourceTree {
    SourceTree::Work {
        id: "worktree".into(),
        root: path,
    }
}
pub fn ref_tree(id: &str) -> Option<SourceTree> {
    let ok = Command::new("git")
        .args([
            "rev-parse",
            "--verify",
            "--quiet",
            &format!("{id}^{{commit}}"),
        ])
        .current_dir(root())
        .status()
        .ok()?
        .success();
    ok.then(|| SourceTree::Ref { id: id.into() })
}
pub fn read_json(tree: &SourceTree, path: &str) -> Result<Value, String> {
    let text = tree
        .read(path)
        .ok_or_else(|| format!("{} is missing {path}", tree.id()))?;
    serde_json::from_str(&text).map_err(|e| format!("{path}: {e}"))
}
