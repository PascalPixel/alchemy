pub mod reconstruct;
use overlay_call_targets::{annotate, resolved_call_names, unannotated_call_sites};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};
use std::path::PathBuf;
use std::process::Command;
const PC_LOAD_MASK: u16 = 0xf800;
const PC_LOAD: u16 = 0x4800;
const RETURN_MASK: u16 = 0xff87;
const RETURN: u16 = 0x4700;
const USAGE: &str = "usage: overlay-show <overlay> <offsetHex> [endHex | -n BYTES] [--annotate]";
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Options {
    pub overlay: String,
    pub offset: i64,
    pub length: i64,
    pub annotate: bool,
}
fn root() -> PathBuf {
    std::path::Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .expect("crate dir has a parent")
        .parent()
        .expect("tools has a parent")
        .to_path_buf()
}
fn image_offset(value: i64, label: &str) -> Result<i64, String> {
    let offset = if value >= OVERLAY_BASE { value - OVERLAY_BASE } else { value };
    if offset < 0 {
        return Err(format!("{label} must be hexadecimal"));
    }
    Ok(offset)
}
fn parse_hex(text: &str) -> Option<i64> {
    let stripped = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")).unwrap_or(text);
    i64::from_str_radix(stripped, 16).ok()
}
fn is_hex_text(text: &str) -> bool {
    let stripped = text.strip_prefix("0x").or_else(|| text.strip_prefix("0X")).unwrap_or(text);
    !stripped.is_empty() && stripped.bytes().all(|b| b.is_ascii_hexdigit())
}
pub fn options_of(argv: &[String]) -> Result<Options, String> {
    let mut rest: Vec<String> = Vec::new();
    let mut options = Options { overlay: String::new(), offset: -1, length: 0, annotate: false };
    let mut explicit_length = false;
    let mut index = 0usize;
    while index < argv.len() {
        let argument = &argv[index];
        if argument == "--length" || argument == "-n" {
            index += 1;
            let value = argv.get(index).ok_or_else(|| USAGE.to_string())?;
            options.length = value.parse::<f64>().map_err(|_| USAGE.to_string())? as i64;
            explicit_length = true;
        } else if argument == "--annotate" {
            options.annotate = true;
        } else if argument == "-h" || argument == "--help" {
            return Err(format!("__usage__{USAGE}"));
        } else {
            rest.push(argument.clone());
        }
        index += 1;
    }
    if rest.is_empty() {
        return Err(USAGE.to_string());
    }
    let overlay = rest[0].clone();
    let offset_text = rest.get(1).ok_or_else(|| USAGE.to_string())?.clone();
    let end_text = rest.get(2).cloned();
    if rest.len() > 3 {
        return Err(format!("{USAGE}\nunexpected argument: {}", rest[3]));
    }
    options.overlay = overlay;
    let offset_value = parse_hex(&offset_text).ok_or_else(|| "offset must be hexadecimal".to_string())?;
    options.offset = image_offset(offset_value, "offset")?;
    if let Some(end_text) = end_text {
        if explicit_length {
            return Err("pass an end bound OR -n BYTES, not both".to_string());
        }
        if !is_hex_text(&end_text) {
            return Err(format!("end bound must be hexadecimal: {end_text}"));
        }
        let end_value = parse_hex(&end_text).ok_or_else(|| format!("end bound must be hexadecimal: {end_text}"))?;
        let end = image_offset(end_value, "end bound")?;
        if end <= options.offset {
            return Err("end bound must be greater than the start offset".to_string());
        }
        options.length = end - options.offset;
    }
    Ok(options)
}
fn read_u16le(data: &[u8], at: i64) -> u16 {
    let at = at as usize;
    (data[at] as u16) | ((data[at + 1] as u16) << 8)
}
fn read_u32le(data: &[u8], at: i64) -> u32 {
    let at = at as usize;
    (data[at] as u32) | ((data[at + 1] as u32) << 8) | ((data[at + 2] as u32) << 16) | ((data[at + 3] as u32) << 24)
}
pub fn pool_target(insn_offset: i64, half: u16) -> i64 {
    ((insn_offset + 4) & !3) + ((half & 0xff) as i64) * 4
}
fn extent_of(data: &[u8], offset: i64) -> i64 {
    let mut end = offset + 2;
    let mut furthest = offset;
    let mut at = offset;
    let limit = std::cmp::min(offset + 512, data.len() as i64);
    while at + 2 <= limit {
        let half = read_u16le(data, at);
        if (half & PC_LOAD_MASK) == PC_LOAD {
            furthest = std::cmp::max(furthest, pool_target(at, half) + 4);
        }
        end = at + 2;
        if (half & RETURN_MASK) == RETURN {
            break;
        }
        at += 2;
    }
    std::cmp::max(end, furthest) - offset
}
pub enum Outcome {
    Usage,
    SelfTest(String),
    Lines(Vec<String>),
}
pub fn run(argv: &[String]) -> Result<Outcome, String> {
    let options = match options_of(argv) {
        Ok(options) => options,
        Err(message) => {
            if let Some(usage) = message.strip_prefix("__usage__") {
                println!("{usage}");
                return Ok(Outcome::Usage);
            }
            return Err(message);
        }
    };
    let source_path = root().join("games/gs1/assets/code").join(format!("{}_overlay.s", options.overlay));
    let data = assemble_overlay(&OverlaySource::path(&source_path), OVERLAY_BASE)?;
    let length = if options.length > 0 { options.length } else { extent_of(&data, options.offset) };
    let work = root().join("work/overlay-show");
    std::fs::create_dir_all(&work).map_err(|e| e.to_string())?;
    let binary = work.join(format!("{}_{:x}.bin", options.overlay, options.offset));
    let start = options.offset as usize;
    let stop = (options.offset + length) as usize;
    let slice = data.get(start..stop.min(data.len())).unwrap_or(&[]).to_vec();
    std::fs::write(&binary, &slice).map_err(|e| e.to_string())?;
    let output = Command::new("arm-none-eabi-objdump")
        .args([
            "-D",
            "-b",
            "binary",
            "-m",
            "arm",
            "-M",
            "force-thumb",
            &format!("--adjust-vma=0x{:x}", OVERLAY_BASE + options.offset),
        ])
        .arg(&binary)
        .output()
        .map_err(|e| e.to_string())?;
    if !output.status.success() {
        return Err(format!("objdump failed: {}", String::from_utf8_lossy(&output.stderr).trim()));
    }
    let stdout = String::from_utf8_lossy(&output.stdout);
    let mut listing = stdout
        .split('\n')
        .filter(|line| is_listing_line(line))
        .map(|line| line.trim_end().to_string())
        .collect::<Vec<_>>()
        .join("\n");
    if options.annotate {
        let names = resolved_call_names(&options.overlay, options.offset, options.offset + length)?;
        let missed = unannotated_call_sites(&listing, &names);
        if !missed.is_empty() {
            return Err(format!(
                "overlay show annotation missed {} call site(s); explicit bounds do not cover the listing",
                missed.len()
            ));
        }
        listing = annotate(&listing, &names);
    }
    let mut lines: Vec<String> = Vec::new();
    for line in listing.split('\n') {
        if !line.is_empty() {
            lines.push(line.to_string());
        }
    }
    let mut pools: Vec<(i64, u32)> = Vec::new();
    let mut at = options.offset;
    while at + 2 <= options.offset + length {
        if (at + 2) as usize <= data.len() {
            let half = read_u16le(&data, at);
            if (half & PC_LOAD_MASK) == PC_LOAD {
                let target = pool_target(at, half);
                if target + 4 <= data.len() as i64 {
                    if let Some(existing) = pools.iter_mut().find(|(a, _)| *a == target) {
                        existing.1 = read_u32le(&data, target);
                    } else {
                        pools.push((target, read_u32le(&data, target)));
                    }
                }
            }
        }
        at += 2;
    }
    if !pools.is_empty() {
        lines.push("--- pool words referenced:".to_string());
        pools.sort_by_key(|(at, _)| *at);
        for (at, value) in pools {
            lines.push(format!("  0x{:x} = 0x{:08x}", OVERLAY_BASE + at, value));
        }
    }
    Ok(Outcome::Lines(lines))
}
fn is_listing_line(line: &str) -> bool {
    let trimmed = line.trim_start();
    if trimmed.len() == line.len() {
        return false; // requires at least one leading whitespace char, like `/^\s+/`
    }
    let leading_ws = line.len() - trimmed.len();
    if !line[..leading_ws].chars().all(|c| c.is_whitespace()) {
        return false;
    }
    match trimmed.find(':') {
        Some(colon) => {
            let digits = &trimmed[..colon];
            !digits.is_empty() && digits.bytes().all(|b| b.is_ascii_hexdigit())
        }
        None => false,
    }
}
