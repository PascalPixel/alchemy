//! Rebuild the late-runtime residual regions consumed by `build-assets`.

pub mod cli;

use late_runtime_data::{
    read_late_runtime_catalog, ComponentKind, LateRuntimeCatalog, LateRuntimeComponent, TableKind,
};
use serde_json::Value;
use std::path::Path;

pub type Result<T> = std::result::Result<T, String>;
pub const SOURCE_BYTES: i64 = 7294;

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct Regions {
    entries: Vec<(u32, Vec<u8>)>,
}
impl Regions {
    pub fn iter(&self) -> std::slice::Iter<'_, (u32, Vec<u8>)> {
        self.entries.iter()
    }
    fn push(&mut self, address: u32, data: Vec<u8>) {
        self.entries.push((address, data));
    }
}

#[derive(Debug, Clone)]
pub struct LateRuntimeResidualBuild {
    pub regions: Regions,
    pub source_bytes: i64,
}

fn field<'a>(value: &'a Value, key: &str) -> Result<&'a Value> {
    value.get(key).ok_or_else(|| format!("missing {key}"))
}
fn text(value: &Value) -> Option<&str> {
    value.as_str()
}
fn address(value: &Value) -> Result<u32> {
    let text = text(value).ok_or("invalid address")?;
    u32::from_str_radix(text.strip_prefix("0x").ok_or("invalid address")?, 16)
        .map_err(|_| "invalid address".into())
}
fn type_name(component: &LateRuntimeComponent) -> &'static str {
    match component.kind {
        ComponentKind::Table(TableKind::S16) => "s16",
        ComponentKind::Table(TableKind::U16) => "u16",
        _ => "u8",
    }
}

fn component_bytes(value: &Value, component: &LateRuntimeComponent) -> Result<Vec<u8>> {
    let address_text = format!("0x{:08x}", component.address);
    let end_text = format!("0x{:08x}", component.end);
    for (key, expected) in [
        ("name", component.name.as_str()),
        ("address", address_text.as_str()),
        ("end", end_text.as_str()),
        ("role", component.role.as_str()),
        ("type", type_name(component)),
    ] {
        if text(field(value, key)?) != Some(expected) {
            return Err(format!("{} metadata differs", component.name));
        }
    }
    match text(field(value, "representation")?) {
        Some("uniform_fill") => {
            let fill = field(value, "value")?
                .as_i64()
                .filter(|n| *n == 0 || *n == 255)
                .ok_or_else(|| format!("{} fill differs", component.name))?;
            Ok(vec![fill as u8; component.size()])
        }
        Some("byte_values") => {
            let values = field(value, "values")?
                .as_array()
                .ok_or_else(|| format!("{} values differ", component.name))?;
            if values.len() != component.size() {
                return Err(format!("{} values differ", component.name));
            }
            values
                .iter()
                .map(|value| {
                    value
                        .as_i64()
                        .filter(|n| (0..=255).contains(n))
                        .map(|n| n as u8)
                        .ok_or_else(|| format!("{} value differs", component.name))
                })
                .collect()
        }
        _ => Err(format!("{} representation differs", component.name)),
    }
}

pub fn build_late_runtime_residual(
    index_path: &Path,
    catalog_path: &Path,
) -> Result<LateRuntimeResidualBuild> {
    let catalog: LateRuntimeCatalog = read_late_runtime_catalog(catalog_path)?;
    let source: Value =
        serde_json::from_slice(&std::fs::read(index_path).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
    if field(&source, "format")?.as_i64() != Some(1)
        || text(field(&source, "kind")?) != Some("golden-sun-late-runtime-residual")
        || field(&source, "source_bytes")?.as_i64() != Some(SOURCE_BYTES)
    {
        return Err("late residual source identity differs".into());
    }
    let regions = field(&source, "regions")?
        .as_array()
        .ok_or("late residual regions differ")?;
    if regions.len() != catalog.ranges.len() {
        return Err("late residual region count differs".into());
    }
    let mut built = Regions::default();
    for (raw, range) in regions.iter().zip(&catalog.ranges) {
        if text(field(raw, "name")?) != Some(range.name.as_str())
            || address(field(raw, "address")?)? != range.address
            || address(field(raw, "end")?)? != range.end
        {
            return Err(format!("{} layout differs", range.name));
        }
        let components = field(raw, "components")?
            .as_array()
            .ok_or("late residual components differ")?;
        if components.len() != range.components.len() {
            return Err(format!("{} component count differs", range.name));
        }
        let data = range.components.iter().zip(components).try_fold(
            Vec::new(),
            |mut data, (component, raw)| {
                data.extend(component_bytes(raw, component)?);
                Ok::<_, String>(data)
            },
        )?;
        if data.len() != (range.end - range.address) as usize {
            return Err(format!("{} length differs", range.name));
        }
        built.push(range.address, data);
    }
    let source_bytes = built.iter().map(|(_, data)| data.len() as i64).sum();
    Ok(LateRuntimeResidualBuild {
        regions: built,
        source_bytes,
    })
}
