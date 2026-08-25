use extract_resource::{encode_general, GeneralToken};
use overlay_disasm::{assemble_overlay, OverlaySource};
use serde_json::{Map, Value};
use std::fs;
use std::io::{self, Write};
use std::path::{Path, PathBuf};

pub type Result<T> = std::result::Result<T, String>;
pub const ROM_END: usize = 0x0880_0000;
pub const RESOURCE_ADDRESS: usize = 0x087f_cd20;
pub const DECODED_ADDRESS: i64 = 0x0200_0000;
pub const STREAM_SIZE: usize = 0x0799;
pub const DECODED_SIZE: usize = 0x168c;
pub const PADDING_ADDRESS: usize = RESOURCE_ADDRESS + STREAM_SIZE;
pub const PADDING_SIZE: usize = 3;
pub const FILL_ADDRESS: usize = PADDING_ADDRESS + PADDING_SIZE;

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Resource3ceBuild {
    pub stream_address: usize,
    pub stream: Vec<u8>,
    pub fill_address: usize,
    pub fill: Vec<u8>,
    pub fallback_address: usize,
    pub fallback_size: usize,
}

#[derive(Debug, Clone)]
struct Layout {
    overlay: String,
    plan: String,
}

fn err<T>(message: impl Into<String>) -> Result<T> {
    Err(message.into())
}
fn hex(v: usize, width: usize) -> String {
    format!("0x{v:0width$x}")
}
fn address(v: usize) -> String {
    hex(v, 8)
}
fn obj<'a>(v: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    v.as_object().ok_or_else(|| format!("{label} must be an object"))
}
fn keys<'a>(v: &'a Value, expected: &[&str], label: &str) -> Result<&'a Map<String, Value>> {
    let o = obj(v, label)?;
    let mut got: Vec<&str> = o.keys().map(String::as_str).collect();
    let mut want = expected.to_vec();
    got.sort_unstable();
    want.sort_unstable();
    if got != want {
        return err(format!("{label} has unknown or missing fields"));
    }
    Ok(o)
}
fn field<'a>(o: &'a Map<String, Value>, name: &str) -> Result<&'a Value> {
    o.get(name).ok_or_else(|| format!("missing field {name}"))
}
fn same_string(v: &Value, expected: &str) -> bool {
    v.as_str() == Some(expected)
}
fn same_number(v: &Value, expected: f64) -> bool {
    v.as_f64() == Some(expected)
}
fn number(v: &Value, label: &str) -> Result<usize> {
    let Some(n) = v.as_f64() else {
        return err(format!("{label} must be a numeric integer"));
    };
    if !n.is_finite() || n.fract() != 0.0 || !(0.0..=9_007_199_254_740_991.0).contains(&n) {
        return err(format!("{label} must be a numeric integer"));
    }
    Ok(n as usize)
}
fn json_file(path: &Path) -> Result<Value> {
    let bytes = fs::read(path).map_err(|e| format!("{}: {e}", path.display()))?;
    serde_json::from_str(&String::from_utf8_lossy(&bytes)).map_err(|e| format!("{}: {e}", path.display()))
}

fn parse_layout(v: &Value) -> Result<Layout> {
    let o = keys(v, &["format", "kind", "resource_id", "address", "decoded_address", "stream_size", "decoded_size", "overlay", "compression_plan", "selection", "padding", "zero_fill"], "resource 3ce layout")?;
    if !same_number(field(o, "format")?, 1.0)
        || !same_string(field(o, "kind")?, "golden-sun-final-battle-overlay")
        || !same_string(field(o, "resource_id")?, "0x3ce")
        || !same_string(field(o, "address")?, &address(RESOURCE_ADDRESS))
        || !same_string(field(o, "decoded_address")?, &address(DECODED_ADDRESS as usize))
        || !same_string(field(o, "stream_size")?, &hex(STREAM_SIZE, 3))
        || !same_string(field(o, "decoded_size")?, &hex(DECODED_SIZE, 4))
        || !same_string(field(o, "overlay")?, "overlay.s")
        || !same_string(field(o, "compression_plan")?, "stream.lz.json")
    {
        return err("resource 3ce layout identity differs");
    }
    let s = keys(field(o, "selection")?, &["table", "record_index", "record_address", "resource_id", "group", "variant", "effect_id", "consumers"], "resource 3ce selection")?;
    let consumers = field(s, "consumers")?.as_array().ok_or_else(|| "resource 3ce selection differs".to_string())?;
    let wanted = ["Func_0808a8e4", "Func_0808ab48", "Func_0808ab74"];
    if !same_string(field(s, "table")?, "0x0809f1a8")
        || !same_number(field(s, "record_index")?, 200.0)
        || !same_string(field(s, "record_address")?, "0x0809f7e8")
        || !same_string(field(s, "resource_id")?, "0x3ce")
        || !same_number(field(s, "group")?, 70.0)
        || !same_number(field(s, "variant")?, 1.0)
        || !same_number(field(s, "effect_id")?, 0.0)
        || consumers.len() != wanted.len()
        || consumers.iter().zip(wanted).any(|(a, b)| !same_string(a, b))
    {
        return err("resource 3ce selection differs");
    }
    let p = keys(field(o, "padding")?, &["address", "size", "alignment", "policy"], "resource 3ce padding")?;
    if !same_string(field(p, "address")?, &address(PADDING_ADDRESS)) || !same_number(field(p, "size")?, 3.0) || !same_number(field(p, "alignment")?, 4.0) || !same_string(field(p, "policy")?, "fallback") {
        return err("resource 3ce padding must remain an unclaimed aligned gap");
    }
    let f = keys(field(o, "zero_fill")?, &["address", "end", "value"], "resource 3ce zero fill")?;
    if !same_string(field(f, "address")?, &address(FILL_ADDRESS)) || !same_string(field(f, "end")?, &address(ROM_END)) || !same_number(field(f, "value")?, 0.0) {
        return err("resource 3ce zero fill differs");
    }
    Ok(Layout { overlay: field(o, "overlay")?.as_str().unwrap().into(), plan: field(o, "compression_plan")?.as_str().unwrap().into() })
}

fn parse_plan(v: &Value) -> Result<Vec<GeneralToken>> {
    let o = keys(v, &["format", "codec", "decoded_size", "tokens"], "resource 3ce stream plan")?;
    if !same_number(field(o, "format")?, 1.0) || !same_string(field(o, "codec")?, "golden-sun-general-lz") || !same_string(field(o, "decoded_size")?, &hex(DECODED_SIZE, 4)) {
        return err("resource 3ce stream plan differs");
    }
    let raw = field(o, "tokens")?.as_array().ok_or_else(|| "resource 3ce stream plan differs".to_string())?;
    let mut decoded = 0usize;
    let mut result = Vec::with_capacity(raw.len());
    for (i, value) in raw.iter().enumerate() {
        let Some(token) = value.as_array() else {
            return err(format!("resource 3ce token {i} opcode differs"));
        };
        let Some(opcode) = token.first().and_then(Value::as_str) else {
            return err(format!("resource 3ce token {i} opcode differs"));
        };
        match opcode {
            "l" => {
                if token.len() != 2 {
                    return err(format!("resource 3ce token {i} literal tuple differs"));
                }
                let count = number(&token[1], &format!("resource 3ce token {i} literal count"))?;
                if count < 1 || decoded + count > DECODED_SIZE {
                    return err(format!("resource 3ce token {i} literal count is outside its range"));
                }
                decoded += count;
                match result.last_mut() {
                    Some(GeneralToken::Literal(previous)) => *previous += count as u32,
                    _ => result.push(GeneralToken::Literal(count as u32)),
                }
            }
            "c" => {
                if token.len() != 3 {
                    return err(format!("resource 3ce token {i} copy tuple differs"));
                }
                let length = number(&token[1], &format!("resource 3ce token {i} copy length"))?;
                let distance = number(&token[2], &format!("resource 3ce token {i} copy distance"))?;
                if !(2..=137).contains(&length) || decoded + length > DECODED_SIZE {
                    return err(format!("resource 3ce token {i} copy length is outside its range"));
                }
                if distance < 1 || distance > decoded {
                    return err(format!("resource 3ce token {i} copy distance is outside its replay prefix"));
                }
                decoded += length;
                result.push(GeneralToken::Copy { length: length as u32, distance: distance as u32 });
            }
            _ => return err(format!("resource 3ce token {i} opcode differs")),
        }
    }
    if decoded != DECODED_SIZE {
        return err("resource 3ce token plan has the wrong decoded extent");
    }
    Ok(result)
}

fn layout_paths(path: &Path, layout: &Layout) -> Result<(PathBuf, PathBuf)> {
    let dir = path.parent().unwrap_or_else(|| Path::new("."));
    let name = path.file_name().and_then(|n| n.to_str()).ok_or_else(|| "resource 3ce layout path must name a file".to_string())?;
    let prefix = name.strip_suffix("layout.json").unwrap_or(name);
    let local = |leaf: String| -> Result<PathBuf> {
        if Path::new(&leaf).file_name().and_then(|n| n.to_str()) != Some(leaf.as_str()) {
            return err("resource 3ce source names must be local leaves");
        }
        Ok(dir.join(leaf))
    };
    Ok((local(format!("{prefix}{}", layout.overlay))?, local(format!("{prefix}{}", layout.plan))?))
}

pub fn build_resource_3ce(path: &Path) -> Result<Resource3ceBuild> {
    let layout = parse_layout(&json_file(path)?)?;
    let (overlay, plan) = layout_paths(path, &layout)?;
    let decoded = assemble_overlay(&OverlaySource::path(overlay), DECODED_ADDRESS)?;
    if decoded.len() != DECODED_SIZE {
        return err("resource 3ce overlay size differs");
    }
    let tokens = parse_plan(&json_file(&plan)?)?;
    let stream = encode_general(&decoded, &tokens).map_err(|e| e.0)?;
    if stream.len() != STREAM_SIZE {
        return err("resource 3ce stream size differs");
    }
    Ok(Resource3ceBuild { stream_address: RESOURCE_ADDRESS, stream, fill_address: FILL_ADDRESS, fill: vec![0; ROM_END - FILL_ADDRESS], fallback_address: PADDING_ADDRESS, fallback_size: PADDING_SIZE })
}

pub fn run(args: Vec<String>) -> Result<()> {
    let Some(command) = args.first().map(String::as_str) else {
        return err("resource 3ce build requires a command");
    };
    match (command, args.get(1), args.get(2)) {
        ("build-stdout", Some(path), Some(component)) => {
            let b = build_resource_3ce(Path::new(path))?;
            let data = match component.as_str() {
                "stream" => &b.stream,
                "fill" => &b.fill,
                _ => return err("resource 3ce component must be stream or fill"),
            };
            io::stdout().write_all(data).map_err(|e| e.to_string())?;
        }
        _ => return err(format!("unknown build command: {command}")),
    }
    Ok(())
}
