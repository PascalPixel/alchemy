//! Psynergy: portable Thumb decoding, asset codecs and the dependency map and the worst-code ranking.
//! Callers supply bytes and image addresses; repository ownership and game-specific
//! source rewrites belong to the integrating project.
pub mod assembly {
    pub mod addresses;
}
pub mod assets;
pub mod cache;
pub mod decode;
pub mod deps;
pub mod elf;
pub mod process;
pub mod similar;
pub mod thumb;
pub mod worst;
