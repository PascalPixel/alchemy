//! Rebuild Golden Sun's battle asset packages behind one narrow host boundary.
//!
//! The private modules retain the independently audited screen, display,
//! effect-runtime, stream, and effect-data layouts. `alchemy build assets` is
//! the sole caller, so it receives only reconstructed bytes and source paths.

mod display;
mod effect_data;
mod effect_runtime;
mod screen;
mod streams;

use serde_json::Value;
use std::path::{Path, PathBuf};

pub const SCREEN_ADDRESS: usize = screen::ADDRESS;
pub const SCREEN_SIZE: usize = screen::SIZE;
pub const EFFECT_RUNTIME_ADDRESS: usize = effect_runtime::SENTOU_KOUKA_ADDRESS as usize;
pub const EFFECT_RUNTIME_SIZE: usize = effect_runtime::SENTOU_KOUKA_SIZE;

#[derive(Debug, Clone)]
pub struct BuiltBattleResource {
    pub address: usize,
    pub data: Vec<u8>,
    pub sources: Vec<PathBuf>,
}

pub fn build_effect_data(value: &Value, root: &Path) -> Result<Vec<u8>, String> {
    effect_data::build(value, root)
}

pub fn build_screen(index_path: &Path) -> Result<(Vec<u8>, Vec<PathBuf>), String> {
    screen::build(index_path).map_err(|error| error.to_string())
}

pub fn build_display(index_path: &Path) -> Result<Vec<u8>, String> {
    display::build(index_path).map_err(|error| error.to_string())
}

pub fn build_effect_runtime(index_path: &Path) -> Result<Vec<u8>, String> {
    effect_runtime::build(index_path).map_err(|error| error.to_string())
}

pub fn build_resource_series(index_path: &Path) -> Result<Vec<BuiltBattleResource>, String> {
    streams::build_series(index_path)
}
