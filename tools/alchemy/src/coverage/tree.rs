use std::path::PathBuf;

pub fn root() -> PathBuf {
    crate::compiler::routing::root().to_path_buf()
}
