//! `ovlpack OVERLAY.bin OVERLAY.lz` packs an overlay's image as Camelot's
//! packer did: each Thumb BL pair is stored with its target counted from
//! the block's start (the decoder's BL filter undoes it), then the block is
//! compressed with the packer's tagged LZ, one rule for every stream (W3).

use std::fs;
use std::process::ExitCode;

fn store_thumb_calls(block: &mut [u8]) {
    let half = |block: &[u8], at: usize| u32::from(u16::from_le_bytes([block[at], block[at + 1]]));
    let mut at = 2;
    while at + 2 <= block.len() {
        let low = half(block, at);
        let high = half(block, at - 2);
        at += 2;
        if low >> 11 != 31 || high >> 11 != 30 {
            continue;
        }
        let call = ((high & 0x7ff) << 12) | ((low & 0x7ff) << 1);
        let stored = call.wrapping_add((at - 2) as u32) & 0x7f_fffe;
        let high = 0xf000 | (stored >> 12);
        let low = 0xf800 | ((stored >> 1) & 0x7ff);
        block[at - 4..at - 2].copy_from_slice(&(high as u16).to_le_bytes());
        block[at - 2..at].copy_from_slice(&(low as u16).to_le_bytes());
    }
}

fn main() -> ExitCode {
    let args: Vec<String> = std::env::args().skip(1).collect();
    let [input, output] = args.as_slice() else {
        eprintln!("usage: ovlpack OVERLAY.bin OVERLAY.lz");
        return ExitCode::FAILURE;
    };
    let result = fs::read(input)
        .map_err(|error| format!("{input}: {error}"))
        .and_then(|mut block| {
            store_thumb_calls(&mut block);
            ags::lz::compress_tagged(&block, &ags::resource::PACKER)
        })
        .and_then(|packed| fs::write(output, packed).map_err(|error| format!("{output}: {error}")));
    match result {
        Ok(()) => ExitCode::SUCCESS,
        Err(error) => {
            eprintln!("ovlpack: {error}");
            ExitCode::FAILURE
        }
    }
}
