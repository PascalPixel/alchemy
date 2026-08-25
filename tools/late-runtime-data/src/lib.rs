//! Typed layout needed by the late-runtime asset encoder.

use serde::Deserialize;
use std::path::Path;

pub const ROM_BASE: u32 = 0x0800_0000;
pub type Result<T> = std::result::Result<T, String>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TableKind {
    U8,
    S16,
    U16,
}
impl TableKind {
    pub fn width(self) -> u32 {
        if matches!(self, Self::U8) {
            1
        } else {
            2
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ComponentKind {
    GeneralLzStream,
    Table(TableKind),
    UnresolvedFill,
}
impl ComponentKind {
    fn width(self) -> u32 {
        match self {
            Self::Table(kind) => kind.width(),
            _ => 0,
        }
    }

    fn parse(text: &str) -> Option<Self> {
        Some(match text {
            "general_lz_stream" => Self::GeneralLzStream,
            "u8_table" => Self::Table(TableKind::U8),
            "s16_table" => Self::Table(TableKind::S16),
            "u16_table" => Self::Table(TableKind::U16),
            "unresolved_fill" => Self::UnresolvedFill,
            _ => return None,
        })
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeComponent {
    pub name: String,
    pub kind: ComponentKind,
    pub address: u32,
    pub end: u32,
    pub role: String,
    pub count: Option<i64>,
    pub decoded_bytes: Option<i64>,
}
impl LateRuntimeComponent {
    pub fn size(&self) -> usize {
        (self.end - self.address) as usize
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeRange {
    pub name: String,
    pub address: u32,
    pub end: u32,
    pub components: Vec<LateRuntimeComponent>,
}
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct LateRuntimeCatalog {
    pub ranges: Vec<LateRuntimeRange>,
}

#[derive(Deserialize)]
struct Document {
    format: i64,
    kind: String,
    ranges: Vec<RawRange>,
}
#[derive(Deserialize)]
struct RawRange {
    name: String,
    address: String,
    end: String,
    components: Vec<RawComponent>,
}
#[derive(Deserialize)]
struct RawComponent {
    name: String,
    kind: String,
    address: String,
    end: String,
    role: String,
    count: Option<i64>,
    decoded_bytes: Option<i64>,
}

const EXPECTED_RANGES: [(&str, u32, u32); 3] = [
    ("range_080f38bc", 0x080f_38bc, 0x080f_4000),
    ("range_080f53ce", 0x080f_53ce, 0x080f_6000),
    ("range_080f86f8", 0x080f_86f8, 0x080f_9000),
];
const EXPECTED_COMPONENTS: [(&str, &str); 18] = [
    ("haikei_stream", "general_lz_stream"),
    ("haichi_hyou", "u8_table"),
    ("henka_hyou", "u8_table"),
    ("soroe_080f39ed", "unresolved_fill"),
    ("iro_henka_00", "u16_table"),
    ("iro_henka_01", "u16_table"),
    ("iro_henka_02", "u16_table"),
    ("aki_080f3aae", "unresolved_fill"),
    ("aki_080f53ce", "unresolved_fill"),
    ("hantei_dankai", "s16_table"),
    ("aki_080f5426", "unresolved_fill"),
    ("zahyoubisei_hyou", "u16_table"),
    ("hyouji_shoki_hyou", "u8_table"),
    ("haba_hyou_00", "u8_table"),
    ("haichi_hyou_00", "u8_table"),
    ("haichi_hyou_01", "u8_table"),
    ("haba_hyou_01", "u8_table"),
    ("aki_080f873e", "unresolved_fill"),
];

fn address(text: &str) -> Result<u32> {
    u32::from_str_radix(text.strip_prefix("0x").ok_or("invalid address")?, 16).map_err(|_| "invalid address".into())
}

pub fn read_late_runtime_catalog(path: &Path) -> Result<LateRuntimeCatalog> {
    let raw: Document =
        serde_json::from_slice(&std::fs::read(path).map_err(|e| e.to_string())?).map_err(|e| e.to_string())?;
    if raw.format != 1 || raw.kind != "golden-sun-late-runtime-data-layout" || raw.ranges.len() != 3 {
        return Err("unsupported late runtime catalog".into());
    }
    let mut all = Vec::new();
    let mut ranges = Vec::new();
    for (index, range) in raw.ranges.into_iter().enumerate() {
        let start = address(&range.address)?;
        let end = address(&range.end)?;
        if (range.name.as_str(), start, end) != EXPECTED_RANGES[index] {
            return Err("late runtime range differs".into());
        }
        let mut components = Vec::new();
        for item in range.components {
            let kind = ComponentKind::parse(&item.kind).ok_or("late runtime component differs")?;
            let component_start = address(&item.address)?;
            let end = address(&item.end)?;
            if end <= component_start
                || matches!(kind, ComponentKind::Table(_))
                    && item.count.unwrap_or(0) * i64::from(kind.width()) != i64::from(end - component_start)
            {
                return Err("late runtime component extent differs".into());
            }
            all.push((item.name.clone(), item.kind.clone()));
            components.push(LateRuntimeComponent {
                name: item.name,
                kind,
                address: component_start,
                end,
                role: item.role,
                count: item.count,
                decoded_bytes: item.decoded_bytes,
            });
        }
        ranges.push(LateRuntimeRange { name: range.name, address: start, end, components });
    }
    if all.len() != EXPECTED_COMPONENTS.len()
        || all
            .iter()
            .zip(EXPECTED_COMPONENTS)
            .any(|(actual, expected)| actual.0 != expected.0 || actual.1 != expected.1)
    {
        return Err("late runtime component layout differs".into());
    }
    Ok(LateRuntimeCatalog { ranges })
}
