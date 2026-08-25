//! Build-only encoder for the tracked runtime-support data package.

use std::collections::BTreeMap;
use std::fs;
use std::io::{self, Write};
use std::path::Path;

use serde_json::{Map, Value};

const NUMBER_ADDRESS: u32 = 0x0800_795c;
const NUMBER_END: u32 = 0x0800_7994;
const TRANSFER_ADDRESS: u32 = 0x0800_79b0;
const TRANSFER_END: u32 = 0x0800_7a20;
const DIRECTORY_ADDRESS: u32 = 0x0800_7a0c;
const ALIGNMENT_ADDRESS: u32 = 0x0800_7c64;
const ALIGNMENT_END: u32 = 0x0800_9000;
const FLASH_REGIONS: [(u32, u32); 3] = [(0x0800_7aa4, 0x0800_7b14), (0x0800_7b3c, 0x0800_7bc4), (0x0800_7bcc, 0x0800_7c3c)];

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Error(pub String);

impl std::fmt::Display for Error {
    fn fmt(&self, output: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        output.write_str(&self.0)
    }
}

impl std::error::Error for Error {}

type Result<T> = std::result::Result<T, Error>;

fn fail<T>(message: impl Into<String>) -> Result<T> {
    Err(Error(message.into()))
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct Built {
    pub regions: BTreeMap<u32, Vec<u8>>,
    pub source_bytes: usize,
}

fn object<'a>(value: &'a Value, label: &str) -> Result<&'a Map<String, Value>> {
    value.as_object().ok_or_else(|| Error(format!("{label} must be an object")))
}

fn field<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a Value> {
    value.get(name).ok_or_else(|| Error(format!("{label} is missing")))
}

fn string<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a str> {
    field(value, name, label)?.as_str().ok_or_else(|| Error(format!("{label} must be a string")))
}

fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let number = value.as_i64().ok_or_else(|| Error(format!("{label} is outside the supported range")))?;
    if number < minimum || number > maximum {
        return fail(format!("{label} is outside the supported range"));
    }
    Ok(number)
}

fn uinteger(value: &Value, minimum: u64, maximum: u64, label: &str) -> Result<u64> {
    let number = value.as_u64().ok_or_else(|| Error(format!("{label} is outside the supported range")))?;
    if number < minimum || number > maximum {
        return fail(format!("{label} is outside the supported range"));
    }
    Ok(number)
}

fn canonical_address(value: &Value, label: &str) -> Result<u32> {
    let text = value.as_str().ok_or_else(|| Error(format!("{label} address is not canonical")))?;
    if text.len() != 10 || !text.starts_with("0x080") || !text[5..].bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte)) {
        return fail(format!("{label} address is not canonical"));
    }
    u32::from_str_radix(&text[2..], 16).map_err(|_| Error(format!("{label} address is not canonical")))
}

fn byte_id(value: &Value, label: &str) -> Result<u8> {
    let text = value.as_str().ok_or_else(|| Error(format!("{label} is not a canonical byte")))?;
    if text.len() != 4 || !text.starts_with("0x") || !text[2..].bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte)) {
        return fail(format!("{label} is not a canonical byte"));
    }
    u8::from_str_radix(&text[2..], 16).map_err(|_| Error(format!("{label} is not a canonical byte")))
}

fn ascii<'a>(value: &'a Value, label: &str) -> Result<&'a str> {
    let text = value.as_str().ok_or_else(|| Error(format!("{label} must contain printable ASCII")))?;
    if !text.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) {
        return fail(format!("{label} must contain printable ASCII"));
    }
    Ok(text)
}

fn function_pointer(value: &Value, label: &str) -> Result<u32> {
    let text = value.as_str().ok_or_else(|| Error(format!("{label} is not a canonical function symbol")))?;
    if text.len() != 13 || !text.starts_with("Func_080") || !text[8..].bytes().all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte)) {
        return fail(format!("{label} is not a canonical function symbol"));
    }
    let address = u32::from_str_radix(&text[5..], 16).map_err(|_| Error(format!("{label} is not a canonical function symbol")))?;
    Ok(address | 1)
}

fn fixed_ascii(value: &str, width: usize, label: &str) -> Result<Vec<u8>> {
    let bytes = value.as_bytes();
    if bytes.len() > width {
        return fail(format!("{label} exceeds its field"));
    }
    let mut output = vec![0; width];
    output[..bytes.len()].copy_from_slice(bytes);
    Ok(output)
}

fn put_u16(output: &mut [u8], offset: usize, value: u16) {
    output[offset..offset + 2].copy_from_slice(&value.to_le_bytes());
}

fn put_i16(output: &mut [u8], offset: usize, value: i16) {
    put_u16(output, offset, value as u16);
}

fn put_u32(output: &mut [u8], offset: usize, value: u32) {
    output[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

fn build_number_format(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let number = object(field(source, "number_format", "number format")?, "number format")?;
    let mut output = vec![0; (NUMBER_END - NUMBER_ADDRESS) as usize];
    output[..16].copy_from_slice(string(number, "hexadecimal_digits", "hexadecimal digit table")?.as_bytes());
    let cursor = 20;
    for (index, divisor) in field(number, "decimal_divisors", "decimal divisor table")?.as_array().unwrap().iter().enumerate() {
        put_u32(&mut output, cursor + index * 4, uinteger(divisor, 0, 0xffff_ffff, "decimal divisor")? as u32);
    }
    if cursor + 36 != output.len() {
        return fail("number-format layout differs");
    }
    Ok(output)
}

fn timing_buffer(source: &Map<String, Value>, name: &str) -> Result<Vec<u8>> {
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let timing = object(timings.get(name).ok_or_else(|| Error(format!("unknown timing set {name}")))?, &format!("timing set {name}"))?;
    let slots = field(timing, "slots", "timing slots")?.as_array().unwrap();
    let mut output = vec![0; 24];
    for (slot_index, slot) in slots.iter().enumerate() {
        for (item_index, value) in slot.as_array().unwrap().iter().enumerate() {
            put_i16(&mut output, (slot_index * 3 + item_index) * 2, integer(value, -0x8000, 0x7fff, "timing slot")? as i16);
        }
    }
    Ok(output)
}

fn device_buffer(source: &Map<String, Value>, name: &str) -> Result<Vec<u8>> {
    let devices = object(field(source, "flash_devices", "flash devices")?, "flash devices")?;
    let device = object(devices.get(name).ok_or_else(|| Error(format!("unknown flash device {name}")))?, &format!("flash device {name}"))?;
    let callbacks = object(field(device, "callbacks", "device callbacks")?, "device callbacks")?;
    let geometry = object(field(device, "geometry", "device geometry")?, "device geometry")?;
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let timing_name = string(device, "timing", "device timing")?;
    let timing = object(timings.get(timing_name).unwrap(), "device timing")?;
    let mut output = vec![0; 44];
    for (index, role) in ["program_sector", "erase_chip", "erase_sector", "wait_write"].iter().enumerate() {
        put_u32(&mut output, index * 4, function_pointer(callbacks.get(*role).unwrap(), "device callback")?);
    }
    put_u32(&mut output, 16, canonical_address(field(timing, "address", "timing address")?, "timing")?);
    put_u32(&mut output, 20, uinteger(field(geometry, "total_bytes", "total bytes")?, 0, 0xffff_ffff, "total bytes")? as u32);
    put_u16(&mut output, 24, uinteger(field(geometry, "sector_bytes", "sector bytes")?, 0, 0xffff, "sector bytes")? as u16);
    put_u16(&mut output, 28, uinteger(field(geometry, "sector_shift", "sector shift")?, 0, 31, "sector shift")? as u16);
    put_u16(&mut output, 30, uinteger(field(geometry, "sector_count", "sector count")?, 0, 0xffff, "sector count")? as u16);
    put_u32(&mut output, 32, uinteger(field(geometry, "top_sector", "top sector")?, 0, 0xffff_ffff, "top sector")? as u32);
    let waits = field(device, "wait_control", "wait control")?.as_array().unwrap();
    put_u16(&mut output, 36, uinteger(&waits[0], 0, 0xffff, "wait control")? as u16);
    put_u16(&mut output, 38, uinteger(&waits[1], 0, 0xffff, "wait control")? as u16);
    output[40] = byte_id(field(device, "manufacturer_id", "manufacturer ID")?, "manufacturer ID")?;
    output[41] = byte_id(field(device, "device_id", "device ID")?, "device ID")?;
    Ok(output)
}

fn build_transfer(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let transfer = object(field(source, "transfer_test", "transfer test")?, "transfer test")?;
    let identity_width = integer(field(transfer, "identity_field_bytes", "identity width")?, 1, 32, "identity width")? as usize;
    let marker = ascii(field(transfer, "record_marker", "transfer marker")?, "transfer marker")?;
    let reserved = integer(field(transfer, "reserved_zero_bytes", "reserved bytes")?, 0, 32, "reserved bytes")? as usize;
    let packets = field(transfer, "packets", "transfer packets")?.as_array().unwrap();
    let version_width = integer(field(transfer, "flash_version_field_bytes", "version width")?, 1, 32, "version width")? as usize;
    let mut output = vec![0; (TRANSFER_END - TRANSFER_ADDRESS) as usize];
    let mut cursor = 0;
    let identity = fixed_ascii(ascii(field(transfer, "identity", "identity")?, "identity")?, identity_width, "transfer identity")?;
    output[cursor..cursor + identity.len()].copy_from_slice(&identity);
    cursor += identity_width;
    output[cursor..cursor + marker.len()].copy_from_slice(marker.as_bytes());
    cursor += marker.len() + reserved;
    for (index, packet_value) in packets.iter().enumerate() {
        let packet = object(packet_value, "transfer packet")?;
        let text = ascii(field(packet, "text", "packet text")?, &format!("transfer packet {index} text"))?;
        let width = integer(field(packet, "field_bytes", "packet width")?, 1, 64, "packet width")? as usize;
        let bytes = fixed_ascii(text, width, "transfer packet")?;
        output[cursor..cursor + width].copy_from_slice(&bytes);
        cursor += width;
    }
    let version = fixed_ascii(ascii(field(transfer, "flash_version", "flash version")?, "flash version")?, version_width, "flash version")?;
    output[cursor..cursor + version.len()].copy_from_slice(&version);
    cursor += version_width;
    if TRANSFER_ADDRESS + cursor as u32 != DIRECTORY_ADDRESS {
        return fail("transfer-test record does not reach its flash directory");
    }
    let directory = object(field(source, "flash_directory", "flash directory")?, "flash directory")?;
    let devices = object(field(source, "flash_devices", "flash devices")?, "flash devices")?;
    for (index, name) in field(directory, "devices", "directory devices")?.as_array().unwrap().iter().enumerate() {
        let name = name.as_str().unwrap();
        let device = object(devices.get(name).unwrap(), "directory device")?;
        put_u32(&mut output, cursor + index * 4, canonical_address(field(device, "address", "device address")?, "device")?);
    }
    cursor += 20;
    if cursor != output.len() {
        return fail("transfer-test layout differs");
    }
    Ok(output)
}

fn build_flash_regions(source: &Map<String, Value>) -> Result<BTreeMap<u32, Vec<u8>>> {
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let devices = object(field(source, "flash_devices", "flash devices")?, "flash devices")?;
    let mut pieces: Vec<(u32, Vec<u8>)> = Vec::new();
    for (name, timing) in timings {
        pieces.push((canonical_address(field(object(timing, "timing")?, "address", "timing address")?, "timing")?, timing_buffer(source, name)?));
    }
    for (name, device) in devices {
        pieces.push((canonical_address(field(object(device, "device")?, "address", "device address")?, "device")?, device_buffer(source, name)?));
    }
    pieces.sort_by_key(|(address, _)| *address);
    if pieces.len() != 10 {
        return fail("flash region membership differs");
    }
    let mut output = BTreeMap::new();
    for (start, end) in FLASH_REGIONS {
        let mut region = vec![0; (end - start) as usize];
        let mut cursor = start;
        for (address, bytes) in pieces.iter().filter(|(address, _)| start <= *address && *address < end) {
            if *address != cursor || *address + bytes.len() as u32 > end {
                return fail(format!("flash region 0x{start:x} has a gap or overlap"));
            }
            let offset = (*address - start) as usize;
            region[offset..offset + bytes.len()].copy_from_slice(bytes);
            cursor += bytes.len() as u32;
        }
        if cursor != end {
            return fail(format!("flash region 0x{start:x} is incomplete"));
        }
        output.insert(start, region);
    }
    Ok(output)
}

pub fn build_runtime_support_data(value: &Value) -> Result<Built> {
    let source = object(value, "runtime support source")?;
    let mut regions = BTreeMap::new();
    regions.insert(NUMBER_ADDRESS, build_number_format(source)?);
    regions.insert(TRANSFER_ADDRESS, build_transfer(source)?);
    regions.extend(build_flash_regions(source)?);
    regions.insert(ALIGNMENT_ADDRESS, vec![0; (ALIGNMENT_END - ALIGNMENT_ADDRESS) as usize]);
    let source_bytes = regions.values().map(Vec::len).sum::<usize>();
    if source_bytes != 5_548 {
        return fail("runtime-support source-byte total differs");
    }
    Ok(Built { regions, source_bytes })
}

pub fn build_runtime_support_component(value: &Value, address: u32, size: usize) -> Result<Vec<u8>> {
    let built = build_runtime_support_data(value)?;
    let bytes = built.regions.get(&address).ok_or_else(|| Error("runtime-support component extent differs".into()))?;
    if bytes.len() != size {
        return fail("runtime-support component extent differs");
    }
    Ok(bytes.clone())
}

pub fn parse_runtime_support_source(text: &str) -> Result<Value> {
    let value: Value = serde_json::from_str(text).map_err(|error| Error(error.to_string()))?;
    if !canonical_json::is_canonical_json_text(text, &value) {
        return fail("runtime-support source is not canonical JSON text");
    }
    Ok(value)
}

fn read_source(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|error| Error(error.to_string()))?;
    parse_runtime_support_source(&text)
}

pub fn run(args: Vec<String>) -> Result<()> {
    const USAGE: &str = "usage: runtime-support-data build-stdout SOURCE --address ADDRESS --size SIZE";
    if args.is_empty() || args == ["-h"] || args == ["--help"] {
        println!("{USAGE}");
        return Ok(());
    }
    if args.len() != 6 || args[0] != "build-stdout" {
        return fail(USAGE);
    }
    let source = read_source(Path::new(&args[1]))?;
    let address = parse_cli_integer(&args[3], "build address")?;
    let size = parse_cli_integer(&args[5], "build size")? as usize;
    let bytes = build_runtime_support_component(&source, address, size)?;
    io::stdout().write_all(&bytes).map_err(|error| Error(error.to_string()))?;
    Ok(())
}

fn parse_cli_integer(value: &str, label: &str) -> Result<u32> {
    let digits = value.strip_prefix("0x").unwrap_or(value);
    if digits.is_empty() || !digits.bytes().all(|byte| byte.is_ascii_digit() || (value.starts_with("0x") && (b'a'..=b'f').contains(&byte))) {
        return fail(format!("{label} is not a canonical nonnegative integer"));
    }
    u32::from_str_radix(digits, if value.starts_with("0x") { 16 } else { 10 }).map_err(|_| Error(format!("{label} is outside the supported range")))
}
