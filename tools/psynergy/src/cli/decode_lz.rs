//! `psynergy decode-lz`: decode one tagged LZ stream from an explicit file offset.
use psynergy::assets::lz;
use std::path::PathBuf;

pub const USAGE: &str = "usage: psynergy decode-lz INPUT --offset N [--max BYTES] [--out FILE]\n\
Dispatches on the stream's first byte: 0 general LZ, 1 palette LZ, 2 MTF4 LZ.\n\
Prints the tag, consumed input bytes and decoded bytes; --out writes the decoded image.\n\
--max bounds decoded output (default 0x1000000). No ROM layout or resource lookup.";
const DEFAULT_MAXIMUM: u64 = 0x100_0000;

fn number(value: &str, label: &str) -> Result<u64, String> {
    let (radix, digits) = value
        .strip_prefix("0x")
        .or_else(|| value.strip_prefix("0X"))
        .map_or((10, value), |digits| (16, digits));
    u64::from_str_radix(digits, radix).map_err(|_| format!("invalid {label}: {value}"))
}

/// Decode the stream starting at `offset`. Returns the tag, the end cursor
/// (exclusive, bit-reader lookahead trimmed by the codec) and the image.
pub fn decode(data: &[u8], offset: usize, maximum: u64) -> Result<(u8, usize, Vec<u8>), String> {
    let tag = *data.get(offset).ok_or_else(|| {
        format!(
            "offset 0x{offset:x} is past the input end 0x{:x}",
            data.len()
        )
    })?;
    let end = data.len();
    let (decoded, cursor) = match tag {
        0 => lz::decode_general(data, offset, end, maximum),
        1 => lz::decode_palette(data, offset + 1, end, maximum),
        2 => lz::decode_mtf4_lz(data, offset, end, maximum),
        other => return Err(format!("unsupported LZ tag {other} at 0x{offset:x}")),
    }
    .map_err(|error| error.to_string())?;
    Ok((tag, cursor, decoded))
}

pub fn run(arguments: &[String]) -> Result<String, String> {
    let input = arguments
        .first()
        .filter(|value| !value.starts_with("--"))
        .ok_or_else(|| USAGE.to_string())?;
    let (mut offset, mut maximum, mut out) = (None, None, None);
    let mut rest = arguments[1..].iter();
    while let Some(flag) = rest.next() {
        let value = rest
            .next()
            .ok_or_else(|| format!("{flag} needs a value\n{USAGE}"))?;
        let duplicate = match flag.as_str() {
            "--offset" => offset.replace(number(value, "--offset")?).is_some(),
            "--max" => maximum.replace(number(value, "--max")?).is_some(),
            "--out" => out.replace(PathBuf::from(value)).is_some(),
            other => return Err(format!("unknown flag {other}\n{USAGE}")),
        };
        if duplicate {
            return Err(format!("duplicate {flag}"));
        }
    }
    let offset = offset.ok_or_else(|| format!("--offset is required\n{USAGE}"))?;
    let offset =
        usize::try_from(offset).map_err(|_| "--offset exceeds address space".to_string())?;
    let data = std::fs::read(input).map_err(|error| format!("{input}: {error}"))?;
    let (tag, cursor, decoded) = decode(&data, offset, maximum.unwrap_or(DEFAULT_MAXIMUM))?;
    if let Some(path) = &out {
        std::fs::write(path, &decoded).map_err(|error| format!("{}: {error}", path.display()))?;
    }
    Ok(format!(
        "tag {tag}\noffset 0x{offset:x}\nconsumed 0x{:x}\ndecoded 0x{:x}\n",
        cursor - offset,
        decoded.len()
    ))
}

#[test]
fn rejects_unknown_tags_and_missing_offsets() {
    assert!(decode(&[7, 0, 0], 0, 16).unwrap_err().contains("tag 7"));
    assert!(decode(&[0], 4, 16).is_err());
    assert!(run(&["file".into()]).unwrap_err().contains("--offset"));
}
