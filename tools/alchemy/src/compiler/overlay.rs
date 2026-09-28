//! The resource loader at 08002d5c rewrites every Thumb BL-shaped halfword pair,
//! including pairs in literal pools. Owner addresses are resource coordinates.
use psynergy::thumb::bl_displacement as displacement;

/// Where an overlay's resource coordinates start.
pub const RESOURCE_BASE: u32 = 0x0200_0000;
/// Where the resource loader places a code overlay.
pub const RUNTIME_BASE: u32 = 0x0200_8000;

fn transform(bytes: &[u8], offset: usize, encode: bool) -> Result<Vec<u8>, String> {
    if !offset.is_multiple_of(2) || !bytes.len().is_multiple_of(2) {
        return Err("overlay image and offset must be halfword aligned".into());
    }
    let mut result = bytes.to_vec();
    for suffix in (2..result.len()).step_by(2) {
        let Some(value) = displacement(&result[suffix - 2..suffix + 2]) else {
            continue;
        };
        let site = u32::try_from(
            offset
                .checked_add(suffix)
                .ok_or("overlay offset overflow")?,
        )
        .map_err(|_| "overlay offset exceeds address space")?;
        let value = if encode {
            (value as u32).wrapping_add(site)
        } else {
            (value as u32).wrapping_sub(site)
        };
        let high = 0xf000 | ((value >> 12) & 0x7ff) as u16;
        let low = 0xf800 | ((value >> 1) & 0x7ff) as u16;
        result[suffix - 2..suffix].copy_from_slice(&high.to_le_bytes());
        result[suffix..suffix + 2].copy_from_slice(&low.to_le_bytes());
    }
    Ok(result)
}

pub fn load(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    transform(bytes, offset, false)
}

/// The packer's form of a loaded image, which `load` undoes exactly.
#[cfg(test)]
fn encode(bytes: &[u8], offset: usize) -> Result<Vec<u8>, String> {
    let encoded = transform(bytes, offset, true)?;
    if load(&encoded, offset)? != bytes {
        return Err("overlay serialization failed the loader round trip".into());
    }
    Ok(encoded)
}

#[cfg(test)]
mod tests {
    use super::*;

    fn pair(value: i32) -> Vec<u8> {
        [
            0xf000 | (((value as u32) >> 12) & 0x7ff) as u16,
            0xf800 | (((value as u32) >> 1) & 0x7ff) as u16,
        ]
        .into_iter()
        .flat_map(u16::to_le_bytes)
        .collect()
    }

    #[test]
    fn loader_transforms_literals_and_preserves_ordinary_data() {
        let mut data = pair(0x1400);
        data.extend_from_slice(&0x02008101u32.to_le_bytes());
        let runtime = load(&data, 0x23fe).unwrap();
        assert_eq!(&runtime[..4], &pair(-0x1000));
        assert_eq!(&runtime[4..], &data[4..]);
        assert_eq!(encode(&runtime, 0x23fe).unwrap(), data);
    }

    #[test]
    fn codec_round_trips_signed_calls_at_different_owner_offsets() {
        for offset in [0, 2, 0x104, 0x2000, 0xfffe] {
            for value in [-0x400000, -0x1000, -2, 0, 2, 0x1400, 0x3ffffe] {
                let runtime = pair(value);
                assert_eq!(
                    load(&encode(&runtime, offset).unwrap(), offset).unwrap(),
                    runtime
                );
            }
        }
        assert!(load(&[0], 0).is_err());
        assert!(encode(&[0, 0], 1).is_err());
    }
}
