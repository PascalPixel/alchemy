//! Thumb instruction encodings and relocation-site scanning, without game policy.
pub fn bl_displacement(pair: &[u8]) -> Option<i32> {
    let high = u16::from_le_bytes(pair.get(..2)?.try_into().ok()?);
    let low = u16::from_le_bytes(pair.get(2..4)?.try_into().ok()?);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return None;
    }
    let value = (i32::from(high & 0x7ff) << 12) | (i32::from(low & 0x7ff) << 1);
    Some((value << 9) >> 9)
}
/// A Thumb relocation-bearing site: kind (`b'B'` call, `b'L'` literal load),
/// instruction offset, affected byte offset, and the referenced value.
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct Reference(pub u8, pub usize, pub usize, pub u32);
/// Mask the relocation-bearing bytes of a Thumb region that starts at the
/// absolute address `base`: `bl` halfword pairs and the literal words reached
/// by PC-relative loads. Returns the mask and the decoded reference sites.
pub fn relocation_info(bytes: &[u8], base: u64) -> (Vec<bool>, Vec<Reference>) {
    let mut mask = vec![false; bytes.len()];
    let mut references = Vec::new();
    for at in (0..bytes.len().saturating_sub(3)).step_by(2) {
        if let Some(delta) = bl_displacement(&bytes[at..at + 4]) {
            mask[at..at + 4].fill(true);
            references.push(Reference(
                b'B',
                at,
                at,
                (base as i64 + at as i64 + 4 + i64::from(delta)) as u32,
            ));
        }
    }
    for at in (0..bytes.len().saturating_sub(1)).step_by(2) {
        let instruction = u16::from_le_bytes([bytes[at], bytes[at + 1]]);
        if instruction & 0xf800 != 0x4800 {
            continue;
        }
        let pc = (base as usize + at + 4) & !3;
        let target = pc + usize::from(instruction & 0xff) * 4;
        let Some(literal) = target.checked_sub(base as usize) else {
            continue;
        };
        if literal + 4 <= mask.len() {
            mask[literal..literal + 4].fill(true);
            references.push(Reference(
                b'L',
                at,
                literal,
                u32::from_le_bytes(bytes[literal..literal + 4].try_into().unwrap()),
            ));
        }
    }
    (mask, references)
}
#[cfg(test)]
mod tests {
    use super::{bl_displacement, relocation_info};
    #[test]
    fn calls_decode_both_signed_extremes_and_all_low_bits() {
        for upper in [0u16, 0x3ff, 0x400, 0x7ff] {
            for lower in 0..=0x7ffu16 {
                let mut pair = [0; 4];
                pair[..2].copy_from_slice(&(0xf000 | upper).to_le_bytes());
                pair[2..].copy_from_slice(&(0xf800 | lower).to_le_bytes());
                let unsigned = (i32::from(upper) << 12) | (i32::from(lower) << 1);
                let expected = if upper >= 0x400 {
                    unsigned - 0x800000
                } else {
                    unsigned
                };
                assert_eq!(bl_displacement(&pair), Some(expected));
            }
        }
        for length in 0..4 {
            assert_eq!(bl_displacement(&[0x00, 0xf0, 0x00, 0xf8][..length]), None);
        }
        for pair in [[0, 0, 0, 0xf8], [0, 0xf0, 0, 0], [0, 0xf0, 0, 0xe8]] {
            assert_eq!(bl_displacement(&pair), None);
        }
    }

    #[test]
    fn relocation_literals_respect_halfword_aligned_image_bases() {
        let bytes = [0, 0x48, 1, 2, 3, 4];
        let (mask, sites) = relocation_info(&bytes, 0x08000002);
        assert_eq!(mask, [false, false, true, true, true, true]);
        assert_eq!((sites[0].1, sites[0].2, sites[0].3), (0, 2, 0x04030201));
        assert!(relocation_info(&bytes[..5], 0x08000002).1.is_empty());
    }
    #[test]
    fn masks_thumb_calls_and_reached_literals() {
        let bytes = [0x00, 0xf0, 0x00, 0xf8, 0x00, 0x48, 0x70, 0x47, 1, 2, 3, 4];
        let (mask, references) = relocation_info(&bytes, 0x0200_0000);
        assert_eq!(
            mask,
            [true, true, true, true, false, false, false, false, true, true, true, true]
        );
        let core = mask.iter().filter(|masked| !**masked).count();
        assert_eq!(core, 4);
        assert_eq!(references.len(), 2);
        assert_eq!(references[0].3, 0x0200_0004);
        assert_eq!(references[1].3, 0x0403_0201);
    }
}
