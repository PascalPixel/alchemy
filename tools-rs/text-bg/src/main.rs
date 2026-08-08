use text_bg::{decode_entry, encode_entry, flip_tile, TextBgEntry};

fn main() {
    let entry = TextBgEntry {
        tile: 0x2a5,
        palette: 13,
        hflip: true,
        vflip: true,
    };
    let encoded = encode_entry(entry).expect("in-range entry must encode");
    assert_eq!(decode_entry(encoded), entry, "entry round-trip failed");

    let tile: Vec<u16> = (0..64).collect();
    let once = flip_tile(&tile, true, false, 8).expect("8x8 tile must flip");
    assert_eq!(
        flip_tile(&once, true, false, 8).unwrap(),
        tile,
        "hflip not reversible"
    );
    let once = flip_tile(&tile, false, true, 8).expect("8x8 tile must flip");
    assert_eq!(
        flip_tile(&once, false, true, 8).unwrap(),
        tile,
        "vflip not reversible"
    );

    println!("self-test=ok");
}
