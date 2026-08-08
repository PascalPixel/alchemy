//! `tools/overlay/overlay_show.ts`, in Rust.
//!
//! Prints one overlay function's reconstruction disassembly together with the
//! pool words its pc-relative loads resolve to. See the TypeScript original's
//! header comment for the full rationale; it is reproduced there, not
//! duplicated here.

use std::path::PathBuf;
use std::process::Command;

use overlay_call_targets::{annotate, resolved_call_names, unannotated_call_sites};
use overlay_disasm::{assemble_overlay, OverlaySource, OVERLAY_BASE};

const PC_LOAD_MASK: u16 = 0xf800;
const PC_LOAD: u16 = 0x4800;
const RETURN_MASK: u16 = 0xff87;
const RETURN: u16 = 0x4700;

const USAGE: &str = "usage: overlay_show.ts <overlay> <offsetHex> [endHex | -n BYTES] [--annotate]";

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

/// Accept either an image offset or its ordinary 0x02000000 RAM spelling.
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

/// Parse the command line.
///
/// An END bound is accepted as a second positional so this tool takes the
/// same `<overlay> <startHex> <endHex>` spelling as `overlay_call_targets`.
/// Anything unparseable throws rather than being dropped: a silently ignored
/// bound was a real, measured bug (see the TypeScript original's comment).
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
    (data[at] as u32)
        | ((data[at + 1] as u32) << 8)
        | ((data[at + 2] as u32) << 16)
        | ((data[at + 3] as u32) << 24)
}

/// The reference of a pc-relative load. In Thumb: `(PC+4)` rounded down to a
/// four-byte boundary, plus the displacement times four.
pub fn pool_target(insn_offset: i64, half: u16) -> i64 {
    ((insn_offset + 4) & !3) + ((half & 0xff) as i64) * 4
}

/// Walk forward to a return to estimate a function's extent. Pool words
/// follow it, so the default display length reaches the furthest referenced
/// pool word.
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

pub fn self_test() -> Result<String, String> {
    if pool_target(0, 0x4800) != 4 {
        return Err("overlay show self-test: pool target at 0".into());
    }
    if pool_target(2, 0x4800) != 4 {
        return Err("overlay show self-test: pool target rounding".into());
    }
    if pool_target(0, 0x4801) != 8 {
        return Err("overlay show self-test: pool target displacement".into());
    }

    let s = |parts: &[&str]| parts.iter().map(|p| p.to_string()).collect::<Vec<_>>();

    let bounded = options_of(&s(&["resource_3b9", "1cd4", "23e0"]))?;
    if bounded.offset != 0x1cd4 {
        return Err("overlay show self-test: start bound".into());
    }
    if bounded.length != 0x23e0 - 0x1cd4 {
        return Err("overlay show self-test: end bound ignored".into());
    }
    let prefixed = options_of(&s(&["resource_3b9", "1cd4", "0x23e0"]))?;
    if prefixed.length != 0x23e0 - 0x1cd4 {
        return Err("overlay show self-test: 0x end bound".into());
    }
    let absolute = options_of(&s(&["resource_3b9", "0x02001cd4", "0x020023e0", "--annotate"]))?;
    if absolute.offset != 0x1cd4 || absolute.length != 0x23e0 - 0x1cd4 || !absolute.annotate {
        return Err("overlay show self-test: absolute address normalization or annotation flag".into());
    }
    if options_of(&s(&["resource_3b9", "1cd4", "-n", "1804"]))?.length != 1804 {
        return Err("overlay show self-test: -n must still work".into());
    }
    if options_of(&s(&["resource_3b9", "1cd4"]))?.length != 0 {
        return Err("overlay show self-test: no bound must defer to extentOf".into());
    }

    let rejects = |argv: &[&str], why: &str| -> Result<(), String> {
        if options_of(&s(argv)).is_ok() {
            return Err(format!("overlay show self-test: {why}"));
        }
        Ok(())
    };
    rejects(&["resource_3b9", "1cd4", "23e0", "-n", "1804"], "end bound and -n together must be rejected")?;
    rejects(&["resource_3b9", "1cd4", "1cd4"], "an end bound at the start must be rejected")?;
    rejects(&["resource_3b9", "1cd4", "11ac"], "an end bound below the start must be rejected")?;
    rejects(&["resource_3b9", "1cd4", "zzz"], "a non-hex end bound must be rejected")?;
    rejects(&["resource_3b9", "1cd4", "23e0", "2400"], "a fourth positional must be rejected")?;

    Ok("overlay show self-test passed".to_string())
}

pub enum Outcome {
    Usage,
    SelfTest(String),
    Lines(Vec<String>),
}

/// Run the tool. Mirrors `main()` in the TypeScript original, minus process
/// exit handling (left to `main.rs`).
pub fn run(argv: &[String]) -> Result<Outcome, String> {
    if argv.iter().any(|a| a == "--self-test") {
        return Ok(Outcome::SelfTest(self_test()?));
    }
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

    let source_path = root().join("assets/code").join(format!("{}_overlay.s", options.overlay));
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

#[cfg(test)]
mod tests {
    use super::*;

    fn s(parts: &[&str]) -> Vec<String> {
        parts.iter().map(|p| p.to_string()).collect()
    }

    #[test]
    fn pool_target_rounding() {
        assert_eq!(pool_target(0, 0x4800), 4);
        assert_eq!(pool_target(2, 0x4800), 4);
        assert_eq!(pool_target(0, 0x4801), 8);
    }

    #[test]
    fn end_bound_is_not_dropped() {
        let bounded = options_of(&s(&["resource_3b9", "1cd4", "23e0"])).unwrap();
        assert_eq!(bounded.offset, 0x1cd4);
        assert_eq!(bounded.length, 0x23e0 - 0x1cd4);
        let prefixed = options_of(&s(&["resource_3b9", "1cd4", "0x23e0"])).unwrap();
        assert_eq!(prefixed.length, 0x23e0 - 0x1cd4);
        let absolute = options_of(&s(&["resource_3b9", "0x02001cd4", "0x020023e0", "--annotate"])).unwrap();
        assert_eq!(absolute.offset, 0x1cd4);
        assert_eq!(absolute.length, 0x23e0 - 0x1cd4);
        assert!(absolute.annotate);
        assert_eq!(options_of(&s(&["resource_3b9", "1cd4", "-n", "1804"])).unwrap().length, 1804);
        assert_eq!(options_of(&s(&["resource_3b9", "1cd4"])).unwrap().length, 0);
    }

    #[test]
    fn bad_arguments_are_rejected() {
        assert!(options_of(&s(&["resource_3b9", "1cd4", "23e0", "-n", "1804"])).is_err());
        assert!(options_of(&s(&["resource_3b9", "1cd4", "1cd4"])).is_err());
        assert!(options_of(&s(&["resource_3b9", "1cd4", "11ac"])).is_err());
        assert!(options_of(&s(&["resource_3b9", "1cd4", "zzz"])).is_err());
        assert!(options_of(&s(&["resource_3b9", "1cd4", "23e0", "2400"])).is_err());
    }

    #[test]
    fn self_test_passes() {
        assert_eq!(self_test().unwrap(), "overlay show self-test passed");
    }
}
