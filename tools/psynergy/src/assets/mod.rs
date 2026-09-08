//! Byte-oriented asset codecs. Callers own file paths and game layouts.
pub mod compression;
pub mod huffman_archive;
pub mod image;
pub mod lz;
pub mod midi;
pub mod text;
pub mod wav;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AssetError(pub String);

impl std::fmt::Display for AssetError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str(&self.0)
    }
}
impl std::error::Error for AssetError {}

fn err<T>(message: impl Into<String>) -> Result<T, AssetError> {
    Err(AssetError(message.into()))
}
