use super::*;

pub(super) fn pixels(input: &Value, bank: &Value, rom: &[u8]) -> Result<Vec<u8>, String> {
    let width = address(&input["width"])?;
    let height = address(&input["height"])?;
    let fw = address(&bank["frame_width"])?;
    let fh = address(&bank["frame_height"])?;
    let columns = address(&bank["columns"])?;
    let frames = address(&bank["frames"])?;
    let directory = address(&bank["directory"])? - ROM_BASE;
    if fw == 0 || fh == 0 || columns == 0 || width != fw * columns || height % fh != 0 {
        return Err("raw sprite frame geometry differs".into());
    }
    let table = rom
        .get(directory..directory + (frames + 1) * 4)
        .ok_or("raw sprite directory outside ROM")?;
    let pointers = table
        .chunks_exact(4)
        .map(|bytes| u32::from_le_bytes(bytes.try_into().unwrap()) as usize)
        .collect::<Vec<_>>();
    if pointers[frames] != 0 || pointers[0] != address(&bank["address"])? {
        return Err("raw sprite directory terminator or base differs".into());
    }
    let mut output = vec![
        0;
        width
            .checked_mul(height)
            .ok_or("raw sprite sheet overflows")?
    ];
    for frame in 0..frames {
        let start = pointers[frame]
            .checked_sub(ROM_BASE)
            .ok_or("raw sprite pointer precedes ROM")?;
        let end = if frame + 1 == frames {
            directory
        } else {
            pointers[frame + 1]
                .checked_sub(ROM_BASE)
                .ok_or("raw sprite pointer precedes ROM")?
        };
        let bytes = rom.get(start..end).ok_or("raw sprite frame outside bank")?;
        let terminator = bytes
            .iter()
            .position(|b| *b == 0)
            .ok_or("raw sprite frame lacks terminator")?;
        let decoded = zero_skip(
            &bytes[..=terminator],
            fw.checked_mul(fh).ok_or("raw frame overflows")?,
        )?;
        if bytes[terminator + 1..].iter().any(|b| *b != 0) {
            return Err("raw sprite padding differs".into());
        }
        for row in 0..fh {
            let dst = (frame / columns * fh + row) * width + frame % columns * fw;
            output
                .get_mut(dst..dst + fw)
                .ok_or("raw frame outside sheet")?
                .copy_from_slice(&decoded[row * fw..(row + 1) * fw]);
        }
    }
    if sha256::hex(&output) != json_string(&input["decoded_sha256"], "raw sprite digest")? {
        return Err("raw sprite pixels differ".into());
    }
    Ok(output)
}
