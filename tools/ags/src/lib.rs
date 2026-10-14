pub mod asm;
pub mod assets;
pub mod graphics;
pub mod lz;
pub mod patch;
pub mod resource;
pub mod sound;
pub mod sprite;
pub mod text;

/// Writes a built file whole: to a temporary name beside it, then renamed
/// over it. Editions built at once share the assets beside their sources,
/// and one may read a file another is writing; each sees it whole.
pub fn write_whole(path: &str, bytes: &[u8]) -> Result<(), String> {
    let temporary = format!("{path}.{}.tmp", std::process::id());
    std::fs::write(&temporary, bytes).map_err(|error| format!("{path}: {error}"))?;
    std::fs::rename(&temporary, path).map_err(|error| format!("{path}: {error}"))
}
