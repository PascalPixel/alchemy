//! Native builder, exporter, and verifier for the Golden Sun runtime-support
//! data package.
//!
//! The JSON document and its six ROM regions are deliberately kept as a
//! structured source.  This crate mirrors `tools/make/runtime_support_data.ts`;
//! the TypeScript module remains available to `build_assets.ts` during the
//! migration.

use std::collections::{BTreeMap, HashSet};
use std::fs;
use std::path::{Path, PathBuf};

use serde_json::{Map, Value};

pub const ROM_BASE: u32 = 0x0800_0000;
pub const ROM_SIZE: usize = 0x0080_0000;
pub const RUNTIME_SUPPORT_ADDRESS: u32 = 0x0800_795c;
pub const RUNTIME_SUPPORT_END: u32 = 0x0800_9000;

const NUMBER_ADDRESS: u32 = 0x0800_795c;
const NUMBER_END: u32 = 0x0800_7994;
const TRANSFER_ADDRESS: u32 = 0x0800_79b0;
const TRANSFER_END: u32 = 0x0800_7a20;
const DIRECTORY_ADDRESS: u32 = 0x0800_7a0c;
const ALIGNMENT_ADDRESS: u32 = 0x0800_7c64;
const ALIGNMENT_END: u32 = 0x0800_9000;
const FLASH_REGIONS: [(u32, u32); 3] = [
    (0x0800_7aa4, 0x0800_7b14),
    (0x0800_7b3c, 0x0800_7bc4),
    (0x0800_7bcc, 0x0800_7c3c),
];

const TIMING_NAMES: [&str; 4] = ["flash_common", "flash_c2_1c", "flash_32_1b", "flash_1f_3d"];
const TIMING_ADDRESSES: [u32; 4] = [0x0800_7aa4, 0x0800_7b3c, 0x0800_7b54, 0x0800_7bcc];
const DEVICE_NAMES: [&str; 6] = [
    "generic",
    "bf_d4",
    "c2_1c",
    "32_1b",
    "1f_3d_4k",
    "1f_3d_128",
];
const DEVICE_ADDRESSES: [u32; 6] = [
    0x0800_7abc,
    0x0800_7ae8,
    0x0800_7b6c,
    0x0800_7b98,
    0x0800_7be4,
    0x0800_7c10,
];

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
    value
        .as_object()
        .ok_or_else(|| Error(format!("{label} must be an object")))
}

fn exact_keys(value: &Map<String, Value>, expected: &[&str], label: &str) -> Result<()> {
    let mut actual: Vec<&str> = value.keys().map(String::as_str).collect();
    let mut wanted = expected.to_vec();
    actual.sort_unstable();
    wanted.sort_unstable();
    if actual != wanted {
        return fail(format!("{label} fields differ"));
    }
    Ok(())
}

fn field<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a Value> {
    value
        .get(name)
        .ok_or_else(|| Error(format!("{label} is missing")))
}

fn string<'a>(value: &'a Map<String, Value>, name: &str, label: &str) -> Result<&'a str> {
    field(value, name, label)?
        .as_str()
        .ok_or_else(|| Error(format!("{label} must be a string")))
}

fn integer(value: &Value, minimum: i64, maximum: i64, label: &str) -> Result<i64> {
    let number = value
        .as_i64()
        .ok_or_else(|| Error(format!("{label} is outside the supported range")))?;
    if number < minimum || number > maximum {
        return fail(format!("{label} is outside the supported range"));
    }
    Ok(number)
}

fn uinteger(value: &Value, minimum: u64, maximum: u64, label: &str) -> Result<u64> {
    let number = value
        .as_u64()
        .ok_or_else(|| Error(format!("{label} is outside the supported range")))?;
    if number < minimum || number > maximum {
        return fail(format!("{label} is outside the supported range"));
    }
    Ok(number)
}

fn address(value: &Value, expected: u32, label: &str) -> Result<u32> {
    let wanted = format!("0x{expected:08x}");
    if value.as_str() != Some(&wanted) {
        return fail(format!("{label} differs"));
    }
    Ok(expected)
}

fn canonical_address(value: &Value, label: &str) -> Result<u32> {
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("{label} address is not canonical")))?;
    if text.len() != 10
        || !text.starts_with("0x080")
        || !text[5..]
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
    {
        return fail(format!("{label} address is not canonical"));
    }
    u32::from_str_radix(&text[2..], 16)
        .map_err(|_| Error(format!("{label} address is not canonical")))
}

fn byte_id(value: &Value, label: &str) -> Result<u8> {
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("{label} is not a canonical byte")))?;
    if text.len() != 4
        || !text.starts_with("0x")
        || !text[2..]
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
    {
        return fail(format!("{label} is not a canonical byte"));
    }
    u8::from_str_radix(&text[2..], 16)
        .map_err(|_| Error(format!("{label} is not a canonical byte")))
}

fn ascii<'a>(value: &'a Value, label: &str) -> Result<&'a str> {
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("{label} must contain printable ASCII")))?;
    if !text.bytes().all(|byte| (0x20..=0x7e).contains(&byte)) {
        return fail(format!("{label} must contain printable ASCII"));
    }
    Ok(text)
}

fn function_pointer(value: &Value, label: &str) -> Result<u32> {
    let text = value
        .as_str()
        .ok_or_else(|| Error(format!("{label} is not a canonical function symbol")))?;
    if text.len() != 13
        || !text.starts_with("Func_080")
        || !text[8..]
            .bytes()
            .all(|byte| byte.is_ascii_digit() || (b'a'..=b'f').contains(&byte))
    {
        return fail(format!("{label} is not a canonical function symbol"));
    }
    let address = u32::from_str_radix(&text[5..], 16)
        .map_err(|_| Error(format!("{label} is not a canonical function symbol")))?;
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

fn validate_document(value: &Value) -> Result<()> {
    let source = object(value, "runtime support source")?;
    exact_keys(
        source,
        &[
            "format",
            "kind",
            "number_format",
            "transfer_test",
            "flash_directory",
            "timing_sets",
            "flash_devices",
            "alignment_fill",
        ],
        "runtime support source",
    )?;
    if source.get("format").and_then(Value::as_i64) != Some(1)
        || source.get("kind").and_then(Value::as_str) != Some("golden-sun-runtime-support-data")
    {
        return fail("unsupported runtime support source");
    }

    let number = object(
        field(source, "number_format", "number format")?,
        "number format",
    )?;
    exact_keys(
        number,
        &[
            "address",
            "end",
            "hexadecimal_digits",
            "trailing_zero_bytes",
            "decimal_divisors",
        ],
        "number format",
    )?;
    address(
        field(number, "address", "number-format address")?,
        NUMBER_ADDRESS,
        "number-format address",
    )?;
    address(
        field(number, "end", "number-format end")?,
        NUMBER_END,
        "number-format end",
    )?;
    if string(number, "hexadecimal_digits", "hexadecimal digit table")? != "0123456789ABCDEF" {
        return fail("hexadecimal digit table differs");
    }
    if integer(
        field(
            number,
            "trailing_zero_bytes",
            "number-format trailing zero bytes",
        )?,
        0,
        16,
        "number-format trailing zero bytes",
    )? != 4
    {
        return fail("number-format alignment differs");
    }
    let divisors = field(number, "decimal_divisors", "decimal divisor table")?
        .as_array()
        .ok_or_else(|| Error("decimal divisor table differs".into()))?;
    let expected = [
        1_000_000_000_i64,
        100_000_000,
        10_000_000,
        1_000_000,
        100_000,
        10_000,
        1_000,
        100,
        10,
    ];
    if divisors.len() != expected.len()
        || divisors
            .iter()
            .zip(expected)
            .any(|(value, wanted)| value.as_i64() != Some(wanted))
    {
        return fail("decimal divisor table differs");
    }

    let transfer = object(
        field(source, "transfer_test", "transfer test")?,
        "transfer test",
    )?;
    exact_keys(
        transfer,
        &[
            "address",
            "end",
            "identity",
            "identity_field_bytes",
            "record_marker",
            "reserved_zero_bytes",
            "packets",
            "flash_version",
            "flash_version_field_bytes",
        ],
        "transfer test",
    )?;
    address(
        field(transfer, "address", "transfer-test address")?,
        TRANSFER_ADDRESS,
        "transfer-test address",
    )?;
    address(
        field(transfer, "end", "transfer-test end")?,
        TRANSFER_END,
        "transfer-test end",
    )?;
    ascii(
        field(transfer, "identity", "transfer identity")?,
        "transfer identity",
    )?;
    ascii(
        field(transfer, "record_marker", "transfer marker")?,
        "transfer marker",
    )?;
    ascii(
        field(transfer, "flash_version", "flash version")?,
        "flash version",
    )?;
    integer(
        field(transfer, "identity_field_bytes", "identity field bytes")?,
        1,
        32,
        "identity field bytes",
    )?;
    integer(
        field(transfer, "reserved_zero_bytes", "reserved zero bytes")?,
        0,
        32,
        "reserved zero bytes",
    )?;
    integer(
        field(
            transfer,
            "flash_version_field_bytes",
            "flash-version field bytes",
        )?,
        1,
        32,
        "flash-version field bytes",
    )?;
    let packets = field(transfer, "packets", "transfer packets")?
        .as_array()
        .ok_or_else(|| Error("transfer packet collection differs".into()))?;
    if packets.len() != 2 {
        return fail("transfer packet collection differs");
    }
    for (index, packet_value) in packets.iter().enumerate() {
        let packet = object(packet_value, &format!("transfer packet {index}"))?;
        exact_keys(
            packet,
            &["text", "field_bytes"],
            &format!("transfer packet {index}"),
        )?;
        ascii(
            field(packet, "text", &format!("transfer packet {index} text"))?,
            &format!("transfer packet {index} text"),
        )?;
        integer(
            field(
                packet,
                "field_bytes",
                &format!("transfer packet {index} field bytes"),
            )?,
            1,
            64,
            &format!("transfer packet {index} field bytes"),
        )?;
    }

    let directory = object(
        field(source, "flash_directory", "flash directory")?,
        "flash directory",
    )?;
    exact_keys(directory, &["address", "devices"], "flash directory")?;
    address(
        field(directory, "address", "flash-directory address")?,
        DIRECTORY_ADDRESS,
        "flash-directory address",
    )?;
    let directory_devices = field(directory, "devices", "flash-directory devices")?
        .as_array()
        .ok_or_else(|| Error("flash-directory device collection differs".into()))?;
    if directory_devices.len() != 5
        || directory_devices
            .iter()
            .any(|value| value.as_str().is_none())
    {
        return fail("flash-directory device collection differs");
    }

    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let devices = object(
        field(source, "flash_devices", "flash devices")?,
        "flash devices",
    )?;
    if timings.len() != 4 || devices.len() != 6 {
        return fail("flash catalog size differs");
    }
    let mut timing_addresses = HashSet::new();
    for (name, raw) in timings {
        if !is_identifier(name) {
            return fail("timing-set name is not canonical");
        }
        let timing = object(raw, &format!("timing set {name}"))?;
        exact_keys(timing, &["address", "slots"], &format!("timing set {name}"))?;
        let start = canonical_address(
            field(timing, "address", &format!("timing set {name} address"))?,
            &format!("timing set {name}"),
        )?;
        if !timing_addresses.insert(start) {
            return fail("timing-set address is duplicated");
        }
        let slots = field(timing, "slots", &format!("timing set {name} slots"))?
            .as_array()
            .ok_or_else(|| Error(format!("timing set {name} slot collection differs")))?;
        if slots.len() != 4 {
            return fail(format!("timing set {name} slot collection differs"));
        }
        for (slot_index, slot_value) in slots.iter().enumerate() {
            let slot = slot_value
                .as_array()
                .ok_or_else(|| Error(format!("timing set {name} slot {slot_index} differs")))?;
            if slot.len() != 3 {
                return fail(format!("timing set {name} slot {slot_index} differs"));
            }
            for (item_index, item) in slot.iter().enumerate() {
                integer(
                    item,
                    -0x8000,
                    0x7fff,
                    &format!("timing set {name} slot {slot_index}:{item_index}"),
                )?;
            }
        }
    }
    let mut device_addresses = HashSet::new();
    for (name, raw) in devices {
        if !is_identifier(name) {
            return fail("flash-device name is not canonical");
        }
        let device = object(raw, &format!("flash device {name}"))?;
        exact_keys(
            device,
            &[
                "address",
                "callbacks",
                "timing",
                "geometry",
                "wait_control",
                "manufacturer_id",
                "device_id",
            ],
            &format!("flash device {name}"),
        )?;
        let start = canonical_address(
            field(device, "address", &format!("flash device {name} address"))?,
            &format!("flash device {name}"),
        )?;
        if !device_addresses.insert(start) {
            return fail("flash-device address is duplicated");
        }
        let callbacks = object(
            field(
                device,
                "callbacks",
                &format!("flash device {name} callbacks"),
            )?,
            &format!("flash device {name} callbacks"),
        )?;
        exact_keys(
            callbacks,
            &["program_sector", "erase_chip", "erase_sector", "wait_write"],
            &format!("flash device {name} callbacks"),
        )?;
        for (role, symbol) in callbacks {
            function_pointer(symbol, &format!("flash device {name} {role}"))?;
        }
        let timing = string(device, "timing", &format!("flash device {name} timing"))?;
        if !timings.contains_key(timing) {
            return fail(format!("flash device {name} timing differs"));
        }
        let geometry = object(
            field(device, "geometry", &format!("flash device {name} geometry"))?,
            &format!("flash device {name} geometry"),
        )?;
        exact_keys(
            geometry,
            &[
                "total_bytes",
                "sector_bytes",
                "sector_shift",
                "sector_count",
                "top_sector",
            ],
            &format!("flash device {name} geometry"),
        )?;
        let total = uinteger(
            field(
                geometry,
                "total_bytes",
                &format!("flash device {name} total bytes"),
            )?,
            1,
            0xffff_ffff,
            &format!("flash device {name} total bytes"),
        )?;
        let sector = uinteger(
            field(
                geometry,
                "sector_bytes",
                &format!("flash device {name} sector bytes"),
            )?,
            1,
            0xffff,
            &format!("flash device {name} sector bytes"),
        )?;
        let shift = uinteger(
            field(
                geometry,
                "sector_shift",
                &format!("flash device {name} sector shift"),
            )?,
            0,
            31,
            &format!("flash device {name} sector shift"),
        )?;
        let count = uinteger(
            field(
                geometry,
                "sector_count",
                &format!("flash device {name} sector count"),
            )?,
            1,
            0xffff,
            &format!("flash device {name} sector count"),
        )?;
        uinteger(
            field(
                geometry,
                "top_sector",
                &format!("flash device {name} top sector"),
            )?,
            0,
            0xffff_ffff,
            &format!("flash device {name} top sector"),
        )?;
        if sector != (1_u64 << shift) || sector * count != total {
            return fail(format!("flash device {name} geometry is inconsistent"));
        }
        let waits = field(
            device,
            "wait_control",
            &format!("flash device {name} wait control"),
        )?
        .as_array()
        .ok_or_else(|| Error(format!("flash device {name} wait control differs")))?;
        if waits.len() != 2 {
            return fail(format!("flash device {name} wait control differs"));
        }
        for (index, wait) in waits.iter().enumerate() {
            uinteger(
                wait,
                0,
                0xffff,
                &format!("flash device {name} wait control {index}"),
            )?;
        }
        byte_id(
            field(
                device,
                "manufacturer_id",
                &format!("flash device {name} manufacturer ID"),
            )?,
            &format!("flash device {name} manufacturer ID"),
        )?;
        byte_id(
            field(
                device,
                "device_id",
                &format!("flash device {name} device ID"),
            )?,
            &format!("flash device {name} device ID"),
        )?;
    }
    let mut directory_names = HashSet::new();
    for name in directory_devices {
        let name = name.as_str().unwrap();
        if !directory_names.insert(name) || !devices.contains_key(name) {
            return fail("flash-directory device reference differs");
        }
    }

    let alignment = object(
        field(source, "alignment_fill", "alignment fill")?,
        "alignment fill",
    )?;
    exact_keys(alignment, &["address", "end", "value"], "alignment fill")?;
    address(
        field(alignment, "address", "alignment-fill address")?,
        ALIGNMENT_ADDRESS,
        "alignment-fill address",
    )?;
    address(
        field(alignment, "end", "alignment-fill end")?,
        ALIGNMENT_END,
        "alignment-fill end",
    )?;
    if alignment.get("value") != Some(&Value::from(0)) {
        return fail("alignment fill is not zero");
    }
    Ok(())
}

fn is_identifier(value: &str) -> bool {
    !value.is_empty()
        && value
            .bytes()
            .all(|byte| byte.is_ascii_lowercase() || byte.is_ascii_digit() || byte == b'_')
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

fn get_u16(input: &[u8], offset: usize) -> u16 {
    u16::from_le_bytes(input[offset..offset + 2].try_into().unwrap())
}

fn get_i16(input: &[u8], offset: usize) -> i16 {
    get_u16(input, offset) as i16
}

fn get_u32(input: &[u8], offset: usize) -> u32 {
    u32::from_le_bytes(input[offset..offset + 4].try_into().unwrap())
}

fn build_number_format(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let number = object(
        field(source, "number_format", "number format")?,
        "number format",
    )?;
    let mut output = vec![0; (NUMBER_END - NUMBER_ADDRESS) as usize];
    output[..16].copy_from_slice(
        string(number, "hexadecimal_digits", "hexadecimal digit table")?.as_bytes(),
    );
    let cursor = 20;
    for (index, divisor) in field(number, "decimal_divisors", "decimal divisor table")?
        .as_array()
        .unwrap()
        .iter()
        .enumerate()
    {
        put_u32(
            &mut output,
            cursor + index * 4,
            uinteger(divisor, 0, 0xffff_ffff, "decimal divisor")? as u32,
        );
    }
    if cursor + 36 != output.len() {
        return fail("number-format layout differs");
    }
    Ok(output)
}

fn timing_buffer(source: &Map<String, Value>, name: &str) -> Result<Vec<u8>> {
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let timing = object(
        timings
            .get(name)
            .ok_or_else(|| Error(format!("unknown timing set {name}")))?,
        &format!("timing set {name}"),
    )?;
    let slots = field(timing, "slots", "timing slots")?.as_array().unwrap();
    let mut output = vec![0; 24];
    for (slot_index, slot) in slots.iter().enumerate() {
        for (item_index, value) in slot.as_array().unwrap().iter().enumerate() {
            put_i16(
                &mut output,
                (slot_index * 3 + item_index) * 2,
                integer(value, -0x8000, 0x7fff, "timing slot")? as i16,
            );
        }
    }
    Ok(output)
}

fn device_buffer(source: &Map<String, Value>, name: &str) -> Result<Vec<u8>> {
    let devices = object(
        field(source, "flash_devices", "flash devices")?,
        "flash devices",
    )?;
    let device = object(
        devices
            .get(name)
            .ok_or_else(|| Error(format!("unknown flash device {name}")))?,
        &format!("flash device {name}"),
    )?;
    let callbacks = object(
        field(device, "callbacks", "device callbacks")?,
        "device callbacks",
    )?;
    let geometry = object(
        field(device, "geometry", "device geometry")?,
        "device geometry",
    )?;
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let timing_name = string(device, "timing", "device timing")?;
    let timing = object(timings.get(timing_name).unwrap(), "device timing")?;
    let mut output = vec![0; 44];
    for (index, role) in ["program_sector", "erase_chip", "erase_sector", "wait_write"]
        .iter()
        .enumerate()
    {
        put_u32(
            &mut output,
            index * 4,
            function_pointer(callbacks.get(*role).unwrap(), "device callback")?,
        );
    }
    put_u32(
        &mut output,
        16,
        canonical_address(field(timing, "address", "timing address")?, "timing")?,
    );
    put_u32(
        &mut output,
        20,
        uinteger(
            field(geometry, "total_bytes", "total bytes")?,
            0,
            0xffff_ffff,
            "total bytes",
        )? as u32,
    );
    put_u16(
        &mut output,
        24,
        uinteger(
            field(geometry, "sector_bytes", "sector bytes")?,
            0,
            0xffff,
            "sector bytes",
        )? as u16,
    );
    put_u16(
        &mut output,
        28,
        uinteger(
            field(geometry, "sector_shift", "sector shift")?,
            0,
            31,
            "sector shift",
        )? as u16,
    );
    put_u16(
        &mut output,
        30,
        uinteger(
            field(geometry, "sector_count", "sector count")?,
            0,
            0xffff,
            "sector count",
        )? as u16,
    );
    put_u32(
        &mut output,
        32,
        uinteger(
            field(geometry, "top_sector", "top sector")?,
            0,
            0xffff_ffff,
            "top sector",
        )? as u32,
    );
    let waits = field(device, "wait_control", "wait control")?
        .as_array()
        .unwrap();
    put_u16(
        &mut output,
        36,
        uinteger(&waits[0], 0, 0xffff, "wait control")? as u16,
    );
    put_u16(
        &mut output,
        38,
        uinteger(&waits[1], 0, 0xffff, "wait control")? as u16,
    );
    output[40] = byte_id(
        field(device, "manufacturer_id", "manufacturer ID")?,
        "manufacturer ID",
    )?;
    output[41] = byte_id(field(device, "device_id", "device ID")?, "device ID")?;
    Ok(output)
}

fn build_transfer(source: &Map<String, Value>) -> Result<Vec<u8>> {
    let transfer = object(
        field(source, "transfer_test", "transfer test")?,
        "transfer test",
    )?;
    let identity_width = integer(
        field(transfer, "identity_field_bytes", "identity width")?,
        1,
        32,
        "identity width",
    )? as usize;
    let marker = ascii(
        field(transfer, "record_marker", "transfer marker")?,
        "transfer marker",
    )?;
    let reserved = integer(
        field(transfer, "reserved_zero_bytes", "reserved bytes")?,
        0,
        32,
        "reserved bytes",
    )? as usize;
    let packets = field(transfer, "packets", "transfer packets")?
        .as_array()
        .unwrap();
    let version_width = integer(
        field(transfer, "flash_version_field_bytes", "version width")?,
        1,
        32,
        "version width",
    )? as usize;
    let mut output = vec![0; (TRANSFER_END - TRANSFER_ADDRESS) as usize];
    let mut cursor = 0;
    let identity = fixed_ascii(
        ascii(field(transfer, "identity", "identity")?, "identity")?,
        identity_width,
        "transfer identity",
    )?;
    output[cursor..cursor + identity.len()].copy_from_slice(&identity);
    cursor += identity_width;
    output[cursor..cursor + marker.len()].copy_from_slice(marker.as_bytes());
    cursor += marker.len() + reserved;
    for (index, packet_value) in packets.iter().enumerate() {
        let packet = object(packet_value, "transfer packet")?;
        let text = ascii(
            field(packet, "text", "packet text")?,
            &format!("transfer packet {index} text"),
        )?;
        let width = integer(
            field(packet, "field_bytes", "packet width")?,
            1,
            64,
            "packet width",
        )? as usize;
        let bytes = fixed_ascii(text, width, "transfer packet")?;
        output[cursor..cursor + width].copy_from_slice(&bytes);
        cursor += width;
    }
    let version = fixed_ascii(
        ascii(
            field(transfer, "flash_version", "flash version")?,
            "flash version",
        )?,
        version_width,
        "flash version",
    )?;
    output[cursor..cursor + version.len()].copy_from_slice(&version);
    cursor += version_width;
    if TRANSFER_ADDRESS + cursor as u32 != DIRECTORY_ADDRESS {
        return fail("transfer-test record does not reach its flash directory");
    }
    let directory = object(
        field(source, "flash_directory", "flash directory")?,
        "flash directory",
    )?;
    let devices = object(
        field(source, "flash_devices", "flash devices")?,
        "flash devices",
    )?;
    for (index, name) in field(directory, "devices", "directory devices")?
        .as_array()
        .unwrap()
        .iter()
        .enumerate()
    {
        let name = name.as_str().unwrap();
        let device = object(devices.get(name).unwrap(), "directory device")?;
        put_u32(
            &mut output,
            cursor + index * 4,
            canonical_address(field(device, "address", "device address")?, "device")?,
        );
    }
    cursor += 20;
    if cursor != output.len() {
        return fail("transfer-test layout differs");
    }
    Ok(output)
}

fn build_flash_regions(source: &Map<String, Value>) -> Result<BTreeMap<u32, Vec<u8>>> {
    let timings = object(field(source, "timing_sets", "timing sets")?, "timing sets")?;
    let devices = object(
        field(source, "flash_devices", "flash devices")?,
        "flash devices",
    )?;
    let mut pieces: Vec<(u32, Vec<u8>)> = Vec::new();
    for (name, timing) in timings {
        pieces.push((
            canonical_address(
                field(object(timing, "timing")?, "address", "timing address")?,
                "timing",
            )?,
            timing_buffer(source, name)?,
        ));
    }
    for (name, device) in devices {
        pieces.push((
            canonical_address(
                field(object(device, "device")?, "address", "device address")?,
                "device",
            )?,
            device_buffer(source, name)?,
        ));
    }
    pieces.sort_by_key(|(address, _)| *address);
    if pieces.len() != 10 {
        return fail("flash region membership differs");
    }
    let mut output = BTreeMap::new();
    for (start, end) in FLASH_REGIONS {
        let mut region = vec![0; (end - start) as usize];
        let mut cursor = start;
        for (address, bytes) in pieces
            .iter()
            .filter(|(address, _)| start <= *address && *address < end)
        {
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
    validate_document(value)?;
    let source = object(value, "runtime support source")?;
    let mut regions = BTreeMap::new();
    regions.insert(NUMBER_ADDRESS, build_number_format(source)?);
    regions.insert(TRANSFER_ADDRESS, build_transfer(source)?);
    regions.extend(build_flash_regions(source)?);
    regions.insert(
        ALIGNMENT_ADDRESS,
        vec![0; (ALIGNMENT_END - ALIGNMENT_ADDRESS) as usize],
    );
    let source_bytes = regions.values().map(Vec::len).sum::<usize>();
    if source_bytes != 5_548 {
        return fail("runtime-support source-byte total differs");
    }
    Ok(Built {
        regions,
        source_bytes,
    })
}

pub fn build_runtime_support_component(
    value: &Value,
    address: u32,
    size: usize,
) -> Result<Vec<u8>> {
    let built = build_runtime_support_data(value)?;
    let bytes = built
        .regions
        .get(&address)
        .ok_or_else(|| Error("runtime-support component extent differs".into()))?;
    if bytes.len() != size {
        return fail("runtime-support component extent differs");
    }
    Ok(bytes.clone())
}

pub fn verify_runtime_support_data(rom: &[u8], value: &Value) -> Result<Built> {
    if rom.len() != ROM_SIZE {
        return fail("canonical ROM must contain exactly 8 MiB");
    }
    let built = build_runtime_support_data(value)?;
    for (address, bytes) in &built.regions {
        let start = address
            .checked_sub(ROM_BASE)
            .ok_or_else(|| Error("runtime-support ROM range differs".into()))?
            as usize;
        if start + bytes.len() > rom.len() || rom[start..start + bytes.len()] != bytes[..] {
            return fail(format!("runtime-support region 0x{address:x} differs"));
        }
    }
    Ok(built)
}

fn read_range(rom: &[u8], address: u32, end: u32) -> Result<&[u8]> {
    let start = address
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("runtime-support ROM range differs".into()))? as usize;
    let end = end
        .checked_sub(ROM_BASE)
        .ok_or_else(|| Error("runtime-support ROM range differs".into()))? as usize;
    if end < start || end > rom.len() {
        return fail("runtime-support ROM range differs");
    }
    Ok(&rom[start..end])
}

fn trim_ascii(bytes: &[u8], label: &str) -> Result<String> {
    let end = bytes
        .iter()
        .position(|byte| *byte == 0)
        .unwrap_or(bytes.len());
    if !bytes[end..].iter().all(|byte| *byte == 0)
        || !bytes[..end].iter().all(|byte| (0x20..=0x7e).contains(byte))
    {
        return fail(format!("{label} is not printable ASCII"));
    }
    String::from_utf8(bytes[..end].to_vec())
        .map_err(|_| Error(format!("{label} is not printable ASCII")))
}

fn hex_byte(value: u8) -> Value {
    Value::String(format!("0x{value:02x}"))
}

fn hex_address(value: u32) -> Value {
    Value::String(format!("0x{value:08x}"))
}

fn function_symbol(value: u32) -> Value {
    Value::String(format!("Func_{:08x}", value & !1))
}

fn export_timing(rom: &[u8], index: usize) -> Result<Value> {
    let bytes = read_range(rom, TIMING_ADDRESSES[index], TIMING_ADDRESSES[index] + 24)?;
    let mut slots = Vec::new();
    for slot in 0..4 {
        slots.push(Value::Array(
            (0..3)
                .map(|item| Value::from(get_i16(bytes, (slot * 3 + item) * 2)))
                .collect(),
        ));
    }
    let mut map = Map::new();
    map.insert("address".into(), hex_address(TIMING_ADDRESSES[index]));
    map.insert("slots".into(), Value::Array(slots));
    Ok(Value::Object(map))
}

fn export_device(rom: &[u8], index: usize) -> Result<Value> {
    let address = DEVICE_ADDRESSES[index];
    let bytes = read_range(rom, address, address + 44)?;
    let mut callbacks = Map::new();
    for (offset, name) in [
        (0, "program_sector"),
        (4, "erase_chip"),
        (8, "erase_sector"),
        (12, "wait_write"),
    ] {
        callbacks.insert(name.into(), function_symbol(get_u32(bytes, offset)));
    }
    let timing_address = get_u32(bytes, 16) & !1;
    let timing = TIMING_ADDRESSES
        .iter()
        .position(|candidate| *candidate == timing_address)
        .map(|position| TIMING_NAMES[position].to_string())
        .ok_or_else(|| {
            Error(format!(
                "flash device {} timing differs",
                DEVICE_NAMES[index]
            ))
        })?;
    let (sector_bytes, sector_shift, sector_count) =
        (get_u16(bytes, 24), get_u16(bytes, 28), get_u16(bytes, 30));
    let mut geometry = Map::new();
    geometry.insert("total_bytes".into(), Value::from(get_u32(bytes, 20)));
    geometry.insert("sector_bytes".into(), Value::from(sector_bytes));
    geometry.insert("sector_shift".into(), Value::from(sector_shift));
    geometry.insert("sector_count".into(), Value::from(sector_count));
    geometry.insert("top_sector".into(), Value::from(get_u32(bytes, 32)));
    let mut map = Map::new();
    map.insert("address".into(), hex_address(address));
    map.insert("callbacks".into(), Value::Object(callbacks));
    map.insert("timing".into(), Value::String(timing));
    map.insert("geometry".into(), Value::Object(geometry));
    map.insert(
        "wait_control".into(),
        Value::Array(vec![
            Value::from(get_u16(bytes, 36)),
            Value::from(get_u16(bytes, 38)),
        ]),
    );
    map.insert("manufacturer_id".into(), hex_byte(bytes[40]));
    map.insert("device_id".into(), hex_byte(bytes[41]));
    Ok(Value::Object(map))
}

pub fn export_runtime_support_data(rom: &[u8]) -> Result<Value> {
    if rom.len() != ROM_SIZE {
        return fail("canonical ROM must contain exactly 8 MiB");
    }
    let number = read_range(rom, NUMBER_ADDRESS, NUMBER_END)?;
    if number[..16] != *b"0123456789ABCDEF" || number[16..20].iter().any(|byte| *byte != 0) {
        return fail("runtime-support number format differs");
    }
    let mut divisors = Vec::new();
    for offset in (20..56).step_by(4) {
        divisors.push(Value::from(get_u32(number, offset)));
    }
    let mut number_map = Map::new();
    number_map.insert("address".into(), hex_address(NUMBER_ADDRESS));
    number_map.insert("end".into(), hex_address(NUMBER_END));
    number_map.insert(
        "hexadecimal_digits".into(),
        Value::String("0123456789ABCDEF".into()),
    );
    number_map.insert("trailing_zero_bytes".into(), Value::from(4));
    number_map.insert("decimal_divisors".into(), Value::Array(divisors));

    let transfer = read_range(rom, TRANSFER_ADDRESS, TRANSFER_END)?;
    let packet_a = trim_ascii(&transfer[20..52], "transfer packet 0")?;
    let packet_b = trim_ascii(&transfer[52..80], "transfer packet 1")?;
    let mut packet0 = Map::new();
    packet0.insert("text".into(), Value::String(packet_a));
    packet0.insert("field_bytes".into(), Value::from(32));
    let mut packet1 = Map::new();
    packet1.insert("text".into(), Value::String(packet_b));
    packet1.insert("field_bytes".into(), Value::from(28));
    let mut transfer_map = Map::new();
    transfer_map.insert("address".into(), hex_address(TRANSFER_ADDRESS));
    transfer_map.insert("end".into(), hex_address(TRANSFER_END));
    transfer_map.insert(
        "identity".into(),
        Value::String(trim_ascii(&transfer[..8], "transfer identity")?),
    );
    transfer_map.insert("identity_field_bytes".into(), Value::from(8));
    transfer_map.insert(
        "record_marker".into(),
        Value::String(trim_ascii(&transfer[8..16], "transfer marker")?),
    );
    transfer_map.insert("reserved_zero_bytes".into(), Value::from(4));
    transfer_map.insert(
        "packets".into(),
        Value::Array(vec![Value::Object(packet0), Value::Object(packet1)]),
    );
    transfer_map.insert(
        "flash_version".into(),
        Value::String(trim_ascii(&transfer[80..92], "flash version")?),
    );
    transfer_map.insert("flash_version_field_bytes".into(), Value::from(12));

    let mut device_by_address = BTreeMap::new();
    for (index, address) in DEVICE_ADDRESSES.iter().enumerate() {
        device_by_address.insert(*address, DEVICE_NAMES[index]);
    }
    let mut directory = Vec::new();
    for index in 0..5 {
        let address = get_u32(transfer, 92 + index * 4);
        let name = device_by_address
            .get(&address)
            .ok_or_else(|| Error("flash-directory device reference differs".into()))?;
        directory.push(Value::String((*name).into()));
    }
    let mut directory_map = Map::new();
    directory_map.insert("address".into(), hex_address(DIRECTORY_ADDRESS));
    directory_map.insert("devices".into(), Value::Array(directory));

    let mut timings = Map::new();
    for (index, name) in TIMING_NAMES.iter().enumerate() {
        timings.insert((*name).into(), export_timing(rom, index)?);
    }
    let mut devices = Map::new();
    for (index, name) in DEVICE_NAMES.iter().enumerate() {
        devices.insert((*name).into(), export_device(rom, index)?);
    }
    let alignment = read_range(rom, ALIGNMENT_ADDRESS, ALIGNMENT_END)?;
    if alignment.iter().any(|byte| *byte != 0) {
        return fail("alignment fill is not zero");
    }
    let mut alignment_map = Map::new();
    alignment_map.insert("address".into(), hex_address(ALIGNMENT_ADDRESS));
    alignment_map.insert("end".into(), hex_address(ALIGNMENT_END));
    alignment_map.insert("value".into(), Value::from(0));

    let mut root = Map::new();
    root.insert("format".into(), Value::from(1));
    root.insert(
        "kind".into(),
        Value::String("golden-sun-runtime-support-data".into()),
    );
    root.insert("number_format".into(), Value::Object(number_map));
    root.insert("transfer_test".into(), Value::Object(transfer_map));
    root.insert("flash_directory".into(), Value::Object(directory_map));
    root.insert("timing_sets".into(), Value::Object(timings));
    root.insert("flash_devices".into(), Value::Object(devices));
    root.insert("alignment_fill".into(), Value::Object(alignment_map));
    let source = Value::Object(root);
    let built = build_runtime_support_data(&source)?;
    for (address, bytes) in built.regions {
        let original = read_range(rom, address, address + bytes.len() as u32)?;
        if original != bytes {
            return fail("exported runtime-support data does not round-trip");
        }
    }
    Ok(source)
}

fn reject_duplicate_keys(text: &str) -> Result<()> {
    struct Scanner<'a> {
        bytes: &'a [u8],
        position: usize,
    }
    impl<'a> Scanner<'a> {
        fn space(&mut self) {
            while self.position < self.bytes.len()
                && self.bytes[self.position].is_ascii_whitespace()
            {
                self.position += 1;
            }
        }
        fn string(&mut self) -> Result<String> {
            let start = self.position;
            if self.bytes.get(self.position) != Some(&b'"') {
                return fail("runtime-support JSON string is invalid");
            }
            self.position += 1;
            while self.position < self.bytes.len() {
                let byte = self.bytes[self.position];
                self.position += 1;
                if byte == b'"' {
                    let raw = std::str::from_utf8(&self.bytes[start..self.position])
                        .map_err(|_| Error("runtime-support JSON string is invalid".into()))?;
                    return serde_json::from_str(raw)
                        .map_err(|_| Error("runtime-support JSON string is invalid".into()));
                }
                if byte == b'\\' {
                    let escaped =
                        self.bytes.get(self.position).copied().ok_or_else(|| {
                            Error("runtime-support JSON string is invalid".into())
                        })?;
                    self.position += 1;
                    if escaped == b'u' {
                        for _ in 0..4 {
                            if !self
                                .bytes
                                .get(self.position)
                                .is_some_and(u8::is_ascii_hexdigit)
                            {
                                return fail("runtime-support JSON string is invalid");
                            }
                            self.position += 1;
                        }
                    } else if !b"\"\\/bfnrt".contains(&escaped) {
                        return fail("runtime-support JSON string is invalid");
                    }
                } else if byte < 0x20 {
                    return fail("runtime-support JSON string is invalid");
                }
            }
            fail("runtime-support JSON string is unterminated")
        }
        fn value(&mut self) -> Result<()> {
            self.space();
            match self.bytes.get(self.position) {
                Some(b'{') => self.object(),
                Some(b'[') => self.array(),
                Some(b'"') => {
                    self.string()?;
                    Ok(())
                }
                Some(_) => {
                    while self.position < self.bytes.len()
                        && !b" \t\r\n,]}".contains(&self.bytes[self.position])
                    {
                        self.position += 1;
                    }
                    Ok(())
                }
                None => fail("runtime-support JSON value is missing"),
            }
        }
        fn object(&mut self) -> Result<()> {
            self.position += 1;
            self.space();
            let mut keys = HashSet::new();
            if self.bytes.get(self.position) == Some(&b'}') {
                self.position += 1;
                return Ok(());
            }
            loop {
                self.space();
                let key = self.string()?;
                if !keys.insert(key) {
                    return fail("runtime-support JSON has duplicate key");
                }
                self.space();
                if self.bytes.get(self.position) != Some(&b':') {
                    return fail("runtime-support JSON object colon is missing");
                }
                self.position += 1;
                self.value()?;
                self.space();
                match self.bytes.get(self.position) {
                    Some(b'}') => {
                        self.position += 1;
                        return Ok(());
                    }
                    Some(b',') => self.position += 1,
                    _ => return fail("runtime-support JSON object separator is invalid"),
                }
            }
        }
        fn array(&mut self) -> Result<()> {
            self.position += 1;
            self.space();
            if self.bytes.get(self.position) == Some(&b']') {
                self.position += 1;
                return Ok(());
            }
            loop {
                self.value()?;
                self.space();
                match self.bytes.get(self.position) {
                    Some(b']') => {
                        self.position += 1;
                        return Ok(());
                    }
                    Some(b',') => self.position += 1,
                    _ => return fail("runtime-support JSON array separator is invalid"),
                }
            }
        }
    }
    let mut scanner = Scanner {
        bytes: text.as_bytes(),
        position: 0,
    };
    scanner.value()?;
    scanner.space();
    if scanner.position != scanner.bytes.len() {
        return fail("runtime-support JSON has trailing input");
    }
    Ok(())
}

pub fn parse_runtime_support_source(text: &str) -> Result<Value> {
    reject_duplicate_keys(text)?;
    let value: Value = serde_json::from_str(text).map_err(|error| Error(error.to_string()))?;
    validate_document(&value)?;
    if !canonical_json::is_canonical_json_text(text, &value) {
        return fail("runtime-support source is not canonical JSON text");
    }
    Ok(value)
}

fn source_path() -> PathBuf {
    Path::new(env!("CARGO_MANIFEST_DIR"))
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .join("assets/data/runtime_support.json")
}

fn read_source(path: &Path) -> Result<Value> {
    let text = fs::read_to_string(path).map_err(|error| Error(error.to_string()))?;
    parse_runtime_support_source(&text)
}

fn write_file(path: &Path, data: &[u8]) -> Result<()> {
    if let Some(parent) = path.parent() {
        fs::create_dir_all(parent).map_err(|error| Error(error.to_string()))?;
    }
    fs::write(path, data).map_err(|error| Error(error.to_string()))
}

pub fn self_test() -> Result<()> {
    let source = read_source(&source_path())?;
    let built = build_runtime_support_data(&source)?;
    if built.source_bytes != 5_548
        || built.regions.len() != 6
        || built.regions.get(&NUMBER_ADDRESS).map(Vec::len) != Some(56)
        || built.regions[&ALIGNMENT_ADDRESS]
            .iter()
            .any(|byte| *byte != 0)
    {
        return fail("runtime-support builder self-test failed");
    }
    let canonical = format!("{}\n", canonical_json::canonical_json(&source));
    if parse_runtime_support_source(&canonical)? != source {
        return fail("runtime-support canonical parser self-test failed");
    }
    for text in [
        canonical[..canonical.len() - 1].to_string(),
        format!("{canonical}null\n"),
        canonical.replace("  \"kind\":", "   \"kind\":"),
        canonical.replacen("  \"format\": 1,", "  \"format\": 1,\n  \"format\": 1,", 1),
    ] {
        if parse_runtime_support_source(&text).is_ok() {
            return fail("runtime-support canonical parser accepted invalid text");
        }
    }
    let mut bad = source.clone();
    bad.as_object_mut()
        .unwrap()
        .insert("extra".into(), Value::Bool(true));
    if build_runtime_support_data(&bad).is_ok() {
        return fail("runtime-support adversarial self-test failed");
    }
    let mut short = vec![0; ROM_SIZE - 1];
    if verify_runtime_support_data(&short, &source).is_ok() {
        return fail("runtime-support ROM-size self-test failed");
    }
    short.clear();
    Ok(())
}

pub fn run(args: Vec<String>) -> Result<()> {
    const USAGE: &str = "usage: runtime_support_data.ts {export ROM -o SOURCE|verify ROM SOURCE|build SOURCE --address ADDRESS --size SIZE -o FILE|--self-test}";
    if args.is_empty() || args == ["-h"] || args == ["--help"] {
        println!("{USAGE}");
        return Ok(());
    }
    if args == ["--self-test"] {
        self_test()?;
        println!("self-test=ok");
        return Ok(());
    }
    match args.first().map(String::as_str) {
        Some("export") => {
            if args.len() < 4 {
                return fail(USAGE);
            }
            let rom = fs::read(&args[1]).map_err(|error| Error(error.to_string()))?;
            let output = option(&args[2..], "-o")?
                .or(option(&args[2..], "--output")?)
                .ok_or_else(|| Error("export output is required".into()))?;
            let source = export_runtime_support_data(&rom)?;
            write_file(
                Path::new(&output),
                format!("{}\n", canonical_json::canonical_json(&source)).as_bytes(),
            )?;
            println!("identical=true regions=6 source_bytes=5548");
            Ok(())
        }
        Some("verify") if args.len() == 3 => {
            let rom = fs::read(&args[1]).map_err(|error| Error(error.to_string()))?;
            let source = read_source(Path::new(&args[2]))?;
            let built = verify_runtime_support_data(&rom, &source)?;
            println!(
                "identical=true regions={} source_bytes={}",
                built.regions.len(),
                built.source_bytes
            );
            Ok(())
        }
        Some("build") => {
            if args.len() < 3 {
                return fail(USAGE);
            }
            let source_path = Path::new(&args[1]);
            let mut address = None;
            let mut size = None;
            let mut output = None;
            let mut index = 2;
            while index < args.len() {
                let flag = &args[index];
                let value = args
                    .get(index + 1)
                    .ok_or_else(|| Error(format!("{flag} requires a value")))?;
                match flag.as_str() {
                    "--address" => {
                        if address.is_some() {
                            return fail("--address may only be supplied once");
                        }
                        address = Some(parse_cli_integer(value, "build address")?);
                    }
                    "--size" => {
                        if size.is_some() {
                            return fail("--size may only be supplied once");
                        }
                        size = Some(parse_cli_integer(value, "build size")? as usize);
                    }
                    "-o" | "--output" => {
                        if output.is_some() {
                            return fail("build output may only be supplied once");
                        }
                        output = Some(value.clone());
                    }
                    _ => return fail(format!("unknown build option {flag}")),
                }
                index += 2;
            }
            let address = address.ok_or_else(|| {
                Error("build requires exactly one --address, --size, and -o/--output".into())
            })?;
            let size = size.ok_or_else(|| {
                Error("build requires exactly one --address, --size, and -o/--output".into())
            })?;
            let output = output.ok_or_else(|| {
                Error("build requires exactly one --address, --size, and -o/--output".into())
            })?;
            if source_path == Path::new(&output) {
                return fail("build output must differ from its source");
            }
            let source = read_source(source_path)?;
            let bytes = build_runtime_support_component(&source, address, size)?;
            write_file(Path::new(&output), &bytes)?;
            println!("bytes={}", bytes.len());
            Ok(())
        }
        _ => fail(USAGE),
    }
}

fn option(args: &[String], flag: &str) -> Result<Option<String>> {
    if let Some(index) = args.iter().position(|value| value == flag) {
        return args
            .get(index + 1)
            .cloned()
            .map(Some)
            .ok_or_else(|| Error(format!("{flag} requires a value")));
    }
    Ok(None)
}

fn parse_cli_integer(value: &str, label: &str) -> Result<u32> {
    let digits = value.strip_prefix("0x").unwrap_or(value);
    if digits.is_empty()
        || !digits.bytes().all(|byte| {
            byte.is_ascii_digit() || (value.starts_with("0x") && (b'a'..=b'f').contains(&byte))
        })
    {
        return fail(format!("{label} is not a canonical nonnegative integer"));
    }
    u32::from_str_radix(digits, if value.starts_with("0x") { 16 } else { 10 })
        .map_err(|_| Error(format!("{label} is outside the supported range")))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn canonical_source_builds_all_regions() {
        let source = read_source(&source_path()).unwrap();
        let built = build_runtime_support_data(&source).unwrap();
        assert_eq!(built.source_bytes, 5_548);
        assert_eq!(built.regions.len(), 6);
    }

    #[test]
    fn exported_rom_document_round_trips_when_available() {
        let root = Path::new(env!("CARGO_MANIFEST_DIR"))
            .parent()
            .unwrap()
            .parent()
            .unwrap();
        let rom = root.join("baserom.gba");
        if rom.is_file() {
            let bytes = fs::read(rom).unwrap();
            let source = export_runtime_support_data(&bytes).unwrap();
            assert_eq!(
                build_runtime_support_data(&source).unwrap().source_bytes,
                5_548
            );
        }
    }

    #[test]
    fn duplicate_json_keys_are_rejected() {
        assert!(parse_runtime_support_source(r#"{"format":1,"format":1}"#).is_err());
    }
}
