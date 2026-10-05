pub fn bl_displacement(pair: &[u8]) -> Option<i32> {
    let high = u16::from_le_bytes(pair.get(..2)?.try_into().ok()?);
    let low = u16::from_le_bytes(pair.get(2..4)?.try_into().ok()?);
    if high & 0xf800 != 0xf000 || low & 0xf800 != 0xf800 {
        return None;
    }
    let value = (i32::from(high & 0x7ff) << 12) | (i32::from(low & 0x7ff) << 1);
    Some((value << 9) >> 9)
}
